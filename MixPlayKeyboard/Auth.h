#pragma once

#include <string>
#include <iostream>
#include <windows.h>
#include <shellapi.h>
#include "interactive/interactivity.h"
#include "ClientIds.h"

class Auth
{
	public:
		static int Authorize();
		static std::string GetAuthorization() { return authorization; };

	private:
		static std::string authorization;
};

