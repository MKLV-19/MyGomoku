#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QFrame>
#include <QPainter>
#include <QMouseEvent>
#include <QPalette>
#include <QDebug>
#include <QList>
#include <QPushButton>
#include <signal.h>

const int CHESS_RADIUS=20;//棋子半径
const int SPACE=0;//空位置为0
const int BLACK=1;//黑方为1
const int WHITE=2;//白方为2
const int CHESS_PLAYER=1;//玩家方棋子记为1
const int CHESS_COMPUTER=2;//电脑方棋子记为2

/*用来下棋的棋盘类，基类是QFrame，本身也是一个界面类
存储了一个棋盘的基本信息
棋盘采取16*16二维数组，其中1~15为棋盘线
0表示空位置，1为黑棋（玩家，先手），2为白棋（电脑）
棋盘的大小，线间隔，边缘留白，均可以由棋子的半径进行构造
*/
class ChessBoard:public QFrame
{
public:
    ChessBoard();//在构造函数中初始化棋盘和棋子
    //注：以下函数按游戏流程进行排列
    //绘制类
    void drawBackground();//设置棋盘背景颜色
    void drawLine(QPainter& p);//绘制棋盘线
    void drawPoint(QPainter& p);//绘制棋盘上的九个点
    void drawChess(QPainter& p);//绘制棋盘上所有棋子
    void paintEvent(QPaintEvent *);//绘制整个游戏画面，每次落子后会进行一次update()

    //游戏类
    void setChess(int r,int c);//放置当前方棋子
    bool oneSideWin(int r,int c);//对当前落子位置进行检查，若连成五个棋子则结束游戏
    void clearChess();//游戏结束后清空棋盘上所有棋子

    //位置类
    QPoint getPointCenter(int r,int c);//根据原点和棋盘的r行c列确定交点坐标
    QPoint getDrawStartPoint(int r,int c);//根据原点和棋盘的行列确定绘制该位置棋子的左上角的点
    QRect chessRect(int r,int c);//得到放置一个棋子的矩形
    //使用矩形的原因：Qt画圆会出现坐标偏移，必须用矩形控制住

    //操作类
    bool pointIsInRange(QPoint p);//使用一个模糊范围，便于玩家点击落子
    int pointToRow(QPoint p);//根据原点将一个模糊的点转换为确定的行数
    int pointToCol(QPoint p);//根据原点将一个模糊的点转换为确定的列数

    //棋盘的基本变量
    int board[16][16];//棋盘数组，行列代表线条交点
    int chessradius;
    QPoint basepoint;//棋盘上左上角第一个落子点的坐标，棋盘原点
    int firstside;//先手方
    int currentside;//当前方

    //悔棋功能
    QPushButton back;//悔棋按钮
    int count=0;//记录第几手棋
    QList<int> blackrow;
    QList<int> blackcol;
    QList<int> whiterow;
    QList<int> whitecol;//记录具体步数

    //重开
    QPushButton restart;

};

#endif // CHESSBOARD_H
