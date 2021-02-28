#include "editdevicesinbuldingwindow.h"
#include "ui_editdevicesinbuldingwindow.h"

EditDevicesInBuldingWindow::EditDevicesInBuldingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditDevicesInBuldingWindow)
{
    ui->setupUi(this);
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
