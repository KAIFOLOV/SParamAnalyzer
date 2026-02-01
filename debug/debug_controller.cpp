#include "debug_controller.h"

DebugController::DebugController(IVna *vna, CurveModel *model, QObject *parent) :
    QObject { parent },
    _vna(vna),
    _model(model)
{
    _debugMenu = std::make_unique<DebugMenu>(_vna);
    _debugMenu->setWindowTitle("VNA Debug Menu");
    _debugMenu->resize(500, 700);
}

void DebugController::showDebugMenu()
{
    QVector<QPointF> data = { { 1, 0 }, { 2, 1 } };
    _model->addCurve(std::make_unique<Curve>(0, "test", data));
    _model->addCurve(std::make_unique<Curve>(1, "test1", data));
    _model->removeCurve(1);

    if (_debugMenu) {
        _debugMenu->show();
        _debugMenu->raise();
        _debugMenu->activateWindow();
    }
}
