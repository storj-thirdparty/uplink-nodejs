# Binding Functions

## Include uplink in library 
```js
	var storj = require('uplink-storj');
	var libUplink = storj.uplink;
```
* we need to create an object of libUplink class that will be used to call the libuplink functions.

## request_access_with_passphrase(String, String, String)

##### Description:

This function request_access_with_passphrase  requests satellite for a new access grant
using a passhprase, there is no pre-requisites required for this function.\
This function accepts 3 arguments Satellite URL, API Key and  encryptionpassphrase
and returns an AccessResult object on successful execution which can be used to 
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
libUplink.request_access_with_passphrase(satelliteURL,apiKey,encryptionPassphrase).then(accessResult => {
		....//some code//....
}).catch((err) => {
    ....//some code//....
});
//
//OR
var accessResult = await libUplink.request_access_with_passphrase(satelliteURL,apiKey,encryptionPassphrase).catch((err) => {
    ....//some code//....
});
....//some code//....
```




## config_request_access_with_passphrase(Object, String, String, String)

##### Description:

This function config_request_access_with_passphrase requests satellite for a new access grant 
using a passhprase and config.\
There is no pre-requisites required for this function.\
This function accepts 4 arguments Satellite URL, API Key, encryptionpassphrase and config object and returns an AccessResult object on successful execution which can be used to call other functions which are bound to it.

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
libUplink.config_request_access_with_passphrase(config,satelliteURL,apiKey,encryptionPassphrase).then(accessResult => {
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
|<code>stringResult</code>| serialized access string returned by access_serialize function | <code>string</code> |

##### Usage Example:

```js	
libUplink.parse_access(stringResult).then(async (parsedSharedAccess) => {
		....//some code//....
}).catch((err) => {
    ....//some code//....
});
```


> NOTE: These functions require Access(Object) for calling.

## access_share(Object, Object, Object, Int)

##### Description:

access_share function creates new access grant with specific permission. Permission will be
applied to prefixes when defined.
parse access function is required as a pre-requisite for this function.\
this function accepts 4 arguments access(object) which is returned by access function ,
permission(object) Permission defines what actions can be used to share which is access 
from storj Permission defines what actions can be used to share, sharePrefix(object) 
which is access from storj, and prefixcount is getting from the count of share prefix 
in the list.\
It returns an access object on successful execution which can be used 
to call other functions which are bound to it.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>access</code>| access object returned by Access function | <code>object</code> |
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

await accessResult.access_share(accessResult.access,permission,Share Prefix,Prefix Count).then(async (sharedAccess) => {
	// generate serialized access to share
}).catch((err) => {
	....//some code//....
});
```


## access_serialize(Object)

##### Description:

access_serialize function serializes access grant into a string.\
parse access function is required as a pre-requisite for this function.
This function accepts one argument access(object) 
which is returned by access_share function.\
it returns an Serialized Access String 
on successful execution which is used to be as parse_access argument.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>access</code>| access object | <code>object</code> |

##### Usage Example:

```js
await sharedAccessResult.access_serialize(sharedAccessResult.access).then(async (stringResult) => {
	...//some_code//...
	}).catch((err) => {
	....//some code//....
});
```



## open_project(Object)

##### Description:

Once you have a valid access grant, you can open a Project with the access that access grant,
open_project function opens project using access grant.\
request_access_with_passphrase or config_request_access_with_passphrase function is required as a pre-requisite.\
this function accepts one argument access(object) which is returned by access function
it returns an project object on successful execution which can be used to call 
other functions which are bound to it.\
It allows you to manage buckets and objects within buckets.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>access</code>| access object | <code>object</code> |

##### Usage Example:

```js
accessResult.open_project(accessResult.access).then(async (projectResult) => {
		...//some code//...
}).catch((err) => {
	....//some code//....
});
```


## config_open_project(Object, Object)

##### Description:

config_open_project function opens project using access grant and config.\
request_access_with_passphrase or config_request_access_with_passphrase function
is required as a pre-requisite. This function accepts 2 argument access(object)
which is returned by access function and config(object) which is access from storj
library.\
it returns an project object on successful execution which can be used to call 
other functions which are bound to it.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>config</code>| Create using storj library | <code>object</code> |
|<code>access</code>| access object | <code>object</code> |

##### Usage Example:

```js
var config = new storj.config();
accessResult.config_open_project(config, accessResult.access).then(async (projectResult) => {
		...//some code//...
}).catch((err) => {
	....//some code//....
});
```



> NOTE: These functions require Project(Object) for calling.
## close_project(Object)

##### Description:

close_project function closes the project and open_project function is required as a pre-requisite.\
This function accepts one argument project(object) which is returned by open_project function
it returns an error object if on successful execution is not occurred.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>project</code>| project object | <code>object</code> |

##### Usage Example:

```js
await projectResult.close_project(projectResult.project).then(() => {
	...//some_code//...
}).catch((err) => {
    ....//some code//....
});
```


## stat_bucket(Object, String)

##### Description:

stat_bucket function returns information about a bucket and open_project function is 
required as a pre-requisite.\
This function accepts 2 argument project(object) which is returned by open_project function and bucket name which is access from storj configuration.\
it returns an bucket object on successful execution it can be used to get
other properties which are bound to it.


##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>project</code>| Project object | <code>object</code> |
|<code>bucketName</code>| Storj bucket name | <code>string</code> |

##### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
await projectResult.stat_bucket(projectResult.project,bucketName).then((bucketInfo) => {
	...//some code//...
}).catch((err) => {
	....//some code//....
});
```


## ensure_bucket(Object, String)


##### Description:

ensure_bucket function creates a new bucket and ignores the error when it 
already exists and open_project function is required as a pre-requisite.\
 This function accepts 2 argument project(object) which is returned by open_project 
function and bucket name which is access from storj configuration.\
It returns an bucket 
object on successful execution it can be used to get other properties 
which are bound to it.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>project</code>| project object returned by open_project function | <code>object</code> |
|<code>bucketName</code>| bucket name on storj V3 network | <code>string</code> |

##### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
await projectResult.ensure_bucket(projectResult.project,bucketName).then((bucketInfo) => {
...//some code//...
}).catch((err) => {
....//some code//....
});
```


## create_bucket(Object, String)

##### Description:

create_bucket function creates a new bucket When bucket already exists it returns 
a valid Bucket and ErrBucketExists and open_project function is required
as a pre-requisite.\
This function accepts 2 argument project(object) which is 
returned by open_project function and bucket name which is access from storj 
configuration.\
It returns an bucket object on successful execution it can be 
used to get other properties which are bound to it.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>project</code>| project object returned by open_project function | <code>object</code> |
|<code>bucketName</code>|access from storj configuration | <code>string</code> |

##### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
await projectResult.create_bucket(projectResult.project,bucketName).then((bucketInfo) => {
	...//some code//...
}).catch((err) => {
	....//some code//....
});
```


## delete_bucket(Object, String)

##### Description:

delete_bucket function deletes a bucket When bucket is not empty it returns ErrBucketNotEmpty.
and open_project function is requiredas a pre-requisite for this function .\
This function accepts 2 argument project(object) which is 
returned by open_project function and bucket name which is access from storj configuration.\
It returns an bucket object on successful execution it can be used to get other
properties which are bound to it.


##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>project</code>| project object returned by open_project function | <code>object</code> |
|<code>bucketName</code>| access from storj configuration | <code>string</code> |

##### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
await projectResult.delete_bucket(projectResult.project,bucketName).then((bucketInfo) => {
	...//some_code//...
}).catch((err) => {
	....//some code//....
});
```


## list_buckets(Object, Object)

##### Description:

lsitbuckets function lists buckets and open_project function is required
as a pre-requisite for this function .This function accepts 2 argument project(object) which is 
returned by open_project function and listBucketOptions which is access from storj library.\
it returns an bucketList object on successful execution it can be used to get other
properties which are bound to it.



##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>project</code>| project object | <code>object</code> |
|<code>listBucketOptions</code>| Create using storj library | <code>object</code> |

##### Usage Example:

```js
var listBucketsOptions = new storj.ListBucketsOptions();
await projectResult.listbuckets(projectResult.project,listBucketsOptions).then(async (bucketListResult) => {
	...//some_code//...
}).catch((err) => {
    ....//some code//....
});
```


## stat_object(Object, String, String)

##### Description:

stat_object function information about an object at the specific key and 
open_project function is required as a pre-requisite for this function.\
This function accepts 3 argument project(object) which is returned by open_project 
function, bucket name which is access from storj configuration and Object Key which is access from storj configuration.\
It returns an objectinfo object on successful execution it can be used to get other
properties which are bound to it.


##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>project</code>| project object | <code>object</code> |
|<code>bucketName</code>| Bucket name on storj V3 network | <code>string</code> |
|<code>objectName</code>| Object name on storj V3 network | <code>string</code> |

##### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
var objectName = "change-me-to-desired-object-name";
await projectResult.stat_object(projectResult.project,bucketName,objectName).then((objectinfo) => {
	...//some_code//...
}).catch((err) => {
    ....//some code//....
});
```


## list_objects(Object, String, Object)

##### Description:

list_objects function lists objects, open_project function is required as a pre-requisite 
for this function.\
This function accepts 3 argument project(object) which is returned by open_project 
function, bucket name which is access from storj configuration and listObjectOptions 
which is access from storj library ListObjectsOptions defines object listing options.\
it returns an objectList object, on successful execution it can be used to get 
other properties which are bound to it.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>project</code>| project object | <code>object</code> |
|<code>bucketName</code>| bucket name on storj V3 network | <code>string</code> |
|<code>listObjectOptions</code>| Create using storj library | <code>object</code> |

##### Usage Example:

```js		
var bucketName = "change-me-to-desired-bucket-name";
var listObjectsOptions = new storj.ListObjectsOptions();
await projectResult.list_objects(projectResult.project,bucketName,listObjectsOptions).then((objectlist) => {
	...//some_code//...
}).catch((err) => {
    ....//some code//....
});
```




## upload_object(Object, String, String, Object)

##### Description:

upload_object function starts an upload to the specified key, open_project 
function is required as a pre-requisite for this function.\
This function accepts 4 argument project(object) which is returned by open_project function, bucket name 
which is access from storj configuration, ObjectKey which is access from storj 
configuration and uploadOptions which is access from storj library UploadOptions 
contains additional options for uploading.\
It returns an upload object, on successful execution it can be used to call other properties which are bound to it.


##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>project</code>| project object | <code>object</code> |
|<code>bucketName</code>| Bucket name on storj V3 network | <code>string</code> |
|<code>objectName</code>| Object name to be uploaded on storj V3 network | <code>string</code> |
|<code>uploadOptions</code>| Create using storj library | <code>object</code> |

##### Usage Example:

```js		
var bucketName = "change-me-to-desired-bucket-name";
var objectName = "change-me-to-desired-object-name-on-storj";
var uploadOptions = new storj.UploadOptions();
await projectResult.upload_object(projectResult.project,bucketName,objectName,uploadOptions).then(async (uploadResult) => {  
		...//some_code//...
}).catch((err) => {
	....//some code//....
});
```


## download_object(Object, String, String, Object)

##### Description:

download_object function starts download to the specified key, open_project 
function is required as a pre-requisite for this function.\
This function accepts 4 argument project(object) which is returned by open_project function, bucket name 
which is access from storj configuration, ObjectKey which is access from storj 
configuration and downloadOptions which is access from storj library.\
It returns an download object, on successful execution it can be used to call other properties which are bound to it.


##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>project</code>| project object | <code>object</code> |
|<code>bucketName</code>| Bucket name on storj V3 network | <code>string</code> |
|<code>ObjectKey</code>| Object name already uploaded on storj V3 network | <code>string</code> |
|<code>downloadOptions</code>| Create using storj library | <code>object</code> |

##### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
var objectName = "change-me-to-desired-object-name-on-storj";
var downloadOptions = new storj.DownloadOptions();
await projectResult.download_object(projectResult.project,bucketName,objectName,downloadOptions).then(async (downloadresult) => {
	...//some_code//...
}).catch((err) => {
	....//some code//....
});
```

## delete_object(Object, String, String)


##### Description:

delete_object function deletes an object at the specific key, open_project function is required as a pre-requisite 
for this function.\
This function accepts 3 argument project(object) which is returned by open_project 
function, bucket name which is access from storj configuration and ObjectKey
which is access from storj configuration.\
It returns an objectinfo object, on successful 
execution it can be used to get other properties which are bound to it.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>project</code>| project object | <code>object</code> |
|<code>bucketName</code>| Bucket name on storj V3 network | <code>string</code> |
|<code>objectName</code>| object name on storj V3 network | <code>string</code> |

##### Usage Example:

```js
var bucketName = "change-me-to-desired-bucket-name";
var objectName = "change-me-to-desired-object-name-on-storj";
await projectResult.delete_object(projectResult.project,bucketName,objectName).then((objectinfo) => {
	...//some_code//...
}).catch((err) => {
    ....//some code//....
});
```



> NOTE: These functions require Upload(Object) for calling.

## upload_set_custom_metadata(Object, Object)

##### Description:

upload_set_custom_metadata function set custom meta information, upload_object function 
is required as a pre-requisite for this function.\
This function accepts 2 argument upload(object) which is returned by upload_object function and CustomMetaData object which is access from storj library CustomMetadata contains custom user metadata about the object 
it returns an error object, if successful execution is not occurred.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>upload</code>| upload object | <code>object</code> |
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

await uploadResult.upload_set_custom_metadata(uploadResult.upload,customMetadata).then(() => {
	...//some_code//...
}).catch((err) => {
	....//some code//....
});
```



## upload_write(Object, Buffer, Int)

##### Description:

upload_write function uploads len(p) bytes from p to the object's data stream It 
returns the number of bytes written from p (0 <= n <= len(p)) and any error encountered 
that caused the write to stop early.\
upload_object function is required as a pre-requisite 
for this function. This function accepts 3 argument upload(object) which is returned 
by upload_object function, buffer object which is access from allocated buffer and 
Length is data file is being read it returns an writeresult object.\
On successful execution it can be used to get other properties which are bound to it.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>upload</code>| upload object returned by upload_object function | <code>object</code> |
|<code>buffer</code>| Buffer | <code>object</code> |
|<code>lenght</code>| length of data to be upload on storj V3 network | <code>Int</code> |

##### Usage Example:

```js
// creating buffer to store data.data will be stored in buffer that needs to be uploaded
var buffer = new Buffer.alloc(BUFFER_SIZE);
await uploadResult.upload_write(uploadResult.upload,buffer,buffer.length).then((writeResult) => {
		...//some_code//...
}).catch((err) => {
	....//some code//....
});
```


## upload_info(Object)

##### Description:

upload_info function returns the last information about the uploaded object, upload_object function 
is required as a pre-requisite for this function. This function accepts one argument 
upload(object) which is returned by upload_object function.\
It returns an Object, on successful execution it can be use to get property which are bound to it.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>upload</code>| upload object returned by upload_object function | <code>object</code> |

##### Usage Example:

```js
await uploadResult.upload_info(uploadResultupload).then((object) => {
		...//some_code//...
}).catch((err) => {
	....//some code//....
});
```


## upload_commit(Object)

##### Description:

upload_commit function commits the uploaded data, upload_object function 
is required as a pre-requisite for this function. This function accepts one argument 
upload(object) which is returned by upload_object function it returns an error object, 
if successful execution is not occurred.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>upload</code>| upload object returned by upload_object function | <code>object</code> |

##### Usage Example:

```js
await uploadResult.upload_commit(uploadResult.upload).then(() => {
		...//some_code//...
}).catch((err) => {
	....//some code//....
});
```


## upload_abort(Object)

##### Description:

upload_abort function aborts an upload, upload_object function is required as 
a pre-requisite for this function. This function accepts one argument 
upload(object) which is returned by upload_object function and it returns an error object, 
if successful execution is not occurred.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>upload</code>| upload object returned by upload_object function | <code>object</code> |

##### Usage Example:

```js
await uploadResult.upload_abort(uploadResult.upload).then(() => {
		...//some_code//...
}).catch((err) => {
	....//some code//....
});
```



> NOTE: These functions require Download(Object) for calling.

## close_download(Object)

##### Description:

close_download function closes the download, download_object function is required as 
a pre-requisite for this function. This function accepts one argument 
download(object) which is returned by download_object function and it returns an error object, 
if successful execution is not occurred.

##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>download</code>| download object returned by download_object function | <code>object</code> |

##### Usage Example:

```js
await downloadResult.close_download(downloadResult.download).then(() => {
	...//some_code//...
}).catch((err) => {
	....//some code//....
});
```


## download_read(Object, Buffer, Int)

##### Description:

download_read function downloads from object's data stream into bytes up to length amount, 
download_object function is required as a pre-requisite for this function.\
This function accepts 3 argument download(object) which is returned by download_object function buffer object which is access from allocated buffer and Length is length of the buffer.\
It returns an readresult object,
On successful execution it can be used to get other properties which are bound to it.


##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>download</code>| download object returned by download_object function | <code>object</code> |
|<code>buffer</code>| Buffer | <code>Int</code> |
|<code>Length</code>| buffer length | <code>Int</code> |

##### Usage Example:

```js
var buffer = new Buffer.alloc(BUFFER_SIZE);

await downloadResult.download_read(downloadResult.download,buffer,buffer.length).then(async (bytesread) => {
	...//some_code//...
}).catch((err) => {
	....//some code//....
});
```

## download_info(Object)

##### Description:

download_Info returns the last information about the object, download_object 
function is required as a pre-requisite for this function.\
This function accepts one argument download(object) which is returned by download_object function and it returns an download object. On successful execution it can be used to get other properties which are bound to it.


##### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>download</code>| download object returned by download_object function | <code>object</code> |

##### Usage Example:

```js
await downloadResult.download_info(downloadResult.download).then((objectInfo) => {
	...//some_code//...
}).catch((err) => {
	....//some code//....
});
```


> NOTE: All the binding functions are asynchronous functions and return promises.

> NOTE: You can view the libuplink documentation [here](https://godoc.org/storj.io/uplink).