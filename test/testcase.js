//requiring modules
var expect = require("chai").expect;
let chai = require("chai");
const storj = require("../uplink.js");
const libUplink = storj.uplink;
const fs = require("fs");
//
//Storj V3
var storjConfig = {
  apiKey    : "",
  satelliteURL   : "us-central-1.tardigrade.io:7777",
  encryptionPassphrase  : "test",
  bucketName   : "nodejsTest",
  uploadPath   : "filepath/sample.txt",
};
//
//
function readAPIKey(){
  try {
    var filename ="secret.txt";
    if(fs.existsSync(filename)){
      storjConfig.apiKey = fs.readFileSync(filename, {encoding:'utf8', flag:'r'});
    }else{
      throw new Error("secret.txt does not exist");
    }
  } catch(e){
    throw (e.message);
  }
}
readAPIKey();

//String to be uploaded on storj V3 network
var str = "Hello Storj";
//Buffer size
var BUFFER_SIZE= 80000;
//
/*eslint-disable */
var should = chai.should();
//Test case of deleting object
function deleteObject(projectResult){
  //
  describe('Delete Object',function(){
    return new Promise(async (resolve,reject)=>{
      it("Delete Object from storj V3 network",async function(){
        return new Promise(async (resolve,reject)=>{
          await projectResult.delete_object(projectResult.project,storjConfig.bucketName,storjConfig.uploadPath).then((objectinfo) => { 
            describe('Delete object result', function() {
              it('Should have object info ',function(){
                objectinfo.should.be.a('object');
              })
            })
            resolve(true);
          }).catch((err)=>{
            reject(false);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}

//Test case for deleting bucket
function deleteBucket(projectResult){
  //Test case
  describe('Delete Bucket',function(){
    return new Promise(async (resolve,reject)=>{
      it("Delete Bucket",async function(){
        return new Promise(async (resolve,reject)=>{
          await projectResult.delete_bucket(projectResult.project,storjConfig.bucketName).then((bucketInfo) => { 
            resolve(true);
          }).catch((err)=>{
             reject(false);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}  

//close project testcases
function closeProject(projectResult){
  //Test case
  describe('Close project',function(){
    return new Promise(async (resolve,reject)=>{
      it("Close project",async function(){
        return new Promise(async (resolve,reject)=>{
          await projectResult.close_project(projectResult.project).then(() => {
            resolve(true);
          }).catch((err)=>{
            reject(false);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}

//Parsed access test cases
function parsedAccess(stringResult){
  //Test case
  describe('Parsed',function(){
    return new Promise(async (resolve,reject)=>{
      it("Access Shared",async function(){
        return new Promise(async (resolve,reject)=>{
          await libUplink.parse_access(stringResult).then(async (parsedAccess) => {
            describe('parsed access share Result',function(){
              it("should have prase access",function(){
                parsedAccess.access._handle.should.be.a('number');
              });
            });

            describe('parsed access share Result',function(){
              it("should have access serialize function",function(){
               parsedAccess.should.have.property('access_serialize');
              });
            });

            describe('parsed access share Result',function(){
              it("should have open project function",function(){
               parsedAccess.should.have.property('open_project');
              });
            });
       
            describe(' parsed access share Result',function(){
              it("should have config open project function",function(){
               parsedAccess.should.have.property('config_open_project');
              });
            });

            describe('parsed access share Result',function(){
              it("should have access share function",function(){
               parsedAccess.should.have.property('access_share');
              });
            });

            resolve(true);
          }).catch((err)=>{
            reject(false);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}
//Access serialiazed testcases
function serializeAccess(sharedAccess){
  describe('Access Shared',function(){
    return new Promise(async (resolve,reject)=>{
      it("Access Shared",async function(){
        return new Promise(async (resolve,reject)=>{
          await sharedAccess.access_serialize(sharedAccess.access).then(async (stringResult) => {
            parsedAccess(stringResult);
            resolve(true);
          }).catch((err)=>{
            reject(false);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}

//Access share testcases
function accessShare(access){
  describe('Access Shared',function(){
    return new Promise(async (resolve,reject)=>{
      it("Access Shared",async function(){
        return new Promise(async (resolve,reject)=>{
          var permission = new storj.Permission(true,true,true,true,0,0);
          var sharePrefix = storj.SharePrefix;
          var sharePrefixListArray = [];
          sharePrefix.bucket = storjConfig.bucketName;
          sharePrefix.prefix ="filepath/";
          sharePrefixListArray.push(sharePrefix);
          await access.access_share(access.access,permission,sharePrefixListArray,sharePrefixListArray.length).then(async (sharedAccess) => {
            describe('access share Result',function(){
              it("should have share access",function(){
                sharedAccess.access._handle.should.be.a('number');
              });
            });
            describe('access share Result',function(){
              it("should have access serialize function",function(){
                sharedAccess.should.have.property('access_serialize');
              });
            });
            describe('access share Result',function(){
              it("should have open project function",function(){
                sharedAccess.should.have.property('open_project');
              });
            });
            describe('access share Result',function(){
              it("should have config open project function",function(){
                sharedAccess.should.have.property('config_open_project');
              });
            });
            describe('access share Result',function(){
              it("should have access share function",function(){
                sharedAccess.should.have.property('access_share');
              });
            });
            //
            serializeAccess(sharedAccess);
            resolve(true);
          }).catch((err)=>{
            reject(false);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}

//Download read testcases
function downloadRead(downloadresult){
  describe('Download Result Function',function(){
    return new Promise(async (resolve,reject)=>{
      it("Download",async function(){
        return new Promise(async (resolve,reject)=>{
          var buffer = new Buffer.alloc(BUFFER_SIZE);
          await downloadresult.download_read(downloadresult.download,buffer,buffer.length).then(async (bytesread) => {
            describe('Close Download', function() {
              it('Close download function', async function() {
                return new Promise(async (resolve, reject) => {
                  await downloadresult.close_download(downloadresult.download).then(() => {
                  //  
                  }).catch((err) => {
                    reject(false)
                  });
                  resolve(true)
                });
              });
            });
            //
            resolve(true);
          }).catch((err)=>{
            reject(false);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}

//Download object testcases
function downloadObject(projectResult){
  describe('Download Object Function',function(){
    return new Promise(async (resolve,reject)=>{
      it("Download",async function(){
        return new Promise(async (resolve,reject)=>{
          var downloadOptions = new storj.DownloadOptions();
          downloadOptions.offset = 0;
          downloadOptions.length = -1;
          await projectResult.download_object(projectResult.project,storjConfig.bucketName,storjConfig.uploadPath,downloadOptions).then(async (downloadresult) => {
            var buffer = new Buffer.alloc(BUFFER_SIZE)
            describe('download object Result',function(){
              it('Should get download handle',function(){
                expect(downloadresult.download._handle).to.be.a('number');
              })
            })
            describe('download object Result',function(){
              it('Should have download read function ',function(){
                downloadresult.should.have.property('download_read');
              })
            })
            describe('download object Result',function(){
              it('Should have download info function ',function(){
                downloadresult.should.have.property('download_info');
              })
            })
            describe('download object Result',function(){
              it('Should have close download function ',function(){
                downloadresult.should.have.property('close_download');
              })
            })
            downloadRead(downloadresult);
            //
            deleteObject(projectResult);
            //
            deleteBucket(projectResult);
            //
            closeProject(projectResult);
            //
            resolve(true);
          }).catch((err)=>{
            reject(false);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}
//
function listObject(projectResult){
  describe('list object function',function(){
    return new Promise(async (resolve,reject)=>{
      it("List Object function",async function(){
        return new Promise(async (resolve,reject)=>{
          var listObjectsOptions = new storj.ListObjectsOptions();
          await projectResult.list_objects(projectResult.project,storjConfig.bucketName,listObjectsOptions).then(async (objectlist) => {
           
            describe('list object Result',function(){
              it('Should get list of object',function(){
                objectlist.should.be.a('object');
              })
            })
            resolve(true);
          }).catch((err)=>{
            reject(false);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}
//
function statObject(projectResult){
  describe('Download Object Function',function(){
    return new Promise(async (resolve,reject)=>{
      it("Download",async function(){
        return new Promise(async (resolve,reject)=>{
          await projectResult.stat_object(projectResult.project,storjConfig.bucketName,storjConfig.uploadPath).then((objectinfo) => {
          //
          describe('stat object Result ',function(){
            it('Should get information of object',function(){
              objectinfo.should.be.a('object');
            });
         });
          resolve(true)
        }).catch(err => {
          reject(false)
        })
          resolve(true)
        });
      });
      resolve(true);
    });
  });
}
//
// Upload write test cases
function uploadWrite(uploadinfo,projectResult,accessResult){
  describe('Upload Write',function(){
    return new Promise(async (resolve,reject)=>{
      it("Upload Write",async function(){
        return new Promise(async (resolve,reject)=>{
          var buf = Buffer.from(str, 'utf-8')
          var bytesRead = buf.write(str, 0, buf.size, 'utf-8')

          await uploadinfo.upload_write(uploadinfo.upload,buf,bytesRead).then(async (uploadinfo) => {
            describe('upload write Result', function() {
              it('Should have bytes written ',function(){
                expect(uploadinfo.bytes_written).to.be.a('number');
              });
            });
            resolve(true);
          }).catch((err)=>{
            reject(false);
          });
          //
          var customMetadataEntry1 =new storj.CustomMetadataEntry();
          customMetadataEntry1.key = "testing";
          customMetadataEntry1.key_length = customMetadataEntry1.key.length;
          customMetadataEntry1.value = "testing1";
          customMetadataEntry1.value_length = customMetadataEntry1.value.length;
          
          var customMetadataEntry2 =new storj.CustomMetadataEntry();
          customMetadataEntry2.key = "value";
          customMetadataEntry2.key_length = customMetadataEntry2.key.length;
          customMetadataEntry2.value = "value1";
          customMetadataEntry2.value_length = customMetadataEntry2.value.length;
          
          var customMetadataEntryArray = [customMetadataEntry1, customMetadataEntry2];
          var customMetadata = new storj.CustomMetadata();
          customMetadata.count = customMetadataEntryArray.length;
          customMetadata.entries = customMetadataEntryArray;
          //
          describe('upload set custom metadata', function(err) {
            it("Upload set custom metadata function", async function() {
              return new Promise(async (resolve, reject) => {
                await uploadinfo.upload_set_custom_metadata(uploadinfo.upload,customMetadata).then(() => {
        
                }).catch((err) => {
                    reject(false)
                })
                resolve(true)
              })
            })                          
          })
          //
          describe('upload commit', function(err) {
            it("Upload commit function", async function() {
              return new Promise(async (resolve, reject) => {
                await uploadinfo.upload_commit(uploadinfo.upload).then(() => {

                }).catch((err) => {
                    reject(false)
                  });
                resolve(true)
              });
            });                       
          });
          //
          describe('upload info', async function(err) {
            it("Upload info function", async function() {
              return new Promise(async (resolve, reject) => {
                await uploadinfo.upload_info(uploadinfo.upload).then((object) => {
                  it('should have object', () => {
                      
                  });
                }).catch((err) => {
                    reject(false)
                  });
                resolve(true)
              });
            });                       
          }); 
          //
          downloadObject(projectResult);
          //
          listObject(projectResult);
          //
          statObject(projectResult);
          //
          accessShare(accessResult);
          //
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}

// Upload object test cases
function uploadObject(projectResult,accessResult){
  describe('Upload Object',function(){
    return new Promise(async (resolve,reject)=>{
      it("Upload Object",async function(){
        return new Promise(async (resolve,reject)=>{
          var uploadOptions = new storj.UploadOptions();
          uploadOptions.expires = 0;
          await projectResult.upload_object(projectResult.project,storjConfig.bucketName,storjConfig.uploadPath,uploadOptions).then(async (uploadinfo) => {
            describe('upload Result', function() {
              it('Should have upload',function(){
                expect(uploadinfo.upload._handle).to.be.a('number');
              });
            });
            //
            describe('upload Result', function() {
              it('Should have upload write function',function(){
                uploadinfo.should.have.property('upload_write');
              });
            });
            //
            describe('upload Result', function() {
              it('Should have upload commit function',function(){
                uploadinfo.should.have.property('upload_commit')
              });
            });
            //
            describe('upload Result', function() {
              it('Should have upload info function',function(){
                uploadinfo.should.have.property('upload_info')
              });
            });
            //
            describe('upload Result', function() {
              it('Should have upload abort function',function(){
                uploadinfo.should.have.property('upload_abort')
              });
            });
            //
            describe('upload Result', function() {
              it('Should have upload set custom metadata function',function(){
                uploadinfo.should.have.property('upload_set_custom_metadata')
              });
            });
            //
            uploadWrite(uploadinfo,projectResult,accessResult);
            //
            resolve(true);
            //
          }).catch((err)=>{
            reject(false);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}

// List bucket test cases
function listBucket(projectResult){
  describe('List Bucket',function(){
    return new Promise(async (resolve,reject)=>{
      it("Listing Bucket",async function(){
        return new Promise(async (resolve,reject)=>{
          var listBucketsOptions = new storj.ListBucketsOptions();
          await projectResult.listbuckets(projectResult.project,listBucketsOptions).then(async (bucketListResult) => {
            describe('list bucket Result',function(){
              it("should have information",function(){
                bucketListResult.should.be.a('object');
              });
            });
            resolve(true);
          }).catch((err)=>{
            reject(false);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}

// Ensured Bucket testcases
function ensureBucket(projectResult){
  describe('Ensure Bucket',function(){
    return new Promise(async (resolve,reject)=>{
      it("Ensure Bucket",async function(){
        return new Promise(async (resolve,reject)=>{
          await projectResult.ensure_bucket(projectResult.project,storjConfig.bucketName).then(async (bucketInfo) => {
            describe('ensure bucket Result',function(){
              it("should have information",function(){
                bucketInfo.name.should.equal(storjConfig.bucketName);
              });
            });
            resolve(true);
          }).catch((err)=>{
            reject(false);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}

// Stat Bucket test cases
function statBucket(projectResult){
  describe('Stat Bucket',function(){
    return new Promise(async (resolve,reject)=>{
      it("Stat Bucket",async function(){
        return new Promise(async (resolve,reject)=>{
          await projectResult.stat_bucket(projectResult.project,storjConfig.bucketName).then(async (bucketInfo) => {
            describe('stat bucket Result',function(){
              it("should have information",function(){
                bucketInfo.name.should.equal(storjConfig.bucketName);
              });
            });

            resolve(true);
          }).catch((err)=>{
            reject(false);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}

// Create bucket test cases
function createBucket(projectResult,accessResult){
  describe('Create Bucket',function(){
    return new Promise(async (resolve,reject)=>{
      it("Creating Bucket",async function(){
        return new Promise(async (resolve,reject)=>{
          await projectResult.create_bucket(projectResult.project,storjConfig.bucketName).then(async (bucketInfo) => {
            describe('create bucket Result',function(){
              it("should have information",function(){
                bucketInfo.name.should.equal(storjConfig.bucketName);
              });
            });
            //
            statBucket(projectResult);
            //
            ensureBucket(projectResult);
            //
            listBucket(projectResult);
            //
            uploadObject(projectResult,accessResult);
            //
            resolve(true);
            //
          }).catch((err)=>{
            reject(false);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}

// Open project test case
function openProject(accessResult){
  describe('Open Project Function',function(){
    return new Promise(async (resolve,reject)=>{
      it("Opening project",async function(){
        return new Promise(async (resolve,reject)=>{
          await accessResult.open_project(accessResult.access).then(async (projectResult) => {
            describe('Project Result', function() {
              it('Should have project',function(){
                expect(projectResult.project._handle).to.be.a('number');
              });
            });
            //
            describe('Project Result', function() {
              it('Should have stat bucket function',function(){
                projectResult.should.have.property('stat_bucket');
              });
            });
            //
            describe('Project Result', function() {
              it('Should have create bucket function',function(){
                projectResult.should.have.property('create_bucket')
              });
            });
            //
            describe('Project Result', function() {
              it('Should have ensure bucket function',function(){
                projectResult.should.have.property('ensure_bucket')
              });
            });
            //
            describe('Project Result', function() {
              it('Should have list bucket function',function(){
                projectResult.should.have.property('listbuckets')
              });
            });
            //
            describe('Project Result', function() {
              it('Should have download object function',function(){
                projectResult.should.have.property('download_object')
              });
            });
            //
            describe('Project Result', function() {
              it('Should have upload object function',function(){
                projectResult.should.have.property('upload_object')
              });
            });
            //
            describe('Project Result', function() {
              it('Should have stat object function',function(){
                projectResult.should.have.property('stat_object')
              });
            });
            //
            describe('Project Result', function() {
              it('Should have list object function',function(){
                projectResult.should.have.property('list_objects')
              });
            });
            //
            describe('Project Result', function() {
              it('Should have delete object function',function(){
                projectResult.should.have.property('delete_object')
              });
            });
            //
            describe('Project Result', function() {
              it('Should have delete bucket function',function(){
                projectResult.should.have.property('delete_bucket')
              });
            });
            //
            describe('Project Result', function() {
              it('Should have close project function',function(){
                projectResult.should.have.property('close_project')
              });
            });
            //
            createBucket(projectResult,accessResult);
            //
            resolve(true);
          }).catch((err)=>{
            reject(false);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}
//
//
// ##### Uplink NodeJS #####
describe('Uplink NodeJS Test',function(){
//
  describe('Request Access with phassphrase',function(){
    return new Promise(async (resolve, reject) => {
      //
      it("Request Function", async function() {
        return new Promise(async (resolve, reject) => {
          //
          await libUplink.request_access_with_passphrase(storjConfig.satelliteURL,storjConfig.apiKey,storjConfig.encryptionPassphrase).then(async (access)=>{
            //
            describe('Access Result', function() {
              it('Should have access',function(){
                expect(access.access._handle).to.be.a('number');
              });
            });

            describe('Access Result', function() {
              it('Should have open project function',function(){
                access.should.have.property('open_project');
              });
            });

            describe('Access Result', function() {
              it('Should have config open project function',function(){
                access.should.have.property('config_open_project')
              });
            });

            describe('Access Result', function() {
              it('Should have access share function',function(){
                access.should.have.property('access_share')
              });
            });

            describe('Access Result', function() {
              it('Should have access serialize function',function(){
                access.should.have.property('access_serialize')
              });
            });
            //
            openProject(access);
            //
            resolve(true);
            //
          });
        }).catch((err)=>{
          reject(false);
        });
        //
        resolve(true);
      });
      resolve(true);
    });
  });
});
/*eslint-enable */
