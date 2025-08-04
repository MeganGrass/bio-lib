/*
*
*	Megan Grass
*	April 20, 2024
*
*/


#pragma once

#include <lib_gte.h>

#include <std_text.h>


#pragma pack(push, 1)


struct Resident_Evil_2_PRI_Layer
{
	std::uint16_t nSprite;		// Sprite count
	std::uint16_t Clut;			// CLUT
	std::int16_t OfsX;			// Add to x for actual Screen Pos
	std::int16_t OfsY;			// Add to y for actual Screen Pos
};


struct Resident_Evil_2_PRI_Square
{
	std::uint8_t u, v;			// Texture UV
	std::uint8_t x, y;			// Screen XY
	std::uint16_t otz;			// Z-Depth
	std::uint16_t size;			// Texture Width and Height
};


struct Resident_Evil_2_PRI_Rect
{
	std::uint8_t u, v;			// Texture UV
	std::uint8_t x, y;			// Screen XY
	std::uint16_t otz;			// Z-Depth
	std::uint16_t tpage;		// Texture Page (always zero, unless Bio2 Nov96)
	std::uint16_t w, h;			// Texture Width and Height
};


#pragma pack(pop)


struct Resident_Evil_2_PRI_Data
{
	Resident_Evil_2_PRI_Layer Layer{};
	std::vector<Resident_Evil_2_PRI_Rect> Sprite;
};


class Resident_Evil_2_PRI {
private:

	// Data
	std::vector<Resident_Evil_2_PRI_Data> Data;

public:

	explicit Resident_Evil_2_PRI(void) {}

	~Resident_Evil_2_PRI(void) = default;

	// Get data count
	std::size_t Count(void) const { return Data.size(); }

	// Get data element
	Resident_Evil_2_PRI_Data* Get(const std::size_t& iElement) { return &Data[iElement]; }

	// Push back empty element
	void New(void) { Data.push_back(Resident_Evil_2_PRI_Data()); }

	// Push back element from buffer
	void Add(Resident_Evil_2_PRI_Data& Input) { Data.push_back(Input); }

	// Copy element to buffer
	void Copy(std::size_t iEntry, Resident_Evil_2_PRI_Data& Output) { Output = Data[iEntry]; }

	// Paste element from buffer
	void Paste(std::size_t iEntry, Resident_Evil_2_PRI_Data& Input) { Data[iEntry] = Input; }

	// Insert element
	void Insert(std::size_t iEntry, Resident_Evil_2_PRI_Data& Input) { Data.insert(Data.begin() + iEntry, Input); }

	// Delete element
	void Delete(std::size_t iEntry) { Data.erase(Data.begin() + iEntry); }

	// Push back empty element
	void NewSprite(std::size_t iEntry) { Data[iEntry].Sprite.push_back(Resident_Evil_2_PRI_Rect()); }

	// Push back element from buffer
	void AddSprite(std::size_t iEntry, Resident_Evil_2_PRI_Rect& Input) { Data[iEntry].Sprite.push_back(Input); }

	// Copy element to buffer
	void CopySprite(std::size_t iEntry, std::size_t iSprite, Resident_Evil_2_PRI_Rect& Output) { Output = Data[iEntry].Sprite[iSprite]; }

	// Paste element from buffer
	void PasteSprite(std::size_t iEntry, std::size_t iSprite, Resident_Evil_2_PRI_Rect& Input) { Data[iEntry].Sprite[iSprite] = Input; }

	// Insert element
	void InsertSprite(std::size_t iEntry, std::size_t iSprite, Resident_Evil_2_PRI_Rect& Input) { Data[iEntry].Sprite.insert(Data[iEntry].Sprite.begin() + iSprite, Input); }

	// Delete element
	void DeleteSprite(std::size_t iEntry, std::size_t iSprite) { Data[iEntry].Sprite.erase(Data[iEntry].Sprite.begin() + iSprite); }

	// Open
	std::uintmax_t Open(StdFile& File, std::uintmax_t _Ptr);

	// Open
	bool Open(std::filesystem::path Path, std::uintmax_t _Ptr = 0)
	{
		StdFile m_File;

		m_File.SetPath(Path);

		std::uintmax_t OldPtr = _Ptr;

		_Ptr = Open(m_File, _Ptr);

		return OldPtr != _Ptr;
	}

	// Save
	std::uintmax_t Save(StdFile& File, std::uintmax_t _Ptr);

	// Save
	bool Save(std::filesystem::path Path, std::uintmax_t _Ptr);

	// Close
	void Close(void)
	{
		for (auto& Element : Data)
		{
			Element.Sprite.clear();
		}
		Data.clear();
	}

};