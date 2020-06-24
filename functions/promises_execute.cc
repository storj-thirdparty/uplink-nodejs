#include "promises_execute.h"

void openProjectPromiseExecute(napi_env env, void* data){
  openProjectPromiseObj *obj = (openProjectPromiseObj*)data;
  ProjectResult project_result = open_project(&(obj->access));
  obj->project_Result = project_result;
}

void listObjectPromiseExecute(napi_env env, void* data){
  listObjectPromiseObj *obj = (listObjectPromiseObj*)data;
  if(obj->listObjectSet==0) {
    obj->objectIterator = list_objects(&(obj->project_result),
          obj->bucketName, NULL);
  }else{
    obj->objectIterator = list_objects(&(obj->project_result), obj->bucketName,
          &(obj->list_object_options));
  }
}

void downloadInfoPromiseExecute(napi_env env, void* data){
  downloadInfoObj *obj = (downloadInfoObj*)data;
  ObjectResult object_result = download_info(&(obj->download_result));
  obj->object_result = object_result;
}


void downloadClosePromiseExecute(napi_env env, void* data){
  downloadCloseObj *obj = (downloadCloseObj*)data;
  obj->error_result = close_download(&(obj->download_result));
}


void downloadReadPromiseExecute(napi_env env, void* data){
  downloadReadObj *obj = (downloadReadObj*)data;
  uint8_t *ptrToData;
  ptrToData = reinterpret_cast<uint8_t *>(obj->bufferPtr);
  ReadResult read_result = download_read(&(obj->download_result),
    ptrToData, obj->bufferlength);
  obj->read_result = read_result;
}


void downloadObjectPromiseExecute(napi_env env, void* data){
  downloadObjectObj *obj = (downloadObjectObj*)data;
  DownloadResult download_result;
  if(obj->downloadoptionSet==0){
    download_result = download_object(&(obj->project), obj->bucketname,
          obj->objectkey, NULL);
  }else{
    download_result = download_object(&(obj->project), obj->bucketname,
          obj->objectkey, &(obj->downloadOption));
  }
  obj->download_result = download_result;
}

void uploadSetMetaPromiseExecute(napi_env env, void* data){
  uploadSetMetaObj *obj = (uploadSetMetaObj*)data;
  Error* error_result = upload_set_custom_metadata(
    &(obj->upload_result), obj->customMetadata);
  obj->error_result = error_result;
}

void uploadAbortPromiseExecute(napi_env env, void* data){
  uploadAbortPromiseObj *obj = (uploadAbortPromiseObj*)data;
  Error* error_result = upload_abort(&(obj->upload_result));
  obj->error_result = error_result;
}

void uploadInfoPromiseExecute(napi_env env, void* data){
  uploadInfoObj *obj = (uploadInfoObj*)data;
  ObjectResult object_result = upload_info(&(obj->upload_result));
  obj->object_result = object_result;
}

void uploadCommitPromiseExecute(napi_env env, void* data){
  uploadCommitObj *obj = (uploadCommitObj*)data;
  obj->error_result = upload_commit(&(obj->upload_result));
}

void uploadWritePromiseExecute(napi_env env, void* data){
  uploadWriteObj *obj = (uploadWriteObj*)data;
  uint8_t *ptrToData;
  ptrToData = reinterpret_cast<uint8_t *>(obj->bufferPtr);
  WriteResult write_result = upload_write(&(obj->upload_result),ptrToData, obj->bytesread);
  obj->write_result = write_result;
}

void uploadObjectExecute(napi_env env, void* data){
  uploadobjectObj *obj = (uploadobjectObj*)data;
  UploadResult upload_result;
  if(obj->uploadoptionSet==0){
    upload_result = upload_object(&(obj->project), obj->bucketname,
          obj->objectkey, NULL);
  }else{
    upload_result = upload_object(&(obj->project), obj->bucketname,
          obj->objectkey, &(obj->uploadOptions));
  }
  obj->upload_result = upload_result;
}

void stateObjectPromiseExecute(napi_env env, void* data){
  objectOperationObj *obj = (objectOperationObj*)data;
  ObjectResult object_result = stat_object(&(obj->project),obj->bucketname,obj->objectkey);
  obj->object_result = object_result;
}
//
void deleteObjectPromiseExecute(napi_env env, void* data){
  objectOperationObj *obj = (objectOperationObj*)data;
  ObjectResult object_result = delete_object(&(obj->project),obj->bucketname,obj->objectkey);
  obj->object_result = object_result;
}

void stateBucketPromiseExecute(napi_env env, void* data){
  bucketOperationObj *obj = (bucketOperationObj*)data;
  BucketResult bucket_result = stat_bucket(&(obj->project),obj->bucketname);
  obj->bucket_Result = bucket_result;
}
//
void createBucketPromiseExecute(napi_env env, void* data){
  bucketOperationObj *obj = (bucketOperationObj*)data;
  BucketResult bucket_result = create_bucket(&(obj->project),obj->bucketname);
  obj->bucket_Result = bucket_result;
}
//
void ensureBucketPromiseExecute(napi_env env, void* data){
  bucketOperationObj *obj = (bucketOperationObj*)data;
  BucketResult bucket_result = ensure_bucket(&(obj->project),obj->bucketname);
  obj->bucket_Result = bucket_result;
}
//
void deleteBucketPromiseExecute(napi_env env, void* data){
  bucketOperationObj *obj = (bucketOperationObj*)data;
  BucketResult bucket_result = delete_bucket(&(obj->project),obj->bucketname);
  obj->bucket_Result = bucket_result;
}

void ListBucketsPromiseExecute(napi_env env, void* data){
  ListBucketsPromiseObj *obj = (ListBucketsPromiseObj*)data;
  BucketIterator *bucket_resultIterator = list_buckets(&(obj->project_result), NULL);
  obj->bucket_resultIterator = bucket_resultIterator; 
}

void closeProjectPromiseExecute(napi_env env, void* data){
  closeProjectPromiseObj *obj = (closeProjectPromiseObj*)data;
  Error* error_result = close_project(&(obj->project_result));
  obj->error_result = error_result;
}

void configOpenProjectPromiseExecute(napi_env env, void* data){
  configOpenProjectPromiseObj *obj = (configOpenProjectPromiseObj*)data;
  ProjectResult project_result = config_open_project(obj->config, &(obj->access));
  obj->project_Result = project_result;
}

void ParseAccess(napi_env env, void* data) {
  ParseAccessPromiseObj* obj = (ParseAccessPromiseObj*)data;
  AccessResult access_Result = parse_access(obj->accessString);
  obj->access_Result = access_Result;
}

void ShareAccessPromiseExecute(napi_env env, void* data) {
  AccessSharePromiseObj* obj = (AccessSharePromiseObj*)data;
  AccessResult access_Result = access_share(&(obj->access), obj->permission,
  obj->SharePrefixListPointer, obj->SharePrefixSize);
  obj->access_Result = access_Result;
}

void ConfigRequestAccessWithEncryption(napi_env env, void* data){
  ConfigRequestAccessPromiseObj *obj = (ConfigRequestAccessPromiseObj*)data;
  AccessResult access_Result = config_request_access_with_passphrase(obj->config,obj->satellite_address,obj->api_key,obj->passphrase);
  obj->access_Result = access_Result;
}

void RequestAccessWithEncryption(napi_env env, void* data){
  RequestAccessPromiseObj *obj = (RequestAccessPromiseObj*)data;
  AccessResult access_Result = request_access_with_passphrase(obj->satellite_address,obj->api_key,obj->passphrase);
  obj->access_Result = access_Result;
}

void accessSerializePromiseExecute(napi_env env, void* data){
  accessSerializePromiseObj *obj = (accessSerializePromiseObj*)data;
  StringResult string_result = access_serialize(&(obj->access));
  obj->string_result = string_result;
}