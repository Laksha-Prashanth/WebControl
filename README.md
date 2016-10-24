# WebControl
Control you computer through the web

Setup
Change the url in the javascript file to your firebase project. Do the same for the daemon before compiling also. You can also use the current firebase url, its open.

The firebase project should have the following rules:

testData/unread must be indexed

testData/read must be indexed


"testData":{
        "unread":{
          ".indexOn":["i"]
        },
          "read":{
            ".indexOn":["i"]
          }
}


1. Run the daemon on your computer.
2. Go to the website and enter any command.
3. Firebase gets updated.
4. The program should be executed on your machine.
