#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "InputActionValue.h"
#include "Items/Weapons/Weapon.h"
#include "KatanaCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAnimMontage;
class AItem;

UCLASS()
class KATANA02_API AKatanaCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AKatanaCharacter();

protected:
	virtual void BeginPlay() override;
	
	void JumpAct();
	void EKeyPressed();
	virtual void Attack() override;
	virtual void AttackEnd() override;

	virtual void Skill();
	void PlaySkillMontage();
	void ResetAttackCooldown();
	void ResetAttackCombo();
	void ResetSkillCooldown();

	virtual void PlayAttackMontage() override;
	void PlayEquipMontage();
	void PlayUnEquipMontage();

	bool CanJump();
	virtual bool CanAttack() override;
	bool CanSkill();
	bool CanDisarm();
	bool CanArm();
	void Disarm();
	void Arm();

	void EquipWeapon(AWeapon* Weapon);

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// Attack combo variables
	int32 AttackCount;
	bool bCanAttack;

	float AttackCooldown;
	float AttackComboResetTime;
	FTimerHandle AttackCooldownTimer;
	FTimerHandle AttackComboResetTimer;

	// Skill variables
	bool bCanSkill;
	float SkillCooldown;
	FTimerHandle SkillCooldownTimer;

	UPROPERTY(EditAnywhere, Category = InputAct)
	UInputMappingContext* KatanaContent;

	UPROPERTY(EditAnywhere, Category = InputAct)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = InputAct)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = InputAct)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = InputAct)
	UInputAction* EKeyPressedAction;

	UPROPERTY(EditAnywhere, Category = InputAct)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = InputAct)
	UInputAction* SkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* SkillMontage01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* EquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* UnEquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* JumpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* JumpEquipedMontage;

	UFUNCTION(BlueprintCallable)
	void SkillEnd();

	UFUNCTION(BlueprintCallable)
	void EquipEnd();

	UFUNCTION(BlueprintCallable)
	void UnEquipEnd();

	UFUNCTION(BlueprintCallable)
	void JumpEnd();

	UFUNCTION(BlueprintCallable)
	void JumpEquipEnd();

private:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EMoveState MoveState = EMoveState::EMS_CanMoving;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EAttackState AttackState = EAttackState::EATS_CanAttacking;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }

};
