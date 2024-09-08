#include "ALS/Characters/ALSBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ALS/Interfaces/ALSController.h"

// ================================================================================
// Debug

/*
 * Get Debug Info from Player Controller
 */
void AALSBaseCharacter::DrawDebugShapes() {
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (IALSController* ALSController = Cast<IALSController>(PlayerController)) {
        // Get Debug Info from Player Controller
        ACharacter* DebugFocusCharacter;
        bool DebugView;
        bool ShowHUD;
        bool ShowTraces;
        bool ShowDebugShapes;
        bool ShowLayerColors;
        bool Slomo;
        bool ShowCharacterInfo;
        ALSController->GetDebugInfo(DebugFocusCharacter, DebugView, ShowHUD, ShowTraces, ShowDebugShapes, ShowLayerColors, Slomo, ShowCharacterInfo);
        if (ShowDebugShapes) {
            // Velocity Arrow
            const FVector l_LineStart1 = GetActorLocation() - FVector{0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()};
            bool VelocityIsZero = UKismetMathLibrary::EqualEqual_VectorVector(GetVelocity(), FVector::ZeroVector, 0.0001f);
            const FVector l_FVector1 = UKismetMathLibrary::Vector_NormalUnsafe(VelocityIsZero ? LastVelocityRotation.Vector() : GetVelocity());
            const float l_Float1 = UKismetMathLibrary::MapRangeClamped(GetVelocity().Size(), 0.0, GetCharacterMovement()->MaxWalkSpeed, 50.0, 75.0);
            const FVector l_LineEnd1 = l_LineStart1 + l_FVector1 * l_Float1;
            const FLinearColor L_Color1 = VelocityIsZero ? FLinearColor{0.25f, 0.f, 0.25f, 1.f} : FLinearColor{1.f, 0.f, 1.f, 1.f};
            UKismetSystemLibrary::DrawDebugArrow(
                this,          // const UObject* WorldContextObject
                l_LineStart1,  // const FVector LineStart
                l_LineEnd1,    // const FVector LineEnd
                60.f,          // float ArrowSize
                L_Color1,      // FLinearColor LineColor
                0.f,           // float Duration=0.f
                5.f            // float Thickness = 0.f
            );
            // Movement Input Arrow
            const FVector l_LineStart2 = GetActorLocation() - FVector{0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 3.f};
            bool CurrentAccelerationIsZero = UKismetMathLibrary::EqualEqual_VectorVector(GetCharacterMovement()->GetCurrentAcceleration(), FVector::ZeroVector, 0.0001f);
            const FVector l_FVector2 = UKismetMathLibrary::Vector_NormalUnsafe(CurrentAccelerationIsZero ? LastMovementInputRotation.Vector() : GetCharacterMovement()->GetCurrentAcceleration());
            const float l_Float2 = UKismetMathLibrary::MapRangeClamped((GetCharacterMovement()->GetCurrentAcceleration()).Size() / GetCharacterMovement()->GetMaxAcceleration(), 0.0, 1.f, 50.0, 75.0);
            const FVector l_LineEnd2 = l_LineStart2 + l_FVector2 * l_Float2;
            const FLinearColor L_Color2 = CurrentAccelerationIsZero ? FLinearColor{0.25f, 0.125f, 0.f, 1.f} : FLinearColor{1.f, 0.5f, 0.f, 1.f};
            UKismetSystemLibrary::DrawDebugArrow(
                this,          // const UObject* WorldContextObject
                l_LineStart2,  // const FVector LineStart
                l_LineEnd2,    // const FVector LineEnd
                50.f,          // float ArrowSize
                L_Color2,      // FLinearColor LineColor
                0.f,           // float Duration=0.f
                3.f            // float Thickness = 0.f
            );
            // Target Rotation Arrow
            const FVector l_LineStart3 = GetActorLocation() - FVector{0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 7.f};
            const FVector l_LineEnd3 = l_LineStart3 + UKismetMathLibrary::Vector_NormalUnsafe(TargetRotation.Vector()) * 50.f;
            UKismetSystemLibrary::DrawDebugArrow(
                this,                                    // const UObject* WorldContextObject
                l_LineStart3,                            // const FVector LineStart
                l_LineEnd3,                              // const FVector LineEnd
                50.f,                                    // float ArrowSize
                FLinearColor{0.f, 0.333333f, 1.f, 1.f},  // FLinearColor LineColor
                0.f,                                     // float Duration=0.f
                3.f                                      // float Thickness = 0.f
            );
            // Aiming Rotation Cone
            const FVector l_Direction3 = UKismetMathLibrary::Vector_NormalUnsafe(GetControlRotation().Vector());
            UKismetSystemLibrary::DrawDebugConeInDegrees(
                this,                                              // const UObject* WorldContextObject
                GetMesh()->GetSocketLocation(FName("FP_Camera")),  // const FVector Origin
                l_Direction3,                                      // const FVector Direction
                100.f,                                             // float Length=100.f
                30.f,                                              // float AngleWidth=45.f
                30.f,                                              // float AngleHeight=45.f
                8,                                                 // int32 NumSides = 12
                FLinearColor{0.f, 0.5f, 1.f, 1.f},                 // FLinearColor LineColor = FLinearColor::White
                0.f,                                               // float Duration=0.f
                0.5f                                               // float Thickness = 0.f
            );
            // Capsule
            UKismetSystemLibrary::DrawDebugCapsule(
                this,                                                 // const UObject* WorldContextObject
                GetActorLocation(),                                   // const FVector Center
                GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),  // float HalfHeight
                GetCapsuleComponent()->GetScaledCapsuleRadius(),      // float Radius
                GetActorRotation(),                                   // const FRotator Rotation
                FLinearColor::Black,                                  // FLinearColor LineColor = FLinearColor::White
                0.f,                                                  // float Duration=0.f
                0.3f                                                  // float Thickness = 0.f
            );
        }
    }
}

TEnumAsByte<EDrawDebugTrace::Type> AALSBaseCharacter::GetTraceDebugType(TEnumAsByte<EDrawDebugTrace::Type> ShowTraceType) {
    if (IALSController* ALSController = Cast<IALSController>(UGameplayStatics::GetPlayerController(this, 0))) {
        ACharacter* DebugFocusCharacter;
        bool DebugView;
        bool ShowHUD;
        bool ShowTraces;
        bool ShowDebugShapes;
        bool ShowLayerColors;
        bool Slomo;
        bool ShowCharacterInfo;
        ALSController->GetDebugInfo(DebugFocusCharacter, DebugView, ShowHUD, ShowTraces, ShowDebugShapes, ShowLayerColors, Slomo, ShowCharacterInfo);
        if (ShowTraces) {
            return ShowTraceType;
        }
    }
    return EDrawDebugTrace::Type::None;
}