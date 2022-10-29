
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

#include "link/link.h"
#include "utilities/sigutils.h"

namespace regina {

namespace {
    struct SigData {
        size_t crossing;
        int strand;
        int sign;

        bool operator < (const SigData& rhs) const {
            if (crossing < rhs.crossing) return true;
            if (crossing > rhs.crossing) return false;
            if (strand > rhs.strand) return true; /* upper first */
            if (strand < rhs.strand) return false;
            if (sign > rhs.sign) return true; /* positive first */
            return false;
        }
    };
}

std::string Link::knotSig(bool useReflection, bool useReverse) const {
    // Only defined for knots at present.
    if (components_.size() != 1)
        throw NotImplemented(
            "Knot signatures are only implemented for single-component links");

    // Minimise sequence: (crossing, strand, sign) ... (crossing, strand, sign)
    // Ordering: crossing by ID; strand upper first; sign positive first
    // Text: n c_1 c_2 ... c_2n [packed strand bits] [packed sign bits]
    size_t n = crossings_.size();
    auto* best = new SigData[2 * n];
    auto* curr = new SigData[2 * n];

    // The image and preimage for each crossing.
    auto* image = new ssize_t[n];
    auto* preimage = new ssize_t[n];

    bool currBetter;
    StrandRef currStrand;
    size_t nextUnused;
    int reflectSign; /* + becomes - */
    int reflectStrands; /* upper becomes lower */
    bool begin = true;
    for (reflectSign = 0; reflectSign < 2; ++reflectSign) {
        for (reflectStrands = 0; reflectStrands < 2; ++reflectStrands) {
            for (int reverse = 0; reverse < 2; ++reverse) {
                for (auto start : crossings_) {
                    // If we can reflect, then we can guarantee to start with
                    // a positive crossing.
                    if (useReflection &&
                            start->sign() == (reflectSign ? 1 : -1))
                        continue;

                    std::fill(image, image + n, -1);
                    std::fill(preimage, preimage + n, -1);

                    // Map crossing start -> 0.
                    image[start->index()] = 0;
                    preimage[0] = start->index();
                    nextUnused = 1;

                    currStrand = start->strand(reflectStrands ? 0 : 1);

                    curr[0].crossing = 0;
                    curr[0].strand = 1;
                    curr[0].sign = (reflectSign ?
                        - start->sign() : start->sign());

                    if (begin)
                        currBetter = true;
                    else if (curr[0] < best[0])
                        currBetter = true;
                    else if (best[0] < curr[0])
                        goto noncanonical;
                    else
                        currBetter = false;

                    for (size_t i = 1; i < 2 * n; ++i) {
                        if (reverse)
                            --currStrand;
                        else
                            ++currStrand;

                        if (image[currStrand.crossing()->index()] < 0) {
                            // This is a new crossing.
                            preimage[nextUnused] =
                                currStrand.crossing()->index();
                            image[currStrand.crossing()->index()] =
                                nextUnused++;
                        }

                        curr[i].crossing =
                            image[currStrand.crossing()->index()];
                        curr[i].strand = (reflectStrands ?
                            (currStrand.strand() ^ 1) :
                            currStrand.strand());
                        curr[i].sign = (reflectSign ?
                            - currStrand.crossing()->sign() :
                            currStrand.crossing()->sign());

                        if (! currBetter) {
                            if (curr[i] < best[i])
                                currBetter = true;
                            else if (best[i] < curr[i])
                                goto noncanonical;
                        }
                    }

                    if (currBetter) {
                        std::swap(curr, best);
                        begin = false;
                    }

                    noncanonical:
                        ;
                }

                if (! useReverse)
                    break;
            }

            // We always allow reflectStrands, since this is just a
            // rotation of the knot.
        }

        if (! useReflection)
            break;
    }

    delete[] preimage;
    delete[] image;
    delete[] curr;

    // Text: n c_1 c_2 ... c_2n [packed strand bits] [packed sign bits]
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

Link Link::fromKnotSig(const std::string& sig) {
    Link ans;
    Link::ChangeEventSpan span(ans);

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
            throw InvalidArgument("fromKnotSig(): invalid base64 character");
    for (d = end; *d; ++d)
        if (! ::isspace(*d))
            throw InvalidArgument(
                "fromKnotSig(): unexpected internal whitespace");

    unsigned charsPerInt;
    size_t n = Base64SigEncoding::decodeSingle(*c++);
    if (n < 63)
        charsPerInt = 1;
    else {
        if (c == end)
            throw InvalidArgument("fromKnotSig(): incomplete signature");
        charsPerInt = Base64SigEncoding::decodeSingle(*c++);
        if (c + charsPerInt > end)
            throw InvalidArgument("fromKnotSig(): incomplete signature");
        n = Base64SigEncoding::decodeInt<size_t>(c, charsPerInt);
        c += charsPerInt;
    }

    if (n == 0) {
        // Zero-crossing unknot.
        if (c != end)
            throw InvalidArgument("fromKnotSig(): signature has "
                "extra characters");
        ans.components_.emplace_back();
        return ans;
    }

    if (end != c + 2 * n * charsPerInt + 2 * ((n + 2) / 3))
        throw InvalidArgument("fromKnotSig(): length of signature "
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
            throw InvalidArgument("fromKnotSig(): "
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
            throw InvalidArgument("fromKnotSig(): extraneous strand bits");
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
            throw InvalidArgument("fromKnotSig(): extraneous sign bits");
        }
    }

    for (i = 0; i < n; ++i)
        ans.crossings_.push_back(new Crossing());

    Crossing* cr;
    size_t nextIdx;
    for (i = 0; i < 2 * n; ++i) {
        cr = ans.crossings_[crossing[i]];
        if (cr->sign_ == 0)
            cr->sign_ = sign[i];
        else if (cr->sign_ != sign[i]) {
            delete[] crossing;
            delete[] sign;
            delete[] strand;
            throw InvalidArgument("fromKnotSig(): inconsistent crossing signs");
        }

        if (cr->next_[strand[i]].crossing_) {
            delete[] crossing;
            delete[] sign;
            delete[] strand;
            throw InvalidArgument("fromKnotSig(): invalid outgoing connection");
        }
        nextIdx = (i == 2 * n - 1 ? 0 : i + 1);
        cr->next_[strand[i]].crossing_ = ans.crossings_[crossing[nextIdx]];
        cr->next_[strand[i]].strand_ = strand[nextIdx];

        if (ans.crossings_[crossing[nextIdx]]->prev_[strand[nextIdx]]) {
            delete[] crossing;
            delete[] sign;
            delete[] strand;
            throw InvalidArgument("fromKnotSig(): invalid incoming connection");
        }
        ans.crossings_[crossing[nextIdx]]->prev_[strand[nextIdx]].crossing_ =
            cr;
        ans.crossings_[crossing[nextIdx]]->prev_[strand[nextIdx]].strand_ =
            strand[i];
    }

    ans.components_.push_back(ans.crossings_[crossing[0]]->strand(strand[0]));

    delete[] crossing;
    delete[] sign;
    delete[] strand;
    return ans;
}

namespace {
    inline size_t fastMod(size_t k, size_t n) {
        if (k < n) return k;
        return k - n;
    }

    struct FastSigData {
        size_t crossingA;
        size_t crossingB;
        int strand;
        int sign;

        size_t effective_index(size_t start, size_t n) const {
            return std::min(fastMod(crossingA + n - start, n), fastMod(crossingB + n - start, n));
        }

        int cmp(const FastSigData& rhs, size_t lstart, size_t rstart, size_t n) {
            size_t effective_l = effective_index(lstart, n);
            size_t effective_r = rhs.effective_index(rstart, n);
            if (effective_l < effective_r) return -1;
            if (effective_l > effective_r) return 1;
            if (strand > rhs.strand) return -1; /* upper first */
            if (strand < rhs.strand) return 1;
            if (sign > rhs.sign) return -1; /* positive first */
            if (sign < rhs.sign) return 1;
            return 0;
        }
    };

    // Returns 0 if circular slices were still equal after max_equalities comparisons, -x - 1 if a was lesser after x comparisons, and x + 1 if a was greater after x comparisons.
    int circularCompare(FastSigData* a, size_t i, FastSigData* b, size_t j,
            size_t n, size_t max_equalities) {
        size_t astart = i;
        size_t bstart = j;
        for (size_t k = 0; k < max_equalities; k++) {
            int comparison = a[i].cmp(b[j], astart, bstart, n);
            if (comparison < 0) return -((int)k) - 1;
            if (comparison > 0) return (int)k + 1;
            ++i;
            ++j;
            if (i == n) i = 0;
            if (j == n) j = 0;
        }
        return 0;
    }
}

size_t minimumStartingPoint(FastSigData* a, size_t n) {
    size_t halfN = n / 2;
    size_t ceilHalfN = (n + 1) / 2;
    size_t iMax = 0;
    size_t jMax = halfN;
    size_t alpha, beta;
    while (true) {
        if (iMax >= halfN) {
            alpha = std::max(iMax, jMax);
            beta = n;
            break;
        }
        if (jMax >= n) {
            alpha = std::max(iMax, jMax - n);
            beta = halfN;
            break;
        }
        int comparison = circularCompare(a, iMax, a, jMax, n, ceilHalfN);
        if (comparison == 0) {
            if (iMax == 0 || jMax == halfN) {
                // q is even
                return minimumStartingPoint(a, halfN);
            }
            size_t d = std::gcd(n, jMax - iMax);
            size_t s = n - std::max(d, std::max(iMax + 1, jMax - halfN + 1));

            comparison = circularCompare(a, iMax, a, jMax, n, s);
            if (comparison == 0) {
                if (s == n - d) {
                    if (iMax >= jMax - halfN) {
                        alpha = iMax;
                        beta = d + 1;
                    } else {
                        alpha = jMax;
                        beta = halfN + d + 1;
                    }
                } else {
                    if (s == n - iMax - 1) {
                        alpha = std::min(iMax, jMax - iMax);
                        beta = halfN + 1;
                    } else {
                        alpha = std::min(jMax, iMax - jMax + halfN + n);
                        beta = n;
                    }
                }
                break;
            } else if (comparison < 0) {
                jMax += (size_t)(-comparison) - 1;
            } else {
                iMax += (size_t)comparison - 1;
            }
        } else if (comparison < 0) {
            jMax += (size_t)(-comparison);
        } else {
            iMax += (size_t)comparison;
        }
    }

    size_t gamma = alpha;
    size_t delta = gamma + 1;
    while (delta < beta) {
        int comparison = circularCompare(a, gamma, a, delta, n, beta - gamma);
        if (comparison > 0) {
            gamma = std::max(delta, gamma + (size_t)comparison + 1);
            delta = gamma + 1;
        } else if (comparison < 0) {
            delta += (size_t)(-comparison);
        } else {
            delta += beta - gamma + 1;
        }
    }

    return gamma;
}

std::string Link::fastKnotSig(bool useReflection, bool useReverse) const {
    // Only defined for knots at present.
    if (components_.size() != 1)
        throw NotImplemented(
            "Knot signatures are only implemented for single-component links");
    size_t n = crossings_.size();
    
    // Walk through the link once to construct the FastSigData objects
    auto* curr = new FastSigData[2 * n];
    auto* firstCrossing = new size_t[n];
    std::fill(firstCrossing, firstCrossing + n, 2 * n);
    auto strand = crossings_[0]->strand(0);
    for (size_t i = 0; i < 2 * n; ++i) {
        size_t crossing = strand.crossing()->index();
        if (firstCrossing[crossing] == 2 * n) {
            firstCrossing[crossing] = i;
            curr[i] = FastSigData { i, 0, strand.strand(), strand.crossing()->sign() };
        } else {
            curr[i] = FastSigData { firstCrossing[crossing], i, strand.strand(), strand.crossing()->sign() };
            curr[firstCrossing[crossing]].crossingB = i;
        }
        ++strand;
    }
    delete[] firstCrossing;

    auto* best = new FastSigData[2 * n];
    size_t bestStart = 0;

    bool begin = true;
    for (int reflectSign = 0; reflectSign < 2; ++reflectSign) {
        for (int reflectStrands = 0; reflectStrands < 2; ++reflectStrands) {
            for (int reverse = 0; reverse < 2; ++reverse) {
                size_t start = minimumStartingPoint(curr, 2 * n);
                if (begin || circularCompare(curr, start, best, bestStart, 2 * n, 2 * n) < 0) {
                    begin = false;
                    memcpy(best, curr, 2 * n * sizeof(FastSigData));
                    bestStart = start;
                }

                if (!useReverse) {
                    break;
                }
                if (reverse == 0) {
                    for (size_t i = 0; i < 2 * n; ++i) {
                        curr[i].crossingA = 2 * n - curr[i].crossingA;
                        curr[i].crossingB = 2 * n - curr[i].crossingB;
                    }
                }
            }
            if (reflectStrands == 0) {
                for (size_t i = 0; i < 2 * n; ++i) {
                    curr[i].strand ^= 1;
                }
            }
        }

        if (!useReflection) {
            break;
        }
        if (reflectSign == 0) {
            for (size_t i = 0; i < 2 * n; ++i) {
                curr[i].sign *= -1;
            }
        }
    }

    delete[] curr;
    size_t* crossingMap = new size_t[2 * n];
    std::fill(crossingMap, crossingMap + 2 * n, 2 * n);
    size_t nextCrossing = 0;
    size_t k = bestStart;
    do {
        if (crossingMap[best[k].crossingA] == 2 * n) {
            crossingMap[best[k].crossingA] = nextCrossing++;
        }
        k++;
        if (k == 2 * n) k = 0;
    } while (k != bestStart);
    assert(nextCrossing == n);

    // Text: n c_1 c_2 ... c_2n [packed strand bits] [packed sign bits]
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
    for (auto dat = best + bestStart; dat != best + 2*n; ++dat)
        Base64SigEncoding::encodeInt(ans, crossingMap[dat->crossingA], charsPerInt);
    for (auto dat = best; dat != best + bestStart; ++dat)
        Base64SigEncoding::encodeInt(ans, crossingMap[dat->crossingA], charsPerInt);

    // Output strands and signs, each as a packed sequence of bits.
    unsigned i, j;
    unsigned write;
    for (i = 0; i < 2 * n; i += 6) {
        write = 0;
        for (j = 0; j < 6 && i + j < 2 * n; ++j)
            if (best[fastMod(bestStart + i + j, 2 * n)].strand)
                write |= (1 << j);
        ans += Base64SigEncoding::encodeSingle(write);
    }
    for (i = 0; i < 2 * n; i += 6) {
        write = 0;
        for (j = 0; j < 6 && i + j < 2 * n; ++j)
            if (best[fastMod(bestStart + i + j, 2 * n)].sign > 0)
                write |= (1 << j);
        ans += Base64SigEncoding::encodeSingle(write);
    }

    delete[] best;
    delete[] crossingMap;
    return ans;
}

} // namespace regina

