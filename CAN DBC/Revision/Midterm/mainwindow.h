#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QLineEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QMessageBox>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLCDNumber>
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QTimer>
#include <QVector>
#include <QDialog>
#include "qcustomplot.h"
#include "DBCParse.h"
#include "chart_histoWindow.h"
class QLCDNumber;

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

    void on_Delete_Line_clicked();

    void on_Open_File_With_clicked();

    void on_Open_ReadFiles_clicked();

    void on_NewConnection();

    void on_ReadyRead();

    void on_select_currentIndexChanged(int index);

    double dcodeMsgSg(int msg_id, const char* sg_name, const QString& _data,int colindex);

    void on_Graph_clicked();

private:
    Ui::MainWindow *ui;
    QStringList parseDbcFile(const QString& fileName);
    QTcpServer *tcpServer;
    QTcpSocket *clientSocket;
    ChartDialog *chartDialog;
    int second;
    int chart_on_flag;
};
#endif // MAINWINDOW_H
