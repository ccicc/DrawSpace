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

#include "buildobjects.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Dynamics;

Spacebox* BuildSpaceBox( const DrawSpace::Utils::SpaceboxDescriptor& p_descriptor, dsstring& p_error )
{
    Spacebox* spacebox = new Spacebox();
    
    std::map<dsstring, DrawSpace::Utils::SpaceboxPassDescriptor> passes = p_descriptor.passes_slots;

    if( 0 == passes.size() )
    {
        p_error = "BuildSpaceBox: at least one pass required";
        return NULL;
    }

    for( std::map<dsstring, DrawSpace::Utils::SpaceboxPassDescriptor>::iterator it = passes.begin(); it != passes.end(); ++it )
    {
        Pass* current_pass = dynamic_cast<Pass*>( ConfigsBase::GetInstance()->GetConfigurableInstance( it->first ) );
        if( !current_pass )
        {
            p_error = "BuildSpaceBox : unknown Pass config name (" + dsstring( it->first.c_str() ) + dsstring( ")" );
            return NULL;
        }

        spacebox->RegisterPassSlot( current_pass );

        if( false == ConfigsBase::GetInstance()->ConfigurableInstanceExists( it->second.fx_name ) )
        {
            p_error = "BuildSpaceBox : unknown Fx config name (" + it->second.fx_name + dsstring( ")" );
            return NULL;
        }

        Fx* fx = dynamic_cast<Fx*>( ConfigsBase::GetInstance()->GetConfigurableInstance( it->second.fx_name ) );

        if( NULL == fx )
        {
            p_error = "BuildSpaceBox : specified config is not an Fx (" + it->second.fx_name + dsstring( ")" );
            return NULL;
        }
       
        for( long i = 0; i < 6; i++ )
        {
            for( long j = 0; j < RenderingNode::NbMaxTextures; j++ )
            {
                if( it->second.textures[i][j] != "" )
                {
                    if( false == AssetsBase::GetInstance()->AssetIdExists( it->second.textures[i][j] ) )
                    {
                        p_error = "BuildSpaceBox : unknown texture asset name (" + it->second.textures[i][j] + dsstring( ")" );
                        return NULL;
                    }

                    Texture* texture = dynamic_cast<Texture*>( AssetsBase::GetInstance()->GetAsset( it->second.textures[i][j] ) );
                    if( NULL == texture )
                    {
                        p_error = "BuildSpaceBox : specified asset is not a Texture (" + it->second.textures[i][j] + dsstring( ")" );
                        return NULL;
                    }
                                       
                    spacebox->GetNodeFromPass( current_pass, i )->SetTexture( texture, j );

                }
            }

            spacebox->GetNodeFromPass( current_pass, i )->SetFx( fx );
            spacebox->GetNodeFromPass( current_pass, i )->SetOrderNumber( it->second.rendering_order );

            for( size_t j = 0; j < it->second.shader_params.size(); j++ )
            {
                spacebox->GetNodeFromPass( current_pass, i )->AddShaderParameter( it->second.shader_params[j].shader_index, it->second.shader_params[j].id, it->second.shader_params[j].shader_register );
                spacebox->GetNodeFromPass( current_pass, i )->SetShaderRealVector( it->second.shader_params[j].id, it->second.shader_params[j].value );
            }

        }
    }

    p_error = "";
    return spacebox;
}

DrawSpace::Chunk* BuildChunk( const DrawSpace::Utils::ChunkDescriptor& p_descriptor, dsstring& p_error )
{
    Chunk* chunk = new Chunk();

    DrawSpace::Interface::Renderer* renderer = DrawSpace::Core::SingletonPlugin<DrawSpace::Interface::Renderer>::GetInstance()->m_interface;

    std::map<dsstring, DrawSpace::Utils::ChunkPassDescriptor> passes = p_descriptor.passes_slots;
    if( 0 == passes.size() )
    {
        p_error = "BuildChunk: at least one pass required";
        return NULL;
    }

    if( "..." == p_descriptor.meshe || "" == p_descriptor.meshe )
    {
        // no meshe file specified, search for impostors description

        if( 0 == p_descriptor.impostors.size() )
        {
            p_error = "BuildChunk: messhe asset name or impostors list required";
            return NULL;
        }

        Meshe* meshe = new Meshe;
        chunk->SetMeshe( meshe );
        chunk->SetImpostorsDisplayList( p_descriptor.impostors );
        chunk->ImpostorsInit();
    }
    else
    {
        if( false == AssetsBase::GetInstance()->AssetIdExists( p_descriptor.meshe ) )
        {
            p_error = "BuildChunk : unknown meshe asset name (" + p_descriptor.meshe + dsstring( ")" );
            return NULL;
        }

        Meshe* meshe = dynamic_cast<Meshe*>( AssetsBase::GetInstance()->GetAsset( p_descriptor.meshe ) );
        if( NULL == meshe )
        {
            p_error = "BuildChunk : specified asset is not a Meshe (" + p_descriptor.meshe + dsstring( ")" );
        }
        chunk->SetMeshe( meshe );    
    }

    for( std::map<dsstring, DrawSpace::Utils::ChunkPassDescriptor>::iterator it = passes.begin(); it != passes.end(); ++it )
    {
        Pass* current_pass = dynamic_cast<Pass*>( ConfigsBase::GetInstance()->GetConfigurableInstance( it->first ) );
        if( !current_pass )
        {
            p_error = "BuildChunk : unknown Pass config name (" + dsstring( it->first.c_str() ) + dsstring( ")" );
            return NULL;
        }

        chunk->RegisterPassSlot( current_pass );

        if( false == ConfigsBase::GetInstance()->ConfigurableInstanceExists( it->second.fx_name ) )
        {
            p_error = "BuildChunk : unknown Fx config name (" + it->second.fx_name + dsstring( ")" );
            return NULL;
        }

        Fx* fx = dynamic_cast<Fx*>( ConfigsBase::GetInstance()->GetConfigurableInstance( it->second.fx_name ) );

        if( NULL == fx )
        {
            p_error = "BuildChunk : specified config is not an Fx (" + it->second.fx_name + dsstring( ")" );
            return NULL;
        }

        for( long j = 0; j < RenderingNode::NbMaxTextures; j++ )
        {
            if( it->second.textures[j] != "" )
            {
                if( false == AssetsBase::GetInstance()->AssetIdExists( it->second.textures[j] ) )
                {
                    p_error = "BuildChunk : unknown texture asset name (" + it->second.textures[j] + dsstring( ")" );
                    return NULL;
                }

                Texture* texture = dynamic_cast<Texture*>( AssetsBase::GetInstance()->GetAsset( it->second.textures[j] ) );
                if( NULL == texture )
                {
                    p_error = "BuildChunk : specified asset is not a Texture (" + it->second.textures[j] + dsstring( ")" );
                    return NULL;
                }
                                   
                chunk->GetNodeFromPass( current_pass )->SetTexture( texture, j );

            }
        }

        chunk->GetNodeFromPass( current_pass )->SetFx( fx );
        chunk->GetNodeFromPass( current_pass )->SetOrderNumber( it->second.rendering_order );

        for( size_t j = 0; j < it->second.shader_params.size(); j++ )
        {
            chunk->GetNodeFromPass( current_pass )->AddShaderParameter( it->second.shader_params[j].shader_index, it->second.shader_params[j].id, it->second.shader_params[j].shader_register );
            chunk->GetNodeFromPass( current_pass )->SetShaderRealVector( it->second.shader_params[j].id, it->second.shader_params[j].value );
        }
    }

    p_error = "";
    return chunk;
}

DrawSpace::Clouds* BuildClouds( DrawSpace::Utils::CloudsDescriptor& p_descriptor, dsstring& p_error )
{

    Clouds* clouds = new Clouds();

    DrawSpace::Interface::Renderer* renderer = DrawSpace::Core::SingletonPlugin<DrawSpace::Interface::Renderer>::GetInstance()->m_interface;

    std::map<dsstring, DrawSpace::Utils::ChunkPassDescriptor> passes = p_descriptor.chunk_descriptor.passes_slots;
    if( 0 == passes.size() )
    {
        p_error = "BuildClouds: at least one pass required";
        return NULL;
    }

    Meshe* meshe = new Meshe;
    clouds->SetMeshe( meshe );

    clouds->EnableDetails( p_descriptor.details );
    
    p_descriptor.rules = new DrawSpace::Procedural::RulesPackage( clouds->GetProceduralCallback() );
    p_descriptor.rules->InitializeSeedBase( p_descriptor.seed );
    bool parse = p_descriptor.rules->Run( p_descriptor.rules_filepath, " " );

    if( !parse )
    {
        p_error = "BuildClouds: unable to parse procedural rules file";
        return NULL;
    }

    p_descriptor.rules->GetRootParser()->GetRules()->Apply();

    /////////////////////////////////////////////////
    

    clouds->ImpostorsInit();



    for( std::map<dsstring, DrawSpace::Utils::ChunkPassDescriptor>::iterator it = passes.begin(); it != passes.end(); ++it )
    {
        Pass* current_pass = dynamic_cast<Pass*>( ConfigsBase::GetInstance()->GetConfigurableInstance( it->first ) );
        if( !current_pass )
        {
            p_error = "BuildClouds : unknown Pass config name (" + dsstring( it->first.c_str() ) + dsstring( ")" );
            return NULL;
        }

        clouds->RegisterPassSlot( current_pass );

        if( false == ConfigsBase::GetInstance()->ConfigurableInstanceExists( it->second.fx_name ) )
        {
            p_error = "BuildClouds : unknown Fx config name (" + it->second.fx_name + dsstring( ")" );
            return NULL;
        }

        Fx* fx = dynamic_cast<Fx*>( ConfigsBase::GetInstance()->GetConfigurableInstance( it->second.fx_name ) );

        if( NULL == fx )
        {
            p_error = "BuildClouds : specified config is not an Fx (" + it->second.fx_name + dsstring( ")" );
            return NULL;
        }

        for( long j = 0; j < RenderingNode::NbMaxTextures; j++ )
        {
            if( it->second.textures[j] != "" )
            {
                if( false == AssetsBase::GetInstance()->AssetIdExists( it->second.textures[j] ) )
                {
                    p_error = "BuildClouds : unknown texture asset name (" + it->second.textures[j] + dsstring( ")" );
                    return NULL;
                }

                Texture* texture = dynamic_cast<Texture*>( AssetsBase::GetInstance()->GetAsset( it->second.textures[j] ) );
                if( NULL == texture )
                {
                    p_error = "BuildClouds : specified asset is not a Texture (" + it->second.textures[j] + dsstring( ")" );
                    return NULL;
                }
                                   
                clouds->GetNodeFromPass( current_pass )->SetTexture( texture, j );
            }
        }

        clouds->GetNodeFromPass( current_pass )->SetFx( fx );
        clouds->GetNodeFromPass( current_pass )->SetOrderNumber( it->second.rendering_order );

        for( size_t j = 0; j < it->second.shader_params.size(); j++ )
        {
            clouds->GetNodeFromPass( current_pass )->AddShaderParameter( it->second.shader_params[j].shader_index, it->second.shader_params[j].id, it->second.shader_params[j].shader_register );
            clouds->GetNodeFromPass( current_pass )->SetShaderRealVector( it->second.shader_params[j].id, it->second.shader_params[j].value );
        }
    }

    p_error = "";
    return clouds;
}

InertBody* BuildInertBody( const Body::Parameters& p_params, World* p_world )
{
    InertBody* body = new InertBody( p_world, p_params );
    return body;    
}

Collider* BuildCollider( const Body::Parameters& p_params, World* p_world )
{
    Collider* coll = new Collider();

    coll->SetKinematic( p_params );
    coll->AddToWorld( p_world );

    return coll;
}
