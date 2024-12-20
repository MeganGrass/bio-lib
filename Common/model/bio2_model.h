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

#include <bio1_model.h>


class Resident_Evil_2_Model :
	private Resident_Evil_Model {
private:

public:

	// Resident Evil 2 Animation
	std::vector<std::unique_ptr<Resident_Evil_2_Animation>> Anim;

	/*
		Construction
	*/
	explicit Resident_Evil_2_Model(void)
	{
		for (std::int32_t i = 0; i < 8; i++)
		{
			Anim.push_back(std::make_unique<Resident_Evil_2_Animation>());
		}
		Anim[ANIM_NORMAL]->SetType(Resident_Evil_Animation_Type::Normal);
		Anim[ANIM_NORMAL_EX0]->SetType(Resident_Evil_Animation_Type::Normal_Ex0);
		Anim[ANIM_NORMAL_EX1]->SetType(Resident_Evil_Animation_Type::Normal_Ex1);
		Anim[ANIM_DAMAGE]->SetType(Resident_Evil_Animation_Type::Damage);
		Anim[ANIM_WEAPON]->SetType(Resident_Evil_Animation_Type::Weapon);
		Anim[ANIM_WEAPON_EX0]->SetType(Resident_Evil_Animation_Type::Weapon_Ex0);
		Anim[ANIM_WEAPON_EX1]->SetType(Resident_Evil_Animation_Type::Weapon_Ex1);
		Anim[ANIM_ROOM]->SetType(Resident_Evil_Animation_Type::Room);
		SetGame(Resident_Evil_Video_Game::Resident_Evil_2);
	}
	virtual ~Resident_Evil_2_Model(void)
	{
		for (auto& i : Anim) { i.get()->CloseEDD(); i.get()->CloseEMR(); i.reset(); }
		Anim.clear();
	}

	/*
		Get animation data
	*/
	std::unique_ptr<Resident_Evil_2_Animation>& GetAnimation(std::size_t AnimationType) { return Anim[AnimationType]; }

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

};