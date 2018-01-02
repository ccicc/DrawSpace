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

#include "mainservice.h"
#include "component.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Aspect;
using namespace DrawSpace::AspectImplementations;
using namespace DrawSpace::Utils;

_DECLARE_DS_LOGGER( logger, "star_map_mainservice", NULL )

MainService::MainService( void ) :
m_left_mousebutton( false ),
m_right_mousebutton( false )
{
}

bool MainService::Init( void )
{
    m_systems.push_back( &m_timeSystem );
    m_systems.push_back( &m_transformSystem );
    m_systems.push_back( &m_renderingSystem );

    //////////////recup params du service //////////////////

    ComponentList<DrawSpace::Logger::Configuration*> logconfs;
    m_owner->GetComponentsByType<DrawSpace::Logger::Configuration*>( logconfs );

    ComponentList<DrawSpace::Core::BaseCallback<void, bool>*> mouse_cbs;
    m_owner->GetComponentsByType<DrawSpace::Core::BaseCallback<void, bool>*>( mouse_cbs );

    ComponentList<DrawSpace::Core::BaseCallback<void, int>*> app_cbs;
    m_owner->GetComponentsByType<DrawSpace::Core::BaseCallback<void, int>*>( app_cbs );

    DrawSpace::Core::BaseCallback<void, bool>* mousecircularmode_cb;
    DrawSpace::Core::BaseCallback<void, bool>* mousevisible_cb;

    mousecircularmode_cb = mouse_cbs[0]->getPurpose();
    mousevisible_cb = mouse_cbs[1]->getPurpose();

    DrawSpace::Logger::Configuration* logconf = logconfs[0]->getPurpose();

    DrawSpace::Core::BaseCallback<void, int>* closeapp_cb;
    closeapp_cb = app_cbs[0]->getPurpose();


    m_mousecircularmode_cb = mousecircularmode_cb;
    m_closeapp_cb = closeapp_cb;

    ////////////////////////////////////////////////////////

        
    logconf->RegisterSink( &logger );
    logger.SetConfiguration( logconf );

    logconf->RegisterSink( MemAlloc::GetLogSink() );
    MemAlloc::GetLogSink()->SetConfiguration( logconf );

    /////////////////////////////////////////////////////////////////////////////////

    for( size_t i = 0; i < m_systems.size(); i++ )
    {
        m_systems[i]->Init( &m_entitygraph );
    }

    /////////////////////////////////////////////////////////////////////////////////

    m_renderer = DrawSpace::Core::SingletonPlugin<DrawSpace::Interface::Renderer>::GetInstance()->m_interface;

    m_renderer->GetDescr( m_pluginDescr );

    m_renderer->SetRenderState( &DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETCULLING, "cw" ) );
    m_renderer->SetRenderState( &DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETTEXTUREFILTERTYPE, "linear" ) );
    m_renderer->SetRenderState( &DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ENABLEZBUFFER, "false" ) );

    /////////////////////////////////////////////////////////////////////////////////

    m_finalpass = m_rendergraph.CreateRoot( "final_pass" );

    m_finalpass.CreateViewportQuad();

    m_finalpass.GetViewportQuad()->SetFx( _DRAWSPACE_NEW_( Fx, Fx ) );

    RenderStatesSet finalpass_rss;
    finalpass_rss.AddRenderStateIn( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETTEXTUREFILTERTYPE, "point" ) );
    finalpass_rss.AddRenderStateOut( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETTEXTUREFILTERTYPE, "linear" ) );


    m_finalpass.GetViewportQuad()->GetFx()->SetRenderStates( finalpass_rss );


    m_finalpass.GetViewportQuad()->GetFx()->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture.vso", true ) ) );
    m_finalpass.GetViewportQuad()->GetFx()->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture.pso", true ) ) );

    m_finalpass.GetViewportQuad()->GetFx()->GetShader( 0 )->LoadFromFile();
    m_finalpass.GetViewportQuad()->GetFx()->GetShader( 1 )->LoadFromFile();

    m_finalpass.GetRenderingQueue()->EnableDepthClearing( false );
    m_finalpass.GetRenderingQueue()->EnableTargetClearing( false );


    m_texturepass = m_finalpass.CreateChild( "texture_pass", 0 );    
    m_texturepass.GetRenderingQueue()->EnableDepthClearing( false );
    m_texturepass.GetRenderingQueue()->EnableTargetClearing( true );
    m_texturepass.GetRenderingQueue()->SetTargetClearingColor( 90, 90, 90, 255 );

    ////////////////////////////////////////////////////////////////////////////////////

    RenderingAspect* rendering_aspect = m_rootEntity.AddAspect<RenderingAspect>();

    rendering_aspect->AddImplementation( &m_passesRender );
    m_passesRender.SetRendergraph( &m_rendergraph );

    rendering_aspect->AddImplementation( &m_textRender );
    rendering_aspect->AddComponent<TextRenderingAspectImpl::TextDisplay>( "fps", 10, 20, 255, 255, 255, "..." );

    TimeAspect* time_aspect = m_rootEntity.AddAspect<TimeAspect>();

    time_aspect->AddComponent<TimeManager>( "time_manager" );
    time_aspect->AddComponent<TimeAspect::TimeScale>( "time_scale", TimeAspect::NORMAL_TIME );
    time_aspect->AddComponent<dsstring>( "output_formated_datetime", "..." );
    time_aspect->AddComponent<dstime>( "time", 0 );
    time_aspect->AddComponent<int>( "output_fps" );
    time_aspect->AddComponent<int>( "output_world_nbsteps" );

    time_aspect->AddComponent<dsreal>( "output_time_factor" );

    m_rootEntityNode = m_entitygraph.SetRoot( &m_rootEntity );

    /////////////////////////////////////////////////////////////////////////////////////



    create_skybox();
    create_camera();


    // ajouter la skybox a la scene
    m_skyboxEntityNode = m_rootEntityNode.AddChild( &m_skyboxEntity );
    m_skyboxRender->RegisterToRendering( m_rendergraph );

    m_camera2EntityNode = m_rootEntityNode.AddChild( &m_camera2Entity );
    m_transformSystem.SetCurrentCameraEntity( &m_camera2Entity );

    m_rendergraph.RenderingQueueModSignal();
    m_entitygraph.OnSceneRenderBegin();



    set_mouse_circular_mode( true );

    _DSDEBUG( logger, dsstring("MainService : startup...") );

    return true;
}


void MainService::Run( void )
{
    for( size_t i = 0; i < m_systems.size(); i++ )
    {
        m_systems[i]->Run( &m_entitygraph );
    }
    
    m_renderer->FlipScreen();

    TimeAspect* time_aspect = m_rootEntity.GetAspect<TimeAspect>();
    char comment[256];

    sprintf( comment, "%d fps - %s", time_aspect->GetComponent<int>( "output_fps" )->getPurpose(), m_pluginDescr.c_str() );

    RenderingAspect* rendering_aspect = m_rootEntity.GetAspect<RenderingAspect>();
    rendering_aspect->GetComponent<TextRenderingAspectImpl::TextDisplay>( "fps" )->getPurpose().m_text = comment;


    TransformAspect* transform_aspect = m_camera2Entity.GetAspect<TransformAspect>();
    transform_aspect->GetComponent<dsreal>( "rspeed_x" )->getPurpose() = 0.0;
    transform_aspect->GetComponent<dsreal>( "rspeed_y" )->getPurpose() = 0.0;
    transform_aspect->GetComponent<dsreal>( "rspeed_z" )->getPurpose() = 0.0;

}

void MainService::Release( void )
{
    _DSDEBUG( logger, dsstring("MainService : shutdown...") );

    m_entitygraph.OnSceneRenderEnd();
    for( size_t i = 0; i < m_systems.size(); i++ )
    {
        m_systems[i]->Release( &m_entitygraph );
    }
}

void MainService::create_camera( void )
{
    TransformAspect* transform_aspect = m_camera2Entity.AddAspect<TransformAspect>();

    transform_aspect->SetImplementation( &m_free_transformer );
  
    transform_aspect->AddComponent<dsreal>( "rspeed_x", 0.0 );
    transform_aspect->AddComponent<dsreal>( "rspeed_y", 0.0 );
    transform_aspect->AddComponent<dsreal>( "rspeed_z", 0.0 );

    transform_aspect->AddComponent<Vector>( "speed" );
    transform_aspect->AddComponent<Vector>( "rot_axis_x", Vector( 1.0, 0.0, 0.0, 1.0 ) );
    transform_aspect->AddComponent<Vector>( "rot_axis_y", Vector( 0.0, 1.0, 0.0, 1.0 ) );
    transform_aspect->AddComponent<Vector>( "rot_axis_z", Vector( 0.0, 0.0, 1.0, 1.0 ) );

    transform_aspect->AddComponent<Matrix>( "pos" );
    transform_aspect->GetComponent<Matrix>( "pos" )->getPurpose().Translation( Vector( 0.0, 1.0, 2.0, 1.0 ) );

    transform_aspect->AddComponent<Quaternion>( "quat" );
    transform_aspect->GetComponent<Quaternion>( "quat" )->getPurpose().Identity();
    

    DrawSpace::Interface::Renderer::Characteristics characteristics;
    m_renderer->GetRenderCharacteristics( characteristics );


    CameraAspect* camera_aspect = m_camera2Entity.AddAspect<CameraAspect>();

    camera_aspect->AddComponent<Matrix>( "camera_proj" );

    m_renderer->GetRenderCharacteristics( characteristics );
    camera_aspect->GetComponent<Matrix>( "camera_proj" )->getPurpose().Perspective( characteristics.width_viewport, characteristics.height_viewport, 1.0, 100000.0 );

    camera_aspect->AddComponent<dsstring>( "camera_debug_name", "camera2 (free)" );
}

void MainService::create_skybox( void )
{    
    DrawSpace::Interface::Module::Root* sbmod_root;

    if( !DrawSpace::Utils::PILoad::LoadModule( "skyboxmod", "skybox", &sbmod_root ) )
    {
        _DSEXCEPTION( "fail to load skyboxmod module root" )
    }
    m_skyboxRender = sbmod_root->InstanciateRenderingAspectImpls( "skyboxRender" );


    RenderingAspect* rendering_aspect = m_skyboxEntity.AddAspect<RenderingAspect>();
    
    rendering_aspect->AddImplementation( m_skyboxRender );

    ////////////// noms des passes

    std::vector<dsstring> skybox_passes;

    skybox_passes.push_back( "texture_pass" );
    skybox_passes.push_back( "texturemirror_pass" );

    rendering_aspect->AddComponent<std::vector<dsstring>>( "skybox_passes", skybox_passes );

    ////////////// jeu de 6 textures par slot pass

    std::vector<Texture*> skybox_textures;
    skybox_textures.push_back( _DRAWSPACE_NEW_( Texture, Texture( "neb_front5.png" ) ) );
    skybox_textures.push_back( _DRAWSPACE_NEW_( Texture, Texture( "neb_back6.png" ) ) );
    skybox_textures.push_back( _DRAWSPACE_NEW_( Texture, Texture( "neb_left2.png" ) ) );
    skybox_textures.push_back( _DRAWSPACE_NEW_( Texture, Texture( "neb_right1.png" ) ) );
    skybox_textures.push_back( _DRAWSPACE_NEW_( Texture, Texture( "neb_top3.png" ) ) );
    skybox_textures.push_back( _DRAWSPACE_NEW_( Texture, Texture( "neb_bottom4.png" ) ) );

    for( int i = 0; i < 6; i++ )
    {
        skybox_textures[i]->LoadFromFile();    
    }

    rendering_aspect->AddComponent<std::vector<Texture*>>( "skybox_textures", skybox_textures );
    rendering_aspect->AddComponent<std::vector<Texture*>>( "skybox_mirror_textures", skybox_textures );

    /////////////// les FX pour chaque slot pass

    Fx* skybox_texturepass_fx = _DRAWSPACE_NEW_( Fx, Fx );

    skybox_texturepass_fx->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture.vso", true ) ) );
    skybox_texturepass_fx->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture.pso", true ) ) );

    skybox_texturepass_fx->GetShader( 0 )->LoadFromFile();
    skybox_texturepass_fx->GetShader( 1 )->LoadFromFile();

    RenderStatesSet skybox_texturepass_rss;
    skybox_texturepass_rss.AddRenderStateIn( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ENABLEZBUFFER, "false" ) );
    skybox_texturepass_rss.AddRenderStateOut( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ENABLEZBUFFER, "false" ) );

    skybox_texturepass_fx->SetRenderStates( skybox_texturepass_rss );

    rendering_aspect->AddComponent<Fx*>( "skybox_fx", skybox_texturepass_fx );

    
    Fx* skybox_texturemirrorpass_fx = _DRAWSPACE_NEW_( Fx, Fx );

    skybox_texturemirrorpass_fx->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture_mirror.vso", true ) ) );
    skybox_texturemirrorpass_fx->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture_mirror.pso", true ) ) );

    skybox_texturemirrorpass_fx->GetShader( 0 )->LoadFromFile();
    skybox_texturemirrorpass_fx->GetShader( 1 )->LoadFromFile();

    RenderStatesSet skybox_texturemirrorpass_rss;
    skybox_texturemirrorpass_rss.AddRenderStateIn( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ENABLEZBUFFER, "false" ) );
    skybox_texturemirrorpass_rss.AddRenderStateIn( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETCULLING, "ccw" ) );
    skybox_texturemirrorpass_rss.AddRenderStateOut( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ENABLEZBUFFER, "false" ) );
    skybox_texturemirrorpass_rss.AddRenderStateOut( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETCULLING, "cw" ) );

    skybox_texturemirrorpass_fx->SetRenderStates( skybox_texturemirrorpass_rss );
    
    rendering_aspect->AddComponent<Fx*>( "skybox_mirror_fx", skybox_texturemirrorpass_fx );
    

    /////////// params shaders
    
    //std::vector<std::vector<std::pair<dsstring, RenderingNode::ShadersParams>>> skybox_shaders_params;

    std::vector<std::pair<dsstring, RenderingNode::ShadersParams>> skybox_texturepass_shaders_params;

    
    std::vector<std::pair<dsstring, RenderingNode::ShadersParams>> skybox_texturemirrorpass_shaders_params;

    std::pair<dsstring, RenderingNode::ShadersParams> reflector_pos;
    reflector_pos.first = "reflector_pos";
    reflector_pos.second.shader_index = 0;
    reflector_pos.second.param_register = 24;
    reflector_pos.second.vector = true;
    reflector_pos.second.param_values = Vector( 0.0, -4.0, 0.0, 1.0 );


    std::pair<dsstring, RenderingNode::ShadersParams> reflector_normale;
    reflector_normale.first = "reflector_normale";
    reflector_normale.second.shader_index = 0;
    reflector_normale.second.param_register = 25;
    reflector_normale.second.vector = true;
    reflector_normale.second.param_values = Vector( 0.0, 1.0, 0.0, 1.0 );
    

    skybox_texturemirrorpass_shaders_params.push_back( reflector_pos );
    skybox_texturemirrorpass_shaders_params.push_back( reflector_normale );
    

 
    rendering_aspect->AddComponent<std::vector<std::pair<dsstring, RenderingNode::ShadersParams>>>( "skybox_texturepass_shaders_params", skybox_texturepass_shaders_params );
    rendering_aspect->AddComponent<std::vector<std::pair<dsstring, RenderingNode::ShadersParams>>>( "skybox_texturemirrorpass_shaders_params", skybox_texturemirrorpass_shaders_params );

    //////////////// valeur du rendering order pour chaque slot pass
    rendering_aspect->AddComponent<int>( "skybox_ro", -1000 );
    rendering_aspect->AddComponent<int>( "skybox_mirror_ro", -1000 );


    TransformAspect* transform_aspect = m_skyboxEntity.AddAspect<TransformAspect>();

    transform_aspect->SetImplementation( &m_skybox_transformer );

    transform_aspect->AddComponent<Matrix>( "skybox_scaling" );

    transform_aspect->GetComponent<Matrix>( "skybox_scaling" )->getPurpose().Scale( 100.0, 100.0, 100.0 );
    
}

void MainService::OnKeyPress( long p_key )
{
}

void MainService::OnEndKeyPress( long p_key )
{
}

void MainService::OnKeyPulse( long p_key )
{
}

void MainService::OnChar( long p_char, long p_scan )
{
}

void MainService::OnMouseMove( long p_xm, long p_ym, long p_dx, long p_dy )
{
    TransformAspect* transform_aspect = m_camera2Entity.GetAspect<TransformAspect>();

    if( m_left_mousebutton )
    {
        transform_aspect->GetComponent<dsreal>( "rspeed_x" )->getPurpose() = - p_dy / 4.0;
        transform_aspect->GetComponent<dsreal>( "rspeed_y" )->getPurpose() = - p_dx / 4.0;
    }
    else if( m_right_mousebutton )
    {
        transform_aspect->GetComponent<dsreal>( "rspeed_z" )->getPurpose() = - p_dx;
    }
}

void MainService::OnMouseWheel( long p_delta )
{
}

void MainService::OnMouseLeftButtonDown( long p_xm, long p_ym )
{
    m_left_mousebutton = true;
}

void MainService::OnMouseLeftButtonUp( long p_xm, long p_ym )
{
    m_left_mousebutton = false;
}

void MainService::OnMouseRightButtonDown( long p_xm, long p_ym )
{
    m_right_mousebutton = true;
}

void MainService::OnMouseRightButtonUp( long p_xm, long p_ym )
{
    m_right_mousebutton = false;
}

void MainService::OnAppEvent( WPARAM p_wParam, LPARAM p_lParam )
{
}




void MainService::set_mouse_circular_mode( bool p_state )
{
    if( m_mousecircularmode_cb )
    {
        (*m_mousecircularmode_cb)( p_state );        
    }
}
