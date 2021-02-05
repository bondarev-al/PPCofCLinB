#ifndef EDITBUILDINGWINDOW_H
#define EDITBUILDINGWINDOW_H

#include <QWidget>
#include <vector>
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
    ~EditBuildingWindow();

signals:
    void hidden();

private slots:

    void on_menu_but_clicked(bool checked);

    void on_plus_floor_but_clicked();

private:
    Ui::EditBuildingWindow *ui;
//    QMenuBar *menu_bar;
//    QMenu    *menu;
    std::vector<FloorButton *> floors_but_vec;
};

#endif // EDITBUILDINGWINDOW_H
