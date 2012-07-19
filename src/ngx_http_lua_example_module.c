#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_http_lua_api.h"

ngx_module_t ngx_http_lua_example_module;

static ngx_int_t
ngx_http_lua_example_init(ngx_conf_t *cf);

static ngx_http_module_t ngx_http_lua_example_ctx = {
    NULL, /* preconfiguration */
    ngx_http_lua_example_init, /* postconfiguration */
    NULL, /* create main configuration */
    NULL, /* init main configuration */
    NULL, /* create server configuration */
    NULL, /* merge server configuration */
    NULL, /* create location configuration */
    NULL /* merge location configuration */
};

ngx_module_t ngx_http_lua_example_module = {
    NGX_MODULE_V1,
    &ngx_http_lua_example_ctx, /* module context */
    NULL, /* module directives */
    NGX_HTTP_MODULE, /* module type */
    NULL, /* init master */
    NULL, /* init module */
    NULL, /* init process */
    NULL, /* init thread */
    NULL, /* exit thread */
    NULL, /* exit process */
    NULL, /* exit master */
    NGX_MODULE_V1_PADDING
};

static int get_uri(lua_State * L)
{
    ngx_http_request_t *r;

    r = ngx_http_lua_get_request(L);
    lua_pushlstring(L, (const char *)r->uri.data, r->uri.len);

    return 1;
}

static int get_host(lua_State * L)
{
    ngx_http_request_t *r;

    r = ngx_http_lua_get_request(L);

    if (r->headers_in.host && r->headers_in.host->value.data) {
        lua_pushlstring(L, (const char *)r->headers_in.host->value.data, r->headers_in.host->value.len);
    } else {
        lua_pushnil(L);
    }

    return 1;
}

static int luaopen_nginx_example(lua_State * L)
{
    lua_createtable(L, 0, 1);
    lua_pushcfunction(L, get_uri);
    lua_setfield(L, -2, "get_uri");
    lua_pushcfunction(L, get_host);
    lua_setfield(L, -2, "get_host");
    return 1;
}

static ngx_int_t
ngx_http_lua_example_init(ngx_conf_t *cf)
{
    ngx_http_lua_add_package_preload(cf, "nginx.example", luaopen_nginx_example);
    return NGX_OK;
}
