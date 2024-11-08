#include "numbercontrol.h"
#include "ui_numbercontrol.h"

numberControl::numberControl(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::numberControl)
{
    ui->setupUi(this);
}

numberControl::~numberControl()
{
    delete ui;
}
