#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lobby.h"
#include <QMessageBox>
#include <thread>
#include "../src/client/Client.h"

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
        try
        {
            Client client(ui->hostNameEdit->text().toStdString(),port);
            hide();
            Lobby lobby(this);
            lobby.exec();
            client.stop();
            show();
        }
        catch(MyExc &err)
        {
            ui->infoLabel->setText("Failed to connect");
        }
    }
}
