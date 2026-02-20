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
	this->Refresh();
}

// Updates the bitmap which stores the GUI pixel data for the maze
void MazePanel::updateBitmap() {
	wxSize size = this->GetSize();
	
	const int panelY = size.GetY();
	const int panelX = size.GetX();

	const int incrementY = (panelY / height);
	const int incrementX = (panelX / width);

	if (panelY != incrementY * height + BORDERLENGTH || panelX != incrementX * width + BORDERLENGTH) {
		this->SetSize(incrementX * width + BORDERLENGTH, incrementY * height + BORDERLENGTH);
		updateBitmap();
		return;
	}

	cellHeight = incrementY - BORDERLENGTH;
	cellWidth = incrementX - BORDERLENGTH;

	int yPos;
	int xPos;

	bitmap = wxBitmap(panelX, panelY);
	wxMemoryDC memory(bitmap);
	std::unique_ptr<wxGraphicsContext> graphics(wxGraphicsContext::Create(memory));

	if (graphics) {
		yPos = BORDERLENGTH;

		for (int y = 0; height > y; y++) {
			xPos = BORDERLENGTH;

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
	if (0 >= panelPosition.y || 0 >= panelPosition.x) {
		return;
	}

	int y = panelPosition.y / (cellHeight + BORDERLENGTH);
	int x = panelPosition.x / (cellWidth + BORDERLENGTH);

	if (y >= height || x >= width || (y == prevY && x == prevX)) {
		return;
	}

	maze->removePath();
	prevY = y;
	prevX = x;

	if (startMissing) {
		maze->setStart(y, x);
		startMissing = false;

		refreshCells();
		timer->Stop();

		this->Refresh();
		return;
	}

	if (goalMissing) {
		maze->setGoal(y, x);
		goalMissing = false;

		refreshCells();
		timer->Stop();

		this->Refresh();
		return;
	}

	maze->flipCell(y, x);
	cells[y][x] = translateColour(y, x);

	refreshCells();
	this->Refresh();
}

void MazePanel::onTimer(wxTimerEvent& event) {
	wxPoint screenPosition = wxGetMousePosition();
	wxPoint panelPosition = ScreenToClient(screenPosition);
	drawOnMazePanel(panelPosition);
}

MazePanel::MazePanel(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(25,25)) {
	this->Bind(wxEVT_PAINT, &MazePanel::onPaint, this);
	this->Bind(wxEVT_LEFT_DOWN, &MazePanel::onClick, this);
	this->Bind(wxEVT_LEFT_UP, &MazePanel::onRelease, this);
	this->Bind(wxEVT_TIMER, &MazePanel::onTimer, this);
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
	this->Bind(wxEVT_PAINT, &MazePanel::onPaint, this);
	this->Bind(wxEVT_LEFT_DOWN, &MazePanel::onClick, this);
	this->Bind(wxEVT_LEFT_UP, &MazePanel::onRelease, this);
	this->Bind(wxEVT_TIMER, &MazePanel::onTimer, this);
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
}

void MazePanel::removeColumn() {
	if (width == MINWIDTH) {
		return;
	}

	maze->removeColumn();

	for (int y = 0; height > y; y++) {
		cells[y].pop_back();
	}

	width--;
	refreshCells();
}

void MazePanel::removeRow() {
	if (height == MINHEIGHT) {
		return;
	}

	maze->removeRow();
	cells.pop_back();

	height--;
	refreshCells();
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
	if (width == MAXWIDTH) {
		return;
	}

	maze->addColumn();

	for (int y = 0; height > y; y++) {
		cells[y].push_back(translateColour(y, width));
	}

	width++;
	refreshCells();
}

void MazePanel::addRow() {
	if (height == MAXHEIGHT) {
		return;
	}

	maze->addRow();
	cells.emplace_back();

	for (int x = 0; width > x; x++) {
		cells[height].push_back(translateColour(height, x));
	}

	height++;
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

// Forces a redraw of the maze
void MazePanel::rerender() {
	renderNeeded = true;
	updateBitmap();
}

void MazePanel::onPaint(wxPaintEvent& event) {
	if (renderNeeded) {
		updateBitmap();
	}

	wxSize size = this->GetSize();

	const int panelY = size.GetY();
	const int panelX = size.GetX();

	wxPaintDC painter(this);
	painter.DrawBitmap(bitmap, 0, 0);
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

int MazePanel::getHeight() {
	return height;
}

int MazePanel::getWidth() {
	return width;
}