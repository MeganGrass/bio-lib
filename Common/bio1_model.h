/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO:
*
*/


#pragma once

#include <std_basic_fstream.h>

#include <std_text.h>

#include <sony_texture.h>

#include <sony_model.h>

#include <bio_common.h>

#include <bio1_animation.h>

#include <bio2_md1.h>

#include <bio2_animation.h>

#include <bio3_md2.h>

#include <bio3_animation.h>


enum class Resident_Evil_Model_Type : int
{
	Player = 1 << 0,
	Enemy = 1 << 1,
	Weapon = 1 << 2
};


class Resident_Evil_Model :
	private Resident_Evil_Common {
private:

	// Resident Evil model type constants
	static constexpr int PLAYER = std::to_underlying(Resident_Evil_Model_Type::Player);
	static constexpr int ENEMY = std::to_underlying(Resident_Evil_Model_Type::Enemy);
	static constexpr int WEAPON = std::to_underlying(Resident_Evil_Model_Type::Weapon);

	// Resident Evil Model Type
	Resident_Evil_Model_Type Type;

	// Character Model
	std::unique_ptr<Sony_PlayStation_Model> Tmd;

	// Extra Model (Bio3)
	std::unique_ptr<Sony_PlayStation_Model> TmdEx;

	// Character Texture
	std::unique_ptr<Sony_PlayStation_Texture> Tim;

	// Resident Evil Animation
	std::vector<std::unique_ptr<Resident_Evil_Animation>> Anim;

	// Flag
	bool b_Open;

public:

	/*
		Construction
	*/
	explicit Resident_Evil_Model(void) :
		Type(Resident_Evil_Model_Type::Player),
		Tmd(std::make_unique<Sony_PlayStation_Model>()),
		TmdEx(std::make_unique<Sony_PlayStation_Model>()),
		Tim(std::make_unique<Sony_PlayStation_Texture>()),
		b_Open(false)
	{
		for (std::int32_t i = 0; i < 8; i++)
		{
			Anim.push_back(std::make_unique<Resident_Evil_Animation>());
		}
		Anim[ANIM_NORMAL]->SetType(Resident_Evil_Animation_Type::Normal);
		Anim[ANIM_NORMAL_EX]->SetType(Resident_Evil_Animation_Type::Normal_Ex);
		Anim[ANIM_EXTENDED]->SetType(Resident_Evil_Animation_Type::Extended);
		Anim[ANIM_DAMAGE]->SetType(Resident_Evil_Animation_Type::Damage);
		Anim[ANIM_WEAPON]->SetType(Resident_Evil_Animation_Type::Weapon);
		Anim[ANIM_WEAPON_EX0]->SetType(Resident_Evil_Animation_Type::Weapon_Ex0);
		Anim[ANIM_WEAPON_EX1]->SetType(Resident_Evil_Animation_Type::Weapon_Ex1);
		Anim[ANIM_ROOM]->SetType(Resident_Evil_Animation_Type::Room);
		SetGame(Resident_Evil_Video_Game::Resident_Evil);
		Tmd->IgnoreMagic(true);
	}
	virtual ~Resident_Evil_Model(void)
	{
		Close();
		Tmd.reset();
		TmdEx.reset();
		Tim.reset();
		for (auto& i : Anim) { i.get()->CloseEDD(); i.get()->CloseEMR(); i.reset(); }
		Anim.clear();
	}

	/*
		Friend
	*/
	friend class Resident_Evil_2_Model;
	friend class Resident_Evil_3_Model;

	/*
		Check if the model is open
	*/
	bool operator !() { return !b_Open; }

	/*
		Check if the model is open
	*/
	bool IsOpen(void) const noexcept { return b_Open; }

	/*
		Set game
	*/
	virtual void SetGame(Resident_Evil_Video_Game _Game) override;

	/*
		Get model type
	*/
	Resident_Evil_Model_Type GetType(void) const { return Type; }

	/*
		Get model data
	*/
	std::unique_ptr<Sony_PlayStation_Model>& GetModelTMD(void) { return Tmd; }

	/*
		Get model extra data
	*/
	std::unique_ptr<Sony_PlayStation_Model>& GetModelTMDEx(void) { return TmdEx; }

	/*
		Get texture data
	*/
	std::unique_ptr<Sony_PlayStation_Texture>& GetTexture(void) { return Tim; }

	/*
		Get animation data
	*/
	std::unique_ptr<Resident_Evil_Animation>& GetAnimation(std::size_t AnimationType) { return Anim[AnimationType]; }

	/*
		Open
	*/
	virtual bool Open(std::filesystem::path Input);

	/*
		Save
	*/
	virtual bool Save(std::filesystem::path Output);

	/*
		Extract
	*/
	virtual bool Extract(std::filesystem::path Input, Resident_Evil_Video_Game _Game);

	/*
		Assemble
	*/
	virtual bool Assemble(std::filesystem::path Config);

	/*
		Close
	*/
	virtual void Close(void);

};