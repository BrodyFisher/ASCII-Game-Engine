#ifndef VIEW_H
#define VIEW_H
#include <iostream>
#include <string>
#include "entity.h"


using namespace std;


class View {

	public:
		virtual void update() = 0; // update the screen
		virtual void displayEntity(Entity *e) = 0; // shows entities
		virtual ~View() {}
};

class NCurseView: public View { // this acts as our window that we draw to

    const int HEIGHT = 22;
    const int WIDTH = 80;
    public:
		NCurseView();
        int getHeight();
        int getWidth();
		void drawBoard();
        void update() override;
		void displayEntity(Entity *e) override;
        void updateStatus(int statusBar, string msg);
		~NCurseView();
	private:
		void deleteLine(int n);
		void drawCharEntity(const charEntity &ce);
		void drawRectEntity(const rectEntity &re);
		void drawBitMapEntity(const bitmapEntity &bme);
};
#endif
