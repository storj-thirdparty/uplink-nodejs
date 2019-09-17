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

* Copy *libuplinkc.so* file into the folder, where the sample *HelloStorj.js* file is copied


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

### parse_api_key(string)(APIKeyRef)
    * function to parse API key, to be used by Storj
    * pre-requisites: None
    * inputs: API key (string)
    * output: valid APIKeyRef, else error in catch

### open_project(UplinkRef,string,APIKeyRef)(ProjectRef)
    * function to open a Storj project
    * pre-requisites: new_uplink() and parse_api_key() functions have been already called
    * inputs: valid UplinkRef, Satellite Address (string), valid APIKeyRef
    * output: valid ProjectRef, else error in catch

### encryption_key(ProjectRef,string) (EncryptionAccessRef)
    * function to get encryption access to upload and/or download data to/from Storj
    * pre-requisites: open_project() function has been already called
    * inputs: valid ProjectRef, Encryption Pass Phrase (string)
    * output: serialized EncryptionAccess (string), else error in catch

### create_bucket(ProjectRef,string)
    * function to create new bucket in Storj project
    * pre-requisites: open_project() function has been already called
    * inputs: bucketRef , Bucket Name (string)
    * output: None

### close_uplink(UplinkRef)
    * function to close currently opened uplink
    * pre-requisites: new_uplink() function has been already called
    * inputs: UplinkRef
    * output: None

### close_project(ProjectRef)
    * function to close currently opened Storj project
    * pre-requisites: open_project() function has been already called
    * inputs: ProjectRef
    * output: None

### open_bucket(ProjectRef, string, string) (BucketRef)
    * function to open an already existing bucket in Storj project
    * pre-requisites: encryption_key() function has been already called
    * inputs: ProjectRef, Bucket Name (string), serialized EncryptionAccess (string)
    * output: valid BucketRef, else error in catch

### close_bucket(BucketRef)
    * function to close currently open Bucket
    * pre-requisites: open_bucket() function has been already called
    * inputs: valid BucketRef
    * output: None, else error in catch

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
