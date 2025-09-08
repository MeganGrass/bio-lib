/*
*
*	Megan Grass
*	September 4, 2025
*
*/


#pragma once

#include <cstdint>


#pragma pack(push, 1)


struct Resident_Evil_LIT_Data_Aug95
{
	std::uint16_t Ambient[3];		// 0x00
	std::uint32_t Unknown[12];		// 0x06	// always zero (0), probably LIGHTMATRIX
	struct DATA
	{
		VECTOR2 Pos;				// 0x3C
		CVECTOR2 Color;				// 0x48
		std::uint8_t Mode[3];		// 0x4B
		std::int16_t L;				// 0x4E
	} Data[3];
};


struct Resident_Evil_LIT_Data
{
	std::uint16_t Ambient[3];		// 0x00
	struct DATA
	{
		VECTOR2 Pos;				// 0x06
		CVECTOR2 Color;				// 0x12
		std::uint8_t Mode[3];		// 0x15
		std::int16_t L;				// 0x18
	} Data[3];
};


#pragma pack(pop)