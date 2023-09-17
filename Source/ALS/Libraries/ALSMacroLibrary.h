#pragma once

#define SetPreviousAndNewValue(NewValue, NewTarget, PreviousTarget) \
    {                                                               \
        PreviousTarget = NewTarget;                                 \
        NewTarget = NewValue;                                       \
    }

#define GetNextArrayItem(Array, CurrentItem)                                        \
    {                                                                               \
        int32 CurrentIndex = Array.Find(CurrentItem);                               \
        int32 ItemIndex = (CurrentIndex == Array.Num() - 1) ? 0 : CurrentIndex + 1; \
        if (ItemIndex > -1) {                                                       \
            CurrentItem = Array[ItemIndex];                                         \
        }                                                                           \
    }

#define GetPreviousArrayItem(Array, CurrentItem)                                  \
    {                                                                             \
        int32 CurrentIndex = Array.Find(CurrentItem);                             \
        int32 ItemIndex = CurrentIndex <= 0 ? Array.Num() - 1 : CurrentIndex - 1; \
        if (ItemIndex > -1) {                                                     \
            CurrentItem = Array[ItemIndex];                                       \
        }                                                                         \
    }

#define RetriggerDelay(Duration, FunctionName)                                  \
    {                                                                           \
        UKismetSystemLibrary::K2_ClearTimer(this, FunctionName);                \
        UKismetSystemLibrary::K2_SetTimer(this, FunctionName, Duration, false); \
    }

#define AddInputActionBindingLambda(InputComponent, Name, InputEvent, Lambda) \
    {                                                                         \
        FInputActionBinding Binding(Name, InputEvent);                        \
        Binding.ActionDelegate.GetDelegateForManualSet().BindLambda(Lambda);  \
        InputComponent->AddActionBinding(Binding);                            \
    }

#define ComponentWorldToLocal(ComponentAndTransform)                                                         \
    {                                                                                                        \
        ComponentAndTransform.Transform *ComponentAndTransform.Component->GetComponentTransform().Inverse(), \
            ComponentAndTransform.Component                                                                  \
    }

#define ComponentLocalToWorld(ComponentAndTransform)                                               \
    {                                                                                              \
        ComponentAndTransform.Transform *ComponentAndTransform.Component->GetComponentTransform(), \
            ComponentAndTransform.Component                                                        \
    }

#define TransformSubtractionTransform(A, B)                                        \
    {                                                                              \
        FTransform {                                                               \
            FRotator{                                                              \
                A.GetRotation().Rotator().Pitch - B.GetRotation().Rotator().Pitch, \
                A.GetRotation().Rotator().Yaw - B.GetRotation().Rotator().Yaw,     \
                A.GetRotation().Rotator().Roll - B.GetRotation().Rotator().Roll,   \
            },                                                                     \
                A.GetLocation() - B.GetLocation(),                                 \
                A.GetScale3D() - B.GetScale3D()                                    \
        }                                                                          \
    }

#define TransformAdditionTransform(A, B)                                           \
    {                                                                              \
        FTransform {                                                               \
            FRotator{                                                              \
                A.GetRotation().Rotator().Pitch + B.GetRotation().Rotator().Pitch, \
                A.GetRotation().Rotator().Yaw + B.GetRotation().Rotator().Yaw,     \
                A.GetRotation().Rotator().Roll + B.GetRotation().Rotator().Roll,   \
            },                                                                     \
                A.GetLocation() + B.GetLocation(),                                 \
                A.GetScale3D() + B.GetScale3D()                                    \
        }                                                                          \
    }

#define AddTimer(Lambda, ...)                                                                                     \
    {                                                                                                             \
        FTimerHandle TimerHandle;                                                                                 \
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(Lambda), ##__VA_ARGS__); \
    }