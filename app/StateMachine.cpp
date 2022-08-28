#include "StateMachine.h"

Binder::Binder(int event, State *state)
{
  event_ = event;
  state_ = state;
}

Binder::~Binder()
{
  delete state_;
}

State *State::GetNext(int event)
{

  State *next_state = NULL;
  int i_next_state = 0;

  CreateStateList(); //遷移先リストを生成する。

  //遷移先状態を選択し、そのオブジェクトポインタを取得する。
  for (int i = 0; i < state_list_size_; i++)
  {
    if (state_list_[i]->event_ == event)
    {
      // syslog(LOG_NOTICE, "[State] Next state index:%d", i);
      next_state = state_list_[i]->state_;
      i_next_state = i;
      break;
    }
  }

  if (next_state != NULL)
  {
    //遷移先リストのオブジェクトのうち、next非採用のオブジェクトはここですべて破棄する。
    for (int i = 0; i < state_list_size_; i++)
    {
      if (i != i_next_state)
      {
        // syslog(LOG_NOTICE, "[State] Delete object not used for next:%d", i);
        delete state_list_[i];
      }
    }
    return next_state;
  }
  else
  {
    // syslog(LOG_NOTICE, "[StateMachine] Next state not found");
    return NULL;
  }
}

void State::AddStateList(Binder *member)
{
  if (state_list_size_ < STATEMACHINE_STATE_NUM_MAX)
  {
    state_list_[state_list_size_] = member;
    state_list_size_++;
  }
  else
  {
    syslog(LOG_NOTICE, "state list full.");
  }
}

void StateMachine::SendEvent(int event)
{
  // syslog(LOG_NOTICE, "[StateMachine] Send event. DTQ id:%d", dtq_id_);
  snd_dtq(dtq_id_, event);
}

void StateMachine::ReceiveEvent(int event)
{
  last_event_ = event;
  // syslog(LOG_NOTICE, "[StateMachine] Received event:%d", event);
  State *new_state = state_->GetNext(event);
  SetState(new_state);
}

void StateMachine::SetState(State *new_state)
{
  while (new_state != NULL)
  {
    // syslog(LOG_NOTICE, "[StateMachine]Delete old State");
    if (state_ != NULL)
    {
      state_->Leave(this);
      delete state_;
    }
    state_ = new_state;
    State *ret = state_->Enter(this);

    if (ret == NULL)
    {
      break;
    }
    else
    {
      new_state = ret;
    }
  }
}

void StateMachine::Run()
{
  intptr_t dtq_buff = 0;

  // syslog(LOG_NOTICE, "[StateMachine] wait for events...");
  rcv_dtq(dtq_id_, &dtq_buff);
  ReceiveEvent(dtq_buff);
}

int StateMachine::GetLastEvent()
{
  return last_event_;
}
