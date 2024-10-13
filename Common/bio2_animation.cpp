/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO: 
*
*/


#include "bio2_animation.h"


/*
	Open
*/
std::uintmax_t Resident_Evil_2_Animation::OpenEDD(StdFile& File, std::uintmax_t _Ptr)
{
	if (b_EddOpen) { CloseEDD(); }

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read, true, false))
		{
			Str->Message("Resident Evil 2 Animation: Error, could not open EDD at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
			return _Ptr;
		}
	}

	std::size_t nAnimation = 0;
	File.Read(_Ptr + 2, &nAnimation, sizeof(std::uint16_t));
	nAnimation /= 4;

	if (!nAnimation)
	{
		return _Ptr + 4;
	}

	std::vector<Resident_Evil_EDD_Header> Header(nAnimation);
	File.Read(_Ptr, Header.data(), Header.size() * sizeof(Resident_Evil_EDD_Header));

	Anim.resize(Header.size());

	struct EDD_Attr
	{
		std::uint32_t pEmr : 12;
		std::uint32_t Speed : 10;
		std::uint32_t Sound : 10;
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
			Anim[i][x].Attr.Sound = Attr.Sound;
		}
	}

	b_EddOpen = true;

	return _Ptr + SizeEDD();
}


/*
	Open
*/
bool Resident_Evil_2_Animation::OpenEDD(std::filesystem::path Input, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Input);

	OpenEDD(m_File, _Ptr);

	return b_EddOpen;
}


/*
	Open
*/
std::uintmax_t Resident_Evil_2_Animation::OpenEMR(StdFile& File, std::uintmax_t _Ptr)
{
	if (b_EmrOpen) { CloseEMR(); }

	if (!b_EddOpen)
	{
		Str->Message("Resident Evil 2 Animation: Error, EDD must be open first");
		return _Ptr;
	}

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read, true, false))
		{
			Str->Message("Resident Evil 2 Animation: Error, could not open EMR at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
			return _Ptr;
		}
	}

	Resident_Evil_EMR_Header Header{};
	File.Read(_Ptr, &Header, sizeof(Resident_Evil_EMR_Header));

	if (!Header.pJoint && !Header.pFrames)
	{
		return _Ptr + 4;
	}

	Joint.resize(Header.nJoint);

	if (Header.pFrames != sizeof(Resident_Evil_EMR_Header))
	{
		struct Joint_Index
		{
			std::uint16_t Count;
			std::uint16_t Pointer;
		};

		std::vector<Joint_Index> JointIndex(Header.nJoint);
		File.Read(_Ptr + Header.pJoint, JointIndex.data(), JointIndex.size() * sizeof(Joint_Index));

		for (std::size_t i = 0; i < Joint.size(); i++)
		{
			Joint[i].Ref.resize(JointIndex[i].Count);
			File.Read(_Ptr + Header.pJoint + JointIndex[i].Pointer, Joint[i].Ref.data(), Joint[i].Ref.size());
			File.Read(_Ptr + sizeof(Resident_Evil_EMR_Header) + (i * sizeof(SVECTOR2)), &Joint[i].Pos, sizeof(SVECTOR2));
		}
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

			std::uintmax_t pFrame = Pointer + static_cast<std::uintmax_t>(pEmrList.back() * Header.FrameLen);

			File.Read(pFrame, &Anim[i][x].Origin, sizeof(SVECTOR2));
			File.Read(pFrame + sizeof(SVECTOR2), &Anim[i][x].Speed, sizeof(SVECTOR2));

			Anim[i][x].Rotation.resize(Joint.size());

			std::vector<std::uint8_t> FrameBuffer(Header.FrameLen - (sizeof(SVECTOR2) * 2));
			File.Read(pFrame + (sizeof(SVECTOR2) * 2), FrameBuffer.data(), FrameBuffer.size());

			for (std::size_t y = 0; y < Joint.size(); y++)
			{
				std::size_t idx = (y >> 1) * 9;

				if (y & 1)
				{
					idx += 4;
				}

				std::uint8_t a = FrameBuffer.data()[idx + 0];
				std::uint8_t b = FrameBuffer.data()[idx + 1];
				std::uint8_t c = FrameBuffer.data()[idx + 2];
				std::uint8_t d = FrameBuffer.data()[idx + 3];
				std::uint8_t e = FrameBuffer.data()[idx + 4];

				if (y & 1)
				{
					Anim[i][x].Rotation[y].x = ((a & 0xF0) >> 4) | (b << 4);
					Anim[i][x].Rotation[y].y = c | ((d & 0x0F) << 8);
					Anim[i][x].Rotation[y].z = ((d & 0xF0) >> 4) | (e << 4);
				}
				else
				{
					Anim[i][x].Rotation[y].x = a | ((b & 0x0F) << 8);
					Anim[i][x].Rotation[y].y = ((b & 0xF0) >> 4) | (c << 4);
					Anim[i][x].Rotation[y].z = d | ((e & 0x0F) << 8);
				}
			}
		}
	}

	b_EmrOpen = true;

	return (Pointer + (pEmrList.size() * Header.FrameLen));
}


/*
	Open
*/
bool Resident_Evil_2_Animation::OpenEMR(std::filesystem::path Input, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Input);

	OpenEMR(m_File, _Ptr);

	return b_EmrOpen;
}


/*
	Save
*/
std::uintmax_t Resident_Evil_2_Animation::SaveEDD(StdFile& File, std::uintmax_t _Ptr)
{
	if (!IsOpen())
	{
		Str->Message("Resident Evil 2 Animation: cannot save EDD, animation is not open");
		return _Ptr;
	}

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Write, true, false))
		{
			Str->Message("Resident Evil 2 Animation: Error, could not create EDD at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
			return _Ptr;
		}
	}

	std::uintmax_t pIndex = _Ptr;

	std::uintmax_t pAnim = _Ptr + (Anim.size() * sizeof(Resident_Evil_EDD_Header));

	std::vector<Resident_Evil_EDD_Header> Header(Anim.size());

	struct EDD_Attr
	{
		std::uint32_t pEmr : 12;
		std::uint32_t Speed : 10;
		std::uint32_t Sound : 10;
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
			Attr.Sound = Anim[i][x].Attr.Sound;
			File.Write(pAnim + (x * sizeof(EDD_Attr)), &Attr, sizeof(EDD_Attr));
		}
		pAnim += Anim[i].size() * sizeof(EDD_Attr);
	}

	std::uintmax_t FileSize = (SizeEDD() - sizeof(std::int32_t));
	File.Write(_Ptr + FileSize, &FileSize, sizeof(std::uint32_t));

	return _Ptr + FileSize + sizeof(std::int32_t);
}


/*
	Save
*/
bool Resident_Evil_2_Animation::SaveEDD(std::filesystem::path Output, std::uintmax_t _Ptr)
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
std::uintmax_t Resident_Evil_2_Animation::SaveEMR(StdFile& File, std::uintmax_t _Ptr)
{
	if (!IsOpen())
	{
		Str->Message("Resident Evil 2 Animation: cannot save EMR, animation is not open");
		return _Ptr;
	}

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Write, true, false))
		{
			Str->Message("Resident Evil 2 Animation: Error, could not create EMR at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
			return _Ptr;
		}
	}

	Resident_Evil_EMR_Header Header{};

	Header.nJoint = static_cast<std::uint16_t>(Joint.size());
	Header.FrameLen = GetFrameLength();

	std::size_t FileSize = SizeEMR();

	if (std::to_underlying(Type) & (NORMAL | NORMAL_EX | WEAPON_EX0 | WEAPON_EX1))
	{
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
	}
	if (std::to_underlying(Type) & EXTENDED)
	{
		Header.pJoint += static_cast<std::uint16_t>(sizeof(Resident_Evil_EMR_Header) + (Joint.size() * sizeof(SVECTOR2)));
		File.Align(Header.pJoint, 4);

		Header.pFrames = static_cast<std::uint16_t>(sizeof(Resident_Evil_EMR_Header));
	}
	if (std::to_underlying(Type) & (DAMAGE | WEAPON | ROOM))
	{
		Header.pJoint = 0;
		Header.pFrames = static_cast<std::uint16_t>(sizeof(Resident_Evil_EMR_Header));
	}

	for (std::size_t i = 0; i < Anim.size(); i++)
	{
		for (std::size_t x = 0; x < Anim[i].size(); x++)
		{
			std::uintmax_t pFrame = _Ptr + Header.pFrames + static_cast<std::uintmax_t>(Anim[i][x].Attr.pEmr * Header.FrameLen);

			File.Write(pFrame, &Anim[i][x].Origin, sizeof(SVECTOR2));
			File.Write(pFrame + sizeof(SVECTOR2), &Anim[i][x].Speed, sizeof(SVECTOR2));

			pFrame += sizeof(SVECTOR2) * 2;

			std::vector<std::uint8_t> FrameBuffer(Header.FrameLen - (sizeof(SVECTOR2) * 2));

			for (std::size_t y = 0, idx = 0; y < Joint.size(); y++)
			{
				if ((y & 1) == 0)
				{
					idx = (y >> 1) * 9;
					FrameBuffer[idx + 0] = static_cast<std::uint8_t>(Anim[i][x].Rotation[y].x), FrameBuffer[idx + 1] = (Anim[i][x].Rotation[y].x >> 8);
					FrameBuffer[idx + 1] |= (Anim[i][x].Rotation[y].y << 4), FrameBuffer[idx + 2] = (Anim[i][x].Rotation[y].y >> 4);
					FrameBuffer[idx + 3] = static_cast<std::uint8_t>(Anim[i][x].Rotation[y].z), FrameBuffer[idx + 4] = (Anim[i][x].Rotation[y].z >> 8);
				}
				else
				{
					idx += 4;
					FrameBuffer[idx + 0] |= (Anim[i][x].Rotation[y].x << 4), FrameBuffer[idx + 1] = (Anim[i][x].Rotation[y].x >> 4);
					FrameBuffer[idx + 2] = static_cast<std::uint8_t>(Anim[i][x].Rotation[y].y), FrameBuffer[idx + 3] = (Anim[i][x].Rotation[y].y >> 8);
					FrameBuffer[idx + 3] |= (Anim[i][x].Rotation[y].z << 4), FrameBuffer[idx + 4] = (Anim[i][x].Rotation[y].z >> 4);
				}
			}
			File.Write(pFrame, FrameBuffer.data(), FrameBuffer.size());
		}
	}

	File.Write(_Ptr, &Header, sizeof(Resident_Evil_EMR_Header));

	return _Ptr + FileSize;
}


/*
	Save
*/
bool Resident_Evil_2_Animation::SaveEMR(std::filesystem::path Output, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Output);

	std::uintmax_t OldPtr = _Ptr;

	_Ptr = SaveEMR(m_File, _Ptr);

	return OldPtr != _Ptr;
}


/*
	Update EDD index
*/
std::size_t Resident_Evil_2_Animation::GetUniqueFrameTotal(bool UpdateEDD)
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
	Get frame packet length
*/
std::uint16_t Resident_Evil_2_Animation::GetFrameLength(void) const
{
	if (Joint.empty()) { return 0; }

	StdFile File;

	std::size_t idx = 0;

	for (std::size_t y = 0; y < Joint.size(); y++)
	{
		idx = (y >> 1) * 9;
		if (y & 1)
		{
			idx += 4;
		}
	}

	std::uint16_t FrameLen = static_cast<std::uint16_t>((idx + 5) + (sizeof(SVECTOR2) * 2));

	File.Align(FrameLen, 4);

	return FrameLen;
}


/*
	Get EDD file size
*/
std::uintmax_t Resident_Evil_2_Animation::SizeEDD(void)
{
	if (!b_EddOpen) { return 0; }

	std::uintmax_t Size = Anim.size() * sizeof(Resident_Evil_EDD_Header);

	for (std::size_t i = 0; i < Anim.size(); i++)
	{
		Size += Anim[i].size() * 4;
	}

	Size += sizeof(std::int32_t);

	return Size;
}


/*
	Get EMR file size
*/
std::uintmax_t Resident_Evil_2_Animation::SizeEMR(void)
{
	if (!b_EmrOpen) { return 0; }

	std::uintmax_t FileSize = sizeof(Resident_Evil_EMR_Header);

	if (std::to_underlying(Type) & NORMAL)
	{
		StdFile File;

		FileSize += (Joint.size() * sizeof(SVECTOR2));
		File.Align(FileSize, 4);

		for (auto& i : Joint)
		{
			FileSize += i.Ref.size();
		}

		FileSize += (Joint.size() * 4);
		File.Align(FileSize, 4);
	}

	std::size_t nFrames = GetUniqueFrameTotal(true);

	return FileSize + (nFrames * GetFrameLength());
}


/*
	Export Resident Evil animation
*/
std::unique_ptr<Resident_Evil_Animation> Resident_Evil_2_Animation::ExportBio1(void)
{
	std::unique_ptr<Resident_Evil_Animation> Bio1 = std::make_unique<Resident_Evil_Animation>();

	Bio1->SetGame(Resident_Evil_Video_Game::Resident_Evil);

	Bio1->SetType(Resident_Evil_Animation_Type::Normal);

	if (!IsOpen())
	{
		Str->Message("Resident Evil 2 Animation: Error, animation is not open");
		return Bio1;
	}

	if (!GetJointCount())
	{
		Str->Message("Resident Evil 2 Animation: Error, no joints");
		return Bio1;
	}

	if (!GetAnimationCount())
	{
		Str->Message("Resident Evil 2 Animation: Error, no animations");
		return Bio1;
	}

	for (std::size_t i = 0; i < Joint.size(); i++)
	{
		Bio1->AddJoint(GetJoint(i));
	}

	for (std::size_t i = 0; i < Anim.size(); i++)
	{
		Bio1->AddAnimation(GetAnimation(i));
	}

	Bio1->ForceOpen();

	return Bio1;
}