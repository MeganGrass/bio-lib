/*
*
*	Megan Grass
*	July 13, 2025
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

void Resident_Evil_Model::ParseBinary(const std::vector<std::uint8_t>& Binary, size_t JointCount, BINARY_DATA& Output)
{
	if (Binary.empty() || Binary.size() == 0x04 || !JointCount) { return; }

	BINARY_HEADER Header{};
	std::memcpy(&Header, &Binary.data()[0x00], sizeof(BINARY_HEADER));

	std::vector<LIMB_INDEX> LimbIndex(Header.LimbCount);
	std::memcpy(&LimbIndex.data()[0x00], &Binary.data()[0x08], Header.LimbCount * sizeof(LIMB_INDEX));

	Output.BindPose.resize(JointCount);
	std::memcpy(&Output.BindPose.data()[0x00], &Binary.data()[Header.pBindPose], JointCount * sizeof(SVECTOR2));

	size_t Pointer = Output.BindPose.size() * sizeof(SVECTOR2) + sizeof(BINARY_HEADER) + LimbIndex.size() * sizeof(LIMB_INDEX);
	Standard_Basic_FStream().Align(Pointer, 4);

	auto ParseTri = [&](size_t Pointer, std::vector<std::array<uint8_t, 3>>& Output) -> size_t
		{
			uint8_t Value = 0;
			std::memcpy(&Value, &Binary.data()[Pointer], sizeof(Value));
			if (Value == 0xFF) { return Pointer + sizeof(Value); }

			while (Pointer <= Binary.size())
			{
				std::array<uint8_t, 3> Tri{};
				std::memcpy(&Tri, &Binary.data()[Pointer], sizeof(Tri));

				Output.push_back(Tri);

				if (Tri[2] == 0xFF) { Pointer += 3; break; }
				else if (Tri[1] == 0xFF) { Pointer += 2; break; }
				else if (Tri[0] == 0xFF) { Pointer += 1; break; }

				Pointer += sizeof(Tri);
			}

			return Pointer;
		};

	auto ParseVertexBind = [&](size_t Pointer, std::array<int8_t, 256>& OutputA, std::array<int8_t, 256>& OutputB) -> size_t
		{
			uint8_t Value = 0;
			std::memcpy(&Value, &Binary.data()[Pointer], sizeof(Value));
			if (Value == 0xFF) { return Pointer + sizeof(Value); }

			int8_t JointId = -1;
			bool UseOutputB = false;

			while (Pointer <= Binary.size())
			{
				std::memcpy(&Value, &Binary.data()[Pointer], sizeof(Value));
				Pointer += sizeof(Value);

				if (Value == 0xFF) { break; }
				if (Value == 0x00) { JointId = -1; continue; }
				if (Value == 0x60) { UseOutputB = false; continue; }
				if (Value == 0x61) { UseOutputB = true;  continue; }

				if (JointId == -1) { JointId = static_cast<int8_t>(Value); continue; }

				if (Value < 256)
				{
					if (UseOutputB) { OutputB[Value] = JointId; }
					else { OutputA[Value] = JointId; }
				}
			}

			return Pointer;
		};

	Output.Limb.resize(Header.LimbCount);
	for (size_t i = 0; i < Output.Limb.size(); ++i)
	{
		Pointer = ParseTri(Pointer, Output.Limb[i].TriA);
		Pointer = ParseTri(Pointer, Output.Limb[i].TriB);
		Pointer = ParseVertexBind(Pointer, Output.Limb[i].JointA, Output.Limb[i].JointB);
	}
}

SHAPEVECTOR Resident_Evil_Model::HitboxShape(void) noexcept
{
	if (b_EditorMode)
	{
		int32_t Height = -(std::abs(EditorPosition().y) + std::abs(Hitbox().h));
		return { EditorPosition().x - Hitbox().w, EditorPosition().y, EditorPosition().z - Hitbox().d, Hitbox().w * 2, Height, Hitbox().d * 2 };
	}
	else
	{
		int32_t Height = -(std::abs(Position().y) + std::abs(Hitbox().h));
		return { Position().x - Hitbox().w, Position().y, Position().z - Hitbox().d, Hitbox().w * 2, Height, Hitbox().d * 2 };
	}
}

void Resident_Evil_Model::SetWorld(const MATVECTOR& Vec) const
{
	float XX = GTE->ToFloat(Vec.x);
	float YY = GTE->ToFloat(Vec.y);
	float ZZ = GTE->ToFloat(Vec.z);

	float RX = GTE->ToFloat(std::clamp(Vec.rx, -ONE, ONE)) * 360.0f;
	float RY = GTE->ToFloat(std::clamp(Vec.ry, -ONE, ONE)) * 360.0f;
	float RZ = GTE->ToFloat(std::clamp(Vec.rz, -ONE, ONE)) * 360.0f;

	float SX = GTE->ToFloat(Vec.sx);
	float SY = GTE->ToFloat(Vec.sy);
	float SZ = GTE->ToFloat(Vec.sz);

	Standard_Matrix S = Standard_Matrix().Scale(vec3{ SX, SY, SZ });
	Standard_Matrix R = Standard_Matrix().YawPitchRoll(vec3{ World->Radian(RX), World->Radian(RY), World->Radian(RZ) });
	Standard_Matrix T = Standard_Matrix().Translate(vec3{ XX, YY, ZZ });

	*World = T * R * S;
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

		if (Temp->TextureWidth && Temp->TextureHeight)
		{
			if (Texture->GetPaletteCount())
			{
				Temp->Texture.resize(Texture->GetPaletteCount());
				for (uint16_t i = 0; i < Texture->GetPaletteCount(); i++)
				{
					Temp->Texture[i].reset(Render->CreateTexture(Texture, i, Sony_Texture_Transparency::Superblack, 0xFF00FF, true));
				}
			}
			else
			{
				Temp->Texture.resize(1);
				Temp->Texture[0].reset(Render->CreateTexture(Texture, 0, Sony_Texture_Transparency::Superblack, 0xFF00FF, true));
			}
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

bool Resident_Evil_Model::Open(std::filesystem::path Path, std::uintmax_t _Ptr, bool b_Bio1Enemy)
{
	bool b_TerminateDraw = b_Active.load();

	StopDrawing();

	bool b_Open = false;

	String Extension = Path.extension().string();

	if (Standard_String().ToUpper(Extension) == ".TMD" || Standard_String().ToUpper(Extension) == ".MD1" || Standard_String().ToUpper(Extension) == ".MD2")
	{
		b_Open = OpenObject(Path, _Ptr);
	}

	if (Standard_String().ToUpper(Extension) == ".PLD")
	{
		b_Open = OpenPlayer(Path, _Ptr);
	}

	if (Standard_String().ToUpper(Extension) == ".EMD")
	{
		if (GameType() & (AUG95 | OCT95 | BIO1))
		{
			b_Bio1Enemy ? b_Open = OpenEnemy(Path, _Ptr) : b_Open = OpenPlayer(Path, _Ptr);
		}
		else
		{
			b_Open = OpenEnemy(Path, _Ptr);
		}
	}

	if (Standard_String().ToUpper(Extension) == ".EMW" || Standard_String().ToUpper(Extension) == ".PLW")
	{
		b_Open = OpenWeapon(Path, _Ptr);
	}

	b_Active.store(b_TerminateDraw);

	return b_Open;
}

bool Resident_Evil_Model::OpenObject(std::filesystem::path Path, std::uintmax_t _Ptr)
{
	if (m_Model) { m_Model->Close(); }

	m_Filename = Path;

	String Extension = Path.extension().string();

	if (Standard_String().ToUpper(Extension) == ".TMD") { m_Model->Open(Path, _Ptr); }
	if (Standard_String().ToUpper(Extension) == ".MD1") { m_Model = Resident_Evil_2_MD1(Str.hWnd, Path, _Ptr).GetTMD(); }
	if (Standard_String().ToUpper(Extension) == ".MD2") { m_Model = Resident_Evil_3_MD2(Str.hWnd, Path, _Ptr).GetTMD(); }

	if (!m_Texture->IsOpen())
	{
		Standard_FileSystem().SetFileExtension(Path, L".tim");
		if (Standard_FileSystem().Exists(Path)) { m_Texture->OpenTIM(Path); }
	}

#if MSTD_DX9
	if (m_Texture->IsOpen())
	{
		CloseModelDX9();

		std::unique_ptr<FIXED_MODEL> Temp = m_Model->Export(m_Texture->GetWidth(), m_Texture->GetHeight(), true);
		m_DX9Model = ExportDX9(Temp, m_Texture);
	}
	else
	{
		CloseModelDX9();

		std::unique_ptr<Sony_PlayStation_Texture> DummyTexture = std::make_unique<Sony_PlayStation_Texture>();

		std::unique_ptr<FIXED_MODEL> Temp = m_Model->Export(0, 0);
		m_DX9Model = ExportDX9(Temp, DummyTexture);
	}
#endif

	iObjectMax = m_Model->ObjectCount() ? m_Model->ObjectCount() - 1 : 0;

	if (m_ModelType == ModelType::None)
	{
		m_ModelType = ModelType::Object;
		b_DrawSingleObject = true;
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

	Close();

	m_Filename = Path;

	std::vector<std::uint32_t> Pointer = GetDataPtr(m_File, _Ptr);

	for (std::size_t i = 0; i < Pointer.size(); ++i) { Pointer[i] += (std::uint32_t)_Ptr; }

	if (GameType() & (AUG95 | OCT95))
	{
		if (Pointer[1]) { Animation(NORMAL)->OpenEDD(m_File, Pointer[1]); }
		if (Pointer[1]) { if (Animation(NORMAL)->IsEDDOpen()) Animation(NORMAL)->OpenEMR(m_File, Pointer[0]); }
		if (Pointer[2]) { m_Model->Open(m_File, Pointer[2]); }
		if (Pointer[3]) { m_Texture->OpenTIM(m_File, Pointer[3]); }
	}
	else if (GameType() & BIO1)
	{
		if (Pointer[3]) { Animation(NORMAL)->OpenEDD(m_File, Pointer[3]); }
		if (Pointer[3]) { if (Animation(NORMAL)->IsEDDOpen()) Animation(NORMAL)->OpenEMR(m_File, Pointer[2]); }
		if (Pointer[4]) { m_Model->Open(m_File, Pointer[4]); }
		if (Pointer[5]) { m_Texture->OpenTIM(m_File, Pointer[5]); }
	}
	else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2))
	{
		if (Pointer.size() != 4)
		{
			Str.Message(L"Resident Evil Model Error: \"%ws\" is not player data\r\n\r\nAborting...", m_File.GetPath().filename().wstring().c_str());
			return false;
		}
		if (Pointer[0]) { Animation(NORMAL)->OpenEDD(m_File, Pointer[0]); }
		if (Pointer[1]) { if (Animation(NORMAL)->IsEDDOpen()) Animation(NORMAL)->OpenEMR(m_File, Pointer[1]); }
		if (Pointer[2]) { m_Model = Resident_Evil_2_MD1(Str.hWnd, m_File, Pointer[2]).GetTMD(); }
		if (Pointer[3]) { m_Texture->OpenTIM(m_File, Pointer[3]); }
	}
	else if (GameType() & BIO3)
	{
		if (Pointer.size() != 5)
		{
			Str.Message(L"Resident Evil Model Error: \"%ws\" is not player data\r\n\r\nAborting...", m_File.GetPath().filename().wstring().c_str());
			return false;
		}
		if (Pointer[0]) { Animation(NORMAL)->OpenEDD(m_File, Pointer[0]); }
		if (Pointer[1]) { if (Animation(NORMAL)->IsEDDOpen()) Animation(NORMAL)->OpenEMR(m_File, Pointer[1]); }
		if (Pointer[2]) { m_Model = Resident_Evil_3_MD2(Str.hWnd, m_File, Pointer[2]).GetTMD(); }
		if (Pointer[3])
		{
			m_Binary00.resize((std::size_t)(Pointer[4] - Pointer[3]));
			m_File.Read(Pointer[3], m_Binary00.data(), m_Binary00.size());
		}
		if (Pointer[4]) { m_Texture->OpenTIM(m_File, Pointer[4]); }

		if (!m_Binary00.empty() && m_Binary00.size() > 0x04)
		{
			BINARY_DATA Test{};
			ParseBinary(m_Binary00, Animation(NORMAL)->Joints.size(), Test);
		}
	}

	if (Animation(NORMAL)->IsOpen()) { Animation(NORMAL)->CreateSkeleton(); }

#if MSTD_DX9
	std::unique_ptr<FIXED_MODEL> Temp = m_Model->Export(m_Texture->GetWidth(), m_Texture->GetHeight(), true);
	m_DX9Model = ExportDX9(Temp, m_Texture);
#endif

	if (GameType() & BIO1 || GameType() & AUG95 || GameType() & OCT95)
	{
		SetShadow(0, 81, 200, 26, 30);
	}
	else if (GameType() & BIO2NOV96)
	{
		SetShadow(1, 171, 224, 28, 32);
	}
	else if (GameType() & BIO2 || GameType() & BIO3)
	{
		SetShadow(2, 354, 224, 30, 32);
	}

	iObjectMax = m_Model->ObjectCount() ? m_Model->ObjectCount() - 1 : 0;

	m_ModelType = ModelType::Player;
	m_ModelGame = Game;

	b_DrawAllObjects = false;
	b_DrawSingleObject = false;

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

	Close();

	m_Filename = Path;

	std::vector<std::uint32_t> Pointer = GetDataPtr(m_File, _Ptr);

	for (std::size_t i = 0; i < Pointer.size(); ++i) { Pointer[i] += (std::uint32_t)_Ptr; }

	if (GameType() & (AUG95 | OCT95))
	{
		if (Pointer[1]) { Animation(NORMAL)->OpenEDD(m_File, Pointer[1]); }
		if (Pointer[1]) { if (Animation(NORMAL)->IsEDDOpen()) Animation(NORMAL)->OpenEMR(m_File, Pointer[0]); }
		if (Pointer[2]) { m_Model->Open(m_File, Pointer[2]); }
		if (Pointer[3]) { m_Texture->OpenTIM(m_File, Pointer[3]); }
	}
	else if (GameType() & BIO1)
	{
		if (Pointer[1]) { Animation(NORMAL)->OpenEDD(m_File, Pointer[1]); }
		if (Pointer[1]) { if (Animation(NORMAL)->IsEDDOpen()) Animation(NORMAL)->OpenEMR(m_File, Pointer[0]); }
		if (Pointer[3]) { Animation(DAMAGE)->OpenEDD(m_File, Pointer[3]); }
		if (Pointer[3]) { if (Animation(DAMAGE)->IsEDDOpen()) Animation(DAMAGE)->OpenEMR(m_File, Pointer[2]); }
		if (Pointer[4]) { m_Model->Open(m_File, Pointer[4]); }
		if (Pointer[5]) { m_Texture->OpenTIM(m_File, Pointer[5]); }
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
		if (Pointer[2]) { if (Animation(NORMAL)->IsEDDOpen()) Animation(NORMAL)->OpenEMR(m_File, Pointer[2]); }
		if (Pointer[3]) { Animation(NORMAL_EX0)->OpenEDD(m_File, Pointer[3]); }
		if (Pointer[4]) { if (Animation(NORMAL_EX0)->IsEDDOpen()) Animation(NORMAL_EX0)->OpenEMR(m_File, Pointer[4]); }
		if (Pointer[5]) { Animation(DAMAGE)->OpenEDD(m_File, Pointer[5]); }
		if (Pointer[6]) { if (Animation(DAMAGE)->IsEDDOpen()) Animation(DAMAGE)->OpenEMR(m_File, Pointer[6]); }
		if (Pointer[7]) { m_Model = Resident_Evil_2_MD1(Str.hWnd, m_File, Pointer[7]).GetTMD(); }
		if (Pointer[8]) { m_Texture->OpenTIM(m_File, Pointer[8]); }
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
		if (Pointer[2]) { if (Animation(NORMAL)->IsEDDOpen()) Animation(NORMAL)->OpenEMR(m_File, Pointer[2]); }
		if (Pointer[3]) { Animation(NORMAL_EX0)->OpenEDD(m_File, Pointer[3]); }
		if (Pointer[4]) { if (Animation(NORMAL_EX0)->IsEDDOpen()) Animation(NORMAL_EX0)->OpenEMR(m_File, Pointer[4]); }
		if (Pointer[5]) { Animation(DAMAGE)->OpenEDD(m_File, Pointer[5]); }
		if (Pointer[6]) { if (Animation(DAMAGE)->IsEDDOpen()) Animation(DAMAGE)->OpenEMR(m_File, Pointer[6]); }
		if (Pointer[7]) { m_Model = Resident_Evil_2_MD1(Str.hWnd, m_File, Pointer[7]).GetTMD(); }
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
		if (Pointer[3]) { if (Animation(NORMAL)->IsEDDOpen()) Animation(NORMAL)->OpenEMR(m_File, Pointer[3]); }
		if (Pointer[4]) { Animation(NORMAL_EX1)->OpenEDD(m_File, Pointer[4]); }
		if (Pointer[5]) { if (Animation(NORMAL_EX1)->IsEDDOpen()) Animation(NORMAL_EX1)->OpenEMR(m_File, Pointer[5]); }
		if (Pointer[6]) { Animation(DAMAGE)->OpenEDD(m_File, Pointer[6]); }
		if (Pointer[7]) { if (Animation(DAMAGE)->IsEDDOpen()) Animation(DAMAGE)->OpenEMR(m_File, Pointer[7]); }
		if (Pointer[8])
		{
			m_WeaponBinary.resize((std::size_t)(Pointer[9] - Pointer[8]));
			m_File.Read(Pointer[0], m_WeaponBinary.data(), m_WeaponBinary.size());
		}
		if (Pointer[9]) { Animation(WEAPON_EX0)->OpenEDD(m_File, Pointer[9]); }
		if (Pointer[10]) { if (Animation(WEAPON_EX0)->IsEDDOpen()) Animation(WEAPON_EX0)->OpenEMR(m_File, Pointer[10]); }
		if (Pointer[11]) { Animation(WEAPON_EX1)->OpenEDD(m_File, Pointer[11]); }
		if (Pointer[12]) { if (Animation(WEAPON_EX1)->IsEDDOpen()) Animation(WEAPON_EX1)->OpenEMR(m_File, Pointer[12]); }
		if (Pointer[13]) { m_WeaponModel = Resident_Evil_3_MD2(Str.hWnd, m_File, Pointer[13]).GetTMD(); }
		if (Pointer[14]) { m_Model = Resident_Evil_3_MD2(Str.hWnd, m_File, Pointer[14]).GetTMD(); }
	}

	if (Animation(NORMAL)->IsOpen()) { Animation(NORMAL)->CreateSkeleton(); }
	if (Animation(NORMAL_EX0)->IsOpen()) { Animation(NORMAL_EX0)->Skeleton = Animation(NORMAL)->Skeleton->Clone(); }
	if (Animation(NORMAL_EX1)->IsOpen()) { Animation(NORMAL_EX1)->Skeleton = Animation(NORMAL)->Skeleton->Clone(); }
	if (Animation(WEAPON_EX0)->IsOpen()) { Animation(WEAPON_EX0)->CreateSkeleton(); }
	if (Animation(WEAPON_EX1)->IsOpen()) { Animation(WEAPON_EX1)->CreateSkeleton(); }

	if (GameType() & (BIO2TRIAL | BIO2 | BIO3))
	{
		Standard_FileSystem().SetFileExtension(Path, L".tim");
		m_Texture->OpenTIM(Path);
	}

#if MSTD_DX9
	std::unique_ptr<FIXED_MODEL> Temp = m_Model->Export(m_Texture->GetWidth(), m_Texture->GetHeight(), true);
	m_DX9Model = ExportDX9(Temp, m_Texture);

	if (GameType() & BIO3)
	{
		std::unique_ptr<FIXED_MODEL> Temp = m_WeaponModel->Export(m_Texture->GetWidth(), m_Texture->GetHeight(), true);
		m_DX9WeaponModel = ExportDX9(Temp, m_Texture);
	}
#endif

	iObjectMax = m_Model->ObjectCount() ? m_Model->ObjectCount() - 1 : 0;

	m_ModelType = ModelType::Enemy;
	m_ModelGame = Game;

	b_DrawAllObjects = false;
	b_DrawSingleObject = false;

	return true;
}

bool Resident_Evil_Model::OpenWeapon(std::filesystem::path Path, std::uintmax_t _Ptr)
{
	if (!m_Model->ObjectCount())
	{
		Str.Message(L"Resident Evil Model Error: cannot open weapon without player model");
		return false;
	}

	StdFile m_File{ Path, FileAccessMode::Read, true, false };
	if (!m_File.IsOpen())
	{
		Str.Message(L"Resident Evil Model Error: could not read player data at 0x%llX in \"%ws\"", _Ptr, m_File.GetPath().filename().wstring().c_str());
		return false;
	}

	CloseWeapon();

	m_WeaponFilename = Path;

	std::vector<std::uint32_t> Pointer = GetDataPtr(m_File, _Ptr);

	for (std::size_t i = 0; i < Pointer.size(); ++i) { Pointer[i] += (std::uint32_t)_Ptr; }

	if (GameType() & (AUG95 | OCT95 | BIO1))
	{
		if (Pointer[1]) { Animation(WEAPON)->OpenEDD(m_File, Pointer[1]); }
		if (Pointer[1]) { if (Animation(WEAPON)->IsEDDOpen()) Animation(WEAPON)->OpenEMR(m_File, Pointer[0]); }
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
		if (Pointer[1]) { if (Animation(WEAPON)->IsEDDOpen()) Animation(WEAPON)->OpenEMR(m_File, Pointer[1]); }
		if (Pointer[2]) { m_WeaponModel = Resident_Evil_2_MD1(Str.hWnd, m_File, Pointer[2]).GetTMD(); }
		if (Pointer[3]) { m_WeaponTexture->OpenTIM(m_File, Pointer[3]); }
	}
	else if (GameType() & BIO3)
	{
		if (Pointer.size() != 9)
		{
			Str.Message(L"Resident Evil Model Error: \"%ws\" is not weapon data\r\n\r\nAborting...", m_File.GetPath().filename().wstring().c_str());
			return false;
		}
		if (Pointer[0]) { Animation(WEAPON)->OpenEDD(m_File, Pointer[0]); }
		if (Pointer[1]) { if (Animation(WEAPON)->IsEDDOpen()) Animation(WEAPON)->OpenEMR(m_File, Pointer[1]); }
		if (Pointer[2]) { Animation(WEAPON_EX0)->OpenEDD(m_File, Pointer[2]); }
		if (Pointer[3]) { if (Animation(WEAPON_EX0)->IsEDDOpen()) Animation(WEAPON_EX0)->OpenEMR(m_File, Pointer[3]); }
		if (Pointer[4])
		{
			m_WeaponBinary.resize((std::size_t)(Pointer[5] - Pointer[4]));
			m_File.Read(Pointer[4], m_WeaponBinary.data(), m_WeaponBinary.size());
		}
		if (Pointer[5]) { Animation(WEAPON_EX1)->OpenEDD(m_File, Pointer[5]); }
		if (Pointer[6]) { if (Animation(WEAPON_EX1)->IsEDDOpen()) Animation(WEAPON_EX1)->OpenEMR(m_File, Pointer[6]); }
		if (Pointer[7]) { m_WeaponModel = Resident_Evil_3_MD2(Str.hWnd, m_File, Pointer[7]).GetTMD(); }
		if (Pointer[8]) { m_WeaponTexture->OpenTIM(m_File, Pointer[8]); }
	}

	if (Animation(NORMAL)->IsOpen()) { Animation(WEAPON)->Skeleton = Animation(NORMAL)->Skeleton->Clone(); }
	if (Animation(WEAPON_EX0)->IsOpen()) { Animation(WEAPON_EX0)->CreateSkeleton(); }
	if (Animation(WEAPON_EX1)->IsOpen()) { Animation(WEAPON_EX1)->CreateSkeleton(); }

#if MSTD_DX9
	if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2 | BIO3))
	{
		//bool b_TerminateDraw = b_Active.load();

		//StopDrawing();

		IDirect3DTexture9* Temp = Render->BlitTexture(
			Render->CreateTexture(m_WeaponTexture, 0, Sony_Texture_Transparency::Superblack, 0xFF00FF, true),
			Render->CreateTexture(m_Texture, 1, Sony_Texture_Transparency::Superblack, 0xFF00FF, true),
			m_WeaponTexture->GetWidth(), m_WeaponTexture->GetHeight(), 200, 224, false);

		if (m_DX9Model)
		{
			size_t iTexture = !m_DX9Model ? 0 : min(1, m_DX9Model->Texture.size() - 1);
			m_DX9Model->Texture[iTexture].reset(Temp);
		}

		//b_Active.store(b_TerminateDraw);
	}

	std::unique_ptr<FIXED_MODEL> Temp = m_WeaponModel->Export(m_Texture->GetWidth(), m_Texture->GetHeight(), true);
	m_DX9WeaponModel = ExportDX9(Temp, m_Texture);
#endif

	if (GameType() & (AUG95 | OCT95 | BIO1))
	{
		iWeaponObject = 14;
	}
	else if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2))
	{
		iWeaponObject = 11;
	}
	else if (GameType() & BIO3)
	{
		iWeaponObject = 4;
	}

	iWeaponObjectMax = m_Model->ObjectCount() ? m_Model->ObjectCount() - 1 : 0;

	iWeaponObject = std::clamp(iWeaponObject, (size_t)0, iObjectMax);

	m_WeaponModelGame = Game;

	b_DrawWeapon = true;

	return true;
}

bool Resident_Evil_Model::OpenTexture(std::filesystem::path Path, std::uintmax_t _Ptr)
{
	bool b_TerminateDraw = b_Active.load();

	StopDrawing();

	if (m_Texture) { m_Texture->Close(); }

	bool b_Open = false;

	String Extension = Path.extension().string();

	if (Standard_String().ToUpper(Extension) == ".TM2" && !m_Texture->OpenTIM2(Path, _Ptr)) { b_Active.store(b_TerminateDraw); return false; }
	if (Standard_String().ToUpper(Extension) == ".TIM" && !m_Texture->OpenTIM(Path, _Ptr)) { b_Active.store(b_TerminateDraw); return false; }
	if (Standard_String().ToUpper(Extension) == ".BMP" && !m_Texture->OpenBMP(Path, _Ptr)) { b_Active.store(b_TerminateDraw); return false; }
#ifdef LIB_PNG
	if (Standard_String().ToUpper(Extension) == ".PNG" && !m_Texture->OpenPNG(Path, _Ptr)) { b_Active.store(b_TerminateDraw); return false; }
#endif
#ifdef LIB_JPEG
	if ((Standard_String().ToUpper(Extension) == ".JPG" || Standard_String().ToUpper(Extension) == ".JPEG") && !m_Texture->OpenJPEG(Path, _Ptr)) { b_Active.store(b_TerminateDraw); return false; }
#endif

#if MSTD_DX9
	if (m_Model->IsOpen())
	{
		CloseModelDX9();

		std::unique_ptr<FIXED_MODEL> Temp = m_Model->Export(m_Texture->GetWidth(), m_Texture->GetHeight(), true);
		m_DX9Model = ExportDX9(Temp, m_Texture);
	}
#endif

	b_Active.store(b_TerminateDraw);
	
	return b_Open;
}

bool Resident_Evil_Model::OpenWeaponTexture(std::filesystem::path Path, std::uintmax_t _Ptr)
{
	bool b_TerminateDraw = b_Active.load();

	StopDrawing();

	if (m_WeaponTexture) { m_WeaponTexture->Close(); }

	bool b_Open = false;

	String Extension = Path.extension().string();

	if (Standard_String().ToUpper(Extension) == ".TM2" && !m_WeaponTexture->OpenTIM2(Path, _Ptr)) { b_Active.store(b_TerminateDraw); return false; }
	if (Standard_String().ToUpper(Extension) == ".TIM" && !m_WeaponTexture->OpenTIM(Path, _Ptr)) { b_Active.store(b_TerminateDraw); return false; }
	if (Standard_String().ToUpper(Extension) == ".BMP" && !m_WeaponTexture->OpenBMP(Path, _Ptr)) { b_Active.store(b_TerminateDraw); return false; }
#ifdef LIB_PNG
	if (Standard_String().ToUpper(Extension) == ".PNG" && !m_WeaponTexture->OpenPNG(Path, _Ptr)) { b_Active.store(b_TerminateDraw); return false; }
#endif
#ifdef LIB_JPEG
	if ((Standard_String().ToUpper(Extension) == ".JPG" || Standard_String().ToUpper(Extension) == ".JPEG") && !m_WeaponTexture->OpenJPEG(Path, _Ptr)) { b_Active.store(b_TerminateDraw); return false; }
#endif

#if MSTD_DX9
	if (m_WeaponModel->IsOpen() && m_WeaponTexture->IsOpen())
	{
		CloseWeaponDX9();

		std::unique_ptr<FIXED_MODEL> Temp = m_WeaponModel->Export(m_WeaponTexture->GetWidth(), m_WeaponTexture->GetHeight(), true);
		m_DX9WeaponModel = ExportDX9(Temp, m_WeaponTexture);
	}
#endif

	b_Active.store(b_TerminateDraw);

	return b_Open;
}

void Resident_Evil_Model::SetRoomAnimations(std::shared_ptr<Resident_Evil_Animation>& Rbj)
{
	if (Rbj && !Rbj->Data.empty())
	{
		iRoom = 0;
		iRoomMax = Rbj->Data.size() - 1;
		for (size_t i = 0; i < Rbj->Data.size(); i++)
		{
			Rbj->Data[i]->Skeleton = Animation(NORMAL)->Skeleton->Clone();
		}
		Animation(ROOM) = Rbj->Data[0];
	}
	else
	{
		CloseRoom();
	}
}

void Resident_Evil_Model::SetShadow(size_t PaletteID, std::uint16_t X, std::uint16_t Y, std::uint16_t Width, std::uint16_t Height)
{
	auto NextPowerOfTwo = [](int32_t x) {
		if (x <= 0) { return 1; }
		x--;
		for (int Shift = 1; Shift < 32; Shift <<= 1) { x |= x >> Shift; }
		return x + 1;
		};

	float TextureWidth = (float)NextPowerOfTwo((int32_t)m_DX9Model->TextureWidth);
	float TextureHeight = (float)NextPowerOfTwo((int32_t)m_DX9Model->TextureHeight);

	m_Shadow.Rect.l = (float)X;
	m_Shadow.Rect.t = (float)Y;
	m_Shadow.Rect.r = (float)X + (float)Width;
	m_Shadow.Rect.b = (float)Y + (float)Height;

	m_Shadow.Size.w = (float)Width;
	m_Shadow.Size.h = (float)Height;

#ifdef MSTD_DX9
	float TexelOffsetU = 0.5f / TextureWidth;
	float TexelOffsetV = 0.5f / TextureHeight;
#else
	float TexelOffsetU = 0.0f;
	float TexelOffsetV = 0.0f;
#endif

	float U0 = (m_Shadow.Rect.l + TexelOffsetU) / TextureWidth;
	float V0 = (m_Shadow.Rect.t + TexelOffsetV) / TextureHeight;
	float U1 = (m_Shadow.Rect.r + TexelOffsetU) / TextureWidth;
	float V1 = (m_Shadow.Rect.b + TexelOffsetV) / TextureHeight;

	float XX = GTE->ToFloat(Hitbox().w + (Hitbox().w / 4));
	float ZZ = GTE->ToFloat(Hitbox().d + (Hitbox().d / 2));

	m_Shadow.Vec.clear();

	m_Shadow.Vec = {
		{ vec3{ -XX,	0.0f,	-ZZ	}, vec2{ U0, V1 } },
		{ vec3{ -XX,	0.0f,	ZZ	}, vec2{ U0, V0 } },
		{ vec3{ XX,		0.0f,	ZZ	}, vec2{ U1, V0 } },
		{ vec3{ -XX,	0.0f,	-ZZ	}, vec2{ U0, V1 } },
		{ vec3{ XX,		0.0f,	ZZ	}, vec2{ U1, V0 } },
		{ vec3{ XX,		0.0f,	-ZZ	}, vec2{ U1, V1 } }
	};

#ifdef MSTD_DX9
	if (!m_DX9Model || m_DX9Model->Texture.empty()) { return; }
	m_Shadow.TexID = std::clamp(PaletteID, (size_t)0, m_DX9Model->Texture.size() - 1);
	m_Shadow.Vertices.reset(Render->CreateVec3t(m_Shadow.Vec));
#endif
}

void Resident_Evil_Model::CloseModel(void)
{
	m_Filename.clear();

	m_ModelGame = Video_Game::None;

	b_DrawWeapon = false;

	iObject = 0;
	iObjectMin = 0;
	iObjectMax = 0;

	iRoom = 0;
	iRoomMin = 0;
	iRoomMax = 0;

	if (m_Texture) { m_Texture->Close(); }
	if (m_Model) { m_Model->Close(); }

	m_Binary00.clear();
	m_Binary01.clear();
	m_WeaponBinary.clear();

	for (auto& Animation : m_Animations)
	{
		if (Animation->GetType() == AnimationIndex::Room) { continue; }
		Animation->Close(); }

	m_Model->IgnoreMagic(true);

#if MSTD_DX9
	CloseModelDX9();
#endif
}

void Resident_Evil_Model::CloseWeapon(void)
{
	m_WeaponFilename.clear();

	m_WeaponModelGame = Video_Game::None;

	b_DrawWeapon = false;

	b_ControllerMode = false;

	iWeaponObjectMax = 0;

	if (m_WeaponTexture) { m_WeaponTexture->Close(); }
	if (m_WeaponModel) { m_WeaponModel->Close(); }

	m_WeaponBinary.clear();

	Animation(WEAPON)->Close();
	Animation(WEAPON_EX0)->Close();
	Animation(WEAPON_EX1)->Close();

	m_WeaponModel->IgnoreMagic(true);

#if MSTD_DX9
	CloseWeaponDX9();
#endif
}

void Resident_Evil_Model::CloseRoom(void)
{
	iRoom = 0;
	iRoomMin = 0;
	iRoomMax = 0;
	Animation(ROOM)->Close();
}

void Resident_Evil_Model::DrawFrame(std::shared_ptr<Resident_Evil_Animation> Animation, size_t iClip, size_t iFrame, bool b_DrawRoot)
{
	const auto& Origin = Animation->Clip[iClip][iFrame].Origin;
	const auto& Speed = Animation->Clip[iClip][iFrame].Speed;
	const auto& Rotations = Animation->Clip[iClip][iFrame].Rotation;

	vec3 Vector;
	std::vector<vec3c> SkeletonSegments;
	std::vector<Standard_Matrix> Transform(Rotations.size());

	if (b_DrawSkeleton) { SkeletonSegments.reserve(Rotations.size() * 2); }

	if (b_LockPosition) { Vector = { 0.0f, GTE->ToFloat(Origin.y) + GTE->ToFloat(Speed.y), 0.0f }; }
	else
	{
		size_t iNextFrame = iFrame + 1;

		if (iNextFrame >= Animation->GetFrameCount(iClip))
		{
			iNextFrame = 0;
		}

		if (b_LerpKeyframes.load() && iNextFrame != iFrame)
		{
			const auto& OriginNext = Animation->Clip[iClip][iNextFrame].Origin;
			const auto& SpeedNext = Animation->Clip[iClip][iNextFrame].Speed;
			Vector =
			{
				std::lerp(GTE->ToFloat(Origin.x) + GTE->ToFloat(Speed.x), GTE->ToFloat(OriginNext.x) + GTE->ToFloat(SpeedNext.x), m_LerpValue),
				std::lerp(GTE->ToFloat(Origin.y) + GTE->ToFloat(Speed.y), GTE->ToFloat(OriginNext.y) + GTE->ToFloat(SpeedNext.y), m_LerpValue),
				std::lerp(GTE->ToFloat(Origin.z) + GTE->ToFloat(Speed.z), GTE->ToFloat(OriginNext.z) + GTE->ToFloat(SpeedNext.z), m_LerpValue)
			};
		}
		else
		{
			Vector =
			{
				GTE->ToFloat(Origin.x) + GTE->ToFloat(Speed.x),
				GTE->ToFloat(Origin.y) + GTE->ToFloat(Speed.y),
				GTE->ToFloat(Origin.z) + GTE->ToFloat(Speed.z)
			};
		}
	}

	for (size_t i = 0; i < Rotations.size(); i++)
	{
		Transform[i] =
			Standard_Matrix().RotateX(World->Radian(GTE->ToFloat(Rotations[i].x) * 360.0f)) *
			Standard_Matrix().RotateY(World->Radian(GTE->ToFloat(Rotations[i].y) * 360.0f)) *
			Standard_Matrix().RotateZ(World->Radian(GTE->ToFloat(Rotations[i].z) * 360.0f));
	}

	std::function<void(const std::shared_ptr<Resident_Evil_Animation::Bone>&, const Standard_Matrix&)> DrawKeyframe =
		[&](const std::shared_ptr<Resident_Evil_Animation::Bone>& Skeleton, const Standard_Matrix& Matrix)
		{
			if (b_StopDrawing) { return; }

			Standard_Matrix Local;

			if (b_DrawRoot)
			{
				Local = Matrix * Skeleton->World;
			}
			else
			{
				if (Skeleton->Parent.lock())
				{
					Skeleton->Local = Skeleton->World * Transform[Skeleton->ID];
				}
				else
				{
					Skeleton->Local = Standard_Matrix().Translate(Vector) * Transform[Skeleton->ID];
				}

				Local = Matrix * Skeleton->Local;
			}

			if (b_DrawSkeleton)
			{
				for (const auto& Child : Skeleton->Children)
				{
					Standard_Matrix LocalWorld = b_DrawRoot ? (Local * Child->World) : (Local * (Child->World * Transform[Child->ID]));
					SkeletonSegments.push_back({ Local.GetVector(), 0xFFFFFFFF });
					SkeletonSegments.push_back({ LocalWorld.GetVector(), 0xFFFFFFFF });
				}
			}

#if MSTD_DX9
			Render->SetWorld(Local);

			if ((b_DrawWireframe || b_DrawSolidColor || b_DrawTextured) && Skeleton->ID < ModelDX9()->Object.size())
			{
				size_t i = Skeleton->ID;

				if (b_DrawWeapon && iWeaponObject == i && WeaponModelDX9() && !WeaponModelDX9()->Object.empty())
				{
					for (size_t x = 0; x < WeaponModelDX9()->Object[0].size(); x++)
					{
						if (b_StopDrawing.load()) { break; }

						size_t iTexture = std::clamp(WeaponModelDX9()->Object[0][x].iTexture, (size_t)0, WeaponModelDX9()->Texture.empty() ? 0 : WeaponModelDX9()->Texture.size() - 1);

						Render->DrawVec3cnt(
							WeaponModelDX9()->Object[0][x].Data.get(),
							nullptr,
							b_DrawTextured ? ModelDX9()->Texture.empty() ? nullptr : ModelDX9()->Texture[iTexture].get() : nullptr,
							b_Dither ? Render->PS1DitherPixelShader.get() : Render->PassthroughPixelShader.get(),
							ModelDX9()->TextureWidth, ModelDX9()->TextureHeight,
							b_DrawWireframe ? D3DFILL_WIREFRAME : D3DFILL_SOLID, D3DPT_TRIANGLELIST
						);
					}
				}
				else
				{
					for (size_t x = 0; x < ModelDX9()->Object[i].size(); x++)
					{
						if (b_StopDrawing.load()) { break; }

						size_t iTexture = std::clamp(ModelDX9()->Object[i][x].iTexture, (size_t)0, ModelDX9()->Texture.empty() ? 0 : ModelDX9()->Texture.size() - 1);

						Render->DrawVec3cnt(
							ModelDX9()->Object[i][x].Data.get(),
							nullptr,
							b_DrawTextured ? ModelDX9()->Texture.empty() ? nullptr : ModelDX9()->Texture[iTexture].get() : nullptr,
							b_Dither ? Render->PS1DitherPixelShader.get() : Render->PassthroughPixelShader.get(),
							ModelDX9()->TextureWidth, ModelDX9()->TextureHeight,
							b_DrawWireframe ? D3DFILL_WIREFRAME : D3DFILL_SOLID, D3DPT_TRIANGLELIST
						);
					}
				}
			}
#endif
			for (const auto& Children : Skeleton->Children) { DrawKeyframe(Children, Local); }
		};

	if (b_EditorMode)
	{
		SetWorld({ EditorPosition().x, EditorPosition().y, EditorPosition().z, EditorRotation().x, EditorRotation().y, EditorRotation().z, EditorScale().x, EditorScale().y, EditorScale().z });
	}
	else
	{
		SetWorld({ Position().x, Position().y, Position().z, Rotation().x, Rotation().y, Rotation().z, Scale().x, Scale().y, Scale().z });
	}

#if MSTD_DX9
	Render->SetWorld(World);

	Render->Device()->SetRenderState(D3DRS_CULLMODE, b_HorzFlip && b_VertFlip ? D3DCULL_CW : b_HorzFlip ? D3DCULL_CCW : b_VertFlip ? D3DCULL_CCW : D3DCULL_CW);
	Render->Device()->SetRenderState(D3DRS_CLIPPING, TRUE);

	Render->TextureFiltering(m_TextureFilter);
#endif

	b_Drawing.store(true);
	DrawKeyframe(Animation->Skeleton, *World);
	b_Drawing.store(false);
	b_StopDrawing.store(false);

#if MSTD_DX9
	Render->TextureFiltering(D3DTEXF_NONE);

	Render->ResetWorld();

	if (b_DrawSkeleton && !SkeletonSegments.empty())
	{
		std::unique_ptr<IDirect3DVertexBuffer9, IDirect3DDelete9<IDirect3DVertexBuffer9>> Vertices(Render->CreateVec3c(SkeletonSegments));
		Render->DrawVec3c(Vertices.get(), nullptr, nullptr, D3DFILL_SOLID, D3DPT_LINELIST);
	}
#endif
}

void Resident_Evil_Model::DrawObject(std::size_t iObject, bool b_DrawAll, bool DrawWeapon)
{
	b_Drawing.store(true);

#if MSTD_DX9
	auto& Model = DrawWeapon ? WeaponModelDX9() : ModelDX9();

	if (!Model || Model->Object.empty()) { b_Drawing.store(false); return; }

	if (b_EditorMode)
	{
		SetWorld({
			EditorPosition().x, EditorPosition().y, EditorPosition().z,
			EditorRotation().x, EditorRotation().y, EditorRotation().z,
			EditorScale().x, EditorScale().y, EditorScale().z });
	}
	else
	{
		SetWorld({
			Position().x, Position().y, Position().z,
			Rotation().x, Rotation().y, Rotation().z,
			Scale().x, Scale().y, Scale().z });
	}

	std::size_t nObjects = iObject + 1;

	if (nObjects > Model->Object.size())
	{
		nObjects = Model->Object.size();
		iObject = 0;
	}

	if (b_DrawAll)
	{
		iObject = 0;
		nObjects = Model->Object.size();
	}

	Render->SetWorld(World);

	Render->Device()->SetRenderState(D3DRS_CULLMODE, b_HorzFlip && b_VertFlip ? D3DCULL_CW : b_HorzFlip ? D3DCULL_CCW : b_VertFlip ? D3DCULL_CCW : D3DCULL_CW);
	Render->Device()->SetRenderState(D3DRS_CLIPPING, TRUE);

	Render->TextureFiltering(m_TextureFilter);

	for (size_t i = iObject; i < nObjects; i++)
	{
		if (b_StopDrawing.load()) { break; }

		for (size_t x = 0; x < Model->Object[i].size(); x++)
		{
			if (b_StopDrawing.load()) { break; }

			size_t iTexture = std::clamp(Model->Object[i][x].iTexture, (size_t)0, Model->Texture.empty() ? 0 : Model->Texture.size() - 1);
			IDirect3DTexture9* Texture = DrawWeapon ? 
				!ModelDX9() ? nullptr :
				(b_DrawTextured ? ModelDX9()->Texture.empty() ? nullptr : ModelDX9()->Texture[iTexture].get() : nullptr) :
				(b_DrawTextured ? Model->Texture.empty() ? nullptr : Model->Texture[iTexture].get() : nullptr);
			Render->DrawVec3cnt(
				Model->Object[i][x].Data.get(),
				nullptr,
				Texture,
				b_Dither ? Render->PS1DitherPixelShader.get() : Render->PassthroughPixelShader.get(),
				Model->TextureWidth, Model->TextureHeight,
				b_DrawWireframe ? D3DFILL_WIREFRAME : D3DFILL_SOLID, D3DPT_TRIANGLELIST
			);
		}
	}

	Render->TextureFiltering(D3DTEXF_NONE);

	Render->ResetWorld();
#endif

	b_Drawing.store(false);
	b_StopDrawing.store(false);
}

void Resident_Evil_Model::Draw(void)
{
	if (!b_Active.load()) { return; }

	if (b_DrawAllObjects || b_DrawSingleObject)
	{
		DrawObject(iObject, b_DrawAllObjects, b_DrawWeapon);
		//return;
	}

	auto Index = m_AnimationIndex.load();
	bool b_DrawRoot = b_DrawReference;

	if (!Animation(Index)->IsOpen() || !Animation(Index)->Skeleton->IsOpen())
	{
		if (Animation(NORMAL)->IsOpen() && Animation(NORMAL)->Skeleton->IsOpen())
		{
			Index = NORMAL;
			b_DrawRoot = true;
		}
		else
		{
			return;
		}
	}

	iClip.store(min(iClip.load(), Animation(Index)->GetClipCount() - 1));

	iFrame.store(min(iFrame.load(), Animation(Index)->GetFrameCount(iClip.load()) - 1));

	DrawFrame(Animation(Index), iClip.load(), iFrame.load(), b_DrawRoot);

	if (!b_Play.load()) { return; }

	if (m_FrameCounter % 2 == 0)
	{
		if (b_PlayInReverse.load())
		{
			iFrame.fetch_sub(1);
			if (iFrame.load() >= Animation(Index)->GetFrameCount(iClip.load())) { iFrame.store(0); }
		}
		else
		{
			iFrame.fetch_add(1);
		}
	}
	m_FrameCounter++;

	const auto m_FrameCount = Animation(Index)->GetFrameCount(iClip.load());
	const auto b_FirstFrame = (iFrame.load() == 0);
	const auto b_LastFrame = (iFrame.load() >= m_FrameCount);

	if (b_PlayAllFrames.load())
	{
		if ((!b_PlayInReverse.load() && b_LastFrame) || (b_PlayInReverse.load() && b_FirstFrame))
		{
			b_PlayAllFrames.store(false);
			b_PlayInReverse.store(false);
			m_FrameCounter = 0;
		}
	}

	else if (b_Loop.load())
	{
		if (!b_PlayInReverse.load())
		{
			if (b_LastFrame) { iFrame.store(0); m_FrameCounter = 0; }
		}
		else
		{
			if (b_FirstFrame) { iFrame.store(m_FrameCount - 1); m_FrameCounter = 0; }
		}
	}
}

void Resident_Evil_Model::DrawShadow(void)
{
	if (!b_DrawShadow) { return; }
#ifdef MSTD_DX9
	if (!m_DX9Model || m_DX9Model->Texture.empty() || !Shadow().Vertices) { return; }

	Render->SetWorld(World);

	Render->Device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	Render->AlphaTesting(TRUE, 0xFF, D3DCMP_GREATEREQUAL);

	Shadow().TexID = std::clamp(Shadow().TexID, (size_t)0, m_DX9Model->Texture.size() - 1);

	Render->DrawVec3t(
		Shadow().Vertices.get(),
		nullptr,
		ModelDX9()->Texture[Shadow().TexID].get(),
		Render->PS1DitherPixelShader.get(),
		ModelDX9()->TextureWidth, ModelDX9()->TextureHeight,
		D3DFILL_SOLID, D3DPT_TRIANGLESTRIP
	);

	Render->ResetWorld();
#endif
}

void Resident_Evil_Model::AddSpeedXZ(SVECTOR* Speed)
{
	SVECTOR Sv{};
	MATRIX M{};

	Sv.vx = 0;
	Sv.vz = 0;
	Sv.vy = Rotation().y + Speed->vy;

	GTE->RotMatrix(&Sv, &M);
	GTE->ApplyMatrixSV(&M, Speed, &Sv);

	Position().x += Sv.vx;
	Position().y += Speed->vy;
	Position().z += Sv.vz;
}