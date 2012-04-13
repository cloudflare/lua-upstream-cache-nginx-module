#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_http_lua_util.h"

ngx_module_t ngx_http_lua_test_module;

static ngx_int_t
ngx_http_lua_test_init(ngx_conf_t *cf);

static ngx_http_module_t ngx_http_lua_test_ctx = {
    NULL, /* preconfiguration */
    ngx_http_lua_test_init, /* postconfiguration */
    NULL, /* create main configuration */
    NULL, /* init main configuration */
    NULL, /* create server configuration */
    NULL, /* merge server configuration */
    NULL, /* create location configuration */
    NULL /* merge location configuration */
};

ngx_module_t ngx_http_lua_test_module = {
    NGX_MODULE_V1,
    &ngx_http_lua_test_ctx, /* module context */
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
    lua_getglobal(L, GLOBALS_SYMBOL_REQUEST);
    r = lua_touserdata(L, -1);
    lua_pop(L, 1);
    lua_pushlstring(L, r->uri.data, r->uri.len);
    return 1;
}

static int luaopen_nginx_test(lua_State * L)
{
    lua_createtable(L, 0, 1);
    lua_pushcfunction(L, get_uri);
    lua_setfield(L, -2, "get_uri");
    return 1;
}

static ngx_int_t
ngx_http_lua_test_init(ngx_conf_t *cf)
{
    ngx_http_lua_add_preload(cf, "nginx.test", luaopen_nginx_test);
    return NGX_OK;
}
