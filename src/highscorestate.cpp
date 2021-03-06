#include "highscorestate.h"
#include "states.h"
#include "keys.h"

HighScoreState::HighScoreState(ew::Engine *engine) :
  ew::State(engine, &world),
  engine(engine),
  world(engine),
  update(&world),
  render(&world, engine->getRenderContext()),
  control(&world, engine->getControlContext())
{
  setPhases({&control, &update, &render});
  new Controller(this);
  new Timer(this);
}

void HighScoreState::enter()
{
  world.enter();
}

void HighScoreState::exit()
{
  world.exit();
}


void HighScoreState::init()
{
}

void HighScoreState::term()
{
}


HighScoreState::Controller::Controller(HighScoreState *state) :
  ew::Entity(&state->world), ew::Controllable(&state->world), state(state)
{
}

void HighScoreState::Controller::control(ew::ControlContext *context)
{
  if(actionKeyPush(ACTION_START, context))
  {
    state->engine->setState(States::TITLE);
  }

  if(actionKeyPush(ACTION_EXIT, context))
  {
    state->engine->quit();
  }
}

HighScoreState::Timer::Timer(HighScoreState *state) :
  ew::Entity(&state->world), ew::Updatable(&state->world),
  state(state), stateTime(0)
{
}

void HighScoreState::Timer::update(const float delta)
{
  stateTime += delta;

  if(stateTime > 20)
  {
    state->engine->setState(States::TITLE);
    stateTime = 0;
  }
}

