#include "MiniJuegoRedes.h"

AMiniJuegoRedes::AMiniJuegoRedes()
{
    PrimaryActorTick.bCanEverTick = false;
    CorrectSequence = { "Modem", "Router", "PC" };
}

void AMiniJuegoRedes::AddItem(const FString& Item)
{
    PlayerSequence.Add(Item);
    OnItemAdded.Broadcast(Item);
}

bool AMiniJuegoRedes::CheckSequence()
{
    if (PlayerSequence.Num() != CorrectSequence.Num())
    {
        OnSequenceWrong.Broadcast(TEXT("Cantidad incorrecta de elementos."));
        return false;
    }

    for (int32 i = 0; i < CorrectSequence.Num(); ++i)
    {
        if (PlayerSequence[i] != CorrectSequence[i])
        {
            OnSequenceWrong.Broadcast(FString::Printf(TEXT("Error en posicion %d: esperado '%s', recibido '%s'."), i + 1, *CorrectSequence[i], *PlayerSequence[i]));
            return false;
        }
    }

    OnSequenceCorrect.Broadcast(TEXT("Secuencia correcta!"));
    return true;
}

void AMiniJuegoRedes::ResetGame()
{
    PlayerSequence.Empty();
}

FString AMiniJuegoRedes::GetCurrentSequenceText() const
{
    return FString::Join(PlayerSequence, TEXT(" -> "));
}
