#include "gamemenu.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameMenu w;
    w.show();
//    PlayerVsComputeer play;
//    play.show();

    return a.exec();
}
