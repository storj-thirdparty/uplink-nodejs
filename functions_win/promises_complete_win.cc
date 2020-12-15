#include "promises_complete_win.h"
#include <string>

void openProjectPromiseComplete(napi_env env, napi_status status, void* data) {
    openProjectPromiseObj* obj = (openProjectPromiseObj*)data;
    UplinkProjectResult project_result = obj->project_Result;
    if (project_result.project == NULL) {
        if (project_result.error != NULL) {
            UplinkError error_result = *(project_result.error);
            char* errorMessagePtr = error_result.message;
            char blank[] = "";
            if (errorMessagePtr == NULL) { errorMessagePtr = &blank[0]; }
            status = napi_reject_deferred(env, obj->deferred,
            createError(env, error_result.code, errorMessagePtr));
        }
    } else {
        UplinkProject project = *(project_result.project);
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
    typedef bool (*ObjectIteratorNextFUNC)(UplinkObjectIterator*);
    ObjectIteratorNextFUNC object_iterator_next =
        (ObjectIteratorNextFUNC)GetProcAddress(hGetProcIDDLL,
            "uplink_object_iterator_next");
    if (!object_iterator_next) {
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
      int count = 0;
      while (object_iterator_next(obj->objectIterator)) {
            typedef UplinkObject *(*BucketIteratorItemFUNC)(UplinkObjectIterator*);
            BucketIteratorItemFUNC object_iterator_item =
                (BucketIteratorItemFUNC)GetProcAddress(hGetProcIDDLL,
                    "uplink_object_iterator_item");
            if (!object_iterator_item) {
                napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
            } else {
                UplinkObject* objectPtr = object_iterator_item(obj->objectIterator);
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

    typedef UplinkError* (*ObjectIteratorErrFUNC)(UplinkObjectIterator*);
    ObjectIteratorErrFUNC object_iterator_err =
        (ObjectIteratorErrFUNC)GetProcAddress(hGetProcIDDLL,
            "uplink_object_iterator_err");
    if (!object_iterator_err) {
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        UplinkError* err = object_iterator_err(obj->objectIterator);
        if (err != NULL) {
            UplinkError errorResult = *(err);
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
  UplinkObjectResult object_result = obj->object_result;
  //
  if (object_result.object == NULL) {
    if (object_result.error != NULL) {
      UplinkError error_result = *(object_result.error);
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

void downloadCloseOperationComplete(napi_env env,
napi_status status, void* data) {
  downloadCloseObj *obj = (downloadCloseObj*)data;

  if (obj->error_result != NULL) {
      UplinkError error_result = *(obj->error_result);
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
  UplinkReadResult read_result = obj->read_result;

  if (read_result.error != NULL) {
      UplinkError error_result = *(read_result.error);
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
    UplinkDownloadResult download_result = obj->download_result;
    if (download_result.download == NULL) {
        if (download_result.error != NULL) {
            UplinkError error_result = *(download_result.error);
            char* errorMessagePtr = error_result.message;
            char blank[] = "";
            if (errorMessagePtr == NULL) { errorMessagePtr = &blank[0]; }
            status = napi_reject_deferred(env, obj->deferred,
            createError(env, error_result.code, errorMessagePtr));
        }
    } else {
        UplinkDownload download = *(download_result.download);
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
  UplinkError* error_result = obj->error_result;
  if (error_result != NULL) {
      UplinkError errorResult = *(error_result);
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
  UplinkError* error_result = obj->error_result;
  if (error_result != NULL) {
    UplinkError errorResult = *(error_result);
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
  UplinkObjectResult object_result = obj->object_result;

  if (object_result.object == NULL) {
    if (object_result.error != NULL) {
      UplinkError error_result = *(object_result.error);
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
      UplinkError error_result = *(obj->error_result);
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
  UplinkWriteResult write_result = obj->write_result;

  if (write_result.error != NULL) {
      UplinkError error_result = *(write_result.error);
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
    UplinkUploadResult upload_result = obj->upload_result;
    if (upload_result.upload == NULL) {
        if (upload_result.error != NULL) {
            UplinkError error_result = *(upload_result.error);
            char* errorMessagePtr = error_result.message;
            char blank[] = "";
            if (errorMessagePtr == NULL) { errorMessagePtr = &blank[0]; }
            status = napi_reject_deferred(env, obj->deferred,
            createError(env, error_result.code, errorMessagePtr));
        }
    } else {
        UplinkUpload upload = *(upload_result.upload);
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
  napi_value objectNAPI;
  objectOperationObj *obj = (objectOperationObj*)data;
  UplinkObjectResult object_result = obj->object_result;
  //
  if (object_result.error != NULL) {
    UplinkError error_result = *(object_result.error);
    char* errorMessagePtr = error_result.message;
    status = napi_reject_deferred(env, obj->deferred,
    createError(env, error_result.code, errorMessagePtr));
  }
  else if (object_result.object != NULL) {
    objectNAPI = createObjectResult(env, object_result.object);
    status = napi_resolve_deferred(env, obj->deferred, objectNAPI);
  }
  else {
    UplinkObject uplinkobject;
    uplinkobject.key=const_cast<char*>("");
    uplinkobject.is_prefix=false;
    uplinkobject.system.created=0;
    uplinkobject.system.expires=0;
    uplinkobject.system.content_length=0;
    uplinkobject.custom.entries=NULL;
    uplinkobject.custom.count=0;
    objectNAPI = createObjectResult(env, &uplinkobject);
    status = napi_resolve_deferred(env, obj->deferred, objectNAPI);
  }
  if (status != napi_ok) {
      napi_throw_error(env, NULL, "Failed to return promise");
  }
  napi_delete_async_work(env, obj->work);
  free(obj);
}

void bucketOperationComplete(napi_env env, napi_status status, void* data) {
  napi_value bucketNAPI;
  bucketOperationObj *obj = (bucketOperationObj*)data;
  UplinkBucketResult bucket_result = obj->bucket_Result;
  //
  if (bucket_result.error != NULL) {
    UplinkError error_result = *(bucket_result.error);
    char* errorMessagePtr = error_result.message;
    status = napi_reject_deferred(env, obj->deferred,
    createError(env, error_result.code, errorMessagePtr));
  } 
  else if (bucket_result.bucket != NULL) {
    UplinkBucket bucket = *(bucket_result.bucket);
    char* bucketNamePtr = bucket.name;
    int64_t bucketCreated = bucket.created;
    bucketNAPI = createBucketResult(env, "bucket",
    bucketCreated, bucketNamePtr);
    status = napi_resolve_deferred(env, obj->deferred, bucketNAPI);
  }
  else {
    //
    UplinkBucket bucket;
    bucket.name=const_cast<char*>("");
    bucket.created=0;
    bucketNAPI = createBucketResult(env,"bucket",
    bucket.created, bucket.name);
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

    UplinkBucketIterator* bucket_resultIterator = obj->bucket_resultIterator;

    napi_value BucketList, errorObject, returnObject;

    status = napi_create_object(env, &BucketList);
    assert(status == napi_ok);

    status = napi_create_object(env, &errorObject);
    assert(status == napi_ok);

    status = napi_create_object(env, &returnObject);
    assert(status == napi_ok);
    typedef bool (*BucketIteratorNextFUNC)(UplinkBucketIterator*);
    BucketIteratorNextFUNC bucket_iterator_next =
        (BucketIteratorNextFUNC)GetProcAddress(hGetProcIDDLL,
            "uplink_bucket_iterator_next");
    if (!bucket_iterator_next) {
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
      int count = 0;
      while (bucket_iterator_next(bucket_resultIterator)) {
            typedef UplinkBucket* (*BucketIteratorItemFUNC)(UplinkBucketIterator*);
            BucketIteratorItemFUNC bucket_iterator_item =
                (BucketIteratorItemFUNC)GetProcAddress(hGetProcIDDLL,
                    "uplink_bucket_iterator_item");
            if (!bucket_iterator_item) {
                napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
            } else {
                UplinkBucket* bucket_result =
                    bucket_iterator_item(bucket_resultIterator);
                char* bucketNamePtr = bucket_result->name;

                napi_value BucketInfoObj = createBucketResult(env, "bucket",
                    bucket_result->created, bucketNamePtr);

                char str[100];
                itoa(count, str, 10);

                status = napi_set_named_property(env, BucketList, str,
                    BucketInfoObj);
                assert(status == napi_ok);

                count++;
            }
        }
    }
    typedef UplinkError* (*BucketIteratorErrorFUNC)(UplinkBucketIterator*);
    BucketIteratorErrorFUNC bucket_iterator_err =
        (BucketIteratorErrorFUNC)GetProcAddress(hGetProcIDDLL,
            "uplink_bucket_iterator_err");
    if (!bucket_iterator_err) {
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        UplinkError* err = bucket_iterator_err(bucket_resultIterator);
        if (err != NULL) {
            //
            UplinkError error_result = *(err);
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
  UplinkError* error_result = obj->error_result;
  if (error_result != NULL) {
    UplinkError errorResult = *(error_result);
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
  UplinkProjectResult project_result = obj->project_Result;
  if (project_result.project == NULL) {
    if (project_result.error != NULL) {
      UplinkError error_result = *(project_result.error);
      char* errorMessagePtr = error_result.message;
      status = napi_reject_deferred(env, obj->deferred,
      createError(env, error_result.code, errorMessagePtr));
    }
  } else {
    UplinkProject project = *(project_result.project);
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
    UplinkAccessResult access_Result = obj->access_Result;
    if (access_Result.access == NULL) {
        if (access_Result.error != NULL) {
            UplinkError error_result = *(access_Result.error);
            char* errorMessagePtr = error_result.message;
            char blank[] = "";
            if (errorMessagePtr == NULL) { errorMessagePtr = &blank[0]; }
            status = napi_reject_deferred(env, obj->deferred,
                createError(env, error_result.code, errorMessagePtr));
        }
    } else {
        UplinkAccess access = *(access_Result.access);
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
    UplinkAccessResult access_Result = obj->access_Result;
    if (access_Result.access == NULL) {
        if (access_Result.error != NULL) {
            UplinkError error_result = *(access_Result.error);
            char* errorMessagePtr = error_result.message;
            char blank[] = "";
            if (errorMessagePtr == NULL) { errorMessagePtr = &blank[0]; }
            status = napi_reject_deferred(env, obj->deferred,
            createError(env, error_result.code, errorMessagePtr));
        }
    } else {
        UplinkAccess access = *(access_Result.access);
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
    UplinkAccessResult access_Result = obj->access_Result;
    if (access_Result.access == NULL) {
        if (access_Result.error != NULL) {
            UplinkError error_result = *(access_Result.error);
            char* errorMessagePtr = error_result.message;
            char blank[] = "";
            if (errorMessagePtr == NULL) { errorMessagePtr = &blank[0]; }
            status = napi_reject_deferred(env, obj->deferred,
            createError(env, error_result.code, errorMessagePtr));
        }
    } else {
        UplinkAccess access = *(access_Result.access);
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
    UplinkAccessResult access_Result = obj->access_Result;
    if (access_Result.access == NULL) {
        if (access_Result.error != NULL) {
            UplinkError error_result = *(access_Result.error);
            char* errorMessagePtr = error_result.message;
            char blank[] = "";
            if (errorMessagePtr == NULL) { errorMessagePtr = &blank[0]; }
            status = napi_reject_deferred(env, obj->deferred,
            createError(env, error_result.code, errorMessagePtr));
        }
    } else {
        UplinkAccess access = *(access_Result.access);
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
    UplinkStringResult string_result = obj->string_result;

    if (string_result.error != NULL) {
        UplinkError errorResult = *(string_result.error);
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

void deriveEncrpPromiseComplete(napi_env env,
napi_status status, void* data) {
    deriveEncrpPromiseObj* obj = (deriveEncrpPromiseObj*)data;
    UplinkEncryptionKeyResult encryption_Result = obj->encryptionResult;
    if (encryption_Result.encryption_key == NULL) {
        if (encryption_Result.error != NULL) {
            UplinkError error_result = *(encryption_Result.error);
            char* errorMessagePtr = error_result.message;
            char blank[] = "";
            if (errorMessagePtr == NULL) { errorMessagePtr = &blank[0]; }
            status = napi_reject_deferred(env, obj->deferred,
            createError(env, error_result.code, errorMessagePtr));
        }
    } else {
        UplinkEncryptionKey encryption_key = *(encryption_Result.encryption_key);
        size_t handlevalue = encryption_key._handle;

        napi_value EncryptionNAPIObj = createResult(env, "encryption_key", handlevalue);
 
        status = napi_resolve_deferred(env, obj->deferred, EncryptionNAPIObj
        
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

void accessOverRidePromiseComplete(napi_env env,
napi_status status, void* data) {
    accessOverRidePromiseObj *obj = (accessOverRidePromiseObj*)data;

  if (obj->error_result != NULL) {
      UplinkError error_result = *(obj->error_result);
      char* errorMessagePtr = error_result.message;
      char blank[] = "";
      if (errorMessagePtr == NULL) {errorMessagePtr = &blank[0];}
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
