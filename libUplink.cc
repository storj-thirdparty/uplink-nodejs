#include <assert.h>
#include <node_api.h>
#include "uplink_definitions.h"
#include "libuplinkc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std; 

/* A utility function to reverse a string  */
void reverse(char str[], int length) 
{ 
    int start = 0; 
    int end = length -1; 
    while (start < end) 
    { 
        swap(*(str+start), *(str+end)); 
        start++; 
        end--; 
    } 
} 
  

char* itoa(int num, char* str, int base) 
{ 
    int i = 0; 
    bool isNegative = false; 
  
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) 
    { 
        str[i++] = '0'; 
        str[i] = '\0'; 
        return str; 
    } 
  
    // In standard itoa(), negative numbers are handled only with  
    // base 10. Otherwise numbers are considered unsigned. 
    if (num < 0 && base == 10) 
    { 
        isNegative = true; 
        num = -num; 
    } 
  
    // Process individual digits 
    while (num != 0) 
    { 
        int rem = num % base; 
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0'; 
        num = num/base; 
    } 
  
    // If number is negative, append '-' 
    if (isNegative) 
        str[i++] = '-'; 
  
    str[i] = '\0'; // Append string terminator 
  
    // Reverse the string 
    reverse(str, i); 
  
    return str; 
}


napi_value createhandle(napi_env env,long handlevalue,char* errorptr){
  napi_status status;
  napi_value ObjectRef,returnObject; 
  //
  status = napi_create_object(env,&ObjectRef);
  assert(status == napi_ok);
  napi_value _handle;
  status = napi_create_int64(env, handlevalue, &_handle);
  assert(status == napi_ok);
  status = napi_set_named_property(env,ObjectRef,"_handle",_handle);
  assert(status == napi_ok);

  status = napi_create_object(env,&returnObject);
  assert(status == napi_ok);

  napi_value errorString;
  status = napi_create_string_utf8(env,errorptr,NAPI_AUTO_LENGTH,&errorString);
  assert(status == napi_ok);
  status = napi_set_named_property(env,returnObject,"error",errorString);
  assert(status == napi_ok);
  status = napi_set_named_property(env,returnObject,"data",ObjectRef);
  assert(status == napi_ok);
  
  return returnObject;
}


// * function to create new Storj uplink
// * pre-requisites: None
// * inputs: UplinkConfig (object) , Temp(string)
// * output: Object contains error (if any ,else empty string) and data (UplinkRef)
napi_value new_uplinkc(napi_env env, napi_callback_info info) {
  //
  napi_status status;
  
  //
  size_t argc = 2;
  napi_value args[2];
  //
  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  //
  if (argc < 2) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }
  //
  UplinkConfig lO_uplinkConfig;

  //Checking type of first argument is object or not 
  napi_valuetype checktypeofinput,checktypeofinput1;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if((checktypeofinput!=napi_object)&&(checktypeofinput!=napi_null)){

    napi_throw_type_error(env,nullptr,"Wrong datatype excepted object");
  }
  //
  
  status = napi_typeof(env,args[1],&checktypeofinput1);
  assert(status == napi_ok);

  if(checktypeofinput1!=napi_string){
    napi_throw_type_error(env,nullptr,"Wrong datatype excepted string");
  }
  //
  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  convertedvalue = convertedvalue+1;
  char tempDir[convertedvalue];
  status = napi_get_value_string_utf8(env,args[1],&tempDir[0],convertedvalue,&bufsize);
  
  char *_err = "";
  char **err = &_err;
  bool nullUplinkConfig = false;

  if(checktypeofinput==napi_null){
    nullUplinkConfig = true;
  }else if(checktypeofinput ==napi_object){
    nullUplinkConfig = false;
    //Fetching and converting NAPI value to C type value
    napi_value VolatileNAPI;
    status = napi_get_named_property(env,args[0],"Volatile",&VolatileNAPI);
    assert(status == napi_ok);
    
    //
    napi_value tlsNAPI;
    status = napi_get_named_property(env,VolatileNAPI,"tls",&tlsNAPI);
    assert(status == napi_ok);
    
    //
    napi_value skip_peer_ca_whitelistNAPI;
    status = napi_get_named_property(env,tlsNAPI,"skip_peer_ca_whitelist",&skip_peer_ca_whitelistNAPI);
    assert(status == napi_ok);
    //
    bool skip_peer_ca_whitelist;
    napi_get_value_bool(env,skip_peer_ca_whitelistNAPI,&skip_peer_ca_whitelist);
    assert(status == napi_ok);
    //
    lO_uplinkConfig.Volatile.tls.skip_peer_ca_whitelist = skip_peer_ca_whitelist;
    //
    napi_value peer_ca_whitelist_pathNAPI;
    status = napi_get_named_property(env,tlsNAPI,"peer_ca_whitelist_path",&peer_ca_whitelist_pathNAPI);
    assert(status == napi_ok);
    //
    bufsize = 0;
    convertedvalue = 0;
    status = napi_get_value_string_utf8(env, peer_ca_whitelist_pathNAPI,NULL,bufsize,&convertedvalue);
    convertedvalue = convertedvalue+1;
    char peer_ca_whitelist_path[convertedvalue];
    status = napi_get_value_string_utf8(env,peer_ca_whitelist_pathNAPI,&peer_ca_whitelist_path[0],convertedvalue,&bufsize);

    lO_uplinkConfig.Volatile.tls.peer_ca_whitelist_path = &peer_ca_whitelist_path[0];
    //
    napi_value peer_id_versionNAPI;
    status = napi_get_named_property(env,VolatileNAPI,"peer_id_version",&peer_id_versionNAPI);
    assert(status == napi_ok);
    //
    bufsize = 0;
    convertedvalue = 0;
    status = napi_get_value_string_utf8(env, peer_id_versionNAPI,NULL,bufsize,&convertedvalue);
    convertedvalue = convertedvalue+1;
    char peer_id_version[convertedvalue];
    status = napi_get_value_string_utf8(env,peer_id_versionNAPI,&peer_id_version[0],convertedvalue,&bufsize);
    //
    lO_uplinkConfig.Volatile.peer_id_version = &peer_id_version[0];
    //
    napi_value max_inline_sizeNAPI;
    status = napi_get_named_property(env,VolatileNAPI,"max_inline_size",&max_inline_sizeNAPI);
    assert(status == napi_ok);
    //
    int max_inline_size;
    napi_get_value_int32(env,max_inline_sizeNAPI,&max_inline_size);
    assert(status == napi_ok);
    //
    lO_uplinkConfig.Volatile.max_inline_size = max_inline_size;
    //
    napi_value max_memoryNAPI;
    status = napi_get_named_property(env,VolatileNAPI,"max_memory",&max_memoryNAPI);
    assert(status == napi_ok);
    //
    int max_memory;
    napi_get_value_int32(env,max_memoryNAPI,&max_memory);
    assert(status == napi_ok);
    lO_uplinkConfig.Volatile.max_memory = max_memory;
    //
    napi_value dial_timeoutNAPI;
    status = napi_get_named_property(env,VolatileNAPI,"dial_timeout",&dial_timeoutNAPI);
    assert(status == napi_ok);
    //
    int dial_timeout;
    napi_get_value_int32(env,dial_timeoutNAPI,&dial_timeout);
    assert(status == napi_ok);
    
    lO_uplinkConfig.Volatile.dial_timeout = dial_timeout;
    napi_value user_agentNAPI;
    status = napi_get_named_property(env,VolatileNAPI,"user_agent",&user_agentNAPI);
    assert(status == napi_ok);
    
    bufsize = 0;
    convertedvalue = 0;
    status = napi_get_value_string_utf8(env, user_agentNAPI,NULL,bufsize,&convertedvalue);
    convertedvalue = convertedvalue+1;
    char user_agent[convertedvalue];
    status = napi_get_value_string_utf8(env,user_agentNAPI,&user_agent[0],convertedvalue,&bufsize);
    lO_uplinkConfig.Volatile.user_agent = &user_agent[0];
  }
  
    UplinkRef lO_uplinkref = new_uplink(lO_uplinkConfig,&tempDir[0],err);
  //
  return createhandle(env,lO_uplinkref._handle, _err);
  
}

// * function to parse API key, to be used by Storj
// * pre-requisites: None
// * inputs: API key (string)
// * output: object contains error (if any ,else empty string) and data (APIKeyRef)
napi_value parse_api_keyc(napi_env env, napi_callback_info info) {
  //Status type
  napi_status status;

  //
  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  //Checking number of arguments
  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"Wrong datatype excepted object!!");
  }
  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[0],NULL,bufsize,&convertedvalue);
  convertedvalue = convertedvalue+1;
  char apiKey[convertedvalue];  
  status = napi_get_value_string_utf8(env,args[0],&apiKey[0],convertedvalue,&bufsize);
  //TO DO : Remove
  
  char *_err = "";
  char **err = &_err;
  APIKeyRef lO_APIKeyRef = parse_api_key(apiKey,err);
  
  return createhandle(env,lO_APIKeyRef._handle, _err);
}
//

long getHandleValue(napi_env env,napi_value handleobj){
  
  napi_status status;
  
  napi_valuetype checktypeofinput;
  
  status = napi_typeof(env,handleobj,&checktypeofinput);
  assert(status == napi_ok);
  
  if(checktypeofinput!=napi_object){
    return 0;
  }
  napi_value propertyname;
  status = napi_get_property_names(env,handleobj,&propertyname);
  assert(status == napi_ok);

  napi_value handlevalue;
  status = napi_get_property(env,handleobj,propertyname,&handlevalue);
  assert(status == napi_ok);
  
  int64_t handle = 0;
  status = napi_get_value_int64(env,handlevalue,&handle);
  assert(status == napi_ok);
  long handlelong = (long)handle;
  return handlelong;
}

// * function to open a Storj project
// * pre-requisites: new_uplinkc() and parse_api_keyc() functions have been already called
// * inputs: valid UplinkRef, Satellite Address (string), valid APIKeyRef
// * output: object contains error (if any ,else empty string) and data (ProjectRef)
napi_value open_projectc(napi_env env, napi_callback_info info) {

  napi_status status;
  
  size_t argc = 3;
  napi_value args[3];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 3) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }


  napi_valuetype checktypeofinput;

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"Wrong datatype excepted object!!");
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"Wrong datatype excepted Stirng!!");
  }

  status = napi_typeof(env,args[2],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"Wrong datatype excepted object!!");
  }


  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  convertedvalue = convertedvalue+1;
  char satellite[convertedvalue];  
  //char satelliteValue[convertedvalue+1];
  status = napi_get_value_string_utf8(env,args[1],&satellite[0],convertedvalue,&bufsize);
  
  char *_err = "";
  char **err = &_err;
  UplinkRef lO_UplinkRef;
  lO_UplinkRef._handle = getHandleValue(env,args[0]);
  //return getHandleValue(env,args[0]);
  APIKeyRef lO_APIKeyRef;
  lO_APIKeyRef._handle = getHandleValue(env,args[2]);
  ProjectRef lO_ProjectRef = open_project(lO_UplinkRef,satellite,lO_APIKeyRef,err);
  //
  return createhandle(env,lO_ProjectRef._handle, _err);
}

napi_value open_bucketc(napi_env env, napi_callback_info info) {
  //
  napi_status status;
  //
  size_t argc = 3;
  napi_value args[3];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 3) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"Wrong datatype excepted Object!!");
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"Wrong datatype excepted Stirng!!");
  }
  //
  status = napi_typeof(env,args[2],&checktypeofinput);
  assert(status == napi_ok);
  //
  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"Wrong datatype excepted Stirng!!");
  }
  //
  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  convertedvalue = convertedvalue+1;
  char bucketname[convertedvalue];  
  status = napi_get_value_string_utf8(env,args[1],&bucketname[0],convertedvalue,&bufsize);
  

  size_t bufsizeenc = 0;
  size_t convertedvalueenc = 0;
  status = napi_get_value_string_utf8(env, args[2],NULL,bufsizeenc,&convertedvalueenc);
  convertedvalueenc = convertedvalueenc+1;
  char encryption[convertedvalueenc];  
  status = napi_get_value_string_utf8(env,args[2],&encryption[0],convertedvalueenc,&bufsizeenc);
  
  char *_err = "";
  char **err = &_err;

  ProjectRef lO_ProjectRef;
  lO_ProjectRef._handle = getHandleValue(env,args[0]);
  //TO DO : Remove
  BucketRef lO_BucketRef = open_bucket(lO_ProjectRef,&bucketname[0],&encryption[0],err);
  return createhandle(env,lO_BucketRef._handle, _err);
}

// * function to generate a key from given phassphrase
// * pre-requisites: open_projectc() function has been already called
// * inputs: valid ProjectRef, Encryption Pass Phrase (string)
// * output: object contains error (if any ,else empty string) and data (int)
napi_value project_salted_key_from_passphrasec(napi_env env, napi_callback_info info) {
  //
  napi_status status;
  //
  size_t argc = 2;
  napi_value args[2];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 2) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);
  //
  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"Wrong datatype excepted Object!!");
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);
  //
  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"Wrong datatype excepted String!!");
  }
  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  convertedvalue = convertedvalue+1;
  char encryption[convertedvalue];  
  status = napi_get_value_string_utf8(env,args[1],&encryption[0],convertedvalue,&bufsize);
  assert(status == napi_ok);
  //}
  char *_err = "";
  char **err = &_err;
  //
  uint8_t *encryptionpassphraseptr = NULL;
  // 
  ProjectRef lO_projectRef;
  lO_projectRef._handle = getHandleValue(env,args[0]);
  //
  encryptionpassphraseptr = project_salted_key_from_passphrase(lO_projectRef,encryption,err);
  //Converting uint8_t to unint32_t
  uint32_t encrptionphassphrase_unit32 = (uint32_t)*encryptionpassphraseptr;
  //
  napi_value encryptionPhassphrase32;
  status = napi_create_uint32(env,encrptionphassphrase_unit32,&encryptionPhassphrase32);
  assert(status == napi_ok);

  napi_value returnObject;
  status = napi_create_object(env,&returnObject);
  napi_value errorString;
  //
  status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorString);
  assert(status == napi_ok);
  status = napi_set_named_property(env,returnObject,"error",errorString);
  assert(status == napi_ok);
  //
  status = napi_set_named_property(env,returnObject,"data",encryptionPhassphrase32);
  assert(status == napi_ok);
  return returnObject;

}

// * function to  creates an encryption access context with a default key set
// * pre-requisites: new_scope() function has been already called
// * inputs: key(int)
// * output: object contains error (if any ,else empty string) and data (EncryptionAccessRef)
napi_value new_encryption_access_with_default_keyc(napi_env env, napi_callback_info info) {
  //
  napi_status status;
  
  //Number of arguments passed
  size_t argc = 1;
  napi_value args[1];

  //
  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }
  //
  napi_valuetype checktypeofinput;

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);
  //
  if(checktypeofinput!=napi_number){
    napi_throw_type_error(env,nullptr,"\nWrong datatype excepted number!!\n");
  }
  //
  char *_err = "";
  char **err = &_err;
  uint32_t unitvalue;
  status = napi_get_value_uint32(env,args[0],&unitvalue);
  assert(status == napi_ok);
  uint8_t encryptionphassphrase = (uint8_t)unitvalue;
  EncryptionAccessRef lO_EncryptionAccessRef = new_encryption_access_with_default_key(&encryptionphassphrase);
  return createhandle(env,lO_EncryptionAccessRef._handle, _err);
  
}

// * function to turn encryption access into base58
// * pre-requisites: open_projectc() function has been already called
// * inputs: EncryptionAccessRef
// * output: object contains error (if any ,else empty string) and data (string)
napi_value serialize_encryption_accessc(napi_env env, napi_callback_info info) {
  //Status type
  napi_status status;
  
  //
  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);
  //
  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"Wrong datatype excepted Object!!");
  }
  //
  char *_err = "";
  char **err = &_err;
  EncryptionAccessRef lO_EncryptionAccessRef;
  lO_EncryptionAccessRef._handle = getHandleValue(env,args[0]);
  char* serializedaccessptr = serialize_encryption_access(lO_EncryptionAccessRef,err);
  napi_value serializedaccess;
  status = napi_create_string_utf8(env,serializedaccessptr,NAPI_AUTO_LENGTH,&serializedaccess);
  assert(status == napi_ok);
  napi_value returnObject;
  status = napi_create_object(env,&returnObject);
  napi_value errorString;
  //
  status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorString);
  assert(status == napi_ok);
  status = napi_set_named_property(env,returnObject,"error",errorString);
  assert(status == napi_ok);
  //
  status = napi_set_named_property(env,returnObject,"data",serializedaccess);
  assert(status == napi_ok);
  
  return returnObject;
}

// * function to get encryption access to upload and/or download data to/from Storj
// * pre-requisites: open_projectc() function has been already called
// * inputs: valid ProjectRef, Encryption Pass Phrase (string)
// * output: object contains error (if any ,else empty string) and data (EncryptionAccessRef)
napi_value encryption_accessc(napi_env env, napi_callback_info info) {
  //
  napi_status status;
  //
  size_t argc = 2;
  napi_value args[2];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 2) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"Wrong datatype excepted String!!");
  }
  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  convertedvalue = convertedvalue+1;
  char encryption[convertedvalue];  
  status = napi_get_value_string_utf8(env,args[1],&encryption[0],convertedvalue,&bufsize);
  assert(status == napi_ok);
  //
  char *_err = "";
  char **err = &_err;
  //char projectsaltedkey[400];
  ProjectRef lO_projectRef;
  lO_projectRef._handle = getHandleValue(env,args[0]);
  uint8_t *encryptionpassphraseptr = project_salted_key_from_passphrase(lO_projectRef,encryption,err);

  EncryptionAccessRef encryption_access = new_encryption_access_with_default_key(encryptionpassphraseptr);
  char *enc_ctx = serialize_encryption_access(encryption_access, err);

  napi_value returnObject;
  status = napi_create_object(env,&returnObject);
  napi_value errorString;
  status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorString);
  assert(status == napi_ok);
  status = napi_set_named_property(env,returnObject,"error",errorString);
  assert(status == napi_ok);

  napi_value encryption_accessNAPI;
  status = napi_create_string_utf8(env,enc_ctx,NAPI_AUTO_LENGTH,&encryption_accessNAPI);
  assert(status == napi_ok);
  status = napi_set_named_property(env,returnObject,"data",encryption_accessNAPI);
  assert(status == napi_ok);
  return returnObject;
  
}

// * function to get uploader reference used to upload data to Storj (V3) bucket's path
// * pre-requisites: open_bucket() function has been already called
// * inputs: Bucket Reference (BucketRef), Storj Path/File Name (string) within the opened bucket, Upload Options (obj)
// * output: object contains error (if any ,else empty string) and data (UploadRef)
napi_value uploadc(napi_env env, napi_callback_info info) {
  //
  napi_status status;
  //
  size_t argc = 3;
  napi_value args[3];
  //
  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 3) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  //
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"Wrong datatype excepted Object!!");
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"Wrong datatype excepted String!!");
  }

  //
  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  convertedvalue = convertedvalue+1;
  char storjobjectpath[convertedvalue];  
  status = napi_get_value_string_utf8(env,args[1],&storjobjectpath[0],convertedvalue,&bufsize);
  
  
  char *_err = "";
  char **err = &_err;
  BucketRef lO_BucketRef;
  lO_BucketRef._handle = getHandleValue(env,args[0]);
  UploadOptions lO_UploadOptions;
  UploaderRef lO_UploaderRef;
  //
  status = napi_typeof(env,args[2],&checktypeofinput);
  assert(status == napi_ok);
  if(checktypeofinput==napi_object){
    //
    napi_value content_typeNAPI;
    status = napi_get_named_property(env,args[2],"content_type",&content_typeNAPI);
    assert(status == napi_ok);
    //
    bufsize = 0;
    convertedvalue = 0;
    status = napi_get_value_string_utf8(env, content_typeNAPI,NULL,bufsize,&convertedvalue);
    assert(status == napi_ok);
    convertedvalue = convertedvalue+1;
    char content_type[convertedvalue];  
    status = napi_get_value_string_utf8(env,content_typeNAPI,&content_type[0],convertedvalue,&bufsize);
    //
    lO_UploadOptions.content_type = &content_type[0];
    //
    napi_value expiresNAPI;
    status = napi_get_named_property(env,args[2],"expires",&expiresNAPI);
    assert(status == napi_ok);
    //
    int64_t expires;
    status = napi_get_value_int64(env,expiresNAPI,&expires);
    assert(status == napi_ok);
    //
    lO_UploadOptions.expires = expires;
    //
    lO_UploaderRef = upload(lO_BucketRef,storjobjectpath,&lO_UploadOptions,err);
    //free_upload_opts(lO_UploadOptions);
  }else if(checktypeofinput==napi_null){
    lO_UploaderRef = upload(lO_BucketRef,storjobjectpath,NULL,err);
  }else{
    napi_throw_type_error(env,nullptr,"Wrong datatype excepted Object!!");
  }

  //free_upload_opts(&lO_UploadOptions);
  return createhandle(env,lO_UploaderRef._handle, _err);
}

//  * function to write data to Storj (V3) bucket's path
//  * pre-requisites: upload() function has been already called
//  * inputs: Uploader reference (uploaderRef), Data to upload (buffer), Size of data to upload (int)
//  * output: object contains error (if any ,else empty string) and data (APIKeyRef)
// * **Note:** The Data to upload (buffer) passed to function should be a uint8 pointer only. 
napi_value upload_writec(napi_env env, napi_callback_info info) {
  //
  napi_status status;
  
  //
  size_t argc = 3;
  napi_value args[3];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 3) {
    napi_throw_type_error(env, nullptr, "Wrong number of arguments excepted 3 arguments");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);
  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"Wrong datatype excepted Object!!");
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);
  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"Wrong datatype excepted Object!!");
  }

  status = napi_typeof(env,args[2],&checktypeofinput);
  assert(status == napi_ok);
  if(checktypeofinput!=napi_number){
    napi_throw_type_error(env,nullptr,"Wrong datatype excepted number!!");
  }
  //
  char *_err = "";
  char **err = &_err;
  UploaderRef lO_UploaderRef;
  lO_UploaderRef._handle = getHandleValue(env,args[0]);
  //
  bool buffertype1;
  status = napi_is_arraybuffer(env, args[1], &buffertype1);
  assert(status == napi_ok);
  size_t lengthOfBuffer;  
  //
  void* bufferPtr = NULL;  
  //
  status = napi_get_buffer_info(env, args[1], &bufferPtr,&lengthOfBuffer);
  assert(status == napi_ok);
  //
  int64_t bytes64;
  status = napi_get_value_int64(env,args[2],&bytes64);
  assert(status == napi_ok);
  //
  size_t bytesread = (size_t)bytes64;
  //
  uint8_t *ptrToData ; 
  ptrToData = (uint8_t *)bufferPtr;
  //
  size_t datauploaded = upload_write(lO_UploaderRef,ptrToData,bytesread, err);
  //
  napi_value datauploadedObject;
  napi_create_int32(env,datauploaded,&datauploadedObject);
  //
  napi_value returnObject;
  status = napi_create_object(env,&returnObject);
  napi_value errorString;
  status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorString);
  assert(status == napi_ok);
  status = napi_set_named_property(env,returnObject,"error",errorString);
  assert(status == napi_ok);
  status = napi_set_named_property(env,returnObject,"data",datauploadedObject);
  assert(status == napi_ok);
  //
  return returnObject;
}
  // * function to commit and finalize file for uploaded data to Storj (V3) bucket's path
  // * pre-requisites: upload() function has been already called
  // * inputs: Uploader reference (uploaderRef)
  // * output: Error (string) if any else None
napi_value upload_commitc(napi_env env, napi_callback_info info) {
  //
  napi_status status;
  //
  size_t argc = 1;
  napi_value args[1];
  //
  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  //
  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);
  //
  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"Wrong datatype Object!!");
  }
  //
  char *_err = "";
  char **err = &_err;
  //
  UploaderRef lO_UploaderRef;
  lO_UploaderRef._handle = getHandleValue(env,args[0]);
  upload_commit(lO_UploaderRef,err);
  napi_value errorvalue;
  //
  status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorvalue);
  return errorvalue;
}

// * function to close currently established uplink
// * pre-requisites: new_uplinkc() function has been already called
// * inputs: Uplink Reference(UplinkRef)
// * output: Error if any else empty string
napi_value close_uplinkc(napi_env env, napi_callback_info info){
  
  napi_status status;
  
  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  UplinkRef lO_UplinkRef;
  lO_UplinkRef._handle = getHandleValue(env,args[0]);

  char *_err = "";
  char **err = &_err;

  close_uplink(lO_UplinkRef,err);
  napi_value errorString;
  status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorString);
  assert(status == napi_ok);
  return errorString;
}

// * function to get downloader reference to download Storj (V3) object's data and store it on local computer
// * pre-requisites: open_bucket() function has been already called
// * inputs: Bucket reference (BucketRef), Storj Path/File Name (string) within the opened bucket
// * output: Downloader reference (downloaderRef), Error (string) if any else None
napi_value downloadc(napi_env env, napi_callback_info info) {
  //
  napi_status status;
  
  //
  size_t argc = 2;
  napi_value args[2];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  //Checking number of arguments
  if (argc < 2) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype excepted object!!\n");
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype excepted string!!\n");
  }
  //
  size_t bufsize = 0;
  size_t convertedvalue = 0;
  //
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  convertedvalue=convertedvalue+1;
  //
  char storjobjectpath[convertedvalue];  
  status = napi_get_value_string_utf8(env,args[1],&storjobjectpath[0],convertedvalue,&bufsize);
  //
  char *_err = "";
  char **err = &_err;
  BucketRef lO_BucketRef;
  lO_BucketRef._handle = getHandleValue(env,args[0]);
  //
  DownloaderRef lO_DownloaderRef = download(lO_BucketRef,&storjobjectpath[0],err);
  //
  return createhandle(env,lO_DownloaderRef._handle, _err);
  
}

// * function to read Storj (V3) object's data and return the data
// * pre-requisites: download() function has been already called
// * inputs: Downloader Reference (downloaderRef), Length of data to download (int)
// * output: object contains error (if any ,else empty string) and data (int)
napi_value download_readc(napi_env env, napi_callback_info info) {
  //
  napi_status status;
  //
  size_t argc = 3;
  napi_value args[3];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  //
  if (argc < 3) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);
  //
  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype excepted object!!\n");
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);
  //
  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype excepted object!!\n");
  }

  status = napi_typeof(env,args[2],&checktypeofinput);
  assert(status == napi_ok);
  //
  if(checktypeofinput!=napi_number){
    napi_throw_type_error(env,nullptr,"\nWrong datatype excepted number!!\n");
  }
  //
  char *_err = "";
  char **err = &_err;
  DownloaderRef lO_DownloaderRef;
  lO_DownloaderRef._handle = getHandleValue(env,args[0]);
  
  uint8_t buffer[256];
  
  void* bufferPtr = NULL;  
  size_t lengthOfBuffer;  
  status = napi_get_buffer_info(env, args[1], &bufferPtr,&lengthOfBuffer);
  assert(status == napi_ok);
  //
  uint8_t *ptrToData ; 
  ptrToData = (uint8_t *)bufferPtr;
  //
  size_t datadownload = download_read(lO_DownloaderRef,ptrToData,lengthOfBuffer, err);
  //
  napi_value returnObject;
  status = napi_create_object(env,&returnObject);
  assert(status == napi_ok);
  //
  napi_value datadownloadNAPI;
  status = napi_create_int32(env,datadownload,&datadownloadNAPI);
  assert(status == napi_ok);
  //
  status = napi_set_named_property(env, returnObject, "data", datadownloadNAPI);
  assert(status == napi_ok);
  //
  //
  napi_value errorvalue;
  status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorvalue);
  assert(status == napi_ok);
  
  status = napi_set_named_property(env, returnObject, "error", errorvalue);
  assert(status == napi_ok);
  return returnObject;
}

// * function to close downloader after completing the data read process
// * pre-requisites: downloadc() function has been already called
// * inputs: Downloader Reference (downloaderRef)
// * output: Error (string) if any else None
napi_value download_closec(napi_env env, napi_callback_info info) {
  //
  napi_status status;
  //
  size_t argc = 1;
  napi_value args[1];
  //
  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  //
  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong datatype excepted object!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);
  
  char *_err = "";
  char **err = &_err;
  DownloaderRef lO_DownloaderRef;
  lO_DownloaderRef._handle = getHandleValue(env,args[0]);
  //
  download_close(lO_DownloaderRef,err);
  napi_value errorvalue;
  status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorvalue);
  return errorvalue;
}
// * function to create new bucket in Storj project
// * pre-requisites: open_projectc() function has been already called, successfully
// * inputs: valid ProjectRef, Bucket Name (string)
// * output: object contains error (if any ,else empty string) and data (BucketInfo)
napi_value create_bucketc(napi_env env, napi_callback_info info) {
  //
  napi_status status;
  
  size_t argc = 3;
  napi_value args[3];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  //Checking number of arguments
  if (argc < 3) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
 
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype excepted object!!\n");
  }

  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;
  char bucketnameToCreate[convertedvalue]; 
  status = napi_get_value_string_utf8(env,args[1],&bucketnameToCreate[0],convertedvalue,&bufsize);

  
  char *_err = "";
  char **err = &_err;
  //
  ProjectRef lO_ProjectRef;
  lO_ProjectRef._handle = getHandleValue(env,args[0]);

  BucketConfig lO_BucketConfig;
  enum CipherSuite ciphersuitvalue;
  enum RedundancyAlgorithm algorithumvalue;
  ciphersuitvalue = STORJ_ENC_AESGCM;
  
  //
  BucketInfo lO_BucketInfo;
  //
  status = napi_typeof(env,args[2],&checktypeofinput);
  assert(status == napi_ok);
  if(checktypeofinput == napi_null){
    lO_BucketConfig = lO_BucketConfig;
    lO_BucketInfo = create_bucket(lO_ProjectRef,bucketnameToCreate,NULL,err); 
  }else if(checktypeofinput == napi_object){
    napi_value path_cipherNAPI;
    status = napi_get_named_property(env, args[2],"path_cipher",&path_cipherNAPI);
    assert(status == napi_ok);
    
    int path_cipher;
    status = napi_get_value_int32(env,path_cipherNAPI,&path_cipher);
    assert(status == napi_ok);
    if(path_cipher==1){
      ciphersuitvalue = STORJ_ENC_UNSPECIFIED;
    }else if(path_cipher==2){
      ciphersuitvalue = STORJ_ENC_NULL;
    }else if(path_cipher==3){
      ciphersuitvalue = STORJ_ENC_AESGCM;
    }else{
      ciphersuitvalue = STORJ_ENC_SECRET_BOX;
    }

    lO_BucketConfig.path_cipher = ciphersuitvalue;
    //
    napi_value encryption_parametersNAPI;
    status = napi_get_named_property(env, args[2],"encryption_parameters",&encryption_parametersNAPI);
    assert(status == napi_ok);
    
    //
    napi_value encryption_parameterscipher_suiteNAPI;
    status = napi_get_named_property(env, encryption_parametersNAPI,"cipher_suite",&encryption_parameterscipher_suiteNAPI);
    assert(status == napi_ok);
    //
    int encryption_path_cipher;
    status = napi_get_value_int32(env,encryption_parameterscipher_suiteNAPI,&encryption_path_cipher);
    assert(status == napi_ok);
    
    if(encryption_path_cipher==1){
      ciphersuitvalue = STORJ_ENC_UNSPECIFIED;
    }else if(encryption_path_cipher==2){
      ciphersuitvalue = STORJ_ENC_NULL;
    }else if(encryption_path_cipher==3){
      ciphersuitvalue = STORJ_ENC_AESGCM;
    }else{
      ciphersuitvalue = STORJ_ENC_SECRET_BOX;
    }
    lO_BucketConfig.encryption_parameters.cipher_suite = ciphersuitvalue;
    //
    napi_value encryption_parametersblock_sizeNAPI;
    status = napi_get_named_property(env, encryption_parametersNAPI,"block_size",&encryption_parametersblock_sizeNAPI);
    assert(status == napi_ok);
    //
    int block_size;
    status = napi_get_value_int32(env,encryption_parametersblock_sizeNAPI,&block_size);
    assert(status == napi_ok);
    //
    lO_BucketConfig.encryption_parameters.block_size = block_size;
    //
    napi_value redundancy_schemeNAPI;
    status = napi_get_named_property(env, args[2],"redundancy_scheme",&redundancy_schemeNAPI);
    assert(status == napi_ok);
    
    napi_value algorithmNAPI;
    status = napi_get_named_property(env, redundancy_schemeNAPI,"algorithm",&algorithmNAPI);
    assert(status == napi_ok);
    
    int algorithm;
    status = napi_get_value_int32(env,algorithmNAPI,&algorithm);
    assert(status == napi_ok);
    
    if(algorithm==0){
      algorithumvalue = STORJ_INVALID_REDUNDANCY_ALGORITHM;
    }else{
      algorithumvalue = STORJ_REED_SOLOMON;
    }
    lO_BucketConfig.redundancy_scheme.algorithm = algorithumvalue;

    napi_value share_sizeNAPI;
    status = napi_get_named_property(env, redundancy_schemeNAPI,"share_size",&share_sizeNAPI);
    assert(status == napi_ok);
    //
    int share_size;
    status = napi_get_value_int32(env,share_sizeNAPI,&share_size);
    assert(status == napi_ok);
    lO_BucketConfig.redundancy_scheme.share_size = share_size;

    napi_value required_sharesNAPI;
    status = napi_get_named_property(env, redundancy_schemeNAPI,"required_shares",&required_sharesNAPI);
    assert(status == napi_ok);
    //
    int required_shares;
    status = napi_get_value_int32(env,required_sharesNAPI,&required_shares);
    assert(status == napi_ok);

    lO_BucketConfig.redundancy_scheme.required_shares = required_shares;
    //
    napi_value repair_sharesNAPI;
    status = napi_get_named_property(env, redundancy_schemeNAPI,"repair_shares",&repair_sharesNAPI);
    assert(status == napi_ok);

    int repair_shares;
    status = napi_get_value_int32(env,repair_sharesNAPI,&repair_shares);
    assert(status == napi_ok);

    lO_BucketConfig.redundancy_scheme.repair_shares = repair_shares;

    napi_value optimal_sharesNAPI;
    status = napi_get_named_property(env, redundancy_schemeNAPI,"optimal_shares",&optimal_sharesNAPI);
    assert(status == napi_ok);

    int optimal_shares;
    status = napi_get_value_int32(env,optimal_sharesNAPI,&optimal_shares);
    assert(status == napi_ok);

    lO_BucketConfig.redundancy_scheme.optimal_shares = optimal_shares;

    napi_value total_sharesNAPI;
    status = napi_get_named_property(env, redundancy_schemeNAPI,"total_shares",&total_sharesNAPI);
    assert(status == napi_ok);

    int total_shares;
    status = napi_get_value_int32(env,total_sharesNAPI,&total_shares);
    assert(status == napi_ok);
    lO_BucketConfig.redundancy_scheme.total_shares = total_shares;
    
    lO_BucketInfo = create_bucket(lO_ProjectRef,bucketnameToCreate,&lO_BucketConfig,err); 
  }else{
    napi_throw_type_error(env, nullptr, "Wrong datatype excepted object");
  }
  
  
  napi_value returnObject;
  status = napi_create_object(env,&returnObject);
  assert(status == napi_ok);

  napi_value errorString;
  status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorString);
  assert(status == napi_ok);
  status = napi_set_named_property(env,returnObject,"error",errorString);
  assert(status == napi_ok);

  if(strlen(_err)==0){
    //Creating Bucket Info Object
    napi_value bucketInfoObj;
    status = napi_create_object(env,&bucketInfoObj);
    assert(status == napi_ok);
    //Creating bucket name
    napi_value bucketName;
    status = napi_create_string_utf8(env,lO_BucketInfo.name,NAPI_AUTO_LENGTH,&bucketName);
    assert(status == napi_ok);
    //
    status = napi_set_named_property(env,bucketInfoObj,"name",bucketName);
    assert(status == napi_ok);
    //Assigning bucket name to 

    napi_value created;
    //
    status = napi_create_int64(env, lO_BucketInfo.created, &created);
    assert(status == napi_ok);
    //
    status = napi_set_named_property(env,bucketInfoObj,"created",created);
    assert(status == napi_ok);
    //path_cipher
    napi_value path_cipher;
    status = napi_create_int64(env, lO_BucketInfo.path_cipher, &path_cipher);
    assert(status == napi_ok);
    //
    status = napi_set_named_property(env,bucketInfoObj,"path_cipher",path_cipher);
    assert(status == napi_ok);
    //XXXXXX
    napi_value encryption_parametersObj;
    status = napi_create_object(env,&encryption_parametersObj);
    assert(status == napi_ok);
    //
    napi_value cipher_suite;
    status = napi_create_int64(env, lO_BucketInfo.encryption_parameters.cipher_suite, &cipher_suite);
    assert(status == napi_ok);
    //
    status = napi_set_named_property(env,encryption_parametersObj,"cipher_suite",cipher_suite);
    assert(status == napi_ok); 

    napi_value block_size;
    status = napi_create_int64(env, lO_BucketInfo.encryption_parameters.block_size, &block_size);
    assert(status == napi_ok);
    //
    status = napi_set_named_property(env,encryption_parametersObj,"block_size",block_size);
    assert(status == napi_ok); 

    status = napi_set_named_property(env,encryption_parametersObj,"encryption_parameters",encryption_parametersObj);
    assert(status == napi_ok); 
    //
    //Assign
    status = napi_set_named_property(env,bucketInfoObj,"encryption_parameters",encryption_parametersObj);
    assert(status == napi_ok);
  
    //XXXXXX 
    napi_value redundancy_schemeObj;
    status = napi_create_object(env,&redundancy_schemeObj);
    assert(status == napi_ok);
    //
    napi_value redundancySchemeshare_size;
    //X
    status = napi_create_int32(env, lO_BucketInfo.redundancy_scheme.share_size, &redundancySchemeshare_size);
    assert(status == napi_ok);
    //!
    status = napi_set_named_property(env,redundancy_schemeObj,"share_size",redundancySchemeshare_size);
    assert(status == napi_ok);
    //
    napi_value required_shares;
    int16_t required_sharesc = (int16_t)lO_BucketInfo.redundancy_scheme.required_shares;
    //X
    status = napi_create_int32(env, required_sharesc, &required_shares);
    assert(status == napi_ok);
    //!
    status = napi_set_named_property(env,redundancy_schemeObj,"required_shares",redundancySchemeshare_size);
    assert(status == napi_ok);
    //X
    napi_value repair_sharesNAPI;
    int16_t repair_shares = (int16_t)lO_BucketInfo.redundancy_scheme.repair_shares;
    status = napi_create_int32(env, repair_shares, &repair_sharesNAPI);
    assert(status == napi_ok);
    //!
    status = napi_set_named_property(env,redundancy_schemeObj,"repair_shares",repair_sharesNAPI);
    assert(status == napi_ok);
    //
    //X
    napi_value optimal_sharesNAPI;
    int16_t optimal_shares = (int16_t)lO_BucketInfo.redundancy_scheme.optimal_shares;
    status = napi_create_int32(env, optimal_shares, &optimal_sharesNAPI);
    assert(status == napi_ok);
    //!
    status = napi_set_named_property(env,redundancy_schemeObj,"optimal_shares",optimal_sharesNAPI);
    assert(status == napi_ok);
    //

    //X
    napi_value total_sharesNAPI;
    int16_t total_shares = (int16_t)lO_BucketInfo.redundancy_scheme.total_shares;
    status = napi_create_int32(env, total_shares, &total_sharesNAPI);
    assert(status == napi_ok);
    //!
    status = napi_set_named_property(env,redundancy_schemeObj,"total_shares",total_sharesNAPI);
    assert(status == napi_ok);
    //
  
    status = napi_set_named_property(env,bucketInfoObj,"redundancy_scheme",redundancy_schemeObj);
    assert(status == napi_ok);
    //XXXXXXXX
    //Create Bucket Info Project
    napi_value segment_size;
    //
    status = napi_create_int64(env, lO_BucketInfo.segment_size, &segment_size);
    assert(status == napi_ok);
    //
    status = napi_set_named_property(env,bucketInfoObj,"segment_size",segment_size);
    assert(status == napi_ok);

    status = napi_set_named_property(env,returnObject,"data",bucketInfoObj);
    assert(status == napi_ok);
  }else{
    char empty[] = "";
    napi_value emptyNAPI;
    status = napi_create_string_utf8(env,empty,NAPI_AUTO_LENGTH,&emptyNAPI);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,returnObject,"data",emptyNAPI);
    assert(status == napi_ok);
  }
  //Erro Stirng
  
  
  
  free_bucket_info(&lO_BucketInfo);
  return returnObject;
}

// * function to delete a bucket 
// * pre-requisites: open_projectc() function has been already called, successfully
// * inputs: valid ProjectRef, Bucket Name (string)
// * output: error if any else empty string 
napi_value delete_bucketc(napi_env env, napi_callback_info info) {
  //Status type
  napi_status status;
  
  //Number of arguments passed
  size_t argc = 2;
  napi_value args[2];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  //Checking number of arguments
  if (argc < 2) {
    napi_throw_type_error(env, nullptr, "Wrong number of arguments excepted 2 arguments");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  
  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"Please enter string value");
  }
  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  convertedvalue = convertedvalue+1;
  char bucketname[convertedvalue];  
  status = napi_get_value_string_utf8(env,args[1],&bucketname[0],convertedvalue,&bufsize);
  
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  char *_err = "";
  char **err = &_err;
  //
  ProjectRef lO_ProjectRef;
  lO_ProjectRef._handle = getHandleValue(env,args[0]);
  delete_bucket(lO_ProjectRef,bucketname,err);
  napi_value errorString;
  status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorString);
  assert(status == napi_ok);
  return errorString;
}

// * function to delete an object in a bucket
// * pre-requisites: open_bucket() function has been already called, successfully
// * inputs: valid BucketRef, Object Path (string)
// * output: Error (string) if any else None
napi_value delete_objectc(napi_env env, napi_callback_info info) {
  //
  napi_status status;
  //
  size_t argc = 2;
  napi_value args[2];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  //
  if (argc < 2) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype excepted string\n");
  }
  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  convertedvalue = convertedvalue+1;
  char objectname[convertedvalue];  
  status = napi_get_value_string_utf8(env,args[1],&objectname[0],convertedvalue,&bufsize);

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  char *_err = "";
  char **err = &_err;
  //
  BucketRef lO_BucketRef;
  lO_BucketRef._handle = getHandleValue(env,args[0]);
  delete_object(lO_BucketRef,objectname,err);
  napi_value errorString;
  status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorString);
  assert(status == napi_ok);
  return errorString;
}

// * function to close currently opened Storj project
// * pre-requisites: open_bucketc() function has been already called
// * inputs: Bucket Reference (BucketRef)
// * output: error if any else empty string
napi_value close_bucketc(napi_env env, napi_callback_info info) {
  //
  napi_status status;
  //
  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);
  //
  char *_err = "";
  char **err = &_err;
  BucketRef lO_BucketRef;
  lO_BucketRef._handle = getHandleValue(env,args[0]);
  close_bucket(lO_BucketRef,err);
  napi_value errorString;
  status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorString);
  assert(status == napi_ok);
  return errorString;
}

// * function to close currently opened Storj project
// * pre-requisites: open_projectc() function has been already called
// * inputs: Project Reference (ProjectRef)
// * output: error if any else empty string
napi_value close_projectc(napi_env env, napi_callback_info info) {
  //Status type
  napi_status status;
  
  //Number of arguments passed
  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  //Checking number of arguments
  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);
  //
  char *_err = "";
  char **err = &_err;
  ProjectRef lO_ProjectRef;
  lO_ProjectRef._handle = getHandleValue(env,args[0]);
  close_project(lO_ProjectRef,err);
  napi_value errorString;
  status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorString);
  assert(status == napi_ok);
  return errorString;
}

// * function to fetch a list of buckets, available in the Storj project
// * pre-requisites: open_bucketc() function has been already called
// * inputs: valid BucketRef
// * output: object contains error (if any ,else empty string) and data (ObjectList)
napi_value list_objectsc(napi_env env, napi_callback_info info) {
  //
  napi_status status;
  //
  size_t argc = 2;
  napi_value args[2];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 2) {
    napi_throw_type_error(env, nullptr, "Wrong number of arguments excepted 3 arguments");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  
  BucketRef lO_BucketRef;
  lO_BucketRef._handle = getHandleValue(env,args[0]);

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);
  char *_err = "";
  char **err = &_err;
  //
  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);
  //

  ListOptions lO_ListOptions;
  enum ListDirection lO_direction;
  ObjectList lO_ObjectList;
  //
  if(checktypeofinput==napi_null){
    lO_ObjectList = list_objects(lO_BucketRef,NULL,err);
  }else if(checktypeofinput==napi_object){
    napi_value propertyName;
    status = napi_get_property_names(env,args[1],&propertyName);
    assert(status == napi_ok);
    napi_value prefix,cursor,delimiter,recursive,direction,limit;
    status = napi_get_named_property(env,args[1],"prefix",&prefix);
    assert(status == napi_ok);
    
    //
    size_t bufsize = 0;
    size_t convertedvalue = 0;
    status = napi_get_value_string_utf8(env, prefix,NULL,bufsize,&convertedvalue);
    convertedvalue=convertedvalue+1;
    char prefixc[convertedvalue];  
    status = napi_get_value_string_utf8(env,prefix,&prefixc[0],convertedvalue,&bufsize);
    //
    lO_ListOptions.prefix = &prefixc[0];
    status = napi_get_named_property(env,args[1],"cursor",&cursor);
    assert(status == napi_ok);
    //
    //
    bufsize = 0;
    convertedvalue = 0;
    status = napi_get_value_string_utf8(env, cursor,NULL,bufsize,&convertedvalue);
    convertedvalue=convertedvalue+1;
    char cursorc[convertedvalue];  
    status = napi_get_value_string_utf8(env,cursor,&cursorc[0],convertedvalue,&bufsize);
    //
    lO_ListOptions.cursor = &cursorc[0];
    //
    status = napi_get_named_property(env,args[1],"delimiter",&delimiter);
    assert(status == napi_ok);
    
    bufsize = 0;
    convertedvalue = 0;
    status = napi_get_value_string_utf8(env, delimiter,NULL,bufsize,&convertedvalue);
    convertedvalue=convertedvalue+1;
    char delimiterc[convertedvalue];  

    status = napi_get_value_string_utf8(env,delimiter,&delimiterc[0],convertedvalue,&bufsize);
    
    lO_ListOptions.delimiter = delimiterc[0];
    
    status = napi_get_named_property(env,args[1],"recursive",&recursive);
    assert(status == napi_ok);
    
    bool recursivec;
    status = napi_get_boolean(env,recursive,&recursive);
    assert(status == napi_ok);
    
    lO_ListOptions.recursive = recursivec;
    
    status = napi_get_named_property(env,args[1],"direction",&direction);
    assert(status == napi_ok);  
    
    int64_t directionc;
    napi_get_value_int64(env,direction,&directionc);
    
    if(directionc==-2){
      lO_direction = STORJ_BEFORE;
    }else if(directionc==-1){
      lO_direction = STORJ_BACKWARD;
    }else if(directionc==1){
      lO_direction = STORJ_FORWARD;
    }else{
      lO_direction = STORJ_AFTER;
    }
    lO_ListOptions.direction = lO_direction;
    
    status = napi_get_named_property(env,args[1],"limit",&limit);
    assert(status == napi_ok);
    
    int64_t limitc;
    napi_get_value_int64(env,limit,&limitc);
    
    lO_ListOptions.limit = limitc;
    //
    lO_ObjectList = list_objects(lO_BucketRef,&lO_ListOptions,err);
  }
    
  napi_value returnObject;
  status = napi_create_object(env,&returnObject);
  assert(status == napi_ok);
  if(strlen(_err)==0&&lO_ObjectList.bucket!=NULL){
    //
    napi_value lO_objectlistNAPI;
    status = napi_create_object(env, &lO_objectlistNAPI);
    assert(status == napi_ok);
    //
    napi_value bucketName;
    status = napi_create_string_utf8(env,&lO_ObjectList.bucket[0],NAPI_AUTO_LENGTH,&bucketName);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,lO_objectlistNAPI,"bucket",bucketName);
    assert(status == napi_ok);
    
    napi_value prefix;
    status = napi_create_string_utf8(env,lO_ObjectList.prefix,NAPI_AUTO_LENGTH,&prefix);
    assert(status == napi_ok);
    //
    status = napi_set_named_property(env,lO_objectlistNAPI,"prefix",prefix);
    assert(status == napi_ok);
    
    int i=0;
    napi_value itemObj;
    status = napi_create_object(env,&itemObj);
    
    if(lO_ObjectList.length>0){ 
      for(i=0;i<lO_ObjectList.length;i++){
        napi_value itemDataObj;
        status = napi_create_object(env,&itemDataObj);
        assert(status == napi_ok);
      
        napi_value version;
        status = napi_create_uint32(env, lO_ObjectList.items[i].version, &version);
        assert(status == napi_ok);
      
        status = napi_set_named_property(env,itemDataObj,"version",version);
        assert(status == napi_ok);
        
        napi_value bucketInfoObj;
        status = napi_create_object(env,&bucketInfoObj);
        assert(status == napi_ok);
        
        napi_value bucketName;
        status = napi_create_string_utf8(env,lO_ObjectList.items[i].bucket.name,NAPI_AUTO_LENGTH,&bucketName);
        assert(status == napi_ok);
        
        status = napi_set_named_property(env,bucketInfoObj,"name",bucketName);
        assert(status == napi_ok);      
        
        napi_value createdBucket;
        status = napi_create_int64(env, lO_ObjectList.items[i].bucket.created, &createdBucket);
        assert(status == napi_ok);
      
        status = napi_set_named_property(env,bucketInfoObj,"created",createdBucket);
        assert(status == napi_ok);
        
        napi_value path_cipher;
        status = napi_create_int64(env, lO_ObjectList.items[i].bucket.path_cipher, &path_cipher);
        assert(status == napi_ok);
        
        status = napi_set_named_property(env,bucketInfoObj,"path_cipher",path_cipher);
        assert(status == napi_ok);
      
        napi_value encryption_parametersObj;
        status = napi_create_object(env,&encryption_parametersObj);
        assert(status == napi_ok);

        napi_value cipher_suite;
        status = napi_create_int64(env, lO_ObjectList.items[i].bucket.encryption_parameters.cipher_suite, &cipher_suite);
        assert(status == napi_ok);

        status = napi_set_named_property(env,encryption_parametersObj,"cipher_suite",cipher_suite);
        assert(status == napi_ok);

        napi_value block_size;
        status = napi_create_int64(env, lO_ObjectList.items[i].bucket.encryption_parameters.block_size, &block_size);
        assert(status == napi_ok);
      
        status = napi_set_named_property(env,encryption_parametersObj,"block_size",block_size);
        assert(status == napi_ok); 

        status = napi_set_named_property(env,encryption_parametersObj,"encryption_parameters",encryption_parametersObj);
        assert(status == napi_ok);  
      
        status = napi_set_named_property(env,bucketInfoObj,"encryption_parameters",encryption_parametersObj);
        assert(status == napi_ok);

        napi_value redundancy_schemeObj;
        status = napi_create_object(env,&redundancy_schemeObj);
        assert(status == napi_ok); 

        napi_value redundancySchemeshare_size;
        status = napi_create_int32(env, lO_ObjectList.items[i].bucket.redundancy_scheme.share_size, &redundancySchemeshare_size);
        assert(status == napi_ok);

        status = napi_set_named_property(env,redundancy_schemeObj,"share_size",redundancySchemeshare_size);
        assert(status == napi_ok);
        
        napi_value required_shares;
        int16_t required_sharesc = (int16_t)lO_ObjectList.items[i].bucket.redundancy_scheme.required_shares;

        status = napi_create_int32(env, required_sharesc, &required_shares);
        assert(status == napi_ok);

        status = napi_set_named_property(env,redundancy_schemeObj,"required_shares",redundancySchemeshare_size);
        assert(status == napi_ok);
        
        napi_value repair_sharesNAPI;
        int16_t repair_shares = (int16_t)lO_ObjectList.items[i].bucket.redundancy_scheme.repair_shares;
        status = napi_create_int32(env, repair_shares, &repair_sharesNAPI);
        assert(status == napi_ok);
      
        status = napi_set_named_property(env,redundancy_schemeObj,"repair_shares",repair_sharesNAPI);
        assert(status == napi_ok);
      
        napi_value optimal_sharesNAPI;
        int16_t optimal_shares = (int16_t)lO_ObjectList.items[i].bucket.redundancy_scheme.optimal_shares;
      
        status = napi_create_int32(env, optimal_shares, &optimal_sharesNAPI);
        assert(status == napi_ok);
      
        status = napi_set_named_property(env,redundancy_schemeObj,"optimal_shares",optimal_sharesNAPI);
        assert(status == napi_ok);
      
        napi_value total_sharesNAPI;
        int16_t total_shares = (int16_t)lO_ObjectList.items[i].bucket.redundancy_scheme.total_shares;
        status = napi_create_int32(env, total_shares, &total_sharesNAPI);
        assert(status == napi_ok);
      
        status = napi_set_named_property(env,redundancy_schemeObj,"total_shares",total_sharesNAPI);
        assert(status == napi_ok);
        
        status = napi_set_named_property(env,bucketInfoObj,"redundancy_scheme",redundancy_schemeObj);
        assert(status == napi_ok);
      
        napi_value segment_size;
        status = napi_create_int64(env, lO_ObjectList.items[i].bucket.segment_size, &segment_size);
        assert(status == napi_ok);
      
        status = napi_set_named_property(env,bucketInfoObj,"segment_size",segment_size);
        assert(status == napi_ok);
        
        status = napi_set_named_property(env,itemDataObj,"bucket",bucketInfoObj);
        assert(status == napi_ok);
      
        napi_value path;
        status = napi_create_string_utf8(env,lO_ObjectList.items[i].path,NAPI_AUTO_LENGTH,&path);
        assert(status == napi_ok);
      
        status = napi_set_named_property(env,itemDataObj,"prefix",path);
        assert(status == napi_ok);

        napi_value content_type;
        status = napi_create_string_utf8(env,lO_ObjectList.items[i].content_type,NAPI_AUTO_LENGTH,&content_type);
        assert(status == napi_ok);
      
        status = napi_set_named_property(env,itemDataObj,"content_type",content_type);
        assert(status == napi_ok);
        
        napi_value size;
        status = napi_create_int64(env, lO_ObjectList.items[i].size, &size);
        assert(status == napi_ok);
      
        status = napi_set_named_property(env,itemDataObj,"size",size);
        assert(status == napi_ok);
      
        napi_value created;
        status = napi_create_int64(env, lO_ObjectList.items[i].created, &created);
        assert(status == napi_ok);
      
        status = napi_set_named_property(env,itemDataObj,"created",created);
        assert(status == napi_ok);
      
        napi_value modified;
        status = napi_create_int64(env, lO_ObjectList.items[i].modified, &modified);
        assert(status == napi_ok);
      
        status = napi_set_named_property(env,itemDataObj,"modified",modified);
        assert(status == napi_ok);
      
        napi_value expires;
        status = napi_create_int64(env, lO_ObjectList.items[i].expires, &expires);
        assert(status == napi_ok);
      
        status = napi_set_named_property(env,itemDataObj,"expires",expires);
        assert(status == napi_ok);
        
        char str[100];
        itoa(i, str, 10);
        status = napi_set_named_property(env,itemObj,str,itemDataObj);
        assert(status == napi_ok);
      }
    }
    
    status = napi_set_named_property(env,lO_objectlistNAPI,"items",itemObj);
    assert(status == napi_ok);
    
    napi_value lengthNAPI;
    status = napi_create_int32(env, lO_ObjectList.length, &lengthNAPI);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,lO_objectlistNAPI,"length",lengthNAPI);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,returnObject,"data",lO_objectlistNAPI);
    assert(status == napi_ok);
    
    napi_value errorString;
    status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorString);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,returnObject,"error",errorString);
    assert(status == napi_ok);
    free_list_objects(&lO_ObjectList);
    return returnObject;
  }else{
    napi_value errorString;
    status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorString);
    assert(status == napi_ok);

    char data[]="";
    napi_value dataNAPI;
    status = napi_create_string_utf8(env,&data[0],NAPI_AUTO_LENGTH,&dataNAPI);
    assert(status == napi_ok);
    status = napi_set_named_property(env,returnObject,"data",dataNAPI);
    assert(status == napi_ok);
    status = napi_set_named_property(env,returnObject,"error",errorString);
    assert(status == napi_ok);
    free_list_objects(&lO_ObjectList);
    return returnObject; 
  }
  
  napi_value emptyvalue;
  return emptyvalue;
}
// * function to fetch a list of buckets, available in the Storj project
// * pre-requisites: open_projectc() function has been already called
// * inputs: valid ProjectRef
// * output: object contains error (if any ,else empty string) and data (BucketList)
napi_value list_bucketsc(napi_env env, napi_callback_info info) {
  
  napi_status status;
  
  size_t argc = 2;
  napi_value args[2];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
   
  if (argc < 2) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);
  if(checktypeofinput!=napi_object)
  {
    napi_throw_type_error(env, nullptr, "\nWrong datatype excepted object\n");
  }
  
  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  char *_err = "";
  char **err = &_err;
  ProjectRef lO_ProjectRef;
  lO_ProjectRef._handle = getHandleValue(env,args[0]);
  
  BucketListOptions lO_BucketListOptions; 
  
  BucketList lO_bucketlist;

  if(checktypeofinput==napi_null){
    lO_bucketlist = list_buckets(lO_ProjectRef,NULL,err);
  }else if(checktypeofinput==napi_object){


    lO_bucketlist = list_buckets(lO_ProjectRef,&lO_BucketListOptions,err);
  }
  else
  {
    napi_value cursorNAPI;
    status = napi_get_named_property(env,args[1],"cursor",&cursorNAPI);
    assert(status == napi_ok);
    size_t bufsize = 0;
    size_t convertedvalue = 0;
    status = napi_get_value_string_utf8(env, cursorNAPI,NULL,bufsize,&convertedvalue);
    convertedvalue=convertedvalue+1;
    char cursor[convertedvalue];
    status = napi_get_value_string_utf8(env,cursorNAPI,&cursor[0],convertedvalue,&bufsize);
    lO_BucketListOptions.cursor = &cursor[0];

    napi_value directionNAPI;
    status = napi_get_named_property(env,args[1],"direction",&directionNAPI);
    assert(status == napi_ok);

    int direction;
    status = napi_get_value_int32(env,directionNAPI,&direction);
    assert(status == napi_ok);

    ListDirection listdirectionvalue;
    if(direction==-2){
      listdirectionvalue = STORJ_BEFORE;
    }else if(direction==-1){
      listdirectionvalue = STORJ_BACKWARD;
    }else if(direction==1){
      listdirectionvalue = STORJ_FORWARD;
    }else{
      listdirectionvalue = STORJ_AFTER;
    }

    lO_BucketListOptions.direction = listdirectionvalue;
    napi_value limitNAPI;
    status = napi_get_named_property(env,args[1],"limit",&limitNAPI);
    assert(status == napi_ok);

    int limit;
    status = napi_get_value_int32(env,limitNAPI,&limit);
    assert(status == napi_ok);
    lO_BucketListOptions.limit = limit;
    napi_throw_type_error(env, nullptr, "\nWrong datatype excepted object\n");
  }
  
  
  napi_value returnObject;
  status = napi_create_object(env,&returnObject);
  assert(status == napi_ok);  
  if(strlen(_err)==0){
    
    napi_value lO_bucketlistNAPI;
    status = napi_create_object(env, &lO_bucketlistNAPI);
    assert(status == napi_ok);
    
    int i=0;
    napi_value itemObj;
    status = napi_create_object(env,&itemObj);
    
    for(i=0;i<lO_bucketlist.length;i++){
      napi_value itemDataObj;
      status = napi_create_object(env,&itemDataObj);
      
      napi_value bucketName;
      status = napi_create_string_utf8(env,lO_bucketlist.items[i].name,NAPI_AUTO_LENGTH,&bucketName);
      assert(status == napi_ok);
      
      status = napi_set_named_property(env,itemDataObj,"name",bucketName);
      assert(status == napi_ok);
      
      napi_value created;
      status = napi_create_int64(env, lO_bucketlist.items[i].created, &created);
      assert(status == napi_ok);
      
      status = napi_set_named_property(env,itemDataObj,"created",created);
      assert(status == napi_ok);
      
      napi_value path_cipher;
      status = napi_create_int64(env, lO_bucketlist.items[i].path_cipher, &path_cipher);
      assert(status == napi_ok);
      
      status = napi_set_named_property(env,itemDataObj,"path_cipher",path_cipher);
      assert(status == napi_ok);
      
      napi_value encryption_parametersObj;
      status = napi_create_object(env,&encryption_parametersObj);
      assert(status == napi_ok);
      
      napi_value block_size;
      status = napi_create_int64(env, lO_bucketlist.items[i].encryption_parameters.block_size, &block_size);
      assert(status == napi_ok);
      
      status = napi_set_named_property(env,encryption_parametersObj,"block_size",block_size);
      assert(status == napi_ok); 
      
      status = napi_set_named_property(env,encryption_parametersObj,"encryption_parameters",encryption_parametersObj);
      assert(status == napi_ok); 
      
      status = napi_set_named_property(env,itemDataObj,"encryption_parameters",encryption_parametersObj);
      assert(status == napi_ok);

      napi_value redundancy_schemeObj;
      status = napi_create_object(env,&redundancy_schemeObj);
      assert(status == napi_ok);
      
      napi_value redundancySchemeshare_size;
      
      status = napi_create_int32(env, lO_bucketlist.items[i].redundancy_scheme.share_size, &redundancySchemeshare_size);
      assert(status == napi_ok);
      
      status = napi_set_named_property(env,redundancy_schemeObj,"share_size",redundancySchemeshare_size);
      assert(status == napi_ok);
      
      napi_value required_shares;
      int16_t required_sharesc = (int16_t)lO_bucketlist.items[i].redundancy_scheme.required_shares;
      
      status = napi_create_int32(env, required_sharesc, &required_shares);
      assert(status == napi_ok);
      
      status = napi_set_named_property(env,redundancy_schemeObj,"required_shares",redundancySchemeshare_size);
      assert(status == napi_ok);
    
      napi_value repair_sharesNAPI;
      int16_t repair_shares = (int16_t)lO_bucketlist.items[i].redundancy_scheme.repair_shares;
      status = napi_create_int32(env, repair_shares, &repair_sharesNAPI);
      assert(status == napi_ok);
      
      status = napi_set_named_property(env,redundancy_schemeObj,"repair_shares",repair_sharesNAPI);
      assert(status == napi_ok);

      napi_value optimal_sharesNAPI;
      int16_t optimal_shares = (int16_t)lO_bucketlist.items[i].redundancy_scheme.optimal_shares;
      status = napi_create_int32(env, optimal_shares, &optimal_sharesNAPI);
      assert(status == napi_ok);

      status = napi_set_named_property(env,redundancy_schemeObj,"optimal_shares",optimal_sharesNAPI);
      assert(status == napi_ok);

      napi_value total_sharesNAPI;
      int16_t total_shares = (int16_t)lO_bucketlist.items[i].redundancy_scheme.total_shares;
      status = napi_create_int32(env, total_shares, &total_sharesNAPI);
      assert(status == napi_ok);
      
      status = napi_set_named_property(env,redundancy_schemeObj,"total_shares",total_sharesNAPI);
      assert(status == napi_ok);

      status = napi_set_named_property(env,itemDataObj,"redundancy_scheme",redundancy_schemeObj);
      assert(status == napi_ok);
      
      napi_value segment_size;
      
      status = napi_create_int64(env, lO_bucketlist.items[i].segment_size, &segment_size);
      assert(status == napi_ok);
      
      status = napi_set_named_property(env,itemDataObj,"segment_size",segment_size);
      assert(status == napi_ok);
      
      napi_value cipher_suite;
      status = napi_create_int64(env, lO_bucketlist.items[i].encryption_parameters.cipher_suite, &cipher_suite);
      assert(status == napi_ok);
      
      status = napi_set_named_property(env,encryption_parametersObj,"cipher_suite",cipher_suite);
      assert(status == napi_ok); 
      
      char str[100];
      itoa(i, str, 10);
      status = napi_set_named_property(env,itemObj,str,itemDataObj);
      assert(status == napi_ok);
    }
    
    status = napi_set_named_property(env,lO_bucketlistNAPI,"items",itemObj);
    assert(status == napi_ok);
    
    napi_value lengthNAPI;
    int16_t length = (int16_t)lO_bucketlist.length;
    status = napi_create_int32(env, length, &lengthNAPI);
    assert(status == napi_ok);

    status = napi_set_named_property(env,lO_bucketlistNAPI,"length",lengthNAPI);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,returnObject,"data",lO_bucketlistNAPI);
    assert(status == napi_ok);

    napi_value errorString;
    status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorString);
    assert(status == napi_ok);

    status = napi_set_named_property(env,returnObject,"error",errorString);
    assert(status == napi_ok);
    return returnObject;
  
  }else{
    napi_value errorString;
    status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorString);
    assert(status == napi_ok);

    status = napi_set_named_property(env,returnObject,"error",errorString);
    assert(status == napi_ok);
    free_bucket_list(&lO_bucketlist);
    return returnObject;
  }
  
}

// * function to creates new Scope 
// * pre-requisites: parse_api_keyc() and get_encryption_accessc() functions have been already called
// * inputs: satellite address (string), api key refernce (APIKeyRef),  serialized encryption access (EncryptionAccessRef)
// * output: object contains error (if any ,else empty string) and data (ScopeRef)
napi_value new_scopec(napi_env env, napi_callback_info info) {
  
  napi_status status;
  
  size_t argc = 3;
  napi_value args[3];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  napi_valuetype checktypeofinput;
  
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype excepted string\n");
  }
  //
  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[0],NULL,bufsize,&convertedvalue);
  convertedvalue=convertedvalue+1;
  char satellite_addr[convertedvalue];  
  status = napi_get_value_string_utf8(env,args[0],&satellite_addr[0],convertedvalue,&bufsize);
  //
  char *_err = "";
  char **err = &_err;
  APIKeyRef lO_APIKeyRef;
  lO_APIKeyRef._handle = getHandleValue(env,args[1]);
  //
  EncryptionAccessRef lO_EncryptionAccessRef;
  lO_EncryptionAccessRef._handle = getHandleValue(env,args[2]);
  //
  ScopeRef lO_ScopeRef = new_scope(&satellite_addr[0],lO_APIKeyRef,lO_EncryptionAccessRef,err);
  //
  return createhandle(env,lO_ScopeRef._handle, _err);
}

// * function to restricts a given scope with the provided caveat and encryption restrictions
// * pre-requisites: new_scope() function has been already called
// * inputs: scope reference (ScopeRef) , caveat(object),encryptionRestriction(object),restrictionsLen(number)
// * output: object contains error (if any ,else empty string) and data (ScopeRef)
napi_value restrict_scopec(napi_env env, napi_callback_info info) {
  
  napi_status status;
  
  size_t argc = 3;
  napi_value args[3];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 3) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }
  Caveat lO_Caveat;
  //
  napi_value disallow_readsNAPI;
  status = napi_get_named_property(env,args[1],"disallow_reads",&disallow_readsNAPI);
  assert(status == napi_ok);
  //
  bool disallow_readsc;
  status = napi_get_value_bool(env,disallow_readsNAPI,&disallow_readsc);
  assert(status == napi_ok);
  //
  lO_Caveat.disallow_reads = disallow_readsc;
  //
  napi_value disallow_writesNAPI;
  status = napi_get_named_property(env,args[1],"disallow_writes",&disallow_writesNAPI);
  assert(status == napi_ok);
  
  //
  bool disallow_writesc;
  status = napi_get_value_bool(env,disallow_writesNAPI,&disallow_writesc);
  assert(status == napi_ok);
  //
  lO_Caveat.disallow_writes = disallow_writesc;
  //
  napi_value disallow_listsNAPI;
  status = napi_get_named_property(env,args[1],"disallow_lists",&disallow_listsNAPI);
  assert(status == napi_ok);
  //
  bool disallow_listsc;
  status = napi_get_value_bool(env,disallow_listsNAPI,&disallow_listsc);
  assert(status == napi_ok);
  //
  //
  lO_Caveat.disallow_lists = disallow_listsc;
  //
  napi_value disallow_deletesNAPI;
  status = napi_get_named_property(env,args[1],"disallow_deletes",&disallow_deletesNAPI);
  assert(status == napi_ok);
  //
  bool disallow_deletesc;
  status = napi_get_value_bool(env,disallow_deletesNAPI,&disallow_deletesc);
  assert(status == napi_ok);
  //
  //
  lO_Caveat.disallow_deletes = disallow_deletesc;
  //
  napi_value propertyName;
  status = napi_get_property_names(env,args[2],&propertyName);
  assert(status == napi_ok);
  //
  uint32_t restrictionsSize;
  status = napi_get_array_length(env,args[2],&restrictionsSize);
  assert(status == napi_ok);
  //
  EncryptionRestriction restrictions[restrictionsSize];
  //
  napi_value EncryptionRestrictionObject;
  //
  for(uint32_t i=0;i<restrictionsSize;i++){
    EncryptionRestriction lO_EncryptionRestriction;

    status = napi_get_element(env,args[2],i,&EncryptionRestrictionObject);
    assert(status == napi_ok);

    napi_value bucket;
    status = napi_get_named_property(env,EncryptionRestrictionObject,"bucket",&bucket);
    assert(status == napi_ok);
    //
    size_t bufsize = 0;
    size_t convertedvalue = 0;
    status = napi_get_value_string_utf8(env, args[0],NULL,bufsize,&convertedvalue);
    convertedvalue = convertedvalue+1;
    char bucketc[convertedvalue];
    status = napi_get_value_string_utf8(env,args[0],&bucketc[0],convertedvalue,&bufsize);

    napi_value path_prefix;
    status = napi_get_named_property(env,EncryptionRestrictionObject,"path_prefix",&path_prefix);
    assert(status == napi_ok);
    //
    bufsize = 0;
    convertedvalue = 0;
    status = napi_get_value_string_utf8(env, args[0],NULL,bufsize,&convertedvalue);
    convertedvalue = convertedvalue+1;
    char path_prefixc[convertedvalue];
    status = napi_get_value_string_utf8(env,args[0],&path_prefixc[0],convertedvalue,&bufsize);
    
    lO_EncryptionRestriction.bucket = bucketc;
    lO_EncryptionRestriction.path_prefix = path_prefixc;
    restrictions[i] = lO_EncryptionRestriction;
  }
  
  napi_valuetype checktypeofinput;
  
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"Please Enter Object Type");
  }
  
  char *_err = "";
  char **err = &_err;
  
  ScopeRef lO_ScopeRef;
  lO_ScopeRef._handle = getHandleValue(env,args[0]);
  
  EncryptionRestriction* ptrTOrestriction = &restrictions[1];
  
  ScopeRef restrictedScope = restrict_scope(lO_ScopeRef,lO_Caveat,&ptrTOrestriction,1,err);
  
  return createhandle(env,restrictedScope._handle, _err);
}

// * function to get Scope APIKey
// * pre-requisites: new_scopec() function has been already called
// * inputs: scope Reference (ScopeRef)
// * output: object contains error (if any ,else empty string) and data (APIKeyRef)
napi_value get_scope_api_keyc(napi_env env, napi_callback_info info) {
  //
  napi_status status;
  //
  size_t argc = 1;
  napi_value args[1];
  //
  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  //
  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  //

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype excepted object\n");
  }

  char *_err = "";
  char **err = &_err;
  ScopeRef lO_ScopeRef;
  lO_ScopeRef._handle = getHandleValue(env,args[0]);
  //
  APIKeyRef lO_APIKeyRef = get_scope_api_key(lO_ScopeRef,err);
  return createhandle(env,lO_APIKeyRef._handle, _err);
}

// * function to get Scope encryption access
// * pre-requisites: new_scopec() function has been already called
// * inputs: Scope Reference (ScopeRef)
// * output: object contains error (if any ,else empty string) and data (EncryptionAccessRef)
napi_value get_scope_enc_accessc(napi_env env, napi_callback_info info) {
  
  napi_status status;
  
  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  //
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype excepted object\n");
  }
  
  char *_err = "";
  char **err = &_err;
  ScopeRef lO_ScopeRef;
  lO_ScopeRef._handle = getHandleValue(env,args[0]);
  //
  EncryptionAccessRef lO_EncryptionAccessRef = get_scope_enc_access(lO_ScopeRef,err);
  return createhandle(env,lO_EncryptionAccessRef._handle, _err);
}
//
// * function to parses an Scope
// * pre-requisites: new_scopec() function has been already called
// * inputs: ScopeRef Reference (ScopeRef)
// * output: object contains error (if any ,else empty string) and data (ScopeRef)
napi_value parse_scopec(napi_env env, napi_callback_info info) {
  
  napi_status status;
  
  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype excepted string\n");
  }
  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[0],NULL,bufsize,&convertedvalue);
  convertedvalue=convertedvalue+1;
  char scopeValue[convertedvalue];  
  status = napi_get_value_string_utf8(env,args[0],&scopeValue[0],convertedvalue,&bufsize);
  
  char *_err = "";
  char **err = &_err;
  //
  ScopeRef lO_ScopeRef = parse_scope(&scopeValue[0],err);
  return createhandle(env,lO_ScopeRef._handle, _err);
}

// * function to serializes the Scope to a string
// * pre-requisites: new_scopec() function has been already called
// * inputs: Scope Reference (ScopeRef)
// * output: object contains error (if any ,else empty string) and data (String)
napi_value serialize_scopec(napi_env env, napi_callback_info info) {
  
  napi_status status;
  
  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype excepted object\n");
  }
  
  char *_err = "";
  char **err = &_err;
  ScopeRef lO_ScopeRef;
  lO_ScopeRef._handle = getHandleValue(env,args[0]);
  
  char* serializedScopePtr = serialize_scope(lO_ScopeRef,err);
  
  napi_value serializedScope;
  status = napi_create_string_utf8(env,serializedScopePtr,NAPI_AUTO_LENGTH,&serializedScope);
  assert(status == napi_ok);
  
  napi_value returnObject;
  status = napi_create_object(env,&returnObject);
  
  napi_value errorString;
  status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorString);
  assert(status == napi_ok);
  
  status = napi_set_named_property(env,returnObject,"error",errorString);
  assert(status == napi_ok);
  
  status = napi_set_named_property(env,returnObject,"data",serializedScope);
  assert(status == napi_ok);
  
  return returnObject;
}

// * function to gets Scope satellite address
// * pre-requisites: new_scopec() function has been already called
// * inputs: Scope Reference (ScopeRef)
// * output: object contains error (if any ,else empty string) and data (string)
napi_value get_scope_satellite_addressc(napi_env env, napi_callback_info info) {
  
  napi_status status;
  
  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  //
  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype excepted object\n");
  }
  
  char *_err = "";
  char **err = &_err;
  ScopeRef lO_ScopeRef;
  lO_ScopeRef._handle = getHandleValue(env,args[0]);
  //
  char* SatelliteAddrPtr = get_scope_satellite_address(lO_ScopeRef,err);
  //
  napi_value SatelliteAddr;
  status = napi_create_string_utf8(env,SatelliteAddrPtr,NAPI_AUTO_LENGTH,&SatelliteAddr);
  assert(status == napi_ok);
  //
  napi_value returnObject;
  status = napi_create_object(env,&returnObject);
  //
  napi_value errorString;
  status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorString);
  assert(status == napi_ok);
  //
  status = napi_set_named_property(env,returnObject,"error",errorString);
  assert(status == napi_ok);
  //
  status = napi_set_named_property(env,returnObject,"data",SatelliteAddr);
  assert(status == napi_ok);
  //
  return returnObject;
}

// * function to an Object handle, if authorized
// * pre-requisites: open_bucket() function has been already called
// * inputs: Bucket Reference (BucketRef),objectPath(string)
// * output: object contains error (if any ,else empty string) and data (ObjectRef)
napi_value open_objectc(napi_env env, napi_callback_info info) {
  
  napi_status status;
  
  size_t argc = 2;
  napi_value args[2];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 2) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype excepted object\n");
  }


  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype excepted string\n");
  }

  size_t bufsize = 0;
  size_t convertedvalue = 0;
  
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  convertedvalue = convertedvalue+1;
  char objectName[convertedvalue];
  status = napi_get_value_string_utf8(env,args[1],&objectName[0],convertedvalue,&bufsize);
  
  
  char *_err = "";
  char **err = &_err;
  BucketRef lO_BucketRef;
  lO_BucketRef._handle = getHandleValue(env,args[0]);

  ObjectRef lO_ObjectRef = open_object(lO_BucketRef,&objectName[0],err);
  return createhandle(env,lO_ObjectRef._handle,_err);
}

// * function to closes the object
// * pre-requisites: open_objectc() function has been already called
// * inputs: object Reference (ObjectRef)
// * output: error if any else empty string
napi_value close_objectc(napi_env env, napi_callback_info info) {
  
  napi_status status;
  
  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype excepted object\n");
  }
  
  char *_err = "";
  char **err = &_err;
  ObjectRef lO_ObjectRef;
  lO_ObjectRef._handle = getHandleValue(env,args[0]);
  //
  close_object(lO_ObjectRef,err);
  //
  napi_value errorString;
  status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorString);
  assert(status == napi_ok);
  return errorString;
}

// * function to close downloader after completing the data read process
// * pre-requisites: open_object() function has been already called
// * inputs: object Reference (ObjectRef)
// * output: object contains error (if any ,else empty string) and data (ObjectMeta)
napi_value get_object_metac(napi_env env, napi_callback_info info) {
  //
  napi_status status;
  //
  size_t argc = 1;
  napi_value args[1];
  //
  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  //
  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!!\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  //
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype excepted object\n");
  }
  
  char *_err = "";
  char **err = &_err;
  ObjectRef lO_ObjectRef;
  lO_ObjectRef._handle = getHandleValue(env,args[0]);
  //
  ObjectMeta lO_ObjectMeta = get_object_meta(lO_ObjectRef,err);
  //
  napi_value returnObject,objectMetaNAPI;
  //
  status = napi_create_object(env,&returnObject);
  assert(status == napi_ok);
  //
  status = napi_create_object(env,&objectMetaNAPI);
  assert(status == napi_ok);
  //
  napi_value bucketname;
  napi_create_string_utf8(env,&lO_ObjectMeta.bucket[0],NAPI_AUTO_LENGTH,&bucketname);
  assert(status == napi_ok);
  //>>>>>>>
  status = napi_set_named_property(env,objectMetaNAPI,"bucket",bucketname);
  assert(status == napi_ok);
  //>>>>>>>
  napi_value pathAPI;
  napi_create_string_utf8(env,&lO_ObjectMeta.path[0],NAPI_AUTO_LENGTH,&pathAPI);
  assert(status == napi_ok);
  //
  status = napi_set_named_property(env,objectMetaNAPI,"path",pathAPI);
  assert(status == napi_ok);
  //>>>>>>>

  napi_value is_prefixNAPI;
  if(lO_ObjectMeta.is_prefix)
  {
    napi_create_string_utf8(env,"true",4,&is_prefixNAPI);
    assert(status == napi_ok);
  }else{
    napi_create_string_utf8(env,"false",5,&is_prefixNAPI);
    assert(status == napi_ok);
  }
  
  //
  status = napi_set_named_property(env,objectMetaNAPI,"is_prefix",objectMetaNAPI);
  assert(status == napi_ok);
  //>>>>>>>
  napi_value content_typeNAPI;
  napi_create_string_utf8(env,&lO_ObjectMeta.content_type[0],NAPI_AUTO_LENGTH,&content_typeNAPI);
  assert(status == napi_ok);
  //
  status = napi_set_named_property(env,objectMetaNAPI,"content_type",content_typeNAPI);
  assert(status == napi_ok);
  //>>>>>>>
  napi_value created;
  napi_create_int64(env, lO_ObjectMeta.created, &created);
  assert(status == napi_ok);

  status = napi_set_named_property(env,objectMetaNAPI,"created",created);
  assert(status == napi_ok);

  napi_value modified;
  napi_create_int64(env, lO_ObjectMeta.modified, &modified);
  assert(status == napi_ok);
  //>>>>>>>
  status = napi_set_named_property(env,objectMetaNAPI,"modified",modified);
  assert(status == napi_ok);
  //
  napi_value expires;
  napi_create_int64(env, lO_ObjectMeta.expires, &expires);
  assert(status == napi_ok);
  //>>>>>>>
  status = napi_set_named_property(env,objectMetaNAPI,"expires",expires);
  assert(status == napi_ok);
  //
  napi_value size;
  napi_create_int64(env, lO_ObjectMeta.size, &size);
  assert(status == napi_ok);
  //>>>>>>>
  status = napi_set_named_property(env,objectMetaNAPI,"size",size);
  assert(status == napi_ok);
  //
  //>>>>>>>
  status = napi_set_named_property(env,objectMetaNAPI,"checksum_bytes",size);
  assert(status == napi_ok);
  //
  napi_value checksum_length;
  napi_create_int64(env, lO_ObjectMeta.checksum_length, &checksum_length);
  assert(status == napi_ok);
  //>>>>>>>
  status = napi_set_named_property(env,objectMetaNAPI,"checksum_length",checksum_length);
  assert(status == napi_ok);
  //
  napi_value errorString;
  status = napi_create_string_utf8(env,_err,NAPI_AUTO_LENGTH,&errorString);
  assert(status == napi_ok);
  //
  status = napi_set_named_property(env,returnObject,"error",errorString);
  assert(status == napi_ok);
  //
  status = napi_set_named_property(env,returnObject,"data",objectMetaNAPI);
  assert(status == napi_ok);
  //
  return returnObject;
}

#define DECLARE_NAPI_METHOD(name, func)                                        \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor new_uplink = DECLARE_NAPI_METHOD("new_uplinkc", new_uplinkc);
  status = napi_define_properties(env, exports, 1, &new_uplink);
  assert(status == napi_ok);

  napi_property_descriptor close_uplink = DECLARE_NAPI_METHOD("close_uplinkc", close_uplinkc);
  status = napi_define_properties(env, exports, 1, &close_uplink);
  assert(status == napi_ok);
  
  napi_property_descriptor parse_api_key = DECLARE_NAPI_METHOD("parse_api_keyc", parse_api_keyc);
  status = napi_define_properties(env, exports, 1, &parse_api_key);
  assert(status == napi_ok);

  napi_property_descriptor open_project = DECLARE_NAPI_METHOD("open_projectc", open_projectc);
  status = napi_define_properties(env, exports, 1, &open_project);
  assert(status == napi_ok);

  napi_property_descriptor project_salted_key_from_passphrase = DECLARE_NAPI_METHOD("project_salted_key_from_passphrasec", project_salted_key_from_passphrasec);
  status = napi_define_properties(env, exports, 1, &project_salted_key_from_passphrase);
  assert(status == napi_ok);

  napi_property_descriptor new_encryption_access_with_default_key = DECLARE_NAPI_METHOD("new_encryption_access_with_default_keyc", new_encryption_access_with_default_keyc);
  status = napi_define_properties(env, exports, 1, &new_encryption_access_with_default_key);
  assert(status == napi_ok);

  napi_property_descriptor serialize_encryption_access = DECLARE_NAPI_METHOD("serialize_encryption_accessc", serialize_encryption_accessc);
  status = napi_define_properties(env, exports, 1, &serialize_encryption_access);
  assert(status == napi_ok);
  
  napi_property_descriptor encryption_access = DECLARE_NAPI_METHOD("encryption_accessc", encryption_accessc);
  status = napi_define_properties(env, exports, 1, &encryption_access);
  assert(status == napi_ok);

  napi_property_descriptor open_bucket = DECLARE_NAPI_METHOD("open_bucketc", open_bucketc);
  status = napi_define_properties(env, exports, 1, &open_bucket);
  assert(status == napi_ok);

  napi_property_descriptor upload = DECLARE_NAPI_METHOD("uploadc", uploadc);
  status = napi_define_properties(env, exports, 1, &upload);
  assert(status == napi_ok);

  napi_property_descriptor upload_write = DECLARE_NAPI_METHOD("upload_writec", upload_writec);
  status = napi_define_properties(env, exports, 1, &upload_write);
  assert(status == napi_ok);
  
  //upload_commit
  napi_property_descriptor upload_commit = DECLARE_NAPI_METHOD("upload_commitc", upload_commitc);
  status = napi_define_properties(env, exports, 1, &upload_commit);
  assert(status == napi_ok);

  napi_property_descriptor download = DECLARE_NAPI_METHOD("downloadc", downloadc);
  status = napi_define_properties(env, exports, 1, &download);
  assert(status == napi_ok);
  
  napi_property_descriptor download_read = DECLARE_NAPI_METHOD("download_readc", download_readc);
  status = napi_define_properties(env, exports, 1, &download_read);
  assert(status == napi_ok);

  //download_readc
  napi_property_descriptor download_close = DECLARE_NAPI_METHOD("download_closec", download_closec);
  status = napi_define_properties(env, exports, 1, &download_close);
  assert(status == napi_ok);
  
    //New Start
  //Create bucket
  napi_property_descriptor create_bucket = DECLARE_NAPI_METHOD("create_bucketc", create_bucketc);
  status = napi_define_properties(env, exports, 1, &create_bucket);
  assert(status == napi_ok);
  
  //Delete bucket
  napi_property_descriptor delete_bucket = DECLARE_NAPI_METHOD("delete_bucketc", delete_bucketc);
  status = napi_define_properties(env, exports, 1, &delete_bucket);
  assert(status == napi_ok);

  //Delete Object
  napi_property_descriptor delete_object = DECLARE_NAPI_METHOD("delete_objectc", delete_objectc);
  status = napi_define_properties(env, exports, 1, &delete_object);
  assert(status == napi_ok);

  //Close Project
  napi_property_descriptor close_bucket = DECLARE_NAPI_METHOD("close_bucketc", close_bucketc);
  status = napi_define_properties(env, exports, 1, &close_bucket);
  assert(status == napi_ok);

  //Close Bucket
  napi_property_descriptor close_project = DECLARE_NAPI_METHOD("close_projectc", close_projectc);
  status = napi_define_properties(env, exports, 1, &close_project);
  assert(status == napi_ok);
  
  //List Object
  napi_property_descriptor list_objects = DECLARE_NAPI_METHOD("list_objectsc", list_objectsc);
  status = napi_define_properties(env, exports, 1, &list_objects);
  assert(status == napi_ok);
  /**/
  //List Bucket
  napi_property_descriptor list_buckets = DECLARE_NAPI_METHOD("list_bucketsc", list_bucketsc);
  status = napi_define_properties(env, exports, 1, &list_buckets);
  assert(status == napi_ok);

  //new_scope
  napi_property_descriptor new_scope = DECLARE_NAPI_METHOD("new_scopec", new_scopec);
  status = napi_define_properties(env, exports, 1, &new_scope);
  assert(status == napi_ok);
  //restrict_scope
  napi_property_descriptor restrict_scope = DECLARE_NAPI_METHOD("restrict_scopec", restrict_scopec);
  status = napi_define_properties(env, exports, 1, &restrict_scope);
  assert(status == napi_ok);
  //20
  napi_property_descriptor get_scope_api_key = DECLARE_NAPI_METHOD("get_scope_api_keyc", get_scope_api_keyc);
  status = napi_define_properties(env, exports, 1, &get_scope_api_key);
  assert(status == napi_ok);
  //get_scope_api_key
  napi_property_descriptor get_scope_enc_access = DECLARE_NAPI_METHOD("get_scope_enc_accessc", get_scope_enc_accessc);
  status = napi_define_properties(env, exports, 1, &get_scope_enc_access);
  assert(status == napi_ok);
  //parse_scope
  napi_property_descriptor parse_scope = DECLARE_NAPI_METHOD("parse_scopec", parse_scopec);
  status = napi_define_properties(env, exports, 1, &parse_scope);
  assert(status == napi_ok);
  //
  napi_property_descriptor serialize_scope = DECLARE_NAPI_METHOD("serialize_scopec", serialize_scopec);
  status = napi_define_properties(env, exports, 1, &serialize_scope);
  assert(status == napi_ok);
  //
  napi_property_descriptor get_scope_satellite_address = DECLARE_NAPI_METHOD("get_scope_satellite_addressc", get_scope_satellite_addressc);
  status = napi_define_properties(env, exports, 1, &get_scope_satellite_address);
  assert(status == napi_ok);
  //
  napi_property_descriptor open_object = DECLARE_NAPI_METHOD("open_objectc", open_objectc);
  status = napi_define_properties(env, exports, 1, &open_object);
  assert(status == napi_ok);
  //
  napi_property_descriptor close_object = DECLARE_NAPI_METHOD("close_objectc", close_objectc);
  status = napi_define_properties(env, exports, 1, &close_object);
  assert(status == napi_ok);
  //
  napi_property_descriptor get_object_meta = DECLARE_NAPI_METHOD("get_object_metac", get_object_metac);
  status = napi_define_properties(env, exports, 1, &get_object_meta);
  assert(status == napi_ok);

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)