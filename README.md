# lua-blake2
BLAKE2 lua binding.


## Dependencies

- libb2-0.9 - https://blake2.net/


## Functions

### blake2s / blake2sp / blake2b / blake2bp ( data, key )

returns a hexadecimal string of BLAKE2 hash encoded value .

**Parameters**

- data: target string.
- key: hash key string.

**Returns**

1. hash: hexadecimal string.

please refer to [blake2.net](https://blake2.net/) for more information.

