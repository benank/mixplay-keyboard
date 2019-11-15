// MixPlayKeyboard.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <shellapi.h>

#include "interactive/interactivity.h"
#include "Auth.h"
#include "Interactive.h"

int main()
{
	int err = Auth::Authorize();
	if (err) return err;

	err = Interactive::Initialize();
	if (err) return err;

	Interactive::Run();
}