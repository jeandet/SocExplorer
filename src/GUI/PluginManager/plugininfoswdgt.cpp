/*------------------------------------------------------------------------------
--  This file is a part of the SocExplorer Software
--  Copyright (C) 2011, Plasma Physics Laboratory - CNRS
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
#include "plugininfoswdgt.h"
#include "pluginloader.h"
#include <QDir>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextFragment>
#include <QTextDocumentFragment>
#include <QTextBlock>
#include <QTextCharFormat>
#include <QTextTable>
#include <QApplication>



pluginInfosWdgt::pluginInfosWdgt(QWidget *parent) :
    QWidget(parent)
{
    this->mailLayout = new QVBoxLayout;
    this->textInfos = new QTextBrowser;
    this->textInfos->setReadOnly(true);
    this->mailLayout->addWidget(this->textInfos);
    this->setLayout(this->mailLayout);
    this->textInfos->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}


void pluginInfosWdgt::updateInfos(const QStringList &items)
{
    this->textInfos->clear();
    QTextCursor cursor(this->textInfos->document());
    //QTextTable* table;
    int i=0;
    cursor.atStart();
    cursor.insertText("Plugins:\n");
    for(i=0;i<items.count();i++)
    {
        cursor.insertText(pluginloader::getlibDir(items.at(i))+"\n");
    }
    cursor.insertText("\n\n");
    //table = cursor.insertTable(7,items.count()+1);
    cursor.insertTable(7,items.count()+1);
    cursor.insertText(tr("Plugin name"));
    for(i=0;i<items.count();i++)
    {
        cursor.movePosition(cursor.NextCell);
        cursor.insertText(pluginloader::getlibName(items.at(i)));
    }
    cursor.movePosition(cursor.NextRow);
    cursor.insertText(tr("Root"));
    for(i=0;i<items.count();i++)
    {
        cursor.movePosition(cursor.NextCell);
        if(pluginloader::libcanberoot(items.at(i)))
        {
            cursor.insertText("Yes");
        }
        else
        {
            cursor.insertText("No");
        }
    }
    cursor.movePosition(cursor.NextRow);
    cursor.insertText(tr("Child"));
    for(i=0;i<items.count();i++)
    {
        cursor.movePosition(cursor.NextCell);
        if(pluginloader::libcanbechild(items.at(i)))
        {
            cursor.insertText("Yes");
        }
        else
        {
            cursor.insertText("No");
        }
    }
    cursor.movePosition(cursor.NextRow);
    cursor.insertText(tr("VID"));
    for(i=0;i<items.count();i++)
    {
        cursor.movePosition(cursor.NextCell);
        cursor.insertText(pluginloader::getlibVIDstr(items.at(i)));
    }
    cursor.movePosition(cursor.NextRow);
    cursor.insertText(tr("PID"));
    for(i=0;i<items.count();i++)
    {
        cursor.movePosition(cursor.NextCell);
        cursor.insertText(pluginloader::getlibPIDstr(items.at(i)));
    }
    cursor.movePosition(cursor.NextRow);
    cursor.insertText(tr("Version"));
    for(i=0;i<items.count();i++)
    {
        cursor.movePosition(cursor.NextCell);
        cursor.insertText(pluginloader::getlibVersion(items.at(i)));
    }
    cursor.movePosition(cursor.NextRow);
    cursor.insertText(tr("author"));
    for(i=0;i<items.count();i++)
    {
        cursor.movePosition(cursor.NextCell);
        cursor.insertText(pluginloader::getlibAuthor(items.at(i)));
    }
    cursor.movePosition(cursor.NextBlock);

    for(i=0;i<items.count();i++)
    {
        cursor.insertText("\n\n");
        cursor.insertText(pluginloader::getlibName(items.at(i))+" Description:\n");
        cursor.insertText(pluginloader::getlibDescription(items.at(i)));
    }

}


void pluginInfosWdgt::updateInfos(const QString pluginPath)
{
    this->textInfos->clear();
    QTextCursor cursor(this->textInfos->document());
    QTextCharFormat formatHeader(cursor.charFormat());
    QTextCharFormat formatContent(cursor.charFormat());
    //QTextTable* table;
    formatHeader.setFontWeight(QFont::Bold);
    formatHeader.setForeground(QBrush(Qt::red));
    formatContent.setForeground(Qt::black);
    cursor.atStart();
    if(pluginloader::checklibrary(pluginPath)==1)
    {
        cursor.setBlockCharFormat(formatHeader);
        cursor.insertText(tr("plugin name:"));
        //cursor.insertBlock();
        cursor.setBlockCharFormat(formatContent);
        //table = cursor.insertTable(1,1);
        cursor.insertTable(1,1);
        cursor.insertText(pluginloader::getlibName(pluginPath));
        //table->cellAt( (pluginloader::getlibName(pluginPath));
        //this->textInfos->append(pluginloader::getlibName(pluginPath));
        //this->textInfos->append("================================");
        if(pluginloader::libcanberoot(pluginPath))
        {
            this->textInfos->append(tr("This plugin can be used as root driver."));
        }
        else
        {
            this->textInfos->append(tr("This plugin can't' be used as root driver, so it need to be connected to an other plugin."));
        }
        this->textInfos->append("================================");
        if(pluginloader::libcanbechild(pluginPath))
        {
            this->textInfos->append(tr("This plugin can be used as child driver."));
        }
        else
        {
            this->textInfos->append(tr("This plugin can't' be used as child driver."));
        }
        this->textInfos->append("================================");
        this->textInfos->append(tr("plugin VID:"));
        this->textInfos->append(pluginloader::getlibVIDstr(pluginPath));
        this->textInfos->append("================================");
        this->textInfos->append(tr("plugin PID:"));
        this->textInfos->append(pluginloader::getlibPIDstr(pluginPath));
        this->textInfos->append("================================");
        this->textInfos->append(tr("plugin Version:"));
        this->textInfos->append(pluginloader::getlibVersion(pluginPath));
        this->textInfos->append("================================");
        this->textInfos->append(tr("plugin Author:"));
        this->textInfos->append(pluginloader::getlibAuthor(pluginPath));
        this->textInfos->append("================================");
        this->textInfos->append(tr("plugin Description:"));
        this->textInfos->append(pluginloader::getlibDescription(pluginPath));
    }
    else
    {
        this->textInfos->setText(tr("This is not a valid plugin."));
    }
}
















