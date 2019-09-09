//
//  NAPI_Types.hpp
//  NodeJSEmbed
//
//  Created by Luis Finke on 9/9/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

struct napi_env__;
typedef struct napi_env__* napi_env;

struct napi_value__;
typedef struct napi_value__* napi_value;

struct napi_ref__;
typedef struct napi_ref__* napi_ref;

struct napi_handle_scope__;
typedef struct napi_handle_scope__* napi_handle_scope;

struct napi_escapable_handle_scope__;
typedef struct napi_escapable_handle_scope__* napi_escapable_handle_scope;

struct napi_callback_info__;
typedef struct napi_callback_info__* napi_callback_info;

struct napi_deferred__;
typedef struct napi_deferred__* napi_deferred;
