const fs = require("fs");

//include the Node.js-Storj bindings module
const storj = require("uplink-nodejs");
// object for all the function for uplink
const libUplink = storj.uplink;
//
var BUFFER_SIZE = 80000;
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
function getDateTime(unixTimestamp) {
    var dateTime = new Date(unixTimestamp * 1000);
    
    var year   = dateTime.getFullYear();
    var months = ["Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"];
    var month  = months[dateTime.getMonth()];
    var date   = "0" + dateTime.getDate();

    // Hours part from the timestamp
    var hours = "0" + dateTime.getHours();
    // Minutes part from the timestamp
    var minutes = "0" + dateTime.getMinutes();
    // Seconds part from the timestamp
    var seconds = "0" + dateTime.getSeconds();

    // Will display date time in YYYY-Mon-DD HH:MM:SS format
    var formattedDateTime = year + "-" + month + "-" + date.substr(-2) + " " + hours.substr(-2) + ":" + minutes.substr(-2) + ":" + seconds.substr(-2);
    
    return formattedDateTime;
}
//
async function accessshare(uplink,access){
    var permission = new storj.Permission(true,true,true,true,0,0);
    var sharePrefix = storj.SharePrefix;
    var sharePrefixListArray = [];
    sharePrefix.bucket = "change-me-to-desired-bucket-name";
    sharePrefix.prefix ="change-me-to-desired-object-prefix";
    sharePrefixListArray.push(sharePrefix);
    await access.access_share(access.access,permission,sharePrefixListArray,sharePrefixListArray.length).then(async (sharedAccess) => {
        //
        console.log("Serilazing share access");
        await sharedAccess.access_serialize(sharedAccess.access).then(async (stringResult) => {
            //
            console.log("Parsing access share...");
            await uplink.parse_access(stringResult).then(async (parsedSharedAccess) => {
                //
                console.log("Opening project using paresed shared access");
                await parsedSharedAccess.open_project(parsedSharedAccess.access).then(async (project) => {
                    //
                    console.log("\nOpened Project using share access");
                    await project.delete_object(project.project,storjConfig.bucketName,storjConfig.uploadPath).then((objectInfo) => {
                        console.log("\nObject Deleted successfully !!");
                        console.log("Object Name : ",objectInfo.key,"Object Size : ",objectInfo.system.content_length);
                    }).catch((err) => {
                        console.log("Failed to delete object on storj V3 network using shared access");
                        console.log(err);
                    });
                    //
                    await project.close_project(project.project).then(() => {
                        console.log("Project Closed !!");
                    }).catch((err) => {
                        console.log("Failed to close project");
                        console.log(err);
                    });
                }).catch((err) => {
                    console.log("Failed to open project using shared access");
                    console.log(err);
                });
            }).catch((err) => {
                console.log("Failed to parse share access");
                console.log(err);
            });
        }).catch((err) => {
            console.log("Failed to serilaze Access");
            console.log(err);
        });
    }).catch((err) => {
        console.log("Failed to create share access");
        console.log(err);
    });
}
//
async function uploadfile(projectResult){
    console.log("Getting Upload Object....");
    //
    var uploadOptions = new storj.UploadOptions();
    //
    uploadOptions.expires = 0;
    //Uploading object on storj V3 network
    await projectResult.upload_object(projectResult.project,storjConfig.bucketName,storjConfig.uploadPath,uploadOptions).then(async (uploadinfo) => {
        console.log(localFullFileName.src, " File: UPLOADED as ", storjConfig.uploadPath, "!");
        var fileHandle = await fs.openSync(localFullFileName.src, "rs");
        var size = {
            file            : `${await fs.statSync(localFullFileName.src).size}`,
            toWrite         : 0,
            actuallyWritten : 0,
            totalWritten    : 0
        };
        //
        var buffer = new Buffer.alloc(BUFFER_SIZE);
        var loop = true;
        var bytesRead = 0;
        while (loop) {
            //
            size.toWrite = size.file - size.totalWritten;
            //
            if (size.toWrite > BUFFER_SIZE) {
                size.toWrite = BUFFER_SIZE;
            } else if (size.toWrite === 0) {
                break;
            }
            //
            bytesRead = await fs.readSync(fileHandle, buffer, 0, size.toWrite, size.totalWritten);
            //
            //Writing data on storj V3 network
            await uploadinfo.upload_write(uploadinfo.upload,buffer,bytesRead).then((writeResult) => {
                size.actuallyWritten = writeResult.bytes_written;
                size.totalWritten +=  size.actuallyWritten;
                if((size.totalWritten>0)&&(size.file>0)){
                    console.log("File Uploaded On Storj  : ",((Number(size.totalWritten)/Number(size.file))*100).toFixed(4)," %");
                }
            }).catch((err) => {
                console.log("Failed to write data on storj V3 network");
                console.log(err);
                loop = false;
            });
            if(size.totalWritten>=size.file){
                break;
            }
        }
        //
        var customMetadataEntry1 =new storj.CustomMetadataEntry();
        customMetadataEntry1.key = "testing";
        customMetadataEntry1.key_length = customMetadataEntry1.key.length;
        customMetadataEntry1.value = "testing1";
        customMetadataEntry1.value_length = customMetadataEntry1.value.length;
        
        var customMetadataEntry2 =new storj.CustomMetadataEntry();
        customMetadataEntry2.key = "value";
        customMetadataEntry2.key_length = customMetadataEntry2.key.length;
        customMetadataEntry2.value = "value1";
        customMetadataEntry2.value_length = customMetadataEntry2.value.length;
        
        var customMetadataEntryArray =  [customMetadataEntry1,customMetadataEntry2];
        var customMetadata = new storj.CustomMetadata();
        customMetadata.count = customMetadataEntryArray.length;
        customMetadata.entries = customMetadataEntryArray;
        //Adding custom meta to upload object
        await uploadinfo.upload_set_custom_metadata(uploadinfo.upload,customMetadata).then(() => {
            console.log("\nCustom Metadata set successfully");
        }).catch((err) => {
            console.log("Failed to set custom metadata");
            console.log(err);
        });
        //Commiting object on storj V3 network
        await uploadinfo.upload_commit(uploadinfo.upload).then(() => {
            console.log("\nObject on storj V3 network successfully");
        }).catch((err) => {
            console.log("Failed to commit object on storj V3 network");
            console.log(err);
        });
        //Fetching info of uploaded object on storj V3 network
        await uploadinfo.upload_info(uploadinfo.upload).then((object) => {
            console.log("\nObject Info");
            console.log("Object Name : ",object.key,"\nObject Size : ",object.system.content_length);
        }).catch((err) => {
            console.log("Failed to fetch information about object");
            console.log(err);
        });
        //
        fs.closeSync(fileHandle);
    }).catch((err) => {
        console.log("Failed to upload object on storj V3");
        console.log(err);
});
}
//
async function downloadfile(projectResult){
    var downloadOptions = new storj.DownloadOptions();
    downloadOptions.offset = 0;
    downloadOptions.length = -1;
    // Downloading file
    console.log("Downloading file");
    await projectResult.download_object(projectResult.project,storjConfig.bucketName,storjConfig.uploadPath,downloadOptions).then(async (downloadresult) => {
        var objectsize =0;
        //
        console.log("Fetching download object info");
        await downloadresult.download_info(downloadresult.download).then((objectInfo) => {
            objectsize = objectInfo.system.content_length;
        }).catch((err) => {
            console.log("Failed to get downloading object info");
            console.log(err);
        });
        //
        var size = { download : 0,
            actuallyWritten : 0,
            totalWritten    : 0
        };
        var buffer = new Buffer.alloc(BUFFER_SIZE);
        //
        var fileHandle = await fs.openSync(localFullFileName.dest, "w");
        //
        var loop = true;
        while(loop) {
            if((objectsize-size.totalWritten>0)&&(objectsize-size.totalWritten)<BUFFER_SIZE){
                buffer = null;
                buffer = new Buffer.alloc(objectsize-size.totalWritten);
            }
            //Reading data from storj V3 network
            await downloadresult.download_read(downloadresult.download,buffer,buffer.length).then(async (bytesread) => {
                size.download = bytesread.bytes_read;
                size.actuallyWritten = await fs.writeSync(fileHandle, buffer, 0, size.downloaded, size.totalWritten);
                size.totalWritten += size.actuallyWritten;
                if(size.actuallyWritten>=objectsize){
                    loop = false;
                }
                if((size.totalWritten>0)&&(objectsize>0)){
                    console.log("File Dowloaded : ",((Number(size.totalWritten)/Number(objectsize))*100).toFixed(4)," %");
                }
            }).catch((err) => {
                console.log("Failed to read data from storj V3 network ");
                console.log(err);
                loop=false;
            });
            
            if (size.totalWritten >= objectsize) {
                break;
            }
        }
        fs.closeSync(fileHandle);
        //Closing download
        await downloadresult.close_download(downloadresult.download).then(() => {
            console.log("Object Downloaded Successfully");
        }).catch((err) => {
            console.log("Failed to download object");
            console.log(err);
        });
    }).catch((err) => {
        console.log("Failed to download file");
        console.log(err);
    });

}

//Connecting to storj network using Satellite Address , Storj API key , Encryption phassphrase
console.log("Getting Access\nSatellite Address : ",storjConfig.satelliteURL,"\nAPI key : ",storjConfig.apiKey,"\nEncryption Passphrase : ",storjConfig.encryptionPassphrase);
libUplink.request_access_with_passphrase(storjConfig.satelliteURL,storjConfig.apiKey,storjConfig.encryptionPassphrase).then((access) => {
    console.log("Access : Granted !!");
    //
    //Opening project on storj V3 network using created access.
    console.log("Opening Storj Project...");
    access.open_project(access.access).then(async (projectResult) => {
        console.log("Desired Storj Project: OPENED!");
        //Fetching bucket info from the storj V3 network
        console.log("Fetching Information About Bucket : ",storjConfig.bucketName);
        //
        await projectResult.stat_bucket(projectResult.project,storjConfig.bucketName).then((bucketInfo) => {
            console.log("\nBucket Information : \n Bucket Name : ",bucketInfo.name,"\n Bucket Created : ",getDateTime(bucketInfo.created));
        }).catch((err) => {
            console.log("Failed to get bucket Info");
            console.log(err);
        });
        //
        //Creating bucket on storj V3 Network
        console.log("Creating Bucket : ",storjConfig.bucketName);
        await projectResult.create_bucket(projectResult.project,storjConfig.bucketName).then((bucketInfo) => {
            console.log("\nBucket Information : \n Bucket Name : ",bucketInfo.name,"\n Bucket Created : ",getDateTime(bucketInfo.created));
        }).catch((err) => {
            console.log("Failed to create bucket on storj V3 network");
            console.log(err);
        });
        //Ensuring bucket exists on storj V3 network
        console.log("Fetching Info of newly created bucket");
        await projectResult.ensure_bucket(projectResult.project,storjConfig.bucketName).then((bucketInfo) => {
            console.log("\nBucket Information : \n Bucket Name : ",bucketInfo.name,"\n Bucket Created : ",getDateTime(bucketInfo.created));
        }).catch((err) => {
            console.log("Failed to fetch bucket Info");
            console.log(err);
        });
        //
        //Listing buckets on storj V3 network
        var listBucketsOptions = new storj.ListBucketsOptions();
        await projectResult.listbuckets(projectResult.project,listBucketsOptions).then(async (bucketListResult) => {
            var bucketList = bucketListResult.bucketList;
            console.log("S.No. \t Created On \t\t Bucket Name");
            console.log("===== \t ========== \t\t ===========");
            var i = 0;
            for(var bucketinfo in bucketList){
                console.log(i," ",getDateTime(bucketList[bucketinfo].created), "\t", bucketList[bucketinfo].name);
                i++;
            }
        }).catch((err) => {
            console.log("Failed to list bucket");
            console.log(err);
        });
        //Uploading file on storj V3 network
        await uploadfile(projectResult);
        //
        //Fetching info of uploaded object
        console.log("Fetching object Info...");
        await projectResult.stat_object(projectResult.project,storjConfig.bucketName,storjConfig.uploadPath).then((objectinfo) => {
            console.log("Object : ",objectinfo.key,"\nSize : ",objectinfo.system.content_length,"\nCreated : ",getDateTime(objectinfo.system.created));
        }).catch((err) => {
            console.log("Failed to fetch object info");
            console.log(err);
        });
        //Listing object on storj V3 network within specified bucket
        console.log("\nListing Object....");
        var listObjectsOptions = new storj.ListObjectsOptions();
        listObjectsOptions.recursive = true;
        listObjectsOptions.custom = true;
        listObjectsOptions.system = true;
        listObjectsOptions.prefix ="change-me-to-desire-prefix";

        await projectResult.list_objects(projectResult.project,storjConfig.bucketName,listObjectsOptions).then((objectlist) => {
            console.log("S.No. \t Created On \t\t\t FileName \t\t FileSize");
            console.log("===== \t ========== \t\t\t =========== \t\t ===========");
            var i = 0;
            for(var objectInfo in objectlist){
                var numb = "0" + (i + 1).toString();
                console.log(numb.substr(-2), "  ", getDateTime(objectlist[objectInfo].system.created), "\t\t", objectlist[objectInfo].key,"\t\t",objectlist[objectInfo].system.content_length);
                i++;
            }
        }).catch((err) => {
            console.log("Error while listing object");
            console.log(err);
        });
        //Downloading object from storj v3 network
        await downloadfile(projectResult,libUplink);
        //Creating share access
        await accessshare(libUplink,access);
        //
        //Delete object from the network
        console.log("Deleting Object...");
        await projectResult.delete_object(projectResult.project,storjConfig.bucketName,storjConfig.uploadPath).then((objectinfo) => {
            console.log("Object ",storjConfig.uploadPath," Deleted");
            console.log("Object Size : ",objectinfo.system.created);
        }).catch((err) => {
            console.log("Failed to delete object");
            console.log(err);
        });
        //
        //Deleting bucket from the storj V3 network
        console.log("Deleting Bucket : ",storjConfig.bucketName);
        await projectResult.delete_bucket(projectResult.project,storjConfig.bucketName).then((bucketInfo) => {
            console.log("\nBucket Deleted : \n Bucket Name : ",bucketInfo.name,"\n Bucket Created : ",getDateTime(bucketInfo.created));
        }).catch((err) => {
            console.log("Failed to delete bucket");
            console.log(err);
        });
        //
        //Closing opened project
        await projectResult.close_project(projectResult.project).then(() => {
            console.log("\nProject closed successfully");
        }).catch((err) => {
            console.log("Failed to close project");
            console.log(err);
        });
    }).catch((err) => {
        console.log("Failed to open project");
        console.log(err);
    });
}).catch((err) => {
    console.log("Failed to get access");
    console.log(err)
});
