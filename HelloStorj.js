const fs = require("fs");

//include the Node.js-Storj bindings module
var storj = require("storj-nodejs");
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
    encryptionPassphrase  : "change-me-to-desired-bucket-name",
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
var accessResult = libUplink.request_access_with_passphrasec(storjConfig.satelliteURL,storjConfig.apiKey,storjConfig.encryptionPassphrase);

if(accessResult.error.message == ""){
    console.log("Access : Granted !!");
    console.log("Opening Storj Project...");

    var projectResult = libUplink.open_projectc(accessResult.access);
    if(projectResult.error.message == ""){
        console.log("Desired Storj Project: OPENED!");
        console.log("Fetching Information About Bucket : ",storjConfig.bucketName);
        
        var bucketResult = libUplink.stat_bucketc(projectResult.project,storjConfig.bucketName);
        if(bucketResult.error.message != ""){
            console.log("\nFAILed to get bucket information !\n","error code : ",bucketResult.error.code,"\t error message : ",bucketResult.error.message);
        }else{
            console.log("\nBucket Information : \n Bucket Name : ",bucketResult.bucket.name,"\n Bucket Created : ",getDateTime(bucketResult.bucket.created));
        }

        console.log("Creating Bucket : ",storjConfig.bucketName);
        var createBucketResult = libUplink.create_bucketc(projectResult.project,storjConfig.bucketName);
        
        if(createBucketResult.error.message == ""){
            console.log("\nBucket created successfully !\n","Bucket Name : ",createBucketResult.bucket.name,"\t Created : ",getDateTime(createBucketResult.bucket.created));
        }else{
             console.log("\nFAILed to create bucket !\n","error code : ",createBucketResult.error.code,"\t error message : ",createBucketResult.error.message);
        }
        console.log("Listing Buckets ....");
        
        var listBucketsOptions = new storj.ListBucketsOptions();
        var bucketListResult = libUplink.list_bucketsc(projectResult.project,listBucketsOptions);
        var i=0;
        if(bucketListResult.error.message!=""){
            console.log("FAILed to Get list of bucket !! \n Error Code ",bucketListResult.error.code,"\t Error Message ",bucketListResult.error.message);
        }else{
            var bucketList = bucketListResult.bucketList;
            console.log("S.No. \t Created On \t\t Bucket Name");
            console.log("===== \t ========== \t\t ===========");
            for(var bucketinfo in bucketList){
                var num = "0" + (i + 1).toString();
                console.log(num.substr(-2), "  ", getDateTime(bucketListResult.bucketList[bucketinfo].created), "\t", bucketListResult.bucketList[bucketinfo].name);
                i++;
            }
        }
        
        console.log("Getting Upload Object....");

        var uploadOptions = new storj.UploadOptions();

        uploadOptions.expires = 0;
        var uploadObjectResult = libUplink.upload_objectc(projectResult.project,storjConfig.bucketName,storjConfig.uploadPath,null);

        if(uploadObjectResult.error.message == ""){

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
                    var uploadedInfo = libUplink.upload_writec(uploadObjectResult.upload, buffer, bytesRead);
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
            
            console.log("Adding MetaData...");
            
            var customMetadataEntry1 =new storj.CustomMetadataEntry();
            
            customMetadataEntry1.key = "testing";
            customMetadataEntry1.key_length = 7;
            customMetadataEntry1.value = "testing1";
            customMetadataEntry1.value_length = 8;
            
            var customMetadataEntry2 =new storj.CustomMetadataEntry();
            customMetadataEntry2.key = "value";
            customMetadataEntry2.key_length = 4;
            customMetadataEntry2.value = "value1";
            customMetadataEntry2.value_length = 5;

            var customMetadataEntryArray =  [customMetadataEntry1,customMetadataEntry2];
            var customMetadata = new storj.CustomMetadata();
            customMetadata.count = customMetadataEntryArray.length;
            customMetadata.entries = customMetadataEntryArray;
            var setCustomMeta_result = libUplink.upload_set_custom_metadatac(uploadObjectResult.upload,customMetadata);
            
            if(setCustomMeta_result.error.message==""){
                console.log("Custom MetaData for the object !");
            }else{
                console.log("FAILed to set custom metadata \n Error code : ",setCustomMeta_result.error.code,"\t Error message : ",setCustomMeta_result.error.message);
            }
            
            console.log("Commiting Object on storj...");
            var uploadcommitResult = libUplink.upload_commitc(uploadObjectResult.upload);
            if(uploadcommitResult.error.message==""){
                console.log("Object Uploaded on storj : ",storjConfig.uploadPath);
            }else{
                console.log("FAILed to upload object on storj");
                
            }

            console.log("Upload Info on storj...");
            var uploadObjectInfo = libUplink.upload_infoc(uploadObjectResult.upload);

            if(uploadObjectInfo.error.message==""){
                objectsize = uploadObjectInfo.object.system.content_length;
                console.log("\n ------------------  Object Info  ------------------ \n FileName\t\t: ",uploadObjectInfo.object.key,"\n FileSize\t\t: ",uploadObjectInfo.object.system.content_length,"\n Created\t\t: ",getDateTime(uploadObjectInfo.object.system.created));
                
            }else{
                console.log("FAILed to get upload Info \n Error Code : ",uploadObjectInfo.error.code,"\t Error Message : ",uploadObjectInfo.error.message);
            }

        }else{
            console.log("FAILed to upload object !\n","error code : ",uploadObjectResult.error.code,"\t error message : ",uploadObjectResult.error.message);
        }
        
        console.log("\nListing Object....");

        var listObjectsOptions = new storj.ListObjectsOptions();

        listObjectsOptions.recursive = true;
        listObjectsOptions.custom = true;
        listObjectsOptions.system = true;
        listObjectsOptions.prefix ="change-me-to-desired-object-prefix";

        var objectListResult = libUplink.list_objectsc(projectResult.project,storjConfig.bucketName,listObjectsOptions);
        i = 0;
        
        if(objectListResult.error.message==""){
            var objectList = objectListResult.objectList;
            console.log("S.No. \t Created On \t\t\t FileName \t\t FileSize");
            console.log("===== \t ========== \t\t\t =========== \t\t ===========");
            
            for(var objectInfo in objectList){
                var numb = "0" + (i + 1).toString();
                console.log(numb.substr(-2), "  ", getDateTime(objectListResult.objectList[objectInfo].system.created), "\t\t", objectListResult.objectList[objectInfo].key,"\t\t",objectListResult.objectList[objectInfo].system.content_length);
                i++;
            }
        }else{
            console.log("FAILed to List Object \n Error code : ",objectListResult.error.code,"\t Error Message : ",objectListResult.error.message);
        }
        //
        console.log("Getting Object Info");
        
        var statObjectInfo = libUplink.stat_objectc(projectResult.project,storjConfig.bucketName,storjConfig.uploadPath);
        
        if(statObjectInfo.error.message==""){
            console.log("\n\n------ Object Info -------\n");
            console.log("Object Key\t\t: ",statObjectInfo.object.key);
            console.log("Object Size\t\t: ",statObjectInfo.object.system.content_length);
            console.log("Object Created\t\t: ",getDateTime(statObjectInfo.object.system.created));
        }else
        {
            console.log("FAILed to get Object Info \n Error Code : ",statObjectInfo.error.code,"\t Error Message : ",statObjectInfo.error.message);
        }
        
        var downloadOptions = new storj.DownloadOptions();

        downloadOptions.offset = 0;
        downloadOptions.length = -1;

        var downloadObjectResult = libUplink.download_objectc(projectResult.project,storjConfig.bucketName,storjConfig.uploadPath,downloadOptions);
        if(downloadObjectResult.error.message==""){
            console.log("Downloading Object initated...");
            //
            size = { downloaded      : 0,
                    actuallyWritten : 0,
                    totalWritten    : 0
                };
            //
            li_retryCount = RETRY_MAX;
            // set-up buffer
            //var buffer = new Buffer.alloc(BUFFER_SIZE);
            // create/open file, to be written to with downloaded data
            fileHandle = fs.openSync(localFullFileName.dest, "w");
            while(true) {
                ls_error = "";

                do {
                    // download a part of Storj object, as per the buffer size   
                    var downloadedInfo = libUplink.download_readc(downloadObjectResult.download, buffer, BUFFER_SIZE);

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
            var downloadObjectInfo = libUplink.download_infoc(downloadObjectResult.download);
            if(downloadObjectInfo.error.message==""){
                console.log("\n\n------ Download Object Info -------\n");
                console.log("Object Key\t\t: ",downloadObjectInfo.object.key);
                console.log("Object Size\t\t: ",downloadObjectInfo.object.system.content_length);
                console.log("Object Created\t\t: ",getDateTime(downloadObjectInfo.object.system.created));
            }else{
                console.log("FAILed to get upload Info \n Error Code : ",downloadObjectInfo.error.code,"\t Error Message : ",downloadObjectInfo.error.message);
            }
            // close the opened file
            fs.closeSync(fileHandle);
            // close download set-up
            var downloadclose_result = libUplink.close_downloadc(downloadObjectResult.download);
            //
            console.log("Download close function");
            //
            if(downloadclose_result.error.message==""){
                console.log("Object Downloaded successfully");
            }else{
                console.log("FAILed to download file");
                console.log("Error code :",downloadclose_result.error.code," Error message : ",downloadclose_result.error.message);
            }
            //
        }else{
            console.log("\nFAILed to download object : ",storjConfig.uploadPath," ! \n","error code : ",downloadObjectResult.error.code,"\t error message : ",downloadObjectResult.error.message);
        }
        //
        console.log("Creating restrict access....");
        var permission = new storj.Permission(true,true,true,true,0,0);
        var sharePrefix = storj.SharePrefix;
        var sharePrefixListArray = [];
        sharePrefix.bucket = "change-me-to-desired-bucket-name";
        sharePrefix.prefix ="change-me-to-desired-object-prefix";
        sharePrefixListArray.push(sharePrefix);
        var accessShareResult = libUplink.access_sharec(accessResult.access,permission,sharePrefixListArray,sharePrefixListArray.length);
        
        if(accessShareResult.error.message==""){
            console.log("Created share access !!");
            console.log("Creating Share Access Key...");
            var lO_StringResult = libUplink.access_serializec(accessShareResult.access);

            if(lO_StringResult.error.message==""){
                var accessResultSharedObject= libUplink.parse_accessc(lO_StringResult.string);
                console.log("Parsing Access created");
                console.log("Opening Project Using Shared Access..."    );
                var restrictedProjectResult = libUplink.open_projectc(accessResultSharedObject.access);
                if(restrictedProjectResult.error.message==""){
                    console.log("Opened Project using share key ");
                }else{
                    console.log("FAILed to open project \n Error Code : ",restrictedProjectResult.error.code,"\t Error Message : ",restrictedProjectResult.error.message);
                }
                console.log("Deleting Object with shared access ...");
                var deleteObject = libUplink.delete_objectc(restrictedProjectResult.project,storjConfig.bucketName,storjConfig.uploadPath);
                
                if(deleteObject.error.message==""){
                    console.log("Object ",storjConfig.uploadPath," Deleted");
                }else{
                    console.log("FAILed to delete object \n Error code : ",deleteObject.error.code,"\tError Message : ",deleteObject.error.message);
                }
            }else{
                console.log("FAILed to create parse accesse");
            }

        }else{
            console.log("FAILed to receive share access \n","Error Code : ",accessShareResult.error.code,"\t Error message : ",accessShareResult.error.message);
        }

        console.log("Deleting Object without shared access ...");
        
        var deleteObjectWithoutShare = libUplink.delete_objectc(projectResult.project,storjConfig.bucketName,storjConfig.uploadPath);
        
        if(deleteObjectWithoutShare.error.message==""){
            console.log("Object ",storjConfig.uploadPath," Deleted");
        }else{
            console.log("FAILed to delete object \n Error code : ",deleteObjectWithoutShare.error.code,"\tError Message : ",deleteObjectWithoutShare.error.message);
        }
        
        console.log("Deleting Bucket : ",storjConfig.bucketName);
        var deleteBucketResult = libUplink.delete_bucketc(projectResult.project,storjConfig.bucketName);
        
        if(deleteBucketResult.error.message == ""){
            console.log("\nBucket deleted successfully !! \n Bucket Name : ",deleteBucketResult.bucket.name,"\t Created  : ",getDateTime(deleteBucketResult.bucket.created));
        }else{
            console.log("\nFAILed to delete bucket !\n","error code : ",bucketResult.error.code,"\t error message : ",bucketResult.error.message);
        }
		
        var projectError = libUplink.close_projectc(projectResult.project);
        console.log("\nClosing Project !");
        if(projectError.error.message == ""){
            console.log("Closed desired project!\n");
        }else{
            console.log("\nFAILed to close desired project!\n","Error Code : ",projectError.error.code,"\t Error Message : ",projectError.error.message);
        }
    }else{
       console.error("\nFAILed to open desired project!\n","Error Code : ",projectResult.error.code,"\t Error Message : ",projectResult.error.message);
    }

}else{
    console.error("\nFAILed to get access !\n","Error Code : ",accessResult.error.code,"\t Error Message : ",accessResult.error.message);
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
