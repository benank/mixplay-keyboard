#include "Scene.h"

interactive_session Scene::session;
Scene* Scene::currentEnumeratingScene;

Scene::Scene(interactive_scene scene)
{
	this->scene = scene;

}

void Scene::RefreshGroups()
{
	this->groups.clear();
	currentEnumeratingScene = this;
	interactive_scene_get_groups(session, this->scene.id, (on_group_enumerate)enumerate_groups);
}

Scene::~Scene()
{
}

void Scene::AddControl(const interactive_control* control)
{
#ifdef MIXER_DEBUG
	std::cout << "Added control with id " << control->id << " to scene with id " << scene.id << "\n";
#endif
	Control* ic = new Control(*control, Scene::session);
	controlsToIds[std::string(control->id)] = ic;
	ic->SetDisabled(false);
}

Control* Scene::GetControlById(const char * controlId)
{
	return controlsToIds[std::string(controlId)];
}

bool Scene::HasGroup(const char * groupId)
{
	RefreshGroups();
	for (interactive_group group : groups)
	{
		if (strcmp(groupId, group.id) == 0)
		{
			return true;
		}
	}
	return false;
}

void Scene::enumerate_groups(void *context, interactive_session session, const interactive_group *group)
{
	currentEnumeratingScene->groups.push_back(*group);
}
