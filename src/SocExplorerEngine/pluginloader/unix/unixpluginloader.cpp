#include "unixpluginloader.h"
#include <stdio.h>
#include <QDebug>
#include <socexplorerengine.h>

unixPluginLoader::unixPluginLoader(const QString &libPath)
{
    SocExplorerEngine::message("unixPluginLoader::unixPluginLoader","try to open "+libPath,3);
    dlHandle = dlopen(libPath.toStdString().c_str(),RTLD_LAZY|RTLD_GLOBAL);
    if(dlHandle==NULL)
        SocExplorerEngine::message("unixPluginLoader::unixPluginLoader",QString("Failed ") + dlerror(),3);
    else
        SocExplorerEngine::message("unixPluginLoader::unixPluginLoader","Success " ,3);
    this->libPath = libPath;
}

void *unixPluginLoader::resolve(const QString &symbol)
{
    if(dlHandle!=NULL)
    {
        SocExplorerEngine::message("unixPluginLoader::resolve","try to resolve "+symbol+" in "+libPath ,3);
        void* sym =  dlsym (dlHandle, symbol.toStdString().c_str());
        if(sym==NULL)
            SocExplorerEngine::message("unixPluginLoader::resolve",QString("Failed")+ dlerror() ,3);
        else
            SocExplorerEngine::message("unixPluginLoader::resolve","Success",3);
        return sym;
    }
    return NULL;
}

void unixPluginLoader::close()
{
    if(dlHandle!=NULL)
    {
        dlclose(dlHandle);
        SocExplorerEngine::message("unixPluginLoader::close",dlerror(),3);
    }
}
