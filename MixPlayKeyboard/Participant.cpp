#include "Participant.h"


interactive_session Participant::session;

Participant::Participant(interactive_participant participant)
{
	this->participant = participant;
}

Participant::~Participant()
{
}

unsigned int Participant::GetUserId()
{
	return 0;
}

std::string Participant::GetUsername()
{
	return std::string();
}

unsigned int Participant::GetLevel()
{
	return 0;
}

int Participant::SetGroup(std::string groupId)
{
	return 0;
}

std::string Participant::GetGroup()
{
	return std::string();
}
