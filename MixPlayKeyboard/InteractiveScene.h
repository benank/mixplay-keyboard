#pragma once
#include "interactive/interactivity.h"

class InteractiveScene
{
public:
	InteractiveScene(interactive_scene scene, interactive_session session);
	~InteractiveScene();

private:
	interactive_scene scene;
	static interactive_session session;
};

