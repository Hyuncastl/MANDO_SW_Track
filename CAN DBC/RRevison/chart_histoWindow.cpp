#include "chart_histoWindow.h"


ChartDialog::ChartDialog(QWidget *parent)
    : QDialog(parent) {
    cnt_TPS=0;
    cnt_RPM=0;
    cnt_VS=0;
    cnt_TEMP_ENG=0;
    cnt_SAS_Angle=0;
    tm_cnt=0;

    QVBoxLayout *layout_y = new QVBoxLayout(this);
    QHBoxLayout *layout_x = new QHBoxLayout(this);
    layout_y->addLayout(layout_x);
    QHBoxLayout *layout_x1 = new QHBoxLayout(this);
    QHBoxLayout *layout_x2 = new QHBoxLayout(this);
    QHBoxLayout *layout_x3 = new QHBoxLayout(this);

    layout_y->addLayout(layout_x1);
    layout_y->addLayout(layout_x2);
    layout_y->addLayout(layout_x3);

    QVector<QString> plot_name={"Throttling","RPM","Velocity","ENG_Temp","Steering_Angle"};
    QVector<QString> plot_axis={"%","RPM","Km/h","°C","deg","ms","Range","cnt"};

    for(int i=0;i<5;i++){
        plots[i]= new QCustomPlot();
        plots[i]->addGraph();
        plots[i]->graph(0)->setName(plot_name[i]);
        plots[i]->yAxis->setLabel(plot_axis[i]);
        plots[i]->xAxis->setLabel(plot_axis[5]);
        plots[i]->legend->setVisible(true);
        rect_warning[i] = new QCPItemRect(plots[i]);
        rect_warning[i]->topLeft->setCoords(0, 0); // 좌측 상단 (x, y)
        rect_warning[i]->bottomRight->setCoords(0, 0); // 우측 하단 (x, y)
        rect_warning[i]->setBrush(QBrush(QColor(0, 255, 0, 50)));
        plots[i]->setFixedSize(400,300);
        plots[i]->graph(0)->setLineStyle(QCPGraph::lsLine);
        layout_x->addWidget(plots[i]);

        raw_data[i]=new QVector<double>();
        plots_b[i]= new QCustomPlot();
        plots_b[i]->setFixedSize(400,300);
        plots_b[i]->yAxis->setLabel(plot_axis[7]);
        plots_b[i]->xAxis->setLabel(plot_axis[6]);
        bins[i]= new QVector<double>();
        bar_data[i]= new QVector<double>();
        histogram[i] = new QCPBars(plots_b[i]->xAxis, plots_b[i]->yAxis);
        histogram[i]->setData(*bins[i], *bar_data[i]);
        plots_b[i]->rescaleAxes();
        layout_x1->addWidget(plots_b[i]);
    }

    QCheckBox* checkBox = new QCheckBox("AutoScale On/Off", this);
    checkBox->setChecked(false);
    connect(checkBox, &QCheckBox::toggled, this, &ChartDialog::onCheckBoxToggled);
    layout_x3->addWidget(checkBox);
    //설정한 범위만 보기, 경고범위 지정
    QCheckBox* cb_histogram =new QCheckBox("Histogram On",this);
    cb_histogram->setChecked(false);
    connect(cb_histogram, &QCheckBox::toggled, this, &ChartDialog::onCb_histogramToggled);
    layout_x3->addWidget(cb_histogram);

    for(int i=0;i<5;i++){
        if(layout_state[i]==nullptr)layout_state[i] = new QVBoxLayout(this);
        layout_state[i]->addWidget(new QLabel("y값 범위 지정",this));
        lineEdit_range_y[i] = new QLineEdit(this);
        layout_state[i]->addWidget(lineEdit_range_y[i]);
        layout_state[i]->addWidget(new QLabel("x값 범위 지정",this));
        lineEdit_range_x[i] = new QLineEdit(this);
        layout_state[i]->addWidget(lineEdit_range_x[i]);
        layout_state[i]->addWidget(new QLabel("Warning",this));
        lineEdit_warning[i] = new QLineEdit(this);
        layout_state[i]->addWidget(lineEdit_warning[i]);
        layout_x2->addLayout(layout_state[i]);
    }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ChartDialog::updateChart);
    timer->start(500);
}
ChartDialog::~ChartDialog(){

}

int flag_autoscale=0;
void ChartDialog::onCheckBoxToggled(bool checked) {
    if (checked) {
        flag_autoscale=1;
        QMessageBox::information(this, "AutoScale", "AutoScale Off.");
        for(int i=0;i<5;i++){
            if(lineEdit_range_x[i]->text().contains(",")){
                QStringList parts_x=lineEdit_range_x[i]->text().split(',', Qt::SkipEmptyParts);
                range_min[i].setX(parts_x[0].toDouble());
                range_max[i].setX(parts_x[1].toDouble());
            }
            if(lineEdit_range_y[i]->text().contains(",")){
                QStringList parts_y=lineEdit_range_y[i]->text().split(',', Qt::SkipEmptyParts);
                range_min[i].setY(parts_y[0].toDouble());
                range_max[i].setY(parts_y[1].toDouble());
            }
            if(lineEdit_warning[i]->text().contains(",")){
                QStringList parts_warn=lineEdit_warning[i]->text().split(',', Qt::SkipEmptyParts);
                range_warning[i].setX(parts_warn[0].toDouble());
                range_warning[i].setY(parts_warn[1].toDouble());
            }
        }
    } else {
        flag_autoscale=0;
        QMessageBox::information(this, "AutoScale", "AutoScale On.");
        for(int i=0;i<5;i++){
            range_min[i].setX(0);
            range_min[i].setY(0);
            range_max[i].setX(0);
            range_max[i].setY(0);
            range_warning[i].setX(0);
            range_warning[i].setY(0);
            rect_warning[i]->topLeft->setCoords(0, 0); // 좌측 상단 (x, y)
            rect_warning[i]->bottomRight->setCoords(0, 0); // 우측 하단 (x, y)
        }
    }
}

void ChartDialog::onCb_histogramToggled(bool checked){
    if (checked) {
        flag_autoscale=1;
        QMessageBox::information(this, "Histogram", "Histogram On.");
        for(int i=0;i<5;i++){
            histogram_binsize[i]=lineEdit_histogram[i]->text().toInt();
        }
    } else {
        flag_autoscale=0;
        QMessageBox::information(this, "Histogram", "Histogram Off.");
        for(int i=0;i<5;i++){
            histogram_binsize[i]=0;
            raw_data[i]->clear();
            bins[i]->clear();
            bar_data[i]->clear();
            //histogram_binsize[i]=lineEdit_histogram[i]->text().toInt();
        }
    }
}

void ChartDialog::appendTPS(double _scaledValue){
    if(_scaledValue != 7777 && plots[0]!=nullptr && raw_data[0]!=nullptr){
        plots[0]->graph(0)->addData(cnt_TPS++,_scaledValue);
        plots[0]->graph(0)->setPen(QPen(Qt::blue));
        raw_data[0]->append(_scaledValue);
        if(_scaledValue < range_warning[0].x() || _scaledValue > range_warning[0].y()){
            flag_warn[0] = 1;
        }
    }
}
void ChartDialog::appendRPM(double _scaledValue){
    if(_scaledValue != 7777 && plots[1]!=nullptr && raw_data[1]!=nullptr){
        plots[1]->graph(0)->addData(cnt_RPM++,_scaledValue);
        plots[1]->graph(0)->setPen(QPen(Qt::green));
        raw_data[1]->append(_scaledValue);
        if(_scaledValue < range_warning[1].x() || _scaledValue > range_warning[1].y()){
            flag_warn[1] = 1;
        }
    }
}
void ChartDialog::appendVS(double _scaledValue){
    if(_scaledValue != 7777 && plots[2]!=nullptr && raw_data[2]!=nullptr){
        plots[2]->graph(0)->addData(cnt_VS++,_scaledValue);
        plots[2]->graph(0)->setPen(QPen(Qt::cyan));
        raw_data[2]->append(_scaledValue);
        if(_scaledValue < range_warning[2].x() || _scaledValue > range_warning[2].y()){
            flag_warn[2] = 1;
        }
    }
}
void ChartDialog::appendTEMPENG(double _scaledValue){
    if(_scaledValue != 7777 && plots[3]!=nullptr && raw_data[3]!=nullptr){
        plots[3]->graph(0)->addData(cnt_TEMP_ENG++,_scaledValue);
        plots[3]->graph(0)->setPen(QPen(Qt::red));
        raw_data[3]->append(_scaledValue);
        if(_scaledValue < range_warning[3].x() || _scaledValue > range_warning[3].y()){
            flag_warn[3] = 1;
        }
    }
}
void ChartDialog::appendSASANGLE(double _scaledValue){
    if(_scaledValue != 7777 && plots[4]!=nullptr && raw_data[4]!=nullptr){
        plots[4]->graph(0)->addData(cnt_SAS_Angle++,_scaledValue);
        plots[4]->graph(0)->setPen(QPen(Qt::black));
        raw_data[4]->append(_scaledValue);
        if(_scaledValue < range_warning[4].x() || _scaledValue > range_warning[4].y()){
            flag_warn[4] = 1;
        }
    }
}

void ChartDialog::updateChart(){
    tm_cnt++;
    qDebug()<<tm_cnt;
    if(!flag_autoscale){
        for(QCustomPlot* plot_:plots){
            plot_->rescaleAxes();
            plot_->replot();
        }
    }
    else{
        for(int i=0;i<5;i++){

            if(range_max[i].x() != 0){
                plots[i]->xAxis->setRange(range_min[i].x(), range_max[i].x());
                rect_warning[i]->topLeft->setCoords(range_min[i].x(), range_warning[i].y()); // 좌측 상단 (x, y)
                rect_warning[i]->bottomRight->setCoords(range_max[i].x(), range_warning[i].x()); // 우측 하단 (x, y)
                rect_warning[i]->setBrush(QBrush(QColor(0, 255, 0, 10)));
            }
            else{
                plots[i]->rescaleAxes();
                rect_warning[i]->topLeft->setCoords(0, range_warning[i].y()); // 좌측 상단 (x, y)
                rect_warning[i]->bottomRight->setCoords(plots[i]->xAxis->range().upper, range_warning[i].x()); // 우측 하단 (x, y)
                rect_warning[i]->setBrush(QBrush(QColor(0, 255, 0, 50)));

            }
            if (range_warning[i].y() != 0 && flag_warn[i]) {
                auto lastIt = plots[i]->graph(0)->data()->constEnd() - 1;
                QCPGraphData last_data = *lastIt;
                if (last_data.value < range_warning[i].x() || last_data.value > range_warning[i].y()) {
                    // 범위를 벗어나면 빨간색으로 변경하고 깜빡거리게 함
                    if (tm_cnt % 2 == 0) {
                        plots[i]->axisRect()->setBackground(QBrush(QColor(255, 0, 0))); // 빨간색
                    } else {
                        plots[i]->axisRect()->setBackground(QBrush(QColor(255, 255, 255))); // 흰색
                    }
                } else {
                    plots[i]->axisRect()->setBackground(QBrush(QColor(255, 255, 255))); // 흰색
                }
            }

            plots[i]->yAxis->setRange(range_min[i].y(), range_max[i].y());
            plots[i]->replot();
        }
    }
    if(tm_cnt%2==0){
        for(int i=0;i<5;i++){
            replotBar(histogram_binsize[i],plots_b[i],histogram[i],raw_data[i],bins[i],bar_data[i]);
        }
    }

}
void ChartDialog::replotBar(int _binSize,QCustomPlot* plot_b,QCPBars* histogram_,QVector<double>*raw_data,QVector<double>*category, QVector<double>*data_cnt){
    category->clear();
    category->shrink_to_fit();
    data_cnt->clear();
    data_cnt->shrink_to_fit();

    double maxValue = *std::max_element(raw_data->begin(), raw_data->end());
    double minValue = *std::min_element(raw_data->begin(), raw_data->end());

    QPen pen;
    pen.setColor(Qt::NoPen); // 테두리를 없애려면 NoPen을 설정합니다.
    histogram_->setPen(pen);

    histogram[0]->setBrush(QBrush(Qt::blue));
    histogram[1]->setBrush(QBrush(Qt::green));
    histogram[2]->setBrush(QBrush(Qt::cyan));
    histogram[3]->setBrush(QBrush(Qt::red));
    histogram[4]->setBrush(QBrush(Qt::black));

    int binSize;
    if(_binSize==0){
        if (maxValue <= 100 || minValue >= -100) {
            binSize = 10;
        } else if (maxValue >= 1000 || minValue <= -1000) {
            binSize = 100;
        } else {
            binSize = 50;
        }
    }
    else binSize=_binSize;

    int numBins_=0;
    int numBins =0;

    if(minValue<0){
        numBins_= (abs(minValue) + binSize - 1) / binSize +1;
    }
    numBins = (maxValue + binSize - 1) / binSize +1;

    QVector<int> bins(numBins_+numBins, 0);
    for (double value : *raw_data) {
        if(value<0){
            int binIndex = numBins_+ (value / binSize);
            if (binIndex >= 0 && binIndex < numBins_) {
                bins[binIndex]++;
            }
        }
        int binIndex = numBins_ + (value / binSize);
        if (binIndex >= numBins_ && binIndex < numBins+numBins_) {
            bins[binIndex]++;
        }
    }
    for (int i = 0; i < numBins_; i++) {
        int binStart = -(numBins_-i) * binSize;
        category->append(binStart);
        data_cnt->append(bins[i]);
    }
    for (int i = numBins_; i < numBins +numBins_; i++) {
        int binStart = (i-numBins_) * binSize;
        category->append(binStart);
        data_cnt->append(bins[i]);
    }
    histogram_->setData(*category,*data_cnt);
    plot_b->rescaleAxes();
    plot_b->replot();
}
