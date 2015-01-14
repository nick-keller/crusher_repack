#include "basicexportdialog.h"
#include "ui_basicexportdialog.h"

BasicExportDialog::BasicExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BasicExportDialog)
{
    ui->setupUi(this);

    QButtonGroup *dotGrp = new QButtonGroup(this);
    dotGrp->addButton(ui->dotPlot);
    dotGrp->addButton(ui->dotPxl);
    dotGrp->addButton(ui->dotFline);
    dotGrp->addButton(ui->dotDrawStat);

    QButtonGroup *lineGrp = new QButtonGroup(this);
    lineGrp->addButton(ui->lineFline);
    lineGrp->addButton(ui->lineDrawStat);

    QButtonGroup *rectGrp = new QButtonGroup(this);
    rectGrp->addButton(ui->rectFline);
    rectGrp->addButton(ui->rectForLoop);
    rectGrp->addButton(ui->rectDrawStat);

    QObject::connect(ui->dotListX, SIGNAL(currentIndexChanged(int)), this, SLOT(customDrawStatListX(int)));
    QObject::connect(ui->dotListY, SIGNAL(currentIndexChanged(int)), this, SLOT(customDrawStatListY(int)));
    QObject::connect(ui->lineListX, SIGNAL(currentIndexChanged(int)), this, SLOT(customDrawStatListX(int)));
    QObject::connect(ui->lineListY, SIGNAL(currentIndexChanged(int)), this, SLOT(customDrawStatListY(int)));
    QObject::connect(ui->rectListX, SIGNAL(currentIndexChanged(int)), this, SLOT(customDrawStatListX(int)));
    QObject::connect(ui->rectListY, SIGNAL(currentIndexChanged(int)), this, SLOT(customDrawStatListY(int)));

    QObject::connect(ui->dotDrawStat, SIGNAL(toggled(bool)), this, SLOT(customDrawStat(bool)));
    QObject::connect(ui->lineDrawStat, SIGNAL(toggled(bool)), this, SLOT(customDrawStat(bool)));
    QObject::connect(ui->rectDrawStat, SIGNAL(toggled(bool)), this, SLOT(customDrawStat(bool)));
}

BasicExportDialog::~BasicExportDialog()
{
    delete ui;
}

BasicExportDialog::Function BasicExportDialog::dotsFunction()
{
    if(ui->dotPlot->isChecked())
        return PlotOn;
    if(ui->dotPxl->isChecked())
        return PxlOn;
    if(ui->dotFline->isChecked())
        return FLine;
    return DrawStat;
}

int BasicExportDialog::dotsListX()
{
    return ui->dotListX->currentIndex() +1;
}

int BasicExportDialog::dotsListY()
{
    return ui->dotListY->currentIndex() +1;
}

BasicExportDialog::Function BasicExportDialog::rectsFunction()
{
    if(ui->rectFline->isChecked())
        return FLine;
    if(ui->rectForLoop->isChecked())
        return ForLoop;
    return DrawStat;
}

QString BasicExportDialog::rectsLoopVar()
{
    return ui->rectLoopVar->currentText();
}

void BasicExportDialog::on_globalDrawStat_clicked(bool checked)
{
    if(checked){
        ui->dotDrawStat->setChecked(true);
        ui->lineDrawStat->setChecked(true);
        ui->rectDrawStat->setChecked(true);
    }
    else{
        ui->dotFline->setChecked(true);
        ui->lineFline->setChecked(true);
        ui->rectForLoop->setChecked(true);
    }
}

void BasicExportDialog::on_globalListX_currentIndexChanged(int i)
{
    if(i < 6){
        ui->dotListX->setCurrentIndex(i);
        ui->lineListX->setCurrentIndex(i);
        ui->rectListX->setCurrentIndex(i);
    }
}

void BasicExportDialog::on_globalListY_currentIndexChanged(int i)
{
    if(i < 6){
        ui->dotListY->setCurrentIndex(i);
        ui->lineListY->setCurrentIndex(i);
        ui->rectListY->setCurrentIndex(i);
    }
}

void BasicExportDialog::customDrawStatListX(int i)
{
    if(i != ui->globalListX->currentIndex())
        ui->globalListX->setCurrentIndex(6);
}

void BasicExportDialog::customDrawStatListY(int i)
{
    if(i != ui->globalListY->currentIndex())
        ui->globalListY->setCurrentIndex(6);
}

void BasicExportDialog::customDrawStat(bool checked)
{
    if(!checked){
        ui->globalDrawStat->setChecked(false);
    }
    else{
        if(ui->dotDrawStat->isChecked() && ui->lineDrawStat->isChecked() && ui->rectDrawStat->isChecked())
            ui->globalDrawStat->setChecked(true);
    }
}
