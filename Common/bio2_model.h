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

#include <bio1_model.h>


class Resident_Evil_2_Model :
	private Resident_Evil_Model {
private:

public:

	/*
		Construction
	*/
	explicit Resident_Evil_2_Model(void)
	{
		SetGame(Resident_Evil_Video_Game::Resident_Evil_2);
	}
	virtual ~Resident_Evil_2_Model(void)
	{
	}

	/*
		Open
	*/
	virtual bool Open(std::filesystem::path Input) override;

	/*
		Save
	*/
	virtual bool Save(std::filesystem::path Output) override {}

	/*
		Extract
	*/
	virtual bool Extract(std::filesystem::path Input, Resident_Evil_Video_Game _Game) override {}

	/*
		Assemble
	*/
	virtual bool Assemble(std::filesystem::path Config) override {}

};