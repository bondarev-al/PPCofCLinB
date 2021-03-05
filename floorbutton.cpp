#include "floorbutton.h"

AFloorButton::AFloorButton(QVBoxLayout *layout, QWidget *parent):QPushButton(parent)
{
    layout->addWidget(this);
}

int AFloorButton::getFloorNumber()
{
    return floor_number;
}

AFloorButton::~AFloorButton()
{

}

FloorButton::FloorButton(QVBoxLayout *layout, QWidget *parent): AFloorButton(layout, parent)
{
    floor_number = number_of_floors;
    number_of_floors++;
    this->setText(QString::number(number_of_floors));
}

FloorButtonDevices::FloorButtonDevices(QVBoxLayout *layout, QWidget *parent): AFloorButton(layout, parent)
{
    floor_number = number_of_floors;
    number_of_floors++;
    this->setText(QString::number(number_of_floors));
}

FloorButtonPlanning::FloorButtonPlanning(QVBoxLayout *layout, QWidget *parent): AFloorButton(layout, parent)
{
    floor_number = number_of_floors;
    number_of_floors++;
    this->setText(QString::number(number_of_floors));
}

int FloorButton::number_of_floors = 0;
int FloorButtonDevices::number_of_floors = 0;
int FloorButtonPlanning::number_of_floors = 0;
