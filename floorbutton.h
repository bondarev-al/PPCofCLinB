#ifndef FLOORBUTTON_H
#define FLOORBUTTON_H

#include <QPushButton>
#include <QVBoxLayout>

const QSize FLOORS_BUT_SIZE = QSize(35, 35);

class FloorButton: public QPushButton
{
    Q_OBJECT
private:
    int floor_number;
public:
    static int number_of_floors;
    int getFloorNumber();
    explicit FloorButton(QVBoxLayout *layout, QWidget *parent = nullptr);
    ~FloorButton();
};



#endif // FLOORBUTTON_H
