#pragma once
#include <wx/graphics.h>
#include <wx/wx.h>
#include <chrono>
#include <memory>
#include "Maze.h"

class MazePanel : public wxPanel {
	private:
		static const int DEFAULTHEIGHT = 15;
		static const int DEFAULTWIDTH = 35;
		static const int MAXHEIGHT = 150;
		static const int MAXWIDTH = 255;
		static const int MINHEIGHT = 3;
		static const int MINWIDTH = 3;
		static const int BORDERLENGTH = 2;
		static const int MINCELLSIZE = 4;
		static const int CELLSIZE = 15;

		int height = DEFAULTHEIGHT;
		int width = DEFAULTWIDTH;
		bool renderNeeded = false;
		bool startMissing = false;
		bool goalMissing = false;
		int cellHeight;
		int cellWidth;
		int prevY;
		int prevX;

		std::vector<std::vector<unsigned char>> cells;
		std::vector<wxBrush> brush;
		wxBoxSizer* sizer;
		wxBitmap bitmap;
		wxTimer* timer;
		Maze* maze;

		unsigned char translateColour(const int, const int);
		void drawOnMazePanel(const wxPoint&);
		void onTimer(wxTimerEvent&);
		void refreshCells();
		void updateBitmap();
		void setupMaze();

	public:
		MazePanel(wxWindow*);
		MazePanel(wxWindow*, int);
		~MazePanel();

		void removeColumn();
		void removeRow();
		void removeStart();
		void removeGoal();

		void addSizer(wxBoxSizer*);
		void addColumn();
		void addRow();

		void run();
		void randomize();
		void clear();
		void rerender();

		void onPaint(wxPaintEvent&);
		void onClick(wxMouseEvent&);
		void onRelease(wxMouseEvent&);

		int getHeight();
		int getWidth();

};