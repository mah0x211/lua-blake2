blake2 = require('blake2');

local data = "example data";
local key = "example key";

print(  
    '\n',
    'b2s\n',
    'f4564765f92bbcd5ab589b982b527f02db39ea8b4188b22e16b300e68b303583',
    '\n',
    blake2.blake2s( data, key ),
    '\n',
    '\n',
    'b2sp\n',
    'a79b5075f6e28f9ce96c24666bb042bc3349f7f1dcf5befd09717cad4f8847bb',
    '\n',
    blake2.blake2sp( data, key ),
    '\n',
    '\n',
    'b2b\n',
    '2c87a41f3d3775c42fff183c6616d1f47ddc3eb6511e91b93a02694df446172a8ac38178eef99a23256237d1b0d324d6cbaa4fce33b2acc0270076ecdbb796a3',
    '\n',
    blake2.blake2b( data, key ),
    '\n',
    '\n',
    'b2bp\n',
    '7ca4584a051375fcd8bd28a0aa000db5bd807a901dc7216ecf3556feed3aaf7ef30cedd8e69fc3428e41bf8a9dd8e85a8b0c5fc1a85ab7610607b7894cdeba08',
    '\n',
    blake2.blake2bp( data, key ) 
);
