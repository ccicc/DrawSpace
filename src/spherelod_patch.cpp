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

#include "spherelod_patch.h"
#include "spherelod_drawing.h"
#include "renderer.h"
#include "plugin.h"
#include "maths.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Utils;
using namespace DrawSpace::SphericalLOD;

Patch::Patch( dsreal p_ray, int p_orientation, Patch* p_parent, int p_nodeid, BaseQuadtreeNode* p_owner, 
                Patch::SubPassCreationHandler* p_handler, DrawSpace::SphericalLOD::Config* p_config, int p_layer_index, int p_nbLODRanges ) : 

m_orientation( p_orientation ),
m_ray( p_ray ),
m_owner( p_owner ),
m_datatexture_pass( NULL ),
m_config( p_config ),
m_subpasscreation_handler( p_handler ),
m_parent( p_parent ),
m_nodeid( p_nodeid ),
m_subpass_entry_infos_valid( false ),
m_nbLODRanges( p_nbLODRanges )
{
    m_enable_datatexture = m_config->m_layers_descr[p_layer_index].enable_datatextures;

    for( long i = 0; i < 8; i++ )
    {
        m_neighbours[i] = NULL;
    }

    dsreal ui1, vi1, ui2, vi2;

    if( NULL == p_parent )
    {
        //m_lod_level = NB_LOD_RANGES - 1;
        m_lod_level = m_nbLODRanges - 1;
        m_xpos = m_ypos = 0.0;
        m_sidelength = 2.0;    // on travaille sur une sphere de rayon = 1.0, donc diametre = 2.0

    }
    else
    {
        m_lod_level = p_parent->m_lod_level - 1;
        m_sidelength = p_parent->m_sidelength / 2.0;

        switch( p_nodeid )
        {
            case BaseQuadtreeNode::NorthWestNode:

                m_xpos = -p_parent->m_sidelength / 4.0;
                m_xpos += p_parent->m_xpos;
                m_ypos = p_parent->m_sidelength / 4.0;
                m_ypos += p_parent->m_ypos;

                ui1 = 0.0;
                vi1 = 0.0;
                ui2 = 0.5;
                vi2 = 0.5;

                break;

            case BaseQuadtreeNode::NorthEastNode:

                m_xpos = p_parent->m_sidelength / 4.0;
                m_xpos += p_parent->m_xpos;
                m_ypos = p_parent->m_sidelength / 4.0;
                m_ypos += p_parent->m_ypos;

                ui1 = 0.5;
                vi1 = 0.0;
                ui2 = 1.0;
                vi2 = 0.5;

                break;

            case BaseQuadtreeNode::SouthEastNode:

                m_xpos = p_parent->m_sidelength / 4.0;
                m_xpos += p_parent->m_xpos;
                m_ypos = -p_parent->m_sidelength / 4.0;
                m_ypos += p_parent->m_ypos;

                ui1 = 0.5;
                vi1 = 0.5;
                ui2 = 1.0;
                vi2 = 1.0;

                break;

            case BaseQuadtreeNode::SouthWestNode:

                m_xpos = -p_parent->m_sidelength / 4.0;
                m_xpos += p_parent->m_xpos;
                m_ypos = -p_parent->m_sidelength / 4.0;
                m_ypos += p_parent->m_ypos;

                ui1 = 0.0;
                vi1 = 0.5;
                ui2 = 0.5;
                vi2 = 1.0;

                break;

            default:
                m_xpos = 0.0; m_ypos = 0.0;
                break;
        }       
    }

    if( m_enable_datatexture )
    {
        //if( m_lod_level == NB_LOD_RANGES - 1 )
        if( m_lod_level == m_nbLODRanges - 1)
        {
            prepare_data_texture( m_subpasscreation_handler, 1, p_layer_index );
        }
    
        //else if( m_lod_level >= NB_LOD_RANGES - 8 )
        else if( m_lod_level >= m_nbLODRanges - 8 )
        {
            prepare_data_texture( m_subpasscreation_handler, 0, p_layer_index );
        }
    }

    if( p_parent )
    {
        m_texture_referent = p_parent->m_texture_referent;

        m_global_ref_u1 = ( ui1 * ( p_parent->m_global_ref_u2 - p_parent->m_global_ref_u1 ) ) + p_parent->m_global_ref_u1;
        m_global_ref_v1 = ( vi1 * ( p_parent->m_global_ref_v2 - p_parent->m_global_ref_v1 ) ) + p_parent->m_global_ref_v1;

        m_global_ref_u2 = ( ui2 * ( p_parent->m_global_ref_u2 - p_parent->m_global_ref_u1 ) ) + p_parent->m_global_ref_u1;
        m_global_ref_v2 = ( vi2 * ( p_parent->m_global_ref_v2 - p_parent->m_global_ref_v1 ) ) + p_parent->m_global_ref_v1;


        m_global_u1 = ( ui1 * ( p_parent->m_global_u2 - p_parent->m_global_u1 ) ) + p_parent->m_global_u1;
        m_global_v1 = ( vi1 * ( p_parent->m_global_v2 - p_parent->m_global_v1 ) ) + p_parent->m_global_v1;

        m_global_u2 = ( ui2 * ( p_parent->m_global_u2 - p_parent->m_global_u1 ) ) + p_parent->m_global_u1;
        m_global_v2 = ( vi2 * ( p_parent->m_global_v2 - p_parent->m_global_v1 ) ) + p_parent->m_global_v1;

    }
    else
    {
        m_texture_referent = this;

        m_global_ref_u1 = 0.0;
        m_global_ref_v1 = 0.0;
        m_global_ref_u2 = 1.0;
        m_global_ref_v2 = 1.0;

        m_global_u1 = 0.0;
        m_global_v1 = 0.0;
        m_global_u2 = 1.0;
        m_global_v2 = 1.0;
    }
}

Patch::~Patch( void )
{
    destroy_color_texture();
}

void Patch::prepare_data_texture( Patch::SubPassCreationHandler* p_handler, int p_subpass_dest, int p_layer_index )
{
    m_datatexture_pass = create_data_texture_pass();

    std::vector<Patch*> dl;
    dl.push_back( this );

    // creation/preparation du node

    DrawSpace::Interface::Renderer* renderer = SingletonPlugin<DrawSpace::Interface::Renderer>::GetInstance()->m_interface;
    FaceDrawingNode* node = _DRAWSPACE_NEW_( FaceDrawingNode, FaceDrawingNode( renderer, m_config, p_layer_index ) );
            
    node->SetMeshe( SphericalLOD::Body::m_patch2_meshe );
    node->SetDisplayList( dl );

    //node->SetBinder( m_config->m_patchTexturesBinder[m_orientation] );
    node->SetBinder( m_config->m_layers_descr[p_layer_index].patchTexturesBinder[m_orientation] );
               
    void* tx_data;
    if( false == renderer->CreateTexture( m_datatexture_pass->GetTargetTexture(), &tx_data ) )
    {
        _DSEXCEPTION( "failed to create subpasstarget texture in renderer" );
    }
            
    ////////////////////////

    m_subpass = m_datatexture_pass;
    m_subpass_node = node;
        
    // appel handler pour enregistrer et executer la passe
    
    if( p_handler )
    {
        m_subpass_entry_infos = (*p_handler)( this, p_subpass_dest );
        m_subpass_entry_infos_valid = true;
    }
}

void Patch::destroy_color_texture( void )
{
    // remove texture on renderer side

    if( m_subpass && m_subpass_node )
    {
        if( m_subpass_entry_infos_valid )
        {
            try
            {
                remove_entry_from_queue( m_subpass_entry_infos );

            } 
            catch( ... )
            {
                _DSEXCEPTION( "unexpected error while trying to remove subpass queue entry" );
            }
        }

        DrawSpace::Interface::Renderer* renderer = SingletonPlugin<DrawSpace::Interface::Renderer>::GetInstance()->m_interface;
        renderer->DestroyTexture( m_subpass->GetTargetTexture()->GetRenderData() );

        // remove node
        _DRAWSPACE_DELETE_( m_subpass_node );

        // remove pass
        _DRAWSPACE_DELETE_( m_subpass );
    }
}

void Patch::SetNeighbour( DrawSpace::Utils::BaseQuadtreeNode* p_patch, int p_id )
{
    m_neighbours[p_id] = p_patch;
}

DrawSpace::Utils::BaseQuadtreeNode* Patch::GetNeighbour( int p_id )
{
    return m_neighbours[p_id];
}

int Patch::GetLodLevel( void )
{
    return m_lod_level;
}

void Patch::CubeToSphere( const Vector& p_in, Vector& p_out )
{
    dsreal x = p_in[0];
    dsreal y = p_in[1];
    dsreal z = p_in[2];

    p_out[0] = x * sqrt( 1.0 - y * y * 0.5 - z * z * 0.5 + y * y * z * z / 3.0 );
    p_out[1] = y * sqrt( 1.0 - z * z * 0.5 - x * x * 0.5 + x * x * z * z / 3.0 );
    p_out[2] = z * sqrt( 1.0 - x * x * 0.5 - y * y * 0.5 + x * x * y * y / 3.0 );
}

void Patch::SphereToCube( const Vector& p_in, Vector& p_out )
{
    dsreal nx, ny, nz;
    nx = ny = nz = 0;

    dsreal x, y, z;

    x = p_in[0];
    y = p_in[1];
    z = p_in[2];

    dsreal fx, fy, fz;
    fx = fabs( p_in[0] );
    fy = fabs( p_in[1] );
    fz = fabs( p_in[2] );

    int nbIter = 6;

    if( fy >= fx && fy >= fz )
    {
        if( y > 0 )
        {
            for( int n = 0; n < nbIter; n++ )
            {
                nx = x / sqrt( 1.0 - ny * ny * 0.5 - nz * nz * 0.5 + ny * ny * nz * nz / 3.0 );
                ny = y / sqrt( 1.0 - nz * nz * 0.5 - nx * nx * 0.5 + nz * nz * nx * nx / 3.0 );
                nz = z / sqrt( 1.0 - nx * nx * 0.5 - ny * ny * 0.5 + nx * nx * ny * ny / 3.0 );

                nx = nx / ny;
                nz = nz / ny;
                ny = 1.0;
            }
        }
        else
        {
            for( int n = 0; n < nbIter; n++ )
            {
                nx = x / sqrt( 1.0 - ny * ny * 0.5 - nz * nz * 0.5 + ny * ny * nz * nz / 3.0 );
                ny = y / sqrt( 1.0 - nz * nz * 0.5 - nx * nx * 0.5 + nz * nz * nx * nx / 3.0 );
                nz = z / sqrt( 1.0 - nx * nx * 0.5 - ny * ny * 0.5 + nx * nx * ny * ny / 3.0 );

                nx = nx / (-ny);
                nz = nz / (-ny);
                ny = -1.0;
            }
        }
    }
    else if( fx >= fy && fx >= fz )
    {
        if( x > 0 )
        {
            for( int n = 0; n < nbIter; n++ )
            {
                nx = x / sqrt( 1.0 - ny * ny * 0.5 - nz * nz * 0.5 + ny * ny * nz * nz / 3.0 );
                ny = y / sqrt( 1.0 - nz * nz * 0.5 - nx * nx * 0.5 + nz * nz * nx * nx / 3.0 );
                nz = z / sqrt( 1.0 - nx * nx * 0.5 - ny * ny * 0.5 + nx * nx * ny * ny / 3.0 );

                nz = nz / nx;
                ny = ny / nx;
                nx = 1.0;
            }
        }
        else
        {
            for( int n = 0; n < nbIter; n++ )
            {
                nx = x / sqrt( 1.0 - ny * ny * 0.5 - nz * nz * 0.5 + ny * ny * nz * nz / 3.0 );
                ny = y / sqrt( 1.0 - nz * nz * 0.5 - nx * nx * 0.5 + nz * nz * nx * nx / 3.0 );
                nz = z / sqrt( 1.0 - nx * nx * 0.5 - ny * ny * 0.5 + nx * nx * ny * ny / 3.0 );

                nz = nz / (-nx);
                ny = ny / (-nx);
                nx = -1.0;
            }
        }
    }
    else
    {
        if( z > 0 )
        {
            for( int n = 0; n < nbIter; n++ )
            {
                nx = x / sqrt( 1.0 - ny * ny * 0.5 - nz * nz * 0.5 + ny * ny * nz * nz / 3.0 );
                ny = y / sqrt( 1.0 - nz * nz * 0.5 - nx * nx * 0.5 + nz * nz * nx * nx / 3.0 );
                nz = z / sqrt( 1.0 - nx * nx * 0.5 - ny * ny * 0.5 + nx * nx * ny * ny / 3.0 );

                nx = nx / nz;
                ny = ny / nz;
                nz = 1.0;
            }
        }
        else
        {
            for( int n = 0; n < nbIter; n++ )
            {
                nx = x / sqrt( 1.0 - ny * ny * 0.5 - nz * nz * 0.5 + ny * ny * nz * nz / 3.0 );
                ny = y / sqrt( 1.0 - nz * nz * 0.5 - nx * nx * 0.5 + nz * nz * nx * nx / 3.0 );
                nz = z / sqrt( 1.0 - nx * nx * 0.5 - ny * ny * 0.5 + nx * nx * ny * ny / 3.0 );

                nx = nx / (-nz);
                ny = ny / (-nz);
                nz = -1.0;
            }
        }
    }

    p_out[0] = nx;
    p_out[1] = ny;
    p_out[2] = nz;
}

dsreal Patch::GetSideLength( void )
{
    return m_sidelength * m_ray;
}

dsreal Patch::GetUnitSideLenght( void )
{
    return m_sidelength;
}

dsreal Patch::GetTriangleSideLength( void )
{
    return ( ( m_sidelength * m_ray ) / ( PATCH_RESOLUTION - 1 ) );
}

void Patch::GetPos( dsreal& p_xpos, dsreal& p_ypos )
{
    p_xpos = m_xpos * m_ray;
    p_ypos = m_ypos * m_ray;
}

void Patch::GetUnitPos( dsreal& p_xpos, dsreal& p_ypos )
{
    p_xpos = m_xpos;
    p_ypos = m_ypos;
}

BaseQuadtreeNode* Patch::GetOwner( void )
{
    return m_owner;
}

int Patch::GetOrientation( void )
{
    return m_orientation;
}

void Patch::XYToXYZ( int p_orientation, dsreal p_x, dsreal p_y, Vector& p_out )
{
    switch( p_orientation )
    {
        case SphericalLOD::Patch::FrontPlanetFace:

            p_out[0] = p_x;
            p_out[1] = p_y;
            p_out[2] = 1.0;
            break;

        case SphericalLOD::Patch::RearPlanetFace:

            p_out[0] = -p_x;
            p_out[1] = p_y;
            p_out[2] = -1.0;
            break;

        case SphericalLOD::Patch::LeftPlanetFace:

            p_out[0] = -1.0;
            p_out[1] = p_y;
            p_out[2] = p_x;
            break;

        case SphericalLOD::Patch::RightPlanetFace:

            p_out[0] = 1.0;
            p_out[1] = p_y;
            p_out[2] = -p_x;
            break;

        case SphericalLOD::Patch::TopPlanetFace:

            p_out[0] = p_x;
            p_out[1] = 1.0;
            p_out[2] = -p_y;
            break;

        case SphericalLOD::Patch::BottomPlanetFace:

            p_out[0] = p_x;
            p_out[1] = -1.0;
            p_out[2] = p_y;
            break;
    }
}

void Patch::ConvertVectorToFrontFaceCoords( int p_orientation, const DrawSpace::Utils::Vector& p_in, DrawSpace::Utils::Vector& p_out )
{
    if( p_orientation == Patch::FrontPlanetFace )
    {
        p_out[0] = p_in[0];
        p_out[1] = p_in[1];
        p_out[2] = p_in[2];
        p_out[3] = 0.0;
    }

    if( p_orientation == Patch::RearPlanetFace )
    {
        p_out[0] = -p_in[0];
        p_out[1] = p_in[1];
        p_out[2] = -p_in[2];
        p_out[3] = 0.0;
    }

    if( p_orientation == Patch::TopPlanetFace )
    {
        p_out[0] = p_in[0];
        p_out[1] = -p_in[2];
        p_out[2] = p_in[1];
        p_out[3] = 0.0;
    }

    if( p_orientation == Patch::BottomPlanetFace )
    {
        p_out[0] = p_in[0];
        p_out[1] = p_in[2];
        p_out[2] = -p_in[1];
        p_out[3] = 0.0;
    }


    if( p_orientation == Patch::RightPlanetFace )
    {
        p_out[0] = -p_in[2];
        p_out[1] = p_in[1];
        p_out[2] = p_in[0];
        p_out[3] = 0.0;
    }

    if( p_orientation == Patch::LeftPlanetFace )
    {
        p_out[0] = p_in[2];
        p_out[1] = p_in[1];
        p_out[2] = -p_in[0];
        p_out[3] = 0.0;
    }
}

void Patch::ProjectVertex( const DrawSpace::Utils::Vector& p_in, DrawSpace::Utils::Vector& p_out )
{
    DrawSpace::Utils::Vector in = p_in;
    DrawSpace::Utils::Vector v2, v3;

    // sidelenght scaling
    in.Scale( m_sidelength / 2.0 );

    // patch positionning
    in[0] = in[0] + m_xpos;
    in[1] = in[1] + m_ypos;
    in[2] = 0.0;
    in[3] = 1.0;

    // patch reorientation
    XYToXYZ( m_orientation, in[0], in[1], v2 );
    v2[3] = 1.0;

    CubeToSphere( v2, v3 );
    v3[3] = 1.0;
    p_out = v3;
}

void Patch::GetGlobalRelUVCoords( DrawSpace::Utils::Vector& p_uv )
{
    p_uv[0] = m_global_ref_u1;
    p_uv[1] = m_global_ref_v1;
    p_uv[2] = m_global_ref_u2;
    p_uv[3] = m_global_ref_v2;
}

void Patch::GetGlobalUVCoords( DrawSpace::Utils::Vector& p_uv )
{
    p_uv[0] = m_global_u1;
    p_uv[1] = m_global_v1;
    p_uv[2] = m_global_u2;
    p_uv[3] = m_global_v2;
}

void Patch::GetNormalVector( int p_orientation, DrawSpace::Utils::Vector& p_vector )
{
    Vector face_dir;
    switch( p_orientation )
    {
        case Patch::FrontPlanetFace:

            face_dir[0] = 0.0;
            face_dir[1] = 0.0;
            face_dir[2] = 1.0;
            face_dir[3] = 1.0;
            break;

        case Patch::RearPlanetFace:

            face_dir[0] = 0.0;
            face_dir[1] = 0.0;
            face_dir[2] = -1.0;
            face_dir[3] = 1.0;
            break;

        case Patch::TopPlanetFace:

            face_dir[0] = 0.0;
            face_dir[1] = 1.0;
            face_dir[2] = 0.0;
            face_dir[3] = 1.0;
            break;

        case Patch::BottomPlanetFace:

            face_dir[0] = 0.0;
            face_dir[1] = -1.0;
            face_dir[2] = 0.0;
            face_dir[3] = 1.0;
            break;

        case Patch::RightPlanetFace:

            face_dir[0] = 1.0;
            face_dir[1] = 0.0;
            face_dir[2] = 0.0;
            face_dir[3] = 1.0;
            break;

        case Patch::LeftPlanetFace:

            face_dir[0] = -1.0;
            face_dir[1] = 0.0;
            face_dir[2] = 0.0;
            face_dir[3] = 1.0;
            break;
    }
    p_vector = face_dir;
}

bool Patch::IsCircleIntersection( dsreal p_centerx, dsreal p_centery, dsreal p_ray )
{    
    dsreal xpos = m_xpos * m_ray;
    dsreal ypos = m_ypos * m_ray;
    dsreal sidelength = m_sidelength * m_ray;

    Utils::Vector dist( xpos - p_centerx, ypos - p_centery, 0.0, 0.0 );

    if( dist.Length() < p_ray + sidelength )
    {
        return true;
    }
    return false;
}

DrawSpace::IntermediatePass* Patch::create_data_texture_pass( void )
{
    char thisname[32];

    sprintf( thisname, "patch_%x", this );

    dsstring complete_name = dsstring( thisname ) + dsstring( "_colortexture_pass" );
    IntermediatePass* ipass = _DRAWSPACE_NEW_( IntermediatePass, IntermediatePass( complete_name ) );

    ipass->SetTargetDimsFromRenderer( false );    
    //ipass->SetTargetDims( 512, 512 );
    //ipass->SetTargetDims( 256, 256 );
    //ipass->SetTargetDims( 128, 128 );

    ipass->SetTargetDims( PATCH_HIGH_RESOLUTION, PATCH_HIGH_RESOLUTION );

    ipass->SetRenderPurpose( Texture::RENDERPURPOSE_FLOATVECTOR );
    
    ipass->Initialize();
    ipass->GetRenderingQueue()->EnableDepthClearing( false );
    ipass->GetRenderingQueue()->EnableTargetClearing( false );
    //ipass->GetRenderingQueue()->SetTargetClearingColor( 0, 0, 0, 255 );

    return ipass;
}

DrawSpace::Core::Texture* Patch::GetDataTexture( void )
{
    if( m_datatexture_pass )
    {
        return m_datatexture_pass->GetTargetTexture();
    }
    return NULL;
}

Patch* Patch::GetTextureReferent( void )
{
    return m_texture_referent;
}

void Patch::SubPassDone( void )
{
    // subpass effectuee, l'entree dans la queue n'existe donc plus...
    m_subpass_entry_infos_valid = false;

    if( m_parent )
    {
        m_texture_referent = this;
        m_global_ref_u1 = 0.0;
        m_global_ref_v1 = 0.0;
        m_global_ref_u2 = 1.0;
        m_global_ref_v2 = 1.0;

        if( m_owner->HasChildren() )
        {
            for( long i = 0; i < 4; i++ )
            {
                QuadtreeNode<Patch>* child = static_cast<QuadtreeNode<Patch>*>( m_owner->GetChild( i ) );
           
                child->GetContent()->recurs_update_texture_referent( m_texture_referent );
            }
        }
    }
}

void Patch::recurs_update_texture_referent( Patch* p_texture_referent )
{
    m_texture_referent = p_texture_referent;

    dsreal ui1, vi1, ui2, vi2;

    switch( m_nodeid )
    {

        case BaseQuadtreeNode::NorthWestNode:

            ui1 = 0.0;
            vi1 = 0.0;
            ui2 = 0.5;
            vi2 = 0.5;

            break;

        case BaseQuadtreeNode::NorthEastNode:

            ui1 = 0.5;
            vi1 = 0.0;
            ui2 = 1.0;
            vi2 = 0.5;

            break;

        case BaseQuadtreeNode::SouthEastNode:


            ui1 = 0.5;
            vi1 = 0.5;
            ui2 = 1.0;
            vi2 = 1.0;

            break;

        case BaseQuadtreeNode::SouthWestNode:


            ui1 = 0.0;
            vi1 = 0.5;
            ui2 = 0.5;
            vi2 = 1.0;

            break;
    }


    m_global_ref_u1 = ( ui1 * ( m_parent->m_global_ref_u2 - m_parent->m_global_ref_u1 ) ) + m_parent->m_global_ref_u1;
    m_global_ref_v1 = ( vi1 * ( m_parent->m_global_ref_v2 - m_parent->m_global_ref_v1 ) ) + m_parent->m_global_ref_v1;

    m_global_ref_u2 = ( ui2 * ( m_parent->m_global_ref_u2 - m_parent->m_global_ref_u1 ) ) + m_parent->m_global_ref_u1;
    m_global_ref_v2 = ( vi2 * ( m_parent->m_global_ref_v2 - m_parent->m_global_ref_v1 ) ) + m_parent->m_global_ref_v1;

    if( m_owner->HasChildren() )
    {
        for( long i = 0; i < 4; i++ )
        {
            QuadtreeNode<Patch>* child = static_cast<QuadtreeNode<Patch>*>( m_owner->GetChild( i ) );
           
            child->GetContent()->recurs_update_texture_referent( p_texture_referent );
        }
    }
}
