#include "PluginTreeWidget.h"
//#include "pluginloader.h"
#include <QApplication>
#include <qsvgicon.h>
#include <QHeaderView>
#include <QString>
#include <SocExplorerCore.h>
#include <SOC/SOC.h>


PluginTreeWidget::PluginTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    this->editingItem=false;
    this->editeditemprev=new QString;
    this->setAcceptDrops(true);
    this->setDragDropMode(QAbstractItemView::DropOnly);
    connect(this, &PluginTreeWidget::itemChanged, this, &PluginTreeWidget::pluginselectedslt);
    connect(this, &PluginTreeWidget::itemSelectionChanged, this, &PluginTreeWidget::itemSelectionChangedslt);
    this->setHeaderLabels(QStringList()<<"Loaded plugins");
    auto callback = [this](const QHash<QString,std::shared_ptr<ISocexplorerPlugin>>& tree)
    {
        this->treeChanged(tree);
    };

    soc_callbak_id = SOC::registerPluginUpdateCallback(std::move(callback));
}

PluginTreeWidget::~PluginTreeWidget()
{
    SOC::unregisterPluginUpdateCallback(soc_callbak_id);
}


void PluginTreeWidget::pluginselectedslt(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    emit this->pluginselected(item->text(0));
}

void PluginTreeWidget::itemSelectionChangedslt()
{
    if(this->selectedItems().count()==1)
    {
        emit this->pluginselected(this->selectedItems().first()->text(0));
    }
}

void PluginTreeWidget::treeChanged(const QHash<QString, std::shared_ptr<ISocexplorerPlugin> > &tree)
{
    this->clear();
    for(auto driver:tree)
    {
        QTreeWidgetItem* currentItem=new QTreeWidgetItem;
        currentItem->setIcon(0,QSvgIcon(":/images/server.svg"));
        currentItem->setText(0,driver->instanceName());
        this->addTopLevelItem(currentItem);
        if(driver->children().size()!=0)
        {
                this->addplugin(driver,currentItem);
        }
        currentItem->setExpanded(true);
    }
}

void PluginTreeWidget::addplugin(const std::shared_ptr<ISocexplorerPlugin>& parentDriver, QTreeWidgetItem *item)
{

    for(auto driver:parentDriver->children())
    {
        QTreeWidgetItem* currentItem=new QTreeWidgetItem;
        currentItem->setIcon(0,QSvgIcon(":/images/server.svg"));
        currentItem->setText(0,driver->instanceName());
        item->addChild(currentItem);
        if(driver->children().size()!=0)
        {
                this->addplugin(driver,currentItem);
        }
        currentItem->setExpanded(true);
    }
}




void PluginTreeWidget::dragEnterEvent(QDragEnterEvent *event)
{  
    if (event->mimeData()->hasFormat("socexplorer/pluginName"))
    {

        if(SocExplorerCore::pluginManager().isPlugin(event->mimeData()->text()))
        {
            event->acceptProposedAction();
        }else
            {
                event->ignore();
            }

    } else
    {
        event->ignore();
    }
}

void PluginTreeWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("socexplorer/pluginName"))
    {

        if(SocExplorerCore::pluginManager().isPlugin(event->mimeData()->text()))
        {
            this->clearSelection();
            if(this->itemAt(event->pos())!=Q_NULLPTR)
            {
                this->itemAt(event->pos())->setSelected(true);
                event->acceptProposedAction();
            }
            else
                if(SocExplorerCore::pluginManager().pluginCanBeRoot(event->mimeData()->text()))
                {
                    event->acceptProposedAction();
                }
                else
                {
                    event->ignore();
                }
        }else
        {
            event->ignore();
        }

    } else
    {
        event->ignore();
    }
}


void PluginTreeWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    this->clearSelection();
    this->editeditem=this->itemAt(event->pos());
    if(this->editeditem!=NULL && this->editingItem==false)
    {
        *this->editeditemprev=this->editeditem->text(0);
        this->openPersistentEditor(this->editeditem);
        this->editingItem=true;
    }
    QTreeWidget::mouseDoubleClickEvent(event);
}

void PluginTreeWidget::mousePressEvent(QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);
    if(this->editingItem==true && this->editeditem!=this->itemAt(event->pos()))
    {
        this->closePersistentEditor(this->editeditem);
        if(QString::compare(this->editeditem->text(0),this->editeditemprev))
        {
            emit this->changeSysDriverInstName(this->editeditem->text(0),QString(*this->editeditemprev));
        }
        this->editingItem=false;
    }
}


void PluginTreeWidget::keyPressEvent(QKeyEvent *event)
{

    {
        switch(event->key())
        {
        case Qt::Key_Return:
            if(this->editingItem==true)
            {
                this->closePersistentEditor(this->editeditem);
                if(QString::compare(this->editeditem->text(0),this->editeditemprev))
                {
                    emit this->changeSysDriverInstName(this->editeditem->text(0),QString(*this->editeditemprev));
                }
                this->editingItem=false;
            }

            break;
        case Qt::Key_Delete:
            if(this->selectedItems().count()==1)
            {
                emit this->closeSysDriver(this->selectedItems().first()->text(0));
            }
            break;
        default:
            break;
        }
    }
}


void PluginTreeWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("socexplorer/pluginName"))
    {
        QTreeWidgetItem* item=this->itemAt(event->pos());
        if(item!=Q_NULLPTR)
        {
            SOC::loadChildPlugin(event->mimeData()->text(),item->text(0));
            event->acceptProposedAction();
        }
        else if(SocExplorerCore::pluginManager().pluginCanBeRoot(event->mimeData()->text()))
        {
            QString test(event->mimeData()->text());
            SOC::loadPlugin(event->mimeData()->text());
//            emit this->loadSysDriver(event->mimeData()->text());
        }
        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            if(SocExplorerCore::pluginManager().isPlugin(event->mimeData()->text())) //lib ok
            {
                if(SocExplorerCore::pluginManager().pluginCanBeRoot(event->mimeData()->text()))
                    event->acceptProposedAction();
            }
        }
    } else
    {
        event->ignore();
    }
}



