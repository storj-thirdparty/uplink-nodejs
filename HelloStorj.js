//
const fs = require("fs");

// include the Node.js-Storj bindings module
var storj = require("./node_modules/storj-nodejs/libUplinkNode.js");
// object for all the function for uplink
var libUplink = storj.storj_nodejs;
//
const BUFFER_SIZE = 8000;
//
const RETRY_MAX = 5;

// demo Storj (V3) configuration
var storjConfig = {
    apiKey    : "change-me-to-the-api-key-created-in-satellite-gui",
    satelliteURL   : "us-central-1.tardigrade.io:7777",
    encryptionPassphrase  : "you'll never guess this",
    bucketName   : "change-me-to-desired-bucket-name",
    uploadPath   : "optionalpath/requiredfilename",
};

// file names to be uploaded or downloaded as
var localFullFileName = {
    src : "change-me-to-source-file-name-at-local-system",
    dest: "change-me-to-destination-file-name-at-local-system",
};

//
var temp = "";
var lO_uplinkconfig = storj.UplinkConfig;
var lO_ScopeObject;
var lb_scopeRecieved = false;
//
console.log("Setting-up a New Uplink...");
var lO_uplinkRef = libUplink.new_uplinkc(lO_uplinkconfig,temp);
//
if(lO_uplinkRef.error == ""){
    console.log("New Uplink: SET-UP!");
    //
    console.log("\nParsing the API Key: ", storjConfig.apiKey);
    //
    var lO_APIkeyRef = libUplink.parse_api_keyc(storjConfig.apiKey);
    //
    if(lO_APIkeyRef.error == ""){
        console.log("API Key: PARSED!");
        //
        console.log("\nOpening the Storj Project from Satellite: ", storjConfig.satelliteURL);
        //
        var lO_Openproject = libUplink.open_projectc(lO_uplinkRef.data,storjConfig.satelliteURL,lO_APIkeyRef.data);
        
        if(lO_Openproject.error == ""){
            console.log("Desired Storj Project: OPENED!");
            
            console.log("\nCreating a new bucket with name, ", storjConfig.bucketName, "...");
            
            var lO_BucketConfig = storj.BucketConfig;

            lO_BucketConfig.path_cipher = 1;
            lO_BucketConfig.encryption_parameters.cipher_suite = 1;
            lO_BucketConfig.encryption_parameters.block_size=7424;
            lO_BucketConfig.redundancy_scheme.algorithm= 1;
            lO_BucketConfig.redundancy_scheme.share_size=256;
            lO_BucketConfig.redundancy_scheme.required_shares=29;
            lO_BucketConfig.redundancy_scheme.repair_shares=35;
            lO_BucketConfig.redundancy_scheme.optimal_shares=80;
            lO_BucketConfig.redundancy_scheme.total_shares=130;
            
            var lO_bucketInfo = libUplink.create_bucketc(lO_Openproject.data,storjConfig.bucketName,null);
            
            if(lO_bucketInfo.error==""){
                console.log("New Bucket: CREATED!");
            }else{
                console.error("FAILed to create a new bucket!");
                console.error(lO_bucketInfo.error);
            }
            
            console.log("\nListing buckets within the Storj project...");
            //TO DO : Bucket List Options
            var lO_listbucketconfig = storj.BucketListOptions;
            var lO_listBucket = libUplink.list_bucketsc(lO_Openproject.data,lO_listbucketconfig);
            //
            var bucketsList = lO_listBucket.data;
            if (bucketsList.length > 0) {
              console.log("S.No. \t Created On \t\t Bucket Name");
               console.log("===== \t ========== \t\t ===========");
            }

            var i=0;
            for(object in bucketsList.items){
              var num = "0" + (i + 1).toString();
              console.log(num.substr(-2), "  ", getDateTime(bucketsList.items[object].created), "\t", bucketsList.items[object].name);
              i++;
            }
            
            var serialaccess = libUplink.encryption_accessc(lO_Openproject.data,storjConfig.encryptionPassphrase);

            if(serialaccess.error == ""){
                console.log("Encryption Access: RECEIVED!");
                console.log("\nOpening ", storjConfig.bucketName, " Bucket...");
                var lO_OpenBucket = libUplink.open_bucketc(lO_Openproject.data,storjConfig.bucketName,serialaccess.data);
                    
                    if(lO_OpenBucket.error == ""){
                        console.log(storjConfig.bucketName, " Bucket: OPENED!");
                        
                        var uploadOption = storj.UploadOptions;
                        uploadOption.expires = 1679928565;
                        
                        console.log(localFullFileName.src, " File: UPLOADED as ", storjConfig.uploadPath, "!");

                        var lO_UploaderObject = libUplink.uploadc(lO_OpenBucket.data,storjConfig.uploadPath,uploadOption);
                        
                        var uploaderRef = lO_UploaderObject.data;
                        
                        var fileHandle = fs.openSync(localFullFileName.src, "rs");
                        
                        var size = {
                            file            : `${fs.statSync(localFullFileName.src).size}`,
                            toWrite         : 0,
                            actuallyWritten : 0,
                            totalWritten    : 0
                        };
                        
                        var li_retryCount = RETRY_MAX;
                            
                        var buffer = new Buffer.alloc(BUFFER_SIZE);                       
                            
                            while (size.totalWritten < size.file) { 
                                
                                size.toWrite = size.file - size.totalWritten;
                                
                                if (size.toWrite > BUFFER_SIZE) {
                                    size.toWrite = BUFFER_SIZE;
                                } else if (size.toWrite == 0) {
                                    break;
                                }

                                // read proper number of bytes from the file
                                var bytesRead = fs.readSync(fileHandle, buffer, 0, size.toWrite, size.totalWritten);
                                
                                while ((bytesRead <= 0) && (--li_retryCount > 0)) {
                                    //
                                    bytesRead = fs.readSync(fileHandle, buffer, 0, size.toWrite, size.totalWritten);
                                }
                                //
                                if (bytesRead <= 0) {
                                    // could not read further
                                    ls_error = "ERROR: Read " + size.totalWritten.toString() + " bytes of complete " + size.file.toString() + " bytes from the file. Could not read further.";
                                    break;
                                }
                                
                                if (li_retryCount < RETRY_MAX) {
                                    li_retryCount = RETRY_MAX;
                                }
                
                                var ls_error = "";               
                                //
                                do {
                                    // upload the buffer's content to the Storj bucket 
                                    var uploadedInfo = libUplink.upload_writec(uploaderRef, buffer, bytesRead);
                                    if (uploadedInfo.error != "") {
                                        ls_error = uploadedInfo.error;
                                    } else {
                                        size.actuallyWritten = uploadedInfo.data;
                                    }
                                } while ((ls_error != "") && (--li_retryCount > 0));
                                
                                if (ls_error != "") {
                                    break;
                                }
                                
                                if (li_retryCount < RETRY_MAX) {
                                    li_retryCount = RETRY_MAX;
                                }
                                
                                // if no further data is left to be read from the file and then uploaded
                                if (size.actuallyWritten == 0) {
                                    break;
                                }
                                //
                                size.totalWritten += size.actuallyWritten;
                                console.log("Data written on storj : ",size.totalWritten);
                            }
                
                            fs.closeSync(fileHandle);
                
                            var ls_dataUploaded = "\n " + ((size.totalWritten * 100.0)/size.file).toString() + "% uploaded!";
                            
                            console.log("Commiting Object on storj...");
                            var ls_uploadcommit = libUplink.upload_commitc(uploaderRef);
                            if(ls_uploadcommit==""){
                                console.log("Object Uploaded On storj");
                            }else{
                                console.log("FAILed to upload object on storj");
                                console.log(ls_uploadcommit);
                            }
                            var lO_listOptions = storj.ListOptions;
                            lO_listOptions.prefix = "uploadPath01/data";
                            lO_listOptions.cursor = "";
                            lO_listOptions.delimiter = "/";
                            lO_listOptions.direction = 2;
                            lO_listOptions.recursive = false;
                            lO_listOptions.limit = 0;
                            //
                            var listObject = libUplink.list_objectsc(lO_OpenBucket.data,lO_listOptions);
                            
                            var objectsList = listObject.data;
                            
                            console.log("\nListing objects within the Storj project's bucket...");

                            if (objectsList.length > 0) {
                                console.log("S.No. \t Created On \t Object Path");
                                console.log("===== \t ========== \t ===========");
                            }

                            var i=0;
                            for(object in objectsList.items){
                                var num = "0" + (i + 1).toString();
                                
                                console.log(num.substr(-2), "  ", getDateTime(objectsList.items[object].created), "\t", objectsList.items[object].prefix);
                                i++;
                            }
                            
                        
                            var dataread = new Buffer.alloc(BUFFER_SIZE);
                
                            
                            var objectsize;
                            console.log("Fetching object meta data from storj : ");
                            
                            var lO_ObjectMetaData = libUplink.open_objectc(lO_OpenBucket.data,storjConfig.uploadPath);
                            if(lO_ObjectMetaData.error==""){
                                var lO_ObjectMeta = libUplink.get_object_metac(lO_ObjectMetaData.data);
                                if(lO_ObjectMeta.error==""){
                                    console.log("Fetched Metadata of Object : ",storjConfig.uploadPath);
                                    objectsize = lO_ObjectMeta.data.size;
                                    
                                }else{
                                    console.log("FAILed to object meta data");
                                }
                                var closeObjectError = libUplink.close_objectc(lO_ObjectMetaData.data);
                                //
                            }else{
                                console.log("FAILed to open object on storj");
                                console.log(lO_ObjectMetaData.error);
                            }
                            //
                            console.log("\nDownloading ", storjConfig.uploadPath, " Storj Object as ", localFullFileName.dest, " file...");
                            var lO_downloadbucket = libUplink.downloadc(lO_OpenBucket.data,storjConfig.uploadPath);
                            var downloaderRef = lO_downloadbucket.data;
                            if(lO_downloadbucket.error == ""){
                                
                                size = {
                                    downloaded      : 0,
                                    actuallyWritten : 0,
                                    totalWritten    : 0
                                };

                                var li_retryCount = RETRY_MAX;
                                // set-up buffer
                                var buffer = new Buffer.alloc(BUFFER_SIZE);
                                // create/open file, to be written to with downloaded data
                                var fileHandle = fs.openSync(localFullFileName.dest, "w");
                                
                                while(true) {
                                    var ls_error = "";
                                    //    
                                    do {
                                        // download a part of Storj object, as per the buffer size   
                                        var downloadedInfo = libUplink.download_readc(downloaderRef, buffer, BUFFER_SIZE);
                                        if (downloadedInfo.error != "") {
                                            ls_error = downloadedInfo.error;
                                        } else {
                                            size.downloaded = downloadedInfo.data; 
                                        }
                                        
                                    } while((ls_error != "") && (--li_retryCount > 0));
                                    
                                    if (ls_error != "") {
                                        break;
                                    }
                                    
                                    if (li_retryCount < RETRY_MAX) {
                                        li_retryCount = RETRY_MAX;
                                    }
                                    
                                    
                                    if (size.totalWritten == objectsize) {
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
                                    console.log("Total File Download from the storj : ",size.totalWritten);
                                    //
                                    if (size.downloaded != size.actuallyWritten) {
                                        // could not write all downloaded bytes
                                        ls_error = "FAILed to write all downloaded bytes to the file. Downloaded and written " + size.totalWritten.toString() + " bytes.";
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
                                var downoloadclose = libUplink.download_closec(downloaderRef);
                                if(downoloadclose==""){
                                    console.log("Object Downloaded successfully");
                                }else{
                                    console.log("FAILed to download file");
                                    console.log(downoloadclose);
                                }
                              
                            }else{
                                console.log("FAILed to download Object from storj v3");
                                console.log(lO_downloadbucket.error);
                            }

                            console.log("Creating Salted Key");
                            var saltedkeyObject = libUplink.project_salted_key_from_passphrasec(lO_Openproject.data,storjConfig.encryptionPassphrase);
                            if(saltedkeyObject.error == ""){
                                console.log("Salted Key Received : ",saltedkeyObject.data);
                                console.log("Creating Encryption Access");
                                var lO_encryption_access = libUplink.new_encryption_access_with_default_keyc(saltedkeyObject.data);
                                if(lO_encryption_access.error==""){
                                    console.log("Encryption Access received !");
                                    console.log
                                    lO_ScopeObject = libUplink.new_scopec(storjConfig.satelliteURL,lO_APIkeyRef.data, lO_encryption_access.data);
                                    if(lO_ScopeObject.error==""){
                                        console.log("Scope Received !");
                                        lb_scopeRecieved = true;
                                    }else{
                                        console.log("FAILed to create Scope");
                                        console.log(lO_ScopeObject.error);
                                    }
                                }else{
                                    console.log("FAILed to create encryption access")
                                }
                            }else{
                                console.log("FAILed to create salted key");
                                console.log(saltedkeyObject.error);
                            }
                            console.log("Deleting Object...");
                            var deletingerror = libUplink.delete_objectc(lO_OpenBucket.data,storjConfig.uploadPath);
                            if(deletingerror==""){
                                console.log("Object deleted from storj successfully !!");
                            }else{
                                console.log("FAILed to delete object ",storjConfig.uploadPath);
                            }

                            console.log("Closing Bucket...");
                            var errorbucket = libUplink.close_bucketc(lO_OpenBucket.data);
                            if(errorbucket==""){
                                console.log("Bucket closed !!");
                            }else{
                                console.log("FAILed to close bucket");
                            }
                            
   
                        }else{
                            console.error("FAILed to open desired bucket!");
                            console.log(lO_OpenBucket.error);
                        }

                    }else{
                        console.error("FAILed to get encryption access from given passphrase!");
                        console.log(serialaccess.error);
                    }          
            console.log("Closing Project");
            var lS_closeprojecterror = libUplink.close_projectc(lO_Openproject.data);
            if(close_uplink_error==""){
                console.log("Project closed !!");
            }else{
                console.log("FAILed to close project");
                console.log(lS_closeprojecterror);
            }
            console.log("Closing Uplink");
            var close_uplink_error = libUplink.close_uplinkc(lO_uplinkRef.data);
            if(close_uplink_error==""){
                console.log("Uplink closed !!");
            }else{
                console.log("FAILed to close Uplink");
                console.log(close_uplink_error);
            }


        }else{
            console.error("FAILed to open desired Storj project!");
            console.log(lO_Openproject.error);
        }
    }else{
        console.error("FAILed to parse the API key!");
        console.log(lO_APIkeyRef.error);
    }
}else{
    console.error("FAILed to set-up a new uplink!");
    console.log(lO_uplinkRef.error);
}

if(lb_scopeRecieved){
    console.log("Serilized Scope");
    var lO_serializedscope = libUplink.serialize_scopec(lO_ScopeObject.data);
    //
    if(lO_serializedscope.error==""){
        
        console.log("Serilized Scope Received");
        var lO_SerilizedAddress = libUplink.get_scope_satellite_addressc(lO_ScopeObject.data);
        
        if(lO_SerilizedAddress.error==""){
            console.log("Satellite received !!");
            console.log("Getting Encryption Access...");
            
            var lO_ScopeEncAccess = libUplink.get_scope_enc_accessc(lO_ScopeObject.data);
            if(lO_ScopeEncAccess.error==""){
                //
                console.log("Encryption Access Received");
                var lO_APIKeyRefObject = libUplink.get_scope_api_keyc(lO_ScopeObject.data);
                if(lO_APIKeyRefObject.error==""){
                    console.log("API Key Received");
                    console.log("Setting-up a New Uplink...");
                    var tempScope = "";
                    var lO_uplinkconfig1 = storj.UplinkConfig;
                    var lO_uplinkScope = libUplink.new_uplinkc(lO_uplinkconfig1,tempScope);
                    if(lO_uplinkScope.error==""){
                        console.log("Opening Project...");
                        var lO_Openprojectscope = libUplink.open_projectc(lO_uplinkScope.data,lO_SerilizedAddress.data,lO_APIKeyRefObject.data);
                        if(lO_Openprojectscope.error==""){
                            console.log("Project Opened with scope !!");
                            console.log("Opening Bucket with scope...");
                            var serialaccessScope = libUplink.serialize_encryption_accessc(lO_ScopeEncAccess.data);
                            if(serialaccessScope.error=""){
                                var lO_OpenBucketScope = libUplink.open_bucketc(lO_Openprojectscope.data,storjConfig.bucketName,serialaccessScope.data);
                                 if(lO_OpenBucketScope.error==""){
                                    console.log("Bucket Opened with scope!!");

                                    console.log("Closing Bucket...");
                                    var errorbucket = libUplink.close_bucketc(lO_OpenBucketScope.data);
                                    if(errorbucket==""){
                                        console.log("Bucket closed !!");
                                    }else{
                                        console.log("FAILed to close bucket");
                                    }
                                 }else{
                                    console.log("FAILed to open bucket with scope");
                                 }
                                 console.log("Deleting bucket...");
                                 var deleteBucketError = libUplink.delete_bucketc(lO_Openprojectscope.data,storjConfig.bucketName);
                                 if(deleteBucketError==""){
                                    console.log("Bucket deleted !!!");

                                 }else{
                                    console.log("FAILed to deleted bucket");
                                    console.log(deleteBucketError);
                                 }
                            }else{
                                console.log("FAILed to received Serilized access of scope");
                                console.log(serialaccessScope.error);
                            }

                            console.log("Closing Project");
                            var lS_closeprojecterrorscope = libUplink.close_projectc(lO_Openprojectscope.data);
                            if(lS_closeprojecterrorscope==""){
                                console.log("Project closed !!");
                            }else{
                                console.log("FAILed to close project");
                                console.log(lS_closeprojecterrorscope);
                            }
                                 
                        }else{
                            console.log("FAILed to open project with scope");
                            console.log(lO_Openprojectscope.data);
                        }
                        console.log("Closing Uplink");
                        var close_uplink_errorscope = libUplink.close_uplinkc(lO_uplinkScope.data);
                        if(close_uplink_errorscope==""){
                            console.log("Uplink closed !!");
                        }else{
                            console.log("FAILed to close Uplink");
                            console.log(close_uplink_errorscope);
                        }

                    }else{
                        console.error("FAILed to set-up a new uplink!");
                        console.log(lO_uplinkScope.error);
                    }
                }else{
                    console.log("FAILed to get APIkey");
                    console.log(lO_APIKeyRefObject.error);
                }

            }else{

                console.log("FAILed to received encryption access");
                console.log(lO_ScopeEncAccess.error);    
            } 
        }else{

            console.log("FAILed to received satellite address");
            console.log(lO_SerilizedAddress.error);
        }
    
    }else{
        console.log("FAILed to received scope");
        console.log(lO_serializedscope.error);
    }

}



function getDateTime(unix_timestamp) {
    var dateTime = new Date(unix_timestamp * 1000);
 //
 var year   = dateTime.getFullYear();
    var months = ['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec'];
    var month  = months[dateTime.getMonth()];
    var date   = "0" + dateTime.getDate();
 //
 // Hours part from the timestamp
    var hours = "0" + dateTime.getHours();
    // Minutes part from the timestamp
    var minutes = "0" + dateTime.getMinutes();
    // Seconds part from the timestamp
    var seconds = "0" + dateTime.getSeconds();

    // Will display date time in YYYY-Mon-DD HH:MM:SS format
    var formattedDateTime = year + '-' + month + '-' + date.substr(-2) + ' ' + hours.substr(-2) + ':' + minutes.substr(-2) + ':' + seconds.substr(-2);
    //
    return formattedDateTime
}
