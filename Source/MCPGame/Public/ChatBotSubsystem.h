#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChatBotSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGeminiResponse, const FString&, Response, bool, bSuccess);

USTRUCT()
struct FChatBotFAQEntry
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FString> Keywords;

    UPROPERTY()
    FString Response;

    UPROPERTY()
    FString Category;
};

UCLASS()
class MCPGAME_API UChatBotSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category = "ChatBot")
    void LoadFAQsFromJson(const FString& JsonFilePath);

    UFUNCTION(BlueprintCallable, Category = "ChatBot")
    FString FindResponse(const FString& UserInput);

    UFUNCTION(BlueprintCallable, Category = "ChatBot")
    void FindResponseOrGemini(const FString& UserInput);

    UPROPERTY(BlueprintAssignable, Category = "ChatBot")
    FOnGeminiResponse OnGeminiResponse;

    UFUNCTION(BlueprintCallable, Category = "ChatBot")
    void OpenChatBotWidget();

    UFUNCTION(BlueprintCallable, Category = "ChatBot")
    void CloseChatBotWidget();

private:
    UPROPERTY()
    TArray<FChatBotFAQEntry> FAQEntries;

    UPROPERTY()
    TObjectPtr<UUserWidget> ActiveChatWidget;

    TArray<FString> UserHistory;
    TArray<FString> ResponseHistory;
    FString LastCategory;
    int32 ConversationTurn = 0;
    bool bLastWasFallback = false;

    bool IsVagueFollowUp(const FString& NormalizedInput) const;
    FString GetExpandedResponse();
    FString GetFollowUpSuggestions(const FString& Category);
    int32 FindBestMatchInCategory(const FString& Category, const FString& NormalizedInput, int32 ExcludeIndex) const;

    bool bLoaded = false;
};
