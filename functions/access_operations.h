#include "definitions.h"
#include "../libUplink.h"

napi_value parse_accessc(napi_env env, napi_callback_info info);
//
napi_value access_sharec(napi_env env, napi_callback_info info);
//
napi_value config_request_access_with_passphrasec(napi_env env, napi_callback_info info);
//
napi_value request_access_with_passphrasec(napi_env env, napi_callback_info info);

napi_value access_serializec(napi_env env, napi_callback_info info);
//
napi_value uplink_derive_encryption_keyc(napi_env env, napi_callback_info info);
//
napi_value uplink_access_override_encryption_keyc(napi_env env, napi_callback_info info);