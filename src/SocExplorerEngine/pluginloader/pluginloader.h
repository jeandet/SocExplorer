/*------------------------------------------------------------------------------
--  This file is a part of the SocExplorer Software
--  Copyright (C) 2011, Plasma Physics Laboratory - CNRS
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
#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QString>
#include <QLibrary>
#include <QObject>
#include <QStringList>
#include <socexplorerplugininterface.h>
#include <socexplorerplugin.h>
#include <pluginscache.h>
/*
  Debug
*/
#include <stdio.h>

#define loadlib(filename)   QLibrary* lib = new QLibrary((fileName));\
                            lib->load();\
                            if(!lib->isLoaded())\
                            {\
                                return 0;\
                            }


class pluginloader
{

private:
    static pluginloader* _self;
    static PluginsCache* _cache;
    static QStringList* _folderList;
    ~pluginloader();
    pluginloader();
public:

    static void init();
    static pluginloader* self();

    static int checklibrary(const QString fileName);
    static bool isvalid(QString Name);
    static socexplorerplugin* newsocexplorerplugin(const QString Name);
    static QString getlibVersion(const QString Name);
    static QString getlibPIDstr(const QString Name);
    static QString getlibVIDstr(const QString Name);
    static int getlibPID(const QString Name);
    static int getlibVID(const QString Name);
    static int libcanbechild(const QString Name);
    static int libcanberoot(const QString Name);
    static QString getlibAuthor(const QString Name);
    static QString getlibName(const QString Name);
    static QString getlibDescription(const QString Name);
    static QString getlibDir(const QString Name);
    static QString findlib(QString name);
    static QList<PluginsCacheItem *> listAvailiables(bool rescan=true);
    static void scanFolders();
    static void showCache(){_cache->show();}
private:
    int p_checklibraryQlib(const QString fileName);
    int p_checklibraryCustom(const QString fileName);
    socexplorerplugin* p_newsocexplorerpluginQlib(const QString Name);
    socexplorerplugin* p_newsocexplorerpluginCustom(const QString Name);
    static QString* _cacheLookup(const QString Name);
    static QString _getlibName(const QString fileName);
    static int _getlibPID(const QString fileName);
    static int _getlibVID(const QString fileName);
};
#endif
