#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTime>

QString g_msg_dbc="";
QString g_msg_debug="";
int g_debug_cnt=0;
int g_msg_cnt=0;
int g_sg_cnt[100]={0,};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),tcpServer(nullptr),chartDialog(nullptr),
    clientSocket(nullptr)
{
    g_sg_cnt[0]=1;
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    // 타이머의 timeout() 시그널이 발생할 때마다 updateTable() 슬롯 호출
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTable()));

    // 타이머 시작 (1000ms마다 업데이트)
    timer->start(1000);
    tcpServer = new QTcpServer(this);
    // Listen for incoming connections
    if(tcpServer->listen(QHostAddress::Any, 12345)) {
        connect(tcpServer, &QTcpServer::newConnection, this, &MainWindow::on_NewConnection);
    }

    //digital number
    ui->Throttling->        setDigitCount(5);
    ui->RPM->               setDigitCount(5);
    ui->Velocity->          setDigitCount(5);
    ui->ENG_Temp->          setDigitCount(5);
    ui->Steering_Angle->    setDigitCount(5);

    ui->Throttling->    setSegmentStyle(QLCDNumber::Flat);
    ui->RPM->           setSegmentStyle(QLCDNumber::Flat);
    ui->Velocity->      setSegmentStyle(QLCDNumber::Flat);
    ui->ENG_Temp->      setSegmentStyle(QLCDNumber::Flat);
    ui->Steering_Angle->setSegmentStyle(QLCDNumber::Flat);
    chart_on_flag=0;
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
        g_msg_debug = g_msg_debug + QString::number(g_debug_cnt);
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
    g_msg_debug = g_msg_debug +QString::number(g_debug_cnt);
    }

    ui->DBC_temp->setText(g_msg_dbc);
    ui->DBC_temp->verticalScrollBar()->setValue(ui->DBC_temp->verticalScrollBar()->maximum());
}

void MainWindow::on_Save_clicked()
{
    g_debug_cnt++;
    QString dbctemp = ui->DBC_temp->toPlainText(); // DBC_temp에 담긴 텍스트 가져오기

    if (!dbctemp.isEmpty())
    {
        QString init_file_path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString file_path = QFileDialog::getSaveFileName(this, tr("Save DBC File"), init_file_path, tr("DBC Files (*.dbc);;All Files (*)"));
        if (!file_path.isEmpty())
        {
            QFile file(file_path);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream out(&file);
                out << dbctemp; // DBC_temp에 담긴 내용을 파일에 쓰기
                file.close();
                g_msg_debug += QString::number(g_debug_cnt);
            }
            else
            {
                QMessageBox::warning(this, tr("Error"), tr("Failed to open file for writing."));
            }
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("Cannot save DBC file. DBC_temp is empty."));
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

QVector<Message> messages;
void MainWindow::on_Open_File_With_clicked()
{
    QString str_caption = "Open File";
    QString str_dir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString str_filters = "DBC files (*.dbc);; All Files (*.*)";
    QString filePathText = QFileDialog::getOpenFileName(this, str_caption, str_dir, str_filters);
    QFile fileText(filePathText);
    if (!fileText.exists()) {    // 파일이 존재하지 않는다면
        qDebug() << "file";
        return;
    }
    if (fileText.open(QIODevice::ReadOnly | QIODevice::Text)) { // 파일 열기 시도
        QTextStream in(&fileText);
        QString textContents = in.readAll();
        fileText.close();
        ui->DBC_temp->setText(textContents); // 텍스트 파일 내용을 DBC_temp에 설정
        ui->File_Path->setText(filePathText);
    } else {
        QMessageBox::warning(this, "Warning", "Failed to open file."); // 파일 열기 실패 시 경고 표시
    }
}

void MainWindow::on_Open_ReadFiles_clicked()
{
    QString str_caption = "Open File";
    QString str_dir     = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString str_filters = "DBC files (*.dbc);; All Files (*.*)";
    // 파일 저장할 폴더 경로 가져오기
    QString filePathDBC = QFileDialog::getOpenFileName(this, str_caption, str_dir, str_filters);
    QFile fileDBC(filePathDBC);
    if (!fileDBC.exists()) {    // 파일이 존재하지 않는다면
        qDebug() << "file";
        return;
    }

    //ui->File_Path->setText(filePathDBC);
    ui->ReadFIle_Path->setText(filePathDBC);
    messages = DBCParse::parseDBCToMessages(filePathDBC);
    std::sort(messages.begin(), messages.end(), [](const Message &m1, const Message &m2) {
        return m1.id < m2.id;
    });
    ui->select->clear();

    ui->Can_Message_Table->setColumnCount(4);
    ui->Can_Message_Table->setHorizontalHeaderLabels(QStringList() << "Name" << "CAN ID" << "Data Length" << "Comment");

    for (int i = 0; i < messages.size(); ++i) {
        QList<QStandardItem*> rowItems;
        ui->select->addItem(QString::number(messages[i].id));
        ui->Select2->addItem(QString::number(messages[i].id));

        int rowCount = ui->Can_Message_Table->rowCount();
        ui->Can_Message_Table->insertRow(rowCount);

        // 데이터 삽입
        ui->Can_Message_Table->setItem(rowCount, 0, new QTableWidgetItem(messages[i].name));
        ui->Can_Message_Table->setItem(rowCount, 1, new QTableWidgetItem(QString::number(messages[i].id)));
        ui->Can_Message_Table->setItem(rowCount, 2, new QTableWidgetItem(QString::number(messages[i].length)));
        ui->Can_Message_Table->setItem(rowCount, 3, new QTableWidgetItem((messages[i].cm)));
    }

    ui->Can_Message_Table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->Can_Message_Table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // DBC 파일 내용을 읽어와서 DBC_Read_Temp에 표시
    if (fileDBC.open(QIODevice::ReadOnly | QIODevice::Text)) { // 파일 열기 시도
        QTextStream in(&fileDBC);
        QString dbcContents = in.readAll();
        fileDBC.close();
        ui->DBC_Read_Temp->setText(dbcContents); // 텍스트 파일 내용을 DBC_Read_Temp에 설정
    } else {
        QMessageBox::warning(this, "Warning", "Failed to open file."); // 파일 열기 실패 시 경고 표시
    }
}

void MainWindow::on_select_currentIndexChanged(int index)
{
    if (index >= 0 && index < messages.size()) {
        const QVector<Signal> &signalList = messages[index].signalList;

        // 시그널 테이블 모델 초기화
        ui->CAN_Signal_Table->setColumnCount(10);
        ui->CAN_Signal_Table->setHorizontalHeaderLabels(QStringList() << "Name" << "Start Bit" << "Length" << "Endian" << "Scale" << "Offset" << "Min" << "Max" << "Unit" << "Comment");

        // 시그널 테이블에 데이터 추가
        for (int i = 0; i < signalList.size(); ++i) {
            const Signal &signal = signalList[i];
            // 행 추가
            int rowCount = ui->CAN_Signal_Table->rowCount();
            ui->CAN_Signal_Table->insertRow(rowCount);
            ui->CAN_Signal_Table->setItem(rowCount, 0, new QTableWidgetItem(signal.name));
            ui->CAN_Signal_Table->setItem(rowCount, 1, new QTableWidgetItem(QString::number(signal.startBit)));
            ui->CAN_Signal_Table->setItem(rowCount, 2, new QTableWidgetItem(QString::number(signal.bitLength)));
            ui->CAN_Signal_Table->setItem(rowCount, 3, new QTableWidgetItem(signal.endian));
            ui->CAN_Signal_Table->setItem(rowCount, 4, new QTableWidgetItem(QString::number(signal.scale)));
            ui->CAN_Signal_Table->setItem(rowCount, 5, new QTableWidgetItem(QString::number(signal.offset)));
            ui->CAN_Signal_Table->setItem(rowCount, 6, new QTableWidgetItem(QString::number(signal.min)));
            ui->CAN_Signal_Table->setItem(rowCount, 7, new QTableWidgetItem(QString::number(signal.max)));
            ui->CAN_Signal_Table->setItem(rowCount, 8, new QTableWidgetItem(signal.unit));
            ui->CAN_Signal_Table->setItem(rowCount, 9, new QTableWidgetItem(signal.sgcm));
        }
        // 시그널 테이블 모델 설정
        ui->CAN_Signal_Table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->CAN_Signal_Table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}


QStringList MainWindow::parseDbcFile(const QString& fileName)
{
    QStringList messageIds;

    // DBC 파일을 읽어 메시지 ID를 추출합니다.
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // 파일 열기 실패 시 빈 목록을 반환합니다.
        return messageIds;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.startsWith("BO_")) {
            // BO_ 행에서 메시지 ID를 추출하여 리스트에 추가합니다.
            QStringList parts = line.split(" ");
            if (parts.size() >= 2) {
                QString messageId = parts[1];
                messageIds.append(messageId);
            }
        }
    }

    file.close();
    return messageIds;
}

void MainWindow::on_NewConnection()
{
    clientSocket = tcpServer->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &MainWindow::on_ReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
    ui->CAN_Data_Table->setColumnCount(5);
    // 헤더에 Signal의 이름을 추가합니다.
    QStringList headerLabels;
    //for (const Signal &signal : messages[ui->sg_select_combo_2->currentIndex()].signalList) {
    headerLabels << "Throttling"<<"RPM"<<"KM/h"<<"ENG_Temp"<<"Steer_Angle"; // 헤더 라벨에 Signal의 이름 추가
    //}
    ui->CAN_Data_Table->setHorizontalHeaderLabels(headerLabels); // 열 라벨 설정
    ui->CAN_Data_Table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    if(ui->CAN_Data_Table->rowCount()==0){
        ui->CAN_Data_Table->insertRow(0);
    }
}

#define MASK64(nbits) ((0xffffffffffffffff)>> (64-nbits))
double MainWindow::dcodeMsgSg(int msg_id, const char* sg_name, const QString& _data,int colindex){
    QStringList parts = _data.split(' ',Qt::SkipEmptyParts);
    int ID= parts[0].toInt(nullptr,16);
    if(ID != msg_id){
        return 7777;
    }
    int length=parts[1].toInt();
    uint8_t data[8]= {0,};
    for (int i = 0; i < length; ++i) {

        data[i] = parts[2 + i].toInt(nullptr, 16);
    }
    int msg_address=-1;
    for(int i=0;i<messages.size();i++){
        if(messages[i].id==msg_id)msg_address=i;
    }
    if(msg_address<0)return 7777;
    int sg_address=-1;
    for(int i=0;i<messages[msg_address].signalList.size();i++){
        if(messages[msg_address].signalList[i].name==sg_name)sg_address=i;
    }
    if(sg_address<0)return 7777;
    const Signal &signal = messages[msg_address].signalList[sg_address];
    uint8_t start_byte = signal.startBit / 8;
    uint8_t startbit_in_byte=signal.startBit % 8;
    uint8_t end_byte = 0;
    int8_t count = 0;
    uint64_t target = data[start_byte] >> startbit_in_byte;
    uint8_t current_target_length = (8-startbit_in_byte);
    if(signal.endian == "Big") // Motorola (big endian)
    {
        end_byte = (start_byte * 8 + 8 - startbit_in_byte - signal.bitLength) / 8;

        for(count = start_byte-1; count >= end_byte; count --)
        {
            target |= data[count] << current_target_length;
            current_target_length += 8;
        }
    }
    else // Intel (little endian)
    {
        end_byte = (signal.startBit+signal.bitLength) / 8;

        for(count = start_byte+1; count < end_byte; count ++)
        {
            target |= data[count] << current_target_length;
            current_target_length += 8;
        }
    }

    target &= MASK64(signal.bitLength);
    double scaledValue = ( (int64_t) target ) * signal.scale + signal.offset;
    if(strcmp(sg_name, "TPS") == 0 && scaledValue <=0.5f)scaledValue=0;
    else if(strcmp(sg_name, "SAS_Angle") == 0 && scaledValue >=600)scaledValue-=6553.5f;
    QTableWidgetItem *item = new QTableWidgetItem(QString::number(scaledValue));
    ui->CAN_Data_Table->setItem(0, colindex, item);
    switch (colindex) {
    case 0:
        ui->Throttling->display(scaledValue);
        break;
    case 1:
        ui->RPM->display(scaledValue);
        break;
    case 2:
        ui->Velocity->display(scaledValue);
        break;
    case 3:
        ui->ENG_Temp->display(scaledValue);
        break;
    case 4:
        ui->Steering_Angle->display(scaledValue);
        break;
    default:
        break;
    }
    return scaledValue;
}

void MainWindow::on_ReadyRead()
{
    QByteArray data = clientSocket->readAll();
    //qDebug() << "Received:" << QString(data);
    //decodeCan(data);
    if(chart_on_flag == 1){
        chartDialog->appendTPS(dcodeMsgSg(809,"TPS",data,0));
        chartDialog->appendRPM(dcodeMsgSg(790,"N",data,1));
        chartDialog->appendVS(dcodeMsgSg(790,"VS",data,2));
        chartDialog->appendTEMPENG(dcodeMsgSg(809,"TEMP_ENG",data,3));
        chartDialog->appendSASANGLE(dcodeMsgSg(688,"SAS_Angle",data,4));
        ui->Data_Result->setText(QString(data));
    }else{
        dcodeMsgSg(809,"TPS",data,0);
        dcodeMsgSg(790,"N",data,1);
        dcodeMsgSg(790,"VS",data,2);
        dcodeMsgSg(809,"TEMP_ENG",data,3);
        dcodeMsgSg(688,"SAS_Angle",data,4);
    }

}

void MainWindow::on_Graph_clicked()
{
    if (!chartDialog) {
        chart_on_flag=1;
        chartDialog = new ChartDialog(this);
        chartDialog->setWindowTitle("Chart Histogram");
        //chartDialog->resize(800,300);
        chartDialog->exec();  // 새로운 창 열기
        delete chartDialog;
        chartDialog = nullptr;
        chart_on_flag=0;
    }
}

