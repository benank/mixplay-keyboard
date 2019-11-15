#pragma once
#include "interactive/interactivity.h"
#include <string>

class InteractiveControl
{
	public:
		InteractiveControl(interactive_control control, interactive_session session) : control(control) {};
		~InteractiveControl();

		int TriggerCooldown(const unsigned long long cooldownMs);
		int SetDisabled(bool disabled);

		int GetProperty(const char* key, int* data);
		int GetProperty(const char* key, bool* data);
		int GetProperty(const char* key, float* data);
		int GetProperty(const char* key, std::string* data);

		int SetProperty(const char* key, int data);
		int SetProperty(const char* key, bool data);
		int SetProperty(const char* key, float data);
		int SetProperty(const char* key, std::string data);


		const char* GetControlId() { return control.id; }
		static void SetSession(interactive_session session) { InteractiveControl::session = session; };

	private:
		interactive_control control;
		static interactive_session session;
		bool disabled = false;
};

