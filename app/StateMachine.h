#pragma once

/**
 * TOPPERS API
 */
#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "target_syssvc.h"

#define STATEMACHINE_STATE_NUM_MAX (30)

class State;
class Binder
{
public:
  Binder(int, State *);
  ~Binder();
  int event_;
  State *state_;
};

class StateMachine;
class State
{
public:
  State(){};
  virtual ~State(){};
  virtual void CreateStateList() = 0;
  virtual State *Enter(StateMachine *stateMachine) = 0;
  virtual void Leave(StateMachine *stateMachine) = 0;
  void AddStateList(Binder *);
  State *GetNext(int event); //辞書でデータ取得できれば楽。
  Binder *state_list_[STATEMACHINE_STATE_NUM_MAX] = {NULL};
  size_t state_list_size_ = 0;
};

class StateMachine
{
public:
  StateMachine(){};
  void Init();
  void Run();
  void SendEvent(int event);
  int GetLastEvent();

private:
  State *state_ = NULL; //現在状態
protected:
  int dtq_id_;         //静的DTQのID
  int last_event_ = 0; //最後に受け取ったイベント
  void ReceiveEvent(int event);
  void SetState(State *);
};