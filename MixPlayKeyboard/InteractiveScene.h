#pragma once
#include <map>
#include <iostream>
#include <string>
#include "interactive/interactivity.h"
#include "InteractiveControl.h"
#include "ClientIds.h"

class InteractiveScene
{
public:
	InteractiveScene(interactive_scene scene);
	~InteractiveScene();
	const char* GetId() { return scene.id; };
	void AddControl(const interactive_control* control);
	static void SetSession(interactive_session session) { InteractiveScene::session = session; };

private:
	interactive_scene scene;
	std::map<const char*, InteractiveControl*> controlsToIds;
	static interactive_session session;

};

