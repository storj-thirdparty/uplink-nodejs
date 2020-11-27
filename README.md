# <b>uplink-nodejs binding</b>

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/6acbf8b6d3dc42e98a30df0324334ca2)](https://app.codacy.com/gh/storj-thirdparty/uplink-nodejs?utm_source=github.com&utm_medium=referral&utm_content=storj-thirdparty/uplink-nodejs&utm_campaign=Badge_Grade_Dashboard)

#### *Developed using v1.1.0 storj/uplink-c*

#### Binding is not tagged to any release and will use uplink-c master branch.

[API documentation and tutorial](https://storj-thirdparty.github.io/uplink-nodejs/#/)

## <b>Initial Set-up (Important)</b>

>NOTE: For Golang

Make sure your `PATH` includes the `$GOPATH/bin` directory, so that your commands can be easily used [Refer: Install the Go Tools](https://golang.org/doc/install):
```
export PATH=$PATH:$GOPATH/bin
```

>NOTE:: For Nodejs 

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

>NOTE:  please ensure *make* is already installed.

>NOTE:  please ensure *node-gyp* dependencies is already installed.

>NOTE: please ensure *@types/node* dependencies is installed for running module in typescript.

## <b>Documentation</b>
For more information on function definations and diagrams, check out the [Detail](//github.com/storj-thirdparty/uplink-nodejs/wiki/Home) or jump to:
* [Uplink-nodejs Binding Functions](//github.com/storj-thirdparty/uplink-nodejs/wiki/#binding-functions)
* [Flow Diagram](//github.com/storj-thirdparty/uplink-nodejs/wiki/#flow-diagram)
* [libuplink Documentation](https://godoc.org/storj.io/uplink)
* [Testing](//github.com/storj-thirdparty/uplink-nodejs/wiki/#testing)
