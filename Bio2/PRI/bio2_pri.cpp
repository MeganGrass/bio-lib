/*
*
*	Megan Grass
*	April 20, 2024
*
*
*	TODO:
*
*/


#include "bio2_pri.h"


/*
	Open
*/
std::uintmax_t Resident_Evil_2_PRI::Open(StdFile& File, std::uintmax_t _Ptr)
{
	Standard_String Str;

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read, true, false))
		{
			Str.Message("Resident Evil 2: Error, could not open PRI at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
			return _Ptr;
		}
	}

	struct File_Header
	{
		std::uint16_t nLayer;
		std::uint16_t nTotal;
	};

	File_Header Header{};

	File.Read(_Ptr, &Header, sizeof(File_Header));

	if (!Header.nLayer || (Header.nLayer == 0xFFFF))
	{
		return _Ptr += sizeof(File_Header);
	}

	Data.resize(Header.nLayer);

	std::uintmax_t LayerPtr = _Ptr + sizeof(File_Header);

	_Ptr = LayerPtr + (Data.size() * sizeof(File_Header));

	for (std::size_t i = 0; i < Data.size(); i++)
	{
		File.Read(LayerPtr + (i * sizeof(Resident_Evil_2_PRI_Layer)), &Data[i].Layer, sizeof(Resident_Evil_2_PRI_Layer));

		Data[i].Sprite.resize(Data[i].Layer.nSprite);

		for (std::size_t x = 0; x < Data[i].Sprite.size(); x++)
		{
			File.Read(_Ptr, &Data[i].Sprite[x], sizeof(Resident_Evil_2_PRI_Rect));

			if (!Data[i].Sprite[x].tPage)
			{
				_Ptr += sizeof(Resident_Evil_2_PRI_Rect);
			}
			else
			{
				Resident_Evil_2_PRI_Square Square{};
				File.Read(_Ptr, &Square, sizeof(Resident_Evil_2_PRI_Square));
				_Ptr += sizeof(Resident_Evil_2_PRI_Square);
				Data[i].Sprite[x].u = Square.u;
				Data[i].Sprite[x].v = Square.v;
				Data[i].Sprite[x].x = Square.x;
				Data[i].Sprite[x].y = Square.y;
				Data[i].Sprite[x].otz = Square.otz;
				Data[i].Sprite[x].w = Square.size;
				Data[i].Sprite[x].h = Square.size;
			}
		}
	}

	return _Ptr;
}


/*
	Open
*/
bool Resident_Evil_2_PRI::Open(std::filesystem::path Path, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Path);

	std::uintmax_t OldPtr = _Ptr;

	_Ptr = Open(m_File, _Ptr);

	return OldPtr != _Ptr;
}


/*
	Close
*/
void Resident_Evil_2_PRI::Close(void)
{
	for(auto& Element : Data)
	{
		Element.Sprite.clear();
	}
	Data.clear();
}