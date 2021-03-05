#ifndef FLOORBUTTON_H
#define FLOORBUTTON_H

#include <QPushButton>
#include <QVBoxLayout>

const QSize FLOORS_BUT_SIZE = QSize(35, 35);

class AFloorButton: public QPushButton
{
    Q_OBJECT
protected:
    int floor_number;
public:
//    static int number_of_floors;
    int getFloorNumber();
    explicit AFloorButton(QVBoxLayout *layout, QWidget *parent = nullptr);
    ~AFloorButton();
};

class FloorButton: public AFloorButton
{
    Q_OBJECT
public:
    static int number_of_floors;
    explicit FloorButton(QVBoxLayout *layout, QWidget *parent = nullptr);
};

class FloorButtonDevices: public AFloorButton
{
    Q_OBJECT
public:
    static int number_of_floors;
    explicit FloorButtonDevices(QVBoxLayout *layout, QWidget *parent = nullptr);
};

class FloorButtonPlanning: public AFloorButton
{
    Q_OBJECT
public:
    static int number_of_floors;
    explicit FloorButtonPlanning(QVBoxLayout *layout, QWidget *parent = nullptr);
};

#endif // FLOORBUTTON_H
