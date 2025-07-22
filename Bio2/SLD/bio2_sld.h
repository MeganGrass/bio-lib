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
*		SLD Decompression: https://github.com/pmandin/reevengi-tools
*
*/


#pragma once

#include <std_text.h>

#include <sony_texture.h>


class Resident_Evil_2_SLD {
private:

public:

	/*
		Construction
	*/
	explicit Resident_Evil_2_SLD(void)
	{
	}
	virtual ~Resident_Evil_2_SLD(void)
	{
	}

	/*
		Decompress SLD from unsigned char vector
	*/
	bool Decompress(std::vector<std::uint8_t> SLD, std::vector<std::uint8_t>& Output);


	/*
		Decompress SLD from file
	*/
	bool Decompress(std::filesystem::path Input);

};