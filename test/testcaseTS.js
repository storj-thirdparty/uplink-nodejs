var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
var __generator = (this && this.__generator) || function (thisArg, body) {
    var _ = { label: 0, sent: function() { if (t[0] & 1) throw t[1]; return t[1]; }, trys: [], ops: [] }, f, y, t, g;
    return g = { next: verb(0), "throw": verb(1), "return": verb(2) }, typeof Symbol === "function" && (g[Symbol.iterator] = function() { return this; }), g;
    function verb(n) { return function (v) { return step([n, v]); }; }
    function step(op) {
        if (f) throw new TypeError("Generator is already executing.");
        while (_) try {
            if (f = 1, y && (t = op[0] & 2 ? y["return"] : op[0] ? y["throw"] || ((t = y["return"]) && t.call(y), 0) : y.next) && !(t = t.call(y, op[1])).done) return t;
            if (y = 0, t) op = [op[0] & 2, t.value];
            switch (op[0]) {
                case 0: case 1: t = op; break;
                case 4: _.label++; return { value: op[1], done: false };
                case 5: _.label++; y = op[1]; op = [0]; continue;
                case 7: op = _.ops.pop(); _.trys.pop(); continue;
                default:
                    if (!(t = _.trys, t = t.length > 0 && t[t.length - 1]) && (op[0] === 6 || op[0] === 2)) { _ = 0; continue; }
                    if (op[0] === 3 && (!t || (op[1] > t[0] && op[1] < t[3]))) { _.label = op[1]; break; }
                    if (op[0] === 6 && _.label < t[1]) { _.label = t[1]; t = op; break; }
                    if (t && _.label < t[2]) { _.label = t[2]; _.ops.push(op); break; }
                    if (t[2]) _.ops.pop();
                    _.trys.pop(); continue;
            }
            op = body.call(thisArg, _);
        } catch (e) { op = [6, e]; y = 0; } finally { f = t = 0; }
        if (op[0] & 5) throw op[1]; return { value: op[0] ? op[1] : void 0, done: true };
    }
};
var _this = this;
// Requiring modules
var expect = require("chai").expect;
var chai = require("chai");
var storj = require("../dist/uplink.js"), libUplink = new storj.Uplink();
var fs = require("fs"), 
/*
 *
 * Storj V3
 */
storjConfig = {
    "apiKey": "",
    "satelliteURL": "us-central-1.tardigrade.io:7777",
    "encryptionPassphrase": "test",
    "bucketName": "uplinknodejstest",
    "uploadPath": "filepath/sample.txt"
};
/*
 *
 *
 */
/* eslint-disable */
function readAPIKey() {
    try {
        var filename = "secret.txt";
        console.log("Reading file : ", __dirname + "/" + filename);
        if (fs.existsSync(__dirname + filename)) {
            storjConfig.apiKey = fs.readFileSync(filename, { "encoding": "utf8",
                "flag": "r" });
        }
        else {
            throw new Error("File does not exits : " + __dirname + "/" + filename);
        }
    }
    catch (e) {
        throw e.message;
    }
}
readAPIKey();
// String to be uploaded on storj V3 network
var str = "Hello Storj", 
// Buffer size
BUFFER_SIZE = 80000, 
//
should = chai.should();
// Test case of deleting object
function deleteObject(project) {
    var _this = this;
    //
    describe("Delete Object", function () { return new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
        var _this = this;
        return __generator(this, function (_a) {
            it("Delete Object from storj V3 network", function () { return __awaiter(_this, void 0, void 0, function () {
                var _this = this;
                return __generator(this, function (_a) {
                    return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                            return __generator(this, function (_a) {
                                switch (_a.label) {
                                    case 0: return [4 /*yield*/, project.deleteObject(storjConfig.bucketName, storjConfig.uploadPath).then(function (objectinfo) {
                                            describe("Delete object result", function () {
                                                it("Should have object info ", function () {
                                                    objectinfo.should.be.a("object");
                                                });
                                            });
                                            resolve(true);
                                        })["catch"](function (err) {
                                            reject(err);
                                        })];
                                    case 1:
                                        _a.sent();
                                        resolve(true);
                                        return [2 /*return*/];
                                }
                            });
                        }); })];
                });
            }); });
            resolve(true);
            return [2 /*return*/];
        });
    }); }); });
}
// Test case for deleting bucket
function deleteBucket(project) {
    var _this = this;
    // Test case
    describe("Delete Bucket", function () { return new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
        var _this = this;
        return __generator(this, function (_a) {
            it("Delete Bucket", function () { return __awaiter(_this, void 0, void 0, function () {
                var _this = this;
                return __generator(this, function (_a) {
                    return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                            return __generator(this, function (_a) {
                                switch (_a.label) {
                                    case 0: return [4 /*yield*/, project.deleteBucket(storjConfig.bucketName).then(function (bucketInfo) {
                                            resolve(true);
                                        })["catch"](function (err) {
                                            reject(err);
                                        })];
                                    case 1:
                                        _a.sent();
                                        resolve(true);
                                        return [2 /*return*/];
                                }
                            });
                        }); })];
                });
            }); });
            resolve(true);
            return [2 /*return*/];
        });
    }); }); });
}
// Close project testcases
function closeProject(project) {
    var _this = this;
    // Test case
    describe("Close project", function () { return new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
        var _this = this;
        return __generator(this, function (_a) {
            it("Close project", function () { return __awaiter(_this, void 0, void 0, function () {
                var _this = this;
                return __generator(this, function (_a) {
                    return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                            return __generator(this, function (_a) {
                                switch (_a.label) {
                                    case 0: return [4 /*yield*/, project.close().then(function () {
                                            resolve(true);
                                        })["catch"](function (err) {
                                            reject(err);
                                        })];
                                    case 1:
                                        _a.sent();
                                        resolve(true);
                                        return [2 /*return*/];
                                }
                            });
                        }); })];
                });
            }); });
            resolve(true);
            return [2 /*return*/];
        });
    }); }); });
}
// Parsed access test cases
function parsedAccess(stringResult) {
    var _this = this;
    // Test case
    describe("Parsed", function () { return new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
        var _this = this;
        return __generator(this, function (_a) {
            it("Access Shared", function () { return __awaiter(_this, void 0, void 0, function () {
                var _this = this;
                return __generator(this, function (_a) {
                    return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                            var _this = this;
                            return __generator(this, function (_a) {
                                switch (_a.label) {
                                    case 0: return [4 /*yield*/, libUplink.parseAccess(stringResult).then(function (parsedAccess) { return __awaiter(_this, void 0, void 0, function () {
                                            return __generator(this, function (_a) {
                                                describe("parsed access share Result", function () {
                                                    it("should have prase access", function () {
                                                        parsedAccess.access._handle.should.be.a("number");
                                                    });
                                                });
                                                describe("parsed access share Result", function () {
                                                    it("should have access serialize function", function () {
                                                        parsedAccess.should.have.property("serialize");
                                                    });
                                                });
                                                describe("parsed access share Result", function () {
                                                    it("should have open project function", function () {
                                                        parsedAccess.should.have.property("openProject");
                                                    });
                                                });
                                                describe(" parsed access share Result", function () {
                                                    it("should have config open project function", function () {
                                                        parsedAccess.should.have.property("configOpenProject");
                                                    });
                                                });
                                                describe("parsed access share Result", function () {
                                                    it("should have access share function", function () {
                                                        parsedAccess.should.have.property("share");
                                                    });
                                                });
                                                resolve(true);
                                                return [2 /*return*/];
                                            });
                                        }); })["catch"](function (err) {
                                            reject(err);
                                        })];
                                    case 1:
                                        _a.sent();
                                        resolve(true);
                                        return [2 /*return*/];
                                }
                            });
                        }); })];
                });
            }); });
            resolve(true);
            return [2 /*return*/];
        });
    }); }); });
}
// Access serialiazed testcases
function serializeAccess(sharedAccess) {
    var _this = this;
    describe("Access Shared", function () { return new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
        var _this = this;
        return __generator(this, function (_a) {
            it("Access Shared", function () { return __awaiter(_this, void 0, void 0, function () {
                var _this = this;
                return __generator(this, function (_a) {
                    return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                            var _this = this;
                            return __generator(this, function (_a) {
                                switch (_a.label) {
                                    case 0: return [4 /*yield*/, sharedAccess.serialize(sharedAccess.access).then(function (stringResult) { return __awaiter(_this, void 0, void 0, function () {
                                            return __generator(this, function (_a) {
                                                parsedAccess(stringResult);
                                                resolve(true);
                                                return [2 /*return*/];
                                            });
                                        }); })["catch"](function (err) {
                                            reject(err);
                                        })];
                                    case 1:
                                        _a.sent();
                                        resolve(true);
                                        return [2 /*return*/];
                                }
                            });
                        }); })];
                });
            }); });
            resolve(true);
            return [2 /*return*/];
        });
    }); }); });
}
// Access share testcases
function accessShare(access) {
    var _this = this;
    describe("Access Shared", function () { return new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
        var _this = this;
        return __generator(this, function (_a) {
            it("Access Shared", function () { return __awaiter(_this, void 0, void 0, function () {
                var _this = this;
                return __generator(this, function (_a) {
                    return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                            var permission, sharePrefix, sharePrefixListArray;
                            var _this = this;
                            return __generator(this, function (_a) {
                                switch (_a.label) {
                                    case 0:
                                        permission = new storj.Permission(true, true, true, true, 0, 0), sharePrefix = storj.SharePrefix, sharePrefixListArray = [];
                                        sharePrefix.bucket = storjConfig.bucketName;
                                        sharePrefix.prefix = "filepath/";
                                        sharePrefixListArray.push(sharePrefix);
                                        return [4 /*yield*/, access.share(permission, sharePrefixListArray, sharePrefixListArray.length).then(function (sharedAccess) { return __awaiter(_this, void 0, void 0, function () {
                                                return __generator(this, function (_a) {
                                                    describe("access share Result", function () {
                                                        it("should have share access", function () {
                                                            sharedAccess.access._handle.should.be.a("number");
                                                        });
                                                    });
                                                    describe("access share Result", function () {
                                                        it("should have access serialize function", function () {
                                                            sharedAccess.should.have.property("serialize");
                                                        });
                                                    });
                                                    describe("access share Result", function () {
                                                        it("should have open project function", function () {
                                                            sharedAccess.should.have.property("openProject");
                                                        });
                                                    });
                                                    describe("access share Result", function () {
                                                        it("should have config open project function", function () {
                                                            sharedAccess.should.have.property("configOpenProject");
                                                        });
                                                    });
                                                    describe("access share Result", function () {
                                                        it("should have access share function", function () {
                                                            sharedAccess.should.have.property("share");
                                                        });
                                                    });
                                                    //
                                                    serializeAccess(sharedAccess);
                                                    resolve(true);
                                                    return [2 /*return*/];
                                                });
                                            }); })["catch"](function (err) {
                                                reject(err);
                                            })];
                                    case 1:
                                        _a.sent();
                                        resolve(true);
                                        return [2 /*return*/];
                                }
                            });
                        }); })];
                });
            }); });
            resolve(true);
            return [2 /*return*/];
        });
    }); }); });
}
// Download read testcases
function downloadRead(downloadresult) {
    var _this = this;
    describe("Download Result Function", function () { return new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
        var _this = this;
        return __generator(this, function (_a) {
            it("Download", function () { return __awaiter(_this, void 0, void 0, function () {
                var _this = this;
                return __generator(this, function (_a) {
                    return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                            var buffer;
                            var _this = this;
                            return __generator(this, function (_a) {
                                switch (_a.label) {
                                    case 0:
                                        buffer = Buffer.alloc(BUFFER_SIZE);
                                        return [4 /*yield*/, downloadresult.read(buffer, buffer.length).then(function (bytesread) { return __awaiter(_this, void 0, void 0, function () {
                                                var _this = this;
                                                return __generator(this, function (_a) {
                                                    describe("Close Download", function () {
                                                        it("Close download function", function () { return __awaiter(_this, void 0, void 0, function () {
                                                            var _this = this;
                                                            return __generator(this, function (_a) {
                                                                return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                                                                        return __generator(this, function (_a) {
                                                                            switch (_a.label) {
                                                                                case 0: return [4 /*yield*/, downloadresult.close(downloadresult.download).then(function () {
                                                                                        //
                                                                                    })["catch"](function (err) {
                                                                                        reject(err);
                                                                                    })];
                                                                                case 1:
                                                                                    _a.sent();
                                                                                    resolve(true);
                                                                                    return [2 /*return*/];
                                                                            }
                                                                        });
                                                                    }); })];
                                                            });
                                                        }); });
                                                    });
                                                    //
                                                    resolve(true);
                                                    return [2 /*return*/];
                                                });
                                            }); })["catch"](function (err) {
                                                reject(err);
                                            })];
                                    case 1:
                                        _a.sent();
                                        resolve(true);
                                        return [2 /*return*/];
                                }
                            });
                        }); })];
                });
            }); });
            resolve(true);
            return [2 /*return*/];
        });
    }); }); });
}
// Download object testcases
function downloadObject(project) {
    var _this = this;
    describe("Download Object Function", function () { return new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
        var _this = this;
        return __generator(this, function (_a) {
            it("Download", function () { return __awaiter(_this, void 0, void 0, function () {
                var _this = this;
                return __generator(this, function (_a) {
                    return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                            var downloadOptions;
                            var _this = this;
                            return __generator(this, function (_a) {
                                switch (_a.label) {
                                    case 0:
                                        downloadOptions = new storj.DownloadOptions();
                                        downloadOptions.offset = 0;
                                        downloadOptions.length = -1;
                                        return [4 /*yield*/, project.downloadObject(storjConfig.bucketName, storjConfig.uploadPath, downloadOptions).then(function (downloadresult) { return __awaiter(_this, void 0, void 0, function () {
                                                var buffer;
                                                return __generator(this, function (_a) {
                                                    buffer = Buffer.alloc(BUFFER_SIZE);
                                                    describe("download object Result", function () {
                                                        it("Should get download handle", function () {
                                                            expect(downloadresult.download._handle).to.be.a("number");
                                                        });
                                                    });
                                                    describe("download object Result", function () {
                                                        it("Should have download read function ", function () {
                                                            downloadresult.should.have.property("read");
                                                        });
                                                    });
                                                    describe("download object Result", function () {
                                                        it("Should have download info function ", function () {
                                                            downloadresult.should.have.property("info");
                                                        });
                                                    });
                                                    describe("download object Result", function () {
                                                        it("Should have close download function ", function () {
                                                            downloadresult.should.have.property("close");
                                                        });
                                                    });
                                                    downloadRead(downloadresult);
                                                    //
                                                    deleteObject(project);
                                                    //
                                                    deleteBucket(project);
                                                    //
                                                    closeProject(project);
                                                    //
                                                    resolve(true);
                                                    return [2 /*return*/];
                                                });
                                            }); })["catch"](function (err) {
                                                reject(err);
                                            })];
                                    case 1:
                                        _a.sent();
                                        resolve(true);
                                        return [2 /*return*/];
                                }
                            });
                        }); })];
                });
            }); });
            resolve(true);
            return [2 /*return*/];
        });
    }); }); });
}
//
function listObject(project) {
    var _this = this;
    describe("list object function", function () { return new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
        var _this = this;
        return __generator(this, function (_a) {
            it("List Object function", function () { return __awaiter(_this, void 0, void 0, function () {
                var _this = this;
                return __generator(this, function (_a) {
                    return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                            var listObjectsOptions;
                            var _this = this;
                            return __generator(this, function (_a) {
                                switch (_a.label) {
                                    case 0:
                                        listObjectsOptions = new storj.ListObjectsOptions();
                                        return [4 /*yield*/, project.listObjects(storjConfig.bucketName, listObjectsOptions).then(function (objectlist) { return __awaiter(_this, void 0, void 0, function () {
                                                return __generator(this, function (_a) {
                                                    describe("list object Result", function () {
                                                        it("Should get list of object", function () {
                                                            objectlist.should.be.a("object");
                                                        });
                                                    });
                                                    resolve(true);
                                                    return [2 /*return*/];
                                                });
                                            }); })["catch"](function (err) {
                                                reject(err);
                                            })];
                                    case 1:
                                        _a.sent();
                                        resolve(true);
                                        return [2 /*return*/];
                                }
                            });
                        }); })];
                });
            }); });
            resolve(true);
            return [2 /*return*/];
        });
    }); }); });
}
//
function statObject(project) {
    var _this = this;
    describe("Download Object Function", function () { return new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
        var _this = this;
        return __generator(this, function (_a) {
            it("Download", function () { return __awaiter(_this, void 0, void 0, function () {
                var _this = this;
                return __generator(this, function (_a) {
                    return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                            return __generator(this, function (_a) {
                                switch (_a.label) {
                                    case 0: return [4 /*yield*/, project.statObject(storjConfig.bucketName, storjConfig.uploadPath).then(function (objectinfo) {
                                            //
                                            describe("stat object Result ", function () {
                                                it("Should get information of object", function () {
                                                    objectinfo.should.be.a("object");
                                                });
                                            });
                                            resolve(true);
                                        })["catch"](function (err) {
                                            reject(err);
                                        })];
                                    case 1:
                                        _a.sent();
                                        resolve(true);
                                        return [2 /*return*/];
                                }
                            });
                        }); })];
                });
            }); });
            resolve(true);
            return [2 /*return*/];
        });
    }); }); });
}
/*
 *
 * Upload write test cases
 */
function uploadWrite(upload, project, accessResult) {
    var _this = this;
    describe("Upload Write", function () { return new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
        var _this = this;
        return __generator(this, function (_a) {
            it("Upload Write", function () { return __awaiter(_this, void 0, void 0, function () {
                var _this = this;
                return __generator(this, function (_a) {
                    return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                            var buf, bytesRead, customMetadataEntry1, customMetadataEntry2, customMetadataEntryArray, customMetadata;
                            var _this = this;
                            return __generator(this, function (_a) {
                                switch (_a.label) {
                                    case 0:
                                        buf = Buffer.from(str, "utf-8"), bytesRead = buf.write(str, 0, buf.length, "utf-8");
                                        return [4 /*yield*/, upload.write(buf, bytesRead).then(function (upload) { return __awaiter(_this, void 0, void 0, function () {
                                                return __generator(this, function (_a) {
                                                    describe("upload write Result", function () {
                                                        it("Should have bytes written ", function () {
                                                            expect(upload.bytes_written).to.be.a("number");
                                                        });
                                                    });
                                                    resolve(true);
                                                    return [2 /*return*/];
                                                });
                                            }); })["catch"](function (err) {
                                                reject(err);
                                            })];
                                    case 1:
                                        _a.sent();
                                        customMetadataEntry1 = new storj.CustomMetadataEntry();
                                        customMetadataEntry1.key = "testing";
                                        customMetadataEntry1.key_length = customMetadataEntry1.key.length;
                                        customMetadataEntry1.value = "testing1";
                                        customMetadataEntry1.value_length = customMetadataEntry1.value.length;
                                        customMetadataEntry2 = new storj.CustomMetadataEntry();
                                        customMetadataEntry2.key = "value";
                                        customMetadataEntry2.key_length = customMetadataEntry2.key.length;
                                        customMetadataEntry2.value = "value1";
                                        customMetadataEntry2.value_length = customMetadataEntry2.value.length;
                                        customMetadataEntryArray = [
                                            customMetadataEntry1,
                                            customMetadataEntry2
                                        ], customMetadata = new storj.CustomMetadata();
                                        customMetadata.count = customMetadataEntryArray.length;
                                        customMetadata.entries = customMetadataEntryArray;
                                        //
                                        describe("upload set custom metadata", function () {
                                            it("Upload set custom metadata function", function () { return __awaiter(_this, void 0, void 0, function () {
                                                var _this = this;
                                                return __generator(this, function (_a) {
                                                    return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                                                            return __generator(this, function (_a) {
                                                                switch (_a.label) {
                                                                    case 0: return [4 /*yield*/, upload.setCustomMetadata(customMetadata).then(function () {
                                                                        })["catch"](function (err) {
                                                                            reject(err);
                                                                        })];
                                                                    case 1:
                                                                        _a.sent();
                                                                        resolve(true);
                                                                        return [2 /*return*/];
                                                                }
                                                            });
                                                        }); })];
                                                });
                                            }); });
                                        });
                                        //
                                        describe("upload commit", function () {
                                            it("Upload commit function", function () { return __awaiter(_this, void 0, void 0, function () {
                                                var _this = this;
                                                return __generator(this, function (_a) {
                                                    return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                                                            return __generator(this, function (_a) {
                                                                switch (_a.label) {
                                                                    case 0: return [4 /*yield*/, upload.commit().then(function () {
                                                                        })["catch"](function (err) {
                                                                            reject(err);
                                                                        })];
                                                                    case 1:
                                                                        _a.sent();
                                                                        resolve(true);
                                                                        return [2 /*return*/];
                                                                }
                                                            });
                                                        }); })];
                                                });
                                            }); });
                                        });
                                        //
                                        describe("upload info", function () { return __awaiter(_this, void 0, void 0, function () {
                                            var _this = this;
                                            return __generator(this, function (_a) {
                                                it("Upload info function", function () { return __awaiter(_this, void 0, void 0, function () {
                                                    var _this = this;
                                                    return __generator(this, function (_a) {
                                                        return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                                                                return __generator(this, function (_a) {
                                                                    switch (_a.label) {
                                                                        case 0: return [4 /*yield*/, upload.info().then(function (object) {
                                                                                it("should have object", function () {
                                                                                });
                                                                            })["catch"](function (err) {
                                                                                reject(err);
                                                                            })];
                                                                        case 1:
                                                                            _a.sent();
                                                                            resolve(true);
                                                                            return [2 /*return*/];
                                                                    }
                                                                });
                                                            }); })];
                                                    });
                                                }); });
                                                return [2 /*return*/];
                                            });
                                        }); });
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
                                        return [2 /*return*/];
                                }
                            });
                        }); })];
                });
            }); });
            resolve(true);
            return [2 /*return*/];
        });
    }); }); });
}
// Upload object test cases
function uploadObject(project, accessResult) {
    var _this = this;
    describe("Upload Object", function () { return new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
        var _this = this;
        return __generator(this, function (_a) {
            it("Upload Object", function () { return __awaiter(_this, void 0, void 0, function () {
                var _this = this;
                return __generator(this, function (_a) {
                    return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                            var uploadOptions;
                            var _this = this;
                            return __generator(this, function (_a) {
                                switch (_a.label) {
                                    case 0:
                                        uploadOptions = new storj.UploadOptions();
                                        uploadOptions.expires = 0;
                                        return [4 /*yield*/, project.uploadObject(storjConfig.bucketName, storjConfig.uploadPath, uploadOptions).then(function (upload) { return __awaiter(_this, void 0, void 0, function () {
                                                return __generator(this, function (_a) {
                                                    describe("upload Result", function () {
                                                        it("Should have upload", function () {
                                                            expect(upload.upload._handle).to.be.a("number");
                                                        });
                                                    });
                                                    //
                                                    describe("upload Result", function () {
                                                        it("Should have upload write function", function () {
                                                            upload.should.have.property("write");
                                                        });
                                                    });
                                                    //
                                                    describe("upload Result", function () {
                                                        it("Should have upload commit function", function () {
                                                            upload.should.have.property("commit");
                                                        });
                                                    });
                                                    //
                                                    describe("upload Result", function () {
                                                        it("Should have upload info function", function () {
                                                            upload.should.have.property("info");
                                                        });
                                                    });
                                                    //
                                                    describe("upload Result", function () {
                                                        it("Should have upload abort function", function () {
                                                            upload.should.have.property("abort");
                                                        });
                                                    });
                                                    //
                                                    describe("upload Result", function () {
                                                        it("Should have upload set custom metadata function", function () {
                                                            upload.should.have.property("setCustomMetadata");
                                                        });
                                                    });
                                                    //
                                                    uploadWrite(upload, project, accessResult);
                                                    //
                                                    resolve(true);
                                                    return [2 /*return*/];
                                                });
                                            }); })["catch"](function (err) {
                                                reject(err);
                                            })];
                                    case 1:
                                        _a.sent();
                                        resolve(true);
                                        return [2 /*return*/];
                                }
                            });
                        }); })];
                });
            }); });
            resolve(true);
            return [2 /*return*/];
        });
    }); }); });
}
// List bucket test cases
function listBucket(project) {
    var _this = this;
    describe("List Bucket", function () { return new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
        var _this = this;
        return __generator(this, function (_a) {
            it("Listing Bucket", function () { return __awaiter(_this, void 0, void 0, function () {
                var _this = this;
                return __generator(this, function (_a) {
                    return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                            var listBucketsOptions;
                            var _this = this;
                            return __generator(this, function (_a) {
                                switch (_a.label) {
                                    case 0:
                                        listBucketsOptions = new storj.ListBucketsOptions();
                                        return [4 /*yield*/, project.listBuckets(listBucketsOptions).then(function (bucketListResult) { return __awaiter(_this, void 0, void 0, function () {
                                                return __generator(this, function (_a) {
                                                    describe("list bucket Result", function () {
                                                        it("should have information", function () {
                                                            bucketListResult.should.be.a("object");
                                                        });
                                                    });
                                                    resolve(true);
                                                    return [2 /*return*/];
                                                });
                                            }); })["catch"](function (err) {
                                                reject(err);
                                            })];
                                    case 1:
                                        _a.sent();
                                        resolve(true);
                                        return [2 /*return*/];
                                }
                            });
                        }); })];
                });
            }); });
            resolve(true);
            return [2 /*return*/];
        });
    }); }); });
}
// Ensured Bucket testcases
function ensureBucket(project) {
    var _this = this;
    describe("Ensure Bucket", function () { return new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
        var _this = this;
        return __generator(this, function (_a) {
            it("Ensure Bucket", function () { return __awaiter(_this, void 0, void 0, function () {
                var _this = this;
                return __generator(this, function (_a) {
                    return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                            var _this = this;
                            return __generator(this, function (_a) {
                                switch (_a.label) {
                                    case 0: return [4 /*yield*/, project.ensureBucket(storjConfig.bucketName).then(function (bucketInfo) { return __awaiter(_this, void 0, void 0, function () {
                                            return __generator(this, function (_a) {
                                                describe("ensure bucket Result", function () {
                                                    it("should have information", function () {
                                                        bucketInfo.name.should.equal(storjConfig.bucketName);
                                                    });
                                                });
                                                resolve(true);
                                                return [2 /*return*/];
                                            });
                                        }); })["catch"](function (err) {
                                            reject(err);
                                        })];
                                    case 1:
                                        _a.sent();
                                        resolve(true);
                                        return [2 /*return*/];
                                }
                            });
                        }); })];
                });
            }); });
            resolve(true);
            return [2 /*return*/];
        });
    }); }); });
}
// Stat Bucket test cases
function statBucket(project) {
    var _this = this;
    describe("Stat Bucket", function () { return new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
        var _this = this;
        return __generator(this, function (_a) {
            it("Stat Bucket", function () { return __awaiter(_this, void 0, void 0, function () {
                var _this = this;
                return __generator(this, function (_a) {
                    return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                            var _this = this;
                            return __generator(this, function (_a) {
                                switch (_a.label) {
                                    case 0: return [4 /*yield*/, project.statBucket(storjConfig.bucketName).then(function (bucketInfo) { return __awaiter(_this, void 0, void 0, function () {
                                            return __generator(this, function (_a) {
                                                describe("stat bucket Result", function () {
                                                    it("should have information", function () {
                                                        bucketInfo.name.should.equal(storjConfig.bucketName);
                                                    });
                                                });
                                                resolve(true);
                                                return [2 /*return*/];
                                            });
                                        }); })["catch"](function (err) {
                                            reject(err);
                                        })];
                                    case 1:
                                        _a.sent();
                                        resolve(true);
                                        return [2 /*return*/];
                                }
                            });
                        }); })];
                });
            }); });
            resolve(true);
            return [2 /*return*/];
        });
    }); }); });
}
// Create bucket test cases
function createBucket(project, accessResult) {
    var _this = this;
    describe("Create Bucket", function () { return new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
        var _this = this;
        return __generator(this, function (_a) {
            it("Creating Bucket", function () { return __awaiter(_this, void 0, void 0, function () {
                var _this = this;
                return __generator(this, function (_a) {
                    return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                            var _this = this;
                            return __generator(this, function (_a) {
                                switch (_a.label) {
                                    case 0: return [4 /*yield*/, project.createBucket(storjConfig.bucketName).then(function (bucketInfo) { return __awaiter(_this, void 0, void 0, function () {
                                            return __generator(this, function (_a) {
                                                describe("create bucket Result", function () {
                                                    it("should have information", function () {
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
                                                uploadObject(project, accessResult);
                                                //
                                                resolve(true);
                                                return [2 /*return*/];
                                            });
                                        }); })["catch"](function (err) {
                                            reject(err);
                                        })];
                                    case 1:
                                        _a.sent();
                                        resolve(true);
                                        return [2 /*return*/];
                                }
                            });
                        }); })];
                });
            }); });
            resolve(true);
            return [2 /*return*/];
        });
    }); }); });
}
// Open project test case
function openProject(accessResult) {
    var _this = this;
    describe("Open Project Function", function () { return new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
        var _this = this;
        return __generator(this, function (_a) {
            it("Opening project", function () { return __awaiter(_this, void 0, void 0, function () {
                var _this = this;
                return __generator(this, function (_a) {
                    return [2 /*return*/, new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                            var _this = this;
                            return __generator(this, function (_a) {
                                switch (_a.label) {
                                    case 0: return [4 /*yield*/, accessResult.openProject().then(function (project) { return __awaiter(_this, void 0, void 0, function () {
                                            return __generator(this, function (_a) {
                                                describe("Project Result", function () {
                                                    it("Should have project", function () {
                                                        expect(project.project._handle).to.be.a("number");
                                                    });
                                                });
                                                //
                                                describe("Project Result", function () {
                                                    it("Should have stat bucket function", function () {
                                                        project.should.have.property("statBucket");
                                                    });
                                                });
                                                //
                                                describe("Project Result", function () {
                                                    it("Should have create bucket function", function () {
                                                        project.should.have.property("createBucket");
                                                    });
                                                });
                                                //
                                                describe("Project Result", function () {
                                                    it("Should have ensure bucket function", function () {
                                                        project.should.have.property("ensureBucket");
                                                    });
                                                });
                                                //
                                                describe("Project Result", function () {
                                                    it("Should have list bucket function", function () {
                                                        project.should.have.property("listBuckets");
                                                    });
                                                });
                                                //
                                                describe("Project Result", function () {
                                                    it("Should have download object function", function () {
                                                        project.should.have.property("downloadObject");
                                                    });
                                                });
                                                //
                                                describe("Project Result", function () {
                                                    it("Should have upload object function", function () {
                                                        project.should.have.property("uploadObject");
                                                    });
                                                });
                                                //
                                                describe("Project Result", function () {
                                                    it("Should have stat object function", function () {
                                                        project.should.have.property("statObject");
                                                    });
                                                });
                                                //
                                                describe("Project Result", function () {
                                                    it("Should have list object function", function () {
                                                        project.should.have.property("listObjects");
                                                    });
                                                });
                                                //
                                                describe("Project Result", function () {
                                                    it("Should have delete object function", function () {
                                                        project.should.have.property("deleteObject");
                                                    });
                                                });
                                                //
                                                describe("Project Result", function () {
                                                    it("Should have delete bucket function", function () {
                                                        project.should.have.property("deleteBucket");
                                                    });
                                                });
                                                //
                                                describe("Project Result", function () {
                                                    it("Should have close project function", function () {
                                                        project.should.have.property("close");
                                                    });
                                                });
                                                //
                                                createBucket(project, accessResult);
                                                //
                                                resolve(true);
                                                return [2 /*return*/];
                                            });
                                        }); })["catch"](function (err) {
                                            reject(err);
                                        })];
                                    case 1:
                                        _a.sent();
                                        resolve(true);
                                        return [2 /*return*/];
                                }
                            });
                        }); })];
                });
            }); });
            resolve(true);
            return [2 /*return*/];
        });
    }); }); });
}
/*
 *
 *
 */
describe("Uplink NodeJS Test", function () {
    //
    context("Request Access with phassphrase", function () {
        it("Request Access", function () {
            return new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                var _this = this;
                return __generator(this, function (_a) {
                    switch (_a.label) {
                        case 0: 
                        //
                        return [4 /*yield*/, libUplink.requestAccessWithPassphrase(storjConfig.satelliteURL, storjConfig.apiKey, storjConfig.encryptionPassphrase).then(function (access) { return __awaiter(_this, void 0, void 0, function () {
                                return __generator(this, function (_a) {
                                    //
                                    describe("Access Result", function () {
                                        it("Should have access", function () {
                                            expect(access.access._handle).to.be.a("number");
                                        });
                                    });
                                    describe("Access Result", function () {
                                        it("Should have open project function", function () {
                                            access.should.have.property("openProject");
                                        });
                                    });
                                    describe("Access Result", function () {
                                        it("Should have config open project function", function () {
                                            access.should.have.property("configOpenProject");
                                        });
                                    });
                                    describe("Access Result", function () {
                                        it("Should have access share function", function () {
                                            access.should.have.property("share");
                                        });
                                    });
                                    describe("Access Result", function () {
                                        it("Should have access serialize function", function () {
                                            access.should.have.property("serialize");
                                        });
                                    });
                                    //
                                    openProject(access);
                                    //
                                    resolve(true);
                                    return [2 /*return*/];
                                });
                            }); })];
                        case 1:
                            //
                            _a.sent();
                            return [2 /*return*/];
                    }
                });
            }); })["catch"](function (err) {
                console.log(err);
            });
            //
        });
    });
});
