/* Authors:
 * Dávid Mikuš      (xmikus15)
 * Peter Hostačný   (xhosta03)
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_createGameButton_clicked();

    void on_loadButton_clicked();

    void on_exitButton_clicked();

    void on_sizeBox_currentIndexChanged(int index);

    void on_treasureBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
