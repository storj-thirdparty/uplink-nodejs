const fs = require("fs");

//include the Node.js-Storj bindings module
const storj = require("uplink-nodejs");
// object for all the function for uplink
const libUplink = storj.uplink;
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
//
 /*eslint-disable */
function shareaccess(storjV3,Uplink,accessResult,config){

    //Creating permission and shareprefix object required to create shareable access
    var permission = new storjV3.Permission(true,true,true,true,0,0);
    var sharePrefix = storjV3.SharePrefix;
    var sharePrefixListArray = [];
    sharePrefix.bucket = "change-me-to-desired-bucket-name";
    sharePrefix.prefix ="change-me-to-desired-object-prefix";
    sharePrefixListArray.push(sharePrefix);
    //

    //Creating sharable access
    console.log("\nCreating Shared Access....");
    var accessShareResult = Uplink.access_share(accessResult.access,permission,sharePrefixListArray,sharePrefixListArray.length);
    
    if(accessShareResult.error.message != ""){
        throw new Error("\n Error message : "+accessShareResult.error.message);
    }    
    console.log("Created Share Access !!");
    //

    //Creating serialized sharable access
    console.log("\nCreating Serialized Shared Access...");
    var lO_StringResult = Uplink.access_serialize(accessShareResult.access);
    
    if(lO_StringResult.error.message!=""){
        throw new Error("\n Error Message : "+lO_StringResult.error.message);
    }
    //

    //Example of how to use serialized sharable access to access and work up on object on storj V3 network
    //Parsing serialized sharable access to get shared access
    console.log("Parsing Serialized Shared Access ...");
    var accessResultSharedObject= Uplink.parse_access(lO_StringResult.string);
    //
    
    //Opening project using shared access
    console.log("\nOpening Project Using Shared Access..."    );
    var restrictedProjectResult = Uplink.open_project(accessResultSharedObject.access);

    if(restrictedProjectResult.error.message != ""){   
        throw new Error("\nError Message : "+restrictedProjectResult.error.message);
    }
    console.log("Opened Project Using Shared Access");
    //

    //Deleting object using shared access
    console.log("\nDeleting Object with Shared Access ...");
    var deleteObject = Uplink.delete_object(restrictedProjectResult.project,config.bucketName,config.uploadPath);
    
    if(deleteObject.error.message != ""){
        throw new Error("\nError Message : "+deleteObject.error.message);   
    }
    console.log("Object ",config.uploadPath," Deleted");
    //

    //Closing project
    console.log("\nClosing Project Opened Using Shared Access!");
    var projectError = Uplink.close_project(restrictedProjectResult.project);

    if (projectError.error.message != "") {
        console.log("\nFailed To Close Desired Project!\n", "Error Code : ", projectError.error.code, "\t Error Message : ", projectError.error.message);
    }
    console.log("Closed Desired Project!\n");
    //
    //
}
 /*eslint-enable */
//
//uploadfile function will upload file on storj V3 network
/*eslint-disable */
function uploadfile(storjV3,Uplink,config,projectResult){
    //
    console.log("Getting Upload Object....");
    //
    var uploadOptions = new storjV3.UploadOptions();
    //
    uploadOptions.expires = 0;
    //
    var uploadObjectResult = Uplink.upload_object(projectResult.project,config.bucketName,config.uploadPath,null);
    //
    if(uploadObjectResult.error.message != ""){
        throw new Error("Failed To Upload Data On Storj V3 Network");
    }
    //
    console.log(localFullFileName.src, " File: UPLOADED as ", config.uploadPath, "!");
    /*eslint-disable */
    var fileHandle = fs.openSync(localFullFileName.src, "rs");
    //
    var size = {
                    file            : `${fs.statSync(localFullFileName.src).size}`,
                    toWrite         : 0,
                    actuallyWritten : 0,
                    totalWritten    : 0
                };
    //Creating buffer for reading file
    var buffer = new Buffer.alloc(BUFFER_SIZE);
    /*eslint-enable */
    var li_retryCount = RETRY_MAX;
    //               
    var bytesRead = 0;
    var ls_error = "";
    var uploadedInfo = null;
    //Uploading data on storj V3 network
    while (size.totalWritten < size.file) {
        size.toWrite = size.file - size.totalWritten;
        
        if (size.toWrite > BUFFER_SIZE) {
            size.toWrite = BUFFER_SIZE;
        } else if (size.toWrite == 0) {
            break;
        }

        // read proper number of bytes from the file
        while ((bytesRead <= 0) && (--li_retryCount > 0)) {
            //
            /*eslint-disable */
            bytesRead = fs.readSync(fileHandle, buffer, 0, size.toWrite, size.totalWritten);
            /*eslint-enable */
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

        ls_error = "";               
        //
        do {
            // upload the buffer's content to the Storj bucket bytes_written
            uploadedInfo = Uplink.upload_write(uploadObjectResult.upload, buffer, bytesRead);
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
            console.log("File Uploaded On Storj  : ",((Number(size.totalWritten)/Number(size.file))*100).toFixed(4)," %");
        }
        
    }
    /*eslint-disable */  
    fs.closeSync(fileHandle);
    /*eslint-enable */
    console.log("Adding MetaData...");

    var customMetadataEntry1 =new storjV3.CustomMetadataEntry();
    
    customMetadataEntry1.key = "testing";
    customMetadataEntry1.key_length = customMetadataEntry1.key.length;
    customMetadataEntry1.value = "testing1";
    customMetadataEntry1.value_length = customMetadataEntry1.value.length;
    
    var customMetadataEntry2 =new storjV3.CustomMetadataEntry();
    customMetadataEntry2.key = "value";
    customMetadataEntry2.key_length = customMetadataEntry2.key.length;
    customMetadataEntry2.value = "value1";
    customMetadataEntry2.value_length = customMetadataEntry2.value.length;

    var customMetadataEntryArray =  [customMetadataEntry1,customMetadataEntry2];
    var customMetadata = new storjV3.CustomMetadata();
    customMetadata.count = customMetadataEntryArray.length;
    customMetadata.entries = customMetadataEntryArray;
    //Adding custom meta to upload object
    var setCustomMeta_result = Uplink.upload_set_custom_metadata(uploadObjectResult.upload,customMetadata);
    
    if(setCustomMeta_result.error.message==""){
        console.log("Added Custom MetaData !");
    }else{
        console.log("\n Error message : ",setCustomMeta_result.error.message);
    }
    //Commiting object on storj V3 network
    console.log("Commiting Object On Storj...");
    var uploadcommitResult = Uplink.upload_commit(uploadObjectResult.upload);
    if(uploadcommitResult.error.message!=""){
        throw uploadcommitResult.error.message;
        
    }
    console.log("Object Uploaded On Storj : ",config.uploadPath);

    console.log("Upload Info On Storj...");
    //Fetching uploaded object info from storjV3 network
    var uploadObjectInfo = Uplink.upload_info(uploadObjectResult.upload);

    if(uploadObjectInfo.error.message!=""){
        throw new Error("\n Error Message : "+uploadObjectInfo.error.message);
    }
    objectsize = uploadObjectInfo.object.system.content_length;
    console.log("\n ------------------  Object Info  ------------------ \n FileName\t\t: ",uploadObjectInfo.object.key,"\n FileSize\t\t: ",uploadObjectInfo.object.system.content_length,"\n Created\t\t: ",getDateTime(uploadObjectInfo.object.system.created));   
}
/*eslint-enable */
/*eslint-disable */
//downloadfile function will specified object from the storj V3 Network
function downloadfile(storjV3,uplink,config,projectResult){
    //Download Option
    var downloadOptions = new storjV3.DownloadOptions();
    //
    downloadOptions.offset = 0;
    downloadOptions.length = -1;
    //Initiating download
    var downloadObjectResult = uplink.download_object(projectResult.project,config.bucketName,config.uploadPath,downloadOptions);
    
    if(downloadObjectResult.error.message != ""){
        throw new Error("\n Error Message : "+downloadObjectResult.error.message);
    }

    console.log("\nDownloading Object Initated...");
    //
    var size = { downloaded      : 0,
            actuallyWritten : 0,
            totalWritten    : 0
        };
    //
    var li_retryCount = RETRY_MAX;
    //
    var ls_error = "";
    var downloadedInfo = null;
    // set-up buffer
    /*eslint-disable */
    var buffer = new Buffer.alloc(BUFFER_SIZE);
    // create/open file, to be written to with downloaded data
    var fileHandle = fs.openSync(localFullFileName.dest, "w");
    /*eslint-enable */
    var loop = true;
    while(loop) {
        ls_error = "";

        do {
            // download a part of Storj object, as per the buffer size   
            downloadedInfo = uplink.download_read(downloadObjectResult.download, buffer, BUFFER_SIZE);

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
            /*eslint-disable */
            size.actuallyWritten = fs.writeSync(fileHandle, buffer, 0, size.downloaded, size.totalWritten);
            /*eslint-enable */
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
            ls_error = "Failed to write all downloaded bytes to the file. Downloaded and written " + size.totalWritten.toString() + " bytes.";
            //
            break;
        }
        //
        if (li_retryCount < RETRY_MAX) {
            li_retryCount = RETRY_MAX;
        }
    }
    //
    console.log("Download Info Of Object");
    //Fetching infog of storj V3 object
    var downloadObjectInfo = uplink.download_info(downloadObjectResult.download);
    if(downloadObjectInfo.error.message != ""){
        throw new Error("\n Error Message : "+downloadObjectInfo.error.message);
    }
    //
    console.log("\n\n------ Download Object Info -------\n");
    console.log("Object Key\t\t: ",downloadObjectInfo.object.key);
    console.log("Object Size\t\t: ",downloadObjectInfo.object.system.content_length);
    console.log("Object Created\t\t: ",getDateTime(downloadObjectInfo.object.system.created));
    // close the opened file
    /*eslint-disable */
    fs.closeSync(fileHandle);
    /*eslint-enable */
    // close download set-up
    var downloadclose_result = uplink.close_download(downloadObjectResult.download);
    //
    console.log("Download Close");
    //
    if(downloadclose_result.error.message!=""){
        throw new Error("\n Error message : "+downloadclose_result.error.message);
    }
    //
    console.log("Object downloaded successfully");
}
/*eslint-enable */
//
try {
    //Connecting to storj network using Satellite Address , Storj API key , Encryption phassphrase
    console.log("Getting Access\nSatellite Address : ",storjConfig.satelliteURL,"\nAPI key : ",storjConfig.apiKey,"\nEncryption Passphrase : ",storjConfig.encryptionPassphrase);
    var accessResult = libUplink.request_access_with_passphrase(storjConfig.satelliteURL,storjConfig.apiKey,storjConfig.encryptionPassphrase);
    
    if(accessResult.error.message != ""){
        throw new Error("\n Error Message : "+accessResult.error.message);
    }
    console.log("Access : Granted !!");
    //

    //Opening project on storj V3 network using created access.
    console.log("Opening Storj Project...");
    var projectResult = libUplink.open_project(accessResult.access);

    if(projectResult.error.message != ""){
        throw new Error("\n Error Message : "+projectResult.error.message);
    }
    console.log("Desired Storj Project: OPENED!");
    //

    //Fetching bucket info from the storj V3 network
    console.log("Fetching Information About Bucket : ",storjConfig.bucketName);
    var bucketResult = libUplink.stat_bucket(projectResult.project,storjConfig.bucketName);
    
    if(bucketResult.error.message != ""){
        console.log("\nFailed to get bucket information !\n","Error code : ",bucketResult.error.code,"\t Error message : ",bucketResult.error.message);
    }else{
        console.log("\nBucket Information : \n Bucket Name : ",bucketResult.bucket.name,"\n Bucket Created : ",getDateTime(bucketResult.bucket.created));
    }
    //
    
    //Creating bucket on storj V3 Network
    console.log("Creating Bucket : ",storjConfig.bucketName);
    var createBucketResult = libUplink.create_bucket(projectResult.project,storjConfig.bucketName);
    
    if(createBucketResult.error.message == ""){
        console.log("\nBucket Created Successfully !\n","Bucket Name : ",createBucketResult.bucket.name,"\t Created : ",getDateTime(createBucketResult.bucket.created));
    }else{
         console.log("\nFailed to create bucket !\n","Error code : ",createBucketResult.error.code,"\t Error message : ",createBucketResult.error.message);
    }
    //

    //Fetching bucket list and displaying the list on console with their created time.
    console.log("Listing Buckets ....");    
    var listBucketsOptions = new storj.ListBucketsOptions();
    var bucketListResult = libUplink.list_buckets(projectResult.project,listBucketsOptions);
    
    if(bucketListResult.error.message!=""){
       throw new Error("\n Error Message "+bucketListResult.error.message);
    }
    
    var bucketList = bucketListResult.bucketList;
    console.log("S.No. \t Created On \t\t Bucket Name");
    console.log("===== \t ========== \t\t ===========");
    var i = 0;
    for(var bucketinfo in bucketList){
        var num = "0" + (i + 1).toString();
        console.log(num.substr(-2), "  ", getDateTime(bucketListResult.bucketList[bucketinfo].created), "\t", bucketListResult.bucketList[bucketinfo].name);
        i++;
    }
    //

    //Uploading file on storj V3 network
    uploadfile(storj,libUplink,storjConfig,projectResult,fs);
    //

    //Fetching object list and displaying the list on console with their created time and size.
    console.log("\nListing Object....");
    var listObjectsOptions = new storj.ListObjectsOptions();
    listObjectsOptions.recursive = true;
    listObjectsOptions.custom = true;
    listObjectsOptions.system = true;
    listObjectsOptions.prefix ="change-me-to-desired-object-prefix";

    var objectListResult = libUplink.list_objects(projectResult.project,storjConfig.bucketName,listObjectsOptions);
    
    if(objectListResult.error.message != ""){
        throw new Error("\n Error Message : "+objectListResult.error.message);
    }
    
    var objectList = objectListResult.objectList;
    console.log("S.No. \t Created On \t\t\t FileName \t\t FileSize");
    console.log("===== \t ========== \t\t\t =========== \t\t ===========");
    i = 0;
    for(var objectInfo in objectList){
        var numb = "0" + (i + 1).toString();
        console.log(numb.substr(-2), "  ", getDateTime(objectListResult.objectList[objectInfo].system.created), "\t\t", objectListResult.objectList[objectInfo].key,"\t\t",objectListResult.objectList[objectInfo].system.content_length);
        i++;
    }
    //
    
    //Fetching object info and displaying them on console.
    console.log("Getting Object Info");
    var statObjectInfo = libUplink.stat_object(projectResult.project,storjConfig.bucketName,storjConfig.uploadPath);
    
    if(statObjectInfo.error.message != ""){
        throw new Error("\n Error Message : "+statObjectInfo.error.message);
    }

    console.log("\n\n------ Object Info -------\n");
    console.log("Object Key\t\t: ",statObjectInfo.object.key);
    console.log("Object Size\t\t: ",statObjectInfo.object.system.content_length);
    console.log("Object Created\t\t: ",getDateTime(statObjectInfo.object.system.created));
    //

    //Downloading file on storj V3 network
    downloadfile(storj,libUplink,storjConfig,projectResult);
    //

    //Creating share access with restrictions
    shareaccess(storj,libUplink,accessResult,storjConfig);
    //

    //Deleting object from the storj V3 network
    console.log("Deleting Object...");
    var deleteObjectWithoutShare = libUplink.delete_object(projectResult.project,storjConfig.bucketName,storjConfig.uploadPath);
    
    if(deleteObjectWithoutShare.error.message != ""){
        console.log("Failed to delete object \n Error code : ",deleteObjectWithoutShare.error.code,"\tError Message : ",deleteObjectWithoutShare.error.message);
    }
    console.log("Object ",storjConfig.uploadPath," Deleted");
    //

    //Deleting bucket from the storj V3 network
    console.log("Deleting Bucket : ",storjConfig.bucketName);
    var deleteBucketResult = libUplink.delete_bucket(projectResult.project,storjConfig.bucketName);
    
    if(deleteBucketResult.error.message != ""){
        console.log("\nFailed to delete bucket !\n","Error code : ",deleteBucketResult.error.code,"\t Error message : ",deleteBucketResult.error.message);      
    }else {
        console.log("\nBucket deleted successfully !! \n Bucket Name : ",deleteBucketResult.bucket.name,"\t Created  : ",getDateTime(deleteBucketResult.bucket.created));
    }
    //

    //Closing project
    console.log("\nClosing Project !");
    var projectError = libUplink.close_project(projectResult.project);
    
    if(projectError.error.message != ""){
        console.log("\nFailed to close desired project!\n","Error Code : ",projectError.error.code,"\t Error Message : ",projectError.error.message);   
    }
    console.log("Closed desired project!\n");
    //

} catch(e){
    console.log(e);
}
//

//
function getDateTime(unix_timestamp) {
    var dateTime = new Date(unix_timestamp * 1000);
    
    var year   = dateTime.getFullYear();
    var months = ['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec'];
    var month  = months[dateTime.getMonth()];
    var date   = "0" + dateTime.getDate();

    // Hours part from the timestamp
    var hours = "0" + dateTime.getHours();
    // Minutes part from the timestamp
    var minutes = "0" + dateTime.getMinutes();
    // Seconds part from the timestamp
    var seconds = "0" + dateTime.getSeconds();

    // Will display date time in YYYY-Mon-DD HH:MM:SS format
    var formattedDateTime = year + '-' + month + '-' + date.substr(-2) + ' ' + hours.substr(-2) + ':' + minutes.substr(-2) + ':' + seconds.substr(-2);
    
    return formattedDateTime;
}

