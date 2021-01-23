#include "editbuildingwindow.h"
#include "ui_editbuildingwindow.h"

EditBuildingWindow::EditBuildingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditBuildingWindow)
{
    ui->setupUi(this);
    floors_but_vec.push_back(new QPushButton("1", ui->floors_frame));
    floors_but_vec[0]->setFixedSize(FLOORS_BUT_SIZE);
    ui->floors_but_layout->addWidget(floors_but_vec[0]);
}

EditBuildingWindow::~EditBuildingWindow()
{
    delete ui;   
}

void EditBuildingWindow::on_menu_but_clicked(bool checked)
{
    hide();
    emit hidden();
}
