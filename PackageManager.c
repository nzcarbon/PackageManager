#include <gracelib.h>
#include <stdlib.h>
#ifndef __CYGWIN__
#pragma weak main
#endif
static char compilerRevision[] = "3a6ea8eb76fb201b08031f8f3a38a23906bf388e";
static Object undefined;
extern Object done;
extern Object _prelude;
extern Object String;
extern Object Number;
extern Object Boolean;
extern Object Dynamic;
extern Object List;
extern Object Block;
extern Object Done;
extern Object Type;
extern Object GraceDefaultObject;
extern Object sourceObject;
static Object type_String;
static Object type_Number;
static Object type_Boolean;
static Object type_Block;
static Object type_Done;
static Object argv;
static Object emptyclosure;
static Object prelude;
static int isTailObject = 0;
static Object inheritingObject = NULL;
static const char modulename[] = "PackageManager";
Object module_StandardPrelude_init();
static char *originalSourceLines[] = {
  "import \"sys\" as sys",
  "import \"io\" as io",
  "import \"curl\" as curl",
  "",
  "var imported",
  "var toProcess := []",
  "var verbose := false",
  "var global := false",
  "var bundlePath := \"\";",
  "var curFile;",
  "",
  "parseArgs(sys.argv);",
  "",
  "method parseArgs(args : List<String>) {",
  "    var count := 0;",
  "    print(args.size);",
  "    forArgs(args) do { arg, on ->",
  "        print(\"in loop\");",
  "        on.option \"get\" do { toGet->",
  "            doGet(toGet);",
  "        }",
  "        on.flag \"list\" do { ",
  "            listInstalled();",
  "        }",
  "        on.doubleValue \"bundle\" do { toBundle, name ->",
  "            bundle(toBundle,name);",
  "        }",
  "        on.doubleValue \"setAddress\" do { address, prefix ->",
  "            setImportDest(address,prefix);",
  "        }",
  "        on.flag \"verbose\" do { ",
  "            verbose := true;",
  "        }",
  "        on.flag \"global\" do {",
  "            global := true;",
  "        }",
  "    }",
  "} ",
  "",
  "method forArgs(args : List<String>) do(block) is confidential {",
  "    var i := 2",
  "    var ran := false",
  "    def size = args.size",
  "",
  "    def on = object {",
  "        method option(name : String) shortHand(sh : String) do(block') {",
  "            def arg = args.at(i)",
  "            if((arg == \"--{name}\") || (arg == \"-{sh}\")) then {",
  "                if(args.size == i) then {",
  "                    Exception.raise \"Missing value for option {name}\"",
  "                }",
  "",
  "                i := i + 1",
  "                block'.apply(args.at(i))",
  "                ran := true",
  "            }",
  "        }",
  "",
  "        method doubleValue(name: String) do (block'){",
  "            def arg = args.at(i)",
  "            if((arg == \"--{name}\")) then {",
  "                if(args.size < (i+1)) then {",
  "                    Exception.raise \"Missing values for option {name}\"",
  "                }",
  "                var arga := args.at(i+1)",
  "                var argb := args.at(i+2)",
  "                block'.apply(arga,argb)",
  "                i := i + 2",
  "                ran := true",
  "            }",
  "",
  "        }",
  "",
  "        method option(name : String) do(block') {",
  "            option(name) shortHand(\"\") do(block')",
  "        }",
  "",
  "        method flag(name : String) shortHand(sh : String) do(block') {",
  "            def arg = args.at(i)",
  "            if((arg == \"--{name}\") || (arg == \"-{sh}\")) then {",
  "                block'.apply",
  "                ran := true",
  "            }",
  "        }",
  "",
  "        method flag(name : String) do(block') {",
  "            flag(name) shortHand(\"\") do(block')",
  "        }",
  "    }",
  "    while { i <= size } do {",
  "        def arg = args.at(i)",
  "        print(arg);",
  "        ran := false",
  "        block.apply(arg, on)",
  "        if((arg.at(1) == \"-\") && ran.not) then {",
  "            Exception.raise(\"Unrecognised argument {arg}\")",
  "        }",
  "",
  "        i := i + 1",
  "    }",
  "}",
  "",
  "method listInstalled{ ",
  "    print(\"Installed packages:\")",
  "    if (io.exists(\"usr/lib/grace/packages/\"))then{",
  "        print(\"Going into the first call\");",
  "        var globalDir := \"usr/lib/grace/packages\"",
  "        recurseDirectory(globalDir,\"\")",
  "    }",
  "    if (io.exists(sys.environ[\"HOME\"]++\"/grace/packages/\"))then{",
  "        var usrDir := sys.environ[\"HOME\"]++\"/grace/packages\"//++ sys.environ[\"HOME\"]++\"/.grace/packages\")",
  "        recurseDirectory(usrDir,\" \")",
  "    }",
  "}",
  "",
  "method recurseDirectory(path,padding){",
  "    var pathContents := io.getdir(path);",
  "    for (pathContents) do { p-> ",
  "        if ((p != \"..\") && (p != \".\"))then{",
  "            print(padding++p);",
  "            if(io.getdir(path++\"/\"++p).size > 0)then{",
  "                recurseDirectory((path++\"/\"++p),padding++\"   \")",
  "            }",
  "        }",
  "    }",
  "}",
  "",
  "method doGet(impAddress){",
  "    imported := []",
  "    fetchImports(impAddress)",
  "    for(imported)do { im->",
  "        write(im);",
  "    }",
  "    for(imported)do { im->",
  "        compile(im);",
  "    }",
  "}",
  "",
  "method setFile(fileAddress){",
  "    if (fileAddress.substringFrom(fileAddress.size-5)to(fileAddress.size) == \".grace\")then{",
  "        var file := object{",
  "            var address is public := fileAddress",
  "            var data is public",
  "        }",
  "        curFile := file;",
  "        return true;",
  "    }",
  "    return false;",
  "}",
  "",
  "method fetchImports(fileAddress) -> Boolean{",
  "    if (setFile(fileAddress))then{",
  "        print(\"File address {curFile.address}\")",
  "        if (setFileData(curFile) == false)then{",
  "            print(\"Could not retrieve file data\")",
  "            return false",
  "        }",
  "        if (validateFile(curFile))then{",
  "            print(\"Pushing {curFile.address} to imported\")",
  "            imported.push(curFile)",
  "            parseFile(curFile)",
  "            while{toProcess.size > 0}do{ ",
  "                fetchImports(toProcess.pop)",
  "            }",
  "            return true",
  "        }",
  "    }",
  "    else{ ",
  "        if (fetchImports(fileAddress++\".grace\")==false)then{",
  "            print(\"Could not locate file. Check file address.\");",
  "            return false",
  "        }",
  "    }",
  "}",
  "",
  "method setFileData(file) -> Boolean {",
  "    if (file.address.substringFrom(1)to(4) == \"http\")then{",
  "        var strippedUrl := file.address.substringFrom(1)to(4);",
  "        if (findExisting(file.address) != false)then{",
  "            var findData := findExisting(strippedUrl)",
  "            if (findData > 0)then{",
  "                file.data := findData",
  "                return true;",
  "            }",
  "        }",
  "        var req := curl.easy",
  "        req.url := file.address",
  "        print(\"Searching for import: \"++file.address)",
  "        print(\" Connecting...\")",
  "        req.onReceive {d->",
  "            print(\" Response received\")   ",
  "            file.data := d.decode(\"utf-8\")",
  "            return true;",
  "        }",
  "        req.perform",
  "        if(req.responseCode != 200)then{",
  "            print(\" Could not locate import: Error {req.responseCode} for import {file.address}\");",
  "            return false",
  "        }",
  "    }",
  "    if (findExisting(file.address) != false)then{",
  "        print(\"Now searching in find existing\");",
  "        var findData := findExisting(file.address)",
  "        if (findData != false)then{",
  "            file.data := findData",
  "            return true;",
  "        }",
  "    }",
  "    return false;",
  "}",
  "",
  "method findExisting(fileName){",
  "    if(io.exists(fileName))then{",
  "        var open := io.open(fileName,\"r\");",
  "        var ret := open.read;",
  "        return ret; ",
  "    }   ",
  "    if(io.exists(\"/usr/lib/grace/modules/\"++fileName))then{",
  "        return io.open(\"/usr/lib/grace/modules/\"++fileName,\"r\").read ",
  "    }   ",
  "    var homePath := sys.environ[\"HOME\"]",
  "    if(io.exists(\"{homePath}/.local/share/grace/modules/\"++fileName))then{",
  "        return io.open(\"{homePath}/.local/share/grace/modules/\"++fileName,\"r\").read ",
  "    }   ",
  "    if(io.exists(\"{sys.execPath}/../lib/minigrace/modules/\"++fileName))then{",
  "        return io.open(\"{sys.execPath}/../lib/minigrace/modules/\"++fileName,\"r\").read ",
  "    }   ",
  "    if(io.exists(\"{sys.execPath}/\"++fileName))then{",
  "        return io.open(\"{sys.execPath}/\"++fileName,\"r\").read ",
  "    }   ",
  "    if(io.exists(\"{getBuildPath()}/{fileName}\"))then{",
  "        print(\"YES IT DOES\");",
  "        return io.open(\"{getBuildPath()}/{fileName}\",\"r\").read ",
  "    } ",
  "    if(bundlePath != \"\")then{",
  "        return io.open(\"{bundlePath}/{fileName}\",\"r\").read;",
  "    }",
  "",
  "    return false",
  "}",
  "",
  "method validateFile(file) -> Boolean{",
  "    if ((file.data.size)>1)then{",
  "        if(file.data[1]==\"<\")then{",
  "            print(\"Not a valid grace file\");",
  "            return false",
  "        }",
  "    }",
  "    return true",
  "}",
  "",
  "method write(file) -> Boolean{",
  "    var usrDir := \"\"",
  "    if(global) then { ",
  "       usrDir := \"usr/lib/grace/modules/\"",
  "    }",
  "    else{",
  "        usrDir := sys.environ[\"HOME\"]++\"/.local/lib/grace/modules/\"",
  "    }",
  "    var fileDir := createDirectory(usrDir++file.address)",
  "    var toWrite := io.open(fileDir[1]++fileDir[2], \"w\")",
  "    toWrite.write(file.data);",
  "    toWrite.close;",
  "    ",
  " }",
  "",
  "method getBuildPath() -> String{",
  "    var paths := sys.environ[\"PATH\"]",
  "    var buildPath := \"\"",
  "    for (1..paths.size) do { t->",
  "        if(paths.at(t) == \":\")then{",
  "            if(io.exists(buildPath++\"/minigrace\"))then{",
  "               return buildPath; ",
  "            }",
  "            buildPath := \"\"",
  "        } ",
  "        else{",
  "            buildPath :=  buildPath++paths[t]",
  "        }",
  "    }",
  "    return buildPath;",
  "}",
  "",
  "method compile(file){",
  "    var usrDir := \"\"",
  "    if(global) then { ",
  "       usrDir := \"usr/lib/grace/modules/\"",
  "    }",
  "    else{",
  "        usrDir := sys.environ[\"HOME\"]++\"/.local/lib/grace/modules/\"",
  "    }",
  "    var paths := sys.environ[\"PATH\"]",
  "    print(\"PATHS = \"++paths)",
  "    var buildPath := getBuildPath();",
  "    io.system(buildPath++\"/minigrace \"++usrDir++file.address)",
  "    return true;",
  "",
  "}",
  "",
  "method createDirectory(address) -> List{",
  "    var dir:= []",
  "    var nextPath := \"\" ",
  "    var toMake := \"\"",
  "    var count := 1",
  "    print(\"Directory being evaluated {address}\")",
  "    while{count <= address.size} do {",
  "        nextPath := nextPath ++ address[count]",
  "        if(address[count] == \"/\")then{",
  "            toMake := toMake ++ nextPath ",
  "            nextPath := \"\"   ",
  "        }",
  "        count := count+1;",
  "    }",
  "    print(\"Creating directory \"++toMake);",
  "    if (!io.exists(toMake))then{",
  "        io.system(\"mkdir -p \"++toMake)",
  "        print(\"made directory {toMake}\")",
  "    }",
  "    dir.push(toMake)",
  "    dir.push(nextPath)",
  "    return dir",
  "}",
  "",
  "method parseFile(file){",
  "    var data := file.data",
  "    var curPos := 1;",
  "    var startPos := curPos",
  "    while{curPos <= data.size}do{",
  "        startPos := curPos",
  "        while {(curPos <= data.size) && (data[curPos] != \"\\n\")}do{",
  "            curPos := curPos+1;",
  "        }",
  "        var line := data.substringFrom(startPos)to(curPos-1)",
  "        if (!processLine(line))then{",
  "            return",
  "        }",
  "        curPos := curPos + 1",
  "    }",
  "}",
  "",
  "method processLine(line) -> Boolean {",
  "    if (line.size > 1)then{",
  "        if (line[1] == \"#\")then{ ",
  "            return true",
  "        }",
  "        elseif ((line.size > 2) && (line.substringFrom(1)to(2) == \"//\"))then{",
  "            return true",
  "        }",
  "        elseif ((line.size > 6) && (line.substringFrom(1)to(7) == \"import \"))then{",
  "            parseImport(line.substringFrom(8)to(line.size));",
  "            return true",
  "        }",
  "        elseif ((line.size > 7) && (line.substringFrom(1)to(8) == \"dialect \"))then{",
  "            return true",
  "        }",
  "    }",
  "    return false",
  "}",
  "",
  "method parseImport(line) -> Boolean{",
  "    var curPos := 1",
  "    var startPos := curPos",
  "    var nextImport := \"\"",
  "    curPos := skipWhiteSpace(line,curPos)",
  "    curPos := curPos +1 ",
  "    startPos := curPos",
  "    while{((curPos < line.size) && !((line[curPos+1] == \"\\\"\")||(line[curPos+1] == \" \")))} do {",
  "        curPos := curPos+1",
  "    }",
  "    nextImport := line.substringFrom(startPos)to(curPos)",
  "    if (!imported.contains(nextImport))then{   ",
  "        print(\"next import = \"++nextImport)",
  "        toProcess.push(nextImport)",
  "    }",
  "}",
  "",
  "method skipWhiteSpace(line, count) -> Number{ ",
  "    var count2 := count",
  "    while{(count2 <= line.size) && (line[count2] == \"Could not retrieve \")}do{",
  "        count2 := count2 + 1",
  "    }",
  "    count2",
  "}",
  "",
  "method displayHelp{",
  "    print(\"Available options are:\")",
  "    print(\"-get [url] to fetch a package located at a given url\")",
  "    print(\"-list to display currenly installed packages\")",
  "}",
  "",
  "method printMessage(message){",
  "    if (verbose)then{",
  "        print(message)",
  "    }",
  "}",
  "",
  "//ToBundle is the name of the file that needs to be compiled. The compiler",
  "//must be in the directory of the file to bundle in order for this to run ",
  "//Name is the name of the package passed into the compiler",
  "method bundle(toBundle,name){",
  "    imported := []",
  "    bundlePath := getContainingDirectory(toBundle);",
  "    var newDir := createDirectory(\"{name}/\")",
  "    var newFileName := removeContainingDir(toBundle);",
  "    var toOpen := io.open(\"{toBundle}\",\"r\");",
  "    var openData := toOpen.read",
  "    var toWrite := io.open(\"{name}/{newFileName}\", \"w\")",
  "    toWrite.write(openData);",
  "    fetchImports(toBundle)",
  "    print(\"Imported size = \"++imported.size);",
  "    while{imported.size > 0 }do{",
  "        var curImport := imported.pop",
  "        toWrite := io.open(\"{name}/{removeContainingDir(curImport.address)}\",\"w\")",
  "        toWrite.write(curImport.data)   ",
  "    }",
  "}",
  "",
  "method setImportDest(address,prefix){",
  "    var folder := io.getdir(address);",
  "    for(folder) do { file-> ",
  "        if (file.size > 4)then{",
  "            if (file.substringFrom(file.size-5)to(file.size) == \".grace\")then{",
  "                var open := io.open(address++\"/\"++file,\"r\");",
  "                var readFile := open.read;",
  "                var toWrite := parseAndPrefix(readFile,address,prefix);",
  "                open.close;",
  "",
  "                io.system(\"rm \"++address++\"/\"++file)",
  "                var out := io.open(address++\"/\"++file,\"w\");",
  "                for (toWrite) do { d-> ",
  "                    out.write(d);",
  "                }",
  "            }",
  "        }",
  "    }",
  "    //go through directory looking at each file with a .grace suffix.",
  "    //process each file and look at each import statement",
  "    //prepend ",
  "}",
  "",
  "",
  "method parseAndPrefix (readFile: String, address : String,  prefix : String){ ",
  "    var curPos := 1;",
  "    var startPos := curPos",
  "    var outFile := [];",
  "    for (0..readFile.size) do { t-> ",
  "        while{curPos <= readFile.size}do{",
  "            startPos := curPos",
  "            while {(curPos <= readFile.size) && (readFile[curPos] != \"\\n\")}do{",
  "                curPos := curPos+1;",
  "            }",
  "            var data := readFile.substringFrom(startPos)to(curPos-1)",
  "            var line := \"\";",
  "            var count := 1;",
  "            for (startPos..(curPos-1)) do { b-> ",
  "                line := line++readFile[b];",
  "            }",
  "            print(line);",
  "            if ((line.size > 6) && (line.substringFrom(1)to(7) == \"import \"))then{",
  "                var pos := 1",
  "                var start := pos",
  "                var nextImport := \"\"",
  "                pos := skipWhiteSpace(line,pos)",
  "                pos:= pos +1 ",
  "                print(\"LINE = \"++line);",
  "                while{((pos < line.size) && !((line[pos] == \"\\\"\")||(line[pos] == \" \")))} do {",
  "                    pos := pos+1",
  "                }",
  "                pos:= pos+1;",
  "                var remainder := removeExistingUrls(line.substringFrom(pos+1)to(line.size));",
  "                line := \"import \\\"\"++prefix++\"/\"++remainder;",
  "                print(line);",
  "                for (line) do{ d->",
  "                    outFile.push(d);",
  "                }",
  "            }",
  "            else{",
  "                for (line) do{ d->",
  "                    outFile.push(d);",
  "                }",
  "            }",
  "            curPos := curPos + 1",
  "        }",
  "    }",
  "",
  "",
  "    print (\"OUTFILE \"++outFile)",
  "    return outFile;",
  "",
  "}",
  "",
  "method removeExistingUrls(importStatement : String) -> String{",
  "",
  "    if (importStatement.size < 7)then{",
  "        return importStatement;",
  "    }",
  "    if (importStatement.substringFrom(1)to(7) == \"http://\" )then{",
  "        var lastIndex := 7;",
  "        var curPos := 7;",
  "        while {curPos <= importStatement.size}do{",
  "            if (importStatement[curPos] == \"/\")then{",
  "                lastIndex := curPos;",
  "            }",
  "            curPos := curPos +1;",
  "        }",
  "        var res := importStatement.substringFrom(lastIndex+1)to(importStatement.size);",
  "        return res;",
  "    }",
  "    else{",
  "        return importStatement;",
  "    }",
  "",
  "",
  "}",
  "",
  "method removeContainingDir(st:String) -> String{",
  "    var count := 1;",
  "    var lastIndex := -1;",
  "    while{count <= st.size}do{",
  "        if (st[count] == \"/\") then{",
  "            lastIndex := count;",
  "        }",
  "        count := count+1;",
  "    }",
  "    if(lastIndex == -1)then{",
  "        return st;",
  "    }",
  "    return st.substringFrom(lastIndex+1)to(st.size); ",
  "}",
  "",
  "method getContainingDirectory(st : String) -> String{",
  "    var count := 1;",
  "    var last := -1;",
  "    while {count <= st.size}do{",
  "        if (st[count] == \"/\")then{",
  "            last := count;",
  "        }",
  "        count := count+1;",
  "    }",
  "    if (last == -1)then{",
  "        return getBuildPath()++\"/\";",
  "    }",
  "    return st.substringFrom(0)to(last-1);",
  "",
  "}",
  "",
  NULL
};
Object reader_PackageManager_outer_66(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject*)self;
  return uo->data[0];
}
Object reader_PackageManager_outer_397(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject*)self;
  return uo->data[0];
}
Object reader_PackageManager_address_398(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  return uo->data[1];
}
Object writer_PackageManager_address_398(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  uo->data[1] = args[0];
  return done;
}
Object reader_PackageManager_data_399(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  return uo->data[2];
}
Object writer_PackageManager_data_399(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  uo->data[2] = args[0];
  return done;
}
static Object strlit8;
static Object strlit11;
static Object strlit19;
static Object strlit27;
static Object strlit35;
static Object strlit43;
static Object strlit51;
static ClassData objclass65;
static Object strlit75;
static Object strlit78;
static Object strlit86;
static Object strlit89;
static Object strlit105;
static Object strlit108;
static Object strlit132;
static Object strlit135;
static Object strlit153;
static Object strlit156;
static Object strlit185;
static Object strlit195;
static Object strlit198;
static Object strlit206;
static Object strlit209;
static Object strlit222;
static Object strlit246;
static Object strlit256;
static Object strlit259;
static Object strlit274;
static Object strlit278;
static Object strlit280;
static Object strlit282;
static Object strlit284;
static Object strlit291;
static Object strlit293;
static Object strlit300;
static Object strlit302;
static Object strlit306;
static Object strlit317;
static Object strlit321;
static Object strlit336;
static Object strlit349;
static Object strlit355;
static Object strlit393;
static ClassData objclass396;
static Object strlit409;
static Object strlit414;
static Object strlit425;
static Object strlit433;
static Object strlit438;
static Object strlit469;
static Object strlit476;
static Object strlit488;
static Object strlit523;
static Object strlit529;
static Object strlit534;
static Object strlit538;
static Object strlit557;
static Object strlit562;
static Object strlit569;
static Object strlit583;
static Object strlit603;
static Object strlit610;
static Object strlit617;
static Object strlit620;
static Object strlit625;
static Object strlit632;
static Object strlit635;
static Object strlit646;
static Object strlit649;
static Object strlit654;
static Object strlit662;
static Object strlit667;
static Object strlit678;
static Object strlit683;
static Object strlit688;
static Object strlit696;
static Object strlit701;
static Object strlit712;
static Object strlit717;
static Object strlit722;
static Object strlit731;
static Object strlit736;
static Object strlit741;
static Object strlit745;
static Object strlit753;
static Object strlit758;
static Object strlit763;
static Object strlit766;
static Object strlit771;
static Object strlit780;
static Object strlit783;
static Object strlit788;
static Object strlit791;
static Object strlit811;
static Object strlit814;
static Object strlit819;
static Object strlit821;
static Object strlit826;
static Object strlit828;
static Object strlit847;
static Object strlit858;
static Object strlit860;
static Object strlit874;
static Object strlit880;
static Object strlit884;
static Object strlit894;
static Object strlit896;
static Object strlit901;
static Object strlit903;
static Object strlit909;
static Object strlit912;
static Object strlit922;
static Object strlit935;
static Object strlit936;
static Object strlit940;
static Object strlit943;
static Object strlit966;
static Object strlit973;
static Object strlit983;
static Object strlit994;
static Object strlit1000;
static Object strlit1003;
static Object strlit1038;
static Object strlit1083;
static Object strlit1100;
static Object strlit1119;
static Object strlit1145;
static Object strlit1154;
static Object strlit1181;
static Object strlit1190;
static Object strlit1217;
static Object strlit1235;
static Object strlit1251;
static Object strlit1253;
static Object strlit1255;
static Object strlit1269;
static Object strlit1272;
static Object strlit1281;
static Object strlit1284;
static Object strlit1287;
static Object strlit1296;
static Object strlit1299;
static Object strlit1304;
static Object strlit1307;
static Object strlit1314;
static Object strlit1339;
static Object strlit1342;
static Object strlit1351;
static Object strlit1354;
static Object strlit1388;
static Object strlit1394;
static Object strlit1399;
static Object strlit1411;
static Object strlit1414;
static Object strlit1423;
static Object strlit1428;
static Object strlit1474;
static Object strlit1495;
static Object strlit1528;
static Object strlit1534;
static Object strlit1544;
static Object strlit1561;
static Object strlit1566;
static Object strlit1602;
static Object strlit1605;
static Object strlit1636;
static Object strlit1654;
static Object strlit1673;
static Object strlit1711;
static Object strlit1756;
static Object strlit1777;
Object module_sys;
Object module_sys_init();
Object module_io;
Object module_io_init();
Object module_curl;
Object module_curl_init();
static Object strlit1802;
Object meth_PackageManager__apply13(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object self = *(getfromclosure(closure, 0));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *var_toGet = &(stackframe->slots[0]);
  *var_toGet = args[curarg];
  curarg++;
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 20
  setline(20);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe14 = gc_frame_new();
// compilenode returning *var_toGet
  gc_frame_newslot(*var_toGet);
  params[0] = *var_toGet;
  partcv[0] = 1;
  Object call15 = callmethodflags(self, "doGet", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe14);
// compilenode returning call15
  gc_frame_end(frame);
  return call15;
}
Object meth_PackageManager__apply21(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object self = *(getfromclosure(closure, 0));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 23
  setline(23);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe22 = gc_frame_new();
  partcv[0] = 0;
  Object call23 = callmethodflags(self, "listInstalled", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe22);
// compilenode returning call23
  gc_frame_end(frame);
  return call23;
}
Object meth_PackageManager__apply29(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(3, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 2)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[1];
  Object self = *(getfromclosure(closure, 0));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *var_toBundle = &(stackframe->slots[0]);
  *var_toBundle = args[curarg];
  curarg++;
  Object *var_name = &(stackframe->slots[1]);
  *var_name = args[curarg];
  curarg++;
  Object *selfslot = &(stackframe->slots[2]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 26
  setline(26);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe30 = gc_frame_new();
// compilenode returning *var_toBundle
  gc_frame_newslot(*var_toBundle);
// compilenode returning *var_name
  gc_frame_newslot(*var_name);
  params[0] = *var_toBundle;
  params[1] = *var_name;
  partcv[0] = 2;
  Object call31 = callmethodflags(self, "bundle", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe30);
// compilenode returning call31
  gc_frame_end(frame);
  return call31;
}
Object meth_PackageManager__apply37(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(3, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 2)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[1];
  Object self = *(getfromclosure(closure, 0));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *var_address = &(stackframe->slots[0]);
  *var_address = args[curarg];
  curarg++;
  Object *var_prefix = &(stackframe->slots[1]);
  *var_prefix = args[curarg];
  curarg++;
  Object *selfslot = &(stackframe->slots[2]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 29
  setline(29);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe38 = gc_frame_new();
// compilenode returning *var_address
  gc_frame_newslot(*var_address);
// compilenode returning *var_prefix
  gc_frame_newslot(*var_prefix);
  params[0] = *var_address;
  params[1] = *var_prefix;
  partcv[0] = 2;
  Object call39 = callmethodflags(self, "setImportDest", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe38);
// compilenode returning call39
  gc_frame_end(frame);
  return call39;
}
Object meth_PackageManager__apply45(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_verbose = getfromclosure(closure, 0);
  Object self = *(getfromclosure(closure, 1));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 32
  setline(32);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool46 = alloc_Boolean(1);
// compilenode returning bool46
  *var_verbose = bool46;
  if (bool46 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply53(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_global = getfromclosure(closure, 0);
  Object self = *(getfromclosure(closure, 1));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 35
  setline(35);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool54 = alloc_Boolean(1);
// compilenode returning bool54
  *var_global = bool54;
  if (bool54 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply7(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(3, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 2)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  Object *var_verbose = getfromclosure(closure, 0);
  Object *var_global = getfromclosure(closure, 1);
  Object self = *(getfromclosure(closure, 2));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *var_arg = &(stackframe->slots[0]);
  *var_arg = args[curarg];
  curarg++;
  Object *var_on = &(stackframe->slots[1]);
  *var_on = args[curarg];
  curarg++;
  Object *selfslot = &(stackframe->slots[2]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 18
  setline(18);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit8 == NULL) {
    strlit8 = alloc_String("in loop");
    gc_root(strlit8);
  }
// compilenode returning strlit8
  params[0] = strlit8;
  Object call9 = gracelib_print(NULL, 1,  params);
// compilenode returning call9
// Begin line 22
  setline(22);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe10 = gc_frame_new();
// Begin line 19
  setline(19);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit11 == NULL) {
    strlit11 = alloc_String("get");
    gc_root(strlit11);
  }
// compilenode returning strlit11
  gc_frame_newslot(strlit11);
// Begin line 22
  setline(22);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block12 = alloc_Block(NULL, NULL, "PackageManager", 22);
  gc_frame_newslot(block12);
  block_savedest(block12);
  Object closure13 = createclosure(1, "_apply");
setclosureframe(closure13, stackframe);
  addtoclosure(closure13, selfslot);
  struct UserObject *uo13 = (struct UserObject*)block12;
  uo13->data[0] = (Object)closure13;
  Method *meth_meth_PackageManager__apply13 = addmethod2pos(block12, "_apply", &meth_PackageManager__apply13, 0);
int argcv_meth_PackageManager__apply13[] = {1};
meth_meth_PackageManager__apply13->type = alloc_MethodType(1, argcv_meth_PackageManager__apply13);
  meth_meth_PackageManager__apply13->definitionModule = modulename;
  meth_meth_PackageManager__apply13->definitionLine = 16;
// compilenode returning block12
  gc_frame_newslot(block12);
// Begin line 19
  setline(19);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_on
  params[0] = strlit11;
  params[1] = block12;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call17 = callmethod(*var_on, "option()do",
    2, partcv, params);
  gc_frame_end(callframe10);
// compilenode returning call17
// Begin line 25
  setline(25);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe18 = gc_frame_new();
// Begin line 22
  setline(22);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit19 == NULL) {
    strlit19 = alloc_String("list");
    gc_root(strlit19);
  }
// compilenode returning strlit19
  gc_frame_newslot(strlit19);
// Begin line 25
  setline(25);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block20 = alloc_Block(NULL, NULL, "PackageManager", 25);
  gc_frame_newslot(block20);
  block_savedest(block20);
  Object closure21 = createclosure(1, "_apply");
setclosureframe(closure21, stackframe);
  addtoclosure(closure21, selfslot);
  struct UserObject *uo21 = (struct UserObject*)block20;
  uo21->data[0] = (Object)closure21;
  Method *meth_meth_PackageManager__apply21 = addmethod2pos(block20, "_apply", &meth_PackageManager__apply21, 0);
int argcv_meth_PackageManager__apply21[] = {0};
meth_meth_PackageManager__apply21->type = alloc_MethodType(1, argcv_meth_PackageManager__apply21);
  meth_meth_PackageManager__apply21->definitionModule = modulename;
  meth_meth_PackageManager__apply21->definitionLine = 16;
// compilenode returning block20
  gc_frame_newslot(block20);
// Begin line 22
  setline(22);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_on
  params[0] = strlit19;
  params[1] = block20;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call25 = callmethod(*var_on, "flag()do",
    2, partcv, params);
  gc_frame_end(callframe18);
// compilenode returning call25
// Begin line 28
  setline(28);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe26 = gc_frame_new();
// Begin line 25
  setline(25);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit27 == NULL) {
    strlit27 = alloc_String("bundle");
    gc_root(strlit27);
  }
// compilenode returning strlit27
  gc_frame_newslot(strlit27);
// Begin line 28
  setline(28);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block28 = alloc_Block(NULL, NULL, "PackageManager", 28);
  gc_frame_newslot(block28);
  block_savedest(block28);
  Object closure29 = createclosure(1, "_apply");
setclosureframe(closure29, stackframe);
  addtoclosure(closure29, selfslot);
  struct UserObject *uo29 = (struct UserObject*)block28;
  uo29->data[0] = (Object)closure29;
  Method *meth_meth_PackageManager__apply29 = addmethod2pos(block28, "_apply", &meth_PackageManager__apply29, 0);
int argcv_meth_PackageManager__apply29[] = {2};
meth_meth_PackageManager__apply29->type = alloc_MethodType(1, argcv_meth_PackageManager__apply29);
  meth_meth_PackageManager__apply29->definitionModule = modulename;
  meth_meth_PackageManager__apply29->definitionLine = 16;
// compilenode returning block28
  gc_frame_newslot(block28);
// Begin line 25
  setline(25);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_on
  params[0] = strlit27;
  params[1] = block28;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call33 = callmethod(*var_on, "doubleValue()do",
    2, partcv, params);
  gc_frame_end(callframe26);
// compilenode returning call33
// Begin line 31
  setline(31);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe34 = gc_frame_new();
// Begin line 28
  setline(28);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit35 == NULL) {
    strlit35 = alloc_String("setAddress");
    gc_root(strlit35);
  }
// compilenode returning strlit35
  gc_frame_newslot(strlit35);
// Begin line 31
  setline(31);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block36 = alloc_Block(NULL, NULL, "PackageManager", 31);
  gc_frame_newslot(block36);
  block_savedest(block36);
  Object closure37 = createclosure(1, "_apply");
setclosureframe(closure37, stackframe);
  addtoclosure(closure37, selfslot);
  struct UserObject *uo37 = (struct UserObject*)block36;
  uo37->data[0] = (Object)closure37;
  Method *meth_meth_PackageManager__apply37 = addmethod2pos(block36, "_apply", &meth_PackageManager__apply37, 0);
int argcv_meth_PackageManager__apply37[] = {2};
meth_meth_PackageManager__apply37->type = alloc_MethodType(1, argcv_meth_PackageManager__apply37);
  meth_meth_PackageManager__apply37->definitionModule = modulename;
  meth_meth_PackageManager__apply37->definitionLine = 16;
// compilenode returning block36
  gc_frame_newslot(block36);
// Begin line 28
  setline(28);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_on
  params[0] = strlit35;
  params[1] = block36;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call41 = callmethod(*var_on, "doubleValue()do",
    2, partcv, params);
  gc_frame_end(callframe34);
// compilenode returning call41
// Begin line 34
  setline(34);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe42 = gc_frame_new();
// Begin line 31
  setline(31);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit43 == NULL) {
    strlit43 = alloc_String("verbose");
    gc_root(strlit43);
  }
// compilenode returning strlit43
  gc_frame_newslot(strlit43);
// Begin line 34
  setline(34);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block44 = alloc_Block(NULL, NULL, "PackageManager", 34);
  gc_frame_newslot(block44);
  block_savedest(block44);
  Object closure45 = createclosure(2, "_apply");
setclosureframe(closure45, stackframe);
  addtoclosure(closure45, var_verbose);
  addtoclosure(closure45, selfslot);
  struct UserObject *uo45 = (struct UserObject*)block44;
  uo45->data[0] = (Object)closure45;
  Method *meth_meth_PackageManager__apply45 = addmethod2pos(block44, "_apply", &meth_PackageManager__apply45, 0);
int argcv_meth_PackageManager__apply45[] = {0};
meth_meth_PackageManager__apply45->type = alloc_MethodType(1, argcv_meth_PackageManager__apply45);
  meth_meth_PackageManager__apply45->definitionModule = modulename;
  meth_meth_PackageManager__apply45->definitionLine = 16;
// compilenode returning block44
  gc_frame_newslot(block44);
// Begin line 31
  setline(31);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_on
  params[0] = strlit43;
  params[1] = block44;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call49 = callmethod(*var_on, "flag()do",
    2, partcv, params);
  gc_frame_end(callframe42);
// compilenode returning call49
// Begin line 37
  setline(37);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe50 = gc_frame_new();
// Begin line 34
  setline(34);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit51 == NULL) {
    strlit51 = alloc_String("global");
    gc_root(strlit51);
  }
// compilenode returning strlit51
  gc_frame_newslot(strlit51);
// Begin line 37
  setline(37);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block52 = alloc_Block(NULL, NULL, "PackageManager", 37);
  gc_frame_newslot(block52);
  block_savedest(block52);
  Object closure53 = createclosure(2, "_apply");
setclosureframe(closure53, stackframe);
  addtoclosure(closure53, var_global);
  addtoclosure(closure53, selfslot);
  struct UserObject *uo53 = (struct UserObject*)block52;
  uo53->data[0] = (Object)closure53;
  Method *meth_meth_PackageManager__apply53 = addmethod2pos(block52, "_apply", &meth_PackageManager__apply53, 0);
int argcv_meth_PackageManager__apply53[] = {0};
meth_meth_PackageManager__apply53->type = alloc_MethodType(1, argcv_meth_PackageManager__apply53);
  meth_meth_PackageManager__apply53->definitionModule = modulename;
  meth_meth_PackageManager__apply53->definitionLine = 16;
// compilenode returning block52
  gc_frame_newslot(block52);
// Begin line 34
  setline(34);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_on
  params[0] = strlit51;
  params[1] = block52;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call57 = callmethod(*var_on, "flag()do",
    2, partcv, params);
  gc_frame_end(callframe50);
// compilenode returning call57
  gc_frame_end(frame);
  return call57;
}
Object meth_PackageManager_parseArgs0(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 1, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(3, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "parseArgs");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  Object *var_verbose = getfromclosure(closure, 0);
  Object *var_global = getfromclosure(closure, 1);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for parseArgs (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_args = &(stackframe->slots[0]);
  *var_args = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for parseArgs");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_count = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "count");
// Begin line 15
  setline(15);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1 = alloc_Float64(0);
// compilenode returning num1
  *var_count = num1;
  if (num1 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 16
  setline(16);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe2 = gc_frame_new();
// compilenode returning *var_args
  partcv[0] = 0;
  Object call3 = callmethod(*var_args, "size",
    1, partcv, params);
  gc_frame_end(callframe2);
// compilenode returning call3
// compilenode returning call3
  params[0] = call3;
  Object call4 = gracelib_print(NULL, 1,  params);
// compilenode returning call4
// Begin line 38
  setline(38);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe5 = gc_frame_new();
// Begin line 17
  setline(17);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_args
  gc_frame_newslot(*var_args);
// Begin line 38
  setline(38);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block6 = alloc_Block(NULL, NULL, "PackageManager", 38);
  gc_frame_newslot(block6);
  block_savedest(block6);
  Object closure7 = createclosure(3, "_apply");
setclosureframe(closure7, stackframe);
  addtoclosure(closure7, var_verbose);
  addtoclosure(closure7, var_global);
  addtoclosure(closure7, selfslot);
  struct UserObject *uo7 = (struct UserObject*)block6;
  uo7->data[0] = (Object)closure7;
  Method *meth_meth_PackageManager__apply7 = addmethod2pos(block6, "_apply", &meth_PackageManager__apply7, 0);
int argcv_meth_PackageManager__apply7[] = {2};
meth_meth_PackageManager__apply7->type = alloc_MethodType(1, argcv_meth_PackageManager__apply7);
  meth_meth_PackageManager__apply7->definitionModule = modulename;
  meth_meth_PackageManager__apply7->definitionLine = 16;
// compilenode returning block6
  gc_frame_newslot(block6);
  params[0] = *var_args;
  params[1] = block6;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call59 = callmethodflags(self, "forArgs()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe5);
// compilenode returning call59
  gc_frame_end(frame);
  return call59;
}
Object meth_PackageManager_option_40__41_shortHand_40__41_do67(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 1, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(5, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "option()shortHand()do");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  if (nparts > 0 && argcv[1] < 1)
    gracedie("insufficient arguments to method");
  if (nparts > 0 && argcv[2] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_i = getfromclosure(closure, 0);
  Object *var_args = getfromclosure(closure, 1);
  Object *var_ran = getfromclosure(closure, 2);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 3 && args)
    gracedie("missing argument list for option()shortHand()do (probably reflection error): got %i lists, expected 3.", nparts);
  Object *var_name = &(stackframe->slots[0]);
  *var_name = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for option");
  Object *var_sh = &(stackframe->slots[1]);
  *var_sh = args[curarg];
  curarg++;
if (argcv && argcv[1] > 1)
  gracedie("too many arguments for shortHand");
  Object *var_block_39_ = &(stackframe->slots[2]);
  *var_block_39_ = args[curarg];
  curarg++;
if (argcv && argcv[2] > 1)
  gracedie("too many arguments for do");
  Object *selfslot = &(stackframe->slots[3]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_arg = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "arg");
// Begin line 48
  setline(48);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe68 = gc_frame_new();
// Begin line 47
  setline(47);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call69 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe68);
// compilenode returning call69
  *var_arg = call69;
  if (call69 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 57
  setline(57);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 48
  setline(48);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arg
  int op_slot_left_72 = gc_frame_newslot(*var_arg);
  if (strlit75 == NULL) {
    strlit75 = alloc_String("--");
    gc_root(strlit75);
  }
// compilenode returning strlit75
  int op_slot_left_74 = gc_frame_newslot(strlit75);
// compilenode returning *var_name
  int op_slot_right_74 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult77 = callmethod(strlit75, "++", 1, partcv, params);
// compilenode returning opresult77
  int op_slot_left_73 = gc_frame_newslot(opresult77);
  if (strlit78 == NULL) {
    strlit78 = alloc_String("");
    gc_root(strlit78);
  }
// compilenode returning strlit78
  int op_slot_right_73 = gc_frame_newslot(strlit78);
  params[0] = strlit78;
  partcv[0] = 1;
  Object opresult80 = callmethod(opresult77, "++", 1, partcv, params);
// compilenode returning opresult80
  int op_slot_right_72 = gc_frame_newslot(opresult80);
  params[0] = opresult80;
  partcv[0] = 1;
  Object opresult82 = callmethod(*var_arg, "==", 1, partcv, params);
// compilenode returning opresult82
  int op_slot_left_71 = gc_frame_newslot(opresult82);
// compilenode returning *var_arg
  int op_slot_left_83 = gc_frame_newslot(*var_arg);
  if (strlit86 == NULL) {
    strlit86 = alloc_String("-");
    gc_root(strlit86);
  }
// compilenode returning strlit86
  int op_slot_left_85 = gc_frame_newslot(strlit86);
// compilenode returning *var_sh
  int op_slot_right_85 = gc_frame_newslot(*var_sh);
  params[0] = *var_sh;
  partcv[0] = 1;
  Object opresult88 = callmethod(strlit86, "++", 1, partcv, params);
// compilenode returning opresult88
  int op_slot_left_84 = gc_frame_newslot(opresult88);
  if (strlit89 == NULL) {
    strlit89 = alloc_String("");
    gc_root(strlit89);
  }
// compilenode returning strlit89
  int op_slot_right_84 = gc_frame_newslot(strlit89);
  params[0] = strlit89;
  partcv[0] = 1;
  Object opresult91 = callmethod(opresult88, "++", 1, partcv, params);
// compilenode returning opresult91
  int op_slot_right_83 = gc_frame_newslot(opresult91);
  params[0] = opresult91;
  partcv[0] = 1;
  Object opresult93 = callmethod(*var_arg, "==", 1, partcv, params);
// compilenode returning opresult93
  int op_slot_right_71 = gc_frame_newslot(opresult93);
  params[0] = opresult93;
  partcv[0] = 1;
  Object opresult95 = callmethod(opresult82, "||", 1, partcv, params);
// compilenode returning opresult95
  Object if70 = done;
  if (istrue(opresult95)) {
// Begin line 53
  setline(53);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 49
  setline(49);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe98 = gc_frame_new();
// compilenode returning *var_args
  partcv[0] = 0;
  Object call99 = callmethod(*var_args, "size",
    1, partcv, params);
  gc_frame_end(callframe98);
// compilenode returning call99
// compilenode returning call99
  int op_slot_left_97 = gc_frame_newslot(call99);
// compilenode returning *var_i
  int op_slot_right_97 = gc_frame_newslot(*var_i);
  params[0] = *var_i;
  partcv[0] = 1;
  Object opresult101 = callmethod(call99, "==", 1, partcv, params);
// compilenode returning opresult101
  Object if96 = done;
  if (istrue(opresult101)) {
// Begin line 51
  setline(51);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe102 = gc_frame_new();
// Begin line 50
  setline(50);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit105 == NULL) {
    strlit105 = alloc_String("Missing value for option ");
    gc_root(strlit105);
  }
// compilenode returning strlit105
  int op_slot_left_104 = gc_frame_newslot(strlit105);
// compilenode returning *var_name
  int op_slot_right_104 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult107 = callmethod(strlit105, "++", 1, partcv, params);
// compilenode returning opresult107
  int op_slot_left_103 = gc_frame_newslot(opresult107);
  if (strlit108 == NULL) {
    strlit108 = alloc_String("");
    gc_root(strlit108);
  }
// compilenode returning strlit108
  int op_slot_right_103 = gc_frame_newslot(strlit108);
  params[0] = strlit108;
  partcv[0] = 1;
  Object opresult110 = callmethod(opresult107, "++", 1, partcv, params);
// compilenode returning opresult110
  gc_frame_newslot(opresult110);
  int callframe111 = gc_frame_new();
  partcv[0] = 0;
  Object call112 = callmethodflags(prelude, "Exception", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe111);
// compilenode returning call112
  params[0] = opresult110;
  partcv[0] = 1;
  Object call113 = callmethod(call112, "raise",
    1, partcv, params);
  gc_frame_end(callframe102);
// compilenode returning call113
    gc_frame_newslot(call113);
    if96 = call113;
  } else {
  }
// compilenode returning if96
// Begin line 54
  setline(54);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 53
  setline(53);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_114 = gc_frame_newslot(*var_i);
  Object num115 = alloc_Float64(1);
// compilenode returning num115
  int op_slot_right_114 = gc_frame_newslot(num115);
  params[0] = num115;
  partcv[0] = 1;
  Object sum117 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum117
  *var_i = sum117;
  if (sum117 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 55
  setline(55);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe119 = gc_frame_new();
// Begin line 54
  setline(54);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe120 = gc_frame_new();
// compilenode returning *var_i
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call121 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe120);
// compilenode returning call121
  gc_frame_newslot(call121);
// compilenode returning *var_block_39_
  params[0] = call121;
  partcv[0] = 1;
  Object call122 = callmethod(*var_block_39_, "apply",
    1, partcv, params);
  gc_frame_end(callframe119);
// compilenode returning call122
// Begin line 56
  setline(56);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 55
  setline(55);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool123 = alloc_Boolean(1);
// compilenode returning bool123
  *var_ran = bool123;
  if (bool123 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if70 = done;
  } else {
  }
// compilenode returning if70
  gc_frame_end(frame);
  return if70;
}
Object meth_PackageManager_doubleValue_40__41_do125(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 2, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(6, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "doubleValue()do");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  if (nparts > 0 && argcv[1] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[1];
  Object *var_i = getfromclosure(closure, 0);
  Object *var_args = getfromclosure(closure, 1);
  Object *var_ran = getfromclosure(closure, 2);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 2 && args)
    gracedie("missing argument list for doubleValue()do (probably reflection error): got %i lists, expected 2.", nparts);
  Object *var_name = &(stackframe->slots[0]);
  *var_name = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for doubleValue");
  Object *var_block_39_ = &(stackframe->slots[1]);
  *var_block_39_ = args[curarg];
  curarg++;
if (argcv && argcv[1] > 1)
  gracedie("too many arguments for do");
  Object *selfslot = &(stackframe->slots[2]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_arg = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "arg");
  Object *var_arga = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "arga");
  Object *var_argb = &(stackframe->slots[5]);
  setframeelementname(stackframe, 5, "argb");
// Begin line 61
  setline(61);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe126 = gc_frame_new();
// Begin line 60
  setline(60);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call127 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe126);
// compilenode returning call127
  *var_arg = call127;
  if (call127 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 72
  setline(72);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 61
  setline(61);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arg
  int op_slot_left_129 = gc_frame_newslot(*var_arg);
  if (strlit132 == NULL) {
    strlit132 = alloc_String("--");
    gc_root(strlit132);
  }
// compilenode returning strlit132
  int op_slot_left_131 = gc_frame_newslot(strlit132);
// compilenode returning *var_name
  int op_slot_right_131 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult134 = callmethod(strlit132, "++", 1, partcv, params);
// compilenode returning opresult134
  int op_slot_left_130 = gc_frame_newslot(opresult134);
  if (strlit135 == NULL) {
    strlit135 = alloc_String("");
    gc_root(strlit135);
  }
// compilenode returning strlit135
  int op_slot_right_130 = gc_frame_newslot(strlit135);
  params[0] = strlit135;
  partcv[0] = 1;
  Object opresult137 = callmethod(opresult134, "++", 1, partcv, params);
// compilenode returning opresult137
  int op_slot_right_129 = gc_frame_newslot(opresult137);
  params[0] = opresult137;
  partcv[0] = 1;
  Object opresult139 = callmethod(*var_arg, "==", 1, partcv, params);
// compilenode returning opresult139
  Object if128 = done;
  if (istrue(opresult139)) {
// Begin line 65
  setline(65);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 62
  setline(62);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe142 = gc_frame_new();
// compilenode returning *var_args
  partcv[0] = 0;
  Object call143 = callmethod(*var_args, "size",
    1, partcv, params);
  gc_frame_end(callframe142);
// compilenode returning call143
// compilenode returning call143
  int op_slot_left_141 = gc_frame_newslot(call143);
// compilenode returning *var_i
  int op_slot_left_144 = gc_frame_newslot(*var_i);
  Object num145 = alloc_Float64(1);
// compilenode returning num145
  int op_slot_right_144 = gc_frame_newslot(num145);
  params[0] = num145;
  partcv[0] = 1;
  Object sum147 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum147
  int op_slot_right_141 = gc_frame_newslot(sum147);
  params[0] = sum147;
  partcv[0] = 1;
  Object opresult149 = callmethod(call143, "<", 1, partcv, params);
// compilenode returning opresult149
  Object if140 = done;
  if (istrue(opresult149)) {
// Begin line 64
  setline(64);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe150 = gc_frame_new();
// Begin line 63
  setline(63);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit153 == NULL) {
    strlit153 = alloc_String("Missing values for option ");
    gc_root(strlit153);
  }
// compilenode returning strlit153
  int op_slot_left_152 = gc_frame_newslot(strlit153);
// compilenode returning *var_name
  int op_slot_right_152 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult155 = callmethod(strlit153, "++", 1, partcv, params);
// compilenode returning opresult155
  int op_slot_left_151 = gc_frame_newslot(opresult155);
  if (strlit156 == NULL) {
    strlit156 = alloc_String("");
    gc_root(strlit156);
  }
// compilenode returning strlit156
  int op_slot_right_151 = gc_frame_newslot(strlit156);
  params[0] = strlit156;
  partcv[0] = 1;
  Object opresult158 = callmethod(opresult155, "++", 1, partcv, params);
// compilenode returning opresult158
  gc_frame_newslot(opresult158);
  int callframe159 = gc_frame_new();
  partcv[0] = 0;
  Object call160 = callmethodflags(prelude, "Exception", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe159);
// compilenode returning call160
  params[0] = opresult158;
  partcv[0] = 1;
  Object call161 = callmethod(call160, "raise",
    1, partcv, params);
  gc_frame_end(callframe150);
// compilenode returning call161
    gc_frame_newslot(call161);
    if140 = call161;
  } else {
  }
// compilenode returning if140
// Begin line 66
  setline(66);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe162 = gc_frame_new();
// Begin line 65
  setline(65);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_163 = gc_frame_newslot(*var_i);
  Object num164 = alloc_Float64(1);
// compilenode returning num164
  int op_slot_right_163 = gc_frame_newslot(num164);
  params[0] = num164;
  partcv[0] = 1;
  Object sum166 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum166
  gc_frame_newslot(sum166);
// compilenode returning *var_args
  params[0] = sum166;
  partcv[0] = 1;
  Object call167 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe162);
// compilenode returning call167
  *var_arga = call167;
  if (call167 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 67
  setline(67);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe168 = gc_frame_new();
// Begin line 66
  setline(66);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_169 = gc_frame_newslot(*var_i);
  Object num170 = alloc_Float64(2);
// compilenode returning num170
  int op_slot_right_169 = gc_frame_newslot(num170);
  params[0] = num170;
  partcv[0] = 1;
  Object sum172 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum172
  gc_frame_newslot(sum172);
// compilenode returning *var_args
  params[0] = sum172;
  partcv[0] = 1;
  Object call173 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe168);
// compilenode returning call173
  *var_argb = call173;
  if (call173 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 68
  setline(68);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe174 = gc_frame_new();
// Begin line 67
  setline(67);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arga
  gc_frame_newslot(*var_arga);
// compilenode returning *var_argb
  gc_frame_newslot(*var_argb);
// compilenode returning *var_block_39_
  params[0] = *var_arga;
  params[1] = *var_argb;
  partcv[0] = 2;
  Object call175 = callmethod(*var_block_39_, "apply",
    1, partcv, params);
  gc_frame_end(callframe174);
// compilenode returning call175
// Begin line 69
  setline(69);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 68
  setline(68);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_176 = gc_frame_newslot(*var_i);
  Object num177 = alloc_Float64(2);
// compilenode returning num177
  int op_slot_right_176 = gc_frame_newslot(num177);
  params[0] = num177;
  partcv[0] = 1;
  Object sum179 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum179
  *var_i = sum179;
  if (sum179 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 70
  setline(70);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 69
  setline(69);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool181 = alloc_Boolean(1);
// compilenode returning bool181
  *var_ran = bool181;
  if (bool181 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if128 = done;
  } else {
  }
// compilenode returning if128
  gc_frame_end(frame);
  return if128;
}
Object meth_PackageManager_option_40__41_do183(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct StackFrameObject *stackframe = alloc_StackFrame(3, NULL);
  pushclosure(NULL);
  pushstackframe(stackframe, "option()do");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  if (nparts > 0 && argcv[1] < 1)
    gracedie("insufficient arguments to method");
  Object params[3];
  int partcv[3];
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 2 && args)
    gracedie("missing argument list for option()do (probably reflection error): got %i lists, expected 2.", nparts);
  Object *var_name = &(stackframe->slots[0]);
  *var_name = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for option");
  Object *var_block_39_ = &(stackframe->slots[1]);
  *var_block_39_ = args[curarg];
  curarg++;
if (argcv && argcv[1] > 1)
  gracedie("too many arguments for do");
  Object *selfslot = &(stackframe->slots[2]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 76
  setline(76);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe184 = gc_frame_new();
// Begin line 75
  setline(75);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_name
  gc_frame_newslot(*var_name);
  if (strlit185 == NULL) {
    strlit185 = alloc_String("");
    gc_root(strlit185);
  }
// compilenode returning strlit185
  gc_frame_newslot(strlit185);
// compilenode returning *var_block_39_
  gc_frame_newslot(*var_block_39_);
  params[0] = *var_name;
  params[1] = strlit185;
  params[2] = *var_block_39_;
  partcv[0] = 1;
  partcv[1] = 1;
  partcv[2] = 1;
  Object call186 = callmethodflags(self, "option()shortHand()do", 3, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe184);
// compilenode returning call186
  gc_frame_end(frame);
  return call186;
}
Object meth_PackageManager_flag_40__41_shortHand_40__41_do187(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 4, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(5, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "flag()shortHand()do");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  if (nparts > 0 && argcv[1] < 1)
    gracedie("insufficient arguments to method");
  if (nparts > 0 && argcv[2] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_i = getfromclosure(closure, 0);
  Object *var_args = getfromclosure(closure, 1);
  Object *var_ran = getfromclosure(closure, 2);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 3 && args)
    gracedie("missing argument list for flag()shortHand()do (probably reflection error): got %i lists, expected 3.", nparts);
  Object *var_name = &(stackframe->slots[0]);
  *var_name = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for flag");
  Object *var_sh = &(stackframe->slots[1]);
  *var_sh = args[curarg];
  curarg++;
if (argcv && argcv[1] > 1)
  gracedie("too many arguments for shortHand");
  Object *var_block_39_ = &(stackframe->slots[2]);
  *var_block_39_ = args[curarg];
  curarg++;
if (argcv && argcv[2] > 1)
  gracedie("too many arguments for do");
  Object *selfslot = &(stackframe->slots[3]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_arg = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "arg");
// Begin line 80
  setline(80);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe188 = gc_frame_new();
// Begin line 79
  setline(79);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call189 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe188);
// compilenode returning call189
  *var_arg = call189;
  if (call189 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 84
  setline(84);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 80
  setline(80);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arg
  int op_slot_left_192 = gc_frame_newslot(*var_arg);
  if (strlit195 == NULL) {
    strlit195 = alloc_String("--");
    gc_root(strlit195);
  }
// compilenode returning strlit195
  int op_slot_left_194 = gc_frame_newslot(strlit195);
// compilenode returning *var_name
  int op_slot_right_194 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult197 = callmethod(strlit195, "++", 1, partcv, params);
// compilenode returning opresult197
  int op_slot_left_193 = gc_frame_newslot(opresult197);
  if (strlit198 == NULL) {
    strlit198 = alloc_String("");
    gc_root(strlit198);
  }
// compilenode returning strlit198
  int op_slot_right_193 = gc_frame_newslot(strlit198);
  params[0] = strlit198;
  partcv[0] = 1;
  Object opresult200 = callmethod(opresult197, "++", 1, partcv, params);
// compilenode returning opresult200
  int op_slot_right_192 = gc_frame_newslot(opresult200);
  params[0] = opresult200;
  partcv[0] = 1;
  Object opresult202 = callmethod(*var_arg, "==", 1, partcv, params);
// compilenode returning opresult202
  int op_slot_left_191 = gc_frame_newslot(opresult202);
// compilenode returning *var_arg
  int op_slot_left_203 = gc_frame_newslot(*var_arg);
  if (strlit206 == NULL) {
    strlit206 = alloc_String("-");
    gc_root(strlit206);
  }
// compilenode returning strlit206
  int op_slot_left_205 = gc_frame_newslot(strlit206);
// compilenode returning *var_sh
  int op_slot_right_205 = gc_frame_newslot(*var_sh);
  params[0] = *var_sh;
  partcv[0] = 1;
  Object opresult208 = callmethod(strlit206, "++", 1, partcv, params);
// compilenode returning opresult208
  int op_slot_left_204 = gc_frame_newslot(opresult208);
  if (strlit209 == NULL) {
    strlit209 = alloc_String("");
    gc_root(strlit209);
  }
// compilenode returning strlit209
  int op_slot_right_204 = gc_frame_newslot(strlit209);
  params[0] = strlit209;
  partcv[0] = 1;
  Object opresult211 = callmethod(opresult208, "++", 1, partcv, params);
// compilenode returning opresult211
  int op_slot_right_203 = gc_frame_newslot(opresult211);
  params[0] = opresult211;
  partcv[0] = 1;
  Object opresult213 = callmethod(*var_arg, "==", 1, partcv, params);
// compilenode returning opresult213
  int op_slot_right_191 = gc_frame_newslot(opresult213);
  params[0] = opresult213;
  partcv[0] = 1;
  Object opresult215 = callmethod(opresult202, "||", 1, partcv, params);
// compilenode returning opresult215
  Object if190 = done;
  if (istrue(opresult215)) {
// Begin line 81
  setline(81);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe216 = gc_frame_new();
// compilenode returning *var_block_39_
  partcv[0] = 0;
  Object call217 = callmethod(*var_block_39_, "apply",
    1, partcv, params);
  gc_frame_end(callframe216);
// compilenode returning call217
// compilenode returning call217
// Begin line 83
  setline(83);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 82
  setline(82);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool218 = alloc_Boolean(1);
// compilenode returning bool218
  *var_ran = bool218;
  if (bool218 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if190 = done;
  } else {
  }
// compilenode returning if190
  gc_frame_end(frame);
  return if190;
}
Object meth_PackageManager_flag_40__41_do220(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct StackFrameObject *stackframe = alloc_StackFrame(3, NULL);
  pushclosure(NULL);
  pushstackframe(stackframe, "flag()do");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  if (nparts > 0 && argcv[1] < 1)
    gracedie("insufficient arguments to method");
  Object params[3];
  int partcv[3];
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 2 && args)
    gracedie("missing argument list for flag()do (probably reflection error): got %i lists, expected 2.", nparts);
  Object *var_name = &(stackframe->slots[0]);
  *var_name = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for flag");
  Object *var_block_39_ = &(stackframe->slots[1]);
  *var_block_39_ = args[curarg];
  curarg++;
if (argcv && argcv[1] > 1)
  gracedie("too many arguments for do");
  Object *selfslot = &(stackframe->slots[2]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 88
  setline(88);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe221 = gc_frame_new();
// Begin line 87
  setline(87);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_name
  gc_frame_newslot(*var_name);
  if (strlit222 == NULL) {
    strlit222 = alloc_String("");
    gc_root(strlit222);
  }
// compilenode returning strlit222
  gc_frame_newslot(strlit222);
// compilenode returning *var_block_39_
  gc_frame_newslot(*var_block_39_);
  params[0] = *var_name;
  params[1] = strlit222;
  params[2] = *var_block_39_;
  partcv[0] = 1;
  partcv[1] = 1;
  partcv[2] = 1;
  Object call223 = callmethodflags(self, "flag()shortHand()do", 3, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe221);
// compilenode returning call223
  gc_frame_end(frame);
  return call223;
}
Object meth_PackageManager__apply226(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_i = getfromclosure(closure, 0);
  Object *var_size = getfromclosure(closure, 1);
  Object self = *(getfromclosure(closure, 2));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var_i
  int op_slot_left_227 = gc_frame_newslot(*var_i);
// compilenode returning *var_size
  int op_slot_right_227 = gc_frame_newslot(*var_size);
  params[0] = *var_size;
  partcv[0] = 1;
  Object opresult229 = callmethod(*var_i, "<=", 1, partcv, params);
// compilenode returning opresult229
  gc_frame_end(frame);
  return opresult229;
}
Object meth_PackageManager__apply232(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[2];
  int partcv[1];
  Object *var_i = getfromclosure(closure, 0);
  Object *var_args = getfromclosure(closure, 1);
  Object *var_ran = getfromclosure(closure, 2);
  Object *var_on = getfromclosure(closure, 3);
  Object *var_block = getfromclosure(closure, 4);
  Object self = *(getfromclosure(closure, 5));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_arg = &(stackframe->slots[1]);
  setframeelementname(stackframe, 1, "arg");
// Begin line 92
  setline(92);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe233 = gc_frame_new();
// Begin line 91
  setline(91);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call234 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe233);
// compilenode returning call234
  *var_arg = call234;
  if (call234 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 92
  setline(92);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arg
  params[0] = *var_arg;
  Object call235 = gracelib_print(NULL, 1,  params);
// compilenode returning call235
// Begin line 94
  setline(94);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 93
  setline(93);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool236 = alloc_Boolean(0);
// compilenode returning bool236
  *var_ran = bool236;
  if (bool236 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 95
  setline(95);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe238 = gc_frame_new();
// Begin line 94
  setline(94);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arg
  gc_frame_newslot(*var_arg);
// compilenode returning *var_on
  gc_frame_newslot(*var_on);
// compilenode returning *var_block
  params[0] = *var_arg;
  params[1] = *var_on;
  partcv[0] = 2;
  Object call239 = callmethod(*var_block, "apply",
    1, partcv, params);
  gc_frame_end(callframe238);
// compilenode returning call239
// Begin line 99
  setline(99);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 95
  setline(95);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe243 = gc_frame_new();
  Object num244 = alloc_Float64(1);
// compilenode returning num244
  gc_frame_newslot(num244);
// compilenode returning *var_arg
  params[0] = num244;
  partcv[0] = 1;
  Object call245 = callmethod(*var_arg, "at",
    1, partcv, params);
  gc_frame_end(callframe243);
// compilenode returning call245
  int op_slot_left_242 = gc_frame_newslot(call245);
  if (strlit246 == NULL) {
    strlit246 = alloc_String("-");
    gc_root(strlit246);
  }
// compilenode returning strlit246
  int op_slot_right_242 = gc_frame_newslot(strlit246);
  params[0] = strlit246;
  partcv[0] = 1;
  Object opresult248 = callmethod(call245, "==", 1, partcv, params);
// compilenode returning opresult248
  int op_slot_left_241 = gc_frame_newslot(opresult248);
  int callframe249 = gc_frame_new();
// compilenode returning *var_ran
  partcv[0] = 0;
  Object call250 = callmethod(*var_ran, "not",
    1, partcv, params);
  gc_frame_end(callframe249);
// compilenode returning call250
// compilenode returning call250
  int op_slot_right_241 = gc_frame_newslot(call250);
  params[0] = call250;
  partcv[0] = 1;
  Object opresult252 = callmethod(opresult248, "&&", 1, partcv, params);
// compilenode returning opresult252
  Object if240 = done;
  if (istrue(opresult252)) {
// Begin line 97
  setline(97);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe253 = gc_frame_new();
// Begin line 96
  setline(96);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit256 == NULL) {
    strlit256 = alloc_String("Unrecognised argument ");
    gc_root(strlit256);
  }
// compilenode returning strlit256
  int op_slot_left_255 = gc_frame_newslot(strlit256);
// compilenode returning *var_arg
  int op_slot_right_255 = gc_frame_newslot(*var_arg);
  params[0] = *var_arg;
  partcv[0] = 1;
  Object opresult258 = callmethod(strlit256, "++", 1, partcv, params);
// compilenode returning opresult258
  int op_slot_left_254 = gc_frame_newslot(opresult258);
  if (strlit259 == NULL) {
    strlit259 = alloc_String("");
    gc_root(strlit259);
  }
// compilenode returning strlit259
  int op_slot_right_254 = gc_frame_newslot(strlit259);
  params[0] = strlit259;
  partcv[0] = 1;
  Object opresult261 = callmethod(opresult258, "++", 1, partcv, params);
// compilenode returning opresult261
  gc_frame_newslot(opresult261);
  int callframe262 = gc_frame_new();
  partcv[0] = 0;
  Object call263 = callmethodflags(prelude, "Exception", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe262);
// compilenode returning call263
  params[0] = opresult261;
  partcv[0] = 1;
  Object call264 = callmethod(call263, "raise",
    1, partcv, params);
  gc_frame_end(callframe253);
// compilenode returning call264
    gc_frame_newslot(call264);
    if240 = call264;
  } else {
  }
// compilenode returning if240
// Begin line 100
  setline(100);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 99
  setline(99);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_265 = gc_frame_newslot(*var_i);
  Object num266 = alloc_Float64(1);
// compilenode returning num266
  int op_slot_right_265 = gc_frame_newslot(num266);
  params[0] = num266;
  partcv[0] = 1;
  Object sum268 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum268
  *var_i = sum268;
  if (sum268 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_forArgs_40__41_do60(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct StackFrameObject *stackframe = alloc_StackFrame(7, NULL);
  pushclosure(NULL);
  pushstackframe(stackframe, "forArgs()do");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  if (nparts > 0 && argcv[1] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 2 && args)
    gracedie("missing argument list for forArgs()do (probably reflection error): got %i lists, expected 2.", nparts);
  Object *var_args = &(stackframe->slots[0]);
  *var_args = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for forArgs");
  Object *var_block = &(stackframe->slots[1]);
  *var_block = args[curarg];
  curarg++;
if (argcv && argcv[1] > 1)
  gracedie("too many arguments for do");
  Object *selfslot = &(stackframe->slots[2]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_i = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "i");
  Object *var_ran = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "ran");
  Object *var_size = &(stackframe->slots[5]);
  setframeelementname(stackframe, 5, "size");
  Object *var_on = &(stackframe->slots[6]);
  setframeelementname(stackframe, 6, "on");
// Begin line 42
  setline(42);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 41
  setline(41);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num61 = alloc_Float64(2);
// compilenode returning num61
  *var_i = num61;
  if (num61 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 43
  setline(43);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 42
  setline(42);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool62 = alloc_Boolean(0);
// compilenode returning bool62
  *var_ran = bool62;
  if (bool62 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 45
  setline(45);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 43
  setline(43);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe63 = gc_frame_new();
// compilenode returning *var_args
  partcv[0] = 0;
  Object call64 = callmethod(*var_args, "size",
    1, partcv, params);
  gc_frame_end(callframe63);
// compilenode returning call64
// compilenode returning call64
  *var_size = call64;
  if (call64 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 45
  setline(45);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object inheritingObject65 = inheritingObject;
  if (isTailObject) {
    isTailObject = 0;
    inheritingObject = NULL;
  }
  Object obj65 = alloc_userobj2(5,6, objclass65);
  gc_frame_newslot(obj65);
// OBJECT OUTER DEC outer
  adddatum2(obj65, self, 0);
  addmethodreal(obj65,"outer", &reader_PackageManager_outer_66);
  Object oldself65 = self;
  struct StackFrameObject *oldstackframe65 = stackframe;
  stackframe = alloc_StackFrame(1, oldstackframe65);
  gc_frame_newslot((Object)stackframe);
  self = obj65;
  Object *oldselfslot65 = selfslot;
  selfslot = &stackframe->slots[0];
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  Object thisouter65 = (*(struct UserObject *)self).data[0], lowerouter65 = thisouter65;
  if (inheritingObject65) {
    struct UserObject *inho65 = (struct UserObject *)inheritingObject65;
    while (inho65->super != GraceDefaultObject) inho65 = (struct UserObject *)inho65->super;
    inho65->super = obj65;
    self = inheritingObject65;
    *selfslot = self;
    lowerouter65 = (*(struct UserObject *)self).data[0];
    (*(struct UserObject *)self).data[0] = thisouter65;
  }
  block_savedest(obj65);
  Object closure67 = createclosure(3, "option()shortHand()do");
setclosureframe(closure67, stackframe);
  addtoclosure(closure67, var_i);
  addtoclosure(closure67, var_args);
  addtoclosure(closure67, var_ran);
  struct UserObject *uo67 = (struct UserObject*)obj65;
  uo67->data[1] = (Object)closure67;
  Method *meth_meth_PackageManager_option_40__41_shortHand_40__41_do67 = addmethod2pos(obj65, "option()shortHand()do", &meth_PackageManager_option_40__41_shortHand_40__41_do67, 1);
int argcv_meth_PackageManager_option_40__41_shortHand_40__41_do67[] = {1, 1, 1};
meth_meth_PackageManager_option_40__41_shortHand_40__41_do67->type = alloc_MethodType(3, argcv_meth_PackageManager_option_40__41_shortHand_40__41_do67);
meth_meth_PackageManager_option_40__41_shortHand_40__41_do67->type->types[0] = type_String;
meth_meth_PackageManager_option_40__41_shortHand_40__41_do67->type->names[0] = "name";
meth_meth_PackageManager_option_40__41_shortHand_40__41_do67->type->types[1] = type_String;
meth_meth_PackageManager_option_40__41_shortHand_40__41_do67->type->names[1] = "sh";
  meth_meth_PackageManager_option_40__41_shortHand_40__41_do67->definitionModule = modulename;
  meth_meth_PackageManager_option_40__41_shortHand_40__41_do67->definitionLine = 46;
  block_savedest(obj65);
  Object closure125 = createclosure(3, "doubleValue()do");
setclosureframe(closure125, stackframe);
  addtoclosure(closure125, var_i);
  addtoclosure(closure125, var_args);
  addtoclosure(closure125, var_ran);
  struct UserObject *uo125 = (struct UserObject*)obj65;
  uo125->data[2] = (Object)closure125;
  Method *meth_meth_PackageManager_doubleValue_40__41_do125 = addmethod2pos(obj65, "doubleValue()do", &meth_PackageManager_doubleValue_40__41_do125, 2);
int argcv_meth_PackageManager_doubleValue_40__41_do125[] = {1, 1};
meth_meth_PackageManager_doubleValue_40__41_do125->type = alloc_MethodType(2, argcv_meth_PackageManager_doubleValue_40__41_do125);
meth_meth_PackageManager_doubleValue_40__41_do125->type->types[0] = type_String;
meth_meth_PackageManager_doubleValue_40__41_do125->type->names[0] = "name";
  meth_meth_PackageManager_doubleValue_40__41_do125->definitionModule = modulename;
  meth_meth_PackageManager_doubleValue_40__41_do125->definitionLine = 59;
  struct UserObject *uo183 = (struct UserObject*)obj65;
  uo183->data[3] = emptyclosure;
  Method *meth_meth_PackageManager_option_40__41_do183 = addmethod2pos(obj65, "option()do", &meth_PackageManager_option_40__41_do183, 3);
int argcv_meth_PackageManager_option_40__41_do183[] = {1, 1};
meth_meth_PackageManager_option_40__41_do183->type = alloc_MethodType(2, argcv_meth_PackageManager_option_40__41_do183);
meth_meth_PackageManager_option_40__41_do183->type->types[0] = type_String;
meth_meth_PackageManager_option_40__41_do183->type->names[0] = "name";
  meth_meth_PackageManager_option_40__41_do183->definitionModule = modulename;
  meth_meth_PackageManager_option_40__41_do183->definitionLine = 74;
  block_savedest(obj65);
  Object closure187 = createclosure(3, "flag()shortHand()do");
setclosureframe(closure187, stackframe);
  addtoclosure(closure187, var_i);
  addtoclosure(closure187, var_args);
  addtoclosure(closure187, var_ran);
  struct UserObject *uo187 = (struct UserObject*)obj65;
  uo187->data[4] = (Object)closure187;
  Method *meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187 = addmethod2pos(obj65, "flag()shortHand()do", &meth_PackageManager_flag_40__41_shortHand_40__41_do187, 4);
int argcv_meth_PackageManager_flag_40__41_shortHand_40__41_do187[] = {1, 1, 1};
meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187->type = alloc_MethodType(3, argcv_meth_PackageManager_flag_40__41_shortHand_40__41_do187);
meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187->type->types[0] = type_String;
meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187->type->names[0] = "name";
meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187->type->types[1] = type_String;
meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187->type->names[1] = "sh";
  meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187->definitionModule = modulename;
  meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187->definitionLine = 78;
  struct UserObject *uo220 = (struct UserObject*)obj65;
  uo220->data[5] = emptyclosure;
  Method *meth_meth_PackageManager_flag_40__41_do220 = addmethod2pos(obj65, "flag()do", &meth_PackageManager_flag_40__41_do220, 5);
int argcv_meth_PackageManager_flag_40__41_do220[] = {1, 1};
meth_meth_PackageManager_flag_40__41_do220->type = alloc_MethodType(2, argcv_meth_PackageManager_flag_40__41_do220);
meth_meth_PackageManager_flag_40__41_do220->type->types[0] = type_String;
meth_meth_PackageManager_flag_40__41_do220->type->names[0] = "name";
  meth_meth_PackageManager_flag_40__41_do220->definitionModule = modulename;
  meth_meth_PackageManager_flag_40__41_do220->definitionLine = 86;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
objclass65 = obj65->class;
  objclass65->definitionModule = modulename;
  objclass65->definitionLine = 45;
  (*(struct UserObject *)self).data[0] = lowerouter65;
  self = oldself65;
  selfslot = oldselfslot65;
  stackframe = oldstackframe65;
// compilenode returning obj65
  *var_on = obj65;
  if (obj65 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 101
  setline(101);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe224 = gc_frame_new();
// Begin line 90
  setline(90);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block225 = alloc_Block(NULL, NULL, "PackageManager", 90);
  gc_frame_newslot(block225);
  block_savedest(block225);
  Object closure226 = createclosure(3, "_apply");
setclosureframe(closure226, stackframe);
  addtoclosure(closure226, var_i);
  addtoclosure(closure226, var_size);
  addtoclosure(closure226, selfslot);
  struct UserObject *uo226 = (struct UserObject*)block225;
  uo226->data[0] = (Object)closure226;
  Method *meth_meth_PackageManager__apply226 = addmethod2pos(block225, "_apply", &meth_PackageManager__apply226, 0);
int argcv_meth_PackageManager__apply226[] = {0};
meth_meth_PackageManager__apply226->type = alloc_MethodType(1, argcv_meth_PackageManager__apply226);
  meth_meth_PackageManager__apply226->definitionModule = modulename;
  meth_meth_PackageManager__apply226->definitionLine = 81;
// compilenode returning block225
  gc_frame_newslot(block225);
// Begin line 101
  setline(101);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block231 = alloc_Block(NULL, NULL, "PackageManager", 101);
  gc_frame_newslot(block231);
  block_savedest(block231);
  Object closure232 = createclosure(6, "_apply");
setclosureframe(closure232, stackframe);
  addtoclosure(closure232, var_i);
  addtoclosure(closure232, var_args);
  addtoclosure(closure232, var_ran);
  addtoclosure(closure232, var_on);
  addtoclosure(closure232, var_block);
  addtoclosure(closure232, selfslot);
  struct UserObject *uo232 = (struct UserObject*)block231;
  uo232->data[0] = (Object)closure232;
  Method *meth_meth_PackageManager__apply232 = addmethod2pos(block231, "_apply", &meth_PackageManager__apply232, 0);
int argcv_meth_PackageManager__apply232[] = {0};
meth_meth_PackageManager__apply232->type = alloc_MethodType(1, argcv_meth_PackageManager__apply232);
  meth_meth_PackageManager__apply232->definitionModule = modulename;
  meth_meth_PackageManager__apply232->definitionLine = 81;
// compilenode returning block231
  gc_frame_newslot(block231);
  params[0] = block225;
  params[1] = block231;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call271 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe224);
// compilenode returning call271
  gc_frame_end(frame);
  return call271;
}
Object meth_PackageManager_forArgs_40__41_do60_object(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct StackFrameObject *stackframe = alloc_StackFrame(7, NULL);
  pushclosure(NULL);
  pushstackframe(stackframe, "forArgs()do()object");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = args[0 + argcv[0] + argcv[1]];
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  if (nparts > 0 && argcv[1] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 2 && args)
    gracedie("missing argument list for forArgs()do (probably reflection error): got %i lists, expected 2.", nparts);
  Object *var_args = &(stackframe->slots[0]);
  *var_args = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for forArgs");
  Object *var_block = &(stackframe->slots[1]);
  *var_block = args[curarg];
  curarg++;
if (argcv && argcv[1] > 1)
  gracedie("too many arguments for do");
  Object *selfslot = &(stackframe->slots[2]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_i = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "i");
  Object *var_ran = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "ran");
  Object *var_size = &(stackframe->slots[5]);
  setframeelementname(stackframe, 5, "size");
  Object *var_on = &(stackframe->slots[6]);
  setframeelementname(stackframe, 6, "on");
// Begin line 42
  setline(42);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 41
  setline(41);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num61 = alloc_Float64(2);
// compilenode returning num61
  *var_i = num61;
  if (num61 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 43
  setline(43);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 42
  setline(42);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool62 = alloc_Boolean(0);
// compilenode returning bool62
  *var_ran = bool62;
  if (bool62 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 45
  setline(45);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 43
  setline(43);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe63 = gc_frame_new();
// compilenode returning *var_args
  partcv[0] = 0;
  Object call64 = callmethod(*var_args, "size",
    1, partcv, params);
  gc_frame_end(callframe63);
// compilenode returning call64
// compilenode returning call64
  *var_size = call64;
  if (call64 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 45
  setline(45);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object inheritingObject65 = inheritingObject;
  if (isTailObject) {
    isTailObject = 0;
    inheritingObject = NULL;
  }
  Object obj65 = alloc_userobj2(5,6, objclass65);
  gc_frame_newslot(obj65);
// OBJECT OUTER DEC outer
  adddatum2(obj65, self, 0);
  addmethodreal(obj65,"outer", &reader_PackageManager_outer_66);
  Object oldself65 = self;
  struct StackFrameObject *oldstackframe65 = stackframe;
  stackframe = alloc_StackFrame(1, oldstackframe65);
  gc_frame_newslot((Object)stackframe);
  self = obj65;
  Object *oldselfslot65 = selfslot;
  selfslot = &stackframe->slots[0];
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  Object thisouter65 = (*(struct UserObject *)self).data[0], lowerouter65 = thisouter65;
  if (inheritingObject65) {
    struct UserObject *inho65 = (struct UserObject *)inheritingObject65;
    while (inho65->super != GraceDefaultObject) inho65 = (struct UserObject *)inho65->super;
    inho65->super = obj65;
    self = inheritingObject65;
    *selfslot = self;
    lowerouter65 = (*(struct UserObject *)self).data[0];
    (*(struct UserObject *)self).data[0] = thisouter65;
  }
  block_savedest(obj65);
  Object closure67 = createclosure(3, "option()shortHand()do");
setclosureframe(closure67, stackframe);
  addtoclosure(closure67, var_i);
  addtoclosure(closure67, var_args);
  addtoclosure(closure67, var_ran);
  struct UserObject *uo67 = (struct UserObject*)obj65;
  uo67->data[1] = (Object)closure67;
  Method *meth_meth_PackageManager_option_40__41_shortHand_40__41_do67 = addmethod2pos(obj65, "option()shortHand()do", &meth_PackageManager_option_40__41_shortHand_40__41_do67, 1);
int argcv_meth_PackageManager_option_40__41_shortHand_40__41_do67[] = {1, 1, 1};
meth_meth_PackageManager_option_40__41_shortHand_40__41_do67->type = alloc_MethodType(3, argcv_meth_PackageManager_option_40__41_shortHand_40__41_do67);
meth_meth_PackageManager_option_40__41_shortHand_40__41_do67->type->types[0] = type_String;
meth_meth_PackageManager_option_40__41_shortHand_40__41_do67->type->names[0] = "name";
meth_meth_PackageManager_option_40__41_shortHand_40__41_do67->type->types[1] = type_String;
meth_meth_PackageManager_option_40__41_shortHand_40__41_do67->type->names[1] = "sh";
  meth_meth_PackageManager_option_40__41_shortHand_40__41_do67->definitionModule = modulename;
  meth_meth_PackageManager_option_40__41_shortHand_40__41_do67->definitionLine = 46;
  block_savedest(obj65);
  Object closure125 = createclosure(3, "doubleValue()do");
setclosureframe(closure125, stackframe);
  addtoclosure(closure125, var_i);
  addtoclosure(closure125, var_args);
  addtoclosure(closure125, var_ran);
  struct UserObject *uo125 = (struct UserObject*)obj65;
  uo125->data[2] = (Object)closure125;
  Method *meth_meth_PackageManager_doubleValue_40__41_do125 = addmethod2pos(obj65, "doubleValue()do", &meth_PackageManager_doubleValue_40__41_do125, 2);
int argcv_meth_PackageManager_doubleValue_40__41_do125[] = {1, 1};
meth_meth_PackageManager_doubleValue_40__41_do125->type = alloc_MethodType(2, argcv_meth_PackageManager_doubleValue_40__41_do125);
meth_meth_PackageManager_doubleValue_40__41_do125->type->types[0] = type_String;
meth_meth_PackageManager_doubleValue_40__41_do125->type->names[0] = "name";
  meth_meth_PackageManager_doubleValue_40__41_do125->definitionModule = modulename;
  meth_meth_PackageManager_doubleValue_40__41_do125->definitionLine = 59;
  struct UserObject *uo183 = (struct UserObject*)obj65;
  uo183->data[3] = emptyclosure;
  Method *meth_meth_PackageManager_option_40__41_do183 = addmethod2pos(obj65, "option()do", &meth_PackageManager_option_40__41_do183, 3);
int argcv_meth_PackageManager_option_40__41_do183[] = {1, 1};
meth_meth_PackageManager_option_40__41_do183->type = alloc_MethodType(2, argcv_meth_PackageManager_option_40__41_do183);
meth_meth_PackageManager_option_40__41_do183->type->types[0] = type_String;
meth_meth_PackageManager_option_40__41_do183->type->names[0] = "name";
  meth_meth_PackageManager_option_40__41_do183->definitionModule = modulename;
  meth_meth_PackageManager_option_40__41_do183->definitionLine = 74;
  block_savedest(obj65);
  Object closure187 = createclosure(3, "flag()shortHand()do");
setclosureframe(closure187, stackframe);
  addtoclosure(closure187, var_i);
  addtoclosure(closure187, var_args);
  addtoclosure(closure187, var_ran);
  struct UserObject *uo187 = (struct UserObject*)obj65;
  uo187->data[4] = (Object)closure187;
  Method *meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187 = addmethod2pos(obj65, "flag()shortHand()do", &meth_PackageManager_flag_40__41_shortHand_40__41_do187, 4);
int argcv_meth_PackageManager_flag_40__41_shortHand_40__41_do187[] = {1, 1, 1};
meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187->type = alloc_MethodType(3, argcv_meth_PackageManager_flag_40__41_shortHand_40__41_do187);
meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187->type->types[0] = type_String;
meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187->type->names[0] = "name";
meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187->type->types[1] = type_String;
meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187->type->names[1] = "sh";
  meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187->definitionModule = modulename;
  meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187->definitionLine = 78;
  struct UserObject *uo220 = (struct UserObject*)obj65;
  uo220->data[5] = emptyclosure;
  Method *meth_meth_PackageManager_flag_40__41_do220 = addmethod2pos(obj65, "flag()do", &meth_PackageManager_flag_40__41_do220, 5);
int argcv_meth_PackageManager_flag_40__41_do220[] = {1, 1};
meth_meth_PackageManager_flag_40__41_do220->type = alloc_MethodType(2, argcv_meth_PackageManager_flag_40__41_do220);
meth_meth_PackageManager_flag_40__41_do220->type->types[0] = type_String;
meth_meth_PackageManager_flag_40__41_do220->type->names[0] = "name";
  meth_meth_PackageManager_flag_40__41_do220->definitionModule = modulename;
  meth_meth_PackageManager_flag_40__41_do220->definitionLine = 86;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
objclass65 = obj65->class;
  objclass65->definitionModule = modulename;
  objclass65->definitionLine = 45;
  (*(struct UserObject *)self).data[0] = lowerouter65;
  self = oldself65;
  selfslot = oldselfslot65;
  stackframe = oldstackframe65;
// compilenode returning obj65
  *var_on = obj65;
  if (obj65 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 101
  setline(101);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe224 = gc_frame_new();
// Begin line 90
  setline(90);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block225 = alloc_Block(NULL, NULL, "PackageManager", 90);
  gc_frame_newslot(block225);
  block_savedest(block225);
  Object closure226 = createclosure(3, "_apply");
setclosureframe(closure226, stackframe);
  addtoclosure(closure226, var_i);
  addtoclosure(closure226, var_size);
  addtoclosure(closure226, selfslot);
  struct UserObject *uo226 = (struct UserObject*)block225;
  uo226->data[0] = (Object)closure226;
  Method *meth_meth_PackageManager__apply226 = addmethod2pos(block225, "_apply", &meth_PackageManager__apply226, 0);
int argcv_meth_PackageManager__apply226[] = {0};
meth_meth_PackageManager__apply226->type = alloc_MethodType(1, argcv_meth_PackageManager__apply226);
  meth_meth_PackageManager__apply226->definitionModule = modulename;
  meth_meth_PackageManager__apply226->definitionLine = 81;
// compilenode returning block225
  gc_frame_newslot(block225);
// Begin line 101
  setline(101);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block231 = alloc_Block(NULL, NULL, "PackageManager", 101);
  gc_frame_newslot(block231);
  block_savedest(block231);
  Object closure232 = createclosure(6, "_apply");
setclosureframe(closure232, stackframe);
  addtoclosure(closure232, var_i);
  addtoclosure(closure232, var_args);
  addtoclosure(closure232, var_ran);
  addtoclosure(closure232, var_on);
  addtoclosure(closure232, var_block);
  addtoclosure(closure232, selfslot);
  struct UserObject *uo232 = (struct UserObject*)block231;
  uo232->data[0] = (Object)closure232;
  Method *meth_meth_PackageManager__apply232 = addmethod2pos(block231, "_apply", &meth_PackageManager__apply232, 0);
int argcv_meth_PackageManager__apply232[] = {0};
meth_meth_PackageManager__apply232->type = alloc_MethodType(1, argcv_meth_PackageManager__apply232);
  meth_meth_PackageManager__apply232->definitionModule = modulename;
  meth_meth_PackageManager__apply232->definitionLine = 81;
// compilenode returning block231
  gc_frame_newslot(block231);
  params[0] = block225;
  params[1] = block231;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call271 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe224);
// compilenode returning call271
  gc_frame_end(frame);
  return call271;
}
Object meth_PackageManager_listInstalled273(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 3, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(3, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "listInstalled");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[2];
  int partcv[1];
  Object *var_io = getfromclosure(closure, 0);
  Object *var_sys = getfromclosure(closure, 1);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for listInstalled (probably reflection error): got %i lists, expected 1.", nparts);
if (argcv && argcv[0] > 0)
  gracedie("too many arguments for listInstalled");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_globalDir = &(stackframe->slots[1]);
  setframeelementname(stackframe, 1, "globalDir");
  Object *var_usrDir = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "usrDir");
// Begin line 105
  setline(105);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 104
  setline(104);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit274 == NULL) {
    strlit274 = alloc_String("Installed packages:");
    gc_root(strlit274);
  }
// compilenode returning strlit274
  params[0] = strlit274;
  Object call275 = gracelib_print(NULL, 1,  params);
// compilenode returning call275
// Begin line 110
  setline(110);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 105
  setline(105);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe277 = gc_frame_new();
  if (strlit278 == NULL) {
    strlit278 = alloc_String("usr/lib/grace/packages/");
    gc_root(strlit278);
  }
// compilenode returning strlit278
  gc_frame_newslot(strlit278);
// compilenode returning *var_io
  params[0] = strlit278;
  partcv[0] = 1;
  Object call279 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe277);
// compilenode returning call279
  Object if276 = done;
  if (istrue(call279)) {
// Begin line 106
  setline(106);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit280 == NULL) {
    strlit280 = alloc_String("Going into the first call");
    gc_root(strlit280);
  }
// compilenode returning strlit280
  params[0] = strlit280;
  Object call281 = gracelib_print(NULL, 1,  params);
// compilenode returning call281
// Begin line 108
  setline(108);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 107
  setline(107);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit282 == NULL) {
    strlit282 = alloc_String("usr/lib/grace/packages");
    gc_root(strlit282);
  }
// compilenode returning strlit282
  *var_globalDir = strlit282;
  if (strlit282 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 109
  setline(109);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe283 = gc_frame_new();
// Begin line 108
  setline(108);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_globalDir
  gc_frame_newslot(*var_globalDir);
  if (strlit284 == NULL) {
    strlit284 = alloc_String("");
    gc_root(strlit284);
  }
// compilenode returning strlit284
  gc_frame_newslot(strlit284);
  params[0] = *var_globalDir;
  params[1] = strlit284;
  partcv[0] = 2;
  Object call285 = callmethodflags(self, "recurseDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe283);
// compilenode returning call285
    gc_frame_newslot(call285);
    if276 = call285;
  } else {
  }
// compilenode returning if276
// Begin line 114
  setline(114);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 110
  setline(110);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe287 = gc_frame_new();
  int callframe289 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call290 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe289);
// compilenode returning call290
// compilenode returning call290
  if (strlit291 == NULL) {
    strlit291 = alloc_String("HOME");
    gc_root(strlit291);
  }
// compilenode returning strlit291
  params[0] = strlit291;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres292 = callmethod(call290, "[]", 1, partcv, params);
// compilenode returning idxres292
  int op_slot_left_288 = gc_frame_newslot(idxres292);
  if (strlit293 == NULL) {
    strlit293 = alloc_String("/grace/packages/");
    gc_root(strlit293);
  }
// compilenode returning strlit293
  int op_slot_right_288 = gc_frame_newslot(strlit293);
  params[0] = strlit293;
  partcv[0] = 1;
  Object opresult295 = callmethod(idxres292, "++", 1, partcv, params);
// compilenode returning opresult295
  gc_frame_newslot(opresult295);
// compilenode returning *var_io
  params[0] = opresult295;
  partcv[0] = 1;
  Object call296 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe287);
// compilenode returning call296
  Object if286 = done;
  if (istrue(call296)) {
// Begin line 112
  setline(112);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 111
  setline(111);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe298 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call299 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe298);
// compilenode returning call299
// compilenode returning call299
  if (strlit300 == NULL) {
    strlit300 = alloc_String("HOME");
    gc_root(strlit300);
  }
// compilenode returning strlit300
  params[0] = strlit300;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres301 = callmethod(call299, "[]", 1, partcv, params);
// compilenode returning idxres301
  int op_slot_left_297 = gc_frame_newslot(idxres301);
  if (strlit302 == NULL) {
    strlit302 = alloc_String("/grace/packages");
    gc_root(strlit302);
  }
// compilenode returning strlit302
  int op_slot_right_297 = gc_frame_newslot(strlit302);
  params[0] = strlit302;
  partcv[0] = 1;
  Object opresult304 = callmethod(idxres301, "++", 1, partcv, params);
// compilenode returning opresult304
  *var_usrDir = opresult304;
  if (opresult304 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 113
  setline(113);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe305 = gc_frame_new();
// Begin line 112
  setline(112);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_usrDir
  gc_frame_newslot(*var_usrDir);
  if (strlit306 == NULL) {
    strlit306 = alloc_String(" ");
    gc_root(strlit306);
  }
// compilenode returning strlit306
  gc_frame_newslot(strlit306);
  params[0] = *var_usrDir;
  params[1] = strlit306;
  partcv[0] = 2;
  Object call307 = callmethodflags(self, "recurseDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe305);
// compilenode returning call307
    gc_frame_newslot(call307);
    if286 = call307;
  } else {
  }
// compilenode returning if286
  gc_frame_end(frame);
  return if286;
}
Object meth_PackageManager__apply313(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[1];
  Object *var_padding = getfromclosure(closure, 0);
  Object *var_path = getfromclosure(closure, 1);
  Object *var_io = getfromclosure(closure, 2);
  Object self = *(getfromclosure(closure, 3));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *var_p = &(stackframe->slots[0]);
  *var_p = args[curarg];
  curarg++;
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 125
  setline(125);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 119
  setline(119);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_p
  int op_slot_left_316 = gc_frame_newslot(*var_p);
  if (strlit317 == NULL) {
    strlit317 = alloc_String("..");
    gc_root(strlit317);
  }
// compilenode returning strlit317
  int op_slot_right_316 = gc_frame_newslot(strlit317);
  params[0] = strlit317;
  partcv[0] = 1;
  Object opresult319 = callmethod(*var_p, "!=", 1, partcv, params);
// compilenode returning opresult319
  int op_slot_left_315 = gc_frame_newslot(opresult319);
// compilenode returning *var_p
  int op_slot_left_320 = gc_frame_newslot(*var_p);
  if (strlit321 == NULL) {
    strlit321 = alloc_String(".");
    gc_root(strlit321);
  }
// compilenode returning strlit321
  int op_slot_right_320 = gc_frame_newslot(strlit321);
  params[0] = strlit321;
  partcv[0] = 1;
  Object opresult323 = callmethod(*var_p, "!=", 1, partcv, params);
// compilenode returning opresult323
  int op_slot_right_315 = gc_frame_newslot(opresult323);
  params[0] = opresult323;
  partcv[0] = 1;
  Object opresult325 = callmethod(opresult319, "&&", 1, partcv, params);
// compilenode returning opresult325
  Object if314 = done;
  if (istrue(opresult325)) {
// Begin line 120
  setline(120);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_padding
  int op_slot_left_326 = gc_frame_newslot(*var_padding);
// compilenode returning *var_p
  int op_slot_right_326 = gc_frame_newslot(*var_p);
  params[0] = *var_p;
  partcv[0] = 1;
  Object opresult328 = callmethod(*var_padding, "++", 1, partcv, params);
// compilenode returning opresult328
  params[0] = opresult328;
  Object call329 = gracelib_print(NULL, 1,  params);
// compilenode returning call329
// Begin line 124
  setline(124);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 121
  setline(121);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe332 = gc_frame_new();
  int callframe333 = gc_frame_new();
// compilenode returning *var_path
  int op_slot_left_335 = gc_frame_newslot(*var_path);
  if (strlit336 == NULL) {
    strlit336 = alloc_String("/");
    gc_root(strlit336);
  }
// compilenode returning strlit336
  int op_slot_right_335 = gc_frame_newslot(strlit336);
  params[0] = strlit336;
  partcv[0] = 1;
  Object opresult338 = callmethod(*var_path, "++", 1, partcv, params);
// compilenode returning opresult338
  int op_slot_left_334 = gc_frame_newslot(opresult338);
// compilenode returning *var_p
  int op_slot_right_334 = gc_frame_newslot(*var_p);
  params[0] = *var_p;
  partcv[0] = 1;
  Object opresult340 = callmethod(opresult338, "++", 1, partcv, params);
// compilenode returning opresult340
  gc_frame_newslot(opresult340);
// compilenode returning *var_io
  params[0] = opresult340;
  partcv[0] = 1;
  Object call341 = callmethod(*var_io, "getdir",
    1, partcv, params);
  gc_frame_end(callframe333);
// compilenode returning call341
  partcv[0] = 0;
  Object call342 = callmethod(call341, "size",
    1, partcv, params);
  gc_frame_end(callframe332);
// compilenode returning call342
// compilenode returning call342
  int op_slot_left_331 = gc_frame_newslot(call342);
  Object num343 = alloc_Float64(0);
// compilenode returning num343
  int op_slot_right_331 = gc_frame_newslot(num343);
  params[0] = num343;
  partcv[0] = 1;
  Object opresult345 = callmethod(call342, ">", 1, partcv, params);
// compilenode returning opresult345
  Object if330 = done;
  if (istrue(opresult345)) {
// Begin line 123
  setline(123);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe346 = gc_frame_new();
// Begin line 122
  setline(122);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_path
  int op_slot_left_348 = gc_frame_newslot(*var_path);
  if (strlit349 == NULL) {
    strlit349 = alloc_String("/");
    gc_root(strlit349);
  }
// compilenode returning strlit349
  int op_slot_right_348 = gc_frame_newslot(strlit349);
  params[0] = strlit349;
  partcv[0] = 1;
  Object opresult351 = callmethod(*var_path, "++", 1, partcv, params);
// compilenode returning opresult351
  int op_slot_left_347 = gc_frame_newslot(opresult351);
// compilenode returning *var_p
  int op_slot_right_347 = gc_frame_newslot(*var_p);
  params[0] = *var_p;
  partcv[0] = 1;
  Object opresult353 = callmethod(opresult351, "++", 1, partcv, params);
// compilenode returning opresult353
  gc_frame_newslot(opresult353);
// compilenode returning *var_padding
  int op_slot_left_354 = gc_frame_newslot(*var_padding);
  if (strlit355 == NULL) {
    strlit355 = alloc_String("   ");
    gc_root(strlit355);
  }
// compilenode returning strlit355
  int op_slot_right_354 = gc_frame_newslot(strlit355);
  params[0] = strlit355;
  partcv[0] = 1;
  Object opresult357 = callmethod(*var_padding, "++", 1, partcv, params);
// compilenode returning opresult357
  gc_frame_newslot(opresult357);
  params[0] = opresult353;
  params[1] = opresult357;
  partcv[0] = 2;
  Object call358 = callmethodflags(self, "recurseDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe346);
// compilenode returning call358
    gc_frame_newslot(call358);
    if330 = call358;
  } else {
  }
// compilenode returning if330
    gc_frame_newslot(if330);
    if314 = if330;
  } else {
  }
// compilenode returning if314
  gc_frame_end(frame);
  return if314;
}
Object meth_PackageManager_recurseDirectory308(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 4, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(4, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "recurseDirectory");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 2)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  Object *var_io = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for recurseDirectory (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_path = &(stackframe->slots[0]);
  *var_path = args[curarg];
  curarg++;
  Object *var_padding = &(stackframe->slots[1]);
  *var_padding = args[curarg];
  curarg++;
if (argcv && argcv[0] > 2)
  gracedie("too many arguments for recurseDirectory");
  Object *selfslot = &(stackframe->slots[2]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_pathContents = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "pathContents");
// Begin line 117
  setline(117);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe309 = gc_frame_new();
// compilenode returning *var_path
  gc_frame_newslot(*var_path);
// compilenode returning *var_io
  params[0] = *var_path;
  partcv[0] = 1;
  Object call310 = callmethod(*var_io, "getdir",
    1, partcv, params);
  gc_frame_end(callframe309);
// compilenode returning call310
  *var_pathContents = call310;
  if (call310 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 126
  setline(126);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe311 = gc_frame_new();
// Begin line 118
  setline(118);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pathContents
  gc_frame_newslot(*var_pathContents);
// Begin line 126
  setline(126);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block312 = alloc_Block(NULL, NULL, "PackageManager", 126);
  gc_frame_newslot(block312);
  block_savedest(block312);
  Object closure313 = createclosure(4, "_apply");
setclosureframe(closure313, stackframe);
  addtoclosure(closure313, var_padding);
  addtoclosure(closure313, var_path);
  addtoclosure(closure313, var_io);
  addtoclosure(closure313, selfslot);
  struct UserObject *uo313 = (struct UserObject*)block312;
  uo313->data[0] = (Object)closure313;
  Method *meth_meth_PackageManager__apply313 = addmethod2pos(block312, "_apply", &meth_PackageManager__apply313, 0);
int argcv_meth_PackageManager__apply313[] = {1};
meth_meth_PackageManager__apply313->type = alloc_MethodType(1, argcv_meth_PackageManager__apply313);
  meth_meth_PackageManager__apply313->definitionModule = modulename;
  meth_meth_PackageManager__apply313->definitionLine = 111;
// compilenode returning block312
  gc_frame_newslot(block312);
  params[0] = *var_pathContents;
  params[1] = block312;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call360 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe311);
// compilenode returning call360
  gc_frame_end(frame);
  return call360;
}
Object meth_PackageManager__apply368(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object self = *(getfromclosure(closure, 0));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *var_im = &(stackframe->slots[0]);
  *var_im = args[curarg];
  curarg++;
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 132
  setline(132);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe369 = gc_frame_new();
// compilenode returning *var_im
  gc_frame_newslot(*var_im);
  params[0] = *var_im;
  partcv[0] = 1;
  Object call370 = callmethodflags(self, "write", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe369);
// compilenode returning call370
  gc_frame_end(frame);
  return call370;
}
Object meth_PackageManager__apply375(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object self = *(getfromclosure(closure, 0));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *var_im = &(stackframe->slots[0]);
  *var_im = args[curarg];
  curarg++;
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 135
  setline(135);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe376 = gc_frame_new();
// compilenode returning *var_im
  gc_frame_newslot(*var_im);
  params[0] = *var_im;
  partcv[0] = 1;
  Object call377 = callmethodflags(self, "compile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe376);
// compilenode returning call377
  gc_frame_end(frame);
  return call377;
}
Object meth_PackageManager_doGet361(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 5, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "doGet");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  Object *var_imported = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for doGet (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_impAddress = &(stackframe->slots[0]);
  *var_impAddress = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for doGet");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 130
  setline(130);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 129
  setline(129);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array362 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array362
  *var_imported = array362;
  if (array362 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 131
  setline(131);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe364 = gc_frame_new();
// Begin line 130
  setline(130);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_impAddress
  gc_frame_newslot(*var_impAddress);
  params[0] = *var_impAddress;
  partcv[0] = 1;
  Object call365 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe364);
// compilenode returning call365
// Begin line 134
  setline(134);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe366 = gc_frame_new();
// Begin line 131
  setline(131);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_imported
  gc_frame_newslot(*var_imported);
// Begin line 134
  setline(134);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block367 = alloc_Block(NULL, NULL, "PackageManager", 134);
  gc_frame_newslot(block367);
  block_savedest(block367);
  Object closure368 = createclosure(1, "_apply");
setclosureframe(closure368, stackframe);
  addtoclosure(closure368, selfslot);
  struct UserObject *uo368 = (struct UserObject*)block367;
  uo368->data[0] = (Object)closure368;
  Method *meth_meth_PackageManager__apply368 = addmethod2pos(block367, "_apply", &meth_PackageManager__apply368, 0);
int argcv_meth_PackageManager__apply368[] = {1};
meth_meth_PackageManager__apply368->type = alloc_MethodType(1, argcv_meth_PackageManager__apply368);
  meth_meth_PackageManager__apply368->definitionModule = modulename;
  meth_meth_PackageManager__apply368->definitionLine = 121;
// compilenode returning block367
  gc_frame_newslot(block367);
  params[0] = *var_imported;
  params[1] = block367;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call372 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe366);
// compilenode returning call372
// Begin line 137
  setline(137);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe373 = gc_frame_new();
// Begin line 134
  setline(134);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_imported
  gc_frame_newslot(*var_imported);
// Begin line 137
  setline(137);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block374 = alloc_Block(NULL, NULL, "PackageManager", 137);
  gc_frame_newslot(block374);
  block_savedest(block374);
  Object closure375 = createclosure(1, "_apply");
setclosureframe(closure375, stackframe);
  addtoclosure(closure375, selfslot);
  struct UserObject *uo375 = (struct UserObject*)block374;
  uo375->data[0] = (Object)closure375;
  Method *meth_meth_PackageManager__apply375 = addmethod2pos(block374, "_apply", &meth_PackageManager__apply375, 0);
int argcv_meth_PackageManager__apply375[] = {1};
meth_meth_PackageManager__apply375->type = alloc_MethodType(1, argcv_meth_PackageManager__apply375);
  meth_meth_PackageManager__apply375->definitionModule = modulename;
  meth_meth_PackageManager__apply375->definitionLine = 121;
// compilenode returning block374
  gc_frame_newslot(block374);
  params[0] = *var_imported;
  params[1] = block374;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call379 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe373);
// compilenode returning call379
  gc_frame_end(frame);
  return call379;
}
Object meth_PackageManager_setFile380(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 6, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(3, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "setFile");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  Object *var_curFile = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for setFile (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_fileAddress = &(stackframe->slots[0]);
  *var_fileAddress = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for setFile");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_file = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "file");
// Begin line 148
  setline(148);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 140
  setline(140);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe383 = gc_frame_new();
  int callframe385 = gc_frame_new();
// compilenode returning *var_fileAddress
  partcv[0] = 0;
  Object call386 = callmethod(*var_fileAddress, "size",
    1, partcv, params);
  gc_frame_end(callframe385);
// compilenode returning call386
// compilenode returning call386
  int op_slot_left_384 = gc_frame_newslot(call386);
  Object num387 = alloc_Float64(5);
// compilenode returning num387
  int op_slot_right_384 = gc_frame_newslot(num387);
  params[0] = num387;
  partcv[0] = 1;
  Object diff389 = callmethod(call386, "-", 1, partcv, params);
// compilenode returning diff389
  gc_frame_newslot(diff389);
  int callframe390 = gc_frame_new();
// compilenode returning *var_fileAddress
  partcv[0] = 0;
  Object call391 = callmethod(*var_fileAddress, "size",
    1, partcv, params);
  gc_frame_end(callframe390);
// compilenode returning call391
// compilenode returning call391
  gc_frame_newslot(call391);
// compilenode returning *var_fileAddress
  params[0] = diff389;
  params[1] = call391;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call392 = callmethod(*var_fileAddress, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe383);
// compilenode returning call392
  int op_slot_left_382 = gc_frame_newslot(call392);
  if (strlit393 == NULL) {
    strlit393 = alloc_String(".grace");
    gc_root(strlit393);
  }
// compilenode returning strlit393
  int op_slot_right_382 = gc_frame_newslot(strlit393);
  params[0] = strlit393;
  partcv[0] = 1;
  Object opresult395 = callmethod(call392, "==", 1, partcv, params);
// compilenode returning opresult395
  Object if381 = done;
  if (istrue(opresult395)) {
// Begin line 141
  setline(141);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object inheritingObject396 = inheritingObject;
  if (isTailObject) {
    isTailObject = 0;
    inheritingObject = NULL;
  }
  Object obj396 = alloc_userobj2(4,4, objclass396);
  gc_frame_newslot(obj396);
// OBJECT OUTER DEC outer
  adddatum2(obj396, self, 0);
  addmethodreal(obj396,"outer", &reader_PackageManager_outer_397);
  Object oldself396 = self;
  struct StackFrameObject *oldstackframe396 = stackframe;
  stackframe = alloc_StackFrame(1, oldstackframe396);
  gc_frame_newslot((Object)stackframe);
  self = obj396;
  Object *oldselfslot396 = selfslot;
  selfslot = &stackframe->slots[0];
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  Object thisouter396 = (*(struct UserObject *)self).data[0], lowerouter396 = thisouter396;
  if (inheritingObject396) {
    struct UserObject *inho396 = (struct UserObject *)inheritingObject396;
    while (inho396->super != GraceDefaultObject) inho396 = (struct UserObject *)inho396->super;
    inho396->super = obj396;
    self = inheritingObject396;
    *selfslot = self;
    lowerouter396 = (*(struct UserObject *)self).data[0];
    (*(struct UserObject *)self).data[0] = thisouter396;
  }
if (objclass396 == NULL) {
  Method *reader398 = addmethodrealflags(obj396, "address",&reader_PackageManager_address_398, 0);
  Method *writer398 = addmethodrealflags(obj396, "address:=",&writer_PackageManager_address_398, 0);
  reader398->definitionModule = modulename;
  writer398->definitionModule = modulename;
  reader398->definitionLine = 143;
  writer398->definitionLine = 143;
}
obj396->flags |= OFLAG_MUTABLE;
adddatum2(obj396, alloc_Undefined(), 1);
if (objclass396 == NULL) {
  Method *reader399 = addmethodrealflags(obj396, "data",&reader_PackageManager_data_399, 0);
  Method *writer399 = addmethodrealflags(obj396, "data:=",&writer_PackageManager_data_399, 0);
  reader399->definitionModule = modulename;
  writer399->definitionModule = modulename;
  reader399->definitionLine = 144;
  writer399->definitionLine = 144;
}
obj396->flags |= OFLAG_MUTABLE;
adddatum2(obj396, alloc_Undefined(), 2);
  sourceObject = obj396;
// Begin line 142
  setline(142);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_fileAddress
  adddatum2(obj396, *var_fileAddress, 1);
  sourceObject = obj396;
  adddatum2(obj396, undefined, 2);
objclass396 = obj396->class;
  objclass396->definitionModule = modulename;
  objclass396->definitionLine = 141;
  (*(struct UserObject *)self).data[0] = lowerouter396;
  self = oldself396;
  selfslot = oldselfslot396;
  stackframe = oldstackframe396;
// compilenode returning obj396
  *var_file = obj396;
  if (obj396 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 145
  setline(145);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_file
  *var_curFile = *var_file;
  if (*var_file == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 146
  setline(146);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool401 = alloc_Boolean(1);
// compilenode returning bool401
  return bool401;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if381 = undefined;
  } else {
  }
// compilenode returning if381
// Begin line 148
  setline(148);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool402 = alloc_Boolean(0);
// compilenode returning bool402
  return bool402;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply448(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_toProcess = getfromclosure(closure, 0);
  Object self = *(getfromclosure(closure, 1));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  int callframe450 = gc_frame_new();
// compilenode returning *var_toProcess
  partcv[0] = 0;
  Object call451 = callmethod(*var_toProcess, "size",
    1, partcv, params);
  gc_frame_end(callframe450);
// compilenode returning call451
// compilenode returning call451
  int op_slot_left_449 = gc_frame_newslot(call451);
  Object num452 = alloc_Float64(0);
// compilenode returning num452
  int op_slot_right_449 = gc_frame_newslot(num452);
  params[0] = num452;
  partcv[0] = 1;
  Object opresult454 = callmethod(call451, ">", 1, partcv, params);
// compilenode returning opresult454
  gc_frame_end(frame);
  return opresult454;
}
Object meth_PackageManager__apply457(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_toProcess = getfromclosure(closure, 0);
  Object self = *(getfromclosure(closure, 1));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 164
  setline(164);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe458 = gc_frame_new();
// Begin line 163
  setline(163);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe459 = gc_frame_new();
// compilenode returning *var_toProcess
  partcv[0] = 0;
  Object call460 = callmethod(*var_toProcess, "pop",
    1, partcv, params);
  gc_frame_end(callframe459);
// compilenode returning call460
// compilenode returning call460
  gc_frame_newslot(call460);
  params[0] = call460;
  partcv[0] = 1;
  Object call461 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe458);
// compilenode returning call461
  gc_frame_end(frame);
  return call461;
}
Object meth_PackageManager_fetchImports403(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 7, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "fetchImports");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  Object *var_curFile = getfromclosure(closure, 0);
  Object *var_imported = getfromclosure(closure, 1);
  Object *var_toProcess = getfromclosure(closure, 2);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for fetchImports (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_fileAddress = &(stackframe->slots[0]);
  *var_fileAddress = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for fetchImports");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 174
  setline(174);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 152
  setline(152);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe405 = gc_frame_new();
// compilenode returning *var_fileAddress
  gc_frame_newslot(*var_fileAddress);
  params[0] = *var_fileAddress;
  partcv[0] = 1;
  Object call406 = callmethodflags(self, "setFile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe405);
// compilenode returning call406
  Object if404 = done;
  if (istrue(call406)) {
// Begin line 154
  setline(154);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 153
  setline(153);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit409 == NULL) {
    strlit409 = alloc_String("File address ");
    gc_root(strlit409);
  }
// compilenode returning strlit409
  int op_slot_left_408 = gc_frame_newslot(strlit409);
  int callframe410 = gc_frame_new();
// compilenode returning *var_curFile
  partcv[0] = 0;
  Object call411 = callmethod(*var_curFile, "address",
    1, partcv, params);
  gc_frame_end(callframe410);
// compilenode returning call411
// compilenode returning call411
  int op_slot_right_408 = gc_frame_newslot(call411);
  params[0] = call411;
  partcv[0] = 1;
  Object opresult413 = callmethod(strlit409, "++", 1, partcv, params);
// compilenode returning opresult413
  int op_slot_left_407 = gc_frame_newslot(opresult413);
  if (strlit414 == NULL) {
    strlit414 = alloc_String("");
    gc_root(strlit414);
  }
// compilenode returning strlit414
  int op_slot_right_407 = gc_frame_newslot(strlit414);
  params[0] = strlit414;
  partcv[0] = 1;
  Object opresult416 = callmethod(opresult413, "++", 1, partcv, params);
// compilenode returning opresult416
  params[0] = opresult416;
  Object call417 = gracelib_print(NULL, 1,  params);
// compilenode returning call417
// Begin line 158
  setline(158);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 154
  setline(154);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe420 = gc_frame_new();
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call421 = callmethodflags(self, "setFileData", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe420);
// compilenode returning call421
  int op_slot_left_419 = gc_frame_newslot(call421);
  Object bool422 = alloc_Boolean(0);
// compilenode returning bool422
  int op_slot_right_419 = gc_frame_newslot(bool422);
  params[0] = bool422;
  partcv[0] = 1;
  Object opresult424 = callmethod(call421, "==", 1, partcv, params);
// compilenode returning opresult424
  Object if418 = done;
  if (istrue(opresult424)) {
// Begin line 156
  setline(156);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 155
  setline(155);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit425 == NULL) {
    strlit425 = alloc_String("Could not retrieve file data");
    gc_root(strlit425);
  }
// compilenode returning strlit425
  params[0] = strlit425;
  Object call426 = gracelib_print(NULL, 1,  params);
// compilenode returning call426
// Begin line 157
  setline(157);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 156
  setline(156);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool427 = alloc_Boolean(0);
// compilenode returning bool427
  return bool427;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if418 = undefined;
  } else {
  }
// compilenode returning if418
// Begin line 167
  setline(167);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 158
  setline(158);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe429 = gc_frame_new();
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call430 = callmethodflags(self, "validateFile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe429);
// compilenode returning call430
  Object if428 = done;
  if (istrue(call430)) {
// Begin line 160
  setline(160);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 159
  setline(159);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit433 == NULL) {
    strlit433 = alloc_String("Pushing ");
    gc_root(strlit433);
  }
// compilenode returning strlit433
  int op_slot_left_432 = gc_frame_newslot(strlit433);
  int callframe434 = gc_frame_new();
// compilenode returning *var_curFile
  partcv[0] = 0;
  Object call435 = callmethod(*var_curFile, "address",
    1, partcv, params);
  gc_frame_end(callframe434);
// compilenode returning call435
// compilenode returning call435
  int op_slot_right_432 = gc_frame_newslot(call435);
  params[0] = call435;
  partcv[0] = 1;
  Object opresult437 = callmethod(strlit433, "++", 1, partcv, params);
// compilenode returning opresult437
  int op_slot_left_431 = gc_frame_newslot(opresult437);
  if (strlit438 == NULL) {
    strlit438 = alloc_String(" to imported");
    gc_root(strlit438);
  }
// compilenode returning strlit438
  int op_slot_right_431 = gc_frame_newslot(strlit438);
  params[0] = strlit438;
  partcv[0] = 1;
  Object opresult440 = callmethod(opresult437, "++", 1, partcv, params);
// compilenode returning opresult440
  params[0] = opresult440;
  Object call441 = gracelib_print(NULL, 1,  params);
// compilenode returning call441
// Begin line 161
  setline(161);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe442 = gc_frame_new();
// Begin line 160
  setline(160);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
// compilenode returning *var_imported
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call443 = callmethod(*var_imported, "push",
    1, partcv, params);
  gc_frame_end(callframe442);
// compilenode returning call443
// Begin line 162
  setline(162);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe444 = gc_frame_new();
// Begin line 161
  setline(161);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call445 = callmethodflags(self, "parseFile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe444);
// compilenode returning call445
// Begin line 165
  setline(165);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe446 = gc_frame_new();
// Begin line 162
  setline(162);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block447 = alloc_Block(NULL, NULL, "PackageManager", 162);
  gc_frame_newslot(block447);
  block_savedest(block447);
  Object closure448 = createclosure(2, "_apply");
setclosureframe(closure448, stackframe);
  addtoclosure(closure448, var_toProcess);
  addtoclosure(closure448, selfslot);
  struct UserObject *uo448 = (struct UserObject*)block447;
  uo448->data[0] = (Object)closure448;
  Method *meth_meth_PackageManager__apply448 = addmethod2pos(block447, "_apply", &meth_PackageManager__apply448, 0);
int argcv_meth_PackageManager__apply448[] = {0};
meth_meth_PackageManager__apply448->type = alloc_MethodType(1, argcv_meth_PackageManager__apply448);
  meth_meth_PackageManager__apply448->definitionModule = modulename;
  meth_meth_PackageManager__apply448->definitionLine = 159;
// compilenode returning block447
  gc_frame_newslot(block447);
// Begin line 165
  setline(165);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block456 = alloc_Block(NULL, NULL, "PackageManager", 165);
  gc_frame_newslot(block456);
  block_savedest(block456);
  Object closure457 = createclosure(2, "_apply");
setclosureframe(closure457, stackframe);
  addtoclosure(closure457, var_toProcess);
  addtoclosure(closure457, selfslot);
  struct UserObject *uo457 = (struct UserObject*)block456;
  uo457->data[0] = (Object)closure457;
  Method *meth_meth_PackageManager__apply457 = addmethod2pos(block456, "_apply", &meth_PackageManager__apply457, 0);
int argcv_meth_PackageManager__apply457[] = {0};
meth_meth_PackageManager__apply457->type = alloc_MethodType(1, argcv_meth_PackageManager__apply457);
  meth_meth_PackageManager__apply457->definitionModule = modulename;
  meth_meth_PackageManager__apply457->definitionLine = 162;
// compilenode returning block456
  gc_frame_newslot(block456);
  params[0] = block447;
  params[1] = block456;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call463 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe446);
// compilenode returning call463
// Begin line 166
  setline(166);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 165
  setline(165);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool464 = alloc_Boolean(1);
// compilenode returning bool464
  return bool464;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if428 = undefined;
  } else {
  }
// compilenode returning if428
    gc_frame_newslot(if428);
    if404 = if428;
  } else {
// Begin line 173
  setline(173);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 169
  setline(169);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe467 = gc_frame_new();
// compilenode returning *var_fileAddress
  int op_slot_left_468 = gc_frame_newslot(*var_fileAddress);
  if (strlit469 == NULL) {
    strlit469 = alloc_String(".grace");
    gc_root(strlit469);
  }
// compilenode returning strlit469
  int op_slot_right_468 = gc_frame_newslot(strlit469);
  params[0] = strlit469;
  partcv[0] = 1;
  Object opresult471 = callmethod(*var_fileAddress, "++", 1, partcv, params);
// compilenode returning opresult471
  gc_frame_newslot(opresult471);
  params[0] = opresult471;
  partcv[0] = 1;
  Object call472 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe467);
// compilenode returning call472
  int op_slot_left_466 = gc_frame_newslot(call472);
  Object bool473 = alloc_Boolean(0);
// compilenode returning bool473
  int op_slot_right_466 = gc_frame_newslot(bool473);
  params[0] = bool473;
  partcv[0] = 1;
  Object opresult475 = callmethod(call472, "==", 1, partcv, params);
// compilenode returning opresult475
  Object if465 = done;
  if (istrue(opresult475)) {
// Begin line 170
  setline(170);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit476 == NULL) {
    strlit476 = alloc_String("Could not locate file. Check file address.");
    gc_root(strlit476);
  }
// compilenode returning strlit476
  params[0] = strlit476;
  Object call477 = gracelib_print(NULL, 1,  params);
// compilenode returning call477
// Begin line 172
  setline(172);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 171
  setline(171);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool478 = alloc_Boolean(0);
// compilenode returning bool478
  return bool478;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if465 = undefined;
  } else {
  }
// compilenode returning if465
    gc_frame_newslot(if465);
    if404 = if465;
  }
// compilenode returning if404
  gc_frame_end(frame);
  return if404;
}
Object meth_PackageManager__apply533(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_file = getfromclosure(closure, 0);
  Object self = *(getfromclosure(closure, 1));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *var_d = &(stackframe->slots[0]);
  *var_d = args[curarg];
  curarg++;
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 192
  setline(192);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 191
  setline(191);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit534 == NULL) {
    strlit534 = alloc_String(" Response received");
    gc_root(strlit534);
  }
// compilenode returning strlit534
  params[0] = strlit534;
  Object call535 = gracelib_print(NULL, 1,  params);
// compilenode returning call535
// Begin line 193
  setline(193);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 188
  setline(188);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe536 = gc_frame_new();
// Begin line 193
  setline(193);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe537 = gc_frame_new();
// Begin line 192
  setline(192);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit538 == NULL) {
    strlit538 = alloc_String("utf-8");
    gc_root(strlit538);
  }
// compilenode returning strlit538
  gc_frame_newslot(strlit538);
// compilenode returning *var_d
  params[0] = strlit538;
  partcv[0] = 1;
  Object call539 = callmethod(*var_d, "decode",
    1, partcv, params);
  gc_frame_end(callframe537);
// compilenode returning call539
  gc_frame_newslot(call539);
// compilenode returning *var_file
  params[0] = call539;
  partcv[0] = 1;
  Object call540 = callmethod(*var_file, "data:=",
    1, partcv, params);
  gc_frame_end(callframe536);
// compilenode returning call540
// compilenode returning done
// Begin line 193
  setline(193);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool541 = alloc_Boolean(1);
// compilenode returning bool541
  block_return(realself, bool541);
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_setFileData479(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 8, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(6, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "setFileData");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  Object *var_curl = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for setFileData (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_file = &(stackframe->slots[0]);
  *var_file = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for setFileData");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_strippedUrl = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "strippedUrl");
  Object *var_findData = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "findData");
  Object *var_req = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "req");
// Begin line 201
  setline(201);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 177
  setline(177);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe482 = gc_frame_new();
  Object num483 = alloc_Float64(1);
// compilenode returning num483
  gc_frame_newslot(num483);
  Object num484 = alloc_Float64(4);
// compilenode returning num484
  gc_frame_newslot(num484);
  int callframe485 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call486 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe485);
// compilenode returning call486
// compilenode returning call486
  params[0] = num483;
  params[1] = num484;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call487 = callmethod(call486, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe482);
// compilenode returning call487
  int op_slot_left_481 = gc_frame_newslot(call487);
  if (strlit488 == NULL) {
    strlit488 = alloc_String("http");
    gc_root(strlit488);
  }
// compilenode returning strlit488
  int op_slot_right_481 = gc_frame_newslot(strlit488);
  params[0] = strlit488;
  partcv[0] = 1;
  Object opresult490 = callmethod(call487, "==", 1, partcv, params);
// compilenode returning opresult490
  Object if480 = done;
  if (istrue(opresult490)) {
// Begin line 178
  setline(178);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe491 = gc_frame_new();
  Object num492 = alloc_Float64(1);
// compilenode returning num492
  gc_frame_newslot(num492);
  Object num493 = alloc_Float64(4);
// compilenode returning num493
  gc_frame_newslot(num493);
  int callframe494 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call495 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe494);
// compilenode returning call495
// compilenode returning call495
  params[0] = num492;
  params[1] = num493;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call496 = callmethod(call495, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe491);
// compilenode returning call496
  *var_strippedUrl = call496;
  if (call496 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 186
  setline(186);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 179
  setline(179);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe499 = gc_frame_new();
  int callframe500 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call501 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe500);
// compilenode returning call501
// compilenode returning call501
  gc_frame_newslot(call501);
  params[0] = call501;
  partcv[0] = 1;
  Object call502 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe499);
// compilenode returning call502
  int op_slot_left_498 = gc_frame_newslot(call502);
  Object bool503 = alloc_Boolean(0);
// compilenode returning bool503
  int op_slot_right_498 = gc_frame_newslot(bool503);
  params[0] = bool503;
  partcv[0] = 1;
  Object opresult505 = callmethod(call502, "!=", 1, partcv, params);
// compilenode returning opresult505
  Object if497 = done;
  if (istrue(opresult505)) {
// Begin line 181
  setline(181);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe506 = gc_frame_new();
// Begin line 180
  setline(180);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_strippedUrl
  gc_frame_newslot(*var_strippedUrl);
  params[0] = *var_strippedUrl;
  partcv[0] = 1;
  Object call507 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe506);
// compilenode returning call507
  *var_findData = call507;
  if (call507 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 185
  setline(185);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 181
  setline(181);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  int op_slot_left_509 = gc_frame_newslot(*var_findData);
  Object num510 = alloc_Float64(0);
// compilenode returning num510
  int op_slot_right_509 = gc_frame_newslot(num510);
  params[0] = num510;
  partcv[0] = 1;
  Object opresult512 = callmethod(*var_findData, ">", 1, partcv, params);
// compilenode returning opresult512
  Object if508 = done;
  if (istrue(opresult512)) {
// Begin line 183
  setline(183);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 179
  setline(179);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe513 = gc_frame_new();
// Begin line 182
  setline(182);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  gc_frame_newslot(*var_findData);
// compilenode returning *var_file
  params[0] = *var_findData;
  partcv[0] = 1;
  Object call514 = callmethod(*var_file, "data:=",
    1, partcv, params);
  gc_frame_end(callframe513);
// compilenode returning call514
// compilenode returning done
// Begin line 183
  setline(183);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool515 = alloc_Boolean(1);
// compilenode returning bool515
  return bool515;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if508 = undefined;
  } else {
  }
// compilenode returning if508
    gc_frame_newslot(if508);
    if497 = if508;
  } else {
  }
// compilenode returning if497
// Begin line 187
  setline(187);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 186
  setline(186);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe516 = gc_frame_new();
// compilenode returning *var_curl
  partcv[0] = 0;
  Object call517 = callmethod(*var_curl, "easy",
    1, partcv, params);
  gc_frame_end(callframe516);
// compilenode returning call517
// compilenode returning call517
  *var_req = call517;
  if (call517 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 188
  setline(188);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 186
  setline(186);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe518 = gc_frame_new();
// Begin line 187
  setline(187);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe519 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call520 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe519);
// compilenode returning call520
// compilenode returning call520
  gc_frame_newslot(call520);
// compilenode returning *var_req
  params[0] = call520;
  partcv[0] = 1;
  Object call521 = callmethod(*var_req, "url:=",
    1, partcv, params);
  gc_frame_end(callframe518);
// compilenode returning call521
// compilenode returning done
// Begin line 189
  setline(189);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 188
  setline(188);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit523 == NULL) {
    strlit523 = alloc_String("Searching for import: ");
    gc_root(strlit523);
  }
// compilenode returning strlit523
  int op_slot_left_522 = gc_frame_newslot(strlit523);
  int callframe524 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call525 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe524);
// compilenode returning call525
// compilenode returning call525
  int op_slot_right_522 = gc_frame_newslot(call525);
  params[0] = call525;
  partcv[0] = 1;
  Object opresult527 = callmethod(strlit523, "++", 1, partcv, params);
// compilenode returning opresult527
  params[0] = opresult527;
  Object call528 = gracelib_print(NULL, 1,  params);
// compilenode returning call528
// Begin line 190
  setline(190);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 189
  setline(189);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit529 == NULL) {
    strlit529 = alloc_String(" Connecting...");
    gc_root(strlit529);
  }
// compilenode returning strlit529
  params[0] = strlit529;
  Object call530 = gracelib_print(NULL, 1,  params);
// compilenode returning call530
// Begin line 195
  setline(195);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe531 = gc_frame_new();
  Object block532 = alloc_Block(NULL, NULL, "PackageManager", 195);
  gc_frame_newslot(block532);
  block_savedest(block532);
  Object closure533 = createclosure(2, "_apply");
setclosureframe(closure533, stackframe);
  addtoclosure(closure533, var_file);
  addtoclosure(closure533, selfslot);
  struct UserObject *uo533 = (struct UserObject*)block532;
  uo533->data[0] = (Object)closure533;
  Method *meth_meth_PackageManager__apply533 = addmethod2pos(block532, "_apply", &meth_PackageManager__apply533, 0);
int argcv_meth_PackageManager__apply533[] = {1};
meth_meth_PackageManager__apply533->type = alloc_MethodType(1, argcv_meth_PackageManager__apply533);
  meth_meth_PackageManager__apply533->definitionModule = modulename;
  meth_meth_PackageManager__apply533->definitionLine = 188;
// compilenode returning block532
  gc_frame_newslot(block532);
// Begin line 190
  setline(190);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_req
  params[0] = block532;
  partcv[0] = 1;
  Object call543 = callmethod(*var_req, "onReceive",
    1, partcv, params);
  gc_frame_end(callframe531);
// compilenode returning call543
// Begin line 195
  setline(195);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe544 = gc_frame_new();
// compilenode returning *var_req
  partcv[0] = 0;
  Object call545 = callmethod(*var_req, "perform",
    1, partcv, params);
  gc_frame_end(callframe544);
// compilenode returning call545
// compilenode returning call545
// Begin line 200
  setline(200);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 196
  setline(196);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe548 = gc_frame_new();
// compilenode returning *var_req
  partcv[0] = 0;
  Object call549 = callmethod(*var_req, "responseCode",
    1, partcv, params);
  gc_frame_end(callframe548);
// compilenode returning call549
// compilenode returning call549
  int op_slot_left_547 = gc_frame_newslot(call549);
  Object num550 = alloc_Float64(200);
// compilenode returning num550
  int op_slot_right_547 = gc_frame_newslot(num550);
  params[0] = num550;
  partcv[0] = 1;
  Object opresult552 = callmethod(call549, "!=", 1, partcv, params);
// compilenode returning opresult552
  Object if546 = done;
  if (istrue(opresult552)) {
// Begin line 197
  setline(197);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit557 == NULL) {
    strlit557 = alloc_String(" Could not locate import: Error ");
    gc_root(strlit557);
  }
// compilenode returning strlit557
  int op_slot_left_556 = gc_frame_newslot(strlit557);
  int callframe558 = gc_frame_new();
// compilenode returning *var_req
  partcv[0] = 0;
  Object call559 = callmethod(*var_req, "responseCode",
    1, partcv, params);
  gc_frame_end(callframe558);
// compilenode returning call559
// compilenode returning call559
  int op_slot_right_556 = gc_frame_newslot(call559);
  params[0] = call559;
  partcv[0] = 1;
  Object opresult561 = callmethod(strlit557, "++", 1, partcv, params);
// compilenode returning opresult561
  int op_slot_left_555 = gc_frame_newslot(opresult561);
  if (strlit562 == NULL) {
    strlit562 = alloc_String(" for import ");
    gc_root(strlit562);
  }
// compilenode returning strlit562
  int op_slot_right_555 = gc_frame_newslot(strlit562);
  params[0] = strlit562;
  partcv[0] = 1;
  Object opresult564 = callmethod(opresult561, "++", 1, partcv, params);
// compilenode returning opresult564
  int op_slot_left_554 = gc_frame_newslot(opresult564);
  int callframe565 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call566 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe565);
// compilenode returning call566
// compilenode returning call566
  int op_slot_right_554 = gc_frame_newslot(call566);
  params[0] = call566;
  partcv[0] = 1;
  Object opresult568 = callmethod(opresult564, "++", 1, partcv, params);
// compilenode returning opresult568
  int op_slot_left_553 = gc_frame_newslot(opresult568);
  if (strlit569 == NULL) {
    strlit569 = alloc_String("");
    gc_root(strlit569);
  }
// compilenode returning strlit569
  int op_slot_right_553 = gc_frame_newslot(strlit569);
  params[0] = strlit569;
  partcv[0] = 1;
  Object opresult571 = callmethod(opresult568, "++", 1, partcv, params);
// compilenode returning opresult571
  params[0] = opresult571;
  Object call572 = gracelib_print(NULL, 1,  params);
// compilenode returning call572
// Begin line 199
  setline(199);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 198
  setline(198);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool573 = alloc_Boolean(0);
// compilenode returning bool573
  return bool573;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if546 = undefined;
  } else {
  }
// compilenode returning if546
    gc_frame_newslot(if546);
    if480 = if546;
  } else {
  }
// compilenode returning if480
// Begin line 209
  setline(209);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 201
  setline(201);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe576 = gc_frame_new();
  int callframe577 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call578 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe577);
// compilenode returning call578
// compilenode returning call578
  gc_frame_newslot(call578);
  params[0] = call578;
  partcv[0] = 1;
  Object call579 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe576);
// compilenode returning call579
  int op_slot_left_575 = gc_frame_newslot(call579);
  Object bool580 = alloc_Boolean(0);
// compilenode returning bool580
  int op_slot_right_575 = gc_frame_newslot(bool580);
  params[0] = bool580;
  partcv[0] = 1;
  Object opresult582 = callmethod(call579, "!=", 1, partcv, params);
// compilenode returning opresult582
  Object if574 = done;
  if (istrue(opresult582)) {
// Begin line 202
  setline(202);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit583 == NULL) {
    strlit583 = alloc_String("Now searching in find existing");
    gc_root(strlit583);
  }
// compilenode returning strlit583
  params[0] = strlit583;
  Object call584 = gracelib_print(NULL, 1,  params);
// compilenode returning call584
// Begin line 204
  setline(204);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe585 = gc_frame_new();
// Begin line 203
  setline(203);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe586 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call587 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe586);
// compilenode returning call587
// compilenode returning call587
  gc_frame_newslot(call587);
  params[0] = call587;
  partcv[0] = 1;
  Object call588 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe585);
// compilenode returning call588
  *var_findData = call588;
  if (call588 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 208
  setline(208);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 204
  setline(204);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  int op_slot_left_590 = gc_frame_newslot(*var_findData);
  Object bool591 = alloc_Boolean(0);
// compilenode returning bool591
  int op_slot_right_590 = gc_frame_newslot(bool591);
  params[0] = bool591;
  partcv[0] = 1;
  Object opresult593 = callmethod(*var_findData, "!=", 1, partcv, params);
// compilenode returning opresult593
  Object if589 = done;
  if (istrue(opresult593)) {
// Begin line 206
  setline(206);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 203
  setline(203);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe594 = gc_frame_new();
// Begin line 205
  setline(205);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  gc_frame_newslot(*var_findData);
// compilenode returning *var_file
  params[0] = *var_findData;
  partcv[0] = 1;
  Object call595 = callmethod(*var_file, "data:=",
    1, partcv, params);
  gc_frame_end(callframe594);
// compilenode returning call595
// compilenode returning done
// Begin line 206
  setline(206);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool596 = alloc_Boolean(1);
// compilenode returning bool596
  return bool596;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if589 = undefined;
  } else {
  }
// compilenode returning if589
    gc_frame_newslot(if589);
    if574 = if589;
  } else {
  }
// compilenode returning if574
// Begin line 209
  setline(209);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool597 = alloc_Boolean(0);
// compilenode returning bool597
  return bool597;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_findExisting598(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 9, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(5, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "findExisting");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[1];
  Object *var_io = getfromclosure(closure, 0);
  Object *var_sys = getfromclosure(closure, 1);
  Object *var_bundlePath = getfromclosure(closure, 2);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for findExisting (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_fileName = &(stackframe->slots[0]);
  *var_fileName = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for findExisting");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_open = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "open");
  Object *var_ret = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "ret");
  Object *var_homePath = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "homePath");
// Begin line 218
  setline(218);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 213
  setline(213);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe600 = gc_frame_new();
// compilenode returning *var_fileName
  gc_frame_newslot(*var_fileName);
// compilenode returning *var_io
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object call601 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe600);
// compilenode returning call601
  Object if599 = done;
  if (istrue(call601)) {
// Begin line 214
  setline(214);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe602 = gc_frame_new();
// compilenode returning *var_fileName
  gc_frame_newslot(*var_fileName);
  if (strlit603 == NULL) {
    strlit603 = alloc_String("r");
    gc_root(strlit603);
  }
// compilenode returning strlit603
  gc_frame_newslot(strlit603);
// compilenode returning *var_io
  params[0] = *var_fileName;
  params[1] = strlit603;
  partcv[0] = 2;
  Object call604 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe602);
// compilenode returning call604
  *var_open = call604;
  if (call604 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 215
  setline(215);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe605 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call606 = callmethod(*var_open, "read",
    1, partcv, params);
  gc_frame_end(callframe605);
// compilenode returning call606
// compilenode returning call606
  *var_ret = call606;
  if (call606 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 216
  setline(216);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_ret
  return *var_ret;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if599 = undefined;
  } else {
  }
// compilenode returning if599
// Begin line 221
  setline(221);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 218
  setline(218);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe608 = gc_frame_new();
  if (strlit610 == NULL) {
    strlit610 = alloc_String("/usr/lib/grace/modules/");
    gc_root(strlit610);
  }
// compilenode returning strlit610
  int op_slot_left_609 = gc_frame_newslot(strlit610);
// compilenode returning *var_fileName
  int op_slot_right_609 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult612 = callmethod(strlit610, "++", 1, partcv, params);
// compilenode returning opresult612
  gc_frame_newslot(opresult612);
// compilenode returning *var_io
  params[0] = opresult612;
  partcv[0] = 1;
  Object call613 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe608);
// compilenode returning call613
  Object if607 = done;
  if (istrue(call613)) {
// Begin line 220
  setline(220);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 219
  setline(219);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe614 = gc_frame_new();
  int callframe615 = gc_frame_new();
  if (strlit617 == NULL) {
    strlit617 = alloc_String("/usr/lib/grace/modules/");
    gc_root(strlit617);
  }
// compilenode returning strlit617
  int op_slot_left_616 = gc_frame_newslot(strlit617);
// compilenode returning *var_fileName
  int op_slot_right_616 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult619 = callmethod(strlit617, "++", 1, partcv, params);
// compilenode returning opresult619
  gc_frame_newslot(opresult619);
  if (strlit620 == NULL) {
    strlit620 = alloc_String("r");
    gc_root(strlit620);
  }
// compilenode returning strlit620
  gc_frame_newslot(strlit620);
// compilenode returning *var_io
  params[0] = opresult619;
  params[1] = strlit620;
  partcv[0] = 2;
  Object call621 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe615);
// compilenode returning call621
  partcv[0] = 0;
  Object call622 = callmethod(call621, "read",
    1, partcv, params);
  gc_frame_end(callframe614);
// compilenode returning call622
// compilenode returning call622
  return call622;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if607 = undefined;
  } else {
  }
// compilenode returning if607
// Begin line 222
  setline(222);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 221
  setline(221);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe623 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call624 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe623);
// compilenode returning call624
// compilenode returning call624
  if (strlit625 == NULL) {
    strlit625 = alloc_String("HOME");
    gc_root(strlit625);
  }
// compilenode returning strlit625
  params[0] = strlit625;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres626 = callmethod(call624, "[]", 1, partcv, params);
// compilenode returning idxres626
  *var_homePath = idxres626;
  if (idxres626 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 225
  setline(225);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 222
  setline(222);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe628 = gc_frame_new();
  if (strlit632 == NULL) {
    strlit632 = alloc_String("");
    gc_root(strlit632);
  }
// compilenode returning strlit632
  int op_slot_left_631 = gc_frame_newslot(strlit632);
// compilenode returning *var_homePath
  int op_slot_right_631 = gc_frame_newslot(*var_homePath);
  params[0] = *var_homePath;
  partcv[0] = 1;
  Object opresult634 = callmethod(strlit632, "++", 1, partcv, params);
// compilenode returning opresult634
  int op_slot_left_630 = gc_frame_newslot(opresult634);
  if (strlit635 == NULL) {
    strlit635 = alloc_String("/.local/share/grace/modules/");
    gc_root(strlit635);
  }
// compilenode returning strlit635
  int op_slot_right_630 = gc_frame_newslot(strlit635);
  params[0] = strlit635;
  partcv[0] = 1;
  Object opresult637 = callmethod(opresult634, "++", 1, partcv, params);
// compilenode returning opresult637
  int op_slot_left_629 = gc_frame_newslot(opresult637);
// compilenode returning *var_fileName
  int op_slot_right_629 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult639 = callmethod(opresult637, "++", 1, partcv, params);
// compilenode returning opresult639
  gc_frame_newslot(opresult639);
// compilenode returning *var_io
  params[0] = opresult639;
  partcv[0] = 1;
  Object call640 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe628);
// compilenode returning call640
  Object if627 = done;
  if (istrue(call640)) {
// Begin line 224
  setline(224);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 223
  setline(223);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe641 = gc_frame_new();
  int callframe642 = gc_frame_new();
  if (strlit646 == NULL) {
    strlit646 = alloc_String("");
    gc_root(strlit646);
  }
// compilenode returning strlit646
  int op_slot_left_645 = gc_frame_newslot(strlit646);
// compilenode returning *var_homePath
  int op_slot_right_645 = gc_frame_newslot(*var_homePath);
  params[0] = *var_homePath;
  partcv[0] = 1;
  Object opresult648 = callmethod(strlit646, "++", 1, partcv, params);
// compilenode returning opresult648
  int op_slot_left_644 = gc_frame_newslot(opresult648);
  if (strlit649 == NULL) {
    strlit649 = alloc_String("/.local/share/grace/modules/");
    gc_root(strlit649);
  }
// compilenode returning strlit649
  int op_slot_right_644 = gc_frame_newslot(strlit649);
  params[0] = strlit649;
  partcv[0] = 1;
  Object opresult651 = callmethod(opresult648, "++", 1, partcv, params);
// compilenode returning opresult651
  int op_slot_left_643 = gc_frame_newslot(opresult651);
// compilenode returning *var_fileName
  int op_slot_right_643 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult653 = callmethod(opresult651, "++", 1, partcv, params);
// compilenode returning opresult653
  gc_frame_newslot(opresult653);
  if (strlit654 == NULL) {
    strlit654 = alloc_String("r");
    gc_root(strlit654);
  }
// compilenode returning strlit654
  gc_frame_newslot(strlit654);
// compilenode returning *var_io
  params[0] = opresult653;
  params[1] = strlit654;
  partcv[0] = 2;
  Object call655 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe642);
// compilenode returning call655
  partcv[0] = 0;
  Object call656 = callmethod(call655, "read",
    1, partcv, params);
  gc_frame_end(callframe641);
// compilenode returning call656
// compilenode returning call656
  return call656;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if627 = undefined;
  } else {
  }
// compilenode returning if627
// Begin line 228
  setline(228);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 225
  setline(225);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe658 = gc_frame_new();
  if (strlit662 == NULL) {
    strlit662 = alloc_String("");
    gc_root(strlit662);
  }
// compilenode returning strlit662
  int op_slot_left_661 = gc_frame_newslot(strlit662);
  int callframe663 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call664 = callmethod(*var_sys, "execPath",
    1, partcv, params);
  gc_frame_end(callframe663);
// compilenode returning call664
// compilenode returning call664
  int op_slot_right_661 = gc_frame_newslot(call664);
  params[0] = call664;
  partcv[0] = 1;
  Object opresult666 = callmethod(strlit662, "++", 1, partcv, params);
// compilenode returning opresult666
  int op_slot_left_660 = gc_frame_newslot(opresult666);
  if (strlit667 == NULL) {
    strlit667 = alloc_String("/../lib/minigrace/modules/");
    gc_root(strlit667);
  }
// compilenode returning strlit667
  int op_slot_right_660 = gc_frame_newslot(strlit667);
  params[0] = strlit667;
  partcv[0] = 1;
  Object opresult669 = callmethod(opresult666, "++", 1, partcv, params);
// compilenode returning opresult669
  int op_slot_left_659 = gc_frame_newslot(opresult669);
// compilenode returning *var_fileName
  int op_slot_right_659 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult671 = callmethod(opresult669, "++", 1, partcv, params);
// compilenode returning opresult671
  gc_frame_newslot(opresult671);
// compilenode returning *var_io
  params[0] = opresult671;
  partcv[0] = 1;
  Object call672 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe658);
// compilenode returning call672
  Object if657 = done;
  if (istrue(call672)) {
// Begin line 227
  setline(227);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 226
  setline(226);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe673 = gc_frame_new();
  int callframe674 = gc_frame_new();
  if (strlit678 == NULL) {
    strlit678 = alloc_String("");
    gc_root(strlit678);
  }
// compilenode returning strlit678
  int op_slot_left_677 = gc_frame_newslot(strlit678);
  int callframe679 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call680 = callmethod(*var_sys, "execPath",
    1, partcv, params);
  gc_frame_end(callframe679);
// compilenode returning call680
// compilenode returning call680
  int op_slot_right_677 = gc_frame_newslot(call680);
  params[0] = call680;
  partcv[0] = 1;
  Object opresult682 = callmethod(strlit678, "++", 1, partcv, params);
// compilenode returning opresult682
  int op_slot_left_676 = gc_frame_newslot(opresult682);
  if (strlit683 == NULL) {
    strlit683 = alloc_String("/../lib/minigrace/modules/");
    gc_root(strlit683);
  }
// compilenode returning strlit683
  int op_slot_right_676 = gc_frame_newslot(strlit683);
  params[0] = strlit683;
  partcv[0] = 1;
  Object opresult685 = callmethod(opresult682, "++", 1, partcv, params);
// compilenode returning opresult685
  int op_slot_left_675 = gc_frame_newslot(opresult685);
// compilenode returning *var_fileName
  int op_slot_right_675 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult687 = callmethod(opresult685, "++", 1, partcv, params);
// compilenode returning opresult687
  gc_frame_newslot(opresult687);
  if (strlit688 == NULL) {
    strlit688 = alloc_String("r");
    gc_root(strlit688);
  }
// compilenode returning strlit688
  gc_frame_newslot(strlit688);
// compilenode returning *var_io
  params[0] = opresult687;
  params[1] = strlit688;
  partcv[0] = 2;
  Object call689 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe674);
// compilenode returning call689
  partcv[0] = 0;
  Object call690 = callmethod(call689, "read",
    1, partcv, params);
  gc_frame_end(callframe673);
// compilenode returning call690
// compilenode returning call690
  return call690;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if657 = undefined;
  } else {
  }
// compilenode returning if657
// Begin line 231
  setline(231);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 228
  setline(228);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe692 = gc_frame_new();
  if (strlit696 == NULL) {
    strlit696 = alloc_String("");
    gc_root(strlit696);
  }
// compilenode returning strlit696
  int op_slot_left_695 = gc_frame_newslot(strlit696);
  int callframe697 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call698 = callmethod(*var_sys, "execPath",
    1, partcv, params);
  gc_frame_end(callframe697);
// compilenode returning call698
// compilenode returning call698
  int op_slot_right_695 = gc_frame_newslot(call698);
  params[0] = call698;
  partcv[0] = 1;
  Object opresult700 = callmethod(strlit696, "++", 1, partcv, params);
// compilenode returning opresult700
  int op_slot_left_694 = gc_frame_newslot(opresult700);
  if (strlit701 == NULL) {
    strlit701 = alloc_String("/");
    gc_root(strlit701);
  }
// compilenode returning strlit701
  int op_slot_right_694 = gc_frame_newslot(strlit701);
  params[0] = strlit701;
  partcv[0] = 1;
  Object opresult703 = callmethod(opresult700, "++", 1, partcv, params);
// compilenode returning opresult703
  int op_slot_left_693 = gc_frame_newslot(opresult703);
// compilenode returning *var_fileName
  int op_slot_right_693 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult705 = callmethod(opresult703, "++", 1, partcv, params);
// compilenode returning opresult705
  gc_frame_newslot(opresult705);
// compilenode returning *var_io
  params[0] = opresult705;
  partcv[0] = 1;
  Object call706 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe692);
// compilenode returning call706
  Object if691 = done;
  if (istrue(call706)) {
// Begin line 230
  setline(230);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 229
  setline(229);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe707 = gc_frame_new();
  int callframe708 = gc_frame_new();
  if (strlit712 == NULL) {
    strlit712 = alloc_String("");
    gc_root(strlit712);
  }
// compilenode returning strlit712
  int op_slot_left_711 = gc_frame_newslot(strlit712);
  int callframe713 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call714 = callmethod(*var_sys, "execPath",
    1, partcv, params);
  gc_frame_end(callframe713);
// compilenode returning call714
// compilenode returning call714
  int op_slot_right_711 = gc_frame_newslot(call714);
  params[0] = call714;
  partcv[0] = 1;
  Object opresult716 = callmethod(strlit712, "++", 1, partcv, params);
// compilenode returning opresult716
  int op_slot_left_710 = gc_frame_newslot(opresult716);
  if (strlit717 == NULL) {
    strlit717 = alloc_String("/");
    gc_root(strlit717);
  }
// compilenode returning strlit717
  int op_slot_right_710 = gc_frame_newslot(strlit717);
  params[0] = strlit717;
  partcv[0] = 1;
  Object opresult719 = callmethod(opresult716, "++", 1, partcv, params);
// compilenode returning opresult719
  int op_slot_left_709 = gc_frame_newslot(opresult719);
// compilenode returning *var_fileName
  int op_slot_right_709 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult721 = callmethod(opresult719, "++", 1, partcv, params);
// compilenode returning opresult721
  gc_frame_newslot(opresult721);
  if (strlit722 == NULL) {
    strlit722 = alloc_String("r");
    gc_root(strlit722);
  }
// compilenode returning strlit722
  gc_frame_newslot(strlit722);
// compilenode returning *var_io
  params[0] = opresult721;
  params[1] = strlit722;
  partcv[0] = 2;
  Object call723 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe708);
// compilenode returning call723
  partcv[0] = 0;
  Object call724 = callmethod(call723, "read",
    1, partcv, params);
  gc_frame_end(callframe707);
// compilenode returning call724
// compilenode returning call724
  return call724;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if691 = undefined;
  } else {
  }
// compilenode returning if691
// Begin line 235
  setline(235);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 231
  setline(231);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe726 = gc_frame_new();
  if (strlit731 == NULL) {
    strlit731 = alloc_String("");
    gc_root(strlit731);
  }
// compilenode returning strlit731
  int op_slot_left_730 = gc_frame_newslot(strlit731);
  int callframe732 = gc_frame_new();
  partcv[0] = 0;
  Object call733 = callmethodflags(self, "getBuildPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe732);
// compilenode returning call733
  int op_slot_right_730 = gc_frame_newslot(call733);
  params[0] = call733;
  partcv[0] = 1;
  Object opresult735 = callmethod(strlit731, "++", 1, partcv, params);
// compilenode returning opresult735
  int op_slot_left_729 = gc_frame_newslot(opresult735);
  if (strlit736 == NULL) {
    strlit736 = alloc_String("/");
    gc_root(strlit736);
  }
// compilenode returning strlit736
  int op_slot_right_729 = gc_frame_newslot(strlit736);
  params[0] = strlit736;
  partcv[0] = 1;
  Object opresult738 = callmethod(opresult735, "++", 1, partcv, params);
// compilenode returning opresult738
  int op_slot_left_728 = gc_frame_newslot(opresult738);
// compilenode returning *var_fileName
  int op_slot_right_728 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult740 = callmethod(opresult738, "++", 1, partcv, params);
// compilenode returning opresult740
  int op_slot_left_727 = gc_frame_newslot(opresult740);
  if (strlit741 == NULL) {
    strlit741 = alloc_String("");
    gc_root(strlit741);
  }
// compilenode returning strlit741
  int op_slot_right_727 = gc_frame_newslot(strlit741);
  params[0] = strlit741;
  partcv[0] = 1;
  Object opresult743 = callmethod(opresult740, "++", 1, partcv, params);
// compilenode returning opresult743
  gc_frame_newslot(opresult743);
// compilenode returning *var_io
  params[0] = opresult743;
  partcv[0] = 1;
  Object call744 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe726);
// compilenode returning call744
  Object if725 = done;
  if (istrue(call744)) {
// Begin line 232
  setline(232);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit745 == NULL) {
    strlit745 = alloc_String("YES IT DOES");
    gc_root(strlit745);
  }
// compilenode returning strlit745
  params[0] = strlit745;
  Object call746 = gracelib_print(NULL, 1,  params);
// compilenode returning call746
// Begin line 234
  setline(234);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 233
  setline(233);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe747 = gc_frame_new();
  int callframe748 = gc_frame_new();
  if (strlit753 == NULL) {
    strlit753 = alloc_String("");
    gc_root(strlit753);
  }
// compilenode returning strlit753
  int op_slot_left_752 = gc_frame_newslot(strlit753);
  int callframe754 = gc_frame_new();
  partcv[0] = 0;
  Object call755 = callmethodflags(self, "getBuildPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe754);
// compilenode returning call755
  int op_slot_right_752 = gc_frame_newslot(call755);
  params[0] = call755;
  partcv[0] = 1;
  Object opresult757 = callmethod(strlit753, "++", 1, partcv, params);
// compilenode returning opresult757
  int op_slot_left_751 = gc_frame_newslot(opresult757);
  if (strlit758 == NULL) {
    strlit758 = alloc_String("/");
    gc_root(strlit758);
  }
// compilenode returning strlit758
  int op_slot_right_751 = gc_frame_newslot(strlit758);
  params[0] = strlit758;
  partcv[0] = 1;
  Object opresult760 = callmethod(opresult757, "++", 1, partcv, params);
// compilenode returning opresult760
  int op_slot_left_750 = gc_frame_newslot(opresult760);
// compilenode returning *var_fileName
  int op_slot_right_750 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult762 = callmethod(opresult760, "++", 1, partcv, params);
// compilenode returning opresult762
  int op_slot_left_749 = gc_frame_newslot(opresult762);
  if (strlit763 == NULL) {
    strlit763 = alloc_String("");
    gc_root(strlit763);
  }
// compilenode returning strlit763
  int op_slot_right_749 = gc_frame_newslot(strlit763);
  params[0] = strlit763;
  partcv[0] = 1;
  Object opresult765 = callmethod(opresult762, "++", 1, partcv, params);
// compilenode returning opresult765
  gc_frame_newslot(opresult765);
  if (strlit766 == NULL) {
    strlit766 = alloc_String("r");
    gc_root(strlit766);
  }
// compilenode returning strlit766
  gc_frame_newslot(strlit766);
// compilenode returning *var_io
  params[0] = opresult765;
  params[1] = strlit766;
  partcv[0] = 2;
  Object call767 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe748);
// compilenode returning call767
  partcv[0] = 0;
  Object call768 = callmethod(call767, "read",
    1, partcv, params);
  gc_frame_end(callframe747);
// compilenode returning call768
// compilenode returning call768
  return call768;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if725 = undefined;
  } else {
  }
// compilenode returning if725
// Begin line 239
  setline(239);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 235
  setline(235);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_bundlePath
  int op_slot_left_770 = gc_frame_newslot(*var_bundlePath);
  if (strlit771 == NULL) {
    strlit771 = alloc_String("");
    gc_root(strlit771);
  }
// compilenode returning strlit771
  int op_slot_right_770 = gc_frame_newslot(strlit771);
  params[0] = strlit771;
  partcv[0] = 1;
  Object opresult773 = callmethod(*var_bundlePath, "!=", 1, partcv, params);
// compilenode returning opresult773
  Object if769 = done;
  if (istrue(opresult773)) {
// Begin line 236
  setline(236);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe774 = gc_frame_new();
  int callframe775 = gc_frame_new();
  if (strlit780 == NULL) {
    strlit780 = alloc_String("");
    gc_root(strlit780);
  }
// compilenode returning strlit780
  int op_slot_left_779 = gc_frame_newslot(strlit780);
// compilenode returning *var_bundlePath
  int op_slot_right_779 = gc_frame_newslot(*var_bundlePath);
  params[0] = *var_bundlePath;
  partcv[0] = 1;
  Object opresult782 = callmethod(strlit780, "++", 1, partcv, params);
// compilenode returning opresult782
  int op_slot_left_778 = gc_frame_newslot(opresult782);
  if (strlit783 == NULL) {
    strlit783 = alloc_String("/");
    gc_root(strlit783);
  }
// compilenode returning strlit783
  int op_slot_right_778 = gc_frame_newslot(strlit783);
  params[0] = strlit783;
  partcv[0] = 1;
  Object opresult785 = callmethod(opresult782, "++", 1, partcv, params);
// compilenode returning opresult785
  int op_slot_left_777 = gc_frame_newslot(opresult785);
// compilenode returning *var_fileName
  int op_slot_right_777 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult787 = callmethod(opresult785, "++", 1, partcv, params);
// compilenode returning opresult787
  int op_slot_left_776 = gc_frame_newslot(opresult787);
  if (strlit788 == NULL) {
    strlit788 = alloc_String("");
    gc_root(strlit788);
  }
// compilenode returning strlit788
  int op_slot_right_776 = gc_frame_newslot(strlit788);
  params[0] = strlit788;
  partcv[0] = 1;
  Object opresult790 = callmethod(opresult787, "++", 1, partcv, params);
// compilenode returning opresult790
  gc_frame_newslot(opresult790);
  if (strlit791 == NULL) {
    strlit791 = alloc_String("r");
    gc_root(strlit791);
  }
// compilenode returning strlit791
  gc_frame_newslot(strlit791);
// compilenode returning *var_io
  params[0] = opresult790;
  params[1] = strlit791;
  partcv[0] = 2;
  Object call792 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe775);
// compilenode returning call792
  partcv[0] = 0;
  Object call793 = callmethod(call792, "read",
    1, partcv, params);
  gc_frame_end(callframe774);
// compilenode returning call793
// compilenode returning call793
  return call793;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if769 = undefined;
  } else {
  }
// compilenode returning if769
// Begin line 240
  setline(240);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 239
  setline(239);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool794 = alloc_Boolean(0);
// compilenode returning bool794
  return bool794;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_validateFile795(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct StackFrameObject *stackframe = alloc_StackFrame(2, NULL);
  pushclosure(NULL);
  pushstackframe(stackframe, "validateFile");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for validateFile (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_file = &(stackframe->slots[0]);
  *var_file = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for validateFile");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 249
  setline(249);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 243
  setline(243);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe798 = gc_frame_new();
  int callframe799 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call800 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe799);
// compilenode returning call800
// compilenode returning call800
  partcv[0] = 0;
  Object call801 = callmethod(call800, "size",
    1, partcv, params);
  gc_frame_end(callframe798);
// compilenode returning call801
// compilenode returning call801
  int op_slot_left_797 = gc_frame_newslot(call801);
  Object num802 = alloc_Float64(1);
// compilenode returning num802
  int op_slot_right_797 = gc_frame_newslot(num802);
  params[0] = num802;
  partcv[0] = 1;
  Object opresult804 = callmethod(call801, ">", 1, partcv, params);
// compilenode returning opresult804
  Object if796 = done;
  if (istrue(opresult804)) {
// Begin line 248
  setline(248);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 244
  setline(244);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe807 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call808 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe807);
// compilenode returning call808
// compilenode returning call808
  Object num809 = alloc_Float64(1);
// compilenode returning num809
  params[0] = num809;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres810 = callmethod(call808, "[]", 1, partcv, params);
// compilenode returning idxres810
  int op_slot_left_806 = gc_frame_newslot(idxres810);
  if (strlit811 == NULL) {
    strlit811 = alloc_String("<");
    gc_root(strlit811);
  }
// compilenode returning strlit811
  int op_slot_right_806 = gc_frame_newslot(strlit811);
  params[0] = strlit811;
  partcv[0] = 1;
  Object opresult813 = callmethod(idxres810, "==", 1, partcv, params);
// compilenode returning opresult813
  Object if805 = done;
  if (istrue(opresult813)) {
// Begin line 245
  setline(245);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit814 == NULL) {
    strlit814 = alloc_String("Not a valid grace file");
    gc_root(strlit814);
  }
// compilenode returning strlit814
  params[0] = strlit814;
  Object call815 = gracelib_print(NULL, 1,  params);
// compilenode returning call815
// Begin line 247
  setline(247);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 246
  setline(246);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool816 = alloc_Boolean(0);
// compilenode returning bool816
  return bool816;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if805 = undefined;
  } else {
  }
// compilenode returning if805
    gc_frame_newslot(if805);
    if796 = if805;
  } else {
  }
// compilenode returning if796
// Begin line 250
  setline(250);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 249
  setline(249);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool817 = alloc_Boolean(1);
// compilenode returning bool817
  return bool817;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_write818(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 11, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(5, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "write");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[1];
  Object *var_global = getfromclosure(closure, 0);
  Object *var_sys = getfromclosure(closure, 1);
  Object *var_io = getfromclosure(closure, 2);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for write (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_file = &(stackframe->slots[0]);
  *var_file = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for write");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_usrDir = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "usrDir");
  Object *var_fileDir = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "fileDir");
  Object *var_toWrite = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "toWrite");
// Begin line 254
  setline(254);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 253
  setline(253);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit819 == NULL) {
    strlit819 = alloc_String("");
    gc_root(strlit819);
  }
// compilenode returning strlit819
  *var_usrDir = strlit819;
  if (strlit819 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 260
  setline(260);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 254
  setline(254);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_global
  Object if820 = done;
  if (istrue(*var_global)) {
// Begin line 256
  setline(256);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 255
  setline(255);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit821 == NULL) {
    strlit821 = alloc_String("usr/lib/grace/modules/");
    gc_root(strlit821);
  }
// compilenode returning strlit821
  *var_usrDir = strlit821;
  if (strlit821 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if820 = done;
  } else {
// Begin line 259
  setline(259);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 258
  setline(258);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe824 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call825 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe824);
// compilenode returning call825
// compilenode returning call825
  if (strlit826 == NULL) {
    strlit826 = alloc_String("HOME");
    gc_root(strlit826);
  }
// compilenode returning strlit826
  params[0] = strlit826;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres827 = callmethod(call825, "[]", 1, partcv, params);
// compilenode returning idxres827
  int op_slot_left_823 = gc_frame_newslot(idxres827);
  if (strlit828 == NULL) {
    strlit828 = alloc_String("/.local/lib/grace/modules/");
    gc_root(strlit828);
  }
// compilenode returning strlit828
  int op_slot_right_823 = gc_frame_newslot(strlit828);
  params[0] = strlit828;
  partcv[0] = 1;
  Object opresult830 = callmethod(idxres827, "++", 1, partcv, params);
// compilenode returning opresult830
  *var_usrDir = opresult830;
  if (opresult830 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if820 = done;
  }
// compilenode returning if820
// Begin line 261
  setline(261);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe832 = gc_frame_new();
// Begin line 260
  setline(260);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_usrDir
  int op_slot_left_833 = gc_frame_newslot(*var_usrDir);
  int callframe834 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call835 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe834);
// compilenode returning call835
// compilenode returning call835
  int op_slot_right_833 = gc_frame_newslot(call835);
  params[0] = call835;
  partcv[0] = 1;
  Object opresult837 = callmethod(*var_usrDir, "++", 1, partcv, params);
// compilenode returning opresult837
  gc_frame_newslot(opresult837);
  params[0] = opresult837;
  partcv[0] = 1;
  Object call838 = callmethodflags(self, "createDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe832);
// compilenode returning call838
  *var_fileDir = call838;
  if (call838 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 262
  setline(262);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe839 = gc_frame_new();
// Begin line 261
  setline(261);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_fileDir
  Object num841 = alloc_Float64(1);
// compilenode returning num841
  params[0] = num841;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres842 = callmethod(*var_fileDir, "[]", 1, partcv, params);
// compilenode returning idxres842
  int op_slot_left_840 = gc_frame_newslot(idxres842);
// compilenode returning *var_fileDir
  Object num843 = alloc_Float64(2);
// compilenode returning num843
  params[0] = num843;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres844 = callmethod(*var_fileDir, "[]", 1, partcv, params);
// compilenode returning idxres844
  int op_slot_right_840 = gc_frame_newslot(idxres844);
  params[0] = idxres844;
  partcv[0] = 1;
  Object opresult846 = callmethod(idxres842, "++", 1, partcv, params);
// compilenode returning opresult846
  gc_frame_newslot(opresult846);
  if (strlit847 == NULL) {
    strlit847 = alloc_String("w");
    gc_root(strlit847);
  }
// compilenode returning strlit847
  gc_frame_newslot(strlit847);
// compilenode returning *var_io
  params[0] = opresult846;
  params[1] = strlit847;
  partcv[0] = 2;
  Object call848 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe839);
// compilenode returning call848
  *var_toWrite = call848;
  if (call848 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 262
  setline(262);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe849 = gc_frame_new();
  int callframe850 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call851 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe850);
// compilenode returning call851
// compilenode returning call851
  gc_frame_newslot(call851);
// compilenode returning *var_toWrite
  params[0] = call851;
  partcv[0] = 1;
  Object call852 = callmethod(*var_toWrite, "write",
    1, partcv, params);
  gc_frame_end(callframe849);
// compilenode returning call852
// Begin line 263
  setline(263);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe853 = gc_frame_new();
// compilenode returning *var_toWrite
  partcv[0] = 0;
  Object call854 = callmethod(*var_toWrite, "close",
    1, partcv, params);
  gc_frame_end(callframe853);
// compilenode returning call854
// compilenode returning call854
  gc_frame_end(frame);
  return call854;
}
Object meth_PackageManager__apply869(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_paths = getfromclosure(closure, 0);
  Object *var_buildPath = getfromclosure(closure, 1);
  Object *var_io = getfromclosure(closure, 2);
  Object self = *(getfromclosure(closure, 3));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *var_t = &(stackframe->slots[0]);
  *var_t = args[curarg];
  curarg++;
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 280
  setline(280);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 271
  setline(271);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe872 = gc_frame_new();
// compilenode returning *var_t
  gc_frame_newslot(*var_t);
// compilenode returning *var_paths
  params[0] = *var_t;
  partcv[0] = 1;
  Object call873 = callmethod(*var_paths, "at",
    1, partcv, params);
  gc_frame_end(callframe872);
// compilenode returning call873
  int op_slot_left_871 = gc_frame_newslot(call873);
  if (strlit874 == NULL) {
    strlit874 = alloc_String(":");
    gc_root(strlit874);
  }
// compilenode returning strlit874
  int op_slot_right_871 = gc_frame_newslot(strlit874);
  params[0] = strlit874;
  partcv[0] = 1;
  Object opresult876 = callmethod(call873, "==", 1, partcv, params);
// compilenode returning opresult876
  Object if870 = done;
  if (istrue(opresult876)) {
// Begin line 275
  setline(275);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 272
  setline(272);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe878 = gc_frame_new();
// compilenode returning *var_buildPath
  int op_slot_left_879 = gc_frame_newslot(*var_buildPath);
  if (strlit880 == NULL) {
    strlit880 = alloc_String("/minigrace");
    gc_root(strlit880);
  }
// compilenode returning strlit880
  int op_slot_right_879 = gc_frame_newslot(strlit880);
  params[0] = strlit880;
  partcv[0] = 1;
  Object opresult882 = callmethod(*var_buildPath, "++", 1, partcv, params);
// compilenode returning opresult882
  gc_frame_newslot(opresult882);
// compilenode returning *var_io
  params[0] = opresult882;
  partcv[0] = 1;
  Object call883 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe878);
// compilenode returning call883
  Object if877 = done;
  if (istrue(call883)) {
// Begin line 273
  setline(273);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  block_return(realself, *var_buildPath);
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if877 = undefined;
  } else {
  }
// compilenode returning if877
// Begin line 276
  setline(276);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 275
  setline(275);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit884 == NULL) {
    strlit884 = alloc_String("");
    gc_root(strlit884);
  }
// compilenode returning strlit884
  *var_buildPath = strlit884;
  if (strlit884 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if870 = done;
  } else {
// Begin line 279
  setline(279);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 278
  setline(278);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  int op_slot_left_886 = gc_frame_newslot(*var_buildPath);
// Begin line 279
  setline(279);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 278
  setline(278);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_paths
// compilenode returning *var_t
  params[0] = *var_t;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres887 = callmethod(*var_paths, "[]", 1, partcv, params);
// compilenode returning idxres887
  int op_slot_right_886 = gc_frame_newslot(idxres887);
  params[0] = idxres887;
  partcv[0] = 1;
  Object opresult889 = callmethod(*var_buildPath, "++", 1, partcv, params);
// compilenode returning opresult889
  *var_buildPath = opresult889;
  if (opresult889 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if870 = done;
  }
// compilenode returning if870
  gc_frame_end(frame);
  return if870;
}
Object meth_PackageManager_getBuildPath855(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 12, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(3, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "getBuildPath");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[2];
  int partcv[2];
  Object *var_sys = getfromclosure(closure, 0);
  Object *var_io = getfromclosure(closure, 1);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for getBuildPath (probably reflection error): got %i lists, expected 1.", nparts);
if (argcv && argcv[0] > 0)
  gracedie("too many arguments for getBuildPath");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_paths = &(stackframe->slots[1]);
  setframeelementname(stackframe, 1, "paths");
  Object *var_buildPath = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "buildPath");
// Begin line 269
  setline(269);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 268
  setline(268);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe856 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call857 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe856);
// compilenode returning call857
// compilenode returning call857
  if (strlit858 == NULL) {
    strlit858 = alloc_String("PATH");
    gc_root(strlit858);
  }
// compilenode returning strlit858
  params[0] = strlit858;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres859 = callmethod(call857, "[]", 1, partcv, params);
// compilenode returning idxres859
  *var_paths = idxres859;
  if (idxres859 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 270
  setline(270);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 269
  setline(269);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit860 == NULL) {
    strlit860 = alloc_String("");
    gc_root(strlit860);
  }
// compilenode returning strlit860
  *var_buildPath = strlit860;
  if (strlit860 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 281
  setline(281);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe861 = gc_frame_new();
// Begin line 270
  setline(270);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num863 = alloc_Float64(1);
// compilenode returning num863
  int op_slot_left_862 = gc_frame_newslot(num863);
  int callframe864 = gc_frame_new();
// compilenode returning *var_paths
  partcv[0] = 0;
  Object call865 = callmethod(*var_paths, "size",
    1, partcv, params);
  gc_frame_end(callframe864);
// compilenode returning call865
// compilenode returning call865
  int op_slot_right_862 = gc_frame_newslot(call865);
  params[0] = call865;
  partcv[0] = 1;
  Object opresult867 = callmethod(num863, "..", 1, partcv, params);
// compilenode returning opresult867
  gc_frame_newslot(opresult867);
// Begin line 281
  setline(281);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block868 = alloc_Block(NULL, NULL, "PackageManager", 281);
  gc_frame_newslot(block868);
  block_savedest(block868);
  Object closure869 = createclosure(4, "_apply");
setclosureframe(closure869, stackframe);
  addtoclosure(closure869, var_paths);
  addtoclosure(closure869, var_buildPath);
  addtoclosure(closure869, var_io);
  addtoclosure(closure869, selfslot);
  struct UserObject *uo869 = (struct UserObject*)block868;
  uo869->data[0] = (Object)closure869;
  Method *meth_meth_PackageManager__apply869 = addmethod2pos(block868, "_apply", &meth_PackageManager__apply869, 0);
int argcv_meth_PackageManager__apply869[] = {1};
meth_meth_PackageManager__apply869->type = alloc_MethodType(1, argcv_meth_PackageManager__apply869);
  meth_meth_PackageManager__apply869->definitionModule = modulename;
  meth_meth_PackageManager__apply869->definitionLine = 270;
// compilenode returning block868
  gc_frame_newslot(block868);
  params[0] = opresult867;
  params[1] = block868;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call892 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe861);
// compilenode returning call892
// Begin line 281
  setline(281);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  return *var_buildPath;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_compile893(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 13, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(5, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "compile");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_global = getfromclosure(closure, 0);
  Object *var_sys = getfromclosure(closure, 1);
  Object *var_io = getfromclosure(closure, 2);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for compile (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_file = &(stackframe->slots[0]);
  *var_file = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for compile");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_usrDir = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "usrDir");
  Object *var_paths = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "paths");
  Object *var_buildPath = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "buildPath");
// Begin line 286
  setline(286);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 285
  setline(285);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit894 == NULL) {
    strlit894 = alloc_String("");
    gc_root(strlit894);
  }
// compilenode returning strlit894
  *var_usrDir = strlit894;
  if (strlit894 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 292
  setline(292);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 286
  setline(286);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_global
  Object if895 = done;
  if (istrue(*var_global)) {
// Begin line 288
  setline(288);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 287
  setline(287);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit896 == NULL) {
    strlit896 = alloc_String("usr/lib/grace/modules/");
    gc_root(strlit896);
  }
// compilenode returning strlit896
  *var_usrDir = strlit896;
  if (strlit896 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if895 = done;
  } else {
// Begin line 291
  setline(291);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 290
  setline(290);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe899 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call900 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe899);
// compilenode returning call900
// compilenode returning call900
  if (strlit901 == NULL) {
    strlit901 = alloc_String("HOME");
    gc_root(strlit901);
  }
// compilenode returning strlit901
  params[0] = strlit901;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres902 = callmethod(call900, "[]", 1, partcv, params);
// compilenode returning idxres902
  int op_slot_left_898 = gc_frame_newslot(idxres902);
  if (strlit903 == NULL) {
    strlit903 = alloc_String("/.local/lib/grace/modules/");
    gc_root(strlit903);
  }
// compilenode returning strlit903
  int op_slot_right_898 = gc_frame_newslot(strlit903);
  params[0] = strlit903;
  partcv[0] = 1;
  Object opresult905 = callmethod(idxres902, "++", 1, partcv, params);
// compilenode returning opresult905
  *var_usrDir = opresult905;
  if (opresult905 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if895 = done;
  }
// compilenode returning if895
// Begin line 293
  setline(293);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 292
  setline(292);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe907 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call908 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe907);
// compilenode returning call908
// compilenode returning call908
  if (strlit909 == NULL) {
    strlit909 = alloc_String("PATH");
    gc_root(strlit909);
  }
// compilenode returning strlit909
  params[0] = strlit909;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres910 = callmethod(call908, "[]", 1, partcv, params);
// compilenode returning idxres910
  *var_paths = idxres910;
  if (idxres910 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 294
  setline(294);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 293
  setline(293);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit912 == NULL) {
    strlit912 = alloc_String("PATHS = ");
    gc_root(strlit912);
  }
// compilenode returning strlit912
  int op_slot_left_911 = gc_frame_newslot(strlit912);
// compilenode returning *var_paths
  int op_slot_right_911 = gc_frame_newslot(*var_paths);
  params[0] = *var_paths;
  partcv[0] = 1;
  Object opresult914 = callmethod(strlit912, "++", 1, partcv, params);
// compilenode returning opresult914
  params[0] = opresult914;
  Object call915 = gracelib_print(NULL, 1,  params);
// compilenode returning call915
// Begin line 294
  setline(294);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe916 = gc_frame_new();
  partcv[0] = 0;
  Object call917 = callmethodflags(self, "getBuildPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe916);
// compilenode returning call917
  *var_buildPath = call917;
  if (call917 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 296
  setline(296);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe918 = gc_frame_new();
// Begin line 295
  setline(295);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  int op_slot_left_921 = gc_frame_newslot(*var_buildPath);
  if (strlit922 == NULL) {
    strlit922 = alloc_String("/minigrace ");
    gc_root(strlit922);
  }
// compilenode returning strlit922
  int op_slot_right_921 = gc_frame_newslot(strlit922);
  params[0] = strlit922;
  partcv[0] = 1;
  Object opresult924 = callmethod(*var_buildPath, "++", 1, partcv, params);
// compilenode returning opresult924
  int op_slot_left_920 = gc_frame_newslot(opresult924);
// compilenode returning *var_usrDir
  int op_slot_right_920 = gc_frame_newslot(*var_usrDir);
  params[0] = *var_usrDir;
  partcv[0] = 1;
  Object opresult926 = callmethod(opresult924, "++", 1, partcv, params);
// compilenode returning opresult926
  int op_slot_left_919 = gc_frame_newslot(opresult926);
  int callframe927 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call928 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe927);
// compilenode returning call928
// compilenode returning call928
  int op_slot_right_919 = gc_frame_newslot(call928);
  params[0] = call928;
  partcv[0] = 1;
  Object opresult930 = callmethod(opresult926, "++", 1, partcv, params);
// compilenode returning opresult930
  gc_frame_newslot(opresult930);
// compilenode returning *var_io
  params[0] = opresult930;
  partcv[0] = 1;
  Object call931 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe918);
// compilenode returning call931
// Begin line 296
  setline(296);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool932 = alloc_Boolean(1);
// compilenode returning bool932
  return bool932;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply949(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_count = getfromclosure(closure, 0);
  Object *var_address = getfromclosure(closure, 1);
  Object self = *(getfromclosure(closure, 2));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var_count
  int op_slot_left_950 = gc_frame_newslot(*var_count);
  int callframe951 = gc_frame_new();
// compilenode returning *var_address
  partcv[0] = 0;
  Object call952 = callmethod(*var_address, "size",
    1, partcv, params);
  gc_frame_end(callframe951);
// compilenode returning call952
// compilenode returning call952
  int op_slot_right_950 = gc_frame_newslot(call952);
  params[0] = call952;
  partcv[0] = 1;
  Object opresult954 = callmethod(*var_count, "<=", 1, partcv, params);
// compilenode returning opresult954
  gc_frame_end(frame);
  return opresult954;
}
Object meth_PackageManager__apply957(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_nextPath = getfromclosure(closure, 0);
  Object *var_address = getfromclosure(closure, 1);
  Object *var_count = getfromclosure(closure, 2);
  Object *var_toMake = getfromclosure(closure, 3);
  Object self = *(getfromclosure(closure, 4));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 308
  setline(308);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 307
  setline(307);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_nextPath
  int op_slot_left_958 = gc_frame_newslot(*var_nextPath);
// Begin line 308
  setline(308);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 307
  setline(307);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_address
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres959 = callmethod(*var_address, "[]", 1, partcv, params);
// compilenode returning idxres959
  int op_slot_right_958 = gc_frame_newslot(idxres959);
  params[0] = idxres959;
  partcv[0] = 1;
  Object opresult961 = callmethod(*var_nextPath, "++", 1, partcv, params);
// compilenode returning opresult961
  *var_nextPath = opresult961;
  if (opresult961 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 312
  setline(312);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 308
  setline(308);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_address
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres965 = callmethod(*var_address, "[]", 1, partcv, params);
// compilenode returning idxres965
  int op_slot_left_964 = gc_frame_newslot(idxres965);
  if (strlit966 == NULL) {
    strlit966 = alloc_String("/");
    gc_root(strlit966);
  }
// compilenode returning strlit966
  int op_slot_right_964 = gc_frame_newslot(strlit966);
  params[0] = strlit966;
  partcv[0] = 1;
  Object opresult968 = callmethod(idxres965, "==", 1, partcv, params);
// compilenode returning opresult968
  Object if963 = done;
  if (istrue(opresult968)) {
// Begin line 310
  setline(310);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 309
  setline(309);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toMake
  int op_slot_left_969 = gc_frame_newslot(*var_toMake);
// compilenode returning *var_nextPath
  int op_slot_right_969 = gc_frame_newslot(*var_nextPath);
  params[0] = *var_nextPath;
  partcv[0] = 1;
  Object opresult971 = callmethod(*var_toMake, "++", 1, partcv, params);
// compilenode returning opresult971
  *var_toMake = opresult971;
  if (opresult971 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 311
  setline(311);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 310
  setline(310);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit973 == NULL) {
    strlit973 = alloc_String("");
    gc_root(strlit973);
  }
// compilenode returning strlit973
  *var_nextPath = strlit973;
  if (strlit973 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if963 = done;
  } else {
  }
// compilenode returning if963
// Begin line 312
  setline(312);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  int op_slot_left_975 = gc_frame_newslot(*var_count);
  Object num976 = alloc_Float64(1);
// compilenode returning num976
  int op_slot_right_975 = gc_frame_newslot(num976);
  params[0] = num976;
  partcv[0] = 1;
  Object sum978 = callmethod(*var_count, "+", 1, partcv, params);
// compilenode returning sum978
  *var_count = sum978;
  if (sum978 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_createDirectory933(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 14, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(6, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "createDirectory");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  Object *var_io = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for createDirectory (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_address = &(stackframe->slots[0]);
  *var_address = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for createDirectory");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_dir = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "dir");
  Object *var_nextPath = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "nextPath");
  Object *var_toMake = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "toMake");
  Object *var_count = &(stackframe->slots[5]);
  setframeelementname(stackframe, 5, "count");
// Begin line 302
  setline(302);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 301
  setline(301);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array934 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array934
  *var_dir = array934;
  if (array934 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 303
  setline(303);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 302
  setline(302);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit935 == NULL) {
    strlit935 = alloc_String("");
    gc_root(strlit935);
  }
// compilenode returning strlit935
  *var_nextPath = strlit935;
  if (strlit935 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 304
  setline(304);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 303
  setline(303);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit936 == NULL) {
    strlit936 = alloc_String("");
    gc_root(strlit936);
  }
// compilenode returning strlit936
  *var_toMake = strlit936;
  if (strlit936 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 305
  setline(305);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 304
  setline(304);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num937 = alloc_Float64(1);
// compilenode returning num937
  *var_count = num937;
  if (num937 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 306
  setline(306);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 305
  setline(305);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit940 == NULL) {
    strlit940 = alloc_String("Directory being evaluated ");
    gc_root(strlit940);
  }
// compilenode returning strlit940
  int op_slot_left_939 = gc_frame_newslot(strlit940);
// compilenode returning *var_address
  int op_slot_right_939 = gc_frame_newslot(*var_address);
  params[0] = *var_address;
  partcv[0] = 1;
  Object opresult942 = callmethod(strlit940, "++", 1, partcv, params);
// compilenode returning opresult942
  int op_slot_left_938 = gc_frame_newslot(opresult942);
  if (strlit943 == NULL) {
    strlit943 = alloc_String("");
    gc_root(strlit943);
  }
// compilenode returning strlit943
  int op_slot_right_938 = gc_frame_newslot(strlit943);
  params[0] = strlit943;
  partcv[0] = 1;
  Object opresult945 = callmethod(opresult942, "++", 1, partcv, params);
// compilenode returning opresult945
  params[0] = opresult945;
  Object call946 = gracelib_print(NULL, 1,  params);
// compilenode returning call946
// Begin line 314
  setline(314);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe947 = gc_frame_new();
// Begin line 306
  setline(306);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block948 = alloc_Block(NULL, NULL, "PackageManager", 306);
  gc_frame_newslot(block948);
  block_savedest(block948);
  Object closure949 = createclosure(3, "_apply");
setclosureframe(closure949, stackframe);
  addtoclosure(closure949, var_count);
  addtoclosure(closure949, var_address);
  addtoclosure(closure949, selfslot);
  struct UserObject *uo949 = (struct UserObject*)block948;
  uo949->data[0] = (Object)closure949;
  Method *meth_meth_PackageManager__apply949 = addmethod2pos(block948, "_apply", &meth_PackageManager__apply949, 0);
int argcv_meth_PackageManager__apply949[] = {0};
meth_meth_PackageManager__apply949->type = alloc_MethodType(1, argcv_meth_PackageManager__apply949);
  meth_meth_PackageManager__apply949->definitionModule = modulename;
  meth_meth_PackageManager__apply949->definitionLine = 295;
// compilenode returning block948
  gc_frame_newslot(block948);
// Begin line 314
  setline(314);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block956 = alloc_Block(NULL, NULL, "PackageManager", 314);
  gc_frame_newslot(block956);
  block_savedest(block956);
  Object closure957 = createclosure(5, "_apply");
setclosureframe(closure957, stackframe);
  addtoclosure(closure957, var_nextPath);
  addtoclosure(closure957, var_address);
  addtoclosure(closure957, var_count);
  addtoclosure(closure957, var_toMake);
  addtoclosure(closure957, selfslot);
  struct UserObject *uo957 = (struct UserObject*)block956;
  uo957->data[0] = (Object)closure957;
  Method *meth_meth_PackageManager__apply957 = addmethod2pos(block956, "_apply", &meth_PackageManager__apply957, 0);
int argcv_meth_PackageManager__apply957[] = {0};
meth_meth_PackageManager__apply957->type = alloc_MethodType(1, argcv_meth_PackageManager__apply957);
  meth_meth_PackageManager__apply957->definitionModule = modulename;
  meth_meth_PackageManager__apply957->definitionLine = 306;
// compilenode returning block956
  gc_frame_newslot(block956);
  params[0] = block948;
  params[1] = block956;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call981 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe947);
// compilenode returning call981
// Begin line 314
  setline(314);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit983 == NULL) {
    strlit983 = alloc_String("Creating directory ");
    gc_root(strlit983);
  }
// compilenode returning strlit983
  int op_slot_left_982 = gc_frame_newslot(strlit983);
// compilenode returning *var_toMake
  int op_slot_right_982 = gc_frame_newslot(*var_toMake);
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object opresult985 = callmethod(strlit983, "++", 1, partcv, params);
// compilenode returning opresult985
  params[0] = opresult985;
  Object call986 = gracelib_print(NULL, 1,  params);
// compilenode returning call986
// Begin line 319
  setline(319);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 315
  setline(315);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe988 = gc_frame_new();
  int callframe989 = gc_frame_new();
// compilenode returning *var_toMake
  gc_frame_newslot(*var_toMake);
// compilenode returning *var_io
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object call990 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe989);
// compilenode returning call990
  partcv[0] = 0;
  Object call991 = callmethod(call990, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe988);
// compilenode returning call991
  Object if987 = done;
  if (istrue(call991)) {
// Begin line 317
  setline(317);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe992 = gc_frame_new();
// Begin line 316
  setline(316);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit994 == NULL) {
    strlit994 = alloc_String("mkdir -p ");
    gc_root(strlit994);
  }
// compilenode returning strlit994
  int op_slot_left_993 = gc_frame_newslot(strlit994);
// compilenode returning *var_toMake
  int op_slot_right_993 = gc_frame_newslot(*var_toMake);
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object opresult996 = callmethod(strlit994, "++", 1, partcv, params);
// compilenode returning opresult996
  gc_frame_newslot(opresult996);
// compilenode returning *var_io
  params[0] = opresult996;
  partcv[0] = 1;
  Object call997 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe992);
// compilenode returning call997
// Begin line 318
  setline(318);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 317
  setline(317);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1000 == NULL) {
    strlit1000 = alloc_String("made directory ");
    gc_root(strlit1000);
  }
// compilenode returning strlit1000
  int op_slot_left_999 = gc_frame_newslot(strlit1000);
// compilenode returning *var_toMake
  int op_slot_right_999 = gc_frame_newslot(*var_toMake);
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object opresult1002 = callmethod(strlit1000, "++", 1, partcv, params);
// compilenode returning opresult1002
  int op_slot_left_998 = gc_frame_newslot(opresult1002);
  if (strlit1003 == NULL) {
    strlit1003 = alloc_String("");
    gc_root(strlit1003);
  }
// compilenode returning strlit1003
  int op_slot_right_998 = gc_frame_newslot(strlit1003);
  params[0] = strlit1003;
  partcv[0] = 1;
  Object opresult1005 = callmethod(opresult1002, "++", 1, partcv, params);
// compilenode returning opresult1005
  params[0] = opresult1005;
  Object call1006 = gracelib_print(NULL, 1,  params);
// compilenode returning call1006
    gc_frame_newslot(call1006);
    if987 = call1006;
  } else {
  }
// compilenode returning if987
// Begin line 320
  setline(320);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1007 = gc_frame_new();
// Begin line 319
  setline(319);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toMake
  gc_frame_newslot(*var_toMake);
// compilenode returning *var_dir
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object call1008 = callmethod(*var_dir, "push",
    1, partcv, params);
  gc_frame_end(callframe1007);
// compilenode returning call1008
// Begin line 321
  setline(321);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1009 = gc_frame_new();
// Begin line 320
  setline(320);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_nextPath
  gc_frame_newslot(*var_nextPath);
// compilenode returning *var_dir
  params[0] = *var_nextPath;
  partcv[0] = 1;
  Object call1010 = callmethod(*var_dir, "push",
    1, partcv, params);
  gc_frame_end(callframe1009);
// compilenode returning call1010
// Begin line 322
  setline(322);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 321
  setline(321);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_dir
  return *var_dir;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply1017(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_curPos = getfromclosure(closure, 0);
  Object *var_data = getfromclosure(closure, 1);
  Object self = *(getfromclosure(closure, 2));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var_curPos
  int op_slot_left_1018 = gc_frame_newslot(*var_curPos);
  int callframe1019 = gc_frame_new();
// compilenode returning *var_data
  partcv[0] = 0;
  Object call1020 = callmethod(*var_data, "size",
    1, partcv, params);
  gc_frame_end(callframe1019);
// compilenode returning call1020
// compilenode returning call1020
  int op_slot_right_1018 = gc_frame_newslot(call1020);
  params[0] = call1020;
  partcv[0] = 1;
  Object opresult1022 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1022
  gc_frame_end(frame);
  return opresult1022;
}
Object meth_PackageManager__apply1029(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_curPos = getfromclosure(closure, 0);
  Object *var_data = getfromclosure(closure, 1);
  Object self = *(getfromclosure(closure, 2));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var_curPos
  int op_slot_left_1031 = gc_frame_newslot(*var_curPos);
  int callframe1032 = gc_frame_new();
// compilenode returning *var_data
  partcv[0] = 0;
  Object call1033 = callmethod(*var_data, "size",
    1, partcv, params);
  gc_frame_end(callframe1032);
// compilenode returning call1033
// compilenode returning call1033
  int op_slot_right_1031 = gc_frame_newslot(call1033);
  params[0] = call1033;
  partcv[0] = 1;
  Object opresult1035 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1035
  int op_slot_left_1030 = gc_frame_newslot(opresult1035);
// compilenode returning *var_data
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1037 = callmethod(*var_data, "[]", 1, partcv, params);
// compilenode returning idxres1037
  int op_slot_left_1036 = gc_frame_newslot(idxres1037);
  if (strlit1038 == NULL) {
    strlit1038 = alloc_String("""\x0a""");
    gc_root(strlit1038);
  }
// compilenode returning strlit1038
  int op_slot_right_1036 = gc_frame_newslot(strlit1038);
  params[0] = strlit1038;
  partcv[0] = 1;
  Object opresult1040 = callmethod(idxres1037, "!=", 1, partcv, params);
// compilenode returning opresult1040
  int op_slot_right_1030 = gc_frame_newslot(opresult1040);
  params[0] = opresult1040;
  partcv[0] = 1;
  Object opresult1042 = callmethod(opresult1035, "&&", 1, partcv, params);
// compilenode returning opresult1042
  gc_frame_end(frame);
  return opresult1042;
}
Object meth_PackageManager__apply1045(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_curPos = getfromclosure(closure, 0);
  Object self = *(getfromclosure(closure, 1));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 331
  setline(331);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1046 = gc_frame_newslot(*var_curPos);
  Object num1047 = alloc_Float64(1);
// compilenode returning num1047
  int op_slot_right_1046 = gc_frame_newslot(num1047);
  params[0] = num1047;
  partcv[0] = 1;
  Object sum1049 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1049
  *var_curPos = sum1049;
  if (sum1049 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1025(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[2];
  int partcv[2];
  Object *var_curPos = getfromclosure(closure, 0);
  Object *var_startPos = getfromclosure(closure, 1);
  Object *var_data = getfromclosure(closure, 2);
  Object *var_done = getfromclosure(closure, 3);
  Object self = *(getfromclosure(closure, 4));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_line = &(stackframe->slots[1]);
  setframeelementname(stackframe, 1, "line");
// Begin line 330
  setline(330);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 329
  setline(329);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 333
  setline(333);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1027 = gc_frame_new();
// Begin line 330
  setline(330);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1028 = alloc_Block(NULL, NULL, "PackageManager", 330);
  gc_frame_newslot(block1028);
  block_savedest(block1028);
  Object closure1029 = createclosure(3, "_apply");
setclosureframe(closure1029, stackframe);
  addtoclosure(closure1029, var_curPos);
  addtoclosure(closure1029, var_data);
  addtoclosure(closure1029, selfslot);
  struct UserObject *uo1029 = (struct UserObject*)block1028;
  uo1029->data[0] = (Object)closure1029;
  Method *meth_meth_PackageManager__apply1029 = addmethod2pos(block1028, "_apply", &meth_PackageManager__apply1029, 0);
int argcv_meth_PackageManager__apply1029[] = {0};
meth_meth_PackageManager__apply1029->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1029);
  meth_meth_PackageManager__apply1029->definitionModule = modulename;
  meth_meth_PackageManager__apply1029->definitionLine = 328;
// compilenode returning block1028
  gc_frame_newslot(block1028);
// Begin line 333
  setline(333);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1044 = alloc_Block(NULL, NULL, "PackageManager", 333);
  gc_frame_newslot(block1044);
  block_savedest(block1044);
  Object closure1045 = createclosure(2, "_apply");
setclosureframe(closure1045, stackframe);
  addtoclosure(closure1045, var_curPos);
  addtoclosure(closure1045, selfslot);
  struct UserObject *uo1045 = (struct UserObject*)block1044;
  uo1045->data[0] = (Object)closure1045;
  Method *meth_meth_PackageManager__apply1045 = addmethod2pos(block1044, "_apply", &meth_PackageManager__apply1045, 0);
int argcv_meth_PackageManager__apply1045[] = {0};
meth_meth_PackageManager__apply1045->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1045);
  meth_meth_PackageManager__apply1045->definitionModule = modulename;
  meth_meth_PackageManager__apply1045->definitionLine = 330;
// compilenode returning block1044
  gc_frame_newslot(block1044);
  params[0] = block1028;
  params[1] = block1044;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1052 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1027);
// compilenode returning call1052
// Begin line 334
  setline(334);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1053 = gc_frame_new();
// Begin line 333
  setline(333);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_startPos
  gc_frame_newslot(*var_startPos);
// compilenode returning *var_curPos
  int op_slot_left_1054 = gc_frame_newslot(*var_curPos);
  Object num1055 = alloc_Float64(1);
// compilenode returning num1055
  int op_slot_right_1054 = gc_frame_newslot(num1055);
  params[0] = num1055;
  partcv[0] = 1;
  Object diff1057 = callmethod(*var_curPos, "-", 1, partcv, params);
// compilenode returning diff1057
  gc_frame_newslot(diff1057);
// compilenode returning *var_data
  params[0] = *var_startPos;
  params[1] = diff1057;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1058 = callmethod(*var_data, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1053);
// compilenode returning call1058
  *var_line = call1058;
  if (call1058 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 337
  setline(337);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 334
  setline(334);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1060 = gc_frame_new();
  int callframe1061 = gc_frame_new();
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
  params[0] = *var_line;
  partcv[0] = 1;
  Object call1062 = callmethodflags(self, "processLine", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1061);
// compilenode returning call1062
  partcv[0] = 0;
  Object call1063 = callmethod(call1062, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1060);
// compilenode returning call1063
  Object if1059 = done;
  if (istrue(call1063)) {
// Begin line 336
  setline(336);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_done
  block_return(realself, *var_done);
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1059 = undefined;
  } else {
  }
// compilenode returning if1059
// Begin line 338
  setline(338);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 337
  setline(337);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1064 = gc_frame_newslot(*var_curPos);
  Object num1065 = alloc_Float64(1);
// compilenode returning num1065
  int op_slot_right_1064 = gc_frame_newslot(num1065);
  params[0] = num1065;
  partcv[0] = 1;
  Object sum1067 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1067
  *var_curPos = sum1067;
  if (sum1067 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_parseFile1011(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 15, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(5, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "parseFile");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  Object *var_done = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for parseFile (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_file = &(stackframe->slots[0]);
  *var_file = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for parseFile");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_data = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "data");
  Object *var_curPos = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "curPos");
  Object *var_startPos = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "startPos");
// Begin line 326
  setline(326);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 325
  setline(325);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1012 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1013 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe1012);
// compilenode returning call1013
// compilenode returning call1013
  *var_data = call1013;
  if (call1013 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 326
  setline(326);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1014 = alloc_Float64(1);
// compilenode returning num1014
  *var_curPos = num1014;
  if (num1014 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 328
  setline(328);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 327
  setline(327);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 339
  setline(339);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1015 = gc_frame_new();
// Begin line 328
  setline(328);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1016 = alloc_Block(NULL, NULL, "PackageManager", 328);
  gc_frame_newslot(block1016);
  block_savedest(block1016);
  Object closure1017 = createclosure(3, "_apply");
setclosureframe(closure1017, stackframe);
  addtoclosure(closure1017, var_curPos);
  addtoclosure(closure1017, var_data);
  addtoclosure(closure1017, selfslot);
  struct UserObject *uo1017 = (struct UserObject*)block1016;
  uo1017->data[0] = (Object)closure1017;
  Method *meth_meth_PackageManager__apply1017 = addmethod2pos(block1016, "_apply", &meth_PackageManager__apply1017, 0);
int argcv_meth_PackageManager__apply1017[] = {0};
meth_meth_PackageManager__apply1017->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1017);
  meth_meth_PackageManager__apply1017->definitionModule = modulename;
  meth_meth_PackageManager__apply1017->definitionLine = 325;
// compilenode returning block1016
  gc_frame_newslot(block1016);
// Begin line 339
  setline(339);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1024 = alloc_Block(NULL, NULL, "PackageManager", 339);
  gc_frame_newslot(block1024);
  block_savedest(block1024);
  Object closure1025 = createclosure(5, "_apply");
setclosureframe(closure1025, stackframe);
  addtoclosure(closure1025, var_curPos);
  addtoclosure(closure1025, var_startPos);
  addtoclosure(closure1025, var_data);
  addtoclosure(closure1025, var_done);
  addtoclosure(closure1025, selfslot);
  struct UserObject *uo1025 = (struct UserObject*)block1024;
  uo1025->data[0] = (Object)closure1025;
  Method *meth_meth_PackageManager__apply1025 = addmethod2pos(block1024, "_apply", &meth_PackageManager__apply1025, 0);
int argcv_meth_PackageManager__apply1025[] = {0};
meth_meth_PackageManager__apply1025->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1025);
  meth_meth_PackageManager__apply1025->definitionModule = modulename;
  meth_meth_PackageManager__apply1025->definitionLine = 328;
// compilenode returning block1024
  gc_frame_newslot(block1024);
  params[0] = block1016;
  params[1] = block1024;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1070 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1015);
// compilenode returning call1070
  gc_frame_end(frame);
  return call1070;
}
Object meth_PackageManager_processLine1071(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct StackFrameObject *stackframe = alloc_StackFrame(2, NULL);
  pushclosure(NULL);
  pushstackframe(stackframe, "processLine");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for processLine (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_line = &(stackframe->slots[0]);
  *var_line = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for processLine");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 357
  setline(357);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 342
  setline(342);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1074 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1075 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1074);
// compilenode returning call1075
// compilenode returning call1075
  int op_slot_left_1073 = gc_frame_newslot(call1075);
  Object num1076 = alloc_Float64(1);
// compilenode returning num1076
  int op_slot_right_1073 = gc_frame_newslot(num1076);
  params[0] = num1076;
  partcv[0] = 1;
  Object opresult1078 = callmethod(call1075, ">", 1, partcv, params);
// compilenode returning opresult1078
  Object if1072 = done;
  if (istrue(opresult1078)) {
// Begin line 356
  setline(356);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 343
  setline(343);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  Object num1081 = alloc_Float64(1);
// compilenode returning num1081
  params[0] = num1081;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1082 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1082
  int op_slot_left_1080 = gc_frame_newslot(idxres1082);
  if (strlit1083 == NULL) {
    strlit1083 = alloc_String("#");
    gc_root(strlit1083);
  }
// compilenode returning strlit1083
  int op_slot_right_1080 = gc_frame_newslot(strlit1083);
  params[0] = strlit1083;
  partcv[0] = 1;
  Object opresult1085 = callmethod(idxres1082, "==", 1, partcv, params);
// compilenode returning opresult1085
  Object if1079 = done;
  if (istrue(opresult1085)) {
// Begin line 345
  setline(345);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 344
  setline(344);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1086 = alloc_Boolean(1);
// compilenode returning bool1086
  return bool1086;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1079 = undefined;
  } else {
// Begin line 349
  setline(349);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 346
  setline(346);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1090 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1091 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1090);
// compilenode returning call1091
// compilenode returning call1091
  int op_slot_left_1089 = gc_frame_newslot(call1091);
  Object num1092 = alloc_Float64(2);
// compilenode returning num1092
  int op_slot_right_1089 = gc_frame_newslot(num1092);
  params[0] = num1092;
  partcv[0] = 1;
  Object opresult1094 = callmethod(call1091, ">", 1, partcv, params);
// compilenode returning opresult1094
  int op_slot_left_1088 = gc_frame_newslot(opresult1094);
  int callframe1096 = gc_frame_new();
  Object num1097 = alloc_Float64(1);
// compilenode returning num1097
  gc_frame_newslot(num1097);
  Object num1098 = alloc_Float64(2);
// compilenode returning num1098
  gc_frame_newslot(num1098);
// compilenode returning *var_line
  params[0] = num1097;
  params[1] = num1098;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1099 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1096);
// compilenode returning call1099
  int op_slot_left_1095 = gc_frame_newslot(call1099);
  if (strlit1100 == NULL) {
    strlit1100 = alloc_String("//");
    gc_root(strlit1100);
  }
// compilenode returning strlit1100
  int op_slot_right_1095 = gc_frame_newslot(strlit1100);
  params[0] = strlit1100;
  partcv[0] = 1;
  Object opresult1102 = callmethod(call1099, "==", 1, partcv, params);
// compilenode returning opresult1102
  int op_slot_right_1088 = gc_frame_newslot(opresult1102);
  params[0] = opresult1102;
  partcv[0] = 1;
  Object opresult1104 = callmethod(opresult1094, "&&", 1, partcv, params);
// compilenode returning opresult1104
  Object if1087 = done;
  if (istrue(opresult1104)) {
// Begin line 348
  setline(348);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 347
  setline(347);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1105 = alloc_Boolean(1);
// compilenode returning bool1105
  return bool1105;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1087 = undefined;
  } else {
// Begin line 353
  setline(353);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 349
  setline(349);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1109 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1110 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1109);
// compilenode returning call1110
// compilenode returning call1110
  int op_slot_left_1108 = gc_frame_newslot(call1110);
  Object num1111 = alloc_Float64(6);
// compilenode returning num1111
  int op_slot_right_1108 = gc_frame_newslot(num1111);
  params[0] = num1111;
  partcv[0] = 1;
  Object opresult1113 = callmethod(call1110, ">", 1, partcv, params);
// compilenode returning opresult1113
  int op_slot_left_1107 = gc_frame_newslot(opresult1113);
  int callframe1115 = gc_frame_new();
  Object num1116 = alloc_Float64(1);
// compilenode returning num1116
  gc_frame_newslot(num1116);
  Object num1117 = alloc_Float64(7);
// compilenode returning num1117
  gc_frame_newslot(num1117);
// compilenode returning *var_line
  params[0] = num1116;
  params[1] = num1117;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1118 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1115);
// compilenode returning call1118
  int op_slot_left_1114 = gc_frame_newslot(call1118);
  if (strlit1119 == NULL) {
    strlit1119 = alloc_String("import ");
    gc_root(strlit1119);
  }
// compilenode returning strlit1119
  int op_slot_right_1114 = gc_frame_newslot(strlit1119);
  params[0] = strlit1119;
  partcv[0] = 1;
  Object opresult1121 = callmethod(call1118, "==", 1, partcv, params);
// compilenode returning opresult1121
  int op_slot_right_1107 = gc_frame_newslot(opresult1121);
  params[0] = opresult1121;
  partcv[0] = 1;
  Object opresult1123 = callmethod(opresult1113, "&&", 1, partcv, params);
// compilenode returning opresult1123
  Object if1106 = done;
  if (istrue(opresult1123)) {
// Begin line 350
  setline(350);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1124 = gc_frame_new();
  int callframe1125 = gc_frame_new();
  Object num1126 = alloc_Float64(8);
// compilenode returning num1126
  gc_frame_newslot(num1126);
  int callframe1127 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1128 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1127);
// compilenode returning call1128
// compilenode returning call1128
  gc_frame_newslot(call1128);
// compilenode returning *var_line
  params[0] = num1126;
  params[1] = call1128;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1129 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1125);
// compilenode returning call1129
  gc_frame_newslot(call1129);
  params[0] = call1129;
  partcv[0] = 1;
  Object call1130 = callmethodflags(self, "parseImport", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1124);
// compilenode returning call1130
// Begin line 352
  setline(352);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 351
  setline(351);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1131 = alloc_Boolean(1);
// compilenode returning bool1131
  return bool1131;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1106 = undefined;
  } else {
// Begin line 356
  setline(356);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 353
  setline(353);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1135 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1136 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1135);
// compilenode returning call1136
// compilenode returning call1136
  int op_slot_left_1134 = gc_frame_newslot(call1136);
  Object num1137 = alloc_Float64(7);
// compilenode returning num1137
  int op_slot_right_1134 = gc_frame_newslot(num1137);
  params[0] = num1137;
  partcv[0] = 1;
  Object opresult1139 = callmethod(call1136, ">", 1, partcv, params);
// compilenode returning opresult1139
  int op_slot_left_1133 = gc_frame_newslot(opresult1139);
  int callframe1141 = gc_frame_new();
  Object num1142 = alloc_Float64(1);
// compilenode returning num1142
  gc_frame_newslot(num1142);
  Object num1143 = alloc_Float64(8);
// compilenode returning num1143
  gc_frame_newslot(num1143);
// compilenode returning *var_line
  params[0] = num1142;
  params[1] = num1143;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1144 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1141);
// compilenode returning call1144
  int op_slot_left_1140 = gc_frame_newslot(call1144);
  if (strlit1145 == NULL) {
    strlit1145 = alloc_String("dialect ");
    gc_root(strlit1145);
  }
// compilenode returning strlit1145
  int op_slot_right_1140 = gc_frame_newslot(strlit1145);
  params[0] = strlit1145;
  partcv[0] = 1;
  Object opresult1147 = callmethod(call1144, "==", 1, partcv, params);
// compilenode returning opresult1147
  int op_slot_right_1133 = gc_frame_newslot(opresult1147);
  params[0] = opresult1147;
  partcv[0] = 1;
  Object opresult1149 = callmethod(opresult1139, "&&", 1, partcv, params);
// compilenode returning opresult1149
  Object if1132 = done;
  if (istrue(opresult1149)) {
// Begin line 355
  setline(355);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 354
  setline(354);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1150 = alloc_Boolean(1);
// compilenode returning bool1150
  return bool1150;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1132 = undefined;
  } else {
  }
// compilenode returning if1132
    gc_frame_newslot(if1132);
    if1106 = if1132;
  }
// compilenode returning if1106
    gc_frame_newslot(if1106);
    if1087 = if1106;
  }
// compilenode returning if1087
    gc_frame_newslot(if1087);
    if1079 = if1087;
  }
// compilenode returning if1079
    gc_frame_newslot(if1079);
    if1072 = if1079;
  } else {
  }
// compilenode returning if1072
// Begin line 358
  setline(358);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 357
  setline(357);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1151 = alloc_Boolean(0);
// compilenode returning bool1151
  return bool1151;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply1166(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_curPos = getfromclosure(closure, 0);
  Object *var_line = getfromclosure(closure, 1);
  Object self = *(getfromclosure(closure, 2));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var_curPos
  int op_slot_left_1168 = gc_frame_newslot(*var_curPos);
  int callframe1169 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1170 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1169);
// compilenode returning call1170
// compilenode returning call1170
  int op_slot_right_1168 = gc_frame_newslot(call1170);
  params[0] = call1170;
  partcv[0] = 1;
  Object opresult1172 = callmethod(*var_curPos, "<", 1, partcv, params);
// compilenode returning opresult1172
  int op_slot_left_1167 = gc_frame_newslot(opresult1172);
  int callframe1173 = gc_frame_new();
// compilenode returning *var_line
// compilenode returning *var_curPos
  int op_slot_left_1176 = gc_frame_newslot(*var_curPos);
  Object num1177 = alloc_Float64(1);
// compilenode returning num1177
  int op_slot_right_1176 = gc_frame_newslot(num1177);
  params[0] = num1177;
  partcv[0] = 1;
  Object sum1179 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1179
  params[0] = sum1179;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1180 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1180
  int op_slot_left_1175 = gc_frame_newslot(idxres1180);
  if (strlit1181 == NULL) {
    strlit1181 = alloc_String("""\x22""");
    gc_root(strlit1181);
  }
// compilenode returning strlit1181
  int op_slot_right_1175 = gc_frame_newslot(strlit1181);
  params[0] = strlit1181;
  partcv[0] = 1;
  Object opresult1183 = callmethod(idxres1180, "==", 1, partcv, params);
// compilenode returning opresult1183
  int op_slot_left_1174 = gc_frame_newslot(opresult1183);
// compilenode returning *var_line
// compilenode returning *var_curPos
  int op_slot_left_1185 = gc_frame_newslot(*var_curPos);
  Object num1186 = alloc_Float64(1);
// compilenode returning num1186
  int op_slot_right_1185 = gc_frame_newslot(num1186);
  params[0] = num1186;
  partcv[0] = 1;
  Object sum1188 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1188
  params[0] = sum1188;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1189 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1189
  int op_slot_left_1184 = gc_frame_newslot(idxres1189);
  if (strlit1190 == NULL) {
    strlit1190 = alloc_String(" ");
    gc_root(strlit1190);
  }
// compilenode returning strlit1190
  int op_slot_right_1184 = gc_frame_newslot(strlit1190);
  params[0] = strlit1190;
  partcv[0] = 1;
  Object opresult1192 = callmethod(idxres1189, "==", 1, partcv, params);
// compilenode returning opresult1192
  int op_slot_right_1174 = gc_frame_newslot(opresult1192);
  params[0] = opresult1192;
  partcv[0] = 1;
  Object opresult1194 = callmethod(opresult1183, "||", 1, partcv, params);
// compilenode returning opresult1194
  partcv[0] = 0;
  Object call1195 = callmethod(opresult1194, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1173);
// compilenode returning call1195
  int op_slot_right_1167 = gc_frame_newslot(call1195);
  params[0] = call1195;
  partcv[0] = 1;
  Object opresult1197 = callmethod(opresult1172, "&&", 1, partcv, params);
// compilenode returning opresult1197
  gc_frame_end(frame);
  return opresult1197;
}
Object meth_PackageManager__apply1200(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_curPos = getfromclosure(closure, 0);
  Object self = *(getfromclosure(closure, 1));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 369
  setline(369);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 368
  setline(368);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1201 = gc_frame_newslot(*var_curPos);
  Object num1202 = alloc_Float64(1);
// compilenode returning num1202
  int op_slot_right_1201 = gc_frame_newslot(num1202);
  params[0] = num1202;
  partcv[0] = 1;
  Object sum1204 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1204
  *var_curPos = sum1204;
  if (sum1204 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_parseImport1152(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 17, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(5, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "parseImport");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  Object *var_imported = getfromclosure(closure, 0);
  Object *var_toProcess = getfromclosure(closure, 1);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for parseImport (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_line = &(stackframe->slots[0]);
  *var_line = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for parseImport");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_curPos = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "curPos");
  Object *var_startPos = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "startPos");
  Object *var_nextImport = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "nextImport");
// Begin line 362
  setline(362);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 361
  setline(361);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1153 = alloc_Float64(1);
// compilenode returning num1153
  *var_curPos = num1153;
  if (num1153 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 363
  setline(363);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 362
  setline(362);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 364
  setline(364);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 363
  setline(363);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1154 == NULL) {
    strlit1154 = alloc_String("");
    gc_root(strlit1154);
  }
// compilenode returning strlit1154
  *var_nextImport = strlit1154;
  if (strlit1154 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 365
  setline(365);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1155 = gc_frame_new();
// Begin line 364
  setline(364);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// compilenode returning *var_curPos
  gc_frame_newslot(*var_curPos);
  params[0] = *var_line;
  params[1] = *var_curPos;
  partcv[0] = 2;
  Object call1156 = callmethodflags(self, "skipWhiteSpace", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1155);
// compilenode returning call1156
  *var_curPos = call1156;
  if (call1156 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 366
  setline(366);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 365
  setline(365);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1158 = gc_frame_newslot(*var_curPos);
  Object num1159 = alloc_Float64(1);
// compilenode returning num1159
  int op_slot_right_1158 = gc_frame_newslot(num1159);
  params[0] = num1159;
  partcv[0] = 1;
  Object sum1161 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1161
  *var_curPos = sum1161;
  if (sum1161 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 367
  setline(367);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 366
  setline(366);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 370
  setline(370);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1164 = gc_frame_new();
// Begin line 367
  setline(367);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1165 = alloc_Block(NULL, NULL, "PackageManager", 367);
  gc_frame_newslot(block1165);
  block_savedest(block1165);
  Object closure1166 = createclosure(3, "_apply");
setclosureframe(closure1166, stackframe);
  addtoclosure(closure1166, var_curPos);
  addtoclosure(closure1166, var_line);
  addtoclosure(closure1166, selfslot);
  struct UserObject *uo1166 = (struct UserObject*)block1165;
  uo1166->data[0] = (Object)closure1166;
  Method *meth_meth_PackageManager__apply1166 = addmethod2pos(block1165, "_apply", &meth_PackageManager__apply1166, 0);
int argcv_meth_PackageManager__apply1166[] = {0};
meth_meth_PackageManager__apply1166->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1166);
  meth_meth_PackageManager__apply1166->definitionModule = modulename;
  meth_meth_PackageManager__apply1166->definitionLine = 353;
// compilenode returning block1165
  gc_frame_newslot(block1165);
// Begin line 370
  setline(370);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1199 = alloc_Block(NULL, NULL, "PackageManager", 370);
  gc_frame_newslot(block1199);
  block_savedest(block1199);
  Object closure1200 = createclosure(2, "_apply");
setclosureframe(closure1200, stackframe);
  addtoclosure(closure1200, var_curPos);
  addtoclosure(closure1200, selfslot);
  struct UserObject *uo1200 = (struct UserObject*)block1199;
  uo1200->data[0] = (Object)closure1200;
  Method *meth_meth_PackageManager__apply1200 = addmethod2pos(block1199, "_apply", &meth_PackageManager__apply1200, 0);
int argcv_meth_PackageManager__apply1200[] = {0};
meth_meth_PackageManager__apply1200->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1200);
  meth_meth_PackageManager__apply1200->definitionModule = modulename;
  meth_meth_PackageManager__apply1200->definitionLine = 367;
// compilenode returning block1199
  gc_frame_newslot(block1199);
  params[0] = block1165;
  params[1] = block1199;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1207 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1164);
// compilenode returning call1207
// Begin line 371
  setline(371);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1208 = gc_frame_new();
// Begin line 370
  setline(370);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_startPos
  gc_frame_newslot(*var_startPos);
// compilenode returning *var_curPos
  gc_frame_newslot(*var_curPos);
// compilenode returning *var_line
  params[0] = *var_startPos;
  params[1] = *var_curPos;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1209 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1208);
// compilenode returning call1209
  *var_nextImport = call1209;
  if (call1209 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 375
  setline(375);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 371
  setline(371);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1212 = gc_frame_new();
  int callframe1213 = gc_frame_new();
// compilenode returning *var_nextImport
  gc_frame_newslot(*var_nextImport);
// compilenode returning *var_imported
  params[0] = *var_nextImport;
  partcv[0] = 1;
  Object call1214 = callmethod(*var_imported, "contains",
    1, partcv, params);
  gc_frame_end(callframe1213);
// compilenode returning call1214
  partcv[0] = 0;
  Object call1215 = callmethod(call1214, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1212);
// compilenode returning call1215
  Object if1211 = done;
  if (istrue(call1215)) {
// Begin line 373
  setline(373);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 372
  setline(372);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1217 == NULL) {
    strlit1217 = alloc_String("next import = ");
    gc_root(strlit1217);
  }
// compilenode returning strlit1217
  int op_slot_left_1216 = gc_frame_newslot(strlit1217);
// compilenode returning *var_nextImport
  int op_slot_right_1216 = gc_frame_newslot(*var_nextImport);
  params[0] = *var_nextImport;
  partcv[0] = 1;
  Object opresult1219 = callmethod(strlit1217, "++", 1, partcv, params);
// compilenode returning opresult1219
  params[0] = opresult1219;
  Object call1220 = gracelib_print(NULL, 1,  params);
// compilenode returning call1220
// Begin line 374
  setline(374);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1221 = gc_frame_new();
// Begin line 373
  setline(373);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_nextImport
  gc_frame_newslot(*var_nextImport);
// compilenode returning *var_toProcess
  params[0] = *var_nextImport;
  partcv[0] = 1;
  Object call1222 = callmethod(*var_toProcess, "push",
    1, partcv, params);
  gc_frame_end(callframe1221);
// compilenode returning call1222
    gc_frame_newslot(call1222);
    if1211 = call1222;
  } else {
  }
// compilenode returning if1211
  gc_frame_end(frame);
  return if1211;
}
Object meth_PackageManager__apply1226(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_count2 = getfromclosure(closure, 0);
  Object *var_line = getfromclosure(closure, 1);
  Object self = *(getfromclosure(closure, 2));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var_count2
  int op_slot_left_1228 = gc_frame_newslot(*var_count2);
  int callframe1229 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1230 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1229);
// compilenode returning call1230
// compilenode returning call1230
  int op_slot_right_1228 = gc_frame_newslot(call1230);
  params[0] = call1230;
  partcv[0] = 1;
  Object opresult1232 = callmethod(*var_count2, "<=", 1, partcv, params);
// compilenode returning opresult1232
  int op_slot_left_1227 = gc_frame_newslot(opresult1232);
// compilenode returning *var_line
// compilenode returning *var_count2
  params[0] = *var_count2;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1234 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1234
  int op_slot_left_1233 = gc_frame_newslot(idxres1234);
  if (strlit1235 == NULL) {
    strlit1235 = alloc_String("Could not retrieve ");
    gc_root(strlit1235);
  }
// compilenode returning strlit1235
  int op_slot_right_1233 = gc_frame_newslot(strlit1235);
  params[0] = strlit1235;
  partcv[0] = 1;
  Object opresult1237 = callmethod(idxres1234, "==", 1, partcv, params);
// compilenode returning opresult1237
  int op_slot_right_1227 = gc_frame_newslot(opresult1237);
  params[0] = opresult1237;
  partcv[0] = 1;
  Object opresult1239 = callmethod(opresult1232, "&&", 1, partcv, params);
// compilenode returning opresult1239
  gc_frame_end(frame);
  return opresult1239;
}
Object meth_PackageManager__apply1242(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_count2 = getfromclosure(closure, 0);
  Object self = *(getfromclosure(closure, 1));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 381
  setline(381);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 380
  setline(380);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count2
  int op_slot_left_1243 = gc_frame_newslot(*var_count2);
  Object num1244 = alloc_Float64(1);
// compilenode returning num1244
  int op_slot_right_1243 = gc_frame_newslot(num1244);
  params[0] = num1244;
  partcv[0] = 1;
  Object sum1246 = callmethod(*var_count2, "+", 1, partcv, params);
// compilenode returning sum1246
  *var_count2 = sum1246;
  if (sum1246 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_skipWhiteSpace1223(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct StackFrameObject *stackframe = alloc_StackFrame(4, NULL);
  pushclosure(NULL);
  pushstackframe(stackframe, "skipWhiteSpace");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 2)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for skipWhiteSpace (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_line = &(stackframe->slots[0]);
  *var_line = args[curarg];
  curarg++;
  Object *var_count = &(stackframe->slots[1]);
  *var_count = args[curarg];
  curarg++;
if (argcv && argcv[0] > 2)
  gracedie("too many arguments for skipWhiteSpace");
  Object *selfslot = &(stackframe->slots[2]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_count2 = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "count2");
// Begin line 379
  setline(379);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 378
  setline(378);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  *var_count2 = *var_count;
  if (*var_count == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 382
  setline(382);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1224 = gc_frame_new();
// Begin line 379
  setline(379);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1225 = alloc_Block(NULL, NULL, "PackageManager", 379);
  gc_frame_newslot(block1225);
  block_savedest(block1225);
  Object closure1226 = createclosure(3, "_apply");
setclosureframe(closure1226, stackframe);
  addtoclosure(closure1226, var_count2);
  addtoclosure(closure1226, var_line);
  addtoclosure(closure1226, selfslot);
  struct UserObject *uo1226 = (struct UserObject*)block1225;
  uo1226->data[0] = (Object)closure1226;
  Method *meth_meth_PackageManager__apply1226 = addmethod2pos(block1225, "_apply", &meth_PackageManager__apply1226, 0);
int argcv_meth_PackageManager__apply1226[] = {0};
meth_meth_PackageManager__apply1226->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1226);
  meth_meth_PackageManager__apply1226->definitionModule = modulename;
  meth_meth_PackageManager__apply1226->definitionLine = 367;
// compilenode returning block1225
  gc_frame_newslot(block1225);
// Begin line 382
  setline(382);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1241 = alloc_Block(NULL, NULL, "PackageManager", 382);
  gc_frame_newslot(block1241);
  block_savedest(block1241);
  Object closure1242 = createclosure(2, "_apply");
setclosureframe(closure1242, stackframe);
  addtoclosure(closure1242, var_count2);
  addtoclosure(closure1242, selfslot);
  struct UserObject *uo1242 = (struct UserObject*)block1241;
  uo1242->data[0] = (Object)closure1242;
  Method *meth_meth_PackageManager__apply1242 = addmethod2pos(block1241, "_apply", &meth_PackageManager__apply1242, 0);
int argcv_meth_PackageManager__apply1242[] = {0};
meth_meth_PackageManager__apply1242->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1242);
  meth_meth_PackageManager__apply1242->definitionModule = modulename;
  meth_meth_PackageManager__apply1242->definitionLine = 379;
// compilenode returning block1241
  gc_frame_newslot(block1241);
  params[0] = block1225;
  params[1] = block1241;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1249 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1224);
// compilenode returning call1249
// Begin line 382
  setline(382);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count2
  gc_frame_end(frame);
  return *var_count2;
}
Object meth_PackageManager_displayHelp1250(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct StackFrameObject *stackframe = alloc_StackFrame(1, NULL);
  pushclosure(NULL);
  pushstackframe(stackframe, "displayHelp");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for displayHelp (probably reflection error): got %i lists, expected 1.", nparts);
if (argcv && argcv[0] > 0)
  gracedie("too many arguments for displayHelp");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 387
  setline(387);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 386
  setline(386);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1251 == NULL) {
    strlit1251 = alloc_String("Available options are:");
    gc_root(strlit1251);
  }
// compilenode returning strlit1251
  params[0] = strlit1251;
  Object call1252 = gracelib_print(NULL, 1,  params);
// compilenode returning call1252
// Begin line 388
  setline(388);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 387
  setline(387);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1253 == NULL) {
    strlit1253 = alloc_String("-get [url] to fetch a package located at a given url");
    gc_root(strlit1253);
  }
// compilenode returning strlit1253
  params[0] = strlit1253;
  Object call1254 = gracelib_print(NULL, 1,  params);
// compilenode returning call1254
// Begin line 389
  setline(389);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 388
  setline(388);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1255 == NULL) {
    strlit1255 = alloc_String("-list to display currenly installed packages");
    gc_root(strlit1255);
  }
// compilenode returning strlit1255
  params[0] = strlit1255;
  Object call1256 = gracelib_print(NULL, 1,  params);
// compilenode returning call1256
  gc_frame_end(frame);
  return call1256;
}
Object meth_PackageManager_printMessage1257(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 20, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "printMessage");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_verbose = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for printMessage (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_message = &(stackframe->slots[0]);
  *var_message = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for printMessage");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 395
  setline(395);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 392
  setline(392);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_verbose
  Object if1258 = done;
  if (istrue(*var_verbose)) {
// Begin line 394
  setline(394);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 393
  setline(393);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_message
  params[0] = *var_message;
  Object call1259 = gracelib_print(NULL, 1,  params);
// compilenode returning call1259
    gc_frame_newslot(call1259);
    if1258 = call1259;
  } else {
  }
// compilenode returning if1258
  gc_frame_end(frame);
  return if1258;
}
Object meth_PackageManager__apply1322(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_imported = getfromclosure(closure, 0);
  Object self = *(getfromclosure(closure, 1));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  int callframe1324 = gc_frame_new();
// compilenode returning *var_imported
  partcv[0] = 0;
  Object call1325 = callmethod(*var_imported, "size",
    1, partcv, params);
  gc_frame_end(callframe1324);
// compilenode returning call1325
// compilenode returning call1325
  int op_slot_left_1323 = gc_frame_newslot(call1325);
  Object num1326 = alloc_Float64(0);
// compilenode returning num1326
  int op_slot_right_1323 = gc_frame_newslot(num1326);
  params[0] = num1326;
  partcv[0] = 1;
  Object opresult1328 = callmethod(call1325, ">", 1, partcv, params);
// compilenode returning opresult1328
  gc_frame_end(frame);
  return opresult1328;
}
Object meth_PackageManager__apply1331(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[2];
  int partcv[1];
  Object *var_imported = getfromclosure(closure, 0);
  Object *var_name = getfromclosure(closure, 1);
  Object *var_io = getfromclosure(closure, 2);
  Object *var_toWrite = getfromclosure(closure, 3);
  Object self = *(getfromclosure(closure, 4));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_curImport = &(stackframe->slots[1]);
  setframeelementname(stackframe, 1, "curImport");
// Begin line 413
  setline(413);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 412
  setline(412);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1332 = gc_frame_new();
// compilenode returning *var_imported
  partcv[0] = 0;
  Object call1333 = callmethod(*var_imported, "pop",
    1, partcv, params);
  gc_frame_end(callframe1332);
// compilenode returning call1333
// compilenode returning call1333
  *var_curImport = call1333;
  if (call1333 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 414
  setline(414);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1334 = gc_frame_new();
// Begin line 413
  setline(413);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1339 == NULL) {
    strlit1339 = alloc_String("");
    gc_root(strlit1339);
  }
// compilenode returning strlit1339
  int op_slot_left_1338 = gc_frame_newslot(strlit1339);
// compilenode returning *var_name
  int op_slot_right_1338 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult1341 = callmethod(strlit1339, "++", 1, partcv, params);
// compilenode returning opresult1341
  int op_slot_left_1337 = gc_frame_newslot(opresult1341);
  if (strlit1342 == NULL) {
    strlit1342 = alloc_String("/");
    gc_root(strlit1342);
  }
// compilenode returning strlit1342
  int op_slot_right_1337 = gc_frame_newslot(strlit1342);
  params[0] = strlit1342;
  partcv[0] = 1;
  Object opresult1344 = callmethod(opresult1341, "++", 1, partcv, params);
// compilenode returning opresult1344
  int op_slot_left_1336 = gc_frame_newslot(opresult1344);
  int callframe1345 = gc_frame_new();
  int callframe1346 = gc_frame_new();
// compilenode returning *var_curImport
  partcv[0] = 0;
  Object call1347 = callmethod(*var_curImport, "address",
    1, partcv, params);
  gc_frame_end(callframe1346);
// compilenode returning call1347
// compilenode returning call1347
  gc_frame_newslot(call1347);
  params[0] = call1347;
  partcv[0] = 1;
  Object call1348 = callmethodflags(self, "removeContainingDir", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1345);
// compilenode returning call1348
  int op_slot_right_1336 = gc_frame_newslot(call1348);
  params[0] = call1348;
  partcv[0] = 1;
  Object opresult1350 = callmethod(opresult1344, "++", 1, partcv, params);
// compilenode returning opresult1350
  int op_slot_left_1335 = gc_frame_newslot(opresult1350);
  if (strlit1351 == NULL) {
    strlit1351 = alloc_String("");
    gc_root(strlit1351);
  }
// compilenode returning strlit1351
  int op_slot_right_1335 = gc_frame_newslot(strlit1351);
  params[0] = strlit1351;
  partcv[0] = 1;
  Object opresult1353 = callmethod(opresult1350, "++", 1, partcv, params);
// compilenode returning opresult1353
  gc_frame_newslot(opresult1353);
  if (strlit1354 == NULL) {
    strlit1354 = alloc_String("w");
    gc_root(strlit1354);
  }
// compilenode returning strlit1354
  gc_frame_newslot(strlit1354);
// compilenode returning *var_io
  params[0] = opresult1353;
  params[1] = strlit1354;
  partcv[0] = 2;
  Object call1355 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1334);
// compilenode returning call1355
  *var_toWrite = call1355;
  if (call1355 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 415
  setline(415);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1357 = gc_frame_new();
// Begin line 414
  setline(414);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1358 = gc_frame_new();
// compilenode returning *var_curImport
  partcv[0] = 0;
  Object call1359 = callmethod(*var_curImport, "data",
    1, partcv, params);
  gc_frame_end(callframe1358);
// compilenode returning call1359
// compilenode returning call1359
  gc_frame_newslot(call1359);
// compilenode returning *var_toWrite
  params[0] = call1359;
  partcv[0] = 1;
  Object call1360 = callmethod(*var_toWrite, "write",
    1, partcv, params);
  gc_frame_end(callframe1357);
// compilenode returning call1360
  gc_frame_end(frame);
  return call1360;
}
Object meth_PackageManager_bundle1260(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 21, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(8, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "bundle");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 2)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  Object *var_imported = getfromclosure(closure, 0);
  Object *var_bundlePath = getfromclosure(closure, 1);
  Object *var_io = getfromclosure(closure, 2);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for bundle (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_toBundle = &(stackframe->slots[0]);
  *var_toBundle = args[curarg];
  curarg++;
  Object *var_name = &(stackframe->slots[1]);
  *var_name = args[curarg];
  curarg++;
if (argcv && argcv[0] > 2)
  gracedie("too many arguments for bundle");
  Object *selfslot = &(stackframe->slots[2]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_newDir = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "newDir");
  Object *var_newFileName = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "newFileName");
  Object *var_toOpen = &(stackframe->slots[5]);
  setframeelementname(stackframe, 5, "toOpen");
  Object *var_openData = &(stackframe->slots[6]);
  setframeelementname(stackframe, 6, "openData");
  Object *var_toWrite = &(stackframe->slots[7]);
  setframeelementname(stackframe, 7, "toWrite");
// Begin line 402
  setline(402);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 401
  setline(401);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array1261 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array1261
  *var_imported = array1261;
  if (array1261 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 402
  setline(402);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1263 = gc_frame_new();
// compilenode returning *var_toBundle
  gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object call1264 = callmethodflags(self, "getContainingDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1263);
// compilenode returning call1264
  *var_bundlePath = call1264;
  if (call1264 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 404
  setline(404);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1266 = gc_frame_new();
// Begin line 403
  setline(403);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1269 == NULL) {
    strlit1269 = alloc_String("");
    gc_root(strlit1269);
  }
// compilenode returning strlit1269
  int op_slot_left_1268 = gc_frame_newslot(strlit1269);
// compilenode returning *var_name
  int op_slot_right_1268 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult1271 = callmethod(strlit1269, "++", 1, partcv, params);
// compilenode returning opresult1271
  int op_slot_left_1267 = gc_frame_newslot(opresult1271);
  if (strlit1272 == NULL) {
    strlit1272 = alloc_String("/");
    gc_root(strlit1272);
  }
// compilenode returning strlit1272
  int op_slot_right_1267 = gc_frame_newslot(strlit1272);
  params[0] = strlit1272;
  partcv[0] = 1;
  Object opresult1274 = callmethod(opresult1271, "++", 1, partcv, params);
// compilenode returning opresult1274
  gc_frame_newslot(opresult1274);
  params[0] = opresult1274;
  partcv[0] = 1;
  Object call1275 = callmethodflags(self, "createDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1266);
// compilenode returning call1275
  *var_newDir = call1275;
  if (call1275 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 404
  setline(404);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1276 = gc_frame_new();
// compilenode returning *var_toBundle
  gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object call1277 = callmethodflags(self, "removeContainingDir", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1276);
// compilenode returning call1277
  *var_newFileName = call1277;
  if (call1277 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 405
  setline(405);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1278 = gc_frame_new();
  if (strlit1281 == NULL) {
    strlit1281 = alloc_String("");
    gc_root(strlit1281);
  }
// compilenode returning strlit1281
  int op_slot_left_1280 = gc_frame_newslot(strlit1281);
// compilenode returning *var_toBundle
  int op_slot_right_1280 = gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object opresult1283 = callmethod(strlit1281, "++", 1, partcv, params);
// compilenode returning opresult1283
  int op_slot_left_1279 = gc_frame_newslot(opresult1283);
  if (strlit1284 == NULL) {
    strlit1284 = alloc_String("");
    gc_root(strlit1284);
  }
// compilenode returning strlit1284
  int op_slot_right_1279 = gc_frame_newslot(strlit1284);
  params[0] = strlit1284;
  partcv[0] = 1;
  Object opresult1286 = callmethod(opresult1283, "++", 1, partcv, params);
// compilenode returning opresult1286
  gc_frame_newslot(opresult1286);
  if (strlit1287 == NULL) {
    strlit1287 = alloc_String("r");
    gc_root(strlit1287);
  }
// compilenode returning strlit1287
  gc_frame_newslot(strlit1287);
// compilenode returning *var_io
  params[0] = opresult1286;
  params[1] = strlit1287;
  partcv[0] = 2;
  Object call1288 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1278);
// compilenode returning call1288
  *var_toOpen = call1288;
  if (call1288 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 407
  setline(407);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 406
  setline(406);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1289 = gc_frame_new();
// compilenode returning *var_toOpen
  partcv[0] = 0;
  Object call1290 = callmethod(*var_toOpen, "read",
    1, partcv, params);
  gc_frame_end(callframe1289);
// compilenode returning call1290
// compilenode returning call1290
  *var_openData = call1290;
  if (call1290 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 408
  setline(408);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1291 = gc_frame_new();
// Begin line 407
  setline(407);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1296 == NULL) {
    strlit1296 = alloc_String("");
    gc_root(strlit1296);
  }
// compilenode returning strlit1296
  int op_slot_left_1295 = gc_frame_newslot(strlit1296);
// compilenode returning *var_name
  int op_slot_right_1295 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult1298 = callmethod(strlit1296, "++", 1, partcv, params);
// compilenode returning opresult1298
  int op_slot_left_1294 = gc_frame_newslot(opresult1298);
  if (strlit1299 == NULL) {
    strlit1299 = alloc_String("/");
    gc_root(strlit1299);
  }
// compilenode returning strlit1299
  int op_slot_right_1294 = gc_frame_newslot(strlit1299);
  params[0] = strlit1299;
  partcv[0] = 1;
  Object opresult1301 = callmethod(opresult1298, "++", 1, partcv, params);
// compilenode returning opresult1301
  int op_slot_left_1293 = gc_frame_newslot(opresult1301);
// compilenode returning *var_newFileName
  int op_slot_right_1293 = gc_frame_newslot(*var_newFileName);
  params[0] = *var_newFileName;
  partcv[0] = 1;
  Object opresult1303 = callmethod(opresult1301, "++", 1, partcv, params);
// compilenode returning opresult1303
  int op_slot_left_1292 = gc_frame_newslot(opresult1303);
  if (strlit1304 == NULL) {
    strlit1304 = alloc_String("");
    gc_root(strlit1304);
  }
// compilenode returning strlit1304
  int op_slot_right_1292 = gc_frame_newslot(strlit1304);
  params[0] = strlit1304;
  partcv[0] = 1;
  Object opresult1306 = callmethod(opresult1303, "++", 1, partcv, params);
// compilenode returning opresult1306
  gc_frame_newslot(opresult1306);
  if (strlit1307 == NULL) {
    strlit1307 = alloc_String("w");
    gc_root(strlit1307);
  }
// compilenode returning strlit1307
  gc_frame_newslot(strlit1307);
// compilenode returning *var_io
  params[0] = opresult1306;
  params[1] = strlit1307;
  partcv[0] = 2;
  Object call1308 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1291);
// compilenode returning call1308
  *var_toWrite = call1308;
  if (call1308 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 408
  setline(408);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1309 = gc_frame_new();
// compilenode returning *var_openData
  gc_frame_newslot(*var_openData);
// compilenode returning *var_toWrite
  params[0] = *var_openData;
  partcv[0] = 1;
  Object call1310 = callmethod(*var_toWrite, "write",
    1, partcv, params);
  gc_frame_end(callframe1309);
// compilenode returning call1310
// Begin line 410
  setline(410);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1311 = gc_frame_new();
// Begin line 409
  setline(409);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toBundle
  gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object call1312 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1311);
// compilenode returning call1312
// Begin line 410
  setline(410);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1314 == NULL) {
    strlit1314 = alloc_String("Imported size = ");
    gc_root(strlit1314);
  }
// compilenode returning strlit1314
  int op_slot_left_1313 = gc_frame_newslot(strlit1314);
  int callframe1315 = gc_frame_new();
// compilenode returning *var_imported
  partcv[0] = 0;
  Object call1316 = callmethod(*var_imported, "size",
    1, partcv, params);
  gc_frame_end(callframe1315);
// compilenode returning call1316
// compilenode returning call1316
  int op_slot_right_1313 = gc_frame_newslot(call1316);
  params[0] = call1316;
  partcv[0] = 1;
  Object opresult1318 = callmethod(strlit1314, "++", 1, partcv, params);
// compilenode returning opresult1318
  params[0] = opresult1318;
  Object call1319 = gracelib_print(NULL, 1,  params);
// compilenode returning call1319
// Begin line 416
  setline(416);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1320 = gc_frame_new();
// Begin line 411
  setline(411);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1321 = alloc_Block(NULL, NULL, "PackageManager", 411);
  gc_frame_newslot(block1321);
  block_savedest(block1321);
  Object closure1322 = createclosure(2, "_apply");
setclosureframe(closure1322, stackframe);
  addtoclosure(closure1322, var_imported);
  addtoclosure(closure1322, selfslot);
  struct UserObject *uo1322 = (struct UserObject*)block1321;
  uo1322->data[0] = (Object)closure1322;
  Method *meth_meth_PackageManager__apply1322 = addmethod2pos(block1321, "_apply", &meth_PackageManager__apply1322, 0);
int argcv_meth_PackageManager__apply1322[] = {0};
meth_meth_PackageManager__apply1322->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1322);
  meth_meth_PackageManager__apply1322->definitionModule = modulename;
  meth_meth_PackageManager__apply1322->definitionLine = 410;
// compilenode returning block1321
  gc_frame_newslot(block1321);
// Begin line 416
  setline(416);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1330 = alloc_Block(NULL, NULL, "PackageManager", 416);
  gc_frame_newslot(block1330);
  block_savedest(block1330);
  Object closure1331 = createclosure(5, "_apply");
setclosureframe(closure1331, stackframe);
  addtoclosure(closure1331, var_imported);
  addtoclosure(closure1331, var_name);
  addtoclosure(closure1331, var_io);
  addtoclosure(closure1331, var_toWrite);
  addtoclosure(closure1331, selfslot);
  struct UserObject *uo1331 = (struct UserObject*)block1330;
  uo1331->data[0] = (Object)closure1331;
  Method *meth_meth_PackageManager__apply1331 = addmethod2pos(block1330, "_apply", &meth_PackageManager__apply1331, 0);
int argcv_meth_PackageManager__apply1331[] = {0};
meth_meth_PackageManager__apply1331->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1331);
  meth_meth_PackageManager__apply1331->definitionModule = modulename;
  meth_meth_PackageManager__apply1331->definitionLine = 411;
// compilenode returning block1330
  gc_frame_newslot(block1330);
  params[0] = block1321;
  params[1] = block1330;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1362 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1320);
// compilenode returning call1362
  gc_frame_end(frame);
  return call1362;
}
Object meth_PackageManager__apply1432(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_out = getfromclosure(closure, 0);
  Object self = *(getfromclosure(closure, 1));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *var_d = &(stackframe->slots[0]);
  *var_d = args[curarg];
  curarg++;
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 431
  setline(431);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1433 = gc_frame_new();
// compilenode returning *var_d
  gc_frame_newslot(*var_d);
// compilenode returning *var_out
  params[0] = *var_d;
  partcv[0] = 1;
  Object call1434 = callmethod(*var_out, "write",
    1, partcv, params);
  gc_frame_end(callframe1433);
// compilenode returning call1434
  gc_frame_end(frame);
  return call1434;
}
Object meth_PackageManager__apply1368(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(6, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[3];
  int partcv[2];
  Object *var_address = getfromclosure(closure, 0);
  Object *var_io = getfromclosure(closure, 1);
  Object *var_prefix = getfromclosure(closure, 2);
  Object self = *(getfromclosure(closure, 3));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *var_file = &(stackframe->slots[0]);
  *var_file = args[curarg];
  curarg++;
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_open = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "open");
  Object *var_readFile = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "readFile");
  Object *var_toWrite = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "toWrite");
  Object *var_out = &(stackframe->slots[5]);
  setframeelementname(stackframe, 5, "out");
// Begin line 435
  setline(435);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 421
  setline(421);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1371 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1372 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1371);
// compilenode returning call1372
// compilenode returning call1372
  int op_slot_left_1370 = gc_frame_newslot(call1372);
  Object num1373 = alloc_Float64(4);
// compilenode returning num1373
  int op_slot_right_1370 = gc_frame_newslot(num1373);
  params[0] = num1373;
  partcv[0] = 1;
  Object opresult1375 = callmethod(call1372, ">", 1, partcv, params);
// compilenode returning opresult1375
  Object if1369 = done;
  if (istrue(opresult1375)) {
// Begin line 434
  setline(434);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 422
  setline(422);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1378 = gc_frame_new();
  int callframe1380 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1381 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1380);
// compilenode returning call1381
// compilenode returning call1381
  int op_slot_left_1379 = gc_frame_newslot(call1381);
  Object num1382 = alloc_Float64(5);
// compilenode returning num1382
  int op_slot_right_1379 = gc_frame_newslot(num1382);
  params[0] = num1382;
  partcv[0] = 1;
  Object diff1384 = callmethod(call1381, "-", 1, partcv, params);
// compilenode returning diff1384
  gc_frame_newslot(diff1384);
  int callframe1385 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1386 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1385);
// compilenode returning call1386
// compilenode returning call1386
  gc_frame_newslot(call1386);
// compilenode returning *var_file
  params[0] = diff1384;
  params[1] = call1386;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1387 = callmethod(*var_file, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1378);
// compilenode returning call1387
  int op_slot_left_1377 = gc_frame_newslot(call1387);
  if (strlit1388 == NULL) {
    strlit1388 = alloc_String(".grace");
    gc_root(strlit1388);
  }
// compilenode returning strlit1388
  int op_slot_right_1377 = gc_frame_newslot(strlit1388);
  params[0] = strlit1388;
  partcv[0] = 1;
  Object opresult1390 = callmethod(call1387, "==", 1, partcv, params);
// compilenode returning opresult1390
  Object if1376 = done;
  if (istrue(opresult1390)) {
// Begin line 423
  setline(423);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1391 = gc_frame_new();
// compilenode returning *var_address
  int op_slot_left_1393 = gc_frame_newslot(*var_address);
  if (strlit1394 == NULL) {
    strlit1394 = alloc_String("/");
    gc_root(strlit1394);
  }
// compilenode returning strlit1394
  int op_slot_right_1393 = gc_frame_newslot(strlit1394);
  params[0] = strlit1394;
  partcv[0] = 1;
  Object opresult1396 = callmethod(*var_address, "++", 1, partcv, params);
// compilenode returning opresult1396
  int op_slot_left_1392 = gc_frame_newslot(opresult1396);
// compilenode returning *var_file
  int op_slot_right_1392 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1398 = callmethod(opresult1396, "++", 1, partcv, params);
// compilenode returning opresult1398
  gc_frame_newslot(opresult1398);
  if (strlit1399 == NULL) {
    strlit1399 = alloc_String("r");
    gc_root(strlit1399);
  }
// compilenode returning strlit1399
  gc_frame_newslot(strlit1399);
// compilenode returning *var_io
  params[0] = opresult1398;
  params[1] = strlit1399;
  partcv[0] = 2;
  Object call1400 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1391);
// compilenode returning call1400
  *var_open = call1400;
  if (call1400 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 424
  setline(424);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1401 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call1402 = callmethod(*var_open, "read",
    1, partcv, params);
  gc_frame_end(callframe1401);
// compilenode returning call1402
// compilenode returning call1402
  *var_readFile = call1402;
  if (call1402 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 425
  setline(425);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1403 = gc_frame_new();
// compilenode returning *var_readFile
  gc_frame_newslot(*var_readFile);
// compilenode returning *var_address
  gc_frame_newslot(*var_address);
// compilenode returning *var_prefix
  gc_frame_newslot(*var_prefix);
  params[0] = *var_readFile;
  params[1] = *var_address;
  params[2] = *var_prefix;
  partcv[0] = 3;
  Object call1404 = callmethodflags(self, "parseAndPrefix", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1403);
// compilenode returning call1404
  *var_toWrite = call1404;
  if (call1404 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 426
  setline(426);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1405 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call1406 = callmethod(*var_open, "close",
    1, partcv, params);
  gc_frame_end(callframe1405);
// compilenode returning call1406
// compilenode returning call1406
// Begin line 429
  setline(429);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1407 = gc_frame_new();
// Begin line 428
  setline(428);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1411 == NULL) {
    strlit1411 = alloc_String("rm ");
    gc_root(strlit1411);
  }
// compilenode returning strlit1411
  int op_slot_left_1410 = gc_frame_newslot(strlit1411);
// compilenode returning *var_address
  int op_slot_right_1410 = gc_frame_newslot(*var_address);
  params[0] = *var_address;
  partcv[0] = 1;
  Object opresult1413 = callmethod(strlit1411, "++", 1, partcv, params);
// compilenode returning opresult1413
  int op_slot_left_1409 = gc_frame_newslot(opresult1413);
  if (strlit1414 == NULL) {
    strlit1414 = alloc_String("/");
    gc_root(strlit1414);
  }
// compilenode returning strlit1414
  int op_slot_right_1409 = gc_frame_newslot(strlit1414);
  params[0] = strlit1414;
  partcv[0] = 1;
  Object opresult1416 = callmethod(opresult1413, "++", 1, partcv, params);
// compilenode returning opresult1416
  int op_slot_left_1408 = gc_frame_newslot(opresult1416);
// compilenode returning *var_file
  int op_slot_right_1408 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1418 = callmethod(opresult1416, "++", 1, partcv, params);
// compilenode returning opresult1418
  gc_frame_newslot(opresult1418);
// compilenode returning *var_io
  params[0] = opresult1418;
  partcv[0] = 1;
  Object call1419 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe1407);
// compilenode returning call1419
// Begin line 429
  setline(429);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1420 = gc_frame_new();
// compilenode returning *var_address
  int op_slot_left_1422 = gc_frame_newslot(*var_address);
  if (strlit1423 == NULL) {
    strlit1423 = alloc_String("/");
    gc_root(strlit1423);
  }
// compilenode returning strlit1423
  int op_slot_right_1422 = gc_frame_newslot(strlit1423);
  params[0] = strlit1423;
  partcv[0] = 1;
  Object opresult1425 = callmethod(*var_address, "++", 1, partcv, params);
// compilenode returning opresult1425
  int op_slot_left_1421 = gc_frame_newslot(opresult1425);
// compilenode returning *var_file
  int op_slot_right_1421 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1427 = callmethod(opresult1425, "++", 1, partcv, params);
// compilenode returning opresult1427
  gc_frame_newslot(opresult1427);
  if (strlit1428 == NULL) {
    strlit1428 = alloc_String("w");
    gc_root(strlit1428);
  }
// compilenode returning strlit1428
  gc_frame_newslot(strlit1428);
// compilenode returning *var_io
  params[0] = opresult1427;
  params[1] = strlit1428;
  partcv[0] = 2;
  Object call1429 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1420);
// compilenode returning call1429
  *var_out = call1429;
  if (call1429 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 433
  setline(433);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1430 = gc_frame_new();
// Begin line 430
  setline(430);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toWrite
  gc_frame_newslot(*var_toWrite);
// Begin line 433
  setline(433);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1431 = alloc_Block(NULL, NULL, "PackageManager", 433);
  gc_frame_newslot(block1431);
  block_savedest(block1431);
  Object closure1432 = createclosure(2, "_apply");
setclosureframe(closure1432, stackframe);
  addtoclosure(closure1432, var_out);
  addtoclosure(closure1432, selfslot);
  struct UserObject *uo1432 = (struct UserObject*)block1431;
  uo1432->data[0] = (Object)closure1432;
  Method *meth_meth_PackageManager__apply1432 = addmethod2pos(block1431, "_apply", &meth_PackageManager__apply1432, 0);
int argcv_meth_PackageManager__apply1432[] = {1};
meth_meth_PackageManager__apply1432->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1432);
  meth_meth_PackageManager__apply1432->definitionModule = modulename;
  meth_meth_PackageManager__apply1432->definitionLine = 426;
// compilenode returning block1431
  gc_frame_newslot(block1431);
  params[0] = *var_toWrite;
  params[1] = block1431;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1436 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1430);
// compilenode returning call1436
    gc_frame_newslot(call1436);
    if1376 = call1436;
  } else {
  }
// compilenode returning if1376
    gc_frame_newslot(if1376);
    if1369 = if1376;
  } else {
  }
// compilenode returning if1369
  gc_frame_end(frame);
  return if1369;
}
Object meth_PackageManager_setImportDest1363(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 22, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(4, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "setImportDest");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 2)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  Object *var_io = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for setImportDest (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_address = &(stackframe->slots[0]);
  *var_address = args[curarg];
  curarg++;
  Object *var_prefix = &(stackframe->slots[1]);
  *var_prefix = args[curarg];
  curarg++;
if (argcv && argcv[0] > 2)
  gracedie("too many arguments for setImportDest");
  Object *selfslot = &(stackframe->slots[2]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_folder = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "folder");
// Begin line 419
  setline(419);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1364 = gc_frame_new();
// compilenode returning *var_address
  gc_frame_newslot(*var_address);
// compilenode returning *var_io
  params[0] = *var_address;
  partcv[0] = 1;
  Object call1365 = callmethod(*var_io, "getdir",
    1, partcv, params);
  gc_frame_end(callframe1364);
// compilenode returning call1365
  *var_folder = call1365;
  if (call1365 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 439
  setline(439);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1366 = gc_frame_new();
// Begin line 420
  setline(420);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_folder
  gc_frame_newslot(*var_folder);
// Begin line 439
  setline(439);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1367 = alloc_Block(NULL, NULL, "PackageManager", 439);
  gc_frame_newslot(block1367);
  block_savedest(block1367);
  Object closure1368 = createclosure(4, "_apply");
setclosureframe(closure1368, stackframe);
  addtoclosure(closure1368, var_address);
  addtoclosure(closure1368, var_io);
  addtoclosure(closure1368, var_prefix);
  addtoclosure(closure1368, selfslot);
  struct UserObject *uo1368 = (struct UserObject*)block1367;
  uo1368->data[0] = (Object)closure1368;
  Method *meth_meth_PackageManager__apply1368 = addmethod2pos(block1367, "_apply", &meth_PackageManager__apply1368, 0);
int argcv_meth_PackageManager__apply1368[] = {1};
meth_meth_PackageManager__apply1368->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1368);
  meth_meth_PackageManager__apply1368->definitionModule = modulename;
  meth_meth_PackageManager__apply1368->definitionLine = 414;
// compilenode returning block1367
  gc_frame_newslot(block1367);
  params[0] = *var_folder;
  params[1] = block1367;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1438 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1366);
// compilenode returning call1438
  gc_frame_end(frame);
  return call1438;
}
Object meth_PackageManager__apply1453(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_curPos = getfromclosure(closure, 0);
  Object *var_readFile = getfromclosure(closure, 1);
  Object self = *(getfromclosure(closure, 2));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var_curPos
  int op_slot_left_1454 = gc_frame_newslot(*var_curPos);
  int callframe1455 = gc_frame_new();
// compilenode returning *var_readFile
  partcv[0] = 0;
  Object call1456 = callmethod(*var_readFile, "size",
    1, partcv, params);
  gc_frame_end(callframe1455);
// compilenode returning call1456
// compilenode returning call1456
  int op_slot_right_1454 = gc_frame_newslot(call1456);
  params[0] = call1456;
  partcv[0] = 1;
  Object opresult1458 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1458
  gc_frame_end(frame);
  return opresult1458;
}
Object meth_PackageManager__apply1465(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_curPos = getfromclosure(closure, 0);
  Object *var_readFile = getfromclosure(closure, 1);
  Object self = *(getfromclosure(closure, 2));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var_curPos
  int op_slot_left_1467 = gc_frame_newslot(*var_curPos);
  int callframe1468 = gc_frame_new();
// compilenode returning *var_readFile
  partcv[0] = 0;
  Object call1469 = callmethod(*var_readFile, "size",
    1, partcv, params);
  gc_frame_end(callframe1468);
// compilenode returning call1469
// compilenode returning call1469
  int op_slot_right_1467 = gc_frame_newslot(call1469);
  params[0] = call1469;
  partcv[0] = 1;
  Object opresult1471 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1471
  int op_slot_left_1466 = gc_frame_newslot(opresult1471);
// compilenode returning *var_readFile
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1473 = callmethod(*var_readFile, "[]", 1, partcv, params);
// compilenode returning idxres1473
  int op_slot_left_1472 = gc_frame_newslot(idxres1473);
  if (strlit1474 == NULL) {
    strlit1474 = alloc_String("""\x0a""");
    gc_root(strlit1474);
  }
// compilenode returning strlit1474
  int op_slot_right_1472 = gc_frame_newslot(strlit1474);
  params[0] = strlit1474;
  partcv[0] = 1;
  Object opresult1476 = callmethod(idxres1473, "!=", 1, partcv, params);
// compilenode returning opresult1476
  int op_slot_right_1466 = gc_frame_newslot(opresult1476);
  params[0] = opresult1476;
  partcv[0] = 1;
  Object opresult1478 = callmethod(opresult1471, "&&", 1, partcv, params);
// compilenode returning opresult1478
  gc_frame_end(frame);
  return opresult1478;
}
Object meth_PackageManager__apply1481(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_curPos = getfromclosure(closure, 0);
  Object self = *(getfromclosure(closure, 1));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 450
  setline(450);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1482 = gc_frame_newslot(*var_curPos);
  Object num1483 = alloc_Float64(1);
// compilenode returning num1483
  int op_slot_right_1482 = gc_frame_newslot(num1483);
  params[0] = num1483;
  partcv[0] = 1;
  Object sum1485 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1485
  *var_curPos = sum1485;
  if (sum1485 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1506(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_line = getfromclosure(closure, 0);
  Object *var_readFile = getfromclosure(closure, 1);
  Object self = *(getfromclosure(closure, 2));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *var_b = &(stackframe->slots[0]);
  *var_b = args[curarg];
  curarg++;
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 456
  setline(456);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  int op_slot_left_1507 = gc_frame_newslot(*var_line);
// compilenode returning *var_readFile
// compilenode returning *var_b
  params[0] = *var_b;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1508 = callmethod(*var_readFile, "[]", 1, partcv, params);
// compilenode returning idxres1508
  int op_slot_right_1507 = gc_frame_newslot(idxres1508);
  params[0] = idxres1508;
  partcv[0] = 1;
  Object opresult1510 = callmethod(*var_line, "++", 1, partcv, params);
// compilenode returning opresult1510
  *var_line = opresult1510;
  if (opresult1510 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1550(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_pos = getfromclosure(closure, 0);
  Object *var_line = getfromclosure(closure, 1);
  Object self = *(getfromclosure(closure, 2));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var_pos
  int op_slot_left_1552 = gc_frame_newslot(*var_pos);
  int callframe1553 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1554 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1553);
// compilenode returning call1554
// compilenode returning call1554
  int op_slot_right_1552 = gc_frame_newslot(call1554);
  params[0] = call1554;
  partcv[0] = 1;
  Object opresult1556 = callmethod(*var_pos, "<", 1, partcv, params);
// compilenode returning opresult1556
  int op_slot_left_1551 = gc_frame_newslot(opresult1556);
  int callframe1557 = gc_frame_new();
// compilenode returning *var_line
// compilenode returning *var_pos
  params[0] = *var_pos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1560 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1560
  int op_slot_left_1559 = gc_frame_newslot(idxres1560);
  if (strlit1561 == NULL) {
    strlit1561 = alloc_String("""\x22""");
    gc_root(strlit1561);
  }
// compilenode returning strlit1561
  int op_slot_right_1559 = gc_frame_newslot(strlit1561);
  params[0] = strlit1561;
  partcv[0] = 1;
  Object opresult1563 = callmethod(idxres1560, "==", 1, partcv, params);
// compilenode returning opresult1563
  int op_slot_left_1558 = gc_frame_newslot(opresult1563);
// compilenode returning *var_line
// compilenode returning *var_pos
  params[0] = *var_pos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1565 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1565
  int op_slot_left_1564 = gc_frame_newslot(idxres1565);
  if (strlit1566 == NULL) {
    strlit1566 = alloc_String(" ");
    gc_root(strlit1566);
  }
// compilenode returning strlit1566
  int op_slot_right_1564 = gc_frame_newslot(strlit1566);
  params[0] = strlit1566;
  partcv[0] = 1;
  Object opresult1568 = callmethod(idxres1565, "==", 1, partcv, params);
// compilenode returning opresult1568
  int op_slot_right_1558 = gc_frame_newslot(opresult1568);
  params[0] = opresult1568;
  partcv[0] = 1;
  Object opresult1570 = callmethod(opresult1563, "||", 1, partcv, params);
// compilenode returning opresult1570
  partcv[0] = 0;
  Object call1571 = callmethod(opresult1570, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1557);
// compilenode returning call1571
  int op_slot_right_1551 = gc_frame_newslot(call1571);
  params[0] = call1571;
  partcv[0] = 1;
  Object opresult1573 = callmethod(opresult1556, "&&", 1, partcv, params);
// compilenode returning opresult1573
  gc_frame_end(frame);
  return opresult1573;
}
Object meth_PackageManager__apply1576(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_pos = getfromclosure(closure, 0);
  Object self = *(getfromclosure(closure, 1));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 468
  setline(468);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 467
  setline(467);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  int op_slot_left_1577 = gc_frame_newslot(*var_pos);
  Object num1578 = alloc_Float64(1);
// compilenode returning num1578
  int op_slot_right_1577 = gc_frame_newslot(num1578);
  params[0] = num1578;
  partcv[0] = 1;
  Object sum1580 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1580
  *var_pos = sum1580;
  if (sum1580 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1614(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_outFile = getfromclosure(closure, 0);
  Object self = *(getfromclosure(closure, 1));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *var_d = &(stackframe->slots[0]);
  *var_d = args[curarg];
  curarg++;
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 474
  setline(474);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1615 = gc_frame_new();
// compilenode returning *var_d
  gc_frame_newslot(*var_d);
// compilenode returning *var_outFile
  params[0] = *var_d;
  partcv[0] = 1;
  Object call1616 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1615);
// compilenode returning call1616
  gc_frame_end(frame);
  return call1616;
}
Object meth_PackageManager__apply1621(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_outFile = getfromclosure(closure, 0);
  Object self = *(getfromclosure(closure, 1));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *var_d = &(stackframe->slots[0]);
  *var_d = args[curarg];
  curarg++;
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 479
  setline(479);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1622 = gc_frame_new();
// compilenode returning *var_d
  gc_frame_newslot(*var_d);
// compilenode returning *var_outFile
  params[0] = *var_d;
  partcv[0] = 1;
  Object call1623 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1622);
// compilenode returning call1623
  gc_frame_end(frame);
  return call1623;
}
Object meth_PackageManager__apply1461(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(8, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[2];
  int partcv[2];
  Object *var_curPos = getfromclosure(closure, 0);
  Object *var_startPos = getfromclosure(closure, 1);
  Object *var_readFile = getfromclosure(closure, 2);
  Object *var_prefix = getfromclosure(closure, 3);
  Object *var_outFile = getfromclosure(closure, 4);
  Object self = *(getfromclosure(closure, 5));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_data = &(stackframe->slots[1]);
  setframeelementname(stackframe, 1, "data");
  Object *var_line = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "line");
  Object *var_count = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "count");
  Object *var_pos = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "pos");
  Object *var_start = &(stackframe->slots[5]);
  setframeelementname(stackframe, 5, "start");
  Object *var_nextImport = &(stackframe->slots[6]);
  setframeelementname(stackframe, 6, "nextImport");
  Object *var_remainder = &(stackframe->slots[7]);
  setframeelementname(stackframe, 7, "remainder");
// Begin line 449
  setline(449);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 448
  setline(448);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 452
  setline(452);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1463 = gc_frame_new();
// Begin line 449
  setline(449);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1464 = alloc_Block(NULL, NULL, "PackageManager", 449);
  gc_frame_newslot(block1464);
  block_savedest(block1464);
  Object closure1465 = createclosure(3, "_apply");
setclosureframe(closure1465, stackframe);
  addtoclosure(closure1465, var_curPos);
  addtoclosure(closure1465, var_readFile);
  addtoclosure(closure1465, selfslot);
  struct UserObject *uo1465 = (struct UserObject*)block1464;
  uo1465->data[0] = (Object)closure1465;
  Method *meth_meth_PackageManager__apply1465 = addmethod2pos(block1464, "_apply", &meth_PackageManager__apply1465, 0);
int argcv_meth_PackageManager__apply1465[] = {0};
meth_meth_PackageManager__apply1465->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1465);
  meth_meth_PackageManager__apply1465->definitionModule = modulename;
  meth_meth_PackageManager__apply1465->definitionLine = 447;
// compilenode returning block1464
  gc_frame_newslot(block1464);
// Begin line 452
  setline(452);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1480 = alloc_Block(NULL, NULL, "PackageManager", 452);
  gc_frame_newslot(block1480);
  block_savedest(block1480);
  Object closure1481 = createclosure(2, "_apply");
setclosureframe(closure1481, stackframe);
  addtoclosure(closure1481, var_curPos);
  addtoclosure(closure1481, selfslot);
  struct UserObject *uo1481 = (struct UserObject*)block1480;
  uo1481->data[0] = (Object)closure1481;
  Method *meth_meth_PackageManager__apply1481 = addmethod2pos(block1480, "_apply", &meth_PackageManager__apply1481, 0);
int argcv_meth_PackageManager__apply1481[] = {0};
meth_meth_PackageManager__apply1481->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1481);
  meth_meth_PackageManager__apply1481->definitionModule = modulename;
  meth_meth_PackageManager__apply1481->definitionLine = 449;
// compilenode returning block1480
  gc_frame_newslot(block1480);
  params[0] = block1464;
  params[1] = block1480;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1488 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1463);
// compilenode returning call1488
// Begin line 453
  setline(453);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1489 = gc_frame_new();
// Begin line 452
  setline(452);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_startPos
  gc_frame_newslot(*var_startPos);
// compilenode returning *var_curPos
  int op_slot_left_1490 = gc_frame_newslot(*var_curPos);
  Object num1491 = alloc_Float64(1);
// compilenode returning num1491
  int op_slot_right_1490 = gc_frame_newslot(num1491);
  params[0] = num1491;
  partcv[0] = 1;
  Object diff1493 = callmethod(*var_curPos, "-", 1, partcv, params);
// compilenode returning diff1493
  gc_frame_newslot(diff1493);
// compilenode returning *var_readFile
  params[0] = *var_startPos;
  params[1] = diff1493;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1494 = callmethod(*var_readFile, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1489);
// compilenode returning call1494
  *var_data = call1494;
  if (call1494 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 453
  setline(453);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1495 == NULL) {
    strlit1495 = alloc_String("");
    gc_root(strlit1495);
  }
// compilenode returning strlit1495
  *var_line = strlit1495;
  if (strlit1495 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 454
  setline(454);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1496 = alloc_Float64(1);
// compilenode returning num1496
  *var_count = num1496;
  if (num1496 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 458
  setline(458);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1497 = gc_frame_new();
// Begin line 455
  setline(455);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_startPos
  int op_slot_left_1498 = gc_frame_newslot(*var_startPos);
// compilenode returning *var_curPos
  int op_slot_left_1499 = gc_frame_newslot(*var_curPos);
  Object num1500 = alloc_Float64(1);
// compilenode returning num1500
  int op_slot_right_1499 = gc_frame_newslot(num1500);
  params[0] = num1500;
  partcv[0] = 1;
  Object diff1502 = callmethod(*var_curPos, "-", 1, partcv, params);
// compilenode returning diff1502
  int op_slot_right_1498 = gc_frame_newslot(diff1502);
  params[0] = diff1502;
  partcv[0] = 1;
  Object opresult1504 = callmethod(*var_startPos, "..", 1, partcv, params);
// compilenode returning opresult1504
  gc_frame_newslot(opresult1504);
// Begin line 458
  setline(458);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1505 = alloc_Block(NULL, NULL, "PackageManager", 458);
  gc_frame_newslot(block1505);
  block_savedest(block1505);
  Object closure1506 = createclosure(3, "_apply");
setclosureframe(closure1506, stackframe);
  addtoclosure(closure1506, var_line);
  addtoclosure(closure1506, var_readFile);
  addtoclosure(closure1506, selfslot);
  struct UserObject *uo1506 = (struct UserObject*)block1505;
  uo1506->data[0] = (Object)closure1506;
  Method *meth_meth_PackageManager__apply1506 = addmethod2pos(block1505, "_apply", &meth_PackageManager__apply1506, 0);
int argcv_meth_PackageManager__apply1506[] = {1};
meth_meth_PackageManager__apply1506->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1506);
  meth_meth_PackageManager__apply1506->definitionModule = modulename;
  meth_meth_PackageManager__apply1506->definitionLine = 449;
// compilenode returning block1505
  gc_frame_newslot(block1505);
  params[0] = opresult1504;
  params[1] = block1505;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1513 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1497);
// compilenode returning call1513
// Begin line 458
  setline(458);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  params[0] = *var_line;
  Object call1514 = gracelib_print(NULL, 1,  params);
// compilenode returning call1514
// Begin line 482
  setline(482);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 459
  setline(459);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1518 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1519 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1518);
// compilenode returning call1519
// compilenode returning call1519
  int op_slot_left_1517 = gc_frame_newslot(call1519);
  Object num1520 = alloc_Float64(6);
// compilenode returning num1520
  int op_slot_right_1517 = gc_frame_newslot(num1520);
  params[0] = num1520;
  partcv[0] = 1;
  Object opresult1522 = callmethod(call1519, ">", 1, partcv, params);
// compilenode returning opresult1522
  int op_slot_left_1516 = gc_frame_newslot(opresult1522);
  int callframe1524 = gc_frame_new();
  Object num1525 = alloc_Float64(1);
// compilenode returning num1525
  gc_frame_newslot(num1525);
  Object num1526 = alloc_Float64(7);
// compilenode returning num1526
  gc_frame_newslot(num1526);
// compilenode returning *var_line
  params[0] = num1525;
  params[1] = num1526;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1527 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1524);
// compilenode returning call1527
  int op_slot_left_1523 = gc_frame_newslot(call1527);
  if (strlit1528 == NULL) {
    strlit1528 = alloc_String("import ");
    gc_root(strlit1528);
  }
// compilenode returning strlit1528
  int op_slot_right_1523 = gc_frame_newslot(strlit1528);
  params[0] = strlit1528;
  partcv[0] = 1;
  Object opresult1530 = callmethod(call1527, "==", 1, partcv, params);
// compilenode returning opresult1530
  int op_slot_right_1516 = gc_frame_newslot(opresult1530);
  params[0] = opresult1530;
  partcv[0] = 1;
  Object opresult1532 = callmethod(opresult1522, "&&", 1, partcv, params);
// compilenode returning opresult1532
  Object if1515 = done;
  if (istrue(opresult1532)) {
// Begin line 461
  setline(461);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 460
  setline(460);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1533 = alloc_Float64(1);
// compilenode returning num1533
  *var_pos = num1533;
  if (num1533 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 462
  setline(462);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 461
  setline(461);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  *var_start = *var_pos;
  if (*var_pos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 463
  setline(463);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 462
  setline(462);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1534 == NULL) {
    strlit1534 = alloc_String("");
    gc_root(strlit1534);
  }
// compilenode returning strlit1534
  *var_nextImport = strlit1534;
  if (strlit1534 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 464
  setline(464);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1535 = gc_frame_new();
// Begin line 463
  setline(463);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// compilenode returning *var_pos
  gc_frame_newslot(*var_pos);
  params[0] = *var_line;
  params[1] = *var_pos;
  partcv[0] = 2;
  Object call1536 = callmethodflags(self, "skipWhiteSpace", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1535);
// compilenode returning call1536
  *var_pos = call1536;
  if (call1536 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 465
  setline(465);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 464
  setline(464);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  int op_slot_left_1538 = gc_frame_newslot(*var_pos);
  Object num1539 = alloc_Float64(1);
// compilenode returning num1539
  int op_slot_right_1538 = gc_frame_newslot(num1539);
  params[0] = num1539;
  partcv[0] = 1;
  Object sum1541 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1541
  *var_pos = sum1541;
  if (sum1541 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 465
  setline(465);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1544 == NULL) {
    strlit1544 = alloc_String("LINE = ");
    gc_root(strlit1544);
  }
// compilenode returning strlit1544
  int op_slot_left_1543 = gc_frame_newslot(strlit1544);
// compilenode returning *var_line
  int op_slot_right_1543 = gc_frame_newslot(*var_line);
  params[0] = *var_line;
  partcv[0] = 1;
  Object opresult1546 = callmethod(strlit1544, "++", 1, partcv, params);
// compilenode returning opresult1546
  params[0] = opresult1546;
  Object call1547 = gracelib_print(NULL, 1,  params);
// compilenode returning call1547
// Begin line 469
  setline(469);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1548 = gc_frame_new();
// Begin line 466
  setline(466);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1549 = alloc_Block(NULL, NULL, "PackageManager", 466);
  gc_frame_newslot(block1549);
  block_savedest(block1549);
  Object closure1550 = createclosure(3, "_apply");
setclosureframe(closure1550, stackframe);
  addtoclosure(closure1550, var_pos);
  addtoclosure(closure1550, var_line);
  addtoclosure(closure1550, selfslot);
  struct UserObject *uo1550 = (struct UserObject*)block1549;
  uo1550->data[0] = (Object)closure1550;
  Method *meth_meth_PackageManager__apply1550 = addmethod2pos(block1549, "_apply", &meth_PackageManager__apply1550, 0);
int argcv_meth_PackageManager__apply1550[] = {0};
meth_meth_PackageManager__apply1550->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1550);
  meth_meth_PackageManager__apply1550->definitionModule = modulename;
  meth_meth_PackageManager__apply1550->definitionLine = 459;
// compilenode returning block1549
  gc_frame_newslot(block1549);
// Begin line 469
  setline(469);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1575 = alloc_Block(NULL, NULL, "PackageManager", 469);
  gc_frame_newslot(block1575);
  block_savedest(block1575);
  Object closure1576 = createclosure(2, "_apply");
setclosureframe(closure1576, stackframe);
  addtoclosure(closure1576, var_pos);
  addtoclosure(closure1576, selfslot);
  struct UserObject *uo1576 = (struct UserObject*)block1575;
  uo1576->data[0] = (Object)closure1576;
  Method *meth_meth_PackageManager__apply1576 = addmethod2pos(block1575, "_apply", &meth_PackageManager__apply1576, 0);
int argcv_meth_PackageManager__apply1576[] = {0};
meth_meth_PackageManager__apply1576->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1576);
  meth_meth_PackageManager__apply1576->definitionModule = modulename;
  meth_meth_PackageManager__apply1576->definitionLine = 466;
// compilenode returning block1575
  gc_frame_newslot(block1575);
  params[0] = block1549;
  params[1] = block1575;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1583 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1548);
// compilenode returning call1583
// Begin line 469
  setline(469);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  int op_slot_left_1584 = gc_frame_newslot(*var_pos);
  Object num1585 = alloc_Float64(1);
// compilenode returning num1585
  int op_slot_right_1584 = gc_frame_newslot(num1585);
  params[0] = num1585;
  partcv[0] = 1;
  Object sum1587 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1587
  *var_pos = sum1587;
  if (sum1587 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 470
  setline(470);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1589 = gc_frame_new();
  int callframe1590 = gc_frame_new();
// compilenode returning *var_pos
  int op_slot_left_1591 = gc_frame_newslot(*var_pos);
  Object num1592 = alloc_Float64(1);
// compilenode returning num1592
  int op_slot_right_1591 = gc_frame_newslot(num1592);
  params[0] = num1592;
  partcv[0] = 1;
  Object sum1594 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1594
  gc_frame_newslot(sum1594);
  int callframe1595 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1596 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1595);
// compilenode returning call1596
// compilenode returning call1596
  gc_frame_newslot(call1596);
// compilenode returning *var_line
  params[0] = sum1594;
  params[1] = call1596;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1597 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1590);
// compilenode returning call1597
  gc_frame_newslot(call1597);
  params[0] = call1597;
  partcv[0] = 1;
  Object call1598 = callmethodflags(self, "removeExistingUrls", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1589);
// compilenode returning call1598
  *var_remainder = call1598;
  if (call1598 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 471
  setline(471);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1602 == NULL) {
    strlit1602 = alloc_String("import ""\x22""");
    gc_root(strlit1602);
  }
// compilenode returning strlit1602
  int op_slot_left_1601 = gc_frame_newslot(strlit1602);
// compilenode returning *var_prefix
  int op_slot_right_1601 = gc_frame_newslot(*var_prefix);
  params[0] = *var_prefix;
  partcv[0] = 1;
  Object opresult1604 = callmethod(strlit1602, "++", 1, partcv, params);
// compilenode returning opresult1604
  int op_slot_left_1600 = gc_frame_newslot(opresult1604);
  if (strlit1605 == NULL) {
    strlit1605 = alloc_String("/");
    gc_root(strlit1605);
  }
// compilenode returning strlit1605
  int op_slot_right_1600 = gc_frame_newslot(strlit1605);
  params[0] = strlit1605;
  partcv[0] = 1;
  Object opresult1607 = callmethod(opresult1604, "++", 1, partcv, params);
// compilenode returning opresult1607
  int op_slot_left_1599 = gc_frame_newslot(opresult1607);
// compilenode returning *var_remainder
  int op_slot_right_1599 = gc_frame_newslot(*var_remainder);
  params[0] = *var_remainder;
  partcv[0] = 1;
  Object opresult1609 = callmethod(opresult1607, "++", 1, partcv, params);
// compilenode returning opresult1609
  *var_line = opresult1609;
  if (opresult1609 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 472
  setline(472);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  params[0] = *var_line;
  Object call1611 = gracelib_print(NULL, 1,  params);
// compilenode returning call1611
// Begin line 476
  setline(476);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1612 = gc_frame_new();
// Begin line 473
  setline(473);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// Begin line 476
  setline(476);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1613 = alloc_Block(NULL, NULL, "PackageManager", 476);
  gc_frame_newslot(block1613);
  block_savedest(block1613);
  Object closure1614 = createclosure(2, "_apply");
setclosureframe(closure1614, stackframe);
  addtoclosure(closure1614, var_outFile);
  addtoclosure(closure1614, selfslot);
  struct UserObject *uo1614 = (struct UserObject*)block1613;
  uo1614->data[0] = (Object)closure1614;
  Method *meth_meth_PackageManager__apply1614 = addmethod2pos(block1613, "_apply", &meth_PackageManager__apply1614, 0);
int argcv_meth_PackageManager__apply1614[] = {1};
meth_meth_PackageManager__apply1614->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1614);
  meth_meth_PackageManager__apply1614->definitionModule = modulename;
  meth_meth_PackageManager__apply1614->definitionLine = 470;
// compilenode returning block1613
  gc_frame_newslot(block1613);
  params[0] = *var_line;
  params[1] = block1613;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1618 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1612);
// compilenode returning call1618
    gc_frame_newslot(call1618);
    if1515 = call1618;
  } else {
// Begin line 481
  setline(481);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1619 = gc_frame_new();
// Begin line 478
  setline(478);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// Begin line 481
  setline(481);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1620 = alloc_Block(NULL, NULL, "PackageManager", 481);
  gc_frame_newslot(block1620);
  block_savedest(block1620);
  Object closure1621 = createclosure(2, "_apply");
setclosureframe(closure1621, stackframe);
  addtoclosure(closure1621, var_outFile);
  addtoclosure(closure1621, selfslot);
  struct UserObject *uo1621 = (struct UserObject*)block1620;
  uo1621->data[0] = (Object)closure1621;
  Method *meth_meth_PackageManager__apply1621 = addmethod2pos(block1620, "_apply", &meth_PackageManager__apply1621, 0);
int argcv_meth_PackageManager__apply1621[] = {1};
meth_meth_PackageManager__apply1621->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1621);
  meth_meth_PackageManager__apply1621->definitionModule = modulename;
  meth_meth_PackageManager__apply1621->definitionLine = 470;
// compilenode returning block1620
  gc_frame_newslot(block1620);
  params[0] = *var_line;
  params[1] = block1620;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1625 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1619);
// compilenode returning call1625
    gc_frame_newslot(call1625);
    if1515 = call1625;
  }
// compilenode returning if1515
// Begin line 483
  setline(483);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 482
  setline(482);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1626 = gc_frame_newslot(*var_curPos);
  Object num1627 = alloc_Float64(1);
// compilenode returning num1627
  int op_slot_right_1626 = gc_frame_newslot(num1627);
  params[0] = num1627;
  partcv[0] = 1;
  Object sum1629 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1629
  *var_curPos = sum1629;
  if (sum1629 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1450(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  Object *var_curPos = getfromclosure(closure, 0);
  Object *var_readFile = getfromclosure(closure, 1);
  Object *var_startPos = getfromclosure(closure, 2);
  Object *var_prefix = getfromclosure(closure, 3);
  Object *var_outFile = getfromclosure(closure, 4);
  Object self = *(getfromclosure(closure, 5));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *var_t = &(stackframe->slots[0]);
  *var_t = args[curarg];
  curarg++;
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 484
  setline(484);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1451 = gc_frame_new();
// Begin line 447
  setline(447);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1452 = alloc_Block(NULL, NULL, "PackageManager", 447);
  gc_frame_newslot(block1452);
  block_savedest(block1452);
  Object closure1453 = createclosure(3, "_apply");
setclosureframe(closure1453, stackframe);
  addtoclosure(closure1453, var_curPos);
  addtoclosure(closure1453, var_readFile);
  addtoclosure(closure1453, selfslot);
  struct UserObject *uo1453 = (struct UserObject*)block1452;
  uo1453->data[0] = (Object)closure1453;
  Method *meth_meth_PackageManager__apply1453 = addmethod2pos(block1452, "_apply", &meth_PackageManager__apply1453, 0);
int argcv_meth_PackageManager__apply1453[] = {0};
meth_meth_PackageManager__apply1453->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1453);
  meth_meth_PackageManager__apply1453->definitionModule = modulename;
  meth_meth_PackageManager__apply1453->definitionLine = 446;
// compilenode returning block1452
  gc_frame_newslot(block1452);
// Begin line 484
  setline(484);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1460 = alloc_Block(NULL, NULL, "PackageManager", 484);
  gc_frame_newslot(block1460);
  block_savedest(block1460);
  Object closure1461 = createclosure(6, "_apply");
setclosureframe(closure1461, stackframe);
  addtoclosure(closure1461, var_curPos);
  addtoclosure(closure1461, var_startPos);
  addtoclosure(closure1461, var_readFile);
  addtoclosure(closure1461, var_prefix);
  addtoclosure(closure1461, var_outFile);
  addtoclosure(closure1461, selfslot);
  struct UserObject *uo1461 = (struct UserObject*)block1460;
  uo1461->data[0] = (Object)closure1461;
  Method *meth_meth_PackageManager__apply1461 = addmethod2pos(block1460, "_apply", &meth_PackageManager__apply1461, 0);
int argcv_meth_PackageManager__apply1461[] = {0};
meth_meth_PackageManager__apply1461->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1461);
  meth_meth_PackageManager__apply1461->definitionModule = modulename;
  meth_meth_PackageManager__apply1461->definitionLine = 447;
// compilenode returning block1460
  gc_frame_newslot(block1460);
  params[0] = block1452;
  params[1] = block1460;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1632 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1451);
// compilenode returning call1632
  gc_frame_end(frame);
  return call1632;
}
Object meth_PackageManager_parseAndPrefix1439(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct StackFrameObject *stackframe = alloc_StackFrame(7, NULL);
  pushclosure(NULL);
  pushstackframe(stackframe, "parseAndPrefix");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 3)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for parseAndPrefix (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_readFile = &(stackframe->slots[0]);
  *var_readFile = args[curarg];
  curarg++;
  Object *var_address = &(stackframe->slots[1]);
  *var_address = args[curarg];
  curarg++;
  Object *var_prefix = &(stackframe->slots[2]);
  *var_prefix = args[curarg];
  curarg++;
if (argcv && argcv[0] > 3)
  gracedie("too many arguments for parseAndPrefix");
  Object *selfslot = &(stackframe->slots[3]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_curPos = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "curPos");
  Object *var_startPos = &(stackframe->slots[5]);
  setframeelementname(stackframe, 5, "startPos");
  Object *var_outFile = &(stackframe->slots[6]);
  setframeelementname(stackframe, 6, "outFile");
// Begin line 443
  setline(443);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1440 = alloc_Float64(1);
// compilenode returning num1440
  *var_curPos = num1440;
  if (num1440 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 445
  setline(445);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 444
  setline(444);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 445
  setline(445);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array1441 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array1441
  *var_outFile = array1441;
  if (array1441 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 487
  setline(487);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1442 = gc_frame_new();
// Begin line 446
  setline(446);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1444 = alloc_Float64(0);
// compilenode returning num1444
  int op_slot_left_1443 = gc_frame_newslot(num1444);
  int callframe1445 = gc_frame_new();
// compilenode returning *var_readFile
  partcv[0] = 0;
  Object call1446 = callmethod(*var_readFile, "size",
    1, partcv, params);
  gc_frame_end(callframe1445);
// compilenode returning call1446
// compilenode returning call1446
  int op_slot_right_1443 = gc_frame_newslot(call1446);
  params[0] = call1446;
  partcv[0] = 1;
  Object opresult1448 = callmethod(num1444, "..", 1, partcv, params);
// compilenode returning opresult1448
  gc_frame_newslot(opresult1448);
// Begin line 487
  setline(487);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1449 = alloc_Block(NULL, NULL, "PackageManager", 487);
  gc_frame_newslot(block1449);
  block_savedest(block1449);
  Object closure1450 = createclosure(6, "_apply");
setclosureframe(closure1450, stackframe);
  addtoclosure(closure1450, var_curPos);
  addtoclosure(closure1450, var_readFile);
  addtoclosure(closure1450, var_startPos);
  addtoclosure(closure1450, var_prefix);
  addtoclosure(closure1450, var_outFile);
  addtoclosure(closure1450, selfslot);
  struct UserObject *uo1450 = (struct UserObject*)block1449;
  uo1450->data[0] = (Object)closure1450;
  Method *meth_meth_PackageManager__apply1450 = addmethod2pos(block1449, "_apply", &meth_PackageManager__apply1450, 0);
int argcv_meth_PackageManager__apply1450[] = {1};
meth_meth_PackageManager__apply1450->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1450);
  meth_meth_PackageManager__apply1450->definitionModule = modulename;
  meth_meth_PackageManager__apply1450->definitionLine = 446;
// compilenode returning block1449
  gc_frame_newslot(block1449);
  params[0] = opresult1448;
  params[1] = block1449;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1634 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1442);
// compilenode returning call1634
// Begin line 488
  setline(488);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 487
  setline(487);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1636 == NULL) {
    strlit1636 = alloc_String("OUTFILE ");
    gc_root(strlit1636);
  }
// compilenode returning strlit1636
  int op_slot_left_1635 = gc_frame_newslot(strlit1636);
// compilenode returning *var_outFile
  int op_slot_right_1635 = gc_frame_newslot(*var_outFile);
  params[0] = *var_outFile;
  partcv[0] = 1;
  Object opresult1638 = callmethod(strlit1636, "++", 1, partcv, params);
// compilenode returning opresult1638
  params[0] = opresult1638;
  Object call1639 = gracelib_print(NULL, 1,  params);
// compilenode returning call1639
// Begin line 488
  setline(488);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_outFile
  return *var_outFile;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply1661(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_curPos = getfromclosure(closure, 0);
  Object *var_importStatement = getfromclosure(closure, 1);
  Object self = *(getfromclosure(closure, 2));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var_curPos
  int op_slot_left_1662 = gc_frame_newslot(*var_curPos);
  int callframe1663 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1664 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1663);
// compilenode returning call1664
// compilenode returning call1664
  int op_slot_right_1662 = gc_frame_newslot(call1664);
  params[0] = call1664;
  partcv[0] = 1;
  Object opresult1666 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1666
  gc_frame_end(frame);
  return opresult1666;
}
Object meth_PackageManager__apply1669(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_importStatement = getfromclosure(closure, 0);
  Object *var_curPos = getfromclosure(closure, 1);
  Object *var_lastIndex = getfromclosure(closure, 2);
  Object self = *(getfromclosure(closure, 3));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 504
  setline(504);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 501
  setline(501);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1672 = callmethod(*var_importStatement, "[]", 1, partcv, params);
// compilenode returning idxres1672
  int op_slot_left_1671 = gc_frame_newslot(idxres1672);
  if (strlit1673 == NULL) {
    strlit1673 = alloc_String("/");
    gc_root(strlit1673);
  }
// compilenode returning strlit1673
  int op_slot_right_1671 = gc_frame_newslot(strlit1673);
  params[0] = strlit1673;
  partcv[0] = 1;
  Object opresult1675 = callmethod(idxres1672, "==", 1, partcv, params);
// compilenode returning opresult1675
  Object if1670 = done;
  if (istrue(opresult1675)) {
// Begin line 502
  setline(502);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_lastIndex = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if1670 = done;
  } else {
  }
// compilenode returning if1670
// Begin line 504
  setline(504);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1677 = gc_frame_newslot(*var_curPos);
  Object num1678 = alloc_Float64(1);
// compilenode returning num1678
  int op_slot_right_1677 = gc_frame_newslot(num1678);
  params[0] = num1678;
  partcv[0] = 1;
  Object sum1680 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1680
  *var_curPos = sum1680;
  if (sum1680 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_removeExistingUrls1640(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct StackFrameObject *stackframe = alloc_StackFrame(5, NULL);
  pushclosure(NULL);
  pushstackframe(stackframe, "removeExistingUrls");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for removeExistingUrls (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_importStatement = &(stackframe->slots[0]);
  *var_importStatement = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for removeExistingUrls");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_lastIndex = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "lastIndex");
  Object *var_curPos = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "curPos");
  Object *var_res = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "res");
// Begin line 497
  setline(497);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 494
  setline(494);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1643 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1644 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1643);
// compilenode returning call1644
// compilenode returning call1644
  int op_slot_left_1642 = gc_frame_newslot(call1644);
  Object num1645 = alloc_Float64(7);
// compilenode returning num1645
  int op_slot_right_1642 = gc_frame_newslot(num1645);
  params[0] = num1645;
  partcv[0] = 1;
  Object opresult1647 = callmethod(call1644, "<", 1, partcv, params);
// compilenode returning opresult1647
  Object if1641 = done;
  if (istrue(opresult1647)) {
// Begin line 495
  setline(495);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
  return *var_importStatement;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1641 = undefined;
  } else {
  }
// compilenode returning if1641
// Begin line 514
  setline(514);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 497
  setline(497);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1650 = gc_frame_new();
  Object num1651 = alloc_Float64(1);
// compilenode returning num1651
  gc_frame_newslot(num1651);
  Object num1652 = alloc_Float64(7);
// compilenode returning num1652
  gc_frame_newslot(num1652);
// compilenode returning *var_importStatement
  params[0] = num1651;
  params[1] = num1652;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1653 = callmethod(*var_importStatement, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1650);
// compilenode returning call1653
  int op_slot_left_1649 = gc_frame_newslot(call1653);
  if (strlit1654 == NULL) {
    strlit1654 = alloc_String("http://");
    gc_root(strlit1654);
  }
// compilenode returning strlit1654
  int op_slot_right_1649 = gc_frame_newslot(strlit1654);
  params[0] = strlit1654;
  partcv[0] = 1;
  Object opresult1656 = callmethod(call1653, "==", 1, partcv, params);
// compilenode returning opresult1656
  Object if1648 = done;
  if (istrue(opresult1656)) {
// Begin line 498
  setline(498);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1657 = alloc_Float64(7);
// compilenode returning num1657
  *var_lastIndex = num1657;
  if (num1657 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 499
  setline(499);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1658 = alloc_Float64(7);
// compilenode returning num1658
  *var_curPos = num1658;
  if (num1658 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 506
  setline(506);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1659 = gc_frame_new();
// Begin line 500
  setline(500);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1660 = alloc_Block(NULL, NULL, "PackageManager", 500);
  gc_frame_newslot(block1660);
  block_savedest(block1660);
  Object closure1661 = createclosure(3, "_apply");
setclosureframe(closure1661, stackframe);
  addtoclosure(closure1661, var_curPos);
  addtoclosure(closure1661, var_importStatement);
  addtoclosure(closure1661, selfslot);
  struct UserObject *uo1661 = (struct UserObject*)block1660;
  uo1661->data[0] = (Object)closure1661;
  Method *meth_meth_PackageManager__apply1661 = addmethod2pos(block1660, "_apply", &meth_PackageManager__apply1661, 0);
int argcv_meth_PackageManager__apply1661[] = {0};
meth_meth_PackageManager__apply1661->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1661);
  meth_meth_PackageManager__apply1661->definitionModule = modulename;
  meth_meth_PackageManager__apply1661->definitionLine = 494;
// compilenode returning block1660
  gc_frame_newslot(block1660);
// Begin line 506
  setline(506);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1668 = alloc_Block(NULL, NULL, "PackageManager", 506);
  gc_frame_newslot(block1668);
  block_savedest(block1668);
  Object closure1669 = createclosure(4, "_apply");
setclosureframe(closure1669, stackframe);
  addtoclosure(closure1669, var_importStatement);
  addtoclosure(closure1669, var_curPos);
  addtoclosure(closure1669, var_lastIndex);
  addtoclosure(closure1669, selfslot);
  struct UserObject *uo1669 = (struct UserObject*)block1668;
  uo1669->data[0] = (Object)closure1669;
  Method *meth_meth_PackageManager__apply1669 = addmethod2pos(block1668, "_apply", &meth_PackageManager__apply1669, 0);
int argcv_meth_PackageManager__apply1669[] = {0};
meth_meth_PackageManager__apply1669->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1669);
  meth_meth_PackageManager__apply1669->definitionModule = modulename;
  meth_meth_PackageManager__apply1669->definitionLine = 500;
// compilenode returning block1668
  gc_frame_newslot(block1668);
  params[0] = block1660;
  params[1] = block1668;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1683 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1659);
// compilenode returning call1683
// Begin line 506
  setline(506);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1684 = gc_frame_new();
// compilenode returning *var_lastIndex
  int op_slot_left_1685 = gc_frame_newslot(*var_lastIndex);
  Object num1686 = alloc_Float64(1);
// compilenode returning num1686
  int op_slot_right_1685 = gc_frame_newslot(num1686);
  params[0] = num1686;
  partcv[0] = 1;
  Object sum1688 = callmethod(*var_lastIndex, "+", 1, partcv, params);
// compilenode returning sum1688
  gc_frame_newslot(sum1688);
  int callframe1689 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1690 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1689);
// compilenode returning call1690
// compilenode returning call1690
  gc_frame_newslot(call1690);
// compilenode returning *var_importStatement
  params[0] = sum1688;
  params[1] = call1690;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1691 = callmethod(*var_importStatement, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1684);
// compilenode returning call1691
  *var_res = call1691;
  if (call1691 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 507
  setline(507);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_res
  return *var_res;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1648 = undefined;
  } else {
// Begin line 510
  setline(510);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
  return *var_importStatement;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1648 = undefined;
  }
// compilenode returning if1648
  gc_frame_end(frame);
  return if1648;
}
Object meth_PackageManager__apply1699(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_count = getfromclosure(closure, 0);
  Object *var_st = getfromclosure(closure, 1);
  Object self = *(getfromclosure(closure, 2));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var_count
  int op_slot_left_1700 = gc_frame_newslot(*var_count);
  int callframe1701 = gc_frame_new();
// compilenode returning *var_st
  partcv[0] = 0;
  Object call1702 = callmethod(*var_st, "size",
    1, partcv, params);
  gc_frame_end(callframe1701);
// compilenode returning call1702
// compilenode returning call1702
  int op_slot_right_1700 = gc_frame_newslot(call1702);
  params[0] = call1702;
  partcv[0] = 1;
  Object opresult1704 = callmethod(*var_count, "<=", 1, partcv, params);
// compilenode returning opresult1704
  gc_frame_end(frame);
  return opresult1704;
}
Object meth_PackageManager__apply1707(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_st = getfromclosure(closure, 0);
  Object *var_count = getfromclosure(closure, 1);
  Object *var_lastIndex = getfromclosure(closure, 2);
  Object self = *(getfromclosure(closure, 3));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 523
  setline(523);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 520
  setline(520);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_st
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1710 = callmethod(*var_st, "[]", 1, partcv, params);
// compilenode returning idxres1710
  int op_slot_left_1709 = gc_frame_newslot(idxres1710);
  if (strlit1711 == NULL) {
    strlit1711 = alloc_String("/");
    gc_root(strlit1711);
  }
// compilenode returning strlit1711
  int op_slot_right_1709 = gc_frame_newslot(strlit1711);
  params[0] = strlit1711;
  partcv[0] = 1;
  Object opresult1713 = callmethod(idxres1710, "==", 1, partcv, params);
// compilenode returning opresult1713
  Object if1708 = done;
  if (istrue(opresult1713)) {
// Begin line 521
  setline(521);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  *var_lastIndex = *var_count;
  if (*var_count == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if1708 = done;
  } else {
  }
// compilenode returning if1708
// Begin line 523
  setline(523);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  int op_slot_left_1715 = gc_frame_newslot(*var_count);
  Object num1716 = alloc_Float64(1);
// compilenode returning num1716
  int op_slot_right_1715 = gc_frame_newslot(num1716);
  params[0] = num1716;
  partcv[0] = 1;
  Object sum1718 = callmethod(*var_count, "+", 1, partcv, params);
// compilenode returning sum1718
  *var_count = sum1718;
  if (sum1718 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_removeContainingDir1692(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct StackFrameObject *stackframe = alloc_StackFrame(4, NULL);
  pushclosure(NULL);
  pushstackframe(stackframe, "removeContainingDir");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for removeContainingDir (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_st = &(stackframe->slots[0]);
  *var_st = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for removeContainingDir");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_count = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "count");
  Object *var_lastIndex = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "lastIndex");
// Begin line 517
  setline(517);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1693 = alloc_Float64(1);
// compilenode returning num1693
  *var_count = num1693;
  if (num1693 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 518
  setline(518);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1694 = gc_frame_new();
  Object num1695 = alloc_Float64(1);
// compilenode returning num1695
  partcv[0] = 0;
  Object call1696 = callmethod(num1695, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1694);
// compilenode returning call1696
  *var_lastIndex = call1696;
  if (call1696 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 525
  setline(525);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1697 = gc_frame_new();
// Begin line 519
  setline(519);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1698 = alloc_Block(NULL, NULL, "PackageManager", 519);
  gc_frame_newslot(block1698);
  block_savedest(block1698);
  Object closure1699 = createclosure(3, "_apply");
setclosureframe(closure1699, stackframe);
  addtoclosure(closure1699, var_count);
  addtoclosure(closure1699, var_st);
  addtoclosure(closure1699, selfslot);
  struct UserObject *uo1699 = (struct UserObject*)block1698;
  uo1699->data[0] = (Object)closure1699;
  Method *meth_meth_PackageManager__apply1699 = addmethod2pos(block1698, "_apply", &meth_PackageManager__apply1699, 0);
int argcv_meth_PackageManager__apply1699[] = {0};
meth_meth_PackageManager__apply1699->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1699);
  meth_meth_PackageManager__apply1699->definitionModule = modulename;
  meth_meth_PackageManager__apply1699->definitionLine = 506;
// compilenode returning block1698
  gc_frame_newslot(block1698);
// Begin line 525
  setline(525);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1706 = alloc_Block(NULL, NULL, "PackageManager", 525);
  gc_frame_newslot(block1706);
  block_savedest(block1706);
  Object closure1707 = createclosure(4, "_apply");
setclosureframe(closure1707, stackframe);
  addtoclosure(closure1707, var_st);
  addtoclosure(closure1707, var_count);
  addtoclosure(closure1707, var_lastIndex);
  addtoclosure(closure1707, selfslot);
  struct UserObject *uo1707 = (struct UserObject*)block1706;
  uo1707->data[0] = (Object)closure1707;
  Method *meth_meth_PackageManager__apply1707 = addmethod2pos(block1706, "_apply", &meth_PackageManager__apply1707, 0);
int argcv_meth_PackageManager__apply1707[] = {0};
meth_meth_PackageManager__apply1707->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1707);
  meth_meth_PackageManager__apply1707->definitionModule = modulename;
  meth_meth_PackageManager__apply1707->definitionLine = 519;
// compilenode returning block1706
  gc_frame_newslot(block1706);
  params[0] = block1698;
  params[1] = block1706;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1721 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1697);
// compilenode returning call1721
// Begin line 528
  setline(528);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 525
  setline(525);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_lastIndex
  int op_slot_left_1723 = gc_frame_newslot(*var_lastIndex);
  int callframe1724 = gc_frame_new();
  Object num1725 = alloc_Float64(1);
// compilenode returning num1725
  partcv[0] = 0;
  Object call1726 = callmethod(num1725, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1724);
// compilenode returning call1726
  int op_slot_right_1723 = gc_frame_newslot(call1726);
  params[0] = call1726;
  partcv[0] = 1;
  Object opresult1728 = callmethod(*var_lastIndex, "==", 1, partcv, params);
// compilenode returning opresult1728
  Object if1722 = done;
  if (istrue(opresult1728)) {
// Begin line 526
  setline(526);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_st
  return *var_st;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1722 = undefined;
  } else {
  }
// compilenode returning if1722
// Begin line 528
  setline(528);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1729 = gc_frame_new();
// compilenode returning *var_lastIndex
  int op_slot_left_1730 = gc_frame_newslot(*var_lastIndex);
  Object num1731 = alloc_Float64(1);
// compilenode returning num1731
  int op_slot_right_1730 = gc_frame_newslot(num1731);
  params[0] = num1731;
  partcv[0] = 1;
  Object sum1733 = callmethod(*var_lastIndex, "+", 1, partcv, params);
// compilenode returning sum1733
  gc_frame_newslot(sum1733);
  int callframe1734 = gc_frame_new();
// compilenode returning *var_st
  partcv[0] = 0;
  Object call1735 = callmethod(*var_st, "size",
    1, partcv, params);
  gc_frame_end(callframe1734);
// compilenode returning call1735
// compilenode returning call1735
  gc_frame_newslot(call1735);
// compilenode returning *var_st
  params[0] = sum1733;
  params[1] = call1735;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1736 = callmethod(*var_st, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1729);
// compilenode returning call1736
  return call1736;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply1744(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_count = getfromclosure(closure, 0);
  Object *var_st = getfromclosure(closure, 1);
  Object self = *(getfromclosure(closure, 2));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var_count
  int op_slot_left_1745 = gc_frame_newslot(*var_count);
  int callframe1746 = gc_frame_new();
// compilenode returning *var_st
  partcv[0] = 0;
  Object call1747 = callmethod(*var_st, "size",
    1, partcv, params);
  gc_frame_end(callframe1746);
// compilenode returning call1747
// compilenode returning call1747
  int op_slot_right_1745 = gc_frame_newslot(call1747);
  params[0] = call1747;
  partcv[0] = 1;
  Object opresult1749 = callmethod(*var_count, "<=", 1, partcv, params);
// compilenode returning opresult1749
  gc_frame_end(frame);
  return opresult1749;
}
Object meth_PackageManager__apply1752(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_st = getfromclosure(closure, 0);
  Object *var_count = getfromclosure(closure, 1);
  Object *var_last = getfromclosure(closure, 2);
  Object self = *(getfromclosure(closure, 3));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 538
  setline(538);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 535
  setline(535);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_st
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1755 = callmethod(*var_st, "[]", 1, partcv, params);
// compilenode returning idxres1755
  int op_slot_left_1754 = gc_frame_newslot(idxres1755);
  if (strlit1756 == NULL) {
    strlit1756 = alloc_String("/");
    gc_root(strlit1756);
  }
// compilenode returning strlit1756
  int op_slot_right_1754 = gc_frame_newslot(strlit1756);
  params[0] = strlit1756;
  partcv[0] = 1;
  Object opresult1758 = callmethod(idxres1755, "==", 1, partcv, params);
// compilenode returning opresult1758
  Object if1753 = done;
  if (istrue(opresult1758)) {
// Begin line 536
  setline(536);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  *var_last = *var_count;
  if (*var_count == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if1753 = done;
  } else {
  }
// compilenode returning if1753
// Begin line 538
  setline(538);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  int op_slot_left_1760 = gc_frame_newslot(*var_count);
  Object num1761 = alloc_Float64(1);
// compilenode returning num1761
  int op_slot_right_1760 = gc_frame_newslot(num1761);
  params[0] = num1761;
  partcv[0] = 1;
  Object sum1763 = callmethod(*var_count, "+", 1, partcv, params);
// compilenode returning sum1763
  *var_count = sum1763;
  if (sum1763 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_getContainingDirectory1737(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct StackFrameObject *stackframe = alloc_StackFrame(4, NULL);
  pushclosure(NULL);
  pushstackframe(stackframe, "getContainingDirectory");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for getContainingDirectory (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_st = &(stackframe->slots[0]);
  *var_st = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for getContainingDirectory");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_count = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "count");
  Object *var_last = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "last");
// Begin line 532
  setline(532);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1738 = alloc_Float64(1);
// compilenode returning num1738
  *var_count = num1738;
  if (num1738 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 533
  setline(533);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1739 = gc_frame_new();
  Object num1740 = alloc_Float64(1);
// compilenode returning num1740
  partcv[0] = 0;
  Object call1741 = callmethod(num1740, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1739);
// compilenode returning call1741
  *var_last = call1741;
  if (call1741 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 540
  setline(540);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1742 = gc_frame_new();
// Begin line 534
  setline(534);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1743 = alloc_Block(NULL, NULL, "PackageManager", 534);
  gc_frame_newslot(block1743);
  block_savedest(block1743);
  Object closure1744 = createclosure(3, "_apply");
setclosureframe(closure1744, stackframe);
  addtoclosure(closure1744, var_count);
  addtoclosure(closure1744, var_st);
  addtoclosure(closure1744, selfslot);
  struct UserObject *uo1744 = (struct UserObject*)block1743;
  uo1744->data[0] = (Object)closure1744;
  Method *meth_meth_PackageManager__apply1744 = addmethod2pos(block1743, "_apply", &meth_PackageManager__apply1744, 0);
int argcv_meth_PackageManager__apply1744[] = {0};
meth_meth_PackageManager__apply1744->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1744);
  meth_meth_PackageManager__apply1744->definitionModule = modulename;
  meth_meth_PackageManager__apply1744->definitionLine = 528;
// compilenode returning block1743
  gc_frame_newslot(block1743);
// Begin line 540
  setline(540);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1751 = alloc_Block(NULL, NULL, "PackageManager", 540);
  gc_frame_newslot(block1751);
  block_savedest(block1751);
  Object closure1752 = createclosure(4, "_apply");
setclosureframe(closure1752, stackframe);
  addtoclosure(closure1752, var_st);
  addtoclosure(closure1752, var_count);
  addtoclosure(closure1752, var_last);
  addtoclosure(closure1752, selfslot);
  struct UserObject *uo1752 = (struct UserObject*)block1751;
  uo1752->data[0] = (Object)closure1752;
  Method *meth_meth_PackageManager__apply1752 = addmethod2pos(block1751, "_apply", &meth_PackageManager__apply1752, 0);
int argcv_meth_PackageManager__apply1752[] = {0};
meth_meth_PackageManager__apply1752->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1752);
  meth_meth_PackageManager__apply1752->definitionModule = modulename;
  meth_meth_PackageManager__apply1752->definitionLine = 534;
// compilenode returning block1751
  gc_frame_newslot(block1751);
  params[0] = block1743;
  params[1] = block1751;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1766 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1742);
// compilenode returning call1766
// Begin line 543
  setline(543);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 540
  setline(540);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_last
  int op_slot_left_1768 = gc_frame_newslot(*var_last);
  int callframe1769 = gc_frame_new();
  Object num1770 = alloc_Float64(1);
// compilenode returning num1770
  partcv[0] = 0;
  Object call1771 = callmethod(num1770, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1769);
// compilenode returning call1771
  int op_slot_right_1768 = gc_frame_newslot(call1771);
  params[0] = call1771;
  partcv[0] = 1;
  Object opresult1773 = callmethod(*var_last, "==", 1, partcv, params);
// compilenode returning opresult1773
  Object if1767 = done;
  if (istrue(opresult1773)) {
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1775 = gc_frame_new();
  partcv[0] = 0;
  Object call1776 = callmethodflags(self, "getBuildPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1775);
// compilenode returning call1776
  int op_slot_left_1774 = gc_frame_newslot(call1776);
  if (strlit1777 == NULL) {
    strlit1777 = alloc_String("/");
    gc_root(strlit1777);
  }
// compilenode returning strlit1777
  int op_slot_right_1774 = gc_frame_newslot(strlit1777);
  params[0] = strlit1777;
  partcv[0] = 1;
  Object opresult1779 = callmethod(call1776, "++", 1, partcv, params);
// compilenode returning opresult1779
  return opresult1779;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1767 = undefined;
  } else {
  }
// compilenode returning if1767
// Begin line 543
  setline(543);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1780 = gc_frame_new();
  Object num1781 = alloc_Float64(0);
// compilenode returning num1781
  gc_frame_newslot(num1781);
// compilenode returning *var_last
  int op_slot_left_1782 = gc_frame_newslot(*var_last);
  Object num1783 = alloc_Float64(1);
// compilenode returning num1783
  int op_slot_right_1782 = gc_frame_newslot(num1783);
  params[0] = num1783;
  partcv[0] = 1;
  Object diff1785 = callmethod(*var_last, "-", 1, partcv, params);
// compilenode returning diff1785
  gc_frame_newslot(diff1785);
// compilenode returning *var_st
  params[0] = num1781;
  params[1] = diff1785;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1786 = callmethod(*var_st, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1780);
// compilenode returning call1786
  return call1786;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_imported1787(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 27, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "imported");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_imported = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for imported (probably reflection error): got %i lists, expected 1.", nparts);
if (argcv && argcv[0] > 0)
  gracedie("too many arguments for <Identifier[imported]>");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 5
  setline(5);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_imported
  gc_frame_end(frame);
  return *var_imported;
}
Object meth_PackageManager_imported_58__61_1788(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 28, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "imported:=");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_imported = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for imported:= (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var__var_assign_tmp = &(stackframe->slots[0]);
  *var__var_assign_tmp = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for <Identifier[imported:=]>");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var__var_assign_tmp
  *var_imported = *var__var_assign_tmp;
  if (*var__var_assign_tmp == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_toProcess1791(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 29, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "toProcess");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_toProcess = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for toProcess (probably reflection error): got %i lists, expected 1.", nparts);
if (argcv && argcv[0] > 0)
  gracedie("too many arguments for <Identifier[toProcess]>");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 6
  setline(6);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toProcess
  gc_frame_end(frame);
  return *var_toProcess;
}
Object meth_PackageManager_toProcess_58__61_1792(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 30, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "toProcess:=");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_toProcess = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for toProcess:= (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var__var_assign_tmp = &(stackframe->slots[0]);
  *var__var_assign_tmp = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for <Identifier[toProcess:=]>");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var__var_assign_tmp
  *var_toProcess = *var__var_assign_tmp;
  if (*var__var_assign_tmp == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_verbose1795(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 31, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "verbose");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_verbose = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for verbose (probably reflection error): got %i lists, expected 1.", nparts);
if (argcv && argcv[0] > 0)
  gracedie("too many arguments for <Identifier[verbose]>");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 7
  setline(7);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_verbose
  gc_frame_end(frame);
  return *var_verbose;
}
Object meth_PackageManager_verbose_58__61_1796(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 32, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "verbose:=");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_verbose = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for verbose:= (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var__var_assign_tmp = &(stackframe->slots[0]);
  *var__var_assign_tmp = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for <Identifier[verbose:=]>");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var__var_assign_tmp
  *var_verbose = *var__var_assign_tmp;
  if (*var__var_assign_tmp == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_global1799(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 33, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "global");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_global = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for global (probably reflection error): got %i lists, expected 1.", nparts);
if (argcv && argcv[0] > 0)
  gracedie("too many arguments for <Identifier[global]>");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 8
  setline(8);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_global
  gc_frame_end(frame);
  return *var_global;
}
Object meth_PackageManager_global_58__61_1800(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 34, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "global:=");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_global = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for global:= (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var__var_assign_tmp = &(stackframe->slots[0]);
  *var__var_assign_tmp = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for <Identifier[global:=]>");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var__var_assign_tmp
  *var_global = *var__var_assign_tmp;
  if (*var__var_assign_tmp == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_bundlePath1803(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 35, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "bundlePath");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_bundlePath = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for bundlePath (probably reflection error): got %i lists, expected 1.", nparts);
if (argcv && argcv[0] > 0)
  gracedie("too many arguments for <Identifier[bundlePath]>");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 9
  setline(9);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_bundlePath
  gc_frame_end(frame);
  return *var_bundlePath;
}
Object meth_PackageManager_bundlePath_58__61_1804(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 36, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "bundlePath:=");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_bundlePath = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for bundlePath:= (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var__var_assign_tmp = &(stackframe->slots[0]);
  *var__var_assign_tmp = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for <Identifier[bundlePath:=]>");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var__var_assign_tmp
  *var_bundlePath = *var__var_assign_tmp;
  if (*var__var_assign_tmp == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_curFile1806(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 37, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "curFile");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_curFile = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for curFile (probably reflection error): got %i lists, expected 1.", nparts);
if (argcv && argcv[0] > 0)
  gracedie("too many arguments for <Identifier[curFile]>");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 10
  setline(10);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curFile
  gc_frame_end(frame);
  return *var_curFile;
}
Object meth_PackageManager_curFile_58__61_1807(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 38, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "curFile:=");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_curFile = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for curFile:= (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var__var_assign_tmp = &(stackframe->slots[0]);
  *var__var_assign_tmp = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for <Identifier[curFile:=]>");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var__var_assign_tmp
  *var_curFile = *var__var_assign_tmp;
  if (*var__var_assign_tmp == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object module_PackageManager_init() {
  int flags = 0;
  int frame = gc_frame_new();
  Object self = alloc_obj2(41, 41);
  self->class->definitionModule = modulename;
  gc_root(self);
  prelude = module_StandardPrelude_init();
  adddatum2(self, prelude, 0);
  addmethod2(self, "outer", &grace_userobj_outer);
  setline(1);
  setmodule(modulename);
  setsource(originalSourceLines);
  setclassname(self, "Module<PackageManager>");
  Object *var_MatchFailed = alloc_var();
  *var_MatchFailed = alloc_MatchFailed();
  Object *var_noSuchValue = alloc_var();
  *var_noSuchValue = done;
  Object *var_done = alloc_var();
  *var_done = done;
  Object *var_String = alloc_var();
  *var_String = String;
  type_String = String;
  Object *var_Block = alloc_var();
  *var_Block = Block;
  type_Block = Block;
  Object *var_Done = alloc_var();
  *var_Done = Done;
  type_Done = Done;
  Object *var_Number = alloc_var();
  *var_Number = Number;
  type_Number = Number;
  Object *var_Boolean = alloc_var();
  *var_Boolean = Boolean;
  type_Boolean = Boolean;
  Object *var_Dynamic = alloc_var();
  *var_Dynamic = Dynamic;
  Object *var_List = alloc_var();
  *var_List = List;
  Object *var_Type = alloc_var();
  *var_Type = Type;
  Object *var__prelude = alloc_var();
  *var__prelude = grace_prelude();
  gc_root(*var_MatchFailed);
  emptyclosure = createclosure(0, "empty");
  gc_root(emptyclosure);
  struct StackFrameObject *stackframe = alloc_StackFrame(41, NULL);
  gc_root((Object)stackframe);
  pushstackframe(stackframe, "module scope");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  Object params[1];
  int partcv[1];
Object *var_sys = alloc_var();
Object *var_io = alloc_var();
Object *var_curl = alloc_var();
  Object *var_imported = &(stackframe->slots[1]);
  setframeelementname(stackframe, 1, "imported");
  Object *var_toProcess = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "toProcess");
  Object *var_verbose = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "verbose");
  Object *var_global = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "global");
  Object *var_bundlePath = &(stackframe->slots[5]);
  setframeelementname(stackframe, 5, "bundlePath");
  Object *var_curFile = &(stackframe->slots[6]);
  setframeelementname(stackframe, 6, "curFile");
// Begin line 14
  setline(14);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure0 = createclosure(2, "parseArgs");
setclosureframe(closure0, stackframe);
  addtoclosure(closure0, var_verbose);
  addtoclosure(closure0, var_global);
  struct UserObject *uo0 = (struct UserObject*)self;
  uo0->data[1] = (Object)closure0;
  Method *meth_meth_PackageManager_parseArgs0 = addmethod2pos(self, "parseArgs", &meth_PackageManager_parseArgs0, 1);
int argcv_meth_PackageManager_parseArgs0[] = {1};
meth_meth_PackageManager_parseArgs0->type = alloc_MethodType(1, argcv_meth_PackageManager_parseArgs0);
  meth_meth_PackageManager_parseArgs0->definitionModule = modulename;
  meth_meth_PackageManager_parseArgs0->definitionLine = 14;
// compilenode returning 
// Begin line 40
  setline(40);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo60 = (struct UserObject*)self;
  uo60->data[2] = emptyclosure;
  Method *meth_meth_PackageManager_forArgs_40__41_do60 = addmethod2pos(self, "forArgs()do", &meth_PackageManager_forArgs_40__41_do60, 2);
int argcv_meth_PackageManager_forArgs_40__41_do60[] = {1, 1};
meth_meth_PackageManager_forArgs_40__41_do60->type = alloc_MethodType(2, argcv_meth_PackageManager_forArgs_40__41_do60);
  meth_meth_PackageManager_forArgs_40__41_do60->flags |= MFLAG_CONFIDENTIAL;
  meth_meth_PackageManager_forArgs_40__41_do60->definitionModule = modulename;
  meth_meth_PackageManager_forArgs_40__41_do60->definitionLine = 40;
  Method *meth_meth_PackageManager_forArgs_40__41_do60_object = addmethod2pos(self, "forArgs()do()object", &meth_PackageManager_forArgs_40__41_do60_object, 2);
int argcv_meth_PackageManager_forArgs_40__41_do60_object[] = {1, 1};
meth_meth_PackageManager_forArgs_40__41_do60_object->type = alloc_MethodType(2, argcv_meth_PackageManager_forArgs_40__41_do60_object);
  meth_meth_PackageManager_forArgs_40__41_do60_object->flags |= MFLAG_CONFIDENTIAL;
  meth_meth_PackageManager_forArgs_40__41_do60_object->definitionModule = modulename;
  meth_meth_PackageManager_forArgs_40__41_do60_object->definitionLine = 40;
// compilenode returning 
// Begin line 103
  setline(103);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure273 = createclosure(2, "listInstalled");
setclosureframe(closure273, stackframe);
  addtoclosure(closure273, var_io);
  addtoclosure(closure273, var_sys);
  struct UserObject *uo273 = (struct UserObject*)self;
  uo273->data[3] = (Object)closure273;
  Method *meth_meth_PackageManager_listInstalled273 = addmethod2pos(self, "listInstalled", &meth_PackageManager_listInstalled273, 3);
int argcv_meth_PackageManager_listInstalled273[] = {0};
meth_meth_PackageManager_listInstalled273->type = alloc_MethodType(1, argcv_meth_PackageManager_listInstalled273);
  meth_meth_PackageManager_listInstalled273->definitionModule = modulename;
  meth_meth_PackageManager_listInstalled273->definitionLine = 103;
// compilenode returning 
// Begin line 116
  setline(116);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure308 = createclosure(1, "recurseDirectory");
setclosureframe(closure308, stackframe);
  addtoclosure(closure308, var_io);
  struct UserObject *uo308 = (struct UserObject*)self;
  uo308->data[4] = (Object)closure308;
  Method *meth_meth_PackageManager_recurseDirectory308 = addmethod2pos(self, "recurseDirectory", &meth_PackageManager_recurseDirectory308, 4);
int argcv_meth_PackageManager_recurseDirectory308[] = {2};
meth_meth_PackageManager_recurseDirectory308->type = alloc_MethodType(1, argcv_meth_PackageManager_recurseDirectory308);
  meth_meth_PackageManager_recurseDirectory308->definitionModule = modulename;
  meth_meth_PackageManager_recurseDirectory308->definitionLine = 116;
// compilenode returning 
// Begin line 128
  setline(128);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure361 = createclosure(1, "doGet");
setclosureframe(closure361, stackframe);
  addtoclosure(closure361, var_imported);
  struct UserObject *uo361 = (struct UserObject*)self;
  uo361->data[5] = (Object)closure361;
  Method *meth_meth_PackageManager_doGet361 = addmethod2pos(self, "doGet", &meth_PackageManager_doGet361, 5);
int argcv_meth_PackageManager_doGet361[] = {1};
meth_meth_PackageManager_doGet361->type = alloc_MethodType(1, argcv_meth_PackageManager_doGet361);
  meth_meth_PackageManager_doGet361->definitionModule = modulename;
  meth_meth_PackageManager_doGet361->definitionLine = 128;
// compilenode returning 
// Begin line 139
  setline(139);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure380 = createclosure(1, "setFile");
setclosureframe(closure380, stackframe);
  addtoclosure(closure380, var_curFile);
  struct UserObject *uo380 = (struct UserObject*)self;
  uo380->data[6] = (Object)closure380;
  Method *meth_meth_PackageManager_setFile380 = addmethod2pos(self, "setFile", &meth_PackageManager_setFile380, 6);
int argcv_meth_PackageManager_setFile380[] = {1};
meth_meth_PackageManager_setFile380->type = alloc_MethodType(1, argcv_meth_PackageManager_setFile380);
  meth_meth_PackageManager_setFile380->definitionModule = modulename;
  meth_meth_PackageManager_setFile380->definitionLine = 139;
// compilenode returning 
// Begin line 151
  setline(151);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure403 = createclosure(3, "fetchImports");
setclosureframe(closure403, stackframe);
  addtoclosure(closure403, var_curFile);
  addtoclosure(closure403, var_imported);
  addtoclosure(closure403, var_toProcess);
  struct UserObject *uo403 = (struct UserObject*)self;
  uo403->data[7] = (Object)closure403;
  Method *meth_meth_PackageManager_fetchImports403 = addmethod2pos(self, "fetchImports", &meth_PackageManager_fetchImports403, 7);
int argcv_meth_PackageManager_fetchImports403[] = {1};
meth_meth_PackageManager_fetchImports403->type = alloc_MethodType(1, argcv_meth_PackageManager_fetchImports403);
  meth_meth_PackageManager_fetchImports403->definitionModule = modulename;
  meth_meth_PackageManager_fetchImports403->definitionLine = 151;
// compilenode returning 
// Begin line 176
  setline(176);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure479 = createclosure(1, "setFileData");
setclosureframe(closure479, stackframe);
  addtoclosure(closure479, var_curl);
  struct UserObject *uo479 = (struct UserObject*)self;
  uo479->data[8] = (Object)closure479;
  Method *meth_meth_PackageManager_setFileData479 = addmethod2pos(self, "setFileData", &meth_PackageManager_setFileData479, 8);
int argcv_meth_PackageManager_setFileData479[] = {1};
meth_meth_PackageManager_setFileData479->type = alloc_MethodType(1, argcv_meth_PackageManager_setFileData479);
  meth_meth_PackageManager_setFileData479->definitionModule = modulename;
  meth_meth_PackageManager_setFileData479->definitionLine = 176;
// compilenode returning 
// Begin line 212
  setline(212);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure598 = createclosure(3, "findExisting");
setclosureframe(closure598, stackframe);
  addtoclosure(closure598, var_io);
  addtoclosure(closure598, var_sys);
  addtoclosure(closure598, var_bundlePath);
  struct UserObject *uo598 = (struct UserObject*)self;
  uo598->data[9] = (Object)closure598;
  Method *meth_meth_PackageManager_findExisting598 = addmethod2pos(self, "findExisting", &meth_PackageManager_findExisting598, 9);
int argcv_meth_PackageManager_findExisting598[] = {1};
meth_meth_PackageManager_findExisting598->type = alloc_MethodType(1, argcv_meth_PackageManager_findExisting598);
  meth_meth_PackageManager_findExisting598->definitionModule = modulename;
  meth_meth_PackageManager_findExisting598->definitionLine = 212;
// compilenode returning 
// Begin line 242
  setline(242);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo795 = (struct UserObject*)self;
  uo795->data[10] = emptyclosure;
  Method *meth_meth_PackageManager_validateFile795 = addmethod2pos(self, "validateFile", &meth_PackageManager_validateFile795, 10);
int argcv_meth_PackageManager_validateFile795[] = {1};
meth_meth_PackageManager_validateFile795->type = alloc_MethodType(1, argcv_meth_PackageManager_validateFile795);
  meth_meth_PackageManager_validateFile795->definitionModule = modulename;
  meth_meth_PackageManager_validateFile795->definitionLine = 242;
// compilenode returning 
// Begin line 252
  setline(252);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure818 = createclosure(3, "write");
setclosureframe(closure818, stackframe);
  addtoclosure(closure818, var_global);
  addtoclosure(closure818, var_sys);
  addtoclosure(closure818, var_io);
  struct UserObject *uo818 = (struct UserObject*)self;
  uo818->data[11] = (Object)closure818;
  Method *meth_meth_PackageManager_write818 = addmethod2pos(self, "write", &meth_PackageManager_write818, 11);
int argcv_meth_PackageManager_write818[] = {1};
meth_meth_PackageManager_write818->type = alloc_MethodType(1, argcv_meth_PackageManager_write818);
  meth_meth_PackageManager_write818->definitionModule = modulename;
  meth_meth_PackageManager_write818->definitionLine = 252;
// compilenode returning 
// Begin line 267
  setline(267);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure855 = createclosure(2, "getBuildPath");
setclosureframe(closure855, stackframe);
  addtoclosure(closure855, var_sys);
  addtoclosure(closure855, var_io);
  struct UserObject *uo855 = (struct UserObject*)self;
  uo855->data[12] = (Object)closure855;
  Method *meth_meth_PackageManager_getBuildPath855 = addmethod2pos(self, "getBuildPath", &meth_PackageManager_getBuildPath855, 12);
int argcv_meth_PackageManager_getBuildPath855[] = {0};
meth_meth_PackageManager_getBuildPath855->type = alloc_MethodType(1, argcv_meth_PackageManager_getBuildPath855);
  meth_meth_PackageManager_getBuildPath855->definitionModule = modulename;
  meth_meth_PackageManager_getBuildPath855->definitionLine = 267;
// compilenode returning 
// Begin line 284
  setline(284);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure893 = createclosure(3, "compile");
setclosureframe(closure893, stackframe);
  addtoclosure(closure893, var_global);
  addtoclosure(closure893, var_sys);
  addtoclosure(closure893, var_io);
  struct UserObject *uo893 = (struct UserObject*)self;
  uo893->data[13] = (Object)closure893;
  Method *meth_meth_PackageManager_compile893 = addmethod2pos(self, "compile", &meth_PackageManager_compile893, 13);
int argcv_meth_PackageManager_compile893[] = {1};
meth_meth_PackageManager_compile893->type = alloc_MethodType(1, argcv_meth_PackageManager_compile893);
  meth_meth_PackageManager_compile893->definitionModule = modulename;
  meth_meth_PackageManager_compile893->definitionLine = 284;
// compilenode returning 
// Begin line 300
  setline(300);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure933 = createclosure(1, "createDirectory");
setclosureframe(closure933, stackframe);
  addtoclosure(closure933, var_io);
  struct UserObject *uo933 = (struct UserObject*)self;
  uo933->data[14] = (Object)closure933;
  Method *meth_meth_PackageManager_createDirectory933 = addmethod2pos(self, "createDirectory", &meth_PackageManager_createDirectory933, 14);
int argcv_meth_PackageManager_createDirectory933[] = {1};
meth_meth_PackageManager_createDirectory933->type = alloc_MethodType(1, argcv_meth_PackageManager_createDirectory933);
  meth_meth_PackageManager_createDirectory933->definitionModule = modulename;
  meth_meth_PackageManager_createDirectory933->definitionLine = 300;
// compilenode returning 
// Begin line 324
  setline(324);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1011 = createclosure(1, "parseFile");
setclosureframe(closure1011, stackframe);
  addtoclosure(closure1011, var_done);
  struct UserObject *uo1011 = (struct UserObject*)self;
  uo1011->data[15] = (Object)closure1011;
  Method *meth_meth_PackageManager_parseFile1011 = addmethod2pos(self, "parseFile", &meth_PackageManager_parseFile1011, 15);
int argcv_meth_PackageManager_parseFile1011[] = {1};
meth_meth_PackageManager_parseFile1011->type = alloc_MethodType(1, argcv_meth_PackageManager_parseFile1011);
  meth_meth_PackageManager_parseFile1011->definitionModule = modulename;
  meth_meth_PackageManager_parseFile1011->definitionLine = 324;
// compilenode returning 
// Begin line 341
  setline(341);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1071 = (struct UserObject*)self;
  uo1071->data[16] = emptyclosure;
  Method *meth_meth_PackageManager_processLine1071 = addmethod2pos(self, "processLine", &meth_PackageManager_processLine1071, 16);
int argcv_meth_PackageManager_processLine1071[] = {1};
meth_meth_PackageManager_processLine1071->type = alloc_MethodType(1, argcv_meth_PackageManager_processLine1071);
  meth_meth_PackageManager_processLine1071->definitionModule = modulename;
  meth_meth_PackageManager_processLine1071->definitionLine = 341;
// compilenode returning 
// Begin line 360
  setline(360);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1152 = createclosure(2, "parseImport");
setclosureframe(closure1152, stackframe);
  addtoclosure(closure1152, var_imported);
  addtoclosure(closure1152, var_toProcess);
  struct UserObject *uo1152 = (struct UserObject*)self;
  uo1152->data[17] = (Object)closure1152;
  Method *meth_meth_PackageManager_parseImport1152 = addmethod2pos(self, "parseImport", &meth_PackageManager_parseImport1152, 17);
int argcv_meth_PackageManager_parseImport1152[] = {1};
meth_meth_PackageManager_parseImport1152->type = alloc_MethodType(1, argcv_meth_PackageManager_parseImport1152);
  meth_meth_PackageManager_parseImport1152->definitionModule = modulename;
  meth_meth_PackageManager_parseImport1152->definitionLine = 360;
// compilenode returning 
// Begin line 377
  setline(377);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1223 = (struct UserObject*)self;
  uo1223->data[18] = emptyclosure;
  Method *meth_meth_PackageManager_skipWhiteSpace1223 = addmethod2pos(self, "skipWhiteSpace", &meth_PackageManager_skipWhiteSpace1223, 18);
int argcv_meth_PackageManager_skipWhiteSpace1223[] = {2};
meth_meth_PackageManager_skipWhiteSpace1223->type = alloc_MethodType(1, argcv_meth_PackageManager_skipWhiteSpace1223);
  meth_meth_PackageManager_skipWhiteSpace1223->definitionModule = modulename;
  meth_meth_PackageManager_skipWhiteSpace1223->definitionLine = 377;
// compilenode returning 
// Begin line 385
  setline(385);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1250 = (struct UserObject*)self;
  uo1250->data[19] = emptyclosure;
  Method *meth_meth_PackageManager_displayHelp1250 = addmethod2pos(self, "displayHelp", &meth_PackageManager_displayHelp1250, 19);
int argcv_meth_PackageManager_displayHelp1250[] = {0};
meth_meth_PackageManager_displayHelp1250->type = alloc_MethodType(1, argcv_meth_PackageManager_displayHelp1250);
  meth_meth_PackageManager_displayHelp1250->definitionModule = modulename;
  meth_meth_PackageManager_displayHelp1250->definitionLine = 385;
// compilenode returning 
// Begin line 391
  setline(391);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1257 = createclosure(1, "printMessage");
setclosureframe(closure1257, stackframe);
  addtoclosure(closure1257, var_verbose);
  struct UserObject *uo1257 = (struct UserObject*)self;
  uo1257->data[20] = (Object)closure1257;
  Method *meth_meth_PackageManager_printMessage1257 = addmethod2pos(self, "printMessage", &meth_PackageManager_printMessage1257, 20);
int argcv_meth_PackageManager_printMessage1257[] = {1};
meth_meth_PackageManager_printMessage1257->type = alloc_MethodType(1, argcv_meth_PackageManager_printMessage1257);
  meth_meth_PackageManager_printMessage1257->definitionModule = modulename;
  meth_meth_PackageManager_printMessage1257->definitionLine = 391;
// compilenode returning 
// Begin line 400
  setline(400);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1260 = createclosure(3, "bundle");
setclosureframe(closure1260, stackframe);
  addtoclosure(closure1260, var_imported);
  addtoclosure(closure1260, var_bundlePath);
  addtoclosure(closure1260, var_io);
  struct UserObject *uo1260 = (struct UserObject*)self;
  uo1260->data[21] = (Object)closure1260;
  Method *meth_meth_PackageManager_bundle1260 = addmethod2pos(self, "bundle", &meth_PackageManager_bundle1260, 21);
int argcv_meth_PackageManager_bundle1260[] = {2};
meth_meth_PackageManager_bundle1260->type = alloc_MethodType(1, argcv_meth_PackageManager_bundle1260);
  meth_meth_PackageManager_bundle1260->definitionModule = modulename;
  meth_meth_PackageManager_bundle1260->definitionLine = 400;
// compilenode returning 
// Begin line 418
  setline(418);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1363 = createclosure(1, "setImportDest");
setclosureframe(closure1363, stackframe);
  addtoclosure(closure1363, var_io);
  struct UserObject *uo1363 = (struct UserObject*)self;
  uo1363->data[22] = (Object)closure1363;
  Method *meth_meth_PackageManager_setImportDest1363 = addmethod2pos(self, "setImportDest", &meth_PackageManager_setImportDest1363, 22);
int argcv_meth_PackageManager_setImportDest1363[] = {2};
meth_meth_PackageManager_setImportDest1363->type = alloc_MethodType(1, argcv_meth_PackageManager_setImportDest1363);
  meth_meth_PackageManager_setImportDest1363->definitionModule = modulename;
  meth_meth_PackageManager_setImportDest1363->definitionLine = 418;
// compilenode returning 
// Begin line 442
  setline(442);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1439 = (struct UserObject*)self;
  uo1439->data[23] = emptyclosure;
  Method *meth_meth_PackageManager_parseAndPrefix1439 = addmethod2pos(self, "parseAndPrefix", &meth_PackageManager_parseAndPrefix1439, 23);
int argcv_meth_PackageManager_parseAndPrefix1439[] = {3};
meth_meth_PackageManager_parseAndPrefix1439->type = alloc_MethodType(1, argcv_meth_PackageManager_parseAndPrefix1439);
meth_meth_PackageManager_parseAndPrefix1439->type->types[0] = type_String;
meth_meth_PackageManager_parseAndPrefix1439->type->names[0] = "readFile";
meth_meth_PackageManager_parseAndPrefix1439->type->types[1] = type_String;
meth_meth_PackageManager_parseAndPrefix1439->type->names[1] = "address";
meth_meth_PackageManager_parseAndPrefix1439->type->types[2] = type_String;
meth_meth_PackageManager_parseAndPrefix1439->type->names[2] = "prefix";
  meth_meth_PackageManager_parseAndPrefix1439->definitionModule = modulename;
  meth_meth_PackageManager_parseAndPrefix1439->definitionLine = 442;
// compilenode returning 
// Begin line 492
  setline(492);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1640 = (struct UserObject*)self;
  uo1640->data[24] = emptyclosure;
  Method *meth_meth_PackageManager_removeExistingUrls1640 = addmethod2pos(self, "removeExistingUrls", &meth_PackageManager_removeExistingUrls1640, 24);
int argcv_meth_PackageManager_removeExistingUrls1640[] = {1};
meth_meth_PackageManager_removeExistingUrls1640->type = alloc_MethodType(1, argcv_meth_PackageManager_removeExistingUrls1640);
meth_meth_PackageManager_removeExistingUrls1640->type->types[0] = type_String;
meth_meth_PackageManager_removeExistingUrls1640->type->names[0] = "importStatement";
  meth_meth_PackageManager_removeExistingUrls1640->definitionModule = modulename;
  meth_meth_PackageManager_removeExistingUrls1640->definitionLine = 492;
// compilenode returning 
// Begin line 516
  setline(516);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1692 = (struct UserObject*)self;
  uo1692->data[25] = emptyclosure;
  Method *meth_meth_PackageManager_removeContainingDir1692 = addmethod2pos(self, "removeContainingDir", &meth_PackageManager_removeContainingDir1692, 25);
int argcv_meth_PackageManager_removeContainingDir1692[] = {1};
meth_meth_PackageManager_removeContainingDir1692->type = alloc_MethodType(1, argcv_meth_PackageManager_removeContainingDir1692);
meth_meth_PackageManager_removeContainingDir1692->type->types[0] = type_String;
meth_meth_PackageManager_removeContainingDir1692->type->names[0] = "st";
  meth_meth_PackageManager_removeContainingDir1692->definitionModule = modulename;
  meth_meth_PackageManager_removeContainingDir1692->definitionLine = 516;
// compilenode returning 
// Begin line 531
  setline(531);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1737 = (struct UserObject*)self;
  uo1737->data[26] = emptyclosure;
  Method *meth_meth_PackageManager_getContainingDirectory1737 = addmethod2pos(self, "getContainingDirectory", &meth_PackageManager_getContainingDirectory1737, 26);
int argcv_meth_PackageManager_getContainingDirectory1737[] = {1};
meth_meth_PackageManager_getContainingDirectory1737->type = alloc_MethodType(1, argcv_meth_PackageManager_getContainingDirectory1737);
meth_meth_PackageManager_getContainingDirectory1737->type->types[0] = type_String;
meth_meth_PackageManager_getContainingDirectory1737->type->names[0] = "st";
  meth_meth_PackageManager_getContainingDirectory1737->definitionModule = modulename;
  meth_meth_PackageManager_getContainingDirectory1737->definitionLine = 531;
// compilenode returning 
// Begin line 1
  setline(1);
  setmodule(modulename);
  setsource(originalSourceLines);
// Import of sys as sys
  if (module_sys == NULL)
    module_sys = module_sys_init();
  *var_sys = module_sys;
// compilenode returning done
// Begin line 2
  setline(2);
  setmodule(modulename);
  setsource(originalSourceLines);
// Import of io as io
  if (module_io == NULL)
    module_io = module_io_init();
  *var_io = module_io;
// compilenode returning done
// Begin line 3
  setline(3);
  setmodule(modulename);
  setsource(originalSourceLines);
// Import of curl as curl
  if (module_curl == NULL)
    module_curl = dlmodule("curl");
  *var_curl = module_curl;
// compilenode returning done
// compilenode returning 
// Begin line 6
  setline(6);
  setmodule(modulename);
  setsource(originalSourceLines);
  *var_imported = undefined;
// Begin line 534
  setline(534);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1787 = createclosure(1, "imported");
setclosureframe(closure1787, stackframe);
  addtoclosure(closure1787, var_imported);
  struct UserObject *uo1787 = (struct UserObject*)self;
  uo1787->data[27] = (Object)closure1787;
  Method *meth_meth_PackageManager_imported1787 = addmethod2pos(self, "imported", &meth_PackageManager_imported1787, 27);
int argcv_meth_PackageManager_imported1787[] = {0};
meth_meth_PackageManager_imported1787->type = alloc_MethodType(1, argcv_meth_PackageManager_imported1787);
  meth_meth_PackageManager_imported1787->definitionModule = modulename;
  meth_meth_PackageManager_imported1787->definitionLine = 534;
// compilenode returning 
// Begin line 534
  setline(534);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1788 = createclosure(1, "imported:=");
setclosureframe(closure1788, stackframe);
  addtoclosure(closure1788, var_imported);
  struct UserObject *uo1788 = (struct UserObject*)self;
  uo1788->data[28] = (Object)closure1788;
  Method *meth_meth_PackageManager_imported_58__61_1788 = addmethod2pos(self, "imported:=", &meth_PackageManager_imported_58__61_1788, 28);
int argcv_meth_PackageManager_imported_58__61_1788[] = {1};
meth_meth_PackageManager_imported_58__61_1788->type = alloc_MethodType(1, argcv_meth_PackageManager_imported_58__61_1788);
  meth_meth_PackageManager_imported_58__61_1788->definitionModule = modulename;
  meth_meth_PackageManager_imported_58__61_1788->definitionLine = 534;
// compilenode returning 
// compilenode returning done
// Begin line 7
  setline(7);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 6
  setline(6);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array1790 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array1790
  *var_toProcess = array1790;
  if (array1790 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 534
  setline(534);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1791 = createclosure(1, "toProcess");
setclosureframe(closure1791, stackframe);
  addtoclosure(closure1791, var_toProcess);
  struct UserObject *uo1791 = (struct UserObject*)self;
  uo1791->data[29] = (Object)closure1791;
  Method *meth_meth_PackageManager_toProcess1791 = addmethod2pos(self, "toProcess", &meth_PackageManager_toProcess1791, 29);
int argcv_meth_PackageManager_toProcess1791[] = {0};
meth_meth_PackageManager_toProcess1791->type = alloc_MethodType(1, argcv_meth_PackageManager_toProcess1791);
  meth_meth_PackageManager_toProcess1791->definitionModule = modulename;
  meth_meth_PackageManager_toProcess1791->definitionLine = 534;
// compilenode returning 
// Begin line 534
  setline(534);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1792 = createclosure(1, "toProcess:=");
setclosureframe(closure1792, stackframe);
  addtoclosure(closure1792, var_toProcess);
  struct UserObject *uo1792 = (struct UserObject*)self;
  uo1792->data[30] = (Object)closure1792;
  Method *meth_meth_PackageManager_toProcess_58__61_1792 = addmethod2pos(self, "toProcess:=", &meth_PackageManager_toProcess_58__61_1792, 30);
int argcv_meth_PackageManager_toProcess_58__61_1792[] = {1};
meth_meth_PackageManager_toProcess_58__61_1792->type = alloc_MethodType(1, argcv_meth_PackageManager_toProcess_58__61_1792);
  meth_meth_PackageManager_toProcess_58__61_1792->definitionModule = modulename;
  meth_meth_PackageManager_toProcess_58__61_1792->definitionLine = 534;
// compilenode returning 
// compilenode returning done
// Begin line 8
  setline(8);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 7
  setline(7);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1794 = alloc_Boolean(0);
// compilenode returning bool1794
  *var_verbose = bool1794;
  if (bool1794 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 534
  setline(534);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1795 = createclosure(1, "verbose");
setclosureframe(closure1795, stackframe);
  addtoclosure(closure1795, var_verbose);
  struct UserObject *uo1795 = (struct UserObject*)self;
  uo1795->data[31] = (Object)closure1795;
  Method *meth_meth_PackageManager_verbose1795 = addmethod2pos(self, "verbose", &meth_PackageManager_verbose1795, 31);
int argcv_meth_PackageManager_verbose1795[] = {0};
meth_meth_PackageManager_verbose1795->type = alloc_MethodType(1, argcv_meth_PackageManager_verbose1795);
  meth_meth_PackageManager_verbose1795->definitionModule = modulename;
  meth_meth_PackageManager_verbose1795->definitionLine = 534;
// compilenode returning 
// Begin line 534
  setline(534);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1796 = createclosure(1, "verbose:=");
setclosureframe(closure1796, stackframe);
  addtoclosure(closure1796, var_verbose);
  struct UserObject *uo1796 = (struct UserObject*)self;
  uo1796->data[32] = (Object)closure1796;
  Method *meth_meth_PackageManager_verbose_58__61_1796 = addmethod2pos(self, "verbose:=", &meth_PackageManager_verbose_58__61_1796, 32);
int argcv_meth_PackageManager_verbose_58__61_1796[] = {1};
meth_meth_PackageManager_verbose_58__61_1796->type = alloc_MethodType(1, argcv_meth_PackageManager_verbose_58__61_1796);
  meth_meth_PackageManager_verbose_58__61_1796->definitionModule = modulename;
  meth_meth_PackageManager_verbose_58__61_1796->definitionLine = 534;
// compilenode returning 
// compilenode returning done
// Begin line 9
  setline(9);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 8
  setline(8);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1798 = alloc_Boolean(0);
// compilenode returning bool1798
  *var_global = bool1798;
  if (bool1798 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 534
  setline(534);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1799 = createclosure(1, "global");
setclosureframe(closure1799, stackframe);
  addtoclosure(closure1799, var_global);
  struct UserObject *uo1799 = (struct UserObject*)self;
  uo1799->data[33] = (Object)closure1799;
  Method *meth_meth_PackageManager_global1799 = addmethod2pos(self, "global", &meth_PackageManager_global1799, 33);
int argcv_meth_PackageManager_global1799[] = {0};
meth_meth_PackageManager_global1799->type = alloc_MethodType(1, argcv_meth_PackageManager_global1799);
  meth_meth_PackageManager_global1799->definitionModule = modulename;
  meth_meth_PackageManager_global1799->definitionLine = 534;
// compilenode returning 
// Begin line 534
  setline(534);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1800 = createclosure(1, "global:=");
setclosureframe(closure1800, stackframe);
  addtoclosure(closure1800, var_global);
  struct UserObject *uo1800 = (struct UserObject*)self;
  uo1800->data[34] = (Object)closure1800;
  Method *meth_meth_PackageManager_global_58__61_1800 = addmethod2pos(self, "global:=", &meth_PackageManager_global_58__61_1800, 34);
int argcv_meth_PackageManager_global_58__61_1800[] = {1};
meth_meth_PackageManager_global_58__61_1800->type = alloc_MethodType(1, argcv_meth_PackageManager_global_58__61_1800);
  meth_meth_PackageManager_global_58__61_1800->definitionModule = modulename;
  meth_meth_PackageManager_global_58__61_1800->definitionLine = 534;
// compilenode returning 
// compilenode returning done
// Begin line 9
  setline(9);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1802 == NULL) {
    strlit1802 = alloc_String("");
    gc_root(strlit1802);
  }
// compilenode returning strlit1802
  *var_bundlePath = strlit1802;
  if (strlit1802 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 534
  setline(534);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1803 = createclosure(1, "bundlePath");
setclosureframe(closure1803, stackframe);
  addtoclosure(closure1803, var_bundlePath);
  struct UserObject *uo1803 = (struct UserObject*)self;
  uo1803->data[35] = (Object)closure1803;
  Method *meth_meth_PackageManager_bundlePath1803 = addmethod2pos(self, "bundlePath", &meth_PackageManager_bundlePath1803, 35);
int argcv_meth_PackageManager_bundlePath1803[] = {0};
meth_meth_PackageManager_bundlePath1803->type = alloc_MethodType(1, argcv_meth_PackageManager_bundlePath1803);
  meth_meth_PackageManager_bundlePath1803->definitionModule = modulename;
  meth_meth_PackageManager_bundlePath1803->definitionLine = 534;
// compilenode returning 
// Begin line 534
  setline(534);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1804 = createclosure(1, "bundlePath:=");
setclosureframe(closure1804, stackframe);
  addtoclosure(closure1804, var_bundlePath);
  struct UserObject *uo1804 = (struct UserObject*)self;
  uo1804->data[36] = (Object)closure1804;
  Method *meth_meth_PackageManager_bundlePath_58__61_1804 = addmethod2pos(self, "bundlePath:=", &meth_PackageManager_bundlePath_58__61_1804, 36);
int argcv_meth_PackageManager_bundlePath_58__61_1804[] = {1};
meth_meth_PackageManager_bundlePath_58__61_1804->type = alloc_MethodType(1, argcv_meth_PackageManager_bundlePath_58__61_1804);
  meth_meth_PackageManager_bundlePath_58__61_1804->definitionModule = modulename;
  meth_meth_PackageManager_bundlePath_58__61_1804->definitionLine = 534;
// compilenode returning 
// compilenode returning done
// Begin line 10
  setline(10);
  setmodule(modulename);
  setsource(originalSourceLines);
  *var_curFile = undefined;
// Begin line 534
  setline(534);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1806 = createclosure(1, "curFile");
setclosureframe(closure1806, stackframe);
  addtoclosure(closure1806, var_curFile);
  struct UserObject *uo1806 = (struct UserObject*)self;
  uo1806->data[37] = (Object)closure1806;
  Method *meth_meth_PackageManager_curFile1806 = addmethod2pos(self, "curFile", &meth_PackageManager_curFile1806, 37);
int argcv_meth_PackageManager_curFile1806[] = {0};
meth_meth_PackageManager_curFile1806->type = alloc_MethodType(1, argcv_meth_PackageManager_curFile1806);
  meth_meth_PackageManager_curFile1806->definitionModule = modulename;
  meth_meth_PackageManager_curFile1806->definitionLine = 534;
// compilenode returning 
// Begin line 534
  setline(534);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1807 = createclosure(1, "curFile:=");
setclosureframe(closure1807, stackframe);
  addtoclosure(closure1807, var_curFile);
  struct UserObject *uo1807 = (struct UserObject*)self;
  uo1807->data[38] = (Object)closure1807;
  Method *meth_meth_PackageManager_curFile_58__61_1807 = addmethod2pos(self, "curFile:=", &meth_PackageManager_curFile_58__61_1807, 38);
int argcv_meth_PackageManager_curFile_58__61_1807[] = {1};
meth_meth_PackageManager_curFile_58__61_1807->type = alloc_MethodType(1, argcv_meth_PackageManager_curFile_58__61_1807);
  meth_meth_PackageManager_curFile_58__61_1807->definitionModule = modulename;
  meth_meth_PackageManager_curFile_58__61_1807->definitionLine = 534;
// compilenode returning 
// compilenode returning done
// Begin line 12
  setline(12);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
  int callframe1809 = gc_frame_new();
  int callframe1810 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call1811 = callmethod(*var_sys, "argv",
    1, partcv, params);
  gc_frame_end(callframe1810);
// compilenode returning call1811
// compilenode returning call1811
  gc_frame_newslot(call1811);
  params[0] = call1811;
  partcv[0] = 1;
  Object call1812 = callmethodflags(self, "parseArgs", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1809);
// compilenode returning call1812
// Begin line 14
  setline(14);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// compilenode returning 
// Begin line 40
  setline(40);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 103
  setline(103);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 116
  setline(116);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 128
  setline(128);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 139
  setline(139);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 151
  setline(151);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 176
  setline(176);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 212
  setline(212);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 242
  setline(242);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 252
  setline(252);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 267
  setline(267);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 284
  setline(284);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 300
  setline(300);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 324
  setline(324);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 341
  setline(341);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 360
  setline(360);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 377
  setline(377);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 385
  setline(385);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 391
  setline(391);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 400
  setline(400);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 418
  setline(418);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 442
  setline(442);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 492
  setline(492);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 516
  setline(516);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
  gc_frame_end(frame);
  return self;
}
int main(int argc, char **argv) {
  initprofiling();
  setCompilerModulePath("/am/rialto/home1/forretcrai/MiniGrace");
  gracelib_argv(argv);
  Object params[1];
  undefined = alloc_Undefined();
  done = alloc_done();
  Object tmp_argv = alloc_BuiltinList();
  gc_root(tmp_argv);
  int partcv_push[] = {1};
  int i; for (i=0; i<argc; i++) {
    params[0] = alloc_String(argv[i]);
    callmethod(tmp_argv, "push", 1, partcv_push, params);
  }
  module_sys_init_argv(tmp_argv);
  module_PackageManager_init();
  gracelib_stats();
  return 0;
}
