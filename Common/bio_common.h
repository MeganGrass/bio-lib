/*
*
*	Megan Grass
*	March 30, 2024
*
*/


#pragma once


#include <utility>

#include <std_text.h>


#define BIO1_CAMERA_MAX		8
#define BIO2_CAMERA_MAX		16
#define BIO3_CAMERA_MAX		32


#define BIO1_OMODEL_MAX		16
#define BIO2_OMODEL_MAX		32
#define BIO3_OMODEL_MAX		32


#define BIO1_IMODEL_MAX		16
#define BIO2_IMODEL_MAX		0
#define BIO3_IMODEL_MAX		0


#define BIO1_BLK_MAX		16
#define BIO2_BLK_MAX		16
#define BIO3_BLK_MAX		32


enum class Video_Game : std::uint32_t
{
	Resident_Evil_Aug_4_1995 = (1 << 0),
	Resident_Evil_Oct_4_1995 = (1 << 1),
	Resident_Evil_Trial = (1 << 2),	// Jan_15_1996
	Resident_Evil_Jan_31_1996 = (1 << 3),
	Resident_Evil_Feb_8_1996 = (1 << 4),
	Resident_Evil_Feb_22_1996 = (1 << 5),
	Resident_Evil = (1 << 6),
	Resident_Evil_Directors_Cut = (1 << 7),
	Resident_Evil_Directors_Cut_Dual_Shock = (1 << 8),
	Resident_Evil_2_Nov_6_1996 = (1 << 9),
	Resident_Evil_2_Trial = (1 << 10),	// Aug_6_1997 and Aug_28_1997
	Resident_Evil_2_Oct_30_1997 = (1 << 11),
	Resident_Evil_2 = (1 << 12),
	Resident_Evil_2_Dual_Shock = (1 << 13),
	Resident_Evil_3_July_26_1999 = (1 << 14),
	Resident_Evil_3_Trial = (1 << 15),	// Aug_24_1999
	Resident_Evil_3 = (1 << 16),
	Resident_Evil_Complete_Disc = (1 << 30),
};


class Resident_Evil_Common {
private:

public:

	explicit Resident_Evil_Common(void) :
		Game(Video_Game::Resident_Evil),
		Stage(0),
		Room(0),
		Disk(0),
		Cut(0),
		CutMax(0)
	{}

	virtual ~Resident_Evil_Common(void) = default;

	// Resident Evil video game constants
	static constexpr std::uint32_t AUG95 = std::to_underlying(Video_Game::Resident_Evil_Aug_4_1995);	// Resident Evil (Aug 4, 1995)
	static constexpr std::uint32_t OCT95 = std::to_underlying(Video_Game::Resident_Evil_Oct_4_1995);	// Resident Evil (Oct 4, 1995)
	static constexpr std::uint32_t BIO1 = std::to_underlying(Video_Game::Resident_Evil);				// Resident Evil
	static constexpr std::uint32_t BIO2NOV96 = std::to_underlying(Video_Game::Resident_Evil_2_Nov_6_1996);			// Resident Evil 2 (Nov 6, 1996)
	static constexpr std::uint32_t BIO2TRIAL = std::to_underlying(Video_Game::Resident_Evil_2_Trial);			// Resident Evil 2 (Trial Edition)
	static constexpr std::uint32_t BIO2 = std::to_underlying(Video_Game::Resident_Evil_2);			// Resident Evil 2
	static constexpr std::uint32_t BIO3 = std::to_underlying(Video_Game::Resident_Evil_3);			// Resident Evil 3

	// Resident Evil Video Game
	Video_Game Game;

	// RDT File Stage ID
	uint8_t Stage;

	// RDT File Room ID
	uint8_t Room;

	// RDT File Disk ID
	uint8_t Disk;

	// RDT Camera ID
	uint8_t Cut;

	// RDT Camera Max Count
	uint8_t CutMax;

	// Resident Evil Video Game
	std::uint32_t GameType(void) const { return std::to_underlying(Game); }

	// Get game as string
	String GameStr(void);

	// Get game as wide string
	StringW GameStrW(void) { return Standard_String().GetWide(GameStr()); }

	// Set video game type
	virtual void SetGame(Video_Game _Game) { Game = _Game; }

	// Get Stage and Room IDs from RDT file string
	void GetStageRoom(std::string Str);

};