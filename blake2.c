/*
 *  Copyright (C) 2013 Masatoshi Teruya
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */
/*
 *  blake2_bind.c
 *  Created by Masatoshi Teruya on 13/05/17.
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <lauxlib.h>
#include <lualib.h>
#include <blake2.h>

static const char HEX_CHR[] = "0123456789abcdef";

#define push2hex(L,result,len) do { \
    char digest[len*2]; \
    char *hd = digest; \
    int i = 0; \
	hd = digest; \
	for(; i < len; i++ ){ \
		*hd++ = HEX_CHR[result[i] >> 4]; \
		*hd++ = HEX_CHR[result[i] & 0xf]; \
	} \
	*hd = 0; \
    lua_pushstring( L, digest ); \
}while(0)

#define gethash(fn,o,i,k,ol,il,kl)  (fn(o,i,k,ol,il,kl))

#define push2s(L,fn)({ \
    size_t inlen = 0; \
    const char *in = luaL_checklstring( L, 1, &inlen ); \
    size_t keylen = 0; \
    const char *key = luaL_checklstring( L, 2, &keylen ); \
    uint8_t out[BLAKE2S_OUTBYTES]; \
    \
    luaL_argcheck( L, keylen < BLAKE2S_KEYBYTES, 2, \
                  #fn " key must be less than 32 bytes" ); \
    \
    if( gethash( fn, out, in, key, BLAKE2S_OUTBYTES, inlen, keylen ) ){ \
        luaL_error( L, "failed to " #fn ": %s", strerror(errno) ); \
    } \
    else { \
        push2hex( L, out, BLAKE2S_OUTBYTES ); \
    } \
    1; \
})

#define push2b(L,fn)({ \
    size_t inlen = 0; \
    const char *in = luaL_checklstring( L, 1, &inlen ); \
    size_t keylen = 0; \
    const char *key = luaL_checklstring( L, 2, &keylen ); \
    uint8_t out[BLAKE2B_OUTBYTES]; \
    \
    luaL_argcheck( L, keylen < BLAKE2B_KEYBYTES, 2, \
                  #fn " key must be less than 32 bytes" ); \
    \
    if( gethash( fn, out, in, key, BLAKE2B_OUTBYTES, inlen, keylen ) ){ \
        luaL_error( L, "failed to " #fn ": %s", strerror(errno) ); \
    } \
    else { \
        push2hex( L, out, BLAKE2B_OUTBYTES ); \
    } \
    1; \
})

static int blake2s_lua( lua_State *L ){
    return push2s( L, blake2s );
}

static int blake2sp_lua( lua_State *L ){
    return push2s( L, blake2sp );
}

static int blake2b_lua( lua_State *L ){
    return push2b( L, blake2b );
}

static int blake2bp_lua( lua_State *L ){
    return push2b( L, blake2bp );
}

// make error
static int const_newindex( lua_State *L ){
    return luaL_error( L, "attempting to change protected module" );
}

LUALIB_API int luaopen_blake2( lua_State *L )
{
    struct luaL_Reg funcs[] = {
        { "blake2s", blake2s_lua },
        { "blake2sp", blake2sp_lua },
        { "blake2b", blake2b_lua },
        { "blake2bp", blake2bp_lua },
        { NULL, NULL }
    };
    int i = 0;
    
    // create protected-table
    lua_newtable( L );
    // create __metatable
    lua_newtable( L );
    // create substance
    lua_pushstring( L, "__index" );
    lua_newtable( L );
    
    // set functions
    for( i = 0; funcs[i].name; i++ ){ 
        lua_pushstring( L, funcs[i].name );
        lua_pushcfunction( L, funcs[i].func );
        lua_rawset( L, -3 );
    }
    
    // set substance to __metable.__index field
    lua_rawset( L, -3 );
    // set __newindex function to __metable.__newindex filed
    lua_pushstring( L, "__newindex" );
    lua_pushcfunction( L, const_newindex );
    lua_rawset( L, -3 );
    // convert protected-table to metatable
    lua_setmetatable( L, -2 );
    
    return 1;
}

