
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

#include "reginamain.h"
#include "reginapref.h"

#include <kcombobox.h>
#include <kiconloader.h>
#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qvalidator.h>
#include <qwhatsthis.h>

ReginaPreferences::ReginaPreferences(ReginaMain* parent) :
        KDialogBase(IconList, i18n("Regina Preferences"),
        Help|Ok|Apply|Cancel, Ok), mainWindow(parent) {
    // Construct the individual preferences pages.
    QVBox* frame = addVBoxPage(i18n("General"), i18n("General Options"),
        BarIcon("regina", KIcon::SizeMedium));
    generalPrefs = new ReginaPrefGeneral(frame);

    frame = addVBoxPage(i18n("Triangulation"), i18n("Triangulation Options"),
        BarIcon("packet_triangulation", KIcon::SizeMedium));
    triPrefs = new ReginaPrefTri(frame);

    frame = addVBoxPage(i18n("Python"), i18n("Python Options"),
        BarIcon("source_py", KIcon::SizeMedium));
    pythonPrefs = new ReginaPrefPython(frame);

    // Read the current preferences from the main window.
    generalPrefs->cbAutoDock->setChecked(mainWindow->getAutoDock());
    generalPrefs->cbAutoFileExtension->setChecked(
        mainWindow->getAutoFileExtension());
    generalPrefs->cbDisplayIcon->setChecked(mainWindow->getDisplayIcon());

    triPrefs->comboEditMode->setCurrentItem(
        mainWindow->getTriEditMode() == ReginaMain::DirectEdit ? 0 : 1);
    triPrefs->editSurfacePropsThreshold->setText(
        QString::number(mainWindow->getTriSurfacePropsThreshold()));
}

int ReginaPreferences::exec() {
    // Apply changes if OK was pressed.
    int ret = KDialogBase::exec();
    if (ret)
        slotApply();
    return ret;
}

void ReginaPreferences::slotApply() {
    // Propagate changes to the main window.
    mainWindow->setAutoDock(generalPrefs->cbAutoDock->isChecked());
    mainWindow->setDisplayIcon(generalPrefs->cbDisplayIcon->isChecked());
    mainWindow->setAutoFileExtension(
        generalPrefs->cbAutoFileExtension->isChecked());

    mainWindow->setTriEditMode(
        triPrefs->comboEditMode->currentItem() == 0 ?
        ReginaMain::DirectEdit : ReginaMain::Dialog);

    bool ok;
    unsigned uintVal =
        triPrefs->editSurfacePropsThreshold->text().toUInt(&ok);
    if (ok)
        mainWindow->setTriSurfacePropsThreshold(uintVal);
    else {
        KMessageBox::error(triPrefs, i18n("The surface calculation "
            "threshold must be a non-negative integer.  "
            "This is the maximum number of tetrahedra for which normal "
            "surface properties will be calculated automatically."));
        triPrefs->editSurfacePropsThreshold->setText(
            QString::number(mainWindow->getTriSurfacePropsThreshold()));
    }

    // Save these preferences to the global configuration.
    mainWindow->saveOptions();
}

ReginaPrefGeneral::ReginaPrefGeneral(QWidget* parent) : QVBox(parent) {
    cbAutoDock = new QCheckBox(i18n("Automatic packet docking"), this);
    QWhatsThis::add(cbAutoDock, i18n("Try to dock new packet viewers into "
        "the main window instead of opening them in new windows."));

    cbAutoFileExtension = new QCheckBox(i18n("Automatic file extension"), this);
    QWhatsThis::add(cbAutoFileExtension, i18n("Append the default extension "
        "to filenames when saving if no extension is already given."));

    cbDisplayIcon = new QCheckBox(i18n("Display icon"), this);
    QWhatsThis::add(cbDisplayIcon, i18n("Display the large Regina icon "
        "beneath the packet tree."));

    // Add some space at the end.
    setStretchFactor(new QWidget(this), 1);
}

ReginaPrefTri::ReginaPrefTri(QWidget* parent) : QVBox(parent) {
    setSpacing(5);

    // Set up the edit mode.
    // Note that any change of order in the combo box must be reflected
    // in the ReginaPreferences methods as well.
    QHBox* box = new QHBox(this);
    box->setSpacing(5);

    QLabel* label = new QLabel(i18n("Edit mode:"), box);
    comboEditMode = new KComboBox(box);
    comboEditMode->insertItem(SmallIcon("editclear"), i18n("Direct edit"));
    comboEditMode->insertItem(SmallIcon("view_text"), i18n("Pop-up dialog"));
    QString msg = i18n("Specifies the way in which face gluings are edited.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(comboEditMode, msg);

    // Set up the surface properties threshold.
    box = new QHBox(this);
    box->setSpacing(5);

    label = new QLabel(i18n("Surface calculation threshold:"), box);
    editSurfacePropsThreshold = new KLineEdit(box);
    editSurfacePropsThreshold->setMaxLength(
         3 /* ridiculously high number of digits */);
    editSurfacePropsThreshold->setValidator(new QIntValidator(0,
         999 /* ridiculously high */, box));
    msg = i18n("The maximum number of tetrahedra for which normal "
        "surface properties will be calculated automatically.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(editSurfacePropsThreshold, msg);

    // Add some space at the end.
    setStretchFactor(new QWidget(this), 1);
}

ReginaPrefPython::ReginaPrefPython(QWidget* parent) : QVBox(parent) {
    new QLabel(i18n("Python scripting is not yet implemented."), this);

    // Add some space at the end.
    setStretchFactor(new QWidget(this), 1);
}

#include "reginapref.moc"
