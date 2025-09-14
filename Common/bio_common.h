/*
*
*	Megan Grass
*	March 30, 2024
*
*/


#pragma once


#include <utility>

#include <std_text.h>


enum class Video_Game : std::uint32_t
{
	None = (0 << 0),
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
		m_Game(Video_Game::Resident_Evil),
		m_Stage(0),
		m_Room(0),
		m_Disk(0),
		m_Cut(0),
		m_CutMax(0)
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
	Video_Game m_Game;

	// RDT File Stage ID
	uintmax_t m_Stage;

	// RDT File Room ID
	uintmax_t m_Room;

	// RDT File Disk ID
	uintmax_t m_Disk;

	// RDT Camera ID
	uintmax_t m_Cut;

	// RDT Camera Max Count
	uintmax_t m_CutMax;

	// Resident Evil Video Game
	std::uint32_t GameType(void) const { return std::to_underlying(m_Game); }

	// Get game as string
	String GameStr(void);

	// Get game as wide string
	StringW GameStrW(void) { return Standard_String().GetWide(GameStr()); }

	// Set video game type
	virtual void SetGame(Video_Game Game) { m_Game = Game; }

	// Get Stage and Room IDs from RDT file string
	void GetStageRoom(std::string Str);

	// Get Player ID from character string
	std::uint32_t GetPlayerID(Video_Game Game, std::string Str);

	// Get Weapon ID from character string
	std::uint32_t GetWeaponID(Video_Game Game, std::string Str);

	// Get Enemy ID from character string
	std::uint32_t GetEnemyID(Video_Game Game, std::string Str);

	// Get Disk ID from character string
	std::uint32_t GetEnemyDiskID(Video_Game Game, std::string Str);

	// Validate a 32-bit file pointer
	template<std::integral T>
	const bool IsValidPointer(StdFile& File, T Ptr) { return (Ptr != 0 && Ptr != 0xFFFFFFFF && Ptr < File.Size()); }

};