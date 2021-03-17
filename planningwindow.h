#ifndef PLANNINGWINDOW_H
#define PLANNINGWINDOW_H

#include <QWidget>
#include <vector>
#include <QMenuBar>
#include <QFileDialog>
#include <math.h>
#include "floorbutton.h"
#include "buildingcell.h"

const int EMPTY_VALUE = 32767;

namespace Ui {
class PlanningWindow;
}

class PlanningWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PlanningWindow(QWidget *parent = nullptr);
    ~PlanningWindow();
    void setupMenu();
    bool setSize(int height, int width);
    void showFloor(int floor_num);
    void changeFloorLabel();
    void analyzeDevices();
    void analyzeWalls();
    void resetCables();
    void saveFloorCables();
    void putEmptyFloorCables(int floor_num);

signals:
    void hidden();

private slots:

    void on_menu_but_clicked(bool checked);

    void on_floor_but_clicked();

    void on_openBuildingAct_triggered();

    void on_planning_but_clicked();

private:
    Ui::PlanningWindow *ui;
    QMenuBar *menu_bar;
    QMenu    *menu;
    int width_floor;
    int height_floor;
    int floor_number;
    std::vector<FloorButtonPlanning *>                 floors_but_vec;
    std::vector<std::vector<std::vector<Walls>>>       floors_walls;
    std::vector<std::vector<std::vector<Cables>>>      floors_cables;
    std::vector<std::vector<std::vector<int>>>         floors_devices;
    std::vector<std::vector<BuildingCellPlanning *>>   cell_vector;
    std::vector<QPoint>                                pcs_vector;
    std::vector<QPoint>                                switches_vector;
    std::vector<std::vector<Wall>>                     h_walls_vector;
    std::vector<std::vector<Wall>>                     v_walls_vector;
    int doPlanningNotAlongThrough();
    int doPlanningAlongThrough();
    int findNearestSwitch(const QPoint &pc, QPoint &sw);
    void findNearestWalls(const QPoint &pc, const QPoint &sw, Wall &nearest_wall_x, Wall &nearest_wall_y, int &nearest_wall_x_coord,int &nearest_wall_y_coord);
    void showCableBetweenX(int x0, int x1, int y, int cable_type, int r = 0, int g = 255, int b = 0);
    void showCableBetweenY(int y0, int y1, int x, int cable_type, int r = 0, int g = 255, int b = 0);
};

#endif // PLANNINGWINDOW_H
