#include "Tile.h"

#include <string>
// Color according to the number of mines around
const map<int, Color> minesToColor{ {1, Color::blue},
								   {2, Color::red},
								   {3,Color::dark_green},
								   {4, Color::dark_magenta},
								   {5, Color::dark_blue},
								   {6, Color::dark_cyan},
								   {7, Color::dark_red},
								   {8, Color::dark_yellow} };

// Symbol according to state
const map<Cell, string> cellToSymbol{ {Cell::closed, ""},
									 {Cell::open, ""},
									 {Cell::flagged, "@<"} };

void Tile::open()
{
	if (state != Cell::flagged) {
		static_cast<Fl_Button*>(pw)->set(); // open tile
		state = Cell::open;
		
		if (isMine) {
			set_label("X");
			set_label_color(Color::red);
			redraw();
		}
	}

}

void Tile::flag()
{
	if (state == Cell::flagged) {
		state = Cell::closed;
		set_label(cellToSymbol.at(Cell::closed));
	} else {
		state = Cell::flagged;
		set_label(cellToSymbol.at(Cell::flagged));
		set_label_color(Color::blue);
	}
}

void Tile::setNeighborMines(int n)
{
	set_label(to_string(n));
	set_label_color(minesToColor.at(n));
}

void Tile::reset()
{
	state = Cell::closed;
	set_label(cellToSymbol.at(Cell::closed));
	static_cast<Fl_Button*>(pw)->clear(); // button which is not pressed
	isMine = false;
}