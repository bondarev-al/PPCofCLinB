#ifndef BUILDINGCELL_H
#define BUILDINGCELL_H

#include <QBoxLayout>
#include <QFrame>
#include <QSpacerItem>

const int CELL_TYPE_FULL       = 0;
const int CELL_TYPE_TOP        = 1;
const int CELL_TYPE_LEFT       = 2;
const int CELL_TYPE_WITHOUT    = 3;

class BuildingCell: public QVBoxLayout
{
    Q_OBJECT
private:
    QFrame *left_line;
    QFrame *right_line;
    QFrame *top_line;
    QFrame *bottom_line;
    QHBoxLayout *central_layout;
    int type;
    int changeType();
public:
    explicit BuildingCell(int cell_type, QWidget *parent = nullptr);
    int setType(int cell_type);
    ~BuildingCell();
};

#endif // BUILDINGCELL_H
