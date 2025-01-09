/*******************************************************************************************
*
*   Debug v1.0.0 - simple debug options for kritters
*
*   MODULE USAGE:
*       #define GUI_DEBUG_IMPLEMENTATION
*       #include "gui_debug.h"
*
*       INIT: GuiDebugState state = InitGuiDebug();
*       DRAW: GuiDebug(&state);
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 gabiru. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_DEBUG_H
#define GUI_DEBUG_H

typedef struct {
    bool Button000Pressed;

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiDebugState;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
GuiDebugState InitGuiDebug(void);
void GuiDebug(GuiDebugState *state);

#ifdef __cplusplus
}
#endif

#endif // GUI_DEBUG_H

/***********************************************************************************
*
*   GUI_DEBUG IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_DEBUG_IMPLEMENTATION)

#include "raygui.h"

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Internal Module Functions Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
GuiDebugState InitGuiDebug(void)
{
    GuiDebugState state = { 0 };

    state.Button000Pressed = false;

    // Custom variables initialization

    return state;
}

void GuiDebug(GuiDebugState *state)
{
    state->Button000Pressed = GuiButton((Rectangle){ 24, 64, 120, 24 }, "Show Resolution"); 
    GuiLabel((Rectangle){ 24, 88, 120, 24 }, "SAMPLE TEXT");
    GuiLabel((Rectangle){ 24, 112, 120, 24 }, "SAMPLE TEXT");
}

#endif // GUI_DEBUG_IMPLEMENTATION
