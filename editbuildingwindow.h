#ifndef EDITBUILDINGWINDOW_H
#define EDITBUILDINGWINDOW_H

#include <QWidget>
#include <vector>
#include <QPushButton>

const QSize FLOORS_BUT_SIZE = QSize(22, 22);

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
    std::vector<QPushButton *> floors_but_vec;
};

#endif // EDITBUILDINGWINDOW_H
