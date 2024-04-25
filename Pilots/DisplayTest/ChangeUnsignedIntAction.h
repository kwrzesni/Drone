#ifndef CHANGE_UNSIGNED_INT_ACTION
#define CHANGE_UNSIGNED_INT_ACTION

#include "IAction.h"

class IMenu;
class MenuManager;

class ChangeUnsignedIntAction : public IAction
{
public:
  ChangeUnsignedIntAction(MenuManager& menuManager);
  void run() const override;
private:
  MenuManager& menuManager;
};

#endif
