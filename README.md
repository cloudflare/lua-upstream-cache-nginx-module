lua-upstream-cache
==================

**http_cache** - Expose & modify the internal nginx cache metadata.

_This module is not distributed with nginx source or in the lua-nginx-module._

[![Build Status](https://travis-ci.org/alticelabs/lua-upstream-cache-nginx-module.svg?branch=master)](https://travis-ci.org/alticelabs/lua-upstream-cache-nginx-module)

Status
======

This module is under development and is used in production.

Version
=======

This document describes http_cache v0.1.1

Description
===========

This module extends the lua-nginx Lua module. It adds the ability to read
and set values from nginx's internal cache metadata.

Synopsis
========
```
        # set search paths for pure Lua external libraries (';;' is the default path):
        lua_package_path '/foo/bar/?.lua;/blah/?.lua;;';

        # set search paths for Lua external libraries written in C (can also use ';;'):
        lua_package_cpath '/bar/baz/?.so;/blah/blah/?.so;;';

        server {
            listen       8000 default_server;
            server_name  localhost;

        location / {
            proxy_cache pcache;
            proxy_pass http://127.0.0.1:8001/;

            # Force entries in cache to only be cached for 5 seconds
            # This is equivalent to:
            # proxy_ignore_headers "Expires" "Cache-Control;
            # proxy_cache_valid any 5s;
            header_filter_by_lua '
               local http_cache = require("http_cache")
               local cache_status = (ngx.var.upstream_cache_status or "")

               if cache_status == "MISS" or cache_status == "EXPIRED" then
                   local cache_data = http_cache.get_metadata()
                   local new_expire = ngx.time() + 5

                   if cache_data and cache_data["valid_sec"] then
                       http_cache.set_metadata({ valid_sec = new_expire,
                                                 fcn = { valid_sec = new_expire,
                                                     expire = new_expire } })
                   end
               end
            ';
        }
```
API
===

get_metadata
------------
**get_metadata()**: returns a table with all cache metadata.

_resource specific fields_

* **key**: Hexadecimal cache key for this resource
* **crc32**: Numeric crc32 calculated by nginx to check cache collisions
* **valid_sec**: Absolute time, in seconds since Epoch, when this entry expires.
* **last_modified**: Absolute time of last-modified header in seconds, if present.
* **date**: Absolute time of date header in seconds, if present.
* **length**: Length of contents
* **fs_size**: Size of resource, in disk blocks. (NOTE: This can be converted to bytes by multiplying by bsize below.)
* **min_uses**: Minimum number of uses required to cache this resource.
* **valid_msec**: millisecond component of valid_secs (?)

_shared cache information, under "sh"_

* **size**

_cache data, under "fcn"_

* **max_size**: maximum allowable cache size in 512B blocks
* **bsize**: size of disk block in bytes.
* **inactive**: inactive value for cache.
* **files**: currently open files used by loader
* **loader_files**: maximum number of files for the loader to open at once
* **loader_sleep**: time between file opens
* **loader_threshold**:
* **aggressive_purge**: Flag for whether cache manager will use aggressive purge policy

set_metadata
------------
**set_metadata(tbl)**: sets cache metadata from a table similar to the one returned by get_metadata.

_resource specific fields_

* **valid_sec**
* **last_modified**
* **date**
* **min_uses**
* **valid_msec**

_cache data, under "fcn"_

* **uses**
* **valid_msec**
* **expire**
* **valid_sec**

set_uses
--------
**set_uses(number)**: force the number of uses for the cache reference

set_expires
-----------
**set_expires(timestamp)**: for the time to expires
