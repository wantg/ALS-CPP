#include "ALS/GameModes/ALSGameMode.h"
#include "ALS/Controllers/ALSPlayerController.h"
#include "ALS/Characters/AnimManCharacter.h"

AALSGameMode::AALSGameMode() {
    PlayerControllerClass = AALSPlayerController::StaticClass();
    DefaultPawnClass = AAnimManCharacter::StaticClass();
}