/*******************************************************************************

                                moScript.cpp

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


  Description:
  Base class for Lua-scriptable classes.

*******************************************************************************/

#include <assert.h>
#include "moScript.h"
#include "moScriptManager.h"

moLuaVirtualMachine moScript::m_vm;
moLuaDebugger moScript::m_dbg (moScript::m_vm);


// ---------------------------------------------------------------------------

#define BEGIN_LUA_CHECK(vm)   lua_State *state = (lua_State *) vm; \
                              if (vm.Ok ()) {
#define END_LUA_CHECK         }


//============================================================================
// int LuaCallback
//---------------------------------------------------------------------------
// Lua C-API calling that figures out which object to hand over to
//
// Parameter   Dir      Description
// ---------   ---      -----------
// lua         IN       State variable
//
// Return
// ------
// Number of return varaibles on the stack
//
// Comments
// --------
// This is the function lua calls for each C-Function that is
// registered with lua. At the time of registering the function
// with lua, we make lua record the method "number" so we can
// know what method was actually called. The lua stack is the
// following structure:
// 0: 'this' (table)
// 1 - ...: parameters passed in
//
//============================================================================
static int LuaCallback (lua_State *lua)
{
   // Locate the psudo-index for the function number
   int iNumberIdx = lua_upvalueindex (1);
   int nRetsOnStack = 0;

   bool fSuccess = false;

   // Check for the "this" table
   if (lua_istable (lua, 1))
   {
      // Found the "this" table. The object pointer is at the index 0
      lua_rawgeti (lua, 1, 0);

      if (lua_islightuserdata (lua, -1))
      {
         // Found the pointer, need to cast it
         moScript *pThis = (moScript *) lua_touserdata (lua, -1);

         // Get the method index
         int iMethodIdx = (int) lua_tonumber (lua, iNumberIdx);

         // Check that the method is correct index
         assert (!(iMethodIdx > pThis->methods ()));

         // Reformat the stack so our parameters are correct
         // Clean up the "this" table
         lua_remove (lua, 1);
         // Clean up the pThis pointer
         lua_remove (lua, -1);

         // Call the class
         nRetsOnStack = pThis->ScriptCalling (pThis->vm (), iMethodIdx);

         fSuccess = true;
      }
   }

   if (fSuccess == false)
   {
      lua_pushstring (lua, "LuaCallback -> Failed to call the class function");
      lua_error (lua);
   }

   // Number of return variables
   return nRetsOnStack;
}


//============================================================================
// moScript::moScript
//---------------------------------------------------------------------------
// Default constructor.
//
// Parameter            Dir      Description
// ---------            ---      -----------
// None.
//
// Return
// ------
// None.
//
//============================================================================
moScript::moScript ()
 : m_initialized(false), m_nMethods (0), m_iThisRef (0), m_nArgs (0), m_iMethodBaseIterator(0), m_iMethodBaseAncestors(0), m_iMethodBase(-1)
{
	memset( m_MethodBases, -1, sizeof(int)*256);
}

//============================================================================
// moScript::~moScript
//---------------------------------------------------------------------------
// Destructor
//
// Parameter   Dir      Description
// ---------   ---      -----------
// None.
//
// Return
// ------
// None.
//
//============================================================================
moScript::~moScript (void)
{
   if (m_initialized) FinishScript();
}

//============================================================================
// moScript::Init
//---------------------------------------------------------------------------
// Function
//
// Parameter   Dir      Description
// ---------   ---      -----------
// Initializes the script with the VM
//
// Return
// ------
// None.
//
//============================================================================
void moScript::InitScript()
{

    ///Inicializamos el moLuaVirtualMachine con la funcion estatica del moScriptManager
   if (!m_vm.Ok()) moScriptManager::InitVM();

   BEGIN_LUA_CHECK (m_vm)
      // Create a reference to the "this" table. Each reference is unique
      lua_newtable (state);
      m_iThisRef = luaL_ref (state, LUA_REGISTRYINDEX);

      // Save the "this" table to index 0 of the "this" table
      moLuaRestoreStack rs (m_vm);
      lua_rawgeti (state, LUA_REGISTRYINDEX, m_iThisRef);
      lua_pushlightuserdata (state, (void *) this);
      lua_rawseti (state, -2, 0);

	  m_initialized = true;
   END_LUA_CHECK
}

void moScript::FinishScript()
{
   moLuaRestoreStack rs (m_vm);

   BEGIN_LUA_CHECK (m_vm)
      // Get the reference "this" table
      lua_rawgeti (state, LUA_REGISTRYINDEX, m_iThisRef);

      // Clear index 0
      lua_pushnil (state);
      lua_rawseti (state, -2, 0);

      m_initialized = false;
   END_LUA_CHECK
}

//============================================================================
// bool moScript::CompileBuffer
//---------------------------------------------------------------------------
// Compiles a given buffer
//
// Parameter   Dir      Description
// ---------   ---      -----------
// pbBuffer    IN       Data buffer
// szLen       IN       Length of buffer
//
// Return
// ------
// Success
//
//============================================================================
bool moScript::CompileBuffer (unsigned char *pbBuffer, size_t szLen)
{
   assert (pbBuffer != NULL && "moScript::CompileBuffer ->  pbBuffer == NULL");
   assert (szLen != 0 && "moScript::CompileBuffer -> szLen == 0");
   assert (m_vm.Ok () && "VM Not OK");

   // Make sure we have the correct "this" table
   moLuaThis luaThis (m_vm, m_iThisRef);

   return m_vm.RunBuffer (pbBuffer, szLen);
}

//============================================================================
// bool moScript::CompileBuffer
//---------------------------------------------------------------------------
// Compiles a given file
//
// Parameter   Dir      Description
// ---------   ---      -----------
// strFilename IN       File name to compile
//
// Return
// ------
// Success
//
//============================================================================
bool moScript::CompileFile (const char *strFilename)
{
   assert (strFilename != NULL && "moScript::CompileFile -> strFilename == NULL");
   assert (m_vm.Ok () && "VM Not OK");

   // Make sure we have the correct "this" table
   moLuaThis luaThis (m_vm, m_iThisRef);

   return m_vm.RunFile (strFilename);
}

//============================================================================
// int moScript::RegisterFunction
//---------------------------------------------------------------------------
// Registers a function with Lua
//
// Parameter   Dir      Description
// ---------   ---      -----------
// strFuncName IN       Function name
//
// Return
// ------
// Success
//
//============================================================================
int moScript::RegisterFunction (const char *strFuncName, moScript::Function& fun )
{
   assert (strFuncName != NULL && "moScript::RegisterFunction -> strFuncName == NULL");
   assert (m_vm.Ok () && "VM Not OK");

   int iMethodIdx = -1;

   moLuaRestoreStack rs (m_vm);

   BEGIN_LUA_CHECK (m_vm)
      iMethodIdx = ++m_nMethods;
      Functions[iMethodIdx] = fun;

      // Register a function with the lua script. Added it to the "this" table
      lua_rawgeti (state, LUA_REGISTRYINDEX, m_iThisRef);

      // Push the function and parameters
      lua_pushstring (state, strFuncName);
      lua_pushnumber (state, (lua_Number) iMethodIdx);
      lua_pushcclosure (state, LuaCallback, 1);
      lua_settable (state, -3);

   END_LUA_CHECK

   return iMethodIdx;
}

int moScript::RegisterFunction (const char *strFuncName)
{
   assert (strFuncName != NULL && "moScript::RegisterFunction -> strFuncName == NULL");
   assert (m_vm.Ok () && "VM Not OK");

   int iMethodIdx = -1;

   moLuaRestoreStack rs (m_vm);

   BEGIN_LUA_CHECK (m_vm)
      iMethodIdx = ++m_nMethods;
      //Functions[iMethodIdx] = ;

      // Register a function with the lua script. Added it to the "this" table
      lua_rawgeti (state, LUA_REGISTRYINDEX, m_iThisRef);

      // Push the function and parameters
      lua_pushstring (state, strFuncName);
      lua_pushnumber (state, (lua_Number) iMethodIdx);
      lua_pushcclosure (state, LuaCallback, 1);
      lua_settable (state, -3);

   END_LUA_CHECK

   return iMethodIdx;
}

int moScript::RegisterBaseFunction (const char *strFuncName) {

    int MethodBase = m_iMethodBase;

    m_iMethodBase = RegisterFunction( strFuncName );

    if (MethodBase!=m_iMethodBase && m_iMethodBase!=-1) {
        ///Function was correctly registered so beginning new MethodBase for this derivation
        m_MethodBases[m_iMethodBaseAncestors] = m_iMethodBase;
        m_iMethodBaseAncestors++;
    }
}


//============================================================================
// bool moScript::SelectScriptFunction
//---------------------------------------------------------------------------
// Selects a script function to run
//
// Parameter   Dir      Description
// ---------   ---      -----------
// strFuncName IN       Function name
//
// Return
// ------
// Success
//
//============================================================================
bool moScript::SelectScriptFunction (const char *strFuncName)
{
   assert (strFuncName != NULL && "moScript::SelectScriptFunction -> strFuncName == NULL");
   assert (m_vm.Ok () && "VM Not OK");

   bool fSuccess = true;

   BEGIN_LUA_CHECK (m_vm)
      // Look up function name
      lua_rawgeti (state, LUA_REGISTRYINDEX, m_iThisRef);
      lua_pushstring (state, strFuncName);
      lua_rawget (state, -2);
      lua_remove (state, -2);

      // Put the "this" table back
      lua_rawgeti (state, LUA_REGISTRYINDEX, m_iThisRef);

      // Check that we have a valid function
      if (!lua_isfunction (state, -2))
      {
         fSuccess = false;
         lua_pop (state, 2);
      }
      else
      {
         m_nArgs = 0;
         m_strFunctionName = strFuncName;
      }
   END_LUA_CHECK

   return fSuccess;
}

//============================================================================
// bool moScript::ScriptHasFunction
//---------------------------------------------------------------------------
// Checks to see if a function exists
//
// Parameter      Dir      Description
// ---------      ---      -----------
// strScriptName  IN       Function name
//
// Return
// ------
// Success
//
//============================================================================
bool moScript::ScriptHasFunction (const char *strScriptName)
{
   //assert (strScriptName != NULL && "moScript::ScriptHasFunction -> strScriptName == NULL");
   //assert (m_vm.Ok () && "VM Not OK");
   if ( !m_vm.Ok () ) return false;

   moLuaRestoreStack rs (m_vm);

   bool fFoundFunc = false;

   BEGIN_LUA_CHECK (m_vm)
      lua_rawgeti (state, LUA_REGISTRYINDEX, m_iThisRef);
      lua_pushstring (state, strScriptName);
      lua_rawget (state, -2);
      lua_remove (state, -2);

      if (lua_isfunction (state, -1))
      {
         fFoundFunc = true;
      }
   END_LUA_CHECK

   return fFoundFunc;
}

//============================================================================
// void moScript::AddFunctionParam
//---------------------------------------------------------------------------
// Adds a parameter to the parameter list
//
// Parameter   Dir      Description
// ---------   ---      -----------
// string      IN       string param
//
// Return
// ------
// None.
//
//============================================================================
void moScript::AddFunctionParam (char *string)
{
   assert (string != NULL && "moScript::AddFunctionParam -> string == NULL");
   assert (m_vm.Ok () && "VM Not OK");

   BEGIN_LUA_CHECK (m_vm)
      lua_pushstring (state, string);
      ++m_nArgs;
   END_LUA_CHECK
}

//============================================================================
// void moScript::AddFunctionParam
//---------------------------------------------------------------------------
// Adds a parameter to the parameter list
//
// Parameter   Dir      Description
// ---------   ---      -----------
// iInt        IN       int param
//
// Return
// ------
// None.
//
//============================================================================
void moScript::AddFunctionParam (int iInt)
{
   assert (m_vm.Ok () && "VM Not OK");

   BEGIN_LUA_CHECK (m_vm)
      lua_pushnumber (state, (lua_Number) iInt);
      ++m_nArgs;
   END_LUA_CHECK
}

//============================================================================
// void moScript::AddFunctionParam
//---------------------------------------------------------------------------
// Adds a parameter to the parameter list
//
// Parameter   Dir      Description
// ---------   ---      -----------
// fFloat      IN       float param
//
// Return
// ------
// None.
//
//============================================================================
void moScript::AddFunctionParam (float fFloat)
{
   assert (m_vm.Ok () && "VM Not OK");

   BEGIN_LUA_CHECK (m_vm)
      lua_pushnumber (state, (lua_Number) fFloat);
      ++m_nArgs;
   END_LUA_CHECK
}

//============================================================================
// bool moScript::Go
//---------------------------------------------------------------------------
// Runs the selected script function
//
// Parameter   Dir      Description
// ---------   ---      -----------
// nReturns    IN       Number of expected returns
//
// Return
// ------
// None.
//
//============================================================================
bool moScript::RunSelectedFunction (int nReturns /* = 0 */)
{
   assert (m_vm.Ok () && "VM Not OK");

   // At this point there should be a parameters and a function on the
   // Lua stack. Each function get a "this" parameter as default and is
   // pushed onto the stack when the method is selected

   bool fSuccess = m_vm.CallFunction (m_nArgs + 1, nReturns);

   if (fSuccess == true && nReturns > 0)
   {
      // Check for returns
      HandleReturns (m_vm, m_strFunctionName);
      lua_pop ((lua_State *) m_vm, nReturns);
   }

   return fSuccess;
}

