/*------------------------------------------------------------------------------
--  This file is a part of the SocExplorer Software
--  Copyright (C) 2014, Plasma Physics Laboratory - CNRS
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

#include <socexplorerplugin.h>
#include <abstractbinfile.h>
#include <srec/srecfile.h>
#include <BinFile/binaryfile.h>

int socexplorerplugin::isConnected(){return this->Connected;}

QString socexplorerplugin::baseName(){return *_Name;}

int socexplorerplugin::baseAddress(){return this->BaseAddress;}

void socexplorerplugin::setBaseAddress(unsigned int baseAddress){this->BaseAddress = baseAddress;}

QString socexplorerplugin::instanceName()
{
  return this->_instanceName;
}

int socexplorerplugin::registermenu(QMenu *menu)
{
  this->menu = menu->addMenu(this->_instanceName);
  this->closeAction = this->menu->addAction(tr("Close plugin"));
  QObject::connect(this->closeAction,SIGNAL(triggered()),this,SLOT(closeMe()));
  this->ChildsMenu = this->menu->addMenu(QString("Childs"));
  for(int i=0;i<this->childs.count();i++)
    {
      this->childs.at(i)->registermenu(this->ChildsMenu);
    }
  emit this->registerObject((QObject*)this,this->instanceName());
  return 0;
}

void socexplorerplugin::postInstantiationTrigger()
{
  return;
}

unsigned int socexplorerplugin::Write(unsigned int *Value, unsigned int count, unsigned int address)
{
  if(parent!=NULL)
    {
      return parent->Write(Value,count,address);
    }
  return 0;
}

unsigned int socexplorerplugin::Read(unsigned int *Value, unsigned int count, unsigned int address)
{
  if(parent!=NULL)
    {
      return parent->Read(Value,count,address);
    }
  return 0;
}

QVariantList socexplorerplugin::Read(unsigned int address,unsigned int count)
{
    unsigned int data[count];
    QVariantList result;
    Read(data,count,address);
    for(unsigned int i = 0;i<count;i++)
    {
        result.append(QVariant((int)data[i]));
    }
    return result;
}
void socexplorerplugin::Write(unsigned int address,QList<QVariant> dataList)
{
    unsigned int data[dataList.count()];
    for(int i = 0;i<dataList.count();i++)
    {
        data[i] = (unsigned int)dataList.at(i).toUInt();
    }
    Write(data,dataList.count(),address);
}



void socexplorerplugin::closeMe(){emit this->closePlugin(this);}

void socexplorerplugin::activate(bool flag){this->setEnabled(flag);emit this->activateSig(flag);}

void socexplorerplugin::setInstanceName(const QString &newName)
{
  this->_instanceName = newName;
  if(this->menu)
    this->menu->setTitle(this->_instanceName);
  this->setWindowTitle(newName);
  this->setObjectName(newName);
}

bool socexplorerplugin::dumpMemory(unsigned int address, unsigned int count, QString file)
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
        out << "0x"+QString::number(address+(i*4),16) + ": 0x" + QString::number(buffer[i],16) + "\n";
      free(buffer);
      out.flush();
      outfile.close();
      return true;
    }
  return false;
}

bool socexplorerplugin::memSet(unsigned int address, int value, unsigned int count)
{
  unsigned int* buffer = (unsigned int*)malloc(count*sizeof(unsigned int));
  if(buffer!=NULL)
    {
      memset((void*)buffer,value,count*sizeof(unsigned int));
      this->Write(buffer,count,address);
      free(buffer );
      return true;
    }
  return false;
}

bool socexplorerplugin::loadbin(unsigned int address, QString file)
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

bool socexplorerplugin::loadfile(abstractBinFile *file)
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
                  this->Write(buffer,size,fragments.at(i)->address);
                  free(buffer);
                }
            }
          else
            {
              this->Write((uint32_t*) fragments.at(i)->data,size,fragments.at(i)->address);
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
                  this->Write(buffer,size,fragments.at(i)->address);
                  free(buffer);
                }
            }
          else
            {
              this->Write((uint32_t*) fragments.at(i)->data,size,fragments.at(i)->address);
            }
#endif
        }
    }
  return true;
}

bool socexplorerplugin::dumpMemory(unsigned int address, unsigned int count, QString file, const QString &format)
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
          codeFragment fragment((char*)buffer,count*4,address);
          srecFile::toSrec(QList<codeFragment*>()<<&fragment,file);
        }
      if(!format.compare("bin",Qt::CaseInsensitive))
        {
          //beware this format is not portable from a big endian host to a litle endian one
          codeFragment fragment((char*)buffer,count*4,address);
          binaryFile::toBinary(QList<codeFragment*>()<<&fragment,file);
        }
      if(!format.compare("hexa",Qt::CaseInsensitive))
        {
          QFile outfile(file);
          if (!outfile.open(QIODevice::ReadWrite | QIODevice::Text))
            return false;
          QTextStream out(&outfile);
          for(int i=0;(unsigned int)i<count;i++)
            out << "0x"+QString::number(address+(i*4),16) + ": 0x" + QString::number(buffer[i],16) + "\n";
          free(buffer);
          out.flush();
          outfile.close();
        }
      return true;
    }
  return false;
}

