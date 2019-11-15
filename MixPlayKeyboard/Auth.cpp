#include "Auth.h"

std::string Auth::authorization;

int Auth::Authorize()
{
#ifdef MIXER_DEBUG
	std::cout << "Authorizing...\n";
#endif

	int err = 0;
	char shortCode[7];
	size_t shortCodeLength = _countof(shortCode);
	char shortCodeHandle[1024];
	size_t shortCodeHandleLength = _countof(shortCodeHandle);

	// Get an OAuth short code from the user. For more information about OAuth see: https://oauth.net/2/
	err = interactive_auth_get_short_code(CLIENT_ID, CLIENT_SECRET, shortCode, &shortCodeLength, shortCodeHandle, &shortCodeHandleLength);
	if (err) return err;

	// Pop the browser for the user to approve access.
	std::string authUrl = std::string("https://www.mixer.com/go?code=") + shortCode;
	ShellExecuteA(0, 0, authUrl.c_str(), nullptr, nullptr, SW_SHOW);

	// Wait for OAuth token response.
	char refreshTokenBuffer[1024];
	size_t refreshTokenLength = _countof(refreshTokenBuffer);
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

	/*
	*	TODO:	This is where you would serialize the refresh token locally or on your own service for future use in a way that is associated with the current user.
	*			Future calls would then only need to check if the token is stale, refresh it if so, and then parse the new authorization header.
	*/

	// Extract the authorization header from the refresh token.
	char authBuffer[1024];
	size_t authBufferLength = _countof(authBuffer);
	err = interactive_auth_parse_refresh_token(refreshTokenBuffer, authBuffer, &authBufferLength);
	if (err) return err;

	// Set the authorization out parameter.
	Auth::authorization = std::string(authBuffer, authBufferLength);

#ifdef MIXER_DEBUG
	std::cout << "Successfully authorized!\n";
#endif
	return 0;
}