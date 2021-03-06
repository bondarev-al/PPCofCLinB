#include "buildingcell.h"

CellLineWithoutMouse::CellLineWithoutMouse(QWidget *parent):QFrame(parent)
{
    setFrameShadow(QFrame::Raised);
    setLineWidth(CELL_LINE_WIDTH);
    wall = false;
}

void CellLineWithoutMouse::repaintLine()
{
    setStyleSheet("");
    if ( wall ) setStyleSheet(PAINTED_LINE_STYLE);
}


bool CellLineWithoutMouse::setWall(bool w)
{
    wall = w;
    return wall;
}

CellVLineWithoutMouse::CellVLineWithoutMouse(QWidget *parent):CellLineWithoutMouse(parent)
{
    setFrameShape(QFrame::VLine);
}

CellHLineWithoutMouse::CellHLineWithoutMouse(QWidget *parent):CellLineWithoutMouse(parent)
{
    setFrameShape(QFrame::HLine);
}

CellLine::CellLine(QWidget *parent):CellLineWithoutMouse(parent)
{
    setCursor(Qt::PointingHandCursor);
}

void CellLine::enterEvent(QEvent *event)
{
    setStyleSheet(PAINTED_LINE_STYLE);
}

void CellLine::leaveEvent(QEvent *event)
{
    if ( ! wall ) setStyleSheet("");
}

void CellLine::mousePressEvent(QMouseEvent *event)
{
    wall = ! wall;
}

CellVLine::CellVLine(QWidget *parent):CellLine(parent)
{
    setFrameShape(QFrame::VLine);
}

CellHLine::CellHLine(QWidget *parent):CellLine(parent)
{
    setFrameShape(QFrame::HLine);
}

ABuildingCell::ABuildingCell(QGridLayout *layout, int row, int colown, int cell_type, QWidget *parent):QVBoxLayout(parent), type(cell_type)
{
    layout->addLayout(this, row, colown);

    central_layout = new QHBoxLayout;
}


int ABuildingCell::setType(int cell_type)
{
    type = cell_type;
    changeType();
    return cell_type;
}

int ABuildingCell::changeType()
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

void ABuildingCell::setWalls(bool bottom_wall, bool right_wall, bool left_wall, bool top_wall)
{
    bottom_line->setWall(bottom_wall);
    right_line->setWall(right_wall);
    left_line->setWall(left_wall);
    top_line->setWall(top_wall);
}

void ABuildingCell::repaintLines()
{
    bottom_line->repaintLine();
    right_line->repaintLine();
    left_line->repaintLine();
    top_line->repaintLine();
}

ABuildingCell::~ABuildingCell()
{
    delete central_layout;
    delete top_line;
    delete left_line;
    delete right_line;
    delete bottom_line;
}

BuildingCell::BuildingCell(QGridLayout *layout, int row, int colown, int cell_type, QWidget *parent):
    ABuildingCell(layout, row, colown, cell_type, parent)
{
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

BuildingCellDevices::BuildingCellDevices(QGridLayout *layout, int row, int colown, int cell_type, QWidget *parent):
    ABuildingCell(layout, row, colown, cell_type, parent)
{
    left_line      = new CellVLineWithoutMouse;
    right_line     = new CellVLineWithoutMouse;
    top_line       = new CellHLineWithoutMouse;
    bottom_line    = new CellHLineWithoutMouse;
    device_icon    = new DeviceIcon();
    device_type    = EMPTY;

    central_layout->addWidget(left_line);
    central_layout->addStretch();
    central_layout->addWidget(device_icon);
    central_layout->addStretch();
    central_layout->addWidget(right_line);
    this->addWidget(top_line);
    this->addLayout(central_layout);
    this->addWidget(bottom_line);

    connect(device_icon, SIGNAL(mousePressed()), this, SLOT(on_icon_pressed()));
    changeType();
}

void BuildingCellDevices::on_icon_pressed()
{
    emit iconPressed();
}

void BuildingCellDevices::setDeviceType(int deviceType)
{
    device_type = deviceType;
    device_icon->setIcon(deviceType);
}

BuildingCellDevices::~BuildingCellDevices()
{
    delete device_icon;
}

DeviceIcon::DeviceIcon(Qt::CursorShape cursor_shape, QWidget *parent):QLabel(parent)
{
    setCursor(cursor_shape);
    setIcon(EMPTY_ICON);
}

DeviceIcon::DeviceIcon(const QString &fileName, Qt::CursorShape cursor_shape, QWidget *parent):QLabel(parent)
{
    setCursor(cursor_shape);
    setIcon(fileName);
}

void DeviceIcon::setIcon(const QString &fileName)
{
    QPixmap icon(fileName);
    setPixmap(icon.scaledToHeight(ICON_HEIGHT));
}

void DeviceIcon::setIcon(int device_type)
{
    switch (device_type)
    {
        case EMPTY:
            setIcon(EMPTY_ICON);
            break;
        case PC:
            setIcon(PC_ICON);
            break;
        case SWITCH:
            setIcon(SWITCH_ICON);
            break;
    }
}

void DeviceIcon::mousePressEvent(QMouseEvent *event)
{
    emit mousePressed();
}

ACellCable::ACellCable(QWidget *parent):QFrame(parent)
{
    setFrameShadow(QFrame::Raised);
    setLineWidth(CELL_LINE_WIDTH);
    setStyleSheet("background-color: rgb(0, 255, 0);");
}

ACellCable::ACellCable(int r, int g, int b, QWidget *parent):QFrame(parent)
{
    setFrameShadow(QFrame::Raised);
    setLineWidth(CELL_LINE_WIDTH);
    changeColor(r, g, b);
}

void ACellCable::changeColor(int r, int g, int b)
{
    QString string = "background-color: rgb(";
    string += QString::number(r);
    string += ", ";
    string += QString::number(g);
    string += ", ";
    string += QString::number(b);
    string += ");";
    setStyleSheet(string);
}

CellHCable::CellHCable(QWidget *parent):ACellCable(parent)
{
    setFrameShape(QFrame::HLine);
}

CellHCable::CellHCable(int r, int g, int b, QWidget *parent):ACellCable(r, g, b, parent)
{
    setFrameShape(QFrame::HLine);
}

CellVCable::CellVCable(QWidget *parent):ACellCable(parent)
{
    setFrameShape(QFrame::VLine);
}

CellVCable::CellVCable(int r, int g, int b, QWidget *parent):ACellCable(r, g, b, parent)
{
    setFrameShape(QFrame::VLine);
}

BuildingCellPlanning::BuildingCellPlanning(QGridLayout *layout, int row, int colown, int cell_type, int dev_type, QWidget *parent):
    ABuildingCell(layout, row, colown, cell_type, parent)
{
    left_line      = new CellVLineWithoutMouse;
    right_line     = new CellVLineWithoutMouse;
    top_line       = new CellHLineWithoutMouse;
    bottom_line    = new CellHLineWithoutMouse;
    device_type    = dev_type;

    cables.bottom_cable = false;
    cables.top_cable    = false;
    cables.left_cable   = false;
    cables.right_cable  = false;

    central_layout->addWidget(left_line);
    inside_layout = new QHBoxLayout;
    inside_vlayout = new QVBoxLayout;
    central_layout->addLayout(inside_vlayout);
    inside_vlayout->addStretch();
    inside_vlayout->addLayout(inside_layout);
    inside_vlayout->addStretch();
    inside_layout->addStretch();

    if (device_type != EMPTY)
    {
        device_icon = new DeviceIcon(Qt::ArrowCursor);
        device_icon->setIcon(device_type);
        inside_layout->addWidget(device_icon);
        inside_layout->addStretch();
    }

    central_layout->addWidget(right_line);
    this->addWidget(top_line);
    this->addLayout(central_layout);
    this->addWidget(bottom_line);

    changeType();
}

void BuildingCellPlanning::setDeviceType(int deviceType)
{
    device_type = deviceType;
    while (QLayoutItem* item = inside_layout->takeAt(0))
    {
        delete item->widget();
        delete item;
    }
    if (device_type != EMPTY)
    {
        device_icon = new DeviceIcon(Qt::ArrowCursor);
        device_icon->setIcon(device_type);
        inside_layout->addStretch();
        inside_layout->addWidget(device_icon);
        inside_layout->addStretch();
    }
    else inside_layout->addStretch();
}

void BuildingCellPlanning::setCable(int cable_type, int r, int g, int b)
{
    switch (cable_type)
    {
        case CABLE_TOP:
            if (!cables.top_cable)
            {
                cables.top_cable = true;
                inside_vlayout->insertWidget(0, new CellHCable(r, g, b));
                cables.top_color.r = r;
                cables.top_color.g = g;
                cables.top_color.b = b;
            }
            break;
        case CABLE_LEFT:
            if (!cables.left_cable)
            {
                cables.left_cable = true;
                central_layout->insertWidget(1, new CellVCable(r, g, b));
                cables.left_color.r = r;
                cables.left_color.g = g;
                cables.left_color.b = b;
            }
            break;
        case CABLE_BOTTOM:
            if (!cables.bottom_cable)
            {
                cables.bottom_cable = true;
                cables.bottom_color.r = r;
                cables.bottom_color.g = g;
                cables.bottom_color.b = b;
                if (cables.top_cable)
                    inside_vlayout->insertWidget(4, new CellHCable(r, g, b));
                else inside_vlayout->insertWidget(3, new CellHCable(r, g, b));
            }
            break;
        case CABLE_RIGHT:
            if (!cables.right_cable)
            {
                cables.right_cable = true;
                cables.right_color.r = r;
                cables.right_color.g = g;
                cables.right_color.b = b;
                if (cables.left_cable)
                    central_layout->insertWidget(3, new CellVCable(r, g, b));
                else central_layout->insertWidget(2, new CellVCable(r, g, b));
            }
            break;
    }
}

void BuildingCellPlanning::resetCables()
{
    QLayoutItem* item;
    if (cables.top_cable)
    {
        item = inside_vlayout->takeAt(0);
        delete item->widget();
        delete item;
        cables.top_cable = false;
    }
    if (cables.left_cable)
    {
        item = central_layout->takeAt(1);
        delete item->widget();
        delete item;
        cables.left_cable = false;
    }
    if (cables.bottom_cable)
    {
        item = inside_vlayout->takeAt(3);
        delete item->widget();
        delete item;
        cables.bottom_cable = false;
    }
    if (cables.right_cable)
    {
        item = central_layout->takeAt(2);
        delete item->widget();
        delete item;
        cables.right_cable = false;
    }
}

BuildingCellPlanning::~BuildingCellPlanning()
{
    if (device_type != EMPTY) delete device_icon;
}

bool Wall::between(int coord)
{
    if ((coord >= begin) && (coord <= end)) return true;
    return false;
}
