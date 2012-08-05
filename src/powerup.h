#ifndef POWERUP_HH
#define POWERUP_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"
#include "circleshape.h"
#include "textureatlas.h"

#include <string>
#include <vector>

class Powerup : public Sprite
{
public:
  static void init();

  enum Type { LASER, SPREAD, BEAM, PLASMA, EXTRALIFE, LOSELIFE, SHIELD, NUM_TYPES };
  Powerup(World* world, Type const type, Vec2D const& position, Vec2D const& velocity);
  ~Powerup();

  static int const ID;
  int getEntityId() const { return ID; }

  void render();
  void update(float delta);

  virtual CircleShape const* getShape() const;
  virtual void collide(Sprite const* other);

  Vec2D getPosition() const;
  Type getType() const;

private:
  static float const RADIUS;
  static std::vector<std::string> const IMAGES;
  static TextureAtlas atlas;

  glhckObject* o;
  Type type;
  Vec2D v;
  float life;
  CircleShape shape;
};

#endif