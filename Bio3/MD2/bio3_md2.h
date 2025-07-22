/*
*
*	Megan Grass
*	March 07, 2024
*
*/


#pragma once

#include <sony_model.h>


#pragma pack(push, 1)


struct Resident_Evil_3_Model_Header
{
	std::uint32_t Size;
	std::uint32_t nObject;
};


struct Resident_Evil_3_Model_Index
{
	std::uint32_t pVertice;			// Vertice pointer (relative)
	std::uint32_t pNormal;			// Normal pointer (relative)
	std::uint32_t nVertice;			// Vertice/Normal count
	std::uint32_t pTriangle;		// Triangle pointer (relative)
	std::uint32_t pQuadrangle;		// Quadrangle pointer (relative)
	std::uint16_t nTriangle;		// Triangle count
	std::uint16_t nQuadrangle;		// Quadrangle count
};


struct Resident_Evil_3_Triangle
{
	std::uint8_t tu0;
	std::uint8_t tv0;
	std::uint16_t Clut;
	std::uint8_t tu1;
	std::uint8_t tv1;
	std::uint8_t Page;
	std::uint8_t v0;
	std::uint8_t tu2;
	std::uint8_t tv2;
	std::uint8_t v1;
	std::uint8_t v2;
};


struct Resident_Evil_3_Quadrangle
{
	std::uint8_t tu0;
	std::uint8_t tv0;
	std::uint16_t Clut;
	std::uint8_t tu1;
	std::uint8_t tv1;
	std::uint16_t Page;
	std::uint8_t tu2;
	std::uint8_t tv2;
	std::uint8_t v0;
	std::uint8_t v1;
	std::uint8_t tu3;
	std::uint8_t tv3;
	std::uint8_t v2;
	std::uint8_t v3;
};


#pragma pack(pop)


struct Resident_Evil_3_Model_Object
{
	std::vector<SVECTOR> Vertice;
	std::vector<SVECTOR> Normal;
	std::vector<Resident_Evil_3_Triangle> Triangle;
	std::vector<Resident_Evil_3_Quadrangle> Quadrangle;
};


class Resident_Evil_3_MD2 {
private:

	// Resident Evil 3 Model Object
	std::vector<Resident_Evil_3_Model_Object> Object;

	// Flag
	bool b_Open;

public:

	explicit Resident_Evil_3_MD2(void) : b_Open(false) {}
	explicit Resident_Evil_3_MD2(std::filesystem::path Path) : b_Open(false) { Open(Path); }
	explicit Resident_Evil_3_MD2(std::filesystem::path Path, std::uintmax_t _Ptr) : b_Open(false) { Open(Path, _Ptr); }
	explicit Resident_Evil_3_MD2(HWND hWnd, std::filesystem::path Path, std::uintmax_t _Ptr) : b_Open(false) { Str.hWnd = hWnd; Open(Path, _Ptr); }
	explicit Resident_Evil_3_MD2(StdFile& File, std::uintmax_t _Ptr) : b_Open(false) { Open(File, _Ptr); }
	virtual ~Resident_Evil_3_MD2(void) = default;

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
	void AddObject(Resident_Evil_3_Model_Object Input) { Object.push_back(Input); }

	// Get total vertice count
	std::size_t GetVerticeCount(void) const;

	// Get object vertice count
	std::size_t GetVerticeCount(std::size_t iObject);

	// Get total normal count
	std::size_t GetNormalCount(void) const;

	// Get object normal count
	std::size_t GetNormalCount(std::size_t iObject);

	// Get total triangle count
	std::size_t GetTriangleCount(void) const;

	// Get object triangle count
	std::size_t GetTriangleCount(std::size_t iObject);

	// Get total quadrangle count
	std::size_t GetQuadrangleCount(void) const;

	// Get object quadrangle count
	std::size_t GetQuadrangleCount(std::size_t iObject);

	// Get file size
	std::uintmax_t Size(void) const;

	// Close
	void Close(void);

};