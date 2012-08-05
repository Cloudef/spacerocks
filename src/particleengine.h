#ifndef PARTICLEENGINE_HH
#define PARTICLEENGINE_HH

#include "GL/glhck.h"
#include "vec2d.h"
#include "textureatlas.h"

#include <vector>
#include <array>
#include <string>

class ParticleEngine
{
public:
  enum ParticleType { SPARK, NUM_PARTICLE_TYPES };

  static void init();

  ParticleEngine();
  ~ParticleEngine();

  void render();
  void update(float delta);

  void addParticle(ParticleType const type, Vec2D const& position, Vec2D const& velocity, float const life);

private:
  struct Particle
  {
    Particle() :
      o(nullptr), type(NUM_PARTICLE_TYPES), velocity({0, 0}), life(0.0f)
    {
    }
    glhckObject* o;
    ParticleType type;
    Vec2D velocity;
    float life;
  };

  static std::vector<std::string> const IMAGES;
  static std::vector<float> const PARTICLE_SIZES;
  static TextureAtlas atlas;

  static int const NUM_PARTICLES = 64;
  typedef std::array<Particle, NUM_PARTICLES> Particles;

  Particles particles;
  int live;
};

#endif