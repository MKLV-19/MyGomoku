#include "playervscomputeer.h"

PlayerVsComputeer::PlayerVsComputeer()
{
    this->setWindowTitle("人机对战");
    setMouseTracking(true);//设置鼠标跟随
    back.setParent(this);
    back.setText("悔棋");
    back.move(chessradius*15*2,chessradius*7*2);
    restart.setParent(this);
    restart.setText("重来");
    restart.move(chessradius*15*2,chessradius*8*2);

    connect(&restart,&QPushButton::pressed,this,&ChessBoard::clearChess);
    connect(&back,&QPushButton::pressed,this,&PlayerVsComputeer::backChess);
}

void PlayerVsComputeer::mouseReleaseEvent(QMouseEvent *e)
{
    click(e->pos());
}

void PlayerVsComputeer::click(QPoint p)//点一下就是一回合
{
    int row=pointToRow(p);
    int col=pointToCol(p);
    if(pointIsInRange(p)&&board[row][col]==SPACE&&p.x()<=chessradius*2*15){
        setChess(row,col);//下棋
        judge(row,col);//判断输赢
        changeSide();//每下一次就要换子
        computerSetChess();//电脑出手
        changeSide();//换回本方
        count++;//回合加一
    }
}

void PlayerVsComputeer::changeSide()
{
    if(currentside==BLACK){
        currentside=WHITE;
    }else{
        currentside=BLACK;
    }
    update();//每换一次子就要更新
}

void PlayerVsComputeer::judge(int r,int c)
{
    if(oneSideWin(r,c)){//检查哪方获胜
        if(currentside==BLACK){//玩家执黑子
            QMessageBox m(QMessageBox::NoIcon,"游戏结束","您赢了！");
            m.exec();
        }else if(currentside==WHITE){//电脑执白子
            QMessageBox m(QMessageBox::NoIcon,"游戏结束","您输了！");
            m.exec();
        }
        clearChess();//清空棋盘
        return ;
    }
}

void PlayerVsComputeer::computerSetChess()
{
    int row,col;
    AIClimb(row,col);//采用AI改变row和col使得其最优
    setChess(row,col);//电脑下棋
    judge(row,col);//判断输赢
}

void PlayerVsComputeer::AIClimb(int &r, int &c)
{
    //注：坐标的评估方法是从-1格开始的
    int value=0,maxvalue=0;//当前价值和当前最大价值
    int bestrow=0,bestcol=0;//最优坐标

    for(int i=0;i<15;i++){//逐个评估
        for(int j=0;j<15;j++){
            if(board[i+1][j+1]==SPACE){
                value=posValue(i,j);
                if(value>=maxvalue){
                    bestrow=i;
                    bestcol=j;
                    maxvalue=value;
                }
            }
        }
    }
    r=bestrow+1;
    c=bestcol+1;
}

void PlayerVsComputeer::AICompete(int &r, int &c)
{
    int value=0,maxvalue=0;//当前价值和当前最大价值
    int bestrow=0,bestcol=0;//最优坐标
    changeSide();//转换身份
    for(int i=0;i<15;i++){//逐个评估
        for(int j=0;j<15;j++){
            if(board[i+1][j+1]==SPACE){
                value=posValue(i,j);
                if(value>=maxvalue){
                    bestrow=i;
                    bestcol=j;
                    maxvalue=value;
                }
            }
        }
    }
    r=bestrow+1;
    c=bestcol+1;
    changeSide();
}

int PlayerVsComputeer::posValue(int r, int c)
{
    int direction=1;//检查方向
    int finalvalue=0;//最终估价
    int code1,code2;//分别是较小和较大的棋型编号
    int protocode1,protocode2;//编号记忆
    int x1,x2,y1,y2,z1,z2;//解码后的信息
    //棋型采用整数编码，用坐标解码
    //x：0电脑，1玩家 y：0活 1冲 2空活 3空冲  z：0~3代表1~4个连续的子
    //解释，空活：活的空一格的位置，空冲：冲的空一格的位置
    //第一行是电脑，第二行是玩家
    int value[2][4][4]{{{50,300,5000,20000},{5,10,500,20000},{20,80,150,0},{5,10,400,0}}
                       ,{{30,200,3000,0},{1,10,200,10000},{25,100,0,0},{5,20,30,0}}};

    while(direction!=5){
        code1=chessType(direction,r,c);
        direction+=4;//方向转180度
        code2=chessType(direction,r,c);
        direction-=3;//转回0度后换到下一格
        if(code1>code2){//编码1总是较小，这样能减少两边活和冲的讨论
            int t=code1;
            code1=code2;
            code2=t;
        }
        protocode1=code1;
        protocode2=code2;//记录
        z1=code1%10;code1/=10;y1=code1%10;code1/=10;x1=code1;
        z2=code2%10;code2/=10;y2=code2%10;code2/=10;x2=code2;//解码
        //特殊类棋型判断
        if(protocode1==-1){//一边为空
            if(protocode2<0){//另一边也是边界棋型
                continue;
            }else{
                finalvalue+=value[x2][y2][z2];
                continue;
            }
        }
        if(protocode1==-2){//边界冲
            if(protocode2<0){
                continue;
            }else{
                finalvalue+=value[x2][y2][z2]/2;
                continue;
            }
        }
        if(protocode1==-3){//边界空冲
            if(protocode2<0){
                continue;
            }else{
                finalvalue+=value[x2][y2][z2]/4;
                continue;
            }
        }
        //两边棋型的一般判断  电脑：活——a，冲——b 玩家：活——c，冲——d
        //aa ab cc cd
        if(((protocode1>-1&&protocode1<4)&&(protocode2>-1&&protocode2<4))
                ||((protocode1>-1&&protocode1<4)&&(protocode2>9&&protocode2<14))
                ||((protocode1>99&&protocode1<104)&&(protocode2>99&&protocode2<104))
                ||((protocode1>99&&protocode1<104)&&(protocode2>109&&protocode2<114))){
            if(z1+z2>=2){
                finalvalue+=value[x2][y2][3];//四子以上
                continue;
            }else{
                finalvalue+=value[x2][y2][z1+z2+1];//z1+z2+2个子的处理办法
                continue;
            }
        }
        //bb dd
        if(((protocode1>9&&protocode1<14)&&(protocode2>9&&protocode2<14))
                ||((protocode1>109&&protocode1<114)&&(protocode2>109&&protocode2<114))){
            if(z1+z2>=2){
                finalvalue+=20000;
                continue;
            }else{
                continue;//不可能绝杀，不需要去管
            }
        }
        //ac ad bc bd
        if(((protocode1>-1&&protocode1<4)&&(protocode2>99&&protocode2<104))
                ||((protocode1>-1&&protocode1<4)&&(protocode2>109&&protocode2<114))
                ||((protocode1>9&&protocode1<14)&&(protocode2>99&&protocode2<104))
                ||((protocode1>9&&protocode1<14)&&(protocode2>109&&protocode2<114))){
            if(z1==3||z2==3){//绝杀
                finalvalue+=20000;
                continue;
            }else{
                finalvalue+=value[x1][y1][z1]/2+value[x2][y2][z2];//更注重堵住对方，可调
                continue;
            }
        }else{//其它未知棋型
            finalvalue+=value[x1][y1][z1]+value[x2][y2][z2];
            continue;
        }
    }
    return finalvalue;
}

int PlayerVsComputeer::chessType(int d, int r, int c)
{
    int code=0;//编码
    int count=0;//记录连子个数
    //注：空：-1 边界冲：-2 边界空冲：-3
    addStep(d,&r,&c);
    if(r<0||r>14||c<0||c>14){//边界冲
        code=-2;
    }
    switch(board[r+1][c+1]){//检查连子情况
    case CHESS_COMPUTER:{
        count++;
        addStep(d,&r,&c);
        if(r<0||r>14||c<0||c>14){//如果遇到边界
            code=count+9;//冲
            return code;
        }
        while(board[r+1][c+1]==CHESS_COMPUTER){//电脑连子
            count++;
            addStep(d,&r,&c);
            if(r<0||r>14||c<0||c>14){//如果遇到边界
                code=count+9;//冲
                return code;
            }
        }
        if(board[r+1][c+1]==SPACE){
            code=count-1;//电脑空
        }else{
            code=count+9;//电脑冲
        }
    }break;

    case CHESS_PLAYER:{
        count++;
        addStep(d,&r,&c);
        if(r<0||r>14||c<0||c>14){//如果遇到边界
            code=count+109;//冲
            return code;
        }
        while(board[r+1][c+1]==CHESS_PLAYER){//玩家连子
            count++;
            addStep(d,&r,&c);
            if(r<0||r>14||c<0||c>14){//如果遇到边界
                code=count+109;//冲
                return code;
            }
        }
        if(board[r+1][c+1]==SPACE){
            code=count+99;//玩家空
        }else{
            code=count+109;//玩家冲
        }
    }break;

    case SPACE:{
        addStep(d,&r,&c);
        if(r<0||r>14||c<0||c>14){//如果遇到边界
            code=-3;//墙壁空冲
            return code;
        }

        switch(board[r+1][c+1]){
        case CHESS_COMPUTER:{//电脑方空
            count++;
            addStep(d,&r,&c);
            if(r<0||r>14||c<0||c>14){//如果遇到边界
                code=+29;//电脑墙壁空冲
                return code;
            }
            while(board[r+1][c+1]==CHESS_COMPUTER){//电脑连子
                count++;
                addStep(d,&r,&c);
                if(r<0||r>14||c<0||c>14){//如果遇到边界
                    code=count+29;//冲
                    return code;
                }
            }
            if(board[r+1][c+1]==SPACE){
                code=count+19;//电脑空活
            }else{
                code=count+29;
            }
        }break;

        case CHESS_PLAYER:{//玩家方空
            count++;
            addStep(d,&r,&c);
            if(r<0||r>14||c<0||c>14){//如果遇到边界
                code=+129;//玩家墙壁空冲
                return code;
            }
            while(board[r+1][c+1]==CHESS_COMPUTER){//玩家连子
                count++;
                addStep(d,&r,&c);
                if(r<0||r>14||c<0||c>14){//如果遇到边界
                    code=count+129;//冲
                    return code;
                }
            }
            if(board[r+1][c+1]==SPACE){
                code=count+119;//玩家空活
            }else{
                code=count+129;
            }
        }break;

        case SPACE:{//空
            code=-1;
        }break;
        }

    }break;

    }
    return code;
}

void PlayerVsComputeer::addStep(int d, int *r, int *c)
{
    switch(d){
    case 1:*r+=1;break;
    case 2:*r+=1;*c+=1;break;
    case 3:*c+=1;break;
    case 4:*r-=1;*c+=1;break;
    case 5:*r-=1;break;
    case 6:*r-=1;*c-=1;break;
    case 7:*c-=1;break;
    case 8:*r+=1;*c-=1;break;
    }
}

void PlayerVsComputeer::backChess()
{
    if(!blackrow.empty()&&!whiterow.empty()){
        int br=0,bc=0,wr=0,wc=0;
        br=blackrow.back();
        bc=blackcol.back();
        wr=whiterow.back();
        wc=whitecol.back();
        blackrow.pop_back();
        blackcol.pop_back();
        whiterow.pop_back();
        whitecol.pop_back();
        board[br][bc]=SPACE;
        board[wr][wc]=SPACE;
        update();
        return ;
    }else{
        return ;
    }

}
