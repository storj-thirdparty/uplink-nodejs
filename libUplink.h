#ifndef LIBUPLINK_H
#define LIBUPLINK_H


#include "functions/definitions.h"
#include "functions/promises_complete.h"
#include "functions/promises_execute.h"
#include "functions/project_operations.h"
#include "functions/access_operations.h"

void reverse(char str[], int length);

char* itoa(int num, char* str, int base);

// function creates NAPI type error object
napi_value createError(napi_env env, int64_t accessError, char* errorMessagePtr);
//
int64_t getHandleValue(napi_env env, napi_value handleobj);
//
napi_value createResult(napi_env env, string resultType, int64_t handlevalue);
//
napi_value createBucketResult(napi_env env, string resultType, int64_t bucketCreated, char* bucketNamePtr);
//
napi_value createObjectResult(napi_env env, UplinkObject* objectPtr);
//
napi_value list_objectsc(napi_env env, napi_callback_info info);
//
napi_value list_bucketsc(napi_env env, napi_callback_info info);
//
#define DECLARE_NAPI_METHOD(name, func)                                       \
  {name, 0, func, 0, 0, 0, napi_default, 0 }

#endif //LIBUPLINK_H