#include "InteractiveControl.h"




InteractiveControl::~InteractiveControl()
{
}

int InteractiveControl::TriggerCooldown(const unsigned long long cooldownMs)
{
	return interactive_control_trigger_cooldown(session, GetControlId(), cooldownMs);
}

int InteractiveControl::SetDisabled(bool disabled)
{
	return interactive_control_set_property_bool(session, GetControlId(), CONTROL_PROP_DISABLED, disabled);
}
