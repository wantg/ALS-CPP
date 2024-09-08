#include "ALS/GameModes/ALSGameMode.h"
#include "ALS/Controllers/ALSPlayerController.h"
#include "ALS/Characters/ALSAnimManCharacter.h"

AALSGameMode::AALSGameMode() {
    PlayerControllerClass = AALSPlayerController::StaticClass();
    DefaultPawnClass = AALSAnimManCharacter::StaticClass();
}
