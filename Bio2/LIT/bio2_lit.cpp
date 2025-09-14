/*
*
*	Megan Grass
*	May 29, 2024
*
*/


#include "bio2_lit.h"


std::uintmax_t Resident_Evil_2_LIT::Open(StdFile& File, std::uintmax_t nCut, std::uintmax_t _Ptr)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Str.Message(L"Resident Evil 2 Error: could not read LIT at 0x%llX in \"%ws\"", _Ptr, File.GetPath().filename().wstring().c_str());
		return _Ptr;
	}

	Data.resize(nCut);

	return File.Read(_Ptr, Data.data(), Data.size() * sizeof(Resident_Evil_2_LIT_Data));
}

bool Resident_Evil_2_LIT::Open(std::filesystem::path Path, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Path);

	std::uintmax_t OldPtr = _Ptr;

	//_Ptr = Open(m_File, _Ptr);

	return OldPtr != _Ptr;
}

std::uintmax_t Resident_Evil_2_LIT::Save(StdFile& File, std::uintmax_t _Ptr)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Write, true, false))
	{
		Str.Message(L"Resident Evil 2 Error: could not write LIT at 0x%llX in \"%ws\"", _Ptr, File.GetPath().filename().wstring().c_str());
		return _Ptr;
	}

	return File.Write(_Ptr, Data.data(), Data.size() * sizeof(Resident_Evil_2_LIT_Data));
}

bool Resident_Evil_2_LIT::Save(std::filesystem::path Path, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Path);

	std::uintmax_t OldPtr = _Ptr;

	_Ptr = Save(m_File, _Ptr);

	return OldPtr != _Ptr;
}

void Resident_Evil_2_LIT::Close(void)
{
	Data.clear();
	Data.shrink_to_fit();
}