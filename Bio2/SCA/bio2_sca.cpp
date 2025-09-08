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
	Data.shrink_to_fit();
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

void Resident_Evil_2_SCA::SetArea(Resident_Evil_2_SCA_Data& Input)
{
	VECTOR p0{};  // Bottom-Left
	VECTOR p1{};  // Bottom-Right
	VECTOR p2{};  // Top-Left
	VECTOR p3{};  // Top-Right

	// Bottom-Left
	p0.vx = (Input.X - 1800);
	p0.vz = (Input.Z - 1800);

	// Bottom-Right
	p1.vx = Input.X + (Input.W + 1800);
	p1.vz = (Input.Z - 1800);

	// Top-Left
	p2.vx = (Input.X - 1800);
	p2.vz = Input.Z + (Input.D + 1800);

	// Top-Right
	p3.vx = Input.X + (Input.W + 1800);
	p3.vz = Input.Z + (Input.D + 1800);

	// Area
	std::uint32_t Area0 = GetArea(p0.vx, p0.vz, Header.Cx, Header.Cz);
	std::uint32_t Area1 = GetArea(p1.vx, p1.vz, Header.Cx, Header.Cz);
	std::uint32_t Area2 = GetArea(p2.vx, p2.vz, Header.Cx, Header.Cz);
	std::uint32_t Area3 = GetArea(p3.vx, p3.vz, Header.Cx, Header.Cz);

	// Complete
	Input.Type.Bits.Quadrant = (Area0 | Area1 | Area2 | Area3);
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

const std::int32_t Resident_Evil_2_SCA::GetLow(std::size_t iCollision)
{
	std::uint32_t Floor = Data[iCollision].Floor;

	if (!Floor) { return 0; }

	std::int32_t nFloor = 0;

	while ((Floor & 1) == 0)
	{
		Floor = (Floor >> 1);
		nFloor = (nFloor + -1800);
	}

	return nFloor;
}

const std::int32_t Resident_Evil_2_SCA::GetLow(Resident_Evil_2_SCA_Data Input)
{
	std::uint32_t Floor = Input.Floor;

	if (!Floor) { return 0; }

	std::int32_t nFloor = 0;

	while ((Floor & 1) == 0)
	{
		Floor = (Floor >> 1);
		nFloor = (nFloor + -1800);
	}

	return nFloor;
}

const std::int32_t Resident_Evil_2_SCA::GetHigh(std::size_t iCollision)
{
	return (Data[iCollision].Type.Bits.Height * -100) + (Data[iCollision].Type.Bits.nFloor * -1800);
}

const std::int32_t Resident_Evil_2_SCA::GetHigh(Resident_Evil_2_SCA_Data Input)
{
	return (Input.Type.Bits.Height * -100) + (Input.Type.Bits.nFloor * -1800);
}

const std::uint32_t Resident_Evil_2_SCA::SetFloor(std::size_t iCollision, std::int32_t Bit)
{
	std::uint32_t Floor = 0;

	if (!Bit)
	{
		std::int32_t High = Get(iCollision)->Type.Bits.Height * -100 + Get(iCollision)->Type.Bits.nFloor * -1800;

		High /= -1800;

		if (!High) { return Get(iCollision)->Floor = 1; }

		for (std::int32_t nBit = 0; nBit < High; nBit++)
		{
			Floor |= (1 << nBit);
		}

		return Get(iCollision)->Floor = Floor;
	}

	Floor |= (1 << Bit);

	return Get(iCollision)->Floor = Floor;
}

const std::uint32_t Resident_Evil_2_SCA::SetFloor(Resident_Evil_2_SCA_Data& Input, std::int32_t Bit)
{
	std::uint32_t Floor = NULL;

	if (!Bit)
	{
		std::int32_t High = Input.Type.Bits.Height * -100 + Input.Type.Bits.nFloor * -1800;

		High /= -1800;

		if (!High) { return Input.Floor = 1; }

		for (std::int32_t nBit = 0; nBit < High; nBit++)
		{
			Floor |= (1 << nBit);
		}

		return Input.Floor = Floor;
	}

	Floor |= (1 << Bit);

	return Input.Floor = Floor;
}

SHAPEVECTOR Resident_Evil_2_SCA::GetShapeVector(std::size_t iCollision)
{
	int32_t Low = GetLow(iCollision);
	int32_t High = GetHigh(iCollision);

	GetShapeType(iCollision) == Resident_Evil_2_Collision_Shape::Box_3 ? High = -std::abs(Data[iCollision].Type.Bits.nFloor * 1800) : 0;
	GetShapeType(iCollision) == Resident_Evil_2_Collision_Shape::Slope ? High = -std::abs(std::abs(Low) + 1800) : 0;

	return SHAPEVECTOR{ Data[iCollision].X, Low, Data[iCollision].Z, Data[iCollision].W, High, Data[iCollision].D };
}

void Resident_Evil_2_SCA::RemoveDuplicates(void)
{
	std::vector<std::vector<std::uint8_t>> Temp(Data.size());
	std::vector<std::uint32_t> Crc(Data.size());

	for (size_t i = 0; i < Data.size(); i++)
	{
		Temp[i].resize(sizeof(Resident_Evil_2_SCA_Data));

		std::memcpy(Temp[i].data(), &Data[i], sizeof(Resident_Evil_2_SCA_Data));

		Crc[i] = Cyclic_Redundancy_Check().GetCRC32(Temp[i].data(), Temp[i].size());
	}

	for (size_t i = 0; i < Crc.size(); i++)
	{
		for (size_t j = i + 1; j < Crc.size(); j++)
		{
			if (Crc[i] == Crc[j])
			{
				if (std::memcmp(Temp[i].data(), Temp[j].data(), sizeof(Resident_Evil_2_SCA_Data)) == 0)
				{
					Data.erase(Data.begin() + j);
					Temp.erase(Temp.begin() + j);
					Crc.erase(Crc.begin() + j);
					j--;
				}
			}
		}
	}

	Header.nData = static_cast<std::uint32_t>(Data.size()) + 1;
}