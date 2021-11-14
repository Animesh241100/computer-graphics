// A simple OpenGL program that draws a car.

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include<math.h>
#include<time.h>
#include<utility>
#include<iostream>
#include<vector>
#include<unistd.h>

#define M_SIZE 100
using namespace std;

void draw_life();
void draw_image();
void draw_start_button();
void draw_auto_button();


class GameBoard {
  public:
    vector<vector<bool>> board_;
    int size_;
    GameBoard(int size);
    void GameBoardInit();
    void GameBoardMouseInit();
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
    bool IsInRange(int i, int j);

  private:
    void CellNextState(int x, int y, vector<pair<int,int>> &death_row, vector<pair<int,int>> &birth_row);
    int get_live_count(int x, int y);
    bool IsAlive(int i, int j);
    
};

const int MIN_CROWD = 1;
const int MAX_CROWD = 4;
bool is_input_mode = true;
GameBoard Game = GameBoard(M_SIZE);

void HandleMouseClick(int button, int state, int x, int y);
void new_Fun();

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(0, 0);
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
    glOrtho(0, M_SIZE, 0, M_SIZE, 0, M_SIZE);
    draw_life();
}

void draw_life() {
    // Game.GameBoardInit();
    glPointSize(10);
    Game.GameBoardMouseInit();
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
    int n = 1000;
    while(n--) {
        int u = rand() % M_SIZE;
        int v = rand() % M_SIZE;
        board_[u][v] = true;
    }
}

void GameBoard::GameBoardMouseInit() {
    int n = 5;
    // while(1) {
        // cout << "h " << endl;
        glutMouseFunc(HandleMouseClick);
    // }
}

void HandleMouseClick(int button, int state, int x, int y) {
    draw_start_button();
    draw_auto_button();
    int c_x = round((double)x * (double)(M_SIZE-1)/(double)(999)); // 199/999
    int c_y = round((double)y * (double)(M_SIZE-1)/(double)(703)); // 199/703
    // cout << button << " " << state << " " << c_x << " " << c_y << endl;
    glColor3f(1.0, 0.4, 0.0);
    glBegin(GL_POINTS);
    glClear(GL_COLOR_BUFFER_BIT);
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if(Game.IsInRange(c_x, c_y));
            Game.board_[c_x][M_SIZE-1-c_y] = true; 
        Game.DisplayBoard();       
    }
    glEnd();    
    glFlush();
    if(c_x > 80 && c_y <= 7){
        Game.GameBoardAutoInit();
        new_Fun();
    }
    else if(c_x <= 7 && c_y <= 7){
        new_Fun();
    }
}

void draw_start_button() {
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2i(3,97);
    glVertex2i(7,95);
    glVertex2i(3,92);
    glEnd();
    glFlush();
}

void draw_auto_button() {
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2i(88,92);
    glVertex2i(97,92);
    glVertex2i(97,97);
    glVertex2i(88,97);
    glEnd();
    glFlush();
}


void new_Fun() {
    while(1) {
        glBegin(GL_POINTS);
        Game.DisplayBoard();       
        usleep(40000);
        Game.TransitionToNextState();
        glEnd();
        glFlush();
        glClear(GL_COLOR_BUFFER_BIT);
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
    int prev_x = x == 0 ? size_ - 1 : x - 1;
    int post_x = (x + 1) % size_;
    int prev_y = y == 0 ? size_ - 1 : y - 1;
    int post_y = (y + 1) % size_;
    live_count = IsAlive(prev_x, prev_y) ? live_count + 1 : live_count;
    live_count = IsAlive(x, prev_y) ? live_count + 1 : live_count;
    live_count = IsAlive(post_x, prev_y) ? live_count + 1 : live_count;
    live_count = IsAlive(prev_x, y) ? live_count + 1 : live_count;
    live_count = IsAlive(post_x, y) ? live_count + 1 : live_count;
    live_count = IsAlive(prev_x, post_y) ? live_count + 1 : live_count;
    live_count = IsAlive(x, post_y) ? live_count + 1 : live_count;
    live_count = IsAlive(post_x, post_y) ? live_count + 1 : live_count;
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