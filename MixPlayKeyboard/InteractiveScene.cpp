#include "InteractiveScene.h"

interactive_session InteractiveScene::session;

InteractiveScene::InteractiveScene(interactive_scene scene, interactive_session session)
{
	this->scene = scene;
	InteractiveScene::session = session;
}

InteractiveScene::~InteractiveScene()
{
}
