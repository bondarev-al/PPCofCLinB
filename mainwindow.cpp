#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    edit_building_window           = new EditBuildingWindow();
    edit_devices_in_bulding_window = new EditDevicesInBuldingWindow();
    planning_window                = new PlanningWindow();

    connect(edit_building_window,           SIGNAL(hidden()), this, SLOT(show()));
    connect(edit_devices_in_bulding_window, SIGNAL(hidden()), this, SLOT(show()));
    connect(planning_window,                SIGNAL(hidden()), this, SLOT(show()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_edit_building_but_clicked(bool checked)
{
    hide();
    edit_building_window->show();
}

void MainWindow::on_edit_devices_but_clicked(bool checked)
{
    hide();
    edit_devices_in_bulding_window->show();
}

void MainWindow::on_planning_but_clicked(bool checked)
{
    hide();
    planning_window->show();
}
