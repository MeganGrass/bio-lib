/*
*
*	Megan Grass
*	May 29, 2024
*
*/


#include "bio2_blk.h"


std::uintmax_t Resident_Evil_2_BLK::Open(StdFile& File, std::uintmax_t _Ptr)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil 2 Error: could not read BLK at 0x%llX in \"%ws\"", _Ptr, File.GetPath().filename().wstring().c_str());
		return _Ptr;
	}

	std::uint32_t Count = 0;

	File.Read(_Ptr, &Count, sizeof(std::uint32_t));

	Data.resize(Count);

	return File.Read(_Ptr + sizeof(std::uint32_t), Data.data(), Data.size() * sizeof(Resident_Evil_2_BLK_Data));
}

bool Resident_Evil_2_BLK::Open(std::filesystem::path Path, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Path);

	std::uintmax_t OldPtr = _Ptr;

	_Ptr = Open(m_File, _Ptr);

	return OldPtr != _Ptr;
}

std::uintmax_t Resident_Evil_2_BLK::Save(StdFile& File, std::uintmax_t _Ptr)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Write, true, false))
	{
		Str.Message(L"Resident Evil 2 Error: could not write BLK at 0x%llX in \"%ws\"", _Ptr, File.GetPath().filename().wstring().c_str());
		return _Ptr;
	}

	std::uint32_t Count = static_cast<std::uint32_t>(Data.size());

	File.Write(_Ptr, &Count, sizeof(std::uint32_t));

	return File.Write(_Ptr + sizeof(std::uint32_t), Data.data(), Data.size() * sizeof(Resident_Evil_2_BLK_Data));
}

bool Resident_Evil_2_BLK::Save(std::filesystem::path Path, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Path);

	std::uintmax_t OldPtr = _Ptr;

	_Ptr = Save(m_File, _Ptr);

	return OldPtr != _Ptr;
}

void Resident_Evil_2_BLK::Close(void)
{
	Data.clear();
	Data.shrink_to_fit();
}