#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lobby.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    bool ok;
    int port = ui->portNumberEdit->text().toInt(&ok,10);
    if(!ok)
        ui->infoLabel->setText("Wrong port");
    else
    {
        hide();
        Lobby lobby(this);
        lobby.exec();
        show();

    }
}
