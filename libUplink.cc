// Copyright 2020 Storj Storj
/** @mainpage Node-js bindings
 *  It uses napi for creating node module
 * 
 */
#include "libUplink.h"
#include <utility>
#include <string>
/*! \fn void reverse(char str[], int length)
    \brief A utility function to reverse a string
*/
/* A utility function to reverse a string  */
void reverse(char str[], int length) {
    int start = 0;
    int end = length -1;
    while (start < end) {
        swap(*(str+start), *(str+end));
        start++;
        end--;
    }
}
/*! \fn itoa(int num, char* str, int base)
    \brief itoa function negative numbers are handled only with
     base 10. Otherwise numbers are considered unsigned , 
    itoa returns a string
*/
char* itoa(int num, char* str, int base) {
    int i = 0;
    bool isNegative = false;
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }
    // Process individual digits
    while (num  !=  0) {
        int rem = num % base;
        str[i++] = (rem  >  9)? (rem-10)+ 'a' : rem + '0';
        num = num/base;
    }
    // If number is negative, append '-'
    if (isNegative) {
        str[i++] = '-';
    }
    str[i] = '\0';
    // Append string terminator
    // Reverse the string
    reverse(str, i);
    return str;
}
/*! \fn napi_value createError(napi_env env, int64_t accessError,
  char* errorMessagePtr)
   \brief napi_value createError creates NAPI type error object
      it returns the object for which error occurred
*/
// function creates NAPI type error object
napi_value createError(napi_env env, int64_t accessError,
  char* errorMessagePtr) {
//
  napi_status status;
  napi_value returnObject, errorObject, errorCode, errorMessage;

  status = napi_create_object(env, &errorObject);
  assert(status == napi_ok);

  status = napi_create_object(env, &returnObject);
  assert(status == napi_ok);

  status = napi_create_int64(env, accessError, &errorCode);
  assert(status == napi_ok);

  status = napi_create_string_utf8(env,
    errorMessagePtr, NAPI_AUTO_LENGTH, &errorMessage);
  assert(status == napi_ok);

  status = napi_set_named_property(env, errorObject, "code", errorCode);
  assert(status == napi_ok);

  status = napi_set_named_property(env, errorObject,
    "message", errorMessage);
  assert(status == napi_ok);

  status = napi_set_named_property(env, returnObject,
    "error", errorObject);

  assert(status == napi_ok);
  return returnObject;
}
/*! \fn int64_t getHandleValue(napi_env env, napi_value handleobj)
   \brief int64_t getHandleValue handles the value type
      it returns integer type handlelong
*/
//
int64_t getHandleValue(napi_env env, napi_value handleobj) {
  napi_status status;
  napi_valuetype checktypeofinput;

  status = napi_typeof(env, handleobj, &checktypeofinput);
  assert(status == napi_ok);

  if (checktypeofinput != napi_object) {
    return 0;
  }
  napi_value propertyname;
  status = napi_get_property_names(env, handleobj, &propertyname);
  assert(status == napi_ok);

  napi_value handlevalue;
  status = napi_get_property(env, handleobj, propertyname, &handlevalue);
  assert(status == napi_ok);

  status = napi_typeof(env, handlevalue, &checktypeofinput);
  assert(status == napi_ok);

  if (checktypeofinput != napi_number) {
    napi_throw_type_error(env, nullptr, "\nWrong Handle type\n");
    return 0;
  }
  int64_t handle = 0;
  status = napi_get_value_int64(env, handlevalue, &handle);
  assert(status == napi_ok);

  int64_t handlelong = handle;
  return handlelong;
}
/*! \fn napi_value createResult(napi_env env, string resultType,
    int64_t handlevalue)
   \brief napi_value createResult create result of new object whether it is generated or not
*/
//
napi_value createResult(napi_env env, string resultType,
    int64_t handlevalue) {
    napi_status status;
    napi_value ObjectRef, returnObject;
    napi_value _handle;
    // Creating NAPI Object's
    status = napi_create_object(env, &ObjectRef);
    assert(status == napi_ok);

    status = napi_create_object(env, &returnObject);
    assert(status == napi_ok);

    status = napi_create_int64(env, handlevalue, &_handle);
    assert(status == napi_ok);

    status = napi_set_named_property(env, ObjectRef, "_handle", _handle);
    assert(status == napi_ok);
    const char* resultTypeChar = resultType.c_str();

    status = napi_set_named_property(env, returnObject,
    &resultTypeChar[0], ObjectRef);
    assert(status == napi_ok);

    return returnObject;
}
/*! \fn napi_value createBucketResult(napi_env env, string resultType,
  int64_t bucketCreated, char* bucketNamePtr)
     \brief createBucketResult creates bucket result whether it is generated or not
*/
//
napi_value createBucketResult(napi_env env, string resultType,
  int64_t bucketCreated, char* bucketNamePtr) {
  napi_status status;
  napi_value ObjectRef, returnObject;

  status = napi_create_object(env, &ObjectRef);
  assert(status == napi_ok);

  status = napi_create_object(env, &returnObject);
  assert(status == napi_ok);

  napi_value created, bucketName;
  status = napi_create_int64(env, bucketCreated, &created);
  assert(status == napi_ok);
  status = napi_set_named_property(env, ObjectRef, "created", created);
  assert(status == napi_ok);

  status = napi_create_string_utf8(env,
    bucketNamePtr, NAPI_AUTO_LENGTH, &bucketName);
  assert(status == napi_ok);

  status = napi_set_named_property(env, ObjectRef, "name", bucketName);
  assert(status == napi_ok);

  return ObjectRef;
}
/*! \fn napi_value createObjectResult(napi_env env,
  Object* objectPtr)
     \brief createObjectResult create object's result whether it is generated or not
 
  */
//
napi_value createObjectResult(napi_env env,
  Object* objectPtr) {
  napi_value returnObject, objectNAPI,
  SystemMetadataNAPI, CustomMetadataNAPI, CustomMetadataEntryNAPI;
  napi_status status;

  status = napi_create_object(env, &returnObject);
  assert(status == napi_ok);
  if (objectPtr != NULL) {
//
    Object object_result = *(objectPtr);

    status = napi_create_object(env, &objectNAPI);
    assert(status == napi_ok);

    status = napi_create_object(env, &SystemMetadataNAPI);
    assert(status == napi_ok);

    status = napi_create_object(env, &CustomMetadataNAPI);
    assert(status == napi_ok);

    status = napi_create_object(env, &CustomMetadataEntryNAPI);
    assert(status == napi_ok);

    napi_value keyObjectNAPI;
    status = napi_create_string_utf8(env,
      objectPtr-> key, NAPI_AUTO_LENGTH, &keyObjectNAPI);
    assert(status == napi_ok);

    napi_value is_prefixNAPI;
    if (objectPtr->  is_prefix) {
      status = napi_create_int64(env, 1, &is_prefixNAPI);
      assert(status == napi_ok);
    } else {
      status = napi_create_int64(env, 0, &is_prefixNAPI);
      assert(status == napi_ok);
    }
    //
    SystemMetadata systemMetadata;
    CustomMetadata customMetadata;
    CustomMetadataEntry customMetadataEntry;
    systemMetadata = object_result.system;
    customMetadata = object_result.custom;
    napi_value entriesArray;

    status = napi_create_object(env, &entriesArray);
    assert(status == napi_ok);
    char empty[]="";

    if (customMetadata.count > 0) {
//
      CustomMetadataEntry* CustomMetadataEntryListPointer =
      customMetadata.entries;
      for (uint32_t j=0; j < customMetadata.count; j++) {
//
        customMetadataEntry = *(CustomMetadataEntryListPointer+j);
        napi_value key_lengthNAPI, value_lengthNAPI, keyNAPI,
        valueNAPI, customMetaNAPI;
        status = napi_create_object(env, &customMetaNAPI);
        assert(status == napi_ok);

        status = napi_create_int64(env,
          customMetadataEntry.key_length, &key_lengthNAPI);
        assert(status == napi_ok);

        status = napi_create_int64(env,
          customMetadataEntry.value_length, &value_lengthNAPI);
        assert(status == napi_ok);

        status = napi_create_string_utf8(env,
          customMetadataEntry.key, NAPI_AUTO_LENGTH, &keyNAPI);
        assert(status == napi_ok);

        status = napi_create_string_utf8(env,
          customMetadataEntry.value, NAPI_AUTO_LENGTH, &valueNAPI);
        assert(status == napi_ok);

        status = napi_set_named_property(env,
          customMetaNAPI, "key", keyNAPI);
        assert(status == napi_ok);

        status = napi_set_named_property(env,
          customMetaNAPI, "key_length", key_lengthNAPI);
        assert(status == napi_ok);

        status = napi_set_named_property(env,
          customMetaNAPI, "value", valueNAPI);
        assert(status == napi_ok);

        status = napi_set_named_property(env,
          customMetaNAPI, "value_length", value_lengthNAPI);
        assert(status == napi_ok);

        status = napi_set_named_property(env,
          customMetaNAPI, "value_length", value_lengthNAPI);
        assert(status == napi_ok);

        char str[100];
        itoa(j, str, 10);
        status = napi_set_named_property(env,
          entriesArray, str, customMetaNAPI);
        assert(status == napi_ok);
      }
    } else {
      napi_value emptyNAPI;
      status = napi_create_string_utf8(env,
        &empty[0], NAPI_AUTO_LENGTH, &emptyNAPI);
      assert(status == napi_ok);

      status = napi_set_named_property(env,
        entriesArray, "0", emptyNAPI);
      assert(status == napi_ok);
    }

    napi_value createdNAPI;
    status = napi_create_int64(env, objectPtr->system.created,
      &createdNAPI);
    assert(status == napi_ok);

    napi_value expiresNAPI;
    status = napi_create_int64(env, objectPtr->system.expires,
      &expiresNAPI);
    assert(status == napi_ok);

    napi_value countNAPI;
    status = napi_create_int64(env, customMetadata.count,
      &countNAPI);
    assert(status == napi_ok);

    napi_value content_lengthNAPI;
    status = napi_create_int64(env,
      systemMetadata.content_length, &content_lengthNAPI);
    assert(status == napi_ok);

    status = napi_set_named_property(env,
      CustomMetadataNAPI, "entries", entriesArray);
    assert(status == napi_ok);

    status = napi_set_named_property(env,
      CustomMetadataNAPI, "count", countNAPI);
    assert(status == napi_ok);

    status = napi_set_named_property(env,
      objectNAPI, "custom", CustomMetadataNAPI);
    assert(status == napi_ok);

    status = napi_set_named_property(env,
      SystemMetadataNAPI, "created", createdNAPI);
    assert(status == napi_ok);

    status = napi_set_named_property(env,
      SystemMetadataNAPI, "expires", expiresNAPI);
    assert(status == napi_ok);

    status = napi_set_named_property(env,
      SystemMetadataNAPI, "content_length", content_lengthNAPI);
    assert(status == napi_ok);

    status = napi_set_named_property(env,
      objectNAPI, "system", SystemMetadataNAPI);
    assert(status == napi_ok);

    status = napi_set_named_property(env,
      objectNAPI, "is_prefix", is_prefixNAPI);
    assert(status == napi_ok);

    status = napi_set_named_property(env,
      objectNAPI, "key", keyObjectNAPI);
    assert(status == napi_ok);
  }
  return objectNAPI;
}
//
/*!
 \fn napi_value AccessFunction(napi_env env, napi_value AccessNAPIObj)
  \brief AccessFunction helps to use function open_projectc
  ,access_serializec, access_sharec, config_open_projectc
  
 */
napi_value AccessFunction(napi_env env, napi_value AccessNAPIObj) {
  //
  napi_status status;
  napi_value openprojectNAPI, accessSerializeFunction,
  accessShareFunction, configOpenProjectFunction;

  status = napi_create_function(env, NULL, 0, open_projectc,
  NULL, &openprojectNAPI);
  assert(status == napi_ok);

  status = napi_set_named_property(env, AccessNAPIObj,
  "open_project", openprojectNAPI);
  assert(status == napi_ok);

  status = napi_create_function(env, NULL, 0, access_serializec,
  NULL, &accessSerializeFunction);
  assert(status == napi_ok);

  status = napi_set_named_property(env, AccessNAPIObj,
  "access_serialize", accessSerializeFunction);
  assert(status == napi_ok);

  status = napi_create_function(env, NULL, 0, access_sharec,
  NULL, &accessShareFunction);
  assert(status == napi_ok);

  status = napi_set_named_property(env, AccessNAPIObj,
  "access_share", accessShareFunction);
  assert(status == napi_ok);

  status = napi_create_function(env, NULL, 0, config_open_projectc,
  NULL, &configOpenProjectFunction);
  assert(status == napi_ok);

  status = napi_set_named_property(env, AccessNAPIObj,
  "config_open_project", configOpenProjectFunction);
  assert(status == napi_ok);
  //
  return AccessNAPIObj;
}
/*!
 \fn napi_value ProjectFunction(napi_env env, napi_value projectNAPIObj)
 \brief ProjectFunction helps to use functions list_bucketsc, stat_bucketc, 
 create_bucketc, delete_bucketc, ensure_bucketc, list_objectsc, stat_objectc,
  delete_objectc, upload_objectc, download_objectc
 */
//
napi_value ProjectFunction(napi_env env, napi_value projectNAPIObj) {
  napi_status status;
  napi_value listbucketsFunction, statbucketFunction, createBucketFunction,
  ensureBucketFunction, deleteBucketFunction, statObjectFunction,
  deleteObjectFunction, uploadObjectFunction, closeProjectFunction;
    napi_value downloadObjectFunction, listObjectFunction;
    status = napi_create_function(env, NULL, 0, list_bucketsc,
    NULL, &listbucketsFunction);
    assert(status == napi_ok);

    status = napi_create_function(env, NULL, 0, stat_bucketc,
    NULL, &statbucketFunction);
    assert(status == napi_ok);

    status = napi_create_function(env, NULL, 0, create_bucketc,
    NULL, &createBucketFunction);
    assert(status == napi_ok);

    status = napi_create_function(env, NULL, 0, ensure_bucketc,
    NULL, &ensureBucketFunction);
    assert(status == napi_ok);

    status = napi_create_function(env, NULL, 0, delete_bucketc,
    NULL, &deleteBucketFunction);
    assert(status == napi_ok);

    status = napi_create_function(env, NULL, 0, delete_objectc,
    NULL, &deleteObjectFunction);
    assert(status == napi_ok);

    status = napi_create_function(env, NULL, 0, stat_objectc,
    NULL, &statObjectFunction);
    assert(status == napi_ok);

    status = napi_create_function(env, NULL, 0, upload_objectc,
    NULL, &uploadObjectFunction);
    assert(status == napi_ok);

    status = napi_create_function(env, NULL, 0, list_objectsc,
    NULL, &listObjectFunction);
    assert(status == napi_ok);

    status = napi_create_function(env, NULL, 0, download_objectc,
    NULL, &downloadObjectFunction);
    assert(status == napi_ok);
    //
    status = napi_set_named_property(env, projectNAPIObj,
    "listbuckets", listbucketsFunction);
    assert(status == napi_ok);

    status = napi_set_named_property(env, projectNAPIObj,
    "stat_bucket", statbucketFunction);
    assert(status == napi_ok);

    status = napi_set_named_property(env, projectNAPIObj,
    "create_bucket", createBucketFunction);
    assert(status == napi_ok);

    status = napi_set_named_property(env, projectNAPIObj,
    "ensure_bucket", ensureBucketFunction);
    assert(status == napi_ok);

    status = napi_set_named_property(env, projectNAPIObj,
    "delete_bucket", deleteBucketFunction);
    assert(status == napi_ok);

    status = napi_set_named_property(env, projectNAPIObj,
    "delete_object", deleteObjectFunction);
    assert(status == napi_ok);

    status = napi_set_named_property(env, projectNAPIObj,
    "stat_object", statObjectFunction);
    assert(status == napi_ok);

    status = napi_set_named_property(env, projectNAPIObj,
    "upload_object", uploadObjectFunction);
    assert(status == napi_ok);

    status = napi_set_named_property(env, projectNAPIObj,
    "download_object", downloadObjectFunction);
    assert(status == napi_ok);
    //
    status = napi_set_named_property(env, projectNAPIObj,
    "list_objects", listObjectFunction);
    assert(status == napi_ok);

    status = napi_create_function(env, NULL, 0, close_projectc,
    NULL, &closeProjectFunction);
    assert(status == napi_ok);

    status = napi_set_named_property(env, projectNAPIObj,
    "close_project", closeProjectFunction);
    assert(status == napi_ok);
    return projectNAPIObj;
}
/*!
 \fn napi_value list_objectsc(napi_env env, napi_callback_info info)
 \brief list_objectsc creates list of objects 
  promise function makes list_objectsc asynchronous function
 
 */
//
napi_value list_objectsc(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value promise;
  size_t argc = 3;
  napi_value args[3];

  listObjectPromiseObj *obj = (listObjectPromiseObj *)
  malloc(sizeof(listObjectPromiseObj));
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
  if (argc < 3) {
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
      "\nWrong datatype !! Second argument excepted to be string\n");
    return NULL;
  }
  //
  status = napi_typeof(env, args[2], &checktypeofinput);
  assert(status == napi_ok);

  if ((checktypeofinput != napi_object)&&(checktypeofinput != napi_null)) {
      free(obj);
    napi_throw_type_error(env, nullptr,
      "\nWrong datatype !! Second argument excepted to be object or NULL\n");
    return NULL;
  }
  //
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

  Project project_result;
  project_result._handle = getHandleValue(env, args[0]);
  if (project_result._handle == 0) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Object \n");
    return NULL;
  }
  //
  ListObjectsOptions listObjectsOptions;
  //
  if (checktypeofinput == napi_null) {
    obj->listObjectSet = 0;
      } else if (checktypeofinput == napi_object) {
        //
        napi_value cursorNAPI;
        status = napi_get_named_property(env, args[2], "cursor",
          &cursorNAPI);
        assert(status == napi_ok);
        //
        napi_value prefixNAPI;
        status = napi_get_named_property(env, args[2], "prefix",
          &prefixNAPI);
        assert(status == napi_ok);
        //
        size_t bufsize = 0;
        size_t convertedvalue = 0;
        status = napi_get_value_string_utf8(env, cursorNAPI, NULL,
          bufsize, &convertedvalue);
        assert(status == napi_ok);
        convertedvalue = convertedvalue+1;

        char* cursor =  new char[convertedvalue];
        status = napi_get_value_string_utf8(env, cursorNAPI, cursor,
          convertedvalue, &bufsize);
        assert(status == napi_ok);
        listObjectsOptions.cursor = cursor;


        bufsize = 0;
        convertedvalue = 0;
        //
        status = napi_get_value_string_utf8(env, prefixNAPI, NULL,
          bufsize, &convertedvalue);
        assert(status == napi_ok);
        convertedvalue = convertedvalue+1;

        char* prefix =  new char[convertedvalue];
        status = napi_get_value_string_utf8(env, prefixNAPI, prefix,
          convertedvalue, &bufsize);
        assert(status == napi_ok);
        listObjectsOptions.prefix = prefix;

        napi_value recursiveNAPI;
        status = napi_get_named_property(env, args[2], "recursive",
          &recursiveNAPI);
        assert(status == napi_ok);
        //
        napi_value systemNAPI;
        status = napi_get_named_property(env, args[2], "system",
          &systemNAPI);
        assert(status == napi_ok);
        //
        napi_value customNAPI;
        status = napi_get_named_property(env, args[2], "custom",
          &customNAPI);
        assert(status == napi_ok);
        //
        bool recursive;
        napi_get_value_bool(env, recursiveNAPI, &recursive);
        assert(status == napi_ok);
        //
        bool system;
        napi_get_value_bool(env, systemNAPI, &system);
        assert(status == napi_ok);
        //
        bool custom;
        napi_get_value_bool(env, customNAPI, &custom);
        assert(status == napi_ok);
        //
        listObjectsOptions.custom = custom;
        listObjectsOptions.system = system;
        listObjectsOptions.recursive = recursive;
        obj->listObjectSet = 1;
      }
  obj->list_object_options = listObjectsOptions;
  obj->bucketName = bucketName;
  obj->project_result = project_result;
  napi_value resource_name;
  napi_create_string_utf8(env, "listObjects", NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name, listObjectPromiseExecute,
  listObjectPromiseComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
}

//
/*!
 \fn napi_value list_bucketsc(napi_env env, napi_callback_info info)
  \brief list_bucketsc provide list of buckets
  using promise makes list_bucketsc asynchronous
  
 */

napi_value list_bucketsc(napi_env env, napi_callback_info info) {
  napi_value promise;
  napi_status status;

  size_t argc = 2;
  napi_value args[2];

  ListBucketsPromiseObj *obj = (ListBucketsPromiseObj *)
  malloc(sizeof(ListBucketsPromiseObj));
  if (obj == NULL) {
      free(obj);
    napi_throw_error(env, NULL, "Memory allocation error");
    return NULL;
  }
  obj->async_action_status = 1;

  status = napi_get_cb_info(env, info, &argc, args, nullptr , nullptr);
  assert(status == napi_ok);

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

  if ((checktypeofinput != napi_object)&&(checktypeofinput != napi_null)) {
      free(obj);
    napi_throw_type_error(env, nullptr,
  "\nWrong datatype !! Second argument excepted to be object type or null\n");
    return NULL;
  }

  ListBucketsOptions listBucketsOptions;
  if (checktypeofinput == napi_null) {
    listBucketsOptions = {};
    obj->listBucketsOptions = listBucketsOptions;
  } else if (checktypeofinput == napi_object) {
    napi_value cursorNAPI;
    status = napi_get_named_property(env, args[1], "cursor", &cursorNAPI);
    assert(status == napi_ok);
    size_t bufsize = 0;
    size_t convertedvalue = 0;
    status = napi_get_value_string_utf8(env, cursorNAPI, NULL, bufsize,
      &convertedvalue);
    assert(status == napi_ok);
    convertedvalue = convertedvalue+1;

    char* cursor =  new char[convertedvalue];
    status = napi_get_value_string_utf8(env, cursorNAPI, cursor,
      convertedvalue, &bufsize);
    assert(status == napi_ok);
    listBucketsOptions.cursor = cursor;
    obj->listBucketsOptions = listBucketsOptions;
    delete[] cursor;
  }

  status = napi_create_promise(env, &obj->deferred, &promise);
  if (status != napi_ok) {
      free(obj);
    napi_throw_error(env, NULL, "Unable to create promise");
    return NULL;
  }

  napi_value str;
  napi_create_string_utf8(env, "Promise rejected", NAPI_AUTO_LENGTH, &str);
  //
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
  //
  Project project_result;
  project_result._handle = getHandleValue(env, args[0]);

  if (project_result._handle == 0) {
      free(obj);
    napi_throw_type_error(env, nullptr, "\nInvalid Object \n");
    return NULL;
  }


  obj->project_result = project_result;

  napi_value resource_name;
  napi_create_string_utf8(env, "ListBuckets", NAPI_AUTO_LENGTH, &resource_name);
  napi_create_async_work(env, NULL, resource_name, ListBucketsPromiseExecute,
  ListBucketsPromiseComplete, obj, &obj->work);
  napi_queue_async_work(env, obj->work);
  return promise;
//
}
/**/
/*!
  \fn napi_value Init(napi_env env, napi_value exports)
   \brief This is main function
  1) request_access_with_passphrase requests satellite for a new access grant using a passhprase .
  2) parse_access parses serialized access grant string .
  3) config_request_access_with_passphrase requests satellite for a new access grant using a passhprase .
 */
napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor request_access_with_passphrase = DECLARE_NAPI_METHOD(
    "request_access_with_passphrase", request_access_with_passphrasec);
  status = napi_define_properties(
    env, exports, 1, &request_access_with_passphrase);
  assert(status == napi_ok);

  napi_property_descriptor parse_access = DECLARE_NAPI_METHOD(
    "parse_access", parse_accessc);
  status = napi_define_properties(
    env, exports, 1, &parse_access);
  assert(status == napi_ok);

  napi_property_descriptor config_request_access_with_passphrase =
  DECLARE_NAPI_METHOD(
    "config_request_access_with_passphrase",
    config_request_access_with_passphrasec);
  status = napi_define_properties(
    env, exports, 1, &config_request_access_with_passphrase);
  assert(status == napi_ok);
  return exports;
}
NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
