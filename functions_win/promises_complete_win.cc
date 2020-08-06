#include "promises_complete_win.h"
#include <string>

void openProjectPromiseComplete(napi_env env, napi_status status, void* data) {
    openProjectPromiseObj* obj = (openProjectPromiseObj*)data;
    ProjectResult project_result = obj->project_Result;
    if (project_result.project == NULL) {
        if (project_result.error != NULL) {
            Error error_result = *(project_result.error);
            char* errorMessagePtr = error_result.message;
            char blank[] = "";
            if (errorMessagePtr == NULL) { errorMessagePtr = &blank[0]; }
            status = napi_reject_deferred(env, obj->deferred,
            createError(env, error_result.code, errorMessagePtr));
        }
    } else {
        Project project = *(project_result.project);
        size_t handlevalue = project._handle;

        napi_value projectNAPIObj = createResult(env, "project", handlevalue);
      
        //
        status = napi_resolve_deferred(env, obj->deferred, projectNAPIObj);
    }

    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Failed to return promise");
    }

    napi_delete_async_work(env, obj->work);
    free(obj);
}

void ListObjectsPromiseComplete(napi_env env, napi_status status, void* data) {
    listObjectsPromiseObj* obj = (listObjectsPromiseObj*)data;
    napi_value objectList;
    //
    status = napi_create_object(env, &objectList);
    assert(status == napi_ok);
    //
    typedef bool (*ObjectIteratorNextFUNC)(ObjectIterator*);
    ObjectIteratorNextFUNC object_iterator_next =
        (ObjectIteratorNextFUNC)GetProcAddress(hGetProcIDDLL,
            "object_iterator_next");
    if (!object_iterator_next) {
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
      int count = 0;
      while (object_iterator_next(obj->objectIterator)) {
            typedef Object *(*BucketIteratorItemFUNC)(ObjectIterator*);
            BucketIteratorItemFUNC object_iterator_item =
                (BucketIteratorItemFUNC)GetProcAddress(hGetProcIDDLL,
                    "object_iterator_item");
            if (!object_iterator_item) {
                napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
            } else {
                Object* objectPtr = object_iterator_item(obj->objectIterator);
                napi_value objectNAPI = createObjectResult(env, objectPtr);

                char str[100];
                itoa(count, str, 10);
                status = napi_set_named_property(env, objectList, str,
                    objectNAPI);
                assert(status == napi_ok);
                count++;
            }
        }
    }

    typedef Error *(*ObjectIteratorErrFUNC)(ObjectIterator*);
    ObjectIteratorErrFUNC object_iterator_err =
        (ObjectIteratorErrFUNC)GetProcAddress(hGetProcIDDLL,
            "object_iterator_err");
    if (!object_iterator_err) {
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        Error* err = object_iterator_err(obj->objectIterator);
        if (err != NULL) {
            Error errorResult = *(err);
            char* messagePtr = errorResult.message;
            char blank[] = "";
            if (messagePtr == NULL) { messagePtr = &blank[0]; }
            status = napi_reject_deferred(env, obj->deferred,
            createError(env, errorResult.code, messagePtr));
        } else {
            status = napi_resolve_deferred(env, obj->deferred, objectList);
        }

        if (status != napi_ok) {
            napi_throw_error(env, NULL, "Failed to return promise");
        }
    }
    napi_delete_async_work(env, obj->work);
    free(obj);
}

void downloadInfoOperationComplete(napi_env env,
napi_status status, void* data) {
  downloadInfoObj *obj = (downloadInfoObj*)data;
  ObjectResult object_result = obj->object_result;
  //
  if (object_result.object == NULL) {
    if (object_result.error != NULL) {
      Error error_result = *(object_result.error);
      char* errorMessagePtr = error_result.message;
      status = napi_reject_deferred(env, obj->deferred,
      createError(env, error_result.code, errorMessagePtr));
    }
  } else {
    napi_value objectNAPI = createObjectResult(env, object_result.object);

    status = napi_resolve_deferred(env, obj->deferred, objectNAPI);
  }
  //
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to return promise");
  }
  napi_delete_async_work(env, obj->work);
  free(obj);
}

void downloadCloseOperationComplete(napi_env env,
napi_status status, void* data) {
  downloadCloseObj *obj = (downloadCloseObj*)data;

  if (obj->error_result != NULL) {
      Error error_result = *(obj->error_result);
      char* errorMessagePtr = error_result.message;
      status = napi_reject_deferred(env, obj->deferred,
      createError(env, error_result.code, errorMessagePtr));
  } else {
    //
    napi_value undefined;
    status = napi_get_undefined(env, &undefined);
    assert(status == napi_ok);
    //
    status = napi_resolve_deferred(env, obj->deferred, undefined);
  }
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to return promise");
  }
  napi_delete_async_work(env, obj->work);
  free(obj);
}

void downloadReadOperationComplete(napi_env env,
napi_status status, void* data) {
  downloadReadObj *obj = (downloadReadObj*)data;
  ReadResult read_result = obj->read_result;

  if (read_result.error != NULL) {
      Error error_result = *(read_result.error);
      char* errorMessagePtr = error_result.message;
      char blank[] = "";
      if (errorMessagePtr == NULL) {errorMessagePtr=&blank[0];}
      status = napi_reject_deferred(env, obj->deferred,
      createError(env, error_result.code, errorMessagePtr));
  } else {
    napi_value downloadReadNAPI;
    status = napi_create_object(env, &downloadReadNAPI);
    assert(status == napi_ok);
    size_t bytesread = read_result.bytes_read;
    napi_value bytesReadObject;

    status = napi_create_int32(env, bytesread, &bytesReadObject);
    assert(status == napi_ok);

    status = napi_set_named_property(env, downloadReadNAPI, "bytes_read",
    bytesReadObject);
    assert(status == napi_ok);

    status = napi_resolve_deferred(env, obj->deferred, downloadReadNAPI);
  }
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to return promise");
  }
  napi_delete_async_work(env, obj->work);
  free(obj);
}

void downloadObjectOperationComplete(napi_env env,
napi_status status, void* data) {
    downloadObjectObj* obj = (downloadObjectObj*)data;
    DownloadResult download_result = obj->download_result;
    if (download_result.download == NULL) {
        if (download_result.error != NULL) {
            Error error_result = *(download_result.error);
            char* errorMessagePtr = error_result.message;
            char blank[] = "";
            if (errorMessagePtr == NULL) { errorMessagePtr = &blank[0]; }
            status = napi_reject_deferred(env, obj->deferred,
            createError(env, error_result.code, errorMessagePtr));
        }
    } else {
        Download download = *(download_result.download);
        size_t handlevalue = download._handle;


        napi_value downloadResultNAPI = createResult(env, "download",
        handlevalue);

        status = napi_resolve_deferred(env, obj->deferred, downloadResultNAPI);
    }
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Failed to return promise");
    }
    napi_delete_async_work(env, obj->work);
    free(obj);
}

void uploadSetMetaPromiseComplete(napi_env env,
napi_status status, void* data) {
  uploadSetMetaObj *obj = (uploadSetMetaObj*)data;
  Error* error_result = obj->error_result;
  if (error_result != NULL) {
      Error errorResult = *(error_result);
      char* errorMessagePtr = errorResult.message;
      char blank[] = "";
      if (errorMessagePtr == NULL) { errorMessagePtr = &blank[0]; }
      status = napi_reject_deferred(env, obj->deferred,
      createError(env, errorResult.code, errorMessagePtr));
  } else {
      //
      napi_value undefined;
      status = napi_get_undefined(env, &undefined);
      assert(status == napi_ok);
      //
      status = napi_resolve_deferred(env, obj->deferred, undefined);
  }
  if (status != napi_ok) {
      napi_throw_error(env, NULL, "Failed to return promise");
  }

  napi_delete_async_work(env, obj->work);
  free(obj);
}


void uploadAbortPromiseComplete(napi_env env, napi_status status, void* data) {
  uploadAbortPromiseObj *obj = (uploadAbortPromiseObj*)data;
  Error* error_result = obj->error_result;
  if (error_result != NULL) {
    Error errorResult = *(error_result);
    char* errorMessagePtr = errorResult.message;
    status = napi_reject_deferred(env, obj->deferred,
    createError(env, errorResult.code, errorMessagePtr));
  } else {
      napi_value undefined;
      status = napi_get_undefined(env, &undefined);
      assert(status == napi_ok);
      //
      status = napi_resolve_deferred(env, obj->deferred, undefined);
  }
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to return promise");
  }
  napi_delete_async_work(env, obj->work);
  free(obj);
}

void uploadInfoOperationComplete(napi_env env, napi_status status, void* data) {
  uploadInfoObj *obj = (uploadInfoObj*)data;
  ObjectResult object_result = obj->object_result;

  if (object_result.object == NULL) {
    if (object_result.error != NULL) {
      Error error_result = *(object_result.error);
      char* errorMessagePtr = error_result.message;
      status = napi_reject_deferred(env, obj->deferred,
      createError(env, error_result.code, errorMessagePtr));
    }
  } else {
    napi_value objectNAPI = createObjectResult(env, object_result.object);

    status = napi_resolve_deferred(env, obj->deferred, objectNAPI);
  }
  //
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to return promise");
  }
  napi_delete_async_work(env, obj->work);
  free(obj);
}

void uploadCommitOperationComplete(napi_env env,
napi_status status, void* data) {
  uploadCommitObj *obj = (uploadCommitObj*)data;

  if (obj->error_result != NULL) {
      Error error_result = *(obj->error_result);
      char* errorMessagePtr = error_result.message;
      status = napi_reject_deferred(env, obj->deferred,
      createError(env, error_result.code, errorMessagePtr));
  } else {
    //
    napi_value undefined;
    status = napi_get_undefined(env, &undefined);
    assert(status == napi_ok);
    //
    status = napi_resolve_deferred(env, obj->deferred, undefined);
  }
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to return promise");
  }
  napi_delete_async_work(env, obj->work);
  free(obj);
}

void uploadWriteOperationComplete(napi_env env,
napi_status status, void* data) {
  uploadWriteObj *obj = (uploadWriteObj*)data;
  WriteResult write_result = obj->write_result;

  if (write_result.error != NULL) {
      Error error_result = *(write_result.error);
      char* errorMessagePtr = error_result.message;
      status = napi_reject_deferred(env, obj->deferred,
      createError(env, error_result.code, errorMessagePtr));
  } else {
    //
    napi_value uploadWriteNAPI;
    status = napi_create_object(env, &uploadWriteNAPI);
    assert(status == napi_ok);
    //
    size_t datauploaded = write_result.bytes_written;
    napi_value datauploadedObject;

    status = napi_create_int32(env, datauploaded, &datauploadedObject);
    assert(status == napi_ok);

    status = napi_set_named_property(env, uploadWriteNAPI, "bytes_written",
    datauploadedObject);
    assert(status == napi_ok);

    status = napi_resolve_deferred(env, obj->deferred, uploadWriteNAPI);
  }
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to return promise");
  }
  napi_delete_async_work(env, obj->work);
  free(obj);
}

void uploadObjectComplete(napi_env env, napi_status status, void* data) {
    uploadobjectObj* obj = (uploadobjectObj*)data;
    UploadResult upload_result = obj->upload_result;
    if (upload_result.upload == NULL) {
        if (upload_result.error != NULL) {
            Error error_result = *(upload_result.error);
            char* errorMessagePtr = error_result.message;
            char blank[] = "";
            if (errorMessagePtr == NULL) { errorMessagePtr = &blank[0]; }
            status = napi_reject_deferred(env, obj->deferred,
            createError(env, error_result.code, errorMessagePtr));
        }
    } else {
        Upload upload = *(upload_result.upload);
        size_t handlevalue = upload._handle;

        napi_value uploadResultNAPI = createResult(env, "upload", handlevalue);

        //
        status = napi_resolve_deferred(env, obj->deferred, uploadResultNAPI);
    }
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Failed to return promise");
    }
    napi_delete_async_work(env, obj->work);
    free(obj);
}

void objectOperationComplete(napi_env env, napi_status status, void* data) {
  objectOperationObj *obj = (objectOperationObj*)data;
  ObjectResult object_result = obj->object_result;
  if (object_result.object == NULL) {
    if (object_result.error != NULL) {
      Error error_result = *(object_result.error);
      char* errorMessagePtr = error_result.message;
      status = napi_reject_deferred(env, obj->deferred,
      createError(env, error_result.code, errorMessagePtr));
    }
  } else {
    napi_value objectNAPI = createObjectResult(env, object_result.object);

    status = napi_resolve_deferred(env, obj->deferred, objectNAPI);
  }
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to return promise");
  }
  napi_delete_async_work(env, obj->work);
  free(obj);
}

void bucketOperationComplete(napi_env env, napi_status status, void* data) {
  bucketOperationObj *obj = (bucketOperationObj*)data;
  BucketResult bucket_result = obj->bucket_Result;
    if (bucket_result.error != NULL) {
      Error error_result = *(bucket_result.error);
      char* errorMessagePtr = error_result.message;
      status = napi_reject_deferred(env, obj->deferred,
      createError(env, error_result.code, errorMessagePtr));
    }else {
    Bucket bucket = *(bucket_result.bucket);
    char* bucketNamePtr = bucket.name;
    int64_t bucketCreated = bucket.created;
    napi_value bucketNAPI = createBucketResult(env, "bucket",
    bucketCreated, bucketNamePtr);
    status = napi_resolve_deferred(env, obj->deferred, bucketNAPI);
  }
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to return promise");
  }
  napi_delete_async_work(env, obj->work);
  free(obj);
}

void ListBucketsPromiseComplete(napi_env env, napi_status status, void* data) {
    ListBucketsPromiseObj* obj = (ListBucketsPromiseObj*)data;

    BucketIterator* bucket_resultIterator = obj->bucket_resultIterator;

    napi_value BucketList, errorObject, returnObject;

    status = napi_create_object(env, &BucketList);
    assert(status == napi_ok);

    status = napi_create_object(env, &errorObject);
    assert(status == napi_ok);

    status = napi_create_object(env, &returnObject);
    assert(status == napi_ok);

    typedef bool (*BucketIteratorNextFUNC)(BucketIterator*);
    BucketIteratorNextFUNC bucket_iterator_next =
        (BucketIteratorNextFUNC)GetProcAddress(hGetProcIDDLL,
            "bucket_iterator_next");

    if (!bucket_iterator_next) {
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
      int count = 0;
      while (bucket_iterator_next(bucket_resultIterator)) {
            typedef Bucket* (*BucketIteratorItemFUNC)(BucketIterator*);
            BucketIteratorItemFUNC bucket_iterator_item =
                (BucketIteratorItemFUNC)GetProcAddress(hGetProcIDDLL,
                    "bucket_iterator_item");
            if (!bucket_iterator_item) {
                napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
            } else {
                Bucket* bucket_result =
                    bucket_iterator_item(bucket_resultIterator);

                char* bucketNamePtr = bucket_result->name;

                napi_value BucketInfoObj = createBucketResult(env, "bucket",
                    bucket_result->created, bucketNamePtr);

                char str[100];
                itoa(count, str, 10);

                status = napi_set_named_property(env, BucketList, str,
                    BucketInfoObj);
                assert(status == napi_ok);
                typedef void(*FreeBucketFUNC)(Bucket*);
                FreeBucketFUNC free_bucket =
                    (FreeBucketFUNC)GetProcAddress(hGetProcIDDLL,
                        "free_bucket");
                if (!free_bucket) {
                    napi_throw_type_error(env,
                        nullptr, "\nLibrary not found\n");
                } else {
                    free_bucket(bucket_result);
                }
                count++;
            }
        }
    }

    typedef Error
        * (*BucketIteratorErrorFUNC)(BucketIterator*);
    BucketIteratorErrorFUNC bucket_iterator_err =
        (BucketIteratorErrorFUNC)GetProcAddress(hGetProcIDDLL,
            "bucket_iterator_err");
    if (!bucket_iterator_err) {
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        Error* err = bucket_iterator_err(bucket_resultIterator);
        if (err != NULL) {
            //
            Error error_result = *(err);
            char* errorMessagePtr = error_result.message;
            status = napi_reject_deferred(env, obj->deferred,
                createError(env, error_result.code, errorMessagePtr));
        } else {
            status = napi_set_named_property(env, returnObject, "bucketList",
                BucketList);
            assert(status == napi_ok);
            status = napi_resolve_deferred(env, obj->deferred, returnObject);
        }

        if (status != napi_ok) {
            napi_throw_error(env, NULL, "Failed to return promise");
        }
    }

    napi_delete_async_work(env, obj->work);
    free(obj);
}

void closeProjectPromiseComplete(napi_env env, napi_status status, void* data) {
  closeProjectPromiseObj *obj = (closeProjectPromiseObj*)data;
  Error* error_result = obj->error_result;
  if (error_result != NULL) {
    Error errorResult = *(error_result);
    char* errorMessagePtr = errorResult.message;
    status = napi_reject_deferred(env, obj->deferred,
    createError(env, errorResult.code, errorMessagePtr));
  } else {
      napi_value undefined;
      status = napi_get_undefined(env, &undefined);
      assert(status == napi_ok);
      //
      status = napi_resolve_deferred(env, obj->deferred, undefined);
  }
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to return promise");
  }
  napi_delete_async_work(env, obj->work);
  free(obj);
}

void configOpenProjectPromiseComplete(napi_env env,
napi_status status, void* data) {
  configOpenProjectPromiseObj *obj = (configOpenProjectPromiseObj*)data;
  ProjectResult project_result = obj->project_Result;
  if (project_result.project == NULL) {
    if (project_result.error != NULL) {
      Error error_result = *(project_result.error);
      char* errorMessagePtr = error_result.message;
      status = napi_reject_deferred(env, obj->deferred,
      createError(env, error_result.code, errorMessagePtr));
    }
  } else {
    Project project = *(project_result.project);
    size_t handlevalue = project._handle;

    napi_value projectNAPIObj = createResult(env, "project", handlevalue);

    status = napi_resolve_deferred(env, obj->deferred, projectNAPIObj);
  }
//
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to return promise");
  }
//
  napi_delete_async_work(env, obj->work);
  free(obj);
}

void ParseAccessPromiseComplete(napi_env env,
    napi_status status, void* data) {
    ParseAccessPromiseObj* obj = (ParseAccessPromiseObj*)data;
    AccessResult access_Result = obj->access_Result;
    if (access_Result.access == NULL) {
        if (access_Result.error != NULL) {
            Error error_result = *(access_Result.error);
            char* errorMessagePtr = error_result.message;
            char blank[] = "";
            if (errorMessagePtr == NULL) { errorMessagePtr = &blank[0]; }
            status = napi_reject_deferred(env, obj->deferred,
                createError(env, error_result.code, errorMessagePtr));
        }
    } else {
        Access access = *(access_Result.access);
        size_t handlevalue = access._handle;

        napi_value AccessNAPIObj = createResult(env, "access", handlevalue);
 
        //
        status = napi_resolve_deferred(env, obj->deferred, AccessNAPIObj);
    }

    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Failed to return promise");
    }

    napi_delete_async_work(env, obj->work);
    free(obj);
}

void ShareAccessPromiseComplete(napi_env env, napi_status status, void* data) {
    AccessSharePromiseObj* obj = (AccessSharePromiseObj*)data;
    AccessResult access_Result = obj->access_Result;
    if (access_Result.access == NULL) {
        if (access_Result.error != NULL) {
            Error error_result = *(access_Result.error);
            char* errorMessagePtr = error_result.message;
            char blank[] = "";
            if (errorMessagePtr == NULL) { errorMessagePtr = &blank[0]; }
            status = napi_reject_deferred(env, obj->deferred,
            createError(env, error_result.code, errorMessagePtr));
        }
    } else {
        Access access = *(access_Result.access);
        size_t handlevalue = access._handle;

        napi_value AccessNAPIObj = createResult(env, "access", handlevalue);

        status = napi_resolve_deferred(env, obj->deferred, AccessNAPIObj);
    }
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Failed to return promise");
    }
    napi_delete_async_work(env, obj->work);
    free(obj);
}

void ConfigRequestAccessWithEncryptionPromiseComplete(napi_env env,
napi_status status, void* data) {
    ConfigRequestAccessPromiseObj* obj = (ConfigRequestAccessPromiseObj*)data;
    AccessResult access_Result = obj->access_Result;
    if (access_Result.access == NULL) {
        if (access_Result.error != NULL) {
            Error error_result = *(access_Result.error);
            char* errorMessagePtr = error_result.message;
            char blank[] = "";
            if (errorMessagePtr == NULL) { errorMessagePtr = &blank[0]; }
            status = napi_reject_deferred(env, obj->deferred,
            createError(env, error_result.code, errorMessagePtr));
        }
    } else {
        Access access = *(access_Result.access);
        size_t handlevalue = access._handle;

        napi_value AccessNAPIObj = createResult(env, "access", handlevalue);
        
        //
        status = napi_resolve_deferred(env, obj->deferred, AccessNAPIObj);
    }

    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Failed to return promise");
    }

    napi_delete_async_work(env, obj->work);
    free(obj);
}

void RequestAccessWithEncryptionPromiseComplete(napi_env env,
napi_status status, void* data) {
    RequestAccessPromiseObj* obj = (RequestAccessPromiseObj*)data;
    AccessResult access_Result = obj->access_Result;
    if (access_Result.access == NULL) {
        if (access_Result.error != NULL) {
            Error error_result = *(access_Result.error);
            char* errorMessagePtr = error_result.message;
            char blank[] = "";
            if (errorMessagePtr == NULL) { errorMessagePtr = &blank[0]; }
            status = napi_reject_deferred(env, obj->deferred,
            createError(env, error_result.code, errorMessagePtr));
        }
    } else {
        Access access = *(access_Result.access);
        size_t handlevalue = access._handle;

        napi_value AccessNAPIObj = createResult(env, "access", handlevalue);
 
        status = napi_resolve_deferred(env, obj->deferred, AccessNAPIObj
        
        );
    }
    //
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Failed to return promise");
    }
    //
    napi_delete_async_work(env, obj->work);
    free(obj);
}

void accessSerializePromiseComplete(napi_env env,
napi_status status, void* data) {
    accessSerializePromiseObj* obj = (accessSerializePromiseObj*)data;
    StringResult string_result = obj->string_result;

    if (string_result.error != NULL) {
        Error errorResult = *(string_result.error);
        char* errorMessagePtr = errorResult.message;
        char blank[] = "";
        if (errorMessagePtr == NULL) { errorMessagePtr = &blank[0]; }
        status = napi_reject_deferred(env, obj->deferred, createError(env,
        errorResult.code, errorMessagePtr));
    } else {
        napi_value stringNAPI;
        char* StringResultPtr = string_result.string;
        //
        status = napi_create_string_utf8(env, StringResultPtr,
        NAPI_AUTO_LENGTH, &stringNAPI);
        assert(status == napi_ok);
        //
        status = napi_resolve_deferred(env, obj->deferred, stringNAPI);
    }
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Failed to return promise");
    }
    napi_delete_async_work(env, obj->work);
    free(obj);
}
