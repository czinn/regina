
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nfacegluingitems.h
 *  \brief Provides table items for editing triangulation face gluings.
 */

#ifndef __NFACEGLUINGITEMS_H
#define __NFACEGLUINGITEMS_H

#include "triangulation/nperm.h"

#include <qtable.h>

/**
 * A table item for tetrahedron names.
 */
class TetNameItem : public QTableItem {
    private:
        QString name;
            /**< The current tetrahedron name. */

    public:
        /**
         * Constructor.
         */
        TetNameItem(QTable* table, unsigned long tetNum,
                const QString& tetName);

        /**
         * Query properties.
         */
        const QString& getName() const;

        /**
         * QTableItem overrides.
         */
        int alignment() const;
        virtual QWidget* createEditor() const;
        virtual void setContentFromEditor(QWidget* editor);
};

/**
 * A table item for an individual face gluing.
 */
class FaceGluingItem : public QTableItem {
    private:
        long adjTet;
            /**< The adjacent tetrahedron, or -1 if this is a boundary face. */
        regina::NPerm adjPerm;
            /**< The adjacent tetrahedron gluing. */

        bool error;
            /**< Are we currently displaying an error message? */
    public:
        /**
         * Constructors.  The first constructor is for a boundary face,
         * the second for a face that is glued elsewhere.
         */
        FaceGluingItem(QTable* table);
        FaceGluingItem(QTable* table, int myFace, unsigned long destTet,
            const regina::NPerm& gluingPerm);

        /**
         * Query properties.
         */
        long getAdjacentTetrahedron() const;
        const regina::NPerm& getAdjacentTetrahedronGluing() const;

        /**
         * QTableItem overrides.
         */
        virtual QWidget* createEditor() const;
        virtual void setContentFromEditor(QWidget* editor);

    private:
        /**
         * Return a short string describing the destination of a face
         * gluing.
         */
        static QString destString(int srcFace, int destTet,
                const regina::NPerm& gluing);

        /**
         * Display the given error to the user if no error is already
         * being displayed.
         */
        void showError(const QString& message);
};

inline const QString& TetNameItem::getName() const {
    return name;
}

inline long FaceGluingItem::getAdjacentTetrahedron() const {
    return adjTet;
}

inline const regina::NPerm& FaceGluingItem::getAdjacentTetrahedronGluing()
        const {
    return adjPerm;
}

#endif
