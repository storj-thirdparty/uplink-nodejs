#ifndef DEFINITIONS_H
#define DEFINITIONS_H

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
    Project project_result;
    char* bucketName;
    ObjectIterator* objectIterator;
    ListObjectsOptions list_object_options;
    int listObjectSet;
} listObjectsPromiseObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  Download download_result;
  ObjectResult object_result;
} downloadInfoObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  Download download_result;
  Error* error_result;
} downloadCloseObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  uint8_t* bufferPtr;
  size_t bufferlength;
  Download download_result;
  ReadResult read_result;
} downloadReadObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  char* bucketname;
  char* objectkey;
  Project project;
  DownloadResult download_result;
  DownloadOptions downloadOption;
  int downloadoptionSet;
} downloadObjectObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  Upload upload_result;
  CustomMetadata customMetadata;
  Error* error_result;
} uploadSetMetaObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  Upload upload_result;
  Error* error_result;
} uploadAbortPromiseObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  Upload upload_result;
  ObjectResult object_result;
} uploadInfoObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  Upload upload_result;
  Error* error_result;
} uploadCommitObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  uint8_t *bufferPtr;
  size_t bytesread;
  Upload upload_result;
  WriteResult write_result;
} uploadWriteObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  char* bucketname;
  char* objectkey;
  Project project;
  UploadResult upload_result;
  UploadOptions uploadOptions;
  int uploadoptionSet;
} uploadobjectObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  char* bucketname;
  char* objectkey;
  Project project;
  ObjectResult object_result;
} objectOperationObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  char* bucketname;
  Project project;
  BucketResult bucket_Result;
} bucketOperationObj;

typedef struct {
    Project project_result;
    ListBucketsOptions listBucketsOptions;
    BucketIterator* bucket_resultIterator;
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
  Project project_result;
  Error* error_result;
} closeProjectPromiseObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  Access access;
  ProjectResult project_Result;
  Config config;
} configOpenProjectPromiseObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  Access access;
  ProjectResult project_Result;
} openProjectPromiseObj;

typedef struct {
  char *accessString;
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  AccessResult access_Result;
} ParseAccessPromiseObj;

typedef struct {
  char* satellite_address;
  char* api_key;
  char* passphrase;
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  AccessResult access_Result;
} RequestAccessPromiseObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  Access access;
  AccessResult access_Result;
  int64_t SharePrefixSize;
  SharePrefix* SharePrefixListPointer;
  Permission permission;
} AccessSharePromiseObj;

typedef struct {
  char* satellite_address;
  char* api_key;
  char* passphrase;
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  AccessResult access_Result;
  Config config;
} ConfigRequestAccessPromiseObj;

typedef struct {
  int async_action_status;
  napi_deferred deferred;
  napi_async_work work;
  napi_value result;
  Access access;
  StringResult string_result;
} accessSerializePromiseObj;

#endif DEFINITIONS_H