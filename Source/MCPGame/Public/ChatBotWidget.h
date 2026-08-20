#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatBotWidget.generated.h"

UCLASS()
class MCPGAME_API UChatBotWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* TXT_Input;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TXT_Output;

    UPROPERTY(meta = (BindWidget))
    class UButton* BTN_Enviar;

protected:
    UFUNCTION()
    void OnSendClicked();

    UFUNCTION()
    void OnInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

    UFUNCTION()
    void OnGeminiResponse(const FString& Response, bool bSuccess);

    FString PendingInput;
};
