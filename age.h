#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include "model.h"
#include "collision.h"
#include "view.h"
#include "controller.h"
#include "movement.h"

using namespace std;

class Game: public Model {

    map<int, Entity*> entities;
	NCurseView *screen;
    NCurseController *keyboard;
    // int speed
    vector<tuple<int, int>> players; // tracks the ids of a movable player entity
    map<pair<int, int>, Collision *> collisionData;
    stringstream statusLine;
    vector<int> toDelete;
    bool gameIsOver = false;
    int keypress;
	public:
        Game();
        void go();
        void notifyView() override;
        int getControllerInput() override;
        void make_char_entity(int id, int height, int y, int x, char figure);
        void make_rect_entity(int id, int height, int length, int width, int cornerX, int cornerY, char fillChar);
        void make_bitmap_entity(int id, int height, vector<tuple<int, int, char>> position);
        ~Game();
        void addLineMovement(int id, int dy, int dx);
        void addGravitateMovement(int id, int speed, int border);
        void addPlayerMovement(int id, int speed);
        void addinplaceMovement(int id, vector<char> sequence);

        void addBounceCollision(int id1, int id2, bool y, bool x);
        void addStopCollision(int id1, int id2);
        void addPassCollision(int id1, int id2);
        void addGameEndCollision(int id1, int id2);
        void addDestroyCollision(int id1, int id2);

        void setBorderView(int id);
        void removeEntity(int id);
        void changeVulnerability(int id);
        void changeBounce(int id);

        bool checkEntity(int n) const;
        void displayStatus(int n) {
            string passVal;
            getline(statusLine, passVal);
            screen->updateStatus(n, passVal);
            statusLine.clear();
        }
        // since I know I'm a racket programmer at heart...
        template<typename T, typename... Args> void displayStatus(int n, T first, Args... rest) {
            statusLine << first;
            displayStatus(n, rest...);
        }
        bool gameOver() const;
        int getNumEntities() const;
     private:
        void checkCollision();
        void resolveMovement();
        void resolvePlayerMovement();
        void checkCharPos(charEntity &ce);
        void checkRectPos(rectEntity &re);
        void checkBitmapPos(bitmapEntity &bme);
        void checkBorders();
        void movePlayer(charEntity &ce, int dy, int dx);
        void movePlayer(rectEntity &re, int dy, int dx);
        void movePlayer(bitmapEntity &bme, int dy, int dx);
};
