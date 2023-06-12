#pragma once

#include <QObject>
#include "api/candle.h"

class CandleApiDevices;

class CandleApiInterface: public QObject
{
public:
    CandleApiInterface(candle_handle handle, QString name);
    virtual ~CandleApiInterface();

    std::wstring getPath() const;
    void update(candle_handle handle, QString name);
    QString getName() const;
    candle_handle getHandle() const;

private:
    candle_handle _handle;
    QString _name;
};
