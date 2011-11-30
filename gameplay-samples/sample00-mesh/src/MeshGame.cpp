#include "MeshGame.h"

// Declare our game instance
MeshGame game;

MeshGame::~MeshGame()
{
}

void MeshGame::initialize()
{
    _touched = false;

    // Load font
    _font = Font::create("res/arial40.gpb");
    assert(_font);

    // Load mesh/scene from file
    Package* pkg = Package::create("res/duck.gpb");
    _scene = pkg->loadScene();
    SAFE_RELEASE(pkg);
    assert(_scene);

    // Get the duck node
    _modelNode = _scene->findNode("duck");
    assert(_modelNode);

    // Bind the material to the model
    _modelNode->getModel()->setMaterial("res/duck.material");

    // Find the light node
    Node* lightNode = _scene->findNode("directionalLight1");
    assert(lightNode);

    // Bind the light node's direction into duck's material.
    _modelNode->getModel()->getMaterial()->getParameter("u_lightDirection")->bindValue(lightNode, &Node::getForwardVectorView);
}

void MeshGame::finalize()
{
    SAFE_RELEASE(_font);
    SAFE_RELEASE(_scene);
}

void MeshGame::update(long elapsedTime)
{
    // Rotate model
    if (!_touched)
        _modelNode->rotateY(MATH_DEG_TO_RAD(0.5f));
}

void MeshGame::render(long elapsedTime)
{
    // Clear the color and depth buffers.
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Draw our scene
    _scene->visit(this, &MeshGame::visitNode);

    // Draw the fps
    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 5, getFrameRate());
}

void MeshGame::touch(int x, int y, int touchEvent)
{
    switch (touchEvent)
    {
    case Input::TOUCHEVENT_PRESS:
        {
            _touched = true;
            _touchX = x;
        }
        break;
    case Input::TOUCHEVENT_RELEASE:
        {
            _touched = false;
            _touchX = 0;
        }
        break;
    case Input::TOUCHEVENT_MOVE:
        {
            int deltaX = x - _touchX;
            _touchX = x;
            _modelNode->rotateY(MATH_DEG_TO_RAD(deltaX * 0.5f));
        }
        break;
    default:
        break;
    };
}

void MeshGame::visitNode(Node* node, long cookie)
{
    Model* model = node->getModel(); 
    if (model)
        model->draw();
}

void MeshGame::drawFrameRate(Font* font, const Vector4& color, unsigned int x, unsigned int y, unsigned int fps)
{
    char buffer[10];
    sprintf(buffer, "%u", fps);
    font->begin();
    font->drawText(buffer, x, y, color);
    font->end();
}

