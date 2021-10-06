#ifndef TIMERREQUEST_H
#define TIMERREQUEST_H

#include <QTimer>
#include <QObject>

class timerRequest:public QObject
{
    Q_OBJECT

public:
    timerRequest();

public slots:
    void expired();

private:
    QTimer timer;
};

#endif // TIMERREQUEST_H
