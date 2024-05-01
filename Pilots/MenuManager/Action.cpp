#include "Action.h"

Action::Action(ActionType actionType, void* parameter)
  : type{actionType}, parameter{parameter}
{
}