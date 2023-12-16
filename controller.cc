#include "controller.h"

NCurseController::NCurseController() {

    keypad(stdscr, true);
}

void NCurseController::getInput() {

    timeout(50);
    output = getch();
}

int NCurseController::getOutput() {

    return output;
}
