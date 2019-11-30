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
	return this->participant.userId;
}

std::string Participant::GetUsername()
{
	return std::string(this->participant.userName);
}

unsigned int Participant::GetLevel()
{
	return this->participant.level;
}

int Participant::SetGroup(std::string groupId)
{
	return interactive_participant_set_group(session, this->participant.id, groupId.c_str());
}

std::string Participant::GetGroup()
{
	size_t size = 0;
	char group[1024];
	interactive_participant_get_group(session, this->participant.id, group, &size);
	return size == 0 ? "default" : std::string(group, size);
}
