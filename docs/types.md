# Types, Errors and Constants

## Types

### Config(String, Int, String)

#### Description:

Config defines configuration for using uplink library.

#### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>user_agent</code>| Name of the partner | <code>string</code> |
|<code>dial_timeout_milliseconds</code>| How long client should wait for establishing a connection to peers. | <code>int</code> |
|<code>temp_directory</code>| Where to save data during downloads to use less memory. | <code>string</code> |

#### Usage Example

```js
const storj = require('uplink-nodejs') 
config = new storj.Config();
const uplinkError = storj.errorhandle;
config = Config(user_agent="UserAgent")
try{	
	access.config_open_project(config).then(async (project) => {
		...//some code//...
    }).catch((err) => {
	....//some code//....
    });
} catch(err){
  ....//some code//....
}
```



### Permission(String, String, String, String, Int, Int)

#### Description:

Permission defines what actions can be used to share.

#### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>allow_download</code>| Gives permission to download the object's content |<code>string</code> |
|<code>allow_upload</code>| Gives permission to create buckets and upload new objects |<code>string</code> |
|<code>allow_list</code>| Gives permission to list buckets |<code>string</code> |
|<code>allow_delete</code>| Gives permission to delete buckets and objects |<code>string</code> |
|<code>not_before</code>| Restricts when the resulting access grant is valid for |<code>int</code> |
|<code>not_after</code>| Restricts when the resulting access grant is valid till |<code>int</code> |

#### Usage Example

```js
const storj = require('uplink-nodejs') 
const uplinkError = storj.errorhandle;

const permissions = new storj.Permission(allow_list=True, allow_delete=False)
try{	
	access.share(permissions,shared_prefix,prefixcount).then(async (sharedAccess) => {
		...//some code//...
    }).catch((err) => {
	....//some code//....
    });
} catch(err){
	....//some code//....
}
```

### SharePrefix(String, String)

#### Description:

SharePrefix defines a prefix that will be shared.

#### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>bucket</code>| Bucket name on storj V3 network | <code>string</code> |
|<code>prefix</code>| Prefix is the prefix of the shared object keys | <code>string</code> |

>Note: that within a bucket, the hierarchical key derivation scheme is delineated by forward slashes (/), so encryption information will be included in the resulting access grant to decrypt any key that shares the same prefix up until the last slash.

#### Usage Example

```js
const storj = require('uplink-nodejs')
const uplinkError = storj.errorhandle; 

const shared_prefix = new storj.SharePrefix(bucket=bucketname, prefix="");
try{	
	access.share(permissions,shared_prefix,prefixcount).then(async (sharedAccess) => {
		...//some code//...
    }).catch((err) => {
	....//some code//....
    });
} catch(err){
     ....//some code//....
}
```

### CustomMetadata(Array, Int)

#### Description:

CustomMetadata contains a list of CustomMetadataEntry about the object.

#### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>entries</code>| List of CustomMetadataEntry about the object | <code>array of objects</code> |
|<code>count</code>| Number of entries | <code>int</code> |


#### Usage Example

```js
try{
	const storj = require('uplink-nodejs')
	const uplinkError = storj.errorhandle;
    var custom_metadata = new storj.CustomMetadata([CustomMetadataEntry(key="", key_length=0, value="", value_length=0)], 1)
    
    upload.set_custom_metadata(custom_metadata).then(() => {
		...//some code//...
    }).catch((err) => {
	....//some code//....
    });
} catch(err){
     ....//some code//....
}
```
### CustomMetadataEntry(String, Int, String, Int)

#### Description:

CustomMetadata contains custom user metadata about the object.

When choosing a custom key for your application start it with a prefix "app:key",
as an example application named"Image Board" might use a key "image-board:title".

#### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>key</code>| Key | <code>string</code> |
|<code>key_length</code>| Length of key | <code>int</code> |
|<code>value</code>| Value | <code>string</code> |
|<code>value_length</code>| Length of value | <code>int</code> |


#### Usage Example

```js
try{
	const storj = require('uplink-nodejs')
	const uplinkError = storj.errorhandle;
    const custom_metadata_entry = new storj.CustomMetadataEntry(key="", key_length=0, value="", value_length=0);
    
	const custom_metadata = new storj.CustomMetadata([custom_metadata_entry], 1);
    
    upload.set_custom_metadata(custom_metadata).then(() => {
		...//some code//...
    }).catch((err) => {
	....//some code//....
    });
} catch(err){
     ....//some code//....
}
```





### ListObjectsOptions(String, String, Bool, Bool, Bool)

#### Description:

ListObjectsOptions defines object listing options.

#### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>prefix</code>| Prefix allows to filter objects by a key prefix. If not empty, it must end with slash | <code>string</code> |
|<code>cursor</code>| Cursor sets the starting position of the iterator. The first item listed will be the one after the cursor | <code>string</code> |
|<code>recursive</code>| Recursive iterates the objects without collapsing prefixes | <code>bool</code> |
|<code>system</code>| System includes SystemMetadata in the results | <code>bool</code> |
|<code>custom</code>| Custom includes CustomMetadata in the results | <code>bool</code> |


#### Usage Example

```js
try {
	const storj = require('uplink-nodejs')
	const uplinkError = storj.errorhandle;
	const listobjectoptions = new storj.ListObjectsOptions(recursive=True)
    
    project.list_objects(bucketName,listObjectsOptions).then((objectlist) => {
	...//some_code//...
    }).catch((err) => {
    ....//some code//....
    });
} catch(err){
     ....//some code//....
}
```


### ListBucketsOptions(String)

#### Description:

ListBucketsOptions defines bucket listing options.

#### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>cursor</code>| Cursor sets the starting position of the iterator. The first item listed will be the one after the cursor | <code>string</code> |

#### Usage Example

```js
try {
	const storj = require('uplink-nodejs')
	const uplinkError = storj.errorhandle;
    const listbucketsoptions = new storj.ListBucketsOptions(cursor="")
    project.listbuckets(listBucketsOptions).then(async (bucketListResult) => {
	...//some_code//...
    }).catch((err) => {
    ....//some code//....
    });
} catch(err) {
     ....//some code//....
}
```

### UploadOptions(Int)

#### Description:

UploadOptions contains additional options for uploading.

#### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>expires</code>| When expires is 0 or negative, it means no expiration | <code>int</code> |

#### Usage Example

```js
try{
	const storj = require('uplink-nodejs')
	const uplinkError = storj.errorhandle;
	const uploadOptions = new storj.UploadOptions(expires=0)
    project.upload_object(bucketName,objectName,uploadOptions).then(async (upload) => {  
		...//some_code//...
    }).catch((err) => {
	....//some code//....
    });
} catch(err) {
     ....//some code//....
}
```

### DownloadOptions(Int, Int)

#### Description:

DownloadOptions contains additional options for downloading.

#### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>offset</code>| Offset to the download object | <code>int</code> |
|<code>length</code>| When length is negative, it will read until the end of the blob | <code>int</code> |

#### Usage Example

```js
try{
	const storj = require('uplink-nodejs')
	const uplinkError = storj.errorhandle;
	const downloadOptions = DownloadOptions(length=-1)
	project.download_object(bucketName,objectName,downloadOptions).then(async (download) => {
	...//some_code//...
    }).catch((err) => {
	....//some code//....
    });
} catch(err) {
     ....//some code//....
}
```
## Errors

### StorjException

#### Description:

StorjException is a broad category of all the errors thrown from uplink functions.\
It can be used to catch error thrown from uplink when one does not know what exception can be thrown from a particular function.

#### Returns:

| Returns | Description |  Type |
| --- | --- | --- |
|<code>code</code>| Error code related to error | <code>int</code> |
|<code>message</code>| Short description of error  |<code>string</code> |

#### Usage Example

```js
try{
	const uplinkError = storj.errorhandle;
    // function calls
    uplinkError.storjException(code,message);
} catch(err){
	....//some code//....
}
```

StorjException class is further sub-categorized into various error specific classes, These classes inherit all their properties from the base class, i.e. StorjException:

    InternalError
	BandwidthLimitExceededError
	BucketAlreadyExistError
	BucketNameInvalidError
	BucketNotEmptyError
	BucketNotFoundError
	ObjectKeyInvalidError
	ObjectNotFoundError
	TooManyRequestsError
	UploadDoneError
    CancelledError
    InvalidHandleError
    

### InternalError

InternalError is raised when there is issue in resolving request sent to uplink, it might be due to invalid or incorrent parameter passed.

### BandwidthLimitExceededError

BandwidthLimitExceededError is raised when when project will exceeded bandwidth limit.

### BucketAlreadyExistError

BucketAlreadyExistError is raised when the bucket already exists during creation.

### BucketNameInvalidError

BucketNameInvalidError is raised when the bucket name is invalid

### BucketNotEmptyError

BucketNotEmptyError is raised when the bucket is not empty during deletion.

### BucketNotFoundError

BucketNotFoundError is raised when the bucket is not found.

### ObjectKeyInvalidError

ObjectKeyInvalidError is raised when the object key is invalid.

### ObjectNotFoundError

ObjectNotFoundError is raised when the object is not found.

### TooManyRequestsError

TooManyRequestsError is raised when user has sent too many requests in a given amount of time.

### UploadDoneError

UploadDoneError is raised when either Abort or Commit has already been called.

### InvalidHandleError

InvalidHandleError is raised when object handle passes is either invalid or already freed.

## Constants

	ERROR_INTERNAL = 0x02
	ERROR_CANCELED = 0x03
	ERROR_INVALID_HANDLE = 0x04
	ERROR_TOO_MANY_REQUESTS = 0x05
	ERROR_BANDWIDTH_LIMIT_EXCEEDED = 0x06

	ERROR_BUCKET_NAME_INVALID = 0x10
	ERROR_BUCKET_ALREADY_EXISTS = 0x11
	ERROR_BUCKET_NOT_EMPTY = 0x12
	ERROR_BUCKET_NOT_FOUND = 0x13

	ERROR_OBJECT_KEY_INVALID = 0x20
	ERROR_OBJECT_NOT_FOUND = 0x21
	ERROR_UPLOAD_DONE = 0x22


> Note: You can view the libuplink documentation [here](https://godoc.org/storj.io/uplink).