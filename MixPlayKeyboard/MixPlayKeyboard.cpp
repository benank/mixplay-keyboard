// MixPlayKeyboard.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <shellapi.h>

#include "interactive/interactivity.h"
#include "Secret.h"

#define CLIENT_ID		"40eb4e14a903aba102772fdb9df6ecb4818fd17a6ab8c4e4"
#define INTERACTIVE_ID	"429567"

#define MIXER_DEBUG 1

int main()
{
	std::cout << "Authenticating...\n";

	int err = 0;
	char shortCode[7];
	size_t shortCodeLength = sizeof(shortCode);
	char shortCodeHandle[1024];
	size_t shortCodeHandleLength = sizeof(shortCodeHandle);

	// Get an OAuth short code from the user. For more information about OAuth see: https://oauth.net/2/
	err = interactive_auth_get_short_code(CLIENT_ID, CLIENT_SECRET, shortCode, &shortCodeLength, shortCodeHandle, &shortCodeHandleLength);
	if (err) return err;

	// On Windows, pop the browser for the user to approve access.
	std::string authUrl = std::string("https://www.mixer.com/go?code=") + shortCode;
	ShellExecuteA(0, 0, authUrl.c_str(), nullptr, nullptr, SW_SHOW);

	// Wait for OAuth token response.
	char refreshTokenBuffer[1024];
	size_t refreshTokenLength = sizeof(refreshTokenBuffer);
	err = interactive_auth_wait_short_code(CLIENT_ID, CLIENT_SECRET, shortCodeHandle, refreshTokenBuffer, &refreshTokenLength);
	if (err)
	{
		if (MIXER_ERROR_TIMED_OUT == err)
		{
			std::cout << "Authorization timed out, user did not approve access within the time limit." << std::endl;
		}
		else if (MIXER_ERROR_AUTH_DENIED == err)
		{
			std::cout << "User denied access." << std::endl;
		}
		return err;
	}

	// Extract the authorization header from the refresh token.
	char authBuffer[1024];
	size_t authBufferLength = sizeof(authBuffer);
	err = interactive_auth_parse_refresh_token(refreshTokenBuffer, authBuffer, &authBufferLength);
	if (err) return err;

	std::cout << "Authenticated!\n";

	for (;;);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file