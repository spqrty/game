#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HEIGHT 25
#define WIDTH 80

void space(int **old_s);
void life_or_dead(int ***old_s);
void from_file(int ***old_s, int k);
void input_coordinat(int ***old_s);
void work(int ***old_s);

void print_1();

int main() {
    char ch = '0';
    int **old_s = malloc(HEIGHT * sizeof(int *));
    for (int i = 0; i < HEIGHT; i++) {
        old_s[i] = malloc(WIDTH * sizeof(int));
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            old_s[i][j] = 0;
        }
    }
    initscr();
    nodelay(stdscr, 1);
    noecho();
    keypad(stdscr, 1);
    clear();
    print_1();
    while (ch != '1' && ch != '2') {
        ch = getch();
    }
    if (ch == '1') {
        clear();
        system("clear");
        printw("---Game of life---\n   1. File 1\n   2. File 2\n   3. File 3\n   4. File 4\n   5. File 5\n");
        printw("---Press button---\n");
        refresh();
        ch = '0';
        while (ch != '1' && ch != '2' && ch != '3' && ch != '4' && ch != '5') {
            ch = getch();
        }
        from_file(&old_s, (int)(ch - 48));
        ch = '0';
    }
    if (ch == '2') {
        input_coordinat(&old_s);
        noecho();
        nodelay(stdscr, 1);
    }
    keypad(stdscr, 1);
    work(&old_s);
    for (int i = 0; i < HEIGHT; i++) {
        free(old_s[i]);
    }
    free(old_s);
    endwin();
    return 0;
}

void space(int **old_s) {
    system("clear");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (old_s[i][j] != 0)
                printw("%c", '0');
            else
                printw(" ");
        }
        printw("\n");
    }
}

void life_or_dead(int ***old_s) {
    int new_s[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            new_s[i][j] = 0;
        }
    }
    int sum_alive, up, down, left, right, i_, k_;
    for (int i = 0; i < HEIGHT; i++) {
        for (int k = 0; k < WIDTH; k++) {
            i_ = (i + HEIGHT) % HEIGHT;
            k_ = (k + WIDTH) % WIDTH;
            up = (i + 1 + HEIGHT) % HEIGHT;
            down = (i - 1 + HEIGHT) % HEIGHT;
            left = (k - 1 + WIDTH) % WIDTH;
            right = (k + 1 + WIDTH) % WIDTH;

            sum_alive = (*old_s)[down][left] + (*old_s)[down][k_] + (*old_s)[down][right] +
                        (*old_s)[i_][left] + (*old_s)[i_][right] + (*old_s)[up][left] + (*old_s)[up][k_] +
                        (*old_s)[up][right];
            if ((sum_alive == 3 && (*old_s)[i][k] == 0) ||
                ((sum_alive == 2 || sum_alive == 3) && (*old_s)[i][k] == 1)) {
                new_s[i][k] = 1;
            } else
                new_s[i][k] = 0;
        }
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            (*old_s)[i][j] = new_s[i][j];
        }
    }
}

void from_file(int ***old_s, int k) {
    char a;
    FILE *file;
    switch (k) {
        case 1:
            file = fopen("1.txt", "r");
            break;
        case 2:
            file = fopen("2.txt", "r");
            break;
        case 3:
            file = fopen("3.txt", "r");
            break;
        case 4:
            file = fopen("4.txt", "r");
            break;
        case 5:
            file = fopen("5.txt", "r");
            break;
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            a = fgetc(file) - 48;
            (*old_s)[i][j] = a;
        }
        fgetc(file);
    }
    fclose(file);
}

void input_coordinat(int ***old_s) {
    nodelay(stdscr, 0);
    echo();
    int n;
    int x, y;
    system("clear");
    printw("Input coordinate count: ");
    refresh();
    while (scanw("%d", &n) != 1 || n <= 0) {
        printw("Wrong coordinate count. Input coordinate count: \n");
    }
    for (int i = 0; i < n; i++) {
        while (scanw("%d %d", &x, &y) != 2 || x < 0 || x > 24 || y < 0 || y > 79) {
            printw("Wrong coordinate. Input coordinate: \n");
        }
        (*old_s)[x][y] = 1;
    }

    printw("\n");
    refresh();
    // getch(); // Ожидание нажатия клавиши
    endwin();
}

void print_1() {
    system("clear");
    printw("---Game of life---\n");
    printw("   1. File\n");
    printw("   2. Stdin\n");
    printw("---Press button---\n");
    refresh();
    return;
}

void work(int ***old_s) {
    int delay = 500000, flag = 1;
    while (1) {
        keypad(stdscr, 1);
        clear();
        space(*old_s);
        life_or_dead(old_s);
        char ch = getch();
        switch (ch) {
            case 'P':
            case 'p':
                printw("Press 'P' to continue\nPress 'Q' to exit\n");
                do {
                    ch = getch();
                } while (ch != 'p' && ch != 'P' && ch != 'Q' && ch != 'q');
                if (ch == 'Q' || ch == 'q') flag = 0;
                break;
            case 'q':
            case 'Q':
                flag = 0;
                break;
            case 'A':
            case 'a':
                if (delay < 1071793) {
                    delay *= 1.1;
                }
                break;
            case 'Z':
            case 'z':
                if (delay > 21524) {
                    delay /= 1.1;
                }
                break;
        }
        clear();
        space(*old_s);
        printw("Press 'P' to pause\nPress 'Q' to exit\n");
        if (delay <= 21524)
            printw("\nMax speed");
        else if (delay >= 1071793)
            printw("\nMin speed");
        if (!flag) break;
        refresh();
        usleep(delay);
    }
}
