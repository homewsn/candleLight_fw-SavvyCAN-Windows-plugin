/*
* Copyright (c) 2023 Vladimir Alemasov
* All rights reserved
*
* This program and the accompanying materials are distributed under
* the terms of GNU General Public License version 2
* as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

#include "plugin.hpp"
#include "device.hpp"
#include <QString>
#include <QtPlugin>
#include <QtCore/QList>

QList<QCanBusDeviceInfo> CandleLightCanBusPlugin::availableDevices(QString *errorMessage) const
{
    Q_UNUSED(errorMessage);
    return CandleApiDevices::interfaces();
}

QCanBusDevice *CandleLightCanBusPlugin::createDevice(const QString &interfaceName, QString *errorMessage) const
{
    Q_UNUSED(errorMessage);
    auto const info = CandleLightCanBusPlugin::availableDevices(errorMessage);
    for (auto const& entry: info)
    {
        if(entry.name() == interfaceName)
        {
            return new CandleApiDevice(interfaceName);
        }
    }
    return nullptr;
}
