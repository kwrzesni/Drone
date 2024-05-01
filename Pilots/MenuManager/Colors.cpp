#include "Colors.h"

String colorToString(Color color)
{
  switch (color)
  {
    case Color::black:
      return "black";
    case Color::navy:
      return "navy";
    case Color::darkGreen:
      return "dark green";
    case Color::darkCyan:
      return "dark cyan";
    case Color::maroon:
      return "maroon";
    case Color::purple:
      return "purple";
    case Color::olive:
      return "olive";
    case Color::lightGrey:
      return "light grey";
    case Color::darkGrey:
      return "dark grey";
    case Color::blue:
      return "blue";
    case Color::green:
      return "green";
    case Color::cyan:
      return "cyan";
    case Color::red:
      return "red";
    case Color::magenta:
      return "magenta";
    case Color::yellow:
      return "yellow";
    case Color::white:
      return "white";
    case Color::orange:
      return "orange";
    case Color::greenYellow:
      return "green yellow";
  }
  return "?";
}

Color stringToColor(const String& string)
{
  if (string == "black")
  {
    return Color::black;
  }
  if (string == "navy")
  {
    return Color::navy;
  }
  if (string == "dark green")
  {
    return Color::darkGreen;
  }
  if (string == "dark cyan")
  {
    return Color::darkCyan;
  }
  if (string == "maroon")
  {
    return Color::maroon;
  }
  if (string == "purple")
  {
    return Color::purple;
  }
  if (string == "olive")
  {
    return Color::olive;
  }
  if (string == "light grey")
  {
    return Color::lightGrey;
  }
  if (string == "dark grey")
  {
    return Color::darkGrey;
  }
  if (string == "blue")
  {
    return Color::blue;
  }
  if (string == "green")
  {
    return Color::green;
  }
  if (string == "cyan")
  {
    return Color::cyan;
  }
  if (string == "red")
  {
    return Color::red;
  }
  if (string == "magenta")
  {
    return Color::magenta;
  }
  if (string == "yellow")
  {
    return Color::yellow;
  }
  if (string == "white")
  {
    return Color::white;
  }
  if (string == "orange")
  {
    return Color::orange;
  }
  if (string == "green yellow")
  {
    return Color::greenYellow;
  }
  return Color::none;
}