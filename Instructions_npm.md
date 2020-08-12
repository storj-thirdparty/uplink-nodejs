# <b>uplink-nodejs binding</b>

## <b>Instructions to push package on npm</b>

Follow these instructions to make changes to the project and push ```uplink-nodejs``` package to npm, This would make the latest package install using the ```npm install``` command.

Assuming you have git cloned the project to your local directory and have already made the required changes:

* In Command Prompt, navigate to the ```your-local-directory/uplink-nodejs``` folder, here ```uplink-nodejs``` is the root directory of cloned project.

Your directory structure would be something like this:

    uplink-nodejs
    └── CHANGELOG.md
    └── cloudbuild.yaml
    └── Dockerfile
    └── docs
        └── index.html
    └── Instructions_npm.md
    └── LICENSE
    └── README.assets
    └── README.md
    └── functions/
    └── functions_win/
    └── libUplink.cc
    └── libUplink.h
    └── libUplink_win.cc
    └── libUplink_win.h
    └── test
        └── testcases.js
    └── access.js
    └── error.js
    └── download.js
    └── project.js
    └── upload.js
    └── uplink.js
    └── package.json
    └── package-lock.json

* Open ```package.json``` using any text editor and increment the package ```version```:

        {
            "name": "uplink-nodejs",
            "version": "1.0.6",

> Increment package version is mandatory because npm does not allow updating a package with same version.

* Run following command for changing version of package-lock.json file
        
        $ npm install

Make sure version of package-lock.json file is same as package.json file

* Delete file or folder created after running ```npm install``` command :
        

        List of files or folders that will be created after running npm install command
        uplink-nodejs
        └── node_modules/
        └── uplink_definations.h
        └── libuplinkc.h
        └── libuplinkc.dll,libuplinkc.so,libuplinkc.dylib
        └── build/

* Login on npm using following command :

        $ npm login
        You will be prompted for a username ,password and public email, enter username, password and public email used on ```https://www.npmjs.com/```

* Push on npm :

        $ npm publish

> For more details and complete tutorial on how to publish a package on npm, goto [Publish a package](https://docs.npmjs.com/cli/publish)