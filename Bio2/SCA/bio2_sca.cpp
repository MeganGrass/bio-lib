/*
*
*	Megan Grass
*	April 20, 2024
*
*/


#include "bio2_sca.h"


std::uintmax_t Resident_Evil_2_SCA::Open(StdFile& File, std::uintmax_t _Ptr)
{
	Standard_String Str;

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read, true, false))
		{
			Str.Message("Resident Evil 2: Error, could not open SCA at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
			return _Ptr;
		}
	}

	File.Read(_Ptr , &Header, sizeof(Resident_Evil_2_SCA_Header));

	Data.resize(static_cast<std::vector<Resident_Evil_2_SCA_Data, std::allocator<Resident_Evil_2_SCA_Data>>::size_type>(Header.nData) - 1);

	File.Read(_Ptr + sizeof(Resident_Evil_2_SCA_Header), Data.data(), Data.size() * sizeof(Resident_Evil_2_SCA_Data));

	return _Ptr + sizeof(Resident_Evil_2_SCA_Header) + Data.size();
}

bool Resident_Evil_2_SCA::Open(std::filesystem::path Path, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Path);

	std::uintmax_t OldPtr = _Ptr;

	_Ptr = Open(m_File, _Ptr);

	return OldPtr != _Ptr;
}

std::uintmax_t Resident_Evil_2_SCA::Save(StdFile& File, std::uintmax_t _Ptr)
{
	Standard_String Str;

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Write, true, false))
		{
			Str.Message("Resident Evil 2: Error, could not create SCA at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
			return _Ptr;
		}
	}

	File.Write(_Ptr, &Header, sizeof(Resident_Evil_2_SCA_Header));

	File.Write(_Ptr + sizeof(Resident_Evil_2_SCA_Header), Data.data(), Data.size() * sizeof(Resident_Evil_2_SCA_Data));

	return _Ptr + sizeof(Resident_Evil_2_SCA_Header) + (Data.size() * sizeof(Resident_Evil_2_SCA_Data));
}

bool Resident_Evil_2_SCA::Save(std::filesystem::path Path, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Path);

	std::uintmax_t OldPtr = _Ptr;

	_Ptr = Save(m_File, _Ptr);

	return OldPtr != _Ptr;
}

void Resident_Evil_2_SCA::Close(void)
{
	std::memset(&Header, 0, sizeof(Resident_Evil_2_SCA_Header));
	Data.clear();
}

void Resident_Evil_2_SCA::CalcCxCz(void)
{
	std::int32_t MinX = 0x7FFFFFFF, MaxX = 0x80000000;
	std::int32_t MinZ = 0x7FFFFFFF, MaxZ = 0x80000000;

	for (auto& i : Data)
	{
		if (i.X < MinX) { MinX = i.X; }
		if (i.X + i.W > MaxX) { MaxX = i.X + i.W; }
		if (i.Z < MinZ) { MinZ = i.Z; }
		if (i.Z + i.D > MaxZ) { MaxZ = i.Z + i.D; }
	}

	Header.Cx = ((MaxX + MinX) / 2);
	Header.Cz = ((MaxZ + MinZ) / 2);

	for (size_t i = 0; i < Data.size(); i++)
	{
		SetArea(i);
	}
}

std::uint32_t Resident_Evil_2_SCA::GetArea(std::int32_t X, std::int32_t Z, std::int32_t Sx, std::int32_t Sz)
{
	return (1 << ((std::uint32_t)(X - Sx) >> 0x1E)) << ((std::uint32_t)(Z - Sz) >> 0x1E & 2);
}

void Resident_Evil_2_SCA::SetArea(std::size_t iCollision)
{
	VECTOR p0{};  // Bottom-Left
	VECTOR p1{};  // Bottom-Right
	VECTOR p2{};  // Top-Left
	VECTOR p3{};  // Top-Right

	// Bottom-Left
	p0.vx = (Data[iCollision].X - 1800);
	p0.vz = (Data[iCollision].Z - 1800);

	// Bottom-Right
	p1.vx = Data[iCollision].X + (Data[iCollision].W + 1800);
	p1.vz = (Data[iCollision].Z - 1800);

	// Top-Left
	p2.vx = (Data[iCollision].X - 1800);
	p2.vz = Data[iCollision].Z + (Data[iCollision].D + 1800);

	// Top-Right
	p3.vx = Data[iCollision].X + (Data[iCollision].W + 1800);
	p3.vz = Data[iCollision].Z + (Data[iCollision].D + 1800);

	// Area
	std::uint32_t Area0 = GetArea(p0.vx, p0.vz, Header.Cx, Header.Cz);
	std::uint32_t Area1 = GetArea(p1.vx, p1.vz, Header.Cx, Header.Cz);
	std::uint32_t Area2 = GetArea(p2.vx, p2.vz, Header.Cx, Header.Cz);
	std::uint32_t Area3 = GetArea(p3.vx, p3.vz, Header.Cx, Header.Cz);

	// Complete
	Data[iCollision].Type.Bits.Quadrant = (Area0 | Area1 | Area2 | Area3);
}

Resident_Evil_2_Slope_Hypotenuse Resident_Evil_2_SCA::GetSlopeHypotenuse(std::size_t iCollision)
{
	if (Data[iCollision].Type.Bits.Hypotenuse)
	{
		if (Data[iCollision].Type.Bits.Axis) { return Resident_Evil_2_Slope_Hypotenuse::Type_D; }
		else { return Resident_Evil_2_Slope_Hypotenuse::Type_B; }
	}
	else
	{
		if (Data[iCollision].Type.Bits.Axis) { return Resident_Evil_2_Slope_Hypotenuse::Type_C; }
		else { return Resident_Evil_2_Slope_Hypotenuse::Type_A; }
	}
}

std::int32_t Resident_Evil_2_SCA::GetLow(std::size_t iCollision)
{
	std::uint32_t Floor = Data[iCollision].Floor;

	if (!Floor) { return 0; }

	std::int32_t nFloor = 0;

	while ((Floor & 1) == 0)
	{
		Floor = Floor >> 1;
		nFloor = nFloor + -1800;
	}

	return nFloor;
}

std::int32_t Resident_Evil_2_SCA::GetHigh(std::size_t iCollision)
{
	return (Data[iCollision].Type.Bits.Height * -100) + (Data[iCollision].Type.Bits.nFloor * -1800);
}

std::uint32_t Resident_Evil_2_SCA::GetFloor(std::size_t iCollision)
{
	std::uint32_t Floor = Data[iCollision].Floor;

	// Ground
	if (Floor & 1) { return 0; }

	// Ceiling
	for (std::uint32_t Bit = 0; Bit < 31; Bit++)
	{
		if (Floor & (1 << Bit)) { return Bit; }
	}

	// Error
	return 1;
}

std::uint32_t Resident_Evil_2_SCA::SetFloor(std::size_t iCollision, std::int32_t Bit)
{
	std::uint32_t Floor = 0;

	std::uint16_t Type = Data[iCollision].Type.Data;

	if (!Bit)
	{
		std::int32_t High = (Type >> 11) * -100 + (Type >> 6 & 0x1F) * -1800;

		High /= -1800;

		if (!High) { return 1; }

		std::uintmax_t Mask = 1;

		for (std::int32_t nBit = 0; nBit < High; nBit++)
		{
			Floor |= Mask << nBit;
		}

		return Floor;
	}

	std::uintmax_t Mask = 1;

	Floor |= Mask << Bit;

	return Floor;
}