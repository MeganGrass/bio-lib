/*
*
*	Megan Grass
*	September 21, 2024
*
*/

#include <cstddef>

#pragma once

#pragma pack(push, 1)


struct EFF_HEADER_BIO1
{
	std::uint16_t nSpriteGp;				// total count of sprite groups
	std::uint16_t nSSequence;				// total count of sprite sequences
	std::uint16_t CBA;						// CLUT
	std::uint16_t reserved;					// always zero (0)
};

struct EFF_SSEQUENCE_BIO1
{
	std::uint8_t pGp;						// sprite group id
	std::uint8_t Time;						// total count of frames to display this sprite (0xFF = terminator)
	std::uint8_t Width;						// sprite width
	std::uint8_t Height;					// sprite height
};

struct EFF_MSEQUENCE_INDEX_BIO1
{
	std::uint8_t pMSequence[8];				// relative pointer to EFF_MSEQUENCE_HEADER (multiply by 4 and add this struct's starting address for absolute)
};


#pragma pack(pop)