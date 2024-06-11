#include <math.h>
#include "vec2.cpp"
#include "ripple_parameters.cpp"

#ifndef PI
#define PI 3.14159265359
#endif
#ifndef TAU
#define TAU (PI * 2)
#endif

namespace wavepool {
  class Ripple
  {
    private:
      float amplitude = 3;
      float speed = 200;
      float wavelength = 20;
      int crestCount = 1;
      float lifetime = 1.5;

      vec2 origin;
      float startingRadius;
      float radius;
      float strength;
      float timer;

      float waveLimit;
      float waveLowerLimit;
      float waveTailLimit;
      float waveTailLimitSquared;
      float baseScaling;

      float EvaluateLifetimeCurve(float);
      vec2 GetTailOffset(vec2, float);

    public:
      Ripple(vec2, float);
      Ripple(vec2, RippleParameters, float);
      void Update(float);
      void UpdateConstants();
      vec2 GetOffset(vec2);

      bool isAlive;
  };

  Ripple::Ripple(vec2 origin, float radius):
    origin{origin}, startingRadius{radius}, radius{radius}
  {
    strength = 1;
    timer = 0;
    isAlive = true;
  }

  Ripple::Ripple(vec2 origin, RippleParameters parameters, float pitchScale = 1):
    origin{origin}, startingRadius{parameters.radius}, radius{parameters.radius}
  {
    amplitude = parameters.amplitude / pitchScale;
    speed = parameters.speed;
    wavelength = parameters.wavelength / pitchScale;
    crestCount = parameters.crestCount;
    lifetime = parameters.lifetime;

    strength = 1;
    timer = 0;
    isAlive = true;
  }

  void Ripple::Update(float deltaTime)
  {
    radius += speed * deltaTime;
    strength = startingRadius / radius;
    timer += deltaTime;
    float normalizedLifetime = timer / lifetime;
    // decay -= decayRate * deltaTime;

    strength *= EvaluateLifetimeCurve(normalizedLifetime);

    UpdateConstants();

    if (timer > lifetime)
        isAlive = false;
  }

  float Ripple::EvaluateLifetimeCurve(float lifetime)
  {
    return (1 + std::sin(PI * (lifetime + 0.5))) / 2.0;
  }

  void Ripple::UpdateConstants()
  {
    waveLowerLimit = radius - TAU * wavelength * (crestCount - 0.75f);
    if (waveLowerLimit < 0)
        waveLowerLimit = 0;
    else
        waveLowerLimit *= waveLowerLimit;

    waveTailLimit = radius - TAU * wavelength * (crestCount - 0.25f);
    if(waveTailLimit < 0)
        waveTailLimit = 0;
    waveTailLimitSquared = waveTailLimit * waveTailLimit;

    waveLimit = radius + (wavelength * PI) / 2;
    waveLimit *= waveLimit;

    baseScaling = strength * amplitude;
  }

  vec2 Ripple::GetOffset(vec2 point)
  {
    vec2 diff = point - origin;
    float dist = diff.LengthSquared();

    if (dist == 0)
        return vec2();

    if (dist > waveLimit || dist < waveLowerLimit)
    {
        if(dist < waveLowerLimit && dist > waveTailLimitSquared)
        {
            return GetTailOffset(diff, dist);
        }
        return vec2();
    }

    dist = std::sqrt(dist);

    float scaling = 1 + dist / wavelength;

    float oscillation = std::cos((dist - radius) / wavelength);

    return diff.Normalized() * baseScaling * oscillation * scaling;
  }

  vec2 Ripple::GetTailOffset(vec2 dir, float distSquared)
  {
    float dist = std::sqrt(distSquared);

    float scaling = (dist - waveTailLimit) / (wavelength * PI);
    scaling *= 1 + dist / wavelength;

    float oscillation = std::cos((dist - radius) / wavelength);

    return dir.Normalized() * baseScaling * oscillation * scaling;
  }
}