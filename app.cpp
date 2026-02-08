#include "app.h"

#include <memory>
#include <QQmlContext>

App::App(QQmlApplicationEngine &engine) :
    _settings(new QSettings("SParamAnalyzer")),
    _vna(std::make_unique<TcpVna>()),
    _model(std::make_unique<CurveModel>()),
    _debugCtrl(std::make_unique<DebugController>(_vna.get())),
    _controller(std::make_unique<MeasurementController>(_vna.get(), _settings))
{
    auto context = engine.rootContext();
    context->setContextProperty("debugController", _debugCtrl.get());
    context->setContextProperty("curveModel", _model.get());
    context->setContextProperty("measurementController", _controller.get());
    context->setContextProperty("vna", _vna.get());

    qmlRegisterUncreatableType<IVna>("Vna", 1, 0, "IVna", "Only for enums and type information");

    QObject::connect(_controller.get(), &MeasurementController::newDataReady, _model.get(),
                     &CurveModel::addDataToCurve);
}
