#ifndef CURVE_MODEL_H
#define CURVE_MODEL_H

#include "curve.h"

#include <QAbstractListModel>

class CurveModel : public QAbstractListModel
{
public:
    enum Roles
    {
        curveRole = Qt::ItemDataRole::UserRole + 1
    };

    explicit CurveModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role = Qt::ItemDataRole::EditRole) const override;

    void removeCurve(uint32_t index);
    void addCurve(std::unique_ptr<Curve> curve) noexcept;
    void addDataToCurve(const QVector<QPointF> &data, uint32_t id);

private:
    std::vector<std::unique_ptr<Curve>> _curves;
};

#endif // CURVE_MODEL_H
