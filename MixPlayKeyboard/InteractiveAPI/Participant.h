#pragma once
#include "interactivity/interactivity.h"
#include <iostream>
#include <string>

class Participant
{
public:
	Participant(interactive_participant participant);
	~Participant();

	static void SetSession(interactive_session session) { Participant::session = session; };

	unsigned int GetUserId();
	std::string GetUsername();
	unsigned int GetLevel();

	int SetGroup(std::string groupId);
	std::string GetGroup();


private:
	interactive_participant participant;
	static interactive_session session;
};

