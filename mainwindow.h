#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QObject>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent *e);
    bool countFlag=false;
    bool searchAgain=false;
    bool clearColor=false;
    QStandardItemModel *csvModel;
public slots:
    void searchStringSlot(QString str);
    void createDialogSlot();
    void onChanged(QStandardItem *tempItem);

signals:
    void searchString(QString);
    void openDialog();

private:
    Ui::MainWindow *ui;
    QList<QModelIndex > lastSearched;
    void selectedClear();
};

#endif // MAINWINDOW_H
