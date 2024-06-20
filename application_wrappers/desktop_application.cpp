#include "wave_pool_application.cpp"

#include "../window_management/window_manager.cpp"
#include "../settings/settings_menu.cpp"

using namespace window_management;
using namespace settings;

namespace application_wrappers {
  class DesktopApplication: public WavePoolApplication
  {
    protected:
      WindowManager* windowManager = nullptr;
      SettingsMenu* settingsMenu = nullptr;
      Settings settings;

      rect backgroundRect;
      bool settingsMenuIsOpen = false;

      void DrawFpsCounter();
      void ToggleSettingsMenu();
      void CloseWindow();
      void OnWindowResized(vec2);
      void Resize(vec2);
      void UpdateMargin();
      void UpdateStyle();
      void UpdateWaveGridLayout();
      void OnSettingsReset();
      vec2 GetSavedWindowSize();

    public:
      ~DesktopApplication()
      {
        WavePoolApplication::~WavePoolApplication();
        delete windowManager;
        delete settingsMenu;
      }

      void InitializeApplication()
      {
        settings = Settings::LoadFromFile();
        vec2 screenSize = vec2(settings.windowWidth, settings.windowHeight);
        float margin = settings.margin;

        float volume = settings.volume;

        Color mainColour = settings.mainColour;
        this->backgroundColour = settings.backgroundColour;
        Color accentColour = settings.accentColour;
        float lineThickness = settings.lineThickness;

        float dotSize = settings.dotSize;
        float dotSpacing = settings.dotSpacing;


        SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        SetConfigFlags(FLAG_WINDOW_UNDECORATED);

        WavePoolApplication::InitializeApplication(
          screenSize, margin, volume,
          mainColour, backgroundColour, accentColour,
          lineThickness, dotSize, dotSpacing
        );

        backgroundRect = rect(margin, screenSize - (2 * margin)).ToRectangle();
        
        windowManager = new WindowManager(vec2(500));
        windowManager->LoadResources();
        windowManager->SetStyle(accentColour, mainColour, lineThickness);
        windowManager->OnWindowResized(screenSize, margin);

        settingsMenu = new SettingsMenu(&settings);
        settingsMenu->LoadResources();
        settingsMenu->SetStyle(accentColour, mainColour, lineThickness);
        settingsMenu->OnWindowResized(screenSize, margin);

        BindCallbacks();
      }

      void BindCallbacks()
      {
        windowManager->onSettingsButtonPressed = std::bind(ToggleSettingsMenu, this);
        windowManager->onCloseButtonPressed = std::bind(CloseWindow, this);
        windowManager->onWindowResized = std::bind(OnWindowResized, this, _1);

        settingsMenu->onStyleModified = std::bind(UpdateStyle, this);
        settingsMenu->onMarginModified = std::bind(UpdateMargin, this);
        settingsMenu->onWaveGridLayoutModified = std::bind(UpdateWaveGridLayout, this);
        settingsMenu->onReset = std::bind(OnSettingsReset, this);
      }

      void UnloadResources()
      {
        windowManager->UnloadResources();
        settingsMenu->UnloadResources();
        WavePoolApplication::UnloadResources();
      }

      void Update()
      {
        windowManager->Update();
        
        if (settingsMenuIsOpen) {
            settingsMenu->Update();
        }
        else {
            radialInstrument->Update();
            wavePool->Update();
        }
      }

      void Draw()
      {
        BeginDrawing();

        ClearBackground(BLANK);
        backgroundRect.DrawRoundedFilled(backgroundColour, 0.075);
        if (settings.showFps)
          DrawFpsCounter();

        windowManager->Draw();
        if(settingsMenuIsOpen) {
          settingsMenu->Draw();
        }
        else {
          radialInstrument->Draw();
          wavePool->Draw();
        }

        EndDrawing();
      }
  };

  void DesktopApplication::DrawFpsCounter()
  {
    auto displayString = utils::GetString(1 / GetFrameTime()).c_str();
    float offset = settings.lineThickness;
    float fontSize = settings.margin - 2 * offset;
    Color fontColour = settings.accentColour;
    DrawText(displayString, offset, offset, fontSize, fontColour);
  }

  void DesktopApplication::ToggleSettingsMenu()
  {
    settingsMenuIsOpen = !settingsMenuIsOpen;
  }

  void DesktopApplication::CloseWindow()
  {
    shouldExit = true;
  }

  void DesktopApplication::OnWindowResized(vec2 windowSize)
  {
    settings.windowWidth = windowSize.x;
    settings.windowHeight = windowSize.y;
    settings.SaveToFile();
    Resize(windowSize);
  }

  void DesktopApplication::Resize(vec2 screenSize)
  {
    float margin = settings.margin;
    backgroundRect = rect(margin, screenSize - (2 * margin)).ToRectangle();
    
    windowManager->OnWindowResized(screenSize, margin);
    settingsMenu->OnWindowResized(screenSize, margin);
    radialInstrument->OnWindowResized(screenSize, margin);
    wavePool->OnWindowResized(screenSize, margin);
  }

  void DesktopApplication::UpdateMargin()
  {
    Resize(GetSavedWindowSize());
  }

  void DesktopApplication::UpdateStyle()
  {
    vec2 windowSize = GetSavedWindowSize();
    float margin = settings.margin;
  
    Color mainColour = settings.mainColour;
    backgroundColour = settings.backgroundColour;
    Color accentColour = settings.accentColour;
    float lineThickness = settings.lineThickness;

    windowManager->SetStyle(accentColour, mainColour, lineThickness);
    windowManager->OnWindowResized(windowSize, margin);
    settingsMenu->SetStyle(accentColour, mainColour, lineThickness);
    settingsMenu->OnWindowResized(windowSize, margin);
    wavePool->SetColour(mainColour);
  }

  void DesktopApplication::UpdateWaveGridLayout()
  {
    float dotSize = settings.dotSize;
    float dotSpacing = settings.dotSpacing;

    wavePool->SetGridLayout(dotSize, dotSpacing);
  }

  void DesktopApplication::OnSettingsReset()
  {
    UpdateMargin();
    UpdateStyle();
    UpdateWaveGridLayout();
  }

  vec2 DesktopApplication::GetSavedWindowSize()
  {
    return vec2(settings.windowWidth, settings.windowHeight);
  }
}