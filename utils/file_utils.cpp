#include "raylib.h"
#include <sstream>
#include <string>
#include <cstring>
#include <vector>

namespace utils {
#ifndef FILE_UTILITIES
#define FILE_UTILITIES

  std::string ExpandShorthandHexString(std::string hexString)
  {
    std::stringstream expandedHexString;
    for (char hexCharacter : hexString)
      expandedHexString << hexCharacter << hexCharacter;
    return expandedHexString.str();
  }

  Color GetColourFromString(std::string hexString)
  {
    if (hexString[0] == '#')
      hexString.erase(0, 1);
    if (hexString.length() == 3 || hexString.length() == 4)
      hexString = ExpandShorthandHexString(hexString);
    if (hexString.length() == 6)
      hexString += "ff";

    unsigned int hexValue;
    std::stringstream ss;
    ss << std::hex << hexString;
    ss >> hexValue;

    return GetColor(hexValue);
  }

  const char* GetColourString(Color colour)
  {
    std::stringstream ss;
    ss << std::hex << ColorToInt(colour);
    return ss.str().c_str();
  }

  std::vector<std::string> SplitString(std::string s, std::string delimeter = ", ")
  {
    std::vector<std::string> result;
    size_t pos = 0;
    while((pos = s.find(delimeter)) != std::string::npos) {
      result.push_back(s.substr(0, pos));
      s.erase(0, pos + delimeter.length());
    }
    result.push_back(s);
    return result;
  }

  void ConcatenateLine(char* s, const char* newLine)
  {
    strcat(s, newLine);
    strcat(s, "\n");
  }

  template <class T>
  void ConcatenateLine(char* s, T value)
  {
    strcat(s, std::to_string(value).c_str());
    strcat(s, "\n");
  }

  void ConcatenateLine(char* s, bool value)
  {
    strcat(s, value? "1": "0");
    strcat(s, "\n");
  }

  template <size_t N>
  void ConcatenateLine(char* s, Color (&colours)[N], const char* delimeter = ", ")
  {
    if (N == 0)
      return;
    strcat(s, GetColourString(colours[0]));
    for (int i = 1; i < N; i++)
    {
      strcat(s, delimeter);
      strcat(s, GetColourString(colours[i]));
    }
    strcat(s, "\n");
  }

  template <size_t N, class T>
  void ConcatenateLine(char* s, T (&values)[N], const char* delimeter = ", ")
  {
    if (N == 0)
      return;
    std::string joinedString = std::to_string(values[0]);
    for (int i = 1; i < N; i++)
    {
      joinedString += delimeter;
      joinedString += std::to_string(values[i]);
    }
    ConcatenateLine(s, joinedString.c_str());
  }
#endif
}