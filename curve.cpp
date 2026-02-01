#include "curve.h"

Curve::Curve(const uint32_t id, const QString &name, const QVector<QPointF> &data) :
    _id(id),
    _name(name),
    _data(data)
{}

const QVector<QPointF> Curve::data() const
{
    return _data;
}

void Curve::setData(const QVector<QPointF> &data)
{
    _data = data;
    emit dataChanged(_data);
}

uint32_t Curve::id() const
{
    return _id;
}

QString Curve::name() const
{
    return _name;
}
