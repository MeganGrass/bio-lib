/*
*
*	Megan Grass
*	March 31, 2024
*
*/


#include "bio_common.h"


String Resident_Evil_Common::GameStr(void)
{
	switch (Game)
	{
	case Video_Game::Resident_Evil_Aug_4_1995: return "Resident Evil (Aug 4, 1995)";
	case Video_Game::Resident_Evil_Oct_4_1995: return "Resident Evil (Oct 4, 1995)";
	case Video_Game::Resident_Evil_Trial: return "Resident Evil (Trial)";
	case Video_Game::Resident_Evil_Jan_31_1996: return "Resident Evil (Jan 31, 1996)";
	case Video_Game::Resident_Evil_Feb_8_1996: return "Resident Evil (Feb 8, 1996)";
	case Video_Game::Resident_Evil_Feb_22_1996: return "Resident Evil (Feb 22, 1996)";
	case Video_Game::Resident_Evil: return "Resident Evil";
	case Video_Game::Resident_Evil_Directors_Cut: return "Resident Evil (Directors Cut)";
	case Video_Game::Resident_Evil_Directors_Cut_Dual_Shock: return "Resident Evil (Directors Cut Dual Shock)";
	case Video_Game::Resident_Evil_2_Nov_6_1996: return "Resident Evil 2 (Nov 6, 1996)";
	case Video_Game::Resident_Evil_2_Trial: return "Resident Evil 2 (Trial)";
	case Video_Game::Resident_Evil_2_Oct_30_1997: return "Resident Evil 2 (Oct 30, 1997)";
	case Video_Game::Resident_Evil_2: return "Resident Evil 2";
	case Video_Game::Resident_Evil_2_Dual_Shock: return "Resident Evil 2 (Dual Shock)";
	case Video_Game::Resident_Evil_3_July_26_1999: return "Resident Evil 3 (July 26, 1999)";
	case Video_Game::Resident_Evil_3_Trial: return "Resident Evil 3 (Trial)";
	case Video_Game::Resident_Evil_3: return "Resident Evil 3";
	default: return "\0\0";
	}
}

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

	Disk = 0;

	if (Str.size() >= 8)
	{
		Disk = std::stoi(Str.substr(7, 1), nullptr, 16);
	}
}
catch (...) { Stage = 0, Room = 0; Disk = 0; }