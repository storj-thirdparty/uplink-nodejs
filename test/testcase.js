//requiring modules
var expect = require("chai").expect;
let chai = require("chai");
const storj = require("../dist/uplink.js");
const libUplink = new storj.Uplink();
const fs = require("fs");
//
//Storj V3
var storjConfig = {
  apiKey    : "",
  satelliteURL   : "us-central-1.tardigrade.io:7777",
  encryptionPassphrase  : "test",
  bucketName   : "nodejstest",
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
      throw new Error("File does not exits");
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
function deleteObject(project){
  //
  describe('Delete Object',function(){
    return new Promise(async (resolve,reject) => {
      it("Delete Object from storj V3 network",async function(){
        return new Promise(async (resolve,reject) => {
          await project.deleteObject(storjConfig.bucketName,storjConfig.uploadPath).then((objectinfo) => { 
            describe('Delete object result', function() {
              it('Should have object info ',function(){
                objectinfo.should.be.a('object');
              })
            })
            resolve(true);
          }).catch((err) => {
            reject(err);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}

//Test case for deleting bucket
function deleteBucket(project){
  //Test case
  describe('Delete Bucket',function(){
    return new Promise(async (resolve,reject) => {
      it("Delete Bucket",async function(){
        return new Promise(async (resolve,reject) => {
          await project.deleteBucket(storjConfig.bucketName).then((bucketInfo) => { 
            resolve(true);
          }).catch((err) => {
             reject(err);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}  

//close project testcases
function closeProject(project){
  //Test case
  describe('Close project',function(){
    return new Promise(async (resolve,reject) => {
      it("Close project",async function(){
        return new Promise(async (resolve,reject) => {
          await project.close().then(() => {
            resolve(true);
          }).catch((err) => {
            reject(err);
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
    return new Promise(async (resolve,reject) => {
      it("Access Shared",async function(){
        return new Promise(async (resolve,reject) => {
          await libUplink.parseAccess(stringResult).then(async (parsedAccess) => {
            describe('parsed access share Result',function(){
              it("should have prase access",function(){
                parsedAccess.access._handle.should.be.a('number');
              });
            });

            describe('parsed access share Result',function(){
              it("should have access serialize function",function(){
               parsedAccess.should.have.property('serialize');
              });
            });

            describe('parsed access share Result',function(){
              it("should have open project function",function(){
               parsedAccess.should.have.property('openProject');
              });
            });
       
            describe(' parsed access share Result',function(){
              it("should have config open project function",function(){
               parsedAccess.should.have.property('configOpenProject');
              });
            });

            describe('parsed access share Result',function(){
              it("should have access share function",function(){
               parsedAccess.should.have.property('share');
              });
            });

            resolve(true);
          }).catch((err) => {
            reject(err);
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
    return new Promise(async (resolve,reject) => {
      it("Access Shared",async function(){
        return new Promise(async (resolve,reject) => {
          await sharedAccess.serialize(sharedAccess.access).then(async (stringResult) => {
            parsedAccess(stringResult);
            resolve(true);
          }).catch((err) => {
            reject(err);
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
    return new Promise(async (resolve,reject) => {
      it("Access Shared",async function(){
        return new Promise(async (resolve,reject) => {
          var permission = new storj.Permission(true,true,true,true,0,0);
          var sharePrefix = storj.SharePrefix;
          var sharePrefixListArray = [];
          sharePrefix.bucket = storjConfig.bucketName;
          sharePrefix.prefix ="filepath/";
          sharePrefixListArray.push(sharePrefix);
          await access.share(permission,sharePrefixListArray,sharePrefixListArray.length).then(async (sharedAccess) => {
            describe('access share Result',function(){
              it("should have share access",function(){
                sharedAccess.access._handle.should.be.a('number');
              });
            });
            describe('access share Result',function(){
              it("should have access serialize function",function(){
                sharedAccess.should.have.property('serialize');
              });
            });
            describe('access share Result',function(){
              it("should have open project function",function(){
                sharedAccess.should.have.property('openProject');
              });
            });
            describe('access share Result',function(){
              it("should have config open project function",function(){
                sharedAccess.should.have.property('configOpenProject');
              });
            });
            describe('access share Result',function(){
              it("should have access share function",function(){
                sharedAccess.should.have.property('share');
              });
            });
            //
            serializeAccess(sharedAccess);
            resolve(true);
          }).catch((err) => {
            reject(err);
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
    return new Promise(async (resolve,reject) => {
      it("Download",async function(){
        return new Promise(async (resolve,reject) => {
          var buffer = new Buffer.alloc(BUFFER_SIZE);
          await downloadresult.read(buffer,buffer.length).then(async (bytesread) => {
            describe('Close Download', function() {
              it('Close download function', async function() {
                return new Promise(async (resolve, reject) => {
                  await downloadresult.close(downloadresult.download).then(() => {
                  //  
                  }).catch((err) => {
                    reject(err)
                  });
                  resolve(true)
                });
              });
            });
            //
            resolve(true);
          }).catch((err) => {
            reject(err);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}

//Download object testcases
function downloadObject(project){
  describe('Download Object Function',function(){
    return new Promise(async (resolve,reject) => {
      it("Download",async function(){
        return new Promise(async (resolve,reject) => {
          var downloadOptions = new storj.DownloadOptions();
          downloadOptions.offset = 0;
          downloadOptions.length = -1;
          await project.downloadObject(storjConfig.bucketName,storjConfig.uploadPath,downloadOptions).then(async (downloadresult) => {
            var buffer = new Buffer.alloc(BUFFER_SIZE)
            describe('download object Result',function(){
              it('Should get download handle',function(){
                expect(downloadresult.download._handle).to.be.a('number');
              })
            })
            describe('download object Result',function(){
              it('Should have download read function ',function(){
                downloadresult.should.have.property('read');
              })
            })
            describe('download object Result',function(){
              it('Should have download info function ',function(){
                downloadresult.should.have.property('info');
              })
            })
            describe('download object Result',function(){
              it('Should have close download function ',function(){
                downloadresult.should.have.property('close');
              })
            })
            downloadRead(downloadresult);
            //
            deleteObject(project);
            //
            deleteBucket(project);
            //
            closeProject(project);
            //
            resolve(true);
          }).catch((err) => {
            reject(err);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}
//
function listObject(project){
  describe('list object function',function(){
    return new Promise(async (resolve,reject) => {
      it("List Object function",async function(){
        return new Promise(async (resolve,reject) => {
          var listObjectsOptions = new storj.ListObjectsOptions();
          await project.listObjects(storjConfig.bucketName,listObjectsOptions).then(async (objectlist) => {
           
            describe('list object Result',function(){
              it('Should get list of object',function(){
                objectlist.should.be.a('object');
              })
            })
            resolve(true);
          }).catch((err) => {
            reject(err);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}
//
function statObject(project){
  describe('Download Object Function',function(){
    return new Promise(async (resolve,reject) => {
      it("Download",async function(){
        return new Promise(async (resolve,reject) => {
          await project.statObject(storjConfig.bucketName,storjConfig.uploadPath).then((objectinfo) => {
          //
          describe('stat object Result ',function(){
            it('Should get information of object',function(){
              objectinfo.should.be.a('object');
            });
         });
          resolve(true)
        }).catch(err => {
          reject(err)
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
function uploadWrite(upload,project,accessResult){
  describe('Upload Write',function(){
    return new Promise(async (resolve,reject) => {
      it("Upload Write",async function(){
        return new Promise(async (resolve,reject) => {
          var buf = Buffer.from(str, 'utf-8')
          var bytesRead = buf.write(str, 0, buf.size, 'utf-8')

          await upload.write(buf,bytesRead).then(async (upload) => {
            describe('upload write Result', function() {
              it('Should have bytes written ',function(){
                expect(upload.bytes_written).to.be.a('number');
              });
            });
            resolve(true);
          }).catch((err) => {
            reject(err);
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
                await upload.setCustomMetadata(customMetadata).then(() => {
        
                }).catch((err) => {
                    reject(err)
                })
                resolve(true)
              })
            })                          
          })
          //
          describe('upload commit', function(err) {
            it("Upload commit function", async function() {
              return new Promise(async (resolve, reject) => {
                await upload.commit().then(() => {

                }).catch((err) => {
                    reject(err)
                  });
                resolve(true)
              });
            });                       
          });
          //
          describe('upload info', async function(err) {
            it("Upload info function", async function() {
              return new Promise(async (resolve, reject) => {
                await upload.info().then((object) => {
                  it('should have object', () => {
                      
                  });
                }).catch((err) => {
                    reject(err)
                  });
                resolve(true)
              });
            });                       
          }); 
          //
          downloadObject(project);
          //
          listObject(project);
          //
          statObject(project);
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
function uploadObject(project,accessResult){
  describe('Upload Object',function(){
    return new Promise(async (resolve,reject) => {
      it("Upload Object",async function(){
        return new Promise(async (resolve,reject) => {
          var uploadOptions = new storj.UploadOptions();
          uploadOptions.expires = 0;
          await project.uploadObject(storjConfig.bucketName,storjConfig.uploadPath,uploadOptions).then(async (upload) => {
            describe('upload Result', function() {
              it('Should have upload',function(){
                expect(upload.upload._handle).to.be.a('number');
              });
            });
            //
            describe('upload Result', function() {
              it('Should have upload write function',function(){
                upload.should.have.property('write');
              });
            });
            //
            describe('upload Result', function() {
              it('Should have upload commit function',function(){
                upload.should.have.property('commit')
              });
            });
            //
            describe('upload Result', function() {
              it('Should have upload info function',function(){
                upload.should.have.property('info')
              });
            });
            //
            describe('upload Result', function() {
              it('Should have upload abort function',function(){
                upload.should.have.property('abort')
              });
            });
            //
            describe('upload Result', function() {
              it('Should have upload set custom metadata function',function(){
                upload.should.have.property('setCustomMetadata')
              });
            });
            //
            uploadWrite(upload,project,accessResult);
            //
            resolve(true);
            //
          }).catch((err) => {
            reject(err);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}

// List bucket test cases
function listBucket(project){
  describe('List Bucket',function(){
    return new Promise(async (resolve,reject) => {
      it("Listing Bucket",async function(){
        return new Promise(async (resolve,reject) => {
          var listBucketsOptions = new storj.ListBucketsOptions();
          await project.listBuckets(listBucketsOptions).then(async (bucketListResult) => {
            describe('list bucket Result',function(){
              it("should have information",function(){
                bucketListResult.should.be.a('object');
              });
            });
            resolve(true);
          }).catch((err) => {
            reject(err);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}

// Ensured Bucket testcases
function ensureBucket(project){
  describe('Ensure Bucket',function(){
    return new Promise(async (resolve,reject) => {
      it("Ensure Bucket",async function(){
        return new Promise(async (resolve,reject) => {
          await project.ensureBucket(storjConfig.bucketName).then(async (bucketInfo) => {
            describe('ensure bucket Result',function(){
              it("should have information",function(){
                bucketInfo.name.should.equal(storjConfig.bucketName);
              });
            });
            resolve(true);
          }).catch((err) => {
            reject(err);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}

// Stat Bucket test cases
function statBucket(project){
  describe('Stat Bucket',function(){
    return new Promise(async (resolve,reject) => {
      it("Stat Bucket",async function(){
        return new Promise(async (resolve,reject) => {
          await project.statBucket(storjConfig.bucketName).then(async (bucketInfo) => {
            describe('stat bucket Result',function(){
              it("should have information",function(){
                bucketInfo.name.should.equal(storjConfig.bucketName);
              });
            });

            resolve(true);
          }).catch((err) => {
            reject(err);
          });
          resolve(true);
        });
      });
      resolve(true);
    });
  });
}

// Create bucket test cases
function createBucket(project,accessResult){
  describe('Create Bucket',function(){
    return new Promise(async (resolve,reject) => {
      it("Creating Bucket",async function(){
        return new Promise(async (resolve,reject) => {
          await project.createBucket(storjConfig.bucketName).then(async (bucketInfo) => {
            describe('create bucket Result',function(){
              it("should have information",function(){
                bucketInfo.name.should.equal(storjConfig.bucketName);
              });
            });
            //
            statBucket(project);
            //
            ensureBucket(project);
            //
            listBucket(project);
            //
            uploadObject(project,accessResult);
            //
            resolve(true);
            //
          }).catch((err) => {
            reject(err);
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
    return new Promise(async (resolve,reject) => {
      it("Opening project",async function(){
        return new Promise(async (resolve,reject) => {
          await accessResult.openProject().then(async (project) => {
            describe('Project Result', function() {
              it('Should have project',function(){
                expect(project.project._handle).to.be.a('number');
              });
            });
            //
            describe('Project Result', function() {
              it('Should have stat bucket function',function(){
                project.should.have.property('statBucket');
              });
            });
            //
            describe('Project Result', function() {
              it('Should have create bucket function',function(){
                project.should.have.property('createBucket')
              });
            });
            //
            describe('Project Result', function() {
              it('Should have ensure bucket function',function(){
                project.should.have.property('ensureBucket')
              });
            });
            //
            describe('Project Result', function() {
              it('Should have list bucket function',function(){
                project.should.have.property('listBuckets')
              });
            });
            //
            describe('Project Result', function() {
              it('Should have download object function',function(){
                project.should.have.property('downloadObject')
              });
            });
            //
            describe('Project Result', function() {
              it('Should have upload object function',function(){
                project.should.have.property('uploadObject')
              });
            });
            //
            describe('Project Result', function() {
              it('Should have stat object function',function(){
                project.should.have.property('statObject')
              });
            });
            //
            describe('Project Result', function() {
              it('Should have list object function',function(){
                project.should.have.property('listObjects')
              });
            });
            //
            describe('Project Result', function() {
              it('Should have delete object function',function(){
                project.should.have.property('deleteObject')
              });
            });
            //
            describe('Project Result', function() {
              it('Should have delete bucket function',function(){
                project.should.have.property('deleteBucket')
              });
            });
            //
            describe('Project Result', function() {
              it('Should have close project function',function(){
                project.should.have.property('close')
              });
            });
            //
            createBucket(project,accessResult);
            //
            resolve(true);
          }).catch((err) => {
            reject(err);
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
describe('Uplink NodeJS Test',function(){
//
  context('Request Access with phassphrase', function() {
    it("Request Access",function() {
        return new Promise(async (resolve, reject) => {
          //
          await libUplink.requestAccessWithPassphrase(storjConfig.satelliteURL,storjConfig.apiKey,storjConfig.encryptionPassphrase).then(async (access) => {
            //
            describe('Access Result', function() {
              it('Should have access',function(){
                expect(access.access._handle).to.be.a('number');
              });
            });

            describe('Access Result', function() {
              it('Should have open project function',function(){
                access.should.have.property('openProject');
              });
            });

            describe('Access Result', function() {
              it('Should have config open project function',function(){
                access.should.have.property('configOpenProject')
              });
            });

            describe('Access Result', function() {
              it('Should have access share function',function(){
                access.should.have.property('share')
              });
            });

            describe('Access Result', function() {
              it('Should have access serialize function',function(){
                access.should.have.property('serialize')
              });
            });
            //
            openProject(access);
            //
            resolve(true);
            //
          });
        }).catch((err) => {
          reject(err);
        });
        //
        resolve(true);
      });
  });
});

/*eslint-enable */