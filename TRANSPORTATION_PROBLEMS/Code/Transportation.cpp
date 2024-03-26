#include<iostream>
#include<vector>
#include<fstream>
#include<iomanip>

#define Arr vector<int>
#define Matrix vector<Arr>
#define coor pair<int,int>
#define row first
#define col second
#define oo INT_MAX
#define Path pair<pair<int,int>,int>
#define dir second
#define Matrix_Path vector<vector<Path>>
#define push push_back

using namespace std;

ifstream in("C:/Users/Admin/Desktop/University/Year 2/HocKy2/NhapMonOR/TRANSPORTATION_PROBLEMS/Data/test1.txt");

void Read(int &Sour, int &Dest, Matrix &Cost, Arr &Supply, Arr &Demand)
{
    int i;

    in >> Sour >> Dest;
    Cost.resize(Sour);

    for(i=0;i<Sour;i++)
    {
        Cost[i].resize(Dest);
        for(int &j:Cost[i])in >> j;
    }

    Supply.resize(Sour);
    for(int &j:Supply)in >> j;

    Demand.resize(Dest);
    for(int &j:Demand)in >> j;
}

void Print(int Sour, int Dest, Matrix Cost, Arr Supply, Arr Demand)
{
    int i,j;

    cout <<"* Problem Table:\n\n";
    for(i=0;i<Dest;i++)cout <<"------";
    cout <<"-\n";
    for(i=0;i<Sour;i++)
    {
        for(j=0;j<Dest;j++)cout << "|" << setw(5) << right << Cost[i][j];
        cout << "|" << setw(5) << right << Supply[i];
        cout <<'\n';
        for(j=0;j<Dest;j++)cout <<"------";
        cout <<"-------\n";
    }
    cout <<" ";
    for(j=0;j<Dest;j++)cout << setw(5) << right << Demand[j] << "|";
    cout <<"\n\n\n";
}

void PrintSol(int Sour, int Dest, Matrix Cost, Matrix Ans)
{
    int i,j;

    for(i=0;i<Dest;i++)cout <<"---------";
    cout <<"-\n";
    for(i=0;i<Sour;i++)
    {
        for(j=0;j<Dest;j++)
        {
            cout << "|" << setw(3) << right << Cost[i][j];
            if(Ans[i][j])cout <<"("<< setw(3) << right << Ans[i][j] <<")";
            else cout <<" "<< setw(3) << right <<" "<<" ";
        }
        cout <<"|\n";
        for(j=0;j<Dest;j++)cout <<"---------";
        cout <<"-\n";
    }
    cout <<'\n';
}

void Create_Matrix(int Sour, int Dest, Matrix &Ans)
{
    int i;

    Ans.resize(Sour);
    for(i=0;i<Sour;i++)Ans[i].resize(Dest);
}

coor Least_Cost_Cell(int Sour, int Dest, Matrix Cost, Arr Row_Cancelled, Arr Col_Cancelled)
{
    int Min=oo,i,j;
    coor Pos;

    for(i=0;i<Sour;i++)
    {
        if(Row_Cancelled[i])continue;
        for(j=0;j<Dest;j++)
        {
            if(Col_Cancelled[j])continue;
            if(Min>Cost[i][j])
            {
                Min=Cost[i][j];
                Pos={i,j};
            }
        }
    }
    return Pos;
}

void LCEM(int Sour, int Dest, Matrix Cost, Arr Supply, Arr Demand, Matrix &Ans)
{
    int cnt=0,u,v,Res=0;
    coor Min_Cost;
    Arr Row_Cancelled(Sour,0), Col_Cancelled(Dest,0);

    while(cnt<Sour+Dest-1)
    {
        Min_Cost=Least_Cost_Cell(Sour, Dest, Cost, Row_Cancelled, Col_Cancelled); // Select the cell with the lowest transportation cost
        u=Min_Cost.row;
        v=Min_Cost.col;

        // Allocate
        if(Supply[u]<Demand[v])
        {
            Row_Cancelled[u]=1;
            Ans[u][v]=Supply[u];
        }
        else
        {
            Col_Cancelled[v]=1;
            Ans[u][v]=Demand[v];
        }
        Supply[u]-=Ans[u][v];
        Demand[v]-=Ans[u][v];
        Res+=Ans[u][v]*Cost[u][v];
        cnt++;
    }
    cout << "* LCEM initial BFS:\n\n";
    PrintSol(Sour, Dest, Cost, Ans);
    cout <<"The total transportation cost: "<< Res <<"\n\n";
}

void Update_Penalty(int Sour, int Dest, Matrix Cost, Arr &Penalty_Row, Arr &Penalty_Col, Arr Row_Cancelled, Arr Col_Cancelled)
{
    int i,Min1,Min2,Pos,j;

    // Update Row
    for(i=0;i<Sour;i++)
    {
        if(Row_Cancelled[i]){Penalty_Row[i]=-oo;continue;}
        Min1=oo;
        Min2=oo;
        Pos=0;
        for(j=0;j<Dest;j++)
        {
            if(Col_Cancelled[j])continue;
            if(Min1>Cost[i][j])
            {
                Min1=Cost[i][j];
                Pos=j;
            }
        }
        for(j=0;j<Dest;j++)
        {
            if(Col_Cancelled[j]||j==Pos)continue;
            Min2=min(Min2,Cost[i][j]);
        }
        if(Min1==oo)Penalty_Row[i]=-oo;else
        if(Min2==oo)Penalty_Row[i]=Min1;else
        Penalty_Row[i]=Min2-Min1;
    }

    //Update Col
    for(j=0;j<Dest;j++)
    {
        if(Col_Cancelled[j]){Penalty_Col[j]=-oo;continue;}
        Min1=oo;
        Min2=oo;
        Pos=0;
        for(i=0;i<Sour;i++)
        {
            if(Row_Cancelled[i])continue;
            if(Min1>Cost[i][j])
            {
                Min1=Cost[i][j];
                Pos=i;
            }
        }
        for(i=0;i<Sour;i++)
        {
            if(Row_Cancelled[i]||i==Pos)continue;
            Min2=min(Min2,Cost[i][j]);
        }
        if(Min1==oo)Penalty_Col[j]=-oo;else
        if(Min2==oo)Penalty_Col[j]=Min1;else
        Penalty_Col[j]=Min2-Min1;
    }
}

int Max_Penalty(int n, Arr Penalty)
{
    int Max=-oo,i,Pos=0;

    for(i=0;i<n;i++)
    {
        if(Max<Penalty[i])
        {
            Max=Penalty[i];
            Pos=i;
        }
    }
    return Pos;
}

int Min_Cost_Col(int Sour, int Col, Arr Row_Cancelled, Matrix Cost)
{
    int i,Min=oo,Pos=0;

    for(i=0;i<Sour;i++)
    {
        if(Row_Cancelled[i])continue;
        if(Min>Cost[i][Col])
        {
            Min=Cost[i][Col];
            Pos=i;
        }
    }
    return Pos;
}

int Min_Cost_Row(int Dest, int Row, Arr Col_Cancelled, Matrix Cost)
{
    int j,Min=oo,Pos=0;

    for(j=0;j<Dest;j++)
    {
        if(Col_Cancelled[j])continue;
        if(Min>Cost[Row][j])
        {
            Min=Cost[Row][j];
            Pos=j;
        }
    }
    return Pos;
}

void VAM(int Sour, int Dest, Matrix Cost, Arr Supply, Arr Demand, Matrix &Ans)
{
    int cnt=0,u,v,MP_Row,MP_Col,Res=0,MCC,MCR;
    Arr Row_Cancelled(Sour,0), Col_Cancelled(Dest,0), Penalty_Row(Sour), Penalty_Col(Dest);

    while(cnt<Sour+Dest-1)
    {
        Update_Penalty(Sour, Dest, Cost, Penalty_Row, Penalty_Col, Row_Cancelled, Col_Cancelled); // Penalty calculation

        // Find Max Penalty
        MP_Row=Max_Penalty(Sour, Penalty_Row);
        MP_Col=Max_Penalty(Dest, Penalty_Col);

        if(Penalty_Row[MP_Row]<Penalty_Col[MP_Col])
        {
            MCC=Min_Cost_Col(Sour, MP_Col, Row_Cancelled, Cost);
            u=MCC;v=MP_Col;
        }
        else
        {
            MCR=Min_Cost_Row(Dest, MP_Row, Col_Cancelled, Cost);
            u=MP_Row;v=MCR;
        }

        // Allocate
        if(Supply[u]<Demand[v])
        {
            Row_Cancelled[u]=1;
            Ans[u][v]=Supply[u];
        }
        else
        {
            Col_Cancelled[v]=1;
            Ans[u][v]=Demand[v];
        }
        Supply[u]-=Ans[u][v];
        Demand[v]-=Ans[u][v];
        Res+=Ans[u][v]*Cost[u][v];
        cnt++;
    }
    cout << "* VAM initial BFS:\n\n";
    PrintSol(Sour, Dest, Cost, Ans);
    cout <<"The total transportation cost: "<< Res <<"\n\n";
}

void Cal_UV(int Sour, int Dest, Matrix Cost, Matrix Basic_FS, Arr &U, Arr &V)
{
    int i,j,cnt=0;

    while(cnt<Sour+Dest-1)
    {
        for(i=0;i<Sour;i++)
        {
            for(j=0;j<Dest;j++)
            {
                if(Basic_FS[i][j])
                {
                    if(U[i]==oo&&V[j]==oo)
                    {
                        if(cnt==0)U[i]=0;else continue;
                    }
                    if(U[i]==oo){U[i]=Cost[i][j]-V[j];cnt++;}
                    else
                    if(V[j]==oo){V[j]=Cost[i][j]-U[i];cnt++;}
                }
            }
        }
    }
}

void Cal_dij(int Sour, int Dest, Matrix Cost, Matrix Basic_FS, Arr U, Arr V, Matrix &d)
{
    int i,j;

    for(i=0;i<Sour;i++)
    {
        for(j=0;j<Dest;j++)
        {
            if(!Basic_FS[i][j])d[i][j]=Cost[i][j]-(U[i]+V[j]);
        }
    }
}

bool IsOptimal(int Sour, int Dest, Matrix d, Matrix Basic_FS)
{
    int i,j;

    for(i=0;i<Sour;i++)
    {
        for(j=0;j<Dest;j++)
        {
            if(!Basic_FS[i][j]&&d[i][j]<0)return 0;
        }
    }
    return 1;
}

coor Largest_Negative(int Sour, int Dest, Matrix d)
{
    int Max=-oo,i,j;
    coor Pos;

    for(i=0;i<Sour;i++)
    {
        for(j=0;j<Dest;j++)
        {
            if(d[i][j]<0)
            {
                if(Max<d[i][j])
                {
                    Max=d[i][j];
                    Pos={i,j};
                }
            }
        }
    }
    return Pos;
}

bool Check(int u, int v, int Sour, int Dest)
{
    return 0<=u&&u<Sour&&0<=v&&v<Dest;
}

void DFS(int U, int V, int Sour, int Dest, Matrix Basic_FS, Matrix_Path Father, Matrix_Path &Trace, int dir, int x, int y, int Length, int &Min_Length, Matrix Is_Visited, bool IsClosedPath, int &Last_Dir)
{
    int u,v;
    int Move[4][2] =
    {
        {-1, 0},
        {0, 1},
        { 1, 0},
        { 0, -1}
    };

    /*
    North 0
    East  1
    South 2
    West  3
    */

    if(Length>Min_Length)return;
    if(x==U&&y==V&&IsClosedPath){Trace=Father;Min_Length=Length;Last_Dir=dir;return;}
    IsClosedPath=1;

    u=x+Move[dir][0];
    v=y+Move[dir][1];

    if(Check(u, v, Sour, Dest))  // Check if the coordinates are in the table
    {
        //go straight
        Is_Visited[u][v]=1;
        Father[u][v]={{x,y},dir};
        DFS(U, V, Sour, Dest, Basic_FS, Father, Trace, dir, u, v, Length+1, Min_Length, Is_Visited, IsClosedPath, Last_Dir);
        Is_Visited[u][v]=0;
    }
    if(Basic_FS[x][y]||(x==U&&y==V))
    {
        // go to the left
        u=x+Move[(dir-1)%4][0];
        v=y+Move[(dir-1)%4][1];
        if(Check(u, v, Sour, Dest)&&!Is_Visited[u][v])
        {
            Is_Visited[u][v]=1;
            Father[u][v]={{x,y},dir};
            DFS(U, V, Sour, Dest, Basic_FS, Father, Trace, (dir-1)%4, u, v, Length+1, Min_Length, Is_Visited, IsClosedPath, Last_Dir);
            Is_Visited[u][v]=0;
        }
        // go to the right
        u=x+Move[(dir+1)%4][0];
        v=y+Move[(dir+1)%4][1];
        if(Check(u, v, Sour, Dest))
        {
            Is_Visited[u][v]=1;
            Father[u][v]={{x,y},dir};
            DFS(U, V, Sour, Dest, Basic_FS, Father, Trace, (dir+1)%4, u, v, Length+1, Min_Length, Is_Visited, IsClosedPath, Last_Dir);
            Is_Visited[u][v]=0;
        }
    }
    if(x==U&&y==V)
    {
        // go back
        u=x+Move[(dir+2)%4][0];
        v=y+Move[(dir+2)%4][1];
        if(Check(u, v, Sour, Dest))
        {
            Is_Visited[u][v]=1;
            Father[u][v]={{x,y},dir};
            DFS(U, V, Sour, Dest, Basic_FS, Father, Trace, (dir+2)%4, u, v, Length+1, Min_Length, Is_Visited, IsClosedPath, Last_Dir);
            Is_Visited[u][v]=0;
        }
    }
}

void Update_Basic_FS(Matrix &Basic_FS, int Min_Length, int U, int V, Matrix_Path Trace, int dir, int Sour, int Dest)
{
    int sign=0,i,j,Min=oo;
    coor Pos;

    // Trace and Mark
    Path X=Trace[U][V];
    Matrix Mark;
    Create_Matrix(Sour, Dest, Mark);
    Mark[U][V]=2;

    /*
    + 2
    - 1
    */

    while(--Min_Length)
    {
        if(X.second!=dir)
        {
            Mark[X.first.first][X.first.second]=(sign%2==0?1:2);
            if(sign%2==0)
            {
                Min=min(Min,Basic_FS[X.first.first][X.first.second]);
            }
            sign++;
        }
        dir=X.second;
        X=Trace[X.first.first][X.first.second];

    }

    // Update BFS
    for(i=0;i<Sour;i++)
    {
        for(j=0;j<Dest;j++)
        {
            if(Mark[i][j]==2)Basic_FS[i][j]+=Min;
            if(Mark[i][j]==1)Basic_FS[i][j]-=Min;
        }
    }

}

void Create_Matrix_Path(int Sour, int Dest, Matrix_Path &Ans)
{
    int i;

    Ans.resize(Sour);
    for(i=0;i<Sour;i++)Ans[i].resize(Dest);
}

void MODI(int Sour, int Dest, Matrix Cost, Arr Supply, Arr Demand, Matrix Basic_FS)
{
    int u,v,Res=0,i,j;
    coor LN;

    while(1)
    {
        Arr U(Sour, oo), V(Dest, oo);
        Matrix d,Is_Visited;
        Matrix_Path Father, Trace;

        Create_Matrix(Sour, Dest, d);
        Create_Matrix_Path(Sour, Dest, Father);
        Create_Matrix_Path(Sour, Dest, Trace);
        Create_Matrix(Sour, Dest, Is_Visited);

        int Min_Length=oo, Last_Dir=0;

        Cal_UV(Sour, Dest, Cost, Basic_FS, U, V); // Find ui and vj
        Cal_dij(Sour, Dest, Cost, Basic_FS, U, V, d); // Find dij
        if(IsOptimal(Sour, Dest, d, Basic_FS))break; // Check the sign of all dij
        LN=Largest_Negative(Sour, Dest, d); // Select the unoccupied cell with the largest negative value of dij
        u=LN.row;
        v=LN.col;

        DFS(u, v, Sour, Dest, Basic_FS, Father, Trace, 1, u, v, 0, Min_Length, Is_Visited, 0, Last_Dir); // Find the closed path
        Update_Basic_FS(Basic_FS, Min_Length, u, v, Trace, Last_Dir, Sour, Dest);  //Mark and update BFS
    }
    cout << "* MODI Method:\n\n";
    PrintSol(Sour, Dest, Cost, Basic_FS);
    for(i=0;i<Sour;i++)
        for(j=0;j<Dest;j++)
            Res+=Cost[i][j]*Basic_FS[i][j];
    cout <<"The total transportation cost: "<< Res <<"\n\n";
}

int main()
{
    int Sour, Dest;
    Matrix Cost, Sol_LCEM, Sol_VAM;
    Arr Supply, Demand;

    Read(Sour, Dest, Cost, Supply, Demand);

    Create_Matrix(Sour, Dest, Sol_LCEM);
    Create_Matrix(Sour, Dest, Sol_VAM);

    Print(Sour, Dest, Cost, Supply, Demand);

    LCEM(Sour, Dest, Cost, Supply, Demand, Sol_LCEM);
    VAM(Sour, Dest, Cost, Supply, Demand, Sol_VAM);

    MODI(Sour, Dest, Cost, Supply, Demand, Sol_VAM);
}







