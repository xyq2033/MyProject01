#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"

#include "Katana02/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Components/AttributeComponent.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attrubutes"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack()
{

}

void ABaseCharacter::Die()
{

}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactPoint - GetActorLocation()).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}
	FName Section("FromBack");

	/*if (Theta < -135.f)
	{
		Section = FName("FromBack");
	}*/
	if (Theta >= -70.f && Theta < 70.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -70.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 70.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}
	PlayHitReactMontage(Section);


	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.f);


	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta:%f"), Theta));
	}

	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
}

void ABaseCharacter::PlayAttackMontage()
{

}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::AttackEnd()
{
}



//void ABaseCharacter::Die()
//{
//
//}

//void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
//{
//	
//
//}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon)
	{
		UBoxComponent* WeaponBox = EquippedWeapon->GetWeaponBox();
		if (WeaponBox)
		{
			WeaponBox->SetCollisionEnabled(CollisionEnabled);
			EquippedWeapon->IgnoreActors.Empty();
		}
	}
}
