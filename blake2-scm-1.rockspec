package = "blake2"
version = "scm-1"
source = {
    url = "git://github.com/mah0x211/lua-blake2.git"
}
description = {
    summary = "blake2 binding for lua",
    detailed = [[]],
    homepage = "https://github.com/mah0x211/lua-blake2",
    license = "MIT/X11",
    maintainer = "Masatoshi Teruya"
}
dependencies = {
    "lua >= 5.1"
}
external_dependencies = {
    BLAKE2 = {
        header = "blake2.h",
        library = "b2"
    }
}
build = {
    type = "builtin",
    modules = {
        blake2 = {
            sources = { "blake2.c" },
            libraries = { "b2" },
            incdirs = { 
                "$(BLAKE2_INCDIR)"
            },
            libdirs = { 
                "$(BLAKE2_LIBDIR)"
            }
        }
    }
}

