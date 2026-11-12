// Aseprite
// Copyright (C) 2026  Igara Studio S.A.
//
// This program is distributed under the terms of
// the End-User License Agreement for Aseprite.

#ifndef APP_UI_EDITOR_ROTATING_VIEW_STATE_H_INCLUDED
#define APP_UI_EDITOR_ROTATING_VIEW_STATE_H_INCLUDED
#pragma once

#include "app/ui/editor/editor_state.h"

namespace app {

// State to rotate the editor view dragging the mouse around the
// center of the viewport (Ctrl+Shift+Space + drag).
class RotatingViewState : public EditorState {
public:
  RotatingViewState();
  bool isTemporalState() const override { return true; }
  bool onMouseDown(Editor* editor, ui::MouseMessage* msg) override;
  bool onMouseUp(Editor* editor, ui::MouseMessage* msg) override;
  bool onMouseMove(Editor* editor, ui::MouseMessage* msg) override;
  bool onSetCursor(Editor* editor, const gfx::Point& mouseScreenPos) override;
  bool onKeyDown(Editor* editor, ui::KeyMessage* msg) override;
  bool onKeyUp(Editor* editor, ui::KeyMessage* msg) override;
  bool onUpdateStatusBar(Editor* editor) override;

private:
  // Angle (in degrees) of the real mouse position around the
  // rotation center.
  double mouseAngle(Editor* editor) const;

  double m_startMouseAngle = 0.0;
  double m_startRotation = 0.0;
};

} // namespace app

#endif // APP_UI_EDITOR_ROTATING_VIEW_STATE_H_INCLUDED
