#include "PSeIntQuiz.h"

APSeIntQuiz::APSeIntQuiz()
{
    PrimaryActorTick.bCanEverTick = false;
}

void APSeIntQuiz::StartQuiz()
{
    CurrentQuestionIndex = 0;
    Score = 0;
}

bool APSeIntQuiz::AnswerQuestion(int32 SelectedIndex)
{
    if (!Questions.IsValidIndex(CurrentQuestionIndex))
    {
        return false;
    }

    const FPSeIntQuestion& Q = Questions[CurrentQuestionIndex];
    bool bCorrect = (SelectedIndex == Q.CorrectIndex);
    if (bCorrect)
    {
        Score++;
    }

    OnQuestionAnswered.Broadcast(bCorrect ? TEXT("Correcto") : TEXT("Incorrecto"));

    CurrentQuestionIndex++;
    if (IsQuizFinished())
    {
        OnQuizFinished.Broadcast(FString::Printf(TEXT("Quiz terminado. Puntaje: %d/%d"), Score, Questions.Num()));
    }

    return bCorrect;
}

FPSeIntQuestion APSeIntQuiz::GetCurrentQuestion() const
{
    if (Questions.IsValidIndex(CurrentQuestionIndex))
    {
        return Questions[CurrentQuestionIndex];
    }
    return FPSeIntQuestion();
}

bool APSeIntQuiz::IsQuizFinished() const
{
    return CurrentQuestionIndex >= Questions.Num();
}

void APSeIntQuiz::ResetQuiz()
{
    CurrentQuestionIndex = 0;
    Score = 0;
}
