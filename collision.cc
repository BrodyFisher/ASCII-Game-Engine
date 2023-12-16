#include "collision.h"

Collision::Collision() {}

passCollision::passCollision() {}

void passCollision::resolveCollision(Entity *e1, Entity *e2) {

    return; // this doesn't need to do anything since they just pass through
}

stopCollision::stopCollision() {}

void stopCollision::resolveCollision(Entity *e1, Entity *e2) {

    e1->revertPrev();
    e2->revertPrev();

    e1->setPrev();
    e2->setPrev();

    e1->clearMovement();
    e2->clearMovement();
}

bounceCollision::bounceCollision(bool y, bool x): y{y}, x{x} {}

void bounceCollision::resolveCollision(Entity *e1, Entity *e2) {

    // flip all the movement of the two entities???
    e1->revertPrev();
    e2->revertPrev();

    e1->setPrev();
    e2->setPrev();

    if (y) {

        e1->flipMovement(true, false);
        e2->flipMovement(true, false);
    } else if (x) {

        e1->flipMovement(false, true);
        e2->flipMovement(false, true);
    }
}

gameEndCollision::gameEndCollision() {}

void gameEndCollision::resolveCollision(Entity *e1, Entity *e2) {

    return;
}

destroyCollision::destroyCollision() {}

void destroyCollision::resolveCollision(Entity *e1, Entity *e2) {

    return; // game will handle this part sorry for the inconsistency
}
