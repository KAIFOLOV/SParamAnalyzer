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

MeasurementController::MeasurementController(IVna *vna, QObject *parent) :
    QObject { parent },
    _measurement(std::make_unique<SParamMeasurement>(vna))
{
    _continuousModeTimer.setInterval(100);

    connect(this, &MeasurementController::startManualTimer, this, [this]() {
        _continuousModeTimer.start();
    });

    connect(&_continuousModeTimer, &QTimer::timeout, this, &MeasurementController::fetchData);
}

void MeasurementController::start()
{
    // QThread *workerThread = QThread::create([this]() {
    //     try {
    //         _measurement->preparation();
    //         emit startManualTimer();
    //     } catch (const std::runtime_error &e) {
    //         qWarning() << e.what();
    //     }
    // });

    // connect(workerThread, &QThread::finished, workerThread, &QObject::deleteLater);
    // workerThread->start();

    longOperation([this]() {
        _measurement->preparation();
        emit startManualTimer();
    });
}

void MeasurementController::stop()
{
    _measurement->stop();
    _continuousModeTimer.stop();
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
