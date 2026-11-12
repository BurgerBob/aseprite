// Aseprite
// Copyright (C) 2026  Igara Studio S.A.
//
// This program is distributed under the terms of
// the End-User License Agreement for Aseprite.

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

#include "app/ui/editor/rotating_view_state.h"

#include "app/ui/editor/editor.h"
#include "app/ui/skin/skin_theme.h"
#include "app/ui/status_bar.h"
#include "base/pi.h"
#include "fmt/format.h"
#include "ui/message.h"

#include <cmath>

namespace app {

using namespace ui;

RotatingViewState::RotatingViewState()
{
}

double RotatingViewState::mouseAngle(Editor* editor) const
{
  // We use the real mouse position (not the virtual one of the
  // message) because the virtual coordinates change with the
  // rotation.
  const gfx::Point pos = editor->realMousePosInDisplay();
  const gfx::PointF center = editor->viewRotationCenter();
  return std::atan2(pos.y - center.y, pos.x - center.x) * 180.0 / PI;
}

bool RotatingViewState::onMouseDown(Editor* editor, MouseMessage* msg)
{
  m_startMouseAngle = mouseAngle(editor);
  m_startRotation = editor->viewRotation();

  editor->captureMouse();
  return true;
}

bool RotatingViewState::onMouseUp(Editor* editor, MouseMessage* msg)
{
  editor->backToPreviousState();
  editor->releaseMouse();
  return true;
}

bool RotatingViewState::onMouseMove(Editor* editor, MouseMessage* msg)
{
  double rotation = m_startRotation + mouseAngle(editor) - m_startMouseAngle;

  // Alt: rotate by steps of 15 degrees
  if (msg->altPressed()) {
    rotation = 15.0 * std::round(rotation / 15.0);
  }
  // Snap to right angles (0, 90, 180, 270) when we are near
  else {
    const double rightAngle = 90.0 * std::round(rotation / 90.0);
    if (std::fabs(rotation - rightAngle) < 4.0)
      rotation = rightAngle;
  }

  editor->setViewRotation(rotation);
  onUpdateStatusBar(editor);
  return true;
}

bool RotatingViewState::onSetCursor(Editor* editor, const gfx::Point& mouseScreenPos)
{
  auto theme = skin::SkinTheme::get(editor);
  editor->showMouseCursor(kCustomCursor, theme->cursors.rotateN());
  return true;
}

bool RotatingViewState::onKeyDown(Editor* editor, KeyMessage* msg)
{
  // Esc resets the rotation
  if (msg->scancode() == kKeyEsc) {
    m_startRotation = 0.0;
    m_startMouseAngle = mouseAngle(editor);
    editor->setViewRotation(0.0);
    onUpdateStatusBar(editor);
    return true;
  }
  return false;
}

bool RotatingViewState::onKeyUp(Editor* editor, KeyMessage* msg)
{
  return false;
}

bool RotatingViewState::onUpdateStatusBar(Editor* editor)
{
  StatusBar::instance()->setStatusText(
    0,
    fmt::format("View rotation {:.1f}\xC2\xB0 (Alt: 15\xC2\xB0 steps, Esc: reset)",
                editor->viewRotation()));
  return true;
}

} // namespace app
