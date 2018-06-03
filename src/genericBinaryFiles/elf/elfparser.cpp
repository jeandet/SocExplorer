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
#include "elfparser.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

extern QString elfresolveMachine(Elf64_Half	e_machine);


elfparser::elfparser()
{
    this->opened = false;
    this->type_elf = false;
    this->elfFile = (int)NULL;
    this->e = NULL;
}


int elfparser::setFilename(const QString &name)
{
    this->closeFile();
    if(elf_version(EV_CURRENT)==EV_NONE)return 0;
#ifdef _ELF_WINDOWS_
    this->elfFile = open(name.toStdString().c_str(),O_RDONLY|O_BINARY ,0);
#else
    this->elfFile = open(name.toStdString().c_str(),O_RDONLY ,0);
#endif
    if(this->elfFile==(int)NULL)return 0;
    this->e = elf_begin(this->elfFile,ELF_C_READ,NULL);
    if(this->e==NULL)return 0;
    this->ek = elf_kind(this->e);
    gelf_getehdr (this->e, &this->ehdr );
    elf_getshdrstrndx (this->e, &this->shstrndx);
    this->updateSegments();
    this->updateSections();
    return 1;
}


int elfparser::closeFile()
{
    if(this->elfFile!=(int)NULL)
    {
        if(this->e!=NULL)
        {
            elf_end(this->e);
            this->e = NULL;
        }
        close(this->elfFile);
        this->elfFile = (int)NULL;
    }
    return 0;
}

QString elfparser::getClass()
{
    if(this->e!=NULL)
    {
        int eclass = gelf_getclass(this->e);
        if(eclass==ELFCLASS32)return "ELF32";
        if(eclass==ELFCLASS64)return "ELF64";
    }
    return "none";
}


bool elfparser::isopened()
{
    return this->opened;
}


bool elfparser::iself()
{
    return this->type_elf;
}


QString elfparser::getArchitecture()
{
    if(this->e!=NULL)
    {
        return elfresolveMachine(this->ehdr.e_machine);
    }
    return "";
}


QString elfparser::getType()
{
    QString kind("");
    if(this->e!=NULL)
    {
        switch(this->ek)
        {
        case ELF_K_AR:
            kind = "Archive";
            break;
        case ELF_K_ELF:
            kind = "Elf";
            break;
        case ELF_K_COFF:
            kind = "COFF";
            break;
        case ELF_K_NUM:
            kind = "NUM";
            break;
        case ELF_K_NONE:
            kind = "Data";
            break;
        default:
            kind = "Unknow";
            break;
        }
    }
    return kind;
}

QString elfparser::getEndianness()
{
    if(this->e!=NULL)
    {
        if(this->ehdr.e_ident[EI_DATA]==ELFDATA2LSB)return "2's complement, little endian";
        if(this->ehdr.e_ident[EI_DATA]==ELFDATA2MSB)return "2's complement, big endian";
    }
    return "none";
}

QString elfparser::getABI()
{
    if(this->e!=NULL)
    {
        if(this->ehdr.e_ident[EI_OSABI]==ELFOSABI_NONE)return "UNIX System V ABI";
        if(this->ehdr.e_ident[EI_OSABI]==ELFOSABI_SYSV)return "Alias";
        if(this->ehdr.e_ident[EI_OSABI]==ELFOSABI_HPUX)return "HP-UX";
        if(this->ehdr.e_ident[EI_OSABI]==ELFOSABI_NETBSD)return "NetBSD";
        if(this->ehdr.e_ident[EI_OSABI]==ELFOSABI_LINUX)return "Object uses GNU ELF extensions";
        if(this->ehdr.e_ident[EI_OSABI]==ELFOSABI_SOLARIS)return "Sun Solaris";
        if(this->ehdr.e_ident[EI_OSABI]==ELFOSABI_AIX)return "IBM AIX";
        if(this->ehdr.e_ident[EI_OSABI]==ELFOSABI_IRIX)return "SGI Irix";
        if(this->ehdr.e_ident[EI_OSABI]==ELFOSABI_FREEBSD)return "FreeBSD";
        if(this->ehdr.e_ident[EI_OSABI]==ELFOSABI_TRU64)return "Compaq TRU64 UNIX";
        if(this->ehdr.e_ident[EI_OSABI]==ELFOSABI_MODESTO)return " Novell Modesto";
        if(this->ehdr.e_ident[EI_OSABI]==ELFOSABI_OPENBSD)return "OpenBSD";
#ifndef ELFOSABI_ARM_AEABI
    #define ELFOSABI_ARM_AEABI	64
#endif
        if(this->ehdr.e_ident[EI_OSABI]==ELFOSABI_ARM_AEABI)return "ARM EABI";
        if(this->ehdr.e_ident[EI_OSABI]==ELFOSABI_ARM)return "ARM";
        if(this->ehdr.e_ident[EI_OSABI]==ELFOSABI_STANDALONE)return "Standalone (embedded) application";
    }
    return "none";
}


qint64 elfparser::getVersion()
{
    if(this->e!=NULL)
    {
        return this->ehdr.e_version;
    }
    return -1;
}

qint64 elfparser::getEntryPointAddress()
{
    if(this->e!=NULL)
    {
        return this->ehdr.e_entry;
    }
    return -1;
}


int elfparser::getSectioncount()
{
    return (int)this->SectionCount;
}


int elfparser::getSegmentcount()
{
    return (int)this->SegmentCount;
}


QString elfparser::getSegmentType(int index)
{
    QString type("");
    if(this->e!=NULL)
    {
        if(index < this->Segments.count())
        {
            switch(this->Segments.at(index)->p_type)
            {
            case PT_NULL:
                type = "Program header table entry unused";
                break;
            case PT_LOAD:
                type = "Loadable program segment";
                break;
            case PT_DYNAMIC	:
                type = "Dynamic linking information";
                break;
            case PT_INTERP:
                type ="Program interpreter";
                break;
            case PT_NOTE:
                type = "Auxiliary information";
                break;
            case PT_SHLIB:
                type = "Reserved";
                break;
            case PT_PHDR:
                type =	"Entry for header table itself";
                break;
            case PT_TLS:
                type = "Thread-local storage segment";
                break;
            case PT_NUM:
                type = "Number of defined types";
                break;
            case PT_LOOS:
                type =	"Start of OS-specific";
                break;
            case PT_SUNWSTACK:
                type = "Stack segment";
                break;
            case PT_LOPROC:
                type = "Start of processor-specific";
                break;
            case PT_HIPROC:
                type = "End of processor-specific";
                break;
            default:
                type = "Unknow Section Type";
                break;
            }
        }
    }

    return type;
}


qint64 elfparser::getSegmentOffset(int index)
{
    qint64 Offset=-1;
    if(this->e!=NULL)
    {
        if(index < this->Segments.count())
        {
            Offset =  (qint64)this->Segments.at(index)->p_offset;
        }
    }
    return Offset;
}


qint64 elfparser::getSegmentVaddr(int index)
{
    int64_t Vaddr = 0;
    if(this->e!=NULL)
    {
        if(index < this->Segments.count())
        {
            Vaddr =  (int64_t)this->Segments.at(index)->p_vaddr;
        }
    }
    return Vaddr;
}


qint64 elfparser::getSegmentPaddr(int index)
{
    int64_t Paddr=0;
    if(this->e!=NULL)
    {
        if(index < this->Segments.count())
        {
            Paddr = (int64_t)this->Segments.at(index)->p_paddr;
        }
    }
    return Paddr;
}

qint64 elfparser::getSectionPaddr(int index)
{
    int64_t Paddr=0;
    if(this->e!=NULL)
    {
        if(index < this->sections.count())
        {
            Paddr = (int64_t)this->sections.at(index)->section_header->sh_addr;
        }
    }
    return Paddr;
}


qint64 elfparser::getSegmentFilesz(int index)
{
    int64_t FileSz=0;
    if(this->e!=NULL)
    {
        if(index < this->Segments.count())
        {
            FileSz = (int64_t)this->Segments.at(index)->p_filesz;
        }
    }
    return FileSz;
}

qint64 elfparser::getSectionDatasz(int index)
{
    int64_t DataSz=0;
    if(this->e!=NULL)
    {
        if(index < this->sections.count())
        {
            DataSz = (int64_t)this->sections.at(index)->data->d_size;
        }
    }
    return DataSz;
}

bool elfparser::getSectionData(int index, char **buffer)
{
    if(this->e!=NULL)
    {
        if(index < this->sections.count())
        {
            *buffer = (char *)this->sections.at(index)->data->d_buf;
            return true;
        }
    }
    return false;
}


qint64 elfparser::getSegmentMemsz(int index)
{
    int64_t MemSz=0;
    if(this->e!=NULL)
    {
        if(index < this->Segments.count())
        {
            MemSz = (int64_t)this->Segments.at(index)->p_memsz;
        }
    }
    return MemSz;
}

qint64 elfparser::getSectionMemsz(int index)
{
    int64_t MemSz=0;
    if(this->e!=NULL)
    {
        if(index < this->sections.count())
        {
            MemSz = (int64_t)this->sections.at(index)->section_header->sh_size;
        }
    }
    return MemSz;
}


QString elfparser::getSegmentFlags(int index)
{
    QString flags("");
    if(this->e!=NULL)
    {
        if(index < this->Segments.count())
        {
            if((this->Segments.at(index)->p_flags&PF_X) == PF_X)flags+="x";
            if((this->Segments.at(index)->p_flags&PF_W) == PF_W)flags+="w";
            if((this->Segments.at(index)->p_flags&PF_R) == PF_R)flags+="r";
            if((this->Segments.at(index)->p_flags&PF_MASKOS) == PF_MASKOS)flags+=" OS-specific";
            if((this->Segments.at(index)->p_flags&PF_MASKPROC) == PF_MASKPROC)flags+=" Processor-specific";
        }
    }
    return flags;
}


QString elfparser::getSectionName(int index)
{
    char* nameChr = elf_strptr(this->e , this->shstrndx , this->sections.at(index)->section_header->sh_name);
    return QString(nameChr);
}


void elfparser::updateSections()
{
    for(int i=0;i<this->sections.count();i++)
    {
        delete this->sections.at(i);
    }
    this->sections.clear();
    this->scn = elf_nextscn (this->e , NULL );
    this->SectionCount = 0;
    while( this->scn != NULL )
    {
        GElf_Shdr* shdr = (GElf_Shdr*)malloc(sizeof(GElf_Shdr));
        gelf_getshdr ( this->scn , shdr );
        Elf_Data*  data = elf_getdata(this->scn, NULL);
        this->sections.append(new Elf_Section(data,shdr));
        this->SectionCount+=1;
        this->scn = elf_nextscn(e , scn);
    }
}


void elfparser::updateSegments()
{
    for(int i=0;i<this->Segments.count();i++)
    {
        free(this->Segments.at(i));
    }
    this->Segments.clear();
    this->SegmentCount = 0;
    GElf_Phdr* header=(GElf_Phdr*)malloc(sizeof(GElf_Phdr));
    while ( header == gelf_getphdr(this->e ,this->SegmentCount, header ))
    {
        this->Segments.append(header);
        this->SegmentCount++;
        header=(GElf_Phdr*)malloc(sizeof(GElf_Phdr));
    }
    this->SegmentCount = this->Segments.count();
    free(header);
}





QString elfparser::getSectionType(int index)
{
    QString type("");
    if(this->e!=NULL)
    {
        if(index < this->Segments.count())
        {
            switch(this->Segments.at(index)->p_type)
            {
            case SHT_NULL	 :	type = "Section header table entry unused"; break;
            case SHT_PROGBITS	  :		type = "Program data"; break;
            case SHT_SYMTAB	  :		type = "Symbol table"; break;
            case SHT_STRTAB	  :		type = "String table"; break;
            case SHT_RELA	  :		type = "Relocation entries with addends"; break;
            case SHT_HASH	  :		type = "Symbol hash table"; break;
            case SHT_DYNAMIC	 :		type = "Dynamic linking information"; break;
            case SHT_NOTE	  :		type = "Notes"; break;
            case SHT_NOBITS    :type = "Program space with no data (bss)"; break;
            case SHT_REL    :type = "Relocation entries, no addends"; break;
            case SHT_SHLIB	 :		type = "Reserved"; break;
            case SHT_DYNSYM	  :		type = "Dynamic linker symbol table"; break;
            case SHT_INIT_ARRAY	  :		type = "Array of constructors"; break;
            case SHT_FINI_ARRAY	  :		type = "Array of destructors"; break;
            case SHT_PREINIT_ARRAY :		type = "Array of pre-constructors"; break;
            case SHT_GROUP	  :		type = "Section group"; break;
            case SHT_SYMTAB_SHNDX  :		type = "Extended section indeces"; break;
            case SHT_NUM		  :		type = "Number of defined types. "; break;
            case SHT_LOOS	  :	type = "Start OS-specific. "; break;
            case SHT_LOSUNW	  :	type = "Sun-specific low bound. "; break;
            case SHT_SUNW_COMDAT  : type = " "; break;
            case SHT_SUNW_syminfo  : type = " "; break;
            case SHT_GNU_verdef	  :	type = "Version definition section. "; break;
            case SHT_GNU_verneed	  :	type = "Version needs section. "; break;
            case SHT_GNU_versym	  :	type = "Version symbol table. "; break;
            case SHT_LOPROC	  :	type = "Start of processor-specific"; break;
            case SHT_HIPROC	 :	type = "End of processor-specific"; break;
            case SHT_HIUSER	 :	type = "End of application-specific"; break;
            }
        }
    }
    return type;
}

bool elfparser::isElf(const QString &File)
{
    int file =0;
#ifdef _ELF_WINDOWS_
    file = open(File.toStdString().c_str(),O_RDONLY|O_BINARY ,0);
#else
    file = open(File.toStdString().c_str(),O_RDONLY ,0);
#endif
    char Magic[4];
    if(file!=-1)
    {
        size_t res = read(file,Magic,4);
        close(file);
        if((res == 4) && (Magic[0]==0x7f) && (Magic[1]==0x45) && (Magic[2]==0x4c) && (Magic[3]==0x46))
        {
            return true;
        }
    }
    return false;
}








