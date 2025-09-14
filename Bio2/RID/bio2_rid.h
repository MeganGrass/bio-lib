/*
*
*	Megan Grass
*	April 22, 2024
*
*/


#pragma once

#include <lib_gte.h>

#include <std_text.h>


#pragma pack(push, 1)


struct Resident_Evil_2_RID_Data
{
	std::uint16_t end_flg;		// 1 if last element
	std::uint16_t ViewR;		// FOV
	VECTOR2 View_p;				// Eye position
	VECTOR2 View_r;				// At position
	std::uint32_t pSp;			// Pointer to object sprite data (*.pri)
};


#pragma pack(pop)


class Resident_Evil_2_RID {
private:

	std::vector<Resident_Evil_2_RID_Data> Data;

public:

	Standard_String Str;

	explicit Resident_Evil_2_RID(void) {}

	~Resident_Evil_2_RID(void) = default;

	// Copy
	Resident_Evil_2_RID(const Resident_Evil_2_RID& v) noexcept : Data(v.Data) {}
	Resident_Evil_2_RID& operator = (const Resident_Evil_2_RID& v) noexcept { return *this = Resident_Evil_2_RID(v); }

	// Move
	Resident_Evil_2_RID(Resident_Evil_2_RID&& v) noexcept : Data(std::exchange(v.Data, {})) {}
	Resident_Evil_2_RID& operator = (Resident_Evil_2_RID&& v) noexcept { std::swap(Data, v.Data); return *this; }

	// Get data count
	std::size_t Count(void) const { return Data.size(); }

	// Get data element
	Resident_Evil_2_RID_Data* Get(const std::size_t& iElement) { return &Data[iElement]; }

	// Push back empty element
	void New(void) { Data.push_back(Resident_Evil_2_RID_Data()); }

	// Push back element from buffer
	void Add(Resident_Evil_2_RID_Data& Input) { Data.push_back(Input); }

	// Copy element to buffer
	void Copy(std::size_t iEntry, Resident_Evil_2_RID_Data& Output) { Output = Data[iEntry]; }

	// Paste element from buffer
	void Paste(std::size_t iEntry, Resident_Evil_2_RID_Data& Input) { Data[iEntry] = Input; }

	// Insert element
	void Insert(std::size_t iEntry, Resident_Evil_2_RID_Data& Input) { Data.insert(Data.begin() + iEntry, Input); }

	// Delete Delete
	void Delete(std::size_t iEntry) { Data.erase(Data.begin() + iEntry); }

	// Open
	std::uintmax_t Open(StdFile& File, std::size_t nCut, std::uintmax_t _Ptr);

	// Open
	bool Open(std::filesystem::path Path, std::uintmax_t _Ptr = 0);

	// Save
	std::uintmax_t Save(StdFile& File, std::uintmax_t _Ptr);

	// Save
	bool Save(std::filesystem::path Path, std::uintmax_t _Ptr);

	// Close
	void Close(void);

};