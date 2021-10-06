#include "modbusMaster.h"
#include "timerRequest.h"
#include <QDebug>



timerRequest::timerRequest()
{


    connect(&timer, SIGNAL(timeout()), this, SLOT(expired()));
    timer.setSingleShot(false);
    timer.setInterval(5000);
    timer.start();
}

void timerRequest::expired()
{


    modbusMaster *m_master = new modbusMaster();
    QString portName = "ttySTM2";

    m_master->initModbusSerialMaster(portName,QSerialPort::Baud115200,QSerialPort::Data8,QSerialPort::NoParity,QSerialPort::OneStop);
    m_master->connectDevice();
    m_master->readRegisterData(0x00,3,0x01);

}
