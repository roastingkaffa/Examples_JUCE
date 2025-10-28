#include <JuceHeader.h>

namespace HelloWorldLabelExample {
  class Window1 : public DocumentWindow {
  public:
    Window1() : DocumentWindow {"Hello world (Label)", Desktop::getInstance().getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId), DocumentWindow::allButtons} {
      setContentOwned(&mainComponent, false);
      setResizable(true, true);
      setBounds(200, 100, 300, 300);

      label1.setBounds(50, 80, 200, 80);
      label1.setColour(Label::ColourIds::textColourId, Colours::lime);
      label1.setFont({46, Font::FontStyleFlags::bold | Font::FontStyleFlags::italic});
      label1.setJustificationType(Justification::centred);
      label1.setText("Hello new work labe1", NotificationType::dontSendNotification);
      mainComponent.addAndMakeVisible(&label1);

      quitButton.setBounds(75, 180, 150, 40);
      quitButton.setButtonText("Close Window");
      quitButton.onClick = [this] {
        JUCEApplication::quit();
      };
      mainComponent.addAndMakeVisible(&quitButton);
   }

  protected:
    void closeButtonPressed() override {JUCEApplication::quit();}

  private:
    Component mainComponent;
    Label label1;
    TextButton quitButton;
  };
  
  class Application : public JUCEApplication {
  protected:
    const String getApplicationName() override {return "";}
    const String getApplicationVersion() override {return "";}
    
    void initialise(const String& commandLine) override {mainWindow.setVisible(true);}
    void shutdown() override {}
    
  private:
    Window1 mainWindow;
  };
}

START_JUCE_APPLICATION(HelloWorldLabelExample::Application)
