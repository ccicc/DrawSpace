

#include "frontluacontext.h"
#include "renderframe.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;

FrontLuaContext::FrontLuaContext( void )
{
}

FrontLuaContext::~FrontLuaContext( void )
{
}

void FrontLuaContext::Startup( void )
{
	m_L = luaL_newstate();
	
	luaopen_io( m_L );
    luaopen_base( m_L );
    luaopen_table( m_L );
    luaopen_string( m_L );    

    REGISTER_FUNC( "print", lua_print );
}

void FrontLuaContext::Stop( void )
{
	lua_close( m_L );
}

void FrontLuaContext::Exec( const char* p_cmd )
{
	int status = luaL_dostring( m_L, p_cmd );

	if( status )
	{
		char* err_msg = (char *)lua_tostring( m_L, -1 );
        //printf( "error : %s\n", err_msg );
        m_lasterror = err_msg;
		// popper le message d'erreur
		lua_pop( m_L, 1 );

        if( m_errorhandler )
        {
            (*m_errorhandler)( m_lasterror );
        }
	}
}

void FrontLuaContext::Execfile( const char* p_path )
{    
	int status = luaL_dofile( m_L, p_path );

	if( status )
	{
		char* err_msg = (char *)lua_tostring( m_L, -1 );
		//printf( "error : %s\n", err_msg );
        m_lasterror = err_msg;
        // popper le message d'erreur
		lua_pop( m_L, 1 );

        if( m_errorhandler )
        {
            (*m_errorhandler)( m_lasterror );
        }
	}    
}

void FrontLuaContext::GetLastError( dsstring& p_str )
{
    p_str = m_lasterror;
}

void FrontLuaContext::RegisterErrorHandler( BaseCallback<void, const dsstring&>* p_handler )
{
    m_errorhandler = p_handler;
}

lua_State* FrontLuaContext::GetLuaState( void )
{
    return m_L;
}


int FrontLuaContext::lua_print( lua_State* p_L )
{
    std::string text;

    int argc = lua_gettop( p_L );
    if( argc != 1 )
    {
		lua_pushstring( p_L, "print : bad number of args" );
		lua_error( p_L );
    }

    const char* arg;

	arg = luaL_checkstring( p_L, 1 );
	text = arg;

    RenderFrame::GetInstance()->PrintConsole( text );

    return 0;
}