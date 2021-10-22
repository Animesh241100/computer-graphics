// A simple OpenGL program that draws a car.

#include "GL/freeglut.h"
#include "GL/gl.h"
#include<math.h>
#include<time.h>
#include<utility>
#include<iostream>
#include<vector>
#include<unistd.h>

const int MIN_CROWD = 1;
const int MAX_CROWD = 4;

using namespace std;

void draw_life();
void draw_image();


class GameBoard {
  public:
    vector<vector<bool>> board_;
    int size_;
    GameBoard(int size);
    void GameBoardInit();
    void GameBoardAutoInit();
    void TransitionToNextState();
    void DisplayBoard(); 
    void print_mat() {
        for(int i = 0; i < size_; i++) {
            for(int j = 0; j < size_; j++) 
                cout << board_[i][j] << " ";
            cout << endl;
        }
    }

  private:
    void CellNextState(int x, int y, vector<pair<int,int>> &death_row, vector<pair<int,int>> &birth_row);
    int get_live_count(int x, int y);
    bool IsInRange(int i, int j);
    bool IsAlive(int i, int j);
    
};


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(1000, 1000);
    glutCreateWindow("Game Of Life!");
    glutDisplayFunc(draw_image);
    glutMainLoop();
    return 0;
}


void draw_image() {
    srand(time(0));
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.8, 1.0, 1.0);
    glOrtho(0, 100, 0, 100, 0, 100);
    draw_life();
}

void draw_life() {
    GameBoard Game = GameBoard(100);
    Game.GameBoardAutoInit();
    glPointSize(12);
    // int i = 0;
    while(1) {
        glBegin(GL_POINTS);
        Game.DisplayBoard();       
        // glVertex2d(1000+i, 1000+i);
        // glVertex2d(1000+i, 90000-i);
        // glVertex2d(90000-i, 1000+i);
        // glVertex2d(90000-i, 90000-i);
        // Game.print_mat();
        usleep(40000);
        Game.TransitionToNextState();
        glEnd();
        glFlush();
        glClear(GL_COLOR_BUFFER_BIT);
        // i += 50;
    }
}





/*******************************************************************************************************/


GameBoard::GameBoard(int size) {
    vector<vector<bool>> matrix(size, vector<bool>(size, false));
    board_ = matrix;
    size_ = size;
}

void GameBoard::GameBoardInit() {
    int m, n, u, v;
    cout << "Enter the number of init objects: ";
    cin >> m;
    while(m--) {
        cout << "Enter the Number of Points to Initialize: ";
        cin >> n;
        for(int i = 0; i < n; i++) {
            // cout << "Enter the " << i << "th ordered pair of point: ";
            cin >> u >> v;
            board_[u][v] = true;
        }
    }
}

void GameBoard::GameBoardAutoInit() {
    int n = 10000;
    while(n--) {
        int u = rand() % 100;
        int v = rand() % 100;
        board_[u][v] = true;
    }
}


void GameBoard::TransitionToNextState() {
    vector<pair<int,int>> death_row;
    vector<pair<int,int>> birth_row;
    for(int i = 0; i < size_; i++) {
        for(int j = 0; j < size_; j++) 
            CellNextState(i, j, death_row, birth_row);
    }
    for(pair<int,int> p : death_row)
        board_[p.first][p.second] = false;
    for(pair<int,int> p : birth_row)
        board_[p.first][p.second] = true;
}

void GameBoard::CellNextState(int x, int y, vector<pair<int,int>> &death_row, vector<pair<int,int>> &birth_row) {
    int live_count = get_live_count(x, y); 
    if(IsAlive(x, y) && (live_count <= MIN_CROWD || live_count >= MAX_CROWD)) 
        death_row.push_back({x, y});
    else if(live_count == 3)
        birth_row.push_back({x, y});
}

int GameBoard::get_live_count(int x, int y) {
    int live_count = 0;
    for(int i = x - 1; i <= x + 1; i++) {
        for(int j = y - 1; j <= y + 1; j++) {
            if(!(i == x && j == y) && IsInRange(i, j) && IsAlive(i, j))
                live_count++;
        }
    }
    return live_count;
}

bool GameBoard::IsInRange(int i, int j) {
    return (i >= 0 && i < size_ && j >= 0 && j < size_) ;
}

bool GameBoard::IsAlive(int i, int j) {
    return board_[i][j];
}

void GameBoard::DisplayBoard() {
    for(int i = 0; i < size_; i++) {
        for(int j = 0; j < size_; j++) {
            if(IsAlive(i, j))
                glVertex2d(i, j);
        }
    }
}



/*

0. Still
4
40 41
40 42
41 41
41 42

1. Blinkers
3
50 51
50 52
50 53


2. Octa Blinkers
8
50 50
50 51
51 50
51 51
52 52
52 53
53 52
53 53


3. Gliders
5
50 49
50 50
50 51
49 51
48 50


4. Spaceships
11
1 0
3 0
4 1
4 2
4 3
4 4
4 5
3 5
2 5
1 4
0 2

11
21 30
23 30
24 31
24 32
24 33
24 34
24 35
23 35
22 35
21 34
20 32


4. Glider gun
36
54 50
54 51
55 50
55 51

54 60
55 60
56 60

53 61 
57 61

52 62
58 62

52 63
58 63

55 64

53 65
57 65

54 66
55 66
56 66

55 67

52 70
53 70 
54 70

52 71
53 71
54 71

51 72
55 72

50 74 
51 74
55 74
56 74

52 84
52 85
53 84
53 85



36
4 0
4 1
5 0
5 1
4 10
5 10
6 10
3 11 
7 11
2 12
8 12
2 13
8 13
5 14
3 15
7 15
4 16
5 16
6 16
5 17
2 20
3 20 
4 20
2 21
3 21
4 21
1 22
5 22
0 24 
1 24
5 24
6 24
2 34
2 35
3 34
3 35


36
0 94
1 94
0 95
1 95
10 94
10 95
10 96
11 93 
11 97
12 92
12 98
13 92
13 98
14 95
15 93
15 97
16 94
16 95
16 96
17 95
20 92
20 93 
20 94
21 92
21 93
21 94
22 91
22 95
24 90 
24 91
24 95
24 96
34 92
35 92
34 93
35 93 




*/