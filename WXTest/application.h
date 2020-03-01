#pragma once
#include "wx/wx.h"
#include "view.h"
#include "model.h"

class Application :public wxApp
{
public:
    virtual bool OnInit();

private:
    View* view = nullptr;
    Model* model = nullptr;
};

