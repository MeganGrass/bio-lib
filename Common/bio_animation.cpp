/*
*
*	Megan Grass
*	March 07, 2024
*
*/


#include "bio_animation.h"


std::uintmax_t Resident_Evil_Animation::OpenEDD(StdFile& File, std::uintmax_t _Ptr)
{
	if (b_EddOpen) { Close(); }

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read, true, false))
		{
			Str.Message(L"Resident Evil Animation Error: could not read EDD data at 0x%llX in \"%ws\"", _Ptr, File.GetPath().filename().wstring().c_str());
			return _Ptr;
		}
	}

	std::size_t nAnimation = 0;
	File.Read(_Ptr + 2, &nAnimation, sizeof(std::uint16_t));

	if (GameType() != BIO3) { nAnimation /= 4; } else { nAnimation /= 8; }

	if (!nAnimation) { return _Ptr + 4; }	// Bio2 and Bio3 uses 4byte padding for dummy EDD, Bio1 does not (zero byte filesize)

	struct EDD_Header
	{
		std::uint16_t nFrames;
		std::uint16_t pAttr;
		std::uint32_t Counter;
	};

	std::vector<EDD_Header> Header(nAnimation);
	Clip.resize(Header.size());

	if (GameType() & BIO3)
	{
		File.Read(_Ptr, Header.data(), Header.size() * sizeof(EDD_Header));
	}
	else
	{
		for (std::size_t i = 0; i < Clip.size(); i++)
		{
			File.Read(_Ptr + (i * 0x04), &Header[i], 0x04);
		}
	}

	for (std::size_t i = 0; i < Clip.size(); i++)
	{
		Clip[i].resize(Header[i].nFrames);

		for (std::size_t x = 0; x < Clip[i].size(); x++)
		{
			if (GameType() & (AUG95 | OCT95 | BIO1))
			{
				struct EDD_Attr
				{
					std::uint32_t pEmr : 16;
					std::uint32_t Speed : 16;
				};
				EDD_Attr Attr{};
				File.Read(_Ptr + Header[i].pAttr + (x * sizeof(EDD_Attr)), &Attr, sizeof(EDD_Attr));
				Clip[i][x].Attr.pEmr = Attr.pEmr;
				Clip[i][x].Attr.Speed = Attr.Speed;
				Clip[i][x].Attr.Sound = 0;
			}
			else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2))
			{
				struct EDD_Attr
				{
					std::uint32_t pEmr : 12;
					std::uint32_t Speed : 10;
					std::uint32_t Sound : 10;
				};
				EDD_Attr Attr{};
				File.Read(_Ptr + Header[i].pAttr + (x * sizeof(EDD_Attr)), &Attr, sizeof(EDD_Attr));
				Clip[i][x].Attr.pEmr = Attr.pEmr;
				Clip[i][x].Attr.Speed = Attr.Speed;
				Clip[i][x].Attr.Sound = Attr.Sound;
			}
			else if (GameType() & BIO3)
			{
				struct EDD_Attr
				{
					std::uint16_t pEmr : 8;
					std::uint16_t Flag : 8;
				};
				EDD_Attr Attr{};
				File.Read(_Ptr + Header[i].pAttr + (x * sizeof(EDD_Attr)), &Attr, sizeof(EDD_Attr));
				Clip[i][x].Attr.pEmr = Header[i].Counter + Attr.pEmr;
				if (Attr.Flag & 2) { Clip[i][x].Attr.Speed = 1; }
				if (Attr.Flag & 4) { Clip[i][x].Attr.Sound = 1; }
			}
		}
	}

	b_EddOpen = true;

	if (GameType() & BIO3)
	{
		std::int16_t Eof = 0;
		File.Read(_Ptr + Header[Header.size() - 1].pAttr + (std::uintmax_t)(Header[Header.size() - 1].nFrames * 0x02), &Eof, sizeof(std::int16_t));
		if (Eof == -1)
		{
			return _Ptr + SizeEDD() + sizeof(std::int16_t);
		}
	}

	return _Ptr + SizeEDD();
}

std::uintmax_t Resident_Evil_Animation::OpenEMR(StdFile& File, std::uintmax_t _Ptr)
{
	if (b_EmrOpen) { Close(); }

	if (!b_EddOpen)
	{
		Str.Message(L"Resident Evil Animation Error: EDD must be open before corresponding EMR");
		return _Ptr;
	}

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read, true, false))
		{
			Str.Message(L"Resident Evil Animation Error: could not read EMR data at 0x%llX in \"%ws\"", _Ptr, File.GetPath().filename().wstring().c_str());
			return _Ptr;
		}
	}

	struct EMR_Header
	{
		std::uint16_t pJoint;
		std::uint16_t pFrames;
		std::uint16_t nJoint;
		std::uint16_t FrameLen;
	};

	EMR_Header Header{};
	File.Read(_Ptr, &Header, sizeof(EMR_Header));

	if (!Header.pJoint && !Header.pFrames) { return _Ptr + 4; }	// Bio2 and Bio3 uses 4byte padding for dummy EMR, Bio1 does not (zero byte filesize)

	Joints.resize(Header.nJoint);

	if (Header.pFrames != sizeof(EMR_Header))
	{
		struct Joint_Index
		{
			std::uint16_t Count;
			std::uint16_t Pointer;
		};

		std::vector<Joint_Index> JointIndex(Header.nJoint);
		File.Read(_Ptr + Header.pJoint, JointIndex.data(), JointIndex.size() * sizeof(Joint_Index));

		for (std::size_t i = 0; i < Joints.size(); i++)
		{
			Joints[i].Ref.resize(JointIndex[i].Count);
			File.Read(_Ptr + Header.pJoint + JointIndex[i].Pointer, Joints[i].Ref.data(), Joints[i].Ref.size());
			File.Read(_Ptr + sizeof(EMR_Header) + (i * sizeof(SVECTOR2)), &Joints[i].Pos, sizeof(SVECTOR2));
		}
	}

	std::uintmax_t Pointer = _Ptr + Header.pFrames;
	std::vector<std::uint32_t> pEmr;

	for (std::size_t i = 0; i < Clip.size(); i++)
	{
		for (std::size_t x = 0; x < Clip[i].size(); x++)
		{
			pEmr.push_back(Clip[i][x].Attr.pEmr);
			for (std::size_t y = 0; y < pEmr.size() - 1; y++)
			{
				if (Clip[i][x].Attr.pEmr == pEmr[y])
				{
					pEmr.pop_back();
					break;
				}
			}

			if (GameType() & (AUG95 | OCT95 | BIO1))
			{
				std::uintmax_t pFrame = Pointer + (std::uintmax_t)(Clip[i][x].Attr.pEmr * Header.FrameLen);
				File.Align(pFrame, 4);

				Clip[i][x].Rotation.resize(Joints.size());

				File.Read(pFrame, &Clip[i][x].Origin, sizeof(SVECTOR2));
				File.Read(pFrame + sizeof(SVECTOR2), &Clip[i][x].Speed, sizeof(SVECTOR2));
				File.Read(pFrame + (sizeof(SVECTOR2) * 2), Clip[i][x].Rotation.data(), Clip[i][x].Rotation.size() * sizeof(SVECTOR2));
			}
			else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2 | BIO3))
			{
				std::uintmax_t pFrame = Pointer + (std::uintmax_t)(pEmr.back() * Header.FrameLen);

				std::vector<std::uint8_t> FrameBuffer;

				if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2))
				{
					File.Read(pFrame, &Clip[i][x].Origin, sizeof(SVECTOR2));
					File.Read(pFrame + sizeof(SVECTOR2), &Clip[i][x].Speed, sizeof(SVECTOR2));

					Clip[i][x].Rotation.resize(Joints.size());

					FrameBuffer.resize(Header.FrameLen - (sizeof(SVECTOR2) * 2));
					File.Read(pFrame + (sizeof(SVECTOR2) * 2), FrameBuffer.data(), FrameBuffer.size());
				}
				else if (GameType() & BIO3)
				{
					File.Read(pFrame, &Clip[i][x].Speed, sizeof(SVECTOR2));
					File.Read(pFrame + sizeof(SVECTOR2), &Clip[i][x].Origin.y, sizeof(std::int16_t));

					Clip[i][x].Rotation.resize(Joints.size());

					FrameBuffer.resize(Header.FrameLen - (sizeof(SVECTOR2) + sizeof(std::int16_t)));
					File.Read(pFrame + sizeof(SVECTOR2) + sizeof(std::int16_t), FrameBuffer.data(), FrameBuffer.size());
				}

				for (std::size_t y = 0; y < Joints.size(); y++)
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
						Clip[i][x].Rotation[y].x = ((a & 0xF0) >> 4) | (b << 4);
						Clip[i][x].Rotation[y].y = c | ((d & 0x0F) << 8);
						Clip[i][x].Rotation[y].z = ((d & 0xF0) >> 4) | (e << 4);
					}
					else
					{
						Clip[i][x].Rotation[y].x = a | ((b & 0x0F) << 8);
						Clip[i][x].Rotation[y].y = ((b & 0xF0) >> 4) | (c << 4);
						Clip[i][x].Rotation[y].z = d | ((e & 0x0F) << 8);
					}
				}
			}
		}
	}

	b_EmrOpen = true;

	if (GameType() & (AUG95 | OCT95 | BIO1))
	{
		std::uintmax_t FileSize = Pointer + pEmr.size() * Header.FrameLen;

		if ((GameType() & (AUG95 | OCT95)) || (AnimType() & (WEAPON)))
		{
			return FileSize;
		}
		else
		{
			return FileSize + sizeof(std::uint32_t);
		}
	}

	return (Pointer + (pEmr.size() * Header.FrameLen));
}

std::uintmax_t Resident_Evil_Animation::SaveEDD(StdFile& File, std::uintmax_t _Ptr)
{
	if (!b_EddOpen || Clip.empty())
	{
		Str.Message(L"Resident Evil Animation Error: cannot save EDD, animation data is empty");
		return _Ptr;
	}

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read_Ex, true, false))
		{
			if (!File.Open(File.GetPath(), FileAccessMode::Write_Ex, true, true))
			{
				Str.Message(L"Resident Evil Animation Error: could not write EDD data at 0x%llX in \"%ws\"", _Ptr, File.GetPath().filename().wstring().c_str());
				return _Ptr;
			}
		}
	}

	struct EDD_Header
	{
		std::uint16_t nFrames;
		std::uint16_t pAttr;
		std::uint32_t Counter;
	};

	struct EDD_Attr_Bio1
	{
		std::uint32_t pEmr : 16;
		std::uint32_t Speed : 16;
	};
	struct EDD_Attr_Bio2
	{
		std::uint32_t pEmr : 12;
		std::uint32_t Speed : 10;
		std::uint32_t Sound : 10;
	};
	struct EDD_Attr_Bio3
	{
		std::uint16_t pEmr : 8;
		std::uint16_t Flag : 8;
	};

	std::vector<EDD_Header> Header(Clip.size());

	std::uintmax_t pAnim = _Ptr + (Clip.size() * sizeof(EDD_Header));

	if (GameType() != BIO3)
	{
		pAnim = _Ptr + (Clip.size() * 4);

		GetUniqueFrameCountTotal(b_EraseDuplicates);
	}

	std::uint32_t Counter = 0;

	for (std::size_t i = 0; i < Clip.size(); i++)
	{
		Header[i].nFrames = (std::uint16_t)(Clip[i].size());
		Header[i].pAttr = (std::uint16_t)(pAnim - _Ptr);
		Header[i].Counter = Counter;

		if (GameType() & BIO3)
		{
			Counter += (std::uint32_t)(GetUniqueFrameCount(i, b_EraseDuplicates));
			File.Write(_Ptr + (i * sizeof(EDD_Header)), &Header[i], sizeof(EDD_Header));
		}
		else
		{
			File.Write(_Ptr + (i * 0x04), &Header[i], 0x04);
		}

		for (std::size_t x = 0; x < Clip[i].size(); x++)
		{
			if (GameType() & (AUG95 | OCT95 | BIO1))
			{
				EDD_Attr_Bio1 Attr{ Clip[i][x].Attr.pEmr, Clip[i][x].Attr.Speed };
				File.Write(pAnim + (x * sizeof(EDD_Attr_Bio1)), &Attr, sizeof(EDD_Attr_Bio1));
			}
			else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2))
			{
				EDD_Attr_Bio2 Attr{ Clip[i][x].Attr.pEmr, Clip[i][x].Attr.Speed, Clip[i][x].Attr.Sound };
				File.Write(pAnim + (x * sizeof(EDD_Attr_Bio2)), &Attr, sizeof(EDD_Attr_Bio2));
			}
			else if (GameType() & BIO3)
			{
				EDD_Attr_Bio3 Attr{ (std::uint16_t)Clip[i][x].Attr.pEmr, (std::uint16_t)((Clip[i][x].Attr.Speed == 1 ? 2 : 0) | (Clip[i][x].Attr.Sound == 1 ? 4 : 0)) };
				File.Write(pAnim + (x * sizeof(EDD_Attr_Bio3)), &Attr, sizeof(EDD_Attr_Bio3));
			}
		}

		if (GameType() & (AUG95 | OCT95 | BIO1)) { pAnim += Clip[i].size() * sizeof(EDD_Attr_Bio1); }
		else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2)) { pAnim += Clip[i].size() * sizeof(EDD_Attr_Bio2); }
		else if (GameType() & BIO3) { pAnim += Clip[i].size() * sizeof(EDD_Attr_Bio3); }
	}

	if (GameType() != BIO3)
	{
		std::uintmax_t FileSize = (SizeEDD() - sizeof(std::uint32_t));
		File.Write(_Ptr + FileSize, &FileSize, sizeof(std::uint32_t));
	}
	else
	{
		std::int16_t EofBytes = -1;
		std::uintmax_t FileSize = SizeEDD() - sizeof(std::int16_t);
		File.Write(_Ptr + FileSize - sizeof(std::int16_t), &EofBytes, sizeof(std::int16_t));
		File.Write(_Ptr + FileSize, &FileSize, sizeof(std::uint32_t));
	}

	return 0;
}

std::uintmax_t Resident_Evil_Animation::SaveEMR(StdFile& File, std::uintmax_t _Ptr)
{
	if (!b_EmrOpen || Clip.empty())
	{
		Str.Message(L"Resident Evil Animation Error: cannot save EMR, animation data is empty");
		return _Ptr;
	}

	if (!File.IsOpen())
	{
		if (!File.Open(File.GetPath(), FileAccessMode::Read_Ex, true, false))
		{
			if (!File.Open(File.GetPath(), FileAccessMode::Write_Ex, true, true))
			{
				Str.Message(L"Resident Evil Animation Error: could not write EMR data at 0x%llX in \"%ws\"", _Ptr, File.GetPath().filename().wstring().c_str());
				return _Ptr;
			}
		}
	}

	struct EMR_Header
	{
		std::uint16_t pJoint;
		std::uint16_t pFrames;
		std::uint16_t nJoint;
		std::uint16_t FrameLen;
	};

	EMR_Header Header { 0, 0, (std::uint16_t)Joints.size(), GetFrameLength() };

	if (AnimType() & (NORMAL | NORMAL_EX1 | WEAPON_EX0 | WEAPON_EX1))
	{
		struct Joint_Index
		{
			std::uint16_t Count;
			std::uint16_t Pointer;
		};

		std::size_t ObjCount = 0;

		std::vector<Joint_Index> JointIndex(Joints.size());

		Header.pJoint += (std::uint16_t)(sizeof(EMR_Header) + (Joints.size() * sizeof(SVECTOR2)));
		File.Align(Header.pJoint, 4);

		for (std::size_t i = 0; i < Joints.size(); i++)
		{
			JointIndex[i].Count = (std::uint16_t)(Joints[i].Ref.size());
			JointIndex[i].Pointer = (std::uint16_t)((Joints.size() * sizeof(Joint_Index)) + ObjCount);
			ObjCount += Joints[i].Ref.size();

			File.Write(_Ptr + Header.pJoint + (i * sizeof(Joint_Index)), &JointIndex[i], sizeof(Joint_Index));
			File.Write(_Ptr + Header.pJoint + JointIndex[i].Pointer, Joints[i].Ref.data(), Joints[i].Ref.size());
			File.Write(_Ptr + sizeof(EMR_Header) + (i * sizeof(SVECTOR2)), &Joints[i].Pos, sizeof(SVECTOR2));
		}

		Header.pFrames = Header.pJoint + (std::uint16_t)((Joints.size() * sizeof(Joint_Index)) + ObjCount);
		File.Align(Header.pFrames, 4);
	}

	if (AnimType() & NORMAL_EX0)
	{
		Header.pJoint = (std::uint16_t)(sizeof(EMR_Header) + (Joints.size() * sizeof(SVECTOR2)));
		File.Align(Header.pJoint, 4);

		Header.pFrames = (std::uint16_t)(sizeof(EMR_Header));
	}

	if (AnimType() & (DAMAGE | WEAPON | ROOM))
	{
		Header.pFrames = (std::uint16_t)(sizeof(EMR_Header));
	}

	File.Write(_Ptr, &Header, sizeof(EMR_Header));

	std::uintmax_t pFrame = 0;
	std::uintmax_t Pointer = _Ptr + Header.pFrames;
	std::vector<std::uint32_t> pEmr;

	for (std::size_t i = 0; i < Clip.size(); i++)
	{
		for (std::size_t x = 0; x < Clip[i].size(); x++)
		{
			if (GameType() & (AUG95 | OCT95 | BIO1))
			{
				std::uintmax_t pFrame = _Ptr + Header.pFrames + (std::uintmax_t)(Clip[i][x].Attr.pEmr * Header.FrameLen);
				File.Align(pFrame, 4);

				File.Write(pFrame, &Clip[i][x].Origin, sizeof(SVECTOR2));
				File.Write(pFrame + sizeof(SVECTOR2), &Clip[i][x].Speed, sizeof(SVECTOR2));
				File.Write(pFrame + (sizeof(SVECTOR2) * 2), Clip[i][x].Rotation.data(), Clip[i][x].Rotation.size() * sizeof(SVECTOR2));
			}
			else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2 | BIO3))
			{
				std::vector<std::uint8_t> FrameBuffer;

				if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2))
				{
					pFrame = _Ptr + Header.pFrames + (std::uintmax_t)(Clip[i][x].Attr.pEmr * Header.FrameLen);

					File.Write(pFrame, &Clip[i][x].Origin, sizeof(SVECTOR2));
					File.Write(pFrame + sizeof(SVECTOR2), &Clip[i][x].Speed, sizeof(SVECTOR2));

					pFrame += sizeof(SVECTOR2) * 2;

					FrameBuffer.resize(Header.FrameLen - (sizeof(SVECTOR2) * 2));
				}
				else if (GameType() & BIO3)
				{
					pEmr.push_back(Clip[i][x].Attr.pEmr);
					for (std::size_t y = 0; y < pEmr.size() - 1; y++)
					{
						if (Clip[i][x].Attr.pEmr == pEmr[y])
						{
							pEmr.pop_back();
							break;
						}
					}

					pFrame = Pointer + (std::uintmax_t)(pEmr.back() * Header.FrameLen);

					File.Write(pFrame, &Clip[i][x].Speed, sizeof(SVECTOR2));
					File.Write(pFrame + sizeof(SVECTOR2), &Clip[i][x].Origin.y, sizeof(std::int16_t));

					pFrame += sizeof(SVECTOR2) + sizeof(std::int16_t);

					FrameBuffer.resize(Header.FrameLen - (sizeof(SVECTOR2) + sizeof(std::int16_t)));
				}

				for (std::size_t y = 0, idx = 0; y < Joints.size(); y++)
				{
					if ((y & 1) == 0)
					{
						idx = (y >> 1) * 9;
						FrameBuffer[idx + 0] = (std::uint8_t)(Clip[i][x].Rotation[y].x), FrameBuffer[idx + 1] = (Clip[i][x].Rotation[y].x >> 8);
						FrameBuffer[idx + 1] |= (Clip[i][x].Rotation[y].y << 4), FrameBuffer[idx + 2] = (Clip[i][x].Rotation[y].y >> 4);
						FrameBuffer[idx + 3] = (std::uint8_t)(Clip[i][x].Rotation[y].z), FrameBuffer[idx + 4] = (Clip[i][x].Rotation[y].z >> 8);
					}
					else
					{
						idx += 4;
						FrameBuffer[idx + 0] |= (Clip[i][x].Rotation[y].x << 4), FrameBuffer[idx + 1] = (Clip[i][x].Rotation[y].x >> 4);
						FrameBuffer[idx + 2] = (std::uint8_t)(Clip[i][x].Rotation[y].y), FrameBuffer[idx + 3] = (Clip[i][x].Rotation[y].y >> 8);
						FrameBuffer[idx + 3] |= (Clip[i][x].Rotation[y].z << 4), FrameBuffer[idx + 4] = (Clip[i][x].Rotation[y].z >> 4);
					}
				}

				File.Write(pFrame, FrameBuffer.data(), FrameBuffer.size());
			}
		}

		if (GameType() & BIO3)
		{
			Pointer += (pEmr.size() * Header.FrameLen);
			pEmr.clear();
		}
	}

	std::size_t FileSize = SizeEMR();

	if (GameType() & (AUG95 | OCT95 | BIO1))
	{
		if ((GameType() & (AUG95 | OCT95)) || (AnimType() & (WEAPON)))
		{
			File.Close();
			File.ResizeAlign(4);
			return _Ptr + FileSize;
		}

		std::uint32_t _Size = static_cast<std::uint32_t>(FileSize - sizeof(EMR_Header));

		File.Write(_Ptr + FileSize, &_Size, sizeof(std::uint32_t));

		return _Ptr + FileSize + 4;
	}

	File.Write(_Ptr, &Header, sizeof(EMR_Header));

	return _Ptr + FileSize;
}

std::uintmax_t Resident_Evil_Animation::SizeEDD(void)
{
	if (!b_EddOpen) { return 0; }

	std::uintmax_t Size = 0;

	if (GameType() != BIO3) { Size = Clip.size() * 4; }
	else { Size = Clip.size() * 8; }

	for (std::size_t i = 0; i < Clip.size(); i++)
	{
		if (GameType() != BIO3) { Size += Clip[i].size() * 4; }
		else { Size += Clip[i].size() * 2; }
	}

	if (GameType() & BIO1 && AnimType() != WEAPON)
	{
		Size += sizeof(std::uint32_t);
	}
	else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2 | BIO3))
	{
		Size += sizeof(std::uint32_t);
	}

	return Size;
}

std::uintmax_t Resident_Evil_Animation::SizeEMR(void)
{
	if (!b_EmrOpen) { return 0; }

	std::uintmax_t FileSize = 0x08;	// sizeof(EMR_Header)

	if (Joints.size())
	{
		FileSize += (Joints.size() * sizeof(SVECTOR2));
		StdFile().Align(FileSize, 4);

		for (auto& i : Joints) { FileSize += i.Ref.size(); }

		FileSize += (Joints.size() * 4);	// sizeof(Joint_Index)
		StdFile().Align(FileSize, 4);
	}

	std::size_t nFrames = 0;

	if (GameType() != BIO3)
	{
		nFrames = GetUniqueFrameCountTotal(b_EraseDuplicates);
	}
	else
	{
		for (std::size_t i = 0; i < Clip.size(); i++)
		{
			nFrames += GetUniqueFrameCount(i, b_EraseDuplicates);
		}
	}

	return FileSize + (nFrames * GetFrameLength());
}

void Resident_Evil_Animation::Close(void)
{
	b_EddOpen = false;
	b_EmrOpen = false;

	for (auto& j : Joints)
	{
		j.Ref.clear();
	}

	for (auto& a : Clip)
	{
		for (auto& f : a)
		{
			f.Rotation.clear();
		}
		a.clear();
	}

	Joints.clear();
	Clip.clear();

	Skeleton.reset();
}

std::uint16_t Resident_Evil_Animation::GetFrameLength(void) const
{
	if (Joints.empty()) { return 0; }

	std::size_t idx = 0;
	std::size_t FrameLen = 0;

	for (std::size_t i = 0; i < Joints.size(); i++)
	{
		idx = (i >> 1) * 9;
		if (i & 1) { idx += 4; }
	}

	if (GameType() & (AUG95 | OCT95 | BIO1))
	{
		FrameLen = (Joints.size() * sizeof(SVECTOR2)) + (sizeof(SVECTOR2) * 2);
	}
	else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2))
	{
		FrameLen = (idx + 5) + (sizeof(SVECTOR2) * 2);
	}
	else if (GameType() & BIO3)
	{
		FrameLen = (idx + 5) + (sizeof(SVECTOR2) + sizeof(std::int16_t));
	}

	return (std::uint16_t)StdFile().Align(FrameLen, 4);
}

std::size_t Resident_Evil_Animation::GetUniqueFrameCount(std::size_t iClip, bool UpdateEDD)
{
	if (Clip.empty()) { return 0; }

	iClip = std::clamp(iClip, (std::size_t)(0), Clip.size() - 1);

	std::size_t FrameLen = GetFrameLength();

	std::vector<std::tuple<std::size_t, std::uint32_t>> Table;

	std::uint32_t pEmr = 0;

	for (std::size_t x = 0; x < Clip[iClip].size(); x++)
	{
		if (UpdateEDD) { Clip[iClip][x].Attr.pEmr = pEmr; }

		std::vector<std::uint8_t> Buffer(FrameLen + 8);

		std::memcpy(&Buffer.data()[0], &Clip[iClip][x].Attr.Speed, sizeof(std::uint32_t));
		std::memcpy(&Buffer.data()[4], &Clip[iClip][x].Attr.Sound, sizeof(std::uint32_t));
		std::memcpy(&Buffer.data()[8], &Clip[iClip][x].Origin, sizeof(SVECTOR2));
		std::memcpy(&Buffer.data()[8 + sizeof(SVECTOR2)], &Clip[iClip][x].Speed, sizeof(SVECTOR2));
		std::memcpy(&Buffer.data()[8 + (sizeof(SVECTOR2) * 2)], &Clip[iClip][x].Rotation.data()[0], (FrameLen - (sizeof(SVECTOR2) * 2)));

		std::uint32_t CRC = Cyclic_Redundancy_Check().GetCRC32(Buffer.data(), FrameLen);

		Table.push_back(std::make_tuple(x, CRC));

		pEmr++;

		for (std::size_t y = 0; y < Table.size() - 1; y++)
		{
			if (std::get<1>(Table[y]) == CRC)
			{
				if (UpdateEDD) { Clip[iClip][x].Attr.pEmr = Clip[iClip][std::get<0>(Table[y])].Attr.pEmr; }
				pEmr--;
				Table.pop_back();
				break;
			}
		}
	}

	return Table.size();
}

std::size_t Resident_Evil_Animation::GetUniqueFrameCountTotal(bool UpdateEDD)
{
	if (Clip.empty()) { return 0; }

	std::uintmax_t FrameLen = GetFrameLength();

	std::vector<std::tuple<std::size_t, std::size_t, std::uint32_t>> Table;

	std::uint32_t pEmr = 0;

	for (std::size_t i = 0; i < Clip.size(); i++)
	{
		for (std::size_t x = 0; x < Clip[i].size(); x++)
		{
			if (UpdateEDD) { Clip[i][x].Attr.pEmr = pEmr; }

			std::vector<std::uint8_t> Buffer(FrameLen + 8);

			std::memcpy(&Buffer.data()[0], &Clip[i][x].Attr.Speed, sizeof(std::uint32_t));
			std::memcpy(&Buffer.data()[4], &Clip[i][x].Attr.Sound, sizeof(std::uint32_t));
			std::memcpy(&Buffer.data()[8], &Clip[i][x].Origin, sizeof(SVECTOR2));
			std::memcpy(&Buffer.data()[8 + sizeof(SVECTOR2)], &Clip[i][x].Speed, sizeof(SVECTOR2));
			std::memcpy(&Buffer.data()[8 + (sizeof(SVECTOR2) * 2)], &Clip[i][x].Rotation.data()[0], (FrameLen - (sizeof(SVECTOR2) * 2)));

			std::uint32_t CRC = Cyclic_Redundancy_Check().GetCRC32(Buffer.data(), FrameLen);

			Table.push_back(std::make_tuple(i, x, CRC));

			pEmr++;

			for (std::size_t y = 0; y < Table.size() - 1; y++)
			{
				if (std::get<2>(Table[y]) == CRC)
				{
					if (UpdateEDD) { Clip[i][x].Attr.pEmr = Clip[std::get<0>(Table[y])][std::get<1>(Table[y])].Attr.pEmr; }
					pEmr--;
					Table.pop_back();
					break;
				}
			}
		}
	}

	return Table.size();
}