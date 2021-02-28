#ifndef BUILDINGCELLDEVICES_H
#define BUILDINGCELLDEVICES_H

#include <QBoxLayout>
#include <QFrame>
#include <QSpacerItem>
#include "buildingcell.h"

class CellLineDevices: public QFrame
{
    Q_OBJECT
protected:
    bool wall;
public:
    explicit CellLineDevices(QWidget *parent = nullptr);
    bool isWall(){ return wall; }
    bool setWall(bool w);
    void repaintLine();
};

class CellVLineDevices: public CellLineDevices
{
    Q_OBJECT
public:
    explicit CellVLineDevices(QWidget *parent = nullptr);
};

class CellHLineDevices: public CellLineDevices
{
    Q_OBJECT
public:
    explicit CellHLineDevices(QWidget *parent = nullptr);
};

class BuildingCellDevices: public QVBoxLayout
{
    Q_OBJECT
private:
    CellVLineDevices *left_line;
    CellVLineDevices *right_line;
    CellHLineDevices *top_line;
    CellHLineDevices *bottom_line;
    QHBoxLayout *central_layout;
    int type;
    int changeType();
public:
    explicit BuildingCellDevices(QGridLayout *layout, int row, int colown, int cell_type, QWidget *parent = nullptr);
    int setType(int cell_type);
    void setWalls(bool bottom_wall = false, bool right_wall = false, bool left_wall = false, bool top_wall = false);
    bool isLeftLineWall(){return left_line->isWall();}
    bool isRightLineWall(){return right_line->isWall();}
    bool isTopLineWall(){return top_line->isWall();}
    bool isBottomLineWall(){return bottom_line->isWall();}
    void repaintLines();
    ~BuildingCellDevices();
};

#endif // BUILDINGCELLDEVICES_H
