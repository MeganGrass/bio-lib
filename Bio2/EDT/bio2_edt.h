/*
*
*	Megan Grass
*	March 22, 2024
*
*/


#pragma once

#include <std_text.h>


#pragma pack(push, 1)


struct Resident_Evil_2_EDT_Data
{
	std::uint8_t Vab_id;		// PlayStation Soundbank ID
	std::uint8_t Pan;			// PlayStation Soundbank Tone Pan
	std::uint8_t Tone_id;		// PlayStation Soundbank Tone ID
	std::uint8_t Priority;		// PlayStation Soundbank Tone Priority
	std::uint8_t Sch;			// Se_pri table index ID
	std::uint8_t Count;			// Get/Set VAG attributes count
};


#pragma pack(pop)


class Resident_Evil_2_EDT {
private:

	// Data
	std::vector<Resident_Evil_2_EDT_Data> Data;

public:

	Standard_String Str;

	/*
		Construction
	*/
	explicit Resident_Evil_2_EDT(std::filesystem::path Path, std::uintmax_t Count, std::uintmax_t _Ptr = 0) :
		Data(std::vector<Resident_Evil_2_EDT_Data>())
	{
		Open(Path, Count, _Ptr);
	}
	explicit Resident_Evil_2_EDT(void) :
		Data(std::vector<Resident_Evil_2_EDT_Data>())
	{
	}
	virtual ~Resident_Evil_2_EDT(void)
	{
		Close();
	}

	/*
		Count
	*/
	std::uintmax_t Count(void) const { return Data.size(); }

	/*
		Open
	*/
	std::uintmax_t Open(StdFile& File, std::uintmax_t Count, std::uintmax_t _Ptr);

	/*
		Open
	*/
	bool Open(std::filesystem::path Input, std::uintmax_t Count, std::uintmax_t _Ptr = 0);

	/*
		Save
	*/
	std::uintmax_t Save(StdFile& File, std::uintmax_t _Ptr);

	/*
		Save
	*/
	bool Save(std::filesystem::path Output, std::uintmax_t _Ptr = 0);

	/*
		Open text
	*/
	bool OpenText(std::filesystem::path Output);

	/*
		Save as text
	*/
	bool SaveAsText(std::filesystem::path Output);

	/*
		Close
	*/
	void Close(void);

};