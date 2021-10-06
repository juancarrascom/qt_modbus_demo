#ifndef MAIN_H
#define MAIN_H

#include <QModbusDataUnit>

QT_BEGIN_NAMESPACE

class QModbusClient;
class QModbusReply;

namespace Ui {
class MainWindow;
class SettingsDialog;
}

class SettingsDialog;
class WriteRegisterModel;

class Main
{
    Q_OBJECT

private:
    void initActions();
    QModbusDataUnit readRequest() const;
    QModbusDataUnit writeRequest() const;

private slots:
    void onConnectButtonClicked();
    void onModbusStateChanged(int state);

    void onReadButtonClicked();
    void onReadReady();

    void onWriteButtonClicked();
    void onReadWriteButtonClicked();

    void onConnectTypeChanged(int);
    void onWriteTableChanged(int);

private:
    QModbusReply *lastRequest = nullptr;
    QModbusClient *modbusDevice = nullptr;
    SettingsDialog *m_settingsDialog = nullptr;
    WriteRegisterModel *writeModel = nullptr;
};

#endif // MAIN_H
