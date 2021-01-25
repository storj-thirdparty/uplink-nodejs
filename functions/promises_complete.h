#include "definitions.h"
#include "../libUplink.h"
#include "download_operations.h"
#include "upload_operations.h"
#include "bucket_operations.h"
#include "object_operations.h"

void openProjectPromiseComplete(napi_env env, napi_status status, void* data);

void listObjectPromiseComplete(napi_env env, napi_status status, void* data);

void downloadInfoOperationComplete(napi_env env, napi_status status, void* data);

void downloadCloseOperationComplete(napi_env env, napi_status status, void* data);

void downloadReadOperationComplete(napi_env env, napi_status status, void* data);

void downloadObjectOperationComplete(napi_env env, napi_status status, void* data);

void uploadSetMetaPromiseComplete(napi_env env, napi_status status, void* data);

void uploadAbortPromiseComplete(napi_env env, napi_status status, void* data);

void uploadInfoOperationComplete(napi_env env, napi_status status, void* data);

void uploadCommitOperationComplete(napi_env env, napi_status status, void* data);

void uploadWriteOperationComplete(napi_env env, napi_status status, void* data);

void uploadObjectComplete(napi_env env, napi_status status, void* data);

void objectOperationComplete(napi_env env, napi_status status, void* data);

void bucketOperationComplete(napi_env env, napi_status status, void* data);

void ListBucketsPromiseComplete(napi_env env, napi_status status, void* data);

void closeProjectPromiseComplete(napi_env env, napi_status status, void* data);

void configOpenProjectPromiseComplete(napi_env env, napi_status status, void* data);

void ParseAccessPromiseComplete(napi_env env, napi_status status, void* data);

void ShareAccessPromiseComplete(napi_env env, napi_status status, void* data);

void ConfigRequestAccessWithEncryptionPromiseComplete(napi_env env, napi_status status, void* data);

void RequestAccessWithEncryptionPromiseComplete(napi_env env, napi_status status, void* data);

void accessSerializePromiseComplete(napi_env env, napi_status status, void* data);

void deriveEncrpPromiseComplete(napi_env env, napi_status status, void* data);

void accessOverRidePromiseComplete(napi_env env, napi_status status, void* data);