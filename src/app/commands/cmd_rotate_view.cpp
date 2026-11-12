// Aseprite
// Copyright (C) 2026  Igara Studio S.A.
//
// This program is distributed under the terms of
// the End-User License Agreement for Aseprite.

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

#include "app/commands/command.h"
#include "app/commands/params.h"
#include "app/i18n/strings.h"
#include "app/ui/editor/editor.h"
#include "ui/manager.h"
#include "ui/system.h"

#include <cmath>
#include <cstdlib>

namespace app {

// Rotates the view of the editor (not the sprite pixels).
//
// Params:
//   angle=N   Rotates the view N degrees (clockwise) from the current rotation
//   reset=true  Resets the view rotation to 0 degrees
class RotateViewCommand : public Command {
public:
  RotateViewCommand();

protected:
  bool onNeedsParams() const override { return true; }
  void onLoadParams(const Params& params) override;
  bool onEnabled(Context* context) override;
  void onExecute(Context* context) override;
  std::string onGetFriendlyName() const override;

private:
  double m_angle = 15.0;
  bool m_reset = false;
};

RotateViewCommand::RotateViewCommand() : Command(CommandId::RotateView())
{
}

void RotateViewCommand::onLoadParams(const Params& params)
{
  m_angle = 15.0;
  const std::string angle = params.get("angle");
  if (!angle.empty())
    m_angle = std::strtod(angle.c_str(), nullptr);

  m_reset = (params.get("reset") == "true");
}

bool RotateViewCommand::onEnabled(Context* context)
{
  return (Editor::activeEditor() != nullptr);
}

void RotateViewCommand::onExecute(Context* context)
{
  // Use the editor below the mouse (or the active editor by default)
  Editor* editor = Editor::activeEditor();
  ui::Widget* pick = ui::Manager::getDefault()->pickFromScreenPos(ui::get_mouse_position());
  if (pick && pick->type() == Editor::Type())
    editor = static_cast<Editor*>(pick);
  if (!editor)
    return;

  editor->setViewRotation(m_reset ? 0.0 : editor->viewRotation() + m_angle);
}

std::string RotateViewCommand::onGetFriendlyName() const
{
  if (m_reset)
    return Strings::commands_RotateView_Reset();
  return Strings::commands_RotateView_Angle(m_angle);
}

Command* CommandFactory::createRotateViewCommand()
{
  return new RotateViewCommand;
}

} // namespace app
