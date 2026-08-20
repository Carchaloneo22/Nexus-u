#include "ChatBotWidget.h"
#include "ChatBotSubsystem.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Engine/GameInstance.h"

void UChatBotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (BTN_Enviar)
        BTN_Enviar->OnClicked.AddDynamic(this, &UChatBotWidget::OnSendClicked);

    if (TXT_Input)
    {
        TXT_Input->OnTextCommitted.AddDynamic(this, &UChatBotWidget::OnInputTextCommitted);
        if (APlayerController* PC = GetOwningPlayer())
        {
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(TXT_Input->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
        }
    }

    if (UChatBotSubsystem* ChatBot = GetGameInstance()->GetSubsystem<UChatBotSubsystem>())
        ChatBot->OnGeminiResponse.AddDynamic(this, &UChatBotWidget::OnGeminiResponse);

    if (TXT_Output)
        TXT_Output->SetText(FText::FromString(TEXT("Asistente UTO listo. Escribe tu pregunta...")));
}

void UChatBotWidget::OnSendClicked()
{
    if (!TXT_Input || !TXT_Output) return;

    FString UserInput = TXT_Input->GetText().ToString().TrimStartAndEnd();
    if (UserInput.IsEmpty()) return;

    FString CurrentText = TXT_Output->GetText().ToString();
    TXT_Output->SetText(FText::FromString(CurrentText + TEXT("\n\nTu: ") + UserInput));

    TXT_Input->SetText(FText::FromString(TEXT("")));

    if (APlayerController* PC = GetOwningPlayer())
    {
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(TXT_Input->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputMode);
    }

    PendingInput = UserInput;
    if (UChatBotSubsystem* ChatBot = GetGameInstance()->GetSubsystem<UChatBotSubsystem>())
        ChatBot->FindResponseOrGemini(UserInput);
}

void UChatBotWidget::OnInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
    if (CommitMethod == ETextCommit::OnEnter)
        OnSendClicked();
}

void UChatBotWidget::OnGeminiResponse(const FString& Response, bool bSuccess)
{
    if (!TXT_Output) return;

    FString CurrentText = TXT_Output->GetText().ToString();
    FString NewText;
    if (bSuccess)
        NewText = CurrentText + TEXT("\nBot: ") + Response;
    else
        NewText = CurrentText + TEXT("\n[Error] ") + Response;

    TXT_Output->SetText(FText::FromString(NewText));
}
