/*
*
*	Megan Grass
*	April 20, 2024
*
*/


#pragma once


#include <cstdint>


#pragma pack(push, 1)


struct Resident_Evil_2_RDT_Nov96
{
	std::uint8_t nSprite;		// XX // 0x00
	std::uint8_t nCut;			// XX // 0x01
	std::uint8_t nOmodel;		// XX // 0x02
	std::uint8_t nItem;			// XX // 0x03
	std::uint8_t nDoor;			// XX // 0x04
	std::uint8_t nRoom_at;		// XX // 0x05
	std::uint8_t Reverb_lv;		// XX // 0x06
	std::uint8_t nSprite_max;	// XX // 0x07
	std::uint32_t pEdt0;		//  0 // 0x08
	std::uint32_t pVh0;			//  1 // 0x0C
	std::uint32_t pVb0;			//  2 // 0x10
	std::uint32_t pEdt1;		//  3 // 0x14
	std::uint32_t pVh1;			//  4 // 0x18
	std::uint32_t pVb1;			//  5 // 0x1C
	std::uint32_t pSca;			//  6 // 0x20
	std::uint32_t pRcut;		//  7 // 0x24
	std::uint32_t pVcut;		//  8 // 0x28
	std::uint32_t pLight;		//  9 // 0x2C
	std::uint32_t pOmodel;		// 10 // 0x30
	std::uint32_t pFloor;		// 11 // 0x34
	std::uint32_t pBlock;		// 12 // 0x38
	std::uint32_t pMessage;		// 13 // 0x3C
	std::uint32_t pScdx;		// 14 // 0x40
	std::uint32_t pScd0;		// 15 // 0x44
	std::uint32_t pScd1;		// 16 // 0x48
	std::uint32_t pEsp_hed;		// 17 // 0x4C
	std::uint32_t pEsp_end;		// 18 // 0x50
	std::uint32_t pEsp_tim;		// 19 // 0x54
	std::uint32_t pEsp_tim_end;	// 20 // 0x58
	std::uint32_t pRbj;			// 21 // 0x5C
};


#pragma pack(pop)