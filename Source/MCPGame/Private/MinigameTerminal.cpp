#include "MinigameTerminal.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AMinigameTerminal::AMinigameTerminal()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = MeshComponent;
    MeshComponent->SetSimulatePhysics(false);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    InteractionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionZone"));
    InteractionZone->SetupAttachment(RootComponent);
    InteractionZone->SetBoxExtent(FVector(100, 100, 100));
    InteractionZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionZone->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AMinigameTerminal::BeginPlay()
{
    Super::BeginPlay();
    InteractionZone->OnComponentBeginOverlap.AddDynamic(this, &AMinigameTerminal::OnOverlapBegin);
    InteractionZone->OnComponentEndOverlap.AddDynamic(this, &AMinigameTerminal::OnOverlapEnd);
}

void AMinigameTerminal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || !OtherActor->IsA(APawn::StaticClass())) return;
    if (ActiveWidget) return;

    UWorld* World = GetWorld();
    if (!World || MinigameWidgetClass.IsNull()) return;

    APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
    if (!PC) return;

    UUserWidget* Widget = CreateWidget<UUserWidget>(PC, MinigameWidgetClass.Get());
    if (!Widget) return;

    Widget->SetOwningPlayer(PC);
    Widget->AddToViewport(100);
    ActiveWidget = Widget;

    PC->SetShowMouseCursor(true);
    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PC->SetInputMode(InputMode);
}

void AMinigameTerminal::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!OtherActor || !OtherActor->IsA(APawn::StaticClass())) return;

    if (ActiveWidget)
    {
        ActiveWidget->RemoveFromParent();
        ActiveWidget = nullptr;
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        PC->SetShowMouseCursor(false);
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
    }
}
