#include "GridComandos.h"

AGridComandos::AGridComandos()
{
    PrimaryActorTick.bCanEverTick = false;
    GridSize = 5;
    PlayerX = 0;
    PlayerY = 0;
    GoalX = 4;
    GoalY = 4;
}

void AGridComandos::MoveUp()
{
    PlayerY++;
    ClampPosition();
    OnMoved.Broadcast(GetPositionText());
    if (CheckGoal()) { OnGoalReached.Broadcast(TEXT("Meta alcanzada!")); }
}

void AGridComandos::MoveDown()
{
    PlayerY--;
    ClampPosition();
    OnMoved.Broadcast(GetPositionText());
    if (CheckGoal()) { OnGoalReached.Broadcast(TEXT("Meta alcanzada!")); }
}

void AGridComandos::MoveLeft()
{
    PlayerX--;
    ClampPosition();
    OnMoved.Broadcast(GetPositionText());
    if (CheckGoal()) { OnGoalReached.Broadcast(TEXT("Meta alcanzada!")); }
}

void AGridComandos::MoveRight()
{
    PlayerX++;
    ClampPosition();
    OnMoved.Broadcast(GetPositionText());
    if (CheckGoal()) { OnGoalReached.Broadcast(TEXT("Meta alcanzada!")); }
}

bool AGridComandos::CheckGoal() const
{
    return (PlayerX == GoalX && PlayerY == GoalY);
}

void AGridComandos::ResetPosition()
{
    PlayerX = 0;
    PlayerY = 0;
}

FString AGridComandos::GetPositionText() const
{
    return FString::Printf(TEXT("(%d, %d)"), PlayerX, PlayerY);
}

void AGridComandos::ClampPosition()
{
    PlayerX = FMath::Clamp(PlayerX, 0, GridSize - 1);
    PlayerY = FMath::Clamp(PlayerY, 0, GridSize - 1);
}
