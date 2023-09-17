#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ALS/Interfaces/ALSController.h"
#include "ALS/Misc/InputMapping.h"
#include "ALSPlayerController.generated.h"

/**
 * This controller is responsible for managing the debug settings,
 * displaying the Debug HUD, and managing the Overlay State menu.
 * All other character relevant input can be found within the ALS_Base_CharacterBP.
 */
UCLASS()
class ALS_API AALSPlayerController : public APlayerController, public IALSController {
    GENERATED_BODY()

   public:
    AALSPlayerController();

    // ================================================================================
    // IALSController
    virtual void GetDebugInfo(ACharacter*& OutDebugFocusCharacter, bool& OutDebugView, bool& OutShowHUD, bool& OutShowTraces, bool& OutShowDebugShapes, bool& OutShowLayerColors, bool& OutSlomo, bool& OutShowCharacterInfo) override {
        OutDebugFocusCharacter = DebugFocusCharacter;
        OutDebugView = DebugView;
        OutShowHUD = ShowHUD;
        OutShowTraces = ShowTraces;
        OutShowDebugShapes = ShowDebugShapes;
        OutShowLayerColors = ShowLayerColors;
        OutSlomo = Slomo;
        OutShowCharacterInfo = ShowCharacterInfo && !OverlayMenuOpen;
    }

   protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* NewPawn) override;

   private:
    void TabKeyPressed();
    void VKeyPressed();
    void TKeyPressed();
    void YKeyPressed();
    void UKeyPressed();
    void IKeyPressed();
    void ZKeyPressed();
    void CommaKeyPressed();
    void PeriodKeyPressed();
    void OpenOverlayMenu(EInputEvent InputEvent);
    void CycleOverlayUp();
    void CycleOverlayDown();

   public:
    ACharacter* DebugFocusCharacter;
    TArray<ACharacter*> AvailableDebugCharacters;
    bool ShowHUD = true;
    bool DebugView = false;
    bool ShowTraces = false;
    bool ShowDebugShapes = false;
    bool ShowLayerColors = false;
    bool ShowCharacterInfo = false;
    bool Slomo = false;
    UUserWidget* ALSHUD;
    UUserWidget* OverlaySwitcher;
    bool OverlayMenuOpen = false;
};
