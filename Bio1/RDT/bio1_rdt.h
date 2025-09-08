/*
*
*	Megan Grass
*	September 4, 2025
*
*/


#pragma once

#include <cstdint>

#include <rid/bio1_rid.h>

#include <rvd/bio1_rvd.h>

#include <lit/bio1_lit.h>

#include <sca/bio1_sca.h>

#include <blk/bio1_blk.h>

#include <flr/bio1_flr.h>


#pragma pack(push, 1)


struct Resident_Evil_RDT_Header_Aug95
{
	std::uint8_t nSprite;				// XX // 0x00 // 
	std::uint8_t nCut;					// XX // 0x01 // 
	std::uint8_t nOmodel;				// XX // 0x02 // 
	std::uint8_t nItem;					// XX // 0x03 // 
	std::uint8_t nDoor;					// XX // 0x04 // 
	std::uint8_t nRoom_at;				// XX // 0x05 // 
	Resident_Evil_LIT_Data_Aug95 Light;	//  0 // 0x06 // // different from retail
	std::uint32_t pVcut;				//  1 // 0x78 // 
	std::uint32_t pSca;					//  2 // 0x7C // 
	std::uint32_t pOmodel;				//  3 // 0x80 // 
	std::uint32_t pImodel;				//  4 // 0x84 // 
	std::uint32_t pBlock;				//  5 // 0x88 // 
	std::uint32_t pFloor;				//  6 // 0x8C // // different from retail
	std::uint32_t pScdX0;				//  7 // 0x90 // CHAR10 // System function (called only once when setting a room)
	std::uint32_t pScdX1;				//  8 // 0x94 // CHAR11 // System function (called only once when setting a room)
	std::uint32_t pScd0;				//  9 // 0x98 // CHAR10 // System function (called every loop)
	std::uint32_t pScd1;				// 10 // 0x9C // CHAR11 // System function (called every loop)
	std::uint32_t pMessage;				// 11 // 0xA0 // 
	std::uint32_t pEsp_hed;				// 12 // 0xA4 // 
	std::uint32_t pEsp_end;				// 13 // 0xA8 // 
	std::uint32_t pEsp_tim_end;			// 14 // 0xAC // 
	std::uint32_t pEdt;					// 15 // 0xB0 // // different from retail (0xC8 bytes)
	std::uint32_t pVh;					// 16 // 0xB4 // 
	std::uint32_t pVb;					// 17 // 0xB8 // 
};


struct Resident_Evil_RDT_Header_Oct95
{
	std::uint8_t nSprite;				// XX // 0x00 // 
	std::uint8_t nCut;					// XX // 0x01 // 
	std::uint8_t nOmodel;				// XX // 0x02 // 
	std::uint8_t nItem;					// XX // 0x03 // 
	std::uint8_t nDoor;					// XX // 0x04 // 
	std::uint8_t nRoom_at;				// XX // 0x05 // 
	Resident_Evil_LIT_Data Light;		//  0 // 0x06 // 
	std::uint32_t pVcut;				//  1 // 0x48 // 
	std::uint32_t pSca;					//  2 // 0x4C // 
	std::uint32_t pOmodel;				//  3 // 0x50 // 
	std::uint32_t pImodel;				//  4 // 0x54 // 
	std::uint32_t pBlock;				//  5 // 0x58 // 
	std::uint32_t pFloor;				//  6 // 0x5C // // different from retail
	std::uint32_t pScdX0;				//  7 // 0x60 // CHAR10	// System function (called only once when setting a room)
	std::uint32_t pScdX1;				//  8 // 0x64 // CHAR11	// System function (called only once when setting a room)
	std::uint32_t pScd0;				//  9 // 0x68 // CHAR10	// System function (called every loop)
	std::uint32_t pScd1;				// 10 // 0x6C // CHAR11	// System function (called every loop)
	std::uint32_t pScdEv0;				// 11 // 0x70 // CHAR10	// Event function
	std::uint32_t pScdEv1;				// 12 // 0x74 // CHAR11	// Event function
	std::uint32_t pMessage;				// 13 // 0x78 // 
	std::uint32_t pItemPix;				// 14 // 0x7C // // points to pEsp_hed if nItem == 0
	std::uint32_t pEsp_hed;				// 15 // 0x80 // 
	std::uint32_t pEsp_end;				// 16 // 0x84 // 
	std::uint32_t pEsp_tim_end;			// 17 // 0x88 // 
	std::uint32_t pEdt;					// 18 // 0x8C // // different from retail (0x100 bytes)
	std::uint32_t pVh;					// 19 // 0x90 // 
	std::uint32_t pVb;					// 20 // 0x94 // 
};


struct Resident_Evil_RDT_Header
{
	std::uint8_t nSprite;				// XX // 0x00 // 
	std::uint8_t nCut;					// XX // 0x01 // 
	std::uint8_t nOmodel;				// XX // 0x02 // 
	std::uint8_t nItem;					// XX // 0x03 // 
	std::uint8_t nDoor;					// XX // 0x04 // 
	std::uint8_t nRoom_at;				// XX // 0x05 // 
	Resident_Evil_LIT_Data Light;		//  0 // 0x06 // 
	std::uint32_t pVcut;				//  1 // 0x48 // 
	std::uint32_t pSca;					//  2 // 0x4C // 
	std::uint32_t pOmodel;				//  3 // 0x50 // 
	std::uint32_t pImodel;				//  4 // 0x54 // 
	std::uint32_t pBlock;				//  5 // 0x58 // 
	std::uint32_t pFloor;				//  6 // 0x5C // 
	std::uint32_t pScdX;				//  7 // 0x60 // // System function (called only once when setting a room)
	std::uint32_t pScd;					//  8 // 0x64 // // System function (called every loop)
	std::uint32_t pScdEv;				//  9 // 0x68 // // Event function
	std::uint32_t pEmr;					// 10 // 0x6C // 
	std::uint32_t pEdd;					// 11 // 0x70 // 
	std::uint32_t pMessage;				// 12 // 0x74 // 
	std::uint32_t pItemPix;				// 13 // 0x78 // 
	std::uint32_t pEsp_hed;				// 14 // 0x7C // 
	std::uint32_t pEsp_end;				// 15 // 0x80 // 
	std::uint32_t pEsp_tim_end;			// 16 // 0x84 // 
	std::uint32_t pEdt;					// 17 // 0x88 // 
	std::uint32_t pVh;					// 18 // 0x8C // 
	std::uint32_t pVb;					// 19 // 0x90 // 
};


#pragma pack(pop)