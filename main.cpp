#include "tcp_vna.h"
#include "debug/debug_controller.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    IVna *vna = new TcpVna();

    DebugController debugCtrl(vna);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("debugController", &debugCtrl);

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
