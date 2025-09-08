/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO: 
*
*
*	CREDIT:
*
*		BS Decode: https://github.com/XProger/OpenResident
*
*		PAK Decompression: https://github.com/pmandin/reevengi-tools
*
*/


#pragma once

#include <sony_executable.h>

#include <sony_bitstream.h>

#include <sony_soundbank.h>

#include "pak/bio1_pak.h"

#include "clut/bio1_clut.h"

#include <bio_common.h>


class Resident_Evil :
	private Resident_Evil_Common {
private:

	// Sony PlayStation Bitstream
	std::unique_ptr<Sony_PlayStation_Bitstream> Bs;

	// Resident Evil PAK
	std::unique_ptr<Resident_Evil_PAK> Pak;

	// Print Command Line Help
	void PrintHelp(void);

public:

	/*
		Construction
	*/
	explicit Resident_Evil(void) :
		Bs(std::make_unique<Sony_PlayStation_Bitstream>()),
		Pak(std::make_unique<Resident_Evil_PAK>())
	{
		m_Game = Video_Game::Resident_Evil;
	}
	virtual ~Resident_Evil(void)
	{
		Bs.reset();
		Pak.reset();
	}

	/*
		Command Line Interface
	*/
	void Commandline(StrVec Args);

	/*
		Decompress PAK from unsigned char array
	*/
	bool DecompressPAK(std::vector<std::uint8_t> PAK, std::vector<std::uint8_t>& Output) { return Pak->Decompress(PAK, Output); }

	/*
		Decompress PAK from file
	*/
	bool DecompressPAK(std::filesystem::path Input) { return Pak->Decompress(Input); }

	/*
		Extract contents of BSS file container
	*/
	bool ExtractBSS(std::filesystem::path Input);

	/*
		Extract contents of PIX file container
	*/
	bool ExtractPIX(std::filesystem::path Input);

};