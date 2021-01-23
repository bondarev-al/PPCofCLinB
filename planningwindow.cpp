#include "planningwindow.h"
#include "ui_planningwindow.h"

PlanningWindow::PlanningWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlanningWindow)
{
    ui->setupUi(this);
}

PlanningWindow::~PlanningWindow()
{
    delete ui;
}

void PlanningWindow::on_menu_but_clicked(bool checked)
{
    hide();
    emit hidden();
}
