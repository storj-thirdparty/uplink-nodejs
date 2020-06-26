#include "access_operations_win.h"
#include <string>

napi_value parse_accessc(napi_env env,
    napi_callback_info info) {
    napi_value promise;
    napi_status status;

    size_t argc = 1;
    napi_value args[1];

    ParseAccessPromiseObj* obj = (ParseAccessPromiseObj*)
        malloc(sizeof(ParseAccessPromiseObj));
    if (obj == NULL) {
        free(obj);
        napi_throw_error(env, NULL, "Memory allocation error");
        return NULL;
    }

    obj->async_action_status = 1;
    status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    assert(status == napi_ok);

    if (argc < 1) {
        free(obj);
        napi_throw_type_error(env, nullptr,
            "\nWrong number of arguments!! excepted 3 arguments\n");
        return NULL;
    }

    status = napi_create_promise(env, &obj->deferred, &promise);
    if (status != napi_ok) {
        free(obj);
        napi_throw_error(env, NULL, "Unable to create promise");
    }
    napi_value str;
    napi_create_string_utf8(env, "Promise rejected", NAPI_AUTO_LENGTH, &str);

    napi_valuetype checktypeofinput;
    status = napi_typeof(env, args[0], &checktypeofinput);
    assert(status == napi_ok);

    if (checktypeofinput != napi_string) {
        free(obj);
        napi_throw_type_error(env, nullptr,
            "\nWrong datatype!! argument excepted to be string type\n");
        return NULL;
    }

    size_t bufsize = 0;
    size_t convertedvalue = 0;
    status = napi_get_value_string_utf8(env, args[0], NULL,
        bufsize, &convertedvalue);
    assert(status == napi_ok);
    convertedvalue = convertedvalue + 1;

    char* accessString = new char[convertedvalue];
    status = napi_get_value_string_utf8(env, args[0], accessString,
        convertedvalue, &bufsize);
    assert(status == napi_ok);
    obj->accessString = accessString;

    napi_value resource_name;
    napi_create_string_utf8(env, "parseAccess",
        NAPI_AUTO_LENGTH, &resource_name);
    napi_create_async_work(env, NULL, resource_name,
        ParseAccess, ParseAccessPromiseComplete, obj, &obj->work);
    napi_queue_async_work(env, obj->work);

    return promise;
}

napi_value access_sharec(napi_env env, napi_callback_info info) {
  napi_status status;
  size_t argc = 4;
  napi_value args[4];
  napi_value promise;

  AccessSharePromiseObj *obj = (AccessSharePromiseObj *)
  malloc(sizeof(AccessSharePromiseObj));
  if (obj == NULL) {
    free(obj);
    napi_throw_error(env, NULL, "Memory allocation error");
    return NULL;
  }

  status = napi_get_cb_info(env, info, &argc, args, nullptr , nullptr);
  assert(status == napi_ok);

  if (argc < 4) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong number of arguments!! excepted 4 arguments\n");
    return NULL;
  }

  status = napi_create_promise(env, &obj->deferred, &promise);
  if (status != napi_ok) {
      free(obj);
    napi_throw_error(env, NULL, "Unable to create promise");
  }
  napi_valuetype checktypeofinput1, checktypeofinput;
  status = napi_typeof(env, args[0], &checktypeofinput1);
  assert(status == napi_ok);

  if ((checktypeofinput1 != napi_object)&&(checktypeofinput1 != napi_null)) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env, args[1], &checktypeofinput);
  assert(status == napi_ok);

  if (checktypeofinput != napi_object) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! Second argument excepted to be string type\n");
    return NULL;
  }

  status = napi_typeof(env, args[2], &checktypeofinput);
  assert(status == napi_ok);

  if (checktypeofinput != napi_object) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! Third argument excepted to be string type\n");
    return NULL;
  }

  status = napi_typeof(env, args[3], &checktypeofinput);
  assert(status == napi_ok);

  if (checktypeofinput != napi_number) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! Fourth argument excepted to be string type\n");
    return NULL;
  }

  Access access;
  if (checktypeofinput1 != napi_null) {
    bool propertyexists = false;
    napi_value ObjectkeyNAPI;
    string handle = "_handle";
    status = napi_create_string_utf8(env,
      const_cast<char* > (handle.c_str()), NAPI_AUTO_LENGTH , &ObjectkeyNAPI);
    assert(status == napi_ok);

    status = napi_has_property(env, args[0], ObjectkeyNAPI,
      &propertyexists);
    assert(status == napi_ok);
    if (!propertyexists) {
        free(obj);
      napi_throw_type_error(env, nullptr, "\nInvalid Object \n");
      return NULL;
    }
    access._handle = getHandleValue(env, args[0]);
    if (access._handle == 0) {
        free(obj);
      napi_throw_type_error(env, nullptr, "\nInvalid Handle\n");
      return NULL;
    }
  }

  Permission permission;

  napi_value allow_downloadNAPI;
  status = napi_get_named_property(env, args[1], "allow_download",
    &allow_downloadNAPI);
  assert(status == napi_ok);

  bool allow_downloadc;
  status = napi_get_value_bool(env, allow_downloadNAPI, &allow_downloadc);
  assert(status == napi_ok);

  permission.allow_download = allow_downloadc;

  napi_value allow_uploadNAPI;
  status = napi_get_named_property(env, args[1], "allow_upload",
    &allow_uploadNAPI);
  assert(status == napi_ok);

  bool allow_uploadc;
  status = napi_get_value_bool(env, allow_uploadNAPI, &allow_uploadc);
  assert(status == napi_ok);

  permission.allow_upload = allow_uploadc;

  napi_value allow_listNAPI;
  status = napi_get_named_property(env, args[1], "allow_list",
    &allow_listNAPI);
  assert(status == napi_ok);

  bool allow_listc;
  status = napi_get_value_bool(env, allow_listNAPI, &allow_listc);
  assert(status == napi_ok);

  permission.allow_list = allow_listc;

  napi_value allow_deleteNAPI;
  status = napi_get_named_property(env, args[1], "allow_delete",
    &allow_deleteNAPI);
  assert(status == napi_ok);

  bool allow_deletec;
  status = napi_get_value_bool(env, allow_deleteNAPI, &allow_deletec);
  assert(status == napi_ok);

  permission.allow_delete = allow_deletec;

  napi_value not_beforeNAPI;
  status = napi_get_named_property(env, args[1], "not_before",
    &not_beforeNAPI);
  assert(status == napi_ok);

  int64_t not_beforec;
  status = napi_get_value_int64(env, not_beforeNAPI, &not_beforec);
  assert(status == napi_ok);

  permission.not_before = not_beforec;

  napi_value not_afterNAPI;
  status = napi_get_named_property(env, args[1], "not_after",
    &not_afterNAPI);
  assert(status == napi_ok);

  int64_t not_afterc;
  status = napi_get_value_int64(env, not_afterNAPI, &not_afterc);
  assert(status == napi_ok);

  permission.not_after = not_afterc;

  int64_t sharePrefixSize;
  status = napi_get_value_int64(env, args[3], &sharePrefixSize);
  assert(status == napi_ok);

  // Checking whether is array of not
  bool isarray = false;
  status = napi_is_array(env, args[2], &isarray);
  assert(status == napi_ok);
  if (!isarray) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong data type of 3 parameter \n");
    return NULL;
  }

  uint32_t sizeOfArray = 0;
  status = napi_get_array_length(env, args[2], &sizeOfArray);
  assert(status == napi_ok);

  int sizeOfArrayInt = static_cast<int>(sizeOfArray);
  int size;
  if (sizeOfArray == sharePrefixSize) {
    size = static_cast<int>(sharePrefixSize);
  } else {
    size = sizeOfArrayInt;
  }
  SharePrefix* SharePrefixListPointer = new SharePrefix[size];

  napi_value SharePrefixObject;

  for (uint32_t i=0; i < (uint32_t)sizeOfArrayInt; i++) {
    SharePrefix sharePrefix;
    status = napi_get_element(env, args[2], i, &SharePrefixObject);
    assert(status == napi_ok);

    napi_value bucket;
    status = napi_get_named_property(env, SharePrefixObject,
      "bucket", &bucket);
    assert(status == napi_ok);

    size_t bufsize = 0;
    size_t convertedvalue = 0;
    status = napi_get_value_string_utf8(env, bucket, NULL, bufsize,
      &convertedvalue);
    assert(status == napi_ok);
    convertedvalue = convertedvalue+1;

    char* bucketc =  new char[convertedvalue];
    status = napi_get_value_string_utf8(env, bucket, bucketc,
      convertedvalue, &bufsize);
    assert(status == napi_ok);
    napi_value prefix;
    status = napi_get_named_property(env, SharePrefixObject,
      "prefix", &prefix);
    assert(status == napi_ok);

    bufsize = 0;
    convertedvalue = 0;
    status = napi_get_value_string_utf8(env, prefix, NULL, bufsize,
      &convertedvalue);
    assert(status == napi_ok);
    convertedvalue = convertedvalue+1;

    char* prefixc =  new char[convertedvalue];
    status = napi_get_value_string_utf8(env, prefix, prefixc,
      convertedvalue, &bufsize);
    assert(status == napi_ok);
    sharePrefix.bucket = bucketc;
    sharePrefix.prefix = prefixc;
    *(SharePrefixListPointer+i) = sharePrefix;
  }
  obj->SharePrefixSize = sharePrefixSize;
  obj->SharePrefixListPointer = SharePrefixListPointer;
  obj->permission = permission;
  obj->access = access;
  napi_value resource_name;
  napi_create_string_utf8(env, "accessShare", NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name, ShareAccessPromiseExecute,
  ShareAccessPromiseComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}

napi_value config_request_access_with_passphrasec(napi_env env,
  napi_callback_info info) {
  napi_status status;
  size_t argc = 4;
  napi_value args[4];
  napi_value promise;

  ConfigRequestAccessPromiseObj *obj = (ConfigRequestAccessPromiseObj *)
  malloc(sizeof(ConfigRequestAccessPromiseObj));
  if (obj == NULL) {
      free(obj);
    napi_throw_error(env, NULL, "Memory allocation error");
    return NULL;
  }

  status = napi_get_cb_info(env, info, &argc, args, nullptr , nullptr);
  assert(status == napi_ok);

  if (argc < 4) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong number of arguments!! excepted 4 arguments\n");
    return NULL;
  }

  status = napi_create_promise(env, &obj->deferred, &promise);
  if (status != napi_ok) {
      free(obj);
    napi_throw_error(env, NULL, "Unable to create promise");
  }

  Config config;

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

  if (checktypeofinput != napi_string) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! Fourth argument excepted to be string type\n");
    return NULL;
  }

  bool configUserAgentExists, configDialTimeoutMilliSecondsExists,
  configTempDirectoryExists = false;
  napi_value configUserAgentNAPI, configDialTimeoutMilliSecondsNAPI,
  configTempDirectoryNAPI;
  string user_agent_property = "user_agent";
  string dial_timeout_milliseconds_property = "dial_timeout_milliseconds";
  string temp_directory_property = "temp_directory";
  status = napi_create_string_utf8(env,
    const_cast<char* > (user_agent_property.c_str()), NAPI_AUTO_LENGTH ,
    &configUserAgentNAPI);
  assert(status == napi_ok);

  status = napi_has_property(env, args[0], configUserAgentNAPI,
    &configUserAgentExists);
  assert(status == napi_ok);

  if (!configUserAgentExists) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nInvalid Config Object \n");
    return NULL;
  }

  status = napi_create_string_utf8(env,
    const_cast<char* > (dial_timeout_milliseconds_property.c_str()),
    NAPI_AUTO_LENGTH , &configDialTimeoutMilliSecondsNAPI);
  assert(status == napi_ok);

  status = napi_has_property(env, args[0],
    configDialTimeoutMilliSecondsNAPI,
    &configDialTimeoutMilliSecondsExists);
  assert(status == napi_ok);

  if (!configDialTimeoutMilliSecondsExists) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nInvalid Config Object \n");
    return NULL;
  }

  status = napi_create_string_utf8(env,
    const_cast<char* > (temp_directory_property.c_str()),
    NAPI_AUTO_LENGTH , &configTempDirectoryNAPI);
  assert(status == napi_ok);

  status = napi_has_property(env, args[0],
    configTempDirectoryNAPI, &configTempDirectoryExists);
  assert(status == napi_ok);

  if (!configTempDirectoryExists) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nInvalid Config Object \n");
    return NULL;
  }

  napi_value user_agentNAPI;
  status = napi_get_named_property(env, args[0],
    "user_agent", &user_agentNAPI);
  assert(status == napi_ok);

  napi_value dial_timeout_millisecondsNAPI;
  status = napi_get_named_property(env, args[0],
    "dial_timeout_milliseconds", &dial_timeout_millisecondsNAPI);
  assert(status == napi_ok);

  napi_value temp_directoryNAPI;
  status = napi_get_named_property(env, args[0],
    "temp_directory", &temp_directoryNAPI);
  assert(status == napi_ok);

  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env,
    user_agentNAPI, NULL, bufsize, &convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;

  char* user_agent =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,
    user_agentNAPI, user_agent, convertedvalue, &bufsize);
  assert(status == napi_ok);

  bufsize = 0;
  convertedvalue = 0;
  status = napi_get_value_string_utf8(env,
    temp_directoryNAPI, NULL, bufsize, &convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;

  char* temp_directory =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,
    temp_directoryNAPI, temp_directory, convertedvalue, &bufsize);
  assert(status == napi_ok);

  int32_t dial_timeout_milliseconds;
  status = napi_get_value_int32(env,
    dial_timeout_millisecondsNAPI, &dial_timeout_milliseconds);
  assert(status == napi_ok);

  config.temp_directory = temp_directory;
  config.dial_timeout_milliseconds = dial_timeout_milliseconds;
  config.user_agent = user_agent;
  delete[] user_agent;
  delete[] temp_directory;

  bufsize = 0;
  convertedvalue = 0;
  status = napi_get_value_string_utf8(env,
    args[1], NULL, bufsize, &convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;

  char* satellite_address =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,
    args[1], satellite_address, convertedvalue, &bufsize);
  assert(status == napi_ok);

  bufsize = 0;
  convertedvalue = 0;
  status = napi_get_value_string_utf8(env,
    args[2], NULL, bufsize, &convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;

  char* api_key =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,
    args[2], api_key, convertedvalue, &bufsize);
  assert(status == napi_ok);

  bufsize = 0;
  convertedvalue = 0;
  status = napi_get_value_string_utf8(env,
    args[3], NULL, bufsize, &convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;

  char* passphrase =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,
    args[3], passphrase, convertedvalue, &bufsize);
  assert(status == napi_ok);
  obj->config = config;
  obj->satellite_address = satellite_address;
  obj->api_key = api_key;
  obj->passphrase = passphrase;
  napi_value resource_name;
  napi_create_string_utf8(env, "ConfigRequestAccess",
  NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name,
  ConfigRequestAccessWithEncryption,
  ConfigRequestAccessWithEncryptionPromiseComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}

napi_value request_access_with_passphrasec(napi_env env,
  napi_callback_info info) {
  napi_value promise;
  napi_status status;
  size_t argc = 3;
  napi_value args[3];

  RequestAccessPromiseObj *obj = (RequestAccessPromiseObj *)
  malloc(sizeof(RequestAccessPromiseObj));
  if (obj == NULL) {
      free(obj);
    napi_throw_error(env, NULL, "Memory allocation error");
    return NULL;
  }

  obj->async_action_status = 1;
  status = napi_get_cb_info(env, info, &argc, args, nullptr , nullptr);
  assert(status == napi_ok);

  if (argc < 3) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong number of arguments!! excepted 3 arguments\n");
    return NULL;
  }

  status = napi_create_promise(env, &obj->deferred, &promise);
  if (status != napi_ok) {
      free(obj);
    napi_throw_error(env, NULL, "Unable to create promise");
  }
  napi_value str;
  napi_create_string_utf8(env, "Promise rejected", NAPI_AUTO_LENGTH, &str);

  napi_valuetype checktypeofinput;
  status = napi_typeof(env, args[0], &checktypeofinput);
  assert(status == napi_ok);

  if (checktypeofinput != napi_string) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! First argument excepted to be string type\n");
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

  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[0], NULL, bufsize,
    &convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;

  char* satellite_address =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,
    args[0], satellite_address, convertedvalue, &bufsize);
  assert(status == napi_ok);
  obj->satellite_address = satellite_address;

  bufsize = 0;
  convertedvalue = 0;
  status = napi_get_value_string_utf8(env,
    args[1], NULL, bufsize, &convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;

  char* api_key =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,
    args[1], api_key, convertedvalue, &bufsize);
  assert(status == napi_ok);
  obj->api_key = api_key;

  bufsize = 0;
  convertedvalue = 0;
  status = napi_get_value_string_utf8(env,
    args[2], NULL, bufsize, &convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;

  char* passphrase =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,
    args[2], passphrase, convertedvalue, &bufsize);
  assert(status == napi_ok);
  obj->passphrase = passphrase;

  napi_value resource_name;
  napi_create_string_utf8(env, "RequestAccess",
  NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name, RequestAccessWithEncryption,
  RequestAccessWithEncryptionPromiseComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}

napi_value access_serializec(napi_env env, napi_callback_info info) {
  napi_status status;
  size_t argc = 1;
  napi_value args[1];
  napi_value promise;

  accessSerializePromiseObj *obj = (accessSerializePromiseObj *)
  malloc(sizeof(accessSerializePromiseObj));
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
  }
  //
  if (argc < 1) {
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
  status = napi_has_property(env, args[0],
    ObjectkeyNAPI, &propertyexists);
  assert(status == napi_ok);
  if (!propertyexists) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nInvalid Object \n");
    return NULL;
  }

  Access access;
  access._handle = getHandleValue(env, args[0]);
  if (access._handle == 0) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Object \n");
    return NULL;
  }
  obj->access = access;
  napi_value resource_name;
  napi_create_string_utf8(env, "accessSerialize", NAPI_AUTO_LENGTH,
  &resource_name);
  napi_create_async_work(env, NULL, resource_name,
  accessSerializePromiseExecute, accessSerializePromiseComplete,
  obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}
