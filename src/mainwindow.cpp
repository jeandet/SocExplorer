/*------------------------------------------------------------------------------
--  This file is a part of the SocExplorer Software
--  Copyright (C) 2011-2015, Plasma Physics Laboratory - CNRS
--
--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 2 of the License, or
--  (at your option) any later version.
--
--  This program is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--
--  You should have received a copy of the GNU General Public License
--  along with this program; if not, write to the Free Software
--  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
-------------------------------------------------------------------------------*/
/*--                  Author : Alexis Jeandet
--                     Mail : alexis.jeandet@lpp.polytechnique.fr
----------------------------------------------------------------------------*/
#include "mainwindow.h"
#include <QDockWidget>
#include <socexplorersettings.h>
#include <socexplorerconfigkeys.h>

SocExplorerMainWindow::SocExplorerMainWindow(QString ScriptToEval, QWidget *parent)
    : QMainWindow(parent)
{
    QCoreApplication::setApplicationName("SocExplorer");
    QCoreApplication::setOrganizationName("LPP");
    QCoreApplication::setOrganizationDomain("lpp.fr");
    this->makeObjects(ScriptToEval);
    this->makeLayout();
    this->makeMenu();
    SocExplorerSettings::init();
    this->makeConnections();
    this->setWindowIcon(QIcon(":/images/icon.png"));
    this->setAcceptDrops(true);
    this->PythonConsoleInst->pyConsoleRunFile(ScriptToEval);
    QFile file(":/styles/SocExplorer.css");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qApp->setStyleSheet(file.readAll());
        file.close();
    }
}


void SocExplorerMainWindow::makeObjects(QString ScriptToEval)
{
    Q_UNUSED(ScriptToEval)
    this->p_pluginGUIlist = new QList<QDockWidget*>();
    pluginsDockContainer = new QMainWindow;
    pluginsDockContainer->setObjectName("pluginsDockContainer");
    pluginsDockContainer->setWindowFlags(Qt::Widget);
    pluginsDockContainer->setDockNestingEnabled(true);
    this->sessionsActions = new QActionGroup(this);
    this->sessionManagerAction =new QAction(tr("&Session manager"),this);
    this->mainWidget = new QSplitter(Qt::Vertical);
    this->appTranslator = new QTranslator;
    this->Quit = new QAction(tr("&Quit"),this);
    this->Quit->setShortcut(tr("CTRL+Q"));
    this->ManagePlugins = new QAction(tr("&Manage Plugins"),this);
    this->ManagePlugins->setShortcut(tr("CTRL+P"));
    this->regsManager = new QAction(tr("&Manage registers"),this);
    this->exploreRegs = new QAction(tr("&Explore registers"),this);
    this->help = new QAction(tr("&Help"),this);
    this->help->setShortcut(tr("CTRL+H"));
    this->about = new QAction(tr("&About"),this);
    this->p_SessionManagerDialog = new SessionManagerDialog();
    socexplorerproxy::setMainWindow(this);
    SocExplorerEngine::setMainWindow(this);
    SocExplorerEngine::xmlModel()->scanXmlFiles();
    this->regExplorer = new RegsExplorer();
    this->regExplorer->setAllowedAreas(Qt::AllDockWidgetAreas);
    this->regExplorer->setObjectName("regExplorer");
    this->addPluginInterface(this->regExplorer);
    this->PythonConsoleInst = new PythonConsole(socexplorerproxy::self());
    this->PythonConsoleInst->addObject("SocExplorerEngine",SocExplorerEngine::self());
    this->pluginManager = new dockablePluginManager();
    this->toolpane = new toolBar;
    this->toolpane->setObjectName("toolpane");
    this->p_about = new aboutsocexplorer();
}

void SocExplorerMainWindow::makeLayout()
{
    this->mainWidget->addWidget(pluginsDockContainer);
    this->mainWidget->addWidget(this->PythonConsoleInst);
    this->toolpane->setAllowedAreas(Qt::RightDockWidgetArea|Qt::LeftDockWidgetArea);
    this->addDockWidget(Qt::LeftDockWidgetArea,this->toolpane);
    this->toolpane->addTool(this->pluginManager);
    this->setCentralWidget(this->mainWidget);
}


void SocExplorerMainWindow::makeConnections()
{
    connect(socexplorerproxy::self(),SIGNAL(clearMenu()),this,SLOT(clearMenu()));
    connect(this,SIGNAL(translateSig()),socexplorerproxy::self(),SLOT(updateText()));
    connect(socexplorerproxy::self(),SIGNAL(addPluginGUI(QDockWidget*)),this,SLOT(addPluginInterface(QDockWidget*)));
    connect(socexplorerproxy::self(),SIGNAL(removePluginGUI(QDockWidget*)),this,SLOT(removePluginInterface(QDockWidget*)));
    connect(this->ManagePlugins,SIGNAL(triggered()),this,SLOT(launchPluginManager()));
    connect(this->Quit,SIGNAL(triggered()),qApp,SLOT(quit()));
    connect(this,SIGNAL(registerObject(QObject*,QString)),this->PythonConsoleInst,SLOT(registerObject(QObject*,QString)));
    connect(socexplorerproxy::self(),SIGNAL(registerObject(QObject*,QString)),this,SIGNAL(registerObject(QObject*,QString)));
    connect(this->pluginManager,SIGNAL(geteplugintree()),socexplorerproxy::self(),SLOT(geteplugintree()));
    connect(socexplorerproxy::self(),SIGNAL(treeChanged(QList<socexplorerplugin*>)),this->pluginManager,SIGNAL(treeChanged(QList<socexplorerplugin*>)));
    connect(this->pluginManager,SIGNAL(changeSysDriverInstName(QString,QString)),socexplorerproxy::self(),SLOT(changeSysDriverInstName(QString,QString)));
    connect(this->pluginManager,SIGNAL(changeSysDriverInstName(QString,QString)),this->PythonConsoleInst,SLOT(changeSysDriverInstName(QString,QString)));
    connect(this->pluginManager,SIGNAL(closeSysDriver(QString)),socexplorerproxy::self(),SLOT(closeSysDriver(QString)));
    connect(this->pluginManager,SIGNAL(closeSysDriver(QString)),this->PythonConsoleInst,SLOT(removeDriver(QString)));
    connect(this->pluginManager,SIGNAL(pluginselected(QString)),this,SLOT(pluginselected(QString)));
    connect(this->about,SIGNAL(triggered()),this,SLOT(showAboutBox()));
    connect(this->exploreRegs,SIGNAL(triggered()),this->regExplorer,SLOT(show()));

    connect(this->sessionManagerAction, SIGNAL(triggered(bool)),this,SLOT(showSessionManager(bool)));
    connect(this->p_SessionManagerDialog, SIGNAL(switchSession(QString)),this,SLOT(setActiveSession(QString)));
    connect(this->p_SessionManagerDialog, SIGNAL(sessionRenamed(QString,QString)),this,SLOT(renameSession(QString,QString)));
    connect(this->p_SessionManagerDialog, SIGNAL(sessionAdded(QString)),this,SLOT(addSession(QString)));
    connect(this->p_SessionManagerDialog, SIGNAL(sessionRemoved(QString)),this,SLOT(removeSession(QString)));
    connect(this->sessionsActions,SIGNAL(triggered(QAction*)),this,SLOT(setActiveSession(QAction*)));

    this->pluginManager->connect(this->pluginManager,SIGNAL(loadSysDrviver(QString)),socexplorerproxy::self(),SLOT(loadSysDriver(QString)));
    this->pluginManager->connect(this->pluginManager,SIGNAL(loadSysDriverToParent(QString,QString)),socexplorerproxy::self(),SLOT(loadSysDriverToParent(QString,QString)));

}


void SocExplorerMainWindow::launchPluginManager()
{
    if(this->pluginManager->isHidden())
    {
        this->pluginManager->setHidden(false);
    }
}


void SocExplorerMainWindow::addPluginInterface(QDockWidget *plugin)
{
    plugin->setAllowedAreas(Qt::AllDockWidgetAreas);
    this->pluginsDockContainer->addDockWidget(Qt::TopDockWidgetArea,plugin);
    if(p_pluginGUIlist->count()!=0)
        this->pluginsDockContainer->tabifyDockWidget(p_pluginGUIlist->last(),plugin);
    p_pluginGUIlist->append(plugin);
}

void SocExplorerMainWindow::removePluginInterface(QDockWidget *plugin)
{
    p_pluginGUIlist->removeOne(plugin);
    this->pluginsDockContainer->removeDockWidget(plugin);
}


void SocExplorerMainWindow::clearMenu()
{
    this->menuBar()->clear();
    this->makeMenu();
}


void SocExplorerMainWindow::makeMenu()
{
    this->FileMenu = menuBar()->addMenu(tr("&File"));
    this->SessionsMenu = this->FileMenu->addMenu(tr("&Sessions"));
    this->loadSessions();
    this->FileMenu->addAction(this->sessionManagerAction);
    this->SettingsMenu = menuBar()->addMenu(tr("&Settings"));
    SocExplorerGUI::registerMenuBar(menuBar(),this->FileMenu,this->SettingsMenu);
    this->PluginsMenu = menuBar()->addMenu(tr("&Plugins"));
    this->ToolsMenu = menuBar()->addMenu(tr("&Tools"));
    this->ToolsMenu->addAction(this->exploreRegs);
    this->FileMenu->addAction(this->Quit);
    socexplorerproxy::self()->makeMenu(this->PluginsMenu);
    this->PluginsMenu->addAction(this->ManagePlugins);

    this->helpMenu = menuBar()->addMenu(tr("Help"));
    this->helpMenu->addAction(this->help);
    this->helpMenu->addAction(this->about);

}

void SocExplorerMainWindow::loadSessions()
{
    p_Sessions=this->p_SessionManagerDialog->getSessionsList();
    QAction* sact;
    QString stext;
    QList<QAction*> sessions=sessionsActions->actions();
    foreach (sact, sessions)
    {
          sessionsActions->removeAction(sact);
          SessionsMenu->removeAction(sact);
          delete sact;
    }
    foreach (stext, p_Sessions)
    {
        addSession(stext);
    }
}

void SocExplorerMainWindow::savePlugins()
{

}

void SocExplorerMainWindow::saveCurrentSession()
{
    if(p_currentSession.isEmpty())
    {
        SocExplorerSettings::loadSession("default");
    }
    SocExplorerSettings::setValue("GLOBAL","LastModified",QDate::currentDate().toString(),SocExplorerSettings::Session);
    SocExplorerSettings::setValue(this,"DOCK_LOCATIONS",this->saveState(0),SocExplorerSettings::Session);
    SocExplorerSettings::setValue(this,"PLUGINS_DOCK_LOCATIONS",this->pluginsDockContainer->saveState(0),SocExplorerSettings::Session);
    SocExplorerSettings::setValue(this,"MAIN_WINDOW_GEOMETRY",this->saveGeometry(),SocExplorerSettings::Session);
    QStringList plugins = socexplorerproxy::getPluginsList();
    SocExplorerSettings::setValue(this,"LOADED_PLUGINS",QVariant(plugins),SocExplorerSettings::Session);
    SocExplorerSettings::sync();
}

void SocExplorerMainWindow::loadCurrentSession()
{

    QStringList plugins = SocExplorerSettings::value(this,"LOADED_PLUGINS",QVariant(),SocExplorerSettings::Session).toStringList();
    socexplorerproxy::loadPluginsList(plugins);
    this->restoreGeometry(SocExplorerSettings::value(this,"MAIN_WINDOW_GEOMETRY",QVariant(),SocExplorerSettings::Session).toByteArray());
    this->restoreState(SocExplorerSettings::value(this,"DOCK_LOCATIONS",QVariant(),SocExplorerSettings::Session).toByteArray());
    this->pluginsDockContainer->restoreState(SocExplorerSettings::value(this,"PLUGINS_DOCK_LOCATIONS",QVariant(),SocExplorerSettings::Session).toByteArray());
}


SocExplorerMainWindow::~SocExplorerMainWindow()
{

}


void SocExplorerMainWindow::setLangage(QAction *action)
{
    QString local = action->data().toString();
    QString qmPath = QDir(QString("translations")).absolutePath();
    appTranslator->load(qmPath+"/socexplorer_"+local+".qm");
    qApp->installTranslator(appTranslator);
    emit this->translateSig();
}


void SocExplorerMainWindow::createLangMenu()
{
    this->langMenu = menuBar()->addMenu(tr("&Langue"));
    this->langActionGrp = new QActionGroup(this);
    connect(this->langActionGrp,SIGNAL(triggered(QAction*)),this,SLOT(setLangage(QAction*)));
    QDir* qmDir = new QDir(QString("translations"));
    QStringList LangFiles = qmDir->entryList(QStringList("socexplorer_*.qm"));
    for(int i=0;i<LangFiles.size();++i)
    {
        QString Local = LangFiles[i];
        Local.remove(0,Local.indexOf('_')+1);
        Local.chop(3);
        QTranslator translator;
        translator.load(LangFiles[i],qmDir->absolutePath());
        QString langage = translator.translate("MainWindow","English");
        QAction *action = new QAction(tr("&%1 %2").arg(i+1).arg(langage),this);
        action->setCheckable(true);
        action->setData(Local);
        langMenu->addAction(action);
        langActionGrp->addAction(action);
        if(langage==tr("English"))
            action->setChecked(true);
    }
}


void SocExplorerMainWindow::updateText()
{
    emit this->translateSig();
}



void SocExplorerMainWindow::showAboutBox()
{
    p_about->show();
}

void SocExplorerMainWindow::pluginselected(const QString &instanceName)
{
    socexplorerplugin* drv=socexplorerproxy::self()->getSysDriver(instanceName);
    if(drv)
        drv->raise();
}

void SocExplorerMainWindow::setActiveSession(const QString &session)
{
    if(!(p_currentSession.isNull() && session=="default"))
        saveCurrentSession();
    socexplorerproxy::self()->close();
    this->p_currentSession = session;
    SocExplorerSettings::loadSession(session);
    loadCurrentSession();

}

void SocExplorerMainWindow::setActiveSession(QAction *session)
{
    this->setActiveSession(session->text());
}

void SocExplorerMainWindow::showSessionManager(bool)
{
    this->p_SessionManagerDialog->show();
}

void SocExplorerMainWindow::renameSession(const QString &oldName, const QString &newName)
{
    for(int i=0;i<sessionsActions->actions().count();i++)
    {
        if(Q_UNLIKELY(sessionsActions->actions().at(i)->text()==oldName))
        {
            sessionsActions->actions().at(i)->setText(newName);
        }
    }
}

void SocExplorerMainWindow::addSession(const QString &newSession)
{
    QAction* sact = new QAction(newSession,this);
    sact->setCheckable(true);
    sact->setData(newSession);
    if(Q_UNLIKELY(p_currentSession==newSession))
       sact->setChecked(true);
    sessionsActions->addAction(sact);
    SessionsMenu->addAction(sact);
}

void SocExplorerMainWindow::removeSession(const QString &session)
{
    QAction* sact;
    foreach (sact, sessionsActions->actions())
    {
        if(Q_UNLIKELY(sact->text()==session))
        {
            sessionsActions->removeAction(sact);
            SessionsMenu->removeAction(sact);
            delete sact;
        }
    }
}




void SocExplorerMainWindow::closeEvent(QCloseEvent *event)
{
    saveCurrentSession();
    socexplorerproxy::self()->close();
    qApp->closeAllWindows();
    event->accept();
}






