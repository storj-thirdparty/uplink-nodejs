// 'HelloStorj' example Node.js code using Node.js-Storj bindings

// include the Node.js-Storj bindings module
var libUplinkNode = require("./node_modules/storj-nodejs/libUplinkNode.js");


// file names to be uploaded or downloaded as
// demo Storj (V3) configuration

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



// [TEST 01] open a Storj project and process its content
open_project(process_opened_project);

// [TEST 02] list objects in a Storj bucket
// open_project(check_objects);

// [TEST 03] to delete a bucket, after opening the Storj project
// open_project(delete_bucket);


// open a desired Storj project and process it, using given call back function
function open_project(process_project) {
	// step 1: create new uplink
	console.log("Setting-up a New Uplink...");
	//
	libUplinkNode
		.new_uplink()
		.then(function (uplinkRef) {
			console.log("New Uplink: SET-UP!");
			
			// step 2: parse the API key
			console.log("\nParsing the API Key: ", storjConfig.apiKey);
			//
			libUplinkNode
				.parse_api_key(storjConfig.apiKey)
				.then(function (parsedAPIKeyRef) {
					console.log("API Key: PARSED!");
					
					// step 3: open the Storj project, corresponding to the parsed API key
					console.log("\nOpening the Storj Project from Satellite: ", storjConfig.satelliteURL);
					//
					libUplinkNode
						.open_project(uplinkRef, storjConfig.satelliteURL, parsedAPIKeyRef)
						.then(function (projectRef) {
							console.log("Desired Storj Project: OPENED!");
							
							// next step: process this opened project, using the given call-back function
							process_project(projectRef, uplinkRef);
						})
						.catch(function (error) {
							console.error("FAILed to open desired Storj project!");
							console.error(error);
							//
							// close the established Storj uplink
							close_uplink(uplinkRef);
						});
				})
				.catch(function (error) {
					console.error("FAILed to parse the API key!");
					console.error(error);
					//
					// close the established Storj uplink
					close_uplink(uplinkRef);
				});
		})
		.catch(function (error) {
			console.error("FAILed to set-up a new uplink!");
			console.error(error);
		});
}

// 1. attempt to create a new bucket
// 2. list all buckets in the project
// 3. open a bucket to process its objects
function process_opened_project(pO_projectRef, pO_uplinkRef) {
	// attempt to create a new bucket within the Storj project with desired name
	console.log("\nCreating a new bucket with name, ", storjConfig.bucketName, "...");
	//
	libUplinkNode
		.create_bucket(pO_projectRef, storjConfig.bucketName)
		.then(function (bucketInfo) {
			console.log("New Bucket: CREATED!");
		})
		.catch(function (error) {
			console.error("FAILed to create a new bucket!");
			console.error(error);
		})
		.finally(() => {
			// list buckets within the Storj project
			console.log("\nListing buckets within the Storj project...");
			//
			libUplinkNode
				.list_buckets(pO_projectRef)
				.then(function (bucketsList) {
					console.log("Buckets' List: FETCHed!");
					//
					if (bucketsList.length > 0) {
						console.log("S.No. \t Created On \t\t Bucket Name");
						console.log("===== \t ========== \t\t ===========");
					}
					for (let i = 0; i < bucketsList.length; i++) {
						var num = "0" + (i + 1).toString();
						console.log(num.substr(-2), "  ", getDateTime(bucketsList.items[i].created), "\t", bucketsList.items[i].name);
					}
				})
				.catch(function (error) {
					console.error("FAILed to fetch buckets' list!");
					console.error(error);
				})
				.finally(() => {
					// open a bucket in a given opened Storj project and process its objects
					open_bucket(pO_projectRef, pO_uplinkRef, process_opened_bucket);
					//
					// to delete an object, after opening the bucket
					//open_bucket(pO_projectRef, pO_uplinkRef, delete_object);
				});
		});
}
//
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

// to delete a bucket
function delete_bucket(pO_projectRef, pO_uplinkRef) {
	console.log("\nDeleting bucket,", storjConfig.bucketName, "...");
	//
	libUplinkNode
		.delete_bucket(pO_projectRef, storjConfig.bucketName)
		.then(function (fromResolve) {
			console.log("Bucket DELETed!");
		})
		.catch(function (error) {
			console.error("FAILed to delete the bucket!");
			console.error(error);
		})
		.finally(() => {
			// close the already opened Storj project and the established uplink
            close_project_and_uplink(pO_projectRef, pO_uplinkRef);
		});
}


// 1. open a bucket in a given opened Storj project
// 2. process its objects, as per the call back function
// 3. in case of exceptions, close the Storj project and the established uplink
function open_bucket(pO_projectRef, pO_uplinkRef, process_bucket) {
	// step 1: get access encryption passphrase to process data within a bucket
	console.log("\nAccessing given Encryption Passphrase...");
	//
	libUplinkNode
		.encryption_key(pO_projectRef, storjConfig.encryptionPassphrase)
		.then(function (encryptionCtxPtr) {
			console.log("Encryption Access: RECEIVED!");
			
			// step 2: open desired Storj bucket
			console.log("\nOpening ", storjConfig.bucketName, " Bucket...");
			libUplinkNode
				.open_bucket(pO_projectRef, storjConfig.bucketName, encryptionCtxPtr)
				.then(function (bucketRef) {
					console.log(storjConfig.bucketName, " Bucket: OPENED!");

					// next step: process the opened Storj bucket's objects
					process_bucket(bucketRef, pO_projectRef, pO_uplinkRef);
				})
				.catch(function (error) {
					console.error("FAILed to open desired bucket!");
					console.error(error);
					//
					// close the already opened Storj project and the established uplink
					close_project_and_uplink(pO_projectRef, pO_uplinkRef);
				});
		})
		.catch(function (error) {
			console.error("FAILed to get encryption access from given passphrase!");
			console.error(error);
			//
			// close the already opened Storj project and the established uplink
			close_project_and_uplink(pO_projectRef, pO_uplinkRef);
		});
}

// upload a file to desired path within the Storj bucket
// download the Storj object as a desired file name
// close the opened bucket
function process_opened_bucket(pO_bucketRef, pO_projectRef, pO_uplinkRef) {
	// upload a local file to the given Storj bucket's path
	console.log("\nUploading ", localFullFileName.src, " file to the Storj Bucket...");
	//
	upload_file(pO_bucketRef, storjConfig.uploadPath, localFullFileName.src)
	.then(function (fromResolve) {
		console.log(localFullFileName.src, " File: UPLOADED as ", storjConfig.uploadPath, "!");

		// download the just uploaded Storj object as a different destination file
		console.log("\nDownloading ", storjConfig.uploadPath, " Storj Object as ", localFullFileName.dest, " file...");
		//
		download_file(pO_bucketRef, storjConfig.uploadPath, localFullFileName.dest)
			.then(function (fromResolve) {
				console.log("Storj Bucket's ", storjConfig.uploadPath, " object DOWNLOADED as ", localFullFileName.dest, " file!");
			})
			.catch(function (error) {
				console.error("FAILed to download ", storjConfig.uploadPath, " object from the Storj bucket!");
				console.error(error);
			})
			.finally(() => {
				// close the already opened bucket, the Storj project and the established uplink
				close_bucket_project_and_uplink(pO_bucketRef, pO_projectRef, pO_uplinkRef);
			});
	})
	.catch(function (error) {
		console.error("FAILed to upload ", localFullFileName.src, " file into the Storj bucket!");
		console.error(error);
		//
		// close the already opened bucket, the Storj project and the established uplink
		close_bucket_project_and_uplink(pO_bucketRef, pO_projectRef, pO_uplinkRef);
	});
}

function delete_object(pO_bucketRef, pO_projectRef, pO_uplinkRef) {
	console.log("\nDeleting object,", storjConfig.uploadPath, "...");
	//
	libUplinkNode
		.delete_object(pO_bucketRef, storjConfig.uploadPath)
		.then(function (fromResolve) {
			console.log("Object DELETed!");
		})
		.catch(function (error) {
			console.error("FAILed to delete the object!");
			console.error(error);
		})
		.finally(() => {
			// close the already opened bucket, the Storj project and the established uplink
			close_bucket_project_and_uplink(pO_bucketRef, pO_projectRef, pO_uplinkRef);
		});
}

// close the already opened bucket, the Storj project and the established uplink
function close_bucket_project_and_uplink(pO_bucketRef, pO_projectRef, pO_uplinkRef) {
	// close the opened Storj bucket
	console.log("\nClosing the Opened Bucket...");
	//
	libUplinkNode
		.close_bucket(pO_bucketRef)
		.then(function (fromResolve) {
			console.log("Desired Bucket: CLOSED!");
		})
		.catch(function (error) {
			console.error("FAILed to close desired bucket!");
			console.error(error);
		})
		.finally(() => {
			// close the opened project and the established uplink too
			close_project_and_uplink(pO_projectRef, pO_uplinkRef);
		});
}

// close the already opened Storj project and the established uplink
function close_project_and_uplink(pO_projectRef, pO_uplinkRef) {
	// close the opened Storj project
	console.log("\nClosing the opened Storj Project...");
	//
	libUplinkNode
		.close_project(pO_projectRef)
		.then(function (fromResolve) {
			console.log("Storj Project: CLOSED!");
		})
		.catch(function (error) {
			console.error("FAILed to close the opened Storj project!");
			console.error(error);
		})
		.finally(() => {
			// close the established uplink too
			close_uplink(pO_uplinkRef);
		});
}

// close the already established uplink
function close_uplink(pO_uplinkRef) {
	console.log("\nClosing the established Storj Uplink...");
	//
	libUplinkNode
		.close_uplink(pO_uplinkRef)
		.then(function (fromResolve) {
			console.log("Storj Uplink: CLOSED!");
		})
		.catch(function (error) {
			console.error("FAILed to close the established Storj uplink!");
			console.error(error);
		});
}

function check_objects(pO_projectRef, pO_uplinkRef) {
	// open a bucket in a given opened Storj project and process its objects
	open_bucket(pO_projectRef, pO_uplinkRef, list_objects);
};

// attempt to list objects within the opened Storj project's bucket
// free the list, thereafter
function list_objects(pO_bucketRef, pO_projectRef, pO_uplinkRef) {
	// list objects within the Storj project bucket
	console.log("\nListing objects within the Storj project's bucket...");
	//
	libUplinkNode
		.list_objects(pO_bucketRef)
		.then(function (objectsList) {
            console.log("Objects' List: FETCHed!");
			//
			if (objectsList.length > 0) {
				console.log("S.No. \t Created On \t\t Modified On \t\t Size (bytes) \t Object Path");
				console.log("===== \t ========== \t\t =========== \t\t =========== \t ===========");
			}
			for (let i = 0; i < objectsList.length; i++) {
				var num = "0" + (i + 1).toString();
				console.log(num.substr(-2), "  ", getDateTime(objectsList.items[i].created), "\t", getDateTime(objectsList.items[i].modified), "\t", objectsList.items[i].size, "\t\t", objectsList.items[i].path);
			}
		})
		.catch(function (error) {
			console.error("FAILed to fetch objects' list!");
			console.error(error);
		})
		.finally(() => {
			// close the already opened bucket, the Storj project and the established uplink
			close_bucket_project_and_uplink(pO_bucketRef, pO_projectRef, pO_uplinkRef);
		});
}


/* Testing Upload and Download Functionalities */
const fs = require("fs");

const BUFFER_SIZE = 256;
const RETRY_MAX = 5;

// an example function to upload data from a file at local system to a Storj (V3) bucket's path
// pre-requisites: open_bucket() function has been already called
// uses library functions: upload(), upload_write(), and upload_commit() 
// inputs: valid BucketRef, Storj Path/File Name (string), Source Full File Name (string)
// output: None, else error in catch
let upload_file = function(pO_bucketRef, ps_uploadPathStorj, ps_uploadPathLocal) {
    
    return new Promise(function(resolve, reject) { 
        
        libUplinkNode.upload(pO_bucketRef, ps_uploadPathStorj)
        .then(function(uploaderRef) {
			// as an exmple, let us open a local file and get its size.
            var fileHandle = fs.openSync(ps_uploadPathLocal, "rs");
            //
            var size = {
                file            : `${fs.statSync(ps_uploadPathLocal).size}`,
                toWrite         : 0,
                actuallyWritten : 0,
                totalWritten    : 0
            };

            var li_retryCount = RETRY_MAX;
            //
            // set-up buffer, to fetch file's content
            var buffer = new Buffer.alloc(BUFFER_SIZE);
            //
            // size of data to upload 
            while (size.totalWritten < size.file) { 
                size.toWrite = size.file - size.totalWritten;
                //
                if (size.toWrite > BUFFER_SIZE) {
                    size.toWrite = BUFFER_SIZE;
                } else if (size.toWrite == 0) {
                    break;
                }

                // read proper number of bytes from the file
                var bytesRead = fs.readSync(fileHandle, buffer, 0, size.toWrite, size.totalWritten);
                //
                while ((bytesRead <= 0) && (--li_retryCount > 0)) {
                    //
                    bytesRead = fs.readSync(fileHandle, buffer, 0, size.toWrite, size.totalWritten);
                }
                //
                if (bytesRead <= 0) {
                    // could not read further
                    ls_error = "ERROR: Read " + size.totalWritten.toString() + " bytes of complete " + size.file.toString() + " bytes from the file. Could not read further.";
                    //
                    break;
                }
                
                if (li_retryCount < RETRY_MAX) {
                    li_retryCount = RETRY_MAX;
                }

                var ls_error = "";               
                //
                do {
	                // upload the buffer's content to the Storj bucket          
	               	var uploadedInfo = libUplinkNode.upload_write(uploaderRef, buffer, bytesRead);
	                
                   	if (uploadedInfo.error != "") {
		                ls_error = uploadedInfo.error;
		            } else {
						size.actuallyWritten = uploadedInfo.size;
					}
                } while ((ls_error != "") && (--li_retryCount > 0));
                
                //
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
            }

            // close the opened local file
            fs.closeSync(fileHandle);

            var ls_dataUploaded = "\n " + ((size.totalWritten * 100.0)/size.file).toString() + "% uploaded!";

			if (li_retryCount <= 0) {
				reject(ls_error + ls_dataUploaded);
			} else if (li_retryCount > 0) {
				// upload to Storj bucket
				libUplinkNode.upload_commit(uploaderRef)
				.then(function() {
					resolve(true);
				})
				.catch(function(ls_error) {
					reject(ls_error + ls_dataUploaded);
				});
			} 

        })
        .catch(function(error) {
        	reject(error);
        }); 
    });
};

// function to download Storj (V3) object's data and store it into given file at local system
// pre-requisites: open_bucket() function has been already called
// uses library functions: download(), download_read(), and download_close() 
// inputs: valid BucketRef, Storj Path/File Name (string), Destination Full File Name (string)
// output: None, else error in catch
let download_file = function(pO_bucketRef, ps_downloadPathStorj, ps_downloadPathLocal) {
    return new Promise(function(resolve, reject) {
        //
        // prepare to download desired Storj object
        libUplinkNode.download(pO_bucketRef, ps_downloadPathStorj)
        .then(function(downloaderRef){
        	  var size = {
                downloaded      : 0,
                actuallyWritten : 0,
                totalWritten    : 0
            };
            
            var li_retryCount = RETRY_MAX;
            // set-up buffer
            var buffer = new Buffer.alloc(BUFFER_SIZE);

            // create/open file, to be written to with downloaded data
            var fileHandle = fs.openSync(ps_downloadPathLocal, "w");
                
			while(true) {
				var	ls_error = "";
				//    
				do {
					// download a part of Storj object, as per the buffer size   
					var downloadedInfo = libUplinkNode.download_read(downloaderRef, buffer, BUFFER_SIZE);
					
					if (downloadedInfo.error != "") {
						ls_error = downloadedInfo.error;
					} else {
						size.downloaded = downloadedInfo.size; 
					}
					
					// buffer = downloadData.buffer;
					//
				} while((ls_error != "") && (--li_retryCount > 0));
				
				//
				if (ls_error != "") {
					break;
				}
				
				if (li_retryCount < RETRY_MAX) {
					li_retryCount = RETRY_MAX;
				}
				
				if (size.downloaded == 0) {
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
			libUplinkNode.download_close(downloaderRef)
			.then(function(){
				if (ls_error != "") {
					reject(ls_error);
				} else {
					resolve(true);
				}
			})
			.catch(function(ls_errorNew) {
				ls_error += "\n" + ls_errorNew;
				//
				reject(ls_error);
			});   
       	})
        .catch(function(ls_error){
	        reject(ls_error);
	    }); 

   });
};
