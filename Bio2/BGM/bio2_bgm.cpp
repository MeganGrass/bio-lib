/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO:
*
*/


#include "bio2_bgm.h"


/*
	Open
*/
bool Resident_Evil_2_BGM::Open(std::filesystem::path Input)
{
	if (b_Open) { Close(); }

	Standard_String Str;

	StdFile File { Input, FileAccessMode::Read, true, false };
	if (!File.IsOpen())
	{
		Str.Message("BGM: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	std::uintmax_t _Ptr = File.Size();
	std::vector<std::uint32_t> Offset(5);

	for (std::uint32_t i = 0; i < 5; i++, _Ptr -= sizeof(std::uint32_t))
	{
		File.Read(_Ptr - 4, &Offset[i], sizeof(std::uint32_t));
	}

	if (!Seq->OpenSEQ(File, Offset[0]))
	{
		return false;
	}

	if (!Offset[4])
	{
		if (!Seq->OpenSEQ(File, Offset[1]))
		{
			return false;
		}
		if (!Vab->OpenVH(File, Offset[2]))
		{
			return false;
		}
		if (!Vab->OpenVB(File, Offset[3]))
		{
			return false;
		}
	}
	else
	{
		if (!Vab->OpenVH(File, Offset[1]))
		{
			return false;
		}
		if (!Vab->OpenVB(File, Offset[2]))
		{
			return false;
		}
	}

	File.Close();

	return b_Open = true;
}


/*
	Save
*/
bool Resident_Evil_2_BGM::Save(std::filesystem::path Output)
{
	Standard_String Str;

	if (!b_Open)
	{
		Str.Message("BGM: Error, no file is open");
		return false;
	}

	StdFile File { Output, FileAccessMode::Write_Ex, true, false };
	if (!File.IsOpen())
	{
		Str.Message("BGM: Error, could not create %s", Output.filename().string().c_str());
		return false;
	}

	std::size_t nOffset = Seq->GetMidiCount() == 2 ? 5 : 4;
	std::vector<std::uintmax_t> Offset(nOffset);
	std::uintmax_t pIndex = 0;

	Offset[1] = Seq->SaveSEQ(File, 0, 0);
	Offset[1]++;

	if (Seq->GetMidiCount() > 1)
	{
		Offset[2] = Seq->SaveSEQ(File, 1, File.PosAlign(Offset[1], 4));
		Offset[2]++;
		Offset[3] = Vab->SaveVH(File, File.PosAlign(Offset[2], 4));
		Offset[3]++;
		pIndex = Vab->SaveVB(File, File.PosAlign(Offset[3], 4));
		pIndex++;
		pIndex = (File.PosAlign(pIndex, 4) + (sizeof(std::uint32_t) * 3));
	}
	else
	{
		Offset[2] = Vab->SaveVH(File, File.PosAlign(Offset[1], 4));
		Offset[2]++;
		pIndex = Vab->SaveVB(File, File.PosAlign(Offset[2], 4));
		pIndex++;
		pIndex = (File.PosAlign(pIndex, 4) + (sizeof(std::uint32_t) * 2));
	}

	for (std::size_t i = 0; i < Offset.size() - 1; i++, pIndex -= sizeof(std::uint32_t))
	{
		File.Write(pIndex, &Offset[i], sizeof(std::uint32_t));
	}

	File.Close();

	return true;
}


/*
	Extract
*/
bool Resident_Evil_2_BGM::Extract(std::filesystem::path Input)
{
	Standard_String Str;

	std::unique_ptr<Resident_Evil_2_BGM> Bgm = std::make_unique<Resident_Evil_2_BGM>();

	if (!Bgm->Open(Input))
	{
		return false;
	}

	Standard_FileSystem FS;
	std::filesystem::path Dir = FS.GetDirectory(Input);
	FS.CreateDirectory(Dir / Input.stem());

	std::filesystem::path Filename = Str.FormatCStyle("%s\\%s\\%s.ini", Dir.string().c_str(), Input.stem().string().c_str(), Input.stem().string().c_str());

	std::unique_ptr<StdText> Text = std::make_unique<StdText>();
	Text->SetBOM(TextFileBOM::UTF8);
	if (!Text->Open(Filename, FileAccessMode::Write))
	{
		return false;
	}

	Text->AddLine("bio2\tbgm\n");

	for (std::size_t i = 0; i < Bgm->Seq->GetMidiCount(); i++)
	{
		Filename = Str.FormatCStyle("%s\\%s\\%s_%02d.seq", Dir.string().c_str(), Input.stem().string().c_str(), Input.stem().string().c_str(), i);
		Text->AddLine("%s\n", Filename.filename().string().c_str());
		Bgm->Seq->SaveSEQ(Filename, i);
	}

	Filename = Str.FormatCStyle("%s\\%s\\%s.vh", Dir.string().c_str(), Input.stem().string().c_str(), Input.stem().string().c_str());
	Text->AddLine("%s\n", Filename.filename().string().c_str());
	Bgm->Vab->SaveVH(Filename);

	Filename.replace_extension(".vb");
	Text->AddLine("%s\n", Filename.filename().string().c_str());
	Bgm->Vab->SaveVB(Filename);

	Text->FlushUTF8();
	Text->Close();

	Bgm->Close();

	return true;
}


/*
	Assemble
*/
bool Resident_Evil_2_BGM::Assemble(std::filesystem::path Config)
{
	Standard_String Str;

	std::unique_ptr<StdText> Text = std::make_unique<StdText>();
	if (!Text->Open(Config, FileAccessMode::Read))
	{
		Str.Message("BGM: Error, could not open %s", Config.filename().string().c_str());
		return false;
	}

	StrVec Header = Text->GetStrVec(Text->GetLine(0));
	if (Header.empty() || (Header.size() < 2) || (Str.ToUpper(Header[0]) != "BIO2") || (Str.ToUpper(Header[1]) != "BGM"))
	{
		Str.Message("BGM: Assemble error, %s doesn't appear to be properly configured", Config.filename().string().c_str());
		return false;
	}

	Standard_FileSystem FS;

	std::filesystem::path Dir = FS.GetDirectory(Config);

	std::unique_ptr<Resident_Evil_2_BGM> Bgm = std::make_unique<Resident_Evil_2_BGM>();

	if (!Bgm->Seq->OpenSEQ(Dir / FS.GetFileName(Text->GetLine(1))))
	{
		return false;
	}

	String Ext = FS.GetFileExtension(Text->GetLine(2)).string();
	if (Str.ToUpper(Ext) == ".SEQ")
	{
		if (!Bgm->Seq->OpenSEQ(Dir / FS.GetFileName(Text->GetLine(2))))
		{
			return false;
		}
		if (!Bgm->Vab->OpenVH(Dir / FS.GetFileName(Text->GetLine(3))))
		{
			return false;
		}
		if (!Bgm->Vab->OpenVB(Dir / FS.GetFileName(Text->GetLine(4))))
		{
			return false;
		}
	}
	else
	{
		if (!Bgm->Vab->OpenVH(Dir / FS.GetFileName(Text->GetLine(2))))
		{
			return false;
		}
		if (!Bgm->Vab->OpenVB(Dir / FS.GetFileName(Text->GetLine(3))))
		{
			return false;
		}
	}

	Bgm->b_Open = true;

	return Bgm->Save(Dir / Config.stem() += ".bgm");
}


/*
	Close
*/
void Resident_Evil_2_BGM::Close(void)
{
	b_Open = false;
	Seq->Close();
	Vab->CloseVAB();
}