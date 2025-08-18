/*
*
*	Megan Grass
*	May 29, 2024
*
*/


#pragma once

#include <lib_gte.h>

#include <std_text.h>


#pragma pack(push, 1)


struct Resident_Evil_2_RVD_Data
{
	std::uint8_t Be_flg;		// On/Off Flag
	std::int8_t nFloor;			// 
	std::uint8_t Fcut;			// From Camera
	std::uint8_t Tcut;			// To Camera
	std::int16_t Xz[4][2];		// Position
};


#pragma pack(pop)


class Resident_Evil_2_RVD {
private:

	// Data
	std::vector<Resident_Evil_2_RVD_Data> Data;

public:

	/*
		Construction
	*/
	explicit Resident_Evil_2_RVD(void) {}

	/*
		Destruction
	*/
	~Resident_Evil_2_RVD(void) = default;

	/*
		Copy
	*/
	Resident_Evil_2_RVD(const Resident_Evil_2_RVD& v) noexcept : Data(v.Data) {}
	Resident_Evil_2_RVD& operator = (const Resident_Evil_2_RVD& v) noexcept { return *this = Resident_Evil_2_RVD(v); }

	/*
		Move
	*/
	Resident_Evil_2_RVD(Resident_Evil_2_RVD&& v) noexcept : Data(std::exchange(v.Data, {})) {}
	Resident_Evil_2_RVD& operator = (Resident_Evil_2_RVD&& v) noexcept
	{
		std::swap(Data, v.Data);
		return *this;
	}

	/*
		Get data
	*/
	std::vector<Resident_Evil_2_RVD_Data>& data(void) { return Data; }

	/*
		Get data count
	*/
	std::size_t Count(void) const { return Data.size(); }

	/*
		Get data element
	*/
	Resident_Evil_2_RVD_Data* Get(const std::size_t& iElement) { return &Data[iElement]; }

	/*
		Push back empty element
	*/
	void New(void) { Data.push_back(Resident_Evil_2_RVD_Data()); }

	/*
		Push back element from buffer
	*/
	void Add(Resident_Evil_2_RVD_Data& Input) { Data.push_back(Input); }

	/*
		Copy element to buffer
	*/
	void Copy(std::size_t iEntry, Resident_Evil_2_RVD_Data& Output) { Output = Data[iEntry]; }

	/*
		Paste element from buffer
	*/
	void Paste(std::size_t iEntry, Resident_Evil_2_RVD_Data& Input) { Data[iEntry] = Input; }

	/*
		Insert element
	*/
	void Insert(std::size_t iEntry, Resident_Evil_2_RVD_Data& Input) { Data.insert(Data.begin() + iEntry, Input); }

	/*
		Delete element
	*/
	void Delete(std::size_t iEntry) { Data.erase(Data.begin() + iEntry); }

	/*
		Sort
	*/
	void Sort(void);

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

};