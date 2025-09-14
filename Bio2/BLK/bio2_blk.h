/*
*
*	Megan Grass
*	May 29, 2024
*
*/


#pragma once

#include <lib_gte.h>

#include <std_text.h>

#include <bitset>


#pragma pack(push, 1)


struct Resident_Evil_2_BLK_Data
{
	std::int16_t Pos_x;
	std::int16_t Pos_z;
	std::int16_t Size_x;
	std::int16_t Size_z;
	std::int16_t X;
	std::uint16_t dBit;
};


#pragma pack(pop)


class Resident_Evil_2_BLK {
private:

	// Data
	std::vector<Resident_Evil_2_BLK_Data> Data;

public:

	Standard_String Str;

	/*
		Construction
	*/
	explicit Resident_Evil_2_BLK(void)
	{
	}

	/*
		Destruction
	*/
	~Resident_Evil_2_BLK(void)
	{
		Close();
	}

	/*
		Copy
	*/
	Resident_Evil_2_BLK(const Resident_Evil_2_BLK& v) noexcept : Data(v.Data) {}
	Resident_Evil_2_BLK& operator = (const Resident_Evil_2_BLK& v) noexcept { return *this = Resident_Evil_2_BLK(v); }

	/*
		Move
	*/
	Resident_Evil_2_BLK(Resident_Evil_2_BLK&& v) noexcept : Data(std::exchange(v.Data, {})) {}
	Resident_Evil_2_BLK& operator = (Resident_Evil_2_BLK&& v) noexcept
	{
		std::swap(Data, v.Data);
		return *this;
	}

	/*
		Get data count
	*/
	std::size_t Count(void) const { return Data.size(); }

	/*
		Get data element
	*/
	Resident_Evil_2_BLK_Data* Get(const std::size_t& iElement) { return &Data[iElement]; }

	/*
		Push back empty element
	*/
	void New(void) { Data.push_back(Resident_Evil_2_BLK_Data()); }

	/*
		Push back element from buffer
	*/
	void Add(Resident_Evil_2_BLK_Data& Input) { Data.push_back(Input); }

	/*
		Copy element to buffer
	*/
	void Copy(std::size_t iEntry, Resident_Evil_2_BLK_Data& Output) { Output = Data[iEntry]; }

	/*
		Paste element from buffer
	*/
	void Paste(std::size_t iEntry, Resident_Evil_2_BLK_Data& Input) { Data[iEntry] = Input; }

	/*
		Insert element
	*/
	void Insert(std::size_t iEntry, Resident_Evil_2_BLK_Data& Input) { Data.insert(Data.begin() + iEntry, Input); }

	/*
		Delete element
	*/
	void Delete(std::size_t iEntry) { Data.erase(Data.begin() + iEntry); }

	/*
		Open
	*/
	std::uintmax_t Open(StdFile& File, std::uintmax_t _Ptr);

	/*
		Open
	*/
	bool Open(std::filesystem::path Path, std::uintmax_t _Ptr = 0);

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

	/*
		Close
	*/
	std::bitset<16> getbits(std::size_t iEntry)
	{
		std::bitset<16> bits(Data[iEntry].dBit);
		return bits;
	}

};