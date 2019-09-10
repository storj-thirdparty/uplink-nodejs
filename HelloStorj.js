// 'HelloStorj' example Node.js code using Node.js-Storj bindings

// include the Node.js-Storj bindings module
var libUplinkNode = require("./libUplinkNode.js");

// demo Storj (V3) configuration
var storjConfig = {
    apiKey              : "13Yqf9SJhb9ApNZQdY2H4h47pacMyQeAtGafLUygWhTuugCU17P5BffEZveFnP8ivv2dsrWFaKPZJnRupHHgkw9abTw3hzxpHt2td5Y",
    satelliteURL        : "us-central-1.tardigrade.io:7777",
    encryptionPassphrase: "test",
    bucketName          : "partnertest01",
    uploadPath          : "sample.txt",
};

var srcFullFileName = "sample.txt";
var downloadPath = "tst.txt";
// create new uplink
var uplinkRef = libUplinkNode.new_uplink();
// ensure uplink is created
if (uplinkRef._handle > 0) {
    //
    // parse API key
    var apiKeyRef = libUplinkNode.parse_api_key(storjConfig.apiKey);
    // ensure API key is parsed
    if (apiKeyRef._handle > 0) {
        //
        // open the desired Storj project, corresponding to the parsed API key
        var projectRef = libUplinkNode.open_project(uplinkRef, storjConfig.satelliteURL, apiKeyRef);
        // ensure Storj project is opened
        if (projectRef._handle > 0) {
            //
            // create a new bucket within the Storj project with desired name
            libUplinkNode.create_bucket_custom(projectRef, storjConfig.bucketName);
            //
            // Access Encryption passphrase within the Storj project
            var encryptionAccessRef = libUplinkNode.encryption_key_custom(projectRef, storjConfig.encryptionPassphrase);
            // ensure Encryption pass phrase is parsed 
            if (encryptionAccessRef._handle > 0) {
                //
                //  open the desired Storj bucket
                var bucketRef = libUplinkNode.open_bucket_custom(projectRef, storjConfig.bucketName, encryptionAccessRef);
                // ensure Bucket is opened
                if (bucketRef._handle > 0) {
                    // 
                    // Upload the file
                    libUplinkNode.upload_custom(bucketRef, storjConfig.uploadPath, srcFullFileName); 
                    //
                    // Download the uploaded file for verification
                    libUplinkNode.download_custom(bucketRef, srcFullFileName, downloadPath);
                    //
                    // close the bucket
                    libUplinkNode.close_bucket(bucketRef); 
                }
            }
            //
            // close the Storj project
            libUplinkNode.close_project(projectRef);
        }
    }
    //
    // close the uplink
    libUplinkNode.close_uplink(uplinkRef);
}
