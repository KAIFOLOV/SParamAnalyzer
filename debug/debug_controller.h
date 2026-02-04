#ifndef DEBUG_CONTROLLER_H
#define DEBUG_CONTROLLER_H

#include "../IVna.h"
#include "debug_menu.h"

#include <QObject>

class DebugController : public QObject
{
    Q_OBJECT

public:
    explicit DebugController(IVna *vna, QObject *parent = nullptr);

    Q_INVOKABLE void showDebugMenu();

private:
    IVna *_vna;
    std::unique_ptr<DebugMenu> _debugMenu;
};

#endif // DEBUG_CONTROLLER_H
