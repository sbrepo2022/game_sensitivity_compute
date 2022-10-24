#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QString>
#include <windows.h>
#include "helpdialog.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int pixelsDelta, lastX, lastY;
    bool trackingState;
    HelpDialog *helpDialog;

    bool registerInput();

    void mouseTracking(const RAWMOUSE *rmouse);
    void keyboardTrecking(const RAWKEYBOARD *rkeyboard);

    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

public slots:
    void computeButtonPushed();
    void computeSenseButtonPushed();
    void resetComputeButtonText();
    void onDistHelp();
    void onSensHelp();
};

#endif // MAINWINDOW_H
