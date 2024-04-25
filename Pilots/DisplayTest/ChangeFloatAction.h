#ifndef CHANGE_FLOAT_ACTION
#define CHANGE_FLOAT_ACTION

#include "IAction.h"

class IMenu;
class MenuManager;

class ChangeFloatAction : public IAction
{
public:
  ChangeFloatAction(MenuManager& menuManager);
  void run() const override;
private:
  MenuManager& menuManager;
};

#endif
