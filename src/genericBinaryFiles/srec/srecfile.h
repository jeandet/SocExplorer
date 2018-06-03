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
#ifndef SRECFILE_H
#define SRECFILE_H

#include <QObject>
#include <abstractbinfile.h>
#include <QFile>
#include <QStringList>

class srecFile : public abstractBinFile
{
    Q_OBJECT
public:
    explicit srecFile();
    srecFile(const QString& File);
    srecFile(const QStringList& Files);
    ~srecFile();
    bool openFile(const QString& File);
    bool openFiles(const QStringList& Files);
    bool isopened();
    int closeFile();
    QList<codeFragment*> getFragments();
    static bool toSrec(QList<codeFragment*> fragments,const QString& File);
    bool toSrec(const QString &File);
    bool toBinary(const QString& File);
    int lineCount();
    int getFragmentsCount();
    int getFragmentAddress(int index);
    int getFragmentSize(int index);
    codeFragment *getFragment(int index);
    QString getFragmentHeader(int index);
    bool getFragmentData(int index, char **buffer);

    bool mergingRecords();
    void setMergingRecords(bool enabled);
    bool isSREC();
    static bool isSREC(const QString& File);
    static bool checkSum(const QString& line);
signals:

public slots:
private:
    void parseFile(QFile* file);
    static int parseLine(const QString& record, quint64 *address, char** data, quint64 *size);
    static char lineCheckSum(const QString& line);
    static QString buildRecord(int recType,int address,const char* data,int size);
    static bool mergeFragment(codeFragment* fragment, char* data, int size);
    QStringList p_fileNames;
    QList<QFile*>p_files;
    QList<codeFragment*> p_fragments;
    int p_lineCount;
    bool p_isSrec;
    bool p_mergingRecords;

};

#endif // SRECFILE_H
