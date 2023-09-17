#include "ALS/AnimNotifys/FootstepNotify.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/AudioComponent.h"

UFootstepNotify::UFootstepNotify() {
    NotifyColor = FColor{243, 167, 0, 255};
    Sound = LoadObject<USoundCue>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Audio/Footsteps/Footstep_Cue"));
}

void UFootstepNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
    if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance()) {
        float L_VolumeMultiplier;
        if (OverrideMaskCurve) {
            L_VolumeMultiplier = VolumeMultiplier;
        } else {
            float CurveValue = AnimInstance->GetCurveValue(FName("Mask_FootstepSound"));
            L_VolumeMultiplier = (1.f - CurveValue) * VolumeMultiplier;
        }
        if (Sound) {
            UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAttached(
                Sound,                                // USoundBase* Sound
                MeshComp,                             // USceneComponent* AttachToComponent
                AttachPointName,                      // FName AttachPointName
                FVector::ZeroVector,                  // FVector Location
                EAttachLocation::KeepRelativeOffset,  // EAttachLocation::Type LocationType = EAttachLocation::KeepRelativeOffset
                true,                                 // bool bStopWhenAttachedToDestroyed = false
                L_VolumeMultiplier,                   // float VolumeMultiplier = 1.f
                PitchMultiplier                       // float PitchMultiplier = 1.f
                                                      // float StartTime = 0.f
                                                      // USoundAttenuation* AttenuationSettings = nullptr
                                                      // USoundConcurrency* ConcurrencySettings = nullptr
                                                      // bool bAutoDestroy = true
            );
            if (AudioComponent) {
                AudioComponent->SetIntParameter(FName("FootstepType"), (int32)FootstepType);
            }
        }
    }
}
