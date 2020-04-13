#include <assert.h>
#include "uplink_definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <node_api.h>
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

// createObjectResult function creates NAPI type ObjectResult for returning it to javascript
napi_value createObjectResult(napi_env env,Object* lO_ObjectPtr,Error* lO_ErrorPtr){
  napi_value returnObject,objectNAPI,errorObject,SystemMetadataNAPI,CustomMetadataNAPI,CustomMetadataEntryNAPI;
  napi_status status;

  status = napi_create_object(env,&returnObject);
  assert(status == napi_ok);

  if(lO_ObjectPtr!=NULL){

    Object lO_Object = *(lO_ObjectPtr);

    status = napi_create_object(env,&objectNAPI);
    assert(status == napi_ok);
  
    status = napi_create_object(env,&SystemMetadataNAPI);
    assert(status == napi_ok);

    status = napi_create_object(env,&CustomMetadataNAPI);
    assert(status == napi_ok);
  
    status = napi_create_object(env,&CustomMetadataEntryNAPI);
    assert(status == napi_ok);
    
    napi_value keyObjectNAPI;
    status = napi_create_string_utf8(env,lO_ObjectPtr->key,NAPI_AUTO_LENGTH,&keyObjectNAPI);
    assert(status == napi_ok);
    
    napi_value is_prefixNAPI;
    if(lO_ObjectPtr->is_prefix){
      status = napi_create_int64(env, 1, &is_prefixNAPI);
      assert(status == napi_ok);
    }else{
      status = napi_create_int64(env, 0, &is_prefixNAPI);
      assert(status == napi_ok);
    }

    //
    SystemMetadata lO_SystemMetadata;
    CustomMetadata lO_CustomMetadata;
    CustomMetadataEntry lO_CustomMetadataEntry;
    lO_SystemMetadata = lO_Object.system;
    lO_CustomMetadata = lO_Object.custom;
    napi_value entriesArray;
    status = napi_create_object(env,&entriesArray);
    assert(status == napi_ok);
    char empty[]="";
    if(lO_CustomMetadata.count>0){
      CustomMetadataEntry* CustomMetadataEntryListPointer = lO_CustomMetadata.entries;
      for(int j=0;j<lO_CustomMetadata.count;j++){
        lO_CustomMetadataEntry = *(CustomMetadataEntryListPointer+j);
        napi_value key_lengthNAPI,value_lengthNAPI,keyNAPI,valueNAPI,customMetaNAPI;
        status = napi_create_object(env,&customMetaNAPI);
        assert(status == napi_ok);

        status = napi_create_int64(env, lO_CustomMetadataEntry.key_length, &key_lengthNAPI);
        assert(status == napi_ok);
    
        status = napi_create_int64(env, lO_CustomMetadataEntry.value_length, &value_lengthNAPI);
        assert(status == napi_ok);

        status = napi_create_string_utf8(env,lO_CustomMetadataEntry.key,NAPI_AUTO_LENGTH,&keyNAPI);
        assert(status == napi_ok);
        
        status = napi_create_string_utf8(env,lO_CustomMetadataEntry.value,NAPI_AUTO_LENGTH,&valueNAPI);
        assert(status == napi_ok);
        
        status = napi_set_named_property(env,customMetaNAPI,"key",keyNAPI);
        assert(status == napi_ok);

        status = napi_set_named_property(env,customMetaNAPI,"key_length",key_lengthNAPI);
        assert(status == napi_ok);

        status = napi_set_named_property(env,customMetaNAPI,"value",valueNAPI);
        assert(status == napi_ok);

        status = napi_set_named_property(env,customMetaNAPI,"value_length",value_lengthNAPI);
        assert(status == napi_ok);

        status = napi_set_named_property(env,customMetaNAPI,"value_length",value_lengthNAPI);
        assert(status == napi_ok);

        char str[100];
        itoa(j, str, 10);
        status = napi_set_named_property(env,entriesArray,str,customMetaNAPI);
        assert(status == napi_ok);
      }
    }else{
      napi_value emptyNAPI;
      status = napi_create_string_utf8(env,&empty[0],NAPI_AUTO_LENGTH,&emptyNAPI);
      assert(status == napi_ok);

      status = napi_set_named_property(env,entriesArray,"0",emptyNAPI);
      assert(status == napi_ok);
    }
    
    
    napi_value createdNAPI;
    status = napi_create_int64(env, lO_ObjectPtr->system.created, &createdNAPI);
    assert(status == napi_ok);
    
    napi_value expiresNAPI;
    status = napi_create_int64(env, lO_ObjectPtr->system.expires, &expiresNAPI);
    assert(status == napi_ok);

    napi_value countNAPI;
    status = napi_create_int64(env, lO_CustomMetadata.count, &countNAPI);
    assert(status == napi_ok);

    napi_value content_lengthNAPI;
    status = napi_create_int64(env, lO_SystemMetadata.content_length, &content_lengthNAPI);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,CustomMetadataNAPI,"entries",entriesArray);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,CustomMetadataNAPI,"count",countNAPI);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,objectNAPI,"custom",CustomMetadataNAPI);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,SystemMetadataNAPI,"created",createdNAPI);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,SystemMetadataNAPI,"expires",expiresNAPI);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,SystemMetadataNAPI,"content_length",content_lengthNAPI);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,objectNAPI,"system",SystemMetadataNAPI);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,objectNAPI,"is_prefix",is_prefixNAPI);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,objectNAPI,"key",keyObjectNAPI);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,returnObject,"object",objectNAPI);
    assert(status == napi_ok);
    
    napi_value nullvalue;
    status = napi_create_string_utf8(env,&empty[0],NAPI_AUTO_LENGTH,&nullvalue);
    assert(status == napi_ok);
    
    status = napi_create_object(env,&errorObject);
    assert(status == napi_ok);

    napi_value errorCode;
    status = napi_create_int64(env, 0, &errorCode);
    assert(status == napi_ok);

    status = napi_set_named_property(env,errorObject,"code",errorCode);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,errorObject,"message",nullvalue);
    assert(status == napi_ok);

    status = napi_set_named_property(env,returnObject,"error",errorObject);
    assert(status == napi_ok);

  }else if(lO_ErrorPtr!=NULL){
    
    Error lO_Error = *(lO_ErrorPtr);
    
    status = napi_create_object(env,&errorObject);
    assert(status == napi_ok);
    
    napi_value errorCode;
    status = napi_create_int64(env, lO_Error.code, &errorCode);
    assert(status == napi_ok);
    
    napi_value errorMessage;
    status = napi_create_string_utf8(env,lO_Error.message,NAPI_AUTO_LENGTH,&errorMessage);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,errorObject,"code",errorCode);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,errorObject,"message",errorMessage);
    assert(status == napi_ok);
    
    char empty[]="";
    napi_value nullvalue;
    status = napi_create_string_utf8(env,&empty[0],NAPI_AUTO_LENGTH,&nullvalue);
    assert(status == napi_ok);

    status = napi_set_named_property(env,returnObject,"object",nullvalue);
    assert(status == napi_ok);
    
    status = napi_set_named_property(env,returnObject,"error",errorObject);
    assert(status == napi_ok);
  }
  return returnObject;
}

//function create's NAPI type result for accessResult , ProjectResult
napi_value createResult(napi_env env,string resultType,long handlevalue,int64_t accessError,char* errorMessagePtr){
  napi_status status;
  napi_value ObjectRef,returnObject,errorObject; 

  //Creating NAPI Object's
  status = napi_create_object(env,&ObjectRef);
  assert(status == napi_ok);
  
  status = napi_create_object(env,&errorObject);
  assert(status == napi_ok);

  status = napi_create_object(env,&returnObject);
  assert(status == napi_ok);

  //Creating int64 NAPI type
  napi_value _handle,errorCode,errorMessage;
  
  status = napi_create_int64(env, handlevalue, &_handle);
  assert(status == napi_ok);
  
  status = napi_set_named_property(env,ObjectRef,"_handle",_handle);
  assert(status == napi_ok);
  
  status = napi_create_object(env,&errorObject);
  assert(status == napi_ok);
  
  status = napi_create_int64(env, accessError, &errorCode);
  assert(status == napi_ok);
  
  status = napi_create_string_utf8(env,errorMessagePtr,NAPI_AUTO_LENGTH,&errorMessage);
  assert(status == napi_ok);
  
  status = napi_set_named_property(env,errorObject,"code",errorCode);
  assert(status == napi_ok);
  
  status = napi_set_named_property(env,errorObject,"message",errorMessage);
  assert(status == napi_ok);
  
  status = napi_set_named_property(env,returnObject,"error",errorObject);
  assert(status == napi_ok);
  
  const char* resultTypeChar = resultType.c_str();
  
  status = napi_set_named_property(env,returnObject,&resultTypeChar[0],ObjectRef);
  assert(status == napi_ok);

  return returnObject;
}

//function creates BucketResults
napi_value createBucketResult(napi_env env,string resultType,int64_t bucketCreated,char* bucketNamePtr,int64_t bucketErrorCode,char* errorMessagePtr){
  napi_status status;
  napi_value ObjectRef,returnObject,errorObject; 
  
  //Creating NAPI Object
  status = napi_create_object(env,&ObjectRef);
  assert(status == napi_ok);
  
  status = napi_create_object(env,&errorObject);
  assert(status == napi_ok);

  status = napi_create_object(env,&returnObject);
  assert(status == napi_ok);
  
  napi_value created,errorCode,errorMessage,bucketName;
  status = napi_create_int64(env, bucketCreated, &created);
  assert(status == napi_ok);
  status = napi_set_named_property(env,ObjectRef,"created",created);
  assert(status == napi_ok);
  
  status = napi_create_string_utf8(env,bucketNamePtr,NAPI_AUTO_LENGTH,&bucketName);
  assert(status == napi_ok);
  
  status = napi_set_named_property(env,ObjectRef,"name",bucketName);
  assert(status == napi_ok);
  
  status = napi_create_object(env,&errorObject);
  assert(status == napi_ok);
  
  status = napi_create_int64(env, bucketErrorCode, &errorCode);
  assert(status == napi_ok);
  
  status = napi_create_string_utf8(env,errorMessagePtr,NAPI_AUTO_LENGTH,&errorMessage);
  assert(status == napi_ok);
  
  status = napi_set_named_property(env,errorObject,"code",errorCode);
  assert(status == napi_ok);
  
  status = napi_set_named_property(env,errorObject,"message",errorMessage);
  assert(status == napi_ok);
  
  status = napi_set_named_property(env,returnObject,"error",errorObject);
  assert(status == napi_ok);
  
  const char* resultTypeChar = resultType.c_str();
  status = napi_set_named_property(env,returnObject,&resultTypeChar[0],ObjectRef);
  assert(status == napi_ok);
  
  return returnObject;
}

//function creates NAPI type error object
napi_value createError(napi_env env,int64_t accessError,char* errorMessagePtr){
  napi_status status;
  napi_value returnObject,errorObject,errorCode,errorMessage;
  
  status = napi_create_object(env,&errorObject);
  assert(status == napi_ok);
  
  status = napi_create_object(env,&returnObject);
  assert(status == napi_ok);
  
  status = napi_create_int64(env, accessError, &errorCode);
  assert(status == napi_ok);

  status = napi_create_string_utf8(env,errorMessagePtr,NAPI_AUTO_LENGTH,&errorMessage);
  assert(status == napi_ok);
  
  status = napi_set_named_property(env,errorObject,"code",errorCode);
  assert(status == napi_ok);
  
  status = napi_set_named_property(env,errorObject,"message",errorMessage);
  assert(status == napi_ok);
  
  status = napi_set_named_property(env,returnObject,"error",errorObject);
  assert(status == napi_ok);
  return returnObject;
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
  
  status = napi_typeof(env,handlevalue,&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_number){
    napi_throw_type_error(env,nullptr,"\nWrong Handle type\n");
    return 0;
  }
  int64_t handle = 0;
  status = napi_get_value_int64(env,handlevalue,&handle);
  assert(status == napi_ok);

  long handlelong = (long)handle;
  return handlelong;
}

HINSTANCE hGetProcIDDLL;
void loaddll(){
  hGetProcIDDLL = LoadLibrary("libuplinkc.dll");
}


//  * function parse_accessc to parses serialized access grant string
//  * pre-requisites: access_sharec
//  * inputs: accessString (String)
//  * output: AccessResult (Object)
napi_value parse_accessc(napi_env env, napi_callback_info info){
  napi_status status;

  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);

  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 1 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! argument excepted to be string type\n");
    return NULL;
  }

  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[0],NULL,bufsize,&convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;

  char* accessString =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,args[0],accessString,convertedvalue,&bufsize);
  assert(status == napi_ok);

  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found\n");
    return NULL;
  }else{
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"parse_access");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found \n");
      return NULL;
    }else{
      typedef AccessResult(__stdcall * pICAccessResult)(char *);
      pICAccessResult parse_access;
      parse_access = pICAccessResult(fn);
      AccessResult lO_AccessResult = parse_access(accessString);
      if(lO_AccessResult.access==NULL){
        if(lO_AccessResult.error!=NULL){
            Error lO_errorObject = *(lO_AccessResult.error);
            char* errorMessagePtr = lO_errorObject.message;
            return createResult(env,"access",0,lO_errorObject.code,&errorMessagePtr[0]);
          }else{
            char blank[] = "";
            return createResult(env,"access",0,0,&blank[0]);
          }
        }
        Access lO_Access = *(lO_AccessResult.access);
        size_t handlevalue = lO_Access._handle;
        char blank[] = "";
        return createResult(env,"access",handlevalue,0,&blank[0]);
    }
  }
  return NULL;
}


  //  * function request_access_with_passphrasec requests satellite for a new access grant using a passhprase
  //  * pre-requisites: None
  //  * inputs: satellite address (String),API key (String) and passphrase (String)
  //  * output: AccessResult (Object)
napi_value request_access_with_passphrasec(napi_env env, napi_callback_info info){
  napi_status status;

  size_t argc = 3;
  napi_value args[3];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 3) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 3 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! First argument excepted to be string type\n");
    return NULL;
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Second argument excepted to be string type\n");
    return NULL;
  }

  status = napi_typeof(env,args[2],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Third argument excepted to be string type\n");
    return NULL;
  }

  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[0],NULL,bufsize,&convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;
  
  char* satellite_address =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,args[0],satellite_address,convertedvalue,&bufsize);
  assert(status == napi_ok);
  
  bufsize = 0;
  convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;
  
  char* api_key =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,args[1],api_key,convertedvalue,&bufsize);
  assert(status == napi_ok);

  bufsize = 0;
  convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[2],NULL,bufsize,&convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;
  
  char* passphrase =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,args[2],passphrase,convertedvalue,&bufsize);
  assert(status == napi_ok);

  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found\n");
    return NULL;
  }else{
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"request_access_with_passphrase");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found\n");
      return NULL;
    }else{
      typedef AccessResult(__stdcall * pICAccessResult)(char *,char *,char *);
      pICAccessResult request_access_with_passphrase;
      request_access_with_passphrase = pICAccessResult(fn);
      AccessResult lO_AccessResult = request_access_with_passphrase(satellite_address,api_key,passphrase);
      
      if(lO_AccessResult.access==NULL){
        
        if(lO_AccessResult.error!=NULL){
          Error lO_errorObject = *(lO_AccessResult.error);
          char* errorMessagePtr = lO_errorObject.message;
          return createResult(env,"access",0,lO_errorObject.code,&errorMessagePtr[0]);
        }else{
          char blank[] = "";
          return createResult(env,"access",0,-1,&blank[0]);
        }
      }
      Access lO_Access = *(lO_AccessResult.access);
      size_t handlevalue = lO_Access._handle;
      char blank[] = "";
      return createResult(env,"access",handlevalue,0,&blank[0]);
    }
  }
  return NULL;
}

  //  * function open_project opens project using access grant.
  //  * pre-requisites: request_access_with_passphrasec
  //  * inputs: Access (Object)
  //  * output: ProjectResult (Object)
napi_value open_projectc(napi_env env, napi_callback_info info){
  napi_status status;

  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);

  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 1 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }
  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }
  Access lO_Access;
  lO_Access._handle = getHandleValue(env,args[0]);
  if(lO_Access._handle==0){
    return NULL;
  }
  if(!hGetProcIDDLL){
    napi_throw_type_error(env, nullptr, "\nLibrary not found \n");
    return NULL;
  }else{
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"open_project");
    if(!fn){
      napi_throw_type_error(env, nullptr, "\nFunction not found \n");
      return NULL;
    }else{
      typedef ProjectResult(__stdcall * pICProjectResult)(Access *);
      pICProjectResult open_project;
      open_project = pICProjectResult(fn);
      ProjectResult lO_projectResult = open_project(&lO_Access);
      if(lO_projectResult.project==NULL){
        
        if(lO_projectResult.error!=NULL){
          Error lO_errorObject = *(lO_projectResult.error);
          char* errorMessagePtr = lO_errorObject.message;
          return createResult(env,"project",0,lO_errorObject.code,errorMessagePtr);
        }else{
          char blank[] = "";
          return createResult(env,"project",0,-1,&blank[0]);
        }
      }
      Project lO_Project = *(lO_projectResult.project);
      size_t handlevalue = lO_Project._handle;
      char blank[] = "";
      return createResult(env,"project",handlevalue,0,&blank[0]);
    }
  }
  return NULL;
}

  //  * function close_projectc closes the project.
  //  * pre-requisites: open_projectc
  //  * inputs: Project (Object)
  //  * output: Error (Object)
napi_value close_projectc(napi_env env, napi_callback_info info){
  napi_status status;

  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);

  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 1 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }

  Project lO_Project;
  lO_Project._handle = getHandleValue(env,args[0]);
  if(lO_Project._handle==0){
    return NULL;
  }
  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found\n");
    return NULL;
  }else{
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"close_project");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found\n");
      return NULL;
    }else{
      typedef Error*(__stdcall * pICError)(Project *);
      pICError close_project;
      close_project = pICError(fn);
      Error* lO_ErrorPtr = close_project(&lO_Project);
      if(lO_ErrorPtr!=NULL){
        Error lO_Error = *(lO_ErrorPtr);
        char* errorMessagePtr = lO_Error.message;
        return createError(env,lO_Error.code,&errorMessagePtr[0]);
      }
      char blank[] = "";
      return createError(env,0,&blank[0]);
    }
  }
  return NULL;

}
  //  * function stat_bucketc returns information about a bucket.
  //  * pre-requisites: open_projectc
  //  * inputs: Project (Object) ,Bucket Name (String)
  //  * output: BucketResult (Object)
napi_value stat_bucketc(napi_env env, napi_callback_info info){
  napi_status status;

  size_t argc = 2;
  napi_value args[2];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);

  if (argc < 2) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 2 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Second argument excepted to be string type\n");
    return NULL;
  }

  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;
  char* bucketName =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,args[1],bucketName,convertedvalue,&bufsize);
  assert(status == napi_ok);

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }

  Project lO_Project;
  lO_Project._handle = getHandleValue(env,args[0]);
  if(lO_Project._handle==0){
    return NULL;
  }
  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found\n");
    return NULL;
  }else{
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"stat_bucket");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found\n");
      return NULL;
    }else{
      typedef BucketResult(__stdcall * pICBucketResult)(Project *,char *);
      pICBucketResult stat_bucket;
      stat_bucket = pICBucketResult(fn);
      BucketResult lO_bucketResult = stat_bucket(&lO_Project, bucketName);

      if(lO_bucketResult.bucket==NULL){
        
        if(lO_bucketResult.error!=NULL){
          Error lO_errorObject = *(lO_bucketResult.error);
          char* errorMessagePtr = lO_errorObject.message;
          char blank[] = "";
          return createBucketResult(env,"bucket",0,&blank[0],lO_errorObject.code,&errorMessagePtr[0]);

        }else{
          char blank[] = "";
          return createBucketResult(env,"bucket",-1,&blank[0],-1,&blank[0]);
        }
      }
      Bucket lO_bucket = *(lO_bucketResult.bucket);
      char* bucketNamePtr = lO_bucket.name;
      int64_t bucketCreated = lO_bucket.created;
      char blank[] = "";
      return createBucketResult(env,"bucket",bucketCreated,bucketNamePtr,0,&blank[0]);
    }
  }
  return NULL;
}

  //  * function create_bucketc creates a new bucket.
  //  * pre-requisites: open_projectc
  //  * inputs: Project (Object) ,Bucket Name (String)
  //  * output: BucketResult (Object)
napi_value create_bucketc(napi_env env, napi_callback_info info){
  napi_status status;

  size_t argc = 2;
  napi_value args[2];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);

  if (argc < 2) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 2 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Second argument excepted to be string type\n");
    return NULL;
  }

  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;
  
  char* bucketName =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,args[1],bucketName,convertedvalue,&bufsize);
  assert(status == napi_ok);
  
  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }

  Project lO_Project;
  lO_Project._handle = getHandleValue(env,args[0]);
  if(lO_Project._handle==0){
    return NULL;
  }
  //
  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found\n");
    return NULL;
  }else{
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"create_bucket");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found\n");
      return NULL;
    }else{
      typedef BucketResult(__stdcall * pICBucketResult)(Project *,char *);
      pICBucketResult create_bucket;
      create_bucket = pICBucketResult(fn);
      BucketResult lO_bucketResult = create_bucket(&lO_Project, bucketName);
    
    if(lO_bucketResult.bucket==NULL){
      if(lO_bucketResult.error!=NULL){
        Error lO_errorObject = *(lO_bucketResult.error);
        char* errorMessagePtr = lO_errorObject.message;
        char blank[] = "";
        return createBucketResult(env,"bucket",0,&blank[0],lO_errorObject.code,&errorMessagePtr[0]);

      }else{
        char blank[] = "";
        return createBucketResult(env,"bucket",-1,&blank[0],-1,&blank[0]);
      }
   }

    Bucket lO_bucket = *(lO_bucketResult.bucket);
    char* bucketNamePtr = lO_bucket.name;
    int64_t bucketCreated = lO_bucket.created;
    char blank[] = "";
      return createBucketResult(env,"bucket",bucketCreated,bucketNamePtr,0,&blank[0]);
    }
  }
  return NULL;  
}

  //  * function ensure_bucketc creates a new bucket and ignores the error when it already exists
  //  * pre-requisites: open_projectc
  //  * inputs: Project (Object) ,Bucket Name (String)
  //  * output: BucketResult (Object)
napi_value ensure_bucketc(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 2;
  napi_value args[2];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);

  if (argc < 2) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 2 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Second argument excepted to be string type\n");
    return NULL;
  }

  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;
  char* bucketName =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,args[1],&bucketName[0],convertedvalue,&bufsize);
  assert(status == napi_ok);

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }

  Project lO_Project;
  lO_Project._handle = getHandleValue(env,args[0]);
  if(lO_Project._handle==0){
    return NULL;
  }
  
  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found\n");
    return NULL;
  }else{
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"ensure_bucket");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFound not found\n");
      return NULL;
    }else{
      typedef BucketResult(__stdcall * pICBucketResult)(Project *,char *);
      pICBucketResult ensure_bucket;
      ensure_bucket = pICBucketResult(fn);
      BucketResult lO_bucketResult = ensure_bucket(&lO_Project, bucketName);
      if(lO_bucketResult.bucket==NULL){
        
        if(lO_bucketResult.error!=NULL){
          Error lO_errorObject = *(lO_bucketResult.error);
          char* errorMessagePtr = lO_errorObject.message;
          char blank[] = "";
          return createBucketResult(env,"bucket",0,&blank[0],lO_errorObject.code,&errorMessagePtr[0]);
        }else{
          char blank[] = "";
          return createBucketResult(env,"bucket",-1,&blank[0],-1,&blank[0]);
        }
      }
      Bucket lO_bucket = *(lO_bucketResult.bucket);
      char* bucketNamePtr = lO_bucket.name;
      int64_t bucketCreated = lO_bucket.created;
      char blank[] = "";
      return createBucketResult(env,"bucket",bucketCreated,bucketNamePtr,0,&blank[0]); 
    }
  } 
}

  //  * function delete_bucketc deletes a bucket.
  //  * pre-requisites: open_projectc
  //  * inputs: Project (object) ,Bucket Name (string)
  //  * output: BucketResult (object)
napi_value delete_bucketc(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 2;
  napi_value args[2];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);

  if (argc < 2) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 2 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Second argument excepted to be string type\n");
    return NULL;
  }

  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;
  char* bucketName =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,args[1],&bucketName[0],convertedvalue,&bufsize);
  assert(status == napi_ok);
  
  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }

  Project lO_Project;
  lO_Project._handle = getHandleValue(env,args[0]);
  if(lO_Project._handle==0){
    return NULL;
  }
  
  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found\n");
    return NULL;
  }else{
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"delete_bucket");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found\n");
      return NULL;
    }else{
      typedef BucketResult(__stdcall * pICBucketResult)(Project *,char *);
      pICBucketResult delete_bucket;
      delete_bucket = pICBucketResult(fn);
      BucketResult lO_bucketResult = delete_bucket(&lO_Project, bucketName);
      if(lO_bucketResult.bucket==NULL){
        
        if(lO_bucketResult.error!=NULL){
          Error lO_errorObject = *(lO_bucketResult.error);
          char* errorMessagePtr = lO_errorObject.message;
          char blank[] = "";
          return createBucketResult(env,"bucket",0,&blank[0],lO_errorObject.code,&errorMessagePtr[0]);

        }else{
          char blank[] = "";
          return createBucketResult(env,"bucket",-1,&blank[0],-1,&blank[0]);
        }
      }
      Bucket lO_bucket = *(lO_bucketResult.bucket);
      char* bucketNamePtr = lO_bucket.name;
      int64_t bucketCreated = lO_bucket.created;
      char blank[] = "";
      return createBucketResult(env,"bucket",bucketCreated,bucketNamePtr,0,&blank[0]); 
    }
  }
  return NULL;
}

  //  * function list_bucketsc lists buckets
  //  * pre-requisites: open_projectc
  //  * inputs: Project (Object) ,ListBucketsOptions(Object)
  //  * output: BucketList (Object)
napi_value list_bucketsc(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 2;
  napi_value args[2],returnObject;

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);

  if (argc < 2) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 2 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if((checktypeofinput!=napi_object)&&(checktypeofinput!=napi_null)){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Second argument excepted to be object type or null\n");
    return NULL;
  }
  //
  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }
  //
  Project lO_Project;
  lO_Project._handle = getHandleValue(env,args[0]);
  if(lO_Project._handle==0){
    return NULL;
  }
  ListBucketsOptions lO_ListBucketsOptions;
  //
  BucketIterator *lO_BucketIterator= nullptr;

  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found\n");
    return NULL;
  }else{
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"list_buckets");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found\n");
      return NULL;
    }else{
      typedef BucketIterator *(__stdcall * pICBucketIterator)(Project *,ListBucketsOptions *);
      pICBucketIterator list_buckets;
      list_buckets = pICBucketIterator(fn);
      if(checktypeofinput==napi_null){
        lO_BucketIterator = list_buckets(&lO_Project,NULL);
      }else if(checktypeofinput==napi_object){
        napi_value cursorNAPI;
        status = napi_get_named_property(env,args[1],"cursor",&cursorNAPI);
        assert(status == napi_ok);
        size_t bufsize = 0;
        size_t convertedvalue = 0;
        status = napi_get_value_string_utf8(env, cursorNAPI,NULL,bufsize,&convertedvalue);
        assert(status == napi_ok);
        convertedvalue=convertedvalue+1;

        char* cursor =  new char[convertedvalue];
        status = napi_get_value_string_utf8(env,cursorNAPI,cursor,convertedvalue,&bufsize);
        assert(status == napi_ok);
        lO_ListBucketsOptions.cursor = cursor;
        lO_BucketIterator = list_buckets(&lO_Project,&lO_ListBucketsOptions);

      }

      typedef bool (*BucketIteratorNextFUNC)(BucketIterator *);
      BucketIteratorNextFUNC bucket_iterator_next = (BucketIteratorNextFUNC)GetProcAddress(hGetProcIDDLL,"bucket_iterator_next");
      if(!bucket_iterator_next){
          return NULL;
        }else{
          typedef Bucket * (*BucketIteratorItemFUNC)(BucketIterator *);
          BucketIteratorItemFUNC bucket_iterator_item = (BucketIteratorItemFUNC)GetProcAddress(hGetProcIDDLL,"bucket_iterator_item");
          if(!bucket_iterator_item){
            return NULL;
          }else{
            typedef Error * (*BucketIteratorErrorFUNC)(BucketIterator *);
            BucketIteratorErrorFUNC bucket_iterator_err = (BucketIteratorErrorFUNC)GetProcAddress(hGetProcIDDLL,"bucket_iterator_err");
            napi_value BucketList,errorObject;
            if(!bucket_iterator_err){
              return NULL;
            }else{
              status = napi_create_object(env,&BucketList);
              assert(status == napi_ok);

              status = napi_create_object(env,&errorObject);
              assert(status == napi_ok);

              status = napi_create_object(env,&returnObject);
              assert(status == napi_ok);

              int count = 0;
              while(bucket_iterator_next(lO_BucketIterator)){
                  Bucket *lO_bucket = bucket_iterator_item(lO_BucketIterator);

                  napi_value BucketInfoObj;
                  status = napi_create_object(env,&BucketInfoObj);
                  assert(status == napi_ok);

                  char* bucketNamePtr = lO_bucket->name;

                  napi_value bucketName;

                  status = napi_create_string_utf8(env,bucketNamePtr,NAPI_AUTO_LENGTH,&bucketName);
                  assert(status == napi_ok);

                  status = napi_set_named_property(env,BucketInfoObj,"name",bucketName);
                  assert(status == napi_ok);
                  
                  napi_value createdBucket;
                  int64_t bucketcreatedvalue = lO_bucket->created;

                  status = napi_create_int64(env, bucketcreatedvalue, &createdBucket);
                  assert(status == napi_ok);

                  status = napi_set_named_property(env,BucketInfoObj,"created",createdBucket);
                  assert(status == napi_ok);

                  char str[100];
                  itoa(count, str, 10);

                  status = napi_set_named_property(env,BucketList,str,BucketInfoObj);
                  assert(status == napi_ok);

                  count++;
              }

              Error *err = bucket_iterator_err(lO_BucketIterator);

              if(err==NULL){
                int32_t code = 0;
                char message[] = "";
                napi_value codenapi,errormessage;

                status = napi_create_int64(env, code, &codenapi);
                assert(status == napi_ok);

                status = napi_create_string_utf8(env,&message[0],NAPI_AUTO_LENGTH,&errormessage);
                assert(status == napi_ok);

                status = napi_set_named_property(env,errorObject,"error",codenapi);
                assert(status == napi_ok);

                status = napi_set_named_property(env,errorObject,"message",errormessage);
                assert(status == napi_ok);
              }else{
                
                Error lO_Error = *(err);
                char* messagePtr = lO_Error.message;
                napi_value codenapi,errormessage;
                
                status = napi_create_int64(env, lO_Error.code, &codenapi);
                assert(status == napi_ok);
              
                status = napi_create_string_utf8(env,&messagePtr[0],NAPI_AUTO_LENGTH,&errormessage);
                assert(status == napi_ok);

                status = napi_set_named_property(env,errorObject,"error",codenapi);
                assert(status == napi_ok);

                status = napi_set_named_property(env,errorObject,"message",errormessage);
                assert(status == napi_ok);
              }
              status = napi_set_named_property(env,returnObject,"error",errorObject);
              assert(status == napi_ok);
              //
              status = napi_set_named_property(env,returnObject,"bucketList",BucketList);
              assert(status == napi_ok);
              return returnObject;
            }
            
          }

        }
    }

  }
  return NULL;
}

  //  * function stat_objectc information about an object at the specific key.
  //  * pre-requisites: open_projectc
  //  * inputs: Project (Object) ,Bucket Name (String) , Object Key(String)
  //  * output: ObjectResult (Object)
napi_value stat_objectc(napi_env env, napi_callback_info info) {
  napi_status status;
  size_t argc = 3;
  napi_value args[3];
  //
  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  //
  if (argc < 3) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 2 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Second argument excepted to be string type\n");
    return NULL;
  }
  //
  status = napi_typeof(env,args[2],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Third argument excepted to be string type\n");
    return NULL;
  }

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }

  Project lO_Project;
  lO_Project._handle = getHandleValue(env,args[0]);
  if(lO_Project._handle==0){
    return NULL;
  }

  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;
  
  char* bucketName =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,args[1],bucketName,convertedvalue,&bufsize);
  assert(status == napi_ok);
  bufsize = 0;
  convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[2],NULL,bufsize,&convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;
  
  char* objectKey =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,args[2],objectKey,convertedvalue,&bufsize);
  assert(status == napi_ok);
  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found\n");
    return NULL;
  }else{
    
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"stat_object");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found\n");
    return NULL;
    }else{
      typedef ObjectResult(__stdcall * pICObjectResult)(Project *,char *,char *);
      pICObjectResult stat_object;
      stat_object = pICObjectResult(fn);

      ObjectResult lO_ObjectResult = stat_object(&lO_Project,bucketName,objectKey);
        if(lO_ObjectResult.error!=NULL){
          return createObjectResult(env,NULL,lO_ObjectResult.error);
        }else{
          return createObjectResult(env,lO_ObjectResult.object,NULL);
        }
    }
  }
  return NULL;
}

  //  * function upload_objectc starts an upload to the specified key.
  //  * pre-requisites: open_projectc
  //  * inputs: Project (Object) ,Bucket Name (String) , Object Key(String) , Uplaod Options(Object)
  //  * output: UploadResult  (Object)
napi_value upload_objectc(napi_env env, napi_callback_info info) {
  
  napi_status status;
  size_t argc = 4;
  napi_value args[4],returnObject;
  //
  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  //
  if (argc < 4) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 4 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Second argument excepted to be string type\n");
    return NULL;
  }

  status = napi_typeof(env,args[2],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Third argument excepted to be string type\n");
    return NULL;
  }
  
  status = napi_typeof(env,args[3],&checktypeofinput);
  assert(status == napi_ok);

  if((checktypeofinput!=napi_object)&&(checktypeofinput!=napi_null)){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Fourth argument excepted to be object type or null\n");
    return NULL;
  }
  
  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }

  Project lO_Project;
  lO_Project._handle = getHandleValue(env,args[0]);
  if(lO_Project._handle==0){
    return NULL;
  }
  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;
  
  char* bucketName =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,args[1],bucketName,convertedvalue,&bufsize);
  assert(status == napi_ok);
  bufsize = 0;
  convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[2],NULL,bufsize,&convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;
  
  char* objectKey =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,args[2],objectKey,convertedvalue,&bufsize);
  assert(status == napi_ok);

  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found\n");
    return NULL;
  }else{
    
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"upload_object");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found\n");
      return NULL;
    }else{
      typedef UploadResult(__stdcall * pICUploadResult)(Project *,char *,char *,UploadOptions *);
      pICUploadResult upload_object;
      upload_object = pICUploadResult(fn);
      UploadResult lO_UploadResult;
      if(checktypeofinput==napi_null){
        lO_UploadResult = upload_object(&lO_Project,&bucketName[0],&objectKey[0],NULL);
      }else{
        
        UploadOptions lO_UploadOptions;
        napi_value expiresNAPI;

        status = napi_get_named_property(env,args[3],"expires",&expiresNAPI);
        assert(status == napi_ok);

        int64_t expires;
        status = napi_get_value_int64(env,expiresNAPI,&expires);
        assert(status == napi_ok);
        lO_UploadOptions.expires = expires;

        lO_UploadResult = upload_object(&lO_Project,bucketName,objectKey,&lO_UploadOptions);
      }
      if(lO_UploadResult.upload==NULL){
        
        if(lO_UploadResult.error!=NULL){

          Error lO_errorObject = *(lO_UploadResult.error);
          char* errorMessagePtr = lO_errorObject.message;
          return createResult(env,"upload",0,lO_errorObject.code,&errorMessagePtr[0]);
        }else{
          char blank[] = "";
          return createResult(env,"upload",0,-1,&blank[0]);
        }
      }
      
      Upload lO_Upload = *(lO_UploadResult.upload);
      
      size_t handlevalue = lO_Upload._handle;
      
      char blank[] = "";
      return createResult(env,"upload",handlevalue,0,&blank[0]);
    
    }
  }
  return NULL;
}


  //  * function upload_writec uploads len(p) bytes from p to the object's data stream.
  //  * pre-requisites: upload_objectc
  //  * inputs: Upload (Object) ,Pointer(Buffer) , Length (size_t)
  //  * output: WriteResult (Object)
napi_value upload_writec(napi_env env, napi_callback_info info) {
  napi_status status;
  
  size_t argc = 3;
  napi_value args[3];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 3) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 3 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);
  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! first argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);
  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! second argument excepted to be object/buffer type\n");
    return NULL;
  }

  status = napi_typeof(env,args[2],&checktypeofinput);
  assert(status == napi_ok);
  if(checktypeofinput!=napi_number){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! third argument excepted to be number type\n");
    return NULL;
  }
  
  char *_err = "";
  char **err = &_err;
  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }

  Upload lO_UploadRef;
  lO_UploadRef._handle = getHandleValue(env,args[0]);
  if(lO_UploadRef._handle==0){
    return NULL;
  }
  bool buffertype1;
  status = napi_is_arraybuffer(env, args[1], &buffertype1);
  assert(status == napi_ok);
  size_t lengthOfBuffer;  

  void* bufferPtr = NULL;  

  status = napi_get_buffer_info(env, args[1], &bufferPtr,&lengthOfBuffer);
  assert(status == napi_ok);

  int64_t bytes64;
  status = napi_get_value_int64(env,args[2],&bytes64);
  assert(status == napi_ok);

  size_t bytesread = (size_t)bytes64;

  uint8_t *ptrToData ; 
  ptrToData = (uint8_t *)bufferPtr;

  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found\n");
    return NULL;
  }else{
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"upload_write");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found\n");
      return NULL;
    }else{
      typedef WriteResult(__stdcall * pICWriteResult)(Upload *,uint8_t *,size_t);
      pICWriteResult upload_write;
      upload_write = pICWriteResult(fn);

      WriteResult lO_WriteResult = upload_write(&lO_UploadRef,ptrToData,bytesread);
      napi_value returnObject,errorObject;
      status = napi_create_object(env,&returnObject);
      if(lO_WriteResult.error!=NULL){
          Error lO_errorObject = *(lO_WriteResult.error);
          char* errorMessagePtr = lO_errorObject.message;
          size_t datauploaded = 0;
          napi_value datauploadedObject;
          status = napi_create_object(env,&errorObject);
          assert(status == napi_ok);
          status = napi_create_int32(env,datauploaded,&datauploadedObject);
          assert(status == napi_ok);
          napi_value errorObject,errorCode,errorMessage;

          status = napi_set_named_property(env,returnObject,"bytes_written",datauploadedObject);
          assert(status == napi_ok);

          status = napi_create_int64(env, lO_errorObject.code, &errorCode);
          assert(status == napi_ok);

          status = napi_create_string_utf8(env,&errorMessagePtr[0],NAPI_AUTO_LENGTH,&errorMessage);
          assert(status == napi_ok);
          status = napi_set_named_property(env,errorObject,"code",errorCode);
          assert(status == napi_ok);
          
          status = napi_set_named_property(env,errorObject,"message",errorMessage);
          assert(status == napi_ok);

          status = napi_set_named_property(env,returnObject,"error",errorObject);
          assert(status == napi_ok);

          return returnObject;
      }else{
          char blank[] = "";
          napi_value errorObject,errorMessage;
          napi_value errorCode;
          status = napi_create_object(env,&errorObject);
          assert(status == napi_ok);

          size_t datauploaded = lO_WriteResult.bytes_written;
          napi_value datauploadedObject;
          status = napi_create_int32(env,datauploaded,&datauploadedObject);
          assert(status == napi_ok);
          //
          status = napi_set_named_property(env,returnObject,"bytes_written",datauploadedObject);
          assert(status == napi_ok);

          status = napi_create_int64(env, 0, &errorCode);
          assert(status == napi_ok);

          status = napi_create_string_utf8(env,&blank[0],NAPI_AUTO_LENGTH,&errorMessage);
          assert(status == napi_ok);
          status = napi_set_named_property(env,errorObject,"code",errorCode);
          assert(status == napi_ok);
          
          status = napi_set_named_property(env,errorObject,"message",errorMessage);
          assert(status == napi_ok);

          status = napi_set_named_property(env,returnObject,"error",errorObject);
          assert(status == napi_ok);
          return returnObject;
      }
      return NULL;
    }
  }
}

  //  * function download_objectc starts  download to the specified key.
  //  * pre-requisites: open_projectc
  //  * inputs: Project (Object), Buxcket Name(String), Object Key(String), Download Options(Object)
  //  * output: DownloadResult (Object)
napi_value download_objectc(napi_env env, napi_callback_info info) {
  
  napi_status status;
  size_t argc = 4;
  napi_value args[4];
  //
  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  //
  if (argc < 4) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 2 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Second argument excepted to be string type\n");
    return NULL;
  }
  //
  status = napi_typeof(env,args[2],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Third argument excepted to be string type\n");
    return NULL;
  }
  //
  status = napi_typeof(env,args[3],&checktypeofinput);
  assert(status == napi_ok);

  if((checktypeofinput!=napi_object)&&(checktypeofinput!=napi_null)){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Fourth argument excepted to be object type or null\n");
    return NULL;
  }
  
  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }

  Project lO_Project;
  lO_Project._handle = getHandleValue(env,args[0]);
  if(lO_Project._handle==0){
    return NULL;
  }
  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;
  
  char* bucketName =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,args[1],bucketName,convertedvalue,&bufsize);
  assert(status == napi_ok);
  bufsize = 0;
  convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[2],NULL,bufsize,&convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;
  
  char* objectKey =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,args[2],objectKey,convertedvalue,&bufsize);
  assert(status == napi_ok);
  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found\n");
    return NULL;
  }else{
    
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"download_object");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found\n");
    return NULL;
    }else{
      typedef DownloadResult(__stdcall * pICDownloadResult)(Project *,char *,char *,DownloadOptions *);
      pICDownloadResult download_object;
      download_object = pICDownloadResult(fn);
      DownloadResult lO_DownloadResult;
      if(checktypeofinput==napi_null){
        lO_DownloadResult = download_object(&lO_Project,bucketName,objectKey,NULL);
      }else{
        DownloadOptions lO_DownloadOption;
        napi_value offsetNAPI,lengthNAPI;

        status = napi_get_named_property(env,args[3],"offset",&offsetNAPI);
        assert(status == napi_ok);
        
        status = napi_get_named_property(env,args[3],"length",&lengthNAPI);
        assert(status == napi_ok);
        
        int64_t offset,length;
        status = napi_get_value_int64(env,offsetNAPI,&offset);
        assert(status == napi_ok);
        status = napi_get_value_int64(env,lengthNAPI,&length);
        assert(status == napi_ok);

        lO_DownloadOption.offset = offset;
        lO_DownloadOption.length = length;
        lO_DownloadResult = download_object(&lO_Project,&bucketName[0],&objectKey[0],&lO_DownloadOption);
      }
      
      napi_value returnObject;
      
      status = napi_create_object(env,&returnObject);
      
      if(lO_DownloadResult.download==NULL){
        if(lO_DownloadResult.error!=NULL){
          Error lO_errorObject = *(lO_DownloadResult.error);
          char* errorMessagePtr = lO_errorObject.message;
          return createResult(env,"upload",0,lO_errorObject.code,&errorMessagePtr[0]);
        }else{
          char blank[] = "";
          return createResult(env,"upload",0,-1,&blank[0]);
        }
      }
      Download lO_Download = *(lO_DownloadResult.download);
      size_t handlevalue = lO_Download._handle;
      char blank[] = "";
      return createResult(env,"download",handlevalue,0,&blank[0]);
    }
  }
  return NULL;
}

  //  * function upload_commitc commits the uploaded data.
  //  * pre-requisites: upload_objectc
  //  * inputs: Upload (Object)
  //  * output: Error (Object)
napi_value upload_commitc(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);

  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 1 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }

  Upload lO_Upload;
  lO_Upload._handle = getHandleValue(env,args[0]);
  if(lO_Upload._handle==0){
    return NULL;
  }
   if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found \n");
    return NULL;
  }else{
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"upload_commit");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found\n");
      return NULL;
    }else{
      typedef Error *(__stdcall * pICError)(Upload *);
      pICError upload_commit;
      upload_commit = pICError(fn);
      Error* lO_ErrorPtr = upload_commit(&lO_Upload);
      if(lO_ErrorPtr!=NULL){
        Error lO_Error = *(lO_ErrorPtr);
        char* errorMessagePtr = lO_Error.message;
        return createError(env,lO_Error.code,&errorMessagePtr[0]);
      }
      char blank[] = "";
      return createError(env,0,&blank[0]);
    }
  }
  return NULL;
}

//  * function close_downloadc closes the download.
//  * pre-requisites: download_objectc
//  * inputs: Download (Object)
//  * output: Error (Object)
napi_value close_downloadc(napi_env env, napi_callback_info info) {
  
  napi_status status;
  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);

  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 1 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }
  
  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }

  Download lO_Download;
  lO_Download._handle = getHandleValue(env,args[0]);
  //
  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found \n");
    return NULL;
  }else{
    
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"close_download");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\n Function not found \n");
    return NULL;
    }else{
      typedef Error *(__stdcall * pICError)(Download *);
      pICError close_download;
      close_download = pICError(fn);
      Error* lO_ErrorPtr = close_download(&lO_Download);
  //
      if(lO_ErrorPtr!=NULL){
        
        Error lO_Error = *(lO_ErrorPtr);
        char* errorMessagePtr = lO_Error.message;
        return createError(env,lO_Error.code,&errorMessagePtr[0]);
      }

      char blank[] = "";
      return createError(env,0,&blank[0]);
    }
  }
  //
  
}

// * function download_readc downloads from object's data stream into bytes up to length amount.
// * pre-requisites: download_objectc
// * inputs: Download (Object), Pointer (Buffer), Length(size_t) 
// * output: ReadResult  (Object)
napi_value download_readc(napi_env env, napi_callback_info info) {
  napi_status status;
  //
  size_t argc = 3;
  napi_value args[3];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  //
  if (argc < 3) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 3 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);
  //
  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! first argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);
  //
  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! second argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env,args[2],&checktypeofinput);
  assert(status == napi_ok);
  //
  if(checktypeofinput!=napi_number){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! third argument excepted to be number type\n");
    return NULL;
  }

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }

  Download lO_DownloaderRef;
  lO_DownloaderRef._handle = getHandleValue(env,args[0]);
  
  void* bufferPtr = NULL;  
  size_t lengthOfBuffer;  
  status = napi_get_buffer_info(env, args[1], &bufferPtr,&lengthOfBuffer);
  assert(status == napi_ok);
  //
  uint8_t *ptrToData ; 
  ptrToData = (uint8_t *)bufferPtr;
  //
  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found \n");
    return NULL;
  }else{
    
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"download_read");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found \n");
      return NULL;
    }else{
      typedef ReadResult(__stdcall * pICReadResult)(Download *,uint8_t *,size_t);
      pICReadResult download_read;
      download_read = pICReadResult(fn);
      //
        ReadResult lO_ReadResult = download_read(&lO_DownloaderRef,ptrToData,lengthOfBuffer);
        //
        napi_value returnObject;
        //
        status = napi_create_object(env,&returnObject);
        //
        if(lO_ReadResult.error!=NULL){
          
            napi_value errorObject,errorCode,errorMessage;
            Error lO_errorObject = *(lO_ReadResult.error);
            char* errorMessagePtr = lO_errorObject.message;
            
            size_t datadownload = 0;
            napi_value dataDownloadObject;
            status = napi_create_object(env,&errorObject);
            assert(status == napi_ok);
            
            status = napi_create_int32(env,datadownload,&dataDownloadObject);
            assert(status == napi_ok);
            
            status = napi_set_named_property(env,returnObject,"bytes_read",dataDownloadObject);
            assert(status == napi_ok);
            
            lO_errorObject.code = 0;
            
            status = napi_create_int64(env, lO_errorObject.code, &errorCode);
            assert(status == napi_ok);
            
            if(errorMessagePtr!=NULL){
              status = napi_create_string_utf8(env,errorMessagePtr,NAPI_AUTO_LENGTH,&errorMessage);
              assert(status == napi_ok);
            
            }else{
              char blank[] = "";

              status = napi_create_string_utf8(env,&blank[0],NAPI_AUTO_LENGTH,&errorMessage);
              assert(status == napi_ok);
            }
            status = napi_set_named_property(env,errorObject,"code",errorCode);
            assert(status == napi_ok);
            
            status = napi_set_named_property(env,errorObject,"message",errorMessage);
            assert(status == napi_ok);
            
            status = napi_set_named_property(env,returnObject,"error",errorObject);
            assert(status == napi_ok);

            return returnObject;
        }else{
            
            char blank[] = "";
            napi_value errorObject,errorMessage;
            napi_value errorCode;
            status = napi_create_object(env,&errorObject);
            assert(status == napi_ok);
            
            size_t datauploaded = lO_ReadResult.bytes_read;
            napi_value dataDownloadObject;
            status = napi_create_int32(env,datauploaded,&dataDownloadObject);
            assert(status == napi_ok);
            
            status = napi_set_named_property(env,returnObject,"bytes_read",dataDownloadObject);
            assert(status == napi_ok);

            status = napi_create_int64(env, 0, &errorCode);
            assert(status == napi_ok);
            
            status = napi_create_string_utf8(env,&blank[0],NAPI_AUTO_LENGTH,&errorMessage);
            assert(status == napi_ok);
            
            status = napi_set_named_property(env,errorObject,"code",errorCode);
            assert(status == napi_ok);
            
            status = napi_set_named_property(env,errorObject,"message",errorMessage);
            assert(status == napi_ok);

            status = napi_set_named_property(env,returnObject,"error",errorObject);
            assert(status == napi_ok);
            return returnObject;
        }
    }
  }
  return NULL;
  
}

//  * function delete_objectc deletes an object.
//  * pre-requisites: open_projectc
//  * inputs: Project (Object), Bucket Name (String), Object Key (String) 
//  * output: ObjectResult  (Object)
napi_value delete_objectc(napi_env env, napi_callback_info info) {
  napi_status status;
  size_t argc = 3;
  napi_value args[3];
  //
  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  //
  if (argc < 3) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 3 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Second argument excepted to be string type\n");
    return NULL;
  }
  //
  status = napi_typeof(env,args[2],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Third argument excepted to be string type\n");
    return NULL;
  }

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }

  Project lO_Project;
  lO_Project._handle = getHandleValue(env,args[0]);
  if(lO_Project._handle==0){
    return NULL;
  }
  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;

  char* bucketName =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,args[1],bucketName,convertedvalue,&bufsize);
  assert(status == napi_ok);
  bufsize = 0;
  convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[2],NULL,bufsize,&convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;

  char* objectKey =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,args[2],objectKey,convertedvalue,&bufsize);
  assert(status == napi_ok);
  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found \n");
    return NULL;
  }else{

    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"delete_object");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found \n");
      return NULL;
    }else{
      typedef ObjectResult(__stdcall * pICObjectResult)(Project *,char *,char *);
      pICObjectResult delete_object;
      delete_object = pICObjectResult(fn);
      ObjectResult lO_ObjectResult = delete_object(&lO_Project,bucketName,objectKey);
      if(lO_ObjectResult.error!=NULL){
        return createObjectResult(env,NULL,lO_ObjectResult.error);
      }else{
        return createObjectResult(env,lO_ObjectResult.object,NULL);
      }
    }
  }
  return NULL;
  //
}

//  * function access_sharec creates new access grant with specific permission. Permission will be applied to prefixes when defined.
//  * pre-requisites: parse_accessc
//  * inputs: Access (Object), Permission (Object), Share Prefix (Object), prefix count (int)
//  * output: String Result (Object)
napi_value access_sharec(napi_env env, napi_callback_info info) {
  napi_status status;
  size_t argc = 4;
  napi_value args[4];
  //
  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  //
  if (argc < 4) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 4 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput1,checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput1);
  assert(status == napi_ok);

  if((checktypeofinput1!=napi_object)&&(checktypeofinput1!=napi_null)){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }
  
  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Second argument excepted to be string type\n");
    return NULL;
  }
  //
  status = napi_typeof(env,args[2],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Third argument excepted to be string type\n");
    return NULL;
  }
  
  status = napi_typeof(env,args[3],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_number){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Fourth argument excepted to be string type\n");
    return NULL;
  }

  Access lO_Access;
  if(checktypeofinput1!=napi_null){
    bool propertyexists = false;
    napi_value ObjectkeyNAPI;
    char* handle = "_handle";
    status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
    assert(status == napi_ok);

    status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
    assert(status == napi_ok);
    if(!propertyexists){
      napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
      return NULL;
    }
    lO_Access._handle = getHandleValue(env,args[0]);
    if(lO_Access._handle==0){
      napi_throw_type_error(env,nullptr,"\nInvalid Handle\n");
      return NULL;
    }
  }
  
  Permission lO_Permission;
  
  napi_value allow_downloadNAPI;
  status = napi_get_named_property(env,args[1],"allow_download",&allow_downloadNAPI);
  assert(status == napi_ok);
  
  bool allow_downloadc;
  status = napi_get_value_bool(env,allow_downloadNAPI,&allow_downloadc);
  assert(status == napi_ok);
  
  lO_Permission.allow_download = allow_downloadc;

  napi_value allow_uploadNAPI;
  status = napi_get_named_property(env,args[1],"allow_upload",&allow_uploadNAPI);
  assert(status == napi_ok);
  
  bool allow_uploadc;
  status = napi_get_value_bool(env,allow_uploadNAPI,&allow_uploadc);
  assert(status == napi_ok);
  
  lO_Permission.allow_upload = allow_uploadc;
  
  napi_value allow_listNAPI;
  status = napi_get_named_property(env,args[1],"allow_list",&allow_listNAPI);
  assert(status == napi_ok);
  //
  bool allow_listc;
  status = napi_get_value_bool(env,allow_listNAPI,&allow_listc);
  assert(status == napi_ok);
  
  lO_Permission.allow_list = allow_listc;
  
  napi_value allow_deleteNAPI;
  status = napi_get_named_property(env,args[1],"allow_delete",&allow_deleteNAPI);
  assert(status == napi_ok);
  //
  bool allow_deletec;
  status = napi_get_value_bool(env,allow_deleteNAPI,&allow_deletec);
  assert(status == napi_ok);
  //
  lO_Permission.allow_delete = allow_deletec;

  napi_value not_beforeNAPI;
  status = napi_get_named_property(env,args[1],"not_before",&not_beforeNAPI);
  assert(status == napi_ok);

  int64_t not_beforec;
  status = napi_get_value_int64(env,not_beforeNAPI,&not_beforec);
  assert(status == napi_ok);
  
  lO_Permission.not_before = not_beforec;
  //
  napi_value not_afterNAPI;
  status = napi_get_named_property(env,args[1],"not_after",&not_afterNAPI);
  assert(status == napi_ok);

  int64_t not_afterc;
  status = napi_get_value_int64(env,not_afterNAPI,&not_afterc);
  assert(status == napi_ok);
  
  lO_Permission.not_after = not_afterc;
  //
  int64_t sharePrefixSize;
  status = napi_get_value_int64(env,args[3],&sharePrefixSize);
  assert(status == napi_ok);

  //Checking whether is array of not
  bool isarray=false;
  status = napi_is_array(env,args[2],&isarray);
  assert(status == napi_ok);
  if(!isarray){
    napi_throw_type_error(env,nullptr,"\nWrong data type of 3 parameter \n");
    return NULL;
  }
  
  uint32_t sizeOfArray = 0;
  status = napi_get_array_length(env,args[2],&sizeOfArray);
  assert(status == napi_ok);
  
  int sizeOfArrayInt = (int)sizeOfArray;
  int size; 
  if(sizeOfArray==sharePrefixSize){
    size = (int)sharePrefixSize;
  }else{
    size = sizeOfArrayInt;
  }
  SharePrefix* SharePrefixListPointer = new SharePrefix[size];
  
  napi_value SharePrefixObject;
  
  for(uint32_t i=0;i<(uint32_t)sizeOfArrayInt;i++){
    
    SharePrefix lO_SharePrefix;
    status = napi_get_element(env,args[2],i,&SharePrefixObject);
    assert(status == napi_ok);

    napi_value bucket;
    status = napi_get_named_property(env,SharePrefixObject,"bucket",&bucket);
    assert(status == napi_ok);
    //
    size_t bufsize = 0;
    size_t convertedvalue = 0;
    status = napi_get_value_string_utf8(env, bucket,NULL,bufsize,&convertedvalue);
    assert(status == napi_ok);
    convertedvalue = convertedvalue+1;

    char* bucketc =  new char[convertedvalue];
    status = napi_get_value_string_utf8(env,bucket,bucketc,convertedvalue,&bufsize);
    assert(status == napi_ok);
    napi_value prefix;
    status = napi_get_named_property(env,SharePrefixObject,"prefix",&prefix);
    assert(status == napi_ok);
    //
    bufsize = 0;
    convertedvalue = 0;
    status = napi_get_value_string_utf8(env, prefix,NULL,bufsize,&convertedvalue);
    assert(status == napi_ok);
    convertedvalue = convertedvalue+1;

    char* prefixc =  new char[convertedvalue];
    status = napi_get_value_string_utf8(env,prefix,prefixc,convertedvalue,&bufsize);
    assert(status == napi_ok);
    lO_SharePrefix.bucket = bucketc;
    lO_SharePrefix.prefix = prefixc;
    *(SharePrefixListPointer+i)=lO_SharePrefix;
  }

  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found \n");
    return NULL;
  }else{
    
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"access_share");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found \n");
    return NULL;
    }else{
      typedef AccessResult(__stdcall * pICAccessResult)(Access *,Permission,SharePrefix *,int64_t);
      pICAccessResult access_share;
      access_share = pICAccessResult(fn);

      AccessResult lO_AccessResult = access_share(&lO_Access,lO_Permission,SharePrefixListPointer,sharePrefixSize);

      if(lO_AccessResult.access==NULL){
        
        if(lO_AccessResult.error!=NULL){
          Error lO_errorObject = *(lO_AccessResult.error);
          char* errorMessagePtr = lO_errorObject.message;
          return createResult(env,"access",0,lO_errorObject.code,&errorMessagePtr[0]);
        }else{
          char blank[] = "";
          return createResult(env,"access",0,-1,&blank[0]);
        }
      }
      lO_Access = *(lO_AccessResult.access);
      size_t handlevalue = lO_Access._handle;
      
      char blank[] = "";
      return createResult(env,"access",handlevalue,0,&blank[0]);
    }
  }
  return NULL;
}




//  * function list_objectsc lists objects
//  * pre-requisites: open_projectc
//  * inputs: Project (Object), Bucket Name(String), ListObjectsOptions(Object) 
//  * output: ObjectList (Object)
napi_value list_objectsc(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 3;
  napi_value args[3],returnObject;

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);

  if (argc < 3) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 2 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_string){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Second argument excepted to be string\n");
    return NULL;
  }
  //
  status = napi_typeof(env,args[2],&checktypeofinput);
  assert(status == napi_ok);

  if((checktypeofinput!=napi_object)&&(checktypeofinput!=napi_null)){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! Second argument excepted to be object or NULL\n");
    return NULL;
  }
  //
  size_t bufsize = 0;
  size_t convertedvalue = 0;
  status = napi_get_value_string_utf8(env, args[1],NULL,bufsize,&convertedvalue);
  assert(status == napi_ok);
  convertedvalue = convertedvalue+1;
  
  char* bucketName =  new char[convertedvalue];
  status = napi_get_value_string_utf8(env,args[1],bucketName,convertedvalue,&bufsize);
  assert(status == napi_ok);
  
  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }

  Project lO_Project;
  lO_Project._handle = getHandleValue(env,args[0]);
  if(lO_Project._handle==0){
    return NULL;
  }
  //
  ListObjectsOptions lO_ListObjectsOptions;
  //
  ObjectIterator *lO_ObjectIterator= nullptr;
  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found \n");
    return NULL;
  }else{
    
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"list_objects");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found \n");
      return NULL;
    }else{
      typedef ObjectIterator *(__stdcall * pICObjectIterator)(Project *,char *,ListObjectsOptions *);
      pICObjectIterator list_objects;
      list_objects = pICObjectIterator(fn);
      if(checktypeofinput==napi_null){
        lO_ObjectIterator = list_objects(&lO_Project,&bucketName[0],NULL);

      }else if(checktypeofinput==napi_object){
        //
        napi_value cursorNAPI;
        status = napi_get_named_property(env,args[2],"cursor",&cursorNAPI);
        assert(status == napi_ok);
        //
        napi_value prefixNAPI;
        status = napi_get_named_property(env,args[2],"prefix",&prefixNAPI);
        assert(status == napi_ok);
        //
        size_t bufsize = 0;
        size_t convertedvalue = 0;
        status = napi_get_value_string_utf8(env, cursorNAPI,NULL,bufsize,&convertedvalue);
        assert(status == napi_ok);
        convertedvalue=convertedvalue+1;
        //char cursor[convertedvalue];
        char* cursor =  new char[convertedvalue];
        status = napi_get_value_string_utf8(env,cursorNAPI,cursor,convertedvalue,&bufsize);
        assert(status == napi_ok);
        lO_ListObjectsOptions.cursor = cursor;

        bufsize = 0;
        convertedvalue = 0;
        //
        status = napi_get_value_string_utf8(env, prefixNAPI,NULL,bufsize,&convertedvalue);
        assert(status == napi_ok);
        convertedvalue=convertedvalue+1;
        
        char* prefix =  new char[convertedvalue];
        status = napi_get_value_string_utf8(env,prefixNAPI,prefix,convertedvalue,&bufsize);
        assert(status == napi_ok);
        lO_ListObjectsOptions.prefix = prefix;
        
        napi_value recursiveNAPI;
        status = napi_get_named_property(env,args[2],"recursive",&recursiveNAPI);
        assert(status == napi_ok);
        //
        napi_value systemNAPI;
        status = napi_get_named_property(env,args[2],"system",&systemNAPI);
        assert(status == napi_ok);
        //
        napi_value customNAPI;
        status = napi_get_named_property(env,args[2],"custom",&customNAPI);
        assert(status == napi_ok);
        //
        bool recursive;
        napi_get_value_bool(env,recursiveNAPI,&recursive);
        assert(status == napi_ok);
        //
        bool system;
        napi_get_value_bool(env,systemNAPI,&system);
        assert(status == napi_ok);
        //
        bool custom;
        napi_get_value_bool(env,customNAPI,&custom);
        assert(status == napi_ok);
        //
        lO_ListObjectsOptions.custom = custom;
        lO_ListObjectsOptions.system = system;
        lO_ListObjectsOptions.recursive = recursive;
        
        lO_ObjectIterator = list_objects(&lO_Project,bucketName,&lO_ListObjectsOptions);
        
      }
      
      napi_value objectList,errorObject;
      status = napi_create_object(env,&objectList);
      assert(status == napi_ok);

      status = napi_create_object(env,&errorObject);
      assert(status == napi_ok);

      status = napi_create_object(env,&returnObject);
      assert(status == napi_ok);
      int count = 0;
    
      typedef bool (*ObjectIteratorNextFUNC)(ObjectIterator *);
      ObjectIteratorNextFUNC object_iterator_next = (ObjectIteratorNextFUNC)GetProcAddress(hGetProcIDDLL,"object_iterator_next");

      if(!object_iterator_next){
        return NULL;
      }else{
        typedef Object * (*ObjectIteratorItemFUNC)(ObjectIterator *);
        ObjectIteratorItemFUNC object_iterator_item = (ObjectIteratorItemFUNC)GetProcAddress(hGetProcIDDLL,"object_iterator_item");
        if(!object_iterator_item){
            return NULL;
        }else{
          typedef Error * (*ObjectIteratorErrorFUNC)(ObjectIterator *);
          ObjectIteratorErrorFUNC object_iterator_err = (ObjectIteratorErrorFUNC)GetProcAddress(hGetProcIDDLL,"object_iterator_err");
          
          if(!object_iterator_err){
              return NULL;
          }else{

              while(object_iterator_next(lO_ObjectIterator)){

                Object *lO_ObjectPtr = object_iterator_item(lO_ObjectIterator);

                Object lO_Object = *(lO_ObjectPtr);

                napi_value objectNAPI,SystemMetadataNAPI,CustomMetadataNAPI,CustomMetadataEntryNAPI;

                status = napi_create_object(env,&objectNAPI);
                assert(status == napi_ok);

                status = napi_create_object(env,&SystemMetadataNAPI);
                assert(status == napi_ok);

                status = napi_create_object(env,&CustomMetadataNAPI);
                assert(status == napi_ok);

                status = napi_create_object(env,&CustomMetadataEntryNAPI);
                assert(status == napi_ok);
                
                napi_value keyObjectNAPI;
                status = napi_create_string_utf8(env,lO_ObjectPtr->key,NAPI_AUTO_LENGTH,&keyObjectNAPI);
                assert(status == napi_ok);
                napi_value is_prefixNAPI;
                //
                if(lO_ObjectPtr->is_prefix){
                  status = napi_create_int64(env, 1, &is_prefixNAPI);
                  assert(status == napi_ok);
                }else{
                  status = napi_create_int64(env, 0, &is_prefixNAPI);
                  assert(status == napi_ok);
                }

                SystemMetadata lO_SystemMetadata;
                CustomMetadata lO_CustomMetadata;
                CustomMetadataEntry lO_CustomMetadataEntry;
                lO_SystemMetadata = lO_Object.system;
                lO_CustomMetadata = lO_Object.custom;
                napi_value entriesArray;
                status = napi_create_object(env,&entriesArray);
                assert(status == napi_ok);
                char empty[]="";
                if(lO_CustomMetadata.count>0){
                  CustomMetadataEntry* CustomMetadataEntryListPointer = lO_CustomMetadata.entries;
                  for(int j=0;j<lO_CustomMetadata.count;j++){
                    lO_CustomMetadataEntry = *(CustomMetadataEntryListPointer+j);
                    napi_value key_lengthNAPI,value_lengthNAPI,keyNAPI,valueNAPI,customMetaNAPI;
                    status = napi_create_object(env,&customMetaNAPI);
                    assert(status == napi_ok);

                    status = napi_create_int64(env, lO_CustomMetadataEntry.key_length, &key_lengthNAPI);
                    assert(status == napi_ok);
                
                    status = napi_create_int64(env, lO_CustomMetadataEntry.value_length, &value_lengthNAPI);
                    assert(status == napi_ok);

                    status = napi_create_string_utf8(env,lO_CustomMetadataEntry.key,NAPI_AUTO_LENGTH,&keyNAPI);
                    assert(status == napi_ok);
                    
                    status = napi_create_string_utf8(env,lO_CustomMetadataEntry.value,NAPI_AUTO_LENGTH,&valueNAPI);
                    assert(status == napi_ok);
                    
                    status = napi_set_named_property(env,customMetaNAPI,"key",keyNAPI);
                    assert(status == napi_ok);

                    status = napi_set_named_property(env,customMetaNAPI,"key_length",key_lengthNAPI);
                    assert(status == napi_ok);

                    status = napi_set_named_property(env,customMetaNAPI,"value",valueNAPI);
                    assert(status == napi_ok);

                    status = napi_set_named_property(env,customMetaNAPI,"value_length",value_lengthNAPI);
                    assert(status == napi_ok);

                    status = napi_set_named_property(env,customMetaNAPI,"value_length",value_lengthNAPI);
                    assert(status == napi_ok);

                    char str[100];
                    itoa(j, str, 10);
                    status = napi_set_named_property(env,entriesArray,str,customMetaNAPI);
                    assert(status == napi_ok);
                  }
                }else{
                  napi_value emptyNAPI;
                  status = napi_create_string_utf8(env,&empty[0],NAPI_AUTO_LENGTH,&emptyNAPI);
                  assert(status == napi_ok);

                  status = napi_set_named_property(env,entriesArray,"0",emptyNAPI);
                  assert(status == napi_ok);
                }
                napi_value createdNAPI;
                status = napi_create_int64(env, lO_ObjectPtr->system.created, &createdNAPI);
                assert(status == napi_ok);
                
                napi_value expiresNAPI;
                status = napi_create_int64(env, lO_ObjectPtr->system.expires, &expiresNAPI);
                assert(status == napi_ok);

                napi_value countNAPI;
                status = napi_create_int64(env, lO_CustomMetadata.count, &countNAPI);
                assert(status == napi_ok);
                //
                napi_value content_lengthNAPI;
                status = napi_create_int64(env, lO_SystemMetadata.content_length, &content_lengthNAPI);
                assert(status == napi_ok);

                status = napi_set_named_property(env,CustomMetadataNAPI,"entries",entriesArray);
                assert(status == napi_ok);
                
                status = napi_set_named_property(env,CustomMetadataNAPI,"count",countNAPI);
                assert(status == napi_ok);
                
                status = napi_set_named_property(env,objectNAPI,"custom",CustomMetadataNAPI);
                assert(status == napi_ok);
                
                status = napi_set_named_property(env,SystemMetadataNAPI,"created",createdNAPI);
                assert(status == napi_ok);
                
                status = napi_set_named_property(env,SystemMetadataNAPI,"expires",expiresNAPI);
                assert(status == napi_ok);
                
                status = napi_set_named_property(env,SystemMetadataNAPI,"content_length",content_lengthNAPI);
                assert(status == napi_ok);
                
                status = napi_set_named_property(env,objectNAPI,"system",SystemMetadataNAPI);
                assert(status == napi_ok);
                
                status = napi_set_named_property(env,objectNAPI,"is_prefix",is_prefixNAPI);
                assert(status == napi_ok);
                
                status = napi_set_named_property(env,objectNAPI,"key",keyObjectNAPI);
                assert(status == napi_ok);
                //
                char str[100];
                itoa(count, str, 10);

                status = napi_set_named_property(env,objectList,str,objectNAPI);
                assert(status == napi_ok);

                count++;
            }
            //
            Error *err = object_iterator_err(lO_ObjectIterator);
			
			napi_value errorMessage,errorCode;
            
            if(err==NULL){
			  
              int32_t code = 0;
              char message[] = "";
              napi_value codenapi,errormessage;
              //
              status = napi_create_int64(env, code, &codenapi);
              assert(status == napi_ok);
              //
              status = napi_create_string_utf8(env,&message[0],NAPI_AUTO_LENGTH,&errormessage);
              assert(status == napi_ok);
              //
              status = napi_set_named_property(env,errorObject,"code",codenapi);
              assert(status == napi_ok);
              //
              status = napi_set_named_property(env,errorObject,"message",errormessage);
              assert(status == napi_ok);
            }else{
		
                Error lO_Error = *(err);
                char* messagePtr = lO_Error.message;
                napi_value codenapi,errormessage;
                
                status = napi_create_int64(env, lO_Error.code, &codenapi);
                assert(status == napi_ok);
              
                status = napi_create_string_utf8(env,messagePtr,NAPI_AUTO_LENGTH,&errormessage);
                assert(status == napi_ok);

                status = napi_set_named_property(env,errorObject,"code",codenapi);
                assert(status == napi_ok);

                status = napi_set_named_property(env,errorObject,"message",errormessage);
                assert(status == napi_ok);
			}
            status = napi_set_named_property(env,returnObject,"error",errorObject);
            assert(status == napi_ok);
            //
            status = napi_set_named_property(env,returnObject,"objectList",objectList);
            assert(status == napi_ok);

            return returnObject;
          }
        }  
      }

    }
  }
  
  return NULL;
}

//  * function access_serializec serializes access grant into a string.
//  * pre-requisites: parse_accessc
//  * inputs: Access (Object)  
//  * output: String Result (object)
napi_value access_serializec(napi_env env, napi_callback_info info) {
  napi_status status;
  size_t argc = 1;
  napi_value args[1];
  //
  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  //
  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 3 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }

  Access lO_Access;
  lO_Access._handle = lO_Access._handle = getHandleValue(env,args[0]);
  if(lO_Access._handle==0){
    return NULL;
  }
  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found \n");
    return NULL;
  }else{
    
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"access_serialize");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found \n");
      return NULL;
    }else{
      typedef StringResult(__stdcall * pICStringResult)(Access *);
      pICStringResult access_serialize;
      access_serialize = pICStringResult(fn);
      StringResult lO_StringResult = access_serialize(&lO_Access);
      //
      napi_value errorObject,returnObject;
      
      status = napi_create_object(env,&returnObject);
      assert(status == napi_ok);
      
      status = napi_create_object(env,&errorObject);
      assert(status == napi_ok);
      //
      char blank[] = "";

      napi_value errorCode,errorMessage,stringNAPI;
      if(lO_StringResult.error!=NULL){
        Error lO_errorObject = *(lO_StringResult.error);

        status = napi_create_int64(env, lO_errorObject.code, &errorCode);
        assert(status == napi_ok);

        char* errorMessagePtr = lO_errorObject.message;

        status = napi_create_string_utf8(env,&errorMessagePtr[0],NAPI_AUTO_LENGTH,&errorMessage);
        assert(status == napi_ok);

        status = napi_create_string_utf8(env,&blank[0],NAPI_AUTO_LENGTH,&stringNAPI);
        assert(status == napi_ok); 
      }else{
        
        char* StringResultPtr= lO_StringResult.string;
        status = napi_create_int64(env, 0, &errorCode);
        assert(status == napi_ok);
      
        
        status = napi_create_string_utf8(env,&blank[0],NAPI_AUTO_LENGTH,&errorMessage);
        assert(status == napi_ok);
        
        
        status = napi_create_string_utf8(env,StringResultPtr,NAPI_AUTO_LENGTH,&stringNAPI);
        assert(status == napi_ok); 
      }
        
      status = napi_set_named_property(env,errorObject,"code",errorCode);
      assert(status == napi_ok);
      
      status = napi_set_named_property(env,errorObject,"message",errorMessage);
      assert(status == napi_ok);
       
      status = napi_set_named_property(env,returnObject,"error",errorObject);
      assert(status == napi_ok);

      status = napi_set_named_property(env,returnObject,"string",stringNAPI);
      assert(status == napi_ok);
      return returnObject;

    }
  }
  return NULL;
}


//  * function upload_infoc returns the last information about the uploaded object.
//  * pre-requisites: upload_objectc
//  * inputs: Upload (Object)  
//  * output: Object Result (object)
napi_value upload_infoc(napi_env env, napi_callback_info info) {
  napi_status status;
  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  
  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 1 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);
  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! first argument excepted to be object type\n");
    return NULL;
  }
  
  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }

  Upload lO_Upload;
  lO_Upload._handle = getHandleValue(env,args[0]);
  if(lO_Upload._handle==0){
    napi_throw_type_error(env,nullptr,"\nInvalid Object\n");
    return NULL;
  }
  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found\n");
    return NULL;
  }else{
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"upload_info");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found\n");
      return NULL;
    }else{
      typedef ObjectResult(__stdcall * pICObjectResult)(Upload *);
      pICObjectResult upload_info;
      upload_info = pICObjectResult(fn);
      ObjectResult lO_ObjectResult = upload_info(&lO_Upload);
      if(lO_ObjectResult.error!=NULL){
        return createObjectResult(env,NULL,lO_ObjectResult.error);
      }else{
        return createObjectResult(env,lO_ObjectResult.object,NULL);
      }
    }
  }
  return NULL;
}

//  * function upload_abortc aborts an upload.
//  * pre-requisites: upload_objectc
//  * inputs: Upload (Object)  
//  * output: Error (Object)
napi_value upload_abortc(napi_env env, napi_callback_info info){
  napi_status status;

  size_t argc = 1;
  napi_value args[1];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);

  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 1 arguments\n");
    return NULL;
  }

  
  napi_valuetype checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);

  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! First argument excepted to be object type\n");
    return NULL;
  }

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }
  Upload lO_Upload;
  lO_Upload._handle = getHandleValue(env,args[0]);
  
  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found \n");
    return NULL;
  }else{
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"upload_abort");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found \n");
      return NULL;
    }else{
      typedef Error *(__stdcall * pICError)(Upload *);
      pICError upload_abort;
      upload_abort = pICError(fn);
      Error* lO_ErrorPtr = upload_abort(&lO_Upload);
      //
      if(lO_ErrorPtr!=NULL){
        Error lO_Error = *(lO_ErrorPtr);
        char* errorMessagePtr = lO_Error.message;
        return createError(env,lO_Error.code,&errorMessagePtr[0]);
      }
      char blank[] = "";
      return createError(env,0,&blank[0]);
    }
  }
  return NULL;
}

//  * function download_infoc returns information about the downloaded object.
//  * pre-requisites: download_objectc
//  * inputs: Download (Object)  
//  * output: Object Result (Object)
napi_value download_infoc(napi_env env, napi_callback_info info) {
  napi_status status;
  size_t argc = 1;
  napi_value args[1];

   status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
   assert(status == napi_ok);
  
  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 1 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;

  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);
  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! first argument excepted to be object type\n");
    return NULL;
  }
  
  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }

  Download lO_Download;
  lO_Download._handle = getHandleValue(env,args[0]);
  //
  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found \n");
    return NULL;
  }else{
    
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"download_info");
    if(!fn){
     napi_throw_type_error(env,nullptr,"\nFunction not found \n");
      return NULL;
    }else{
      typedef ObjectResult(__stdcall * pICObjectResult)(Download *);
      pICObjectResult download_info;
      download_info = pICObjectResult(fn);
      ObjectResult lO_ObjectResult = download_info(&lO_Download);
  
      if(lO_ObjectResult.error!=NULL){
        return createObjectResult(env,NULL,lO_ObjectResult.error);
      }else{
        return createObjectResult(env,lO_ObjectResult.object,NULL);
      }
    
    }
  }
  //
  return NULL;
}

//  * function upload_set_custom_metadatac set custom meta information
//  * pre-requisites: upload_objectc
//  * inputs: Upload (Object), CustomMetadata 
//  * output: Error (object)
napi_value upload_set_custom_metadatac(napi_env env, napi_callback_info info){
  napi_status status;
  //
  size_t argc = 2;
  napi_value args[2];

  status = napi_get_cb_info(env, info,&argc,args, nullptr ,nullptr);
  assert(status == napi_ok);
  if (argc < 2) {
    napi_throw_type_error(env, nullptr, "\nWrong number of arguments!! excepted 1 arguments\n");
    return NULL;
  }

  napi_valuetype checktypeofinput;
  status = napi_typeof(env,args[0],&checktypeofinput);
  assert(status == napi_ok);
  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! first argument excepted to be object type\n");
    return NULL;
  }

  status = napi_typeof(env,args[1],&checktypeofinput);
  assert(status == napi_ok);
  if(checktypeofinput!=napi_object){
    napi_throw_type_error(env,nullptr,"\nWrong datatype !! second argument excepted to be array\n");
    return NULL;
  }
  

  bool propertyexists = false;
  napi_value ObjectkeyNAPI;
  char* handle = "_handle";
  status = napi_create_string_utf8(env,handle,NAPI_AUTO_LENGTH ,&ObjectkeyNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[0],ObjectkeyNAPI,&propertyexists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nInvalid Object \n");
    return NULL;
  }
  Upload lO_Upload;
  lO_Upload._handle = getHandleValue(env,args[0]);
  if(lO_Upload._handle==0){
    napi_throw_type_error(env,nullptr,"\nWrong object passed\n");
    return NULL;
  }
  //Checking Property exits aur not
  bool entriesExists = false;
  napi_value entriesStringNAPI;
  char* entries = "entries";
  status = napi_create_string_utf8(env,entries,NAPI_AUTO_LENGTH ,&entriesStringNAPI);
  assert(status == napi_ok);
  //
  status = napi_has_property(env,args[1],entriesStringNAPI,&entriesExists);
  assert(status == napi_ok);
  if(!propertyexists){
    napi_throw_type_error(env,nullptr,"\nWrong Object Passed In Second Parameter\n");
    return NULL;
  }
  //Fetching Entries
  napi_value entriesArrayNAPI;
  status = napi_get_named_property(env,args[1],"entries",&entriesArrayNAPI);
  assert(status == napi_ok);

  napi_value countNAPI;
  status = napi_get_named_property(env,args[1],"count",&countNAPI);
  assert(status == napi_ok);
  int64_t countc;
  status = napi_get_value_int64(env,countNAPI,&countc);
  assert(status == napi_ok);
  
  //Checking whether is array of not
  bool isarray=false;
  status = napi_is_array(env,entriesArrayNAPI,&isarray);
  assert(status == napi_ok);
  if(!isarray){
    napi_throw_type_error(env,nullptr,"\nEntries is not array\n");
    return NULL;
  }
  
  uint32_t sizeOfArray = 0;
  status = napi_get_array_length(env,entriesArrayNAPI,&sizeOfArray);
  assert(status == napi_ok);
  
  int size = (int)sizeOfArray;
  CustomMetadataEntry* CustomMetadataEntryListPointer = new CustomMetadataEntry[size];
  CustomMetadata lO_CustomMetadata;
  napi_value CustomMetadataEntryObject;
  
  for(int i=0;i<size;i++){
    CustomMetadataEntry lO_CustomMetadataEntry;
    status = napi_get_element(env,entriesArrayNAPI,i,&CustomMetadataEntryObject);
    assert(status == napi_ok);

    napi_value keyNAPI,key_lengthNAPI,valueNAPI,value_lengthNAPI;
    int64_t key_lengthc,value_lengthc;

    status = napi_get_named_property(env,CustomMetadataEntryObject,"key",&keyNAPI);
    assert(status == napi_ok);

    size_t bufsize = 0;
    size_t convertedvalue = 0;

    status = napi_get_value_string_utf8(env,keyNAPI,NULL,bufsize,&convertedvalue);
    assert(status == napi_ok);
    convertedvalue = convertedvalue+1;

    char* keyc =  new char[convertedvalue];
    status = napi_get_value_string_utf8(env,keyNAPI,keyc,convertedvalue,&bufsize);
    assert(status == napi_ok);

    status = napi_get_named_property(env,CustomMetadataEntryObject,"key_length",&key_lengthNAPI);
    assert(status == napi_ok);
    status = napi_get_value_int64(env,key_lengthNAPI,&key_lengthc);
    assert(status == napi_ok);

    status = napi_get_named_property(env,CustomMetadataEntryObject,"value",&valueNAPI);
    assert(status == napi_ok);
    
    bufsize = 0;
    convertedvalue = 0;
    status = napi_get_value_string_utf8(env, valueNAPI,NULL,bufsize,&convertedvalue);
    assert(status == napi_ok);
    convertedvalue = convertedvalue+1;
    
    char* valuec =  new char[convertedvalue];
    status = napi_get_value_string_utf8(env,valueNAPI,valuec,convertedvalue,&bufsize);
    assert(status == napi_ok);

    status = napi_get_named_property(env,CustomMetadataEntryObject,"value_length",&value_lengthNAPI);
    assert(status == napi_ok);

    int64_t value_lengthsize;

    status = napi_get_value_int64(env,value_lengthNAPI,&value_lengthsize);
    assert(status == napi_ok);

    lO_CustomMetadataEntry.key = keyc;
    lO_CustomMetadataEntry.key_length =key_lengthc;
    lO_CustomMetadataEntry.value=valuec;
    lO_CustomMetadataEntry.value_length = value_lengthsize;
    *(CustomMetadataEntryListPointer+i)=lO_CustomMetadataEntry;

  }
  lO_CustomMetadata.entries = (CustomMetadataEntryListPointer+0);
  if(countc==size){
    lO_CustomMetadata.count = countc;
  }else{
    lO_CustomMetadata.count = sizeOfArray;
  }
  
  //
  if(!hGetProcIDDLL){
    napi_throw_type_error(env,nullptr,"\nLibrary not found\n");
    return NULL;
  }else{
    
    FARPROC fn = GetProcAddress(HMODULE(hGetProcIDDLL),"upload_set_custom_metadata");
    if(!fn){
      napi_throw_type_error(env,nullptr,"\nFunction not found\n");
      return NULL;
    }else{
      typedef Error *(__stdcall * pICError)(Upload *,CustomMetadata);
      pICError upload_set_custom_metadata;
      upload_set_custom_metadata = pICError(fn);
      Error* lO_ErrorPtr = upload_set_custom_metadata(&lO_Upload,lO_CustomMetadata);
      if(lO_ErrorPtr!=NULL){
        Error lO_Error = *(lO_ErrorPtr);
        char* errorMessagePtr = lO_Error.message;
        return createError(env,lO_Error.code,&errorMessagePtr[0]);
      }

      char blank[] = "";
      return createError(env,0,&blank[0]);
    }
  }
  return NULL;
}

#define DECLARE_NAPI_METHOD(name, func)                                        \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value Init(napi_env env, napi_value exports) {
  
  napi_status status;
  
  napi_property_descriptor parse_access = DECLARE_NAPI_METHOD("parse_accessc", parse_accessc);
  status = napi_define_properties(env, exports, 1, &parse_access);
  assert(status == napi_ok);
 
  napi_property_descriptor request_access_with_passphrase = DECLARE_NAPI_METHOD("request_access_with_passphrasec", request_access_with_passphrasec);
  status = napi_define_properties(env, exports, 1, &request_access_with_passphrase);
  assert(status == napi_ok);
  
  napi_property_descriptor  open_project = DECLARE_NAPI_METHOD("open_projectc", open_projectc);
  status = napi_define_properties(env, exports, 1, & open_project);
  assert(status == napi_ok);
  
  napi_property_descriptor close_project = DECLARE_NAPI_METHOD("close_projectc", close_projectc);
  status = napi_define_properties(env, exports, 1, &close_project);
  assert(status == napi_ok);
  
  napi_property_descriptor stat_bucket = DECLARE_NAPI_METHOD("stat_bucketc", stat_bucketc);
  status = napi_define_properties(env, exports, 1, &stat_bucket);
  assert(status == napi_ok);
  
  napi_property_descriptor create_bucket = DECLARE_NAPI_METHOD("create_bucketc", create_bucketc);
  status = napi_define_properties(env, exports, 1, &create_bucket);
  assert(status == napi_ok);
  
  napi_property_descriptor ensure_bucket = DECLARE_NAPI_METHOD("ensure_bucketc", ensure_bucketc);
  status = napi_define_properties(env, exports, 1, &ensure_bucket);
  assert(status == napi_ok);

  napi_property_descriptor delete_bucket = DECLARE_NAPI_METHOD("delete_bucketc", delete_bucketc);
  status = napi_define_properties(env, exports, 1, &delete_bucket);
  assert(status == napi_ok);
  
  napi_property_descriptor list_buckets = DECLARE_NAPI_METHOD("list_bucketsc", list_bucketsc);
  status = napi_define_properties(env, exports, 1, &list_buckets);
  assert(status == napi_ok);
  
  napi_property_descriptor stat_object = DECLARE_NAPI_METHOD("stat_objectc", stat_objectc);
  status = napi_define_properties(env, exports, 1, &stat_object);
  assert(status == napi_ok);
  
  napi_property_descriptor delete_object = DECLARE_NAPI_METHOD("delete_objectc", delete_objectc);
  status = napi_define_properties(env, exports, 1, &delete_object);
  assert(status == napi_ok);
   
  napi_property_descriptor upload_object = DECLARE_NAPI_METHOD("upload_objectc", upload_objectc);
  status = napi_define_properties(env, exports, 1, &upload_object);
  assert(status == napi_ok);
 
  napi_property_descriptor upload_write = DECLARE_NAPI_METHOD("upload_writec", upload_writec);
  status = napi_define_properties(env, exports, 1, &upload_write);
  assert(status == napi_ok);
  
  napi_property_descriptor upload_commit = DECLARE_NAPI_METHOD("upload_commitc", upload_commitc);
  status = napi_define_properties(env, exports, 1, &upload_commit);
  assert(status == napi_ok);
  
  napi_property_descriptor download_object = DECLARE_NAPI_METHOD("download_objectc", download_objectc);
  status = napi_define_properties(env, exports, 1, &download_object);
  assert(status == napi_ok);
  
  napi_property_descriptor download_read = DECLARE_NAPI_METHOD("download_readc", download_readc);
  status = napi_define_properties(env, exports, 1, &download_read);
  assert(status == napi_ok);
  
  napi_property_descriptor close_download = DECLARE_NAPI_METHOD("close_downloadc", close_downloadc);
  status = napi_define_properties(env, exports, 1, &close_download);
  assert(status == napi_ok);
 
  napi_property_descriptor list_objects = DECLARE_NAPI_METHOD("list_objectsc", list_objectsc);
  status = napi_define_properties(env, exports, 1, &list_objects);
  assert(status == napi_ok);
   
  napi_property_descriptor access_share = DECLARE_NAPI_METHOD("access_sharec", access_sharec);
  status = napi_define_properties(env, exports, 1, &access_share);
  assert(status == napi_ok);
  /**/
  napi_property_descriptor access_serialize = DECLARE_NAPI_METHOD("access_serializec", access_serializec);
  status = napi_define_properties(env, exports, 1, &access_serialize);
  assert(status == napi_ok);
  
  napi_property_descriptor download_info = DECLARE_NAPI_METHOD("download_infoc", download_infoc);
  status = napi_define_properties(env, exports, 1, &download_info);
  assert(status == napi_ok);
  
  napi_property_descriptor upload_info = DECLARE_NAPI_METHOD("upload_infoc", upload_infoc);
  status = napi_define_properties(env, exports, 1, &upload_info);
  assert(status == napi_ok);
  
  napi_property_descriptor upload_abort = DECLARE_NAPI_METHOD("upload_abortc", upload_abortc);
  status = napi_define_properties(env, exports, 1, &upload_abort);
  assert(status == napi_ok);
 
  napi_property_descriptor upload_set_custom_metadata = DECLARE_NAPI_METHOD("upload_set_custom_metadatac", upload_set_custom_metadatac);
  status = napi_define_properties(env, exports, 1, &upload_set_custom_metadata);
  assert(status == napi_ok);
  
  loaddll();
  return exports;
}
//

//
NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
