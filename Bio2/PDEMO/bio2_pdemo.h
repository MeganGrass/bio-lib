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

#include <bio2_global.h>


#pragma pack(push, 1)


struct Resident_Evil_2_Player_Demo
{
	std::uint8_t Stage_no;
	std::uint8_t Room_no;
	std::uint8_t Cut_no;
	std::uint8_t Equip_id;
	std::uint8_t Equip_no;
	std::uint8_t Key_idx;
	std::uint8_t Player_id;
	std::uint8_t UNKNOWN;
	ITEM_WORK Item_wk[11]{};
	std::uint16_t Pdemo_cnt;
	std::int16_t Pos_x;
	std::int16_t Pos_y;
	std::int16_t Pos_z;
	std::int16_t Cdir_y;
	std::uint16_t Padd[900]{};
};


#pragma pack(pop)


class Resident_Evil_2_PDEMO {
private:

	// Data
	std::unique_ptr<Resident_Evil_2_Player_Demo> Data;

	// Flag
	bool b_Open;

public:

	/*
		Construction
	*/
	explicit Resident_Evil_2_PDEMO(std::filesystem::path Path) :
		Data(std::make_unique<Resident_Evil_2_Player_Demo>()),
		b_Open(false)
	{
		Open(Path);
	}
	explicit Resident_Evil_2_PDEMO(void) :
		Data(std::make_unique<Resident_Evil_2_Player_Demo>()),
		b_Open(false)
	{
	}
	virtual ~Resident_Evil_2_PDEMO(void)
	{
		Data.reset();
	}

	/*
		Open
	*/
	bool Open(std::filesystem::path Input);

	/*
		Save
	*/
	bool Save(std::filesystem::path Output);

	/*
		Save as text
	*/
	bool DisassembleToText(std::filesystem::path Config);

	/*
		Assemble
	*/
	bool AssembleText(std::filesystem::path Input);

};