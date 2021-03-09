#ifndef PLANNINGWINDOW_H
#define PLANNINGWINDOW_H

#include <QWidget>
#include <vector>
#include <QMenuBar>
#include <QFileDialog>
#include "floorbutton.h"
#include "buildingcell.h"

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
    std::vector<FloorButtonPlanning *>                    floors_but_vec;
    std::vector<std::vector<std::vector<Walls>>>          floors_walls;
    std::vector<std::vector<std::vector<int>>>            floors_devices;
    std::vector<std::vector<BuildingCellPlanning *>>      cell_vector;
    std::vector<QPoint>                                   pcs_vector;
    std::vector<QPoint>                                   switches_vector;
};

#endif // PLANNINGWINDOW_H
