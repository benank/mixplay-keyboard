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

void InteractiveScene::AddControl(const interactive_control* control)
{
#ifdef MIXER_DEBUG
	std::cout << "Added control with id " << control->id << " to scene with id " << scene.id << "\n";
#endif
	controlsToIds[control->id] = new InteractiveControl(*control, session);
}
