# lua-blake2
BLAKE2 lua binding.


## Dependencies

- libb2-0.9 - https://blake2.net/

## Installation

```sh
luarocks install --from=http://mah0x211.github.io/rocks/ blake2
```

or 

```sh
git clone https://github.com/mah0x211/lua-blake2.git
cd lua-blake2
luarocks make
```


## Functions

### blake2.<s / sp / b / bp>( data [, key [, digest_length] ] )

returns a hexadecimal string of BLAKE2 hash encoded value .

**Parameters**

- data: target string.
- key: hash key string. (optional)
- digest_length: value range should be 1-32(s / sp) or 1-64(b / bp). (optional)

**Returns**

1. hash: hexadecimal string.

please refer to [blake2.net](https://blake2.net/) for more information.

