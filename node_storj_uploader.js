var fs = require("fs");
var ffi = require("ffi-napi");
var Struct = require("ref-struct");

var GoString = Struct({
  string: "string",
  length: "longlong"
});

var goLibUplink = ffi.Library("./libuplinkc.so", {
  ConnectStorjUploadData:["void", [GoString, GoString, GoString, GoString, GoString, GoString]]
});

var gb_DEBUG = false;

// Storj V3 configuration file information
var storjConfigFile = {
    name: "./config/storj_config.json",
    content: {
        json: "",
        jsObject: null
    }
};

// default file contents
var file = {
    name: "helloStorj.txt",
    data: "Hello Storj! -Nodejs"
};

// check if either no command is given as third argument in the command line
// OR help or 'h' flag is set
var commandFlag = process.argv[2];
//
if ((commandFlag == null)
 || (commandFlag == "h" || commandFlag == "help")
 || (commandFlag != "s" && commandFlag != "store")) {
    // display help for proper usage at the command prompt
    console.log("NAME:\nNode.js Storj Uploader - Uploads a file from local system to the decentralized Storj network\n\nUSAGE:\nnode_storj_uploader.js command [arguments...]\n\nVERSION:\n1.0.1\n\nAUTHOR:\n Satyam Shivam - Utropicmedia\n\nCOMMANDS:\n store, s  Command to read and parse JSON information about Storj network and upload either a default 'Hello Storj!' message or desired file to given Storj Bucket\nhelp, h   Shows a list of commands or help for one command");
    process.exit(1);
}


// check if the command is to 'store' a file into Storj V3 network
if (commandFlag == "s" || commandFlag == "store") {
    // check if any config file's full path is given as fourth argument 
    if (process.argv[3] != null) {
        storjConfigFile.name = process.argv[3];
    } 
    //
    // ensure that the file exists
    if (fs.existsSync(storjConfigFile.name) == false) {
        console.error("FAILed to find the storj config file, ", storjConfigFile.name);
        process.exit(1);
    }
    //
    // read the JSON content of the config file and parse it
    storjConfigFile.content.json = fs.readFileSync(storjConfigFile.name);
    storjConfigFile.content.jsObject = JSON.parse(storjConfigFile.content.json);

    if (gb_DEBUG) {
        console.log("Read ", storjConfigFile.name, " config file:");
        console.log("satellite : ", storjConfigFile.content.jsObject.satellite);
        console.log("apiKey : ", storjConfigFile.content.jsObject.apikey);
        console.log("bucket : ", storjConfigFile.content.jsObject.bucket);
        console.log("uploadPath : ", storjConfigFile.content.jsObject.uploadPath);
        console.log("encryptionPassphrase : ", storjConfigFile.content.jsObject.encryptionpassphrase);
    }

    /* Read desired file and upload to Storj V3 bucket */

    // check if content file name (with full path) is given as argument 
    if (process.argv[4] != null) {
        file.name = process.argv[4];
        //
        // check if the file exists
        if (fs.existsSync(file.name)) {
            console.log("Reading content of ", file.name, " file")
            try {
                file.data = fs.readFileSync(file.name, 'utf8');
            } catch(e) {
                console.error('FAILed to read the file: ', e.stack);
            }
        } else {
            console.error("FAILed to find the file.");
            process.exit(1);
        }
    } else {
        console.log("\nNo content file name is given");
        console.log("Uploading default '", file.data, "' message in ", file.name, " object to Storj V3...");
    }

    
    // convert JavaScript objects to golang compatible strings
    satellite = jsObjectToGoString(storjConfigFile.content.jsObject.satellite);
    //
    apikey = jsObjectToGoString(storjConfigFile.content.jsObject.apikey);
    //
    bucket = jsObjectToGoString(storjConfigFile.content.jsObject.bucket);
    //
    uploadPath = jsObjectToGoString(storjConfigFile.content.jsObject.uploadPath + file.name);
    //
    encryptionpassphrase = jsObjectToGoString(storjConfigFile.content.jsObject.encryptionpassphrase);
    //
    fileData = jsObjectToGoString(file.data);
    
    // connect to Storj V3 and upload the file's content
    goLibUplink.ConnectStorjUploadData(satellite, apikey, bucket, uploadPath, encryptionpassphrase, fileData);
}

// convert a JavaScript object to golang String
function jsObjectToGoString(jsObject) {
    goString = new GoString();
    //
    goString["string"] = jsObject;
    goString["length"] = jsObject.length;
    //
    return goString;
}
