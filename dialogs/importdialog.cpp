#include "importdialog.h"
#include "ui_importdialog.h"

ImportDialog::ImportDialog(QImage image, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportDialog)
{
    ui->setupUi(this);
    ui->dithering->setChecked(false);

    QObject::connect(ui->cropper, SIGNAL(imageCropped(QImage)), ui->importer, SLOT(setImage(QImage)));
    QObject::connect(ui->cropper, SIGNAL(imageCropping(QImage)), ui->importer, SLOT(setPreview(QImage)));
    QObject::connect(ui->strech, SIGNAL(toggled(bool)), ui->cropper, SLOT(setStretch(bool)));
    QObject::connect(ui->crop, SIGNAL(clicked()), ui->cropper, SLOT(setCroppingCrop()));
    QObject::connect(ui->fit, SIGNAL(clicked()), ui->cropper, SLOT(setCroppingFit()));
    QObject::connect(ui->stretch, SIGNAL(clicked()), ui->cropper, SLOT(setCroppingStretch()));
    QObject::connect(ui->dithering, SIGNAL(toggled(bool)), ui->importer, SLOT(setDithering(bool)));
    QObject::connect(ui->ditheringLevel, SIGNAL(valueChanged(int)), ui->importer, SLOT(setDitheringLevel(int)));

    ui->cropper->setImage(image);
}

ImportDialog::~ImportDialog()
{
    delete ui;
}
