#ifndef EDITDEVICESINBULDINGWINDOW_H
#define EDITDEVICESINBULDINGWINDOW_H

#include <QWidget>
#include <vector>
#include <QMenuBar>
#include <QInputDialog>
#include <QFileDialog>
#include "floorbutton.h"
#include "buildingcell.h"

namespace Ui {
class EditDevicesInBuldingWindow;
}

class EditDevicesInBuldingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EditDevicesInBuldingWindow(QWidget *parent = nullptr);
    ~EditDevicesInBuldingWindow();
    bool setSize(int height, int width);
    void showFloor(int floor_num);
    void changeFloorLabel();
    void setupMenu();

signals:
    void hidden();

private slots:


    void on_menu_but_clicked(bool checked);

    void on_floor_but_clicked();

    void on_openBuildingAct_triggered();

private:
    Ui::EditDevicesInBuldingWindow *ui;
    QMenuBar *menu_bar;
    QMenu    *menu;
    int width_floor;
    int height_floor;
    int floor_number;
    QString fileName;
    std::vector<FloorButtonDevices *>                     floors_but_vec;
    std::vector<std::vector<std::vector<Walls>>>          floors_walls;
    std::vector<std::vector<BuildingCellDevices *>>       cell_vector;
};

#endif // EDITDEVICESINBULDINGWINDOW_H
