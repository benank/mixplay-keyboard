#pragma once

#include <string>
#include <iostream>
#include <windows.h>
#include <shellapi.h>
#include <fstream>
#include <sstream>
#include "interactivity/interactivity.h"
#include "../ClientIds.h"

class Auth
{
	public:
		static int Authorize();
		static std::string GetAuthorization() { return authorization; };

	private:
		static std::string authorization;
		static std::string token;

		static int ReAuthenticate();
		static void SaveToken(std::string token);
		static void LoadToken();
};

