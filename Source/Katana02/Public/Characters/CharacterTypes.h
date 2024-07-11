#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	//û������������������˫������
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	//����״̬���ܻ���������װ�����������ܡ�����
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_HitReaction UMETA(DisplayName = "HitReaction"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon"),
	EAS_Dodge UMETA(DisplayName = "Dodge"),
	EAS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Alive UMETA(DisplayName = "Alive"),
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_Death2 UMETA(DisplayName = "Death2"),
	EDP_Death3 UMETA(DisplayName = "Death3"),
	EDP_Death4 UMETA(DisplayName = "Death4"),
	EDP_Death5 UMETA(DisplayName = "Death5"),
	EDP_Death6 UMETA(DisplayName = "Death6"),

	EDP_MAX UMETA(DisplayName = "DefaultMAX")

	//��������
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	//û��״̬��������Ѳ�ߡ�׷�𡢹�������ռ��

	EES_NoState UMETA(DisplayName = "NoState"),

	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged")

};

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	//�����ƶ��Ͳ������ƶ�
	EMS_CanMoving UMETA(DisplayName = "CanMoving"),
	EMS_UnMoving UMETA(DisplayName = "UnMoving")
};


UENUM(BlueprintType)
enum class EAttackState : uint8
{
	//�����ƶ��Ͳ������ƶ�
	EATS_CanAttacking UMETA(DisplayName = "CanAttacking"),
	EATS_UnAtacking UMETA(DisplayName = "UnAtacking")
};