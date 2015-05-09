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

    ui->setupUi(this);
    CustomView *view = new CustomView(this);
    view->setScene(scene);
    ui->gridLayout->addWidget(view);
    setNames();
    loadPathImgs();
    drawStones();
    spawnPlayer();
    /*QPixmap player(":/player/art/down_stand.png");
    QBitmap mask(player.createMaskFromColor(QColor(255,255,255), Qt::MaskOutColor));

        QPainter paint(&player);
        paint.setPen(QColor(0,0,0));
        paint.drawPixmap(player.rect(), mask, mask.rect());
        paint.end();
    QGraphicsPixmapItem *pm = scene->addPixmap(player);
    pm->setPos(60,-70);*/
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


void GameGUI::drawStones()
{
    int N = gameLogic.labyrinth.getSize();
    int size = 66;
    if(N == 9)
        _view->scale(0.9,0.9);
    else if (N==11)
        _view->scale(0.75,0.75);
    QGraphicsPixmapItem *pm;
    Stone stone;
    int imgX;
    int imgY;
    for(int y=1;y<=N;y++)
        for(int x=1;x<=N;x++)
        {
            stone = gameLogic.labyrinth.get(x,y);
            qDebug() << stone.type << " " << stone.rotation;
            pm = scene->addPixmap(_pathImg[stoneToImgIndex(stone)]);
            imgX = x * size;
            imgY = y * size;
            xPos.insert(boost::bimap<int,int>::value_type(imgX,x));
            yPos.insert(boost::bimap<int,int>::value_type(imgY,y));
            pm->setPos(imgX,imgY);
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

void GameGUI::spawnPlayer()
{
    QPixmap player(":/player/art/down_stand.png");
    player.setDevicePixelRatio(1.5);
    player1Pixmap = scene->addPixmap(player);
    QPointF pos = getCoords(3,2);
    player1Pixmap->setPos(pos.x() + 20,pos.y() + 10);
}

QPointF GameGUI::getCoords(int x, int y)
{
    int a = xPos.right.at(x);
    int b = yPos.right.at(y);
    return QPointF(a,b);
}

void GameGUI::movePlayer(int x, int y)
{
    QPointF pos = getCoords(x,y);
    player1Pixmap->setPos(pos.x() + 20,pos.y() + 10);
}

void GameGUI::clicked(QPointF pos)
{
    player1Pixmap->setPos(pos.x() + 20,pos.y() + 10);
}


void CustomView::mousePressEvent(QMouseEvent *event)
{
    QList<QGraphicsItem*> _items = items(event->pos());
    QGraphicsItem *item;
    if(!_items.isEmpty())
    {
        item = _items.last();
        //qDebug() << _game->xPos.left.at(item->pos().x()) << ":" << _game->xPos.left.at(item->pos().y());
        _game->clicked(item->pos());

    }
    else
        qDebug() << "miss";
}

