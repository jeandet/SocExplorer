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
--                     Mail : alexis.jeandet@member.fsf.org
----------------------------------------------------------------------------*/
#include "srecfile.h"
#include <QTextStream>
#include "binaryfile.h"

srecFile::srecFile()
{
    p_mergingRecords = true;
    litleendian = false;
}

srecFile::srecFile(const QString &File)
{
    p_mergingRecords = true;
    openFile(File);
    litleendian = false;
}

srecFile::srecFile(const QStringList &Files)
{
    p_mergingRecords = true;
    openFiles(Files);
    litleendian = false;
}

srecFile::~srecFile()
{

}

bool srecFile::openFile(const QString &File)
{
    return openFiles(QStringList()<<File);
}

bool srecFile::openFiles(const QStringList &Files)
{
    for(int i=0;i<Files.count();i++)
    {
        this->p_isSrec=true;
        this->p_isSrec &= isSREC(Files.at(i));
        this->p_files.append(new QFile(Files.at(i)));
        this->p_files.at(i)->open(QIODevice::ReadOnly);
        parseFile(this->p_files.at(i));
    }
    return true;
}

bool srecFile::isopened()
{
    bool opened = true;
    for(int i=0;i<this->p_files.count();i++)
    {
        opened &= p_files.at(i)->isOpen();
    }
    return opened;
}

int srecFile::closeFile()
{
    for(int i=0;i<p_files.count();i++)
    {
        delete p_files.at(i);
        delete p_fragments.at(i);
    }
    p_fragments.clear();
    p_files.clear();
    p_fileName.clear();
    return 0;
}

QList<codeFragment *> srecFile::getFragments()
{
    return p_fragments;
}

bool srecFile::toSrec(QList<codeFragment *> fragments, const QString &File)
{
    QString line;
    QFile file(File);
    file.open(QIODevice::WriteOnly);
    if(file.isOpen())
    {
        QTextStream stream( &file );
        //First build header
        stream << buildRecord(0,0,File.toStdString().c_str(),File.count());
        for(int i=0;i<fragments.count();i++)
        {
            codeFragment *fragment = fragments.at(i);
            for(int j=0;j<((int)(fragment->size)/16);j++)
            {
                stream << buildRecord(3,fragment->address+(j*16),fragment->data+(j*16),16);
            }
            int rem = fragment->size % 16;
            if(rem)
            {
                stream << buildRecord(3,fragment->address+fragment->size-rem,fragment->data+fragment->size-rem,rem);
            }
            stream << buildRecord(7,fragment->address,NULL,0);
        }
        file.close();
        return true;
    }

    return false;
}

bool srecFile::toSrec(const QString &File)
{
    return toSrec(p_fragments,File);
}

bool srecFile::toBinary(const QString &File)
{
    return binaryFile::toBinary(p_fragments,File);
}

int srecFile::lineCount()
{
    return p_lineCount;
}

int srecFile::getFragmentsCount()
{
    return p_fragments.count();
}

int srecFile::getFragmentAddress(int index)
{
    if((index < p_fragments.count()) && (index>=0))
    {
        return p_fragments.at(index)->address;
    }
    return 0;
}

int srecFile::getFragmentSize(int index)
{
    if((index < p_fragments.count()) && (index>=0))
    {
        return p_fragments.at(index)->size;
    }
    return 0;
}

codeFragment *srecFile::getFragment(int index)
{
    if((index < p_fragments.count()) && (index>=0))
    {
        return p_fragments.at(index);
    }
    return NULL;
}

QString srecFile::getFragmentHeader(int index)
{
    if((index < p_fragments.count()) && (index>=0))
    {
        return p_fragments.at(index)->header;
    }
    return "";
}

bool srecFile::getFragmentData(int index, char **buffer)
{

    if((index < p_fragments.count()) && (index>=0))
    {
        *buffer = (char *)this->p_fragments.at(index)->data;
        return true;
    }
    return false;
}

bool srecFile::mergingRecords()
{
    return p_mergingRecords;
}

void srecFile::setMergingRecords(bool enabled)
{
    p_mergingRecords = enabled;
}

bool srecFile::isSREC()
{
    return p_isSrec & isopened();
}

bool srecFile::isSREC(const QString &File)
{
    QFile file(File);
    file.open(QIODevice::ReadOnly);
    if(file.isOpen())
    {
        file.seek(0);
        QString line=file.readLine();
        file.close();
        return ((line.at(0)=='S')&&(line.at(1)=='0'));
    }
    return false;
}


void srecFile::parseFile(QFile *file)
{
    if(file->isOpen())
    {
        this->p_lineCount = 0;
        file->seek(0);
        codeFragment* fragment=NULL;
        bool newFragment=true;
        char* data;
        quint64 size=0;
        quint64 address=-1;
        QString header;
        while (!file->atEnd())
        {
            QString line = file->readLine();
            p_lineCount++;
            int rectype = parseLine(line,&address,&data,&size);
            if(rectype==0)
            {
                header.clear();
                header.append(data);
                fragment = new codeFragment(data,size,address);
                fragment->header = header;
                p_fragments.append(fragment);
            }
            else
            {
                if((rectype>=1) && (rectype<=3))
                {
                    if(p_mergingRecords)
                    {
                        //Could I merge it with an other fragment?
                        bool merged = false;
                        for(int i=0;i<p_fragments.count();i++)
                        {
                            codeFragment* frag = p_fragments.at(i);
                            if(((frag->address+frag->size)==address) && (merged==false) && (size!=0))
                            {
                                merged = mergeFragment(frag,data,size);
                            }
                        }
                        if(!merged)
                        {
                            fragment = new codeFragment(data,size,address);
                            fragment->header = header;
                            p_fragments.append(fragment);
                        }
                    }
                    else
                    {
                        if(newFragment)
                        {
                            fragment = new codeFragment(data,size,address);
                            fragment->header = header;
                            p_fragments.append(fragment);
                            newFragment = false;
                        }
                        else
                        {
                            codeFragment* frag = p_fragments.last();
                            mergeFragment(frag,data,size);
                        }
                    }

                }
                else
                {
                    if((rectype>=7) && (rectype<=9))
                    {
                        newFragment  = true;
                    }
                }
            }
        }
    }
}


int srecFile::parseLine(const QString &record, quint64 *address, char **data, quint64 *size)
{
#define newData     (*data)
#define newAddress  (*address)
#define newSize     (*size)
    int recType = -1;
    if((record.count()>4) && checkSum(record))
    {
        if(record.at(0)=='S')
        {
            recType = record.at(1).toLatin1() & 0x0F;
            //Header type
            if(recType==0)
            {
                newAddress = record.mid(4,4).toInt(0,16);
                newSize = record.mid(2,2).toInt(0,16) - 3;
                if(newSize>0)
                {
                    newData=(char*)malloc(newSize+1);
                    for(int i=0;i<(int)newSize;i++)
                    {
                        newData[i] = ((char)record.mid((2*i)+8,2).toInt(0,16));
                    }
                    newData[newSize] = '\0'; // force string end for header
                }
            }
            //2 address byte record type
            if((recType==1) || (recType==5) || (recType==9))
            {
                newAddress = record.mid(4,4).toInt(0,16);
                newSize = record.mid(2,2).toInt(0,16) - 3;
                if(newSize>0)
                {
                    newData=(char*)malloc(newSize);
                    for(int i=0;i<(int)newSize;i++)
                    {
                        newData[i] = ((char)record.mid((2*i)+8,2).toInt(0,16));
                    }
                }
            }
            //3 address byte record type
            if((recType==2) || (recType==6) || (recType==8))
            {
                newAddress = record.mid(4,6).toInt(0,16);
                newSize = record.mid(2,2).toInt(0,16) - 4;
                if(newSize>0)
                {
                    newData=(char*)malloc(newSize);
                    for(int i=0;i<(int)newSize;i++)
                    {
                        newData[i] = ((char)record.mid((2*i)+10,2).toInt(0,16));
                    }
                }
            }
            //4 address byte record type
            if((recType==3) || (recType==7))
            {
                newAddress = record.mid(4,8).toInt(0,16);
                newSize = record.mid(2,2).toInt(0,16) - 5;
                if(newSize>0)
                {
                    newData=(char*)malloc(newSize);
                    for(int i=0;i<(int)newSize;i++)
                    {
                        newData[i] = ((char)record.mid((2*i)+12,2).toInt(0,16));
                    }
                }
            }
        }
    }
    return recType;
}


char srecFile::lineCheckSum(const QString &line)
{
    char sum=0;
    QString localLine = line;
    bool ok;
    if(localLine.at(0)=='S') // then should skip the first two digits
    {
        localLine.remove(0,2);
    }
    for(int i=0;i<localLine.count();i+=2)
    {
        sum+=(char)(localLine.mid(i,2).toInt(&ok,16));
    }
    return ~sum;
}

bool srecFile::checkSum(const QString &line)
{
    QString scp=line;
    scp.remove('\n');
    scp.remove('\r');
    char ck2 = (char)scp.mid(scp.count()-2,2).toInt(0,16);
    char ck=lineCheckSum(scp.remove(scp.count()-2,2));
    if(ck==ck2)
    {
        return true;
    }
    return  false;
//    return (ck2==ck);
}


QString srecFile::buildRecord(int recType, int address,const char *data, int size)
{
    QString record;
    if((recType>=0) && (recType<=9))
    {
        record.append("S");
        record.append(QString::number(recType));
        //2 address byte record type
        if((recType==0) || (recType==1) || (recType==5) || (recType==9))
        {
            record.append(QString("%1").arg(3+size,2,16).replace(' ','0'));
            record.append(QString("%1").arg(address,4,16).replace(' ','0'));
        }
        //3 address byte record type
        if((recType==2) || (recType==6) || (recType==8))
        {
            record.append(QString("%1").arg(4+size,2,16).replace(' ','0'));
            record.append(QString("%1").arg(address,6,16).replace(' ','0'));
        }
        //4 address byte record type
        if((recType==3) || (recType==7))
        {
            record.append(QString("%1").arg(5+size,2,16).replace(' ','0'));
            record.append(QString("%1").arg(address,8,16).replace(' ','0'));
        }
        for(int i=0; i<size;i++)
        {
            record.append(QString("%1").arg((uchar)data[i],2,16).replace(' ','0'));
        }
        record.append(QString("%1").arg((uchar)srecFile::lineCheckSum(record),2,16).replace(' ','0'));
        record.append('\n');
    }
    return record.toUpper();
}

bool srecFile::mergeFragment(codeFragment *fragment, char *data, int size)
{
    char* mergedData=(char*)malloc(size+fragment->size);
    if(mergedData!=NULL)
    {
        memcpy(mergedData,fragment->data,fragment->size);
        memcpy(mergedData+fragment->size,data,size);
        free(fragment->data);
        free(data);
        fragment->data = mergedData;
        fragment->size = fragment->size+size;
        return true;
    }
    return false;
}

