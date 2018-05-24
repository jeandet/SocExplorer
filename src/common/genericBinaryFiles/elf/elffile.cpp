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
/*--                  Author :
        Alexis Jeandet
--                     Mail :
        alexis.jeandet@member.fsf.org
----------------------------------------------------------------------------*/
#include "elffile.h"
#include "srecfile.h"
#include "binaryfile.h"
#include <stdint.h>

ElfFile::ElfFile()
    :abstractBinFile()
{
    this->opened = false;
    this->type_elf = false;
    this->elfFile = (int)NULL;
    this->e = Q_NULLPTR;
    this->data = Q_NULLPTR;
    this->scn = Q_NULLPTR;
    this->SymbolCount = 0;
}

ElfFile::ElfFile(const QString &File)
    :abstractBinFile()
{
    this->opened = false;
    this->type_elf = false;
    this->elfFile = (int)NULL;
    this->e = Q_NULLPTR;
    this->data = Q_NULLPTR;
    this->scn = Q_NULLPTR;
    this->SymbolCount = 0;
    this->p_fileName = File;
    openFile(File);
}

ElfFile::~ElfFile()
{
    closeFile();
    if(scn)free(scn);
    if(data)free(data);
    for(int i=0;i<this->sections.count();i++)
    {
        delete this->sections.at(i);
    }
    this->sections.clear();
    for(int i=0;i<this->Segments.count();i++)
    {
        free(this->Segments.at(i));
    }
    this->Segments.clear();
    for(int i=0;i<symbols.count();i++)
    {
        delete this->symbols.at(i);
    }
    this->symbols.clear();
}

bool ElfFile::openFile(const QString &File)
{
    this->p_fileName = File;
    this->closeFile();
    if(elf_version(EV_CURRENT)==EV_NONE)return 0;
#ifdef _ELF_WINDOWS_
    this->elfFile = open(File.toStdString().c_str(),O_RDONLY|O_BINARY ,0);
#else
    this->elfFile = open(File.toStdString().c_str(),O_RDONLY ,0);
#endif
    if(this->elfFile==(int)NULL)return 0;
    this->e = elf_begin(this->elfFile,ELF_C_READ,NULL);
    if(this->e==NULL)return 0;
    this->ek = elf_kind(this->e);
    gelf_getehdr (this->e, &this->ehdr );
    elf_getshdrstrndx (this->e, &this->shstrndx);
    this->updateSegments();
    this->updateSections();
    this->updateSymbols();
    this->opened = true;
    litleendian = isLitleEndian();
    return 1;
}

bool ElfFile::isopened()
{
    return this->opened;
}

int ElfFile::closeFile()
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
    this->opened = false;
    return 0;
}


QList<codeFragment*> ElfFile::getFragments(QStringList fragmentList)
{
    QList<codeFragment*> fragments;
    if (isopened())
    {
        for(int i =0;i<fragmentList.count();i++)
        {
            fragments.append(getFragment(fragmentList.at(i)));
        }
    }
    return fragments;
}

QList<codeFragment*> ElfFile::getFragments()
{
    return getFragments(QStringList()<<".data"<<".text");
}

codeFragment *ElfFile::getFragment(const QString &name)
{
    codeFragment* fragment= new codeFragment();
    for(int i=0;i<getSectionCount();i++)
    {
        if(getSectionName(i) == name)
        {
            fragment->data =NULL;
            fragment->size = getSectionDatasz(i);
            fragment->address = getSectionPaddr(i);
            getSectionData(i,&fragment->data);
        }
    }
    return fragment;
}







QString elfresolveMachine(Elf64_Half	e_machine)
{
    QString machineName;
    //Update from with bash script don't write it by yourself!
    switch(e_machine)
    {
    case EM_NONE:
        machineName = " No machine ";
        break;
    case EM_M32:
        machineName = " AT&T WE 32100 ";
        break;
    case EM_SPARC:
        machineName = " SUN SPARC ";
        break;
    case EM_386:
        machineName = " Intel 80386 ";
        break;
    case EM_68K:
        machineName = " Motorola m68k family ";
        break;
    case EM_88K:
        machineName = " Motorola m88k family ";
        break;
    case EM_860:
        machineName = " Intel 80860 ";
        break;
    case EM_MIPS:
        machineName = " MIPS R3000 big-endian ";
        break;
    case EM_S370:
        machineName = " IBM System/370 ";
        break;
    case EM_MIPS_RS3_LE:
        machineName = " MIPS R3000 little-endian ";
        break;
    case EM_PARISC:
        machineName = " HPPA ";
        break;
    case EM_VPP500:
        machineName = " Fujitsu VPP500 ";
        break;
    case EM_SPARC32PLUS:
        machineName = " Sun's \"v8plus\" ";
        break;
    case EM_960:
        machineName = " Intel 80960 ";
        break;
    case EM_PPC:
        machineName = " PowerPC ";
        break;
    case EM_PPC64:
        machineName = " PowerPC 64-bit ";
        break;
    case EM_S390:
        machineName = " IBM S390 ";
        break;
    case EM_V800:
        machineName = " NEC V800 series ";
        break;
    case EM_FR20:
        machineName = " Fujitsu FR20 ";
        break;
    case EM_RH32:
        machineName = " TRW RH-32 ";
        break;
    case EM_RCE:
        machineName = " Motorola RCE ";
        break;
    case EM_ARM:
        machineName = " ARM ";
        break;
    case EM_FAKE_ALPHA:
        machineName = " Digital Alpha ";
        break;
    case EM_SH:
        machineName = " Hitachi SH ";
        break;
    case EM_SPARCV9:
        machineName = " SPARC v9 64-bit ";
        break;
    case EM_TRICORE:
        machineName = " Siemens Tricore ";
        break;
    case EM_ARC:
        machineName = " Argonaut RISC Core ";
        break;
    case EM_H8_300:
        machineName = " Hitachi H8/300 ";
        break;
    case EM_H8_300H:
        machineName = " Hitachi H8/300H ";
        break;
    case EM_H8S:
        machineName = " Hitachi H8S ";
        break;
    case EM_H8_500:
        machineName = " Hitachi H8/500 ";
        break;
    case EM_IA_64:
        machineName = " Intel Merced ";
        break;
    case EM_MIPS_X:
        machineName = " Stanford MIPS-X ";
        break;
    case EM_COLDFIRE:
        machineName = " Motorola Coldfire ";
        break;
    case EM_68HC12:
        machineName = " Motorola M68HC12 ";
        break;
    case EM_MMA:
        machineName = " Fujitsu MMA Multimedia Accelerator";
        break;
    case EM_PCP:
        machineName = " Siemens PCP ";
        break;
    case EM_NCPU:
        machineName = " Sony nCPU embeeded RISC ";
        break;
    case EM_NDR1:
        machineName = " Denso NDR1 microprocessor ";
        break;
    case EM_STARCORE:
        machineName = " Motorola Start*Core processor ";
        break;
    case EM_ME16:
        machineName = " Toyota ME16 processor ";
        break;
    case EM_ST100:
        machineName = " STMicroelectronic ST100 processor ";
        break;
    case EM_TINYJ:
        machineName = " Advanced Logic Corp. Tinyj emb.fam";
        break;
    case EM_X86_64:
        machineName = " AMD x86-64 architecture ";
        break;
    case EM_PDSP:
        machineName = " Sony DSP Processor ";
        break;
    case EM_FX66:
        machineName = " Siemens FX66 microcontroller ";
        break;
    case EM_ST9PLUS:
        machineName = " STMicroelectronics ST9+ 8/16 mc ";
        break;
    case EM_ST7:
        machineName = " STmicroelectronics ST7 8 bit mc ";
        break;
    case EM_68HC16:
        machineName = " Motorola MC68HC16 microcontroller ";
        break;
    case EM_68HC11:
        machineName = " Motorola MC68HC11 microcontroller ";
        break;
    case EM_68HC08:
        machineName = " Motorola MC68HC08 microcontroller ";
        break;
    case EM_68HC05:
        machineName = " Motorola MC68HC05 microcontroller ";
        break;
    case EM_SVX:
        machineName = " Silicon Graphics SVx ";
        break;
    case EM_ST19:
        machineName = " STMicroelectronics ST19 8 bit mc ";
        break;
    case EM_VAX:
        machineName = " Digital VAX ";
        break;
    case EM_CRIS:
        machineName = " Axis Communications 32-bit embedded processor ";
        break;
    case EM_JAVELIN:
        machineName = " Infineon Technologies 32-bit embedded processor ";
        break;
    case EM_FIREPATH:
        machineName = " Element 14 64-bit DSP Processor ";
        break;
    case EM_ZSP:
        machineName = " LSI Logic 16-bit DSP Processor ";
        break;
    case EM_MMIX:
        machineName = " Donald Knuth's educational 64-bit processor ";
        break;
    case EM_HUANY:
        machineName = " Harvard University machine-independent object files ";
        break;
    case EM_PRISM:
        machineName = " SiTera Prism ";
        break;
    case EM_AVR:
        machineName = " Atmel AVR 8-bit microcontroller ";
        break;
    case EM_FR30:
        machineName = " Fujitsu FR30 ";
        break;
    case EM_D10V:
        machineName = " Mitsubishi D10V ";
        break;
    case EM_D30V:
        machineName = " Mitsubishi D30V ";
        break;
    case EM_V850:
        machineName = " NEC v850 ";
        break;
    case EM_M32R:
        machineName = " Mitsubishi M32R ";
        break;
    case EM_MN10300:
        machineName = " Matsushita MN10300 ";
        break;
    case EM_MN10200:
        machineName = " Matsushita MN10200 ";
        break;
    case EM_PJ:
        machineName = " picoJava ";
        break;
    case EM_OPENRISC:
        machineName = " OpenRISC 32-bit embedded processor ";
        break;
    case EM_ARC_A5:
        machineName = " ARC Cores Tangent-A5 ";
        break;
    case EM_XTENSA:
        machineName = " Tensilica Xtensa Architecture ";
        break;
#ifndef EM_AARCH64
        #define EM_AARCH64	183
#endif
    case EM_AARCH64:
        machineName = " ARM AARCH64 ";
        break;
#ifndef EM_TILEPRO
        #define EM_TILEPRO	188
#endif
    case EM_TILEPRO:
        machineName = " Tilera TILEPro ";
        break;
#ifndef EM_MICROBLAZE
        #define EM_MICROBLAZE	189
#endif
    case EM_MICROBLAZE:
        machineName = " Xilinx MicroBlaze ";
        break;
#ifndef EM_TILEGX
        #define EM_TILEGX	191
#endif
    case EM_TILEGX:
        machineName = " Tilera TILE-Gx ";
        break;
    case EM_NUM:
        machineName = "";
        break;
    default:
        machineName ="Unknow Machine";
        break;
    }
    return machineName;
}




QString ElfFile::getClass()
{
    if(this->e!=NULL)
    {
        int eclass = gelf_getclass(this->e);
        if(eclass==ELFCLASS32)return "ELF32";
        if(eclass==ELFCLASS64)return "ELF64";
    }
    return "none";
}


bool ElfFile::iself()
{
    return (this->getType()!="Unknow");
}

QString ElfFile::getArchitecture()
{
    if(this->e!=NULL)
    {
        return elfresolveMachine(this->ehdr.e_machine);
    }
    return "";
}


QString ElfFile::getType()
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

QString ElfFile::getEndianness()
{
    if(this->e!=NULL)
    {
        if(this->ehdr.e_ident[EI_DATA]==ELFDATA2LSB)return "2's complement, little endian";
        if(this->ehdr.e_ident[EI_DATA]==ELFDATA2MSB)return "2's complement, big endian";
    }
    return "none";
}

bool ElfFile::isLitleEndian()
{
  if(this->e!=NULL)
  {
      if(this->ehdr.e_ident[EI_DATA]==ELFDATA2LSB)return true;
      if(this->ehdr.e_ident[EI_DATA]==ELFDATA2MSB)return false;
  }
  return false;
}

bool ElfFile::isBigEndian()
{
  if(this->e!=NULL)
  {
      if(this->ehdr.e_ident[EI_DATA]==ELFDATA2LSB)return false;
      if(this->ehdr.e_ident[EI_DATA]==ELFDATA2MSB)return true;
  }
  return false;
}

QString ElfFile::getABI()
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


qint64 ElfFile::getVersion()
{
    if(this->e!=NULL)
    {
        return this->ehdr.e_version;
    }
    return -1;
}

qint64 ElfFile::getEntryPointAddress()
{
    if(this->e!=NULL)
    {
        return this->ehdr.e_entry;
    }
    return -1;
}


int ElfFile::getSectionCount()
{
    return (int)this->SectionCount;
}

int ElfFile::getSymbolCount()
{
    return (int)this->SymbolCount;
}


int ElfFile::getSegmentCount()
{
    return (int)this->SegmentCount;
}


QString ElfFile::getSegmentType(int index)
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


qint64 ElfFile::getSegmentOffset(int index)
{
    qint64 Offset = -1;
    if(this->e!=NULL)
    {
        if(index < this->Segments.count())
        {
            Offset =  (qint64)this->Segments.at(index)->p_offset;
        }
    }
    return Offset;
}


qint64 ElfFile::getSegmentVaddr(int index)
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


qint64 ElfFile::getSegmentPaddr(int index)
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

qint64 ElfFile::getSectionPaddr(int index)
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


qint64 ElfFile::getSegmentFilesz(int index)
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

qint64 ElfFile::getSectionDatasz(int index)
{
    int64_t DataSz=0;
    if(this->e!=NULL)
    {
        if(index < this->sections.count())
        {
            if(this->sections.at(index)->section_header->sh_type==SHT_NOBITS)
            {
                DataSz=0;
            }
            else
            {
                DataSz = (int64_t)this->sections.at(index)->data->d_size;
            }
        }
    }
    return DataSz;
}

bool ElfFile::getSectionData(int index, char **buffer)
{
    if(this->e!=NULL)
    {
        if(index < this->sections.count())
        {
            *buffer = (char *)malloc(this->sections.at(index)->data->d_size);
            memcpy(*buffer,this->sections.at(index)->data->d_buf,this->sections.at(index)->data->d_size);
            return true;
        }
    }
    return false;
}


qint64 ElfFile::getSegmentMemsz(int index)
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

qint64 ElfFile::getSectionMemsz(int index)
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


QString ElfFile::getSegmentFlags(int index)
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


QString ElfFile::getSectionName(int index)
{
    if((index<sections.count()) && (index>=0))
    {
        char* nameChr = elf_strptr(this->e , this->shstrndx , this->sections.at(index)->section_header->sh_name);
        return QString(nameChr);
    }
    return "";
}


void ElfFile::updateSections()
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


void ElfFile::updateSegments()
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

void ElfFile::updateSymbols()
{
    for(int i=0;i<symbols.count();i++)
    {
        delete this->symbols.at(i);
    }
    this->symbols.clear();
    updateSections(); //Useless in most case but safer to do it
    for(int i=0;i<(int)SectionCount;i++)
    {
        //First find Symbol table
        QString sectionName = this->getSectionName(i);
        if(sectionName==".symtab" || sectionName == ".dynsym")
        {
            Elf_Section* sec = sections.at(i);
            this->SymbolCount =   sec->section_header->sh_size / sec->section_header->sh_entsize;
            //Then list all symbols
            for(int j=0;j<(int)this->SymbolCount;j++)
            {
                GElf_Sym* esym = (GElf_Sym*)malloc(sizeof(GElf_Sym));
                gelf_getsym(sec->data, j, esym);
                QString name = elf_strptr(this->e,sec->section_header->sh_link,esym->st_name);
                Elf_Symbol* sym = new Elf_Symbol(name,esym);
                symbols.append(sym);
            }
        }
    }

}



QString ElfFile::getSectionType(int index)
{
    QString type("");
    if(this->e!=NULL)
    {
        if(index < this->sections.count())
        {
            switch(this->sections.at(index)->section_header->sh_type)
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

int ElfFile::getSectionIndex(QString name)
{
    if(this->e!=NULL)
    {
        for(int i=0;i<sections.count();i++)
        {
            if(getSectionName(i)==name)
                return i;
        }
    }
    return -1;
}

bool ElfFile::sectionIsNobits(int index)
{
    if(this->e!=NULL)
    {
        if(index < this->sections.count())
        {
            return this->sections.at(index)->section_header->sh_type== SHT_NOBITS;
        }
    }
    return false;
}

QString ElfFile::getSymbolName(int index)
{
    if(this->e!=NULL)
    {
        if(index < this->symbols.count())
        {
            return symbols.at(index)->name;
        }
    }
    return "";
}

QString ElfFile::getSymbolType(int index)
{
    if(this->e!=NULL)
    {
        if(index < this->symbols.count())
        {
            int type = GELF_ST_TYPE(symbols.at(index)->sym->st_info);
            switch(type)
            {
            case STT_NOTYPE:
                return "No Type";
                break;
            case STT_OBJECT:
                return "Object";
                break;
            case STT_FUNC:
                return "Function";
                break;
            case STT_SECTION:
                return "Section";
                break;
            case STT_FILE:
                return "File";
                break;
            case STT_COMMON:
                return "Common data object";
                break;
            case STT_TLS:
                return "Thread-local data object";
                break;
            case STT_NUM:
                return "Number of defined types";
                break;
            case STT_LOOS:
                return "Start of OS-specific";
                break;
            case STT_HIOS:
                return "End of OS-specific";
                break;
            case STT_LOPROC:
                return "Start of processor-specific";
                break;
            case STT_HIPROC:
                return "End of processor-specific";
                break;
            default:
                return "none";
                break;
            }
        }
    }
    return "none";
}

quint64 ElfFile::getSymbolSize(int index)
{
    if(this->e!=NULL)
    {
        if((index < this->symbols.count()) && (index>=0))
        {
            return symbols.at(index)->sym->st_size;
        }
    }
    return 0;
}

int ElfFile::getSymbolIndex(const QString &name)
{
    if(this->e!=NULL)
    {
        for(int i=0;i<this->symbols.count();i++)
        {
            if(this->symbols.at(i)->name==name)
            {
                return i;
            }
        }
    }
    return -1;
}

QString ElfFile::getSymbolSectionName(int index)
{
    if(this->e!=NULL)
    {
        if((index < this->symbols.count()) && (index>=0))
        {
            return getSectionName(symbols.at(index)->sym->st_shndx-1);
        }
    }
    return "none";
}

int ElfFile::getSymbolSectionIndex(int index)
{
    if(this->e!=NULL)
    {
        if((index < this->symbols.count()) && (index>=0))
        {
            return symbols.at(index)->sym->st_shndx;
        }
    }
    return 0;
}

quint64 ElfFile::getSymbolAddress(int index)
{
    if(this->e!=NULL)
    {
        if((index < this->symbols.count()) && (index>=0))
        {
            return symbols.at(index)->sym->st_value;
        }
    }
    return 0;
}

QString ElfFile::getSymbolLinkType(int index)
{
    if(this->e!=NULL)
    {
        if(index < this->symbols.count())
        {
            int btype = GELF_ST_BIND(symbols.at(index)->sym->st_info);
            switch(btype)
            {
            case STB_LOCAL:
                return "Local";
                break;
            case STB_GLOBAL:
                return "Global";
                break;
            case STB_WEAK:
                return "Weak";
                break;
            case STB_NUM:
                return "Number of defined types";
                break;
            case STB_LOOS:
                return "Start of OS-specific";
                break;
            case STB_HIOS:
                return "End of OS-specific";
                break;
            case STB_LOPROC:
                return "Start of processor-specific";
                break;
            case STB_HIPROC:
                return "End of processor-specific";
                break;
            default:
                return "none";
                break;
            }
        }
    }
    return "none";
}

bool ElfFile::isElf(const QString &File)
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
        if((res==4) && (Magic[0]==0x7f) && (Magic[1]==0x45) && (Magic[2]==0x4c) && (Magic[3]==0x46))
        {
            return true;
        }
    }
    return false;
}

bool ElfFile::toSrec(const QString &File)
{
    return srecFile::toSrec(this->getFragments(),File);
}

bool ElfFile::toBinary(const QString &File)
{
    return binaryFile::toBinary(getFragments(),File);
}
