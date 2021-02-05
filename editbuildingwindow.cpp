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

    const int n = 10;
    std::array<std::array<BuildingCell *, n>, n> cell_array;
    cell_array[0][0] = new BuildingCell(ui->edit_building_layout, 0, 0, CELL_TYPE_FULL);
    for (int i = 1; i < n; i++)
    {
        cell_array[0][i] = new BuildingCell(ui->edit_building_layout, 0, i, CELL_TYPE_TOP);
        cell_array[i][0] = new BuildingCell(ui->edit_building_layout, i, 0, CELL_TYPE_LEFT);
        for (int j = 1; j < n ; j++) cell_array[i][j] = new BuildingCell(ui->edit_building_layout, i, j, CELL_TYPE_WITHOUT);
    }

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
