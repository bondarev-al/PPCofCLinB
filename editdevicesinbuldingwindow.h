#ifndef EDITDEVICESINBULDINGWINDOW_H
#define EDITDEVICESINBULDINGWINDOW_H

#include <QWidget>

namespace Ui {
class EditDevicesInBuldingWindow;
}

class EditDevicesInBuldingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EditDevicesInBuldingWindow(QWidget *parent = nullptr);
    ~EditDevicesInBuldingWindow();

signals:
    void hidden();

private slots:


    void on_menu_but_clicked(bool checked);

private:
    Ui::EditDevicesInBuldingWindow *ui;
};

#endif // EDITDEVICESINBULDINGWINDOW_H
