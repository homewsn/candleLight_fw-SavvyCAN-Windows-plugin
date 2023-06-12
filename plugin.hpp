#pragma once

#include <QObject>
#include <QtSerialBus/QCanBusFactoryV2>
#include <QtSerialBus/QCanBusDevice>

class CandleApiInterface;

class CandleLightCanBusPlugin : public QObject, public QCanBusFactoryV2
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCanBusFactory" FILE "plugin.json")
    Q_INTERFACES(QCanBusFactoryV2)

    friend class QCanBusDevice;

public:
    QList<QCanBusDeviceInfo> availableDevices(QString *errorMessage) const override;
    QCanBusDevice *createDevice(const QString &interfaceName, QString *errorMessage) const override;

private:
    static QList<QCanBusDeviceInfo> getInterfaces();
    static QList<CandleApiInterface *> _ifaces;
};
