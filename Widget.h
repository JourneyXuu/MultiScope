#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QVector>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void initControlPanel(void);

    void timer_refreshSerialPorts(void);    //自动扫描串口号
    void timer_sendSimulatedData(void);     //200ms 10个周期发送一次完整的200个数据包

    void initMainPlot(void);   // 初始化主图
    void initSubPlots();   // 初始化子图
    // void initSerialPort(); // 初始化串口

private slots:
    void on_btnSwitchPort_clicked();
    void on_btnTest_clicked();
    void onSubPlotClicked(QMouseEvent *event); // 处理子图点击的槽
    void on_serialDataReady(); // 处理串口到达信号

private:
    Ui::Widget *ui;

    QSerialPort *serial;
    QTimer *timer_serialRefresh;//定时刷新定时器
    QTimer *timer_testData;
    QVector<QCustomPlot*> m_subPlots; // 存放右侧 8 个子图

    int m_currentChannel = 0; // 默认主图显示第 0 路（第一路）

    QByteArray m_buffer; // 用于暂存未处理的串口字节流
    void processBinaryData(const QByteArray &data); // 解析 19 字节帧

    double m_gains[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    double m_offsets[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    const QColor colorTable[8] = {
        Qt::red,
        Qt::green,
        Qt::blue,
        Qt::yellow,
        Qt::cyan,
        Qt::magenta,
        Qt::darkGray,
        Qt::darkYellow
    };
    QVector<uint16_t> m_sinTable;

};
#endif // WIDGET_H
