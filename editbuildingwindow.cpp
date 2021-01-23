#include "editbuildingwindow.h"
#include "ui_editbuildingwindow.h"

EditBuildingWindow::EditBuildingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditBuildingWindow)
{
    ui->setupUi(this);
    floors_but_vec.push_back(new FloorButton(ui->floors_but_layout, ui->floors_frame));

}

EditBuildingWindow::~EditBuildingWindow()
{
    delete ui;   
}

void EditBuildingWindow::on_menu_but_clicked(bool checked)
{
    hide();
    emit hidden();
}
