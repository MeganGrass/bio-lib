/*
*
*	Megan Grass
*	September 4, 2025
*
*/


#pragma once

#include <cstdint>


#pragma pack(push, 1)


struct Resident_Evil_FLR_Data
{
	std::int16_t X;
	std::int16_t Z;
	std::uint16_t W;
	std::uint16_t D;
	std::uint8_t Se_no;
	std::uint8_t unk;
};


#pragma pack(pop)