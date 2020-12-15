// Copyright 2020 Storj Storj
/** @mainpage Node-js bindings
 *  It uses napi for creating node module
 * 
 */
#include "download_operations.h"
#include <string>
/*!
 \fn napi_value download_infoc(napi_env env, napi_callback_info info)
 \brief download_infoc function is called from the javascript file
 download_info returns information about the downloaded object . 
 */
napi_value download_infoc(napi_env env, napi_callback_info info) {
  napi_status status;
  size_t argc = 1;
  napi_value promise;
  napi_value args[1];

  downloadInfoObj *obj = (downloadInfoObj *)
  malloc(sizeof(downloadInfoObj));
  if (obj == NULL) {
      free(obj);
    napi_throw_error(env, NULL, "Memory allocation error");
    return NULL;
  }
  //
  status = napi_get_cb_info(env,
    info, &argc, args, nullptr , nullptr);
  assert(status == napi_ok);
  //
  status = napi_create_promise(env, &obj->deferred, &promise);
  if (status != napi_ok) {
      free(obj);
    napi_throw_error(env, NULL, "Unable to create promise");
    return NULL;
  }
  //
  if (argc < 1) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong number of arguments!! excepted 1 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env,
    args[0], &checktypeofinput);
  assert(status == napi_ok);
  if (checktypeofinput != napi_object) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! first argument excepted to be object type\n");
    return NULL;
  }

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  string handle = "_handle";
  status = napi_create_string_utf8(env,
    const_cast<char* > (handle.c_str()), NAPI_AUTO_LENGTH , &ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env, args[0], ObjectkeyNAPI, &propertyexists);
  assert(status == napi_ok);
  if (!propertyexists) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Object \n");
    return NULL;
  }

  UplinkDownload download_result;
  download_result._handle = getHandleValue(env, args[0]);
  obj->download_result = download_result;
  napi_value resource_name;
  napi_create_string_utf8(env, "downloadInfo",
  NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name, downloadInfoPromiseExecute,
  downloadInfoOperationComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}
/*!
 \fn napi_value close_downloadc(napi_env env, napi_callback_info info)
 \brief close_downloadc function is called from the javascript file
         close_downloadc closes the download .
     
 */
//
napi_value close_downloadc(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value promise;
  size_t argc = 1;
  napi_value args[1];

  downloadCloseObj *obj = (downloadCloseObj *)
  malloc(sizeof(downloadCloseObj));
  if (obj == NULL) {
      free(obj);
    napi_throw_error(env, NULL, "Memory allocation error");
    return NULL;
  }
  status = napi_get_cb_info(env, info, &argc, args, nullptr , nullptr);
  assert(status == napi_ok);

  if (argc < 1) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong number of arguments!! excepted 1 arguments\n");
    return NULL;
  }
  status = napi_create_promise(env, &obj->deferred, &promise);
  if (status != napi_ok) {
      free(obj);
    napi_throw_error(env, NULL, "Unable to create promise");
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

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  string handle = "_handle";
  status = napi_create_string_utf8(env,
    const_cast<char* > (handle.c_str()), NAPI_AUTO_LENGTH , &ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env, args[0], ObjectkeyNAPI, &propertyexists);
  assert(status == napi_ok);
  if (!propertyexists) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nInvalid Object \n");
    return NULL;
  }

  UplinkDownload download_result;
  download_result._handle = getHandleValue(env, args[0]);
  obj->download_result = download_result;
  napi_value resource_name;
  napi_create_string_utf8(env, "downloadClose",
  NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name, downloadClosePromiseExecute,
  downloadCloseOperationComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}
/*!
 \fn napi_value download_readc(napi_env env, napi_callback_info info)
 \brief download_readc function is called from the javascript file
  download_readc reads the download .
  
 */
//
napi_value download_readc(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value promise;
  size_t argc = 3;
  napi_value args[3];

  downloadReadObj *obj = (downloadReadObj *)malloc(sizeof(downloadReadObj));
  if (obj == NULL) {
      free(obj);
    napi_throw_error(env, NULL, "Memory allocation error");
    return NULL;
  }

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
  if (argc < 3) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong number of arguments!! excepted 3 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env, args[0], &checktypeofinput);
  assert(status == napi_ok);
  //
  if (checktypeofinput != napi_object) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! first argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env, args[1], &checktypeofinput);
  assert(status == napi_ok);
  //
  if (checktypeofinput != napi_object) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! second argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env, args[2], &checktypeofinput);
  assert(status == napi_ok);
  //
  if (checktypeofinput != napi_number) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! third argument excepted to be number type\n");
    return NULL;
  }

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  string handle = "_handle";
  status = napi_create_string_utf8(env,
    const_cast<char* > (handle.c_str()), NAPI_AUTO_LENGTH , &ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env, args[0],
    ObjectkeyNAPI, &propertyexists);
  assert(status == napi_ok);
  if (!propertyexists) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nInvalid Object \n");
    return NULL;
  }

  UplinkDownload download_resulterRef;
  download_resulterRef._handle = getHandleValue(env, args[0]);

  void* bufferPtr = NULL;
  size_t lengthOfBuffer;
  status = napi_get_buffer_info(env, args[1],
    &bufferPtr, &lengthOfBuffer);
  assert(status == napi_ok);
  //
  obj->bufferPtr = bufferPtr;
  obj->download_result = download_resulterRef;
  //
  obj->bufferlength = lengthOfBuffer;
  //
  napi_value resource_name;
  napi_create_string_utf8(env, "downloadRead",
  NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name, downloadReadPromiseExecute,
  downloadReadOperationComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}
/*!
 \fn napi_value download_objectc(napi_env env, napi_callback_info info)
 \brief download_objectc function is called from the javascript file
 download_objectc starts  download to the specified key.
    
 */
//
napi_value download_objectc(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value promise;
  size_t argc = 4;
  napi_value args[4];
  //
  downloadObjectObj *obj = (downloadObjectObj *)
  malloc(sizeof(downloadObjectObj));
  if (obj == NULL) {
      free(obj);
    napi_throw_error(env, NULL, "Memory allocation error");
    return NULL;
  }
  //
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
  if (argc < 4) {
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
  //
  status = napi_typeof(env, args[2], &checktypeofinput);
  assert(status == napi_ok);

  if (checktypeofinput != napi_string) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! Third argument excepted to be string type\n");
    return NULL;
  }
  //
  status = napi_typeof(env, args[3], &checktypeofinput);
  assert(status == napi_ok);

  if ((checktypeofinput != napi_object)&&(checktypeofinput != napi_null)) {
      free(obj);
    napi_throw_type_error(env, nullptr,
  "\nWrong datatype !! Fourth argument excepted to be object type or null\n");
    return NULL;
  }

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  string handle = "_handle";
  status = napi_create_string_utf8(env,
    const_cast<char* > (handle.c_str()), NAPI_AUTO_LENGTH , &ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env, args[0], ObjectkeyNAPI, &propertyexists);
  assert(status == napi_ok);
  if (!propertyexists) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Object \n");
    return NULL;
  }

  UplinkProject project_result;
  project_result._handle = getHandleValue(env, args[0]);
  if (project_result._handle == 0) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Object \n");
    return NULL;
  }
  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1], NULL, bufsize,
    &convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;

  char* bucketName =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env, args[1], bucketName,
    convertedvalue, &bufsize);
  assert(status == napi_ok);
  bufsize = 0;
  convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[2], NULL, bufsize,
    &convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;

  char* objectKey =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env, args[2], objectKey,
    convertedvalue, &bufsize);
  assert(status == napi_ok);
  obj->bucketname = bucketName;
  obj->objectkey = objectKey;
  obj->project = project_result;
  if (checktypeofinput == napi_null) {
    obj->downloadoptionSet = 0;

  } else {
    obj->downloadoptionSet = 1;
    UplinkDownloadOptions downloadOption;
    napi_value offsetNAPI, lengthNAPI;

    status = napi_get_named_property(env, args[3], "offset", &offsetNAPI);
    assert(status == napi_ok);

    status = napi_get_named_property(env, args[3], "length", &lengthNAPI);
    assert(status == napi_ok);

    int64_t offset, length;
    status = napi_get_value_int64(env, offsetNAPI, &offset);
    assert(status == napi_ok);
    status = napi_get_value_int64(env, lengthNAPI, &length);
    assert(status == napi_ok);

    downloadOption.offset = offset;
    downloadOption.length = length;
    obj->downloadOption = downloadOption;
  }
  napi_value resource_name;
  napi_create_string_utf8(env, "downloadObject",
  NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name, downloadObjectPromiseExecute,
  downloadObjectOperationComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}