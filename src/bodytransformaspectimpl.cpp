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

#include "bodytransformaspectimpl.h"
#include "component.h"
#include "transformaspect.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Aspect;
using namespace DrawSpace::AspectImplementations;
using namespace DrawSpace::Utils;

BodyTransformAspectImpl::BodyTransformAspectImpl( btDefaultMotionState** p_motionState ) :
m_motionState( p_motionState )
{
}


void BodyTransformAspectImpl::GetLocaleTransform( TransformAspect* p_transformaspect, Utils::Matrix& p_out_base_transform )
{
    ComponentList<bool> flags;
    p_transformaspect->GetComponentsByType<bool>( flags );
    
    btScalar                 bt_matrix[16];
    DrawSpace::Utils::Matrix updated_matrix;

    if( *m_motionState )
    {        
        (*m_motionState)->m_graphicsWorldTrans.getOpenGLMatrix( bt_matrix );
   
        updated_matrix( 0, 0 ) = bt_matrix[0];
        updated_matrix( 0, 1 ) = bt_matrix[1];
        updated_matrix( 0, 2 ) = bt_matrix[2];
        updated_matrix( 0, 3 ) = bt_matrix[3];

        updated_matrix( 1, 0 ) = bt_matrix[4];
        updated_matrix( 1, 1 ) = bt_matrix[5];
        updated_matrix( 1, 2 ) = bt_matrix[6];
        updated_matrix( 1, 3 ) = bt_matrix[7];

        updated_matrix( 2, 0 ) = bt_matrix[8];
        updated_matrix( 2, 1 ) = bt_matrix[9];
        updated_matrix( 2, 2 ) = bt_matrix[10];
        updated_matrix( 2, 3 ) = bt_matrix[11];

        updated_matrix( 3, 0 ) = bt_matrix[12];
        updated_matrix( 3, 1 ) = bt_matrix[13];
        updated_matrix( 3, 2 ) = bt_matrix[14];
        updated_matrix( 3, 3 ) = bt_matrix[15];
    }
    else
    {
        updated_matrix.Identity();
    }
    p_out_base_transform = updated_matrix;
}