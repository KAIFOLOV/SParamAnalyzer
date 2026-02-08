#include "tcp_vna.h"

#include <QEventLoop>
#include <QTimer>
#include <qthread.h>

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

IVna::Status TcpVna::connectStatus()
{
    return static_cast<IVna::Status>(isOpen());
}

bool TcpVna::send(const QByteArray &command)
{
    if (!isOpen()) {
        emit error("Vna not connected");
        return false;
    }

    emit sendToSocket(command);

    QByteArray err;
    if (systemError(err)) {
        emit error(QString("SCPI error after send: %1 Error: %2").arg(command).arg(err));
        return false;
    }
    return true;
}

bool TcpVna::query(const QByteArray &command, QByteArray &result)
{
    if (!isOpen())
        return false;

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
        return;
    }

    qint64 bytesWritten = _socket->write(cmd);

    if (bytesWritten != cmd.size()) {
        emit error(QString(("Ошибка отправки сообщения, записано %1 из %2 байт"))
                    .arg(bytesWritten)
                    .arg(command.size()));
        return;
    }

    if (bytesWritten == -1) {
        emit error(QString("Ошибка записи данных в буфер:" % _socket->errorString()));
        return;
    }

    if (!_socket->waitForBytesWritten(1000)) {
        emit error(QString("Ошибка отправки данных:" % _socket->errorString()));
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

    if (errResponse.contains("No error"))
        return false;

    return true;
}

bool TcpVna::startMeasure()
{
    return send("INITiate:IMMediate");
}

bool TcpVna::switchStateContinuousStart(const bool state, const int channel)
{
    QString command = QString("INITiate%1:CONTinuous %2").arg(channel).arg(state);
    return send(command.toUtf8());
}

bool TcpVna::setFormat(const QString &format, const int channel, const int trace)
{
    QString command = QString("CALCulate%1:TRACe%2:FORMat %3").arg(channel).arg(trace).arg(format);
    return send(command.toUtf8());
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

bool TcpVna::setStartFreq(const double value, const int channel)
{
    QString command = QString("SENSe%1:FREQuency:STARt %2").arg(channel).arg(value);
    return send(command.toUtf8());
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

bool TcpVna::setStopFreq(const double value, const int channel)
{
    QString command = QString("SENSe%1:FREQuency:STOP %2").arg(channel).arg(value);
    return send(command.toUtf8());
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

bool TcpVna::setPointsCount(const int value, const int channel)
{
    QString command = QString("SENSe%1:SWEep:POINts %2").arg(channel).arg(value);
    return send(command.toUtf8());
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

bool TcpVna::setOutputPower(const float power)
{
    QString command = QString("SOUR:POW %1").arg(power);
    return send(command.toUtf8());
}

float TcpVna::getOutputPower()
{
    QByteArray response;
    QString command = QString("SOUR:POW?");

    query(command.toUtf8(), response);
    if (!response.isEmpty())
        return response.toFloat();

    return 0.0;
}

bool TcpVna::setFilterPch(const double value, const int channel)
{
    QString command = QString("SENSe%1:BAND %2").arg(channel).arg(value);
    return send(command.toUtf8());
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
    _buffer = _socket->readAll();
    emit commandRecived();
}

void TcpVna::onError(QAbstractSocket::SocketError)
{
    qWarning() << _socket->errorString();
}

QString TcpVna::ip() const
{
    return _ip;
}

void TcpVna::setIp(const QString &newIp)
{
    if (_ip == newIp)
        return;

    _ip = newIp;
    emit ipChanged();
}

int TcpVna::port() const
{
    return _port;
}

void TcpVna::setPort(int newPort)
{
    if (_port == newPort)
        return;

    _port = newPort;
    emit portChanged();
}
