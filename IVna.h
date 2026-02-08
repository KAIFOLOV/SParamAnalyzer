#ifndef IVNA_H
#define IVNA_H

#include <QObject>

class IVna : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Status connectStatus READ connectStatus NOTIFY connectStatusChanged FINAL)

public:
    enum class Status
    {
        Closed = 0,
        Opened,
        Wait
    };
    Q_ENUM(Status)

    virtual ~IVna() = default;

    Q_SIGNAL void error(const QString &error);

    Q_INVOKABLE virtual void connect() = 0;
    Q_INVOKABLE virtual void disconnect() = 0;

    virtual Status connectStatus() = 0;
    Q_SIGNAL void connectStatusChanged(const Status status);

    virtual bool send(const QByteArray &command) = 0;
    virtual bool query(const QByteArray &command, QByteArray &result) = 0;

    Q_INVOKABLE virtual bool isOpen() const = 0;

    virtual bool OPC() = 0;
    virtual QString IDN() = 0;
    virtual bool systemError(QByteArray &errResponse) = 0;

    virtual bool startMeasure() = 0;
    virtual bool switchStateContinuousStart(const bool state, const int channel = 1) = 0;

    virtual bool setFormat(const QString &format, const int channel = 1, const int trace = 1) = 0;
    virtual QVector<double> getData(const int channel = 1, const int trace = 1) = 0;

    virtual bool setStartFreq(const double value, const int channel = 1) = 0;
    virtual double getStartFreq(const int channel = 1) = 0;

    virtual bool setStopFreq(const double value, const int channel = 1) = 0;
    virtual double getStopFreq(const int channel = 1) = 0;

    virtual bool setPointsCount(const int value, const int channel = 1) = 0;
    virtual int getPointsCount(const int channel = 1) = 0;

    virtual bool setOutputPower(const float power) = 0;
    virtual float getOutputPower() = 0;

    virtual bool setFilterPch(const double value, const int channel = 1) = 0;
    virtual double getFilterPch(const int channel = 1) = 0;
};

#endif // IVNA_H
