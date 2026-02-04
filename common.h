#ifndef COMMON_H
#define COMMON_H

#include <QPointF>
#include <QVector>
#include <QEventLoop>
#include <QtConcurrent>
#include <QFutureWatcher>

template<typename T>
static QVector<QPointF>
makeChartFromFormatted(const QVector<T> &buffer, const double freqStart, const double freqStop)
{
    if (buffer.isEmpty())
        return {};

    if (buffer.size() == 1 || qFuzzyCompare(freqStart, freqStop))
        return { QPointF(freqStart, buffer[0]) };

    QVector<QPointF> result(buffer.size());

    double step = (freqStop - freqStart) / (buffer.size() - 1);
    for (int i = 0; i < buffer.size(); i++) {
        QPointF p { freqStart + (step * i), static_cast<double>(buffer.at(i)) };
        result[i] = p;
    }
    return result;
}

#endif // COMMON_H
