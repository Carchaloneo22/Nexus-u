#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinigameTerminal.generated.h"

UCLASS()
class MCPGAME_API AMinigameTerminal : public AActor
{
    GENERATED_BODY()

public:
    AMinigameTerminal();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terminal")
    TObjectPtr<AActor> MinigameActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terminal")
    TSoftClassPtr<UUserWidget> MinigameWidgetClass;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UPROPERTY()
    TObjectPtr<UUserWidget> ActiveWidget;

private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UStaticMeshComponent> MeshComponent;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UBoxComponent> InteractionZone;
};
