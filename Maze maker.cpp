#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

using namespace std;

void printr(string *mazemap, COORD mapsize){

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

    mazerandomer(mazemap, mapsize, (mapsize.X + mapsize.Y)*7/10);

    //cout << "Finished!!"; getch();

}

int main(){

    srand(time(0));

    COORD mapsize = {60, 30};

    COORD mazestart = {2, 2};

    system("cls");

    //cout << "Random Maze Maker\n\n";
    //cout << "Number of Rows: "; cin >> mapsize.Y;
    //cout << "Number of Column: "; cin >> mapsize.X;

    string mapa[mapsize.Y];

    mazegenerator(mapa, mapsize, mazestart);

    system("cls");

    printr(mapa, mapsize);
}
