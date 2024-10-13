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
*		SLD Decompression: https://github.com/pmandin/reevengi-tools
*
*/


#pragma once

#include <std_basic_fstream.h>

#include <std_text.h>

#include <sony_executable.h>

#include <sony_texture.h>

#include <sony_soundbank.h>

#include <bio2.h>

#include "clut/bio2_nov96_clut.h"


class Resident_Evil_2_Nov96 :
	private Resident_Evil_Common {
private:

	// Resident Evil 2
	std::unique_ptr<Resident_Evil_2> Bio2;

	// Print Command Line Help
	void PrintHelp(void);

public:

	/*
		Construction
	*/
	explicit Resident_Evil_2_Nov96(void) :
		Bio2(std::make_unique<Resident_Evil_2>())
	{
		Game = Resident_Evil_Video_Game::Resident_Evil_2_Nov_6_1996;
	}
	virtual ~Resident_Evil_2_Nov96(void)
	{
		Bio2.reset();
	}

	/*
		Command Line Interface
	*/
	void Commandline(StrVec Args);

	/*
		Decompress SLD from unsigned char array
	*/
	bool DecompressSLD(std::vector<std::uint8_t> SLD, std::vector<std::uint8_t>& Output) { return Bio2->DecompressSLD(SLD, Output); }

	/*
		Decompress SLD from file
	*/
	bool DecompressSLD(std::filesystem::path Input) { return Bio2->DecompressSLD(Input); }

	/*
		Extract contents of BSS file container
	*/
	bool ExtractBSS(std::filesystem::path Input) { return Bio2->ExtractBSS(Input); }

	/*
		Extract contents of ITP file container
	*/
	bool ExtractITP(std::filesystem::path Input);

	/*
		Extract contents of PIX file container
	*/
	bool ExtractPIX(std::filesystem::path Input);

};