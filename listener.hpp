#pragma once

#include <QObject>

class CandleApiDevice;

class CandleApiListener : public QObject
{
    Q_OBJECT

public:
    explicit CandleApiListener(CandleApiDevice &dev);
    virtual ~CandleApiListener();

public slots:
    void run();
    void startThread();
    void stopThread();

public:
    bool _shouldBeRunning;

private:
    CandleApiDevice &_dev;
    QThread *_thread;
};
