/*
*
*	Megan Grass
*	June 11, 2024
*
*/


#pragma once

#include <bio_common.h>

#include <sony_texture.h>


#pragma pack(push, 1)


struct EFF_HEADER
{
	std::uint16_t nSSequence;	// total count of sprite sequences
	std::uint16_t nSpriteGp;	// total count of sprite groups
	std::uint16_t CBA;			// CLUT
	std::uint16_t reserved;		// always zero (0)
};

struct EFF_SSEQUENCE
{
	std::uint8_t pGp;			// sprite group id
	std::uint8_t nSpr;			// total count of sprites in this sequence
	std::uint8_t Time;			// total count of frames to display this sprite (0xFF = terminator)
	std::uint8_t Twh;			// sprite width/height
	std::int16_t Hotx;			// hotspot X
	std::int16_t Hoty;			// hotspot Y
};

struct EFF_SPRITEGp
{
	std::uint8_t U, V;			// texture coordinates
	std::int8_t OfsX, OfsY;		// screen offset
};

struct EFF_MSEQUENCE_INDEX
{
	std::uint16_t pMs[8];		// relative pointer to EFF_MSEQUENCE_HEADER (multiply by 4 and add this struct's starting address for absolute)
};

struct EFF_MSEQUENCE_HEADER
{
	std::uint32_t unk;			// always one (1) (?)
	std::uint32_t nMSequence;	// total number of motion sequences, EFF_MSEQUENCE struct(s) immediately follow
};

struct EFF_MSEQUENCE
{
	std::uint8_t Routine0;
	std::uint8_t Routine1;
	std::uint8_t Free0;
	std::uint8_t Free1;
	std::uint16_t Transx;
	std::uint16_t Transy;
	std::int8_t Add_x;
	std::int8_t Add_y;
	std::int8_t Add_z;
	std::uint8_t Free2;
	std::int16_t Speed_x;
	std::int16_t Speed_y;
	std::int16_t Speed_z;
	std::uint16_t Free3;
	std::uint16_t Free4;
	std::uint16_t Free5;
};

struct EFF_DATA
{
	EFF_HEADER Header;
	std::vector<EFF_SSEQUENCE> SSequence;
	std::vector<EFF_SPRITEGp> SpriteGp;
	std::vector<std::vector<EFF_MSEQUENCE>> MSequence;
};


#pragma pack(pop)


struct Resident_Evil_2_EFF
{
	std::int8_t Id;
	std::vector<EFF_DATA> Data;
	std::unique_ptr<Sony_PlayStation_Texture> Tim;
};


class Resident_Evil_2_Effect :
	protected Resident_Evil_Common {
private:

	std::vector<Resident_Evil_2_EFF> m_Data;

public:

	Resident_Evil_2_Effect(void)
	{
	}

	~Resident_Evil_2_Effect(void) = default;

	// Get data count
	constexpr std::size_t Count(void) const { return m_Data.size(); }

	// Get data element
	Resident_Evil_2_EFF* Get(const std::size_t& iElement) { return &m_Data[iElement]; }

	// Open
	std::uintmax_t Open(StdFile& File, std::uintmax_t _Ptr);

	// Open
	bool Open(std::filesystem::path Path, std::uintmax_t _Ptr = 0);

	// Save
	std::uintmax_t Save(StdFile& File, std::uintmax_t _Ptr);

	// Save
	bool Save(std::filesystem::path Path, std::uintmax_t _Ptr = 0);

	// Close
	void Close(void);

};