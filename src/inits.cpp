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

#include "inits.h"
#include "factory.h"
#include "texture.h"
#include "shader.h"
#include "meshe.h"
#include "font.h"
#include "fx.h"
#include "pass.h"
#include "spacebox.h"
#include "chunk.h"
#include "inertbody.h"
#include "circularmovement.h"
#include "fpsmovement.h"
#include "freemovement.h"
#include "headmovement.h"
#include "linearmovement.h"
#include "longlatmovement.h"
#include "spectatormovement.h"
#include "renderstate.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Dynamics;

// generic and statics initialisations done here
void DrawSpace::Initialize( void )
{
    Factory::GetInstance()->RegisterAssetInstanciationFuncByText( TEXTURE_TEXT_KEYWORD, Texture::Instanciate );
    Factory::GetInstance()->RegisterAssetInstanciationFuncByArc( TEXTURE_ARC_MAGICNUMBER, Texture::Instanciate );

    Factory::GetInstance()->RegisterAssetInstanciationFuncByText( SHADER_TEXT_KEYWORD, Shader::Instanciate );
    Factory::GetInstance()->RegisterAssetInstanciationFuncByArc( SHADER_ARC_MAGICNUMBER, Shader::Instanciate );

    Factory::GetInstance()->RegisterAssetInstanciationFuncByText( MESHE_TEXT_KEYWORD, Meshe::Instanciate );
    Factory::GetInstance()->RegisterAssetInstanciationFuncByArc( MESHE_ARC_MAGICNUMBER, Meshe::Instanciate );

    Factory::GetInstance()->RegisterAssetInstanciationFuncByText( FONT_TEXT_KEYWORD, Font::Instanciate );
    Factory::GetInstance()->RegisterAssetInstanciationFuncByArc( FONT_ARC_MAGICNUMBER, Font::Instanciate );

    Factory::GetInstance()->RegisterConfigInstanciationFuncByText( FX_TEXT_KEYWORD, Fx::Instanciate );
    Factory::GetInstance()->RegisterConfigInstanciationFuncByArc( FX_ARC_MAGICNUMBER, Fx::Instanciate );

    Factory::GetInstance()->RegisterConfigInstanciationFuncByText( INTERMEDIATEPASS_TEXT_KEYWORD, IntermediatePass::Instanciate );
    Factory::GetInstance()->RegisterConfigInstanciationFuncByArc( INTERMEDIATEPASS_ARC_MAGICNUMBER, IntermediatePass::Instanciate );

    Factory::GetInstance()->RegisterConfigInstanciationFuncByText( FINALPASS_TEXT_KEYWORD, FinalPass::Instanciate );
    Factory::GetInstance()->RegisterConfigInstanciationFuncByArc( FINALPASS_ARC_MAGICNUMBER, FinalPass::Instanciate );

    Factory::GetInstance()->RegisterConfigInstanciationFuncByText( SPACEBOX_TEXT_KEYWORD, Spacebox::Instanciate );
    Factory::GetInstance()->RegisterConfigInstanciationFuncByArc( SPACEBOX_ARC_MAGICNUMBER, Spacebox::Instanciate );

    Factory::GetInstance()->RegisterConfigInstanciationFuncByText( CHUNK_TEXT_KEYWORD, Chunk::Instanciate );
    Factory::GetInstance()->RegisterConfigInstanciationFuncByArc( CHUNK_ARC_MAGICNUMBER, Chunk::Instanciate );

    Factory::GetInstance()->RegisterConfigInstanciationFuncByText( INERTBODY_TEXT_KEYWORD, InertBody::Instanciate );
    Factory::GetInstance()->RegisterConfigInstanciationFuncByArc( INERTBODY_ARC_MAGICNUMBER, InertBody::Instanciate );


    Factory::GetInstance()->RegisterConfigInstanciationFuncByText( CIRCULARMVT_TEXT_KEYWORD, CircularMovement::Instanciate );
    Factory::GetInstance()->RegisterConfigInstanciationFuncByArc( CIRCULARMVT_ARC_MAGICNUMBER, CircularMovement::Instanciate );

    Factory::GetInstance()->RegisterConfigInstanciationFuncByText( FPSMVT_TEXT_KEYWORD, FPSMovement::Instanciate );
    Factory::GetInstance()->RegisterConfigInstanciationFuncByArc( FPSMVT_ARC_MAGICNUMBER, FPSMovement::Instanciate );

    Factory::GetInstance()->RegisterConfigInstanciationFuncByText( FREEMVT_TEXT_KEYWORD, FreeMovement::Instanciate );
    Factory::GetInstance()->RegisterConfigInstanciationFuncByArc( FREEMVT_ARC_MAGICNUMBER, FreeMovement::Instanciate );

    Factory::GetInstance()->RegisterConfigInstanciationFuncByText( HEADMVT_TEXT_KEYWORD, HeadMovement::Instanciate );
    Factory::GetInstance()->RegisterConfigInstanciationFuncByArc( HEADMVT_ARC_MAGICNUMBER, HeadMovement::Instanciate );

    Factory::GetInstance()->RegisterConfigInstanciationFuncByText( LINEARMVT_TEXT_KEYWORD, LinearMovement::Instanciate );
    Factory::GetInstance()->RegisterConfigInstanciationFuncByArc( LINEARMVT_ARC_MAGICNUMBER, LinearMovement::Instanciate );

    Factory::GetInstance()->RegisterConfigInstanciationFuncByText( LONGLATMVT_TEXT_KEYWORD, LongLatMovement::Instanciate );
    Factory::GetInstance()->RegisterConfigInstanciationFuncByArc( LONGLATMVT_ARC_MAGICNUMBER, LongLatMovement::Instanciate );

    Factory::GetInstance()->RegisterConfigInstanciationFuncByText( SPECTATORMVT_TEXT_KEYWORD, SpectatorMovement::Instanciate );
    Factory::GetInstance()->RegisterConfigInstanciationFuncByArc( SPECTATORMVT_ARC_MAGICNUMBER, SpectatorMovement::Instanciate );

    RenderState::InitStringMaps();
}