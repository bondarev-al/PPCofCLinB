#ifndef BUILDINGCELL_H
#define BUILDINGCELL_H

#include <QBoxLayout>
#include <QFrame>
#include <QSpacerItem>

const int CELL_TYPE_FULL       = 0;
const int CELL_TYPE_TOP        = 1;
const int CELL_TYPE_LEFT       = 2;
const int CELL_TYPE_WITHOUT    = 3;
const int CELL_LINE_WIDTH      = 1;
const QString PAINTED_LINE_STYLE = "background-color: rgb(255, 0, 0);";

struct Walls
{
    bool bottom_wall;
    bool right_wall;
    bool left_wall;
    bool top_wall;
};

class CellLine: public QFrame
{
    Q_OBJECT
protected:
    bool wall;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
public:
    explicit CellLine(QWidget *parent = nullptr);
    bool isWall(){ return wall; }
    bool setWall(bool w);
    void repaintLine();
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

class BuildingCell: public QVBoxLayout
{
    Q_OBJECT
private:
    CellVLine *left_line;
    CellVLine *right_line;
    CellHLine *top_line;
    CellHLine *bottom_line;
    QHBoxLayout *central_layout;
    int type;
    int changeType();
public:
    explicit BuildingCell(QGridLayout *layout, int row, int colown, int cell_type, QWidget *parent = nullptr);
    int setType(int cell_type);
    void setWalls(bool bottom_wall = false, bool right_wall = false, bool left_wall = false, bool top_wall = false);
    bool isLeftLineWall(){return left_line->isWall();}
    bool isRightLineWall(){return right_line->isWall();}
    bool isTopLineWall(){return top_line->isWall();}
    bool isBottomLineWall(){return bottom_line->isWall();}
    void repaintLines();
    ~BuildingCell();
};

#endif // BUILDINGCELL_H
