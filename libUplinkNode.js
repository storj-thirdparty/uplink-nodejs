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


const BUFFER_SIZE = 256;
const RETRY_MAX = 5;

/* Define required datatypes to communicate with C functions */
// uint8*
const uint8Ptr_t = ref.refType(ref.types.uint8);
//
// char*
const charPtr_t = ref.refType(ref.types.char);
//
// char**
const charPtrPtr_t = ref.refType(charPtr_t);  // ref.types.CString


/* Define structures for golang structure objects */

// Uplink configuration nested structure
const tls = struct({
	skip_peer_ca_whitelist : "bool"
});

const Volatile = struct({
    tls : tls
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
    cipher_suite: "uint64",
    block_size  : "int32"
});

const RedundancyScheme = struct({
    algorithm      : "uint64",
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

const BucketConfigPtrType = ref.refType(BucketConfig);

const BucketInfo = struct ({
    name                 : charPtr_t,
    created              : "int64",
    path_cipher          : "uint64",
    segment_size         : "uint64",
    encryption_parameters: EncryptionParameters,
    redundancy_scheme    : RedundancyScheme
});


// to create pointer-to-pointer-to-char,
// to get error message from a C function
const NO_ERROR = "0";
//
function createErrorPtrPtr() {
    var lc_dummyErrorPtr = ref.allocCString(NO_ERROR);
    //
    return (lc_dummyErrorPtr.ref());
}


/* 
 * Include the golang exported libuplinkc library functions, and
 * declare types of arguments and response of each of these functions
 */
var libUplink = ffi.Library("./libuplinkc.so",
    {
    	new_uplink   : [ UplinkRef,  [ UplinkConfig,    charPtrPtr_t ] ],
    	close_uplink : [ "void" ,    [ UplinkRef,       charPtrPtr_t ] ],
        
        parse_api_key: [ APIKeyRef,  [ charPtr_t,       charPtrPtr_t ] ],
    	open_project : [ ProjectRef, [ UplinkRef,       charPtr_t,      APIKeyRef,              charPtrPtr_t ] ],
        close_project: [ "void" ,    [ ProjectRef,      charPtrPtr_t ] ],
        
        project_salted_key_from_passphrase    : [ uint8Ptr_t ,         [ ProjectRef,            charPtr_t,  charPtrPtr_t ] ],
        new_encryption_access_with_default_key: [ EncryptionAccessRef, [ uint8Ptr_t ] ],
        serialize_encryption_access           : [ charPtr_t,           [ EncryptionAccessRef,   charPtrPtr_t ] ],

        create_bucket: [ BucketInfo, [ ProjectRef,      charPtr_t,      BucketConfigPtrType,    charPtrPtr_t ] ],
        open_bucket	 : [ BucketRef,  [ ProjectRef,      charPtr_t,      charPtr_t,              charPtrPtr_t ] ],
        close_bucket : [ "void" ,    [ BucketRef,       charPtrPtr_t ] ],

        upload       : [ UploaderRef, [ BucketRef,      charPtr_t,      charPtr_t,              charPtrPtr_t ] ],
        upload_write : [ "size_t",    [ UploaderRef,    uint8Ptr_t,     "size_t",               charPtrPtr_t ] ],
        upload_commit: [ "void",      [ UploaderRef,    charPtrPtr_t ] ],
        
        download      : [ DownloaderRef, [ BucketRef,     charPtr_t,    charPtr_t,              charPtrPtr_t ] ],
        download_read : [ "size_t",      [ DownloaderRef, uint8Ptr_t,   "size_t",               charPtrPtr_t ] ]  ,
        download_close: [ "void",        [ DownloaderRef, charPtrPtr_t ] ]
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
        //
        // Define char* type local variable, to store error when a function is called
        var errorPtrPtr = createErrorPtrPtr();
        //
        // create new uplink by calling the exported golang function
        var uplinkRef = libUplink.new_uplink(lO_uplinkConfig, errorPtrPtr);
        uplinkRef.type = UplinkRef;
        //
        // if handle is received
        if (uplinkRef._handle > 0) {
            resolve(uplinkRef);
        } else {
            var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
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
        var errorPtrPtr = createErrorPtrPtr();     
        //
        //  by calling the exported golang function
        libUplink.close_uplink(pO_uplinkRef, errorPtrPtr);
        //
        // check for any possible error after closing the uplink
        var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
        //
        if (ls_error != NO_ERROR) {
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
        var errorPtrPtr = createErrorPtrPtr();
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
        var errorPtrPtr = createErrorPtrPtr();
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
        var errorPtrPtr = createErrorPtrPtr();   
        //
        //  by calling the exported golang function
        libUplink.close_project(pO_projectRef, errorPtrPtr);
        //
        // check for any possible error after closing the uplink
        var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
        //
        if (ls_error != NO_ERROR) {
            reject(ls_error);
        } else {
            resolve(true);
        }    
    });
};


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
        var errorPtrPtr = createErrorPtrPtr();   
        //
        // create a new by calling the exported golang function
        var bucketInfo = libUplink.create_bucket(pO_projectRef, bucketNamePtr, bucketOptions, errorPtrPtr);
        //
        // check for any possible error after creating the bucket
        var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
        //
        if (ls_error != NO_ERROR) {
            reject(ls_error);
        } else {
            resolve(bucketInfo);
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
        var errorPtrPtr = createErrorPtrPtr();
        //
        //  by calling the exported golang function
        var saltedKeyPtr = libUplink.project_salted_key_from_passphrase(pO_projectRef, encryptionPassphrasePtr, errorPtrPtr);
        //
        saltedKeyPtr.type = uint8Ptr_t;
        //
        // check for any possible error after download
        var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
        //
        if (ls_error != NO_ERROR) {
            reject(ls_error);
        } else {
            var encryptionAccessRef = libUplink.new_encryption_access_with_default_key(saltedKeyPtr);
            encryptionAccessRef.type = EncryptionAccessRef;
            //
            // if handle is received
            if (encryptionAccessRef._handle > 0) {
                var serializedEncryptionAccessPtr = libUplink.serialize_encryption_access(encryptionAccessRef, errorPtrPtr);
                serializedEncryptionAccessPtr.type = charPtr_t;
                //
                // check for any possible error after download
                ls_error = ref.readCString(errorPtrPtr.deref(), 0);
                //
                if (ls_error != NO_ERROR) {
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
        var errorPtrPtr = createErrorPtrPtr();
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
        var errorPtrPtr = createErrorPtrPtr();  
        //
        //  by calling the exported golang function
        libUplink.close_bucket(pO_bucketRef, errorPtrPtr);
        //
        // check for any possible error after closing the uplink
        var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
        //
        if (ls_error != NO_ERROR) {
            reject(ls_error);
        } else {
            resolve(true);
        }
    });
};


// function to upload data from a file at local system to a Storj (V3) bucket's path
// pre-requisites: open_bucket() function has been already called
// inputs: valid BucketRef, Storj Path/File Name (string), Source Full File Name (string)
// output: None, else error in catch
let upload_file = function(pO_bucketRef, ps_uploadPathStorj, ps_uploadPathLocal) {
    
    return new Promise(function(resolve, reject) { 
        // prepare to upload the file
        var uploadOptions = ref.allocCString(ref.NULL);
        //
        // Define char* type local variable, to store error when a function is called
        var errorPtrPtr = createErrorPtrPtr();
        //
        var uploaderRef = libUplink.upload(pO_bucketRef, ref.allocCString(ps_uploadPathStorj), uploadOptions, errorPtrPtr);
        uploaderRef.type = UploaderRef;
        //
        // ensure there was no error
        var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
        //
        if (ls_error != NO_ERROR) {
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
                    ls_error = ref.readCString(errorPtrPtr.deref(), 0);
                    //
                } while ((ls_error != NO_ERROR) && (--li_retryCount > 0));
                //
                if (ls_error != NO_ERROR) {
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
                reject(ls_error + ls_dataUploaded);
            } else if (li_retryCount > 0) {
                // upload to Storj bucket
                libUplink.upload_commit(uploaderRef, errorPtrPtr);
                //
                ls_error = ref.readCString(errorPtrPtr.deref(), 0);
                //
                if (ls_error != NO_ERROR) {
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
        var errorPtrPtr = createErrorPtrPtr();
        //
        // prepare to upload desired Storj object
        var downloaderRef = libUplink.download(pO_bucketRef, ref.allocCString(ps_downloadPathStorj), downloadOptions, errorPtrPtr);
        downloaderRef.type = DownloaderRef;
        //
        // ensure there was no error
        var ls_error = ref.readCString(errorPtrPtr.deref(), 0);
        //
        if (ls_error != NO_ERROR) {
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
                    ls_error = ref.readCString(errorPtrPtr.deref(), 0);
                    //
                } while ((ls_error != NO_ERROR) && (--li_retryCount > 0));
                //
                if (ls_error != NO_ERROR) {
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
            var ls_errorNew = ref.readCString(errorPtrPtr.deref(), 0);
            //
            if (ls_errorNew != NO_ERROR) {
                ls_error += "\n" + ls_errorNew;
            }
            
            if (ls_error != NO_ERROR) {
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

    create_bucket : create_bucket,
    encryption_key: encryption_key,
    open_bucket   : open_bucket,
    close_bucket  : close_bucket,

    upload_file   : upload_file,
    download_file : download_file
}
