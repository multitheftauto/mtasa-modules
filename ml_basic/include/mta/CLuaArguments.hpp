#pragma once

#include <mta/CLuaArgument.hpp>
#include <vector>

class CLuaArguments {
    std::vector<CLuaArgument*> m_Arguments;

    void CopyArguments(const CLuaArguments& Arguments) {
        for (const auto& pArg : Arguments) {
            m_Arguments.push_back(new CLuaArgument(*pArg));
        }
    }
public:
    CLuaArguments() noexcept {}
    CLuaArguments(const CLuaArguments& Arguments) noexcept {
        // Copy all the arguments
        CopyArguments(Arguments);
    }
    ~CLuaArguments() { DeleteArguments(); };

    const CLuaArguments& operator=(const CLuaArguments& Arguments) noexcept {
        // Clear our previous list if any
        DeleteArguments();

        // Copy all the arguments
        CopyArguments(Arguments);

        // Return the given reference allowing for chaining
        return Arguments;
    }

    void ReadArguments(lua_State* luaVM, std::uint32_t uiIndexBegin = 1) noexcept {
        // Delete the previous arguments if any
        DeleteArguments();

        // Start reading arguments until there are none left
        while (lua_type(luaVM, uiIndexBegin) != LUA_TNONE) {
            // Create an argument, let it read out the argument and add it to our vector
            m_Arguments.push_back(new CLuaArgument(luaVM, uiIndexBegin++));
        }
    }

    void PushArguments(lua_State* luaVM) const noexcept {
        // Push all our arguments
        for (const auto& pArg : m_Arguments) pArg->Push(luaVM);
    }

    void PushArguments(CLuaArguments& Arguments) noexcept {
        CopyArguments(Arguments);
    }

    bool Call(lua_State* luaVM, const char* szFunction) const noexcept {
        assert(szFunction);

        // Add the function name to the stack and get the event from the table
        assert(luaVM);

        lua_pushstring(luaVM, szFunction);
        lua_gettable(luaVM, LUA_GLOBALSINDEX);

        // Push our arguments onto the stack
        PushArguments(luaVM);

        int iret = lua_pcall(luaVM, static_cast<int>(m_Arguments.size()), 0, 0);

        return iret != LUA_ERRRUN && iret != LUA_ERRMEM;
    }

    CLuaArgument* PushNil() noexcept {
        CLuaArgument* pArgument = new CLuaArgument;
        m_Arguments.push_back(pArgument);
        return pArgument;
    }

    CLuaArgument* PushBoolean(bool bBool) noexcept {
        CLuaArgument* pArgument = new CLuaArgument(bBool);
        m_Arguments.push_back(pArgument);
        return pArgument;
    }

    CLuaArgument* PushNumber(double dNumber) noexcept {
        CLuaArgument* pArgument = new CLuaArgument(dNumber);
        m_Arguments.push_back(pArgument);
        return pArgument;
    }

    CLuaArgument* PushString(const char* szString) noexcept {
        CLuaArgument* pArgument = new CLuaArgument(szString);
        m_Arguments.push_back(pArgument);
        return pArgument;
    }

    CLuaArgument* PushUserData(void* pUserData) noexcept {
        CLuaArgument* pArgument = new CLuaArgument(pUserData);
        m_Arguments.push_back(pArgument);
        return pArgument;
    }

    CLuaArgument* PushFunction(lua_CFunction function) noexcept {
        CLuaArgument* pArgument = new CLuaArgument(function);
        m_Arguments.push_back(pArgument);
        return pArgument;
    }

    void DeleteArguments() noexcept {
        // Delete each item
        for(const auto& pArg : m_Arguments) delete pArg;

        // Clear the vector
        m_Arguments.clear();
    }

    std::size_t Count() const noexcept {
        return m_Arguments.size();
    }

    constexpr std::vector<CLuaArgument*>::iterator begin() noexcept {
        return m_Arguments.begin();
    }
    constexpr std::vector<CLuaArgument*>::iterator end() noexcept {
        return m_Arguments.end();
    }
    constexpr std::vector<CLuaArgument*>::const_iterator begin() const noexcept {
        return m_Arguments.cbegin();
    }
    constexpr std::vector<CLuaArgument*>::const_iterator end() const noexcept {
        return m_Arguments.cend();
    }
};