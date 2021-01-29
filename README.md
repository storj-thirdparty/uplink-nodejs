# <b>uplink-nodejs binding</b>

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/6acbf8b6d3dc42e98a30df0324334ca2)](https://app.codacy.com/gh/storj-thirdparty/uplink-nodejs?utm_source=github.com&utm_medium=referral&utm_content=storj-thirdparty/uplink-nodejs&utm_campaign=Badge_Grade_Dashboard)

#### *Developed using v1.2.0 storj/uplink-c*

#### Binding is tagged to uplink-c v1.2.0.

[API documentation and tutorial](https://storj-thirdparty.github.io/uplink-nodejs/#/)

## <b>Initial Set-up (Important)</b>

>NOTE: For Golang

Make sure your `PATH` includes the `$GOPATH/bin` directory, so that your commands can be easily used [Refer: Install the Go Tools](https://golang.org/doc/install):
```
export PATH=$PATH:$GOPATH/bin
```

>NOTE: For Nodejs 

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
$ npm install uplink-nodejs
```

NOTE:  please ensure *make* is already installed.

NOTE:  please ensure *node-gyp* dependencies is already installed.

NOTE: please ensure *@types/node* dependencies is installed for running module in typescript.

## <b>Run Unit Test Cases on Local</b>

Directory with unit test cases *test* can be found in the uplink-nodejs repository.

To run the test cases on a local system, you need to perform the following steps:
* clone the repo so that you have the test folder on your local system.

        directory_on_local
        └── test

* Add a test file parallel to the tests folder, add ```API Key``` in the file, and name it as ```secret.txt```. The directory structure would be something like this now:

        directory_on_local
        └── secret.txt
        └── test
            └── testcase.ts

* Navigate to the folder, here ```directory_on_local``` and use the following command to run through all the tests.
```
        $ npm test
```

## <b>Documentation</b>
For more information on function definations and diagrams, check out the [Detail](//github.com/storj-thirdparty/uplink-nodejs/wiki/Home) or jump to:
* [Uplink-nodejs Binding Functions](//github.com/storj-thirdparty/uplink-nodejs/wiki/#binding-functions)
* [Flow Diagram](//github.com/storj-thirdparty/uplink-nodejs/wiki/#flow-diagram)
* [libuplink Documentation](https://godoc.org/storj.io/uplink)
* [Testing](//github.com/storj-thirdparty/uplink-nodejs/wiki/#testing)
