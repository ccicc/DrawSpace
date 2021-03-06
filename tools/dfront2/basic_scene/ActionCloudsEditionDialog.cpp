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

#include <wx/wx.h>
#include "BasicSceneMainFrame.h"

#include "ActionCloudsEditionDialog.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Dynamics;
using namespace DrawSpace::Utils;

void ActionCloudsEditionDialog::Execute( void )
{
    void* id = BasicSceneMainFrame::GetInstance()->m_last_clicked_treeitem.GetID();

    if( BasicSceneMainFrame::GetInstance()->m_clouds_descriptors.count( id ) > 0 )
    {
        BasicSceneMainFrame::SceneNodeEntry<DrawSpace::Clouds> clouds_node = BasicSceneMainFrame::GetInstance()->m_clouds_nodes[id];

        DrawSpace::Utils::CloudsDescriptor clouds_descr = BasicSceneMainFrame::GetInstance()->m_clouds_descriptors[id];

        DIALOG_ACTION_DECLARE( DIALOG_CLOUDS_EDITION_TITLE )
       
        DIALOG_APPENDROOT_STRING( "scene name", clouds_descr.chunk_descriptor.scene_name )
        DIALOG_APPENDROOT_FLOAT( "sorting distance", clouds_node.scene_node->GetContent()->GetSortingDistance() )

        for( std::map<dsstring, ChunkPassDescriptor>::iterator it = clouds_descr.chunk_descriptor.passes_slots.begin(); it != clouds_descr.chunk_descriptor.passes_slots.end(); ++it )
        {
            ChunkPassDescriptor pass_descr = it->second;
            IntermediatePass* ipass = static_cast<IntermediatePass*>( ConfigsBase::GetInstance()->GetConfigurableInstance( it->first ) );

            DrawSpace::Core::RenderingNode* rendering_node = clouds_node.scene_node->GetContent()->GetNodeFromPass( ipass );

            std::map<dsstring, RenderingNode::ShadersParams*> shaders_params_list;
            rendering_node->GetShadersParams( shaders_params_list );

            DIALOG_APPENDROOT_NODE( it->first, pass_root );

            DIALOG_BUILD_LABELS( pass_descr.shader_params.size(), "shader parameter %d", params_list )

            DIALOG_APPENDNODE_ITERATE_NODE_BEGIN( pass_root, i, params_list, param_root )
                
                DIALOG_APPENDNODE_STRING( param_root, "param id", pass_descr.shader_params[i].id );

                DIALOG_APPENDNODE_NODE( param_root, "values", shader_param_values_root )
                    
                Vector params_value = shaders_params_list[pass_descr.shader_params[i].id]->param_values;

                DIALOG_APPENDNODE_FLOAT( shader_param_values_root, "x", params_value[0] )
                DIALOG_APPENDNODE_FLOAT( shader_param_values_root, "y", params_value[1] )
                DIALOG_APPENDNODE_FLOAT( shader_param_values_root, "z", params_value[2] )
                DIALOG_APPENDNODE_FLOAT( shader_param_values_root, "w", params_value[3] )

            DIALOG_APPENDNODE_ITERATE_NODE_END
        }

        DIALOG_APPLY

        DIALOG_SHOW
    }

}
