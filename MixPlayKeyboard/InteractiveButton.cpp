#include "InteractiveButton.h"




InteractiveButton::~InteractiveButton()
{
}

int InteractiveButton::TriggerCooldown(const unsigned long long cooldownMs)
{
	return interactive_control_trigger_cooldown(Interactive::GetSession(), GetControlId(), cooldownMs);
}

int InteractiveButton::SetDisabled(bool disabled)
{
	return interactive_control_set_property_bool(Interactive::GetSession(), GetControlId(), CONTROL_PROP_DISABLED, disabled);
}
