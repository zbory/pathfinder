#pragma once
#include "wx/glcanvas.h"


class GLCanvas :public wxGLCanvas
{
public:
    GLCanvas(wxWindow* parent, int* args);
    ~GLCanvas();
    void resized(wxSizeEvent& evt);

    int getWidth();
    int getHeight();
    void render(wxPaintEvent& evt);

private:
    void prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
    void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
    void drawNodes();
    void drawEdges();
    void drawPath();
    void drawSquare(int x, int y);
    wxGLContext* glContext = nullptr;
};