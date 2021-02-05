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

    BuildingCell *cell = new BuildingCell(CELL_TYPE_FULL);
    ui->edit_building_layout->addLayout(cell, 0, 0);
    BuildingCell *cell1 = new BuildingCell(CELL_TYPE_TOP);
    ui->edit_building_layout->addLayout(cell1, 0, 1);
    BuildingCell *cell2 = new BuildingCell(CELL_TYPE_LEFT);
    ui->edit_building_layout->addLayout(cell2, 1, 0);
    BuildingCell *cell3 = new BuildingCell(CELL_TYPE_WITHOUT);
    ui->edit_building_layout->addLayout(cell3, 1, 1);

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
