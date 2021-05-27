# <b>uplink-nodejs binding</b>

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/6acbf8b6d3dc42e98a30df0324334ca2)](https://app.codacy.com/gh/storj-thirdparty/uplink-nodejs?utm_source=github.com&utm_medium=referral&utm_content=storj-thirdparty/uplink-nodejs&utm_campaign=Badge_Grade_Dashboard)

#### *Developed using v1.2.2 storj/uplink-c*

## <b>Initial Set-up (Important)</b>

>For Golang
   * Make sure your `PATH` includes the `$GOPATH/bin` directory, so that your commands can be easily used [Refer: Install the Go Tools](https://golang.org/doc/install):
        ```
        export PATH=$PATH:$GOPATH/bin
        ```


* Install node-gyp globally
```
$ npm install -g node-gyp
```
* Install the storj-nodejs Node.js package
```
$ npm install uplink-nodejs
```

* Set Environment variable
  * MacOS
    * Set DYLD_LIBRARY_PATH  environment variable
      * Run following command inside root directory of your project
        ```
        export  DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$PWD/node_modules/uplink-nodejs/
        ```
      OR
    * Copy libuplinkc*.* files from $PROJECTROOT/node_modules/uplink-nodejs/ to /usr/local/lib
   * Windows
     * Set Path environment variable to libuplinkc*.* which is  $PROJECTROOT/node_modules/uplink-nodejs

>For NodeJS
 * Please ensure Node.js with version 10 or higher is [installed](https://nodejs.org/en/download/)
   * Check Node.js version
        ```
        $ node -v
        ```
 * please ensure *make* is already installed.
 * please ensure *node-gyp* dependencies is already installed.
 * please ensure *@types/node* dependencies is installed for running module in typescript.


## <b>Flow Diagram</b>

![](/_images/arch.drawio.png ':include :type=iframe width=100% height=1000px')

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