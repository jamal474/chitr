#include "RootFrame.h"
#include <wx/wx.h>
using namespace std;

/**
 * Chitr is the main entry point of the application
 * it is inherited from wxApp class which can be considered
 * as the application itself
 */
class Chitr : public wxApp {
public:
    virtual bool OnInit() wxOVERRIDE {
        RootFrame *rootFrame = new RootFrame("Chitr : Media Player");
        wxLog::SetLogLevel(wxLOG_Info);
        rootFrame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(Chitr);
