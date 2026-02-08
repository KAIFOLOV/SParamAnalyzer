#ifndef APP_H
#define APP_H

#include "tcp_vna.h"
#include "curve_model.h"
#include "measurement_controller.h"
#include "debug/debug_controller.h"

#include <QSettings>
#include <QQmlApplicationEngine>

class App
{
public:
    App(QQmlApplicationEngine &engine);

private:
    QPointer<QSettings> _settings;

    std::unique_ptr<IVna> _vna;
    std::unique_ptr<CurveModel> _model;
    std::unique_ptr<DebugController> _debugCtrl;
    std::unique_ptr<MeasurementController> _controller;
};

#endif // APP_H
