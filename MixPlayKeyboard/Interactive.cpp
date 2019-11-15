#include "Interactive.h"


interactive_session Interactive::session;
std::map<std::string, std::string> Interactive::controlsByTransaction;
std::map<const char*, InteractiveScene*> Interactive::scenesById;
int Interactive::err;
InteractiveScene* Interactive::currentEnumeratingScene;

int Interactive::Initialize()
{
	err = open_session();
	if (err) return err;

	InteractiveScene::SetSession(session);
	InteractiveControl::SetSession(session);
	Participant::SetSession(session);

	err = set_handlers();
	if (err) return err;

	// Asynchronously connect to the user's interactive channel, using the interactive project specified by the version ID.
	err = interactive_connect(session, Auth::GetAuthorization().c_str(), INTERACTIVE_ID, SHARE_CODE, false);
	if (err) return err;

	return err;
}

void Interactive::Run()
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

int Interactive::Destroy()
{
	return 0;
}

void Interactive::handle_input(void * context, interactive_session session, const interactive_input * input)
{

	// Get the participant's Mixer name to give them attribution.
	std::string participantName;
	int err = get_participant_name(session, input->participantId, participantName);
	if (err)
	{
		std::cerr << "Failed to get participant user name (" << std::to_string(err) << ")" << std::endl;
		return;
	}

	// Now handle the input based on input type.
	if ((input->type == input_type_key || input->type == input_type_click) && input->buttonData.action == interactive_button_action_down)
	{
		interactive_group user_group = interactive_participant_get_group(interactive_session session, const char *participantId, char *, size_t *groupLength)
		InteractiveControl control = 
		
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

void Interactive::handle_user(void * context, interactive_session session, const interactive_user * user)
{
	std::cout << "Connecting as: " << user->userName << std::endl;
	std::cout << "Avatar: " << user->avatarUrl << std::endl;
	std::cout << "Experience: " << user->experience << std::endl;
	std::cout << "Level: " << user->level << std::endl;
	std::cout << "Sparks: " << user->sparks << std::endl;
	std::cout << "Broadcasting: " << (user->isBroadcasting ? "true" : "false") << std::endl;
}

void Interactive::handle_transaction_complete(void * context, interactive_session session, const char * transactionId, size_t transactionIdLength, unsigned int errorCode, const char * errorMessage, size_t errorMessageLength)
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

void Interactive::handle_error(void * context, interactive_session session, int errorCode, const char * errorMessage, size_t errorMessageLength)
{
	std::cerr << "Unexpected Mixer interactive error: " << errorMessage << std::endl;
}

int Interactive::set_handlers()
{
	err = set_error_handler();
	if (err) return err;

	err = set_input_handler();
	if (err) return err;

	err = set_transaction_complete_handler();
	if (err) return err;

	err = set_state_changed_handler();
	if (err) return err;

	return 0;
}

int Interactive::set_error_handler()
{
	return interactive_set_error_handler(session, handle_error);
}

int Interactive::set_input_handler()
{
	return interactive_set_input_handler(session, handle_input);
}

int Interactive::set_transaction_complete_handler()
{
	return interactive_set_transaction_complete_handler(session, handle_transaction_complete);
}

int Interactive::set_state_changed_handler()
{
	return interactive_set_state_changed_handler(session, [](void* context, interactive_session session, interactive_state previousState, interactive_state currentState)
	{
		if (interactive_connecting == previousState && interactive_connected == currentState)
		{
			// Get the connected user's data.
			int err = interactive_get_user(session, handle_user);
			if (err)
			{
				puts(std::to_string(err).c_str());
				return;
			}

			// Create a group for participants to view the joystick scene.
			err = interactive_create_group(session, "JoystickGroup", "Joystick");
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


			err = get_all_scenes();
			if (err)
			{
				puts(std::to_string(err).c_str());
				return;
			}
		}
	});
}

int Interactive::open_session()
{
	err = interactive_open_session(&session);
	return err;
}

int Interactive::close_session()
{
	interactive_close_session(session);
	return 0;
}

int Interactive::get_all_scenes()
{
	return interactive_get_scenes(session, (on_scene_enumerate) get_all_scenes_handle_scene);
}

int Interactive::get_all_scenes_handle_scene(void * context, interactive_session session, const interactive_scene * scene)
{
#ifdef MIXER_DEBUG
	std::cout << "Added scene with id " << scene->id << "\n";
#endif
	scenesById[scene->id] = new InteractiveScene(*scene);
	get_scene_controls(scenesById[scene->id]);
	return 0;
}

int Interactive::get_scene_controls(InteractiveScene* scene)
{
	currentEnumeratingScene = scene;
	return interactive_scene_get_controls(session, scene->GetId(), (on_control_enumerate) get_scene_controls_handler);
}

int Interactive::get_scene_controls_handler(void * context, interactive_session session, const interactive_control * control)
{
	currentEnumeratingScene->AddControl(control);
	return 0;
}

int Interactive::get_participant_name(interactive_session session, const char * participantId, std::string & participantName)
{
	// Get the participant's name.
	size_t participantNameLength = 0;

	// First call with a nullptr to get the required size for the user's name, MIXER_ERROR_BUFFER_SIZE is the expected return value.
	int err = interactive_participant_get_user_name(session, participantId, nullptr, &participantNameLength);
	if (MIXER_ERROR_BUFFER_SIZE != err)
	{
		return err;
	}

	// Resize the string to the correct size and call it again.
	participantName.resize(participantNameLength);
	err = interactive_participant_get_user_name(session, participantId, (char*)participantName.data(), &participantNameLength);
	// STL strings don't need a trailing null character.
	participantName = participantName.erase(participantNameLength - 1);

	return 0;
}
