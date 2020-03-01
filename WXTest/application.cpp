#include "application.h"

wxIMPLEMENT_APP(Application);

bool Application::OnInit()
{
    model = new Model();
    view = new View(model);

    view->Show();
    return true;
}
