
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"


UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};


class KATANA02_API IHitInterface
{
	GENERATED_BODY()

public:

	//UFUNCTION(BlueprintNativeEvent)
	virtual void GetHit(const FVector& ImpactPoint) = 0;
};
