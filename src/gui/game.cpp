#include "game.h"
#include "ui_game.h"
#include <QGraphicsRectItem>
#include <QGraphicsProxyWidget>
#include <QDebug>
#include <QDirIterator>

GameGUI::GameGUI(QWidget *parent, Game &_gameLogic) :
    QDialog(parent),
    ui(new Ui::Game),
    scene(new QGraphicsScene(this)),
    gameLogic(_gameLogic)
{
    playersPixmap.resize(4);
    ui->setupUi(this);
    _view = new CustomView(this);
    _view->setScene(scene);
    ui->gridLayout->addWidget(_view);
    setNames();
    loadStones();
    loadCardsImgs();
    drawScene();

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

void GameGUI::loadCardsImgs()
{
    QDirIterator it(":/cards/art", QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        _cardsImg.push_back(QPixmap(it.next()));
    }
}

void GameGUI::loadStones()
{
    loadPathImgs();
}
void GameGUI::redrawScene()
{
    scene->clear();
    xPos.clear();
    yPos.clear();
    drawScene();
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
    drawFreeStone();
    drawCard();
}

void GameGUI::drawFreeStone()
{
    int mid = (gameLogic.labyrinth.getSize()) / 2 + 1;
    Stone stone = gameLogic.labyrinth.getFreeStone();
    freeStone = scene->addPixmap(_pathImg[stoneToImgIndex(stone)]);
    // Bimap for maping real position and grid position
    QPointF pos = getCoords(gameLogic.labyrinth.getSize(),mid,true);
    int x = pos.x() + 100;
    xPos.insert(boost::bimap<int,int>::value_type(x,-1));
    yPos.insert(boost::bimap<int,int>::value_type(pos.y(),-1));
    // Move it to right position
    freeStone->setPos(x,pos.y());
    // Create button for rotation
    rotateButton = new QPushButton();
    //but->setGeometry(QRect(QPoint(x,pos.y()),QSize(50,30)));
    rotateButton->setGeometry(x,pos.y()+55,50,20);
    rotateButton->setText("Rotate");
    QObject::connect(rotateButton, SIGNAL(released()), this, SLOT(rotateClicked()));
    scene->addWidget(rotateButton);
}

void GameGUI::drawCard()
{
    int N = gameLogic.labyrinth.getSize();
    QPointF pos = getCoords(N,1,true);
    QGraphicsPixmapItem *pm = scene->addPixmap(_cardsImg[gameLogic.actualCard()]);
    pm->setPos(pos.x() + 90, pos.y());
    pm->setScale(0.8);
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

QPointF GameGUI::getCoords(int x, int y,bool right)
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

    return QPointF(a,b);
}



void GameGUI::clicked(QPointF pos)
{
    QPointF rpos = getCoords(pos.x(),pos.y(),false);
    if(rpos.x() == -1)
        qDebug() << "Free stone";
    else
        gameLogic.clickBoard(rpos.y(),rpos.x());
    qDebug() << rpos.x() << ":" << rpos.y();
    redrawScene();
}

void GameGUI::rotateClicked()
{
    gameLogic.labyrinth.rotateFreeStone();
    Stone stone = gameLogic.labyrinth.getFreeStone();
    freeStone->setPixmap(_pathImg[stoneToImgIndex(stone)]);
}

void CustomView::mousePressEvent(QMouseEvent *event)
{
    QList<QGraphicsItem*> _items = items(event->pos());
    QGraphicsItem *item;
    if(!_items.isEmpty())
    {
        item = _items.last();
        if(item->type() == 7)
            _game->clicked(item->pos());
    }
    else
        qDebug() << "miss";
    QGraphicsView::mousePressEvent(event);

}

