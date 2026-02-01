#ifndef CURVE_H
#define CURVE_H

#include <QObject>
#include <QPointF>

class Curve : public QObject
{
    Q_OBJECT

    Q_PROPERTY(uint32_t id READ id)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QVector<QPointF> data READ data WRITE setData NOTIFY dataChanged)

public:
    explicit Curve() = default;
    explicit Curve(const uint32_t id, const QString &name, const QVector<QPointF> &data);

    const QVector<QPointF> data() const;
    void setData(const QVector<QPointF> &data);
    Q_SIGNAL void dataChanged(const QVector<QPointF> &data);

    uint32_t id() const;

    QString name() const;
    Q_SIGNAL void nameChanged();

private:
    uint32_t _id { 0 };
    QString _name { "" };
    QVector<QPointF> _data;
};

#endif // CURVE_H
