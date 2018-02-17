/* -*-LIC_BEGIN-*- */
/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2018                        
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
/* -*-LIC_END-*- */

#ifndef _ENTITYFACTORY_H_
#define _ENTITYFACTORY_H_

#include "entitynodegraph.h"
#include "jsonparser.h"
#include "hub.h"

namespace DrawSpace
{
namespace EntityGraph
{
class Factory
{
protected:
    using ObjectContentEventCb = DrawSpace::Core::CallBack3<Factory, DrawSpace::Utils::JSONParser::UserData*, DrawSpace::Utils::JSONParser::UserData*, const dsstring&, const dsstring&>;
    using ArrayContentEventCb = DrawSpace::Core::CallBack3<Factory, DrawSpace::Utils::JSONParser::UserData*, DrawSpace::Utils::JSONParser::UserData*, const dsstring&, const dsstring&>;
    using ArrayObjectContentEventCb = DrawSpace::Core::CallBack3<Factory, DrawSpace::Utils::JSONParser::UserData*, DrawSpace::Utils::JSONParser::UserData*, const dsstring&, int>;
    using StringContentEventCb = DrawSpace::Core::CallBack4<Factory, DrawSpace::Utils::JSONParser::UserData*, DrawSpace::Utils::JSONParser::UserData*, const dsstring&, const dsstring&, const dsstring&>;
    using NumericContentEventCb = DrawSpace::Core::CallBack4<Factory, DrawSpace::Utils::JSONParser::UserData*, DrawSpace::Utils::JSONParser::UserData*, const dsstring&, const dsstring&, dsreal>;
    
    using EntityData = std::pair<DrawSpace::EntityGraph::EntityNode, DrawSpace::Core::Entity*>;
    
    using ParserState = enum
    {
        EXPECT_ENTITY_DECL,
        EXPECT_ENTITY_ARGS,
        EXPECT_ASPECT_ARGS,
        EXPECT_PROCEDURAL_ASPECT_COMPONENT_DECL,
        EXPECT_PROCEDURAL_ASPECT_COMPONENT_ARGS,
        EXPECT_PROCEDURAL_PUBLISHER_ARGS
    };

    struct ParserData
    {
        EntityData  m_entity_data;
        ParserState m_parser_state;
    };
        
    using ParserDataImpl = DrawSpace::Utils::JSONParser::UserDataImpl<ParserData>;

    ObjectContentEventCb                                                            m_object_content_cb;
    ArrayContentEventCb                                                             m_array_content_cb;
    ArrayObjectContentEventCb                                                       m_array_object_content_cb;
    StringContentEventCb                                                            m_string_content_cb;
    NumericContentEventCb                                                           m_num_content_cb;

    DrawSpace::Systems::Hub&                                                        m_hub;

    std::list<ParserDataImpl>                                                       m_parser_data;
    std::map<dsstring, EntityData>                                                  m_nodes;

    DrawSpace::Aspect::ProceduralAspect::PublishProceduralBloc::ProceduralPublicationEventHandler*     m_pub_evt_handlers;

    DrawSpace::Utils::JSONParser::UserData* on_object_content( DrawSpace::Utils::JSONParser::UserData* p_userdata, const dsstring& p_owner_id, const dsstring& p_id );
    DrawSpace::Utils::JSONParser::UserData* on_array_content( DrawSpace::Utils::JSONParser::UserData* p_userdata, const dsstring& p_owner_id, const dsstring& p_id );
    DrawSpace::Utils::JSONParser::UserData* on_array_object_content( DrawSpace::Utils::JSONParser::UserData* p_userdata, const dsstring& p_owner_id, int p_index );
    DrawSpace::Utils::JSONParser::UserData* on_string_content( DrawSpace::Utils::JSONParser::UserData* p_userdata, const dsstring& p_owner_id, const dsstring& p_id, const dsstring& p_str );
    DrawSpace::Utils::JSONParser::UserData* on_num_content( DrawSpace::Utils::JSONParser::UserData* p_userdata, const dsstring& p_owner_id, const dsstring& p_id, dsreal p_val );

public:
    Factory( DrawSpace::Systems::Hub& p_hub );
    bool BuildFromFile( const std::string& p_filepath, DrawSpace::EntityGraph::EntityNode& p_node, DrawSpace::Aspect::ProceduralAspect::PublishProceduralBloc::ProceduralPublicationEventHandler* p_pub_evt_handler );

};
}
}

#endif