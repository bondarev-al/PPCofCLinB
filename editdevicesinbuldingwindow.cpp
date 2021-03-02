#include "editdevicesinbuldingwindow.h"
#include "ui_editdevicesinbuldingwindow.h"

EditDevicesInBuldingWindow::EditDevicesInBuldingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditDevicesInBuldingWindow)
{
    ui->setupUi(this);
    setupMenu();
    type_of_device = EMPTY;
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

bool EditDevicesInBuldingWindow::setSize(int height, int width)
{
    if ((width < 1) || (height < 1)) return false;
    width_floor  = width;
    height_floor = height;
    cell_vector.clear();
    while (QLayoutItem* item = ui->edit_building_layout->takeAt(0))
    {
        delete item->widget();
        delete item;
    }
    cell_vector.push_back(std::vector<BuildingCellDevices *>());
    cell_vector[0].push_back(new BuildingCellDevices(ui->edit_building_layout, 0, 0, CELL_TYPE_FULL));
    for (int i = 1; i < width_floor ; i++)  cell_vector[0].push_back(new BuildingCellDevices(ui->edit_building_layout, 0, i, CELL_TYPE_TOP));
    for (int i = 1; i < height_floor; i++)
    {
        cell_vector.push_back(std::vector<BuildingCellDevices *>());
        cell_vector[i].push_back(new BuildingCellDevices(ui->edit_building_layout, i, 0, CELL_TYPE_LEFT));
        for (int j = 1; j < width_floor ; j++)  cell_vector[i].push_back(new BuildingCellDevices(ui->edit_building_layout, i, j, CELL_TYPE_WITHOUT));
    }
    for (int i = 0; i < height_floor ; i++)
        for (int j = 0; j < width_floor ; j++)
            connect(cell_vector[i][j], SIGNAL(iconPressed()),this, SLOT(on_cell_icon_pressed()));
    return true;
}

void EditDevicesInBuldingWindow::showFloor(int floor_num)
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
            cell_vector[i][j]->setDeviceType(floors_devices[floor_number][i][j]);
        }
    changeFloorLabel();
}

void EditDevicesInBuldingWindow::changeFloorLabel()
{
    QString string = "Этаж - ";
    string += QString::number(floor_number + 1);
    ui->floor_num_lab->setText(string);
}

void EditDevicesInBuldingWindow::setupMenu()
{
    menu_bar = new QMenuBar();
    menu     = new QMenu("Действия");
    menu_bar->addMenu(menu);
    QAction *openBuildingAct   = menu->addAction("Открыть план здания");

    connect(openBuildingAct,     SIGNAL(triggered()), this, SLOT(on_openBuildingAct_triggered()));

    ui->menu_layout->addWidget(menu_bar);
}

void EditDevicesInBuldingWindow::saveFloorDevices()
{
    floors_devices[floor_number].clear();
    for (int i = 0; i < height_floor; i++)
    {
        floors_devices[floor_number].push_back(std::vector<int>());
        for (int j = 0; j < width_floor ; j++)
            floors_devices[floor_number][i].push_back(cell_vector[i][j]->getDeviceType());
    }
}

void EditDevicesInBuldingWindow::on_floor_but_clicked()
{
    saveFloorDevices();
    showFloor(static_cast<FloorButtonDevices *>(sender())->getFloorNumber());
}

void EditDevicesInBuldingWindow::on_openBuildingAct_triggered()
{
    QString name = QFileDialog::getOpenFileName(0, "Открытие плана здания", "", "*.bld");
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
        if (num >= FloorButtonDevices::number_of_floors)
            for (int i = FloorButtonDevices::number_of_floors; i < num ;  i++)
            {
                floors_but_vec.push_back(new FloorButtonDevices(ui->floors_but_layout, ui->floors_frame));
                floors_walls.push_back(std::vector<std::vector<Walls>>());
                floors_devices.push_back(std::vector<std::vector<int>>());
                connect(floors_but_vec.back(), SIGNAL(clicked()), this, SLOT(on_floor_but_clicked()));
            }
        else while ( FloorButtonDevices::number_of_floors > num )
            {
                FloorButtonDevices::number_of_floors--;
                floors_walls.pop_back();
                floors_devices.pop_back();
                QLayoutItem* item = ui->floors_but_layout->takeAt(FloorButtonDevices::number_of_floors);
                delete item->widget();
                delete item;
                floors_but_vec.pop_back();
            }

        for (int floor = 0; floor < FloorButtonDevices::number_of_floors ; floor++)
        {
            floors_walls[floor].clear();
            floors_devices[floor].clear();
            for (int i = 0; i < height_floor; i++)
            {
                floors_walls[floor].push_back(std::vector<Walls>());
                floors_devices[floor].push_back(std::vector<int>());
                for (int j = 0; j < width_floor ; j++)
                {
                    Walls walls;
                    file.read((char *)&walls, sizeof(Walls));
                    floors_walls[floor][i].push_back(walls);
                    floors_devices[floor][i].push_back(EMPTY);
                }
            }
        }
        showFloor(0);
        file.close();
    }
}

void EditDevicesInBuldingWindow::on_devices_PC_but_clicked(bool checked)
{
    if (checked)
    {
        type_of_device = PC;
        ui->devices_switch_but->setChecked(false);
    }
    else type_of_device = EMPTY;
}

void EditDevicesInBuldingWindow::on_devices_switch_but_clicked(bool checked)
{
    if (checked)
    {
        type_of_device = SWITCH;
        ui->devices_PC_but->setChecked(false);
    }
    else type_of_device = EMPTY;
}

void EditDevicesInBuldingWindow::on_cell_icon_pressed()
{
    BuildingCellDevices * cell = static_cast<BuildingCellDevices *>(sender());
    switch (type_of_device)
    {
        case EMPTY:
            cell->setDeviceType(EMPTY);
            break;
        case PC:
            cell->setDeviceType(PC);
            break;
        case SWITCH:
            cell->setDeviceType(SWITCH);
            break;
    }
}
