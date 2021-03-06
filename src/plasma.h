#ifndef PLASMA_HH
#define PLASMA_HH

#include "gameworld.h"
#include "ew/renderable.h"
#include "ew/updatable.h"
#include "ew/collidable.h"

#include "glhck/glhck.h"
#include "util/vec2d.h"
#include "util/circleshape.h"

#include <string>

class Plasma : public ew::Renderable, public ew::Updatable, public ew::Collidable
{
public:
  static void init();
  static void term();

  Plasma(GameWorld* world, float const life, float const power, Vec2D const& position, Vec2D const& velocity);
  ~Plasma();

  void render(ew::RenderContext* context);
  void update(float const delta);
  void collide(ew::Collidable const* other);

  virtual CircleShape const* getShape() const;

  Vec2D getPosition() const;
  float getPower() const;
  float getRadius() const;

private:
  static std::string const IMAGE;
  static glhckTexture* TEXTURE;

  GameWorld* gameWorld;
  glhckObject* o;
  glhckObject* oGlow;
  float power;
  float nextPower;
  float life;
  float glowPhase;
  Vec2D v;
  CircleShape shape;
};

#endif
