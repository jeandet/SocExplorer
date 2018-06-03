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
#include <abstractbinfile.h>
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <libelf.h>
#include <gelf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#ifndef ELFFILE_H
#define ELFFILE_H

class Elf_Section
{
public:
    Elf_Section(){}
    Elf_Section(Elf_Data*  data,GElf_Shdr* section_header)
    {
        this->data = data;
        this->section_header = section_header;
    }
    ~Elf_Section()
    {
        free(section_header);
    }
    Elf_Data*  data;
    GElf_Shdr* section_header;
};

class Elf_Symbol
{
public:
    Elf_Symbol(){}
    Elf_Symbol(const QString& name,GElf_Sym* sym):name(name),sym(sym){}
    ~Elf_Symbol(){free(sym);}
    QString name;
    GElf_Sym* sym;
};

class ElfFile : public abstractBinFile
{
       Q_OBJECT
public:
    ElfFile();
    ElfFile(const QString& File);
    ~ElfFile();
    bool openFile(const QString& File);
    bool isopened();
    int closeFile();
    QList<codeFragment*> getFragments();
    QList<codeFragment*> getFragments(QStringList fragmentList);

    QString getClass();
    QString getArchitecture();
    QString getType();
    QString getEndianness();
    bool isLitleEndian();
    bool isBigEndian();
    QString getABI();
    qint64 getVersion();
    qint64 getEntryPointAddress();

    int getSectionCount();
    int getSymbolCount();
    int getSegmentCount();

    QString getSegmentType(int index);
    qint64 getSegmentOffset(int index);
    qint64 getSegmentVaddr(int index);
    qint64 getSegmentPaddr(int index);
    qint64 getSegmentFilesz(int index);
    qint64 getSegmentMemsz(int index);
    QString getSegmentFlags(int index);

    bool getSectionData(int index, char **buffer);
    qint64 getSectionPaddr(int index);
    qint64 getSectionMemsz(int index);
    qint64 getSectionDatasz(int index);
    QString getSectionName(int index);
    QString getSectionType(int index);
    int getSectionIndex(QString name);
    bool sectionIsNobits(int index);

    QString getSymbolName(int index);
    QString getSymbolType(int index);
    quint64 getSymbolSize(int index);
    int getSymbolIndex(const QString& name);
    QString getSymbolSectionName(int index);
    int getSymbolSectionIndex(int index);
    quint64 getSymbolAddress(int index);
    QString getSymbolLinkType(int index);
    bool iself();
    static bool isElf(const QString& File);

    bool toSrec(const QString& File);
    bool toBinary(const QString& File);

private:
    codeFragment* getFragment(const QString& name);
    void updateSections();
    void updateSegments();
    void updateSymbols();
    int elfFile;
    bool opened;
    bool type_elf;
    Elf* e;
    Elf_Kind ek;
    GElf_Ehdr ehdr;
    Elf_Scn * scn;
    Elf_Data * data;
    size_t SymbolCount,SectionCount,SegmentCount, shstrndx;
    QList<GElf_Phdr*> Segments;
    QList<Elf_Section*> sections;
    QList<Elf_Symbol*> symbols;

};

#endif // ELFFILE_H
