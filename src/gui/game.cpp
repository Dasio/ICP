#include "game.h"
#include "ui_game.h"
#include <QGraphicsRectItem>
#include <QDebug>
#include <QBitmap>

Game::Game(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Game),
    scene(new QGraphicsScene(this))
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
Game::~Game()
{
    delete ui;
    delete scene;
}

void Game::setNames()
{
    // Get from API and check player count
    ui->player1Name_label->setText("Abc");
    ui->player2Name_label->setText("Def");
    ui->player3Name_label->setText("Xzy");
    ui->player4Name_label->setText("Uvy");
}
void Game::loadPathImgs()
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


void Game::drawStones()
{
    std::vector<pos> stones;
    int N = 7;
    int size = 66;
    int r;
    if(N == 9)
        _view->scale(0.9,0.9);
    else if (N==11)
        _view->scale(0.75,0.75);
    QGraphicsPixmapItem *pm;
    for(int y=0;y<N;y++)
        for(int x=0;x<N;x++)
        {
            stones.push_back(pos(x,y));
        }
    for(auto stone: stones)
    {
        r = qrand() % _pathImg.size();
        pm = scene->addPixmap(_pathImg[r]);
        int x = stone.x * size;
        int y = stone.y * size;
        xPos.insert(boost::bimap<int,int>::value_type(x,stone.x));
        yPos.insert(boost::bimap<int,int>::value_type(y,stone.y));
        pm->setPos(x,y);
    }
}

void Game::spawnPlayer()
{
    QPixmap player(":/player/art/down_stand.png");
    player.setDevicePixelRatio(1.5);
    player1Pixmap = scene->addPixmap(player);
    QPointF pos = getCoords(3,2);
    player1Pixmap->setPos(pos.x() + 20,pos.y() + 10);
}

QPointF Game::getCoords(int x, int y)
{
    int a = xPos.right.at(x);
    int b = yPos.right.at(y);
    return QPointF(a,b);
}

void Game::movePlayer(int x, int y)
{
    QPointF pos = getCoords(x,y);
    player1Pixmap->setPos(pos.x() + 20,pos.y() + 10);
}

void Game::clicked(QPointF pos)
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

