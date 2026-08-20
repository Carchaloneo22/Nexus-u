#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridComandos.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoved, FString, PositionText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoalReached, FString, Message);

UCLASS()
class MCPGAME_API AGridComandos : public AActor
{
    GENERATED_BODY()

public:
    AGridComandos();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 GridSize = 5;

    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    int32 PlayerX = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    int32 PlayerY = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 GoalX = 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 GoalY = 4;

    UFUNCTION(BlueprintCallable, Category = "Grid")
    void MoveUp();

    UFUNCTION(BlueprintCallable, Category = "Grid")
    void MoveDown();

    UFUNCTION(BlueprintCallable, Category = "Grid")
    void MoveLeft();

    UFUNCTION(BlueprintCallable, Category = "Grid")
    void MoveRight();

    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CheckGoal() const;

    UFUNCTION(BlueprintCallable, Category = "Grid")
    void ResetPosition();

    UFUNCTION(BlueprintCallable, Category = "Grid")
    FString GetPositionText() const;

    UPROPERTY(BlueprintAssignable, Category = "Grid")
    FOnMoved OnMoved;

    UPROPERTY(BlueprintAssignable, Category = "Grid")
    FOnGoalReached OnGoalReached;

private:
    void ClampPosition();
};
