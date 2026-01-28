#include "vna.h"

Vna::Vna() : _socket(new QTcpSocket(this))
{
    QObject::connect(_socket, &QTcpSocket::readyRead, this, &Vna::onReadyRead);
    QObject::connect(_socket, &QTcpSocket::errorOccurred, this, &Vna::onError);

    QObject::connect(_socket, &QTcpSocket::connected, this, &Vna::onConnected);
    QObject::connect(_socket, &QTcpSocket::disconnected, this, &Vna::onDisconnected);
}

void Vna::connect()
{
    _socket->connectToHost("127.0.0.1", 5025);
}

void Vna::disconnect()
{
    Q_FUNC_INFO;

    _socket->disconnectFromHost();
}

void Vna::sendCommand(const QString &scpi)
{
    if (_socket->state() == QAbstractSocket::ConnectedState) {
        QByteArray msg = scpi.toUtf8();
        msg.append("\n");
        _socket->write(msg);
    }
}

void Vna::onReadyRead()
{
    QString data = QString::fromUtf8(_socket->readAll());

    qInfo() << data;
}

void Vna::onError(QAbstractSocket::SocketError)
{
    qInfo() << _socket->errorString();
}

void Vna::onConnected()
{
    qInfo() << "connected";
}

void Vna::onDisconnected()
{
    qInfo() << "disconnected";
}
