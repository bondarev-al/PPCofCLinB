#ifndef BUILDINGCELL_H
#define BUILDINGCELL_H

#include <QBoxLayout>
#include <QFrame>
#include <QSpacerItem>

const int CELL_TYPE_FULL       = 0;
const int CELL_TYPE_TOP        = 1;
const int CELL_TYPE_LEFT       = 2;
const int CELL_TYPE_WITHOUT    = 3;
const int CELL_LINE_WIDTH      = 3;

class CellLine: public QFrame
{
    Q_OBJECT
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
    ~BuildingCell();
};

#endif // BUILDINGCELL_H
