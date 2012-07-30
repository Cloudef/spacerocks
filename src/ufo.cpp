#include "ufo.h"
#include "laser.h"
#include "util.h"
#include "explosion.h"
#include "world.h"
#include "spark.h"
#include "ufolaser.h"

float const TAU = 2 * 3.14159265;

int const Ufo::ID = Entity::newEntityId();

int const Ufo::ANIMATION_FRAMES[NUM_FRAMES] = {1, 1, 1, 1, 1, 1, 1, 1, 2, 3};

std::string const Ufo::IMAGES[NUM_IMAGES] = {
  "img/ufo_1.png",
  "img/ufo_2.png",
  "img/ufo_3.png",
  "img/ufo_4.png",
};

std::vector<Sprite::TransformData> Ufo::TRANSFORM;
glhckTexture *Ufo::TEXTURE = NULL;

float const Ufo::SHOOT_INTERVAL = 1.5;

void Ufo::init()
{
  glhckAtlas *TEXTURES = glhckAtlasNew();
  for(int i = 0; i < NUM_IMAGES; ++i)
  {
    glhckTexture* texture = glhckTextureNew(IMAGES[i].data(), GLHCK_TEXTURE_DEFAULTS);
    glhckAtlasInsertTexture(TEXTURES, texture);
    glhckTextureFree(texture);
  }
  glhckAtlasPack(TEXTURES, true, false);

  for(int i = 0; i < NUM_IMAGES; ++i)
  {
    TransformData t;
    glhckAtlasGetTransform(TEXTURES, glhckAtlasGetTextureByIndex(TEXTURES, i), &t.transform, &t.degree);
    TRANSFORM.push_back(t);
  }

  TEXTURE = glhckTextureRef(glhckAtlasGetTexture(TEXTURES));
  glhckAtlasFree(TEXTURES);
}

Ufo::Ufo(World* world, Vec2D const& startPosition, Vec2D const& endPosition,
         int freq, float amplitude, float duration) :
  Sprite(world, 1), o(0), startPosition(startPosition), endPosition(endPosition),
  freq(freq), amplitude(amplitude), duration(duration), time(0), life(3), shape(startPosition, RADIUS)
{
  o = glhckSpriteNew(TEXTURE, 16, 16);
  glhckObjectTransformCoordinates(o, &TRANSFORM[0].transform, TRANSFORM[0].degree);
  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, startPosition.x, startPosition.y, 0);
}

Ufo::~Ufo()
{
  glhckObjectFree(o);
}


void Ufo::render()
{
  glhckObjectRender(o);
}

void Ufo::update(float delta)
{
  time += delta;
  int frame = ANIMATION_FRAMES[static_cast<int>(time * FPS) % NUM_FRAMES];
  glhckObjectTransformCoordinates(o, &TRANSFORM[frame].transform, TRANSFORM[frame].degree);
  glhckObjectRotatef(o, 0, 0, delta * 180);

  Vec2D journey = endPosition - startPosition;
  float deviation = amplitude * sin(freq * TAU * time / duration);
  Vec2D position = startPosition
    + journey.scale(time / duration)
    + journey.normal().uniti().scale(deviation);
  glhckObjectPositionf(o, position.x, position.y, 0);

  bool timeToShoot = static_cast<int>((time - delta) / SHOOT_INTERVAL) !=
    static_cast<int>(time / SHOOT_INTERVAL);

  if(timeToShoot && world->ship != nullptr)
  {
    Vec2D direction = (world->ship->getPosition() - getPosition()).uniti();
    direction += direction.normal().scalei(randFloat(-0.3, 0.3));
    Vec2D velocity = direction.scale(600);
    UfoLaser* laser = new UfoLaser(world, 5.0f, getPosition(), velocity);
    world->sprites.insert(std::shared_ptr<UfoLaser>(laser));
  }

  shape.center = getPosition();
}

bool Ufo::alive() const
{
  return life > 0 && time < duration;
}

CircleShape const* Ufo::getShape() const
{
  return &shape;
}

void Ufo::collide(Sprite const* other) {
  Vec2D position = getPosition();

  if(other->getEntityId() == Laser::ID) {
    Laser const* laser = static_cast<Laser const*>(other);
    if(shape.collidesWith(laser->getShape()))
    {
      life -= 0.5;

      if(alive())
      {
        Vec2D hitDirection = (laser->getPosition() - position).uniti();
        Vec2D hitPosition = position + hitDirection.scale(RADIUS);
        Vec2D hitNormal = hitDirection.normal();

        for(int i = 0; i < 10; ++i)
        {
          float pLife = 0.1 + randFloat(0, 0.1);
          float speed = 120 + (rand() % 40);
          Vec2D dev = hitNormal.scale(randFloat(-100, 100));
          Spark* spark = new Spark(world, pLife, hitPosition, hitDirection.scale(speed) + dev);
          world->sprites.insert(std::shared_ptr<Spark>(spark));
        }
      }
      else
      {
        world->score += 100;

        Explosion* explosion = new Explosion(world, position);
        world->sprites.insert(std::shared_ptr<Explosion>(explosion));
      }
    }
    return;
  }
}

Vec2D Ufo::getPosition() const
{
  kmVec3 const* pos = glhckObjectGetPosition(o);
  return {pos->x, pos->y};
}