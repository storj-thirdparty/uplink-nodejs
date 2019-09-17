// 'HelloStorj' example Node.js code using Node.js-Storj bindings

// include the Node.js-Storj bindings module
var libUplinkNode = require("./libUplinkNode.js");

// demo Storj (V3) configuration
var storjConfig = {
	apiKey				: "change-me-to-the-api-key-created-in-satellite-gui",
	satelliteURL		: "us-central-1.tardigrade.io:7777",
	encryptionPassphrase: "you'll never guess this",
	bucketName			: "change-me-to-desired-bucket-name",
	uploadPath			: "optionapath/requiredfilename",
};

// file names to be uploaded or downloaded as
var localFullFileName = {
	src : "change-me-to-source-file-name-at-local-system",
	dest: "change-me-to-destination-file-name-at-local-system",
};


// open a Storj project and process its content
open_project(process_opened_project);


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

// attempt to create a new bucket
// open a bucket and process its objects, as per the call back function
// close the Storj project and the established uplink
function process_opened_project(pO_projectRef, pO_uplinkRef) {
	// create a new bucket within the Storj project with desired name
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
			// open a bucket in a given opened Storj project and process its objects
			open_bucket(pO_projectRef, pO_uplinkRef, process_opened_bucket);
		});
}

// open a bucket in a given opened Storj project and process its objects
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
	libUplinkNode
		.upload_file(pO_bucketRef, storjConfig.uploadPath, localFullFileName.src)
		.then(function (fromResolve) {
			console.log(localFullFileName.src, " File: UPLOADED as ", storjConfig.uploadPath, "!");

			// download the just uploaded Storj object as a different destination file
			console.log("\nDownloading ", storjConfig.uploadPath, " Storj Object as ", localFullFileName.dest, " file...");
			//
			libUplinkNode
				.download_file(pO_bucketRef, storjConfig.uploadPath, localFullFileName.dest)
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
