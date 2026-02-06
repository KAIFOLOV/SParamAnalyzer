#include "app.h"

#include <QApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    App mainApp(engine);

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
