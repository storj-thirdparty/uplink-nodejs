#include "bucket_operations_win.h"
#include <string>

napi_value stat_bucketc(napi_env env, napi_callback_info info) {
  napi_status status;
  size_t argc = 2;
  napi_value args[2];
  napi_value promise;

  bucketOperationObj *obj = (bucketOperationObj *)
  malloc(sizeof(bucketOperationObj));
  if (obj == NULL) {
      free(obj);
    napi_throw_error(env, NULL, "Memory allocation error");
    return NULL;
  }
  obj->async_action_status = 1;
  status = napi_get_cb_info(env, info, &argc, args, nullptr , nullptr);
  assert(status == napi_ok);
  //
  status = napi_create_promise(env, &obj->deferred, &promise);
  if (status != napi_ok) {
      free(obj);
    napi_throw_error(env, NULL, "Unable to create promise");
    return NULL;
  }
  //

  if (argc < 2) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong number of arguments!! excepted 2 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env, args[0], &checktypeofinput);
  assert(status == napi_ok);

  if (checktypeofinput != napi_object) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env, args[1], &checktypeofinput);
  assert(status == napi_ok);

  if (checktypeofinput != napi_string) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! Second argument excepted to be string type\n");
    return NULL;
  }

  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1], NULL,
    bufsize, &convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;
  char* bucketName =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env, args[1],
    bucketName, convertedvalue, &bufsize);
  assert(status == napi_ok);

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  string handle = "_handle";
  status = napi_create_string_utf8(env,
    const_cast<char* > (handle.c_str()), NAPI_AUTO_LENGTH , &ObjectkeyNAPI);
  assert(status == napi_ok);

  status = napi_has_property(env, args[0],
    ObjectkeyNAPI, &propertyexists);
  assert(status == napi_ok);
  if (!propertyexists) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nInvalid Object \n");
    return NULL;
  }

  UplinkProject project_result;
  project_result._handle = getHandleValue(env, args[0]);
  if (project_result._handle == 0) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Object \n");
    return NULL;
  }
  obj->project = project_result;
  obj->bucketname = bucketName;
  napi_value resource_name;
  napi_create_string_utf8(env, "stateBucket",
  NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name, stateBucketPromiseExecute,
  bucketOperationComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}
//
napi_value create_bucketc(napi_env env, napi_callback_info info) {
  napi_status status;
  size_t argc = 2;
  napi_value args[2];
  napi_value promise;

  bucketOperationObj *obj = (bucketOperationObj *)
  malloc(sizeof(bucketOperationObj));
  if (obj == NULL) {
      free(obj);
    napi_throw_error(env, NULL, "Memory allocation error");
    return NULL;
  }
  obj->async_action_status = 1;
  status = napi_get_cb_info(env, info, &argc, args, nullptr , nullptr);
  assert(status == napi_ok);
  //
  status = napi_create_promise(env, &obj->deferred, &promise);
  if (status != napi_ok) {
      free(obj);
    napi_throw_error(env, NULL, "Unable to create promise");
    return NULL;
  }
  //

  if (argc < 2) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong number of arguments!! excepted 2 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env, args[0], &checktypeofinput);
  assert(status == napi_ok);

  if (checktypeofinput != napi_object) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env, args[1], &checktypeofinput);
  assert(status == napi_ok);

  if (checktypeofinput != napi_string) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! Second argument excepted to be string type\n");
    return NULL;
  }

  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1], NULL,
    bufsize, &convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;
  char* bucketName =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env, args[1],
    bucketName, convertedvalue, &bufsize);
  assert(status == napi_ok);

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  string handle = "_handle";
  status = napi_create_string_utf8(env,
    const_cast<char* > (handle.c_str()), NAPI_AUTO_LENGTH , &ObjectkeyNAPI);
  assert(status == napi_ok);

  status = napi_has_property(env, args[0],
    ObjectkeyNAPI, &propertyexists);
  assert(status == napi_ok);
  if (!propertyexists) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nInvalid Object \n");
    return NULL;
  }

  UplinkProject project_result;
  project_result._handle = getHandleValue(env, args[0]);
  if (project_result._handle == 0) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Object \n");
    return NULL;
  }
  obj->project = project_result;
  obj->bucketname = bucketName;
  napi_value resource_name;
  napi_create_string_utf8(env, "createBucket",
  NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name, createBucketPromiseExecute,
  bucketOperationComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}
//
napi_value ensure_bucketc(napi_env env, napi_callback_info info) {
  napi_status status;
  size_t argc = 2;
  napi_value args[2];
  napi_value promise;

  bucketOperationObj *obj = (bucketOperationObj *)
  malloc(sizeof(bucketOperationObj));
  if (obj == NULL) {
      free(obj);
    napi_throw_error(env, NULL, "Memory allocation error");
    return NULL;
  }
  obj->async_action_status = 1;
  status = napi_get_cb_info(env, info, &argc, args, nullptr , nullptr);
  assert(status == napi_ok);
  //
  status = napi_create_promise(env, &obj->deferred, &promise);
  if (status != napi_ok) {
      free(obj);
    napi_throw_error(env, NULL, "Unable to create promise");
    return NULL;
  }
  //

  if (argc < 2) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong number of arguments!! excepted 2 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env, args[0], &checktypeofinput);
  assert(status == napi_ok);

  if (checktypeofinput != napi_object) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env, args[1], &checktypeofinput);
  assert(status == napi_ok);

  if (checktypeofinput != napi_string) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! Second argument excepted to be string type\n");
    return NULL;
  }

  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1], NULL,
    bufsize, &convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;
  char* bucketName =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env, args[1],
    bucketName, convertedvalue, &bufsize);
  assert(status == napi_ok);

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  string handle = "_handle";
  status = napi_create_string_utf8(env,
    const_cast<char* > (handle.c_str()), NAPI_AUTO_LENGTH , &ObjectkeyNAPI);
  assert(status == napi_ok);

  status = napi_has_property(env, args[0],
    ObjectkeyNAPI, &propertyexists);
  assert(status == napi_ok);
  if (!propertyexists) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nInvalid Object \n");
    return NULL;
  }

  UplinkProject project_result;
  project_result._handle = getHandleValue(env, args[0]);
  if (project_result._handle == 0) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Object \n");
    return NULL;
  }
  obj->project = project_result;
  obj->bucketname = bucketName;
  napi_value resource_name;
  napi_create_string_utf8(env, "ensureBucket",
  NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name, ensureBucketPromiseExecute,
  bucketOperationComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}

napi_value delete_bucketc(napi_env env, napi_callback_info info) {
  napi_status status;
  size_t argc = 2;
  napi_value args[2];
  napi_value promise;

  bucketOperationObj *obj = (bucketOperationObj *)
  malloc(sizeof(bucketOperationObj));
  if (obj == NULL) {
      free(obj);
    napi_throw_error(env, NULL, "Memory allocation error");
    return NULL;
  }
  obj->async_action_status = 1;
  status = napi_get_cb_info(env, info, &argc, args, nullptr , nullptr);
  assert(status == napi_ok);
  //
  status = napi_create_promise(env, &obj->deferred, &promise);
  if (status != napi_ok) {
      free(obj);
    napi_throw_error(env, NULL, "Unable to create promise");
    return NULL;
  }
  //

  if (argc < 2) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong number of arguments!! excepted 2 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env, args[0], &checktypeofinput);
  assert(status == napi_ok);

  if (checktypeofinput != napi_object) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env, args[1], &checktypeofinput);
  assert(status == napi_ok);

  if (checktypeofinput != napi_string) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! Second argument excepted to be string type\n");
    return NULL;
  }

  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1], NULL,
    bufsize, &convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;
  char* bucketName =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env, args[1],
    bucketName, convertedvalue, &bufsize);
  assert(status == napi_ok);

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  string handle = "_handle";
  status = napi_create_string_utf8(env,
    const_cast<char* > (handle.c_str()), NAPI_AUTO_LENGTH , &ObjectkeyNAPI);
  assert(status == napi_ok);

  status = napi_has_property(env, args[0],
    ObjectkeyNAPI, &propertyexists);
  assert(status == napi_ok);
  if (!propertyexists) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nInvalid Object \n");
    return NULL;
  }

  UplinkProject project_result;
  project_result._handle = getHandleValue(env, args[0]);
  if (project_result._handle == 0) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Object \n");
    return NULL;
  }
  obj->project = project_result;
  obj->bucketname = bucketName;
  napi_value resource_name;
  napi_create_string_utf8(env, "deleteBucket",
  NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name, deleteBucketPromiseExecute,
  bucketOperationComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}
