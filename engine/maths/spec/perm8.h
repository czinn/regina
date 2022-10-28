
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

/*! \file maths/spec/perm5.h
 *  \brief Internal header for permutations of {0,1,2,3,4,5,6,7}.
 *
 *  This file is automatically included from perm.h; there is no need for
 *  end users to include this specialisation header explicitly.
 */

// We include perm.h before the header guard, to ensure that the
// various permutation headers are processed in exactly the right order.
// This specialisation header will be re-included at the beginning of
// perm-impl.h.
#include "maths/perm.h"

#ifndef __REGINA_PERM8_H
#ifndef __DOXYGEN
#define __REGINA_PERM8_H
#endif

namespace regina {

/**
 * Represents a permutation of {0,1,2,3,4,5,6,7}.
 * This is a specialisation of the generic Perm template: it is highly
 * optimised, and also offers some additional functionality.
 *
 * As with all Perm template classes, these objects are small enough to pass
 * by value and swap with std::swap(), with no need for any specialised move
 * operations or swap functions.
 *
 * Each permutation has an internal code, which is a single native
 * integer that is sufficient to reconstruct the permutation.
 * Thus the internal code may be a useful means for passing permutation
 * objects to and from the engine.  For Perm<8>, the internal permutation
 * codes have changed as of Regina 7.0:
 *
 * - \e First-generation codes were used internally in Regina 6.0.1 and earlier.
 *   These are <i>image packs</i>: integers whose lowest three bits represent
 *   the image of 0, whose next lowest three bits represent the image of 1,
 *   and so on.  The routines permCode1(), setPermCode1(), fromPermCode1()
 *   and isPermCode1() continue to work with first-generation codes for
 *   backward compatibility.  Likewise, the XML data file format
 *   continues to use first-generation codes to describe gluings between
 *   6-simplices.
 *
 * - \e Second-generation codes are used internally in Regina 7.0 and above.
 *   These codes are integers between 0 and 5039 inclusive, representing the
 *   index of the permutation in the array Perm<8>::S8.  The routines
 *   permCode2(), setPermCode2(), fromPermCode2() and isPermCode2()
 *   work with second-generation codes.
 *
 * It is highly recommended that, if you need to work with permutation
 * codes at all, you use second-generation codes where possible.  This
 * is because the first-generation routines incur additional overhead
 * in converting back and forth between the second-generation codes
 * (which are used internally by Perm<8>).
 *
 * To use this class, simply include the main permutation header maths/perm.h.
 *
 * \ifacespython Since Python does not support templates, this class is
 * made available under the name Perm8.
 *
 * \ingroup maths
 */
template <>
class Perm<8> {
    private:
        /**
         * An array-like object used to implement Perm<8>::S8.
         */
        struct S8Lookup {
            /**
             * Returns the permutation at the given index in the array S8.
             * See Perm<8>::S8 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * @param index an index between 0 and 40319 inclusive.
             * @return the corresponding permutation in S8.
             */
            constexpr Perm<8> operator[] (int index) const;
        };

        /**
         * An array-like object used to implement Perm<8>::orderedS8.
         */
        struct OrderedS8Lookup {
            /**
             * Returns the permutation at the given index in the array
             * orderedS8.  See Perm<8>::orderedS8 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * @param index an index between 0 and 40319 inclusive.
             * @return the corresponding permutation in orderedS8.
             */
            constexpr Perm<8> operator[] (int index) const;
        };

    public:
        /**
         * Denotes a native signed integer type large enough to count all
         * permutations on seven elements.  In other words, this is a
         * native signed integer type large enough to store (8!).
         */
        using Index = int;

        /**
         * Indicates what type of internal permutation code is used by
         * this instance of the Perm class template.
         */
        static constexpr PermCodeType codeType = PERM_CODE_INDEX;

        /**
         * The total number of permutations on seven elements.
         * This is the size of the array Sn.
         */
        static constexpr Index nPerms = 40320;

        /**
         * The total number of permutations on six elements.
         * This is the size of the symmetric group <i>S<sub>6</sub></i>.
         */
        static constexpr Index nPerms_1 = 5040;

        /**
         * Indicates the number of bits used in an image pack to store the
         * image of a single integer.
         *
         * A full image pack combines 8 such images together, and so uses
         * 8 * \a imageBits bits in total.
         */
        static constexpr int imageBits = 3;

        /**
         * Indicates the native unsigned integer type used to store a
         * single image pack.  See the class notes for more information
         * on image packs, and how they are used to build the old
         * first-generation permutation codes.
         */
        using ImagePack = uint32_t;

        /**
         * A bitmask whose lowest \a imageBits bits are 1, and whose
         * remaining higher order bits are all 0.  This may be useful when
         * creating or analysing image packs.
         */
        static constexpr ImagePack imageMask =
                (static_cast<ImagePack>(1) << imageBits) - 1;

        /**
         * Indicates the native unsigned integer type used to store a
         * first-generation permutation code.
         */
        using Code1 = ImagePack;

        /**
         * Indicates the native unsigned integer type used to store a
         * second-generation permutation code.
         */
        using Code2 = uint16_t;

        /**
         * Gives array-like access to all possible permutations of
         * seven elements.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>Sn[i]</tt>.  The index \a i must be
         * between 0 and 40319 inclusive.
         *
         * Accessing elements through this object is extremely fast.
         * The object that is returned is lightweight and is defined in the
         * headers only; in particular, you cannot make a reference to it
         * (but you can always make a copy).
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * This is different from Perm<8>::orderedSn, since this array \a Sn
         * alternates between even and odd permutations, whereas \a orderedSn
         * stores permutations in lexicographical order.
         */
        static constexpr S8Lookup Sn {};

        /**
         * Gives array-like access to all possible permutations of
         * seven elements.
         *
         * This is a dimension-specific alias for Perm<8>::Sn; see that member
         * for further information.  In general, for every \a n there will be
         * a static member Perm<n>::Sn; however, these numerical aliases
         * Perm<2>::S2, ..., Perm<8>::S8 are only available for small \a n.
         */
        static constexpr S8Lookup S8 {};

        /**
         * Gives array-like access to all possible permutations of seven
         * elements in lexicographical order.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>orderedSn[i]</tt>.  The index \a i
         * must be between 0 and 40319 inclusive.
         *
         * Lexicographical ordering treats each permutation \a p as the
         * ordered pair (\a p[0], ..., \a p[7]).
         *
         * Accessing elements through this object is extremely fast.
         * The object that is returned is lightweight and is defined in the
         * headers only; in particular, you cannot make a reference to it
         * (but you can always make a copy).
         *
         * This is different from Perm<8>::Sn, since this array \a orderedSn
         * stores permutations in lexicographical order, whereas \a Sn
         * alternates between even and odd permutations.
         */
        static constexpr OrderedS8Lookup orderedSn {};

        /**
         * Gives array-like access to all possible permutations of seven
         * elements in lexicographical order.
         *
         * This is a dimension-specific alias for Perm<8>::orderedSn; see that
         * member for further information.  In general, for every \a n there
         * will be a static member Perm<n>::orderedSn; however, these numerical
         * aliases Perm<2>::orderedS2, ..., Perm<8>::orderedS8 are only
         * available for small \a n.
         */
        static constexpr OrderedS8Lookup orderedS8 {};

    protected:
        Code2 code2_;
            /**< The internal second-generation permutation code
                 representing this permutation. */

    private:
        static Code2 (*products_)[40320];
            /**< The precomputed table of products, or \c null if this
                 has not yet been generated.  This is a 40320-by-40320 array,
                 whose indices and values all represent indices into S8. */

        static int *orders_;
            /**< The precomputed table of orders, or \c null if this has
                 not yet been generated.  This is an array of size 40320,
                 whose indices represent indices into S8. */

        static Code2 (*powers_)[14];
            /**< The precomputed table of powers, or \c null if this has
                 not yet been generated.  This is a 40320-by-14 array,
                 where powers[i][e] is the S8 index of S8[i]^(e+1).
                 If e+1 >= orders_[i] then the corresponding value in
                 this array is left undefined (and is irrelevant). */

    public:
        /**
         * Performs the precomputation necessary for using the optimised
         * cachedComp(), cachedPow() and cachedOrder() routines.
         *
         * This \e must be called before calling any of cachedComp(),
         * cachedPow() or cachedOrder().
         *
         * This only needs to be done once in the lifetime of the program.
         * If you do try to call precompute() a second time then it will
         * do nothing and return immediately.
         *
         * This routine is thread-safe.
         */
        static void precompute();

        /**
         * Creates the identity permutation.
         */
        constexpr Perm();

        /**
         * Creates the transposition of \a a and \a b.
         * Note that \a a and \a b need not be distinct.
         *
         * \pre \a a and \a b are in {0,1,2,3,4,5,6,7}.
         *
         * @param a the element to switch with \a b.
         * @param b the element to switch with \a a.
         */
        constexpr Perm(int a, int b);

        /**
         * Creates a permutation mapping (0,1,2,3,4,5,6,7) to
         * (<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>,<i>e</i>,<i>f</i>,<i>g</i>,<i>h</i>)
         * respectively.
         *
         * \pre
         * {<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>,<i>e</i>,<i>f</i>,<i>g</i>,<i>h</i>} =
         * {0,1,2,3,4,5,6,7}.
         *
         * @param a the desired image of 0.
         * @param b the desired image of 1.
         * @param c the desired image of 2.
         * @param d the desired image of 3.
         * @param e the desired image of 4.
         * @param f the desired image of 5.
         * @param g the desired image of 6.
         * @param h the desired image of 7.
         */
        constexpr Perm(int a, int b, int c, int d, int e, int f, int g, int h);

        /**
         * Creates a permutation mapping \a i to \a image[i] for each
         * \a i = 0,1,2,3,4,5,6,7.
         *
         * \pre The elements of \a image are 0, 1, 2, 3, 4, 5, 6 and 7 in some
         * order.
         *
         * @param image the array of images.
         */
        constexpr Perm(const std::array<int, 8>& image);

        /**
         * Creates a permutation mapping
         * (<i>a0</i>,<i>b0</i>,<i>c0</i>,<i>d0</i>,<i>e0</i>,<i>f0</i>,<i>g0</i>,<i>h0</i>) to
         * (<i>a1</i>,<i>b1</i>,<i>c1</i>,<i>d1</i>,<i>e1</i>,<i>f1</i>,<i>g1</i>,<i>h1</i>)
         * respectively.
         *
         * \pre {<i>a0</i>,<i>b0</i>,<i>c0</i>,<i>d0</i>,<i>e0</i>,<i>f0</i>,<i>g0</i>,<i>h0</i>} =
         * {<i>a1</i>,<i>b1</i>,<i>c1</i>,<i>d1</i>,<i>e1</i>,<i>f1</i>,<i>g1</i>,<i>h1</i>} =
         * {0,1,2,3,4,5,6,7}.
         *
         * @param a0 the desired preimage of <i>a1</i>.
         * @param b0 the desired preimage of <i>b1</i>.
         * @param c0 the desired preimage of <i>c1</i>.
         * @param d0 the desired preimage of <i>d1</i>.
         * @param e0 the desired preimage of <i>e1</i>.
         * @param f0 the desired preimage of <i>f1</i>.
         * @param g0 the desired preimage of <i>g1</i>.
         * @param h0 the desired preimage of <i>h1</i>.
         * @param a1 the desired image of <i>a0</i>.
         * @param b1 the desired image of <i>b0</i>.
         * @param c1 the desired image of <i>c0</i>.
         * @param d1 the desired image of <i>d0</i>.
         * @param e1 the desired image of <i>e0</i>.
         * @param f1 the desired image of <i>f0</i>.
         * @param g1 the desired image of <i>g0</i>.
         * @param h1 the desired image of <i>h0</i>.
         */
        constexpr Perm(int a0, int a1, int b0, int b1, int c0, int c1,
            int d0, int d1, int e0, int e1, int f0, int f1, int g0, int g1,
            int h0, int h1);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * @param cloneMe the permutation to clone.
         */
        constexpr Perm(const Perm<8>& cloneMe) = default;

        /**
         * Returns the first-generation code representing this permutation.
         * This code is sufficient to reproduce the entire permutation.
         *
         * The code returned will be a valid first-generation permutation
         * code as determined by isPermCode1().
         *
         * \warning This routine will incur additional overhead, since
         * Perm<8> now uses second-generation codes internally.
         * See the class notes and the routine isPermCode2() for details.
         *
         * @return the first-generation permutation code.
         */
        constexpr Code1 permCode1() const;

        /**
         * Returns the second-generation code representing this permutation.
         * This code is sufficient to reproduce the entire permutation.
         *
         * The code returned will be a valid second-generation permutation
         * code as determined by isPermCode2().
         *
         * Second-generation codes are fast to work with, since they are
         * used internally by the Perm<8> class.
         *
         * @return the second-generation permutation code.
         */
        constexpr Code2 permCode2() const;

        /**
         * Sets this permutation to that represented by the given
         * first-generation permutation code.
         *
         * \pre the given code is a valid first-generation permutation code;
         * see isPermCode1() for details.
         *
         * \warning This routine will incur additional overhead, since
         * Perm<8> now uses second-generation codes internally.
         * See the class notes and the routine setPermCode2() for details.
         *
         * @param code the first-generation code that will determine the
         * new value of this permutation.
         */
        void setPermCode1(Code1 code);

        /**
         * Sets this permutation to that represented by the given
         * second-generation permutation code.
         *
         * Second-generation codes are fast to work with, since they are
         * used internally by the Perm<8> class.
         *
         * \pre the given code is a valid second-generation permutation code;
         * see isPermCode2() for details.
         *
         * @param code the second-generation code that will determine the
         * new value of this permutation.
         */
        void setPermCode2(Code2 code);

        /**
         * Creates a permutation from the given first-generation
         * permutation code.
         *
         * \pre the given code is a valid first-generation permutation code;
         * see isPermCode1() for details.
         *
         * \warning This routine will incur additional overhead, since
         * Perm<8> now uses second-generation codes internally.
         * See the class notes and the routine fromPermCode2() for details.
         *
         * @param code the first-generation code for the new permutation.
         * @return the permutation represented by the given code.
         */
        static constexpr Perm<8> fromPermCode1(Code1 code);

        /**
         * Creates a permutation from the given second-generation
         * permutation code.
         *
         * Second-generation codes are fast to work with, since they are
         * used internally by the Perm<8> class.
         *
         * \pre the given code is a valid second-generation permutation code;
         * see isPermCode2() for details.
         *
         * @param code the second-generation code for the new permutation.
         * @return the permutation represented by the given code.
         */
        static constexpr Perm<8> fromPermCode2(Code2 code);

        /**
         * Determines whether the given character is a valid first-generation
         * permutation code.  Valid first-generation codes can be passed to
         * setPermCode1() or fromPermCode1(), and are returned by permCode1().
         *
         * \warning This routine will incur additional overhead, since
         * Perm<8> now uses second-generation codes internally.
         * See the class notes and the routine isPermCode2() for details.
         *
         * @param code the permutation code to test.
         * @return \c true if and only if the given code is a valid
         * first-generation permutation code.
         */
        static constexpr bool isPermCode1(Code1 code);

        /**
         * Determines whether the given character is a valid second-generation
         * permutation code.  Valid second-generation codes can be passed to
         * setPermCode2() or fromPermCode2(), and are returned by permCode2().
         *
         * Second-generation codes are fast to work with, since they are
         * used internally by the Perm<8> class.
         *
         * @param code the permutation code to test.
         * @return \c true if and only if the given code is a valid
         * second-generation permutation code.
         */
        static constexpr bool isPermCode2(Code2 code);

        /**
         * Returns the image pack that represents this permutation.
         *
         * See the class notes for more information on image packs, and how
         * they are used to build the old first-generation permutation codes.
         *
         * For Perm<8>, this routine is identical to permCode1().
         *
         * @return the image pack for this permutation.
         */
        constexpr ImagePack imagePack() const;

        /**
         * Creates a permutation from the given image pack.
         *
         * See the class notes for more information on image packs, and how
         * they are used to build the old first-generation permutation codes.
         *
         * For Perm<8>, this routine is identical to fromPermCode1().
         *
         * \pre The argument \a pack is a valid image pack; see isImagePack()
         * for details.
         *
         * @param pack an image pack that describes a permutation.
         * @return the permutation represented by the given image pack.
         */
        static constexpr Perm fromImagePack(ImagePack pack);

        /**
         * Determines whether the given argument is the image pack of
         * some 8-element permutation.
         *
         * See the class notes for more information on image packs, and how
         * they are used to build the old first-generation permutation codes.
         *
         * For Perm<8>, this routine is identical to isPermCode1().
         *
         * @param pack the candidate image pack to test.
         * @return \c true if and only if \a pack is a valid image pack.
         */
        static constexpr bool isImagePack(ImagePack pack);

        /**
         * Sets this permutation to be equal to the given permutation.
         *
         * @param cloneMe the permutation whose value will be assigned
         * to this permutation.
         * @return a reference to this permutation.
         */
        Perm<8>& operator = (const Perm<8>& cloneMe) = default;

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is <i>p</i>, the
         * resulting permutation will be <i>p o q</i>, satisfying
         * <tt>(p*q)[x] == p[q[x]]</tt>.
         *
         * For permutations of five and fewer objects, composition is
         * extremely fast because it uses hard-coded lookup tables.
         * However, for Perm<8> these tables would grow too large, and so
         * instead this routine involves significant computational overhead.
         *
         * If you are going to make significant use of the Perm<8> class,
         * you should instead:
         *
         * - call precompute() to precompute a full 40320-by-40320 lookup table
         *   in advance (this will consume roughly (TODO czinn) a lot of
         *   memory); and then
         *
         * - call cachedComp() instead of the * operator to compute your
         *   compositions.
         *
         * @param q the permutation to compose this with.
         * @return the composition of both permutations.
         */
        constexpr Perm<8> operator * (const Perm<8>& q) const;

        /**
         * Returns the composition of this and the given permutation,
         * using fast precomputed lookup tables.
         *
         * The advantage of this routine is speed: calling cachedComp() is a
         * single table lookup, whereas the * operator requires significant
         * computational overhead.
         *
         * The disadvantages of this routine are that (1) you must remember
         * to call precompute() in advance, and (2) the resulting lookup table
         * will consume roughly a lot of memory for the lifetime of your program.
         *
         * The permutation that is returned is the same as you would
         * obtain by calling <tt>(*this) * q</tt>.
         *
         * \pre You \e must have called the routine precompute() at least once
         * in the lifetime of this program before using cachedComp().
         * Otherwise this routine will almost certainly crash your program.
         *
         * @param q the permutation to compose this with.
         * @return the composition of both permutations.
         */
        Perm<8> cachedComp(const Perm<8>& q) const;

        /**
         * Returns the composition of this and the given two permutations,
         * using fast precomputed lookup tables.
         *
         * The advantage of this routine is speed: calling cachedComp()
         * with two arguments requires just two table lookups, whereas using
         * the * operator twice involves significant computational overhead.
         *
         * The disadvantages of this routine are that (1) you must remember
         * to call precompute() in advance, and (2) the resulting lookup tables
         * will consume roughly 50MB of memory for the lifetime of your program.
         *
         * The permutation that is returned is the same as you would
         * obtain by calling <tt>(*this) * q * r</tt>.
         *
         * \pre You \e must have called the routine precompute() at least once
         * in the lifetime of this program before using cachedComp().
         * Otherwise this routine will almost certainly crash your program.
         *
         * @param q the first permutation to compose this with.
         * @param r the second permutation to compose this with.
         * @return the composition of both permutations.
         */
        Perm<8> cachedComp(const Perm<8>& q, const Perm<8>& r) const;

        /**
         * Finds the inverse of this permutation.
         *
         * @return the inverse of this permutation.
         */
        constexpr Perm<8> inverse() const;

        /**
         * Computes the given power of this permutation.
         *
         * This routine runs in constant time.
         *
         * For Perm<8>, this routine makes use of the composition operator *,
         * which involves significant computational overhead.  If you need
         * your powers to be fast, you can instead:
         *
         * - call precompute() to precompute a full 40320-by-40320 product table
         *   in advance (this will consume roughly a lot of memory); and then
         *
         * - call cachedPow() instead of pow() to make full use of this table,
         *   which will remove most of the overhead from this routine.
         *
         * @param exp the exponent; this may be positive, zero or negative.
         * @return this permutation raised to the power of \a exp.
         */
        constexpr Perm<8> pow(long exp) const;

        /**
         * Computes the given power of this permutation, using fast
         * precomputed lookup tables.
         *
         * This routine runs in constant time.
         *
         * The advantage of this routine is speed: calling cachedPow() removes
         * most of the significant computational overhead required by pow().
         *
         * The disadvantages of this routine are that (1) you must remember
         * to call precompute() in advance, and (2) the resulting lookup tables
         * will consume roughly a lot of memory for the lifetime of your program.
         * Note that these are the same lookup tables used by cachedComp() and
         * cachedOrder(), so if you are already using cachedComp() or
         * cachedOrder() then there is no extra cost for using this routine
         * also.
         *
         * The permutation that is returned is the same as you would
         * obtain by calling pow(exp).
         *
         * \pre You \e must have called the routine precompute() at least once
         * in the lifetime of this program before using cachedPow().
         * Otherwise this routine will almost certainly crash your program.
         *
         * @param exp the exponent; this may be positive, zero or negative.
         * @return this permutation raised to the power of \a exp.
         */
        Perm<8> cachedPow(long exp) const;

        /**
         * Returns the order of this permutation.
         *
         * In other words; this routine returns the smallest positive
         * integer \a k for which the <i>k</i>th power of this
         * permutation is the identity.
         *
         * Unlike the smaller permutation classes, Perm<8>::order() does
         * not use precomputed tables; instead it computes orders on the fly.
         * If you need your order computation to be faster, you can instead:
         *
         * - call precompute() to precompute a full table of orders in advance
         *   (though this will also compute the much larger 40320-by-40320 table
         *   of products, which consumes roughly a lot of memory); and then
         *
         * - call cachedOrder() instead of order(), which will now become a
         *   fast table lookup.
         *
         * @return the order of this permutation.
         */
        constexpr int order() const;

        /**
         * Returns the order of this permutation, using fast precomputed
         * lookup tables.
         *
         * In other words; this routine returns the smallest positive
         * integer \a k for which the <i>k</i>th power of this
         * permutation is the identity.
         *
         * The advantage of this routine is speed: calling cachedOrder()
         * removes most of the computational overhead required by order().
         *
         * The disadvantages of this routine are that (1) you must remember
         * to call precompute() in advance, and (2) the resulting lookup tables
         * will consume roughly a lot of memory for the lifetime of your program.
         * Note that these are the same lookup tables used by cachedComp()
         * and cachedPow(), so if you are already using cachedComp() or
         * cachedPow() then there is no extra cost for using this routine also.
         *
         * The permutation that is returned is the same as you would
         * obtain by calling order().
         *
         * \pre You \e must have called the routine precompute() at least once
         * in the lifetime of this program before using cachedOrder().
         * Otherwise this routine will almost certainly crash your program.
         *
         * @return the order of this permutation.
         */
        int cachedOrder() const;

        /**
         * Finds the reverse of this permutation.
         *
         * Here \e reverse means that we reverse the images of 0,...,7.
         * In other words, if permutation \a q is the
         * reverse of \a p, then <tt>p[i] == q[7 - i]</tt> for all \a i.
         */
        constexpr Perm<8> reverse() const;

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
         * should be between 0 and 7 inclusive.
         * @return the image of \a source.
         */
        constexpr int operator[](int source) const;

        /**
         * Determines the preimage of the given integer under this
         * permutation.
         *
         * @param image the integer whose preimage we wish to find.  This
         * should be between 0 and 7 inclusive.
         * @return the preimage of \a image.
         */
        constexpr int pre(int image) const;

        /**
         * Determines if this is equal to the given permutation.
         * This is true if and only if both permutations have the same
         * images for 0, 1, 2, 3, 4, 5, 6 and 7.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * are equal.
         */
        constexpr bool operator == (const Perm<8>& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for at least one of 0, 1, 2, 3, 4, 5, 7 or 8.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * differ.
         */
        constexpr bool operator != (const Perm<8>& other) const;

        /**
         * Lexicographically compares the images of (0,1,2,3,4,5,6,7) under this
         * and the given permutation.
         *
         * Note that this does \e not yield the same ordering of permutations
         * as used by the less-than and increment operators.  Moreover,
         * compareWith() is slower than the less-than operator to compute.
         *
         * @param other the permutation with which to compare this.
         * @return -1 if this permutation produces a smaller image, 0 if
         * the permutations are equal and 1 if this permutation produces
         * a greater image.
         */
        constexpr int compareWith(const Perm<8>& other) const;

        /**
         * Determines if this is the identity permutation.
         * This is true if and only if each of 0, 1, 2, 3, 4, 5, 6 and 7 is
         * mapped to itself.
         *
         * @return \c true if and only if this is the identity
         * permutation.
         */
        constexpr bool isIdentity() const;

        /**
         * A preincrement operator that changes this to be the next permutation
         * in the array Perm<8>::Sn.  If this is the last such permutation
         * then this will wrap around to become the first permutation in
         * Perm<8>::Sn, which is the identity.
         *
         * \ifacespython Not present, although the postincrement operator is
         * present in python as the member function inc().
         *
         * @return a reference to this permutation after the increment.
         */
        Perm<8>& operator ++();

        /**
         * A postincrement operator that changes this to be the next permutation
         * in the array Perm<8>::Sn.  If this is the last such permutation
         * then this will wrap around to become the first permutation in
         * Perm<8>::Sn, which is the identity.
         *
         * \ifacespython This routine is named inc() since python does
         * not support the increment operator.
         *
         * @return a copy of this permutation before the increment took place.
         */
        constexpr Perm<8> operator ++(int);

        /**
         * Determines if this appears earlier than the given permutation
         * in the array Perm<8>::Sn.
         *
         * Note that this is \e not the same ordering of permutations as
         * the ordering implied by compareWith().  This is, however,
         * consistent with the ordering implied by the ++ operators,
         * and this order is also faster to compute than compareWith().
         *
         * @param rhs the permutation to compare this against.
         * @return \c true if and only if this appears before \a rhs in \a Sn.
         */
        constexpr bool operator < (const Perm<8>& rhs) const;

        /**
         * Returns the <i>i</i>th rotation.
         * This maps <i>k</i> to <i>k</i>&nbsp;+&nbsp;<i>i</i> (mod 8)
         * for all \a k.
         *
         * @param i the image of 0; this must be between 0 and 7 inclusive.
         * @return the <i>i</i>th rotation.
         */
        static constexpr Perm rot(int i);

        /**
         * Returns a random permutation on seven elements.
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
         * Returns a random permutation on seven elements, using the
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
         * Returns a string representation of this permutation.
         * The representation will consist of seven adjacent digits
         * representing the images of 0, 1, 2, 3, 4, 5, 6 and 7 respectively.
         * An example of a string representation is <tt>30457261</tt>.
         *
         * @return a string representation of this permutation.
         */
        std::string str() const;

        /**
         * Returns a prefix of the string representation of this permutation,
         * containing only the images of the first \a len integers.
         *
         * @param len the length of the prefix required; this must be
         * between 0 and 8 inclusive.
         * @return the corresponding prefix of the string representation
         * of this permutation.
         */
        std::string trunc(int len) const;

        /**
         * Writes the tight encoding of this permutation to the given output
         * stream.  See the page on \ref tight "tight encodings" for details.
         *
         * For all permutation classes Perm<n>, the tight encoding is based on
         * the index into the full permutation group \a S_n.  For smaller
         * permutation classes (\a n &le; 8), such encodings are very fast to
         * work with since the \a S_n index is used as the internal permutation
         * code.  For larger permutation classes however (8 &le; \a n &le; 16),
         * the \a S_n index requires some non-trivial work to compute.
         *
         * \ifacespython Not present; use tightEncoding() instead, which
         * returns a string.
         *
         * @param out the output stream to which the encoded string will
         * be written.
         */
        void tightEncode(std::ostream& out) const;

        /**
         * Returns the tight encoding of this permutation.
         * See the page on \ref tight "tight encodings" for details.
         *
         * For all permutation classes Perm<n>, the tight encoding is based on
         * the index into the full permutation group \a S_n.  For smaller
         * permutation classes (\a n &le; 8), such encodings are very fast to
         * work with since the \a S_n index is used as the internal permutation
         * code.  For larger permutation classes however (9 &le; \a n &le; 16),
         * the \a S_n index requires some non-trivial work to compute.
         *
         * @return the resulting encoded string.
         */
        std::string tightEncoding() const;

        /**
         * Reconstructs a permutation from its given tight encoding.
         * See the page on \ref tight "tight encodings" for details.
         *
         * The tight encoding will be given as a string.  If this string
         * contains leading whitespace or any trailing characters at all
         * (including trailing whitespace), then it will be treated as
         * an invalid encoding (i.e., this routine will throw an exception).
         *
         * Tight encodings are fast to work with for small permutation classes
         * (\a n &le; 8), but slower for larger permutation classes
         * (9 &le; \a n &le; 16).  See tightEncoding() for further details.
         *
         * \exception InvalidArgument the given string is not a tight encoding
         * of a 8-element permutation.
         *
         * @param enc the tight encoding for a 8-element permutation.
         * @return the permutation represented by the given tight encoding.
         */
        static Perm tightDecoding(const std::string& enc);

        /**
         * Reconstructs a permutation from its given tight encoding.
         * See the page on \ref tight "tight encodings" for details.
         *
         * The tight encoding will be read from the given input stream.
         * If the input stream contains leading whitespace then it will be
         * treated as an invalid encoding (i.e., this routine will throw an
         * exception).  The input routine \e may contain further data: if this
         * routine is successful then the input stream will be left positioned
         * immediately after the encoding, without skipping any trailing
         * whitespace.
         *
         * Tight encodings are fast to work with for small permutation classes
         * (\a n &le; 8), but slower for larger permutation classes
         * (9 &le; \a n &le; 16).  See tightEncoding() for further details.
         *
         * \exception InvalidInput the given input stream does not begin with
         * a tight encoding of a 8-element permutation.
         *
         * \ifacespython Not present; use tightDecoding() instead, which takes
         * a string as its argument.
         *
         * @param input an input stream that begins with the tight encoding
         * for a 8-element permutation.
         * @return the permutation represented by the given tight encoding.
         */
        static Perm tightDecode(std::istream& input);

        /**
         * Resets the images of all integers from \a from onwards to the
         * identity map.
         *
         * Specifically, for each \a i in the range <i>from</i>,...,7,
         * this routine will ensure that <tt>image[i] == i</tt>.  The images of
         * 0,1,...,<i>from</i>-1 will not be altered.
         *
         * \pre The images of <i>from</i>,...,7 are exactly
         * <i>from</i>,...,7, but possibly in a different order.
         *
         * @param from the first integer whose image should be reset.
         * This must be between 0 and 8 inclusive.
         */
        void clear(unsigned from);

        /**
         * Returns the index of this permutation in the Perm<8>::Sn array.
         *
         * See Sn for further information on how these permutations are indexed.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<8>::Sn[i].  This will be between 0 and 40319 inclusive.
         */
        constexpr Index SnIndex() const;

        /**
         * Returns the index of this permutation in the Perm<8>::S8 array.
         *
         * This is a dimension-specific alias for SnIndex().  In general,
         * for every \a n there will be a member function Perm<n>::SnIndex();
         * however, these numerical aliases Perm<2>::S2Index(), ...,
         * Perm<8>::S8Index() are only available for small \a n.
         *
         * See Sn for further information on how these permutations are indexed.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<8>::S8[i].  This will be between 0 and 40319 inclusive.
         */
        constexpr Index S8Index() const;

        /**
         * Returns the lexicographical index of this permutation.  This will
         * be the index of this permutation in the Perm<8>::orderedSn array.
         *
         * See orderedSn for further information on lexicographical ordering.
         *
         * @return the lexicographical index of this permutation.
         * This will be between 0 and 40319 inclusive.
         */
        constexpr Index orderedSnIndex() const;

        /**
         * Returns the lexicographical index of this permutation.  This will
         * be the index of this permutation in the Perm<8>::orderedSn array.
         *
         * This is a dimension-specific alias for orderedSnIndex().
         * In general, for every \a n there will be a member function
         * Perm<n>::orderedSnIndex(); however, these numerical aliases
         * Perm<2>::orderedS2Index(), ..., Perm<8>::orderedS8Index() are
         * only available for small \a n.
         *
         * See orderedSn for further information on lexicographical ordering.
         *
         * @return the lexicographical index of this permutation.
         * This will be between 0 and 40319 inclusive.
         */
        constexpr Index orderedS8Index() const;

        /**
         * Extends a <i>k</i>-element permutation to a 8-element permutation,
         * where 2 &le; \a k &lt; 8.
         *
         * The resulting permutation will map 0,...,<i>k</i>-1 to their
         * respective images under \a p, and will map the "unused" elements
         * <i>k</i>,...,7 to themselves.
         *
         * \tparam k the number of elements for the input permutation;
         * this must be 2, 3, 4, 5, 6 or 7.
         *
         * @param p a permutation on \a k elements.
         * @return the same permutation expressed as a permutation on
         * seven elements.
         */
        template <int k>
        static constexpr Perm<8> extend(Perm<k> p);

        /**
         * Restricts a <i>k</i>-element permutation to a 8-element
         * permutation, where \a k &gt; 8.
         *
         * The resulting permutation will map 0,...,7 to their
         * respective images under \a p, and will ignore the "unused" images
         * \a p[8],...,\a p[<i>k</i>-1].
         *
         * \pre The given permutation maps 0,...,7 to 0,...,7 in some order.
         *
         * \tparam k the number of elements for the input permutation;
         * this must be strictly greater than 8.
         *
         * @param p a permutation on \a k elements.
         * @return the same permutation restricted to a permutation on
         * 8 elements.
         */
        template <int k>
        static constexpr Perm<8> contract(Perm<k> p);

        /**
         * Is this permutation minimal in its conjugacy class?
         *
         * Here "minimal" means that, amongst all its conjugates, this
         * permutation has the smallest index in the array Perm<8>::Sn.
         *
         * See Sn for further information on how permutations are indexed.
         *
         * This routine is extremely fast for Perm<8>, since it essentially
         * uses a hard-coded lookup table.
         *
         * @return \c true if and only if this permutation is minimal in its
         * conjugacy class.
         */
        constexpr bool isConjugacyMinimal() const;

    private:
        /**
         * Contains the inverses of the permutations in the array \a S8.
         *
         * Specifically, the inverse of permutation <tt>S8[i]</tt> is
         * the permutation <tt>S8[ invS8[i] ]</tt>.
         */
        static constexpr Code2 invS8[40320] = {
            0, 1, 4, 3, 2, 5, 6, 7, 12, 19, 
            18, 13, 8, 11, 20, 15, 16, 23, 10, 9, 
            14, 21, 22, 17, 24, 25, 26, 29, 28, 27, 
            48, 49, 96, 73, 72, 97, 52, 51, 74, 99, 
            100, 77, 50, 53, 98, 75, 76, 101, 30, 31, 
            42, 37, 36, 43, 54, 55, 78, 103, 102, 79, 
            60, 67, 108, 85, 90, 115, 66, 61, 84, 109, 
            114, 91, 34, 33, 38, 45, 46, 41, 56, 59, 
            104, 81, 82, 107, 68, 63, 86, 111, 116, 93, 
            64, 71, 112, 89, 94, 119, 32, 35, 44, 39, 
            40, 47, 58, 57, 80, 105, 106, 83, 62, 69, 
            110, 87, 92, 117, 70, 65, 88, 113, 118, 95, 
            120, 121, 122, 125, 124, 123, 126, 127, 138, 133, 
            132, 139, 130, 129, 134, 141, 142, 137, 128, 131, 
            140, 135, 136, 143, 240, 241, 244, 243, 242, 245, 
            360, 361, 480, 601, 600, 481, 362, 365, 602, 483, 
            484, 605, 364, 363, 482, 603, 604, 485, 246, 247, 
            252, 259, 258, 253, 366, 367, 606, 487, 486, 607, 
            378, 373, 492, 613, 618, 499, 372, 379, 612, 493, 
            498, 619, 248, 251, 260, 255, 256, 263, 370, 369, 
            488, 609, 610, 491, 374, 381, 614, 495, 500, 621, 
            382, 377, 496, 617, 622, 503, 250, 249, 254, 261, 
            262, 257, 368, 371, 608, 489, 490, 611, 380, 375, 
            494, 615, 620, 501, 376, 383, 616, 497, 502, 623, 
            144, 145, 148, 147, 146, 149, 168, 169, 192, 217, 
            216, 193, 170, 173, 218, 195, 196, 221, 172, 171, 
            194, 219, 220, 197, 264, 265, 266, 269, 268, 267, 
            384, 385, 624, 505, 504, 625, 388, 387, 506, 627, 
            628, 509, 386, 389, 626, 507, 508, 629, 288, 289, 
            336, 313, 312, 337, 408, 409, 528, 649, 648, 529, 
            432, 457, 672, 553, 576, 697, 456, 433, 552, 673, 
            696, 577, 292, 291, 314, 339, 340, 317, 410, 413, 
            650, 531, 532, 653, 458, 435, 554, 675, 698, 579, 
            436, 461, 676, 557, 580, 701, 290, 293, 338, 315, 
            316, 341, 412, 411, 530, 651, 652, 533, 434, 459, 
            674, 555, 578, 699, 460, 437, 556, 677, 700, 581, 
            150, 151, 156, 163, 162, 157, 174, 175, 222, 199, 
            198, 223, 186, 181, 204, 229, 234, 211, 180, 187, 
            228, 205, 210, 235, 270, 271, 282, 277, 276, 283, 
            390, 391, 510, 631, 630, 511, 396, 403, 636, 517, 
            522, 643, 402, 397, 516, 637, 642, 523, 294, 295, 
            318, 343, 342, 319, 414, 415, 654, 535, 534, 655, 
            462, 439, 558, 679, 702, 583, 438, 463, 678, 559, 
            582, 703, 300, 307, 348, 325, 330, 355, 426, 421, 
            540, 661, 666, 547, 444, 469, 684, 565, 588, 709, 
            474, 451, 570, 691, 714, 595, 306, 301, 324, 349, 
            354, 331, 420, 427, 660, 541, 546, 667, 468, 445, 
            564, 685, 708, 589, 450, 475, 690, 571, 594, 715, 
            152, 155, 164, 159, 160, 167, 178, 177, 200, 225, 
            226, 203, 182, 189, 230, 207, 212, 237, 190, 185, 
            208, 233, 238, 215, 274, 273, 278, 285, 286, 281, 
            392, 395, 632, 513, 514, 635, 404, 399, 518, 639, 
            644, 525, 400, 407, 640, 521, 526, 647, 296, 299, 
            344, 321, 322, 347, 418, 417, 536, 657, 658, 539, 
            440, 465, 680, 561, 584, 705, 466, 443, 562, 683, 
            706, 587, 308, 303, 326, 351, 356, 333, 422, 429, 
            662, 543, 548, 669, 470, 447, 566, 687, 710, 591, 
            452, 477, 692, 573, 596, 717, 304, 311, 352, 329, 
            334, 359, 430, 425, 544, 665, 670, 551, 448, 473, 
            688, 569, 592, 713, 478, 455, 574, 695, 718, 599, 
            154, 153, 158, 165, 166, 161, 176, 179, 224, 201, 
            202, 227, 188, 183, 206, 231, 236, 213, 184, 191, 
            232, 209, 214, 239, 272, 275, 284, 279, 280, 287, 
            394, 393, 512, 633, 634, 515, 398, 405, 638, 519, 
            524, 645, 406, 401, 520, 641, 646, 527, 298, 297, 
            320, 345, 346, 323, 416, 419, 656, 537, 538, 659, 
            464, 441, 560, 681, 704, 585, 442, 467, 682, 563, 
            586, 707, 302, 309, 350, 327, 332, 357, 428, 423, 
            542, 663, 668, 549, 446, 471, 686, 567, 590, 711, 
            476, 453, 572, 693, 716, 597, 310, 305, 328, 353, 
            358, 335, 424, 431, 664, 545, 550, 671, 472, 449, 
            568, 689, 712, 593, 454, 479, 694, 575, 598, 719, 
            720, 721, 722, 725, 724, 723, 726, 727, 738, 733, 
            732, 739, 730, 729, 734, 741, 742, 737, 728, 731, 
            740, 735, 736, 743, 744, 745, 748, 747, 746, 749, 
            768, 769, 792, 817, 816, 793, 770, 773, 818, 795, 
            796, 821, 772, 771, 794, 819, 820, 797, 750, 751, 
            756, 763, 762, 757, 774, 775, 822, 799, 798, 823, 
            786, 781, 804, 829, 834, 811, 780, 787, 828, 805, 
            810, 835, 752, 755, 764, 759, 760, 767, 778, 777, 
            800, 825, 826, 803, 782, 789, 830, 807, 812, 837, 
            790, 785, 808, 833, 838, 815, 754, 753, 758, 765, 
            766, 761, 776, 779, 824, 801, 802, 827, 788, 783, 
            806, 831, 836, 813, 784, 791, 832, 809, 814, 839, 
            1440, 1441, 1444, 1443, 1442, 1445, 1446, 1447, 1452, 1459, 
            1458, 1453, 1448, 1451, 1460, 1455, 1456, 1463, 1450, 1449, 
            1454, 1461, 1462, 1457, 2160, 2161, 2162, 2165, 2164, 2163, 
            2880, 2881, 4320, 3601, 3600, 4321, 2884, 2883, 3602, 4323, 
            4324, 3605, 2882, 2885, 4322, 3603, 3604, 4325, 2166, 2167, 
            2178, 2173, 2172, 2179, 2886, 2887, 3606, 4327, 4326, 3607, 
            2892, 2899, 4332, 3613, 3618, 4339, 2898, 2893, 3612, 4333, 
            4338, 3619, 2170, 2169, 2174, 2181, 2182, 2177, 2888, 2891, 
            4328, 3609, 3610, 4331, 2900, 2895, 3614, 4335, 4340, 3621, 
            2896, 2903, 4336, 3617, 3622, 4343, 2168, 2171, 2180, 2175, 
            2176, 2183, 2890, 2889, 3608, 4329, 4330, 3611, 2894, 2901, 
            4334, 3615, 3620, 4341, 2902, 2897, 3616, 4337, 4342, 3623, 
            1464, 1465, 1466, 1469, 1468, 1467, 1488, 1489, 1536, 1513, 
            1512, 1537, 1492, 1491, 1514, 1539, 1540, 1517, 1490, 1493, 
            1538, 1515, 1516, 1541, 2184, 2185, 2188, 2187, 2186, 2189, 
            2904, 2905, 3624, 4345, 4344, 3625, 2906, 2909, 4346, 3627, 
            3628, 4349, 2908, 2907, 3626, 4347, 4348, 3629, 2208, 2209, 
            2232, 2257, 2256, 2233, 2928, 2929, 4368, 3649, 3648, 4369, 
            2976, 2953, 3672, 4393, 4416, 3697, 2952, 2977, 4392, 3673, 
            3696, 4417, 2210, 2213, 2258, 2235, 2236, 2261, 2932, 2931, 
            3650, 4371, 4372, 3653, 2954, 2979, 4394, 3675, 3698, 4419, 
            2980, 2957, 3676, 4397, 4420, 3701, 2212, 2211, 2234, 2259, 
            2260, 2237, 2930, 2933, 4370, 3651, 3652, 4373, 2978, 2955, 
            3674, 4395, 4418, 3699, 2956, 2981, 4396, 3677, 3700, 4421, 
            1470, 1471, 1482, 1477, 1476, 1483, 1494, 1495, 1518, 1543, 
            1542, 1519, 1500, 1507, 1548, 1525, 1530, 1555, 1506, 1501, 
            1524, 1549, 1554, 1531, 2190, 2191, 2196, 2203, 2202, 2197, 
            2910, 2911, 4350, 3631, 3630, 4351, 2922, 2917, 3636, 4357, 
            4362, 3643, 2916, 2923, 4356, 3637, 3642, 4363, 2214, 2215, 
            2262, 2239, 2238, 2263, 2934, 2935, 3654, 4375, 4374, 3655, 
            2958, 2983, 4398, 3679, 3702, 4423, 2982, 2959, 3678, 4399, 
            4422, 3703, 2226, 2221, 2244, 2269, 2274, 2251, 2940, 2947, 
            4380, 3661, 3666, 4387, 2988, 2965, 3684, 4405, 4428, 3709, 
            2970, 2995, 4410, 3691, 3714, 4435, 2220, 2227, 2268, 2245, 
            2250, 2275, 2946, 2941, 3660, 4381, 4386, 3667, 2964, 2989, 
            4404, 3685, 3708, 4429, 2994, 2971, 3690, 4411, 4434, 3715, 
            1474, 1473, 1478, 1485, 1486, 1481, 1496, 1499, 1544, 1521, 
            1522, 1547, 1508, 1503, 1526, 1551, 1556, 1533, 1504, 1511, 
            1552, 1529, 1534, 1559, 2192, 2195, 2204, 2199, 2200, 2207, 
            2914, 2913, 3632, 4353, 4354, 3635, 2918, 2925, 4358, 3639, 
            3644, 4365, 2926, 2921, 3640, 4361, 4366, 3647, 2218, 2217, 
            2240, 2265, 2266, 2243, 2936, 2939, 4376, 3657, 3658, 4379, 
            2984, 2961, 3680, 4401, 4424, 3705, 2962, 2987, 4402, 3683, 
            3706, 4427, 2222, 2229, 2270, 2247, 2252, 2277, 2948, 2943, 
            3662, 4383, 4388, 3669, 2966, 2991, 4406, 3687, 3710, 4431, 
            2996, 2973, 3692, 4413, 4436, 3717, 2230, 2225, 2248, 2273, 
            2278, 2255, 2944, 2951, 4384, 3665, 3670, 4391, 2992, 2969, 
            3688, 4409, 4432, 3713, 2974, 2999, 4414, 3695, 3718, 4439, 
            1472, 1475, 1484, 1479, 1480, 1487, 1498, 1497, 1520, 1545, 
            1546, 1523, 1502, 1509, 1550, 1527, 1532, 1557, 1510, 1505, 
            1528, 1553, 1558, 1535, 2194, 2193, 2198, 2205, 2206, 2201, 
            2912, 2915, 4352, 3633, 3634, 4355, 2924, 2919, 3638, 4359, 
            4364, 3645, 2920, 2927, 4360, 3641, 3646, 4367, 2216, 2219, 
            2264, 2241, 2242, 2267, 2938, 2937, 3656, 4377, 4378, 3659, 
            2960, 2985, 4400, 3681, 3704, 4425, 2986, 2963, 3682, 4403, 
            4426, 3707, 2228, 2223, 2246, 2271, 2276, 2253, 2942, 2949, 
            4382, 3663, 3668, 4389, 2990, 2967, 3686, 4407, 4430, 3711, 
            2972, 2997, 4412, 3693, 3716, 4437, 2224, 2231, 2272, 2249, 
            2254, 2279, 2950, 2945, 3664, 4385, 4390, 3671, 2968, 2993, 
            4408, 3689, 3712, 4433, 2998, 2975, 3694, 4415, 4438, 3719, 
            840, 841, 844, 843, 842, 845, 846, 847, 852, 859, 
            858, 853, 848, 851, 860, 855, 856, 863, 850, 849, 
            854, 861, 862, 857, 960, 961, 962, 965, 964, 963, 
            1080, 1081, 1320, 1201, 1200, 1321, 1084, 1083, 1202, 1323, 
            1324, 1205, 1082, 1085, 1322, 1203, 1204, 1325, 966, 967, 
            978, 973, 972, 979, 1086, 1087, 1206, 1327, 1326, 1207, 
            1092, 1099, 1332, 1213, 1218, 1339, 1098, 1093, 1212, 1333, 
            1338, 1219, 970, 969, 974, 981, 982, 977, 1088, 1091, 
            1328, 1209, 1210, 1331, 1100, 1095, 1214, 1335, 1340, 1221, 
            1096, 1103, 1336, 1217, 1222, 1343, 968, 971, 980, 975, 
            976, 983, 1090, 1089, 1208, 1329, 1330, 1211, 1094, 1101, 
            1334, 1215, 1220, 1341, 1102, 1097, 1216, 1337, 1342, 1223, 
            1560, 1561, 1562, 1565, 1564, 1563, 1566, 1567, 1578, 1573, 
            1572, 1579, 1570, 1569, 1574, 1581, 1582, 1577, 1568, 1571, 
            1580, 1575, 1576, 1583, 2280, 2281, 2284, 2283, 2282, 2285, 
            3000, 3001, 3720, 4441, 4440, 3721, 3002, 3005, 4442, 3723, 
            3724, 4445, 3004, 3003, 3722, 4443, 4444, 3725, 2286, 2287, 
            2292, 2299, 2298, 2293, 3006, 3007, 4446, 3727, 3726, 4447, 
            3018, 3013, 3732, 4453, 4458, 3739, 3012, 3019, 4452, 3733, 
            3738, 4459, 2288, 2291, 2300, 2295, 2296, 2303, 3010, 3009, 
            3728, 4449, 4450, 3731, 3014, 3021, 4454, 3735, 3740, 4461, 
            3022, 3017, 3736, 4457, 4462, 3743, 2290, 2289, 2294, 2301, 
            2302, 2297, 3008, 3011, 4448, 3729, 3730, 4451, 3020, 3015, 
            3734, 4455, 4460, 3741, 3016, 3023, 4456, 3737, 3742, 4463, 
            1680, 1681, 1684, 1683, 1682, 1685, 1800, 1801, 1920, 2041, 
            2040, 1921, 1802, 1805, 2042, 1923, 1924, 2045, 1804, 1803, 
            1922, 2043, 2044, 1925, 2400, 2401, 2402, 2405, 2404, 2403, 
            3120, 3121, 4560, 3841, 3840, 4561, 3124, 3123, 3842, 4563, 
            4564, 3845, 3122, 3125, 4562, 3843, 3844, 4565, 2520, 2521, 
            2760, 2641, 2640, 2761, 3240, 3241, 3960, 4681, 4680, 3961, 
            3360, 3481, 4800, 4081, 4200, 4921, 3480, 3361, 4080, 4801, 
            4920, 4201, 2524, 2523, 2642, 2763, 2764, 2645, 3242, 3245, 
            4682, 3963, 3964, 4685, 3482, 3363, 4082, 4803, 4922, 4203, 
            3364, 3485, 4804, 4085, 4204, 4925, 2522, 2525, 2762, 2643, 
            2644, 2765, 3244, 3243, 3962, 4683, 4684, 3965, 3362, 3483, 
            4802, 4083, 4202, 4923, 3484, 3365, 4084, 4805, 4924, 4205, 
            1686, 1687, 1692, 1699, 1698, 1693, 1806, 1807, 2046, 1927, 
            1926, 2047, 1818, 1813, 1932, 2053, 2058, 1939, 1812, 1819, 
            2052, 1933, 1938, 2059, 2406, 2407, 2418, 2413, 2412, 2419, 
            3126, 3127, 3846, 4567, 4566, 3847, 3132, 3139, 4572, 3853, 
            3858, 4579, 3138, 3133, 3852, 4573, 4578, 3859, 2526, 2527, 
            2646, 2767, 2766, 2647, 3246, 3247, 4686, 3967, 3966, 4687, 
            3486, 3367, 4086, 4807, 4926, 4207, 3366, 3487, 4806, 4087, 
            4206, 4927, 2532, 2539, 2772, 2653, 2658, 2779, 3258, 3253, 
            3972, 4693, 4698, 3979, 3372, 3493, 4812, 4093, 4212, 4933, 
            3498, 3379, 4098, 4819, 4938, 4219, 2538, 2533, 2652, 2773, 
            2778, 2659, 3252, 3259, 4692, 3973, 3978, 4699, 3492, 3373, 
            4092, 4813, 4932, 4213, 3378, 3499, 4818, 4099, 4218, 4939, 
            1688, 1691, 1700, 1695, 1696, 1703, 1810, 1809, 1928, 2049, 
            2050, 1931, 1814, 1821, 2054, 1935, 1940, 2061, 1822, 1817, 
            1936, 2057, 2062, 1943, 2410, 2409, 2414, 2421, 2422, 2417, 
            3128, 3131, 4568, 3849, 3850, 4571, 3140, 3135, 3854, 4575, 
            4580, 3861, 3136, 3143, 4576, 3857, 3862, 4583, 2528, 2531, 
            2768, 2649, 2650, 2771, 3250, 3249, 3968, 4689, 4690, 3971, 
            3368, 3489, 4808, 4089, 4208, 4929, 3490, 3371, 4090, 4811, 
            4930, 4211, 2540, 2535, 2654, 2775, 2780, 2661, 3254, 3261, 
            4694, 3975, 3980, 4701, 3494, 3375, 4094, 4815, 4934, 4215, 
            3380, 3501, 4820, 4101, 4220, 4941, 2536, 2543, 2776, 2657, 
            2662, 2783, 3262, 3257, 3976, 4697, 4702, 3983, 3376, 3497, 
            4816, 4097, 4216, 4937, 3502, 3383, 4102, 4823, 4942, 4223, 
            1690, 1689, 1694, 1701, 1702, 1697, 1808, 1811, 2048, 1929, 
            1930, 2051, 1820, 1815, 1934, 2055, 2060, 1941, 1816, 1823, 
            2056, 1937, 1942, 2063, 2408, 2411, 2420, 2415, 2416, 2423, 
            3130, 3129, 3848, 4569, 4570, 3851, 3134, 3141, 4574, 3855, 
            3860, 4581, 3142, 3137, 3856, 4577, 4582, 3863, 2530, 2529, 
            2648, 2769, 2770, 2651, 3248, 3251, 4688, 3969, 3970, 4691, 
            3488, 3369, 4088, 4809, 4928, 4209, 3370, 3491, 4810, 4091, 
            4210, 4931, 2534, 2541, 2774, 2655, 2660, 2781, 3260, 3255, 
            3974, 4695, 4700, 3981, 3374, 3495, 4814, 4095, 4214, 4935, 
            3500, 3381, 4100, 4821, 4940, 4221, 2542, 2537, 2656, 2777, 
            2782, 2663, 3256, 3263, 4696, 3977, 3982, 4703, 3496, 3377, 
            4096, 4817, 4936, 4217, 3382, 3503, 4822, 4103, 4222, 4943, 
            864, 865, 866, 869, 868, 867, 888, 889, 936, 913, 
            912, 937, 892, 891, 914, 939, 940, 917, 890, 893, 
            938, 915, 916, 941, 984, 985, 988, 987, 986, 989, 
            1104, 1105, 1224, 1345, 1344, 1225, 1106, 1109, 1346, 1227, 
            1228, 1349, 1108, 1107, 1226, 1347, 1348, 1229, 1008, 1009, 
            1032, 1057, 1056, 1033, 1128, 1129, 1368, 1249, 1248, 1369, 
            1176, 1153, 1272, 1393, 1416, 1297, 1152, 1177, 1392, 1273, 
            1296, 1417, 1010, 1013, 1058, 1035, 1036, 1061, 1132, 1131, 
            1250, 1371, 1372, 1253, 1154, 1179, 1394, 1275, 1298, 1419, 
            1180, 1157, 1276, 1397, 1420, 1301, 1012, 1011, 1034, 1059, 
            1060, 1037, 1130, 1133, 1370, 1251, 1252, 1373, 1178, 1155, 
            1274, 1395, 1418, 1299, 1156, 1181, 1396, 1277, 1300, 1421, 
            1584, 1585, 1588, 1587, 1586, 1589, 1608, 1609, 1632, 1657, 
            1656, 1633, 1610, 1613, 1658, 1635, 1636, 1661, 1612, 1611, 
            1634, 1659, 1660, 1637, 2304, 2305, 2306, 2309, 2308, 2307, 
            3024, 3025, 4464, 3745, 3744, 4465, 3028, 3027, 3746, 4467, 
            4468, 3749, 3026, 3029, 4466, 3747, 3748, 4469, 2328, 2329, 
            2376, 2353, 2352, 2377, 3048, 3049, 3768, 4489, 4488, 3769, 
            3072, 3097, 4512, 3793, 3816, 4537, 3096, 3073, 3792, 4513, 
            4536, 3817, 2332, 2331, 2354, 2379, 2380, 2357, 3050, 3053, 
            4490, 3771, 3772, 4493, 3098, 3075, 3794, 4515, 4538, 3819, 
            3076, 3101, 4516, 3797, 3820, 4541, 2330, 2333, 2378, 2355, 
            2356, 2381, 3052, 3051, 3770, 4491, 4492, 3773, 3074, 3099, 
            4514, 3795, 3818, 4539, 3100, 3077, 3796, 4517, 4540, 3821, 
            1704, 1705, 1706, 1709, 1708, 1707, 1824, 1825, 2064, 1945, 
            1944, 2065, 1828, 1827, 1946, 2067, 2068, 1949, 1826, 1829, 
            2066, 1947, 1948, 2069, 2424, 2425, 2428, 2427, 2426, 2429, 
            3144, 3145, 3864, 4585, 4584, 3865, 3146, 3149, 4586, 3867, 
            3868, 4589, 3148, 3147, 3866, 4587, 4588, 3869, 2544, 2545, 
            2664, 2785, 2784, 2665, 3264, 3265, 4704, 3985, 3984, 4705, 
            3504, 3385, 4104, 4825, 4944, 4225, 3384, 3505, 4824, 4105, 
            4224, 4945, 2546, 2549, 2786, 2667, 2668, 2789, 3268, 3267, 
            3986, 4707, 4708, 3989, 3386, 3507, 4826, 4107, 4226, 4947, 
            3508, 3389, 4108, 4829, 4948, 4229, 2548, 2547, 2666, 2787, 
            2788, 2669, 3266, 3269, 4706, 3987, 3988, 4709, 3506, 3387, 
            4106, 4827, 4946, 4227, 3388, 3509, 4828, 4109, 4228, 4949, 
            1728, 1729, 1776, 1753, 1752, 1777, 1848, 1849, 1968, 2089, 
            2088, 1969, 1872, 1897, 2112, 1993, 2016, 2137, 1896, 1873, 
            1992, 2113, 2136, 2017, 2448, 2449, 2472, 2497, 2496, 2473, 
            3168, 3169, 4608, 3889, 3888, 4609, 3216, 3193, 3912, 4633, 
            4656, 3937, 3192, 3217, 4632, 3913, 3936, 4657, 2568, 2569, 
            2808, 2689, 2688, 2809, 3288, 3289, 4008, 4729, 4728, 4009, 
            3408, 3529, 4848, 4129, 4248, 4969, 3528, 3409, 4128, 4849, 
            4968, 4249, 2616, 2593, 2712, 2833, 2856, 2737, 3312, 3337, 
            4752, 4033, 4056, 4777, 3552, 3433, 4152, 4873, 4992, 4273, 
            3456, 3577, 4896, 4177, 4296, 5017, 2592, 2617, 2832, 2713, 
            2736, 2857, 3336, 3313, 4032, 4753, 4776, 4057, 3432, 3553, 
            4872, 4153, 4272, 4993, 3576, 3457, 4176, 4897, 5016, 4297, 
            1732, 1731, 1754, 1779, 1780, 1757, 1850, 1853, 2090, 1971, 
            1972, 2093, 1898, 1875, 1994, 2115, 2138, 2019, 1876, 1901, 
            2116, 1997, 2020, 2141, 2450, 2453, 2498, 2475, 2476, 2501, 
            3172, 3171, 3890, 4611, 4612, 3893, 3194, 3219, 4634, 3915, 
            3938, 4659, 3220, 3197, 3916, 4637, 4660, 3941, 2572, 2571, 
            2690, 2811, 2812, 2693, 3290, 3293, 4730, 4011, 4012, 4733, 
            3530, 3411, 4130, 4851, 4970, 4251, 3412, 3533, 4852, 4133, 
            4252, 4973, 2594, 2619, 2834, 2715, 2738, 2859, 3338, 3315, 
            4034, 4755, 4778, 4059, 3434, 3555, 4874, 4155, 4274, 4995, 
            3578, 3459, 4178, 4899, 5018, 4299, 2620, 2597, 2716, 2837, 
            2860, 2741, 3316, 3341, 4756, 4037, 4060, 4781, 3556, 3437, 
            4156, 4877, 4996, 4277, 3460, 3581, 4900, 4181, 4300, 5021, 
            1730, 1733, 1778, 1755, 1756, 1781, 1852, 1851, 1970, 2091, 
            2092, 1973, 1874, 1899, 2114, 1995, 2018, 2139, 1900, 1877, 
            1996, 2117, 2140, 2021, 2452, 2451, 2474, 2499, 2500, 2477, 
            3170, 3173, 4610, 3891, 3892, 4613, 3218, 3195, 3914, 4635, 
            4658, 3939, 3196, 3221, 4636, 3917, 3940, 4661, 2570, 2573, 
            2810, 2691, 2692, 2813, 3292, 3291, 4010, 4731, 4732, 4013, 
            3410, 3531, 4850, 4131, 4250, 4971, 3532, 3413, 4132, 4853, 
            4972, 4253, 2618, 2595, 2714, 2835, 2858, 2739, 3314, 3339, 
            4754, 4035, 4058, 4779, 3554, 3435, 4154, 4875, 4994, 4275, 
            3458, 3579, 4898, 4179, 4298, 5019, 2596, 2621, 2836, 2717, 
            2740, 2861, 3340, 3317, 4036, 4757, 4780, 4061, 3436, 3557, 
            4876, 4157, 4276, 4997, 3580, 3461, 4180, 4901, 5020, 4301, 
            870, 871, 882, 877, 876, 883, 894, 895, 918, 943, 
            942, 919, 900, 907, 948, 925, 930, 955, 906, 901, 
            924, 949, 954, 931, 990, 991, 996, 1003, 1002, 997, 
            1110, 1111, 1350, 1231, 1230, 1351, 1122, 1117, 1236, 1357, 
            1362, 1243, 1116, 1123, 1356, 1237, 1242, 1363, 1014, 1015, 
            1062, 1039, 1038, 1063, 1134, 1135, 1254, 1375, 1374, 1255, 
            1158, 1183, 1398, 1279, 1302, 1423, 1182, 1159, 1278, 1399, 
            1422, 1303, 1026, 1021, 1044, 1069, 1074, 1051, 1140, 1147, 
            1380, 1261, 1266, 1387, 1188, 1165, 1284, 1405, 1428, 1309, 
            1170, 1195, 1410, 1291, 1314, 1435, 1020, 1027, 1068, 1045, 
            1050, 1075, 1146, 1141, 1260, 1381, 1386, 1267, 1164, 1189, 
            1404, 1285, 1308, 1429, 1194, 1171, 1290, 1411, 1434, 1315, 
            1590, 1591, 1596, 1603, 1602, 1597, 1614, 1615, 1662, 1639, 
            1638, 1663, 1626, 1621, 1644, 1669, 1674, 1651, 1620, 1627, 
            1668, 1645, 1650, 1675, 2310, 2311, 2322, 2317, 2316, 2323, 
            3030, 3031, 3750, 4471, 4470, 3751, 3036, 3043, 4476, 3757, 
            3762, 4483, 3042, 3037, 3756, 4477, 4482, 3763, 2334, 2335, 
            2358, 2383, 2382, 2359, 3054, 3055, 4494, 3775, 3774, 4495, 
            3102, 3079, 3798, 4519, 4542, 3823, 3078, 3103, 4518, 3799, 
            3822, 4543, 2340, 2347, 2388, 2365, 2370, 2395, 3066, 3061, 
            3780, 4501, 4506, 3787, 3084, 3109, 4524, 3805, 3828, 4549, 
            3114, 3091, 3810, 4531, 4554, 3835, 2346, 2341, 2364, 2389, 
            2394, 2371, 3060, 3067, 4500, 3781, 3786, 4507, 3108, 3085, 
            3804, 4525, 4548, 3829, 3090, 3115, 4530, 3811, 3834, 4555, 
            1710, 1711, 1722, 1717, 1716, 1723, 1830, 1831, 1950, 2071, 
            2070, 1951, 1836, 1843, 2076, 1957, 1962, 2083, 1842, 1837, 
            1956, 2077, 2082, 1963, 2430, 2431, 2436, 2443, 2442, 2437, 
            3150, 3151, 4590, 3871, 3870, 4591, 3162, 3157, 3876, 4597, 
            4602, 3883, 3156, 3163, 4596, 3877, 3882, 4603, 2550, 2551, 
            2790, 2671, 2670, 2791, 3270, 3271, 3990, 4711, 4710, 3991, 
            3390, 3511, 4830, 4111, 4230, 4951, 3510, 3391, 4110, 4831, 
            4950, 4231, 2562, 2557, 2676, 2797, 2802, 2683, 3276, 3283, 
            4716, 3997, 4002, 4723, 3516, 3397, 4116, 4837, 4956, 4237, 
            3402, 3523, 4842, 4123, 4242, 4963, 2556, 2563, 2796, 2677, 
            2682, 2803, 3282, 3277, 3996, 4717, 4722, 4003, 3396, 3517, 
            4836, 4117, 4236, 4957, 3522, 3403, 4122, 4843, 4962, 4243, 
            1734, 1735, 1758, 1783, 1782, 1759, 1854, 1855, 2094, 1975, 
            1974, 2095, 1902, 1879, 1998, 2119, 2142, 2023, 1878, 1903, 
            2118, 1999, 2022, 2143, 2454, 2455, 2502, 2479, 2478, 2503, 
            3174, 3175, 3894, 4615, 4614, 3895, 3198, 3223, 4638, 3919, 
            3942, 4663, 3222, 3199, 3918, 4639, 4662, 3943, 2574, 2575, 
            2694, 2815, 2814, 2695, 3294, 3295, 4734, 4015, 4014, 4735, 
            3534, 3415, 4134, 4855, 4974, 4255, 3414, 3535, 4854, 4135, 
            4254, 4975, 2598, 2623, 2838, 2719, 2742, 2863, 3342, 3319, 
            4038, 4759, 4782, 4063, 3438, 3559, 4878, 4159, 4278, 4999, 
            3582, 3463, 4182, 4903, 5022, 4303, 2622, 2599, 2718, 2839, 
            2862, 2743, 3318, 3343, 4758, 4039, 4062, 4783, 3558, 3439, 
            4158, 4879, 4998, 4279, 3462, 3583, 4902, 4183, 4302, 5023, 
            1740, 1747, 1788, 1765, 1770, 1795, 1866, 1861, 1980, 2101, 
            2106, 1987, 1884, 1909, 2124, 2005, 2028, 2149, 1914, 1891, 
            2010, 2131, 2154, 2035, 2466, 2461, 2484, 2509, 2514, 2491, 
            3180, 3187, 4620, 3901, 3906, 4627, 3228, 3205, 3924, 4645, 
            4668, 3949, 3210, 3235, 4650, 3931, 3954, 4675, 2580, 2587, 
            2820, 2701, 2706, 2827, 3306, 3301, 4020, 4741, 4746, 4027, 
            3420, 3541, 4860, 4141, 4260, 4981, 3546, 3427, 4146, 4867, 
            4986, 4267, 2628, 2605, 2724, 2845, 2868, 2749, 3324, 3349, 
            4764, 4045, 4068, 4789, 3564, 3445, 4164, 4885, 5004, 4285, 
            3468, 3589, 4908, 4189, 4308, 5029, 2610, 2635, 2850, 2731, 
            2754, 2875, 3354, 3331, 4050, 4771, 4794, 4075, 3450, 3571, 
            4890, 4171, 4290, 5011, 3594, 3475, 4194, 4915, 5034, 4315, 
            1746, 1741, 1764, 1789, 1794, 1771, 1860, 1867, 2100, 1981, 
            1986, 2107, 1908, 1885, 2004, 2125, 2148, 2029, 1890, 1915, 
            2130, 2011, 2034, 2155, 2460, 2467, 2508, 2485, 2490, 2515, 
            3186, 3181, 3900, 4621, 4626, 3907, 3204, 3229, 4644, 3925, 
            3948, 4669, 3234, 3211, 3930, 4651, 4674, 3955, 2586, 2581, 
            2700, 2821, 2826, 2707, 3300, 3307, 4740, 4021, 4026, 4747, 
            3540, 3421, 4140, 4861, 4980, 4261, 3426, 3547, 4866, 4147, 
            4266, 4987, 2604, 2629, 2844, 2725, 2748, 2869, 3348, 3325, 
            4044, 4765, 4788, 4069, 3444, 3565, 4884, 4165, 4284, 5005, 
            3588, 3469, 4188, 4909, 5028, 4309, 2634, 2611, 2730, 2851, 
            2874, 2755, 3330, 3355, 4770, 4051, 4074, 4795, 3570, 3451, 
            4170, 4891, 5010, 4291, 3474, 3595, 4914, 4195, 4314, 5035, 
            874, 873, 878, 885, 886, 881, 896, 899, 944, 921, 
            922, 947, 908, 903, 926, 951, 956, 933, 904, 911, 
            952, 929, 934, 959, 992, 995, 1004, 999, 1000, 1007, 
            1114, 1113, 1232, 1353, 1354, 1235, 1118, 1125, 1358, 1239, 
            1244, 1365, 1126, 1121, 1240, 1361, 1366, 1247, 1018, 1017, 
            1040, 1065, 1066, 1043, 1136, 1139, 1376, 1257, 1258, 1379, 
            1184, 1161, 1280, 1401, 1424, 1305, 1162, 1187, 1402, 1283, 
            1306, 1427, 1022, 1029, 1070, 1047, 1052, 1077, 1148, 1143, 
            1262, 1383, 1388, 1269, 1166, 1191, 1406, 1287, 1310, 1431, 
            1196, 1173, 1292, 1413, 1436, 1317, 1030, 1025, 1048, 1073, 
            1078, 1055, 1144, 1151, 1384, 1265, 1270, 1391, 1192, 1169, 
            1288, 1409, 1432, 1313, 1174, 1199, 1414, 1295, 1318, 1439, 
            1592, 1595, 1604, 1599, 1600, 1607, 1618, 1617, 1640, 1665, 
            1666, 1643, 1622, 1629, 1670, 1647, 1652, 1677, 1630, 1625, 
            1648, 1673, 1678, 1655, 2314, 2313, 2318, 2325, 2326, 2321, 
            3032, 3035, 4472, 3753, 3754, 4475, 3044, 3039, 3758, 4479, 
            4484, 3765, 3040, 3047, 4480, 3761, 3766, 4487, 2336, 2339, 
            2384, 2361, 2362, 2387, 3058, 3057, 3776, 4497, 4498, 3779, 
            3080, 3105, 4520, 3801, 3824, 4545, 3106, 3083, 3802, 4523, 
            4546, 3827, 2348, 2343, 2366, 2391, 2396, 2373, 3062, 3069, 
            4502, 3783, 3788, 4509, 3110, 3087, 3806, 4527, 4550, 3831, 
            3092, 3117, 4532, 3813, 3836, 4557, 2344, 2351, 2392, 2369, 
            2374, 2399, 3070, 3065, 3784, 4505, 4510, 3791, 3088, 3113, 
            4528, 3809, 3832, 4553, 3118, 3095, 3814, 4535, 4558, 3839, 
            1714, 1713, 1718, 1725, 1726, 1721, 1832, 1835, 2072, 1953, 
            1954, 2075, 1844, 1839, 1958, 2079, 2084, 1965, 1840, 1847, 
            2080, 1961, 1966, 2087, 2432, 2435, 2444, 2439, 2440, 2447, 
            3154, 3153, 3872, 4593, 4594, 3875, 3158, 3165, 4598, 3879, 
            3884, 4605, 3166, 3161, 3880, 4601, 4606, 3887, 2554, 2553, 
            2672, 2793, 2794, 2675, 3272, 3275, 4712, 3993, 3994, 4715, 
            3512, 3393, 4112, 4833, 4952, 4233, 3394, 3515, 4834, 4115, 
            4234, 4955, 2558, 2565, 2798, 2679, 2684, 2805, 3284, 3279, 
            3998, 4719, 4724, 4005, 3398, 3519, 4838, 4119, 4238, 4959, 
            3524, 3405, 4124, 4845, 4964, 4245, 2566, 2561, 2680, 2801, 
            2806, 2687, 3280, 3287, 4720, 4001, 4006, 4727, 3520, 3401, 
            4120, 4841, 4960, 4241, 3406, 3527, 4846, 4127, 4246, 4967, 
            1736, 1739, 1784, 1761, 1762, 1787, 1858, 1857, 1976, 2097, 
            2098, 1979, 1880, 1905, 2120, 2001, 2024, 2145, 1906, 1883, 
            2002, 2123, 2146, 2027, 2458, 2457, 2480, 2505, 2506, 2483, 
            3176, 3179, 4616, 3897, 3898, 4619, 3224, 3201, 3920, 4641, 
            4664, 3945, 3202, 3227, 4642, 3923, 3946, 4667, 2576, 2579, 
            2816, 2697, 2698, 2819, 3298, 3297, 4016, 4737, 4738, 4019, 
            3416, 3537, 4856, 4137, 4256, 4977, 3538, 3419, 4138, 4859, 
            4978, 4259, 2624, 2601, 2720, 2841, 2864, 2745, 3320, 3345, 
            4760, 4041, 4064, 4785, 3560, 3441, 4160, 4881, 5000, 4281, 
            3464, 3585, 4904, 4185, 4304, 5025, 2602, 2627, 2842, 2723, 
            2746, 2867, 3346, 3323, 4042, 4763, 4786, 4067, 3442, 3563, 
            4882, 4163, 4282, 5003, 3586, 3467, 4186, 4907, 5026, 4307, 
            1748, 1743, 1766, 1791, 1796, 1773, 1862, 1869, 2102, 1983, 
            1988, 2109, 1910, 1887, 2006, 2127, 2150, 2031, 1892, 1917, 
            2132, 2013, 2036, 2157, 2462, 2469, 2510, 2487, 2492, 2517, 
            3188, 3183, 3902, 4623, 4628, 3909, 3206, 3231, 4646, 3927, 
            3950, 4671, 3236, 3213, 3932, 4653, 4676, 3957, 2588, 2583, 
            2702, 2823, 2828, 2709, 3302, 3309, 4742, 4023, 4028, 4749, 
            3542, 3423, 4142, 4863, 4982, 4263, 3428, 3549, 4868, 4149, 
            4268, 4989, 2606, 2631, 2846, 2727, 2750, 2871, 3350, 3327, 
            4046, 4767, 4790, 4071, 3446, 3567, 4886, 4167, 4286, 5007, 
            3590, 3471, 4190, 4911, 5030, 4311, 2636, 2613, 2732, 2853, 
            2876, 2757, 3332, 3357, 4772, 4053, 4076, 4797, 3572, 3453, 
            4172, 4893, 5012, 4293, 3476, 3597, 4916, 4197, 4316, 5037, 
            1744, 1751, 1792, 1769, 1774, 1799, 1870, 1865, 1984, 2105, 
            2110, 1991, 1888, 1913, 2128, 2009, 2032, 2153, 1918, 1895, 
            2014, 2135, 2158, 2039, 2470, 2465, 2488, 2513, 2518, 2495, 
            3184, 3191, 4624, 3905, 3910, 4631, 3232, 3209, 3928, 4649, 
            4672, 3953, 3214, 3239, 4654, 3935, 3958, 4679, 2584, 2591, 
            2824, 2705, 2710, 2831, 3310, 3305, 4024, 4745, 4750, 4031, 
            3424, 3545, 4864, 4145, 4264, 4985, 3550, 3431, 4150, 4871, 
            4990, 4271, 2632, 2609, 2728, 2849, 2872, 2753, 3328, 3353, 
            4768, 4049, 4072, 4793, 3568, 3449, 4168, 4889, 5008, 4289, 
            3472, 3593, 4912, 4193, 4312, 5033, 2614, 2639, 2854, 2735, 
            2758, 2879, 3358, 3335, 4054, 4775, 4798, 4079, 3454, 3575, 
            4894, 4175, 4294, 5015, 3598, 3479, 4198, 4919, 5038, 4319, 
            872, 875, 884, 879, 880, 887, 898, 897, 920, 945, 
            946, 923, 902, 909, 950, 927, 932, 957, 910, 905, 
            928, 953, 958, 935, 994, 993, 998, 1005, 1006, 1001, 
            1112, 1115, 1352, 1233, 1234, 1355, 1124, 1119, 1238, 1359, 
            1364, 1245, 1120, 1127, 1360, 1241, 1246, 1367, 1016, 1019, 
            1064, 1041, 1042, 1067, 1138, 1137, 1256, 1377, 1378, 1259, 
            1160, 1185, 1400, 1281, 1304, 1425, 1186, 1163, 1282, 1403, 
            1426, 1307, 1028, 1023, 1046, 1071, 1076, 1053, 1142, 1149, 
            1382, 1263, 1268, 1389, 1190, 1167, 1286, 1407, 1430, 1311, 
            1172, 1197, 1412, 1293, 1316, 1437, 1024, 1031, 1072, 1049, 
            1054, 1079, 1150, 1145, 1264, 1385, 1390, 1271, 1168, 1193, 
            1408, 1289, 1312, 1433, 1198, 1175, 1294, 1415, 1438, 1319, 
            1594, 1593, 1598, 1605, 1606, 1601, 1616, 1619, 1664, 1641, 
            1642, 1667, 1628, 1623, 1646, 1671, 1676, 1653, 1624, 1631, 
            1672, 1649, 1654, 1679, 2312, 2315, 2324, 2319, 2320, 2327, 
            3034, 3033, 3752, 4473, 4474, 3755, 3038, 3045, 4478, 3759, 
            3764, 4485, 3046, 3041, 3760, 4481, 4486, 3767, 2338, 2337, 
            2360, 2385, 2386, 2363, 3056, 3059, 4496, 3777, 3778, 4499, 
            3104, 3081, 3800, 4521, 4544, 3825, 3082, 3107, 4522, 3803, 
            3826, 4547, 2342, 2349, 2390, 2367, 2372, 2397, 3068, 3063, 
            3782, 4503, 4508, 3789, 3086, 3111, 4526, 3807, 3830, 4551, 
            3116, 3093, 3812, 4533, 4556, 3837, 2350, 2345, 2368, 2393, 
            2398, 2375, 3064, 3071, 4504, 3785, 3790, 4511, 3112, 3089, 
            3808, 4529, 4552, 3833, 3094, 3119, 4534, 3815, 3838, 4559, 
            1712, 1715, 1724, 1719, 1720, 1727, 1834, 1833, 1952, 2073, 
            2074, 1955, 1838, 1845, 2078, 1959, 1964, 2085, 1846, 1841, 
            1960, 2081, 2086, 1967, 2434, 2433, 2438, 2445, 2446, 2441, 
            3152, 3155, 4592, 3873, 3874, 4595, 3164, 3159, 3878, 4599, 
            4604, 3885, 3160, 3167, 4600, 3881, 3886, 4607, 2552, 2555, 
            2792, 2673, 2674, 2795, 3274, 3273, 3992, 4713, 4714, 3995, 
            3392, 3513, 4832, 4113, 4232, 4953, 3514, 3395, 4114, 4835, 
            4954, 4235, 2564, 2559, 2678, 2799, 2804, 2685, 3278, 3285, 
            4718, 3999, 4004, 4725, 3518, 3399, 4118, 4839, 4958, 4239, 
            3404, 3525, 4844, 4125, 4244, 4965, 2560, 2567, 2800, 2681, 
            2686, 2807, 3286, 3281, 4000, 4721, 4726, 4007, 3400, 3521, 
            4840, 4121, 4240, 4961, 3526, 3407, 4126, 4847, 4966, 4247, 
            1738, 1737, 1760, 1785, 1786, 1763, 1856, 1859, 2096, 1977, 
            1978, 2099, 1904, 1881, 2000, 2121, 2144, 2025, 1882, 1907, 
            2122, 2003, 2026, 2147, 2456, 2459, 2504, 2481, 2482, 2507, 
            3178, 3177, 3896, 4617, 4618, 3899, 3200, 3225, 4640, 3921, 
            3944, 4665, 3226, 3203, 3922, 4643, 4666, 3947, 2578, 2577, 
            2696, 2817, 2818, 2699, 3296, 3299, 4736, 4017, 4018, 4739, 
            3536, 3417, 4136, 4857, 4976, 4257, 3418, 3539, 4858, 4139, 
            4258, 4979, 2600, 2625, 2840, 2721, 2744, 2865, 3344, 3321, 
            4040, 4761, 4784, 4065, 3440, 3561, 4880, 4161, 4280, 5001, 
            3584, 3465, 4184, 4905, 5024, 4305, 2626, 2603, 2722, 2843, 
            2866, 2747, 3322, 3347, 4762, 4043, 4066, 4787, 3562, 3443, 
            4162, 4883, 5002, 4283, 3466, 3587, 4906, 4187, 4306, 5027, 
            1742, 1749, 1790, 1767, 1772, 1797, 1868, 1863, 1982, 2103, 
            2108, 1989, 1886, 1911, 2126, 2007, 2030, 2151, 1916, 1893, 
            2012, 2133, 2156, 2037, 2468, 2463, 2486, 2511, 2516, 2493, 
            3182, 3189, 4622, 3903, 3908, 4629, 3230, 3207, 3926, 4647, 
            4670, 3951, 3212, 3237, 4652, 3933, 3956, 4677, 2582, 2589, 
            2822, 2703, 2708, 2829, 3308, 3303, 4022, 4743, 4748, 4029, 
            3422, 3543, 4862, 4143, 4262, 4983, 3548, 3429, 4148, 4869, 
            4988, 4269, 2630, 2607, 2726, 2847, 2870, 2751, 3326, 3351, 
            4766, 4047, 4070, 4791, 3566, 3447, 4166, 4887, 5006, 4287, 
            3470, 3591, 4910, 4191, 4310, 5031, 2612, 2637, 2852, 2733, 
            2756, 2877, 3356, 3333, 4052, 4773, 4796, 4077, 3452, 3573, 
            4892, 4173, 4292, 5013, 3596, 3477, 4196, 4917, 5036, 4317, 
            1750, 1745, 1768, 1793, 1798, 1775, 1864, 1871, 2104, 1985, 
            1990, 2111, 1912, 1889, 2008, 2129, 2152, 2033, 1894, 1919, 
            2134, 2015, 2038, 2159, 2464, 2471, 2512, 2489, 2494, 2519, 
            3190, 3185, 3904, 4625, 4630, 3911, 3208, 3233, 4648, 3929, 
            3952, 4673, 3238, 3215, 3934, 4655, 4678, 3959, 2590, 2585, 
            2704, 2825, 2830, 2711, 3304, 3311, 4744, 4025, 4030, 4751, 
            3544, 3425, 4144, 4865, 4984, 4265, 3430, 3551, 4870, 4151, 
            4270, 4991, 2608, 2633, 2848, 2729, 2752, 2873, 3352, 3329, 
            4048, 4769, 4792, 4073, 3448, 3569, 4888, 4169, 4288, 5009, 
            3592, 3473, 4192, 4913, 5032, 4313, 2638, 2615, 2734, 2855, 
            2878, 2759, 3334, 3359, 4774, 4055, 4078, 4799, 3574, 3455, 
            4174, 4895, 5014, 4295, 3478, 3599, 4918, 4199, 4318, 5039, 
            5040, 5041, 5042, 5045, 5044, 5043, 5046, 5047, 5058, 5053, 
            5052, 5059, 5050, 5049, 5054, 5061, 5062, 5057, 5048, 5051, 
            5060, 5055, 5056, 5063, 5064, 5065, 5068, 5067, 5066, 5069, 
            5088, 5089, 5112, 5137, 5136, 5113, 5090, 5093, 5138, 5115, 
            5116, 5141, 5092, 5091, 5114, 5139, 5140, 5117, 5070, 5071, 
            5076, 5083, 5082, 5077, 5094, 5095, 5142, 5119, 5118, 5143, 
            5106, 5101, 5124, 5149, 5154, 5131, 5100, 5107, 5148, 5125, 
            5130, 5155, 5072, 5075, 5084, 5079, 5080, 5087, 5098, 5097, 
            5120, 5145, 5146, 5123, 5102, 5109, 5150, 5127, 5132, 5157, 
            5110, 5105, 5128, 5153, 5158, 5135, 5074, 5073, 5078, 5085, 
            5086, 5081, 5096, 5099, 5144, 5121, 5122, 5147, 5108, 5103, 
            5126, 5151, 5156, 5133, 5104, 5111, 5152, 5129, 5134, 5159, 
            5160, 5161, 5164, 5163, 5162, 5165, 5166, 5167, 5172, 5179, 
            5178, 5173, 5168, 5171, 5180, 5175, 5176, 5183, 5170, 5169, 
            5174, 5181, 5182, 5177, 5280, 5281, 5282, 5285, 5284, 5283, 
            5400, 5401, 5640, 5521, 5520, 5641, 5404, 5403, 5522, 5643, 
            5644, 5525, 5402, 5405, 5642, 5523, 5524, 5645, 5286, 5287, 
            5298, 5293, 5292, 5299, 5406, 5407, 5526, 5647, 5646, 5527, 
            5412, 5419, 5652, 5533, 5538, 5659, 5418, 5413, 5532, 5653, 
            5658, 5539, 5290, 5289, 5294, 5301, 5302, 5297, 5408, 5411, 
            5648, 5529, 5530, 5651, 5420, 5415, 5534, 5655, 5660, 5541, 
            5416, 5423, 5656, 5537, 5542, 5663, 5288, 5291, 5300, 5295, 
            5296, 5303, 5410, 5409, 5528, 5649, 5650, 5531, 5414, 5421, 
            5654, 5535, 5540, 5661, 5422, 5417, 5536, 5657, 5662, 5543, 
            5184, 5185, 5186, 5189, 5188, 5187, 5208, 5209, 5256, 5233, 
            5232, 5257, 5212, 5211, 5234, 5259, 5260, 5237, 5210, 5213, 
            5258, 5235, 5236, 5261, 5304, 5305, 5308, 5307, 5306, 5309, 
            5424, 5425, 5544, 5665, 5664, 5545, 5426, 5429, 5666, 5547, 
            5548, 5669, 5428, 5427, 5546, 5667, 5668, 5549, 5328, 5329, 
            5352, 5377, 5376, 5353, 5448, 5449, 5688, 5569, 5568, 5689, 
            5496, 5473, 5592, 5713, 5736, 5617, 5472, 5497, 5712, 5593, 
            5616, 5737, 5330, 5333, 5378, 5355, 5356, 5381, 5452, 5451, 
            5570, 5691, 5692, 5573, 5474, 5499, 5714, 5595, 5618, 5739, 
            5500, 5477, 5596, 5717, 5740, 5621, 5332, 5331, 5354, 5379, 
            5380, 5357, 5450, 5453, 5690, 5571, 5572, 5693, 5498, 5475, 
            5594, 5715, 5738, 5619, 5476, 5501, 5716, 5597, 5620, 5741, 
            5190, 5191, 5202, 5197, 5196, 5203, 5214, 5215, 5238, 5263, 
            5262, 5239, 5220, 5227, 5268, 5245, 5250, 5275, 5226, 5221, 
            5244, 5269, 5274, 5251, 5310, 5311, 5316, 5323, 5322, 5317, 
            5430, 5431, 5670, 5551, 5550, 5671, 5442, 5437, 5556, 5677, 
            5682, 5563, 5436, 5443, 5676, 5557, 5562, 5683, 5334, 5335, 
            5382, 5359, 5358, 5383, 5454, 5455, 5574, 5695, 5694, 5575, 
            5478, 5503, 5718, 5599, 5622, 5743, 5502, 5479, 5598, 5719, 
            5742, 5623, 5346, 5341, 5364, 5389, 5394, 5371, 5460, 5467, 
            5700, 5581, 5586, 5707, 5508, 5485, 5604, 5725, 5748, 5629, 
            5490, 5515, 5730, 5611, 5634, 5755, 5340, 5347, 5388, 5365, 
            5370, 5395, 5466, 5461, 5580, 5701, 5706, 5587, 5484, 5509, 
            5724, 5605, 5628, 5749, 5514, 5491, 5610, 5731, 5754, 5635, 
            5194, 5193, 5198, 5205, 5206, 5201, 5216, 5219, 5264, 5241, 
            5242, 5267, 5228, 5223, 5246, 5271, 5276, 5253, 5224, 5231, 
            5272, 5249, 5254, 5279, 5312, 5315, 5324, 5319, 5320, 5327, 
            5434, 5433, 5552, 5673, 5674, 5555, 5438, 5445, 5678, 5559, 
            5564, 5685, 5446, 5441, 5560, 5681, 5686, 5567, 5338, 5337, 
            5360, 5385, 5386, 5363, 5456, 5459, 5696, 5577, 5578, 5699, 
            5504, 5481, 5600, 5721, 5744, 5625, 5482, 5507, 5722, 5603, 
            5626, 5747, 5342, 5349, 5390, 5367, 5372, 5397, 5468, 5463, 
            5582, 5703, 5708, 5589, 5486, 5511, 5726, 5607, 5630, 5751, 
            5516, 5493, 5612, 5733, 5756, 5637, 5350, 5345, 5368, 5393, 
            5398, 5375, 5464, 5471, 5704, 5585, 5590, 5711, 5512, 5489, 
            5608, 5729, 5752, 5633, 5494, 5519, 5734, 5615, 5638, 5759, 
            5192, 5195, 5204, 5199, 5200, 5207, 5218, 5217, 5240, 5265, 
            5266, 5243, 5222, 5229, 5270, 5247, 5252, 5277, 5230, 5225, 
            5248, 5273, 5278, 5255, 5314, 5313, 5318, 5325, 5326, 5321, 
            5432, 5435, 5672, 5553, 5554, 5675, 5444, 5439, 5558, 5679, 
            5684, 5565, 5440, 5447, 5680, 5561, 5566, 5687, 5336, 5339, 
            5384, 5361, 5362, 5387, 5458, 5457, 5576, 5697, 5698, 5579, 
            5480, 5505, 5720, 5601, 5624, 5745, 5506, 5483, 5602, 5723, 
            5746, 5627, 5348, 5343, 5366, 5391, 5396, 5373, 5462, 5469, 
            5702, 5583, 5588, 5709, 5510, 5487, 5606, 5727, 5750, 5631, 
            5492, 5517, 5732, 5613, 5636, 5757, 5344, 5351, 5392, 5369, 
            5374, 5399, 5470, 5465, 5584, 5705, 5710, 5591, 5488, 5513, 
            5728, 5609, 5632, 5753, 5518, 5495, 5614, 5735, 5758, 5639, 
            10080, 10081, 10084, 10083, 10082, 10085, 10086, 10087, 10092, 10099, 
            10098, 10093, 10088, 10091, 10100, 10095, 10096, 10103, 10090, 10089, 
            10094, 10101, 10102, 10097, 10104, 10105, 10106, 10109, 10108, 10107, 
            10128, 10129, 10176, 10153, 10152, 10177, 10132, 10131, 10154, 10179, 
            10180, 10157, 10130, 10133, 10178, 10155, 10156, 10181, 10110, 10111, 
            10122, 10117, 10116, 10123, 10134, 10135, 10158, 10183, 10182, 10159, 
            10140, 10147, 10188, 10165, 10170, 10195, 10146, 10141, 10164, 10189, 
            10194, 10171, 10114, 10113, 10118, 10125, 10126, 10121, 10136, 10139, 
            10184, 10161, 10162, 10187, 10148, 10143, 10166, 10191, 10196, 10173, 
            10144, 10151, 10192, 10169, 10174, 10199, 10112, 10115, 10124, 10119, 
            10120, 10127, 10138, 10137, 10160, 10185, 10186, 10163, 10142, 10149, 
            10190, 10167, 10172, 10197, 10150, 10145, 10168, 10193, 10198, 10175, 
            15120, 15121, 15122, 15125, 15124, 15123, 15126, 15127, 15138, 15133, 
            15132, 15139, 15130, 15129, 15134, 15141, 15142, 15137, 15128, 15131, 
            15140, 15135, 15136, 15143, 20160, 20161, 20164, 20163, 20162, 20165, 
            25200, 25201, 30240, 35281, 35280, 30241, 25202, 25205, 35282, 30243, 
            30244, 35285, 25204, 25203, 30242, 35283, 35284, 30245, 20166, 20167, 
            20172, 20179, 20178, 20173, 25206, 25207, 35286, 30247, 30246, 35287, 
            25218, 25213, 30252, 35293, 35298, 30259, 25212, 25219, 35292, 30253, 
            30258, 35299, 20168, 20171, 20180, 20175, 20176, 20183, 25210, 25209, 
            30248, 35289, 35290, 30251, 25214, 25221, 35294, 30255, 30260, 35301, 
            25222, 25217, 30256, 35297, 35302, 30263, 20170, 20169, 20174, 20181, 
            20182, 20177, 25208, 25211, 35288, 30249, 30250, 35291, 25220, 25215, 
            30254, 35295, 35300, 30261, 25216, 25223, 35296, 30257, 30262, 35303, 
            15144, 15145, 15148, 15147, 15146, 15149, 15168, 15169, 15192, 15217, 
            15216, 15193, 15170, 15173, 15218, 15195, 15196, 15221, 15172, 15171, 
            15194, 15219, 15220, 15197, 20184, 20185, 20186, 20189, 20188, 20187, 
            25224, 25225, 35304, 30265, 30264, 35305, 25228, 25227, 30266, 35307, 
            35308, 30269, 25226, 25229, 35306, 30267, 30268, 35309, 20208, 20209, 
            20256, 20233, 20232, 20257, 25248, 25249, 30288, 35329, 35328, 30289, 
            25272, 25297, 35352, 30313, 30336, 35377, 25296, 25273, 30312, 35353, 
            35376, 30337, 20212, 20211, 20234, 20259, 20260, 20237, 25250, 25253, 
            35330, 30291, 30292, 35333, 25298, 25275, 30314, 35355, 35378, 30339, 
            25276, 25301, 35356, 30317, 30340, 35381, 20210, 20213, 20258, 20235, 
            20236, 20261, 25252, 25251, 30290, 35331, 35332, 30293, 25274, 25299, 
            35354, 30315, 30338, 35379, 25300, 25277, 30316, 35357, 35380, 30341, 
            15150, 15151, 15156, 15163, 15162, 15157, 15174, 15175, 15222, 15199, 
            15198, 15223, 15186, 15181, 15204, 15229, 15234, 15211, 15180, 15187, 
            15228, 15205, 15210, 15235, 20190, 20191, 20202, 20197, 20196, 20203, 
            25230, 25231, 30270, 35311, 35310, 30271, 25236, 25243, 35316, 30277, 
            30282, 35323, 25242, 25237, 30276, 35317, 35322, 30283, 20214, 20215, 
            20238, 20263, 20262, 20239, 25254, 25255, 35334, 30295, 30294, 35335, 
            25302, 25279, 30318, 35359, 35382, 30343, 25278, 25303, 35358, 30319, 
            30342, 35383, 20220, 20227, 20268, 20245, 20250, 20275, 25266, 25261, 
            30300, 35341, 35346, 30307, 25284, 25309, 35364, 30325, 30348, 35389, 
            25314, 25291, 30330, 35371, 35394, 30355, 20226, 20221, 20244, 20269, 
            20274, 20251, 25260, 25267, 35340, 30301, 30306, 35347, 25308, 25285, 
            30324, 35365, 35388, 30349, 25290, 25315, 35370, 30331, 30354, 35395, 
            15152, 15155, 15164, 15159, 15160, 15167, 15178, 15177, 15200, 15225, 
            15226, 15203, 15182, 15189, 15230, 15207, 15212, 15237, 15190, 15185, 
            15208, 15233, 15238, 15215, 20194, 20193, 20198, 20205, 20206, 20201, 
            25232, 25235, 35312, 30273, 30274, 35315, 25244, 25239, 30278, 35319, 
            35324, 30285, 25240, 25247, 35320, 30281, 30286, 35327, 20216, 20219, 
            20264, 20241, 20242, 20267, 25258, 25257, 30296, 35337, 35338, 30299, 
            25280, 25305, 35360, 30321, 30344, 35385, 25306, 25283, 30322, 35363, 
            35386, 30347, 20228, 20223, 20246, 20271, 20276, 20253, 25262, 25269, 
            35342, 30303, 30308, 35349, 25310, 25287, 30326, 35367, 35390, 30351, 
            25292, 25317, 35372, 30333, 30356, 35397, 20224, 20231, 20272, 20249, 
            20254, 20279, 25270, 25265, 30304, 35345, 35350, 30311, 25288, 25313, 
            35368, 30329, 30352, 35393, 25318, 25295, 30334, 35375, 35398, 30359, 
            15154, 15153, 15158, 15165, 15166, 15161, 15176, 15179, 15224, 15201, 
            15202, 15227, 15188, 15183, 15206, 15231, 15236, 15213, 15184, 15191, 
            15232, 15209, 15214, 15239, 20192, 20195, 20204, 20199, 20200, 20207, 
            25234, 25233, 30272, 35313, 35314, 30275, 25238, 25245, 35318, 30279, 
            30284, 35325, 25246, 25241, 30280, 35321, 35326, 30287, 20218, 20217, 
            20240, 20265, 20266, 20243, 25256, 25259, 35336, 30297, 30298, 35339, 
            25304, 25281, 30320, 35361, 35384, 30345, 25282, 25307, 35362, 30323, 
            30346, 35387, 20222, 20229, 20270, 20247, 20252, 20277, 25268, 25263, 
            30302, 35343, 35348, 30309, 25286, 25311, 35366, 30327, 30350, 35391, 
            25316, 25293, 30332, 35373, 35396, 30357, 20230, 20225, 20248, 20273, 
            20278, 20255, 25264, 25271, 35344, 30305, 30310, 35351, 25312, 25289, 
            30328, 35369, 35392, 30353, 25294, 25319, 35374, 30335, 30358, 35399, 
            10200, 10201, 10202, 10205, 10204, 10203, 10206, 10207, 10218, 10213, 
            10212, 10219, 10210, 10209, 10214, 10221, 10222, 10217, 10208, 10211, 
            10220, 10215, 10216, 10223, 10320, 10321, 10324, 10323, 10322, 10325, 
            10440, 10441, 10560, 10681, 10680, 10561, 10442, 10445, 10682, 10563, 
            10564, 10685, 10444, 10443, 10562, 10683, 10684, 10565, 10326, 10327, 
            10332, 10339, 10338, 10333, 10446, 10447, 10686, 10567, 10566, 10687, 
            10458, 10453, 10572, 10693, 10698, 10579, 10452, 10459, 10692, 10573, 
            10578, 10699, 10328, 10331, 10340, 10335, 10336, 10343, 10450, 10449, 
            10568, 10689, 10690, 10571, 10454, 10461, 10694, 10575, 10580, 10701, 
            10462, 10457, 10576, 10697, 10702, 10583, 10330, 10329, 10334, 10341, 
            10342, 10337, 10448, 10451, 10688, 10569, 10570, 10691, 10460, 10455, 
            10574, 10695, 10700, 10581, 10456, 10463, 10696, 10577, 10582, 10703, 
            15240, 15241, 15244, 15243, 15242, 15245, 15246, 15247, 15252, 15259, 
            15258, 15253, 15248, 15251, 15260, 15255, 15256, 15263, 15250, 15249, 
            15254, 15261, 15262, 15257, 20280, 20281, 20282, 20285, 20284, 20283, 
            25320, 25321, 35400, 30361, 30360, 35401, 25324, 25323, 30362, 35403, 
            35404, 30365, 25322, 25325, 35402, 30363, 30364, 35405, 20286, 20287, 
            20298, 20293, 20292, 20299, 25326, 25327, 30366, 35407, 35406, 30367, 
            25332, 25339, 35412, 30373, 30378, 35419, 25338, 25333, 30372, 35413, 
            35418, 30379, 20290, 20289, 20294, 20301, 20302, 20297, 25328, 25331, 
            35408, 30369, 30370, 35411, 25340, 25335, 30374, 35415, 35420, 30381, 
            25336, 25343, 35416, 30377, 30382, 35423, 20288, 20291, 20300, 20295, 
            20296, 20303, 25330, 25329, 30368, 35409, 35410, 30371, 25334, 25341, 
            35414, 30375, 30380, 35421, 25342, 25337, 30376, 35417, 35422, 30383, 
            15360, 15361, 15362, 15365, 15364, 15363, 15480, 15481, 15720, 15601, 
            15600, 15721, 15484, 15483, 15602, 15723, 15724, 15605, 15482, 15485, 
            15722, 15603, 15604, 15725, 20400, 20401, 20404, 20403, 20402, 20405, 
            25440, 25441, 30480, 35521, 35520, 30481, 25442, 25445, 35522, 30483, 
            30484, 35525, 25444, 25443, 30482, 35523, 35524, 30485, 20520, 20521, 
            20640, 20761, 20760, 20641, 25560, 25561, 35640, 30601, 30600, 35641, 
            25800, 25681, 30720, 35761, 35880, 30841, 25680, 25801, 35760, 30721, 
            30840, 35881, 20522, 20525, 20762, 20643, 20644, 20765, 25564, 25563, 
            30602, 35643, 35644, 30605, 25682, 25803, 35762, 30723, 30842, 35883, 
            25804, 25685, 30724, 35765, 35884, 30845, 20524, 20523, 20642, 20763, 
            20764, 20645, 25562, 25565, 35642, 30603, 30604, 35645, 25802, 25683, 
            30722, 35763, 35882, 30843, 25684, 25805, 35764, 30725, 30844, 35885, 
            15366, 15367, 15378, 15373, 15372, 15379, 15486, 15487, 15606, 15727, 
            15726, 15607, 15492, 15499, 15732, 15613, 15618, 15739, 15498, 15493, 
            15612, 15733, 15738, 15619, 20406, 20407, 20412, 20419, 20418, 20413, 
            25446, 25447, 35526, 30487, 30486, 35527, 25458, 25453, 30492, 35533, 
            35538, 30499, 25452, 25459, 35532, 30493, 30498, 35539, 20526, 20527, 
            20766, 20647, 20646, 20767, 25566, 25567, 30606, 35647, 35646, 30607, 
            25686, 25807, 35766, 30727, 30846, 35887, 25806, 25687, 30726, 35767, 
            35886, 30847, 20538, 20533, 20652, 20773, 20778, 20659, 25572, 25579, 
            35652, 30613, 30618, 35659, 25812, 25693, 30732, 35773, 35892, 30853, 
            25698, 25819, 35778, 30739, 30858, 35899, 20532, 20539, 20772, 20653, 
            20658, 20779, 25578, 25573, 30612, 35653, 35658, 30619, 25692, 25813, 
            35772, 30733, 30852, 35893, 25818, 25699, 30738, 35779, 35898, 30859, 
            15370, 15369, 15374, 15381, 15382, 15377, 15488, 15491, 15728, 15609, 
            15610, 15731, 15500, 15495, 15614, 15735, 15740, 15621, 15496, 15503, 
            15736, 15617, 15622, 15743, 20408, 20411, 20420, 20415, 20416, 20423, 
            25450, 25449, 30488, 35529, 35530, 30491, 25454, 25461, 35534, 30495, 
            30500, 35541, 25462, 25457, 30496, 35537, 35542, 30503, 20530, 20529, 
            20648, 20769, 20770, 20651, 25568, 25571, 35648, 30609, 30610, 35651, 
            25808, 25689, 30728, 35769, 35888, 30849, 25690, 25811, 35770, 30731, 
            30850, 35891, 20534, 20541, 20774, 20655, 20660, 20781, 25580, 25575, 
            30614, 35655, 35660, 30621, 25694, 25815, 35774, 30735, 30854, 35895, 
            25820, 25701, 30740, 35781, 35900, 30861, 20542, 20537, 20656, 20777, 
            20782, 20663, 25576, 25583, 35656, 30617, 30622, 35663, 25816, 25697, 
            30736, 35777, 35896, 30857, 25702, 25823, 35782, 30743, 30862, 35903, 
            15368, 15371, 15380, 15375, 15376, 15383, 15490, 15489, 15608, 15729, 
            15730, 15611, 15494, 15501, 15734, 15615, 15620, 15741, 15502, 15497, 
            15616, 15737, 15742, 15623, 20410, 20409, 20414, 20421, 20422, 20417, 
            25448, 25451, 35528, 30489, 30490, 35531, 25460, 25455, 30494, 35535, 
            35540, 30501, 25456, 25463, 35536, 30497, 30502, 35543, 20528, 20531, 
            20768, 20649, 20650, 20771, 25570, 25569, 30608, 35649, 35650, 30611, 
            25688, 25809, 35768, 30729, 30848, 35889, 25810, 25691, 30730, 35771, 
            35890, 30851, 20540, 20535, 20654, 20775, 20780, 20661, 25574, 25581, 
            35654, 30615, 30620, 35661, 25814, 25695, 30734, 35775, 35894, 30855, 
            25700, 25821, 35780, 30741, 30860, 35901, 20536, 20543, 20776, 20657, 
            20662, 20783, 25582, 25577, 30616, 35657, 35662, 30623, 25696, 25817, 
            35776, 30737, 30856, 35897, 25822, 25703, 30742, 35783, 35902, 30863, 
            10224, 10225, 10228, 10227, 10226, 10229, 10248, 10249, 10272, 10297, 
            10296, 10273, 10250, 10253, 10298, 10275, 10276, 10301, 10252, 10251, 
            10274, 10299, 10300, 10277, 10344, 10345, 10346, 10349, 10348, 10347, 
            10464, 10465, 10704, 10585, 10584, 10705, 10468, 10467, 10586, 10707, 
            10708, 10589, 10466, 10469, 10706, 10587, 10588, 10709, 10368, 10369, 
            10416, 10393, 10392, 10417, 10488, 10489, 10608, 10729, 10728, 10609, 
            10512, 10537, 10752, 10633, 10656, 10777, 10536, 10513, 10632, 10753, 
            10776, 10657, 10372, 10371, 10394, 10419, 10420, 10397, 10490, 10493, 
            10730, 10611, 10612, 10733, 10538, 10515, 10634, 10755, 10778, 10659, 
            10516, 10541, 10756, 10637, 10660, 10781, 10370, 10373, 10418, 10395, 
            10396, 10421, 10492, 10491, 10610, 10731, 10732, 10613, 10514, 10539, 
            10754, 10635, 10658, 10779, 10540, 10517, 10636, 10757, 10780, 10661, 
            15264, 15265, 15266, 15269, 15268, 15267, 15288, 15289, 15336, 15313, 
            15312, 15337, 15292, 15291, 15314, 15339, 15340, 15317, 15290, 15293, 
            15338, 15315, 15316, 15341, 20304, 20305, 20308, 20307, 20306, 20309, 
            25344, 25345, 30384, 35425, 35424, 30385, 25346, 25349, 35426, 30387, 
            30388, 35429, 25348, 25347, 30386, 35427, 35428, 30389, 20328, 20329, 
            20352, 20377, 20376, 20353, 25368, 25369, 35448, 30409, 30408, 35449, 
            25416, 25393, 30432, 35473, 35496, 30457, 25392, 25417, 35472, 30433, 
            30456, 35497, 20330, 20333, 20378, 20355, 20356, 20381, 25372, 25371, 
            30410, 35451, 35452, 30413, 25394, 25419, 35474, 30435, 30458, 35499, 
            25420, 25397, 30436, 35477, 35500, 30461, 20332, 20331, 20354, 20379, 
            20380, 20357, 25370, 25373, 35450, 30411, 30412, 35453, 25418, 25395, 
            30434, 35475, 35498, 30459, 25396, 25421, 35476, 30437, 30460, 35501, 
            15384, 15385, 15388, 15387, 15386, 15389, 15504, 15505, 15624, 15745, 
            15744, 15625, 15506, 15509, 15746, 15627, 15628, 15749, 15508, 15507, 
            15626, 15747, 15748, 15629, 20424, 20425, 20426, 20429, 20428, 20427, 
            25464, 25465, 35544, 30505, 30504, 35545, 25468, 25467, 30506, 35547, 
            35548, 30509, 25466, 25469, 35546, 30507, 30508, 35549, 20544, 20545, 
            20784, 20665, 20664, 20785, 25584, 25585, 30624, 35665, 35664, 30625, 
            25704, 25825, 35784, 30745, 30864, 35905, 25824, 25705, 30744, 35785, 
            35904, 30865, 20548, 20547, 20666, 20787, 20788, 20669, 25586, 25589, 
            35666, 30627, 30628, 35669, 25826, 25707, 30746, 35787, 35906, 30867, 
            25708, 25829, 35788, 30749, 30868, 35909, 20546, 20549, 20786, 20667, 
            20668, 20789, 25588, 25587, 30626, 35667, 35668, 30629, 25706, 25827, 
            35786, 30747, 30866, 35907, 25828, 25709, 30748, 35789, 35908, 30869, 
            15408, 15409, 15432, 15457, 15456, 15433, 15528, 15529, 15768, 15649, 
            15648, 15769, 15576, 15553, 15672, 15793, 15816, 15697, 15552, 15577, 
            15792, 15673, 15696, 15817, 20448, 20449, 20496, 20473, 20472, 20497, 
            25488, 25489, 30528, 35569, 35568, 30529, 25512, 25537, 35592, 30553, 
            30576, 35617, 25536, 25513, 30552, 35593, 35616, 30577, 20568, 20569, 
            20688, 20809, 20808, 20689, 25608, 25609, 35688, 30649, 30648, 35689, 
            25848, 25729, 30768, 35809, 35928, 30889, 25728, 25849, 35808, 30769, 
            30888, 35929, 20592, 20617, 20832, 20713, 20736, 20857, 25656, 25633, 
            30672, 35713, 35736, 30697, 25752, 25873, 35832, 30793, 30912, 35953, 
            25896, 25777, 30816, 35857, 35976, 30937, 20616, 20593, 20712, 20833, 
            20856, 20737, 25632, 25657, 35712, 30673, 30696, 35737, 25872, 25753, 
            30792, 35833, 35952, 30913, 25776, 25897, 35856, 30817, 30936, 35977, 
            15410, 15413, 15458, 15435, 15436, 15461, 15532, 15531, 15650, 15771, 
            15772, 15653, 15554, 15579, 15794, 15675, 15698, 15819, 15580, 15557, 
            15676, 15797, 15820, 15701, 20452, 20451, 20474, 20499, 20500, 20477, 
            25490, 25493, 35570, 30531, 30532, 35573, 25538, 25515, 30554, 35595, 
            35618, 30579, 25516, 25541, 35596, 30557, 30580, 35621, 20570, 20573, 
            20810, 20691, 20692, 20813, 25612, 25611, 30650, 35691, 35692, 30653, 
            25730, 25851, 35810, 30771, 30890, 35931, 25852, 25733, 30772, 35813, 
            35932, 30893, 20618, 20595, 20714, 20835, 20858, 20739, 25634, 25659, 
            35714, 30675, 30698, 35739, 25874, 25755, 30794, 35835, 35954, 30915, 
            25778, 25899, 35858, 30819, 30938, 35979, 20596, 20621, 20836, 20717, 
            20740, 20861, 25660, 25637, 30676, 35717, 35740, 30701, 25756, 25877, 
            35836, 30797, 30916, 35957, 25900, 25781, 30820, 35861, 35980, 30941, 
            15412, 15411, 15434, 15459, 15460, 15437, 15530, 15533, 15770, 15651, 
            15652, 15773, 15578, 15555, 15674, 15795, 15818, 15699, 15556, 15581, 
            15796, 15677, 15700, 15821, 20450, 20453, 20498, 20475, 20476, 20501, 
            25492, 25491, 30530, 35571, 35572, 30533, 25514, 25539, 35594, 30555, 
            30578, 35619, 25540, 25517, 30556, 35597, 35620, 30581, 20572, 20571, 
            20690, 20811, 20812, 20693, 25610, 25613, 35690, 30651, 30652, 35693, 
            25850, 25731, 30770, 35811, 35930, 30891, 25732, 25853, 35812, 30773, 
            30892, 35933, 20594, 20619, 20834, 20715, 20738, 20859, 25658, 25635, 
            30674, 35715, 35738, 30699, 25754, 25875, 35834, 30795, 30914, 35955, 
            25898, 25779, 30818, 35859, 35978, 30939, 20620, 20597, 20716, 20837, 
            20860, 20741, 25636, 25661, 35716, 30677, 30700, 35741, 25876, 25757, 
            30796, 35837, 35956, 30917, 25780, 25901, 35860, 30821, 30940, 35981, 
            10230, 10231, 10236, 10243, 10242, 10237, 10254, 10255, 10302, 10279, 
            10278, 10303, 10266, 10261, 10284, 10309, 10314, 10291, 10260, 10267, 
            10308, 10285, 10290, 10315, 10350, 10351, 10362, 10357, 10356, 10363, 
            10470, 10471, 10590, 10711, 10710, 10591, 10476, 10483, 10716, 10597, 
            10602, 10723, 10482, 10477, 10596, 10717, 10722, 10603, 10374, 10375, 
            10398, 10423, 10422, 10399, 10494, 10495, 10734, 10615, 10614, 10735, 
            10542, 10519, 10638, 10759, 10782, 10663, 10518, 10543, 10758, 10639, 
            10662, 10783, 10380, 10387, 10428, 10405, 10410, 10435, 10506, 10501, 
            10620, 10741, 10746, 10627, 10524, 10549, 10764, 10645, 10668, 10789, 
            10554, 10531, 10650, 10771, 10794, 10675, 10386, 10381, 10404, 10429, 
            10434, 10411, 10500, 10507, 10740, 10621, 10626, 10747, 10548, 10525, 
            10644, 10765, 10788, 10669, 10530, 10555, 10770, 10651, 10674, 10795, 
            15270, 15271, 15282, 15277, 15276, 15283, 15294, 15295, 15318, 15343, 
            15342, 15319, 15300, 15307, 15348, 15325, 15330, 15355, 15306, 15301, 
            15324, 15349, 15354, 15331, 20310, 20311, 20316, 20323, 20322, 20317, 
            25350, 25351, 35430, 30391, 30390, 35431, 25362, 25357, 30396, 35437, 
            35442, 30403, 25356, 25363, 35436, 30397, 30402, 35443, 20334, 20335, 
            20382, 20359, 20358, 20383, 25374, 25375, 30414, 35455, 35454, 30415, 
            25398, 25423, 35478, 30439, 30462, 35503, 25422, 25399, 30438, 35479, 
            35502, 30463, 20346, 20341, 20364, 20389, 20394, 20371, 25380, 25387, 
            35460, 30421, 30426, 35467, 25428, 25405, 30444, 35485, 35508, 30469, 
            25410, 25435, 35490, 30451, 30474, 35515, 20340, 20347, 20388, 20365, 
            20370, 20395, 25386, 25381, 30420, 35461, 35466, 30427, 25404, 25429, 
            35484, 30445, 30468, 35509, 25434, 25411, 30450, 35491, 35514, 30475, 
            15390, 15391, 15396, 15403, 15402, 15397, 15510, 15511, 15750, 15631, 
            15630, 15751, 15522, 15517, 15636, 15757, 15762, 15643, 15516, 15523, 
            15756, 15637, 15642, 15763, 20430, 20431, 20442, 20437, 20436, 20443, 
            25470, 25471, 30510, 35551, 35550, 30511, 25476, 25483, 35556, 30517, 
            30522, 35563, 25482, 25477, 30516, 35557, 35562, 30523, 20550, 20551, 
            20670, 20791, 20790, 20671, 25590, 25591, 35670, 30631, 30630, 35671, 
            25830, 25711, 30750, 35791, 35910, 30871, 25710, 25831, 35790, 30751, 
            30870, 35911, 20556, 20563, 20796, 20677, 20682, 20803, 25602, 25597, 
            30636, 35677, 35682, 30643, 25716, 25837, 35796, 30757, 30876, 35917, 
            25842, 25723, 30762, 35803, 35922, 30883, 20562, 20557, 20676, 20797, 
            20802, 20683, 25596, 25603, 35676, 30637, 30642, 35683, 25836, 25717, 
            30756, 35797, 35916, 30877, 25722, 25843, 35802, 30763, 30882, 35923, 
            15414, 15415, 15462, 15439, 15438, 15463, 15534, 15535, 15654, 15775, 
            15774, 15655, 15558, 15583, 15798, 15679, 15702, 15823, 15582, 15559, 
            15678, 15799, 15822, 15703, 20454, 20455, 20478, 20503, 20502, 20479, 
            25494, 25495, 35574, 30535, 30534, 35575, 25542, 25519, 30558, 35599, 
            35622, 30583, 25518, 25543, 35598, 30559, 30582, 35623, 20574, 20575, 
            20814, 20695, 20694, 20815, 25614, 25615, 30654, 35695, 35694, 30655, 
            25734, 25855, 35814, 30775, 30894, 35935, 25854, 25735, 30774, 35815, 
            35934, 30895, 20622, 20599, 20718, 20839, 20862, 20743, 25638, 25663, 
            35718, 30679, 30702, 35743, 25878, 25759, 30798, 35839, 35958, 30919, 
            25782, 25903, 35862, 30823, 30942, 35983, 20598, 20623, 20838, 20719, 
            20742, 20863, 25662, 25639, 30678, 35719, 35742, 30703, 25758, 25879, 
            35838, 30799, 30918, 35959, 25902, 25783, 30822, 35863, 35982, 30943, 
            15426, 15421, 15444, 15469, 15474, 15451, 15540, 15547, 15780, 15661, 
            15666, 15787, 15588, 15565, 15684, 15805, 15828, 15709, 15570, 15595, 
            15810, 15691, 15714, 15835, 20460, 20467, 20508, 20485, 20490, 20515, 
            25506, 25501, 30540, 35581, 35586, 30547, 25524, 25549, 35604, 30565, 
            30588, 35629, 25554, 25531, 30570, 35611, 35634, 30595, 20586, 20581, 
            20700, 20821, 20826, 20707, 25620, 25627, 35700, 30661, 30666, 35707, 
            25860, 25741, 30780, 35821, 35940, 30901, 25746, 25867, 35826, 30787, 
            30906, 35947, 20604, 20629, 20844, 20725, 20748, 20869, 25668, 25645, 
            30684, 35725, 35748, 30709, 25764, 25885, 35844, 30805, 30924, 35965, 
            25908, 25789, 30828, 35869, 35988, 30949, 20634, 20611, 20730, 20851, 
            20874, 20755, 25650, 25675, 35730, 30691, 30714, 35755, 25890, 25771, 
            30810, 35851, 35970, 30931, 25794, 25915, 35874, 30835, 30954, 35995, 
            15420, 15427, 15468, 15445, 15450, 15475, 15546, 15541, 15660, 15781, 
            15786, 15667, 15564, 15589, 15804, 15685, 15708, 15829, 15594, 15571, 
            15690, 15811, 15834, 15715, 20466, 20461, 20484, 20509, 20514, 20491, 
            25500, 25507, 35580, 30541, 30546, 35587, 25548, 25525, 30564, 35605, 
            35628, 30589, 25530, 25555, 35610, 30571, 30594, 35635, 20580, 20587, 
            20820, 20701, 20706, 20827, 25626, 25621, 30660, 35701, 35706, 30667, 
            25740, 25861, 35820, 30781, 30900, 35941, 25866, 25747, 30786, 35827, 
            35946, 30907, 20628, 20605, 20724, 20845, 20868, 20749, 25644, 25669, 
            35724, 30685, 30708, 35749, 25884, 25765, 30804, 35845, 35964, 30925, 
            25788, 25909, 35868, 30829, 30948, 35989, 20610, 20635, 20850, 20731, 
            20754, 20875, 25674, 25651, 30690, 35731, 35754, 30715, 25770, 25891, 
            35850, 30811, 30930, 35971, 25914, 25795, 30834, 35875, 35994, 30955, 
            10232, 10235, 10244, 10239, 10240, 10247, 10258, 10257, 10280, 10305, 
            10306, 10283, 10262, 10269, 10310, 10287, 10292, 10317, 10270, 10265, 
            10288, 10313, 10318, 10295, 10354, 10353, 10358, 10365, 10366, 10361, 
            10472, 10475, 10712, 10593, 10594, 10715, 10484, 10479, 10598, 10719, 
            10724, 10605, 10480, 10487, 10720, 10601, 10606, 10727, 10376, 10379, 
            10424, 10401, 10402, 10427, 10498, 10497, 10616, 10737, 10738, 10619, 
            10520, 10545, 10760, 10641, 10664, 10785, 10546, 10523, 10642, 10763, 
            10786, 10667, 10388, 10383, 10406, 10431, 10436, 10413, 10502, 10509, 
            10742, 10623, 10628, 10749, 10550, 10527, 10646, 10767, 10790, 10671, 
            10532, 10557, 10772, 10653, 10676, 10797, 10384, 10391, 10432, 10409, 
            10414, 10439, 10510, 10505, 10624, 10745, 10750, 10631, 10528, 10553, 
            10768, 10649, 10672, 10793, 10558, 10535, 10654, 10775, 10798, 10679, 
            15274, 15273, 15278, 15285, 15286, 15281, 15296, 15299, 15344, 15321, 
            15322, 15347, 15308, 15303, 15326, 15351, 15356, 15333, 15304, 15311, 
            15352, 15329, 15334, 15359, 20312, 20315, 20324, 20319, 20320, 20327, 
            25354, 25353, 30392, 35433, 35434, 30395, 25358, 25365, 35438, 30399, 
            30404, 35445, 25366, 25361, 30400, 35441, 35446, 30407, 20338, 20337, 
            20360, 20385, 20386, 20363, 25376, 25379, 35456, 30417, 30418, 35459, 
            25424, 25401, 30440, 35481, 35504, 30465, 25402, 25427, 35482, 30443, 
            30466, 35507, 20342, 20349, 20390, 20367, 20372, 20397, 25388, 25383, 
            30422, 35463, 35468, 30429, 25406, 25431, 35486, 30447, 30470, 35511, 
            25436, 25413, 30452, 35493, 35516, 30477, 20350, 20345, 20368, 20393, 
            20398, 20375, 25384, 25391, 35464, 30425, 30430, 35471, 25432, 25409, 
            30448, 35489, 35512, 30473, 25414, 25439, 35494, 30455, 30478, 35519, 
            15392, 15395, 15404, 15399, 15400, 15407, 15514, 15513, 15632, 15753, 
            15754, 15635, 15518, 15525, 15758, 15639, 15644, 15765, 15526, 15521, 
            15640, 15761, 15766, 15647, 20434, 20433, 20438, 20445, 20446, 20441, 
            25472, 25475, 35552, 30513, 30514, 35555, 25484, 25479, 30518, 35559, 
            35564, 30525, 25480, 25487, 35560, 30521, 30526, 35567, 20552, 20555, 
            20792, 20673, 20674, 20795, 25594, 25593, 30632, 35673, 35674, 30635, 
            25712, 25833, 35792, 30753, 30872, 35913, 25834, 25715, 30754, 35795, 
            35914, 30875, 20564, 20559, 20678, 20799, 20804, 20685, 25598, 25605, 
            35678, 30639, 30644, 35685, 25838, 25719, 30758, 35799, 35918, 30879, 
            25724, 25845, 35804, 30765, 30884, 35925, 20560, 20567, 20800, 20681, 
            20686, 20807, 25606, 25601, 30640, 35681, 35686, 30647, 25720, 25841, 
            35800, 30761, 30880, 35921, 25846, 25727, 30766, 35807, 35926, 30887, 
            15418, 15417, 15440, 15465, 15466, 15443, 15536, 15539, 15776, 15657, 
            15658, 15779, 15584, 15561, 15680, 15801, 15824, 15705, 15562, 15587, 
            15802, 15683, 15706, 15827, 20456, 20459, 20504, 20481, 20482, 20507, 
            25498, 25497, 30536, 35577, 35578, 30539, 25520, 25545, 35600, 30561, 
            30584, 35625, 25546, 25523, 30562, 35603, 35626, 30587, 20578, 20577, 
            20696, 20817, 20818, 20699, 25616, 25619, 35696, 30657, 30658, 35699, 
            25856, 25737, 30776, 35817, 35936, 30897, 25738, 25859, 35818, 30779, 
            30898, 35939, 20600, 20625, 20840, 20721, 20744, 20865, 25664, 25641, 
            30680, 35721, 35744, 30705, 25760, 25881, 35840, 30801, 30920, 35961, 
            25904, 25785, 30824, 35865, 35984, 30945, 20626, 20603, 20722, 20843, 
            20866, 20747, 25642, 25667, 35722, 30683, 30706, 35747, 25882, 25763, 
            30802, 35843, 35962, 30923, 25786, 25907, 35866, 30827, 30946, 35987, 
            15422, 15429, 15470, 15447, 15452, 15477, 15548, 15543, 15662, 15783, 
            15788, 15669, 15566, 15591, 15806, 15687, 15710, 15831, 15596, 15573, 
            15692, 15813, 15836, 15717, 20468, 20463, 20486, 20511, 20516, 20493, 
            25502, 25509, 35582, 30543, 30548, 35589, 25550, 25527, 30566, 35607, 
            35630, 30591, 25532, 25557, 35612, 30573, 30596, 35637, 20582, 20589, 
            20822, 20703, 20708, 20829, 25628, 25623, 30662, 35703, 35708, 30669, 
            25742, 25863, 35822, 30783, 30902, 35943, 25868, 25749, 30788, 35829, 
            35948, 30909, 20630, 20607, 20726, 20847, 20870, 20751, 25646, 25671, 
            35726, 30687, 30710, 35751, 25886, 25767, 30806, 35847, 35966, 30927, 
            25790, 25911, 35870, 30831, 30950, 35991, 20612, 20637, 20852, 20733, 
            20756, 20877, 25676, 25653, 30692, 35733, 35756, 30717, 25772, 25893, 
            35852, 30813, 30932, 35973, 25916, 25797, 30836, 35877, 35996, 30957, 
            15430, 15425, 15448, 15473, 15478, 15455, 15544, 15551, 15784, 15665, 
            15670, 15791, 15592, 15569, 15688, 15809, 15832, 15713, 15574, 15599, 
            15814, 15695, 15718, 15839, 20464, 20471, 20512, 20489, 20494, 20519, 
            25510, 25505, 30544, 35585, 35590, 30551, 25528, 25553, 35608, 30569, 
            30592, 35633, 25558, 25535, 30574, 35615, 35638, 30599, 20590, 20585, 
            20704, 20825, 20830, 20711, 25624, 25631, 35704, 30665, 30670, 35711, 
            25864, 25745, 30784, 35825, 35944, 30905, 25750, 25871, 35830, 30791, 
            30910, 35951, 20608, 20633, 20848, 20729, 20752, 20873, 25672, 25649, 
            30688, 35729, 35752, 30713, 25768, 25889, 35848, 30809, 30928, 35969, 
            25912, 25793, 30832, 35873, 35992, 30953, 20638, 20615, 20734, 20855, 
            20878, 20759, 25654, 25679, 35734, 30695, 30718, 35759, 25894, 25775, 
            30814, 35855, 35974, 30935, 25798, 25919, 35878, 30839, 30958, 35999, 
            10234, 10233, 10238, 10245, 10246, 10241, 10256, 10259, 10304, 10281, 
            10282, 10307, 10268, 10263, 10286, 10311, 10316, 10293, 10264, 10271, 
            10312, 10289, 10294, 10319, 10352, 10355, 10364, 10359, 10360, 10367, 
            10474, 10473, 10592, 10713, 10714, 10595, 10478, 10485, 10718, 10599, 
            10604, 10725, 10486, 10481, 10600, 10721, 10726, 10607, 10378, 10377, 
            10400, 10425, 10426, 10403, 10496, 10499, 10736, 10617, 10618, 10739, 
            10544, 10521, 10640, 10761, 10784, 10665, 10522, 10547, 10762, 10643, 
            10666, 10787, 10382, 10389, 10430, 10407, 10412, 10437, 10508, 10503, 
            10622, 10743, 10748, 10629, 10526, 10551, 10766, 10647, 10670, 10791, 
            10556, 10533, 10652, 10773, 10796, 10677, 10390, 10385, 10408, 10433, 
            10438, 10415, 10504, 10511, 10744, 10625, 10630, 10751, 10552, 10529, 
            10648, 10769, 10792, 10673, 10534, 10559, 10774, 10655, 10678, 10799, 
            15272, 15275, 15284, 15279, 15280, 15287, 15298, 15297, 15320, 15345, 
            15346, 15323, 15302, 15309, 15350, 15327, 15332, 15357, 15310, 15305, 
            15328, 15353, 15358, 15335, 20314, 20313, 20318, 20325, 20326, 20321, 
            25352, 25355, 35432, 30393, 30394, 35435, 25364, 25359, 30398, 35439, 
            35444, 30405, 25360, 25367, 35440, 30401, 30406, 35447, 20336, 20339, 
            20384, 20361, 20362, 20387, 25378, 25377, 30416, 35457, 35458, 30419, 
            25400, 25425, 35480, 30441, 30464, 35505, 25426, 25403, 30442, 35483, 
            35506, 30467, 20348, 20343, 20366, 20391, 20396, 20373, 25382, 25389, 
            35462, 30423, 30428, 35469, 25430, 25407, 30446, 35487, 35510, 30471, 
            25412, 25437, 35492, 30453, 30476, 35517, 20344, 20351, 20392, 20369, 
            20374, 20399, 25390, 25385, 30424, 35465, 35470, 30431, 25408, 25433, 
            35488, 30449, 30472, 35513, 25438, 25415, 30454, 35495, 35518, 30479, 
            15394, 15393, 15398, 15405, 15406, 15401, 15512, 15515, 15752, 15633, 
            15634, 15755, 15524, 15519, 15638, 15759, 15764, 15645, 15520, 15527, 
            15760, 15641, 15646, 15767, 20432, 20435, 20444, 20439, 20440, 20447, 
            25474, 25473, 30512, 35553, 35554, 30515, 25478, 25485, 35558, 30519, 
            30524, 35565, 25486, 25481, 30520, 35561, 35566, 30527, 20554, 20553, 
            20672, 20793, 20794, 20675, 25592, 25595, 35672, 30633, 30634, 35675, 
            25832, 25713, 30752, 35793, 35912, 30873, 25714, 25835, 35794, 30755, 
            30874, 35915, 20558, 20565, 20798, 20679, 20684, 20805, 25604, 25599, 
            30638, 35679, 35684, 30645, 25718, 25839, 35798, 30759, 30878, 35919, 
            25844, 25725, 30764, 35805, 35924, 30885, 20566, 20561, 20680, 20801, 
            20806, 20687, 25600, 25607, 35680, 30641, 30646, 35687, 25840, 25721, 
            30760, 35801, 35920, 30881, 25726, 25847, 35806, 30767, 30886, 35927, 
            15416, 15419, 15464, 15441, 15442, 15467, 15538, 15537, 15656, 15777, 
            15778, 15659, 15560, 15585, 15800, 15681, 15704, 15825, 15586, 15563, 
            15682, 15803, 15826, 15707, 20458, 20457, 20480, 20505, 20506, 20483, 
            25496, 25499, 35576, 30537, 30538, 35579, 25544, 25521, 30560, 35601, 
            35624, 30585, 25522, 25547, 35602, 30563, 30586, 35627, 20576, 20579, 
            20816, 20697, 20698, 20819, 25618, 25617, 30656, 35697, 35698, 30659, 
            25736, 25857, 35816, 30777, 30896, 35937, 25858, 25739, 30778, 35819, 
            35938, 30899, 20624, 20601, 20720, 20841, 20864, 20745, 25640, 25665, 
            35720, 30681, 30704, 35745, 25880, 25761, 30800, 35841, 35960, 30921, 
            25784, 25905, 35864, 30825, 30944, 35985, 20602, 20627, 20842, 20723, 
            20746, 20867, 25666, 25643, 30682, 35723, 35746, 30707, 25762, 25883, 
            35842, 30803, 30922, 35963, 25906, 25787, 30826, 35867, 35986, 30947, 
            15428, 15423, 15446, 15471, 15476, 15453, 15542, 15549, 15782, 15663, 
            15668, 15789, 15590, 15567, 15686, 15807, 15830, 15711, 15572, 15597, 
            15812, 15693, 15716, 15837, 20462, 20469, 20510, 20487, 20492, 20517, 
            25508, 25503, 30542, 35583, 35588, 30549, 25526, 25551, 35606, 30567, 
            30590, 35631, 25556, 25533, 30572, 35613, 35636, 30597, 20588, 20583, 
            20702, 20823, 20828, 20709, 25622, 25629, 35702, 30663, 30668, 35709, 
            25862, 25743, 30782, 35823, 35942, 30903, 25748, 25869, 35828, 30789, 
            30908, 35949, 20606, 20631, 20846, 20727, 20750, 20871, 25670, 25647, 
            30686, 35727, 35750, 30711, 25766, 25887, 35846, 30807, 30926, 35967, 
            25910, 25791, 30830, 35871, 35990, 30951, 20636, 20613, 20732, 20853, 
            20876, 20757, 25652, 25677, 35732, 30693, 30716, 35757, 25892, 25773, 
            30812, 35853, 35972, 30933, 25796, 25917, 35876, 30837, 30956, 35997, 
            15424, 15431, 15472, 15449, 15454, 15479, 15550, 15545, 15664, 15785, 
            15790, 15671, 15568, 15593, 15808, 15689, 15712, 15833, 15598, 15575, 
            15694, 15815, 15838, 15719, 20470, 20465, 20488, 20513, 20518, 20495, 
            25504, 25511, 35584, 30545, 30550, 35591, 25552, 25529, 30568, 35609, 
            35632, 30593, 25534, 25559, 35614, 30575, 30598, 35639, 20584, 20591, 
            20824, 20705, 20710, 20831, 25630, 25625, 30664, 35705, 35710, 30671, 
            25744, 25865, 35824, 30785, 30904, 35945, 25870, 25751, 30790, 35831, 
            35950, 30911, 20632, 20609, 20728, 20849, 20872, 20753, 25648, 25673, 
            35728, 30689, 30712, 35753, 25888, 25769, 30808, 35849, 35968, 30929, 
            25792, 25913, 35872, 30833, 30952, 35993, 20614, 20639, 20854, 20735, 
            20758, 20879, 25678, 25655, 30694, 35735, 35758, 30719, 25774, 25895, 
            35854, 30815, 30934, 35975, 25918, 25799, 30838, 35879, 35998, 30959, 
            5760, 5761, 5764, 5763, 5762, 5765, 5766, 5767, 5772, 5779, 
            5778, 5773, 5768, 5771, 5780, 5775, 5776, 5783, 5770, 5769, 
            5774, 5781, 5782, 5777, 5784, 5785, 5786, 5789, 5788, 5787, 
            5808, 5809, 5856, 5833, 5832, 5857, 5812, 5811, 5834, 5859, 
            5860, 5837, 5810, 5813, 5858, 5835, 5836, 5861, 5790, 5791, 
            5802, 5797, 5796, 5803, 5814, 5815, 5838, 5863, 5862, 5839, 
            5820, 5827, 5868, 5845, 5850, 5875, 5826, 5821, 5844, 5869, 
            5874, 5851, 5794, 5793, 5798, 5805, 5806, 5801, 5816, 5819, 
            5864, 5841, 5842, 5867, 5828, 5823, 5846, 5871, 5876, 5853, 
            5824, 5831, 5872, 5849, 5854, 5879, 5792, 5795, 5804, 5799, 
            5800, 5807, 5818, 5817, 5840, 5865, 5866, 5843, 5822, 5829, 
            5870, 5847, 5852, 5877, 5830, 5825, 5848, 5873, 5878, 5855, 
            6480, 6481, 6482, 6485, 6484, 6483, 6486, 6487, 6498, 6493, 
            6492, 6499, 6490, 6489, 6494, 6501, 6502, 6497, 6488, 6491, 
            6500, 6495, 6496, 6503, 7200, 7201, 7204, 7203, 7202, 7205, 
            7920, 7921, 8640, 9361, 9360, 8641, 7922, 7925, 9362, 8643, 
            8644, 9365, 7924, 7923, 8642, 9363, 9364, 8645, 7206, 7207, 
            7212, 7219, 7218, 7213, 7926, 7927, 9366, 8647, 8646, 9367, 
            7938, 7933, 8652, 9373, 9378, 8659, 7932, 7939, 9372, 8653, 
            8658, 9379, 7208, 7211, 7220, 7215, 7216, 7223, 7930, 7929, 
            8648, 9369, 9370, 8651, 7934, 7941, 9374, 8655, 8660, 9381, 
            7942, 7937, 8656, 9377, 9382, 8663, 7210, 7209, 7214, 7221, 
            7222, 7217, 7928, 7931, 9368, 8649, 8650, 9371, 7940, 7935, 
            8654, 9375, 9380, 8661, 7936, 7943, 9376, 8657, 8662, 9383, 
            6504, 6505, 6508, 6507, 6506, 6509, 6528, 6529, 6552, 6577, 
            6576, 6553, 6530, 6533, 6578, 6555, 6556, 6581, 6532, 6531, 
            6554, 6579, 6580, 6557, 7224, 7225, 7226, 7229, 7228, 7227, 
            7944, 7945, 9384, 8665, 8664, 9385, 7948, 7947, 8666, 9387, 
            9388, 8669, 7946, 7949, 9386, 8667, 8668, 9389, 7248, 7249, 
            7296, 7273, 7272, 7297, 7968, 7969, 8688, 9409, 9408, 8689, 
            7992, 8017, 9432, 8713, 8736, 9457, 8016, 7993, 8712, 9433, 
            9456, 8737, 7252, 7251, 7274, 7299, 7300, 7277, 7970, 7973, 
            9410, 8691, 8692, 9413, 8018, 7995, 8714, 9435, 9458, 8739, 
            7996, 8021, 9436, 8717, 8740, 9461, 7250, 7253, 7298, 7275, 
            7276, 7301, 7972, 7971, 8690, 9411, 9412, 8693, 7994, 8019, 
            9434, 8715, 8738, 9459, 8020, 7997, 8716, 9437, 9460, 8741, 
            6510, 6511, 6516, 6523, 6522, 6517, 6534, 6535, 6582, 6559, 
            6558, 6583, 6546, 6541, 6564, 6589, 6594, 6571, 6540, 6547, 
            6588, 6565, 6570, 6595, 7230, 7231, 7242, 7237, 7236, 7243, 
            7950, 7951, 8670, 9391, 9390, 8671, 7956, 7963, 9396, 8677, 
            8682, 9403, 7962, 7957, 8676, 9397, 9402, 8683, 7254, 7255, 
            7278, 7303, 7302, 7279, 7974, 7975, 9414, 8695, 8694, 9415, 
            8022, 7999, 8718, 9439, 9462, 8743, 7998, 8023, 9438, 8719, 
            8742, 9463, 7260, 7267, 7308, 7285, 7290, 7315, 7986, 7981, 
            8700, 9421, 9426, 8707, 8004, 8029, 9444, 8725, 8748, 9469, 
            8034, 8011, 8730, 9451, 9474, 8755, 7266, 7261, 7284, 7309, 
            7314, 7291, 7980, 7987, 9420, 8701, 8706, 9427, 8028, 8005, 
            8724, 9445, 9468, 8749, 8010, 8035, 9450, 8731, 8754, 9475, 
            6512, 6515, 6524, 6519, 6520, 6527, 6538, 6537, 6560, 6585, 
            6586, 6563, 6542, 6549, 6590, 6567, 6572, 6597, 6550, 6545, 
            6568, 6593, 6598, 6575, 7234, 7233, 7238, 7245, 7246, 7241, 
            7952, 7955, 9392, 8673, 8674, 9395, 7964, 7959, 8678, 9399, 
            9404, 8685, 7960, 7967, 9400, 8681, 8686, 9407, 7256, 7259, 
            7304, 7281, 7282, 7307, 7978, 7977, 8696, 9417, 9418, 8699, 
            8000, 8025, 9440, 8721, 8744, 9465, 8026, 8003, 8722, 9443, 
            9466, 8747, 7268, 7263, 7286, 7311, 7316, 7293, 7982, 7989, 
            9422, 8703, 8708, 9429, 8030, 8007, 8726, 9447, 9470, 8751, 
            8012, 8037, 9452, 8733, 8756, 9477, 7264, 7271, 7312, 7289, 
            7294, 7319, 7990, 7985, 8704, 9425, 9430, 8711, 8008, 8033, 
            9448, 8729, 8752, 9473, 8038, 8015, 8734, 9455, 9478, 8759, 
            6514, 6513, 6518, 6525, 6526, 6521, 6536, 6539, 6584, 6561, 
            6562, 6587, 6548, 6543, 6566, 6591, 6596, 6573, 6544, 6551, 
            6592, 6569, 6574, 6599, 7232, 7235, 7244, 7239, 7240, 7247, 
            7954, 7953, 8672, 9393, 9394, 8675, 7958, 7965, 9398, 8679, 
            8684, 9405, 7966, 7961, 8680, 9401, 9406, 8687, 7258, 7257, 
            7280, 7305, 7306, 7283, 7976, 7979, 9416, 8697, 8698, 9419, 
            8024, 8001, 8720, 9441, 9464, 8745, 8002, 8027, 9442, 8723, 
            8746, 9467, 7262, 7269, 7310, 7287, 7292, 7317, 7988, 7983, 
            8702, 9423, 9428, 8709, 8006, 8031, 9446, 8727, 8750, 9471, 
            8036, 8013, 8732, 9453, 9476, 8757, 7270, 7265, 7288, 7313, 
            7318, 7295, 7984, 7991, 9424, 8705, 8710, 9431, 8032, 8009, 
            8728, 9449, 9472, 8753, 8014, 8039, 9454, 8735, 8758, 9479, 
            10800, 10801, 10802, 10805, 10804, 10803, 10806, 10807, 10818, 10813, 
            10812, 10819, 10810, 10809, 10814, 10821, 10822, 10817, 10808, 10811, 
            10820, 10815, 10816, 10823, 10824, 10825, 10828, 10827, 10826, 10829, 
            10848, 10849, 10872, 10897, 10896, 10873, 10850, 10853, 10898, 10875, 
            10876, 10901, 10852, 10851, 10874, 10899, 10900, 10877, 10830, 10831, 
            10836, 10843, 10842, 10837, 10854, 10855, 10902, 10879, 10878, 10903, 
            10866, 10861, 10884, 10909, 10914, 10891, 10860, 10867, 10908, 10885, 
            10890, 10915, 10832, 10835, 10844, 10839, 10840, 10847, 10858, 10857, 
            10880, 10905, 10906, 10883, 10862, 10869, 10910, 10887, 10892, 10917, 
            10870, 10865, 10888, 10913, 10918, 10895, 10834, 10833, 10838, 10845, 
            10846, 10841, 10856, 10859, 10904, 10881, 10882, 10907, 10868, 10863, 
            10886, 10911, 10916, 10893, 10864, 10871, 10912, 10889, 10894, 10919, 
            15840, 15841, 15844, 15843, 15842, 15845, 15846, 15847, 15852, 15859, 
            15858, 15853, 15848, 15851, 15860, 15855, 15856, 15863, 15850, 15849, 
            15854, 15861, 15862, 15857, 20880, 20881, 20882, 20885, 20884, 20883, 
            25920, 25921, 36000, 30961, 30960, 36001, 25924, 25923, 30962, 36003, 
            36004, 30965, 25922, 25925, 36002, 30963, 30964, 36005, 20886, 20887, 
            20898, 20893, 20892, 20899, 25926, 25927, 30966, 36007, 36006, 30967, 
            25932, 25939, 36012, 30973, 30978, 36019, 25938, 25933, 30972, 36013, 
            36018, 30979, 20890, 20889, 20894, 20901, 20902, 20897, 25928, 25931, 
            36008, 30969, 30970, 36011, 25940, 25935, 30974, 36015, 36020, 30981, 
            25936, 25943, 36016, 30977, 30982, 36023, 20888, 20891, 20900, 20895, 
            20896, 20903, 25930, 25929, 30968, 36009, 36010, 30971, 25934, 25941, 
            36014, 30975, 30980, 36021, 25942, 25937, 30976, 36017, 36022, 30983, 
            15864, 15865, 15866, 15869, 15868, 15867, 15888, 15889, 15936, 15913, 
            15912, 15937, 15892, 15891, 15914, 15939, 15940, 15917, 15890, 15893, 
            15938, 15915, 15916, 15941, 20904, 20905, 20908, 20907, 20906, 20909, 
            25944, 25945, 30984, 36025, 36024, 30985, 25946, 25949, 36026, 30987, 
            30988, 36029, 25948, 25947, 30986, 36027, 36028, 30989, 20928, 20929, 
            20952, 20977, 20976, 20953, 25968, 25969, 36048, 31009, 31008, 36049, 
            26016, 25993, 31032, 36073, 36096, 31057, 25992, 26017, 36072, 31033, 
            31056, 36097, 20930, 20933, 20978, 20955, 20956, 20981, 25972, 25971, 
            31010, 36051, 36052, 31013, 25994, 26019, 36074, 31035, 31058, 36099, 
            26020, 25997, 31036, 36077, 36100, 31061, 20932, 20931, 20954, 20979, 
            20980, 20957, 25970, 25973, 36050, 31011, 31012, 36053, 26018, 25995, 
            31034, 36075, 36098, 31059, 25996, 26021, 36076, 31037, 31060, 36101, 
            15870, 15871, 15882, 15877, 15876, 15883, 15894, 15895, 15918, 15943, 
            15942, 15919, 15900, 15907, 15948, 15925, 15930, 15955, 15906, 15901, 
            15924, 15949, 15954, 15931, 20910, 20911, 20916, 20923, 20922, 20917, 
            25950, 25951, 36030, 30991, 30990, 36031, 25962, 25957, 30996, 36037, 
            36042, 31003, 25956, 25963, 36036, 30997, 31002, 36043, 20934, 20935, 
            20982, 20959, 20958, 20983, 25974, 25975, 31014, 36055, 36054, 31015, 
            25998, 26023, 36078, 31039, 31062, 36103, 26022, 25999, 31038, 36079, 
            36102, 31063, 20946, 20941, 20964, 20989, 20994, 20971, 25980, 25987, 
            36060, 31021, 31026, 36067, 26028, 26005, 31044, 36085, 36108, 31069, 
            26010, 26035, 36090, 31051, 31074, 36115, 20940, 20947, 20988, 20965, 
            20970, 20995, 25986, 25981, 31020, 36061, 36066, 31027, 26004, 26029, 
            36084, 31045, 31068, 36109, 26034, 26011, 31050, 36091, 36114, 31075, 
            15874, 15873, 15878, 15885, 15886, 15881, 15896, 15899, 15944, 15921, 
            15922, 15947, 15908, 15903, 15926, 15951, 15956, 15933, 15904, 15911, 
            15952, 15929, 15934, 15959, 20912, 20915, 20924, 20919, 20920, 20927, 
            25954, 25953, 30992, 36033, 36034, 30995, 25958, 25965, 36038, 30999, 
            31004, 36045, 25966, 25961, 31000, 36041, 36046, 31007, 20938, 20937, 
            20960, 20985, 20986, 20963, 25976, 25979, 36056, 31017, 31018, 36059, 
            26024, 26001, 31040, 36081, 36104, 31065, 26002, 26027, 36082, 31043, 
            31066, 36107, 20942, 20949, 20990, 20967, 20972, 20997, 25988, 25983, 
            31022, 36063, 36068, 31029, 26006, 26031, 36086, 31047, 31070, 36111, 
            26036, 26013, 31052, 36093, 36116, 31077, 20950, 20945, 20968, 20993, 
            20998, 20975, 25984, 25991, 36064, 31025, 31030, 36071, 26032, 26009, 
            31048, 36089, 36112, 31073, 26014, 26039, 36094, 31055, 31078, 36119, 
            15872, 15875, 15884, 15879, 15880, 15887, 15898, 15897, 15920, 15945, 
            15946, 15923, 15902, 15909, 15950, 15927, 15932, 15957, 15910, 15905, 
            15928, 15953, 15958, 15935, 20914, 20913, 20918, 20925, 20926, 20921, 
            25952, 25955, 36032, 30993, 30994, 36035, 25964, 25959, 30998, 36039, 
            36044, 31005, 25960, 25967, 36040, 31001, 31006, 36047, 20936, 20939, 
            20984, 20961, 20962, 20987, 25978, 25977, 31016, 36057, 36058, 31019, 
            26000, 26025, 36080, 31041, 31064, 36105, 26026, 26003, 31042, 36083, 
            36106, 31067, 20948, 20943, 20966, 20991, 20996, 20973, 25982, 25989, 
            36062, 31023, 31028, 36069, 26030, 26007, 31046, 36087, 36110, 31071, 
            26012, 26037, 36092, 31053, 31076, 36117, 20944, 20951, 20992, 20969, 
            20974, 20999, 25990, 25985, 31024, 36065, 36070, 31031, 26008, 26033, 
            36088, 31049, 31072, 36113, 26038, 26015, 31054, 36095, 36118, 31079, 
            11520, 11521, 11524, 11523, 11522, 11525, 11526, 11527, 11532, 11539, 
            11538, 11533, 11528, 11531, 11540, 11535, 11536, 11543, 11530, 11529, 
            11534, 11541, 11542, 11537, 12240, 12241, 12242, 12245, 12244, 12243, 
            12960, 12961, 14400, 13681, 13680, 14401, 12964, 12963, 13682, 14403, 
            14404, 13685, 12962, 12965, 14402, 13683, 13684, 14405, 12246, 12247, 
            12258, 12253, 12252, 12259, 12966, 12967, 13686, 14407, 14406, 13687, 
            12972, 12979, 14412, 13693, 13698, 14419, 12978, 12973, 13692, 14413, 
            14418, 13699, 12250, 12249, 12254, 12261, 12262, 12257, 12968, 12971, 
            14408, 13689, 13690, 14411, 12980, 12975, 13694, 14415, 14420, 13701, 
            12976, 12983, 14416, 13697, 13702, 14423, 12248, 12251, 12260, 12255, 
            12256, 12263, 12970, 12969, 13688, 14409, 14410, 13691, 12974, 12981, 
            14414, 13695, 13700, 14421, 12982, 12977, 13696, 14417, 14422, 13703, 
            16560, 16561, 16562, 16565, 16564, 16563, 16566, 16567, 16578, 16573, 
            16572, 16579, 16570, 16569, 16574, 16581, 16582, 16577, 16568, 16571, 
            16580, 16575, 16576, 16583, 21600, 21601, 21604, 21603, 21602, 21605, 
            26640, 26641, 31680, 36721, 36720, 31681, 26642, 26645, 36722, 31683, 
            31684, 36725, 26644, 26643, 31682, 36723, 36724, 31685, 21606, 21607, 
            21612, 21619, 21618, 21613, 26646, 26647, 36726, 31687, 31686, 36727, 
            26658, 26653, 31692, 36733, 36738, 31699, 26652, 26659, 36732, 31693, 
            31698, 36739, 21608, 21611, 21620, 21615, 21616, 21623, 26650, 26649, 
            31688, 36729, 36730, 31691, 26654, 26661, 36734, 31695, 31700, 36741, 
            26662, 26657, 31696, 36737, 36742, 31703, 21610, 21609, 21614, 21621, 
            21622, 21617, 26648, 26651, 36728, 31689, 31690, 36731, 26660, 26655, 
            31694, 36735, 36740, 31701, 26656, 26663, 36736, 31697, 31702, 36743, 
            17280, 17281, 17284, 17283, 17282, 17285, 18000, 18001, 18720, 19441, 
            19440, 18721, 18002, 18005, 19442, 18723, 18724, 19445, 18004, 18003, 
            18722, 19443, 19444, 18725, 22320, 22321, 22322, 22325, 22324, 22323, 
            27360, 27361, 37440, 32401, 32400, 37441, 27364, 27363, 32402, 37443, 
            37444, 32405, 27362, 27365, 37442, 32403, 32404, 37445, 23040, 23041, 
            24480, 23761, 23760, 24481, 28080, 28081, 33120, 38161, 38160, 33121, 
            28800, 29521, 38880, 33841, 34560, 39601, 29520, 28801, 33840, 38881, 
            39600, 34561, 23044, 23043, 23762, 24483, 24484, 23765, 28082, 28085, 
            38162, 33123, 33124, 38165, 29522, 28803, 33842, 38883, 39602, 34563, 
            28804, 29525, 38884, 33845, 34564, 39605, 23042, 23045, 24482, 23763, 
            23764, 24485, 28084, 28083, 33122, 38163, 38164, 33125, 28802, 29523, 
            38882, 33843, 34562, 39603, 29524, 28805, 33844, 38885, 39604, 34565, 
            17286, 17287, 17292, 17299, 17298, 17293, 18006, 18007, 19446, 18727, 
            18726, 19447, 18018, 18013, 18732, 19453, 19458, 18739, 18012, 18019, 
            19452, 18733, 18738, 19459, 22326, 22327, 22338, 22333, 22332, 22339, 
            27366, 27367, 32406, 37447, 37446, 32407, 27372, 27379, 37452, 32413, 
            32418, 37459, 27378, 27373, 32412, 37453, 37458, 32419, 23046, 23047, 
            23766, 24487, 24486, 23767, 28086, 28087, 38166, 33127, 33126, 38167, 
            29526, 28807, 33846, 38887, 39606, 34567, 28806, 29527, 38886, 33847, 
            34566, 39607, 23052, 23059, 24492, 23773, 23778, 24499, 28098, 28093, 
            33132, 38173, 38178, 33139, 28812, 29533, 38892, 33853, 34572, 39613, 
            29538, 28819, 33858, 38899, 39618, 34579, 23058, 23053, 23772, 24493, 
            24498, 23779, 28092, 28099, 38172, 33133, 33138, 38179, 29532, 28813, 
            33852, 38893, 39612, 34573, 28818, 29539, 38898, 33859, 34578, 39619, 
            17288, 17291, 17300, 17295, 17296, 17303, 18010, 18009, 18728, 19449, 
            19450, 18731, 18014, 18021, 19454, 18735, 18740, 19461, 18022, 18017, 
            18736, 19457, 19462, 18743, 22330, 22329, 22334, 22341, 22342, 22337, 
            27368, 27371, 37448, 32409, 32410, 37451, 27380, 27375, 32414, 37455, 
            37460, 32421, 27376, 27383, 37456, 32417, 32422, 37463, 23048, 23051, 
            24488, 23769, 23770, 24491, 28090, 28089, 33128, 38169, 38170, 33131, 
            28808, 29529, 38888, 33849, 34568, 39609, 29530, 28811, 33850, 38891, 
            39610, 34571, 23060, 23055, 23774, 24495, 24500, 23781, 28094, 28101, 
            38174, 33135, 33140, 38181, 29534, 28815, 33854, 38895, 39614, 34575, 
            28820, 29541, 38900, 33861, 34580, 39621, 23056, 23063, 24496, 23777, 
            23782, 24503, 28102, 28097, 33136, 38177, 38182, 33143, 28816, 29537, 
            38896, 33857, 34576, 39617, 29542, 28823, 33862, 38903, 39622, 34583, 
            17290, 17289, 17294, 17301, 17302, 17297, 18008, 18011, 19448, 18729, 
            18730, 19451, 18020, 18015, 18734, 19455, 19460, 18741, 18016, 18023, 
            19456, 18737, 18742, 19463, 22328, 22331, 22340, 22335, 22336, 22343, 
            27370, 27369, 32408, 37449, 37450, 32411, 27374, 27381, 37454, 32415, 
            32420, 37461, 27382, 27377, 32416, 37457, 37462, 32423, 23050, 23049, 
            23768, 24489, 24490, 23771, 28088, 28091, 38168, 33129, 33130, 38171, 
            29528, 28809, 33848, 38889, 39608, 34569, 28810, 29531, 38890, 33851, 
            34570, 39611, 23054, 23061, 24494, 23775, 23780, 24501, 28100, 28095, 
            33134, 38175, 38180, 33141, 28814, 29535, 38894, 33855, 34574, 39615, 
            29540, 28821, 33860, 38901, 39620, 34581, 23062, 23057, 23776, 24497, 
            24502, 23783, 28096, 28103, 38176, 33137, 33142, 38183, 29536, 28817, 
            33856, 38897, 39616, 34577, 28822, 29543, 38902, 33863, 34582, 39623, 
            11544, 11545, 11546, 11549, 11548, 11547, 11568, 11569, 11616, 11593, 
            11592, 11617, 11572, 11571, 11594, 11619, 11620, 11597, 11570, 11573, 
            11618, 11595, 11596, 11621, 12264, 12265, 12268, 12267, 12266, 12269, 
            12984, 12985, 13704, 14425, 14424, 13705, 12986, 12989, 14426, 13707, 
            13708, 14429, 12988, 12987, 13706, 14427, 14428, 13709, 12288, 12289, 
            12312, 12337, 12336, 12313, 13008, 13009, 14448, 13729, 13728, 14449, 
            13056, 13033, 13752, 14473, 14496, 13777, 13032, 13057, 14472, 13753, 
            13776, 14497, 12290, 12293, 12338, 12315, 12316, 12341, 13012, 13011, 
            13730, 14451, 14452, 13733, 13034, 13059, 14474, 13755, 13778, 14499, 
            13060, 13037, 13756, 14477, 14500, 13781, 12292, 12291, 12314, 12339, 
            12340, 12317, 13010, 13013, 14450, 13731, 13732, 14453, 13058, 13035, 
            13754, 14475, 14498, 13779, 13036, 13061, 14476, 13757, 13780, 14501, 
            16584, 16585, 16588, 16587, 16586, 16589, 16608, 16609, 16632, 16657, 
            16656, 16633, 16610, 16613, 16658, 16635, 16636, 16661, 16612, 16611, 
            16634, 16659, 16660, 16637, 21624, 21625, 21626, 21629, 21628, 21627, 
            26664, 26665, 36744, 31705, 31704, 36745, 26668, 26667, 31706, 36747, 
            36748, 31709, 26666, 26669, 36746, 31707, 31708, 36749, 21648, 21649, 
            21696, 21673, 21672, 21697, 26688, 26689, 31728, 36769, 36768, 31729, 
            26712, 26737, 36792, 31753, 31776, 36817, 26736, 26713, 31752, 36793, 
            36816, 31777, 21652, 21651, 21674, 21699, 21700, 21677, 26690, 26693, 
            36770, 31731, 31732, 36773, 26738, 26715, 31754, 36795, 36818, 31779, 
            26716, 26741, 36796, 31757, 31780, 36821, 21650, 21653, 21698, 21675, 
            21676, 21701, 26692, 26691, 31730, 36771, 36772, 31733, 26714, 26739, 
            36794, 31755, 31778, 36819, 26740, 26717, 31756, 36797, 36820, 31781, 
            17304, 17305, 17306, 17309, 17308, 17307, 18024, 18025, 19464, 18745, 
            18744, 19465, 18028, 18027, 18746, 19467, 19468, 18749, 18026, 18029, 
            19466, 18747, 18748, 19469, 22344, 22345, 22348, 22347, 22346, 22349, 
            27384, 27385, 32424, 37465, 37464, 32425, 27386, 27389, 37466, 32427, 
            32428, 37469, 27388, 27387, 32426, 37467, 37468, 32429, 23064, 23065, 
            23784, 24505, 24504, 23785, 28104, 28105, 38184, 33145, 33144, 38185, 
            29544, 28825, 33864, 38905, 39624, 34585, 28824, 29545, 38904, 33865, 
            34584, 39625, 23066, 23069, 24506, 23787, 23788, 24509, 28108, 28107, 
            33146, 38187, 38188, 33149, 28826, 29547, 38906, 33867, 34586, 39627, 
            29548, 28829, 33868, 38909, 39628, 34589, 23068, 23067, 23786, 24507, 
            24508, 23789, 28106, 28109, 38186, 33147, 33148, 38189, 29546, 28827, 
            33866, 38907, 39626, 34587, 28828, 29549, 38908, 33869, 34588, 39629, 
            17328, 17329, 17376, 17353, 17352, 17377, 18048, 18049, 18768, 19489, 
            19488, 18769, 18072, 18097, 19512, 18793, 18816, 19537, 18096, 18073, 
            18792, 19513, 19536, 18817, 22368, 22369, 22392, 22417, 22416, 22393, 
            27408, 27409, 37488, 32449, 32448, 37489, 27456, 27433, 32472, 37513, 
            37536, 32497, 27432, 27457, 37512, 32473, 32496, 37537, 23088, 23089, 
            24528, 23809, 23808, 24529, 28128, 28129, 33168, 38209, 38208, 33169, 
            28848, 29569, 38928, 33889, 34608, 39649, 29568, 28849, 33888, 38929, 
            39648, 34609, 23136, 23113, 23832, 24553, 24576, 23857, 28152, 28177, 
            38232, 33193, 33216, 38257, 29592, 28873, 33912, 38953, 39672, 34633, 
            28896, 29617, 38976, 33937, 34656, 39697, 23112, 23137, 24552, 23833, 
            23856, 24577, 28176, 28153, 33192, 38233, 38256, 33217, 28872, 29593, 
            38952, 33913, 34632, 39673, 29616, 28897, 33936, 38977, 39696, 34657, 
            17332, 17331, 17354, 17379, 17380, 17357, 18050, 18053, 19490, 18771, 
            18772, 19493, 18098, 18075, 18794, 19515, 19538, 18819, 18076, 18101, 
            19516, 18797, 18820, 19541, 22370, 22373, 22418, 22395, 22396, 22421, 
            27412, 27411, 32450, 37491, 37492, 32453, 27434, 27459, 37514, 32475, 
            32498, 37539, 27460, 27437, 32476, 37517, 37540, 32501, 23092, 23091, 
            23810, 24531, 24532, 23813, 28130, 28133, 38210, 33171, 33172, 38213, 
            29570, 28851, 33890, 38931, 39650, 34611, 28852, 29573, 38932, 33893, 
            34612, 39653, 23114, 23139, 24554, 23835, 23858, 24579, 28178, 28155, 
            33194, 38235, 38258, 33219, 28874, 29595, 38954, 33915, 34634, 39675, 
            29618, 28899, 33938, 38979, 39698, 34659, 23140, 23117, 23836, 24557, 
            24580, 23861, 28156, 28181, 38236, 33197, 33220, 38261, 29596, 28877, 
            33916, 38957, 39676, 34637, 28900, 29621, 38980, 33941, 34660, 39701, 
            17330, 17333, 17378, 17355, 17356, 17381, 18052, 18051, 18770, 19491, 
            19492, 18773, 18074, 18099, 19514, 18795, 18818, 19539, 18100, 18077, 
            18796, 19517, 19540, 18821, 22372, 22371, 22394, 22419, 22420, 22397, 
            27410, 27413, 37490, 32451, 32452, 37493, 27458, 27435, 32474, 37515, 
            37538, 32499, 27436, 27461, 37516, 32477, 32500, 37541, 23090, 23093, 
            24530, 23811, 23812, 24533, 28132, 28131, 33170, 38211, 38212, 33173, 
            28850, 29571, 38930, 33891, 34610, 39651, 29572, 28853, 33892, 38933, 
            39652, 34613, 23138, 23115, 23834, 24555, 24578, 23859, 28154, 28179, 
            38234, 33195, 33218, 38259, 29594, 28875, 33914, 38955, 39674, 34635, 
            28898, 29619, 38978, 33939, 34658, 39699, 23116, 23141, 24556, 23837, 
            23860, 24581, 28180, 28157, 33196, 38237, 38260, 33221, 28876, 29597, 
            38956, 33917, 34636, 39677, 29620, 28901, 33940, 38981, 39700, 34661, 
            11550, 11551, 11562, 11557, 11556, 11563, 11574, 11575, 11598, 11623, 
            11622, 11599, 11580, 11587, 11628, 11605, 11610, 11635, 11586, 11581, 
            11604, 11629, 11634, 11611, 12270, 12271, 12276, 12283, 12282, 12277, 
            12990, 12991, 14430, 13711, 13710, 14431, 13002, 12997, 13716, 14437, 
            14442, 13723, 12996, 13003, 14436, 13717, 13722, 14443, 12294, 12295, 
            12342, 12319, 12318, 12343, 13014, 13015, 13734, 14455, 14454, 13735, 
            13038, 13063, 14478, 13759, 13782, 14503, 13062, 13039, 13758, 14479, 
            14502, 13783, 12306, 12301, 12324, 12349, 12354, 12331, 13020, 13027, 
            14460, 13741, 13746, 14467, 13068, 13045, 13764, 14485, 14508, 13789, 
            13050, 13075, 14490, 13771, 13794, 14515, 12300, 12307, 12348, 12325, 
            12330, 12355, 13026, 13021, 13740, 14461, 14466, 13747, 13044, 13069, 
            14484, 13765, 13788, 14509, 13074, 13051, 13770, 14491, 14514, 13795, 
            16590, 16591, 16596, 16603, 16602, 16597, 16614, 16615, 16662, 16639, 
            16638, 16663, 16626, 16621, 16644, 16669, 16674, 16651, 16620, 16627, 
            16668, 16645, 16650, 16675, 21630, 21631, 21642, 21637, 21636, 21643, 
            26670, 26671, 31710, 36751, 36750, 31711, 26676, 26683, 36756, 31717, 
            31722, 36763, 26682, 26677, 31716, 36757, 36762, 31723, 21654, 21655, 
            21678, 21703, 21702, 21679, 26694, 26695, 36774, 31735, 31734, 36775, 
            26742, 26719, 31758, 36799, 36822, 31783, 26718, 26743, 36798, 31759, 
            31782, 36823, 21660, 21667, 21708, 21685, 21690, 21715, 26706, 26701, 
            31740, 36781, 36786, 31747, 26724, 26749, 36804, 31765, 31788, 36829, 
            26754, 26731, 31770, 36811, 36834, 31795, 21666, 21661, 21684, 21709, 
            21714, 21691, 26700, 26707, 36780, 31741, 31746, 36787, 26748, 26725, 
            31764, 36805, 36828, 31789, 26730, 26755, 36810, 31771, 31794, 36835, 
            17310, 17311, 17322, 17317, 17316, 17323, 18030, 18031, 18750, 19471, 
            19470, 18751, 18036, 18043, 19476, 18757, 18762, 19483, 18042, 18037, 
            18756, 19477, 19482, 18763, 22350, 22351, 22356, 22363, 22362, 22357, 
            27390, 27391, 37470, 32431, 32430, 37471, 27402, 27397, 32436, 37477, 
            37482, 32443, 27396, 27403, 37476, 32437, 32442, 37483, 23070, 23071, 
            24510, 23791, 23790, 24511, 28110, 28111, 33150, 38191, 38190, 33151, 
            28830, 29551, 38910, 33871, 34590, 39631, 29550, 28831, 33870, 38911, 
            39630, 34591, 23082, 23077, 23796, 24517, 24522, 23803, 28116, 28123, 
            38196, 33157, 33162, 38203, 29556, 28837, 33876, 38917, 39636, 34597, 
            28842, 29563, 38922, 33883, 34602, 39643, 23076, 23083, 24516, 23797, 
            23802, 24523, 28122, 28117, 33156, 38197, 38202, 33163, 28836, 29557, 
            38916, 33877, 34596, 39637, 29562, 28843, 33882, 38923, 39642, 34603, 
            17334, 17335, 17358, 17383, 17382, 17359, 18054, 18055, 19494, 18775, 
            18774, 19495, 18102, 18079, 18798, 19519, 19542, 18823, 18078, 18103, 
            19518, 18799, 18822, 19543, 22374, 22375, 22422, 22399, 22398, 22423, 
            27414, 27415, 32454, 37495, 37494, 32455, 27438, 27463, 37518, 32479, 
            32502, 37543, 27462, 27439, 32478, 37519, 37542, 32503, 23094, 23095, 
            23814, 24535, 24534, 23815, 28134, 28135, 38214, 33175, 33174, 38215, 
            29574, 28855, 33894, 38935, 39654, 34615, 28854, 29575, 38934, 33895, 
            34614, 39655, 23118, 23143, 24558, 23839, 23862, 24583, 28182, 28159, 
            33198, 38239, 38262, 33223, 28878, 29599, 38958, 33919, 34638, 39679, 
            29622, 28903, 33942, 38983, 39702, 34663, 23142, 23119, 23838, 24559, 
            24582, 23863, 28158, 28183, 38238, 33199, 33222, 38263, 29598, 28879, 
            33918, 38959, 39678, 34639, 28902, 29623, 38982, 33943, 34662, 39703, 
            17340, 17347, 17388, 17365, 17370, 17395, 18066, 18061, 18780, 19501, 
            19506, 18787, 18084, 18109, 19524, 18805, 18828, 19549, 18114, 18091, 
            18810, 19531, 19554, 18835, 22386, 22381, 22404, 22429, 22434, 22411, 
            27420, 27427, 37500, 32461, 32466, 37507, 27468, 27445, 32484, 37525, 
            37548, 32509, 27450, 27475, 37530, 32491, 32514, 37555, 23100, 23107, 
            24540, 23821, 23826, 24547, 28146, 28141, 33180, 38221, 38226, 33187, 
            28860, 29581, 38940, 33901, 34620, 39661, 29586, 28867, 33906, 38947, 
            39666, 34627, 23148, 23125, 23844, 24565, 24588, 23869, 28164, 28189, 
            38244, 33205, 33228, 38269, 29604, 28885, 33924, 38965, 39684, 34645, 
            28908, 29629, 38988, 33949, 34668, 39709, 23130, 23155, 24570, 23851, 
            23874, 24595, 28194, 28171, 33210, 38251, 38274, 33235, 28890, 29611, 
            38970, 33931, 34650, 39691, 29634, 28915, 33954, 38995, 39714, 34675, 
            17346, 17341, 17364, 17389, 17394, 17371, 18060, 18067, 19500, 18781, 
            18786, 19507, 18108, 18085, 18804, 19525, 19548, 18829, 18090, 18115, 
            19530, 18811, 18834, 19555, 22380, 22387, 22428, 22405, 22410, 22435, 
            27426, 27421, 32460, 37501, 37506, 32467, 27444, 27469, 37524, 32485, 
            32508, 37549, 27474, 27451, 32490, 37531, 37554, 32515, 23106, 23101, 
            23820, 24541, 24546, 23827, 28140, 28147, 38220, 33181, 33186, 38227, 
            29580, 28861, 33900, 38941, 39660, 34621, 28866, 29587, 38946, 33907, 
            34626, 39667, 23124, 23149, 24564, 23845, 23868, 24589, 28188, 28165, 
            33204, 38245, 38268, 33229, 28884, 29605, 38964, 33925, 34644, 39685, 
            29628, 28909, 33948, 38989, 39708, 34669, 23154, 23131, 23850, 24571, 
            24594, 23875, 28170, 28195, 38250, 33211, 33234, 38275, 29610, 28891, 
            33930, 38971, 39690, 34651, 28914, 29635, 38994, 33955, 34674, 39715, 
            11554, 11553, 11558, 11565, 11566, 11561, 11576, 11579, 11624, 11601, 
            11602, 11627, 11588, 11583, 11606, 11631, 11636, 11613, 11584, 11591, 
            11632, 11609, 11614, 11639, 12272, 12275, 12284, 12279, 12280, 12287, 
            12994, 12993, 13712, 14433, 14434, 13715, 12998, 13005, 14438, 13719, 
            13724, 14445, 13006, 13001, 13720, 14441, 14446, 13727, 12298, 12297, 
            12320, 12345, 12346, 12323, 13016, 13019, 14456, 13737, 13738, 14459, 
            13064, 13041, 13760, 14481, 14504, 13785, 13042, 13067, 14482, 13763, 
            13786, 14507, 12302, 12309, 12350, 12327, 12332, 12357, 13028, 13023, 
            13742, 14463, 14468, 13749, 13046, 13071, 14486, 13767, 13790, 14511, 
            13076, 13053, 13772, 14493, 14516, 13797, 12310, 12305, 12328, 12353, 
            12358, 12335, 13024, 13031, 14464, 13745, 13750, 14471, 13072, 13049, 
            13768, 14489, 14512, 13793, 13054, 13079, 14494, 13775, 13798, 14519, 
            16592, 16595, 16604, 16599, 16600, 16607, 16618, 16617, 16640, 16665, 
            16666, 16643, 16622, 16629, 16670, 16647, 16652, 16677, 16630, 16625, 
            16648, 16673, 16678, 16655, 21634, 21633, 21638, 21645, 21646, 21641, 
            26672, 26675, 36752, 31713, 31714, 36755, 26684, 26679, 31718, 36759, 
            36764, 31725, 26680, 26687, 36760, 31721, 31726, 36767, 21656, 21659, 
            21704, 21681, 21682, 21707, 26698, 26697, 31736, 36777, 36778, 31739, 
            26720, 26745, 36800, 31761, 31784, 36825, 26746, 26723, 31762, 36803, 
            36826, 31787, 21668, 21663, 21686, 21711, 21716, 21693, 26702, 26709, 
            36782, 31743, 31748, 36789, 26750, 26727, 31766, 36807, 36830, 31791, 
            26732, 26757, 36812, 31773, 31796, 36837, 21664, 21671, 21712, 21689, 
            21694, 21719, 26710, 26705, 31744, 36785, 36790, 31751, 26728, 26753, 
            36808, 31769, 31792, 36833, 26758, 26735, 31774, 36815, 36838, 31799, 
            17314, 17313, 17318, 17325, 17326, 17321, 18032, 18035, 19472, 18753, 
            18754, 19475, 18044, 18039, 18758, 19479, 19484, 18765, 18040, 18047, 
            19480, 18761, 18766, 19487, 22352, 22355, 22364, 22359, 22360, 22367, 
            27394, 27393, 32432, 37473, 37474, 32435, 27398, 27405, 37478, 32439, 
            32444, 37485, 27406, 27401, 32440, 37481, 37486, 32447, 23074, 23073, 
            23792, 24513, 24514, 23795, 28112, 28115, 38192, 33153, 33154, 38195, 
            29552, 28833, 33872, 38913, 39632, 34593, 28834, 29555, 38914, 33875, 
            34594, 39635, 23078, 23085, 24518, 23799, 23804, 24525, 28124, 28119, 
            33158, 38199, 38204, 33165, 28838, 29559, 38918, 33879, 34598, 39639, 
            29564, 28845, 33884, 38925, 39644, 34605, 23086, 23081, 23800, 24521, 
            24526, 23807, 28120, 28127, 38200, 33161, 33166, 38207, 29560, 28841, 
            33880, 38921, 39640, 34601, 28846, 29567, 38926, 33887, 34606, 39647, 
            17336, 17339, 17384, 17361, 17362, 17387, 18058, 18057, 18776, 19497, 
            19498, 18779, 18080, 18105, 19520, 18801, 18824, 19545, 18106, 18083, 
            18802, 19523, 19546, 18827, 22378, 22377, 22400, 22425, 22426, 22403, 
            27416, 27419, 37496, 32457, 32458, 37499, 27464, 27441, 32480, 37521, 
            37544, 32505, 27442, 27467, 37522, 32483, 32506, 37547, 23096, 23099, 
            24536, 23817, 23818, 24539, 28138, 28137, 33176, 38217, 38218, 33179, 
            28856, 29577, 38936, 33897, 34616, 39657, 29578, 28859, 33898, 38939, 
            39658, 34619, 23144, 23121, 23840, 24561, 24584, 23865, 28160, 28185, 
            38240, 33201, 33224, 38265, 29600, 28881, 33920, 38961, 39680, 34641, 
            28904, 29625, 38984, 33945, 34664, 39705, 23122, 23147, 24562, 23843, 
            23866, 24587, 28186, 28163, 33202, 38243, 38266, 33227, 28882, 29603, 
            38962, 33923, 34642, 39683, 29626, 28907, 33946, 38987, 39706, 34667, 
            17348, 17343, 17366, 17391, 17396, 17373, 18062, 18069, 19502, 18783, 
            18788, 19509, 18110, 18087, 18806, 19527, 19550, 18831, 18092, 18117, 
            19532, 18813, 18836, 19557, 22382, 22389, 22430, 22407, 22412, 22437, 
            27428, 27423, 32462, 37503, 37508, 32469, 27446, 27471, 37526, 32487, 
            32510, 37551, 27476, 27453, 32492, 37533, 37556, 32517, 23108, 23103, 
            23822, 24543, 24548, 23829, 28142, 28149, 38222, 33183, 33188, 38229, 
            29582, 28863, 33902, 38943, 39662, 34623, 28868, 29589, 38948, 33909, 
            34628, 39669, 23126, 23151, 24566, 23847, 23870, 24591, 28190, 28167, 
            33206, 38247, 38270, 33231, 28886, 29607, 38966, 33927, 34646, 39687, 
            29630, 28911, 33950, 38991, 39710, 34671, 23156, 23133, 23852, 24573, 
            24596, 23877, 28172, 28197, 38252, 33213, 33236, 38277, 29612, 28893, 
            33932, 38973, 39692, 34653, 28916, 29637, 38996, 33957, 34676, 39717, 
            17344, 17351, 17392, 17369, 17374, 17399, 18070, 18065, 18784, 19505, 
            19510, 18791, 18088, 18113, 19528, 18809, 18832, 19553, 18118, 18095, 
            18814, 19535, 19558, 18839, 22390, 22385, 22408, 22433, 22438, 22415, 
            27424, 27431, 37504, 32465, 32470, 37511, 27472, 27449, 32488, 37529, 
            37552, 32513, 27454, 27479, 37534, 32495, 32518, 37559, 23104, 23111, 
            24544, 23825, 23830, 24551, 28150, 28145, 33184, 38225, 38230, 33191, 
            28864, 29585, 38944, 33905, 34624, 39665, 29590, 28871, 33910, 38951, 
            39670, 34631, 23152, 23129, 23848, 24569, 24592, 23873, 28168, 28193, 
            38248, 33209, 33232, 38273, 29608, 28889, 33928, 38969, 39688, 34649, 
            28912, 29633, 38992, 33953, 34672, 39713, 23134, 23159, 24574, 23855, 
            23878, 24599, 28198, 28175, 33214, 38255, 38278, 33239, 28894, 29615, 
            38974, 33935, 34654, 39695, 29638, 28919, 33958, 38999, 39718, 34679, 
            11552, 11555, 11564, 11559, 11560, 11567, 11578, 11577, 11600, 11625, 
            11626, 11603, 11582, 11589, 11630, 11607, 11612, 11637, 11590, 11585, 
            11608, 11633, 11638, 11615, 12274, 12273, 12278, 12285, 12286, 12281, 
            12992, 12995, 14432, 13713, 13714, 14435, 13004, 12999, 13718, 14439, 
            14444, 13725, 13000, 13007, 14440, 13721, 13726, 14447, 12296, 12299, 
            12344, 12321, 12322, 12347, 13018, 13017, 13736, 14457, 14458, 13739, 
            13040, 13065, 14480, 13761, 13784, 14505, 13066, 13043, 13762, 14483, 
            14506, 13787, 12308, 12303, 12326, 12351, 12356, 12333, 13022, 13029, 
            14462, 13743, 13748, 14469, 13070, 13047, 13766, 14487, 14510, 13791, 
            13052, 13077, 14492, 13773, 13796, 14517, 12304, 12311, 12352, 12329, 
            12334, 12359, 13030, 13025, 13744, 14465, 14470, 13751, 13048, 13073, 
            14488, 13769, 13792, 14513, 13078, 13055, 13774, 14495, 14518, 13799, 
            16594, 16593, 16598, 16605, 16606, 16601, 16616, 16619, 16664, 16641, 
            16642, 16667, 16628, 16623, 16646, 16671, 16676, 16653, 16624, 16631, 
            16672, 16649, 16654, 16679, 21632, 21635, 21644, 21639, 21640, 21647, 
            26674, 26673, 31712, 36753, 36754, 31715, 26678, 26685, 36758, 31719, 
            31724, 36765, 26686, 26681, 31720, 36761, 36766, 31727, 21658, 21657, 
            21680, 21705, 21706, 21683, 26696, 26699, 36776, 31737, 31738, 36779, 
            26744, 26721, 31760, 36801, 36824, 31785, 26722, 26747, 36802, 31763, 
            31786, 36827, 21662, 21669, 21710, 21687, 21692, 21717, 26708, 26703, 
            31742, 36783, 36788, 31749, 26726, 26751, 36806, 31767, 31790, 36831, 
            26756, 26733, 31772, 36813, 36836, 31797, 21670, 21665, 21688, 21713, 
            21718, 21695, 26704, 26711, 36784, 31745, 31750, 36791, 26752, 26729, 
            31768, 36809, 36832, 31793, 26734, 26759, 36814, 31775, 31798, 36839, 
            17312, 17315, 17324, 17319, 17320, 17327, 18034, 18033, 18752, 19473, 
            19474, 18755, 18038, 18045, 19478, 18759, 18764, 19485, 18046, 18041, 
            18760, 19481, 19486, 18767, 22354, 22353, 22358, 22365, 22366, 22361, 
            27392, 27395, 37472, 32433, 32434, 37475, 27404, 27399, 32438, 37479, 
            37484, 32445, 27400, 27407, 37480, 32441, 32446, 37487, 23072, 23075, 
            24512, 23793, 23794, 24515, 28114, 28113, 33152, 38193, 38194, 33155, 
            28832, 29553, 38912, 33873, 34592, 39633, 29554, 28835, 33874, 38915, 
            39634, 34595, 23084, 23079, 23798, 24519, 24524, 23805, 28118, 28125, 
            38198, 33159, 33164, 38205, 29558, 28839, 33878, 38919, 39638, 34599, 
            28844, 29565, 38924, 33885, 34604, 39645, 23080, 23087, 24520, 23801, 
            23806, 24527, 28126, 28121, 33160, 38201, 38206, 33167, 28840, 29561, 
            38920, 33881, 34600, 39641, 29566, 28847, 33886, 38927, 39646, 34607, 
            17338, 17337, 17360, 17385, 17386, 17363, 18056, 18059, 19496, 18777, 
            18778, 19499, 18104, 18081, 18800, 19521, 19544, 18825, 18082, 18107, 
            19522, 18803, 18826, 19547, 22376, 22379, 22424, 22401, 22402, 22427, 
            27418, 27417, 32456, 37497, 37498, 32459, 27440, 27465, 37520, 32481, 
            32504, 37545, 27466, 27443, 32482, 37523, 37546, 32507, 23098, 23097, 
            23816, 24537, 24538, 23819, 28136, 28139, 38216, 33177, 33178, 38219, 
            29576, 28857, 33896, 38937, 39656, 34617, 28858, 29579, 38938, 33899, 
            34618, 39659, 23120, 23145, 24560, 23841, 23864, 24585, 28184, 28161, 
            33200, 38241, 38264, 33225, 28880, 29601, 38960, 33921, 34640, 39681, 
            29624, 28905, 33944, 38985, 39704, 34665, 23146, 23123, 23842, 24563, 
            24586, 23867, 28162, 28187, 38242, 33203, 33226, 38267, 29602, 28883, 
            33922, 38963, 39682, 34643, 28906, 29627, 38986, 33947, 34666, 39707, 
            17342, 17349, 17390, 17367, 17372, 17397, 18068, 18063, 18782, 19503, 
            19508, 18789, 18086, 18111, 19526, 18807, 18830, 19551, 18116, 18093, 
            18812, 19533, 19556, 18837, 22388, 22383, 22406, 22431, 22436, 22413, 
            27422, 27429, 37502, 32463, 32468, 37509, 27470, 27447, 32486, 37527, 
            37550, 32511, 27452, 27477, 37532, 32493, 32516, 37557, 23102, 23109, 
            24542, 23823, 23828, 24549, 28148, 28143, 33182, 38223, 38228, 33189, 
            28862, 29583, 38942, 33903, 34622, 39663, 29588, 28869, 33908, 38949, 
            39668, 34629, 23150, 23127, 23846, 24567, 24590, 23871, 28166, 28191, 
            38246, 33207, 33230, 38271, 29606, 28887, 33926, 38967, 39686, 34647, 
            28910, 29631, 38990, 33951, 34670, 39711, 23132, 23157, 24572, 23853, 
            23876, 24597, 28196, 28173, 33212, 38253, 38276, 33237, 28892, 29613, 
            38972, 33933, 34652, 39693, 29636, 28917, 33956, 38997, 39716, 34677, 
            17350, 17345, 17368, 17393, 17398, 17375, 18064, 18071, 19504, 18785, 
            18790, 19511, 18112, 18089, 18808, 19529, 19552, 18833, 18094, 18119, 
            19534, 18815, 18838, 19559, 22384, 22391, 22432, 22409, 22414, 22439, 
            27430, 27425, 32464, 37505, 37510, 32471, 27448, 27473, 37528, 32489, 
            32512, 37553, 27478, 27455, 32494, 37535, 37558, 32519, 23110, 23105, 
            23824, 24545, 24550, 23831, 28144, 28151, 38224, 33185, 33190, 38231, 
            29584, 28865, 33904, 38945, 39664, 34625, 28870, 29591, 38950, 33911, 
            34630, 39671, 23128, 23153, 24568, 23849, 23872, 24593, 28192, 28169, 
            33208, 38249, 38272, 33233, 28888, 29609, 38968, 33929, 34648, 39689, 
            29632, 28913, 33952, 38993, 39712, 34673, 23158, 23135, 23854, 24575, 
            24598, 23879, 28174, 28199, 38254, 33215, 33238, 38279, 29614, 28895, 
            33934, 38975, 39694, 34655, 28918, 29639, 38998, 33959, 34678, 39719, 
            5880, 5881, 5882, 5885, 5884, 5883, 5886, 5887, 5898, 5893, 
            5892, 5899, 5890, 5889, 5894, 5901, 5902, 5897, 5888, 5891, 
            5900, 5895, 5896, 5903, 6000, 6001, 6004, 6003, 6002, 6005, 
            6120, 6121, 6240, 6361, 6360, 6241, 6122, 6125, 6362, 6243, 
            6244, 6365, 6124, 6123, 6242, 6363, 6364, 6245, 6006, 6007, 
            6012, 6019, 6018, 6013, 6126, 6127, 6366, 6247, 6246, 6367, 
            6138, 6133, 6252, 6373, 6378, 6259, 6132, 6139, 6372, 6253, 
            6258, 6379, 6008, 6011, 6020, 6015, 6016, 6023, 6130, 6129, 
            6248, 6369, 6370, 6251, 6134, 6141, 6374, 6255, 6260, 6381, 
            6142, 6137, 6256, 6377, 6382, 6263, 6010, 6009, 6014, 6021, 
            6022, 6017, 6128, 6131, 6368, 6249, 6250, 6371, 6140, 6135, 
            6254, 6375, 6380, 6261, 6136, 6143, 6376, 6257, 6262, 6383, 
            6600, 6601, 6604, 6603, 6602, 6605, 6606, 6607, 6612, 6619, 
            6618, 6613, 6608, 6611, 6620, 6615, 6616, 6623, 6610, 6609, 
            6614, 6621, 6622, 6617, 7320, 7321, 7322, 7325, 7324, 7323, 
            8040, 8041, 9480, 8761, 8760, 9481, 8044, 8043, 8762, 9483, 
            9484, 8765, 8042, 8045, 9482, 8763, 8764, 9485, 7326, 7327, 
            7338, 7333, 7332, 7339, 8046, 8047, 8766, 9487, 9486, 8767, 
            8052, 8059, 9492, 8773, 8778, 9499, 8058, 8053, 8772, 9493, 
            9498, 8779, 7330, 7329, 7334, 7341, 7342, 7337, 8048, 8051, 
            9488, 8769, 8770, 9491, 8060, 8055, 8774, 9495, 9500, 8781, 
            8056, 8063, 9496, 8777, 8782, 9503, 7328, 7331, 7340, 7335, 
            7336, 7343, 8050, 8049, 8768, 9489, 9490, 8771, 8054, 8061, 
            9494, 8775, 8780, 9501, 8062, 8057, 8776, 9497, 9502, 8783, 
            6720, 6721, 6722, 6725, 6724, 6723, 6840, 6841, 7080, 6961, 
            6960, 7081, 6844, 6843, 6962, 7083, 7084, 6965, 6842, 6845, 
            7082, 6963, 6964, 7085, 7440, 7441, 7444, 7443, 7442, 7445, 
            8160, 8161, 8880, 9601, 9600, 8881, 8162, 8165, 9602, 8883, 
            8884, 9605, 8164, 8163, 8882, 9603, 9604, 8885, 7560, 7561, 
            7680, 7801, 7800, 7681, 8280, 8281, 9720, 9001, 9000, 9721, 
            8520, 8401, 9120, 9841, 9960, 9241, 8400, 8521, 9840, 9121, 
            9240, 9961, 7562, 7565, 7802, 7683, 7684, 7805, 8284, 8283, 
            9002, 9723, 9724, 9005, 8402, 8523, 9842, 9123, 9242, 9963, 
            8524, 8405, 9124, 9845, 9964, 9245, 7564, 7563, 7682, 7803, 
            7804, 7685, 8282, 8285, 9722, 9003, 9004, 9725, 8522, 8403, 
            9122, 9843, 9962, 9243, 8404, 8525, 9844, 9125, 9244, 9965, 
            6726, 6727, 6738, 6733, 6732, 6739, 6846, 6847, 6966, 7087, 
            7086, 6967, 6852, 6859, 7092, 6973, 6978, 7099, 6858, 6853, 
            6972, 7093, 7098, 6979, 7446, 7447, 7452, 7459, 7458, 7453, 
            8166, 8167, 9606, 8887, 8886, 9607, 8178, 8173, 8892, 9613, 
            9618, 8899, 8172, 8179, 9612, 8893, 8898, 9619, 7566, 7567, 
            7806, 7687, 7686, 7807, 8286, 8287, 9006, 9727, 9726, 9007, 
            8406, 8527, 9846, 9127, 9246, 9967, 8526, 8407, 9126, 9847, 
            9966, 9247, 7578, 7573, 7692, 7813, 7818, 7699, 8292, 8299, 
            9732, 9013, 9018, 9739, 8532, 8413, 9132, 9853, 9972, 9253, 
            8418, 8539, 9858, 9139, 9258, 9979, 7572, 7579, 7812, 7693, 
            7698, 7819, 8298, 8293, 9012, 9733, 9738, 9019, 8412, 8533, 
            9852, 9133, 9252, 9973, 8538, 8419, 9138, 9859, 9978, 9259, 
            6730, 6729, 6734, 6741, 6742, 6737, 6848, 6851, 7088, 6969, 
            6970, 7091, 6860, 6855, 6974, 7095, 7100, 6981, 6856, 6863, 
            7096, 6977, 6982, 7103, 7448, 7451, 7460, 7455, 7456, 7463, 
            8170, 8169, 8888, 9609, 9610, 8891, 8174, 8181, 9614, 8895, 
            8900, 9621, 8182, 8177, 8896, 9617, 9622, 8903, 7570, 7569, 
            7688, 7809, 7810, 7691, 8288, 8291, 9728, 9009, 9010, 9731, 
            8528, 8409, 9128, 9849, 9968, 9249, 8410, 8531, 9850, 9131, 
            9250, 9971, 7574, 7581, 7814, 7695, 7700, 7821, 8300, 8295, 
            9014, 9735, 9740, 9021, 8414, 8535, 9854, 9135, 9254, 9975, 
            8540, 8421, 9140, 9861, 9980, 9261, 7582, 7577, 7696, 7817, 
            7822, 7703, 8296, 8303, 9736, 9017, 9022, 9743, 8536, 8417, 
            9136, 9857, 9976, 9257, 8422, 8543, 9862, 9143, 9262, 9983, 
            6728, 6731, 6740, 6735, 6736, 6743, 6850, 6849, 6968, 7089, 
            7090, 6971, 6854, 6861, 7094, 6975, 6980, 7101, 6862, 6857, 
            6976, 7097, 7102, 6983, 7450, 7449, 7454, 7461, 7462, 7457, 
            8168, 8171, 9608, 8889, 8890, 9611, 8180, 8175, 8894, 9615, 
            9620, 8901, 8176, 8183, 9616, 8897, 8902, 9623, 7568, 7571, 
            7808, 7689, 7690, 7811, 8290, 8289, 9008, 9729, 9730, 9011, 
            8408, 8529, 9848, 9129, 9248, 9969, 8530, 8411, 9130, 9851, 
            9970, 9251, 7580, 7575, 7694, 7815, 7820, 7701, 8294, 8301, 
            9734, 9015, 9020, 9741, 8534, 8415, 9134, 9855, 9974, 9255, 
            8420, 8541, 9860, 9141, 9260, 9981, 7576, 7583, 7816, 7697, 
            7702, 7823, 8302, 8297, 9016, 9737, 9742, 9023, 8416, 8537, 
            9856, 9137, 9256, 9977, 8542, 8423, 9142, 9863, 9982, 9263, 
            10920, 10921, 10924, 10923, 10922, 10925, 10926, 10927, 10932, 10939, 
            10938, 10933, 10928, 10931, 10940, 10935, 10936, 10943, 10930, 10929, 
            10934, 10941, 10942, 10937, 11040, 11041, 11042, 11045, 11044, 11043, 
            11160, 11161, 11400, 11281, 11280, 11401, 11164, 11163, 11282, 11403, 
            11404, 11285, 11162, 11165, 11402, 11283, 11284, 11405, 11046, 11047, 
            11058, 11053, 11052, 11059, 11166, 11167, 11286, 11407, 11406, 11287, 
            11172, 11179, 11412, 11293, 11298, 11419, 11178, 11173, 11292, 11413, 
            11418, 11299, 11050, 11049, 11054, 11061, 11062, 11057, 11168, 11171, 
            11408, 11289, 11290, 11411, 11180, 11175, 11294, 11415, 11420, 11301, 
            11176, 11183, 11416, 11297, 11302, 11423, 11048, 11051, 11060, 11055, 
            11056, 11063, 11170, 11169, 11288, 11409, 11410, 11291, 11174, 11181, 
            11414, 11295, 11300, 11421, 11182, 11177, 11296, 11417, 11422, 11303, 
            15960, 15961, 15962, 15965, 15964, 15963, 15966, 15967, 15978, 15973, 
            15972, 15979, 15970, 15969, 15974, 15981, 15982, 15977, 15968, 15971, 
            15980, 15975, 15976, 15983, 21000, 21001, 21004, 21003, 21002, 21005, 
            26040, 26041, 31080, 36121, 36120, 31081, 26042, 26045, 36122, 31083, 
            31084, 36125, 26044, 26043, 31082, 36123, 36124, 31085, 21006, 21007, 
            21012, 21019, 21018, 21013, 26046, 26047, 36126, 31087, 31086, 36127, 
            26058, 26053, 31092, 36133, 36138, 31099, 26052, 26059, 36132, 31093, 
            31098, 36139, 21008, 21011, 21020, 21015, 21016, 21023, 26050, 26049, 
            31088, 36129, 36130, 31091, 26054, 26061, 36134, 31095, 31100, 36141, 
            26062, 26057, 31096, 36137, 36142, 31103, 21010, 21009, 21014, 21021, 
            21022, 21017, 26048, 26051, 36128, 31089, 31090, 36131, 26060, 26055, 
            31094, 36135, 36140, 31101, 26056, 26063, 36136, 31097, 31102, 36143, 
            16080, 16081, 16084, 16083, 16082, 16085, 16200, 16201, 16320, 16441, 
            16440, 16321, 16202, 16205, 16442, 16323, 16324, 16445, 16204, 16203, 
            16322, 16443, 16444, 16325, 21120, 21121, 21122, 21125, 21124, 21123, 
            26160, 26161, 36240, 31201, 31200, 36241, 26164, 26163, 31202, 36243, 
            36244, 31205, 26162, 26165, 36242, 31203, 31204, 36245, 21240, 21241, 
            21480, 21361, 21360, 21481, 26280, 26281, 31320, 36361, 36360, 31321, 
            26400, 26521, 36480, 31441, 31560, 36601, 26520, 26401, 31440, 36481, 
            36600, 31561, 21244, 21243, 21362, 21483, 21484, 21365, 26282, 26285, 
            36362, 31323, 31324, 36365, 26522, 26403, 31442, 36483, 36602, 31563, 
            26404, 26525, 36484, 31445, 31564, 36605, 21242, 21245, 21482, 21363, 
            21364, 21485, 26284, 26283, 31322, 36363, 36364, 31325, 26402, 26523, 
            36482, 31443, 31562, 36603, 26524, 26405, 31444, 36485, 36604, 31565, 
            16086, 16087, 16092, 16099, 16098, 16093, 16206, 16207, 16446, 16327, 
            16326, 16447, 16218, 16213, 16332, 16453, 16458, 16339, 16212, 16219, 
            16452, 16333, 16338, 16459, 21126, 21127, 21138, 21133, 21132, 21139, 
            26166, 26167, 31206, 36247, 36246, 31207, 26172, 26179, 36252, 31213, 
            31218, 36259, 26178, 26173, 31212, 36253, 36258, 31219, 21246, 21247, 
            21366, 21487, 21486, 21367, 26286, 26287, 36366, 31327, 31326, 36367, 
            26526, 26407, 31446, 36487, 36606, 31567, 26406, 26527, 36486, 31447, 
            31566, 36607, 21252, 21259, 21492, 21373, 21378, 21499, 26298, 26293, 
            31332, 36373, 36378, 31339, 26412, 26533, 36492, 31453, 31572, 36613, 
            26538, 26419, 31458, 36499, 36618, 31579, 21258, 21253, 21372, 21493, 
            21498, 21379, 26292, 26299, 36372, 31333, 31338, 36379, 26532, 26413, 
            31452, 36493, 36612, 31573, 26418, 26539, 36498, 31459, 31578, 36619, 
            16088, 16091, 16100, 16095, 16096, 16103, 16210, 16209, 16328, 16449, 
            16450, 16331, 16214, 16221, 16454, 16335, 16340, 16461, 16222, 16217, 
            16336, 16457, 16462, 16343, 21130, 21129, 21134, 21141, 21142, 21137, 
            26168, 26171, 36248, 31209, 31210, 36251, 26180, 26175, 31214, 36255, 
            36260, 31221, 26176, 26183, 36256, 31217, 31222, 36263, 21248, 21251, 
            21488, 21369, 21370, 21491, 26290, 26289, 31328, 36369, 36370, 31331, 
            26408, 26529, 36488, 31449, 31568, 36609, 26530, 26411, 31450, 36491, 
            36610, 31571, 21260, 21255, 21374, 21495, 21500, 21381, 26294, 26301, 
            36374, 31335, 31340, 36381, 26534, 26415, 31454, 36495, 36614, 31575, 
            26420, 26541, 36500, 31461, 31580, 36621, 21256, 21263, 21496, 21377, 
            21382, 21503, 26302, 26297, 31336, 36377, 36382, 31343, 26416, 26537, 
            36496, 31457, 31576, 36617, 26542, 26423, 31462, 36503, 36622, 31583, 
            16090, 16089, 16094, 16101, 16102, 16097, 16208, 16211, 16448, 16329, 
            16330, 16451, 16220, 16215, 16334, 16455, 16460, 16341, 16216, 16223, 
            16456, 16337, 16342, 16463, 21128, 21131, 21140, 21135, 21136, 21143, 
            26170, 26169, 31208, 36249, 36250, 31211, 26174, 26181, 36254, 31215, 
            31220, 36261, 26182, 26177, 31216, 36257, 36262, 31223, 21250, 21249, 
            21368, 21489, 21490, 21371, 26288, 26291, 36368, 31329, 31330, 36371, 
            26528, 26409, 31448, 36489, 36608, 31569, 26410, 26531, 36490, 31451, 
            31570, 36611, 21254, 21261, 21494, 21375, 21380, 21501, 26300, 26295, 
            31334, 36375, 36380, 31341, 26414, 26535, 36494, 31455, 31574, 36615, 
            26540, 26421, 31460, 36501, 36620, 31581, 21262, 21257, 21376, 21497, 
            21502, 21383, 26296, 26303, 36376, 31337, 31342, 36383, 26536, 26417, 
            31456, 36497, 36616, 31577, 26422, 26543, 36502, 31463, 31582, 36623, 
            11640, 11641, 11642, 11645, 11644, 11643, 11646, 11647, 11658, 11653, 
            11652, 11659, 11650, 11649, 11654, 11661, 11662, 11657, 11648, 11651, 
            11660, 11655, 11656, 11663, 12360, 12361, 12364, 12363, 12362, 12365, 
            13080, 13081, 13800, 14521, 14520, 13801, 13082, 13085, 14522, 13803, 
            13804, 14525, 13084, 13083, 13802, 14523, 14524, 13805, 12366, 12367, 
            12372, 12379, 12378, 12373, 13086, 13087, 14526, 13807, 13806, 14527, 
            13098, 13093, 13812, 14533, 14538, 13819, 13092, 13099, 14532, 13813, 
            13818, 14539, 12368, 12371, 12380, 12375, 12376, 12383, 13090, 13089, 
            13808, 14529, 14530, 13811, 13094, 13101, 14534, 13815, 13820, 14541, 
            13102, 13097, 13816, 14537, 14542, 13823, 12370, 12369, 12374, 12381, 
            12382, 12377, 13088, 13091, 14528, 13809, 13810, 14531, 13100, 13095, 
            13814, 14535, 14540, 13821, 13096, 13103, 14536, 13817, 13822, 14543, 
            16680, 16681, 16684, 16683, 16682, 16685, 16686, 16687, 16692, 16699, 
            16698, 16693, 16688, 16691, 16700, 16695, 16696, 16703, 16690, 16689, 
            16694, 16701, 16702, 16697, 21720, 21721, 21722, 21725, 21724, 21723, 
            26760, 26761, 36840, 31801, 31800, 36841, 26764, 26763, 31802, 36843, 
            36844, 31805, 26762, 26765, 36842, 31803, 31804, 36845, 21726, 21727, 
            21738, 21733, 21732, 21739, 26766, 26767, 31806, 36847, 36846, 31807, 
            26772, 26779, 36852, 31813, 31818, 36859, 26778, 26773, 31812, 36853, 
            36858, 31819, 21730, 21729, 21734, 21741, 21742, 21737, 26768, 26771, 
            36848, 31809, 31810, 36851, 26780, 26775, 31814, 36855, 36860, 31821, 
            26776, 26783, 36856, 31817, 31822, 36863, 21728, 21731, 21740, 21735, 
            21736, 21743, 26770, 26769, 31808, 36849, 36850, 31811, 26774, 26781, 
            36854, 31815, 31820, 36861, 26782, 26777, 31816, 36857, 36862, 31823, 
            17400, 17401, 17402, 17405, 17404, 17403, 18120, 18121, 19560, 18841, 
            18840, 19561, 18124, 18123, 18842, 19563, 19564, 18845, 18122, 18125, 
            19562, 18843, 18844, 19565, 22440, 22441, 22444, 22443, 22442, 22445, 
            27480, 27481, 32520, 37561, 37560, 32521, 27482, 27485, 37562, 32523, 
            32524, 37565, 27484, 27483, 32522, 37563, 37564, 32525, 23160, 23161, 
            23880, 24601, 24600, 23881, 28200, 28201, 38280, 33241, 33240, 38281, 
            29640, 28921, 33960, 39001, 39720, 34681, 28920, 29641, 39000, 33961, 
            34680, 39721, 23162, 23165, 24602, 23883, 23884, 24605, 28204, 28203, 
            33242, 38283, 38284, 33245, 28922, 29643, 39002, 33963, 34682, 39723, 
            29644, 28925, 33964, 39005, 39724, 34685, 23164, 23163, 23882, 24603, 
            24604, 23885, 28202, 28205, 38282, 33243, 33244, 38285, 29642, 28923, 
            33962, 39003, 39722, 34683, 28924, 29645, 39004, 33965, 34684, 39725, 
            17406, 17407, 17418, 17413, 17412, 17419, 18126, 18127, 18846, 19567, 
            19566, 18847, 18132, 18139, 19572, 18853, 18858, 19579, 18138, 18133, 
            18852, 19573, 19578, 18859, 22446, 22447, 22452, 22459, 22458, 22453, 
            27486, 27487, 37566, 32527, 32526, 37567, 27498, 27493, 32532, 37573, 
            37578, 32539, 27492, 27499, 37572, 32533, 32538, 37579, 23166, 23167, 
            24606, 23887, 23886, 24607, 28206, 28207, 33246, 38287, 38286, 33247, 
            28926, 29647, 39006, 33967, 34686, 39727, 29646, 28927, 33966, 39007, 
            39726, 34687, 23178, 23173, 23892, 24613, 24618, 23899, 28212, 28219, 
            38292, 33253, 33258, 38299, 29652, 28933, 33972, 39013, 39732, 34693, 
            28938, 29659, 39018, 33979, 34698, 39739, 23172, 23179, 24612, 23893, 
            23898, 24619, 28218, 28213, 33252, 38293, 38298, 33259, 28932, 29653, 
            39012, 33973, 34692, 39733, 29658, 28939, 33978, 39019, 39738, 34699, 
            17410, 17409, 17414, 17421, 17422, 17417, 18128, 18131, 19568, 18849, 
            18850, 19571, 18140, 18135, 18854, 19575, 19580, 18861, 18136, 18143, 
            19576, 18857, 18862, 19583, 22448, 22451, 22460, 22455, 22456, 22463, 
            27490, 27489, 32528, 37569, 37570, 32531, 27494, 27501, 37574, 32535, 
            32540, 37581, 27502, 27497, 32536, 37577, 37582, 32543, 23170, 23169, 
            23888, 24609, 24610, 23891, 28208, 28211, 38288, 33249, 33250, 38291, 
            29648, 28929, 33968, 39009, 39728, 34689, 28930, 29651, 39010, 33971, 
            34690, 39731, 23174, 23181, 24614, 23895, 23900, 24621, 28220, 28215, 
            33254, 38295, 38300, 33261, 28934, 29655, 39014, 33975, 34694, 39735, 
            29660, 28941, 33980, 39021, 39740, 34701, 23182, 23177, 23896, 24617, 
            24622, 23903, 28216, 28223, 38296, 33257, 33262, 38303, 29656, 28937, 
            33976, 39017, 39736, 34697, 28942, 29663, 39022, 33983, 34702, 39743, 
            17408, 17411, 17420, 17415, 17416, 17423, 18130, 18129, 18848, 19569, 
            19570, 18851, 18134, 18141, 19574, 18855, 18860, 19581, 18142, 18137, 
            18856, 19577, 19582, 18863, 22450, 22449, 22454, 22461, 22462, 22457, 
            27488, 27491, 37568, 32529, 32530, 37571, 27500, 27495, 32534, 37575, 
            37580, 32541, 27496, 27503, 37576, 32537, 32542, 37583, 23168, 23171, 
            24608, 23889, 23890, 24611, 28210, 28209, 33248, 38289, 38290, 33251, 
            28928, 29649, 39008, 33969, 34688, 39729, 29650, 28931, 33970, 39011, 
            39730, 34691, 23180, 23175, 23894, 24615, 24620, 23901, 28214, 28221, 
            38294, 33255, 33260, 38301, 29654, 28935, 33974, 39015, 39734, 34695, 
            28940, 29661, 39020, 33981, 34700, 39741, 23176, 23183, 24616, 23897, 
            23902, 24623, 28222, 28217, 33256, 38297, 38302, 33263, 28936, 29657, 
            39016, 33977, 34696, 39737, 29662, 28943, 33982, 39023, 39742, 34703, 
            11760, 11761, 11764, 11763, 11762, 11765, 11880, 11881, 12000, 12121, 
            12120, 12001, 11882, 11885, 12122, 12003, 12004, 12125, 11884, 11883, 
            12002, 12123, 12124, 12005, 12480, 12481, 12482, 12485, 12484, 12483, 
            13200, 13201, 14640, 13921, 13920, 14641, 13204, 13203, 13922, 14643, 
            14644, 13925, 13202, 13205, 14642, 13923, 13924, 14645, 12600, 12601, 
            12840, 12721, 12720, 12841, 13320, 13321, 14040, 14761, 14760, 14041, 
            13440, 13561, 14880, 14161, 14280, 15001, 13560, 13441, 14160, 14881, 
            15000, 14281, 12604, 12603, 12722, 12843, 12844, 12725, 13322, 13325, 
            14762, 14043, 14044, 14765, 13562, 13443, 14162, 14883, 15002, 14283, 
            13444, 13565, 14884, 14165, 14284, 15005, 12602, 12605, 12842, 12723, 
            12724, 12845, 13324, 13323, 14042, 14763, 14764, 14045, 13442, 13563, 
            14882, 14163, 14282, 15003, 13564, 13445, 14164, 14885, 15004, 14285, 
            16800, 16801, 16802, 16805, 16804, 16803, 16920, 16921, 17160, 17041, 
            17040, 17161, 16924, 16923, 17042, 17163, 17164, 17045, 16922, 16925, 
            17162, 17043, 17044, 17165, 21840, 21841, 21844, 21843, 21842, 21845, 
            26880, 26881, 31920, 36961, 36960, 31921, 26882, 26885, 36962, 31923, 
            31924, 36965, 26884, 26883, 31922, 36963, 36964, 31925, 21960, 21961, 
            22080, 22201, 22200, 22081, 27000, 27001, 37080, 32041, 32040, 37081, 
            27240, 27121, 32160, 37201, 37320, 32281, 27120, 27241, 37200, 32161, 
            32280, 37321, 21962, 21965, 22202, 22083, 22084, 22205, 27004, 27003, 
            32042, 37083, 37084, 32045, 27122, 27243, 37202, 32163, 32282, 37323, 
            27244, 27125, 32164, 37205, 37324, 32285, 21964, 21963, 22082, 22203, 
            22204, 22085, 27002, 27005, 37082, 32043, 32044, 37085, 27242, 27123, 
            32162, 37203, 37322, 32283, 27124, 27245, 37204, 32165, 32284, 37325, 
            17520, 17521, 17524, 17523, 17522, 17525, 18240, 18241, 18960, 19681, 
            19680, 18961, 18242, 18245, 19682, 18963, 18964, 19685, 18244, 18243, 
            18962, 19683, 19684, 18965, 22560, 22561, 22562, 22565, 22564, 22563, 
            27600, 27601, 37680, 32641, 32640, 37681, 27604, 27603, 32642, 37683, 
            37684, 32645, 27602, 27605, 37682, 32643, 32644, 37685, 23280, 23281, 
            24720, 24001, 24000, 24721, 28320, 28321, 33360, 38401, 38400, 33361, 
            29040, 29761, 39120, 34081, 34800, 39841, 29760, 29041, 34080, 39121, 
            39840, 34801, 23284, 23283, 24002, 24723, 24724, 24005, 28322, 28325, 
            38402, 33363, 33364, 38405, 29762, 29043, 34082, 39123, 39842, 34803, 
            29044, 29765, 39124, 34085, 34804, 39845, 23282, 23285, 24722, 24003, 
            24004, 24725, 28324, 28323, 33362, 38403, 38404, 33365, 29042, 29763, 
            39122, 34083, 34802, 39843, 29764, 29045, 34084, 39125, 39844, 34805, 
            17640, 17641, 17760, 17881, 17880, 17761, 18360, 18361, 19800, 19081, 
            19080, 19801, 18600, 18481, 19200, 19921, 20040, 19321, 18480, 18601, 
            19920, 19201, 19320, 20041, 22680, 22681, 22920, 22801, 22800, 22921, 
            27720, 27721, 32760, 37801, 37800, 32761, 27840, 27961, 37920, 32881, 
            33000, 38041, 27960, 27841, 32880, 37921, 38040, 33001, 23400, 23401, 
            24120, 24841, 24840, 24121, 28440, 28441, 38520, 33481, 33480, 38521, 
            29880, 29161, 34200, 39241, 39960, 34921, 29160, 29881, 39240, 34201, 
            34920, 39961, 23520, 23641, 24960, 24241, 24360, 25081, 28680, 28561, 
            33600, 38641, 38760, 33721, 29280, 30001, 39360, 34321, 35040, 40081, 
            30120, 29401, 34440, 39481, 40200, 35161, 23640, 23521, 24240, 24961, 
            25080, 24361, 28560, 28681, 38640, 33601, 33720, 38761, 30000, 29281, 
            34320, 39361, 40080, 35041, 29400, 30121, 39480, 34441, 35160, 40201, 
            17642, 17645, 17882, 17763, 17764, 17885, 18364, 18363, 19082, 19803, 
            19804, 19085, 18482, 18603, 19922, 19203, 19322, 20043, 18604, 18485, 
            19204, 19925, 20044, 19325, 22684, 22683, 22802, 22923, 22924, 22805, 
            27722, 27725, 37802, 32763, 32764, 37805, 27962, 27843, 32882, 37923, 
            38042, 33003, 27844, 27965, 37924, 32885, 33004, 38045, 23402, 23405, 
            24842, 24123, 24124, 24845, 28444, 28443, 33482, 38523, 38524, 33485, 
            29162, 29883, 39242, 34203, 34922, 39963, 29884, 29165, 34204, 39245, 
            39964, 34925, 23642, 23523, 24242, 24963, 25082, 24363, 28562, 28683, 
            38642, 33603, 33722, 38763, 30002, 29283, 34322, 39363, 40082, 35043, 
            29402, 30123, 39482, 34443, 35162, 40203, 23524, 23645, 24964, 24245, 
            24364, 25085, 28684, 28565, 33604, 38645, 38764, 33725, 29284, 30005, 
            39364, 34325, 35044, 40085, 30124, 29405, 34444, 39485, 40204, 35165, 
            17644, 17643, 17762, 17883, 17884, 17765, 18362, 18365, 19802, 19083, 
            19084, 19805, 18602, 18483, 19202, 19923, 20042, 19323, 18484, 18605, 
            19924, 19205, 19324, 20045, 22682, 22685, 22922, 22803, 22804, 22925, 
            27724, 27723, 32762, 37803, 37804, 32765, 27842, 27963, 37922, 32883, 
            33002, 38043, 27964, 27845, 32884, 37925, 38044, 33005, 23404, 23403, 
            24122, 24843, 24844, 24125, 28442, 28445, 38522, 33483, 33484, 38525, 
            29882, 29163, 34202, 39243, 39962, 34923, 29164, 29885, 39244, 34205, 
            34924, 39965, 23522, 23643, 24962, 24243, 24362, 25083, 28682, 28563, 
            33602, 38643, 38762, 33723, 29282, 30003, 39362, 34323, 35042, 40083, 
            30122, 29403, 34442, 39483, 40202, 35163, 23644, 23525, 24244, 24965, 
            25084, 24365, 28564, 28685, 38644, 33605, 33724, 38765, 30004, 29285, 
            34324, 39365, 40084, 35045, 29404, 30125, 39484, 34445, 35164, 40205, 
            11766, 11767, 11772, 11779, 11778, 11773, 11886, 11887, 12126, 12007, 
            12006, 12127, 11898, 11893, 12012, 12133, 12138, 12019, 11892, 11899, 
            12132, 12013, 12018, 12139, 12486, 12487, 12498, 12493, 12492, 12499, 
            13206, 13207, 13926, 14647, 14646, 13927, 13212, 13219, 14652, 13933, 
            13938, 14659, 13218, 13213, 13932, 14653, 14658, 13939, 12606, 12607, 
            12726, 12847, 12846, 12727, 13326, 13327, 14766, 14047, 14046, 14767, 
            13566, 13447, 14166, 14887, 15006, 14287, 13446, 13567, 14886, 14167, 
            14286, 15007, 12612, 12619, 12852, 12733, 12738, 12859, 13338, 13333, 
            14052, 14773, 14778, 14059, 13452, 13573, 14892, 14173, 14292, 15013, 
            13578, 13459, 14178, 14899, 15018, 14299, 12618, 12613, 12732, 12853, 
            12858, 12739, 13332, 13339, 14772, 14053, 14058, 14779, 13572, 13453, 
            14172, 14893, 15012, 14293, 13458, 13579, 14898, 14179, 14298, 15019, 
            16806, 16807, 16818, 16813, 16812, 16819, 16926, 16927, 17046, 17167, 
            17166, 17047, 16932, 16939, 17172, 17053, 17058, 17179, 16938, 16933, 
            17052, 17173, 17178, 17059, 21846, 21847, 21852, 21859, 21858, 21853, 
            26886, 26887, 36966, 31927, 31926, 36967, 26898, 26893, 31932, 36973, 
            36978, 31939, 26892, 26899, 36972, 31933, 31938, 36979, 21966, 21967, 
            22206, 22087, 22086, 22207, 27006, 27007, 32046, 37087, 37086, 32047, 
            27126, 27247, 37206, 32167, 32286, 37327, 27246, 27127, 32166, 37207, 
            37326, 32287, 21978, 21973, 22092, 22213, 22218, 22099, 27012, 27019, 
            37092, 32053, 32058, 37099, 27252, 27133, 32172, 37213, 37332, 32293, 
            27138, 27259, 37218, 32179, 32298, 37339, 21972, 21979, 22212, 22093, 
            22098, 22219, 27018, 27013, 32052, 37093, 37098, 32059, 27132, 27253, 
            37212, 32173, 32292, 37333, 27258, 27139, 32178, 37219, 37338, 32299, 
            17526, 17527, 17532, 17539, 17538, 17533, 18246, 18247, 19686, 18967, 
            18966, 19687, 18258, 18253, 18972, 19693, 19698, 18979, 18252, 18259, 
            19692, 18973, 18978, 19699, 22566, 22567, 22578, 22573, 22572, 22579, 
            27606, 27607, 32646, 37687, 37686, 32647, 27612, 27619, 37692, 32653, 
            32658, 37699, 27618, 27613, 32652, 37693, 37698, 32659, 23286, 23287, 
            24006, 24727, 24726, 24007, 28326, 28327, 38406, 33367, 33366, 38407, 
            29766, 29047, 34086, 39127, 39846, 34807, 29046, 29767, 39126, 34087, 
            34806, 39847, 23292, 23299, 24732, 24013, 24018, 24739, 28338, 28333, 
            33372, 38413, 38418, 33379, 29052, 29773, 39132, 34093, 34812, 39853, 
            29778, 29059, 34098, 39139, 39858, 34819, 23298, 23293, 24012, 24733, 
            24738, 24019, 28332, 28339, 38412, 33373, 33378, 38419, 29772, 29053, 
            34092, 39133, 39852, 34813, 29058, 29779, 39138, 34099, 34818, 39859, 
            17646, 17647, 17886, 17767, 17766, 17887, 18366, 18367, 19086, 19807, 
            19806, 19087, 18486, 18607, 19926, 19207, 19326, 20047, 18606, 18487, 
            19206, 19927, 20046, 19327, 22686, 22687, 22806, 22927, 22926, 22807, 
            27726, 27727, 37806, 32767, 32766, 37807, 27966, 27847, 32886, 37927, 
            38046, 33007, 27846, 27967, 37926, 32887, 33006, 38047, 23406, 23407, 
            24846, 24127, 24126, 24847, 28446, 28447, 33486, 38527, 38526, 33487, 
            29166, 29887, 39246, 34207, 34926, 39967, 29886, 29167, 34206, 39247, 
            39966, 34927, 23646, 23527, 24246, 24967, 25086, 24367, 28566, 28687, 
            38646, 33607, 33726, 38767, 30006, 29287, 34326, 39367, 40086, 35047, 
            29406, 30127, 39486, 34447, 35166, 40207, 23526, 23647, 24966, 24247, 
            24366, 25087, 28686, 28567, 33606, 38647, 38766, 33727, 29286, 30007, 
            39366, 34327, 35046, 40087, 30126, 29407, 34446, 39487, 40206, 35167, 
            17658, 17653, 17772, 17893, 17898, 17779, 18372, 18379, 19812, 19093, 
            19098, 19819, 18612, 18493, 19212, 19933, 20052, 19333, 18498, 18619, 
            19938, 19219, 19338, 20059, 22692, 22699, 22932, 22813, 22818, 22939, 
            27738, 27733, 32772, 37813, 37818, 32779, 27852, 27973, 37932, 32893, 
            33012, 38053, 27978, 27859, 32898, 37939, 38058, 33019, 23418, 23413, 
            24132, 24853, 24858, 24139, 28452, 28459, 38532, 33493, 33498, 38539, 
            29892, 29173, 34212, 39253, 39972, 34933, 29178, 29899, 39258, 34219, 
            34938, 39979, 23532, 23653, 24972, 24253, 24372, 25093, 28692, 28573, 
            33612, 38653, 38772, 33733, 29292, 30013, 39372, 34333, 35052, 40093, 
            30132, 29413, 34452, 39493, 40212, 35173, 23658, 23539, 24258, 24979, 
            25098, 24379, 28578, 28699, 38658, 33619, 33738, 38779, 30018, 29299, 
            34338, 39379, 40098, 35059, 29418, 30139, 39498, 34459, 35178, 40219, 
            17652, 17659, 17892, 17773, 17778, 17899, 18378, 18373, 19092, 19813, 
            19818, 19099, 18492, 18613, 19932, 19213, 19332, 20053, 18618, 18499, 
            19218, 19939, 20058, 19339, 22698, 22693, 22812, 22933, 22938, 22819, 
            27732, 27739, 37812, 32773, 32778, 37819, 27972, 27853, 32892, 37933, 
            38052, 33013, 27858, 27979, 37938, 32899, 33018, 38059, 23412, 23419, 
            24852, 24133, 24138, 24859, 28458, 28453, 33492, 38533, 38538, 33499, 
            29172, 29893, 39252, 34213, 34932, 39973, 29898, 29179, 34218, 39259, 
            39978, 34939, 23652, 23533, 24252, 24973, 25092, 24373, 28572, 28693, 
            38652, 33613, 33732, 38773, 30012, 29293, 34332, 39373, 40092, 35053, 
            29412, 30133, 39492, 34453, 35172, 40213, 23538, 23659, 24978, 24259, 
            24378, 25099, 28698, 28579, 33618, 38659, 38778, 33739, 29298, 30019, 
            39378, 34339, 35058, 40099, 30138, 29419, 34458, 39499, 40218, 35179, 
            11768, 11771, 11780, 11775, 11776, 11783, 11890, 11889, 12008, 12129, 
            12130, 12011, 11894, 11901, 12134, 12015, 12020, 12141, 11902, 11897, 
            12016, 12137, 12142, 12023, 12490, 12489, 12494, 12501, 12502, 12497, 
            13208, 13211, 14648, 13929, 13930, 14651, 13220, 13215, 13934, 14655, 
            14660, 13941, 13216, 13223, 14656, 13937, 13942, 14663, 12608, 12611, 
            12848, 12729, 12730, 12851, 13330, 13329, 14048, 14769, 14770, 14051, 
            13448, 13569, 14888, 14169, 14288, 15009, 13570, 13451, 14170, 14891, 
            15010, 14291, 12620, 12615, 12734, 12855, 12860, 12741, 13334, 13341, 
            14774, 14055, 14060, 14781, 13574, 13455, 14174, 14895, 15014, 14295, 
            13460, 13581, 14900, 14181, 14300, 15021, 12616, 12623, 12856, 12737, 
            12742, 12863, 13342, 13337, 14056, 14777, 14782, 14063, 13456, 13577, 
            14896, 14177, 14296, 15017, 13582, 13463, 14182, 14903, 15022, 14303, 
            16810, 16809, 16814, 16821, 16822, 16817, 16928, 16931, 17168, 17049, 
            17050, 17171, 16940, 16935, 17054, 17175, 17180, 17061, 16936, 16943, 
            17176, 17057, 17062, 17183, 21848, 21851, 21860, 21855, 21856, 21863, 
            26890, 26889, 31928, 36969, 36970, 31931, 26894, 26901, 36974, 31935, 
            31940, 36981, 26902, 26897, 31936, 36977, 36982, 31943, 21970, 21969, 
            22088, 22209, 22210, 22091, 27008, 27011, 37088, 32049, 32050, 37091, 
            27248, 27129, 32168, 37209, 37328, 32289, 27130, 27251, 37210, 32171, 
            32290, 37331, 21974, 21981, 22214, 22095, 22100, 22221, 27020, 27015, 
            32054, 37095, 37100, 32061, 27134, 27255, 37214, 32175, 32294, 37335, 
            27260, 27141, 32180, 37221, 37340, 32301, 21982, 21977, 22096, 22217, 
            22222, 22103, 27016, 27023, 37096, 32057, 32062, 37103, 27256, 27137, 
            32176, 37217, 37336, 32297, 27142, 27263, 37222, 32183, 32302, 37343, 
            17528, 17531, 17540, 17535, 17536, 17543, 18250, 18249, 18968, 19689, 
            19690, 18971, 18254, 18261, 19694, 18975, 18980, 19701, 18262, 18257, 
            18976, 19697, 19702, 18983, 22570, 22569, 22574, 22581, 22582, 22577, 
            27608, 27611, 37688, 32649, 32650, 37691, 27620, 27615, 32654, 37695, 
            37700, 32661, 27616, 27623, 37696, 32657, 32662, 37703, 23288, 23291, 
            24728, 24009, 24010, 24731, 28330, 28329, 33368, 38409, 38410, 33371, 
            29048, 29769, 39128, 34089, 34808, 39849, 29770, 29051, 34090, 39131, 
            39850, 34811, 23300, 23295, 24014, 24735, 24740, 24021, 28334, 28341, 
            38414, 33375, 33380, 38421, 29774, 29055, 34094, 39135, 39854, 34815, 
            29060, 29781, 39140, 34101, 34820, 39861, 23296, 23303, 24736, 24017, 
            24022, 24743, 28342, 28337, 33376, 38417, 38422, 33383, 29056, 29777, 
            39136, 34097, 34816, 39857, 29782, 29063, 34102, 39143, 39862, 34823, 
            17650, 17649, 17768, 17889, 17890, 17771, 18368, 18371, 19808, 19089, 
            19090, 19811, 18608, 18489, 19208, 19929, 20048, 19329, 18490, 18611, 
            19930, 19211, 19330, 20051, 22688, 22691, 22928, 22809, 22810, 22931, 
            27730, 27729, 32768, 37809, 37810, 32771, 27848, 27969, 37928, 32889, 
            33008, 38049, 27970, 27851, 32890, 37931, 38050, 33011, 23410, 23409, 
            24128, 24849, 24850, 24131, 28448, 28451, 38528, 33489, 33490, 38531, 
            29888, 29169, 34208, 39249, 39968, 34929, 29170, 29891, 39250, 34211, 
            34930, 39971, 23528, 23649, 24968, 24249, 24368, 25089, 28688, 28569, 
            33608, 38649, 38768, 33729, 29288, 30009, 39368, 34329, 35048, 40089, 
            30128, 29409, 34448, 39489, 40208, 35169, 23650, 23531, 24250, 24971, 
            25090, 24371, 28570, 28691, 38650, 33611, 33730, 38771, 30010, 29291, 
            34330, 39371, 40090, 35051, 29410, 30131, 39490, 34451, 35170, 40211, 
            17654, 17661, 17894, 17775, 17780, 17901, 18380, 18375, 19094, 19815, 
            19820, 19101, 18494, 18615, 19934, 19215, 19334, 20055, 18620, 18501, 
            19220, 19941, 20060, 19341, 22700, 22695, 22814, 22935, 22940, 22821, 
            27734, 27741, 37814, 32775, 32780, 37821, 27974, 27855, 32894, 37935, 
            38054, 33015, 27860, 27981, 37940, 32901, 33020, 38061, 23414, 23421, 
            24854, 24135, 24140, 24861, 28460, 28455, 33494, 38535, 38540, 33501, 
            29174, 29895, 39254, 34215, 34934, 39975, 29900, 29181, 34220, 39261, 
            39980, 34941, 23654, 23535, 24254, 24975, 25094, 24375, 28574, 28695, 
            38654, 33615, 33734, 38775, 30014, 29295, 34334, 39375, 40094, 35055, 
            29414, 30135, 39494, 34455, 35174, 40215, 23540, 23661, 24980, 24261, 
            24380, 25101, 28700, 28581, 33620, 38661, 38780, 33741, 29300, 30021, 
            39380, 34341, 35060, 40101, 30140, 29421, 34460, 39501, 40220, 35181, 
            17662, 17657, 17776, 17897, 17902, 17783, 18376, 18383, 19816, 19097, 
            19102, 19823, 18616, 18497, 19216, 19937, 20056, 19337, 18502, 18623, 
            19942, 19223, 19342, 20063, 22696, 22703, 22936, 22817, 22822, 22943, 
            27742, 27737, 32776, 37817, 37822, 32783, 27856, 27977, 37936, 32897, 
            33016, 38057, 27982, 27863, 32902, 37943, 38062, 33023, 23422, 23417, 
            24136, 24857, 24862, 24143, 28456, 28463, 38536, 33497, 33502, 38543, 
            29896, 29177, 34216, 39257, 39976, 34937, 29182, 29903, 39262, 34223, 
            34942, 39983, 23536, 23657, 24976, 24257, 24376, 25097, 28696, 28577, 
            33616, 38657, 38776, 33737, 29296, 30017, 39376, 34337, 35056, 40097, 
            30136, 29417, 34456, 39497, 40216, 35177, 23662, 23543, 24262, 24983, 
            25102, 24383, 28582, 28703, 38662, 33623, 33742, 38783, 30022, 29303, 
            34342, 39383, 40102, 35063, 29422, 30143, 39502, 34463, 35182, 40223, 
            11770, 11769, 11774, 11781, 11782, 11777, 11888, 11891, 12128, 12009, 
            12010, 12131, 11900, 11895, 12014, 12135, 12140, 12021, 11896, 11903, 
            12136, 12017, 12022, 12143, 12488, 12491, 12500, 12495, 12496, 12503, 
            13210, 13209, 13928, 14649, 14650, 13931, 13214, 13221, 14654, 13935, 
            13940, 14661, 13222, 13217, 13936, 14657, 14662, 13943, 12610, 12609, 
            12728, 12849, 12850, 12731, 13328, 13331, 14768, 14049, 14050, 14771, 
            13568, 13449, 14168, 14889, 15008, 14289, 13450, 13571, 14890, 14171, 
            14290, 15011, 12614, 12621, 12854, 12735, 12740, 12861, 13340, 13335, 
            14054, 14775, 14780, 14061, 13454, 13575, 14894, 14175, 14294, 15015, 
            13580, 13461, 14180, 14901, 15020, 14301, 12622, 12617, 12736, 12857, 
            12862, 12743, 13336, 13343, 14776, 14057, 14062, 14783, 13576, 13457, 
            14176, 14897, 15016, 14297, 13462, 13583, 14902, 14183, 14302, 15023, 
            16808, 16811, 16820, 16815, 16816, 16823, 16930, 16929, 17048, 17169, 
            17170, 17051, 16934, 16941, 17174, 17055, 17060, 17181, 16942, 16937, 
            17056, 17177, 17182, 17063, 21850, 21849, 21854, 21861, 21862, 21857, 
            26888, 26891, 36968, 31929, 31930, 36971, 26900, 26895, 31934, 36975, 
            36980, 31941, 26896, 26903, 36976, 31937, 31942, 36983, 21968, 21971, 
            22208, 22089, 22090, 22211, 27010, 27009, 32048, 37089, 37090, 32051, 
            27128, 27249, 37208, 32169, 32288, 37329, 27250, 27131, 32170, 37211, 
            37330, 32291, 21980, 21975, 22094, 22215, 22220, 22101, 27014, 27021, 
            37094, 32055, 32060, 37101, 27254, 27135, 32174, 37215, 37334, 32295, 
            27140, 27261, 37220, 32181, 32300, 37341, 21976, 21983, 22216, 22097, 
            22102, 22223, 27022, 27017, 32056, 37097, 37102, 32063, 27136, 27257, 
            37216, 32177, 32296, 37337, 27262, 27143, 32182, 37223, 37342, 32303, 
            17530, 17529, 17534, 17541, 17542, 17537, 18248, 18251, 19688, 18969, 
            18970, 19691, 18260, 18255, 18974, 19695, 19700, 18981, 18256, 18263, 
            19696, 18977, 18982, 19703, 22568, 22571, 22580, 22575, 22576, 22583, 
            27610, 27609, 32648, 37689, 37690, 32651, 27614, 27621, 37694, 32655, 
            32660, 37701, 27622, 27617, 32656, 37697, 37702, 32663, 23290, 23289, 
            24008, 24729, 24730, 24011, 28328, 28331, 38408, 33369, 33370, 38411, 
            29768, 29049, 34088, 39129, 39848, 34809, 29050, 29771, 39130, 34091, 
            34810, 39851, 23294, 23301, 24734, 24015, 24020, 24741, 28340, 28335, 
            33374, 38415, 38420, 33381, 29054, 29775, 39134, 34095, 34814, 39855, 
            29780, 29061, 34100, 39141, 39860, 34821, 23302, 23297, 24016, 24737, 
            24742, 24023, 28336, 28343, 38416, 33377, 33382, 38423, 29776, 29057, 
            34096, 39137, 39856, 34817, 29062, 29783, 39142, 34103, 34822, 39863, 
            17648, 17651, 17888, 17769, 17770, 17891, 18370, 18369, 19088, 19809, 
            19810, 19091, 18488, 18609, 19928, 19209, 19328, 20049, 18610, 18491, 
            19210, 19931, 20050, 19331, 22690, 22689, 22808, 22929, 22930, 22811, 
            27728, 27731, 37808, 32769, 32770, 37811, 27968, 27849, 32888, 37929, 
            38048, 33009, 27850, 27971, 37930, 32891, 33010, 38051, 23408, 23411, 
            24848, 24129, 24130, 24851, 28450, 28449, 33488, 38529, 38530, 33491, 
            29168, 29889, 39248, 34209, 34928, 39969, 29890, 29171, 34210, 39251, 
            39970, 34931, 23648, 23529, 24248, 24969, 25088, 24369, 28568, 28689, 
            38648, 33609, 33728, 38769, 30008, 29289, 34328, 39369, 40088, 35049, 
            29408, 30129, 39488, 34449, 35168, 40209, 23530, 23651, 24970, 24251, 
            24370, 25091, 28690, 28571, 33610, 38651, 38770, 33731, 29290, 30011, 
            39370, 34331, 35050, 40091, 30130, 29411, 34450, 39491, 40210, 35171, 
            17660, 17655, 17774, 17895, 17900, 17781, 18374, 18381, 19814, 19095, 
            19100, 19821, 18614, 18495, 19214, 19935, 20054, 19335, 18500, 18621, 
            19940, 19221, 19340, 20061, 22694, 22701, 22934, 22815, 22820, 22941, 
            27740, 27735, 32774, 37815, 37820, 32781, 27854, 27975, 37934, 32895, 
            33014, 38055, 27980, 27861, 32900, 37941, 38060, 33021, 23420, 23415, 
            24134, 24855, 24860, 24141, 28454, 28461, 38534, 33495, 33500, 38541, 
            29894, 29175, 34214, 39255, 39974, 34935, 29180, 29901, 39260, 34221, 
            34940, 39981, 23534, 23655, 24974, 24255, 24374, 25095, 28694, 28575, 
            33614, 38655, 38774, 33735, 29294, 30015, 39374, 34335, 35054, 40095, 
            30134, 29415, 34454, 39495, 40214, 35175, 23660, 23541, 24260, 24981, 
            25100, 24381, 28580, 28701, 38660, 33621, 33740, 38781, 30020, 29301, 
            34340, 39381, 40100, 35061, 29420, 30141, 39500, 34461, 35180, 40221, 
            17656, 17663, 17896, 17777, 17782, 17903, 18382, 18377, 19096, 19817, 
            19822, 19103, 18496, 18617, 19936, 19217, 19336, 20057, 18622, 18503, 
            19222, 19943, 20062, 19343, 22702, 22697, 22816, 22937, 22942, 22823, 
            27736, 27743, 37816, 32777, 32782, 37823, 27976, 27857, 32896, 37937, 
            38056, 33017, 27862, 27983, 37942, 32903, 33022, 38063, 23416, 23423, 
            24856, 24137, 24142, 24863, 28462, 28457, 33496, 38537, 38542, 33503, 
            29176, 29897, 39256, 34217, 34936, 39977, 29902, 29183, 34222, 39263, 
            39982, 34943, 23656, 23537, 24256, 24977, 25096, 24377, 28576, 28697, 
            38656, 33617, 33736, 38777, 30016, 29297, 34336, 39377, 40096, 35057, 
            29416, 30137, 39496, 34457, 35176, 40217, 23542, 23663, 24982, 24263, 
            24382, 25103, 28702, 28583, 33622, 38663, 38782, 33743, 29302, 30023, 
            39382, 34343, 35062, 40103, 30142, 29423, 34462, 39503, 40222, 35183, 
            5904, 5905, 5908, 5907, 5906, 5909, 5928, 5929, 5952, 5977, 
            5976, 5953, 5930, 5933, 5978, 5955, 5956, 5981, 5932, 5931, 
            5954, 5979, 5980, 5957, 6024, 6025, 6026, 6029, 6028, 6027, 
            6144, 6145, 6384, 6265, 6264, 6385, 6148, 6147, 6266, 6387, 
            6388, 6269, 6146, 6149, 6386, 6267, 6268, 6389, 6048, 6049, 
            6096, 6073, 6072, 6097, 6168, 6169, 6288, 6409, 6408, 6289, 
            6192, 6217, 6432, 6313, 6336, 6457, 6216, 6193, 6312, 6433, 
            6456, 6337, 6052, 6051, 6074, 6099, 6100, 6077, 6170, 6173, 
            6410, 6291, 6292, 6413, 6218, 6195, 6314, 6435, 6458, 6339, 
            6196, 6221, 6436, 6317, 6340, 6461, 6050, 6053, 6098, 6075, 
            6076, 6101, 6172, 6171, 6290, 6411, 6412, 6293, 6194, 6219, 
            6434, 6315, 6338, 6459, 6220, 6197, 6316, 6437, 6460, 6341, 
            6624, 6625, 6626, 6629, 6628, 6627, 6648, 6649, 6696, 6673, 
            6672, 6697, 6652, 6651, 6674, 6699, 6700, 6677, 6650, 6653, 
            6698, 6675, 6676, 6701, 7344, 7345, 7348, 7347, 7346, 7349, 
            8064, 8065, 8784, 9505, 9504, 8785, 8066, 8069, 9506, 8787, 
            8788, 9509, 8068, 8067, 8786, 9507, 9508, 8789, 7368, 7369, 
            7392, 7417, 7416, 7393, 8088, 8089, 9528, 8809, 8808, 9529, 
            8136, 8113, 8832, 9553, 9576, 8857, 8112, 8137, 9552, 8833, 
            8856, 9577, 7370, 7373, 7418, 7395, 7396, 7421, 8092, 8091, 
            8810, 9531, 9532, 8813, 8114, 8139, 9554, 8835, 8858, 9579, 
            8140, 8117, 8836, 9557, 9580, 8861, 7372, 7371, 7394, 7419, 
            7420, 7397, 8090, 8093, 9530, 8811, 8812, 9533, 8138, 8115, 
            8834, 9555, 9578, 8859, 8116, 8141, 9556, 8837, 8860, 9581, 
            6744, 6745, 6748, 6747, 6746, 6749, 6864, 6865, 6984, 7105, 
            7104, 6985, 6866, 6869, 7106, 6987, 6988, 7109, 6868, 6867, 
            6986, 7107, 7108, 6989, 7464, 7465, 7466, 7469, 7468, 7467, 
            8184, 8185, 9624, 8905, 8904, 9625, 8188, 8187, 8906, 9627, 
            9628, 8909, 8186, 8189, 9626, 8907, 8908, 9629, 7584, 7585, 
            7824, 7705, 7704, 7825, 8304, 8305, 9024, 9745, 9744, 9025, 
            8424, 8545, 9864, 9145, 9264, 9985, 8544, 8425, 9144, 9865, 
            9984, 9265, 7588, 7587, 7706, 7827, 7828, 7709, 8306, 8309, 
            9746, 9027, 9028, 9749, 8546, 8427, 9146, 9867, 9986, 9267, 
            8428, 8549, 9868, 9149, 9268, 9989, 7586, 7589, 7826, 7707, 
            7708, 7829, 8308, 8307, 9026, 9747, 9748, 9029, 8426, 8547, 
            9866, 9147, 9266, 9987, 8548, 8429, 9148, 9869, 9988, 9269, 
            6768, 6769, 6792, 6817, 6816, 6793, 6888, 6889, 7128, 7009, 
            7008, 7129, 6936, 6913, 7032, 7153, 7176, 7057, 6912, 6937, 
            7152, 7033, 7056, 7177, 7488, 7489, 7536, 7513, 7512, 7537, 
            8208, 8209, 8928, 9649, 9648, 8929, 8232, 8257, 9672, 8953, 
            8976, 9697, 8256, 8233, 8952, 9673, 9696, 8977, 7608, 7609, 
            7728, 7849, 7848, 7729, 8328, 8329, 9768, 9049, 9048, 9769, 
            8568, 8449, 9168, 9889, 10008, 9289, 8448, 8569, 9888, 9169, 
            9288, 10009, 7632, 7657, 7872, 7753, 7776, 7897, 8376, 8353, 
            9072, 9793, 9816, 9097, 8472, 8593, 9912, 9193, 9312, 10033, 
            8616, 8497, 9216, 9937, 10056, 9337, 7656, 7633, 7752, 7873, 
            7896, 7777, 8352, 8377, 9792, 9073, 9096, 9817, 8592, 8473, 
            9192, 9913, 10032, 9313, 8496, 8617, 9936, 9217, 9336, 10057, 
            6770, 6773, 6818, 6795, 6796, 6821, 6892, 6891, 7010, 7131, 
            7132, 7013, 6914, 6939, 7154, 7035, 7058, 7179, 6940, 6917, 
            7036, 7157, 7180, 7061, 7492, 7491, 7514, 7539, 7540, 7517, 
            8210, 8213, 9650, 8931, 8932, 9653, 8258, 8235, 8954, 9675, 
            9698, 8979, 8236, 8261, 9676, 8957, 8980, 9701, 7610, 7613, 
            7850, 7731, 7732, 7853, 8332, 8331, 9050, 9771, 9772, 9053, 
            8450, 8571, 9890, 9171, 9290, 10011, 8572, 8453, 9172, 9893, 
            10012, 9293, 7658, 7635, 7754, 7875, 7898, 7779, 8354, 8379, 
            9794, 9075, 9098, 9819, 8594, 8475, 9194, 9915, 10034, 9315, 
            8498, 8619, 9938, 9219, 9338, 10059, 7636, 7661, 7876, 7757, 
            7780, 7901, 8380, 8357, 9076, 9797, 9820, 9101, 8476, 8597, 
            9916, 9197, 9316, 10037, 8620, 8501, 9220, 9941, 10060, 9341, 
            6772, 6771, 6794, 6819, 6820, 6797, 6890, 6893, 7130, 7011, 
            7012, 7133, 6938, 6915, 7034, 7155, 7178, 7059, 6916, 6941, 
            7156, 7037, 7060, 7181, 7490, 7493, 7538, 7515, 7516, 7541, 
            8212, 8211, 8930, 9651, 9652, 8933, 8234, 8259, 9674, 8955, 
            8978, 9699, 8260, 8237, 8956, 9677, 9700, 8981, 7612, 7611, 
            7730, 7851, 7852, 7733, 8330, 8333, 9770, 9051, 9052, 9773, 
            8570, 8451, 9170, 9891, 10010, 9291, 8452, 8573, 9892, 9173, 
            9292, 10013, 7634, 7659, 7874, 7755, 7778, 7899, 8378, 8355, 
            9074, 9795, 9818, 9099, 8474, 8595, 9914, 9195, 9314, 10035, 
            8618, 8499, 9218, 9939, 10058, 9339, 7660, 7637, 7756, 7877, 
            7900, 7781, 8356, 8381, 9796, 9077, 9100, 9821, 8596, 8477, 
            9196, 9917, 10036, 9317, 8500, 8621, 9940, 9221, 9340, 10061, 
            10944, 10945, 10946, 10949, 10948, 10947, 10968, 10969, 11016, 10993, 
            10992, 11017, 10972, 10971, 10994, 11019, 11020, 10997, 10970, 10973, 
            11018, 10995, 10996, 11021, 11064, 11065, 11068, 11067, 11066, 11069, 
            11184, 11185, 11304, 11425, 11424, 11305, 11186, 11189, 11426, 11307, 
            11308, 11429, 11188, 11187, 11306, 11427, 11428, 11309, 11088, 11089, 
            11112, 11137, 11136, 11113, 11208, 11209, 11448, 11329, 11328, 11449, 
            11256, 11233, 11352, 11473, 11496, 11377, 11232, 11257, 11472, 11353, 
            11376, 11497, 11090, 11093, 11138, 11115, 11116, 11141, 11212, 11211, 
            11330, 11451, 11452, 11333, 11234, 11259, 11474, 11355, 11378, 11499, 
            11260, 11237, 11356, 11477, 11500, 11381, 11092, 11091, 11114, 11139, 
            11140, 11117, 11210, 11213, 11450, 11331, 11332, 11453, 11258, 11235, 
            11354, 11475, 11498, 11379, 11236, 11261, 11476, 11357, 11380, 11501, 
            15984, 15985, 15988, 15987, 15986, 15989, 16008, 16009, 16032, 16057, 
            16056, 16033, 16010, 16013, 16058, 16035, 16036, 16061, 16012, 16011, 
            16034, 16059, 16060, 16037, 21024, 21025, 21026, 21029, 21028, 21027, 
            26064, 26065, 36144, 31105, 31104, 36145, 26068, 26067, 31106, 36147, 
            36148, 31109, 26066, 26069, 36146, 31107, 31108, 36149, 21048, 21049, 
            21096, 21073, 21072, 21097, 26088, 26089, 31128, 36169, 36168, 31129, 
            26112, 26137, 36192, 31153, 31176, 36217, 26136, 26113, 31152, 36193, 
            36216, 31177, 21052, 21051, 21074, 21099, 21100, 21077, 26090, 26093, 
            36170, 31131, 31132, 36173, 26138, 26115, 31154, 36195, 36218, 31179, 
            26116, 26141, 36196, 31157, 31180, 36221, 21050, 21053, 21098, 21075, 
            21076, 21101, 26092, 26091, 31130, 36171, 36172, 31133, 26114, 26139, 
            36194, 31155, 31178, 36219, 26140, 26117, 31156, 36197, 36220, 31181, 
            16104, 16105, 16106, 16109, 16108, 16107, 16224, 16225, 16464, 16345, 
            16344, 16465, 16228, 16227, 16346, 16467, 16468, 16349, 16226, 16229, 
            16466, 16347, 16348, 16469, 21144, 21145, 21148, 21147, 21146, 21149, 
            26184, 26185, 31224, 36265, 36264, 31225, 26186, 26189, 36266, 31227, 
            31228, 36269, 26188, 26187, 31226, 36267, 36268, 31229, 21264, 21265, 
            21384, 21505, 21504, 21385, 26304, 26305, 36384, 31345, 31344, 36385, 
            26544, 26425, 31464, 36505, 36624, 31585, 26424, 26545, 36504, 31465, 
            31584, 36625, 21266, 21269, 21506, 21387, 21388, 21509, 26308, 26307, 
            31346, 36387, 36388, 31349, 26426, 26547, 36506, 31467, 31586, 36627, 
            26548, 26429, 31468, 36509, 36628, 31589, 21268, 21267, 21386, 21507, 
            21508, 21389, 26306, 26309, 36386, 31347, 31348, 36389, 26546, 26427, 
            31466, 36507, 36626, 31587, 26428, 26549, 36508, 31469, 31588, 36629, 
            16128, 16129, 16176, 16153, 16152, 16177, 16248, 16249, 16368, 16489, 
            16488, 16369, 16272, 16297, 16512, 16393, 16416, 16537, 16296, 16273, 
            16392, 16513, 16536, 16417, 21168, 21169, 21192, 21217, 21216, 21193, 
            26208, 26209, 36288, 31249, 31248, 36289, 26256, 26233, 31272, 36313, 
            36336, 31297, 26232, 26257, 36312, 31273, 31296, 36337, 21288, 21289, 
            21528, 21409, 21408, 21529, 26328, 26329, 31368, 36409, 36408, 31369, 
            26448, 26569, 36528, 31489, 31608, 36649, 26568, 26449, 31488, 36529, 
            36648, 31609, 21336, 21313, 21432, 21553, 21576, 21457, 26352, 26377, 
            36432, 31393, 31416, 36457, 26592, 26473, 31512, 36553, 36672, 31633, 
            26496, 26617, 36576, 31537, 31656, 36697, 21312, 21337, 21552, 21433, 
            21456, 21577, 26376, 26353, 31392, 36433, 36456, 31417, 26472, 26593, 
            36552, 31513, 31632, 36673, 26616, 26497, 31536, 36577, 36696, 31657, 
            16132, 16131, 16154, 16179, 16180, 16157, 16250, 16253, 16490, 16371, 
            16372, 16493, 16298, 16275, 16394, 16515, 16538, 16419, 16276, 16301, 
            16516, 16397, 16420, 16541, 21170, 21173, 21218, 21195, 21196, 21221, 
            26212, 26211, 31250, 36291, 36292, 31253, 26234, 26259, 36314, 31275, 
            31298, 36339, 26260, 26237, 31276, 36317, 36340, 31301, 21292, 21291, 
            21410, 21531, 21532, 21413, 26330, 26333, 36410, 31371, 31372, 36413, 
            26570, 26451, 31490, 36531, 36650, 31611, 26452, 26573, 36532, 31493, 
            31612, 36653, 21314, 21339, 21554, 21435, 21458, 21579, 26378, 26355, 
            31394, 36435, 36458, 31419, 26474, 26595, 36554, 31515, 31634, 36675, 
            26618, 26499, 31538, 36579, 36698, 31659, 21340, 21317, 21436, 21557, 
            21580, 21461, 26356, 26381, 36436, 31397, 31420, 36461, 26596, 26477, 
            31516, 36557, 36676, 31637, 26500, 26621, 36580, 31541, 31660, 36701, 
            16130, 16133, 16178, 16155, 16156, 16181, 16252, 16251, 16370, 16491, 
            16492, 16373, 16274, 16299, 16514, 16395, 16418, 16539, 16300, 16277, 
            16396, 16517, 16540, 16421, 21172, 21171, 21194, 21219, 21220, 21197, 
            26210, 26213, 36290, 31251, 31252, 36293, 26258, 26235, 31274, 36315, 
            36338, 31299, 26236, 26261, 36316, 31277, 31300, 36341, 21290, 21293, 
            21530, 21411, 21412, 21533, 26332, 26331, 31370, 36411, 36412, 31373, 
            26450, 26571, 36530, 31491, 31610, 36651, 26572, 26453, 31492, 36533, 
            36652, 31613, 21338, 21315, 21434, 21555, 21578, 21459, 26354, 26379, 
            36434, 31395, 31418, 36459, 26594, 26475, 31514, 36555, 36674, 31635, 
            26498, 26619, 36578, 31539, 31658, 36699, 21316, 21341, 21556, 21437, 
            21460, 21581, 26380, 26357, 31396, 36437, 36460, 31421, 26476, 26597, 
            36556, 31517, 31636, 36677, 26620, 26501, 31540, 36581, 36700, 31661, 
            11664, 11665, 11668, 11667, 11666, 11669, 11688, 11689, 11712, 11737, 
            11736, 11713, 11690, 11693, 11738, 11715, 11716, 11741, 11692, 11691, 
            11714, 11739, 11740, 11717, 12384, 12385, 12386, 12389, 12388, 12387, 
            13104, 13105, 14544, 13825, 13824, 14545, 13108, 13107, 13826, 14547, 
            14548, 13829, 13106, 13109, 14546, 13827, 13828, 14549, 12408, 12409, 
            12456, 12433, 12432, 12457, 13128, 13129, 13848, 14569, 14568, 13849, 
            13152, 13177, 14592, 13873, 13896, 14617, 13176, 13153, 13872, 14593, 
            14616, 13897, 12412, 12411, 12434, 12459, 12460, 12437, 13130, 13133, 
            14570, 13851, 13852, 14573, 13178, 13155, 13874, 14595, 14618, 13899, 
            13156, 13181, 14596, 13877, 13900, 14621, 12410, 12413, 12458, 12435, 
            12436, 12461, 13132, 13131, 13850, 14571, 14572, 13853, 13154, 13179, 
            14594, 13875, 13898, 14619, 13180, 13157, 13876, 14597, 14620, 13901, 
            16704, 16705, 16706, 16709, 16708, 16707, 16728, 16729, 16776, 16753, 
            16752, 16777, 16732, 16731, 16754, 16779, 16780, 16757, 16730, 16733, 
            16778, 16755, 16756, 16781, 21744, 21745, 21748, 21747, 21746, 21749, 
            26784, 26785, 31824, 36865, 36864, 31825, 26786, 26789, 36866, 31827, 
            31828, 36869, 26788, 26787, 31826, 36867, 36868, 31829, 21768, 21769, 
            21792, 21817, 21816, 21793, 26808, 26809, 36888, 31849, 31848, 36889, 
            26856, 26833, 31872, 36913, 36936, 31897, 26832, 26857, 36912, 31873, 
            31896, 36937, 21770, 21773, 21818, 21795, 21796, 21821, 26812, 26811, 
            31850, 36891, 36892, 31853, 26834, 26859, 36914, 31875, 31898, 36939, 
            26860, 26837, 31876, 36917, 36940, 31901, 21772, 21771, 21794, 21819, 
            21820, 21797, 26810, 26813, 36890, 31851, 31852, 36893, 26858, 26835, 
            31874, 36915, 36938, 31899, 26836, 26861, 36916, 31877, 31900, 36941, 
            17424, 17425, 17428, 17427, 17426, 17429, 18144, 18145, 18864, 19585, 
            19584, 18865, 18146, 18149, 19586, 18867, 18868, 19589, 18148, 18147, 
            18866, 19587, 19588, 18869, 22464, 22465, 22466, 22469, 22468, 22467, 
            27504, 27505, 37584, 32545, 32544, 37585, 27508, 27507, 32546, 37587, 
            37588, 32549, 27506, 27509, 37586, 32547, 32548, 37589, 23184, 23185, 
            24624, 23905, 23904, 24625, 28224, 28225, 33264, 38305, 38304, 33265, 
            28944, 29665, 39024, 33985, 34704, 39745, 29664, 28945, 33984, 39025, 
            39744, 34705, 23188, 23187, 23906, 24627, 24628, 23909, 28226, 28229, 
            38306, 33267, 33268, 38309, 29666, 28947, 33986, 39027, 39746, 34707, 
            28948, 29669, 39028, 33989, 34708, 39749, 23186, 23189, 24626, 23907, 
            23908, 24629, 28228, 28227, 33266, 38307, 38308, 33269, 28946, 29667, 
            39026, 33987, 34706, 39747, 29668, 28949, 33988, 39029, 39748, 34709, 
            17448, 17449, 17472, 17497, 17496, 17473, 18168, 18169, 19608, 18889, 
            18888, 19609, 18216, 18193, 18912, 19633, 19656, 18937, 18192, 18217, 
            19632, 18913, 18936, 19657, 22488, 22489, 22536, 22513, 22512, 22537, 
            27528, 27529, 32568, 37609, 37608, 32569, 27552, 27577, 37632, 32593, 
            32616, 37657, 27576, 27553, 32592, 37633, 37656, 32617, 23208, 23209, 
            23928, 24649, 24648, 23929, 28248, 28249, 38328, 33289, 33288, 38329, 
            29688, 28969, 34008, 39049, 39768, 34729, 28968, 29689, 39048, 34009, 
            34728, 39769, 23232, 23257, 24672, 23953, 23976, 24697, 28296, 28273, 
            33312, 38353, 38376, 33337, 28992, 29713, 39072, 34033, 34752, 39793, 
            29736, 29017, 34056, 39097, 39816, 34777, 23256, 23233, 23952, 24673, 
            24696, 23977, 28272, 28297, 38352, 33313, 33336, 38377, 29712, 28993, 
            34032, 39073, 39792, 34753, 29016, 29737, 39096, 34057, 34776, 39817, 
            17450, 17453, 17498, 17475, 17476, 17501, 18172, 18171, 18890, 19611, 
            19612, 18893, 18194, 18219, 19634, 18915, 18938, 19659, 18220, 18197, 
            18916, 19637, 19660, 18941, 22492, 22491, 22514, 22539, 22540, 22517, 
            27530, 27533, 37610, 32571, 32572, 37613, 27578, 27555, 32594, 37635, 
            37658, 32619, 27556, 27581, 37636, 32597, 32620, 37661, 23210, 23213, 
            24650, 23931, 23932, 24653, 28252, 28251, 33290, 38331, 38332, 33293, 
            28970, 29691, 39050, 34011, 34730, 39771, 29692, 28973, 34012, 39053, 
            39772, 34733, 23258, 23235, 23954, 24675, 24698, 23979, 28274, 28299, 
            38354, 33315, 33338, 38379, 29714, 28995, 34034, 39075, 39794, 34755, 
            29018, 29739, 39098, 34059, 34778, 39819, 23236, 23261, 24676, 23957, 
            23980, 24701, 28300, 28277, 33316, 38357, 38380, 33341, 28996, 29717, 
            39076, 34037, 34756, 39797, 29740, 29021, 34060, 39101, 39820, 34781, 
            17452, 17451, 17474, 17499, 17500, 17477, 18170, 18173, 19610, 18891, 
            18892, 19613, 18218, 18195, 18914, 19635, 19658, 18939, 18196, 18221, 
            19636, 18917, 18940, 19661, 22490, 22493, 22538, 22515, 22516, 22541, 
            27532, 27531, 32570, 37611, 37612, 32573, 27554, 27579, 37634, 32595, 
            32618, 37659, 27580, 27557, 32596, 37637, 37660, 32621, 23212, 23211, 
            23930, 24651, 24652, 23933, 28250, 28253, 38330, 33291, 33292, 38333, 
            29690, 28971, 34010, 39051, 39770, 34731, 28972, 29693, 39052, 34013, 
            34732, 39773, 23234, 23259, 24674, 23955, 23978, 24699, 28298, 28275, 
            33314, 38355, 38378, 33339, 28994, 29715, 39074, 34035, 34754, 39795, 
            29738, 29019, 34058, 39099, 39818, 34779, 23260, 23237, 23956, 24677, 
            24700, 23981, 28276, 28301, 38356, 33317, 33340, 38381, 29716, 28997, 
            34036, 39077, 39796, 34757, 29020, 29741, 39100, 34061, 34780, 39821, 
            11784, 11785, 11786, 11789, 11788, 11787, 11904, 11905, 12144, 12025, 
            12024, 12145, 11908, 11907, 12026, 12147, 12148, 12029, 11906, 11909, 
            12146, 12027, 12028, 12149, 12504, 12505, 12508, 12507, 12506, 12509, 
            13224, 13225, 13944, 14665, 14664, 13945, 13226, 13229, 14666, 13947, 
            13948, 14669, 13228, 13227, 13946, 14667, 14668, 13949, 12624, 12625, 
            12744, 12865, 12864, 12745, 13344, 13345, 14784, 14065, 14064, 14785, 
            13584, 13465, 14184, 14905, 15024, 14305, 13464, 13585, 14904, 14185, 
            14304, 15025, 12626, 12629, 12866, 12747, 12748, 12869, 13348, 13347, 
            14066, 14787, 14788, 14069, 13466, 13587, 14906, 14187, 14306, 15027, 
            13588, 13469, 14188, 14909, 15028, 14309, 12628, 12627, 12746, 12867, 
            12868, 12749, 13346, 13349, 14786, 14067, 14068, 14789, 13586, 13467, 
            14186, 14907, 15026, 14307, 13468, 13589, 14908, 14189, 14308, 15029, 
            16824, 16825, 16828, 16827, 16826, 16829, 16944, 16945, 17064, 17185, 
            17184, 17065, 16946, 16949, 17186, 17067, 17068, 17189, 16948, 16947, 
            17066, 17187, 17188, 17069, 21864, 21865, 21866, 21869, 21868, 21867, 
            26904, 26905, 36984, 31945, 31944, 36985, 26908, 26907, 31946, 36987, 
            36988, 31949, 26906, 26909, 36986, 31947, 31948, 36989, 21984, 21985, 
            22224, 22105, 22104, 22225, 27024, 27025, 32064, 37105, 37104, 32065, 
            27144, 27265, 37224, 32185, 32304, 37345, 27264, 27145, 32184, 37225, 
            37344, 32305, 21988, 21987, 22106, 22227, 22228, 22109, 27026, 27029, 
            37106, 32067, 32068, 37109, 27266, 27147, 32186, 37227, 37346, 32307, 
            27148, 27269, 37228, 32189, 32308, 37349, 21986, 21989, 22226, 22107, 
            22108, 22229, 27028, 27027, 32066, 37107, 37108, 32069, 27146, 27267, 
            37226, 32187, 32306, 37347, 27268, 27149, 32188, 37229, 37348, 32309, 
            17544, 17545, 17546, 17549, 17548, 17547, 18264, 18265, 19704, 18985, 
            18984, 19705, 18268, 18267, 18986, 19707, 19708, 18989, 18266, 18269, 
            19706, 18987, 18988, 19709, 22584, 22585, 22588, 22587, 22586, 22589, 
            27624, 27625, 32664, 37705, 37704, 32665, 27626, 27629, 37706, 32667, 
            32668, 37709, 27628, 27627, 32666, 37707, 37708, 32669, 23304, 23305, 
            24024, 24745, 24744, 24025, 28344, 28345, 38424, 33385, 33384, 38425, 
            29784, 29065, 34104, 39145, 39864, 34825, 29064, 29785, 39144, 34105, 
            34824, 39865, 23306, 23309, 24746, 24027, 24028, 24749, 28348, 28347, 
            33386, 38427, 38428, 33389, 29066, 29787, 39146, 34107, 34826, 39867, 
            29788, 29069, 34108, 39149, 39868, 34829, 23308, 23307, 24026, 24747, 
            24748, 24029, 28346, 28349, 38426, 33387, 33388, 38429, 29786, 29067, 
            34106, 39147, 39866, 34827, 29068, 29789, 39148, 34109, 34828, 39869, 
            17664, 17665, 17904, 17785, 17784, 17905, 18384, 18385, 19104, 19825, 
            19824, 19105, 18504, 18625, 19944, 19225, 19344, 20065, 18624, 18505, 
            19224, 19945, 20064, 19345, 22704, 22705, 22824, 22945, 22944, 22825, 
            27744, 27745, 37824, 32785, 32784, 37825, 27984, 27865, 32904, 37945, 
            38064, 33025, 27864, 27985, 37944, 32905, 33024, 38065, 23424, 23425, 
            24864, 24145, 24144, 24865, 28464, 28465, 33504, 38545, 38544, 33505, 
            29184, 29905, 39264, 34225, 34944, 39985, 29904, 29185, 34224, 39265, 
            39984, 34945, 23664, 23545, 24264, 24985, 25104, 24385, 28584, 28705, 
            38664, 33625, 33744, 38785, 30024, 29305, 34344, 39385, 40104, 35065, 
            29424, 30145, 39504, 34465, 35184, 40225, 23544, 23665, 24984, 24265, 
            24384, 25105, 28704, 28585, 33624, 38665, 38784, 33745, 29304, 30025, 
            39384, 34345, 35064, 40105, 30144, 29425, 34464, 39505, 40224, 35185, 
            17668, 17667, 17786, 17907, 17908, 17789, 18386, 18389, 19826, 19107, 
            19108, 19829, 18626, 18507, 19226, 19947, 20066, 19347, 18508, 18629, 
            19948, 19229, 19348, 20069, 22706, 22709, 22946, 22827, 22828, 22949, 
            27748, 27747, 32786, 37827, 37828, 32789, 27866, 27987, 37946, 32907, 
            33026, 38067, 27988, 27869, 32908, 37949, 38068, 33029, 23428, 23427, 
            24146, 24867, 24868, 24149, 28466, 28469, 38546, 33507, 33508, 38549, 
            29906, 29187, 34226, 39267, 39986, 34947, 29188, 29909, 39268, 34229, 
            34948, 39989, 23546, 23667, 24986, 24267, 24386, 25107, 28706, 28587, 
            33626, 38667, 38786, 33747, 29306, 30027, 39386, 34347, 35066, 40107, 
            30146, 29427, 34466, 39507, 40226, 35187, 23668, 23549, 24268, 24989, 
            25108, 24389, 28588, 28709, 38668, 33629, 33748, 38789, 30028, 29309, 
            34348, 39389, 40108, 35069, 29428, 30149, 39508, 34469, 35188, 40229, 
            17666, 17669, 17906, 17787, 17788, 17909, 18388, 18387, 19106, 19827, 
            19828, 19109, 18506, 18627, 19946, 19227, 19346, 20067, 18628, 18509, 
            19228, 19949, 20068, 19349, 22708, 22707, 22826, 22947, 22948, 22829, 
            27746, 27749, 37826, 32787, 32788, 37829, 27986, 27867, 32906, 37947, 
            38066, 33027, 27868, 27989, 37948, 32909, 33028, 38069, 23426, 23429, 
            24866, 24147, 24148, 24869, 28468, 28467, 33506, 38547, 38548, 33509, 
            29186, 29907, 39266, 34227, 34946, 39987, 29908, 29189, 34228, 39269, 
            39988, 34949, 23666, 23547, 24266, 24987, 25106, 24387, 28586, 28707, 
            38666, 33627, 33746, 38787, 30026, 29307, 34346, 39387, 40106, 35067, 
            29426, 30147, 39506, 34467, 35186, 40227, 23548, 23669, 24988, 24269, 
            24388, 25109, 28708, 28589, 33628, 38669, 38788, 33749, 29308, 30029, 
            39388, 34349, 35068, 40109, 30148, 29429, 34468, 39509, 40228, 35189, 
            11808, 11809, 11856, 11833, 11832, 11857, 11928, 11929, 12048, 12169, 
            12168, 12049, 11952, 11977, 12192, 12073, 12096, 12217, 11976, 11953, 
            12072, 12193, 12216, 12097, 12528, 12529, 12552, 12577, 12576, 12553, 
            13248, 13249, 14688, 13969, 13968, 14689, 13296, 13273, 13992, 14713, 
            14736, 14017, 13272, 13297, 14712, 13993, 14016, 14737, 12648, 12649, 
            12888, 12769, 12768, 12889, 13368, 13369, 14088, 14809, 14808, 14089, 
            13488, 13609, 14928, 14209, 14328, 15049, 13608, 13489, 14208, 14929, 
            15048, 14329, 12696, 12673, 12792, 12913, 12936, 12817, 13392, 13417, 
            14832, 14113, 14136, 14857, 13632, 13513, 14232, 14953, 15072, 14353, 
            13536, 13657, 14976, 14257, 14376, 15097, 12672, 12697, 12912, 12793, 
            12816, 12937, 13416, 13393, 14112, 14833, 14856, 14137, 13512, 13633, 
            14952, 14233, 14352, 15073, 13656, 13537, 14256, 14977, 15096, 14377, 
            16848, 16849, 16872, 16897, 16896, 16873, 16968, 16969, 17208, 17089, 
            17088, 17209, 17016, 16993, 17112, 17233, 17256, 17137, 16992, 17017, 
            17232, 17113, 17136, 17257, 21888, 21889, 21936, 21913, 21912, 21937, 
            26928, 26929, 31968, 37009, 37008, 31969, 26952, 26977, 37032, 31993, 
            32016, 37057, 26976, 26953, 31992, 37033, 37056, 32017, 22008, 22009, 
            22128, 22249, 22248, 22129, 27048, 27049, 37128, 32089, 32088, 37129, 
            27288, 27169, 32208, 37249, 37368, 32329, 27168, 27289, 37248, 32209, 
            32328, 37369, 22032, 22057, 22272, 22153, 22176, 22297, 27096, 27073, 
            32112, 37153, 37176, 32137, 27192, 27313, 37272, 32233, 32352, 37393, 
            27336, 27217, 32256, 37297, 37416, 32377, 22056, 22033, 22152, 22273, 
            22296, 22177, 27072, 27097, 37152, 32113, 32136, 37177, 27312, 27193, 
            32232, 37273, 37392, 32353, 27216, 27337, 37296, 32257, 32376, 37417, 
            17568, 17569, 17616, 17593, 17592, 17617, 18288, 18289, 19008, 19729, 
            19728, 19009, 18312, 18337, 19752, 19033, 19056, 19777, 18336, 18313, 
            19032, 19753, 19776, 19057, 22608, 22609, 22632, 22657, 22656, 22633, 
            27648, 27649, 37728, 32689, 32688, 37729, 27696, 27673, 32712, 37753, 
            37776, 32737, 27672, 27697, 37752, 32713, 32736, 37777, 23328, 23329, 
            24768, 24049, 24048, 24769, 28368, 28369, 33408, 38449, 38448, 33409, 
            29088, 29809, 39168, 34129, 34848, 39889, 29808, 29089, 34128, 39169, 
            39888, 34849, 23376, 23353, 24072, 24793, 24816, 24097, 28392, 28417, 
            38472, 33433, 33456, 38497, 29832, 29113, 34152, 39193, 39912, 34873, 
            29136, 29857, 39216, 34177, 34896, 39937, 23352, 23377, 24792, 24073, 
            24096, 24817, 28416, 28393, 33432, 38473, 38496, 33457, 29112, 29833, 
            39192, 34153, 34872, 39913, 29856, 29137, 34176, 39217, 39936, 34897, 
            17688, 17689, 17808, 17929, 17928, 17809, 18408, 18409, 19848, 19129, 
            19128, 19849, 18648, 18529, 19248, 19969, 20088, 19369, 18528, 18649, 
            19968, 19249, 19368, 20089, 22728, 22729, 22968, 22849, 22848, 22969, 
            27768, 27769, 32808, 37849, 37848, 32809, 27888, 28009, 37968, 32929, 
            33048, 38089, 28008, 27889, 32928, 37969, 38088, 33049, 23448, 23449, 
            24168, 24889, 24888, 24169, 28488, 28489, 38568, 33529, 33528, 38569, 
            29928, 29209, 34248, 39289, 40008, 34969, 29208, 29929, 39288, 34249, 
            34968, 40009, 23568, 23689, 25008, 24289, 24408, 25129, 28728, 28609, 
            33648, 38689, 38808, 33769, 29328, 30049, 39408, 34369, 35088, 40129, 
            30168, 29449, 34488, 39529, 40248, 35209, 23688, 23569, 24288, 25009, 
            25128, 24409, 28608, 28729, 38688, 33649, 33768, 38809, 30048, 29329, 
            34368, 39409, 40128, 35089, 29448, 30169, 39528, 34489, 35208, 40249, 
            17712, 17737, 17952, 17833, 17856, 17977, 18456, 18433, 19152, 19873, 
            19896, 19177, 18552, 18673, 19992, 19273, 19392, 20113, 18696, 18577, 
            19296, 20017, 20136, 19417, 22776, 22753, 22872, 22993, 23016, 22897, 
            27792, 27817, 37872, 32833, 32856, 37897, 28032, 27913, 32952, 37993, 
            38112, 33073, 27936, 28057, 38016, 32977, 33096, 38137, 23472, 23497, 
            24912, 24193, 24216, 24937, 28536, 28513, 33552, 38593, 38616, 33577, 
            29232, 29953, 39312, 34273, 34992, 40033, 29976, 29257, 34296, 39337, 
            40056, 35017, 23712, 23593, 24312, 25033, 25152, 24433, 28632, 28753, 
            38712, 33673, 33792, 38833, 30072, 29353, 34392, 39433, 40152, 35113, 
            29472, 30193, 39552, 34513, 35232, 40273, 23616, 23737, 25056, 24337, 
            24456, 25177, 28776, 28657, 33696, 38737, 38856, 33817, 29376, 30097, 
            39456, 34417, 35136, 40177, 30216, 29497, 34536, 39577, 40296, 35257, 
            17736, 17713, 17832, 17953, 17976, 17857, 18432, 18457, 19872, 19153, 
            19176, 19897, 18672, 18553, 19272, 19993, 20112, 19393, 18576, 18697, 
            20016, 19297, 19416, 20137, 22752, 22777, 22992, 22873, 22896, 23017, 
            27816, 27793, 32832, 37873, 37896, 32857, 27912, 28033, 37992, 32953, 
            33072, 38113, 28056, 27937, 32976, 38017, 38136, 33097, 23496, 23473, 
            24192, 24913, 24936, 24217, 28512, 28537, 38592, 33553, 33576, 38617, 
            29952, 29233, 34272, 39313, 40032, 34993, 29256, 29977, 39336, 34297, 
            35016, 40057, 23592, 23713, 25032, 24313, 24432, 25153, 28752, 28633, 
            33672, 38713, 38832, 33793, 29352, 30073, 39432, 34393, 35112, 40153, 
            30192, 29473, 34512, 39553, 40272, 35233, 23736, 23617, 24336, 25057, 
            25176, 24457, 28656, 28777, 38736, 33697, 33816, 38857, 30096, 29377, 
            34416, 39457, 40176, 35137, 29496, 30217, 39576, 34537, 35256, 40297, 
            11812, 11811, 11834, 11859, 11860, 11837, 11930, 11933, 12170, 12051, 
            12052, 12173, 11978, 11955, 12074, 12195, 12218, 12099, 11956, 11981, 
            12196, 12077, 12100, 12221, 12530, 12533, 12578, 12555, 12556, 12581, 
            13252, 13251, 13970, 14691, 14692, 13973, 13274, 13299, 14714, 13995, 
            14018, 14739, 13300, 13277, 13996, 14717, 14740, 14021, 12652, 12651, 
            12770, 12891, 12892, 12773, 13370, 13373, 14810, 14091, 14092, 14813, 
            13610, 13491, 14210, 14931, 15050, 14331, 13492, 13613, 14932, 14213, 
            14332, 15053, 12674, 12699, 12914, 12795, 12818, 12939, 13418, 13395, 
            14114, 14835, 14858, 14139, 13514, 13635, 14954, 14235, 14354, 15075, 
            13658, 13539, 14258, 14979, 15098, 14379, 12700, 12677, 12796, 12917, 
            12940, 12821, 13396, 13421, 14836, 14117, 14140, 14861, 13636, 13517, 
            14236, 14957, 15076, 14357, 13540, 13661, 14980, 14261, 14380, 15101, 
            16850, 16853, 16898, 16875, 16876, 16901, 16972, 16971, 17090, 17211, 
            17212, 17093, 16994, 17019, 17234, 17115, 17138, 17259, 17020, 16997, 
            17116, 17237, 17260, 17141, 21892, 21891, 21914, 21939, 21940, 21917, 
            26930, 26933, 37010, 31971, 31972, 37013, 26978, 26955, 31994, 37035, 
            37058, 32019, 26956, 26981, 37036, 31997, 32020, 37061, 22010, 22013, 
            22250, 22131, 22132, 22253, 27052, 27051, 32090, 37131, 37132, 32093, 
            27170, 27291, 37250, 32211, 32330, 37371, 27292, 27173, 32212, 37253, 
            37372, 32333, 22058, 22035, 22154, 22275, 22298, 22179, 27074, 27099, 
            37154, 32115, 32138, 37179, 27314, 27195, 32234, 37275, 37394, 32355, 
            27218, 27339, 37298, 32259, 32378, 37419, 22036, 22061, 22276, 22157, 
            22180, 22301, 27100, 27077, 32116, 37157, 37180, 32141, 27196, 27317, 
            37276, 32237, 32356, 37397, 27340, 27221, 32260, 37301, 37420, 32381, 
            17572, 17571, 17594, 17619, 17620, 17597, 18290, 18293, 19730, 19011, 
            19012, 19733, 18338, 18315, 19034, 19755, 19778, 19059, 18316, 18341, 
            19756, 19037, 19060, 19781, 22610, 22613, 22658, 22635, 22636, 22661, 
            27652, 27651, 32690, 37731, 37732, 32693, 27674, 27699, 37754, 32715, 
            32738, 37779, 27700, 27677, 32716, 37757, 37780, 32741, 23332, 23331, 
            24050, 24771, 24772, 24053, 28370, 28373, 38450, 33411, 33412, 38453, 
            29810, 29091, 34130, 39171, 39890, 34851, 29092, 29813, 39172, 34133, 
            34852, 39893, 23354, 23379, 24794, 24075, 24098, 24819, 28418, 28395, 
            33434, 38475, 38498, 33459, 29114, 29835, 39194, 34155, 34874, 39915, 
            29858, 29139, 34178, 39219, 39938, 34899, 23380, 23357, 24076, 24797, 
            24820, 24101, 28396, 28421, 38476, 33437, 33460, 38501, 29836, 29117, 
            34156, 39197, 39916, 34877, 29140, 29861, 39220, 34181, 34900, 39941, 
            17690, 17693, 17930, 17811, 17812, 17933, 18412, 18411, 19130, 19851, 
            19852, 19133, 18530, 18651, 19970, 19251, 19370, 20091, 18652, 18533, 
            19252, 19973, 20092, 19373, 22732, 22731, 22850, 22971, 22972, 22853, 
            27770, 27773, 37850, 32811, 32812, 37853, 28010, 27891, 32930, 37971, 
            38090, 33051, 27892, 28013, 37972, 32933, 33052, 38093, 23450, 23453, 
            24890, 24171, 24172, 24893, 28492, 28491, 33530, 38571, 38572, 33533, 
            29210, 29931, 39290, 34251, 34970, 40011, 29932, 29213, 34252, 39293, 
            40012, 34973, 23690, 23571, 24290, 25011, 25130, 24411, 28610, 28731, 
            38690, 33651, 33770, 38811, 30050, 29331, 34370, 39411, 40130, 35091, 
            29450, 30171, 39530, 34491, 35210, 40251, 23572, 23693, 25012, 24293, 
            24412, 25133, 28732, 28613, 33652, 38693, 38812, 33773, 29332, 30053, 
            39412, 34373, 35092, 40133, 30172, 29453, 34492, 39533, 40252, 35213, 
            17738, 17715, 17834, 17955, 17978, 17859, 18434, 18459, 19874, 19155, 
            19178, 19899, 18674, 18555, 19274, 19995, 20114, 19395, 18578, 18699, 
            20018, 19299, 19418, 20139, 22754, 22779, 22994, 22875, 22898, 23019, 
            27818, 27795, 32834, 37875, 37898, 32859, 27914, 28035, 37994, 32955, 
            33074, 38115, 28058, 27939, 32978, 38019, 38138, 33099, 23498, 23475, 
            24194, 24915, 24938, 24219, 28514, 28539, 38594, 33555, 33578, 38619, 
            29954, 29235, 34274, 39315, 40034, 34995, 29258, 29979, 39338, 34299, 
            35018, 40059, 23594, 23715, 25034, 24315, 24434, 25155, 28754, 28635, 
            33674, 38715, 38834, 33795, 29354, 30075, 39434, 34395, 35114, 40155, 
            30194, 29475, 34514, 39555, 40274, 35235, 23738, 23619, 24338, 25059, 
            25178, 24459, 28658, 28779, 38738, 33699, 33818, 38859, 30098, 29379, 
            34418, 39459, 40178, 35139, 29498, 30219, 39578, 34539, 35258, 40299, 
            17716, 17741, 17956, 17837, 17860, 17981, 18460, 18437, 19156, 19877, 
            19900, 19181, 18556, 18677, 19996, 19277, 19396, 20117, 18700, 18581, 
            19300, 20021, 20140, 19421, 22780, 22757, 22876, 22997, 23020, 22901, 
            27796, 27821, 37876, 32837, 32860, 37901, 28036, 27917, 32956, 37997, 
            38116, 33077, 27940, 28061, 38020, 32981, 33100, 38141, 23476, 23501, 
            24916, 24197, 24220, 24941, 28540, 28517, 33556, 38597, 38620, 33581, 
            29236, 29957, 39316, 34277, 34996, 40037, 29980, 29261, 34300, 39341, 
            40060, 35021, 23716, 23597, 24316, 25037, 25156, 24437, 28636, 28757, 
            38716, 33677, 33796, 38837, 30076, 29357, 34396, 39437, 40156, 35117, 
            29476, 30197, 39556, 34517, 35236, 40277, 23620, 23741, 25060, 24341, 
            24460, 25181, 28780, 28661, 33700, 38741, 38860, 33821, 29380, 30101, 
            39460, 34421, 35140, 40181, 30220, 29501, 34540, 39581, 40300, 35261, 
            11810, 11813, 11858, 11835, 11836, 11861, 11932, 11931, 12050, 12171, 
            12172, 12053, 11954, 11979, 12194, 12075, 12098, 12219, 11980, 11957, 
            12076, 12197, 12220, 12101, 12532, 12531, 12554, 12579, 12580, 12557, 
            13250, 13253, 14690, 13971, 13972, 14693, 13298, 13275, 13994, 14715, 
            14738, 14019, 13276, 13301, 14716, 13997, 14020, 14741, 12650, 12653, 
            12890, 12771, 12772, 12893, 13372, 13371, 14090, 14811, 14812, 14093, 
            13490, 13611, 14930, 14211, 14330, 15051, 13612, 13493, 14212, 14933, 
            15052, 14333, 12698, 12675, 12794, 12915, 12938, 12819, 13394, 13419, 
            14834, 14115, 14138, 14859, 13634, 13515, 14234, 14955, 15074, 14355, 
            13538, 13659, 14978, 14259, 14378, 15099, 12676, 12701, 12916, 12797, 
            12820, 12941, 13420, 13397, 14116, 14837, 14860, 14141, 13516, 13637, 
            14956, 14237, 14356, 15077, 13660, 13541, 14260, 14981, 15100, 14381, 
            16852, 16851, 16874, 16899, 16900, 16877, 16970, 16973, 17210, 17091, 
            17092, 17213, 17018, 16995, 17114, 17235, 17258, 17139, 16996, 17021, 
            17236, 17117, 17140, 17261, 21890, 21893, 21938, 21915, 21916, 21941, 
            26932, 26931, 31970, 37011, 37012, 31973, 26954, 26979, 37034, 31995, 
            32018, 37059, 26980, 26957, 31996, 37037, 37060, 32021, 22012, 22011, 
            22130, 22251, 22252, 22133, 27050, 27053, 37130, 32091, 32092, 37133, 
            27290, 27171, 32210, 37251, 37370, 32331, 27172, 27293, 37252, 32213, 
            32332, 37373, 22034, 22059, 22274, 22155, 22178, 22299, 27098, 27075, 
            32114, 37155, 37178, 32139, 27194, 27315, 37274, 32235, 32354, 37395, 
            27338, 27219, 32258, 37299, 37418, 32379, 22060, 22037, 22156, 22277, 
            22300, 22181, 27076, 27101, 37156, 32117, 32140, 37181, 27316, 27197, 
            32236, 37277, 37396, 32357, 27220, 27341, 37300, 32261, 32380, 37421, 
            17570, 17573, 17618, 17595, 17596, 17621, 18292, 18291, 19010, 19731, 
            19732, 19013, 18314, 18339, 19754, 19035, 19058, 19779, 18340, 18317, 
            19036, 19757, 19780, 19061, 22612, 22611, 22634, 22659, 22660, 22637, 
            27650, 27653, 37730, 32691, 32692, 37733, 27698, 27675, 32714, 37755, 
            37778, 32739, 27676, 27701, 37756, 32717, 32740, 37781, 23330, 23333, 
            24770, 24051, 24052, 24773, 28372, 28371, 33410, 38451, 38452, 33413, 
            29090, 29811, 39170, 34131, 34850, 39891, 29812, 29093, 34132, 39173, 
            39892, 34853, 23378, 23355, 24074, 24795, 24818, 24099, 28394, 28419, 
            38474, 33435, 33458, 38499, 29834, 29115, 34154, 39195, 39914, 34875, 
            29138, 29859, 39218, 34179, 34898, 39939, 23356, 23381, 24796, 24077, 
            24100, 24821, 28420, 28397, 33436, 38477, 38500, 33461, 29116, 29837, 
            39196, 34157, 34876, 39917, 29860, 29141, 34180, 39221, 39940, 34901, 
            17692, 17691, 17810, 17931, 17932, 17813, 18410, 18413, 19850, 19131, 
            19132, 19853, 18650, 18531, 19250, 19971, 20090, 19371, 18532, 18653, 
            19972, 19253, 19372, 20093, 22730, 22733, 22970, 22851, 22852, 22973, 
            27772, 27771, 32810, 37851, 37852, 32813, 27890, 28011, 37970, 32931, 
            33050, 38091, 28012, 27893, 32932, 37973, 38092, 33053, 23452, 23451, 
            24170, 24891, 24892, 24173, 28490, 28493, 38570, 33531, 33532, 38573, 
            29930, 29211, 34250, 39291, 40010, 34971, 29212, 29933, 39292, 34253, 
            34972, 40013, 23570, 23691, 25010, 24291, 24410, 25131, 28730, 28611, 
            33650, 38691, 38810, 33771, 29330, 30051, 39410, 34371, 35090, 40131, 
            30170, 29451, 34490, 39531, 40250, 35211, 23692, 23573, 24292, 25013, 
            25132, 24413, 28612, 28733, 38692, 33653, 33772, 38813, 30052, 29333, 
            34372, 39413, 40132, 35093, 29452, 30173, 39532, 34493, 35212, 40253, 
            17714, 17739, 17954, 17835, 17858, 17979, 18458, 18435, 19154, 19875, 
            19898, 19179, 18554, 18675, 19994, 19275, 19394, 20115, 18698, 18579, 
            19298, 20019, 20138, 19419, 22778, 22755, 22874, 22995, 23018, 22899, 
            27794, 27819, 37874, 32835, 32858, 37899, 28034, 27915, 32954, 37995, 
            38114, 33075, 27938, 28059, 38018, 32979, 33098, 38139, 23474, 23499, 
            24914, 24195, 24218, 24939, 28538, 28515, 33554, 38595, 38618, 33579, 
            29234, 29955, 39314, 34275, 34994, 40035, 29978, 29259, 34298, 39339, 
            40058, 35019, 23714, 23595, 24314, 25035, 25154, 24435, 28634, 28755, 
            38714, 33675, 33794, 38835, 30074, 29355, 34394, 39435, 40154, 35115, 
            29474, 30195, 39554, 34515, 35234, 40275, 23618, 23739, 25058, 24339, 
            24458, 25179, 28778, 28659, 33698, 38739, 38858, 33819, 29378, 30099, 
            39458, 34419, 35138, 40179, 30218, 29499, 34538, 39579, 40298, 35259, 
            17740, 17717, 17836, 17957, 17980, 17861, 18436, 18461, 19876, 19157, 
            19180, 19901, 18676, 18557, 19276, 19997, 20116, 19397, 18580, 18701, 
            20020, 19301, 19420, 20141, 22756, 22781, 22996, 22877, 22900, 23021, 
            27820, 27797, 32836, 37877, 37900, 32861, 27916, 28037, 37996, 32957, 
            33076, 38117, 28060, 27941, 32980, 38021, 38140, 33101, 23500, 23477, 
            24196, 24917, 24940, 24221, 28516, 28541, 38596, 33557, 33580, 38621, 
            29956, 29237, 34276, 39317, 40036, 34997, 29260, 29981, 39340, 34301, 
            35020, 40061, 23596, 23717, 25036, 24317, 24436, 25157, 28756, 28637, 
            33676, 38717, 38836, 33797, 29356, 30077, 39436, 34397, 35116, 40157, 
            30196, 29477, 34516, 39557, 40276, 35237, 23740, 23621, 24340, 25061, 
            25180, 24461, 28660, 28781, 38740, 33701, 33820, 38861, 30100, 29381, 
            34420, 39461, 40180, 35141, 29500, 30221, 39580, 34541, 35260, 40301, 
            5910, 5911, 5916, 5923, 5922, 5917, 5934, 5935, 5982, 5959, 
            5958, 5983, 5946, 5941, 5964, 5989, 5994, 5971, 5940, 5947, 
            5988, 5965, 5970, 5995, 6030, 6031, 6042, 6037, 6036, 6043, 
            6150, 6151, 6270, 6391, 6390, 6271, 6156, 6163, 6396, 6277, 
            6282, 6403, 6162, 6157, 6276, 6397, 6402, 6283, 6054, 6055, 
            6078, 6103, 6102, 6079, 6174, 6175, 6414, 6295, 6294, 6415, 
            6222, 6199, 6318, 6439, 6462, 6343, 6198, 6223, 6438, 6319, 
            6342, 6463, 6060, 6067, 6108, 6085, 6090, 6115, 6186, 6181, 
            6300, 6421, 6426, 6307, 6204, 6229, 6444, 6325, 6348, 6469, 
            6234, 6211, 6330, 6451, 6474, 6355, 6066, 6061, 6084, 6109, 
            6114, 6091, 6180, 6187, 6420, 6301, 6306, 6427, 6228, 6205, 
            6324, 6445, 6468, 6349, 6210, 6235, 6450, 6331, 6354, 6475, 
            6630, 6631, 6642, 6637, 6636, 6643, 6654, 6655, 6678, 6703, 
            6702, 6679, 6660, 6667, 6708, 6685, 6690, 6715, 6666, 6661, 
            6684, 6709, 6714, 6691, 7350, 7351, 7356, 7363, 7362, 7357, 
            8070, 8071, 9510, 8791, 8790, 9511, 8082, 8077, 8796, 9517, 
            9522, 8803, 8076, 8083, 9516, 8797, 8802, 9523, 7374, 7375, 
            7422, 7399, 7398, 7423, 8094, 8095, 8814, 9535, 9534, 8815, 
            8118, 8143, 9558, 8839, 8862, 9583, 8142, 8119, 8838, 9559, 
            9582, 8863, 7386, 7381, 7404, 7429, 7434, 7411, 8100, 8107, 
            9540, 8821, 8826, 9547, 8148, 8125, 8844, 9565, 9588, 8869, 
            8130, 8155, 9570, 8851, 8874, 9595, 7380, 7387, 7428, 7405, 
            7410, 7435, 8106, 8101, 8820, 9541, 9546, 8827, 8124, 8149, 
            9564, 8845, 8868, 9589, 8154, 8131, 8850, 9571, 9594, 8875, 
            6750, 6751, 6756, 6763, 6762, 6757, 6870, 6871, 7110, 6991, 
            6990, 7111, 6882, 6877, 6996, 7117, 7122, 7003, 6876, 6883, 
            7116, 6997, 7002, 7123, 7470, 7471, 7482, 7477, 7476, 7483, 
            8190, 8191, 8910, 9631, 9630, 8911, 8196, 8203, 9636, 8917, 
            8922, 9643, 8202, 8197, 8916, 9637, 9642, 8923, 7590, 7591, 
            7710, 7831, 7830, 7711, 8310, 8311, 9750, 9031, 9030, 9751, 
            8550, 8431, 9150, 9871, 9990, 9271, 8430, 8551, 9870, 9151, 
            9270, 9991, 7596, 7603, 7836, 7717, 7722, 7843, 8322, 8317, 
            9036, 9757, 9762, 9043, 8436, 8557, 9876, 9157, 9276, 9997, 
            8562, 8443, 9162, 9883, 10002, 9283, 7602, 7597, 7716, 7837, 
            7842, 7723, 8316, 8323, 9756, 9037, 9042, 9763, 8556, 8437, 
            9156, 9877, 9996, 9277, 8442, 8563, 9882, 9163, 9282, 10003, 
            6774, 6775, 6822, 6799, 6798, 6823, 6894, 6895, 7014, 7135, 
            7134, 7015, 6918, 6943, 7158, 7039, 7062, 7183, 6942, 6919, 
            7038, 7159, 7182, 7063, 7494, 7495, 7518, 7543, 7542, 7519, 
            8214, 8215, 9654, 8935, 8934, 9655, 8262, 8239, 8958, 9679, 
            9702, 8983, 8238, 8263, 9678, 8959, 8982, 9703, 7614, 7615, 
            7854, 7735, 7734, 7855, 8334, 8335, 9054, 9775, 9774, 9055, 
            8454, 8575, 9894, 9175, 9294, 10015, 8574, 8455, 9174, 9895, 
            10014, 9295, 7662, 7639, 7758, 7879, 7902, 7783, 8358, 8383, 
            9798, 9079, 9102, 9823, 8598, 8479, 9198, 9919, 10038, 9319, 
            8502, 8623, 9942, 9223, 9342, 10063, 7638, 7663, 7878, 7759, 
            7782, 7903, 8382, 8359, 9078, 9799, 9822, 9103, 8478, 8599, 
            9918, 9199, 9318, 10039, 8622, 8503, 9222, 9943, 10062, 9343, 
            6786, 6781, 6804, 6829, 6834, 6811, 6900, 6907, 7140, 7021, 
            7026, 7147, 6948, 6925, 7044, 7165, 7188, 7069, 6930, 6955, 
            7170, 7051, 7074, 7195, 7500, 7507, 7548, 7525, 7530, 7555, 
            8226, 8221, 8940, 9661, 9666, 8947, 8244, 8269, 9684, 8965, 
            8988, 9709, 8274, 8251, 8970, 9691, 9714, 8995, 7626, 7621, 
            7740, 7861, 7866, 7747, 8340, 8347, 9780, 9061, 9066, 9787, 
            8580, 8461, 9180, 9901, 10020, 9301, 8466, 8587, 9906, 9187, 
            9306, 10027, 7644, 7669, 7884, 7765, 7788, 7909, 8388, 8365, 
            9084, 9805, 9828, 9109, 8484, 8605, 9924, 9205, 9324, 10045, 
            8628, 8509, 9228, 9949, 10068, 9349, 7674, 7651, 7770, 7891, 
            7914, 7795, 8370, 8395, 9810, 9091, 9114, 9835, 8610, 8491, 
            9210, 9931, 10050, 9331, 8514, 8635, 9954, 9235, 9354, 10075, 
            6780, 6787, 6828, 6805, 6810, 6835, 6906, 6901, 7020, 7141, 
            7146, 7027, 6924, 6949, 7164, 7045, 7068, 7189, 6954, 6931, 
            7050, 7171, 7194, 7075, 7506, 7501, 7524, 7549, 7554, 7531, 
            8220, 8227, 9660, 8941, 8946, 9667, 8268, 8245, 8964, 9685, 
            9708, 8989, 8250, 8275, 9690, 8971, 8994, 9715, 7620, 7627, 
            7860, 7741, 7746, 7867, 8346, 8341, 9060, 9781, 9786, 9067, 
            8460, 8581, 9900, 9181, 9300, 10021, 8586, 8467, 9186, 9907, 
            10026, 9307, 7668, 7645, 7764, 7885, 7908, 7789, 8364, 8389, 
            9804, 9085, 9108, 9829, 8604, 8485, 9204, 9925, 10044, 9325, 
            8508, 8629, 9948, 9229, 9348, 10069, 7650, 7675, 7890, 7771, 
            7794, 7915, 8394, 8371, 9090, 9811, 9834, 9115, 8490, 8611, 
            9930, 9211, 9330, 10051, 8634, 8515, 9234, 9955, 10074, 9355, 
            10950, 10951, 10962, 10957, 10956, 10963, 10974, 10975, 10998, 11023, 
            11022, 10999, 10980, 10987, 11028, 11005, 11010, 11035, 10986, 10981, 
            11004, 11029, 11034, 11011, 11070, 11071, 11076, 11083, 11082, 11077, 
            11190, 11191, 11430, 11311, 11310, 11431, 11202, 11197, 11316, 11437, 
            11442, 11323, 11196, 11203, 11436, 11317, 11322, 11443, 11094, 11095, 
            11142, 11119, 11118, 11143, 11214, 11215, 11334, 11455, 11454, 11335, 
            11238, 11263, 11478, 11359, 11382, 11503, 11262, 11239, 11358, 11479, 
            11502, 11383, 11106, 11101, 11124, 11149, 11154, 11131, 11220, 11227, 
            11460, 11341, 11346, 11467, 11268, 11245, 11364, 11485, 11508, 11389, 
            11250, 11275, 11490, 11371, 11394, 11515, 11100, 11107, 11148, 11125, 
            11130, 11155, 11226, 11221, 11340, 11461, 11466, 11347, 11244, 11269, 
            11484, 11365, 11388, 11509, 11274, 11251, 11370, 11491, 11514, 11395, 
            15990, 15991, 15996, 16003, 16002, 15997, 16014, 16015, 16062, 16039, 
            16038, 16063, 16026, 16021, 16044, 16069, 16074, 16051, 16020, 16027, 
            16068, 16045, 16050, 16075, 21030, 21031, 21042, 21037, 21036, 21043, 
            26070, 26071, 31110, 36151, 36150, 31111, 26076, 26083, 36156, 31117, 
            31122, 36163, 26082, 26077, 31116, 36157, 36162, 31123, 21054, 21055, 
            21078, 21103, 21102, 21079, 26094, 26095, 36174, 31135, 31134, 36175, 
            26142, 26119, 31158, 36199, 36222, 31183, 26118, 26143, 36198, 31159, 
            31182, 36223, 21060, 21067, 21108, 21085, 21090, 21115, 26106, 26101, 
            31140, 36181, 36186, 31147, 26124, 26149, 36204, 31165, 31188, 36229, 
            26154, 26131, 31170, 36211, 36234, 31195, 21066, 21061, 21084, 21109, 
            21114, 21091, 26100, 26107, 36180, 31141, 31146, 36187, 26148, 26125, 
            31164, 36205, 36228, 31189, 26130, 26155, 36210, 31171, 31194, 36235, 
            16110, 16111, 16122, 16117, 16116, 16123, 16230, 16231, 16350, 16471, 
            16470, 16351, 16236, 16243, 16476, 16357, 16362, 16483, 16242, 16237, 
            16356, 16477, 16482, 16363, 21150, 21151, 21156, 21163, 21162, 21157, 
            26190, 26191, 36270, 31231, 31230, 36271, 26202, 26197, 31236, 36277, 
            36282, 31243, 26196, 26203, 36276, 31237, 31242, 36283, 21270, 21271, 
            21510, 21391, 21390, 21511, 26310, 26311, 31350, 36391, 36390, 31351, 
            26430, 26551, 36510, 31471, 31590, 36631, 26550, 26431, 31470, 36511, 
            36630, 31591, 21282, 21277, 21396, 21517, 21522, 21403, 26316, 26323, 
            36396, 31357, 31362, 36403, 26556, 26437, 31476, 36517, 36636, 31597, 
            26442, 26563, 36522, 31483, 31602, 36643, 21276, 21283, 21516, 21397, 
            21402, 21523, 26322, 26317, 31356, 36397, 36402, 31363, 26436, 26557, 
            36516, 31477, 31596, 36637, 26562, 26443, 31482, 36523, 36642, 31603, 
            16134, 16135, 16158, 16183, 16182, 16159, 16254, 16255, 16494, 16375, 
            16374, 16495, 16302, 16279, 16398, 16519, 16542, 16423, 16278, 16303, 
            16518, 16399, 16422, 16543, 21174, 21175, 21222, 21199, 21198, 21223, 
            26214, 26215, 31254, 36295, 36294, 31255, 26238, 26263, 36318, 31279, 
            31302, 36343, 26262, 26239, 31278, 36319, 36342, 31303, 21294, 21295, 
            21414, 21535, 21534, 21415, 26334, 26335, 36414, 31375, 31374, 36415, 
            26574, 26455, 31494, 36535, 36654, 31615, 26454, 26575, 36534, 31495, 
            31614, 36655, 21318, 21343, 21558, 21439, 21462, 21583, 26382, 26359, 
            31398, 36439, 36462, 31423, 26478, 26599, 36558, 31519, 31638, 36679, 
            26622, 26503, 31542, 36583, 36702, 31663, 21342, 21319, 21438, 21559, 
            21582, 21463, 26358, 26383, 36438, 31399, 31422, 36463, 26598, 26479, 
            31518, 36559, 36678, 31639, 26502, 26623, 36582, 31543, 31662, 36703, 
            16140, 16147, 16188, 16165, 16170, 16195, 16266, 16261, 16380, 16501, 
            16506, 16387, 16284, 16309, 16524, 16405, 16428, 16549, 16314, 16291, 
            16410, 16531, 16554, 16435, 21186, 21181, 21204, 21229, 21234, 21211, 
            26220, 26227, 36300, 31261, 31266, 36307, 26268, 26245, 31284, 36325, 
            36348, 31309, 26250, 26275, 36330, 31291, 31314, 36355, 21300, 21307, 
            21540, 21421, 21426, 21547, 26346, 26341, 31380, 36421, 36426, 31387, 
            26460, 26581, 36540, 31501, 31620, 36661, 26586, 26467, 31506, 36547, 
            36666, 31627, 21348, 21325, 21444, 21565, 21588, 21469, 26364, 26389, 
            36444, 31405, 31428, 36469, 26604, 26485, 31524, 36565, 36684, 31645, 
            26508, 26629, 36588, 31549, 31668, 36709, 21330, 21355, 21570, 21451, 
            21474, 21595, 26394, 26371, 31410, 36451, 36474, 31435, 26490, 26611, 
            36570, 31531, 31650, 36691, 26634, 26515, 31554, 36595, 36714, 31675, 
            16146, 16141, 16164, 16189, 16194, 16171, 16260, 16267, 16500, 16381, 
            16386, 16507, 16308, 16285, 16404, 16525, 16548, 16429, 16290, 16315, 
            16530, 16411, 16434, 16555, 21180, 21187, 21228, 21205, 21210, 21235, 
            26226, 26221, 31260, 36301, 36306, 31267, 26244, 26269, 36324, 31285, 
            31308, 36349, 26274, 26251, 31290, 36331, 36354, 31315, 21306, 21301, 
            21420, 21541, 21546, 21427, 26340, 26347, 36420, 31381, 31386, 36427, 
            26580, 26461, 31500, 36541, 36660, 31621, 26466, 26587, 36546, 31507, 
            31626, 36667, 21324, 21349, 21564, 21445, 21468, 21589, 26388, 26365, 
            31404, 36445, 36468, 31429, 26484, 26605, 36564, 31525, 31644, 36685, 
            26628, 26509, 31548, 36589, 36708, 31669, 21354, 21331, 21450, 21571, 
            21594, 21475, 26370, 26395, 36450, 31411, 31434, 36475, 26610, 26491, 
            31530, 36571, 36690, 31651, 26514, 26635, 36594, 31555, 31674, 36715, 
            11670, 11671, 11676, 11683, 11682, 11677, 11694, 11695, 11742, 11719, 
            11718, 11743, 11706, 11701, 11724, 11749, 11754, 11731, 11700, 11707, 
            11748, 11725, 11730, 11755, 12390, 12391, 12402, 12397, 12396, 12403, 
            13110, 13111, 13830, 14551, 14550, 13831, 13116, 13123, 14556, 13837, 
            13842, 14563, 13122, 13117, 13836, 14557, 14562, 13843, 12414, 12415, 
            12438, 12463, 12462, 12439, 13134, 13135, 14574, 13855, 13854, 14575, 
            13182, 13159, 13878, 14599, 14622, 13903, 13158, 13183, 14598, 13879, 
            13902, 14623, 12420, 12427, 12468, 12445, 12450, 12475, 13146, 13141, 
            13860, 14581, 14586, 13867, 13164, 13189, 14604, 13885, 13908, 14629, 
            13194, 13171, 13890, 14611, 14634, 13915, 12426, 12421, 12444, 12469, 
            12474, 12451, 13140, 13147, 14580, 13861, 13866, 14587, 13188, 13165, 
            13884, 14605, 14628, 13909, 13170, 13195, 14610, 13891, 13914, 14635, 
            16710, 16711, 16722, 16717, 16716, 16723, 16734, 16735, 16758, 16783, 
            16782, 16759, 16740, 16747, 16788, 16765, 16770, 16795, 16746, 16741, 
            16764, 16789, 16794, 16771, 21750, 21751, 21756, 21763, 21762, 21757, 
            26790, 26791, 36870, 31831, 31830, 36871, 26802, 26797, 31836, 36877, 
            36882, 31843, 26796, 26803, 36876, 31837, 31842, 36883, 21774, 21775, 
            21822, 21799, 21798, 21823, 26814, 26815, 31854, 36895, 36894, 31855, 
            26838, 26863, 36918, 31879, 31902, 36943, 26862, 26839, 31878, 36919, 
            36942, 31903, 21786, 21781, 21804, 21829, 21834, 21811, 26820, 26827, 
            36900, 31861, 31866, 36907, 26868, 26845, 31884, 36925, 36948, 31909, 
            26850, 26875, 36930, 31891, 31914, 36955, 21780, 21787, 21828, 21805, 
            21810, 21835, 26826, 26821, 31860, 36901, 36906, 31867, 26844, 26869, 
            36924, 31885, 31908, 36949, 26874, 26851, 31890, 36931, 36954, 31915, 
            17430, 17431, 17436, 17443, 17442, 17437, 18150, 18151, 19590, 18871, 
            18870, 19591, 18162, 18157, 18876, 19597, 19602, 18883, 18156, 18163, 
            19596, 18877, 18882, 19603, 22470, 22471, 22482, 22477, 22476, 22483, 
            27510, 27511, 32550, 37591, 37590, 32551, 27516, 27523, 37596, 32557, 
            32562, 37603, 27522, 27517, 32556, 37597, 37602, 32563, 23190, 23191, 
            23910, 24631, 24630, 23911, 28230, 28231, 38310, 33271, 33270, 38311, 
            29670, 28951, 33990, 39031, 39750, 34711, 28950, 29671, 39030, 33991, 
            34710, 39751, 23196, 23203, 24636, 23917, 23922, 24643, 28242, 28237, 
            33276, 38317, 38322, 33283, 28956, 29677, 39036, 33997, 34716, 39757, 
            29682, 28963, 34002, 39043, 39762, 34723, 23202, 23197, 23916, 24637, 
            24642, 23923, 28236, 28243, 38316, 33277, 33282, 38323, 29676, 28957, 
            33996, 39037, 39756, 34717, 28962, 29683, 39042, 34003, 34722, 39763, 
            17454, 17455, 17502, 17479, 17478, 17503, 18174, 18175, 18894, 19615, 
            19614, 18895, 18198, 18223, 19638, 18919, 18942, 19663, 18222, 18199, 
            18918, 19639, 19662, 18943, 22494, 22495, 22518, 22543, 22542, 22519, 
            27534, 27535, 37614, 32575, 32574, 37615, 27582, 27559, 32598, 37639, 
            37662, 32623, 27558, 27583, 37638, 32599, 32622, 37663, 23214, 23215, 
            24654, 23935, 23934, 24655, 28254, 28255, 33294, 38335, 38334, 33295, 
            28974, 29695, 39054, 34015, 34734, 39775, 29694, 28975, 34014, 39055, 
            39774, 34735, 23262, 23239, 23958, 24679, 24702, 23983, 28278, 28303, 
            38358, 33319, 33342, 38383, 29718, 28999, 34038, 39079, 39798, 34759, 
            29022, 29743, 39102, 34063, 34782, 39823, 23238, 23263, 24678, 23959, 
            23982, 24703, 28302, 28279, 33318, 38359, 38382, 33343, 28998, 29719, 
            39078, 34039, 34758, 39799, 29742, 29023, 34062, 39103, 39822, 34783, 
            17466, 17461, 17484, 17509, 17514, 17491, 18180, 18187, 19620, 18901, 
            18906, 19627, 18228, 18205, 18924, 19645, 19668, 18949, 18210, 18235, 
            19650, 18931, 18954, 19675, 22500, 22507, 22548, 22525, 22530, 22555, 
            27546, 27541, 32580, 37621, 37626, 32587, 27564, 27589, 37644, 32605, 
            32628, 37669, 27594, 27571, 32610, 37651, 37674, 32635, 23226, 23221, 
            23940, 24661, 24666, 23947, 28260, 28267, 38340, 33301, 33306, 38347, 
            29700, 28981, 34020, 39061, 39780, 34741, 28986, 29707, 39066, 34027, 
            34746, 39787, 23244, 23269, 24684, 23965, 23988, 24709, 28308, 28285, 
            33324, 38365, 38388, 33349, 29004, 29725, 39084, 34045, 34764, 39805, 
            29748, 29029, 34068, 39109, 39828, 34789, 23274, 23251, 23970, 24691, 
            24714, 23995, 28290, 28315, 38370, 33331, 33354, 38395, 29730, 29011, 
            34050, 39091, 39810, 34771, 29034, 29755, 39114, 34075, 34794, 39835, 
            17460, 17467, 17508, 17485, 17490, 17515, 18186, 18181, 18900, 19621, 
            19626, 18907, 18204, 18229, 19644, 18925, 18948, 19669, 18234, 18211, 
            18930, 19651, 19674, 18955, 22506, 22501, 22524, 22549, 22554, 22531, 
            27540, 27547, 37620, 32581, 32586, 37627, 27588, 27565, 32604, 37645, 
            37668, 32629, 27570, 27595, 37650, 32611, 32634, 37675, 23220, 23227, 
            24660, 23941, 23946, 24667, 28266, 28261, 33300, 38341, 38346, 33307, 
            28980, 29701, 39060, 34021, 34740, 39781, 29706, 28987, 34026, 39067, 
            39786, 34747, 23268, 23245, 23964, 24685, 24708, 23989, 28284, 28309, 
            38364, 33325, 33348, 38389, 29724, 29005, 34044, 39085, 39804, 34765, 
            29028, 29749, 39108, 34069, 34788, 39829, 23250, 23275, 24690, 23971, 
            23994, 24715, 28314, 28291, 33330, 38371, 38394, 33355, 29010, 29731, 
            39090, 34051, 34770, 39811, 29754, 29035, 34074, 39115, 39834, 34795, 
            11790, 11791, 11802, 11797, 11796, 11803, 11910, 11911, 12030, 12151, 
            12150, 12031, 11916, 11923, 12156, 12037, 12042, 12163, 11922, 11917, 
            12036, 12157, 12162, 12043, 12510, 12511, 12516, 12523, 12522, 12517, 
            13230, 13231, 14670, 13951, 13950, 14671, 13242, 13237, 13956, 14677, 
            14682, 13963, 13236, 13243, 14676, 13957, 13962, 14683, 12630, 12631, 
            12870, 12751, 12750, 12871, 13350, 13351, 14070, 14791, 14790, 14071, 
            13470, 13591, 14910, 14191, 14310, 15031, 13590, 13471, 14190, 14911, 
            15030, 14311, 12642, 12637, 12756, 12877, 12882, 12763, 13356, 13363, 
            14796, 14077, 14082, 14803, 13596, 13477, 14196, 14917, 15036, 14317, 
            13482, 13603, 14922, 14203, 14322, 15043, 12636, 12643, 12876, 12757, 
            12762, 12883, 13362, 13357, 14076, 14797, 14802, 14083, 13476, 13597, 
            14916, 14197, 14316, 15037, 13602, 13483, 14202, 14923, 15042, 14323, 
            16830, 16831, 16836, 16843, 16842, 16837, 16950, 16951, 17190, 17071, 
            17070, 17191, 16962, 16957, 17076, 17197, 17202, 17083, 16956, 16963, 
            17196, 17077, 17082, 17203, 21870, 21871, 21882, 21877, 21876, 21883, 
            26910, 26911, 31950, 36991, 36990, 31951, 26916, 26923, 36996, 31957, 
            31962, 37003, 26922, 26917, 31956, 36997, 37002, 31963, 21990, 21991, 
            22110, 22231, 22230, 22111, 27030, 27031, 37110, 32071, 32070, 37111, 
            27270, 27151, 32190, 37231, 37350, 32311, 27150, 27271, 37230, 32191, 
            32310, 37351, 21996, 22003, 22236, 22117, 22122, 22243, 27042, 27037, 
            32076, 37117, 37122, 32083, 27156, 27277, 37236, 32197, 32316, 37357, 
            27282, 27163, 32202, 37243, 37362, 32323, 22002, 21997, 22116, 22237, 
            22242, 22123, 27036, 27043, 37116, 32077, 32082, 37123, 27276, 27157, 
            32196, 37237, 37356, 32317, 27162, 27283, 37242, 32203, 32322, 37363, 
            17550, 17551, 17562, 17557, 17556, 17563, 18270, 18271, 18990, 19711, 
            19710, 18991, 18276, 18283, 19716, 18997, 19002, 19723, 18282, 18277, 
            18996, 19717, 19722, 19003, 22590, 22591, 22596, 22603, 22602, 22597, 
            27630, 27631, 37710, 32671, 32670, 37711, 27642, 27637, 32676, 37717, 
            37722, 32683, 27636, 27643, 37716, 32677, 32682, 37723, 23310, 23311, 
            24750, 24031, 24030, 24751, 28350, 28351, 33390, 38431, 38430, 33391, 
            29070, 29791, 39150, 34111, 34830, 39871, 29790, 29071, 34110, 39151, 
            39870, 34831, 23322, 23317, 24036, 24757, 24762, 24043, 28356, 28363, 
            38436, 33397, 33402, 38443, 29796, 29077, 34116, 39157, 39876, 34837, 
            29082, 29803, 39162, 34123, 34842, 39883, 23316, 23323, 24756, 24037, 
            24042, 24763, 28362, 28357, 33396, 38437, 38442, 33403, 29076, 29797, 
            39156, 34117, 34836, 39877, 29802, 29083, 34122, 39163, 39882, 34843, 
            17670, 17671, 17790, 17911, 17910, 17791, 18390, 18391, 19830, 19111, 
            19110, 19831, 18630, 18511, 19230, 19951, 20070, 19351, 18510, 18631, 
            19950, 19231, 19350, 20071, 22710, 22711, 22950, 22831, 22830, 22951, 
            27750, 27751, 32790, 37831, 37830, 32791, 27870, 27991, 37950, 32911, 
            33030, 38071, 27990, 27871, 32910, 37951, 38070, 33031, 23430, 23431, 
            24150, 24871, 24870, 24151, 28470, 28471, 38550, 33511, 33510, 38551, 
            29910, 29191, 34230, 39271, 39990, 34951, 29190, 29911, 39270, 34231, 
            34950, 39991, 23550, 23671, 24990, 24271, 24390, 25111, 28710, 28591, 
            33630, 38671, 38790, 33751, 29310, 30031, 39390, 34351, 35070, 40111, 
            30150, 29431, 34470, 39511, 40230, 35191, 23670, 23551, 24270, 24991, 
            25110, 24391, 28590, 28711, 38670, 33631, 33750, 38791, 30030, 29311, 
            34350, 39391, 40110, 35071, 29430, 30151, 39510, 34471, 35190, 40231, 
            17676, 17683, 17916, 17797, 17802, 17923, 18402, 18397, 19116, 19837, 
            19842, 19123, 18516, 18637, 19956, 19237, 19356, 20077, 18642, 18523, 
            19242, 19963, 20082, 19363, 22722, 22717, 22836, 22957, 22962, 22843, 
            27756, 27763, 37836, 32797, 32802, 37843, 27996, 27877, 32916, 37957, 
            38076, 33037, 27882, 28003, 37962, 32923, 33042, 38083, 23436, 23443, 
            24876, 24157, 24162, 24883, 28482, 28477, 33516, 38557, 38562, 33523, 
            29196, 29917, 39276, 34237, 34956, 39997, 29922, 29203, 34242, 39283, 
            40002, 34963, 23676, 23557, 24276, 24997, 25116, 24397, 28596, 28717, 
            38676, 33637, 33756, 38797, 30036, 29317, 34356, 39397, 40116, 35077, 
            29436, 30157, 39516, 34477, 35196, 40237, 23562, 23683, 25002, 24283, 
            24402, 25123, 28722, 28603, 33642, 38683, 38802, 33763, 29322, 30043, 
            39402, 34363, 35082, 40123, 30162, 29443, 34482, 39523, 40242, 35203, 
            17682, 17677, 17796, 17917, 17922, 17803, 18396, 18403, 19836, 19117, 
            19122, 19843, 18636, 18517, 19236, 19957, 20076, 19357, 18522, 18643, 
            19962, 19243, 19362, 20083, 22716, 22723, 22956, 22837, 22842, 22963, 
            27762, 27757, 32796, 37837, 37842, 32803, 27876, 27997, 37956, 32917, 
            33036, 38077, 28002, 27883, 32922, 37963, 38082, 33043, 23442, 23437, 
            24156, 24877, 24882, 24163, 28476, 28483, 38556, 33517, 33522, 38563, 
            29916, 29197, 34236, 39277, 39996, 34957, 29202, 29923, 39282, 34243, 
            34962, 40003, 23556, 23677, 24996, 24277, 24396, 25117, 28716, 28597, 
            33636, 38677, 38796, 33757, 29316, 30037, 39396, 34357, 35076, 40117, 
            30156, 29437, 34476, 39517, 40236, 35197, 23682, 23563, 24282, 25003, 
            25122, 24403, 28602, 28723, 38682, 33643, 33762, 38803, 30042, 29323, 
            34362, 39403, 40122, 35083, 29442, 30163, 39522, 34483, 35202, 40243, 
            11814, 11815, 11838, 11863, 11862, 11839, 11934, 11935, 12174, 12055, 
            12054, 12175, 11982, 11959, 12078, 12199, 12222, 12103, 11958, 11983, 
            12198, 12079, 12102, 12223, 12534, 12535, 12582, 12559, 12558, 12583, 
            13254, 13255, 13974, 14695, 14694, 13975, 13278, 13303, 14718, 13999, 
            14022, 14743, 13302, 13279, 13998, 14719, 14742, 14023, 12654, 12655, 
            12774, 12895, 12894, 12775, 13374, 13375, 14814, 14095, 14094, 14815, 
            13614, 13495, 14214, 14935, 15054, 14335, 13494, 13615, 14934, 14215, 
            14334, 15055, 12678, 12703, 12918, 12799, 12822, 12943, 13422, 13399, 
            14118, 14839, 14862, 14143, 13518, 13639, 14958, 14239, 14358, 15079, 
            13662, 13543, 14262, 14983, 15102, 14383, 12702, 12679, 12798, 12919, 
            12942, 12823, 13398, 13423, 14838, 14119, 14142, 14863, 13638, 13519, 
            14238, 14959, 15078, 14359, 13542, 13663, 14982, 14263, 14382, 15103, 
            16854, 16855, 16902, 16879, 16878, 16903, 16974, 16975, 17094, 17215, 
            17214, 17095, 16998, 17023, 17238, 17119, 17142, 17263, 17022, 16999, 
            17118, 17239, 17262, 17143, 21894, 21895, 21918, 21943, 21942, 21919, 
            26934, 26935, 37014, 31975, 31974, 37015, 26982, 26959, 31998, 37039, 
            37062, 32023, 26958, 26983, 37038, 31999, 32022, 37063, 22014, 22015, 
            22254, 22135, 22134, 22255, 27054, 27055, 32094, 37135, 37134, 32095, 
            27174, 27295, 37254, 32215, 32334, 37375, 27294, 27175, 32214, 37255, 
            37374, 32335, 22062, 22039, 22158, 22279, 22302, 22183, 27078, 27103, 
            37158, 32119, 32142, 37183, 27318, 27199, 32238, 37279, 37398, 32359, 
            27222, 27343, 37302, 32263, 32382, 37423, 22038, 22063, 22278, 22159, 
            22182, 22303, 27102, 27079, 32118, 37159, 37182, 32143, 27198, 27319, 
            37278, 32239, 32358, 37399, 27342, 27223, 32262, 37303, 37422, 32383, 
            17574, 17575, 17598, 17623, 17622, 17599, 18294, 18295, 19734, 19015, 
            19014, 19735, 18342, 18319, 19038, 19759, 19782, 19063, 18318, 18343, 
            19758, 19039, 19062, 19783, 22614, 22615, 22662, 22639, 22638, 22663, 
            27654, 27655, 32694, 37735, 37734, 32695, 27678, 27703, 37758, 32719, 
            32742, 37783, 27702, 27679, 32718, 37759, 37782, 32743, 23334, 23335, 
            24054, 24775, 24774, 24055, 28374, 28375, 38454, 33415, 33414, 38455, 
            29814, 29095, 34134, 39175, 39894, 34855, 29094, 29815, 39174, 34135, 
            34854, 39895, 23358, 23383, 24798, 24079, 24102, 24823, 28422, 28399, 
            33438, 38479, 38502, 33463, 29118, 29839, 39198, 34159, 34878, 39919, 
            29862, 29143, 34182, 39223, 39942, 34903, 23382, 23359, 24078, 24799, 
            24822, 24103, 28398, 28423, 38478, 33439, 33462, 38503, 29838, 29119, 
            34158, 39199, 39918, 34879, 29142, 29863, 39222, 34183, 34902, 39943, 
            17694, 17695, 17934, 17815, 17814, 17935, 18414, 18415, 19134, 19855, 
            19854, 19135, 18534, 18655, 19974, 19255, 19374, 20095, 18654, 18535, 
            19254, 19975, 20094, 19375, 22734, 22735, 22854, 22975, 22974, 22855, 
            27774, 27775, 37854, 32815, 32814, 37855, 28014, 27895, 32934, 37975, 
            38094, 33055, 27894, 28015, 37974, 32935, 33054, 38095, 23454, 23455, 
            24894, 24175, 24174, 24895, 28494, 28495, 33534, 38575, 38574, 33535, 
            29214, 29935, 39294, 34255, 34974, 40015, 29934, 29215, 34254, 39295, 
            40014, 34975, 23694, 23575, 24294, 25015, 25134, 24415, 28614, 28735, 
            38694, 33655, 33774, 38815, 30054, 29335, 34374, 39415, 40134, 35095, 
            29454, 30175, 39534, 34495, 35214, 40255, 23574, 23695, 25014, 24295, 
            24414, 25135, 28734, 28615, 33654, 38695, 38814, 33775, 29334, 30055, 
            39414, 34375, 35094, 40135, 30174, 29455, 34494, 39535, 40254, 35215, 
            17742, 17719, 17838, 17959, 17982, 17863, 18438, 18463, 19878, 19159, 
            19182, 19903, 18678, 18559, 19278, 19999, 20118, 19399, 18582, 18703, 
            20022, 19303, 19422, 20143, 22758, 22783, 22998, 22879, 22902, 23023, 
            27822, 27799, 32838, 37879, 37902, 32863, 27918, 28039, 37998, 32959, 
            33078, 38119, 28062, 27943, 32982, 38023, 38142, 33103, 23502, 23479, 
            24198, 24919, 24942, 24223, 28518, 28543, 38598, 33559, 33582, 38623, 
            29958, 29239, 34278, 39319, 40038, 34999, 29262, 29983, 39342, 34303, 
            35022, 40063, 23598, 23719, 25038, 24319, 24438, 25159, 28758, 28639, 
            33678, 38719, 38838, 33799, 29358, 30079, 39438, 34399, 35118, 40159, 
            30198, 29479, 34518, 39559, 40278, 35239, 23742, 23623, 24342, 25063, 
            25182, 24463, 28662, 28783, 38742, 33703, 33822, 38863, 30102, 29383, 
            34422, 39463, 40182, 35143, 29502, 30223, 39582, 34543, 35262, 40303, 
            17718, 17743, 17958, 17839, 17862, 17983, 18462, 18439, 19158, 19879, 
            19902, 19183, 18558, 18679, 19998, 19279, 19398, 20119, 18702, 18583, 
            19302, 20023, 20142, 19423, 22782, 22759, 22878, 22999, 23022, 22903, 
            27798, 27823, 37878, 32839, 32862, 37903, 28038, 27919, 32958, 37999, 
            38118, 33079, 27942, 28063, 38022, 32983, 33102, 38143, 23478, 23503, 
            24918, 24199, 24222, 24943, 28542, 28519, 33558, 38599, 38622, 33583, 
            29238, 29959, 39318, 34279, 34998, 40039, 29982, 29263, 34302, 39343, 
            40062, 35023, 23718, 23599, 24318, 25039, 25158, 24439, 28638, 28759, 
            38718, 33679, 33798, 38839, 30078, 29359, 34398, 39439, 40158, 35119, 
            29478, 30199, 39558, 34519, 35238, 40279, 23622, 23743, 25062, 24343, 
            24462, 25183, 28782, 28663, 33702, 38743, 38862, 33823, 29382, 30103, 
            39462, 34423, 35142, 40183, 30222, 29503, 34542, 39583, 40302, 35263, 
            11820, 11827, 11868, 11845, 11850, 11875, 11946, 11941, 12060, 12181, 
            12186, 12067, 11964, 11989, 12204, 12085, 12108, 12229, 11994, 11971, 
            12090, 12211, 12234, 12115, 12546, 12541, 12564, 12589, 12594, 12571, 
            13260, 13267, 14700, 13981, 13986, 14707, 13308, 13285, 14004, 14725, 
            14748, 14029, 13290, 13315, 14730, 14011, 14034, 14755, 12660, 12667, 
            12900, 12781, 12786, 12907, 13386, 13381, 14100, 14821, 14826, 14107, 
            13500, 13621, 14940, 14221, 14340, 15061, 13626, 13507, 14226, 14947, 
            15066, 14347, 12708, 12685, 12804, 12925, 12948, 12829, 13404, 13429, 
            14844, 14125, 14148, 14869, 13644, 13525, 14244, 14965, 15084, 14365, 
            13548, 13669, 14988, 14269, 14388, 15109, 12690, 12715, 12930, 12811, 
            12834, 12955, 13434, 13411, 14130, 14851, 14874, 14155, 13530, 13651, 
            14970, 14251, 14370, 15091, 13674, 13555, 14274, 14995, 15114, 14395, 
            16866, 16861, 16884, 16909, 16914, 16891, 16980, 16987, 17220, 17101, 
            17106, 17227, 17028, 17005, 17124, 17245, 17268, 17149, 17010, 17035, 
            17250, 17131, 17154, 17275, 21900, 21907, 21948, 21925, 21930, 21955, 
            26946, 26941, 31980, 37021, 37026, 31987, 26964, 26989, 37044, 32005, 
            32028, 37069, 26994, 26971, 32010, 37051, 37074, 32035, 22026, 22021, 
            22140, 22261, 22266, 22147, 27060, 27067, 37140, 32101, 32106, 37147, 
            27300, 27181, 32220, 37261, 37380, 32341, 27186, 27307, 37266, 32227, 
            32346, 37387, 22044, 22069, 22284, 22165, 22188, 22309, 27108, 27085, 
            32124, 37165, 37188, 32149, 27204, 27325, 37284, 32245, 32364, 37405, 
            27348, 27229, 32268, 37309, 37428, 32389, 22074, 22051, 22170, 22291, 
            22314, 22195, 27090, 27115, 37170, 32131, 32154, 37195, 27330, 27211, 
            32250, 37291, 37410, 32371, 27234, 27355, 37314, 32275, 32394, 37435, 
            17580, 17587, 17628, 17605, 17610, 17635, 18306, 18301, 19020, 19741, 
            19746, 19027, 18324, 18349, 19764, 19045, 19068, 19789, 18354, 18331, 
            19050, 19771, 19794, 19075, 22626, 22621, 22644, 22669, 22674, 22651, 
            27660, 27667, 37740, 32701, 32706, 37747, 27708, 27685, 32724, 37765, 
            37788, 32749, 27690, 27715, 37770, 32731, 32754, 37795, 23340, 23347, 
            24780, 24061, 24066, 24787, 28386, 28381, 33420, 38461, 38466, 33427, 
            29100, 29821, 39180, 34141, 34860, 39901, 29826, 29107, 34146, 39187, 
            39906, 34867, 23388, 23365, 24084, 24805, 24828, 24109, 28404, 28429, 
            38484, 33445, 33468, 38509, 29844, 29125, 34164, 39205, 39924, 34885, 
            29148, 29869, 39228, 34189, 34908, 39949, 23370, 23395, 24810, 24091, 
            24114, 24835, 28434, 28411, 33450, 38491, 38514, 33475, 29130, 29851, 
            39210, 34171, 34890, 39931, 29874, 29155, 34194, 39235, 39954, 34915, 
            17706, 17701, 17820, 17941, 17946, 17827, 18420, 18427, 19860, 19141, 
            19146, 19867, 18660, 18541, 19260, 19981, 20100, 19381, 18546, 18667, 
            19986, 19267, 19386, 20107, 22740, 22747, 22980, 22861, 22866, 22987, 
            27786, 27781, 32820, 37861, 37866, 32827, 27900, 28021, 37980, 32941, 
            33060, 38101, 28026, 27907, 32946, 37987, 38106, 33067, 23466, 23461, 
            24180, 24901, 24906, 24187, 28500, 28507, 38580, 33541, 33546, 38587, 
            29940, 29221, 34260, 39301, 40020, 34981, 29226, 29947, 39306, 34267, 
            34986, 40027, 23580, 23701, 25020, 24301, 24420, 25141, 28740, 28621, 
            33660, 38701, 38820, 33781, 29340, 30061, 39420, 34381, 35100, 40141, 
            30180, 29461, 34500, 39541, 40260, 35221, 23706, 23587, 24306, 25027, 
            25146, 24427, 28626, 28747, 38706, 33667, 33786, 38827, 30066, 29347, 
            34386, 39427, 40146, 35107, 29466, 30187, 39546, 34507, 35226, 40267, 
            17724, 17749, 17964, 17845, 17868, 17989, 18468, 18445, 19164, 19885, 
            19908, 19189, 18564, 18685, 20004, 19285, 19404, 20125, 18708, 18589, 
            19308, 20029, 20148, 19429, 22788, 22765, 22884, 23005, 23028, 22909, 
            27804, 27829, 37884, 32845, 32868, 37909, 28044, 27925, 32964, 38005, 
            38124, 33085, 27948, 28069, 38028, 32989, 33108, 38149, 23484, 23509, 
            24924, 24205, 24228, 24949, 28548, 28525, 33564, 38605, 38628, 33589, 
            29244, 29965, 39324, 34285, 35004, 40045, 29988, 29269, 34308, 39349, 
            40068, 35029, 23724, 23605, 24324, 25045, 25164, 24445, 28644, 28765, 
            38724, 33685, 33804, 38845, 30084, 29365, 34404, 39445, 40164, 35125, 
            29484, 30205, 39564, 34525, 35244, 40285, 23628, 23749, 25068, 24349, 
            24468, 25189, 28788, 28669, 33708, 38749, 38868, 33829, 29388, 30109, 
            39468, 34429, 35148, 40189, 30228, 29509, 34548, 39589, 40308, 35269, 
            17754, 17731, 17850, 17971, 17994, 17875, 18450, 18475, 19890, 19171, 
            19194, 19915, 18690, 18571, 19290, 20011, 20130, 19411, 18594, 18715, 
            20034, 19315, 19434, 20155, 22770, 22795, 23010, 22891, 22914, 23035, 
            27834, 27811, 32850, 37891, 37914, 32875, 27930, 28051, 38010, 32971, 
            33090, 38131, 28074, 27955, 32994, 38035, 38154, 33115, 23514, 23491, 
            24210, 24931, 24954, 24235, 28530, 28555, 38610, 33571, 33594, 38635, 
            29970, 29251, 34290, 39331, 40050, 35011, 29274, 29995, 39354, 34315, 
            35034, 40075, 23610, 23731, 25050, 24331, 24450, 25171, 28770, 28651, 
            33690, 38731, 38850, 33811, 29370, 30091, 39450, 34411, 35130, 40171, 
            30210, 29491, 34530, 39571, 40290, 35251, 23754, 23635, 24354, 25075, 
            25194, 24475, 28674, 28795, 38754, 33715, 33834, 38875, 30114, 29395, 
            34434, 39475, 40194, 35155, 29514, 30235, 39594, 34555, 35274, 40315, 
            11826, 11821, 11844, 11869, 11874, 11851, 11940, 11947, 12180, 12061, 
            12066, 12187, 11988, 11965, 12084, 12205, 12228, 12109, 11970, 11995, 
            12210, 12091, 12114, 12235, 12540, 12547, 12588, 12565, 12570, 12595, 
            13266, 13261, 13980, 14701, 14706, 13987, 13284, 13309, 14724, 14005, 
            14028, 14749, 13314, 13291, 14010, 14731, 14754, 14035, 12666, 12661, 
            12780, 12901, 12906, 12787, 13380, 13387, 14820, 14101, 14106, 14827, 
            13620, 13501, 14220, 14941, 15060, 14341, 13506, 13627, 14946, 14227, 
            14346, 15067, 12684, 12709, 12924, 12805, 12828, 12949, 13428, 13405, 
            14124, 14845, 14868, 14149, 13524, 13645, 14964, 14245, 14364, 15085, 
            13668, 13549, 14268, 14989, 15108, 14389, 12714, 12691, 12810, 12931, 
            12954, 12835, 13410, 13435, 14850, 14131, 14154, 14875, 13650, 13531, 
            14250, 14971, 15090, 14371, 13554, 13675, 14994, 14275, 14394, 15115, 
            16860, 16867, 16908, 16885, 16890, 16915, 16986, 16981, 17100, 17221, 
            17226, 17107, 17004, 17029, 17244, 17125, 17148, 17269, 17034, 17011, 
            17130, 17251, 17274, 17155, 21906, 21901, 21924, 21949, 21954, 21931, 
            26940, 26947, 37020, 31981, 31986, 37027, 26988, 26965, 32004, 37045, 
            37068, 32029, 26970, 26995, 37050, 32011, 32034, 37075, 22020, 22027, 
            22260, 22141, 22146, 22267, 27066, 27061, 32100, 37141, 37146, 32107, 
            27180, 27301, 37260, 32221, 32340, 37381, 27306, 27187, 32226, 37267, 
            37386, 32347, 22068, 22045, 22164, 22285, 22308, 22189, 27084, 27109, 
            37164, 32125, 32148, 37189, 27324, 27205, 32244, 37285, 37404, 32365, 
            27228, 27349, 37308, 32269, 32388, 37429, 22050, 22075, 22290, 22171, 
            22194, 22315, 27114, 27091, 32130, 37171, 37194, 32155, 27210, 27331, 
            37290, 32251, 32370, 37411, 27354, 27235, 32274, 37315, 37434, 32395, 
            17586, 17581, 17604, 17629, 17634, 17611, 18300, 18307, 19740, 19021, 
            19026, 19747, 18348, 18325, 19044, 19765, 19788, 19069, 18330, 18355, 
            19770, 19051, 19074, 19795, 22620, 22627, 22668, 22645, 22650, 22675, 
            27666, 27661, 32700, 37741, 37746, 32707, 27684, 27709, 37764, 32725, 
            32748, 37789, 27714, 27691, 32730, 37771, 37794, 32755, 23346, 23341, 
            24060, 24781, 24786, 24067, 28380, 28387, 38460, 33421, 33426, 38467, 
            29820, 29101, 34140, 39181, 39900, 34861, 29106, 29827, 39186, 34147, 
            34866, 39907, 23364, 23389, 24804, 24085, 24108, 24829, 28428, 28405, 
            33444, 38485, 38508, 33469, 29124, 29845, 39204, 34165, 34884, 39925, 
            29868, 29149, 34188, 39229, 39948, 34909, 23394, 23371, 24090, 24811, 
            24834, 24115, 28410, 28435, 38490, 33451, 33474, 38515, 29850, 29131, 
            34170, 39211, 39930, 34891, 29154, 29875, 39234, 34195, 34914, 39955, 
            17700, 17707, 17940, 17821, 17826, 17947, 18426, 18421, 19140, 19861, 
            19866, 19147, 18540, 18661, 19980, 19261, 19380, 20101, 18666, 18547, 
            19266, 19987, 20106, 19387, 22746, 22741, 22860, 22981, 22986, 22867, 
            27780, 27787, 37860, 32821, 32826, 37867, 28020, 27901, 32940, 37981, 
            38100, 33061, 27906, 28027, 37986, 32947, 33066, 38107, 23460, 23467, 
            24900, 24181, 24186, 24907, 28506, 28501, 33540, 38581, 38586, 33547, 
            29220, 29941, 39300, 34261, 34980, 40021, 29946, 29227, 34266, 39307, 
            40026, 34987, 23700, 23581, 24300, 25021, 25140, 24421, 28620, 28741, 
            38700, 33661, 33780, 38821, 30060, 29341, 34380, 39421, 40140, 35101, 
            29460, 30181, 39540, 34501, 35220, 40261, 23586, 23707, 25026, 24307, 
            24426, 25147, 28746, 28627, 33666, 38707, 38826, 33787, 29346, 30067, 
            39426, 34387, 35106, 40147, 30186, 29467, 34506, 39547, 40266, 35227, 
            17748, 17725, 17844, 17965, 17988, 17869, 18444, 18469, 19884, 19165, 
            19188, 19909, 18684, 18565, 19284, 20005, 20124, 19405, 18588, 18709, 
            20028, 19309, 19428, 20149, 22764, 22789, 23004, 22885, 22908, 23029, 
            27828, 27805, 32844, 37885, 37908, 32869, 27924, 28045, 38004, 32965, 
            33084, 38125, 28068, 27949, 32988, 38029, 38148, 33109, 23508, 23485, 
            24204, 24925, 24948, 24229, 28524, 28549, 38604, 33565, 33588, 38629, 
            29964, 29245, 34284, 39325, 40044, 35005, 29268, 29989, 39348, 34309, 
            35028, 40069, 23604, 23725, 25044, 24325, 24444, 25165, 28764, 28645, 
            33684, 38725, 38844, 33805, 29364, 30085, 39444, 34405, 35124, 40165, 
            30204, 29485, 34524, 39565, 40284, 35245, 23748, 23629, 24348, 25069, 
            25188, 24469, 28668, 28789, 38748, 33709, 33828, 38869, 30108, 29389, 
            34428, 39469, 40188, 35149, 29508, 30229, 39588, 34549, 35268, 40309, 
            17730, 17755, 17970, 17851, 17874, 17995, 18474, 18451, 19170, 19891, 
            19914, 19195, 18570, 18691, 20010, 19291, 19410, 20131, 18714, 18595, 
            19314, 20035, 20154, 19435, 22794, 22771, 22890, 23011, 23034, 22915, 
            27810, 27835, 37890, 32851, 32874, 37915, 28050, 27931, 32970, 38011, 
            38130, 33091, 27954, 28075, 38034, 32995, 33114, 38155, 23490, 23515, 
            24930, 24211, 24234, 24955, 28554, 28531, 33570, 38611, 38634, 33595, 
            29250, 29971, 39330, 34291, 35010, 40051, 29994, 29275, 34314, 39355, 
            40074, 35035, 23730, 23611, 24330, 25051, 25170, 24451, 28650, 28771, 
            38730, 33691, 33810, 38851, 30090, 29371, 34410, 39451, 40170, 35131, 
            29490, 30211, 39570, 34531, 35250, 40291, 23634, 23755, 25074, 24355, 
            24474, 25195, 28794, 28675, 33714, 38755, 38874, 33835, 29394, 30115, 
            39474, 34435, 35154, 40195, 30234, 29515, 34554, 39595, 40314, 35275, 
            5912, 5915, 5924, 5919, 5920, 5927, 5938, 5937, 5960, 5985, 
            5986, 5963, 5942, 5949, 5990, 5967, 5972, 5997, 5950, 5945, 
            5968, 5993, 5998, 5975, 6034, 6033, 6038, 6045, 6046, 6041, 
            6152, 6155, 6392, 6273, 6274, 6395, 6164, 6159, 6278, 6399, 
            6404, 6285, 6160, 6167, 6400, 6281, 6286, 6407, 6056, 6059, 
            6104, 6081, 6082, 6107, 6178, 6177, 6296, 6417, 6418, 6299, 
            6200, 6225, 6440, 6321, 6344, 6465, 6226, 6203, 6322, 6443, 
            6466, 6347, 6068, 6063, 6086, 6111, 6116, 6093, 6182, 6189, 
            6422, 6303, 6308, 6429, 6230, 6207, 6326, 6447, 6470, 6351, 
            6212, 6237, 6452, 6333, 6356, 6477, 6064, 6071, 6112, 6089, 
            6094, 6119, 6190, 6185, 6304, 6425, 6430, 6311, 6208, 6233, 
            6448, 6329, 6352, 6473, 6238, 6215, 6334, 6455, 6478, 6359, 
            6634, 6633, 6638, 6645, 6646, 6641, 6656, 6659, 6704, 6681, 
            6682, 6707, 6668, 6663, 6686, 6711, 6716, 6693, 6664, 6671, 
            6712, 6689, 6694, 6719, 7352, 7355, 7364, 7359, 7360, 7367, 
            8074, 8073, 8792, 9513, 9514, 8795, 8078, 8085, 9518, 8799, 
            8804, 9525, 8086, 8081, 8800, 9521, 9526, 8807, 7378, 7377, 
            7400, 7425, 7426, 7403, 8096, 8099, 9536, 8817, 8818, 9539, 
            8144, 8121, 8840, 9561, 9584, 8865, 8122, 8147, 9562, 8843, 
            8866, 9587, 7382, 7389, 7430, 7407, 7412, 7437, 8108, 8103, 
            8822, 9543, 9548, 8829, 8126, 8151, 9566, 8847, 8870, 9591, 
            8156, 8133, 8852, 9573, 9596, 8877, 7390, 7385, 7408, 7433, 
            7438, 7415, 8104, 8111, 9544, 8825, 8830, 9551, 8152, 8129, 
            8848, 9569, 9592, 8873, 8134, 8159, 9574, 8855, 8878, 9599, 
            6752, 6755, 6764, 6759, 6760, 6767, 6874, 6873, 6992, 7113, 
            7114, 6995, 6878, 6885, 7118, 6999, 7004, 7125, 6886, 6881, 
            7000, 7121, 7126, 7007, 7474, 7473, 7478, 7485, 7486, 7481, 
            8192, 8195, 9632, 8913, 8914, 9635, 8204, 8199, 8918, 9639, 
            9644, 8925, 8200, 8207, 9640, 8921, 8926, 9647, 7592, 7595, 
            7832, 7713, 7714, 7835, 8314, 8313, 9032, 9753, 9754, 9035, 
            8432, 8553, 9872, 9153, 9272, 9993, 8554, 8435, 9154, 9875, 
            9994, 9275, 7604, 7599, 7718, 7839, 7844, 7725, 8318, 8325, 
            9758, 9039, 9044, 9765, 8558, 8439, 9158, 9879, 9998, 9279, 
            8444, 8565, 9884, 9165, 9284, 10005, 7600, 7607, 7840, 7721, 
            7726, 7847, 8326, 8321, 9040, 9761, 9766, 9047, 8440, 8561, 
            9880, 9161, 9280, 10001, 8566, 8447, 9166, 9887, 10006, 9287, 
            6778, 6777, 6800, 6825, 6826, 6803, 6896, 6899, 7136, 7017, 
            7018, 7139, 6944, 6921, 7040, 7161, 7184, 7065, 6922, 6947, 
            7162, 7043, 7066, 7187, 7496, 7499, 7544, 7521, 7522, 7547, 
            8218, 8217, 8936, 9657, 9658, 8939, 8240, 8265, 9680, 8961, 
            8984, 9705, 8266, 8243, 8962, 9683, 9706, 8987, 7618, 7617, 
            7736, 7857, 7858, 7739, 8336, 8339, 9776, 9057, 9058, 9779, 
            8576, 8457, 9176, 9897, 10016, 9297, 8458, 8579, 9898, 9179, 
            9298, 10019, 7640, 7665, 7880, 7761, 7784, 7905, 8384, 8361, 
            9080, 9801, 9824, 9105, 8480, 8601, 9920, 9201, 9320, 10041, 
            8624, 8505, 9224, 9945, 10064, 9345, 7666, 7643, 7762, 7883, 
            7906, 7787, 8362, 8387, 9802, 9083, 9106, 9827, 8602, 8483, 
            9202, 9923, 10042, 9323, 8506, 8627, 9946, 9227, 9346, 10067, 
            6782, 6789, 6830, 6807, 6812, 6837, 6908, 6903, 7022, 7143, 
            7148, 7029, 6926, 6951, 7166, 7047, 7070, 7191, 6956, 6933, 
            7052, 7173, 7196, 7077, 7508, 7503, 7526, 7551, 7556, 7533, 
            8222, 8229, 9662, 8943, 8948, 9669, 8270, 8247, 8966, 9687, 
            9710, 8991, 8252, 8277, 9692, 8973, 8996, 9717, 7622, 7629, 
            7862, 7743, 7748, 7869, 8348, 8343, 9062, 9783, 9788, 9069, 
            8462, 8583, 9902, 9183, 9302, 10023, 8588, 8469, 9188, 9909, 
            10028, 9309, 7670, 7647, 7766, 7887, 7910, 7791, 8366, 8391, 
            9806, 9087, 9110, 9831, 8606, 8487, 9206, 9927, 10046, 9327, 
            8510, 8631, 9950, 9231, 9350, 10071, 7652, 7677, 7892, 7773, 
            7796, 7917, 8396, 8373, 9092, 9813, 9836, 9117, 8492, 8613, 
            9932, 9213, 9332, 10053, 8636, 8517, 9236, 9957, 10076, 9357, 
            6790, 6785, 6808, 6833, 6838, 6815, 6904, 6911, 7144, 7025, 
            7030, 7151, 6952, 6929, 7048, 7169, 7192, 7073, 6934, 6959, 
            7174, 7055, 7078, 7199, 7504, 7511, 7552, 7529, 7534, 7559, 
            8230, 8225, 8944, 9665, 9670, 8951, 8248, 8273, 9688, 8969, 
            8992, 9713, 8278, 8255, 8974, 9695, 9718, 8999, 7630, 7625, 
            7744, 7865, 7870, 7751, 8344, 8351, 9784, 9065, 9070, 9791, 
            8584, 8465, 9184, 9905, 10024, 9305, 8470, 8591, 9910, 9191, 
            9310, 10031, 7648, 7673, 7888, 7769, 7792, 7913, 8392, 8369, 
            9088, 9809, 9832, 9113, 8488, 8609, 9928, 9209, 9328, 10049, 
            8632, 8513, 9232, 9953, 10072, 9353, 7678, 7655, 7774, 7895, 
            7918, 7799, 8374, 8399, 9814, 9095, 9118, 9839, 8614, 8495, 
            9214, 9935, 10054, 9335, 8518, 8639, 9958, 9239, 9358, 10079, 
            10954, 10953, 10958, 10965, 10966, 10961, 10976, 10979, 11024, 11001, 
            11002, 11027, 10988, 10983, 11006, 11031, 11036, 11013, 10984, 10991, 
            11032, 11009, 11014, 11039, 11072, 11075, 11084, 11079, 11080, 11087, 
            11194, 11193, 11312, 11433, 11434, 11315, 11198, 11205, 11438, 11319, 
            11324, 11445, 11206, 11201, 11320, 11441, 11446, 11327, 11098, 11097, 
            11120, 11145, 11146, 11123, 11216, 11219, 11456, 11337, 11338, 11459, 
            11264, 11241, 11360, 11481, 11504, 11385, 11242, 11267, 11482, 11363, 
            11386, 11507, 11102, 11109, 11150, 11127, 11132, 11157, 11228, 11223, 
            11342, 11463, 11468, 11349, 11246, 11271, 11486, 11367, 11390, 11511, 
            11276, 11253, 11372, 11493, 11516, 11397, 11110, 11105, 11128, 11153, 
            11158, 11135, 11224, 11231, 11464, 11345, 11350, 11471, 11272, 11249, 
            11368, 11489, 11512, 11393, 11254, 11279, 11494, 11375, 11398, 11519, 
            15992, 15995, 16004, 15999, 16000, 16007, 16018, 16017, 16040, 16065, 
            16066, 16043, 16022, 16029, 16070, 16047, 16052, 16077, 16030, 16025, 
            16048, 16073, 16078, 16055, 21034, 21033, 21038, 21045, 21046, 21041, 
            26072, 26075, 36152, 31113, 31114, 36155, 26084, 26079, 31118, 36159, 
            36164, 31125, 26080, 26087, 36160, 31121, 31126, 36167, 21056, 21059, 
            21104, 21081, 21082, 21107, 26098, 26097, 31136, 36177, 36178, 31139, 
            26120, 26145, 36200, 31161, 31184, 36225, 26146, 26123, 31162, 36203, 
            36226, 31187, 21068, 21063, 21086, 21111, 21116, 21093, 26102, 26109, 
            36182, 31143, 31148, 36189, 26150, 26127, 31166, 36207, 36230, 31191, 
            26132, 26157, 36212, 31173, 31196, 36237, 21064, 21071, 21112, 21089, 
            21094, 21119, 26110, 26105, 31144, 36185, 36190, 31151, 26128, 26153, 
            36208, 31169, 31192, 36233, 26158, 26135, 31174, 36215, 36238, 31199, 
            16114, 16113, 16118, 16125, 16126, 16121, 16232, 16235, 16472, 16353, 
            16354, 16475, 16244, 16239, 16358, 16479, 16484, 16365, 16240, 16247, 
            16480, 16361, 16366, 16487, 21152, 21155, 21164, 21159, 21160, 21167, 
            26194, 26193, 31232, 36273, 36274, 31235, 26198, 26205, 36278, 31239, 
            31244, 36285, 26206, 26201, 31240, 36281, 36286, 31247, 21274, 21273, 
            21392, 21513, 21514, 21395, 26312, 26315, 36392, 31353, 31354, 36395, 
            26552, 26433, 31472, 36513, 36632, 31593, 26434, 26555, 36514, 31475, 
            31594, 36635, 21278, 21285, 21518, 21399, 21404, 21525, 26324, 26319, 
            31358, 36399, 36404, 31365, 26438, 26559, 36518, 31479, 31598, 36639, 
            26564, 26445, 31484, 36525, 36644, 31605, 21286, 21281, 21400, 21521, 
            21526, 21407, 26320, 26327, 36400, 31361, 31366, 36407, 26560, 26441, 
            31480, 36521, 36640, 31601, 26446, 26567, 36526, 31487, 31606, 36647, 
            16136, 16139, 16184, 16161, 16162, 16187, 16258, 16257, 16376, 16497, 
            16498, 16379, 16280, 16305, 16520, 16401, 16424, 16545, 16306, 16283, 
            16402, 16523, 16546, 16427, 21178, 21177, 21200, 21225, 21226, 21203, 
            26216, 26219, 36296, 31257, 31258, 36299, 26264, 26241, 31280, 36321, 
            36344, 31305, 26242, 26267, 36322, 31283, 31306, 36347, 21296, 21299, 
            21536, 21417, 21418, 21539, 26338, 26337, 31376, 36417, 36418, 31379, 
            26456, 26577, 36536, 31497, 31616, 36657, 26578, 26459, 31498, 36539, 
            36658, 31619, 21344, 21321, 21440, 21561, 21584, 21465, 26360, 26385, 
            36440, 31401, 31424, 36465, 26600, 26481, 31520, 36561, 36680, 31641, 
            26504, 26625, 36584, 31545, 31664, 36705, 21322, 21347, 21562, 21443, 
            21466, 21587, 26386, 26363, 31402, 36443, 36466, 31427, 26482, 26603, 
            36562, 31523, 31642, 36683, 26626, 26507, 31546, 36587, 36706, 31667, 
            16148, 16143, 16166, 16191, 16196, 16173, 16262, 16269, 16502, 16383, 
            16388, 16509, 16310, 16287, 16406, 16527, 16550, 16431, 16292, 16317, 
            16532, 16413, 16436, 16557, 21182, 21189, 21230, 21207, 21212, 21237, 
            26228, 26223, 31262, 36303, 36308, 31269, 26246, 26271, 36326, 31287, 
            31310, 36351, 26276, 26253, 31292, 36333, 36356, 31317, 21308, 21303, 
            21422, 21543, 21548, 21429, 26342, 26349, 36422, 31383, 31388, 36429, 
            26582, 26463, 31502, 36543, 36662, 31623, 26468, 26589, 36548, 31509, 
            31628, 36669, 21326, 21351, 21566, 21447, 21470, 21591, 26390, 26367, 
            31406, 36447, 36470, 31431, 26486, 26607, 36566, 31527, 31646, 36687, 
            26630, 26511, 31550, 36591, 36710, 31671, 21356, 21333, 21452, 21573, 
            21596, 21477, 26372, 26397, 36452, 31413, 31436, 36477, 26612, 26493, 
            31532, 36573, 36692, 31653, 26516, 26637, 36596, 31557, 31676, 36717, 
            16144, 16151, 16192, 16169, 16174, 16199, 16270, 16265, 16384, 16505, 
            16510, 16391, 16288, 16313, 16528, 16409, 16432, 16553, 16318, 16295, 
            16414, 16535, 16558, 16439, 21190, 21185, 21208, 21233, 21238, 21215, 
            26224, 26231, 36304, 31265, 31270, 36311, 26272, 26249, 31288, 36329, 
            36352, 31313, 26254, 26279, 36334, 31295, 31318, 36359, 21304, 21311, 
            21544, 21425, 21430, 21551, 26350, 26345, 31384, 36425, 36430, 31391, 
            26464, 26585, 36544, 31505, 31624, 36665, 26590, 26471, 31510, 36551, 
            36670, 31631, 21352, 21329, 21448, 21569, 21592, 21473, 26368, 26393, 
            36448, 31409, 31432, 36473, 26608, 26489, 31528, 36569, 36688, 31649, 
            26512, 26633, 36592, 31553, 31672, 36713, 21334, 21359, 21574, 21455, 
            21478, 21599, 26398, 26375, 31414, 36455, 36478, 31439, 26494, 26615, 
            36574, 31535, 31654, 36695, 26638, 26519, 31558, 36599, 36718, 31679, 
            11672, 11675, 11684, 11679, 11680, 11687, 11698, 11697, 11720, 11745, 
            11746, 11723, 11702, 11709, 11750, 11727, 11732, 11757, 11710, 11705, 
            11728, 11753, 11758, 11735, 12394, 12393, 12398, 12405, 12406, 12401, 
            13112, 13115, 14552, 13833, 13834, 14555, 13124, 13119, 13838, 14559, 
            14564, 13845, 13120, 13127, 14560, 13841, 13846, 14567, 12416, 12419, 
            12464, 12441, 12442, 12467, 13138, 13137, 13856, 14577, 14578, 13859, 
            13160, 13185, 14600, 13881, 13904, 14625, 13186, 13163, 13882, 14603, 
            14626, 13907, 12428, 12423, 12446, 12471, 12476, 12453, 13142, 13149, 
            14582, 13863, 13868, 14589, 13190, 13167, 13886, 14607, 14630, 13911, 
            13172, 13197, 14612, 13893, 13916, 14637, 12424, 12431, 12472, 12449, 
            12454, 12479, 13150, 13145, 13864, 14585, 14590, 13871, 13168, 13193, 
            14608, 13889, 13912, 14633, 13198, 13175, 13894, 14615, 14638, 13919, 
            16714, 16713, 16718, 16725, 16726, 16721, 16736, 16739, 16784, 16761, 
            16762, 16787, 16748, 16743, 16766, 16791, 16796, 16773, 16744, 16751, 
            16792, 16769, 16774, 16799, 21752, 21755, 21764, 21759, 21760, 21767, 
            26794, 26793, 31832, 36873, 36874, 31835, 26798, 26805, 36878, 31839, 
            31844, 36885, 26806, 26801, 31840, 36881, 36886, 31847, 21778, 21777, 
            21800, 21825, 21826, 21803, 26816, 26819, 36896, 31857, 31858, 36899, 
            26864, 26841, 31880, 36921, 36944, 31905, 26842, 26867, 36922, 31883, 
            31906, 36947, 21782, 21789, 21830, 21807, 21812, 21837, 26828, 26823, 
            31862, 36903, 36908, 31869, 26846, 26871, 36926, 31887, 31910, 36951, 
            26876, 26853, 31892, 36933, 36956, 31917, 21790, 21785, 21808, 21833, 
            21838, 21815, 26824, 26831, 36904, 31865, 31870, 36911, 26872, 26849, 
            31888, 36929, 36952, 31913, 26854, 26879, 36934, 31895, 31918, 36959, 
            17432, 17435, 17444, 17439, 17440, 17447, 18154, 18153, 18872, 19593, 
            19594, 18875, 18158, 18165, 19598, 18879, 18884, 19605, 18166, 18161, 
            18880, 19601, 19606, 18887, 22474, 22473, 22478, 22485, 22486, 22481, 
            27512, 27515, 37592, 32553, 32554, 37595, 27524, 27519, 32558, 37599, 
            37604, 32565, 27520, 27527, 37600, 32561, 32566, 37607, 23192, 23195, 
            24632, 23913, 23914, 24635, 28234, 28233, 33272, 38313, 38314, 33275, 
            28952, 29673, 39032, 33993, 34712, 39753, 29674, 28955, 33994, 39035, 
            39754, 34715, 23204, 23199, 23918, 24639, 24644, 23925, 28238, 28245, 
            38318, 33279, 33284, 38325, 29678, 28959, 33998, 39039, 39758, 34719, 
            28964, 29685, 39044, 34005, 34724, 39765, 23200, 23207, 24640, 23921, 
            23926, 24647, 28246, 28241, 33280, 38321, 38326, 33287, 28960, 29681, 
            39040, 34001, 34720, 39761, 29686, 28967, 34006, 39047, 39766, 34727, 
            17458, 17457, 17480, 17505, 17506, 17483, 18176, 18179, 19616, 18897, 
            18898, 19619, 18224, 18201, 18920, 19641, 19664, 18945, 18202, 18227, 
            19642, 18923, 18946, 19667, 22496, 22499, 22544, 22521, 22522, 22547, 
            27538, 27537, 32576, 37617, 37618, 32579, 27560, 27585, 37640, 32601, 
            32624, 37665, 27586, 27563, 32602, 37643, 37666, 32627, 23218, 23217, 
            23936, 24657, 24658, 23939, 28256, 28259, 38336, 33297, 33298, 38339, 
            29696, 28977, 34016, 39057, 39776, 34737, 28978, 29699, 39058, 34019, 
            34738, 39779, 23240, 23265, 24680, 23961, 23984, 24705, 28304, 28281, 
            33320, 38361, 38384, 33345, 29000, 29721, 39080, 34041, 34760, 39801, 
            29744, 29025, 34064, 39105, 39824, 34785, 23266, 23243, 23962, 24683, 
            24706, 23987, 28282, 28307, 38362, 33323, 33346, 38387, 29722, 29003, 
            34042, 39083, 39802, 34763, 29026, 29747, 39106, 34067, 34786, 39827, 
            17462, 17469, 17510, 17487, 17492, 17517, 18188, 18183, 18902, 19623, 
            19628, 18909, 18206, 18231, 19646, 18927, 18950, 19671, 18236, 18213, 
            18932, 19653, 19676, 18957, 22508, 22503, 22526, 22551, 22556, 22533, 
            27542, 27549, 37622, 32583, 32588, 37629, 27590, 27567, 32606, 37647, 
            37670, 32631, 27572, 27597, 37652, 32613, 32636, 37677, 23222, 23229, 
            24662, 23943, 23948, 24669, 28268, 28263, 33302, 38343, 38348, 33309, 
            28982, 29703, 39062, 34023, 34742, 39783, 29708, 28989, 34028, 39069, 
            39788, 34749, 23270, 23247, 23966, 24687, 24710, 23991, 28286, 28311, 
            38366, 33327, 33350, 38391, 29726, 29007, 34046, 39087, 39806, 34767, 
            29030, 29751, 39110, 34071, 34790, 39831, 23252, 23277, 24692, 23973, 
            23996, 24717, 28316, 28293, 33332, 38373, 38396, 33357, 29012, 29733, 
            39092, 34053, 34772, 39813, 29756, 29037, 34076, 39117, 39836, 34797, 
            17470, 17465, 17488, 17513, 17518, 17495, 18184, 18191, 19624, 18905, 
            18910, 19631, 18232, 18209, 18928, 19649, 19672, 18953, 18214, 18239, 
            19654, 18935, 18958, 19679, 22504, 22511, 22552, 22529, 22534, 22559, 
            27550, 27545, 32584, 37625, 37630, 32591, 27568, 27593, 37648, 32609, 
            32632, 37673, 27598, 27575, 32614, 37655, 37678, 32639, 23230, 23225, 
            23944, 24665, 24670, 23951, 28264, 28271, 38344, 33305, 33310, 38351, 
            29704, 28985, 34024, 39065, 39784, 34745, 28990, 29711, 39070, 34031, 
            34750, 39791, 23248, 23273, 24688, 23969, 23992, 24713, 28312, 28289, 
            33328, 38369, 38392, 33353, 29008, 29729, 39088, 34049, 34768, 39809, 
            29752, 29033, 34072, 39113, 39832, 34793, 23278, 23255, 23974, 24695, 
            24718, 23999, 28294, 28319, 38374, 33335, 33358, 38399, 29734, 29015, 
            34054, 39095, 39814, 34775, 29038, 29759, 39118, 34079, 34798, 39839, 
            11794, 11793, 11798, 11805, 11806, 11801, 11912, 11915, 12152, 12033, 
            12034, 12155, 11924, 11919, 12038, 12159, 12164, 12045, 11920, 11927, 
            12160, 12041, 12046, 12167, 12512, 12515, 12524, 12519, 12520, 12527, 
            13234, 13233, 13952, 14673, 14674, 13955, 13238, 13245, 14678, 13959, 
            13964, 14685, 13246, 13241, 13960, 14681, 14686, 13967, 12634, 12633, 
            12752, 12873, 12874, 12755, 13352, 13355, 14792, 14073, 14074, 14795, 
            13592, 13473, 14192, 14913, 15032, 14313, 13474, 13595, 14914, 14195, 
            14314, 15035, 12638, 12645, 12878, 12759, 12764, 12885, 13364, 13359, 
            14078, 14799, 14804, 14085, 13478, 13599, 14918, 14199, 14318, 15039, 
            13604, 13485, 14204, 14925, 15044, 14325, 12646, 12641, 12760, 12881, 
            12886, 12767, 13360, 13367, 14800, 14081, 14086, 14807, 13600, 13481, 
            14200, 14921, 15040, 14321, 13486, 13607, 14926, 14207, 14326, 15047, 
            16832, 16835, 16844, 16839, 16840, 16847, 16954, 16953, 17072, 17193, 
            17194, 17075, 16958, 16965, 17198, 17079, 17084, 17205, 16966, 16961, 
            17080, 17201, 17206, 17087, 21874, 21873, 21878, 21885, 21886, 21881, 
            26912, 26915, 36992, 31953, 31954, 36995, 26924, 26919, 31958, 36999, 
            37004, 31965, 26920, 26927, 37000, 31961, 31966, 37007, 21992, 21995, 
            22232, 22113, 22114, 22235, 27034, 27033, 32072, 37113, 37114, 32075, 
            27152, 27273, 37232, 32193, 32312, 37353, 27274, 27155, 32194, 37235, 
            37354, 32315, 22004, 21999, 22118, 22239, 22244, 22125, 27038, 27045, 
            37118, 32079, 32084, 37125, 27278, 27159, 32198, 37239, 37358, 32319, 
            27164, 27285, 37244, 32205, 32324, 37365, 22000, 22007, 22240, 22121, 
            22126, 22247, 27046, 27041, 32080, 37121, 37126, 32087, 27160, 27281, 
            37240, 32201, 32320, 37361, 27286, 27167, 32206, 37247, 37366, 32327, 
            17554, 17553, 17558, 17565, 17566, 17561, 18272, 18275, 19712, 18993, 
            18994, 19715, 18284, 18279, 18998, 19719, 19724, 19005, 18280, 18287, 
            19720, 19001, 19006, 19727, 22592, 22595, 22604, 22599, 22600, 22607, 
            27634, 27633, 32672, 37713, 37714, 32675, 27638, 27645, 37718, 32679, 
            32684, 37725, 27646, 27641, 32680, 37721, 37726, 32687, 23314, 23313, 
            24032, 24753, 24754, 24035, 28352, 28355, 38432, 33393, 33394, 38435, 
            29792, 29073, 34112, 39153, 39872, 34833, 29074, 29795, 39154, 34115, 
            34834, 39875, 23318, 23325, 24758, 24039, 24044, 24765, 28364, 28359, 
            33398, 38439, 38444, 33405, 29078, 29799, 39158, 34119, 34838, 39879, 
            29804, 29085, 34124, 39165, 39884, 34845, 23326, 23321, 24040, 24761, 
            24766, 24047, 28360, 28367, 38440, 33401, 33406, 38447, 29800, 29081, 
            34120, 39161, 39880, 34841, 29086, 29807, 39166, 34127, 34846, 39887, 
            17672, 17675, 17912, 17793, 17794, 17915, 18394, 18393, 19112, 19833, 
            19834, 19115, 18512, 18633, 19952, 19233, 19352, 20073, 18634, 18515, 
            19234, 19955, 20074, 19355, 22714, 22713, 22832, 22953, 22954, 22835, 
            27752, 27755, 37832, 32793, 32794, 37835, 27992, 27873, 32912, 37953, 
            38072, 33033, 27874, 27995, 37954, 32915, 33034, 38075, 23432, 23435, 
            24872, 24153, 24154, 24875, 28474, 28473, 33512, 38553, 38554, 33515, 
            29192, 29913, 39272, 34233, 34952, 39993, 29914, 29195, 34234, 39275, 
            39994, 34955, 23672, 23553, 24272, 24993, 25112, 24393, 28592, 28713, 
            38672, 33633, 33752, 38793, 30032, 29313, 34352, 39393, 40112, 35073, 
            29432, 30153, 39512, 34473, 35192, 40233, 23554, 23675, 24994, 24275, 
            24394, 25115, 28714, 28595, 33634, 38675, 38794, 33755, 29314, 30035, 
            39394, 34355, 35074, 40115, 30154, 29435, 34474, 39515, 40234, 35195, 
            17684, 17679, 17798, 17919, 17924, 17805, 18398, 18405, 19838, 19119, 
            19124, 19845, 18638, 18519, 19238, 19959, 20078, 19359, 18524, 18645, 
            19964, 19245, 19364, 20085, 22718, 22725, 22958, 22839, 22844, 22965, 
            27764, 27759, 32798, 37839, 37844, 32805, 27878, 27999, 37958, 32919, 
            33038, 38079, 28004, 27885, 32924, 37965, 38084, 33045, 23444, 23439, 
            24158, 24879, 24884, 24165, 28478, 28485, 38558, 33519, 33524, 38565, 
            29918, 29199, 34238, 39279, 39998, 34959, 29204, 29925, 39284, 34245, 
            34964, 40005, 23558, 23679, 24998, 24279, 24398, 25119, 28718, 28599, 
            33638, 38679, 38798, 33759, 29318, 30039, 39398, 34359, 35078, 40119, 
            30158, 29439, 34478, 39519, 40238, 35199, 23684, 23565, 24284, 25005, 
            25124, 24405, 28604, 28725, 38684, 33645, 33764, 38805, 30044, 29325, 
            34364, 39405, 40124, 35085, 29444, 30165, 39524, 34485, 35204, 40245, 
            17680, 17687, 17920, 17801, 17806, 17927, 18406, 18401, 19120, 19841, 
            19846, 19127, 18520, 18641, 19960, 19241, 19360, 20081, 18646, 18527, 
            19246, 19967, 20086, 19367, 22726, 22721, 22840, 22961, 22966, 22847, 
            27760, 27767, 37840, 32801, 32806, 37847, 28000, 27881, 32920, 37961, 
            38080, 33041, 27886, 28007, 37966, 32927, 33046, 38087, 23440, 23447, 
            24880, 24161, 24166, 24887, 28486, 28481, 33520, 38561, 38566, 33527, 
            29200, 29921, 39280, 34241, 34960, 40001, 29926, 29207, 34246, 39287, 
            40006, 34967, 23680, 23561, 24280, 25001, 25120, 24401, 28600, 28721, 
            38680, 33641, 33760, 38801, 30040, 29321, 34360, 39401, 40120, 35081, 
            29440, 30161, 39520, 34481, 35200, 40241, 23566, 23687, 25006, 24287, 
            24406, 25127, 28726, 28607, 33646, 38687, 38806, 33767, 29326, 30047, 
            39406, 34367, 35086, 40127, 30166, 29447, 34486, 39527, 40246, 35207, 
            11816, 11819, 11864, 11841, 11842, 11867, 11938, 11937, 12056, 12177, 
            12178, 12059, 11960, 11985, 12200, 12081, 12104, 12225, 11986, 11963, 
            12082, 12203, 12226, 12107, 12538, 12537, 12560, 12585, 12586, 12563, 
            13256, 13259, 14696, 13977, 13978, 14699, 13304, 13281, 14000, 14721, 
            14744, 14025, 13282, 13307, 14722, 14003, 14026, 14747, 12656, 12659, 
            12896, 12777, 12778, 12899, 13378, 13377, 14096, 14817, 14818, 14099, 
            13496, 13617, 14936, 14217, 14336, 15057, 13618, 13499, 14218, 14939, 
            15058, 14339, 12704, 12681, 12800, 12921, 12944, 12825, 13400, 13425, 
            14840, 14121, 14144, 14865, 13640, 13521, 14240, 14961, 15080, 14361, 
            13544, 13665, 14984, 14265, 14384, 15105, 12682, 12707, 12922, 12803, 
            12826, 12947, 13426, 13403, 14122, 14843, 14866, 14147, 13522, 13643, 
            14962, 14243, 14362, 15083, 13666, 13547, 14266, 14987, 15106, 14387, 
            16858, 16857, 16880, 16905, 16906, 16883, 16976, 16979, 17216, 17097, 
            17098, 17219, 17024, 17001, 17120, 17241, 17264, 17145, 17002, 17027, 
            17242, 17123, 17146, 17267, 21896, 21899, 21944, 21921, 21922, 21947, 
            26938, 26937, 31976, 37017, 37018, 31979, 26960, 26985, 37040, 32001, 
            32024, 37065, 26986, 26963, 32002, 37043, 37066, 32027, 22018, 22017, 
            22136, 22257, 22258, 22139, 27056, 27059, 37136, 32097, 32098, 37139, 
            27296, 27177, 32216, 37257, 37376, 32337, 27178, 27299, 37258, 32219, 
            32338, 37379, 22040, 22065, 22280, 22161, 22184, 22305, 27104, 27081, 
            32120, 37161, 37184, 32145, 27200, 27321, 37280, 32241, 32360, 37401, 
            27344, 27225, 32264, 37305, 37424, 32385, 22066, 22043, 22162, 22283, 
            22306, 22187, 27082, 27107, 37162, 32123, 32146, 37187, 27322, 27203, 
            32242, 37283, 37402, 32363, 27226, 27347, 37306, 32267, 32386, 37427, 
            17576, 17579, 17624, 17601, 17602, 17627, 18298, 18297, 19016, 19737, 
            19738, 19019, 18320, 18345, 19760, 19041, 19064, 19785, 18346, 18323, 
            19042, 19763, 19786, 19067, 22618, 22617, 22640, 22665, 22666, 22643, 
            27656, 27659, 37736, 32697, 32698, 37739, 27704, 27681, 32720, 37761, 
            37784, 32745, 27682, 27707, 37762, 32723, 32746, 37787, 23336, 23339, 
            24776, 24057, 24058, 24779, 28378, 28377, 33416, 38457, 38458, 33419, 
            29096, 29817, 39176, 34137, 34856, 39897, 29818, 29099, 34138, 39179, 
            39898, 34859, 23384, 23361, 24080, 24801, 24824, 24105, 28400, 28425, 
            38480, 33441, 33464, 38505, 29840, 29121, 34160, 39201, 39920, 34881, 
            29144, 29865, 39224, 34185, 34904, 39945, 23362, 23387, 24802, 24083, 
            24106, 24827, 28426, 28403, 33442, 38483, 38506, 33467, 29122, 29843, 
            39202, 34163, 34882, 39923, 29866, 29147, 34186, 39227, 39946, 34907, 
            17698, 17697, 17816, 17937, 17938, 17819, 18416, 18419, 19856, 19137, 
            19138, 19859, 18656, 18537, 19256, 19977, 20096, 19377, 18538, 18659, 
            19978, 19259, 19378, 20099, 22736, 22739, 22976, 22857, 22858, 22979, 
            27778, 27777, 32816, 37857, 37858, 32819, 27896, 28017, 37976, 32937, 
            33056, 38097, 28018, 27899, 32938, 37979, 38098, 33059, 23458, 23457, 
            24176, 24897, 24898, 24179, 28496, 28499, 38576, 33537, 33538, 38579, 
            29936, 29217, 34256, 39297, 40016, 34977, 29218, 29939, 39298, 34259, 
            34978, 40019, 23576, 23697, 25016, 24297, 24416, 25137, 28736, 28617, 
            33656, 38697, 38816, 33777, 29336, 30057, 39416, 34377, 35096, 40137, 
            30176, 29457, 34496, 39537, 40256, 35217, 23698, 23579, 24298, 25019, 
            25138, 24419, 28618, 28739, 38698, 33659, 33778, 38819, 30058, 29339, 
            34378, 39419, 40138, 35099, 29458, 30179, 39538, 34499, 35218, 40259, 
            17720, 17745, 17960, 17841, 17864, 17985, 18464, 18441, 19160, 19881, 
            19904, 19185, 18560, 18681, 20000, 19281, 19400, 20121, 18704, 18585, 
            19304, 20025, 20144, 19425, 22784, 22761, 22880, 23001, 23024, 22905, 
            27800, 27825, 37880, 32841, 32864, 37905, 28040, 27921, 32960, 38001, 
            38120, 33081, 27944, 28065, 38024, 32985, 33104, 38145, 23480, 23505, 
            24920, 24201, 24224, 24945, 28544, 28521, 33560, 38601, 38624, 33585, 
            29240, 29961, 39320, 34281, 35000, 40041, 29984, 29265, 34304, 39345, 
            40064, 35025, 23720, 23601, 24320, 25041, 25160, 24441, 28640, 28761, 
            38720, 33681, 33800, 38841, 30080, 29361, 34400, 39441, 40160, 35121, 
            29480, 30201, 39560, 34521, 35240, 40281, 23624, 23745, 25064, 24345, 
            24464, 25185, 28784, 28665, 33704, 38745, 38864, 33825, 29384, 30105, 
            39464, 34425, 35144, 40185, 30224, 29505, 34544, 39585, 40304, 35265, 
            17746, 17723, 17842, 17963, 17986, 17867, 18442, 18467, 19882, 19163, 
            19186, 19907, 18682, 18563, 19282, 20003, 20122, 19403, 18586, 18707, 
            20026, 19307, 19426, 20147, 22762, 22787, 23002, 22883, 22906, 23027, 
            27826, 27803, 32842, 37883, 37906, 32867, 27922, 28043, 38002, 32963, 
            33082, 38123, 28066, 27947, 32986, 38027, 38146, 33107, 23506, 23483, 
            24202, 24923, 24946, 24227, 28522, 28547, 38602, 33563, 33586, 38627, 
            29962, 29243, 34282, 39323, 40042, 35003, 29266, 29987, 39346, 34307, 
            35026, 40067, 23602, 23723, 25042, 24323, 24442, 25163, 28762, 28643, 
            33682, 38723, 38842, 33803, 29362, 30083, 39442, 34403, 35122, 40163, 
            30202, 29483, 34522, 39563, 40282, 35243, 23746, 23627, 24346, 25067, 
            25186, 24467, 28666, 28787, 38746, 33707, 33826, 38867, 30106, 29387, 
            34426, 39467, 40186, 35147, 29506, 30227, 39586, 34547, 35266, 40307, 
            11828, 11823, 11846, 11871, 11876, 11853, 11942, 11949, 12182, 12063, 
            12068, 12189, 11990, 11967, 12086, 12207, 12230, 12111, 11972, 11997, 
            12212, 12093, 12116, 12237, 12542, 12549, 12590, 12567, 12572, 12597, 
            13268, 13263, 13982, 14703, 14708, 13989, 13286, 13311, 14726, 14007, 
            14030, 14751, 13316, 13293, 14012, 14733, 14756, 14037, 12668, 12663, 
            12782, 12903, 12908, 12789, 13382, 13389, 14822, 14103, 14108, 14829, 
            13622, 13503, 14222, 14943, 15062, 14343, 13508, 13629, 14948, 14229, 
            14348, 15069, 12686, 12711, 12926, 12807, 12830, 12951, 13430, 13407, 
            14126, 14847, 14870, 14151, 13526, 13647, 14966, 14247, 14366, 15087, 
            13670, 13551, 14270, 14991, 15110, 14391, 12716, 12693, 12812, 12933, 
            12956, 12837, 13412, 13437, 14852, 14133, 14156, 14877, 13652, 13533, 
            14252, 14973, 15092, 14373, 13556, 13677, 14996, 14277, 14396, 15117, 
            16862, 16869, 16910, 16887, 16892, 16917, 16988, 16983, 17102, 17223, 
            17228, 17109, 17006, 17031, 17246, 17127, 17150, 17271, 17036, 17013, 
            17132, 17253, 17276, 17157, 21908, 21903, 21926, 21951, 21956, 21933, 
            26942, 26949, 37022, 31983, 31988, 37029, 26990, 26967, 32006, 37047, 
            37070, 32031, 26972, 26997, 37052, 32013, 32036, 37077, 22022, 22029, 
            22262, 22143, 22148, 22269, 27068, 27063, 32102, 37143, 37148, 32109, 
            27182, 27303, 37262, 32223, 32342, 37383, 27308, 27189, 32228, 37269, 
            37388, 32349, 22070, 22047, 22166, 22287, 22310, 22191, 27086, 27111, 
            37166, 32127, 32150, 37191, 27326, 27207, 32246, 37287, 37406, 32367, 
            27230, 27351, 37310, 32271, 32390, 37431, 22052, 22077, 22292, 22173, 
            22196, 22317, 27116, 27093, 32132, 37173, 37196, 32157, 27212, 27333, 
            37292, 32253, 32372, 37413, 27356, 27237, 32276, 37317, 37436, 32397, 
            17588, 17583, 17606, 17631, 17636, 17613, 18302, 18309, 19742, 19023, 
            19028, 19749, 18350, 18327, 19046, 19767, 19790, 19071, 18332, 18357, 
            19772, 19053, 19076, 19797, 22622, 22629, 22670, 22647, 22652, 22677, 
            27668, 27663, 32702, 37743, 37748, 32709, 27686, 27711, 37766, 32727, 
            32750, 37791, 27716, 27693, 32732, 37773, 37796, 32757, 23348, 23343, 
            24062, 24783, 24788, 24069, 28382, 28389, 38462, 33423, 33428, 38469, 
            29822, 29103, 34142, 39183, 39902, 34863, 29108, 29829, 39188, 34149, 
            34868, 39909, 23366, 23391, 24806, 24087, 24110, 24831, 28430, 28407, 
            33446, 38487, 38510, 33471, 29126, 29847, 39206, 34167, 34886, 39927, 
            29870, 29151, 34190, 39231, 39950, 34911, 23396, 23373, 24092, 24813, 
            24836, 24117, 28412, 28437, 38492, 33453, 33476, 38517, 29852, 29133, 
            34172, 39213, 39932, 34893, 29156, 29877, 39236, 34197, 34916, 39957, 
            17702, 17709, 17942, 17823, 17828, 17949, 18428, 18423, 19142, 19863, 
            19868, 19149, 18542, 18663, 19982, 19263, 19382, 20103, 18668, 18549, 
            19268, 19989, 20108, 19389, 22748, 22743, 22862, 22983, 22988, 22869, 
            27782, 27789, 37862, 32823, 32828, 37869, 28022, 27903, 32942, 37983, 
            38102, 33063, 27908, 28029, 37988, 32949, 33068, 38109, 23462, 23469, 
            24902, 24183, 24188, 24909, 28508, 28503, 33542, 38583, 38588, 33549, 
            29222, 29943, 39302, 34263, 34982, 40023, 29948, 29229, 34268, 39309, 
            40028, 34989, 23702, 23583, 24302, 25023, 25142, 24423, 28622, 28743, 
            38702, 33663, 33782, 38823, 30062, 29343, 34382, 39423, 40142, 35103, 
            29462, 30183, 39542, 34503, 35222, 40263, 23588, 23709, 25028, 24309, 
            24428, 25149, 28748, 28629, 33668, 38709, 38828, 33789, 29348, 30069, 
            39428, 34389, 35108, 40149, 30188, 29469, 34508, 39549, 40268, 35229, 
            17750, 17727, 17846, 17967, 17990, 17871, 18446, 18471, 19886, 19167, 
            19190, 19911, 18686, 18567, 19286, 20007, 20126, 19407, 18590, 18711, 
            20030, 19311, 19430, 20151, 22766, 22791, 23006, 22887, 22910, 23031, 
            27830, 27807, 32846, 37887, 37910, 32871, 27926, 28047, 38006, 32967, 
            33086, 38127, 28070, 27951, 32990, 38031, 38150, 33111, 23510, 23487, 
            24206, 24927, 24950, 24231, 28526, 28551, 38606, 33567, 33590, 38631, 
            29966, 29247, 34286, 39327, 40046, 35007, 29270, 29991, 39350, 34311, 
            35030, 40071, 23606, 23727, 25046, 24327, 24446, 25167, 28766, 28647, 
            33686, 38727, 38846, 33807, 29366, 30087, 39446, 34407, 35126, 40167, 
            30206, 29487, 34526, 39567, 40286, 35247, 23750, 23631, 24350, 25071, 
            25190, 24471, 28670, 28791, 38750, 33711, 33830, 38871, 30110, 29391, 
            34430, 39471, 40190, 35151, 29510, 30231, 39590, 34551, 35270, 40311, 
            17732, 17757, 17972, 17853, 17876, 17997, 18476, 18453, 19172, 19893, 
            19916, 19197, 18572, 18693, 20012, 19293, 19412, 20133, 18716, 18597, 
            19316, 20037, 20156, 19437, 22796, 22773, 22892, 23013, 23036, 22917, 
            27812, 27837, 37892, 32853, 32876, 37917, 28052, 27933, 32972, 38013, 
            38132, 33093, 27956, 28077, 38036, 32997, 33116, 38157, 23492, 23517, 
            24932, 24213, 24236, 24957, 28556, 28533, 33572, 38613, 38636, 33597, 
            29252, 29973, 39332, 34293, 35012, 40053, 29996, 29277, 34316, 39357, 
            40076, 35037, 23732, 23613, 24332, 25053, 25172, 24453, 28652, 28773, 
            38732, 33693, 33812, 38853, 30092, 29373, 34412, 39453, 40172, 35133, 
            29492, 30213, 39572, 34533, 35252, 40293, 23636, 23757, 25076, 24357, 
            24476, 25197, 28796, 28677, 33716, 38757, 38876, 33837, 29396, 30117, 
            39476, 34437, 35156, 40197, 30236, 29517, 34556, 39597, 40316, 35277, 
            11824, 11831, 11872, 11849, 11854, 11879, 11950, 11945, 12064, 12185, 
            12190, 12071, 11968, 11993, 12208, 12089, 12112, 12233, 11998, 11975, 
            12094, 12215, 12238, 12119, 12550, 12545, 12568, 12593, 12598, 12575, 
            13264, 13271, 14704, 13985, 13990, 14711, 13312, 13289, 14008, 14729, 
            14752, 14033, 13294, 13319, 14734, 14015, 14038, 14759, 12664, 12671, 
            12904, 12785, 12790, 12911, 13390, 13385, 14104, 14825, 14830, 14111, 
            13504, 13625, 14944, 14225, 14344, 15065, 13630, 13511, 14230, 14951, 
            15070, 14351, 12712, 12689, 12808, 12929, 12952, 12833, 13408, 13433, 
            14848, 14129, 14152, 14873, 13648, 13529, 14248, 14969, 15088, 14369, 
            13552, 13673, 14992, 14273, 14392, 15113, 12694, 12719, 12934, 12815, 
            12838, 12959, 13438, 13415, 14134, 14855, 14878, 14159, 13534, 13655, 
            14974, 14255, 14374, 15095, 13678, 13559, 14278, 14999, 15118, 14399, 
            16870, 16865, 16888, 16913, 16918, 16895, 16984, 16991, 17224, 17105, 
            17110, 17231, 17032, 17009, 17128, 17249, 17272, 17153, 17014, 17039, 
            17254, 17135, 17158, 17279, 21904, 21911, 21952, 21929, 21934, 21959, 
            26950, 26945, 31984, 37025, 37030, 31991, 26968, 26993, 37048, 32009, 
            32032, 37073, 26998, 26975, 32014, 37055, 37078, 32039, 22030, 22025, 
            22144, 22265, 22270, 22151, 27064, 27071, 37144, 32105, 32110, 37151, 
            27304, 27185, 32224, 37265, 37384, 32345, 27190, 27311, 37270, 32231, 
            32350, 37391, 22048, 22073, 22288, 22169, 22192, 22313, 27112, 27089, 
            32128, 37169, 37192, 32153, 27208, 27329, 37288, 32249, 32368, 37409, 
            27352, 27233, 32272, 37313, 37432, 32393, 22078, 22055, 22174, 22295, 
            22318, 22199, 27094, 27119, 37174, 32135, 32158, 37199, 27334, 27215, 
            32254, 37295, 37414, 32375, 27238, 27359, 37318, 32279, 32398, 37439, 
            17584, 17591, 17632, 17609, 17614, 17639, 18310, 18305, 19024, 19745, 
            19750, 19031, 18328, 18353, 19768, 19049, 19072, 19793, 18358, 18335, 
            19054, 19775, 19798, 19079, 22630, 22625, 22648, 22673, 22678, 22655, 
            27664, 27671, 37744, 32705, 32710, 37751, 27712, 27689, 32728, 37769, 
            37792, 32753, 27694, 27719, 37774, 32735, 32758, 37799, 23344, 23351, 
            24784, 24065, 24070, 24791, 28390, 28385, 33424, 38465, 38470, 33431, 
            29104, 29825, 39184, 34145, 34864, 39905, 29830, 29111, 34150, 39191, 
            39910, 34871, 23392, 23369, 24088, 24809, 24832, 24113, 28408, 28433, 
            38488, 33449, 33472, 38513, 29848, 29129, 34168, 39209, 39928, 34889, 
            29152, 29873, 39232, 34193, 34912, 39953, 23374, 23399, 24814, 24095, 
            24118, 24839, 28438, 28415, 33454, 38495, 38518, 33479, 29134, 29855, 
            39214, 34175, 34894, 39935, 29878, 29159, 34198, 39239, 39958, 34919, 
            17710, 17705, 17824, 17945, 17950, 17831, 18424, 18431, 19864, 19145, 
            19150, 19871, 18664, 18545, 19264, 19985, 20104, 19385, 18550, 18671, 
            19990, 19271, 19390, 20111, 22744, 22751, 22984, 22865, 22870, 22991, 
            27790, 27785, 32824, 37865, 37870, 32831, 27904, 28025, 37984, 32945, 
            33064, 38105, 28030, 27911, 32950, 37991, 38110, 33071, 23470, 23465, 
            24184, 24905, 24910, 24191, 28504, 28511, 38584, 33545, 33550, 38591, 
            29944, 29225, 34264, 39305, 40024, 34985, 29230, 29951, 39310, 34271, 
            34990, 40031, 23584, 23705, 25024, 24305, 24424, 25145, 28744, 28625, 
            33664, 38705, 38824, 33785, 29344, 30065, 39424, 34385, 35104, 40145, 
            30184, 29465, 34504, 39545, 40264, 35225, 23710, 23591, 24310, 25031, 
            25150, 24431, 28630, 28751, 38710, 33671, 33790, 38831, 30070, 29351, 
            34390, 39431, 40150, 35111, 29470, 30191, 39550, 34511, 35230, 40271, 
            17728, 17753, 17968, 17849, 17872, 17993, 18472, 18449, 19168, 19889, 
            19912, 19193, 18568, 18689, 20008, 19289, 19408, 20129, 18712, 18593, 
            19312, 20033, 20152, 19433, 22792, 22769, 22888, 23009, 23032, 22913, 
            27808, 27833, 37888, 32849, 32872, 37913, 28048, 27929, 32968, 38009, 
            38128, 33089, 27952, 28073, 38032, 32993, 33112, 38153, 23488, 23513, 
            24928, 24209, 24232, 24953, 28552, 28529, 33568, 38609, 38632, 33593, 
            29248, 29969, 39328, 34289, 35008, 40049, 29992, 29273, 34312, 39353, 
            40072, 35033, 23728, 23609, 24328, 25049, 25168, 24449, 28648, 28769, 
            38728, 33689, 33808, 38849, 30088, 29369, 34408, 39449, 40168, 35129, 
            29488, 30209, 39568, 34529, 35248, 40289, 23632, 23753, 25072, 24353, 
            24472, 25193, 28792, 28673, 33712, 38753, 38872, 33833, 29392, 30113, 
            39472, 34433, 35152, 40193, 30232, 29513, 34552, 39593, 40312, 35273, 
            17758, 17735, 17854, 17975, 17998, 17879, 18454, 18479, 19894, 19175, 
            19198, 19919, 18694, 18575, 19294, 20015, 20134, 19415, 18598, 18719, 
            20038, 19319, 19438, 20159, 22774, 22799, 23014, 22895, 22918, 23039, 
            27838, 27815, 32854, 37895, 37918, 32879, 27934, 28055, 38014, 32975, 
            33094, 38135, 28078, 27959, 32998, 38039, 38158, 33119, 23518, 23495, 
            24214, 24935, 24958, 24239, 28534, 28559, 38614, 33575, 33598, 38639, 
            29974, 29255, 34294, 39335, 40054, 35015, 29278, 29999, 39358, 34319, 
            35038, 40079, 23614, 23735, 25054, 24335, 24454, 25175, 28774, 28655, 
            33694, 38735, 38854, 33815, 29374, 30095, 39454, 34415, 35134, 40175, 
            30214, 29495, 34534, 39575, 40294, 35255, 23758, 23639, 24358, 25079, 
            25198, 24479, 28678, 28799, 38758, 33719, 33838, 38879, 30118, 29399, 
            34438, 39479, 40198, 35159, 29518, 30239, 39598, 34559, 35278, 40319, 
            5914, 5913, 5918, 5925, 5926, 5921, 5936, 5939, 5984, 5961, 
            5962, 5987, 5948, 5943, 5966, 5991, 5996, 5973, 5944, 5951, 
            5992, 5969, 5974, 5999, 6032, 6035, 6044, 6039, 6040, 6047, 
            6154, 6153, 6272, 6393, 6394, 6275, 6158, 6165, 6398, 6279, 
            6284, 6405, 6166, 6161, 6280, 6401, 6406, 6287, 6058, 6057, 
            6080, 6105, 6106, 6083, 6176, 6179, 6416, 6297, 6298, 6419, 
            6224, 6201, 6320, 6441, 6464, 6345, 6202, 6227, 6442, 6323, 
            6346, 6467, 6062, 6069, 6110, 6087, 6092, 6117, 6188, 6183, 
            6302, 6423, 6428, 6309, 6206, 6231, 6446, 6327, 6350, 6471, 
            6236, 6213, 6332, 6453, 6476, 6357, 6070, 6065, 6088, 6113, 
            6118, 6095, 6184, 6191, 6424, 6305, 6310, 6431, 6232, 6209, 
            6328, 6449, 6472, 6353, 6214, 6239, 6454, 6335, 6358, 6479, 
            6632, 6635, 6644, 6639, 6640, 6647, 6658, 6657, 6680, 6705, 
            6706, 6683, 6662, 6669, 6710, 6687, 6692, 6717, 6670, 6665, 
            6688, 6713, 6718, 6695, 7354, 7353, 7358, 7365, 7366, 7361, 
            8072, 8075, 9512, 8793, 8794, 9515, 8084, 8079, 8798, 9519, 
            9524, 8805, 8080, 8087, 9520, 8801, 8806, 9527, 7376, 7379, 
            7424, 7401, 7402, 7427, 8098, 8097, 8816, 9537, 9538, 8819, 
            8120, 8145, 9560, 8841, 8864, 9585, 8146, 8123, 8842, 9563, 
            9586, 8867, 7388, 7383, 7406, 7431, 7436, 7413, 8102, 8109, 
            9542, 8823, 8828, 9549, 8150, 8127, 8846, 9567, 9590, 8871, 
            8132, 8157, 9572, 8853, 8876, 9597, 7384, 7391, 7432, 7409, 
            7414, 7439, 8110, 8105, 8824, 9545, 9550, 8831, 8128, 8153, 
            9568, 8849, 8872, 9593, 8158, 8135, 8854, 9575, 9598, 8879, 
            6754, 6753, 6758, 6765, 6766, 6761, 6872, 6875, 7112, 6993, 
            6994, 7115, 6884, 6879, 6998, 7119, 7124, 7005, 6880, 6887, 
            7120, 7001, 7006, 7127, 7472, 7475, 7484, 7479, 7480, 7487, 
            8194, 8193, 8912, 9633, 9634, 8915, 8198, 8205, 9638, 8919, 
            8924, 9645, 8206, 8201, 8920, 9641, 9646, 8927, 7594, 7593, 
            7712, 7833, 7834, 7715, 8312, 8315, 9752, 9033, 9034, 9755, 
            8552, 8433, 9152, 9873, 9992, 9273, 8434, 8555, 9874, 9155, 
            9274, 9995, 7598, 7605, 7838, 7719, 7724, 7845, 8324, 8319, 
            9038, 9759, 9764, 9045, 8438, 8559, 9878, 9159, 9278, 9999, 
            8564, 8445, 9164, 9885, 10004, 9285, 7606, 7601, 7720, 7841, 
            7846, 7727, 8320, 8327, 9760, 9041, 9046, 9767, 8560, 8441, 
            9160, 9881, 10000, 9281, 8446, 8567, 9886, 9167, 9286, 10007, 
            6776, 6779, 6824, 6801, 6802, 6827, 6898, 6897, 7016, 7137, 
            7138, 7019, 6920, 6945, 7160, 7041, 7064, 7185, 6946, 6923, 
            7042, 7163, 7186, 7067, 7498, 7497, 7520, 7545, 7546, 7523, 
            8216, 8219, 9656, 8937, 8938, 9659, 8264, 8241, 8960, 9681, 
            9704, 8985, 8242, 8267, 9682, 8963, 8986, 9707, 7616, 7619, 
            7856, 7737, 7738, 7859, 8338, 8337, 9056, 9777, 9778, 9059, 
            8456, 8577, 9896, 9177, 9296, 10017, 8578, 8459, 9178, 9899, 
            10018, 9299, 7664, 7641, 7760, 7881, 7904, 7785, 8360, 8385, 
            9800, 9081, 9104, 9825, 8600, 8481, 9200, 9921, 10040, 9321, 
            8504, 8625, 9944, 9225, 9344, 10065, 7642, 7667, 7882, 7763, 
            7786, 7907, 8386, 8363, 9082, 9803, 9826, 9107, 8482, 8603, 
            9922, 9203, 9322, 10043, 8626, 8507, 9226, 9947, 10066, 9347, 
            6788, 6783, 6806, 6831, 6836, 6813, 6902, 6909, 7142, 7023, 
            7028, 7149, 6950, 6927, 7046, 7167, 7190, 7071, 6932, 6957, 
            7172, 7053, 7076, 7197, 7502, 7509, 7550, 7527, 7532, 7557, 
            8228, 8223, 8942, 9663, 9668, 8949, 8246, 8271, 9686, 8967, 
            8990, 9711, 8276, 8253, 8972, 9693, 9716, 8997, 7628, 7623, 
            7742, 7863, 7868, 7749, 8342, 8349, 9782, 9063, 9068, 9789, 
            8582, 8463, 9182, 9903, 10022, 9303, 8468, 8589, 9908, 9189, 
            9308, 10029, 7646, 7671, 7886, 7767, 7790, 7911, 8390, 8367, 
            9086, 9807, 9830, 9111, 8486, 8607, 9926, 9207, 9326, 10047, 
            8630, 8511, 9230, 9951, 10070, 9351, 7676, 7653, 7772, 7893, 
            7916, 7797, 8372, 8397, 9812, 9093, 9116, 9837, 8612, 8493, 
            9212, 9933, 10052, 9333, 8516, 8637, 9956, 9237, 9356, 10077, 
            6784, 6791, 6832, 6809, 6814, 6839, 6910, 6905, 7024, 7145, 
            7150, 7031, 6928, 6953, 7168, 7049, 7072, 7193, 6958, 6935, 
            7054, 7175, 7198, 7079, 7510, 7505, 7528, 7553, 7558, 7535, 
            8224, 8231, 9664, 8945, 8950, 9671, 8272, 8249, 8968, 9689, 
            9712, 8993, 8254, 8279, 9694, 8975, 8998, 9719, 7624, 7631, 
            7864, 7745, 7750, 7871, 8350, 8345, 9064, 9785, 9790, 9071, 
            8464, 8585, 9904, 9185, 9304, 10025, 8590, 8471, 9190, 9911, 
            10030, 9311, 7672, 7649, 7768, 7889, 7912, 7793, 8368, 8393, 
            9808, 9089, 9112, 9833, 8608, 8489, 9208, 9929, 10048, 9329, 
            8512, 8633, 9952, 9233, 9352, 10073, 7654, 7679, 7894, 7775, 
            7798, 7919, 8398, 8375, 9094, 9815, 9838, 9119, 8494, 8615, 
            9934, 9215, 9334, 10055, 8638, 8519, 9238, 9959, 10078, 9359, 
            10952, 10955, 10964, 10959, 10960, 10967, 10978, 10977, 11000, 11025, 
            11026, 11003, 10982, 10989, 11030, 11007, 11012, 11037, 10990, 10985, 
            11008, 11033, 11038, 11015, 11074, 11073, 11078, 11085, 11086, 11081, 
            11192, 11195, 11432, 11313, 11314, 11435, 11204, 11199, 11318, 11439, 
            11444, 11325, 11200, 11207, 11440, 11321, 11326, 11447, 11096, 11099, 
            11144, 11121, 11122, 11147, 11218, 11217, 11336, 11457, 11458, 11339, 
            11240, 11265, 11480, 11361, 11384, 11505, 11266, 11243, 11362, 11483, 
            11506, 11387, 11108, 11103, 11126, 11151, 11156, 11133, 11222, 11229, 
            11462, 11343, 11348, 11469, 11270, 11247, 11366, 11487, 11510, 11391, 
            11252, 11277, 11492, 11373, 11396, 11517, 11104, 11111, 11152, 11129, 
            11134, 11159, 11230, 11225, 11344, 11465, 11470, 11351, 11248, 11273, 
            11488, 11369, 11392, 11513, 11278, 11255, 11374, 11495, 11518, 11399, 
            15994, 15993, 15998, 16005, 16006, 16001, 16016, 16019, 16064, 16041, 
            16042, 16067, 16028, 16023, 16046, 16071, 16076, 16053, 16024, 16031, 
            16072, 16049, 16054, 16079, 21032, 21035, 21044, 21039, 21040, 21047, 
            26074, 26073, 31112, 36153, 36154, 31115, 26078, 26085, 36158, 31119, 
            31124, 36165, 26086, 26081, 31120, 36161, 36166, 31127, 21058, 21057, 
            21080, 21105, 21106, 21083, 26096, 26099, 36176, 31137, 31138, 36179, 
            26144, 26121, 31160, 36201, 36224, 31185, 26122, 26147, 36202, 31163, 
            31186, 36227, 21062, 21069, 21110, 21087, 21092, 21117, 26108, 26103, 
            31142, 36183, 36188, 31149, 26126, 26151, 36206, 31167, 31190, 36231, 
            26156, 26133, 31172, 36213, 36236, 31197, 21070, 21065, 21088, 21113, 
            21118, 21095, 26104, 26111, 36184, 31145, 31150, 36191, 26152, 26129, 
            31168, 36209, 36232, 31193, 26134, 26159, 36214, 31175, 31198, 36239, 
            16112, 16115, 16124, 16119, 16120, 16127, 16234, 16233, 16352, 16473, 
            16474, 16355, 16238, 16245, 16478, 16359, 16364, 16485, 16246, 16241, 
            16360, 16481, 16486, 16367, 21154, 21153, 21158, 21165, 21166, 21161, 
            26192, 26195, 36272, 31233, 31234, 36275, 26204, 26199, 31238, 36279, 
            36284, 31245, 26200, 26207, 36280, 31241, 31246, 36287, 21272, 21275, 
            21512, 21393, 21394, 21515, 26314, 26313, 31352, 36393, 36394, 31355, 
            26432, 26553, 36512, 31473, 31592, 36633, 26554, 26435, 31474, 36515, 
            36634, 31595, 21284, 21279, 21398, 21519, 21524, 21405, 26318, 26325, 
            36398, 31359, 31364, 36405, 26558, 26439, 31478, 36519, 36638, 31599, 
            26444, 26565, 36524, 31485, 31604, 36645, 21280, 21287, 21520, 21401, 
            21406, 21527, 26326, 26321, 31360, 36401, 36406, 31367, 26440, 26561, 
            36520, 31481, 31600, 36641, 26566, 26447, 31486, 36527, 36646, 31607, 
            16138, 16137, 16160, 16185, 16186, 16163, 16256, 16259, 16496, 16377, 
            16378, 16499, 16304, 16281, 16400, 16521, 16544, 16425, 16282, 16307, 
            16522, 16403, 16426, 16547, 21176, 21179, 21224, 21201, 21202, 21227, 
            26218, 26217, 31256, 36297, 36298, 31259, 26240, 26265, 36320, 31281, 
            31304, 36345, 26266, 26243, 31282, 36323, 36346, 31307, 21298, 21297, 
            21416, 21537, 21538, 21419, 26336, 26339, 36416, 31377, 31378, 36419, 
            26576, 26457, 31496, 36537, 36656, 31617, 26458, 26579, 36538, 31499, 
            31618, 36659, 21320, 21345, 21560, 21441, 21464, 21585, 26384, 26361, 
            31400, 36441, 36464, 31425, 26480, 26601, 36560, 31521, 31640, 36681, 
            26624, 26505, 31544, 36585, 36704, 31665, 21346, 21323, 21442, 21563, 
            21586, 21467, 26362, 26387, 36442, 31403, 31426, 36467, 26602, 26483, 
            31522, 36563, 36682, 31643, 26506, 26627, 36586, 31547, 31666, 36707, 
            16142, 16149, 16190, 16167, 16172, 16197, 16268, 16263, 16382, 16503, 
            16508, 16389, 16286, 16311, 16526, 16407, 16430, 16551, 16316, 16293, 
            16412, 16533, 16556, 16437, 21188, 21183, 21206, 21231, 21236, 21213, 
            26222, 26229, 36302, 31263, 31268, 36309, 26270, 26247, 31286, 36327, 
            36350, 31311, 26252, 26277, 36332, 31293, 31316, 36357, 21302, 21309, 
            21542, 21423, 21428, 21549, 26348, 26343, 31382, 36423, 36428, 31389, 
            26462, 26583, 36542, 31503, 31622, 36663, 26588, 26469, 31508, 36549, 
            36668, 31629, 21350, 21327, 21446, 21567, 21590, 21471, 26366, 26391, 
            36446, 31407, 31430, 36471, 26606, 26487, 31526, 36567, 36686, 31647, 
            26510, 26631, 36590, 31551, 31670, 36711, 21332, 21357, 21572, 21453, 
            21476, 21597, 26396, 26373, 31412, 36453, 36476, 31437, 26492, 26613, 
            36572, 31533, 31652, 36693, 26636, 26517, 31556, 36597, 36716, 31677, 
            16150, 16145, 16168, 16193, 16198, 16175, 16264, 16271, 16504, 16385, 
            16390, 16511, 16312, 16289, 16408, 16529, 16552, 16433, 16294, 16319, 
            16534, 16415, 16438, 16559, 21184, 21191, 21232, 21209, 21214, 21239, 
            26230, 26225, 31264, 36305, 36310, 31271, 26248, 26273, 36328, 31289, 
            31312, 36353, 26278, 26255, 31294, 36335, 36358, 31319, 21310, 21305, 
            21424, 21545, 21550, 21431, 26344, 26351, 36424, 31385, 31390, 36431, 
            26584, 26465, 31504, 36545, 36664, 31625, 26470, 26591, 36550, 31511, 
            31630, 36671, 21328, 21353, 21568, 21449, 21472, 21593, 26392, 26369, 
            31408, 36449, 36472, 31433, 26488, 26609, 36568, 31529, 31648, 36689, 
            26632, 26513, 31552, 36593, 36712, 31673, 21358, 21335, 21454, 21575, 
            21598, 21479, 26374, 26399, 36454, 31415, 31438, 36479, 26614, 26495, 
            31534, 36575, 36694, 31655, 26518, 26639, 36598, 31559, 31678, 36719, 
            11674, 11673, 11678, 11685, 11686, 11681, 11696, 11699, 11744, 11721, 
            11722, 11747, 11708, 11703, 11726, 11751, 11756, 11733, 11704, 11711, 
            11752, 11729, 11734, 11759, 12392, 12395, 12404, 12399, 12400, 12407, 
            13114, 13113, 13832, 14553, 14554, 13835, 13118, 13125, 14558, 13839, 
            13844, 14565, 13126, 13121, 13840, 14561, 14566, 13847, 12418, 12417, 
            12440, 12465, 12466, 12443, 13136, 13139, 14576, 13857, 13858, 14579, 
            13184, 13161, 13880, 14601, 14624, 13905, 13162, 13187, 14602, 13883, 
            13906, 14627, 12422, 12429, 12470, 12447, 12452, 12477, 13148, 13143, 
            13862, 14583, 14588, 13869, 13166, 13191, 14606, 13887, 13910, 14631, 
            13196, 13173, 13892, 14613, 14636, 13917, 12430, 12425, 12448, 12473, 
            12478, 12455, 13144, 13151, 14584, 13865, 13870, 14591, 13192, 13169, 
            13888, 14609, 14632, 13913, 13174, 13199, 14614, 13895, 13918, 14639, 
            16712, 16715, 16724, 16719, 16720, 16727, 16738, 16737, 16760, 16785, 
            16786, 16763, 16742, 16749, 16790, 16767, 16772, 16797, 16750, 16745, 
            16768, 16793, 16798, 16775, 21754, 21753, 21758, 21765, 21766, 21761, 
            26792, 26795, 36872, 31833, 31834, 36875, 26804, 26799, 31838, 36879, 
            36884, 31845, 26800, 26807, 36880, 31841, 31846, 36887, 21776, 21779, 
            21824, 21801, 21802, 21827, 26818, 26817, 31856, 36897, 36898, 31859, 
            26840, 26865, 36920, 31881, 31904, 36945, 26866, 26843, 31882, 36923, 
            36946, 31907, 21788, 21783, 21806, 21831, 21836, 21813, 26822, 26829, 
            36902, 31863, 31868, 36909, 26870, 26847, 31886, 36927, 36950, 31911, 
            26852, 26877, 36932, 31893, 31916, 36957, 21784, 21791, 21832, 21809, 
            21814, 21839, 26830, 26825, 31864, 36905, 36910, 31871, 26848, 26873, 
            36928, 31889, 31912, 36953, 26878, 26855, 31894, 36935, 36958, 31919, 
            17434, 17433, 17438, 17445, 17446, 17441, 18152, 18155, 19592, 18873, 
            18874, 19595, 18164, 18159, 18878, 19599, 19604, 18885, 18160, 18167, 
            19600, 18881, 18886, 19607, 22472, 22475, 22484, 22479, 22480, 22487, 
            27514, 27513, 32552, 37593, 37594, 32555, 27518, 27525, 37598, 32559, 
            32564, 37605, 27526, 27521, 32560, 37601, 37606, 32567, 23194, 23193, 
            23912, 24633, 24634, 23915, 28232, 28235, 38312, 33273, 33274, 38315, 
            29672, 28953, 33992, 39033, 39752, 34713, 28954, 29675, 39034, 33995, 
            34714, 39755, 23198, 23205, 24638, 23919, 23924, 24645, 28244, 28239, 
            33278, 38319, 38324, 33285, 28958, 29679, 39038, 33999, 34718, 39759, 
            29684, 28965, 34004, 39045, 39764, 34725, 23206, 23201, 23920, 24641, 
            24646, 23927, 28240, 28247, 38320, 33281, 33286, 38327, 29680, 28961, 
            34000, 39041, 39760, 34721, 28966, 29687, 39046, 34007, 34726, 39767, 
            17456, 17459, 17504, 17481, 17482, 17507, 18178, 18177, 18896, 19617, 
            19618, 18899, 18200, 18225, 19640, 18921, 18944, 19665, 18226, 18203, 
            18922, 19643, 19666, 18947, 22498, 22497, 22520, 22545, 22546, 22523, 
            27536, 27539, 37616, 32577, 32578, 37619, 27584, 27561, 32600, 37641, 
            37664, 32625, 27562, 27587, 37642, 32603, 32626, 37667, 23216, 23219, 
            24656, 23937, 23938, 24659, 28258, 28257, 33296, 38337, 38338, 33299, 
            28976, 29697, 39056, 34017, 34736, 39777, 29698, 28979, 34018, 39059, 
            39778, 34739, 23264, 23241, 23960, 24681, 24704, 23985, 28280, 28305, 
            38360, 33321, 33344, 38385, 29720, 29001, 34040, 39081, 39800, 34761, 
            29024, 29745, 39104, 34065, 34784, 39825, 23242, 23267, 24682, 23963, 
            23986, 24707, 28306, 28283, 33322, 38363, 38386, 33347, 29002, 29723, 
            39082, 34043, 34762, 39803, 29746, 29027, 34066, 39107, 39826, 34787, 
            17468, 17463, 17486, 17511, 17516, 17493, 18182, 18189, 19622, 18903, 
            18908, 19629, 18230, 18207, 18926, 19647, 19670, 18951, 18212, 18237, 
            19652, 18933, 18956, 19677, 22502, 22509, 22550, 22527, 22532, 22557, 
            27548, 27543, 32582, 37623, 37628, 32589, 27566, 27591, 37646, 32607, 
            32630, 37671, 27596, 27573, 32612, 37653, 37676, 32637, 23228, 23223, 
            23942, 24663, 24668, 23949, 28262, 28269, 38342, 33303, 33308, 38349, 
            29702, 28983, 34022, 39063, 39782, 34743, 28988, 29709, 39068, 34029, 
            34748, 39789, 23246, 23271, 24686, 23967, 23990, 24711, 28310, 28287, 
            33326, 38367, 38390, 33351, 29006, 29727, 39086, 34047, 34766, 39807, 
            29750, 29031, 34070, 39111, 39830, 34791, 23276, 23253, 23972, 24693, 
            24716, 23997, 28292, 28317, 38372, 33333, 33356, 38397, 29732, 29013, 
            34052, 39093, 39812, 34773, 29036, 29757, 39116, 34077, 34796, 39837, 
            17464, 17471, 17512, 17489, 17494, 17519, 18190, 18185, 18904, 19625, 
            19630, 18911, 18208, 18233, 19648, 18929, 18952, 19673, 18238, 18215, 
            18934, 19655, 19678, 18959, 22510, 22505, 22528, 22553, 22558, 22535, 
            27544, 27551, 37624, 32585, 32590, 37631, 27592, 27569, 32608, 37649, 
            37672, 32633, 27574, 27599, 37654, 32615, 32638, 37679, 23224, 23231, 
            24664, 23945, 23950, 24671, 28270, 28265, 33304, 38345, 38350, 33311, 
            28984, 29705, 39064, 34025, 34744, 39785, 29710, 28991, 34030, 39071, 
            39790, 34751, 23272, 23249, 23968, 24689, 24712, 23993, 28288, 28313, 
            38368, 33329, 33352, 38393, 29728, 29009, 34048, 39089, 39808, 34769, 
            29032, 29753, 39112, 34073, 34792, 39833, 23254, 23279, 24694, 23975, 
            23998, 24719, 28318, 28295, 33334, 38375, 38398, 33359, 29014, 29735, 
            39094, 34055, 34774, 39815, 29758, 29039, 34078, 39119, 39838, 34799, 
            11792, 11795, 11804, 11799, 11800, 11807, 11914, 11913, 12032, 12153, 
            12154, 12035, 11918, 11925, 12158, 12039, 12044, 12165, 11926, 11921, 
            12040, 12161, 12166, 12047, 12514, 12513, 12518, 12525, 12526, 12521, 
            13232, 13235, 14672, 13953, 13954, 14675, 13244, 13239, 13958, 14679, 
            14684, 13965, 13240, 13247, 14680, 13961, 13966, 14687, 12632, 12635, 
            12872, 12753, 12754, 12875, 13354, 13353, 14072, 14793, 14794, 14075, 
            13472, 13593, 14912, 14193, 14312, 15033, 13594, 13475, 14194, 14915, 
            15034, 14315, 12644, 12639, 12758, 12879, 12884, 12765, 13358, 13365, 
            14798, 14079, 14084, 14805, 13598, 13479, 14198, 14919, 15038, 14319, 
            13484, 13605, 14924, 14205, 14324, 15045, 12640, 12647, 12880, 12761, 
            12766, 12887, 13366, 13361, 14080, 14801, 14806, 14087, 13480, 13601, 
            14920, 14201, 14320, 15041, 13606, 13487, 14206, 14927, 15046, 14327, 
            16834, 16833, 16838, 16845, 16846, 16841, 16952, 16955, 17192, 17073, 
            17074, 17195, 16964, 16959, 17078, 17199, 17204, 17085, 16960, 16967, 
            17200, 17081, 17086, 17207, 21872, 21875, 21884, 21879, 21880, 21887, 
            26914, 26913, 31952, 36993, 36994, 31955, 26918, 26925, 36998, 31959, 
            31964, 37005, 26926, 26921, 31960, 37001, 37006, 31967, 21994, 21993, 
            22112, 22233, 22234, 22115, 27032, 27035, 37112, 32073, 32074, 37115, 
            27272, 27153, 32192, 37233, 37352, 32313, 27154, 27275, 37234, 32195, 
            32314, 37355, 21998, 22005, 22238, 22119, 22124, 22245, 27044, 27039, 
            32078, 37119, 37124, 32085, 27158, 27279, 37238, 32199, 32318, 37359, 
            27284, 27165, 32204, 37245, 37364, 32325, 22006, 22001, 22120, 22241, 
            22246, 22127, 27040, 27047, 37120, 32081, 32086, 37127, 27280, 27161, 
            32200, 37241, 37360, 32321, 27166, 27287, 37246, 32207, 32326, 37367, 
            17552, 17555, 17564, 17559, 17560, 17567, 18274, 18273, 18992, 19713, 
            19714, 18995, 18278, 18285, 19718, 18999, 19004, 19725, 18286, 18281, 
            19000, 19721, 19726, 19007, 22594, 22593, 22598, 22605, 22606, 22601, 
            27632, 27635, 37712, 32673, 32674, 37715, 27644, 27639, 32678, 37719, 
            37724, 32685, 27640, 27647, 37720, 32681, 32686, 37727, 23312, 23315, 
            24752, 24033, 24034, 24755, 28354, 28353, 33392, 38433, 38434, 33395, 
            29072, 29793, 39152, 34113, 34832, 39873, 29794, 29075, 34114, 39155, 
            39874, 34835, 23324, 23319, 24038, 24759, 24764, 24045, 28358, 28365, 
            38438, 33399, 33404, 38445, 29798, 29079, 34118, 39159, 39878, 34839, 
            29084, 29805, 39164, 34125, 34844, 39885, 23320, 23327, 24760, 24041, 
            24046, 24767, 28366, 28361, 33400, 38441, 38446, 33407, 29080, 29801, 
            39160, 34121, 34840, 39881, 29806, 29087, 34126, 39167, 39886, 34847, 
            17674, 17673, 17792, 17913, 17914, 17795, 18392, 18395, 19832, 19113, 
            19114, 19835, 18632, 18513, 19232, 19953, 20072, 19353, 18514, 18635, 
            19954, 19235, 19354, 20075, 22712, 22715, 22952, 22833, 22834, 22955, 
            27754, 27753, 32792, 37833, 37834, 32795, 27872, 27993, 37952, 32913, 
            33032, 38073, 27994, 27875, 32914, 37955, 38074, 33035, 23434, 23433, 
            24152, 24873, 24874, 24155, 28472, 28475, 38552, 33513, 33514, 38555, 
            29912, 29193, 34232, 39273, 39992, 34953, 29194, 29915, 39274, 34235, 
            34954, 39995, 23552, 23673, 24992, 24273, 24392, 25113, 28712, 28593, 
            33632, 38673, 38792, 33753, 29312, 30033, 39392, 34353, 35072, 40113, 
            30152, 29433, 34472, 39513, 40232, 35193, 23674, 23555, 24274, 24995, 
            25114, 24395, 28594, 28715, 38674, 33635, 33754, 38795, 30034, 29315, 
            34354, 39395, 40114, 35075, 29434, 30155, 39514, 34475, 35194, 40235, 
            17678, 17685, 17918, 17799, 17804, 17925, 18404, 18399, 19118, 19839, 
            19844, 19125, 18518, 18639, 19958, 19239, 19358, 20079, 18644, 18525, 
            19244, 19965, 20084, 19365, 22724, 22719, 22838, 22959, 22964, 22845, 
            27758, 27765, 37838, 32799, 32804, 37845, 27998, 27879, 32918, 37959, 
            38078, 33039, 27884, 28005, 37964, 32925, 33044, 38085, 23438, 23445, 
            24878, 24159, 24164, 24885, 28484, 28479, 33518, 38559, 38564, 33525, 
            29198, 29919, 39278, 34239, 34958, 39999, 29924, 29205, 34244, 39285, 
            40004, 34965, 23678, 23559, 24278, 24999, 25118, 24399, 28598, 28719, 
            38678, 33639, 33758, 38799, 30038, 29319, 34358, 39399, 40118, 35079, 
            29438, 30159, 39518, 34479, 35198, 40239, 23564, 23685, 25004, 24285, 
            24404, 25125, 28724, 28605, 33644, 38685, 38804, 33765, 29324, 30045, 
            39404, 34365, 35084, 40125, 30164, 29445, 34484, 39525, 40244, 35205, 
            17686, 17681, 17800, 17921, 17926, 17807, 18400, 18407, 19840, 19121, 
            19126, 19847, 18640, 18521, 19240, 19961, 20080, 19361, 18526, 18647, 
            19966, 19247, 19366, 20087, 22720, 22727, 22960, 22841, 22846, 22967, 
            27766, 27761, 32800, 37841, 37846, 32807, 27880, 28001, 37960, 32921, 
            33040, 38081, 28006, 27887, 32926, 37967, 38086, 33047, 23446, 23441, 
            24160, 24881, 24886, 24167, 28480, 28487, 38560, 33521, 33526, 38567, 
            29920, 29201, 34240, 39281, 40000, 34961, 29206, 29927, 39286, 34247, 
            34966, 40007, 23560, 23681, 25000, 24281, 24400, 25121, 28720, 28601, 
            33640, 38681, 38800, 33761, 29320, 30041, 39400, 34361, 35080, 40121, 
            30160, 29441, 34480, 39521, 40240, 35201, 23686, 23567, 24286, 25007, 
            25126, 24407, 28606, 28727, 38686, 33647, 33766, 38807, 30046, 29327, 
            34366, 39407, 40126, 35087, 29446, 30167, 39526, 34487, 35206, 40247, 
            11818, 11817, 11840, 11865, 11866, 11843, 11936, 11939, 12176, 12057, 
            12058, 12179, 11984, 11961, 12080, 12201, 12224, 12105, 11962, 11987, 
            12202, 12083, 12106, 12227, 12536, 12539, 12584, 12561, 12562, 12587, 
            13258, 13257, 13976, 14697, 14698, 13979, 13280, 13305, 14720, 14001, 
            14024, 14745, 13306, 13283, 14002, 14723, 14746, 14027, 12658, 12657, 
            12776, 12897, 12898, 12779, 13376, 13379, 14816, 14097, 14098, 14819, 
            13616, 13497, 14216, 14937, 15056, 14337, 13498, 13619, 14938, 14219, 
            14338, 15059, 12680, 12705, 12920, 12801, 12824, 12945, 13424, 13401, 
            14120, 14841, 14864, 14145, 13520, 13641, 14960, 14241, 14360, 15081, 
            13664, 13545, 14264, 14985, 15104, 14385, 12706, 12683, 12802, 12923, 
            12946, 12827, 13402, 13427, 14842, 14123, 14146, 14867, 13642, 13523, 
            14242, 14963, 15082, 14363, 13546, 13667, 14986, 14267, 14386, 15107, 
            16856, 16859, 16904, 16881, 16882, 16907, 16978, 16977, 17096, 17217, 
            17218, 17099, 17000, 17025, 17240, 17121, 17144, 17265, 17026, 17003, 
            17122, 17243, 17266, 17147, 21898, 21897, 21920, 21945, 21946, 21923, 
            26936, 26939, 37016, 31977, 31978, 37019, 26984, 26961, 32000, 37041, 
            37064, 32025, 26962, 26987, 37042, 32003, 32026, 37067, 22016, 22019, 
            22256, 22137, 22138, 22259, 27058, 27057, 32096, 37137, 37138, 32099, 
            27176, 27297, 37256, 32217, 32336, 37377, 27298, 27179, 32218, 37259, 
            37378, 32339, 22064, 22041, 22160, 22281, 22304, 22185, 27080, 27105, 
            37160, 32121, 32144, 37185, 27320, 27201, 32240, 37281, 37400, 32361, 
            27224, 27345, 37304, 32265, 32384, 37425, 22042, 22067, 22282, 22163, 
            22186, 22307, 27106, 27083, 32122, 37163, 37186, 32147, 27202, 27323, 
            37282, 32243, 32362, 37403, 27346, 27227, 32266, 37307, 37426, 32387, 
            17578, 17577, 17600, 17625, 17626, 17603, 18296, 18299, 19736, 19017, 
            19018, 19739, 18344, 18321, 19040, 19761, 19784, 19065, 18322, 18347, 
            19762, 19043, 19066, 19787, 22616, 22619, 22664, 22641, 22642, 22667, 
            27658, 27657, 32696, 37737, 37738, 32699, 27680, 27705, 37760, 32721, 
            32744, 37785, 27706, 27683, 32722, 37763, 37786, 32747, 23338, 23337, 
            24056, 24777, 24778, 24059, 28376, 28379, 38456, 33417, 33418, 38459, 
            29816, 29097, 34136, 39177, 39896, 34857, 29098, 29819, 39178, 34139, 
            34858, 39899, 23360, 23385, 24800, 24081, 24104, 24825, 28424, 28401, 
            33440, 38481, 38504, 33465, 29120, 29841, 39200, 34161, 34880, 39921, 
            29864, 29145, 34184, 39225, 39944, 34905, 23386, 23363, 24082, 24803, 
            24826, 24107, 28402, 28427, 38482, 33443, 33466, 38507, 29842, 29123, 
            34162, 39203, 39922, 34883, 29146, 29867, 39226, 34187, 34906, 39947, 
            17696, 17699, 17936, 17817, 17818, 17939, 18418, 18417, 19136, 19857, 
            19858, 19139, 18536, 18657, 19976, 19257, 19376, 20097, 18658, 18539, 
            19258, 19979, 20098, 19379, 22738, 22737, 22856, 22977, 22978, 22859, 
            27776, 27779, 37856, 32817, 32818, 37859, 28016, 27897, 32936, 37977, 
            38096, 33057, 27898, 28019, 37978, 32939, 33058, 38099, 23456, 23459, 
            24896, 24177, 24178, 24899, 28498, 28497, 33536, 38577, 38578, 33539, 
            29216, 29937, 39296, 34257, 34976, 40017, 29938, 29219, 34258, 39299, 
            40018, 34979, 23696, 23577, 24296, 25017, 25136, 24417, 28616, 28737, 
            38696, 33657, 33776, 38817, 30056, 29337, 34376, 39417, 40136, 35097, 
            29456, 30177, 39536, 34497, 35216, 40257, 23578, 23699, 25018, 24299, 
            24418, 25139, 28738, 28619, 33658, 38699, 38818, 33779, 29338, 30059, 
            39418, 34379, 35098, 40139, 30178, 29459, 34498, 39539, 40258, 35219, 
            17744, 17721, 17840, 17961, 17984, 17865, 18440, 18465, 19880, 19161, 
            19184, 19905, 18680, 18561, 19280, 20001, 20120, 19401, 18584, 18705, 
            20024, 19305, 19424, 20145, 22760, 22785, 23000, 22881, 22904, 23025, 
            27824, 27801, 32840, 37881, 37904, 32865, 27920, 28041, 38000, 32961, 
            33080, 38121, 28064, 27945, 32984, 38025, 38144, 33105, 23504, 23481, 
            24200, 24921, 24944, 24225, 28520, 28545, 38600, 33561, 33584, 38625, 
            29960, 29241, 34280, 39321, 40040, 35001, 29264, 29985, 39344, 34305, 
            35024, 40065, 23600, 23721, 25040, 24321, 24440, 25161, 28760, 28641, 
            33680, 38721, 38840, 33801, 29360, 30081, 39440, 34401, 35120, 40161, 
            30200, 29481, 34520, 39561, 40280, 35241, 23744, 23625, 24344, 25065, 
            25184, 24465, 28664, 28785, 38744, 33705, 33824, 38865, 30104, 29385, 
            34424, 39465, 40184, 35145, 29504, 30225, 39584, 34545, 35264, 40305, 
            17722, 17747, 17962, 17843, 17866, 17987, 18466, 18443, 19162, 19883, 
            19906, 19187, 18562, 18683, 20002, 19283, 19402, 20123, 18706, 18587, 
            19306, 20027, 20146, 19427, 22786, 22763, 22882, 23003, 23026, 22907, 
            27802, 27827, 37882, 32843, 32866, 37907, 28042, 27923, 32962, 38003, 
            38122, 33083, 27946, 28067, 38026, 32987, 33106, 38147, 23482, 23507, 
            24922, 24203, 24226, 24947, 28546, 28523, 33562, 38603, 38626, 33587, 
            29242, 29963, 39322, 34283, 35002, 40043, 29986, 29267, 34306, 39347, 
            40066, 35027, 23722, 23603, 24322, 25043, 25162, 24443, 28642, 28763, 
            38722, 33683, 33802, 38843, 30082, 29363, 34402, 39443, 40162, 35123, 
            29482, 30203, 39562, 34523, 35242, 40283, 23626, 23747, 25066, 24347, 
            24466, 25187, 28786, 28667, 33706, 38747, 38866, 33827, 29386, 30107, 
            39466, 34427, 35146, 40187, 30226, 29507, 34546, 39587, 40306, 35267, 
            11822, 11829, 11870, 11847, 11852, 11877, 11948, 11943, 12062, 12183, 
            12188, 12069, 11966, 11991, 12206, 12087, 12110, 12231, 11996, 11973, 
            12092, 12213, 12236, 12117, 12548, 12543, 12566, 12591, 12596, 12573, 
            13262, 13269, 14702, 13983, 13988, 14709, 13310, 13287, 14006, 14727, 
            14750, 14031, 13292, 13317, 14732, 14013, 14036, 14757, 12662, 12669, 
            12902, 12783, 12788, 12909, 13388, 13383, 14102, 14823, 14828, 14109, 
            13502, 13623, 14942, 14223, 14342, 15063, 13628, 13509, 14228, 14949, 
            15068, 14349, 12710, 12687, 12806, 12927, 12950, 12831, 13406, 13431, 
            14846, 14127, 14150, 14871, 13646, 13527, 14246, 14967, 15086, 14367, 
            13550, 13671, 14990, 14271, 14390, 15111, 12692, 12717, 12932, 12813, 
            12836, 12957, 13436, 13413, 14132, 14853, 14876, 14157, 13532, 13653, 
            14972, 14253, 14372, 15093, 13676, 13557, 14276, 14997, 15116, 14397, 
            16868, 16863, 16886, 16911, 16916, 16893, 16982, 16989, 17222, 17103, 
            17108, 17229, 17030, 17007, 17126, 17247, 17270, 17151, 17012, 17037, 
            17252, 17133, 17156, 17277, 21902, 21909, 21950, 21927, 21932, 21957, 
            26948, 26943, 31982, 37023, 37028, 31989, 26966, 26991, 37046, 32007, 
            32030, 37071, 26996, 26973, 32012, 37053, 37076, 32037, 22028, 22023, 
            22142, 22263, 22268, 22149, 27062, 27069, 37142, 32103, 32108, 37149, 
            27302, 27183, 32222, 37263, 37382, 32343, 27188, 27309, 37268, 32229, 
            32348, 37389, 22046, 22071, 22286, 22167, 22190, 22311, 27110, 27087, 
            32126, 37167, 37190, 32151, 27206, 27327, 37286, 32247, 32366, 37407, 
            27350, 27231, 32270, 37311, 37430, 32391, 22076, 22053, 22172, 22293, 
            22316, 22197, 27092, 27117, 37172, 32133, 32156, 37197, 27332, 27213, 
            32252, 37293, 37412, 32373, 27236, 27357, 37316, 32277, 32396, 37437, 
            17582, 17589, 17630, 17607, 17612, 17637, 18308, 18303, 19022, 19743, 
            19748, 19029, 18326, 18351, 19766, 19047, 19070, 19791, 18356, 18333, 
            19052, 19773, 19796, 19077, 22628, 22623, 22646, 22671, 22676, 22653, 
            27662, 27669, 37742, 32703, 32708, 37749, 27710, 27687, 32726, 37767, 
            37790, 32751, 27692, 27717, 37772, 32733, 32756, 37797, 23342, 23349, 
            24782, 24063, 24068, 24789, 28388, 28383, 33422, 38463, 38468, 33429, 
            29102, 29823, 39182, 34143, 34862, 39903, 29828, 29109, 34148, 39189, 
            39908, 34869, 23390, 23367, 24086, 24807, 24830, 24111, 28406, 28431, 
            38486, 33447, 33470, 38511, 29846, 29127, 34166, 39207, 39926, 34887, 
            29150, 29871, 39230, 34191, 34910, 39951, 23372, 23397, 24812, 24093, 
            24116, 24837, 28436, 28413, 33452, 38493, 38516, 33477, 29132, 29853, 
            39212, 34173, 34892, 39933, 29876, 29157, 34196, 39237, 39956, 34917, 
            17708, 17703, 17822, 17943, 17948, 17829, 18422, 18429, 19862, 19143, 
            19148, 19869, 18662, 18543, 19262, 19983, 20102, 19383, 18548, 18669, 
            19988, 19269, 19388, 20109, 22742, 22749, 22982, 22863, 22868, 22989, 
            27788, 27783, 32822, 37863, 37868, 32829, 27902, 28023, 37982, 32943, 
            33062, 38103, 28028, 27909, 32948, 37989, 38108, 33069, 23468, 23463, 
            24182, 24903, 24908, 24189, 28502, 28509, 38582, 33543, 33548, 38589, 
            29942, 29223, 34262, 39303, 40022, 34983, 29228, 29949, 39308, 34269, 
            34988, 40029, 23582, 23703, 25022, 24303, 24422, 25143, 28742, 28623, 
            33662, 38703, 38822, 33783, 29342, 30063, 39422, 34383, 35102, 40143, 
            30182, 29463, 34502, 39543, 40262, 35223, 23708, 23589, 24308, 25029, 
            25148, 24429, 28628, 28749, 38708, 33669, 33788, 38829, 30068, 29349, 
            34388, 39429, 40148, 35109, 29468, 30189, 39548, 34509, 35228, 40269, 
            17726, 17751, 17966, 17847, 17870, 17991, 18470, 18447, 19166, 19887, 
            19910, 19191, 18566, 18687, 20006, 19287, 19406, 20127, 18710, 18591, 
            19310, 20031, 20150, 19431, 22790, 22767, 22886, 23007, 23030, 22911, 
            27806, 27831, 37886, 32847, 32870, 37911, 28046, 27927, 32966, 38007, 
            38126, 33087, 27950, 28071, 38030, 32991, 33110, 38151, 23486, 23511, 
            24926, 24207, 24230, 24951, 28550, 28527, 33566, 38607, 38630, 33591, 
            29246, 29967, 39326, 34287, 35006, 40047, 29990, 29271, 34310, 39351, 
            40070, 35031, 23726, 23607, 24326, 25047, 25166, 24447, 28646, 28767, 
            38726, 33687, 33806, 38847, 30086, 29367, 34406, 39447, 40166, 35127, 
            29486, 30207, 39566, 34527, 35246, 40287, 23630, 23751, 25070, 24351, 
            24470, 25191, 28790, 28671, 33710, 38751, 38870, 33831, 29390, 30111, 
            39470, 34431, 35150, 40191, 30230, 29511, 34550, 39591, 40310, 35271, 
            17756, 17733, 17852, 17973, 17996, 17877, 18452, 18477, 19892, 19173, 
            19196, 19917, 18692, 18573, 19292, 20013, 20132, 19413, 18596, 18717, 
            20036, 19317, 19436, 20157, 22772, 22797, 23012, 22893, 22916, 23037, 
            27836, 27813, 32852, 37893, 37916, 32877, 27932, 28053, 38012, 32973, 
            33092, 38133, 28076, 27957, 32996, 38037, 38156, 33117, 23516, 23493, 
            24212, 24933, 24956, 24237, 28532, 28557, 38612, 33573, 33596, 38637, 
            29972, 29253, 34292, 39333, 40052, 35013, 29276, 29997, 39356, 34317, 
            35036, 40077, 23612, 23733, 25052, 24333, 24452, 25173, 28772, 28653, 
            33692, 38733, 38852, 33813, 29372, 30093, 39452, 34413, 35132, 40173, 
            30212, 29493, 34532, 39573, 40292, 35253, 23756, 23637, 24356, 25077, 
            25196, 24477, 28676, 28797, 38756, 33717, 33836, 38877, 30116, 29397, 
            34436, 39477, 40196, 35157, 29516, 30237, 39596, 34557, 35276, 40317, 
            11830, 11825, 11848, 11873, 11878, 11855, 11944, 11951, 12184, 12065, 
            12070, 12191, 11992, 11969, 12088, 12209, 12232, 12113, 11974, 11999, 
            12214, 12095, 12118, 12239, 12544, 12551, 12592, 12569, 12574, 12599, 
            13270, 13265, 13984, 14705, 14710, 13991, 13288, 13313, 14728, 14009, 
            14032, 14753, 13318, 13295, 14014, 14735, 14758, 14039, 12670, 12665, 
            12784, 12905, 12910, 12791, 13384, 13391, 14824, 14105, 14110, 14831, 
            13624, 13505, 14224, 14945, 15064, 14345, 13510, 13631, 14950, 14231, 
            14350, 15071, 12688, 12713, 12928, 12809, 12832, 12953, 13432, 13409, 
            14128, 14849, 14872, 14153, 13528, 13649, 14968, 14249, 14368, 15089, 
            13672, 13553, 14272, 14993, 15112, 14393, 12718, 12695, 12814, 12935, 
            12958, 12839, 13414, 13439, 14854, 14135, 14158, 14879, 13654, 13535, 
            14254, 14975, 15094, 14375, 13558, 13679, 14998, 14279, 14398, 15119, 
            16864, 16871, 16912, 16889, 16894, 16919, 16990, 16985, 17104, 17225, 
            17230, 17111, 17008, 17033, 17248, 17129, 17152, 17273, 17038, 17015, 
            17134, 17255, 17278, 17159, 21910, 21905, 21928, 21953, 21958, 21935, 
            26944, 26951, 37024, 31985, 31990, 37031, 26992, 26969, 32008, 37049, 
            37072, 32033, 26974, 26999, 37054, 32015, 32038, 37079, 22024, 22031, 
            22264, 22145, 22150, 22271, 27070, 27065, 32104, 37145, 37150, 32111, 
            27184, 27305, 37264, 32225, 32344, 37385, 27310, 27191, 32230, 37271, 
            37390, 32351, 22072, 22049, 22168, 22289, 22312, 22193, 27088, 27113, 
            37168, 32129, 32152, 37193, 27328, 27209, 32248, 37289, 37408, 32369, 
            27232, 27353, 37312, 32273, 32392, 37433, 22054, 22079, 22294, 22175, 
            22198, 22319, 27118, 27095, 32134, 37175, 37198, 32159, 27214, 27335, 
            37294, 32255, 32374, 37415, 27358, 27239, 32278, 37319, 37438, 32399, 
            17590, 17585, 17608, 17633, 17638, 17615, 18304, 18311, 19744, 19025, 
            19030, 19751, 18352, 18329, 19048, 19769, 19792, 19073, 18334, 18359, 
            19774, 19055, 19078, 19799, 22624, 22631, 22672, 22649, 22654, 22679, 
            27670, 27665, 32704, 37745, 37750, 32711, 27688, 27713, 37768, 32729, 
            32752, 37793, 27718, 27695, 32734, 37775, 37798, 32759, 23350, 23345, 
            24064, 24785, 24790, 24071, 28384, 28391, 38464, 33425, 33430, 38471, 
            29824, 29105, 34144, 39185, 39904, 34865, 29110, 29831, 39190, 34151, 
            34870, 39911, 23368, 23393, 24808, 24089, 24112, 24833, 28432, 28409, 
            33448, 38489, 38512, 33473, 29128, 29849, 39208, 34169, 34888, 39929, 
            29872, 29153, 34192, 39233, 39952, 34913, 23398, 23375, 24094, 24815, 
            24838, 24119, 28414, 28439, 38494, 33455, 33478, 38519, 29854, 29135, 
            34174, 39215, 39934, 34895, 29158, 29879, 39238, 34199, 34918, 39959, 
            17704, 17711, 17944, 17825, 17830, 17951, 18430, 18425, 19144, 19865, 
            19870, 19151, 18544, 18665, 19984, 19265, 19384, 20105, 18670, 18551, 
            19270, 19991, 20110, 19391, 22750, 22745, 22864, 22985, 22990, 22871, 
            27784, 27791, 37864, 32825, 32830, 37871, 28024, 27905, 32944, 37985, 
            38104, 33065, 27910, 28031, 37990, 32951, 33070, 38111, 23464, 23471, 
            24904, 24185, 24190, 24911, 28510, 28505, 33544, 38585, 38590, 33551, 
            29224, 29945, 39304, 34265, 34984, 40025, 29950, 29231, 34270, 39311, 
            40030, 34991, 23704, 23585, 24304, 25025, 25144, 24425, 28624, 28745, 
            38704, 33665, 33784, 38825, 30064, 29345, 34384, 39425, 40144, 35105, 
            29464, 30185, 39544, 34505, 35224, 40265, 23590, 23711, 25030, 24311, 
            24430, 25151, 28750, 28631, 33670, 38711, 38830, 33791, 29350, 30071, 
            39430, 34391, 35110, 40151, 30190, 29471, 34510, 39551, 40270, 35231, 
            17752, 17729, 17848, 17969, 17992, 17873, 18448, 18473, 19888, 19169, 
            19192, 19913, 18688, 18569, 19288, 20009, 20128, 19409, 18592, 18713, 
            20032, 19313, 19432, 20153, 22768, 22793, 23008, 22889, 22912, 23033, 
            27832, 27809, 32848, 37889, 37912, 32873, 27928, 28049, 38008, 32969, 
            33088, 38129, 28072, 27953, 32992, 38033, 38152, 33113, 23512, 23489, 
            24208, 24929, 24952, 24233, 28528, 28553, 38608, 33569, 33592, 38633, 
            29968, 29249, 34288, 39329, 40048, 35009, 29272, 29993, 39352, 34313, 
            35032, 40073, 23608, 23729, 25048, 24329, 24448, 25169, 28768, 28649, 
            33688, 38729, 38848, 33809, 29368, 30089, 39448, 34409, 35128, 40169, 
            30208, 29489, 34528, 39569, 40288, 35249, 23752, 23633, 24352, 25073, 
            25192, 24473, 28672, 28793, 38752, 33713, 33832, 38873, 30112, 29393, 
            34432, 39473, 40192, 35153, 29512, 30233, 39592, 34553, 35272, 40313, 
            17734, 17759, 17974, 17855, 17878, 17999, 18478, 18455, 19174, 19895, 
            19918, 19199, 18574, 18695, 20014, 19295, 19414, 20135, 18718, 18599, 
            19318, 20039, 20158, 19439, 22798, 22775, 22894, 23015, 23038, 22919, 
            27814, 27839, 37894, 32855, 32878, 37919, 28054, 27935, 32974, 38015, 
            38134, 33095, 27958, 28079, 38038, 32999, 33118, 38159, 23494, 23519, 
            24934, 24215, 24238, 24959, 28558, 28535, 33574, 38615, 38638, 33599, 
            29254, 29975, 39334, 34295, 35014, 40055, 29998, 29279, 34318, 39359, 
            40078, 35039, 23734, 23615, 24334, 25055, 25174, 24455, 28654, 28775, 
            38734, 33695, 33814, 38855, 30094, 29375, 34414, 39455, 40174, 35135, 
            29494, 30215, 39574, 34535, 35254, 40295, 23638, 23759, 25078, 24359, 
            24478, 25199, 28798, 28679, 33718, 38759, 38878, 33839, 29398, 30119, 
            39478, 34439, 35158, 40199, 30238, 29519, 34558, 39599, 40318, 35279, 
        };

        /**
         * Contains a full table of two-element swaps.
         *
         * Specifically, the permutation that swaps \a x and \a y is
         * <tt>S8[swapTable[x][y]]</tt>.  Here \a x and \a y may be equal.
         */
        static constexpr Code2 swapTable[8][8] = {
            { 0, 5041, 10801, 15961, 21025, 26071, 31113, 36153 },
            { 5041, 0, 721, 1561, 2305, 3031, 3753, 4473 },
            { 10801, 721, 0, 121, 265, 391, 513, 633 },
            { 15961, 1561, 121, 0, 25, 55, 81, 105 },
            { 21025, 2305, 265, 25, 0, 7, 15, 21 },
            { 26071, 3031, 391, 55, 7, 0, 3, 5 },
            { 31113, 3753, 513, 81, 15, 3, 0, 1 },
            { 36153, 4473, 633, 105, 21, 5, 1, 0 }
        };

    protected:
        /**
         * Creates a permutation from the given second-generation
         * permutation code.
         *
         * \pre the given code is a valid second-generation permutation code;
         * see isPermCode2() for details.
         *
         * @param code the second-generation code from which the new
         * permutation will be created.
         */
        constexpr Perm<8>(Code2 code);

    private:
        /**
         * Returns the index into the Perm<8>::S8 array of the permutation that
         * maps (0,...,7) to (<i>a</i>,...,<i>f</i>,<i>g</i>,<i>h</i>) respectively.
         *
         * \pre {<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>,<i>e</i>,<i>f</i>,<i>g</i>,<i>h</i>}
         * = {0,1,2,3,4,5,6,7}.
         *
         * @param a the desired image of 0.
         * @param b the desired image of 1.
         * @param c the desired image of 2.
         * @param d the desired image of 3.
         * @param e the desired image of 4.
         * @param f the desired image of 5.
         * @param g the desired image of 6.
         * @param h the desired image of 7.
         * @return the index \a i for which the given permutation is equal to
         * Perm<8>::S8[i].  This will be between 0 and 40319 inclusive.
         */
        static constexpr int S8Index(int a, int b, int c, int d, int e, int f,
            int g, int h);

        /**
         * Converts between an index into Perm<8>::S8 and an index into
         * Perm<8>::orderedS8.  This conversion works in either direction.
         *
         * \tparam Int a native integer type; this would typically be
         * either \c int or \a Code2.
         */
        template <typename Int>
        static constexpr Int convOrderedUnordered(Int index);

        /**
         * Reconstructs a permutation from its given tight encoding.
         *
         * The tight encoding will be extracted one character at a time
         * beginning with the iterator \a start, in a single pass, without
         * skipping any leading whitespace.  If the iterator ever reaches
         * \a limit before the encoding is complete then the encoding is
         * treated as invalid (i.e., this routine will throw an exception).
         *
         * If \a noTrailingData is \c true then the iterator is required to
         * \e finish at \a limit, or else the encoding will be considered
         * invalid also; if \a noTrailingData is \c false then there is no
         * constraint on the final state of the iterator.
         *
         * \exception InvalidInput the given iterator does not point to
         * a tight encoding of a 8-element permutation.
         *
         * \tparam iterator an input iterator type.
         *
         * @param start an iterator that points to the beginning of a
         * tight encoding.
         * @param limit an iterator that, if reached, indicates that no more
         * characters are available.
         * @param noTrailingData \c true if iteration should reach \a limit
         * immediately after the encoding is read, or \c false if there is
         * allowed to be additional unread data.
         * @return the permutation represented by the given tight encoding.
         */
        template <typename iterator>
        static Perm tightDecode(iterator start, iterator limit,
            bool noTrailingData);
};

// Inline functions for Perm<8>

template <typename Int>
inline constexpr Int Perm<8>::convOrderedUnordered(Int index) {
    // S8 is almost the same as orderedS8, except that some pairs
    // S8[2i] <--> S8[2i+1] have been swapped to ensure that all
    // permutations S8[2i] are even and all permutations S8[2i+1] are odd.
    //
    // Specifically, we must flip between 2i <--> 2i+1 if and only if the
    // parities of (S8Index / 2), (S8Index / 24) and (S8Index / 720) XOR to 1.
    // Here we use (index >> 1), which is equivalent to (index / 2).
    //
    // A note for myself for if/when we extend this beyond 8:
    // what matters is the mod 2 sum of the parities of
    // (index / 2!), (index / 4!), (index / 6!), etc., up to (index / (n-1)!).
    //
    return ((((index >> 1) ^ (index / 24) ^ (index / 720)) & 1) ?
        (index ^ 1) : index);
}

inline constexpr Perm<8> Perm<8>::S8Lookup::operator[] (int index) const {
    return Perm<8>(static_cast<Code2>(index));
}

inline constexpr Perm<8> Perm<8>::OrderedS8Lookup::operator[] (int index)
        const {
    return Perm<8>(static_cast<Code2>(convOrderedUnordered(index)));
}

inline constexpr Perm<8>::Perm() : code2_(0) {
}

inline constexpr Perm<8>::Perm(Code2 code) : code2_(code) {
}

inline constexpr Perm<8>::Perm(int a, int b) : code2_(swapTable[a][b]) {
}

inline constexpr Perm<8>::Perm(int a, int b, int c, int d, int e, int f,
        int g, int h) : code2_(static_cast<Code2>(S8Index(a, b, c, d, e, f, g, h))) {
}

inline constexpr Perm<8>::Perm(const std::array<int, 8>& image) :
        code2_(static_cast<Code2>(S8Index(
            image[0], image[1], image[2], image[3], image[4], image[5],
            image[6], image[7]))) {
}

inline constexpr Perm<8>::Perm(int a0, int a1, int b0, int b1,
        int c0, int c1, int d0, int d1, int e0, int e1, int f0, int f1,
        int g0, int g1, int h0, int h1) :
        code2_(0) {
    // TODO: When we move to C++20, we can get rid of the zero initialisers.
    int image[8] = { 0, 0, 0, 0, 0, 0, 0 };
    image[a0] = a1;
    image[b0] = b1;
    image[c0] = c1;
    image[d0] = d1;
    image[e0] = e1;
    image[f0] = f1;
    image[g0] = g1;
    image[h0] = h1;
    code2_ = static_cast<Code2>(S8Index(
        image[0], image[1], image[2], image[3], image[4], image[5], image[6],
        image[7]));
}

inline constexpr Perm<8>::Code1 Perm<8>::permCode1() const {
    // This basically follows the implementation of operator [], but reuses
    // the parts of the computation that are common to each image calculation.
    int slice = code2_ / 5040;
    Perm<7> suffix = Perm<7>::Sn[(code2_ % 5040) ^ (slice & 1)];

    Code1 ans = slice;
    for (int i = 0; i < 7; ++i) {
        int img = suffix[i];
        if (img >= slice)
            ++img;
        ans |= (static_cast<Code1>(img) << (3 * (i + 1)));
    }

    return ans;
}

inline constexpr Perm<8>::Code2 Perm<8>::permCode2() const {
    return code2_;
}

inline void Perm<8>::setPermCode1(Code1 code) {
    code2_ = static_cast<Code2>(S8Index(
        code & 0x07,
        (code >> 3) & 0x07,
        (code >> 6) & 0x07,
        (code >> 9) & 0x07,
        (code >> 12) & 0x07,
        (code >> 15) & 0x07,
        (code >> 18) & 0x07,
        (code >> 21) & 0x07));
}

inline void Perm<8>::setPermCode2(Code2 code) {
    code2_ = code;
}

inline constexpr Perm<8> Perm<8>::fromPermCode1(Code1 code) {
    return Perm<8>(static_cast<Code2>(S8Index(
        code & 0x07,
        (code >> 3) & 0x07,
        (code >> 6) & 0x07,
        (code >> 9) & 0x07,
        (code >> 12) & 0x07,
        (code >> 15) & 0x07,
        (code >> 18) & 0x07,
        (code >> 21) & 0x07)));
}

inline constexpr Perm<8> Perm<8>::fromPermCode2(Code2 code) {
    return Perm<8>(code);
}

inline constexpr bool Perm<8>::isPermCode1(Code1 code) {
    unsigned mask = 0;
    for (int i = 0; i < 8; i++)
        mask |= (1 << ((code >> (3 * i)) & 7));
            // mask |= (1 << imageOf(i));
    return (mask == 255 && (code >> 24) == 0);
}

inline constexpr bool Perm<8>::isPermCode2(Code2 code) {
    // code >= 0 is automatic because we are using an unsigned data type.
    return (code < 40320);
}

inline constexpr Perm<8>::ImagePack Perm<8>::imagePack() const {
    return permCode1();
}

inline constexpr Perm<8> Perm<8>::fromImagePack(ImagePack pack) {
    return fromPermCode1(pack);
}

inline constexpr bool Perm<8>::isImagePack(ImagePack pack) {
    return isPermCode1(pack);
}

inline constexpr Perm<8> Perm<8>::operator *(const Perm<8>& q) const {
    // TODO: Speed this up.
    // We are not fussing too hard about this right now, since if you
    // really care about speed then you should be using cachedComp() instead.
    // Still: at the very least, there is code within operator [] that can be
    // reused instead of being recomputed for all eight images below.
    return Perm<8>((*this)[q[0]], (*this)[q[1]], (*this)[q[2]], (*this)[q[3]],
        (*this)[q[4]], (*this)[q[5]], (*this)[q[6]], (*this)[q[7]]);
}

inline Perm<8> Perm<8>::cachedComp(const Perm<8>& q) const {
    return Perm<8>(products_[code2_][q.code2_]);
}

inline Perm<8> Perm<8>::cachedComp(const Perm<8>& q, const Perm<8>& r) const {
    return Perm<8>(products_[code2_][products_[q.code2_][r.code2_]]);
}

inline constexpr Perm<8> Perm<8>::inverse() const {
    return Perm<8>(invS8[code2_]);
}

inline constexpr Perm<8> Perm<8>::pow(long exp) const {
    // We will just iterate the exponentiation here.
    // The maximum order is 15, so this will involve at most 7 compositions.
    // If you really need this to be fast, use cachedPow() instead.

    int ord = order();

    exp %= ord;
    if (exp == 0)
        return Perm<8>();

    // Normalise exp to be in the range ( -order/2, +order/2 ].
    if (exp < 0)
        exp += ord;
    if ((exp << 1) > ord)
        exp -= ord;

    if (exp > 0) {
        Perm<8> p = *this;
        --exp;
        while (exp) {
            p = p * (*this);
            --exp;
        }
        return p;
    } else {
        Perm<8> inv = inverse();
        Perm<8> p = inv;
        ++exp;
        while (exp) {
            p = p * inv;
            ++exp;
        }
        return p;
    }
}

inline Perm<8> Perm<8>::cachedPow(long exp) const {
    // Normalise exp in the range [0, order), and look up the result.
    int ord = cachedOrder();
    exp %= ord;
    if (exp == 0)
        return Perm<8>();
    if (exp < 0)
        exp += ord;
    return Perm<8>(powers_[code2_][exp]);
}

inline constexpr int Perm<8>::order() const {
    // TODO: Speed this up.
    // We are not fussing too hard about optimisation here, because if you
    // really care about speed then you would be using cachedOrder() instead.
    //
    // Still: there should be optimisations possible that look at the orders
    // of individual elements - in particular, how many elements have orders
    // 1, 2 and 3.  Using these counts, we should be able to work out the
    // order completely without powering any element beyond its cube.
    //
    // The possible overall orders, and the corresponding element orders, are:
    //
    // Order 1: orders 1 1 1 1 1 1 1 1
    // Order 2: orders 1 1 1 1 1 1 2 2, 1 1 1 1 2 2 2 2, 1 1 2 2 2 2 2 2, or 2 2 2 2 2 2 2 2
    // Order 3: orders 1 1 1 1 1 3 3 3, or 1 1 3 3 3 3 3 3
    // Order 4: orders 1 1 1 1 4 4 4 4, 1 1 2 2 4 4 4 4, 2 2 2 2 4 4 4 4, or 4 4 4 4 4 4 4 4
    // Order 5: orders 1 1 1 5 5 5 5 5
    // Order 6: orders 1 1 1 2 2 3 3 3, 1 2 2 2 2 3 3 3, 2 2 3 3 3 3 3 3, 1 1 6 6 6 6 6 6, or 2 2 6 6 6 6 6 6
    // Order 7: orders 1 7 7 7 7 7 7 7
    // Order 8: orders 8 8 8 8 8 8 8 8
    // Order 10: orders 1 2 2 5 5 5 5 5
    // Order 12: orders 1 3 3 3 4 4 4 4
    // Order 15: orders 3 3 3 5 5 5 5 5

    // But anyway... for now, here is the brute force implementation, which
    // in the worse case performs a (costly) composition operation 15 times.

    Perm<8> p = *this;
    int ans = 1;
    while (! p.isIdentity()) {
        p = p * (*this);
        ++ans;
    }
    return ans;
}

inline int Perm<8>::cachedOrder() const {
    return orders_[code2_];
}

inline constexpr Perm<8> Perm<8>::reverse() const {
    // p becomes p * 76543210 (which has second-generation code 40318).
    return (*this) * Perm<8>(static_cast<Code2>(40318));
}

inline constexpr int Perm<8>::sign() const {
    return (code2_ % 2 ? -1 : 1);
}

inline constexpr int Perm<8>::operator[](int src) const {
    // We are trying to fly without a hard-coded table as best we can.
    // Our strategy: use the operator[] from Perm7, which only
    // require a small amount of work to adapt to Perm8.
    int slice = code2_ / 5040;
    if (src == 0) {
        return slice;
    } else {
        int img = Perm<7>::Sn[(code2_ % 5040) ^ (slice & 1)][src - 1];
        return (img < slice ? img : img + 1);
    }
}

inline constexpr int Perm<8>::pre(int image) const {
    return inverse()[image];
}

inline constexpr bool Perm<8>::operator == (const Perm<8>& other) const {
    return (code2_ == other.code2_);
}

inline constexpr bool Perm<8>::operator != (const Perm<8>& other) const {
    return (code2_ != other.code2_);
}

inline constexpr int Perm<8>::compareWith(const Perm<8>& other) const {
    // Computing orderedS8Index() is very fast, now that we use S8 indices
    // for internal permutation codes.  Use this instead of comparing images
    // one at a time.
    int o1 = orderedS8Index();
    int o2 = other.orderedS8Index();
    return (o1 == o2 ? 0 : o1 < o2 ? -1 : 1);
}

inline constexpr bool Perm<8>::isIdentity() const {
    return (code2_ == 0);
}

inline Perm<8>& Perm<8>::operator ++() {
    if (++code2_ == 40320)
        code2_ = 0;
    return *this;
}

inline constexpr Perm<8> Perm<8>::operator ++(int) {
    Perm<8> ans(code2_);
    if (++code2_ == 40320)
        code2_ = 0;
    return ans;
}

inline constexpr bool Perm<8>::operator < (const Perm<8>& rhs) const {
    return code2_ < rhs.code2_;
}

inline constexpr Perm<8> Perm<8>::rot(int i) {
    switch (i) {
        case 1: return Perm<8>(static_cast<Code2>(5913));
        case 2: return Perm<8>(static_cast<Code2>(11824));
        case 3: return Perm<8>(static_cast<Code2>(17731));
        case 4: return Perm<8>(static_cast<Code2>(23616));
        case 5: return Perm<8>(static_cast<Code2>(29401));
        case 6: return Perm<8>(static_cast<Code2>(34560));
        case 7: return Perm<8>(static_cast<Code2>(35281));
        default: return Perm<8>(); // Identity
    }
}

inline Perm<8> Perm<8>::rand(bool even) {
    RandomEngine engine;
    return rand(engine.engine(), even);
}

template <class URBG>
inline Perm<8> Perm<8>::rand(URBG&& gen, bool even) {
    if (even) {
        std::uniform_int_distribution<short> d(0, 20159);
        return S8[2 * d(gen)];
    } else {
        std::uniform_int_distribution<int> d(0, 40319);
        return S8[d(gen)];
    }
}

inline void Perm<8>::tightEncode(std::ostream& out) const {
    // Write the Sn index in base 36, least significant digit first.
    // Note: 36^3 = 46656 > 8! = 40320
    out << static_cast<char>((code2_ % 36) + 33)
        << static_cast<char>(((code2_ / 36) % 36) + 33)
        << static_cast<char>((code2_ / 1296) + 33);
}

inline std::string Perm<8>::tightEncoding() const {
    // Write the Sn index in base 36, least significant digit first.
    // Note: 36^3 = 46656 > 8! = 40320
    char ans[4] {
        static_cast<char>((code2_ % 36) + 33),
        static_cast<char>(((code2_ / 36) % 36) + 33),
        static_cast<char>((code2_ / 1296) + 33),
        0 };
    return ans;
}

inline Perm<8> Perm<8>::tightDecoding(const std::string& enc) {
    try {
        return tightDecode(enc.begin(), enc.end(), true);
    } catch (const InvalidInput& exc) {
        // For strings we use a different exception type.
        throw InvalidArgument(exc.what());
    }
}

inline Perm<8> Perm<8>::tightDecode(std::istream& input) {
    return tightDecode(std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>(), false);
}

template <typename iterator>
Perm<8> Perm<8>::tightDecode(iterator start, iterator limit,
        bool noTrailingData) {
    // All codes are >= 0 because we are using an unsigned data type.
    if (start == limit)
        throw InvalidInput("The tight encoding is incomplete");
    Code2 code0 = (*start++) - 33;
    if (code0 >= 36)
        throw InvalidInput("The tight encoding is invalid");

    if (start == limit)
        throw InvalidInput("The tight encoding is incomplete");
    Code2 code1 = (*start++) - 33;
    if (code1 >= 36)
        throw InvalidInput("The tight encoding is invalid");

    Code2 code2 = (*start++) - 33;
    if (code1 > 31 /* (8! / 36^2) */)
        throw InvalidInput("The tight encoding is invalid");

    code0 += (36 * code1) + (1296 * code2);
    if (code0 >= 40320 /* 8! */)
        throw InvalidInput("The tight encoding is invalid");
    if (noTrailingData && (start != limit))
        throw InvalidInput("The tight encoding has trailing characters");

    return Perm<8>(code0);
}

inline constexpr Perm<8>::Index Perm<8>::S8Index() const {
    return code2_;
}

inline constexpr Perm<8>::Index Perm<8>::SnIndex() const {
    return code2_;
}

inline constexpr Perm<8>::Index Perm<8>::orderedS8Index() const {
    return convOrderedUnordered(code2_);
}

inline constexpr Perm<8>::Index Perm<8>::orderedSnIndex() const {
    return convOrderedUnordered(code2_);
}

inline constexpr int Perm<8>::S8Index(int a, int b, int c, int d, int e,
        int f, int g, int h) {
    // First compute the ordered S8 index.
    int ans = 5040 * a +
              720 * (b - (b > a ? 1 : 0)) +
              120 * (c - ((c > b ? 1 : 0) + (c > a ? 1 : 0))) +
              24 * (d - ((d > c ? 1 : 0) + (d > b ? 1 : 0) + (d > a ? 1 : 0))) +
              6 * ((e > f ? 1 : 0) + (e > g ? 1 : 0) + (e > h ? 1 : 0)) +
              2 * ((f > g ? 1 : 0) + (f > h ? 1 : 0)) +
              (g > h ? 1 : 0);

    // Then switch to the plain (unordered) S8 index.
    return convOrderedUnordered(ans);
}

inline constexpr bool Perm<8>::isConjugacyMinimal() const {
    switch (code2_) {
        // Put the 22 minimal cases in a switch statement, which may help
        // the compiler to optimise this better than we can.
        case 0: case 1: case 2: case 6: case 9: case 27:
        case 32: case 127: case 128: case 146: case 153:
        case 746: case 753: case 849: case 872: case 5166: case 5169:
        case 5187: case 5192: case 5792: case 5888: case 5913:
            return true;
        default:
            return false;
    }
}

} // namespace regina

#endif

