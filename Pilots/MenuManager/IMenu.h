#ifndef IMENU
#define IMENU

#include <ILI9488.h>

class MenuManager;
class Action;

class IMenu
{
public:
  IMenu(MenuManager& menuManager);
  void setParent(IMenu* parent);
  IMenu* getParent() const;
  int getMenuVersion() const;
  virtual const Action& getAction() const = 0;
  virtual void onLeft();
  virtual void onRigth();
  virtual void onUp();
  virtual void onDown();
  virtual void draw() = 0;
  virtual void resetSelection() = 0;
  virtual String getValue() const = 0;
  virtual String getItemValue() const = 0;
  virtual bool setValue(const String& value) = 0;
  virtual bool setItemValue(const String& value) = 0;
  virtual bool updateValue(const String& value) = 0;
  virtual bool updateItemValue(const String& value) = 0;
  virtual bool isValidValue(const String& value) = 0;
  virtual bool isValidItemValue(const String& value) = 0;
  virtual void showValueError();
  virtual void updateLayout();
  virtual ~IMenu();
protected:
  MenuManager& menuManager;
  IMenu* parent = nullptr;
  int menuVersion = 0;
};

#endif
