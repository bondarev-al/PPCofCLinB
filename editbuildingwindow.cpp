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
    floors_walls.push_back(std::vector<std::vector<Walls>>());
    putEmptyFloorWalls(floors_but_vec.back()->getFloorNumber());
    setSize(5, 4);
}

void EditBuildingWindow::setupMenu()
{
    menu_bar = new QMenuBar();
    menu     = new QMenu("Действия");
    menu_bar->addMenu(menu);
    QAction *createAct   = menu->addAction("Создать новый файл");
    QAction *openAct     = menu->addAction("Открыть файл");
             saveAct     = menu->addAction("Сохранить файл");
    QAction *saveAsAct   = menu->addAction("Сохранить как...");
    menu->addSeparator();
    QAction *sizeAct     = menu->addAction("Изменить сетку поля");
    connect(sizeAct,     SIGNAL(triggered()), this, SLOT(on_sizeAct_triggered()));
    connect(saveAct,     SIGNAL(triggered()), this, SLOT(on_saveAct_triggered()));
    connect(saveAsAct,   SIGNAL(triggered()), this, SLOT(on_saveAsAct_triggered()));
    connect(openAct,     SIGNAL(triggered()), this, SLOT(on_openAct_triggered()));
    connect(createAct,   SIGNAL(triggered()), this, SLOT(on_createAct_triggered()));
    ui->menu_layout->addWidget(menu_bar);
    saveAct->setEnabled(false);
}

bool EditBuildingWindow::setSize(int height, int width)
{
    if ((width < 1) || (height < 1)) return false;
    width_floor  = width;
    height_floor = height;
    cell_vector.clear();
    for (int i = 0; i < FloorButton::number_of_floors; i++)
    {
        floors_walls[i].resize(height_floor);
        for (int j = 0; j < height_floor; j++)
            floors_walls[i][j].resize(width_floor);
    }
    while (QLayoutItem* item = ui->edit_building_layout->takeAt(0))
    {
        delete item->widget();
        delete item;
    }
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
    putEmptyFloorWalls(floors_but_vec.back()->getFloorNumber());
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

void EditBuildingWindow::on_sizeAct_triggered()
{
    int height = QInputDialog::getInt(nullptr, "Ввод количества строк", "Количество строк:", height_floor, 1, 20);
    int width = QInputDialog::getInt(nullptr, "Ввод количества столбцов", "Количество столбцов:", width_floor, 1, 40);
    saveFloorWalls();
    setSize(height, width);
    showFloor(floor_number);
}

void EditBuildingWindow::on_createAct_triggered()
{
    QString name = QFileDialog::getSaveFileName(0, "Создать новый файл", "", "*.bld");
    if (name != "")
    {
        fileName = name;
        resetFloorWalls(0);
        while (QLayoutItem* item = ui->floors_but_layout->takeAt(0))
        {
            delete item->widget();
            delete item;
        }
        floors_but_vec.clear();
        floors_walls.clear();
        FloorButton::number_of_floors = 0;
        floors_but_vec.push_back(new FloorButton(ui->floors_but_layout, ui->floors_frame));
        connect(floors_but_vec.back(), SIGNAL(clicked()), this, SLOT(on_floor_but_clicked()));
        floors_walls.push_back(std::vector<std::vector<Walls>>());
        putEmptyFloorWalls(floors_but_vec.back()->getFloorNumber());
        on_saveAct_triggered();
        saveAct->setEnabled(true);
    }
}

void EditBuildingWindow::on_openAct_triggered()
{
    QString name = QFileDialog::getOpenFileName(0, "Открытие файла здания", "", "*.bld");
    if (name != "")
    {
        fileName = name;
        QFile file(fileName);
        int h, w, num;
        file.open(QIODevice::ReadOnly);
        file.read((char *)&h, sizeof(h));
        file.read((char *)&w, sizeof(w));
        file.read((char *)&num, sizeof(num));

        setSize(h, w);
        if (num >= FloorButton::number_of_floors)
            for (int i = FloorButton::number_of_floors; i < num ;  i++)
            {
                floors_but_vec.push_back(new FloorButton(ui->floors_but_layout, ui->floors_frame));
                floors_walls.push_back(std::vector<std::vector<Walls>>());
                connect(floors_but_vec.back(), SIGNAL(clicked()), this, SLOT(on_floor_but_clicked()));
            }
        else while ( FloorButton::number_of_floors > num )
            {
                FloorButton::number_of_floors--;
                floors_walls.pop_back();
                QLayoutItem* item = ui->floors_but_layout->takeAt(FloorButton::number_of_floors);
                delete item->widget();
                delete item;
                floors_but_vec.pop_back();
            }

        for (int floor = 0; floor < FloorButton::number_of_floors ; floor++)
        {
            floors_walls[floor].clear();
            for (int i = 0; i < height_floor; i++)
            {
                floors_walls[floor].push_back(std::vector<Walls>());
                for (int j = 0; j < width_floor ; j++)
                {
                    Walls walls;
                    file.read((char *)&walls, sizeof(Walls));
                    floors_walls[floor][i].push_back(walls);
                }
            }
        }
        showFloor(0);
        saveAct->setEnabled(true);
        file.close();
    }
}

void EditBuildingWindow::on_saveAct_triggered()
{
    if (fileName != "")
    {
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);

        file.write((char *)&height_floor, sizeof(height_floor));
        file.write((char *)&width_floor, sizeof(height_floor));
        file.write((char *)&FloorButton::number_of_floors, sizeof(FloorButton::number_of_floors));

        saveFloorWalls();

        for (int floor = 0; floor < FloorButton::number_of_floors ; floor++)
            for (int i = 0; i < height_floor; i++)
                for (int j = 0; j < width_floor ; j++)
                    file.write((char *)&floors_walls[floor][i][j], sizeof(Walls));
        file.close();
    }
}

void EditBuildingWindow::on_saveAsAct_triggered()
{
    QString name = QFileDialog::getSaveFileName(0, "Сохранить файл", "", "*.bld");
    if (name != "")
    {
        fileName = name;
        on_saveAct_triggered();
        saveAct->setEnabled(true);
    }
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

void EditBuildingWindow::putEmptyFloorWalls(int floor_num)
{
    floors_walls[floor_num].clear();
    for (int i = 0; i < height_floor; i++)
    {
        floors_walls[floor_num].push_back(std::vector<Walls>());
        for (int j = 0; j < width_floor ; j++)
        {
            Walls walls;
            walls.bottom_wall = false;
            walls.right_wall  = false;
            walls.left_wall   = false;
            walls.top_wall    = false;
            floors_walls[floor_num][i].push_back(walls);
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
