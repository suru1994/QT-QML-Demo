#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QKeyEvent>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    csvModel = new QStandardItemModel(this);
    ui->tableView->setModel(csvModel);
    QFile file(":/exampleTable.csv"); //To read csv file
    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
        qDebug() << "File not exists";
    } else {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QList<QStandardItem *> standardItemsList;
            for (QString item : line.split(",")) {
                standardItemsList.append(new QStandardItem(item));
            }
            csvModel->insertRow(csvModel->rowCount(), standardItemsList);
        }
        file.close();
    }
    connect(this, SIGNAL(openDialog()), this, SLOT(createDialogSlot())); //signal slot to create a window to search
    connect(this, SIGNAL(searchString(QString)), this, SLOT(searchStringSlot(QString))); //to search a string
    QObject::connect(csvModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(onChanged(QStandardItem*)));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete csvModel;
}

void MainWindow::keyPressEvent(QKeyEvent *e) //keypressevent to take Control + F event
{
    if ( (e->key() == Qt::Key_F)  && QApplication::keyboardModifiers() && Qt::ControlModifier)
    {
        emit openDialog();
    }
    else
    {
        qDebug()<<"Not Correct Key";
    }
}
void MainWindow::createDialogSlot()
{
    qDebug()<<Q_FUNC_INFO;
    QInputDialog *newDialog = new QInputDialog();
    if(QInputDialog::Accepted){
        QString bar = newDialog->getText(this,tr("QInputDialog::getText()"),tr("Keyword:"),QLineEdit::Normal);
        qDebug()<<"Bar"<<bar;
        emit searchString(bar);
    }
}
void MainWindow::selectedClear() //to clear for second search
{

    qDebug()<<Q_FUNC_INFO<<lastSearched.count();
    for(int k=0; k<lastSearched.count(); k++)
    {
        QModelIndex modelIndex = lastSearched.at(k);
        ((QStandardItemModel*)modelIndex.model())->item(modelIndex.row(),modelIndex.column())->setData(QBrush(Qt::transparent),Qt::BackgroundRole);
    }
    lastSearched.clear();
}

void MainWindow::searchStringSlot(QString str) //to search a string in table
{
    selectedClear();
    qDebug()<<Q_FUNC_INFO<<str;
    for (int index = 0; index < csvModel->columnCount(); index++)
    {
        QList<QStandardItem*> foundLst = csvModel->findItems(str, Qt::MatchExactly, index);
        int count = foundLst.count();
        for(int k=0; k<count; k++)
        {
            countFlag=true;
            QModelIndex modelIndex = csvModel->indexFromItem(foundLst[k]);
            lastSearched.append(modelIndex);
            ((QStandardItemModel*)modelIndex.model())->item(modelIndex.row(),index)->setData(QBrush(Qt::yellow),Qt::BackgroundRole);
        }
    }
    if(countFlag==false)
    {
        QMessageBox msgBox;
        msgBox.setText("Not Found..");
        msgBox.exec();
    }
}
void MainWindow::onChanged(QStandardItem *tempItem)
{
    qDebug()<<Q_FUNC_INFO;
    tempItem->text();
    tempItem->column();
    int row = tempItem->row();
    qDebug()<<"text::"<<tempItem->text()<<"Column"<<tempItem->column()<<"Row"<<tempItem->row();
    QFile file("/Users/suru1994/DemosQT/csvDemo/exampleTable.csv"); //To write Data in csv file
    if ( !file.open(QFile::WriteOnly|QFile::Text)) {
        qDebug() << "File not exists";
    }
    else
    {
        for( int row = 0; row < csvModel->rowCount(); ++row )
        {
            QString line;
            for ( int col = 0; col < csvModel->columnCount(); ++col )
            {
               // qDebug() << csvModel->index(row,col).data();
                line.append(csvModel->index(row,col).data().toString());
                if(col+1 != csvModel->columnCount())
                {
                    line.append(",");
                }
            }
            QTextStream out(&file);
            out <<line<<"\n";
        }
    }
    file.close();
}
