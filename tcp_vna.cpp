#include "tcp_vna.h"

#include <QEventLoop>
#include <QTimer>

TcpVna::TcpVna() : _socket(new QTcpSocket(this))
{
    QObject::connect(_socket, &QTcpSocket::readyRead, this, &TcpVna::onReadyRead);
    QObject::connect(_socket, &QTcpSocket::errorOccurred, this, &TcpVna::onError);
    QObject::connect(_socket, &QTcpSocket::stateChanged, this, &TcpVna::statusChanged);

    QObject::connect(this, &TcpVna::sendToSocket, this, &TcpVna::write, Qt::QueuedConnection);
}

TcpVna::~TcpVna()
{
    disconnect();
}

void TcpVna::connect()
{
    if (isOpen()) {
        emit connectStatusChanged(Status::Opened);
        return;
    }
    _socket->connectToHost(_ip, _port);

    if (!_socket->waitForConnected()) {
        // emit errorMessage(tr("Failed to connect to server"));
        return;
    }

    emit connectStatusChanged(Status::Opened);
}

void TcpVna::disconnect()
{
    if (_socket && (isOpen() || _socket->state() == QTcpSocket::ConnectingState)) {
        _socket->disconnectFromHost();
        if (_socket->state() != QTcpSocket::UnconnectedState) {
            _socket->waitForDisconnected();
        }
    }

    emit connectStatusChanged(Status::Closed);
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

    QByteArray err;
    if (systemError(err)) {
        qWarning() << "SCPI error after send:" << command << "Error:" << err;
        return false;
    }

    return true;
}

bool TcpVna::query(const QByteArray &command, QByteArray &result)
{
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

    QByteArray cmd = command;
    cmd.append('\n');

    if (!isOpen()) {
        emit connectStatusChanged(Status::Closed);
        // emit errorMessage(QString(tr("Не удалось отправить команду. Устройство не подключено")));
        return;
    }

    qint64 bytesWritten = _socket->write(cmd);

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

void TcpVna::statusChanged(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::ConnectedState: {
        emit connectStatusChanged(Status::Opened);
    } break;
    case QAbstractSocket::ConnectingState:
    case QAbstractSocket::HostLookupState: {
        emit connectStatusChanged(Status::Wait);
    } break;
    case QAbstractSocket::UnconnectedState: {
        emit connectStatusChanged(Status::Closed);
    } break;
    default:
        break;
    };
}

bool TcpVna::isOpen() const
{
    return _socket->state() == QTcpSocket::ConnectedState;
}

bool TcpVna::OPC()
{
    QString command = "*OPC?";
    QByteArray response;

    query(command.toUtf8(), response);

    if (response.contains("1"))
        return true;

    return false;
}

QString TcpVna::IDN()
{
    QString command = "*IDN?";
    QByteArray response;

    query(command.toUtf8(), response);

    return QString::fromUtf8(response).trimmed();
}

bool TcpVna::systemError(QByteArray &errResponse)
{
    QString command = "SYST:ERR?";

    query(command.toUtf8(), errResponse);

    if (errResponse.contains("+0"))
        return false;

    return true;
}

void TcpVna::startMeasure()
{
    if (!isOpen())
        return;

    send("INITiate:IMMediate");
}

void TcpVna::switchStateContinuousStart(const bool state, const int channel)
{
    if (!isOpen())
        return;

    QString command = QString("INITiate%1:CONTinuous %2").arg(channel).arg(state);
    send(command.toUtf8());
}

void TcpVna::setFormat(const QString &format, const int channel, const int trace)
{
    QString command = QString("CALCulate%1:TRACe%2:FORMat %3").arg(channel).arg(trace).arg(format);
}

QVector<double> TcpVna::getData(int channel, int trace)
{
    QByteArray response;
    QString command = QString("CALCulate%1:TRACe%2:DATA:FDATa?").arg(channel).arg(trace);

    if (!query(command.toUtf8(), response))
        return {};

    QList<QByteArray> parts = response.trimmed().split(',');

    QVector<double> result;
    result.reserve(parts.size() / 2);

    for (int i = 0; i < parts.size(); i += 2) {
        bool ok = false;
        double v = parts[i].toDouble(&ok);
        if (ok)
            result.push_back(v);
    }

    return result;
}

void TcpVna::setStartFreq(const double value, const int channel)
{
    QString command = QString("SENSe%1:FREQuency:STARt %2").arg(channel).arg(value);
    send(command.toUtf8());
}

double TcpVna::getStartFreq(const int channel)
{
    QByteArray response;
    QString command = QString("SENSe%1:FREQuency:STARt?").arg(channel);

    query(command.toUtf8(), response);
    if (!response.isEmpty())
        return response.toDouble();

    return double();
}

void TcpVna::setStopFreq(const double value, const int channel)
{
    QString command = QString("SENSe%1:FREQuency:STOP %2").arg(channel).arg(value);
    send(command.toUtf8());
}

double TcpVna::getStopFreq(const int channel)
{
    QByteArray response;
    QString command = QString("SENSe%1:FREQuency:STOP?").arg(channel);

    query(command.toUtf8(), response);
    if (!response.isEmpty())
        return response.toDouble();

    return double();
}

void TcpVna::setPointsCount(const int value, const int channel)
{
    QString command = QString("SENSe%1:SWEep:POINts %2").arg(channel).arg(value);
    send(command.toUtf8());
}

int TcpVna::getPointsCount(const int channel)
{
    QByteArray response;
    QString command = QString("SENSe%1:SWEep:POINts?").arg(channel);

    query(command.toUtf8(), response);
    if (!response.isEmpty())
        return response.toInt();

    return int();
}

void TcpVna::setOutputPower(const float power)
{
    QString command = QString("SOUR:POW %1").arg(power);
    send(command.toUtf8());
}

float TcpVna::getOutputPower()
{
    QByteArray response;
    QString command = QString("SOUR:POW?");

    query(command.toUtf8(), response);
    if (!response.isEmpty())
        return response.toFloat();

    return float();
}

void TcpVna::setFilterPch(const double value, const int channel)
{
    QString command = QString("SENSe%1:BAND %2").arg(channel).arg(value);
    send(command.toUtf8());
}

double TcpVna::getFilterPch(const int channel)
{
    QByteArray response;
    QString command = QString("SENSe:POW?");

    query(command.toUtf8(), response);
    if (!response.isEmpty())
        return response.toDouble();

    return double();
}

void TcpVna::onReadyRead()
{
    qInfo() << "onReadyRead";
    _buffer = _socket->readAll();
    emit commandRecived();
}

void TcpVna::onError(QAbstractSocket::SocketError)
{
    qInfo() << _socket->errorString();
}
