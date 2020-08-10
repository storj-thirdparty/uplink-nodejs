const process = require("process");
var uplink = require("bindings")("uplink");
const AccessResultStruct = require('./access.js');
const errorhandle = require('./error.js');
process.chdir(__dirname);

/*eslint-disable */
class ListBucketsOptions {
    constructor(cursor="") {
        this.cursor = cursor;
    }
}

class Permission {
    constructor(allow_download=false,allow_upload=false,allow_list=false,allow_delete=false,not_before=0,not_after=0) {
        this.allow_download=allow_download;
        this.allow_upload=allow_upload;
        this.allow_list=allow_list;
        this.allow_delete=allow_delete;
        this.not_before=not_before;
        this.not_after=not_after;
    }
}

class UploadOptions {
    constructor(expires=0) {
        this.expires = expires;
    }
}

class DownloadOptions{
    constructor(offset = 0,length = 0) {
    this.offset = offset;
    this.length = length; 
    }
}

class ListObjectsOptions {
    constructor(prefix="",cursor="",recursive=false,system=false,custom=false) {
        this.prefix=prefix;
        this.cursor=cursor;
        this.recursive=recursive;
        this.system=system;
        this.custom=custom;
    }
}

class SharePrefix {
    constructor(bucket="",prefix="") {
        this.bucket=bucket;
        this.prefix=prefix;
    }    
}

class CustomMetadata {
    constructor(entries=[],count=0) {
        this.entries = entries;
        this.count = count;
    }
}

class CustomMetadataEntry {
    constructor(key="", key_length=0,value=0,value_length=0) {
        this.key=key;
        this.key_length=key_length;
        this.value=value;
        this.value_length=value_length;    
    }
}

class Config {
    constructor(user_agent="", dial_timeout_milliseconds=0,temp_directory="") {
        this.user_agent=user_agent;
        this.dial_timeout_milliseconds=dial_timeout_milliseconds;
        this.temp_directory=temp_directory;
    }
}

class Uplink {
    // request_access_with_passphrase generates a new access grant using a passhprase.
    // It must talk to the Satellite provided to get a project-based salt for deterministic
    // key derivation.
    // Note: this is a CPU-heavy function that uses a password-based key derivation
    // function (Argon2). This should be a setup-only step.
    // Most common interactions with the library should be using a serialized access grant
    // through ParseAccess directly.
    // Input : Satellite Address (String) , API key (String) , Encryption phassphrase(String)
    // Output : Access (Object)
    async requestAccessWithPassphrase(satelliteURL,apiKey,encryptionPassphrase){
        var access = await uplink.request_access_with_passphrase(satelliteURL,apiKey,encryptionPassphrase).catch((error) => {
            errorhandle.storjException(error.error.code,error.error.message);
        });
        var accessResultReturn = new AccessResultStruct(access.access);
        return(accessResultReturn);
    }

    // ParseAccess parses a serialized access grant string.
    // This should be the main way to instantiate an access grant for opening a project.
    // See the note on RequestAccessWithPassphrase
    // Input : Shared string
    // Output : Access (Object)
    async parseAccess(stringResult){
        var parsedSharedAccess = await uplink.parse_access(stringResult).catch((error) => {
            errorhandle.storjException(error.error.code,error.error.message);
        });
        var accessResultReturn = new AccessResultStruct(parsedSharedAccess.access);
        return(accessResultReturn);
    }

    // RequestAccessWithPassphrase generates a new access grant using a passhprase and custom configuration.
    // It must talk to the Satellite provided to get a project-based salt for deterministic key derivation.
    // Note: this is a CPU-heavy function that uses a password-based key derivation function (Argon2). This should be a setup-only step.
    // Most common interactions with the library should be using a serialized access grant
    // hrough ParseAccess directly.
    // Input : Config (Object) , Satellite Address (String) , API key (String) , Encryption phassphrase(String)
    // Output : Access (Object)
    async configRequestAccessWithPassphrase(config,satelliteURL,apiKey,encryptionPassphrase){
        var access = await uplink.config_request_access_with_passphrase(config,satelliteURL,apiKey,encryptionPassphrase).catch((error) => {
            errorhandle.storjException(error.error.code,error.error.message);
        });
        var accessResultReturn = new AccessResultStruct(access.access);
        return(accessResultReturn);
    }
}
/*eslint-enable */
//exporting function and object
module.exports = {
    Uplink ,
    DownloadOptions,
    ListBucketsOptions,
    Permission,
    UploadOptions,
    ListObjectsOptions,
    CustomMetadataEntry,
    CustomMetadata,
    SharePrefix,
    Config,
    errorhandle
};