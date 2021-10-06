#ifndef MODBUSMASTER_H
#define MODBUSMASTER_H

#include <QModbusClient>
#include <QModbusDataUnit>
#include <QObject>
#include <QSerialPort>
#include <QModbusRtuSerialMaster>
#include <QModbusTcpClient>
#include <QThread>
#include <QTimer>

class modbusMaster : public QObject
{
    Q_OBJECT
public:
    explicit modbusMaster(QObject *parent = nullptr);

    enum modbusConnection
        {
            Serial,
            Tcp
        };

    bool initModbusSerialMaster(QString portName, QSerialPort::BaudRate baudRate, QSerialPort::DataBits dataBits,
                         QSerialPort::Parity parity, QSerialPort::StopBits stopBits);


    bool connectDevice();
    bool disconnectDevice();

    bool writeRegisterData(QString str,int startAddress,int num);

    void readRegisterData(quint16 startAddress,quint16 num, quint16 slaveAddress);

public slots:

    void readyRead();

private:
    QModbusClient* m_master;
//    QModbusRtuSerialMaster* m_master;
    modbusConnection m_mode;

    QSerialPort* cmdSerial;
};


#endif // MODBUSMASTER_H
