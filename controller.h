#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <iostream>
#include <ncurses.h>

class Controller {
    public:
        virtual void getInput() = 0;
        virtual int getOutput() = 0;
        virtual ~Controller() {}
};

class NCurseController: public Controller {

    int output;
    public:
        NCurseController();
        void getInput() override;
        int getOutput() override;
};
#endif
