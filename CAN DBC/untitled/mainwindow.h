#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QLineEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>


QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_Save_clicked();
    void on_Signal_clicked();
    void on_Message_clicked();

    //void on_pushButton_clicked();

    void on_Delete_Line_clicked();

    void on_Open_File_With_clicked();
    void on_Open_ReadFiles_clicked();

    void on_Apply_clicked();

    void on_Select_Name_activated(const QString &selectedMessageName);
    void updateTable();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
