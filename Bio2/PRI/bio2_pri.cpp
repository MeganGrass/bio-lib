/*
*
*	Megan Grass
*	April 20, 2024
*
*/


#include "bio2_pri.h"


std::uintmax_t Resident_Evil_2_PRI::Open(StdFile& File, std::uintmax_t _Ptr)
{
	Standard_String Str;

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read, true, false))
		{
			Str.Message("LResident Evil 2 PRI Error: could not open at 0x%llX in \"%ws\"", _Ptr, File.GetPath().filename().wstring().c_str());
			return _Ptr;
		}
	}

	struct HEADER
	{
		std::uint16_t nLayer;
		std::uint16_t nTotal;
	} Header;

	File.Read(_Ptr, &Header, sizeof(HEADER));

	if (!Header.nLayer || (Header.nLayer == 0xFFFF)) { return _Ptr += sizeof(HEADER); }

	Data.resize(Header.nLayer);

	std::uintmax_t LayerPtr = _Ptr + sizeof(HEADER);

	_Ptr = LayerPtr + (Data.size() * sizeof(Resident_Evil_2_PRI_Layer));

	for (std::size_t i = 0; i < Data.size(); i++)
	{
		File.Read(LayerPtr + (i * sizeof(Resident_Evil_2_PRI_Layer)), &Data[i].Layer, sizeof(Resident_Evil_2_PRI_Layer));

		Data[i].Sprite.resize(Data[i].Layer.nSprite);

		for (std::size_t x = 0; x < Data[i].Sprite.size(); x++)
		{
			File.Read(_Ptr, &Data[i].Sprite[x], sizeof(Resident_Evil_2_PRI_Rect));

			if (!Data[i].Sprite[x].tpage)
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
				Data[i].Sprite[x].tpage = 0;
				Data[i].Sprite[x].w = Square.size;
				Data[i].Sprite[x].h = Square.size;
			}
		}
	}

	return _Ptr;
}