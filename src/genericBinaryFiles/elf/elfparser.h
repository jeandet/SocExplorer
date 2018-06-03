/*------------------------------------------------------------------------------
--  This file is a part of the SocExplorer Software
--  Copyright (C) 2013, Plasma Physics Laboratory - CNRS
--
--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 3 of the License, or
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
#ifndef ELFPARSER_H
#define ELFPARSER_H
#include <libelf.h>
#include <gelf.h>
#include <QString>
#include <stdio.h>
#include <QList>
#include <stdint.h>
#include "elffile.h"

class elfparser
{
public:
    elfparser();
    bool iself();
    bool isopened();
    int setFilename(const QString& name);
    int closeFile();
    QString getClass();
    QString getArchitecture();
    QString getType();
    QString getEndianness();
    QString getABI();
    qint64 getVersion();
    qint64 getEntryPointAddress();
    int getSectioncount();
    int getSegmentcount();
    QString getSegmentType(int index);
    qint64 getSegmentOffset(int index);
    qint64 getSegmentVaddr(int index);
    qint64 getSegmentPaddr(int index);
    qint64 getSectionPaddr(int index);
    qint64 getSegmentFilesz(int index);
    qint64 getSectionDatasz(int index);
    bool getSectionData(int index, char **buffer);
    qint64 getSegmentMemsz(int index);
    qint64 getSectionMemsz(int index);
    QString getSegmentFlags(int index);
    QString getSectionName(int index);
    QString getSectionType(int index);
    static bool isElf(const QString& File);

private:
    void updateSections();
    void updateSegments();
    int elfFile;
    bool opened;
    bool type_elf;
    Elf* e;
    Elf_Kind ek;
    GElf_Ehdr ehdr;
    Elf_Scn * scn;
    Elf_Data * data;
    size_t SectionCount,SegmentCount, shstrndx;
    QList<GElf_Phdr*> Segments;
    QList<Elf_Section*> sections;

};

#endif // ELFPARSER_H








