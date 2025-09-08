/*
*
*	Megan Grass
*	September 4, 2025
*
*/


#pragma once

#include <lib_gte.h>

#include <cstdint>


#pragma pack(push, 1)


struct Resident_Evil_RID_Data
{
	std::uint32_t pSp;
	std::uint32_t pTim;
	VECTOR2 View_p;
	VECTOR2 View_r;
	std::uint32_t Zero[2];
	std::uint32_t ViewR;
};


#pragma pack(pop)