#ifndef PLANNINGWINDOW_H
#define PLANNINGWINDOW_H

#include <QWidget>

namespace Ui {
class PlanningWindow;
}

class PlanningWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PlanningWindow(QWidget *parent = nullptr);
    ~PlanningWindow();

signals:
    void hidden();

private slots:


    void on_menu_but_clicked(bool checked);

private:
    Ui::PlanningWindow *ui;
};

#endif // PLANNINGWINDOW_H
