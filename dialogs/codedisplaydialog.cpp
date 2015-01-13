#include "codedisplaydialog.h"
#include "ui_codedisplaydialog.h"

CodeDisplayDialog::CodeDisplayDialog(QString code, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CodeDisplayDialog)
{
    ui->setupUi(this);
    ui->code->setHtml(code);
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
