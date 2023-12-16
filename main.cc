#include <iostream>
#include "age.h"
#include <string>
#include <time.h>

using namespace std;

const vector<tuple<int, int, char>> bird = 
{
   {10, 10, '('},
   {10, 11, '\''},
   {10, 12, 'v'},
   {10, 13, '\''},
   {10, 14, ')'},
   {11, 9,  ')'},
   {11, 10, '('},
   {11, 11, '('},
   {11, 13, ')'},
   {11, 14, ')'},
   {11, 15, '('},
   {12, 11, '^' },
   {12, 13, '^'}
};
void flappyBird() {

   srand(time(NULL));

   Game g;
   g.make_bitmap_entity(1, 1, bird);

   int pipeHeight = 1 + (rand() % 10);

   g.make_rect_entity(4, 1, pipeHeight, 2, 75, 0, '*');
   g.make_rect_entity(5, 1, 12 - pipeHeight, 2, 75, pipeHeight + 8, '*');
   
   g.addLineMovement(4, 0, -1);
   g.addLineMovement(5, 0, -1);

   g.addPlayerMovement(1, 6);
   g.addLineMovement(1, 1, 0);


   g.addGameEndCollision(1, 4);
   g.addGameEndCollision(1, 5);
   g.setBorderView(4);
   g.setBorderView(5);

   int score = 0;
   while(!g.gameOver()) {
      
      g.displayStatus(0, "Score: ", score);

      if (!g.checkEntity(4)) {

         pipeHeight = 1 + (rand() % 10);

         g.make_rect_entity(4, 1, pipeHeight, 2, 75, 0, '*');
         g.make_rect_entity(5, 1, 12 - pipeHeight, 2, 75, pipeHeight + 8, '*');  

         g.addLineMovement(4, 0, -1);
         g.addLineMovement(5, 0, -1);

         g.setBorderView(4);
         g.setBorderView(5);
         ++score;
      }
      g.go();
   }
}

void breakout() {

   srand(time(NULL));
   Game g;

   g.make_rect_entity(1, 1, 1, 20, 30, 18, '-'); // cotrollable
   g.make_rect_entity(2, 1, 2, 2, 15 + rand() % 30 + 1, 5, '0'); // ball
   
   g.changeVulnerability(2);

   for (int i = 3; i < 10; ++i) {

      g.make_rect_entity(i, 1, 2, 3, 0 + 6 * i, 3, '&');
      g.addinplaceMovement(i, {'#', '*', '+'});
   }
   for(int i = 3; i < 10; ++i) {

      g.addDestroyCollision(2, i);
   }
   
   g.make_rect_entity(3, 1, 1, 78, 0, 19, '^'); // spikes at the bottom

   // movements
   g.addPlayerMovement(1, 4);
   g.addLineMovement(2, 1, rand() % 2 + 1);

   // collisions
   g.addGameEndCollision(2, 3);
   g.addBounceCollision(1, 2, true, false);
   g.changeBounce(2);
   
   while(!g.gameOver()) {

      g.displayStatus(0, g.getNumEntities() - 3, " more to go!");
      if (g.getNumEntities() == 3) break;
      g.go();
   }
}

int main(int argc, char *argv[]) {

   if (argc == 1 || string(argv[1]) == "-f") { // for flappy bird

      flappyBird();
   } else if (string(argv[1]) == "-b") { // for breakout

      breakout();
   }
}
