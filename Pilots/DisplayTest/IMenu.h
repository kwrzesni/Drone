#ifndef IMENU
#define IMENU

#include <ILI9488.h>

class MenuManager;
enum class MenuControlCommand;

class IMenu
{
public:
  IMenu(MenuManager& menuManager);
  void setParent(IMenu* parent);
  IMenu* getParent() const;
  void lowerMenuVersion();
  virtual void draw() = 0;
  virtual void update(MenuControlCommand menuControlCommand) = 0;
  virtual void resetSelection() = 0;
  virtual String getValue() const = 0;
  virtual String getValue(int ind) const = 0;
  virtual void setValue(const String& value) = 0;
  virtual void setSelectedItemValue(const String& value) = 0;
  virtual void updateValue(const String& value) = 0;
  virtual void showValueError();
  virtual ~IMenu() = default;
protected:
  MenuManager& menuManager;
  IMenu* parent = nullptr;
  int menuVersion = 0;
};

#endif
