/*
*
*	Megan Grass
*	March 07, 2024
*
*/


#include "bio_model.h"


std::vector<std::uint32_t> Resident_Evil_Model::GetDataPtr(StdFile& File, std::uintmax_t _FileBeginPtr)
{
	if (!File.IsOpen()) { return std::vector<std::uint32_t>(); }

	std::vector<std::uint32_t> Pointer(0);

	if (GameType() & (AUG95 | OCT95 | BIO1))
	{
		String Extension = File.GetPath().extension().string();

		if (Standard_String().ToUpper(Extension) == ".EMW") { Pointer.resize(3); }
		else if (GameType() & (AUG95 | OCT95)) { Pointer.resize(4); }
		else { Pointer.resize(6); }

		File.Read(File.Size() - sizeof(std::uint32_t) * Pointer.size() + sizeof(std::uint32_t), &Pointer[1], sizeof(std::uint32_t) * Pointer.size() - sizeof(std::uint32_t));
	}
	if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2 | BIO3))
	{
		struct HEADER
		{
			std::uint32_t IndexOffset;
			std::uint32_t DataCount;
		} Header;

		File.Read(_FileBeginPtr, &Header, sizeof(Header));

		Pointer.resize(Header.DataCount);

		File.Read(_FileBeginPtr + Header.IndexOffset, Pointer.data(), Header.DataCount * sizeof(std::uint32_t));
	}

	return Pointer;
}

void Resident_Evil_Model::SetWorld(const MATVECTOR& Vec) const
{
	float XX = GTE->ToFloat(Vec.x);
	float YY = GTE->ToFloat(Vec.y);
	float ZZ = GTE->ToFloat(Vec.z);

	float RX = GTE->ToFloat(std::clamp((std::int16_t)Vec.rx, (std::int16_t)-ONE, (std::int16_t)ONE)) * 360.0f;
	float RY = GTE->ToFloat(std::clamp((std::int16_t)Vec.ry, (std::int16_t)-ONE, (std::int16_t)ONE)) * 360.0f;
	float RZ = GTE->ToFloat(std::clamp((std::int16_t)Vec.rz, (std::int16_t)-ONE, (std::int16_t)ONE)) * 360.0f;

	Standard_Matrix Neg = Standard_Matrix().Translate(-vec3{ XX, YY, ZZ });
	Standard_Matrix Pos = Standard_Matrix().Translate(vec3{ XX, YY, ZZ });
	Standard_Matrix R = Standard_Matrix().YawPitchRoll(vec3{ World->Radian(RX), World->Radian(RY), World->Radian(RZ) });
	Standard_Matrix T = Standard_Matrix().Translate(vec3{ GTE->ToFloat(Vec.x), GTE->ToFloat(Vec.y), GTE->ToFloat(Vec.z) });

	*World = T * (Pos * R * Neg);
}

#if MSTD_DX9
std::unique_ptr<DX9_MODEL> Resident_Evil_Model::ExportDX9(std::unique_ptr<FIXED_MODEL>& Model, std::unique_ptr<Sony_PlayStation_Texture>& Texture) const
{
	std::unique_ptr<DX9_MODEL> Temp = std::make_unique<DX9_MODEL>();

	if (!Model || Model->Obj.empty()) { return Temp; }

	if (Texture && Texture->IsOpen())
	{
		Temp->TextureWidth = (float)Texture->GetWidth();
		Temp->TextureHeight = (float)Texture->GetHeight();
		Temp->Texture.resize(Texture->GetPaletteCount());
		for (uint16_t i = 0; i < Texture->GetPaletteCount(); i++)
		{
			Temp->Texture[i].reset(Render->CreateTexture(Texture, i, Sony_Texture_Transparency::Superblack, 0xFF00FF, true));
		}
	}

	std::vector<vec3cnt> Shape;

	Temp->Object.resize(Model->Obj.size());

	for (size_t i = 0; i < Model->Obj.size(); i++)
	{
		Temp->Object[i].resize(Model->Obj[i].size());

		for (size_t x = 0; x < Model->Obj[i].size(); x++)
		{
			Temp->Object[i][x].iTexture = Model->Obj[i][x].Texture.iPalette;

			Shape.clear();

			for (size_t y = 0; y < Model->Obj[i][x].Vector.size(); y++)
			{
				Shape.push_back(
					vec3cnt{
						vec3{ Model->Obj[i][x].Vector[y].x, Model->Obj[i][x].Vector[y].y, Model->Obj[i][x].Vector[y].z },
						vec3{ Model->Obj[i][x].Normal[y].x, Model->Obj[i][x].Normal[y].y, Model->Obj[i][x].Normal[y].z },
						Model->Obj[i][x].Color[y],
						vec2{ Model->Obj[i][x].UV[y].x, Model->Obj[i][x].UV[y].y }
					}
				);
			}

			if (Shape.size()) { Temp->Object[i][x].Data.reset(Render->CreateVec3cnt(Shape)); }
		}
	}

	return Temp;
}
#endif

bool Resident_Evil_Model::OpenObject(std::filesystem::path Path, std::uintmax_t _Ptr, bool b_ReplaceModel)
{
	if (b_ReplaceModel) { if (m_Model->IsOpen()) { m_Model->Close(); } }
	else if (m_Model->IsOpen()) { Close(); }

	String Extension = Path.extension().string();

	if (Standard_String().ToUpper(Extension) == ".TMD")
	{
		m_Model->Open(Path, _Ptr);
	}
	if (Standard_String().ToUpper(Extension) == ".MD1")
	{
		m_Model = Resident_Evil_2_MD1(Str.hWnd, Path, _Ptr).GetTMD();
	}
	if (Standard_String().ToUpper(Extension) == ".MD2")
	{
		m_Model = Resident_Evil_3_MD2(Str.hWnd, Path, _Ptr).GetTMD();
	}

	return m_Model->IsOpen();
}

bool Resident_Evil_Model::OpenPlayer(std::filesystem::path Path, std::uintmax_t _Ptr)
{
	StdFile m_File{ Path, FileAccessMode::Read, true, false };
	if (!m_File.IsOpen())
	{
		Str.Message(L"Resident Evil Model Error: could not read player data at 0x%llX in \"%ws\"", _Ptr, m_File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (m_Model->IsOpen()) { Close(); }

	std::vector<std::uint32_t> Pointer = GetDataPtr(m_File, _Ptr);

	for (std::size_t i = 0; i < Pointer.size(); ++i) { Pointer[i] += (std::uint32_t)_Ptr; }

	if (GameType() & (AUG95 | OCT95))
	{
		if (Pointer[1]) { Animation(NORMAL)->OpenEDD(m_File, Pointer[1]); }
		if (Pointer[1]) { Animation(NORMAL)->OpenEMR(m_File, Pointer[0]); }
		if (Pointer[2]) { m_Model->Open(m_File, Pointer[2]); }
		if (Pointer[3]) { m_Texture->OpenTIM(m_File, Pointer[3]); }
		if (Animation(NORMAL)->IsOpen()) { Animation(NORMAL)->CreateSkeleton(); }
	}
	else if (GameType() & BIO1)
	{
		if (Pointer[3]) { Animation(NORMAL)->OpenEDD(m_File, Pointer[3]); }
		if (Pointer[3]) { Animation(NORMAL)->OpenEMR(m_File, Pointer[2]); }
		if (Pointer[4]) { m_Model->Open(m_File, Pointer[4]); }
		if (Pointer[5]) { m_Texture->OpenTIM(m_File, Pointer[5]); }
		if (Animation(NORMAL)->IsOpen()) { Animation(NORMAL)->CreateSkeleton(); }
	}
	else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2))
	{
		if (Pointer.size() != 4)
		{
			Str.Message(L"Resident Evil Model Error: \"%ws\" is not player data\r\n\r\nAborting...", m_File.GetPath().filename().wstring().c_str());
			return false;
		}
		if (Pointer[0]) { Animation(NORMAL)->OpenEDD(m_File, Pointer[0]); }
		if (Pointer[1]) { Animation(NORMAL)->OpenEMR(m_File, Pointer[1]); }
		if (Pointer[2])
		{
			std::unique_ptr<Resident_Evil_2_MD1> Model = std::make_unique<Resident_Evil_2_MD1>(Str.hWnd, m_File, Pointer[2]);
			m_Model = Model->GetTMD();
		}
		if (Pointer[3]) { m_Texture->OpenTIM(m_File, Pointer[3]); }
		if (Animation(NORMAL)->IsOpen()) { Animation(NORMAL)->CreateSkeleton(); }
	}
	else if (GameType() & BIO3)
	{
		if (Pointer.size() != 5)
		{
			Str.Message(L"Resident Evil Model Error: \"%ws\" is not player data\r\n\r\nAborting...", m_File.GetPath().filename().wstring().c_str());
			return false;
		}
		if (Pointer[0]) { Animation(NORMAL)->OpenEDD(m_File, Pointer[0]); }
		if (Pointer[1]) { Animation(NORMAL)->OpenEMR(m_File, Pointer[1]); }
		if (Pointer[2])
		{
			std::unique_ptr<Resident_Evil_3_MD2> Model = std::make_unique<Resident_Evil_3_MD2>(Str.hWnd, m_File, Pointer[2]);
			m_Model = Model->GetTMD();
		}
		if (Pointer[3])
		{
			m_Binary00.resize((std::size_t)(Pointer[4] - Pointer[3]));
			m_File.Read(Pointer[3], m_Binary00.data(), m_Binary00.size());
		}
		if (Pointer[4]) { m_Texture->OpenTIM(m_File, Pointer[4]); }
		if (Animation(NORMAL)->IsOpen()) { Animation(NORMAL)->CreateSkeleton(); }
	}

#if MSTD_DX9
	std::unique_ptr<FIXED_MODEL> Temp = m_Model->Export(m_Texture->GetWidth(), m_Texture->GetHeight());
	m_DX9Model = ExportDX9(Temp, m_Texture);
#endif

	return true;
}

bool Resident_Evil_Model::OpenEnemy(std::filesystem::path Path, std::uintmax_t _Ptr)
{
	StdFile m_File{ Path, FileAccessMode::Read, true, false };
	if (!m_File.IsOpen())
	{
		Str.Message(L"Resident Evil Model Error: could not read enemy data at 0x%llX in \"%ws\"", _Ptr, m_File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (m_Model->IsOpen()) { Close(); }

	std::vector<std::uint32_t> Pointer = GetDataPtr(m_File, _Ptr);

	for (std::size_t i = 0; i < Pointer.size(); ++i) { Pointer[i] += (std::uint32_t)_Ptr; }

	if (GameType() & (AUG95 | OCT95))
	{
		if (Pointer[1]) { Animation(NORMAL)->OpenEDD(m_File, Pointer[1]); }
		if (Pointer[1]) { Animation(NORMAL)->OpenEMR(m_File, Pointer[0]); }
		if (Pointer[2]) { m_Model->Open(m_File, Pointer[2]); }
		if (Pointer[3]) { m_Texture->OpenTIM(m_File, Pointer[3]); }
		if (Animation(NORMAL)->IsOpen()) { Animation(NORMAL)->CreateSkeleton(); }
	}
	else if (GameType() & BIO1)
	{
		if (Pointer[1]) { Animation(NORMAL)->OpenEDD(m_File, Pointer[1]); }
		if (Pointer[1]) { Animation(NORMAL)->OpenEMR(m_File, Pointer[0]); }
		if (Pointer[3]) { Animation(DAMAGE)->OpenEDD(m_File, Pointer[3]); }
		if (Pointer[3]) { Animation(DAMAGE)->OpenEMR(m_File, Pointer[2]); }
		if (Pointer[4]) { m_Model->Open(m_File, Pointer[4]); }
		if (Pointer[5]) { m_Texture->OpenTIM(m_File, Pointer[5]); }
		if (Animation(NORMAL)->IsOpen()) { Animation(NORMAL)->CreateSkeleton(); }
	}
	else if (GameType() & BIO2NOV96)
	{
		if (Pointer.size() != 9)
		{
			Str.Message(L"Resident Evil Model Error: \"%ws\" is not enemy data\r\n\r\nAborting...", m_File.GetPath().filename().wstring().c_str());
			return false;
		}
		if (Pointer[0])
		{
			m_Binary00.resize((std::size_t)(Pointer[1] - Pointer[0]));
			m_File.Read(Pointer[0], m_Binary00.data(), m_Binary00.size());
		}
		if (Pointer[1]) { Animation(NORMAL)->OpenEDD(m_File, Pointer[1]); }
		if (Pointer[2]) { Animation(NORMAL)->OpenEMR(m_File, Pointer[2]); }
		if (Pointer[3]) { Animation(NORMAL_EX0)->OpenEDD(m_File, Pointer[3]); }
		if (Pointer[4]) { Animation(NORMAL_EX0)->OpenEMR(m_File, Pointer[4]); }
		if (Pointer[5]) { Animation(DAMAGE)->OpenEDD(m_File, Pointer[5]); }
		if (Pointer[6]) { Animation(DAMAGE)->OpenEMR(m_File, Pointer[6]); }
		if (Pointer[7])
		{
			std::unique_ptr<Resident_Evil_2_MD1> Model = std::make_unique<Resident_Evil_2_MD1>(Str.hWnd, m_File, Pointer[7]);
			m_Model = Model->GetTMD();
		}
		if (Pointer[8]) { m_Texture->OpenTIM(m_File, Pointer[8]); }
		if (Animation(NORMAL)->IsOpen()) { Animation(NORMAL)->CreateSkeleton(); }
		Animation(NORMAL_EX0)->Skeleton = Animation(NORMAL)->Skeleton;
	}
	else if (GameType() & (BIO2TRIAL | BIO2))
	{
		if (Pointer.size() != 8)
		{
			Str.Message(L"Resident Evil Model Error: \"%ws\" is not enemy data\r\n\r\nAborting...", m_File.GetPath().filename().wstring().c_str());
			return false;
		}
		if (Pointer[0])
		{
			m_Binary00.resize((std::size_t)(Pointer[1] - Pointer[0]));
			m_File.Read(Pointer[0], m_Binary00.data(), m_Binary00.size());
		}
		if (Pointer[1]) { Animation(NORMAL)->OpenEDD(m_File, Pointer[1]); }
		if (Pointer[2]) { Animation(NORMAL)->OpenEMR(m_File, Pointer[2]); }
		if (Pointer[3]) { Animation(NORMAL_EX0)->OpenEDD(m_File, Pointer[3]); }
		if (Pointer[4]) { Animation(NORMAL_EX0)->OpenEMR(m_File, Pointer[4]); }
		if (Pointer[5]) { Animation(DAMAGE)->OpenEDD(m_File, Pointer[5]); }
		if (Pointer[6]) { Animation(DAMAGE)->OpenEMR(m_File, Pointer[6]); }
		if (Pointer[7])
		{
			std::unique_ptr<Resident_Evil_2_MD1> Model = std::make_unique<Resident_Evil_2_MD1>(Str.hWnd, m_File, Pointer[7]);
			m_Model = Model->GetTMD();
		}
		if (Animation(NORMAL)->IsOpen()) { Animation(NORMAL)->CreateSkeleton(); }
		Animation(NORMAL_EX0)->Skeleton = Animation(NORMAL)->Skeleton;
	}
	else if (GameType() & BIO3)
	{
		if (Pointer.size() != 15)
		{
			Str.Message(L"Resident Evil Model Error: \"%ws\" is not enemy data\r\n\r\nAborting...", m_File.GetPath().filename().wstring().c_str());
			return false;
		}
		if (Pointer[0])
		{
			m_Binary00.resize((std::size_t)(Pointer[1] - Pointer[0]));
			m_File.Read(Pointer[0], m_Binary00.data(), m_Binary00.size());
		}
		if (Pointer[1])
		{
			m_Binary01.resize((std::size_t)(Pointer[2] - Pointer[1]));
			m_File.Read(Pointer[1], m_Binary01.data(), m_Binary01.size());
		}
		if (Pointer[2]) { Animation(NORMAL)->OpenEDD(m_File, Pointer[2]); }
		if (Pointer[3]) { Animation(NORMAL)->OpenEMR(m_File, Pointer[3]); }
		if (Pointer[4]) { Animation(NORMAL_EX1)->OpenEDD(m_File, Pointer[4]); }
		if (Pointer[5]) { Animation(NORMAL_EX1)->OpenEMR(m_File, Pointer[5]); }
		if (Pointer[6]) { Animation(DAMAGE)->OpenEDD(m_File, Pointer[6]); }
		if (Pointer[7]) { Animation(DAMAGE)->OpenEMR(m_File, Pointer[7]); }
		if (Pointer[8])
		{
			m_WeaponBinary.resize((std::size_t)(Pointer[9] - Pointer[8]));
			m_File.Read(Pointer[0], m_WeaponBinary.data(), m_WeaponBinary.size());
		}
		if (Pointer[9]) { Animation(WEAPON_EX0)->OpenEDD(m_File, Pointer[9]); }
		if (Pointer[10]) { Animation(WEAPON_EX0)->OpenEMR(m_File, Pointer[10]); }
		if (Pointer[11]) { Animation(WEAPON_EX1)->OpenEDD(m_File, Pointer[11]); }
		if (Pointer[12]) { Animation(WEAPON_EX1)->OpenEMR(m_File, Pointer[12]); }
		if (Pointer[13])
		{
			std::unique_ptr<Resident_Evil_3_MD2> Model = std::make_unique<Resident_Evil_3_MD2>(Str.hWnd, m_File, Pointer[13]);
			m_WeaponModel = Model->GetTMD();
		}
		if (Pointer[14])
		{
			std::unique_ptr<Resident_Evil_3_MD2> Model = std::make_unique<Resident_Evil_3_MD2>(Str.hWnd, m_File, Pointer[14]);
			m_Model = Model->GetTMD();
		}
		if (Animation(NORMAL)->IsOpen()) { Animation(NORMAL)->CreateSkeleton(); }
		if (Animation(NORMAL_EX1)->IsOpen()) { Animation(NORMAL_EX1)->CreateSkeleton(); }
		if (Animation(WEAPON_EX0)->IsOpen()) { Animation(WEAPON_EX0)->CreateSkeleton(); }
		if (Animation(WEAPON_EX1)->IsOpen()) { Animation(WEAPON_EX1)->CreateSkeleton(); }
	}

#if MSTD_DX9
	std::unique_ptr<FIXED_MODEL> Temp = m_Model->Export(m_Texture->GetWidth(), m_Texture->GetHeight());
	m_DX9Model = ExportDX9(Temp, m_Texture);
	if (GameType() & BIO3)
	{
		std::unique_ptr<FIXED_MODEL> Temp = m_WeaponModel->Export(m_Texture->GetWidth(), m_Texture->GetHeight());
		m_DX9WeaponModel = ExportDX9(Temp, m_Texture);
	}
#endif

	return true;
}

bool Resident_Evil_Model::OpenWeapon(std::filesystem::path Path, std::uintmax_t _Ptr)
{
	StdFile m_File{ Path, FileAccessMode::Read, true, false };
	if (!m_File.IsOpen())
	{
		Str.Message(L"Resident Evil Model Error: could not read player data at 0x%llX in \"%ws\"", _Ptr, m_File.GetPath().filename().wstring().c_str());
		return false;
	}

	if (m_WeaponModel->IsOpen()) { CloseWeapon(); }

	std::vector<std::uint32_t> Pointer = GetDataPtr(m_File, _Ptr);

	for (std::size_t i = 0; i < Pointer.size(); ++i) { Pointer[i] += (std::uint32_t)_Ptr; }

	if (GameType() & (AUG95 | OCT95 | BIO1))
	{
		if (Pointer[1]) { Animation(WEAPON)->OpenEDD(m_File, Pointer[1]); }
		if (Pointer[1]) { Animation(WEAPON)->OpenEMR(m_File, Pointer[0]); }
		if (Pointer[2]) { m_WeaponModel->Open(m_File, Pointer[2]); }
	}
	else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2))
	{
		if (Pointer.size() != 4)
		{
			Str.Message(L"Resident Evil Model Error: \"%ws\" is not weapon data\r\n\r\nAborting...", m_File.GetPath().filename().wstring().c_str());
			return false;
		}
		if (Pointer[0]) { Animation(WEAPON)->OpenEDD(m_File, Pointer[0]); }
		if (Pointer[1]) { Animation(WEAPON)->OpenEMR(m_File, Pointer[1]); }
		if (Pointer[2])
		{
			std::unique_ptr<Resident_Evil_2_MD1> Model = std::make_unique<Resident_Evil_2_MD1>(Str.hWnd, m_File, Pointer[2]);
			m_WeaponModel = Model->GetTMD();
		}
		if (Pointer[3]) { m_WeaponTexture->OpenTIM(m_File, Pointer[3]); }
		Animation(WEAPON)->Skeleton = Animation(NORMAL)->Skeleton;
	}
	else if (GameType() & BIO3)
	{
		if (Pointer.size() != 9)
		{
			Str.Message(L"Resident Evil Model Error: \"%ws\" is not weapon data\r\n\r\nAborting...", m_File.GetPath().filename().wstring().c_str());
			return false;
		}
		if (Pointer[0]) { Animation(WEAPON)->OpenEDD(m_File, Pointer[0]); }
		if (Pointer[1]) { Animation(WEAPON)->OpenEMR(m_File, Pointer[1]); }
		if (Pointer[2]) { Animation(WEAPON_EX0)->OpenEDD(m_File, Pointer[2]); }
		if (Pointer[3]) { Animation(WEAPON_EX0)->OpenEMR(m_File, Pointer[3]); }
		if (Pointer[4])
		{
			m_WeaponBinary.resize((std::size_t)(Pointer[5] - Pointer[4]));
			m_File.Read(Pointer[4], m_WeaponBinary.data(), m_WeaponBinary.size());
		}
		if (Pointer[5]) { Animation(WEAPON_EX1)->OpenEDD(m_File, Pointer[5]); }
		if (Pointer[6]) { Animation(WEAPON_EX1)->OpenEMR(m_File, Pointer[6]); }
		if (Pointer[7])
		{
			std::unique_ptr<Resident_Evil_3_MD2> Model = std::make_unique<Resident_Evil_3_MD2>(Str.hWnd, m_File, Pointer[7]);
			m_WeaponModel = Model->GetTMD();
		}
		if (Pointer[8]) { m_WeaponTexture->OpenTIM(m_File, Pointer[8]); }
		if (Animation(WEAPON_EX0)->IsOpen()) { Animation(WEAPON_EX0)->CreateSkeleton(); }
		if (Animation(WEAPON_EX1)->IsOpen()) { Animation(WEAPON_EX1)->CreateSkeleton(); }
	}

#if MSTD_DX9
	if (GameType() & (AUG95 | OCT95 | BIO1))
	{
		std::unique_ptr<FIXED_MODEL> Temp = m_WeaponModel->Export(m_Texture->GetWidth(), m_Texture->GetHeight());
		m_DX9WeaponModel = ExportDX9(Temp, m_Texture);
	}
	else
	{
		std::unique_ptr<FIXED_MODEL> Temp = m_WeaponModel->Export(m_WeaponTexture->GetWidth(), m_WeaponTexture->GetHeight());
		m_DX9WeaponModel = ExportDX9(Temp, m_WeaponTexture);
	}
#endif

	return true;
}

void Resident_Evil_Model::Close(void)
{
	if (m_Texture) m_Texture->Close();
	if (m_WeaponTexture) m_WeaponTexture->Close();
	if (m_Model) m_Model->Close();
	if (m_WeaponModel) m_WeaponModel->Close();
	m_Binary00.clear();
	m_Binary01.clear();
	m_WeaponBinary.clear();
	for (auto& a : m_Animations) { a->Close(); }

#if MSTD_DX9
	m_DX9Model->Object.clear();
	m_DX9Model->Texture.clear();
	m_DX9WeaponModel->Object.clear();
	m_DX9WeaponModel->Texture.clear();
#endif
}

void Resident_Evil_Model::CloseWeapon(void)
{
	m_WeaponTexture->Close();
	m_WeaponModel->Close();
	m_WeaponBinary.clear();
	Animation(WEAPON)->Close();
	Animation(WEAPON_EX0)->Close();
	Animation(WEAPON_EX1)->Close();

#if MSTD_DX9
	m_DX9WeaponModel->Object.clear();
	m_DX9WeaponModel->Texture.clear();
#endif
}

void Resident_Evil_Model::DrawFrame(std::shared_ptr<Resident_Evil_Animation> Animation, size_t iClip, size_t iFrame)
{
	if (!Animation || !Animation->IsOpen() || Animation->Clip.empty()) { return; }

	iClip = std::clamp(iClip, (size_t)0, Animation->Clip.size() - 1);

	if (Animation->Clip[iClip].empty()) { return; }

	iFrame = std::clamp(iFrame, (size_t)0, Animation->Clip[iClip].size() - 1);

	const auto& Frame = Animation->Clip[iClip][iFrame];

	size_t iNextFrame = std::clamp(iFrame + 1, (size_t)0, Animation->Clip[iClip].size() - 1);
	if (iFrame >= Animation->GetFrameCount(iClip)) { iNextFrame = 0; }

	const auto& NextFrame = Animation->Clip[iClip][min(iFrame + 1, Animation->Clip[iClip].size() - 1)];

	float t = 0.5f;

	std::function<void(const std::shared_ptr<Resident_Evil_Animation::Bone>&, const Standard_Matrix&)> DrawKeyframe =
		[&](const std::shared_ptr<Resident_Evil_Animation::Bone>& Skeleton, const Standard_Matrix& Matrix)
		{
			Standard_Matrix Local;

			if (b_DrawRootOnly)
			{
				Local = Matrix * Skeleton->World;
			}
			else
			{
				Standard_Matrix Transform;
				if (Skeleton->ID < Frame.Rotation.size())
				{
					const auto& Rotation = Frame.Rotation[Skeleton->ID];
					Standard_Matrix RotX = Standard_Matrix().RotateX(World->Radian(GTE->ToFloat(Rotation.x) * 360.0f));
					Standard_Matrix RotY = Standard_Matrix().RotateY(World->Radian(GTE->ToFloat(Rotation.y) * 360.0f));
					Standard_Matrix RotZ = Standard_Matrix().RotateZ(World->Radian(GTE->ToFloat(Rotation.z) * 360.0f));
					Transform = RotX * RotY * RotZ;
				}

				if (Skeleton->Parent.lock())
				{
					Skeleton->Local = Skeleton->World * Transform;
				}
				else
				{
					//const auto& Origin = Frame.Origin;
					//const auto& Speed = Frame.Speed;
					//float OX = GTE->ToFloat(Origin.x) + GTE->ToFloat(Speed.x);
					//float OY = GTE->ToFloat(Origin.y) + GTE->ToFloat(Speed.y);
					//float OZ = GTE->ToFloat(Origin.z) + GTE->ToFloat(Speed.z);
					//Standard_Matrix T = Standard_Matrix().Translate(vec3{ OX, OY, OZ });

					const auto& Origin = Frame.Origin;
					const auto& Speed = Frame.Speed;
					const auto& NextOrigin = NextFrame.Origin;
					const auto& NextSpeed = NextFrame.Speed;
					vec3 Position =
					{
						std::lerp(GTE->ToFloat(Origin.x) + GTE->ToFloat(Speed.x), GTE->ToFloat(NextOrigin.x) + GTE->ToFloat(NextSpeed.x), t),
						std::lerp(GTE->ToFloat(Origin.y) + GTE->ToFloat(Speed.y), GTE->ToFloat(NextOrigin.y) + GTE->ToFloat(NextSpeed.y), t),
						std::lerp(GTE->ToFloat(Origin.z) + GTE->ToFloat(Speed.z), GTE->ToFloat(NextOrigin.z) + GTE->ToFloat(NextSpeed.z), t)
					};
					Standard_Matrix T = Standard_Matrix().Translate(Position);

					Skeleton->Local = T * Transform;
				}

				Local = Matrix * Skeleton->Local;
			}

#if MSTD_DX9
			Render->SetWorld(Local);

			if (Skeleton->ID < ModelDX9()->Object.size())
			{
				size_t i = Skeleton->ID;
				for (size_t x = 0; x < ModelDX9()->Object[i].size(); x++)
				{
					size_t Texture = std::clamp(ModelDX9()->Object[i][x].iTexture, (size_t)0, ModelDX9()->Texture.empty() ? 0 : ModelDX9()->Texture.size() - 1);
					Render->DrawVec3cnt(
						ModelDX9()->Object[i][x].Data.get(),
						nullptr,
						ModelDX9()->Texture.empty() ? nullptr : ModelDX9()->Texture[Texture].get(),
						Render->PS1DitherPixelShader.get(),
						ModelDX9()->TextureWidth, ModelDX9()->TextureHeight,
						D3DFILL_SOLID, D3DPT_TRIANGLELIST
					);
				}
			}
#endif

			for (const auto& Children : Skeleton->Children) { DrawKeyframe(Children, Local); }
		};

	SetWorld({ Position().x, Position().y, Position().z, Rotation().x, Rotation().y, Rotation().z });

#if MSTD_DX9
	if (!Render || !Render->NormalState()) { return; }

	if (!ModelDX9() || ModelDX9()->Object.empty()) { return; }

	DWORD CullMode = 0;
	DWORD Clipping = 0;

	Render->SetWorld(World);

	Render->Device()->GetRenderState(D3DRS_CULLMODE, &CullMode);
	Render->Device()->GetRenderState(D3DRS_CLIPPING, &Clipping);

	Render->Device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	Render->Device()->SetRenderState(D3DRS_CLIPPING, TRUE);
#endif

	DrawKeyframe(Animation->Skeleton, *World);

#if MSTD_DX9
	Render->Device()->SetRenderState(D3DRS_CULLMODE, CullMode);
	Render->Device()->SetRenderState(D3DRS_CLIPPING, Clipping);

	Render->ResetWorld();
#endif
}

void Resident_Evil_Model::DrawObject(std::size_t iObject, bool b_DrawAll)
{
#if MSTD_DX9
	if (!Render || !Render->NormalState()) { return; }

	if (!ModelDX9() || ModelDX9()->Object.empty()) { return; }

	SetWorld({ Position().x, Position().y, Position().z, Rotation().x, Rotation().y, Rotation().z });

	std::size_t nObjects = iObject + 1;

	if (b_DrawAll)
	{
		iObject = 0;
		nObjects = ModelDX9()->Object.size();
	}

	DWORD CullMode = 0;
	DWORD Clipping = 0;

	Render->Device()->GetRenderState(D3DRS_CULLMODE, &CullMode);
	Render->Device()->GetRenderState(D3DRS_CLIPPING, &Clipping);

	Render->Device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	Render->Device()->SetRenderState(D3DRS_CLIPPING, TRUE);

	Render->SetWorld(World);

	for (size_t i = iObject; i < nObjects; i++)
	{
		for (size_t x = 0; x < ModelDX9()->Object[i].size(); x++)
		{
			size_t Texture = std::clamp(ModelDX9()->Object[i][x].iTexture, (size_t)0, ModelDX9()->Texture.empty() ? 0 : ModelDX9()->Texture.size() - 1);
			Render->DrawVec3cnt(
				ModelDX9()->Object[i][x].Data.get(),
				nullptr,
				ModelDX9()->Texture.empty() ? nullptr : ModelDX9()->Texture[Texture].get(),
				Render->PS1DitherPixelShader.get(),
				ModelDX9()->TextureWidth, ModelDX9()->TextureHeight,
				D3DFILL_SOLID, D3DPT_TRIANGLELIST
			);
		}
	}

	Render->Device()->SetRenderState(D3DRS_CULLMODE, CullMode);
	Render->Device()->SetRenderState(D3DRS_CLIPPING, Clipping);

	Render->ResetWorld();
#endif
}

void Resident_Evil_Model::Draw(void)
{
	if (!Animation(NORMAL)->IsOpen())
	{
		DrawObject(0, true);
	}
	else
	{
		static std::size_t frameCounter = 0;

		if (iClip >= Animation(NORMAL)->GetClipCount()) { iClip = 0; }
		if (iFrame >= Animation(NORMAL)->GetFrameCount(iClip)) { iFrame = 0; }
		DrawFrame(Animation(NORMAL), iClip, iFrame);

		const auto& Frame = Animation(NORMAL)->Clip[iClip][iFrame];

		static std::size_t Skip = Frame.Attr.Speed;
		Skip << 1;

		if (frameCounter % 2 == 0) { iFrame++; }
		frameCounter++;
		if (frameCounter >= Animation(NORMAL)->GetFrameCount(iClip)) { frameCounter = 0; }
	}
}