/*
 * libuplinkc package provides function to connect to a Storj network,
 * based on the configuration, passed through parameters.
 * It then stores given data at desired Storj V3 bucket.
 *
 */

// Copyright (C) 2019 Storj Labs, Inc.
// See LICENSE for copying information.
 
package main

import "C"

import (
 	// standard packages
	"bytes"
	"context"

	"fmt"
	
	// external package
	"storj.io/storj/lib/uplink"
)

// DEBUG allows more detailed working to be exposed through the terminal
var DEBUG = false

//export ConnectStorjUploadData
// It connects to a Storj network, as per the given configuration parameters
// and then uploads given data to desired upload path within a bucket.
func ConnectStorjUploadData(Satellite string, APIKey string, Bucket string, UploadPath string, EncryptionPassphrase string, Data string) {
	// Satellite refers to a Storj V3 network's satellite URL
	// APIKey corresponds to a specific Storj project
	// Bucket is the name of desired Storj bucket, where data is to be stored
	// UploadPath is an optional path within the bucket for data storage
	// EncryptionPassphrase is for secure storage of data in Storj bucket
	// Data contains data that needs to be uploaded as object into storj V3 network

	if DEBUG {
	    fmt.Println("\nsatellite : ", Satellite)
	    fmt.Println("API : ", APIKey)
	    fmt.Println("bucket :", Bucket)
	    fmt.Println("uploadpath :", UploadPath)  
	    fmt.Println("Encryption :", EncryptionPassphrase) 
	    fmt.Println("datatoupload :", Data)
	}
   								 				  
    dataToUpload := []byte(Data)

	fmt.Println("\nCreating New Uplink...")

	var cfg uplink.Config
	//
	cfg.Volatile.PartnerID = "a1ba07a4-e095-4a43-914c-1d56c9ff5afd"
	//
	if DEBUG {
		fmt.Println("PartnerID in config variable :", cfg.Volatile.PartnerID)
	}
	
	ctx := context.Background()
	//
	uplinkstorj, err := uplink.NewUplink(ctx, &cfg)
	//
	if err != nil {
		fmt.Errorf("FAILed to create new Uplink object: %s\n", err)
		//
		return
	}
	//
	// defer uplinkstorj.Close()


	fmt.Println("\nParsing the API key...")

	key, err := uplink.ParseAPIKey(APIKey)
	//
	if err != nil {
		uplinkstorj.Close()
		//
		fmt.Errorf("FAILed to parse API key: %s\n", err)
		//
		return
	}
	//
	if DEBUG {
		fmt.Println("API key \t   :", key)
		fmt.Println("Serialized API key :", key.Serialize())
	}


	fmt.Println("\nOpening Project...")

	proj, err := uplinkstorj.OpenProject(ctx, Satellite, key)
	//
	if err != nil {
		uplinkstorj.Close()
		//
		fmt.Errorf("FAILed to open project: %s\n", err)
		//
		return
	}
	//
	// defer proj.Close()


	// Create an encryption key from encryption passphrase.
	fmt.Println("\nFetching encryption key from the pass phrase...")

	encryptionKey, err := proj.SaltedKeyFromPassphrase(ctx, EncryptionPassphrase)
	//
	if err != nil {
		proj.Close()
		uplinkstorj.Close()
		//
		fmt.Errorf("FAILed to create encryption key: %s\n", err)
		//
		return
	}
	//
	// Create an encryption context.
	access := uplink.NewEncryptionAccessWithDefaultKey(*encryptionKey)
	//
	fmt.Println("Created encryption access:", *access)


	// Open the desired Bucket within the project.
	fmt.Println("\nOpening Bucket...", Bucket)

	bucket, err := proj.OpenBucket(ctx, Bucket, access)
	//
	if err != nil {
		proj.Close()
		uplinkstorj.Close()
		//
		fmt.Errorf("FAILed to open bucket %q: %s\n", Bucket, err)
		//
		return
	}
	//
	// defer bucket.Close()


	if DEBUG {
		fmt.Println("\nPutting data into a buffer...")
	}
	//
	buf := bytes.NewBuffer(dataToUpload)


	fmt.Println("\nUploading data to the Storj bucket's ", UploadPath, " path: Initiated...")
	
	// Upload data to Storj.
	err = bucket.UploadObject(ctx, UploadPath, buf, nil)
	//
	if err != nil {
		if DEBUG {
			fmt.Println("FAILed to upload data to Storj V3: ", err)
			fmt.Println("Retrying to upload data...")
		}
		//
		err = bucket.UploadObject(ctx, UploadPath, buf, nil)
		//
		if err != nil {
			fmt.Errorf("FAILed to upload data to Storj V3 even on second attempt: %s\n", err)
		}
	}
	//
	if err == nil {
		fmt.Println("Uploading of data to the Storj bucket's ", UploadPath, " path: Completed!")
		//
		bucket.Close()
	}
	//
	proj.Close()
	uplinkstorj.Close()
}

func main() {
}
