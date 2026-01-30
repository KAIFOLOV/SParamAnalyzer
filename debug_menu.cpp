#include "debug_menu.h"

#include <QGroupBox>
#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QIntValidator>

DebugMenu::DebugMenu(IVna *vna, QWidget *parent) : QWidget(parent, Qt::Window), _vna(vna)
{
    setAttribute(Qt::WA_DeleteOnClose, false);
    initGui();

    connect(_vna, &IVna::connectStatusChanged, this, &DebugMenu::onUpdateStatus);
    onUpdateStatus(_vna->isOpen() ? IVna::Status::Opened : IVna::Status::Closed);
}

void DebugMenu::initGui()
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    auto connectionGroup = new QGroupBox("Подключение к анализатору", this);
    auto connectionLayout = new QFormLayout(connectionGroup);
    connectionLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    auto connectBtn = new QPushButton("Подключиться", this);
    connect(connectBtn, &QPushButton::clicked, this, [this]() {
        _vna->connect();
    });

    auto disconnectBtn = new QPushButton("Отключиться", this);
    connect(disconnectBtn, &QPushButton::clicked, this, [this]() {
        _vna->disconnect();
    });

    auto systemError = new QPushButton("System error", this);
    connect(systemError, &QPushButton::clicked, this, [this]() {
        QByteArray responce;
        _vna->systemError(responce);
        qInfo() << QString::fromUtf8(responce);
    });

    auto btnLayout = new QHBoxLayout;
    btnLayout->addWidget(connectBtn);
    btnLayout->addWidget(disconnectBtn);
    btnLayout->addWidget(systemError);
    connectionLayout->addRow(btnLayout);

    mainLayout->addWidget(connectionGroup);

    _statusLabel = new QLabel(this);
    _statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(_statusLabel);

    auto freqGroup = new QGroupBox("Частотные параметры", this);
    auto freqLayout = new QFormLayout(freqGroup);

    _startFreqEdit = new QLineEdit("1000000", this);
    auto startLayout = new QHBoxLayout;
    startLayout->addWidget(_startFreqEdit);

    auto startFreqBtn = new QPushButton("Set Start Freq (Hz)", this);
    connect(startFreqBtn, &QPushButton::clicked, this, &DebugMenu::onSetStartFreq);
    freqLayout->addRow("Start Freq:", startLayout);
    freqLayout->addRow("", startFreqBtn);

    _stopFreqEdit = new QLineEdit("2000000000", this);
    auto stopLayout = new QHBoxLayout;
    stopLayout->addWidget(_stopFreqEdit);
    auto stopFreqBtn = new QPushButton("Set Stop Freq (Hz)", this);
    connect(stopFreqBtn, &QPushButton::clicked, this, &DebugMenu::onSetStopFreq);
    freqLayout->addRow("Stop Freq:", stopLayout);
    freqLayout->addRow("", stopFreqBtn);

    mainLayout->addWidget(freqGroup);

    auto paramsGroup = new QGroupBox("Прочие параметры", this);
    auto paramsLayout = new QFormLayout(paramsGroup);

    _pointsEdit = new QLineEdit("101", this);
    auto pointsLayout = new QHBoxLayout;
    pointsLayout->addWidget(_pointsEdit);
    auto pointsBtn = new QPushButton("Set Points Count", this);
    connect(pointsBtn, &QPushButton::clicked, this, &DebugMenu::onSetPoints);
    paramsLayout->addRow("Points:", pointsLayout);
    paramsLayout->addRow("", pointsBtn);

    _powerEdit = new QLineEdit("-10", this);
    auto powerBtn = new QPushButton("Set Output Power (dBm)", this);
    connect(powerBtn, &QPushButton::clicked, this, &DebugMenu::onSetPower);
    paramsLayout->addRow("Power:", _powerEdit);
    paramsLayout->addRow("", powerBtn);

    _filterEdit = new QLineEdit("100000", this);
    auto filterLayout = new QHBoxLayout;
    filterLayout->addWidget(_filterEdit);
    auto filterBtn = new QPushButton("Set Filter Pch (Hz)", this);
    connect(filterBtn, &QPushButton::clicked, this, &DebugMenu::onSetFilterPch);
    paramsLayout->addRow("Filter Pch:", filterLayout);
    paramsLayout->addRow("", filterBtn);

    mainLayout->addWidget(paramsGroup);

    auto controlGroup = new QGroupBox("Управление измерением", this);
    auto controlLayout = new QVBoxLayout(controlGroup);

    auto contLayout = new QHBoxLayout;
    auto contStartBtn = new QPushButton("Contin start", this);
    connect(contStartBtn, &QPushButton::clicked, this, &DebugMenu::onContinuousStart);

    auto contStopBtn = new QPushButton("Contin stop", this);
    connect(contStopBtn, &QPushButton::clicked, this, &DebugMenu::onContinuousStop);

    controlLayout->addLayout(contLayout);
    controlLayout->addWidget(contStartBtn);
    controlLayout->addWidget(contStopBtn);

    auto startBtn = new QPushButton("Start Single Measurement", this);
    connect(startBtn, &QPushButton::clicked, this, &DebugMenu::onStartMeasure);
    controlLayout->addWidget(startBtn);

    mainLayout->addWidget(controlGroup);

    // Группа: Сырые команды
    auto rawGroup = new QGroupBox("Raw SCPI Commands", this);
    auto rawLayout = new QVBoxLayout(rawGroup);

    _rawCommandEdit = new QLineEdit(this);
    _rawCommandEdit->setPlaceholderText("Введите команду");
    auto sendBtn = new QPushButton("Send Command", this);
    connect(sendBtn, &QPushButton::clicked, this, &DebugMenu::onSend);

    auto queryBtn = new QPushButton("Query", this);
    connect(queryBtn, &QPushButton::clicked, this, &DebugMenu::onQuery);

    _responseDisplay = new QTextEdit(this);
    _responseDisplay->setReadOnly(true);
    _responseDisplay->setFixedHeight(80);

    rawLayout->addWidget(_rawCommandEdit);
    rawLayout->addWidget(sendBtn);
    rawLayout->addWidget(queryBtn);
    rawLayout->addWidget(new QLabel("Response:"));
    rawLayout->addWidget(_responseDisplay);

    mainLayout->addWidget(rawGroup);

    auto closeBtn = new QPushButton("Close", this);
    connect(closeBtn, &QPushButton::clicked, this, &QWidget::close);
    mainLayout->addWidget(closeBtn);
}

void DebugMenu::onSetStartFreq()
{
    bool ok;
    double freq = parseDouble(_startFreqEdit, &ok);
    if (!ok)
        return showError("Некорректное значение частоты");

    _vna->setStartFreq(freq);
}

void DebugMenu::onSetStopFreq()
{
    bool ok;
    double freq = parseDouble(_stopFreqEdit, &ok);
    if (!ok)
        return showError("Некорректное значение частоты");

    _vna->setStopFreq(freq);
}

void DebugMenu::onSetPoints()
{
    bool ok;
    int points = parseInt(_pointsEdit, &ok);
    if (!ok || points <= 0)
        return showError("Points must be > 0");

    _vna->setPointsCount(points);
}

void DebugMenu::onSetPower()
{
    bool ok;
    float power = static_cast<float>(parseDouble(_powerEdit, &ok));
    if (!ok)
        return showError("Некорректное значение мощности");

    _vna->setOutputPower(power);
}

void DebugMenu::onSetFilterPch()
{
    bool ok;
    double val = parseDouble(_filterEdit, &ok);
    if (!ok)
        return showError("Некорректное значение фильтра");

    _vna->setFilterPch(val);
}

void DebugMenu::onContinuousStart()
{
    _vna->switchStateContinuousStart(true);
}

void DebugMenu::onContinuousStop()
{
    _vna->switchStateContinuousStart(false);
}

void DebugMenu::onStartMeasure()
{
    _vna->startMeasure();
}

void DebugMenu::onSend()
{
    QByteArray cmd = _rawCommandEdit->text().toUtf8().trimmed();
    if (cmd.isEmpty())
        return showError("Команда не задана");

    bool ok = _vna->send(cmd);

    if (ok) {
        _responseDisplay->setText("Command sent successfully");
    } else {
        showError("Ошибка отправки команды");
    }
}

void DebugMenu::onQuery()
{
    QByteArray cmd = _rawCommandEdit->text().toUtf8().trimmed();
    if (cmd.isEmpty())
        return showError("Команда не задана");

    QByteArray result;
    bool ok = _vna->query(cmd, result);

    _responseDisplay->setText(result);
}

void DebugMenu::onUpdateStatus(IVna::Status status)
{
    QString text;
    QColor color;
    switch (status) {
    case IVna::Status::Opened:
        text = "Подключено";
        color = Qt::green;
        break;
    case IVna::Status::Closed:
        text = "Отключено";
        color = Qt::red;
        break;
    case IVna::Status::Wait:
        text = "Ожидание...";
        color = Qt::yellow;
        break;
    }
    _statusLabel->setText(text);
    _statusLabel->setStyleSheet(
     QString("QLabel { font-weight: bold; padding: 8px; border: 1px solid #aaa; "
             "border-radius: 4px; background-color: %1; color: %2; }")
      .arg(color.name())
      .arg(color.lightness() > 150 ? "black" : "white"));
}

void DebugMenu::showError(const QString &msg)
{
    QMessageBox::warning(this, "Ошибка", msg);
}

double DebugMenu::parseDouble(QLineEdit *edit, bool *ok) const
{
    return edit->text().toDouble(ok);
}

int DebugMenu::parseInt(QLineEdit *edit, bool *ok) const
{
    return edit->text().toInt(ok);
}
