#include "InteractiveScene.h"

interactive_session InteractiveScene::session;

InteractiveScene::InteractiveScene(interactive_scene scene)
{
	this->scene = scene;
}

InteractiveScene::~InteractiveScene()
{
}

void InteractiveScene::AddControl(const interactive_control* control)
{
#ifdef MIXER_DEBUG
	std::cout << "Added control with id " << control->id << " to scene with id " << scene.id << "\n";
#endif
	InteractiveControl* ic = new InteractiveControl(*control, InteractiveScene::session);
	controlsToIds[control->id] = ic;
	ic->SetDisabled(false);
}
