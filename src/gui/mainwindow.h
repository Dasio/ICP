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
    /**
     * @brief Try to init game
     */
    void on_createGameButton_clicked();

    /**
     * @brief Try to load game
     */
    void on_loadButton_clicked();

    /**
     * @brief Exit game
     */
    void on_exitButton_clicked();

    /**
     * @brief Check allowed combination
     * @param index
     */
    void on_sizeBox_currentIndexChanged(int index);

    /**
     * @brief Check allowed combination
     * @param index
     */
    void on_treasureBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
