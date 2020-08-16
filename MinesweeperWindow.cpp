#include "MinesweeperWindow.h"

MinesweeperWindow::MinesweeperWindow(Point xy, int width, int height, int mines, const string& title) :
	Graph_lib::Window{xy, width * cellSize, height*cellSize, title}, width{width}, height{height}, mines{mines}
{
	// Add all tiles to the window
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			int y = i* cellSize,
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
	// if (tilePressed.getIsMine()) {
		
	// 	return;
	// }

	tilePressed.open();

	vector<Point> neighborTiles = adjacentPoints(xy);
	int neighborMines = countMines(neighborTiles);
	if (neighborMines > 0) {
		tilePressed.setNeighborMines(neighborMines);
	} else {
		for (Point p : neighborTiles) {
			openTile(p);
		}
	}
}

void MinesweeperWindow::flagTile(Point xy) {
	Tile& tilePressed = at(xy);
	tilePressed.flag();
}

//Kaller opentile ved venstreklikk og flagTile ved hoyreklikk/trykke med to fingre paa mac
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
