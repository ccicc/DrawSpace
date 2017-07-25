/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2017                        
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

#include "mainloopservice.h"
#include "drawspace.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Utils;
using namespace DrawSpace::Interface::Module;

_DECLARE_DS_LOGGER( logger, "test01mainloopservice", NULL )

MainLoopService::MainLoopService( void )
{
}

MainLoopService::~MainLoopService( void )
{
}

void MainLoopService::GetKeys( std::vector<DrawSpace::Module::KeySinkBase*>& p_keys )
{

}

void MainLoopService::Init( DrawSpace::Logger::Configuration* p_logconf, 
                            DrawSpace::Core::BaseCallback<void, bool>* p_mousecircularmode_cb, 
                            DrawSpace::Core::BaseCallback<void, bool>* p_mousevisible_cb, 
                            DrawSpace::Core::BaseCallback<void, int>* p_closeapp_cb )
{
    p_logconf->RegisterSink( &logger );
    logger.SetConfiguration( p_logconf );

    p_logconf->RegisterSink( MemAlloc::GetLogSink() );
    MemAlloc::GetLogSink()->SetConfiguration( p_logconf );

    /////////////////////////////////////////////////////////////////////////////////

    m_renderer = DrawSpace::Core::SingletonPlugin<DrawSpace::Interface::Renderer>::GetInstance()->m_interface;

    m_renderer->GetDescr( m_pluginDescr );

    m_renderer->SetRenderState( &DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETCULLING, "cw" ) );
    m_renderer->SetRenderState( &DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETTEXTUREFILTERTYPE, "linear" ) );
    m_renderer->SetRenderState( &DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ENABLEZBUFFER, "false" ) );


    /*
    /////////////////////////////////////////////////////////////////////////////////
    // BUILD CUBE2 CHUNK

    m_Component_cube2_meshe = _DRAWSPACE_NEW_( DrawSpace::MesheComponent, DrawSpace::MesheComponent );
    m_Component_cube2_meshe->m_filepath = "object.ac";
    m_Component_cube2_meshe->m_index_in_file = 0;



    m_Entity_cube2 += m_Component_cube2_meshe;
    */




    /////////////////////////////////////////////////////////////////////////////////
    // BUILD FINALPASS

    m_Component_color = _DRAWSPACE_NEW_( DrawSpace::ColorArgComponent, DrawSpace::ColorArgComponent );
    m_Component_color->m_r = 5;
    m_Component_color->m_g = 5;
    m_Component_color->m_b = 250;

    
    m_Component_rendering_queue = _DRAWSPACE_NEW_( DrawSpace::RenderingQueueComponent, DrawSpace::RenderingQueueComponent );
    m_Component_rendering_queue->m_name = "final_pass";


    m_Component_fps_text = _DRAWSPACE_NEW_( DrawSpace::TextComponent, DrawSpace::TextComponent );
    m_Component_fps_text->m_r = 255;
    m_Component_fps_text->m_g = 255;
    m_Component_fps_text->m_b = 250;
    m_Component_fps_text->m_x = 10;
    m_Component_fps_text->m_y = 10;
    m_Component_fps_text->m_text = "";



    m_Component_viewport_quad = _DRAWSPACE_NEW_( DrawSpace::ViewportQuadComponent, DrawSpace::ViewportQuadComponent );
    m_Component_viewport_quad->m_zoffset = -0.3;
    m_Component_viewport_quad->m_dims_from_renderer = true;




    m_Entity_finalpass += m_Component_color;
    m_Entity_finalpass += m_Component_rendering_queue;
    m_Entity_finalpass += m_Component_fps_text;
    m_Entity_finalpass += m_Component_viewport_quad;



    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///// BUILD TEXTUREPASS

    m_Component_texturepass_rendering_queue = _DRAWSPACE_NEW_( DrawSpace::RenderingQueueComponent, DrawSpace::RenderingQueueComponent );
    m_Component_texturepass_rendering_queue->m_name = "texture_pass";

    m_Component_texturepass_render_target = _DRAWSPACE_NEW_( DrawSpace::RenderTargetComponent, DrawSpace::RenderTargetComponent( "texturepass_target" ) );
    m_Component_texturepass_render_target->m_destination = std::pair<ViewportQuadComponent*,int>( m_Component_viewport_quad, 0 );

    m_Component_texturepass_color = _DRAWSPACE_NEW_( DrawSpace::ColorArgComponent, DrawSpace::ColorArgComponent );
    m_Component_texturepass_color->m_r = 5;
    m_Component_texturepass_color->m_g = 255;
    m_Component_texturepass_color->m_b = 0;


    m_Entity_texturepass += m_Component_texturepass_color;
    m_Entity_texturepass += m_Component_texturepass_rendering_queue;
    m_Entity_texturepass += m_Component_texturepass_render_target;

    

    /////////////////////////////////////////////////////////////////////////////////////////////////////////



    m_Data_Rendergraph.AddRoot( &m_Entity_finalpass );
    m_Data_Rendergraph.AddLeaf( &m_Entity_texturepass, 0 );



    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    
    m_Component_viewport_quad->m_viewportquad->SetFx( _DRAWSPACE_NEW_( Fx, Fx ) );


    RenderStatesSet finalpass_rss;


    finalpass_rss.AddRenderStateIn( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETTEXTUREFILTERTYPE, "point" ) );
    finalpass_rss.AddRenderStateOut( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETTEXTUREFILTERTYPE, "linear" ) );
    //finalpass_rss.AddRenderStateIn( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETFILLMODE, "line" ) );
    //finalpass_rss.AddRenderStateOut( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETFILLMODE, "solid" ) );

    m_Component_viewport_quad->m_viewportquad->GetFx()->SetRenderStates( finalpass_rss );

    m_Component_viewport_quad->m_viewportquad->GetFx()->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture.vso", true ) ) );
    m_Component_viewport_quad->m_viewportquad->GetFx()->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture.pso", true ) ) );

    m_Component_viewport_quad->m_viewportquad->GetFx()->GetShader( 0 )->LoadFromFile();
    m_Component_viewport_quad->m_viewportquad->GetFx()->GetShader( 1 )->LoadFromFile();

    //m_Component_viewport_quad->m_viewportquad->AddShaderParameter( 1, "color", 0 );
    //m_Component_viewport_quad->m_viewportquad->SetShaderRealVector( "color", Vector( 1.0, 0.5, 0.5, 1.0 ) );

    //m_Component_viewport_quad->m_viewportquad->SetTexture( m_Component_texturepass_render_target->m_targettexture, 0 );




    m_Component_rendering_queue->m_queue->UpdateOutputQueue();
    m_Component_texturepass_rendering_queue->m_queue->UpdateOutputQueue();
    

    _DSDEBUG( logger, dsstring("main loop service : startup...") );
}

void MainLoopService::Run( void )
{   
    m_Component_fps_text->m_text = dsstring( "fps :" ) << m_tm.GetFPS() << dsstring( " - " ) <<  m_pluginDescr.c_str();

    m_Data_Rendergraph.AcceptSystemLeafsToTopRecursive( &m_System_rendergraph );

    m_renderer->FlipScreen();
            
    m_tm.Update();
    if( m_tm.IsReady() )
    {
    }
}

void MainLoopService::Release( void )
{
    _DSDEBUG( logger, dsstring("main loop service : shutdown...") );

}

DrawSpace::Core::BaseSceneNode* MainLoopService::InstanciateSceneNode( const dsstring& p_sceneNodeName, DrawSpace::Dynamics::Calendar* p_calendar, LODDependantNodeInfoStateHandler* p_handler )
{
    return NULL;
}

void MainLoopService::RegisterScenegraphCallbacks( DrawSpace::Core::SceneNodeGraph& p_scenegraph )
{
}

void MainLoopService::UnregisterScenegraphCallbacks( DrawSpace::Core::SceneNodeGraph& p_scenegraph )
{
}

void MainLoopService::ReleaseSceneNode( const dsstring& p_sceneNodeName, DrawSpace::Dynamics::Calendar* p_calendar )
{
}

void MainLoopService::OnKeyPress( long p_key )
{
}

void MainLoopService::OnEndKeyPress( long p_key )
{
}

void MainLoopService::OnKeyPulse( long p_key )
{
}

void MainLoopService::OnChar( long p_char, long p_scan )
{
}

void MainLoopService::OnMouseMove( long p_xm, long p_ym, long p_dx, long p_dy )
{
}

void MainLoopService::OnMouseWheel( long p_delta )
{
}


void MainLoopService::OnMouseLeftButtonDown( long p_xm, long p_ym )
{
}

void MainLoopService::OnMouseLeftButtonUp( long p_xm, long p_ym )
{
}

void MainLoopService::OnMouseRightButtonDown( long p_xm, long p_ym )
{
}

void MainLoopService::OnMouseRightButtonUp( long p_xm, long p_ym )
{
}

void MainLoopService::OnAppEvent( WPARAM p_wParam, LPARAM p_lParam )
{
}


