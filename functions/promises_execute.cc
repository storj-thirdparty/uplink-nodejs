// Copyright 2020 Storj Storj
/** @mainpage Node-js bindings
 *  It uses napi for creating node module
 * 
 */
#include "promises_execute.h"
#include <string>
/*!
 \fn void openProjectPromiseExecute(napi_env env, void* data) 
 \brief openProjectPromiseExecute function called when async operation get 
 complete and convert c data type into NAPI type 
  
 */

void openProjectPromiseExecute(napi_env env, void* data) {
  openProjectPromiseObj *obj = (openProjectPromiseObj*)data;
  obj->project_Result = uplink_open_project(&(obj->access));
}
/*!
 \fn void listObjectPromiseExecute(napi_env env, void* data) 
 \brief listObjectPromiseExecute used to implement the uplink-c library function
 ListObjectPromiseExecute returns list of object using promise
 */
void listObjectPromiseExecute(napi_env env, void* data) {
  listObjectPromiseObj *obj = (listObjectPromiseObj*)data;
  if (obj->listObjectSet == 0) {
    obj->objectIterator = uplink_list_objects(&(obj->project_result),
          obj->bucketName, NULL);
  } else {
    obj->objectIterator = uplink_list_objects(&(obj->project_result), obj->bucketName,
          &(obj->list_object_options));
  }
}
/*!
 \fn void downloadInfoPromiseExecute(napi_env env, void* data) 
 \brief downloadInfoPromiseExecute used to implement the uplink-c library function
       downloadInfoPromiseExecute provides download information using promise
  
  
 */
void downloadInfoPromiseExecute(napi_env env, void* data) {
  downloadInfoObj *obj = (downloadInfoObj*)data;
  obj->object_result = uplink_download_info(&(obj->download_result));
}
/*!
 \fn void downloadClosePromiseExecute(napi_env env, void* data) 
 \brief downloadClosePromiseExecute used to implement the uplink-c library function
         downloadClosePromiseExecute close downloads using promise   
 */

void downloadClosePromiseExecute(napi_env env, void* data) {
  downloadCloseObj *obj = (downloadCloseObj*)data;
  obj->error_result = uplink_close_download(&(obj->download_result));
}
/*!
 \fn void downloadReadPromiseExecute(napi_env env, void* data) 
 \brief downloadReadPromiseExecute used to implement the uplink-c library function
         downloadReadPromiseExecute reads downloads using promise 

 */

void downloadReadPromiseExecute(napi_env env, void* data) {
  downloadReadObj *obj = (downloadReadObj*)data;
  uint8_t *ptrToData;
  ptrToData = reinterpret_cast<uint8_t *>(obj->bufferPtr);
  obj->read_result = uplink_download_read(&(obj->download_result),
    ptrToData, obj->bufferlength);
}

/*!
 \fn void downloadObjectPromiseExecute(napi_env env, void* data) 
 \brief downloadObjectPromiseExecute used to implement the uplink-c library function
         downloadClosePromiseExecute downloads objects using promise  
 */
void downloadObjectPromiseExecute(napi_env env, void* data) {
  downloadObjectObj *obj = (downloadObjectObj*)data;
  if (obj->downloadoptionSet == 0) {
    obj->download_result = uplink_download_object(&(obj->project), obj->bucketname,
          obj->objectkey, NULL);
  } else {
    obj->download_result = uplink_download_object(&(obj->project), obj->bucketname,
          obj->objectkey, &(obj->downloadOption));
  }
}
/*!
 \fn void uploadSetMetaPromiseExecute(napi_env env, void* data) 
 \brief uploadSetMetaPromiseExecute used to implement the uplink-c library function
         uploadSetMetaPromiseExecute uploads the metadata using limit

 */

void uploadSetMetaPromiseExecute(napi_env env, void* data) {
  uploadSetMetaObj *obj = (uploadSetMetaObj*)data;
  obj->error_result = uplink_upload_set_custom_metadata(
    &(obj->upload_result), obj->customMetadata);
}
/*!
 \fn void uploadAbortPromiseExecute(napi_env env, void* data) 
 \brief uploadAbortPromiseExecute used to implement the uplink-c library function
         uploadAbortPromiseExecute aborts the upload using promise

 */
void uploadAbortPromiseExecute(napi_env env, void* data) {
  uploadAbortPromiseObj *obj = (uploadAbortPromiseObj*)data;
  obj->error_result = uplink_upload_abort(&(obj->upload_result));
}
/*!
 \fn void uploadInfoPromiseExecute(napi_env env, void* data) 
 \brief uploadInfoPromiseExecute used to implement the uplink-c library function
         uploadInfoPromiseExecute uploads the information using promise

 */
void uploadInfoPromiseExecute(napi_env env, void* data) {
  uploadInfoObj *obj = (uploadInfoObj*)data;
  obj->object_result = uplink_upload_info(&(obj->upload_result));
}

/*!
 \fn void uploadCommitPromiseExecute(napi_env env, void* data) 
 \brief uploadCommitPromiseExecute used to implement the uplink-c library function
         uploadCommitPromiseExecute commits the upload using promise

 */
void uploadCommitPromiseExecute(napi_env env, void* data) {
  uploadCommitObj *obj = (uploadCommitObj*)data;
  obj->error_result = uplink_upload_commit(&(obj->upload_result));
}
/*!
 \fn void uploadWritePromiseExecute(napi_env env, void* data) 
 \brief uploadWritePromiseExecute used to implement the uplink-c library function
         uploadWritePromiseExecute writes the upload using promise

 */
void uploadWritePromiseExecute(napi_env env, void* data) {
  uploadWriteObj *obj = (uploadWriteObj*)data;
  uint8_t *ptrToData;
  ptrToData = reinterpret_cast<uint8_t *>(obj->bufferPtr);
  obj->write_result = uplink_upload_write(&(obj->upload_result),
  ptrToData, obj->bytesread);
}
/*!
 \fn void uploadObjectExecute(napi_env env, void* data) 
 \brief uploadObjectExecute used to implement the uplink-c library function
         uploadObjectExecute uploads the object using promise

 */
void uploadObjectExecute(napi_env env, void* data) {
  uploadobjectObj *obj = (uploadobjectObj*)data;
  if (obj->uploadoptionSet == 0) {
    obj->upload_result = uplink_upload_object(&(obj->project), obj->bucketname,
          obj->objectkey, NULL);
  } else {
    obj->upload_result = uplink_upload_object(&(obj->project), obj->bucketname,
          obj->objectkey, &(obj->uploadOptions));
  }
}
/*!
 \fn void stateObjectPromiseExecute(napi_env env, void* data) 
 \brief stateObjectPromiseExecute creates the handle for stat_object

 */
void stateObjectPromiseExecute(napi_env env, void* data) {
  objectOperationObj *obj = (objectOperationObj*)data;
  obj->object_result = uplink_stat_object(&(obj->project),
  obj->bucketname, obj->objectkey);
}
/*!
 \fn void deleteObjectPromiseExecute(napi_env env, void* data) 
 \brief deleteObjectPromiseExecute creates the handle for delete_object

 */
//
void deleteObjectPromiseExecute(napi_env env, void* data) {
  objectOperationObj *obj = (objectOperationObj*)data;
  obj->object_result = uplink_delete_object(&(obj->project),
  obj->bucketname, obj->objectkey);
}
/*!
 \fn void stateBucketPromiseExecute(napi_env env, void* data) 
 \brief stateBucketPromiseExecute creates the handle for stat_bucket
 */
void stateBucketPromiseExecute(napi_env env, void* data) {
  bucketOperationObj *obj = (bucketOperationObj*)data;
  obj->bucket_Result = uplink_stat_bucket(&(obj->project), obj->bucketname);
}
/*!
 \fn void createBucketPromiseExecute(napi_env env, void* data) 
 \brief createBucketPromiseExecute creates the handle for create_bucket
 */
void createBucketPromiseExecute(napi_env env, void* data) {
  bucketOperationObj *obj = (bucketOperationObj*)data;
  obj->bucket_Result = uplink_create_bucket(&(obj->project), obj->bucketname);
}
/*!
 \fn void ensureBucketPromiseExecute(napi_env env, void* data) 
 \brief ensureBucketPromiseExecute creates the handle for ensure_bucket
 */
void ensureBucketPromiseExecute(napi_env env, void* data) {
  bucketOperationObj *obj = (bucketOperationObj*)data;
  obj->bucket_Result = uplink_ensure_bucket(&(obj->project), obj->bucketname);
}
/*!
 \fn void deleteBucketPromiseExecute(napi_env env, void* data) 
 \brief deleteBucketPromiseExecute creates the handle for delete_bucket
 */
void deleteBucketPromiseExecute(napi_env env, void* data) {
  bucketOperationObj *obj = (bucketOperationObj*)data;
  obj->bucket_Result = uplink_delete_bucket(&(obj->project), obj->bucketname);
}
/*!
 \fn void ListBucketsPromiseExecute(napi_env env, void* data) 
 \brief ListBucketPromiseExecute used to implement the uplink-c library function
      ListBucketPromiseExecute provide buckets list using promise

 */
void ListBucketsPromiseExecute(napi_env env, void* data) {
  ListBucketsPromiseObj *obj = (ListBucketsPromiseObj*)data;
  if (obj->listBucketOptionSet == 0) {
    obj->bucket_resultIterator = uplink_list_buckets(&(obj->project_result), NULL);
  } else {
    obj->bucket_resultIterator = uplink_list_buckets(&(obj->project_result), &(obj->listBucketsOptions));
  }
}
/*!
 \fn void closeProjectPromiseExecute(napi_env env, void* data) 
 \brief closeProjectPromiseExecute used to implement the uplink-c library function
      closeProjectPromiseExecute closes the project using promise
 */
void closeProjectPromiseExecute(napi_env env, void* data) {
  closeProjectPromiseObj *obj = (closeProjectPromiseObj*)data;
  obj->error_result = uplink_close_project(&(obj->project_result));
}
/*!
 \fn void configOpenProjectPromiseExecute(napi_env env, void* data) 
 \brief configOpenProjectPromiseExecute used to implement the uplink-c library function
      configOpenProjectPromiseExecute opens project using access grant 
 */
void configOpenProjectPromiseExecute(napi_env env, void* data) {
  configOpenProjectPromiseObj *obj = (configOpenProjectPromiseObj*)data;
  obj->project_Result = uplink_config_open_project
  (obj->config, &(obj->access));
}
/*!
 \fn void ParseAccess(napi_env env, void* data) 
 \brief ParseAccess used to implement the uplink-c library function
        ParseAccess parses serialized access grant string. 
 */
void ParseAccess(napi_env env, void* data) {
  ParseAccessPromiseObj* obj = (ParseAccessPromiseObj*)data;
  obj->access_Result = uplink_parse_access(obj->accessString);
}
/*!
 \fn void ShareAccessPromiseExecute(napi_env env, void* data) 
 \brief ShareAccessPromiseExecute used to implement the uplink-c library function
        ShareAccessPromiseExecute creates new access grant with specific permission. 
 */
void ShareAccessPromiseExecute(napi_env env, void* data) {
  AccessSharePromiseObj* obj = (AccessSharePromiseObj*)data;
  obj->access_Result = uplink_access_share(&(obj->access), obj->permission,
  obj->SharePrefixListPointer, obj->SharePrefixSize);
}
/*!
 \fn void ConfigRequestAccessWithEncryption(napi_env env, void* data) 
 \brief ConfigRequestAccessWithEncryption used to implement the uplink-c library function
        ConfigRequestAccessWithEncryption requests for a new access grant using encryption
 */
void ConfigRequestAccessWithEncryption(napi_env env, void* data) {
  ConfigRequestAccessPromiseObj *obj = (ConfigRequestAccessPromiseObj*)data;
  obj->access_Result = uplink_config_request_access_with_passphrase
  (obj->config, obj->satellite_address, obj->api_key, obj->passphrase);
}
/*!
 \fn void RequestAccessWithEncryption(napi_env env, void* data) 
 \brief RequestAccessWithEncryption used to implement the uplink-c library function
        RequestAccessWithEncryption requests for a new access grant using encryption
 */
void RequestAccessWithEncryption(napi_env env, void* data) {
  RequestAccessPromiseObj *obj = (RequestAccessPromiseObj*)data;
  obj->access_Result = uplink_request_access_with_passphrase
  (obj->satellite_address, obj->api_key, obj->passphrase);
}
/*!
 \fn void accessSerializePromiseExecute(napi_env env, void* data) 
 \brief accessSerializePromiseExecute used to implement the uplink-c library function
        accessSerializePromiseExecute serializes access grant into a string.
 */
void accessSerializePromiseExecute(napi_env env, void* data) {
  accessSerializePromiseObj *obj = (accessSerializePromiseObj*)data;
  obj->string_result = uplink_access_serialize(&(obj->access));
}

/*!
 \fn void deriveEncrpPromiseExecute(napi_env env, void* data) 
 \brief accessSerializePromiseExecute used to implement the uplink-c library function
        accessSerializePromiseExecute serializes access grant into a string.
 */
void deriveEncrpPromiseExecute(napi_env env, void* data) {
  deriveEncrpPromiseObj *obj = (deriveEncrpPromiseObj*)data;
  char *ptrToData;
  ptrToData = reinterpret_cast<char *>(obj->saltCharArrayPointer);
  obj->encryptionResult = uplink_derive_encryption_key(obj->passphrase,ptrToData,obj->saltSize);
}
/*!
 \fn void accessSerializePromiseExecute(napi_env env, void* data) 
 \brief accessSerializePromiseExecute used to implement the uplink-c library function
        accessSerializePromiseExecute serializes access grant into a string.
 */
void accessOverRidePromiseExecute(napi_env env, void* data) {
  accessOverRidePromiseObj *obj = (accessOverRidePromiseObj*)data;
  obj->error_result = uplink_access_override_encryption_key(&(obj->access),obj->bucket,obj->prefix,&(obj->encryptionKey));
}