/***************************************************************************
*                                                                          *
* DrawSpace Rendering engine                                               *
* Emmanuel Chaumont Copyright (c) 2013-2014                                *
*                                                                          *
* This file is part of DrawSpace.                                          *
*                                                                          *
*    DrawSpace is free software: you can redistribute it and/or modify     *
*    it under the terms of the GNU General Public License as published by  *
*    the Free Software Foundation, either version 3 of the License, or     *
*    (at your option) any later version.                                   *
*                                                                          *
*    DrawSpace is distributed in the hope that it will be useful,          *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*    GNU General Public License for more details.                          *
*                                                                          *
*    You should have received a copy of the GNU General Public License     *
*    along with DrawSpace.  If not, see <http://www.gnu.org/licenses/>.    *
*                                                                          *
***************************************************************************/

#include "text.h"
#include "renderer.h"
#include "plugin.h"


using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Utils;
using namespace DrawSpace::Interface;

Text::Text( Core::Font* p_font ) : m_font( p_font ), m_height( 0.5 ), m_x( 0.0 ), m_y( 0.0 )
{
	Renderer* renderer = DrawSpace::Core::Plugin<DrawSpace::Interface::Renderer>::GetInstance()->m_interface;
	renderer->GetRenderCharacteristics( m_rc );
}

Text::~Text( void )
{

}

void Text::OnDraw( void )
{
	dsreal curr_x = m_x;
	for( unsigned long i = 0; i < m_text.size(); i++ )
	{
		unsigned char curr_ch = m_text[i];

		Font::CharMapping chmap;
		if( false == m_font->GetCharMapping( curr_ch, chmap ) )
		{
			return;
		}
		
		dsreal ratio = chmap.width / chmap.height;
		dsreal width = ratio * m_height;

		chmap.image->SetTranslation( curr_x, m_y );
		chmap.image->SetScale( m_height, m_height );

		chmap.image->OnDraw();
		curr_x += width;
	}
}

void Text::SetText( long p_x, long p_y, long p_height, const dsstring& p_text, unsigned char p_flag )
{
	m_text = p_text;
	m_height = ( (dsreal)p_height / (dsreal)DRAWSPACE_GUI_HEIGHT ) * m_rc.height_viewport;

	dsreal pos_x, pos_y;
	pos_x = ( ( ( (dsreal)p_x / (dsreal)DRAWSPACE_GUI_WIDTH ) - 0.5 ) ) * ( m_rc.width_viewport );
	pos_y = ( ( 0.5 - ( (dsreal)p_y / (dsreal)DRAWSPACE_GUI_HEIGHT ) ) ) * ( m_rc.height_viewport );

	if( p_flag & HorizontalCentering )
	{
		dsreal textwidth = 0.0;

		for( unsigned long i = 0; i < m_text.size(); i++ )
		{
			unsigned char curr_ch = m_text[i];

			Font::CharMapping chmap;
			if( false == m_font->GetCharMapping( curr_ch, chmap ) )
			{
				continue;
			}			
			dsreal ratio = chmap.width / chmap.height;
			dsreal char_width = ratio * m_height;

			textwidth += char_width;
		}

		if( m_text.size() > 0 )
		{
			Font::CharMapping chmap;

			if( m_font->GetCharMapping( m_text[0], chmap ) )
			{
				dsreal halfw = ( m_rc.width_viewport - textwidth ) / 2.0;

				if( halfw < 0.0 )
				{
					m_x = 0.0;
				}
				else
				{
					dsreal ratio = chmap.width / chmap.height;
					dsreal char_width = ratio * m_height;

					m_x = ( halfw + char_width / 2.0 ) - ( m_rc.width_viewport * 0.5 );
				}
			}
			else
			{
				m_x = 0.0;
			}
		}
		else
		{
			m_x = 0.0;
		}
	}
	else
	{
		m_x = pos_x;
	}

	if( p_flag & VerticalCentering )
	{
		m_y = 0.0;
	}
	else
	{
		m_y = pos_y;
	}


}


bool Text::LoadAssets( void )
{
	Texture* texture;
	dsstring texturepath;

	m_font->GetTexturePath( texturepath );
    texture = _DRAWSPACE_NEW_( Texture, Texture( texturepath ) );
	if( false == texture->LoadFromFile() )
	{
		return false;
	}
	SetTexture( texture, 0 );

	Renderer* renderer = DrawSpace::Core::Plugin<DrawSpace::Interface::Renderer>::GetInstance()->m_interface;
	if( false == renderer->CreateRenderingNode( this ) )
	{
		return false;
	}
	return true;
}
