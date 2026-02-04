#include "debug_controller.h"

DebugController::DebugController(IVna *vna, QObject *parent) : QObject { parent }, _vna(vna)
{
    _debugMenu = std::make_unique<DebugMenu>(_vna);
    _debugMenu->setWindowTitle("VNA Debug Menu");
    _debugMenu->resize(500, 700);
}

void DebugController::showDebugMenu()
{
    if (_debugMenu) {
        _debugMenu->show();
        _debugMenu->raise();
        _debugMenu->activateWindow();
    }
}
