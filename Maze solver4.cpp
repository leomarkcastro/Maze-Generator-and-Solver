//{

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <stdlib.h>
#include <thread>
#include <time.h>


#define wait getch()
//#define clean system("cls")
#define say cout<<
#define input cin>>
#define work cout<<"Working!\t"

#define B_BLACK        0
#define B_BLUE         16
#define B_GREEN        32
#define B_SKY          48
#define B_RED          64
#define B_VIOLET       80
#define B_YELLOW        96
#define B_WHITE        112

#define F_BLACK	    0
#define F_BLUE      1
#define F_GREEN	    2
#define F_SKY  	    3
#define F_RED       4
#define F_VIOLET    5
#define F_YELLOW    6
#define F_WHITE     7
//}

using namespace std;


int thread_opened = 0;

const int maprow = 60;  ///Change the map row and column here if you wish to change the map size
const int mapcol = 60;  ///The map itself is in the main function.
const int one_way = 1;  ///One way
const int random_start = 0; ///Start and end at random point or at the farthest possible separation (watdafuq)

const int anim_speed = 1; ///Change this to lower value for faster animation
const int node_speed = 1;   ///If you want to see the nodes, then set this above 0

const int node_sign = 1; ///0 to display direction at node. 1 to display 'x'

const char nodesprite = ' '; ///Change if you want to see the nodes created for the maze.
const char map_block = char(177);
const int map_color = 7;
const int path_color = 9;
const char path_char = char(219);

const int move_style = 0; ///0 is the erratic but more effective one; 1 is the straightforward but super buggy one

const int wait_before_solution = 500 ; ///Set to 0 if you want to proceed automatically, set to 1 for 'click to continue', set above 1 to automatically proceed

const int wait_before_restart = 2000; ///If 0, then you have to click before restarting, crank above 0 to automatically proceed at the said rate

const int fast_mode = 1;    ///Set to 1 if you want faster answers but is not concerned of the path efficiency. For best result, set the thread limit low
const int tweak_opt = 1; ///Experimental method to make the calculation go faster. Might cause error, turn off in case.

const int optistep1_initial = 5;
const int opti_max_init = 20;

const int thread_limit = 10; ///Will make the process alot faster.  Lower this if you encounter 'termination error'. Don't set too high


/// MAZE SOLVER. BAGUHIN MO YUNG PARAMETERS ABOVE AS YOU WISH



/// The premise is that this program will attempt to find the shortest route from start to finish of a maze.

/// It will assume that there is only one start above and one end at the bottom. Anything else will not work. The highest Start will be the assumed start and the End placed in the lowest part is the assumed end of the maze <if you will try to put multiple terminals>

/// Thanks to multi threading, ambilis na niya magsolve

/// Achievement na sakin maimplement yung recursion of function saka multithreading kahit di ko pa sya totally gets hahahaha




vector<COORD> anspath;
int optisteps = 0;
int already_ans = 0;

void printr2(string *mazemap, COORD mapsize){

   for(int i = 0; i < mapsize.Y; i++){
        for(int j = 0; j < mapsize.X; j++){
            if(mazemap[i][j] == '#') cout << char(177);
            else cout << ' ';
        }
        cout << endl;
    }

}

void mazehashfiller(string *mazemap, COORD mapsize){

    for(int y = 0; y < mapsize.Y; y++)
        for(int x = 0; x < mapsize.X; x++)
            mazemap[y] += '#';

}

void pathbuilder(string *mazemap, COORD mapsize, COORD pathstart){

    //cout << "One Recursion Started\n";

    //system("cls");
    //printr(mazemap, mapsize);

    int randir = 0;

    mazemap[pathstart.Y][pathstart.X] = ' ';

    int run = 1;

    while(run){

        if(pathstart.X < 1 || pathstart.Y < 1 || pathstart.X > (mapsize.X - 1) || pathstart.Y > (mapsize.Y - 1)){run = 0; break;}

        if (
            ((mazemap[pathstart.Y-2][pathstart.X] == '#') && (pathstart.Y - 2 > 1))+
            ((mazemap[pathstart.Y][pathstart.X+2] == '#') && (pathstart.X + 2 < (mapsize.X - 2)))+
            ((mazemap[pathstart.Y+2][pathstart.X] == '#') && (pathstart.Y + 2 < (mapsize.Y - 2))+
            ((mazemap[pathstart.Y][pathstart.X-2] == '#') && (pathstart.X - 2 > 1))
            ) == 0
           ){run = 0; break; }

        randir = (rand() % 4);

        if((randir == 0) && (mazemap[pathstart.Y-2][pathstart.X] == '#') && (pathstart.Y - 2 > 0)){
            mazemap[pathstart.Y - 1][pathstart.X] = ' ';
            pathstart.Y -= 2;
            pathbuilder(mazemap, mapsize, pathstart);
        }

        else if((randir == 1) && (mazemap[pathstart.Y][pathstart.X+2] == '#') && (pathstart.X + 2 < (mapsize.X - 2))){
            mazemap[pathstart.Y][pathstart.X + 1] = ' ';
            pathstart.X += 2;
            pathbuilder(mazemap, mapsize, pathstart);
        }

        else if((randir == 2) && (mazemap[pathstart.Y+2][pathstart.X] == '#') && (pathstart.Y + 2 < (mapsize.Y - 2))){
            mazemap[pathstart.Y + 1][pathstart.X] = ' ';
            pathstart.Y += 2;
            pathbuilder(mazemap, mapsize, pathstart);
        }

        else if((randir == 3) && (mazemap[pathstart.Y][pathstart.X-2] == '#') && (pathstart.X - 2 > 0)){
            mazemap[pathstart.Y][pathstart.X - 1] = ' ';
            pathstart.X -= 2;
            pathbuilder(mazemap, mapsize, pathstart);
        }

        /*
        else {
            break;
        }
        */

    }

    //cout << "One recursion finished\n";



}

void mazerandomer(string *mazemap, COORD mapsize, int random_spots){

    int pointY;
    int pointX;

    for(int i = 0; i < random_spots; i++){

        do{

        pointX = (rand() % mapsize.X);
        pointY = (rand() % mapsize.Y);

        // getch();

        }while((mazemap[pointY][pointX] == ' ') || (pointX < 3) || (pointX > mapsize.X-3) || (pointY < 3) || (pointY > mapsize.Y-3));


        mazemap[pointY-1][pointX-1] = ' ';


    }

}

void mazegenerator(string *mazemap, COORD mapsize, COORD mazestart){

    mazehashfiller(mazemap, mapsize);

    pathbuilder(mazemap, mapsize, mazestart);

    if(one_way == 0) mazerandomer(mazemap, mapsize, (mapsize.X + mapsize.Y)*4);

    //cout << "Finished!!"; getch();

}



COORD randompoint(string *vmap){

    int ycoord, xcoord;

    do{
        ycoord = rand() % maprow;
        xcoord = rand() % mapcol;

    } while(vmap[ycoord][xcoord] == '#');

    COORD retcor = {ycoord, xcoord};

    return retcor;

}

void activatemouse(){   /// You should initiate this function before using any function that involves the mouse

    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD InputRecord;
    DWORD Events;
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 25;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hout, &cci);

    SetConsoleMode(hin, ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
}

void setcolor(int x = 240){      ///Change text cell color
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
}

void gotoxy(int x = 0, int y = 0){    ///Move to console position
    COORD coord = {0,0};
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void printr(string *vmap){

    setcolor(map_color);
    for(int i = 0; i < maprow; i++){
        for(int j = 0; j < mapcol; j++){
            if(vmap[i][j] == '#') say map_block;
            else say ' ';
        }
        say endl;
    }

}

void clean(){   ///Alternative and more friendly console screen cleaner
    COORD coordScreen = { 0, 0 };
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    SetConsoleCursorPosition(hConsole, coordScreen);
    return;
}

int whitecounter(string *vmap){

    int whitecount = 0;

    for(int i = 0; i < maprow; i++){
        for(int j = 0; j < mapcol; j++){
            if (vmap[i][j] == ' ') whitecount++;
        }
    }

    return whitecount;
}

int nodes(string *vmap, vector<COORD> &lcrd, COORD &mstart, COORD &mend){

    /*size_t rowend = 0;
    while (!vmap[rowend].empty()) ++rowend;*/
    int nodecount = 0;

    for(int row = 0; row < maprow; row++){
        for(int column = 0; column < mapcol; column++){
            if (vmap[row][column] == 'S') {lcrd.push_back({row, column}); mstart = {row, column}; nodecount++;}
            if ((vmap[row][column] == ' ')&&((((vmap[row][column+1] != '#')+(vmap[row][column-1] != '#'))> 0))&&((((vmap[row+1][column] != '#')+(vmap[row-1][column] != '#'))> 0))&&((((vmap[row][column+1] != '#')+(vmap[row][column-1] != '#')+(vmap[row+1][column] != '#')+(vmap[row-1][column] != '#')+(vmap[row+1][column+1] != '#')+(vmap[row+1][column-1] != '#')+(vmap[row-1][column+1] != '#')+(vmap[row-1][column-1] != '#'))<= 7))) {lcrd.push_back({row, column}); nodecount++;}
            if (vmap[row][column] == 'E') {lcrd.push_back({row, column}); mend = {row, column}; nodecount++;}
    }}

    return nodecount;
}

COORD direction(int dir){
    COORD direct;
    switch (dir){
    case 0:{
        direct.Y = 0;
        direct.X = -1;
    break; }
    case 1:{
        direct.Y = 1;
        direct.X = 0;
    break; }
    case 2:{
        direct.Y = 0;
        direct.X = 1;
    break; }
    case 3:{
        direct.Y = -1;
        direct.X = 0;
    break; }
    }

    return direct;
}

void traverser(string *vmap, vector<COORD> lcrd, vector<int> nodestat, COORD mstart, COORD mend, int direc, vector<COORD> pcrd, int countersteps, int &routesexp, int &routesfin, int &threadlevel){
    COORD flow = direction(direc);
    bool run = TRUE;
    routesexp++;
    //gotoxy(mapcol+1,0); setcolor(F_WHITE|B_BLACK); printf("Best Route No. of Steps: %4d", optisteps);
    //gotoxy(mapcol+1,1); setcolor(F_WHITE|B_BLACK); printf("Nodes visited: %d", routesexp);
    //gotoxy(mapcol+1,2); setcolor(F_WHITE|B_BLACK); printf("Routes that reached the finish: %d", routesfin);

    //bool solve = FALSE;
    if(node_speed > 0){
    gotoxy(mstart.Y,mstart.X); if(!node_sign) say direc; else say 'x';
    Sleep(node_speed);}


    while(run){
        mstart.X += flow.X;
        mstart.Y += flow.Y;
        countersteps++;

        if(fast_mode && anspath.size() != 0){run = 0; break;}

        if(countersteps > optisteps){run = 0; break;}

        if(vmap[mstart.X][mstart.Y] == '#'){run = 0; break;}

        if(mstart.X < 0 || mstart.X > maprow || mstart.Y < 0 || mstart.Y > mapcol) {run = 0; break;}

        if(mstart.X == mend.X && mstart.Y == mend.Y) {
            run = 0; //solve = 1;
            pcrd.push_back({mstart.X, mstart.Y});
            nodestat[nodestat.size() - 1] = 1;
            routesfin++;

            if(countersteps < optisteps){

            anspath = move(pcrd);
            optisteps = countersteps;


            /*
            for(int i = 0; i != anspath.size(); i++){
                printf("i: %2i | X: %2i | Y: %2i | nS: %2i\n", i, anspath[i].X, anspath[i].Y, nodestat[i]);
            }
            wait;*/

            }

            break;}

        for(int i = 0; i != lcrd.size(); i++){
            if (mstart.X == lcrd[i].X && mstart.Y == lcrd[i].Y) {
                if(nodestat[i] == 1) {run = 0; break;}
                //else {run = 0; break;}
                else{

                nodestat[i] = 1;

                pcrd.push_back({mstart.X, mstart.Y});

                if(move_style == 1 && threadlevel < thread_limit){

                if(direc == 2){

                threadlevel++;

                int use_thread = 0;
                int intarray[5];

                if(vmap[mstart.X+1][mstart.Y] != '#'){
                intarray[use_thread] = 2;
                use_thread++;
                }
                if(vmap[mstart.X][mstart.Y+1] != '#'){
                intarray[use_thread] = 1;
                use_thread++;
                }
                if(vmap[mstart.X][mstart.Y-1] != '#'){
                intarray[use_thread] = 3;
                use_thread++;
                }
                if(vmap[mstart.X-1][mstart.Y] != '#'){
                intarray[use_thread] = 0;
                use_thread++;
                }

                thread threads[use_thread];

                for(int i = 0; i < use_thread; i++){
                threads[i] = thread(traverser, vmap, lcrd, nodestat, mstart, mend, intarray[i], pcrd, countersteps, ref(routesexp), ref(routesfin), ref(threadlevel));}


                for(int i = 0; i < use_thread; i++){
                    threads[i].join();
                }

                }

                else if(direc == 0){

                threadlevel++;

                int use_thread = 0;
                int intarray[5];

                if(vmap[mstart.X-1][mstart.Y] != '#'){
                intarray[use_thread] = 0;
                use_thread++;
                }
                if(vmap[mstart.X][mstart.Y-1] != '#'){
                intarray[use_thread] = 3;
                use_thread++;
                }
                if(vmap[mstart.X][mstart.Y+1] != '#'){
                intarray[use_thread] = 1;
                use_thread++;
                }
                if(vmap[mstart.X+1][mstart.Y] != '#'){
                intarray[use_thread] = 2;
                use_thread++;
                }

                thread threads[use_thread];

                for(int i = 0; i < use_thread; i++){
                threads[i] = thread(traverser, vmap, lcrd, nodestat, mstart, mend, intarray[i], pcrd, countersteps, ref(routesexp), ref(routesfin), ref(threadlevel));}


                for(int i = 0; i < use_thread; i++){
                    threads[i].join();
                }

                }

                else if(direc == 3){

                threadlevel++;

                int use_thread = 0;
                int intarray[5];

                if(vmap[mstart.X][mstart.Y-1] != '#'){
                intarray[use_thread] = 3;
                use_thread++;
                }
                if(vmap[mstart.X+1][mstart.Y] != '#'){
                intarray[use_thread] = 2;
                use_thread++;
                }
                if(vmap[mstart.X-1][mstart.Y] != '#'){
                intarray[use_thread] = 0;
                use_thread++;
                }
                if(vmap[mstart.X][mstart.Y+1] != '#'){
                intarray[use_thread] = 1;
                use_thread++;
                }

                thread threads[use_thread];

                for(int i = 0; i < use_thread; i++){
                threads[i] = thread(traverser, vmap, lcrd, nodestat, mstart, mend, intarray[i], pcrd, countersteps, ref(routesexp), ref(routesfin), ref(threadlevel));}


                for(int i = 0; i < use_thread; i++){
                    threads[i].join();
                }

                }

                else if(direc == 1){

                threadlevel++;

                int use_thread = 0;
                int intarray[5];

                if(vmap[mstart.X][mstart.Y+1] != '#'){
                intarray[use_thread] = 1;
                use_thread++;
                }
                if(vmap[mstart.X+1][mstart.Y] != '#'){
                intarray[use_thread] = 2;
                use_thread++;
                }
                if(vmap[mstart.X-1][mstart.Y] != '#'){
                intarray[use_thread] = 0;
                use_thread++;
                }
                if(vmap[mstart.X][mstart.Y-1] != '#'){
                intarray[use_thread] = 3;
                use_thread++;
                }

                thread threads[use_thread];

                for(int i = 0; i < use_thread; i++){
                threads[i] = thread(traverser, vmap, lcrd, nodestat, mstart, mend, intarray[i], pcrd, countersteps, ref(routesexp), ref(routesfin), ref(threadlevel));}


                for(int i = 0; i < use_thread; i++){
                    threads[i].join();
                }

                }
                }

                else if(move_style == 1){

                if(direc == 2){
                if(vmap[mstart.X+1][mstart.Y] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 2, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                if(vmap[mstart.X][mstart.Y+1] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 1, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                if(vmap[mstart.X][mstart.Y-1] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 3, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                if(vmap[mstart.X-1][mstart.Y] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 0, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                }
                else if(direc == 0){
                if(vmap[mstart.X-1][mstart.Y] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 0, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                if(vmap[mstart.X][mstart.Y-1] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 3, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                if(vmap[mstart.X][mstart.Y+1] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 1, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                if(vmap[mstart.X+1][mstart.Y] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 2, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                }
                else if(direc == 3){
                if(vmap[mstart.X][mstart.Y-1] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 3, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                if(vmap[mstart.X+1][mstart.Y] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 2, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                if(vmap[mstart.X-1][mstart.Y] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 0, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                if(vmap[mstart.X][mstart.Y+1] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 1, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                }
                else if(direc == 1){
                if(vmap[mstart.X][mstart.Y+1] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 1, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                if(vmap[mstart.X+1][mstart.Y] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 2, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                if(vmap[mstart.X-1][mstart.Y] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 0, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                if(vmap[mstart.X][mstart.Y-1] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 3, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                }
                }

                else if(move_style == 0 && threadlevel < thread_limit){

                threadlevel++;

                int use_thread = 0;
                int intarray[5];

                if(vmap[mstart.X+1][mstart.Y] != '#'){
                intarray[use_thread] = 2;
                use_thread++;
                }

                if(vmap[mstart.X][mstart.Y+1] != '#'){
                intarray[use_thread] = 1;
                use_thread++;
                }

                if(vmap[mstart.X][mstart.Y-1] != '#'){
                intarray[use_thread] = 3;
                use_thread++;
                }

                if(vmap[mstart.X-1][mstart.Y] != '#'){
                intarray[use_thread] = 0;
                use_thread++;
                }


                thread threads[use_thread];

                for(int i = 0; i < use_thread; i++){
                threads[i] = thread(traverser, vmap, lcrd, nodestat, mstart, mend, intarray[i], pcrd, countersteps, ref(routesexp), ref(routesfin), ref(threadlevel));}


                for(int i = 0; i < use_thread; i++){
                    threads[i].join();
                }




                //gotoxy(mapcol+1,1); setcolor(F_WHITE|B_BLACK); printf("%d", routesexp);

                }

                else if(move_style == 0){

                if(vmap[mstart.X+1][mstart.Y] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 2, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                if(vmap[mstart.X][mstart.Y+1] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 1, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                if(vmap[mstart.X][mstart.Y-1] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 3, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                if(vmap[mstart.X-1][mstart.Y] != '#'){
                traverser(vmap, lcrd, nodestat, mstart, mend, 0, pcrd, countersteps, routesexp, routesfin, threadlevel);}
                }



                run = 0;
                break;}


            }
        }
    }
}

int main(){

    activatemouse();

    vector<COORD> anspatreset;

    clock_t tick, tock;

    COORD mapsize = {mapcol, maprow};

    COORD mazestart = {2, 2};


    int opti_step1check_initial = optistep1_initial; /// must be same with opti_step1check
    int opti_max = opti_max_init;

    while(1){

    int timestart = 1;

    int thread_level = 0;


    already_ans = 0;

    int opti_step1check = opti_step1check_initial;


    srand(time(0));

    string mapa[maprow + 1];

    mazegenerator(mapa, mapsize, mazestart);


    COORD mapstart, mapend;

    if(random_start){
        mapstart =randompoint(mapa);
        mapend = randompoint(mapa);
    }
    else{
        mapstart = {2,2};
        mapend = {maprow - 4, mapcol - 4};
    }



    mapa[mapstart.X][mapstart.Y] = 'S';
    mapa[mapend.X][mapend.Y] = 'E';

    opti1:

    anspath = move(anspatreset);

    setcolor(15);
    clean();

    int routesexplored = 0;
    int routesfinished = 0;
    vector<COORD> listcoord;
    vector<COORD> pathcoord;
    int nodecount = 0;
    int countersteps = 0;
    int stepstaken = 0;
    COORD mstart, mend;




    nodecount = nodes(mapa, listcoord, mstart, mend);

    vector<int> nodestat (nodecount);
    std::fill( nodestat.begin(), nodestat.end(), 0 );


    for(int i = 0; i != listcoord.size(); i++){
        mapa[listcoord[i].X][listcoord[i].Y] = nodesprite;
    }

    //printf("\nNode Count: %i", nodecount);

    /*for(int i = 0; i != listcoord.size(); i++){
       printf("i: %2i | X: %2i | Y: %2i | nS: %2i\n", i, listcoord[i].X, listcoord[i].Y, nodestat[i]);
    }*/

    if(tweak_opt == 0 || opti_step1check < opti_step1check_initial) optisteps = mapcol * maprow;
    else if (tweak_opt == 1) {optisteps = whitecounter(mapa) * opti_step1check / opti_max; opti_step1check+=2;}
    else if (tweak_opt == 2) optisteps = 3*nodecount / (float(maprow/mend.Y) * float(mapcol/mend.X));


    printr(mapa);

    gotoxy(mstart.Y, mstart.X); setcolor(B_BLUE | F_WHITE); say 'S';
    gotoxy(mend.Y, mend.X); setcolor(B_RED | F_WHITE); say 'E';

    gotoxy(0, maprow+1); setcolor(15); printf("Working with maze. May be fast, too slow, or crash");

    int movedir = 2;





    int use_thread = 0;
    int thread_direc[4] = {0,0,0,0};

    string travtype = ((move_style == 0) ? "Down-Right Direction Priority" : "Straight path priority");
    string fasmo = ((fast_mode == 0) ? "OFF" : "ON");
    string twopt = ((tweak_opt == 0) ? "Max Movement Cap" : ((tweak_opt == 1) ? "Incrementing Radial Cap" : "Experimental Cap"));

    int b = 0;

    gotoxy(mapcol+1,b);printf("Traverse type: %s\n", travtype.c_str()); b++;
    gotoxy(mapcol+1,b);printf("Fast Mode: %s\n", fasmo.c_str()); b++;
    gotoxy(mapcol+1,b);printf("Max thread: %2d\n", thread_limit); b++;
    gotoxy(mapcol+1,b);printf("Init.Opt Mode: %s\n", twopt.c_str()); b+=2;
    gotoxy(mapcol+1,b);printf("Map Size: %2d x %2d\n", mapcol, maprow); b++;
    gotoxy(mapcol+1,b);printf("Available Path: %2d\n", whitecounter(mapa)); b++;
    gotoxy(mapcol+1,b);printf("One way maze: %d\n", one_way); b++;
    gotoxy(mapcol+1,b);printf("Random Start: %d\n", random_start); b+=2;

    gotoxy(mapcol+1,b); setcolor(F_WHITE|B_BLACK); printf("Best Route No. of Steps: %4d <estimate least>", optisteps); b++;
    gotoxy(mapcol+1,b); setcolor(F_WHITE|B_BLACK); printf("Nodes visited: <currently processing>"); b++;
    gotoxy(mapcol+1,b); setcolor(F_WHITE|B_BLACK); printf("Routes that reached the finish: <currently processing>"); b-= 2;

    if(mapa[mstart.X+1][mstart.Y] != '#') {thread_direc[use_thread] = 2; use_thread++;}
    if(mapa[mstart.X-1][mstart.Y] != '#') {thread_direc[use_thread] = 0; use_thread++;}
    if(mapa[mstart.X][mstart.Y+1] != '#') {thread_direc[use_thread] = 1; use_thread++;}
    if(mapa[mstart.X][mstart.Y-1] != '#') {thread_direc[use_thread] = 3; use_thread++;}

    ((timestart == 1) ? tick = clock(), timestart = 0 : NULL);

    thread threads[use_thread];

    for(int i = 0; i < use_thread; i++){
    threads[i] = thread(traverser, mapa, listcoord, nodestat, mstart, mend, thread_direc[i], pathcoord, countersteps, ref(routesexplored), ref(routesfinished), ref(thread_level));}

    if(node_speed == 0){
    gotoxy(mapcol+1,b); setcolor(F_WHITE|B_BLACK); printf("Best Route No. of Steps: %4d                 ", optisteps); b++;
    gotoxy(mapcol+1,b); setcolor(F_WHITE|B_BLACK); printf("Nodes visited: %d                      ", routesexplored); b++;
    gotoxy(mapcol+1,b); setcolor(F_WHITE|B_BLACK); printf("Routes that reached the finish: %d                      ", routesfinished); b-= 2;}

    for(int i = 0; i < use_thread; i++)
    threads[i].join();

    ((anspath.size() != 0) ? tock = clock() : NULL);

    gotoxy(mapcol+1,b); setcolor(F_WHITE|B_BLACK); printf("Best Route No. of Steps: %4d                 ", optisteps); b++;
    gotoxy(mapcol+1,b); setcolor(F_WHITE|B_BLACK); printf("Nodes visited: %d                      ", routesexplored); b++;
    gotoxy(mapcol+1,b); setcolor(F_WHITE|B_BLACK); printf("Routes that reached the finish: %d                      ", routesfinished); b+=2;

    gotoxy(mapcol+1,b);printf("Nodes created: %2d\n", nodecount); b++;
    gotoxy(mapcol+1,b);printf("Thread level explored: %2d\n", thread_level); b++;
    gotoxy(mapcol+1,b);printf("Process speed: %2.2f s\n", float((float(tock)-float(tick))/1000)); b++;


    if(anspath.size() != 0){

    if(wait_before_solution == 0){
        gotoxy(0, maprow+1);
        setcolor(B_BLUE | F_WHITE); printf("NOW READY!!!"); setcolor(B_BLACK | F_WHITE); printf("                                      ");
    }
    else if(wait_before_solution == 1){
        gotoxy(0, maprow+1);
        setcolor(B_BLUE | F_WHITE); printf("NOW READY!!! PRESS ANY KEY TO CONTINUE"); setcolor(B_BLACK | F_WHITE); printf("            ");
        wait;
    }
    else if(wait_before_solution > 1){
        gotoxy(0, maprow+1);
        setcolor(B_BLUE | F_WHITE); printf("NOW READY!!!"); setcolor(B_BLACK | F_WHITE); printf("                                      ");
        Sleep(wait_before_solution);}
    }

    /*for(int i = 0; i != anspath.size(); i++){
        printf("i: %2i | X: %2i | Y: %2i | nS: %2i\n", i, anspath[i].X, anspath[i].Y, nodestat[i]);
    }*/


    for(int i = 0; i != anspath.size(); i++){

        int next =1;
        while(next){

                gotoxy(mstart.Y, mstart.X); setcolor(path_color); say path_char;
                //gotoxy(maprow+1, mapcol-3); setcolor(F_WHITE | B_RED); printf("goalX: %2i | goalY: %2i", anspath[i].X, anspath[i].Y);
                //gotoxy(maprow+1, mapcol-2); setcolor(F_WHITE | B_BLUE); printf("youX: %2i | youY: %2i", mstart.X, mstart.Y);

                if(mstart.X > anspath[i].X){mstart.X-= 1;}
                else if(mstart.X < anspath[i].X){mstart.X+=1;}
                else if(mstart.Y > anspath[i].Y){mstart.Y-=1;}
                else if(mstart.Y < anspath[i].Y){mstart.Y+=1;}

                //Sleep(anim_speed);
                Sleep(anim_speed);
                stepstaken++;
                if ((mstart.X == anspath[i].X)&&(mstart.Y == anspath[i].Y)) next = 0;


        }

        gotoxy(mstart.Y, mstart.X); setcolor(path_color); say path_char;

    }


    if(opti_step1check != opti_step1check_initial && opti_step1check < opti_max && anspath.size() == 0){goto opti1;}

    gotoxy(0, maprow+2);
    setcolor(B_BLACK|F_RED);

    if(anspath.size() == 0)
        say "\n\nRecheck your map, it is impossible to reach the end. Check if some hashes blocks the path.\n\n";


    setcolor(B_BLACK|F_WHITE);

    if(wait_before_restart == 0) wait;
    else Sleep(wait_before_restart);

    clean();
    setcolor(15);

    optisteps = maprow * mapcol;
    }


}
