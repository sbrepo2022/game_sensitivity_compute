#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    helpDialog = new HelpDialog();

    connect(ui->computeButton, SIGNAL(clicked()), this, SLOT(computeButtonPushed()));
    connect(ui->computeSenseButton, SIGNAL(clicked()), this, SLOT(computeSenseButtonPushed()));
    connect(ui->enableShortcutCheckBox, SIGNAL(stateChanged(int)), this, SLOT(resetComputeButtonText()));
    connect(ui->distHelpButton, SIGNAL(clicked()), this, SLOT(onDistHelp()));
    connect(ui->sensHelpButton, SIGNAL(clicked()), this, SLOT(onSensHelp()));

    trackingState = false;
    pixelsDelta = 0;
    registerInput();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result) {
    Q_UNUSED(eventType);
    Q_UNUSED(result);

    MSG* msg = reinterpret_cast<MSG*>(message);
    switch ( msg->message ) {
        case WM_INPUT:
            UINT iBuffer;

            GetRawInputData((HRAWINPUT)(msg->lParam), RID_INPUT, NULL, &iBuffer, sizeof(RAWINPUTHEADER));
            LPBYTE lpb = new BYTE[iBuffer];
            if (lpb == NULL) {
                break;
            }

            UINT readSize = GetRawInputData((HRAWINPUT)(msg->lParam), RID_INPUT, lpb, &iBuffer, sizeof(RAWINPUTHEADER)) ;

            if( readSize != iBuffer ) {
                qDebug() << "incorrect data!";
                break;
            }
            RAWINPUT *raw = (RAWINPUT*)lpb;

            if (trackingState) {
                if (raw->header.dwType== RIM_TYPEMOUSE) {
                    mouseTracking(&raw->data.mouse);
                }
            }
            if (ui->enableShortcutCheckBox->isChecked()) {
                if (raw->header.dwType== RIM_TYPEKEYBOARD) {
                    keyboardTrecking(&raw->data.keyboard);
                }
            }
            return true;
    }
    return false;
}

bool MainWindow::registerInput() {
    HWND hWnd = (HWND)this->winId();
    RAWINPUTDEVICE Rid[2];

    Rid[0].usUsagePage = 0x01;
    Rid[0].usUsage = 0x02;
    Rid[0].dwFlags = RIDEV_INPUTSINK;   // adds HID mouse and also ignores legacy mouse messages
    Rid[0].hwndTarget = hWnd;

    Rid[1].usUsagePage = 0x01;
    Rid[1].usUsage = 0x06;
    Rid[1].dwFlags = RIDEV_INPUTSINK;   // adds HID keyboard and also ignores legacy keyboard messages
    Rid[1].hwndTarget = hWnd;

    if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == false) {
        qDebug() << "register error!";
        return false;
    }
    return true;
}

void MainWindow::mouseTracking(const RAWMOUSE* rmouse) {
    if (MOUSE_MOVE_RELATIVE == rmouse->usFlags) {
        lastX = rmouse->lLastX;
        lastY = rmouse->lLastY;
        pixelsDelta += lastX;
    }
}

void MainWindow::keyboardTrecking(const RAWKEYBOARD *rkeyboard) {
    if (rkeyboard->Flags == RI_KEY_MAKE) {
        switch (rkeyboard->VKey) {
            case 0x50:
                computeButtonPushed();
                break;
        }
    }
}

void MainWindow::resetComputeButtonText() {
    QString button_text;

    if (trackingState) {
        button_text = "End compute";
        if (ui->enableShortcutCheckBox->isChecked())
            button_text += " (press P)";
        ui->computeButton->setText(button_text);
    }
    else {
        button_text = "Start compute";
        if (ui->enableShortcutCheckBox->isChecked())
            button_text += " (press P)";
        ui->computeButton->setText(button_text);
    }
}

void MainWindow::computeButtonPushed() {
    if (!trackingState) {
        trackingState = true;
        resetComputeButtonText();
        pixelsDelta = 0;
    }
    else {
        trackingState = false;
        resetComputeButtonText();

        ui->pixelsLabel->setText("Number of pixsels in 360 rotate: " + QString::number(abs(pixelsDelta)));
        ui->distLabel->setText("360 rotate distance: " + QString::number((float)abs(pixelsDelta) / ui->dpiSpinBox->value()));
    }
}

void MainWindow::computeSenseButtonPushed() {
    ui->prefSenseLabel->setText("Preferred sensitivity: " + QString::number(ui->currDistSpinBox->value() * ui->currSenseSpinBox->value() / ui->prefDistSpinBox->value()));
}

void MainWindow::onDistHelp() {
    helpDialog->setPage(0);
    helpDialog->show();
}

void MainWindow::onSensHelp() {
    helpDialog->setPage(1);
    helpDialog->show();
}
