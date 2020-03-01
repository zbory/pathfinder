#include "view.h"


View::View(Model* model) :wxFrame(nullptr, wxID_ANY, "LeGo", wxPoint(250,150),wxSize(1280,720))
{
    this->model = model;
    CreateGUI();
}

Model* View::getModel()
{
    return model;
}

void View::CreateGUI(){
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);

    int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };

    glCanvas = new GLCanvas(this, args);
    glCanvas->Bind(wxEVT_SIZE, &GLCanvas::resized, glCanvas);
    glCanvas->Bind(wxEVT_PAINT, &GLCanvas::render, glCanvas);
    glCanvas->Bind(wxEVT_LEFT_DOWN, &View::mouseDown, this);
    glCanvas->Bind(wxEVT_RIGHT_DOWN, &View::rightClick, this);
    hbox1->Add(glCanvas, 1, wxEXPAND);

    wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);
    vbox->Add(hbox1, 1, wxEXPAND);
    vbox->Add(hbox2, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

    resetButton = new wxButton(this, RESET_GRAPH, "Reset");
    resetButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &View::buttonPresseed, this);
    addWeightButton = new wxButton(this, ADD_WEIGHT_BUTTON, "AddWeight");
    addWeightButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &View::buttonPresseed, this);
    selectStartButton = new wxButton(this, SELECT_START_BUTTON, "SelectStart");
    selectStartButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &View::buttonPresseed, this);
    selectEndButton = new wxButton(this, SELECT_END_BUTTON, "SelectEnd");
    selectEndButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &View::buttonPresseed, this);
    bfsButton = new wxButton(this, BFS_BUTTON, "BFS");
    bfsButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &View::buttonPresseed, this);
    addWeightTextBox = new wxTextCtrl(this, 1002, "weight");
    hbox2->Add(bfsButton);
    hbox2->Add(selectStartButton);
    hbox2->Add(selectEndButton);
    hbox2->Add(addWeightTextBox);
    hbox2->Add(addWeightButton);
    hbox2->Add(resetButton);
    this->SetSizer(vbox);
    this->SetAutoLayout(true);   
}

GLCanvas* View::getCanvas()
{
    return glCanvas;
}

void View::mouseDown(wxMouseEvent& event) {
    int posX = event.GetX();
    int posY = event.GetY();
    int nodeId = IsNodeClicked(posX, posY);

    if (nodeId != 0) {
        if (state == GUIState::SELECTING_START) {
            model->SetStartNodeId(nodeId);
            state = GUIState::ADDING_NODES;
        }
        if (state == GUIState::SELECTING_END) {
            model->SetEndNodeId(nodeId);
            state = GUIState::ADDING_NODES;
        }
        if (state == GUIState::NODE_SELECTED) {
            model->GetGraph()->AddLink(model->GetSelectedNodeId(), nodeId);
        }
        if (state == GUIState::DELETING_NODES) {
            model->GetGraph()->DeleteNodeById(nodeId);
        }
    }    
    else {
        state = GUIState::ADDING_NODES;
        model->GetGraph()->AddNode(model->GetSelectedNodeId(), posX, posY);
    }
    glCanvas->Refresh();
}

void View::buttonPresseed(wxCommandEvent& event) {
    switch (event.GetId())
    {
    case ADD_WEIGHT_BUTTON:
        if (state == GUIState::EDGE_SELECTED) {
            model->GetGraph()->AddLinkWeight(model->GetSelectedEdge(),std::atoi(addWeightTextBox->GetValue().c_str()));
            state = GUIState::ADDING_NODES;
        }
        break;
    case SELECT_START_BUTTON:
        state = GUIState::SELECTING_START;
        break;
    case SELECT_END_BUTTON:
        state = GUIState::SELECTING_END;
        break;
    case BFS_BUTTON:
        model->RunBFS();
        glCanvas->Refresh();
        break;
    case RESET_GRAPH:
        model->Reset();
        glCanvas->Refresh();
    default:
        break;
    }
}

void View::rightClick(wxMouseEvent& event)
{
    int posX = event.GetX();
    int posY = event.GetY();
    int nodeID = IsNodeClicked(posX, posY);
    Edge edge = IsEdgeClicked(posX, posY);
    if (nodeID != 0) {
        model->SetSelectedNodeId(nodeID);
        model->SetSelectedEdge({ 0,0 });
        state = GUIState::NODE_SELECTED;
    }else if (edge.first != 0) {
        model->SetSelectedNodeId(0);
        model->SetSelectedEdge(edge);
        state = GUIState::EDGE_SELECTED;
    }
    else {
        model->SetSelectedNodeId(0);
        model->SetSelectedEdge({ 0,0 });
        state = GUIState::ADDING_NODES;
    }

    glCanvas->Refresh();
}

int View::IsNodeClicked(int x, int y)
{
    for (auto node : model->GetGraph()->GetNodes()) {
        if (abs(node.X - x) < 10 && abs(node.Y - y) < 10)
            return node.nodeId;
    }
    return 0;
}

Edge View::IsEdgeClicked(int x, int y)
{  
    for (auto node : model->GetGraph()->GetNodes()) {
        for (auto id : node.adjacentNodes) {
            auto adjNode = model->GetGraph()->GetNodeById(id.first);
                if (Intersected(node, *adjNode, x, y))
                    return { node.nodeId, adjNode->nodeId };            
        }
    }
    return {0,0};
}

// https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm
// https://stackoverflow.com/questions/23016676/line-segment-and-circle-intersection
// https://stackoverflow.com/questions/328107/how-can-you-determine-a-point-is-between-two-other-points-on-a-line-segment
// https://www.geogebra.org/m/psMTGDgc
// https://www.geogebra.org/m/Yu6869By
bool View::Intersected(Node node1, Node node2, int x, int y)
{
    double tolerance = 0.1;

    auto dot = [](std::pair<int, int> a, std::pair<int, int> b)
    {
        return a.first * b.first + a.second * b.second;
    };

    auto cross = [](std::pair<int, int> a, std::pair<int, int> b)
    {
        return a.first * b.second - a.second * b.first;
    };

    std::pair<int, int> d = { node2.X - node1.X, node2.Y - node1.Y };
    std::pair<int, int> f = { x - node1.X, y - node1.Y};

    double edgeLength = dot(d,d);
    double scaledArea = abs(cross(d, f)) / edgeLength;
    double projection = dot(d, f);

    if (scaledArea < tolerance && projection > 0 && projection < edgeLength)
        return true;

    return false;

    //double a = dot(d, d);
    //double b = 2 * dot(f, d);
    //double c = dot(f, f) - tolerance * tolerance;

    //double determinant = b * b - 4 * a * c;
    //if (a <= 0.0000001 || determinant < 0)
    //    return false;

    //else if (determinant == 0)
    //{
    //    // One solution.
    //    double t = -b / (2 * a);
    //    std::pair<int, int> intersection1 = { node1.X + t * d.first, node1.Y + t * d.second };
    //    glCanvas->debugClicks.push_back(intersection1);
    //    return true;
    //}
    //else
    //{
    //    // Two solutions.
    //    double t = (double)((-b + sqrt(determinant)) / (2 * a));
    //    std::pair<int, int> intersection1 = { node1.X + t * d.first, node1.Y + t * d.second };
    //    glCanvas->debugClicks.push_back(intersection1);
    //    t = (double)((-b - sqrt(determinant)) / (2 * a));
    //    std::pair<int, int> intersection2 = { node1.X + t * d.first, node1.Y + t * d.second };
    //    glCanvas->debugClicks.push_back(intersection2);
    //    return true;
    //}
    
    //-------------------------------------------------------------------------------------------------

    //else
    //{
    //    determinant = sqrt(determinant);

    //    float t1 = (-b - determinant) / (2 * a);
    //    float t2 = (-b + determinant) / (2 * a);

    //    if (t1 >= 0 && t1 <= 1)
    //        return true;

    //    if (t2 >= 0 && t2 <= 1)
    //        return true;

    //    return false;
    //}

    //-------------------------------------------------------------------------------------------------

    //return abs(-1 * (node2.Y - node1.Y) * x + (node2.X - node1.X) * y + (node2.Y - node1.Y) * node1.X - (node2.X - node1.X) * node1.Y)
    //                    / sqrt((node2.Y - node1.Y) * (node2.Y - node1.Y) + (node2.X - node1.X) * (node2.X - node1.X));
}

void View::mouseMoved(wxMouseEvent& event) {}
void View::mouseWheelMoved(wxMouseEvent& event) {}
void View::mouseReleased(wxMouseEvent& event) {}
void View::mouseLeftWindow(wxMouseEvent& event) {}
void View::keyPressed(wxKeyEvent& event) {}
void View::keyReleased(wxKeyEvent& event) {}
