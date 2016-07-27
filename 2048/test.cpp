#include "2048.h"

int main() {
    int n;
    cout << "Enter 'A/a' to turn left"<< endl;
    cout << "Enter 'D/d' to turn right"<< endl;
    cout << "Enter 'W/w' to turn up"<< endl;
    cout << "Enter 'S/s' to turn down"<< endl;
    cout << endl;
    cout << "Now give me the board's size" << endl;
    cin >> n;
    cin.ignore(INT_MAX, '\n');
    Game game(n);
    game.init();
    game.play();
    endwin();
    return 0;
}

