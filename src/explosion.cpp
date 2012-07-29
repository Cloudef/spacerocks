#include "explosion.h"

int const Explosion::ID = Entity::newEntityId();

std::string const Explosion::IMAGES[NUM_IMAGES] = {
  "img/explosion/explosion1_0001.png",
  "img/explosion/explosion1_0002.png",
  "img/explosion/explosion1_0003.png",
  "img/explosion/explosion1_0004.png",
  "img/explosion/explosion1_0005.png",
  "img/explosion/explosion1_0006.png",
  "img/explosion/explosion1_0007.png",
  "img/explosion/explosion1_0008.png",
  "img/explosion/explosion1_0009.png",
  "img/explosion/explosion1_0010.png",
  "img/explosion/explosion1_0011.png",
  "img/explosion/explosion1_0012.png",
  "img/explosion/explosion1_0013.png",
  "img/explosion/explosion1_0014.png",
  "img/explosion/explosion1_0015.png",
  "img/explosion/explosion1_0016.png",
  "img/explosion/explosion1_0017.png",
  "img/explosion/explosion1_0018.png",
  "img/explosion/explosion1_0019.png",
  "img/explosion/explosion1_0020.png",
  "img/explosion/explosion1_0021.png",
  "img/explosion/explosion1_0022.png",
  "img/explosion/explosion1_0023.png",
  "img/explosion/explosion1_0024.png",
  "img/explosion/explosion1_0025.png",
  "img/explosion/explosion1_0026.png",
  "img/explosion/explosion1_0027.png",
  "img/explosion/explosion1_0028.png",
  "img/explosion/explosion1_0029.png",
  "img/explosion/explosion1_0030.png",
  "img/explosion/explosion1_0031.png",
  "img/explosion/explosion1_0032.png",
  "img/explosion/explosion1_0033.png",
  "img/explosion/explosion1_0034.png",
  "img/explosion/explosion1_0035.png",
  "img/explosion/explosion1_0036.png",
  "img/explosion/explosion1_0037.png",
  "img/explosion/explosion1_0038.png",
  "img/explosion/explosion1_0039.png",
  "img/explosion/explosion1_0040.png",
  "img/explosion/explosion1_0041.png",
  "img/explosion/explosion1_0042.png",
  "img/explosion/explosion1_0043.png",
  "img/explosion/explosion1_0044.png",
  "img/explosion/explosion1_0045.png",
  "img/explosion/explosion1_0046.png",
  "img/explosion/explosion1_0047.png",
  "img/explosion/explosion1_0048.png",
  "img/explosion/explosion1_0049.png",
  "img/explosion/explosion1_0050.png",
  "img/explosion/explosion1_0051.png",
  "img/explosion/explosion1_0052.png",
  "img/explosion/explosion1_0053.png",
  "img/explosion/explosion1_0054.png",
  "img/explosion/explosion1_0055.png",
  "img/explosion/explosion1_0056.png",
  "img/explosion/explosion1_0057.png",
  "img/explosion/explosion1_0058.png",
  "img/explosion/explosion1_0059.png",
  "img/explosion/explosion1_0060.png",
  "img/explosion/explosion1_0061.png",
  "img/explosion/explosion1_0062.png",
  "img/explosion/explosion1_0063.png",
  "img/explosion/explosion1_0064.png",
  "img/explosion/explosion1_0065.png",
  "img/explosion/explosion1_0066.png",
  "img/explosion/explosion1_0067.png",
  "img/explosion/explosion1_0068.png",
  "img/explosion/explosion1_0069.png",
  "img/explosion/explosion1_0070.png",
  "img/explosion/explosion1_0071.png",
  "img/explosion/explosion1_0072.png",
  "img/explosion/explosion1_0073.png",
  "img/explosion/explosion1_0074.png",
  "img/explosion/explosion1_0075.png",
  "img/explosion/explosion1_0076.png",
  "img/explosion/explosion1_0077.png",
  "img/explosion/explosion1_0078.png",
  "img/explosion/explosion1_0079.png",
  "img/explosion/explosion1_0080.png",
  "img/explosion/explosion1_0081.png",
  "img/explosion/explosion1_0082.png",
  "img/explosion/explosion1_0083.png",
  "img/explosion/explosion1_0084.png",
  "img/explosion/explosion1_0085.png",
  "img/explosion/explosion1_0086.png",
  "img/explosion/explosion1_0087.png",
  "img/explosion/explosion1_0088.png",
  "img/explosion/explosion1_0089.png",
  "img/explosion/explosion1_0090.png"
};

std::vector<Sprite::TransformData> Explosion::TRANSFORM;
glhckTexture *Explosion::TEXTURE = NULL;

void Explosion::init()
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

Explosion::Explosion(World* world, Vec2D const& position) :
  Sprite(world, 1), o(0), time(0)
{
  o = glhckSpriteNew(TEXTURE, 160, 120);
  glhckObjectTransformCoordinates(o, &TRANSFORM[0].transform, TRANSFORM[0].degree);

  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);
}

Explosion::~Explosion()
{
  glhckObjectFree(o);
}


void Explosion::render()
{
  glhckObjectRender(o);
}

void Explosion::update(float delta)
{
  time += delta;
  int frame = static_cast<int>(time * FPS);

  if(frame < NUM_IMAGES)
  {
    glhckObjectTransformCoordinates(o, &TRANSFORM[frame].transform, TRANSFORM[frame].degree);
  }
}

bool Explosion::alive() const
{
  return static_cast<int>(time * FPS) < NUM_IMAGES;
}