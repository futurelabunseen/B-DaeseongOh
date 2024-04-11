
#pragma once

#include "GameplayTagContainer.h"

#define MDTAG_INPUT_ATTACK FGameplayTag::RequestGameplayTag(FName("Player.Input.Attack"))
#define MDTAG_INPUT_SKILL01 FGameplayTag::RequestGameplayTag(FName("Player.Input.Skill_01"))
#define MDTAG_INPUT_SKILL02 FGameplayTag::RequestGameplayTag(FName("Player.Input.Skill_02"))
#define MDTAG_INPUT_SKILL03 FGameplayTag::RequestGameplayTag(FName("Player.Input.Skill_03"))
#define MDTAG_DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))

#define MDTAG_WEAPON_NONE FGameplayTag::RequestGameplayTag(FName("Weapon.Type.None"))
#define MDTAG_WEAPON_BOW FGameplayTag::RequestGameplayTag(FName("Weapon.Type.Bow"))
#define MDTAG_WEAPON_ONEHANDEDSWORD FGameplayTag::RequestGameplayTag(FName("Weapon.Type.OneHandedSword"))
#define MDTAG_WEAPON_TWOHANDEDSWORD FGameplayTag::RequestGameplayTag(FName("Weapon.Type.TwoHandedSword"))

#define MDTAG_HITCHECK_TRACE FGameplayTag::RequestGameplayTag(FName("Character.AttackHitCheck.Trace"))
#define MDTAG_HITCHECK_PROJECTILE FGameplayTag::RequestGameplayTag(FName("Character.AttackHitCheck.Projectile"))