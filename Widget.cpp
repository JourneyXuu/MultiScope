#include "Widget.h"
#include "./ui_Widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    serial = new QSerialPort(this);
    timer_serialRefresh = new QTimer(this);
    timer_testData = new QTimer(this);

    for(int i=0; i<200; ++i) {
        double angle = 2.0 * M_PI * i / 200.0;
        uint16_t adc = static_cast<uint16_t>((sin(angle) + 1.0) * 2047.5);
        m_sinTable.append(adc);
    }

    initControlPanel();//初始化combo，添加波特率选项

    initMainPlot();
    initSubPlots();

    connect(timer_serialRefresh, &QTimer::timeout, this, &Widget::timer_refreshSerialPorts);
    connect(timer_testData,&QTimer::timeout,this,&Widget::timer_sendSimulatedData);
    connect(serial, &QSerialPort::readyRead, this, &Widget::on_serialDataReady);

    timer_serialRefresh->start(1000); //1000ms扫描一次串口

}

Widget::~Widget()
{
    delete ui;
}

/**
 *@brief:初始化控制面板
 */
void Widget::initControlPanel()
{
    //1.波特率配置
    QStringList baudRates = {"1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200","921600"};
    ui->cbBaud->addItems(baudRates);
    ui->cbBaud->setCurrentText("921600"); // 设置默认值
    //2.初始化测试按键
    ui->btnTest->setText("发送测试");
}

/**
 * @brief:自动扫描串口号并添加到下拉框
 */
void Widget::timer_refreshSerialPorts() {
    // 如果串口已经打开，为了稳定性，通常不建议在此时刷新端口列表（或者跳过当前使用的端口）
    if (serial->isOpen()) return;

    const auto infos = QSerialPortInfo::availablePorts();

    // 只有当数量发生变化时才更新，减少 UI 闪烁
    if (infos.count() == ui->cbPort->count()) return;

    ui->cbPort->clear();//清除原先的串口号后重新刷新

    for (const QSerialPortInfo &info : infos) {
        ui->cbPort->addItem(info.portName());
        // qDebug() << info.portName();
    }
}

/**
 * @brief:定时器槽函数：模拟打包发送一个完整周期的 200 个数据包
 */
void Widget::timer_sendSimulatedData() {
    if (!serial->isOpen()) return;

    // 循环发送 200 个包，每个包 19 字节
    for (int p = 0; p < 200; ++p) {
        QByteArray frame;
        frame.append(0x55);
        frame.append(static_cast<char>(0xAA));  //0xAA显示截断，消除警告

        uint16_t adcVal = m_sinTable[p]; // 从预存表中取点

        for (int i = 0; i < 8; ++i) {
            frame.append(adcVal & 0xFF);         // 低字节在前小端
            frame.append((adcVal >> 8) & 0xFF);  // 高字节
        }

        frame.append(static_cast<char>(0x00)); // 校验位
        serial->write(frame);
    }
    // 提醒系统尽快刷入硬件缓冲区
    serial->flush();
}

/**
 * @brief:开关串口，开启后禁用设置
 */
void Widget::on_btnSwitchPort_clicked()
{
    if (serial->isOpen())
    {
        serial->close();
        qDebug() << "Serial Close Successfully!";
        ui->btnSwitchPort->setText("打开串口");
        // 允许修改配置
        ui->cbPort->setEnabled(true);
        ui->cbBaud->setEnabled(true);
    }
    else {
        // 从下拉框读取用户选中的参数
        serial->setPortName(ui->cbPort->currentText());
        serial->setBaudRate(ui->cbBaud->currentText().toInt());
        //8位数据位，无校验位，一位停止位
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);

        if (serial->open(QIODevice::ReadWrite)) //同时对这个串口 读 + 写
        {
            ui->btnSwitchPort->setText("关闭串口");
            qDebug() << "Serial open Successfully!";
            ui->cbPort->setEnabled(false); // 打开后禁用配置
            ui->cbBaud->setEnabled(false);
        } else {
            QMessageBox::critical(this, "错误", "无法打开串口，可能被占用！");
        }
    }
}

/**
 * @brief:
 */
void Widget::on_btnTest_clicked()
{
    if (!serial->isOpen()) return;

    if (timer_testData->isActive()) {
        timer_testData->stop();
        ui->btnTest->setText("发送测试");
    } else {
        timer_testData->start(200); // 设置为 200ms 触发一次
        ui->btnTest->setText("停止测试");
    }

}

/**
 * @brief:初始化customplot波形图
 * @note:黑色背景 + 黄色曲线 + 白色坐标轴 + 可拖动缩放 + OpenGL 加速
 */
void Widget::initMainPlot() {
    ui->customplot->setBackground(Qt::black);//黑色背景
    ui->customplot->setOpenGl(true);//开启OpenGL硬件渲染（显卡加速）

    ui->customplot->addGraph();//创建一条新的曲线对象
    ui->customplot->graph(0)->setPen(QPen(Qt::yellow, 3));//设置第一条曲线画笔颜色为黄色，宽度2像素

    // 新增：添加网格线（浅灰色，虚线）
    ui->customplot->xAxis->grid()->setPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->customplot->yAxis->grid()->setPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    //白色XY轴线
    ui->customplot->xAxis->setBasePen(QPen(Qt::white));
    ui->customplot->yAxis->setBasePen(QPen(Qt::white));
    //白色XY刻度文字
    ui->customplot->xAxis->setTickLabelColor(Qt::white);
    ui->customplot->yAxis->setTickLabelColor(Qt::white);
    // 抗锯齿
    ui->customplot->graph(0)->setAntialiased(true);
    // 设置初始视野
    ui->customplot->xAxis->setRange(0, 1000);   // 假设显示最近 100 个点
    ui->customplot->yAxis->setRange(-1000, 6000);   // 0-4096
    //允许用户通过鼠标拖动、滚轮缩放
    ui->customplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    //设置尺寸策略，确保其在布局中优先占用空间
    ui->customplot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // ui->customplot->setMinimumHeight(300);   // 像素下限
    // ui->customplot->setMaximumHeight(10000);

    // connect(ui->customplot, &QCustomPlot::mouseWheel, this, [this]() {
    //     ui->customplot->rescaleAxes();
    //     ui->customplot->replot();
    // });
}

/**
 * @brief:初始化8子图波形
 * @note:布局初始化、子图样式配置、交互控制、性能优化、点击事件绑定
 */
void Widget::initSubPlots() {
    // 获取subContainer设置的布局
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->subContainer->layout());
    if (!layout) {
        layout = new QVBoxLayout(ui->subContainer);
        ui->subContainer->setLayout(layout);
    }
    layout->setSpacing(2);// 子图之间的间距设为2像素
    layout->setContentsMargins(0, 0, 0, 0);// 布局内边距清零，节省空间

    for (int i = 0; i < 8; ++i) {
        QCustomPlot *plot = new QCustomPlot(this);
        // 子图高度固定值 + Fixed 策略
        // plot->setFixedHeight(70);
        // plot->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        // plot->setMinimumHeight(150);
        plot->setMinimumHeight(120);
        plot->setMaximumHeight(150);

        plot->setBackground(Qt::black);     // 黑色背景
        plot->setOpenGl(true);

        plot->setProperty("channelID", i);// 为每个子图设置一个属性
        connect(plot, &QCustomPlot::mousePress, this, &Widget::onSubPlotClicked);// 绑定子图点击事件

        plot->addGraph();
        plot->graph(0)->setPen(QPen(colorTable[i % 8], 3));

        plot->xAxis->setTicks(false);// 隐藏X轴刻度线
        plot->xAxis->setTickLabels(false);// 隐藏X轴刻度文字（简化子图显示）

        //X、Y轴初始范围
        plot->xAxis->setRange(0,1000);
        plot->yAxis->setRange(-1000, 6000);
        //X、Y轴刻度文字
        plot->xAxis->setTickLabelColor(Qt::white);
        plot->yAxis->setTickLabelColor(Qt::white);

        // 不响应任何鼠标拖拽或滚动缩放
        plot->setInteractions(QCP::iNone);

        // 放入布局并存入数组
        layout->addWidget(plot);
        m_subPlots.append(plot);
    }
    // 添加弹簧，防止子图在窗口拉大时被异常拉伸
    layout->addStretch();
}

/**
 * @brief: 单击子图切换主图绘制为点击的子图数据
 */
void Widget::onSubPlotClicked(QMouseEvent *event) {
    // 获取发送信号的子图指针
    QCustomPlot *clickedPlot = qobject_cast<QCustomPlot*>(sender());
    if(!clickedPlot) return;

    //只换了颜色，没把真正的数据搬过去，等待维护
    // 读取预设的 ID
    m_currentChannel = clickedPlot->property("channelID").toInt();
    // 可选：清空主图之前的历史数据，切换到新通道
    ui->customplot->graph(0)->data()->clear();
    // 更改主图曲线颜色与子图一致（如果需要）
    ui->customplot->graph(0)->setPen(clickedPlot->graph(0)->pen());

    qDebug() << "Switch Main Plot to Channel:" << m_currentChannel;

}

/**
 * @brief: 串口有数据时自动触发，解决粘包/断包问题
 */
void Widget::on_serialDataReady() {
    m_buffer.append(serial->readAll()); //接收到的数据全部放到缓冲区

    // 头对齐 + 定长方案：循环查找帧头 0x55 0xAA 并解析 19 字节包
    while (m_buffer.size() >= 19) {
        if (static_cast<uint8_t>(m_buffer[0]) == 0x55 &&
            static_cast<uint8_t>(m_buffer[1]) == 0xAA) {

            //取出 19 字节，交给解析函数
            QByteArray packet = m_buffer.mid(0, 19);
            processBinaryData(packet); // 调用解析
            m_buffer.remove(0, 19);    // 处理完后移出缓存
        } else {
            // 没找到帧头，逐字节滑动对齐
            m_buffer.remove(0, 1);
        }
    }
}
/**
 * @brief
 * @param
 */
void Widget::processBinaryData(const QByteArray &data) {
    static double xCount = 0;
    xCount += 1.0; // X 轴坐标，按点数增加

    // 1. 解析 8 路数据并刷新右侧子图
    for (int i = 0; i < 8; ++i) {
        // 假设小端字节序：低位在前，高位在后
        uint16_t low  = static_cast<uint8_t>(data[2 + i*2]);
        uint16_t high = static_cast<uint8_t>(data[3 + i*2]);
        uint16_t rawVal = (low | (high << 8)) & 0x0FFF; //只取 12 位 ADC

        float voltage = (static_cast<double>(rawVal) * m_gains[i]) + m_offsets[i];
        double val = static_cast<double>(low | (high << 8));

        //子图范围x轴4000点
        m_subPlots[i]->graph(0)->addData(xCount, voltage);
        m_subPlots[i]->xAxis->setRange(xCount, 4000, Qt::AlignRight);
        // 2. 如果当前循环的索引 i 等于选中的通道，则更新主图
        if (i == m_currentChannel) {
            ui->customplot->graph(0)->addData(xCount, val);
            ui->customplot->xAxis->setRange(xCount, 1000, Qt::AlignRight); // 主图显示点数可以多一点
            ui->customplot->replot(QCustomPlot::rpQueuedReplot);
        }
    }
        // 建议：在解析完所有路数据后，或者使用定时器统一刷新
        // 如果一定要在这里刷，请务必使用 QueuedReplot
        ui->customplot->replot(QCustomPlot::rpQueuedReplot);
        for(auto p : m_subPlots) p->replot(QCustomPlot::rpQueuedReplot);

}
