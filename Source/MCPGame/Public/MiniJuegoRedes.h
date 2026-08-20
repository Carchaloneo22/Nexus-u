#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiniJuegoRedes.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSequenceCorrect, FString, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSequenceWrong, FString, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAdded, FString, ItemName);

UCLASS()
class MCPGAME_API AMiniJuegoRedes : public AActor
{
    GENERATED_BODY()

public:
    AMiniJuegoRedes();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Redes")
    TArray<FString> CorrectSequence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Redes")
    TArray<FString> PlayerSequence;

    UFUNCTION(BlueprintCallable, Category = "Redes")
    void AddItem(const FString& Item);

    UFUNCTION(BlueprintCallable, Category = "Redes")
    bool CheckSequence();

    UFUNCTION(BlueprintCallable, Category = "Redes")
    void ResetGame();

    UFUNCTION(BlueprintCallable, Category = "Redes")
    FString GetCurrentSequenceText() const;

    UPROPERTY(BlueprintAssignable, Category = "Redes")
    FOnSequenceCorrect OnSequenceCorrect;

    UPROPERTY(BlueprintAssignable, Category = "Redes")
    FOnSequenceWrong OnSequenceWrong;

    UPROPERTY(BlueprintAssignable, Category = "Redes")
    FOnItemAdded OnItemAdded;
};
