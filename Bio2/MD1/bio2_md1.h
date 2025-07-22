/*
*
*	Megan Grass
*	March 07, 2024
*
*/


#pragma once

#include <sony_model.h>


#pragma pack(push, 1)


struct Resident_Evil_2_Model_Header
{
	std::uint32_t pTexture;			// 0x00	// Absolute pointer to UV mapping data chunk
	std::uint32_t pad;				// 0x04	// Always zero (0)
	std::uint32_t nObject;			// 0x08	// Object count, (relative, absolute = nObject / 2)
};


struct Resident_Evil_2_Model_Index
{
	struct Triangle
	{
		std::uint32_t pVertice;		// 0x00	// Vertice pointer (relative)
		std::uint32_t nVertice;		// 0x04	// Vertice count
		std::uint32_t pNormal;		// 0x08	// Normal pointer (relative)
		std::uint32_t nNormal;		// 0x0C	// Normal count
		std::uint32_t pPrimitive;	// 0x10	// Primitive pointer (relative)
		std::uint32_t nPrimitive;	// 0x14	// Primitive/Texture count
		std::uint32_t pTexture;		// 0x18	// Texture pointer (relative)
	} Tri;
	struct Quadrangle
	{
		std::uint32_t pVertice;		// 0x1C	// Vertice pointer (relative)
		std::uint32_t nVertice;		// 0x20	// Vertice count
		std::uint32_t pNormal;		// 0x24	// Normal pointer (relative)
		std::uint32_t nNormal;		// 0x28	// Normal count
		std::uint32_t pPrimitive;	// 0x2C	// Primitive pointer (relative)
		std::uint32_t nPrimitive;	// 0x30	// Primitive/Texture count
		std::uint32_t pTexture;		// 0x34	// Texture pointer (relative)
	} Quad;
};


struct Resident_Evil_2_Triangle_Primitive
{
	std::int16_t n0, v0;
	std::int16_t n1, v1;
	std::int16_t n2, v2;
};


struct Resident_Evil_2_Triangle_Texture
{
	std::uint8_t tu0, tv0;
	std::uint16_t clut;
	std::uint8_t tu1, tv1;
	std::uint16_t tpage;
	std::uint8_t tu2, tv2;
	std::uint16_t pad;
};


struct Resident_Evil_2_Quadrangle_Primitive
{
	std::int16_t n0, v0;
	std::int16_t n1, v1;
	std::int16_t n2, v2;
	std::int16_t n3, v3;
};


struct Resident_Evil_2_Quadrangle_Texture
{
	std::uint8_t tu0, tv0;
	std::uint16_t clut;
	std::uint8_t tu1, tv1;
	std::uint16_t tpage;
	std::uint8_t tu2, tv2;
	std::uint16_t pad0;
	std::uint8_t tu3, tv3;
	std::uint16_t pad1;
};


#pragma pack(pop)


struct Resident_Evil_2_Triangle
{
	Resident_Evil_2_Triangle_Primitive Primitive;
	Resident_Evil_2_Triangle_Texture Texture;
};


struct Resident_Evil_2_Quadrangle
{
	Resident_Evil_2_Quadrangle_Primitive Primitive;
	Resident_Evil_2_Quadrangle_Texture Texture;
};


struct Resident_Evil_2_Model_Object
{
	std::vector<SVECTOR> Vertice;
	std::vector<SVECTOR> Normal;
	std::vector<Resident_Evil_2_Triangle> Triangle;
	std::vector<Resident_Evil_2_Quadrangle> Quadrangle;
};


class Resident_Evil_2_MD1 {
private:

	// Resident Evil 2 Model Object
	std::vector<Resident_Evil_2_Model_Object> Object;

	// Flag
	bool b_Open;

public:

	explicit Resident_Evil_2_MD1(void) : b_Open(false) {}
	explicit Resident_Evil_2_MD1(std::filesystem::path Path) : b_Open(false) { Open(Path); }
	explicit Resident_Evil_2_MD1(std::filesystem::path Path, std::uintmax_t _Ptr) : b_Open(false) { Open(Path, _Ptr); }
	explicit Resident_Evil_2_MD1(HWND hWnd, std::filesystem::path Path, std::uintmax_t _Ptr) : b_Open(false) { Str.hWnd = hWnd; Open(Path, _Ptr); }
	explicit Resident_Evil_2_MD1(StdFile& File, std::uintmax_t _Ptr) : b_Open(false) { Open(File, _Ptr); }
	virtual ~Resident_Evil_2_MD1(void) = default;

	// Standard String
	Standard_String Str;

	// Is the model open?
	bool operator !() { return !b_Open; }

	// Is the model open?
	bool IsOpen(void) const noexcept { return b_Open; }

	// Force open if object container is not empty
	bool ForceOpen(void) noexcept { if (!Object.empty()) { b_Open = true; } return b_Open; }

	// Open
	std::uintmax_t Open(StdFile& File, std::uintmax_t _Ptr);

	// Open
	bool Open(std::filesystem::path Path, std::uintmax_t _Ptr = 0)
	{
		StdFile m_File;
		m_File.SetPath(Path);
		Open(m_File, _Ptr);
		return b_Open;
	}

	// Save
	std::uintmax_t Save(StdFile& File, std::uintmax_t _Ptr);

	// Save
	bool Save(std::filesystem::path Path, std::uintmax_t _Ptr = 0, bool b_Truncate = true)
	{
		StdFile m_File;

		if (b_Truncate)
		{
			m_File.Open(Path, FileAccessMode::Write_Ex, true, true);
		}
		else
		{
			m_File.Open(Path, FileAccessMode::Read_Ex, true, false);
		}

		std::uintmax_t m_Ptr = _Ptr;

		_Ptr = Save(m_File, _Ptr);

		return m_Ptr != _Ptr;
	}

	// Save object
	bool SaveObject(std::filesystem::path Path, std::size_t iObject);

	// Save all objects
	bool SaveAllObjects(std::filesystem::path Directory, std::filesystem::path Stem = "obj");

	// Get Sony PlayStation Model
	std::unique_ptr<Sony_PlayStation_Model> GetTMD(void);

	// Get total object count
	std::size_t GetObjectCount(void) const { return Object.size(); }

	// Push back empty object
	void AddObject(Resident_Evil_2_Model_Object Path) { Object.push_back(Path); }

	// Get total vertice count
	std::size_t GetVerticeCount(void) const;

	// Get object vertice count
	std::size_t GetVerticeCount(std::size_t iObject) const { return Object[iObject].Vertice.size(); }

	// Get total normal count
	std::size_t GetNormalCount(void) const;

	// Get object normal count
	std::size_t GetNormalCount(std::size_t iObject) const { return Object[iObject].Normal.size(); }

	// Get total triangle count
	std::size_t GetTriangleCount(void) const;

	// Get object triangle count
	std::size_t GetTriangleCount(std::size_t iObject) const { return Object[iObject].Triangle.size(); }

	// Get total quadrangle count
	std::size_t GetQuadrangleCount(void) const;

	// Get object quadrangle count
	std::size_t GetQuadrangleCount(std::size_t iObject) { return Object[iObject].Quadrangle.size(); }

	// Get file size
	std::uintmax_t Size(void) const;

	// Close
	void Close(void);

};