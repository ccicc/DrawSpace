/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2016                              
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

#ifndef _SPHERELOD_LANDSCAPE_H_
#define _SPHERELOD_LANDSCAPE_H_

#include "drawspace_commons.h"
#include "shader.h"

namespace DrawSpace
{
namespace SphericalLOD
{
class Landscape
{
protected:

    std::vector<DrawSpace::Core::Shader*>   m_collisionsShaders;
    std::vector<DrawSpace::Core::Shader*>   m_colorsTextureShaders;

public:

    virtual void InitialiseResources( void ) = 0;

    virtual void BindShadersParams( void ) = 0;
    virtual void BindTextures( void ) = 0;
    virtual void UnbindTextures( void ) = 0;

    virtual void AddCollisionsShaders( DrawSpace::Core::Shader* p_shader );
    virtual void AddColorsTextureShaders( DrawSpace::Core::Shader* p_shader );

    virtual DrawSpace::Core::Shader* GetCollisionsShader( int p_index );
    virtual DrawSpace::Core::Shader* GetColorsTextureShader( int p_index );

    virtual int GetCollisionsShadersListSize( void );
    virtual int GetColorsTextureShadersListSize( void );

};
}
}

#endif