#ifndef GAMEGUI_H
#define GAMEGUI_H

#include <QDialog>
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
    void clicked(QPointF pos);
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
     * @brief Draw stones to graphics view
     */
    void drawStones();

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

    void spawnPlayer();
    void movePlayer(int x, int y);
    const unsigned int playerHeight = 70;
    const unsigned int playerWidth = 45;
    const unsigned int playerSpace = 20;
    QPointF getCoords(int x, int y);
    std::vector<QPixmap> _playerSprite;
    Ui::Game *ui;
    QGraphicsScene *scene;
    CustomView* _view;
    std::vector<QPixmap> _pathImg;
    QGraphicsPixmapItem *player1Pixmap;
    Game &gameLogic;


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
