#include "raylib.h"
#include "../utils/file_utils.cpp"

namespace settings {
  #define SAVE_PATH "settings.txt"

  /* Settings object saved as unlabelled plaintext values in the format:
  [volume]
  [fullscreen]
  [window width], [window height], [margin], 
  [dot radius], [dot spacing]
  [background colour], [guide colour], [main colour]
  */
  struct Settings
  {
    float volume = 0.5;

    bool fullscreen = false;
    int windowWidth = 600;
    int windowHeight = 600;
    int margin = 40;

    float dotSize = 6;
    float dotSpacing = 16;

    Color backgroundColour = utils::GetColourFromString("#3e6990cc");
    Color guideColour = utils::GetColourFromString("#000000dd");
    Color mainColour = utils::GetColourFromString("#c0fdfb");


    void SaveToFile(const char* path = SAVE_PATH)
    {
      char* fileText = new char [1000];
      *fileText = '\0';
      utils::ConcatenateLine(fileText, volume);
      utils::ConcatenateLine(fileText, fullscreen);

      int windowValues [] = {windowWidth, windowHeight, margin};
      utils::ConcatenateLine(fileText, windowValues);

      float gridValues [] = {dotSize, dotSpacing};
      utils::ConcatenateLine(fileText, gridValues);

      Color colours [] = {backgroundColour, guideColour, mainColour};
      utils::ConcatenateLine(fileText, colours);

      SaveFileText(path, fileText);
      delete fileText;
    }

    static Settings LoadFromFile(const char* path = SAVE_PATH)
    {
      if (FileExists(path))
      {
        char* fileText = LoadFileText(path);
        std::stringstream fileTextStream(fileText);
        std::string fileTextLine;

        if (fileText != NULL)
        {
          Settings loadedSettings = Settings();
          // volume
          if (std::getline(fileTextStream, fileTextLine, '\n'))
          {
            loadedSettings.volume = std::stof(fileTextLine);
          }
          // fullscreen
          if (std::getline(fileTextStream, fileTextLine, '\n'))
          {
            loadedSettings.fullscreen = std::stoi(fileTextLine);
          }
          // window
          if (std::getline(fileTextStream, fileTextLine, '\n'))
          {
            auto windowStrings = utils::SplitString(fileTextLine);
            loadedSettings.windowWidth = std::stof(windowStrings[0]);
            loadedSettings.windowHeight = std::stof(windowStrings[1]);
            loadedSettings.margin = std::stof(windowStrings[2]);
          }
          // dot size and spacing
          if (std::getline(fileTextStream, fileTextLine, '\n'))
          {
            auto numberStrings = utils::SplitString(fileTextLine);
            loadedSettings.dotSize = std::stof(numberStrings[0]);
            loadedSettings.dotSpacing = std::stof(numberStrings[1]);
          }
          // colours
          if (std::getline(fileTextStream, fileTextLine, '\n'))
          {
            auto colourStrings = utils::SplitString(fileTextLine);
            loadedSettings.backgroundColour = utils::GetColourFromString(colourStrings[0]);
            loadedSettings.guideColour = utils::GetColourFromString(colourStrings[1]);
            loadedSettings.mainColour = utils::GetColourFromString(colourStrings[2]);
          }
          return loadedSettings;
        }
      }
      Settings defaultSettings = Settings();
      defaultSettings.SaveToFile(path);
      return defaultSettings;
    }
  };
}