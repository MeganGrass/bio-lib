/*
*
*	Megan Grass
*	April 22, 2024
*
*/


#include "bio2_pri.h"


std::uintmax_t Resident_Evil_2_PRI::Open(StdFile& File, std::uintmax_t _Ptr, bool b_OldData, bool b_Bio1, bool b_Bio2Nov96)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil 2 Error: could not read PRI at 0x%llX in \"%ws\"", _Ptr, File.GetPath().filename().wstring().c_str());
		return _Ptr;
	}

	struct HEADER
	{
		std::uint16_t nGroup;
		std::uint16_t nTotal;
	} Header;

	File.Read(_Ptr, &Header, sizeof(HEADER));

	if (!Header.nGroup || (Header.nGroup == 0xFFFF)) { return _Ptr += sizeof(HEADER); }

	Data.resize(Header.nGroup);

	std::uintmax_t pGroup = _Ptr + sizeof(HEADER);

	_Ptr = pGroup + (Data.size() * sizeof(Resident_Evil_2_PRI_Group));

	for (std::size_t i = 0; i < Data.size(); i++)
	{
		File.Read(pGroup + (i * sizeof(Resident_Evil_2_PRI_Group)), &Data[i].Group, sizeof(Resident_Evil_2_PRI_Group));

		Data[i].Sprite.resize(Data[i].Group.nSprite);

		for (std::size_t x = 0; x < Data[i].Sprite.size(); x++)
		{
			File.Read(_Ptr, &Data[i].Sprite[x], sizeof(Resident_Evil_2_PRI_Rect));

			uint16_t Flag = Data[i].Sprite[x].flag;

			if (b_OldData)
			{
				Flag = (Flag >> 8);
			}

			if (Flag == 0)
			{
				_Ptr += sizeof(Resident_Evil_2_PRI_Rect);
			}
			else
			{
				_Ptr += 0x08;

				if (b_OldData)
				{
					Data[i].Sprite[x].w = (Data[i].Sprite[x].flag >> 8) / 2;
					Data[i].Sprite[x].h = (Data[i].Sprite[x].flag >> 8) / 2;
				}
				else
				{
					Data[i].Sprite[x].w = Data[i].Sprite[x].flag;
					Data[i].Sprite[x].h = Data[i].Sprite[x].flag;
				}
			}

			Data[i].Sprite[x].flag = 0;

			if (b_Bio1)
			{
				Data[i].Sprite[x].otz = ((Data[i].Sprite[x].otz * 4) / 9);
			}

			if (b_Bio2Nov96)
			{
				Data[i].Sprite[x].otz = ((Data[i].Sprite[x].otz * 190) / 100);
			}
		}
	}

	return _Ptr;
}