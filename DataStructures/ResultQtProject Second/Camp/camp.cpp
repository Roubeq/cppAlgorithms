#include "camp.h"
#include "./ui_camp.h"

Camp::Camp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Camp)
{
    ui->setupUi(this);
}

Camp::~Camp()
{
    delete ui;
}
