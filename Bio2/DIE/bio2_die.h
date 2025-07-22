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

#include <std_text.h>

#include <sony_texture.h>

#include <sony_soundbank.h>

#include "../edt/bio2_edt.h"


class Resident_Evil_2_DIE {
private:

	// Sony PlayStation Texture
	std::unique_ptr<Sony_PlayStation_Texture> Tim;

	// Resident Evil 2 EDT
	std::unique_ptr<Resident_Evil_2_EDT> Edt;

	// Sony PlayStation Soundbank
	std::unique_ptr<Sony_PlayStation_Soundbank> Vab;

	// Flag
	bool b_Open;

public:

	/*
		Construction
	*/
	explicit Resident_Evil_2_DIE(std::filesystem::path Path) :
		Tim(std::make_unique<Sony_PlayStation_Texture>()),
		Edt(std::make_unique<Resident_Evil_2_EDT>()),
		Vab(std::make_unique<Sony_PlayStation_Soundbank>()),
		b_Open(false)
	{
		Open(Path);
	}
	explicit Resident_Evil_2_DIE(void) :
		Tim(std::make_unique<Sony_PlayStation_Texture>()),
		Edt(std::make_unique<Resident_Evil_2_EDT>()),
		Vab(std::make_unique<Sony_PlayStation_Soundbank>()),
		b_Open(false)
	{
	}
	virtual ~Resident_Evil_2_DIE(void)
	{
		Close();
		Tim.reset();
		Edt.reset();
		Vab.reset();
	}

	/*
		Open
	*/
	bool Open(std::filesystem::path Input);

	/*
		Save
	*/
	bool Save(std::filesystem::path Output);

	/*
		Extract
	*/
	bool Extract(std::filesystem::path Input);

	/*
		Assemble
	*/
	bool Assemble(std::filesystem::path Config);

	/*
		Close
	*/
	void Close(void);

};