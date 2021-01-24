#include "editbuildingwindow.h"
#include "ui_editbuildingwindow.h"

EditBuildingWindow::EditBuildingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditBuildingWindow)
{
    ui->setupUi(this);
//    QVBoxLayout *menu_layout = new QVBoxLayout(this);
//    menu_bar = new QMenuBar(this);
//    menu     = new QMenu("Меню");
//    menu_bar->addMenu(menu);
//    menu_layout->addWidget(menu_bar);
//    ui->edit_layout->addWidget(menu_bar);

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

void EditBuildingWindow::on_plus_floor_but_clicked()
{
    floors_but_vec.push_back(new FloorButton(ui->floors_but_layout, ui->floors_frame));
}
