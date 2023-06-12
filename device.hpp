#pragma once

#include <QtSerialBus/QCanBusDevice>
#include "devices.hpp"

class CandleApiListener;

class CandleApiDevice : public CandleApiDevices
{
public:
    explicit CandleApiDevice(QString name);
    virtual ~CandleApiDevice();

public:
    bool open() override;
    void close() override;
    bool writeFrame(const QCanBusFrame &msg) override;
    QString interpretErrorFrame(const QCanBusFrame &errorFrame) override;

    bool setBitTiming(uint32_t bitrate, uint32_t samplePoint);
    void readFrames();

private:
    CandleApiListener *_lstn;
    CandleApiInterface *_iface;
    uint32_t _deviceTicksStart;
    uint64_t _hostOffsetStart;
};
