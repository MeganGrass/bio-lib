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


#pragma pack(push, 1)


#pragma pack(pop)


class Resident_Evil_2_Message {
private:

	// Data
	std::vector<std::uint8_t> Data;

public:

	/*
		Construction
	*/
	explicit Resident_Evil_2_Message(void)
	{
	}

	/*
		Destruction
	*/
	~Resident_Evil_2_Message(void)
	{
		Close();
	}

	/*
		Copy
	*/
	Resident_Evil_2_Message(const Resident_Evil_2_Message& v) noexcept : Data(v.Data) {}
	Resident_Evil_2_Message& operator = (const Resident_Evil_2_Message& v) noexcept { return *this = Resident_Evil_2_Message(v); }

	/*
		Move
	*/
	Resident_Evil_2_Message(Resident_Evil_2_Message&& v) noexcept : Data(std::exchange(v.Data, {})) {}
	Resident_Evil_2_Message& operator = (Resident_Evil_2_Message&& v) noexcept
	{
		std::swap(Data, v.Data);
		return *this;
	}

	/*
		Get data count
	*/
	std::size_t Count(void) const { return Data.size(); }

	/*
		Open
	*/
	std::uintmax_t Open(StdFile& File, std::uintmax_t _Ptr, std::vector<std::wint_t> _DelimiterList = { 0xFE, 0xFFFF });

	/*
		Open
	*/
	bool Open(std::filesystem::path Path, std::uintmax_t _Ptr = 0, std::vector<std::wint_t> _DelimiterList = { 0xFE, 0xFFFF });

	/*
		Save
	*/
	std::uintmax_t Save(StdFile& File, std::uintmax_t _Ptr);

	/*
		Save
	*/
	bool Save(std::filesystem::path Path, std::uintmax_t _Ptr);

	/*
		Close
	*/
	void Close(void);

};