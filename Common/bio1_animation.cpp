/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO: 
*
*/


#include "bio1_animation.h"


/*
	Open
*/
std::uintmax_t Resident_Evil_Animation::OpenEDD(StdFile& File, std::uintmax_t _Ptr)
{
	if (b_EddOpen) { CloseEDD(); }

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read, true, false))
		{
			Str->Message("Resident Evil Animation: Error, could not open EDD at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
			return _Ptr;
		}
	}

	std::size_t nAnimation = 0;
	File.Read(_Ptr + 2, &nAnimation, sizeof(std::uint16_t));
	nAnimation /= 4;

	if (!nAnimation)
	{
		Str->Message("Resident Evil Animation: Error, animation count was null at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
		return _Ptr;
	}

	std::vector<Resident_Evil_EDD_Header> Header(nAnimation);
	File.Read(_Ptr, Header.data(), Header.size() * sizeof(Resident_Evil_EDD_Header));

	Anim.resize(Header.size());

	struct EDD_Attr
	{
		std::uint32_t pEmr : 16;
		std::uint32_t Speed : 16;
	};
	EDD_Attr Attr{};

	for (std::size_t i = 0; i < Anim.size(); i++)
	{
		Anim[i].resize(Header[i].nFrames);

		for (std::size_t x = 0; x < Anim[i].size(); x++)
		{
			File.Read(_Ptr + Header[i].pAttr + (x * sizeof(EDD_Attr)), &Attr, sizeof(EDD_Attr));
			Anim[i][x].Attr.pEmr = Attr.pEmr;
			Anim[i][x].Attr.Speed = Attr.Speed;
			Anim[i][x].Attr.Sound = 0;
		}
	}

	b_EddOpen = true;

	return _Ptr + SizeEDD();
}


/*
	Open
*/
bool Resident_Evil_Animation::OpenEDD(std::filesystem::path Input, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Input);

	OpenEDD(m_File, _Ptr);

	return b_EddOpen;
}


/*
	Open
*/
std::uintmax_t Resident_Evil_Animation::OpenEMR(StdFile& File, std::uintmax_t _Ptr)
{
	if (b_EmrOpen) { CloseEMR(); }

	if (!b_EddOpen)
	{
		Str->Message("Resident Evil Animation: Error, EDD must be open first");
		return _Ptr;
	}

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read, true, false))
		{
			Str->Message("Resident Evil Animation: Error, could not open EMR at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
			return _Ptr;
		}
	}

	Resident_Evil_EMR_Header Header{};
	File.Read(_Ptr, &Header, sizeof(Resident_Evil_EMR_Header));

	struct Joint_Index
	{
		std::uint16_t Count;
		std::uint16_t Pointer;
	};

	std::vector<Joint_Index> JointIndex(Header.nJoint);
	File.Read(_Ptr + Header.pJoint, JointIndex.data(), JointIndex.size() * sizeof(Joint_Index));

	Joint.resize(JointIndex.size());

	for (std::size_t i = 0; i < Joint.size(); i++)
	{
		Joint[i].Ref.resize(JointIndex[i].Count);
		File.Read(_Ptr + Header.pJoint + JointIndex[i].Pointer, Joint[i].Ref.data(), Joint[i].Ref.size());
		File.Read(_Ptr + sizeof(Resident_Evil_EMR_Header) + (i * sizeof(SVECTOR2)), &Joint[i].Pos, sizeof(SVECTOR2));
	}

	std::uintmax_t Pointer = _Ptr + Header.pFrames;
	std::vector<std::uint32_t> pEmrList;

	for (std::size_t i = 0; i < Anim.size(); i++)
	{
		for (std::size_t x = 0; x < Anim[i].size(); x++)
		{
			pEmrList.push_back(Anim[i][x].Attr.pEmr);
			for (std::size_t y = 0; y < pEmrList.size() - 1; y++)
			{
				if (Anim[i][x].Attr.pEmr == pEmrList[y])
				{
					pEmrList.pop_back();
					break;
				}
			}

			std::uintmax_t pFrame = Pointer + static_cast<std::uintmax_t>(Anim[i][x].Attr.pEmr * Header.FrameLen);
			File.Align(pFrame, 4);

			Anim[i][x].Rotation.resize(Joint.size());

			File.Read(pFrame, &Anim[i][x].Origin, sizeof(SVECTOR2));
			File.Read(pFrame + sizeof(SVECTOR2), &Anim[i][x].Speed, sizeof(SVECTOR2));
			File.Read(pFrame + (sizeof(SVECTOR2) * 2), Anim[i][x].Rotation.data(), Anim[i][x].Rotation.size() * sizeof(SVECTOR2));
		}
	}

	b_EmrOpen = true;

	std::uintmax_t FileSize = Pointer + pEmrList.size() * Header.FrameLen;

	if ((std::to_underlying(Game) & (AUG95 | OCT95)) || (std::to_underlying(Type) & (WEAPON)))
	{
		return FileSize;
	}

	return FileSize + 4;
}


/*
	Open
*/
bool Resident_Evil_Animation::OpenEMR(std::filesystem::path Input, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Input);

	OpenEMR(m_File, _Ptr);

	return b_EmrOpen;
}


/*
	Save
*/
std::uintmax_t Resident_Evil_Animation::SaveEDD(StdFile& File, std::uintmax_t _Ptr)
{
	if (!IsOpen())
	{
		Str->Message("Resident Evil Animation: cannot save EDD, animation is not open");
		return _Ptr;
	}

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Write, true, false))
		{
			Str->Message("Resident Evil Animation: Error, could not create EDD at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
			return _Ptr;
		}
	}

	std::uintmax_t pIndex = _Ptr;

	std::uintmax_t pAnim = _Ptr + (Anim.size() * sizeof(Resident_Evil_EDD_Header));

	std::vector<Resident_Evil_EDD_Header> Header(Anim.size());

	struct EDD_Attr
	{
		std::uint32_t pEmr : 16;
		std::uint32_t Speed : 16;
	};
	EDD_Attr Attr{};

	GetUniqueFrameTotal(true);

	for (std::size_t i = 0; i < Anim.size(); i++, pIndex += sizeof(Resident_Evil_EDD_Header))
	{
		Header[i].nFrames = static_cast<std::uint16_t>(Anim[i].size());
		Header[i].pAttr = static_cast<std::uint16_t>(pAnim - _Ptr);
		File.Write(pIndex, &Header[i], sizeof(Resident_Evil_EDD_Header));
		for (std::size_t x = 0; x < Anim[i].size(); x++)
		{
			Attr.pEmr = Anim[i][x].Attr.pEmr;
			Attr.Speed = Anim[i][x].Attr.Speed;
			File.Write(pAnim + (x * sizeof(EDD_Attr)), &Attr, sizeof(EDD_Attr));
		}
		pAnim += Anim[i].size() * sizeof(EDD_Attr);
	}

	if ((std::to_underlying(Game) & (AUG95 | OCT95)) || (std::to_underlying(Type) & (WEAPON)))
	{
		return _Ptr + SizeEDD();
	}

	std::uintmax_t FileSize = (SizeEDD() - 4);
	File.Write(_Ptr + FileSize, &FileSize, sizeof(std::uint32_t));

	return _Ptr + FileSize + 4;
}


/*
	Save
*/
bool Resident_Evil_Animation::SaveEDD(std::filesystem::path Output, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Output);

	std::uintmax_t OldPtr = _Ptr;

	_Ptr = SaveEDD(m_File, _Ptr);

	return OldPtr != _Ptr;
}


/*
	Save
*/
std::uintmax_t Resident_Evil_Animation::SaveEMR(StdFile& File, std::uintmax_t _Ptr)
{
	if (!IsOpen())
	{
		Str->Message("Resident Evil Animation: cannot save EMR, animation is not open");
		return _Ptr;
	}

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Write, true, false))
		{
			Str->Message("Resident Evil Animation: Error, could not create EMR at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
			return _Ptr;
		}
	}

	Resident_Evil_EMR_Header Header{};

	Header.nJoint = static_cast<std::uint16_t>(Joint.size());

	Header.FrameLen = GetFrameLength();

	std::size_t FileSize = SizeEMR();

	struct Joint_Index
	{
		std::uint16_t Count;
		std::uint16_t Pointer;
	};

	std::size_t ObjCount = 0;

	std::vector<Joint_Index> JointIndex(Joint.size());

	Header.pJoint += static_cast<std::uint16_t>(sizeof(Resident_Evil_EMR_Header) + (Joint.size() * sizeof(SVECTOR2)));
	File.Align(Header.pJoint, 4);

	for (std::size_t i = 0; i < Joint.size(); i++)
	{
		JointIndex[i].Count = static_cast<std::uint16_t>(Joint[i].Ref.size());
		JointIndex[i].Pointer = static_cast<std::uint16_t>((Joint.size() * sizeof(Joint_Index)) + ObjCount);
		ObjCount += Joint[i].Ref.size();

		File.Write(_Ptr + Header.pJoint + (i * sizeof(Joint_Index)), &JointIndex[i], sizeof(Joint_Index));
		File.Write(_Ptr + Header.pJoint + JointIndex[i].Pointer, Joint[i].Ref.data(), Joint[i].Ref.size());
		File.Write(_Ptr + sizeof(Resident_Evil_EMR_Header) + (i * sizeof(SVECTOR2)), &Joint[i].Pos, sizeof(SVECTOR2));
	}

	Header.pFrames = Header.pJoint + static_cast<std::uint16_t>((Joint.size() * sizeof(Joint_Index)) + ObjCount);
	File.Align(Header.pFrames, 4);

	for (std::size_t i = 0; i < Anim.size(); i++)
	{
		for (std::size_t x = 0; x < Anim[i].size(); x++)
		{
			std::uintmax_t pFrame = _Ptr + Header.pFrames + static_cast<std::uintmax_t>(Anim[i][x].Attr.pEmr * Header.FrameLen);
			File.Align(pFrame, 4);

			File.Write(pFrame, &Anim[i][x].Origin, sizeof(SVECTOR2));
			File.Write(pFrame + sizeof(SVECTOR2), &Anim[i][x].Speed, sizeof(SVECTOR2));
			File.Write(pFrame + (sizeof(SVECTOR2) * 2), Anim[i][x].Rotation.data(), Anim[i][x].Rotation.size() * sizeof(SVECTOR2));
		}
	}

	File.Write(_Ptr, &Header, sizeof(Resident_Evil_EMR_Header));

	if ((std::to_underlying(Game) & (AUG95 | OCT95)) || (std::to_underlying(Type) & (WEAPON)))
	{
		File.ResizeAlign(4);
		return _Ptr + FileSize;
	}

	std::uint32_t _Size = static_cast<std::uint32_t>(FileSize - sizeof(Resident_Evil_EMR_Header));

	File.Write(_Ptr + FileSize, &_Size, sizeof(std::uint32_t));

	return _Ptr + FileSize + 4;
}


/*
	Save
*/
bool Resident_Evil_Animation::SaveEMR(std::filesystem::path Output, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Output);

	std::uintmax_t OldPtr = _Ptr;

	_Ptr = SaveEMR(m_File, _Ptr);

	return OldPtr != _Ptr;
}


/*
	Push back empty joint
*/
std::size_t Resident_Evil_Animation::NewJoint(void)
{
	Joint.resize(Joint.size() + 1);
	Joint[Joint.size() - 1].Ref.resize(1);
	return Joint.size();
}


/*
	Push back empty animation
*/
std::size_t Resident_Evil_Animation::NewAnimation(void)
{
	Anim.resize(Anim.size() + 1);
	Anim[Anim.size() - 1].resize(1);
	return Anim.size();
}


/*
	Get the total amount of unique frames
*/
std::size_t Resident_Evil_Animation::GetUniqueFrameTotal(bool UpdateEDD)
{
	std::uintmax_t FrameLen = GetFrameLength();

	Cyclic_Redundancy_Check Crc;

	std::vector<std::tuple<std::size_t, std::size_t, std::uint32_t>> Table;

	std::uint32_t pEmr = 0;

	for (std::size_t i = 0; i < Anim.size(); i++)
	{
		for (std::size_t x = 0; x < Anim[i].size(); x++)
		{
			if (UpdateEDD) { Anim[i][x].Attr.pEmr = pEmr; }

			std::vector<std::uint8_t> Buffer(FrameLen + 8);

			std::memcpy(&Buffer.data()[0], &Anim[i][x].Attr.Speed, sizeof(std::uint32_t));
			std::memcpy(&Buffer.data()[4], &Anim[i][x].Attr.Sound, sizeof(std::uint32_t));
			std::memcpy(&Buffer.data()[8], &Anim[i][x].Origin, sizeof(SVECTOR2));
			std::memcpy(&Buffer.data()[8 + sizeof(SVECTOR2)], &Anim[i][x].Speed, sizeof(SVECTOR2));
			std::memcpy(&Buffer.data()[8 + (sizeof(SVECTOR2) * 2)], &Anim[i][x].Rotation.data()[0], (FrameLen - (sizeof(SVECTOR2) * 2)));

			std::uint32_t CRC = Crc.GetCRC32(Buffer.data(), FrameLen);

			Table.push_back(std::make_tuple(i, x, CRC));

			pEmr++;

			for (std::size_t y = 0; y < Table.size() - 1; y++)
			{
				if (std::get<2>(Table[y]) == CRC)
				{
					if (UpdateEDD) { Anim[i][x].Attr.pEmr = Anim[std::get<0>(Table[y])][std::get<1>(Table[y])].Attr.pEmr; }
					pEmr--;
					Table.pop_back();
					break;
				}
			}
		}
	}

	return Table.size();
}


/*
	Push back empty frame
*/
std::size_t Resident_Evil_Animation::NewFrame(std::size_t iAnim)
{
	Anim[iAnim].push_back(Resident_Evil_Frame());
	Anim[iAnim][0].Rotation.resize(Joint.size());
	return Anim[iAnim].size();
}


/*
	Get frame packet length
*/
std::uint16_t Resident_Evil_Animation::GetFrameLength(void) const
{
	if (Joint.empty()) { return 0; }
	std::uint16_t FrameLen = static_cast<std::uint16_t>((Joint.size() * sizeof(SVECTOR2)) + (sizeof(SVECTOR2) * 2));
	StdFile File;
	File.Align(FrameLen, 4);
	return FrameLen;
}


/*
	Get EDD file size
*/
std::uintmax_t Resident_Evil_Animation::SizeEDD(void)
{
	if (!b_EddOpen) { return 0; }

	std::uintmax_t Size = Anim.size() * sizeof(Resident_Evil_EDD_Header);

	for (std::size_t i = 0; i < Anim.size(); i++)
	{
		Size += Anim[i].size() * 4;
	}

	if (std::to_underlying(Game) & (AUG95 | OCT95)) {}
	else
	{
		if (std::to_underlying(Type) & (WEAPON)) {}
		else
		{
			Size += 4;
		}
	}

	return Size;
}


/*
	Get EMR file size
*/
std::uintmax_t Resident_Evil_Animation::SizeEMR(void)
{
	if (!b_EmrOpen) { return 0; }

	std::uintmax_t FileSize = sizeof(Resident_Evil_EMR_Header);

	StdFile File;

	FileSize += (Joint.size() * sizeof(SVECTOR2));
	File.Align(FileSize, 4);

	for (auto& i : Joint)
	{
		FileSize += i.Ref.size();
	}

	FileSize += (Joint.size() * 4);
	File.Align(FileSize, 4);

	std::size_t nFrames = GetUniqueFrameTotal(true);

	return FileSize + (nFrames * GetFrameLength());
}


/*
	Close EDD
*/
void Resident_Evil_Animation::CloseEDD(void)
{
	b_EddOpen = false;
	for (std::size_t iAnim = 0; iAnim < Anim.size(); iAnim++)
	{
		for (std::size_t iFrame = 0; iFrame < Anim[iAnim].size(); iFrame++)
		{
			Anim[iAnim][iFrame].Rotation.clear();
		}
		Anim[iAnim].clear();
	}
	Anim.clear();
}


/*
	Close EMR
*/
void Resident_Evil_Animation::CloseEMR(void)
{
	b_EmrOpen = false;
	for (std::size_t iArm = 0; iArm < Joint.size(); iArm++)
	{
		Joint[iArm].Ref.clear();
	}
	Joint.clear();
}