#ifndef POWERUP_HH
#define POWERUP_HH

#include "gameworld.h"
#include "ew/renderable.h"
#include "ew/updatable.h"
#include "ew/collidable.h"

#include "glhck/glhck.h"
#include "util/vec2d.h"
#include "util/circleshape.h"
#include "textureatlas.h"
#include "sound.h"

#include <string>
#include <vector>

class Powerup : public ew::Renderable, public ew::Updatable, public ew::Collidable
{
public:
  static void init();
  static void term();

  enum Type { LASER, SPREAD, BEAM, PLASMA, EXTRALIFE, LOSELIFE, SHIELD, NUM_TYPES };
  Powerup(GameWorld* world, Type const type, Vec2D const& position, Vec2D const& velocity);
  ~Powerup();

  void render(ew::RenderContext* context);
  void update(float const delta);
  virtual void collide(ew::Collidable const* other);

  virtual CircleShape const* getShape() const;

  Vec2D getPosition() const;
  Type getType() const;
  bool alive() const;
private:
  static float const RADIUS;
  static std::vector<std::string> const IMAGES;
  constexpr static float const FADEOUT_TIME = 1.0f;

  static TextureAtlas atlas;

  GameWorld* gameWorld;
  glhckObject* o;
  Type type;
  Vec2D v;
  float life;
  float fadeOutTimer;
  CircleShape shape;
  
  static Sound weaponSound;
  static Sound extralifeSound;
  static Sound loselifeSound;
  static Sound shieldSound;
};

#endif
