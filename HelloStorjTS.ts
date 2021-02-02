const fs = require("fs");

/*
 * Include the Node.js-Storj bindings module
 */
/* eslint-disable */
import storj = require("uplink-nodejs");
const BUFFER_SIZE = 80000,
    checkFileSize = 0,
    defaultConversion = 100,
    defaultFileRead = 0,
    defaultValue = 0,
    fixedValue = 4,
    //
    incrementValue = 1,
    initialValue = 0,
    //
    libUplink = new storj.Uplink(),
    // File names to be uploaded or downloaded as
    localFullFileName = {
        "dest": "change-me-to-destination-file-name-at-local-system",
        "src": "change-me-to-destination-file-name-at-local-system"
    },
    // Object for all the function for uplink

    /*
     *
     * Demo Storj (V3) configuration
     */
    storjConfig = {
        "apiKey": "change-me-to-the-api-key-created-in-satellite-gui",
        "bucketName": "change-me-to-desired-bucket-name",
        "encryptionPassphrase": "you'll never guess this",
        "satelliteURL": "change-me-to-desired-satellite-url",
        "uploadPath": "optionalpath/requiredfilename"
    },
    //
    subString = -2,
    // Object for all error for uplink
    uplinkError = storj.errorhandle,

    /**
     * Convert unixTimestamp to other date format
     * @param {number} unixTimestamp Date in unixtimestamp format
     * @returns {date} in format YYYY-MM-DD
     */
    getDateTime = (unixTimestamp: number) => {

        const converionValue = 1000,
            dateTime = new Date(unixTimestamp * converionValue),
            date = `0${dateTime.getDate()}`,
            // Hours part from the timestamp
            hours = `0${dateTime.getHours()}`,
            // Minutes part from the timestamp
            minutes = `0${dateTime.getMinutes()}`,
            months = [
                "Jan",
                "Feb",
                "Mar",
                "Apr",
                "May",
                "Jun",
                "Jul",
                "Aug",
                "Sep",
                "Oct",
                "Nov",
                "Dec"
            ],
            month = months[dateTime.getMonth()],
            // Seconds part from the timestamp
            seconds = `0${dateTime.getSeconds()}`,
            year = dateTime.getFullYear(),
            // Will display date time in YYYY-Mon-DD HH:MM:SS format
            formattedDateTime = `${year}-${month}-${date.substr(subString)} ${hours.substr(subString)}:${minutes.substr(subString)}:${seconds.substr(subString)}`;

        return formattedDateTime;

    },
    //
    /**
     * Creates access share of the object on storj V3 network
     * @param {uplink} uplink uplink object
     * @param {access} access access object
     * @returns {void}
     */
    accessshare = async (uplink: any, access: any) => {

        const permission = new storj.Permission(
                true,
                true,
                true,
                false,
                defaultValue,
                defaultValue
            ),
            sharePrefix = new storj.SharePrefix(),
            sharePrefixListArray = [];

        sharePrefix.bucket = "change-me-to-desired-bucket-name";
        sharePrefix.prefix = "change-me-to-desired-object-prefix-with-/";
        sharePrefixListArray.push(sharePrefix);
        await access.share(
            permission,
            sharePrefixListArray,
            sharePrefixListArray.length
        ).then(async (sharedAccess: any) => {

            //
            console.log("Serilazing share access");
            await sharedAccess.serialize().then(async (stringResult: any) => {

                //
                console.log("Parsing access share...");
                await uplink.parseAccess(stringResult).then(async (parsedSharedAccess: any) => {

                    //
                    console.log("\nDeriving encryption key");
                    //
                    const encryption = await libUplink.uplinkDeriveEncryptionKey(
                        "test",
                        [
                            4,
                            5,
                            6
                        ]
                    ).catch((error: any) => {

                        console.log("Failed to derive encryption key");
                        console.log(error);

                    });
                    //

                    console.log("Over riding encryption key");
                    //
                    await parsedSharedAccess.overrideEncryptionKey(
                        sharePrefix.bucket,
                        sharePrefix.prefix,
                        encryption.encryption_key
                    ).catch((err) => {

                        console.log("Failed to over write encryption key");
                        console.log(err);

                    });
                    //
                    console.log("Opening project using paresed shared access");
                    await parsedSharedAccess.openProject().then(async (project: any) => {

                        //
                        console.log("\nOpened Project using share access");
                        await project.deleteObject(
                            storjConfig.bucketName,
                            storjConfig.uploadPath
                        ).then((objectInfo: any) => {

                            console.log("\nObject Deleted successfully !!");
                            console.log(
                                "Object Name : ",
                                objectInfo.key,
                                "Object Size : ",
                                objectInfo.system.content_length
                            );

                        }).
                            catch((err: any) => {

                                console.log("Failed to delete object on storj V3 network using shared access");
                                console.log(err);

                            });
                        //
                        await project.close().then(() => {

                            console.log("Project Closed !!");

                        }).
                            catch((err: any) => {

                                console.log("Failed to close project");
                                console.log(err);

                            });

                    }).
                        catch((err: any) => {

                            console.log("Failed to open project using shared access");
                            console.log(err);

                        });

                }).
                    catch((err: any) => {

                        console.log("Failed to parse share access");
                        console.log(err);

                    });

            }).
                catch((err: any) => {

                    console.log("Failed to serilaze Access");
                    console.log(err);

                });

        }).
            catch((err: any) => {

                console.log("Failed to create share access");
                console.log(err);

            });

    },

    /**
     * Downloads file from storj V3 network
     * @param {project} project project handle
     * @returns {null} null
     */
    downloadfile = async (project : any) => {

        const downloadOptions = new storj.DownloadOptions();

        downloadOptions.offset = defaultFileRead;
        downloadOptions.length = -1;
        // Downloading file
        console.log("Downloading file");
        await project.downloadObject(
            storjConfig.bucketName,
            storjConfig.uploadPath,
            downloadOptions
        ).then(async (download :any) => {

            let objectsize = defaultFileRead;
            //

            console.log("Fetching download object info");
            await download.info().then((objectInfo: any) => {

                objectsize = objectInfo.system.content_length;

            }).
                catch((err: any) => {

                    console.log("Failed to get downloading object info");
                    console.log(err);

                });
            //
            let buffer = Buffer.alloc(BUFFER_SIZE),
                //
                fileHandle = await fs.openSync(
                    localFullFileName.dest,
                    "w"
                ),
                //
                loop = true;
            const size = {"actuallyWritten": defaultFileRead,
                "downloaded": defaultFileRead,
                "totalWritten": defaultFileRead};

            while (loop) {

                if (objectsize - size.totalWritten > checkFileSize && objectsize - size.totalWritten < BUFFER_SIZE) {

                    buffer = Buffer.alloc(objectsize - size.totalWritten);

                }
                // Reading data from storj V3 network
                await download.read(
                    buffer,
                    buffer.length
                ).then(async (bytesread : any) => {

                    size.downloaded = bytesread.bytes_read;
                    size.actuallyWritten = await fs.writeSync(
                        fileHandle,
                        buffer,
                        initialValue,
                        size.downloaded,
                        size.totalWritten
                    );
                    size.totalWritten += size.actuallyWritten;
                    if (size.actuallyWritten >= objectsize) {

                        loop = false;

                    }
                    if (size.totalWritten > checkFileSize && objectsize > checkFileSize) {

                        console.log(
                            "File Dowloaded : ",
                            (Number(size.totalWritten) / Number(objectsize) * defaultConversion).toFixed(fixedValue),
                            " %"
                        );

                    }

                }).
                    catch((err: any) => {

                        console.log("Failed to read data from storj V3 network ");
                        console.log(err);
                        loop = false;

                    });

                if (size.totalWritten >= objectsize) {

                    break;

                }

            }
            fs.closeSync(fileHandle);
            // Closing download
            await download.close().then(() => {

                console.log("Object Downloaded Successfully");

            }).
                catch((err: any) => {

                    console.log("Failed to download object");
                    console.log(err);

                });

        }).
            catch((err: any) => {

                console.log("Failed to download file");
                console.log(err);

            });

    },

    /**
     * Uploads file on storj V3 network
     * @param {project} project project handle
     * @returns {void}
     */
    uploadfile = async (project: any) => {

        console.log("Getting Upload Object....");
        //
        const uploadOptions = new storj.UploadOptions();
        //

        uploadOptions.expires = 0;
        // Uploading object on storj V3 network
        await project.uploadObject(
            storjConfig.bucketName,
            storjConfig.uploadPath,
            uploadOptions
        ).then(async (upload: any) => {

            console.log(
                localFullFileName.src,
                " File: UPLOADED as ",
                storjConfig.uploadPath,
                "!"
            );
            let buffer = Buffer.alloc(BUFFER_SIZE),
                bytesRead = initialValue,
                fileHandle = await fs.openSync(
                    localFullFileName.src,
                    "rs"
                ),
                loop = true;
            const size = {
                "actuallyWritten": defaultFileRead,
                "file": Number(`${await fs.statSync(localFullFileName.src).size}`),
                "toWrite": defaultFileRead,
                "totalWritten": defaultFileRead
            };

            while (loop) {

                //
                size.toWrite = size.file - size.totalWritten;
                //
                if (size.toWrite > BUFFER_SIZE) {

                    size.toWrite = BUFFER_SIZE;

                } else if (size.toWrite === checkFileSize) {

                    break;

                }
                //
                bytesRead = await fs.readSync(
                    fileHandle,
                    buffer,
                    defaultFileRead,
                    size.toWrite,
                    size.totalWritten
                );

                /*
                 *
                 * Writing data on storj V3 network
                 */
                await upload.write(
                    buffer,
                    bytesRead
                ).then((writeResult: any) => {

                    size.actuallyWritten = writeResult.bytes_written;
                    size.totalWritten += size.actuallyWritten;
                    if (size.totalWritten > checkFileSize && size.file > checkFileSize) {

                        console.log(
                            "File Uploaded On Storj  : ",
                            (Number(size.totalWritten) / Number(size.file) * defaultConversion).toFixed(fixedValue),
                            " %"
                        );

                    }

                }).
                    catch((err: any) => {

                        console.log("Failed to write data on storj V3 network");
                        console.log(err);
                        loop = false;

                    });
                if (size.totalWritten >= size.file) {

                    break;

                }

            }
            //
            const customMetadataEntry1 = new storj.CustomMetadataEntry(),
                customMetadataEntry2 = new storj.CustomMetadataEntry();

            customMetadataEntry1.key = "testing";
            customMetadataEntry1.key_length = customMetadataEntry1.key.length;
            customMetadataEntry1.value = "testing1";
            customMetadataEntry1.value_length = customMetadataEntry1.value.length;

            customMetadataEntry2.key = "value";
            customMetadataEntry2.key_length = customMetadataEntry2.key.length;
            customMetadataEntry2.value = "value1";
            customMetadataEntry2.value_length = customMetadataEntry2.value.length;

            const customMetadata = new storj.CustomMetadata(),
                customMetadataEntryArray = [
                    customMetadataEntry1,
                    customMetadataEntry2
                ];

            customMetadata.count = customMetadataEntryArray.length;
            customMetadata.entries = customMetadataEntryArray;
            // Adding custom meta to upload object
            await upload.setCustomMetadata(customMetadata).then(() => {

                console.log("\nCustom Metadata set successfully");

            }).
                catch((err: any) => {

                    console.log("Failed to set custom metadata");
                    console.log(err);

                });
            // Commiting object on storj V3 network
            await upload.commit().then(() => {

                console.log("\nObject on storj V3 network successfully");

            }).
                catch((err: any) => {

                    console.log("Failed to commit object on storj V3 network");
                    console.log(err);

                });
            // Fetching info of uploaded object on storj V3 network
            await upload.info().then((object: any) => {

                console.log("\nObject Info");
                console.log(
                    "Object Name : ",
                    object.key,
                    "\nObject Size : ",
                    object.system.content_length
                );

            }).
                catch((err: any) => {

                    console.log("Failed to fetch information about object");
                    console.log(err);

                });
            //
            fs.closeSync(fileHandle);

        }).
            catch((err: any) => {

                console.log("Failed to upload object on storj V3");
                console.log(err);

            });

    };

/*
 *
 * Connecting to storj network using Satellite Address , Storj API key , Encryption phassphrase
 */
console.log(
    "Getting Access\nSatellite Address : ",
    storjConfig.satelliteURL,
    "\nAPI key : ",
    storjConfig.apiKey,
    "\nEncryption Passphrase : ",
    storjConfig.encryptionPassphrase
);
libUplink.requestAccessWithPassphrase(
    storjConfig.satelliteURL,
    storjConfig.apiKey,
    storjConfig.encryptionPassphrase
).then((access: any) => {

    console.log("Access : Granted !!");
    // Opening project on storj V3 network using created access.
    console.log("Opening Storj Project...");
    access.openProject().then(async (project: any) => {

        console.log("Desired Storj Project: OPENED!");
        // Fetching bucket info from the storj V3 network
        console.log(
            "Fetching Information About Bucket : ",
            storjConfig.bucketName
        );
        //
        await project.statBucket(storjConfig.bucketName).then((bucketInfo: any) => {

            console.log(
                "\nBucket Information : \n Bucket Name : ",
                bucketInfo.name,
                "\n Bucket Created : ",
                getDateTime(bucketInfo.created)
            );

        }).
            catch((err: any) => {

                console.log("Failed to get bucket Info");
                console.log(err);

            });

        /*
         *
         * Creating bucket on storj V3 Network
         */
        console.log(
            "\nCreating Bucket : ",
            storjConfig.bucketName
        );
        await project.createBucket(storjConfig.bucketName).then((bucketInfo: any) => {

            console.log(
                "\nBucket Information : \n Bucket Name : ",
                bucketInfo.name,
                "\n Bucket Created : ",
                getDateTime(bucketInfo.created)
            );

        }).
            catch((err: any) => {

                console.log("Failed to create bucket on storj V3 network");
                console.log(err);

            });
        // Ensuring bucket exists on storj V3 network
        console.log("\nFetching Info of newly created bucket");
        await project.ensureBucket(storjConfig.bucketName).then((bucketInfo: any) => {

            console.log(
                "\nBucket Information : \n Bucket Name : ",
                bucketInfo.name,
                "\n Bucket Created : ",
                getDateTime(bucketInfo.created)
            );

        }).
            catch((err: any) => {

                console.log("Failed to fetch bucket Info");
                console.log(err);

            });

        /*
         *
         * Listing buckets on storj V3 network
         */
        const listBucketsOptions = new storj.ListBucketsOptions();

        await project.listBuckets(listBucketsOptions).then((bucketListResult: any) => {

            const {bucketList} = bucketListResult;

            console.log("S.No. \t Created On \t\t Bucket Name");
            console.log("===== \t ========== \t\t ===========");
            let index = initialValue;

            for (const bucketinfo in bucketList) {

                if (bucketList[bucketinfo]) {

                    console.log(
                        index,
                        " ",
                        getDateTime(bucketList[bucketinfo].created),
                        "\t",
                        bucketList[bucketinfo].name
                    );
                    index += incrementValue;

                }

            }

        }).
            catch((err: any) => {

                console.log("Failed to list bucket");
                console.log(err);

            });
        // Uploading file on storj V3 network
        await uploadfile(project);

        /*
         *
         * Fetching info of uploaded object
         */
        console.log("Fetching object Info...");
        await project.statObject(
            storjConfig.bucketName,
            storjConfig.uploadPath
        ).then((objectinfo: any) => {

            console.log(
                "Object : ",
                objectinfo.key,
                "\nSize : ",
                objectinfo.system.content_length,
                "\nCreated : ",
                getDateTime(objectinfo.system.created)
            );

        }).
            catch((err: any) => {

                console.log("Failed to fetch object info");
                console.log(err);

            });
        // Listing object on storj V3 network within specified bucket
        console.log("\nListing Object....");
        const listObjectsOptions = new storj.ListObjectsOptions();

        listObjectsOptions.recursive = true;
        listObjectsOptions.custom = true;
        listObjectsOptions.system = true;
        listObjectsOptions.prefix = "change-me-to-desired-object-prefix-with-/";

        await project.listObjects(
            storjConfig.bucketName,
            listObjectsOptions
        ).then((objectlist: any) => {

            console.log("S.No. \t Created On \t\t\t FileName \t\t FileSize");
            console.log("===== \t ========== \t\t\t =========== \t\t ===========");
            let index = initialValue;

            for (const objectInfo in objectlist) {

                if (objectlist[objectInfo]) {

                    const numb = `0${(index + incrementValue).toString()}`;

                    console.log(
                        numb.substr(subString),
                        "  ",
                        getDateTime(objectlist[objectInfo].system.created),
                        "\t\t",
                        objectlist[objectInfo].key,
                        "\t\t",
                        objectlist[objectInfo].system.content_length
                    );
                    index += incrementValue;

                }

            }

        }).
            catch((err: any) => {

                console.log("Error while listing object");
                console.log(err);

            });
        // Downloading object from storj v3 network
        await downloadfile(project);
        // Creating share access
        await accessshare(
            libUplink,
            access
        );

        /*
         *
         * Deleting bucket from the storj V3 network
         */
        console.log(
            "Deleting Bucket : ",
            storjConfig.bucketName
        );
        await project.deleteBucket(storjConfig.bucketName).then((bucketInfo: any) => {

            console.log(
                "\nBucket Deleted : \n Bucket Name : ",
                bucketInfo.name,
                "\n Bucket Created : ",
                getDateTime(bucketInfo.created)
            );

        }).
            catch(async (err: any) => {

                // Checking error type
                if (err instanceof uplinkError.BucketNotEmptyError) {

                    // Delete object from the network
                    console.log("Bucket is not empty !!\nDeleting object from storj V3 bucket...");
                    await project.deleteObject(
                        storjConfig.bucketName,
                        storjConfig.uploadPath
                    ).then((objectinfo: any) => {

                        console.log(
                            "Object ",
                            storjConfig.uploadPath,
                            " Deleted"
                        );
                        console.log(
                            "Object Size : ",
                            objectinfo.system.content_length
                        );

                    }).
                        catch((errDeleteObject: any) => {

                            console.log("Failed to delete object");
                            console.log(errDeleteObject);

                        });

                } else {

                    console.log("Failed to delete bucket");
                    console.log(err);

                }

            });

        /*
         *
         *
         */
        console.log(
            "Deleting Bucket : ",
            storjConfig.bucketName
        );
        await project.deleteBucket(storjConfig.bucketName).then((bucketInfo: any) => {

            console.log(
                "\nBucket Deleted : \n Bucket Name : ",
                bucketInfo.name,
                "\n Bucket Created : ",
                getDateTime(bucketInfo.created)
            );

        }).
            catch((err: any) => {

                console.log("Failed to delete bucket");
                console.log(err);

            });

        /*
         *
         *
         * Closing opened project
         */
        await project.close().then(() => {

            console.log("\nProject closed successfully");

        }).
            catch((errCloseProject: any) => {

                console.log("Failed to close project");
                console.log(errCloseProject);

            });

    }).
        catch((err: any) => {

            console.log("Failed to open project");
            console.log(err);

        });

}).
    catch((err: any) => {

        console.log("Failed to get access");
        console.log(err);

    });
/* eslint-enable */
