#include "game.h"
#include "ui_game.h"
#include <QGraphicsRectItem>
#include <QDebug>
#include <QBitmap>

GameGUI::GameGUI(QWidget *parent, Game &_gameLogic) :
    QDialog(parent),
    ui(new Ui::Game),
    scene(new QGraphicsScene(this)),
    gameLogic(_gameLogic)
{
    playersPixmap.resize(4);
    ui->setupUi(this);
    CustomView *view = new CustomView(this);
    view->setScene(scene);
    ui->gridLayout->addWidget(view);
    setNames();
    loadStones();
    drawScene();
    //createButtons();
   // spawnPlayer(2,2,3);
}
GameGUI::~GameGUI()
{
    delete ui;
    delete scene;
}

void GameGUI::setNames()
{
    // Get from API and check player count
    ui->player1Name_label->setText(QString::fromStdString(gameLogic.getPlayerName(0)));
    ui->player2Name_label->setText(QString::fromStdString(gameLogic.getPlayerName(1)));
    int playersSize = gameLogic.getPlayersCount();
    switch(playersSize)
    {
        case 4:
            ui->player4_label->setEnabled(true);
            ui->player4Name_label->setText(QString::fromStdString(gameLogic.getPlayerName(3)));
        case 3:
            ui->player3_label->setEnabled(true);
            ui->player3Name_label->setText(QString::fromStdString(gameLogic.getPlayerName(2)));

    };
}
void GameGUI::loadPathImgs()
{
    _pathImg.push_back(QPixmap(":/path/art/vertical.png"));
    _pathImg.push_back(QPixmap(":/path/art/horizontal.png"));
    _pathImg.push_back(QPixmap(":/path/art/two1.png"));
    _pathImg.push_back(QPixmap(":/path/art/two2.png"));
    _pathImg.push_back(QPixmap(":/path/art/two3.png"));
    _pathImg.push_back(QPixmap(":/path/art/two4.png"));
    _pathImg.push_back(QPixmap(":/path/art/three1.png"));
    _pathImg.push_back(QPixmap(":/path/art/three2.png"));
    _pathImg.push_back(QPixmap(":/path/art/three3.png"));
    _pathImg.push_back(QPixmap(":/path/art/three4.png"));
}

void GameGUI::loadStones()
{
    loadPathImgs();
}

void GameGUI::drawScene()
{
    int N = gameLogic.labyrinth.getSize();
    int playersCount = gameLogic.getPlayersCount();
    int size = 66;
    if(N == 9)
        _view->scale(0.9,0.9);
    else if (N==11)
        _view->scale(0.75,0.75);
    QGraphicsPixmapItem *pm;
    Stone stone;
    int imgX;
    int imgY;
    int playersOnStone;
    for(int y=1;y<=N;y++)
        for(int x=1;x<=N;x++)
        {
            // Get stone from board
            stone = gameLogic.labyrinth.get(y,x);
            //qDebug() << x << ":" << y << " = " << stone.type << ":" << stone.rotation;
            pm = scene->addPixmap(_pathImg[stoneToImgIndex(stone)]);
            imgX = x * size;
            imgY = y * size;
            // Bimap for maping real position and grid position
            xPos.insert(boost::bimap<int,int>::value_type(imgX,x));
            yPos.insert(boost::bimap<int,int>::value_type(imgY,y));
            // Move it to right position
            pm->setPos(imgX,imgY);
            // Spawn players
            playersOnStone = 0;
            // First check how many they are there
            for(int i=0;i<playersCount;i++)
                if(stone.player_slots[i] != nullptr)
                    playersOnStone++;
            int begin,size;
            switch(playersOnStone)
            {
                case 4:
                    begin=-2;
                    size = 13;
                    break;
                case 3:
                    begin=5;
                    size = 15;
                    break;
                case 2:
                    begin = 8;
                    size = 22;
                    break;
                default:
                    begin = 20;
                    size = 0;
            }
            // Draw them
            for(int i=0,j=0;i<playersCount;i++)
                if(stone.player_slots[i] != nullptr)
                {
                    spawnPlayer(i+1,x,y,begin+j*size);
                    j++;
                }
        }
}
void GameGUI::createButtons()
{
    int N = gameLogic.labyrinth.getSize();
    int i =0;
    for(int x=2; x<=N; x+=2)
    {
        //QPoint coords = getCoords(x,0,false);
        _buttons.push_back(new QPushButton);
        _buttons[i]->setGeometry(QRect(QPoint(0,0),QSize(20,20)));
        _buttons[i]->setText("v");
        scene->addWidget(_buttons[i]);
        i++;
    }
}

int GameGUI::stoneToImgIndex(Stone &stone)
{
    switch(stone.type)
    {
        case I:
            switch(stone.rotation)
            {
                case 0:
                    return 0;
                default:
                    return 1;
            }
        case L:
            switch(stone.rotation)
            {
                case 0:
                    return 5;
                case 1:
                    return 2;
                case 2:
                    return 3;
                case 3:
                    return 4;

            }
        case T:
            switch(stone.rotation)
            {
                case 0:
                    return 6;
                case 1:
                    return 7;
                case 2:
                    return 8;
                case 3:
                    return 9;
            }
    }
    return 0;
}

void GameGUI::spawnPlayer(int id, int x, int y, int off)
{
    QPixmap player;
    switch(id)
    {
        case 1:
            player.load(":/player/art/player1.png");
            break;
        case 2:
            player.load(":/player/art/player2.png");
            break;
        case 3:
            player.load(":/player/art/player3.png");
            break;
        case 4:
            player.load(":/player/art/player4.png");
            break;
        default:
            player.load(":/player/art/player1.png");

    }

    player.setDevicePixelRatio(1.5);
    playersPixmap[id-1] = scene->addPixmap(player);
    QPointF pos = getCoords(x,y,true);
    playersPixmap[id-1]->setPos(pos.x() + off,pos.y() + 10);
}

QPoint GameGUI::getCoords(int x, int y,bool right)
{
    int a,b;
    if(right)
    {
        a = xPos.right.at(x);
        b = yPos.right.at(y);
    }
    else
    {
        a = xPos.left.at(x);
        b = yPos.left.at(y);
    }

    return QPoint(a,b);
}

void GameGUI::movePlayer(int id,int x, int y)
{
    QPoint pos = getCoords(x,y,true);
    playersPixmap[id]->setPos(pos.x() + 20,pos.y() + 10);
}

void GameGUI::clicked(QPoint pos)
{
    //player1Pixmap->setPos(pos.x() + 20,pos.y() + 10);
    QPoint newpos = getCoords(pos.x(),pos.y(),false);
    movePlayer(0,newpos.x(),newpos.y());
}


void CustomView::mousePressEvent(QMouseEvent *event)
{
    QList<QGraphicsItem*> _items = items(event->pos());
    QGraphicsItem *item;
    if(!_items.isEmpty())
    {
        item = _items.last();
        //qDebug() << _game->xPos.left.at(item->pos().x()) << ":" << _game->xPos.left.at(item->pos().y());
        _game->clicked(item->pos().toPoint());

    }
    else
        qDebug() << "miss";
}

