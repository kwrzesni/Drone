#ifndef ACTION
#define ACTION

enum class ActionType
{
  none,
  changeMenu,
  updateCurrentMenuValue,
  updateCurrentItemValue,
  updateParentMenuValue,
  updateParentItemValue
};

class Action
{
public:
  Action(ActionType actionType = ActionType::none, void* parameter = nullptr);
public:
  ActionType type;
  void* parameter;
};

#endif
