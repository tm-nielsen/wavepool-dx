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
    vec2 diff = position - centre;
    float centre_distance = diff.Length();

    if (centre_distance < innerRadius)
    {
      PlaySound(switchTone);
      rippleSpawner->SpawnCentreRipple();

      isMajor = !isMajor;
      StopMusicStream(*currentAmbiance);
      currentAmbiance = isMajor? &majorAmbiance: &minorAmbiance;
      PlayMusicStream(*currentAmbiance);
    }
    else
    {
      float angle = atan2(diff.y, diff.x) + PI;
      int toneIndex = (int)(angle / TAU * 4);
      if (toneIndex == 4)
        toneIndex--;

      Sound* tones = isMajor? majorTones: minorTones;
      Sound tone = Sound(tones[toneIndex]);
      SetSoundPan(tone, GetPanning(position));
      SetSoundPitch(tone, GetPitch(position));

      PlaySound(tone);
      rippleSpawner->SpawnRipple(position);
    }
  }

  void RadialInstrument::DrawGuides(Color guideColor, float guideThickness)
  {

  }


  float RadialInstrument::GetPitch(vec2 position)
  {
    return 1.0;
  }

  float RadialInstrument::GetPanning(vec2 position)
  {
    float panning = (position.x - centre.x) / centre.x;
    float normalized_panning = Clamp((panning + 1) / 2, 0, 1);
    return normalized_panning;
  }
}