#include "pluginscache.h"
#include <QDebug>
#include <socexplorerengine.h>

PluginsCacheItem::PluginsCacheItem(const QString &fileName, const QString &path, const QString &pluginName, int VID, int PID)
{
    this->fileName = new QString(fileName);
    this->path = new QString(path);
    this->pluginName = new QString(pluginName);
    this->VID = VID;
    this->PID = PID;
}

bool PluginsCacheItem::comparePluginName(const QString& pluginName)
{
    return (!this->pluginName->compare(pluginName));
}

bool PluginsCacheItem::comparefileName(const QString &fileName)
{
    bool same=false;
    same|=!(this->fileName->compare(fileName));
    same|=!(this->fileName->compare(fileName+".so"));
    same|=!(this->fileName->compare("lib"+fileName+".so"));
    return same;
}

bool PluginsCacheItem::comparefilePath(const QString &filePath)
{
    return (!QString::compare(*this->path +"/"+*this->fileName,filePath));
}

bool PluginsCacheItem::compareIDs(int VID,int PID)
{
    return (VID==this->VID && PID==this->PID);
}

bool PluginsCacheItem::compare(PluginsCacheItem *item)
{
    return comparefilePath(item->getpath()+"/"+item->getfileName());
}



PluginsCache::PluginsCache(QObject* parent)
    :QObject(parent)
{
    this->items = new QList<PluginsCacheItem*>;
    this->__view =NULL;
}


void PluginsCache::append(const QString &fileName, const QString &path, const QString &pluginName, int VID, int PID)
{
    PluginsCacheItem* item=new PluginsCacheItem(fileName,path,pluginName,  VID,  PID);
    for(int i=0;i<this->items->count();i++)
    {
        if(this->items->at(i)->compare(item))
        {
            SocExplorerEngine::message("PluginsCache",fileName + " already in the cache",3);
            delete item;
            return;
        }
    }
    SocExplorerEngine::message("PluginsCache",fileName + " added to cache",3);
    this->items->append(item);
}


QString PluginsCache::first(const QString& pluginName)
{
    for(int i=0;i<this->items->count();i++)
    {
        if(this->items->at(i)->comparePluginName(pluginName))
        {
            return QString(this->items->at(i)->getpath()+"/"+this->items->at(i)->getfileName());
        }
        if(this->items->at(i)->comparefileName(pluginName))
        {
            return QString(this->items->at(i)->getpath()+"/"+this->items->at(i)->getfileName());
        }
        if(this->items->at(i)->comparefilePath(pluginName))
        {
            return QString(this->items->at(i)->getpath()+"/"+this->items->at(i)->getfileName());
        }

    }
    return QString("");
}




QString PluginsCache::first(int VID,int PID)
{
    for(int i=0;i<this->items->count();i++)
    {
        if(this->items->at(i)->compareIDs(VID,PID))
        {
            return QString(this->items->at(i)->getpath()+"/"+this->items->at(i)->getfileName());
        }
    }
    return QString("");
}



QList<PluginsCacheItem *> PluginsCache::listDrivers()
{
    return *items;
}


void PluginsCache::flush()
{
    PluginsCacheItem* ptr;
    for(int i=0;i<this->items->count();i++)
    {
        ptr=this->items->at(i);
        this->items->removeAt(i);
        delete ptr;
    }
}



void PluginsCache::show()
{
    if(this->__view==NULL)this->__view = new QTableWidget(0,5);
    if(!this->__view->isVisible())
    {
        this->__view->clear();
        this->__view->setHorizontalHeaderLabels(QStringList()<<"File Name"<<"Path"<<"PluginName"<<"VID"<<"PID");
        QTableWidgetItem* item;
        this->__view->setRowCount(this->items->count());
        for(int i=0;i<this->items->count();i++)
        {
            item = new QTableWidgetItem(items->at(i)->getfileName());
            item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
            this->__view->setItem(i,0,item);
            item = new QTableWidgetItem(items->at(i)->getpath());
            item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
            this->__view->setItem(i,1,item);
            item = new QTableWidgetItem(items->at(i)->getpluginName());
            item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
            this->__view->setItem(i,2,item);
            item = new QTableWidgetItem(QString::number(items->at(i)->getVID()));
            item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
            this->__view->setItem(i,3,item);
            item = new QTableWidgetItem(QString::number(items->at(i)->getPID()));
            item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
            this->__view->setItem(i,4,item);
        }
        this->__view->resizeRowsToContents();
        this->__view->resizeColumnsToContents();
        this->__view->show();
    }
}







