#include "gLCanvas.h"
#include "glut.h"
#include <wx\dcclient.h>
#include "view.h"
#include "igraph.h"


GLCanvas::GLCanvas(wxWindow* parent, int* args)
    :wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
    glContext = new wxGLContext(this);

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

GLCanvas::~GLCanvas()
{
    delete glContext;
}

void GLCanvas::drawEdges()
{
    View* parent = wxDynamicCast(this->GetParent(), View);
    Edge selectedEdge = parent->getModel()->GetSelectedEdge();
    auto graph = parent->getModel()->GetGraph();


    for (auto node : graph->GetNodes()) {
        for (auto id : node.adjacentNodes) {
            auto adjNode = graph->GetNodeById(id.first);
            glColor4f(0, 0, 0, 1);

            if(node.nodeId == selectedEdge.first && adjNode->nodeId == selectedEdge.second ||
                node.nodeId == selectedEdge.second && adjNode->nodeId == selectedEdge.first)
                glColor4f(0, 0, 1, 1);

            glBegin(GL_LINES);
                glVertex3f(node.X, node.Y, 0);
                glVertex3f(adjNode->X, adjNode->Y, 0);
            glEnd();
        }
    }
    
}

void GLCanvas::drawNodes()
{
    View* parent = wxDynamicCast(this->GetParent(), View);
    auto graph = parent->getModel()->GetGraph();

    int startNodeId = parent->getModel()->GetStartNodeId();
    int endNodeId = parent->getModel()->GetEndNodeId();
    int selectedNodeId = parent->getModel()->GetSelectedNodeId();

    

    for (auto node : graph->GetNodes()) {
        glColor4f(0, 0, 0, 1);
        if (node.nodeId == startNodeId) {
            glColor4f(0, 1, 0, 1);
        }
        if (node.nodeId == endNodeId) {
            glColor4f(1, 0, 0, 1);
        }
        if (node.nodeId == selectedNodeId) {
            glColor4f(0, 0, 1, 1);
        }
        drawSquare(node.X, node.Y);
    }
}

void GLCanvas::drawSquare(int x, int y)
{
    glBegin(GL_QUADS);
        glVertex3f(x + 5, y + 5, 0);
        glVertex3f(x + 5, y - 5, 0);
        glVertex3f(x - 5, y - 5, 0);
        glVertex3f(x - 5, y + 5, 0);
    glEnd();

}

void GLCanvas::drawPath()
{
    View* parent = wxDynamicCast(this->GetParent(), View);

    auto bfsLinkList = parent->getModel()->GetBFSTree();
    if (bfsLinkList.empty())
        return;

    int startNodeId = parent->getModel()->GetStartNodeId();

    for (auto link : bfsLinkList) {
        auto node1 = parent->getModel()->GetGraph()->GetNodeById(link.first);
        auto node2 = parent->getModel()->GetGraph()->GetNodeById(link.second);

        glColor4f(1, 1, 0, 1);
        drawSquare(node1->X, node1->Y);
        drawSquare(node2->X, node2->Y);

        glColor4f(1, 0, 1, 1);
        glBegin(GL_LINES);
            glVertex3f(node1->X, node1->Y, 0);
            glVertex3f(node2->X, node2->Y, 0);
        glEnd();

    }
}

void GLCanvas::render(wxPaintEvent& evt)
{
    if (!IsShown()) return;

    wxGLCanvas::SetCurrent(*glContext);
    wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // ------------- draw some 2D ----------------
    prepare2DViewport(0, 0, getWidth(), getHeight());
    glLoadIdentity();

    // white background
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, 0);
        glVertex3f(getWidth(), 0, 0);
        glVertex3f(getWidth(), getHeight(), 0);
        glVertex3f(0, getHeight(), 0);
    glEnd();

    drawNodes();
    drawEdges();
    drawPath();

    glFlush();
    SwapBuffers();
}

void GLCanvas::resized(wxSizeEvent& evt)
{
    //	wxGLCanvas::OnSize(evt);
    Refresh();
}

/** Inits the OpenGL viewport for drawing in 3D. */
void GLCanvas::prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    glClearDepth(1.0f);	// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_COLOR_MATERIAL);

    glViewport(topleft_x, topleft_y, bottomrigth_x - topleft_x, bottomrigth_y - topleft_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float ratio_w_h = (float)(bottomrigth_x - topleft_x) / (float)(bottomrigth_y - topleft_y);
    gluPerspective(45 /*view angle*/, ratio_w_h, 0.1 /*clip close*/, 200 /*clip far*/);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

/** Inits the OpenGL viewport for drawing in 2D. */
void GLCanvas::prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    glEnable(GL_TEXTURE_2D);   // textures
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(topleft_x, topleft_y, bottomrigth_x - topleft_x, bottomrigth_y - topleft_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(topleft_x, bottomrigth_x, bottomrigth_y, topleft_y);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int GLCanvas::getWidth()
{
    return GetSize().x;
}

int GLCanvas::getHeight()
{
    return GetSize().y;
}