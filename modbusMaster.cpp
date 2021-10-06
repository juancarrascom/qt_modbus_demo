#include "modbusMaster.h"
#include <QDebug>


modbusMaster::modbusMaster(QObject *parent) : QObject(parent)
{
    qDebug()<<"Init mosbusMaster Class";
}

bool modbusMaster::initModbusSerialMaster(QString portName, QSerialPort::BaudRate baudRate,
                                         QSerialPort::DataBits dataBits,
                                         QSerialPort::Parity parity,
                                         QSerialPort::StopBits stopBits)
{

    m_master = new QModbusRtuSerialMaster(this);


    if(!m_master)
    {
        qDebug()<<"init master error";
        return 0;
    }

    m_master->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
                                    portName);
    m_master->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
                                    baudRate);
    m_master->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
                                    dataBits);
    m_master->setConnectionParameter(QModbusDevice::SerialParityParameter,
                                    parity);
    m_master->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
                                    stopBits);

    return 1;

}

bool modbusMaster::connectDevice()
{
    m_master->connectDevice();
    return 1;
}

bool modbusMaster::disconnectDevice()
{
    m_master->disconnectDevice();
}

bool modbusMaster::writeRegisterData(QString str, int startAddress, int num)
{
    QByteArray byte = QByteArray::fromHex(str.toLatin1().data());
    QString hexStr = byte.toHex().data();

    // enviar información de datos
    QModbusDataUnit writeUnit(QModbusDataUnit::HoldingRegisters
                              ,startAddress,num);

         // el tipo de registro recibido a la tabla
    QModbusDataUnit::RegisterType table = writeUnit.registerType();

    for(int i = 0;i < writeUnit.valueCount(); i++)
    {
        int j = 2*i;
        QString str = hexStr.mid(j,2);

        bool ok;
        int hex = str.toInt(&ok,16);

        writeUnit.setValue(i,hex);
    }

    if(auto* reply = m_master->sendWriteRequest(writeUnit,0x01))
    {
        if(!reply->isFinished())
        {
            connect(reply,&QModbusReply::finished,this,[this,reply]()
            {
                if(reply->error() == QModbusDevice::ProtocolError)
                {
                    qDebug()<<"write response Protocol Error:"<<reply->errorString();
                    return 0;
                }
                else if(reply->error() != QModbusDevice::NoError)
                {
                    qDebug()<<"write response error:"<<reply->errorString();
                    return 0;
                }

                reply->deleteLater();
            });
        }
        else
        {
            reply->deleteLater();
            return 1;
        }
    }
    else
    {
        qDebug()<<"write error: "<<m_master->errorString();
        return 0;
    }


}

void modbusMaster::readRegisterData(quint16 startAddress, quint16 num, quint16 slaveAddress)
{
    if(!m_master)
        return;

    m_master->setTimeout(10);
    qDebug()<<m_master->timeout();

    qDebug()<<"Read Register Data";

    QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters,startAddress,num);

    if(auto *reply = m_master->sendReadRequest(readUnit,slaveAddress))
    {

        if(!reply->isFinished())
            QObject::connect(reply,&QModbusReply::finished,this,&modbusMaster::readyRead);
        else
            delete reply;
    }
    else
    {
        qDebug()<<"Read error: "<<m_master->errorString();
    }
}

void modbusMaster::readyRead()
{
    qDebug()<<"Ready Read";

    auto reply = qobject_cast<QModbusReply*>(sender());
    if(!reply)
        return;

    qDebug()<<reply->rawResult();
    QList<uint8_t> sensor_value;


    if(reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();

        for(uint i=0; i< unit.valueCount();i++)
        {
           const QString entry = tr("Address: %1, Value: %2").arg(unit.startAddress() + i)
                                         .arg(QString::number(unit.value(i),
                                              unit.registerType() <= QModbusDataUnit::Coils ? 10 : 16));

           sensor_value.append(unit.value(i));
//           qDebug()<<entry;
        }
        qDebug()<<sensor_value;
    }
    else if (reply->error() == QModbusDevice::ProtocolError) {
        qDebug()<<"Protocol Error: "<<reply->errorString();
        qDebug()<<"Modbus Exception: "<<reply->rawResult().exceptionCode();
    }
    else {
        qDebug()<<"Read response error:"<<reply->errorString();
        qDebug()<<"code: "<<reply->error();
    }

    reply->deleteLater();

}






















