
/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2014                                
*                                                                          
* This file is part of DrawSpace.                                          
*                                                                          
*    DrawSpace is free software: you can redistribute it and/or modify     
*    it under the terms of the GNU General Public License as published by  
*    the Free Software Foundation, either version 3 of the License, or     
*    (at your option) any later version.                                   
*                                                                          
*    DrawSpace is distributed in the hope that it will be useful,          
*    but WITHOUT ANY WARRANTY; without even the implied warranty of        
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
*    GNU General Public License for more details.                          
*                                                                          
*    You should have received a copy of the GNU General Public License     
*    along with DrawSpace.  If not, see <http://www.gnu.org/licenses/>.    
*                                                                          
*/

#include "bindingsinit.h"
#include "lua_assetsbase.h"
#include "lua_texture.h"
#include "lua_meshe.h"
#include "lua_shader.h"
#include "lua_renderstate.h"

using namespace DrawSpace;

void DrawSpaceLuaBindingsInit( lua_State* p_L )
{
    Luna<LuaAssetsBase>::Register( p_L );
    Luna<LuaTexture>::Register( p_L );
    Luna<LuaMeshe>::Register( p_L );
    Luna<LuaShader>::Register( p_L );
    Luna<LuaRenderState>::Register( p_L );
}