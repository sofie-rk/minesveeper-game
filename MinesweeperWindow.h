#pragma once
#include "Window.h"
#include "Tile.h"

using namespace Graph_lib;

enum class MouseButton { left = FL_LEFT_MOUSE, right = FL_RIGHT_MOUSE }; //

// Minesweeper GUI
class MinesweeperWindow : public Graph_lib::Window
{
public:
	static constexpr int cellSize = 30;
	MinesweeperWindow(Point xy, int width, int height, int mines, const string& title);
private:

	const int width;
	const int height;
	const int mines;
	int minesLeft;
	int tilesLeft;

	

	
	Vector_ref<Tile> tiles; 
	

	//Height and Width (pixel)
	int Height() const { return height * cellSize; }
	int Width() const { return width * cellSize; }

	// Faa en liste med naborutene rundt en tile
	vector<Point> adjacentPoints(Point xy) const;
	//tell miner basert paa liste
	int countMines(vector<Point> coords) const;

	//Sjekk at punkt er paa brettet og gjor det til Tile
	bool inRange(Point xy) const { return xy.x >= 0 && xy.x < Width() && xy.y >= 0 && xy.y < Height(); }
	Tile& at(Point xy) { return tiles[xy.x / cellSize + (xy.y / cellSize) * width]; }
	const Tile& at(Point xy) const { return tiles[xy.x / cellSize + (xy.y / cellSize) * width]; }

	//aapne og flagge rute
	void openTile(Point xy);
	void flagTile(Point xy);

	void gameWon();
	void gameLost();

	//callback fuctions tile
	static void cb_click(Address, Address pw);

	static void cb_quit(Address, Address win);
	void quit() {cout << ": quit called\n"; hide();}

	static void cb_restart(Address, Address pw);
	void restart();

	// Text at the top of the window
	Text gameStateText;

	// Restart and quit button
	Button quitBtn;
	Button restartBtn;

	// Game ended
	Rectangle gameEndBackground;
	Text gameEndText;
	
};
