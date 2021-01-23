#ifndef EDITBUILDINGWINDOW_H
#define EDITBUILDINGWINDOW_H

#include <QWidget>

namespace Ui {
class EditBuildingWindow;
}

class EditBuildingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EditBuildingWindow(QWidget *parent = nullptr);
    ~EditBuildingWindow();

signals:
    void hidden();

private slots:


    void on_menu_but_clicked(bool checked);

private:
    Ui::EditBuildingWindow *ui;
};

#endif // EDITBUILDINGWINDOW_H
