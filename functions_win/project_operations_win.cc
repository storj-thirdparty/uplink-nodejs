#include "project_operations_win.h"
#include "project_operations_win.h"
#include <string>

napi_value close_projectc(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value promise;

  size_t argc = 1;
  napi_value args[1];

  closeProjectPromiseObj *obj = (closeProjectPromiseObj *)
  malloc(sizeof(closeProjectPromiseObj));

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
      "\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }
  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  string handle = "_handle";
  status = napi_create_string_utf8(env,
    const_cast<char* > (handle.c_str()), NAPI_AUTO_LENGTH , &ObjectkeyNAPI);
  assert(status == napi_ok);

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
    return NULL;
  }
  obj->project_result = project_result;
  napi_value resource_name;
  napi_create_string_utf8(env, "CloseProject",
  NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name, closeProjectPromiseExecute,
  closeProjectPromiseComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}

napi_value config_open_projectc(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value promise;

  size_t argc = 2;
  napi_value args[2];

  configOpenProjectPromiseObj *obj = (configOpenProjectPromiseObj *)
  malloc(sizeof(configOpenProjectPromiseObj));

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

  if (argc < 2) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong number of arguments!! excepted 2 arguments\n");
    return NULL;
  }

  UplinkConfig config;

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

  if (checktypeofinput != napi_object) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! Second argument excepted to be object type\n");
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

  status = napi_has_property(env, args[0],
    configUserAgentNAPI, &configUserAgentExists);
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
    napi_throw_type_error(env, nullptr, "\nInvalid Config Object \n");
    return NULL;
  }

  status = napi_create_string_utf8(env,
    const_cast<char* > (temp_directory_property.c_str()), NAPI_AUTO_LENGTH ,
    &configTempDirectoryNAPI);
  assert(status == napi_ok);

  status = napi_has_property(env, args[0],
    configTempDirectoryNAPI, &configTempDirectoryExists);
  assert(status == napi_ok);

  if (!configTempDirectoryExists) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Config Object \n");
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
  status = napi_get_value_string_utf8(env, user_agentNAPI,
    NULL, bufsize, &convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;

  char* user_agent =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env, user_agentNAPI,
    user_agent, convertedvalue, &bufsize);
  assert(status == napi_ok);

  bufsize = 0;
  convertedvalue = 0;
  status = napi_get_value_string_utf8(env, temp_directoryNAPI,
    NULL, bufsize, &convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;

  char* temp_directory =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env, temp_directoryNAPI,
    temp_directory, convertedvalue, &bufsize);
  assert(status == napi_ok);

  int32_t dial_timeout_milliseconds;
  status = napi_get_value_int32(env, dial_timeout_millisecondsNAPI,
    &dial_timeout_milliseconds);
  assert(status == napi_ok);

  config.temp_directory = temp_directory;
  config.dial_timeout_milliseconds = dial_timeout_milliseconds;
  config.user_agent = user_agent;

  obj->config = config;

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  string handle = "_handle";
  status = napi_create_string_utf8(env,
    const_cast<char* > (handle.c_str()), NAPI_AUTO_LENGTH , &ObjectkeyNAPI);
  assert(status == napi_ok);

  status = napi_has_property(env, args[1], ObjectkeyNAPI, &propertyexists);
  assert(status == napi_ok);
  if (!propertyexists) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Object \n");
    return NULL;
  }

  UplinkAccess access;
  access._handle = getHandleValue(env, args[1]);
  if (access._handle == 0) {
      free(obj);
    return NULL;
  }

  obj->access = access;
  napi_value resource_name;
  napi_create_string_utf8(env, "ConfigOpenProject",
  NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name,
  configOpenProjectPromiseExecute, configOpenProjectPromiseComplete,
  obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}

napi_value open_projectc(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value promise;

  size_t argc = 1;
  napi_value args[1];

  openProjectPromiseObj *obj = (openProjectPromiseObj *)
  malloc(sizeof(openProjectPromiseObj));

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
      "\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  string handle = "_handle";
  status = napi_create_string_utf8(env,
    const_cast<char* > (handle.c_str()), NAPI_AUTO_LENGTH , &ObjectkeyNAPI);
  assert(status == napi_ok);

  status = napi_has_property(env, args[0], ObjectkeyNAPI, &propertyexists);
  assert(status == napi_ok);
  if (!propertyexists) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Object \n");
    return NULL;
  }
  UplinkAccess access;
  access._handle = getHandleValue(env, args[0]);
  if (access._handle == 0) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Object \n");
    return NULL;
  }
  obj->access = access;
  napi_value resource_name;
  napi_create_string_utf8(env, "OpenProject", NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name, openProjectPromiseExecute,
  openProjectPromiseComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}
