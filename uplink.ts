/* eslint-disable */
import {AccessResultStruct} from "./access.js";

import bindings = require("bindings");
const uplink = bindings("uplink"),
    defaultValue = 0;


const errorhandle = require("./error.js");

class ListBucketsOptions {

    cursor: string;

    constructor (cursor = "") {

        this.cursor = cursor;

    }

}

class Permission {


    allow_download: boolean;

    allow_upload: boolean;

    allow_list: boolean;

    allow_delete: boolean;

    not_before: number;

    not_after: number;

    constructor (allow_download = false, allow_upload = false, allow_list = false, allow_delete = false, not_before = defaultValue, not_after = defaultValue) {

        this.allow_download = allow_download;
        this.allow_upload = allow_upload;
        this.allow_list = allow_list;
        this.allow_delete = allow_delete;
        this.not_before = not_before;
        this.not_after = not_after;

    }

}

class UploadOptions {

    expires: number;

    constructor (expires = defaultValue) {

        this.expires = expires;

    }

}

class DownloadOptions {

    offset: number;

    length: number;

    constructor (offset = defaultValue, length = defaultValue) {

        this.offset = offset;
        this.length = length;

    }

}

class ListObjectsOptions {

    prefix: string;

    cursor: string;

    recursive: boolean;

    system: boolean;

    custom: boolean;

    constructor (prefix = "", cursor = "", recursive = false, system = false, custom = false) {

        this.prefix = prefix;
        this.cursor = cursor;
        this.recursive = recursive;
        this.system = system;
        this.custom = custom;

    }

}

class SharePrefix {

    bucket: string;

    prefix: string;

    constructor (bucket = "", prefix = "") {

        this.bucket = bucket;
        this.prefix = prefix;

    }

}

class CustomMetadataEntry {

    key: string;

    key_length: number;

    value: string;

    value_length: number;

    constructor (key = "", key_length = defaultValue, value = "", value_length = defaultValue) {

        this.key = key;
        this.key_length = key_length;
        this.value = value;
        this.value_length = value_length;

    }

}

class CustomMetadata {

    entries: CustomMetadataEntry[];

    count: number;

    constructor (entries = [], count = defaultValue) {

        this.entries = entries;
        this.count = count;

    }

}

class Config {

    user_agent: string;

    dial_timeout_milliseconds: number;

    temp_directory: string;

    constructor (user_agent = "", dial_timeout_milliseconds = defaultValue, temp_directory = "") {

        this.user_agent = user_agent;
        this.dial_timeout_milliseconds = dial_timeout_milliseconds;
        this.temp_directory = temp_directory;

    }

}

class Uplink {

    /*
     * Request_access_with_passphrase generates a new access grant using a passhprase.
     * It must talk to the Satellite provided to get a project-based salt for deterministic
     * key derivation.
     * Note: this is a CPU-heavy function that uses a password-based key derivation
     * function (Argon2). This should be a setup-only step.
     * Most common interactions with the library should be using a serialized access grant
     * through ParseAccess directly.
     * Input : Satellite Address (String) , API key (String) , Encryption phassphrase(String)
     * Output : Access (Object)
     */
    async requestAccessWithPassphrase (satelliteURL: string, apiKey: string, encryptionPassphrase: string): Promise<AccessResultStruct> {

        const access = await uplink.request_access_with_passphrase(
                satelliteURL,
                apiKey,
                encryptionPassphrase
            ).catch((error: any) => {

                errorhandle.storjException(
                    error.error.code,
                    error.error.message
                );

            }),
            accessResultReturn = new AccessResultStruct(access.access);


        return accessResultReturn;

    }

    /*
     * ParseAccess parses a serialized access grant string.
     * This should be the main way to instantiate an access grant for opening a project.
     * See the note on RequestAccessWithPassphrase
     * Input : Shared string
     * Output : Access (Object)
     */
    async parseAccess (stringResult: string): Promise<AccessResultStruct> {

        const parsedSharedAccess = await uplink.parse_access(stringResult).catch((error: any) => {

                errorhandle.storjException(
                    error.error.code,
                    error.error.message
                );

            }),
            accessResultReturn = new AccessResultStruct(parsedSharedAccess.access);


        return accessResultReturn;

    }

    /*
     * RequestAccessWithPassphrase generates a new access grant using a passhprase and custom configuration.
     * It must talk to the Satellite provided to get a project-based salt for deterministic key derivation.
     * Note: this is a CPU-heavy function that uses a password-based key derivation function (Argon2). This should be a setup-only step.
     * Most common interactions with the library should be using a serialized access grant
     * hrough ParseAccess directly.
     * Input : Config (Object) , Satellite Address (String) , API key (String) , Encryption phassphrase(String)
     * Output : Access (Object)
     */
    async configRequestAccessWithPassphrase (config: Config, satelliteURL: string, apiKey: string, encryptionPassphrase: string): Promise<AccessResultStruct> {

        const access = await uplink.config_request_access_with_passphrase(
                config,
                satelliteURL,
                apiKey,
                encryptionPassphrase
            ).catch((error: any) => {

                errorhandle.storjException(
                    error.error.code,
                    error.error.message
                );

            }),
            accessResultReturn = new AccessResultStruct(access.access);


        return accessResultReturn;

    }


    /*
     * Input : Encryption phassphrase(String) , Array
     * Output : Output : Encryptio_Key (Object)
     */
    async uplinkDeriveEncryptionKey (phassphrase: string, salt: any): Promise<any> {

        const encryption = await uplink.derive_encryption_key(
            phassphrase,
            salt,
            salt.length
        ).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });


        return encryption;

    }

}

// Exporting function and object
export {
    Uplink,
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
/* eslint-enable */
