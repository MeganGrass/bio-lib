/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO:
*
*/


#pragma once


#include <cstdint>

#include <utility>

#include <string>


enum class Resident_Evil_Video_Game : std::uint64_t
{
	Resident_Evil_Aug_4_1995 = 1 << 0,
	Resident_Evil_Oct_4_1995 = 1 << 1,
	Resident_Evil_Jan_31_1996 = 1 << 2,
	Resident_Evil_Feb_8_1996 = 1 << 3,
	Resident_Evil_Feb_22_1996 = 1 << 4,
	Resident_Evil_Trial = 1 << 5,
	Resident_Evil = 1 << 6,
	Resident_Evil_Directors_Cut = 1 << 7,
	Resident_Evil_Directors_Cut_Dual_Shock = 1 << 8,
	Resident_Evil_Complete_Disc = 1 << 9,
	Resident_Evil_2_Nov_6_1996 = 1 << 10,
	Resident_Evil_2_Oct_30_1997 = 1 << 11,
	Resident_Evil_2_Trial = 1 << 12,
	Resident_Evil_2 = 1 << 13,
	Resident_Evil_2_Dual_Shock = 1 << 14,
	Resident_Evil_3_July_26_1999 = 1 << 15,
	Resident_Evil_3_Trial = 1 << 16,
	Resident_Evil_3 = 1 << 17
};


class Resident_Evil_Common {
private:

public:

	// Resident Evil video game constants
	static constexpr std::uint64_t AUG95 = std::to_underlying(Resident_Evil_Video_Game::Resident_Evil_Aug_4_1995);	// Resident Evil (Aug 4, 1995)
	static constexpr std::uint64_t OCT95 = std::to_underlying(Resident_Evil_Video_Game::Resident_Evil_Oct_4_1995);	// Resident Evil (Oct 4, 1995)
	static constexpr std::uint64_t BIO1 = std::to_underlying(Resident_Evil_Video_Game::Resident_Evil);				// Resident Evil
	static constexpr std::uint64_t BIO2NOV96 = std::to_underlying(Resident_Evil_Video_Game::Resident_Evil_2_Nov_6_1996);			// Resident Evil 2 (Nov 6, 1996)
	static constexpr std::uint64_t BIO2TRIAL = std::to_underlying(Resident_Evil_Video_Game::Resident_Evil_2_Trial);			// Resident Evil 2 (Trial Edition)
	static constexpr std::uint64_t BIO2 = std::to_underlying(Resident_Evil_Video_Game::Resident_Evil_2);			// Resident Evil 2
	static constexpr std::uint64_t BIO3 = std::to_underlying(Resident_Evil_Video_Game::Resident_Evil_3);			// Resident Evil 3

	// Resident Evil Video Game
	Resident_Evil_Video_Game Game;

	// RDT File Stage ID
	uint8_t Stage;

	// RDT File Room ID
	uint8_t Room;

	// RDT File Disk ID
	uint8_t Disk;

	/*
		Construction
	*/
	explicit Resident_Evil_Common(void) :
		Game(Resident_Evil_Video_Game::Resident_Evil),
		Stage(0),
		Room(0),
		Disk(0)
	{
	}

	/*
		Get game as integer
	*/
	std::uint64_t GetGame(void) const { return std::to_underlying(Game); }

	/*
		Set game
	*/
	virtual void SetGame(Resident_Evil_Video_Game _Game) { Game = _Game; }

	/*
		Get Stage and Room IDs from RDT file string
	*/
	void GetStageRoom(std::string Str);

};