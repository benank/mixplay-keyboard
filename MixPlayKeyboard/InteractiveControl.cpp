#include "InteractiveControl.h"


interactive_session InteractiveControl::session;

InteractiveControl::~InteractiveControl()
{
}

int InteractiveControl::TriggerCooldown(const unsigned long long cooldownMs)
{
	return interactive_control_trigger_cooldown(session, control.id, cooldownMs);
}

int InteractiveControl::SetDisabled(bool disabled)
{
	return interactive_control_set_property_bool(session, control.id, CONTROL_PROP_DISABLED, disabled);
}

int InteractiveControl::GetProperty(const char * key, int * data)
{
	return interactive_control_get_property_int(session, control.id, key, data);
}

int InteractiveControl::GetProperty(const char * key, bool * data)
{
	return interactive_control_get_property_bool(session, control.id, key, data);
}

int InteractiveControl::GetProperty(const char * key, float * data)
{
	return interactive_control_get_property_float(session, control.id, key, data);
}

int InteractiveControl::GetProperty(const char * key, std::string * data)
{
	size_t length;
	char data_c[1024];
	int err = interactive_control_get_property_string(session, control.id, key, data_c, &length);
	if (err) return err;
	*data = std::string(data_c, length);
	return 0;
}

int InteractiveControl::SetProperty(const char * key, int data)
{
	return interactive_control_set_property_int(session, control.id, key, data);
}

int InteractiveControl::SetProperty(const char * key, bool data)
{
	return interactive_control_set_property_bool(session, control.id, key, data);
}

int InteractiveControl::SetProperty(const char * key, float data)
{
	return interactive_control_set_property_float(session, control.id, key, data);
}

int InteractiveControl::SetProperty(const char * key, std::string data)
{
	return interactive_control_set_property_string(session, control.id, key, data.c_str());
}
