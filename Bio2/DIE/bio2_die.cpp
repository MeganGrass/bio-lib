/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO:
*
*/


#include "bio2_die.h"


/*
	Open
*/
bool Resident_Evil_2_DIE::Open(std::filesystem::path Input)
{
	if (b_Open) { Close(); }

	Standard_String Str;

	StdFile File { Input, FileAccessMode::Read, true, false };
	if (!File.IsOpen())
	{
		Str.Message("DIE: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	std::uintmax_t _Ptr = Tim->OpenTIM(File, 0);

	_Ptr = Edt->Open(File, 16, _Ptr);

	_Ptr = Vab->OpenVH(File, _Ptr);

	_Ptr += 8;

	_Ptr = Vab->OpenVB(File, _Ptr);

	File.Close();

	return b_Open = true;
}


/*
	Save
*/
bool Resident_Evil_2_DIE::Save(std::filesystem::path Output)
{
	Standard_String Str;

	if (!b_Open)
	{
		Str.Message("DIE: Error, no file is open");
		return false;
	}

	StdFile File { Output, FileAccessMode::Write_Ex, true, false };
	if (!File.IsOpen())
	{
		Str.Message("DIE: Error, could not create %s", Output.filename().string().c_str());
		return false;
	}

	std::uintmax_t _Ptr = Tim->SaveTIM(File, 0);

	_Ptr = Edt->Save(File, _Ptr);

	_Ptr = Vab->SaveVH(File, _Ptr);

	std::uint32_t Offset = 0x40;
	File.Write(_Ptr, &Offset, sizeof(std::uint32_t));
	Offset = 0;
	File.Write(_Ptr + 4, &Offset, sizeof(std::uint32_t));

	_Ptr += 8;

	_Ptr = Vab->SaveVB(File, _Ptr);

	File.Close();

	return true;
}


/*
	Extract
*/
bool Resident_Evil_2_DIE::Extract(std::filesystem::path Input)
{
	Standard_String Str;

	std::unique_ptr<Resident_Evil_2_DIE> Die = std::make_unique<Resident_Evil_2_DIE>();

	if (!Die->Open(Input))
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

	Text->AddLine("bio2\tdie\n");

	Filename = Str.FormatCStyle("%s\\%s\\%s.tim", Dir.string().c_str(), Input.stem().string().c_str(), Input.stem().string().c_str());
	Text->AddLine("%s\n", Filename.filename().string().c_str());
	Die->Tim->SaveTIM(Filename);

	std::unique_ptr<Standard_Image> Image = Die->Tim->ExportImage();
	Image->SaveBMP(Filename.replace_extension(".bmp"));
	Image->Close();

	Filename = Str.FormatCStyle("%s\\%s\\edt.ini", Dir.string().c_str(), Input.stem().string().c_str());
	Text->AddLine("%s\n", Filename.filename().string().c_str());
	Die->Edt->SaveAsText(Filename);

	Filename = Str.FormatCStyle("%s\\%s\\%s.vh", Dir.string().c_str(), Input.stem().string().c_str(), Input.stem().string().c_str());
	Text->AddLine("%s\n", Filename.filename().string().c_str());
	Die->Vab->SaveVH(Filename);

	Filename = Str.FormatCStyle("%s\\%s\\%s.vb", Dir.string().c_str(), Input.stem().string().c_str(), Input.stem().string().c_str());
	Text->AddLine("%s\n", Filename.filename().string().c_str());
	Die->Vab->SaveVB(Filename);

	Text->FlushUTF8();
	Text->Close();

	Die->Close();

	return true;
}


/*
	Assemble
*/
bool Resident_Evil_2_DIE::Assemble(std::filesystem::path Config)
{
	Standard_String Str;

	std::unique_ptr<StdText> Text = std::make_unique<StdText>();
	if (!Text->Open(Config, FileAccessMode::Read))
	{
		Str.Message("DIE: Error, could not open %s", Config.filename().string().c_str());
		return false;
	}

	StrVec Header = Text->GetStrVec(Text->GetLine(0));
	if (Header.empty() || (Header.size() < 2) || (Str.ToUpper(Header[0]) != "BIO2") || (Str.ToUpper(Header[1]) != "DIE"))
	{
		Str.Message("DIE: Assemble error, %s doesn't appear to be properly configured", Config.filename().string().c_str());
		return false;
	}

	Standard_FileSystem FS;

	std::filesystem::path Dir = FS.GetDirectory(Config);

	std::unique_ptr<Resident_Evil_2_DIE> Die = std::make_unique<Resident_Evil_2_DIE>();

	if (!Die->Tim->OpenTIM(Dir / FS.GetFileName(Text->GetLine(1))))
	{
		return false;
	}

	if (!Die->Edt->OpenText(Dir / FS.GetFileName(Text->GetLine(2))))
	{
		return false;
	}

	if (!Die->Vab->OpenVH(Dir / FS.GetFileName(Text->GetLine(3))))
	{
		return false;
	}

	if (!Die->Vab->OpenVB(Dir / FS.GetFileName(Text->GetLine(4))))
	{
		return false;
	}

	Die->b_Open = true;

	return Die->Save(Dir / Config.stem() += ".die");
}


/*
	Close
*/
void Resident_Evil_2_DIE::Close(void)
{
	b_Open = false;
	Tim->Close();
	Edt->Close();
	Vab->CloseVAB();
}