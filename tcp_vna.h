#ifndef TCP_VNA_H
#define TCP_VNA_H

#include "IVna.h"

#include <QMutex>
#include <QPointer>
#include <QTcpSocket>

class TcpVna : public IVna
{
    Q_OBJECT

    Q_PROPERTY(QString ip READ ip WRITE setIp NOTIFY ipChanged FINAL)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged FINAL)

public:
    explicit TcpVna();
    ~TcpVna();

    void connect() override;
    void disconnect() override;

    Status connectStatus() override;

    bool send(const QByteArray &command) override;
    bool query(const QByteArray &command, QByteArray &result) override;

    bool isOpen() const override;

    bool OPC() override;
    QString IDN() override;
    bool systemError(QByteArray &errResponse) override;

    bool startMeasure() override;
    bool switchStateContinuousStart(const bool state, const int channel = 1) override;

    bool setFormat(const QString &format, const int channel = 1, const int trace = 1) override;
    QVector<double> getData(const int channel = 1, const int trace = 1) override;

    bool setStartFreq(const double value, const int channel = 1) override;
    double getStartFreq(const int channel = 1) override;

    bool setStopFreq(const double value, const int channel = 1) override;
    double getStopFreq(const int channel = 1) override;

    bool setPointsCount(const int value, const int channel = 1) override;
    int getPointsCount(const int channel = 1) override;

    bool setOutputPower(const float power) override;
    float getOutputPower() override;

    bool setFilterPch(const double value, const int channel = 1) override;
    double getFilterPch(const int channel = 1) override;

    QString ip() const;
    void setIp(const QString &newIp);
    Q_SIGNAL void ipChanged();

    int port() const;
    void setPort(int newPort);
    Q_SIGNAL void portChanged();

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

    QPointer<QTcpSocket> _socket;

    QByteArray _buffer;
};

#endif // TCP_VNA_H
