#pragma once
#include <wx/graphics.h>
#include <wx/wx.h>
#include <chrono>
#include "Maze.h"

class MazePanel : public wxPanel {
	private:
		static const int DEFAULTHEIGHT = 6;
		static const int DEFAULTWIDTH = 15;
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

		std::vector<std::vector<unsigned char>> cells;
		std::vector<wxBrush> brush;
		wxBoxSizer* sizer;
		wxBitmap bitmap;
		Maze* maze;

		unsigned char translateColour(int, int);
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
		void rerender();

		void onClick(wxMouseEvent&);
		void onPaint(wxPaintEvent&);

		int getHeight();
		int getWidth();

};