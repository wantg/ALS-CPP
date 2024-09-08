#pragma once

#include "CoreMinimal.h"
#include "ALS/Characters/BaseCharacter.h"
#include "AnimManCharacter.generated.h"

/**
 * IMPORTANT!!!
 *
 * This blueprint is a child of the ALS_Base_CharacterBP and contains dynamic coloring logic for debugging purposes (press 'U' to display). It also provides placeholder functionality for equiping a held object (not gameplay ready). There is currently no inventory or weapon management system.
 *
 * Its other primary function is to override certian functions within the Base_Character to update the camera target information and to properly select montages based on the current Overlay State.
 *
 * All Overriden Functions:
 *
 *   - Get3PPivotTarget
 *   - GetFPCameraTarget
 *   - Get3PTraceParams
 *
 *   - OnOverlayStateChanged
 *
 *   - GetRollAnimation
 *
 *   - GetMantleAsset
 *   - MantleEnd
 *   - MantleStart
 *
 *   - GetGetUpAnimation
 *   - RagdollStart
 *   - RagdollEnd
 */
UCLASS()
class ALS_API AAnimManCharacter : public ABaseCharacter {
    GENERATED_BODY()

   public:
    AAnimManCharacter();
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // ================================================================================
    // IALSCamera
    virtual void Get3PTraceParams(FVector& TraceOrigin, float& TraceRadius, TEnumAsByte<ETraceTypeQuery>& TraceChannel) override;
    virtual FTransform Get3PPivotTarget() override;
    virtual FVector GetFPCameraTarget() override;

    // ================================================================================
    // Coloring System
    void SetDynamicMaterials();
    void SetResetColors();
    void UpdateColoringSystem();
    void UpdateLayeringColors();

    // ================================================================================
    // Held Object
    void UpdateHeldObject();
    void ClearHeldObject();
    void AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh, UClass* NewAnimClass, bool LeftHand, FVector Offset);
    void UpdateHeldObjectAnimations();
    // ================================================================================
    // State Changes
    void OnOverlayStateChanged(EALSOverlayState NewOverlayState) override;

    // ================================================================================
    // Movement System
    UAnimMontage* GetRollAnimation() override;

    // ================================================================================
    // Mantle System
    void MantleStart(float MantleHeight, FALSComponentAndTransform MantleLedgeWS, EMantleType MantleType) override;
    void MantleEnd() override;
    FMantleAsset GetMantleAsset(EMantleType MantleType) override;

    // ================================================================================
    // Ragdoll System
    void RagdollStart() override;
    void RagdollEnd() override;
    UAnimMontage* GetGetUpAnimation(bool InRagdollFaceUp) override;

   public:
    // ================================================================================
    // Components
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
    USceneComponent* VisualMeshes;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
    USkeletalMeshComponent* BodyMesh;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
    USceneComponent* HeldObjectRoot;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
    USkeletalMeshComponent* SkeletalMesh;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* StaticMesh;

    // ================================================================================
    // Colors
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Colors")
    FLinearColor DefaultColor = FLinearColor{0.2f, 0.2f, 1.f, 1.f};

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Colors")
    FLinearColor SkinColor = FLinearColor{0.760525f, 0.376263f, 0.250159f, 1.f};

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Colors")
    FLinearColor ShirtColor = FLinearColor{0.5625f, 0.375f, 0.75f, 1.f};

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Colors")
    FLinearColor PantsColor = FLinearColor{0.5f, 0.2f, 0.05f, 1.f};

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Colors")
    FLinearColor ShoesColor = FLinearColor{0.75f, 0.75f, 0.75f, 1.f};

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Colors")
    FLinearColor GlovesColor = FLinearColor{0.026042, 0.026042, 0.026042, 1.f};

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Colors")
    int32 ShirtType = 2;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Colors")
    int32 PantsType = 2;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Colors")
    bool Shoes = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Colors")
    bool Gloves = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Colors")
    bool SolidColor = true;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Colors")
    FLinearColor BaseLayerColor = FLinearColor{0.f, 0.f, 0.f, 1.f};

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Colors")
    FLinearColor OverlayLayerColor = FLinearColor{1.f, 1.f, 1.f, 1.f};

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Colors")
    FLinearColor AdditiveAmount_Color = FLinearColor{1.f, 0.f, 0.f, 1.f};

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Colors")
    FLinearColor Hand_Color = FLinearColor{0.f, 0.666667, 1.f, 1.f};

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Colors")
    FLinearColor HandIK_Color = FLinearColor{0.f, 1.f, 0.f, 1.f};

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Colors")
    UMaterialInstanceDynamic* Head;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Colors")
    UMaterialInstanceDynamic* Torso;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Colors")
    UMaterialInstanceDynamic* Pelvis;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Colors")
    UMaterialInstanceDynamic* Feet;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Colors")
    UMaterialInstanceDynamic* Shoulder_L;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Colors")
    UMaterialInstanceDynamic* UpperArm_L;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Colors")
    UMaterialInstanceDynamic* LowerArm_L;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Colors")
    UMaterialInstanceDynamic* Hand_L;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Colors")
    UMaterialInstanceDynamic* Shoulder_R;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Colors")
    UMaterialInstanceDynamic* UpperArm_R;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Colors")
    UMaterialInstanceDynamic* LowerArm_R;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Colors")
    UMaterialInstanceDynamic* Hand_R;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Colors")
    UMaterialInstanceDynamic* UpperLegs;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Colors")
    UMaterialInstanceDynamic* LowerLegs;

    // ================================================================================
    // Mantle Animations
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mantle Animations")
    FMantleAsset Mantle_2m_Default_0;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mantle Animations")
    FMantleAsset Mantle_1m_Default_0;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mantle Animations")
    FMantleAsset Mantle_1m_RH_0;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mantle Animations")
    FMantleAsset Mantle_1m_LH_0;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mantle Animations")
    FMantleAsset Mantle_1m_2H_0;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mantle Animations")
    FMantleAsset Mantle_1m_Box_0;

    // ================================================================================
    // Roll Animations
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Roll Animations")
    UAnimMontage* LandRoll_Default;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Roll Animations")
    UAnimMontage* LandRoll_RH;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Roll Animations")
    UAnimMontage* LandRoll_LH;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Roll Animations")
    UAnimMontage* LandRoll_2H;

    // ================================================================================
    // Get Up Animations
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Get Up Animations")
    UAnimMontage* GetUpFront_Default;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Get Up Animations")
    UAnimMontage* GetUpFront_RH;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Get Up Animations")
    UAnimMontage* GetUpFront_LH;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Get Up Animations")
    UAnimMontage* GetUpFront_2H;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Get Up Animations")
    UAnimMontage* GetUpBack_Default;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Get Up Animations")
    UAnimMontage* GetUpBack_RH;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Get Up Animations")
    UAnimMontage* GetUpBack_LH;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Get Up Animations")
    UAnimMontage* GetUpBack_2H;

   private:
    void MKeyPressed();

    bool MKeyFlipFlop = false;
};
