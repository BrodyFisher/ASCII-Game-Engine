#include "movement.h"

using namespace std;

Movement::Movement(int speed): speed{speed} {}

void Movement::pos(Entity* ep) { // so sorry

    charEntity* cep = dynamic_cast<charEntity*>(ep);

    if (cep) {
    
        updatePosition(*cep);
    }

    rectEntity* rep = dynamic_cast<rectEntity*>(ep);

    if (rep) {

        updatePosition(*rep);
    }

    bitmapEntity* bmep = dynamic_cast<bitmapEntity*>(ep);

    if (bmep) {

        updatePosition(*bmep);
    }
}

int Movement::getSpeed() const {

    return speed;
}

// line movement methods

lineMovement::lineMovement(int speed, int dy, int dx):Movement{speed}, dy{dy}, dx{dx} {}

void lineMovement::updatePosition(charEntity &ce) {

    ce.deletePrev();
    ce.yPosition += dy;
    ce.xPosition += dx;
}

void lineMovement::updatePosition(rectEntity &re) {

    re.deletePrev();
    re.cornerY += dy;
    re.cornerX += dx;
}

void lineMovement::updatePosition(bitmapEntity &bme) {

    bme.deletePrev();

    for (size_t i = 0; i < bme.position.size(); ++i) {

        get<0>(bme.position[i]) += dy;
        get<1>(bme.position[i]) += dx;
    }
}

void lineMovement::flipMove(bool y, bool x) {

    if (y) dy *= -1;
    if (x) dx *= -1;
}

// grav movement methods

gravitateMovement::gravitateMovement(int speed, int border): Movement{speed}, border{border} {}

void gravitateMovement::updatePosition(charEntity &ce) {

    ce.deletePrev();

    if (border == TOP) {

        ce.yPosition -= getSpeed();
    } else if (border == RIGHT) {

        ce.xPosition += getSpeed();
    } else if (border == BOTTOM) {

        ce.yPosition += getSpeed();
    } else if (border == LEFT) {

        ce.xPosition -= getSpeed();
    }
}

void gravitateMovement::updatePosition(rectEntity &re) {

    re.deletePrev();

    if (border == TOP) {

        re.cornerY -= getSpeed();
    } else if (border == RIGHT) {

        re.cornerX += getSpeed();
    } else if (border == BOTTOM) {

        re.cornerY += getSpeed();
    } else if (border == LEFT) {

        re.cornerX -= getSpeed();
    }
}

void gravitateMovement::updatePosition(bitmapEntity &bme) {

    bme.deletePrev();
    int changeX = 0, changeY = 0;

    if (border == TOP) {

        changeY = -1;
    } else if (border == RIGHT) {

        changeX = 1;
    } else if (border == BOTTOM) {

        changeY = 1;
    } else if (border == LEFT) {

        changeX = -1;
    }

    for (size_t i = 0; i < bme.getPoints(); ++i) {

        get<0>(bme.position[i]) += changeY;
        get<1>(bme.position[i]) += changeX;
    }
}

void gravitateMovement::flipMove(bool y, bool x) {

    if (border == TOP) border = BOTTOM;
    else if (border == RIGHT) border = LEFT;
    else if (border == BOTTOM) border = TOP;
    else if (border == LEFT) border = RIGHT;
}

// in place movement methods

inplaceMovement::inplaceMovement(int speed, vector<char> sequence): Movement{speed}, sequence{sequence} {

    size = sequence.size();
}

void inplaceMovement::updatePosition(charEntity &ce){
    
    if (size > 0) {

        ce.changeForm(sequence[index++]);
        if (index >= sequence.size()) index = 0;
    }
}
void inplaceMovement::updatePosition(rectEntity &re) {
    
    if (size > 0) {

        re.changeForm(sequence[index++]);
        if (index >= sequence.size()) index = 0;
    }
}
void inplaceMovement::updatePosition(bitmapEntity &bme) {
    
    if (size > 0) {

        bme.changeForm(sequence[index++]);
        if (index >= sequence.size()) index = 0;
    }
}

void inplaceMovement::flipMove(bool y, bool x){
    
    return; // no need
}
