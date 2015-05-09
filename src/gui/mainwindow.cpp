#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsRectItem>
#include "../src/game/Game.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->player1Enable->setEnabled(false);
    ui->player2Enable->setEnabled(false);
    ui->sizeBox->setCurrentIndex(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_createGameButton_clicked()
{
    Game gameLogic;
    gameLogic.addPlayer(ui->player1Name->text().toStdString());
    gameLogic.addPlayer(ui->player2Name->text().toStdString());
    if(ui->player3Enable->isChecked())
        gameLogic.addPlayer(ui->player3Name->text().toStdString());
    if(ui->player4Enable->isChecked())
        gameLogic.addPlayer(ui->player4Name->text().toStdString());
    int mapSize = ui->sizeBox->currentText().toInt();
    qDebug() << mapSize;
    gameLogic.initialize(mapSize);
    hide();
    GameGUI gameGUI(this,gameLogic);
    gameGUI.exec();
    close();
}
