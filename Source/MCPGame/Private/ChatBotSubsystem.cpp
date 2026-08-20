#include "ChatBotSubsystem.h"
#include "ChatBotWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "JsonObjectConverter.h"

namespace
{
    FString NormalizeText(const FString& InText)
    {
        FString Result;
        Result.Reserve(InText.Len());

        for (TCHAR C : InText)
        {
            TCHAR Lower = FChar::ToLower(C);
            switch (Lower)
            {
            case TEXT('á'): case TEXT('à'): case TEXT('â'): case TEXT('ä'):
                Lower = TEXT('a'); break;
            case TEXT('é'): case TEXT('è'): case TEXT('ê'): case TEXT('ë'):
                Lower = TEXT('e'); break;
            case TEXT('í'): case TEXT('ì'): case TEXT('î'): case TEXT('ï'):
                Lower = TEXT('i'); break;
            case TEXT('ó'): case TEXT('ò'): case TEXT('ô'): case TEXT('ö'):
                Lower = TEXT('o'); break;
            case TEXT('ú'): case TEXT('ù'): case TEXT('û'): case TEXT('ü'):
                Lower = TEXT('u'); break;
            case TEXT('ñ'):
                Lower = TEXT('n'); break;
            default:
                break;
            }

            if (FChar::IsAlnum(Lower) || FChar::IsWhitespace(Lower))
            {
                if (FChar::IsWhitespace(Lower))
                {
                    if (Result.Len() == 0 || !FChar::IsWhitespace(Result[Result.Len() - 1]))
                    {
                        Result.AppendChar(TEXT(' '));
                    }
                }
                else
                {
                    Result.AppendChar(Lower);
                }
            }
        }

        return Result.TrimStartAndEnd();
    }

    int32 LevenshteinDistance(const FString& A, const FString& B)
    {
        int32 LenA = A.Len(), LenB = B.Len();
        if (LenA == 0) return LenB;
        if (LenB == 0) return LenA;

        TArray<int32> PrevRow, CurrRow;
        PrevRow.SetNumUninitialized(LenB + 1);
        CurrRow.SetNumUninitialized(LenB + 1);

        for (int32 j = 0; j <= LenB; ++j) PrevRow[j] = j;

        for (int32 i = 0; i < LenA; ++i)
        {
            CurrRow[0] = i + 1;
            for (int32 j = 0; j < LenB; ++j)
            {
                int32 Cost = (A[i] == B[j]) ? 0 : 1;
                CurrRow[j + 1] = FMath::Min3(
                    PrevRow[j + 1] + 1,
                    CurrRow[j] + 1,
                    PrevRow[j] + Cost
                );
            }
            Swap(PrevRow, CurrRow);
        }
        return PrevRow[LenB];
    }

    float WordMatchRatio(const FString& Input, const FString& Keyword)
    {
        TArray<FString> InputWords, KeywordWords;
        Input.ParseIntoArrayWS(InputWords);
        Keyword.ParseIntoArrayWS(KeywordWords);

        if (InputWords.Num() == 0 || KeywordWords.Num() == 0) return 0;

        int32 MatchCount = 0;
        for (const FString& KW : KeywordWords)
        {
            if (KW.Len() < 3) continue;
            for (const FString& IW : InputWords)
            {
                if (IW == KW)
                {
                    MatchCount++;
                    break;
                }
                int32 MaxDist = FMath::Max(1, KW.Len() / 5);
                if (KW.Len() > 3 && LevenshteinDistance(IW, KW) <= MaxDist)
                {
                    MatchCount++;
                    break;
                }
            }
        }

        int32 OptionalWords = 0;
        for (const FString& KW : KeywordWords)
        {
            if (KW.Len() < 3) OptionalWords++;
        }
        float EffectiveTotal = (float)(KeywordWords.Num() - OptionalWords);
        if (EffectiveTotal <= 0) return 0;

        return (float)MatchCount / EffectiveTotal;
    }

    FString GetCategoryLabel(const FString& RawCategory)
    {
        if (RawCategory == TEXT("Juego"))        return TEXT("Juego");
        if (RawCategory == TEXT("Precio"))       return TEXT("Precio");
        if (RawCategory == TEXT("Plan"))         return TEXT("Plan de Estudios");
        if (RawCategory == TEXT("Contacto"))     return TEXT("Contacto");
        if (RawCategory == TEXT("Requisitos"))   return TEXT("Requisitos");
        return TEXT("Informacion");
    }
}

void UChatBotSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    const FString DefaultPath = FPaths::ProjectContentDir() / TEXT("Data/ChatBot/FAQ_IS_Completo_UTOriente.json");
    LoadFAQsFromJson(DefaultPath);
}

void UChatBotSubsystem::LoadFAQsFromJson(const FString& JsonFilePath)
{
    FString JsonContent;
    if (!FFileHelper::LoadFileToString(JsonContent, *JsonFilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("ChatBot: Failed to load JSON at %s"), *JsonFilePath);
        return;
    }

    TSharedPtr<FJsonObject> RootObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonContent);
    if (!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("ChatBot: Failed to parse JSON"));
        return;
    }

    const TArray<TSharedPtr<FJsonValue>>* FaqArray = nullptr;
    if (!RootObject->TryGetArrayField(TEXT("faqs"), FaqArray))
    {
        UE_LOG(LogTemp, Error, TEXT("ChatBot: JSON missing 'faqs' array"));
        return;
    }

    FAQEntries.Empty();
    for (const TSharedPtr<FJsonValue>& Value : *FaqArray)
    {
        if (!Value.IsValid()) continue;
        TSharedPtr<FJsonObject> EntryObj = Value->AsObject();
        if (!EntryObj.IsValid()) continue;

        FChatBotFAQEntry Entry;
        Entry.Response = EntryObj->GetStringField(TEXT("response"));
        Entry.Category = EntryObj->GetStringField(TEXT("category"));

        const TArray<TSharedPtr<FJsonValue>>* KeywordsArray = nullptr;
        if (EntryObj->TryGetArrayField(TEXT("keywords"), KeywordsArray))
        {
            for (const TSharedPtr<FJsonValue>& KwValue : *KeywordsArray)
            {
                Entry.Keywords.Add(NormalizeText(KwValue->AsString()));
            }
        }

        FAQEntries.Add(Entry);
    }

    bLoaded = true;
    UE_LOG(LogTemp, Log, TEXT("ChatBot: Loaded %d FAQs"), FAQEntries.Num());
}

FString UChatBotSubsystem::FindResponse(const FString& UserInput)
{
    if (!bLoaded || FAQEntries.Num() == 0)
    {
        return TEXT("Lo siento, el asistente aun no esta listo.");
    }

    const FString NormalizedInput = NormalizeText(UserInput);

    float BestScore = 0;
    int32 BestIndex = 0;

    for (int32 i = 0; i < FAQEntries.Num(); ++i)
    {
        const FChatBotFAQEntry& Entry = FAQEntries[i];
        float Score = 0;

        for (const FString& Keyword : Entry.Keywords)
        {
            int32 Idx = NormalizedInput.Find(Keyword);
            if (Idx != INDEX_NONE)
            {
                bool bBoundaryStart = (Idx == 0 || FChar::IsWhitespace(NormalizedInput[Idx - 1]));
                bool bBoundaryEnd = (Idx + Keyword.Len() >= NormalizedInput.Len() ||
                                     FChar::IsWhitespace(NormalizedInput[Idx + Keyword.Len()]));

                if (Keyword.Len() < 4)
                {
                    if (bBoundaryStart && bBoundaryEnd)
                    {
                        Score += 2.0f;
                    }
                }
                else
                {
                    Score += Keyword.Len() > 4 ? 3.0f : 1.0f;
                    if (bBoundaryStart && bBoundaryEnd) Score += 2.0f;
                }
            }

            float WordRatio = WordMatchRatio(NormalizedInput, Keyword);
            if (WordRatio >= 0.6f)
            {
                Score += WordRatio * 4.0f;
            }
        }

        if (Score > BestScore)
        {
            BestScore = Score;
            BestIndex = i;
        }
    }

    if (BestScore > 0)
    {
        const FChatBotFAQEntry& Best = FAQEntries[BestIndex];
        FString Label = GetCategoryLabel(Best.Category);
        return FString::Printf(TEXT("[%s]\n\n%s"), *Label, *Best.Response);
    }

    return TEXT("");
}

int32 UChatBotSubsystem::FindBestMatchInCategory(const FString& Category, const FString& NormalizedInput, int32 ExcludeIndex) const
{
    float BestScore = 0;
    int32 BestIndex = INDEX_NONE;

    for (int32 i = 0; i < FAQEntries.Num(); ++i)
    {
        if (i == ExcludeIndex) continue;
        if (FAQEntries[i].Category != Category) continue;

        const FChatBotFAQEntry& Entry = FAQEntries[i];
        float Score = 0;
        for (const FString& Keyword : Entry.Keywords)
        {
            if (NormalizedInput.Contains(Keyword))
            {
                Score += Keyword.Len() > 4 ? 3.0f : 1.0f;
            }
            float WordRatio = WordMatchRatio(NormalizedInput, Keyword);
            if (WordRatio >= 0.6f) Score += WordRatio * 4.0f;
        }

        if (Score > BestScore)
        {
            BestScore = Score;
            BestIndex = i;
        }
    }

    return BestIndex;
}

bool UChatBotSubsystem::IsVagueFollowUp(const FString& NormalizedInput) const
{
    const TArray<FString> VagueWords = {
        TEXT("ok"), TEXT("si"), TEXT("sí"), TEXT("dale"), TEXT("bueno"),
        TEXT("okey"), TEXT("va"), TEXT("claro"), TEXT("sigue"), TEXT("continua"),
        TEXT("adelante"), TEXT("dime mas"), TEXT("cuentame"), TEXT("mas"),
        TEXT("explica"), TEXT("prosigue"), TEXT("y"), TEXT("entonces"),
        TEXT("ah"), TEXT("ya"), TEXT("bien"), TEXT("entiendo"), TEXT("comprendo"),
        TEXT("interesante"), TEXT("genial"), TEXT("perfecto"), TEXT("interesado"),
        TEXT("quiero saber mas"), TEXT("cuentame mas"), TEXT("siguiente"),
        TEXT("otro"), TEXT("que mas"), TEXT("mas informacion")
    };

    FString Trimmed = NormalizedInput.TrimStartAndEnd();
    for (const FString& Vague : VagueWords)
    {
        if (Trimmed == Vague) return true;
    }
    return false;
}

FString UChatBotSubsystem::GetExpandedResponse()
{
    if (FAQEntries.Num() == 0 || LastCategory.IsEmpty()) return TEXT("");

    TSet<FString> Shown;
    for (const FString& R : ResponseHistory)
    {
        Shown.Add(R);
    }

    for (int32 i = 0; i < FAQEntries.Num(); ++i)
    {
        const FChatBotFAQEntry& Entry = FAQEntries[i];
        if (Entry.Category != LastCategory) continue;

        FString Formatted = FString::Printf(TEXT("[%s]\n\n%s"), *GetCategoryLabel(Entry.Category), *Entry.Response);
        if (!Shown.Contains(Formatted))
        {
            return Formatted;
        }
    }

    return TEXT("");
}

FString UChatBotSubsystem::GetFollowUpSuggestions(const FString& Category)
{
    if (Category == TEXT("Saludos"))
    {
        return TEXT("\n\n---\nPuedo ayudarte con: informacion de la carrera, plan de estudios, requisitos de inscripcion, precios, o el juego. Que te interesa?");
    }
    if (Category == TEXT("General"))
    {
        return TEXT("\n\n---\nQuieres saber sobre el plan de estudios, los requisitos de inscripcion, los costos, o la modalidad virtual?");
    }
    if (Category == TEXT("Precio"))
    {
        return TEXT("\n\n---\nTambien puedes preguntar por la duracion de la carrera, el plan de estudios, o si hay opciones de financiacion.");
    }
    if (Category == TEXT("Contacto"))
    {
        return TEXT("\n\n---\nNecesitas el numero de WhatsApp, el correo electronico, la direccion, o el PBX?");
    }
    if (Category == TEXT("Requisitos"))
    {
        return TEXT("\n\n---\nTe interesa saber sobre los documentos para inscripcion, las pruebas ICFES, o los requisitos tecnicos de computador?");
    }
    if (Category == TEXT("Plan"))
    {
        return TEXT("\n\n---\nPregunta por las materias de un semestre en especifico, o temas como programacion, IA, redes, base de datos o seguridad.");
    }
    if (Category == TEXT("Juego"))
    {
        return TEXT("\n\n---\nQuieres saber sobre el robot, el puente, el minijuego Network Cabling, los controles, o como explorar las islas?");
    }
    return TEXT("\n\n---\nPregunta sobre la carrera, requisitos, precio, contacto o el juego. O escribe 'ayuda' para ver todas las opciones.");
}

void UChatBotSubsystem::FindResponseOrGemini(const FString& UserInput)
{
    FString NormalizedInput = NormalizeText(UserInput);

    UserHistory.Add(UserInput);
    if (UserHistory.Num() > 10) UserHistory.RemoveAt(0);

    ConversationTurn++;

    if (ConversationTurn > 1 && IsVagueFollowUp(NormalizedInput))
    {
        FString Expanded = GetExpandedResponse();
        if (!Expanded.IsEmpty())
        {
            LastCategory = FAQEntries[0].Category;
            for (const FChatBotFAQEntry& E : FAQEntries)
            {
                FString TestResponse = FString::Printf(TEXT("[%s]\n\n%s"), *GetCategoryLabel(E.Category), *E.Response);
                if (TestResponse == Expanded)
                {
                    LastCategory = E.Category;
                    break;
                }
            }

            ResponseHistory.Add(Expanded);
            if (ResponseHistory.Num() > 10) ResponseHistory.RemoveAt(0);
            bLastWasFallback = false;

            FString FinalResponse = Expanded + GetFollowUpSuggestions(LastCategory);
            OnGeminiResponse.Broadcast(FinalResponse, true);
            return;
        }

        FString NoMore = TEXT("Ya te he contado todo sobre este tema.");
        if (ConversationTurn > 2 && UserHistory.Num() >= 2)
        {
            FString PreviousInput = UserHistory[UserHistory.Num() - 2];
            FString RetryResponse = FindResponse(PreviousInput);
            if (!RetryResponse.IsEmpty())
            {
                NoMore = RetryResponse;
            }
        }
        NoMore += GetFollowUpSuggestions(TEXT(""));
        OnGeminiResponse.Broadcast(NoMore, true);
        return;
    }

    FString Response = FindResponse(UserInput);
    if (!Response.IsEmpty())
    {
        bLastWasFallback = false;
        for (const FChatBotFAQEntry& E : FAQEntries)
        {
            FString TestResponse = FString::Printf(TEXT("[%s]\n\n%s"), *GetCategoryLabel(E.Category), *E.Response);
            if (TestResponse == Response)
            {
                LastCategory = E.Category;
                break;
            }
        }

        ResponseHistory.Add(Response);
        if (ResponseHistory.Num() > 10) ResponseHistory.RemoveAt(0);

        FString FinalResponse = Response + GetFollowUpSuggestions(LastCategory);
        OnGeminiResponse.Broadcast(FinalResponse, true);
        return;
    }

    bLastWasFallback = true;

    FString Fallback = TEXT("No tengo informacion sobre eso.");
    Fallback += TEXT("\n\n---\nIntenta preguntar sobre: la carrera, plan de estudios por semestre, requisitos de inscripcion, precios, contacto, o el juego. O escribe 'ayuda' para ver todo lo que se.");
    OnGeminiResponse.Broadcast(Fallback, true);
}

void UChatBotSubsystem::OpenChatBotWidget()
{
    if (IsValid(ActiveChatWidget))
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("ChatBot: OpenChatBotWidget failed - no World"));
        return;
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
    if (!PC)
    {
        UE_LOG(LogTemp, Error, TEXT("ChatBot: OpenChatBotWidget failed - no PlayerController"));
        return;
    }

    static const FString WidgetPath = TEXT("/Game/UI/ChatBot/WBP_ChatBot_Final.WBP_ChatBot_Final_C");
    UClass* WidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr, *WidgetPath);
    if (!WidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("ChatBot: Failed to load widget class at %s."), *WidgetPath);
        return;
    }

    UChatBotWidget* ChatWidget = CreateWidget<UChatBotWidget>(PC, WidgetClass);
    if (ChatWidget)
    {
        ChatWidget->AddToViewport(100);
        ActiveChatWidget = ChatWidget;
        PC->SetShowMouseCursor(true);
        UE_LOG(LogTemp, Log, TEXT("ChatBot: Widget opened"));
    }
}

void UChatBotSubsystem::CloseChatBotWidget()
{
    if (IsValid(ActiveChatWidget))
    {
        ActiveChatWidget->RemoveFromParent();
        ActiveChatWidget = nullptr;
        UE_LOG(LogTemp, Log, TEXT("ChatBot: Widget closed"));
    }
}
