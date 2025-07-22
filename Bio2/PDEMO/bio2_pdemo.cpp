/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO:
*
*/


#include "bio2_pdemo.h"


/*
	Open
*/
bool Resident_Evil_2_PDEMO::Open(std::filesystem::path Input)
{
	Standard_String Str;

	b_Open = false;

	StdFile m_File { Input, FileAccessMode::Read, true, false };
	if (!m_File.IsOpen())
	{
		Str.Message("PDEMO: Error, could not open %s", Input.filename().string().c_str());
		return false;
	}

	if (m_File.Size() != 0x746)
	{
		Str.Message("PDEMO: Error, invalid file size: %llX", m_File.Size());
		return false;
	}

	m_File.Read(0, Data.get(), sizeof(Resident_Evil_2_Player_Demo));

	m_File.Close();

	return b_Open = true;
}


/*
	Save
*/
bool Resident_Evil_2_PDEMO::Save(std::filesystem::path Output)
{
	Standard_String Str;

	if (!b_Open)
	{
		Str.Message("PDEMO: Error, file is not open");
		return false;
	}

	StdFile m_File { Output, FileAccessMode::Write, true, false };
	if (!m_File.IsOpen())
	{
		Str.Message("PDEMO: Error, could not create %s", Output.filename().string().c_str());
		return false;
	}

	m_File.Write(0, Data.get(), sizeof(Resident_Evil_2_Player_Demo));

	m_File.Close();

	return true;
}


/*
	Save as text
*/
bool Resident_Evil_2_PDEMO::DisassembleToText(std::filesystem::path Input)
{
	Standard_String Str;

	std::unique_ptr<Resident_Evil_2_PDEMO> Demo = std::make_unique<Resident_Evil_2_PDEMO>();
	if (!Demo->Open(Input))
	{
		return false;
	}

	Standard_FileSystem FS;
	std::filesystem::path Dir = FS.GetDirectory(Input);

	std::unique_ptr<StdText> Text = std::make_unique<StdText>();
	Text->SetBOM(TextFileBOM::UTF8);
	if (!Text->Open(Str.FormatCStyle("%s\\%s.ini", Dir.string().c_str(), Input.stem().string().c_str()), FileAccessMode::Write))
	{
		return false;
	}

	Text->AddLine("bio2\tpdemo\n");

	Text->AddLine("Stage_no\t0x%x\n", Demo->Data->Stage_no);
	Text->AddLine("Room_no\t\t0x%02x\n", Demo->Data->Room_no);
	Text->AddLine("Cut_no\t\t%02d\n", Demo->Data->Cut_no);
	Text->AddLine("Equip_id\t0x%02x\n", Demo->Data->Equip_id);
	Text->AddLine("Equip_no\t0x%02x\n", Demo->Data->Equip_no);
	Text->AddLine("Key_idx\t\t0x%02x\n", Demo->Data->Key_idx);
	Text->AddLine("Player_id\t0x%02x\n", Demo->Data->Player_id);
	for (std::size_t i = 0; i < 11; i++)
	{
		Text->AddLine("Item[%d].Id\t0x%02x\n", i, Demo->Data->Item_wk[i].Id);
		Text->AddLine("Item[%d].Num\t0x%02x\n", i, Demo->Data->Item_wk[i].Num);
		Text->AddLine("Item[%d].Size\t0x%02x\n", i, Demo->Data->Item_wk[i].Size);
	}
	Text->AddLine("Pdemo_cnt\t%d\n", Demo->Data->Pdemo_cnt);
	Text->AddLine("Pos_x\t\t%d\n", Demo->Data->Pos_x);
	Text->AddLine("Pos_y\t\t%d\n", Demo->Data->Pos_y);
	Text->AddLine("Pos_z\t\t%d\n", Demo->Data->Pos_z);
	Text->AddLine("Cdir_y\t\t%d\n", Demo->Data->Cdir_y);
	for (std::size_t i = 0; i < Demo->Data->Pdemo_cnt; i++)
	{
		Text->AddLine("Padd[%d]\t0x%04x\n", i, Demo->Data->Padd[i]);
	}

	Text->FlushUTF8();
	Text->Close();

	return true;
}


/*
	Assemble
*/
bool Resident_Evil_2_PDEMO::AssembleText(std::filesystem::path Config)
{
	Standard_String Str;

	std::unique_ptr<StdText> Text = std::make_unique<StdText>();
	if (!Text->Open(Config, FileAccessMode::Read))
	{
		Str.Message("PDEMO: Error, could not open %s", Config.filename().string().c_str());
		return false;
	}

	StrVec Header = Text->GetStrVec(Text->GetLine(0));
	if (Header.empty() || (Header.size() < 2) || (Str.ToUpper(Header[0]) != "BIO2") || (Str.ToUpper(Header[1]) != "PDEMO"))
	{
		Str.Message("PDEMO: Assemble error, %s doesn't appear to be properly configured", Config.filename().string().c_str());
		return false;
	}

	Standard_FileSystem FS;
	std::filesystem::path Dir = FS.GetDirectory(Config);

	std::unique_ptr<Resident_Evil_2_PDEMO> Demo = std::make_unique<Resident_Evil_2_PDEMO>();

	Demo->Data->Stage_no = static_cast<std::uint8_t>(std::strtoull(Text->GetStrVec(Text->GetLine(1))[1].c_str(), nullptr, 16));
	Demo->Data->Room_no = static_cast<std::uint8_t>(std::strtoull(Text->GetStrVec(Text->GetLine(2))[1].c_str(), nullptr, 16));
	Demo->Data->Cut_no = static_cast<std::uint8_t>(std::strtoull(Text->GetStrVec(Text->GetLine(3))[1].c_str(), nullptr, 10));
	Demo->Data->Equip_id = static_cast<std::uint8_t>(std::strtoull(Text->GetStrVec(Text->GetLine(4))[1].c_str(), nullptr, 16));
	Demo->Data->Equip_no = static_cast<std::uint8_t>(std::strtoull(Text->GetStrVec(Text->GetLine(5))[1].c_str(), nullptr, 16));
	Demo->Data->Key_idx = static_cast<std::uint8_t>(std::strtoull(Text->GetStrVec(Text->GetLine(6))[1].c_str(), nullptr, 16));
	Demo->Data->Player_id = static_cast<std::uint8_t>(std::strtoull(Text->GetStrVec(Text->GetLine(7))[1].c_str(), nullptr, 16));
	for (std::size_t i = 0, x = 8; i < 11; i++, x += 3)
	{
		Demo->Data->Item_wk[i].Id = static_cast<std::uint8_t>(std::strtoull(Text->GetStrVec(Text->GetLine(x + 0))[1].c_str(), nullptr, 16));
		Demo->Data->Item_wk[i].Num = static_cast<std::uint8_t>(std::strtoull(Text->GetStrVec(Text->GetLine(x + 1))[1].c_str(), nullptr, 16));
		Demo->Data->Item_wk[i].Size = static_cast<std::uint8_t>(std::strtoull(Text->GetStrVec(Text->GetLine(x + 2))[1].c_str(), nullptr, 16));
	}
	Demo->Data->Pdemo_cnt = static_cast<std::uint16_t>(std::strtoull(Text->GetStrVec(Text->GetLine(41))[1].c_str(), nullptr, 10));
	Demo->Data->Pos_x = static_cast<std::int16_t>(std::strtoll(Text->GetStrVec(Text->GetLine(42))[1].c_str(), nullptr, 10));
	Demo->Data->Pos_y = static_cast<std::int16_t>(std::strtoll(Text->GetStrVec(Text->GetLine(43))[1].c_str(), nullptr, 10));
	Demo->Data->Pos_z = static_cast<std::int16_t>(std::strtoll(Text->GetStrVec(Text->GetLine(44))[1].c_str(), nullptr, 10));
	Demo->Data->Cdir_y = static_cast<std::int16_t>(std::strtoll(Text->GetStrVec(Text->GetLine(45))[1].c_str(), nullptr, 10));
	for (std::size_t i = 0, x = 46; i < Demo->Data->Pdemo_cnt; i++, x++)
	{
		Demo->Data->Padd[i] = static_cast<std::uint16_t>(std::strtoull(Text->GetStrVec(Text->GetLine(x))[1].c_str(), nullptr, 16));
	}

	Demo->b_Open = true;
	return Demo->Save(Dir / Config.stem() += ".dat");
}