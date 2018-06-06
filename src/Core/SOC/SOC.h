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
#include <functional>
#include "pluginloaderV2/pluginmanager.h"
#include <SocExplorerCore.h>
#include <QHash>
#include <QObject>
#include "socperipheral.h"

class SOC
{
    QHash<QString,address64_t> _symbols_LUT;

    QHash<QString,std::shared_ptr<ISocexplorerPlugin>> _plugins;
    QHash<QString,std::shared_ptr<ISocexplorerPlugin>> _root_plugins;

    QHash<int,std::function<void(const QHash<QString,std::shared_ptr<ISocexplorerPlugin>>&)>> plugin_update_callbacks;

    QString _name;


    QHash<QString, SOCPeripheral> _peripherals;

    SOC(const QString& name)
        :_name(name)
    {

    }

    QString _makeInstanceName(const QString& baseName)const
    {
        int i=0;
        while(this->_plugins.keys().contains(baseName+QString::number(i)))
            i+=1;
        return baseName+QString::number(i);
    }

    void _notifyPluginChange()
    {
        for(const auto& callback:plugin_update_callbacks)
        {
            callback(this->_root_plugins);
        }
    }

    bool _unloadPlugin(const std::shared_ptr<ISocexplorerPlugin>& plugin)
    {
        for(auto& child:plugin->children())
        {
            _unloadPlugin(child);
        }
        this->_root_plugins.remove(plugin->instanceName());
        this->_plugins.remove(plugin->instanceName());
        return true;
    }

    bool _unloadPlugin(const QString& instanceName)
    {
        auto plugin = this->_plugins[instanceName];
        if(plugin==Q_NULLPTR)
            return false;
        auto success = _unloadPlugin(plugin);
        _notifyPluginChange();
        return success;
    }

    bool _loadPlugin(const QString& name, const QString& instanceName)
    {
        auto plugin = SocExplorerCore::makeInstance(name);
        if(plugin!=Q_NULLPTR)
        {
            plugin->setInstanceName(instanceName);
            this->_root_plugins[instanceName] = plugin;
            this->_plugins[instanceName] = plugin;
            _notifyPluginChange();
            return true;
        }
        return false;
    }

    bool _loadChildPlugin(const QString& name, const QString& parentInstanceName, const QString& instanceName)
    {
        if(this->_plugins.contains(parentInstanceName))
        {
            auto plugin = SocExplorerCore::makeInstance(name);
            if(plugin!=Q_NULLPTR)
            {
                plugin->setInstanceName(instanceName);
                auto parent = this->_plugins[parentInstanceName];
                parent->appendChildPlugin(plugin);
                this->_plugins[instanceName] = plugin;
                _notifyPluginChange();
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

    static bool unloadPlugin(const QString& instanceName)
    {
        return  SOC::instance()._unloadPlugin(instanceName);
    }

    static void unloadAllPlugins()
    {
        SOC::instance()._root_plugins.clear();
        SOC::instance()._plugins.clear();
        SOC::instance()._notifyPluginChange();
    }

    static std::shared_ptr<ISocexplorerPlugin> getPlugin(const QString &instanceName)
    {
        return SOC::instance()._plugins.value(instanceName);
    }

    static const QHash<QString,std::shared_ptr<ISocexplorerPlugin>>& plugins()
    {
        return SOC::instance()._plugins;
    }

    static int registerPluginUpdateCallback(std::function<void(const QHash<QString,std::shared_ptr<ISocexplorerPlugin>>&)> callback)
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

    static void addSymbol(const QString& symboleName, address64_t address)
    {
        SOC::instance()._symbols_LUT[symboleName] = address;
    }

    static void addPeripheral(SOCPeripheral periph)
    {
        SOC::instance()._peripherals[periph.name()] = periph;
    }

    static const QHash<QString, SOCPeripheral>& peripherals()
    {
        return SOC::instance()._peripherals;
    }
};
#endif //SOC_H
