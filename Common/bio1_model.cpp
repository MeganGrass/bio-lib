/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO:
*
*/


#include "bio1_model.h"


/*
	Set game
*/
void Resident_Evil_Model::SetGame(Resident_Evil_Video_Game _Game)
{
	Game = _Game;
	for (std::int32_t i = 0; i < 8; i++)
	{
		Anim[i]->SetGame(Game);
	}
}


/*
	Open
*/
bool Resident_Evil_Model::Open(std::filesystem::path Input)
{
	if (b_Open) { Close(); }

	StdFile File { Input, FileAccessMode::Read, true, false };
	if (!File.IsOpen())
	{
		Str->Message("Resident Evil Model: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	String Extension = Input.extension().string();

	std::size_t nOffset = 0;

	if (Str->ToUpper(Extension) == ".EMW")
	{
		Type = Resident_Evil_Model_Type::Weapon;
		nOffset = 2;
	}
	else if (std::to_underlying(Game) & (AUG95 | OCT95))
	{
		nOffset = 3;
	}
	else
	{
		nOffset = 5;
	}

	std::vector<std::uint32_t> Offset(nOffset);
	File.Read(File.Size() - (Offset.size() * sizeof(std::uint32_t)), Offset.data(), Offset.size() * sizeof(std::uint32_t));

	if (std::to_underlying(Type) & (WEAPON))
	{
		if (Anim[ANIM_WEAPON]->OpenEDD(File, Offset[0]) != Offset[1]) { return false; }
		if (Anim[ANIM_WEAPON]->OpenEMR(File, 0) != Offset[0]) { return false; }
		if (Tmd->Open(File, Offset[1]) != (File.Size() - (Offset.size() * sizeof(std::uint32_t)))) { return false; }
	}
	else if (std::to_underlying(Game) & (AUG95 | OCT95))
	{
		if (Anim[ANIM_NORMAL]->OpenEDD(File, Offset[0]) != Offset[1]) { return false; }
		if (Anim[ANIM_NORMAL]->OpenEMR(File, 0) != Offset[0]) { return false; }
		if (Tmd->Open(File, Offset[1]) != Offset[2]) { return false; }
		if (Tim->Open(File, Offset[2]) != (File.Size() - (Offset.size() * sizeof(std::uint32_t)))) { return false; }
	}
	else
	{
		if (Offset[0])
		{
			Type = Resident_Evil_Model_Type::Enemy;
			if (Anim[ANIM_NORMAL]->OpenEDD(File, Offset[0]) != Offset[1]) { return false; }
			if (Anim[ANIM_NORMAL]->OpenEMR(File, 0) != Offset[0]) { return false; }
			if (Anim[ANIM_DAMAGE]->OpenEDD(File, Offset[2]) != Offset[3]) { return false; }
			if (Anim[ANIM_DAMAGE]->OpenEMR(File, Offset[1]) != Offset[2]) { return false; }
		}
		else
		{
			if (Anim[ANIM_NORMAL]->OpenEDD(File, Offset[2]) != Offset[3]) { return false; }
			if (Anim[ANIM_NORMAL]->OpenEMR(File, 0) != Offset[2]) { return false; }
		}
		if (Tmd->Open(File, Offset[3]) != Offset[4]) { return false; }
		if (Tim->Open(File, Offset[4]) != (File.Size() - (Offset.size() * sizeof(std::uint32_t)))) { return false; }
	}

	return b_Open = true;
}


/*
	Save
*/
bool Resident_Evil_Model::Save(std::filesystem::path Output)
{
	if (!b_Open)
	{
		Str->Message("Resident Evil Model: Error, model is not open");
		return false;
	}

	StdFile File { Output, FileAccessMode::Write_Ex, true, false };
	if (!File.IsOpen())
	{
		Str->Message("Resident Evil Model: Error, could not create %s", Output.filename().string().c_str());
		return false;
	}

	std::size_t nOffset = 0;

	if (std::to_underlying(Type) & (WEAPON))
	{
		nOffset = 2;
	}
	else if (std::to_underlying(Game) & (AUG95 | OCT95))
	{
		nOffset = 3;
	}
	else
	{
		nOffset = 5;
	}

	std::vector<std::uintmax_t> Offset(nOffset);

	if (std::to_underlying(Type) & (WEAPON))
	{
		Offset[0] = Anim[ANIM_WEAPON]->SaveEMR(File, 0);
		Offset[1] = Anim[ANIM_WEAPON]->SaveEDD(File, Offset[0]);
		Tmd->Save(File, Offset[1]);
	}
	else if (std::to_underlying(Game) & (AUG95 | OCT95))
	{
		Offset[0] = Anim[ANIM_NORMAL]->SaveEMR(File, 0);
		Offset[1] = Anim[ANIM_NORMAL]->SaveEDD(File, Offset[0]);
		Offset[2] = Tmd->Save(File, Offset[1]);
		Tim->Save(File, Offset[2]);
		{
			std::uint32_t TmdSize = static_cast<std::uint32_t>(Tmd->Size());
			File.Write(Offset[1], &TmdSize, sizeof(std::uint32_t));
		}
	}
	else
	{
		if (std::to_underlying(Type) & (ENEMY))
		{
			Offset[0] = Anim[ANIM_NORMAL]->SaveEMR(File, 0);
			Offset[1] = Anim[ANIM_NORMAL]->SaveEDD(File, Offset[0]);
			Offset[2] = Anim[ANIM_DAMAGE]->SaveEMR(File, Offset[1]);
			Offset[3] = Anim[ANIM_DAMAGE]->SaveEDD(File, Offset[2]);
		}
		else
		{
			Offset[2] = Anim[ANIM_NORMAL]->SaveEMR(File, 0);
			Offset[3] = Anim[ANIM_NORMAL]->SaveEDD(File, Offset[2]);
		}
		Offset[4] = Tmd->Save(File, Offset[3]);
		Tim->Save(File, Offset[4]);
		if (std::to_underlying(Type) & (ENEMY))
		{
			std::uint32_t TmdSize = static_cast<std::uint32_t>(Tmd->Size());
			File.Write(Offset[3], &TmdSize, sizeof(std::uint32_t));
		}
	}

	std::uint32_t FileSize = static_cast<std::uint32_t>(File.Size());
	for (std::size_t i = 0; i < Offset.size(); i++)
	{
		File.Write(FileSize + (i * sizeof(std::uint32_t)), &Offset[i], sizeof(std::uint32_t));
	}

	return true;
}


/*
	Extract
*/
bool Resident_Evil_Model::Extract(std::filesystem::path Input, Resident_Evil_Video_Game _Game)
{
	std::unique_ptr<Resident_Evil_Model> Emd = std::make_unique<Resident_Evil_Model>();

	Emd->SetGame(_Game);

	if (!Emd->Open(Input))
	{
		return false;
	}

	Standard_FileSystem FS;
	std::filesystem::path Dir = FS.GetDirectory(Input);
	FS.CreateDirectory(Dir / Input.stem());

	std::filesystem::path Filename = Str->FormatCStyle("%s\\%s\\%s.ini", Dir.string().c_str(), Input.stem().string().c_str(), Input.stem().string().c_str());

	std::unique_ptr<StdText> Text = std::make_unique<StdText>();
	Text->SetBOM(TextFileBOM::UTF8);
	if (!Text->Open(Filename, FileAccessMode::Write))
	{
		return false;
	}

	Text->AddLine(Text->FormatCStyle("bio1\temd\t%d\n", Emd->GetGame()));

	// Animation
	for (std::size_t i = 0; i < Emd->Anim.size(); i++)
	{
		if (Emd->Anim[i]->IsOpen())
		{
			Filename = Str->FormatCStyle("%s\\%s\\%s_%02d.emr", Dir.string().c_str(), Input.stem().string().c_str(), Input.stem().string().c_str(), i);
			Text->AddLine(Text->FormatCStyle("%s\n", Filename.filename().string().c_str()));
			Emd->Anim[i]->SaveEMR(Filename);
			Filename = Str->FormatCStyle("%s\\%s\\%s_%02d.edd", Dir.string().c_str(), Input.stem().string().c_str(), Input.stem().string().c_str(), i);
			Text->AddLine(Text->FormatCStyle("%s\n", Filename.filename().string().c_str()));
			Emd->Anim[i]->SaveEDD(Filename);
		}
	}

	// TMD
	if (Emd->Tmd->IsOpen())
	{
		Filename = Str->FormatCStyle("%s\\%s\\%s.tmd", Dir.string().c_str(), Input.stem().string().c_str(), Input.stem().string().c_str());
		Text->AddLine(Text->FormatCStyle("%s\n", Filename.filename().string().c_str()));
		Emd->Tmd->Save(Filename);
	}

	// TIM
	if (Emd->Tim->IsOpen())
	{
		Filename = Str->FormatCStyle("%s\\%s\\%s.tim", Dir.string().c_str(), Input.stem().string().c_str(), Input.stem().string().c_str());
		Text->AddLine(Text->FormatCStyle("%s\n", Filename.filename().string().c_str()));
		Emd->Tim->Save(Filename);

		// BMP
		std::unique_ptr<Standard_Image> Image = Emd->Tim->GetBitmap();
		for (uint32_t i = 0; i < Emd->Tim->GetClutSize(); i++)
		{
			Filename = Str->FormatCStyle("%s\\%s\\%s_%02d.bmp", Dir.string().c_str(), Input.stem().string().c_str(), Input.stem().string().c_str(), i); \
				Emd->Tim->UpdateBitmapPalette(Image, i);
			Image->SaveAsBitmap(Filename);
		}
		Image->Close();
	}

	Text->FlushUTF8();
	Text->Close();

	Emd->Close();

	return true;
}


/*
	Assemble
*/
bool Resident_Evil_Model::Assemble(std::filesystem::path Config)
{
	std::unique_ptr<StdText> Text = std::make_unique<StdText>();
	if (!Text->Open(Config, FileAccessMode::Read))
	{
		Str->Message("BGM: Error, could not open %s", Config.filename().string().c_str());
		return false;
	}

	StrVec Header = Text->GetStrVec(Text->GetLine(0));
	if (Header.empty() || (Header.size() < 2) || (Str->ToUpper(Header[0]) != "BIO1") || (Str->ToUpper(Header[1]) != "EMD"))
	{
		Str->Message("BGM: Assemble error, %s doesn't appear to be properly configured", Config.filename().string().c_str());
		return false;
	}

	Standard_FileSystem FS;

	std::filesystem::path Dir = FS.GetDirectory(Config);

	std::unique_ptr<Resident_Evil_Model> Emd = std::make_unique<Resident_Evil_Model>();

	Emd->b_Open = true;

	return Emd->Save(Dir / Config.stem() += ".emd");
}


/*
	Close
*/
void Resident_Evil_Model::Close(void)
{
	b_Open = false;
	Tmd->Close();
	TmdEx->Close();
	Tim->Close();
	for (auto& i : Anim)
	{
		i->CloseEDD();
		i->CloseEMR();
	}
}