#include "tcp_vna.h"
#include "curve_model.h"
#include "debug/debug_controller.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    IVna *vna = new TcpVna();

    CurveModel *model = new CurveModel();
    QVector<QPointF> data = { { 1, 0 }, { 2, 1 }, { 5, 5 } };
    model->addCurve(std::make_unique<Curve>(0, "test", data));
    DebugController debugCtrl(vna, model);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("debugController", &debugCtrl);
    engine.rootContext()->setContextProperty("curveModel", model);

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
