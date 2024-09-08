#include "ALS/Controllers/ALSPlayerController.h"
#include "ALS/CameraSystem/ALSPlayerCameraManager.h"
#include "ALS/Characters/ALSAnimManCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/InputSettings.h"
#include "ALS/Libraries/ALSMacroLibrary.h"

AALSPlayerController::AALSPlayerController() {
    PlayerCameraManagerClass = AALSPlayerCameraManager::StaticClass();
}

void AALSPlayerController::BeginPlay() {
    Super::BeginPlay();

    // Create and add HUD to Viewport
    ALSHUD = CreateWidget<UUserWidget>(this, LoadClass<UUserWidget>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Blueprints/UI/ALS_HUD.ALS_HUD_C")));
    ALSHUD->SetOwningPlayer(this);
    ALSHUD->AddToViewport();

    // Search for all ALS Characters and populate array. Used to switch target character when viewing character info in the HUD
    TArray<AActor*> OutActors;
    UGameplayStatics::GetAllActorsOfClass(this, AALSAnimManCharacter::StaticClass(), OutActors);
    for (AActor* i : OutActors) {
        if (ACharacter* L_Character = Cast<ACharacter>(i)) {
            AvailableDebugCharacters.Add(L_Character);
        }
    }
    DebugFocusCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

    // Add input mapping
    PlayerInput->AddActionMapping(FInputActionKeyMapping{FName("TabKeyPressed"), EKeys::Tab});
    PlayerInput->AddActionMapping(FInputActionKeyMapping{FName("VKeyPressed"), EKeys::V});
    PlayerInput->AddActionMapping(FInputActionKeyMapping{FName("TKeyPressed"), EKeys::T});
    PlayerInput->AddActionMapping(FInputActionKeyMapping{FName("YKeyPressed"), EKeys::Y});
    PlayerInput->AddActionMapping(FInputActionKeyMapping{FName("UKeyPressed"), EKeys::U});
    PlayerInput->AddActionMapping(FInputActionKeyMapping{FName("IKeyPressed"), EKeys::I});
    PlayerInput->AddActionMapping(FInputActionKeyMapping{FName("ZKeyPressed"), EKeys::Z});
    PlayerInput->AddActionMapping(FInputActionKeyMapping{FName("CommaKeyPressed"), EKeys::Comma});
    PlayerInput->AddActionMapping(FInputActionKeyMapping{FName("PeriodKeyPressed"), EKeys::Period});
}

void AALSPlayerController::OnPossess(APawn* NewPawn) {
    Super::OnPossess(NewPawn);

    // Call "OnPossess" in ALS Player Camera Manager when possessing a pawn
    if (AALSPlayerCameraManager* ALSPlayerCameraManager = Cast<AALSPlayerCameraManager>(PlayerCameraManager)) {
        ALSPlayerCameraManager->OnPossess(NewPawn);
    }

    // Bind keys
    NewPawn->InputComponent->BindAction("TabKeyPressed", IE_Pressed, this, &AALSPlayerController::TabKeyPressed);
    NewPawn->InputComponent->BindAction("VKeyPressed", IE_Pressed, this, &AALSPlayerController::VKeyPressed);
    NewPawn->InputComponent->BindAction("TKeyPressed", IE_Pressed, this, &AALSPlayerController::TKeyPressed);
    NewPawn->InputComponent->BindAction("YKeyPressed", IE_Pressed, this, &AALSPlayerController::YKeyPressed);
    NewPawn->InputComponent->BindAction("UKeyPressed", IE_Pressed, this, &AALSPlayerController::UKeyPressed);
    NewPawn->InputComponent->BindAction("IKeyPressed", IE_Pressed, this, &AALSPlayerController::IKeyPressed);
    NewPawn->InputComponent->BindAction("ZKeyPressed", IE_Pressed, this, &AALSPlayerController::ZKeyPressed);
    NewPawn->InputComponent->BindAction("CommaKeyPressed", IE_Pressed, this, &AALSPlayerController::CommaKeyPressed);
    NewPawn->InputComponent->BindAction("PeriodKeyPressed", IE_Pressed, this, &AALSPlayerController::PeriodKeyPressed);
    
    FInputActionBinding OpenOverlayMenuPressed("OpenOverlayMenu", IE_Pressed);
    OpenOverlayMenuPressed.ActionDelegate.GetDelegateForManualSet().BindLambda([&]() { OpenOverlayMenu(IE_Pressed); });
    NewPawn->InputComponent->AddActionBinding(OpenOverlayMenuPressed);

    FInputActionBinding OpenOverlayMenuReleased("OpenOverlayMenu", IE_Released);
    OpenOverlayMenuReleased.ActionDelegate.GetDelegateForManualSet().BindLambda([&]() { OpenOverlayMenu(IE_Released); });
    NewPawn->InputComponent->AddActionBinding(OpenOverlayMenuReleased);
    
    NewPawn->InputComponent->BindAction("CycleOverlayUp", IE_Pressed, this, &AALSPlayerController::CycleOverlayUp);
    NewPawn->InputComponent->BindAction("CycleOverlayDown", IE_Pressed, this, &AALSPlayerController::CycleOverlayDown);
}

void AALSPlayerController::TabKeyPressed() { ShowHUD = !ShowHUD; }

void AALSPlayerController::VKeyPressed() { DebugView = !DebugView; }

void AALSPlayerController::TKeyPressed() { ShowTraces = !ShowTraces; }

void AALSPlayerController::YKeyPressed() { ShowDebugShapes = !ShowDebugShapes; }

void AALSPlayerController::UKeyPressed() { ShowLayerColors = !ShowLayerColors; }

void AALSPlayerController::IKeyPressed() { ShowCharacterInfo = !ShowCharacterInfo; }

void AALSPlayerController::ZKeyPressed() {
    Slomo = !Slomo;
    UGameplayStatics::SetGlobalTimeDilation(this, Slomo ? 0.15f : 1.f);
}

void AALSPlayerController::CommaKeyPressed() {
    GetPreviousArrayItem(AvailableDebugCharacters, DebugFocusCharacter);
}

void AALSPlayerController::PeriodKeyPressed() {
    GetNextArrayItem(AvailableDebugCharacters, DebugFocusCharacter);
}

void AALSPlayerController::OpenOverlayMenu(EInputEvent InputEvent) {
    if (InputEvent == IE_Pressed) {
        OverlayMenuOpen = true;
        if (!Slomo) {
            UGameplayStatics::SetGlobalTimeDilation(this, 0.35f);
        }
        OverlaySwitcher = CreateWidget<UUserWidget>(this, LoadClass<UUserWidget>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Blueprints/UI/OverlayStateSwitcher.OverlayStateSwitcher_C")));
        OverlaySwitcher->SetOwningPlayer(this);
        OverlaySwitcher->AddToViewport();
        UGameplayStatics::PlaySound2D(this, LoadObject<USoundWave>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Audio/UI/Click")), 1.25f, 1.f, 0.2f, nullptr, nullptr, true);
        return;
    }
    if (InputEvent == IE_Released) {
        OverlayMenuOpen = false;
        if (!Slomo) {
            UGameplayStatics::SetGlobalTimeDilation(this, 1.f);
        }
        UFunction* Func = OverlaySwitcher->FindFunction(FName("SelectOverlayState"));
        if (Func != NULL && Func->IsInBlueprint()) {
            OverlaySwitcher->ProcessEvent(Func, nullptr);
        }
        OverlaySwitcher->RemoveFromParent();
        UGameplayStatics::PlaySound2D(this, LoadObject<USoundWave>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Audio/UI/Click")), 1.25f, 1.f, 0.2f, nullptr, nullptr, true);
        return;
    }
}

void AALSPlayerController::CycleOverlayUp() {
    if (OverlayMenuOpen && OverlaySwitcher) {
        UFunction* Func = OverlaySwitcher->FindFunction(FName("CycleState"));
        if (Func != NULL && Func->IsInBlueprint()) {
            struct Parameter {
                bool Up;
            };
            Parameter Parameter{true};
            OverlaySwitcher->ProcessEvent(Func, &Parameter);
        }
    }
}

void AALSPlayerController::CycleOverlayDown() {
    if (OverlayMenuOpen && OverlaySwitcher) {
        UFunction* Func = OverlaySwitcher->FindFunction(FName("CycleState"));
        if (Func != NULL && Func->IsInBlueprint()) {
            struct Parameter {
                bool Up;
            };
            Parameter Parameter{false};
            OverlaySwitcher->ProcessEvent(Func, &Parameter);
        }
    }
}
