#include "age.h"

using namespace std;

// may or may not use this bad boy
bool vectorContains(vector<int> v, int n) {

    for (auto i : v) if (i == n) return true;
    return false;
}

// village 1 reference!?
bool intersect(const vector<pair<int, int>> &v1, const vector<pair<int, int>> &v2) {

    for (size_t i = 0; i < v1.size(); ++i) {

        for (size_t j = 0; j < v2.size(); ++j) {

            if (v1[i] == v2[j]) return true;
        }
    }

    return false;
}

Game::Game() {

    screen = new NCurseView;
    keyboard = new NCurseController;
    //screen->drawBoard();
}

void Game::go() {

    screen->drawBoard();
    keypress = getControllerInput();
    if (keypress == KEY_BACKSPACE) gameIsOver = true;
    resolveMovement();
    checkBorders(); // may eventually be changed to check collision
    checkCollision();
    notifyView();
}

void Game::checkCharPos(charEntity &ce) {

    int width = screen->getWidth();
    int height = screen->getHeight();

    int x = ce.getX();
    int y = ce.getY();

    if (x <= 0 || x >= width || y <= 0 || y >= height) {

        if (ce.viewBorder) {

            toDelete.push_back(ce.id);
            return;
        } else if (ce.bounceBorder) {

            if (x <= 0 || x >= width) {

                ce.flipMovement(false, true);
            } else if (y <= 0 || y >= height) {

                ce.flipMovement(true, false);
            }
            return;
        }  else {

            ce.revertPrev();
        }
    }
    ce.setPrev();
}


void Game::checkRectPos(rectEntity &re) {

    int width = screen->getWidth();
    int height = screen->getHeight();

    int x = re.getX();
    int y = re.getY();

    if (x <= 0 || x + re.getWidth() >= width || y <= 0 || y + re.getLength() >= height) {

        if (re.viewBorder) {

            toDelete.push_back(re.id);
            return;
        } else if (re.bounceBorder) {

            if(x <= 0 || x + re.getWidth() >= width) {

                re.flipMovement(false, true);
            } else if (y <= 0 || y + re.getLength() >= height) {

                re.flipMovement(true, false);
            }
            return;
        }  else {

            re.revertPrev();
        }
    }
    re.setPrev();
}


void Game::checkBitmapPos(bitmapEntity &bme) {

    int width = screen->getWidth();
    int height = screen->getHeight();

    for (size_t i = 0; i < bme.getPoints(); ++i) {

        int y = get<0>(bme.getPointData(i));
        int x = get<1>(bme.getPointData(i));

        if (x <= 0 || x + 1 >= width || y <= 0 || y + 1 >= height) {

            if (bme.viewBorder) {

                toDelete.push_back(bme.id);
                return;
            } else if (bme.bounceBorder) {

                if (x <= 0 || x + 1 >= width) {

                    bme.flipMovement(false, true);
                } else if (y <= 0 || y + 1 >= height) {

                    bme.flipMovement(true, false);
                }
                return;
            } else {

                bme.revertPrev();
                break;
            }
        }
    }
    bme.setPrev();
}

void Game::checkCollision() {

   
    for (auto i : entities) {

        for (auto j : entities) {

            if (i.second == j.second) continue;
            else if (i.second->height != j.second->height) continue;

            if (intersect(i.second->getCoords(), j.second->getCoords())) {

                if (collisionData.find(make_pair(i.second->id, j.second->id)) != collisionData.end()) {

                    if (dynamic_cast<gameEndCollision*>(collisionData[make_pair(i.second->id, j.second->id)])) gameIsOver = true;
                    else if (dynamic_cast<destroyCollision*>(collisionData[make_pair(i.second->id, j.second->id)])) {

                        if (!i.second->invincible) {

                            toDelete.push_back(i.second->id);
                        }
                        if (!j.second->invincible) {

                            toDelete.push_back(j.second->id);
                        }
                    }
                    collisionData[make_pair(i.second->id, j.second->id)]->resolveCollision(i.second, j.second);
                } 
            }
        }
    }
    for (size_t i = 0; i < toDelete.size(); ++i) {

        removeEntity(toDelete[i]);
    }
    toDelete.clear();
}

// delete element by id
void Game::removeEntity(int id) {

    auto it = entities.find(id);
    if (it != entities.end()) {

        entities[id]->deletePrev();
        delete entities[id];
        entities.erase(it);
    }
}

void Game::setBorderView(int id) {

    if (entities.find(id) != entities.end()) {

        entities[id]->viewBorder = true;
    }
}

void Game::checkBorders() {

    // need to see if stuff is outside the borders
    // this will unfortunately have casting since the check is different for each
    // type of entity

    Entity* ep;

    for (auto &i : entities) {

        ep = i.second; // get the pointer

        charEntity* cep = dynamic_cast<charEntity*>(ep);

        if (cep) {
        
            checkCharPos(*cep);
            continue;
        }

        rectEntity* rep = dynamic_cast<rectEntity*>(ep);

        if (rep) {

            checkRectPos(*rep);
            continue;
        }

        bitmapEntity* bmep = dynamic_cast<bitmapEntity*>(ep);

        if (bmep) {

            checkBitmapPos(*bmep);
            continue;
        }
    }

    for (size_t i = 0; i < toDelete.size(); ++i) {

        removeEntity(toDelete[i]);
    }
    toDelete.clear();
}

void Game::notifyView() {

    for (auto i : entities) {

        screen->displayEntity(i.second);
    }
    screen->update();
}

int Game::getControllerInput() {

    keyboard->getInput();
    return keyboard->getOutput();
}

void Game::make_char_entity(int id, int height, int y, int x, char figure) {

    entities[id] = (new charEntity{id, height, y + 1, x + 1, figure});
}

void Game::make_rect_entity(int id, int height, int length, int width, int cornerX, int cornerY, char fillChar) {

    entities[id] = (new rectEntity{id, height, length, width, cornerX + 1, cornerY + 1, fillChar});    
}

void Game::make_bitmap_entity(int id, int height, vector<tuple<int, int, char>> position) {

    entities[id] = (new bitmapEntity{id, height, position});
}

void Game::movePlayer(charEntity &ce, int dy, int dx) {

    ce.deletePrev();

    ce.yPosition += dy;
    ce.xPosition += dx;
}

void Game::movePlayer(rectEntity &re, int dy, int dx) {

    re.deletePrev();

    re.cornerY += dy;
    re.cornerX += dx;
}

void Game::movePlayer(bitmapEntity &bme, int dy, int dx) {

    bme.deletePrev();
    
    for (size_t i = 0; i < bme.position.size(); ++i) {

        get<0>(bme.position[i]) += dy;
        get<1>(bme.position[i]) += dx;
    }
}

void Game::resolvePlayerMovement() {
    
    int dx, dy;

    if (keypress == KEY_LEFT) {

        dx = -1;
        dy = 0;
    } else if (keypress == KEY_RIGHT) {

        dx = 1;
        dy = 0;
    } else if (keypress == KEY_UP) {

        dx = 0;
        dy = -1;
    } else if (keypress == KEY_DOWN) {

        dx = 0;
        dy = 1;
    }  

    int id;
    Entity * ep;
    for (size_t i = 0; i < players.size(); ++i) {
        
        id = get<0>(players[i]); // id of the entity we are accessing
        ep = entities[id];

        dy *= get<1>(players[i]);
        dx *= get<1>(players[i]);  

        charEntity* cep = dynamic_cast<charEntity*>(ep);

        if (cep) {
        
            movePlayer(*cep, dy, dx);
        }

        rectEntity* rep = dynamic_cast<rectEntity*>(ep);

        if (rep) {

            movePlayer(*rep, dy, dx);
        }

        bitmapEntity* bmep = dynamic_cast<bitmapEntity*>(ep);

        if (bmep) {

            movePlayer(*bmep, dy, dx);
        }
    }
}

void Game::resolveMovement() {

    for (auto &i : entities) {

        for (size_t j = 0; j < i.second->moveList.size(); ++j) {

            i.second->moveList[j]->pos(i.second);
        }
    }
    mvaddstr(0, 81, " "); // don't hate me but this makes it work somehow
    resolvePlayerMovement();
    keypress = 0;
}

void Game::addLineMovement(int id, int dy, int dx) {

    entities[id]->moveList.push_back(new lineMovement{1, dy, dx});
}

void Game::addGravitateMovement(int id, int speed, int border) {

    entities[id]->moveList.push_back(new gravitateMovement{speed, border});
}

void Game::addinplaceMovement(int id, vector<char> sequence) {

    entities[id]->moveList.push_back(new inplaceMovement{1, sequence});
}

void Game::addPlayerMovement(int id, int speed) {
    
    players.push_back(make_tuple(id, speed)); // keeping track of player entities
}

void Game::addBounceCollision(int id1, int id2, bool y, bool x) {

    collisionData[make_pair(id1, id2)] = new bounceCollision{y, x};
}

void Game::addPassCollision(int id1, int id2) {

    collisionData[make_pair(id1, id2)] = new passCollision;
}

void Game::addStopCollision(int id1, int id2) {

    collisionData[make_pair(id1, id2)] = new stopCollision;
}

void Game::addGameEndCollision(int id1, int id2) {

    collisionData[make_pair(id1, id2)] = new gameEndCollision;
}

void Game::addDestroyCollision(int id1, int id2) {

    collisionData[make_pair(id1, id2)] = new destroyCollision;
}

bool Game::gameOver() const {

    return gameIsOver;
}

bool Game::checkEntity(int n) const {

    if (entities.find(n) != entities.end()) return true;
    else return false;
}

void Game::changeVulnerability(int id) {

    if (entities.find(id) != entities.end()) {

        entities[id]->invincible = !entities[id]->invincible;
    }
}

void Game::changeBounce(int id) {

    if (entities.find(id) != entities.end()) {

        entities[id]->bounceBorder = !entities[id]->bounceBorder;
    }
}

int Game::getNumEntities() const {

    return entities.size();
}

Game::~Game() {

    for (map<int, Entity *>::iterator iter = entities.begin(); iter != entities.end(); ++iter) {

        delete iter->second;
    }
    for (map<pair<int, int>, Collision*>::iterator iter = collisionData.begin(); iter != collisionData.end(); ++iter) {

        free(iter->second);
    }
    delete screen;
    delete keyboard;
}
