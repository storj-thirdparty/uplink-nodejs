{
  'conditions':[
    ['OS=="linux"',{
      'targets':[
        {
          "target_name": "storjnodejs",
          "sources":["./libUplink.cc"],
          "libraries":["<(module_root_dir)/libuplinkc.so"],
          "ldflags": ["-Wl,-rpath,'$$ORIGIN'"],
          "cflags_cc": ["-fexceptions","-fPIC","-Wno-unknown-pragmas"]
        }
      ]
    }],
    ['OS=="win"',{
      'targets':[
        {
          "target_name": "storjnodejs",
          "sources":["libUplink_win.cc"],
          "copies":[{
            "destination":"<(module_root_dir)/build/Release",
            "files":["<(module_root_dir)/libuplinkc.dll","<(module_root_dir)/libuplinkc.h","<(module_root_dir)/uplink_definitions.h"]
          }],
          "ldflags":["-Wl,-rpath","build/Release"],  
        },
      ]
    }],
    ['OS=="mac"',{
        'targets':[
          {
            "target_name": "storjnodejs",
            "sources":["./libUplink.cc"],
            'include_dirs': ['<(module_root_dir)'],
            "copies":[{
            "destination":"<(module_root_dir)/build/Release",
            "files":["<(module_root_dir)/libuplinkc.dylib","<(module_root_dir)/libuplinkc.h","<(module_root_dir)/uplink_definitions.h"]
            }],
            "link_settings": {
              "libraries": ["-L/<(module_root_dir)/build/Release","libuplinkc.dylib"]
            },
            "ldflags":["-Wl,-rpath","build/Release"],
            "cxxflags":["-fPIC"],
            "cflags_cc": ["-fexceptions","-fPIC","-Wno-unknown-pragmas"]
          }
        ]
    }]
  ]
}