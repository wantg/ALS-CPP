#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ALSCamera.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UALSCamera : public UInterface {
    GENERATED_BODY()
};

class ALS_API IALSCamera {
    GENERATED_BODY()

   public:
    UFUNCTION(BlueprintCallable, Category = "Camera System")
    virtual void GetCameraParameters(float& TPFOV, float& FPFOV, bool& RightShoulder) = 0;

    UFUNCTION(BlueprintCallable, Category = "Camera System")
    virtual FVector GetFPCameraTarget() = 0;

    UFUNCTION(BlueprintCallable, Category = "Camera System")
    virtual FTransform Get3PPivotTarget() = 0;

    UFUNCTION(BlueprintCallable, Category = "Camera System")
    virtual void Get3PTraceParams(FVector& TraceOrigin, float& TraceRadius, TEnumAsByte<ETraceTypeQuery>& TraceChannel) = 0;
};
