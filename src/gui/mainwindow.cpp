/* Authors:
 * Dávid Mikuš      (xmikus15)
 * Peter Hostačný   (xhosta03)
 */

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
    int treasureCount = ui->treasureBox->currentText().toInt();
    if(gameLogic.initialize(mapSize,treasureCount))
    {
        hide();
        GameGUI gameGUI(this,gameLogic);
        gameGUI.exec();
        show();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Failed to init game.");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void MainWindow::on_loadButton_clicked()
{
    QFileDialog dialog(this);
    // User have to choose file
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("Save files (*.xml)");
    QString fileName;
    if(dialog.exec())
        fileName = dialog.selectedFiles()[0];
    // If no file was chosen, nothing happens
    if(!fileName.isEmpty())
    {
        Game gameLogic;
        if(gameLogic.loadGame(fileName.toStdString()))
        {
            hide();
            GameGUI gameGUI(this,gameLogic);
            gameGUI.exec();
            show();
        }
        // Failed to load game
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Failed to load game.");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
        }

    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Failed to load. You have to choose file");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void MainWindow::on_exitButton_clicked()
{
    close();
}

void MainWindow::on_sizeBox_currentIndexChanged(int index)
{
    // If size is 5, change treasure count to 12
    if(index == 0)
        ui->treasureBox->setCurrentIndex(0);
}

void MainWindow::on_treasureBox_currentIndexChanged(int index)
{
    // If was treasure set to 24 and size was 5, set it to 7
    if(index == 1 && ui->sizeBox->currentIndex() == 0)
        ui->sizeBox->setCurrentIndex(1);
}
