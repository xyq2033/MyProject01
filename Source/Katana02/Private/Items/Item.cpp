#include "Items/Item.h"
#include "Components/SphereComponent.h"
#include "Characters/KatanaCharacter.h"
#include "Katana02/DebugMacros.h"


AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;


	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AKatanaCharacter* KatanaCharacter = Cast<AKatanaCharacter>(OtherActor);
	if (KatanaCharacter) 
	{
		KatanaCharacter->SetOverlappingItem(this);
	
	}

}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AKatanaCharacter* KatanaCharacter = Cast<AKatanaCharacter>(OtherActor);
	if (KatanaCharacter)
	{
		KatanaCharacter->SetOverlappingItem(nullptr);

	}
}



void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

