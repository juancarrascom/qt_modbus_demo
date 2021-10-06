#include "modbusMaster.h"
#include <QCoreApplication>
#include <QLoggingCategory>
#include <QTimer>


int main(int argc, char *argv[])
{
//    QLoggingCategory::setFilterRules(QStringLiteral("qt.modbus* = true"));

    QCoreApplication app(argc, argv);

    QString portName = "/dev/ttySTM2";

    modbusMaster *m_master = new  modbusMaster();
    m_master->initModbusSerialMaster(portName,QSerialPort::Baud115200,QSerialPort::Data8,QSerialPort::NoParity,QSerialPort::OneStop);
    m_master->connectDevice();
    QTimer tocTimer;
    tocTimer.setInterval(1000);
    tocTimer.setSingleShot(false);

    QObject::connect(&tocTimer, &QTimer::timeout, [&m_master]() {
        m_master->readRegisterData(0,50,1);
    });
        tocTimer.start();

//    m_master->disconnectDevice();
    return app.exec();

}
