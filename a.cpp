#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

const int width = 20, height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
bool gameOver;
enum eDirection
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
eDirection dir;

void setup()
{
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);

    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}

void draw()
{
    clear();

    for (int i = 0; i < width + 2; i++)
        mvprintw(0, i, "#");
    for (int i = 0; i < height + 2; i++)
        mvprintw(i, 0, "#");
    for (int i = 0; i < width + 2; i++)
        mvprintw(height + 1, i, "#");
    for (int i = 0; i < height + 2; i++)
        mvprintw(i, width + 1, "#");

    mvprintw(fruitY, fruitX, "F");

    for (int i = 0; i < nTail; i++)
        mvprintw(tailY[i], tailX[i], "o");

    mvprintw(y, x, "O");

    mvprintw(0, width + 3, "Score: %d", score);

    refresh();
}

void input()
{
    keypad(stdscr, TRUE);
    halfdelay(1);

    int c = getch();

    switch (c)
    {
    case KEY_LEFT:
        dir = LEFT;
        break;
    case KEY_RIGHT:
        dir = RIGHT;
        break;
    case KEY_UP:
        dir = UP;
        break;
    case KEY_DOWN:
        dir = DOWN;
        break;
    case 113:
        gameOver = true;
        break;
    }
}

void logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }
    if (x > width || x < 1 || y > height || y < 1)
        gameOver = true;

    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    if (x == fruitX && y == fruitY)
    {
        score++;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main()
{
    srand(time(0));
    setup();
    while (!gameOver)
    {
        draw();
        input();
        logic();
        usleep(100000);
    }

    endwin();

    std::cout << "Game Over" << std::endl;
    std::cout << "Final score: " << score << std::endl;

    return 0;
}
