#include "titlestate.h"
#include "keys.h"
#include "states.h"

TitleState::TitleState(ew::Engine *engine) :
  ew::State(engine, &world),
  engine(engine), world(engine),
  bgSound("snd/sfx/weaponfire17.wav"),
  music("snd/music/title.ogg"),
  update(&world),
  render(&world, engine->getRenderContext()),
  control(&world, engine->getControlContext())
{
  setPhases({&control, &update, &render});
  new Controller(this);
  new Timer(this);
}

void TitleState::enter()
{
  if(!music.playing())
    music.play();
  bgSound.play();
  world.reset();
}


void TitleState::init()
{
}

void TitleState::term()
{
}


TitleState::Controller::Controller(TitleState *state) :
  ew::Entity(&state->world), ew::Controllable(&state->world), state(state)
{
}

void TitleState::Controller::control(ew::ControlContext *context)
{
  if(actionKeyPush(ACTION_START, context))
  {
    state->music.stop();
    state->engine->setState(States::GAME);
  }

  if(actionKeyPush(ACTION_EXIT, context))
  {
    state->music.stop();
    state->engine->quit();
  }
}

TitleState::Timer::Timer(TitleState *state) :
  ew::Entity(&state->world), ew::Updatable(&state->world),
  state(state), stateTime(0)
{
}

void TitleState::Timer::update(const float delta)
{
  stateTime += delta;

  if(stateTime > 10)
  {
    state->engine->setState(States::HIGHSCORES);
    stateTime = 0;
  }
}

