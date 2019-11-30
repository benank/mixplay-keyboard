#pragma once
#include "interactivity/interactivity.h"
#include "Auth.h"
#include "Scene.h"
#include "Participant.h"

#include <iostream>
#include <string>
#include <map>
#include <chrono>
#include <thread>

class Scene;

class Core
{
	public:
		static int Initialize();
		static void Run();
		static int Destroy();
		static interactive_session GetSession() { return session; };
		static std::map<std::string, Scene*> GetScenes() { return scenesById; };

		static Participant* GetParticipantById(const char* id);
		static Scene* GetSceneById(const char* id);
		static Scene* GetSceneByGroup(const char* group);


	private:
		static interactive_session session;
		static std::map<std::string, std::string> controlsByTransaction;
		static std::map<std::string, Scene*> scenesById;
		static std::map<std::string, Participant*> participantsById;
		static int err;
		static Scene* currentEnumeratingScene;

		static void handle_participants_changed(void* context, interactive_session session, interactive_participant_action action, const interactive_participant *participant);
		static void handle_input(void* context, interactive_session session, const interactive_input* input);
		static void handle_user(void* context, interactive_session session, const interactive_user* user);
		static void handle_transaction_complete(void* context, interactive_session session, const char* transactionId, size_t transactionIdLength, unsigned int errorCode, const char* errorMessage, size_t errorMessageLength);
		static void handle_error(void* context, interactive_session session, int errorCode, const char* errorMessage, size_t errorMessageLength);
		static int set_handlers();
		static int set_participants_changed_handler();
		static int set_error_handler();
		static int set_input_handler();
		static int set_transaction_complete_handler();
		static int set_state_changed_handler();
		static void OnConnected();
		static int open_session();
		static int close_session();
		static int get_all_scenes();
		static int get_all_scenes_handle_scene(void *context, interactive_session session, const interactive_scene *scene);
		static int get_all_participants();
		static int get_all_participants_handler(void *context, interactive_session session, const interactive_participant *participant);
		static int get_scene_controls(Scene* scene);
		static int get_scene_controls_handler(void *context, interactive_session session, const interactive_control *control);
};

