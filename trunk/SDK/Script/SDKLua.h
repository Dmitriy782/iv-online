#pragma once

extern "C" {

/*
** $Id: lua.h,v 1.285 2013/03/15 13:04:22 roberto Exp $
** Lua - A Scripting Language
** Lua.org, PUC-Rio, Brazil (http://www.lua.org)
** See Copyright Notice at the end of this file
*/

#include <stdarg.h>
#include <stddef.h>


//#include "luaconf.h"


#define LUA_VERSION_MAJOR	"5"
#define LUA_VERSION_MINOR	"2"
#define LUA_VERSION_NUM		502
#define LUA_VERSION_RELEASE	"2"

#define LUA_VERSION		"Lua " LUA_VERSION_MAJOR "." LUA_VERSION_MINOR
#define LUA_RELEASE		LUA_VERSION "." LUA_VERSION_RELEASE
#define LUA_COPYRIGHT	LUA_RELEASE "  Copyright (C) 1994-2013 Lua.org, PUC-Rio"
#define LUA_AUTHORS		"R. Ierusalimschy, L. H. de Figueiredo, W. Celes"


/* mark for precompiled code ('<esc>Lua') */
#define LUA_SIGNATURE	"\033Lua"

/* option for multiple returns in 'lua_pcall' and 'lua_call' */
#define LUA_MULTRET	(-1)


/*
** pseudo-indices
*/
#define LUA_REGISTRYINDEX	LUAI_FIRSTPSEUDOIDX
#define lua_upvalueindex(i)	(LUA_REGISTRYINDEX - (i))


/* thread status */
#define LUA_OK		0
#define LUA_YIELD	1
#define LUA_ERRRUN	2
#define LUA_ERRSYNTAX	3
#define LUA_ERRMEM	4
#define LUA_ERRGCMM	5
#define LUA_ERRERR	6


typedef struct lua_State lua_State;

typedef int (*lua_CFunction) (lua_State *L);


/*
** functions that read/write blocks when loading/dumping Lua chunks
*/
typedef const char * (*lua_Reader) (lua_State *L, void *ud, size_t *sz);

typedef int (*lua_Writer) (lua_State *L, const void* p, size_t sz, void* ud);


/*
** prototype for memory-allocation functions
*/
typedef void * (*lua_Alloc) (void *ud, void *ptr, size_t osize, size_t nsize);


/*
** basic types
*/
#define LUA_TNONE		(-1)

#define LUA_TNIL		0
#define LUA_TBOOLEAN		1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER		3
#define LUA_TSTRING		4
#define LUA_TTABLE		5
#define LUA_TFUNCTION		6
#define LUA_TUSERDATA		7
#define LUA_TTHREAD		8

#define LUA_NUMTAGS		9



/* minimum Lua stack available to a C function */
#define LUA_MINSTACK	20


/* predefined values in the registry */
#define LUA_RIDX_MAINTHREAD	1
#define LUA_RIDX_GLOBALS	2
#define LUA_RIDX_LAST		LUA_RIDX_GLOBALS


/* type of numbers in Lua */
typedef double lua_Number;


/* type for integer functions */
typedef int lua_Integer;

/* unsigned integer type */
typedef unsigned int lua_Unsigned;



/*
** RCS ident string
*/
extern const char lua_ident[];


/*
** state manipulation
*/
typedef lua_State *(* Flua_newstate) (lua_Alloc f, void *ud);
typedef void       (* Flua_close) (lua_State *L);
typedef lua_State *(* Flua_newthread) (lua_State *L);

typedef lua_CFunction (* Flua_atpanic) (lua_State *L, lua_CFunction panicf);


typedef const lua_Number *(* Flua_version) (lua_State *L);


/*
** basic stack manipulation
*/
typedef int   (* Flua_absindex) (lua_State *L, int idx);
typedef int   (* Flua_gettop) (lua_State *L);
typedef void  (* Flua_settop) (lua_State *L, int idx);
typedef void  (* Flua_pushvalue) (lua_State *L, int idx);
typedef void  (* Flua_remove) (lua_State *L, int idx);
typedef void  (* Flua_insert) (lua_State *L, int idx);
typedef void  (* Flua_replace) (lua_State *L, int idx);
typedef void  (* Flua_copy) (lua_State *L, int fromidx, int toidx);
typedef int   (* Flua_checkstack) (lua_State *L, int sz);

typedef void  (* Flua_xmove) (lua_State *from, lua_State *to, int n);


/*
** access functions (stack -> C)
*/

typedef int             (* Flua_isnumber) (lua_State *L, int idx);
typedef int             (* Flua_isstring) (lua_State *L, int idx);
typedef int             (* Flua_iscfunction) (lua_State *L, int idx);
typedef int             (* Flua_isuserdata) (lua_State *L, int idx);
typedef int             (* Flua_type) (lua_State *L, int idx);
typedef const char     *(* Flua_typename) (lua_State *L, int tp);

typedef lua_Number      (* Flua_tonumberx) (lua_State *L, int idx, int *isnum);
typedef lua_Integer     (* Flua_tointegerx) (lua_State *L, int idx, int *isnum);
typedef lua_Unsigned    (* Flua_tounsignedx) (lua_State *L, int idx, int *isnum);
typedef int             (* Flua_toboolean) (lua_State *L, int idx);
typedef const char     *(* Flua_tolstring) (lua_State *L, int idx, size_t *len);
typedef size_t          (* Flua_rawlen) (lua_State *L, int idx);
typedef lua_CFunction   (* Flua_tocfunction) (lua_State *L, int idx);
typedef void	       *(* Flua_touserdata) (lua_State *L, int idx);
typedef lua_State      *(* Flua_tothread) (lua_State *L, int idx);
typedef const void     *(* Flua_topointer) (lua_State *L, int idx);


/*
** Comparison and arithmetic functions
*/

#define LUA_OPADD	0	/* ORDER TM */
#define LUA_OPSUB	1
#define LUA_OPMUL	2
#define LUA_OPDIV	3
#define LUA_OPMOD	4
#define LUA_OPPOW	5
#define LUA_OPUNM	6

typedef void  (* Flua_arith) (lua_State *L, int op);

#define LUA_OPEQ	0
#define LUA_OPLT	1
#define LUA_OPLE	2

typedef int   (* Flua_rawequal) (lua_State *L, int idx1, int idx2);
typedef int   (* Flua_compare) (lua_State *L, int idx1, int idx2, int op);


/*
** push functions (C -> stack)
*/
typedef void        (* Flua_pushnil) (lua_State *L);
typedef void        (* Flua_pushnumber) (lua_State *L, lua_Number n);
typedef void        (* Flua_pushinteger) (lua_State *L, lua_Integer n);
typedef void        (* Flua_pushunsigned) (lua_State *L, lua_Unsigned n);
typedef const char *(* Flua_pushlstring) (lua_State *L, const char *s, size_t l);
typedef const char *(* Flua_pushstring) (lua_State *L, const char *s);
typedef const char *(* Flua_pushvfstring) (lua_State *L, const char *fmt,
                                                      va_list argp);
typedef const char *(* Flua_pushfstring) (lua_State *L, const char *fmt, ...);
typedef void  (* Flua_pushcclosure) (lua_State *L, lua_CFunction fn, int n);
typedef void  (* Flua_pushboolean) (lua_State *L, int b);
typedef void  (* Flua_pushlightuserdata) (lua_State *L, void *p);
typedef int   (* Flua_pushthread) (lua_State *L);


/*
** get functions (Lua -> stack)
*/
typedef void  (* Flua_getglobal) (lua_State *L, const char *var);
typedef void  (* Flua_gettable) (lua_State *L, int idx);
typedef void  (* Flua_getfield) (lua_State *L, int idx, const char *k);
typedef void  (* Flua_rawget) (lua_State *L, int idx);
typedef void  (* Flua_rawgeti) (lua_State *L, int idx, int n);
typedef void  (* Flua_rawgetp) (lua_State *L, int idx, const void *p);
typedef void  (* Flua_createtable) (lua_State *L, int narr, int nrec);
typedef void *(* Flua_newuserdata) (lua_State *L, size_t sz);
typedef int   (* Flua_getmetatable) (lua_State *L, int objindex);
typedef void  (* Flua_getuservalue) (lua_State *L, int idx);


/*
** set functions (stack -> Lua)
*/
typedef void  (* Flua_setglobal) (lua_State *L, const char *var);
typedef void  (* Flua_settable) (lua_State *L, int idx);
typedef void  (* Flua_setfield) (lua_State *L, int idx, const char *k);
typedef void  (* Flua_rawset) (lua_State *L, int idx);
typedef void  (* Flua_rawseti) (lua_State *L, int idx, int n);
typedef void  (* Flua_rawsetp) (lua_State *L, int idx, const void *p);
typedef int   (* Flua_setmetatable) (lua_State *L, int objindex);
typedef void  (* Flua_setuservalue) (lua_State *L, int idx);


/*
** 'load' and 'call' functions (load and run Lua code)
*/
typedef void  (* Flua_callk) (lua_State *L, int nargs, int nresults, int ctx,lua_CFunction k);
typedef int   (* Flua_getctx) (lua_State *L, int *ctx);
typedef int   (* Flua_pcallk) (lua_State *L, int nargs, int nresults, int errfunc,int ctx, lua_CFunction k);
typedef int   (* Flua_load) (lua_State *L, lua_Reader reader, void *dt, const char *chunkname, const char *mode);
typedef int	  (* Flua_dump) (lua_State *L, lua_Writer writer, void *data);


/*
** coroutine functions
*/
typedef int  (* Flua_yieldk) (lua_State *L, int nresults, int ctx, lua_CFunction k);
typedef int  (* Flua_resume) (lua_State *L, lua_State *from, int narg);
typedef int  (* Flua_status) (lua_State *L);

/*
** garbage-collection function and options
*/

#define LUA_GCSTOP		0
#define LUA_GCRESTART		1
#define LUA_GCCOLLECT		2
#define LUA_GCCOUNT		3
#define LUA_GCCOUNTB		4
#define LUA_GCSTEP		5
#define LUA_GCSETPAUSE		6
#define LUA_GCSETSTEPMUL	7
#define LUA_GCSETMAJORINC	8
#define LUA_GCISRUNNING		9
#define LUA_GCGEN		10
#define LUA_GCINC		11

typedef int (* Flua_gc) (lua_State *L, int what, int data);


/*
** miscellaneous functions
*/

typedef int       (* Flua_error) (lua_State *L);
typedef int       (* Flua_next) (lua_State *L, int idx);
typedef void      (* Flua_concat) (lua_State *L, int n);
typedef void      (* Flua_len)    (lua_State *L, int idx);
typedef lua_Alloc (* Flua_getallocf) (lua_State *L, void **ud);
typedef void      (* Flua_setallocf) (lua_State *L, lua_Alloc f, void *ud);



/*
** ===============================================================
** some useful macros
** ===============================================================
*/

/*
#define lua_tonumber(L,i)	lua_tonumberx(L,i,NULL)
#define lua_tointeger(L,i)	lua_tointegerx(L,i,NULL)
#define lua_tounsigned(L,i)	lua_tounsignedx(L,i,NULL)

#define lua_pop(L,n)		lua_settop(L, -(n)-1)

#define lua_newtable(L)		lua_createtable(L, 0, 0)

#define lua_register(L,n,f) (lua_pushcfunction(L, (f)), lua_setglobal(L, (n)))

#define lua_pushcfunction(L,f)	lua_pushcclosure(L, (f), 0)

#define lua_isfunction(L,n)	(lua_type(L, (n)) == LUA_TFUNCTION)
#define lua_istable(L,n)	(lua_type(L, (n)) == LUA_TTABLE)
#define lua_islightuserdata(L,n)	(lua_type(L, (n)) == LUA_TLIGHTUSERDATA)
#define lua_isnil(L,n)		(lua_type(L, (n)) == LUA_TNIL)
#define lua_isboolean(L,n)	(lua_type(L, (n)) == LUA_TBOOLEAN)
#define lua_isthread(L,n)	(lua_type(L, (n)) == LUA_TTHREAD)
#define lua_isnone(L,n)		(lua_type(L, (n)) == LUA_TNONE)
#define lua_isnoneornil(L, n)	(lua_type(L, (n)) <= 0)

#define lua_pushliteral(L, s)	lua_pushlstring(L, "" s, (sizeof(s)/sizeof(char))-1)

#define lua_pushglobaltable(L)  lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS)

#define lua_tostring(L,i)	lua_tolstring(L, (i), NULL)

#define lua_yield(L,n)		lua_yieldk(L, (n), 0, NULL)
#define lua_call(L,n,r)		lua_callk(L, (n), (r), 0, NULL)
#define lua_pcall(L,n,r,f)	lua_pcallk(L, (n), (r), (f), 0, NULL)
*/

/*
** {======================================================================
** Debug API
** =======================================================================
*/


/*
** Event codes
*/
#define LUA_HOOKCALL	0
#define LUA_HOOKRET	1
#define LUA_HOOKLINE	2
#define LUA_HOOKCOUNT	3
#define LUA_HOOKTAILCALL 4


/*
** Event masks
*/
#define LUA_MASKCALL	(1 << LUA_HOOKCALL)
#define LUA_MASKRET	(1 << LUA_HOOKRET)
#define LUA_MASKLINE	(1 << LUA_HOOKLINE)
#define LUA_MASKCOUNT	(1 << LUA_HOOKCOUNT)

typedef struct lua_Debug lua_Debug;  /* activation record */


/* Functions to be called by the debugger in specific events */
typedef void (*lua_Hook) (lua_State *L, lua_Debug *ar);


typedef int			(* Flua_getstack) (lua_State *L, int level, lua_Debug *ar);
typedef int			(* Flua_getinfo) (lua_State *L, const char *what, lua_Debug *ar);
typedef const char *(* Flua_getlocal) (lua_State *L, const lua_Debug *ar, int n);
typedef const char *(* Flua_setlocal) (lua_State *L, const lua_Debug *ar, int n);
typedef const char *(* Flua_getupvalue) (lua_State *L, int funcindex, int n);
typedef const char *(* Flua_setupvalue) (lua_State *L, int funcindex, int n);

typedef void *(* Flua_upvalueid) (lua_State *L, int fidx, int n);
typedef void  (* Flua_upvaluejoin) (lua_State *L, int fidx1, int n1,
                                               int fidx2, int n2);

typedef int		 (* Flua_sethook) (lua_State *L, lua_Hook func, int mask, int count);
typedef lua_Hook (* Flua_gethook) (lua_State *L);
typedef int	     (* Flua_gethookmask) (lua_State *L);
typedef int	     (* Flua_gethookcount) (lua_State *L);

#define LUA_IDSIZE	60

struct lua_Debug {
  int event;
  const char *name;	/* (n) */
  const char *namewhat;	/* (n) 'global', 'local', 'field', 'method' */
  const char *what;	/* (S) 'Lua', 'C', 'main', 'tail' */
  const char *source;	/* (S) */
  int currentline;	/* (l) */
  int linedefined;	/* (S) */
  int lastlinedefined;	/* (S) */
  unsigned char nups;	/* (u) number of upvalues */
  unsigned char nparams;/* (u) number of parameters */
  char isvararg;        /* (u) */
  char istailcall;	/* (t) */
  char short_src[LUA_IDSIZE]; /* (S) */
  /* private part */
  struct CallInfo *i_ci;  /* active function */
};

/* }====================================================================== */


/******************************************************************************
* Copyright (C) 1994-2013 Lua.org, PUC-Rio.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/


typedef void (* FLogPrintf)(const char * szFormat, ...);



struct FuncContainer_t
{
	Flua_absindex			LUA_absindex;
	Flua_arith				LUA_arith;
	Flua_atpanic			LUA_atpanic;
	Flua_callk				LUA_callk;
	Flua_checkstack			LUA_checkstack;
	Flua_close				LUA_close;
	Flua_compare			LUA_compare;
	Flua_concat				LUA_concat;
	Flua_copy				LUA_copy;
	Flua_createtable		LUA_createtable;
	Flua_dump				LUA_dump;
	Flua_error				LUA_error;
	Flua_gc					LUA_gc;
	Flua_getallocf			LUA_getallocf;
	Flua_getctx				LUA_getctx;
	Flua_getfield			LUA_getfield;
	Flua_getglobal			LUA_getglobal;
	Flua_gethook			LUA_gethook;
	Flua_gethookcount		LUA_gethookcount;
	Flua_gethookmask		LUA_gethookmask;
	Flua_getinfo			LUA_getinfo;
	Flua_getlocal			LUA_getlocal;
	Flua_getmetatable		LUA_getmetatable;
	Flua_getstack			LUA_getstack;
	Flua_gettable			LUA_gettable;
	Flua_gettop				LUA_gettop;
	Flua_getupvalue			LUA_getupvalue;
	Flua_getuservalue		LUA_getuservalue;
	Flua_insert				LUA_insert;
	Flua_iscfunction		LUA_iscfunction;
	Flua_isnumber			LUA_isnumber;
	Flua_isstring			LUA_isstring;
	Flua_isuserdata			LUA_isuserdata;
	Flua_len				LUA_len;
	Flua_load				LUA_load;
	Flua_newstate			LUA_newstate;
	Flua_newthread			LUA_newthread;
	Flua_newuserdata		LUA_newuserdata;
	Flua_next				LUA_next;
	Flua_pcallk				LUA_pcallk;
	Flua_pushboolean		LUA_pushboolean;
	Flua_pushcclosure		LUA_pushcclosure;
	Flua_pushfstring		LUA_pushfstring;
	Flua_pushinteger		LUA_pushinteger;
	Flua_pushlightuserdata	LUA_pushlightuserdata;
	Flua_pushlstring		LUA_pushlstring;
	Flua_pushnil			LUA_pushnil;
	Flua_pushnumber			LUA_pushnumber;
	Flua_pushstring			LUA_pushstring;
	Flua_pushthread			LUA_pushthread;
	Flua_pushunsigned		LUA_pushunsigned;
	Flua_pushvalue			LUA_pushvalue;
	Flua_pushvfstring		LUA_pushvfstring;
	Flua_rawequal			LUA_rawequal;
	Flua_rawget				LUA_rawget;
	Flua_rawgeti			LUA_rawgeti;
	Flua_rawgetp			LUA_rawgetp;
	Flua_rawlen				LUA_rawlen;
	Flua_rawset				LUA_rawset;
	Flua_rawseti			LUA_rawseti;
	Flua_rawsetp			LUA_rawsetp;
	Flua_remove				LUA_remove;
	Flua_replace			LUA_replace;
	Flua_resume				LUA_resume;
	Flua_setallocf			LUA_setallocf;
	Flua_setfield			LUA_setfield;
	Flua_setglobal			LUA_setglobal;
	Flua_sethook			LUA_sethook;
	Flua_setlocal			LUA_setlocal;
	Flua_setmetatable		LUA_setmetatable;
	Flua_settable			LUA_settable;
	Flua_settop				LUA_settop;
	Flua_setupvalue			LUA_setupvalue;
	Flua_setuservalue		LUA_setuservalue;
	Flua_status				LUA_status;
	Flua_toboolean			LUA_toboolean;
	Flua_tocfunction		LUA_tocfunction;
	Flua_tointegerx			LUA_tointegerx;
	Flua_tolstring			LUA_tolstring;
	Flua_tonumberx			LUA_tonumberx;
	Flua_topointer			LUA_topointer;
	Flua_tothread			LUA_tothread;
	Flua_tounsignedx		LUA_tounsignedx;
	Flua_touserdata			LUA_touserdata;
	Flua_type				LUA_type;
	Flua_typename			LUA_typename;
	Flua_upvalueid			LUA_upvalueid;
	Flua_upvaluejoin		LUA_upvaluejoin;
	Flua_version			LUA_version;
	Flua_xmove				LUA_xmove;
	Flua_yieldk				LUA_yieldk;
	FLogPrintf				Printf;
};

#define lua_absindex			FuncContainer.LUA_absindex
#define lua_arith				FuncContainer.LUA_arith
#define lua_atpanic				FuncContainer.LUA_atpanic
#define lua_callk				FuncContainer.LUA_callk
#define lua_checkstack			FuncContainer.LUA_checkstack
#define lua_close				FuncContainer.LUA_close
#define lua_compare				FuncContainer.LUA_compare
#define lua_concat				FuncContainer.LUA_concat
#define lua_copy				FuncContainer.LUA_copy
#define lua_createtable			FuncContainer.LUA_createtable
#define lua_dump				FuncContainer.LUA_dump
#define lua_error				FuncContainer.LUA_error
#define lua_gc					FuncContainer.LUA_gc
#define lua_getallocf			FuncContainer.LUA_getallocf
#define lua_getctx				FuncContainer.LUA_getctx
#define lua_getfield			FuncContainer.LUA_getfield
#define lua_getglobal			FuncContainer.LUA_getglobal
#define lua_gethook				FuncContainer.LUA_gethook
#define lua_gethookcount		FuncContainer.LUA_gethookcount
#define lua_gethookmask			FuncContainer.LUA_gethookmask
#define lua_getinfo				FuncContainer.LUA_getinfo
#define lua_getlocal			FuncContainer.LUA_getlocal
#define lua_getmetatable		FuncContainer.LUA_getmetatable
#define lua_getstack			FuncContainer.LUA_getstack
#define lua_gettable			FuncContainer.LUA_gettable
#define lua_gettop				FuncContainer.LUA_gettop
#define lua_getupvalue			FuncContainer.LUA_getupvalue
#define lua_getuservalue		FuncContainer.LUA_getuservalue
#define lua_insert				FuncContainer.LUA_insert
#define lua_iscfunction			FuncContainer.LUA_iscfunction
#define lua_isnumber			FuncContainer.LUA_isnumber
#define lua_isstring			FuncContainer.LUA_isstring
#define lua_isuserdata			FuncContainer.LUA_isuserdata
#define lua_len					FuncContainer.LUA_len
#define lua_load				FuncContainer.LUA_load
#define lua_newstate			FuncContainer.LUA_newstate
#define lua_newthread			FuncContainer.LUA_newthread
#define lua_newuserdata			FuncContainer.LUA_newuserdata
#define lua_next				FuncContainer.LUA_next
#define lua_pcallk				FuncContainer.LUA_pcallk
#define lua_pushboolean			FuncContainer.LUA_pushboolean
#define lua_pushcclosure		FuncContainer.LUA_pushcclosure
#define lua_pushfstring			FuncContainer.LUA_pushfstring
#define lua_pushinteger			FuncContainer.LUA_pushinteger
#define lua_pushlightuserdata	FuncContainer.LUA_pushlightuserdata
#define lua_pushlstring			FuncContainer.LUA_pushlstring
#define lua_pushnil				FuncContainer.LUA_pushnil
#define lua_pushnumber			FuncContainer.LUA_pushnumber
#define lua_pushstring			FuncContainer.LUA_pushstring
#define lua_pushthread			FuncContainer.LUA_pushthread
#define lua_pushunsigned		FuncContainer.LUA_pushunsigned
#define lua_pushvalue			FuncContainer.LUA_pushvalue
#define lua_pushvfstring		FuncContainer.LUA_pushvfstring
#define lua_rawequal			FuncContainer.LUA_rawequal
#define lua_rawget				FuncContainer.LUA_rawget
#define lua_rawgeti				FuncContainer.LUA_rawgeti
#define lua_rawgetp				FuncContainer.LUA_rawgetp
#define lua_rawlen				FuncContainer.LUA_rawlen
#define lua_rawset				FuncContainer.LUA_rawset
#define lua_rawseti				FuncContainer.LUA_rawseti
#define lua_rawsetp				FuncContainer.LUA_rawsetp
#define lua_remove				FuncContainer.LUA_remove
#define lua_replace				FuncContainer.LUA_replace
#define lua_resume				FuncContainer.LUA_resume
#define lua_setallocf			FuncContainer.LUA_setallocf
#define lua_setfield			FuncContainer.LUA_setfield
#define lua_setglobal			FuncContainer.LUA_setglobal
#define lua_sethook				FuncContainer.LUA_sethook
#define lua_setlocal			FuncContainer.LUA_setlocal
#define lua_setmetatable		FuncContainer.LUA_setmetatable
#define lua_settable			FuncContainer.LUA_settable
#define lua_settop				FuncContainer.LUA_settop
#define lua_setupvalue			FuncContainer.LUA_setupvalue
#define lua_setuservalue		FuncContainer.LUA_setuservalue
#define lua_status				FuncContainer.LUA_status
#define lua_toboolean			FuncContainer.LUA_toboolean
#define lua_tocfunction			FuncContainer.LUA_tocfunction
#define lua_tointegerx			FuncContainer.LUA_tointegerx
#define lua_tolstring			FuncContainer.LUA_tolstring
#define lua_tonumberx			FuncContainer.LUA_tonumberx
#define lua_topointer			FuncContainer.LUA_topointer
#define lua_tothread			FuncContainer.LUA_tothread
#define lua_tounsignedx			FuncContainer.LUA_tounsignedx
#define lua_touserdata			FuncContainer.LUA_touserdata
#define lua_type				FuncContainer.LUA_type
#define lua_typename			FuncContainer.LUA_typename
#define lua_upvalueid			FuncContainer.LUA_upvalueid
#define lua_upvaluejoin			FuncContainer.LUA_upvaluejoin
#define lua_version				FuncContainer.LUA_version
#define lua_xmove				FuncContainer.LUA_xmove
#define lua_yieldk				FuncContainer.LUA_yieldk
#define LogPrintf				FuncContainer.Printf


#define lua_call(L,n,r)			FuncContainer.LUA_callk(L, (n), (r), 0, NULL)
#define lua_pcall(L,n,r,f)		FuncContainer.LUA_pcallk(L, (n), (r), (f), 0, NULL)
#define lua_yield(L,n)			FuncContainer.LUA_yieldk(L, (n), 0, NULL)

#define lua_tonumber(L,i)		(float)FuncContainer.LUA_tonumberx(L,i,NULL)
#define lua_tointeger(L,i)		FuncContainer.LUA_tointegerx(L,i,NULL)
#define lua_tounsigned(L,i)		FuncContainer.LUA_tounsignedx(L,i,NULL)

#define lua_pop(L,n)			FuncContainer.LUA_settop(L, -(n)-1)

#define lua_newtable(L)			FuncContainer.LUA_createtable(L, 0, 0)


#define lua_pushcfunction(L,f)	FuncContainer.LUA_pushcclosure(L, (f), 0)

#define lua_register(L,n,f)		(lua_pushcfunction(L, (f)), FuncContainer.LUA_setglobal(L, (n)))


#define lua_isfunction(L,n)		(FuncContainer.LUA_type(L, (n)) == LUA_TFUNCTION)
#define lua_istable(L,n)		(FuncContainer.LUA_type(L, (n)) == LUA_TTABLE)
#define lua_islightuserdata(L,n)(FuncContainer.LUA_type(L, (n)) == LUA_TLIGHTUSERDATA)
#define lua_isnil(L,n)			(FuncContainer.LUA_type(L, (n)) == LUA_TNIL)
#define lua_isboolean(L,n)		(FuncContainer.LUA_type(L, (n)) == LUA_TBOOLEAN)
#define lua_isthread(L,n)		(FuncContainer.LUA_type(L, (n)) == LUA_TTHREAD)
#define lua_isnone(L,n)			(FuncContainer.LUA_type(L, (n)) == LUA_TNONE)
#define lua_isnoneornil(L, n)	(FuncContainer.LUA_type(L, (n)) <= 0)

#define lua_pushliteral(L, s)	FuncContainer.LUA_pushlstring(L, "" s, (sizeof(s)/sizeof(char))-1)

#define lua_pushglobaltable(L)  FuncContainer.LUA_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS)

#define lua_tostring(L,i)		FuncContainer.LUA_tolstring(L, (i), NULL)*/


}