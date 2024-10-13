/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO:
*
*/


#include "bio_common.h"


/*
	Get Stage and Room from rdt string
*/
void Resident_Evil_Common::GetStageRoom(std::string Str) try
{
	if (Str.size() >= 6)
	{
		Stage = std::stoi(Str.substr(4, 1), nullptr, 16);
		Room = std::stoi(Str.substr(5, 2), nullptr, 16);
	}
	else if(Str.size() >= 4)
	{
		Stage = std::stoi(Str.substr(1, 1), nullptr, 16);
		Room = std::stoi(Str.substr(2, 2), nullptr, 16);
	}

	if (std::to_underlying(Game) & (BIO3))
	{
		Disk = 0;
	}
	else if (Str.size() >= 8)
	{
		Disk = std::stoi(Str.substr(7, 1), nullptr, 16);
	}
}
catch (...) { Stage = 0, Room = 0; Disk = 0; }