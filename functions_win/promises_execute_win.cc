#include "promises_execute_win.h"
#include <string>

HINSTANCE hGetProcIDDLL = LoadLibrary("../libuplinkc.dll");


void openProjectPromiseExecute(napi_env env, void* data) {
    openProjectPromiseObj* obj =
        reinterpret_cast<openProjectPromiseObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found \n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "open_project");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found \n");
        } else {
            typedef ProjectResult(__stdcall* pICProjectResult)(Access*);
            pICProjectResult open_project;
            open_project = pICProjectResult(fn);
            obj->project_Result = open_project(&(obj->access));
        }
    }
}

void ListObjectsPromiseExecute(napi_env env, void* data) {
    listObjectsPromiseObj* obj =
        reinterpret_cast<listObjectsPromiseObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found \n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "list_objects");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found \n");
        } else {
            typedef ObjectIterator* (__stdcall* pICObjectIterator)(Project*,
                char*, ListObjectsOptions*);
            pICObjectIterator list_objects;
            list_objects = pICObjectIterator(fn);
         //
            if (obj->listObjectSet == 0) {
                obj->objectIterator = list_objects(&(obj->project_result),
                obj->bucketName,
                    NULL);
            } else {
                obj->objectIterator = list_objects(&(obj->project_result),
                obj->bucketName, &(obj->list_object_options));
            }
        }
    }
}

void downloadInfoPromiseExecute(napi_env env, void* data) {
    downloadInfoObj* obj =
        reinterpret_cast<downloadInfoObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found \n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "download_info");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found \n");
        } else {
            typedef ObjectResult(__stdcall* pICObjectResult)(Download*);
            pICObjectResult download_info;
            download_info = pICObjectResult(fn);
            obj->object_result = download_info(&obj->download_result);
        }
    }
}


void downloadClosePromiseExecute(napi_env env, void* data) {
    downloadCloseObj* obj =
        reinterpret_cast<downloadCloseObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found \n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "close_download");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\n Function not found \n");
        } else {
            typedef Error* (__stdcall* pICError)(Download*);
            pICError close_download;
            close_download = pICError(fn);
            obj->error_result = close_download(&obj->download_result);
        }
    }
}


void downloadReadPromiseExecute(napi_env env, void* data) {
    downloadReadObj* obj = reinterpret_cast<downloadReadObj*>(data);
    uint8_t *ptrToData;
    ptrToData = reinterpret_cast<uint8_t *>(obj->bufferPtr);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found \n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "download_read");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found \n");
        } else {
            typedef ReadResult(__stdcall* pICReadResult)(Download*,
                uint8_t*, size_t);
            pICReadResult download_read;
            download_read = pICReadResult(fn);

            obj->read_result = download_read(&obj->download_result,
                ptrToData, obj->bufferlength);
        }
    }
}

void downloadObjectPromiseExecute(napi_env env, void* data) {
    downloadObjectObj* obj = reinterpret_cast<downloadObjectObj *>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "download_object");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found\n");
        } else {
            typedef DownloadResult(__stdcall* pICDownloadResult)(Project*,
                char*, char*, DownloadOptions*);
            pICDownloadResult download_object;
            download_object = pICDownloadResult(fn);
            obj->download_result = download_object(&obj->project,
                obj->bucketname, obj->objectkey, NULL);
        }
    }
}

void uploadSetMetaPromiseExecute(napi_env env, void* data) {
    uploadSetMetaObj* obj = reinterpret_cast<uploadSetMetaObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),
            "upload_set_custom_metadata");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found\n");
        } else {
            typedef Error* (__stdcall* pICError)(Upload*, CustomMetadata);
            pICError upload_set_custom_metadata;
            upload_set_custom_metadata = pICError(fn);
            obj->error_result =
                upload_set_custom_metadata(&obj->upload_result,
                    obj->customMetadata);
        }
    }
}

void uploadAbortPromiseExecute(napi_env env, void* data) {
    uploadAbortPromiseObj* obj =
        reinterpret_cast<uploadAbortPromiseObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found \n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "upload_abort");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found \n");
        } else {
            typedef Error* (__stdcall* pICError)(Upload*);
            pICError upload_abort;
            upload_abort = pICError(fn);
            obj->error_result = upload_abort(&obj->upload_result);
        }
    }
}

void uploadInfoPromiseExecute(napi_env env, void* data) {
    uploadInfoObj* obj = reinterpret_cast<uploadInfoObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "upload_info");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found\n");
        } else {
            typedef ObjectResult(__stdcall* pICObjectResult)(Upload*);
            pICObjectResult upload_info;
            upload_info = pICObjectResult(fn);
            obj->object_result = upload_info(&obj->upload_result);
        }
    }
}

void uploadCommitPromiseExecute(napi_env env, void* data) {
    uploadCommitObj* obj = reinterpret_cast<uploadCommitObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found \n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "upload_commit");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found\n");
        } else {
            typedef Error* (__stdcall* pICError)(Upload*);
            pICError upload_commit;
            upload_commit = pICError(fn);
            obj->error_result = upload_commit(&(obj->upload_result));
        }
    }
}

void uploadWritePromiseExecute(napi_env env, void* data) {
    uploadWriteObj* obj = (uploadWriteObj*)data;
    uint8_t* ptrToData;
    ptrToData = reinterpret_cast<uint8_t*>(obj->bufferPtr);

    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "upload_write");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found\n");
        } else {
            typedef WriteResult(__stdcall* pICWriteResult)(Upload*,
                uint8_t*, size_t);
            pICWriteResult upload_write;
            upload_write = pICWriteResult(fn);
            obj->write_result = upload_write(&(obj->upload_result),
                ptrToData, obj->bytesread);
        }
    }
}

void uploadObjectExecute(napi_env env, void* data) {
    uploadobjectObj* obj = (uploadobjectObj*)data;
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "upload_object");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found\n");
        } else {
            typedef UploadResult(__stdcall* pICUploadResult)(Project*,
                char*, char*, UploadOptions*);
            pICUploadResult upload_object;
            upload_object = pICUploadResult(fn);
            if (obj->uploadoptionSet == 0) {
                obj->upload_result = upload_object(&(obj->project),
                obj->bucketname, obj->objectkey, NULL);
            } else {
                obj->upload_result = upload_object(&(obj->project),
                obj->bucketname, obj->objectkey, &(obj->uploadOptions));
            }
        }
    }
}

void stateObjectPromiseExecute(napi_env env, void* data) {
    objectOperationObj* obj = reinterpret_cast<objectOperationObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "stat_object");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found\n");
        } else {
            typedef ObjectResult(__stdcall* pICObjectResult)(Project*,
                char*, char*);
            pICObjectResult stat_object;
            stat_object = pICObjectResult(fn);
            obj->object_result = stat_object(&(obj->project),
                obj->bucketname, obj->objectkey);
        }
    }
}

void deleteObjectPromiseExecute(napi_env env, void* data) {
    objectOperationObj* obj = reinterpret_cast<objectOperationObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found \n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "delete_object");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found \n");
        } else {
            typedef ObjectResult(__stdcall* pICObjectResult)(Project*,
                char*, char*);
            pICObjectResult delete_object;
            delete_object = pICObjectResult(fn);
            obj->object_result = delete_object(&(obj->project),
                obj->bucketname, obj->objectkey);
        }
    }
}

void stateBucketPromiseExecute(napi_env env, void* data) {
    bucketOperationObj* obj = reinterpret_cast<bucketOperationObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "stat_bucket");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found\n");
        } else {
            typedef BucketResult(__stdcall* pICBucketResult)(Project*,
                char*);
            pICBucketResult stat_bucket;
            stat_bucket = pICBucketResult(fn);
            obj->bucket_Result =
                stat_bucket(&(obj->project), obj->bucketname);
        }
    }
}

void createBucketPromiseExecute(napi_env env, void* data) {
    bucketOperationObj* obj = reinterpret_cast<bucketOperationObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "create_bucket");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found\n");
        } else {
            typedef BucketResult(__stdcall* pICBucketResult)(Project*,
                char*);
            pICBucketResult create_bucket;
            create_bucket = pICBucketResult(fn);
            obj->bucket_Result =
                create_bucket(&(obj->project), obj->bucketname);
        }
    }
}
//
void ensureBucketPromiseExecute(napi_env env, void* data) {
    bucketOperationObj* obj = reinterpret_cast<bucketOperationObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "ensure_bucket");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFound not found\n");
        } else {
            typedef BucketResult(__stdcall* pICBucketResult)(Project*,
                char*);
            pICBucketResult ensure_bucket;
            ensure_bucket = pICBucketResult(fn);
            obj->bucket_Result =
                ensure_bucket(&(obj->project), obj->bucketname);
        }
    }
}

void deleteBucketPromiseExecute(napi_env env, void* data) {
    bucketOperationObj* obj = reinterpret_cast<bucketOperationObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "delete_bucket");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found\n");
        } else {
            typedef BucketResult(__stdcall* pICBucketResult)(Project*,
                char*);
            pICBucketResult delete_bucket;
            delete_bucket = pICBucketResult(fn);
            obj->bucket_Result =
                delete_bucket(&(obj->project), obj->bucketname);
        }
    }
}

void ListBucketsPromiseExecute(napi_env env, void* data) {
    ListBucketsPromiseObj* obj =
        reinterpret_cast<ListBucketsPromiseObj*>(data);

    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "list_buckets");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found\n");
        } else {
            typedef BucketIterator* (__stdcall* pICBucketIterator)(Project*,
                ListBucketsOptions*);
            pICBucketIterator list_buckets;
            list_buckets = pICBucketIterator(fn);
            if (obj->listBucketOptionSet == 0) {
                obj->bucket_resultIterator = list_buckets(&(obj->project_result),
                    NULL);
            } else {
                obj->bucket_resultIterator = list_buckets(&(obj->project_result),
                    &(obj->listBucketsOptions));
            }

        }
    }
}

void closeProjectPromiseExecute(napi_env env, void* data) {
    closeProjectPromiseObj* obj = reinterpret_cast
    <closeProjectPromiseObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "close_project");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found\n");
        } else {
            typedef Error* (__stdcall* pICError)(Project*);
            pICError close_project;
            close_project = pICError(fn);
            obj->error_result = close_project(&(obj->project_result));
        }
    }
}

void configOpenProjectPromiseExecute(napi_env env, void* data) {
    configOpenProjectPromiseObj* obj = reinterpret_cast
    <configOpenProjectPromiseObj *>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found \n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),
            "config_open_project");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found \n");
        } else {
            typedef ProjectResult(__stdcall* pICProjectResult)(Config,
                Access*);
            pICProjectResult config_open_project;
            config_open_project = pICProjectResult(fn);
            obj->project_Result = config_open_project(obj->config,
            &(obj->access));
        }
    }
}

void ParseAccess(napi_env env, void* data) {
    ParseAccessPromiseObj* obj = reinterpret_cast<ParseAccessPromiseObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "parse_access");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found \n");
        } else {
            typedef AccessResult(__stdcall* pICAccessResult)(char*);
            pICAccessResult parse_access;
            parse_access = pICAccessResult(fn);
            obj->access_Result = parse_access(obj->accessString);
        }
    }
}

void ShareAccessPromiseExecute(napi_env env, void* data) {
    AccessSharePromiseObj* obj = reinterpret_cast<AccessSharePromiseObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found \n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL), "access_share");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found \n");
        } else {
            typedef AccessResult(__stdcall* pICAccessResult)(Access*,
                Permission, SharePrefix*, int64_t);
            pICAccessResult access_share;
            access_share = pICAccessResult(fn);
            obj->access_Result = access_share(&(obj->access), obj->permission,
                obj->SharePrefixListPointer, obj->SharePrefixSize);
        }
    }
}

void accessSerializePromiseExecute(napi_env env, void* data) {
    accessSerializePromiseObj* obj =
        reinterpret_cast<accessSerializePromiseObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),
            "access_serialize");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found\n");
        } else {
            typedef StringResult(__stdcall* pICStringResult)(Access*);
            pICStringResult access_serialize;
            access_serialize = pICStringResult(fn);
            obj->string_result = access_serialize(&(obj->access));
        }
    }
}

void ConfigRequestAccessWithEncryption(napi_env env, void* data) {
    ConfigRequestAccessPromiseObj* obj = reinterpret_cast
    <ConfigRequestAccessPromiseObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),
            "config_request_access_with_passphrase");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found\n");
        } else {
            typedef AccessResult(__stdcall* pICAccessResult)(Config,
                char*, char*, char*);
            pICAccessResult config_request_access_with_passphrase;
            config_request_access_with_passphrase = pICAccessResult(fn);
            obj->access_Result =
            config_request_access_with_passphrase(obj->config,
            obj->satellite_address, obj->api_key, obj->passphrase);
        }
    }
}

void RequestAccessWithEncryption(napi_env env, void* data) {
    RequestAccessPromiseObj* obj =
        reinterpret_cast<RequestAccessPromiseObj*>(data);
    if (!hGetProcIDDLL) {
        free(obj);
        napi_throw_type_error(env, nullptr, "\nLibrary not found\n");
    } else {
        FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),
            "request_access_with_passphrase");
        if (!fn) {
            free(obj);
            napi_throw_type_error(env, nullptr, "\nFunction not found\n");
        } else {
            typedef AccessResult(__stdcall* pICAccessResult)(char*,
                char*, char*);
            pICAccessResult request_access_with_passphrase;
            request_access_with_passphrase = pICAccessResult(fn);
            obj->access_Result =
                request_access_with_passphrase(obj->satellite_address,
                    obj->api_key, obj->passphrase);
        }
    }
}
