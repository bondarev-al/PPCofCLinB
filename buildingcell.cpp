#include "buildingcell.h"

BuildingCell::BuildingCell(int cell_type, QWidget *parent):QVBoxLayout(parent), type(cell_type)
{
    central_layout = new QHBoxLayout;
    left_line      = new QFrame;
    right_line     = new QFrame;
    top_line       = new QFrame;
    bottom_line    = new QFrame;

    left_line->setFrameShape(QFrame::VLine);
    left_line->setFrameShadow(QFrame::Plain);
    right_line->setFrameShape(QFrame::VLine);
    right_line->setFrameShadow(QFrame::Plain);
    top_line->setFrameShape(QFrame::HLine);
    top_line->setFrameShadow(QFrame::Plain);
    bottom_line->setFrameShape(QFrame::HLine);
    bottom_line->setFrameShadow(QFrame::Plain);

    changeType();

//    right_line->setStyleSheet("background-color: rgb(255, 0, 0);");

    central_layout->addWidget(left_line);
    central_layout->addStretch();
    central_layout->addWidget(right_line);
    this->addWidget(top_line);
    this->addLayout(central_layout);
    this->addWidget(bottom_line);

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
