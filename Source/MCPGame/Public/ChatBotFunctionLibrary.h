#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ChatBotFunctionLibrary.generated.h"

UCLASS()
class MCPGAME_API UChatBotFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "ChatBot", meta = (WorldContext = "WorldContextObject"))
    static FString FindChatBotResponse(const UObject* WorldContextObject, const FString& UserInput);

    /** Abre el widget de chat en el viewport del jugador local. */
    UFUNCTION(BlueprintCallable, Category = "ChatBot", meta = (WorldContext = "WorldContextObject"))
    static void OpenChatBot(const UObject* WorldContextObject);

    /** Cierra el widget de chat si esta abierto. */
    UFUNCTION(BlueprintCallable, Category = "ChatBot", meta = (WorldContext = "WorldContextObject"))
    static void CloseChatBot(const UObject* WorldContextObject);
};
