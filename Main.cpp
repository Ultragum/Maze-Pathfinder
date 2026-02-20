#include <wx/wx.h>
#include "MazeFrame.h"

class App : public wxApp {
public:
    bool OnInit() {
        MazeFrame* frame = new MazeFrame();
        frame->SetIcon(wxIconLocation("MazeIcon.ico"));
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(App);