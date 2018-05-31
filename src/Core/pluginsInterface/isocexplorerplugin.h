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
#ifndef ISOCEXPLORERPLUGIN_H
#define ISOCEXPLORERPLUGIN_H
#include <QtPlugin>
#include <QWidget>
#include <QDockWidget>
#include <QObject>
#include <QVariant>
#include <QVariantList>
#include <socexplorer.h>
#include <malloc.h>
#include <QFile>
#include <stdint.h>
#include <QTextStream>
#include <abstractbinfile.h>
#include <srec/srecfile.h>
#include <BinFile/binaryfile.h>
#include <memory>
#include <vector>
#include <address.h>

class ISocexplorerPlugin : public QDockWidget
{
    Q_OBJECT
public:
    //! Default plugin constructor, any plugin should call this constructor.
    ISocexplorerPlugin(QWidget *parent = Q_NULLPTR,bool createPyObject=true):QDockWidget(parent)
    {
        Q_UNUSED(createPyObject)
        this->Connected = false;
        this->setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetVerticalTitleBar);
    }
    //! Tells if the plugin is connected, it is used to enable or disable all childrens interfaces.
    virtual bool isConnected(){return this->Connected;}
    //! Gives the associated Vendor IDentifier, usefull to automatically associate plugins with found
    //! hardware while board enumeration.
    virtual address64_t baseAddress(){return this->BaseAddress;}
    //! Sets the base address of the current instance, for example if your plugin is supposed to drive
    //! an UART it will correspond to the address of it's first register. This address have at least to
    //! be set by SocExplorer and it can be user accessible if you want.
    virtual void setBaseAddress(address64_t baseAddress){this->BaseAddress = baseAddress;}

    QString  instanceName(){return _instanceName;}
    QString  instance(){return instanceName();}

signals:
    //! Signal emited each time the plugin is about to be closed.
    void closePlugin(ISocexplorerPlugin* driver);
    void activateSig(bool flag);
    void registerObject(QObject* object,const QString& instanceName);

public slots:
    void appendChildPlugin(std::shared_ptr<ISocexplorerPlugin> plugin)
    {
        this->_children.push_back(plugin);
        plugin->setParentPlugin(this);
    }
    void setParentPlugin(ISocexplorerPlugin* parent)
    {
        this->parent = parent;
    }
    virtual void postInstantiationTrigger(){}
    //! Write slot this is the way your children plugins ask you for writing data.
    //! If your plugin is supposed to have childern drivers you should implement this methode.
    //! By default this methode forward the write request to the parent plugin.
    //! \param Value Pointer the data buffer.
    //! \param count Number of 32 bits words you should to write.
    //! \param address Address from where you should to start to write.
    //! \return Quantity of 32 bits words writtens.
    virtual unsigned int Write(unsigned int* Value, int count, address64_t address)
    {
        if(parent!=Q_NULLPTR)
        {
            return parent->Write(Value,count,address);
        }
        return 0;
    }

    void Write(address64_t address, QList<QVariant> dataList)
    {
        unsigned int data[dataList.count()];
        for(int i = 0;i<dataList.count();i++)
        {
            data[i] = (unsigned int)dataList.at(i).toUInt();
        }
        Write(data,dataList.count(),address);
    }
    //! Read slot this is the way your children plugins ask you for reading data.
    //! If your plugin is supposed to have childern drivers you should implement this methode.
    //! By default this methode forward the write request to the parent plugin.
    //! \param Value Pointer the data buffer.
    //! \param count Number of 32 bits words you should to read.
    //! \param address Address from where you should to start to read.
    //! \return Quantity of 32 bits words read.
    virtual unsigned int Read(unsigned int* Value, int count, address64_t address)
    {
        if(parent!=Q_NULLPTR)
        {
            return parent->Read(Value,count,address);
        }
        return 0;
    }
    QVariantList Read(address64_t address, int count)
    {
        unsigned int data[count];
        QVariantList result;
        Read(data,count,address);
        for(int i = 0;i<count;i++)
        {
            result.append(QVariant(static_cast<int>(data[i])));
        }
        return result;
    }

    virtual void closeMe(){emit this->closePlugin(this);}

    virtual void activate(bool flag)
    {
        this->setEnabled(flag);
        emit this->activateSig(flag);
    }

    virtual void setInstanceName(const QString& newName)
    {
        this->_instanceName = newName;
        this->setWindowTitle(newName);
        this->setObjectName(newName);
    }

    virtual bool dumpMemory(address64_t address, unsigned int count, QString file)
    {
        unsigned int* buffer = (unsigned int*)malloc(count*sizeof(unsigned int));
        if(buffer!=NULL)
        {
            this->Read(buffer,count,address);
            QFile outfile(file);
            if (!outfile.open(QIODevice::ReadWrite | QIODevice::Text))
                return false;
            QTextStream out(&outfile);
            for(int i=0;(unsigned int)i<count;i++)
                out << "0x"+QString::number(address.value+(i*4),16) + ": 0x" + QString::number(buffer[i],16) + "\n";
            free(buffer);
            out.flush();
            outfile.close();
            return true;
        }
        return false;
    }

    virtual bool dumpMemory(address64_t address,unsigned int count,QString file,const QString& format)
    {
        unsigned int* buffer = (unsigned int*)malloc(count*sizeof(unsigned int));
        if(buffer!=NULL)
        {
            this->Read(buffer,count,address);
            if(!format.compare("srec",Qt::CaseInsensitive))
            {
                //need to convert from in memory endianness to file endianness
                //SREC is always big endian
#if __BYTE_ORDER == __LITTLE_ENDIAN
                for(int l=0;l<(count);l++)
                {
                    buffer[l] = socexplorerBswap32(buffer[l]);
                }
#elif __BYTE_ORDER == __BIG_ENDIAN

#endif
                codeFragment fragment((char*)buffer,count*4,address.value);
                srecFile::toSrec(QList<codeFragment*>()<<&fragment,file);
            }
            if(!format.compare("bin",Qt::CaseInsensitive))
            {
                //beware this format is not portable from a big endian host to a litle endian one
                codeFragment fragment((char*)buffer,count*4,address.value);
                binaryFile::toBinary(QList<codeFragment*>()<<&fragment,file);
            }
            if(!format.compare("hexa",Qt::CaseInsensitive))
            {
                QFile outfile(file);
                if (!outfile.open(QIODevice::ReadWrite | QIODevice::Text))
                    return false;
                QTextStream out(&outfile);
                for(int i=0;(unsigned int)i<count;i++)
                    out << "0x"+QString::number(address.value+(i*4),16) + ": 0x" + QString::number(buffer[i],16) + "\n";
                free(buffer);
                out.flush();
                outfile.close();
            }
            return true;
        }
        return false;
    }

    virtual bool memSet(address64_t address, int value, unsigned int count)
    {
        unsigned int* buffer = static_cast<unsigned int*>(malloc(count*sizeof(unsigned int)));
        if(buffer!=Q_NULLPTR)
        {
            memset(static_cast<void*>(buffer),value,count*sizeof(unsigned int));
            this->Write(buffer,count,address);
            free(buffer );
            return true;
        }
        return false;
    }

    virtual bool loadbin(address64_t address,QString file)
    {
        QFile infile(file);
        if (!infile.open(QIODevice::ReadOnly))
            return false;
        uint32_t* buffer = (uint32_t*)malloc(infile.size());
        if(buffer!=NULL)
        {
            infile.read((char*)buffer,infile.size());
            for(int i=0;i<(infile.size()/4);i++)
            {
                buffer[i] = socexplorerBswap32(buffer[i]);
            }
            this->Write(buffer,infile.size()/4,address);
            free(buffer);
            return true;
        }
        return false;

    }

    virtual bool loadfile(abstractBinFile* file)
    {
        {
            if(file->isopened())
            {
                QList<codeFragment*> fragments= file->getFragments();
                for(int i=0;i<fragments.count();i++)
                {
                    int size = fragments.at(i)->size/4;
                    // TODO fixme, should be the oposite
#if __BYTE_ORDER == __LITTLE_ENDIAN
                    if(!file->litleendian)
                    {
                        uint32_t* buffer = (uint32_t*)malloc(fragments.at(i)->size);
                        memcpy(buffer,fragments.at(i)->data,fragments.at(i)->size);
                        if(buffer!=NULL)
                        {
                            for(int l=0;l<(size);l++)
                            {
                                buffer[l] = socexplorerBswap32(buffer[l]);
                            }
                            this->Write(buffer,size,address64_t{fragments.at(i)->address});
                            free(buffer);
                        }
                    }
                    else
                    {
                        this->Write((uint32_t*) fragments.at(i)->data,size,address64_t{fragments.at(i)->address});
                    }
#elif __BYTE_ORDER == __BIG_ENDIAN
                    if(file->litleendian)
                    {
                        uint32_t* buffer = (uint32_t*)malloc(fragments.at(i)->size);
                        memcpy(buffer,fragments.at(i)->data,fragments.at(i)->size);
                        if(buffer!=NULL)
                        {
                            for(int l=0;l<(size);l++)
                            {
                                buffer[l] = socexplorerBswap32(buffer[l]);
                            }
                            this->Write(buffer,size,address64_t{fragments.at(i)->address});
                            free(buffer);
                        }
                    }
                    else
                    {
                        this->Write((uint32_t*) fragments.at(i)->data,size,address64_t{fragments.at(i)->address});
                    }
#endif
                }
            }
            return true;
        }
    }
    ISocexplorerPlugin* parentPlugin(){return this->parent;}
    ISocexplorerPlugin* toPlugin(){return static_cast<ISocexplorerPlugin*>(this);}
    const std::vector<std::shared_ptr<ISocexplorerPlugin>>& children()
    {
        return this->_children;
    }
protected:
    QString  _instanceName;
    address64_t BaseAddress;
    std::vector<std::shared_ptr<ISocexplorerPlugin>> _children;
    ISocexplorerPlugin* parent;
    bool Connected;
};

Q_DECLARE_INTERFACE(ISocexplorerPlugin, "socexplorer.plugins.ISocexplorerPlugin")

#endif // ISOCEXPLORERPLUGIN_H
