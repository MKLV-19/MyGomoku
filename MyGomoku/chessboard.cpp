#include "chessboard.h"

ChessBoard::ChessBoard()
{
    clearChess();//开始时清空棋盘
    chessradius=CHESS_RADIUS;//设置棋子大小
    setMinimumSize(chessradius*15*2+100,chessradius*15*2);
    setMaximumSize(chessradius*15*2+100,chessradius*15*2);//设置界面大小
    firstside=BLACK;
    currentside=BLACK;//设置先手方和当前方棋子
    update();//刷新一次
}

void ChessBoard::drawBackground()
{
    QPalette p=this->palette();
    p.setColor(QPalette::Window, QColor(180,180,180));
    this->setPalette(p);//背景颜色
}

void ChessBoard::drawLine(QPainter& p)
{
    for(int i=1;i<=15;i++){//横线
        if(i==1||i==15){//边缘加粗
            p.setPen(QPen(Qt::black,3,Qt::SolidLine));//颜色，粗细，线型
        }else{//细线
            p.setPen(QPen(Qt::black,1,Qt::SolidLine));
        }
        p.drawLine(getPointCenter(i,1),getPointCenter(i,15));
        //从第一行画到第十五行
    }

    for(int i=1;i<=15;i++){//竖线
        if(i==1||i==15){//边缘加粗
            p.setPen(QPen(Qt::black,3,Qt::SolidLine));//颜色，粗细，线型
        }else{//细线
            p.setPen(QPen(Qt::black,1,Qt::SolidLine));
        }
        p.drawLine(getPointCenter(1,i),getPointCenter(15,i));
        //从第一列画到第十五列
    }
}

void ChessBoard::drawPoint(QPainter &p)
{
    //设置颜色
    QColor color;
    color=Qt::black;
    p.setBrush(Qt::black);
    p.setPen(QPen(QBrush(color),2));//黑色，填充样式2

    QPoint lefttop;
    for(int i=1;i<=15;i++){
        for(int j=1;j<=15;j++){
            if((i==4||i==8||i==12)&&(j==4||j==8||j==12)){
                lefttop=getPointCenter(i,j)-QPoint(chessradius/6.0,chessradius/6.0);
                QRect rec(lefttop.x(),lefttop.y(),chessradius/3.0,chessradius/3.0);
                p.drawEllipse(rec);

            }
        }
    }
}

void ChessBoard::drawChess(QPainter& p)
{
    QColor color;
    for(int i=1;i<=15;i++){
        for(int j=1;j<=15;j++){
            if(board[i][j]==WHITE){//白棋
                color=Qt::white;
                p.setBrush(Qt::white);
                p.setPen(QPen(QBrush(color),2));//白色，填充样式2
                p.drawEllipse(chessRect(i,j));
            }
            if(board[i][j]==BLACK){//黑棋
                color=Qt::black;
                p.setBrush(Qt::black);
                p.setPen(QPen(QBrush(color),2));//黑色，填充样式2
                p.drawEllipse(chessRect(i,j));
            }
        }
    }
}

void ChessBoard::paintEvent(QPaintEvent *)
{
    int r=height()/(15*2);//设置半径
    chessradius=r;
    basepoint=QPoint(chessradius+1,chessradius+1);//原点设置
    //上面这几步是在继承的时候也能保持绘制准确

    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);//抗锯齿
    drawBackground();//背景色
    drawLine(p);//画线
    drawPoint(p);//画点
    drawChess(p);//画棋子

}

void ChessBoard::setChess(int r, int c)
{
    if(r<1||r>15||c<1||c>15){//界外不能落子
        return ;
    }
    if(board[r][c]!=SPACE){//已有棋子时不能落子
        return ;
    }
    board[r][c]=currentside;

    if(currentside==BLACK){//记录
        blackrow.push_back(r);
        blackcol.push_back(c);
    }
    if(currentside==WHITE){
        whiterow.push_back(r);
        whitecol.push_back(c);
    }

}

bool ChessBoard::oneSideWin(int r, int c)
{
    int dx[8]={0,1,1,1,0,-1,-1,-1};
    int dy[8]={1,1,0,-1,-1,-1,0,1};//dx，dy分别对应从(r,c)正下方开始逆时针的一周
    int side=board[r][c];//当前位置的颜色
    int count=0;
    int row=r,col=c;
    int direction=0;
    for(int i=0;i<4;i++){//四条线分别判定，count记录连子
        count=0;
        direction=i;//当前方向
        row=r;
        col=c;//归位
        while(board[row][col]==side){//正方向
            count++;
            row+=dy[direction];
            col+=dx[direction];
        }
        direction+=4;
        row=r;
        col=c;
        while(board[row][col]==side){//反方向
            count++;
            row+=dy[direction];
            col+=dx[direction];
        }

        if(count>5){//这里要求大于5的原因是当前位置被记录了两次，所以至少是6
            return true;
        }
    }
    return false;
}

void ChessBoard::clearChess()
{
    for(int i=1;i<=15;i++){
        for(int j=1;j<=15;j++){
            board[i][j]=SPACE;
        }
    }
    count=0;//回合数清零
    while(!blackrow.empty()){
        blackrow.pop_back();
    }
    while(!blackcol.empty()){
        blackcol.pop_back();
    }
    while(!whiterow.empty()){
        whiterow.pop_back();
    }
    while(!whitecol.empty()){
        whitecol.pop_back();
    }
    update();
}

QPoint ChessBoard::getPointCenter(int r, int c)
{
    QPoint deltapoint(chessradius*(c-1)*2,chessradius*(r-1)*2);
    //相对于原点的增量
    return basepoint+deltapoint;
}

QPoint ChessBoard::getDrawStartPoint(int r, int c)
{
    return getPointCenter(r,c)-QPoint(chessradius,chessradius);
}

QRect ChessBoard::chessRect(int r, int c)
{
    return QRect(getDrawStartPoint(r,c),QSize(chessradius*2-1,chessradius*2-1));
    //-1是为了防止边缘完全重合
}

bool ChessBoard::pointIsInRange(QPoint p)
{
    int r=p.y()/(2*chessradius)+1;
    int c=p.x()/(2*chessradius)+1;//以原点为参考确定具体行列
    QPoint lt=getPointCenter(r,c);//顶点

    int distance=(p.x()-lt.x())*(p.x()-lt.x())+(p.y()-lt.y())*(p.y()-lt.y());
    if(distance<chessradius*chessradius){//该点在范围之内
        return true;
    }
    return false;
}

int ChessBoard::pointToRow(QPoint p)
{
    return p.y()/(2*chessradius)+1;
}

int ChessBoard::pointToCol(QPoint p)
{
    return p.x()/(2*chessradius)+1;
}

//基本完善
