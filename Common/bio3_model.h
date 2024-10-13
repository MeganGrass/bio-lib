/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO:

	// PLD
	// 00	EDD (w/Skeleton)
	// 01	EMR (w/Skeleton)
	// 02	MD2
	// 03	Binary
	// 04	TIM

	// PLW
	// 00	EDD
	// 01	EMR
	// 02	EDD (w/Skeleton)
	// 03	EMR (w/Skeleton)
	// 04	Weapon Binary
	// 05	EDD (w/Skeleton)
	// 06	EMR (w/Skeleton)
	// 07	MD2
	// 08	TIM

	// EMD
	// 00	Binary
	// 01	Binary
	// 02	EDD (w/Skeleton)
	// 03	EMR (w/Skeleton)
	// 04	EDD (w/Skeleton) (same skeleton as 03)
	// 05	EMR (w/Skeleton) (same skeleton as 03)
	// 06	EDD
	// 07	EMR
	// 08	Weapon Binary
	// 09	EDD (w/Skeleton) (same as PLW 03)
	// 10	EMR (w/Skeleton) (same as PLW 03)
	// 11	EDD (w/Skeleton) (same as PLW 06)
	// 12	EMR (w/Skeleton) (same as PLW 06)
	// 13	MD2 (Sub) found in em34.emd
	// 14	MD2 (Main)
*
*/


#pragma once

#include <bio1_model.h>


class Resident_Evil_3_Model :
	public Resident_Evil_Model {
private:

	// Character Model
	std::unique_ptr<Resident_Evil_3_MD2> Md2;

	// Extra Model
	std::unique_ptr<Resident_Evil_3_MD2> Md2Ex;

	// Binary Chunk 00
	std::vector<std::uint8_t> Binary00;

	// Binary Chunk 01
	std::vector<std::uint8_t> Binary01;

	// Weapon Binary
	std::vector<std::uint8_t> WeaponBinary;

	// Resident Evil 3 Animation
	std::vector<std::unique_ptr<Resident_Evil_3_Animation>> Anim;

public:

	/*
		Construction
	*/
	explicit Resident_Evil_3_Model(void) :
		Md2(std::make_unique<Resident_Evil_3_MD2>()),
		Md2Ex(std::make_unique<Resident_Evil_3_MD2>())
	{
		for (std::int32_t i = 0; i < 8; i++)
		{
			Anim.push_back(std::make_unique<Resident_Evil_3_Animation>());
		}
		Anim[ANIM_NORMAL]->SetType(Resident_Evil_Animation_Type::Normal);
		Anim[ANIM_NORMAL_EX]->SetType(Resident_Evil_Animation_Type::Normal_Ex);
		Anim[ANIM_EXTENDED]->SetType(Resident_Evil_Animation_Type::Extended);
		Anim[ANIM_DAMAGE]->SetType(Resident_Evil_Animation_Type::Damage);
		Anim[ANIM_WEAPON]->SetType(Resident_Evil_Animation_Type::Weapon);
		Anim[ANIM_WEAPON_EX0]->SetType(Resident_Evil_Animation_Type::Weapon_Ex0);
		Anim[ANIM_WEAPON_EX1]->SetType(Resident_Evil_Animation_Type::Weapon_Ex1);
		Anim[ANIM_ROOM]->SetType(Resident_Evil_Animation_Type::Room);
		SetGame(Resident_Evil_Video_Game::Resident_Evil_3);
	}
	virtual ~Resident_Evil_3_Model(void)
	{
		Close();
		Md2.reset();
		Md2Ex.reset();
		for (auto& i : Anim) { i.reset(); }
		Anim.clear();
	}

	/*
		Get model data
	*/
	std::unique_ptr<Resident_Evil_3_MD2>& GetModelMD2(void) { return Md2; }

	/*
		Get model extra data
	*/
	std::unique_ptr<Resident_Evil_3_MD2>& GetModelMD2Ex(void) { return Md2Ex; }

	/*
		Get animation data
	*/
	std::unique_ptr<Resident_Evil_3_Animation>& GetAnimation(std::size_t AnimationType) { return Anim[AnimationType]; }

	/*
		Open
	*/
	virtual bool Open(std::filesystem::path Input) override;

	/*
		Save
	*/
	virtual bool Save(std::filesystem::path Output) override { return false; }

	/*
		Extract
	*/
	virtual bool Extract(std::filesystem::path Input, Resident_Evil_Video_Game _Game) override { return false; }

	/*
		Assemble
	*/
	virtual bool Assemble(std::filesystem::path Config) override { return false; }

	/*
		Close
	*/
	virtual void Close(void) override;

};