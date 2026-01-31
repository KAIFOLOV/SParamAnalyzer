#include "s_param_measurement.h"

SParamMeasurement::SParamMeasurement(IVna *vna) : _vna(vna)
{}

void SParamMeasurement::preparation()
{
    _vna->setStartFreq(_startFreq);
    _vna->setStopFreq(_stopFreq);
    _vna->setPointsCount(_pointsCount);
    _vna->setFilterPch(_filterPch);
    _vna->setOutputPower(_outputPower);
}

QVector<QPointF> SParamMeasurement::startMeasure()
{
    _vna->setOutputPower(_outputPower);
    return {};
}

double SParamMeasurement::startFreq() const
{
    return _startFreq;
}

void SParamMeasurement::setStartFreq(double newStartFreq)
{
    if (qFuzzyCompare(_startFreq, newStartFreq))
        return;

    _startFreq = newStartFreq;
    emit startFreqChanged();
}

double SParamMeasurement::stopFreq() const
{
    return _stopFreq;
}

void SParamMeasurement::setStopFreq(double newStopFreq)
{
    if (qFuzzyCompare(_stopFreq, newStopFreq))
        return;

    _stopFreq = newStopFreq;
    emit stopFreqChanged();
}

int SParamMeasurement::pointsCount() const
{
    return _pointsCount;
}

void SParamMeasurement::setPointsCount(int newPointsCount)
{
    if (_pointsCount == newPointsCount)
        return;

    _pointsCount = newPointsCount;
    emit pointsCountChanged();
}

float SParamMeasurement::outputPower() const
{
    return _outputPower;
}

void SParamMeasurement::setOutputPower(float newOutputPower)
{
    if (qFuzzyCompare(_outputPower, newOutputPower))
        return;

    _outputPower = newOutputPower;
    emit outputPowerChanged();
}

double SParamMeasurement::filterPch() const
{
    return _filterPch;
}

void SParamMeasurement::setFilterPch(double newFilterPch)
{
    if (qFuzzyCompare(_filterPch, newFilterPch))
        return;

    _filterPch = newFilterPch;
    emit filterPchChanged();
}

QString SParamMeasurement::format() const
{
    return _format;
}

void SParamMeasurement::setFormat(const QString &newFormat)
{
    if (_format == newFormat)
        return;

    _format = newFormat;
    emit formatChanged();
}
