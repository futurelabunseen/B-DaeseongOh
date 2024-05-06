
#pragma once

#include "GameplayTagContainer.h"

#define MDTAG_ANIM_CHARGE FGameplayTag::RequestGameplayTag(FName("Player.Anim.Charge"))
#define MDTAG_ANIM_COMBO FGameplayTag::RequestGameplayTag(FName("Player.Anim.Combo"))
#define MDTAG_ANIM_ONCE FGameplayTag::RequestGameplayTag(FName("Player.Anim.Once"))
#define MDTAG_DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))

#define MDTAG_WEAPON_TYPE FGameplayTag::RequestGameplayTag(FName("Weapon.Type"))
#define MDTAG_WEAPON_NONE FGameplayTag::RequestGameplayTag(FName("Weapon.Type.None"))
#define MDTAG_WEAPON_BOW FGameplayTag::RequestGameplayTag(FName("Weapon.Type.Bow"))
#define MDTAG_WEAPON_ONEHANDEDSWORD FGameplayTag::RequestGameplayTag(FName("Weapon.Type.OneHandedSword"))
#define MDTAG_WEAPON_TWOHANDEDSWORD FGameplayTag::RequestGameplayTag(FName("Weapon.Type.TwoHandedSword"))
#define MDTAG_WEAPON_PRIMARYATTACK FGameplayTag::RequestGameplayTag(FName("Weapon.Attack.PrimaryAttack"))
#define MDTAG_WEAPON_SKILL_01 FGameplayTag::RequestGameplayTag(FName("Weapon.Attack.Skill_01"))
#define MDTAG_WEAPON_SKILL_02 FGameplayTag::RequestGameplayTag(FName("Weapon.Attack.Skill_02"))
#define MDTAG_WEAPON_SKILL_03 FGameplayTag::RequestGameplayTag(FName("Weapon.Attack.Skill_03"))
#define MDTAG_WEAPON_SKILL_04 FGameplayTag::RequestGameplayTag(FName("Weapon.Attack.Skill_04"))
#define MDTAG_WEAPON_ATTACK FGameplayTag::RequestGameplayTag(FName("Weapon.Attack"))

#define MDTAG_HITCHECK_TRACE FGameplayTag::RequestGameplayTag(FName("Character.AttackHitCheck.Trace"))
#define MDTAG_CHARACTER_ISDEAD FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"))
#define MDTAG_CHARACTER_INVINCIBLE FGameplayTag::RequestGameplayTag(FName("Character.State.Invincible"))

#define MDTAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.AttackHit"))

#define MDTAG_EVENT_CHARACTER_WEAPONEQUIP FGameplayTag::RequestGameplayTag(FName("Event.Character.Weapon.Equip"))
#define MDTAG_EVENT_CHARACTER_WEAPONUNEQUIP FGameplayTag::RequestGameplayTag(FName("Event.Character.Weapon.Unequip"))