#include "Control.h"


interactive_session Control::session;

Control::Control(interactive_control control, interactive_session session)
{
	this->session = session;
	this->id = std::string(control.id, control.idLength);
	this->kind = std::string(control.kind, control.kindLength);
}

Control::~Control()
{
}

int Control::TriggerCooldown(const unsigned long long cooldownMs)
{
	return interactive_control_trigger_cooldown(session, GetControlId(), cooldownMs);
}

int Control::SetDisabled(bool disabled)
{
	return interactive_control_set_property_bool(session, GetControlId(), CONTROL_PROP_DISABLED, disabled);
}

int Control::GetProperty(const char * key, int * data)
{
	return interactive_control_get_property_int(session, GetControlId(), key, data);
}

int Control::GetProperty(const char * key, bool * data)
{
	return interactive_control_get_property_bool(session, GetControlId(), key, data);
}

int Control::GetProperty(const char * key, float * data)
{
	return interactive_control_get_property_float(session, GetControlId(), key, data);
}

int Control::GetProperty(const char * key, std::string * data)
{
	size_t length;
	char data_c[1024];
	int err = interactive_control_get_property_string(session, GetControlId(), key, data_c, &length);
	if (err) return err;
	*data = std::string(data_c, length);
	return 0;
}

int Control::GetMetaProperty(const char * key, int * data)
{
	return interactive_control_get_meta_property_int(session, GetControlId(), key, data);
}

int Control::GetMetaProperty(const char * key, bool * data)
{
	return interactive_control_get_meta_property_bool(session, GetControlId(), key, data);
}

int Control::GetMetaProperty(const char * key, float * data)
{
	return interactive_control_get_meta_property_float(session, GetControlId(), key, data);
}

int Control::GetMetaProperty(const char * key, std::string * data)
{
	size_t length;
	char data_c[1024];
	int err = interactive_control_get_meta_property_string(session, GetControlId(), key, data_c, &length);
	if (err) return err;
	*data = std::string(data_c, length);
}

int Control::SetProperty(const char * key, int data)
{
	return interactive_control_set_property_int(session, GetControlId(), key, data);
}

int Control::SetProperty(const char * key, bool data)
{
	return interactive_control_set_property_bool(session, GetControlId(), key, data);
}

int Control::SetProperty(const char * key, float data)
{
	return interactive_control_set_property_float(session, GetControlId(), key, data);
}

int Control::SetProperty(const char * key, std::string data)
{
	return interactive_control_set_property_string(session, GetControlId(), key, data.c_str());
}

void Control::SetValue(std::string key, void * value)
{
	values[key] = value;
}

void * Control::GetValue(std::string key)
{
	return values[key];
}
