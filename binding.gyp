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
          "sources":["libUplink_win.cc"]
        },
      ]
    }],
    ['OS=="mac"',{
      'targets':[
        {
          "target_name": "storjnodejs",
          "sources":["./libUplink.cc"],
          "libraries":["<(module_root_dir)/libuplinkc.so"],
          "ldflags": ["-Wl,-rpath,'$$ORIGIN'"],
          "cflags_cc": ["-fexceptions","-fPIC","-Wno-unknown-pragmas"]
        }
      ]
    }]
  ]
}