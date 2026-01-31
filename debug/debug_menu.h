#ifndef DEBUG_MENU_H
#define DEBUG_MENU_H

#include "../IVna.h"

#include <QLabel>
#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QTextEdit>

class DebugMenu : public QWidget
{
public:
    explicit DebugMenu(IVna *vna, QWidget *parent = nullptr);

private:
    Q_SLOT void onSetStartFreq();
    Q_SLOT void onSetStopFreq();
    Q_SLOT void onSetPoints();
    Q_SLOT void onSetPower();
    Q_SLOT void onSetFilterPch();
    Q_SLOT void onContinuousStart();
    Q_SLOT void onContinuousStop();
    Q_SLOT void onStartMeasure();
    Q_SLOT void onUpdateStatus(IVna::Status status);

    Q_SLOT void onSend();
    Q_SLOT void onQuery();

private:
    void initGui();
    void showError(const QString &msg);
    double parseDouble(QLineEdit *edit, bool *ok = nullptr) const;
    int parseInt(QLineEdit *edit, bool *ok = nullptr) const;

private:
    int _channel { 1 };

    IVna *_vna { nullptr };
    QLabel *_statusLabel { nullptr };

    QLineEdit *_startFreqEdit { nullptr };
    QLineEdit *_stopFreqEdit { nullptr };
    QLineEdit *_pointsEdit { nullptr };
    QLineEdit *_powerEdit { nullptr };
    QLineEdit *_filterEdit { nullptr };
    QLineEdit *_rawCommandEdit { nullptr };

    QTextEdit *_responseDisplay { nullptr };
};

#endif // DEBUG_MENU_H
