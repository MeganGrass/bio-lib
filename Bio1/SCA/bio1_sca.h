/*
*
*	Megan Grass
*	September 4, 2025
*
*/


#pragma once

#include <cstdint>


#pragma pack(push, 1)


struct Resident_Evil_SCA_Header
{
	std::int16_t Cx;
	std::int16_t Cz;
	std::uint32_t nData0;
	std::uint32_t nData1;
	std::uint32_t nData2;
	std::uint32_t nData3;
	std::uint32_t nData4;
};


struct Resident_Evil_SCA_Data
{
	std::uint16_t Xz[2][2];
	std::uint16_t Id;						/*
											*	Shape Type
											*	0 = unused (?)
											*	1 = Box
											*	2 = unused (?)
											*	3 = Circle
											*	4 = Box_1 // Special Function: Jump Down
											*	5 = Box_2 // Special Function: Climb Up
											*/
	union Type
	{
		std::uint16_t Data;					// Contiguous
		struct Bits							// Bitfield
		{
			std::uint16_t Zero0 : 8;		// always zero (0) (?)
			std::uint16_t High : 4;			// Height in units of -1800
			std::uint16_t Zero1 : 4;		// always zero (0) (?)
		} Bits;
	} Type;
};


#pragma pack(pop)