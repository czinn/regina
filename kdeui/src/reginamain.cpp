
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

#include <qdragobject.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qprintdialog.h>
#include <qpaintdevicemetrics.h>
#include <qpainter.h>
#include <qvbox.h>
#include <kaccel.h>
#include <kaction.h>
#include <kconfig.h>
#include <kedittoolbar.h>
#include <kfiledialog.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <kio/netaccess.h>
#include <klibloader.h>
#include <kkeydialog.h>
#include <klocale.h>
#include <kmenubar.h>
#include <kparts/event.h>
#include <kstatusbar.h>
#include <kstdaccel.h>
#include <kstdaction.h>
#include <ktexteditor/document.h>
#include <ktexteditor/view.h>
#include <ktrader.h>
#include <kurl.h>
#include <kurlrequesterdlg.h>

ReginaMain::ReginaMain() : KParts::MainWindow( 0, "Regina" ),
        currentPart(0), currentGUI(0) {

    if (! initialGeometrySet())
        resize(640, 400);

    // accept dnd
    setAcceptDrops(true);

    setXMLFile("reginamain.rc");

    // then, setup our actions
    setupActions();

    // and a status bar
    statusBar()->show();

    // allow the view to change the statusbar and caption
    /*
    connect(m_view, SIGNAL(signalChangeStatusbar(const QString&)),
            this,   SLOT(changeStatusbar(const QString&)));
    connect(m_view, SIGNAL(signalChangeCaption(const QString&)),
            this,   SLOT(changeCaption(const QString&)));
            */

    readOptions(kapp->config());
}

ReginaMain::~ReginaMain() {
}

void ReginaMain::load(const KURL& url) {
    // Are we looking at a Regina data file?

    // Otherwise we'll use KParts to find an appropriate central widget.
    
    // Are we looking at a Python file (text/x-python)?
    // If so, fire up our preferred text editor.

    KTrader::OfferList offers = KTrader::self()->query("KTextEditor/Document");
    ASSERT( offers.count() >= 1 );

    KService::Ptr service = *offers.begin();
    KLibFactory *libFactory = KLibLoader::self()->factory(service->library());
    ASSERT( libFactory );
    KTextEditor::Document* editor =
        (KTextEditor::Document*)(libFactory->create(this, service->name(),
        "KTextEditor::Document"));
    ASSERT(editor);

    KTextEditor::View* view = editor->createView(this, 0);
    setCentralWidget(view);
    guiFactory()->addClient(view);

    connect(editor, SIGNAL(setStatusBarText(const QString&)),
        this, SLOT(changeStatusbar(const QString&)));
    connect(editor, SIGNAL(setWindowCaption(const QString&)),
        this, SLOT(changeCaption(const QString&)));

    editor->openURL(url);
    view->show();

    currentPart = editor;
    currentGUI = view;

    //QString target;
    // the below code is what you should normally do.  in this
    // example case, we want the url to our own.  you probably
    // want to use this code instead for your app

    #if 0
    // download the contents
    if (KIONetAccess::download(url, target))
    {
        // set our caption
        setCaption(url);

        // load in the file (target is always local)
        loadFile(target);

        // and remove the temp file
        KIONetAccess::removeTempFile(target);
    }
    #endif

    //setCaption(url.url());
    //load(url);
}

void ReginaMain::setupActions() {
    KStdAction::openNew(this, SLOT(fileNew()), actionCollection());
    KStdAction::open(this, SLOT(fileOpen()), actionCollection());
    fileOpenRecent = KStdAction::openRecent(this,
        SLOT(load(const KURL&)), actionCollection());
    KStdAction::close(this, SLOT(close()), actionCollection());
    KStdAction::quit(kapp, SLOT(quit()), actionCollection());

    showToolbar = KStdAction::showToolbar(this, SLOT(optionsShowToolbar()), actionCollection());
    showStatusbar = KStdAction::showStatusbar(this, SLOT(optionsShowStatusbar()), actionCollection());

    KStdAction::keyBindings(this, SLOT(optionsConfigureKeys()), actionCollection());
    KStdAction::configureToolbars(this, SLOT(optionsConfigureToolbars()), actionCollection());
    KStdAction::preferences(this, SLOT(optionsPreferences()), actionCollection());

    // this doesn't do anything useful.  it's just here to illustrate
    // how to insert a custom menu and menu item
    new KAction(i18n("Python Console (&Standalone)"), "source_py",
        0 /* shortcut */, this, SLOT(optionsPreferences()),
        actionCollection(), "python_standalone");
    createGUI(0);
}

void ReginaMain::saveProperties(KConfig *config) {
    // the 'config' object points to the session managed
    // config file.  anything you write here will be available
    // later when this app is restored
    
    if (currentPart) {
        QString url = currentPart->url().url();
        if (url != QString::null)
            config->writeEntry("lastURL", url);
    }
}

void ReginaMain::readProperties(KConfig *config) {
    // the 'config' object points to the session managed
    // config file.  this function is automatically called whenever
    // the app is being restored.  read in here whatever you wrote
    // in 'saveProperties'

    QString url = config->readEntry("lastURL"); 

    if (url != QString::null)
        load(KURL(url));
}

void ReginaMain::dragEnterEvent(QDragEnterEvent *event) {
    // accept uri drops only
    event->accept(QUriDrag::canDecode(event));
}

void ReginaMain::dropEvent(QDropEvent *event) {
    // this is a very simplistic implementation of a drop event.  we
    // will only accept a dropped URL.  the Qt dnd code can do *much*
    // much more, so please read the docs there
    QStrList uri;

    // see if we can decode a URI.. if not, just ignore it
    if (QUriDrag::decode(event, uri))
    {
        // okay, we have a URI.. process it
        QString url, target;
        url = uri.first();

        // load in the file
        load(KURL(url));
    }
}

bool ReginaMain::queryClose() {
    if (currentPart)
        return currentPart->closeURL();
    else
        return true;
}

void ReginaMain::fileNew() {
    // this slot is called whenever the File->New menu is selected,
    // the New shortcut is pressed (usually CTRL+N) or the New toolbar
    // button is clicked

    // create a new window
    (new ReginaMain)->show();
}

void ReginaMain::fileOpen() {
    // this slot is called whenever the File->Open menu is selected,
    // the Open shortcut is pressed (usually CTRL+O) or the Open toolbar
    // button is clicked
    KURL url = KURLRequesterDlg::getURL(QString::null, this, i18n("Open Location") );
    if (!url.isEmpty())
        load(url);
}

void ReginaMain::fileSave()
{
    // this slot is called whenever the File->Save menu is selected,
    // the Save shortcut is pressed (usually CTRL+S) or the Save toolbar
    // button is clicked

    // save the current file
}

void ReginaMain::fileSaveAs() {
    // this slot is called whenever the File->Save As menu is selected,
    KURL file_url = KFileDialog::getSaveURL();
    if (!file_url.isEmpty() && !file_url.isMalformed())
    {
        // save your info, here
    }
}

void ReginaMain::optionsShowToolbar() {
    // this is all very cut and paste code for showing/hiding the
    // toolbar
    if (showToolbar->isChecked())
        toolBar()->show();
    else
        toolBar()->hide();
}

void ReginaMain::optionsShowStatusbar() {
    // this is all very cut and paste code for showing/hiding the
    // statusbar
    if (showStatusbar->isChecked())
        statusBar()->show();
    else
        statusBar()->hide();
}

void ReginaMain::optionsConfigureKeys() {
    KKeyDialog::configure(actionCollection());
}

void ReginaMain::optionsConfigureToolbars() {
    // use the standard toolbar editor
    KEditToolbar dlg(guiFactory());
    if (dlg.exec()) {
        // recreate our GUI
        //createGUI("reginamain.rc", false);
        //createGUI(currentGUI);
        createGUI(0);
        // TODO: wtf?
    } 
}

void ReginaMain::optionsPreferences() {
    // popup some sort of preference dialog, here
    ReginaPreferences dlg(this);
    if (dlg.exec()) {
        // redo your settings
        saveOptions();
    }
}

void ReginaMain::changeStatusbar(const QString& text) {
    // display the text on the statusbar
    statusBar()->message(text);
}

void ReginaMain::changeCaption(const QString& text) {
    // display the text on the caption
    setCaption(text);
}

void ReginaMain::readOptions(KConfig* config) {
    config->setGroup("Display");
    setAutoDock(config->readBoolEntry("Packet docking", true));
    setDisplayIcon(config->readBoolEntry("Display icon", true));

    config->setGroup("File");
    setAutoFileExtension(config->readBoolEntry("Automatic extension", true));

    fileOpenRecent->loadEntries(config);
}

void ReginaMain::saveOptions() {
    KConfig* config = kapp->config();

    // Save the current set of options.
    config->setGroup("Display");
    config->writeEntry("Packet docking", autoDock);
    config->writeEntry("Display icon", displayIcon);

    config->setGroup("File");
    config->writeEntry("Automatic extension", autoFileExtension);

    fileOpenRecent->saveEntries(config);

    config->sync();

    // Make sure other main windows read in the new options.
    for (ReginaMain* otherMain = (ReginaMain*)(memberList->first()); otherMain;
            otherMain = (ReginaMain*)(memberList->next()))
        if (otherMain != this)
            otherMain->readOptions(config);
}

void ReginaMain::setDisplayIcon(bool value) {
    displayIcon = value;
}

#include "reginamain.moc"
