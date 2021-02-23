#ifndef EDITBUILDINGWINDOW_H
#define EDITBUILDINGWINDOW_H

#include <QWidget>
#include <vector>
#include <array>
#include <QPushButton>
#include <QMenuBar>
#include "floorbutton.h"
#include "buildingcell.h"

namespace Ui {
class EditBuildingWindow;
}

class EditBuildingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EditBuildingWindow(QWidget *parent = nullptr);
    bool setSize(int height, int width);
    ~EditBuildingWindow();
    void saveFloorWalls();
    void showFloor(int floor_num);
    void resetFloorWalls(int floor_num);

signals:
    void hidden();

private slots:

    void on_menu_but_clicked(bool checked);

    void on_plus_floor_but_clicked();

    void on_floor_but_clicked();

private:
    Ui::EditBuildingWindow *ui;
//    QMenuBar *menu_bar;
//    QMenu    *menu;
    int width_floor;
    int height_floor;
    int floor_number;
    std::vector<FloorButton *>                            floors_but_vec;
    std::vector<std::vector<std::vector<Walls>>>          floors_walls;
    std::vector<std::vector<BuildingCell *>>              cell_vector;
};

#endif // EDITBUILDINGWINDOW_H
