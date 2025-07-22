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

#include <lib_gte.h>

#include <std_text.h>


#pragma pack(push, 1)


struct Resident_Evil_2_LIT_Data
{
	std::uint8_t Mag;			// 
	std::uint8_t Mode[3];		//
	CVECTOR2 Col[3];			// Diffuse Color
	CVECTOR2 Ambient;			// Ambient Color
	SVECTOR2 Pos[3];			// Position
	std::uint16_t L[3];			// Luminescence
};


#pragma pack(pop)


class Resident_Evil_2_LIT {
private:

	// Data
	std::vector<Resident_Evil_2_LIT_Data> Data;

public:

	/*
		Construction
	*/
	explicit Resident_Evil_2_LIT(void)
	{
	}

	/*
		Destruction
	*/
	~Resident_Evil_2_LIT(void)
	{
		Close();
	}

	/*
		Copy
	*/
	Resident_Evil_2_LIT(const Resident_Evil_2_LIT& v) noexcept : Data(v.Data) {}
	Resident_Evil_2_LIT& operator = (const Resident_Evil_2_LIT& v) noexcept { return *this = Resident_Evil_2_LIT(v); }

	/*
		Move
	*/
	Resident_Evil_2_LIT(Resident_Evil_2_LIT&& v) noexcept : Data(std::exchange(v.Data, {})) {}
	Resident_Evil_2_LIT& operator = (Resident_Evil_2_LIT&& v) noexcept
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
	Resident_Evil_2_LIT_Data* Get(const std::size_t& iElement) { return &Data[iElement]; }

	/*
		Push back empty element
	*/
	void New(void) { Data.push_back(Resident_Evil_2_LIT_Data()); }

	/*
		Push back element from buffer
	*/
	void Add(Resident_Evil_2_LIT_Data& Input) { Data.push_back(Input); }

	/*
		Copy element to buffer
	*/
	void Copy(std::size_t iEntry, Resident_Evil_2_LIT_Data& Output) { Output = Data[iEntry]; }

	/*
		Paste element from buffer
	*/
	void Paste(std::size_t iEntry, Resident_Evil_2_LIT_Data& Input) { Data[iEntry] = Input; }

	/*
		Insert element
	*/
	void Insert(std::size_t iEntry, Resident_Evil_2_LIT_Data& Input) { Data.insert(Data.begin() + iEntry, Input); }

	/*
		Delete element
	*/
	void Delete(std::size_t iEntry) { Data.erase(Data.begin() + iEntry); }

	/*
		Open
	*/
	std::uintmax_t Open(StdFile& File, std::uintmax_t nCut, std::uintmax_t _Ptr);

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