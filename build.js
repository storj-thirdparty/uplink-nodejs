var exec = require('child_process').exec;
var os = require('os');
const configuration = require("./config.json");

var OSType = os.type();
if(OSType.includes('Linux')){
    var GOPATH = process.env.GOPATH;
    var STORJPACKAGEPATH = configuration.STORJPACKAGEPATH;
    var STORJPATH = configuration.STORJPATH;
    var storjPath;
    if(GOPATH!=undefined){
    storjPath = GOPATH+STORJPATH+STORJPACKAGEPATH;
    }else{
    var HOMEPATH = process.env.HOME;
    var STORJPATHHOME = configuration.STORJPATHHOME;
    storjPath = HOMEPATH+STORJPATHHOME+STORJPACKAGEPATH;
    }
    exec("((cd "+storjPath+" && go build -o libuplinkc.so -buildmode=c-shared ) && cp "+storjPath+"/libuplinkc.* ./ && cp "+storjPath+"/uplink_definitions.h ./ && node-gyp configure && node-gyp rebuild)",(error,stdout,stderr)=>{
        if(error){
           console.log("Error : ",error);
           return;
        }
        if(stdout){
           console.log("Std Error : ",stdout);
        }
        console.log("Shared library created ");
    });

}else if(OSType.includes('Darwin')){
    var GOPATH = process.env.GOPATH;
    var STORJPACKAGEPATH = configuration.STORJPACKAGEPATH;
    var STORJPATH = configuration.STORJPATH;
    var storjPath;
    if(GOPATH!=undefined){
        storjPath = GOPATH+STORJPATH+STORJPACKAGEPATH;
    }else{
        var HOMEPATH = process.env.HOME;
        var STORJPATHHOME = configuration.STORJPATHHOME;
        storjPath = HOMEPATH+STORJPATHHOME+STORJPACKAGEPATH;
    }
    
 exec("((cd "+storjPath+" && go build -o libuplinkc.so -buildmode=c-shared ) && cp "+storjPath+"/libuplinkc.* ./ && cp "+storjPath+"/uplink_definitions.h ./ )",(error,stdout,stderr)=>{
    if(error){
       console.log("Error : ",error);
       return;
    }
});
}else if(OSType.includes('Windows')){
 var GOPATH = process.env.GOPATH;
 var STORJPACKAGEPATH = configuration.STORJPACKAGEPATH;
 var STORJPATH = configuration.STORJPATH;
 GOPATH=GOPATH.replace(/\\/g, '/');
 var storjPath;
if(GOPATH!=undefined){
 storjPath = GOPATH+STORJPATH+STORJPACKAGEPATH;
}else{
 var UserProfile = process.env.USERPROFILE;
 var STORJPATHHOME = configuration.STORJPATHHOME;
 UserProfile= UserProfile.replace(/\\/g, '/');
 storjPath = UserProfile+STORJPATHHOME+STORJPACKAGEPATH;
}
var changedirectory;
    if(storjPath.includes(':')){
        changedirectory = storjPath.split(":")[0];
    }else{
        changedirectory="C";
    }
 exec('('+changedirectory+': && cd '+storjPath+' && go build -o libuplinkc.dll -buildmode=c-shared && copy "libuplinkc.*" "'+__dirname+'" && copy "uplink_definitions.h" "'+__dirname+'") && cd "'+__dirname+'"',(error,stdout,stderr)=>{
     if(error){
        console.log("Error : ",error);
        return;
     }
 });
}else{
    var GOPATH = process.env.GOPATH;
    var STORJPACKAGEPATH = configuration.STORJPACKAGEPATH;
    var STORJPATH = configuration.STORJPATH;
    var storjPath;
    if(GOPATH!=undefined){
    storjPath = GOPATH+STORJPATH+STORJPACKAGEPATH;
    }else{
    var HOMEPATH = process.env.HOME;
    var STORJPATHHOME = configuration.STORJPATHHOME;
    storjPath = HOMEPATH+STORJPATHHOME+STORJPACKAGEPATH;
    }
    exec("((cd "+storjPath+" && go build -o libuplinkc.so -buildmode=c-shared ) && cp "+storjPath+"/libuplinkc.* ./ && cp "+storjPath+"/uplink_definitions.h ./ && node-gyp configure && node-gyp rebuild)",(error,stdout,stderr)=>{
        if(error){
           console.log("Error : ",error);
           return;
        }
    });
}
