#include "view.h"


NCurseView::NCurseView() {

    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);
}

NCurseView::~NCurseView() {

    endwin();
}

int NCurseView::getHeight() { return HEIGHT; }

int NCurseView::getWidth() { return WIDTH; }

void NCurseView::update(){ refresh(); }


void NCurseView::displayEntity(Entity *e) {

    charEntity *cep = dynamic_cast<charEntity*>(e); // sorry for the cast
    if (cep) {

        // print the single char
        drawCharEntity(*cep);
    }
    rectEntity *rep = dynamic_cast<rectEntity*>(e);
    if (rep) {

        // print the rectangle
        drawRectEntity(*rep);
    }
    bitmapEntity *bmep = dynamic_cast<bitmapEntity*>(e);
    if (bmep) {

        // print the bitmap
        drawBitMapEntity(*bmep);
    }
}

void NCurseView::deleteLine(int n) { // delete previous line

    move(n, 0);
    clrtoeol();
}

void NCurseView::updateStatus(int statusBar, string msg) {
    
    int line = statusBar % 3;
    deleteLine(HEIGHT + line);

    mvaddstr(HEIGHT + line, 0, msg.c_str());
}

void NCurseView::drawCharEntity(const charEntity &ce) {

    mvwaddch(stdscr, ce.getY(), ce.getX(), ce.getFig());
}

void NCurseView::drawRectEntity(const rectEntity &re) {

    for (int i = re.getY(); i < re.getY() + re.getLength(); ++i) {

        for (int j = re.getX(); j < re.getX() + re.getWidth(); ++j) {

            mvwaddch(stdscr, i, j, re.getChar());
        }
    }
}

void NCurseView::drawBitMapEntity(const bitmapEntity &bme) {

    for (size_t i = 0; i < bme.getPoints(); ++i) {
        
        mvwaddch(stdscr, get<0>(bme.getPointData(i)), get<1>(bme.getPointData(i)), get<char>(bme.getPointData(i)));
    }
}


void NCurseView::drawBoard() {
	// need to have called startScreen
	for (int i = 0; i < HEIGHT; ++i) {

		if (i == 0 || i == HEIGHT - 1) {

			// print +--...--+
			for (int j = 0; j < WIDTH; ++j) {

				move(i, j);
				if (j == 0 || j == WIDTH - 1) {

					addch('+');
				} else {

					addch('-');
				}
			}
		} else {

			mvaddch(i, 0, '|');
			mvaddch(i, WIDTH - 1, '|');
		}
	}
	update();
}
