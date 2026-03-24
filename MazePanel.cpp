#include "MazePanel.h"

// Translates the original ASCII maze symbols to integers which acts as keys for selecting a brush color
unsigned char MazePanel::translateColour(const int y, const int x) {
	char c = maze->getCell(y, x);

	switch (c) {
		case 'O':
			return 0;       //wxColour(255, 255, 255);
		case 'X':
			return 1;       //wxColour(197, 15, 31);
		case '@':
			return 2;       //wxColour(0, 55, 218);
		case '*':
			return 3;       //wxColour(193, 156, 0);
		case '!':
			return 4;       //wxColour(58, 150, 221);
		default:
			return 255;     //wxColour(0, 0, 0);
	}
}

// Updates the entire cells matrix to represent the current maze and forces a rerender
void MazePanel::refreshCells() {
	for (int y = 0; height > y; y++) {
		for (int x = 0; width > x; x++) {
			cells[y][x] = translateColour(y, x);
		}
	}

	renderNeeded = true;
	Refresh();
}

// Updates the bitmap which stores the GUI pixel data for the maze
void MazePanel::updateBitmap() {
	wxSize size = GetSize();
	
	int bitmapY = size.GetY();
	int bitmapX = size.GetX();

	int incrementY = (bitmapY / height);
	int incrementX = (bitmapX / width);

	borderLength = 2;

	if (10 > incrementY || 10 > incrementX) {
		borderLength = 1;
	}

	while (bitmapY != incrementY * height + borderLength || bitmapX != incrementX * width + borderLength) {
		bitmapY = incrementY * height + borderLength;
		bitmapX = incrementX * width + borderLength;

		incrementY = (bitmapY / height);
		incrementX = (bitmapX / width);

		if (bitmapY > size.GetY()) {
			incrementY--;
		}

		if (bitmapX > size.GetX()) {
			incrementX--;
		}

		if (10 > incrementY || 10 > incrementX) {
			borderLength = 1;
		}
	}
	
	offsetY = (size.GetY() - bitmapY) / 2;
	offsetX = (size.GetX() - bitmapX) / 2;

	cellHeight = std::max(1, incrementY - borderLength);
	cellWidth = std::max(1, incrementX - borderLength);

	bitmap = wxBitmap(bitmapX, bitmapY);
	wxMemoryDC memory(bitmap);
	std::unique_ptr<wxGraphicsContext> graphics(wxGraphicsContext::Create(memory));

	int yPos;
	int xPos;

	if (graphics) {
		yPos = borderLength;

		for (int y = 0; height > y; y++) {
			xPos = borderLength;

			for (int x = 0; width > x; x++) {
				graphics->SetBrush(brush[cells[y][x]]);
				graphics->DrawRectangle(xPos, yPos, cellWidth, cellHeight);
				xPos += incrementX;
			}

			yPos += incrementY;
		}
	}

	renderNeeded = true;
}

void MazePanel::setupMaze() {
	maze = Maze::init(DEFAULTHEIGHT, DEFAULTWIDTH);

	for (int y = 0; DEFAULTHEIGHT > y; y++) {
		cells.emplace_back();

		for (int x = 0; DEFAULTWIDTH > x; x++) {
			cells[y].push_back(translateColour(y, x));
		}
	}
}

void MazePanel::drawOnMazePanel(const wxPoint& panelPosition) {
	if (offsetY >= panelPosition.y || offsetX >= panelPosition.x) {
		return;
	}

	int y = (panelPosition.y - offsetY) / (cellHeight + borderLength);
	int x = (panelPosition.x - offsetX) / (cellWidth + borderLength);

	if ((y == prevY && x == prevX) || y >= height || x >= width) {
		return;
	}

	prevY = y;
	prevX = x;

	if (startMissing) {
		maze->setStart(y, x);
		startMissing = false;

		refreshCells();
		timer->Stop();
		return;
	}

	if (goalMissing) {
		maze->setGoal(y, x);
		goalMissing = false;

		refreshCells();
		timer->Stop();
		return;
	}

	maze->flipCell(y, x);
	cells[y][x] = translateColour(y, x);

	refreshCells();
}

void MazePanel::onTimer(wxTimerEvent& event) {
	wxPoint screenPosition = wxGetMousePosition();
	wxPoint panelPosition = ScreenToClient(screenPosition);
	drawOnMazePanel(panelPosition);
}

MazePanel::MazePanel(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(25,25)) {
	Bind(wxEVT_PAINT, &MazePanel::onPaint, this);
	Bind(wxEVT_LEFT_DOWN, &MazePanel::onClick, this);
	Bind(wxEVT_LEFT_UP, &MazePanel::onRelease, this);
	Bind(wxEVT_TIMER, &MazePanel::onTimer, this);
	timer = new wxTimer(this);
	setupMaze();

	brush.emplace_back(wxColour(255, 255, 255));
	brush.emplace_back(wxColour(197, 15, 31));
	brush.emplace_back(wxColour(0, 55, 218));
	brush.emplace_back(wxColour(193, 156, 0));
	brush.emplace_back(wxColour(58, 150, 221));

	updateBitmap();
}

MazePanel::MazePanel(wxWindow* parent, int size) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(size, size)) {
	Bind(wxEVT_PAINT, &MazePanel::onPaint, this);
	Bind(wxEVT_LEFT_DOWN, &MazePanel::onClick, this);
	Bind(wxEVT_LEFT_UP, &MazePanel::onRelease, this);
	Bind(wxEVT_TIMER, &MazePanel::onTimer, this);
	setupMaze();

	brush.emplace_back(wxColour(255, 255, 255));
	brush.emplace_back(wxColour(197, 15, 31));
	brush.emplace_back(wxColour(0, 55, 218));
	brush.emplace_back(wxColour(193, 156, 0));
	brush.emplace_back(wxColour(58, 150, 221));

	updateBitmap();
}

MazePanel::~MazePanel() {
	Maze::uninit();
	delete timer;
}

void MazePanel::removeColumn() {
	if (width == maze->getMinWidth()) {
		return;
	}

	maze->removeColumn();

	for (int y = 0; height > y; y++) {
		cells[y].pop_back();
	}

	width--;

	if (renderNeeded) {
		refreshCells();
	}
}

void MazePanel::removeRow() {
	if (height == maze->getMinHeight()) {
		return;
	}

	maze->removeRow();
	cells.pop_back();

	height--;
	
	if (renderNeeded) {
		refreshCells();
	}
}

void MazePanel::removeStart() {
	if (startMissing || goalMissing) {
		return;
	}

	maze->removeStart();
	startMissing = true;
	refreshCells();
}

void MazePanel::removeGoal() {
	if (startMissing || goalMissing) {
		return;
	}

	maze->removeGoal();
	goalMissing = true;
	refreshCells();
}

void MazePanel::addSizer(wxBoxSizer* sizer) {
	this->sizer = sizer;
}

void MazePanel::addColumn() {
	if (width == maze->getMaxWidth()) {
		return;
	}

	maze->addColumn();

	for (int y = 0; height > y; y++) {
		cells[y].push_back(translateColour(y, width));
	}

	width++;

	if (renderNeeded) {
		refreshCells();
	}
}

void MazePanel::addRow() {
	if (height == maze->getMaxHeight()) {
		return;
	}

	maze->addRow();
	cells.emplace_back();

	for (int x = 0; width > x; x++) {
		cells[height].push_back(translateColour(height, x));
	}

	height++;

	if (renderNeeded) {
		refreshCells();
	}
}

void MazePanel::setColumns(int columns) {
	if (columns == width) {
		return;
	}

	renderNeeded = false;

	while (columns > width) {
		addColumn();
	}

	while (width > columns) {
		removeColumn();
	}

	refreshCells();
}

void MazePanel::setRows(int rows) {
	if (rows == height) {
		return;
	}

	renderNeeded = true;

	while (rows > height) {
		addRow();
	}

	while (height > rows) {
		removeRow();
	}

	refreshCells();
}

void MazePanel::run() {
	if (startMissing || goalMissing) {
		return;
	}

	maze->pathfind();
	refreshCells();
}

void MazePanel::randomize() {
	maze->addObstacles(35);
	refreshCells();
}

void MazePanel::clear() {
	maze->clear();
	refreshCells();
}

// Forces a redraw of the maze
void MazePanel::rerender() {
	renderNeeded = true;
	updateBitmap();
}

void MazePanel::onPaint(wxPaintEvent& event) {
	if (renderNeeded) {
		updateBitmap();
	}

	wxPaintDC painter(this);
	painter.DrawBitmap(bitmap, offsetX, offsetY);
}

// Modifies one of the maze's cells when it is clicked. Either the cell flips colors, becomes a start/goal cell, or nothing happens.
void MazePanel::onClick(wxMouseEvent& event) {
	prevY = -1;
	prevX = -1;

	if (!timer->IsRunning()) {
		timer->Start(10);
	}

	if (!HasCapture()) {
		CaptureMouse();
	}

	drawOnMazePanel(event.GetPosition());
}

void MazePanel::onRelease(wxMouseEvent& event) {
	if (timer->IsRunning()) {
		timer->Stop();
	}

	if (HasCapture()) {
		ReleaseMouse();
	}
}

int MazePanel::getMaxHeight() {
	return maze->getMaxHeight();
}

int MazePanel::getMaxWidth() {
	return maze->getMaxWidth();
}

int MazePanel::getMinHeight() {
	return maze->getMinHeight();
}

int MazePanel::getMinWidth() {
	return maze->getMinWidth();
}

int MazePanel::getHeight() {
	return height;
}

int MazePanel::getWidth() {
	return width;
}