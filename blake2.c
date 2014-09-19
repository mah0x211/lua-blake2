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
 *  Created by Masatoshi Teruya on 13/10/02.
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


#define b2_lua( L, fn, maxklen, maxolen ) do { \
    int argc = lua_gettop( L ); \
    uint8_t out[maxolen] = {0}; \
    size_t ilen = 0; \
    const char *in = NULL; \
    size_t klen = 0; \
    const char *key = NULL; \
    int olen = maxolen; \
    if( argc > 3 ){ argc = 3; } \
    \
    in = luaL_checklstring( L, 1, &ilen ); \
    switch( argc ){ \
        case 3: \
            if( !lua_isnoneornil( L, 3 ) ){ \
                olen = luaL_checkint( L, 3 ); \
                luaL_argcheck( L, olen > 0 && olen <= maxolen, 3, \
                               "output length must be 1 to " #maxolen " bytes" ); \
            } \
        case 2: \
            if( !lua_isnoneornil( L, 2 ) ){ \
                key = luaL_checklstring( L, 2, &klen ); \
                luaL_argcheck( L, klen <= maxklen, 2, \
                               "key length must be less than " #maxklen " bytes" ); \
            } \
        break; \
    } \
    if( fn( out, in, key, (size_t)olen, ilen, klen ) ){ \
        lua_pushnil( L ); \
        lua_pushstring( L, "failed to " #fn ); \
        return 2; \
    } \
    push2hex( L, out, olen ); \
    return 1; \
}while(0)


static int b2s_lua( lua_State *L ){
    b2_lua( L, blake2s, 32, 32 );
}

static int b2sp_lua( lua_State *L ){
    b2_lua( L, blake2sp, 32, 32 );
}

static int b2b_lua( lua_State *L ){
    b2_lua( L, blake2b, 64, 64 );
}

static int b2bp_lua( lua_State *L ){
    b2_lua( L, blake2bp, 64, 64 );
}


LUALIB_API int luaopen_blake2( lua_State *L )
{
    struct luaL_Reg method[] = {
        { "s", b2s_lua },
        { "sp", b2sp_lua },
        { "b", b2b_lua },
        { "bp", b2bp_lua },
        { NULL, NULL }
    };
    struct luaL_Reg *ptr = method;

    lua_newtable( L );
    do {
        lua_pushstring( L, ptr->name );
        lua_pushcfunction( L, ptr->func );
        lua_rawset( L, -3 );
        ptr++;
    } while( ptr->name );

    return 1;
}

