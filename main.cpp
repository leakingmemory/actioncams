
#include <wx/wx.h>
#include "ActionCamsFrame.h"

class ActionCamsApp : public wxApp {
public:
    bool OnInit() override;
};

bool ActionCamsApp::OnInit() {
    auto *actionCamsFrame = new ActionCamsFrame();
    actionCamsFrame->Show();
    return true;
}

wxIMPLEMENT_APP(ActionCamsApp);
