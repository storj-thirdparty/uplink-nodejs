# storj-nodejs binding

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

**NOTE**: for Nodejs 

* Please ensure Node.js with version 10 or higher is [installed](https://nodejs.org/en/download/)

* Check its version
```
$ node -v
```

* Install the storj-nodejs Node.js package
```
$ npm install storj-nodejs
```


## Set-up Files

* Copy the sample *HelloStorj.js* file from ```node_modules/storj-nodejs/``` folder to the location, where from npm package was installed

* Create '.so' file at  ```$HOME/go/src/storj.io/storj/lib/uplinkc``` folder, by using following command:
```
$ go build -o libuplinkc.so -buildmode=c-shared 
```

* Either set a *.env* file (similar to *.env.example*), with full path to the shared object, as follows:
```LIB_UPLINKC_PATH=/home/username/go/src/storj.io/storj/lib/uplinkc/libuplinkc.so```

* OR copy *libuplinkc.so* file into the folder, where the sample *HelloStorj.js* file is copied


## Sample Hello Storj!
The sample *HelloStorj.js* code calls the *libUplinkNode.js* binding module to do the following:
    * create a new bucket within desired Storj project
    * write a file from local system to the created Storj bucket
    * read back the object from the Storj bucket to local system for verification


## Node.js-Storj Binding Functions

### new_uplink()
    * function to create new Storj uplink
    * pre-requisites: None
    * inputs: None
    * output: valid UplinkRef, else error in catch

### close_uplink(UplinkRef)
    * function to close currently established uplink
    * pre-requisites: new_uplink() function has been already called
    * inputs: UplinkRef
    * output: None, else error in catch

### parse_api_key(string) (APIKeyRef)
    * function to parse API key, to be used by Storj
    * pre-requisites: None
    * inputs: API key (string)
    * output: valid APIKeyRef, else error in catch

### open_project(UplinkRef, string, APIKeyRef) (ProjectRef)
    * function to open a Storj project
    * pre-requisites: new_uplink() and parse_api_key() functions have been already called
    * inputs: valid UplinkRef, Satellite Address (string), valid APIKeyRef
    * output: valid ProjectRef, else error in catch

### close_project(ProjectRef)
    * function to close currently opened Storj project
    * pre-requisites: open_project() function has been already called
    * inputs: ProjectRef
    * output: None, else error in catch


### list_buckets(ProjectRef)
    * function to fetch a list of buckets, available in the Storj project
    * pre-requisites: open_project() function has been already called
    * inputs: valid ProjectRef
    * output: BucketList, else error in catch

### create_bucket(ProjectRef, string)
    * function to create new bucket in Storj project
    * pre-requisites: open_project() function has been already called, successfully
    * inputs: valid ProjectRef, Bucket Name (string)
    * output: None, else error in catch

### delete_bucket(ProjectRef, string)
    * function to delete a bucket 
    * pre-requisites: open_project() function has been already called, successfully
    * inputs: valid ProjectRef, Bucket Name (string)
    * output: None, else Error (string) in catch

### encryption_key(ProjectRef, string) (EncryptionAccessRef)
    * function to get encryption access to upload and/or download data to/from Storj
    * pre-requisites: open_project() function has been already called
    * inputs: valid ProjectRef, Encryption Pass Phrase (string)
    * output: serialized EncryptionAccess (string), else error in catch

### open_bucket(ProjectRef, string, string) (BucketRef)
    * function to open an already existing bucket in Storj project
    * pre-requisites: encryption_key() function has been already called
    * inputs: ProjectRef, Bucket Name (string), serialized EncryptionAccess (string)
    * output: valid BucketRef, else error in catch

### delete_object(BucketRef, string)
    * function to delete an object in a bucket
    * pre-requisites: open_bucket() function has been already called, successfully
    * inputs: valid BucketRef, Object Path (string)
    * output: Error (string) if any else None

### upload_file(BucketRef, string, string)
    * function to upload data from a file at local system to a Storj (V3) bucket's path
    * pre-requisites: open_bucket() function has been already called
    * inputs: valid BucketRef, Storj Path/File Name (string), Source Full File Name (string)
    * output: None, else error in catch

### download_file(BucketRef, string, string)
    * function to download Storj (V3) object's data and store it into given file at local system
    * pre-requisites: open_bucket() function has been already called
    * inputs: valid BucketRef, Storj Path/File Name (string), Destination Full File Name (string)
    * output: None, else error in catch

### upload(BucketRef, string, obj)
    * function to get uploader reference used to upload data to Storj (V3) bucket's path
    * pre-requisites: open_bucket() function has been already called
    * inputs: Bucket Reference (BucketRef), Storj Path/File Name (string) within the opened bucket, Upload Options (obj)
    * output: Uploader reference (uploaderRef), Error (string) if any else None
    
### upload_write(uploaderRef, buffer, int)
    * function to write data to Storj (V3) bucket's path
    * pre-requisites: upload() function has been already called
    * inputs: Uploader reference (uploaderRef), Data to upload (buffer), Size of data to upload (int)
    * output: Size of data uploaded (long), Error (string) if any else None
   * **Note:** The Data to upload (buffer) passed to function should be a uint8 pointer only. 
    
### upload_commit(uploaderRef)
    * function to commit and finalize file for uploaded data to Storj (V3) bucket's path
    * pre-requisites: upload() function has been already called
    * inputs: Uploader reference (uploaderRef)
    * output: Error (string) if any else None
    
### download(BucketRef, string)
    * function to get downloader reference to download Storj (V3) object's data and store it on local computer
    * pre-requisites: open_bucket() function has been already called
    * inputs: Bucket reference (BucketRef), Storj Path/File Name (string) within the opened bucket
    * output: Downloader reference (downloaderRef), Error (string) if any else None

### download_read(downloaderRef, int)
    * function to read Storj (V3) object's data and return the data
    * pre-requisites: download() function has been already called
    * inputs: Downloader Reference (downloaderRef), Length of data to download (int)
    * output: Data downloaded (buffer), Size of data downloaded (int), Error (string) if any else None

### download_close(downloaderRef)
    * function to close downloader after completing the data read process
    * pre-requisites: download() function has been already called
    * inputs: Downloader Reference (downloaderRef)
    * output: Error (string) if any else None

### close_bucket(BucketRef)
    * function to close currently open Bucket
    * pre-requisites: open_bucket() function has been already called
    * inputs: valid BucketRef
    * output: None, else error in catch
