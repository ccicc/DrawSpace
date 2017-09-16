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

#ifndef _MESHERENDERINGASPECT_H_
#define _MESHERENDERINGASPECT_H_

#include "aspect.h"
#include "renderingnode.h"

namespace DrawSpace
{
namespace Core
{
class MesheRenderingAspect : public Aspect
{
public:
    class PassSlot
    {
    private:
        dsstring                m_pass_name;
        RenderingNode*          m_rendering_node;

    public:
        PassSlot( const dsstring& p_pass_name ):
            m_pass_name( p_pass_name )
        {
            m_rendering_node = _DRAWSPACE_NEW_( RenderingNode, RenderingNode );
        }

        ~PassSlot( void )
        {
            _DRAWSPACE_DELETE_( m_rendering_node );
        }
    };
    
protected:
    virtual void on_renderingnode_draw( DrawSpace::Core::RenderingNode* p_rendering_node );
    
};

}
}

#endif