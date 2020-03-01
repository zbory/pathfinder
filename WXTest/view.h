#pragma once
#include "wx/wx.h"
#include "model.h"
#include "gLCanvas.h"

enum ButtonID {
    ADD_WEIGHT_BUTTON,
    SELECT_START_BUTTON,
    SELECT_END_BUTTON,
    RESET_GRAPH,
    BFS_BUTTON
};

enum class GUIState {
    ADDING_NODES,
    NODE_SELECTED,
    SELECTING_START,
    SELECTING_END,
    DELETING_NODES,
    EDGE_SELECTED
};

class View :public wxFrame
{
public:
    View(Model* model);
    Model* getModel();
    GLCanvas* getCanvas();
private:
    void mouseDown(wxMouseEvent& event);
    void buttonPresseed(wxCommandEvent& event);
    int IsNodeClicked(int x, int y);
    std::pair<int, int> IsEdgeClicked(int x, int y);
    bool Intersected(Node node1, Node node2, int x, int y);

    void mouseMoved(wxMouseEvent& event);
    void mouseWheelMoved(wxMouseEvent& event);
    void mouseReleased(wxMouseEvent& event);
    void rightClick(wxMouseEvent& event);
    void mouseLeftWindow(wxMouseEvent& event);
    void keyPressed(wxKeyEvent& event);
    void keyReleased(wxKeyEvent& event);

    void CreateGUI();
    Model* model;
    GUIState state = GUIState::ADDING_NODES;

    wxBoxSizer* vbox = nullptr;
    wxBoxSizer* hbox1 = nullptr;
    wxButton* addWeightButton = nullptr;
    wxButton* selectStartButton = nullptr;
    wxButton* selectEndButton = nullptr;
    wxButton* resetButton = nullptr;
    wxButton* bfsButton = nullptr;
    wxTextCtrl* addWeightTextBox = nullptr;
    GLCanvas* glCanvas = nullptr;

};

