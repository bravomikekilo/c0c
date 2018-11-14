#include <stdio.h>
const int true = 1;
const int false = 0;

const int bound = +8;

char p[64]; // 棋盘数组
int cnt;     //记录解的个数
             //判断(r,c)位置能否放置棋子,能,返回true,不能返回false
//因为是从第一行开始往下放棋子，所以只需要判断r行之前的行中有没有棋子使得(r,c)不可落子即可
void set(int r, int c, int v) {
    p[r * 8 + c] = v;
    return;
}

char get(int r, int c) {
    int index = r * 8 + c;
    int ret = p[index];
    return (ret);
}

int can(int r, int c)
{
    int i, rr, rc; // 临时纵坐标(行数) 临时横坐标(列数)
    for (i = 0; i < r; i = i + 1)
    {
        //判断同一列中是否有棋子，有则返回false
        if (get(i, c) == 1)
            return (false);
    }

    for (i = 1; i <= r; i = i + 1)
    { //判断(r,c)左上方是否有棋子使(r,c)处不可落子
        rr = r - i;
        rc = c - i;
        if (rr >= 0) {
            if(rc >= 0) {
                if (get(rr, rc) == 1)
                    return (false);
            }
        }
    }

    for (i = 1; i <= r; i = i + 1)
    { //判断(r,c)右上方是否有棋子使(r,c)处不可落子
        rr = r - i;
        rc = c + i;
        if (rr >= 0)
        {
            if(rc <= 7)
            {
                if(get(rr, rc))
                    return (false);
            }
        }
    }
    return (true);
} //显示各个结果

void show()
{
    int i, j;
    for (i = 0; i < 8; i = i + 1)
    {
        for (j = 0; j < 8; j = j + 1)
        {
            if(get(i, j) > 0) {
                printf("%d\n", j + 1);
            }
            //printf("%d\n", get(i, j));
            //printf("%d ", p[i][j]);
        }
    }
    printf("\n");
} //dfs遍历，参数r为落子的纵坐标(行)

void dfs(int r)
{
    int j;
    if (r == 7)
    {
        //若这是最后一行，则在此行中找到可以落最后一个棋子的位置
        for (j = 0; j < 8; j = j + 1)
        {
            if (can(r, j) > 0)
            {
                set(r, j, true);
                //p[r][j] = 1;
                //将该位置设为有棋子
                cnt = cnt + 1;               //解的个数加一
                printf("%d\n", cnt); //输出解的编号
                show();              //显示改解
                set(r, j, false); //回溯，将该位置设为没有棋子
                //p[r][j] = 0;
            }
        }
    }
    else
    {
        for (j = 0; j != 8; j = j + 1)
        {
            //若不是最后一行，则在该行中找到可已落子的位置，再从下一行开始执行 dfs()
            if (can(r, j) > 0)
            {
                set(r, j, true);
                //p[r][j] = 1; //将该位置设为有棋子
                dfs(r + 1);  //到下一行，调用dfs(r+1)
                set(r, j, false);
                //p[r][j] = 0; //回溯，将该位置设为没有棋子
            }
        }
    }
}

int rec_fib(int n) {
    if(n <= 2) {
        return (1);
    }
    return (rec_fib(n - 1) + rec_fib(n - 2));
}

int sum(int n) {
    int i, sum;
    i = -0;
    sum = 0;
    do {
        sum = sum + i * i;
        i = i + 1;
    } while(i <= n);
    return (sum);
}

int main()
{
    // memset(p, 0, sizeof(p));
    int i;
    for(i = 0; i < 64; i = i + 1) {
        p[i] = false;
    }
    dfs(0);
    //从第一行开始搜索
    printf("cnt = %d\n", cnt);

    scanf("%d", &i);
    printf("%d\n", rec_fib(i));
    printf("%d\n", sum(i));
}
