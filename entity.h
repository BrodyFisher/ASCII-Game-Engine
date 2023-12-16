#ifndef ENTITY_H
#define ENTITY_H
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <ncurses.h>

using namespace std;

class Movement;

class Entity {

    int id, height;
    bool viewBorder = false;
    bool invincible = false;
    bool bounceBorder = false;
    vector<Movement*> moveList;
    public:
        friend class stopCollsion;
        friend class Game;
        Entity(int id, int height);
        virtual void changeForm(char change) = 0;
        virtual ~Entity();
        void clearMovement();
        void flipMovement(bool y, bool x);
        virtual void setPrev() = 0;
        virtual void revertPrev() = 0;
        virtual vector<pair<int, int>> getCoords() = 0;
    private:
        virtual void deletePrev() = 0;
};

class charEntity: public Entity {

    int yPosition, xPosition;
    char figure; // what will be printed on the screen

    int prevY;
    int prevX;

    charEntity(int id, int height, int yPosition, int xPosition, char figure);
    public:
    friend class Game;
    friend class lineMovement;
    friend class gravitateMovement;
    friend class playerMovement;
    friend class stopCollision;
    friend class inplaceCollision;
        void changeForm(char change) override;
        int getY() const;
        int getX() const;
        char getFig() const;
        void setPrev() override;
        void revertPrev() override;
        vector<pair<int, int>> getCoords() override;
    private:
        void deletePrev() override;
        
};

class rectEntity: public Entity {

    int length, width, cornerX, cornerY;

    int prevYCorner;
    int prevXCorner;

    char fillChar;
    rectEntity(int id, int height, int length, int width, int cornerX, int cornerY, char fillChar);
    public:
    friend class Game;
    friend class lineMovement;
    friend class gravitateMovement;
    friend class playerMovement;
    friend class stopCollision;
    friend class inplaceCollision;
        void changeForm(char change) override;
        int getLength() const;
        int getWidth() const;
        int getY() const;
        int getX() const;
        char getChar() const;
        void setPrev() override; // before a change
        void revertPrev() override; // when there is a collision
        vector<pair<int, int>> getCoords() override;
    private:
        void deletePrev() override;
        
};

class bitmapEntity: public Entity {

    vector<tuple<int, int, char>> position;
    vector<tuple<int, int, char>> prev;

    bitmapEntity(int id, int height, vector<tuple<int, int, char>> position);
    public:
        friend class Game;
        friend class lineMovement;
        friend class gravitateMovement;
        friend class playerMovement;
        friend class stopCollision;
        friend class inplaceCollision;
        void changeForm(char change) override;
        size_t getPoints() const;
        tuple<int, int, char> getPointData(size_t i) const;
        void setPrev() override; // called right before a change
        void revertPrev() override; // called when there is a collision
        vector<pair<int, int>> getCoords() override;
    private:
        void deletePrev() override;  
};
#endif
 