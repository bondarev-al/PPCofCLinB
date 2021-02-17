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
    floor_number = 0;
    floors_but_vec.push_back(new FloorButton(ui->floors_but_layout, ui->floors_frame));
    setSize(5, 4);
}

bool EditBuildingWindow::setSize(int height, int width)
{
    if ((width < 1) || (height < 1)) return false;
    width_floor  = width;
    height_floor = height;
    cell_vector.clear();
    floors_walls.clear();
    floors_walls.push_back(new bool[height_floor * width_floor * 4]);
    cell_vector.push_back(std::vector<BuildingCell *>());
    cell_vector[0].push_back(new BuildingCell(ui->edit_building_layout, 0, 0, CELL_TYPE_FULL));
    for (int i = 1; i < width_floor ; i++)  cell_vector[0].push_back(new BuildingCell(ui->edit_building_layout, 0, i, CELL_TYPE_TOP));
    for (int i = 1; i < height_floor; i++)
    {
        cell_vector.push_back(std::vector<BuildingCell *>());
        cell_vector[i].push_back(new BuildingCell(ui->edit_building_layout, i, 0, CELL_TYPE_LEFT));
        for (int j = 1; j < width_floor ; j++)  cell_vector[i].push_back(new BuildingCell(ui->edit_building_layout, i, j, CELL_TYPE_WITHOUT));
    }
    return true;
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

void EditBuildingWindow::saveFloorWalls()
{
//    for (int i = 0; i < height_floor; i++)
//        for (int j = 0; j < width_floor ; j++)
//        {
//            floors_walls[floor_number][i][j][0] = cell_vector[i][j].
//        }


}
