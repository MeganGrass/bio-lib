/*
*
*	Megan Grass
*	March 07, 2024
*
*/


#include "bio3_md2.h"


std::uintmax_t Resident_Evil_3_MD2::Open(StdFile& File, std::uintmax_t _Ptr)
{
	if (b_Open) { Close(); }

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read, true, false))
		{
			Str.Message(L"Resident Evil 3 Model Error: could not open at 0x%llX in \"%ws\"", _Ptr, File.GetPath().filename().wstring().c_str());
			return _Ptr;
		}
	}

	Resident_Evil_3_Model_Header Header{};
	File.Read(_Ptr, &Header, sizeof(Resident_Evil_3_Model_Header));

	if (!Header.Size || !Header.nObject) { return _Ptr; }

	std::vector<Resident_Evil_3_Model_Index> Index(Header.nObject);
	File.Read(_Ptr + sizeof(Resident_Evil_3_Model_Header), Index.data(), Index.size() * sizeof(Resident_Evil_3_Model_Index));

	_Ptr += sizeof(Resident_Evil_3_Model_Header);

	for (std::size_t i = 0; i < Index.size(); i++)
	{
		Resident_Evil_3_Model_Object Obj;

		Obj.Vertice.resize(Index[i].nVertice);
		File.Read(_Ptr + Index[i].pVertice, Obj.Vertice.data(), Obj.Vertice.size() * sizeof(SVECTOR));

		Obj.Normal.resize(Index[i].nVertice);
		File.Read(_Ptr + Index[i].pNormal, Obj.Normal.data(), Obj.Normal.size() * sizeof(SVECTOR));

		Obj.Triangle.resize(Index[i].nTriangle);
		File.Read(_Ptr + Index[i].pTriangle, Obj.Triangle.data(), Obj.Triangle.size() * sizeof(Resident_Evil_3_Triangle));

		Obj.Quadrangle.resize(Index[i].nQuadrangle);
		File.Read(_Ptr + Index[i].pQuadrangle, Obj.Quadrangle.data(), Obj.Quadrangle.size() * sizeof(Resident_Evil_3_Quadrangle));

		Object.push_back(Obj);
	}

	b_Open = true;

	return _Ptr + (Size() - sizeof(Resident_Evil_3_Model_Header));
}

std::uintmax_t Resident_Evil_3_MD2::Save(StdFile& File, std::uintmax_t _Ptr)
{
	if (!b_Open)
	{
		Str.Message(L"Resident Evil 3 Model Error: model is not open");
		return _Ptr;
	}

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read_Ex, true, false))
		{
			if (!File.Open(File.GetPath(), FileAccessMode::Write_Ex, true, true))
			{
				Str.Message(L"Resident Evil 3 Model Error: could not create at 0x%llX in \"%ws\"", _Ptr, File.GetPath().filename().wstring().c_str());
				return _Ptr;
			}
		}
	}

	std::uintmax_t pIndex = _Ptr + sizeof(Resident_Evil_3_Model_Header);
	std::uintmax_t pVertice = pIndex + (sizeof(Resident_Evil_3_Model_Index) * GetObjectCount()) + (sizeof(Resident_Evil_3_Triangle) * GetTriangleCount()) + (sizeof(Resident_Evil_3_Quadrangle) * GetQuadrangleCount());
	std::uintmax_t pNormal = pVertice + (sizeof(SVECTOR) * GetVerticeCount());
	std::uintmax_t FileSize = pNormal + (sizeof(SVECTOR) * GetNormalCount());

	std::uintmax_t pTriangle = pIndex + (sizeof(Resident_Evil_3_Model_Index) * GetObjectCount());

	Resident_Evil_3_Model_Header Header { static_cast<std::uint32_t>(FileSize - _Ptr), static_cast<std::uint32_t>(GetObjectCount()) };
	File.Write(_Ptr, &Header, sizeof(Resident_Evil_3_Model_Header));

	for (std::size_t i = 0; i < GetObjectCount(); i++)
	{
		std::uintmax_t pQuadrangle = pTriangle + (sizeof(Resident_Evil_3_Triangle) * GetTriangleCount(i));

		Resident_Evil_3_Model_Index Index{};
		Index.pVertice = static_cast<std::uint32_t>((pVertice - _Ptr) - sizeof(Resident_Evil_3_Model_Header));
		Index.pNormal = static_cast<std::uint32_t>((pNormal - _Ptr) - sizeof(Resident_Evil_3_Model_Header));
		Index.nVertice = static_cast<std::uint32_t>(Object[i].Vertice.size());
		Index.pTriangle = static_cast<std::uint32_t>((pTriangle - _Ptr) - sizeof(Resident_Evil_3_Model_Header));
		Index.nTriangle = static_cast<std::uint32_t>(Object[i].Triangle.size());
		Index.pQuadrangle = static_cast<std::uint32_t>((pQuadrangle - _Ptr) - sizeof(Resident_Evil_3_Model_Header));
		Index.nQuadrangle = static_cast<std::uint32_t>(Object[i].Quadrangle.size());
		File.Write(pIndex, &Index, sizeof(Resident_Evil_3_Model_Index));
		pIndex += sizeof(Resident_Evil_3_Model_Index);

		File.Write(pVertice, Object[i].Vertice.data(), Object[i].Vertice.size() * sizeof(SVECTOR));
		pVertice += Object[i].Vertice.size() * sizeof(SVECTOR);

		File.Write(pNormal, Object[i].Normal.data(), Object[i].Normal.size() * sizeof(SVECTOR));
		pNormal += Object[i].Normal.size() * sizeof(SVECTOR);

		File.Write(pTriangle, Object[i].Triangle.data(), Object[i].Triangle.size() * sizeof(Resident_Evil_3_Triangle));
		pTriangle += (Object[i].Triangle.size() * sizeof(Resident_Evil_3_Triangle)) + (Object[i].Quadrangle.size() * sizeof(Resident_Evil_3_Quadrangle));

		File.Write(pQuadrangle, Object[i].Quadrangle.data(), Object[i].Quadrangle.size() * sizeof(Resident_Evil_3_Quadrangle));
	}

	return _Ptr + Size();
}

bool Resident_Evil_3_MD2::SaveObject(std::filesystem::path Path, std::size_t iObject)
{
	if (!b_Open)
	{
		Str.Message(L"Resident Evil 3 Model Error: model is not open");
		return false;
	}

	if ((iObject + 1) > Object.size())
	{
		Str.Message(L"Resident Evil 3 Model Error: object index out of range");
		return false;
	}

	StdFile File{ Path, FileAccessMode::Write, true, false };

	if (!File.IsOpen())
	{
		Str.Message(L"Resident Evil 3 Model: Error, could not create \"%ws\"", Path.filename().wstring().c_str());
		return false;
	}

	std::uintmax_t pIndex = sizeof(Resident_Evil_3_Model_Header);
	std::uintmax_t pVertice = pIndex + sizeof(Resident_Evil_3_Model_Index) + (sizeof(Resident_Evil_3_Triangle) * GetTriangleCount(iObject)) + (sizeof(Resident_Evil_3_Quadrangle) * GetQuadrangleCount(iObject));
	std::uintmax_t pNormal = pVertice + (sizeof(SVECTOR) * GetVerticeCount(iObject));
	std::uintmax_t FileSize = pNormal + (sizeof(SVECTOR) * GetNormalCount(iObject));

	std::uintmax_t pTriangle = pIndex + sizeof(Resident_Evil_3_Model_Index);
	std::uintmax_t pQuadrangle = pTriangle + (sizeof(Resident_Evil_3_Triangle) * GetTriangleCount(iObject));

	Resident_Evil_3_Model_Header Header { static_cast<std::uint32_t>(FileSize), 1 };
	File.Write(0, &Header, sizeof(Resident_Evil_3_Model_Header));

	Resident_Evil_3_Model_Index Index{};
	Index.pVertice = static_cast<std::uint32_t>((pVertice) - sizeof(Resident_Evil_3_Model_Header));
	Index.pNormal = static_cast<std::uint32_t>((pNormal) - sizeof(Resident_Evil_3_Model_Header));
	Index.nVertice = static_cast<std::uint32_t>(Object[iObject].Vertice.size());
	Index.pTriangle = static_cast<std::uint32_t>((pTriangle) - sizeof(Resident_Evil_3_Model_Header));
	Index.nTriangle = static_cast<std::uint32_t>(Object[iObject].Triangle.size());
	Index.pQuadrangle = static_cast<std::uint32_t>((pQuadrangle) - sizeof(Resident_Evil_3_Model_Header));
	Index.nQuadrangle = static_cast<std::uint32_t>(Object[iObject].Quadrangle.size());
	File.Write(pIndex, &Index, sizeof(Resident_Evil_3_Model_Index));

	File.Write(pVertice, Object[iObject].Vertice.data(), Object[iObject].Vertice.size() * sizeof(SVECTOR));

	File.Write(pNormal, Object[iObject].Normal.data(), Object[iObject].Normal.size() * sizeof(SVECTOR));

	File.Write(pTriangle, Object[iObject].Triangle.data(), Object[iObject].Triangle.size() * sizeof(Resident_Evil_3_Triangle));

	File.Write(pQuadrangle, Object[iObject].Quadrangle.data(), Object[iObject].Quadrangle.size() * sizeof(Resident_Evil_3_Quadrangle));

	return true;
}

bool Resident_Evil_3_MD2::SaveAllObjects(std::filesystem::path Directory, std::filesystem::path Stem)
{
	if (!b_Open)
	{
		Str.Message(L"Resident Evil 3 Model Error: model is not open");
		return false;
	}

	Standard_FileSystem FS;
	std::filesystem::path Dir = FS.GetDirectory(Directory);
	FS.CreateDirectory(Dir / Stem.stem());

	for (std::size_t i = 0; i < Object.size(); i++)
	{
		std::filesystem::path Output = Str.FormatCStyle(L"%ws\\%ws\\%ws_%02d.md2", Dir.wstring().c_str(), Stem.stem().wstring().c_str(), Stem.stem().wstring().c_str(), i);
		SaveObject(Output, i);
	}

	return true;
}

std::unique_ptr<Sony_PlayStation_Model> Resident_Evil_3_MD2::GetTMD(void)
{
	std::unique_ptr<Sony_PlayStation_Model> Tmd = std::make_unique<Sony_PlayStation_Model>();

	if (Object.empty()) { return Tmd; }

	TMD_P_TG3 PolyGT3{};
	TMD_P_TG4 PolyGT4{};

	std::size_t nPrimitive = 0;

	for (std::size_t i = 0; i < GetObjectCount(); i++)
	{
		Sony_PlayStation_Model_Object Obj;

		Obj.Scale = 0;

		Obj.Vertice.resize(Object[i].Vertice.size());
		std::memcpy(Obj.Vertice.data(), Object[i].Vertice.data(), Object[i].Vertice.size() * sizeof(SVECTOR));

		Obj.Normal.resize(Object[i].Normal.size());
		std::memcpy(Obj.Normal.data(), Object[i].Normal.data(), Object[i].Normal.size() * sizeof(SVECTOR));

		Obj.Primitive.resize(Object[i].Triangle.size() + Object[i].Quadrangle.size());
		nPrimitive = 0;

		for (std::size_t x = 0; x < Object[i].Triangle.size(); x++, nPrimitive++)
		{
			PolyGT3.tag = 0x34000609;
			PolyGT3.tu0 = Object[i].Triangle[x].tu0;
			PolyGT3.tv0 = Object[i].Triangle[x].tv0;
			PolyGT3.clut = Object[i].Triangle[x].Clut;
			PolyGT3.tu1 = Object[i].Triangle[x].tu1;
			PolyGT3.tv1 = Object[i].Triangle[x].tv1;
			PolyGT3.tpage = Object[i].Triangle[x].Page;
			PolyGT3.tu2 = Object[i].Triangle[x].tu2;
			PolyGT3.tv2 = Object[i].Triangle[x].tv2;
			PolyGT3.p = 0;
			PolyGT3.n0 = Object[i].Triangle[x].v0;
			PolyGT3.v0 = Object[i].Triangle[x].v0;
			PolyGT3.n1 = Object[i].Triangle[x].v1;
			PolyGT3.v1 = Object[i].Triangle[x].v1;
			PolyGT3.n2 = Object[i].Triangle[x].v2;
			PolyGT3.v2 = Object[i].Triangle[x].v2;
			Obj.Primitive[nPrimitive].Packet.resize(sizeof(TMD_P_TG3));
			std::memcpy(&Obj.Primitive[nPrimitive].Header, &PolyGT3.tag, sizeof(Sony_PlayStation_Model_Primitive::Header));
			std::memcpy(Obj.Primitive[nPrimitive].Packet.data(), &PolyGT3, sizeof(TMD_P_TG3));
		}

		for (std::size_t x = 0; x < Object[i].Quadrangle.size(); x++, nPrimitive++)
		{
			PolyGT4.tag = 0x3C00080C;
			PolyGT4.tu0 = Object[i].Quadrangle[x].tu0;
			PolyGT4.tv0 = Object[i].Quadrangle[x].tv0;
			PolyGT4.clut = Object[i].Quadrangle[x].Clut;
			PolyGT4.tu1 = Object[i].Quadrangle[x].tu1;
			PolyGT4.tv1 = Object[i].Quadrangle[x].tv1;
			PolyGT4.tpage = Object[i].Quadrangle[x].Page;
			PolyGT4.tu2 = Object[i].Quadrangle[x].tu2;
			PolyGT4.tv2 = Object[i].Quadrangle[x].tv2;
			PolyGT4.p0 = 0;
			PolyGT4.tu3 = Object[i].Quadrangle[x].tu3;
			PolyGT4.tv3 = Object[i].Quadrangle[x].tv3;
			PolyGT4.p1 = 0;
			PolyGT4.n0 = Object[i].Quadrangle[x].v0;
			PolyGT4.v0 = Object[i].Quadrangle[x].v0;
			PolyGT4.n1 = Object[i].Quadrangle[x].v1;
			PolyGT4.v1 = Object[i].Quadrangle[x].v1;
			PolyGT4.n2 = Object[i].Quadrangle[x].v2;
			PolyGT4.v2 = Object[i].Quadrangle[x].v2;
			PolyGT4.n3 = Object[i].Quadrangle[x].v3;
			PolyGT4.v3 = Object[i].Quadrangle[x].v3;
			Obj.Primitive[nPrimitive].Packet.resize(sizeof(TMD_P_TG4));
			std::memcpy(&Obj.Primitive[nPrimitive].Header, &PolyGT4.tag, sizeof(Sony_PlayStation_Model_Primitive::Header));
			std::memcpy(Obj.Primitive[nPrimitive].Packet.data(), &PolyGT4, sizeof(TMD_P_TG4));
		}

		Tmd->AddObject(Obj);
	}

	Tmd->ForceOpen();

	return Tmd;
}

std::size_t Resident_Evil_3_MD2::GetVerticeCount(void) const
{
	if (!b_Open) { return 0; }

	std::size_t nVertice = 0;

	for (std::size_t i = 0; i < Object.size(); i++)
	{
		nVertice += Object[i].Vertice.size();
	}

	return nVertice;
}

std::size_t Resident_Evil_3_MD2::GetVerticeCount(std::size_t iObject)
{
	if (!b_Open) { return 0; }

	if ((iObject + 1) > Object.size())
	{
		Str.Message(L"Resident Evil 3 Model Error: object index out of range");
		return 0;
	}

	return Object[iObject].Vertice.size();
}

std::size_t Resident_Evil_3_MD2::GetNormalCount(void) const
{
	if (!b_Open) { return 0; }

	std::size_t nNormal = 0;

	for (std::size_t i = 0; i < Object.size(); i++)
	{
		nNormal += Object[i].Normal.size();
	}

	return nNormal;
}

std::size_t Resident_Evil_3_MD2::GetNormalCount(std::size_t iObject)
{
	if (!b_Open) { return 0; }

	if ((iObject + 1) > Object.size())
	{
		Str.Message(L"Resident Evil 3 Model Error: object index out of range");
		return 0;
	}

	return Object[iObject].Normal.size();
}

std::size_t Resident_Evil_3_MD2::GetTriangleCount(void) const
{
	if (!b_Open) { return 0; }

	std::size_t nTriangle = 0;

	for (std::size_t i = 0; i < Object.size(); i++)
	{
		nTriangle += Object[i].Triangle.size();
	}

	return nTriangle;
}

std::size_t Resident_Evil_3_MD2::GetTriangleCount(std::size_t iObject)
{
	if (!b_Open) { return 0; }

	if ((iObject + 1) > Object.size())
	{
		Str.Message(L"Resident Evil 3 Model Error: object index out of range");
		return 0;
	}

	return Object[iObject].Triangle.size();
}

std::size_t Resident_Evil_3_MD2::GetQuadrangleCount(void) const
{
	if (!b_Open) { return 0; }

	std::size_t nQuadrangle = 0;

	for (std::size_t i = 0; i < Object.size(); i++)
	{
		nQuadrangle += Object[i].Quadrangle.size();
	}

	return nQuadrangle;

}

std::size_t Resident_Evil_3_MD2::GetQuadrangleCount(std::size_t iObject)
{
	if (!b_Open) { return 0; }

	if ((iObject + 1) > Object.size())
	{
		Str.Message(L"Resident Evil 3 Model Error: object index out of range");
		return 0;
	}

	return Object[iObject].Quadrangle.size();
}

std::uintmax_t Resident_Evil_3_MD2::Size(void) const
{
	if (!b_Open) { return 0; }

	std::uintmax_t pIndex = sizeof(Resident_Evil_3_Model_Header);
	std::uintmax_t pVertice = pIndex + (sizeof(Resident_Evil_3_Model_Index) * GetObjectCount()) + (sizeof(Resident_Evil_3_Triangle) * GetTriangleCount()) + (sizeof(Resident_Evil_3_Quadrangle) * GetQuadrangleCount());
	std::uintmax_t pNormal = pVertice + (sizeof(SVECTOR) * GetVerticeCount());
	std::uintmax_t Size = pNormal + (sizeof(SVECTOR) * GetNormalCount());

	return Size;
}

void Resident_Evil_3_MD2::Close(void)
{
	b_Open = false;
	for (std::size_t i = 0; i < Object.size(); i++)
	{
		Object[i].Vertice.clear();
		Object[i].Normal.clear();
		Object[i].Triangle.clear();
		Object[i].Quadrangle.clear();
	}
	Object.clear();
}