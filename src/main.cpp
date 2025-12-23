#include "chitr/RootFrame.h"
#include "chitr/ChitrLogger.h"
#include <wx/wx.h>
using namespace std;

class Chitr : public wxApp {
public:
    virtual bool OnInit() wxOVERRIDE {
        LOG_INFO("Application Initializing");
        RootFrame *rootFrame = new RootFrame("Chitr : Media Player");
        rootFrame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP_CONSOLE(Chitr);
