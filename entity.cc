#include "entity.h"
#include "movement.h"

using namespace std;

Entity::Entity(int id, int height): id{id}, height{height} {}
Entity::~Entity() {

    for (auto p : moveList) {

        free(p);
    }
}

void Entity::clearMovement() { // gets rid if the movement

    for (auto p : moveList) {

        free(p);
    }
    moveList.clear();
}

void Entity::flipMovement(bool y, bool x) {

    for (auto p : moveList) {

        p->flipMove(y, x);
    }
}

// char Entity methods

charEntity::charEntity(int id, int height, int yPosition, int xPosition, char figure): 
            Entity{id, height}, yPosition{yPosition}, xPosition{xPosition}, 
            figure{figure} {}

void charEntity::changeForm(char change) { // change the shape of the entity

    figure = change;
}

int charEntity::getY() const {

    return yPosition;
}

int charEntity::getX() const {

    return xPosition;
}

char charEntity::getFig() const {

    return figure;
}

void charEntity::deletePrev() {

    mvaddch(yPosition, xPosition, ' '); // delete old char
}

void charEntity::setPrev() {

    prevY = yPosition;
    prevX = xPosition;
}

void charEntity::revertPrev() {

    yPosition = prevY;
    xPosition = prevX;
}

vector<pair<int, int>> charEntity::getCoords() {

    vector<pair<int, int>> retval;
    retval.emplace_back(yPosition, xPosition);

    return retval;
}

// rect entity methods

rectEntity::rectEntity(int id, int height, int length, int width, int cornerX, 
                   int cornerY, char fillChar): Entity{id, height}, length{length},
                   width{width}, cornerX{cornerX}, cornerY{cornerY}, fillChar{fillChar} {}

void rectEntity::changeForm(char change) {

    fillChar = change;
}
int rectEntity::getLength() const {

    return length;
}
int rectEntity::getWidth() const {

    return width;
}
int rectEntity::getY() const {

    return cornerY;
}
int rectEntity::getX() const {

    return cornerX;
}
char rectEntity::getChar() const {

    return fillChar;
}
void rectEntity::deletePrev() { // call before you change any coords

    for (int i = cornerY; i < cornerY + length; ++i) {

        for (int j = cornerX; j < cornerX + width; ++j) {

            mvaddch(i, j, ' '); // replace with a bunch of spaces
        }
    }
}

void rectEntity::setPrev() {

    prevYCorner = cornerY;
    prevXCorner = cornerX;
}

void rectEntity::revertPrev() {

    cornerY = prevYCorner;
    cornerX = prevXCorner;
}

vector<pair<int, int>> rectEntity::getCoords() {

    vector<pair<int, int>> retval;

    for (int i = cornerY; i < cornerY + length; ++i) {

        for (int j = cornerX; j < cornerX + width; ++j) {

            retval.emplace_back(i, j);
        }
    }

    return retval;
}

// bitmap entity methods

bitmapEntity::bitmapEntity(int id, int height, vector<tuple<int, int, char>> position):
                            Entity{id, height}, position{position} {}

void bitmapEntity::changeForm(char change) {

    for (size_t i = 0; i < position.size(); ++i) {

        // change later, currently changes all the chars to be the same
        get<char>(position[i]) = change;
    }
}
size_t bitmapEntity::getPoints() const {

    return position.size();
}

tuple<int, int, char> bitmapEntity::getPointData(size_t i) const {

    return position[i];
}

void bitmapEntity::deletePrev() {

    for (size_t i = 0; i < position.size(); ++i) {

        mvaddch(get<0>(position[i]), get<1>(position[i]), ' ');
    }
}

void bitmapEntity::setPrev() {

    prev = position;
}

void bitmapEntity::revertPrev() {

    position = prev;
}

vector<pair<int, int>> bitmapEntity::getCoords() {

    vector<pair<int, int>> retval;
    
    for (size_t i = 0; i < position.size(); ++i) {

        retval.emplace_back(get<0>(position[i]), get<1>(position[i]));
    }

    return retval; 
}
  