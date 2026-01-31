#ifndef TCP_VNA_H
#define TCP_VNA_H

#include "IVna.h"

#include <QTcpSocket>
#include <QMutex>

class TcpVna : public IVna
{
    Q_OBJECT

public:
    explicit TcpVna();
    ~TcpVna();

    void connect() override;
    void disconnect() override;

    bool send(const QByteArray &command) override;
    bool query(const QByteArray &command, QByteArray &result) override;

    bool isOpen() const override;

    bool OPC() override;
    QString IDN() override;
    bool systemError(QByteArray &errResponse) override;

    void startMeasure() override;
    void switchStateContinuousStart(const bool state, const int channel = 1) override;

    void setFormat(const QString &format, const int channel = 1, const int trace = 1) override;
    QVector<double> getData(const int channel = 1, const int trace = 1) override;

    void setStartFreq(const double value, const int channel = 1) override;
    double getStartFreq(const int channel = 1) override;

    void setStopFreq(const double value, const int channel = 1) override;
    double getStopFreq(const int channel = 1) override;

    void setPointsCount(const int value, const int channel = 1) override;
    int getPointsCount(const int channel = 1) override;

    void setOutputPower(const float power) override;
    float getOutputPower() override;

    void setFilterPch(const double value, const int channel = 1) override;
    double getFilterPch(const int channel = 1) override;

private:
    Q_SIGNAL void sendToSocket(const QByteArray &command);
    Q_SIGNAL void commandRecived();

    Q_SLOT void onReadyRead();
    Q_SLOT void write(const QByteArray &command);
    Q_SLOT void onError(QAbstractSocket::SocketError);
    Q_SLOT void statusChanged(QAbstractSocket::SocketState state);

private:
    QString _ip { "127.0.0.1" };
    int _port { 5025 };

    /// мс
    int _timeout { 5000 };

    QTcpSocket *_socket;

    QByteArray _buffer;

    QMutex _mutex;
};

#endif // TCP_VNA_H
