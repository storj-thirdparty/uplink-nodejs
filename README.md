# node-storj binder

## Initial Set-up

**NOTE**: For Golang

    Make sure your `PATH` includes the `$GOPATH/bin` directory, so that your commands can be easily used [Refer: Install the Go Tools](https://golang.org/doc/install):
    ```
    export PATH=$PATH:$GOPATH/bin
    ```

    Install [storj-uplink](https://godoc.org/storj.io/storj/lib/uplink) go package, by running:
    ```
    $ go get storj.io/storj/lib/uplink
    ```

**NOTE**: For Nodejs 

    * Please ensure Node.js with version 10 or higher is [installed](https://nodejs.org/en/download/)

    * Check its version
    ```
    $ node -v
    ```


## Set-up Files

* After git cloning this project, store *libuplink_custom.go* file within ```$HOME/go/src/storj.io/storj/lib/uplinkc``` folder

* Create '.so' file at  ```$HOME/go/src/storj.io/storj/lib/uplinkc``` folder, by using following command:
```
$ go build -o libuplinkc.so -buildmode=c-shared 
```

* Copy *libuplinkc.so* file into the folder, where Node.js project was cloned

* Install the required Node.js packages
```
$ npm install ffi-napi
$ npm install ref-struct
```


## Sample Hello Storj!
The sample *HelloStorj.js* code calls the *libUplinkNode.js* binding module to do the following:
    * create a new bucket within desired Storj project
    * write a file from local computer to the created Storj bucket
    * read back the object from the Storj bucket to local PC for verification


## Node.js-Storj Binding Functions

**NOTE**: After calling a function, that returns a reference, ensure that the returned reference's _handle is positive, before using it further. Please refer the sample *HelloStorj.js* file for example.

### new_uplink()
    * function to create new Storj uplink
    * pre-requisites: None
    * inputs: None
    * output: UplinkRef

### parse_api_key(string)(APIKeyRef)
    * function to parse API key, to be used by Storj
    * pre-requisites: None
    * inputs: API key (string)
    * output: APIKeyRef

### open_project(UplinkRef,string,APIKeyRef)(ProjectRef)
    * function to open a Storj project
    * pre-requisites: new_uplink() and parse_api_key() functions have been already called
    * inputs: UplinkRef , Satellite Address (string) ,APIKeyRef
    * output: ProjectRef

### encryption_key_custom(ProjectRef,string)(EncryptionAccessRef)
    * function to get encryption access to upload and download data on Storj
    * pre-requisites: open_project() function has been already called
    * inputs: ProjectRef, Encryption Pass Phrase (string) ,
    * output: EncryptionAccessRef

### open_bucket_custom(ProjectRef,string,EncryptionAccessRef)(bucketRef)
    * function to open an already existing bucket in Storj project
    * pre-requisites: get_encryption_access() function has been already called
    * inputs: ProjectRef , Bucket Name (string) , EncryptionAccessRef
    * output: bucketRef

### upload_custom(bucketRef,string,string)
    * function to upload data from srcFullFileName (at local computer) to Storj (V3) bucket's path
    * pre-requisites: open_bucket() function has been already called
    * inputs: Storj Path/File Name (string) , Source Full File Name(string)
    * output: None

### download_custom(bucketRef,string,string)
    * function to download Storj (V3) object's data and store it in given file with destFullFileName (on local computer)
    * pre-requisites: open_bucket() function has been already called
    * inputs: bucketRef , Storj Path/File Name (string) , Destination Full File Name(string)
    * output: None

### create_bucket_custom(ProjectRef,string)
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

### close_bucket(bucketRef)
    * function to close currently opened Bucket
    * pre-requisites: open_bucket() function has been already called
    * inputs: bucketRef
    * output: None
