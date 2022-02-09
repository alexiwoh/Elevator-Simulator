
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ECS.h"
#include "View.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model =  new ECS(4,8,-2);
    view = new View(this,model);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete view;
}



