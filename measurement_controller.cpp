#include "measurement_controller.h"

#include <QThread>
#include <qdebug.h>
#include <qeventloop.h>
#include <qfuturewatcher.h>
#include <qtconcurrentrun.h>

void longOperation(std::function<void()> func)
{
    QEventLoop loop;
    QFutureWatcher<void> watcher;
    QObject::connect(&watcher, &QFutureWatcherBase::finished, &loop, &QEventLoop::quit);

    QFuture<void> future = QtConcurrent::run([&func] {
        func();
    });

    watcher.setFuture(future);
    if (!watcher.isFinished())
        loop.exec();
    future.waitForFinished();
}

MeasurementController::MeasurementController(IVna *vna, QSettings *settings, QObject *parent) :
    QObject { parent },
    _measurement(std::make_unique<SParamMeasurement>(vna, settings))
{
    _continuousModeTimer.setInterval(100);

    connect(this, &MeasurementController::startManualTimer, this, [this]() {
        _continuousModeTimer.start();
    });

    connect(&_continuousModeTimer, &QTimer::timeout, this, &MeasurementController::fetchData);

    connect(vna, &IVna::error, this, [this](const QString &error) {
        qWarning() << error;
        stop();
    });
}

void MeasurementController::start()
{
    if (_isRunning)
        return;

    longOperation([this]() {
        if (!_measurement->preparation())
            return;

        emit startManualTimer();
        setIsRunning(true);
    });
}

void MeasurementController::stop()
{
    if (!_isRunning)
        return;

    _continuousModeTimer.stop();
    _measurement->stop();

    setIsRunning(false);
}

void MeasurementController::fetchData()
{
    longOperation([this]() {
        const auto data = _measurement->startMeasure();
        emit newDataReady(data);
    });
}

bool MeasurementController::isRunning() const
{
    return _isRunning;
}

void MeasurementController::setIsRunning(bool newIsRunning)
{
    if (_isRunning == newIsRunning)
        return;

    _isRunning = newIsRunning;
    emit isRunningChanged();
}

SParamMeasurement *MeasurementController::measurement() const
{
    return _measurement.get();
}

void MeasurementController::setMeasurement(std::unique_ptr<SParamMeasurement> newMeasurement)
{
    if (_measurement.get() == newMeasurement.get())
        return;

    _measurement = std::move(newMeasurement);
    emit measurementChanged();
}
