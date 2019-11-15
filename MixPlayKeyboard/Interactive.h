#pragma once
#include "interactive/interactivity.h"
#include "Auth.h"
#include "InteractiveScene.h"

#include <iostream>
#include <string>
#include <map>
#include <chrono>
#include <thread>

class InteractiveScene;

class Interactive
{
	public:
		static int Initialize();
		static void Run();
		static int Destroy();
		static interactive_session GetSession() { return session; };
		static std::map<const char*, InteractiveScene*> GetScenes() { return scenesById; };


	private:
		static interactive_session session;
		static std::map<std::string, std::string> controlsByTransaction;
		static std::map<const char*, InteractiveScene*> scenesById;
		static int err;
		static InteractiveScene* currentEnumeratingScene;

		static void handle_input(void* context, interactive_session session, const interactive_input* input);
		static void handle_user(void* context, interactive_session session, const interactive_user* user);
		static void handle_transaction_complete(void* context, interactive_session session, const char* transactionId, size_t transactionIdLength, unsigned int errorCode, const char* errorMessage, size_t errorMessageLength);
		static void handle_error(void* context, interactive_session session, int errorCode, const char* errorMessage, size_t errorMessageLength);
		static int set_handlers();
		static int set_error_handler();
		static int set_input_handler();
		static int set_transaction_complete_handler();
		static int set_state_changed_handler();
		static int open_session();
		static int close_session();
		static int get_all_scenes();
		static int get_all_scenes_handle_scene(void *context, interactive_session session, const interactive_scene *scene);
		static int get_scene_controls(InteractiveScene* scene);
		static int get_scene_controls_handler(void *context, interactive_session session, const interactive_control *control);
		static int get_participant_name(interactive_session session, const char* participantId, std::string& participantName);
};

