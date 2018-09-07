﻿#include "monitorfoldersadddialog.h"
#include "ui_monitorfoldersadddialog.h"

MonitorFoldersAddDialog::MonitorFoldersAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MonitorFoldersAddDialog)
{
    ui->setupUi(this);
}

MonitorFoldersAddDialog::~MonitorFoldersAddDialog()
{
    delete ui;
}

QString MonitorFoldersAddDialog::getNewPath() const
{
    return ui->pathLineEdit->text();
}

void MonitorFoldersAddDialog::on_exploreButton_clicked()
{
    auto path = QFileDialog::getExistingDirectory(this,tr(u8"选择一个文件夹"),QString(),QFileDialog::ShowDirsOnly);
    if (!path.isEmpty())
        ui->pathLineEdit->setText(path);
}