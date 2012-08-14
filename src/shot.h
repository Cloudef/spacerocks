#ifndef SHOT_HH
#define SHOT_HH

#include "gameworld.h"
#include "ew/renderable.h"
#include "ew/updatable.h"
#include "ew/collidable.h"

#include "GL/glhck.h"
#include "util/vec2d.h"
#include "util/circleshape.h"

#include <string>

class Shot : public Renderable, public Updatable, public Collidable
{
public:
  static void init();
  static void term();

  Shot(GameWorld* world, float const life, Vec2D const& position, Vec2D const& velocity);
  ~Shot();

  static UID const ID;
  UID getEntityId() const { return ID; }

  void render(RenderContext* context);
  void update(float const delta);
  virtual void collide(Collidable const* other);
  virtual CircleShape const* getShape() const;


  Vec2D getPosition() const;

private:
  static std::string const IMAGE;
  static glhckTexture* TEXTURE;
  static int const RADIUS = 4;

  glhckObject* o;
  float life;
  Vec2D v;
  CircleShape shape;
};

#endif
