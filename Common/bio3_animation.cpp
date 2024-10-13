/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO:
*
*/


#include "bio3_animation.h"


struct EDD_Attr_Bio3
{
	std::uint16_t pEmr : 8;
	std::uint16_t Flag : 8;
};
std::vector<std::uint32_t> TestList;
std::uint32_t TestCounter = 0;

static void TestFunctionBio3(Resident_Evil_EDD_Header_Bio3 Index, std::vector<EDD_Attr_Bio3> List)
{
	std::cout << std::hex << "pAttr " << Index.pAttr << " nFrames " << Index.nFrames << " Counter " << Index.Counter << std::endl;
	for (std::size_t i = 0; i < List.size(); i++, TestCounter++)
	{
		TestList.push_back(List[i].pEmr);
		for (std::size_t y = 0; y < TestList.size() - 1; y++)
		{
			if (List[i].pEmr == TestList[y])
			{
				TestCounter--;
				TestList.pop_back();
				break;
			}
		}

		std::cout << std::hex << "\tList: " << List[i].pEmr << "\t\tTotal " << TestCounter << std::endl;
	}

	TestList.clear();

}


/*
	Open
*/
std::uintmax_t Resident_Evil_3_Animation::OpenEDD(StdFile& File, std::uintmax_t _Ptr)
{
	if (b_EddOpen) { CloseEDD(); }

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read, true, false))
		{
			Str->Message("Resident Evil 3 Animation: Error, could not open EDD at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
			return _Ptr;
		}
	}

	std::size_t nAnimation = 0;
	File.Read(_Ptr + 2, &nAnimation, sizeof(std::uint16_t));
	nAnimation /= 8;

	if (!nAnimation)
	{
		return _Ptr + 4;
	}

	std::vector<Resident_Evil_EDD_Header_Bio3> Header(nAnimation);
	File.Read(_Ptr, Header.data(), Header.size() * sizeof(Resident_Evil_EDD_Header_Bio3));

	Anim.resize(Header.size());

	struct EDD_Attr
	{
		std::uint16_t pEmr : 8;
		std::uint16_t Flag : 8;
	};
	EDD_Attr Attr{};

	//std::vector<EDD_Attr_Bio3> Bio3Attr;	// Delete me

	for (std::size_t i = 0; i < Anim.size(); i++)
	{
		Anim[i].resize(Header[i].nFrames);

		//Bio3Attr.resize(Header[i].nFrames);	// Delete me
		//File.Read(_Ptr + Header[i].pAttr, Bio3Attr.data(), Bio3Attr.size() * sizeof(EDD_Attr_Bio3));	// Delete me
		//TestFunctionBio3(Header[i], Bio3Attr);	// Delete me

		for (std::size_t x = 0; x < Anim[i].size(); x++)
		{
			File.Read(_Ptr + Header[i].pAttr + (x * sizeof(EDD_Attr)), &Attr, sizeof(EDD_Attr));
			Anim[i][x].Attr.pEmr = Header[i].Counter + Attr.pEmr;
			if (Attr.Flag & 2)
			{
				Anim[i][x].Attr.Speed = 1;
			}
			if (Attr.Flag & 4)
			{
				Anim[i][x].Attr.Sound = 1;
			}
		}
	}

	b_EddOpen = true;

	std::int16_t EofBytes = 0;
	File.Read(_Ptr + Header[Header.size() - 1].pAttr + (Header[Header.size() - 1].nFrames * sizeof(EDD_Attr)), &EofBytes, sizeof(std::int16_t));
	if (EofBytes == -1)
	{
		return _Ptr + SizeEDD() + sizeof(std::int16_t);
	}
	else
	{
		return _Ptr + SizeEDD();
	}
}


/*
	Open
*/
bool Resident_Evil_3_Animation::OpenEDD(std::filesystem::path Input, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Input);

	OpenEDD(m_File, _Ptr);

	return b_EddOpen;
}


/*
	Open
*/
std::uintmax_t Resident_Evil_3_Animation::OpenEMR(StdFile& File, std::uintmax_t _Ptr)
{
	if (b_EmrOpen) { CloseEMR(); }

	if (!b_EddOpen)
	{
		Str->Message("Resident Evil 3 Animation: Error, EDD must be open first");
		return _Ptr;
	}

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read, true, false))
		{
			Str->Message("Resident Evil 3 Animation: Error, could not open EMR at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
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

			File.Read(pFrame, &Anim[i][x].Speed, sizeof(SVECTOR2));
			File.Read(pFrame + sizeof(SVECTOR2), &Anim[i][x].Origin.y, sizeof(std::int16_t));

			Anim[i][x].Rotation.resize(Joint.size());

			std::vector<std::uint8_t> FrameBuffer(Header.FrameLen - (sizeof(SVECTOR2) + sizeof(std::int16_t)));
			File.Read(pFrame + sizeof(SVECTOR2) + sizeof(std::int16_t), FrameBuffer.data(), FrameBuffer.size());

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
bool Resident_Evil_3_Animation::OpenEMR(std::filesystem::path Input, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Input);

	OpenEMR(m_File, _Ptr);

	return b_EmrOpen;
}


/*
	Save
*/
std::uintmax_t Resident_Evil_3_Animation::SaveEDD(StdFile& File, std::uintmax_t _Ptr)
{
	if (!IsOpen())
	{
		Str->Message("Resident Evil 3 Animation: cannot save EDD, animation is not open");
		return _Ptr;
	}

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Write, true, false))
		{
			Str->Message("Resident Evil 3 Animation: Error, could not create EDD at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
			return _Ptr;
		}
	}

	std::uintmax_t pIndex = _Ptr;

	std::uintmax_t pAnim = _Ptr + (Anim.size() * sizeof(Resident_Evil_EDD_Header_Bio3));

	std::vector<Resident_Evil_EDD_Header_Bio3> Header(Anim.size());

	struct EDD_Attr
	{
		std::uint16_t pEmr : 8;
		std::uint16_t Flag : 8;
	};
	EDD_Attr Attr{};

	std::uint32_t Counter = 0;

	for (std::size_t i = 0; i < Anim.size(); i++, pIndex += sizeof(Resident_Evil_EDD_Header_Bio3))
	{
		Header[i].nFrames = static_cast<std::uint16_t>(Anim[i].size());
		Header[i].pAttr = static_cast<std::uint16_t>(pAnim - _Ptr);
		Header[i].Counter = Counter;
		File.Write(pIndex, &Header[i], sizeof(Resident_Evil_EDD_Header_Bio3));
		Counter += static_cast<std::uint32_t>(GetUniqueFrameCount(i));

		for (std::size_t x = 0; x < Anim[i].size(); x++)
		{
			Attr = {};
			Attr.pEmr = Anim[i][x].Attr.pEmr;
			Anim[i][x].Attr.Speed == 1 ? Attr.Flag |= 2 : 0;
			Anim[i][x].Attr.Sound == 1 ? Attr.Flag |= 4 : 0;
			File.Write(pAnim + (x * sizeof(EDD_Attr)), &Attr, sizeof(EDD_Attr));
		}
		pAnim += Anim[i].size() * sizeof(EDD_Attr);
	}

	std::int16_t EofBytes = -1;
	std::uintmax_t FileSize = SizeEDD() - sizeof(std::int16_t);
	File.Write(_Ptr + FileSize - sizeof(std::int16_t), &EofBytes, sizeof(std::int16_t));
	File.Write(_Ptr + FileSize, &FileSize, sizeof(std::uint32_t));

	return _Ptr + SizeEDD() + sizeof(std::int16_t);
}


/*
	Save
*/
bool Resident_Evil_3_Animation::SaveEDD(std::filesystem::path Output, std::uintmax_t _Ptr)
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
std::uintmax_t Resident_Evil_3_Animation::SaveEMR(StdFile& File, std::uintmax_t _Ptr)
{
	if (!IsOpen())
	{
		Str->Message("Resident Evil 3 Animation: cannot save EMR, animation is not open");
		return _Ptr;
	}

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Write, true, false))
		{
			Str->Message("Resident Evil 3 Animation: Error, could not create EMR at 0x%llX in %s", _Ptr, File.GetPath().filename().string().c_str());
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

			File.Write(pFrame, &Anim[i][x].Speed, sizeof(SVECTOR2));
			File.Write(pFrame + sizeof(SVECTOR2), &Anim[i][x].Origin.y, sizeof(std::int16_t));

			pFrame += sizeof(SVECTOR2) + sizeof(std::int16_t);

			std::vector<std::uint8_t> FrameBuffer(Header.FrameLen - (sizeof(SVECTOR2) + sizeof(std::int16_t)));

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

		Pointer += (pEmrList.size() * Header.FrameLen);
		pEmrList.clear();
	}

	File.Write(_Ptr, &Header, sizeof(Resident_Evil_EMR_Header));

	return _Ptr + FileSize;
}


/*
	Save
*/
bool Resident_Evil_3_Animation::SaveEMR(std::filesystem::path Output, std::uintmax_t _Ptr)
{
	StdFile m_File;

	m_File.SetPath(Output);

	std::uintmax_t OldPtr = _Ptr;

	_Ptr = SaveEMR(m_File, _Ptr);

	return OldPtr != _Ptr;
}


/*
	Get the total amount of unique frames
*/
std::size_t Resident_Evil_3_Animation::GetUniqueFrameTotal(bool UpdateEDD)
{
	std::size_t nFrames = 0;

	for (std::size_t i = 0; i < Anim.size(); i++)
	{
		nFrames += GetUniqueFrameCount(i);
	}

	return nFrames;
}


/*
	Get the amount of unique frames in a single animation
*/
std::size_t Resident_Evil_3_Animation::GetUniqueFrameCount(std::size_t iAnim)
{
	std::uintmax_t FrameLen = GetFrameLength();

	Cyclic_Redundancy_Check Crc;

	std::vector<std::tuple<std::size_t, std::uint32_t>> Table;

	std::uint32_t pEmr = 0;

	for (std::size_t x = 0; x < Anim[iAnim].size(); x++)
	{
		Anim[iAnim][x].Attr.pEmr = pEmr;

		std::vector<std::uint8_t> Buffer(FrameLen + 8);

		std::memcpy(&Buffer.data()[0], &Anim[iAnim][x].Attr.Speed, sizeof(std::uint32_t));
		std::memcpy(&Buffer.data()[4], &Anim[iAnim][x].Attr.Sound, sizeof(std::uint32_t));
		std::memcpy(&Buffer.data()[8], &Anim[iAnim][x].Origin, sizeof(SVECTOR2));
		std::memcpy(&Buffer.data()[8 + sizeof(SVECTOR2)], &Anim[iAnim][x].Speed, sizeof(SVECTOR2));
		std::memcpy(&Buffer.data()[8 + (sizeof(SVECTOR2) * 2)], &Anim[iAnim][x].Rotation.data()[0], (FrameLen - (sizeof(SVECTOR2) * 2)));

		std::uint32_t CRC = Crc.GetCRC32(Buffer.data(), FrameLen);

		Table.push_back(std::make_tuple(x, CRC));

		pEmr++;

		for (std::size_t y = 0; y < Table.size() - 1; y++)
		{
			if (std::get<1>(Table[y]) == CRC)
			{
				Anim[iAnim][x].Attr.pEmr = Anim[iAnim][std::get<0>(Table[y])].Attr.pEmr;
				pEmr--;
				Table.pop_back();
				break;
			}
		}
	}

	return Table.size();
}


/*
	Get frame packet length
*/
std::uint16_t Resident_Evil_3_Animation::GetFrameLength(void) const
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

	std::uint16_t FrameLen = static_cast<std::uint16_t>((idx + 5) + (sizeof(SVECTOR2) + sizeof(std::int16_t)));

	File.Align(FrameLen, 4);

	return FrameLen;
}


/*
	Get EDD file size
*/
std::uintmax_t Resident_Evil_3_Animation::SizeEDD(void)
{
	if (!b_EddOpen) { return 0; }

	std::uintmax_t Size = Anim.size() * sizeof(Resident_Evil_EDD_Header_Bio3);

	for (std::size_t i = 0; i < Anim.size(); i++)
	{
		Size += Anim[i].size() * 2;
	}

	Size += sizeof(std::int32_t);

	return Size;
}


/*
	Get EMR file size
*/
std::uintmax_t Resident_Evil_3_Animation::SizeEMR(void)
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