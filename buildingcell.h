#ifndef BUILDINGCELL_H
#define BUILDINGCELL_H

#include <QBoxLayout>
#include <QFrame>
#include <QSpacerItem>
#include <QLabel>
#include <QPicture>

const int CELL_TYPE_FULL       =  0;
const int CELL_TYPE_TOP        =  1;
const int CELL_TYPE_LEFT       =  2;
const int CELL_TYPE_WITHOUT    =  3;
const int CELL_LINE_WIDTH      =  1;
const int ICON_HEIGHT          = 25;
const QString PAINTED_LINE_STYLE = "background-color: rgb(255, 0, 0);";
const QString PC_ICON            = ":/icons/PC.png";
const QString EMPTY_ICON         = ":/icons/empty.png";
const QString SWITCH_ICON        = ":/icons/switch.png";
const int EMPTY  = -1;
const int PC     =  0;
const int SWITCH =  1;
const int CABLE_LEFT      = 1;
const int CABLE_TOP       = 2;
const int CABLE_RIGHT     = 3;
const int CABLE_BOTTOM    = 4;

struct Walls
{
    bool bottom_wall;
    bool right_wall;
    bool left_wall;
    bool top_wall;
};

struct Color
{
    int r;
    int g;
    int b;
};

struct Cables
{
    bool bottom_cable;
    bool right_cable;
    bool left_cable;
    bool top_cable;
    Color bottom_color;
    Color right_color;
    Color left_color;
    Color top_color;
};

class CellLineWithoutMouse: public QFrame
{
    Q_OBJECT
protected:
    bool wall;
public:
    explicit CellLineWithoutMouse(QWidget *parent = nullptr);
    bool isWall(){ return wall; }
    bool setWall(bool w);
    void repaintLine();
};

class CellVLineWithoutMouse: public CellLineWithoutMouse
{
    Q_OBJECT
public:
    explicit CellVLineWithoutMouse(QWidget *parent = nullptr);
};

class CellHLineWithoutMouse: public CellLineWithoutMouse
{
    Q_OBJECT
public:
    explicit CellHLineWithoutMouse(QWidget *parent = nullptr);
};

class CellLine: public CellLineWithoutMouse
{
    Q_OBJECT
protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
public:
    explicit CellLine(QWidget *parent = nullptr);
};

class CellVLine: public CellLine
{
    Q_OBJECT
public:
    explicit CellVLine(QWidget *parent = nullptr);
};

class CellHLine: public CellLine
{
    Q_OBJECT
public:
    explicit CellHLine(QWidget *parent = nullptr);
};

class ABuildingCell: public QVBoxLayout
{
    Q_OBJECT
protected:
    CellLineWithoutMouse *left_line;
    CellLineWithoutMouse *right_line;
    CellLineWithoutMouse *top_line;
    CellLineWithoutMouse *bottom_line;
    QHBoxLayout *central_layout;
    int type;
    int changeType();
public:
    explicit ABuildingCell(QGridLayout *layout, int row, int colown, int cell_type, QWidget *parent = nullptr);
    int setType(int cell_type);
    void setWalls(bool bottom_wall = false, bool right_wall = false, bool left_wall = false, bool top_wall = false);
    bool isLeftLineWall(){return left_line->isWall();}
    bool isRightLineWall(){return right_line->isWall();}
    bool isTopLineWall(){return top_line->isWall();}
    bool isBottomLineWall(){return bottom_line->isWall();}
    void repaintLines();
    ~ABuildingCell();
};

class BuildingCell: public ABuildingCell
{
    Q_OBJECT

public:
    explicit BuildingCell(QGridLayout *layout, int row, int colown, int cell_type, QWidget *parent = nullptr);
};

class DeviceIcon: public QLabel
{
    Q_OBJECT
signals:
    void mousePressed();
protected:
    void mousePressEvent(QMouseEvent *event) override;
public:
    explicit DeviceIcon(const QString &fileName, Qt::CursorShape cursor_shape = Qt::PointingHandCursor,  QWidget *parent = nullptr);
    explicit DeviceIcon(Qt::CursorShape cursor_shape = Qt::PointingHandCursor, QWidget *parent = nullptr);
    void setIcon(const QString &fileName);
    void setIcon(int device_type);
};

class BuildingCellDevices: public ABuildingCell
{
    Q_OBJECT
signals:
    void iconPressed();
private slots:
    void on_icon_pressed();
protected:
    int device_type;
    DeviceIcon *device_icon;
public:
    explicit BuildingCellDevices(QGridLayout *layout, int row, int colown, int cell_type, QWidget *parent = nullptr);
    void setDeviceType(int deviceType);
    int  getDeviceType(){return device_type;}
    ~BuildingCellDevices();
};

class ACellCable: public QFrame
{
    Q_OBJECT
public:
    explicit ACellCable(QWidget *parent = nullptr);
    explicit ACellCable(int r, int g, int b, QWidget *parent = nullptr);
    void changeColor(int r, int g, int b);
};

class CellHCable: public ACellCable
{
    Q_OBJECT
public:
    explicit CellHCable(QWidget *parent = nullptr);
    explicit CellHCable(int r, int g, int b, QWidget *parent = nullptr);
};

class CellVCable: public ACellCable
{
    Q_OBJECT
public:
    explicit CellVCable(QWidget *parent = nullptr);
    explicit CellVCable(int r, int g, int b, QWidget *parent = nullptr);
};

class BuildingCellPlanning: public ABuildingCell
{
    Q_OBJECT
protected:
    int device_type;
    QBoxLayout *inside_layout;
    DeviceIcon *device_icon;
    Cables cables;
    QBoxLayout *inside_vlayout;
public:
    explicit BuildingCellPlanning(QGridLayout *layout, int row, int colown, int cell_type, int dev_type = EMPTY, QWidget *parent = nullptr);
    void setDeviceType(int deviceType);
    int  getDeviceType(){return device_type;}
    void setCable(int cable_type, int r = 0, int g = 255, int b = 0);
    Cables getCables() {return cables;};
    void resetCables();
    ~BuildingCellPlanning();
};

class Wall
{
private:
    int begin;
    int end;
public:
    Wall(int beg, int ed):begin(beg), end(ed) { };
    Wall() = delete;
    int getLength() {return  abs(end - begin) + 1;}
    int getBegin() {return begin;}
    int getEnd()   {return end;}
    bool between(int coord);
};

#endif // BUILDINGCELL_H
