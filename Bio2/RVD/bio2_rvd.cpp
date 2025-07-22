/*
*
*	Megan Grass
*	April 20, 2024
*
*
*	TODO:
*
*/


#include "bio2_rvd.h"


/*
	Sort
*/
void Resident_Evil_2_RVD::Sort(void)
{
	if (Data.empty()) { return; }

	std::vector<Resident_Evil_2_RVD_Data> Buffer;

	std::uint8_t FcutHi = 0;
	for (size_t i = 0; i < Data.size(); i++)
	{
		if (Data[i].Fcut >= FcutHi) { FcutHi = Data[i].Fcut; }
	}

	std::uint8_t iCut = 0;
	for (uint8_t i = 0; i < FcutHi + 1; i++, iCut++)
	{
		for (size_t x = 0; x < Data.size(); x++)
		{
			if (Data[x].Fcut == iCut) { Buffer.push_back(Data[x]); }
		}
	}

	for (size_t ID = 0; ID < Data.size(); ID++)
	{
		Data.at(ID) = Buffer.at(ID);
	}
}


/*
	Open
*/
std::uintmax_t Resident_Evil_2_RVD::Open(StdFile& File, std::uintmax_t _Ptr)
{
	Standard_String Str;

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read, true, false))
		{
			Str.Message("Resident Evil 2: Error, could not open RVD at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
			return _Ptr;
		}
	}

	Resident_Evil_2_RVD_Data Vcut{};

	std::uintmax_t pData = 0;

	do {

		pData = (_Ptr + (Data.size() * sizeof(Resident_Evil_2_RVD_Data)));

		File.Read(pData, &Vcut, sizeof(Resident_Evil_2_RVD_Data));

		if ((Vcut.Fcut != 0xFF) && (Vcut.Tcut != 0xFF))
		{
			Data.push_back(Vcut);
		}
		else
		{
			break;
		}

	} while ((Vcut.Fcut != 0xFF) && (Vcut.Tcut != 0xFF));

	Sort();

	return _Ptr + (Data.size() * sizeof(Resident_Evil_2_RVD_Data)) + sizeof(std::uint32_t);
}


/*
	Open
*/
bool Resident_Evil_2_RVD::Open(std::filesystem::path Path, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Path);

	std::uintmax_t OldPtr = _Ptr;

	_Ptr = Open(m_File, _Ptr);

	return OldPtr != _Ptr;
}


/*
	Save
*/
std::uintmax_t Resident_Evil_2_RVD::Save(StdFile& File, std::uintmax_t _Ptr)
{
	Standard_String Str;

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Write, true, false))
		{
			Str.Message("Resident Evil 2: Error, could not create RVD at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
			return _Ptr;
		}
	}

	Sort();

	File.Write(_Ptr, Data.data(), Data.size() * sizeof(Resident_Evil_2_RVD_Data));

	std::uint32_t EoF = 0xFFFFFFFF;
	return File.Write(_Ptr + Data.size() * sizeof(Resident_Evil_2_RVD_Data), &EoF, sizeof(uint32_t));
}


/*
	Save
*/
bool Resident_Evil_2_RVD::Save(std::filesystem::path Path, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Path);

	std::uintmax_t OldPtr = _Ptr;

	_Ptr = Save(m_File, _Ptr);

	return OldPtr != _Ptr;
}


/*
	Close
*/
void Resident_Evil_2_RVD::Close(void)
{
	Data.clear();
}