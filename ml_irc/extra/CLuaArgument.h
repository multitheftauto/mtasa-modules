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

#ifndef __CLUAARGUMENT_H
#define __CLUAARGUMENT_H

extern "C"
{
    #include "../include/lua.h"
}

class CLuaArgument
{
public:
                            CLuaArgument        ( void );
                            CLuaArgument        ( bool bBool );
                            CLuaArgument        ( double dNumber );
                            CLuaArgument        ( const char* szString );
                            CLuaArgument        ( void* pUserData );
                            CLuaArgument        ( const CLuaArgument& Argument );
                            CLuaArgument        ( lua_State* luaVM, unsigned int uiArgument );
                            ~CLuaArgument       ( void );

    const CLuaArgument&     operator =          ( const CLuaArgument& Argument );
    bool                    operator ==         ( const CLuaArgument& Argument );
    bool                    operator !=         ( const CLuaArgument& Argument );

    void                    Read                ( lua_State* luaVM, unsigned int uiArgument );
    void                    Push                ( lua_State* luaVM ) const;

    inline int              GetType             ( void ) const      { return m_iType; };

    inline bool             GetBoolean          ( void ) const      { return m_bBoolean; };
    inline lua_Number       GetNumber           ( void ) const      { return m_Number; };
    inline const char*      GetString           ( void ) const      { return m_szString; };
    inline void*            GetLightUserData    ( void ) const      { return m_pLightUserData; };

private:
    int                     m_iType;
    bool                    m_bBoolean;
    lua_Number              m_Number;
    char*                   m_szString;
    void*                   m_pLightUserData;
};

#endif
