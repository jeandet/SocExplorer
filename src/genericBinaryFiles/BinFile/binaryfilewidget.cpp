#include "binaryfilewidget.h"
#include "ui_binaryfilewidget.h"
#include "qtablewidgetintitem.h"
#if QT_VERSION >= 0x050000
    #include <QtWidgets/QTableWidget>
    #include <QtWidgets/QFileDialog>
#else
    #include <QTableWidget>
    #include <QFileDialog>
#endif
#include "srecfile.h"

binaryFileWidget::binaryFileWidget(QWidget *parent) :
    abstractBinFileWidget(parent),
    ui(new Ui::binaryFileWidget)
{
    ui->setupUi(this);
    connect(this->ui->fragmentList,SIGNAL(cellActivated(int,int)),this,SLOT(fragmentCellActivated(int,int)));
    connect(this->ui->fragmentList,SIGNAL(cellChanged(int,int)),this,SLOT(fragmentCellChanged(int,int)));
    exportToSREC_action = new QAction(tr("Export to SREC"),this);
    exportToBIN_action = new QAction(tr("Export to Binary"),this);
    this->ui->fragmentList->addAction(exportToBIN_action);
    this->ui->fragmentList->addAction(exportToSREC_action);
    connect(this->exportToBIN_action,SIGNAL(triggered()),this,SLOT(exportToBIN()));
    connect(this->exportToSREC_action,SIGNAL(triggered()),this,SLOT(exportToSREC()));
}

binaryFileWidget::~binaryFileWidget()
{
    delete ui;
}

void binaryFileWidget::setFile(abstractBinFile *file)
{
    this->p_binfile = (binaryFile*)file;
    if(p_binfile->isopened())
    {
        reloadFile();
    }
}

void binaryFileWidget::reloadFile()
{
    this->ui->fragmentList->clear();
    this->ui->fragmentList->setRowCount(p_binfile->getFragmentsCount());
    this->ui->fragmentList->setHorizontalHeaderLabels(QStringList()<<"File"<<"Size"<<"Address");
    for(int i=0;i<p_binfile->getFragmentsCount();i++)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem(p_binfile->getFragmentHeader(i));
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        this->ui->fragmentList->setItem(i, 0, newItem);

        newItem = (QTableWidgetItem*)new QTableWidgetIntItem(QString("%1").arg(p_binfile->getFragmentSize(i)),DecimalItem);
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        this->ui->fragmentList->setItem(i, 1, newItem);

        newItem = (QTableWidgetItem*)new QTableWidgetIntItem(QString("0x%1").arg(p_binfile->getFragmentAddress(i),8,16).replace(" ","0"),HexaDecimalItem);
//        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        this->ui->fragmentList->setItem(i, 2, newItem);

    }
    this->ui->fragmentList->resizeColumnsToContents();
}

void binaryFileWidget::fragmentCellActivated(int row, int column)
{
    Q_UNUSED(column)
    char* buff=NULL;
    int index = this->ui->fragmentList->item(row,0)->text().toInt();
    if(index!=-1)
    {
        this->p_binfile->getFragmentData(index,&buff);
        this->ui->hexViewer->setData(QByteArray(buff,this->p_binfile->getFragmentSize(index)));
        this->ui->hexViewer->setAddressOffset(this->p_binfile->getFragmentAddress(index));
    }
}

void binaryFileWidget::fragmentCellChanged(int row, int column)
{
    if(column==2)
    {
        QString newAddressStr = this->ui->fragmentList->item(row,column)->text();
        int newAddress = 0;
        newAddressStr.remove(" ");
        if(newAddressStr.at(0)=='0' && newAddressStr.at(1)=='x')
        {
            newAddress = newAddressStr.remove("0x").toUInt(0,16);
        }
        else
        {
            newAddress = newAddressStr.toUInt();
        }
        this->p_binfile->getFragments().at(row)->address = newAddress;
    }
}

void binaryFileWidget::exportToSREC()
{
    QList<codeFragment *>  SelectedFragmentsList=getSelectedFragments();
    if(SelectedFragmentsList.count()>0)
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                   NULL,
                                   tr("SREC Files (*.srec)"));
        if(!fileName.isEmpty())
        {
            srecFile::toSrec(SelectedFragmentsList,fileName);
        }
    }
}

void binaryFileWidget::exportToBIN()
{
    QList<codeFragment *>  SelectedFragmentsList=getSelectedFragments();
    if(SelectedFragmentsList.count()>0)
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                   NULL,
                                   tr("Binary Files (*.bin)"));
        if(!fileName.isEmpty())
        {
            binaryFile::toBinary(SelectedFragmentsList,fileName);
        }
    }
}

QStringList binaryFileWidget::getSelectedFilesNames()
{
    QStringList SelectedFilesList;
    QList<QTableWidgetItem*> items = this->ui->fragmentList->selectedItems();
    for(int i=0;i<items.count();i++)
    {
        QString file = p_binfile->getFragmentHeader(items.at(i)->row());
        if(!SelectedFilesList.contains(file))
        {
            SelectedFilesList.append(file);
        }
    }
    return SelectedFilesList;
}

QList<codeFragment *> binaryFileWidget::getSelectedFragments()
{
    QList<codeFragment *>  SelectedFragmentsList;
    QList<QTableWidgetItem*> items = this->ui->fragmentList->selectedItems();
    for(int i=0;i<items.count();i++)
    {
        codeFragment * fragment = p_binfile->getFragment(items.at(i)->row());
        if(!SelectedFragmentsList.contains(fragment))
        {
            SelectedFragmentsList.append(fragment);
        }
    }
    return SelectedFragmentsList;
}
