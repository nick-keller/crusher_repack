#include "importdialog.h"
#include "ui_importdialog.h"

ImportDialog::ImportDialog(QImage image, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportDialog)
{
    ui->setupUi(this);
    ui->cropper->setImage(image);
}

ImportDialog::~ImportDialog()
{
    delete ui;
}
