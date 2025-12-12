#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <termios.h>
using namespace std;

const int HEIGHT = 10;
const int WIDTH = 30;

// Non-blocking keyboard input
int kbhit() {
    termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int bytesWaiting;
    ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return bytesWaiting;
}

char getchNow() {
    char c;
    read(STDIN_FILENO, &c, 1);
    return c;
}

struct Pipe {
    int x;
    int gapY;
};

int main() {
    srand(time(0));

    int birdY = HEIGHT / 2;
    int score = 0;
    bool gameOver = false;

    vector<Pipe> pipes;
    pipes.push_back({WIDTH - 1, rand() % (HEIGHT - 3) + 1});

    while (!gameOver) {

        // Input
        if (kbhit()) {
            char c = getchNow();
            if (c == 'w' || c == 'W') birdY--;
        } else {
            birdY++;
        }

        // Add pipes
        if (pipes.back().x == WIDTH - 15) {
            pipes.push_back({WIDTH - 1, rand() % (HEIGHT - 3) + 1});
        }

        // Move pipes
        for (auto &p : pipes) {
            p.x--;
        }

        // Clear screen for Linux
        cout << "\033[2J\033[1;1H";

        // Draw game
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                bool drawn = false;

                if (x == 5 && y == birdY) {
                    cout << ">";
                    drawn = true;
                }

                for (auto &p : pipes) {
                    if (x == p.x && !(y >= p.gapY && y <= p.gapY + 2)) {
                        cout << "|";
                        drawn = true;
                    }
                }

                if (!drawn) cout << " ";
            }
            cout << "\n";
        }

        // Collision: top/bottom
        if (birdY < 0 || birdY >= HEIGHT) gameOver = true;

        // Collision pipes
        for (auto &p : pipes) {
            if (p.x == 5 && !(birdY >= p.gapY && birdY <= p.gapY + 2)) {
                gameOver = true;
            }
        }

        // Score
        for (auto &p : pipes) {
            if (p.x == 4) score++;
        }

        cout << "Score: " << score << endl;

        usleep(120000); // 120ms
    }

    cout << "GAME OVER!" << endl;
    cout << "Final Score: " << score << endl;

    return 0;
}
