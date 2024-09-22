/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO: 
*
*/


#include "bio2_edt.h"


/*
	Open
*/
std::uintmax_t Resident_Evil_2_EDT::Open(StdFile& File, std::uintmax_t Count, std::uintmax_t _Ptr)
{
	Close();

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read, true, false))
		{
			Str->Message("EDT: Error, could not open at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
			return _Ptr;
		}
	}

	struct EDT
	{
		std::uint8_t id_check;
		std::uint8_t pan_prog;
		std::uint8_t tone_pri;
		std::uint8_t monopoly;
	};

	EDT Edt{};

	for (std::uintmax_t i = 0; i < Count; i++, _Ptr += sizeof(EDT))
	{
		File.Read(_Ptr, &Edt, sizeof(EDT));

		if ((Edt.id_check == 0xFF) && (Edt.pan_prog == 0xFF) && (Edt.tone_pri == 0xFF) && (Edt.monopoly == 0xFF))
		{
			Resident_Evil_2_EDT_Data _Data = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
			Data.push_back(_Data);
			continue;
		}

		std::uint8_t Vab_id = 0;

		if (Edt.id_check & 0x80)
		{
			Vab_id = Edt.id_check & 0x7F;
		}

		std::uint8_t Pan = Edt.pan_prog & 0x7F;

		std::uint8_t Tone_id = Edt.tone_pri >> 4;

		std::uint8_t Priority = (Edt.tone_pri & 0xF) & 7;

		std::uint8_t Sch = Edt.monopoly & 0x1F;

		std::uint8_t EdtCount = (Edt.monopoly >> 5);

		Resident_Evil_2_EDT_Data _Data = { Vab_id, Pan, Tone_id, Priority, Sch, EdtCount };

		Data.push_back(_Data);
	}

	return _Ptr;
}


/*
	Open
*/
bool Resident_Evil_2_EDT::Open(std::filesystem::path Input, std::uintmax_t Count, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Input);

	Open(m_File, Count, _Ptr);

	return !Data.empty();
}


/*
	Save
*/
std::uintmax_t Resident_Evil_2_EDT::Save(StdFile& File, std::uintmax_t _Ptr)
{
	if (Data.empty())
	{
		Str->Message("EDT: Error, no data to save");
		return _Ptr;
	}

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Write, true, false))
		{
			Str->Message("EDT: Error, could not create at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
			return _Ptr;
		}
	}

	struct EDT
	{
		std::uint8_t id_check;
		std::uint8_t pan_prog;
		std::uint8_t tone_pri;
		std::uint8_t monopoly;
	};

	EDT Edt{};

	for (std::size_t i = 0; i < Data.size(); i++, _Ptr += sizeof(EDT))
	{
		Edt.id_check = Data[i].Vab_id;

		Edt.pan_prog = Data[i].Pan;

		Edt.tone_pri = (Data[i].Tone_id << 4) | Data[i].Priority;

		Edt.monopoly = (Data[i].Count << 5) | Data[i].Sch;

		File.Write(_Ptr, &Edt, sizeof(EDT));
	}

	return _Ptr;
}


/*
	Save
*/
bool Resident_Evil_2_EDT::Save(std::filesystem::path Output, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Output);

	std::uintmax_t OldPtr = _Ptr;

	_Ptr = Save(m_File, _Ptr);

	return OldPtr != _Ptr;
}


/*
	Open text
*/
bool Resident_Evil_2_EDT::OpenText(std::filesystem::path Output)
{
	Close();

	std::unique_ptr<StdText> Text = std::make_unique<StdText>();

	if (!Text->Open(Output, FileAccessMode::Read))
	{
		return false;
	}

	for (std::size_t i = 0; i < Text->GetLineCount(); i++)
	{
		StrVec Args = Text->GetStrVec(Text->GetLine(i));

		if (Args.size() != 6) { break; }

		Resident_Evil_2_EDT_Data _Data = {
			static_cast<std::uint8_t>(std::stoul(Args.at(0))),
			static_cast<std::uint8_t>(std::stoul(Args.at(1))),
			static_cast<std::uint8_t>(std::stoul(Args.at(2))),
			static_cast<std::uint8_t>(std::stoul(Args.at(3))),
			static_cast<std::uint8_t>(std::stoul(Args.at(4))),
			static_cast<std::uint8_t>(std::stoul(Args.at(5))), };

		Data.push_back(_Data);
	}

	Text->Close();

	return true;
}


/*
	Save as text
*/
bool Resident_Evil_2_EDT::SaveAsText(std::filesystem::path Output)
{
	std::unique_ptr<StdText> Text = std::make_unique<StdText>();

	Text->SetBOM(TextFileBOM::UTF8);

	if (!Text->Open(Output, FileAccessMode::Write))
	{
		return false;
	}

	for (std::size_t i = 0; i < Data.size(); i++)
	{
		Text->AddLine(Text->FormatCStyle("%d %d %d %d %d %d\n", Data[i].Vab_id, Data[i].Pan, Data[i].Tone_id, Data[i].Priority, Data[i].Sch, Data[i].Count));
	}

	Text->FlushUTF8();

	Text->Close();

	return true;
}


/*
	Close
*/
void Resident_Evil_2_EDT::Close(void)
{
	Data.clear();
}