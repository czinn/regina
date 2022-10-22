
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
#include "link/tangle.h"
#include "triangulation/detail/retriangulate-impl.h"

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace regina {

namespace detail {
    template <>
    struct RetriangulateParams<Link> {
        static std::string sig(const Link& link) {
            return link.knotSig();
        }

        static constexpr const char* progressStage = "Exploring diagrams";

        template <class Retriangulator>
        static void propagateFrom(const std::string& sig, size_t maxSize,
                Retriangulator* retriang) {
            Link t = Link::fromKnotSig(sig);
            t.propagate(maxSize, [&](Link&& link) {
                        return retriang->candidate(std::move(link), sig);
                    });
        }
    };

    
    template <>
    struct RetriangulateParams<Tangle> {
        static std::string sig(const Tangle& tangle) {
            return tangle.tangleSig();
        }

        static constexpr const char* progressStage = "Exploring diagrams";

        template <class Retriangulator>
        static void propagateFrom(const std::string& sig, size_t maxSize,
                Retriangulator* retriang) {
            Tangle t = Tangle::fromTangleSig(sig);
            if (t.size() == 0) {
                // We have a zero-crossing tangle.
                if (maxSize >= 1) {
                    for (int side = 0; side < 2; side++) {
                        Tangle alt(t);
                        alt.r1(regina::StrandRef(), side, 1 /* sign */, false, true);
                        if (retriang->candidate(std::move(alt), sig))
                            return;
                    }
                }
                if (maxSize >= 2) {
                    Tangle alt(t);
                    int firstStrandSide = t.type() == '-' ? 1 : 0;
                    alt.r2(regina::StrandRef(nullptr, 0), firstStrandSide, regina::StrandRef(nullptr, 1), !firstStrandSide, false, true);
                    if (retriang->candidate(std::move(alt), sig))
                        return;
                }
            }

            // From here we assume >= 1 crossing.
            size_t i;
            int strand, side, sign;

            for (i = 0; i < t.size(); ++i)
                if (t.r1(t.crossing(i), true, false)) {
                    Tangle alt(t);
                    alt.r1(alt.crossing(i), false, true);
                    if (retriang->candidate(std::move(alt), sig))
                        return;
                }

            for (i = 0; i < t.size(); ++i)
                if (t.r2(t.crossing(i), true, false)) {
                    Tangle alt(t);
                    alt.r2(alt.crossing(i), false, true);
                    if (retriang->candidate(std::move(alt), sig))
                        return;
                }

            for (i = 0; i < t.size(); ++i)
                for (side = 0; side < 2; ++side)
                    if (t.r3(t.crossing(i), side, true, false)) {
                        Tangle alt(t);
                        alt.r3(alt.crossing(i), side, false, true);
                        if (retriang->candidate(std::move(alt), sig))
                            return;
                    }

            // R1 twist moves on arcs are always valid.
            if (t.size() < maxSize)
                for (i = 0; i <= t.size(); ++i)
                    for (strand = 0; strand < 2; ++strand)
                        for (side = 0; side < 2; ++side)
                            for (sign = -1; sign <= 1; sign += 2) {
                                Tangle alt(t);
                                StrandRef arc = i == t.size() ?
                                    regina::StrandRef(nullptr, strand) :
                                    alt.crossing(i)->strand(strand);
                                alt.r1(arc, side, sign, false, true);
                                if (retriang->candidate(std::move(alt), sig))
                                    return;
                            }

            if (t.size() + 1 < maxSize) {
                StrandRef upperArc, lowerArc;
                int upperSide, lowerSide;
                for (i = 0; i <= t.size(); ++i)
                    for (strand = 0; strand < 2; ++strand) {
                        upperArc = i == t.size() ?
                            regina::StrandRef(nullptr, strand) :
                            t.crossing(i)->strand(strand);
                        for (upperSide = 0; upperSide < 2; ++upperSide) {
                            // Walk around the 2-cell containing upperArc.
                            lowerArc = upperArc;
                            lowerSide = upperSide;
                            while (true) {
                                auto [nextArc, nextSide] = t.nextArcInCell(lowerArc, lowerSide);
                                lowerArc = nextArc;
                                lowerSide = nextSide;

                                // By planarity, the 2-cell can meet one side of
                                // an arc, but never both.
                                if (lowerArc == upperArc) {
                                    // We completed the cycle.
                                    break;
                                }

                                Tangle alt(t);
                                alt.r2(alt.translate(upperArc), upperSide,
                                    alt.translate(lowerArc), lowerSide,
                                    false, true);
                                if (retriang->candidate(std::move(alt), sig))
                                    return;
                            }
                        }
                    }
            }
        }
    };
} // namespace detail

// Instantiate all necessary rewriting/retriangulation template functions
// so the full implementation can stay out of the headers.

template bool regina::detail::retriangulateInternal<Link, true>(
    const Link&, int, unsigned, ProgressTrackerOpen*,
    regina::detail::RetriangulateActionFunc<Link, true>&&);

template bool regina::detail::retriangulateInternal<Link, false>(
    const Link&, int, unsigned, ProgressTrackerOpen*,
    regina::detail::RetriangulateActionFunc<Link, false>&&);

template bool regina::detail::retriangulateInternal<Tangle, true>(
    const Tangle&, int, unsigned, ProgressTrackerOpen*,
    regina::detail::RetriangulateActionFunc<Tangle, true>&&);

template bool regina::detail::retriangulateInternal<Tangle, false>(
    const Tangle&, int, unsigned, ProgressTrackerOpen*,
    regina::detail::RetriangulateActionFunc<Tangle, false>&&);

} // namespace regina

