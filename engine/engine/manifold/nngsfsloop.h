
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

/*! \file nngsfsloop.h
 *  \brief Deals with graph manifolds formed from self-identified Seifert
 *  fibred spaces.
 */

#ifndef __NNGSFSLOOP_H
#ifndef __DOXYGEN
#define __NNGSFSLOOP_H
#endif

#include "manifold/nmanifold.h"
#include "utilities/nmatrix2.h"

namespace regina {

class NSFSpace;

/**
 * \weakgroup manifold
 * @{
 */

/**
 * Represents a closed graph manifold formed by joining a
 * single bounded Seifert fibred space to itself along a torus.
 *
 * The Seifert fibred space must have two boundary components, each a
 * torus corresponding to a puncture in the base orbifold (with no
 * fibre-reversing twist as one travels around the boundary).
 *
 * The way in which the two torus boundaries are joined together is specified
 * by a 2-by-2 matrix \a M.  This matrix relates the locations of the fibres
 * and base orbifold on the two boundary tori.
 *
 * More specifically, suppose that \a f0 and \a o0 are generators of the
 * first boundary torus, where \a f0 represents a directed fibre in the
 * Seifert fibred space and \a o0 represents the oriented boundary of
 * the base orbifold.  Likewise, let \a f1 and \a o1 be generators of the
 * second boundary torus representing a directed fibre and the oriented
 * boundary of the base orbifold.  Then the tori are joined together so
 * that the curves \a f0, \a o0, \a f1 and \a o1 become related as follows:
 *
 * <pre>
 *     [f1]       [f0]
 *     [  ] = M * [  ]
 *     [o1]       [o0]
 * </pre>
 *
 * See the page on \ref sfsnotation for details on some of the
 * terminology used above.
 *
 * The optional NManifold routine getHomologyH1() is implemented, but
 * the optional routine construct() is not.
 *
 * \todo \opt Speed up homology calculations involving orientable base
 * spaces by adding rank afterwards, instead of adding generators for
 * genus into the presentation matrix.
 */
class NNGSFSLoop : public NManifold {
    private:
        NSFSpace* sfs_;
            /**< The bounded Seifert fibred space that is joined to itself. */
        NMatrix2 matchingReln_;
            /**< The matrix describing how the two boundary tori are joined;
                 see the class notes for details. */

    public:
        /**
         * Creates a new graph manifold as a self-identified Seifert fibred
         * space.  The bounded Seifert fibred space and the four elements of
         * the 2-by-2 matching matrix are all passed separately.  The elements
         * of the matching matrix combine to give the full matrix \a M as
         * follows:
         *
         * <pre>
         *           [ mat00  mat01 ]
         *     M  =  [              ]
         *           [ mat10  mat11 ]
         * </pre>
         *
         * Note that the new object will take ownership of the given
         * Seifert fibred space, and when this object is destroyed the
         * Seifert fibred space will be destroyed also.
         *
         * \pre The given Seifert fibred space has precisely two torus
         * boundaries, corresponding to two untwisted punctures in the
         * base orbifold.
         * \pre The given matching matrix has determinant +1 or -1.
         *
         * @param sfs the bounded Seifert fibred space.
         * @param mat00 the (0,0) element of the matching matrix.
         * @param mat01 the (0,1) element of the matching matrix.
         * @param mat10 the (1,0) element of the matching matrix.
         * @param mat11 the (1,1) element of the matching matrix.
         */
        NNGSFSLoop(NSFSpace* sfs, long mat00, long mat01,
            long mat10, long mat11);
        /**
         * Creates a new graph manifold as a self-identified Seifert fibred
         * space.  The bounded Seifert fibred space and the entire 2-by-2
         * matching matrix are each passed separately.
         *
         * Note that the new object will take ownership of the given
         * Seifert fibred space, and when this object is destroyed the
         * Seifert fibred space will be destroyed also.
         *
         * \pre The given Seifert fibred space has precisely two torus
         * boundaries, corresponding to two punctures in the base orbifold.
         * \pre The given matching matrix has determinant +1 or -1.
         *
         * @param sfs the bounded Seifert fibred space.
         * @param matchingReln the 2-by-2 matching matrix.
         */
        NNGSFSLoop(NSFSpace* sfs, const NMatrix2& matchingReln);
        /**
         * Destroys this structure along with the bounded Seifert
         * fibred space and the matching matrix.
         */
        ~NNGSFSLoop();

        /**
         * Returns a reference to the bounded Seifert fibred space that
         * is joined to itself.
         *
         * @return a reference to the bounded Seifert fibred space.
         */
        const NSFSpace& sfs() const;
        /**
         * Returns a reference to the 2-by-2 matrix describing how the
         * two boundary tori of the Seifert fibred space are joined together.
         * See the class notes for details on precisely how this matrix is
         * represented.
         *
         * @return a reference to the matching matrix.
         */
        const NMatrix2& matchingReln() const;

        NAbelianGroup* getHomologyH1() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;

    private:
        /**
         * Uses (1,1) twists, inversion and/or reflection to make the
         * presentation of this space more aesthetically pleasing.
         */
        void reduce();

        /**
         * Uses (1,1) twists and/or inversion to make the given matching
         * matrix more aesthetically pleasing.
         *
         * This routine is for internal use by reduce().
         *
         * @param reln the matching matrix to simplify.
         */
        static void reduce(NMatrix2& reln);

        /**
         * Uses (1,1) twists to make the given matching matrix more
         * aesthetically pleasing.
         *
         * This routine is for internal use by reduce().
         *
         * @param reln the matching matrix to simplify.
         */
        static void reduceBasis(NMatrix2& reln);

        /**
         * Decides whether the first given matrix is more aesthetically
         * pleasing than the second.  This judgement is somewhat
         * arbitrary and is subject to change in future versions of Regina.
         *
         * This routine is for internal use by reduce().
         *
         * \warning This routine might behave differently from similar
         * routines in other classes (as a result of differing aesthetic
         * requirements).
         *
         * @param m1 the first matrix to examine.
         * @param m2 the second matrix to examine.
         * @return \c true if \a m1 is declared to be more pleasing than
         * \a m2, or \c false if \a m2 is more pleasing or a decision
         * could not be reached.
         */
        static bool simpler(const NMatrix2& m1, const NMatrix2& m2);
};

/*@}*/

// Inline functions for NNGSFSLoop

inline NNGSFSLoop::NNGSFSLoop(NSFSpace* sfs,
        long mat00, long mat01, long mat10, long mat11) :
        sfs_(sfs), matchingReln_(mat00, mat01, mat10, mat11) {
    reduce();
}

inline NNGSFSLoop::NNGSFSLoop(NSFSpace* sfs, const NMatrix2& matchingReln) :
        sfs_(sfs), matchingReln_(matchingReln) {
    reduce();
}

inline const NSFSpace& NNGSFSLoop::sfs() const {
    return *sfs_;
}

inline const NMatrix2& NNGSFSLoop::matchingReln() const {
    return matchingReln_;
}

} // namespace regina

#endif

