/*------------------------------------------------------------------------------
███████╗ ██████╗  ██████╗    ███████╗██╗  ██╗██████╗ ██╗      ██████╗ ██████╗ ███████╗██████╗
██╔════╝██╔═══██╗██╔════╝    ██╔════╝╚██╗██╔╝██╔══██╗██║     ██╔═══██╗██╔══██╗██╔════╝██╔══██╗
███████╗██║   ██║██║         █████╗   ╚███╔╝ ██████╔╝██║     ██║   ██║██████╔╝█████╗  ██████╔╝
╚════██║██║   ██║██║         ██╔══╝   ██╔██╗ ██╔═══╝ ██║     ██║   ██║██╔══██╗██╔══╝  ██╔══██╗
███████║╚██████╔╝╚██████╗    ███████╗██╔╝ ██╗██║     ███████╗╚██████╔╝██║  ██║███████╗██║  ██║
╚══════╝ ╚═════╝  ╚═════╝    ╚══════╝╚═╝  ╚═╝╚═╝     ╚══════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝

--  This file is a part of the SOC Explorer Software
--  Copyright (C) 2018, Plasma Physics Laboratory - CNRS
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
#include <isocexplorerplugin.h>
#include <QHash>
#include <QStringLiteral>

template<typename T>
auto loadMetadata(const QString& name, const QString &pluginName,const QHash<QString, QHash<QString,QString>>& plugins_metada_cache);

template<>
auto loadMetadata<QString>(const QString& name, const QString &pluginName,const QHash<QString, QHash<QString,QString>>& plugins_metada_cache)
{
    if(plugins_metada_cache.contains(pluginName))
    {
        return plugins_metada_cache[pluginName][name];
    }
    return QString();
}

template<>
auto loadMetadata<int>(const QString& name, const QString &pluginName,const QHash<QString, QHash<QString,QString>>& plugins_metada_cache)
{
    if(plugins_metada_cache.contains(pluginName))
    {
        return plugins_metada_cache[pluginName][name].toInt();
    }
    return -1;
}

template<>
auto loadMetadata<bool>(const QString& name, const QString &pluginName,const QHash<QString, QHash<QString,QString>>& plugins_metada_cache)
{
    if(plugins_metada_cache.contains(pluginName))
    {
        return bool(plugins_metada_cache[pluginName][name].toInt());
    }
    return false;
}

namespace pluginKeys
{
    const auto Name=QStringLiteral("Name");
    const auto Author=QStringLiteral("Author");
    const auto Version=QStringLiteral("Version");
    const auto Description=QStringLiteral("Description");
    const auto CanBeRoot=QStringLiteral("root");
    const auto CanBeChild=QStringLiteral("child");
    const auto VID=QStringLiteral("VID");
    const auto PID=QStringLiteral("PID");

    const auto all={Name, Author, Version, Description, CanBeRoot, CanBeChild, VID, PID};
};

class PluginManager
{
    void loadPluginMetaData(const QString &pluginPath);
    bool isPlugin(const QString &pluginPath)const;

    QString resolvePluginName(const QString &pluginName)const
    {
        if( plugins_metada_cache.contains(pluginName))
            return pluginName;
        else if (pluginTable.contains(pluginName)) {
            return pluginTable[pluginName].last();
        }
        return QString();
    }
public:
    PluginManager(const QStringList& folderList);
    ISocexplorerPlugin* makeInstance(const QString &pluginName);

#define METADATA_GETTER(type, name)\
    type plugin##name(const QString &pluginName)const{return loadMetadata<type>(pluginKeys::name, resolvePluginName(pluginName), plugins_metada_cache);}

    METADATA_GETTER(QString, Name)
    METADATA_GETTER(QString, Author)
    METADATA_GETTER(QString, Description)
    METADATA_GETTER(QString, Version)

    METADATA_GETTER(bool, CanBeRoot)
    METADATA_GETTER(bool, CanBeChild)

    METADATA_GETTER(int, VID)
    METADATA_GETTER(int, PID)

    void scanFolders();
private:
    QHash<QString, QHash<QString,QString>> plugins_metada_cache;
    QHash<QString, QStringList> pluginTable;

    QStringList folderList;
};

#endif // PLUGINLOADER_H
