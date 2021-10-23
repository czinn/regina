
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

/*! \file surfaces/disc.h
 *  \brief Deals with individual normal discs and sets of normal discs
 *  in a normal surface.
 */

#ifndef __REGINA_DISC_H
#ifndef __DOXYGEN
#define __REGINA_DISC_H
#endif

#include <cassert>
#include <cstddef> // for nullptr_t
#include <iterator>
#include "regina-core.h"
#include "surfaces/normalsurface.h"
#include "triangulation/dim3.h"

namespace regina {

template <typename TetData> class DiscSpecIterator;

/**
 * Specifies a single normal disc in a normal surface.
 *
 * There are 10 disc types.  Types 0-3 represent triangles 0-3,
 * types 4-6 represent quads 0-2 and types 7-9 represent
 * octagons 0-2.
 *
 * Discs of a specific type are assigned numbers from 0 upwards.
 * Triangular discs are numbered outwards from the vertex they surround.
 * Quad discs and octagonal discs are numbered outwards away from vertex 0
 * of the tetrahedron.
 *
 * Note that, unlike DiscType in which the meaning of DiscType::type is
 * flexible, the meaning of DiscSpec::type is fixed as described above.
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \warning This class converts the indices of normal discs of a
 * given type from LargeInteger to <tt>unsigned long</tt>.  See the
 * precondition below.
 *
 * \pre The number of normal discs of a particular type
 * in a particular tetrahedron can be represented by a long integer.
 * \pre This class should only be used with \a embedded normal surfaces.
 *
 * \ingroup surfaces
 */
struct DiscSpec {
    size_t tetIndex;
        /**< The index in the triangulation of the tetrahedron
             containing the disc. */
    int type;
        /**< The disc type; this is between 0 and 9 inclusive, as described
             in the \a DiscSpec class notes. */
    unsigned long number;
        /**< Specifies which disc of the particular type in the
             particular tetrahedron is being referred to; discs
             are numbered as described in the \a DiscSpec class notes. */

    /**
     * Creates a new uninitialised disc specifier.
     */
    DiscSpec() = default;
    /**
     * Creates a new disc specifier containing the given values.
     *
     * @param newTetIndex the index in the triangulation of the tetrahedron
     * containing the disc.
     * @param newType the disc type; this is between 0 and 9 inclusive,
     * as described in the \a DiscSpec class notes.
     * @param newNumber specifies which disc of the particular type in the
     * particular tetrahedron is being referred to; discs are numbered
     * as described in the \a DiscSpec class notes.
     */
    DiscSpec(size_t newTetIndex, int newType, unsigned long newNumber);
    /**
     * Creates a new disc specifier that is a clone of the given specifier.
     *
     * @param cloneMe the disc specifier to clone.
     */
    DiscSpec(const DiscSpec& cloneMe) = default;

    /**
     * Copies the values from the given disc specifier into this specifier.
     *
     * @param cloneMe the disc specifier whose values should be copied.
     * @return a reference to this disc specifier.
     */
    DiscSpec& operator = (const DiscSpec& cloneMe) = default;
    /**
     * Determines if this and the given disc specifier contain identical
     * information.
     *
     * @param other the disc specifier to compare with this.
     * @return \c true if and only if this and the given disc specifier
     * contain identical information.
     */
    bool operator == (const DiscSpec& other) const;
    /**
     * Determines if this and the given disc specifier contain different
     * information.
     *
     * @param other the disc specifier to compare with this.
     * @return \c true if and only if this and the given disc specifier
     * contain different information.
     */
    bool operator != (const DiscSpec& other) const;

    friend std::ostream& operator << (std::ostream& out, const DiscSpec& spec);
};

/**
 * Writes the given disc specifier to the given output stream.
 * The disc specifier will be written as a triple
 * <tt>(tetIndex, type, number)</tt>.
 *
 * @param out the output stream to which to write.
 * @param spec the disc specifier to write.
 * @return a reference to \a out.
 *
 * \ingroup surfaces
 */
std::ostream& operator << (std::ostream& out, const DiscSpec& spec);

/**
 * Determines whether or not normal discs of the given type are
 * numbered away from the given vertex.
 *
 * @param discType the normal disc type under consideration; this
 * should be between 0 and 9 inclusive, as described by the DiscSpec
 * class notes.
 * @param vertex the vertex under consideration; this should be
 * between 0 and 3 inclusive.
 * @return \c true if normal discs of the given type are
 * numbered away from the given vertex, or \c false if they are
 * numbered towards the given vertex.
 *
 * \ingroup surfaces
 */
bool numberDiscsAwayFromVertex(int discType, int vertex);

/**
 * Determines whether or not the natural boundary orientation of a normal
 * disc of the given type follows the given directed normal arc.
 * Natural boundary orientation is defined by arrays regina::triDiscArcs,
 * regina::quadDiscArcs and regina::octDiscArcs.
 *
 * \pre The given normal arc lies on a normal disc of the given type.
 *
 * @param discType the normal disc type under consideration; this should
 * be between 0 and 9 inclusive, as described by the DiscSpec class
 * notes.
 * @param vertex the vertex about which the normal arc runs.
 * @param edgeStart the start vertex of the edge to which the normal arc
 * is parallel.
 * @param edgeEnd the end vertex of the edge to which the normal arc is
 * parallel.
 *
 * \ingroup surfaces
 */
bool discOrientationFollowsEdge(int discType, int vertex,
        int edgeStart, int edgeEnd);

/**
 * Represents a set of normal discs inside a single tetrahedron.
 * The numbers of discs of each type are stored in this structure, so
 * querying them is fast regardless of the underlying normal surface
 * coordinate system used.
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \warning This class converts the number of normal discs of a
 * given type from LargeInteger to <tt>unsigned long</tt>.  See the
 * precondition below.
 *
 * \pre The number of normal discs of a particular type
 * in a particular tetrahedron can be represented by a long integer.
 * \pre This class should only be used with \a embedded normal surfaces.
 *
 * \todo \problong Have some error flag so we can barf politely if the number
 * of normal discs of a given type does not fit into an <tt>unsigned
 * long</tt>.  See how this affects DiscSetTetData also.
 *
 * \ingroup surfaces
 */
class DiscSetTet {
    public:
        /**
         * A null type alias, for consistency with DiscSetTetData.
         *
         * This type alias is purely to help avoid compile errors in templated
         * code.  Since the DiscSetTet class does not actually hold any data
         * for each normal disc, this type alias should normally not be used.
         */
        using Data = std::nullptr_t;

    protected:
        unsigned long discs_[10];
            /**< The number of discs of each type. */

    public:
        /**
         * Creates a new set of normal discs corresponding to the discs
         * of the given normal surface that lie within the given
         * tetrahedron.
         *
         * @param surface the normal surface whose discs we shall use.
         * @param tetIndex the index in the triangulation of the
         * tetrahedron that our discs must lie in; this must be between
         * 0 and <tt>tri.size()-1</tt> inclusive, where
         * <tt>tri</tt> is the triangulation containing the given normal
         * surface.
         */
        DiscSetTet(const NormalSurface& surface, size_t tetIndex);
        /**
         * Creates a new set of normal discs where the number of discs of
         * each type is explicitly given.
         *
         * @param tri0 the number of triangular discs surrounding vertex 0.
         * @param tri1 the number of triangular discs surrounding vertex 1.
         * @param tri2 the number of triangular discs surrounding vertex 2.
         * @param tri3 the number of triangular discs surrounding vertex 3.
         * @param quad0 the number of quadrilateral discs of type 0.
         * @param quad1 the number of quadrilateral discs of type 1.
         * @param quad2 the number of quadrilateral discs of type 2.
         * @param oct0 the number of octahedral discs of type 0.
         * @param oct1 the number of octahedral discs of type 1.
         * @param oct2 the number of octahedral discs of type 2.
         */
        DiscSetTet(unsigned long tri0, unsigned long tri1,
            unsigned long tri2, unsigned long tri3,
            unsigned long quad0, unsigned long quad1, unsigned long quad2,
            unsigned long oct0 = 0, unsigned long oct1 = 0,
            unsigned long oct2 = 0);

        /**
         * Creates a new copy of the given set of normal discs.
         *
         * @param src the disc set to copy.
         */
        DiscSetTet(const DiscSetTet& src) = default;

        /**
         * Sets this to be a copy of the given set of normal discs.
         *
         * @param src the disc set to copy.
         * @return a reference to this disc set.
         */
        DiscSetTet& operator = (const DiscSetTet& src) = default;

        /**
         * Determines the number of discs of the given type inside this
         * tetrahedron.
         *
         * @param type the disc type to examine; this should be between
         * 0 and 9 inclusive.  Disc types are outlined in
         * the DiscSpec class notes.
         * @return the number of discs of the given type inside this
         * tetrahedron.
         */
        unsigned long nDiscs(int type) const;

        /**
         * Determines which normal arc of a given type on a given face
         * of this tetrahedron corresponds to the given normal disc.
         *
         * \pre The given normal disc actually meets a
         * normal arc of the given type on the given face.
         *
         * @param arcFace the face of this tetrahedron containing the
         * normal arc (between 0 and 3 inclusive).
         * @param arcVertex the vertex of this tetrahedron about which the
         * normal arc runs (between 0 and 3 inclusive); \a arcFace and
         * \a arcVertex should not be the same.
         * @param discType the disc type of the given normal disc;
         * this should be between 0 and 9 inclusive, as described in the
         * DiscSpec class notes.
         * @param discNumber indicates which normal disc of the given disc
         * type is referred to (between 0 and <tt>nDiscs(discType)-1</tt>
         * inclusive).
         * @return the number of the normal arc of the given type that belongs
         * to the given normal disc.
         * Arcs of a given type (where \a arcFace and \a arcVertex
         * together define the arc type) are numbered starting at 0 from the
         * tetrahedron vertex outwards.
         */
        unsigned long arcFromDisc(int arcFace, int arcVertex,
            int discType, unsigned long discNumber) const;
        /**
         * Determines which normal disc in this tetrahedron meets 
         * the given normal arc on the given face.
         *
         * \pre The given normal arc
         * actually exists in the normal surface with which this
         * \a DiscSetTet object was created.
         *
         * @param arcFace the face of this tetrahedron containing the
         * normal arc (between 0 and 3 inclusive).
         * @param arcVertex the vertex of this tetrahedron about which the
         * normal arc runs (between 0 and 3 inclusive); \a arcFace and
         * \a arcVertex should not be the same.
         * @param arcNumber indicates which normal arc of the given type
         * is referred to.
         * Arcs of a given type (where \a arcFace and \a arcVertex
         * together define the arc type) are numbered starting at 0 from the
         * tetrahedron vertex outwards.
         * @return a pair (\a discType, \a discNumber), where \a discType gives
         * the disc type of the normal disc that meets the given normal arc
         * (between 0 and 9 inclusive), and \a discNumber indicates which
         * normal disc of the returned disc type (<tt>discType</tt>)
         * meets the given normal arc (between 0 and <tt>nDiscs(discType)-1</tt>
         * inclusive).
         */
        std::pair<int, unsigned long> discFromArc(int arcFace, int arcVertex,
            unsigned long arcNumber) const;
};

/**
 * Stores data of type \c T for every normal disc inside a single
 * tetrahedron.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \warning This class converts the number of normal discs of a
 * given type from LargeInteger to <tt>unsigned long</tt>.  See the
 * precondition below.
 *
 * \pre The number of normal discs of a particular type
 * in a particular tetrahedron can be represented by a long integer.
 * \pre This class should only be used with \a embedded
 * normal surfaces.
 * \pre Type T has a default constructor and an
 * assignment operator.  That is, if \c a and \c b are of type T, then
 * \c a can be declared with no parameters and can then receive the
 * value of \c b using <tt>a=b</tt>.
 *
 * \ifacespython Not present.
 *
 * \ingroup surfaces
 */
template <class T>
class DiscSetTetData : public DiscSetTet {
    public:
        /**
         * The type of data being stored alongside each normal disc.
         */
        using Data = T;
        /**
         * A pointer to the type of data stored alongside each normal disc.
         *
         * \deprecated Simply use <tt>Data*</tt> instead.
         */
        using DataPtr [[deprecated]] = T*;

    protected:
        T* data_[10];
            /**< Stores the data corresponding to each normal disc. */

    public:
        /**
         * Creates a new disc set corresponding to the discs of the given
         * normal surface that lie within the given tetrahedron.  The data for
         * each disc will be initialised using its default constructor.
         *
         * @param surface the normal surface whose discs we shall use.
         * @param tetIndex the index in the triangulation of the
         * tetrahedron that our discs must lie in; this must be between
         * 0 and <tt>tri.size()-1</tt> inclusive, where <tt>tri</tt> is the
         * triangulation containing the given normal surface.
         */
        DiscSetTetData(const NormalSurface& surface,
                size_t tetIndex) : DiscSetTet(surface, tetIndex) {
            for (int i=0; i<10; i++)
                if (discs_[i])
                    data_[i] = new T[discs_[i]];
                else
                    data_[i] = nullptr;
        }
        /**
         * Creates a new disc set corresponding to the discs of the
         * given normal surface that lie within the given tetrahedron.
         * The data for each disc will be initialised to the given value.
         *
         * @param surface the normal surface whose discs we shall use.
         * @param tetIndex the index in the triangulation of the
         * tetrahedron that our discs must lie in; this must be between
         * 0 and <tt>tri.size()-1</tt> inclusive, where
         * <tt>tri</tt> is the triangulation containing the given normal
         * surface.
         * @param initValue the value with which to initialise the data
         * corresponding to each disc.
         */
        DiscSetTetData(const NormalSurface& surface,
                size_t tetIndex, const T& initValue) :
                DiscSetTet(surface, tetIndex) {
            unsigned long disc;
            for (int i=0; i<10; i++)
                if (discs_[i]) {
                    data_[i] = new T[discs_[i]];
                    std::fill(data_[i], data_[i] + discs_[i], initValue);
                } else
                    data_[i] = nullptr;
        }
        /**
         * Creates a new disc set where the number of discs of each type
         * is explicitly given.  The data for each disc will be initialised
         * using its default constructor.
         *
         * @param tri0 the number of triangular discs surrounding vertex 0.
         * @param tri1 the number of triangular discs surrounding vertex 1.
         * @param tri2 the number of triangular discs surrounding vertex 2.
         * @param tri3 the number of triangular discs surrounding vertex 3.
         * @param quad0 the number of quadrilateral discs of type 0.
         * @param quad1 the number of quadrilateral discs of type 1.
         * @param quad2 the number of quadrilateral discs of type 2.
         * @param oct0 the number of octahedral discs of type 0.
         * @param oct1 the number of octahedral discs of type 1.
         * @param oct2 the number of octahedral discs of type 2.
         */
        DiscSetTetData(unsigned long tri0, unsigned long tri1,
                unsigned long tri2, unsigned long tri3,
                unsigned long quad0, unsigned long quad1, unsigned long quad2,
                unsigned long oct0 = 0, unsigned long oct1 = 0,
                unsigned long oct2 = 0) :
                DiscSetTet(tri0, tri1, tri2, tri3, quad0, quad1, quad2,
                    oct0, oct1, oct2) {
            for (int i=0; i<10; i++)
                if (discs_[i])
                    data_[i] = new T[discs_[i]];
                else
                    data_[i] = nullptr;
        }

        /**
         * Creates a new copy of the given set of normal discs.
         *
         * The data for each disc will be copied using the copy
         * assignment operator for type \a T.
         *
         * @param src the disc set to copy.
         */
        DiscSetTetData(const DiscSetTetData& src) : DiscSetTet(src) {
            for (int i = 0; i < 10; ++i) {
                if (discs_[i]) {
                    data_[i] = new T[discs_[i]];
                    std::copy(src.data_[i], src.data_[i] + discs_[i], data_[i]);
                } else
                    data_[i] = nullptr;
            }
        }

        /**
         * Moves the contents of the given disc set into this new disc set.
         * This is a fast (constant time) operation.
         *
         * The disc set that was passed (\a src) will no longer be usable.
         *
         * @param src the disc set to move from.
         */
        DiscSetTetData(DiscSetTetData&& src) noexcept :
                DiscSetTet(src) /* parent copy constructor */ {
            std::copy(src.data_, src.data_ + 10, data_);
            std::fill(src.data_, src.data_ + 10, nullptr);
        }

        /**
         * Destroys this disc set and deallocates all data arrays.
         * Note that no assumption is made about type \c T, so if data
         * elements are pointers to dynamically allocated objects, these
         * will not be destroyed.
         */
        ~DiscSetTetData() {
            for (T* t : data_)
                delete[] t;
        }

        /**
         * Sets this to be a copy of the given set of normal discs.
         *
         * The data for each disc will be copied using the copy
         * assignment operator for type \a T.
         *
         * @param src the disc set to copy.
         * @return a reference to this disc set.
         */
        DiscSetTetData& operator = (const DiscSetTetData& src) {
            // std::copy() exhibits undefined behaviour with self-assignment.
            if (std::addressof(src) == this)
                return *this;

            // Resize the data arrays:
            for (int i = 0; i < 10; ++i) {
                if (discs_[i] != src.discs_[i]) {
                    delete[] data_[i];
                    if (src.discs_[i])
                        data_[i] = new T[src.discs_[i]];
                    else
                        data_[i] = nullptr;
                }
            }

            // Copy across the disc counts:
            DiscSetTet::operator = (src);

            // Refill the data arrays:
            for (int i = 0; i < 10; ++i)
                if (discs_[i])
                    std::copy(src.data_[i], src.data_[i] + discs_[i], data_[i]);

            return *this;
        }

        /**
         * Moves the contents of the given disc set into this disc set.
         * This is a fast (constant time) operation.
         *
         * The disc set that was passed (\a src) will no longer be usable.
         *
         * @param src the disc set to move from.
         */
        DiscSetTetData& operator = (DiscSetTetData&& src) noexcept {
            // Swap the data arrays, and leave the originals for src to destroy:
            std::swap_ranges(data_, data_ + 10, src.data_);

            // Copy across the disc counts:
            DiscSetTet::operator = (src);

            return *this;
        }

        /**
         * Swaps the contents of this and the given disc set.
         *
         * @param other the disc set whose contents should be swapped with this.
         */
        void swap(DiscSetTetData& other) noexcept {
            std::swap_ranges(data_, data_ + 10, other.data_);
        }

        /**
         * Retrieves a reference to the data corresponding to the given
         * normal disc.
         *
         * @param discType the disc type of the given normal disc;
         * this should be between 0 and 9 inclusive, as described in the
         * DiscSpec class notes.
         * @param discNumber indicates which normal disc of the given disc
         * type is referred to; this should be between 0 and
         * <tt>nDiscs(discType)-1</tt> inclusive.
         * @return a reference to the data corresponding to the given
         * normal disc.
         */
        T& data(int discType, unsigned long discNumber) {
            // assert(0 <= discType && discType < 10);
            // assert(discNumber < discs_[discType]);
            return data_[discType][discNumber];
        }
};

/**
 * Swaps the contents of the two given disc sets.
 *
 * This global routine simply calls DiscSetTetData::swap(); it is provided
 * so that DiscSetTetData meets the C++ Swappable requirements.
 *
 * @param a the first disc set whose contents should be swapped.
 * @param b the second disc set whose contents should be swapped.
 *
 * \ingroup surfaces
 */
template <class T>
void swap(DiscSetTetData<T>& a, DiscSetTetData<T>& b) noexcept {
    a.swap(b);
}

/**
 * Stores a piece of data alongside every normal disc within a particular
 * normal surface.
 *
 * End users should not refer to this class directly; instead use one of
 * the type aliases DiscSetSurfaceData<T> or DiscSetSurface.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \tparam TetData This must be either (1) DiscSetTet, in which case
 * there will be no additional data stored for each normal disc; or
 * (2) a class of the form DiscSetTetData<T>, in which case there will
 * be data of type \a T stored alongside each normal disc.
 *
 * \warning This class converts the number of normal discs of a
 * given type from LargeInteger to <tt>unsigned long</tt>.  See the
 * precondition below.
 *
 * \pre The number of normal discs of a particular type
 * in a particular tetrahedron can be represented by a long integer.
 * \pre This class should only be used with \a embedded normal surfaces.
 *
 * \ifacespython The only instance of this class that is available
 * through python is DiscSetSurface (i.e., the "vanilla" case where
 * \a TetData is the type DiscSetTet).
 *
 * \ingroup surfaces
 */
template <class TetData>
class DiscSetSurfaceDataImpl {
    private:
        TetData** discSets;
            /**< The disc sets and associated data for each tetrahedron. */
        const Triangulation<3>* triangulation;
            /**< The triangulation in which the normal surface lives.
                 This is kept as a pointer to support assignment; it
                 must never be null. */

    public:
        /**
         * Creates a new disc set corresponding to the discs of the
         * given normal surface.
         * The data for each disc will be initialised using its default
         * constructor.
         *
         * @param surface the normal surface whose discs we shall use.
         */
        DiscSetSurfaceDataImpl(const NormalSurface& surface) :
                triangulation(&surface.triangulation()) {
            size_t tot = triangulation->size();
            if (tot) {
                discSets = new TetData*[tot];
                for (size_t index = 0; index < tot; index++)
                    discSets[index] = new TetData(surface, index);
            } else
                discSets = nullptr;
        }
        /**
         * Creates a new disc set corresponding to the discs of the
         * given normal surface.
         * The data for each disc will be initialised to the given value.
         *
         * \pre The template argument TetData is a class of the form
         * DiscSetTetData<T>, not DiscSetTet.
         *
         * @param surface the normal surface whose discs we shall use.
         * @param initValue the value with which to initialise the data
         * corresponding to each disc.
         */
        DiscSetSurfaceDataImpl(const NormalSurface& surface,
                const typename TetData::Data& initValue) :
                triangulation(&surface.triangulation()) {
            size_t tot = triangulation->size();
            if (tot) {
                discSets = new TetData*[tot];
                for (size_t index = 0; index < tot; index++)
                    discSets[index] = new TetData(surface, index, initValue);
            } else
                discSets = nullptr;
        }

        /**
         * Creates a new copy of the given set of normal discs.
         *
         * The data for each disc will be copied using the copy
         * assignment operator for type \a T.
         *
         * @param src the disc set to copy.
         */
        DiscSetSurfaceDataImpl(const DiscSetSurfaceDataImpl& src) :
                triangulation(src.triangulation) {
            size_t tot = triangulation->size();
            if (tot) {
                discSets = new TetData*[tot];
                for (size_t index = 0; index < tot; index++)
                    discSets[index] = new TetData(*src.discSets[index]);
            } else
                discSets = nullptr;
        }

        /**
         * Moves the contents of the given disc set into this new disc set.
         * This is a fast (constant time) operation.
         *
         * The disc set that was passed (\a src) will no longer be usable.
         *
         * @param src the disc set to move from.
         */
        DiscSetSurfaceDataImpl(DiscSetSurfaceDataImpl&& src) noexcept :
                discSets(src.discSets), triangulation(src.triangulation) {
            src.discSets = nullptr;
        }

        /**
         * Destroys this set of discs and deallocates all associated memory.
         */
        ~DiscSetSurfaceDataImpl() {
            if (discSets) {
                size_t tot = nTets();
                for (size_t index = 0; index < tot; index++)
                    delete discSets[index];
                delete[] discSets;
            }
        }

        /**
         * Sets this to be a copy of the given disc set.
         *
         * The data for each disc will be copied using the copy
         * assignment operator for type \a T.
         *
         * @param src the disc set to copy.
         * @return a reference to this disc set.
         */
        DiscSetSurfaceDataImpl& operator = (const DiscSetSurfaceDataImpl& src) {
            // The code below will break badly with self-assignment.
            if (std::addressof(src) == this)
                return *this;

            // Destroy the original data.
            // Note: we could optimise this if it is the same triangulation
            // (not deleting and reallocating the arrays).
            // However, we will leave this until if/when it actually matters.
            if (discSets) {
                size_t tot = nTets();
                for (size_t index = 0; index < tot; index++)
                    delete discSets[index];
                delete[] discSets;
            }

            triangulation = src.triangulation;

            size_t tot = triangulation->size();
            if (tot) {
                discSets = new TetData*[tot];
                for (size_t index = 0; index < tot; index++)
                    discSets[index] = new TetData(*src.discSets[index]);
            } else
                discSets = nullptr;

            return *this;
        }

        /**
         * Moves the contents of the given disc set into this disc set.
         *
         * The running time of this operation is linear in the number of
         * tetrahedra from the \e original triangulation (since the
         * underlying data for those tetrahedra will be destroyed immediately).
         *
         * The disc set that was passed (\a src) will no longer be usable.
         *
         * @param src the disc set to move from.
         */
        DiscSetSurfaceDataImpl& operator = (DiscSetSurfaceDataImpl&& src)
                noexcept {
            // We destroy the original data now, since otherwise this
            // would require us to impose a constraint on the continued
            // lifespan of the original triangulation (which we need
            // during the destruction process).
            if (discSets) {
                size_t tot = nTets();
                for (size_t index = 0; index < tot; index++)
                    delete discSets[index];
                delete[] discSets;
            }

            discSets = src.discSets;
            triangulation = src.triangulation;

            src.discSets = nullptr;
            return *this;
        }

        /**
         * Swaps the contents of this and the given disc set.
         *
         * @param other the disc set whose contents should be swapped with this.
         */
        void swap(DiscSetSurfaceDataImpl& other) noexcept {
            std::swap(discSets, other.discSets);
            std::swap(triangulation, other.triangulation);
        }

        /**
         * Returns the number of tetrahedra in the underlying triangulation.
         *
         * @return the number of tetrahedra.
         */
        size_t nTets() const {
            return triangulation->size();
        }

        /**
         * Determines the number of discs of the given type inside the
         * given tetrahedron.
         *
         * @param tetIndex the index in the triangulation of the
         * tetrahedron to examine.
         * @param type the disc type to examine; this should be between
         * 0 and 9 inclusive.  Disc types are outlined in
         * the DiscSpec class notes.
         * @return the number of discs of the given type inside the
         * given tetrahedron.
         */
        unsigned long nDiscs(size_t tetIndex, int type) const {
            return discSets[tetIndex]->nDiscs(type);
        }

        /**
         * Returns the specific set of discs living inside the given
         * tetrahedron.
         *
         * @param tetIndex the index in the triangulation of the given
         * tetrahedron.
         * @return the set of discs inside the given tetrahedron.
         */
        TetData& tetDiscs(size_t tetIndex) const {
            return *(discSets[tetIndex]);
        }

        /**
         * Retrieves a reference to the data corresponding to the given
         * normal disc.
         *
         * \pre The template argument TetData is a class of the form
         * DiscSetTetData<T>, not DiscSetTet.
         *
         * @param disc the disc whose data we require; this must refer
         * to a disc within this disc set.
         * @return a reference to the data corresponding to the given
         * normal disc.
         */
        typename TetData::Data& data(const DiscSpec& disc) {
            return discSets[disc.tetIndex]->data(disc.type, disc.number);
        }

        /**
         * Determines which normal disc is adjacent to the given normal disc
         * along the given directed normal arc in the surface described by
         * this disc set.
         *
         * A directed normal arc will be specified by a permutation
         * <i>p</i>, where the arc runs around vertex <tt>p[0]</tt>
         * parallel to the directed edge from vertex <tt>p[1]</tt> to
         * <tt>p[2]</tt>.
         *
         * @param disc the given normal disc; this must be a disc in this
         * disc set.
         * @param arc the given normal arc; this must actually be an arc
         * on the boundary of the given normal disc (although it may run
         * in either direction).
         * @return a pair (\a adj, \a perm), where \a adj is the normal disc
         * adjacent to the given disc along the given arc, and \a perm represents
         * the same directed normal arc that was passed but expressed in terms
         * of the vertices of the adjacent tetrahedron.  This will be no value
         * if there is no adjacent disc.
         */
        std::optional<std::pair<DiscSpec, Perm<4>>> adjacentDisc(
                const DiscSpec& disc, Perm<4> arc) const {
            const Tetrahedron<3>* tet = triangulation->tetrahedron(
                disc.tetIndex);
            int arcFace = arc[3];
            if (tet->adjacentTetrahedron(arcFace) == nullptr)
                return std::nullopt;

            DiscSpec ans;
            ans.tetIndex = tet->adjacentTetrahedron(arcFace)->index();
            Perm<4> adjArc = tet->adjacentGluing(arcFace) * arc;

            unsigned long arcNumber = discSets[disc.tetIndex]->arcFromDisc(
                arcFace, arc[0], disc.type, disc.number);
            std::tie(ans.type, ans.number) = discSets[ans.tetIndex]->
                discFromArc(adjArc[3], adjArc[0], arcNumber);

            return std::make_pair(ans, adjArc);
        }

        /**
         * Returns an iterator at the beginning of the range of all
         * normal discs in the underlying normal surface.
         *
         * These begin() and end() routines allow you to iterate through
         * all normal discs using C++11 range-based \c for loops:
         *
         * \code{.cpp}
         * for (const DiscSpec& s : surfaceData) { ... }
         * \endcode
         *
         * In Python, an object of this class can be treated as an iterable
         * object, again iterating through all normal discs:
         *
         * \code{.py}
         * for s in surfaceData:
         *     ...
         * \endcode
         *
         * @return an iterator at the beginning of the range of all
         * normal discs.
         */
        DiscSpecIterator<TetData> begin() const {
            return DiscSpecIterator<TetData>(*this);
        }
        /**
         * Returns an iterator at the end of the range of all
         * normal discs in the underlying normal surface.
         *
         * In C++, the begin() and end() routines allow you to iterate through
         * all normal discs using C++11 range-based \c for loops.  In Python,
         * an object of this class can be treated as an iterable object.
         *
         * See the begin() documentation for further details.
         *
         * @return an iterator at the end of the range of all normal discs.
         */
        DiscSpecIterator<TetData> end() const {
            DiscSpecIterator<TetData> ans(*this);
            ans.current.tetIndex = triangulation->size();
            ans.current.type = 0;
            ans.current.number = 0;
            return ans;
        }
};

/**
 * Swaps the contents of the two given disc sets.
 *
 * This global routine simply calls DiscSetSurfaceDataImpl::swap(); it is
 * provided so that DiscSetSurfaceDataImpl meets the C++ Swappable requirements.
 *
 * @param a the first disc set whose contents should be swapped.
 * @param b the second disc set whose contents should be swapped.
 *
 * \ingroup surfaces
 */
template <class T>
void swap(DiscSetSurfaceDataImpl<T>& a, DiscSetSurfaceDataImpl<T>& b) noexcept {
    a.swap(b);
}

/**
 * A structure that stores data of type \a T alongside every normal disc
 * within a particular normal surface.
 *
 * \ifacespython Not present.
 *
 * \ingroup surfaces
 */
template <typename T>
using DiscSetSurfaceData = DiscSetSurfaceDataImpl<DiscSetTetData<T>>;

/**
 * A structure that builds all of the normal discs within a particular
 * normal surface, but does not store any additional data alongside them.
 *
 * This structure can be used for iterating through disc types, and for
 * moving between adjacent disc types within a surface.
 *
 * \ingroup surfaces
 */
using DiscSetSurface = DiscSetSurfaceDataImpl<DiscSetTet>;

/**
 * A forward iterator used for running through all normal discs in a normal
 * surface.
 *
 * This class is designed to work alongside DiscSetSurfaceDataImpl<TetData>.
 * Typically you would not need to specify the template argument \a TetData;
 * for copy initialisation you can use the \c auto type, and when constructing
 * from a DiscSetSurfaceData<T> the template argument can be automatically
 * deduced.
 *
 * \warning This class converts the indices of normal discs of a
 * given type from LargeInteger to <tt>unsigned long</tt>.  See the
 * precondition below.
 *
 * \pre The number of normal discs of a particular type
 * in a particular tetrahedron can be represented by a long integer.
 *
 * \ifacespython The only instance of this class that is available
 * through python is the iterator for DiscSetSurface (i.e., the "vanilla"
 * case where \a TetData is the type DiscSetTet).  Moreover, instead of the
 * C++ interface described here, in Python DiscSetSurface and this class
 * together implement the Python iterable/iterator interface.
 * The DiscSetSurface class implements <tt>__iter__()</tt>, which returns a
 * DiscSpecIterator; then DiscSpecIterator implements <tt>next()</tt>, which
 * either returns the next normal disc in the set or else throws a
 * <tt>StopException</tt> if there are no more discs to return.
 *
 * \ingroup surfaces
 */
template <class TetData>
class DiscSpecIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
            /**< Declares this to be a forward iterator type. */
        using value_type = regina::DiscSpec;
            /**< Indicates what type the iterator points to. */
        using difference_type = long;
            /**< The type obtained by subtracting iterators. */
        using pointer = const regina::DiscSpec*;
            /**< A pointer to \a value_type. */
        using reference = const regina::DiscSpec&;
            /**< The type obtained when dereferencing iterators. */

    private:
        const DiscSetSurfaceDataImpl<TetData>* internalDiscSet;
            /**< The disc set through which we are iterating. */
        DiscSpec current;
            /**< The disc currently pointed to. */

    public:
        /**
         * Creates a new uninitialised iterator.
         * This iterator cannot be used or queried until either init() or the
         * assignmemnt operator is called.
         */
        DiscSpecIterator() : internalDiscSet(nullptr) {
        }
        /**
         * Creates a new iterator pointing to the first disc in the
         * given disc set.
         *
         * @param discSet the disc set used to initialise this iterator.
         */
        DiscSpecIterator(const DiscSetSurfaceDataImpl<TetData>& discSet) :
                internalDiscSet(&discSet), current(0, 0, 0) {
            makeValid();
        }
        /**
         * Default copy constructor.
         */
        DiscSpecIterator(const DiscSpecIterator&) = default;
        /**
         * Points this iterator to the first disc in the given disc set.
         *
         * @param discSet the disc set used to reinitialise this iterator.
         */
        void init(const DiscSetSurfaceDataImpl<TetData>& discSet) {
            internalDiscSet = &discSet;
            current.tetIndex = 0;
            current.type = 0;
            current.number = 0;
            makeValid();
        }

        /**
         * Default copy assignment operator.
         */
        DiscSpecIterator& operator = (const DiscSpecIterator&) = default;

        /**
         * Preincrement operator.
         *
         * \pre This iterator is not past-the-end.
         *
         * \ifacespython Not present; instead this class implements
         * <tt>next()</tt>, which either returns the current packet in the
         * subtree and increments the iterator, or else throws a
         * <tt>StopIteration</tt> exception if the iterator is past-the-end.
         *
         * @return a reference to this iterator.
         */
        DiscSpecIterator& operator++() {
            ++current.number;
            makeValid();
            return *this;
        }
        /**
         * Postincrement operator.
         *
         * \pre This iterator is not past-the-end.
         *
         * \ifacespython Not present; instead this class implements
         * <tt>next()</tt>, which either returns the current packet in the
         * subtree and increments the iterator, or else throws a
         * <tt>StopIteration</tt> exception if the iterator is past-the-end.
         *
         * @return a copy of this iterator before it was incremented.
         */
        DiscSpecIterator operator++(int) {
            DiscSpecIterator ans = *this;
            ++current.number;
            makeValid();
            return ans;
        }
        /**
         * Returns a reference to the disc pointed to by this iterator.
         *
         * \pre This iterator is not past-the-end.
         *
         * \ifacespython This routine is called deref(), since Python does
         * not support the dereference operator.
         *
         * @return a reference to the disc pointed to by this iterator.
         */
        const DiscSpec& operator *() const {
            return current;
        }
        /**
         * Determines if this iterator is past-the-end.
         *
         * @return \c true if and only if this iterator is past-the-end.
         */
        bool done() const {
            return (current.tetIndex == internalDiscSet->nTets());
        }
        /**
         * Determines if this and the given iterator are equal.
         *
         * Two iterators are considered equal if (i) they were constructed
         * from the same DiscSetSurface object (not two different
         * DiscSetSurface objects with identical contents), and (ii) they
         * point to the same disc of the same tetrahedron.
         *
         * @param other the iterator to compare with this.
         * @return \c true if and only if this and the given iterator
         * are equal.
         */
        bool operator == (const DiscSpecIterator& other) const {
            return internalDiscSet == other.internalDiscSet &&
                current == other.current;
        }
        /**
         * Determines if this and the given iterator are different.
         *
         * Two iterators are considered equal if (i) they were constructed
         * from the same DiscSetSurface object (not two different
         * DiscSetSurface objects with identical contents), and (ii) they
         * point to the same disc of the same tetrahedron.
         *
         * @param other the iterator to compare with this.
         * @return \c true if and only if this and the given iterator
         * are equal.
         */
        bool operator != (const DiscSpecIterator& other) const {
            return internalDiscSet != other.internalDiscSet ||
                current != other.current;
        }

    private:
        /**
         * Ensures the data member \a current points to a real disc and
         * not a virtual disc (in which the disc number exceeds the
         * number of discs of the corresponding type).
         *
         * \pre This iterator is not yet past-the-end
         * (although it may be in the middle of an increment operation
         * that will put it past-the-end).
         */
        void makeValid() {
            if (current.tetIndex == internalDiscSet->nTets())
                return;
            while (current.number ==
                    internalDiscSet->nDiscs(current.tetIndex, current.type)) {
                current.number = 0;
                ++current.type;
                if (current.type == 10) {
                    current.type = 0;
                    ++current.tetIndex;
                    if (current.tetIndex == internalDiscSet->nTets())
                        break;
                }
            }
        }

    friend class DiscSetSurfaceDataImpl<TetData>;
};

// Inline functions for DiscSpec

inline DiscSpec::DiscSpec(size_t newTetIndex, int newType,
        unsigned long newNumber) : tetIndex(newTetIndex), type(newType),
        number(newNumber) {
}

inline bool DiscSpec::operator == (const DiscSpec& other) const {
    return (tetIndex == other.tetIndex && type == other.type &&
        number == other.number);
}
inline bool DiscSpec::operator != (const DiscSpec& other) const {
    return (tetIndex != other.tetIndex || type != other.type ||
        number != other.number);
}

// Inline functions for DiscSetTet

inline DiscSetTet::DiscSetTet(unsigned long tri0, unsigned long tri1,
        unsigned long tri2, unsigned long tri3,
        unsigned long quad0, unsigned long quad1, unsigned long quad2,
        unsigned long oct0, unsigned long oct1, unsigned long oct2) :
        discs_{ tri0, tri1, tri2, tri3, quad0, quad1, quad2, oct0, oct1, oct2 } {
}

inline unsigned long DiscSetTet::nDiscs(int type) const {
    return discs_[type];
}

} // namespace regina

#endif

