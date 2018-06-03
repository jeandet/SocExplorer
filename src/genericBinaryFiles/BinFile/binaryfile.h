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
#ifndef BINARYFILE_H
#define BINARYFILE_H
#include "abstractbinfile.h"
#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>
#include <QFile>

class binaryFile : public abstractBinFile
{
    Q_OBJECT
public:
    explicit binaryFile();
    binaryFile(const QString& File);
    binaryFile(const QStringList& Files);
    ~binaryFile();
    bool openFile(const QString& File);
    bool openFiles(const QStringList& Files);
    bool isopened();
    int closeFile();
    QList<codeFragment*> getFragments();
    int getFragmentsCount();
    int getFragmentAddress(int index);
    int getFragmentSize(int index);
    QString getFragmentHeader(int index);
    codeFragment* getFragment(int index);
    bool getFragmentData(int index, char **buffer); 
    bool toSrec(const QString& fileName);
    bool toBinary(const QString& fileName);
    static bool toBinary(QList<codeFragment*> fragments,const QString& File);
signals:

public slots:

private:
    void loadFile(QFile *file);
    QStringList p_fileNames;
    QList<QFile*>p_files;
    QList<codeFragment*> p_fragments;
};

#endif // BINARYFILE_H
