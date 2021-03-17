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
    saveFloorCables();
    showFloor(static_cast<FloorButtonPlanning *>(sender())->getFloorNumber());
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
                floors_cables.push_back(std::vector<std::vector<Cables>>());
                floors_devices.push_back(std::vector<std::vector<int>>());
                connect(floors_but_vec.back(), SIGNAL(clicked()), this, SLOT(on_floor_but_clicked()));
            }
        else while ( FloorButtonPlanning::number_of_floors > num )
            {
                FloorButtonPlanning::number_of_floors--;
                floors_walls.pop_back();
                floors_cables.pop_back();
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
        for (int floor = 0; floor < FloorButtonPlanning::number_of_floors ; floor++)
        {
            floors_cables[floor].clear();
            putEmptyFloorCables(floor);
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
            cell_vector[i][j]->resetCables();
            Cables cables = floors_cables[floor_number][i][j];
            if (cables.bottom_cable) cell_vector[i][j]->setCable(CABLE_BOTTOM, cables.bottom_color.r, cables.bottom_color.g, cables.bottom_color.b);
            if (cables.top_cable) cell_vector[i][j]->setCable(CABLE_TOP, cables.top_color.r, cables.top_color.g, cables.top_color.b);
            if (cables.right_cable) cell_vector[i][j]->setCable(CABLE_RIGHT, cables.right_color.r, cables.right_color.g, cables.right_color.b);
            if (cables.left_cable) cell_vector[i][j]->setCable(CABLE_LEFT, cables.left_color.r, cables.left_color.g, cables.left_color.b);
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

void PlanningWindow::saveFloorCables()
{
    floors_cables[floor_number].clear();
    for (int i = 0; i < height_floor; i++)
    {
        floors_cables[floor_number].push_back(std::vector<Cables>());
        for (int j = 0; j < width_floor ; j++)
            floors_cables[floor_number][i].push_back(cell_vector[i][j]->getCables());
    }
}

void PlanningWindow::putEmptyFloorCables(int floor_num)
{
    floors_cables[floor_num].clear();
    for (int i = 0; i < height_floor; i++)
    {
        floors_cables[floor_num].push_back(std::vector<Cables>());
        for (int j = 0; j < width_floor ; j++)
        {
            Cables cables;
            cables.bottom_cable = false;
            cables.right_cable  = false;
            cables.left_cable   = false;
            cables.top_cable    = false;
            floors_cables[floor_num][i].push_back(cables);
        }
    }
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
    v_walls_vector.clear();
    h_walls_vector.clear();
    for (int i = 0; i <= height_floor ; i++)
        h_walls_vector.push_back(std::vector<Wall>());
    for (int j = 0; j <=  width_floor ; j++)
        v_walls_vector.push_back(std::vector<Wall>());
    bool wall  = false;
    int  beg   = -1;

    for(int i = 0; i < height_floor ; i++)
    {
        wall  = cell_vector[i][0]->isLeftLineWall();
        if (wall && (beg == -1)) beg = i;
        else if (!wall && (beg != -1))
        {
            v_walls_vector[0].push_back(Wall(beg, i - 1));
            beg = -1;
        }
    }
    if (beg != -1) v_walls_vector[0].push_back(Wall(beg, height_floor - 1));
    for (int j = 0; j < width_floor ; j++)
    {
        beg = -1;
        for(int i = 0; i < height_floor ; i++)
        {
            wall  = cell_vector[i][j]->isRightLineWall();
            if (wall && (beg == -1)) beg = i;
            else if (!wall && (beg != -1))
            {
                v_walls_vector[j + 1].push_back(Wall(beg, i - 1));
                beg = -1;
            }
        }
        if (beg != -1) v_walls_vector[j + 1].push_back(Wall(beg, height_floor - 1));
    }

    for(int j = 0; j < width_floor ; j++)
    {
        wall  = cell_vector[0][j]->isTopLineWall();
        if (wall && (beg == -1)) beg = j;
        else if (!wall && (beg != -1))
        {
            h_walls_vector[0].push_back(Wall(beg, j - 1));
            beg = -1;
        }
    }
    if (beg != -1) h_walls_vector[0].push_back(Wall(beg, width_floor - 1));
    for (int i = 0; i < height_floor ; i++)
    {
        beg = -1;
        for(int j = 0; j < width_floor ; j++)
        {
            wall  = cell_vector[i][j]->isBottomLineWall();
            if (wall && (beg == -1)) beg = j;
            else if (!wall && (beg != -1))
            {
                h_walls_vector[i + 1].push_back(Wall(beg, j - 1));
                beg = -1;
            }
        }
        if (beg != -1) h_walls_vector[i + 1].push_back(Wall(beg, width_floor - 1));
    }
}

void PlanningWindow::on_planning_but_clicked()
{
    resetCables();
    analyzeDevices();
    int cable_quanity = 0;
    if (!ui->along_walls_but->isChecked())
    {
        cable_quanity = doPlanningNotAlongThrough();
    }
    else
    {
        analyzeWalls();
        cable_quanity = doPlanningAlongThrough();
    }
}

int PlanningWindow::findNearestSwitch(const QPoint &pc, QPoint &nearest_switch)
{
    int number_of_switches = switches_vector.size();
    int min_distance = EMPTY_VALUE;
    for (int j = 0; j < number_of_switches ; j++)
    {
        const QPoint sw = switches_vector[j];
        int distance = abs(sw.x() - pc.x()) + abs(sw.y() - pc.y());
        if (distance < min_distance)
        {
            min_distance = distance;
            nearest_switch.setX(sw.x());
            nearest_switch.setY(sw.y());
        }
    }
    return min_distance;
}

void PlanningWindow::findNearestWalls(const QPoint &pc, const QPoint &sw, Wall &nearest_wall_x, Wall &nearest_wall_y, int &nearest_wall_x_coord,int &nearest_wall_y_coord)
{
    nearest_wall_x_coord = EMPTY_VALUE;
    nearest_wall_y_coord = EMPTY_VALUE;
    if (sw.y() >= pc.y())
    {
        bool found_wall = false;
        int begin = pc.y() + 1;
        int end = sw.y() + 1;
        if (sw.y() == pc.y()) begin = pc.y();
        for (int i = begin; (i <= end) && !found_wall ; i++)
            if (!v_walls_vector[i].empty())
                for (int j = 0 ; (j < v_walls_vector[i].size()) && !found_wall ; j++)
                    if (v_walls_vector[i][j].between(pc.x()))
                    {
                        found_wall = true;
                        nearest_wall_y = v_walls_vector[i][j];
                        nearest_wall_y_coord = i;
                    }
    }
    else
    {
        bool found_wall = false;
        for (int i = pc.y(); (i >= sw.y()) && !found_wall ; i--)
            if (!v_walls_vector[i].empty())
                for (int j = 0 ; (j < v_walls_vector[i].size()) && !found_wall ; j++)
                    if (v_walls_vector[i][j].between(pc.x()))
                    {
                        found_wall = true;
                        nearest_wall_y = v_walls_vector[i][j];
                        nearest_wall_y_coord = i;
                    }
    }
    if (sw.x() >= pc.x())
    {
        bool found_wall = false;
        int begin = pc.x() + 1;
        int end = sw.x() + 1;
        if (sw.x() == pc.x()) begin = pc.x();
        for (int i = begin; (i <= end) && !found_wall ; i++)
            if (!h_walls_vector[i].empty())
                for (int j = 0 ; (j < h_walls_vector[i].size()) && !found_wall ; j++)
                    if (h_walls_vector[i][j].between(pc.y()))
                    {
                        found_wall = true;
                        nearest_wall_x = h_walls_vector[i][j];
                        nearest_wall_x_coord = i;
                    }
    }
    else
    {
        bool found_wall = false;
        for (int i = pc.x(); (i >= sw.x()) && !found_wall ; i--)
            if (!h_walls_vector[i].empty())
                for (int j = 0 ; (j < h_walls_vector[i].size()) && !found_wall ; j++)
                    if (h_walls_vector[i][j].between(pc.y()))
                    {
                        found_wall = true;
                        nearest_wall_x = h_walls_vector[i][j];
                        nearest_wall_x_coord = i;
                    }
    }
}

void PlanningWindow::showCableBetweenX(int x0, int x1, int y, int cable_type, int r, int g, int b)
{
    if ((cable_type != CABLE_LEFT) && (cable_type != CABLE_RIGHT)) return;
    if (x1 < x0)
    {
        int temp = x0;
        x0 = x1;
        x1 = temp;
    }
    for (int i = x0; i <= x1 ; i++)
         cell_vector[i][y]->setCable(cable_type, r, g, b);
}

void PlanningWindow::showCableBetweenY(int y0, int y1, int x, int cable_type, int r, int g, int b)
{
    if ((cable_type != CABLE_TOP) && (cable_type != CABLE_BOTTOM)) return;
    if (y1 < y0)
    {
        int temp = y0;
        y0 = y1;
        y1 = temp;
    }
    for (int j = y0; j <= y1 ; j++)
         cell_vector[x][j]->setCable(cable_type, r, g, b);

}

int PlanningWindow::doPlanningNotAlongThrough()
{
    int number_of_pcs = pcs_vector.size();
    int cable_quanity = 0;
    int color_g = 255;
    int color_b = 0;
    for (int i = 0; i < number_of_pcs ; i++)
    {
        QPoint pc = pcs_vector[i];
        QPoint nearest_switch;
        int min_distance = findNearestSwitch(pc, nearest_switch);
        if (min_distance != EMPTY_VALUE)
        {
            cable_quanity += min_distance;
            if (nearest_switch.x() > pc.x())
            {
                if (nearest_switch.y() > pc.y())
                {
                    showCableBetweenY(pc.y() + 1, nearest_switch.y(), pc.x(), CABLE_TOP, 0, color_g, color_b);
                    showCableBetweenX(pc.x(), nearest_switch.x() - 1, nearest_switch.y(), CABLE_RIGHT, 0, color_g, color_b);
                }
                else if (nearest_switch.y() < pc.y())
                {
                    showCableBetweenY(pc.y() - 1, nearest_switch.y(), pc.x(), CABLE_TOP, 0, color_g, color_b);
                    showCableBetweenX(pc.x(), nearest_switch.x() - 1, nearest_switch.y(), CABLE_LEFT, 0, color_g, color_b);
                }
                else
                    showCableBetweenX(pc.x() + 1, nearest_switch.x(), nearest_switch.y(), CABLE_RIGHT, 0, color_g, color_b);
            }
            else if (nearest_switch.x() < pc.x())
            {
                if (nearest_switch.y() > pc.y())
                {
                    showCableBetweenY(pc.y() + 1, nearest_switch.y(), pc.x(), CABLE_BOTTOM, 0, color_g, color_b);
                    showCableBetweenX(pc.x(), nearest_switch.x() + 1, nearest_switch.y(), CABLE_RIGHT, 0, color_g, color_b);
                }
                else if (nearest_switch.y() < pc.y())
                {
                    showCableBetweenY(pc.y() - 1, nearest_switch.y(), pc.x(), CABLE_BOTTOM, 0, color_g, color_b);
                    showCableBetweenX(pc.x(), nearest_switch.x() + 1, nearest_switch.y(), CABLE_LEFT, 0, color_g, color_b);
                }
                else
                    showCableBetweenX(pc.x() - 1, nearest_switch.x(), nearest_switch.y(), CABLE_RIGHT, 0, color_g, color_b);
            }
            else
            {
                if (nearest_switch.y() > pc.y())
                    showCableBetweenY(pc.y() + 1, nearest_switch.y(), pc.x(), CABLE_BOTTOM, 0, color_g, color_b);
                else
                    showCableBetweenY(pc.y() - 1, nearest_switch.y(), pc.x(), CABLE_BOTTOM, 0, color_g, color_b);
            }
            if (color_g > 100) color_g -= 60;
            else if (color_b < 200) color_b += 60;
            else
            {
                color_g = 255;
                color_b = 0;
            }
        }
    }
    return cable_quanity;
}

int PlanningWindow::doPlanningAlongThrough()
{
    int number_of_pcs = pcs_vector.size();
    int cable_quanity = 0;
    int color_g = 255;
    int color_b = 0;
    for (int i = 0; i < number_of_pcs ; i++)
    {
        QPoint pc = pcs_vector[i];
        QPoint nearest_switch;
        int min_distance = findNearestSwitch(pc, nearest_switch);
        if (min_distance != EMPTY_VALUE)
        {
            Wall nearest_wall_x(EMPTY_VALUE, EMPTY_VALUE);
            Wall nearest_wall_y(EMPTY_VALUE, EMPTY_VALUE);
            int nearest_wall_x_coord;
            int nearest_wall_y_coord;
            bool reached_x = false;
            bool reached_y = false;
            int  v_cable_type, h_cable_type;
            if (nearest_switch.x() >= pc.x()) h_cable_type = CABLE_BOTTOM;
            else h_cable_type = CABLE_TOP;
            if (nearest_switch.y() >= pc.y()) v_cable_type = CABLE_RIGHT;
            else v_cable_type = CABLE_LEFT;
            QPoint position(pc.x(), pc.y());
            while (!reached_x || !reached_y)
            {
                findNearestWalls(position, nearest_switch, nearest_wall_x, nearest_wall_y, nearest_wall_x_coord, nearest_wall_y_coord);
                if ((nearest_wall_x_coord != EMPTY_VALUE) || (nearest_wall_y_coord != EMPTY_VALUE))
                {
                    if (nearest_wall_x_coord > position.x()) nearest_wall_x_coord--;
                    if (nearest_wall_y_coord > position.y()) nearest_wall_y_coord--;
                    int distance_x = abs(position.x() - nearest_wall_x_coord);
                    int distance_y = abs(position.y() - nearest_wall_y_coord);
                    if (distance_x <= distance_y)
                    {
                        if (distance_x != 0)
                        {
                            int begin;
                            if (nearest_wall_x_coord > position.x()) begin = position.x() + 1;
                            else begin = position.x() - 1;
                            int end = nearest_wall_x_coord;
                            showCableBetweenX(begin, end, position.y(), v_cable_type, 0, color_g, color_b);
                            cable_quanity += distance_x;
                            position.setX(end);
                        }
                        if (nearest_switch.y() != position.y())
                        {
                            int begin;
                            if (nearest_switch.y() > position.y()) begin = position.y() + 1;
                            else begin = position.y() - 1;
                            int end;
                            if (nearest_switch.y() > position.y())
                            {
                                if (nearest_switch.y() <= nearest_wall_x.getEnd()) end = nearest_switch.y();
                                else end = nearest_wall_x.getEnd() + 1;
                            }
                            else
                            {
                                if (nearest_switch.y() >= nearest_wall_x.getBegin()) end = nearest_switch.y();
                                else end = nearest_wall_x.getBegin() - 1;
                            }
                            showCableBetweenY(begin, end, position.x(), h_cable_type, 0, color_g, color_b);
                            cable_quanity += abs(end - begin) + 1;
                            position.setY(end);
                        }
                        else if (nearest_switch.x() != position.x())
                        {
                            int begin;
                            if (nearest_switch.x() > position.x()) begin = position.x() + 1;
                            else begin = position.x() - 1;
                            showCableBetweenX(begin, begin, position.y(), v_cable_type, 0, color_g, color_b);
                            cable_quanity++;
                            position.setX(begin);
                        }
                    }
                    else
                    {
                        if (distance_y != 0)
                        {
                            int begin;
                            if (nearest_wall_y_coord > position.y()) begin = position.y() + 1;
                            else begin = position.y() - 1;
                            int end = nearest_wall_y_coord;
                            showCableBetweenY(begin, end, position.x(), h_cable_type, 0, color_g, color_b);
                            cable_quanity += distance_y;
                            position.setY(end);
                        }
                        if (nearest_switch.x() != position.x())
                        {
                            int begin;
                            if (nearest_switch.x() > position.x()) begin = position.x() + 1;
                            else begin = position.x() - 1;
                            int end;
                            if (nearest_switch.x() > position.x())
                            {
                                if (nearest_switch.x() <= nearest_wall_y.getEnd()) end = nearest_switch.x();
                                else end = nearest_wall_y.getEnd() + 1;
                            }
                            else
                            {
                                if (nearest_switch.x() >= nearest_wall_y.getBegin()) end = nearest_switch.x();
                                else end = nearest_wall_y.getBegin() - 1;
                            }
                            showCableBetweenX(begin, end, position.y(), v_cable_type, 0, color_g, color_b);
                            cable_quanity += abs(end - begin) + 1;
                            position.setX(end);
                        }
                        else if (nearest_switch.y() != position.y())
                        {
                            int begin;
                            if (nearest_switch.y() > position.y()) begin = position.y() + 1;
                            else begin = position.y() - 1;
                            showCableBetweenY(begin, begin, position.x(), h_cable_type, 0, color_g, color_b);
                            cable_quanity++;
                            position.setY(begin);
                        }
                    }
                }
                else
                {
                    if (position.x() != nearest_switch.x())
                    {
                        int begin;
                        if (nearest_switch.x() > pc.x()) begin = pc.x() + 1;
                        else begin = pc.x() - 1;
                        int end = nearest_switch.x();
                        showCableBetweenX(begin, end, position.y(), v_cable_type, 0, color_g, color_b);
                        cable_quanity += abs(end - begin) + 1;
                        position.setX(end);
                    }
                    if (position.y() != nearest_switch.y())
                    {
                        int begin;
                        if (nearest_switch.y() > position.y()) begin = pc.y() + 1;
                        else begin = position.y() - 1;
                        int end = nearest_switch.y();
                        showCableBetweenY(begin, end, position.x(), h_cable_type, 0, color_g, color_b);
                        cable_quanity += abs(end - begin) + 1;
                        position.setY(end);
                    }

                }
                if (nearest_switch.x() == position.x()) reached_x = true;
                if (nearest_switch.y() == position.y()) reached_y = true;
            }

            if (color_g > 100) color_g -= 60;
            else if (color_b < 200) color_b += 60;
            else
            {
                color_g = 255;
                color_b = 0;
            }
        }
    }
    return cable_quanity;
}
