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


#include "renderframe.h"
#include "resources.h"

#include "defaultscene.h"

BEGIN_EVENT_TABLE( RenderFrame, wxFrame )
    EVT_IDLE( RenderFrame::OnIdle )
	EVT_CLOSE( RenderFrame::OnClose )
END_EVENT_TABLE()

RenderFrame::RenderFrame( const wxString& p_caption, const wxSize& p_size ) : 
wxFrame( NULL, wxID_ANY, p_caption, wxPoint( 50, 50 ), p_size, wxMINIMIZE_BOX | wxCLOSE_BOX | wxSYSTEM_MENU | wxCAPTION ),
m_gl_ready( false )
{

    m_dialog = _DRAWSPACE_NEW_( Dialog, Dialog( this, _( "Tools" ) ) );
    m_dialog->Show();

    m_console_dialog = _DRAWSPACE_NEW_( ConsoleDialog, ConsoleDialog( this, _( "User inputs" ) ) );
    m_console_dialog->Show();

    DefaultScene* default_scene = _DRAWSPACE_NEW_( DefaultScene, DefaultScene );
    m_scenes.push_back( default_scene );
}

RenderFrame::~RenderFrame( void )
{
}

void RenderFrame::OnIdle( wxIdleEvent& p_event )
{
    p_event.RequestMore( true );
	if( m_gl_ready )
    {
        on_render_frame();
    }
}

void RenderFrame::OnClose( wxCloseEvent& p_event )
{
    m_dialog->Close();
    m_console_dialog->Close();
    Destroy();
}

void RenderFrame::SetGlReady( bool p_ready )
{
    m_gl_ready = p_ready;
}

void RenderFrame::on_render_frame( void )
{
    m_scenes[0]->Draw();
}

