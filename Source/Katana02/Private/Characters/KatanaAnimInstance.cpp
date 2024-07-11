#include "Characters/KatanaAnimInstance.h"
#include "Characters/KatanaCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UKatanaAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	KatanaCharacter = Cast<AKatanaCharacter>(TryGetPawnOwner());
	if (KatanaCharacter)
	{
		KatanaCharacterMovement = KatanaCharacter->GetCharacterMovement();
	}
}

void UKatanaAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (KatanaCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(KatanaCharacterMovement->Velocity);
		IsFalling = KatanaCharacterMovement->IsFalling();
		
		CharacterState = KatanaCharacter->GetCharacterState();


		FVector Velocity = KatanaCharacterMovement->Velocity;

		if (!Velocity.IsNearlyZero())
		{
			FVector MovementDirection = Velocity.GetSafeNormal();

			MovementDirectionAngle = FMath::Atan2(MovementDirection.Y, MovementDirection.X);

			MovementDirectionAngle = FMath::RadiansToDegrees(MovementDirectionAngle);
		}
		else
		{
			MovementDirectionAngle = 0.0f;
		}
	}
}

