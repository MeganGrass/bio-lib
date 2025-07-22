/*
*
*	Megan Grass
*	April 20, 2024
*
*/


#pragma once

#include <lib_gte.h>

#include <std_text.h>


enum class Resident_Evil_2_Collision_Shape : std::uint16_t
{
	Box,									// Square/Rectangle
	Naname_a,								// Right-Angled Triangle	// \| SouthEast
	Naname_b,								// Right-Angled Triangle	// |/ SouthWest
	Naname_c,								// Right-Angled Triangle	// /| NorthWest
	Naname_d,								// Right-Angled Triangle	// |\ NorthEast
	Hishi,									// Rhombus
	Circle,									// Cylinder
	Koban_x,								// Oblong Cylinder			// X-Axis
	Koban_z,								// Oblong Cylinder			// Z-Axis
	Box_1,									// Square/Rectangle			// Special Function: Climb Up
	Box_2,									// Square/Rectangle			// Special Function: Jump Down
	Slope,									// Right-Angled Triangle	// Special Function: Walkway Slope
	Box_3,									// Square/Rectangle			// Special Function: Stairs
	Curve									// Curve/Donut				// Sewer pipe, found in 40B and 40F
};


enum class Resident_Evil_2_Slope_Hypotenuse : std::uint16_t
{
	Type_A,									// Adjacent = EAST, Opposite = WEST
	Type_B,									// Adjacent = WEST, Opposite = EAST
	Type_C,									// Adjacent = NORTH, Opposite = SOUTH
	Type_D,									// Adjacent = SOUTH, Opposite = NORTH
};


#pragma pack(push, 1)


struct Resident_Evil_2_SCA_Header
{
	std::int16_t Cx;						// Polygon Group Centroid - Used to generate "Area" bitfield of "Type" variable
	std::int16_t Cz;						// Polygon Group Centroid - Used to generate "Area" bitfield of "Type" variable
	std::uint32_t nData;					// Polygon Count (subtract 1 for absolute amount)
	std::int32_t Ceiling;					// Ceiling Height (multiply by -1800)
	std::uint32_t Color;					// RGB color, unused
};


struct Resident_Evil_2_SCA_Data
{
	std::int16_t X;
	std::int16_t Z;
	std::uint16_t W;
	std::uint16_t D;
	union Id
	{
		std::uint16_t Data;					// Contiguous
		struct Bits							// Bitfield
		{
			std::uint16_t Shape : 4;		// 0 = Box
											// 1 = Naname_a
											// 2 = Naname_b
											// 3 = Naname_c
											// 4 = Naname_d
											// 5 = Hishi
											// 6 = Circle
											// 7 = Koban_x
											// 8 = Koban_z
											// 9 = Box_1
											// 10 = Box_2
											// 11 = Slope
											// 12 = Box_3
											// 13 = Curve
			std::uint16_t bit0 : 4;			// 0 = Weapon Collision OFF (Desk, etc)
											// 8 = Weapon Collision ON (Wall, etc)
			std::uint16_t bit1 : 1;			// 0 = nFloor Height OFF (Cannot Walk Under)
											// 1 = nFloor Height ON (Can Walk Under)
			std::uint16_t bit2 : 1;			// 0 = 
											// 1 = 
			std::uint16_t bit3 : 1;			// 0 = Enemy Collision OFF
											// 1 = Enemy Collision ON
			std::uint16_t bit4 : 1;			// 0 = 
											// 1 = 
			std::uint16_t bit5 : 1;			// 0 = 
											// 1 = 
			std::uint16_t bit6 : 1;			// 0 = Bullet Collision OFF
											// 1 = Bullet Collision ON
			std::uint16_t bit7 : 1;			// 0 = Object Collision OFF
											// 1 = Object Collision ON
			std::uint16_t bit8 : 1;			// 0 = Player Collision OFF
											// 1 = Player Collision ON
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
											//	Adjacent = EAST, Opposite = WEST
											// 
											// if "Axis" is 0 and "Hypotenuse" is 1:
											//	Adjacent = WEST, Opposite = EAST
											// 
											// if "Axis" is 1 and "Hypotenuse" is 0:
											//	Adjacent = NORTH, Opposite = SOUTH
											// 
											// if "Axis" is 1 and "Hypotenuse" is 1:
											//	Adjacent = SOUTH, Opposite = NORTH

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
	std::uint32_t Floor;					// Ground in units of -1800
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
											//			ULONG High = ((Height * -200) + (nFloor * -1800));
											//			High /= -1800;
											//			if (!High) { Floor = 1; }	// Floor variable cannot be zero (0)
											//			else for (ULONG nBit = NULL; nBit < High; nBit++) { Floor |= 1 << nBit; }
											//		}
											//
											//		else Floor |= 1 << Bit;		// Collision is not at ground level (Ceiling, lifted platform, etc)
};


#pragma pack(pop)


class Resident_Evil_2_SCA {
private:

	// Header
	Resident_Evil_2_SCA_Header Header;

	// Data
	std::vector<Resident_Evil_2_SCA_Data> Data;

public:

	/*
		Construction
	*/
	explicit Resident_Evil_2_SCA(void) :
		Header(Resident_Evil_2_SCA_Header())
	{
	}

	/*
		Copy
	*/
	Resident_Evil_2_SCA(const Resident_Evil_2_SCA& v) noexcept : Header(v.Header), Data(v.Data) {}
	Resident_Evil_2_SCA& operator = (const Resident_Evil_2_SCA& v) noexcept { return *this = Resident_Evil_2_SCA(v); }

	/*
		Move
	*/
	Resident_Evil_2_SCA(Resident_Evil_2_SCA&& v) noexcept : Header(std::exchange(v.Header, {})), Data(std::exchange(v.Data, {})) {}
	Resident_Evil_2_SCA& operator = (Resident_Evil_2_SCA&& v) noexcept
	{
		std::swap(Header, v.Header);
		std::swap(Data, v.Data);
		return *this;
	}

	/*
		Get data count
	*/
	std::size_t Count(void) const { return Data.size(); }

	/*
		Get Header element
	*/
	Resident_Evil_2_SCA_Header* GetHeader(void) { return &Header; }

	/*
		Get data element
	*/
	Resident_Evil_2_SCA_Data* Get(const std::size_t& iElement) { return &Data[iElement]; }

	/*
		Push back empty element
	*/
	void New(void) { Data.push_back(Resident_Evil_2_SCA_Data()); }

	/*
		Push back element from buffer
	*/
	void Add(Resident_Evil_2_SCA_Data& Input) { Data.push_back(Input); }

	/*
		Copy element to buffer
	*/
	void Copy(std::size_t iEntry, Resident_Evil_2_SCA_Data& Output) { Output = Data[iEntry]; }

	/*
		Paste element from buffer
	*/
	void Paste(std::size_t iEntry, Resident_Evil_2_SCA_Data& Input) { Data[iEntry] = Input; }

	/*
		Insert element
	*/
	void Insert(std::size_t iEntry, Resident_Evil_2_SCA_Data& Input) { Data.insert(Data.begin() + iEntry, Input); }

	/*
		Delete element
	*/
	void Delete(std::size_t iEntry) { Data.erase(Data.begin() + iEntry); }

	/*
		Open
	*/
	std::uintmax_t Open(StdFile& File, std::uintmax_t _Ptr);

	/*
		Open
	*/
	bool Open(std::filesystem::path Path, std::uintmax_t _Ptr = 0);

	/*
		Save
	*/
	std::uintmax_t Save(StdFile& File, std::uintmax_t _Ptr);

	/*
		Save
	*/
	bool Save(std::filesystem::path Path, std::uintmax_t _Ptr);

	/*
		Close
	*/
	void Close(void);

	/*
		Calculate Axis Center Point
	*/
	void CalcCxCz(void);

	/*
		Get Area
	*/
	std::uint32_t GetArea(std::int32_t X, std::int32_t Z, std::int32_t Sx, std::int32_t Sz);

	/*
		Set Area
	*/
	void SetArea(std::size_t iCollision);

	/*
		Get Slope/Stair Hypotenuse
	*/
	Resident_Evil_2_Slope_Hypotenuse GetSlopeHypotenuse(std::size_t iCollision);

	/*
		Get Ground
	*/
	std::int32_t GetLow(std::size_t iCollision);

	/*
		Get Height
	*/
	std::int32_t GetHigh(std::size_t iCollision);

	/*
		Get Floor
	*/
	std::uint32_t GetFloor(std::size_t iCollision);

	/*
		Set Floor
	*/
	std::uint32_t SetFloor(std::size_t iCollision, std::int32_t Bit);

};