#include "Core.h"


interactive_session Core::session;
std::map<std::string, std::string> Core::controlsByTransaction;
std::map<std::string, Scene*> Core::scenesById;
std::map<std::string, Participant*> Core::participantsById;
int Core::err;
Scene* Core::currentEnumeratingScene;

int Core::Initialize()
{
	err = open_session();
	if (err) return err;

	Scene::SetSession(session);
	Control::SetSession(session);
	Participant::SetSession(session);

	err = set_handlers();
	if (err) return err;

	// Asynchronously connect to the user's interactive channel, using the interactive project specified by the version ID.
	err = interactive_connect(session, Auth::GetAuthorization().c_str(), INTERACTIVE_ID, SHARE_CODE, false);
	if (err) return err;

	return err;
}

void Core::Run()
{
	// Simulate game update loop. All previously registered session callbacks will be called from this thread.
	for (;;)
	{
		// This call processes any waiting messages from the interactive service. If there are no messages this returns immediately.
		err = interactive_run(session, 1);
		if (err) break;
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	close_session();
}

int Core::Destroy()
{
	return 0;
}

Participant* Core::GetParticipantById(const char * id)
{
	return participantsById[std::string(id)];
}

Scene* Core::GetSceneById(const char * id)
{
	return scenesById[id];
}

Scene* Core::GetSceneByGroup(const char * group)
{
	std::map<std::string, Scene*>::iterator it = scenesById.begin();

	for (std::pair<std::string, Scene*> element : scenesById)
	{
		Scene* scene = element.second;
		if (scene->HasGroup(group))
		{
			return scene;
		}
	}

	return scenesById["default"];
}

void Core::handle_participants_changed(void * context, interactive_session session, interactive_participant_action action, const interactive_participant * participant)
{
	std::cout << "Participants changed, adding " << std::string(participant->id) << std::endl;
	participantsById[std::string(participant->id)] = new Participant(*participant);
}

void Core::handle_input(void * context, interactive_session session, const interactive_input * input)
{
	// Get the participant's Mixer name to give them attribution.
	Participant* participant = GetParticipantById(input->participantId);
	if (participant == NULL)
	{
		std::cout << "Participant who clicked was null." << std::endl;
		return;
	}
	Scene* scene = GetSceneByGroup(participant->GetGroup().c_str());
	Control* control = scene->GetControlById(input->control.id);

	// Now handle the input based on input type.
	if ((input->type == input_type_key || input->type == input_type_click) && input->buttonData.action == interactive_button_action_down)
	{
		bool isLivePlayButton = false;
		control->GetProperty("liveplay", &isLivePlayButton);
		if (isLivePlayButton)
		{
			std::cout << "Clicked live play button!" << std::endl;
		}
		
		/*if (0 != input->transactionIdLength)
		{
			// Capture the transaction on button down to deduct the viewer's sparks before executing any game functionality.
			controlsByTransaction[input->transactionId] = input->control.id;
			std::cout << participantName << " clicked '" << input->control.id << "'. Deducting sparks..." << std::endl;
			err = interactive_capture_transaction(session, input->transactionId);
			if (err)
			{
				std::cerr << "Failed to capture interactive transaction." << std::endl;
				return;
			}
		}
		else if (0 == strcmp("ToJoystickScene", input->control.id))
		{
			std::cout << "Moving " << participantName << " to the Joystick group." << std::endl;
			err = interactive_participant_set_group(session, input->participantId, "JoystickGroup");
			if (err)
			{
				std::cerr << "Failed to move " << participantName << " to the Joystick group." << std::endl;
				return;
			}
		}
		else if (0 == strcmp("ToDefaultScene", input->control.id))
		{
			std::cout << "Moving " << participantName << " to the default group." << std::endl;
			err = interactive_participant_set_group(session, input->participantId, "default");
			if (err)
			{
				std::cerr << "Failed to move " << participantName << " to the default group." << std::endl;
				return;
			}
		}*/
	}
}

void Core::handle_user(void * context, interactive_session session, const interactive_user * user)
{
	std::cout << "Connecting as: " << user->userName << std::endl;
	std::cout << "Avatar: " << user->avatarUrl << std::endl;
	std::cout << "Experience: " << user->experience << std::endl;
	std::cout << "Level: " << user->level << std::endl;
	std::cout << "Sparks: " << user->sparks << std::endl;
	std::cout << "Broadcasting: " << (user->isBroadcasting ? "true" : "false") << std::endl;
}

void Core::handle_transaction_complete(void * context, interactive_session session, const char * transactionId, size_t transactionIdLength, unsigned int errorCode, const char * errorMessage, size_t errorMessageLength)
{
	if (!errorCode)
	{
		// Transaction was captured, now execute the most super awesome interactive functionality!
		std::string controlId = controlsByTransaction[std::string(transactionId, transactionIdLength)];
		if (0 == strcmp("GiveHealth", controlId.c_str()))
		{
			std::cout << "Giving health to the player!" << std::endl;
		}
	}
	else
	{
		std::cerr << errorMessage << "(" << std::to_string(errorCode) << ")" << std::endl;
	}

	controlsByTransaction.erase(transactionId);
}

void Core::handle_error(void * context, interactive_session session, int errorCode, const char * errorMessage, size_t errorMessageLength)
{
	std::cerr << "Unexpected Mixer interactive error: " << errorMessage << std::endl;
}

int Core::set_handlers()
{
	err = set_error_handler();
	if (err) return err;

	err = set_input_handler();
	if (err) return err;

	err = set_transaction_complete_handler();
	if (err) return err;

	err = set_state_changed_handler();
	if (err) return err;

	err = set_participants_changed_handler();
	if (err) return err;

	return 0;
}

int Core::set_participants_changed_handler()
{
	return interactive_set_participants_changed_handler(session, handle_participants_changed);
}

int Core::set_error_handler()
{
	return interactive_set_error_handler(session, handle_error);
}

int Core::set_input_handler()
{
	return interactive_set_input_handler(session, handle_input);
}

int Core::set_transaction_complete_handler()
{
	return interactive_set_transaction_complete_handler(session, handle_transaction_complete);
}

int Core::set_state_changed_handler()
{
	return interactive_set_state_changed_handler(session, [](void* context, interactive_session session, interactive_state previousState, interactive_state currentState)
	{
		if (interactive_connecting == previousState && interactive_connected == currentState)
		{
			OnConnected();
		}
	});
}

// Called when this connects to the interactive module of the channel
void Core::OnConnected()
{
	// Get the connected user's data.
	int err = interactive_get_user(session, handle_user);
	if (err)
	{
		puts(std::to_string(err).c_str());
		return;
	}

	err = get_all_scenes();
	if (err)
	{
		puts(std::to_string(err).c_str());
		return;
	}

	// Create a group for participants to view the liveplay scene.
	err = interactive_create_group(session, "LivePlay", "liveplay");
	if (err)
	{
		puts(std::to_string(err).c_str());
		return;
	}

	// Set the corresponding scene for the LivePlay group
	err = interactive_group_set_scene(session, "LivePlay", "liveplay");
	if (err)
	{
		puts(std::to_string(err).c_str());
		return;
	}

	// Now notify participants that interactive is ready.
	err = interactive_set_ready(session, true);
	if (err)
	{
		puts(std::to_string(err).c_str());
		return;
	}

}

int Core::open_session()
{
	err = interactive_open_session(&session);
	return err;
}

int Core::close_session()
{
	interactive_close_session(session);
	return 0;
}

int Core::get_all_scenes()
{
	return interactive_get_scenes(session, (on_scene_enumerate) get_all_scenes_handle_scene);
}

int Core::get_all_scenes_handle_scene(void * context, interactive_session session, const interactive_scene * scene)
{
#ifdef MIXER_DEBUG
	std::cout << "Added scene with id " << scene->id << "\n";
#endif
	scenesById[std::string(scene->id)] = new Scene(*scene);
	get_scene_controls(scenesById[std::string(scene->id)]);
	return 0;
}

int Core::get_all_participants()
{
#ifdef MIXER_DEBUG
	std::cout << "Getting all participants..." << std::endl;
#endif
	participantsById.clear();
	return interactive_get_participants(session, (on_participant_enumerate) get_all_participants_handler);
}

int Core::get_all_participants_handler(void * context, interactive_session session, const interactive_participant * participant)
{
#ifdef MIXER_DEBUG
	std::cout << "Added participant " << std::string(participant->id) << std::endl;
#endif
	participantsById[std::string(participant->id)] = new Participant(*participant);
	return 0;
}

int Core::get_scene_controls(Scene* scene)
{
	currentEnumeratingScene = scene;
	return interactive_scene_get_controls(session, scene->GetId(), (on_control_enumerate) get_scene_controls_handler);
}

int Core::get_scene_controls_handler(void * context, interactive_session session, const interactive_control * control)
{
	currentEnumeratingScene->AddControl(control);
	return 0;
}