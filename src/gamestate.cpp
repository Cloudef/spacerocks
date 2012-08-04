#include "gamestate.h"

#include "GL/glfw3.h"

#include <iomanip>
#include <sstream>
#include "vec2d.h"
#include "util.h"

#include "engine.h"

#include "asteroid.h"
#include "ship.h"
#include "laser.h"
#include "shot.h"
#include "plasma.h"
#include "beam.h"
#include "spark.h"
#include "explosion.h"
#include "ufo.h"
#include "ufolaser.h"
#include "powerup.h"

void GameState::init()
{
  Ship::init();
  Asteroid::init();
  Laser::init();
  Shot::init();
  Plasma::init();
  Beam::init();
  Explosion::init();
  Spark::init();
  Ufo::init();
  UfoLaser::init();
  Powerup::init();
}

GameState::GameState(Engine* engine) :
  State(engine), world(), background(nullptr),
  gameText(nullptr), fpsText(nullptr), gameFont(0), fpsFont(0)
{
  world.player.lives = 3;
  world.player.weapon[Ship::RAPID] = 1;
  world.initLevel(0);

  world.player.ship = new Ship(&world, {0, 0}, {0, 0});
  world.addSprite(world.player.ship);

  background = glhckSpriteNewFromFile("img/background.png", 0, 0, GLHCK_TEXTURE_DEFAULTS);
  glhckObjectScalef(background, 0.5f, 0.5f, 0.5f);
  glhckObjectPositionf(background, 0, 0, -0.01);

  gameText = glhckTextNew(200, 200);
  fpsText = glhckTextNew(800, 40);

  gameFont = glhckTextNewFont(gameText, "fonts/DejaVuSans.ttf");
  fpsFont = glhckTextNewFont(fpsText, "fonts/DejaVuSans.ttf");
}

void GameState::input()
{
  GLFWwindow& window = *engine->getWindow();

  bool lastPrevWeaponButtonState = glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS;
  bool lastNextWeaponButtonState = glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS;
  bool lastF1 = glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS;
  bool lastF2 = glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS;
  bool lastF3 = glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS;
  bool lastF4 = glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS;

  glfwPollEvents();

  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    engine->quit();
  }

  if(world.player.ship != nullptr)
  {
    world.player.ship->turnLeft(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS);
    world.player.ship->turnRight(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS);
    world.player.ship->accelerate(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS);
    world.player.ship->shoot(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);

    if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && !lastPrevWeaponButtonState)
      world.player.ship->prevWeapon();

    if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && !lastNextWeaponButtonState)
      world.player.ship->nextWeapon();

    if(glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS && !lastF1)
      world.player.weapon[Ship::RAPID] += world.player.weapon[Ship::RAPID] < 8 ? 1 : 0;
    if(glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS && !lastF2)
      world.player.weapon[Ship::SPREAD] += world.player.weapon[Ship::SPREAD] < 8 ? 1 : 0;
    if(glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS && !lastF3)
      world.player.weapon[Ship::BEAM] += world.player.weapon[Ship::BEAM] < 8 ? 1 : 0;
    if(glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS && !lastF4)
      world.player.weapon[Ship::PLASMA] += world.player.weapon[Ship::PLASMA] < 8 ? 1 : 0;
  }
}

void GameState::update(float const delta)
{
  world.update(delta);
}

void GameState::render()
{
  glhckObjectRender(background);

  for(auto i : world.getSprites())
  {
    i->render();
  }

  Ship::Weapon w = world.player.ship != nullptr ? world.player.ship->getWeapon() : Ship::NUM_WEAPONS;
  std::ostringstream ss;
  ss << "Level: " << (world.level.n + 1)
      << " | Score: " << world.player.score
      << " | Lives: " << world.player.lives
      << " | Weapons: "
      << (w == Ship::RAPID ? "[R" : "R") << world.player.weapon[Ship::RAPID] << (w == Ship::RAPID ? "] " : " ")
      << (w == Ship::SPREAD ? "[S" : "S") << world.player.weapon[Ship::SPREAD] << (w == Ship::SPREAD ? "] " : " ")
      << (w == Ship::BEAM ? "[B" : "B") << world.player.weapon[Ship::BEAM] << (w == Ship::BEAM ? "] " : " ")
      << (w == Ship::PLASMA ? "[P" : "P") << world.player.weapon[Ship::PLASMA] << (w == Ship::PLASMA ? "] " : " ");

  glhckTextDraw(gameText, gameFont, 20, 5, 20, ss.str().data(), NULL);

  ss.str("");
  ss << std::setprecision(2) << std::fixed
      << "FPS: " << engine->getTimer().getFPS()
      << " | total: " << engine->getTimer().getTotalTime()
      << "s | frame: " << engine->getTimer().getTicks();

  glhckTextDraw(fpsText, fpsFont, 14, 5, 35, ss.str().data(), NULL);

  glhckTextRender(gameText);
  glhckTextRender(fpsText);

}
