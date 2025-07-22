/*
*
*	Megan Grass
*	May 29, 2024
*
*
*	TODO:
*
*/


#pragma once

#include <std_text.h>

#include <sony_texture.h>


#pragma pack(push, 1)


struct Resident_Evil_2_ESP_Data
{
	std::int8_t Id;
	std::vector<std::uint8_t> Data;
	std::unique_ptr<Sony_PlayStation_Texture> Tim;
};


#pragma pack(pop)