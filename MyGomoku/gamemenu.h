#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <QWidget>
#include "playervscomputeer.h"
#include "chessboard.h"

//游戏的主菜单
namespace Ui {
class GameMenu;
}

class GameMenu : public QWidget
{
    Q_OBJECT

public:
    explicit GameMenu(QWidget *parent = nullptr);
    ~GameMenu();

private slots:
    void on_pushButton_clicked();

private:
    Ui::GameMenu *ui;
};

#endif // GAMEMENU_H
