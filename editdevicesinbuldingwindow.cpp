#include "editdevicesinbuldingwindow.h"
#include "ui_editdevicesinbuldingwindow.h"

EditDevicesInBuldingWindow::EditDevicesInBuldingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditDevicesInBuldingWindow)
{
    ui->setupUi(this);
    ui->line->setStyleSheet("background-color: rgb(255, 0, 0);");
}

EditDevicesInBuldingWindow::~EditDevicesInBuldingWindow()
{
    delete ui;
}

void EditDevicesInBuldingWindow::on_menu_but_clicked(bool checked)
{
    hide();
    emit hidden();
}
