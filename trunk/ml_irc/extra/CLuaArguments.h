/*********************************************************
*
*  Multi Theft Auto v1.0 - Module
*  
*  Module:       ml_irc
*  Url:          http://development.mtasa.com/index.php?title=Modules/SebasIRC
*  Project page: http://multitheftauto-modules.googlecode.com/
*  Developers:   Sebas Lamers <baslamers@home.nl>
*
*********************************************************/

#ifndef __CLUAARGUMENTS_H
#define __CLUAARGUMENTS_H

extern "C"
{
    #include "../include/lua.h"
}

#include "CLuaArgument.h"
#include <vector>

using namespace std;

class CLuaArguments
{
public:
    inline                                              CLuaArguments       ( void )                {};
                                                        CLuaArguments       ( const CLuaArguments& Arguments );
    inline                                              ~CLuaArguments      ( void )                { DeleteArguments (); };

    const CLuaArguments&                                operator =          ( const CLuaArguments& Arguments );

    void                                                ReadArguments       ( lua_State* luaVM, unsigned int uiIndexBegin = 1 );
    void                                                PushArguments       ( lua_State* luaVM ) const;
    void                                                PushArguments       ( CLuaArguments& Arguments );
    bool                                                Call                ( lua_State* luaVM, const char* szFunction ) const;

    CLuaArgument*                                       PushNil             ( void );
    CLuaArgument*                                       PushBoolean         ( bool bBool );
    CLuaArgument*                                       PushNumber          ( double dNumber );
    CLuaArgument*                                       PushString          ( const char* szString );
    CLuaArgument*                                       PushUserData        ( void* pUserData );

    void                                                DeleteArguments     ( void );

    inline unsigned int                                 Count               ( void ) const          { return static_cast < unsigned int > ( m_Arguments.size () ); };
    inline vector < CLuaArgument* > ::const_iterator    IterBegin           ( void )                { return m_Arguments.begin (); };
    inline vector < CLuaArgument* > ::const_iterator    IterEnd             ( void )                { return m_Arguments.end (); };

private:
    vector < CLuaArgument* >                            m_Arguments;
};

#endif
