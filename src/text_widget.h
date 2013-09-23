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

#include "widget.h"
#include "pass.h"

namespace DrawSpace
{
namespace Gui
{
class TextWidget : public Widget
{
protected:

	// image de fond
	Image*				m_backgroundimage;

	// texte
	DrawSpace::Text*	m_text;

	// image finale (representation concrete du widget)
	IntermediatePass*	m_pass;
	Image*				m_image;
	

public:
	TextWidget( const dsstring& p_name, long p_virtual_width, long p_virtual_height, DrawSpace::Core::Font* p_font, bool p_backgroundimage = true );
	virtual ~TextWidget( void );

	void SetVirtualTranslation( long p_x ,long p_y );

	Core::Fx* GetImageFx( void );
	Core::Fx* GetBackGroundImageFx( void );
	Core::Texture* GetBackGroundImageTexture( long p_stage );
	Core::Fx* GetTextFx( void );

	void SetBackgroundTexture( Core::Texture* p_backgroundtexture, long p_stage );

	void SetText( long p_x, long p_y, long p_height, const dsstring& p_text, unsigned char p_flag = 0 );

	void Draw( void );
	void RegisterToPass( Pass* p_pass );
	bool LoadAssets( void );

};
}
}
