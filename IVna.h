#ifndef IVNA_H
#define IVNA_H

#include <QObject>

class IVna : public QObject
{
    Q_OBJECT

public:
    enum class Status
    {
        Opened,
        Closed,
        Wait
    };

    virtual ~IVna() = default;

    virtual void connect() = 0;
    virtual void disconnect() = 0;

    Q_SIGNAL void connectStatusChanged(const Status status);

    virtual bool send(const QByteArray &command) = 0;
    virtual bool query(const QByteArray &command, QByteArray &result) = 0;

    virtual bool isOpen() const = 0;

    virtual bool OPC() = 0;
    virtual QString IDN() = 0;
    virtual bool systemError(QByteArray &errResponse) = 0;

    virtual void startMeasure() = 0;
    virtual void switchStateContinuousStart(const bool state, const int channel = 1) = 0;

    virtual void setFormat(const QString &format, const int channel = 1, const int trace = 1) = 0;
    virtual QVector<double> getData(const int channel = 1, const int trace = 1) = 0;

    virtual void setStartFreq(const double value, const int channel = 1) = 0;
    virtual double getStartFreq(const int channel = 1) = 0;

    virtual void setStopFreq(const double value, const int channel = 1) = 0;
    virtual double getStopFreq(const int channel = 1) = 0;

    virtual void setPointsCount(const int value, const int channel = 1) = 0;
    virtual int getPointsCount(const int channel = 1) = 0;

    virtual void setOutputPower(const float power) = 0;
    virtual float getOutputPower() = 0;

    virtual void setFilterPch(const double value, const int channel = 1) = 0;
    virtual double getFilterPch(const int channel = 1) = 0;
};

#endif // IVNA_H
