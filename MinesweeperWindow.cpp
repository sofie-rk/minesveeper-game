#include "MinesweeperWindow.h"

MinesweeperWindow::MinesweeperWindow(Point xy, int width, int height, int mines, const string& title) :
	Graph_lib::Window{xy, width * cellSize, (height+3)*cellSize, title}, width{width}, height{height}, mines{mines},
	minesLeft{mines}, tilesLeft{height*width - mines},
	gameStateText{{cellSize / 4, height*cellSize + 20}, "Mines left: " + to_string(mines)},
	quitBtn{{0, (height+2)*cellSize}, Width() / 2, cellSize, "Quit", cb_quit},
	restartBtn{{Width() / 2, (height+2)*cellSize}, Width() / 2, cellSize, "Restart", cb_restart},
	gameEndBackground{{0, (height+1)*cellSize}, Width(), cellSize},
	gameEndText{{0, (height+1)*cellSize + 15}, "End text"}
{
	// Add all tiles to the window
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			int y = i * cellSize,
				x = j * cellSize;
			tiles.push_back(new Tile{ Point{x, y}, cellSize, cb_click });
			attach(tiles.back());
		}
	}

	// Add mines to the table
	int i = 0;
	while (i < mines) {
		int pos = rand() % tiles.size(); // random index
		if (!tiles[pos].getIsMine()) { // if not already a mine
			tiles[pos].setIsMine(true);
			++i;
		}
	}

	// Text at the top of the window
	gameStateText.set_color(Color::black);
	gameStateText.set_font_size(20);
	attach(gameStateText);


	// Remove window rescale
	resizable(nullptr);
	size_range(x_max(), y_max(), x_max(), y_max());
}

int MinesweeperWindow::countMines(vector<Point> points) const {
	int count = 0;
	for (const Point p : points) {
		if (at(p).getIsMine()) {++count;}
	}
	return count;
};

vector<Point> MinesweeperWindow::adjacentPoints(Point xy) const {
	vector<Point> points;
	for (int di = -1; di <= 1; ++di) {
		for (int dj = -1; dj <= 1; ++dj) {
			if (di == 0 && dj == 0) {
				continue;
			}

			Point neighbour{ xy.x + di * cellSize,xy.y + dj * cellSize };
			if (inRange(neighbour)) {
				points.push_back(neighbour);
			}
		}
	}

	return points;
}

void MinesweeperWindow::openTile(Point xy) {
	Tile& tilePressed = at(xy);
	
	// Nothing happens if tile is not closed
	if (tilePressed.getState() != Cell::closed) {
		return;
	}

	// Game over if mine is pressed
	if (tilePressed.getIsMine()) {
		gameLost();
		return;
	}

	tilePressed.open();
	tilesLeft--;

	vector<Point> neighborTiles = adjacentPoints(xy);
	int neighborMines = countMines(neighborTiles);
	if (neighborMines > 0) {
		tilePressed.setNeighborMines(neighborMines);
	} else {
		for (Point p : neighborTiles) {
			openTile(p);
		}
	}

	if (tilesLeft == 0) {
		gameWon();
		return;
	}


}

void MinesweeperWindow::flagTile(Point xy) {
	Tile& tilePressed = at(xy);
	tilePressed.flag();

	if (tilePressed.getState() == Cell::closed) {
		minesLeft++;
	} else if (tilePressed.getState() == Cell::flagged) {
		minesLeft--;
	}

	redraw();
	gameStateText.set_label("Mines left: " + to_string(minesLeft));

}

void MinesweeperWindow::gameWon() 
{
	// When game is won, all tiles with mines are flagged
	for (int i = 0; i < tiles.size(); i++) {
		if (tiles[i].getState() == Cell::closed && tiles[i].getIsMine()) {
			tiles[i].flag();
		}
	}
	redraw();
	gameEndBackground.set_fill_color(Color::green);
	gameEndBackground.set_color(Color::green);
	gameEndText.set_label("You won!");
	gameEndText.set_color(Color::black);
	gameEndText.set_font_size(15);
	attach(gameEndBackground);
	attach(gameEndText);
	attach(quitBtn);
	attach(restartBtn);


}
void MinesweeperWindow::gameLost() 
{
	// When game is lost, all tiles with mines open
	for (int i = 0; i < tiles.size(); i++) {
		if (tiles[i].getState() == Cell::closed && tiles[i].getIsMine()) {
			tiles[i].open();
		} else if (tiles[i].getState() == Cell::flagged) {
			tiles[i].flag();
			tiles[i].open();
		}
	}
	redraw();
	gameEndBackground.set_fill_color(Color::red);
	gameEndBackground.set_color(Color::red);
	gameEndText.set_label("You lost");
	gameEndText.set_color(Color::black);
	gameEndText.set_font_size(15);
	attach(gameEndBackground);
	attach(gameEndText);
	attach(quitBtn);
	attach(restartBtn);
}


// Left click: openTile, right click: flagTile
void MinesweeperWindow::cb_click(Address, Address pw)
{
	Point xy{ Fl::event_x(),Fl::event_y() };
	MouseButton mb = static_cast<MouseButton>(Fl::event_button());
	auto& win = reference_to<MinesweeperWindow>(pw);

	if (!win.inRange(xy)) {
		return;
	}

	switch (mb) {
	case MouseButton::left:
		win.openTile(xy);
		break;
	case MouseButton::right:
		win.flagTile(xy);
		break;
	}
	win.flush();
}

void MinesweeperWindow::cb_quit(Address, Address win)
{
	reference_to<MinesweeperWindow>(win).quit();
}

void MinesweeperWindow::cb_restart(Address, Address win)
{
	reference_to<MinesweeperWindow>(win).restart();
    reference_to<MinesweeperWindow>(win).flush();
    MinesweeperWindow{Point{200,300},10,10,10, "Minesweeper"};
}

void MinesweeperWindow::restart() 
{
	// Reset all tiles on the table
	for (int i = 0; i < tiles.size(); i++) {
		tiles[i].reset(); // close cell, remove symbol, remove mine, unclicked
		tiles[i].redraw();
	}

	// Place out new mines
	int i = 0;
	while (i < mines) {
		int pos = rand() % tiles.size(); // random index
		if (!tiles[pos].getIsMine()) { // if not already a mine
			tiles[pos].setIsMine(true);
			++i;
		}
	}

	// Remove game-end text, background, buttons
	detach(gameEndText);
	detach(gameEndBackground);
	detach(quitBtn);
	detach(restartBtn);

	// Reset variables
	minesLeft = mines;
	gameStateText.set_label("Mines left: " + to_string(mines));
	tilesLeft = height*width - mines;
	

}