/* Authors:
 * Dávid Mikuš      (xmikus15)
 * Peter Hostačný   (xhosta03)
 */

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
    ui->player3Name_label->setVisible(false);
    ui->player4Name_label->setVisible(false);
    // Create view
    _view = new CustomView(this);
    _view->setBackgroundBrush(QBrush(Qt::yellow, Qt::Dense6Pattern));
    // Attach it to scene
    _view->setScene(scene);
    // Add it to window
    ui->gridLayout->addWidget(_view);
    setNames();
    loadStones();
    loadCardsImgs();
    drawScene();
    _view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);

}
GameGUI::~GameGUI()
{
    delete ui;
    delete scene;
}

void GameGUI::setNames()
{
    ui->player1Name_label->setText(QString::fromStdString(gameLogic.getPlayerName(0)));
    ui->player2Name_label->setText(QString::fromStdString(gameLogic.getPlayerName(1)));
    int playersSize = gameLogic.getPlayersCount();
    switch(playersSize)
    {
        case 4:
            ui->player4Name_label->setVisible(true);
            ui->player4Name_label->setText(QString::fromStdString(gameLogic.getPlayerName(3)));
        case 3:
            ui->player3Name_label->setVisible(true);
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
void GameGUI::actualizeStatus()
{
    // Set who is on turn
    QString activeStyle("QLabel { border: 1px solid  #ff0000 ;");
    ui->player1Name_label->setStyleSheet("QLabel { color : green; }");
    ui->player2Name_label->setStyleSheet("QLabel { color : red; }");
    ui->player3Name_label->setStyleSheet("QLabel { color : blue; }");
    ui->player4Name_label->setStyleSheet("QLabel { color : yellow; }");
    switch(gameLogic.activePlayer())
    {
        case 0:
            ui->player1Name_label->setStyleSheet(activeStyle + "color:green;}" );
            break;
        case 1:
            ui->player2Name_label->setStyleSheet(activeStyle + "color:red;}" );
            break;
        case 2:
            ui->player3Name_label->setStyleSheet(activeStyle + "color:blue;}" );
            break;
        case 3:
            ui->player4Name_label->setStyleSheet(activeStyle + "color:yellow;}" );
            break;
    }
    QString maxscore = QString::number(gameLogic.getMaxScore());
    // Actualize score
    ui->player1Score->setText(QString::number(gameLogic.getPlayerScore(0)) + "/" + maxscore);
    ui->player2Score->setText(QString::number(gameLogic.getPlayerScore(1)) + "/" + maxscore);

    int playersSize = gameLogic.getPlayersCount();
    switch(playersSize)
    {
        case 4:
            ui->player4Score->setText(QString::number(gameLogic.getPlayerScore(3)) + "/" + maxscore);
        case 3:
            ui->player3Score->setText(QString::number(gameLogic.getPlayerScore(2)) + "/" + maxscore);
    };
    // Set action
    if(gameLogic.nextAction() == MOVE)
        ui->actionLabel->setText("Move");
    else
        ui->actionLabel->setText("Shift");

}

void GameGUI::drawScene()
{
    actualizeStatus();
    int N = gameLogic.labyrinth.getSize();
    int playersCount = gameLogic.getPlayersCount();
    // 64px stone, and 2px for space
    int size = 66;
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

            // Draw treasure
            if(stone.treasure)
            {
                pm = scene->addPixmap(_cardsImg[stone.treasure]);
                pm->setScale(0.35);
                pm->setPos(imgX+10,imgY+17);
            }
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
    QGraphicsPixmapItem *pm;
    if(stone.treasure)
    {
        pm = scene->addPixmap(_cardsImg[stone.treasure]);
        pm->setScale(0.35);
        pm->setPos(x+10,pos.y()+17);
    }
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
    int x = pos.x() + 90;
    xPos.insert(boost::bimap<int,int>::value_type(x,-2));
    yPos.insert(boost::bimap<int,int>::value_type(pos.y(),-2));
    pm->setPos(x, pos.y());
    pm->setScale(0.8);
}


int GameGUI::stoneToImgIndex(Stone &stone)
{
    switch(stone.type)
    {
        case I:
            switch(stone.rotation)
            {
                case 0:
                case 2:
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
    else if(rpos.x() == -2)
        qDebug() << "Card";
    else
    {
        if(gameLogic.clickBoard(rpos.y(),rpos.x()))
        {
            ui->infoLabel->setText("OK");
            std::string *winner = gameLogic.checkWinner();
            // End game
            if(winner != nullptr)
            {
                QMessageBox msgBox(this);
                msgBox.setText(QString::fromStdString(*winner) + " won the game!");
                msgBox.exec();
                close();
            }
        }
        else
            ui->infoLabel->setText("Failed to make action");

    }
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


void GameGUI::on_saveButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    QString fileName;
    if(dialog.exec())
        fileName = dialog.selectedFiles()[0];
    // If no file was chosen, nothing happens
    if(!fileName.isEmpty())
    {
        QMessageBox msgBox(this);
        if(gameLogic.saveGame(fileName.toStdString()))
            msgBox.setText("Successfully saved");
        else
            msgBox.setText("Failed to save");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox(this);
        msgBox.setText("Failed to save. You have to choose file name");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void GameGUI::on_undoButton_clicked()
{
    qDebug() << "undo";
}

void GameGUI::on_exitButton_clicked()
{
    close();
}
