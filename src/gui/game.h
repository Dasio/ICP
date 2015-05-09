#ifndef GAMEGUI_H
#define GAMEGUI_H

#include <QDialog>
#include <QPushButton>
#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QMouseEvent>
#include <QDebug>
#include <boost/bimap.hpp>
#include "../src/game/Game.h"

namespace Ui {
class Game;
}
class CustomView;


class GameGUI : public QDialog
{
    Q_OBJECT

public:
    explicit GameGUI(QWidget *parent, Game &_gameLogic);
    ~GameGUI();
    void clicked(QPoint pos);
    boost::bimap<int, int> xPos;
    boost::bimap<int, int> yPos;

private:
    struct pos{
        pos() = default;
        pos(int _x,int _y) : x(_x),y(_y) { }
        int x,y;
    };
    /**
     * @brief Update labels with player names
     */
    void setNames();

    /**
     * @brief Draw scene to graphics view
     */
    void drawScene();

    /**
     * @brief Load images for path in vector
     */
    void loadPathImgs();

    /**
     * @brief stoneToImgIndex
     * @param stone
     * @return correspoindg index to path img vector
     */
    int stoneToImgIndex(Stone &stone);

    /**
     * @brief createButtons
     */
    void createButtons();
    void loadStones();
    /**
     * @brief spawnPlayer
     * @param id
     * @param x
     * @param y
     * @param x-offset
     */
    void spawnPlayer(int id, int x, int y, int off = 20);
    void movePlayer(int id, int x, int y);
    const unsigned int playerHeight = 70;
    const unsigned int playerWidth = 45;
    const unsigned int playerSpace = 20;
    QPoint getCoords(int x, int y,bool right);
    std::vector<QPixmap> _playerSprite;
    Ui::Game *ui;
    QGraphicsScene *scene;
    CustomView* _view;
    std::vector<QPixmap> _pathImg;
    std::vector<QGraphicsPixmapItem*> playersPixmap;
    Game &gameLogic;
    std::vector<QPushButton*> _buttons;


};

class CustomView : public QGraphicsView
{
    Q_OBJECT
public:
    CustomView(GameGUI *game) : QGraphicsView(), _game(game)
    {
        setMouseTracking(true);
    }
private:
    GameGUI* _game;
    void mousePressEvent(QMouseEvent *event);
    bool _hover;

};
#endif // GAMEGUI_H
