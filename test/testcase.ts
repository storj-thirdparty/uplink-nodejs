// Requiring modules
const {expect} = require("chai");
const chai = require("chai");
const storj = require("../dist/uplink.js"),
    libUplink = new storj.Uplink();
const fs = require("fs"),

    /*
     *
     * Storj V3
     */
    storjConfig = {
        "apiKey": "",
        "satelliteURL": "12EayRS2V1kEsWESU9QMRseFhdxYxKicsiFmxrsLZHeLUtdps3S@us-central-1.tardigrade.io:7777",
        "encryptionPassphrase": "test",
        "bucketName": "uplinknodejstesttypescript",
        "uploadPath": "filepath/sample.txt"
    };

/*
 *
 *
 */
/* eslint-disable */
function readAPIKey () {

    try {
        //
        const filename = "secret.txt";
        if (fs.existsSync(filename)) {
            storjConfig.apiKey = fs.readFileSync(
                filename,
                {"encoding": "utf8",
                    "flag": "r"}
            );

        } else {

            throw new Error("File does not exits : "+__dirname+"/"+filename);

        }

    } catch (e) {

        throw e.message;

    }

}
readAPIKey();

// String to be uploaded on storj V3 network
const str = "Hello Storj",
    // Buffer size
    BUFFER_SIZE = 80000,
    //
    should = chai.should();
// Test case of deleting object

function deleteObject (project) {

    //
    describe(
        "Delete Object",
        () => new Promise<boolean>(async (resolve, reject) => {

            it(
                "Delete Object from storj V3 network",
                async () => new Promise<boolean>(async (resolve, reject) => {

                    await project.deleteObject(
                        storjConfig.bucketName,
                        storjConfig.uploadPath
                    ).then((objectinfo) => {

                        describe(
                            "Delete object result",
                            () => {

                                it(
                                    "Should have object info ",
                                    () => {

                                        objectinfo.should.be.a("object");

                                    }
                                );

                            }
                        );
                        resolve(true);

                    }).
                        catch((err) => {

                            reject(err);

                        });
                    resolve(true);

                })
            );
            resolve(true);

        })
    );

}

// Test case for deleting bucket
function deleteBucket (project) {

    // Test case
    describe(
        "Delete Bucket",
        () => new Promise<boolean>(async (resolve, reject) => {

            it(
                "Delete Bucket",
                async () => new Promise<boolean>(async (resolve, reject) => {

                    await project.deleteBucket(storjConfig.bucketName).then((bucketInfo) => {

                        resolve(true);

                    }).
                        catch((err) => {

                            reject(err);

                        });
                    resolve(true);

                })
            );
            resolve(true);

        })
    );

}

// Close project testcases
function closeProject (project) {

    // Test case
    describe(
        "Close project",
        () => new Promise<boolean>(async (resolve, reject) => {

            it(
                "Close project",
                async () => new Promise<boolean>(async (resolve, reject) => {

                    await project.close().then(() => {

                        resolve(true);

                    }).
                        catch((err) => {

                            reject(err);

                        });
                    resolve(true);

                })
            );
            resolve(true);

        })
    );

}

// Parsed access test cases
function parsedAccess (stringResult) {

    // Test case
    describe(
        "Parsed",
        () => new Promise<boolean>(async (resolve, reject) => {

            it(
                "Access Shared",
                async () => new Promise<boolean>(async (resolve, reject) => {

                    await libUplink.parseAccess(stringResult).then(async (parsedAccess) => {

                        describe(
                            "parsed access share Result",
                            () => {

                                it(
                                    "should have prase access",
                                    () => {

                                        parsedAccess.access._handle.should.be.a("number");

                                    }
                                );

                            }
                        );

                        describe(
                            "parsed access share Result",
                            () => {

                                it(
                                    "should have access serialize function",
                                    () => {

                                        parsedAccess.should.have.property("serialize");

                                    }
                                );

                            }
                        );

                        describe(
                            "parsed access share Result",
                            () => {

                                it(
                                    "should have open project function",
                                    () => {

                                        parsedAccess.should.have.property("openProject");

                                    }
                                );

                            }
                        );

                        describe(
                            " parsed access share Result",
                            () => {

                                it(
                                    "should have config open project function",
                                    () => {

                                        parsedAccess.should.have.property("configOpenProject");

                                    }
                                );

                            }
                        );

                        describe(
                            "parsed access share Result",
                            () => {

                                it(
                                    "should have access share function",
                                    () => {

                                        parsedAccess.should.have.property("share");

                                    }
                                );

                            }
                        );

                        resolve(true);

                    }).
                        catch((err) => {

                            reject(err);

                        });
                    resolve(true);

                })
            );
            resolve(true);

        })
    );

}
// Access serialiazed testcases
function serializeAccess (sharedAccess) {

    describe(
        "Access Shared",
        () => new Promise<boolean>(async (resolve, reject) => {

            it(
                "Access Shared",
                async () => new Promise<boolean>(async (resolve, reject) => {

                    await sharedAccess.serialize(sharedAccess.access).then(async (stringResult) => {

                        parsedAccess(stringResult);
                        resolve(true);

                    }).
                        catch((err) => {

                            reject(err);

                        });
                    resolve(true);

                })
            );
            resolve(true);

        })
    );

}

// Access share testcases
function accessShare (access) {

    describe(
        "Access Shared",
        () => new Promise<boolean>(async (resolve, reject) => {

            it(
                "Access Shared",
                async () => new Promise<boolean>(async (resolve, reject) => {

                    const permission = new storj.Permission(
                            true,
                            true,
                            true,
                            true,
                            0,
                            0
                        ),
                        sharePrefix = storj.SharePrefix,
                        sharePrefixListArray = [];

                    sharePrefix.bucket = storjConfig.bucketName;
                    sharePrefix.prefix = "filepath/";
                    sharePrefixListArray.push(sharePrefix);
                    await access.share(
                        permission,
                        sharePrefixListArray,
                        sharePrefixListArray.length
                    ).then(async (sharedAccess) => {

                        describe(
                            "access share Result",
                            () => {

                                it(
                                    "should have share access",
                                    () => {

                                        sharedAccess.access._handle.should.be.a("number");

                                    }
                                );

                            }
                        );
                        describe(
                            "access share Result",
                            () => {

                                it(
                                    "should have access serialize function",
                                    () => {

                                        sharedAccess.should.have.property("serialize");

                                    }
                                );

                            }
                        );
                        describe(
                            "access share Result",
                            () => {

                                it(
                                    "should have open project function",
                                    () => {

                                        sharedAccess.should.have.property("openProject");

                                    }
                                );

                            }
                        );
                        describe(
                            "access share Result",
                            () => {

                                it(
                                    "should have config open project function",
                                    () => {

                                        sharedAccess.should.have.property("configOpenProject");

                                    }
                                );

                            }
                        );
                        describe(
                            "access share Result",
                            () => {

                                it(
                                    "should have access share function",
                                    () => {

                                        sharedAccess.should.have.property("share");

                                    }
                                );

                            }
                        );
                        //
                        serializeAccess(sharedAccess);
                        resolve(true);

                    }).
                        catch((err) => {

                            reject(err);

                        });
                    resolve(true);

                })
            );
            resolve(true);

        })
    );

}

// Download read testcases
function downloadRead (downloadresult) {

    describe(
        "Download Result Function",
        () => new Promise<boolean>(async (resolve, reject) => {

            it(
                "Download",
                async () => new Promise<boolean>(async (resolve, reject) => {

                    const buffer = Buffer.alloc(BUFFER_SIZE);

                    await downloadresult.read(
                        buffer,
                        buffer.length
                    ).then(async (bytesread) => {

                        describe(
                            "Close Download",
                            () => {

                                it(
                                    "Close download function",
                                    async () => new Promise<boolean>(async (resolve, reject) => {

                                        await downloadresult.close(downloadresult.download).then(() => {
                                            //
                                        }).
                                            catch((err) => {

                                                reject(err);

                                            });
                                        resolve(true);

                                    })
                                );

                            }
                        );
                        //
                        resolve(true);

                    }).
                        catch((err) => {

                            reject(err);

                        });
                    resolve(true);

                })
            );
            resolve(true);

        })
    );

}

// Download object testcases
function downloadObject (project) {

    describe(
        "Download Object Function",
        () => new Promise<boolean>(async (resolve, reject) => {

            it(
                "Download",
                async () => new Promise<boolean>(async (resolve, reject) => {

                    const downloadOptions = new storj.DownloadOptions();

                    downloadOptions.offset = 0;
                    downloadOptions.length = -1;
                    await project.downloadObject(
                        storjConfig.bucketName,
                        storjConfig.uploadPath,
                        downloadOptions
                    ).then(async (downloadresult) => {

                        const buffer = Buffer.alloc(BUFFER_SIZE);

                        describe(
                            "download object Result",
                            () => {

                                it(
                                    "Should get download handle",
                                    () => {

                                        expect(downloadresult.download._handle).to.be.a("number");

                                    }
                                );

                            }
                        );
                        describe(
                            "download object Result",
                            () => {

                                it(
                                    "Should have download read function ",
                                    () => {

                                        downloadresult.should.have.property("read");

                                    }
                                );

                            }
                        );
                        describe(
                            "download object Result",
                            () => {

                                it(
                                    "Should have download info function ",
                                    () => {

                                        downloadresult.should.have.property("info");

                                    }
                                );

                            }
                        );
                        describe(
                            "download object Result",
                            () => {

                                it(
                                    "Should have close download function ",
                                    () => {

                                        downloadresult.should.have.property("close");

                                    }
                                );

                            }
                        );
                        downloadRead(downloadresult);
                        //
                        deleteObject(project);
                        //
                        deleteBucket(project);
                        //
                        closeProject(project);
                        //
                        resolve(true);

                    }).
                        catch((err) => {

                            reject(err);

                        });
                    resolve(true);

                })
            );
            resolve(true);

        })
    );

}
//
function listObject (project) {

    describe(
        "list object function",
        () => new Promise<boolean>(async (resolve, reject) => {

            it(
                "List Object function",
                async () => new Promise<boolean>(async (resolve, reject) => {

                    const listObjectsOptions = new storj.ListObjectsOptions();

                    await project.listObjects(
                        storjConfig.bucketName,
                        listObjectsOptions
                    ).then(async (objectlist) => {

                        describe(
                            "list object Result",
                            () => {

                                it(
                                    "Should get list of object",
                                    () => {

                                        objectlist.should.be.a("object");

                                    }
                                );

                            }
                        );
                        resolve(true);

                    }).
                        catch((err) => {

                            reject(err);

                        });
                    resolve(true);

                })
            );
            resolve(true);

        })
    );

}
//
function statObject (project) {

    describe(
        "Download Object Function",
        () => new Promise<boolean>(async (resolve, reject) => {

            it(
                "Download",
                async () => new Promise<boolean>(async (resolve, reject) => {

                    await project.statObject(
                        storjConfig.bucketName,
                        storjConfig.uploadPath
                    ).then((objectinfo) => {

                        //
                        describe(
                            "stat object Result ",
                            () => {

                                it(
                                    "Should get information of object",
                                    () => {

                                        objectinfo.should.be.a("object");

                                    }
                                );

                            }
                        );
                        resolve(true);

                    }).
                        catch((err) => {

                            reject(err);

                        });
                    resolve(true);

                })
            );
            resolve(true);

        })
    );

}

/*
 *
 * Upload write test cases
 */
function uploadWrite (upload, project, accessResult) {

    describe(
        "Upload Write",
        () => new Promise<boolean>(async (resolve, reject) => {

            it(
                "Upload Write",
                async () => new Promise<boolean>(async (resolve, reject) => {

                    const buf = Buffer.from(
                            str,
                            "utf-8"
                        ),
                        bytesRead = buf.write(
                            str,
                            0,
                            buf.length,
                            "utf-8"
                        );

                    await upload.write(
                        buf,
                        bytesRead
                    ).then(async (upload) => {

                        describe(
                            "upload write Result",
                            () => {

                                it(
                                    "Should have bytes written ",
                                    () => {

                                        expect(upload.bytes_written).to.be.a("number");

                                    }
                                );

                            }
                        );
                        resolve(true);

                    }).
                        catch((err) => {

                            reject(err);

                        });
                    //
                    const customMetadataEntry1 = new storj.CustomMetadataEntry();

                    customMetadataEntry1.key = "testing";
                    customMetadataEntry1.key_length = customMetadataEntry1.key.length;
                    customMetadataEntry1.value = "testing1";
                    customMetadataEntry1.value_length = customMetadataEntry1.value.length;

                    const customMetadataEntry2 = new storj.CustomMetadataEntry();

                    customMetadataEntry2.key = "value";
                    customMetadataEntry2.key_length = customMetadataEntry2.key.length;
                    customMetadataEntry2.value = "value1";
                    customMetadataEntry2.value_length = customMetadataEntry2.value.length;

                    const customMetadataEntryArray = [
                            customMetadataEntry1,
                            customMetadataEntry2
                        ],
                        customMetadata = new storj.CustomMetadata();

                    customMetadata.count = customMetadataEntryArray.length;
                    customMetadata.entries = customMetadataEntryArray;
                    //
                    describe(
                        "upload set custom metadata",
                        () => {

                            it(
                                "Upload set custom metadata function",
                                async () => new Promise<boolean>(async (resolve, reject) => {

                                    await upload.setCustomMetadata(customMetadata).then(() => {

                                    }).
                                        catch((err) => {

                                            reject(err);

                                        });
                                    resolve(true);

                                })
                            );

                        }
                    );
                    //
                    describe(
                        "upload commit",
                        () => {

                            it(
                                "Upload commit function",
                                async () => new Promise<boolean>(async (resolve, reject) => {

                                    await upload.commit().then(() => {

                                    }).
                                        catch((err) => {

                                            reject(err);

                                        });
                                    resolve(true);

                                })
                            );

                        }
                    );
                    //
                    describe(
                        "upload info",
                        async () => {

                            it(
                                "Upload info function",
                                async () => new Promise<boolean>(async (resolve, reject) => {

                                    await upload.info().then((object) => {

                                        it(
                                            "should have object",
                                            () => {

                                            }
                                        );

                                    }).
                                        catch((err) => {

                                            reject(err);

                                        });
                                    resolve(true);

                                })
                            );

                        }
                    );
                    //
                    downloadObject(project);
                    //
                    listObject(project);
                    //
                    statObject(project);
                    //
                    accessShare(accessResult);
                    //
                    resolve(true);

                })
            );
            resolve(true);

        })
    );

}

// Upload object test cases
function uploadObject (project, accessResult) {

    describe(
        "Upload Object",
        () => new Promise<boolean>(async (resolve, reject) => {

            it(
                "Upload Object",
                async () => new Promise<boolean>(async (resolve, reject) => {

                    const uploadOptions = new storj.UploadOptions();

                    uploadOptions.expires = 0;
                    await project.uploadObject(
                        storjConfig.bucketName,
                        storjConfig.uploadPath,
                        uploadOptions
                    ).then(async (upload) => {

                        describe(
                            "upload Result",
                            () => {

                                it(
                                    "Should have upload",
                                    () => {

                                        expect(upload.upload._handle).to.be.a("number");

                                    }
                                );

                            }
                        );
                        //
                        describe(
                            "upload Result",
                            () => {

                                it(
                                    "Should have upload write function",
                                    () => {

                                        upload.should.have.property("write");

                                    }
                                );

                            }
                        );
                        //
                        describe(
                            "upload Result",
                            () => {

                                it(
                                    "Should have upload commit function",
                                    () => {

                                        upload.should.have.property("commit");

                                    }
                                );

                            }
                        );
                        //
                        describe(
                            "upload Result",
                            () => {

                                it(
                                    "Should have upload info function",
                                    () => {

                                        upload.should.have.property("info");

                                    }
                                );

                            }
                        );
                        //
                        describe(
                            "upload Result",
                            () => {

                                it(
                                    "Should have upload abort function",
                                    () => {

                                        upload.should.have.property("abort");

                                    }
                                );

                            }
                        );
                        //
                        describe(
                            "upload Result",
                            () => {

                                it(
                                    "Should have upload set custom metadata function",
                                    () => {

                                        upload.should.have.property("setCustomMetadata");

                                    }
                                );

                            }
                        );
                        //
                        uploadWrite(
                            upload,
                            project,
                            accessResult
                        );
                        //
                        resolve(true);
                        //

                    }).
                        catch((err) => {

                            reject(err);

                        });
                    resolve(true);

                })
            );
            resolve(true);

        })
    );

}

// List bucket test cases
function listBucket (project) {

    describe(
        "List Bucket",
        () => new Promise<boolean>(async (resolve, reject) => {

            it(
                "Listing Bucket",
                async () => new Promise<boolean>(async (resolve, reject) => {

                    const listBucketsOptions = new storj.ListBucketsOptions();

                    await project.listBuckets(listBucketsOptions).then(async (bucketListResult) => {

                        describe(
                            "list bucket Result",
                            () => {

                                it(
                                    "should have information",
                                    () => {

                                        bucketListResult.should.be.a("object");

                                    }
                                );

                            }
                        );
                        resolve(true);

                    }).
                        catch((err) => {

                            reject(err);

                        });
                    resolve(true);

                })
            );
            resolve(true);

        })
    );

}

// Ensured Bucket testcases
function ensureBucket (project) {

    describe(
        "Ensure Bucket",
        () => new Promise<boolean>(async (resolve, reject) => {

            it(
                "Ensure Bucket",
                async () => new Promise<boolean>(async (resolve, reject) => {

                    await project.ensureBucket(storjConfig.bucketName).then(async (bucketInfo) => {

                        describe(
                            "ensure bucket Result",
                            () => {

                                it(
                                    "should have information",
                                    () => {

                                        bucketInfo.name.should.equal(storjConfig.bucketName);

                                    }
                                );

                            }
                        );
                        resolve(true);

                    }).
                        catch((err) => {

                            reject(err);

                        });
                    resolve(true);

                })
            );
            resolve(true);

        })
    );

}

// Stat Bucket test cases
function statBucket (project) {

    describe(
        "Stat Bucket",
        () => new Promise<boolean>(async (resolve, reject) => {

            it(
                "Stat Bucket",
                async () => new Promise<boolean>(async (resolve, reject) => {

                    await project.statBucket(storjConfig.bucketName).then(async (bucketInfo) => {

                        describe(
                            "stat bucket Result",
                            () => {

                                it(
                                    "should have information",
                                    () => {

                                        bucketInfo.name.should.equal(storjConfig.bucketName);

                                    }
                                );

                            }
                        );

                        resolve(true);

                    }).
                        catch((err) => {

                            reject(err);

                        });
                    resolve(true);

                })
            );
            resolve(true);

        })
    );

}

// Create bucket test cases
function createBucket (project, accessResult) {

    describe(
        "Create Bucket",
        () => new Promise<boolean>(async (resolve, reject) => {

            it(
                "Creating Bucket",
                async () => new Promise<boolean>(async (resolve, reject) => {

                    await project.createBucket(storjConfig.bucketName).then(async (bucketInfo) => {

                        describe(
                            "create bucket Result",
                            () => {

                                it(
                                    "should have information",
                                    () => {

                                        bucketInfo.name.should.equal(storjConfig.bucketName);

                                    }
                                );

                            }
                        );
                        //
                        statBucket(project);
                        //
                        ensureBucket(project);
                        //
                        listBucket(project);
                        //
                        uploadObject(
                            project,
                            accessResult
                        );
                        //
                        resolve(true);
                        //

                    }).
                        catch((err) => {

                            reject(err);

                        });
                    resolve(true);

                })
            );
            resolve(true);

        })
    );

}

// Open project test case
function openProject (accessResult) {

    describe(
        "Open Project Function",
        () => new Promise<boolean>(async (resolve, reject) => {

            it(
                "Opening project",
                async () => new Promise<boolean>(async (resolve, reject) => {

                    await accessResult.openProject().then(async (project) => {

                        describe(
                            "Project Result",
                            () => {

                                it(
                                    "Should have project",
                                    () => {

                                        expect(project.project._handle).to.be.a("number");

                                    }
                                );

                            }
                        );
                        //
                        describe(
                            "Project Result",
                            () => {

                                it(
                                    "Should have stat bucket function",
                                    () => {

                                        project.should.have.property("statBucket");

                                    }
                                );

                            }
                        );
                        //
                        describe(
                            "Project Result",
                            () => {

                                it(
                                    "Should have create bucket function",
                                    () => {

                                        project.should.have.property("createBucket");

                                    }
                                );

                            }
                        );
                        //
                        describe(
                            "Project Result",
                            () => {

                                it(
                                    "Should have ensure bucket function",
                                    () => {

                                        project.should.have.property("ensureBucket");

                                    }
                                );

                            }
                        );
                        //
                        describe(
                            "Project Result",
                            () => {

                                it(
                                    "Should have list bucket function",
                                    () => {

                                        project.should.have.property("listBuckets");

                                    }
                                );

                            }
                        );
                        //
                        describe(
                            "Project Result",
                            () => {

                                it(
                                    "Should have download object function",
                                    () => {

                                        project.should.have.property("downloadObject");

                                    }
                                );

                            }
                        );
                        //
                        describe(
                            "Project Result",
                            () => {

                                it(
                                    "Should have upload object function",
                                    () => {

                                        project.should.have.property("uploadObject");

                                    }
                                );

                            }
                        );
                        //
                        describe(
                            "Project Result",
                            () => {

                                it(
                                    "Should have stat object function",
                                    () => {

                                        project.should.have.property("statObject");

                                    }
                                );

                            }
                        );
                        //
                        describe(
                            "Project Result",
                            () => {

                                it(
                                    "Should have list object function",
                                    () => {

                                        project.should.have.property("listObjects");

                                    }
                                );

                            }
                        );
                        //
                        describe(
                            "Project Result",
                            () => {

                                it(
                                    "Should have delete object function",
                                    () => {

                                        project.should.have.property("deleteObject");

                                    }
                                );

                            }
                        );
                        //
                        describe(
                            "Project Result",
                            () => {

                                it(
                                    "Should have delete bucket function",
                                    () => {

                                        project.should.have.property("deleteBucket");

                                    }
                                );

                            }
                        );
                        //
                        describe(
                            "Project Result",
                            () => {

                                it(
                                    "Should have close project function",
                                    () => {

                                        project.should.have.property("close");

                                    }
                                );

                            }
                        );
                        //
                        createBucket(
                            project,
                            accessResult
                        );
                        //
                        resolve(true);

                    }).
                        catch((err) => {

                            reject(err);

                        });
                    resolve(true);

                })
            );
            resolve(true);

        })
    );

}

/*
 *
 *
 */
describe(
    "Uplink NodeJS Test",
    () => {

        //
        context(
            "Request Access with phassphrase",
            () => {

                it(
                    "Request Access",
                    () => {

                        return new Promise<boolean>(async (resolve, reject) => {

                            //
                            await libUplink.requestAccessWithPassphrase(
                                storjConfig.satelliteURL,
                                storjConfig.apiKey,
                                storjConfig.encryptionPassphrase
                            ).then(async (access) => {

                                //
                                describe(
                                    "Access Result",
                                    () => {

                                        it(
                                            "Should have access",
                                            () => {

                                                expect(access.access._handle).to.be.a("number");

                                            }
                                        );

                                    }
                                );

                                describe(
                                    "Access Result",
                                    () => {

                                        it(
                                            "Should have open project function",
                                            () => {

                                                access.should.have.property("openProject");

                                            }
                                        );

                                    }
                                );

                                describe(
                                    "Access Result",
                                    () => {

                                        it(
                                            "Should have config open project function",
                                            () => {

                                                access.should.have.property("configOpenProject");

                                            }
                                        );

                                    }
                                );

                                describe(
                                    "Access Result",
                                    () => {

                                        it(
                                            "Should have access share function",
                                            () => {

                                                access.should.have.property("share");

                                            }
                                        );

                                    }
                                );

                                describe(
                                    "Access Result",
                                    () => {

                                        it(
                                            "Should have access serialize function",
                                            () => {

                                                access.should.have.property("serialize");

                                            }
                                        );

                                    }
                                );
                                //
                                openProject(access);
                                //
                                resolve(true);
                                //

                            });

                        }).catch((err) => {
                            console.log(err);
                        });
                        //
                    }
                );

            }
        );

    }
);

