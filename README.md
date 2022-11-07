# <b>uplink-nodejs binding</b>

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/6acbf8b6d3dc42e98a30df0324334ca2)](https://app.codacy.com/gh/storj-thirdparty/uplink-nodejs?utm_source=github.com&utm_medium=referral&utm_content=storj-thirdparty/uplink-nodejs&utm_campaign=Badge_Grade_Dashboard)

#### *Developed using v1.2.2 storj/uplink-c*

[API documentation and tutorial](https://storj-thirdparty.github.io/uplink-nodejs/#/)

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## <b> About The Project </b>

[Storj](https://storj.io) is one of the most popular Cloud Storage which is  Decentralized, Secure, Private, Reliable, Affordable

How it works: 
* Objects are encrypted
* Encrypted objects are split into smaller pieces.
* Object pieces are globally distributed.
* Objects are retrieved when you need them.

This project is built with C/C++ and the building result is binding libraries of uplink in nodejs.


## <b> Getting Started </b>

### Prerequisites
 * please ensure *make* is already installed.
 * proper C/C++ compiler toolchain, like *GCC*
 * please ensure Node.js with version 10 or higher is [installed](https://nodejs.org/en/download/)
    ```
    $ node -v
    ```
 * please ensure *node-gyp* dependencies is already installed.
    ```
    $ npm install -g node-gyp
    ```
  * Make sure your `PATH` includes the `$GOPATH/bin` directory, so that your commands can be easily used [Refer: Install the Go Tools](https://golang.org/doc/install):
    ```
    $ export PATH=$PATH:$GOPATH/bin
    ```

### Building the Package
 * download the source project
    ```
    $ git clone https://github.com/storj-thirdparty/uplink-nodejs.git
    ```
* after changing the source code, build it
  ```
  $ npm run preinstall
  ```

### Testing the Results
The source project includes testing script to test it locally.
* need to build and release the binaries to root directory of the project
  ```
  $ npm install
  ```
* creat *secret.txt* in root directory of project and input api key getting from access panel whiling creating Access Grant in [storj.io](https://storj.io)
* In */test/testcase.js*, change the *satelliteURL* with the information getting from access panel whiling creating Access Grant in [storj.io](https://storj.io)
* run the test
  ```
  $ npm test
  ```

## <b> Usage </b>
* Install the storj-nodejs Node.js package
  ```
  $ npm install uplink-nodejs
  ```
* Set Environment variable
  * MacOS
    * Set DYLD_LIBRARY_PATH  environment variable
      ```
      $ export  DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$PWD/node_modules/uplink-nodejs/
      ```
      OR
    * Copy libuplinkc*.* files from $PROJECTROOT/node_modules/uplink-nodejs/ to /usr/local/lib
  * Linux
    * Set LD_LIBRARY_PATH  environment variable
      ```
      $ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/node_modules/uplink-nodejs/
      ```
      OR
    * Copy libuplinkc*.* files from $PROJECTROOT/node_modules/uplink-nodejs/ to /usr/lib
   * Windows
     * Set Path environment variable to libuplinkc*.* which is  $PROJECTROOT/node_modules/uplink-nodejs
* In node application, import the package and call functions
  ```
  const storj = require("uplink-nodejs");
  const libUplink = new storj.Uplink();
  ```

## <b>Documentation</b>
For more information on function definations and diagrams, check out the [Detail](https://github.com/storj-thirdparty/uplink-nodejs/wiki/Home) or jump to:
* [Uplink-nodejs Binding Functions](https://github.com/storj-thirdparty/uplink-nodejs/wiki/#binding-functions)
* [Flow Diagram](https://github.com/storj-thirdparty/uplink-nodejs/wiki/#flow-diagram)
* [libuplink Documentation](https://godoc.org/storj.io/uplink)
* [Testing](https://github.com/storj-thirdparty/uplink-nodejs/wiki/#testing)
