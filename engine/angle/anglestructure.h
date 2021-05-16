
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

/*! \file angle/anglestructure.h
 *  \brief Deals with angle structures on triangulations.
 */

#ifndef __ANGLESTRUCTURE_H
#ifndef __DOXYGEN
#define __ANGLESTRUCTURE_H
#endif

#include "regina-core.h"
#include "core/output.h"
#include "maths/rational.h"
#include "maths/vector.h"
#include "triangulation/forward.h"

namespace regina {

class XMLAngleStructureReader;

template <typename, bool> class Matrix;
typedef Matrix<Integer, true> MatrixInt;

/**
 * \addtogroup angle Angle Structures
 * Angle structures on triangulations.
 * @{
 */

/**
 * Deprecated typedef for the vector type that Regina uses internally to
 * store angle structures.
 *
 * Before Regina 6.1, this was a subclass of Vector<LargeInteger> that
 * merely added an extra static routine makeAngleEquations() that was
 * identical to the global regina::makeAngleEquations().
 *
 * As of Regina 6.1, angle structures use Integer (not LargeInteger)
 * internally, and so this type has had to change in an incompatible way.
 * Since it added nothing genuinely new, the subclass has been removed
 * entirely; instead angle structures just use Vector<Integer> directly.
 *
 * A side-effect is that the static makeAngleEquations() has been removed
 * completely.  However, as always, you can still access the angle equations
 * routine through the global regina::makeAngleEquations() instead.
 *
 * \deprecated Simply use Vector<Integer> (or the alias VectorInt) instead.
 */
typedef Vector<Integer> AngleStructureVector [[deprecated]];

/**
 * Represents an angle structure on a triangulation.
 * Once the underlying triangulation changes, this angle structure
 * is no longer valid.
 */
class REGINA_API AngleStructure : public ShortOutput<AngleStructure> {
    private:
        Vector<Integer>* vector_;
            /**< Stores (indirectly) the individual angles in this angle
             *   structure. */
        const Triangulation<3>* triangulation_;
            /**< The triangulation on which this angle structure is placed. */

        mutable unsigned long flags_;
            /**< Stores a variety of angle structure properties as
             *   described by the flag constants in this class.
             *   Flags can be combined using bitwise OR. */
        static constexpr unsigned long flagStrict = 1;
            /**< Signals that this angle structure is strict. */
        static constexpr unsigned long flagTaut = 2;
            /**< Signals that this angle structure is taut.  A taut
                 structure might also be veering, in which case the
                 flag \a flagVeering will be set also. */
        static constexpr unsigned long flagVeering = 8;
            /**< Signals that this angle structure is veering (in which
                 case that the \a flagTaut flag must be set also). */
        static constexpr unsigned long flagCalculatedType = 4;
            /**< Signals that the type (strict/taut/veering) has been
                 calculated. */

    public:
        /**
         * Creates a new copy of the given angle structure.
         *
         * @param other the angle structure to clone.
         */
        AngleStructure(const AngleStructure& other);

        /**
         * Moves the given angle structure into this new angle structure.
         * This is a fast (constant time) operation.
         *
         * The angle structure that is passed (\a src) will no longer be usable.
         *
         * @param src the angle structure to move.
         */
        AngleStructure(AngleStructure&& src) noexcept;

        /**
         * Creates a new angle structure on the given triangulation with
         * the given coordinate vector.
         *
         * This angle structure will claim ownership of the given vector,
         * and so you should not change or destroy the vector yourself
         * afterwards.
         *
         * \pre The given coordinate vector represents an angle structure on
         * the given triangulation, according to the integer vector
         * representation described in the notes for vector().
         * \pre The given vector is not a null pointer.
         *
         * \ifacespython Not present.
         *
         * @param triang the triangulation on which this angle structure lies.
         * @param newVector a vector containing the individual angles in the
         * angle structure.
         */
        AngleStructure(const Triangulation<3>* triang,
            Vector<Integer>* newVector);
        /**
         * Destroys this angle structure.
         * The underlying vector of angles will also be deallocated.
         */
        ~AngleStructure();

        /**
         * Deprecated routine that creates a newly allocated clone of this
         * angle structure.
         *
         * \deprecated Simply use the copy constructor instead.
         *
         * @return a clone of this angle structure.
         */
        [[deprecated]] AngleStructure* clone() const;

        /**
         * Returns the requested angle in this angle structure.
         * The angle returned will be scaled down; the actual angle is
         * the returned value multiplied by <i>pi</i>.
         *
         * Within a tetrahedron, the three angles are indexed 0, 1 and 2.
         * Angle \a i appears on edge \a i of the tetrahedron as well as
         * its opposite edge (5-\a i).
         *
         * @param tetIndex the index in the triangulation of the
         * tetrahedron in which the requested angle lives; this should
         * be between 0 and Triangulation<3>::size()-1
         * inclusive.
         * @param edgePair the number representing the pair of edges holding
         * the requested angle, as described above; this should be 0, 1 or 2.
         * @return the requested angle scaled down by <i>pi</i>.
         */
        Rational angle(size_t tetIndex, int edgePair) const;

        /**
         * Returns the triangulation on which this angle structure lies.
         *
         * @return the underlying triangulation.
         */
        const Triangulation<3>* triangulation() const;

        /**
         * Determines whether this is a strict angle structure.
         * A strict angle structure has all angles strictly between (not
         * including) 0 and <i>pi</i>.
         *
         * @return \c true if and only if this is a strict angle structure.
         */
        bool isStrict() const;

        /**
         * Determines whether this is a taut angle structure.
         * A taut angle structure contains only angles 0 and <i>pi</i>.
         *
         * Here we use the Kang-Rubinstein definition of a taut
         * angle structure [1], which is based on the angles alone.
         * In his original paper [2], Lackenby has an extra condition
         * whereby 2-faces of the triangulation must have consistent
         * coorientations, which we do not enforce here.
         *
         * [1] E. Kang and J. H. Rubinstein, "Ideal triangulations of
         * 3-manifolds II; Taut and angle structures", Algebr. Geom. Topol.
         * 5 (2005), pp. 1505-1533.
         *
         * [2] M. Lackenby, "Taut ideal triangulations of 3-manifolds",
         * Geom. Topol. 4 (2000), pp. 369-395.
         *
         * @return \c true if and only if this is a taut structure.
         */
        bool isTaut() const;

        /**
         * Determines whether this is a veering structure.
         * A veering structure is taut angle structure with additional
         * strong combinatorial constraints, which we do not outline here.
         * For a definition, see C. D. Hodgson, J. H. Rubinstein,
         * H. Segerman, and S. Tillmann, "Veering triangulations admit
         * strict angle structures", Geom. Topol., 15 (2011), pp. 2073-2089.
         *
         * Note that the Hodgson et al. definition is slightly more general
         * than Agol's veering taut triangulations from his original paper:
         * I. Agol, " Ideal triangulations of pseudo-Anosov mapping tori",
         * in "Topology and Geometry in Dimension Three", volume 560 of
         * Contemp. Math., pp. 1-17, Amer. Math. Soc., 2011.
         * This mirrors the way in which the Kang-Rubinstein definition of
         * taut angle structure is slightly more general than Lackenby's.
         * See the Hodgson et al. paper for full details and comparisons
         * between the two settings.
         *
         * If this angle structure is not taut, or if the underlying
         * triangulation is non-orientable, then this routine will
         * return \c false.
         *
         * @return \c true if and only if this is a veering structure.
         */
        bool isVeering() const;

        /**
         * Gives read-only access to the integer vector that Regina uses
         * internally to represent this angle structure.
         *
         * This vector contains one member per angle plus a final scaling
         * member; to obtain the actual angle in the angle structure one should
         * divide the corresonding angle member by the scaling member and then
         * multiply by <i>pi</i>.
         *
         * If there are \a t tetrahedra in the underlying triangulation, there
         * will be precisely 3<i>t</i>+1 elements in this vector.  The first
         * three elements will be the angle members for the first tetrahedron,
         * the next three for the second tetrahedron and so on.  For each
         * tetraheron, the three individual elements are the angle members
         * corresponding to edges 0, 1 and 2 of the tetrahedron (and also their
         * opposite edges 5, 4 and 3 respectively).  The final element of the
         * vector is the scaling member as described above.
         *
         * @return the underlying integer vector.
         */
        const Vector<Integer>& vector() const;

        /**
         * A deprecated alias for vector().
         *
         * \deprecated This routine has been renamed to vector().  Note that
         * the replacement routine vector() returns a reference, not a pointer.
         *
         * @return the underlying integer vector.
         */
        [[deprecated]] const Vector<Integer>* rawVector() const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        /**
         * Writes a chunk of XML containing this angle structure and all
         * of its properties.  This routine will be called from within
         * AngleStructures::writeXMLPacketData().
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which the XML should be written.
         */
        void writeXMLData(std::ostream& out) const;

        // Make this class non-assignable.
        AngleStructure& operator = (const AngleStructure&) = delete;
        AngleStructure& operator = (AngleStructure&&) = delete;

    protected:
        /**
         * Calculates the structure type (strict or taut) and stores it
         * as a property.
         */
        void calculateType() const;

    friend class regina::XMLAngleStructureReader;
};

/*@}*/

// Inline functions for AngleStructure

inline AngleStructure::AngleStructure(const Triangulation<3>* triang,
        Vector<Integer>* newVector) : vector_(newVector),
        triangulation_(triang), flags_(0) {
}

inline AngleStructure::AngleStructure(const AngleStructure& other) :
        vector_(new Vector<Integer>(*other.vector_)),
        triangulation_(other.triangulation_),
        flags_(other.flags_) {
}

inline AngleStructure::AngleStructure(AngleStructure&& src) noexcept :
        vector_(src.vector_),
        triangulation_(src.triangulation_),
        flags_(src.flags_) {
    src.vector_ = nullptr;
}

inline AngleStructure::~AngleStructure() {
    delete vector_;
}

inline AngleStructure* AngleStructure::clone() const {
    return new AngleStructure(*this);
}

inline const Triangulation<3>* AngleStructure::triangulation() const {
    return triangulation_;
}

inline bool AngleStructure::isStrict() const {
    if ((flags_ & flagCalculatedType) == 0)
        calculateType();
    return ((flags_ & flagStrict) != 0);
}

inline bool AngleStructure::isTaut() const {
    if ((flags_ & flagCalculatedType) == 0)
        calculateType();
    return ((flags_ & flagTaut) != 0);
}

inline bool AngleStructure::isVeering() const {
    if ((flags_ & flagCalculatedType) == 0)
        calculateType();
    return ((flags_ & flagVeering) != 0);
}

inline const Vector<Integer>& AngleStructure::vector() const {
    return *vector_;
}

inline const Vector<Integer>* AngleStructure::rawVector() const {
    return vector_;
}

} // namespace regina

#endif

