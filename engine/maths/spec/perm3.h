
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file maths/spec/perm3.h
 *  \brief Internal header for permutations of {0,1,2}.
 *
 *  This file is automatically included from perm.h; there is no need for
 *  end users to include this specialisation header explicitly.
 */

// We include perm.h before the header guard, to ensure that the
// various permutation headers are processed in exactly the right order.
// This specialisation header will be re-included at the beginning of
// perm-impl.h.
#include "maths/perm.h"

#ifndef __PERM3_H
#ifndef __DOXYGEN
#define __PERM3_H
#endif

#include <cstdlib>
#include <string>
#include "regina-core.h"

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * Represents a permutation of {0,1,2}.
 * This is a specialisation of the generic Perm template: it is highly
 * optimised, and also offers some additional functionality.
 * Amongst other things, this permutation class is used to specify how
 * simplices of a 2-manifold triangulation are glued together.
 *
 * As with all Perm template classes, these objects are small enough to
 * pass about by value instead of by reference.  Moreover, Perm<3> in
 * particular is extremely fast to work with.
 *
 * Each permutation has an internal code, which is a single native
 * integer that is sufficient to reconstruct the permutation.
 * Thus the internal code may be a useful means for passing permutation
 * objects to and from the engine.  For Perm<3>, the internal code is an
 * integer between 0 and 5 inclusive that gives the index of the
 * permutation in the array Perm<3>::S3.
 *
 * To use this class, simply include the main permutation header maths/perm.h.
 *
 * \ifacespython Since Python does not support templates, this class is
 * made available under the name Perm3.
 */
template <>
class REGINA_API Perm<3> {
    private:
        /**
         * An array-like object used to implement Perm<3>::S3.
         */
        struct S3Lookup {
            /**
             * Returns the permutation at the given index in the array S3.
             * See Perm<3>::S3 for details.
             *
             * @param index an index between 0 and 5 inclusive.
             * @return the corresponding permutation in S3.
             */
            constexpr Perm<3> operator[] (int index) const;
        };

        /**
         * An array-like object used to implement Perm<3>::orderedS3.
         */
        struct OrderedS3Lookup {
            /**
             * Returns the permutation at the given index in the array
             * orderedS3.  See Perm<3>::orderedS3 for details.
             *
             * @param index an index between 0 and 5 inclusive.
             * @return the corresponding permutation in orderedS3.
             */
            constexpr Perm<3> operator[] (int index) const;
        };

        /**
         * An array-like object used to implement Perm<3>::S2.
         */
        struct S2Lookup {
            /**
             * Returns the permutation at the given index in the array S2.
             * See Perm<3>::S2 for details.
             *
             * @param index an index between 0 and 1 inclusive.
             * @return the corresponding permutation in S2.
             */
            constexpr Perm<3> operator[] (int index) const;
        };

    public:
        /**
         * Denotes a native signed integer type large enough to count all
         * permutations on three elements.  In other words, this is a
         * native signed integer type large enough to store (3!).
         */
        typedef int Index;

        /**
         * The total number of permutations on three elements.
         * This is the size of the array Sn.
         */
        static constexpr Index nPerms = 6;

        /**
         * The total number of permutations on two elements.
         * This is the size of the array Sn_1.
         */
        static constexpr Index nPerms_1 = 2;

        /**
         * Indicates the native unsigned integer type used to store the
         * internal permutation code.
         */
        typedef uint8_t Code;

        /**
         * Gives array-like access to all possible permutations of
         * three elements.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>S3[i]</tt>.  The index \a i must be
         * between 0 and 5 inclusive.
         *
         * In Regina 6.0 and earlier, this was a hard-coded C-style array;
         * since Regina 6.1 it has changed type, but accessing elements as
         * described above remains extremely fast.  The object that is returned
         * is lightweight and is defined in the headers only; in particular,
         * you cannot make a reference to it (but you can always make a copy).
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * For all permutation classes (Perm<3>, Perm<4> and so on),
         * the S3 array stores the same permutations in the same order
         * (but of course using different data types).
         *
         * Note that these permutations are not necessarily in
         * lexicographical order.
         */
        static constexpr S3Lookup S3 {};

        /**
         * A dimension-agnostic alias for Perm<3>::S3.  In general, for
         * each \a K the class PermK will define an alias \a Sn
         * that references the list of all permutations PermK::SK.
         */
        static constexpr S3Lookup Sn {};

        /**
         * Gives array-like access to all possible permutations of three
         * elements in lexicographical order.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>orderedS3[i]</tt>.  The index \a i
         * must be between 0 and 5 inclusive.
         *
         * In Regina 6.0 and earlier, this was a hard-coded C-style array;
         * since Regina 6.1 it has changed type, but accessing elements as
         * described above remains extremely fast.  The object that is returned
         * is lightweight and is defined in the headers only; in particular,
         * you cannot make a reference to it (but you can always make a copy).
         */
        static constexpr OrderedS3Lookup orderedS3 {};

        /**
         * A dimension-agnostic alias for Perm<3>::orderedS3.  In general, for
         * each \a K the class PermK will define an alias \a orderedSn
         * that references the list of all permutations PermK::orderedSK.
         */
        static constexpr OrderedS3Lookup orderedSn {};

        /**
         * Gives array-like access to all possible permutations of
         * two elements.  In each permutation, 2 maps to 2.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>S2[i]</tt>.  The index \a i must be
         * between 0 and 1 inclusive.
         *
         * In Regina 6.0 and earlier, this was a hard-coded C-style array;
         * since Regina 6.1 it has changed type, but accessing elements as
         * described above remains extremely fast.  The object that is returned
         * is lightweight and is defined in the headers only; in particular,
         * you cannot make a reference to it (but you can always make a copy).
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * For all permutation classes (Perm<3>, Perm<4> and so on),
         * the S2 array stores the same permutations in the same order
         * (but of course using different data types).
         *
         * Note that these permutations are already in lexicographical order.
         */
        static constexpr S2Lookup S2 {};

        /**
         * A dimension-agnostic alias for Perm<3>::S2.  In general, for
         * each \a K the class PermK will define an alias \a Sn_1
         * that references the list of all permutations PermK::S(K-1).
         */
        static constexpr S2Lookup Sn_1 {};

        enum {
            /**
             * The internal code for the permutation (0,1,2).
             *
             * \ifacespython Not present.
             */
            code012 = 0,
            /**
             * The internal code for the permutation (0,2,1).
             *
             * \ifacespython Not present.
             */
            code021 = 1,
            /**
             * The internal code for the permutation (1,2,0).
             *
             * \ifacespython Not present.
             */
            code120 = 2,
            /**
             * The internal code for the permutation (1,0,2).
             *
             * \ifacespython Not present.
             */
            code102 = 3,
            /**
             * The internal code for the permutation (2,0,1).
             *
             * \ifacespython Not present.
             */
            code201 = 4,
            /**
             * The internal code for the permutation (2,1,0).
             *
             * \ifacespython Not present.
             */
            code210 = 5
        };

    private:
        Code code_;
            /**< The internal code representing this permutation. */

    public:
        /**
         * Creates the identity permutation.
         */
        constexpr Perm();

        /**
         * Creates the transposition of \a a and \a b.
         * Note that \a a and \a b need not be distinct.
         *
         * \pre \a a and \a b are in {0,1,2}.
         *
         * @param a the element to switch with \a b.
         * @param b the element to switch with \a a.
         */
        constexpr Perm(int a, int b);

        /**
         * Creates a permutation mapping (0,1,2) to
         * (<i>a</i>,<i>b</i>,<i>c</i>) respectively.
         *
         * \pre {<i>a</i>,<i>b</i>,<i>c</i>} = {0,1,2}.
         *
         * @param a the desired image of 0.
         * @param b the desired image of 1.
         * @param c the desired image of 2.
         */
        constexpr Perm(int a, int b, int c);

        /**
         * Creates a permutation mapping \a i to \a image[i] for each
         * \a i = 0,1,2.
         *
         * \pre The array \a image contains three elements, which are
         * 0, 1 and 2 in some order.
         *
         * \ifacespython Not present.
         *
         * @param image the array of images.
         */
        constexpr Perm(const int* image);

        /**
         * Creates a permutation mapping (\a a[0], ..., \a a[2]) to
         * (\a b[0], ..., \a b[2]) respectively.
         *
         * \pre Both arrays \a a and \a b contain 3 elements, which
         * are 0,...,2 in some order.
         *
         * \ifacespython Not present.
         *
         * @param a the array of preimages; this must have length 3.
         * @param b the corresponding array of images; this must also have
         * length 3.
         */
        constexpr Perm(const int* a, const int* b);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * @param cloneMe the permutation to clone.
         */
        constexpr Perm(const Perm<3>& cloneMe) = default;

        /**
         * Returns the internal code representing this permutation.
         * Note that the internal code is sufficient to reproduce the
         * entire permutation.
         *
         * The code returned will be a valid permutation code as
         * determined by isPermCode().
         *
         * @return the internal code.
         */
        constexpr Code permCode() const;

        /**
         * Sets this permutation to that represented by the given
         * internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param code the internal code that will determine the
         * new value of this permutation.
         */
        void setPermCode(Code code);

        /**
         * Creates a permutation from the given internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param code the internal code for the new permutation.
         * @return the permutation represented by the given internal code.
         */
        static constexpr Perm<3> fromPermCode(Code code);

        /**
         * Determines whether the given integer is a valid internal
         * permutation code.  Valid permutation codes can be passed to
         * setPermCode() or fromPermCode(), and are returned by permCode().
         *
         * @return \c true if and only if the given code is a valid
         * internal permutation code.
         */
        static constexpr bool isPermCode(Code code);

        /**
         * Sets this permutation to be equal to the given permutation.
         *
         * @param cloneMe the permutation whose value will be assigned
         * to this permutation.
         * @return a reference to this permutation.
         */
        Perm<3>& operator = (const Perm<3>& cloneMe) = default;

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is <i>p</i>, the
         * resulting permutation will be <i>p o q</i>, satisfying
         * <tt>(p*q)[x] == p[q[x]]</tt>.
         *
         * @param q the permutation with which to compose this.
         * @return the composition of both permutations.
         */
        constexpr Perm<3> operator * (const Perm<3>& q) const;

        /**
         * Finds the inverse of this permutation.
         *
         * @return the inverse of this permutation.
         */
        constexpr Perm<3> inverse() const;

        /**
         * Finds the reverse of this permutation.
         *
         * Here \e reverse means that we reverse the images of 0,1,2.
         * In other words, if permutation \a q is the
         * reverse of \a p, then <tt>p[i] == q[2 - i]</tt> for all \a i.
         */
        constexpr Perm<3> reverse() const;

        /**
         * Determines the sign of this permutation.
         *
         * @return 1 if this permutation is even, or -1 if this
         * permutation is odd.
         */
        constexpr int sign() const;

        /**
         * Determines the image of the given integer under this
         * permutation.
         *
         * @param source the integer whose image we wish to find.  This
         * should be between 0 and 2 inclusive.
         * @return the image of \a source.
         */
        constexpr int operator[](int source) const;

        /**
         * Determines the preimage of the given integer under this
         * permutation.
         *
         * @param image the integer whose preimage we wish to find.  This
         * should be between 0 and 2 inclusive.
         * @return the preimage of \a image.
         */
        constexpr int preImageOf(int image) const;

        /**
         * Determines if this is equal to the given permutation.
         * This is true if and only if both permutations have the same
         * images for 0, 1 and 2.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * are equal.
         */
        constexpr bool operator == (const Perm<3>& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for at least one of 0, 1 or 2.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * differ.
         */
        constexpr bool operator != (const Perm<3>& other) const;

        /**
         * Lexicographically compares the images of (0,1,2) under this
         * and the given permutation.
         *
         * @param other the permutation with which to compare this.
         * @return -1 if this permutation produces a smaller image, 0 if
         * the permutations are equal and 1 if this permutation produces
         * a greater image.
         */
        constexpr int compareWith(const Perm<3>& other) const;

        /**
         * Determines if this is the identity permutation.
         * This is true if and only if each of 0, 1 and 2 is mapped to itself.
         *
         * @return \c true if and only if this is the identity permutation.
         */
        constexpr bool isIdentity() const;

        /**
         * Returns the <i>i</i>th rotation.
         * This maps <i>k</i> to <i>k</i>&nbsp;+&nbsp;<i>i</i> (mod 3)
         * for all \a k.
         *
         * @param i the image of 0; this must be between 0 and 2 inclusive.
         * @return the <i>i</i>th rotation.
         */
        static constexpr Perm rot(int i);

        /**
         * Returns the <i>i</i>th permutation on three elements, where
         * permutations are numbered lexicographically beginning at 0.
         *
         * Lexicographical ordering treats each permutation \a p as the
         * 3-tuple (\a p[0], \a p[1], \a p[2]).
         *
         * The return value will be identical to orderedS3[\a i].
         *
         * @param i the lexicographical index of the permutation; this
         * must be between 0 and 5 inclusive.
         * @return the <i>i</i>th permutation.
         */
        static constexpr Perm atIndex(Index i);

        /**
         * Returns a random permutation on three elements.
         * All permutations are returned with equal probability.
         *
         * This routine is thread-safe, and uses RandomEngine for its
         * random number generation.
         *
         * \warning This routine is expensive, since it locks and unlocks
         * the mutex protecting Regina's global uniform random bit generator.
         * If you are calling this many times in quick succession, consider
         * creating a single RandomEngine object yourself and then calling
         * <tt>rand(randomEngine.engine(), even)</tt>.
         *
         * @param even if \c true, then the resulting permutation is
         * guaranteed to be even (and again all even permutations are
         * returned with equal probability).
         * @return a random permutation.
         */
        static Perm rand(bool even = false);

        /**
         * Returns a random permutation on three elements, using the
         * given uniform random bit generator.
         * All permutations are returned with equal probability.
         *
         * The thread safety of this routine is of course dependent on
         * the thread safety of your uniform random bit generator \a gen.
         *
         * \tparam URBG A type which, once any references are removed, must
         * adhere to the C++ \a UniformRandomBitGenerator concept.
         *
         * \ifacespython Not present, though the non-thread-safe variant
         * without the \a gen argument is available.
         *
         * @param gen the source of randomness to use (e.g., one of the
         * many options provided in the C++ standard \c random header).
         * @param even if \c true, then the resulting permutation is
         * guaranteed to be even (and again all even permutations are
         * returned with equal probability).
         * @return a random permutation.
         */
        template <class URBG>
        static Perm rand(URBG&& gen, bool even = false);

        /**
         * Returns the lexicographical index of this permutation.  This
         * indicates where this permutation sits within a full lexicographical
         * ordering of all 3! permutations on three elements.
         *
         * Lexicographical ordering treats each permutation \a p as the
         * 3-tuple (\a p[0], \a p[1], \a p[2]).
         * In particular, the identity permutation has index 0, and the
         * "reverse" permutation (which maps each \a i to 2-<i>i</i>)
         * has index 5 = 3!-1.
         *
         * This routine is identical to orderedS3Index().
         *
         * @return the index of this permutation, which will be between
         * 0 and 5 inclusive.
         */
        constexpr Index index() const;

        /**
         * Returns a string representation of this permutation.
         * The representation will consist of three adjacent digits
         * representing the images of 0, 1 and 2 respectively.  An
         * example of a string representation is <tt>120</tt>.
         *
         * @return a string representation of this permutation.
         */
        std::string str() const;

        /**
         * Returns a prefix of the string representation of this permutation,
         * containing only the images of the first \a len integers.
         *
         * @param len the length of the prefix required; this must be
         * between 0 and 3 inclusive.
         * @return the corresponding prefix of the string representation
         * of this permutation.
         */
        std::string trunc(unsigned len) const;

        /**
         * Returns a string representation of this permutation with only
         * the images of 0 and 1.  The resulting string will therefore
         * have length two.
         *
         * @return a truncated string representation of this permutation.
         */
        std::string trunc2() const;

        /**
         * Resets the images of all integers from \a from onwards to the
         * identity map.
         *
         * Specifically, for each \a i in the range <i>from</i>,...,2,
         * this routine will ensure that <tt>image[i] == i</tt>.  The images of
         * 0,1,...,<i>from</i>-1 will not be altered.
         *
         * \pre The images of <i>from</i>,...,2 are exactly
         * <i>from</i>,...,2, but possibly in a different order.
         *
         * @param from the first integer whose image should be reset.
         * This must be between 0 and 3 inclusive.
         */
        void clear(unsigned from);

        /**
         * Returns the index of this permutation in the Perm<3>::S3 array.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<3>::S3[i].  This will be between 0 and 5 inclusive.
         */
        constexpr int S3Index() const;

        /**
         * Returns the index of this permutation in the Perm<3>::S3 array.
         * This is a dimension-agnostic alias for S3Index().
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<3>::S3[i].  This will be between 0 and 5 inclusive.
         */
        constexpr int SnIndex() const;

        /**
         * Returns the index of this permutation in the Perm<3>::orderedS3 array.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<3>::orderedS3[i].  This will be between 0 and 5 inclusive.
         */
        constexpr int orderedS3Index() const;

        /**
         * Returns the index of this permutation in the Perm<3>::orderedS3 array.
         * This is a dimension-agnostic alias for orderedS3Index().
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<3>::orderedS3[i].  This will be between 0 and 5 inclusive.
         */
        constexpr int orderedSnIndex() const;

        /**
         * Extends a <i>k</i>-element permutation to an 3-element permutation.
         * where 2 &le; \a k &lt; 3.  The only possible value of \a k is 2, but
         * this routine is kept as a template function for consistency
         * with the other classes' Perm<n>::extend() routines.
         *
         * The resulting permutation will map 0,1 to their respective images
         * under \a p, and will map the "unused" element 3 to itself.
         *
         * \tparam k the number of elements for the input permutation;
         * this must be exactly 2.
         *
         * @param p a permutation on two elements.
         * @return the same permutation expressed as a permutation on
         * three elements.
         */
        template <int k>
        static constexpr Perm<3> extend(Perm<k> p);

        /**
         * Restricts a <i>k</i>-element permutation to an 3-element
         * permutation, where \a k &gt; 3.
         *
         * The resulting permutation will map 0,1,2 to their
         * respective images under \a p, and will ignore the "unused" images
         * \a p[3],...,\a p[<i>k</i>-1].
         *
         * \pre The given permutation maps 0,1,2 to 0,1,2 in some order.
         *
         * \tparam k the number of elements for the input permutation;
         * this must be strictly greater than 3.
         *
         * @param p a permutation on \a k elements.
         * @return the same permutation restricted to a permutation on
         * 3 elements.
         */
        template <int k>
        static constexpr Perm<3> contract(Perm<k> p);

    private:
        /**
         * Contains the images of every element under every possible
         * permutation.
         *
         * Specifically, the image of \a x under the permutation <tt>S3[i]</tt>
         * is <tt>imageTable[i][x]</tt>.
         */
        static constexpr Code imageTable[6][3] = {
            { 0, 1, 2 },
            { 0, 2, 1 },
            { 1, 2, 0 },
            { 1, 0, 2 },
            { 2, 0, 1 },
            { 2, 1, 0 }
        };

        /**
         * Contains the inverses of the permutations in the array \a S3.
         *
         * Specifically, the inverse of permutation <tt>S3[i]</tt> is
         * the permutation <tt>S3[ invS3[i] ]</tt>.
         */
        static constexpr Code invS3[6] = {
            0, 1, 4, 3, 2, 5
        };

        /**
         * Contains the full multiplication table for all possible
         * permutations.
         *
         * Specifically, the product <tt>S3[x] * S3[y]</tt> is
         * the permutation <tt>S3[product[x][y]]</tt>.
         */
        static constexpr Code productTable[6][6] = {
            { 0, 1, 2, 3, 4, 5 },
            { 1, 0, 5, 4, 3, 2 },
            { 2, 3, 4, 5, 0, 1 },
            { 3, 2, 1, 0, 5, 4 },
            { 4, 5, 0, 1, 2, 3 },
            { 5, 4, 3, 2, 1, 0 }
        };

    private:
        /**
         * Creates a permutation from the given internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param code the internal code from which the new
         * permutation will be created.
         */
        constexpr Perm<3>(Code code);

        /**
         * Converts between an index into Perm<3>::S3 and an index into
         * Perm<3>::orderedS3.  This conversion works in either direction.
         *
         * \tparam Int a native integer type; this would typically be
         * either \c int or \a Code.
         */
        template <typename Int>
        static constexpr Int convOrderedUnordered(Int index);
};

/*@}*/

// Inline functions for Perm<3>

template <typename Int>
inline constexpr Int Perm<3>::convOrderedUnordered(Int index) {
    // S5 is almost the same as orderedS5, except that we
    // swap indices 2 <--> 3.
    return ((index == 2 || index == 3) ? (index ^ 1) : index);
}

inline constexpr Perm<3> Perm<3>::S3Lookup::operator[] (int index) const {
    return Perm<3>(static_cast<Code>(index));
}

inline constexpr Perm<3> Perm<3>::OrderedS3Lookup::operator[] (int index)
        const {
    return Perm<3>(static_cast<Code>(convOrderedUnordered(index)));
}

inline constexpr Perm<3> Perm<3>::S2Lookup::operator[] (int index) const {
    return Perm<3>(index == 0 ? code012 : code102);
}

inline constexpr Perm<3>::Perm() : code_(0) {
}

inline constexpr Perm<3>::Perm(Code code) : code_(code) {
}

inline constexpr Perm<3>::Perm(int a, int b) :
        code_(a == b ? code012 :
              a == 0 ? (b == 1 ? code102 : code210) :
              a == 1 ? (b == 0 ? code102 : code021) :
                       (b == 0 ? code210 : code021)) {
}

inline constexpr Perm<3>::Perm(int a, int b, int) :
        code_(a == 0 ? (b == 1 ? 0 : 1) :
              a == 1 ? (b == 2 ? 2 : 3) :
                       (b == 0 ? 4 : 5)) {
}

inline constexpr Perm<3>::Perm(const int* image) :
        code_(image[0] == 0 ? (image[1] == 1 ? 0 : 1) :
              image[0] == 1 ? (image[1] == 2 ? 2 : 3) :
                              (image[1] == 0 ? 4 : 5)) {
}

inline constexpr Perm<3>::Perm(const int* a, const int* b) : code_(0) {
    // TODO: When we move to C++20, we can get rid of the zero initialisers.
    int image[3] = { 0, 0, 0 };
    image[a[0]] = b[0];
    image[a[1]] = b[1];
    image[a[2]] = b[2];

    switch (image[0]) {
        case 0:
            code_ = static_cast<Code>(image[1] == 1 ? 0 : 1); break;
        case 1:
            code_ = static_cast<Code>(image[1] == 2 ? 2 : 3); break;
        case 2:
            code_ = static_cast<Code>(image[1] == 0 ? 4 : 5); break;
    }
}

inline constexpr Perm<3>::Code Perm<3>::permCode() const {
    return code_;
}

inline void Perm<3>::setPermCode(Code code) {
    code_ = code;
}

inline constexpr Perm<3> Perm<3>::fromPermCode(Code code) {
    return Perm<3>(code);
}

inline constexpr bool Perm<3>::isPermCode(Code code) {
    // code >= 0 is a no-op because we are using an unsigned data type.
    return (code < 6);
}

inline constexpr Perm<3> Perm<3>::operator * (const Perm<3>& q) const {
    return Perm<3>(productTable[code_][q.code_]);
}

inline constexpr Perm<3> Perm<3>::inverse() const {
    return Perm<3>(invS3[code_]);
}

inline constexpr Perm<3> Perm<3>::reverse() const {
    // p becomes p * 210.
    return Perm<3>(productTable[code_][code210]);
}

inline constexpr int Perm<3>::sign() const {
    return (code_ % 2 ? -1 : 1);
}

inline constexpr int Perm<3>::operator[](int source) const {
    return imageTable[code_][source];
}

inline constexpr int Perm<3>::preImageOf(int image) const {
    return imageTable[invS3[code_]][image];
}

inline constexpr bool Perm<3>::operator == (const Perm<3>& other) const {
    return (code_ == other.code_);
}

inline constexpr bool Perm<3>::operator != (const Perm<3>& other) const {
    return (code_ != other.code_);
}

inline constexpr int Perm<3>::compareWith(const Perm<3>& other) const {
    // Computing orderedS3Index() is very fast.
    // Use this instead of comparing images one at a time.
    int o1 = orderedS3Index();
    int o2 = other.orderedS3Index();
    return (o1 == o2 ? 0 : o1 < o2 ? -1 : 1);
}

inline constexpr bool Perm<3>::isIdentity() const {
    return (code_ == 0);
}

inline constexpr Perm<3> Perm<3>::rot(int i) {
    switch (i) {
        case 1: return Perm<3>(static_cast<Code>(code120));
        case 2: return Perm<3>(static_cast<Code>(code201));
        default: return Perm<3>(); // Identity
    }
}

inline constexpr Perm<3> Perm<3>::atIndex(Index i) {
    return orderedS3[i];
}

inline constexpr Perm<3>::Index Perm<3>::index() const {
    return orderedS3Index();
}

inline Perm<3> Perm<3>::rand(bool even) {
    RandomEngine engine;
    return rand(engine.engine(), even);
}

template <class URBG>
inline Perm<3> Perm<3>::rand(URBG&& gen, bool even) {
    if (even) {
        std::uniform_int_distribution<short> d(0, 2);
        return S3[2 * d(gen)];
    } else {
        std::uniform_int_distribution<short> d(0, 5);
        return S3[d(gen)];
    }
}

inline constexpr int Perm<3>::S3Index() const {
    return code_;
}

inline constexpr int Perm<3>::SnIndex() const {
    return code_;
}

inline constexpr int Perm<3>::orderedS3Index() const {
    return convOrderedUnordered(code_);
}

inline constexpr int Perm<3>::orderedSnIndex() const {
    return orderedS3Index();
}

} // namespace regina

#endif

