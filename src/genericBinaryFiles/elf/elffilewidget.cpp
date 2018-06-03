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
#include "elffilewidget.h"
#include "ui_elffilewidget.h"
#if QT_VERSION >= 0x050000
    #include <QtWidgets/QTableWidgetItem>
    #include <QtWidgets/QFileDialog>
#else
    #include <QTableWidgetItem>
    #include <QFileDialog>
#endif
#include "qhexedit.h"
#include "qtablewidgetintitem.h"
#include "../srec/srecfile.h"
#include "../BinFile/binaryfile.h"

elfFileWidget::elfFileWidget(QWidget *parent) :
    abstractBinFileWidget(parent),
    ui(new Ui::elfFileWidget)
{
    ui->setupUi(this);
    exportToSREC_action = new QAction(tr("Export to SREC"),this);
    exportToBIN_action = new QAction(tr("Export to Binary"),this);
    viewSymbolInHexViewer_action = new QAction(tr("View in Hexviewer"),this);
    connect(this->ui->sectionsList,SIGNAL(cellActivated(int,int)),this,SLOT(sectionCellActivated(int,int)));
    this->ui->sectionsList->addAction(exportToSREC_action);
    this->ui->sectionsList->addAction(exportToBIN_action);
    this->ui->symbolsList->addAction(viewSymbolInHexViewer_action);
    this->ui->symbolsList->addAction(exportToSREC_action);
    this->ui->symbolsList->addAction(exportToBIN_action);
    connect(this->exportToBIN_action,SIGNAL(triggered()),this,SLOT(exportToBIN()));
    connect(this->exportToSREC_action,SIGNAL(triggered()),this,SLOT(exportToSREC()));
    connect(this->ui->symbolsFilter,SIGNAL(textChanged(QString)),this,SLOT(filterSymbols(QString)));
    connect(this->ui->caseSensitive,SIGNAL(toggled(bool)),this,SLOT(filterSymbolsCaseUpdate(bool)));
    connect(this->viewSymbolInHexViewer_action,SIGNAL(triggered()),this,SLOT(viewSymbolInHexViewer()));
    this->p_hexviewer = new QHexEdit();
    this->p_hexviewer->setWindowTitle("SocExplorer Hexadecimal viewer");
    this->setWindowTitle("SocExplorer Elf viewer");
}



elfFileWidget::~elfFileWidget()
{
    delete ui;
    delete p_hexviewer;
}



void elfFileWidget::setFile(abstractBinFile *file)
{
    this->p_elf = (ElfFile*)file;
    if(p_elf->isopened() && p_elf->iself())
    {
        this->ui->classLabel->setText(p_elf->getClass());
        this->ui->VersionLabel->setText(QString::number(p_elf->getVersion()));
        this->ui->machineLabel->setText(p_elf->getArchitecture());
        this->ui->endiannesLabel->setText(p_elf->getEndianness());
        this->ui->abiLabel->setText(p_elf->getABI());
        this->ui->entryPointLabel->setText(QString("0x%1").arg((uint)p_elf->getEntryPointAddress(),8,16));
        this->ui->typeLabel->setText(p_elf->getType());
        this->ui->sectionCountLabel->setText(QString::number(p_elf->getSectionCount()));
        this->ui->symbolCountLabel->setText(QString::number(p_elf->getSymbolCount()));
    }
    reloadFile();
}

void elfFileWidget::reloadFile()
{
    updateSymbols();
    updateSections();
}



void elfFileWidget::updateSymbols()
{
    this->ui->symbolsList->clear();
    this->ui->symbolsList->setRowCount(p_elf->getSymbolCount());
    this->ui->symbolsList->setHorizontalHeaderLabels(QStringList()<<"Index"<<"Value"<<"Size"<<"Type"<<"Link"<<"Section"<<"Name");
    for(int i=0;i<p_elf->getSymbolCount();i++)
    {
        QTableWidgetItem *newItem = (QTableWidgetItem*)new QTableWidgetIntItem(QString("%1").arg(i),DecimalItem);
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        this->ui->symbolsList->setItem(i, 0, newItem);

        newItem = (QTableWidgetItem*)new QTableWidgetIntItem(QString("0x%1").arg(p_elf->getSymbolAddress(i),8,16).replace(" ","0"),HexaDecimalItem);
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        this->ui->symbolsList->setItem(i, 1, newItem);

        newItem = (QTableWidgetItem*)new QTableWidgetIntItem(QString("%1").arg(p_elf->getSymbolSize(i)),DecimalItem);
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        this->ui->symbolsList->setItem(i, 2, newItem);

        newItem = new QTableWidgetItem(p_elf->getSymbolType(i));
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        this->ui->symbolsList->setItem(i, 3, newItem);

        newItem = new QTableWidgetItem(p_elf->getSymbolLinkType(i));
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        this->ui->symbolsList->setItem(i, 4, newItem);

        newItem = new QTableWidgetItem(p_elf->getSymbolSectionName(i));
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        this->ui->symbolsList->setItem(i, 5, newItem);

        newItem = new QTableWidgetItem(p_elf->getSymbolName(i));
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        this->ui->symbolsList->setItem(i, 6, newItem);
    }
    this->ui->symbolsList->resizeColumnsToContents();
}



void elfFileWidget::updateSections()
{
    this->ui->sectionsList->clear();
    this->ui->sectionsList->setRowCount(p_elf->getSectionCount());
    this->ui->sectionsList->setHorizontalHeaderLabels(QStringList()<<"Index"<<"Name"<<"Address"<<"Size"<<"File Size"<<"Type");
    for(int i=0;i<p_elf->getSectionCount();i++)
    {
        QTableWidgetItem *newItem = (QTableWidgetItem*) new QTableWidgetIntItem(QString("%1").arg(i),DecimalItem);
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        this->ui->sectionsList->setItem(i,0, newItem);

        newItem = new QTableWidgetItem(p_elf->getSectionName(i));
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        this->ui->sectionsList->setItem(i, 1, newItem);

        newItem = (QTableWidgetItem*) new QTableWidgetIntItem(QString("0x%1").arg(p_elf->getSectionPaddr(i),8,16).replace(" ","0"),HexaDecimalItem);
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        this->ui->sectionsList->setItem(i, 2, newItem);

        newItem = (QTableWidgetItem*) new QTableWidgetIntItem(QString("%1").arg(p_elf->getSectionMemsz(i)),DecimalItem);
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        this->ui->sectionsList->setItem(i, 3, newItem);

        newItem = (QTableWidgetItem*) new QTableWidgetIntItem(QString("%1").arg(p_elf->getSectionDatasz(i)),DecimalItem);
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        this->ui->sectionsList->setItem(i, 4, newItem);

        newItem = new QTableWidgetItem(p_elf->getSectionType(i));
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        this->ui->sectionsList->setItem(i, 5, newItem);
    }
    this->ui->sectionsList->resizeColumnsToContents();
}

void elfFileWidget::sectionCellActivated(int row, int column)
{
    Q_UNUSED(column)
    char* buff=NULL;
    int sectionIndex = p_elf->getSectionIndex(this->ui->sectionsList->item(row,1)->text());
    if(sectionIndex!=-1)
    {
        QString type = p_elf->getSectionType(sectionIndex);
        if(!p_elf->sectionIsNobits(sectionIndex))
        {
            this->p_elf->getSectionData(sectionIndex,&buff);
            this->ui->sectionsHexView->setData(QByteArray(buff,this->p_elf->getSectionDatasz(sectionIndex)));
            this->ui->sectionsHexView->setAddressOffset(this->p_elf->getSectionPaddr(sectionIndex));
        }
    }
}

void elfFileWidget::exportToSREC()
{
    QList<codeFragment *> fragments;
    if(this->ui->tabWidget->currentWidget()==this->ui->symbolsTab)
    {
        fragments = getSelectedSymbolsFragments();
    }
    if(this->ui->tabWidget->currentWidget()==this->ui->sectionsTab)
    {
        QStringList sectionList=getSelectedSectionsNames();
        if(sectionList.count()>0)
            fragments = p_elf->getFragments(sectionList);
    }
    if(fragments.count()>0)
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                        NULL,
                                                        tr("SREC Files (*.srec)"));
        if(!fileName.isEmpty())
        {
            srecFile::toSrec(fragments,fileName);
        }
    }

}

void elfFileWidget::exportToBIN()
{
    QList<codeFragment *> fragments;
    if(this->ui->tabWidget->currentWidget()==this->ui->symbolsTab)
    {
        fragments = getSelectedSymbolsFragments();
    }
    if(this->ui->tabWidget->currentWidget()==this->ui->sectionsTab)
    {
        QStringList sectionList=getSelectedSectionsNames();
        if(sectionList.count()>0)
            fragments = p_elf->getFragments(sectionList);
    }
    if(fragments.count()>0)
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                        NULL,
                                                        tr("Binary Files (*.bin)"));
        if(!fileName.isEmpty())
        {
            binaryFile::toBinary(fragments,fileName);
        }
    }

}

void elfFileWidget::viewSymbolInHexViewer()
{
    int row=this->ui->symbolsList->item(this->ui->symbolsList->currentRow(),0)->text().toInt();
    int section = p_elf->getSectionIndex(p_elf->getSymbolSectionName(row));
    if(section!=-1)
    {
        qint64 address = p_elf->getSymbolAddress(row);
        qint64 secAddress = p_elf->getSectionPaddr(section);
        qint64 size = p_elf->getSymbolSize(row);
        char* buff=NULL;
        char* symBuff=NULL;
        if(size && !p_elf->sectionIsNobits(section))
        {
            if(section!=-1)
            {
                symBuff = (char*)malloc(size);
                this->p_elf->getSectionData(section,&buff);
                memcpy(symBuff,buff+(address-secAddress),size);
                this->p_hexviewer->setData(QByteArray(symBuff,size));
                this->p_hexviewer->setAddressOffset(address);
                this->p_hexviewer->show();
            }
        }
    }

}

void elfFileWidget::filterSymbols(const QString &pattern)
{
    Qt::MatchFlags flag = Qt::MatchContains | Qt::MatchStartsWith	| Qt::MatchEndsWith  | Qt::MatchRegExp | Qt::MatchWildcard | Qt::MatchWrap |Qt::MatchRecursive;
    if(this->ui->caseSensitive->isChecked())
        flag |= Qt::MatchCaseSensitive;
    if(pattern.isEmpty())
    {
        for(int i=0;i<this->ui->symbolsList->rowCount();i++)
            this->ui->symbolsList->setRowHidden(i,false);
    }
    else
    {
        for(int i=0;i<this->ui->symbolsList->rowCount();i++)
            this->ui->symbolsList->setRowHidden(i,true);
        QList<QTableWidgetItem*> items = this->ui->symbolsList->findItems(pattern,flag);
        for(int i=0;i<items.count();i++)
            this->ui->symbolsList->setRowHidden(items.at(i)->row(),false);
    }
}

void elfFileWidget::filterSymbolsCaseUpdate(bool toggled)
{
    Q_UNUSED(toggled)
    this->filterSymbols(this->ui->symbolsFilter->text());
}

QList<codeFragment *> elfFileWidget::getSelectedSymbolsFragments()
{
    QList<codeFragment *> fragments;
    codeFragment * fragment;
    if(p_elf->isopened())
    {
        QList<QTableWidgetItem*> items = this->ui->symbolsList->selectedItems();
        for(int i=0;i<items.count();i++)
        {
            int row=this->ui->symbolsList->item(items.at(i)->row(),0)->text().toInt();
            int section = p_elf->getSectionIndex(p_elf->getSymbolSectionName(row));
            if(section!=-1)
            {
                qint64 address = p_elf->getSymbolAddress(row);
                qint64 secAddress = p_elf->getSectionPaddr(section);
                qint64 size = p_elf->getSymbolSize(row);
                char* buff=NULL;
                if(size && !p_elf->sectionIsNobits(section))
                {
                    if(section!=-1)
                    {
                        fragment= new codeFragment();
                        fragment->data = (char*)malloc(size);
                        fragment->address = address;
                        fragment->size = size;
                        this->p_elf->getSectionData(section,&buff);
                        memcpy(fragment->data,buff+(address-secAddress),size);
                        fragments.append(fragment);
                    }
                }
            }

        }
    }
    return fragments;

}



QStringList elfFileWidget::getSelectedSectionsNames()
{
    QStringList sectionList;
    QList<QTableWidgetItem*> items = this->ui->sectionsList->selectedItems();
    for(int i=0;i<items.count();i++)
    {
        QString section = p_elf->getSectionName(items.at(i)->row());
        if(!sectionList.contains(section))
        {
            sectionList.append(section);
        }
    }
    return sectionList;
}














