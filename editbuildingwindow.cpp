#include "editbuildingwindow.h"
#include "ui_editbuildingwindow.h"

EditBuildingWindow::EditBuildingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditBuildingWindow)
{
    ui->setupUi(this);
    setupMenu();
    floor_number = 0;
    floors_but_vec.push_back(new FloorButton(ui->floors_but_layout, ui->floors_frame));
    connect(floors_but_vec.back(), SIGNAL(clicked()), this, SLOT(on_floor_but_clicked()));
    setSize(5, 4);
    floors_walls.push_back(std::vector<std::vector<Walls>>());
}

void EditBuildingWindow::setupMenu()
{
    menu_bar = new QMenuBar();
    menu     = new QMenu("Действия");
    menu_bar->addMenu(menu);
    menu->addAction("Создать новый файл");
    menu->addAction("Открыть файл");
    menu->addAction("Сохранить файл");
    menu->addSeparator();
    menu->addAction("Изменить сетку поля");
    ui->menu_layout->addWidget(menu_bar);
}

bool EditBuildingWindow::setSize(int height, int width)
{
    if ((width < 1) || (height < 1)) return false;
    width_floor  = width;
    height_floor = height;
    cell_vector.clear();
    floors_walls.clear();
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
    floors_walls.push_back(std::vector<std::vector<Walls>>());
    saveFloorWalls();
    resetFloorWalls(floors_but_vec.back()->getFloorNumber());
    changeFloorLabel();
    connect(floors_but_vec.back(), SIGNAL(clicked()), this, SLOT(on_floor_but_clicked()));
}

void EditBuildingWindow::on_floor_but_clicked()
{
    saveFloorWalls();
    showFloor(static_cast<FloorButton *>(sender())->getFloorNumber());
}

void EditBuildingWindow::saveFloorWalls()
{
    floors_walls[floor_number].clear();
    for (int i = 0; i < height_floor; i++)
    {
        floors_walls[floor_number].push_back(std::vector<Walls>());
        for (int j = 0; j < width_floor ; j++)
        {
            Walls walls;
            walls.bottom_wall = cell_vector[i][j]->isBottomLineWall();
            walls.right_wall  = cell_vector[i][j]->isRightLineWall();
            walls.left_wall   = cell_vector[i][j]->isLeftLineWall();
            walls.top_wall    = cell_vector[i][j]->isTopLineWall();
            floors_walls[floor_number][i].push_back(walls);
        }
    }
}

void EditBuildingWindow::resetFloorWalls(int floor_num)
{
    floor_number = floor_num;
    for (int i = 0; i < height_floor; i++)
        for (int j = 0; j < width_floor ; j++)
        {
            cell_vector[i][j]->setWalls();
            cell_vector[i][j]->repaintLines();
        }
}

void EditBuildingWindow::showFloor(int floor_num)
{
    floor_number = floor_num;
    for (int i = 0; i < height_floor; i++)
        for (int j = 0; j < width_floor ; j++)
        {
            cell_vector[i][j]->setWalls(floors_walls[floor_number][i][j].bottom_wall,
                                        floors_walls[floor_number][i][j].right_wall,
                                        floors_walls[floor_number][i][j].left_wall,
                                        floors_walls[floor_number][i][j].top_wall);
            cell_vector[i][j]->repaintLines();
        }
    changeFloorLabel();
}

void EditBuildingWindow::changeFloorLabel()
{
    QString string = "Этаж - ";
    string += QString::number(floor_number + 1);
    ui->floor_num_lab->setText(string);
}
