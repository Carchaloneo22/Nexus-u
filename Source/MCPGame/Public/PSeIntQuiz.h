#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSeIntQuiz.generated.h"

USTRUCT(BlueprintType)
struct FPSeIntQuestion
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString QuestionText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Options;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CorrectIndex = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestionAnswered, FString, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuizFinished, FString, FinalScore);

UCLASS()
class MCPGAME_API APSeIntQuiz : public AActor
{
    GENERATED_BODY()

public:
    APSeIntQuiz();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz")
    TArray<FPSeIntQuestion> Questions;

    UPROPERTY(BlueprintReadOnly, Category = "Quiz")
    int32 CurrentQuestionIndex = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Quiz")
    int32 Score = 0;

    UFUNCTION(BlueprintCallable, Category = "Quiz")
    void StartQuiz();

    UFUNCTION(BlueprintCallable, Category = "Quiz")
    bool AnswerQuestion(int32 SelectedIndex);

    UFUNCTION(BlueprintCallable, Category = "Quiz")
    FPSeIntQuestion GetCurrentQuestion() const;

    UFUNCTION(BlueprintCallable, Category = "Quiz")
    bool IsQuizFinished() const;

    UFUNCTION(BlueprintCallable, Category = "Quiz")
    void ResetQuiz();

    UPROPERTY(BlueprintAssignable, Category = "Quiz")
    FOnQuestionAnswered OnQuestionAnswered;

    UPROPERTY(BlueprintAssignable, Category = "Quiz")
    FOnQuizFinished OnQuizFinished;
};
