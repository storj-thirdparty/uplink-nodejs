# storj-nodejs binding
### *Developed using RC v1.0.1 storj/uplink-c 
## Initial Set-up

**NOTE**: for Golang

Make sure your `PATH` includes the `$GOPATH/bin` directory, so that your commands can be easily used [Refer: Install the Go Tools](https://golang.org/doc/install):
```
export PATH=$PATH:$GOPATH/bin
```

Install [storj-uplink-c](https://github.com/storj/uplink-c) go package, by running:
```
$ go get storj.io/uplink-c
```
* Please ensure GOPATH is set
* Please ensure uplink-c is installed at following location 
```
$ $GOPATH/go/src/
```
In case, following errors get reported during the process:
```
go/src/github.com/zeebo/errs/errs.go:42:9: undefined: errors.Unwrap
go/src/github.com/zeebo/errs/group.go:84:6: undefined: errors.Is
```
please arrange for the module dependencies to be met, using the Go modules functionality.
Reference: [ISSUE#3053: Getting error while downloading module](https://github.com/storj/storj/issues/3053#issuecomment-532883993)


**NOTE**: for Nodejs 

* Please ensure Node.js with version 10 or higher is [installed](https://nodejs.org/en/download/)

* Check its version
```
$ node -v
```
* Install node-gyp globally
```
$ npm install -g node-gyp
```
* Install the storj-nodejs Node.js package
```
$ npm install storj-nodejs
```



## Sample Hello Storj!
The sample *HelloStorj.js* code calls the *libUplinkNode.js* binding module to do the following:
    * create a new bucket within desired Storj project
    * list buckets on storj V3
    * write a file from local system to the created Storj bucket
    * list object on storj V3
    * read back the object from the Storj bucket to local system for verification
    * create share access
    * delete uploaded object
    * delete created bucket


## Node.js-Storj Binding Functions

### parse_accessc(String)
   * function parse_accessc to parses serialized access grant string
   * pre-requisites: TO DO
   * inputs: accessString (String)
   * output: AccessResult (Object)

### request_access_with_passphrasec(String, String, String)
   * function request_access_with_passphrasec requests satellite for a new access grant using a passhprase
   * pre-requisites: None
   * inputs: satellite address (String),API key (String) and passphrase (String)
   * output: AccessResult (Object)

### open_projectc(Access)
   * function open_project opens project using access grant.
   * pre-requisites: request_access_with_passphrasec
   * inputs: Access (Object)
   * output: ProjectResult (Object)

### close_projectc(Project)
   * function close_projectc closes the project.
   * pre-requisites: open_projectc
   * inputs: Project (Object)
   * output: Error (Object)

### stat_bucketc(Project, String)
   * function stat_bucketc returns information about a bucket.
   * pre-requisites: open_projectc
   * inputs: Project (Object) ,Bucket Name (String)
   * output: BucketResult (Object)

### ensure_bucketc(Project, String)
   * function ensure_bucketc creates a new bucket and ignores the error when it already exists
   * pre-requisites: open_projectc
   * inputs: Project (Object) ,Bucket Name (String)
   * output: BucketResult (Object)

### create_bucketc(Project, String)
   * function create_bucketc creates a new bucket.
   * pre-requisites: open_projectc
   * inputs: Project (Object) ,Bucket Name (String)
   * output: BucketResult (Object)

### delete_bucketc(Project, String)
   * function delete_bucketc deletes a bucket.
   * pre-requisites: open_projectc
   * inputs: Project (object) ,Bucket Name (string)
   * output: BucketResult (object)

### list_bucketsc(Project, ListBucketsOptions)
   * function list_bucketsc lists buckets
   * pre-requisites: open_projectc
   * inputs: Project (Object) ,ListBucketsOptions(Object)
   * output: BucketList (Object)

### stat_objectc(Project, String, String)
   * function stat_objectc information about an object at the specific key.
   * pre-requisites: open_projectc
   * inputs: Project (Object) ,Bucket Name (String) , Object Key(String)
   * output: ObjectResult (Object)

### upload_objectc(Project, String, String, Upload Options)
   * function upload_objectc starts an upload to the specified key.
   * pre-requisites: open_projectc
   * inputs: Project (Object) ,Bucket Name (String) , Object Key(String) , Uplaod Options(Object)
   * output: UploadResult  (Object)

### upload_writec(Upload, Pointer, size_t)
   * function upload_writec uploads len(p) bytes from p to the object's data stream.
   * pre-requisites: upload_objectc
   * inputs: Upload (Object) ,Pointer(Buffer) , Length (size_t)
   * output: WriteResult (Object)

### download_objectc(Project, String, String, Download Options)
   * function download_objectc starts  download to the specified key.
   * pre-requisites: open_projectc
   * inputs: Project (Object), Buxcket Name(String), Object Key(String), Download Options(Object)
   * output: DownloadResult (Object)

### upload_commitc(Upload)
   * function upload_commitc commits the uploaded data.
   * pre-requisites: upload_objectc
   * inputs: Upload (Object)
   * output: Error (Object)

### close_downloadc(Download)
   * function close_downloadc closes the download.
   * pre-requisites: download_objectc
   * inputs: Download (Object)
   * output: Error (Object)

### download_readc(Download, Pointer, size_t)
   * function download_readc downloads from object's data stream into bytes up to length amount.
   * pre-requisites: download_objectc
   * inputs: Download (Object), Pointer (Buffer), Length(size_t) 
   * output: ReadResult  (Object)

### delete_objectc(Project, String, String)
   * function delete_objectc deletes an object.
   * pre-requisites: open_projectc
   * inputs: Project (Object), Bucket Name (String), Object Key (String) 
   * output: ObjectResult  (Object)

### access_sharec(Access, Permission, SharePrefix, int)
   * function access_sharec creates new access grant with specific permission. Permission will be applied to prefixes when defined.
   * pre-requisites: parse_accessc
   * inputs: Access (Object), Permission (Object), Share Prefix (Object), prefix count (int)
   * output: String Result (Object)

### list_objectsc(Project, String, ListObjectsOptions) 
   * function list_objectsc lists objects
   * pre-requisites: open_projectc
   * inputs: Project (Object), Bucket Name(String), ListObjectsOptions(Object) 
   * output: ObjectList (Object)

### access_serializec(Access)
   * function access_serializec serializes access grant into a string.
   * pre-requisites: parse_accessc
   * inputs: Access (Object)
   * output: String Result (object)

### upload_infoc(Upload)
   * function upload_infoc returns the last information about the uploaded object.
   * pre-requisites: upload_objectc
   * inputs: Upload (Object)
   * output: Object Result (object)

### upload_abortc(Upload)
   * function upload_abortc aborts an upload.
   * pre-requisites: upload_objectc
   * inputs: Upload (Object)
   * output: Error (Object)

### download_infoc(Download)
   * function download_infoc returns information about the downloaded object.
   * pre-requisites: download_objectc
   * inputs: Download (Object)
   * output: Object Result (Object)

### upload_set_custom_metadatac(Upload, CustomMetadata)
   * function upload_set_custom_metadatac set custom meta information
   * pre-requisites: upload_objectc
   * inputs: Upload (Object), CustomMetadata 
   * output: Error (object)

## Tested On
### MacOS
   * OS version : 10.15.3
   * Node version : 13.6.0

### Windows
   * OS version : Windows 10
   * Node version : v10.18.1

### Ubuntu
   * OS version : 16.04
   * Node version : 10.12.0
