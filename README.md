# storj-nodejs binding
### *Developed using libuplinkc v0.30.7*
## Initial Set-up

**NOTE**: for Golang

Make sure your `PATH` includes the `$GOPATH/bin` directory, so that your commands can be easily used [Refer: Install the Go Tools](https://golang.org/doc/install):
```
export PATH=$PATH:$GOPATH/bin
```

Install [storj-uplink](https://godoc.org/storj.io/storj/lib/uplink) go package, by running:
```
$ go get storj.io/storj/lib/uplink
```
* Please ensure storj-uplink is installed at following location 
```
$ $HOME/go/src/
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
    * write a file from local system to the created Storj bucket
    * read back the object from the Storj bucket to local system for verification


## Node.js-Storj Binding Functions

### new_uplinkc(UplinkConfig,string)
    * function to create new Storj uplink
    * pre-requisites: None
    * inputs: UplinkConfig (object) , Temp(string)
    * output: Object contains error (if any ,else empty string) and data (UplinkRef)

### close_uplinkc(UplinkRef)
    * function to close currently established uplink
    * pre-requisites: new_uplinkc() function has been already called
    * inputs: Uplink Reference(UplinkRef)
    * output: Error if any else empty string

### parse_api_keyc(string)
    * function to parse API key, to be used by Storj
    * pre-requisites: None
    * inputs: API key (string)
    * output: object contains error (if any ,else empty string) and data (APIKeyRef)

### open_projectc(UplinkRef, string, APIKeyRef)
    * function to open a Storj project
    * pre-requisites: new_uplinkc() and parse_api_keyc() functions have been already called
    * inputs: valid UplinkRef, Satellite Address (string), valid APIKeyRef
    * output: object contains error (if any ,else empty string) and data (ProjectRef)

### close_projectc(ProjectRef)
    * function to close currently opened Storj project
    * pre-requisites: open_projectc() function has been already called
    * inputs: Project Reference (ProjectRef)
    * output: error if any else empty string


### list_bucketsc(ProjectRef)
    * function to fetch a list of buckets, available in the Storj project
    * pre-requisites: open_projectc() function has been already called
    * inputs: valid ProjectRef
    * output: object contains error (if any ,else empty string) and data (BucketList)

### list_objectsc(BucketRef)
    * function to fetch a list of buckets, available in the Storj project
    * pre-requisites: open_bucketc() function has been already called
    * inputs: valid BucketRef
    * output: object contains error (if any ,else empty string) and data (ObjectList)

### create_bucketc(ProjectRef, string)
    * function to create new bucket in Storj project
    * pre-requisites: open_projectc() function has been already called, successfully
    * inputs: valid ProjectRef, Bucket Name (string)
    * output: object contains error (if any ,else empty string) and data (BucketInfo)

### delete_bucketc(ProjectRef, string)
    * function to delete a bucket 
    * pre-requisites: open_projectc() function has been already called, successfully
    * inputs: valid ProjectRef, Bucket Name (string)
    * output: error if any else empty string 

### encryption_accessc(ProjectRef, string)
    * function to get encryption access to upload and/or download data to/from Storj
    * pre-requisites: open_projectc() function has been already called
    * inputs: valid ProjectRef, Encryption Pass Phrase (string)
    * output: object contains error (if any ,else empty string) and data (EncryptionAccessRef)

### project_salted_key_from_passphrasec(ProjectRef, string)
    * function to generate a key from given phassphrase
    * pre-requisites: open_projectc() function has been already called
    * inputs: valid ProjectRef, Encryption Pass Phrase (string)
    * output: object contains error (if any ,else empty string) and data (int)

### serialize_encryption_accessc(EncryptionAccessRef) (string)
    * function to turn encryption access into base58
    * pre-requisites: open_projectc() function has been already called
    * inputs: EncryptionAccessRef
    * output: object contains error (if any ,else empty string) and data (string)

### open_bucketc(ProjectRef, string, string) (BucketRef)
    * function to open an already existing bucket in Storj project
    * pre-requisites: encryption_keyc() function has been already called
    * inputs: ProjectRef, Bucket Name (string), serialized EncryptionAccess (string)
    * output: object contains error (if any ,else empty string) and data (APIKeyRef)

### close_bucketc(BucketRef)
    * function to close currently opened Storj project
    * pre-requisites: open_bucketc() function has been already called
    * inputs: Bucket Reference (BucketRef)
    * output: error if any else empty string

### delete_objectc(BucketRef, string)
    * function to delete an object in a bucket
    * pre-requisites: open_bucket() function has been already called, successfully
    * inputs: valid BucketRef, Object Path (string)
    * output: Error (string) if any else None

### uploadc(BucketRef, string, obj)
    * function to get uploader reference used to upload data to Storj (V3) bucket's path
    * pre-requisites: open_bucket() function has been already called
    * inputs: Bucket Reference (BucketRef), Storj Path/File Name (string) within the opened bucket, Upload Options (obj)
    * output: object contains error (if any ,else empty string) and data (UploadRef)
    
### upload_writec(uploaderRef, buffer, int)
    * function to write data to Storj (V3) bucket's path
    * pre-requisites: upload() function has been already called
    * inputs: Uploader reference (uploaderRef), Data to upload (buffer), Size of data to upload (int)
    * output: object contains error (if any ,else empty string) and data (int)
   * **Note:** The Data to upload (buffer) passed to function should be a uint8 pointer only. 
    
### upload_commitc(uploaderRef)
    * function to commit and finalize file for uploaded data to Storj (V3) bucket's path
    * pre-requisites: upload() function has been already called
    * inputs: Uploader reference (uploaderRef)
    * output: Error (string) if any else None
    
### downloadc(BucketRef, string)
    * function to get downloader reference to download Storj (V3) object's data and store it on local computer
    * pre-requisites: open_bucket() function has been already called
    * inputs: Bucket reference (BucketRef), Storj Path/File Name (string) within the opened bucket
    * output: Downloader reference (downloaderRef), Error (string) if any else None

### download_readc(downloaderRef, int)
    * function to read Storj (V3) object's data and return the data
    * pre-requisites: download() function has been already called
    * inputs: Downloader Reference (downloaderRef), Length of data to download (int)
    * output: object contains error (if any ,else empty string) and data (int)

### download_closec(downloaderRef)
    * function to close downloader after completing the data read process
    * pre-requisites: downloadc() function has been already called
    * inputs: Downloader Reference (downloaderRef)
    * output: Error (string) if any else None

### open_objectc(ObjectRef)
    * function to an Object handle, if authorized
    * pre-requisites: open_bucket() function has been already called
    * inputs: Bucket Reference (BucketRef),objectPath(string)
    * output: object contains error (if any ,else empty string) and data (ObjectRef)

### close_objectc(ObjectRef)
    * function to closes the object
    * pre-requisites: open_objectc() function has been already called
    * inputs: object Reference (ObjectRef)
    * output: error if any else empty string

### get_object_metac(ObjectRef)
    * function to close downloader after completing the data read process
    * pre-requisites: open_object() function has been already called
    * inputs: object Reference (ObjectRef)
    * output: object contains error (if any ,else empty string) and data (ObjectMeta)

### get_scope_satellite_addressc(ScopeRef)
    * function to gets Scope satellite address
    * pre-requisites: new_scopec() function has been already called
    * inputs: Scope Reference (ScopeRef)
    * output: object contains error (if any ,else empty string) and data (string)

### serialize_scopec(ScopeRef)
    * function to serializes the Scope to a string
    * pre-requisites: new_scopec() function has been already called
    * inputs: Scope Reference (ScopeRef)
    * output: object contains error (if any ,else empty string) and data (String)

### parse_scopec(ScopeRef)
    * function to parses an Scope
    * pre-requisites: new_scopec() function has been already called
    * inputs: ScopeRef Reference (ScopeRef)
    * output: object contains error (if any ,else empty string) and data (ScopeRef)

### get_scope_enc_accessc(ScopeRef)
    * function to get Scope encryption access
    * pre-requisites: new_scopec() function has been already called
    * inputs: Scope Reference (ScopeRef)
    * output: object contains error (if any ,else empty string) and data (EncryptionAccessRef)

### get_scope_api_keyc(ScopeRef)
    * function to get Scope APIKey
    * pre-requisites: new_scopec() function has been already called
    * inputs: scope Reference (ScopeRef)
    * output: object contains error (if any ,else empty string) and data (APIKeyRef)

### restrict_scopec(ScopeRef)
    * function to restricts a given scope with the provided caveat and encryption restrictions
    * pre-requisites: new_scope() function has been already called
    * inputs: scope reference (ScopeRef) , caveat(object),encryptionRestriction(object),restrictionsLen(number)
    * output: object contains error (if any ,else empty string) and data (ScopeRef)

### new_encryption_access_with_default_keyc(int)
    * function to  creates an encryption access context with a default key set
    * pre-requisites: new_scope() function has been already called
    * inputs: key(int)
    * output: object contains error (if any ,else empty string) and data (EncryptionAccessRef)

### new_scopec(ScopeRef)
    * function to creates new Scope	
    * pre-requisites: parse_api_keyc() and get_encryption_accessc() functions have been already called
    * inputs: satellite address (string), api key refernce (APIKeyRef),  serialized encryption access (EncryptionAccessRef)
    * output: object contains error (if any ,else empty string) and data (ScopeRef)

