#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->player1Enable->setEnabled(false);
    ui->player2Enable->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_createGameButton_clicked()
{
    QMessageBox msg;
    msg.setText("createGameButton");
    msg.exec();
}
