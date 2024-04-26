#include "mainwindow.h"
#include "./ui_mainwindow.h"


QString g_msg_dbc="";
QString g_msg_debug="";
int g_debug_cnt=0;
int g_msg_cnt=0;
int g_sg_cnt[100]={0,};

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    g_sg_cnt[0]=1;

    // QTimer 생성
    QTimer *timer = new QTimer(this);

    // 타이머의 timeout() 시그널이 발생할 때마다 updateTable() 슬롯 호출
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTable()));

    // 타이머 시작 (1000ms마다 업데이트)
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Message_clicked()
{
    QString CAN_id = ui->CAN_id->text();
    QString msg_name = ui->msg_name->text();
    QString msg_len = ui->msg_len->text();
    QString Comment_MSG = ui->Comment_MSG->text();

    g_debug_cnt++;
    if(CAN_id.isEmpty()||msg_name.isEmpty()||msg_len.isEmpty()|| g_sg_cnt[g_msg_cnt] ==0)
    {
        g_msg_debug = g_msg_debug + QString::number(g_debug_cnt) +"  Failed to add message\n";
    }
    else
    {
        g_msg_cnt++;

        QString msg_text ="";
        if(!g_msg_dbc.isEmpty())g_msg_dbc=g_msg_dbc+"\n";
        msg_text = "BO_ "+
                   CAN_id + " " +
                   msg_name + ": " +
                   msg_len + " " +
                   Comment_MSG + " ";
        g_msg_dbc = g_msg_dbc + msg_text;
        g_msg_debug = g_msg_debug +QString::number(g_debug_cnt) +"  Message added successfully\n";
    }
    ui->DBC_temp->setText(g_msg_dbc);
    ui->DBC_temp->verticalScrollBar()->setValue(ui->DBC_temp->verticalScrollBar()->maximum());
}

void MainWindow::on_Signal_clicked()
{
    QString sg_name= ui->sg_name->text();
    QString sg_start = ui->sg_start->text();
    QString sg_len = ui->sg_len->text();
    QString sg_factor= ui->sg_factor->text();
    QString sg_offset= ui->sg_offset->text();
    QString sg_min= ui->sg_min->text();
    QString sg_max= ui->sg_max->text();
    QString sg_unit= ui->sg_unit->text();
    QString sg_endian= ui->sg_endian->text();
    QString sg_type= ui->sg_type->text();
    QString Comment_SG = ui->Comment_SG->text();

    g_debug_cnt++;
    if(sg_name.isEmpty()||sg_factor.isEmpty()||sg_offset.isEmpty()||sg_min.isEmpty() ||sg_max.isEmpty() ||sg_unit.isEmpty() || g_msg_cnt == 0)
    {
        g_msg_debug = g_msg_debug + QString::number(g_debug_cnt) +"  Failed to add signal\n";
    }
    else
    {
        if(sg_endian == "Little")
        {
            sg_endian ="@1";
        }
        else
        {
            sg_endian ="@0";
        }
        if(sg_type == "Signed")
        {
            sg_type ="-";
        }
        else
        {
            sg_type ="+";
        }
    g_sg_cnt[g_msg_cnt]++;
    QString sg_text = "\n SG_ "+
                      sg_name + " : " +
                      sg_start + "|" +
                      sg_len +
                      sg_endian +
                      sg_type + " (" +
                      sg_factor + ","+
                      sg_offset + ") ["+
                      sg_min + "|"+
                      sg_max + "] \""+
                      sg_unit + "\" "+
                      Comment_SG + " ";

    g_msg_dbc = g_msg_dbc + sg_text;
    g_msg_debug = g_msg_debug +QString::number(g_debug_cnt) +"  signal added Successfully\n";
    }

    ui->DBC_temp->setText(g_msg_dbc);
    ui->DBC_temp->verticalScrollBar()->setValue(ui->DBC_temp->verticalScrollBar()->maximum());
}

void MainWindow::on_Save_clicked()
{
    g_debug_cnt++;
    if(g_sg_cnt[g_msg_cnt]!=0 && g_msg_cnt != 0 )
    {
        QString init_file_path=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString file_path = QFileDialog::getSaveFileName(this, tr("Save DBC File"), init_file_path, tr("DBC Files (*.dbc);;All Files (*)"));
        if (!file_path.isEmpty())
        {
            QFile file(file_path);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream out(&file);
                out << g_msg_dbc;
                file.close();
                g_msg_debug = g_msg_debug +QString::number(g_debug_cnt) +"  DBC file saved successfully\n";
            }
            else
            {
                g_msg_debug = g_msg_debug +QString::number(g_debug_cnt) +"  Failed to save DBC file\n";
            }
        }
        else
        {
            g_msg_debug = g_msg_debug +QString::number(g_debug_cnt) +"  No file selected for saving\n";
        }
    }
    else
    {
        g_msg_debug = g_msg_debug +QString::number(g_debug_cnt) +"  Syntax is not correct\n";
    }
}


void MainWindow::on_Delete_Line_clicked()
{
    // DBC_temp에 표시된 텍스트 가져오기
    QString text = ui->DBC_temp->toPlainText();

    // 텍스트를 줄 단위로 분할
    QStringList lines = text.split('\n');

    // 마지막 줄 삭제
    if (!lines.isEmpty()) {
        lines.removeLast();
    }

    // 수정된 텍스트를 다시 DBC_temp에 설정
    ui->DBC_temp->setText(lines.join('\n'));
}

void MainWindow::on_Open_File_With_clicked()
{
    // 파일 탐색기창 제목
    QString str_caption = ui->Open_File_With->text();
    // 처음 탐색할 경로 설정
    QString str_dir     = "";
    // 탐색기에서 보여줄 파일 타입 필터 설정
    QString str_filters = "DBC Files (*.dbc);;All Files (*.*)";

    // 파일이름 가져오기
    QString file = QFileDialog::getOpenFileName(this, str_caption, str_dir, str_filters);

    // 만약 파일이 선택되지 않았으면 함수 종료
    if(file.isEmpty()) return;

    ui->File_Path->setText(file);

    // 파일 열기
    QFile dbcFile(file);
    if (!dbcFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // 파일을 열 수 없는 경우 처리
        QMessageBox::warning(this, "Warning", "Failed to open file.");
        return;
    }

    // 파일에서 데이터 읽기
    QTextStream in(&dbcFile);
    QString dbcContents = in.readAll();

    // 파일 닫기
    dbcFile.close();

    // 읽은 데이터를 QLineEdit에 표시
    ui->DBC_temp->setText(dbcContents);
}


void MainWindow::on_Open_ReadFiles_clicked()
{
    QString str_caption = ui->Open_ReadFiles->text();
    QString str_dir = "";
    QString str_filters = "DBC Files (*.dbc);;All Files (*.*)";
    QString file = QFileDialog::getOpenFileName(this, str_caption, str_dir, str_filters);

    if(file.isEmpty())return;
    ui->ReadFIle_Path->setText(file);
    QFile dbcFile(file);

    if(!dbcFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Warning", "Failed to open file.");
        return;
    }

    QTextStream in(&dbcFile);
    QString dbcContents = in.readAll();
    dbcFile.close();
    ui->DBC_Read_Temp->setText(dbcContents);
}


void MainWindow::on_Apply_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("DBC Files (*.dbc)"));
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Warning", "Failed to open file.");
        return;
    }

    // 데이터 분류 및 표에 삽입
    QTextStream in(&file);
    int messageRow = 0;
    int signalRow = 0;
    while (!in.atEnd())
    {
        QString line = in.readLine();

        // BO_로 시작하는 라인은 CAN 메시지로 처리
        if (line.startsWith("BO_"))
        {
            QStringList messageParts = line.split(" ");
            if (messageParts.size() >= 4)
            {
                // 메시지 ID, 이름, 길이 추출
                QString messageId = messageParts[1];
                QString messageName = messageParts[2];
                QString messageLength = messageParts[3];
                QString messageComment = messageParts[4];

                // 데이터 삽입
                ui->Can_Message_Table->insertRow(messageRow);
                ui->Can_Message_Table->setItem(messageRow, 0, new QTableWidgetItem(messageName));
                ui->Can_Message_Table->setItem(messageRow, 1, new QTableWidgetItem(messageId));
                ui->Can_Message_Table->setItem(messageRow, 2, new QTableWidgetItem(messageLength));
                ui->Can_Message_Table->setItem(messageRow, 3, new QTableWidgetItem(messageComment)); // 코멘트는 빈 문자열
                ui->Can_Message_Table->setItem(messageRow, 4, new QTableWidgetItem("Message")); // 타입: 메시지

                ++messageRow;
            }
        }
        // SG_로 시작하는 라인은 CAN 시그널로 처리
        else if (line.startsWith(" SG_")) {
            QStringList signalParts = line.split(" ");
            if (signalParts.size() >= 10) {
                // 시그널 이름, 시작 비트, 길이, Factor, Offset, Min, Max, Unit, Comment 등 추출
                QString signalName = signalParts[2]; // 시그널 이름 추가
                QString startBit = signalParts[4].split("|").first();
                QString length = signalParts[4].split("|").last().replace("@1+", "");
                QString factor = signalParts[5].split(",").first().replace("(", "");;
                QString offset = signalParts[5].split(",").last().chopped(1);
                QString min = signalParts[6].split("|").first().replace("[","");
                QString max = signalParts[6].split("|").last().replace("]","");
                QString unit = signalParts[7].remove('"');
                QString comment = signalParts[9].remove('"');

                // 데이터 삽입
                ui->CAN_Signal_Table->insertRow(signalRow);
                ui->CAN_Signal_Table->setItem(signalRow, 0, new QTableWidgetItem(signalName)); // 시그널 이름 삽입
                ui->CAN_Signal_Table->setItem(signalRow, 1, new QTableWidgetItem(startBit));
                ui->CAN_Signal_Table->setItem(signalRow, 2, new QTableWidgetItem(length));
                ui->CAN_Signal_Table->setItem(signalRow, 3, new QTableWidgetItem(factor));
                ui->CAN_Signal_Table->setItem(signalRow, 4, new QTableWidgetItem(offset));
                ui->CAN_Signal_Table->setItem(signalRow, 5, new QTableWidgetItem(min));
                ui->CAN_Signal_Table->setItem(signalRow, 6, new QTableWidgetItem(max));
                ui->CAN_Signal_Table->setItem(signalRow, 7, new QTableWidgetItem(unit));
                ui->CAN_Signal_Table->setItem(signalRow, 8, new QTableWidgetItem(comment));
                ui->CAN_Signal_Table->setItem(signalRow, 9, new QTableWidgetItem("Signal")); // 타입: 시그널

                ++signalRow;
            }
        }
    }
    file.close();
}


// MainWindow 클래스 소스 파일에 아래 코드 추가
void MainWindow::on_Select_Name_activated(const QString &selectedMessageName_)
{
    // Clear the CAN Signal Table before populating with new data
    ui->CAN_Signal_Table->clearContents();
    ui->CAN_Signal_Table->setRowCount(0);

    // Iterate through each row in the CAN Signal Table
    for (int row = 0; row < ui->CAN_Signal_Table->rowCount(); ++row)
    {
        // Get the CAN Message Name of the current row
        QString messageName = ui->CAN_Signal_Table->item(row, 0)->text();

        // If the CAN Message Name matches the selected CAN Message Name, keep the row visible
        // Otherwise, hide the row
        if (messageName == selectedMessageName_)
        {
            ui->CAN_Signal_Table->showRow(row);
        }
        else
        {
            ui->CAN_Signal_Table->hideRow(row);
        }
    }
}

void MainWindow::updateTable()
{
    // 임의의 샘플 데이터 생성
    QList<QStringList> sampleDataList;
    for (int i = 0; i < 100; ++i) {
        QStringList sampleData;
        sampleData << QString("Time %1").arg(i)
                   << QString::number(i * 100)
                   << (i % 2 == 0 ? "Rx" : "Tx")
                   << (i % 3 == 0 ? "Standard" : "Extended")
                   << QString::number(i % 8)
                   << QString("0x%1 %2 %3 %4 %5 %6 %7 %8").arg(i).arg(i+1).arg(i+2).arg(i+3).arg(i+4).arg(i+5).arg(i+6).arg(i+7);
        sampleDataList.append(sampleData);
    }

    // TableWidget에 데이터 표시
    int rowCount = sampleDataList.size();
    int columnCount = sampleDataList[0].size(); // 모든 행이 동일한 열 수를 가져야 합니다.

    ui->CAN_Data_Table->setRowCount(rowCount); // 행 개수 설정
    ui->CAN_Data_Table->setColumnCount(columnCount); // 열 개수 설정

    for (int i = 0; i < rowCount; ++i) {
        const QStringList &rowData = sampleDataList[i];
        for (int j = 0; j < columnCount; ++j) {
            QTableWidgetItem *item = new QTableWidgetItem(rowData[j]);
            ui->CAN_Data_Table->setItem(i, j, item); // 특정 셀에 데이터 삽입
        }
    }
}
