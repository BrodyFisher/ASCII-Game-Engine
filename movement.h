#ifndef MOVEMENT_H
#define MOVEMENT_H
#include <iostream>
#include <ncurses.h>
#include "entity.h"

using namespace std;

class Movement {

    int speed; // measured in how many cells you move every clock cycle
    public:
        Movement(int speed);
        void pos(Entity* e);
        virtual void updatePosition(charEntity &ce) = 0;
        virtual void updatePosition(rectEntity &re) = 0;
        virtual void updatePosition(bitmapEntity &bme) = 0;
        virtual void flipMove(bool y, bool x) = 0;
        int getSpeed() const;
        virtual ~Movement() {}
};


class lineMovement: public Movement {

    int dy, dx; // can be one of four directions
    public:
        friend class Entity;
        lineMovement(int speed, int dy, int dx);
        void updatePosition(charEntity &ce) override;
        void updatePosition(rectEntity &re) override;
        void updatePosition(bitmapEntity &bme) override;
        void flipMove(bool y, bool x) override;
};

class inplaceMovement: public Movement {

    vector<char> sequence;
    int size;
    size_t index = 0;
    public:
        friend class Entity;
        inplaceMovement(int speed, vector<char> sequence);
        void updatePosition(charEntity &ce) override;
        void updatePosition(rectEntity &re) override;
        void updatePosition(bitmapEntity &bme) override;
        void flipMove(bool y, bool x) override;
};

class gravitateMovement: public Movement {

    // border names
    const int TOP = 0;
    const int RIGHT = 1;
    const int BOTTOM = 2;
    const int LEFT = 3;

    int border;
    public:
        friend class Entity;
        gravitateMovement(int speed, int border);
        void updatePosition(charEntity &ce) override;
        void updatePosition(rectEntity &re) override;
        void updatePosition(bitmapEntity &bme) override;
        void flipMove(bool y, bool x) override;
};

#endif
