
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#ifndef __NUMBERTHEORY_H
#ifndef __DOXYGEN
#define __NUMBERTHEORY_H
#endif

/*! \file numbertheory.h
 *  \brief Provides miscellaneous number theory routines.
 */

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "ndoublelist.h"
    #include "nmpi.h"
#else
    #include "engine/utilities/ndoublelist.h"
    #include "engine/utilities/nmpi.h"
#endif

/**
 * Reduces \a k modulo \a modBase to give the smallest possible
 * absolute value.  For instance, <tt>reducedMod(10,4) = 4</tt> but
 * <tt>reducedMod(10,6) = -4</tt>.  In the case of a tie, the positive
 * solution is taken.
 *
 * \pre \a modBase is strictly positive.
 *
 * \ifaces Not present.
 *
 * @param k the number to reduce modulo \a modBase.
 * @param modBase the modular base in which to work.
 */
long reducedMod(long k, long modBase);

/**
 * Calculates the greatest common divisor of two given integers.
 * This routine is not recursive.
 *
 * \pre Both integers are non-negative.
 *
 * \ifaces Not present.
 *
 * @param a one of the two integers to work with.
 * @param b the other integer with which to work.
 * @return the greatest common divisor of \a a and \a b.
 */
unsigned long gcd(unsigned long a, unsigned long b);

/**
 * Calculates the greatest common divisor of two given integers and finds the
 * smallest coefficients with which these integers combine to give their
 * gcd.
 * This routine is not recursive.
 *
 * Note that the given integers need not be non-negative.  However, the
 * gcd returned is guaranteed to be non-negative.
 *
 * If \a d is the gcd of \a a and \a b, the values placed in \a u and \a
 * v will be those for which <tt>u*a + v*b = d</tt>,
 * <tt>-abs(a)/d < v*sign(b) <= 0</tt> and
 * <tt>1 <= u*sign(a) <= abs(b)/d</tt>.
 *
 * \ifaces Not present.
 *
 * @param a one of the integers to work with.
 * @param b the other integer with which to work.
 * @param u a variable into which the final coefficient of \a a will be
 * placed.
 * @param v a variable into which the final coefficient of \a b will be
 * placed.
 * @return the greatest common divisor of \a a and \a b.
 */
long gcdWithCoeffs(long a, long b, long& u, long& v);

/**
 * Calculates the prime factorisation of the given integer.
 * All the prime factors will be inserted into the given list.
 * The algorithm used is <b>very neanderthal</b> and should only be used with
 * reasonably sized integers.  Don't use it to do RSA!
 *
 * If a prime factor is repeated, it will be inserted multiple times into
 * the list.  The primes in the list are not guaranteed to appear in any
 * specific order, nor are multiple occurrences of the same prime
 * guaranteed to appear together.
 *
 * Note that once finished the list will contain the prime factors as well
 * as whatever happened to be in the list before this function was
 * called.
 *
 * \pre The given integer is at least 1.
 *
 * \ifaces Not present.
 *
 * @param n the integer to factorise.
 * @param factors the list into which prime factors will be inserted.
 */
void factorise(unsigned long n, NDoubleList<unsigned long>& factors);

/**
 * Determines all primes up to and including the given upper bound.
 * All the primes found will be inserted into the given list in
 * increasing order.
 *
 * The algorithm currently used is <b>fairly neanderthal</b>.
 *
 * \pre The given list is empty.
 *
 * \ifaces Not present.
 *
 * @param roof the upper bound up to which primes will be found.
 * @param primes the list into which the primes will be inserted.
 */
void primesUpTo(const NLargeInteger& roof,
        NDoubleList<NLargeInteger>& primes);

#endif

