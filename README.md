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


**IMP**: Git clone this project within $HOME/go/src/ folder


## Set-up Files

* Create '.so' file by using following command:
```
$ go build -o libuplinkc.so -buildmode=c-shared libuplinkc.go
```

* Install the required Node.js packages
```
$ npm install ffi-napi
$ npm install ref-struct
```


* Create a `storj_config.json` file, with Storj network's configuration information in JSON format:
```json
    { 
        "apikey":     "change-me-to-the-api-key-created-in-satellite-gui",
        "satellite":  "mars.tardigrade.io:7777",
        "bucket":     "my-first-bucket",
        "uploadPath": "foo/bar/baz",
        "encryptionpassphrase": "test"
    }
```

* Store this file in a `config` folder



## Run the command-line tool

* Get help
```
    $ node node_storj_uploader.js h
```

* Upload a file from local system to Storj V3 bucket as per the Storj configuration file:
```
    $ node node_storj_uploader.js s ./config/storj_config.json <fileToUpload>.txt
```
