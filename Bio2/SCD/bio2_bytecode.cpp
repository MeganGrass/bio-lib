/*
*
*	Megan Grass
*	May 30, 2024
*
*/


#include "bio2_bytecode.h"


const std::size_t Resident_Evil_2_Bytecode::BytecodeSize(std::uint8_t Opcode) const noexcept
{
	switch (Opcode)
	{
	case 0x00: return sizeof(Nop);
	case 0x01: return sizeof(Evt_end);
	case 0x02: return sizeof(Evt_next);
	case 0x03: return sizeof(Evt_chain);
	case 0x04: return sizeof(Evt_exec);
	case 0x05: return sizeof(Evt_kill);
	case 0x06: return sizeof(Ifel_ck);
	case 0x07: return sizeof(Else_ck);
	case 0x08: return sizeof(Endif);
	case 0x09: return sizeof(Sleep);
	case 0x0A: return sizeof(Sleeping);
	case 0x0B: return sizeof(Wsleep);
	case 0x0C: return sizeof(Wsleeping);
	case 0x0D: return sizeof(For);
	case 0x0E: return sizeof(Next);
	case 0x0F: return sizeof(While);
	case 0x10: return sizeof(Ewhile);
	case 0x11: return sizeof(Do);
	case 0x12: return sizeof(Edwhile);
	case 0x13: return sizeof(Switch);
	case 0x14: return sizeof(Case);
	case 0x15: return sizeof(Default);
	case 0x16: return sizeof(Eswitch);
	case 0x17: return sizeof(Goto);
	case 0x18: return sizeof(Gosub);
	case 0x19: return sizeof(Return);
	case 0x1A: return sizeof(Break);
	case 0x1B: return sizeof(For2);
	case 0x1C: return sizeof(Break_point);
	case 0x1D: return sizeof(Work_copy);
	case 0x1E: return sizeof(Nop);
	case 0x1F: return sizeof(Nop);
	case 0x20: return sizeof(Nop);
	case 0x21: return sizeof(Ck);
	case 0x22: return sizeof(Set);
	case 0x23: return sizeof(Cmp);
	case 0x24: return sizeof(Save);
	case 0x25: return sizeof(Copy);
	case 0x26: return sizeof(Calc);
	case 0x27: return sizeof(Calc2);
	case 0x28: return sizeof(Sce_rnd);
	case 0x29: return sizeof(Cut_chg);
	case 0x2A: return sizeof(Cut_old);
	case 0x2B: return sizeof(Message_on);
	case 0x2C: return sizeof(Aot_set);
	case 0x2D: return sizeof(Obj_model_set);
	case 0x2E: return sizeof(Work_set);
	case 0x2F: return sizeof(Speed_set);
	case 0x30: return sizeof(Add_speed);
	case 0x31: return sizeof(Add_aspeed);
	case 0x32: return sizeof(Pos_set);
	case 0x33: return sizeof(Dir_set);
	case 0x34: return sizeof(Member_set);
	case 0x35: return sizeof(Member_set2);
	case 0x36: return sizeof(Se_on);
	case 0x37: return sizeof(Sca_id_set);
	case 0x38: return sizeof(Flr_set);
	case 0x39: return sizeof(Dir_ck);
	case 0x3A: return sizeof(Sce_espr_on);
	case 0x3B: return sizeof(Door_aot_set);
	case 0x3C: return sizeof(Cut_auto);
	case 0x3D: return sizeof(Member_copy);
	case 0x3E: return sizeof(Member_cmp);
	case 0x3F: return sizeof(Plc_motion);
	case 0x40: return sizeof(Plc_dest);
	case 0x41: return sizeof(Plc_neck);
	case 0x42: return sizeof(Plc_ret);
	case 0x43: return sizeof(Plc_flg);
	case 0x44: return sizeof(Sce_em_set);
	case 0x45: return sizeof(Col_chg_set);
	case 0x46: return sizeof(Aot_reset);
	case 0x47: return sizeof(Aot_on);
	case 0x48: return sizeof(Super_set);
	case 0x49: return sizeof(Super_reset);
	case 0x4A: return sizeof(Plc_gun);
	case 0x4B: return sizeof(Cut_replace);
	case 0x4C: return sizeof(Sce_espr_kill);
	case 0x4D: return sizeof(Door_model_set);
	case 0x4E: return sizeof(Item_aot_set);
	case 0x4F: return sizeof(Sce_key_ck);
	case 0x50: return sizeof(Sce_trg_ck);
	case 0x51: return sizeof(Sce_bgm_control);
	case 0x52: return sizeof(Sce_espr_control);
	case 0x53: return sizeof(Sce_fade_set);
	case 0x54: return sizeof(Sce_espr3d_on);
	case 0x55: return sizeof(Member_calc);
	case 0x56: return sizeof(Member_calc2);
	case 0x57: return sizeof(Sce_bgmtbl_set);
	case 0x58: return sizeof(Plc_rot);
	case 0x59: return sizeof(Xa_on);
	case 0x5A: return sizeof(Weapon_chg);
	case 0x5B: return sizeof(Plc_cnt);
	case 0x5C: return sizeof(Sce_shake_on);
	case 0x5D: return sizeof(Mizu_div_set);
	case 0x5E: return sizeof(Keep_Item_ck);
	case 0x5F: return sizeof(Xa_vol);
	case 0x60: return sizeof(Kage_set);
	case 0x61: return sizeof(Cut_be_set);
	case 0x62: return sizeof(Sce_Item_lost);
	case 0x63: return sizeof(Plc_gun_eff);
	case 0x64: return sizeof(Sce_espr_on2);
	case 0x65: return sizeof(Sce_espr_kill2);
	case 0x66: return sizeof(Plc_stop);
	case 0x67: return sizeof(Aot_set_4p);
	case 0x68: return sizeof(Door_aot_set_4p);
	case 0x69: return sizeof(Item_aot_set_4p);
	case 0x6A: return sizeof(Light_pos_set);
	case 0x6B: return sizeof(Light_kido_set);
	case 0x6C: return sizeof(Rbj_reset);
	case 0x6D: return sizeof(Sce_scr_move);
	case 0x6E: return sizeof(Parts_set);
	case 0x6F: return sizeof(Movie_on);
	case 0x70: return sizeof(Splc_ret);
	case 0x71: return sizeof(Splc_sce);
	case 0x72: return sizeof(Super_on);
	case 0x73: return sizeof(Mirror_set);
	case 0x74: return sizeof(Sce_fade_adjust);
	case 0x75: return sizeof(Sce_espr3d_on2);
	case 0x76: return sizeof(Sce_Item_get);
	case 0x77: return sizeof(Sce_line_start);
	case 0x78: return sizeof(Sce_line_main);
	case 0x79: return sizeof(Sce_line_end);
	case 0x7A: return sizeof(Sce_parts_bomb);
	case 0x7B: return sizeof(Sce_parts_down);
	case 0x7C: return sizeof(Light_color_set);
	case 0x7D: return sizeof(Light_pos_set2);
	case 0x7E: return sizeof(Light_kido_set2);
	case 0x7F: return sizeof(Light_color_set2);
	case 0x80: return sizeof(Se_vol);
	case 0x81: return sizeof(Sce_Item_cmp);
	case 0x82: return sizeof(Sce_espr_kill_all);
	case 0x83: return sizeof(Plc_heal);
	case 0x84: return sizeof(St_map_hint);
	case 0x85: return sizeof(Sce_em_pos_ck);
	case 0x86: return sizeof(Poison_ck);
	case 0x87: return sizeof(Poison_clr);
	case 0x88: return sizeof(Sce_Item_lost2);
	case 0x89: return sizeof(Plc_life_max);
	case 0x8A: return sizeof(Vib_set0);
	case 0x8B: return sizeof(Vib_set1);
	case 0x8C: return sizeof(Vib_fade_set);
	case 0x8D: return sizeof(Item_aot_set2);
	case 0x8E: return sizeof(Sce_em_set2);
	case 0x8F: return sizeof(Nop);
	}
	return 0xBEEFBABE;
}

const bool Resident_Evil_2_Bytecode::Open(StdFile& File, std::uintmax_t Ptr)
{
	if (!File.IsOpen() || !File.Open(File.GetPath(), FileAccessMode::Read, true, false))
	{
		Standard_String().Message(L"Resident Evil 2 SCD Error: could not read script \"%ws\"", File.GetPath().filename().wstring().c_str());
		return false;
	}

	Close();

	std::int8_t Opcode = 0;

	std::uintmax_t FileSize = File.Size();

	do {

		File.Read(Ptr, &Opcode, sizeof(std::int8_t));

		std::size_t Size = BytecodeSize(Opcode);

		if (Ptr + Size > FileSize || Size == 0xBEEFBABE)
		{
			break;
		}

		std::vector<std::uint8_t> Data(Size);

		File.Read(Ptr, Data.data(), Data.size());

		m_Bytecode.push_back(Bytecode(Data));

		Ptr += Data.size();

	} while ((Ptr < FileSize) && (Ptr != FileSize) && (!File.eof()));

	BuildContiguousData();

	return true;
}

const bool Resident_Evil_2_Bytecode::Open(std::vector<std::uint8_t> Buffer)
{
	Close();

	std::uintmax_t Ptr = 0;

	std::uintmax_t FileSize = Buffer.size();

	do {
		std::int8_t Opcode = Buffer[Ptr];

		std::size_t Size = BytecodeSize(Opcode);
		
		if (Ptr + Size > Buffer.size() || Size == 0xBEEFBABE)
		{
			break;
		}

		std::vector<std::uint8_t> Data(Size);

		std::copy(Buffer.begin() + Ptr, Buffer.begin() + Ptr + Data.size(), Data.begin());

		m_Bytecode.push_back(Bytecode(Data));

		Ptr += Data.size();

	} while (Ptr < Buffer.size() && (Ptr != FileSize));

	BuildContiguousData();

	return true;
}

const bool Resident_Evil_2_Bytecode::Disassemble(std::filesystem::path _Path)
{
	std::unique_ptr<StdText> Text = std::make_unique<StdText>(_Path, FileAccessMode::Write, TextFileBOM::UTF8);
	if (!Text->IsOpen())
	{
		Standard_String().Message(L"Resident Evil 2 SCD Error: could not create script disassembly \"%ws\"", _Path.filename().wstring().c_str());
		return false;
	}

	for (auto& Bytecode : m_Bytecode)
	{
		Text->AddLine("%s(", Bytecode.OpcodeName().c_str());

		size_t VariableCount = Bytecode.Count();

		for (size_t i = 1; i < VariableCount; i++)
		{
			if (Bytecode.IsSigned(i) && Bytecode.VariableSize(i) == sizeof(CHAR))
			{
				Text->AddLine("%d", Bytecode.Data<CHAR>(i));
			}
			else if (Bytecode.IsSigned(i) && Bytecode.VariableSize(i) == sizeof(SHORT))
			{
				Text->AddLine("%d", Bytecode.Data<SHORT>(i));
			}
			else if (!Bytecode.IsSigned(i) && Bytecode.VariableSize(i) == sizeof(UCHAR))
			{
				Text->AddLine("0x%02X", Bytecode.Data<UCHAR>(i));
			}
			else if (!Bytecode.IsSigned(i) && Bytecode.VariableSize(i) == sizeof(USHORT))
			{
				Text->AddLine("0x%04X", Bytecode.Data<USHORT>(i));
			}

			if (i + 1 < VariableCount)
			{
				Text->AddLine(", ");
			}
		}

		Text->AddLine(")\r");
	}

	Text->FlushUTF8();

	return true;
}

void Resident_Evil_2_Bytecode::Close(void)
{
	for (auto& Bytecode : m_Bytecode)
	{
		Bytecode.Close();
	}

	m_Bytecode.clear();
	m_Bytecode.shrink_to_fit();

	m_Contiguous.clear();
	m_Contiguous.shrink_to_fit();
}