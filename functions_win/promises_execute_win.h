#include "definitions_win.h"

void loaddll();

void openProjectPromiseExecute(napi_env env, void* data);

void ListObjectsPromiseExecute(napi_env env, void* data);

void downloadInfoPromiseExecute(napi_env env, void* data);

void downloadClosePromiseExecute(napi_env env, void* data);

void downloadReadPromiseExecute(napi_env env, void* data);

void downloadObjectPromiseExecute(napi_env env, void* data);

void uploadSetMetaPromiseExecute(napi_env env, void* data);

void uploadAbortPromiseExecute(napi_env env, void* data);

void uploadInfoPromiseExecute(napi_env env, void* data);

void uploadCommitPromiseExecute(napi_env env, void* data);

void uploadWritePromiseExecute(napi_env env, void* data);

void uploadObjectExecute(napi_env env, void* data);

void stateObjectPromiseExecute(napi_env env, void* data);
//
void deleteObjectPromiseExecute(napi_env env, void* data);

void stateBucketPromiseExecute(napi_env env, void* data);
//
void createBucketPromiseExecute(napi_env env, void* data);
//
void ensureBucketPromiseExecute(napi_env env, void* data);
//
void deleteBucketPromiseExecute(napi_env env, void* data);

void ListBucketsPromiseExecute(napi_env env, void* data);

void closeProjectPromiseExecute(napi_env env, void* data);

void configOpenProjectPromiseExecute(napi_env env, void* data);

void ParseAccess(napi_env env, void* data);

void ShareAccessPromiseExecute(napi_env env, void* data);

void ConfigRequestAccessWithEncryption(napi_env env, void* data);

void RequestAccessWithEncryption(napi_env env, void* data);

void accessSerializePromiseExecute(napi_env env, void* data);

void deriveEncrpPromiseExecute(napi_env env, void* data);

void accessOverRidePromiseExecute(napi_env env, void* data);