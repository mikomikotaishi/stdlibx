/**
 * @file lua.cppm
 * @module stdx:lua
 * @brief Implementation of the Lua extension library.
 *
 * This file contains the implementation of the Lua extension library, which is used to provide Lua functionality within the standard library.
 */

module;

#ifdef STDLIBX_EXTENSIONS_COMPILE_LUA_LIBRARY
#include <lua.hpp>
#endif

export module stdx:lua;

import :main;

using namespace stdx::core;

/**
 * @namespace stdx::lua
 * @brief Wrapper namespace for standard library extension Lua libraries.
 */
export namespace stdx::lua {
#ifdef STDLIBX_EXTENSIONS_COMPILE_LUA_LIBRARY
    constexpr i32 OK = LUA_OK;
    constexpr i32 YIELD = LUA_YIELD;
    constexpr i32 ERRRUN = LUA_ERRRUN;
    constexpr i32 ERRSYNTAX = LUA_ERRSYNTAX;
    constexpr i32 ERRMEM = LUA_ERRMEM;
    constexpr i32 ERRERR = LUA_ERRERR;

    constexpr i32 TNONE = LUA_TNONE;
    constexpr i32 TNIL = LUA_TNIL;
    constexpr i32 TBOOLEAN = LUA_TBOOLEAN;
    constexpr i32 TLIGHTUSERDATA = LUA_TLIGHTUSERDATA;
    constexpr i32 TNUMBER = LUA_TNUMBER;
    constexpr i32 TSTRING = LUA_TSTRING;
    constexpr i32 TTABLE = LUA_TTABLE;
    constexpr i32 TFUNCTION = LUA_TFUNCTION;
    constexpr i32 TUSERDATA = LUA_TUSERDATA;
    constexpr i32 TTHREAD = LUA_TTHREAD;
    constexpr i32 NUMTYPES = LUA_NUMTYPES;

    constexpr i32 MAXSTACK = LUAI_MAXSTACK;
    constexpr i32 MINSTACK = LUA_MINSTACK;
    constexpr i32 RIDX_MAINTHREAD = LUA_RIDX_MAINTHREAD;
    constexpr i32 RIDX_GLOBALS = LUA_RIDX_GLOBALS;
    constexpr i32 RIDX_LAST = LUA_RIDX_LAST;

    constexpr i32 OPADD = LUA_OPADD;
    constexpr i32 OPSUB = LUA_OPSUB;
    constexpr i32 OPMUL = LUA_OPMUL;
    constexpr i32 OPMOD = LUA_OPMOD;
    constexpr i32 OPPOW = LUA_OPPOW;
    constexpr i32 OPDIV = LUA_OPDIV;
    constexpr i32 OPIDIV = LUA_OPIDIV;
    constexpr i32 OPBAND = LUA_OPBAND;
    constexpr i32 OPBOR = LUA_OPBOR;
    constexpr i32 OPBXOR = LUA_OPBXOR;
    constexpr i32 OPSHL = LUA_OPSHL;
    constexpr i32 OPSHR = LUA_OPSHR;
    constexpr i32 OPUNM = LUA_OPUNM;
    constexpr i32 OPBNOT = LUA_OPBNOT;

    constexpr i32 OPEQ = LUA_OPEQ;
    constexpr i32 OPLT = LUA_OPLT;
    constexpr i32 OPLE = LUA_OPLE;

    constexpr i32 GCSTOP = LUA_GCSTOP;
    constexpr i32 GCRESTART = LUA_GCRESTART;
    constexpr i32 GCCOLLECT = LUA_GCCOLLECT;
    constexpr i32 GCCOUNT = LUA_GCCOUNT;
    constexpr i32 GCCOUNTB = LUA_GCCOUNTB;
    constexpr i32 GCSTEP = LUA_GCSTEP;
    constexpr i32 GCSETPAUSE = LUA_GCSETPAUSE;
    constexpr i32 GCSETSTEPMUL = LUA_GCSETSTEPMUL;
    constexpr i32 GCISRUNNING = LUA_GCISRUNNING;
    constexpr i32 GCGEN = LUA_GCGEN;
    constexpr i32 GCINC = LUA_GCINC;

    constexpr i32 NUMTAGS = LUA_NUMTAGS;

    constexpr i32 HOOKCALL = LUA_HOOKCALL;
    constexpr i32 HOOKRET = LUA_HOOKRET;
    constexpr i32 HOOKLINE = LUA_HOOKLINE;
    constexpr i32 HOOKCOUNT = LUA_HOOKCOUNT;
    constexpr i32 HOOKTAILCALL = LUA_HOOKTAILCALL;

    constexpr i32 MASKCALL = LUA_MASKCALL;
    constexpr i32 MASKRET = LUA_MASKRET;
    constexpr i32 MASKLINE = LUA_MASKLINE;
    constexpr i32 MASKCOUNT = LUA_MASKCOUNT;

    constexpr StringView COLIBNAME = "coroutine";
    constexpr StringView TABLIBNAME = "table";
    constexpr StringView IOLIBNAME = "io";
    constexpr StringView OSLIBNAME = "os";
    constexpr StringView STRLIBNAME = "string";
    constexpr StringView UTF8LIBNAME = "utf8";
    constexpr StringView MATHLIBNAME = "math";
    constexpr StringView DBLIBNAME = "debug";
    constexpr StringView LOADLIBNAME = "package";

    using State = ::lua_State;
    using LuaNumber = ::lua_Number;
    using LuaInteger = ::lua_Integer;
    using LuaUnsigned = ::lua_Unsigned;
    using KContext = ::lua_KContext;
    using CFunction = ::lua_CFunction;
    using KFunction = ::lua_KFunction;
    using Reader = ::lua_Reader;
    using Writer = ::lua_Writer;
    using Alloc = ::lua_Alloc;
    using WarnFunction = ::lua_WarnFunction;
    using Debug = ::lua_Debug;
    using Hook = ::lua_Hook;

    [[nodiscard]]
    State* new_state(Alloc f, void* ud) noexcept {
        return lua_newstate(f, ud);
    }

    void close(State* l) noexcept {
        lua_close(l);
    }

    [[nodiscard]]
    State* new_thread(State* l) noexcept {
        return lua_newthread(l);
    }

    i32 close_thread(State* l, State* from) noexcept {
        return lua_closethread(l, from);
    }

    [[deprecated]]
    i32 reset_thread(State* l) noexcept {
        return lua_resetthread(l);
    }

    CFunction at_panic(State* l, CFunction panicf) noexcept {
        return lua_atpanic(l, panicf);
    }

    [[nodiscard]]
    LuaNumber version(State* l) noexcept {
        return lua_version(l);
    }

    [[nodiscard]]
    i32 abs_index(State* l, i32 idx) noexcept {
        return lua_absindex(l, idx);
    }

    [[nodiscard]]
    i32 get_top(State* l) noexcept {
        return lua_gettop(l);
    }

    void set_top(State* l, i32 idx) noexcept {
        lua_settop(l, idx);
    }

    void push_value(State* l, i32 idx) noexcept {
        lua_pushvalue(l, idx);
    }

    void rotate(State* l, i32 idx, i32 n) noexcept {
        lua_rotate(l, idx, n);
    }

    void copy(State* l, i32 fromidx, i32 toidx) noexcept {
        lua_copy(l, fromidx, toidx);
    }

    [[nodiscard]]
    i32 check_stack(State* l, i32 n) noexcept {
        return lua_checkstack(l, n);
    }

    [[nodiscard]]
    i32 is_number(State* l, i32 idx) noexcept {
        return lua_isnumber(l, idx);
    }

    [[nodiscard]]
    i32 is_string(State* l, i32 idx) noexcept {
        return lua_isstring(l, idx);
    }

    [[nodiscard]]
    i32 is_cfunction(State* l, i32 idx) noexcept {
        return lua_iscfunction(l, idx);
    }

    [[nodiscard]]
    i32 is_integer(State* l, i32 idx) noexcept {
        return lua_isinteger(l, idx);
    }

    [[nodiscard]]
    i32 is_userdata(State* l, i32 idx) noexcept {
        return lua_isuserdata(l, idx);
    }

    [[nodiscard]]
    i32 type(State* l, i32 idx) noexcept {
        return lua_type(l, idx);
    }

    [[nodiscard]]
    String type_name(State* l, i32 tp) noexcept {
        return String(lua_typename(l, tp));
    }

    [[nodiscard]]
    LuaNumber to_number_x(State* l, i32 idx, i32* isnum) noexcept {
        return lua_tonumberx(l, idx, isnum);
    }

    [[nodiscard]]
    LuaInteger to_integer_x(State* l, i32 idx, i32* isnum) noexcept {
        return lua_tointegerx(l, idx, isnum);
    }

    [[nodiscard]]
    i32 to_boolean(State* l, i32 idx) noexcept {
        return lua_toboolean(l, idx);
    }

    [[nodiscard]]
    String to_lstring(State* l, i32 idx, usize* len) noexcept {
        return String(lua_tolstring(l, idx, len));
    }

    [[nodiscard]]
    LuaUnsigned raw_len(State* l, i32 idx) noexcept {
        return lua_rawlen(l, idx);
    }

    [[nodiscard]]
    CFunction to_cfunction(State* l, i32 idx) noexcept {
        return lua_tocfunction(l, idx);
    }

    [[nodiscard]]
    void* to_userdata(State* l, i32 idx) noexcept {
        return lua_touserdata(l, idx);
    }

    [[nodiscard]]
    State* to_thread(State* l, i32 idx) noexcept {
        return lua_tothread(l, idx);
    }

    [[nodiscard]]
    const void* to_pointer(State* l, i32 idx) noexcept {
        return lua_topointer(l, idx);
    }

    void arith(State* l, i32 op) noexcept {
        lua_arith(l, op);
    }

    [[nodiscard]]
    i32 raw_equal(State* l, i32 idx1, i32 idx2) noexcept {
        return lua_rawequal(l, idx1, idx2);
    }

    [[nodiscard]]
    i32 compare(State* l, i32 idx1, i32 idx2, i32 op) noexcept {
        return lua_compare(l, idx1, idx2, op);
    }

    void push_nil(State* l) noexcept {
        lua_pushnil(l);
    }

    void push_number(State* l, LuaNumber n) noexcept {
        lua_pushnumber(l, n);
    }

    void push_integer(State* l, LuaInteger n) noexcept {
        lua_pushinteger(l, n);
    }

    void push_lstring(State* l, StringView s) noexcept {
        lua_pushlstring(l, s.data(), s.size());
    }

    void push_string(State* l, StringView s) noexcept {
        lua_pushlstring(l, s.data(), s.size());
    }

    [[nodiscard]]
    String push_vfstring(State* l, StringView fmt, auto&&... args) noexcept {
        return String(lua_pushvfstring(
            l,
            fmt.data(),
            stdx::util::forward<decltype(args)>(args)...
        ));
    }

    [[nodiscard]]
    String push_fstring(State* l, StringView fmt, auto&&... args) noexcept {
        return String(lua_pushvfstring(
            l,
            fmt.data(),
            stdx::util::forward<decltype(args)>(args)...
        ));
    }

    void push_cclosure(State* l, CFunction fn, i32 n) noexcept {
        lua_pushcclosure(l, fn, n);
    }

    void push_boolean(State* l, i32 b) noexcept {
        lua_pushboolean(l, b);
    }

    void push_lightuserdata(State* l, void* p) noexcept {
        lua_pushlightuserdata(l, p);
    }

    i32 push_thread(State* l) noexcept {
        return lua_pushthread(l);
    }

    [[nodiscard]]
    i32 get_global(State* l, StringView name) noexcept {
        return lua_getglobal(l, name.data());
    }

    [[nodiscard]]
    i32 get_table(State* l, i32 idx) noexcept {
        return lua_gettable(l, idx);
    }

    [[nodiscard]]
    i32 get_field(State* l, i32 idx, StringView k) noexcept {
        return lua_getfield(l, idx, k.data());
    }

    [[nodiscard]]
    i32 get_i(State* l, i32 idx, LuaInteger n) noexcept {
        return lua_geti(l, idx, n);
    }

    [[nodiscard]]
    i32 raw_get(State* l, i32 idx) noexcept {
        return lua_rawget(l, idx);
    }

    [[nodiscard]]
    i32 raw_get_i(State* l, i32 idx, LuaInteger n) noexcept {
        return lua_rawgeti(l, idx, n);
    }

    [[nodiscard]]
    i32 raw_get_p(State* l, i32 idx, const void* p) noexcept {
        return lua_rawgetp(l, idx, p);
    }

    void create_table(State* l, i32 narr, i32 nrec) noexcept {
        lua_createtable(l, narr, nrec);
    }

    [[nodiscard]]
    void* new_userdata_uv(State* l, usize sz, i32 nuvalue) noexcept {
        return lua_newuserdatauv(l, sz, nuvalue);
    }

    [[nodiscard]]
    i32 get_metatable(State* l, i32 objindex) noexcept {
        return lua_getmetatable(l, objindex);
    }

    [[nodiscard]]
    i32 get_i_userdata_value(State* l, i32 idx, i32 n) noexcept {
        return lua_getiuservalue(l, idx, n);
    }

    void set_global(State* l, StringView name) noexcept {
        lua_setglobal(l, name.data());
    }

    void set_table(State* l, i32 idx) noexcept {
        lua_settable(l, idx);
    }

    void set_field(State* l, i32 idx, StringView k) noexcept {
        lua_setfield(l, idx, k.data());
    }

    void set_i(State* l, i32 idx, LuaInteger n) noexcept {
        lua_seti(l, idx, n);
    }

    void raw_set(State* l, i32 idx) noexcept {
        lua_rawset(l, idx);
    }

    void raw_set_i(State* l, i32 idx, LuaInteger n) noexcept {
        lua_rawseti(l, idx, n);
    }

    void raw_set_p(State* l, i32 idx, const void* p) noexcept {
        lua_rawsetp(l, idx, p);
    }

    int set_metatable(State* l, i32 objindex) noexcept {
        return lua_setmetatable(l, objindex);
    }

    int set_i_userdata_value(State* l, i32 idx, i32 n) noexcept {
        return lua_setiuservalue(l, idx, n);
    }

    void callk(State* l, i32 nargs, i32 nresults, KContext ctx, KFunction k) noexcept {
        lua_callk(l, nargs, nresults, ctx, k);
    }

    void call(State* l, i32 nargs, i32 nresults) noexcept {
        lua_call(l, nargs, nresults);
    }

    void pcallk(State* l, i32 nargs, i32 nresults, i32 errfunc, KContext ctx, KFunction k) noexcept {
        lua_pcallk(l, nargs, nresults, errfunc, ctx, k);
    }

    void pcall(State* l, i32 nargs, i32 nresults, i32 errfunc) noexcept {
        lua_pcall(l, nargs, nresults, errfunc);
    }

    i32 load(State* l, Reader reader, void* dt, StringView chunkname, StringView mode) noexcept {
        return lua_load(
            l,
            reader,
            dt,
            chunkname.data(),
            mode.data()
        );
    }

    i32 dump(State* l, Writer writer, void* data, i32 strip) noexcept {
        return lua_dump(l, writer, data, strip);
    }

    i32 yieldk(State* l, i32 nresults, KContext ctx, KFunction k) noexcept {
        return lua_yieldk(l, nresults, ctx, k);
    }

    i32 yield(State* l, i32 nresults) noexcept {
        return lua_yield(l, nresults);
    }

    i32 resume(State* l, State* from, i32 narg, i32* nres) noexcept {
        return lua_resume(l, from, narg, nres);
    }

    [[nodiscard]]
    i32 status(State* l) noexcept {
        return lua_status(l);
    }

    [[nodiscard]]
    i32 is_yieldable(State* l) noexcept {
        return lua_isyieldable(l);
    }

    void set_warnf(State* l, WarnFunction f, void* ud) noexcept {
        lua_setwarnf(l, f, ud);
    }

    void warning(State* l, StringView msg, i32 tocont) noexcept {
        lua_warning(l, msg.data(), tocont);
    }

    i32 gc(State* l, i32 what, auto&&... args) noexcept {
        return lua_gc(
            l,
            what,
            stdx::util::forward<decltype(args)>(args)...
        );
    }

    [[nodiscard]]
    i32 error(State* l) noexcept {
        return lua_error(l);
    }

    [[nodiscard]]
    i32 next(State* l, i32 idx) noexcept {
        return lua_next(l, idx);
    }

    void concat(State* l, i32 n) noexcept {
        lua_concat(l, n);
    }

    void len(State* l, i32 idx) noexcept {
        lua_len(l, idx);
    }

    [[nodiscard]]
    usize string_to_number(State* l, StringView s) noexcept {
        return lua_stringtonumber(l, s.data());
    }

    [[nodiscard]]
    Alloc get_allocf(State* l, void** ud) noexcept {
        return lua_getallocf(l, ud);
    }

    void set_allocf(State* l, Alloc f, void* ud) noexcept {
        lua_setallocf(l, f, ud);
    }

    void to_close(State* l, i32 idx) noexcept {
        lua_toclose(l, idx);
    }

    void close_slot(State* l, i32 idx) noexcept {
        lua_closeslot(l, idx);
    }

    [[nodiscard]]
    i32 get_stack(State* l, i32 level, Debug* ar) noexcept {
        return lua_getstack(l, level, ar);
    }

    [[nodiscard]]
    i32 get_info(State* l, StringView what, Debug* ar) noexcept {
        return lua_getinfo(l, what.data(), ar);
    }

    [[nodiscard]]
    String get_local(State* l, const Debug* ar, i32 n) noexcept {
        return String(lua_getlocal(l, ar, n));
    }

    String set_local(State* l, const Debug* ar, i32 n) noexcept {
        return String(lua_setlocal(l, ar, n));
    }

    [[nodiscard]]
    String get_upvalue(State* l, i32 funcindex, i32 n) noexcept {
        return String(lua_getupvalue(l, funcindex, n));
    }

    String set_upvalue(State* l, i32 funcindex, i32 n) noexcept {
        return String(lua_setupvalue(l, funcindex, n));
    }

    [[nodiscard]]
    void* upvalue_id(State* l, i32 fidx, i32 n) noexcept {
        return lua_upvalueid(l, fidx, n);
    }

    void upvalue_join(State* l, i32 fidx1, i32 n1, i32 fidx2, i32 n2) noexcept {
        lua_upvaluejoin(l, fidx1, n1, fidx2, n2);
    }

    void set_hook(State* l, Hook f, i32 mask, i32 count) noexcept {
        lua_sethook(l, f, mask, count);
    }

    [[nodiscard]]
    Hook get_hook(State* l) noexcept {
        return lua_gethook(l);
    }

    [[nodiscard]]
    i32 get_hook_mask(State* l) noexcept {
        return lua_gethookmask(l);
    }

    [[nodiscard]]
    i32 get_hook_count(State* l) noexcept {
        return lua_gethookcount(l);
    }

    i32 set_cstack_limit(State* l, u32 limit) noexcept {
        return lua_setcstacklimit(l, limit);
    }

    i32 open_base(State* l) noexcept {
        return luaopen_base(l);
    }

    i32 open_coroutine(State* l) noexcept {
        return luaopen_coroutine(l);
    }

    i32 open_table(State* l) noexcept {
        return luaopen_table(l);
    }

    i32 open_io(State* l) noexcept {
        return luaopen_io(l);
    }

    i32 open_os(State* l) noexcept {
        return luaopen_os(l);
    }

    i32 open_string(State* l) noexcept {
        return luaopen_string(l);
    }

    i32 open_utf8(State* l) noexcept {
        return luaopen_utf8(l);
    }

    i32 open_math(State* l) noexcept {
        return luaopen_math(l);
    }

    i32 open_debug(State* l) noexcept {
        return luaopen_debug(l);
    }

    i32 open_package(State* l) noexcept {
        return luaopen_package(l);
    }

    namespace libs {
        void open_libs(State* l) noexcept {
            luaL_openlibs(l);
        }

        constexpr i32 NOREF = LUA_NOREF;
        constexpr i32 REFNIL = LUA_REFNIL;

        using LuaBuffer = ::luaL_Buffer;
        using LuaRegistry = ::luaL_Reg;
        using LuaStream = ::luaL_Stream;

        void check_version(State* l) noexcept {
            luaL_checkversion(l);
        }

        [[nodiscard]]
        i32 get_metafield(State* l, i32 obj, StringView e) noexcept {
            return luaL_getmetafield(l, obj, e.data());
        }

        i32 call_meta(State* l, i32 obj, StringView e) noexcept {
            return luaL_callmeta(l, obj, e.data());
        }

        [[nodiscard]]
        String to_lstring(State* l, i32 idx, usize* len) noexcept {
            return String(luaL_tolstring(l, idx, len));
        }

        [[nodiscard]]
        i32 arg_error(State* l, i32 arg, StringView extramsg) noexcept {
            return luaL_argerror(l, arg, extramsg.data());
        }

        [[nodiscard]]
        i32 type_error(State* l, i32 arg, StringView tname) noexcept {
            return luaL_typeerror(l, arg, tname.data());
        }

        [[nodiscard]]
        String check_lstring(State* l, i32 arg, usize* len) noexcept {
            return String(luaL_checklstring(l, arg, len));
        }

        [[nodiscard]]
        String opt_lstring(State* l, i32 arg, StringView def, usize* len) noexcept {
            return String(luaL_optlstring(l, arg, def.data(), len));
        }

        [[nodiscard]]
        LuaNumber check_number(State* l, i32 arg) noexcept {
            return luaL_checknumber(l, arg);
        }

        [[nodiscard]]
        LuaNumber opt_number(State* l, i32 arg, LuaNumber def) noexcept {
            return luaL_optnumber(l, arg, def);
        }

        [[nodiscard]]
        LuaInteger check_integer(State* l, i32 arg) noexcept {
            return luaL_checkinteger(l, arg);
        }

        [[nodiscard]]
        LuaInteger opt_integer(State* l, i32 arg, LuaInteger def) noexcept {
            return luaL_optinteger(l, arg, def);
        }

        void check_stack(State* l, i32 sz, StringView msg) noexcept {
            luaL_checkstack(l, sz, msg.data());
        }

        void check_type(State* l, i32 arg, i32 t) noexcept {
            luaL_checktype(l, arg, t);
        }

        void check_any(State* l, i32 arg) noexcept {
            luaL_checkany(l, arg);
        }

        [[nodiscard]]
        i32 new_metatable(State* l, StringView tname) noexcept {
            return luaL_newmetatable(l, tname.data());
        }

        void set_metatable(State* l, StringView tname) noexcept {
            luaL_setmetatable(l, tname.data());
        }

        void* test_udata(State* l, i32 ud, StringView tname) noexcept {
            return luaL_testudata(l, ud, tname.data());
        }

        void* check_udata(State* l, i32 ud, StringView tname) noexcept {
            return luaL_checkudata(l, ud, tname.data());
        }

        void where(State* l, i32 lvl) noexcept {
            luaL_where(l, lvl);
        }

        [[nodiscard]]
        i32 error(State* l, StringView fmt, auto&&... args) noexcept {
            return luaL_error(
                l,
                fmt.data(),
                stdx::util::forward<decltype(args)>(args)...
            );
        }

        [[nodiscard]]
        i32 check_option(State* l, i32 arg, StringView def, Span<const char* const> lst) noexcept {
            return luaL_checkoption(l, arg, def.data(), lst.data());
        }

        [[nodiscard]]
        i32 file_result(State* l, i32 stat, StringView fname) noexcept {
            return luaL_fileresult(l, stat, fname.data());
        }

        [[nodiscard]]
        i32 exec_result(State* l, i32 stat) noexcept {
            return luaL_execresult(l, stat);
        }

        i32 ref(State* l, i32 t) noexcept {
            return luaL_ref(l, t);
        }

        void unref(State* l, i32 t, i32 ref) noexcept {
            luaL_unref(l, t, ref);
        }

        i32 load_file_x(State* l, StringView filename, StringView mode) noexcept {
            return luaL_loadfilex(l, filename.data(), mode.data());
        }

        i32 load_file(State* l, StringView f) noexcept {
            return luaL_loadfilex(l, f.data(), nullptr);
        }

        i32 load_buffer_x(State* l, StringView buff, StringView name, StringView mode) noexcept {
            return luaL_loadbufferx(
                l,
                buff.data(),
                buff.size(),
                name.data(),
                mode.data()
            );
        }

        i32 load_string(State* l, StringView s) noexcept {
            return luaL_loadstring(l, s.data());
        }

        [[nodiscard]]
        State* new_state() noexcept {
            return luaL_newstate();
        }

        [[nodiscard]]
        LuaInteger len(State* l, i32 idx) noexcept {
            return luaL_len(l, idx);
        }

        // TODO: Fix this function - luaL_addgsub signature doesn't match
        // void add_gsub(State* l, StringView name, CFunction f) noexcept {
        //     luaL_addgsub(l, name.data(), "%s+", "_", f);
        // }

        [[nodiscard]]
        String gsub(State* l, StringView s, StringView p, StringView r) noexcept {
            return String(luaL_gsub(l, s.data(), p.data(), r.data()));
        }

        void set_funcs(State* l, const LuaRegistry* lreg, i32 nup) noexcept {
            luaL_setfuncs(l, lreg, nup);
        }

        [[nodiscard]]
        i32 get_subtable(State* l, i32 idx, StringView fname) noexcept {
            return luaL_getsubtable(l, idx, fname.data());
        }

        void traceback(State* l, State* from, StringView msg, i32 level) noexcept {
            luaL_traceback(l, from, msg.data(), level);
        }

        void requiref(State* l, StringView modname, i32 ref) noexcept {
            luaL_requiref(l, modname.data(), reinterpret_cast<CFunction>(ref), 1);
        }

        void buff_init(State* l, LuaBuffer* B) noexcept {
            luaL_buffinit(l, B);
        }

        [[nodiscard]]
        String prep_buff_size(LuaBuffer* B, usize sz) noexcept {
            return String(luaL_prepbuffsize(B, sz));
        }

        void add_lstring(LuaBuffer* B, StringView s) noexcept {
            luaL_addlstring(B, s.data(), s.size());
        }

        void add_string(LuaBuffer* B, StringView s) noexcept {
            luaL_addstring(B, s.data());
        }

        void add_value(LuaBuffer* B) noexcept {
            luaL_addvalue(B);
        }

        void push_result(LuaBuffer* B) noexcept {
            luaL_pushresult(B);
        }

        void push_result_size(LuaBuffer* B, usize sz) noexcept {
            luaL_pushresultsize(B, sz);
        }

        [[nodiscard]]
        String buff_init_size(State* l, LuaBuffer* b, usize sz) noexcept {
            return String(luaL_buffinitsize(l, b, sz));
        }
    }
#endif
}
