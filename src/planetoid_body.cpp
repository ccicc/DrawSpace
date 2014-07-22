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

#include "planetoid_body.h"
#include "plugin.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Utils;
using namespace DrawSpace::Dynamics;
using namespace DrawSpace::Planetoid;


DrawSpace::Planetoid::Body::Body( const dsstring& p_name, dsreal p_ray ) : 
m_name( p_name ),
m_ray( p_ray * 1000.0 )
{
    m_world.Initialize();
       
    m_drawable = _DRAWSPACE_NEW_( SphericalLOD::Drawing, SphericalLOD::Drawing );
    m_drawable->SetName( p_name );
    m_drawable->SetRenderer( SingletonPlugin<DrawSpace::Interface::Renderer>::GetInstance()->m_interface );
    
    m_orbiter = _DRAWSPACE_NEW_( Orbiter, Orbiter( &m_world, m_drawable ) );

    m_camera_evt_cb = _DRAWSPACE_NEW_( CameraEvtCb, CameraEvtCb( this, &DrawSpace::Planetoid::Body::on_camera_event ) );
}

DrawSpace::Planetoid::Body::~Body( void )
{

}

void DrawSpace::Planetoid::Body::GetCameraHotpoint( const dsstring& p_name, Matrix& p_outmat )
{
    if( m_registered_camerapoints.count( p_name ) == 0 )
    {
        return;
    }

    if( INERTBODY_LINKED == m_registered_camerapoints[p_name].type )
    {
        m_registered_camerapoints[p_name].attached_body->GetLastLocalWorldTrans( p_outmat );
    }
    else if( FREE_ON_PLANET == m_registered_camerapoints[p_name].type )
    {
        m_registered_camerapoints[p_name].camera->GetLocalTransform( p_outmat );
    }
}

void DrawSpace::Planetoid::Body::attach_body( InertBody* p_body )
{
    p_body->Attach( m_orbiter );
    m_registered_bodies[p_body].attached = true;
}

void DrawSpace::Planetoid::Body::detach_body( InertBody* p_body )
{
    p_body->Detach();
    m_registered_bodies[p_body].attached = false;
}

void DrawSpace::Planetoid::Body::body_find_attached_camera( InertBody* p_body, std::vector<dsstring>& p_name )
{
    for( std::map<dsstring, RegisteredCamera>::iterator it = m_registered_camerapoints.begin(); it != m_registered_camerapoints.end(); ++it )
    {
        if( it->second.camera->GetAttachedBody() == p_body )
        {
            p_name.push_back( it->first );            
        }
    }
}

void DrawSpace::Planetoid::Body::on_camera_event( Scenegraph::CameraEvent p_event, TransformNode* p_node )
{
    if( DrawSpace::Scenegraph::ACTIVE == p_event )
    {
        if( !p_node )
        {
            return;
        }

        dsstring current_camera_name;
        p_node->GetName( current_camera_name );

        if( m_registered_camerapoints.count( current_camera_name ) > 0 )
        {
            m_current_camerapoint = current_camera_name;

            for( std::map<dsstring, RegisteredCamera>::iterator it = m_registered_camerapoints.begin(); it != m_registered_camerapoints.end(); ++it )
            {
                if( it->second.camera == p_node )
                {
                    Fragment* fragment = it->second.fragment;
                    m_drawable->SetCurrentPlanetBody( fragment->GetPlanetBody() );
                    
                    break;
                }
            }

        }
        else
        {
            // camera non enregistree
            m_current_camerapoint = "";
        }
    }
}

SphericalLOD::Drawing* DrawSpace::Planetoid::Body::GetDrawable( void )
{
    return m_drawable;
}

Orbiter* DrawSpace::Planetoid::Body::GetOrbiter( void )
{
    return m_orbiter;
}

World* DrawSpace::Planetoid::Body::GetWorld( void )
{
    return &m_world;
}

DrawSpace::Planetoid::Body::CameraEvtCb* DrawSpace::Planetoid::Body::GetCameraEvtCb( void )
{
    return m_camera_evt_cb;
}

void DrawSpace::Planetoid::Body::ApplyGravity( void )
{
    for( std::map<InertBody*, RegisteredBody>::iterator it = m_registered_bodies.begin(); it != m_registered_bodies.end(); ++it )
    {
        if( it->second.attached )
        {
            DrawSpace::Utils::Matrix local_pos;
            it->second.body->GetLastLocalWorldTrans( local_pos );

            Vector gravity;

            gravity[0] = -local_pos( 3, 0 );
            gravity[1] = -local_pos( 3, 1 );
            gravity[2] = -local_pos( 3, 2 );
            gravity[3] = 1.0;
            gravity.Normalize();

            InertBody::Parameters params;
            it->second.body->GetParameters( params );

            gravity.Scale( params.mass * 9.81 );

            it->second.body->ApplyForce( gravity );
        }
    }
}

void DrawSpace::Planetoid::Body::ManageBodies( void )
{
    for( std::map<DrawSpace::Dynamics::InertBody*, RegisteredBody>::iterator it = m_registered_bodies.begin(); it != m_registered_bodies.end(); ++it )
    {
        if( it->second.attached )
        {
            DrawSpace::Utils::Matrix bodypos;

            it->second.body->GetLastLocalWorldTrans( bodypos );

            DrawSpace::Utils::Vector bodypos2;
            bodypos2[0] = bodypos( 3, 0 );
            bodypos2[1] = bodypos( 3, 1 );
            bodypos2[2] = bodypos( 3, 2 );

            dsreal rel_alt = ( bodypos2.Length() / m_ray );

            if( rel_alt >= 1.2 )
            {

                detach_body( it->second.body );

                // rechercher si une camera enregistree est associee a ce body
                std::vector<dsstring> cameras;
                body_find_attached_camera( it->second.body, cameras );

                for( size_t i = 0; i < cameras.size(); i++ )
                {
                    m_registered_camerapoints[cameras[i]].camera->SetRelativeOrbiter( NULL );

                    Fragment* fragment = m_registered_camerapoints[cameras[i]].fragment;
                    fragment->SetHotState( false );

                }

                //////

                DrawSpace::Core::TransformNode* node;                
                node = it->second.body->GetDrawable();


                Fragment* fragment = it->second.fragment;
                fragment->RemoveColliderFromWorld();

                //////
            }
        }
        else
        {
            DrawSpace::Utils::Matrix bodypos;

            it->second.body->GetLastLocalWorldTrans( bodypos );

            DrawSpace::Utils::Vector bodypos2;
            bodypos2[0] = bodypos( 3, 0 );
            bodypos2[1] = bodypos( 3, 1 );
            bodypos2[2] = bodypos( 3, 2 );

            DrawSpace::Utils::Matrix planetbodypos;
            m_orbiter->GetLastWorldTransformation( planetbodypos );

            DrawSpace::Utils::Vector planetbodypos2;
            planetbodypos2[0] = planetbodypos( 3, 0 );
            planetbodypos2[1] = planetbodypos( 3, 1 );
            planetbodypos2[2] = planetbodypos( 3, 2 );

            Vector delta;

            delta[0] = planetbodypos2[0] - bodypos2[0];
            delta[1] = planetbodypos2[1] - bodypos2[1];
            delta[2] = planetbodypos2[2] - bodypos2[2];
            delta[3] = 1.0;

            dsreal rel_alt = delta.Length() / m_ray;

            if( rel_alt < 1.1 )
            {

                attach_body( it->second.body );

                /////

                std::vector<dsstring> cameras;
                body_find_attached_camera( it->second.body, cameras );

                for( size_t i = 0; i < cameras.size(); i++ )
                {
                    m_registered_camerapoints[cameras[i]].camera->SetRelativeOrbiter( m_orbiter );

                    Fragment* fragment = m_registered_camerapoints[cameras[i]].fragment;

                    fragment->SetHotState( true );
                }
            }
        }
    }
}

void DrawSpace::Planetoid::Body::Update( void )
{
    for( size_t i = 0; i < m_planetfragments_list.size(); i++ )
    {
        Fragment* curr = m_planetfragments_list[i];

        curr->Update( this );

        InertBody* inertbody = curr->GetInertBody();
        CameraPoint* camera = curr->GetCamera();

        if( inertbody )
        {
            if( inertbody->GetRefBody() == m_orbiter )
            {
                std::vector<dsstring> cameras;
                body_find_attached_camera( inertbody, cameras );

                for( size_t i = 0; i < cameras.size(); i++ )
                {
                    m_registered_camerapoints[cameras[i]].camera->SetRelativeAltitude( curr->GetPlanetBody()->GetAltitud() );
                }
            }
        }
        else if( camera )
        {
            CameraPoint::Infos cam_infos;
            camera->GetInfos( cam_infos );
            camera->SetRelativeAltitude( curr->GetPlanetBody()->GetAltitud() );
        }       
    }
}

void DrawSpace::Planetoid::Body::RegisterInertBody( const dsstring& p_bodyname, InertBody* p_body )
{
    RegisteredBody reg_body;

    reg_body.attached = false;
    reg_body.body = p_body;

    DrawSpace::SphericalLOD::Body* planet_body = _DRAWSPACE_NEW_( DrawSpace::SphericalLOD::Body, DrawSpace::SphericalLOD::Body( m_ray * 2.0 ) );
    Collider* collider = _DRAWSPACE_NEW_( Collider, Collider( &m_world ) );

    dsstring final_name = m_name + dsstring( " " ) + p_bodyname;
    Fragment* planet_fragment = _DRAWSPACE_NEW_( Fragment, Fragment( final_name, planet_body, collider, m_ray, true ) );
    planet_fragment->SetHotState( false );

    m_planetfragments_list.push_back( planet_fragment );
    reg_body.fragment = planet_fragment;

    planet_fragment->SetInertBody( p_body );

    planet_body->Initialize();

    m_registered_bodies[p_body] = reg_body;
}

void DrawSpace::Planetoid::Body::RegisterIncludedInertBody( const dsstring& p_bodyname, InertBody* p_body, const Matrix& p_initmat )
{
    RegisteredBody reg_body;

    reg_body.attached = true;
    reg_body.body = p_body;

    p_body->IncludeTo( m_orbiter, p_initmat );

    DrawSpace::SphericalLOD::Body* planet_body = _DRAWSPACE_NEW_( DrawSpace::SphericalLOD::Body, DrawSpace::SphericalLOD::Body( m_ray * 2.0 ) );
    Collider* collider = _DRAWSPACE_NEW_( Collider, Collider( &m_world ) );

    dsstring final_name = m_name + dsstring( " " ) + p_bodyname;
    Fragment* planet_fragment = _DRAWSPACE_NEW_( Fragment, Fragment( final_name, planet_body, collider, m_ray, true ) );
    planet_fragment->SetHotState( true );

    m_planetfragments_list.push_back( planet_fragment );
    reg_body.fragment = planet_fragment;

    planet_fragment->SetInertBody( p_body );

    planet_body->Initialize();

    m_registered_bodies[p_body] = reg_body;
}

bool DrawSpace::Planetoid::Body::RegisterCameraPoint( CameraPoint* p_camera )
{
    RegisteredCamera reg_camera;

    dsstring camera_name;
    p_camera->GetName( camera_name );

    reg_camera.camera = p_camera;

    ////

    Dynamics::Body* attached_body = p_camera->GetAttachedBody();

    if( attached_body )
    {
        InertBody* inert_body = dynamic_cast<InertBody*>( attached_body );
        if( inert_body )
        {
            if( m_registered_bodies.count( inert_body ) > 0 )
            {
                reg_camera.type = INERTBODY_LINKED;
                reg_camera.attached_body = inert_body;
                reg_camera.fragment = m_registered_bodies[inert_body].fragment;
            }
            else
            {
                // body attache n'est pas enregistre, erreur
                return false;
            }
        }
        else
        {
            Orbiter* orbiter = dynamic_cast<Orbiter*>( attached_body );
            if( orbiter )
            {
                // attachee a un orbiter
                if( orbiter == m_orbiter )
                {
                    // la camera est attachee a notre planete !

                    reg_camera.type = FREE_ON_PLANET;
                    reg_camera.attached_body = NULL;


                    DrawSpace::SphericalLOD::Body* planet_body = _DRAWSPACE_NEW_( DrawSpace::SphericalLOD::Body, DrawSpace::SphericalLOD::Body( m_ray * 2.0 ) );
                    Collider* collider = _DRAWSPACE_NEW_( Collider, Collider( &m_world ) );

                    dsstring final_name = m_name + dsstring( " " ) + camera_name;
                    Fragment* planet_fragment = _DRAWSPACE_NEW_( Fragment, Fragment( final_name, planet_body, collider, m_ray, false ) );

                    planet_body->Initialize();

                    planet_fragment->SetHotState( true );
                    planet_fragment->SetCamera( p_camera );

                    reg_camera.fragment = planet_fragment;

                    reg_camera.camera->SetRelativeOrbiter( m_orbiter );

                    m_planetfragments_list.push_back( planet_fragment );
                }
                else
                {
                    // la camera est attachee a un autre orbiter
                    return false;
                }
            }
            else
            {
                // camera attachee a autre chose qu'un orbiter ou un inertbody (???)
                return false;
            }
        }
    }
    else
    {
        // camera libre (attachee a aucun body)
        reg_camera.type = FREE;
        reg_camera.attached_body = NULL;


        DrawSpace::SphericalLOD::Body* planet_body = _DRAWSPACE_NEW_( DrawSpace::SphericalLOD::Body, DrawSpace::SphericalLOD::Body( m_ray * 2.0 ) );
        Collider* collider = _DRAWSPACE_NEW_( Collider, Collider( &m_world ) );

        dsstring final_name = m_name + dsstring( " " ) + camera_name;
        Fragment* planet_fragment = _DRAWSPACE_NEW_( Fragment, Fragment( final_name, planet_body, collider, m_ray, false ) );

        planet_body->Initialize();

        planet_fragment->SetHotState( false );
        planet_fragment->SetCamera( p_camera );

        reg_camera.fragment = planet_fragment;

        m_planetfragments_list.push_back( planet_fragment );
    }

    ////

    m_registered_camerapoints[camera_name] = reg_camera;

    return true;
}

void DrawSpace::Planetoid::Body::GetName( dsstring& p_name )
{
    p_name = m_name;
}