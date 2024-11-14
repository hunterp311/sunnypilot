#include <QDebug>

#include "selfdrive/ui/qt/offroad/developer_panel.h"
#include "selfdrive/ui/qt/widgets/ssh_keys.h"
#include "selfdrive/ui/qt/widgets/controls.h"

DeveloperPanel::DeveloperPanel(SettingsWindow *parent) : ListWidget(parent) {
  // SSH keys
  addItem(new SshToggle());
  addItem(new SshControl());

  joystickToggle = new ParamControl("JoystickDebugMode", tr("Joystick Debug Mode"), "", "");
  QObject::connect(joystickToggle, &ParamControl::toggleFlipped, [=](bool state) {
    params.putBool("LongitudinalManeuverMode", false);
    longManeuverToggle->refresh();
  });
  addItem(joystickToggle);

  longManeuverToggle = new ParamControl("LongitudinalManeuverMode", tr("Longitudinal Maneuver Mode"), "", "");
  QObject::connect(longManeuverToggle, &ParamControl::toggleFlipped, [=](bool state) {
    params.putBool("JoystickDebugMode", false);
    joystickToggle->refresh();
  });
  addItem(longManeuverToggle);

  // Joystick and longitudinal maneuvers should be hidden on release branches
  // also the toggles should be not available to change in onroad state
  const bool is_release = params.getBool("IsReleaseBranch");
  QObject::connect(uiState(), &UIState::offroadTransition, [=](bool offroad) {
    for (auto btn : findChildren<ParamControl *>()) {
      btn->setVisible(!is_release);
      btn->setEnabled(offroad);
    }
  });

  // FIXME-SP: Move to sunnypilot panels before merging
  auto madsToggle = new ParamControl("Mads", tr("Modular Assistive Driving System (MADS)"), "", "");
  addItem(madsToggle);

  // TODO-SP: Rename toggle
  auto madsCruiseMainToggle = new ParamControl("MadsCruiseMain", tr("MADS: Cruise Main"), "", "");
  addItem(madsCruiseMainToggle);

  auto madsDisengageLateralOnBrakeToggle = new ParamControl("MadsDisengageLateralOnBrake", tr("MADS: Disengage Lateral on Brake"), "", "");
  addItem(madsDisengageLateralOnBrakeToggle);

  auto madsUnifiedEngagementModeToggle = new ParamControl("MadsUnifiedEngagementMode", tr("MADS: Unified Engagement Mode"), "", "");
  addItem(madsUnifiedEngagementModeToggle);

  auto toyotaabhToggle = new ParamControl("ToyotaAutoHold", tr("Toyota: Auto Brake Hold"), "", "");
  addItem(toyotaabhToggle);

  auto toyotaebsmToggle = new ParamControl("ToyotaEnhancedBsm", tr("Toyota: Enhanced BSM"), "", "");
  addItem(toyotaebsmToggle);

  auto toyotatss2longToggle = new ParamControl("ToyotaTSS2Long", tr("Toyota: custom tune"), "", "");
  addItem(toyotatss2longToggle);
}
