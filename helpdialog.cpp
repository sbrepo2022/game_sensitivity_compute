#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) : QDialog(parent), ui(new Ui::HelpDialog) {
    ui->setupUi(this);
    this->setWindowTitle("Help");
    connect(ui->pageSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(setPage(int)));

    QFile file;

    file.setFileName(":/help/help_dist_compute.txt");
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            texts.append(file.readAll());
            file.close();
        }
    }

    file.setFileName(":/help/help_sense_compute.txt");
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            texts.append(file.readAll());
            file.close();
        }
    }

    setPage(0);
}

HelpDialog::~HelpDialog() {
    delete ui;
}


void HelpDialog::setPage(int page) {
    if (texts.size() - 1 >= page) {
        ui->pageSelect->setCurrentIndex(page);
        ui->helpText->setHtml(texts[page]);
    }
}
