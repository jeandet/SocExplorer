/*------------------------------------------------------------------------------
--  This file is a part of the SocExplorer Software
--  Copyright (C) 2012, Plasma Physics Laboratory - CNRS
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
#ifndef SOCEXPLORERENGINE_H
#define SOCEXPLORERENGINE_H

#include <stdint.h>
#include <QObject>
#include <QtWidgets>
#include <QStringList>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QApplication>
#include <QtCore/qglobal.h>
#include <QFileDialog>
#include <socexplorerxmlfile.h>
#include <socexplorerenumdevice.h>
#include <XMLmodel.h>
#include <peripheralwidget.h>
#include <registerwidget.h>
#include <socmodel.h>
#include <memtester.h>
#include <QSettings>

#if defined(SOCEXPLORER_SDK_BUILD)
#  define SOCEXPLORER_SDK_EXPORT Q_DECL_EXPORT
#else
#  define SOCEXPLORER_SDK_EXPORT Q_DECL_IMPORT
#endif


class SOCEXPLORER_SDK_EXPORT SocExplorerAutoProgressBar
{
public:
    SocExplorerAutoProgressBar(QProgressBar* progressBar=NULL)
    {
        this->p_progressbar=progressBar;
    }
     ~SocExplorerAutoProgressBar()
    {
        if(p_progressbar)
        {
            delete p_progressbar;
        }
    }
    void setProgressBar(QProgressBar* progressBar)
    {
        this->p_progressbar=progressBar;
    }
    void setValue(int value)
    {
        p_progressbar->setValue(value);
    }
private:
    QProgressBar* p_progressbar;
};

//! SocExplorerEngine is a pure static class which aims to provide services for both SocExplorer software and plugins.

class SOCEXPLORER_SDK_EXPORT SocExplorerEngine : public QObject
{
    Q_OBJECT
private:
    static SocExplorerEngine* _self;
    static QSettings* m_settings;
    SocExplorerEngine(QObject *parent = 0);
    static void init();

public:
    static SocExplorerEngine* self(){ if(!_self){_self= new SocExplorerEngine;}return _self;}
    //! Return the configuration folder path, OS dependant.
    SOCEXPLORER_SDK_EXPORT static QString configFolder();
    //! Return the default plugin folder path, OS dependant.
     static QStringList pluginFolders();
     static QString configPath(){return QString(SOCEXPLORER_CONFIG_PATH);}
     static QString sharePath(){return QString(SOCEXPLORER_SHARE_PATH);}
     static int addEnumDevice(socexplorerplugin* rootPlugin,int VID,int PID,qint32 baseAddress,const QString& name);
     static QList<SOCModel*>* getSOCs();
     static QString getDevName(int VID, int PID);
     static QString SocExplorerVersion();
     static QString SocExplorerChangeset();
     static QString SocExplorerBranch();
     static socExplorerXmlModel* xmlModel();
     static void setMainWindow(QMainWindow* Mainwindow);
     static QProgressBar* getProgressBar(const QString &format, int max);
     static void deleteProgressBar(QProgressBar* progressBar);
     static void addSOC(socexplorerplugin* rootPlugin);
     static void removeSOC(socexplorerplugin* rootPlugin);
     static void message(socexplorerplugin* sender,const QString& message,int debugLevel=0);
     static void message(QObject* sender,const QString& message,int debugLevel=0);
     static void message(const QString& sender,const QString& message,int debugLevel=0);
     static void setLogLevel(int level);
     static bool isSocLitleEndian(socexplorerplugin* plugin);
signals:
     void enumDeviceAdded(socExplorerEnumDevice* device);
public slots:
     QString getSocExplorerVersion(){return SocExplorerEngine::SocExplorerVersion();}
     QString getSocExplorerChangeset(){return SocExplorerEngine::SocExplorerChangeset();}
     QString getSocExplorerBranch(){return SocExplorerEngine::SocExplorerBranch();}
     qint32 getEnumDeviceBaseAddress(const QString& rootPlugin,int VID,int PID,int count=0);
     qint32 getEnumDeviceBaseAddress(socexplorerplugin* plugin,int VID,int PID,int count=0);
     qint32 getEnumDeviceCount(socexplorerplugin* plugin,int VID,int PID);
     qint32 getEnumDeviceCount(const QString& rootPlugin,int VID,int PID);
     int addEnumDevice(const QString& rootPlugin,int VID,int PID,qint32 baseAddress,const QString& name);
     unsigned int memMeasureSize(socexplorerplugin* plugin, unsigned int address,unsigned int maxSize=0xFFFFFFFF);
     unsigned int memMeasureSize(const QString& plugin, unsigned int address,unsigned int maxSize=0xFFFFFFFF);

private:
    static SOCModel* plugin2Soc(socexplorerplugin* plugin);
    static socExplorerXmlModel* p_xmlmodel;
    static QMainWindow* mainWindow;
    static QList<SOCModel*>* SOCs;
    static int loglvl;
};

#endif // SOCEXPLORERENGINE_H







