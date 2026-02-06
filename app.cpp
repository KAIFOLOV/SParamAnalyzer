#include "app.h"

#include <memory>
#include <QQmlContext>

App::App(QQmlApplicationEngine &engine) :
    _vna(std::make_unique<TcpVna>()),
    _model(std::make_unique<CurveModel>()),
    _debugCtrl(std::make_unique<DebugController>(_vna.get())),
    _controller(std::make_unique<MeasurementController>(_vna.get()))
{
    auto context = engine.rootContext();
    context->setContextProperty("debugController", _debugCtrl.get());
    context->setContextProperty("curveModel", _model.get());
    context->setContextProperty("measurementController", _controller.get());

    QObject::connect(_controller.get(), &MeasurementController::newDataReady, _model.get(),
                     &CurveModel::addDataToCurve);
}
