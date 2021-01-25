#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define UPLINK_DISABLE_NAMESPACE_COMPAT NULL

#include <assert.h>
#include <node_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <iostream>
#include "../uplink_definitions.h"
using std::string;
using std::swap;


typedef struct{
    int async_action_status;
    napi_deferred deferred;
    napi_async_work work;
    napi_value result;
    UplinkProject project_result;
    char* bucketName;
    UplinkObjectIterator* objectIterator;
    UplinkListObjectsOptions list_object_options;
    int listObjectSet;
} listObjectsPromiseObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  UplinkDownload download_result;
  UplinkObjectResult object_result;
} downloadInfoObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  UplinkDownload download_result;
  UplinkError* error_result;
} downloadCloseObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  uint8_t* bufferPtr;
  size_t bufferlength;
  UplinkDownload download_result;
  UplinkReadResult read_result;
} downloadReadObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  char* bucketname;
  char* objectkey;
  UplinkProject project;
  UplinkDownloadResult download_result;
  UplinkDownloadOptions downloadOption;
  int downloadoptionSet;
} downloadObjectObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  UplinkUpload upload_result;
  UplinkCustomMetadata customMetadata;
  UplinkError* error_result;
} uploadSetMetaObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  UplinkUpload upload_result;
  UplinkError* error_result;
} uploadAbortPromiseObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  UplinkUpload upload_result;
  UplinkObjectResult object_result;
} uploadInfoObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  UplinkUpload upload_result;
  UplinkError* error_result;
} uploadCommitObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  uint8_t *bufferPtr;
  size_t bytesread;
  UplinkUpload upload_result;
  UplinkWriteResult write_result;
} uploadWriteObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  char* bucketname;
  char* objectkey;
  UplinkProject project;
  UplinkUploadResult upload_result;
  UplinkUploadOptions uploadOptions;
  int uploadoptionSet;
} uploadobjectObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  char* bucketname;
  char* objectkey;
  UplinkProject project;
  UplinkObjectResult object_result;
} objectOperationObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  char* bucketname;
  UplinkProject project;
  UplinkBucketResult bucket_Result;
} bucketOperationObj;

typedef struct {
    UplinkProject project_result;
    UplinkListBucketsOptions listBucketsOptions;
    UplinkBucketIterator* bucket_resultIterator;
    int async_action_status;
    napi_deferred deferred;
    napi_async_work work;
    napi_value result;
    int listBucketOptionSet;
} ListBucketsPromiseObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  UplinkProject project_result;
  UplinkError* error_result;
} closeProjectPromiseObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  UplinkAccess access;
  UplinkProjectResult project_Result;
  UplinkConfig config;
} configOpenProjectPromiseObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  UplinkAccess access;
  UplinkProjectResult project_Result;
} openProjectPromiseObj;

typedef struct {
  char *accessString;
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  UplinkAccessResult access_Result;
} ParseAccessPromiseObj;

typedef struct {
  char* satellite_address;
  char* api_key;
  char* passphrase;
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  UplinkAccessResult access_Result;
} RequestAccessPromiseObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  UplinkAccess access;
  UplinkAccessResult access_Result;
  int64_t SharePrefixSize;
  UplinkSharePrefix* SharePrefixListPointer;
  UplinkPermission permission;
} AccessSharePromiseObj;

typedef struct {
  char* satellite_address;
  char* api_key;
  char* passphrase;
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  UplinkAccessResult access_Result;
  UplinkConfig config;
} ConfigRequestAccessPromiseObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  UplinkAccess access;
  UplinkStringResult string_result;
} accessSerializePromiseObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  UplinkAccess access;
  char* passphrase;
  int64_t saltSize;
  char* saltCharArrayPointer;
  UplinkEncryptionKeyResult encryptionResult;
} deriveEncrpPromiseObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  UplinkAccess access;
  char* bucket;
  char* prefix;
  UplinkEncryptionKey encryptionKey;
  UplinkError* error_result;
} accessOverRidePromiseObj;

#endif DEFINITIONS_H