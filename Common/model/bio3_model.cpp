/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO:
*
*/


#include "bio3_model.h"


/*
	Open
*/
bool Resident_Evil_3_Model::Open(std::filesystem::path Input)
{
	if (b_Open) { Close(); }

	StdFile File{ Input, FileAccessMode::Read, true, false };
	if (!File.IsOpen())
	{
		Str->Message("Resident Evil 3 Model: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	String Extension = Input.extension().string();

	Standard_FileSystem FS;
	std::filesystem::path Dir = FS.GetDirectory(Input);
	FS.CreateDirectory(Dir / Input.stem());

	struct File_Header
	{
		std::uint32_t pIndex;
		std::uint32_t Count;
	};

	File_Header Header{};
	File.Read(0, &Header, sizeof(File_Header));

	std::vector<std::uint32_t> Offset(Header.Count);
	File.Read(Header.pIndex, Offset.data(), Offset.size() * sizeof(std::uint32_t));

	if ((Str->ToUpper(Extension) == ".PLD") && (Header.Count == 5))
	{
		Type = Resident_Evil_Model_Type::Player;

		if (Anim[ANIM_NORMAL]->OpenEDD(File, Offset[0]) != Offset[1]) { return false; }
		if (Anim[ANIM_NORMAL]->OpenEMR(File, Offset[1]) != Offset[2]) { return false; }
		if (Md2->Open(File, Offset[2]) != Offset[3]) { return false; }
		if (Tim->Open(File, Offset[4]) != (File.Size() - (Offset.size() * sizeof(std::uint32_t)))) { return false; }

		std::uint32_t BinarySize = Offset[4] - Offset[3];
		Binary00.resize(BinarySize);
		File.Read(Offset[3], Binary00.data(), Binary00.size());

		Tmd = Md2->GetTMD();
	}
	else if ((Str->ToUpper(Extension) == ".EMD") && (Header.Count == 15))
	{
		Type = Resident_Evil_Model_Type::Enemy;
	}
	else if ((Str->ToUpper(Extension) == ".PLW") && (Header.Count == 9))
	{
		Type = Resident_Evil_Model_Type::Weapon;

		if (Anim[ANIM_WEAPON]->OpenEDD(File, Offset[0]) != Offset[1]) { std::cout << "EDD 0" << std::endl; return false; }

		//size_t Pointer = Anim[ANIM_WEAPON]->OpenEMR(File, Offset[1]);
		//Anim[ANIM_WEAPON]->SaveEDD(Dir / Input.stem() / "saving.edd");
		//std::cout << "Ret: " << std::hex << Pointer << " " << Offset[2] << std::endl;
		//return false;

		if (Anim[ANIM_WEAPON]->OpenEMR(File, Offset[1]) != Offset[2]) { std::cout << "EMR0" << std::endl; return false; }
		if (Anim[ANIM_WEAPON_EX0]->OpenEDD(File, Offset[2]) != Offset[3]) { std::cout << "EDD1" << std::endl; return false; }
		if (Anim[ANIM_WEAPON_EX0]->OpenEMR(File, Offset[3]) != Offset[4]) { std::cout << "EMR1" << std::endl; return false; }
		if (Anim[ANIM_WEAPON_EX1]->OpenEDD(File, Offset[5]) != Offset[6]) { std::cout << "EDD2" << std::endl; return false; }
		if (Anim[ANIM_WEAPON_EX1]->OpenEMR(File, Offset[6]) != Offset[7]) { std::cout << "EMR2" << std::endl; return false; }
		if (Md2->Open(File, Offset[7]) != Offset[8]) { return false; }
		if (Tim->Open(File, Offset[8]) != (File.Size() - (Offset.size() * sizeof(std::uint32_t)))) { return false; }
	}
	else
	{
		return false;
	}

	std::vector<std::uint32_t> Size(Header.Count);
	for (std::size_t i = 0; i < Size.size(); ++i)
	{
		if ((i + 1) == Size.size())
		{
			Size[i] = Header.pIndex - Offset[i];
		}
		else
		{
			Size[i] = Offset[i + 1] - Offset[i];
		}
	}

	std::vector<std::uint8_t> Buffer;

	if (Offset.size() == 5)
	{
		Buffer.resize(Size[0]);
		File.Read(Offset[0], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / Input.stem() += ".edd", Buffer.data(), Buffer.size());

		Buffer.resize(Size[1]);
		File.Read(Offset[1], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / Input.stem() += ".emr", Buffer.data(), Buffer.size());

		Buffer.resize(Size[2]);
		File.Read(Offset[2], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / Input.stem() += ".md2", Buffer.data(), Buffer.size());

		Buffer.resize(Size[3]);
		File.Read(Offset[3], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / Input.stem() += ".bin", Buffer.data(), Buffer.size());

		Buffer.resize(Size[4]);
		File.Read(Offset[4], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / Input.stem() += ".tim", Buffer.data(), Buffer.size());
	}

	if (Offset.size() == 9)
	{
		Buffer.resize(Size[0]);
		File.Read(Offset[0], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / Input.stem() += ".edd", Buffer.data(), Buffer.size());

		Buffer.resize(Size[1]);
		File.Read(Offset[1], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / Input.stem() += ".emr", Buffer.data(), Buffer.size());

		Buffer.resize(Size[2]);
		File.Read(Offset[2], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / "weapon_00.edd", Buffer.data(), Buffer.size());

		Buffer.resize(Size[3]);
		File.Read(Offset[3], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / "weapon_00.emr", Buffer.data(), Buffer.size());

		Buffer.resize(Size[4]);
		File.Read(Offset[4], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / "weapon.bin", Buffer.data(), Buffer.size());

		Buffer.resize(Size[5]);
		File.Read(Offset[5], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / "weapon_01.edd", Buffer.data(), Buffer.size());

		Buffer.resize(Size[6]);
		File.Read(Offset[6], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / "weapon_01.emr", Buffer.data(), Buffer.size());

		Buffer.resize(Size[7]);
		File.Read(Offset[7], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / Input.stem() += ".md2", Buffer.data(), Buffer.size());

		Buffer.resize(Size[8]);
		File.Read(Offset[8], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / Input.stem() += ".tim", Buffer.data(), Buffer.size());
	}

	if (Offset.size() == 15)
	{
		Buffer.resize(Size[0]);
		File.Read(Offset[0], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / "binary_00.bin", Buffer.data(), Buffer.size());

		Buffer.resize(Size[1]);
		File.Read(Offset[1], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / "binary_01.bin", Buffer.data(), Buffer.size());

		Buffer.resize(Size[2]);
		File.Read(Offset[2], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / Input.stem() += "_00.edd", Buffer.data(), Buffer.size());

		Buffer.resize(Size[3]);
		File.Read(Offset[3], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / Input.stem() += "_00.emr", Buffer.data(), Buffer.size());

		Buffer.resize(Size[4]);
		File.Read(Offset[4], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / Input.stem() += "_01.edd", Buffer.data(), Buffer.size());

		Buffer.resize(Size[5]);
		File.Read(Offset[5], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / Input.stem() += "_01.emr", Buffer.data(), Buffer.size());

		Buffer.resize(Size[6]);
		File.Read(Offset[6], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / "damage.edd", Buffer.data(), Buffer.size());

		Buffer.resize(Size[7]);
		File.Read(Offset[7], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / "damage.emr", Buffer.data(), Buffer.size());

		Buffer.resize(Size[8]);
		File.Read(Offset[8], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / "weapon.bin", Buffer.data(), Buffer.size());

		Buffer.resize(Size[9]);
		File.Read(Offset[9], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / "weapon_00.edd", Buffer.data(), Buffer.size());

		Buffer.resize(Size[10]);
		File.Read(Offset[10], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / "weapon_00.emr", Buffer.data(), Buffer.size());

		Buffer.resize(Size[11]);
		File.Read(Offset[11], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / "weapon_01.edd", Buffer.data(), Buffer.size());

		Buffer.resize(Size[12]);
		File.Read(Offset[12], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / "weapon_01.emr", Buffer.data(), Buffer.size());

		Buffer.resize(Size[13]);
		File.Read(Offset[13], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / "model_00.md2", Buffer.data(), Buffer.size());

		Buffer.resize(Size[14]);
		File.Read(Offset[14], Buffer.data(), Buffer.size());
		File.Create(Dir / Input.stem() / "model_01.md2", Buffer.data(), Buffer.size());
	}

	return b_Open = true;
}


/*
	Close
*/
void Resident_Evil_3_Model::Close(void)
{
	Md2->Close();
	Md2Ex->Close();
	Binary00.clear();
	Binary01.clear();
	WeaponBinary.clear();
	for (auto& i : Anim)
	{
		i->CloseEDD();
		i->CloseEMR();
	}
}