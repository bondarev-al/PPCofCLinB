#ifndef EDITDEVICESINBULDINGWINDOW_H
#define EDITDEVICESINBULDINGWINDOW_H

#include <QWidget>
#include <vector>
#include <QMenuBar>
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
    void saveFloorDevices();

signals:
    void hidden();

private slots:

    void on_menu_but_clicked(bool checked);

    void on_floor_but_clicked();

    void on_openBuildingAct_triggered();

    void on_openAct_triggered();

    void on_saveAct_triggered();

    void on_saveAsAct_triggered();

    void on_devices_PC_but_clicked(bool checked);

    void on_devices_switch_but_clicked(bool checked);

    void on_cell_icon_pressed();

private:
    Ui::EditDevicesInBuldingWindow *ui;
    QMenuBar *menu_bar;
    QMenu    *menu;
    QAction  *saveAct;
    QAction  *saveAsAct;
    int width_floor;
    int height_floor;
    int floor_number;
    int type_of_device;
    QString fileName;
    std::vector<FloorButtonDevices *>                     floors_but_vec;
    std::vector<std::vector<std::vector<Walls>>>          floors_walls;
    std::vector<std::vector<std::vector<int>>>            floors_devices;
    std::vector<std::vector<BuildingCellDevices *>>       cell_vector;
};

#endif // EDITDEVICESINBULDINGWINDOW_H
