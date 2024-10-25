#include "gamemenu.h"
#include "ui_gamemenu.h"

GameMenu::GameMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameMenu)
{
    ui->setupUi(this);
}

GameMenu::~GameMenu()
{
    delete ui;
}

void GameMenu::on_pushButton_clicked()
{
    PlayerVsComputeer* pvc=new PlayerVsComputeer();
    pvc->show();
}

//只有人机对战
