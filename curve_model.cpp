#include "curve_model.h"

#include <QQmlApplicationEngine>

CurveModel::CurveModel(QObject *parent) : QAbstractListModel(parent)
{}

int CurveModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(_curves.size());
}

QVariant CurveModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return false;

    const auto curve = _curves.at(static_cast<uint32_t>(index.row())).get();

    if (!curve) {
        qWarning() << "Curve object is nullptr";
        return QVariant();
    }

    QQmlApplicationEngine::setObjectOwnership(curve, QQmlApplicationEngine::CppOwnership);

    return role == curveRole ? QVariant::fromValue(curve) : QVariant();
}

void CurveModel::removeCurve(uint32_t index)
{
    beginRemoveRows(QModelIndex(), static_cast<int>(index), static_cast<int>(index));
    {
        _curves.erase(_curves.begin() + index);
    }
    endRemoveRows();
}

QHash<int, QByteArray> CurveModel::roleNames() const
{
    return { { curveRole, "curve" } };
}

void CurveModel::addCurve(std::unique_ptr<Curve> curve) noexcept
{
    if (!curve.get()) {
        qWarning() << "Curve was nullptr";
        return;
    }

    int lastIndex = static_cast<int>(_curves.size());

    QModelIndex index;
    connect(curve.get(), &Curve::dataChanged, [this, index]() {
        emit CurveModel::dataChanged(index, index, {});
    });

    beginInsertRows(QModelIndex(), static_cast<int>(lastIndex), static_cast<int>(lastIndex));
    {
        _curves.push_back(std::move(curve));
    }
    endInsertRows();
}

void CurveModel::addDataToCurve(const QVector<QPointF> &data, uint32_t id)
{
    auto size = _curves.size();

    for (int i = 0; i < size; ++i) {
        if (_curves[i]->id() == id) {
            _curves[i]->setData(std::move(data));

            emit dataChanged(QModelIndex(), QModelIndex(), {});

            return;
        }
    }
}
