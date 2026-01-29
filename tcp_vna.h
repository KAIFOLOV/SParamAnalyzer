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

    Q_SIGNAL void connectStatusChanged();

    void connect() override;
    void disconnect() override;

    virtual bool send(const QByteArray &command) override;
    virtual bool query(const QByteArray &command, QByteArray &result) override;

    bool isOpen() const override;

private:
    Q_SIGNAL void sendToSocket(const QByteArray &command);
    Q_SIGNAL void commandRecived();

    Q_SLOT void onReadyRead();
    Q_SLOT void write(const QByteArray &command);
    Q_SLOT void onError(QAbstractSocket::SocketError);
    Q_SLOT void statusChange(QAbstractSocket::SocketState state);

private:
    QString _ip {};
    int _port { 0 };

    /// мс
    int _timeout { 5000 };

    QTcpSocket *_socket;

    QByteArray _buffer;

    QMutex _mutex;
};

#endif // TCP_VNA_H
