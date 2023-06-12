#pragma once

#include <QtSerialBus/QCanBusDevice>
#include "api/candle.h"

class CandleApiListener;
class CandleApiInterface;
class CandleApiTiming;

class CandleApiDevices : public QCanBusDevice
{
public:
    static QList<QCanBusDeviceInfo> interfaces();
    static CandleApiInterface *findInterface(QString name);

private:
    static CandleApiInterface *findInterface(candle_handle dev);
    static void deleteAllInterfaces();
    static QList<CandleApiInterface *> getInterfaces();
    static void addInterface(CandleApiInterface *iface);

private:
    static QList<CandleApiInterface *> _ifaces;
protected:
    static QList<CandleApiTiming> _timings;
};
