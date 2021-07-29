#include "main_tab.h"
#include "ui_main_tab.h"

Main_Tab::Main_Tab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Main_Tab)
{
    ui->setupUi(this);
}

Main_Tab::~Main_Tab()
{
    delete ui;
}

void Main_Tab::on_start_clicked()
{
    emit signalStart();
}
