// MixPlayKeyboard.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <shellapi.h>

#include "InteractiveAPI/Core.h"

int main()
{
	int err = Auth::Authorize();
	if (err) return err;

	err = Core::Initialize();
	if (err) return err;

	Core::Run();
}