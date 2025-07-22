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

#include <sony_sequence.h>

#include <sony_soundbank.h>


class Resident_Evil_2_BGM {
private:

	// Sony PlayStation Sequence
	std::unique_ptr<Sony_PlayStation_Sequence> Seq;

	// Sony PlayStation Soundbank
	std::unique_ptr<Sony_PlayStation_Soundbank> Vab;

	// Flag
	bool b_Open;

public:

	/*
		Construction
	*/
	explicit Resident_Evil_2_BGM(std::filesystem::path Path) :
		Seq(std::make_unique<Sony_PlayStation_Sequence>()),
		Vab(std::make_unique<Sony_PlayStation_Soundbank>()),
		b_Open(false)
	{
		Open(Path);
	}
	explicit Resident_Evil_2_BGM(void) :
		Seq(std::make_unique<Sony_PlayStation_Sequence>()),
		Vab(std::make_unique<Sony_PlayStation_Soundbank>()),
		b_Open(false)
	{
	}
	virtual ~Resident_Evil_2_BGM(void)
	{
		Close();
		Seq.reset();
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