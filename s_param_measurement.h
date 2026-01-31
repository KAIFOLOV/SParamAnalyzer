#ifndef S_PARAM_MEASUREMENT_H
#define S_PARAM_MEASUREMENT_H

#include "IVna.h"

#include <QPointF>

class SParamMeasurement : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString format READ format WRITE setFormat NOTIFY formatChanged)
    Q_PROPERTY(double stopFreq READ stopFreq WRITE setStopFreq NOTIFY stopFreqChanged)
    Q_PROPERTY(double startFreq READ startFreq WRITE setStartFreq NOTIFY startFreqChanged)
    Q_PROPERTY(double filterPch READ filterPch WRITE setFilterPch NOTIFY filterPchChanged)
    Q_PROPERTY(int pointsCount READ pointsCount WRITE setPointsCount NOTIFY pointsCountChanged)
    Q_PROPERTY(float outputPower READ outputPower WRITE setOutputPower NOTIFY outputPowerChanged)

    explicit SParamMeasurement(IVna *vna);

    void preparation();
    QVector<QPointF> startMeasure();

    double startFreq() const;
    void setStartFreq(double newStartFreq);
    Q_SIGNAL void startFreqChanged();

    double stopFreq() const;
    void setStopFreq(double newStopFreq);
    Q_SIGNAL void stopFreqChanged();

    int pointsCount() const;
    void setPointsCount(int newPointsCount);
    Q_SIGNAL void pointsCountChanged();

    float outputPower() const;
    void setOutputPower(float newOutputPower);
    Q_SIGNAL void outputPowerChanged();

    double filterPch() const;
    void setFilterPch(double newFilterPch);
    Q_SIGNAL void filterPchChanged();

    QString format() const;
    void setFormat(const QString &newFormat);
    Q_SIGNAL void formatChanged();

private:
    IVna *_vna;

    QString _format { "PHASE" };
    int _pointsCount { 50 };
    double _stopFreq { 2000000000 };
    double _startFreq { 1000000000 };
    double _filterPch { 96812 };
    float _outputPower { 10 };
};

#endif // S_PARAM_MEASUREMENT_H
