#include "buildingcell.h"

CellLine::CellLine(QWidget *parent):QFrame(parent)
{
    setFrameShadow(QFrame::Raised);
    setLineWidth(CELL_LINE_WIDTH);
    setCursor(Qt::PointingHandCursor);
    wall = false;
}

void CellLine::enterEvent(QEvent *event)
{
    setStyleSheet(PAINTED_LINE_STYLE);
}

void CellLine::leaveEvent(QEvent *event)
{
    if ( ! wall ) setStyleSheet("");
}

void CellLine::repaintLine()
{
    setStyleSheet("");
    if ( wall ) setStyleSheet(PAINTED_LINE_STYLE);
}

void CellLine::mousePressEvent(QMouseEvent *event)
{
    wall = ! wall;
}

bool CellLine::setWall(bool w)
{
    wall = w;
    return wall;
}

CellVLine::CellVLine(QWidget *parent):CellLine(parent)
{
    setFrameShape(QFrame::VLine);
}

CellHLine::CellHLine(QWidget *parent):CellLine(parent)
{
    setFrameShape(QFrame::HLine);
}

BuildingCell::BuildingCell(QGridLayout *layout, int row, int colown, int cell_type, QWidget *parent):QVBoxLayout(parent), type(cell_type)
{
    layout->addLayout(this, row, colown);

    central_layout = new QHBoxLayout;
    left_line      = new CellVLine;
    right_line     = new CellVLine;
    top_line       = new CellHLine;
    bottom_line    = new CellHLine;

    central_layout->addWidget(left_line);
    central_layout->addStretch();
    central_layout->addWidget(right_line);
    this->addWidget(top_line);
    this->addLayout(central_layout);
    this->addWidget(bottom_line);

    changeType();
}


int BuildingCell::setType(int cell_type)
{
    type = cell_type;
    changeType();
    return cell_type;
}

int BuildingCell::changeType()
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

void BuildingCell::setWalls(bool bottom_wall, bool right_wall, bool left_wall, bool top_wall)
{
    bottom_line->setWall(bottom_wall);
    right_line->setWall(right_wall);
    left_line->setWall(left_wall);
    top_line->setWall(top_wall);
}

void BuildingCell::repaintLines()
{
    bottom_line->repaintLine();
    right_line->repaintLine();
    left_line->repaintLine();
    top_line->repaintLine();
}

BuildingCell::~BuildingCell()
{
    delete central_layout;
    delete top_line;
    delete left_line;
    delete right_line;
    delete bottom_line;
}
