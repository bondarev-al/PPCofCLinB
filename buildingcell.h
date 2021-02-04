#ifndef BUILDINGCELL_H
#define BUILDINGCELL_H

#include <QBoxLayout>
#include <QFrame>
#include <QSpacerItem>

class BuildingCell: public QVBoxLayout
{
    Q_OBJECT
private:
    QFrame *left_line;
    QFrame *right_line;
    QFrame *top_line;
    QFrame *bottom_line;
    QHBoxLayout *central_layout;
    QSpacerItem *central_spacer;
public:
    explicit BuildingCell(QWidget *parent = nullptr);
    ~BuildingCell();
};

#endif // BUILDINGCELL_H
