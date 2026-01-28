#include "vna.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Vna *vna = new Vna();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("vna", vna);

    const QUrl url(QStringLiteral("qrc:/SParamAnalyzer/Main.qml"));
    QObject::connect(
     &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
     []() {
         QCoreApplication::exit(-1);
     },
     Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
