# Binding Functions

## Include uplink in library 
```js
	const storj = require("uplink-nodejs");
	const libUplink = new storj.Uplink();

```
* We need to create an object of Uplink class that will be used to call the libuplink functions.

## requestAccessWithPassphrase(String, String, String)

### Description:

This function requestAccessWithPassphrase  requests satellite for a new access grant
using a passhprase, there is no pre-requisites required for this function.\
This function accepts 3 arguments Satellite URL, API Key and  encryptionpassphrase
and returns an access object on successful execution which can be used to 
call other functions which are bound to it.\
An access grant is a serialized structure that is internally comprised of an 
API Key, a set of encryption key information, and information about which Satellite
address is responsible for the metadata.\
An access grant is always associated with exactly one Project on one Satellite.

### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>satelliteURL</code> | Storj V3 network satellite address | <code>string</code> |
| <code>apikey</code> | Storj V3 network API key | <code>string</code> |
| <code>encryptionPassphrase</code> | Any passphrase | <code>string</code> |

### Usage Example:

```js
var satelliteURL = "change-me-to-desired-satellite-address";
var apiKey = "change-me-to-desired-api-key";
var encryptionPassphrase = "change-me-to-desired-encryption";
libUplink.requestAccessWithPassphrase(satelliteURL, apiKey, encryptionPassphrase).then(access => {
		....//some code//....
}).catch((err) => {
    ....//some code//....
});
//
//OR
var access = await libUplink.requestAccessWithPassphrase(satelliteURL, apiKey, encryptionPassphrase).catch((err) => {
    ....//some code//....
});
....//some code//....
```

## configRequestAccessWithPassphrase(Object, String, String, String)

### Description:

This function configRequestAccessWithPassphrase requests satellite for a new access grant 
using a passhprase and config.\
There is no pre-requisites required for this function.\
This function accepts 4 arguments Satellite URL, API Key, encryptionpassphrase and config object and returns an access object on successful execution which can be used to call other functions which are bound to it.

### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>config</code> | Create using storj library | <code>object</code> |
| <code>satelliteURL</code> | Storj V3 network satellite address | <code>string</code> |
| <code>apikey</code> | Storj V3 network API key | <code>string</code> |
| <code>encryptionPassphrase</code> | any passphrase string | <code>string</code> |

### Usage Example:

```js
var satelliteURL = "change-me-to-desired-satellite-address";
var apiKey = "change-me-to-desired-api-key";
var encryptionPassphrase = "change-me-to-desired-encryption";
var config = new storj.Config();
libUplink.configRequestAccessWithPassphrase(config, satelliteURL, apiKey, encryptionPassphrase).then(access => {
		....//some code//....
}).catch((err) => {
    ....//some code//....
});
```

##  parseAccess(String)

### Description:

parse_access function to parses serialized access grant string there is no pre-requisites 
required for this function.\
This function accepts one argument serialized access String
which is returned by access_serialize function it returns an access object on successful 
execution which can be used to call other functions which are bound to it.\
This should be the main way to instantiate an access grant for opening a project.

### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>stringResult</code> | serialized access string returned by access.serialize function | <code>string</code> |

### Usage Example:

```js	
libUplink.parse_access(stringResult).then(async (parsedSharedAccess) => {
		....//some code//....
}).catch((err) => {
    ....//some code//....
});
```

## uplinkDeriveEncryptionKey(String, Array)

### Description:

uplinkDeriveEncryptionKey function derives a salted encryption key for passphrase using the salt. This function is useful for deriving a salted encryption key for users when implementing multitenancy in a single app bucket.\
This function takes 2 argumeents Encryption Passphrase and a byte array.
It returns the Salted Encryption Key.

### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>encryptionPassphrase</code> | Any passphrase | <code>string</code> |
| <code>salt</code> | Any byte array | Array |

### Usage Example:

```js
var encryption = await libUplink.uplinkDeriveEncryptionKey("test", [4, 5, 6])
.catch((error) => {
    ....//some code//....
});
```

> NOTE: Following functions require Access(Object) for calling.

## share(Object, Object, Int)

### Description:

share function creates new access grant with specific permission. Permission will be
applied to prefixes when defined.
parse access function is required as a pre-requisite for this function.\
this function accepts 3 arguments permission(object) Permission defines what actions can be used to share which is access 
from storj Permission defines what actions can be used to share, sharePrefix(object) 
which is access from storj, and prefixcount is getting from the count of share prefix 
in the list.\
It returns an access object on successful execution which can be used 
to call other functions which are bound to it.

### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>permission</code> | Create using storj library | <code>object</code> |
| <code>SharePrefix</code> | Create using storj library | <code>object</code> |
| <code>PrefixCount</code> |count of share prefix | <code>Int</code> |

### Usage Example:

```js		
var permission = new storj.Permission(true, true, true, true, 0, 0);
// set shared Prefix
var sharePrefix = storj.SharePrefix;
var sharePrefixListArray = [];
sharePrefix.bucket = "change-me-to-desired-bucket-name";
sharePrefix.prefix ="change-me-to-desired-object-prefix";
sharePrefixListArray.push(sharePrefix);

await access.share(permission, Share Prefix, Prefix Count).then(async (sharedAccess) => {
	// generate serialized access to share
}).catch((err) => {
	....//some code//....
});
```

## serialize()

### Description:

serialize function serializes access grant into a string.\
parse access function is required as a pre-requisite for this function.
which is returned by access_share function.\
it returns an Serialized Access String 
on successful execution which is used to be as parse_access argument.

### Usage Example:

```js
await sharedAccessResult.serialize().then(async (stringResult) => {
	...//some_code//...
	}).catch((err) => {
	....//some code//....
});
```

## openProject()

### Description:

Once you have a valid access grant, you can open a Project with the access that access grant, 
openProject function opens project using access grant.\
requestAccessWithPassphrase or configRequestAccessWithPassphrase function is required as a pre-requisite.\
It returns an project object on successful execution which can be used to call 
other functions which are bound to it.\
It allows you to manage buckets and objects within buckets.

### Usage Example:

```js
access.openProject().then(async (project) => {
		...//some code//...
}).catch((err) => {
	....//some code//....
});
```

## configOpenProject(Object)

### Description:

configOpenProject function opens project using access grant and config.\
requestAccessWithPassphrase or configRequestAccessWithPassphrase function
is required as a pre-requisite. This function accepts 1 argument config(object) which is access from storj
library.\
it returns an project object on successful execution which can be used to call 
other functions which are bound to it.

### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>config</code> | Create using storj library | <code>object</code> |

### Usage Example:

```js
var config = new storj.config();
access.configOpenProject(config).then(async (project) => {
		...//some code//...
}).catch((err) => {
	....//some code//....
});
```

## overrideEncryptionKey(String, String, Object)

### Description:

overrideEncryptionKey function overrides the root encryption key for the prefix in bucket with encryptionKey. This function is useful for overriding the encryption key in user-specific access grants when implementing multitenancy in a single app bucket.
This function accepts 3 arguemnts Bucket name, Prefix and Encryption key.

### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>bucketName</code> | Storj bucket name | <code>string</code> |
| <code>prefix</code> | Storj prefix | <code>string</code> |
| <code>encryptionKey</code> | Create using storj library | <code>object</code> |

### Usage Example:

```js
await parsedSharedAccess.overrideEncryptionKey(sharePrefix.bucket, sharePrefix.prefix, encryption["encryption_key"])
.catch((err) => {
    ....//some code//....
});
```

> NOTE: Following functions require Project(Object) for calling.

## close()

### Description:

close function closes the project and openProject function is required as a pre-requisite.\
it returns an error object if on successful execution is not occurred.

### Usage Example:

```js
await project.close().then(() => {
	...//some_code//...
}).catch((err) => {
    ....//some code//....
});
```

## statBucket(String)

### Description:

statBucket function returns information about a bucket and openProject function is 
required as a pre-requisite.\
This function accepts 1 argument bucket name which is access from storj configuration.\
it returns an bucket object on successful execution it can be used to get
other properties which are bound to it.

### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>bucketName</code> | Storj bucket name | <code>string</code> |

### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
await project.statBucket(bucketName).then((bucketInfo) => {
	...//some code//...
}).catch((err) => {
	....//some code//....
});
```

## ensureBucket(String)

### Description:

ensureBucket function creates a new bucket and ignores the error when it 
already exists and openProject function is required as a pre-requisite.\
 This function accepts 1 argument bucket name which is access from storj configuration.\
It returns an bucket 
object on successful execution it can be used to get other properties 
which are bound to it.

### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>bucketName</code> | bucket name on storj V3 network | <code>string</code> |

### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
await project.ensureBucket(bucketName).then((bucketInfo) => {
...//some code//...
}).catch((err) => {
....//some code//....
});
```

## createBucket(String)

### Description:

createBucket function creates a new bucket When bucket already exists it returns 
a valid Bucket and ErrBucketExists and openProject function is required
as a pre-requisite.\
This function accepts 1 argument bucket name which is access from storj 
configuration.\
It returns an bucket object on successful execution it can be 
used to get other properties which are bound to it.

### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>bucketName</code> |access from storj configuration | <code>string</code> |

### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
await project.createBucket(bucketName).then((bucketInfo) => {
	...//some code//...
}).catch((err) => {
	....//some code//....
});
```

## deleteBucket(String)

### Description:

deleteBucket function deletes a bucket When bucket is not empty it returns ErrBucketNotEmpty.
and openProject function is requiredas a pre-requisite for this function .\
This function accepts 1 argument bucket name which is access from storj configuration.\
It returns an bucket object on successful execution it can be used to get other
properties which are bound to it.

### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>bucketName</code> | access from storj configuration | <code>string</code> |

### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
await project.deleteBucket(bucketName).then((bucketInfo) => {
	...//some_code//...
}).catch((err) => {
	....//some code//....
});
```

## listBuckets(Object)

### Description:

lsitBuckets function lists buckets and openProject function is required
as a pre-requisite for this function .This function accepts 1 argument listBucketOptions which is access from storj library.\
it returns an bucketList object on successful execution it can be used to get other
properties which are bound to it.


### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>listBucketOptions</code> | Create using storj library | <code>object</code> |

### Usage Example:

```js
var listBucketsOptions = new storj.ListBucketsOptions();
await project.listBuckets(listBucketsOptions).then(async (bucketListResult) => {
	...//some_code//...
}).catch((err) => {
    ....//some code//....
});
```

## statObject(String, String)

### Description:

statObject function information about an object at the specific key and 
openProject function is required as a pre-requisite for this function.\
This function accepts 2 argument bucket name which is access from storj configuration and Object Key which is access from storj configuration.\
It returns an objectinfo object on successful execution it can be used to get other
properties which are bound to it.

### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>bucketName</code> | Bucket name on storj V3 network | <code>string</code> |
| <code>objectName</code> | Object name on storj V3 network | <code>string</code> |

### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
var objectName = "change-me-to-desired-object-name";
await project.statObject(bucketName, objectName).then((objectinfo) => {
	...//some_code//...
}).catch((err) => {
    ....//some code//....
});
```

## listObjects(String, Object)

### Description:

listObjects function lists objects, openProject function is required as a pre-requisite 
for this function.\
This function accepts 2 argument bucket name which is access from storj configuration and listObjectOptions 
which is access from storj library ListObjectsOptions defines object listing options.\
it returns an objectList object, on successful execution it can be used to get 
other properties which are bound to it.

### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>bucketName</code> | bucket name on storj V3 network | <code>string</code> |
| <code>listObjectOptions</code> | Create using storj library | <code>object</code> |

### Usage Example:

```js		
var bucketName = "change-me-to-desired-bucket-name";
var listObjectsOptions = new storj.ListObjectsOptions();
await project.listObjects(bucketName, listObjectsOptions).then((objectlist) => {
	...//some_code//...
}).catch((err) => {
    ....//some code//....
});
```

## uploadObject(String, String, Object)

### Description:

uploadObject function starts an upload to the specified key, openProject 
function is required as a pre-requisite for this function.\
This function accepts 3 argument bucket name 
which is access from storj configuration, ObjectKey which is access from storj 
configuration and uploadOptions which is access from storj library UploadOptions 
contains additional options for uploading.\
It returns an upload object, on successful execution it can be used to call other properties which are bound to it.

### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>bucketName</code> | Bucket name on storj V3 network | <code>string</code> |
| <code>objectName</code> | Object name to be uploaded on storj V3 network | <code>string</code> |
| <code>uploadOptions</code> | Create using storj library | <code>object</code> |

### Usage Example:

```js		
var bucketName = "change-me-to-desired-bucket-name";
var objectName = "change-me-to-desired-object-name-on-storj";
var uploadOptions = new storj.UploadOptions();
await project.uploadObject(bucketName, objectName, uploadOptions).then(async (upload) => {  
		...//some_code//...
}).catch((err) => {
	....//some code//....
});
```

## downloadObject(String, String, Object)

### Description:

downloadObject function starts download to the specified key, openProject 
function is required as a pre-requisite for this function.\
This function accepts 3 argument  bucket name 
which is access from storj configuration, ObjectKey which is access from storj 
configuration and downloadOptions which is access from storj library.\
It returns an download object, on successful execution it can be used to call other properties which are bound to it.

### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>bucketName</code> | Bucket name on storj V3 network | <code>string</code> |
| <code>ObjectKey</code> | Object name already uploaded on storj V3 network | <code>string</code> |
| <code>downloadOptions</code> | Create using storj library | <code>object</code> |

### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
var objectName = "change-me-to-desired-object-name-on-storj";
var downloadOptions = new storj.DownloadOptions();
await project.downloadObject(bucketName, objectName, downloadOptions).then(async (download) => {
	...//some_code//...
}).catch((err) => {
	....//some code//....
});
```

## deleteObject(String, String)

### Description:

deleteObject function deletes an object at the specific key, openProject function is required as a pre-requisite 
for this function.\
This function accepts 2 argument  bucket name which is access from storj configuration and ObjectKey
which is access from storj configuration.\
It returns an objectinfo object, on successful 
execution it can be used to get other properties which are bound to it.

### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>bucketName</code> | Bucket name on storj V3 network | <code>string</code> |
| <code>objectName</code> | object name on storj V3 network | <code>string</code> |

### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
var objectName = "change-me-to-desired-object-name-on-storj";
await project.deleteObject(bucketName, objectName).then((objectinfo) => {
	...//some_code//...
}).catch((err) => {
    ....//some code//....
});
```

> NOTE: Following functions require Upload(Object) for calling.

## setCustomMetadata(Object)

### Description:

setCustomMetadata function set custom meta information, upload_object function 
is required as a pre-requisite for this function.\
This function accepts 1 argument CustomMetaData object which is access from storj library CustomMetadata contains custom user metadata about the object 
it returns an error object, if successful execution is not occurred.

### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>CustomMetaData</code> | Create using storj library | <code>object</code> |

### Usage Example:

```js
var customMetadataEntry1 =new storj.CustomMetadataEntry();
customMetadataEntry1.key = "change-me-to-metadata-key";
customMetadataEntry1.key_length = customMetadataEntry1.key.length;
customMetadataEntry1.value = "change-me-to--metadata-value";
customMetadataEntry1.value_length = customMetadataEntry1.value.length;
var customMetadataEntryArray = [ customMetadataEntry1];

var customMetadata = new storj.CustomMetadata();
customMetadata.count = customMetadataEntryArray.length;
customMetadata.entries = customMetadataEntryArray;

await upload.setCustomMetadata(customMetadata).then(() => {
	...//some_code//...
}).catch((err) => {
	....//some code//....
});
```

## write(Buffer, Int)

### Description:

write function uploads len(p) bytes from p to the object's data stream It 
returns the number of bytes written from p (0 <= n <= len(p)) and any error encountered 
that caused the write to stop early.\
upload_object function is required as a pre-requisite 
for this function. This function accepts 2 argument buffer object which is access from allocated buffer and 
Length is data file is being read it returns an writeresult object.\
On successful execution it can be used to get other properties which are bound to it.

### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>buffer</code> | Buffer | <code>object</code> |
| <code>lenght</code> | length of data to be upload on storj V3 network | <code>Int</code> |

### Usage Example:

```js
// creating buffer to store data.data will be stored in buffer that needs to be uploaded
var buffer = new Buffer.alloc(BUFFER_SIZE);
await upload.write(buffer, buffer.length).then((writeResult) => {
		...//some_code//...
}).catch((err) => {
	....//some code//....
});
```

## info()

### Description:

info function returns the last information about the uploaded object, upload_object function 
is required as a pre-requisite for this function.\ 
It returns an Object, on successful execution it can be use to get property which are bound to it.

### Usage Example:

```js
await upload.info().then((object) => {
		...//some_code//...
}).catch((err) => {
	....//some code//....
});
```

## commit()

### Description:

commit function commits the uploaded data, upload_object function 
is required as a pre-requisite for this function. it returns an error object, 
if successful execution is not occurred.

### Usage Example:

```js
await upload.commit().then(() => {
		...//some_code//...
}).catch((err) => {
	....//some code//....
});
```

## abort()

### Description:

abort function aborts an upload, upload_object function is required as 
a pre-requisite for this function. it returns an error object, 
if successful execution is not occurred.

### Usage Example:

```js
await upload.abort().then(() => {
		...//some_code//...
}).catch((err) => {
	....//some code//....
});
```

> NOTE: Following functions require Download(Object) for calling.

## close()

### Description:

close function closes the download, download_object function is required as 
a pre-requisite for this function. it returns an error object, 
if successful execution is not occurred.

### Usage Example:

```js
await download.close().then(() => {
	...//some_code//...
}).catch((err) => {
	....//some code//....
});
```

## read(Buffer, Int)

### Description:

read function downloads from object's data stream into bytes up to length amount, 
download_object function is required as a pre-requisite for this function.\
This function accepts 2 argument download(object) which is buffer object which is access from allocated buffer and Length is length of the buffer.\
It returns an readresult object, 
On successful execution it can be used to get other properties which are bound to it.

### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>buffer</code> | Buffer | <code>Int</code> |
| <code>Length</code> | buffer length | <code>Int</code> |

### Usage Example:

```js
var buffer = new Buffer.alloc(BUFFER_SIZE);

await download.read(buffer, buffer.length).then(async (bytesread) => {
	...//some_code//...
}).catch((err) => {
	....//some code//....
});
```

## info()

### Description:

Info function returns the last information about the object, download_object 
function is required as a pre-requisite for this function.\
it returns an download object. On successful execution it can be used to get other properties which are bound to it.

### Usage Example:

```js
await download.info().then((objectInfo) => {
	...//some_code//...
}).catch((err) => {
	....//some code//....
});
```

> NOTE: All the binding functions are asynchronous functions and return promises.

> NOTE: You can view the libuplink documentation [here](https://godoc.org/storj.io/uplink).