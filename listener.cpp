#include "listener.hpp"
#include "device.hpp"
#include <QThread>
#include <QtCore/QList>
#include <QtSerialBus/QCanBusDevice>

CandleApiListener::CandleApiListener(CandleApiDevice &dev) :
    _shouldBeRunning(false),
    _dev(dev)
{
    _thread = new QThread();
}

CandleApiListener::~CandleApiListener()
{
    delete _thread;
}

void CandleApiListener::run()
{
    _dev.readFrames();
    _thread->quit();
}

void CandleApiListener::startThread()
{
    _shouldBeRunning = true;
    moveToThread(_thread);
    connect(_thread, SIGNAL(started()), this, SLOT(run()));
    _thread->start();
}

void CandleApiListener::stopThread()
{
    if (_shouldBeRunning)
    {
        _shouldBeRunning = false;
        _thread->wait();
    }
}
