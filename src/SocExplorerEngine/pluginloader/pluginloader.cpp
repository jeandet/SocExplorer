#include "pluginloader.h"
#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QLabel>
#include <QSettings>
#include <QApplication>
#include <QCoreApplication>
#include <socexplorerengine.h>
#ifdef SOCEXPLORER_CUSTOM_PLUGIN_LOADER
#include "unix/unixpluginloader.h"
#endif
#include <socexplorerengine.h>
#include <socexplorersettings.h>

pluginloader* pluginloader::_self = NULL;
PluginsCache* pluginloader::_cache = NULL;
QStringList* pluginloader::_folderList = NULL;


pluginloader::pluginloader()
{
    _cache = new PluginsCache();
    _folderList = new QStringList();
    _folderList->append(SocExplorerEngine::pluginFolders());
    scanFolders();
}

void pluginloader::scanFolders()
{
    QDir dir;
    QStringList filters;
    filters <<"*.so"<< "*.dll";
    _cache->flush();
    for(int d=0;d<_folderList->count();d++)
    {
        dir.setPath(_folderList->at(d));
        dir.setFilter(QDir::Files);
        dir.setNameFilters(filters);
        QFileInfoList list = dir.entryInfoList();
        for (int i = 0; i < list.size(); ++i)
        {
            QFileInfo fileInfo = list.at(i);
            SocExplorerEngine::message("pluginloader::scanFolders","Checking "+ fileInfo.filePath(),3);
            if(checklibrary(fileInfo.filePath())!=0)
            {
                _cache->append(fileInfo.fileName(),fileInfo.path(),_getlibName(fileInfo.filePath()),_getlibPID(fileInfo.filePath()),_getlibPID(fileInfo.filePath()));
            }
        }
    }
}

int pluginloader::p_checklibraryQlib(const QString fileName)
{
    QLibrary* lib = new QLibrary;
    lib->setFileName(fileName);
    lib->setLoadHints(QLibrary::PreventUnloadHint);
    lib->load();
    if(!lib->isLoaded())
    {
        SocExplorerEngine::message("pluginloader::p_checklibraryQlib",lib->errorString(),3);
        lib->~QLibrary();
        lib = new QLibrary(fileName);
        lib->load();
    }
    delete lib;
    if(QLibrary::resolve(fileName,"socexplorerpluginCreateObject"))
    {
        if(QLibrary::resolve(fileName,"socexplorerpluginpid"))
        {
            if(QLibrary::resolve(fileName,"socexplorerpluginvid"))
            {
                if(QLibrary::resolve(fileName,"socexplorerpluginVersion"))
                {
                    if(QLibrary::resolve(fileName,"socexplorerpluginAuthor"))
                    {
                        if(QLibrary::resolve(fileName,"socexplorerpluginDescription"))
                        {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int pluginloader::p_checklibraryCustom(const QString fileName)
{
#ifdef SOCEXPLORER_CUSTOM_PLUGIN_LOADER
    unixPluginLoader lib(fileName);
    if(NULL!=lib.resolve("socexplorerpluginCreateObject"))
    {
        if(NULL!=lib.resolve("socexplorerpluginpid"))
        {
            if(NULL!=lib.resolve("socexplorerpluginvid"))
            {
                if(NULL!=lib.resolve("socexplorerpluginVersion"))
                {
                    if(NULL!=lib.resolve("socexplorerpluginAuthor"))
                    {
                        if(NULL!=lib.resolve("socexplorerpluginDescription"))
                        {
                            return 1;
                        }
                    }
                }
            }
        }
    }
#endif
    return 0;
}

socexplorerplugin *pluginloader::p_newsocexplorerpluginQlib(const QString Name)
{
    QString* libfile= _cacheLookup(Name);
    if(libfile==NULL)return NULL;
    QLibrary* lib = new QLibrary(*libfile);
    delete libfile;
    socexplorerpluginCreateObjectT newDrvr = NULL;
    newDrvr=(socexplorerpluginCreateObjectT)lib->resolve("socexplorerpluginCreateObject");
    if(newDrvr==NULL)
    {
        return NULL;
    }
    return (socexplorerplugin*) newDrvr();
}

socexplorerplugin *pluginloader::p_newsocexplorerpluginCustom(const QString Name)
{
#ifdef SOCEXPLORER_CUSTOM_PLUGIN_LOADER
    QString* libfile= _cacheLookup(Name);
    if(libfile==NULL)return NULL;
    unixPluginLoader lib(*libfile);
    delete libfile;
    socexplorerpluginCreateObjectT newDrvr = NULL;
    newDrvr=(socexplorerpluginCreateObjectT)lib.resolve("socexplorerpluginCreateObject");
    if(newDrvr==NULL)
    {
        return NULL;
    }
    return (socexplorerplugin*) newDrvr();
#endif
}

QList<PluginsCacheItem*> pluginloader::listAvailiables(bool rescan)
{
    if(_self==NULL)
    {
        init();
        return _cache->listDrivers();
    }
    if(rescan)
    {
        scanFolders();
    }

    return _cache->listDrivers();
}


void pluginloader::init()
{
    if(_self==NULL)
    {
        _self=new pluginloader();
    }
}


pluginloader* pluginloader::self()
{
    if(_self==NULL)
    {
        init();
    }
    return _self;
}

bool pluginloader::isvalid(QString Name)
{
    if(_self==NULL)init();
    QString* libfile= _cacheLookup(Name);
    if(libfile==NULL)return false;
    else
    {
        delete libfile;
        return true;
    }

}

int pluginloader::checklibrary(const QString fileName)
{
#ifdef SOCEXPLORER_CUSTOM_PLUGIN_LOADER
    return _self->p_checklibraryCustom(fileName);
#else
    return _self->p_checklibraryQlib(fileName);
#endif
}





socexplorerplugin* pluginloader::newsocexplorerplugin(const QString Name)
{
#ifdef SOCEXPLORER_CUSTOM_PLUGIN_LOADER
    return _self->p_newsocexplorerpluginCustom(Name);
#else
    return _self->p_newsocexplorerpluginQlib(Name);
#endif
}

QString pluginloader::getlibVersion(const QString Name)
{
    if(_self==NULL)init();
    QString* libfile= _cacheLookup(Name);
    if(libfile==NULL)return NULL;
    QLibrary* lib = new QLibrary(*libfile);
    delete libfile;
    lib->load();
    if(lib->isLoaded())
    {
        socexplorerpluginVersionT pluginversion = (socexplorerpluginVersionT)lib->resolve("socexplorerpluginVersion");
        if(pluginversion!=NULL)
        {
            QString version = pluginversion();
            ////lib->unload();
            lib->~QLibrary();
            return version;
        }
    }
    lib->~QLibrary();
    return QObject::tr("Can't load Plugin.");
}



QString pluginloader::getlibPIDstr(const QString Name)
{
    return QString("0x" + QString::number(pluginloader::getlibPID(Name) , 16));
}

QString pluginloader::getlibVIDstr(const QString Name)
{
    return QString("0x" + QString::number(pluginloader::getlibVID(Name) , 16));
}



int pluginloader::libcanbechild(const QString Name)
{
    if(_self==NULL)init();
    QString* libfile= _cacheLookup(Name);
    if(libfile==NULL)return (int)NULL;
    QLibrary* lib = new QLibrary(*libfile);
    delete libfile;
    lib->load();
    if(lib->isLoaded())
    {
        socexplorerplugincanbechildT canbechild = (socexplorerplugincanbechildT)lib->resolve("socexplorerplugincanbechild");
        if(canbechild!=NULL)
        {
            int value = canbechild();
            ////lib->unload();
            //lib->~QLibrary();
            return value;
        }
    }
    //lib->~QLibrary();
    return 0;
}




int pluginloader::libcanberoot(const QString Name)
{
    if(_self==NULL)init();
    QString* libfile= _cacheLookup(Name);
    if(libfile==NULL)return (int)NULL;
    QLibrary* lib = new QLibrary(*libfile);
    delete libfile;
    lib->load();
    if(lib->isLoaded())
    {
        socexplorerplugincanberootT canberoot = (socexplorerplugincanberootT)lib->resolve("socexplorerplugincanberoot");
        if(canberoot!=NULL)
        {
            int value = canberoot();
            ////lib->unload();
            //lib->~QLibrary();
            return value;
        }
    }
    delete lib;
    //lib->~QLibrary();
    return 0;
}

int pluginloader::getlibVID(const QString Name)
{
    if(_self==NULL)init();
    QString* libfile= _cacheLookup(Name);
    if(libfile==NULL)return 0;
    QString file(*libfile);
    delete libfile;
    return _getlibVID(file);
}


int pluginloader::getlibPID(const QString Name)
{
    if(_self==NULL)init();
    QString* libfile= _cacheLookup(Name);
    if(libfile==NULL)return 0;
    QString file(*libfile);
    delete libfile;
    return _getlibPID(file);
}

QString pluginloader::getlibAuthor(const QString Name)
{
    if(_self==NULL)init();
    QString* libfile= _cacheLookup(Name);
    if(libfile==NULL)return NULL;
    QLibrary* lib = new QLibrary(*libfile);
    delete libfile;
    lib->load();
    if(lib->isLoaded())
    {
        socexplorerpluginAuthorT pluginauthor = (socexplorerpluginAuthorT)lib->resolve("socexplorerpluginAuthor");
        if(pluginauthor!=NULL)
        {
            QString author = pluginauthor();
            ////lib->unload();
            lib->~QLibrary();
            return author;
        }
    }
    lib->~QLibrary();
    return QObject::tr("Can't load Plugin.");
}

QString pluginloader::getlibName(const QString Name)
{
    if(_self==NULL)init();
    QString* libfile= _cacheLookup(Name);
    if(libfile==NULL)return QString("");
    QString file(*libfile);
    delete libfile;
    return _getlibName(file);
}

QString pluginloader::getlibDescription(const QString Name)
{
    if(_self==NULL)init();
    QString* libfile= _cacheLookup(Name);
    if(libfile==NULL)return NULL;
    QLibrary* lib = new QLibrary(*libfile);
    delete libfile;
    lib->load();
    if(lib->isLoaded())
    {
        socexplorerpluginDescriptionT plugindescription = (socexplorerpluginDescriptionT)lib->resolve("socexplorerpluginDescription");
        if(plugindescription!=NULL)
        {
            QString description = plugindescription();
            ////lib->unload();
            lib->~QLibrary();
            return description;
        }
    }
    lib->~QLibrary();
    return QObject::tr("Can't load Plugin.");
}

QString pluginloader::getlibDir(const QString Name)
{
    if(_self==NULL)init();
    return *_cacheLookup(Name);
}

QString pluginloader::_getlibName(const QString fileName)
{
    QLibrary* lib = new QLibrary(fileName);
    lib->load();
    if(lib->isLoaded())
    {
        socexplorerpluginNameT pluginName = (socexplorerpluginAuthorT)lib->resolve("socexplorerpluginName");
        if(pluginName!=NULL)
        {
            QString name = pluginName();
            //lib->unload();
            lib->~QLibrary();
            return name;
        }
    }
    lib->~QLibrary();
    return QObject::tr("Can't load Plugin.");
}

int pluginloader::_getlibPID(const QString fileName)
{
    QLibrary* lib = new QLibrary(fileName);
    lib->load();
    if(lib->isLoaded())
    {
        socexplorerpluginpidT pluginpid = (socexplorerpluginpidT)lib->resolve("socexplorerpluginpid");
        if(pluginpid!=NULL)
        {
            int pid = pluginpid();
            //lib->unload();
            lib->~QLibrary();
            return pid;
        }
    }
    lib->~QLibrary();
    return 0;
}

int pluginloader::_getlibVID(const QString fileName)
{
    QLibrary* lib = new QLibrary(fileName);
    lib->load();
    if(lib->isLoaded())
    {
        socexplorerpluginvidT pluginvid = (socexplorerpluginvidT)lib->resolve("socexplorerpluginvid");
        if(pluginvid!=NULL)
        {
            int vid = pluginvid();
            //lib->unload();
            lib->~QLibrary();
            return vid;
        }
    }
    lib->~QLibrary();
    return 0;
}

QString* pluginloader::_cacheLookup(const QString Name)
{
    QString* libfile= new QString(_cache->first(Name));
    if(!QFile::exists(*libfile))
    {
        scanFolders();
        *libfile = _cache->first(Name);
        if(QFile::exists(*libfile))return libfile;
    }
    else
    {
        return libfile;
    }
    delete libfile;
    return NULL;
}

/*QString findlib(QString name)
{

}*/

