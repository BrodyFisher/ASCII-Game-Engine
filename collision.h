#ifndef COLLISION_H
#define COLLISION_H
#include <iostream>
#include "entity.h"

using namespace std;


class Collision {

    public:
        Collision();
        virtual void resolveCollision(Entity *e1, Entity *e2) = 0;
};

class passCollision: public Collision {

    public:
        passCollision();
        void resolveCollision(Entity *e1, Entity *e2) override;
};

class stopCollision: public Collision {

    public:
        stopCollision();
        void resolveCollision(Entity *e1, Entity *e2) override;
};

class bounceCollision: public Collision {

    bool y, x;
    public:
        bounceCollision(bool x, bool y);
        void resolveCollision(Entity *e1, Entity *e2) override;
};

class gameEndCollision: public Collision {

    public:
        gameEndCollision();
        void resolveCollision(Entity *e1, Entity *e2) override;
};

class destroyCollision: public Collision {

    
    public:
        destroyCollision();
        void resolveCollision(Entity *e1, Entity *e2) override;
};

#endif
