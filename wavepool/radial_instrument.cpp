#include "raylib.h"
#include "vec2.cpp"
#include "ripple_spawner.cpp"

namespace wavepool {
  class RadialInstrument {
    private:
      const int majorPitches [6] = {-9, -4, 0, 3, 5, 10};
      const int minorPitches [6] = {-10, -5, 0, 4, 5, 9};

      RippleSpawner* rippleSpawner;

      Music staticAmbiance;
      Music majorAmbiance;
      Music minorAmbiance;
      Music* currentAmbiance;

      Sound majorTones [4];
      Sound minorTones [4];
      Sound switchTone;

      vec2 centre;
      float margin;
      float outerRadius;
      float innerRadius;

      bool isMajor;
      
      float GetPitch(vec2);
      float GetPanning(vec2);
      void OnCentreClick();
      void OnRadialClick(vec2, vec2);
      bool EllipseContains(vec2, vec2, vec2);
      float EllipseContainsHelper(float, float, float);

    public:
      RadialInstrument(RippleSpawner*, vec2, float, float);
      void LoadSounds();
      void UnloadSounds();
      void Update();
      void DrawGuides(Color, float);
      void OnClick(vec2);
  };

  RadialInstrument::RadialInstrument(RippleSpawner* rippleSpawner, vec2 screenSize,
    float innerRadius, float margin): rippleSpawner{rippleSpawner}
  {
    isMajor = true;

    this->innerRadius = innerRadius;
    this->margin = margin;
    centre = screenSize / 2;
    outerRadius = centre.y;
  }

  void RadialInstrument::LoadSounds()
  {
    staticAmbiance = LoadMusicStream("resources/Ambiance/WavegameAmbienceStaticStereo.ogg");
    staticAmbiance.looping = true;
    majorAmbiance = LoadMusicStream("resources/Ambiance/WavegameAmbianceMajorStereo.ogg");
    majorAmbiance.looping = true;
    minorAmbiance = LoadMusicStream("resources/Ambiance/WavegameAmbianceMinorStereo.ogg");
    minorAmbiance.looping = true;

    PlayMusicStream(staticAmbiance);
    currentAmbiance = &majorAmbiance;
    PlayMusicStream(*currentAmbiance);

    majorTones[0] = LoadSound("resources/Pings/MajorSets/MajorWaveA.ogg");
    majorTones[1] = LoadSound("resources/Pings/MajorSets/MajorWaveB.ogg");
    majorTones[2] = LoadSound("resources/Pings/MajorSets/MajorWaveC.ogg");
    majorTones[3] = LoadSound("resources/Pings/MajorSets/MajorWaveD.ogg");

    minorTones[0] = LoadSound("resources/Pings/MinorSets/MinorWaveA.ogg");
    minorTones[1] = LoadSound("resources/Pings/MinorSets/MinorWaveB.ogg");
    minorTones[2] = LoadSound("resources/Pings/MinorSets/MinorWaveC.ogg");
    minorTones[3] = LoadSound("resources/Pings/MinorSets/MinorWaveD.ogg");

    switchTone = LoadSound("resources/Ambiance/WavegamePingSplashZone.ogg");
  }

  void RadialInstrument::UnloadSounds()
  {
    UnloadMusicStream(staticAmbiance);
    UnloadMusicStream(majorAmbiance);
    UnloadMusicStream(minorAmbiance);

    for (Sound majorSound : majorTones)
      UnloadSound(majorSound);
    for (Sound minorSound : minorTones)
      UnloadSound(minorSound);
    UnloadSound(switchTone);
  }

  void RadialInstrument::Update()
  {
    UpdateMusicStream(staticAmbiance);
    UpdateMusicStream(*currentAmbiance);

    auto mousePosition = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
      OnClick(mousePosition);
    else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      rippleSpawner->SpawnHeldRipple(mousePosition);
  }


  void RadialInstrument::OnClick(vec2 position)
  {
    vec2 offset = position - centre;
    float centre_distance = offset.Length();

    if (centre_distance < innerRadius)
      OnCentreClick();
    else
      OnRadialClick(position, offset);
  }

  void RadialInstrument::OnCentreClick()
  {
    PlaySound(switchTone);
    rippleSpawner->SpawnCentreRipple();

    isMajor = !isMajor;
    StopMusicStream(*currentAmbiance);
    currentAmbiance = isMajor? &majorAmbiance: &minorAmbiance;
    PlayMusicStream(*currentAmbiance);
  }

  void RadialInstrument::OnRadialClick(vec2 position, vec2 offset)
  {
    float angle = atan2(offset.y, offset.x) + PI;
    int toneIndex = (int)(angle / TAU * 4);
    if (toneIndex == 4)
      toneIndex--;

    Sound* tones = isMajor? majorTones: minorTones;
    Sound tone = Sound(tones[toneIndex]);
    SetSoundPan(tone, GetPanning(position));
    float pitchScale = GetPitch(position);
    SetSoundPitch(tone, pitchScale);

    PlaySound(tone);
    rippleSpawner->SpawnRipple(position, pitchScale);
  }


  void RadialInstrument::DrawGuides(Color guideColor, float guideThickness)
  {
    DrawCircle(centre.x, centre.y, innerRadius, guideColor);

    // vec2 radiiStep = (centre - vec2(innerRadius + margin))
    for (int i = 0; i < 5; i++)
    {
      vec2 radii = vec2(innerRadius) + (centre - vec2(innerRadius + margin)) * float(i + 1) / 5;
      DrawEllipseLines(centre.x, centre.y, radii.x, radii.y, guideColor);
    }

    float radialStep = TAU / 4;
    for (int i = 0; i < 4; i++)
    {
      float angle = i * radialStep;
      vec2 direction = vec2(cos(angle), sin(angle));
      direction = direction.Normalized();
      vec2 lineStart = centre + direction * innerRadius;
      vec2 lineEnd = centre + direction * outerRadius;
      DrawLine(lineStart.x, lineStart.y, lineEnd.x, lineEnd.y, guideColor);
    }
  }


  float RadialInstrument::GetPitch(vec2 position)
  {
    int pitchIndex = 5;

    vec2 radii = vec2(innerRadius);
    vec2 radiiStep = (centre - radii - vec2(margin)) / 5;

    while(pitchIndex > 0)
    {
      radii += radiiStep;
      if (EllipseContains(centre, radii, position))
        break;
      pitchIndex--;
    }

    int semitones = (isMajor? majorPitches: minorPitches)[pitchIndex];
    return pow(2, semitones / 12.0);
  }

  bool RadialInstrument::EllipseContains(vec2 origin, vec2 radii, vec2 point)
  {
    float x = EllipseContainsHelper(origin.x, point.x, radii.x);
    float y = EllipseContainsHelper(origin.y, point.y, radii.y);

    return x + y <= 1;
  }

  float RadialInstrument::EllipseContainsHelper(float origin, float point, float radius)
  {
    float result = point - origin;
    result *= result;
    result /= (radius * radius);
    return result;
  }


  float RadialInstrument::GetPanning(vec2 position)
  {
    float panning = (position.x - centre.x) / centre.x;
    float normalized_panning = Clamp((panning + 1) / 2, 0, 1);
    return normalized_panning;
  }
}