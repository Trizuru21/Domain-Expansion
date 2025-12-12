#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h> // for _kbhit() and getch() on Windows
using namespace std;

const int HEIGHT = 10;
const int WIDTH = 30;

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
        if (_kbhit()) {
            char c = getch();
            if (c == 'w' || c == 'W') birdY--; // flap up
        } 
        else {
            birdY++; // gravity
        }

        // Add new pipe
        if (pipes.back().x == WIDTH - 15) {
            pipes.push_back({WIDTH - 1, rand() % (HEIGHT - 3) + 1});
        }

        // Move pipes left
        for (auto &p : pipes) {
            p.x--;
        }

        // Clear screen
        system("cls");

        // Draw game
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                bool drawn = false;

                // Bird
                if (x == 5 && y == birdY) {
                    cout << ">";
                    drawn = true;
                }

                // Pipes
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

        // Collisions
        if (birdY < 0 || birdY >= HEIGHT) gameOver = true;

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

        // Game speed
        _sleep(120);
    }

    cout << "GAME OVER!" << endl;
    cout << "Final Score: " << score << endl;

    return 0;
}
