/*
*
*	Megan Grass
*	May 30, 2024
*
*/


#pragma once

#include <array>

#include <unordered_map>

#include <string>

#include <std_data_types.h>
#include <std_defines.h>
#include <std_cstyle.h>


struct OpcodeVariable
{
	std::size_t Offset;
	bool Signed;
	std::size_t Size;
	std::string Name;
};

struct OpcodeDetails
{
	std::string Name;
	std::vector<OpcodeVariable> Meta;
};


std::array<OpcodeVariable, 1> iNop =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 2> iEvt_end =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"}
};

std::array<OpcodeVariable, 1> iEvt_next =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 4> iEvt_chain =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Gosub"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Evt_no"}
};

std::array<OpcodeVariable, 4> iEvt_exec =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Task_level"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Gosub"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Evt_no"}
};

std::array<OpcodeVariable, 2> iEvt_kill =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Evt_no"}
};

std::array<OpcodeVariable, 3> iIfel_ck =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(USHORT), "Ofs"}
};

std::array<OpcodeVariable, 3> iElse_ck =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(USHORT), "Ofs"}
};

std::array<OpcodeVariable, 2> iEndif =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"}
};

std::array<OpcodeVariable, 3> iSleep =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Sleeping"},
	OpcodeVariable{2, false, sizeof(USHORT), "Lcnt"}
};

std::array<OpcodeVariable, 2> iSleeping =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(USHORT), "Lcnt"}
};

std::array<OpcodeVariable, 1> iWsleep =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 1> iWsleeping =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 4> iFor =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(USHORT), "Ofs"},
	OpcodeVariable{4, false, sizeof(USHORT), "Lcnt"}
};

std::array<OpcodeVariable, 2> iNext =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"}
};

std::array<OpcodeVariable, 3> iWhile =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "ConditionLen"},
	OpcodeVariable{2, false, sizeof(USHORT), "Ofs"}
};

std::array<OpcodeVariable, 2> iEwhile =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"}
};

std::array<OpcodeVariable, 3> iDo =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(USHORT), "Ofs"}
};

std::array<OpcodeVariable, 2> iEdwhile =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "ConditionLen"}
};

std::array<OpcodeVariable, 3> iSwitch =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Work_no"},
	OpcodeVariable{2, false, sizeof(USHORT), "Ofs"}
};

std::array<OpcodeVariable, 4> iCase =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(USHORT), "Ofs"},
	OpcodeVariable{4, false, sizeof(USHORT), "Expression"}
};

std::array<OpcodeVariable, 2> iDefault =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"}
};

std::array<OpcodeVariable, 2> iEswitch =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"}
};

std::array<OpcodeVariable, 5> iGoto =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Ifel_ctr"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Loop_ctr"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{4, true, sizeof(SHORT), "Ofs"}
};

std::array<OpcodeVariable, 2> iGosub =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Evt_no"}
};

std::array<OpcodeVariable, 2> iReturn =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"}
};

std::array<OpcodeVariable, 2> iBreak =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"}
};

std::array<OpcodeVariable, 5> iFor2 =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop0"},
	OpcodeVariable{2, true, sizeof(SHORT), "Ofs"},
	OpcodeVariable{4, false, sizeof(UCHAR), "Nop1"},
	OpcodeVariable{5, false, sizeof(UCHAR), "Lcnt"}
};

std::array<OpcodeVariable, 1> iBreak_point =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 4> iWork_copy =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Work_no"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Work_dest"},
	OpcodeVariable{3, false, sizeof(UCHAR), "DataType"}
};

std::array<OpcodeVariable, 4> iCk =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Flag"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Bit"},
	OpcodeVariable{3, false, sizeof(UCHAR), "OnOff"}
};

std::array<OpcodeVariable, 4> iSet =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Flag"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Bit"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Operator"}
};

std::array<OpcodeVariable, 5> iCmp =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Flag"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Operator"},
	OpcodeVariable{4, true, sizeof(SHORT), "Num"}
};

std::array<OpcodeVariable, 3> iSave =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Flag"},
	OpcodeVariable{2, true, sizeof(SHORT), "Num"}
};

std::array<OpcodeVariable, 3> iCopy =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Flag0"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Flag1"}
};

std::array<OpcodeVariable, 5> iCalc =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Operator"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Flag"},
	OpcodeVariable{4, true, sizeof(SHORT), "Num"}
};

std::array<OpcodeVariable, 4> iCalc2 =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Operator"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Flag0"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Flag1"}
};

std::array<OpcodeVariable, 1> iSce_rnd =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 2> iCut_chg =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Cut_no"}
};

std::array<OpcodeVariable, 1> iCut_old =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 5> iMessage_on =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Mess_no"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Attr"},
	OpcodeVariable{4, false, sizeof(USHORT), "Stop_data"}
};

std::array<OpcodeVariable, 13> iAot_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Aot"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Id"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Type"},
	OpcodeVariable{4, false, sizeof(UCHAR), "nFloor"},
	OpcodeVariable{5, false, sizeof(UCHAR), "Super"},
	OpcodeVariable{6, true, sizeof(SHORT), "X"},
	OpcodeVariable{8, true, sizeof(SHORT), "Z"},
	OpcodeVariable{10, false, sizeof(USHORT), "W"},
	OpcodeVariable{12, false, sizeof(USHORT), "D"},
	OpcodeVariable{14, false, sizeof(USHORT), "Data0"},
	OpcodeVariable{16, false, sizeof(USHORT), "Data1"},
	OpcodeVariable{18, false, sizeof(USHORT), "Data2"}
};

std::array<OpcodeVariable, 23> iObj_model_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Om_no"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Id"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Cc_ctr"},
	OpcodeVariable{4, false, sizeof(UCHAR), "Cc_wait"},
	OpcodeVariable{5, false, sizeof(UCHAR), "Cc_num"},
	OpcodeVariable{6, false, sizeof(UCHAR), "nFloor"},
	OpcodeVariable{7, false, sizeof(UCHAR), "Super"},
	OpcodeVariable{8, false, sizeof(USHORT), "Type"},
	OpcodeVariable{10, false, sizeof(USHORT), "Be_flg"},
	OpcodeVariable{12, true, sizeof(SHORT), "Attribute"},
	OpcodeVariable{14, true, sizeof(SHORT), "Pos_x"},
	OpcodeVariable{16, true, sizeof(SHORT), "Pos_y"},
	OpcodeVariable{18, true, sizeof(SHORT), "Pos_z"},
	OpcodeVariable{20, true, sizeof(SHORT), "Cdir_x"},
	OpcodeVariable{22, true, sizeof(SHORT), "Cdir_y"},
	OpcodeVariable{24, true, sizeof(SHORT), "Cdir_z"},
	OpcodeVariable{26, true, sizeof(SHORT), "Ofs_x"},
	OpcodeVariable{28, true, sizeof(SHORT), "Ofs_y"},
	OpcodeVariable{30, true, sizeof(SHORT), "Ofs_z"},
	OpcodeVariable{32, true, sizeof(SHORT), "At_w"},
	OpcodeVariable{34, true, sizeof(SHORT), "At_h"},
	OpcodeVariable{36, true, sizeof(SHORT), "At_d"}
};

std::array<OpcodeVariable, 3> iWork_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Work_type"},
	OpcodeVariable{2, true, sizeof(CHAR), "Work_no"}
};

std::array<OpcodeVariable, 3> iSpeed_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Spd_type"},
	OpcodeVariable{2, false, sizeof(USHORT), "Num"}
};

std::array<OpcodeVariable, 1> iAdd_speed =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 1> iAdd_aspeed =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 5> iPos_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, true, sizeof(SHORT), "Pos_x"},
	OpcodeVariable{4, true, sizeof(SHORT), "Pos_y"},
	OpcodeVariable{6, true, sizeof(SHORT), "Pos_z"}
};

std::array<OpcodeVariable, 5> iDir_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, true, sizeof(SHORT), "Cdir_x"},
	OpcodeVariable{4, true, sizeof(SHORT), "Cdir_y"},
	OpcodeVariable{6, true, sizeof(SHORT), "Cdir_z"}
};

std::array<OpcodeVariable, 3> iMember_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Member"},
	OpcodeVariable{2, true, sizeof(SHORT), "Num"}
};

std::array<OpcodeVariable, 3> iMember_set2 =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Member"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Flag"}
};

std::array<OpcodeVariable, 7> iSe_on =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "VAB"},
	OpcodeVariable{2, true, sizeof(SHORT), "EDT"},
	OpcodeVariable{4, true, sizeof(SHORT), "Work_type"},
	OpcodeVariable{6, true, sizeof(SHORT), "Pos_x"},
	OpcodeVariable{8, true, sizeof(SHORT), "Pos_y"},
	OpcodeVariable{10, true, sizeof(SHORT), "Pos_z"}
};

std::array<OpcodeVariable, 3> iSca_id_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Col_no"},
	OpcodeVariable{2, false, sizeof(USHORT), "Id"}
};

std::array<OpcodeVariable, 3> iFlr_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Flr_no"},
	OpcodeVariable{2, false, sizeof(UCHAR), "OnOff"}
};

std::array<OpcodeVariable, 5> iDir_ck =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, true, sizeof(SHORT), "Vec_x"},
	OpcodeVariable{4, true, sizeof(SHORT), "Vec_z"},
	OpcodeVariable{6, true, sizeof(SHORT), "Add_dir"}
};

std::array<OpcodeVariable, 11> iSce_espr_on =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Esp_id"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Esp_dt"},
	OpcodeVariable{4, false, sizeof(UCHAR), "Work_kind"},
	OpcodeVariable{5, false, sizeof(UCHAR), "Work_no"},
	OpcodeVariable{6, false, sizeof(USHORT), "Espmv"},
	OpcodeVariable{8, true, sizeof(SHORT), "Svec_x"},
	OpcodeVariable{10, true, sizeof(SHORT), "Svec_y"},
	OpcodeVariable{12, true, sizeof(SHORT), "Svec_z"},
	OpcodeVariable{14, false, sizeof(USHORT), "Dir_y"}
};

std::array<OpcodeVariable, 24> iDoor_aot_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Aot"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Id"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Type"},
	OpcodeVariable{4, false, sizeof(UCHAR), "nFloor"},
	OpcodeVariable{5, false, sizeof(UCHAR), "Super"},
	OpcodeVariable{6, true, sizeof(SHORT), "X"},
	OpcodeVariable{8, true, sizeof(SHORT), "Z"},
	OpcodeVariable{10, false, sizeof(USHORT), "W"},
	OpcodeVariable{12, false, sizeof(USHORT), "D"},
	OpcodeVariable{14, true, sizeof(SHORT), "Next_pos_x"},
	OpcodeVariable{16, true, sizeof(SHORT), "Next_pos_y"},
	OpcodeVariable{18, true, sizeof(SHORT), "Next_pos_z"},
	OpcodeVariable{20, true, sizeof(SHORT), "Next_cdir_y"},
	OpcodeVariable{22, false, sizeof(UCHAR), "Next_stage"},
	OpcodeVariable{23, false, sizeof(UCHAR), "Next_room"},
	OpcodeVariable{24, false, sizeof(UCHAR), "Next_cut"},
	OpcodeVariable{25, false, sizeof(UCHAR), "Next_nfloor"},
	OpcodeVariable{26, false, sizeof(UCHAR), "Dtex_type"},
	OpcodeVariable{27, false, sizeof(UCHAR), "Door_type"},
	OpcodeVariable{28, false, sizeof(UCHAR), "Knock_type"},
	OpcodeVariable{29, false, sizeof(UCHAR), "Key_id"},
	OpcodeVariable{30, false, sizeof(UCHAR), "Key_type"},
	OpcodeVariable{31, false, sizeof(UCHAR), "Free"}
};

std::array<OpcodeVariable, 2> iCut_auto =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "OnOff"}
};

std::array<OpcodeVariable, 3> iMember_copy =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, true, sizeof(CHAR), "Flag"},
	OpcodeVariable{2, true, sizeof(CHAR), "Member"}
};

std::array<OpcodeVariable, 5> iMember_cmp =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Member"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Operator"},
	OpcodeVariable{4, true, sizeof(SHORT), "Num"}
};

std::array<OpcodeVariable, 4> iPlc_motion =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Routine_0"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Move_no"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Sce_flg"}
};

std::array<OpcodeVariable, 6> iPlc_dest =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Routine_0"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Set_flg_no"},
	OpcodeVariable{4, true, sizeof(SHORT), "Dest_x"},
	OpcodeVariable{6, true, sizeof(SHORT), "Dest_z"}
};

std::array<OpcodeVariable, 6> iPlc_neck =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Neck_flg"},
	OpcodeVariable{2, true, sizeof(SHORT), "Neck_point_x"},
	OpcodeVariable{4, true, sizeof(SHORT), "Neck_point_y"},
	OpcodeVariable{6, true, sizeof(SHORT), "Neck_point_z"},
	OpcodeVariable{8, true, sizeof(SHORT), "Free"}
};

std::array<OpcodeVariable, 1> iPlc_ret =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 3> iPlc_flg =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Operator"},
	OpcodeVariable{2, false, sizeof(USHORT), "Num"}
};

std::array<OpcodeVariable, 15> iSce_em_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, true, sizeof(CHAR), "Em_no"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Id"},
	OpcodeVariable{4, false, sizeof(USHORT), "Type"},
	OpcodeVariable{6, false, sizeof(UCHAR), "nFloor"},
	OpcodeVariable{7, false, sizeof(UCHAR), "Sound_flg"},
	OpcodeVariable{8, false, sizeof(UCHAR), "Model_type"},
	OpcodeVariable{9, false, sizeof(UCHAR), "Em_set_flg"},
	OpcodeVariable{10, true, sizeof(SHORT), "Pos_x"},
	OpcodeVariable{12, true, sizeof(SHORT), "Pos_y"},
	OpcodeVariable{14, true, sizeof(SHORT), "Pos_z"},
	OpcodeVariable{16, true, sizeof(SHORT), "Cdir_y"},
	OpcodeVariable{18, true, sizeof(SHORT), "Motion"},
	OpcodeVariable{20, true, sizeof(SHORT), "Ctr_flg"}
};

std::array<OpcodeVariable, 5> iCol_chg_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Cc_ctr"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Cc_cnt"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Cc_wait"},
	OpcodeVariable{4, false, sizeof(UCHAR), "Cc_num"}
};

std::array<OpcodeVariable, 7> iAot_reset =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Aot"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Id"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Type"},
	OpcodeVariable{4, false, sizeof(USHORT), "Data0"},
	OpcodeVariable{6, false, sizeof(USHORT), "Data1"},
	OpcodeVariable{8, false, sizeof(USHORT), "Data2"}
};

std::array<OpcodeVariable, 2> iAot_on =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Aot"}
};

std::array<OpcodeVariable, 10> iSuper_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Work_type"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Work_no"},
	OpcodeVariable{4, true, sizeof(SHORT), "Pos_x"},
	OpcodeVariable{6, true, sizeof(SHORT), "Pos_y"},
	OpcodeVariable{8, true, sizeof(SHORT), "Pos_z"},
	OpcodeVariable{10, true, sizeof(SHORT), "Cdir_x"},
	OpcodeVariable{12, true, sizeof(SHORT), "Cdir_y"},
	OpcodeVariable{14, true, sizeof(SHORT), "Cdir_z"}
};

std::array<OpcodeVariable, 5> iSuper_reset =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, true, sizeof(SHORT), "Cdir_x"},
	OpcodeVariable{4, true, sizeof(SHORT), "Cdir_y"},
	OpcodeVariable{6, true, sizeof(SHORT), "Cdir_z"}
};

std::array<OpcodeVariable, 2> iPlc_gun =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Routine_0"}
};

std::array<OpcodeVariable, 3> iCut_replace =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Fcut"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Tcut"}
};

std::array<OpcodeVariable, 5> iSce_espr_kill =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Esp_id"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Esp_type"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Work_kind"},
	OpcodeVariable{4, false, sizeof(UCHAR), "Work_no"}
};

std::array<OpcodeVariable, 14> iDoor_model_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Work_no"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Id"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Type"},
	OpcodeVariable{4, false, sizeof(UCHAR), "Be_flg"},
	OpcodeVariable{5, false, sizeof(UCHAR), "Model_no"},
	OpcodeVariable{6, false, sizeof(USHORT), "Attribute2"},
	OpcodeVariable{8, false, sizeof(USHORT), "Attribute"},
	OpcodeVariable{10, true, sizeof(SHORT), "Pos_x"},
	OpcodeVariable{12, true, sizeof(SHORT), "Pos_y"},
	OpcodeVariable{14, true, sizeof(SHORT), "Pos_z"},
	OpcodeVariable{16, true, sizeof(SHORT), "Cdir_x"},
	OpcodeVariable{18, true, sizeof(SHORT), "Cdir_y"},
	OpcodeVariable{20, true, sizeof(SHORT), "Cdir_z"}
};

std::array<OpcodeVariable, 15> iItem_aot_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, true, sizeof(CHAR), "Aot"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Id"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Type"},
	OpcodeVariable{4, false, sizeof(UCHAR), "nFloor"},
	OpcodeVariable{5, false, sizeof(UCHAR), "Super"},
	OpcodeVariable{6, true, sizeof(SHORT), "X"},
	OpcodeVariable{8, true, sizeof(SHORT), "Z"},
	OpcodeVariable{10, false, sizeof(USHORT), "W"},
	OpcodeVariable{12, false, sizeof(USHORT), "D"},
	OpcodeVariable{14, false, sizeof(USHORT), "iItem"},
	OpcodeVariable{16, false, sizeof(USHORT), "nItem"},
	OpcodeVariable{18, false, sizeof(USHORT), "Item_flg"},
	OpcodeVariable{20, false, sizeof(UCHAR), "Om_no"},
	OpcodeVariable{21, false, sizeof(UCHAR), "Action"}
};

std::array<OpcodeVariable, 3> iSce_key_ck =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Flag"},
	OpcodeVariable{2, false, sizeof(USHORT), "Key"}
};

std::array<OpcodeVariable, 3> iSce_trg_ck =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Flag"},
	OpcodeVariable{2, false, sizeof(USHORT), "Key_trg"}
};

std::array<OpcodeVariable, 6> iSce_bgm_control =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Work_no"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Op"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Data0"},
	OpcodeVariable{4, false, sizeof(UCHAR), "Data1"},
	OpcodeVariable{5, false, sizeof(UCHAR), "Data2"}
};

std::array<OpcodeVariable, 6> iSce_espr_control =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Esp_id"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Esp_type"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Esp_rtn"},
	OpcodeVariable{4, false, sizeof(UCHAR), "Work_kind"},
	OpcodeVariable{5, false, sizeof(UCHAR), "Work_no"}
};

std::array<OpcodeVariable, 5> iSce_fade_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Work_no"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Hrate"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Mask_rgb"},
	OpcodeVariable{4, true, sizeof(SHORT), "Add"}
};

std::array<OpcodeVariable, 14> iSce_espr3d_on =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Esp_id"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Esp_dt"},
	OpcodeVariable{4, false, sizeof(UCHAR), "Work_kind"},
	OpcodeVariable{5, false, sizeof(UCHAR), "Work_no"},
	OpcodeVariable{6, false, sizeof(USHORT), "Espmv"},
	OpcodeVariable{8, true, sizeof(SHORT), "Svec_x"},
	OpcodeVariable{10, true, sizeof(SHORT), "Svec_y"},
	OpcodeVariable{12, true, sizeof(SHORT), "Svec_z"},
	OpcodeVariable{14, true, sizeof(SHORT), "Dsv_x"},
	OpcodeVariable{16, true, sizeof(SHORT), "Dsv_y"},
	OpcodeVariable{18, true, sizeof(SHORT), "Dsv_z"},
	OpcodeVariable{20, true, sizeof(SHORT), "Dir_y"}
};

std::array<OpcodeVariable, 5> iMember_calc =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Operator"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Member"},
	OpcodeVariable{4, true, sizeof(SHORT), "Num"}
};

std::array<OpcodeVariable, 4> iMember_calc2 =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Operator"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Member"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Work_no"}
};

std::array<OpcodeVariable, 7> iSce_bgmtbl_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Stage"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Room"},
	OpcodeVariable{4, false, sizeof(UCHAR), "Main"},
	OpcodeVariable{5, false, sizeof(UCHAR), "Sub"},
	OpcodeVariable{6, false, sizeof(USHORT), "PANVT"}
};

std::array<OpcodeVariable, 3> iPlc_rot =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Free_no"},
	OpcodeVariable{2, false, sizeof(USHORT), "Sce_free"}
};

std::array<OpcodeVariable, 3> iXa_on =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Mode"},
	OpcodeVariable{2, false, sizeof(USHORT), "Num"}
};

std::array<OpcodeVariable, 2> iWeapon_chg =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Plw_no"}
};

std::array<OpcodeVariable, 2> iPlc_cnt =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Move_cnt"}
};

std::array<OpcodeVariable, 3> iSce_shake_on =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, true, sizeof(CHAR), "Slide_ofs"},
	OpcodeVariable{2, true, sizeof(CHAR), "Copy_ofs"}
};

std::array<OpcodeVariable, 2> iMizu_div_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Div_max"}
};

std::array<OpcodeVariable, 2> iKeep_Item_ck =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Item_no"}
};

std::array<OpcodeVariable, 2> iXa_vol =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Cd_vol"}
};

std::array<OpcodeVariable, 10> iKage_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Work_type"},
	OpcodeVariable{2, true, sizeof(CHAR), "Work_no"},
	OpcodeVariable{3, false, sizeof(UCHAR), "R"},
	OpcodeVariable{4, false, sizeof(UCHAR), "G"},
	OpcodeVariable{5, false, sizeof(UCHAR), "B"},
	OpcodeVariable{6, false, sizeof(USHORT), "Half_x"},
	OpcodeVariable{8, false, sizeof(USHORT), "Half_z"},
	OpcodeVariable{10, false, sizeof(USHORT), "Off_x"},
	OpcodeVariable{12, false, sizeof(USHORT), "Off_z"}
};

std::array<OpcodeVariable, 4> iCut_be_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Fcut"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Vcut_num"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Be_flg"}
};

std::array<OpcodeVariable, 2> iSce_Item_lost =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Item_no"}
};

std::array<OpcodeVariable, 1> iPlc_gun_eff =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 11> iSce_espr_on2 =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Dir_y_id0"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Esp_id"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Esp_dt"},
	OpcodeVariable{4, false, sizeof(UCHAR), "Work_kind"},
	OpcodeVariable{5, false, sizeof(UCHAR), "Work_no"},
	OpcodeVariable{6, false, sizeof(USHORT), "Espmv"},
	OpcodeVariable{8, true, sizeof(SHORT), "Svec_x"},
	OpcodeVariable{10, true, sizeof(SHORT), "Svec_y"},
	OpcodeVariable{12, true, sizeof(SHORT), "Svec_z"},
	OpcodeVariable{14, false, sizeof(USHORT), "Dir_y_id1"}
};

std::array<OpcodeVariable, 2> iSce_espr_kill2 =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Esp_id"}
};

std::array<OpcodeVariable, 1> iPlc_stop =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 17> iAot_set_4p =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Aot"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Id"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Type"},
	OpcodeVariable{4, false, sizeof(UCHAR), "nFloor"},
	OpcodeVariable{5, false, sizeof(UCHAR), "Super"},
	OpcodeVariable{6, true, sizeof(SHORT), "Xz[0][0]"},
	OpcodeVariable{8, true, sizeof(SHORT), "Xz[0][1]"},
	OpcodeVariable{10, true, sizeof(SHORT), "Xz[1][0]"},
	OpcodeVariable{12, true, sizeof(SHORT), "Xz[1][1]"},
	OpcodeVariable{14, true, sizeof(SHORT), "Xz[2][0]"},
	OpcodeVariable{16, true, sizeof(SHORT), "Xz[2][1]"},
	OpcodeVariable{18, true, sizeof(SHORT), "Xz[3][0]"},
	OpcodeVariable{20, true, sizeof(SHORT), "Xz[3][1]"},
	OpcodeVariable{22, false, sizeof(USHORT), "Data0"},
	OpcodeVariable{24, false, sizeof(USHORT), "Data1"},
	OpcodeVariable{26, false, sizeof(USHORT), "Data2"}
};

std::array<OpcodeVariable, 28> iDoor_aot_set_4p =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Aot"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Id"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Type"},
	OpcodeVariable{4, false, sizeof(UCHAR), "nFloor"},
	OpcodeVariable{5, false, sizeof(UCHAR), "Super"},
	OpcodeVariable{6, true, sizeof(SHORT), "Xz[0][0]"},
	OpcodeVariable{8, true, sizeof(SHORT), "Xz[0][1]"},
	OpcodeVariable{10, true, sizeof(SHORT), "Xz[1][0]"},
	OpcodeVariable{12, true, sizeof(SHORT), "Xz[1][1]"},
	OpcodeVariable{14, true, sizeof(SHORT), "Xz[2][0]"},
	OpcodeVariable{16, true, sizeof(SHORT), "Xz[2][1]"},
	OpcodeVariable{18, true, sizeof(SHORT), "Xz[3][0]"},
	OpcodeVariable{20, true, sizeof(SHORT), "Xz[3][1]"},
	OpcodeVariable{22, true, sizeof(SHORT), "Next_pos_x"},
	OpcodeVariable{24, true, sizeof(SHORT), "Next_pos_y"},
	OpcodeVariable{26, true, sizeof(SHORT), "Next_pos_z"},
	OpcodeVariable{28, true, sizeof(SHORT), "Next_cdir_y"},
	OpcodeVariable{30, false, sizeof(UCHAR), "Next_stage"},
	OpcodeVariable{31, false, sizeof(UCHAR), "Next_room"},
	OpcodeVariable{32, false, sizeof(UCHAR), "Next_cut"},
	OpcodeVariable{33, false, sizeof(UCHAR), "Next_nfloor"},
	OpcodeVariable{34, false, sizeof(UCHAR), "Dtex_type"},
	OpcodeVariable{35, false, sizeof(UCHAR), "Door_type"},
	OpcodeVariable{36, false, sizeof(UCHAR), "Knock_type"},
	OpcodeVariable{37, false, sizeof(UCHAR), "Key_id"},
	OpcodeVariable{38, false, sizeof(UCHAR), "Key_type"},
	OpcodeVariable{39, false, sizeof(UCHAR), "Free"}
};

std::array<OpcodeVariable, 19> iItem_aot_set_4p =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Aot"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Id"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Type"},
	OpcodeVariable{4, false, sizeof(UCHAR), "nFloor"},
	OpcodeVariable{5, false, sizeof(UCHAR), "Super"},
	OpcodeVariable{6, true, sizeof(SHORT), "Xz[0][0]"},
	OpcodeVariable{8, true, sizeof(SHORT), "Xz[0][1]"},
	OpcodeVariable{10, true, sizeof(SHORT), "Xz[1][0]"},
	OpcodeVariable{12, true, sizeof(SHORT), "Xz[1][1]"},
	OpcodeVariable{14, true, sizeof(SHORT), "Xz[2][0]"},
	OpcodeVariable{16, true, sizeof(SHORT), "Xz[2][1]"},
	OpcodeVariable{18, true, sizeof(SHORT), "Xz[3][0]"},
	OpcodeVariable{20, true, sizeof(SHORT), "Xz[3][1]"},
	OpcodeVariable{22, false, sizeof(USHORT), "iItem"},
	OpcodeVariable{24, false, sizeof(USHORT), "nItem"},
	OpcodeVariable{26, false, sizeof(USHORT), "Item_flg"},
	OpcodeVariable{28, false, sizeof(UCHAR), "Om_no"},
	OpcodeVariable{29, false, sizeof(UCHAR), "Action"}
};

std::array<OpcodeVariable, 5> iLight_pos_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Pos_no"},
	OpcodeVariable{3, false, sizeof(UCHAR), "XYZ"},
	OpcodeVariable{4, true, sizeof(SHORT), "Position"}
};

std::array<OpcodeVariable, 3> iLight_kido_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Brit_no"},
	OpcodeVariable{2, false, sizeof(USHORT), "Brightness"}
};

std::array<OpcodeVariable, 1> iRbj_reset =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 3> iSce_scr_move =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, true, sizeof(SHORT), "Scrl_y"}
};

std::array<OpcodeVariable, 5> iParts_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Ptr"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Work_type"},
	OpcodeVariable{4, true, sizeof(SHORT), "Num"}
};

std::array<OpcodeVariable, 2> iMovie_on =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Str_num"}
};

std::array<OpcodeVariable, 1> iSplc_ret =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 1> iSplc_sce =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 10> iSuper_on =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Work_type"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Work_no"},
	OpcodeVariable{4, true, sizeof(SHORT), "Mat_x"},
	OpcodeVariable{6, true, sizeof(SHORT), "Mat_y"},
	OpcodeVariable{8, true, sizeof(SHORT), "Mat_z"},
	OpcodeVariable{10, true, sizeof(SHORT), "Rot_x"},
	OpcodeVariable{12, true, sizeof(SHORT), "Rot_y"},
	OpcodeVariable{14, true, sizeof(SHORT), "Rot_z"}
};

std::array<OpcodeVariable, 5> iMirror_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Flag"},
	OpcodeVariable{2, false, sizeof(USHORT), "Position"},
	OpcodeVariable{4, false, sizeof(USHORT), "Min"},
	OpcodeVariable{6, false, sizeof(USHORT), "Max"}
};

std::array<OpcodeVariable, 3> iSce_fade_adjust =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "No"},
	OpcodeVariable{2, true, sizeof(SHORT), "Brightness"}
};

std::array<OpcodeVariable, 14> iSce_espr3d_on2 =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Dir_y_id0"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Esp_id"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Esp_dt"},
	OpcodeVariable{4, false, sizeof(UCHAR), "Work_kind"},
	OpcodeVariable{5, false, sizeof(UCHAR), "Work_no"},
	OpcodeVariable{6, false, sizeof(USHORT), "Espmv"},
	OpcodeVariable{8, true, sizeof(SHORT), "Svec_x"},
	OpcodeVariable{10, true, sizeof(SHORT), "Svec_y"},
	OpcodeVariable{12, true, sizeof(SHORT), "Svec_z"},
	OpcodeVariable{14, true, sizeof(SHORT), "Dsv_x"},
	OpcodeVariable{16, true, sizeof(SHORT), "Dsv_y"},
	OpcodeVariable{18, true, sizeof(SHORT), "Dsv_z"},
	OpcodeVariable{20, false, sizeof(USHORT), "Dir_y_id1"}
};

std::array<OpcodeVariable, 3> iSce_Item_get =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Item_no"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Num"}
};

std::array<OpcodeVariable, 3> iSce_line_start =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Ot_no"},
	OpcodeVariable{2, false, sizeof(USHORT), "Prio"}
};

std::array<OpcodeVariable, 4> iSce_line_main =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Line_no"},
	OpcodeVariable{2, true, sizeof(SHORT), "Slide_ofs"},
	OpcodeVariable{4, true, sizeof(SHORT), "Copy_ofs"}
};

std::array<OpcodeVariable, 1> iSce_line_end =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 11> iSce_parts_bomb =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Ptr"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Poly_R"},
	OpcodeVariable{4, false, sizeof(UCHAR), "Poly_G"},
	OpcodeVariable{5, false, sizeof(UCHAR), "Poly_B"},
	OpcodeVariable{6, true, sizeof(SHORT), "Free0"},
	OpcodeVariable{8, true, sizeof(SHORT), "Free2"},
	OpcodeVariable{10, true, sizeof(SHORT), "Free6"},
	OpcodeVariable{12, true, sizeof(SHORT), "Free1"},
	OpcodeVariable{14, true, sizeof(SHORT), "Free3"}
};

std::array<OpcodeVariable, 9> iSce_parts_down =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Ptr"},
	OpcodeVariable{2, true, sizeof(SHORT), "Old_x"},
	OpcodeVariable{4, true, sizeof(SHORT), "Old_y"},
	OpcodeVariable{6, true, sizeof(SHORT), "Old_z"},
	OpcodeVariable{8, true, sizeof(SHORT), "Timer1"},
	OpcodeVariable{10, true, sizeof(SHORT), "Old_x2"},
	OpcodeVariable{12, true, sizeof(SHORT), "Old_y2"},
	OpcodeVariable{14, true, sizeof(SHORT), "Old_z2"}
};

std::array<OpcodeVariable, 6> iLight_color_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Col_no"},
	OpcodeVariable{2, false, sizeof(UCHAR), "R"},
	OpcodeVariable{3, false, sizeof(UCHAR), "G"},
	OpcodeVariable{4, false, sizeof(UCHAR), "B"},
	OpcodeVariable{5, false, sizeof(UCHAR), "Nop"}
};

std::array<OpcodeVariable, 5> iLight_pos_set2 =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Lit_no"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Pos_no"},
	OpcodeVariable{3, false, sizeof(UCHAR), "XYZ"},
	OpcodeVariable{4, true, sizeof(SHORT), "Position"}
};

std::array<OpcodeVariable, 5> iLight_kido_set2 =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Lit_no"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Brit_no"},
	OpcodeVariable{4, false, sizeof(USHORT), "Brightness"}
};

std::array<OpcodeVariable, 6> iLight_color_set2 =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Lit_no"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Col_no"},
	OpcodeVariable{3, false, sizeof(UCHAR), "R"},
	OpcodeVariable{4, false, sizeof(UCHAR), "G"},
	OpcodeVariable{5, false, sizeof(UCHAR), "B"}
};

std::array<OpcodeVariable, 2> iSe_vol =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Volume_Se"}
};

std::array<OpcodeVariable, 3> iSce_Item_cmp =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Operator"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Num"}
};

std::array<OpcodeVariable, 3> iSce_espr_kill_all =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, true, sizeof(CHAR), "Work_kind"},
	OpcodeVariable{2, true, sizeof(CHAR), "Work_no"}
};

std::array<OpcodeVariable, 1> iPlc_heal =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 2> iSt_map_hint =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Tag"}
};

std::array<OpcodeVariable, 5> iSce_em_pos_ck =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Em_no0"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Em_no1"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Flg"},
	OpcodeVariable{4, false, sizeof(USHORT), "Att"}
};

std::array<OpcodeVariable, 1> iPoison_ck =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 1> iPoison_clr =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 3> iSce_Item_lost2 =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Item_no"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Num"}
};

std::array<OpcodeVariable, 1> iPlc_life_max =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"}
};

std::array<OpcodeVariable, 4> iVib_set0 =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(USHORT), "data0"},
	OpcodeVariable{4, false, sizeof(USHORT), "data1"}
};

std::array<OpcodeVariable, 4> iVib_set1 =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Id"},
	OpcodeVariable{2, false, sizeof(USHORT), "data0"},
	OpcodeVariable{4, false, sizeof(USHORT), "data1"}
};

std::array<OpcodeVariable, 6> iVib_fade_set =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop"},
	OpcodeVariable{2, false, sizeof(UCHAR), "data0"},
	OpcodeVariable{3, false, sizeof(UCHAR), "data1"},
	OpcodeVariable{4, false, sizeof(USHORT), "data2"},
	OpcodeVariable{6, false, sizeof(USHORT), "data3"}
};

std::array<OpcodeVariable, 17> iItem_aot_set2 =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Aot"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Id"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Type"},
	OpcodeVariable{4, false, sizeof(UCHAR), "nFloor"},
	OpcodeVariable{5, false, sizeof(UCHAR), "Super"},
	OpcodeVariable{6, true, sizeof(SHORT), "X"},
	OpcodeVariable{8, true, sizeof(SHORT), "Z"},
	OpcodeVariable{10, false, sizeof(USHORT), "W"},
	OpcodeVariable{12, false, sizeof(USHORT), "D"},
	OpcodeVariable{14, false, sizeof(USHORT), "iItem"},
	OpcodeVariable{16, false, sizeof(USHORT), "nItem"},
	OpcodeVariable{18, false, sizeof(USHORT), "Flag"},
	OpcodeVariable{20, false, sizeof(UCHAR), "Om_no"},
	OpcodeVariable{21, false, sizeof(UCHAR), "Action"},
	OpcodeVariable{22, false, sizeof(UCHAR), "Rnd_flg"},
	OpcodeVariable{23, false, sizeof(UCHAR), "Nop"}
};

std::array<OpcodeVariable, 17> iSce_em_set2 =
{
	OpcodeVariable{0, false, sizeof(UCHAR), "Opcode"},
	OpcodeVariable{1, false, sizeof(UCHAR), "Nop0"},
	OpcodeVariable{2, false, sizeof(UCHAR), "Em_no"},
	OpcodeVariable{3, false, sizeof(UCHAR), "Id"},
	OpcodeVariable{4, false, sizeof(USHORT), "Type"},
	OpcodeVariable{6, false, sizeof(UCHAR), "nFloor"},
	OpcodeVariable{7, false, sizeof(UCHAR), "Sound_flg"},
	OpcodeVariable{8, false, sizeof(UCHAR), "Model_type"},
	OpcodeVariable{9, false, sizeof(UCHAR), "Em_set_flg"},
	OpcodeVariable{10, true, sizeof(SHORT), "Pos_x"},
	OpcodeVariable{12, true, sizeof(SHORT), "Pos_y"},
	OpcodeVariable{14, true, sizeof(SHORT), "Pos_z"},
	OpcodeVariable{16, true, sizeof(SHORT), "Cdir_y"},
	OpcodeVariable{18, true, sizeof(SHORT), "Motion"},
	OpcodeVariable{20, true, sizeof(SHORT), "Ctr_flg"},
	OpcodeVariable{22, false, sizeof(UCHAR), "Rnd_flg"},
	OpcodeVariable{23, false, sizeof(UCHAR), "Nop1"}
};

std::unordered_map<std::int8_t, OpcodeDetails> iBytecode =
{
	{ 0x00, { "Nop", { iNop.begin(), iNop.end() } } },
	{ 0x01, { "Evt_end", { iEvt_end.begin(), iEvt_end.end() } } },
	{ 0x02, { "Evt_next", { iEvt_next.begin(), iEvt_next.end() } } },
	{ 0x03, { "Evt_chain", { iEvt_chain.begin(), iEvt_chain.end() } } },
	{ 0x04, { "Evt_exec", { iEvt_exec.begin(), iEvt_exec.end() } } },
	{ 0x05, { "Evt_kill", { iEvt_kill.begin(), iEvt_kill.end() } } },
	{ 0x06, { "Ifel_ck", { iIfel_ck.begin(), iIfel_ck.end() } } },
	{ 0x07, { "Else_ck", { iElse_ck.begin(), iElse_ck.end() } } },
	{ 0x08, { "Endif", { iEndif.begin(), iEndif.end() } } },
	{ 0x09, { "Sleep", { iSleep.begin(), iSleep.end() } } },
	{ 0x0A, { "Sleeping", { iSleeping.begin(), iSleeping.end() } } },
	{ 0x0B, { "Wsleep", { iWsleep.begin(), iWsleep.end() } } },
	{ 0x0C, { "Wsleeping", { iWsleeping.begin(), iWsleeping.end() } } },
	{ 0x0D, { "For", { iFor.begin(), iFor.end() } } },
	{ 0x0E, { "Next", { iNext.begin(), iNext.end() } } },
	{ 0x0F, { "While", { iWhile.begin(), iWhile.end() } } },
	{ 0x10, { "Ewhile", { iEwhile.begin(), iEwhile.end() } } },
	{ 0x11, { "Do", { iDo.begin(), iDo.end() } } },
	{ 0x12, { "Edwhile", { iEdwhile.begin(), iEdwhile.end() } } },
	{ 0x13, { "Switch", { iSwitch.begin(), iSwitch.end() } } },
	{ 0x14, { "Case", { iCase.begin(), iCase.end() } } },
	{ 0x15, { "Default", { iDefault.begin(), iDefault.end() } } },
	{ 0x16, { "Eswitch", { iEswitch.begin(), iEswitch.end() } } },
	{ 0x17, { "Goto", { iGoto.begin(), iGoto.end() } } },
	{ 0x18, { "Gosub", { iGosub.begin(), iGosub.end() } } },
	{ 0x19, { "Return", { iReturn.begin(), iReturn.end() } } },
	{ 0x1A, { "Break", { iBreak.begin(), iBreak.end() } } },
	{ 0x1B, { "For2", { iFor2.begin(), iFor2.end() } } },
	{ 0x1C, { "Break_point", { iBreak_point.begin(), iBreak_point.end() } } },
	{ 0x1D, { "Work_copy", { iWork_copy.begin(), iWork_copy.end() } } },
	{ 0x1E, { "Nop", { iNop.begin(), iNop.end() } } },
	{ 0x1F, { "Nop", { iNop.begin(), iNop.end() } } },
	{ 0x20, { "Nop", { iNop.begin(), iNop.end() } } },
	{ 0x21, { "Ck", { iCk.begin(), iCk.end() } } },
	{ 0x22, { "Set", { iSet.begin(), iSet.end() } } },
	{ 0x23, { "Cmp", { iCmp.begin(), iCmp.end() } } },
	{ 0x24, { "Save", { iSave.begin(), iSave.end() } } },
	{ 0x25, { "Copy", { iCopy.begin(), iCopy.end() } } },
	{ 0x26, { "Calc", { iCalc.begin(), iCalc.end() } } },
	{ 0x27, { "Calc2", { iCalc2.begin(), iCalc2.end() } } },
	{ 0x28, { "Sce_rnd", { iSce_rnd.begin(), iSce_rnd.end() } } },
	{ 0x29, { "Cut_chg", { iCut_chg.begin(), iCut_chg.end() } } },
	{ 0x2A, { "Cut_old", { iCut_old.begin(), iCut_old.end() } } },
	{ 0x2B, { "Message_on", { iMessage_on.begin(), iMessage_on.end() } } },
	{ 0x2C, { "Aot_set", { iAot_set.begin(), iAot_set.end() } } },
	{ 0x2D, { "Obj_model_set", { iObj_model_set.begin(), iObj_model_set.end() } } },
	{ 0x2E, { "Work_set", { iWork_set.begin(), iWork_set.end() } } },
	{ 0x2F, { "Speed_set", { iSpeed_set.begin(), iSpeed_set.end() } } },
	{ 0x30, { "Add_speed", { iAdd_speed.begin(), iAdd_speed.end() } } },
	{ 0x31, { "Add_aspeed", { iAdd_aspeed.begin(), iAdd_aspeed.end() } } },
	{ 0x32, { "Pos_set", { iPos_set.begin(), iPos_set.end() } } },
	{ 0x33, { "Dir_set", { iDir_set.begin(), iDir_set.end() } } },
	{ 0x34, { "Member_set", { iMember_set.begin(), iMember_set.end() } } },
	{ 0x35, { "Member_set2", { iMember_set2.begin(), iMember_set2.end() } } },
	{ 0x36, { "Se_on", { iSe_on.begin(), iSe_on.end() } } },
	{ 0x37, { "Sca_id_set", { iSca_id_set.begin(), iSca_id_set.end() } } },
	{ 0x38, { "Flr_set", { iFlr_set.begin(), iFlr_set.end() } } },
	{ 0x39, { "Dir_ck", { iDir_ck.begin(), iDir_ck.end() } } },
	{ 0x3A, { "Sce_espr_on", { iSce_espr_on.begin(), iSce_espr_on.end() } } },
	{ 0x3B, { "Door_aot_set", { iDoor_aot_set.begin(), iDoor_aot_set.end() } } },
	{ 0x3C, { "Cut_auto", { iCut_auto.begin(), iCut_auto.end() } } },
	{ 0x3D, { "Member_copy", { iMember_copy.begin(), iMember_copy.end() } } },
	{ 0x3E, { "Member_cmp", { iMember_cmp.begin(), iMember_cmp.end() } } },
	{ 0x3F, { "Plc_motion", { iPlc_motion.begin(), iPlc_motion.end() } } },
	{ 0x40, { "Plc_dest", { iPlc_dest.begin(), iPlc_dest.end() } } },
	{ 0x41, { "Plc_neck", { iPlc_neck.begin(), iPlc_neck.end() } } },
	{ 0x42, { "Plc_ret", { iPlc_ret.begin(), iPlc_ret.end() } } },
	{ 0x43, { "Plc_flg", { iPlc_flg.begin(), iPlc_flg.end() } } },
	{ 0x44, { "Sce_em_set", { iSce_em_set.begin(), iSce_em_set.end() } } },
	{ 0x45, { "Col_chg_set", { iCol_chg_set.begin(), iCol_chg_set.end() } } },
	{ 0x46, { "Aot_reset", { iAot_reset.begin(), iAot_reset.end() } } },
	{ 0x47, { "Aot_on", { iAot_on.begin(), iAot_on.end() } } },
	{ 0x48, { "Super_set", { iSuper_set.begin(), iSuper_set.end() } } },
	{ 0x49, { "Super_reset", { iSuper_reset.begin(), iSuper_reset.end() } } },
	{ 0x4A, { "Plc_gun", { iPlc_gun.begin(), iPlc_gun.end() } } },
	{ 0x4B, { "Cut_replace", { iCut_replace.begin(), iCut_replace.end() } } },
	{ 0x4C, { "Sce_espr_kill", { iSce_espr_kill.begin(), iSce_espr_kill.end() } } },
	{ 0x4D, { "Door_model_set", { iDoor_model_set.begin(), iDoor_model_set.end() } } },
	{ 0x4E, { "Item_aot_set", { iItem_aot_set.begin(), iItem_aot_set.end() } } },
	{ 0x4F, { "Sce_key_ck", { iSce_key_ck.begin(), iSce_key_ck.end() } } },
	{ 0x50, { "Sce_trg_ck", { iSce_trg_ck.begin(), iSce_trg_ck.end() } } },
	{ 0x51, { "Sce_bgm_control", { iSce_bgm_control.begin(), iSce_bgm_control.end() } } },
	{ 0x52, { "Sce_espr_control", { iSce_espr_control.begin(), iSce_espr_control.end() } } },
	{ 0x53, { "Sce_fade_set", { iSce_fade_set.begin(), iSce_fade_set.end() } } },
	{ 0x54, { "Sce_espr3d_on", { iSce_espr3d_on.begin(), iSce_espr3d_on.end() } } },
	{ 0x55, { "Member_calc", { iMember_calc.begin(), iMember_calc.end() } } },
	{ 0x56, { "Member_calc2", { iMember_calc2.begin(), iMember_calc2.end() } } },
	{ 0x57, { "Sce_bgmtbl_set", { iSce_bgmtbl_set.begin(), iSce_bgmtbl_set.end() } } },
	{ 0x58, { "Plc_rot", { iPlc_rot.begin(), iPlc_rot.end() } } },
	{ 0x59, { "Xa_on", { iXa_on.begin(), iXa_on.end() } } },
	{ 0x5A, { "Weapon_chg", { iWeapon_chg.begin(), iWeapon_chg.end() } } },
	{ 0x5B, { "Plc_cnt", { iPlc_cnt.begin(), iPlc_cnt.end() } } },
	{ 0x5C, { "Sce_shake_on", { iSce_shake_on.begin(), iSce_shake_on.end() } } },
	{ 0x5D, { "Mizu_div_set", { iMizu_div_set.begin(), iMizu_div_set.end() } } },
	{ 0x5E, { "Keep_Item_ck", { iKeep_Item_ck.begin(), iKeep_Item_ck.end() } } },
	{ 0x5F, { "Xa_vol", { iXa_vol.begin(), iXa_vol.end() } } },
	{ 0x60, { "Kage_set", { iKage_set.begin(), iKage_set.end() } } },
	{ 0x61, { "Cut_be_set", { iCut_be_set.begin(), iCut_be_set.end() } } },
	{ 0x62, { "Sce_Item_lost", { iSce_Item_lost.begin(), iSce_Item_lost.end() } } },
	{ 0x63, { "Plc_gun_eff", { iPlc_gun_eff.begin(), iPlc_gun_eff.end() } } },
	{ 0x64, { "Sce_espr_on2", { iSce_espr_on2.begin(), iSce_espr_on2.end() } } },
	{ 0x65, { "Sce_espr_kill2", { iSce_espr_kill2.begin(), iSce_espr_kill2.end() } } },
	{ 0x66, { "Plc_stop", { iPlc_stop.begin(), iPlc_stop.end() } } },
	{ 0x67, { "Aot_set_4p", { iAot_set_4p.begin(), iAot_set_4p.end() } } },
	{ 0x68, { "Door_aot_set_4p", { iDoor_aot_set_4p.begin(), iDoor_aot_set_4p.end() } } },
	{ 0x69, { "Item_aot_set_4p", { iItem_aot_set_4p.begin(), iItem_aot_set_4p.end() } } },
	{ 0x6A, { "Light_pos_set", { iLight_pos_set.begin(), iLight_pos_set.end() } } },
	{ 0x6B, { "Light_kido_set", { iLight_kido_set.begin(), iLight_kido_set.end() } } },
	{ 0x6C, { "Rbj_reset", { iRbj_reset.begin(), iRbj_reset.end() } } },
	{ 0x6D, { "Sce_scr_move", { iSce_scr_move.begin(), iSce_scr_move.end() } } },
	{ 0x6E, { "Parts_set", { iParts_set.begin(), iParts_set.end() } } },
	{ 0x6F, { "Movie_on", { iMovie_on.begin(), iMovie_on.end() } } },
	{ 0x70, { "Splc_ret", { iSplc_ret.begin(), iSplc_ret.end() } } },
	{ 0x71, { "Splc_sce", { iSplc_sce.begin(), iSplc_sce.end() } } },
	{ 0x72, { "Super_on", { iSuper_on.begin(), iSuper_on.end() } } },
	{ 0x73, { "Mirror_set", { iMirror_set.begin(), iMirror_set.end() } } },
	{ 0x74, { "Sce_fade_adjust", { iSce_fade_adjust.begin(), iSce_fade_adjust.end() } } },
	{ 0x75, { "Sce_espr3d_on2", { iSce_espr3d_on2.begin(), iSce_espr3d_on2.end() } } },
	{ 0x76, { "Sce_Item_get", { iSce_Item_get.begin(), iSce_Item_get.end() } } },
	{ 0x77, { "Sce_line_start", { iSce_line_start.begin(), iSce_line_start.end() } } },
	{ 0x78, { "Sce_line_main", { iSce_line_main.begin(), iSce_line_main.end() } } },
	{ 0x79, { "Sce_line_end", { iSce_line_end.begin(), iSce_line_end.end() } } },
	{ 0x7A, { "Sce_parts_bomb", { iSce_parts_bomb.begin(), iSce_parts_bomb.end() } } },
	{ 0x7B, { "Sce_parts_down", { iSce_parts_down.begin(), iSce_parts_down.end() } } },
	{ 0x7C, { "Light_color_set", { iLight_color_set.begin(), iLight_color_set.end() } } },
	{ 0x7D, { "Light_pos_set2", { iLight_pos_set2.begin(), iLight_pos_set2.end() } } },
	{ 0x7E, { "Light_kido_set2", { iLight_kido_set2.begin(), iLight_kido_set2.end() } } },
	{ 0x7F, { "Light_color_set2", { iLight_color_set2.begin(), iLight_color_set2.end() } } },
	{ 0x80, { "Se_vol", { iSe_vol.begin(), iSe_vol.end() } } },
	{ 0x81, { "Sce_Item_cmp", { iSce_Item_cmp.begin(), iSce_Item_cmp.end() } } },
	{ 0x82, { "Sce_espr_kill_all", { iSce_espr_kill_all.begin(), iSce_espr_kill_all.end() } } },
	{ 0x83, { "Plc_heal", { iPlc_heal.begin(), iPlc_heal.end() } } },
	{ 0x84, { "St_map_hint", { iSt_map_hint.begin(), iSt_map_hint.end() } } },
	{ 0x85, { "Sce_em_pos_ck", { iSce_em_pos_ck.begin(), iSce_em_pos_ck.end() } } },
	{ 0x86, { "Poison_ck", { iPoison_ck.begin(), iPoison_ck.end() } } },
	{ 0x87, { "Poison_clr", { iPoison_clr.begin(), iPoison_clr.end() } } },
	{ 0x88, { "Sce_Item_lost2", { iSce_Item_lost2.begin(), iSce_Item_lost2.end() } } },
	{ 0x89, { "Plc_life_max", { iPlc_life_max.begin(), iPlc_life_max.end() } } },
	{ 0x8A, { "Vib_set0", { iVib_set0.begin(), iVib_set0.end() } } },
	{ 0x8B, { "Vib_set1", { iVib_set1.begin(), iVib_set1.end() } } },
	{ 0x8C, { "Vib_fade_set", { iVib_fade_set.begin(), iVib_fade_set.end() } } },
	{ 0x8D, { "Item_aot_set2", { iItem_aot_set2.begin(), iItem_aot_set2.end() } } },
	{ 0x8E, { "Sce_em_set2", { iSce_em_set2.begin(), iSce_em_set2.end() } } }
};