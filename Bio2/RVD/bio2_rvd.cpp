/*
*
*	Megan Grass
*	April 20, 2024
*
*/


#include "bio2_rvd.h"


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

void Resident_Evil_2_RVD::GetFrustum(std::uintmax_t CurrentCut, std::int16_t(&Frustum)[4][2])
{
	for (auto& Element : Data)
	{
		if (Element.Fcut == CurrentCut && Element.Tcut == 0)
		{
			Frustum[0][0] = Element.Xz[0][0];
			Frustum[0][1] = Element.Xz[0][1];
			Frustum[1][0] = Element.Xz[1][0];
			Frustum[1][1] = Element.Xz[1][1];
			Frustum[2][0] = Element.Xz[2][0];
			Frustum[2][1] = Element.Xz[2][1];
			Frustum[3][0] = Element.Xz[3][0];
			Frustum[3][1] = Element.Xz[3][1];
			return;
		}
	}
}

std::uintmax_t Resident_Evil_2_RVD::Open(StdFile& File, std::uintmax_t _Ptr)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil 2 Error: could not read RVD at 0x%llX in \"%ws\"", _Ptr, File.GetPath().filename().wstring().c_str());
		return _Ptr;
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

bool Resident_Evil_2_RVD::Open(std::filesystem::path Path, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Path);

	std::uintmax_t OldPtr = _Ptr;

	_Ptr = Open(m_File, _Ptr);

	return OldPtr != _Ptr;
}

std::uintmax_t Resident_Evil_2_RVD::Save(StdFile& File, std::uintmax_t _Ptr)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Write, true, false))
	{
		Str.Message(L"Resident Evil 2 Error: could not write RVD at 0x%llX in \"%ws\"", _Ptr, File.GetPath().filename().wstring().c_str());
		return _Ptr;
	}

	Sort();

	File.Write(_Ptr, Data.data(), Data.size() * sizeof(Resident_Evil_2_RVD_Data));

	std::uint32_t EoF = 0xFFFFFFFF;
	return File.Write(_Ptr + Data.size() * sizeof(Resident_Evil_2_RVD_Data), &EoF, sizeof(uint32_t));
}

bool Resident_Evil_2_RVD::Save(std::filesystem::path Path, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Path);

	std::uintmax_t OldPtr = _Ptr;

	_Ptr = Save(m_File, _Ptr);

	return OldPtr != _Ptr;
}

void Resident_Evil_2_RVD::Close(void)
{
	Data.clear();
	Data.shrink_to_fit();
}