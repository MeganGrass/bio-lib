/*
*
*	Megan Grass
*	September 4, 2025
*
*/


#pragma once

#include <cstdint>


#pragma pack(push, 1)


struct Resident_Evil_BLK_Data
{
	std::int16_t Pos_x;
	std::int16_t Pos_z;
	std::int16_t Size_x;
	std::int16_t Size_z;
	std::int16_t X;
	std::uint16_t dBit;
};


#pragma pack(pop)