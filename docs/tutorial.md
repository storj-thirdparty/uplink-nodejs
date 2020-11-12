# Tutorial

> Welcome to the tutorial of creating a project by yourself. Let's start!

> Note: All the functions are asynchronous and return promises.

## Step 1: Storj Configurations

First and foremost you need to have all the keys and configuration to connect to the storj network. You can create a JSON file and parse the JSON file to sotre the values in a class/strucure or you can simply initialize the corrsponding variables inside the main method as done in the sample *HelloStorj.js* file in the following way:

```js
var storjConfig = {
    apiKey    : "change-me-to-the-api-key-created-in-satellite-gui",
    satelliteURL   : "us-central-1.tardigrade.io:7777",
    encryptionPassphrase  : "you'll never guess this",
    bucketName   : "change-me-to-desired-bucket-name",
    uploadPath   : "optionalpath/requiredfilename",
};
```

## Step 2: File Path

File path could be path of the file to be upload from or path where a file needs to be download at or both. You can create a JSON file and parse it or simply initialize file path variable(s) as done in the sample *HelloStorj.js* file in the following way:

```js
var localFullFileName = {
    src : "change-me-to-source-file-name-at-local-system",
    dest: "change-me-to-destination-file-name-at-local-system",
};
```

## Step 3: Create libUplink class object

Next, you need to create an object of Uplink class that will be used to call the libuplink functions.

```js
const storj = require("uplink-nodejs");
const libUplink = new storj.Uplink();

```

## Step 4: Create access handle

Now we need to get access handle to Storj V3 network. For that we can use any of these functions:
  
### requestAccessWithPassphrase

```js
//first method for handling promise
libUplink.requestAccessWithPassphrase(satelliteURL,apiKey,encryptionPassphrase).then(access => {
  ....//some code//....
}).catch((err) => {
        ....//some code//....
    });

//or
//second method for handling promise
var access = await libUplink.requestAccessWithPassphrase(satelliteURL,apiKey,encryptionPassphrase).catch((err) => {
        ....//some code//....
    });
....//some code//....
```

The function accepts 3 arguments Satellite URL, API Key, and Passphrase and returns an AccessResult object (access) on successful execution which can be used to call other functions which are bound to it.

> NOTE: We will be use first convention for handling promise in our documentation but user are free to use second convention.

### configRequestAccessWithPassphrase

```js
libUplink.configRequestAccessWithPassphrase(satelliteURL,apiKey,encryptionPassphrase).then(access => {
  ....//some code//....
}).catch((err) => {
        ....//some code//....
    });
```

The function accepts 4 arguments Config Object, Satellite URL, API Key, and Passphrase and returns an AccessResult object (access) on successful execution which can be used to call other functions which are bound to it.

### parseAccess

```js
libUplink.parseAccess(stringResult).then(async (parsedSharedAccess) => {
  ....//some code//....
}).catch((err) => {
        ....//some code//....
    });
```

The function accepts a Serialized Access (String) and returns an AccessResult object (parsedSharedAccess) on successful execution which can be used to call other functions which are bound to it.

> NOTE: parse_access can be called only after getting a Serialized Access String which is given by function access_serialize.

## Step 5: Open Project

Once we have the Access handle, it can be used to open a project using any of these functions:

### openProject

```js
access.openProject().then(async (project) => {
  ...//some code//...
}).catch((err) => {
        ....//some code//....
    });
```

This function is a property of accessResult which is returned by Access Functions and denoted as 'access' in the above codes. It  returns project on successful execution.

### configOpenProject

```js
access.configOpenProject(config).then(async (project) => {
  ...//some code//...
}).catch((err) => {
        ....//some code//....
    });
```

This function is a property of 'access' which is returned by Access Functions. It accepts a config object and returns project on successful execution. The project used in the steps ahead refers to this project object only.

## Step 6: Create/Ensure Bucket

After getting project we need a bucket for further execution. If a bucket is not available, one is created otherwise information about current bucket is collected and execution moves ahead.
We use *stat_bucket* function to get information about the bucket.

### statBucket

```js
await project.statBucket(bucketName).then((bucketInfo) => {
...//some code//...
}).catch((err) => {
        ....//some code//....
    });
```

This function accepts one arguments, a bucketname and returns bucketInfo (information about the current bucket)

> NOTE: If the specified bucket does not exist, it is created using the function below

### createBucket

```js
await project.createBucket(bucketName).then((bucketInfo) => {
...//some code//...
}).catch((err) => {
        ....//some code//....
    });
```

This function accepts one arguments, a bucketname, creates the bucket and returns bucketInfo (information about the current bucket)

> After creating the bucket, it is important to ensure that the bucket exists on Storj V3 network before moving ahead.

### ensureBucket

```js
await project.ensureBucket(bucketName).then((bucketInfo) => {
...//some code//...
}).catch((err) => {
        ....//some code//....
    });
```

This function accepts one arguments, a bucketname, ensures that the bucket is present on Storj V3 network and returns bucketInfo (information about the current bucket) 

## Step 7: List Buckets (Optional)

Once it is ensured that bucket exists on Storj V3 network, you can list all the buckets. However this step is completely optional. If you don't wish to list the buckets, you can skip this step.

### listBuckets

```js
var listBucketsOptions = new storj.ListBucketsOptions();
await project.listBuckets(listBucketsOptions).then(async (bucketListResult) => {
...//some_code//...
}).catch((err) => {
        ....//some code//....
    });
```

This function accepts one arguments, listBucketOptions and returns bucketListResult.

## Step 8: Upload

Uploading a file consists of following sub-steps:

### Get Upload Info
  
```js
var uploadOptions = new storj.UploadOptions();
uploadOptions.expires = 0;
await project.uploadObject(bucketName,uploadPath,uploadOptions).then(async (upload) => {  
  ...//some_code//...
}).catch((err) => {
        ....//some code//....
    });
```

This function accepts 3 arguments bucketname, uploadpath and uploadoptions. It then returns an uploadinfo object.

You can create *uploadOptions* by referring to the [libuplink documentation](https://godoc.org/storj.io/uplink) or simply pass *none*.

### Create File Handle:

To stream the data to storj you need to create a file stream or handle which can be done in the following way. Here *uploadinfo* is your upload handle.  

```js
await project.uploadObject(bucketName,uploadPath,uploadOptions).then(async (upload) => {
  ...//some_code//...
}).catch((err) => {
        ....//some code//....
    });
```

### Create Buffer:

Now we need to create buffer to store our file's data. It can be done in the following way.

```js
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
```

### Upload File:

Once we have the File Handle and Buffer, we can upload data to Storj V3 network in the following way:

```js
await upload.write(buffer,bytesRead).then((writeResult) => {
  ...//some_code//...
}).catch((err) => {
        ....//some code//....
    });
```

### Commit Upload:

Once the data has been successfully streamed, the upload needs to be committed using the following method:
```js
await upload.commit().then(() => {
    ...//some_code//...
}).catch((err) => {
    ...//some_code//...
});
```
## Step 9: Download
  
Downloading a file consists of following steps:

### Create Download Handle:
  
We need to create a download handle first in the following way. In the code, downloadresult is your 
  
```js
var downloadOptions = new storj.DownloadOptions();
downloadOptions.offset = 0;
downloadOptions.length = -1;

await project.downloadObject(bucketName,uploadPath,downloadOptions).then(async (download) => {
  ...//some_code//...
}).catch((err) => {
        ....//some code//....
    });
```

### Get Download Object Info:

After creating the download handle, we need to get info about the object to be downloaded by using the following function:

```js
await download.info().then((objectInfo) => {
  ...//some_code//...
}).catch((err) => {
        ....//some code//....
    });
```

### Creating download buffer and file handle:

Now we need to create a download buffer and file handle in the same way as we did for upload.

```js
var size = { download : 0,
            actuallyWritten : 0,
            totalWritten    : 0};
var buffer = new Buffer.alloc(BUFFER_SIZE);

var fileHandle = await fs.openSync(localFullFileName.dest, "w");

var loop = true;
while(loop) {
    if((objectsize-size.totalWritten>0)&&(objectsize-size.totalWritten)<BUFFER_SIZE){
        buffer = null;
        buffer = new Buffer.alloc(objectsize-size.totalWritten);
}
```

### Downloading the object:

Now we can download the object from Stroj V3 server using the following function.

```js
await download.read(buffer,buffer.length).then(async (bytesread) => {
  ...//some_code//...
}).catch((err) => {
        ....//some code//....
    });
```

### Closing the download stream:

When the file is downloaded, you need to close the downlaod stream. It can be done in the following way:

```js
fs.closeSync(fileHandle);
await download.close().then(() => {
  ...//some_codee//...
}).catch((err) => {
        ....//some code//....
    });
```

> NOTE: Perform error handling as as per your implmentation.

> NOTE: Alternatively, you can simple create a method(s) that can be called from the main method and performs all the above steps internally as done in the sample file.


## Step 10: Providing Shared Access (Optional)
  
To provide shared access, we can use this function

```js
// set permissions for the new access to be created
var permission = new storj.Permission(true,true,true,true,0,0);

// set shared prefix
var sharePrefix = storj.SharePrefix;
var sharePrefixListArray = [];
sharePrefix.bucket = "change-me-to-desired-bucket-name";
sharePrefix.prefix ="change-me-to-desired-object-prefix";
sharePrefixListArray.push(sharePrefix);

// create new access
await access.share(permission,sharePrefixListArray,sharePrefixListArray.length).then(async (sharedAccess) => {
  // generate serialized access to share
  await sharedAccess.serialize().then(async (stringResult) => {
    ...//some_code//...
  }).catch((err) => {
        ....//some code//....
    });
}).catch((err) => {
        ....//some code//....
    });
```
## Step 11: Delete Object

To delete an object, we can use the following function:

```js
await project.deleteObject(bucketName,uploadPath).then((objectinfo) => {
...//some_code//...
}).catch((err) => {
        ....//some code//....
    });
```

This function accepts 2 arguments,  bucketname and uploadpath to identify the object to be deleted and returns information about the deleted object.

## Step 12: Delete Bucket

To delete a bucket, we can use the following function:

```js
await project.deleteBucket(bucketName).then((bucketInfo) => {
...//some_code//...
}).catch((err) => {
        ....//some code//....
    });
```

## Step 13: Close Project 

After finalizing everything, you can close the project in following way;

```js
await project.close().then(() => {
...//some_code//...
}).catch((err) => {
        ....//some code//....
    });
```

> NOTE: For more binding functions refer to the [uplink-nodejs Binding Functions](/library.md) and for implemtation purpose refer *HelloStorj.js* file.
