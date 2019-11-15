#pragma once
#include "interactive/interactivity.h"

class InteractiveControl
{
	public:
		InteractiveControl(interactive_control control, interactive_session session) : control(control) {};
		~InteractiveControl();

		int TriggerCooldown(const unsigned long long cooldownMs);
		int SetDisabled(bool disabled);

		const char* GetControlId() { return control.id; }

	private:
		interactive_control control;
		interactive_session session;
		bool disabled = false;
};

