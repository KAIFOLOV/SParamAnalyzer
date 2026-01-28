#ifndef IVNA_H
#define IVNA_H

#include <QObject>

class IVna : public QObject
{
    Q_OBJECT

public:
    virtual ~IVna() = default;

    Q_INVOKABLE virtual void connect() = 0;
    Q_INVOKABLE virtual void disconnect() = 0;

    Q_INVOKABLE virtual void sendCommand(const QString &command) = 0;
};

#endif // IVNA_H
