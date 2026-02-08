#ifndef MEASUREMENT_CONTROLLER_H
#define MEASUREMENT_CONTROLLER_H

#include "s_param_measurement.h"

#include <QTimer>
#include <QObject>
#include <QSettings>

class MeasurementController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isRunning READ isRunning WRITE setIsRunning NOTIFY isRunningChanged FINAL)
    Q_PROPERTY(SParamMeasurement *measurement READ measurement NOTIFY measurementChanged FINAL)

public:
    explicit MeasurementController(IVna *vna, QSettings *settings, QObject *parent = nullptr);

    Q_SIGNAL void startManualTimer();
    Q_SIGNAL void newDataReady(const QVector<QPointF> &data, uint32_t id = 0);

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

    bool isRunning() const;
    void setIsRunning(bool newIsRunning);
    Q_SIGNAL void isRunningChanged();

    SParamMeasurement *measurement() const;
    void setMeasurement(std::unique_ptr<SParamMeasurement> newMeasurement);
    Q_SIGNAL void measurementChanged();

private:
    void fetchData();

private:
    bool _isRunning { false };
    std::atomic<bool> _stopRequested { false };

    QTimer _continuousModeTimer;

    std::unique_ptr<SParamMeasurement> _measurement;
};

#endif // MEASUREMENT_CONTROLLER_H
