
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include "link/tangle.h"
#include "utilities/sigutils.h"

namespace regina {

namespace {
    struct SigData {
        int tangleStrand; // strand 0 or 1 in the tangle
        size_t crossing;
        int strand;
        int sign;

        bool operator < (const SigData& rhs) const {
            if (tangleStrand < rhs.tangleStrand) return true;
            if (tangleStrand > rhs.tangleStrand) return false;
            if (crossing < rhs.crossing) return true;
            if (crossing > rhs.crossing) return false;
            if (strand > rhs.strand) return true; /* upper first */
            if (strand < rhs.strand) return false;
            if (sign > rhs.sign) return true; /* positive first */
            return false;
        }
    };
}

int typeToInt(char type) {
    if (type == '-') {
        return 0;
    } else if (type == '|') {
        return 1;
    } else {
        return 2;
    }
}

char typeOfInt(int typeInt) {
    if (typeInt == 0) {
        return '-';
    } else if (typeInt == 1) {
        return '|';
    } else {
        return 'x';
    }
}

std::string Tangle::tangleSig(bool useReflection, bool useFlipping) const {
    // Minimise sequence: (crossing, strand, sign) ... (crossing, strand, sign)
    // Ordering: crossing by ID; strand upper first; sign positive first
    // Text: n t m c_1 c_2 ... c_2n [packed strand bits] [packed sign bits]
    // where t is the type of the tangle and m is the number of crossings on strand 0
    size_t n = crossings_.size();
    if (n == 0) {
        std::string ans;
        ans = Base64SigEncoding::encodeSingle(0);
        ans += Base64SigEncoding::encodeSingle(typeToInt(type()));
        return ans;
    }

    auto* best = new SigData[2 * n];
    size_t bestM = crossings_.size();
    auto* curr = new SigData[2 * n];
    size_t currM;

    // The image and preimage for each crossing.
    auto* image = new ssize_t[n];
    auto* preimage = new ssize_t[n];

    bool currBetter;
    StrandRef currStrand;
    size_t nextUnused;
    bool firstCrossing = true;

    for (int flipStart = 0; flipStart < 2; ++flipStart) {
        for (int flipDir = 0; flipDir < 2; ++flipDir) {
            for (int reflect = 0; reflect < 2; ++reflect) {
                int flipStrands = flipStart ^ flipDir ^ reflect;
                if (type() == 'x') {
                    // If the tangle is a diagonal tangle, then when you start
                    // from the opposite side, you need to also change which
                    // strand you start with so that you're still starting with
                    // the "upper" strand.
                    flipStart ^= flipDir;
                }
                int tangleStrand = 0;
                // TODO czinn: Do reflection optimization (we know longer strand should always be strand 0 so we can skip half the cases if they're not the same length)
                size_t m = 0;
                auto currStrand = flipDir ? end(flipStart) : begin(flipStart);
                if (!currStrand) {
                    tangleStrand = 1;
                    currStrand = flipDir ? end(!flipStart) : begin(!flipStart);
                    currM = 0;
                }

                std::fill(image, image + n, -1);
                std::fill(preimage, preimage + n, -1);

                // Map crossing start -> 0.
                image[currStrand.crossing()->index()] = 0;
                preimage[0] = currStrand.crossing()->index();
                nextUnused = 1;

                curr[0].tangleStrand = tangleStrand;
                curr[0].crossing = 0;
                curr[0].strand = currStrand.strand() ^ flipStrands;
                curr[0].sign = currStrand.crossing()->sign() * (reflect ? -1 : 1);

                if (firstCrossing || curr[0] < best[0]) {
                    currBetter = true;
                } else if (best[0] < curr[0]) {
                    goto noncanonical;
                } else {
                    currBetter = false;
                }

                for (size_t i = 1; i < 2 * n; i++) {
                    if (flipDir) {
                        currStrand = currStrand.prev();
                    } else {
                        currStrand = currStrand.next();
                    }
                    if (!currStrand) {
                        tangleStrand = 1;
                        currStrand = flipDir ? end(!flipStart) : begin(!flipStart);
                        currM = i;
                    }

                    if (image[currStrand.crossing()->index()] < 0) {
                        // This is a new crossing.
                        preimage[nextUnused] = currStrand.crossing()->index();
                        image[currStrand.crossing()->index()] = nextUnused++;
                    }

                    curr[i].tangleStrand = tangleStrand;
                    curr[i].crossing = image[currStrand.crossing()->index()];
                    curr[i].strand = currStrand.strand() ^ flipStrands;
                    curr[i].sign = currStrand.crossing()->sign() * (reflect ? -1 : 1);

                    if (!currBetter) {
                        if (curr[i] < best[i]) {
                            currBetter = true;
                        } else if (best[i] < curr[i]) {
                            goto noncanonical;
                        }
                    }
                }

                if (currBetter) {
                    std::swap(curr, best);
                    bestM = currM;
                    firstCrossing = false;
                }

                noncanonical:
                    ;

                if (!useReflection) {
                    break;
                }
            }
            
            if (!useFlipping) {
                break;
            }
        }

        if (!useFlipping) {
            break;
        }
    }

    delete[] preimage;
    delete[] image;
    delete[] curr;

    // Text: n t m c_1 c_2 ... c_2n [packed strand bits] [packed sign bits]
    // where t is the type of the tangle and m is the number of crossings on strand 0
    std::string ans;

    // Output chars per crossing index, if this is > 1.
    unsigned charsPerInt;
    if (n < 63)
        charsPerInt = 1;
    else {
        charsPerInt = 0;
        size_t tmp = n;
        while (tmp > 0) {
            tmp >>= 6;
            ++charsPerInt;
        }
        ans = Base64SigEncoding::encodeSingle(63);
        ans += Base64SigEncoding::encodeSingle(charsPerInt);
    }

    // Output crossings in order.
    Base64SigEncoding::encodeInt(ans, n, charsPerInt);
    ans += Base64SigEncoding::encodeSingle(typeToInt(type()));
    Base64SigEncoding::encodeInt(ans, bestM, charsPerInt);
    for (auto dat = best; dat != best + 2*n; ++dat)
        Base64SigEncoding::encodeInt(ans, dat->crossing, charsPerInt);

    // Output strands and signs, each as a packed sequence of bits.
    unsigned i, j;
    unsigned write;
    for (i = 0; i < 2 * n; i += 6) {
        write = 0;
        for (j = 0; j < 6 && i + j < 2 * n; ++j)
            if (best[i + j].strand)
                write |= (1 << j);
        ans += Base64SigEncoding::encodeSingle(write);
    }
    for (i = 0; i < 2 * n; i += 6) {
        write = 0;
        for (j = 0; j < 6 && i + j < 2 * n; ++j)
            if (best[i + j].sign > 0)
                write |= (1 << j);
        ans += Base64SigEncoding::encodeSingle(write);
    }

    delete[] best;
    return ans;
}

Tangle Tangle::fromTangleSig(const std::string& sig) {
    Tangle ans;

    const char* c = sig.c_str();

    // Skip any leading whitespace.
    while (*c && ::isspace(*c))
        ++c;

    // Find the end of the string.
    const char* end = c;
    while (*end && ! ::isspace(*end))
        ++end;

    // Initial check for invalid characters.
    const char* d;
    for (d = c; d != end; ++d)
        if (! Base64SigEncoding::isValid(*d))
            throw InvalidArgument("fromTangleSig(): invalid base64 character");
    for (d = end; *d; ++d)
        if (! ::isspace(*d))
            throw InvalidArgument(
                "fromTangleSig(): unexpected internal whitespace");

    unsigned charsPerInt;
    size_t n = Base64SigEncoding::decodeSingle(*c++);
    if (n < 63)
        charsPerInt = 1;
    else {
        if (c == end)
            throw InvalidArgument("fromTangleSig(): incomplete signature");
        charsPerInt = Base64SigEncoding::decodeSingle(*c++);
        if (c + charsPerInt > end)
            throw InvalidArgument("fromTangleSig(): incomplete signature");
        n = Base64SigEncoding::decodeInt<size_t>(c, charsPerInt);
        c += charsPerInt;
    }

    ans.type_ = typeOfInt(Base64SigEncoding::decodeSingle(*c++));

    if (n == 0) {
        // Zero-crossing tangle.
        if (c != end)
            throw InvalidArgument("fromTangleSig(): signature has "
                "extra characters");
        return ans;
    }

    size_t m = Base64SigEncoding::decodeInt<size_t>(c, charsPerInt);
    c += charsPerInt;

    if (end != c + 2 * n * charsPerInt + 2 * ((n + 2) / 3))
        throw InvalidArgument("fromTangleSig(): length of signature "
            "does not match number of crossings");

    auto* crossing = new size_t[2 * n];
    int* sign = new int[2 * n];
    int* strand = new int[2 * n];

    size_t i, j;

    for (i = 0; i < 2 * n; ++i) {
        crossing[i] = Base64SigEncoding::decodeInt<size_t>(c, charsPerInt);
        if (/* crossing[i] < 0 || */ crossing[i] >= n) {
            delete[] crossing;
            delete[] sign;
            delete[] strand;
            throw InvalidArgument("fromTangleSig(): "
                "invalid destination crossing");
        }
        c += charsPerInt;
    }

    unsigned bits;
    for (i = 0; i < 2 * n; i += 6) {
        bits = Base64SigEncoding::decodeSingle(*c++);
        for (j = 0; j < 6 && i + j < 2 * n; ++j) {
            strand[i + j] = (bits & 1);
            bits >>= 1;
        }
        if (bits) {
            delete[] crossing;
            delete[] sign;
            delete[] strand;
            throw InvalidArgument("fromTangleSig(): extraneous strand bits");
        }
    }
    for (i = 0; i < 2 * n; i += 6) {
        bits = Base64SigEncoding::decodeSingle(*c++);
        for (j = 0; j < 6 && i + j < 2 * n; ++j) {
            sign[i + j] = ((bits & 1) ? 1 : -1);
            bits >>= 1;
        }
        if (bits) {
            delete[] crossing;
            delete[] sign;
            delete[] strand;
            throw InvalidArgument("fromTangleSig(): extraneous sign bits");
        }
    }

    for (i = 0; i < n; ++i)
        ans.crossings_.push_back(new Crossing());

    Crossing* cr;
    int nextIdx;
    for (i = 0; i < 2 * n; ++i) {
        cr = ans.crossings_[crossing[i]];
        if (cr->sign_ == 0)
            cr->sign_ = sign[i];
        else if (cr->sign_ != sign[i]) {
            delete[] crossing;
            delete[] sign;
            delete[] strand;
            throw InvalidArgument("fromTangleSig(): inconsistent crossing signs");
        }

        if (cr->next_[strand[i]].crossing_) {
            delete[] crossing;
            delete[] sign;
            delete[] strand;
            throw InvalidArgument("fromTangleSig(): invalid outgoing connection");
        }
        nextIdx = (i == 2 * n - 1 || i == m - 1 ? -1 : i + 1);
        if (nextIdx != -1) {
            cr->next_[strand[i]].crossing_ = ans.crossings_[crossing[nextIdx]];
            cr->next_[strand[i]].strand_ = strand[nextIdx];

            if (ans.crossings_[crossing[nextIdx]]->prev_[strand[nextIdx]]) {
                delete[] crossing;
                delete[] sign;
                delete[] strand;
                throw InvalidArgument("fromTangleSig(): invalid incoming connection");
            }

            ans.crossings_[crossing[nextIdx]]->prev_[strand[nextIdx]].crossing_ =
                cr;
            ans.crossings_[crossing[nextIdx]]->prev_[strand[nextIdx]].strand_ =
                strand[i];
        } else {
            ans.end_[i >= m ? 1 : 0][1] = StrandRef { cr, strand[i] };
        }

        if (i == 0 || i == m) {
            ans.end_[i == 0 ? 0 : 1][0] = StrandRef { cr, strand[i] };
        }
    }

    delete[] crossing;
    delete[] sign;
    delete[] strand;
    return ans;
}

} // namespace regina
