
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include "NVertexI.h"
#include "NEdgeI.h"
#include "NComponentI.h"
#include "NBoundaryComponentI.h"
#include "NTetrahedronI.h"

Regina::Triangulation::NComponent_ptr NEdge_i::getComponent() {
    return NComponent_i::newWrapper(MY_ENGINE_OBJECT->getComponent());
}
Regina::Triangulation::NBoundaryComponent_ptr
        NEdge_i::getBoundaryComponent() {
    return NBoundaryComponent_i::newWrapper(MY_ENGINE_OBJECT->
        getBoundaryComponent());
}
Regina::Triangulation::NVertex_ptr NEdge_i::getVertex(CORBA::Long index) {
    return NVertex_i::newWrapper(MY_ENGINE_OBJECT->getVertex((int)index));
}
CORBA::Boolean NEdge_i::isBoundary() {
    return MY_ENGINE_OBJECT->isBoundary();
}
CORBA::Boolean NEdge_i::isValid() {
    return MY_ENGINE_OBJECT->isValid();
}
CORBA::Long NEdge_i::getNumberOfEmbeddings() {
    return MY_ENGINE_OBJECT->getNumberOfEmbeddings();
}
void NEdge_i::getEmbedding(Regina::Triangulation::NTetrahedron_out tet,
        CORBA::Long& edge, CORBA::Long index) {
    const NEdgeEmbedding& emb = MY_ENGINE_OBJECT->getEmbedding(index);
    tet = NTetrahedron_i::newWrapper(emb.getTetrahedron());
    edge = emb.getEdge();
}

