#include "ChatBotFunctionLibrary.h"
#include "ChatBotSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

FString UChatBotFunctionLibrary::FindChatBotResponse(const UObject* WorldContextObject, const FString& UserInput)
{
    if (!WorldContextObject)
    {
        return TEXT("Error: WorldContext nulo.");
    }

    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World)
    {
        return TEXT("Error: No se pudo obtener World.");
    }

    UGameInstance* GI = World->GetGameInstance();
    if (!GI)
    {
        return TEXT("Error: GameInstance no disponible.");
    }

    UChatBotSubsystem* ChatBot = GI->GetSubsystem<UChatBotSubsystem>();
    if (!ChatBot)
    {
        return TEXT("Error: ChatBotSubsystem no encontrado.");
    }

    return ChatBot->FindResponse(UserInput);
}

void UChatBotFunctionLibrary::OpenChatBot(const UObject* WorldContextObject)
{
    if (!WorldContextObject)
    {
        UE_LOG(LogTemp, Error, TEXT("ChatBot: OpenChatBot failed - WorldContext is null"));
        return;
    }

    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("ChatBot: OpenChatBot failed - no World"));
        return;
    }

    UGameInstance* GI = World->GetGameInstance();
    if (!GI)
    {
        UE_LOG(LogTemp, Error, TEXT("ChatBot: OpenChatBot failed - no GameInstance"));
        return;
    }

    UChatBotSubsystem* ChatBot = GI->GetSubsystem<UChatBotSubsystem>();
    if (!ChatBot)
    {
        UE_LOG(LogTemp, Error, TEXT("ChatBot: OpenChatBot failed - ChatBotSubsystem not found"));
        return;
    }

    ChatBot->OpenChatBotWidget();
}

void UChatBotFunctionLibrary::CloseChatBot(const UObject* WorldContextObject)
{
    if (!WorldContextObject)
    {
        return;
    }

    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World)
    {
        return;
    }

    if (UGameInstance* GI = World->GetGameInstance())
    {
        if (UChatBotSubsystem* ChatBot = GI->GetSubsystem<UChatBotSubsystem>())
        {
            ChatBot->CloseChatBotWidget();
        }
    }
}
