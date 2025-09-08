/*
*
*	Megan Grass
*	September 4, 2025
*
*/


#pragma once

#include <cstdint>


#pragma pack(push, 1)


struct Resident_Evil_RVD_Data
{
	std::uint16_t Tcut;
	std::uint16_t Fcut;
	std::int16_t Xz[4][2];
};


#pragma pack(pop)