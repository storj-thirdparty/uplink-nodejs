## Flow Diagram

![](https://github.com/storj-thirdparty/uplink-nodejs/blob/master/README.assets/arch.drawio.png)

## Binding Functions

>Note: All the functions, if unsuccessful, throw an exception that can be caught using try-except block. For implementation, refer to *hello_storj.js*.

## Uplink Functions

### requestAccessWithPassphrase(String, String, String)

#### Description

This function requestAccessWithPassphrase  requests satellite for a new access grant
using a passhprase, there is no pre-requisites required for this function.\
This function accepts 3 arguments Satellite URL, API Key and  encryptionpassphrase
and returns an access object on successful execution which can be used to 
call other functions which are bound to it.\
An access grant is a serialized structure that is internally comprised of an 
API Key, a set of encryption key information, and information about which Satellite
address is responsible for the metadata.\
An access grant is always associated with exactly one Project on one Satellite.

#### Arguments

| arguments | Description |  Type |
| --- | --- | --- |
|<code>satellite</code>| Storj V3 network satellite address | <code>string</code> |
|<code>api_key</code>| Storj V3 network API key |<code>string</code> |
|<code>passphrase</code>| Any passphrase |<code>string</code> |

### configRequestAccessWithPassphrase(Object, String, String, String)

#### Description

This function configRequestAccessWithPassphrase requests satellite for a new access grant 
using a passhprase and config.\
There is no pre-requisites required for this function.\
This function accepts 4 arguments Satellite URL, API Key, encryptionpassphrase and config object and returns an access object on successful execution which can be used to call other functions which are bound to it.

#### Arguments

| arguments | Description |  Type |
| --- | --- | --- |
|<code>config</code>| Create using uplink_python.module_classes | <code>object</code> |
|<code>satellite</code>|  Storj V3 network satellite address | <code>string</code> |
|<code>api_key</code>| Storj V3 network API key |<code>string</code> |
|<code>passphrase</code>| Any passphrase string |<code>string</code> |

###  parse_access(String)

#### Description

parse_access function to parses serialized access grant string there is no pre-requisites 
required for this function.\
this function accepts one argument serialized access String
which is returned by access_serialize function it returns an access object on successful 
execution which can be used to call other functions which are bound to it.\
This should be the main way to instantiate an access grant for opening a project.

#### Arguments

| arguments | Description |  Type |
| --- | --- | --- |
|<code>serialized_access</code>| Serialized access string returned by access.serialize function | <code>string</code> |

### uplinkDeriveEncryptionKey(String, Array)

#### Description:

uplinkDeriveEncryptionKey function derives a salted encryption key for passphrase using the salt. This function is useful for deriving a salted encryption key for users when implementing multitenancy in a single app bucket.\
This function takes 2 argumeents Encryption Passphrase and a byte array.
It returns the Salted Encryption Key.

#### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>encryptionPassphrase</code> | Any passphrase | <code>string</code> |
| <code>salt</code> | Any byte array | Array |

## Access Functions

These functions require Access(Object) for calling.

### serialize()

#### Description

serialize function serializes access grant into a string.\
parse access function is required as a pre-requisite for this function.
which is returned by access_share function.\
it returns an Serialized Access String 
on successful execution which is used to be as parse_access argument.

### share(Object, List, Int)

#### Description

share function creates new access grant with specific permission. Permission will be
applied to prefixes when defined.
parse access function is required as a pre-requisite for this function.\
this function accepts 3 arguments permissions(object) Permission defines what actions can be used to share which is access 
from storj Permission defines what actions can be used to share, shared_prefix
which is access from storj, and prefixcount is getting from the count of share prefix 
in the list.\
It returns an access object on successful execution which can be used 
to call other functions which are bound to it.

#### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>permission</code>| Create using storj library |<code>object</code> |
|<code>SharePrefix</code>| Create using storj library |<code>object</code> |
|<code>PrefixCount</code>|count of share prefix |<code>Int</code> |

### openProject()

#### Description

Once you have a valid access grant, you can open a Project with the access that access grant,
openProject function opens project using access grant.\
requestAccessWithPassphrase or configRequestAccessWithPassphrase function is required as a pre-requisite.\
it returns an project object on successful execution which can be used to call 
other functions which are bound to it.\
It allows you to manage buckets and objects within buckets.

### configOpenProject(Object)

#### Description

configOpenProject function opens project using access grant and config.\
requestAccessWithPassphrase or configRequestAccessWithPassphrase function
is required as a pre-requisite. This function accepts 1 argument config(object) which is access from storj
library.\
it returns an project object on successful execution which can be used to call 
other functions which are bound to it.

#### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>config</code>| Create using storj library | <code>object</code> |

### overrideEncryptionKey(String, String, Object)

#### Description:

overrideEncryptionKey function overrides the root encryption key for the prefix in bucket with encryptionKey. This function is useful for overriding the encryption key in user-specific access grants when implementing multitenancy in a single app bucket.
This function accepts 3 arguemnts Bucket name, Prefix and Encryption key.

#### Arguments:

| arguments | Description | Type |
| --- | --- | --- |
| <code>bucketName</code> | Storj bucket name | <code>string</code> |
| <code>prefix</code> | Storj prefix | <code>string</code> |
| <code>encryptionKey</code> | Create using storj library | <code>object</code> |

## Project Related Functions

These functions require Project(Object) for calling.

### close()

#### Description

close function closes the project and openProject function is required as a pre-requisite.\
it returns an error object if on successful execution is not occurred.

### ensureBucket(String)

#### Description

ensureBucket function creates a new bucket and ignores the error when it 
already exists and openProject function is required as a pre-requisite.\
 This function accepts 1 argument bucket name which is access from storj configuration.\
It returns an bucket 
object on successful execution it can be used to get other properties 
which are bound to it.

#### Arguments

| arguments | Description |  Type |
| --- | --- | --- |
|<code>bucket_name</code>| Bucket name on storj V3 network | <code>string</code> |

### statBucket(String)

#### Description

statBucket function returns information about a bucket and openProject function is 
required as a pre-requisite.\
This function accepts 1 argument bucket name which is access from storj configuration.\
it returns an bucket object on successful execution it can be used to get
other properties which are bound to it.

#### Arguments

| arguments | Description |  Type |
| --- | --- | --- |
|<code>bucket_name</code>| Bucket name on storj V3 network | <code>string</code> |

### createBucket(String)

#### Description

createBucket function creates a new bucket When bucket already exists it returns 
a valid Bucket and ErrBucketExists and openProject function is required
as a pre-requisite.\
This function accepts 1 argument bucket name which is access from storj 
configuration.\
It returns an bucket object on successful execution it can be 
used to get other properties which are bound to it.

#### Arguments

| arguments | Description |  Type |
| --- | --- | --- |
|<code>bucket_name</code>| Bucket name on storj V3 network | <code>string</code> |

### listBuckets()

#### Description

lsitBuckets function lists buckets and openProject function is required
as a pre-requisite for this function.\
This function takes 1 optional argument listObjectOptions which is accessed from module_classes.
It returns an bucketList object on successful execution it can be used to get other
properties which are bound to it.

| arguments | Description |  Type |
| --- | --- | --- |
|<code>list_bucket_options</code>| Create using storj library  | <code>object</code> |

### deleteBucket(String)

#### Description

deleteBucket function deletes a bucket When bucket is not empty it returns ErrBucketNotEmpty.
and openProject function is requiredas a pre-requisite for this function .\
This function accepts 1 argument bucket name which is access from storj configuration.\
It returns an bucket object on successful execution it can be used to get other
properties which are bound to it.

#### Arguments

| arguments | Description |  Type |
| --- | --- | --- |
|<code>bucket_name</code>| Bucket name on storj V3 network | <code>string</code> |

### statObject(String, String)

#### Description

statObject function information about an object at the specific key and 
openProject function is required as a pre-requisite for this function.\
This function accepts 2 argument bucket name which is access from storj configuration and Object Key which is access from storj configuration.\
It returns an objectinfo object on successful execution it can be used to get other
properties which are bound to it.

#### Arguments

| arguments | Description |  Type |
| --- | --- | --- |
|<code>bucket_name</code>| Bucket name on storj V3 network | <code>string</code> |
|<code>objectName</code>| Object path on storj V3 network | <code>string</code> |

### listObjects(String, Object)

#### Description

listObjects function lists objects, openProject function is required as a pre-requisite 
for this function.\
This function accepts 2 argument bucket name which is access from storj configuration and listObjectOptions 
which is access from storj library ListObjectsOptions defines object listing options.\
it returns an objectList object, on successful execution it can be used to get 
other properties which are bound to it.

#### Arguments

| arguments | Description |  Type |
| --- | --- | --- |
|<code>bucket_name</code>| Bucket name on storj V3 network | <code>string</code> |
|<code>list_object_options</code>| Create using storj library | <code>object</code> |

### deleteObject(String, String)

#### Description

deleteObject function deletes an object at the specific key, openProject function is required as a pre-requisite 
for this function.\
This function accepts 2 argument  bucket name which is access from storj configuration and ObjectKey
which is access from storj configuration.\
It returns an objectinfo object, on successful 
execution it can be used to get other properties which are bound to it.

#### Arguments

| arguments | Description |  Type |
| --- | --- | --- |
|<code>bucket_name</code>| Bucket name on storj V3 network | <code>string</code> |
|<code>objectName</code>| Object path on storj V3 network | <code>string</code> |

### uploadObject(String, String, Object)

#### Description

uploadObject function starts an upload to the specified key, openProject 
function is required as a pre-requisite for this function.\
This function accepts 3 argument bucket name 
which is access from storj configuration, ObjectKey which is access from storj 
configuration and uploadOptions which is access from storj library UploadOptions 
contains additional options for uploading.\
It returns an upload object, on successful execution it can be used to call other properties which are bound to it.

#### Arguments

| arguments | Description |  Type |
| --- | --- | --- |
|<code>bucket_name</code>| Bucket name on storj V3 network | <code>string</code> |
|<code>objectName</code>| Object path to be uploaded on storj V3 network | <code>string</code> |
|<code>upload_options</code>| Create using storj library| <code>object</code> |
   
### downloadObject(String, String, Object)

#### Description

downloadObject function starts download to the specified key, openProject 
function is required as a pre-requisite for this function.\
This function accepts 3 argument  bucket name 
which is access from storj configuration, ObjectKey which is access from storj 
configuration and downloadOptions which is access from storj library.\
It returns an download object, on successful execution it can be used to call other properties which are bound to it.

#### Arguments

| arguments | Description |  Type |
| --- | --- | --- |
|<code>bucket_name</code>| Bucket name on storj V3 network | <code>string</code> |
|<code>objectName</code>| Path to object already uploaded on storj V3 network | <code>string</code> |
|<code>download_options</code>| Create using storj library | <code>object</code> |

## Upload Funcitons

These functions require Upload(Object) for calling.

### setCustomMetadata(Object)

#### Description:

setCustomMetadata function set custom meta information, upload_object function 
is required as a pre-requisite for this function.\
This function accepts 1 argument CustomMetaData object which is access from storj library CustomMetadata contains custom user metadata about the object 
it returns an error object, if successful execution is not occurred.

#### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>CustomMetaData</code>| Create using storj library | <code>object</code> |

### write(Buffer, Int)

#### Description:

write function uploads len(p) bytes from p to the object's data stream It 
returns the number of bytes written from p (0 <= n <= len(p)) and any error encountered 
that caused the write to stop early.\
upload_object function is required as a pre-requisite 
for this function. This function accepts 2 argument buffer object which is access from allocated buffer and 
Length is data file is being read it returns an writeresult object.\
On successful execution it can be used to get other properties which are bound to it.

#### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>buffer</code>| Buffer | <code>object</code> |
|<code>lenght</code>| length of data to be upload on storj V3 network | <code>Int</code> |

### info()

#### Description:

info function returns the last information about the uploaded object, upload_object function 
is required as a pre-requisite for this function.\ 
It returns an Object, on successful execution it can be use to get property which are bound to it.

### commit()

#### Description:

commit function commits the uploaded data, upload_object function 
is required as a pre-requisite for this function. it returns an error object, 
if successful execution is not occurred.

### abort()

#### Description:

abort function aborts an upload, upload_object function is required as 
a pre-requisite for this function. it returns an error object, 
if successful execution is not occurred.

#### Download Related Functions

These functions require Download(Object) for calling.

### close()

#### Description:

close function closes the download, download_object function is required as 
a pre-requisite for this function. it returns an error object, 
if successful execution is not occurred.

### read(Buffer, Int)

#### Description:

read function downloads from object's data stream into bytes up to length amount, 
download_object function is required as a pre-requisite for this function.\
This function accepts 2 argument download(object) which is buffer object which is access from allocated buffer and Length is length of the buffer.\
It returns an readresult object,
On successful execution it can be used to get other properties which are bound to it.

#### Arguments:

| arguments | Description |  Type |
| --- | --- | --- |
|<code>buffer</code>| Buffer | <code>Int</code> |
|<code>Length</code>| buffer length | <code>Int</code> |

### info()

#### Description:

Info function returns the last information about the object, download_object 
function is required as a pre-requisite for this function.\
 it returns an download object. On successful execution it can be used to get other properties which are bound to it.

>NOTE: All the binding functions are asynchronous functions and return promises.

## Testing
Create file with name secret.txt at uplink-nodejs module root directory and paste storj API key in same file.

* Run testcases
```
npm test
```

> NOTE: Please ensure *npm install* command is already executed.

* The project has been tested on the following operating systems:
```
* Windows
	* Version: 10 Pro
	* Processor: Intel(R) Core(TM) i3-5005U CPU @ 2.00GHz 2.00GHz
	* Node version : v10.18.1

* macOS Catalina
	* Version: 10.15.4
	* Processor: 2.5 GHz Dual-Core Intel Core i5
	* Node version : 13.6.0

* ubuntu
	* Version: 16.04 LTS
	* Processor: AMD A6-7310 APU with AMD Radeon R4 Graphics × 4
	* Node version : 10.12.0
```
