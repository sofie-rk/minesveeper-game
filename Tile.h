#pragma once
#include "GUI.h"

using namespace Graph_lib;

// Tile's three states
enum class Cell { closed, open, flagged };

// Tile is inherited from Button
class Tile : public Button
{
	Cell state = Cell::closed;
	bool isMine;

	void set_label(string s) { label = s; redraw(); }
	void set_label_color(Color c) { pw->labelcolor(c.as_int()); redraw();}
public:
	Tile(Point pos, int size, Graph_lib::Callback cb, bool isMine=false)
		:Button{pos, size, size, "", cb}, isMine{isMine} {};
	
	Cell getState() const {return state; }
	

	bool getIsMine() const {return isMine; }
	void setIsMine(bool mineState) {this->isMine = mineState;}

	void open(); 
	void flag();

	void setNeighborMines(int n); 

	void reset();
};