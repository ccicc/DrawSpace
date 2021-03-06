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
#include "planetsetupsubservice.h"
#include "planetviewsubservice.h"
#include "planetgroundsetupsubservice.h"
#include "planetseedssetupsubservice.h"
#include "planetfogatmosetupsubservice.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Utils;
using namespace DrawSpace::Interface::Module;

_DECLARE_DS_LOGGER( logger, "worldinspectormainloopservice", NULL )

MainLoopService::MainLoopService( void ) :
m_skybox_texturesbankpath( "skybox.TexturesBankPath" ),
m_skybox_texturesbankvirtualfspath( "skybox.TexturesBankVirtualFSPath" ),
m_skybox_texturesnames( "skybox.TexturesNames" )
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

    m_keysLinkTable.RegisterClientKey( &m_skybox_texturesbankpath );
    m_keysLinkTable.RegisterClientKey( &m_skybox_texturesbankvirtualfspath );
    m_keysLinkTable.RegisterClientKey( &m_skybox_texturesnames );

    p_logconf->RegisterSink( &logger );
    logger.SetConfiguration( p_logconf );

    p_logconf->RegisterSink( MemAlloc::GetLogSink() );
    MemAlloc::GetLogSink()->SetConfiguration( p_logconf );

    _DSDEBUG( logger, dsstring("main loop service : startup...") );

   
    // hide OS mouse cursor (we use CEGUI mouse cursor instead)
    (*p_mousevisible_cb)( false );

    m_closeapp_cb = p_closeapp_cb;

    /////////////////////////////////////////////////////////////////////////////////

    m_renderer = DrawSpace::Core::SingletonPlugin<DrawSpace::Interface::Renderer>::GetInstance()->m_interface;
    m_renderer->GetDescr( m_pluginDescr );
    
    m_renderer->GUI_InitSubSystem();

    m_renderer->GUI_SetResourcesRootDirectory( "./xfskin2" );
    m_renderer->GUI_LoadScheme( "xfskin.scheme" );

    m_renderer->GUI_SetMouseCursorImage( "xfskin/MouseCursor" );
    m_renderer->GUI_ShowMouseCursor( true );


    load_cdlodplanet_module( p_logconf );
    PlanetSetupSubService::GetInstance()->SetCDLODInfos( m_cdlodp_root, m_cdlodp_service );
    PlanetViewSubService::GetInstance()->SetCDLODInfos( m_cdlodp_root, m_cdlodp_service );

   
    load_skybox_module( p_logconf );
    PlanetViewSubService::GetInstance()->SetSkyboxInfos( m_sbmod_root, m_sb_service );

    PlanetSetupSubService::GetInstance()->Init( p_logconf, p_mousecircularmode_cb, p_mousevisible_cb, p_closeapp_cb );
    PlanetViewSubService::GetInstance()->Init( p_logconf, p_mousecircularmode_cb, p_mousevisible_cb, p_closeapp_cb );
	PlanetGroundSetupSubService::GetInstance()->Init( p_logconf, p_mousecircularmode_cb, p_mousevisible_cb, p_closeapp_cb );
    PlanetSeedsSetupSubService::GetInstance()->Init( p_logconf, p_mousecircularmode_cb, p_mousevisible_cb, p_closeapp_cb );
    PlanetFogAtmoSetupSubService::GetInstance()->Init( p_logconf, p_mousecircularmode_cb, p_mousevisible_cb, p_closeapp_cb );



    PlanetSetupSubService::GetInstance()->Activate();

    m_current_subservice = PlanetSetupSubService::GetInstance();

}

void MainLoopService::Run( void )
{   
    m_current_subservice->Run();
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
    m_current_subservice->OnKeyPress( p_key );
}

void MainLoopService::OnEndKeyPress( long p_key )
{
    m_current_subservice->OnEndKeyPress( p_key );
}

void MainLoopService::OnKeyPulse( long p_key )
{
    m_current_subservice->OnKeyPulse( p_key );
}

void MainLoopService::OnChar( long p_char, long p_scan )
{
    m_current_subservice->OnChar( p_char, p_scan );
}

void MainLoopService::OnMouseMove( long p_xm, long p_ym, long p_dx, long p_dy )
{
    m_current_subservice->OnMouseMove( p_xm, p_ym, p_dx, p_dy );
}

void MainLoopService::OnMouseWheel( long p_delta )
{
    m_current_subservice->OnMouseWheel( p_delta );
}

void MainLoopService::OnMouseLeftButtonDown( long p_xm, long p_ym )
{
    m_current_subservice->OnMouseLeftButtonDown( p_xm, p_ym );
}

void MainLoopService::OnMouseLeftButtonUp( long p_xm, long p_ym )
{
    m_current_subservice->OnMouseLeftButtonUp( p_xm, p_ym );
}

void MainLoopService::OnMouseRightButtonDown( long p_xm, long p_ym )
{
    m_current_subservice->OnMouseRightButtonDown( p_xm, p_ym );
}

void MainLoopService::OnMouseRightButtonUp( long p_xm, long p_ym )
{
    m_current_subservice->OnMouseRightButtonUp( p_xm, p_ym );
}

void MainLoopService::OnAppEvent( WPARAM p_wParam, LPARAM p_lParam )
{
}

void MainLoopService::OnGUIEvent( APP_GUI_EVENT p_evt )
{
    switch( p_evt )
    {
        case GUIEVT_PLANETSETUP_QUITBUTTON_CLIC:

            // close whole app
            (*m_closeapp_cb)( 0 );
            break;

        case GUIEVT_PLANETSETUP_PLANETVIEWBUTTON_CLIC:

            PlanetSetupSubService::GetInstance()->Unactivate();
			PlanetViewSubService::GetInstance()->Activate( PlanetSetupSubService::GetInstance()->GetSelectedPlanetConfig() );

            m_current_subservice = PlanetViewSubService::GetInstance();
            break;

		case GUIEVT_PLANETSETUP_PLANETGROUNDSETUPBUTTON_CLIC:

			PlanetSetupSubService::GetInstance()->Unactivate();
			PlanetGroundSetupSubService::GetInstance()->Activate( PlanetSetupSubService::GetInstance()->GetSelectedPlanetConfig() );

			m_current_subservice = PlanetGroundSetupSubService::GetInstance();
			break;

        case GUIEVT_PLANETSETUP_PLANETSEEDSSETUPBUTTON_CLIC:

			PlanetSetupSubService::GetInstance()->Unactivate();
			PlanetSeedsSetupSubService::GetInstance()->Activate( PlanetSetupSubService::GetInstance()->GetSelectedPlanetConfig() );

			m_current_subservice = PlanetSeedsSetupSubService::GetInstance();
            break;

        case GUIEVT_PLANETSETUP_PLANETFOGATMOSETUPBUTTON_CLIC:

			PlanetSetupSubService::GetInstance()->Unactivate();
			PlanetFogAtmoSetupSubService::GetInstance()->Activate( PlanetSetupSubService::GetInstance()->GetSelectedPlanetConfig() );

			m_current_subservice = PlanetFogAtmoSetupSubService::GetInstance();

            break;

        case GUIEVT_PLANETSETUP_F1_KEY:

            PlanetViewSubService::GetInstance()->DumpMemoryAllocs();
            break;

        case GUIEVT_PLANETVIEW_CLOSEBUTTON_CLIC:

            PlanetViewSubService::GetInstance()->Unactivate();
            PlanetSetupSubService::GetInstance()->Activate();
            m_current_subservice = PlanetSetupSubService::GetInstance();

            break;

		case GUIEVT_PLANETGROUNDSETUP_CLOSEBUTTON_CLIC:

			PlanetGroundSetupSubService::GetInstance()->Unactivate();
			PlanetSetupSubService::GetInstance()->Activate();
			m_current_subservice = PlanetSetupSubService::GetInstance();

			break;

        case GUIEVT_PLANETSEEDSSETUP_CLOSEBUTTON_CLIC:

			PlanetSeedsSetupSubService::GetInstance()->Unactivate();
			PlanetSetupSubService::GetInstance()->Activate();
			m_current_subservice = PlanetSetupSubService::GetInstance();

            break;

        case GUIEVT_PLANETFOGATMOSETUP_CLOSEBUTTON_CLIC:

			PlanetFogAtmoSetupSubService::GetInstance()->Unactivate();
			PlanetSetupSubService::GetInstance()->Activate();
			m_current_subservice = PlanetSetupSubService::GetInstance();
            break;
    }
}

void MainLoopService::load_cdlodplanet_module( DrawSpace::Logger::Configuration* p_logconf )
{
    if( !DrawSpace::Utils::PILoad::LoadModule( "cdlodplanetmod", "cdlodplanet", &m_cdlodp_root ) )
    {
        _DSEXCEPTION( "fail to load cdlodplanet module root" )
    }    
    m_cdlodp_service = m_cdlodp_root->InstanciateService( "cdlodplanet" );
    connect_keys( m_cdlodp_service );

    m_cdlodp_service->Init( p_logconf, NULL, NULL, NULL );
}

void MainLoopService::load_skybox_module( DrawSpace::Logger::Configuration* p_logconf )
{
    if( !DrawSpace::Utils::PILoad::LoadModule( "skyboxmod", "skybox", &m_sbmod_root ) )
    {
        _DSEXCEPTION( "fail to load skyboxmod module root" )
    }

    m_sb_service = m_sbmod_root->InstanciateService( "skybox" );
    if( NULL == m_sb_service )
    {
        _DSEXCEPTION( "fail to load skybox module service" )
    }
    connect_keys( m_sb_service );

    //m_skybox_scenenodegraph = &m_scenenodegraph;
    m_skybox_texturesbankpath = "astrolab_data/textures_bank";
    m_skybox_texturesbankvirtualfspath = "test_data.bank";

    std::vector<dsstring> textures_names;
    textures_names.assign( 6, "" );

    textures_names[0] = "spacebox_front5.png";
    textures_names[1] = "spacebox_back6.png";
    textures_names[2] = "spacebox_left2.png";
    textures_names[3] = "spacebox_right1.png";
    textures_names[4] = "spacebox_top3.png";
    textures_names[5] = "spacebox_bottom4.png";

    m_skybox_texturesnames = textures_names;
    
    m_sb_service->Init( p_logconf, NULL, NULL, NULL );
}