#include "tcp_vna.h"
#include "curve_model.h"
#include "measurement_controller.h"
#include "debug/debug_controller.h"

#include <QQmlContext>
#include <QApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    IVna *vna = new TcpVna();
    DebugController debugCtrl(vna);

    CurveModel *model = new CurveModel();
    MeasurementController *controller = new MeasurementController(vna);

    QObject::connect(controller, &MeasurementController::newDataReady, model,
                     &CurveModel::addDataToCurve);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("debugController", &debugCtrl);
    engine.rootContext()->setContextProperty("curveModel", model);
    engine.rootContext()->setContextProperty("measurementController", controller);

    const QUrl url(QStringLiteral("qrc:/qml/Main.qml"));
    QObject::connect(
     &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
     []() {
         QCoreApplication::exit(-1);
     },
     Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
