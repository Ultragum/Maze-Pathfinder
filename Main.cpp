#include <wx/wx.h>
#include "MazeFrame.h"

class App : public wxApp {
public:
    bool OnInit() {
        MazeFrame* frame = new MazeFrame();
        //frame->SetIcon(wxIcon("MazeIcon", wxBITMAP_TYPE_PNG));
        frame->Show();
        //frame->SetIcon(wxIcon());
        return true;
    }
};

wxIMPLEMENT_APP(App);