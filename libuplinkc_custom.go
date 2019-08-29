// Copyright (C) 2019 Storj Labs, Inc.
// See LICENSE for copying information.

package main

// #include "uplink_definitions.h"
import "C"

import (
	"context"
	"bytes"
	"fmt"
	"io/ioutil"
	"storj.io/storj/lib/uplink"

)

// open_bucket opens a given valid bucket at Storj (V3) project
// it generates access using salted key, which is created from given encryption pass-phrase
//export open_bucket_custom
func open_bucket_custom(projectHandle C.ProjectRef, name *C.char, encryptionAccessRef C.EncryptionAccessRef, cerr **C.char) C.BucketRef {
	project, ok := universe.Get(projectHandle._handle).(*Project)
	if !ok {
		*cerr = C.CString("invalid project")
		return C.BucketRef{}
	}

	scope := project.scope.child()
	//
	access, ok := universe.Get(encryptionAccessRef._handle).(*uplink.EncryptionAccess)
	if !ok{
		*cerr = C.CString("invalid access")
		return C.BucketRef{}
	}
	//
	bucket, err := project.OpenBucket(scope.ctx, C.GoString(name), access)
	if err != nil {
		*cerr = C.CString(err.Error())
		return C.BucketRef{}
	}

	return C.BucketRef{universe.Add(&Bucket{scope, bucket})}
}


// delete_object_custom removes a object(s) from a given Storj (V3) bucket path
//export delete_object_custom
func delete_object_custom(bucketRef C.BucketRef, path *C.char, cerr **C.char) {
	bucket, ok := universe.Get(bucketRef._handle).(*Bucket)
	if !ok {
		*cerr = C.CString("invalid bucket")
		return
	}

	scope := bucket.scope.child()

	if err := bucket.DeleteObject(scope.ctx, C.GoString(path)); err != nil {
		*cerr = C.CString(fmt.Sprintf("%+v", err))
		return
	}
}

// upload_custom uploads a new object from srcFullFileName (at local compuet) to Storj (V3) bucket's path, if authorized.
//export upload_custom
func upload_custom(cBucket C.BucketRef, path *C.char, srcFullFileName *C.char, cErr **C.char) {
	bucket, ok := universe.Get(cBucket._handle).(*Bucket)
	if !ok {
		*cErr = C.CString("invalid bucket")
		return 
	}

	scope := bucket.scope.child()
	var err error 
	var content = []byte{}
	if C.GoString(srcFullFileName) != ""{
		content, err = ioutil.ReadFile(C.GoString(srcFullFileName))
		if err != nil {
			*cErr = C.CString(err.Error())
			return
		}
	}
	//
	buf := bytes.NewBuffer(content)

	var opts uplink.UploadOptions

	err = bucket.UploadObject(scope.ctx, C.GoString(path), buf, &opts)
	if err != nil {
		// retry once more
		err = bucket.UploadObject(scope.ctx, C.GoString(path), buf, &opts)
		if err != nil {
			*cErr = C.CString(err.Error())
			return 
		}
	}
}

// download fetches Storj (V3) object's data and stores it into given file with destFullFileName name on local computer
//export download_custom
func download_custom(cBucket C.BucketRef, storjPath *C.char, destFullFileName *C.char, cErr **C.char) {
	bucket, ok := universe.Get(cBucket._handle).(*Bucket)
	if !ok {
		*cErr = C.CString("invalid bucket")
		return 
	}

	scope := bucket.scope.child()

	// Open the object from given bucket's storjPath
	readBack, err := bucket.OpenObject(scope.ctx, C.GoString(storjPath))
	if err != nil {
		// could NOT open object from given bucket's storjPath
		*cErr = C.CString(err.Error())
		return
	}
	defer readBack.Close()

	// Initiate the process to download the whole thing
	strm, err := readBack.DownloadRange(scope.ctx, 0, -1)
	if err != nil {
		// could not initiate download
		*cErr = C.CString(err.Error())
		return
	}
	defer strm.Close()

	// Read everything from the stream
	receivedContents, err := ioutil.ReadAll(strm)
	if err != nil {
		// could not read object
		*cErr = C.CString(err.Error())
		return
	}
	
	// Write the content to the desired file
	err = ioutil.WriteFile(C.GoString(destFullFileName), receivedContents, 0644)
	if err != nil {
		// could not write object's content to desired file
		*cErr = C.CString(err.Error())
		return
	}
}

// generate encryption key access to project's buckets
//export encryption_key_custom
func encryption_key_custom(projectHandle C.ProjectRef, encryptionAccess *C.char, cerr **C.char) C.EncryptionAccessRef {
	
	project, ok := universe.Get(projectHandle._handle).(*Project)
	if !ok {
		*cerr = C.CString("invalid project")
		return C.EncryptionAccessRef{}
	}
	//
	scope := project.scope.child()
	//
	encryptionKey, err := project.SaltedKeyFromPassphrase(scope.ctx, C.GoString(encryptionAccess))
    if err != nil {
       *cerr = C.CString(err.Error())
       return C.EncryptionAccessRef{}
    }
    //
    access := uplink.NewEncryptionAccessWithDefaultKey(*encryptionKey)
    // 
    return C.EncryptionAccessRef{_handle: universe.Add(access)}
}

// uplink_custom creates a new uplink with given partner ID
//export uplink_custom
func uplink_custom(cfg C.UplinkConfig, cerr **C.char) C.UplinkRef {
	scope := rootScope("")
	//
	libcfg := &uplink.Config{}
	//
	libcfg.Volatile.TLS.SkipPeerCAWhitelist = cfg.Volatile.tls.skip_peer_ca_whitelist == C.bool(true)
	//
	libcfg.Volatile.PartnerID = "a1ba07a4-e095-4a43-914c-1d56c9ff5afd"
	//
	lib, err := uplink.NewUplink(scope.ctx, libcfg)
	if err != nil {
		*cerr = C.CString(fmt.Sprintf("%+v", err))
		return C.UplinkRef{}
	}

	return C.UplinkRef{universe.Add(&Uplink{scope, lib})}
}

//export createbucket_custom
func createbucket_custom(uplinkHandle C.UplinkRef,satelliteAddress *C.char,apiKey *C.char,name *C.char,cerr **C.char) C.BucketInfo {
	var cfg uplink.Config
    ctx := context.Background()
    cfg.Volatile.TLS.SkipPeerCAWhitelist = true
    uplinkstorj, err := uplink.NewUplink(ctx, &cfg)
    if err != nil {
        *cerr = C.CString("invalid uplink")
		return C.BucketInfo{}
    }
    defer uplinkstorj.Close()

	// uplink, ok := universe.Get(uplinkHandle._handle).(*Uplink)
	// if !ok {
	// 	*cerr = C.CString("invalid uplink")
	// 	return C.BucketInfo{}
	// }

	apiKeyparse, err := uplink.ParseAPIKey(C.GoString(apiKey))

	project, err := uplinkstorj.OpenProject(ctx, C.GoString(satelliteAddress), apiKeyparse)
    if err != nil {
        return C.BucketInfo{}
    }

	bucket, err := project.CreateBucket(ctx, C.GoString(name), nil)
	if err != nil {
		*cerr = C.CString(fmt.Sprintf("%+v", err))
		return C.BucketInfo{}
	}

	return newBucketInfo(&bucket)

}
