#pragma once
#include "interactivity/interactivity.h"
#include <string>
#include <map>

class Control
{
	public:
		Control(interactive_control control, interactive_session session);
		~Control();

		int TriggerCooldown(const unsigned long long cooldownMs);
		int SetDisabled(bool disabled);

		int GetProperty(const char* key, int* data);
		int GetProperty(const char* key, bool* data);
		int GetProperty(const char* key, float* data);
		int GetProperty(const char* key, std::string* data);

		int GetMetaProperty(const char* key, int* data);
		int GetMetaProperty(const char* key, bool* data);
		int GetMetaProperty(const char* key, float* data);
		int GetMetaProperty(const char* key, std::string* data);

		int SetProperty(const char* key, int data);
		int SetProperty(const char* key, bool data);
		int SetProperty(const char* key, float data);
		int SetProperty(const char* key, std::string data);

		void SetValue(std::string key, void* value);
		void* GetValue(std::string key);


		const char* GetControlId() { return id.c_str(); }
		const char* GetControlKind() { return kind.c_str(); }
		static void SetSession(interactive_session session) { Control::session = session; };

	private:
		std::string id;
		std::string kind;
		static interactive_session session;
		bool disabled = false;
		std::map<std::string, void*> values;
};

