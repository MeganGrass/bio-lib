/*
*
*	Megan Grass
*	April 20, 2024
*
*
*	TODO: 
*
*/


#include "bio2_rid.h"


/*
	Open
*/
std::uintmax_t Resident_Evil_2_RID::Open(StdFile& File, std::size_t nCut, std::uintmax_t _Ptr)
{
	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read, true, false))
		{
			Str->Message("Resident Evil 2: Error, could not open RID at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
			return _Ptr;
		}
	}

	Data.resize(nCut);

	return File.Read(_Ptr, Data.data(), Data.size() * sizeof(Resident_Evil_2_RID_Data));
}


/*
	Open
*/
bool Resident_Evil_2_RID::Open(std::filesystem::path Path, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Path);

	std::uintmax_t OldPtr = _Ptr;

	//_Ptr = Open(m_File, _Ptr);

	return OldPtr != _Ptr;
}


/*
	Save
*/
std::uintmax_t Resident_Evil_2_RID::Save(StdFile& File, std::uintmax_t _Ptr)
{
	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Write, true, false))
		{
			Str->Message("Resident Evil 2: Error, could not create RID at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
			return _Ptr;
		}
	}

	return File.Write(_Ptr, Data.data(), Data.size() * sizeof(Resident_Evil_2_RID_Data));
}


/*
	Save
*/
bool Resident_Evil_2_RID::Save(std::filesystem::path Path, std::uintmax_t _Ptr)
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
void Resident_Evil_2_RID::Close(void)
{
	Data.clear();
}