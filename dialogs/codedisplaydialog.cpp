#include "codedisplaydialog.h"
#include "ui_codedisplaydialog.h"

CodeDisplayDialog::CodeDisplayDialog(QString code, int rects, int lines, int dots, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CodeDisplayDialog)
{
    ui->setupUi(this);
    ui->code->setHtml(code);
    ui->nbrRects->setText(QString::number(rects));
    ui->nbrLines->setText(QString::number(lines));
    ui->nbrDots->setText(QString::number(dots));
}

CodeDisplayDialog::~CodeDisplayDialog()
{
    delete ui;
}

void CodeDisplayDialog::on_copyBtn_clicked()
{
    ui->code->selectAll();
    ui->code->copy();
}
