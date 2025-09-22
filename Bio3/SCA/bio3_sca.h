/*
*
*	Megan Grass
*	September 16, 2025
*
*/


#pragma once

#include <cstdint>


#pragma pack(push, 1)


struct Resident_Evil_3_SCA_Header
{
	std::uint32_t nData;					// Polygon Count (subtract 1 for absolute amount)
	std::int16_t Cx;						// Polygon Group Centroid - Used to generate "Area" bitfield of "Type" variable
	std::int16_t Cz;						// Polygon Group Centroid - Used to generate "Area" bitfield of "Type" variable
	std::int16_t Cx_Copy;					// Polygon Group Centroid - Used to generate "Area" bitfield of "Type" variable
	std::int16_t Cz_Copy;					// Polygon Group Centroid - Used to generate "Area" bitfield of "Type" variable
	std::uint32_t Unk;						// unknown
};


struct Resident_Evil_3_SCA_Data
{
	std::uint16_t Xz[2][2];
	union Id
	{
		std::uint16_t Data;					// Contiguous
		struct Bits							// Bitfield
		{
			std::uint16_t Shape : 4;		// 0 = Circle
											// 1 = Box
											// 2 = Koban X
											// 3 = Koban Z
											// 4 = Naname D
											// 5 = 
											// 6 = Naname C
											// 7 = 
											// 8 = 
											// 9 = Slope
											// 10 = Box 3
											// 11 = 
											// 12 = 
											// 13 = 
											// 14 = 
											// 15 = 
			std::uint16_t bit0 : 1;			// 0 = 
											// 1 = 
			std::uint16_t bit1 : 1;			// 0 = 
											// 1 = 
			std::uint16_t bit2 : 1;			// 0 = 
											// 1 = 
			std::uint16_t bit3 : 1;			// 0 = 
											// 1 = 
			std::uint16_t bit4 : 1;			// 0 = 
											// 1 = 
			std::uint16_t bit5 : 1;			// 0 = 
											// 1 = 
			std::uint16_t bit6 : 1;			// 0 = 
											// 1 = 
			std::uint16_t bit7 : 1;			// 0 = 
											// 1 = 
			std::uint16_t bit8 : 1;			// 0 = 
											// 1 = 
			std::uint16_t bit9 : 1;			// 0 = 
											// 1 = 
			std::uint16_t bit10 : 1;		// 0 = 
											// 1 = 
			std::uint16_t bit11 : 1;		// 0 = 
											// 1 = 
		} Bits;
	} Id;
	union Type
	{
		std::uint16_t Data;					// Contiguous
		struct Bits							// Bitfield
		{
			std::uint16_t Quadrant : 4;		// Axis Quadrant(s)
											// 
											// Determines where the shape type is located within the axis quadrants
											// Individual shape types can have multiple axis quadrants
											//
											// Axis quadrant values, where "x" represents the axis centroid (x=0,y=0,z=0):
											//
											//		2 | 1
											//		--x--
											//		8 | 4
											//
											// Formula to calculate this value:
											//		ULONG Sca_get_area(LONG X, LONG Z, LONG Sx, LONG Sz)
											//		{
											//			return (1 << ((UINT)(X - Sx) >> 0x1f)) << ((UINT)(Z - Sz) >> 0x1e & 2);
											//		}
											//		ULONG Area0 = Sca_get_area((X - 1800), (Z - 1800), Cx, Cz);
											//		ULONG Area1 = Sca_get_area((X + (W + 1800)), (Z - 1800), Cx, Cz);
											//		ULONG Area2 = Sca_get_area((X - 1800), (Z + (D + 1800)), Cx, Cz);
											//		ULONG Area4 = Sca_get_area((X + (W + 1800)), (Z + (D + 1800)), Cx, Cz);
											//		Area = (Area0 | Area1 | Area2 | Area3);

			std::uint16_t Hypotenuse : 1;	// Stair & Slope Hypotenuse
											// 
											// Determines where the Adjacent/Hypotenuse and Opposite/Hypotenuse are located for the "Stair" and "Slope" shape types
											// Ignored when shape type is neither "Stair" or "Slope"
											// 
											// if "Axis" is 0 and "Hypotenuse" is 0:
											//	Adjacent = WEST, Opposite = EAST
											// 
											// if "Axis" is 0 and "Hypotenuse" is 1:
											//	Adjacent = EAST, Opposite = WEST
											// 
											// if "Axis" is 1 and "Hypotenuse" is 0:
											//	Adjacent = SOUTH, Opposite = NORTH
											// 
											// if "Axis" is 1 and "Hypotenuse" is 1:
											//	Adjacent = NORTH, Opposite = SOUTH

			std::uint16_t Axis : 1;			// Stair & Slope Axis
											// Determines if "Stair" and "Slope" shape types are accessible from either the X or Z axis
											// Ignored when shape type is neither "Stair" or "Slope"
											// 0 = X-Axis
											// 1 = Z-Axis

			std::uint16_t nFloor : 5;		// Height in units of -1800
											// 
											// Used by the Sca_get_high function to obtain height of collision
											// Ignored when shape type is "Slope" (height is always -1800 for Slope, hardcoded in executable)
											// 
											// Accepted values are 0-31 (Range: 0, -55800)
											// 
											// This value is irregular in ROOM104:
											//		 Wrecked Vehicles (Value: 33)

			std::uint16_t Height : 5;		// Height in units of -100
											// 
											// This value is typically always 18 to ensure a minimum height of -1800
											// 
											// Used by the Sca_get_high function to obtain height of collision
											// Ignored when shape type is "Slope" (height is always -1800 for Slope, hardcoded in executable)
											// 
											// Accepted values are 0-31 (Range: 0, -3100)
											// 
											// This value is irregular in ROOM104:
											//		 Wrecked Vehicles (Value: 2)
											//		 Wrecked Motorcycle (Value: 1)
		} Bits;
	} Type;
	std::uint16_t Floor;					// Ground in units of -1800
											// 
											// Used by the Sca_get_low function to obtain ground (y-axis) of collision
											// 
											// Accepted bits are 0-31 (Range: 0, -55800)
											//
											// Formula:
											//		Floor = 0;	// Reset to null before setting
											// 
											//		if (!Bit)	// Ground height is zero (0)
											//		{
											//			LONG High = ((Height * -100) + (nFloor * -1800));
											//			High /= -1800;
											//			if (!High) { Floor = 1; }	// Floor variable cannot be zero (0)
											//			else for (ULONG nBit = NULL; nBit < High; nBit++) { Floor |= 1 << nBit; }
											//		}
											//
											//		else Floor |= 1 << Bit;		// Collision is not at ground level (Ceiling, lifted platform, etc)
	std::uint16_t Unk1;						// Compared with Unk of Header
};


#pragma pack(pop)