#include "buildingcelldevices.h"

CellLineDevices::CellLineDevices(QWidget *parent):QFrame(parent)
{
    setFrameShadow(QFrame::Raised);
    setLineWidth(CELL_LINE_WIDTH);
    wall = false;
}

void CellLineDevices::repaintLine()
{
    setStyleSheet("");
    if ( wall ) setStyleSheet(PAINTED_LINE_STYLE);
}


bool CellLineDevices::setWall(bool w)
{
    wall = w;
    return wall;
}

CellVLineDevices::CellVLineDevices(QWidget *parent):CellLineDevices(parent)
{
    setFrameShape(QFrame::VLine);
}

CellHLineDevices::CellHLineDevices(QWidget *parent):CellLineDevices(parent)
{
    setFrameShape(QFrame::HLine);
}

BuildingCellDevices::BuildingCellDevices(QGridLayout *layout, int row, int colown, int cell_type, QWidget *parent):QVBoxLayout(parent), type(cell_type)
{
    layout->addLayout(this, row, colown);

    central_layout = new QHBoxLayout;
    left_line      = new CellVLineDevices;
    right_line     = new CellVLineDevices;
    top_line       = new CellHLineDevices;
    bottom_line    = new CellHLineDevices;

    central_layout->addWidget(left_line);
    central_layout->addStretch();
    central_layout->addWidget(right_line);
    this->addWidget(top_line);
    this->addLayout(central_layout);
    this->addWidget(bottom_line);

    changeType();
}


int BuildingCellDevices::setType(int cell_type)
{
    type = cell_type;
    changeType();
    return cell_type;
}

int BuildingCellDevices::changeType()
{
    switch (type)
    {
        case CELL_TYPE_FULL:
            left_line->show();
            top_line->show();
            break;
        case CELL_TYPE_TOP:
            left_line->hide();
            top_line->show();
            break;
        case CELL_TYPE_LEFT:
            left_line->show();
            top_line->hide();
            break;
        case CELL_TYPE_WITHOUT:
            left_line->hide();
            top_line->hide();
            break;
    }
    return type;
}

void BuildingCellDevices::setWalls(bool bottom_wall, bool right_wall, bool left_wall, bool top_wall)
{
    bottom_line->setWall(bottom_wall);
    right_line->setWall(right_wall);
    left_line->setWall(left_wall);
    top_line->setWall(top_wall);
}

void BuildingCellDevices::repaintLines()
{
    bottom_line->repaintLine();
    right_line->repaintLine();
    left_line->repaintLine();
    top_line->repaintLine();
}

BuildingCellDevices::~BuildingCellDevices()
{
    delete central_layout;
    delete top_line;
    delete left_line;
    delete right_line;
    delete bottom_line;
}
