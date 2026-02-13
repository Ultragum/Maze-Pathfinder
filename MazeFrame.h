#pragma once
#include <wx/wx.h>
#include <thread>
#include <chrono>
#include <random>
#include <functional>
#include "MazePanel.h"

class MazeFrame : public wxFrame {
	private:
		bool placeStart;
		bool placeGoal;

		wxPanel* mainPanel;
		wxPanel* controlPanel;
		wxPanel* borderPanel;
		MazePanel* mazePanel;
		wxBoxSizer* controlSettingSizer;
		wxBoxSizer* controlRunSizer;
		wxBoxSizer* controlVSizer;
		wxBoxSizer* borderHSizer;
		wxBoxSizer* panelSizer;
		wxBoxSizer* mazeHSizer;
		wxBoxSizer* mazeVSizer;
		wxButton* runButton;
		wxButton* randomButton;
		wxButton* removeColumn;
		wxButton* removeRow;
		wxButton* addColumn;
		wxButton* addRow;
		wxButton* setStart;
		wxButton* setGoal;

		void initSizers();
		void initButtons();
		void bindButtons();

		void timeFunction(std::function<void()>);
	protected:

	public:
		MazeFrame();

		void onRunClick(wxCommandEvent&);
		void onRandomClick(wxCommandEvent&);
		void onRemoveColumn(wxCommandEvent&);
		void onRemoveRow(wxCommandEvent&);
		void onAddColumn(wxCommandEvent&);
		void onAddRow(wxCommandEvent&);
		void onSetStart(wxCommandEvent&);
		void onSetGoal(wxCommandEvent&);
};