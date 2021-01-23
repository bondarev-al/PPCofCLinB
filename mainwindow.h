#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <editbuildingwindow.h>
#include <editdevicesinbuldingwindow.h>
#include <planningwindow.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_edit_building_but_clicked(bool checked);

    void on_edit_devices_but_clicked(bool checked);

    void on_planning_but_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    EditBuildingWindow *edit_building_window;
    EditDevicesInBuldingWindow *edit_devices_in_bulding_window;
    PlanningWindow *planning_window;
};
#endif // MAINWINDOW_H
