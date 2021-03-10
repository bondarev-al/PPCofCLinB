#include "planningwindow.h"
#include "ui_planningwindow.h"

PlanningWindow::PlanningWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlanningWindow)
{
    ui->setupUi(this);
    setupMenu();
}

PlanningWindow::~PlanningWindow()
{
    delete ui;
}

void PlanningWindow::on_menu_but_clicked(bool checked)
{
    hide();
    emit hidden();
}

void PlanningWindow:: on_floor_but_clicked()
{
    showFloor(static_cast<FloorButtonDevices *>(sender())->getFloorNumber());
}

void PlanningWindow::on_openBuildingAct_triggered()
{
    QString name = QFileDialog::getOpenFileName(0, "Открытие плана здания с устройствами", "", "*.dev");
    if (name != "")
    {
        QFile file(name);
        int h, w, num;
        file.open(QIODevice::ReadOnly);
        file.read((char *)&h, sizeof(h));
        file.read((char *)&w, sizeof(w));
        file.read((char *)&num, sizeof(num));

        if (!cell_vector.empty()) resetCables();
        setSize(h, w);
        if (num >= FloorButtonPlanning::number_of_floors)
            for (int i = FloorButtonPlanning::number_of_floors; i < num ;  i++)
            {
                floors_but_vec.push_back(new FloorButtonPlanning(ui->floors_but_layout, ui->floors_frame));
                floors_walls.push_back(std::vector<std::vector<Walls>>());
                floors_devices.push_back(std::vector<std::vector<int>>());
                connect(floors_but_vec.back(), SIGNAL(clicked()), this, SLOT(on_floor_but_clicked()));
            }
        else while ( FloorButtonPlanning::number_of_floors > num )
            {
                FloorButtonPlanning::number_of_floors--;
                floors_walls.pop_back();
                floors_devices.pop_back();
                QLayoutItem* item = ui->floors_but_layout->takeAt(FloorButtonPlanning::number_of_floors);
                delete item->widget();
                delete item;
                floors_but_vec.pop_back();
            }

        for (int floor = 0; floor < FloorButtonPlanning::number_of_floors ; floor++)
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
                    int   type;
                    file.read((char *)&walls, sizeof(Walls));
                    file.read((char *)&type, sizeof(int));
                    floors_walls[floor][i].push_back(walls);
                    floors_devices[floor][i].push_back(type);
                }
            }
        }
        showFloor(0);
        ui->planning_but->setEnabled(true);
        file.close();
    }
}

void PlanningWindow::setupMenu()
{
    menu_bar = new QMenuBar();
    menu     = new QMenu("Действия");
    menu_bar->addMenu(menu);
    QAction *openBuildingAct = menu->addAction("Открыть план здания с устройствами");
    connect(openBuildingAct,     SIGNAL(triggered()), this, SLOT(on_openBuildingAct_triggered()));
    ui->menu_layout->addWidget(menu_bar);
}

bool PlanningWindow::setSize(int height, int width)
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
    cell_vector.push_back(std::vector<BuildingCellPlanning *>());
    cell_vector[0].push_back(new BuildingCellPlanning(ui->edit_building_layout, 0, 0, CELL_TYPE_FULL));
    for (int i = 1; i < width_floor ; i++)  cell_vector[0].push_back(new BuildingCellPlanning(ui->edit_building_layout, 0, i, CELL_TYPE_TOP));
    for (int i = 1; i < height_floor; i++)
    {
        cell_vector.push_back(std::vector<BuildingCellPlanning *>());
        cell_vector[i].push_back(new BuildingCellPlanning(ui->edit_building_layout, i, 0, CELL_TYPE_LEFT));
        for (int j = 1; j < width_floor ; j++)  cell_vector[i].push_back(new BuildingCellPlanning(ui->edit_building_layout, i, j, CELL_TYPE_WITHOUT));
    }
    return true;
}

void PlanningWindow::showFloor(int floor_num)
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

void PlanningWindow::changeFloorLabel()
{
    QString string = "Этаж - ";
    string += QString::number(floor_number + 1);
    ui->floor_num_lab->setText(string);
}

void PlanningWindow::resetCables()
{
    for (int i = 0; i < height_floor; i++)
        for (int j = 0; j < width_floor ; j++)
            cell_vector[i][j]->resetCables();
}

void PlanningWindow::analyzeDevices()
{
    pcs_vector.clear();
    switches_vector.clear();
    for (int i = 0; i < height_floor; i++)
        for (int j = 0; j < width_floor; j++)
            switch (floors_devices[floor_number][i][j])
            {
                case PC:
                    pcs_vector.push_back(QPoint(i, j));
                    break;
                case SWITCH:
                    switches_vector.push_back(QPoint(i, j));
                    break;
            }
}

void PlanningWindow::analyzeWalls()
{

}

void PlanningWindow::on_planning_but_clicked()
{
    resetCables();
    analyzeDevices();
    int cable_quanity = 0;
    if (!ui->along_walls_but->isChecked() && ui->through_walls_but->isChecked())
    {
        cable_quanity = doPlanningNotAlongThrough();
    }
}

int PlanningWindow::doPlanningNotAlongThrough()
{
    int number_of_pcs      = pcs_vector.size();
    int number_of_switches = switches_vector.size();
    int cable_quanity = 0;
    for (int i = 0; i < number_of_pcs ; i++)
    {
        QPoint pc = pcs_vector[i];
        QPoint nearest_switch;
        int min_distance = MAX_VALUE;
        for (int j = 0; j < number_of_switches ; j++)
        {
            QPoint sw = switches_vector[j];
            int distance = abs(sw.x() - pc.x()) + abs(sw.y() - pc.y());
            if (distance < min_distance)
            {
                min_distance = distance;
                nearest_switch = sw;
            }
        }
        if (min_distance != MAX_VALUE)
        {
            cable_quanity += min_distance;
            if (nearest_switch.x() > pc.x())
            {
                if (nearest_switch.y() > pc.y())
                {
                    for (int j = pc.y() + 1; j <= nearest_switch.y() ; j++)
                        cell_vector[pc.x()][j]->setCable(CABLE_TOP);
                    for (int i = pc.x(); i < nearest_switch.x() ; i++)
                         cell_vector[i][nearest_switch.y()]->setCable(CABLE_RIGHT);
                }
                else if (nearest_switch.y() < pc.y())
                {
                    for (int j = pc.y() - 1; j >= nearest_switch.y() ; j--)
                        cell_vector[pc.x()][j]->setCable(CABLE_TOP);
                    for (int i = pc.x(); i < nearest_switch.x() ; i++)
                         cell_vector[i][nearest_switch.y()]->setCable(CABLE_LEFT);
                }
                else
                    for (int i = pc.x() + 1; i <= nearest_switch.x() ; i++)
                         cell_vector[i][nearest_switch.y()]->setCable(CABLE_RIGHT);
            }
            else if (nearest_switch.x() < pc.x())
            {
                if (nearest_switch.y() > pc.y())
                {
                    for (int j = pc.y() + 1; j <= nearest_switch.y() ; j++)
                        cell_vector[pc.x()][j]->setCable(CABLE_BOTTOM);
                    for (int i = pc.x(); i > nearest_switch.x() ; i--)
                         cell_vector[i][nearest_switch.y()]->setCable(CABLE_RIGHT);
                }
                else if (nearest_switch.y() < pc.y())
                {
                    for (int j = pc.y() - 1; j >= nearest_switch.y() ; j--)
                        cell_vector[pc.x()][j]->setCable(CABLE_BOTTOM);
                    for (int i = pc.x(); i > nearest_switch.x() ; i--)
                         cell_vector[i][nearest_switch.y()]->setCable(CABLE_LEFT);
                }
                else
                    for (int i = pc.x() - 1; i >= nearest_switch.x() ; i--)
                         cell_vector[i][nearest_switch.y()]->setCable(CABLE_RIGHT);
            }
            else
            {
                if (nearest_switch.y() > pc.y())
                    for (int j = pc.y() + 1; j <= nearest_switch.y() ; j++)
                        cell_vector[pc.x()][j]->setCable(CABLE_BOTTOM);
                else
                    for (int j = pc.y() - 1; j >= nearest_switch.y() ; j--)
                        cell_vector[pc.x()][j]->setCable(CABLE_BOTTOM);
            }
        }
    }
    return cable_quanity;
}
