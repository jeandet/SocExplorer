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
#ifndef SOC_H
#define SOC_H
#include <isocexplorerplugin.h>
#include <memory>
#include <vector>
#include "pluginloaderV2/pluginmanager.h"
#include <SocExplorerCore.h>
#include <QHash>
#include <QObject>

class SOC
{
    QHash<QString,std::shared_ptr<ISocexplorerPlugin>> plugins;
    QHash<QString,std::shared_ptr<ISocexplorerPlugin>> root_plugins;

    QHash<int,std::function<void(const QString&,const QString&)>> plugin_update_callbacks;

    QString name;
    PluginManager<SocExplorerCore> plugin_loader;

    SOC(const QString& name)
        :name(name),plugin_loader({})
    {

    }

    QString _makeInstanceName(const QString& baseName)const
    {
        int i=0;
        while(this->plugins.keys().contains(baseName+QString::number(i)))
            i+=1;
        return baseName+QString::number(i);
    }

    bool _loadPlugin(const QString& name, const QString& instanceName)
    {
        auto plugin = plugin_loader.makeInstance(name);
        if(plugin!=Q_NULLPTR)
        {
            plugin->setInstanceName(instanceName);
            this->root_plugins[instanceName] = plugin;
            this->plugins[instanceName] = plugin;
            for(const auto& callback:plugin_update_callbacks)
            {
                callback(instanceName,"");
            }
            return true;
        }
        return false;
    }

    bool _loadChildPlugin(const QString& name, const QString& parentInstanceName, const QString& instanceName)
    {
        if(this->plugins.contains(parentInstanceName))
        {
            auto plugin = plugin_loader.makeInstance(name);
            if(plugin!=Q_NULLPTR)
            {
                plugin->setInstanceName(instanceName);
                auto parent = this->plugins[parentInstanceName];
                parent->appendChildPlugin(plugin);
                this->plugins[instanceName] = plugin;
                for(const auto& callback:plugin_update_callbacks)
                {
                    callback(instanceName, parentInstanceName);
                }
                return true;
            }
        }
        return false;
    }

public:

    static SOC& instance()
    {
        static SOC inst("SOC");
        return inst;
    }

    static bool loadPlugin(const QString& name)
    {
        auto& soc = SOC::instance();
        return soc._loadPlugin(name,soc._makeInstanceName(name));
    }

    static bool loadPlugin(const QString& name, const QString& instanceName)
    {
        return  SOC::instance()._loadPlugin(name,instanceName);
    }

    static bool loadChildPlugin(const QString& name, const QString& parentInstanceName)
    {
        auto& soc = SOC::instance();
        return  soc._loadChildPlugin(name,parentInstanceName,soc._makeInstanceName(name));
    }

    static bool loadChildPlugin(const QString& name, const QString& parentInstanceName, const QString& instanceName)
    {
        return  SOC::instance()._loadChildPlugin(name,parentInstanceName,instanceName);
    }

    static std::shared_ptr<ISocexplorerPlugin> getPlugin(const QString &instanceName)
    {
        return SOC::instance().plugins.value(instanceName);
    }

    static void addPluginLookupPath(const QString& path)
    {
        return SOC::instance().plugin_loader.addPluginLookupPath(path, true);
    }

    static int registerPluginUpdateCallback(std::function<void(const QString&,const QString&)> callback)
    {
        static int i=0;
        i+=1;
        SOC::instance().plugin_update_callbacks[i]=callback;
        return i;
    }

    static void unregisterPluginUpdateCallback(int token)
    {
        SOC::instance().plugin_update_callbacks.remove(token);
    }
};
#endif //SOC_H
