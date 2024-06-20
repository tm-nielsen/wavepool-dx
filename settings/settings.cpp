#include "raylib.h"
#include "../utils/file_utils.cpp"

namespace settings {
  #define SAVE_PATH "settings.txt"

  /* Settings object saved as unlabelled plaintext values in the format:
  [volume]
  [window width], [window height]
  [margin], [line thickness]
  [dot radius], [dot spacing]
  [background colour], [guide colour], [main colour]
  [display fps]
  */
  struct Settings
  {
    float volume = 0.5;

    int windowWidth = 600;
    int windowHeight = 600;

    float margin = 40;
    float lineThickness = 4;

    Color backgroundColour = utils::GetColourFromString("#3e6990cc");
    Color accentColour = utils::GetColourFromString("#000000dd");
    Color mainColour = utils::GetColourFromString("#c0fdfb");

    float dotSize = 6;
    float dotSpacing = 16;

    bool showFps = false;


    void SaveToFile(const char* path = SAVE_PATH)
    {
      char* fileText = new char [1000];
      *fileText = '\0';
      utils::ConcatenateLine(fileText, utils::GetString(volume, 2));

      int windowValues [] = {windowWidth, windowHeight};
      utils::ConcatenateLine(fileText, windowValues);

      float styleValues [] = {margin, lineThickness};
      utils::ConcatenateLine(fileText, styleValues);

      float gridValues [] = {dotSize, dotSpacing};
      utils::ConcatenateLine(fileText, gridValues);

      Color colours [] = {backgroundColour, accentColour, mainColour};
      utils::ConcatenateLine(fileText, colours);

      utils::ConcatenateLine(fileText, showFps);

      fileText[strlen(fileText) - 1] = '\0';
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
          // window
          if (std::getline(fileTextStream, fileTextLine, '\n'))
          {
            auto windowStrings = utils::SplitString(fileTextLine);
            loadedSettings.windowWidth = std::stof(windowStrings[0]);
            loadedSettings.windowHeight = std::stof(windowStrings[1]);
          }
          // style values
          if (std::getline(fileTextStream, fileTextLine, '\n'))
          {
            auto styleStrings = utils::SplitString(fileTextLine);
            loadedSettings.margin = std::stof(styleStrings[0]);
            loadedSettings.lineThickness = std::stof(styleStrings[1]);
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
            loadedSettings.accentColour = utils::GetColourFromString(colourStrings[1]);
            loadedSettings.mainColour = utils::GetColourFromString(colourStrings[2]);
          }
          // show fps
          if (std::getline(fileTextStream, fileTextLine, '\n'))
          {
            loadedSettings.showFps = std::stoi(fileTextLine);
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