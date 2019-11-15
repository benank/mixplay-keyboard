#pragma once
#include "interactive/interactivity.h"
#include "Interactive.h"

class InteractiveButton
{
	public:
		InteractiveButton(interactive_control control) : control(control) {};
		~InteractiveButton();

		int TriggerCooldown(const unsigned long long cooldownMs);
		int SetDisabled(bool disabled);

		const char* GetControlId() { return control.id; }

	private:
		interactive_control control;
		bool disabled = false;
};

