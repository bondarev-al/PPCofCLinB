#include "buildingcell.h"

CellLine::CellLine(QWidget *parent):QFrame(parent)
{
    setFrameShadow(QFrame::Plain);
    setLineWidth(CELL_LINE_WIDTH);
    setCursor(Qt::PointingHandCursor);
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

//    right_line->setStyleSheet("background-color: rgb(255, 0, 0);");

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

BuildingCell::~BuildingCell()
{

}
