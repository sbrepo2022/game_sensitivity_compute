#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QVector>
#include <QString>
#include <QFile>
#include <QDebug>

namespace Ui{
    class HelpDialog;
}

class HelpDialog : public QDialog {
    Q_OBJECT

private:
    Ui::HelpDialog *ui;
    QVector<QString> texts;

public:
    HelpDialog(QWidget* parent = 0);
    ~HelpDialog();

public slots:
    void setPage(int page);
};
