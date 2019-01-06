#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
void output(int[100][100],int,int);
int wander(int[100][100],int[100][100],int,int,int,int*,int*,int*);
void explode(int[100][100],int[100][100],int,int,int,int);
void fallDown(int[100][100],int,int);
void fillGaps(int[100][100],int,int);
void exchange(int[100][100],int,int);
int main()
{
    system("COLOR");
    srand(time(NULL));
    int score=0,n,m,p_sameControl=0,p_empty=0;
    int matris[100][100]; // ana matris

    printf("**************************************************\n");
    printf("\t\tNumber Crash\n\nPlease give the row and column number of the playground: ");
    scanf("%d %d",&n,&m);

    int identicals[100][100],p_itr=0;
    for (int i=0;i<n;i++)
    {
        for (int j=0;j<m;j++)
        {
            matris[i][j]=rand()%10; // ana matrisi randomla
            identicals[i][j]=-1;    // eş matrisi -1 lerle dolduruyor
        }
    }
    output(matris,n,m);

    int exit=0;
    while (exit==0) // doğru olana kadar oyun devam etsin
    {
        score=wander(matris,identicals,n,m,score,&p_itr,&p_sameControl,&p_empty);   // matrisi dolaşsın
        if ((p_sameControl+p_empty)==1) // Bosluk var
        {
            printf("\n\033[1;0mPress any key to continue...\n");
            getch();
            fillGaps(matris,n,m);
            wander(matris,identicals,n,m,score,&p_itr,&p_sameControl,&p_empty);
        }
        else if ((p_sameControl+p_empty)==0) // Ana matriste Bosluk yok, es matriste ayni yok
        {
            char ans;
            printf("\n\033[1;0m>Do you want to continue? (Y/N)\n");
            scanf(" %c", &ans);
            if (ans=='Y'||ans=='y') {
                exchange(matris,n,m);
                wander(matris,identicals,n,m,score,&p_itr,&p_sameControl,&p_empty);
            }
            else if (ans=='N'||ans=='n')
            {
                printf(">Score: %d\n",score*score);
                exit=1;
            }
        }
        else // Ayni var / Bosluk var
        {
            printf("\033[1;0m\nPress any key to continue...\n");
            getch();
            explode(identicals,matris,n,m,score,p_itr);
            wander(matris,identicals,n,m,score,&p_itr,&p_sameControl,&p_empty);
        }
    }

    return 0;
}
void output(int matris[100][100],int n,int m)
{
    printf("\n");
    for (int i=0;i<n;i++)
    {
        for (int j=0;j<m;j++)
        {
            if (matris[i][j]==' ')
                printf(" %c ",matris[i][j]);
            else
                printf(" %d ",matris[i][j]);
        }
        printf("\n");
    }
}
int wander(int matris[100][100],int identicals[100][100],int n,int m,int score,int *p_itr,int *p_sameControl,int *p_empty)
{
    for (int i=0;i<n;i++)
        for (int j=0;j<m;j++)
            identicals[i][j]=-1;

    for (int i=0;i<n;i++)
    {
        for (int j=0;j<m;j++)
        {
            if (matris[i][j]!=' ')
            {
                if (i<n-1 && j<m-1)
                {
                    if (matris[i][j]==matris[i][j+1])
                    {
                        identicals[i][j]=matris[i][j];
                        identicals[i][j+1]=matris[i][j+1];
                    }
                    if (matris[i][j]==matris[i+1][j])
                    {
                        identicals[i][j]=matris[i][j];
                        identicals[i+1][j]=matris[i+1][j];
                    }
                }
                else if (i==n-1 && j<m-1)
                {
                    if (matris[i][j]==matris[i][j+1])
                    {
                        identicals[i][j]=matris[i][j];
                        identicals[i][j+1]=matris[i][j+1];
                    }
                }
                else if (i<n-1 && j==m-1)
                {
                    if (matris[i][j]==matris[i+1][j])
                    {
                        identicals[i][j]=matris[i][j];
                        identicals[i+1][j]=matris[i+1][j];
                    }
                }
            }
        }
    }
    *p_itr=0;*p_sameControl=0;*p_empty=0;
    for (int i=0;i<n;i++)
    {
        for (int j=0;j<m;j++)
        {
            if (identicals[i][j]>=0)
            {
                (*p_itr)++;     // eş matrisde -1 olmayan elemanları sayıyor, patlatma fonk. için
                score++;        // eş matrisde -1 olmayan elemanları sayıyor, son puan için
                (*p_sameControl)=-10;       // main`de while döngüsünün son kontrolüne (else) gitmesi için
            }

            if (matris[i][j]==' ')  (*p_empty)=1;   //  boşluk varsa main`de fillGaps`a girsin
        }
    }
    return score;
}
void explode(int identicals[100][100],int matris[100][100],int n,int m,int score,int p_itr)
{
    if (p_itr==0) // if true, no values left to explode
    {
        output(matris,n,m);
        for (int i=0;i<n;i++) // To check if there's no need to do fallDown
        {
            for (int j=0;j<m;j++)
            {
                if (matris[i][j]!=' '&&matris[i+1][j]==' ')
                {
                    printf("\nPress any key to continue...\n");
                    getch();
                    fallDown(matris,n,m);
                }
            }
        }
    }
    else
    {
        for (int i=0;i<n;i++)
        {
            for (int j=0;j<m;j++)
            {
                if (matris[i][j]==identicals[i][j])
                {
                    matris[i][j]=' ';
                    p_itr--;
                    explode(identicals,matris,n,m,score,p_itr); // Recursive
                }
            }
        }
    }
}
void fallDown(int matris[100][100],int n,int m)
{
    for (int i=n-1;i>=0;i--)
    {
        for (int j=m-1;j>=0;j--)
        {
            if (matris[i][j]!=' '&&matris[i+1][j]==' ')
            {
                while (matris[i+1][j]==' ') // aşağısı boşluk olmayana kadar devam etsin
                {
                    matris[i+1][j]=matris[i][j];
                    matris[i][j]=' ';
                    i++;
                }
            }
        }
    }
    output(matris,n,m);
}
void fillGaps(int matris[100][100],int n,int m)
{
    int color[100][100];

    // To avoid conflict with 0s
    for (int i=0;i<n;i++)
        for (int j=0;j<m;j++)
            color[i][j]=-2;

    for (int i=0;i<n;i++)
    {
        for (int j=0;j<m;j++)
        {
            if (matris[i][j]==' ')
            {
                matris[i][j]=rand()%10;
                color[i][j]=matris[i][j];
            }
        }
    }
    printf("\n");
    // Output with color (red)
    for (int i=0;i<n;i++)
    {
        for (int j=0;j<m;j++)
        {
            if (matris[i][j]==color[i][j])
                printf("\033[1;31m %d ",matris[i][j]);
            else
                printf("\033[1;0m %d ",matris[i][j]);
        }
        printf("\n");
    }
}
void exchange(int matris[100][100],int n,int m)
{
    printf(">Give the indices of objects to be exchanged\n");
    int row1,col1,row2,col2,tmp;
    scanf("%d %d %d %d",&row1,&col1,&row2,&col2);

    tmp=matris[row1-1][col1-1];
    matris[row1-1][col1-1]=matris[row2-1][col2-1];
    matris[row2-1][col2-1]=tmp;

    // Output with color (green)
    printf("\n");
    for (int i=0;i<n;i++)
    {
        for (int j=0;j<m;j++)
        {
            if ((i==row1-1&&j==col1-1)||(i==row2-1&&j==col2-1))
                printf("\033[1;32m %d ",matris[i][j]);
            else
                printf("\033[1;0m %d ",matris[i][j]);
        }
        printf("\n");
    }
}
