/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO:
*
*/


#include "bio2_model.h"


/*
	Open
*/
bool Resident_Evil_2_Model::Open(std::filesystem::path Input)
{
	if (b_Open) { Close(); }

	StdFile File{ Input, FileAccessMode::Read, true, false };
	if (!File.IsOpen())
	{
		Str->Message("Resident Evil 2 Model: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	String Extension = Input.extension().string();

	struct File_Header
	{
		std::uint32_t pIndex;
		std::uint32_t Count;
	};

	File_Header Header{};
	File.Read(0, &Header, sizeof(File_Header));

	std::vector<std::uint32_t> Offset(Header.Count);
	File.Read(Header.pIndex, Offset.data(), Offset.size() * sizeof(std::uint32_t));

	if (Str->ToUpper(Extension) == ".PLW")
	{
		Type = Resident_Evil_Model_Type::Weapon;
	}

	if ((Str->ToUpper(Extension) == ".EMD") || (Header.Count == 8))
	{
		Type = Resident_Evil_Model_Type::Enemy;
	}

	return b_Open = true;
}