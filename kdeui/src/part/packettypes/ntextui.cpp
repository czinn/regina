
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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

// Regina core includes:
#include "packet/ntext.h"

// UI includes:
#include "ntextui.h"

#include <cstring>
#include <sstream>
#include <klocale.h>
#include <ktexteditor/document.h>
#include <ktexteditor/editinterface.h>
#include <ktexteditor/undointerface.h>
#include <ktexteditor/view.h>
#include <ktexteditor/wordwrapinterface.h>

using regina::NPacket;
using regina::NText;

NTextUI::NTextUI(NText* packet, PacketPane* enclosingPane,
        KTextEditor::Document* doc) :
        PacketUI(enclosingPane), text(packet), document(doc) {
    // Create a view (which must be parented) before we do anything else.
    // Otherwise the Vim component crashes.
    view = document->createView(enclosingPane);
    if (strcmp(document->className(), "Vim::Document") == 0)
        enclosingPane->setDirtinessBroken();

    document->setReadWrite(enclosingPane->isReadWrite());
    KTextEditor::wordWrapInterface(document)->setWordWrap(true);

    editInterface = KTextEditor::editInterface(document);
    refresh();

    if (strcmp(document->className(), "Vim::Document") == 0)
        std::cerr << "Not flushing the undo list since this has strange "
            "side-effects with the Vim component." << std::endl;
    else
        KTextEditor::undoInterface(document)->clearUndo();

    connect(document, SIGNAL(textChanged()),
        this, SLOT(notifyTextChanged()));
}

NTextUI::~NTextUI() {
    delete document;
}

NPacket* NTextUI::getPacket() {
    return text;
}

QWidget* NTextUI::getInterface() {
    return view;
}

KTextEditor::Document* NTextUI::getTextComponent() {
    return document;
}

QString NTextUI::getPacketMenuText() const {
    return i18n("Te&xt");
}

void NTextUI::commit() {
    text->setText(editInterface->text().ascii());
    setDirty(false);
}

void NTextUI::refresh() {
    // A kate part needs to be in read-write mode before we can alter its
    // contents.
    bool wasReadWrite = editInterface->isReadWrite();
    if (! wasReadWrite)
        editInterface->setReadWrite(true);

    editInterface->clear();

    // Back to all-at-once insertion instead of line-by-line insertion.
    // Grrr vimpart.
    if (! text->getText().empty()) {
        QString data = text->getText().c_str();

        // We are guaranteed that data.length() >= 1.
        if (data[data.length() - 1] == '\n')
            data.truncate(data.length() - 1);

        editInterface->setText(data);
    }

    if (! wasReadWrite)
        editInterface->setReadWrite(false);

    setDirty(false);
}

void NTextUI::setReadWrite(bool readWrite) {
    document->setReadWrite(readWrite);
}

void NTextUI::notifyTextChanged() {
    setDirty(true);
}

#include "ntextui.moc"
