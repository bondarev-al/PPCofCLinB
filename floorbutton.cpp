#include "floorbutton.h"

FloorButton::FloorButton(QVBoxLayout *layout, QWidget *parent):QPushButton(parent)
{
    number_of_floors++;
    floor_number = number_of_floors;
    this->setText(QString::number(floor_number));
    this->setFixedSize(FLOORS_BUT_SIZE);
    layout->addWidget(this);
}

int FloorButton::getFloorNumber()
{
    return floor_number;
}

FloorButton::~FloorButton()
{

}

int FloorButton::number_of_floors = 0;
