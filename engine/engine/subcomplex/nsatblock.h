
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

/*! \file nsatblock.h
 *  \brief Deals with saturated blocks in triangulations of Seifert fibred
 *  spaces.
 */

#ifndef __NSATBLOCK_H
#ifndef __DOXYGEN
#define __NSATBLOCK_H
#endif

#include "shareableobject.h"
#include "subcomplex/nsatannulus.h"
#include <list>

namespace regina {

class NIsomorphism;
class NSatAnnulus;
class NSFSpace;
class NTetrahedron;
class NTriangulation;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents a saturated block in a Seifert fibred space.  A saturated
 * block is a connected set of tetrahedra built from a subset of fibres
 * (no fibres may enter or exit the boundary of the block).  In addition,
 * the boundary of this block must be a ring of saturated annuli, as
 * described by the NSatAnnulus class.  Aside from this ring of saturated
 * annuli, there may be no other boundary faces within the block.
 *
 * The boundary annuli are numbered consecutively as illustrated below,
 * where the markings 0 and 1 within the triangles represent the first
 * and second face of each annulus (see the NSatAnnulus class notes for
 * details).  Note that the following diagram is viewed from \e inside
 * the block.
 *
 * <pre>
 *               -+---+---+---+---+---+---+-
 *                |0 /|0 /|0 /|0 /|0 /|0 /|
 *            ... | / | / | / | / | / | / | ...
 *                |/ 1|/ 1|/ 1|/ 1|/ 1|/ 1|
 *               -+---+---+---+---+---+---+-
 * Annulus #  ...  n-2 n-1  0   1   2   3   ...
 * </pre>
 *
 * The ring of boundary annuli may optionally be twisted, so that together
 * the annuli form a long Mobius band.  In this case, for the purposes of
 * labelling and marking annuli, the twist occurs between annuli n-1 and 0.
 * Be careful when dealing with blocks with twisted boundaries, since
 * with twists it is possible to identify an edge with itself in reverse
 * (thus producing something that is not a 3-manifold triangulation).
 *
 * Each saturated block corresponds to a piece of the base orbifold of the
 * larger Seifert fibred space.  For the purpose of connecting the base
 * orbifold together, we assume that the boundary of this particular
 * piece runs horizontally in the diagram above (specifically following
 * the horizontal edges of the boundary annuli, as described in the
 * NSatAnnulus class notes).  Insisting on such a boundary may lead to
 * (1,\a k) twists within the block; these are accounted for by the
 * virtual adjustSFS() routine.
 *
 * Saturated blocks are generally joined to one another (or themselves)
 * along their boundary annuli.  For this purpose, each saturated block
 * contains a list of which annulus of this block is adjacent to which
 * annulus of which other block.  Adjacencies may be \e reflected, meaning
 * that the adjacent annulus has its fibres reversed (i.e., the adjacent
 * annulus has undergone an up-to-down reflection); they may also be
 * \e backwards, meaning that the first face of one annulus is joined to
 * the second face of the other (and vice versa).
 *
 * \warning In addition to mandatory overrides such as clone() and
 * adjustSFS(), some subclasses will need to override the virtual
 * routine transform() in order to correctly adjust additional
 * triangulation-specific information stored in the subclass.  See the
 * transform() documentation for further details.
 */
class NSatBlock : public ShareableObject {
    public:
        typedef std::list<NTetrahedron*> TetList;
            /**< The data structure used to store a list of tetrahedra
                 that should not be examined by isBlock(). */

    protected:
        unsigned nAnnuli_;
            /**< The number of boundary annuli. */
        NSatAnnulus* annulus_;
            /**< Details of each boundary annulus, as seen from the
                 inside of this saturated block. */
        bool twistedBoundary_;
            /**< Is the ring of boundary annuli twisted to form a Mobius
                 band? */

        NSatBlock** adjBlock_;
            /**< The saturated block joined to each boundary annulus;
                 this may be null if there is no adjacency or if this
                 information is not known. */
        unsigned* adjAnnulus_;
            /**< Describes which specific annulus of the adjacent
                 saturated block is joined to each boundary annulus of this
                 block.  Values may be undefined if the corresponding
                 entries in the \a adjBlock array is null. */
        bool* adjReflected_;
            /**< Describes whether the adjacency for each boundary
                 annulus is reflected (see the class notes above).
                 Values may be undefined if the corresponding
                 entries in the \a adjBlock array is null. */
        bool* adjBackwards_;
            /**< Describes whether the adjacency for each boundary
                 annulus is backwards (see the class notes above).
                 Values may be undefined if the corresponding
                 entries in the \a adjBlock array is null. */

    public:
        /**
         * Creates a new clone of the given block.
         *
         * Note that the new \a adjBlock_ array will contain pointers to
         * the same adjacent blocks as the original.  That is, adjacent
         * blocks will not be cloned also; instead pointers to adjacent
         * blocks will simply be copied across.
         *
         * @param cloneMe the saturated block to clone.
         */
        NSatBlock(const NSatBlock& cloneMe);
        /**
         * Destroys all internal arrays.  Note that any adjacent blocks
         * that are referenced by the \a adjBlock array will \e not be
         * destroyed.
         */
        ~NSatBlock();

        /**
         * Returns a newly created clone of this saturated block structure.
         * A clone of the correct subclass of NSatBlock will be returned.
         * For this reason, each subclass of NSatBlock must implement this
         * routine.
         *
         * @return a new clone of this block.
         */
        virtual NSatBlock* clone() const = 0;

        /**
         * Returns the number of annuli on the boundary of this
         * saturated block.
         *
         * @return the number of boundary annuli.
         */
        unsigned nAnnuli() const;

        /**
         * Returns details of the requested annulus on the boundary of
         * this saturated block.  Annuli are numbered from 0 to
         * nAnnuli()-1 as described in the class notes.
         *
         * @param which indicates which boundary annulus is requested;
         * this must be between 0 and nAnnuli()-1 inclusive.
         * @return a reference to the requested boundary annulus.
         */
        const NSatAnnulus& annulus(unsigned which) const;

        /**
         * Is the ring of boundary annuli twisted to form a long Mobius
         * strip?
         *
         * Recall from the class notes that the twist occurs between
         * boundary annuli nAnnuli()-1 and 0.
         *
         * @return \c true if the ring of boundary annuli is twisted, or
         * \c false if not.
         */
        bool twistedBoundary() const;

        /**
         * Returns whether there is another saturated block listed as
         * being adjacent to the given boundary annulus of this block.
         *
         * @param which indicates which boundary annulus of this block
         * should be examined; this must be between 0 and nAnnuli()-1
         * inclusive.
         * @return \c true if the given boundary annulus has an adjacent
         * block listed, or \c false otherwise.
         */
        bool hasAdjacentBlock(unsigned whichAnnulus) const;

        /**
         * Lists the given saturated block as being adjacent to the
         * given boundary annulus of this block.  Both block structures
         * (this and the given block) will be updated.
         *
         * @param which indicates which boundary annulus of this block
         * has the new adjacency; this must be between 0 and nAnnuli()-1
         * inclusive.
         * @param adjBlock the other saturated block that is adjacent to
         * this.
         * @param adjAnnulus indicates which boundary annulus of the
         * adjacent block meets the given boundary annulus of this block;
         * this must be between 0 and adjBlock->nAnnuli()-1 inclusive.
         * @param adjReflected indicates whether the new adjacency is
         * reflected (see the class notes for details).
         * @param adjBackwards indicates whether the new adjacency is
         * backwards (see the class notes for details).
         */
        void setAdjacent(unsigned whichAnnulus, NSatBlock* adjBlock,
                unsigned adjAnnulus, bool adjReflected, bool adjBackwards);

        /**
         * Adjusts the given Seifert fibred space to insert the contents
         * of this saturated block.  In particular, the space should be
         * adjusted as though an ordinary solid torus (base orbifold a
         * disc, no twists or exceptional fibres) had been replaced by
         * this block.  This description does not make sense for blocks
         * with twisted boundary; the twisted case is discussed below.
         *
         * If the argument \a reflect is \c true, it should be assumed
         * that this saturated block is being reflected before being
         * inserted into the larger Seifert fibred space.  That is, any
         * twists or exceptional fibres should be negated before being
         * added.
         *
         * Regarding the signs of exceptional fibres:  Consider a
         * saturated block containing a solid torus whose meridinal curve
         * runs \a p times horizontally around the boundary in order through
         * annuli 0,1,... and follows the fibres \a q times from bottom
         * to top (as depicted in the diagram in the NSatBlock class
         * notes).  Then this saturated block adds a positive (\a p, \a q)
         * fibre to the underlying Seifert fibred space.
         *
         * If the ring of saturated annuli bounding this block is
         * twisted then the situation becomes more complex.  It can be
         * proven that such a block must contain a reflector boundary
         * in the base orbifold (use Z_2 homology with fibre-reversing
         * paths to show that the base orbifold must contain another
         * boundary component, and then recall that real boundaries are
         * not allowed inside blocks).
         *
         * In this twisted boundary case, it should be assumed that the
         * reflector boundary and the fibre-reversing path around the
         * boundary annuli are already stored in the given Seifert
         * fibred space.  This routine should make any further changes
         * that are required (there may well be none).  That is, the
         * space should be adjusted as though a trivial Seifert fibred
         * space over the annulus with one reflector boundary had been
         * replaced by this block.  In particular, this routine should
         * \e not add the reflector boundary itself.
         *
         * @param sfs the Seifert fibred space to adjust.
         * @param reflect \c true if this block is to be reflected, or
         * \c false if it should be inserted directly.
         */
        virtual void adjustSFS(NSFSpace& sfs, bool reflect) const = 0;

        /**
         * Adjusts the structure of this block according to the given
         * isomorphism between triangulations.  Any triangulation-specific
         * information will be transformed accordingly (for instance, the
         * routine NSatAnnulus::transform() will be called for each
         * boundary annulus).
         *
         * Information regarding adjacent blocks will \e not be changed.
         * Only structural information for this particular block will be
         * updated.
         *
         * The given isomorphism must describe a mapping from \a originalTri
         * to \a newTri, and this block must currently refer to tetrahedra in
         * \a originalTri.  After this routine is called the block will
         * instead refer to the corresponding tetrahedra in \a newTri (with
         * changes in vertex/face numbering also accounted for).
         *
         * \pre This block currently refers to tetrahedra in \a originalTri,
         * and \a iso describes a mapping from \a originalTri to \a newTri.
         *
         * \warning Any subclasses of NSatBlock that store additional
         * triangulation-specific information will need to override this
         * routine.  When doing so, be sure to call NSatBlock::transform()
         * so that the generic changes defined here will still take place.
         *
         * @param originalTri the triangulation currently used by this
         * saturated block.
         * @param iso the mapping from \a originalTri to \a newTri.
         * @param newTri the triangulation to be used by the updated
         * block structure.
         */
        virtual void transform(const NTriangulation* originalTri,
                const NIsomorphism* iso, NTriangulation* newTri);

        /**
         * Determines whether the given annulus is in fact a boundary
         * annulus for a recognised type of saturated block.  The
         * annulus should be represented from the inside of the proposed
         * saturated block.
         *
         * Only certain types of saturated block are recognised by this
         * routine.  More exotic saturated blocks will not be identified,
         * and this routine will return \c null in such cases.
         *
         * The given list of tetrahedra will not be examined by this
         * routine.  That is, only saturated blocks that do not contain
         * any of these tetrahedra will be considered.  As a consequence,
         * if the given annulus uses any of these tetrahedra then \c null
         * will be returned.
         *
         * If a block is found on the other hand, all of the tetrahedra
         * within this block will be added to the given list.
         *
         * In the event that a block is found, it is guaranteed that the
         * given annulus will be listed as annulus number 0 in the block
         * structure, without any horizontal or vertical reflection.
         *
         * @param annulus the proposed boundary annulus that should form
         * part of the new saturated block.
         * @param avoidTets the list of tetrahedra that should not be
         * considered, and to which any new tetrahedra will be added.
         * @return details of the saturated block if one was found, or
         * \c null if none was found.
         */
        static NSatBlock* isBlock(const NSatAnnulus& annulus,
            TetList& avoidTets);

    protected:
        /**
         * Constructor for a block with the given number of annuli on
         * the boundary.
         *
         * All arrays will be constructed but their contents will remain
         * uninitialised, with the exception that the \a adjBlock array
         * will be filled with null pointers.
         *
         * @param nAnnuli the number of annuli on the boundary of this
         * block; this must be strictly positive.
         * @param twistedBoundary \c true if the ring of boundary annuli
         * is twisted to form a long Mobius band, or \c false (the default)
         * if it is not.
         */
        NSatBlock(unsigned nAnnuli, bool twistedBoundary = false);

        /**
         * Determines whether the given tetrahedron is contained within the
         * given list.
         *
         * This is intended as a helper routine for isBlock(), in the
         * event that the data structure TetList should change.
         *
         * @param t the tetrahedron to search for.
         * @param list the list in which to search.
         * @return \c true if and only if the given tetrahedron was found.
         */
        static bool isBad(NTetrahedron* t, const TetList& list);
};

/*@}*/

// Inline functions for NSatBlock

inline NSatBlock::NSatBlock(unsigned nAnnuli, bool twistedBoundary) :
        nAnnuli_(nAnnuli),
        annulus_(new NSatAnnulus[nAnnuli]),
        twistedBoundary_(twistedBoundary),
        adjBlock_(new NSatBlock*[nAnnuli]),
        adjAnnulus_(new unsigned[nAnnuli]),
        adjReflected_(new bool[nAnnuli]),
        adjBackwards_(new bool[nAnnuli]) {
    for (unsigned i = 0; i < nAnnuli; i++)
        adjBlock_[i] = 0;
}

inline NSatBlock::~NSatBlock() {
    delete[] annulus_;
    delete[] adjBlock_;
    delete[] adjAnnulus_;
    delete[] adjReflected_;
    delete[] adjBackwards_;
}

inline unsigned NSatBlock::nAnnuli() const {
    return nAnnuli_;
}

inline const NSatAnnulus& NSatBlock::annulus(unsigned which) const {
    return annulus_[which];
}

inline bool NSatBlock::twistedBoundary() const {
    return twistedBoundary_;
}

inline bool NSatBlock::hasAdjacentBlock(unsigned whichAnnulus) const {
    return (adjBlock_[whichAnnulus] != 0);
}

inline void NSatBlock::setAdjacent(unsigned whichAnnulus, NSatBlock* adjBlock,
        unsigned adjAnnulus, bool adjReflected, bool adjBackwards) {
    adjBlock_[whichAnnulus] = adjBlock;
    adjAnnulus_[whichAnnulus] = adjAnnulus;
    adjReflected_[whichAnnulus] = adjReflected;
    adjBackwards_[whichAnnulus] = adjBackwards;

    adjBlock->adjBlock_[adjAnnulus] = this;
    adjBlock->adjAnnulus_[adjAnnulus] = whichAnnulus;
    adjBlock->adjReflected_[adjAnnulus] = adjReflected;
    adjBlock->adjBackwards_[adjAnnulus] = adjBackwards;
}

} // namespace regina

#endif

