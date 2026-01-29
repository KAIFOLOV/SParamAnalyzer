#ifndef IVNA_H
#define IVNA_H

#include <QObject>

class IVna : public QObject
{
    Q_OBJECT

public:
    virtual ~IVna() = default;

    Q_INVOKABLE virtual void connect() = 0;
    Q_INVOKABLE virtual void disconnect() = 0;

    virtual bool send(const QByteArray &command) = 0;
    virtual bool query(const QByteArray &command, QByteArray &result) = 0;

    virtual bool isOpen() const = 0;

    virtual bool OPC() = 0;
    virtual QString IDN() = 0;
    virtual bool systemError(QByteArray &errResponse) = 0;

    virtual void startMeasure() = 0;
    virtual void switchStateContinuousStart(const bool state) = 0;

    virtual QVector<double> getData(const int channel = 1, const int trace = 1) = 0;

    virtual void setStartFreq(const double value) = 0;
    virtual double getStartFreq() = 0;

    virtual void setStopFreq(const double value) = 0;
    virtual double getStopFreq() = 0;

    virtual void setPointsCount(const int value) = 0;
    virtual int getPointsCount() = 0;

    virtual void setOutputPower(const int power) = 0;
    virtual double getOutputPower() = 0;

    virtual void setFilterPch(const int value) = 0;
    virtual double getFilterPch() = 0;
};

#endif // IVNA_H
