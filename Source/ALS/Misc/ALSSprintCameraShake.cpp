#include "ALS/Misc/ALSSprintCameraShake.h"

UALSSprintCameraShake::UALSSprintCameraShake() {
    OscillationDuration     = 0.2f;
    OscillationBlendInTime  = 0.1f;
    OscillationBlendOutTime = 0.1f;

    RotOscillation.Pitch.Amplitude     = 0.1f;
    RotOscillation.Pitch.Frequency     = 0.1f;
    RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
    RotOscillation.Pitch.Waveform      = EOscillatorWaveform::SineWave;

    RotOscillation.Yaw.Amplitude     = 0.1f;
    RotOscillation.Yaw.Frequency     = 0.1f;
    RotOscillation.Yaw.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
    RotOscillation.Yaw.Waveform      = EOscillatorWaveform::SineWave;

    RotOscillation.Roll.Amplitude     = 0.1f;
    RotOscillation.Roll.Frequency     = 0.1f;
    RotOscillation.Roll.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
    RotOscillation.Roll.Waveform      = EOscillatorWaveform::SineWave;

    LocOscillation.X.Amplitude     = 0.f;
    LocOscillation.X.Frequency     = 0.f;
    LocOscillation.X.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
    LocOscillation.X.Waveform      = EOscillatorWaveform::SineWave;

    LocOscillation.Y.Amplitude     = 0.f;
    LocOscillation.Y.Frequency     = 0.f;
    LocOscillation.Y.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
    LocOscillation.Y.Waveform      = EOscillatorWaveform::SineWave;

    LocOscillation.Z.Amplitude     = 0.f;
    LocOscillation.Z.Frequency     = 0.f;
    LocOscillation.Z.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
    LocOscillation.Z.Waveform      = EOscillatorWaveform::SineWave;

    FOVOscillation.Amplitude     = 0.f;
    FOVOscillation.Frequency     = 0.f;
    FOVOscillation.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
    FOVOscillation.Waveform      = EOscillatorWaveform::SineWave;

    AnimPlayRate     = 1.f;
    AnimScale        = 1.f;
    AnimBlendInTime  = 0.2f;
    AnimBlendOutTime = 0.2f;
}
