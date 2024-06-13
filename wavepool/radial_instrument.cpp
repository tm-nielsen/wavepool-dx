#include "raylib.h"
#include "../utils/vec2.cpp"
#include "../utils/rect.cpp"
#include "ripple_spawner.cpp"

namespace wavepool {
  using namespace utils;

  class RadialInstrument {
    private:
      const int majorPitches [6] = {-9, -4, 0, 3, 5, 10};
      const int minorPitches [6] = {-10, -5, 0, 4, 5, 9};
      const int toneCount = 4;

      RippleSpawner* rippleSpawner;
      rect centralKeyRect;
      rect keyRects [5];

      Music staticAmbiance;
      Music majorAmbiance;
      Music minorAmbiance;
      Music* currentAmbiance;

      Sound majorTones [4];
      Sound minorTones [4];
      Sound switchTone;

      vec2 centre;
      float margin;
      float centralKeyHalfWidth;

      bool isMajor;
      
      float GetPitch(vec2);
      float GetPanning(vec2);
      void OnCentreClick();
      void OnRadialClick(vec2, vec2);
      void GenerateKeyRects(vec2, float);
      bool IsFlat();
      bool IsHorizontallyFlat();
      bool IsVerticallyFlat();
      Music InitializeMusicStream(const char*, float);

    public:
      RadialInstrument(RippleSpawner*, vec2, float, float);
      void LoadSounds();
      void UnloadSounds();
      void Update();
      void OnWindowResized(vec2);
      void DrawGuides(Color, float);
      void OnClick(vec2);
  };

  RadialInstrument::RadialInstrument(RippleSpawner* rippleSpawner, vec2 screenSize,
    float innerRadius, float margin): rippleSpawner{rippleSpawner}
  {
    isMajor = true;

    this->margin = margin;

    OnWindowResized(screenSize);
  }

  void RadialInstrument::LoadSounds()
  {
    staticAmbiance = InitializeMusicStream("resources/Ambiance/WavegameAmbienceStaticStereo.ogg", 0.1);
    majorAmbiance = InitializeMusicStream("resources/Ambiance/WavegameAmbianceMajorStereo.ogg", 0.35);
    minorAmbiance = InitializeMusicStream("resources/Ambiance/WavegameAmbianceMinorStereo.ogg", 0.35);

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

  Music RadialInstrument::InitializeMusicStream(const char *path, float volume = 1)
  {
    Music musicStream = LoadMusicStream(path);
    musicStream.looping = true;
    SetMusicVolume(musicStream, volume);
    return musicStream;
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


  void RadialInstrument::OnWindowResized(vec2 screenSize)
  {
    centre = screenSize / 2;
    centralKeyHalfWidth = screenSize.GetLargestComponent() / 12;
    centralKeyRect = rect(centre - centralKeyHalfWidth, centralKeyHalfWidth * 2);
    GenerateKeyRects(screenSize, margin);
  }


  void RadialInstrument::OnClick(vec2 position)
  {
    vec2 offset = position - centre;
    float centre_distance = offset.Length();

    if (centralKeyRect.ContainsPoint(position))
      OnCentreClick();
    else
      OnRadialClick(position, offset);
  }

  void RadialInstrument::OnCentreClick()
  {
    PlaySound(switchTone);
    rippleSpawner->SpawnCentreRipple(centre);

    isMajor = !isMajor;
    StopMusicStream(*currentAmbiance);
    currentAmbiance = isMajor? &majorAmbiance: &minorAmbiance;
    PlayMusicStream(*currentAmbiance);
  }

  void RadialInstrument::OnRadialClick(vec2 position, vec2 offset)
  {
    float angle = atan2(offset.y, offset.x) + PI;
    int toneIndex = (int)(toneCount * angle / TAU);
    if (toneIndex == toneCount)
      toneIndex--;

    Sound* tones = isMajor? majorTones: minorTones;
    Sound tone = Sound(tones[toneIndex]);
    SetSoundPan(tone, GetPanning(position));
    float pitchScale = GetPitch(position);
    SetSoundPitch(tone, pitchScale);

    PlaySound(tone);
    rippleSpawner->SpawnRipple(position, pitchScale);
  }


  void RadialInstrument::DrawGuides(Color colour, float thickness)
  {
    if (IsHorizontallyFlat())
      centralKeyRect.DrawHorizontalBorders(thickness, colour);
    else if (IsVerticallyFlat())
      centralKeyRect.DrawVerticalBorders(thickness, colour);
    else
      centralKeyRect.DrawRounded(thickness, colour, 0.075, 4);

    for (rect keyRect : keyRects)
    {
      if (IsHorizontallyFlat())
        keyRect.DrawVerticalBorders(thickness, colour);
      else if (IsVerticallyFlat())
        keyRect.DrawHorizontalBorders(thickness, colour);
      else
        keyRect.DrawRounded(thickness, colour, 0.075, 4);
    }

    float radialStep = TAU / toneCount;
    for (int i = 0; i < toneCount; i++)
    {
      float angle = i * radialStep;
      vec2 direction = vec2(cos(angle), sin(angle));
      direction = direction.Normalized();
      vec2 lineStart = centre + direction * centralKeyHalfWidth;
      vec2 lineEnd = centre + direction * (centre - margin);
      DrawLineEx(lineStart.ToVector2(), lineEnd.ToVector2(), thickness, colour);
    }
  }


  float RadialInstrument::GetPitch(vec2 position)
  {
    int pitchIndex = 5;
    while (pitchIndex > 0)
    {
      if (keyRects[pitchIndex - 1].ContainsPoint(position))
        break;
      pitchIndex--;
    }

    int semitones = (isMajor? majorPitches: minorPitches)[pitchIndex];
    return pow(2, semitones / 12.0);
  }

  float RadialInstrument::GetPanning(vec2 position)
  {
    float panning = (position.x - centre.x) / centre.x;
    float normalized_panning = Clamp((panning + 1) / 2, 0, 1);
    return normalized_panning;
  }

  void RadialInstrument::GenerateKeyRects(vec2 screenSize, float margin)
  {
    vec2 halfScreenSize = screenSize / 2;
    vec2 rectOrigin = halfScreenSize - centralKeyHalfWidth;
    vec2 rectSize = centralKeyHalfWidth * 2;
    rect baseRect = rect(rectOrigin, rectSize);
    vec2 sizeIncrease = (rectOrigin - margin) / 5;

    if (IsHorizontallyFlat())
    {
      baseRect.origin.y = margin;
      baseRect.size.y = screenSize.y - 2 * margin;
      sizeIncrease.y = 0;
    }
    else if (IsVerticallyFlat())
    {
      baseRect.origin.x = margin;
      baseRect.size.x = screenSize.x - 2 * margin;
      sizeIncrease.x = 0;
    }
    else
    {
      sizeIncrease *= 5 / 6.0;
      baseRect.GrowRadially(sizeIncrease);
    }

    for (int i = 4; i >= 0; i--)
    {
      keyRects[i] = rect(baseRect);
      baseRect.GrowRadially(sizeIncrease);
    }
  }

  bool RadialInstrument::IsFlat() {
    return IsHorizontallyFlat() || IsVerticallyFlat();
  }
  bool RadialInstrument::IsHorizontallyFlat() {
    return centre.x > centre.y * 1.3;
  }
  bool RadialInstrument::IsVerticallyFlat() {
    return centre.y > centre.x * 1.3;
  }
}