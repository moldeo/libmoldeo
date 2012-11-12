/*******************************************************************************

                               moLuaBase.cpp

  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *   (at your option) any later version.                                    *
  *                                                                          *
  *   This code is distributed in the hope that it will be useful, but       *
  *   WITHOUT ANY WARRANTY; without even the implied warranty of             *
  *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      *
  *   General Public License for more details.                               *
  *                                                                          *
  *   A copy of the GNU General Public License is available on the World     *
  *   Wide Web at <http://www.gnu.org/copyleft/gpl.html>. You can also       *
  *   obtain it by writing to the Free Software Foundation,                  *
  *   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.         *
  *                                                                          *
  ****************************************************************************

  Copyright (C) 2006 Fabricio Costa

  Authors:
  Fabricio Costa
  Andrés Colubri

  Description:
  Classes for Lua scripting. Based in the Lua article and VC project by Richard
  Shephard, available online at:
  http://www.codeproject.com/cpp/luaincpp.asp?msg=1741798

*******************************************************************************/

#include "moLuaBase.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

//============================================================================
// int printMessage
//---------------------------------------------------------------------------
// Prints a message to the console
//
// Parameter   Dir      Description
// ---------   ---      -----------
// lua         IN       State variable
//
// Return
// ------
// Number of return varaibles on the stack
//
//============================================================================
static int printMessage (lua_State *lua)
{
   assert (lua_isstring (lua,1));

   const char *msg = lua_tostring (lua, 1);

   // get caller
   lua_Debug ar;
   memset (&ar, 0, sizeof(ar));
   lua_getstack (lua, 1, &ar);
   lua_getinfo (lua, "Snl", &ar);

   // debug output
   const char *str = ar.source;
   printf ("script: %s -- at %s(%d)\n", msg, str, ar.currentline);
   return 0;
}

//============================================================================
// moLuaVirtualMachine::moLuaVirtualMachine
//---------------------------------------------------------------------------
// Constructor. Setups the default VM state
//
// Parameter   Dir      Description
// ---------   ---      -----------
//
//
// Return
// ------
// None.
//
//============================================================================
moLuaVirtualMachine::moLuaVirtualMachine (void) : m_pState (NULL), m_pDbg (NULL)
{
   m_fIsOk = false;
}

//============================================================================
// moLuaVirtualMachine::moLuaVirtualMachine
//---------------------------------------------------------------------------
// Destructor. Closes the VM
//
// Parameter   Dir      Description
// ---------   ---      -----------
//
//
// Return
// ------
// None.
//
//============================================================================
moLuaVirtualMachine::~moLuaVirtualMachine (void)
{
   FinaliseVM();
}

//============================================================================
// moLuaVirtualMachine::Panic
//---------------------------------------------------------------------------
// When things in Lua go wrong (ever called in protected mode??)
//
// Parameter   Dir      Description
// ---------   ---      -----------
// lua         IN       State variable
//
// Return
// ------
// None.
//
//============================================================================
void moLuaVirtualMachine::Panic (lua_State *lua)
{
}

//============================================================================
// bool moLuaVirtualMachine::InitialiseVM
//---------------------------------------------------------------------------
// Initialises the VM, open lua, makes sure things are OK
//
// Parameter   Dir      Description
// ---------   ---      -----------
// None.
//
// Return
// ------
// Success.
//
//============================================================================
bool moLuaVirtualMachine::InitialiseVM (void)
{
   // Open Lua!
   if (Ok ()) FinaliseVM ();

   m_pState = lua_open ();

   if (m_pState)
   {
      m_fIsOk = true;

      // Load util libs into lua
      luaL_openlibs (m_pState);

      // setup global printing (trace)
      lua_pushcclosure (m_pState, printMessage, 0);
      lua_setglobal (m_pState, "trace");

      lua_atpanic (m_pState, (lua_CFunction) moLuaVirtualMachine::Panic);

      return true;
   }

   return false;
}

//============================================================================
// bool moLuaVirtualMachine::FinaliseVM
//---------------------------------------------------------------------------
// Clears the current Lua state
//
// Parameter   Dir      Description
// ---------   ---      -----------
// None.
//
// Return
// ------
// Success.
//
//============================================================================
bool moLuaVirtualMachine::FinaliseVM (void)
{
   if (m_pState)
   {
      lua_close (m_pState);
      m_pState = NULL;
      m_fIsOk = false;
   }
   return true;
}


//============================================================================
// bool moLuaVirtualMachine::RunFile
//---------------------------------------------------------------------------
// Compiles and runs a lua script file
//
// Parameter   Dir      Description
// ---------   ---      -----------
// strFilename IN       Filename to compile and run
//
// Return
// ------
// Success.
//
//============================================================================
bool moLuaVirtualMachine::RunFile (const char *strFilename)
{
   bool fSuccess = false;
   int iErr = 0;

   if ((iErr = luaL_loadfile (m_pState, strFilename)) == 0)
   {
      // Call main...
       if ((iErr = lua_pcall (m_pState, 0, LUA_MULTRET, 0)) == 0)
       {
          fSuccess = true;
       }
   }

   if (fSuccess == false)
   {
      if (m_pDbg != NULL) m_pDbg->ErrorRun (iErr);
   }

   return fSuccess;
}

//============================================================================
// bool moLuaVirtualMachine::RunBuffer
//---------------------------------------------------------------------------
// Compiles and runs a pre-compiled data buffer
//
// Parameter   Dir      Description
// ---------   ---      -----------
// pbBuffer    IN       Buffer to run
// szLen       IN       Length of buffer
// strName     IN       Name of Buffer
//
// Return
// ------
// Success.
//
//============================================================================
bool moLuaVirtualMachine::RunBuffer (const unsigned char *pbBuffer, size_t szLen, const char *strName /* = NULL */)
{
   bool fSuccess = false;
   int iErr = 0;

   if (strName == NULL)
   {
      strName = "Temp";
   }

   if ((iErr = luaL_loadbuffer (m_pState, (const char *) pbBuffer, szLen, strName)) == 0)
   {
      // Call main...
       if ((iErr = lua_pcall (m_pState, 0, LUA_MULTRET, 0)) == 0)
       {
          fSuccess = true;
       }
   }

   if (fSuccess == false)
   {
      if (m_pDbg != NULL) m_pDbg->ErrorRun (iErr);
   }

   return fSuccess;

}

//============================================================================
// moLuaVirtualMachine::CallFunction
//---------------------------------------------------------------------------
// Calls a function that is already on the stack
//
// Parameter   Dir      Description
// ---------   ---      -----------
// nArgs       IN       Args that are aleady on the stack
// nReturns    IN       Number of expected returns (will be on the stack)
//
// Return
// ------
// Success.
//
//============================================================================
bool moLuaVirtualMachine::CallFunction (int nArgs, int nReturns /* = 0 */)
{
   bool fSuccess = false;

   if (lua_isfunction (m_pState, -nArgs-1))
   {
      int iErr = 0;
      iErr = lua_pcall (m_pState, nArgs, nReturns, 0);

      if (iErr == 0)
      {
         fSuccess = true;
      }
      else
      {
         if (m_pDbg != NULL) m_pDbg->ErrorRun (iErr);
      }
   }

   return fSuccess;
}

// typedef void (*lua_Hook) (lua_State *L, lua_Debug *ar);
static void LuaHookCall (lua_State *lua)
{

   printf ("---- Call Stack ----\n");
//   printf ("[Level] [Function] [# args] [@line] [src]\n");

   lua_Debug ar;

   // Look at call stack
   for (int iLevel = 0; lua_getstack (lua, iLevel, &ar) != 0; ++iLevel)
   {
      if (lua_getinfo (lua, "Snlu", &ar) != 0)
      {
         printf ("%d %s %s %d @%d %s\n", iLevel, ar.namewhat, ar.name, ar.nups, ar.linedefined, ar.short_src);
      }
   }
}

static void LuaHookRet (lua_State *lua)
{

}

static void LuaHookLine (lua_State *lua)
{
   lua_Debug ar;
   lua_getstack (lua, 0, &ar);


   if (lua_getinfo (lua, "Sl", &ar) != 0)
   {
      printf ("exe %s on line %d\n", ar.short_src, ar.currentline);
   }
}

static void LuaHookCount (lua_State *lua)
{
   LuaHookLine (lua);
}

static void LuaHook (lua_State *lua, lua_Debug *ar)
{
   // Handover to the correct hook
   switch (ar->event)
   {
   case LUA_HOOKCALL:
      LuaHookCall (lua);
      break;
   case LUA_HOOKRET:
   case LUA_HOOKTAILRET:
      LuaHookRet (lua);
      break;
   case LUA_HOOKLINE:
      LuaHookLine (lua);
      break;
   case LUA_HOOKCOUNT:
      LuaHookCount (lua);
      break;
   }
}

moLuaDebugger::moLuaDebugger (moLuaVirtualMachine& vm) : m_iCountMask (10), m_vm (vm)
{
}

moLuaDebugger::~moLuaDebugger (void)
{
	FinaliseDBG();
}

void moLuaDebugger::InitaliseDBG()
{

   // Clear all current hooks and attach debugger to vm.
   if (m_vm.Ok ())
   {
      m_vm.AttachDebugger (this);
      lua_sethook ((lua_State *) m_vm, LuaHook, 0, m_iCountMask);
   }
}

void moLuaDebugger::FinaliseDBG (void)
{
   // Clear all current hooks
   if (m_vm.Ok ())
   {
      lua_sethook ((lua_State *) m_vm, LuaHook, 0, m_iCountMask);
   }
}

void moLuaDebugger::SetHooksFlag (int iMask)
{
   // Set hooks
   lua_sethook ((lua_State *) m_vm, LuaHook, iMask, m_iCountMask);
}

void moLuaDebugger::ErrorRun (int iErrorCode)
{
   switch (iErrorCode)
   {
   case LUA_ERRRUN:
      printf ("LUA_ERRRUN\n");
	  if (MODebug2 != NULL) MODebug2->Error("LUA_ERRRUN");
      break;
   case LUA_ERRMEM:
      printf ("LUA_ERRMEM\n");
	  if (MODebug2 != NULL) MODebug2->Error("LUA_ERRMEM");
      break;
   case LUA_ERRERR:
      printf ("LUA_ERRERR\n");
	  if (MODebug2 != NULL) MODebug2->Error("LUA_ERRERR");
      break;
   }

   // Get the error string that appears on top of stack when a function
   // fails to run
   printf ("LUA Error: %s\n", lua_tostring ((lua_State *) m_vm, -1));
   if (MODebug2!= NULL) MODebug2->Error(moText("LUA Error: ") + moText((char*)lua_tostring ((lua_State *) m_vm, -1)));
}

