const fs = require("fs");

//include the Node.js-Storj bindings module
var storj = require("./node_modules/storj-nodejs/libUplinkNode.js");
// object for all the function for uplink
var libUplink = storj.storj_nodejs;
//
const BUFFER_SIZE = 80000;
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

var objectsize =0;
console.log("Getting Access\nSatellite Address : ",storjConfig.satelliteURL,"\nAPI key : ",storjConfig.apiKey,"\nEncryption Passphrase : ",storjConfig.encryptionPassphrase);
var lO_AccessResult = libUplink.request_access_with_passphrasec(storjConfig.satelliteURL,storjConfig.apiKey,storjConfig.encryptionPassphrase);

if(lO_AccessResult.error.message == ""){
    console.log("Access : Granted !!");
    console.log("Opening Storj Project...");
    var lO_ProjectResult = libUplink.open_projectc(lO_AccessResult.access);
    if(lO_ProjectResult.error.message == ""){
        console.log("Desired Storj Project: OPENED!");
        console.log("Fetching Information About Bucket : ",storjConfig.bucketName);
        
        var lO_BucketResult = libUplink.stat_bucketc(lO_ProjectResult.project,storjConfig.bucketName);
        if(lO_BucketResult.error.message != ""){
            console.log("\nFAILed to get bucket information !\n","error code : ",lO_BucketResult.error.code,"\t error message : ",lO_BucketResult.error.message);
        }else{
            console.log("\nBucket Information : \n Bucket Name : ",lO_BucketResult.bucket.name,"\n Bucket Created : ",getDateTime(lO_BucketResult.bucket.created));
        }

        console.log("Creating Bucket : ",storjConfig.bucketName);
        var lO_createBucketResult = libUplink.create_bucketc(lO_ProjectResult.project,storjConfig.bucketName);
        
        if(lO_createBucketResult.error.message == ""){
            console.log("\nBucket created successfully !\n","Bucket Name : ",lO_createBucketResult.bucket.name,"\t Created : ",getDateTime(lO_createBucketResult.bucket.created));
        }else{
             console.log("\nFAILed to create bucket !\n","error code : ",lO_createBucketResult.error.code,"\t error message : ",lO_createBucketResult.error.message);
        }
        console.log("Listing Buckets ....");
        
        var lO_ListBucketsOptions = new storj.ListBucketsOptions();
        var lO_BucketListObject = libUplink.list_bucketsc(lO_ProjectResult.project,lO_ListBucketsOptions);
        var i=0;
        if(lO_BucketListObject.error.message!=""){
            console.log("FAILed to Get list of bucket !! \n Error Code ",lO_BucketListObject.error.code,"\t Error Message ",lO_BucketListObject.error.message);
        }else{
            var bucketList = lO_BucketListObject.bucketList;
            console.log("S.No. \t Created On \t\t Bucket Name");
            console.log("===== \t ========== \t\t ===========");
            for(bucketinfo in bucketList){
                var num = "0" + (i + 1).toString();
                console.log(num.substr(-2), "  ", getDateTime(lO_BucketListObject.bucketList[bucketinfo].created), "\t", lO_BucketListObject.bucketList[bucketinfo].name);
                i++;
            }
        }
        
        console.log("Getting Upload Object....");

        var lO_uploadOptions = new storj.UploadOptions();

        lO_uploadOptions.expires = 0;
        var lO_uploadObject = libUplink.upload_objectc(lO_ProjectResult.project,storjConfig.bucketName,storjConfig.uploadPath,null);

        if(lO_uploadObject.error.message == ""){

            console.log(localFullFileName.src, " File: UPLOADED as ", storjConfig.uploadPath, "!");
            
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
                    // upload the buffer's content to the Storj bucket bytes_written
                    var uploadedInfo = libUplink.upload_writec(lO_uploadObject.upload, buffer, bytesRead);
                    if (uploadedInfo.error.message != "") {
                        ls_error = uploadedInfo.error.message;
                    } else {
                        size.actuallyWritten = uploadedInfo.bytes_written;
                        if(size.actuallyWritten>=size.file){
                            break;
                        }
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
                if((size.totalWritten>0)&&(size.file>0)){
                    console.log("File uploaded on storj  : ",((Number(size.totalWritten)/Number(size.file))*100).toFixed(4)," %");
                }
                
            }
                
            fs.closeSync(fileHandle);
            
            var ls_dataUploaded = "\n " + ((size.totalWritten * 100.0)/size.file).toString() + "% uploaded!";
            
            console.log("Adding MetaData...");
            
            var lO_CustomMetadataEntry1 =new storj.CustomMetadataEntry();
            
            lO_CustomMetadataEntry1.key = "testing";
            lO_CustomMetadataEntry1.key_length = 7;
            lO_CustomMetadataEntry1.value = "testing1";
            lO_CustomMetadataEntry1.value_length = 8;
            
            var lO_CustomMetadataEntry2 =new storj.CustomMetadataEntry();
            lO_CustomMetadataEntry2.key = "value";
            lO_CustomMetadataEntry2.key_length = 4;
            lO_CustomMetadataEntry2.value = "value1";
            lO_CustomMetadataEntry2.value_length = 5;

            var lO_CustomMetadataEntryArray =  [lO_CustomMetadataEntry1,lO_CustomMetadataEntry2];
            var lO_CustomMetadata = new storj.CustomMetadata();
            lO_CustomMetadata.count = lO_CustomMetadataEntryArray.length;
            lO_CustomMetadata.entries = lO_CustomMetadataEntryArray;
            var lO_setCustomMeta = libUplink.upload_set_custom_metadatac(lO_uploadObject.upload,lO_CustomMetadata);
            
            if(lO_setCustomMeta.error.message==""){
                console.log("Custom MetaData for the object !");
            }else{
                console.log("FAILed to set custom metadata \n Error code : ",lO_setCustomMeta.error.code,"\t Error message : ",lO_setCustomMeta.error.message);
            }
            
            console.log("Commiting Object on storj...");
            var ls_uploadcommit = libUplink.upload_commitc(lO_uploadObject.upload);
            if(ls_uploadcommit.error.message==""){
                console.log("Object Uploaded on storj : ",storjConfig.uploadPath);
            }else{
                console.log("FAILed to upload object on storj");
                
            }

            console.log("Upload Info on storj...");
            var lO_uploadinfo = libUplink.upload_infoc(lO_uploadObject.upload);

            if(lO_uploadinfo.error.message==""){
                objectsize = lO_uploadinfo.object.system.content_length;
                console.log("\n ------------------  Object Info  ------------------ \n FileName\t\t: ",lO_uploadinfo.object.key,"\n FileSize\t\t: ",lO_uploadinfo.object.system.content_length,"\n Created\t\t: ",getDateTime(lO_uploadinfo.object.system.created));
                
            }else{
                console.log("FAILed to get upload Info \n Error Code : ",lO_uploadinfo.error.code,"\t Error Message : ",lO_uploadinfo.error.message);
            }

        }else{
            console.log("FAILed to upload object !\n","error code : ",lO_uploadObject.error.code,"\t error message : ",lO_uploadObject.error.message);
        }
        
        console.log("\nListing Object....");

        var lO_ListObjectsOptions = new storj.ListObjectsOptions();

        lO_ListObjectsOptions.recursive = true;
        lO_ListObjectsOptions.custom = true;
        lO_ListObjectsOptions.system = true;
        lO_ListObjectsOptions.prefix ="change-me-to-desired-object-prefix";

        var lO_ObjectList = libUplink.list_objectsc(lO_ProjectResult.project,storjConfig.bucketName,lO_ListObjectsOptions);
        i = 0;
        
        if(lO_ObjectList.error.message==""){
            var objectList = lO_ObjectList.objectList;
            console.log("S.No. \t Created On \t\t\t FileName \t\t FileSize");
            console.log("===== \t ========== \t\t\t =========== \t\t ===========");
            
            for(objectInfo in objectList){
                var num = "0" + (i + 1).toString();
                console.log(num.substr(-2), "  ", getDateTime(lO_ObjectList.objectList[objectInfo].system.created), "\t\t", lO_ObjectList.objectList[objectInfo].key,"\t\t",lO_ObjectList.objectList[objectInfo].system.content_length);
                i++;
            }
        }else{
            console.log("FAILed to List Object \n Error code : ",lO_ObjectList.error.code,"\t Error Message : ",lO_ObjectList.error.message);
        }
        //
        console.log("Getting Object Info");
        
        var lO_ObjectInfo = libUplink.stat_objectc(lO_ProjectResult.project,storjConfig.bucketName,storjConfig.uploadPath);
        
        if(lO_ObjectInfo.error.message==""){
            console.log("\n\n------ Object Info -------\n");
            console.log("Object Key\t\t: ",lO_ObjectInfo.object.key);
            console.log("Object Size\t\t: ",lO_ObjectInfo.object.system.content_length);
            console.log("Object Created\t\t: ",getDateTime(lO_ObjectInfo.object.system.created));
        }else
        {
            console.log("FAILed to get Object Info \n Error Code : ",lO_ObjectInfo.error.code,"\t Error Message : ",lO_ObjectInfo.error.message);
        }
        
        var dataread = new Buffer.alloc(BUFFER_SIZE);
        
        var lO_DownloadOptions = new storj.DownloadOptions();

        lO_DownloadOptions.offset = 0;
        lO_DownloadOptions.length = -1;

        var lO_downloadObject = libUplink.download_objectc(lO_ProjectResult.project,storjConfig.bucketName,storjConfig.uploadPath,lO_DownloadOptions);
        if(lO_downloadObject.error.message==""){
            console.log("Downloading Object initated...");
            //
            size = { downloaded      : 0,
                    actuallyWritten : 0,
                    totalWritten    : 0
                };
            //
            var li_retryCount = RETRY_MAX;
            // set-up buffer
            var buffer = new Buffer.alloc(BUFFER_SIZE);
            // create/open file, to be written to with downloaded data
            var fileHandle = fs.openSync(localFullFileName.dest, "w");
            while(true) {
                var ls_error = "";

                do {
                    // download a part of Storj object, as per the buffer size   
                    var downloadedInfo = libUplink.download_readc(lO_downloadObject.download, buffer, BUFFER_SIZE);

                    if (downloadedInfo.error.message != "") {
                        ls_error = downloadedInfo.error;
                    } else {
                        size.downloaded = downloadedInfo.bytes_read;
                        if(size.actuallyWritten>=objectsize){
                            break;
                        } 
                    }
                    
                } while((ls_error != "") && (--li_retryCount > 0));
                //
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
                if((size.totalWritten>0)&&(objectsize>0)){
                    console.log("File Dowloaded : ",((Number(size.totalWritten)/Number(objectsize))*100).toFixed(4)," %");
                }
               
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

            console.log("Download Info from storj...");
            var lO_downloadinfo = libUplink.download_infoc(lO_downloadObject.download);
            if(lO_downloadinfo.error.message==""){
                console.log("\n\n------ Download Object Info -------\n");
                console.log("Object Key\t\t: ",lO_downloadinfo.object.key);
                console.log("Object Size\t\t: ",lO_downloadinfo.object.system.content_length);
                console.log("Object Created\t\t: ",getDateTime(lO_downloadinfo.object.system.created));
            }else{
                console.log("FAILed to get upload Info \n Error Code : ",lO_downloadinfo.error.code,"\t Error Message : ",lO_downloadinfo.error.message);
            }
            // close the opened file
            fs.closeSync(fileHandle);
            // close download set-up
            var lO_downloadclose = libUplink.close_downloadc(lO_downloadObject.download);
            //
            console.log("Download close function");
            //
            if(lO_downloadclose.error.message==""){
                console.log("Object Downloaded successfully");
            }else{
                console.log("FAILed to download file");
                console.log("Error code :",lO_downloadclose.error.code," Error message : ",lO_downloadclose.error.message);
            }
            //
        }else{
            console.log("\nFAILed to download object : ",storjConfig.uploadPath," ! \n","error code : ",lO_downloadObject.error.code,"\t error message : ",lO_downloadObject.error.message);
        }
        //
        console.log("Creating restrict access....");
        var lO_Permission = new storj.Permission();
        lO_Permission.allow_delete = true
        var lO_SharePrefix = storj.SharePrefix;
        var lO_SharePrefixListArray = [];
        lO_SharePrefix.bucket = "change-me-to-desired-bucket-name";
        lO_SharePrefix.prefix ="change-me-to-desired-object-prefix";
        lO_SharePrefixListArray.push(lO_SharePrefix);
        var lO_AccessShareResult = libUplink.access_sharec(lO_AccessResult.access,lO_Permission,lO_SharePrefixListArray,lO_SharePrefixListArray.length);
        
        if(lO_AccessShareResult.error.message==""){
            console.log("Created share access !!");
            console.log("Creating Share Access Key...");
            var lO_StringResult = libUplink.access_serializec(lO_AccessShareResult.access);

            if(lO_StringResult.error.message==""){
                var lO_AccessResultSharedObject= libUplink.parse_accessc(lO_StringResult.string);
                console.log("Parsing Access created");
                console.log("Opening Project Using Shared Access..."    );
                var lO_RestrictedProjectResult = libUplink.open_projectc(lO_AccessResultSharedObject.access);
                if(lO_RestrictedProjectResult.error.message==""){
                    console.log("Opened Project using share key ");
                }else{
                    console.log("FAILed to open project \n Error Code : ",lO_RestrictedProjectResult.error.code,"\t Error Message : ",lO_RestrictedProjectResult.error.message);
                }
                console.log("Deleting Object with shared access ...");
                var lO_deleteObject = libUplink.delete_objectc(lO_RestrictedProjectResult.project,storjConfig.bucketName,storjConfig.uploadPath);
            
                if(lO_deleteObject.error.message==""){
                    console.log("Object ",storjConfig.uploadPath," Deleted");
                }else{
                    console.log("FAILed to delete object \n Error code : ",lO_deleteObject.error.code,"\tError Message : ",lO_deleteObject.error.message);
                }
            }else{
                console.log("FAILed to create parse accesse");
            }

        }else{
            console.log("FAILed to receive share access \n","Error Code : ",lO_AccessShareResult.error.code,"\t Error message : ",lO_AccessShareResult.error.message);
        }

        console.log("Deleting Object without shared access ...");
        
        var lO_deleteObjectWithoutShare = libUplink.delete_objectc(lO_ProjectResult.project,storjConfig.bucketName,storjConfig.uploadPath);
        
        if(lO_deleteObjectWithoutShare.error.message==""){
            console.log("Object ",storjConfig.uploadPath," Deleted");
        }else{
            console.log("FAILed to delete object \n Error code : ",lO_deleteObjectWithoutShare.error.code,"\tError Message : ",lO_deleteObjectWithoutShare.error.message);
        }
        
        console.log("Deleting Bucket : ",storjConfig.bucketName);
        var lO_deleteBucketResult = libUplink.delete_bucketc(lO_ProjectResult.project,storjConfig.bucketName);
        
        if(lO_deleteBucketResult.error.message == ""){
            console.log("\nBucket deleted successfully !! \n Bucket Name : ",lO_deleteBucketResult.bucket.name,"\t Created  : ",getDateTime(lO_deleteBucketResult.bucket.created));
        }else{
            console.log("\nFAILed to delete bucket !\n","error code : ",lO_BucketResult.error.code,"\t error message : ",lO_BucketResult.error.message);
        }
		
        var lO_ProjectError = libUplink.close_projectc(lO_ProjectResult.project);
        console.log("\nClosing Project !");
        if(lO_ProjectError.error.message == ""){
            console.log("Closed desired project!\n");
        }else{
            console.log("\nFAILed to close desired project!\n","Error Code : ",lO_ProjectError.error.code,"\t Error Message : ",lO_ProjectError.error.message);
        }
    }else{
       console.error("\nFAILed to open desired project!\n","Error Code : ",lO_ProjectResult.error.code,"\t Error Message : ",lO_ProjectResult.error.message);
    }

}else{
    console.error("\nFAILed to get access !\n","Error Code : ",lO_AccessResult.error.code,"\t Error Message : ",lO_AccessResult.error.message);
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
