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
#include "binaryfile.h"
#include "srecfile.h"

binaryFile::binaryFile()
{
  litleendian = false;
}

binaryFile::binaryFile(const QString &File)
{
    openFile(File);
    litleendian = false;
}

binaryFile::binaryFile(const QStringList &Files)
{
    openFiles(Files);
    litleendian = false;
}

binaryFile::~binaryFile()
{

}

bool binaryFile::openFile(const QString &File)
{
    return openFiles(QStringList()<<File);
}

bool binaryFile::openFiles(const QStringList &Files)
{
    for(int i=0;i<Files.count();i++)
    {
        this->p_files.append(new QFile(Files.at(i)));
        this->p_files.at(i)->open(QIODevice::ReadOnly);
        loadFile(this->p_files.at(i));
    }
    return true;
}

bool binaryFile::isopened()
{
    bool opened = true;
    for(int i=0;i<this->p_files.count();i++)
    {
        opened &= p_files.at(i)->isOpen();
    }
    return opened;
}

int binaryFile::closeFile()
{
    for(int i=0;i<p_files.count();i++)
    {
        delete p_files.at(i);
        for(int j=0;j<p_fragments.count();j++)
        {
            if(p_fragments.at(j)->header == p_files.at(i)->fileName())
            {
                codeFragment* fragment = p_fragments.at(j);
                p_fragments.removeAt(j);
                delete fragment;
            }
        }
    }
    p_files.clear();
    p_fileName.clear();
    return 0;
}

QList<codeFragment *> binaryFile::getFragments()
{
    return p_fragments;
}

int binaryFile::getFragmentsCount()
{
    return p_fragments.count();
}

int binaryFile::getFragmentAddress(int index)
{
    if((index>=0)&&(index<p_fragments.count()))
        return p_fragments.at(index)->address;
    return 0;
}

int binaryFile::getFragmentSize(int index)
{
    if((index>=0)&&(index<p_fragments.count()))
        return p_fragments.at(index)->size;
    return 0;
}

QString binaryFile::getFragmentHeader(int index)
{
    if((index>=0)&&(index<p_fragments.count()))
        return p_fragments.at(index)->header;
    return "";
}

codeFragment *binaryFile::getFragment(int index)
{
    if((index>=0)&&(index<p_fragments.count()))
        return p_fragments.at(index);
    return NULL;
}

bool binaryFile::getFragmentData(int index, char **buffer)
{
    if((index>=0)&&(index<p_fragments.count()))
    {
        *buffer = p_fragments.at(index)->data;
        return true;
    }
    return false;
}

bool binaryFile::toSrec(const QString &fileName)
{
    srecFile::toSrec(p_fragments,fileName);
}

bool binaryFile::toBinary(const QString &fileName)
{
    toBinary(p_fragments,fileName);
}

bool binaryFile::toBinary(QList<codeFragment *> fragments, const QString &File)
{
    QFile file(File);
    file.open(QIODevice::WriteOnly);
    if(file.isOpen())
    {
        for(int i=0;i<fragments.count();i++)
        {
            file.write(fragments.at(i)->data,fragments.at(i)->size);
        }
        return true;
    }
    return false;
}

void binaryFile::loadFile(QFile *file)
{
    if (file->isOpen())
    {
        codeFragment* fragment = new codeFragment();
        fragment->header = file->fileName();
        fragment->address = 0;
        fragment->size = file->size();
        fragment->data = (char*)malloc(file->size());
        file->read(fragment->data,file->size());
        p_fragments.append(fragment);
    }
}
