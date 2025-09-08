/*
*
*	Megan Grass
*	March 07, 2024
*
*/


#include "bio2_md1.h"


std::uintmax_t Resident_Evil_2_MD1::Open(StdFile& File, std::uintmax_t _Ptr)
{
	if (b_Open) { Close(); }

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read, true, false))
		{
			Str.Message(L"Resident Evil 2 Model Error: could not open at 0x%llX in \"%ws\"", _Ptr, File.GetPath().filename().wstring().c_str());
			return _Ptr;
		}
	}

	Resident_Evil_2_Model_Header Header;
	File.Read(_Ptr , &Header, sizeof(Resident_Evil_2_Model_Header));

	if (!Header.nObject) { return _Ptr; }

	std::vector<Resident_Evil_2_Model_Index> Index(Header.nObject / 2);
	File.Read(_Ptr + sizeof(Resident_Evil_2_Model_Header), Index.data(), Index.size() * sizeof(Resident_Evil_2_Model_Index));

	_Ptr += sizeof(Resident_Evil_2_Model_Header);

	for (std::size_t i = 0; i < Index.size(); i++)
	{
		Resident_Evil_2_Model_Object Obj;

		Obj.Vertice.resize(Index[i].Tri.nVertice);
		File.Read(_Ptr + Index[i].Tri.pVertice, Obj.Vertice.data(), Obj.Vertice.size() * sizeof(SVECTOR));

		Obj.Normal.resize(Index[i].Tri.nNormal);
		File.Read(_Ptr + Index[i].Tri.pNormal, Obj.Normal.data(), Obj.Normal.size() * sizeof(SVECTOR));

		Obj.Triangle.resize(Index[i].Tri.nPrimitive);
		for (std::size_t x = 0; x < Obj.Triangle.size(); x++)
		{
			File.Read(_Ptr + Index[i].Tri.pPrimitive + (x * sizeof(Resident_Evil_2_Triangle_Primitive)), &Obj.Triangle[x].Primitive, sizeof(Resident_Evil_2_Triangle_Primitive));
			File.Read(_Ptr + Index[i].Tri.pTexture + (x * sizeof(Resident_Evil_2_Triangle_Texture)), &Obj.Triangle[x].Texture, sizeof(Resident_Evil_2_Triangle_Texture));
		}

		Obj.Quadrangle.resize(Index[i].Quad.nPrimitive);
		for (std::size_t x = 0; x < Obj.Quadrangle.size(); x++)
		{
			File.Read(_Ptr + Index[i].Quad.pPrimitive + (x * sizeof(Resident_Evil_2_Quadrangle_Primitive)), &Obj.Quadrangle[x].Primitive, sizeof(Resident_Evil_2_Quadrangle_Primitive));
			File.Read(_Ptr + Index[i].Quad.pTexture + (x * sizeof(Resident_Evil_2_Quadrangle_Texture)), &Obj.Quadrangle[x].Texture, sizeof(Resident_Evil_2_Quadrangle_Texture));
		}

		Object.push_back(Obj);
	}

	b_Open = true;

	return _Ptr + (Size() - sizeof(Resident_Evil_2_Model_Header));
}

std::uintmax_t Resident_Evil_2_MD1::Save(StdFile& File, std::uintmax_t _Ptr)
{
	if (!b_Open)
	{
		Str.Message(L"Resident Evil 2 Model Error: model is not open");
		return _Ptr;
	}

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read_Ex, true, false))
		{
			if (!File.Open(File.GetPath(), FileAccessMode::Write_Ex, true, true))
			{
				Str.Message(L"Resident Evil 2 Model Error: could not create at 0x%llX in \"%ws\"", _Ptr, File.GetPath().filename().wstring().c_str());
				return _Ptr;
			}
		}
	}

	std::uintmax_t pIndex = _Ptr + sizeof(Resident_Evil_2_Model_Header);
	std::uintmax_t pVertice = pIndex + (sizeof(Resident_Evil_2_Model_Index) * GetObjectCount());
	std::uintmax_t pNormal = pVertice + (sizeof(SVECTOR) * GetVerticeCount());
	std::uintmax_t pPrimitive = pNormal + (sizeof(SVECTOR) * GetNormalCount());
	std::uintmax_t pTexture = pPrimitive + (sizeof(Resident_Evil_2_Triangle_Primitive) * GetTriangleCount()) + (sizeof(Resident_Evil_2_Quadrangle_Primitive) * GetQuadrangleCount());
	//std::uintmax_t Size = pTexture + (sizeof(Resident_Evil_2_Triangle_Texture) * GetTriangleCount()) + (sizeof(Resident_Evil_2_Quadrangle_Texture) * GetQuadrangleCount());

	Resident_Evil_2_Model_Header Header { static_cast<std::uint32_t>(pTexture - _Ptr), 0, static_cast<std::uint32_t>(GetObjectCount() * 2) };
	File.Write(_Ptr, &Header, sizeof(Resident_Evil_2_Model_Header));

	for (std::size_t i = 0; i < GetObjectCount(); i++)
	{
		Resident_Evil_2_Model_Index Index{};
		Index.Tri.pVertice = static_cast<std::uint32_t>((pVertice - _Ptr) - sizeof(Resident_Evil_2_Model_Header));
		Index.Tri.nVertice = static_cast<std::uint32_t>(Object[i].Vertice.size());
		Index.Tri.pNormal = static_cast<std::uint32_t>((pNormal - _Ptr) - sizeof(Resident_Evil_2_Model_Header));
		Index.Tri.nNormal = static_cast<std::uint32_t>(Object[i].Normal.size());
		Index.Tri.pPrimitive = static_cast<std::uint32_t>((pPrimitive - _Ptr) - sizeof(Resident_Evil_2_Model_Header));
		Index.Tri.nPrimitive = static_cast<std::uint32_t>(Object[i].Triangle.size());
		Index.Tri.pTexture = static_cast<std::uint32_t>((pTexture - _Ptr) - sizeof(Resident_Evil_2_Model_Header));
		Index.Quad.pVertice = static_cast<std::uint32_t>((pVertice - _Ptr) - sizeof(Resident_Evil_2_Model_Header));
		Index.Quad.nVertice = static_cast<std::uint32_t>(Object[i].Vertice.size());
		Index.Quad.pNormal = static_cast<std::uint32_t>((pNormal - _Ptr) - sizeof(Resident_Evil_2_Model_Header));
		Index.Quad.nNormal = static_cast<std::uint32_t>(Object[i].Normal.size());
		Index.Quad.pPrimitive = static_cast<std::uint32_t>(((pPrimitive - _Ptr) + (sizeof(Resident_Evil_2_Triangle_Primitive) * Object[i].Triangle.size())) - sizeof(Resident_Evil_2_Model_Header));
		Index.Quad.nPrimitive = static_cast<std::uint32_t>(Object[i].Quadrangle.size());
		Index.Quad.pTexture = static_cast<std::uint32_t>(((pTexture - _Ptr) + (sizeof(Resident_Evil_2_Triangle_Texture) * Object[i].Triangle.size())) - sizeof(Resident_Evil_2_Model_Header));
		File.Write(pIndex, &Index, sizeof(Resident_Evil_2_Model_Index));
		pIndex += sizeof(Resident_Evil_2_Model_Index);
		
		File.Write(pVertice, Object[i].Vertice.data(), Object[i].Vertice.size() * sizeof(SVECTOR));
		pVertice += Object[i].Vertice.size() * sizeof(SVECTOR);

		File.Write(pNormal, Object[i].Normal.data(), Object[i].Normal.size() * sizeof(SVECTOR));
		pNormal += Object[i].Normal.size() * sizeof(SVECTOR);

		for (std::size_t x = 0; x < Object[i].Triangle.size(); x++, pPrimitive += sizeof(Resident_Evil_2_Triangle_Primitive), pTexture += sizeof(Resident_Evil_2_Triangle_Texture))
		{
			File.Write(pPrimitive, &Object[i].Triangle[x].Primitive, sizeof(Resident_Evil_2_Triangle_Primitive));
			File.Write(pTexture, &Object[i].Triangle[x].Texture, sizeof(Resident_Evil_2_Triangle_Texture));
		}

		for (std::size_t x = 0; x < Object[i].Quadrangle.size(); x++, pPrimitive += sizeof(Resident_Evil_2_Quadrangle_Primitive), pTexture += sizeof(Resident_Evil_2_Quadrangle_Texture))
		{
			File.Write(pPrimitive, &Object[i].Quadrangle[x].Primitive, sizeof(Resident_Evil_2_Quadrangle_Primitive));
			File.Write(pTexture, &Object[i].Quadrangle[x].Texture, sizeof(Resident_Evil_2_Quadrangle_Texture));
		}
	}

	return _Ptr + Size();
}

bool Resident_Evil_2_MD1::SaveObject(std::filesystem::path Path, std::size_t iObject)
{
	if (!b_Open)
	{
		Str.Message(L"Resident Evil 2 Model Error: model is not open");
		return false;
	}

	if ((iObject + 1) > Object.size())
	{
		Str.Message(L"Resident Evil 2 Model Error: object index out of range");
		return false;
	}

	StdFile File { Path, FileAccessMode::Write, true, false };

	if (!File.IsOpen())
	{
		Str.Message(L"Resident Evil 2 Model: Error, could not create \"%ws\"", Path.filename().wstring().c_str());
		return false;
	}

	std::uintmax_t pIndex = sizeof(Resident_Evil_2_Model_Header);
	std::uintmax_t pVertice = pIndex + sizeof(Resident_Evil_2_Model_Index);
	std::uintmax_t pNormal = pVertice + (sizeof(SVECTOR) * GetVerticeCount(iObject));
	std::uintmax_t pPrimitive = pNormal + (sizeof(SVECTOR) * GetNormalCount(iObject));
	std::uintmax_t pTexture = pPrimitive + (sizeof(Resident_Evil_2_Triangle_Primitive) * GetTriangleCount(iObject)) + (sizeof(Resident_Evil_2_Quadrangle_Primitive) * GetQuadrangleCount(iObject));
	std::uintmax_t Size = pTexture + (sizeof(Resident_Evil_2_Triangle_Texture) * GetTriangleCount(iObject)) + (sizeof(Resident_Evil_2_Quadrangle_Texture) * GetQuadrangleCount(iObject));

	Resident_Evil_2_Model_Header Header { static_cast<std::uint32_t>(pTexture), 0, 2 };

	File.Write(0, &Header, sizeof(Resident_Evil_2_Model_Header));

	Resident_Evil_2_Model_Index Index{};
	Index.Tri.pVertice = static_cast<std::uint32_t>(pVertice - sizeof(Resident_Evil_2_Model_Header));
	Index.Tri.nVertice = static_cast<std::uint32_t>(Object[iObject].Vertice.size());
	Index.Tri.pNormal = static_cast<std::uint32_t>(pNormal - sizeof(Resident_Evil_2_Model_Header));
	Index.Tri.nNormal = static_cast<std::uint32_t>(Object[iObject].Normal.size());
	Index.Tri.pPrimitive = static_cast<std::uint32_t>(pPrimitive - sizeof(Resident_Evil_2_Model_Header));
	Index.Tri.nPrimitive = static_cast<std::uint32_t>(Object[iObject].Triangle.size());
	Index.Tri.pTexture = static_cast<std::uint32_t>(pTexture - sizeof(Resident_Evil_2_Model_Header));
	Index.Quad.pVertice = static_cast<std::uint32_t>(pVertice - sizeof(Resident_Evil_2_Model_Header));
	Index.Quad.nVertice = static_cast<std::uint32_t>(Object[iObject].Vertice.size());
	Index.Quad.pNormal = static_cast<std::uint32_t>(pNormal - sizeof(Resident_Evil_2_Model_Header));
	Index.Quad.nNormal = static_cast<std::uint32_t>(Object[iObject].Normal.size());
	Index.Quad.pPrimitive = static_cast<std::uint32_t>((pPrimitive + (sizeof(Resident_Evil_2_Triangle_Primitive) * Object[iObject].Triangle.size())) - sizeof(Resident_Evil_2_Model_Header));
	Index.Quad.nPrimitive = static_cast<std::uint32_t>(Object[iObject].Quadrangle.size());
	Index.Quad.pTexture = static_cast<std::uint32_t>((pTexture + (sizeof(Resident_Evil_2_Triangle_Texture) * Object[iObject].Triangle.size())) - sizeof(Resident_Evil_2_Model_Header));

	File.Write(pIndex, &Index, sizeof(Resident_Evil_2_Model_Index));

	File.Write(pVertice, Object[iObject].Vertice.data(), Object[iObject].Vertice.size() * sizeof(SVECTOR));

	File.Write(pNormal, Object[iObject].Normal.data(), Object[iObject].Normal.size() * sizeof(SVECTOR));

	for (std::size_t x = 0; x < Object[iObject].Triangle.size(); x++, pPrimitive += sizeof(Resident_Evil_2_Triangle_Primitive), pTexture += sizeof(Resident_Evil_2_Triangle_Texture))
	{
		File.Write(pPrimitive, &Object[iObject].Triangle[x].Primitive, sizeof(Resident_Evil_2_Triangle_Primitive));
		File.Write(pTexture, &Object[iObject].Triangle[x].Texture, sizeof(Resident_Evil_2_Triangle_Texture));
	}

	for (std::size_t x = 0; x < Object[iObject].Quadrangle.size(); x++, pPrimitive += sizeof(Resident_Evil_2_Quadrangle_Primitive), pTexture += sizeof(Resident_Evil_2_Quadrangle_Texture))
	{
		File.Write(pPrimitive, &Object[iObject].Quadrangle[x].Primitive, sizeof(Resident_Evil_2_Quadrangle_Primitive));
		File.Write(pTexture, &Object[iObject].Quadrangle[x].Texture, sizeof(Resident_Evil_2_Quadrangle_Texture));
	}

	return true;
}

bool Resident_Evil_2_MD1::SaveAllObjects(std::filesystem::path Directory, std::filesystem::path Stem)
{
	if (!b_Open)
	{
		Str.Message(L"Resident Evil 2 Model Error: model is not open");
		return false;
	}

	Standard_FileSystem FS;
	std::filesystem::path Dir = FS.GetDirectory(Directory);
	FS.CreateDirectory(Dir / Stem.stem());

	for (std::size_t i = 0; i < Object.size(); i++)
	{
		std::filesystem::path Output = Str.FormatCStyle(L"%ws\\%ws\\%ws_%02d.md1", Dir.wstring().c_str(), Stem.stem().wstring().c_str(), Stem.stem().wstring().c_str(), i);
		SaveObject(Output, i);
	}

	return true;
}

std::unique_ptr<Sony_PlayStation_Model> Resident_Evil_2_MD1::GetTMD(void)
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
			PolyGT3.tu0 = Object[i].Triangle[x].Texture.tu0;
			PolyGT3.tv0 = Object[i].Triangle[x].Texture.tv0;
			PolyGT3.clut = Object[i].Triangle[x].Texture.clut;
			PolyGT3.tu1 = Object[i].Triangle[x].Texture.tu1;
			PolyGT3.tv1 = Object[i].Triangle[x].Texture.tv1;
			PolyGT3.tpage = Object[i].Triangle[x].Texture.tpage;
			PolyGT3.tu2 = Object[i].Triangle[x].Texture.tu2;
			PolyGT3.tv2 = Object[i].Triangle[x].Texture.tv2;
			PolyGT3.p = Object[i].Triangle[x].Texture.pad;
			PolyGT3.n0 = Object[i].Triangle[x].Primitive.n0;
			PolyGT3.v0 = Object[i].Triangle[x].Primitive.v0;
			PolyGT3.n1 = Object[i].Triangle[x].Primitive.n1;
			PolyGT3.v1 = Object[i].Triangle[x].Primitive.v1;
			PolyGT3.n2 = Object[i].Triangle[x].Primitive.n2;
			PolyGT3.v2 = Object[i].Triangle[x].Primitive.v2;
			Obj.Primitive[nPrimitive].Packet.resize(sizeof(TMD_P_TG3));
			std::memcpy(&Obj.Primitive[nPrimitive].Header, &PolyGT3.tag, sizeof(Sony_PlayStation_Model_Primitive::Header));
			std::memcpy(Obj.Primitive[nPrimitive].Packet.data(), &PolyGT3, sizeof(TMD_P_TG3));
		}

		for (std::size_t x = 0; x < Object[i].Quadrangle.size(); x++, nPrimitive++)
		{
			PolyGT4.tag = 0x3C00080C;
			PolyGT4.tu0 = Object[i].Quadrangle[x].Texture.tu0;
			PolyGT4.tv0 = Object[i].Quadrangle[x].Texture.tv0;
			PolyGT4.clut = Object[i].Quadrangle[x].Texture.clut;
			PolyGT4.tu1 = Object[i].Quadrangle[x].Texture.tu1;
			PolyGT4.tv1 = Object[i].Quadrangle[x].Texture.tv1;
			PolyGT4.tpage = Object[i].Quadrangle[x].Texture.tpage;
			PolyGT4.tu2 = Object[i].Quadrangle[x].Texture.tu2;
			PolyGT4.tv2 = Object[i].Quadrangle[x].Texture.tv2;
			PolyGT4.p0 = Object[i].Quadrangle[x].Texture.pad0;
			PolyGT4.tu3 = Object[i].Quadrangle[x].Texture.tu3;
			PolyGT4.tv3 = Object[i].Quadrangle[x].Texture.tv3;
			PolyGT4.p1 = Object[i].Quadrangle[x].Texture.pad1;
			PolyGT4.n0 = Object[i].Quadrangle[x].Primitive.n0;
			PolyGT4.v0 = Object[i].Quadrangle[x].Primitive.v0;
			PolyGT4.n1 = Object[i].Quadrangle[x].Primitive.n1;
			PolyGT4.v1 = Object[i].Quadrangle[x].Primitive.v1;
			PolyGT4.n2 = Object[i].Quadrangle[x].Primitive.n2;
			PolyGT4.v2 = Object[i].Quadrangle[x].Primitive.v2;
			PolyGT4.n3 = Object[i].Quadrangle[x].Primitive.n3;
			PolyGT4.v3 = Object[i].Quadrangle[x].Primitive.v3;
			Obj.Primitive[nPrimitive].Packet.resize(sizeof(TMD_P_TG4));
			std::memcpy(&Obj.Primitive[nPrimitive].Header, &PolyGT4.tag, sizeof(Sony_PlayStation_Model_Primitive::Header));
			std::memcpy(Obj.Primitive[nPrimitive].Packet.data(), &PolyGT4, sizeof(TMD_P_TG4));
		}

		Tmd->AddObject(Obj);
	}

	Tmd->ForceOpen();

	return Tmd;
}

std::size_t Resident_Evil_2_MD1::GetVerticeCount(void) const
{
	if (!b_Open) { return 0; }

	std::size_t nVertice = 0;

	for (std::size_t i = 0; i < Object.size(); i++)
	{
		nVertice += Object[i].Vertice.size();
	}

	return nVertice;
}

std::size_t Resident_Evil_2_MD1::GetNormalCount(void) const
{
	if (!b_Open) { return 0; }

	std::size_t nNormal = 0;

	for (std::size_t i = 0; i < Object.size(); i++)
	{
		nNormal += Object[i].Normal.size();
	}

	return nNormal;
}

std::size_t Resident_Evil_2_MD1::GetTriangleCount(void) const
{
	if (!b_Open) { return 0; }

	std::size_t nTriangle = 0;

	for (std::size_t i = 0; i < Object.size(); i++)
	{
		nTriangle += Object[i].Triangle.size();
	}

	return nTriangle;
}

std::size_t Resident_Evil_2_MD1::GetQuadrangleCount(void) const
{
	if (!b_Open) { return 0; }

	std::size_t nQuadrangle = 0;

	for (std::size_t i = 0; i < Object.size(); i++)
	{
		nQuadrangle += Object[i].Quadrangle.size();
	}

	return nQuadrangle;

}

std::uintmax_t Resident_Evil_2_MD1::Size(void) const
{
	if (!b_Open) { return 0; }

	std::uintmax_t pIndex = sizeof(Resident_Evil_2_Model_Header);
	std::uintmax_t pVertice = pIndex + (sizeof(Resident_Evil_2_Model_Index) * GetObjectCount());
	std::uintmax_t pNormal = pVertice + (sizeof(SVECTOR) * GetVerticeCount());
	std::uintmax_t pPrimitive = pNormal + (sizeof(SVECTOR) * GetNormalCount());
	std::uintmax_t pTexture = pPrimitive + (sizeof(Resident_Evil_2_Triangle_Primitive) * GetTriangleCount()) + (sizeof(Resident_Evil_2_Quadrangle_Primitive) * GetQuadrangleCount());
	std::uintmax_t Size = pTexture + (sizeof(Resident_Evil_2_Triangle_Texture) * GetTriangleCount()) + (sizeof(Resident_Evil_2_Quadrangle_Texture) * GetQuadrangleCount());

	return Size;
}

void Resident_Evil_2_MD1::Close(void)
{
	b_Open = false;
	for (std::size_t i = 0; i < Object.size(); i++)
	{
		Object[i].Vertice.clear();
		Object[i].Vertice.shrink_to_fit();
		Object[i].Normal.clear();
		Object[i].Normal.shrink_to_fit();
		Object[i].Triangle.clear();
		Object[i].Triangle.shrink_to_fit();
		Object[i].Quadrangle.clear();
		Object[i].Quadrangle.shrink_to_fit();
	}
	Object.clear();
	Object.shrink_to_fit();
}