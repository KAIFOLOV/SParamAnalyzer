#include "s_param_measurement.h"

#include "common.h"

SParamMeasurement::SParamMeasurement(IVna *vna, QSettings *settings) :
    _vna(vna),
    _settings(settings)
{
    loadSettings();
}

SParamMeasurement::~SParamMeasurement()
{
    saveSettings();
}

bool SParamMeasurement::preparation()
{
    if (!_vna->setStartFreq(_startFreq))
        return false;

    if (!_vna->setStopFreq(_stopFreq))
        return false;

    if (!_vna->setPointsCount(_pointsCount))
        return false;

    if (!_vna->setFilterPch(_filterPch))
        return false;

    if (!_vna->setOutputPower(_outputPower))
        return false;

    if (!_vna->setFormat(_format))
        return false;

    if (!_vna->switchStateContinuousStart(true))
        return false;

    return true;
}

QVector<QPointF> SParamMeasurement::startMeasure()
{
    const auto data = _vna->getData();
    const auto formattedData = makeChartFromFormatted(data, _startFreq / MHz, _stopFreq / MHz);

    return formattedData;
}

void SParamMeasurement::stop()
{
    _vna->switchStateContinuousStart(false);
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

void SParamMeasurement::loadSettings()
{
    _startFreq = _settings->value("measurement/startFreq", 1e9).toDouble(); // 1 GHz по умолчанию
    _stopFreq = _settings->value("measurement/stopFreq", 3e9).toDouble(); // 3 GHz по умолчанию
    _pointsCount = _settings->value("measurement/pointsCount", 201).toInt();
    _filterPch = _settings->value("measurement/filterPch", 1e6).toDouble();
    _outputPower = _settings->value("measurement/outputPower", 0.0f).toFloat();
    _format = _settings->value("measurement/format", "MLOG").toString();
}

void SParamMeasurement::saveSettings()
{
    _settings->setValue("measurement/startFreq", _startFreq);
    _settings->setValue("measurement/stopFreq", _stopFreq);
    _settings->setValue("measurement/pointsCount", _pointsCount);
    _settings->setValue("measurement/filterPch", _filterPch);
    _settings->setValue("measurement/outputPower", _outputPower);
    _settings->setValue("measurement/format", _format);
}
