# Binding Functions

## Include uplink in library 
```js
	const storj = require("uplink-nodejs");
	const libUplink = new storj.uplink();

```
* we need to create an object of libUplink class that will be used to call the libuplink functions.

## request_access_with_passphrase(String, String, String)

##### Description:

This function request_access_with_passphrase  requests satellite for a new access grant
using a passhprase, there is no pre-requisites required for this function.\
This function accepts 3 arguments Satellite URL, API Key and  encryptionpassphrase
and returns an access object on successful execution which can be used to 
call other functions which are bound to it.\
An access grant is a serialized structure that is internally comprised of an 
API Key, a set of encryption key information, and information about which Satellite
address is responsible for the metadata.\
An access grant is always associated with exactly one Project on one Satellite.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>satelliteURL</code>| Storj V3 network satellite address | <code>string</code> |
|<code>apikey</code>| Storj V3 network API key |<code>string</code> |
|<code>encryptionPassphrase</code>| Any passphrase |<code>string</code> |

##### Usage Example:

```js
var satelliteURL = "change-me-to-desired-satellite-address";
var apiKey = "change-me-to-desired-api-key";
var encryptionPassphrase = "change-me-to-desired-encryption";
libUplink.request_access_with_passphrase(satelliteURL,apiKey,encryptionPassphrase).then(access => {
		....//some code//....
}).catch((err) => {
    ....//some code//....
});
//
//OR
var access = await libUplink.request_access_with_passphrase(satelliteURL,apiKey,encryptionPassphrase).catch((err) => {
    ....//some code//....
});
....//some code//....
```




## config_request_access_with_passphrase(Object, String, String, String)

##### Description:

This function config_request_access_with_passphrase requests satellite for a new access grant 
using a passhprase and config.\
There is no pre-requisites required for this function.\
This function accepts 4 arguments Satellite URL, API Key, encryptionpassphrase and config object and returns an access object on successful execution which can be used to call other functions which are bound to it.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>config</code>| Create using storj library | <code>object</code> |
|<code>satelliteURL</code>|  Storj V3 network satellite address | <code>string</code> |
|<code>apikey</code>| Storj V3 network API key |<code>string</code> |
|<code>encryptionPassphrase</code>| any passphrase string |<code>string</code> |

##### Usage Example:

```js
var satelliteURL = "change-me-to-desired-satellite-address";
var apiKey = "change-me-to-desired-api-key";
var encryptionPassphrase = "change-me-to-desired-encryption";
var config = new storj.config();
libUplink.config_request_access_with_passphrase(config,satelliteURL,apiKey,encryptionPassphrase).then(access => {
		....//some code//....
}).catch((err) => {
    ....//some code//....
});
```



##  parse_access(String)

##### Description:

parse_access function to parses serialized access grant string there is no pre-requisites 
required for this function.\
this function accepts one argument serialized access String
which is returned by access_serialize function it returns an access object on successful 
execution which can be used to call other functions which are bound to it.\
This should be the main way to instantiate an access grant for opening a project.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>stringResult</code>| serialized access string returned by access.serialize function | <code>string</code> |

##### Usage Example:

```js	
libUplink.parse_access(stringResult).then(async (parsedSharedAccess) => {
		....//some code//....
}).catch((err) => {
    ....//some code//....
});
```


> NOTE: These functions require Access(Object) for calling.

## share(Object, Object, Int)

##### Description:

share function creates new access grant with specific permission. Permission will be
applied to prefixes when defined.
parse access function is required as a pre-requisite for this function.\
this function accepts 3 arguments permission(object) Permission defines what actions can be used to share which is access 
from storj Permission defines what actions can be used to share, sharePrefix(object) 
which is access from storj, and prefixcount is getting from the count of share prefix 
in the list.\
It returns an access object on successful execution which can be used 
to call other functions which are bound to it.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>permission</code>| Create using storj library |<code>object</code> |
|<code>SharePrefix</code>| Create using storj library |<code>object</code> |
|<code>PrefixCount</code>|count of share prefix |<code>Int</code> |

##### Usage Example:

```js		
var permission = new storj.Permission(true,true,true,true,0,0);
// set shared Prefix
var sharePrefix = storj.SharePrefix;
var sharePrefixListArray = [];
sharePrefix.bucket = "change-me-to-desired-bucket-name";
sharePrefix.prefix ="change-me-to-desired-object-prefix";
sharePrefixListArray.push(sharePrefix);

await access.share(permission,Share Prefix,Prefix Count).then(async (sharedAccess) => {
	// generate serialized access to share
}).catch((err) => {
	....//some code//....
});
```


## serialize()

##### Description:

serialize function serializes access grant into a string.\
parse access function is required as a pre-requisite for this function.
which is returned by access_share function.\
it returns an Serialized Access String 
on successful execution which is used to be as parse_access argument.


##### Usage Example:

```js
await sharedAccessResult.serialize().then(async (stringResult) => {
	...//some_code//...
	}).catch((err) => {
	....//some code//....
});
```



## open_project()

##### Description:

Once you have a valid access grant, you can open a Project with the access that access grant,
open_project function opens project using access grant.\
request_access_with_passphrase or config_request_access_with_passphrase function is required as a pre-requisite.\
it returns an project object on successful execution which can be used to call 
other functions which are bound to it.\
It allows you to manage buckets and objects within buckets.


##### Usage Example:

```js
access.open_project().then(async (project) => {
		...//some code//...
}).catch((err) => {
	....//some code//....
});
```


## config_open_project(Object)

##### Description:

config_open_project function opens project using access grant and config.\
request_access_with_passphrase or config_request_access_with_passphrase function
is required as a pre-requisite. This function accepts 1 argument config(object) which is access from storj
library.\
it returns an project object on successful execution which can be used to call 
other functions which are bound to it.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>config</code>| Create using storj library | <code>object</code> |


##### Usage Example:

```js
var config = new storj.config();
access.config_open_project(config).then(async (project) => {
		...//some code//...
}).catch((err) => {
	....//some code//....
});
```



> NOTE: These functions require Project(Object) for calling.
## close()

##### Description:

close function closes the project and open_project function is required as a pre-requisite.\
it returns an error object if on successful execution is not occurred.

##### Usage Example:

```js
await project.close().then(() => {
	...//some_code//...
}).catch((err) => {
    ....//some code//....
});
```


## stat_bucket(String)

##### Description:

stat_bucket function returns information about a bucket and open_project function is 
required as a pre-requisite.\
This function accepts 1 argument bucket name which is access from storj configuration.\
it returns an bucket object on successful execution it can be used to get
other properties which are bound to it.


##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>bucketName</code>| Storj bucket name | <code>string</code> |

##### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
await project.stat_bucket(bucketName).then((bucketInfo) => {
	...//some code//...
}).catch((err) => {
	....//some code//....
});
```


## ensure_bucket(String)


##### Description:

ensure_bucket function creates a new bucket and ignores the error when it 
already exists and open_project function is required as a pre-requisite.\
 This function accepts 1 argument bucket name which is access from storj configuration.\
It returns an bucket 
object on successful execution it can be used to get other properties 
which are bound to it.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>bucketName</code>| bucket name on storj V3 network | <code>string</code> |

##### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
await project.ensure_bucket(bucketName).then((bucketInfo) => {
...//some code//...
}).catch((err) => {
....//some code//....
});
```


## create_bucket(String)

##### Description:

create_bucket function creates a new bucket When bucket already exists it returns 
a valid Bucket and ErrBucketExists and open_project function is required
as a pre-requisite.\
This function accepts 1 argument bucket name which is access from storj 
configuration.\
It returns an bucket object on successful execution it can be 
used to get other properties which are bound to it.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>bucketName</code>|access from storj configuration | <code>string</code> |

##### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
await project.create_bucket(bucketName).then((bucketInfo) => {
	...//some code//...
}).catch((err) => {
	....//some code//....
});
```


## delete_bucket(String)

##### Description:

delete_bucket function deletes a bucket When bucket is not empty it returns ErrBucketNotEmpty.
and open_project function is requiredas a pre-requisite for this function .\
This function accepts 1 argument bucket name which is access from storj configuration.\
It returns an bucket object on successful execution it can be used to get other
properties which are bound to it.


##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>bucketName</code>| access from storj configuration | <code>string</code> |

##### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
await project.delete_bucket(bucketName).then((bucketInfo) => {
	...//some_code//...
}).catch((err) => {
	....//some code//....
});
```


## list_buckets(Object)

##### Description:

lsitbuckets function lists buckets and open_project function is required
as a pre-requisite for this function .This function accepts 1 argument listBucketOptions which is access from storj library.\
it returns an bucketList object on successful execution it can be used to get other
properties which are bound to it.



##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>listBucketOptions</code>| Create using storj library | <code>object</code> |

##### Usage Example:

```js
var listBucketsOptions = new storj.ListBucketsOptions();
await project.listbuckets(listBucketsOptions).then(async (bucketListResult) => {
	...//some_code//...
}).catch((err) => {
    ....//some code//....
});
```


## stat_object(String, String)

##### Description:

stat_object function information about an object at the specific key and 
open_project function is required as a pre-requisite for this function.\
This function accepts 2 argument bucket name which is access from storj configuration and Object Key which is access from storj configuration.\
It returns an objectinfo object on successful execution it can be used to get other
properties which are bound to it.


##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>bucketName</code>| Bucket name on storj V3 network | <code>string</code> |
|<code>objectName</code>| Object name on storj V3 network | <code>string</code> |

##### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
var objectName = "change-me-to-desired-object-name";
await project.stat_object(bucketName,objectName).then((objectinfo) => {
	...//some_code//...
}).catch((err) => {
    ....//some code//....
});
```


## list_objects(String, Object)

##### Description:

list_objects function lists objects, open_project function is required as a pre-requisite 
for this function.\
This function accepts 2 argument bucket name which is access from storj configuration and listObjectOptions 
which is access from storj library ListObjectsOptions defines object listing options.\
it returns an objectList object, on successful execution it can be used to get 
other properties which are bound to it.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>bucketName</code>| bucket name on storj V3 network | <code>string</code> |
|<code>listObjectOptions</code>| Create using storj library | <code>object</code> |

##### Usage Example:

```js		
var bucketName = "change-me-to-desired-bucket-name";
var listObjectsOptions = new storj.ListObjectsOptions();
await project.list_objects(bucketName,listObjectsOptions).then((objectlist) => {
	...//some_code//...
}).catch((err) => {
    ....//some code//....
});
```




## upload_object(String, String, Object)

##### Description:

upload_object function starts an upload to the specified key, open_project 
function is required as a pre-requisite for this function.\
This function accepts 3 argument bucket name 
which is access from storj configuration, ObjectKey which is access from storj 
configuration and uploadOptions which is access from storj library UploadOptions 
contains additional options for uploading.\
It returns an upload object, on successful execution it can be used to call other properties which are bound to it.


##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>bucketName</code>| Bucket name on storj V3 network | <code>string</code> |
|<code>objectName</code>| Object name to be uploaded on storj V3 network | <code>string</code> |
|<code>uploadOptions</code>| Create using storj library | <code>object</code> |

##### Usage Example:

```js		
var bucketName = "change-me-to-desired-bucket-name";
var objectName = "change-me-to-desired-object-name-on-storj";
var uploadOptions = new storj.UploadOptions();
await project.upload_object(bucketName,objectName,uploadOptions).then(async (upload) => {  
		...//some_code//...
}).catch((err) => {
	....//some code//....
});
```


## download_object(String, String, Object)

##### Description:

download_object function starts download to the specified key, open_project 
function is required as a pre-requisite for this function.\
This function accepts 3 argument  bucket name 
which is access from storj configuration, ObjectKey which is access from storj 
configuration and downloadOptions which is access from storj library.\
It returns an download object, on successful execution it can be used to call other properties which are bound to it.


##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>bucketName</code>| Bucket name on storj V3 network | <code>string</code> |
|<code>ObjectKey</code>| Object name already uploaded on storj V3 network | <code>string</code> |
|<code>downloadOptions</code>| Create using storj library | <code>object</code> |

##### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
var objectName = "change-me-to-desired-object-name-on-storj";
var downloadOptions = new storj.DownloadOptions();
await project.download_object(bucketName,objectName,downloadOptions).then(async (download) => {
	...//some_code//...
}).catch((err) => {
	....//some code//....
});
```

## delete_object(String, String)


##### Description:

delete_object function deletes an object at the specific key, open_project function is required as a pre-requisite 
for this function.\
This function accepts 2 argument  bucket name which is access from storj configuration and ObjectKey
which is access from storj configuration.\
It returns an objectinfo object, on successful 
execution it can be used to get other properties which are bound to it.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>bucketName</code>| Bucket name on storj V3 network | <code>string</code> |
|<code>objectName</code>| object name on storj V3 network | <code>string</code> |

##### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
var objectName = "change-me-to-desired-object-name-on-storj";
await project.delete_object(bucketName,objectName).then((objectinfo) => {
	...//some_code//...
}).catch((err) => {
    ....//some code//....
});
```



> NOTE: These functions require Upload(Object) for calling.

## set_custom_metadata(Object)

##### Description:

set_custom_metadata function set custom meta information, upload_object function 
is required as a pre-requisite for this function.\
This function accepts 1 argument CustomMetaData object which is access from storj library CustomMetadata contains custom user metadata about the object 
it returns an error object, if successful execution is not occurred.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>CustomMetaData</code>| Create using storj library | <code>object</code> |

##### Usage Example:

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

await upload.set_custom_metadata(customMetadata).then(() => {
	...//some_code//...
}).catch((err) => {
	....//some code//....
});
```



## write(Buffer, Int)

##### Description:

write function uploads len(p) bytes from p to the object's data stream It 
returns the number of bytes written from p (0 <= n <= len(p)) and any error encountered 
that caused the write to stop early.\
upload_object function is required as a pre-requisite 
for this function. This function accepts 2 argument buffer object which is access from allocated buffer and 
Length is data file is being read it returns an writeresult object.\
On successful execution it can be used to get other properties which are bound to it.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>buffer</code>| Buffer | <code>object</code> |
|<code>lenght</code>| length of data to be upload on storj V3 network | <code>Int</code> |

##### Usage Example:

```js
// creating buffer to store data.data will be stored in buffer that needs to be uploaded
var buffer = new Buffer.alloc(BUFFER_SIZE);
await upload.write(buffer,buffer.length).then((writeResult) => {
		...//some_code//...
}).catch((err) => {
	....//some code//....
});
```


## info()

##### Description:

info function returns the last information about the uploaded object, upload_object function 
is required as a pre-requisite for this function.\ 
It returns an Object, on successful execution it can be use to get property which are bound to it.


##### Usage Example:

```js
await upload.info().then((object) => {
		...//some_code//...
}).catch((err) => {
	....//some code//....
});
```


## commit()

##### Description:

commit function commits the uploaded data, upload_object function 
is required as a pre-requisite for this function. it returns an error object, 
if successful execution is not occurred.


##### Usage Example:

```js
await upload.commit().then(() => {
		...//some_code//...
}).catch((err) => {
	....//some code//....
});
```


## abort()

##### Description:

abort function aborts an upload, upload_object function is required as 
a pre-requisite for this function. it returns an error object, 
if successful execution is not occurred.

##### Usage Example:

```js
await upload.abort().then(() => {
		...//some_code//...
}).catch((err) => {
	....//some code//....
});
```



> NOTE: These functions require Download(Object) for calling.

## close()

##### Description:

close function closes the download, download_object function is required as 
a pre-requisite for this function. it returns an error object, 
if successful execution is not occurred.

##### Usage Example:

```js
await download.close().then(() => {
	...//some_code//...
}).catch((err) => {
	....//some code//....
});
```


## read(Buffer, Int)

##### Description:

read function downloads from object's data stream into bytes up to length amount, 
download_object function is required as a pre-requisite for this function.\
This function accepts 2 argument download(object) which is buffer object which is access from allocated buffer and Length is length of the buffer.\
It returns an readresult object,
On successful execution it can be used to get other properties which are bound to it.


##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>buffer</code>| Buffer | <code>Int</code> |
|<code>Length</code>| buffer length | <code>Int</code> |

##### Usage Example:

```js
var buffer = new Buffer.alloc(BUFFER_SIZE);

await download.read(buffer,buffer.length).then(async (bytesread) => {
	...//some_code//...
}).catch((err) => {
	....//some code//....
});
```

## info()

##### Description:

Info function returns the last information about the object, download_object 
function is required as a pre-requisite for this function.\
 it returns an download object. On successful execution it can be used to get other properties which are bound to it.


##### Usage Example:

```js
await download.info().then((objectInfo) => {
	...//some_code//...
}).catch((err) => {
	....//some code//....
});
```


> NOTE: All the binding functions are asynchronous functions and return promises.

> NOTE: You can view the libuplink documentation [here](https://godoc.org/storj.io/uplink).