#include "MazeFrame.h"

void MazeFrame::initSizers() {
	controlSettingSizer = new wxBoxSizer(wxHORIZONTAL);
	controlRunSizer = new wxBoxSizer(wxHORIZONTAL);
	controlVSizer = new wxBoxSizer(wxVERTICAL);
	borderHSizer = new wxBoxSizer(wxHORIZONTAL);
	panelSizer = new wxBoxSizer(wxVERTICAL);

	mazeHSizer = new wxBoxSizer(wxHORIZONTAL);
	mazeVSizer = new wxBoxSizer(wxVERTICAL);
}

void MazeFrame::initButtons() {
	runButton = new wxButton(controlPanel, wxID_ANY, "Pathfind");
	randomButton = new wxButton(controlPanel, wxID_ANY, "Randomize");
	removeColumn = new wxButton(controlPanel, wxID_ANY, "- Column");
	removeRow = new wxButton(controlPanel, wxID_ANY, "- Row");
	addColumn = new wxButton(controlPanel, wxID_ANY, "+ Column");
	addRow = new wxButton(controlPanel, wxID_ANY, "+ Row");
	setStart = new wxButton(controlPanel, wxID_ANY, "Set Start");
	setGoal = new wxButton(controlPanel, wxID_ANY, "Set Goal");
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

// Debugging function used to time other functions
void MazeFrame::timeFunction(std::function<void()> function) {
	auto start = std::chrono::high_resolution_clock::now();

	function();

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> timeTaken(end - start);

	wxLogStatus(std::to_string(timeTaken.count()));
}

MazeFrame::MazeFrame() : wxFrame(nullptr, wxID_ANY, "Maze", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE /* | wxMAXIMIZE*/) {
	mainPanel = new wxPanel(this);
	mazePanel = new MazePanel(mainPanel);
	controlPanel = new wxPanel(mainPanel);
	borderPanel = new wxPanel(mainPanel, wxID_ANY, wxDefaultPosition, wxSize(400, 4));
	initSizers();
	initButtons();
	bindButtons();
	CreateStatusBar();

	mainPanel->SetBackgroundColour(wxColour(0, 0, 255));
	mazePanel->SetBackgroundColour(wxColour(0, 0, 0));
	controlPanel->SetBackgroundColour(wxColour(125, 125, 125));
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
	controlSettingSizer->Add(removeRow, SettingButtonSize);
	controlSettingSizer->Add(addRow, SettingButtonSize);
	controlSettingSizer->AddStretchSpacer(1);
	controlSettingSizer->Add(setStart, SettingButtonSize);
	controlSettingSizer->Add(setGoal, SettingButtonSize);
	controlSettingSizer->AddStretchSpacer(1);
	controlSettingSizer->Add(removeColumn, SettingButtonSize);
	controlSettingSizer->Add(addColumn, SettingButtonSize);
	controlSettingSizer->AddStretchSpacer(1);

	controlRunSizer->AddStretchSpacer(5);
	controlRunSizer->Add(runButton, wxSizerFlags().Expand().Proportion(3));
	controlRunSizer->AddStretchSpacer(1);
	controlRunSizer->Add(randomButton, wxSizerFlags().Expand().Proportion(3));
	controlRunSizer->AddStretchSpacer(5);

	mazePanel->addSizer(mazeVSizer);

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