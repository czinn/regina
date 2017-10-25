
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2017, Ben Burton                                   *
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

// Regina core includes:
#include "link/link.h"
#include "maths/laurent.h"
#include "maths/laurent2.h"

// UI includes:
#include "linkpoly.h"
#include "patiencedialog.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QToolTip>
#include <QWhatsThis>

#define MAX_LINK_AUTO_POLYNOMIALS 6

using regina::Packet;

LinkPolynomialUI::LinkPolynomialUI(regina::Link* packet,
        PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), link(packet) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    QLabel* label = new QLabel(tr("<b>Jones</b>"), ui);
    QString msg = tr("The Jones polynomial of this link.<p>"
        "The Jones polynomial is a Laurent polynomial "
        "in the square root of <i>t</i>, and Regina will try to "
        "present it in this way.<p>"
        "However, if you disable unicode in Regina's preferences "
        "then Regina cannot use the square root symbol, and so "
        "the square root of <i>t</i> will be represented by the single "
        "symbol <i>x</i> instead.");
    label->setWhatsThis(msg);
    layout->addWidget(label);

    layout->addSpacing(5);

    QHBoxLayout* sublayout = new QHBoxLayout();
    sublayout->setContentsMargins(0, 0, 0, 0);
    sublayout->setSpacing(0);
    btnJones = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnJones->setToolTip(tr("Calculate the Jones polynomial"));
    btnJones->setWhatsThis(tr("<qt>Calculate the Jones polynomial "
        "of this link.<p>"
        "<b>Warning:</b> This calculation may be slow for "
        "larger links (which is why the Jones polynomial is not "
        "always computed automatically).</qt>"));
    sublayout->addWidget(btnJones);
    connect(btnJones, SIGNAL(clicked()), this, SLOT(calculateJones()));
    jones = new QLabel(ui);
    jones->setWordWrap(true);
    jones->setWhatsThis(msg);
    sublayout->addWidget(jones);
    layout->addLayout(sublayout);

    layout->addSpacing(10);

    sublayout = new QHBoxLayout();
    sublayout->setContentsMargins(0, 0, 0, 0);
    sublayout->setSpacing(0);
    label = new QLabel(tr("<b>HOMFLY-PT</b>"), ui);
    msg = trUtf8("The HOMFLY-PT polynomial of this link.<p>"
        "There are two tightly related variants of this polynomial: "
        "one as a Laurent polynomial in the variables (α, <i>z</i>), and "
        "one as a Laurent polynomial in the variables (<i>l</i>, <i>m</i>).  "
        "You can switch between these two variants using the buttons "
        "provided.");
    label->setWhatsThis(msg);
    sublayout->addWidget(label);
    sublayout->addStretch(1);
    btnAZ = new QRadioButton(ui);
    btnAZ->setToolTip(trUtf8("Present the HOMFLY-PT polynomial "
        "using the variant in variables (α, <i>z</i>)."));
    btnAZ->setWhatsThis(trUtf8("Presents the HOMFLY-PT polynomial "
        "using the variant in variables (α, <i>z</i>).<p>"
        "There are two tightly related variants of the HOMFLY-PT polynomial: "
        "one as a Laurent polynomial in the variables (α, <i>z</i>), and "
        "one as a Laurent polynomial in the variables (<i>l</i>, <i>m</i>).  "
        "These buttons allow you to switch between these two variants."));
    sublayout->addWidget(btnAZ);
    sublayout->addSpacing(5);
    btnLM = new QRadioButton(ui);
    btnLM->setToolTip(trUtf8("Present the HOMFLY-PT polynomial "
        "using the variant in variables (<i>l</i>, <i>m</i>)."));
    btnLM->setWhatsThis(trUtf8("Presents the HOMFLY-PT polynomial "
        "using the variant in variables (<i>l</i>, <i>m</i>).<p>"
        "There are two tightly related variants of the HOMFLY-PT polynomial: "
        "one as a Laurent polynomial in the variables (α, <i>z</i>), and "
        "one as a Laurent polynomial in the variables (<i>l</i>, <i>m</i>).  "
        "These buttons allow you to switch between these two variants."));
    sublayout->addWidget(btnLM);
    if (ReginaPrefSet::global().linkHomflyType == ReginaPrefSet::HomflyLM)
        btnLM->setChecked(true);
    else
        btnAZ->setChecked(true);
    connect(btnAZ, SIGNAL(toggled(bool)), this, SLOT(homflyTypeChanged(bool)));
    connect(btnLM, SIGNAL(toggled(bool)), this, SLOT(homflyTypeChanged(bool)));
    layout->addLayout(sublayout);

    layout->addSpacing(5);

    sublayout = new QHBoxLayout();
    sublayout->setContentsMargins(0, 0, 0, 0);
    sublayout->setSpacing(0);
    btnHomfly = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnHomfly->setToolTip(tr("Calculate the HOMFLY-PT polynomial"));
    btnHomfly->setWhatsThis(tr("<qt>Calculate the HOMFLY-PT polynomial "
        "of this link.<p>"
        "<b>Warning:</b> This calculation may be slow for "
        "larger links (which is why the HOMFLY-PT polynomial is not "
        "always computed automatically).</qt>"));
    sublayout->addWidget(btnHomfly);
    connect(btnHomfly, SIGNAL(clicked()), this, SLOT(calculateHomfly()));
    homfly = new QLabel(ui);
    homfly->setWordWrap(true);
    homfly->setWhatsThis(msg);
    sublayout->addWidget(homfly);
    layout->addLayout(sublayout);

    layout->addSpacing(10);

    label = new QLabel(tr("<b>Kauffman bracket</b>"), ui);
    msg = trUtf8("The Kauffman bracket polynomial of this link.");
    label->setWhatsThis(msg);
    layout->addWidget(label);

    layout->addSpacing(5);

    sublayout = new QHBoxLayout();
    sublayout->setContentsMargins(0, 0, 0, 0);
    sublayout->setSpacing(0);
    btnBracket = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnBracket->setToolTip(tr("Calculate the Kauffman bracket"));
    btnBracket->setWhatsThis(tr("<qt>Calculate the Kauffman bracket "
        "of this link.<p>"
        "<b>Warning:</b> This calculation may be slow for "
        "larger links (which is why the Kauffman bracket is not "
        "always computed automatically).</qt>"));
    sublayout->addWidget(btnBracket);
    connect(btnBracket, SIGNAL(clicked()), this, SLOT(calculateBracket()));
    bracket = new QLabel(ui);
    bracket->setWordWrap(true);
    bracket->setWhatsThis(msg);
    sublayout->addWidget(bracket);
    layout->addLayout(sublayout);

    layout->addStretch(1);

    updateLabels();

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));
}

regina::Packet* LinkPolynomialUI::getPacket() {
    return link;
}

QWidget* LinkPolynomialUI::getInterface() {
    return ui;
}

void LinkPolynomialUI::refresh() {
    bool unicode = ReginaPrefSet::global().displayUnicode;

    if (link->knowsJones() || link->size() <= MAX_LINK_AUTO_POLYNOMIALS) {
        btnJones->setVisible(false);
        if (unicode)
            jones->setText(link->jones().utf8(regina::Link::jonesVar).c_str());
        else
            jones->setText(link->jones().str("x").c_str());
    } else {
        jones->setText(QString());
        btnJones->setVisible(true);
    }

    if (link->knowsHomfly() || link->size() <= MAX_LINK_AUTO_POLYNOMIALS) {
        btnHomfly->setVisible(false);
        if (! btnLM->isChecked()) {
            if (unicode)
                homfly->setText(link->homflyAZ().utf8(
                    regina::Link::homflyAZVarX,
                    regina::Link::homflyAZVarY).c_str());
            else
                homfly->setText(link->homflyAZ().str(
                    "a", regina::Link::homflyAZVarY).c_str());
        } else {
            if (unicode)
                homfly->setText(link->homflyLM().utf8(
                    "l", regina::Link::homflyLMVarY).c_str());
            else
                homfly->setText(link->homflyLM().str(
                    "l", regina::Link::homflyLMVarY).c_str());
        }
    } else {
        homfly->setText(QString());
        btnHomfly->setVisible(true);
    }

    if (link->knowsBracket() || link->size() <= MAX_LINK_AUTO_POLYNOMIALS) {
        btnBracket->setVisible(false);
        if (unicode)
            bracket->setText(link->bracket().utf8("A").c_str());
        else
            bracket->setText(link->bracket().str("A").c_str());
    } else {
        bracket->setText(QString());
        btnBracket->setVisible(true);
    }
}

void LinkPolynomialUI::calculateJones() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "Computing Jones polynomials can be slow\n"
        "for larger knots and links.\n\n"
        "Please be patient."), ui);
    link->jones();
    delete dlg;

    refresh();
}

void LinkPolynomialUI::calculateHomfly() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "Computing HOMFLY-PT polynomials can be slow\n"
        "for larger knots and links.\n\n"
        "Please be patient."), ui);
    link->homfly();
    delete dlg;

    refresh();
}

void LinkPolynomialUI::calculateBracket() {
    PatienceDialog* dlg = PatienceDialog::warn(tr(
        "Computing Kauffman brackets can be slow\n"
        "for larger knots and links.\n\n"
        "Please be patient."), ui);
    link->bracket();
    delete dlg;

    refresh();
}

void LinkPolynomialUI::homflyTypeChanged(bool checked) {
    if (checked) {
        ReginaPrefSet::global().linkHomflyType = (btnLM->isChecked() ?
            ReginaPrefSet::HomflyLM : ReginaPrefSet::HomflyAZ);
        refresh();
    }
}

void LinkPolynomialUI::updatePreferences() {
    updateLabels();
    refresh();
}

void LinkPolynomialUI::updateLabels() {
    if (ReginaPrefSet::global().displayUnicode) {
        btnAZ->setText(trUtf8("(α, z)"));
    } else {
        btnAZ->setText(tr("(a, z)"));
    }
    btnLM->setText(tr("(l, m)"));
}

