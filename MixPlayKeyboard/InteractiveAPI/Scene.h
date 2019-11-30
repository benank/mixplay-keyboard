#pragma once
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include "interactivity/interactivity.h"
#include "Control.h"
#include "../ClientIds.h"

class Scene
{
public:
	Scene(interactive_scene scene);
	~Scene();
	const char* GetId() { return scene.id; };
	void AddControl(const interactive_control* control);
	Control* GetControlById(const char* controlId);
	bool HasGroup(const char* group);

	static void SetSession(interactive_session session) { Scene::session = session; };


private:
	interactive_scene scene;
	std::map<std::string, Control*> controlsToIds;
	std::vector<interactive_group> groups;
	static interactive_session session;
	static Scene* currentEnumeratingScene;

	void RefreshGroups();
	static void enumerate_groups(void *context, interactive_session session, const interactive_group *group);
};

