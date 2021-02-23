#include "floorbutton.h"

FloorButton::FloorButton(QVBoxLayout *layout, QWidget *parent):QPushButton(parent)
{
    floor_number = number_of_floors;
    number_of_floors++;
    this->setText(QString::number(number_of_floors));
    //this->setFixedSize(FLOORS_BUT_SIZE);
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
