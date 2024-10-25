#ifndef PLAYERVSCOMPUTEER_H
#define PLAYERVSCOMPUTEER_H

#include "chessboard.h"
#include <QMouseEvent>
#include <QMessageBox>


class PlayerVsComputeer:public ChessBoard
{
public:
    PlayerVsComputeer();

    //玩家
    void mouseReleaseEvent(QMouseEvent *e);//用click函数重载
    void click(QPoint p);//点击下子
    void changeSide();//换子
    void judge(int r,int c);//裁判

    //电脑
    void computerSetChess();//电脑下子

    //不同种类型的两种AI
    void AIClimb(int &r,int &c);//电脑遍历棋盘的空位，并选择价值最大的位置，本质是爬山搜索
    void AICompete(int& r,int& c);//对抗搜索，主要作用是预判对方最强的level步棋，然后占领位置，思路和minmax一样

    //实际估值操作
    int posValue(int r,int c);//r行c列位置的价值计算函数
    int chessType(int d,int r,int c);//通过周围遍历来确定棋型,d为方向,一次检查一条线
    void addStep(int d,int* r,int* c);//从当前位置按d方向走一格
    //d为1到8，也为从正下方逆时针转一周

    //悔棋
    void backChess();

};

#endif // PLAYERVSCOMPUTEER_H
