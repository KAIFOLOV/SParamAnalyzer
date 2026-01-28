#ifndef VNA_H
#define VNA_H

#include "IVna.h"

#include <QTcpSocket>

class Vna : public IVna
{
    Q_OBJECT

public:
    explicit Vna();

    void connect() override;
    void disconnect() override;
    void sendCommand(const QString &scpi) override;

private slots:
    void onError(QAbstractSocket::SocketError);
    void onReadyRead();

    void onConnected();
    void onDisconnected();

private:
    QTcpSocket *_socket;
};

#endif // VNA_H
