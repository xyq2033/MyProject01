#include "Characters/KatanaCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Engine/Engine.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Components/BoxComponent.h"


AKatanaCharacter::AKatanaCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);


	bCanAttack = true;
	
	AttackCount = 0;
	AttackCooldown = 0.5f; // Example cooldown duration
	AttackComboResetTime = 1.f; // Time to reset attack combo if no further attacks

	// Initialize skill variables
	bCanSkill = true;
	SkillCooldown = 1.f; // Example cooldown duration for skill

	// 使用ConstructorHelpers查找并初始化JumpMontage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> JumpMontageObj(TEXT("/Game/Blueprints/Animations/AM_Jump.AM_Jump")); 
	if (JumpMontageObj.Succeeded())
	{
		JumpMontage = JumpMontageObj.Object;
	}

}


void AKatanaCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("R_Hand_WeaponSocket"), this, this);
	//CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;

}

void AKatanaCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("EngageableTarget"));
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(KatanaContent, 0);
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("EKeyPressed: ActionState = %d, CharacterState = %d"), (int32)ActionState, (int32)CharacterState));
}

void AKatanaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKatanaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKatanaCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AKatanaCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AKatanaCharacter::JumpAct);
		EnhancedInputComponent->BindAction(EKeyPressedAction, ETriggerEvent::Triggered, this, &AKatanaCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AKatanaCharacter::Attack);
		EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &AKatanaCharacter::Skill);
	}

}

void AKatanaCharacter::Move(const FInputActionValue& Value)
{
	if (MoveState == EMoveState::EMS_UnMoving) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AKatanaCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

//void AKatanaCharacter::Jump()
//{
//	Super::Jump();
//}

void AKatanaCharacter::JumpAct()
{

	if (CanJump())
	{
		if (JumpMontage && !GetMesh()->GetAnimInstance()->Montage_IsPlaying(JumpMontage))
		{
			PlayAnimMontage(JumpMontage);
		}

		// 调用基础的跳跃逻辑
		Jump();
	}
	else 
	{
		if (JumpEquipedMontage && !GetMesh()->GetAnimInstance()->Montage_IsPlaying(JumpEquipedMontage))
		{
			PlayAnimMontage(JumpEquipedMontage);
		}

		// 调用基础的跳跃逻辑
		Jump();
	}
	

}

void AKatanaCharacter::EKeyPressed()
{
	if (CanArm())
	{
		AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
		if (OverlappingWeapon)
		{
			if (EquippedWeapon)
			{
				EquippedWeapon->Destroy();
			}
			EquipWeapon(OverlappingWeapon);
		}
		Arm();
	}
	else if (CanDisarm())
	{
		Disarm();
	}
}

bool AKatanaCharacter::CanArm()
{
	//状态未占用，且人物没有武器，可以抽剑
	/*return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped;*/

	return CharacterState == ECharacterState::ECS_Unequipped;
}

void AKatanaCharacter::Arm()
{
	//设置状态：人物状态单手武器，行动状态为装备武器状态
	MoveState = EMoveState::EMS_UnMoving;
	PlayEquipMontage();

	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Arming weapon"));
}

bool AKatanaCharacter::CanDisarm()
{
	//状态未占用，且人物不为没武器状态，可以收剑
	return ActionState == EActionState::EAS_EquippingWeapon &&
		CharacterState == ECharacterState::ECS_EquippedOneHandedWeapon;
}

void AKatanaCharacter::Disarm()
{
	//人物状态未占用，且行动状态为有武器
	PlayUnEquipMontage();

	MoveState = EMoveState::EMS_UnMoving;
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_Unoccupied;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Disarming weapon"));
}

void AKatanaCharacter::PlayEquipMontage()
{

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Playing Equip Montage"));
}

void AKatanaCharacter::PlayUnEquipMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && UnEquipMontage)
	{
		AnimInstance->Montage_Play(UnEquipMontage);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Playing UnEquip Montage"));
}

bool AKatanaCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_EquippingWeapon &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

void AKatanaCharacter::EquipEnd()
{
	MoveState = EMoveState::EMS_CanMoving;
}

void AKatanaCharacter::UnEquipEnd()
{
	MoveState = EMoveState::EMS_CanMoving;
}

void AKatanaCharacter::JumpEnd()
{
	MoveState = EMoveState::EMS_CanMoving;
}

void AKatanaCharacter::JumpEquipEnd()
{
	MoveState = EMoveState::EMS_CanMoving;
}

bool AKatanaCharacter::CanJump()
{
	return CharacterState == ECharacterState::ECS_Unequipped;
}

void AKatanaCharacter::Attack()
{
	Super::Attack();

	if (CanAttack())
	{
		PlayAttackMontage();
		//ActionState = EActionState::EAS_Attacking;
		MoveState = EMoveState::EMS_UnMoving;
		AttackState = EAttackState::EATS_CanAttacking;
	}
}

void AKatanaCharacter::PlayAttackMontage()
{
	Super::PlayAttackMontage();

	if (bCanAttack)
	{
		UAnimMontage* CurrentMontage = nullptr;
		switch (AttackCount)
		{
		case 0:
			CurrentMontage = Attack1;
			break;
		case 1:
			CurrentMontage = Attack2;
			break;
		case 2:
			CurrentMontage = Attack3;
			break;
		}

		if (CurrentMontage)
		{
			PlayAnimMontage(CurrentMontage);
		}

		AttackCount = (AttackCount + 1) % 3;

		bCanAttack = false;
		GetWorldTimerManager().SetTimer(AttackCooldownTimer, this, &AKatanaCharacter::ResetAttackCooldown, AttackCooldown, false);

		GetWorldTimerManager().ClearTimer(AttackComboResetTimer);
		GetWorldTimerManager().SetTimer(AttackComboResetTimer, this, &AKatanaCharacter::ResetAttackCombo, AttackComboResetTime, false);
	}
	
}

void AKatanaCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_EquippingWeapon;
	MoveState = EMoveState::EMS_CanMoving;
}

bool AKatanaCharacter::CanSkill()
{
	return ActionState == EActionState::EAS_EquippingWeapon &&
		CharacterState != ECharacterState::ECS_Unequipped &&
		bCanSkill;
}

void AKatanaCharacter::Skill()
{
	if (CanSkill())
	{
		MoveState = EMoveState::EMS_UnMoving;
		PlaySkillMontage();
		//ActionState = EActionState::EAS_Attacking;
		AttackState = EAttackState::EATS_CanAttacking;

	}
	else if (!bCanSkill)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("技能正在冷却中！"));
		}
	}
}

void AKatanaCharacter::SkillEnd()
{
	ActionState = EActionState::EAS_EquippingWeapon;
	MoveState = EMoveState::EMS_CanMoving;

	// 确保在技能结束时正确重置状态
	if (AttackState == EAttackState::EATS_CanAttacking)
	{
		ActionState = EActionState::EAS_EquippingWeapon;
	}
	MoveState = EMoveState::EMS_CanMoving;
}


void AKatanaCharacter::PlaySkillMontage()
{
	if (bCanSkill)
	{
		if (SkillMontage01)
		{
			PlayAnimMontage(SkillMontage01);
		}

		bCanSkill = false;
		
		GetWorldTimerManager().SetTimer(SkillCooldownTimer, this, &AKatanaCharacter::ResetSkillCooldown, SkillCooldown, false);
	
	}
}

void AKatanaCharacter::ResetAttackCooldown()
{
	bCanAttack = true;
}

void AKatanaCharacter::ResetAttackCombo()
{
	AttackCount = 0;

}
	
void AKatanaCharacter::ResetSkillCooldown()
{
	bCanSkill = true;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("技能已冷却完毕！"));
	}

	if (AttackState == EAttackState::EATS_CanAttacking)
	{
		ActionState = EActionState::EAS_EquippingWeapon;
	}
	MoveState = EMoveState::EMS_CanMoving;
}



