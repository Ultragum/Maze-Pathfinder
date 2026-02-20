#include "MazeFrame.h"

void MazeFrame::initSizers() {
	controlSettingSizer = new wxBoxSizer(wxHORIZONTAL);
	controlRunSizer = new wxBoxSizer(wxHORIZONTAL);
	controlVSizer = new wxBoxSizer(wxVERTICAL);
	borderHSizer = new wxBoxSizer(wxHORIZONTAL);
	panelSizer = new wxBoxSizer(wxVERTICAL);
	mazeHSizer = new wxBoxSizer(wxHORIZONTAL);
	mazeVSizer = new wxBoxSizer(wxVERTICAL);

	for (int i = 0; BUTTONGROUPS > i; i++) {
		buttonHSizer[i] = new wxBoxSizer(wxHORIZONTAL);
		buttonVSizer[i] = new wxBoxSizer(wxVERTICAL);
	}
}

void MazeFrame::initButtons() {
	runButton = new wxButton(buttonPanels[3], wxID_ANY, "Pathfind");
	randomButton = new wxButton(buttonPanels[4], wxID_ANY, "Randomize");
	removeColumn = new wxButton(buttonPanels[2], wxID_ANY, "- Column");
	removeRow = new wxButton(buttonPanels[0], wxID_ANY, "- Row");
	addColumn = new wxButton(buttonPanels[2], wxID_ANY, "+ Column");
	addRow = new wxButton(buttonPanels[0], wxID_ANY, "+ Row");
	setStart = new wxButton(buttonPanels[1], wxID_ANY, "Set Start");
	setGoal = new wxButton(buttonPanels[1], wxID_ANY, "Set Goal");
}

void MazeFrame::bindButtons() {
	runButton->Bind(wxEVT_BUTTON, &MazeFrame::onRunClick, this);
	randomButton->Bind(wxEVT_BUTTON, &MazeFrame::onRandomClick, this);
	removeColumn->Bind(wxEVT_BUTTON, &MazeFrame::onRemoveColumn, this);
	removeRow->Bind(wxEVT_BUTTON, &MazeFrame::onRemoveRow, this);
	addColumn->Bind(wxEVT_BUTTON, &MazeFrame::onAddColumn, this);
	addRow->Bind(wxEVT_BUTTON, &MazeFrame::onAddRow, this);
	setStart->Bind(wxEVT_BUTTON, &MazeFrame::onSetStart, this);
	setGoal->Bind(wxEVT_BUTTON, &MazeFrame::onSetGoal, this);
}

void MazeFrame::addButtonStyling(wxButton* button, int i) {
	buttonVSizer[i]->AddStretchSpacer(1);
	buttonVSizer[i]->Add(buttonHSizer[i], wxSizerFlags().Expand().Proportion(38));
	buttonVSizer[i]->AddStretchSpacer(1);

	buttonHSizer[i]->AddStretchSpacer(2);
	buttonHSizer[i]->Add(button, wxSizerFlags().Expand().Proportion(246));
	buttonHSizer[i]->AddStretchSpacer(2);
}

void MazeFrame::addButtonStyling(wxButton* leftButton, wxButton* rightButton, int i) {
	buttonVSizer[i]->AddStretchSpacer(1);
	buttonVSizer[i]->Add(buttonHSizer[i], wxSizerFlags().Expand().Proportion(38));
	buttonVSizer[i]->AddStretchSpacer(1);

	buttonHSizer[i]->AddStretchSpacer(2);
	buttonHSizer[i]->Add(leftButton, wxSizerFlags().Expand().Proportion(122));
	buttonHSizer[i]->AddStretchSpacer(2);
	buttonHSizer[i]->Add(rightButton, wxSizerFlags().Expand().Proportion(122));
	buttonHSizer[i]->AddStretchSpacer(2);
}

// Debugging function used to time other functions
void MazeFrame::timeFunction(std::function<void()> function) {
	auto start = std::chrono::high_resolution_clock::now();

	function();

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> timeTaken(end - start);

	wxLogStatus(std::to_string(timeTaken.count()));
}

MazeFrame::MazeFrame() : wxFrame(nullptr, wxID_ANY, "Maze Pathfinder", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE) {
	mainPanel = new wxPanel(this);
	mazePanel = new MazePanel(mainPanel);
	controlPanel = new wxPanel(mainPanel);
	borderPanel = new wxPanel(mainPanel, wxID_ANY, wxDefaultPosition, wxSize(400, 4));
	
	for (int i = 0; 5 > i; i++) {
		buttonPanels[i] = new wxPanel(controlPanel);
		buttonPanels[i]->SetBackgroundColour(wxColour(0, 0, 0));
	}

	initSizers();
	initButtons();
	bindButtons();
	CreateStatusBar();

	mainPanel->SetBackgroundColour(wxColour(92, 184, 255));
	mazePanel->SetBackgroundColour(wxColour(0, 0, 0));
	controlPanel->SetBackgroundColour(wxColour(235, 235, 235));
	borderPanel->SetBackgroundColour(wxColour(0,0,0));

	panelSizer->Add(mazeVSizer, wxSizerFlags().Expand().Proportion(11));
	panelSizer->Add(borderHSizer, wxSizerFlags().Expand().Proportion(0));
	panelSizer->Add(controlPanel, wxSizerFlags().Expand().Proportion(6));

	mazeVSizer->AddStretchSpacer(1);
	mazeVSizer->Add(mazeHSizer, wxSizerFlags().Expand().Proportion(18));
	mazeVSizer->AddStretchSpacer(1);

	mazeHSizer->AddStretchSpacer(1);
	mazeHSizer->Add(mazePanel, wxSizerFlags().Expand().Proportion(18));
	mazeHSizer->AddStretchSpacer(1);

	borderHSizer->Add(borderPanel, wxSizerFlags().Expand().Proportion(2));

	controlVSizer->AddStretchSpacer(1);
	controlVSizer->Add(controlSettingSizer, wxSizerFlags().Expand().Proportion(3));
	controlVSizer->AddStretchSpacer(2);
	controlVSizer->Add(controlRunSizer, wxSizerFlags().Expand().Proportion(3));
	controlVSizer->AddStretchSpacer(1);

	wxSizerFlags SettingButtonSize = wxSizerFlags().Expand().Proportion(2);
	controlSettingSizer->AddStretchSpacer(1);

	controlSettingSizer->Add(buttonPanels[0], wxSizerFlags().Expand().Proportion(4));
	addButtonStyling(removeRow, addRow, 0);
	controlSettingSizer->AddStretchSpacer(1);

	controlSettingSizer->Add(buttonPanels[1], wxSizerFlags().Expand().Proportion(4));
	addButtonStyling(setStart, setGoal, 1);
	controlSettingSizer->AddStretchSpacer(1);

	controlSettingSizer->Add(buttonPanels[2], wxSizerFlags().Expand().Proportion(4));
	addButtonStyling(removeColumn, addColumn, 2);
	controlSettingSizer->AddStretchSpacer(1);


	controlRunSizer->AddStretchSpacer(5);

	controlRunSizer->Add(buttonPanels[3], wxSizerFlags().Expand().Proportion(3));
	addButtonStyling(runButton, 3);
	controlRunSizer->AddStretchSpacer(1);

	controlRunSizer->Add(buttonPanels[4], wxSizerFlags().Expand().Proportion(3));
	addButtonStyling(randomButton, 4);
	controlRunSizer->AddStretchSpacer(5);

	mazePanel->addSizer(mazeVSizer);

	for (int i = 0; BUTTONGROUPS > i; i++) {
		buttonPanels[i]->SetSizer(buttonVSizer[i]);
		buttonVSizer[i]->SetSizeHints(controlPanel);
	}

	controlPanel->SetSizer(controlVSizer);
	controlVSizer->SetSizeHints(mainPanel);

	mainPanel->SetSizer(panelSizer);
	panelSizer->SetSizeHints(this);
	Maximize();
}

void MazeFrame::onRunClick(wxCommandEvent& event) {
	timeFunction(std::bind(&MazePanel::run, mazePanel));
}

void MazeFrame::onRandomClick(wxCommandEvent& event) {
	timeFunction(std::bind(&MazePanel::randomize, mazePanel));
}

void MazeFrame::onRemoveColumn(wxCommandEvent& event) {
	timeFunction(std::bind(&MazePanel::removeColumn, mazePanel));
	mazeVSizer->Layout();
}

void MazeFrame::onRemoveRow(wxCommandEvent& event) {
	timeFunction(std::bind(&MazePanel::removeRow, mazePanel));
	mazeVSizer->Layout();
}

void MazeFrame::onAddColumn(wxCommandEvent& event) {
	timeFunction(std::bind(&MazePanel::addColumn, mazePanel));
	mazeVSizer->Layout();
}

void MazeFrame::onAddRow(wxCommandEvent& event) {
	timeFunction(std::bind(&MazePanel::addRow, mazePanel));
	mazeVSizer->Layout();
}

void MazeFrame::onSetStart(wxCommandEvent& event) {
	mazePanel->removeStart();
}

void MazeFrame::onSetGoal(wxCommandEvent& event) {
	mazePanel->removeGoal();
}