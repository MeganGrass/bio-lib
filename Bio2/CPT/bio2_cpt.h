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

#include <std_image.h>


class Resident_Evil_2_CPT {
private:

	// Decompression Complete
	bool b_Complete;

	// CPT File Pointer
	std::size_t p_Input;

public:

	/*
		Construction
	*/
	explicit Resident_Evil_2_CPT(void) :
		b_Complete(false),
		p_Input(0)
	{
	}
	virtual ~Resident_Evil_2_CPT(void)
	{
	}

	/*
		Decompress CPT from unsigned char vector
	*/
	bool Decompress(std::vector<std::uint8_t> Input, std::vector<std::uint8_t>& Output);


	/*
		Decompress CPT from file
	*/
	bool Decompress(std::filesystem::path Input);

};