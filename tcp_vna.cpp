#include "tcp_vna.h"

#include <QEventLoop>
#include <QTimer>

TcpVna::TcpVna() : _socket(new QTcpSocket(this))
{
    QObject::connect(_socket, &QTcpSocket::readyRead, this, &TcpVna::onReadyRead);
    QObject::connect(_socket, &QTcpSocket::errorOccurred, this, &TcpVna::onError);
    connect(_socket, &QTcpSocket::stateChanged, this, &AbstractTCPDevice::statusChange);

    QObject::connect(this, &TcpVna::sendToSocket, this, &TcpVna::write, Qt::QueuedConnection);
}

TcpVna::~TcpVna()
{
    disconnect();
}

void TcpVna::connect()
{
    if (isOpen()) {
        // emit connectStatusChanged(DeviceConnect::Status::Opened);
        return;
    }
    _socket->connectToHost("127.0.0.1", 5025);

    if (!_socket->waitForConnected()) {
        // emit errorMessage(tr("Failed to connect to server"));
        return;
    }

    // emit connectStatusChanged(DeviceConnect::Status::Opened);
}

void TcpVna::disconnect()
{
    if (_socket && (isOpen() || _socket->state() == QTcpSocket::ConnectingState)) {
        _socket->disconnectFromHost();
        if (_socket->state() != QTcpSocket::UnconnectedState) {
            _socket->waitForDisconnected();
        }
    }

    // emit connectStatusChanged(DeviceConnect::Status::Closed);
}

bool TcpVna::send(const QByteArray &command)
{
    try {
        QMutexLocker lock(&_mutex);
        emit sendToSocket(command);
    } catch (const std::exception &e) {
        // emit errorMessage(e.what());
        return false;
    }

    return true;
}

bool TcpVna::query(const QByteArray &command, QByteArray &result)
{
    QMutexLocker lock(&_mutex);
    _buffer.clear();

    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    timer.setInterval(_timeout);

    bool responseReceived = false;

    QObject::connect(&timer, &QTimer::timeout, &loop, [&]() {
        loop.quit();
    });

    QObject::connect(this, &TcpVna::commandRecived, &loop, [&]() {
        result = _buffer;
        responseReceived = true;
        loop.quit();
    });

    emit sendToSocket(command);

    timer.start();
    loop.exec();

    return responseReceived;
}

void TcpVna::write(const QByteArray &command)
{
    qInfo() << "TCP Device send: " << QString::fromUtf8(command);

    if (!isOpen()) {
        // emit connectStatusChanged(DeviceConnect::Status::Closed);
        // emit errorMessage(QString(tr("Не удалось отправить команду. Устройство не подключено")));
        return;
    }

    qint64 bytesWritten = _socket->write(command.data());

    if (bytesWritten != command.size()) {
        // emit errorMessage(tr("Ошибка отправки сообщения, записано %1 из %2 байт")
        // .arg(bytesWritten)
        // .arg(command.size()));
        return;
    }

    if (bytesWritten == -1) {
        // emit errorMessage("Ошибка записи данных в буфер:" % _tcpSocket->errorString());
        return;
    }

    if (!_socket->waitForBytesWritten(1000)) {
        // emit errorMessage("Ошибка отправки данных:" % _socket->errorString());
        return;
    }
}

void TcpVna::statusChange(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::ConnectedState: {
        emit connectStatusChanged(DeviceConnect::Status::Opened);
    } break;
    case QAbstractSocket::ConnectingState:
    case QAbstractSocket::HostLookupState: {
        emit connectStatusChanged(DeviceConnect::Status::Wait);
    } break;
    case QAbstractSocket::UnconnectedState: {
        emit connectStatusChanged(DeviceConnect::Status::Closed);
    } break;
    default:
        break;
    };
}

bool TcpVna::isOpen() const
{
    return _socket->state() == QTcpSocket::ConnectedState;
}

void TcpVna::onReadyRead()
{
    _buffer = _socket->readAll();
    emit commandRecived();
}

void TcpVna::onError(QAbstractSocket::SocketError)
{
    qInfo() << _socket->errorString();
}
