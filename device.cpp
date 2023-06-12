#include "device.hpp"
#include "interface.hpp"
#include "listener.hpp"
#include "timing.hpp"
#include <QQueue>
#include <QDate>
#include <QDataStream>
#include <QDebug>
#include "api/candle.h"

CandleApiDevice::CandleApiDevice(QString name)
{
    qDebug() << "New CandleApiDevice is created:" << name;
    _iface = findInterface(name);
    _hostOffsetStart = QDateTime::currentMSecsSinceEpoch();
}

CandleApiDevice::~CandleApiDevice()
{
    qDebug() << "CandleApiDevice is destoyed.";
}

bool CandleApiDevice::open()
{
    setState(CanBusDeviceState::ConnectingState);
    qDebug() << "CandleApiDevice::open";

    _lstn = new CandleApiListener(*this);
    connect(_lstn, SIGNAL(readyRead()), this, SLOT(dataAvailable()));

    candle_handle handle = _iface->getHandle();
    uint32_t flags = 0;

    if (!candle_dev_open(handle))
    {
        setState(CanBusDeviceState::UnconnectedState);
        return false;
    }

    QVector<int> keys = configurationKeys();
    foreach (int key, keys)
    {
        if (key == QCanBusDevice::BitRateKey)
        {
            QVariant var = configurationParameter(key);
            bool ok;
            uint32_t bitrate = var.toInt(&ok);
            if (ok)
            {
                if (!setBitTiming(bitrate, 875))
                {
                    close();
                    return false;
                }
            }
        }
    }

    uint32_t t_dev;
    if (candle_dev_get_timestamp_us(handle, &t_dev))
    {
        _deviceTicksStart = t_dev;
    }

    candle_channel_start(handle, 0, flags);

    _lstn->startThread();

    setState(CanBusDeviceState::ConnectedState);
    return true;
}

void CandleApiDevice::close()
{
    setState(CanBusDeviceState::ClosingState);
    qDebug() << "CandleApiDevice::close";
    candle_handle handle = _iface->getHandle();
    candle_channel_stop(handle, 0);

    _lstn->stopThread();
    delete _lstn;

    setState(CanBusDeviceState::UnconnectedState);
    candle_dev_close(handle);
}

bool CandleApiDevice::writeFrame(const QCanBusFrame &msg)
{
    candle_frame_t frame;
    frame.can_id = msg.frameId() & 0x1FFFFFFF; //??

    qDebug() << "CandleApiDevice::writeFrame";
    switch (msg.frameType())
    {
    case QCanBusFrame::DataFrame:
        break;
    case QCanBusFrame::RemoteRequestFrame:
        frame.can_id |= CANDLE_ID_RTR;
        break;
    case QCanBusFrame::ErrorFrame:
        frame.can_id |= CANDLE_ID_ERR;
        break;
    default:
        qDebug() << "CandleApiDevice::writeFrame -> unknown or error frame.";
        return true;
    }

    if (msg.hasExtendedFrameFormat())
    {
        frame.can_id |= CANDLE_ID_EXTENDED;
    }

    frame.can_dlc = msg.payload().length();
    for (int cnt = 0; cnt < 8 && cnt < frame.can_dlc; cnt++)
    {
        frame.data[cnt] = msg.payload().at(cnt);
    }

    candle_handle handle = _iface->getHandle();
    if (!candle_frame_send(handle, 0, &frame))
    {
        return false;
    }
    return true;
}

QString CandleApiDevice::interpretErrorFrame(const QCanBusFrame &errorFrame)
{
    Q_UNUSED(errorFrame);
    qDebug() << "CandleApiDevice::interpretErrorFrame";
    return QString();
}

bool CandleApiDevice::setBitTiming(uint32_t bitrate, uint32_t samplePoint)
{
    candle_handle handle = _iface->getHandle();

    candle_capability_t caps;
    if (!candle_channel_get_capabilities(handle, 0, &caps))
    {
        return false;
    }

    foreach (const CandleApiTiming t, _timings)
    {
        if ( (t.getBaseClk() == caps.fclk_can)
          && (t.getBitrate()==bitrate)
          && (t.getSamplePoint()==samplePoint) )
        {
            candle_bittiming_t timing = t.getTiming();
            return candle_channel_set_timing(handle, 0, &timing);
        }
    }

    // no valid timing found
    return false;
}

void CandleApiDevice::readFrames()
{
    candle_frame_t frame;
    unsigned int timeout_ms = 10;
    candle_handle handle = _iface->getHandle();
    QVector<QCanBusFrame> msg_list;

    while (_lstn->_shouldBeRunning)
    {
        if (candle_frame_read(handle, &frame, timeout_ms))
        {
            if (candle_frame_type(&frame) == CANDLE_FRAMETYPE_RECEIVE)
            {
                QCanBusFrame msg;

                if (candle_frame_is_rtr(&frame))
                {
                    msg.setFrameType(QCanBusFrame::RemoteRequestFrame);
                }
                else
                {
                    msg.setFrameType(QCanBusFrame::DataFrame);
                }
                if (candle_frame_is_extended_id(&frame))
                {
                    msg.setExtendedFrameFormat(true);
                }
                else
                {
                    msg.setExtendedFrameFormat(false);
                }
                msg.setFrameId(candle_frame_id(&frame) & 0x1FFFFFFF); //??

                uint8_t dlc = candle_frame_dlc(&frame);
                QByteArray payload = QByteArray(reinterpret_cast<const char*>(candle_frame_data(&frame)), dlc);
                msg.setPayload(payload);

                uint32_t dev_ts = candle_frame_timestamp_us(&frame) - _deviceTicksStart;
                uint64_t ts_us = 0/*_hostOffsetStart*/ + dev_ts;
                uint64_t us_since_start = QDateTime::currentMSecsSinceEpoch() - _hostOffsetStart;
                if (us_since_start > 0x180000000)
                {
                    // device timestamp overflow
                    ts_us += us_since_start & 0xFFFFFFFF00000000;
                }
                auto ts = QCanBusFrame::TimeStamp(ts_us/1000000, ts_us % 1000000);
                msg.setTimeStamp(ts);

                msg_list.append(msg);
                enqueueReceivedFrames(msg_list);
                msg_list.clear();
            }
        }
    }
}
