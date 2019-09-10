
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


// Define required datatypes to communicate with C functions
const stringPtrPtr = ref.refType(ref.types.CString);

// Define char* type global variable, to store error when a function is called
var gs_error = new Buffer.alloc(100);
gs_error.type = ref.types.char;


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


/* 
 * Include the golang exported libuplinkc library functions, and
 * declare types of arguments and response of each of these functions
 */
var libUplink = ffi.Library("./libuplinkc.so",
    {
    	uplink_custom		  : [ UplinkRef , [ UplinkConfig, stringPtrPtr ] ] ,
    	parse_api_key		  : [ APIKeyRef, [ "char*" , stringPtrPtr ] ],
    	open_project		  : [ ProjectRef, [ UplinkRef, "char*", APIKeyRef, stringPtrPtr ] ],
        encryption_key_custom : [ EncryptionAccessRef ,[ ProjectRef,"char*", stringPtrPtr ] ],
        open_bucket_custom	  : [ BucketRef, [ ProjectRef, "char*", EncryptionAccessRef, stringPtrPtr ] ],
        upload_custom		  : [ "void" , [ BucketRef, "char*", "char*", stringPtrPtr ] ],
        download_custom		  : [ "void" , [ BucketRef, "char*", "char*", stringPtrPtr ] ],
        close_uplink		  : [ "void" , [ UplinkRef, stringPtrPtr ] ],
        close_project		  : [ "void" , [ ProjectRef, stringPtrPtr ] ],
        close_bucket 		  : [ "void" , [ BucketRef, stringPtrPtr ] ],
        create_bucket_custom  : [ "void" , [ ProjectRef, "char*", stringPtrPtr ] ] 
    });


/*
 * Declare Node.js binding methods for required libuplink golang functions
 */


// function to create new Storj uplink
// pre-requisites: none
// inputs: none
// output: uplinkRef with a long type _handle property
// NOTE: output is correct, if its _handle poperty is a positive integer
function new_uplink() {
	console.log("\nSetting-up New Uplink...");
	//
	// set-up uplink configuration
	lO_uplinkConfig = new UplinkConfig();
	lO_uplinkConfig.Volatile.tls.skip_peer_ca_whitelist = true;
	//
	// create new uplink by calling the exported golang function
	var uplinkRef = libUplink.uplink_custom(lO_uplinkConfig, gs_error.ref());
	uplinkRef.type = UplinkRef;
	//
	// if error occurred
	if (uplinkRef._handle <= 0) {
		console.error("FAILed to set-up new uplink!\n");
	}
	//
	return uplinkRef;
}

// function to parse API key, to be used by Storj
// pre-requisites: None
// inputs: API key (string)
// output: APIKeyRef
// NOTE: output is correct, if its _handle poperty is a positive integer
function parse_api_key(ps_apiKey) {
	console.log("\nParsing the API Key: ", ps_apiKey);
	//
	// prepare the input for the function, by converting it into char*
	var lc_apiKeyPtr = ref.allocCString(ps_apiKey);
	//
	// parse the API key by calling the exported golang function
	var parsedAPIKeyRef = libUplink.parse_api_key(lc_apiKeyPtr, gs_error.ref());
	parsedAPIKeyRef.type = APIKeyRef;
	//
	// if error occurred
	if (parsedAPIKeyRef._handle <= 0) {
		console.error("FAILed to parse the API!\n")
	}
	//
	return parsedAPIKeyRef;
}

//function to open a Storj project
// pre-requisites: new_uplink() and parse_api_key() functions have been already called
// inputs: UplinkRef , Satellite Address (string) ,APIKeyRef
// output: ProjectRef
// NOTE: output is correct, if its _handle poperty is a positive integer
function open_project(pO_uplinkRef, ps_satellite, pO_apiKeyRef) {
	console.log("\nOpening project from satellite: ", ps_satellite);
	//
	// prepare the input for the function, by converting it into char*
	var lc_satellitePtr = ref.allocCString(ps_satellite);
	//
	// open project by calling the exported golang function
	var projectRef = libUplink.open_project(pO_uplinkRef, lc_satellitePtr, pO_apiKeyRef, gs_error.ref());
	projectRef.type = ProjectRef;
	//
	// if error occurred
	if (projectRef._handle <= 0) {
		console.error("FAILed to open the project!\n")
	}
	//
	return projectRef;
}

//function to get encryption access to upload and download data on Storj
// pre-requisites: open_project() function has been already called
// inputs: ProjectRef, Encryption Pass Phrase (string) ,
// output: EncryptionAccessRef
// NOTE: output is correct, if its _handle poperty is a positive integer
function encryption_key_custom(pO_projectRef,ps_encryptionpassphrase) {
        console.log("\nCreating Encryption key...");
        //
        // prepare the input for the function, by converting it into char*
        var encryptionpassphrasePtr = ref.allocCString(ps_encryptionpassphrase);
        //
        //  by calling the exported golang function
        var encryptionAccessRef = libUplink.encryption_key_custom(pO_projectRef,encryptionpassphrasePtr,gs_error.ref() );
        encryptionAccessRef.type = EncryptionAccessRef;
        //
		// if error occurred
		if (encryptionAccessRef._handle <= 0) {
			console.error("FAILed to parse the encryption pass phrase!\n")
		}
		//
		return encryptionAccessRef;
} 

// function to open an already existing bucket in Storj project
// pre-requisites: get_encryption_access() function has been already called
// inputs: ProjectRef , Bucket Name (string) , EncryptionAccessRef
// output: bucketRef
// NOTE: output is correct, if its _handle poperty is a positive integer
 function open_bucket_custom(pO_ProjectRef,ps_BucketName,pO_Encryption_AccessRef) {
        console.log("\nOpening '", ps_BucketName,"' bucket...");
        //
        // prepare the input for the function, by converting it into char*
        var bucketNamePtr = ref.allocCString(ps_BucketName);
        //
        //  by calling the exported golang function
        var bucketRef = libUplink.open_bucket_custom(pO_ProjectRef,bucketNamePtr,pO_Encryption_AccessRef,gs_error.ref());
        bucketRef.type = BucketRef;
        //
		// if error occurred
		if (bucketRef._handle <= 0) {
			console.error("FAILed to open the bucket!\n")
		}
		//
		return bucketRef;
}

//function to upload data from srcFullFileName (at local computer) to Storj (V3) bucket's path
// pre-requisites: open_bucket() function has been already called
// inputs: Storj Path/File Name (string) , Source Full File Name(string)
// output: None
 function upload_custom(pO_bucketRef, ps_uploadPath, ps_uploadPathFull ) {
        console.log("\nUploading Data...");
        //
        // prepare the input for the function, by converting it into char*
        uploadPathPtr = ref.allocCString( ps_uploadPath );
        srcFullFileNamePtr = ref.allocCString( ps_uploadPathFull ); 
        //
        //  by calling the exported golang function
        libUplink.upload_custom(pO_bucketRef, uploadPathPtr, srcFullFileNamePtr, gs_error.ref() );

}

// function to download Storj (V3) object's data and store it in given file with destFullFileName (on local computer)
// pre-requisites: open_bucket() function has been already called
// inputs: bucketRef , Storj Path/File Name (string) , Destination Full File Name(string)
// output: None
 function download_custom(pO_bucketRef, ps_downloadPath, ps_downloadPathFull ) {
        console.log("\nDownloading Data...");
        //
        // prepare the input for the function, by converting it into char*
        downloadPathPtr = ref.allocCString( ps_downloadPath );
        srcFullFileNamePtr = ref.allocCString( ps_downloadPathFull ); 
        // 
        //  by calling the exported golang function
        libUplink.download_custom(pO_bucketRef, downloadPathPtr, srcFullFileNamePtr, gs_error.ref() );
}

// function to create new bucket in Storj project
// pre-requisites: open_project() function has been already called
// inputs: bucketRef , Bucket Name (string)
// output: None
function create_bucket_custom( pO_projectRef, ps_bucketName ) {
        console.log("\nCreating '", ps_bucketName, "' bucket...");
        //
        // prepare the input for the function, by converting it into char*
        bucketNamePtr = ref.allocCString( ps_bucketName );
        //
        //  by calling the exported golang function
        libUplink.create_bucket_custom(pO_projectRef, bucketNamePtr, gs_error.ref() );
}

// function to close currently opened uplink
// pre-requisites: new_uplink() function has been already called
// inputs: UplinkRef
// output: None
function close_uplink( p_UplinkRef ) {
       console.log("\nClosing the established uplink.");
        //
        //  by calling the exported golang function
        libUplink.close_uplink(p_UplinkRef, gs_error.ref() );
}

// function to close currently opened Storj project
// pre-requisites: open_project() function has been already called
// inputs: ProjectRef
// output: None 
function close_project(p_projectRef) {
         console.log("\nClosing the opened project...");
        //
        //  by calling the exported golang function
        libUplink.close_project(p_projectRef, gs_error.ref() );       
}

// function to close currently opened Bucket
// pre-requisites: open_bucket() function has been already called
// inputs: bucketRef
// output: None
function close_bucket(p_bucketRef) {
         console.log("\nClosing the opened bucket...");
        //
        //  by calling the exported golang function
        libUplink.close_bucket(p_bucketRef, gs_error.ref() );       
}


// export the functions as part of Node.js module
module.exports = {
    new_uplink              : new_uplink,
    parse_api_key           : parse_api_key,
    open_project            : open_project,
    encryption_key_custom   : encryption_key_custom,
    open_bucket_custom      : open_bucket_custom,
    upload_custom           : upload_custom,
    download_custom         : download_custom,
    create_bucket_custom    : create_bucket_custom,
    close_uplink            : close_uplink,
    close_project           : close_project,
    close_bucket            : close_bucket
}
