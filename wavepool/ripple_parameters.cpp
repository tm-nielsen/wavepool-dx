namespace wavepool {
  struct RippleParameters {
    float radius;
    float amplitude;
    float speed;
    float wavelength;
    int crestCount;
    float lifetime;

    RippleParameters();
    RippleParameters(float, float, float, float, int, float);
  };

  RippleParameters::RippleParameters()
  {
    radius = 10;
    amplitude = 3;
    speed = 200;
    wavelength = 20;
    crestCount = 1;
    lifetime = 1.5;
  }

  RippleParameters::RippleParameters(float radius, float amplitude, float speed,
    float wavelength, int crestCount, float lifetime)
  {
    this->radius = radius;
    this->amplitude = amplitude;
    this->speed = speed;
    this->wavelength = wavelength;
    this->crestCount = crestCount;
    this->lifetime = lifetime;
  }
}