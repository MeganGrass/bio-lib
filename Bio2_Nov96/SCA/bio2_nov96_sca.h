/*
*
*	Megan Grass
*	September 4, 2025
*
*/


#pragma once

#include <cstdint>


#pragma pack(push, 1)


struct Resident_Evil_2_Nov96_SCA_Header
{
	std::int16_t Cx;
	std::int16_t Cz;
	std::uint32_t nData0;
	std::uint32_t nData1;
	std::uint32_t nData2;
	std::uint32_t nData3;
	std::uint32_t nData4;
};


struct Resident_Evil_2_Nov96_SCA_Data
{
	std::uint16_t W;
	std::uint16_t D;
	std::int16_t X;
	std::int16_t Z;
	union Id
	{
		std::uint16_t Data;					// Contiguous
		struct Bits							// Bitfield
		{
			std::uint16_t Shape : 8;		// 0 = *unused*
											// 1 = Box
											// 2 = Hishi
											// 3 = Circle
											// 4 = Naname A
											// 5 = Naname B
											// 6 = Naname C
											// 7 = Naname D
											// 8 = Koban X
											// 9 = Koban Z

			std::uint16_t unk : 8;			// 0 = ???
											// 1 = ???
		} Bits;
	} Id;
	union Type
	{
		std::uint16_t Data;					// Contiguous
		struct Bits							// Bitfield
		{
			std::uint16_t Climb : 8;		// 1 = Jump Down (Sca_hit_box_2)
											// 2 = Climb Up (Sca_hit_box_1)
											// E = 

			std::uint16_t High : 4;			// Height in units of -1800
											// 
											// Accepted bits are 0-15 (Range: 0, -27000)
											// 
											// Add "Low" to this value to get true height
											// 
											// Example:
											//		Height = ((Low + High) * -1800)
											//
											// Retail Conversion ("Type.Bits.nFloor" variable calculation):
											//		Type.Bits.nFloor = Low + High

			std::uint16_t Low : 4;			// Ground in units of -1800
											// 
											// Accepted bits are 0-15 (Range: 0, -27000)
											//
											// Retail Conversion ("Floor" variable calculation):
											// 
											//		Type.Bits.Height = 18;	// Default height is always 18 (-1800)
											// 
											//		Floor = 0;
											// 
											//		if (!Low)	// Ground height is zero (0)
											//		{
											//			LONG Height = ((Low + (High - 1)) * -1800);
											//			Height /= -1800;
											//			if (!Height) { Floor = 1; }	// Floor variable cannot be zero (0)
											//			else for (ULONG nBit = NULL; nBit < Height; nBit++) { Floor |= 1 << nBit; }
											//		}
											//
											//		else Floor |= 1 << Low;		// Collision is not at ground level (Ceiling, lifted platform, etc)
		} Bits;
	} Type;
};


#pragma pack(pop)