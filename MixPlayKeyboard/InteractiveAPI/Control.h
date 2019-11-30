#pragma once
#include "interactivity/interactivity.h"
#include <string>
#include <map>

class Control
{
	public:
		Control(interactive_control control, interactive_session session) : control(control) {};
		~Control();

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

		void SetValue(std::string key, void* value);
		void* GetValue(std::string key);


		const char* GetControlId() { return control.id; }
		static void SetSession(interactive_session session) { Control::session = session; };

	private:
		interactive_control control;
		static interactive_session session;
		bool disabled = false;
		std::map<std::string, void*> values;
};

