/*
 * Node.js Bindings for Storj (V3)
 */

// Import modules to bind C functions, exported from golang library
const fs = require("fs");
//
const ffi = require("ffi-napi");
//
const struct = require("ref-struct");
const ref = require("ref-napi");


/* Define required datatypes to communicate with C functions */
// uint8*
const uint8Ptr_t = ref.refType(ref.types.uint8);
//
const string_t = ref.types.CString;  // ref.refType(ref.types.char);
//
// string*
const stringPtr_t = ref.refType(string_t);


/* Define structures for golang structure objects */

// Uplink configuration nested structure
const tls = struct({
	skip_peer_ca_whitelist : "bool"
});

const Volatile = struct({
    tls             : tls,
    peer_id_version : string_t,
    max_inline_size : "int32",
    max_memory      : "int32",
    dial_timeout    : "int32"
});

const UplinkConfig = struct({
	Volatile : Volatile
});

// Uplink reference structure
const UplinkRef = struct({
    _handle: "long"
});

// API key reference structure
const APIKeyRef = struct({
	_handle : "long"
});

// Project reference structure
const ProjectRef = struct({
    _handle: "long"
});

// Access reference structure
const EncryptionAccessRef = struct({
    _handle: "long"
});

// Bucket reference structure
const BucketRef = struct({
    _handle: "long"
});                                                                                 

// Uploader reference structure
const UploaderRef = struct({
    _handle: "long"
});

// Downloader reference structure
const DownloaderRef = struct({
    _handle: "long"
});


const EncryptionParameters = struct({
    cipher_suite: "uint32",
    block_size  : "int32"
});

const RedundancyScheme = struct({
    algorithm      : "uint32",
    share_size     : "int32",
    required_shares: "int16",
    repair_shares  : "int16",
    optimal_shares : "int16",
    total_shares   : "int16"
});

const BucketConfig = struct({
    path_cipher          : "uint64",
    encryption_parameters: EncryptionParameters,
    redundancy_scheme    : RedundancyScheme 
});

const BucketInfo = struct ({
    name                 : string_t,
    created              : "int64",
    path_cipher          : "uint64",
    segment_size         : "uint64",
    encryption_parameters: EncryptionParameters,
    redundancy_scheme    : RedundancyScheme
});
const BucketInfoPtr = ref.refType(BucketInfo);

const BucketListOptions = struct({
    cursor   : string_t,
    direction: "uint32",
    limit    : "int64"
});

const BucketList = struct({
    more  : ref.types.bool,
    items : BucketInfoPtr, 
    length: "int32"
});

const ListOptions = struct({
    prefix   : string_t,
    cursor   : string_t,
    delimiter: ref.types.char,
    recursive: ref.types.bool,
    direction: "uint32",
    limit    : "int64"
});

const ObjectInfo = struct({
    version     : "uint32",
    bucket      : BucketInfo,
    path        : string_t,
    is_prefix   : ref.types.bool,
    content_type: string_t,
    size        : "int64",
    created     : "int64",
    modified    : "int64",
    expires     : "int64"
});
const ObjectInfoPtr = ref.refType(ObjectInfo);

const ObjectList = struct({
    bucket: string_t,
    prefix: string_t,
    more  : ref.types.bool,
    items : ObjectInfoPtr, 
    length: "int32"
});


/* 
 * Include the golang exported libuplinkc library functions, and
 * declare types of arguments and response of each of these functions
 */

// import the config.js module and
// destructure to access environment variables
//var { LIB_UPLINKC_PATH } = require('./config');
//
//if ((typeof LIB_UPLINKC_PATH == "undefined") || (!LIB_UPLINKC_PATH)) {
    // set default path
    LIB_UPLINKC_PATH = "./libuplinkc.so";
//}
//
var libUplink = ffi.Library(LIB_UPLINKC_PATH,
    {
    	new_uplink   : [ UplinkRef,  [ UplinkConfig,    string_t,   stringPtr_t ] ],
    	close_uplink : [ "void" ,    [ UplinkRef,       stringPtr_t ] ],
        
        parse_api_key: [ APIKeyRef,  [ string_t,        stringPtr_t ] ],
    	open_project : [ ProjectRef, [ UplinkRef,       string_t,   APIKeyRef,  stringPtr_t ] ],
        close_project: [ "void" ,    [ ProjectRef,      stringPtr_t ] ],
        
        project_salted_key_from_passphrase    : [ uint8Ptr_t ,          [ ProjectRef,           string_t,  stringPtr_t ] ],
        new_encryption_access_with_default_key: [ EncryptionAccessRef,  [ uint8Ptr_t ] ],
        serialize_encryption_access           : [ string_t,             [ EncryptionAccessRef,  stringPtr_t ] ],

        list_buckets    : [ BucketList, [ ProjectRef,   ref.refType(BucketListOptions), stringPtr_t ] ],
        free_bucket_list: [ "void",     [ ref.refType(BucketList) ] ],

        create_bucket: [ BucketInfo, [ ProjectRef,      string_t,       ref.refType(BucketConfig),  stringPtr_t ] ],
        delete_bucket: [ "void",     [ ProjectRef,      string_t,   stringPtr_t ] ],
        open_bucket	 : [ BucketRef,  [ ProjectRef,      string_t,       string_t,                   stringPtr_t ] ],
        close_bucket : [ "void" ,    [ BucketRef,       stringPtr_t ] ],
        
        list_objects     : [ ObjectList, [ BucketRef,   ref.refType(ListOptions),   stringPtr_t ] ],
        free_list_objects: [ "void",     [ ref.refType(ObjectList) ] ],
        
        upload       : [ UploaderRef, [ BucketRef,      string_t,       string_t,   stringPtr_t ] ],
        upload_write : [ "size_t",    [ UploaderRef,    uint8Ptr_t,     "size_t",   stringPtr_t ] ],
        upload_commit: [ "void",      [ UploaderRef,    stringPtr_t ] ],
        
        download      : [ DownloaderRef, [ BucketRef,     string_t,     string_t,   stringPtr_t ] ],
        download_read : [ "size_t",      [ DownloaderRef, uint8Ptr_t,   "size_t",   stringPtr_t ] ]  ,
        download_close: [ "void",        [ DownloaderRef, stringPtr_t ] ],

        delete_object: [ "void", [ BucketRef, string_t, stringPtr_t ] ]
    }
);


/*
 * Declare Node.js binding methods for required libuplink golang functions
 */

// function to create new Storj uplink
// pre-requisites: none
// inputs: none
// output: valid UplinkRef, else error in catch
let new_uplink = function() {
    
    return new Promise(function(resolve, reject) {
        // set-up uplink configuration
        lO_uplinkConfig = new UplinkConfig();
        lO_uplinkConfig.Volatile.tls.skip_peer_ca_whitelist = true;
        lO_uplinkConfig.Volatile.dial_timeout = 2147483645;
        //
        // Define a null pointer local variable, to store error when a function is called
        var errorPtrPtr = ref.NULL.ref();
        //
        // create new uplink by calling the exported golang function
        var ls_tempdir = ""
        var tmpdirPtr = ref.allocCString(ls_tempdir);
        var uplinkRef = libUplink.new_uplink(lO_uplinkConfig, tmpdirPtr,errorPtrPtr);
        //
        uplinkRef.type = UplinkRef;
        //
        // if handle is received
        if (uplinkRef._handle > 0) {
            resolve(uplinkRef);
        } else {
            var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
            //
            errorPtrPtr = null;
            //
            reject(ls_error);
        }   
    });
};
 
// function to close currently established uplink
// pre-requisites: new_uplink() function has been already called
// inputs: valid UplinkRef
// output: None, else error in catch
let close_uplink = function(pO_uplinkRef) {
    
    return new Promise(function(resolve, reject) {
        // Define char* type local variable, to store error when a function is called
        var errorPtrPtr = ref.NULL.ref();     
        //
        //  by calling the exported golang function
        libUplink.close_uplink(pO_uplinkRef, errorPtrPtr);
        //
        // check for any possible error after closing the uplink
        if (errorPtrPtr.deref().isNull() == false) {
            var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
            //
            errorPtrPtr = null;
            //
            reject(ls_error);
        } else {
            resolve(true);
        }
    });
};


// function to parse API key, to be used by Storj
// pre-requisites: None
// inputs: API key (string)
// output: valid APIKeyRef, else error in catch
let parse_api_key = function(ps_apiKey) {

    return new Promise(function(resolve, reject) {
        // prepare the input for the function, by converting it into char*
        var lc_apiKeyPtr = ref.allocCString(ps_apiKey);
        //
        // Define char* type local variable, to store error when a function is called
        var errorPtrPtr = ref.NULL.ref();
        //
        // parse the API key by calling the exported golang function
        var parsedAPIKeyRef = libUplink.parse_api_key(lc_apiKeyPtr, errorPtrPtr);
        parsedAPIKeyRef.type = APIKeyRef;
        //
        // if handle is received
        if (parsedAPIKeyRef._handle > 0) {
            resolve(parsedAPIKeyRef);
        } else {
            var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
            //
            errorPtrPtr = null;
            //
            reject(ls_error);
        }
    });
};
 
// function to open a Storj project
// pre-requisites: new_uplink() and parse_api_key() functions have been already called
// inputs: valid UplinkRef, Satellite Address (string), valid APIKeyRef
// output: valid ProjectRef, else error in catch
let open_project = function(pO_uplinkRef, ps_satellite, pO_apiKeyRef) {

    return new Promise(function(resolve, reject) {
        // prepare the input for the function, by converting it into char*
        var lc_satellitePtr = ref.allocCString(ps_satellite);
        //
        // Define char* type local variable, to store error when a function is called
        var errorPtrPtr = ref.NULL.ref();
        //
        // open project by calling the exported golang function
        var projectRef = libUplink.open_project(pO_uplinkRef, lc_satellitePtr, pO_apiKeyRef, errorPtrPtr);
        projectRef.type = ProjectRef;
        //
        // if handle is received
        if (projectRef._handle > 0) {
            resolve(projectRef);
        }
        else {
            var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
            //
            errorPtrPtr = null;
            //
            reject(ls_error);
        }
    });
};
 
// function to close currently open Storj project
// pre-requisites: open_project() function has been already called
// inputs: valid ProjectRef
// output: None, else error in catch 
let close_project = function(pO_projectRef) {

    return new Promise(function(resolve, reject) {
        // Define char* type local variable, to store error when a function is called
        var errorPtrPtr = ref.NULL.ref();   
        //
        //  by calling the exported golang function
        libUplink.close_project(pO_projectRef, errorPtrPtr);
        //
        // check for any possible error after closing the uplink
        if (errorPtrPtr.deref().isNull() == false) {
            var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
            //
            errorPtrPtr = null;
            //
            reject(ls_error);
        } else {
            resolve(true);
        }    
    });
};


// function to fetch a list of buckets, available in the Storj project
// pre-requisites: open_project() function has been already called
// inputs: valid ProjectRef
// output: BucketList, else error in catch
let list_buckets = function(pO_projectRef) {

    return new Promise(function(resolve, reject) {
        // Define char* type local variable, to store error when a function is called
        var errorPtrPtr = ref.NULL.ref();   
        //
        // get buckets' list, by calling the exported golang function
        var bucketList = libUplink.list_buckets(pO_projectRef, null, errorPtrPtr);
        //
        // check for any possible error while listing buckets
        if (errorPtrPtr.deref().isNull() == false) {
            var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
            //
            errorPtrPtr = null;
            //
            reject(ls_error);
        } else {
            // parse the list, as per the defined structure
            bucketList.type = BucketList;
            //
            var bucketsInfo = [];
            //
            // fetch information of each bucket in the list
            for (var i = 0; i < bucketList.length; i++) {
                var offset = i * BucketInfoPtr.size;
                //
                var bucketInfo = {
                    name        : ref.get(bucketList.items, offset, string_t),
                    created     : ref.get(bucketList.items, offset + string_t.size, "int64"),
                    path_cipher : ref.get(bucketList.items, offset + string_t.size + ref.types.int64.size, "uint64"),
                    segment_size: ref.get(bucketList.items, offset + string_t.size + ref.types.int64.size + ref.types.uint64.size, "uint64")
                };
                //
                // encryption parameters
                offset = offset + string_t.size + ref.types.int64.size + ref.types.uint64.size + ref.types.uint64.size;
                //
                var encryptParams = parseUInt64To32(ref.get(bucketList.items, offset, "uint64"));
                //
                bucketInfo.encryption_parameters = {
                    cipher_suite: encryptParams.hi32,
                    block_size  : encryptParams.lo32
                }
                //
                // redundancy scheme
                offset = offset + EncryptionParameters.size;
                //
                var redundantScheme = {
                    hi64: parseUInt64To32(ref.get(bucketList.items, offset, "uint64")),
                    lo64: parseUInt64To16(ref.get(bucketList.items, offset + ref.types.uint64.size, "uint64"))
                };
                //
                bucketInfo.redundancy_scheme = {
                    algorithm      : redundantScheme.hi64.hi32,
                    share_size     : redundantScheme.hi64.lo32,
                    required_shares: redundantScheme.lo64.hh16,
                    repair_shares  : redundantScheme.lo64.hi16,
                    optimal_shares : redundantScheme.lo64.lo16,
                    total_shares   : redundantScheme.lo64.ll16
                };
                //
                bucketsInfo.push(bucketInfo);
            }
            // recreate a buckets' list, as a JavaScript object
            var bucketsList = {
                more  : bucketList.more,
                items : bucketsInfo,
                length: bucketList.length
            };
            //
            // free the received buckets' list, by calling the exported golang function
            libUplink.free_bucket_list(bucketList.ref());
            //
            resolve(bucketsList);
        }
    });
}
//
// parse 64-bits into High and Low 32-bits
function parseUInt64To32(allBits) {
    if (typeof allBits == "string") {
        // 64-bits number
        return {
            hi32: parseInt(allBits.slice(0, 32), 10),
            lo32: parseInt(allBits.slice(32, 64), 10)
        };    
    }
    //
    // JavaScript integer
    return {
        hi32: (allBits >> 32),
        lo32: (allBits & 0xffffffff)
    };
}
//
// parse 64-bits into Highest-High, High, Low and Lowest-Low 16-bits
function parseUInt64To16(allBits) {
    if (typeof allBits == "string") {
        // 64-bits number
        return {
            hh16: parseInt(allBits.slice(0, 16), 10),
            hi16: parseInt(allBits.slice(16, 32), 10),
            lo16: parseInt(allBits.slice(32, 48), 10),
            ll16: parseInt(allBits.slice(48, 64), 10)
        };    
    }
    // JavaScript integer
    // lowest low 16-bits
    var retNums = {
        ll16: (allBits & 0x0000ffff)
    };
    //
    // low 16-bits
    allBits = (allBits >> 16);
    retNums.lo16 = (allBits & 0x0000ffff);
    //
    // high 16-bits
    allBits = (allBits >> 16);
    retNums.hi16 = (allBits & 0x0000ffff);
    //
    // highest high 16-bits
    retNums.hh16 = (allBits >> 16);
    //
    return retNums;
}

// function to create new bucket in Storj project
// pre-requisites: open_project() function has been already called
// inputs: valid BucketRef, Bucket Name (string)
// output: BucketInfo, else error in catch
let create_bucket = function(pO_projectRef, ps_bucketName) {

    return new Promise(function(resolve, reject) {
        // prepare the input for the function, by converting it into char*
        var bucketNamePtr = ref.allocCString(ps_bucketName);
        //
        var bucketOptions = ref.allocCString(ref.NULL);
        //
        // Define char* type local variable, to store error when a function is called
        var errorPtrPtr = ref.NULL.ref();   
        //
        // create a new by calling the exported golang function
        var bucketInfo = libUplink.create_bucket(pO_projectRef, bucketNamePtr, bucketOptions, errorPtrPtr);
        //
        // check for any possible error after creating the bucket
        if (errorPtrPtr.deref().isNull() == false) {
            var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
            //
            errorPtrPtr = null;
            //
            reject(ls_error);
        } else {
            resolve(bucketInfo);
        }
    });
};

// function to delete a bucket
// pre-requisites: open_project() function has been already called, successfully
// inputs: Storj Project (ProjectRef), Bucket Name (string)
// output: None, else Error (string) in catch
let delete_bucket = function(projectRef, ps_bucketName) {
      
    return new Promise(function(resolve, reject) { 
        // Define char* type local variable, to store error when a function is called
        var errorPtrPtr = ref.NULL.ref();

        libUplink.delete_bucket(projectRef, ref.allocCString(ps_bucketName), errorPtrPtr);
        //
        if (errorPtrPtr.deref().isNull() == false) {
            var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
            //
            errorPtrPtr = null;
            //
            reject(ls_error);
        } else {
            resolve(true);
        }
  });
};

// function to get encryption access to upload and/or download data to/from Storj
// pre-requisites: open_project() function has been already called
// inputs: valid ProjectRef, Encryption Pass Phrase (string)
// output: serialized EncryptionAccess (string), else error in catch
let encryption_key = function(pO_projectRef, ps_encryptionPassphrase) {
    
    return new Promise(function(resolve, reject) {
        // prepare the input for the function, by converting it into char*
        var encryptionPassphrasePtr = ref.allocCString(ps_encryptionPassphrase);
        //
        // Define char* type local variable, to store error when a function is called
        var errorPtrPtr = ref.NULL.ref();
        //
        //  by calling the exported golang function
        var saltedKeyPtr = libUplink.project_salted_key_from_passphrase(pO_projectRef, encryptionPassphrasePtr, errorPtrPtr);
        //
        saltedKeyPtr.type = uint8Ptr_t;
        //
        // check for any possible error after download
        if (errorPtrPtr.deref().isNull() == false) {
            var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
            //
            errorPtrPtr = null;
            //
            reject(ls_error);
        } else {
            var encryptionAccessRef = libUplink.new_encryption_access_with_default_key(saltedKeyPtr);
            encryptionAccessRef.type = EncryptionAccessRef;
            //
            // if handle is received
            if (encryptionAccessRef._handle > 0) {
                var serializedEncryptionAccessPtr = libUplink.serialize_encryption_access(encryptionAccessRef, errorPtrPtr);
                serializedEncryptionAccessPtr.type = string_t;
                //
                // check for any possible error after download
                if (errorPtrPtr.deref().isNull() == false) {
                    var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
                    //
                    errorPtrPtr = null;
                    //
                    reject(ls_error);
                } else {
                    resolve(serializedEncryptionAccessPtr);
                }
            } else {
                reject("FAILED to created encryption access from the salted key!");
            } 
        }
    });
}; 

// function to open an already existing bucket in Storj project
// pre-requisites: encryption_key() function has been already called
// inputs: ProjectRef, Bucket Name (string), serialized EncryptionAccess (string)
// output: valid BucketRef, else error in catch
let open_bucket = function(pO_projectRef, ps_BucketName, pc_serializedEncryptionAccessPtr) {
    
    return new Promise(function(resolve, reject) {
        // prepare the input for the function, by converting it into char*
        var bucketNamePtr = ref.allocCString(ps_BucketName);
        //
        // Define char* type local variable, to store error when a function is called
        var errorPtrPtr = ref.NULL.ref();
        //
        //  by calling the exported golang function
        var bucketRef = libUplink.open_bucket(pO_projectRef, bucketNamePtr, pc_serializedEncryptionAccessPtr, errorPtrPtr);
        bucketRef.type = BucketRef;
        //
        // if handle is received
        if (bucketRef._handle > 0) {
            resolve(bucketRef);
        } else {
            var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
            //
            errorPtrPtr = null;
            //
            reject(ls_error);
        }
    });
};

// function to close currently open Bucket
// pre-requisites: open_bucket() function has been already called
// inputs: valid BucketRef
// output: None, else error in catch
let close_bucket = function(pO_bucketRef) {
    
    return new Promise(function(resolve, reject) {
        // Define char* type local variable, to store error when a function is called
        var errorPtrPtr = ref.NULL.ref();  
        //
        //  by calling the exported golang function
        libUplink.close_bucket(pO_bucketRef, errorPtrPtr);
        //
        // check for any possible error after closing the uplink
        if (errorPtrPtr.deref().isNull() == false) {
            var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
            //
            errorPtrPtr = null;
            //
            reject(ls_error);
        } else {
            resolve(true);
        }
    });
};

// function to fetch a list of buckets, available in the Storj project
// pre-requisites: open_project() function has been already called
// inputs: valid ProjectRef
// output: BucketList, else error in catch
let list_objects = function(pO_bucketRef) {

    return new Promise(function(resolve, reject) {
        // Define char* type local variable, to store error when a function is called
        var errorPtrPtr = ref.NULL.ref();   
        //
        // get objects' list, by calling the exported golang function
        var objectList = libUplink.list_objects(pO_bucketRef, null, errorPtrPtr);
        //
        // check for any possible error while listing objects
        if (errorPtrPtr.deref().isNull() == false) {
            var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
            //
            errorPtrPtr = null;
            //
            reject(ls_error);
        } else {
            // parse the list, as per the defined structure
            objectList.type = ObjectList;
            //
            var objectsInfo = [];
            //
            // fetch information of each object in the list
            for (var i = 0; i < objectList.length; i++) {
                var offset = i * ObjectInfoPtr.size;
                //
                var verBuck = parseUInt64To32(ref.get(objectList.items, offset, "uint64"));
                //
                var objectInfo = {
                    version: verBuck.hi32,
                    // bucket: ref.get(objectList.items, offset + ref.types.uint64.size, BucketInfo),
                    path: ref.get(objectList.items, offset + ref.types.uint64.size + BucketInfo.size, string_t),
                    is_prefix: ref.get(objectList.items, offset + ref.types.uint64.size + BucketInfo.size + string_t.size, "uint64"),
                    content_type: ref.get(objectList.items, offset + ref.types.uint64.size + BucketInfo.size + ref.types.uint64.size + string_t.size, string_t),
                    size: ref.get(objectList.items, offset + ref.types.uint64.size + BucketInfo.size + ref.types.uint64.size + (string_t.size * 2), "int64"),
                    created: ref.get(objectList.items, offset + ref.types.uint64.size + BucketInfo.size + ref.types.uint64.size + (string_t.size * 2) + ref.types.uint64.size, "int64"),
                    modified: ref.get(objectList.items, offset + ref.types.uint64.size + BucketInfo.size + ref.types.uint64.size + (string_t.size * 2) + (ref.types.uint64.size * 2), "int64"),
                    expires: ref.get(objectList.items, offset + ref.types.uint64.size + BucketInfo.size + ref.types.uint64.size + (string_t.size * 2) + (ref.types.uint64.size * 3), "int64")
                };
                //
                //
                objectsInfo.push(objectInfo);
            }
            // recreate a objects' list, as a JavaScript object
            var objectsList = {
                bucket: objectList.bucket,
                prefix: objectList.prefix,
                more  : objectList.more,
                items : objectsInfo,
                length: objectList.length
            };
            //
            // free the received objects' list, by calling the exported golang function
            libUplink.free_list_objects(objectList.ref());
            //
            resolve(objectsList);
        }
    });
}


// function to get uploader reference used to upload data to Storj (V3) bucket's path
// pre-requisites: open_bucket() function has been already called
// inputs: Bucket Reference (BucketRef), Storj Path/File Name (string) within the opened bucket
// output: Uploader reference (uploaderRef), else Error (string) in catch
let upload = function(pO_bucketRef, ps_uploadPathStorj) {
       
    return new Promise(function(resolve, reject) {  
        // prepare to upload the file
        var uploadOptions = ref.allocCString(ref.NULL);
        //
        // Define char* type local variable, to store error when a function is called
        var errorPtrPtr = ref.NULL.ref();
        //
        var uploaderRef = libUplink.upload(pO_bucketRef, ref.allocCString(ps_uploadPathStorj), uploadOptions, errorPtrPtr);
        uploaderRef.type = UploaderRef; 
        //
        // check for any possible error after closing the uplink
        if (errorPtrPtr.deref().isNull() == false) {
            var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
            //
            errorPtrPtr = null;
            //
            reject(ls_error);
        } else {
            resolve(uploaderRef);
        }
    });
};
//
// function to write data to Storj (V3) bucket's path
// pre-requisites: upload() function has been already called
// inputs: Uploader reference (uploaderRef), Data to upload (buffer), Size of data in bytes to be uploaded (size_t)
// output: { size: Size of data uploaded (size_t), error: Error (string) }
// **Note:** The Data to upload (buffer) passed to function should be a uint8 pointer only. 
let upload_write = function(uploaderRef, buffer, bytesRead) {
         
    // Define char* type local variable, to store error when a function is called
    var errorPtrPtr = ref.NULL.ref();
    //
    bytesRead.type = ref.types.size_t;
    //
    // write them to the buffer
    var actuallyWritten = libUplink.upload_write(uploaderRef, buffer, bytesRead, errorPtrPtr);
    //
    return {
        size: actuallyWritten,
        error: (errorPtrPtr.deref().isNull() == false) ? ref.readCString(errorPtrPtr.deref(), 0) : ""
    };
};
//
// function to commit and finalize data to be uploaded to Storj (V3) bucket's path
// pre-requisites: upload() function has been already called
// inputs: Uploader reference (uploaderRef)
// output: None, else error (string) in catch
let upload_commit = function(uploaderRef) {
       
    return new Promise(function(resolve, reject) { 

        // Define char* type local variable, to store error when a function is called
        var errorPtrPtr = ref.NULL.ref();
        //
        libUplink.upload_commit(uploaderRef, errorPtrPtr);
        //
        if (errorPtrPtr.deref().isNull() == false) {
            var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
            //
            errorPtrPtr = null;
            //
            reject(ls_error);
        } else {
            resolve(true);
        }
    });
};

// function to get downloader reference to download Storj (V3) object's data and store it on local computer
// pre-requisites: open_bucket() function has been already called
// inputs: Bucket reference (BucketRef), Storj Path/File Name (string) within the opened bucket
// output: Downloader reference (downloaderRef), else Error (string) in catch
let download = function(pO_bucketRef, ps_downloadPathStorj) {
       
    return new Promise(function(resolve, reject) {
        // check if the given path exists in the Storj bucket
        list_objects(pO_bucketRef)
        .then(function (objectsList) {
            var lb_objectFound = false;
            //
            for (let i = (objectsList.length - 1); i > -1; i--) {
                if (ps_downloadPathStorj == objectsList.items[i].path) {
                    lb_objectFound = true;

                    // prepare to download the file
                    var downloadOptions = ref.allocCString(ref.NULL);
                    //
                    // Define char* type local variable, to store error when a function is called
                    var errorPtrPtr = ref.NULL.ref();
                    //
                    var downloaderRef = libUplink.download(pO_bucketRef, ref.allocCString(ps_downloadPathStorj), downloadOptions, errorPtrPtr);
                    downloaderRef.type = DownloaderRef;
                    //
                    // check for any possible error after closing the uplink
                    if (errorPtrPtr.deref().isNull() == false) {
                        var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
                        //
                        errorPtrPtr = null;
                        //
                        reject(ls_error);
                    } else {
                        resolve(downloaderRef);
                    }

                    break;
                }
            }
            //
            if (lb_objectFound == false) {
                reject("object not found in the Storj bucket");
            }    
        })
        .catch(function (error) {
            reject(error);
        });
    });
};
//
// function to read Storj (V3) object's data into given buffer, as per given size
// pre-requisites: download() function has been already called
// inputs: Downloader Reference (downloaderRef), buffer, Length of data to download (size_t)
// output: { size: Size of data downloaded (size_t), error: Error (string) }
// given buffer will contain the downloaded data
let download_read = function(downloaderRef, buffer, BUFFER_SIZE) {
        
    // Define char* type local variable, to store error when a function is called
    var errorPtrPtr = ref.NULL.ref();
    //
    // write them to the buffer
    var downloadedSize = libUplink.download_read(downloaderRef, buffer, BUFFER_SIZE, errorPtrPtr);
    //
    return {
        size: downloadedSize,
        error: (errorPtrPtr.deref().isNull() == false) ? ref.readCString(errorPtrPtr.deref(), 0) : ""
    };
};
//
// function to close downloader after completing the data read process
// pre-requisites: download() function has been already called
// inputs: Downloader Reference (downloaderRef)
// output: None, else Error (string) in catch
let download_close = function(downloaderRef) {
       
    return new Promise(function(resolve, reject) { 

        // Define char* type local variable, to store error when a function is called
        var errorPtrPtr = ref.NULL.ref();
        //
        libUplink.download_close(downloaderRef, errorPtrPtr);
        //
        if (errorPtrPtr.deref().isNull() == false) {
            var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
            //
            errorPtrPtr = null;
            //
            reject(ls_error);
        } else {
            resolve(true);
        }
    });
};


// function to delete an object in a bucket
// pre-requisites: open_bucket() function has been already called, successfully
// inputs: Bucket (BucketRef), Object Path (string)
// output: None, else Error (string) in catch
let delete_object = function(pO_bucketRef, ps_objectPath) {
       
    return new Promise(function(resolve, reject) { 
        // Define char* type local variable, to store error when a function is called
        var errorPtrPtr = ref.NULL.ref();

        libUplink.delete_object(pO_bucketRef, ref.allocCString(ps_objectPath), errorPtrPtr);
        //
        if (errorPtrPtr.deref().isNull() == false) {
            var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
            //
            errorPtrPtr = null;
            //
            reject(ls_error);
        } else {
            resolve(true);
        }
   });    
};


/* SAMPLE USAGE OF UPLOAD and DOWNLOAD FUNCTIONS */
const BUFFER_SIZE = 8000;
const RETRY_MAX = 5;

// function to upload data from a file at local system to a Storj (V3) bucket's path
// pre-requisites: open_bucket() function has been already called
// inputs: valid BucketRef, Storj Path/File Name (string), Source Full File Name (string)
// output: None, else error (string) in catch
let upload_file = function(pO_bucketRef, ps_uploadPathStorj, ps_uploadPathLocal) {
    
    return new Promise(function(resolve, reject) { 
        // prepare to upload the file
        var uploadOptions = ref.allocCString(ref.NULL);
        //
        // Define char* type local variable, to store error when a function is called
        var errorPtrPtr = ref.NULL.ref();
        //
        var uploaderRef = libUplink.upload(pO_bucketRef, ref.allocCString(ps_uploadPathStorj), uploadOptions, errorPtrPtr);
        uploaderRef.type = UploaderRef;
        //
        var ls_error = "";
        //
        // ensure there was no error
        if (errorPtrPtr.deref().isNull() == false) {
            ls_error = ref.readCString(errorPtrPtr.deref(), 0);
            //
            errorPtrPtr = null;
            //
            reject(ls_error);
        } else {
            // open desired local file and get its size
            var fileHandle = fs.openSync(ps_uploadPathLocal, "rs");
            //
            var size = {
                file            : `${fs.statSync(ps_uploadPathLocal).size}`,
                toWrite         : 0,
                actuallyWritten : 0,
                totalWritten    : 0
            };
            
            var li_retryCount = RETRY_MAX;

            // set-up buffer
            var buffer = new Buffer.alloc(BUFFER_SIZE);
            buffer.type = uint8Ptr_t;
            //
            // fill-up the buffer
            while (size.totalWritten < size.file) { 
                size.toWrite = size.file - size.totalWritten;
                //
                if (size.toWrite > BUFFER_SIZE) {
                    size.toWrite = BUFFER_SIZE;
                } else if (size.toWrite == 0) {
                    break;
                }

                do {
                    // read proper number of bytes from the file
                    var bytesRead = fs.readSync(fileHandle, buffer, 0, size.toWrite, size.totalWritten);
                    //
                } while ((bytesRead <= 0) && (--li_retryCount > 0));
                //
                if (bytesRead <= 0) {
                    // could not read further
                    ls_error = "ERROR: Read " + size.totalWritten.toString() + " bytes of complete " + size.file.toString() + " bytes from the file. Could not read further.";
                    //
                    break;
                }
                
                if (li_retryCount < RETRY_MAX) {
                    li_retryCount = RETRY_MAX;
                }
                //
                bytesRead.type = ref.types.size_t;
                
                do {
                    // write them to the buffer
                    size.actuallyWritten = libUplink.upload_write(uploaderRef, buffer, bytesRead, errorPtrPtr);
                    //
                    if (errorPtrPtr.deref().isNull() == false) {
                        ls_error = ref.readCString(errorPtrPtr.deref(), 0);
                    }                    
                    //
                } while ((errorPtrPtr.deref().isNull() == false) && (--li_retryCount > 0));
                //
                if (errorPtrPtr.deref().isNull() == false) {
                    break;
                }
                
                if (li_retryCount < RETRY_MAX) {
                    li_retryCount = RETRY_MAX;
                }
                
                if (size.actuallyWritten == 0) {
                    break;
                }
                //
                size.totalWritten += size.actuallyWritten;
            }

            // close the opened local file
            fs.closeSync(fileHandle);

            var ls_dataUploaded = "\n " + ((size.totalWritten * 100.0)/size.file).toString() + "% uploaded!";
            if (li_retryCount <= 0) {
                errorPtrPtr = null;
                //
                reject(ls_error + ls_dataUploaded);
            } else if (li_retryCount > 0) {
                // upload to Storj bucket
                libUplink.upload_commit(uploaderRef, errorPtrPtr);
                //
                if (errorPtrPtr.deref().isNull() == false) {
                    ls_error = ref.readCString(errorPtrPtr.deref(), 0);
                    //
                    errorPtrPtr = null;
                    //
                    reject(ls_error + ls_dataUploaded);
                } else {
                    resolve(true);
                }
            }
        }
    });
};

// function to download Storj (V3) object's data and store it into given file at local system
// pre-requisites: open_bucket() function has been already called
// inputs: valid BucketRef, Storj Path/File Name (string), Destination Full File Name (string)
// output: None, else error in catch
let download_file = function(pO_bucketRef, ps_downloadPathStorj, ps_downloadPathLocal) {
    return new Promise(function(resolve, reject) {
        // prepare to download desired Storj object
        var downloadOptions = ref.allocCString(ref.NULL);
        //
        // Define char* type local variable, to store error when a function is called
        var errorPtrPtr = ref.NULL.ref();
        //
        // prepare to upload desired Storj object
        var downloaderRef = libUplink.download(pO_bucketRef, ref.allocCString(ps_downloadPathStorj), downloadOptions, errorPtrPtr);
        downloaderRef.type = DownloaderRef;
        //
        var ls_error = "";
        //
        // ensure there was no error
        if (errorPtrPtr.deref().isNull() == false) {
            ls_error = ref.readCString(errorPtrPtr.deref(), 0);
            //
            errorPtrPtr = null;
            //
            reject(ls_error);
        } else {
            var size = {
                downloaded      : 0,
                actuallyWritten : 0,
                totalWritten    : 0
            };
    
            var li_retryCount = RETRY_MAX;
            
            // set-up buffer
            var buffer = new Buffer.alloc(BUFFER_SIZE);
            buffer.type = uint8Ptr_t;
    
            // create/open file, to be written to with downloaded data
            var fileHandle = fs.openSync(ps_downloadPathLocal, "w");
             
            while(true) {
                do {
                    // download a part of Storj object, as per the buffer size   
                    size.downloaded = libUplink.download_read(downloaderRef, buffer, BUFFER_SIZE, errorPtrPtr);
                    //
                    if (errorPtrPtr.deref().isNull() == false) {
                        ls_error = ref.readCString(errorPtrPtr.deref(), 0);
                    }
                    //
                } while ((errorPtrPtr.deref().isNull() == false) && (--li_retryCount > 0));
                //
                if (errorPtrPtr.deref().isNull() == false) {
                    break;
                }
                
                if (li_retryCount < RETRY_MAX) {
                    li_retryCount = RETRY_MAX;
                }
                
                if (size.downloaded == 0) {
                    break;
                }
    
                do {
                    // write the downloaded stuff to the file
                    size.actuallyWritten = fs.writeSync(fileHandle, buffer, 0, size.downloaded, size.totalWritten);
                    //
                } while ((size.downloaded != size.actuallyWritten) && (--li_retryCount > 0));
                //
                size.totalWritten += size.actuallyWritten;
                //
                if (size.downloaded != size.actuallyWritten) {
                    // could not write all downloaded bytes
                    ls_error = "FAILed to all downloaded bytes to the file. Downloaded and written " + size.totalWritten.toString() + " bytes.";
                    //
                    break;
                }
                //
                if (li_retryCount < RETRY_MAX) {
                    li_retryCount = RETRY_MAX;
                }
            }
    
            // close the opened file
            fs.closeSync(fileHandle);

            // close download set-up
            libUplink.download_close(downloaderRef, errorPtrPtr);
            //
            if (errorPtrPtr.deref().isNull() == false) {
                ls_error += "\n" + ref.readCString(errorPtrPtr.deref(), 0);
            }
            
            if (ls_error != "") {
                //
                errorPtrPtr = null;
                //
                reject(ls_error);
            } else {
                resolve(true);
            }
        }
    });
};


// export the functions as part of Node.js module
module.exports = {
    new_uplink    : new_uplink,
    close_uplink  : close_uplink,

    parse_api_key : parse_api_key,
    open_project  : open_project,
    close_project : close_project,

    
    list_buckets  : list_buckets,

    create_bucket : create_bucket,
    delete_bucket : delete_bucket,
    
    encryption_key: encryption_key,
    open_bucket   : open_bucket,
    close_bucket  : close_bucket,

    
    list_objects  : list_objects,

    upload        : upload,
    upload_write  : upload_write,
    upload_commit : upload_commit,

    download      : download,
    download_read : download_read,
    download_close: download_close,

    delete_object : delete_object
}
