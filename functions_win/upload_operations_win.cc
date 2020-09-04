#include "upload_operations_win.h"
#include <string>

napi_value upload_set_custom_metadatac(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value promise;
  size_t argc = 2;
  napi_value args[2];

  uploadSetMetaObj *obj = (uploadSetMetaObj *)malloc(sizeof(uploadSetMetaObj));
  if (obj == NULL) {
      free(obj);
    napi_throw_error(env, NULL, "Memory allocation error");
    return NULL;
  }

  status = napi_get_cb_info(env, info, &argc, args, nullptr , nullptr);
  assert(status == napi_ok);
  if (argc < 2) {
      free(obj);
    napi_throw_type_error(env,
      nullptr, "\nWrong number of arguments!! excepted 1 arguments\n");
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
      "\nWrong datatype !! first argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env, args[1], &checktypeofinput);
  assert(status == napi_ok);
  if (checktypeofinput != napi_object) {
      free(obj);
    napi_throw_type_error(env,
      nullptr, "\nWrong datatype !! second argument excepted to be array\n");
    return NULL;
  }

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  string handle = "_handle";
  status = napi_create_string_utf8(env,
    const_cast<char* > (handle.c_str()), NAPI_AUTO_LENGTH ,
    &ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env, args[0], ObjectkeyNAPI, &propertyexists);
  assert(status == napi_ok);
  if (!propertyexists) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Object \n");
    return NULL;
  }
  UplinkUpload upload_result;
  upload_result._handle = getHandleValue(env, args[0]);
  if (upload_result._handle == 0) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nWrong object passed\n");
    return NULL;
  }
  // Checking Property exits aur not
  bool entriesExists = false;
  napi_value entriesStringNAPI;
  string entries = "entries";
  status = napi_create_string_utf8(env,
    const_cast<char* > (entries.c_str()), NAPI_AUTO_LENGTH ,
    &entriesStringNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env, args[1], entriesStringNAPI, &entriesExists);
  assert(status == napi_ok);
  if (!entriesExists) {
      free(obj);
    napi_throw_type_error(
      env, nullptr, "\nWrong Object Passed In Second Parameter\n");
    return NULL;
  }
  // Fetching Entries
  napi_value entriesArrayNAPI;
  status = napi_get_named_property(env, args[1], "entries", &entriesArrayNAPI);
  assert(status == napi_ok);

  napi_value countNAPI;
  status = napi_get_named_property(env, args[1], "count", &countNAPI);
  assert(status == napi_ok);
  int64_t countc;
  status = napi_get_value_int64(env, countNAPI, &countc);
  assert(status == napi_ok);
  // Checking whether is array of not
  bool isarray = false;
  status = napi_is_array(env, entriesArrayNAPI, &isarray);
  assert(status == napi_ok);
  if (!isarray) {
      free(obj);
    napi_throw_type_error(
      env, nullptr, "\nEntries is not array\n");
    return NULL;
  }

  uint32_t sizeOfArray = 0;
  status = napi_get_array_length(env, entriesArrayNAPI, &sizeOfArray);
  assert(status == napi_ok);

  int size = static_cast<int>(sizeOfArray);
  UplinkCustomMetadataEntry* CustomMetadataEntryListPointer =
  new UplinkCustomMetadataEntry[size];
  UplinkCustomMetadata customMetadata;
  napi_value CustomMetadataEntryObject;

  for (int i=0; i < size; i++) {
    UplinkCustomMetadataEntry customMetadataEntry;
    status = napi_get_element(
      env, entriesArrayNAPI, i, &CustomMetadataEntryObject);
    assert(status == napi_ok);

    napi_value keyNAPI, key_lengthNAPI, valueNAPI, value_lengthNAPI;

    int64_t key_lengthc;

    status = napi_get_named_property(
      env, CustomMetadataEntryObject, "key", &keyNAPI);
    assert(status == napi_ok);

    size_t bufsize = 0;
    size_t convertedvalue = 0;

    status = napi_get_value_string_utf8(
      env, keyNAPI, NULL, bufsize, &convertedvalue);
    assert(status == napi_ok);
    convertedvalue = convertedvalue+1;

    char* keyc =  new char[convertedvalue];
    status = napi_get_value_string_utf8(
      env, keyNAPI, keyc, convertedvalue, &bufsize);
    assert(status == napi_ok);

    status = napi_get_named_property(
      env, CustomMetadataEntryObject, "key_length", &key_lengthNAPI);
    assert(status == napi_ok);
    status = napi_get_value_int64(
      env, key_lengthNAPI, &key_lengthc);
    assert(status == napi_ok);

    status = napi_get_named_property(
      env, CustomMetadataEntryObject, "value", &valueNAPI);
    assert(status == napi_ok);

    bufsize = 0;
    convertedvalue = 0;
    status = napi_get_value_string_utf8(
      env, valueNAPI, NULL, bufsize, &convertedvalue);
    assert(status == napi_ok);
    convertedvalue = convertedvalue+1;

    char* valuec =  new char[convertedvalue];
    status = napi_get_value_string_utf8(env,
      valueNAPI, valuec, convertedvalue, &bufsize);
    assert(status == napi_ok);

    status = napi_get_named_property(env,
      CustomMetadataEntryObject, "value_length", &value_lengthNAPI);
    assert(status == napi_ok);

    int64_t value_lengthsize;

    status = napi_get_value_int64(env, value_lengthNAPI, &value_lengthsize);
    assert(status == napi_ok);

    customMetadataEntry.key = keyc;
    customMetadataEntry.key_length = key_lengthc;
    customMetadataEntry.value = valuec;
    customMetadataEntry.value_length = value_lengthsize;
    *(CustomMetadataEntryListPointer+i) = customMetadataEntry;
  }
  customMetadata.entries = (CustomMetadataEntryListPointer+0);
  if (countc == size) {
    customMetadata.count = countc;
  } else {
    customMetadata.count = sizeOfArray;
  }
  obj->upload_result = upload_result;
  obj->customMetadata = customMetadata;
  napi_value resource_name;
  napi_create_string_utf8(env, "uploadSetCustomObject",
  NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name, uploadSetMetaPromiseExecute,
  uploadSetMetaPromiseComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}
//
napi_value upload_abortc(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value promise;
  size_t argc = 1;
  napi_value args[1];
  uploadAbortPromiseObj *obj = (uploadAbortPromiseObj *)
  malloc(sizeof(uploadAbortPromiseObj));
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
    const_cast<char* > (handle.c_str()), NAPI_AUTO_LENGTH ,
    &ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env, args[0], ObjectkeyNAPI, &propertyexists);
  assert(status == napi_ok);
  if (!propertyexists) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Object \n");
    return NULL;
  }
  UplinkUpload upload_result;
  upload_result._handle = getHandleValue(env, args[0]);

  obj->upload_result = upload_result;
  napi_value resource_name;
  napi_create_string_utf8(env, "uploadAbort", NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name, uploadAbortPromiseExecute,
  uploadAbortPromiseComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}

napi_value upload_infoc(napi_env env, napi_callback_info info) {
  napi_status status;
  size_t argc = 1;
  napi_value args[1];
  napi_value promise;

  uploadInfoObj *obj = (uploadInfoObj *)malloc(sizeof(uploadInfoObj));
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
  if (argc < 1) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong number of arguments!! excepted 1 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env, args[0], &checktypeofinput);
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
    const_cast<char* > (handle.c_str()), NAPI_AUTO_LENGTH ,
    &ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env, args[0], ObjectkeyNAPI, &propertyexists);
  assert(status == napi_ok);
  if (!propertyexists) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Object \n");
    return NULL;
  }

  UplinkUpload upload_result;
  upload_result._handle = getHandleValue(env, args[0]);
  if (upload_result._handle == 0) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Object\n");
    return NULL;
  }
  obj->upload_result = upload_result;
  napi_value resource_name;
  napi_create_string_utf8(env, "upladInfoObject",
  NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name, uploadInfoPromiseExecute,
  uploadInfoOperationComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}

napi_value upload_commitc(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value promise;

  size_t argc = 1;
  napi_value args[1];
  uploadCommitObj *obj = (uploadCommitObj *)malloc(sizeof(uploadCommitObj));
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

  UplinkUpload upload_result;
  upload_result._handle = getHandleValue(env, args[0]);
  if (upload_result._handle == 0) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Object \n");
    return NULL;
  }
  obj->upload_result = upload_result;
  napi_value resource_name;
  napi_create_string_utf8(env, "uploadCommit",
  NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name, uploadCommitPromiseExecute,
  uploadCommitOperationComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}

napi_value upload_writec(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value promise;

  size_t argc = 3;
  napi_value args[3];

  uploadWriteObj *obj = (uploadWriteObj *)malloc(sizeof(uploadWriteObj));
  if (obj == NULL) {
      free(obj);
    napi_throw_error(env, NULL, "Memory allocation error");
    return NULL;
  }
  obj->async_action_status = 1;
  status = napi_get_cb_info(env, info, &argc, args, nullptr , nullptr);
  assert(status == napi_ok);

  status = napi_create_promise(env, &obj->deferred, &promise);
  if (status != napi_ok) {
      free(obj);
    napi_throw_error(env, NULL, "Unable to create promise");
    return NULL;
  }

  if (argc < 3) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong number of arguments!! excepted 3 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env, args[0], &checktypeofinput);
  assert(status == napi_ok);
  if (checktypeofinput != napi_object) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! first argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env, args[1], &checktypeofinput);
  assert(status == napi_ok);
  if (checktypeofinput != napi_object) {
      free(obj);
    napi_throw_type_error(env, nullptr,
  "\nWrong datatype !! second argument excepted to be object/buffer type\n");
    return NULL;
  }

  status = napi_typeof(env, args[2], &checktypeofinput);
  assert(status == napi_ok);
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
  status = napi_has_property(env, args[0], ObjectkeyNAPI, &propertyexists);
  assert(status == napi_ok);
  if (!propertyexists) {
    napi_throw_type_error(env, nullptr,
      "\nInvalid Object \n");
    return NULL;
  }

  UplinkUpload upload_resultRef;
  upload_resultRef._handle = getHandleValue(env, args[0]);
  if (upload_resultRef._handle == 0) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Object \n");
    return NULL;
  }
  bool buffertype1;
  status = napi_is_arraybuffer(env, args[1],
    &buffertype1);
  assert(status == napi_ok);
  size_t lengthOfBuffer;
  void* bufferPtr = NULL;

  status = napi_get_buffer_info(env, args[1], &bufferPtr,
    &lengthOfBuffer);
  assert(status == napi_ok);

  int64_t bytes64;
  status = napi_get_value_int64(env, args[2], &bytes64);
  assert(status == napi_ok);

  size_t bytesread = (size_t)bytes64;
  obj->bufferPtr = reinterpret_cast<uint8_t *>(bufferPtr);
  obj->bytesread = bytesread;
  obj->upload_result = upload_resultRef;
  napi_value resource_name;
  napi_create_string_utf8(env, "uploadWrite", NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name, uploadWritePromiseExecute,
  uploadWriteOperationComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}

napi_value upload_objectc(napi_env env, napi_callback_info info) {
  napi_status status;
  size_t argc = 4;
  napi_value args[4];
  napi_value promise;
  //
  uploadobjectObj *obj = (uploadobjectObj *)malloc(sizeof(uploadobjectObj));
  if (obj == NULL) {
      free(obj);
    napi_throw_error(env, NULL, "Memory allocation error");
    return NULL;
  }
  obj->async_action_status = 1;
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
      "\nWrong number of arguments!! excepted 4 arguments\n");
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

  status = napi_typeof(env, args[2], &checktypeofinput);
  assert(status == napi_ok);

  if (checktypeofinput != napi_string) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! Third argument excepted to be string type\n");
    return NULL;
  }

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
      if (checktypeofinput == napi_null) {
          obj->uploadoptionSet = 0;
      } else {
        UplinkUploadOptions uploadOptions;
        napi_value expiresNAPI;
        obj->uploadoptionSet = 1;
        status = napi_get_named_property(env, args[3], "expires",
          &expiresNAPI);
        assert(status == napi_ok);

        int64_t expires;
        status = napi_get_value_int64(env, expiresNAPI, &expires);
        assert(status == napi_ok);
        uploadOptions.expires = expires;
        obj->uploadOptions = uploadOptions;
      }
      obj->bucketname = bucketName;
      obj->objectkey = objectKey;
      obj->project = project_result;
      napi_value resource_name;
      napi_create_string_utf8(env, "uploadObject",
      NAPI_AUTO_LENGTH, &resource_name);
      napi_create_async_work(env, NULL, resource_name, uploadObjectExecute,
      uploadObjectComplete, obj, &obj->work);
      napi_queue_async_work(env, obj->work);
      return promise;
}
