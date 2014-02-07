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
  "var bundlePath := \"\"",
  "var baseUrl := \"\"",
  "var curFile",
  "var build := false",
  "var install := false",
  "",
  "parseArgs(sys.argv)",
  "",
  "method parseArgs(args : List<String>) {",
  "    var count := 0",
  "    print(args.size)",
  "    forArgs(args) do { arg, on ->",
  "        print(\"in loop\")",
  "        on.option \"get\" do { toGet->",
  "            doGet(toGet)",
  "        }",
  "        on.flag \"list\" do { ",
  "            listInstalled()",
  "        }",
  "        on.doubleValue \"bundle\" do { toBundle, name ->",
  "            bundle(toBundle,name)",
  "        }",
  "        on.doubleValue \"set-address\" do { address, prefix ->",
  "            setImportDest(address,prefix)",
  "        }",
  "",
  "        on.option \"package\" do { folder ->",
  "            package(folder)",
  "        }",
  "",
  "        on.flag \"--verbose\" do { ",
  "            verbose := true",
  "        }",
  "        on.flag \"--global\" do {",
  "            global := true",
  "        }",
  "",
  "        on.flag \"--build\" do {",
  "            build := true;",
  "        }",
  "",
  "        on.flag \"--install\" do {",
  "            install := true;",
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
  "        method option(name : String) do(block') {",
  "            def arg = args.at(i)",
  "            if(arg == name) then {",
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
  "            if(arg == name) then {",
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
  "        method flag(name : String) do(block') {",
  "            def arg = args.at(i)",
  "            if(arg == name) then {",
  "                block'.apply",
  "                ran := true",
  "            }",
  "        }",
  "    }",
  "    while { i <= size } do {",
  "        def arg = args.at(i)",
  "        print(arg)",
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
  "    checkListPath(\"usr/lib/grace/packages/\")",
  "    checkListPath(\"usr/lib/grace/packages/\")",
  "    checkListPath(sys.environ[\"HOME\"]++\"/grace/packages/\")",
  "    checkListPath(sys.environ[\"HOME\"]++\"/.local/lib/grace/modules/\")",
  "}",
  "",
  "method checkListPath(path : String){",
  "    ",
  "    if (io.exists(path))then{",
  "        print(\" Installed in path: {path}\")",
  "        recurseDirectory(path,\" \")",
  "    }",
  "}",
  "",
  "method recurseDirectory(path,padding){",
  "    var pathContents := io.getdir(path)",
  "    for (pathContents) do { p-> ",
  "        if ((p != \"..\") && (p != \".\"))then{",
  "            print(padding++p)",
  "            if(io.getdir(path++\"/\"++p).size > 0)then{",
  "                recurseDirectory((path++\"/\"++p),padding++\"   \")",
  "            }",
  "        }",
  "    }",
  "}",
  "",
  "method doGet(impAddress){",
  "    imported := []",
  "",
  "    if ((impAddress.size >= 7) && (impAddress.substringFrom(1)to(7) == \"http://\"))then{",
  "        setBaseUrl(impAddress)",
  "        if (impAddress.substringFrom(impAddress.size-6)to(impAddress.size) == \".tar.gz\")then{",
  "            getPackage(impAddress)",
  "            return",
  "        }",
  "        ",
  "    }",
  "    fetchImports(impAddress)",
  "    for(imported)do { im->",
  "        write(im)",
  "    }",
  "    for(imported)do { im->",
  "        compile(im)",
  "    }",
  "}",
  "",
  "method getPackage(impAddress){",
  "",
  "",
  "",
  "",
  "}",
  "",
  "method setBaseUrl(baseAddress: String){",
  "    baseUrl := getBaseUrl(baseAddress)",
  "}",
  "",
  "method setFile(fileAddress){",
  "    if (fileAddress.substringFrom(fileAddress.size-5)to(fileAddress.size) == \".grace\")then{",
  "        var file := object{",
  "            var address is public := fileAddress",
  "            var data is public",
  "        }",
  "        curFile := file",
  "        return true",
  "    }",
  "    return false",
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
  "            print(\"Could not locate file. Check file address.\")",
  "            return false",
  "        }",
  "    }",
  "}",
  "",
  "method performCurlFetch(file) -> Boolean{",
  "    var req := curl.easy",
  "    req.url := file.address",
  "    print(\"Searching for import: \"++file.address)",
  "    print(\" Connecting...\")",
  "    req.onReceive {d->",
  "        print(\" Response received\")   ",
  "        if(req.responseCode != 200)then{",
  "            print(\" Could not locate import: Error {req.responseCode} for import {file.address}\")",
  "            return false",
  "        }",
  "        file.data := d.decode(\"utf-8\")",
  "        return true",
  "    }",
  "    req.perform",
  "}",
  "",
  "method setFileData(file) -> Boolean {",
  "    if (file.address.substringFrom(1)to(4) == \"http\")then{",
  "        var strippedUrl := file.address.substringFrom(1)to(4)",
  "        if (findExisting(file.address) != false)then{",
  "            var findData := findExisting(strippedUrl)",
  "            if (findData > 0)then{",
  "                file.data := findData",
  "                return true",
  "            }",
  "            return false;",
  "        }",
  "        return performCurlFetch(file)",
  "        ",
  "    }",
  "    elseif (findExisting(file.address) != false)then{",
  "        print(\"Now searching in find existing\")",
  "        var findData := findExisting(file.address)",
  "        if (findData != false)then{",
  "            file.data := findData",
  "            return true",
  "        }",
  "    }",
  "    elseif (baseUrl != \"\")then{",
  "",
  "        file.address := baseUrl++file.address",
  "        return performCurlFetch(file)",
  "",
  "    }",
  "    return false",
  "}",
  "",
  "method findExisting(fileName){",
  "    if(io.exists(fileName))then{",
  "        var open := io.open(fileName,\"r\")",
  "        var ret := open.read",
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
  "        return io.open(\"{getBuildPath()}/{fileName}\",\"r\").read ",
  "    } ",
  "    if(bundlePath != \"\")then{",
  "        return io.open(\"{bundlePath}/{fileName}\",\"r\").read",
  "    }",
  "",
  "    return false",
  "}",
  "",
  "method validateFile(file) -> Boolean{",
  "    if ((file.data.size)>1)then{",
  "        if(file.data[1]==\"<\")then{",
  "            print(\"Not a valid grace file\")",
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
  "    toWrite.write(file.data)",
  "    toWrite.close",
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
  "    return buildPath",
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
  "    var buildPath := getBuildPath()",
  "    io.system(buildPath++\"/minigrace \"++usrDir++file.address)",
  "    return true",
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
  "        count := count+1",
  "    }",
  "    print(\"Creating directory \"++toMake)",
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
  "    var curPos := 1",
  "    var startPos := curPos",
  "    while{curPos <= data.size}do{",
  "        startPos := curPos",
  "        while {(curPos <= data.size) && (data[curPos] != \"\\n\")}do{",
  "            curPos := curPos+1",
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
  "            parseImport(line.substringFrom(8)to(line.size))",
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
  "method bundle(toBundle,name){",
  "    imported := []",
  "   // bundlePath := getContainingDirectory(toBundle)",
  "    print(\"In bundle. toBundle = {toBundle}\")",
  "    var newDir := createDirectory(\"{getContainingDirectory(toBundle)}../{name}/\")[1]",
  "    var folder := io.listdir(toBundle)",
  "    for(folder) do { file-> ",
  "        if (file.size > 4)then{",
  "            if (file.substringFrom(file.size-5)to(file.size) == \".grace\")then{",
  "                print(\"FILE = {file}\")",
  "                var open := io.open(\"{toBundle}{file}\",\"r\")",
  "                var openData := open.read",
  "                var toWrite := io.open(\"{newDir}{file}\", \"w\")",
  "                toWrite.write(openData)",
  "                toWrite.close",
  "                fetchImports(newDir++file)",
  "                print(\"Imported size = \"++imported.size)",
  "                while{imported.size > 0 }do{",
  "                    var curImport := imported.pop",
  "                    toWrite := io.open(\"{newDir}/{removeContainingDir(curImport.address)}\",\"w\")",
  "                    toWrite.write(curImport.data)   ",
  "                    toWrite.close",
  "                }",
  "            }",
  "        }",
  "    }",
  "}",
  "",
  "method setImportDest(address,prefix){",
  "    var folder := io.getdir(address)",
  "    print(\"Setting import dest\");",
  "    for(folder) do { file-> ",
  "        if (file.size > 4)then{",
  "            if (file.substringFrom(file.size-5)to(file.size) == \".grace\")then{",
  "                var open := io.open(address++\"/\"++file,\"r\")",
  "                var readFile := open.read",
  "                var toWrite := parseAndPrefix(readFile,address,prefix)",
  "                open.close",
  "                io.system(\"rm \"++address++\"/\"++file)",
  "                var out := io.open(address++\"/\"++file,\"w\")",
  "                for (toWrite) do { d-> ",
  "                    out.write(d)",
  "                }",
  "            }",
  "        }",
  "    }",
  "}",
  "",
  "",
  "method parseAndPrefix (readFile: String, address : String,  prefix : String){ ",
  "    var curPos := 1",
  "    var startPos := curPos",
  "    var outFile := []",
  "    for (0..readFile.size) do { t-> ",
  "        while{curPos <= readFile.size}do{",
  "            startPos := curPos",
  "            while {(curPos <= readFile.size) && (readFile[curPos] != \"\\n\")}do{",
  "                curPos := curPos+1",
  "            }",
  "            var data := readFile.substringFrom(startPos)to(curPos-1)",
  "            var line := \"\"",
  "            var count := 1",
  "            for (startPos..(curPos-1)) do { b-> ",
  "                line := line++readFile[b]",
  "            }",
  "            print(line)",
  "            if ((line.size > 6) && (line.substringFrom(1)to(7) == \"import \"))then{",
  "                var pos := 1",
  "                var start := pos",
  "                var nextImport := \"\"",
  "                pos := skipWhiteSpace(line,pos)",
  "                pos:= pos +1 ",
  "                print(\"LINE = \"++line)",
  "                while{((pos < line.size) && !((line[pos] == \"\\\"\")||(line[pos] == \" \")))} do {",
  "                    pos := pos+1",
  "                }",
  "                pos:= pos+1",
  "                var remainder := removeExistingUrls(line.substringFrom(pos+1)to(line.size))",
  "                line := \"import \\\"\"++prefix++\"/\"++remainder",
  "                print(line)",
  "                for (line) do{ d->",
  "                    outFile.push(d)",
  "                }",
  "                outFile.push(\"\\n\")",
  "            }",
  "            else{",
  "                for (line) do{ d->",
  "                    outFile.push(d)",
  "                }",
  "                outFile.push(\"\\n\")",
  "            }",
  "            curPos := curPos + 1",
  "        }",
  "    }",
  "    outFile.push(\"\\n\")",
  "",
  "",
  "    print (\"OUTFILE \"++outFile)",
  "    return outFile",
  "",
  "}",
  "",
  "method removeExistingUrls(importStatement : String) -> String{",
  "    if (importStatement.size < 7)then{",
  "        return importStatement",
  "    }",
  "    if (importStatement.substringFrom(1)to(7) == \"http://\" )then{",
  "        var lastIndex := 7",
  "        var curPos := 7",
  "        while {curPos <= importStatement.size}do{",
  "            if (importStatement[curPos] == \"/\")then{",
  "                lastIndex := curPos",
  "            }",
  "            curPos := curPos +1",
  "        }",
  "        var res := importStatement.substringFrom(lastIndex+1)to(importStatement.size)",
  "        return res",
  "    }",
  "    else{",
  "        return importStatement",
  "    }",
  "",
  "}",
  "",
  "method getBaseUrl(importStatement : String) -> String{",
  "",
  "    if (importStatement.size < 7)then{",
  "        return importStatement",
  "    }",
  "    if (importStatement.substringFrom(1)to(7) == \"http://\" )then{",
  "        var lastIndex := 7",
  "        var curPos := 7",
  "        while {curPos <= importStatement.size}do{",
  "            if (importStatement[curPos] == \"/\")then{",
  "                lastIndex := curPos",
  "            }",
  "            curPos := curPos +1",
  "        }",
  "        var res := importStatement.substringFrom(1)to(lastIndex)",
  "        return res",
  "    }",
  "    else{",
  "        return importStatement",
  "    }",
  "",
  "}",
  "",
  "method removeContainingDir(st:String) -> String{",
  "    var count := 1",
  "    var lastIndex := -1",
  "    while{count <= st.size}do{",
  "        if (st[count] == \"/\") then{",
  "            lastIndex := count",
  "        }",
  "        count := count+1",
  "    }",
  "    if(lastIndex == -1)then{",
  "        return st",
  "    }",
  "    return st.substringFrom(lastIndex+1)to(st.size); ",
  "}",
  "",
  "method getContainingDirectory(st : String) -> String{",
  "    var count := 1",
  "    var last := -1",
  "    while {count <= st.size}do{",
  "        if (st[count] == \"/\")then{",
  "            last := count",
  "        }",
  "        count := count+1",
  "    }",
  "    if (last == -1)then{",
  "        return getBuildPath()++\"/\"",
  "    }",
  "    return st.substringFrom(0)to(last-1)",
  "",
  "}",
  "",
  "method package (folder : String){",
  "    var setupFile := \"~/Packagemanager/setup.grace\"",
  "    var buildPath := getBuildPath()",
  "    if (!io.exists(folder++\"/pkg.grace\"))then{",
  "        print(\"Folder must contain pkg.grace file in order to create package\")",
  "    }",
  "    var open := io.open(\"{folder}/pkg.grace\",\"r\")",
  "    var pkgData := open.read",
  "    open.close",
  "    var loc := \"{buildPath}/__pkg-temp\"",
  "    var create := io.open(\"{loc}\",\"w\")",
  "    pkgData := pkgData++\"\\ndef __bundle = true\"",
  "    pkgData := pkgData++\"\\ndef __loc = \\\"{folder}\\\"\"",
  "    create.write(pkgData) ",
  "    create.close",
  "    io.system(\"mv {loc} {loc}.grace\")",
  "    print(\"build path = {buildPath} ++ minigrace\")",
  "    io.system(\"cat {setupFile} | {buildPath}/minigrace\")",
  "",
  "    var suffix := [\".grace\",\".gct\",\".gcn\",\".c\"];",
  "    for (suffix) do {s->",
  "        io.system(\"rm {buildPath}/__pkg-temp{s}\")",
  "    }",
  "",
  "}",
  "",
  NULL
};
Object reader_PackageManager_outer_90(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject*)self;
  return uo->data[0];
}
Object reader_PackageManager_outer_413(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject*)self;
  return uo->data[0];
}
Object reader_PackageManager_address_414(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  return uo->data[1];
}
Object writer_PackageManager_address_414(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  uo->data[1] = args[0];
  return done;
}
Object reader_PackageManager_data_415(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  return uo->data[2];
}
Object writer_PackageManager_data_415(Object self, int nparams, int *argcv, Object* args, int flags) {
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
static Object strlit59;
static Object strlit67;
static Object strlit75;
static ClassData objclass89;
static Object strlit107;
static Object strlit110;
static Object strlit147;
static Object strlit150;
static Object strlit210;
static Object strlit220;
static Object strlit223;
static Object strlit238;
static Object strlit241;
static Object strlit244;
static Object strlit250;
static Object strlit252;
static Object strlit260;
static Object strlit262;
static Object strlit272;
static Object strlit275;
static Object strlit280;
static Object strlit291;
static Object strlit295;
static Object strlit310;
static Object strlit323;
static Object strlit329;
static Object strlit351;
static Object strlit370;
static Object strlit409;
static ClassData objclass412;
static Object strlit425;
static Object strlit430;
static Object strlit441;
static Object strlit449;
static Object strlit454;
static Object strlit485;
static Object strlit492;
static Object strlit503;
static Object strlit509;
static Object strlit514;
static Object strlit527;
static Object strlit532;
static Object strlit539;
static Object strlit546;
static Object strlit563;
static Object strlit603;
static Object strlit619;
static Object strlit637;
static Object strlit644;
static Object strlit651;
static Object strlit654;
static Object strlit659;
static Object strlit666;
static Object strlit669;
static Object strlit680;
static Object strlit683;
static Object strlit688;
static Object strlit696;
static Object strlit701;
static Object strlit712;
static Object strlit717;
static Object strlit722;
static Object strlit730;
static Object strlit735;
static Object strlit746;
static Object strlit751;
static Object strlit756;
static Object strlit765;
static Object strlit770;
static Object strlit775;
static Object strlit785;
static Object strlit790;
static Object strlit795;
static Object strlit798;
static Object strlit803;
static Object strlit812;
static Object strlit815;
static Object strlit820;
static Object strlit823;
static Object strlit843;
static Object strlit846;
static Object strlit851;
static Object strlit853;
static Object strlit858;
static Object strlit860;
static Object strlit879;
static Object strlit890;
static Object strlit892;
static Object strlit906;
static Object strlit912;
static Object strlit916;
static Object strlit926;
static Object strlit928;
static Object strlit933;
static Object strlit935;
static Object strlit941;
static Object strlit944;
static Object strlit954;
static Object strlit967;
static Object strlit968;
static Object strlit972;
static Object strlit975;
static Object strlit998;
static Object strlit1005;
static Object strlit1015;
static Object strlit1026;
static Object strlit1032;
static Object strlit1035;
static Object strlit1070;
static Object strlit1115;
static Object strlit1132;
static Object strlit1151;
static Object strlit1177;
static Object strlit1186;
static Object strlit1213;
static Object strlit1222;
static Object strlit1249;
static Object strlit1267;
static Object strlit1283;
static Object strlit1285;
static Object strlit1287;
static Object strlit1297;
static Object strlit1300;
static Object strlit1309;
static Object strlit1314;
static Object strlit1319;
static Object strlit1349;
static Object strlit1354;
static Object strlit1357;
static Object strlit1366;
static Object strlit1369;
static Object strlit1374;
static Object strlit1377;
static Object strlit1386;
static Object strlit1389;
static Object strlit1394;
static Object strlit1397;
static Object strlit1409;
static Object strlit1434;
static Object strlit1437;
static Object strlit1446;
static Object strlit1449;
static Object strlit1465;
static Object strlit1489;
static Object strlit1495;
static Object strlit1500;
static Object strlit1512;
static Object strlit1515;
static Object strlit1524;
static Object strlit1529;
static Object strlit1575;
static Object strlit1596;
static Object strlit1629;
static Object strlit1635;
static Object strlit1645;
static Object strlit1662;
static Object strlit1667;
static Object strlit1703;
static Object strlit1706;
static Object strlit1721;
static Object strlit1731;
static Object strlit1743;
static Object strlit1746;
static Object strlit1764;
static Object strlit1783;
static Object strlit1816;
static Object strlit1835;
static Object strlit1868;
static Object strlit1913;
static Object strlit1934;
static Object strlit1945;
static Object strlit1952;
static Object strlit1957;
static Object strlit1962;
static Object strlit1965;
static Object strlit1968;
static Object strlit1976;
static Object strlit1979;
static Object strlit1985;
static Object strlit1988;
static Object strlit1991;
static Object strlit1994;
static Object strlit2001;
static Object strlit2004;
static Object strlit2019;
static Object strlit2022;
static Object strlit2027;
static Object strlit2033;
static Object strlit2036;
static Object strlit2045;
static Object strlit2048;
static Object strlit2053;
static Object strlit2058;
static Object strlit2059;
static Object strlit2060;
static Object strlit2061;
static Object strlit2070;
static Object strlit2073;
static Object strlit2078;
Object module_sys;
Object module_sys_init();
Object module_io;
Object module_io_init();
Object module_curl;
Object module_curl_init();
static Object strlit2099;
static Object strlit2103;
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
// Begin line 24
  setline(24);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe14 = gc_frame_new();
// Begin line 23
  setline(23);
  setmodule(modulename);
  setsource(originalSourceLines);
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
// Begin line 27
  setline(27);
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
// Begin line 30
  setline(30);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe30 = gc_frame_new();
// Begin line 29
  setline(29);
  setmodule(modulename);
  setsource(originalSourceLines);
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
// Begin line 33
  setline(33);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe38 = gc_frame_new();
// Begin line 32
  setline(32);
  setmodule(modulename);
  setsource(originalSourceLines);
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
  Object *var_folder = &(stackframe->slots[0]);
  *var_folder = args[curarg];
  curarg++;
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 37
  setline(37);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe46 = gc_frame_new();
// Begin line 36
  setline(36);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_folder
  gc_frame_newslot(*var_folder);
  params[0] = *var_folder;
  partcv[0] = 1;
  Object call47 = callmethodflags(self, "package", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe46);
// compilenode returning call47
  gc_frame_end(frame);
  return call47;
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
// Begin line 41
  setline(41);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 40
  setline(40);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool54 = alloc_Boolean(1);
// compilenode returning bool54
  *var_verbose = bool54;
  if (bool54 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply61(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 44
  setline(44);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 43
  setline(43);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool62 = alloc_Boolean(1);
// compilenode returning bool62
  *var_global = bool62;
  if (bool62 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply69(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  Object *var_build = getfromclosure(closure, 0);
  Object self = *(getfromclosure(closure, 1));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 47
  setline(47);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool70 = alloc_Boolean(1);
// compilenode returning bool70
  *var_build = bool70;
  if (bool70 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply77(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  Object *var_install = getfromclosure(closure, 0);
  Object self = *(getfromclosure(closure, 1));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 51
  setline(51);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool78 = alloc_Boolean(1);
// compilenode returning bool78
  *var_install = bool78;
  if (bool78 == undefined)
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
  Object *var_build = getfromclosure(closure, 2);
  Object *var_install = getfromclosure(closure, 3);
  Object self = *(getfromclosure(closure, 4));
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
// Begin line 22
  setline(22);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 21
  setline(21);
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
// Begin line 25
  setline(25);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe10 = gc_frame_new();
// Begin line 22
  setline(22);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit11 == NULL) {
    strlit11 = alloc_String("get");
    gc_root(strlit11);
  }
// compilenode returning strlit11
  gc_frame_newslot(strlit11);
// Begin line 25
  setline(25);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block12 = alloc_Block(NULL, NULL, "PackageManager", 25);
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
  meth_meth_PackageManager__apply13->definitionLine = 19;
// compilenode returning block12
  gc_frame_newslot(block12);
// Begin line 22
  setline(22);
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
// Begin line 28
  setline(28);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe18 = gc_frame_new();
// Begin line 25
  setline(25);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit19 == NULL) {
    strlit19 = alloc_String("list");
    gc_root(strlit19);
  }
// compilenode returning strlit19
  gc_frame_newslot(strlit19);
// Begin line 28
  setline(28);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block20 = alloc_Block(NULL, NULL, "PackageManager", 28);
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
  meth_meth_PackageManager__apply21->definitionLine = 19;
// compilenode returning block20
  gc_frame_newslot(block20);
// Begin line 25
  setline(25);
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
// Begin line 31
  setline(31);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe26 = gc_frame_new();
// Begin line 28
  setline(28);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit27 == NULL) {
    strlit27 = alloc_String("bundle");
    gc_root(strlit27);
  }
// compilenode returning strlit27
  gc_frame_newslot(strlit27);
// Begin line 31
  setline(31);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block28 = alloc_Block(NULL, NULL, "PackageManager", 31);
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
  meth_meth_PackageManager__apply29->definitionLine = 19;
// compilenode returning block28
  gc_frame_newslot(block28);
// Begin line 28
  setline(28);
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
// Begin line 35
  setline(35);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe34 = gc_frame_new();
// Begin line 31
  setline(31);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit35 == NULL) {
    strlit35 = alloc_String("set-address");
    gc_root(strlit35);
  }
// compilenode returning strlit35
  gc_frame_newslot(strlit35);
// Begin line 35
  setline(35);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block36 = alloc_Block(NULL, NULL, "PackageManager", 35);
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
  meth_meth_PackageManager__apply37->definitionLine = 19;
// compilenode returning block36
  gc_frame_newslot(block36);
// Begin line 31
  setline(31);
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
// Begin line 39
  setline(39);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe42 = gc_frame_new();
// Begin line 35
  setline(35);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit43 == NULL) {
    strlit43 = alloc_String("package");
    gc_root(strlit43);
  }
// compilenode returning strlit43
  gc_frame_newslot(strlit43);
// Begin line 39
  setline(39);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block44 = alloc_Block(NULL, NULL, "PackageManager", 39);
  gc_frame_newslot(block44);
  block_savedest(block44);
  Object closure45 = createclosure(1, "_apply");
setclosureframe(closure45, stackframe);
  addtoclosure(closure45, selfslot);
  struct UserObject *uo45 = (struct UserObject*)block44;
  uo45->data[0] = (Object)closure45;
  Method *meth_meth_PackageManager__apply45 = addmethod2pos(block44, "_apply", &meth_PackageManager__apply45, 0);
int argcv_meth_PackageManager__apply45[] = {1};
meth_meth_PackageManager__apply45->type = alloc_MethodType(1, argcv_meth_PackageManager__apply45);
  meth_meth_PackageManager__apply45->definitionModule = modulename;
  meth_meth_PackageManager__apply45->definitionLine = 19;
// compilenode returning block44
  gc_frame_newslot(block44);
// Begin line 35
  setline(35);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_on
  params[0] = strlit43;
  params[1] = block44;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call49 = callmethod(*var_on, "option()do",
    2, partcv, params);
  gc_frame_end(callframe42);
// compilenode returning call49
// Begin line 42
  setline(42);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe50 = gc_frame_new();
// Begin line 39
  setline(39);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit51 == NULL) {
    strlit51 = alloc_String("--verbose");
    gc_root(strlit51);
  }
// compilenode returning strlit51
  gc_frame_newslot(strlit51);
// Begin line 42
  setline(42);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block52 = alloc_Block(NULL, NULL, "PackageManager", 42);
  gc_frame_newslot(block52);
  block_savedest(block52);
  Object closure53 = createclosure(2, "_apply");
setclosureframe(closure53, stackframe);
  addtoclosure(closure53, var_verbose);
  addtoclosure(closure53, selfslot);
  struct UserObject *uo53 = (struct UserObject*)block52;
  uo53->data[0] = (Object)closure53;
  Method *meth_meth_PackageManager__apply53 = addmethod2pos(block52, "_apply", &meth_PackageManager__apply53, 0);
int argcv_meth_PackageManager__apply53[] = {0};
meth_meth_PackageManager__apply53->type = alloc_MethodType(1, argcv_meth_PackageManager__apply53);
  meth_meth_PackageManager__apply53->definitionModule = modulename;
  meth_meth_PackageManager__apply53->definitionLine = 19;
// compilenode returning block52
  gc_frame_newslot(block52);
// Begin line 39
  setline(39);
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
// Begin line 46
  setline(46);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe58 = gc_frame_new();
// Begin line 42
  setline(42);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit59 == NULL) {
    strlit59 = alloc_String("--global");
    gc_root(strlit59);
  }
// compilenode returning strlit59
  gc_frame_newslot(strlit59);
// Begin line 46
  setline(46);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block60 = alloc_Block(NULL, NULL, "PackageManager", 46);
  gc_frame_newslot(block60);
  block_savedest(block60);
  Object closure61 = createclosure(2, "_apply");
setclosureframe(closure61, stackframe);
  addtoclosure(closure61, var_global);
  addtoclosure(closure61, selfslot);
  struct UserObject *uo61 = (struct UserObject*)block60;
  uo61->data[0] = (Object)closure61;
  Method *meth_meth_PackageManager__apply61 = addmethod2pos(block60, "_apply", &meth_PackageManager__apply61, 0);
int argcv_meth_PackageManager__apply61[] = {0};
meth_meth_PackageManager__apply61->type = alloc_MethodType(1, argcv_meth_PackageManager__apply61);
  meth_meth_PackageManager__apply61->definitionModule = modulename;
  meth_meth_PackageManager__apply61->definitionLine = 19;
// compilenode returning block60
  gc_frame_newslot(block60);
// Begin line 42
  setline(42);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_on
  params[0] = strlit59;
  params[1] = block60;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call65 = callmethod(*var_on, "flag()do",
    2, partcv, params);
  gc_frame_end(callframe58);
// compilenode returning call65
// Begin line 50
  setline(50);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe66 = gc_frame_new();
// Begin line 46
  setline(46);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit67 == NULL) {
    strlit67 = alloc_String("--build");
    gc_root(strlit67);
  }
// compilenode returning strlit67
  gc_frame_newslot(strlit67);
// Begin line 50
  setline(50);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block68 = alloc_Block(NULL, NULL, "PackageManager", 50);
  gc_frame_newslot(block68);
  block_savedest(block68);
  Object closure69 = createclosure(2, "_apply");
setclosureframe(closure69, stackframe);
  addtoclosure(closure69, var_build);
  addtoclosure(closure69, selfslot);
  struct UserObject *uo69 = (struct UserObject*)block68;
  uo69->data[0] = (Object)closure69;
  Method *meth_meth_PackageManager__apply69 = addmethod2pos(block68, "_apply", &meth_PackageManager__apply69, 0);
int argcv_meth_PackageManager__apply69[] = {0};
meth_meth_PackageManager__apply69->type = alloc_MethodType(1, argcv_meth_PackageManager__apply69);
  meth_meth_PackageManager__apply69->definitionModule = modulename;
  meth_meth_PackageManager__apply69->definitionLine = 19;
// compilenode returning block68
  gc_frame_newslot(block68);
// Begin line 46
  setline(46);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_on
  params[0] = strlit67;
  params[1] = block68;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call73 = callmethod(*var_on, "flag()do",
    2, partcv, params);
  gc_frame_end(callframe66);
// compilenode returning call73
// Begin line 53
  setline(53);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe74 = gc_frame_new();
// Begin line 50
  setline(50);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit75 == NULL) {
    strlit75 = alloc_String("--install");
    gc_root(strlit75);
  }
// compilenode returning strlit75
  gc_frame_newslot(strlit75);
// Begin line 53
  setline(53);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block76 = alloc_Block(NULL, NULL, "PackageManager", 53);
  gc_frame_newslot(block76);
  block_savedest(block76);
  Object closure77 = createclosure(2, "_apply");
setclosureframe(closure77, stackframe);
  addtoclosure(closure77, var_install);
  addtoclosure(closure77, selfslot);
  struct UserObject *uo77 = (struct UserObject*)block76;
  uo77->data[0] = (Object)closure77;
  Method *meth_meth_PackageManager__apply77 = addmethod2pos(block76, "_apply", &meth_PackageManager__apply77, 0);
int argcv_meth_PackageManager__apply77[] = {0};
meth_meth_PackageManager__apply77->type = alloc_MethodType(1, argcv_meth_PackageManager__apply77);
  meth_meth_PackageManager__apply77->definitionModule = modulename;
  meth_meth_PackageManager__apply77->definitionLine = 19;
// compilenode returning block76
  gc_frame_newslot(block76);
// Begin line 50
  setline(50);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_on
  params[0] = strlit75;
  params[1] = block76;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call81 = callmethod(*var_on, "flag()do",
    2, partcv, params);
  gc_frame_end(callframe74);
// compilenode returning call81
  gc_frame_end(frame);
  return call81;
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
  Object *var_build = getfromclosure(closure, 2);
  Object *var_install = getfromclosure(closure, 3);
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
// Begin line 19
  setline(19);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 18
  setline(18);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1 = alloc_Float64(0);
// compilenode returning num1
  *var_count = num1;
  if (num1 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 20
  setline(20);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 19
  setline(19);
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
// Begin line 54
  setline(54);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe5 = gc_frame_new();
// Begin line 20
  setline(20);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_args
  gc_frame_newslot(*var_args);
// Begin line 54
  setline(54);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block6 = alloc_Block(NULL, NULL, "PackageManager", 54);
  gc_frame_newslot(block6);
  block_savedest(block6);
  Object closure7 = createclosure(5, "_apply");
setclosureframe(closure7, stackframe);
  addtoclosure(closure7, var_verbose);
  addtoclosure(closure7, var_global);
  addtoclosure(closure7, var_build);
  addtoclosure(closure7, var_install);
  addtoclosure(closure7, selfslot);
  struct UserObject *uo7 = (struct UserObject*)block6;
  uo7->data[0] = (Object)closure7;
  Method *meth_meth_PackageManager__apply7 = addmethod2pos(block6, "_apply", &meth_PackageManager__apply7, 0);
int argcv_meth_PackageManager__apply7[] = {2};
meth_meth_PackageManager__apply7->type = alloc_MethodType(1, argcv_meth_PackageManager__apply7);
  meth_meth_PackageManager__apply7->definitionModule = modulename;
  meth_meth_PackageManager__apply7->definitionLine = 19;
// compilenode returning block6
  gc_frame_newslot(block6);
  params[0] = *var_args;
  params[1] = block6;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call83 = callmethodflags(self, "forArgs()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe5);
// compilenode returning call83
  gc_frame_end(frame);
  return call83;
}
Object meth_PackageManager_option_40__41_do91(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 1, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(4, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "option()do");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  if (nparts > 0 && argcv[1] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_i = getfromclosure(closure, 0);
  Object *var_args = getfromclosure(closure, 1);
  Object *var_ran = getfromclosure(closure, 2);
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
  Object *var_arg = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "arg");
// Begin line 64
  setline(64);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe92 = gc_frame_new();
// Begin line 63
  setline(63);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call93 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe92);
// compilenode returning call93
  *var_arg = call93;
  if (call93 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 73
  setline(73);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 64
  setline(64);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arg
  int op_slot_left_95 = gc_frame_newslot(*var_arg);
// compilenode returning *var_name
  int op_slot_right_95 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult97 = callmethod(*var_arg, "==", 1, partcv, params);
// compilenode returning opresult97
  Object if94 = done;
  if (istrue(opresult97)) {
// Begin line 69
  setline(69);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 65
  setline(65);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe100 = gc_frame_new();
// compilenode returning *var_args
  partcv[0] = 0;
  Object call101 = callmethod(*var_args, "size",
    1, partcv, params);
  gc_frame_end(callframe100);
// compilenode returning call101
// compilenode returning call101
  int op_slot_left_99 = gc_frame_newslot(call101);
// compilenode returning *var_i
  int op_slot_right_99 = gc_frame_newslot(*var_i);
  params[0] = *var_i;
  partcv[0] = 1;
  Object opresult103 = callmethod(call101, "==", 1, partcv, params);
// compilenode returning opresult103
  Object if98 = done;
  if (istrue(opresult103)) {
// Begin line 67
  setline(67);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe104 = gc_frame_new();
// Begin line 66
  setline(66);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit107 == NULL) {
    strlit107 = alloc_String("Missing value for option ");
    gc_root(strlit107);
  }
// compilenode returning strlit107
  int op_slot_left_106 = gc_frame_newslot(strlit107);
// compilenode returning *var_name
  int op_slot_right_106 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult109 = callmethod(strlit107, "++", 1, partcv, params);
// compilenode returning opresult109
  int op_slot_left_105 = gc_frame_newslot(opresult109);
  if (strlit110 == NULL) {
    strlit110 = alloc_String("");
    gc_root(strlit110);
  }
// compilenode returning strlit110
  int op_slot_right_105 = gc_frame_newslot(strlit110);
  params[0] = strlit110;
  partcv[0] = 1;
  Object opresult112 = callmethod(opresult109, "++", 1, partcv, params);
// compilenode returning opresult112
  gc_frame_newslot(opresult112);
  int callframe113 = gc_frame_new();
  partcv[0] = 0;
  Object call114 = callmethodflags(prelude, "Exception", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe113);
// compilenode returning call114
  params[0] = opresult112;
  partcv[0] = 1;
  Object call115 = callmethod(call114, "raise",
    1, partcv, params);
  gc_frame_end(callframe104);
// compilenode returning call115
    gc_frame_newslot(call115);
    if98 = call115;
  } else {
  }
// compilenode returning if98
// Begin line 70
  setline(70);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 69
  setline(69);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_116 = gc_frame_newslot(*var_i);
  Object num117 = alloc_Float64(1);
// compilenode returning num117
  int op_slot_right_116 = gc_frame_newslot(num117);
  params[0] = num117;
  partcv[0] = 1;
  Object sum119 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum119
  *var_i = sum119;
  if (sum119 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 71
  setline(71);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe121 = gc_frame_new();
// Begin line 70
  setline(70);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe122 = gc_frame_new();
// compilenode returning *var_i
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call123 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe122);
// compilenode returning call123
  gc_frame_newslot(call123);
// compilenode returning *var_block_39_
  params[0] = call123;
  partcv[0] = 1;
  Object call124 = callmethod(*var_block_39_, "apply",
    1, partcv, params);
  gc_frame_end(callframe121);
// compilenode returning call124
// Begin line 72
  setline(72);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 71
  setline(71);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool125 = alloc_Boolean(1);
// compilenode returning bool125
  *var_ran = bool125;
  if (bool125 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if94 = done;
  } else {
  }
// compilenode returning if94
  gc_frame_end(frame);
  return if94;
}
Object meth_PackageManager_doubleValue_40__41_do127(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 77
  setline(77);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe128 = gc_frame_new();
// Begin line 76
  setline(76);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call129 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe128);
// compilenode returning call129
  *var_arg = call129;
  if (call129 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 88
  setline(88);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 77
  setline(77);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arg
  int op_slot_left_131 = gc_frame_newslot(*var_arg);
// compilenode returning *var_name
  int op_slot_right_131 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult133 = callmethod(*var_arg, "==", 1, partcv, params);
// compilenode returning opresult133
  Object if130 = done;
  if (istrue(opresult133)) {
// Begin line 81
  setline(81);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 78
  setline(78);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe136 = gc_frame_new();
// compilenode returning *var_args
  partcv[0] = 0;
  Object call137 = callmethod(*var_args, "size",
    1, partcv, params);
  gc_frame_end(callframe136);
// compilenode returning call137
// compilenode returning call137
  int op_slot_left_135 = gc_frame_newslot(call137);
// compilenode returning *var_i
  int op_slot_left_138 = gc_frame_newslot(*var_i);
  Object num139 = alloc_Float64(1);
// compilenode returning num139
  int op_slot_right_138 = gc_frame_newslot(num139);
  params[0] = num139;
  partcv[0] = 1;
  Object sum141 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum141
  int op_slot_right_135 = gc_frame_newslot(sum141);
  params[0] = sum141;
  partcv[0] = 1;
  Object opresult143 = callmethod(call137, "<", 1, partcv, params);
// compilenode returning opresult143
  Object if134 = done;
  if (istrue(opresult143)) {
// Begin line 80
  setline(80);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe144 = gc_frame_new();
// Begin line 79
  setline(79);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit147 == NULL) {
    strlit147 = alloc_String("Missing values for option ");
    gc_root(strlit147);
  }
// compilenode returning strlit147
  int op_slot_left_146 = gc_frame_newslot(strlit147);
// compilenode returning *var_name
  int op_slot_right_146 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult149 = callmethod(strlit147, "++", 1, partcv, params);
// compilenode returning opresult149
  int op_slot_left_145 = gc_frame_newslot(opresult149);
  if (strlit150 == NULL) {
    strlit150 = alloc_String("");
    gc_root(strlit150);
  }
// compilenode returning strlit150
  int op_slot_right_145 = gc_frame_newslot(strlit150);
  params[0] = strlit150;
  partcv[0] = 1;
  Object opresult152 = callmethod(opresult149, "++", 1, partcv, params);
// compilenode returning opresult152
  gc_frame_newslot(opresult152);
  int callframe153 = gc_frame_new();
  partcv[0] = 0;
  Object call154 = callmethodflags(prelude, "Exception", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe153);
// compilenode returning call154
  params[0] = opresult152;
  partcv[0] = 1;
  Object call155 = callmethod(call154, "raise",
    1, partcv, params);
  gc_frame_end(callframe144);
// compilenode returning call155
    gc_frame_newslot(call155);
    if134 = call155;
  } else {
  }
// compilenode returning if134
// Begin line 82
  setline(82);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe156 = gc_frame_new();
// Begin line 81
  setline(81);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_157 = gc_frame_newslot(*var_i);
  Object num158 = alloc_Float64(1);
// compilenode returning num158
  int op_slot_right_157 = gc_frame_newslot(num158);
  params[0] = num158;
  partcv[0] = 1;
  Object sum160 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum160
  gc_frame_newslot(sum160);
// compilenode returning *var_args
  params[0] = sum160;
  partcv[0] = 1;
  Object call161 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe156);
// compilenode returning call161
  *var_arga = call161;
  if (call161 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 83
  setline(83);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe162 = gc_frame_new();
// Begin line 82
  setline(82);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_163 = gc_frame_newslot(*var_i);
  Object num164 = alloc_Float64(2);
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
  *var_argb = call167;
  if (call167 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 84
  setline(84);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe168 = gc_frame_new();
// Begin line 83
  setline(83);
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
  Object call169 = callmethod(*var_block_39_, "apply",
    1, partcv, params);
  gc_frame_end(callframe168);
// compilenode returning call169
// Begin line 85
  setline(85);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 84
  setline(84);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_170 = gc_frame_newslot(*var_i);
  Object num171 = alloc_Float64(2);
// compilenode returning num171
  int op_slot_right_170 = gc_frame_newslot(num171);
  params[0] = num171;
  partcv[0] = 1;
  Object sum173 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum173
  *var_i = sum173;
  if (sum173 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 86
  setline(86);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 85
  setline(85);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool175 = alloc_Boolean(1);
// compilenode returning bool175
  *var_ran = bool175;
  if (bool175 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if130 = done;
  } else {
  }
// compilenode returning if130
  gc_frame_end(frame);
  return if130;
}
Object meth_PackageManager_flag_40__41_do177(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 3, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(4, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "flag()do");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  if (nparts > 0 && argcv[1] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_i = getfromclosure(closure, 0);
  Object *var_args = getfromclosure(closure, 1);
  Object *var_ran = getfromclosure(closure, 2);
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
  Object *var_arg = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "arg");
// Begin line 92
  setline(92);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe178 = gc_frame_new();
// Begin line 91
  setline(91);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call179 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe178);
// compilenode returning call179
  *var_arg = call179;
  if (call179 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 96
  setline(96);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 92
  setline(92);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arg
  int op_slot_left_181 = gc_frame_newslot(*var_arg);
// compilenode returning *var_name
  int op_slot_right_181 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult183 = callmethod(*var_arg, "==", 1, partcv, params);
// compilenode returning opresult183
  Object if180 = done;
  if (istrue(opresult183)) {
// Begin line 93
  setline(93);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe184 = gc_frame_new();
// compilenode returning *var_block_39_
  partcv[0] = 0;
  Object call185 = callmethod(*var_block_39_, "apply",
    1, partcv, params);
  gc_frame_end(callframe184);
// compilenode returning call185
// compilenode returning call185
// Begin line 95
  setline(95);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 94
  setline(94);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool186 = alloc_Boolean(1);
// compilenode returning bool186
  *var_ran = bool186;
  if (bool186 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if180 = done;
  } else {
  }
// compilenode returning if180
  gc_frame_end(frame);
  return if180;
}
Object meth_PackageManager__apply190(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_191 = gc_frame_newslot(*var_i);
// compilenode returning *var_size
  int op_slot_right_191 = gc_frame_newslot(*var_size);
  params[0] = *var_size;
  partcv[0] = 1;
  Object opresult193 = callmethod(*var_i, "<=", 1, partcv, params);
// compilenode returning opresult193
  gc_frame_end(frame);
  return opresult193;
}
Object meth_PackageManager__apply196(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 100
  setline(100);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe197 = gc_frame_new();
// Begin line 99
  setline(99);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call198 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe197);
// compilenode returning call198
  *var_arg = call198;
  if (call198 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 101
  setline(101);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 100
  setline(100);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arg
  params[0] = *var_arg;
  Object call199 = gracelib_print(NULL, 1,  params);
// compilenode returning call199
// Begin line 102
  setline(102);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 101
  setline(101);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool200 = alloc_Boolean(0);
// compilenode returning bool200
  *var_ran = bool200;
  if (bool200 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 103
  setline(103);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe202 = gc_frame_new();
// Begin line 102
  setline(102);
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
  Object call203 = callmethod(*var_block, "apply",
    1, partcv, params);
  gc_frame_end(callframe202);
// compilenode returning call203
// Begin line 107
  setline(107);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 103
  setline(103);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe207 = gc_frame_new();
  Object num208 = alloc_Float64(1);
// compilenode returning num208
  gc_frame_newslot(num208);
// compilenode returning *var_arg
  params[0] = num208;
  partcv[0] = 1;
  Object call209 = callmethod(*var_arg, "at",
    1, partcv, params);
  gc_frame_end(callframe207);
// compilenode returning call209
  int op_slot_left_206 = gc_frame_newslot(call209);
  if (strlit210 == NULL) {
    strlit210 = alloc_String("-");
    gc_root(strlit210);
  }
// compilenode returning strlit210
  int op_slot_right_206 = gc_frame_newslot(strlit210);
  params[0] = strlit210;
  partcv[0] = 1;
  Object opresult212 = callmethod(call209, "==", 1, partcv, params);
// compilenode returning opresult212
  int op_slot_left_205 = gc_frame_newslot(opresult212);
  int callframe213 = gc_frame_new();
// compilenode returning *var_ran
  partcv[0] = 0;
  Object call214 = callmethod(*var_ran, "not",
    1, partcv, params);
  gc_frame_end(callframe213);
// compilenode returning call214
// compilenode returning call214
  int op_slot_right_205 = gc_frame_newslot(call214);
  params[0] = call214;
  partcv[0] = 1;
  Object opresult216 = callmethod(opresult212, "&&", 1, partcv, params);
// compilenode returning opresult216
  Object if204 = done;
  if (istrue(opresult216)) {
// Begin line 105
  setline(105);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe217 = gc_frame_new();
// Begin line 104
  setline(104);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit220 == NULL) {
    strlit220 = alloc_String("Unrecognised argument ");
    gc_root(strlit220);
  }
// compilenode returning strlit220
  int op_slot_left_219 = gc_frame_newslot(strlit220);
// compilenode returning *var_arg
  int op_slot_right_219 = gc_frame_newslot(*var_arg);
  params[0] = *var_arg;
  partcv[0] = 1;
  Object opresult222 = callmethod(strlit220, "++", 1, partcv, params);
// compilenode returning opresult222
  int op_slot_left_218 = gc_frame_newslot(opresult222);
  if (strlit223 == NULL) {
    strlit223 = alloc_String("");
    gc_root(strlit223);
  }
// compilenode returning strlit223
  int op_slot_right_218 = gc_frame_newslot(strlit223);
  params[0] = strlit223;
  partcv[0] = 1;
  Object opresult225 = callmethod(opresult222, "++", 1, partcv, params);
// compilenode returning opresult225
  gc_frame_newslot(opresult225);
  int callframe226 = gc_frame_new();
  partcv[0] = 0;
  Object call227 = callmethodflags(prelude, "Exception", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe226);
// compilenode returning call227
  params[0] = opresult225;
  partcv[0] = 1;
  Object call228 = callmethod(call227, "raise",
    1, partcv, params);
  gc_frame_end(callframe217);
// compilenode returning call228
    gc_frame_newslot(call228);
    if204 = call228;
  } else {
  }
// compilenode returning if204
// Begin line 108
  setline(108);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 107
  setline(107);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_229 = gc_frame_newslot(*var_i);
  Object num230 = alloc_Float64(1);
// compilenode returning num230
  int op_slot_right_229 = gc_frame_newslot(num230);
  params[0] = num230;
  partcv[0] = 1;
  Object sum232 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum232
  *var_i = sum232;
  if (sum232 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_forArgs_40__41_do84(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 58
  setline(58);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 57
  setline(57);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num85 = alloc_Float64(2);
// compilenode returning num85
  *var_i = num85;
  if (num85 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 59
  setline(59);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 58
  setline(58);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool86 = alloc_Boolean(0);
// compilenode returning bool86
  *var_ran = bool86;
  if (bool86 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 61
  setline(61);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 59
  setline(59);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe87 = gc_frame_new();
// compilenode returning *var_args
  partcv[0] = 0;
  Object call88 = callmethod(*var_args, "size",
    1, partcv, params);
  gc_frame_end(callframe87);
// compilenode returning call88
// compilenode returning call88
  *var_size = call88;
  if (call88 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 61
  setline(61);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object inheritingObject89 = inheritingObject;
  if (isTailObject) {
    isTailObject = 0;
    inheritingObject = NULL;
  }
  Object obj89 = alloc_userobj2(3,4, objclass89);
  gc_frame_newslot(obj89);
// OBJECT OUTER DEC outer
  adddatum2(obj89, self, 0);
  addmethodreal(obj89,"outer", &reader_PackageManager_outer_90);
  Object oldself89 = self;
  struct StackFrameObject *oldstackframe89 = stackframe;
  stackframe = alloc_StackFrame(1, oldstackframe89);
  gc_frame_newslot((Object)stackframe);
  self = obj89;
  Object *oldselfslot89 = selfslot;
  selfslot = &stackframe->slots[0];
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  Object thisouter89 = (*(struct UserObject *)self).data[0], lowerouter89 = thisouter89;
  if (inheritingObject89) {
    struct UserObject *inho89 = (struct UserObject *)inheritingObject89;
    while (inho89->super != GraceDefaultObject) inho89 = (struct UserObject *)inho89->super;
    inho89->super = obj89;
    self = inheritingObject89;
    *selfslot = self;
    lowerouter89 = (*(struct UserObject *)self).data[0];
    (*(struct UserObject *)self).data[0] = thisouter89;
  }
  block_savedest(obj89);
  Object closure91 = createclosure(3, "option()do");
setclosureframe(closure91, stackframe);
  addtoclosure(closure91, var_i);
  addtoclosure(closure91, var_args);
  addtoclosure(closure91, var_ran);
  struct UserObject *uo91 = (struct UserObject*)obj89;
  uo91->data[1] = (Object)closure91;
  Method *meth_meth_PackageManager_option_40__41_do91 = addmethod2pos(obj89, "option()do", &meth_PackageManager_option_40__41_do91, 1);
int argcv_meth_PackageManager_option_40__41_do91[] = {1, 1};
meth_meth_PackageManager_option_40__41_do91->type = alloc_MethodType(2, argcv_meth_PackageManager_option_40__41_do91);
meth_meth_PackageManager_option_40__41_do91->type->types[0] = type_String;
meth_meth_PackageManager_option_40__41_do91->type->names[0] = "name";
  meth_meth_PackageManager_option_40__41_do91->definitionModule = modulename;
  meth_meth_PackageManager_option_40__41_do91->definitionLine = 62;
  block_savedest(obj89);
  Object closure127 = createclosure(3, "doubleValue()do");
setclosureframe(closure127, stackframe);
  addtoclosure(closure127, var_i);
  addtoclosure(closure127, var_args);
  addtoclosure(closure127, var_ran);
  struct UserObject *uo127 = (struct UserObject*)obj89;
  uo127->data[2] = (Object)closure127;
  Method *meth_meth_PackageManager_doubleValue_40__41_do127 = addmethod2pos(obj89, "doubleValue()do", &meth_PackageManager_doubleValue_40__41_do127, 2);
int argcv_meth_PackageManager_doubleValue_40__41_do127[] = {1, 1};
meth_meth_PackageManager_doubleValue_40__41_do127->type = alloc_MethodType(2, argcv_meth_PackageManager_doubleValue_40__41_do127);
meth_meth_PackageManager_doubleValue_40__41_do127->type->types[0] = type_String;
meth_meth_PackageManager_doubleValue_40__41_do127->type->names[0] = "name";
  meth_meth_PackageManager_doubleValue_40__41_do127->definitionModule = modulename;
  meth_meth_PackageManager_doubleValue_40__41_do127->definitionLine = 75;
  block_savedest(obj89);
  Object closure177 = createclosure(3, "flag()do");
setclosureframe(closure177, stackframe);
  addtoclosure(closure177, var_i);
  addtoclosure(closure177, var_args);
  addtoclosure(closure177, var_ran);
  struct UserObject *uo177 = (struct UserObject*)obj89;
  uo177->data[3] = (Object)closure177;
  Method *meth_meth_PackageManager_flag_40__41_do177 = addmethod2pos(obj89, "flag()do", &meth_PackageManager_flag_40__41_do177, 3);
int argcv_meth_PackageManager_flag_40__41_do177[] = {1, 1};
meth_meth_PackageManager_flag_40__41_do177->type = alloc_MethodType(2, argcv_meth_PackageManager_flag_40__41_do177);
meth_meth_PackageManager_flag_40__41_do177->type->types[0] = type_String;
meth_meth_PackageManager_flag_40__41_do177->type->names[0] = "name";
  meth_meth_PackageManager_flag_40__41_do177->definitionModule = modulename;
  meth_meth_PackageManager_flag_40__41_do177->definitionLine = 90;
  sourceObject = obj89;
  sourceObject = obj89;
  sourceObject = obj89;
objclass89 = obj89->class;
  objclass89->definitionModule = modulename;
  objclass89->definitionLine = 61;
  (*(struct UserObject *)self).data[0] = lowerouter89;
  self = oldself89;
  selfslot = oldselfslot89;
  stackframe = oldstackframe89;
// compilenode returning obj89
  *var_on = obj89;
  if (obj89 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 109
  setline(109);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe188 = gc_frame_new();
// Begin line 98
  setline(98);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block189 = alloc_Block(NULL, NULL, "PackageManager", 98);
  gc_frame_newslot(block189);
  block_savedest(block189);
  Object closure190 = createclosure(3, "_apply");
setclosureframe(closure190, stackframe);
  addtoclosure(closure190, var_i);
  addtoclosure(closure190, var_size);
  addtoclosure(closure190, selfslot);
  struct UserObject *uo190 = (struct UserObject*)block189;
  uo190->data[0] = (Object)closure190;
  Method *meth_meth_PackageManager__apply190 = addmethod2pos(block189, "_apply", &meth_PackageManager__apply190, 0);
int argcv_meth_PackageManager__apply190[] = {0};
meth_meth_PackageManager__apply190->type = alloc_MethodType(1, argcv_meth_PackageManager__apply190);
  meth_meth_PackageManager__apply190->definitionModule = modulename;
  meth_meth_PackageManager__apply190->definitionLine = 93;
// compilenode returning block189
  gc_frame_newslot(block189);
// Begin line 109
  setline(109);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block195 = alloc_Block(NULL, NULL, "PackageManager", 109);
  gc_frame_newslot(block195);
  block_savedest(block195);
  Object closure196 = createclosure(6, "_apply");
setclosureframe(closure196, stackframe);
  addtoclosure(closure196, var_i);
  addtoclosure(closure196, var_args);
  addtoclosure(closure196, var_ran);
  addtoclosure(closure196, var_on);
  addtoclosure(closure196, var_block);
  addtoclosure(closure196, selfslot);
  struct UserObject *uo196 = (struct UserObject*)block195;
  uo196->data[0] = (Object)closure196;
  Method *meth_meth_PackageManager__apply196 = addmethod2pos(block195, "_apply", &meth_PackageManager__apply196, 0);
int argcv_meth_PackageManager__apply196[] = {0};
meth_meth_PackageManager__apply196->type = alloc_MethodType(1, argcv_meth_PackageManager__apply196);
  meth_meth_PackageManager__apply196->definitionModule = modulename;
  meth_meth_PackageManager__apply196->definitionLine = 93;
// compilenode returning block195
  gc_frame_newslot(block195);
  params[0] = block189;
  params[1] = block195;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call235 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe188);
// compilenode returning call235
  gc_frame_end(frame);
  return call235;
}
Object meth_PackageManager_forArgs_40__41_do84_object(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 58
  setline(58);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 57
  setline(57);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num85 = alloc_Float64(2);
// compilenode returning num85
  *var_i = num85;
  if (num85 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 59
  setline(59);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 58
  setline(58);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool86 = alloc_Boolean(0);
// compilenode returning bool86
  *var_ran = bool86;
  if (bool86 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 61
  setline(61);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 59
  setline(59);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe87 = gc_frame_new();
// compilenode returning *var_args
  partcv[0] = 0;
  Object call88 = callmethod(*var_args, "size",
    1, partcv, params);
  gc_frame_end(callframe87);
// compilenode returning call88
// compilenode returning call88
  *var_size = call88;
  if (call88 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 61
  setline(61);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object inheritingObject89 = inheritingObject;
  if (isTailObject) {
    isTailObject = 0;
    inheritingObject = NULL;
  }
  Object obj89 = alloc_userobj2(3,4, objclass89);
  gc_frame_newslot(obj89);
// OBJECT OUTER DEC outer
  adddatum2(obj89, self, 0);
  addmethodreal(obj89,"outer", &reader_PackageManager_outer_90);
  Object oldself89 = self;
  struct StackFrameObject *oldstackframe89 = stackframe;
  stackframe = alloc_StackFrame(1, oldstackframe89);
  gc_frame_newslot((Object)stackframe);
  self = obj89;
  Object *oldselfslot89 = selfslot;
  selfslot = &stackframe->slots[0];
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  Object thisouter89 = (*(struct UserObject *)self).data[0], lowerouter89 = thisouter89;
  if (inheritingObject89) {
    struct UserObject *inho89 = (struct UserObject *)inheritingObject89;
    while (inho89->super != GraceDefaultObject) inho89 = (struct UserObject *)inho89->super;
    inho89->super = obj89;
    self = inheritingObject89;
    *selfslot = self;
    lowerouter89 = (*(struct UserObject *)self).data[0];
    (*(struct UserObject *)self).data[0] = thisouter89;
  }
  block_savedest(obj89);
  Object closure91 = createclosure(3, "option()do");
setclosureframe(closure91, stackframe);
  addtoclosure(closure91, var_i);
  addtoclosure(closure91, var_args);
  addtoclosure(closure91, var_ran);
  struct UserObject *uo91 = (struct UserObject*)obj89;
  uo91->data[1] = (Object)closure91;
  Method *meth_meth_PackageManager_option_40__41_do91 = addmethod2pos(obj89, "option()do", &meth_PackageManager_option_40__41_do91, 1);
int argcv_meth_PackageManager_option_40__41_do91[] = {1, 1};
meth_meth_PackageManager_option_40__41_do91->type = alloc_MethodType(2, argcv_meth_PackageManager_option_40__41_do91);
meth_meth_PackageManager_option_40__41_do91->type->types[0] = type_String;
meth_meth_PackageManager_option_40__41_do91->type->names[0] = "name";
  meth_meth_PackageManager_option_40__41_do91->definitionModule = modulename;
  meth_meth_PackageManager_option_40__41_do91->definitionLine = 62;
  block_savedest(obj89);
  Object closure127 = createclosure(3, "doubleValue()do");
setclosureframe(closure127, stackframe);
  addtoclosure(closure127, var_i);
  addtoclosure(closure127, var_args);
  addtoclosure(closure127, var_ran);
  struct UserObject *uo127 = (struct UserObject*)obj89;
  uo127->data[2] = (Object)closure127;
  Method *meth_meth_PackageManager_doubleValue_40__41_do127 = addmethod2pos(obj89, "doubleValue()do", &meth_PackageManager_doubleValue_40__41_do127, 2);
int argcv_meth_PackageManager_doubleValue_40__41_do127[] = {1, 1};
meth_meth_PackageManager_doubleValue_40__41_do127->type = alloc_MethodType(2, argcv_meth_PackageManager_doubleValue_40__41_do127);
meth_meth_PackageManager_doubleValue_40__41_do127->type->types[0] = type_String;
meth_meth_PackageManager_doubleValue_40__41_do127->type->names[0] = "name";
  meth_meth_PackageManager_doubleValue_40__41_do127->definitionModule = modulename;
  meth_meth_PackageManager_doubleValue_40__41_do127->definitionLine = 75;
  block_savedest(obj89);
  Object closure177 = createclosure(3, "flag()do");
setclosureframe(closure177, stackframe);
  addtoclosure(closure177, var_i);
  addtoclosure(closure177, var_args);
  addtoclosure(closure177, var_ran);
  struct UserObject *uo177 = (struct UserObject*)obj89;
  uo177->data[3] = (Object)closure177;
  Method *meth_meth_PackageManager_flag_40__41_do177 = addmethod2pos(obj89, "flag()do", &meth_PackageManager_flag_40__41_do177, 3);
int argcv_meth_PackageManager_flag_40__41_do177[] = {1, 1};
meth_meth_PackageManager_flag_40__41_do177->type = alloc_MethodType(2, argcv_meth_PackageManager_flag_40__41_do177);
meth_meth_PackageManager_flag_40__41_do177->type->types[0] = type_String;
meth_meth_PackageManager_flag_40__41_do177->type->names[0] = "name";
  meth_meth_PackageManager_flag_40__41_do177->definitionModule = modulename;
  meth_meth_PackageManager_flag_40__41_do177->definitionLine = 90;
  sourceObject = obj89;
  sourceObject = obj89;
  sourceObject = obj89;
objclass89 = obj89->class;
  objclass89->definitionModule = modulename;
  objclass89->definitionLine = 61;
  (*(struct UserObject *)self).data[0] = lowerouter89;
  self = oldself89;
  selfslot = oldselfslot89;
  stackframe = oldstackframe89;
// compilenode returning obj89
  *var_on = obj89;
  if (obj89 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 109
  setline(109);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe188 = gc_frame_new();
// Begin line 98
  setline(98);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block189 = alloc_Block(NULL, NULL, "PackageManager", 98);
  gc_frame_newslot(block189);
  block_savedest(block189);
  Object closure190 = createclosure(3, "_apply");
setclosureframe(closure190, stackframe);
  addtoclosure(closure190, var_i);
  addtoclosure(closure190, var_size);
  addtoclosure(closure190, selfslot);
  struct UserObject *uo190 = (struct UserObject*)block189;
  uo190->data[0] = (Object)closure190;
  Method *meth_meth_PackageManager__apply190 = addmethod2pos(block189, "_apply", &meth_PackageManager__apply190, 0);
int argcv_meth_PackageManager__apply190[] = {0};
meth_meth_PackageManager__apply190->type = alloc_MethodType(1, argcv_meth_PackageManager__apply190);
  meth_meth_PackageManager__apply190->definitionModule = modulename;
  meth_meth_PackageManager__apply190->definitionLine = 93;
// compilenode returning block189
  gc_frame_newslot(block189);
// Begin line 109
  setline(109);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block195 = alloc_Block(NULL, NULL, "PackageManager", 109);
  gc_frame_newslot(block195);
  block_savedest(block195);
  Object closure196 = createclosure(6, "_apply");
setclosureframe(closure196, stackframe);
  addtoclosure(closure196, var_i);
  addtoclosure(closure196, var_args);
  addtoclosure(closure196, var_ran);
  addtoclosure(closure196, var_on);
  addtoclosure(closure196, var_block);
  addtoclosure(closure196, selfslot);
  struct UserObject *uo196 = (struct UserObject*)block195;
  uo196->data[0] = (Object)closure196;
  Method *meth_meth_PackageManager__apply196 = addmethod2pos(block195, "_apply", &meth_PackageManager__apply196, 0);
int argcv_meth_PackageManager__apply196[] = {0};
meth_meth_PackageManager__apply196->type = alloc_MethodType(1, argcv_meth_PackageManager__apply196);
  meth_meth_PackageManager__apply196->definitionModule = modulename;
  meth_meth_PackageManager__apply196->definitionLine = 93;
// compilenode returning block195
  gc_frame_newslot(block195);
  params[0] = block189;
  params[1] = block195;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call235 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe188);
// compilenode returning call235
  gc_frame_end(frame);
  return call235;
}
Object meth_PackageManager_listInstalled237(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 3, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "listInstalled");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_sys = getfromclosure(closure, 0);
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
// Begin line 113
  setline(113);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 112
  setline(112);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit238 == NULL) {
    strlit238 = alloc_String("Installed packages:");
    gc_root(strlit238);
  }
// compilenode returning strlit238
  params[0] = strlit238;
  Object call239 = gracelib_print(NULL, 1,  params);
// compilenode returning call239
// Begin line 114
  setline(114);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe240 = gc_frame_new();
// Begin line 113
  setline(113);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit241 == NULL) {
    strlit241 = alloc_String("usr/lib/grace/packages/");
    gc_root(strlit241);
  }
// compilenode returning strlit241
  gc_frame_newslot(strlit241);
  params[0] = strlit241;
  partcv[0] = 1;
  Object call242 = callmethodflags(self, "checkListPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe240);
// compilenode returning call242
// Begin line 115
  setline(115);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe243 = gc_frame_new();
// Begin line 114
  setline(114);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit244 == NULL) {
    strlit244 = alloc_String("usr/lib/grace/packages/");
    gc_root(strlit244);
  }
// compilenode returning strlit244
  gc_frame_newslot(strlit244);
  params[0] = strlit244;
  partcv[0] = 1;
  Object call245 = callmethodflags(self, "checkListPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe243);
// compilenode returning call245
// Begin line 116
  setline(116);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe246 = gc_frame_new();
// Begin line 115
  setline(115);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe248 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call249 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe248);
// compilenode returning call249
// compilenode returning call249
  if (strlit250 == NULL) {
    strlit250 = alloc_String("HOME");
    gc_root(strlit250);
  }
// compilenode returning strlit250
  params[0] = strlit250;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres251 = callmethod(call249, "[]", 1, partcv, params);
// compilenode returning idxres251
  int op_slot_left_247 = gc_frame_newslot(idxres251);
  if (strlit252 == NULL) {
    strlit252 = alloc_String("/grace/packages/");
    gc_root(strlit252);
  }
// compilenode returning strlit252
  int op_slot_right_247 = gc_frame_newslot(strlit252);
  params[0] = strlit252;
  partcv[0] = 1;
  Object opresult254 = callmethod(idxres251, "++", 1, partcv, params);
// compilenode returning opresult254
  gc_frame_newslot(opresult254);
  params[0] = opresult254;
  partcv[0] = 1;
  Object call255 = callmethodflags(self, "checkListPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe246);
// compilenode returning call255
// Begin line 117
  setline(117);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe256 = gc_frame_new();
// Begin line 116
  setline(116);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe258 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call259 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe258);
// compilenode returning call259
// compilenode returning call259
  if (strlit260 == NULL) {
    strlit260 = alloc_String("HOME");
    gc_root(strlit260);
  }
// compilenode returning strlit260
  params[0] = strlit260;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres261 = callmethod(call259, "[]", 1, partcv, params);
// compilenode returning idxres261
  int op_slot_left_257 = gc_frame_newslot(idxres261);
  if (strlit262 == NULL) {
    strlit262 = alloc_String("/.local/lib/grace/modules/");
    gc_root(strlit262);
  }
// compilenode returning strlit262
  int op_slot_right_257 = gc_frame_newslot(strlit262);
  params[0] = strlit262;
  partcv[0] = 1;
  Object opresult264 = callmethod(idxres261, "++", 1, partcv, params);
// compilenode returning opresult264
  gc_frame_newslot(opresult264);
  params[0] = opresult264;
  partcv[0] = 1;
  Object call265 = callmethodflags(self, "checkListPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe256);
// compilenode returning call265
  gc_frame_end(frame);
  return call265;
}
Object meth_PackageManager_checkListPath266(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 4, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "checkListPath");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[1];
  Object *var_io = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for checkListPath (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_path = &(stackframe->slots[0]);
  *var_path = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for checkListPath");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 125
  setline(125);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 121
  setline(121);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe268 = gc_frame_new();
// compilenode returning *var_path
  gc_frame_newslot(*var_path);
// compilenode returning *var_io
  params[0] = *var_path;
  partcv[0] = 1;
  Object call269 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe268);
// compilenode returning call269
  Object if267 = done;
  if (istrue(call269)) {
// Begin line 123
  setline(123);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 122
  setline(122);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit272 == NULL) {
    strlit272 = alloc_String(" Installed in path: ");
    gc_root(strlit272);
  }
// compilenode returning strlit272
  int op_slot_left_271 = gc_frame_newslot(strlit272);
// compilenode returning *var_path
  int op_slot_right_271 = gc_frame_newslot(*var_path);
  params[0] = *var_path;
  partcv[0] = 1;
  Object opresult274 = callmethod(strlit272, "++", 1, partcv, params);
// compilenode returning opresult274
  int op_slot_left_270 = gc_frame_newslot(opresult274);
  if (strlit275 == NULL) {
    strlit275 = alloc_String("");
    gc_root(strlit275);
  }
// compilenode returning strlit275
  int op_slot_right_270 = gc_frame_newslot(strlit275);
  params[0] = strlit275;
  partcv[0] = 1;
  Object opresult277 = callmethod(opresult274, "++", 1, partcv, params);
// compilenode returning opresult277
  params[0] = opresult277;
  Object call278 = gracelib_print(NULL, 1,  params);
// compilenode returning call278
// Begin line 124
  setline(124);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe279 = gc_frame_new();
// Begin line 123
  setline(123);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_path
  gc_frame_newslot(*var_path);
  if (strlit280 == NULL) {
    strlit280 = alloc_String(" ");
    gc_root(strlit280);
  }
// compilenode returning strlit280
  gc_frame_newslot(strlit280);
  params[0] = *var_path;
  params[1] = strlit280;
  partcv[0] = 2;
  Object call281 = callmethodflags(self, "recurseDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe279);
// compilenode returning call281
    gc_frame_newslot(call281);
    if267 = call281;
  } else {
  }
// compilenode returning if267
  gc_frame_end(frame);
  return if267;
}
Object meth_PackageManager__apply287(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 136
  setline(136);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 130
  setline(130);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_p
  int op_slot_left_290 = gc_frame_newslot(*var_p);
  if (strlit291 == NULL) {
    strlit291 = alloc_String("..");
    gc_root(strlit291);
  }
// compilenode returning strlit291
  int op_slot_right_290 = gc_frame_newslot(strlit291);
  params[0] = strlit291;
  partcv[0] = 1;
  Object opresult293 = callmethod(*var_p, "!=", 1, partcv, params);
// compilenode returning opresult293
  int op_slot_left_289 = gc_frame_newslot(opresult293);
// compilenode returning *var_p
  int op_slot_left_294 = gc_frame_newslot(*var_p);
  if (strlit295 == NULL) {
    strlit295 = alloc_String(".");
    gc_root(strlit295);
  }
// compilenode returning strlit295
  int op_slot_right_294 = gc_frame_newslot(strlit295);
  params[0] = strlit295;
  partcv[0] = 1;
  Object opresult297 = callmethod(*var_p, "!=", 1, partcv, params);
// compilenode returning opresult297
  int op_slot_right_289 = gc_frame_newslot(opresult297);
  params[0] = opresult297;
  partcv[0] = 1;
  Object opresult299 = callmethod(opresult293, "&&", 1, partcv, params);
// compilenode returning opresult299
  Object if288 = done;
  if (istrue(opresult299)) {
// Begin line 132
  setline(132);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 131
  setline(131);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_padding
  int op_slot_left_300 = gc_frame_newslot(*var_padding);
// compilenode returning *var_p
  int op_slot_right_300 = gc_frame_newslot(*var_p);
  params[0] = *var_p;
  partcv[0] = 1;
  Object opresult302 = callmethod(*var_padding, "++", 1, partcv, params);
// compilenode returning opresult302
  params[0] = opresult302;
  Object call303 = gracelib_print(NULL, 1,  params);
// compilenode returning call303
// Begin line 135
  setline(135);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 132
  setline(132);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe306 = gc_frame_new();
  int callframe307 = gc_frame_new();
// compilenode returning *var_path
  int op_slot_left_309 = gc_frame_newslot(*var_path);
  if (strlit310 == NULL) {
    strlit310 = alloc_String("/");
    gc_root(strlit310);
  }
// compilenode returning strlit310
  int op_slot_right_309 = gc_frame_newslot(strlit310);
  params[0] = strlit310;
  partcv[0] = 1;
  Object opresult312 = callmethod(*var_path, "++", 1, partcv, params);
// compilenode returning opresult312
  int op_slot_left_308 = gc_frame_newslot(opresult312);
// compilenode returning *var_p
  int op_slot_right_308 = gc_frame_newslot(*var_p);
  params[0] = *var_p;
  partcv[0] = 1;
  Object opresult314 = callmethod(opresult312, "++", 1, partcv, params);
// compilenode returning opresult314
  gc_frame_newslot(opresult314);
// compilenode returning *var_io
  params[0] = opresult314;
  partcv[0] = 1;
  Object call315 = callmethod(*var_io, "getdir",
    1, partcv, params);
  gc_frame_end(callframe307);
// compilenode returning call315
  partcv[0] = 0;
  Object call316 = callmethod(call315, "size",
    1, partcv, params);
  gc_frame_end(callframe306);
// compilenode returning call316
// compilenode returning call316
  int op_slot_left_305 = gc_frame_newslot(call316);
  Object num317 = alloc_Float64(0);
// compilenode returning num317
  int op_slot_right_305 = gc_frame_newslot(num317);
  params[0] = num317;
  partcv[0] = 1;
  Object opresult319 = callmethod(call316, ">", 1, partcv, params);
// compilenode returning opresult319
  Object if304 = done;
  if (istrue(opresult319)) {
// Begin line 134
  setline(134);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe320 = gc_frame_new();
// Begin line 133
  setline(133);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_path
  int op_slot_left_322 = gc_frame_newslot(*var_path);
  if (strlit323 == NULL) {
    strlit323 = alloc_String("/");
    gc_root(strlit323);
  }
// compilenode returning strlit323
  int op_slot_right_322 = gc_frame_newslot(strlit323);
  params[0] = strlit323;
  partcv[0] = 1;
  Object opresult325 = callmethod(*var_path, "++", 1, partcv, params);
// compilenode returning opresult325
  int op_slot_left_321 = gc_frame_newslot(opresult325);
// compilenode returning *var_p
  int op_slot_right_321 = gc_frame_newslot(*var_p);
  params[0] = *var_p;
  partcv[0] = 1;
  Object opresult327 = callmethod(opresult325, "++", 1, partcv, params);
// compilenode returning opresult327
  gc_frame_newslot(opresult327);
// compilenode returning *var_padding
  int op_slot_left_328 = gc_frame_newslot(*var_padding);
  if (strlit329 == NULL) {
    strlit329 = alloc_String("   ");
    gc_root(strlit329);
  }
// compilenode returning strlit329
  int op_slot_right_328 = gc_frame_newslot(strlit329);
  params[0] = strlit329;
  partcv[0] = 1;
  Object opresult331 = callmethod(*var_padding, "++", 1, partcv, params);
// compilenode returning opresult331
  gc_frame_newslot(opresult331);
  params[0] = opresult327;
  params[1] = opresult331;
  partcv[0] = 2;
  Object call332 = callmethodflags(self, "recurseDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe320);
// compilenode returning call332
    gc_frame_newslot(call332);
    if304 = call332;
  } else {
  }
// compilenode returning if304
    gc_frame_newslot(if304);
    if288 = if304;
  } else {
  }
// compilenode returning if288
  gc_frame_end(frame);
  return if288;
}
Object meth_PackageManager_recurseDirectory282(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 5, (flags>>24)&0xff);
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
// Begin line 129
  setline(129);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe283 = gc_frame_new();
// Begin line 128
  setline(128);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_path
  gc_frame_newslot(*var_path);
// compilenode returning *var_io
  params[0] = *var_path;
  partcv[0] = 1;
  Object call284 = callmethod(*var_io, "getdir",
    1, partcv, params);
  gc_frame_end(callframe283);
// compilenode returning call284
  *var_pathContents = call284;
  if (call284 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 137
  setline(137);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe285 = gc_frame_new();
// Begin line 129
  setline(129);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pathContents
  gc_frame_newslot(*var_pathContents);
// Begin line 137
  setline(137);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block286 = alloc_Block(NULL, NULL, "PackageManager", 137);
  gc_frame_newslot(block286);
  block_savedest(block286);
  Object closure287 = createclosure(4, "_apply");
setclosureframe(closure287, stackframe);
  addtoclosure(closure287, var_padding);
  addtoclosure(closure287, var_path);
  addtoclosure(closure287, var_io);
  addtoclosure(closure287, selfslot);
  struct UserObject *uo287 = (struct UserObject*)block286;
  uo287->data[0] = (Object)closure287;
  Method *meth_meth_PackageManager__apply287 = addmethod2pos(block286, "_apply", &meth_PackageManager__apply287, 0);
int argcv_meth_PackageManager__apply287[] = {1};
meth_meth_PackageManager__apply287->type = alloc_MethodType(1, argcv_meth_PackageManager__apply287);
  meth_meth_PackageManager__apply287->definitionModule = modulename;
  meth_meth_PackageManager__apply287->definitionLine = 116;
// compilenode returning block286
  gc_frame_newslot(block286);
  params[0] = *var_pathContents;
  params[1] = block286;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call334 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe285);
// compilenode returning call334
  gc_frame_end(frame);
  return call334;
}
Object meth_PackageManager__apply379(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 153
  setline(153);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe380 = gc_frame_new();
// Begin line 152
  setline(152);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_im
  gc_frame_newslot(*var_im);
  params[0] = *var_im;
  partcv[0] = 1;
  Object call381 = callmethodflags(self, "write", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe380);
// compilenode returning call381
  gc_frame_end(frame);
  return call381;
}
Object meth_PackageManager__apply386(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 156
  setline(156);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe387 = gc_frame_new();
// Begin line 155
  setline(155);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_im
  gc_frame_newslot(*var_im);
  params[0] = *var_im;
  partcv[0] = 1;
  Object call388 = callmethodflags(self, "compile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe387);
// compilenode returning call388
  gc_frame_end(frame);
  return call388;
}
Object meth_PackageManager_doGet335(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 6, (flags>>24)&0xff);
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
  Object *var_done = getfromclosure(closure, 1);
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
// Begin line 142
  setline(142);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 140
  setline(140);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array336 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array336
  *var_imported = array336;
  if (array336 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 150
  setline(150);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 142
  setline(142);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe341 = gc_frame_new();
// compilenode returning *var_impAddress
  partcv[0] = 0;
  Object call342 = callmethod(*var_impAddress, "size",
    1, partcv, params);
  gc_frame_end(callframe341);
// compilenode returning call342
// compilenode returning call342
  int op_slot_left_340 = gc_frame_newslot(call342);
  Object num343 = alloc_Float64(7);
// compilenode returning num343
  int op_slot_right_340 = gc_frame_newslot(num343);
  params[0] = num343;
  partcv[0] = 1;
  Object opresult345 = callmethod(call342, ">=", 1, partcv, params);
// compilenode returning opresult345
  int op_slot_left_339 = gc_frame_newslot(opresult345);
  int callframe347 = gc_frame_new();
  Object num348 = alloc_Float64(1);
// compilenode returning num348
  gc_frame_newslot(num348);
  Object num349 = alloc_Float64(7);
// compilenode returning num349
  gc_frame_newslot(num349);
// compilenode returning *var_impAddress
  params[0] = num348;
  params[1] = num349;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call350 = callmethod(*var_impAddress, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe347);
// compilenode returning call350
  int op_slot_left_346 = gc_frame_newslot(call350);
  if (strlit351 == NULL) {
    strlit351 = alloc_String("http://");
    gc_root(strlit351);
  }
// compilenode returning strlit351
  int op_slot_right_346 = gc_frame_newslot(strlit351);
  params[0] = strlit351;
  partcv[0] = 1;
  Object opresult353 = callmethod(call350, "==", 1, partcv, params);
// compilenode returning opresult353
  int op_slot_right_339 = gc_frame_newslot(opresult353);
  params[0] = opresult353;
  partcv[0] = 1;
  Object opresult355 = callmethod(opresult345, "&&", 1, partcv, params);
// compilenode returning opresult355
  Object if338 = done;
  if (istrue(opresult355)) {
// Begin line 144
  setline(144);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe356 = gc_frame_new();
// Begin line 143
  setline(143);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_impAddress
  gc_frame_newslot(*var_impAddress);
  params[0] = *var_impAddress;
  partcv[0] = 1;
  Object call357 = callmethodflags(self, "setBaseUrl", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe356);
// compilenode returning call357
// Begin line 149
  setline(149);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 144
  setline(144);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe360 = gc_frame_new();
  int callframe362 = gc_frame_new();
// compilenode returning *var_impAddress
  partcv[0] = 0;
  Object call363 = callmethod(*var_impAddress, "size",
    1, partcv, params);
  gc_frame_end(callframe362);
// compilenode returning call363
// compilenode returning call363
  int op_slot_left_361 = gc_frame_newslot(call363);
  Object num364 = alloc_Float64(6);
// compilenode returning num364
  int op_slot_right_361 = gc_frame_newslot(num364);
  params[0] = num364;
  partcv[0] = 1;
  Object diff366 = callmethod(call363, "-", 1, partcv, params);
// compilenode returning diff366
  gc_frame_newslot(diff366);
  int callframe367 = gc_frame_new();
// compilenode returning *var_impAddress
  partcv[0] = 0;
  Object call368 = callmethod(*var_impAddress, "size",
    1, partcv, params);
  gc_frame_end(callframe367);
// compilenode returning call368
// compilenode returning call368
  gc_frame_newslot(call368);
// compilenode returning *var_impAddress
  params[0] = diff366;
  params[1] = call368;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call369 = callmethod(*var_impAddress, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe360);
// compilenode returning call369
  int op_slot_left_359 = gc_frame_newslot(call369);
  if (strlit370 == NULL) {
    strlit370 = alloc_String(".tar.gz");
    gc_root(strlit370);
  }
// compilenode returning strlit370
  int op_slot_right_359 = gc_frame_newslot(strlit370);
  params[0] = strlit370;
  partcv[0] = 1;
  Object opresult372 = callmethod(call369, "==", 1, partcv, params);
// compilenode returning opresult372
  Object if358 = done;
  if (istrue(opresult372)) {
// Begin line 146
  setline(146);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe373 = gc_frame_new();
// Begin line 145
  setline(145);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_impAddress
  gc_frame_newslot(*var_impAddress);
  params[0] = *var_impAddress;
  partcv[0] = 1;
  Object call374 = callmethodflags(self, "getPackage", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe373);
// compilenode returning call374
// Begin line 147
  setline(147);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_done
  return *var_done;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if358 = undefined;
  } else {
  }
// compilenode returning if358
    gc_frame_newslot(if358);
    if338 = if358;
  } else {
  }
// compilenode returning if338
// Begin line 151
  setline(151);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe375 = gc_frame_new();
// Begin line 150
  setline(150);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_impAddress
  gc_frame_newslot(*var_impAddress);
  params[0] = *var_impAddress;
  partcv[0] = 1;
  Object call376 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe375);
// compilenode returning call376
// Begin line 154
  setline(154);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe377 = gc_frame_new();
// Begin line 151
  setline(151);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_imported
  gc_frame_newslot(*var_imported);
// Begin line 154
  setline(154);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block378 = alloc_Block(NULL, NULL, "PackageManager", 154);
  gc_frame_newslot(block378);
  block_savedest(block378);
  Object closure379 = createclosure(1, "_apply");
setclosureframe(closure379, stackframe);
  addtoclosure(closure379, selfslot);
  struct UserObject *uo379 = (struct UserObject*)block378;
  uo379->data[0] = (Object)closure379;
  Method *meth_meth_PackageManager__apply379 = addmethod2pos(block378, "_apply", &meth_PackageManager__apply379, 0);
int argcv_meth_PackageManager__apply379[] = {1};
meth_meth_PackageManager__apply379->type = alloc_MethodType(1, argcv_meth_PackageManager__apply379);
  meth_meth_PackageManager__apply379->definitionModule = modulename;
  meth_meth_PackageManager__apply379->definitionLine = 144;
// compilenode returning block378
  gc_frame_newslot(block378);
  params[0] = *var_imported;
  params[1] = block378;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call383 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe377);
// compilenode returning call383
// Begin line 157
  setline(157);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe384 = gc_frame_new();
// Begin line 154
  setline(154);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_imported
  gc_frame_newslot(*var_imported);
// Begin line 157
  setline(157);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block385 = alloc_Block(NULL, NULL, "PackageManager", 157);
  gc_frame_newslot(block385);
  block_savedest(block385);
  Object closure386 = createclosure(1, "_apply");
setclosureframe(closure386, stackframe);
  addtoclosure(closure386, selfslot);
  struct UserObject *uo386 = (struct UserObject*)block385;
  uo386->data[0] = (Object)closure386;
  Method *meth_meth_PackageManager__apply386 = addmethod2pos(block385, "_apply", &meth_PackageManager__apply386, 0);
int argcv_meth_PackageManager__apply386[] = {1};
meth_meth_PackageManager__apply386->type = alloc_MethodType(1, argcv_meth_PackageManager__apply386);
  meth_meth_PackageManager__apply386->definitionModule = modulename;
  meth_meth_PackageManager__apply386->definitionLine = 144;
// compilenode returning block385
  gc_frame_newslot(block385);
  params[0] = *var_imported;
  params[1] = block385;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call390 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe384);
// compilenode returning call390
  gc_frame_end(frame);
  return call390;
}
Object meth_PackageManager_getPackage391(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct StackFrameObject *stackframe = alloc_StackFrame(2, NULL);
  pushclosure(NULL);
  pushstackframe(stackframe, "getPackage");
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
    gracedie("missing argument list for getPackage (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_impAddress = &(stackframe->slots[0]);
  *var_impAddress = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for getPackage");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_setBaseUrl392(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 8, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "setBaseUrl");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_baseUrl = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for setBaseUrl (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_baseAddress = &(stackframe->slots[0]);
  *var_baseAddress = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for setBaseUrl");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 168
  setline(168);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe393 = gc_frame_new();
// Begin line 167
  setline(167);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_baseAddress
  gc_frame_newslot(*var_baseAddress);
  params[0] = *var_baseAddress;
  partcv[0] = 1;
  Object call394 = callmethodflags(self, "getBaseUrl", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe393);
// compilenode returning call394
  *var_baseUrl = call394;
  if (call394 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_setFile396(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 9, (flags>>24)&0xff);
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
// Begin line 179
  setline(179);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 171
  setline(171);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe399 = gc_frame_new();
  int callframe401 = gc_frame_new();
// compilenode returning *var_fileAddress
  partcv[0] = 0;
  Object call402 = callmethod(*var_fileAddress, "size",
    1, partcv, params);
  gc_frame_end(callframe401);
// compilenode returning call402
// compilenode returning call402
  int op_slot_left_400 = gc_frame_newslot(call402);
  Object num403 = alloc_Float64(5);
// compilenode returning num403
  int op_slot_right_400 = gc_frame_newslot(num403);
  params[0] = num403;
  partcv[0] = 1;
  Object diff405 = callmethod(call402, "-", 1, partcv, params);
// compilenode returning diff405
  gc_frame_newslot(diff405);
  int callframe406 = gc_frame_new();
// compilenode returning *var_fileAddress
  partcv[0] = 0;
  Object call407 = callmethod(*var_fileAddress, "size",
    1, partcv, params);
  gc_frame_end(callframe406);
// compilenode returning call407
// compilenode returning call407
  gc_frame_newslot(call407);
// compilenode returning *var_fileAddress
  params[0] = diff405;
  params[1] = call407;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call408 = callmethod(*var_fileAddress, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe399);
// compilenode returning call408
  int op_slot_left_398 = gc_frame_newslot(call408);
  if (strlit409 == NULL) {
    strlit409 = alloc_String(".grace");
    gc_root(strlit409);
  }
// compilenode returning strlit409
  int op_slot_right_398 = gc_frame_newslot(strlit409);
  params[0] = strlit409;
  partcv[0] = 1;
  Object opresult411 = callmethod(call408, "==", 1, partcv, params);
// compilenode returning opresult411
  Object if397 = done;
  if (istrue(opresult411)) {
// Begin line 172
  setline(172);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object inheritingObject412 = inheritingObject;
  if (isTailObject) {
    isTailObject = 0;
    inheritingObject = NULL;
  }
  Object obj412 = alloc_userobj2(4,4, objclass412);
  gc_frame_newslot(obj412);
// OBJECT OUTER DEC outer
  adddatum2(obj412, self, 0);
  addmethodreal(obj412,"outer", &reader_PackageManager_outer_413);
  Object oldself412 = self;
  struct StackFrameObject *oldstackframe412 = stackframe;
  stackframe = alloc_StackFrame(1, oldstackframe412);
  gc_frame_newslot((Object)stackframe);
  self = obj412;
  Object *oldselfslot412 = selfslot;
  selfslot = &stackframe->slots[0];
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  Object thisouter412 = (*(struct UserObject *)self).data[0], lowerouter412 = thisouter412;
  if (inheritingObject412) {
    struct UserObject *inho412 = (struct UserObject *)inheritingObject412;
    while (inho412->super != GraceDefaultObject) inho412 = (struct UserObject *)inho412->super;
    inho412->super = obj412;
    self = inheritingObject412;
    *selfslot = self;
    lowerouter412 = (*(struct UserObject *)self).data[0];
    (*(struct UserObject *)self).data[0] = thisouter412;
  }
if (objclass412 == NULL) {
  Method *reader414 = addmethodrealflags(obj412, "address",&reader_PackageManager_address_414, 0);
  Method *writer414 = addmethodrealflags(obj412, "address:=",&writer_PackageManager_address_414, 0);
  reader414->definitionModule = modulename;
  writer414->definitionModule = modulename;
  reader414->definitionLine = 174;
  writer414->definitionLine = 174;
}
obj412->flags |= OFLAG_MUTABLE;
adddatum2(obj412, alloc_Undefined(), 1);
if (objclass412 == NULL) {
  Method *reader415 = addmethodrealflags(obj412, "data",&reader_PackageManager_data_415, 0);
  Method *writer415 = addmethodrealflags(obj412, "data:=",&writer_PackageManager_data_415, 0);
  reader415->definitionModule = modulename;
  writer415->definitionModule = modulename;
  reader415->definitionLine = 175;
  writer415->definitionLine = 175;
}
obj412->flags |= OFLAG_MUTABLE;
adddatum2(obj412, alloc_Undefined(), 2);
  sourceObject = obj412;
// Begin line 173
  setline(173);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_fileAddress
  adddatum2(obj412, *var_fileAddress, 1);
  sourceObject = obj412;
  adddatum2(obj412, undefined, 2);
objclass412 = obj412->class;
  objclass412->definitionModule = modulename;
  objclass412->definitionLine = 172;
  (*(struct UserObject *)self).data[0] = lowerouter412;
  self = oldself412;
  selfslot = oldselfslot412;
  stackframe = oldstackframe412;
// compilenode returning obj412
  *var_file = obj412;
  if (obj412 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 177
  setline(177);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 176
  setline(176);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_file
  *var_curFile = *var_file;
  if (*var_file == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 178
  setline(178);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 177
  setline(177);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool417 = alloc_Boolean(1);
// compilenode returning bool417
  return bool417;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if397 = undefined;
  } else {
  }
// compilenode returning if397
// Begin line 180
  setline(180);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 179
  setline(179);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool418 = alloc_Boolean(0);
// compilenode returning bool418
  return bool418;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply464(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int callframe466 = gc_frame_new();
// compilenode returning *var_toProcess
  partcv[0] = 0;
  Object call467 = callmethod(*var_toProcess, "size",
    1, partcv, params);
  gc_frame_end(callframe466);
// compilenode returning call467
// compilenode returning call467
  int op_slot_left_465 = gc_frame_newslot(call467);
  Object num468 = alloc_Float64(0);
// compilenode returning num468
  int op_slot_right_465 = gc_frame_newslot(num468);
  params[0] = num468;
  partcv[0] = 1;
  Object opresult470 = callmethod(call467, ">", 1, partcv, params);
// compilenode returning opresult470
  gc_frame_end(frame);
  return opresult470;
}
Object meth_PackageManager__apply473(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 195
  setline(195);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe474 = gc_frame_new();
// Begin line 194
  setline(194);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe475 = gc_frame_new();
// compilenode returning *var_toProcess
  partcv[0] = 0;
  Object call476 = callmethod(*var_toProcess, "pop",
    1, partcv, params);
  gc_frame_end(callframe475);
// compilenode returning call476
// compilenode returning call476
  gc_frame_newslot(call476);
  params[0] = call476;
  partcv[0] = 1;
  Object call477 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe474);
// compilenode returning call477
  gc_frame_end(frame);
  return call477;
}
Object meth_PackageManager_fetchImports419(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 10, (flags>>24)&0xff);
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
// Begin line 205
  setline(205);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 183
  setline(183);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe421 = gc_frame_new();
// compilenode returning *var_fileAddress
  gc_frame_newslot(*var_fileAddress);
  params[0] = *var_fileAddress;
  partcv[0] = 1;
  Object call422 = callmethodflags(self, "setFile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe421);
// compilenode returning call422
  Object if420 = done;
  if (istrue(call422)) {
// Begin line 185
  setline(185);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 184
  setline(184);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit425 == NULL) {
    strlit425 = alloc_String("File address ");
    gc_root(strlit425);
  }
// compilenode returning strlit425
  int op_slot_left_424 = gc_frame_newslot(strlit425);
  int callframe426 = gc_frame_new();
// compilenode returning *var_curFile
  partcv[0] = 0;
  Object call427 = callmethod(*var_curFile, "address",
    1, partcv, params);
  gc_frame_end(callframe426);
// compilenode returning call427
// compilenode returning call427
  int op_slot_right_424 = gc_frame_newslot(call427);
  params[0] = call427;
  partcv[0] = 1;
  Object opresult429 = callmethod(strlit425, "++", 1, partcv, params);
// compilenode returning opresult429
  int op_slot_left_423 = gc_frame_newslot(opresult429);
  if (strlit430 == NULL) {
    strlit430 = alloc_String("");
    gc_root(strlit430);
  }
// compilenode returning strlit430
  int op_slot_right_423 = gc_frame_newslot(strlit430);
  params[0] = strlit430;
  partcv[0] = 1;
  Object opresult432 = callmethod(opresult429, "++", 1, partcv, params);
// compilenode returning opresult432
  params[0] = opresult432;
  Object call433 = gracelib_print(NULL, 1,  params);
// compilenode returning call433
// Begin line 189
  setline(189);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 185
  setline(185);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe436 = gc_frame_new();
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call437 = callmethodflags(self, "setFileData", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe436);
// compilenode returning call437
  int op_slot_left_435 = gc_frame_newslot(call437);
  Object bool438 = alloc_Boolean(0);
// compilenode returning bool438
  int op_slot_right_435 = gc_frame_newslot(bool438);
  params[0] = bool438;
  partcv[0] = 1;
  Object opresult440 = callmethod(call437, "==", 1, partcv, params);
// compilenode returning opresult440
  Object if434 = done;
  if (istrue(opresult440)) {
// Begin line 187
  setline(187);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 186
  setline(186);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit441 == NULL) {
    strlit441 = alloc_String("Could not retrieve file data");
    gc_root(strlit441);
  }
// compilenode returning strlit441
  params[0] = strlit441;
  Object call442 = gracelib_print(NULL, 1,  params);
// compilenode returning call442
// Begin line 188
  setline(188);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 187
  setline(187);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool443 = alloc_Boolean(0);
// compilenode returning bool443
  return bool443;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if434 = undefined;
  } else {
  }
// compilenode returning if434
// Begin line 198
  setline(198);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 189
  setline(189);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe445 = gc_frame_new();
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call446 = callmethodflags(self, "validateFile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe445);
// compilenode returning call446
  Object if444 = done;
  if (istrue(call446)) {
// Begin line 191
  setline(191);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 190
  setline(190);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit449 == NULL) {
    strlit449 = alloc_String("Pushing ");
    gc_root(strlit449);
  }
// compilenode returning strlit449
  int op_slot_left_448 = gc_frame_newslot(strlit449);
  int callframe450 = gc_frame_new();
// compilenode returning *var_curFile
  partcv[0] = 0;
  Object call451 = callmethod(*var_curFile, "address",
    1, partcv, params);
  gc_frame_end(callframe450);
// compilenode returning call451
// compilenode returning call451
  int op_slot_right_448 = gc_frame_newslot(call451);
  params[0] = call451;
  partcv[0] = 1;
  Object opresult453 = callmethod(strlit449, "++", 1, partcv, params);
// compilenode returning opresult453
  int op_slot_left_447 = gc_frame_newslot(opresult453);
  if (strlit454 == NULL) {
    strlit454 = alloc_String(" to imported");
    gc_root(strlit454);
  }
// compilenode returning strlit454
  int op_slot_right_447 = gc_frame_newslot(strlit454);
  params[0] = strlit454;
  partcv[0] = 1;
  Object opresult456 = callmethod(opresult453, "++", 1, partcv, params);
// compilenode returning opresult456
  params[0] = opresult456;
  Object call457 = gracelib_print(NULL, 1,  params);
// compilenode returning call457
// Begin line 192
  setline(192);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe458 = gc_frame_new();
// Begin line 191
  setline(191);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
// compilenode returning *var_imported
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call459 = callmethod(*var_imported, "push",
    1, partcv, params);
  gc_frame_end(callframe458);
// compilenode returning call459
// Begin line 193
  setline(193);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe460 = gc_frame_new();
// Begin line 192
  setline(192);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call461 = callmethodflags(self, "parseFile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe460);
// compilenode returning call461
// Begin line 196
  setline(196);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe462 = gc_frame_new();
// Begin line 193
  setline(193);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block463 = alloc_Block(NULL, NULL, "PackageManager", 193);
  gc_frame_newslot(block463);
  block_savedest(block463);
  Object closure464 = createclosure(2, "_apply");
setclosureframe(closure464, stackframe);
  addtoclosure(closure464, var_toProcess);
  addtoclosure(closure464, selfslot);
  struct UserObject *uo464 = (struct UserObject*)block463;
  uo464->data[0] = (Object)closure464;
  Method *meth_meth_PackageManager__apply464 = addmethod2pos(block463, "_apply", &meth_PackageManager__apply464, 0);
int argcv_meth_PackageManager__apply464[] = {0};
meth_meth_PackageManager__apply464->type = alloc_MethodType(1, argcv_meth_PackageManager__apply464);
  meth_meth_PackageManager__apply464->definitionModule = modulename;
  meth_meth_PackageManager__apply464->definitionLine = 190;
// compilenode returning block463
  gc_frame_newslot(block463);
// Begin line 196
  setline(196);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block472 = alloc_Block(NULL, NULL, "PackageManager", 196);
  gc_frame_newslot(block472);
  block_savedest(block472);
  Object closure473 = createclosure(2, "_apply");
setclosureframe(closure473, stackframe);
  addtoclosure(closure473, var_toProcess);
  addtoclosure(closure473, selfslot);
  struct UserObject *uo473 = (struct UserObject*)block472;
  uo473->data[0] = (Object)closure473;
  Method *meth_meth_PackageManager__apply473 = addmethod2pos(block472, "_apply", &meth_PackageManager__apply473, 0);
int argcv_meth_PackageManager__apply473[] = {0};
meth_meth_PackageManager__apply473->type = alloc_MethodType(1, argcv_meth_PackageManager__apply473);
  meth_meth_PackageManager__apply473->definitionModule = modulename;
  meth_meth_PackageManager__apply473->definitionLine = 193;
// compilenode returning block472
  gc_frame_newslot(block472);
  params[0] = block463;
  params[1] = block472;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call479 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe462);
// compilenode returning call479
// Begin line 197
  setline(197);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 196
  setline(196);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool480 = alloc_Boolean(1);
// compilenode returning bool480
  return bool480;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if444 = undefined;
  } else {
  }
// compilenode returning if444
    gc_frame_newslot(if444);
    if420 = if444;
  } else {
// Begin line 204
  setline(204);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 200
  setline(200);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe483 = gc_frame_new();
// compilenode returning *var_fileAddress
  int op_slot_left_484 = gc_frame_newslot(*var_fileAddress);
  if (strlit485 == NULL) {
    strlit485 = alloc_String(".grace");
    gc_root(strlit485);
  }
// compilenode returning strlit485
  int op_slot_right_484 = gc_frame_newslot(strlit485);
  params[0] = strlit485;
  partcv[0] = 1;
  Object opresult487 = callmethod(*var_fileAddress, "++", 1, partcv, params);
// compilenode returning opresult487
  gc_frame_newslot(opresult487);
  params[0] = opresult487;
  partcv[0] = 1;
  Object call488 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe483);
// compilenode returning call488
  int op_slot_left_482 = gc_frame_newslot(call488);
  Object bool489 = alloc_Boolean(0);
// compilenode returning bool489
  int op_slot_right_482 = gc_frame_newslot(bool489);
  params[0] = bool489;
  partcv[0] = 1;
  Object opresult491 = callmethod(call488, "==", 1, partcv, params);
// compilenode returning opresult491
  Object if481 = done;
  if (istrue(opresult491)) {
// Begin line 202
  setline(202);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 201
  setline(201);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit492 == NULL) {
    strlit492 = alloc_String("Could not locate file. Check file address.");
    gc_root(strlit492);
  }
// compilenode returning strlit492
  params[0] = strlit492;
  Object call493 = gracelib_print(NULL, 1,  params);
// compilenode returning call493
// Begin line 203
  setline(203);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 202
  setline(202);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool494 = alloc_Boolean(0);
// compilenode returning bool494
  return bool494;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if481 = undefined;
  } else {
  }
// compilenode returning if481
    gc_frame_newslot(if481);
    if420 = if481;
  }
// compilenode returning if420
  gc_frame_end(frame);
  return if420;
}
Object meth_PackageManager__apply513(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  Object *var_req = getfromclosure(closure, 0);
  Object *var_file = getfromclosure(closure, 1);
  Object self = *(getfromclosure(closure, 2));
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
// Begin line 214
  setline(214);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 213
  setline(213);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit514 == NULL) {
    strlit514 = alloc_String(" Response received");
    gc_root(strlit514);
  }
// compilenode returning strlit514
  params[0] = strlit514;
  Object call515 = gracelib_print(NULL, 1,  params);
// compilenode returning call515
// Begin line 218
  setline(218);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 214
  setline(214);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe518 = gc_frame_new();
// compilenode returning *var_req
  partcv[0] = 0;
  Object call519 = callmethod(*var_req, "responseCode",
    1, partcv, params);
  gc_frame_end(callframe518);
// compilenode returning call519
// compilenode returning call519
  int op_slot_left_517 = gc_frame_newslot(call519);
  Object num520 = alloc_Float64(200);
// compilenode returning num520
  int op_slot_right_517 = gc_frame_newslot(num520);
  params[0] = num520;
  partcv[0] = 1;
  Object opresult522 = callmethod(call519, "!=", 1, partcv, params);
// compilenode returning opresult522
  Object if516 = done;
  if (istrue(opresult522)) {
// Begin line 216
  setline(216);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 215
  setline(215);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit527 == NULL) {
    strlit527 = alloc_String(" Could not locate import: Error ");
    gc_root(strlit527);
  }
// compilenode returning strlit527
  int op_slot_left_526 = gc_frame_newslot(strlit527);
  int callframe528 = gc_frame_new();
// compilenode returning *var_req
  partcv[0] = 0;
  Object call529 = callmethod(*var_req, "responseCode",
    1, partcv, params);
  gc_frame_end(callframe528);
// compilenode returning call529
// compilenode returning call529
  int op_slot_right_526 = gc_frame_newslot(call529);
  params[0] = call529;
  partcv[0] = 1;
  Object opresult531 = callmethod(strlit527, "++", 1, partcv, params);
// compilenode returning opresult531
  int op_slot_left_525 = gc_frame_newslot(opresult531);
  if (strlit532 == NULL) {
    strlit532 = alloc_String(" for import ");
    gc_root(strlit532);
  }
// compilenode returning strlit532
  int op_slot_right_525 = gc_frame_newslot(strlit532);
  params[0] = strlit532;
  partcv[0] = 1;
  Object opresult534 = callmethod(opresult531, "++", 1, partcv, params);
// compilenode returning opresult534
  int op_slot_left_524 = gc_frame_newslot(opresult534);
  int callframe535 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call536 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe535);
// compilenode returning call536
// compilenode returning call536
  int op_slot_right_524 = gc_frame_newslot(call536);
  params[0] = call536;
  partcv[0] = 1;
  Object opresult538 = callmethod(opresult534, "++", 1, partcv, params);
// compilenode returning opresult538
  int op_slot_left_523 = gc_frame_newslot(opresult538);
  if (strlit539 == NULL) {
    strlit539 = alloc_String("");
    gc_root(strlit539);
  }
// compilenode returning strlit539
  int op_slot_right_523 = gc_frame_newslot(strlit539);
  params[0] = strlit539;
  partcv[0] = 1;
  Object opresult541 = callmethod(opresult538, "++", 1, partcv, params);
// compilenode returning opresult541
  params[0] = opresult541;
  Object call542 = gracelib_print(NULL, 1,  params);
// compilenode returning call542
// Begin line 217
  setline(217);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 216
  setline(216);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool543 = alloc_Boolean(0);
// compilenode returning bool543
  block_return(realself, bool543);
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if516 = undefined;
  } else {
  }
// compilenode returning if516
// Begin line 219
  setline(219);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 215
  setline(215);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe544 = gc_frame_new();
// Begin line 219
  setline(219);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe545 = gc_frame_new();
// Begin line 218
  setline(218);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit546 == NULL) {
    strlit546 = alloc_String("utf-8");
    gc_root(strlit546);
  }
// compilenode returning strlit546
  gc_frame_newslot(strlit546);
// compilenode returning *var_d
  params[0] = strlit546;
  partcv[0] = 1;
  Object call547 = callmethod(*var_d, "decode",
    1, partcv, params);
  gc_frame_end(callframe545);
// compilenode returning call547
  gc_frame_newslot(call547);
// compilenode returning *var_file
  params[0] = call547;
  partcv[0] = 1;
  Object call548 = callmethod(*var_file, "data:=",
    1, partcv, params);
  gc_frame_end(callframe544);
// compilenode returning call548
// compilenode returning done
// Begin line 220
  setline(220);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 219
  setline(219);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool549 = alloc_Boolean(1);
// compilenode returning bool549
  block_return(realself, bool549);
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_performCurlFetch495(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 11, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(3, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "performCurlFetch");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_curl = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for performCurlFetch (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_file = &(stackframe->slots[0]);
  *var_file = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for performCurlFetch");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_req = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "req");
// Begin line 209
  setline(209);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 208
  setline(208);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe496 = gc_frame_new();
// compilenode returning *var_curl
  partcv[0] = 0;
  Object call497 = callmethod(*var_curl, "easy",
    1, partcv, params);
  gc_frame_end(callframe496);
// compilenode returning call497
// compilenode returning call497
  *var_req = call497;
  if (call497 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 210
  setline(210);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 208
  setline(208);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe498 = gc_frame_new();
// Begin line 209
  setline(209);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe499 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call500 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe499);
// compilenode returning call500
// compilenode returning call500
  gc_frame_newslot(call500);
// compilenode returning *var_req
  params[0] = call500;
  partcv[0] = 1;
  Object call501 = callmethod(*var_req, "url:=",
    1, partcv, params);
  gc_frame_end(callframe498);
// compilenode returning call501
// compilenode returning done
// Begin line 211
  setline(211);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 210
  setline(210);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit503 == NULL) {
    strlit503 = alloc_String("Searching for import: ");
    gc_root(strlit503);
  }
// compilenode returning strlit503
  int op_slot_left_502 = gc_frame_newslot(strlit503);
  int callframe504 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call505 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe504);
// compilenode returning call505
// compilenode returning call505
  int op_slot_right_502 = gc_frame_newslot(call505);
  params[0] = call505;
  partcv[0] = 1;
  Object opresult507 = callmethod(strlit503, "++", 1, partcv, params);
// compilenode returning opresult507
  params[0] = opresult507;
  Object call508 = gracelib_print(NULL, 1,  params);
// compilenode returning call508
// Begin line 212
  setline(212);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 211
  setline(211);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit509 == NULL) {
    strlit509 = alloc_String(" Connecting...");
    gc_root(strlit509);
  }
// compilenode returning strlit509
  params[0] = strlit509;
  Object call510 = gracelib_print(NULL, 1,  params);
// compilenode returning call510
// Begin line 221
  setline(221);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe511 = gc_frame_new();
  Object block512 = alloc_Block(NULL, NULL, "PackageManager", 221);
  gc_frame_newslot(block512);
  block_savedest(block512);
  Object closure513 = createclosure(3, "_apply");
setclosureframe(closure513, stackframe);
  addtoclosure(closure513, var_req);
  addtoclosure(closure513, var_file);
  addtoclosure(closure513, selfslot);
  struct UserObject *uo513 = (struct UserObject*)block512;
  uo513->data[0] = (Object)closure513;
  Method *meth_meth_PackageManager__apply513 = addmethod2pos(block512, "_apply", &meth_PackageManager__apply513, 0);
int argcv_meth_PackageManager__apply513[] = {1};
meth_meth_PackageManager__apply513->type = alloc_MethodType(1, argcv_meth_PackageManager__apply513);
  meth_meth_PackageManager__apply513->definitionModule = modulename;
  meth_meth_PackageManager__apply513->definitionLine = 210;
// compilenode returning block512
  gc_frame_newslot(block512);
// Begin line 212
  setline(212);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_req
  params[0] = block512;
  partcv[0] = 1;
  Object call551 = callmethod(*var_req, "onReceive",
    1, partcv, params);
  gc_frame_end(callframe511);
// compilenode returning call551
// Begin line 221
  setline(221);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe552 = gc_frame_new();
// compilenode returning *var_req
  partcv[0] = 0;
  Object call553 = callmethod(*var_req, "perform",
    1, partcv, params);
  gc_frame_end(callframe552);
// compilenode returning call553
// compilenode returning call553
  gc_frame_end(frame);
  return call553;
}
Object meth_PackageManager_setFileData554(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 12, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(5, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "setFileData");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  Object *var_baseUrl = getfromclosure(closure, 0);
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
// Begin line 252
  setline(252);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 225
  setline(225);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe557 = gc_frame_new();
  Object num558 = alloc_Float64(1);
// compilenode returning num558
  gc_frame_newslot(num558);
  Object num559 = alloc_Float64(4);
// compilenode returning num559
  gc_frame_newslot(num559);
  int callframe560 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call561 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe560);
// compilenode returning call561
// compilenode returning call561
  params[0] = num558;
  params[1] = num559;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call562 = callmethod(call561, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe557);
// compilenode returning call562
  int op_slot_left_556 = gc_frame_newslot(call562);
  if (strlit563 == NULL) {
    strlit563 = alloc_String("http");
    gc_root(strlit563);
  }
// compilenode returning strlit563
  int op_slot_right_556 = gc_frame_newslot(strlit563);
  params[0] = strlit563;
  partcv[0] = 1;
  Object opresult565 = callmethod(call562, "==", 1, partcv, params);
// compilenode returning opresult565
  Object if555 = done;
  if (istrue(opresult565)) {
// Begin line 227
  setline(227);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe566 = gc_frame_new();
// Begin line 226
  setline(226);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num567 = alloc_Float64(1);
// compilenode returning num567
  gc_frame_newslot(num567);
  Object num568 = alloc_Float64(4);
// compilenode returning num568
  gc_frame_newslot(num568);
  int callframe569 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call570 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe569);
// compilenode returning call570
// compilenode returning call570
  params[0] = num567;
  params[1] = num568;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call571 = callmethod(call570, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe566);
// compilenode returning call571
  *var_strippedUrl = call571;
  if (call571 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 235
  setline(235);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 227
  setline(227);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe574 = gc_frame_new();
  int callframe575 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call576 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe575);
// compilenode returning call576
// compilenode returning call576
  gc_frame_newslot(call576);
  params[0] = call576;
  partcv[0] = 1;
  Object call577 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe574);
// compilenode returning call577
  int op_slot_left_573 = gc_frame_newslot(call577);
  Object bool578 = alloc_Boolean(0);
// compilenode returning bool578
  int op_slot_right_573 = gc_frame_newslot(bool578);
  params[0] = bool578;
  partcv[0] = 1;
  Object opresult580 = callmethod(call577, "!=", 1, partcv, params);
// compilenode returning opresult580
  Object if572 = done;
  if (istrue(opresult580)) {
// Begin line 229
  setline(229);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe581 = gc_frame_new();
// Begin line 228
  setline(228);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_strippedUrl
  gc_frame_newslot(*var_strippedUrl);
  params[0] = *var_strippedUrl;
  partcv[0] = 1;
  Object call582 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe581);
// compilenode returning call582
  *var_findData = call582;
  if (call582 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 233
  setline(233);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 229
  setline(229);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  int op_slot_left_584 = gc_frame_newslot(*var_findData);
  Object num585 = alloc_Float64(0);
// compilenode returning num585
  int op_slot_right_584 = gc_frame_newslot(num585);
  params[0] = num585;
  partcv[0] = 1;
  Object opresult587 = callmethod(*var_findData, ">", 1, partcv, params);
// compilenode returning opresult587
  Object if583 = done;
  if (istrue(opresult587)) {
// Begin line 231
  setline(231);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 227
  setline(227);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe588 = gc_frame_new();
// Begin line 230
  setline(230);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  gc_frame_newslot(*var_findData);
// compilenode returning *var_file
  params[0] = *var_findData;
  partcv[0] = 1;
  Object call589 = callmethod(*var_file, "data:=",
    1, partcv, params);
  gc_frame_end(callframe588);
// compilenode returning call589
// compilenode returning done
// Begin line 232
  setline(232);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 231
  setline(231);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool590 = alloc_Boolean(1);
// compilenode returning bool590
  return bool590;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if583 = undefined;
  } else {
  }
// compilenode returning if583
// Begin line 233
  setline(233);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool591 = alloc_Boolean(0);
// compilenode returning bool591
  return bool591;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if572 = undefined;
  } else {
  }
// compilenode returning if572
// Begin line 237
  setline(237);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe592 = gc_frame_new();
// Begin line 235
  setline(235);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_file
  gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object call593 = callmethodflags(self, "performCurlFetch", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe592);
// compilenode returning call593
  return call593;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if555 = undefined;
  } else {
// Begin line 246
  setline(246);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 238
  setline(238);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe596 = gc_frame_new();
  int callframe597 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call598 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe597);
// compilenode returning call598
// compilenode returning call598
  gc_frame_newslot(call598);
  params[0] = call598;
  partcv[0] = 1;
  Object call599 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe596);
// compilenode returning call599
  int op_slot_left_595 = gc_frame_newslot(call599);
  Object bool600 = alloc_Boolean(0);
// compilenode returning bool600
  int op_slot_right_595 = gc_frame_newslot(bool600);
  params[0] = bool600;
  partcv[0] = 1;
  Object opresult602 = callmethod(call599, "!=", 1, partcv, params);
// compilenode returning opresult602
  Object if594 = done;
  if (istrue(opresult602)) {
// Begin line 240
  setline(240);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 239
  setline(239);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit603 == NULL) {
    strlit603 = alloc_String("Now searching in find existing");
    gc_root(strlit603);
  }
// compilenode returning strlit603
  params[0] = strlit603;
  Object call604 = gracelib_print(NULL, 1,  params);
// compilenode returning call604
// Begin line 241
  setline(241);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe605 = gc_frame_new();
// Begin line 240
  setline(240);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe606 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call607 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe606);
// compilenode returning call607
// compilenode returning call607
  gc_frame_newslot(call607);
  params[0] = call607;
  partcv[0] = 1;
  Object call608 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe605);
// compilenode returning call608
  *var_findData = call608;
  if (call608 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 245
  setline(245);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 241
  setline(241);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  int op_slot_left_610 = gc_frame_newslot(*var_findData);
  Object bool611 = alloc_Boolean(0);
// compilenode returning bool611
  int op_slot_right_610 = gc_frame_newslot(bool611);
  params[0] = bool611;
  partcv[0] = 1;
  Object opresult613 = callmethod(*var_findData, "!=", 1, partcv, params);
// compilenode returning opresult613
  Object if609 = done;
  if (istrue(opresult613)) {
// Begin line 243
  setline(243);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 240
  setline(240);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe614 = gc_frame_new();
// Begin line 242
  setline(242);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  gc_frame_newslot(*var_findData);
// compilenode returning *var_file
  params[0] = *var_findData;
  partcv[0] = 1;
  Object call615 = callmethod(*var_file, "data:=",
    1, partcv, params);
  gc_frame_end(callframe614);
// compilenode returning call615
// compilenode returning done
// Begin line 244
  setline(244);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 243
  setline(243);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool616 = alloc_Boolean(1);
// compilenode returning bool616
  return bool616;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if609 = undefined;
  } else {
  }
// compilenode returning if609
    gc_frame_newslot(if609);
    if594 = if609;
  } else {
// Begin line 252
  setline(252);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 246
  setline(246);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_baseUrl
  int op_slot_left_618 = gc_frame_newslot(*var_baseUrl);
  if (strlit619 == NULL) {
    strlit619 = alloc_String("");
    gc_root(strlit619);
  }
// compilenode returning strlit619
  int op_slot_right_618 = gc_frame_newslot(strlit619);
  params[0] = strlit619;
  partcv[0] = 1;
  Object opresult621 = callmethod(*var_baseUrl, "!=", 1, partcv, params);
// compilenode returning opresult621
  Object if617 = done;
  if (istrue(opresult621)) {
// Begin line 249
  setline(249);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 240
  setline(240);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe622 = gc_frame_new();
// Begin line 249
  setline(249);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 248
  setline(248);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_baseUrl
  int op_slot_left_623 = gc_frame_newslot(*var_baseUrl);
  int callframe624 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call625 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe624);
// compilenode returning call625
// compilenode returning call625
  int op_slot_right_623 = gc_frame_newslot(call625);
  params[0] = call625;
  partcv[0] = 1;
  Object opresult627 = callmethod(*var_baseUrl, "++", 1, partcv, params);
// compilenode returning opresult627
  gc_frame_newslot(opresult627);
// compilenode returning *var_file
  params[0] = opresult627;
  partcv[0] = 1;
  Object call628 = callmethod(*var_file, "address:=",
    1, partcv, params);
  gc_frame_end(callframe622);
// compilenode returning call628
// compilenode returning done
// Begin line 251
  setline(251);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe629 = gc_frame_new();
// Begin line 249
  setline(249);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_file
  gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object call630 = callmethodflags(self, "performCurlFetch", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe629);
// compilenode returning call630
  return call630;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if617 = undefined;
  } else {
  }
// compilenode returning if617
    gc_frame_newslot(if617);
    if594 = if617;
  }
// compilenode returning if594
    gc_frame_newslot(if594);
    if555 = if594;
  }
// compilenode returning if555
// Begin line 253
  setline(253);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 252
  setline(252);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool631 = alloc_Boolean(0);
// compilenode returning bool631
  return bool631;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_findExisting632(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 13, (flags>>24)&0xff);
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
// Begin line 261
  setline(261);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 256
  setline(256);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe634 = gc_frame_new();
// compilenode returning *var_fileName
  gc_frame_newslot(*var_fileName);
// compilenode returning *var_io
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object call635 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe634);
// compilenode returning call635
  Object if633 = done;
  if (istrue(call635)) {
// Begin line 258
  setline(258);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe636 = gc_frame_new();
// Begin line 257
  setline(257);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_fileName
  gc_frame_newslot(*var_fileName);
  if (strlit637 == NULL) {
    strlit637 = alloc_String("r");
    gc_root(strlit637);
  }
// compilenode returning strlit637
  gc_frame_newslot(strlit637);
// compilenode returning *var_io
  params[0] = *var_fileName;
  params[1] = strlit637;
  partcv[0] = 2;
  Object call638 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe636);
// compilenode returning call638
  *var_open = call638;
  if (call638 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 259
  setline(259);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 258
  setline(258);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe639 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call640 = callmethod(*var_open, "read",
    1, partcv, params);
  gc_frame_end(callframe639);
// compilenode returning call640
// compilenode returning call640
  *var_ret = call640;
  if (call640 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 259
  setline(259);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_ret
  return *var_ret;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if633 = undefined;
  } else {
  }
// compilenode returning if633
// Begin line 264
  setline(264);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 261
  setline(261);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe642 = gc_frame_new();
  if (strlit644 == NULL) {
    strlit644 = alloc_String("/usr/lib/grace/modules/");
    gc_root(strlit644);
  }
// compilenode returning strlit644
  int op_slot_left_643 = gc_frame_newslot(strlit644);
// compilenode returning *var_fileName
  int op_slot_right_643 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult646 = callmethod(strlit644, "++", 1, partcv, params);
// compilenode returning opresult646
  gc_frame_newslot(opresult646);
// compilenode returning *var_io
  params[0] = opresult646;
  partcv[0] = 1;
  Object call647 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe642);
// compilenode returning call647
  Object if641 = done;
  if (istrue(call647)) {
// Begin line 263
  setline(263);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 262
  setline(262);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe648 = gc_frame_new();
  int callframe649 = gc_frame_new();
  if (strlit651 == NULL) {
    strlit651 = alloc_String("/usr/lib/grace/modules/");
    gc_root(strlit651);
  }
// compilenode returning strlit651
  int op_slot_left_650 = gc_frame_newslot(strlit651);
// compilenode returning *var_fileName
  int op_slot_right_650 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult653 = callmethod(strlit651, "++", 1, partcv, params);
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
  gc_frame_end(callframe649);
// compilenode returning call655
  partcv[0] = 0;
  Object call656 = callmethod(call655, "read",
    1, partcv, params);
  gc_frame_end(callframe648);
// compilenode returning call656
// compilenode returning call656
  return call656;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if641 = undefined;
  } else {
  }
// compilenode returning if641
// Begin line 265
  setline(265);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 264
  setline(264);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe657 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call658 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe657);
// compilenode returning call658
// compilenode returning call658
  if (strlit659 == NULL) {
    strlit659 = alloc_String("HOME");
    gc_root(strlit659);
  }
// compilenode returning strlit659
  params[0] = strlit659;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres660 = callmethod(call658, "[]", 1, partcv, params);
// compilenode returning idxres660
  *var_homePath = idxres660;
  if (idxres660 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 268
  setline(268);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 265
  setline(265);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe662 = gc_frame_new();
  if (strlit666 == NULL) {
    strlit666 = alloc_String("");
    gc_root(strlit666);
  }
// compilenode returning strlit666
  int op_slot_left_665 = gc_frame_newslot(strlit666);
// compilenode returning *var_homePath
  int op_slot_right_665 = gc_frame_newslot(*var_homePath);
  params[0] = *var_homePath;
  partcv[0] = 1;
  Object opresult668 = callmethod(strlit666, "++", 1, partcv, params);
// compilenode returning opresult668
  int op_slot_left_664 = gc_frame_newslot(opresult668);
  if (strlit669 == NULL) {
    strlit669 = alloc_String("/.local/share/grace/modules/");
    gc_root(strlit669);
  }
// compilenode returning strlit669
  int op_slot_right_664 = gc_frame_newslot(strlit669);
  params[0] = strlit669;
  partcv[0] = 1;
  Object opresult671 = callmethod(opresult668, "++", 1, partcv, params);
// compilenode returning opresult671
  int op_slot_left_663 = gc_frame_newslot(opresult671);
// compilenode returning *var_fileName
  int op_slot_right_663 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult673 = callmethod(opresult671, "++", 1, partcv, params);
// compilenode returning opresult673
  gc_frame_newslot(opresult673);
// compilenode returning *var_io
  params[0] = opresult673;
  partcv[0] = 1;
  Object call674 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe662);
// compilenode returning call674
  Object if661 = done;
  if (istrue(call674)) {
// Begin line 267
  setline(267);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 266
  setline(266);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe675 = gc_frame_new();
  int callframe676 = gc_frame_new();
  if (strlit680 == NULL) {
    strlit680 = alloc_String("");
    gc_root(strlit680);
  }
// compilenode returning strlit680
  int op_slot_left_679 = gc_frame_newslot(strlit680);
// compilenode returning *var_homePath
  int op_slot_right_679 = gc_frame_newslot(*var_homePath);
  params[0] = *var_homePath;
  partcv[0] = 1;
  Object opresult682 = callmethod(strlit680, "++", 1, partcv, params);
// compilenode returning opresult682
  int op_slot_left_678 = gc_frame_newslot(opresult682);
  if (strlit683 == NULL) {
    strlit683 = alloc_String("/.local/share/grace/modules/");
    gc_root(strlit683);
  }
// compilenode returning strlit683
  int op_slot_right_678 = gc_frame_newslot(strlit683);
  params[0] = strlit683;
  partcv[0] = 1;
  Object opresult685 = callmethod(opresult682, "++", 1, partcv, params);
// compilenode returning opresult685
  int op_slot_left_677 = gc_frame_newslot(opresult685);
// compilenode returning *var_fileName
  int op_slot_right_677 = gc_frame_newslot(*var_fileName);
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
  gc_frame_end(callframe676);
// compilenode returning call689
  partcv[0] = 0;
  Object call690 = callmethod(call689, "read",
    1, partcv, params);
  gc_frame_end(callframe675);
// compilenode returning call690
// compilenode returning call690
  return call690;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if661 = undefined;
  } else {
  }
// compilenode returning if661
// Begin line 271
  setline(271);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 268
  setline(268);
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
    strlit701 = alloc_String("/../lib/minigrace/modules/");
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
// Begin line 270
  setline(270);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 269
  setline(269);
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
    strlit717 = alloc_String("/../lib/minigrace/modules/");
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
// Begin line 274
  setline(274);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 271
  setline(271);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe726 = gc_frame_new();
  if (strlit730 == NULL) {
    strlit730 = alloc_String("");
    gc_root(strlit730);
  }
// compilenode returning strlit730
  int op_slot_left_729 = gc_frame_newslot(strlit730);
  int callframe731 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call732 = callmethod(*var_sys, "execPath",
    1, partcv, params);
  gc_frame_end(callframe731);
// compilenode returning call732
// compilenode returning call732
  int op_slot_right_729 = gc_frame_newslot(call732);
  params[0] = call732;
  partcv[0] = 1;
  Object opresult734 = callmethod(strlit730, "++", 1, partcv, params);
// compilenode returning opresult734
  int op_slot_left_728 = gc_frame_newslot(opresult734);
  if (strlit735 == NULL) {
    strlit735 = alloc_String("/");
    gc_root(strlit735);
  }
// compilenode returning strlit735
  int op_slot_right_728 = gc_frame_newslot(strlit735);
  params[0] = strlit735;
  partcv[0] = 1;
  Object opresult737 = callmethod(opresult734, "++", 1, partcv, params);
// compilenode returning opresult737
  int op_slot_left_727 = gc_frame_newslot(opresult737);
// compilenode returning *var_fileName
  int op_slot_right_727 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult739 = callmethod(opresult737, "++", 1, partcv, params);
// compilenode returning opresult739
  gc_frame_newslot(opresult739);
// compilenode returning *var_io
  params[0] = opresult739;
  partcv[0] = 1;
  Object call740 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe726);
// compilenode returning call740
  Object if725 = done;
  if (istrue(call740)) {
// Begin line 273
  setline(273);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 272
  setline(272);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe741 = gc_frame_new();
  int callframe742 = gc_frame_new();
  if (strlit746 == NULL) {
    strlit746 = alloc_String("");
    gc_root(strlit746);
  }
// compilenode returning strlit746
  int op_slot_left_745 = gc_frame_newslot(strlit746);
  int callframe747 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call748 = callmethod(*var_sys, "execPath",
    1, partcv, params);
  gc_frame_end(callframe747);
// compilenode returning call748
// compilenode returning call748
  int op_slot_right_745 = gc_frame_newslot(call748);
  params[0] = call748;
  partcv[0] = 1;
  Object opresult750 = callmethod(strlit746, "++", 1, partcv, params);
// compilenode returning opresult750
  int op_slot_left_744 = gc_frame_newslot(opresult750);
  if (strlit751 == NULL) {
    strlit751 = alloc_String("/");
    gc_root(strlit751);
  }
// compilenode returning strlit751
  int op_slot_right_744 = gc_frame_newslot(strlit751);
  params[0] = strlit751;
  partcv[0] = 1;
  Object opresult753 = callmethod(opresult750, "++", 1, partcv, params);
// compilenode returning opresult753
  int op_slot_left_743 = gc_frame_newslot(opresult753);
// compilenode returning *var_fileName
  int op_slot_right_743 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult755 = callmethod(opresult753, "++", 1, partcv, params);
// compilenode returning opresult755
  gc_frame_newslot(opresult755);
  if (strlit756 == NULL) {
    strlit756 = alloc_String("r");
    gc_root(strlit756);
  }
// compilenode returning strlit756
  gc_frame_newslot(strlit756);
// compilenode returning *var_io
  params[0] = opresult755;
  params[1] = strlit756;
  partcv[0] = 2;
  Object call757 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe742);
// compilenode returning call757
  partcv[0] = 0;
  Object call758 = callmethod(call757, "read",
    1, partcv, params);
  gc_frame_end(callframe741);
// compilenode returning call758
// compilenode returning call758
  return call758;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if725 = undefined;
  } else {
  }
// compilenode returning if725
// Begin line 277
  setline(277);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 274
  setline(274);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe760 = gc_frame_new();
  if (strlit765 == NULL) {
    strlit765 = alloc_String("");
    gc_root(strlit765);
  }
// compilenode returning strlit765
  int op_slot_left_764 = gc_frame_newslot(strlit765);
  int callframe766 = gc_frame_new();
  partcv[0] = 0;
  Object call767 = callmethodflags(self, "getBuildPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe766);
// compilenode returning call767
  int op_slot_right_764 = gc_frame_newslot(call767);
  params[0] = call767;
  partcv[0] = 1;
  Object opresult769 = callmethod(strlit765, "++", 1, partcv, params);
// compilenode returning opresult769
  int op_slot_left_763 = gc_frame_newslot(opresult769);
  if (strlit770 == NULL) {
    strlit770 = alloc_String("/");
    gc_root(strlit770);
  }
// compilenode returning strlit770
  int op_slot_right_763 = gc_frame_newslot(strlit770);
  params[0] = strlit770;
  partcv[0] = 1;
  Object opresult772 = callmethod(opresult769, "++", 1, partcv, params);
// compilenode returning opresult772
  int op_slot_left_762 = gc_frame_newslot(opresult772);
// compilenode returning *var_fileName
  int op_slot_right_762 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult774 = callmethod(opresult772, "++", 1, partcv, params);
// compilenode returning opresult774
  int op_slot_left_761 = gc_frame_newslot(opresult774);
  if (strlit775 == NULL) {
    strlit775 = alloc_String("");
    gc_root(strlit775);
  }
// compilenode returning strlit775
  int op_slot_right_761 = gc_frame_newslot(strlit775);
  params[0] = strlit775;
  partcv[0] = 1;
  Object opresult777 = callmethod(opresult774, "++", 1, partcv, params);
// compilenode returning opresult777
  gc_frame_newslot(opresult777);
// compilenode returning *var_io
  params[0] = opresult777;
  partcv[0] = 1;
  Object call778 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe760);
// compilenode returning call778
  Object if759 = done;
  if (istrue(call778)) {
// Begin line 276
  setline(276);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 275
  setline(275);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe779 = gc_frame_new();
  int callframe780 = gc_frame_new();
  if (strlit785 == NULL) {
    strlit785 = alloc_String("");
    gc_root(strlit785);
  }
// compilenode returning strlit785
  int op_slot_left_784 = gc_frame_newslot(strlit785);
  int callframe786 = gc_frame_new();
  partcv[0] = 0;
  Object call787 = callmethodflags(self, "getBuildPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe786);
// compilenode returning call787
  int op_slot_right_784 = gc_frame_newslot(call787);
  params[0] = call787;
  partcv[0] = 1;
  Object opresult789 = callmethod(strlit785, "++", 1, partcv, params);
// compilenode returning opresult789
  int op_slot_left_783 = gc_frame_newslot(opresult789);
  if (strlit790 == NULL) {
    strlit790 = alloc_String("/");
    gc_root(strlit790);
  }
// compilenode returning strlit790
  int op_slot_right_783 = gc_frame_newslot(strlit790);
  params[0] = strlit790;
  partcv[0] = 1;
  Object opresult792 = callmethod(opresult789, "++", 1, partcv, params);
// compilenode returning opresult792
  int op_slot_left_782 = gc_frame_newslot(opresult792);
// compilenode returning *var_fileName
  int op_slot_right_782 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult794 = callmethod(opresult792, "++", 1, partcv, params);
// compilenode returning opresult794
  int op_slot_left_781 = gc_frame_newslot(opresult794);
  if (strlit795 == NULL) {
    strlit795 = alloc_String("");
    gc_root(strlit795);
  }
// compilenode returning strlit795
  int op_slot_right_781 = gc_frame_newslot(strlit795);
  params[0] = strlit795;
  partcv[0] = 1;
  Object opresult797 = callmethod(opresult794, "++", 1, partcv, params);
// compilenode returning opresult797
  gc_frame_newslot(opresult797);
  if (strlit798 == NULL) {
    strlit798 = alloc_String("r");
    gc_root(strlit798);
  }
// compilenode returning strlit798
  gc_frame_newslot(strlit798);
// compilenode returning *var_io
  params[0] = opresult797;
  params[1] = strlit798;
  partcv[0] = 2;
  Object call799 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe780);
// compilenode returning call799
  partcv[0] = 0;
  Object call800 = callmethod(call799, "read",
    1, partcv, params);
  gc_frame_end(callframe779);
// compilenode returning call800
// compilenode returning call800
  return call800;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if759 = undefined;
  } else {
  }
// compilenode returning if759
// Begin line 281
  setline(281);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 277
  setline(277);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_bundlePath
  int op_slot_left_802 = gc_frame_newslot(*var_bundlePath);
  if (strlit803 == NULL) {
    strlit803 = alloc_String("");
    gc_root(strlit803);
  }
// compilenode returning strlit803
  int op_slot_right_802 = gc_frame_newslot(strlit803);
  params[0] = strlit803;
  partcv[0] = 1;
  Object opresult805 = callmethod(*var_bundlePath, "!=", 1, partcv, params);
// compilenode returning opresult805
  Object if801 = done;
  if (istrue(opresult805)) {
// Begin line 279
  setline(279);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 278
  setline(278);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe806 = gc_frame_new();
  int callframe807 = gc_frame_new();
  if (strlit812 == NULL) {
    strlit812 = alloc_String("");
    gc_root(strlit812);
  }
// compilenode returning strlit812
  int op_slot_left_811 = gc_frame_newslot(strlit812);
// compilenode returning *var_bundlePath
  int op_slot_right_811 = gc_frame_newslot(*var_bundlePath);
  params[0] = *var_bundlePath;
  partcv[0] = 1;
  Object opresult814 = callmethod(strlit812, "++", 1, partcv, params);
// compilenode returning opresult814
  int op_slot_left_810 = gc_frame_newslot(opresult814);
  if (strlit815 == NULL) {
    strlit815 = alloc_String("/");
    gc_root(strlit815);
  }
// compilenode returning strlit815
  int op_slot_right_810 = gc_frame_newslot(strlit815);
  params[0] = strlit815;
  partcv[0] = 1;
  Object opresult817 = callmethod(opresult814, "++", 1, partcv, params);
// compilenode returning opresult817
  int op_slot_left_809 = gc_frame_newslot(opresult817);
// compilenode returning *var_fileName
  int op_slot_right_809 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult819 = callmethod(opresult817, "++", 1, partcv, params);
// compilenode returning opresult819
  int op_slot_left_808 = gc_frame_newslot(opresult819);
  if (strlit820 == NULL) {
    strlit820 = alloc_String("");
    gc_root(strlit820);
  }
// compilenode returning strlit820
  int op_slot_right_808 = gc_frame_newslot(strlit820);
  params[0] = strlit820;
  partcv[0] = 1;
  Object opresult822 = callmethod(opresult819, "++", 1, partcv, params);
// compilenode returning opresult822
  gc_frame_newslot(opresult822);
  if (strlit823 == NULL) {
    strlit823 = alloc_String("r");
    gc_root(strlit823);
  }
// compilenode returning strlit823
  gc_frame_newslot(strlit823);
// compilenode returning *var_io
  params[0] = opresult822;
  params[1] = strlit823;
  partcv[0] = 2;
  Object call824 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe807);
// compilenode returning call824
  partcv[0] = 0;
  Object call825 = callmethod(call824, "read",
    1, partcv, params);
  gc_frame_end(callframe806);
// compilenode returning call825
// compilenode returning call825
  return call825;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if801 = undefined;
  } else {
  }
// compilenode returning if801
// Begin line 282
  setline(282);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 281
  setline(281);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool826 = alloc_Boolean(0);
// compilenode returning bool826
  return bool826;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_validateFile827(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 291
  setline(291);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 285
  setline(285);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe830 = gc_frame_new();
  int callframe831 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call832 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe831);
// compilenode returning call832
// compilenode returning call832
  partcv[0] = 0;
  Object call833 = callmethod(call832, "size",
    1, partcv, params);
  gc_frame_end(callframe830);
// compilenode returning call833
// compilenode returning call833
  int op_slot_left_829 = gc_frame_newslot(call833);
  Object num834 = alloc_Float64(1);
// compilenode returning num834
  int op_slot_right_829 = gc_frame_newslot(num834);
  params[0] = num834;
  partcv[0] = 1;
  Object opresult836 = callmethod(call833, ">", 1, partcv, params);
// compilenode returning opresult836
  Object if828 = done;
  if (istrue(opresult836)) {
// Begin line 290
  setline(290);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 286
  setline(286);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe839 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call840 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe839);
// compilenode returning call840
// compilenode returning call840
  Object num841 = alloc_Float64(1);
// compilenode returning num841
  params[0] = num841;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres842 = callmethod(call840, "[]", 1, partcv, params);
// compilenode returning idxres842
  int op_slot_left_838 = gc_frame_newslot(idxres842);
  if (strlit843 == NULL) {
    strlit843 = alloc_String("<");
    gc_root(strlit843);
  }
// compilenode returning strlit843
  int op_slot_right_838 = gc_frame_newslot(strlit843);
  params[0] = strlit843;
  partcv[0] = 1;
  Object opresult845 = callmethod(idxres842, "==", 1, partcv, params);
// compilenode returning opresult845
  Object if837 = done;
  if (istrue(opresult845)) {
// Begin line 288
  setline(288);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 287
  setline(287);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit846 == NULL) {
    strlit846 = alloc_String("Not a valid grace file");
    gc_root(strlit846);
  }
// compilenode returning strlit846
  params[0] = strlit846;
  Object call847 = gracelib_print(NULL, 1,  params);
// compilenode returning call847
// Begin line 289
  setline(289);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 288
  setline(288);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool848 = alloc_Boolean(0);
// compilenode returning bool848
  return bool848;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if837 = undefined;
  } else {
  }
// compilenode returning if837
    gc_frame_newslot(if837);
    if828 = if837;
  } else {
  }
// compilenode returning if828
// Begin line 292
  setline(292);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 291
  setline(291);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool849 = alloc_Boolean(1);
// compilenode returning bool849
  return bool849;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_write850(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 15, (flags>>24)&0xff);
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
// Begin line 296
  setline(296);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 295
  setline(295);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit851 == NULL) {
    strlit851 = alloc_String("");
    gc_root(strlit851);
  }
// compilenode returning strlit851
  *var_usrDir = strlit851;
  if (strlit851 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 302
  setline(302);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 296
  setline(296);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_global
  Object if852 = done;
  if (istrue(*var_global)) {
// Begin line 298
  setline(298);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 297
  setline(297);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit853 == NULL) {
    strlit853 = alloc_String("usr/lib/grace/modules/");
    gc_root(strlit853);
  }
// compilenode returning strlit853
  *var_usrDir = strlit853;
  if (strlit853 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if852 = done;
  } else {
// Begin line 301
  setline(301);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 300
  setline(300);
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
    strlit858 = alloc_String("HOME");
    gc_root(strlit858);
  }
// compilenode returning strlit858
  params[0] = strlit858;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres859 = callmethod(call857, "[]", 1, partcv, params);
// compilenode returning idxres859
  int op_slot_left_855 = gc_frame_newslot(idxres859);
  if (strlit860 == NULL) {
    strlit860 = alloc_String("/.local/lib/grace/modules/");
    gc_root(strlit860);
  }
// compilenode returning strlit860
  int op_slot_right_855 = gc_frame_newslot(strlit860);
  params[0] = strlit860;
  partcv[0] = 1;
  Object opresult862 = callmethod(idxres859, "++", 1, partcv, params);
// compilenode returning opresult862
  *var_usrDir = opresult862;
  if (opresult862 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if852 = done;
  }
// compilenode returning if852
// Begin line 303
  setline(303);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe864 = gc_frame_new();
// Begin line 302
  setline(302);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_usrDir
  int op_slot_left_865 = gc_frame_newslot(*var_usrDir);
  int callframe866 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call867 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe866);
// compilenode returning call867
// compilenode returning call867
  int op_slot_right_865 = gc_frame_newslot(call867);
  params[0] = call867;
  partcv[0] = 1;
  Object opresult869 = callmethod(*var_usrDir, "++", 1, partcv, params);
// compilenode returning opresult869
  gc_frame_newslot(opresult869);
  params[0] = opresult869;
  partcv[0] = 1;
  Object call870 = callmethodflags(self, "createDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe864);
// compilenode returning call870
  *var_fileDir = call870;
  if (call870 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 304
  setline(304);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe871 = gc_frame_new();
// Begin line 303
  setline(303);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_fileDir
  Object num873 = alloc_Float64(1);
// compilenode returning num873
  params[0] = num873;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres874 = callmethod(*var_fileDir, "[]", 1, partcv, params);
// compilenode returning idxres874
  int op_slot_left_872 = gc_frame_newslot(idxres874);
// compilenode returning *var_fileDir
  Object num875 = alloc_Float64(2);
// compilenode returning num875
  params[0] = num875;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres876 = callmethod(*var_fileDir, "[]", 1, partcv, params);
// compilenode returning idxres876
  int op_slot_right_872 = gc_frame_newslot(idxres876);
  params[0] = idxres876;
  partcv[0] = 1;
  Object opresult878 = callmethod(idxres874, "++", 1, partcv, params);
// compilenode returning opresult878
  gc_frame_newslot(opresult878);
  if (strlit879 == NULL) {
    strlit879 = alloc_String("w");
    gc_root(strlit879);
  }
// compilenode returning strlit879
  gc_frame_newslot(strlit879);
// compilenode returning *var_io
  params[0] = opresult878;
  params[1] = strlit879;
  partcv[0] = 2;
  Object call880 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe871);
// compilenode returning call880
  *var_toWrite = call880;
  if (call880 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 305
  setline(305);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe881 = gc_frame_new();
// Begin line 304
  setline(304);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe882 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call883 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe882);
// compilenode returning call883
// compilenode returning call883
  gc_frame_newslot(call883);
// compilenode returning *var_toWrite
  params[0] = call883;
  partcv[0] = 1;
  Object call884 = callmethod(*var_toWrite, "write",
    1, partcv, params);
  gc_frame_end(callframe881);
// compilenode returning call884
// Begin line 305
  setline(305);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe885 = gc_frame_new();
// compilenode returning *var_toWrite
  partcv[0] = 0;
  Object call886 = callmethod(*var_toWrite, "close",
    1, partcv, params);
  gc_frame_end(callframe885);
// compilenode returning call886
// compilenode returning call886
  gc_frame_end(frame);
  return call886;
}
Object meth_PackageManager__apply901(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 322
  setline(322);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 313
  setline(313);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe904 = gc_frame_new();
// compilenode returning *var_t
  gc_frame_newslot(*var_t);
// compilenode returning *var_paths
  params[0] = *var_t;
  partcv[0] = 1;
  Object call905 = callmethod(*var_paths, "at",
    1, partcv, params);
  gc_frame_end(callframe904);
// compilenode returning call905
  int op_slot_left_903 = gc_frame_newslot(call905);
  if (strlit906 == NULL) {
    strlit906 = alloc_String(":");
    gc_root(strlit906);
  }
// compilenode returning strlit906
  int op_slot_right_903 = gc_frame_newslot(strlit906);
  params[0] = strlit906;
  partcv[0] = 1;
  Object opresult908 = callmethod(call905, "==", 1, partcv, params);
// compilenode returning opresult908
  Object if902 = done;
  if (istrue(opresult908)) {
// Begin line 317
  setline(317);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 314
  setline(314);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe910 = gc_frame_new();
// compilenode returning *var_buildPath
  int op_slot_left_911 = gc_frame_newslot(*var_buildPath);
  if (strlit912 == NULL) {
    strlit912 = alloc_String("/minigrace");
    gc_root(strlit912);
  }
// compilenode returning strlit912
  int op_slot_right_911 = gc_frame_newslot(strlit912);
  params[0] = strlit912;
  partcv[0] = 1;
  Object opresult914 = callmethod(*var_buildPath, "++", 1, partcv, params);
// compilenode returning opresult914
  gc_frame_newslot(opresult914);
// compilenode returning *var_io
  params[0] = opresult914;
  partcv[0] = 1;
  Object call915 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe910);
// compilenode returning call915
  Object if909 = done;
  if (istrue(call915)) {
// Begin line 315
  setline(315);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  block_return(realself, *var_buildPath);
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if909 = undefined;
  } else {
  }
// compilenode returning if909
// Begin line 318
  setline(318);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 317
  setline(317);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit916 == NULL) {
    strlit916 = alloc_String("");
    gc_root(strlit916);
  }
// compilenode returning strlit916
  *var_buildPath = strlit916;
  if (strlit916 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if902 = done;
  } else {
// Begin line 321
  setline(321);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 320
  setline(320);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  int op_slot_left_918 = gc_frame_newslot(*var_buildPath);
// Begin line 321
  setline(321);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 320
  setline(320);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_paths
// compilenode returning *var_t
  params[0] = *var_t;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres919 = callmethod(*var_paths, "[]", 1, partcv, params);
// compilenode returning idxres919
  int op_slot_right_918 = gc_frame_newslot(idxres919);
  params[0] = idxres919;
  partcv[0] = 1;
  Object opresult921 = callmethod(*var_buildPath, "++", 1, partcv, params);
// compilenode returning opresult921
  *var_buildPath = opresult921;
  if (opresult921 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if902 = done;
  }
// compilenode returning if902
  gc_frame_end(frame);
  return if902;
}
Object meth_PackageManager_getBuildPath887(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 16, (flags>>24)&0xff);
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
// Begin line 311
  setline(311);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 310
  setline(310);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe888 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call889 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe888);
// compilenode returning call889
// compilenode returning call889
  if (strlit890 == NULL) {
    strlit890 = alloc_String("PATH");
    gc_root(strlit890);
  }
// compilenode returning strlit890
  params[0] = strlit890;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres891 = callmethod(call889, "[]", 1, partcv, params);
// compilenode returning idxres891
  *var_paths = idxres891;
  if (idxres891 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 312
  setline(312);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 311
  setline(311);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit892 == NULL) {
    strlit892 = alloc_String("");
    gc_root(strlit892);
  }
// compilenode returning strlit892
  *var_buildPath = strlit892;
  if (strlit892 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 323
  setline(323);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe893 = gc_frame_new();
// Begin line 312
  setline(312);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num895 = alloc_Float64(1);
// compilenode returning num895
  int op_slot_left_894 = gc_frame_newslot(num895);
  int callframe896 = gc_frame_new();
// compilenode returning *var_paths
  partcv[0] = 0;
  Object call897 = callmethod(*var_paths, "size",
    1, partcv, params);
  gc_frame_end(callframe896);
// compilenode returning call897
// compilenode returning call897
  int op_slot_right_894 = gc_frame_newslot(call897);
  params[0] = call897;
  partcv[0] = 1;
  Object opresult899 = callmethod(num895, "..", 1, partcv, params);
// compilenode returning opresult899
  gc_frame_newslot(opresult899);
// Begin line 323
  setline(323);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block900 = alloc_Block(NULL, NULL, "PackageManager", 323);
  gc_frame_newslot(block900);
  block_savedest(block900);
  Object closure901 = createclosure(4, "_apply");
setclosureframe(closure901, stackframe);
  addtoclosure(closure901, var_paths);
  addtoclosure(closure901, var_buildPath);
  addtoclosure(closure901, var_io);
  addtoclosure(closure901, selfslot);
  struct UserObject *uo901 = (struct UserObject*)block900;
  uo901->data[0] = (Object)closure901;
  Method *meth_meth_PackageManager__apply901 = addmethod2pos(block900, "_apply", &meth_PackageManager__apply901, 0);
int argcv_meth_PackageManager__apply901[] = {1};
meth_meth_PackageManager__apply901->type = alloc_MethodType(1, argcv_meth_PackageManager__apply901);
  meth_meth_PackageManager__apply901->definitionModule = modulename;
  meth_meth_PackageManager__apply901->definitionLine = 312;
// compilenode returning block900
  gc_frame_newslot(block900);
  params[0] = opresult899;
  params[1] = block900;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call924 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe893);
// compilenode returning call924
// Begin line 324
  setline(324);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 323
  setline(323);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  return *var_buildPath;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_compile925(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 17, (flags>>24)&0xff);
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
// Begin line 328
  setline(328);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 327
  setline(327);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit926 == NULL) {
    strlit926 = alloc_String("");
    gc_root(strlit926);
  }
// compilenode returning strlit926
  *var_usrDir = strlit926;
  if (strlit926 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 334
  setline(334);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 328
  setline(328);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_global
  Object if927 = done;
  if (istrue(*var_global)) {
// Begin line 330
  setline(330);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 329
  setline(329);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit928 == NULL) {
    strlit928 = alloc_String("usr/lib/grace/modules/");
    gc_root(strlit928);
  }
// compilenode returning strlit928
  *var_usrDir = strlit928;
  if (strlit928 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if927 = done;
  } else {
// Begin line 333
  setline(333);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 332
  setline(332);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe931 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call932 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe931);
// compilenode returning call932
// compilenode returning call932
  if (strlit933 == NULL) {
    strlit933 = alloc_String("HOME");
    gc_root(strlit933);
  }
// compilenode returning strlit933
  params[0] = strlit933;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres934 = callmethod(call932, "[]", 1, partcv, params);
// compilenode returning idxres934
  int op_slot_left_930 = gc_frame_newslot(idxres934);
  if (strlit935 == NULL) {
    strlit935 = alloc_String("/.local/lib/grace/modules/");
    gc_root(strlit935);
  }
// compilenode returning strlit935
  int op_slot_right_930 = gc_frame_newslot(strlit935);
  params[0] = strlit935;
  partcv[0] = 1;
  Object opresult937 = callmethod(idxres934, "++", 1, partcv, params);
// compilenode returning opresult937
  *var_usrDir = opresult937;
  if (opresult937 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if927 = done;
  }
// compilenode returning if927
// Begin line 335
  setline(335);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 334
  setline(334);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe939 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call940 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe939);
// compilenode returning call940
// compilenode returning call940
  if (strlit941 == NULL) {
    strlit941 = alloc_String("PATH");
    gc_root(strlit941);
  }
// compilenode returning strlit941
  params[0] = strlit941;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres942 = callmethod(call940, "[]", 1, partcv, params);
// compilenode returning idxres942
  *var_paths = idxres942;
  if (idxres942 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 336
  setline(336);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 335
  setline(335);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit944 == NULL) {
    strlit944 = alloc_String("PATHS = ");
    gc_root(strlit944);
  }
// compilenode returning strlit944
  int op_slot_left_943 = gc_frame_newslot(strlit944);
// compilenode returning *var_paths
  int op_slot_right_943 = gc_frame_newslot(*var_paths);
  params[0] = *var_paths;
  partcv[0] = 1;
  Object opresult946 = callmethod(strlit944, "++", 1, partcv, params);
// compilenode returning opresult946
  params[0] = opresult946;
  Object call947 = gracelib_print(NULL, 1,  params);
// compilenode returning call947
// Begin line 337
  setline(337);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe948 = gc_frame_new();
  partcv[0] = 0;
  Object call949 = callmethodflags(self, "getBuildPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe948);
// compilenode returning call949
  *var_buildPath = call949;
  if (call949 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 338
  setline(338);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe950 = gc_frame_new();
// Begin line 337
  setline(337);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  int op_slot_left_953 = gc_frame_newslot(*var_buildPath);
  if (strlit954 == NULL) {
    strlit954 = alloc_String("/minigrace ");
    gc_root(strlit954);
  }
// compilenode returning strlit954
  int op_slot_right_953 = gc_frame_newslot(strlit954);
  params[0] = strlit954;
  partcv[0] = 1;
  Object opresult956 = callmethod(*var_buildPath, "++", 1, partcv, params);
// compilenode returning opresult956
  int op_slot_left_952 = gc_frame_newslot(opresult956);
// compilenode returning *var_usrDir
  int op_slot_right_952 = gc_frame_newslot(*var_usrDir);
  params[0] = *var_usrDir;
  partcv[0] = 1;
  Object opresult958 = callmethod(opresult956, "++", 1, partcv, params);
// compilenode returning opresult958
  int op_slot_left_951 = gc_frame_newslot(opresult958);
  int callframe959 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call960 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe959);
// compilenode returning call960
// compilenode returning call960
  int op_slot_right_951 = gc_frame_newslot(call960);
  params[0] = call960;
  partcv[0] = 1;
  Object opresult962 = callmethod(opresult958, "++", 1, partcv, params);
// compilenode returning opresult962
  gc_frame_newslot(opresult962);
// compilenode returning *var_io
  params[0] = opresult962;
  partcv[0] = 1;
  Object call963 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe950);
// compilenode returning call963
// Begin line 340
  setline(340);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 338
  setline(338);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool964 = alloc_Boolean(1);
// compilenode returning bool964
  return bool964;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply981(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_982 = gc_frame_newslot(*var_count);
  int callframe983 = gc_frame_new();
// compilenode returning *var_address
  partcv[0] = 0;
  Object call984 = callmethod(*var_address, "size",
    1, partcv, params);
  gc_frame_end(callframe983);
// compilenode returning call984
// compilenode returning call984
  int op_slot_right_982 = gc_frame_newslot(call984);
  params[0] = call984;
  partcv[0] = 1;
  Object opresult986 = callmethod(*var_count, "<=", 1, partcv, params);
// compilenode returning opresult986
  gc_frame_end(frame);
  return opresult986;
}
Object meth_PackageManager__apply989(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 350
  setline(350);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 349
  setline(349);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_nextPath
  int op_slot_left_990 = gc_frame_newslot(*var_nextPath);
// Begin line 350
  setline(350);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 349
  setline(349);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_address
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres991 = callmethod(*var_address, "[]", 1, partcv, params);
// compilenode returning idxres991
  int op_slot_right_990 = gc_frame_newslot(idxres991);
  params[0] = idxres991;
  partcv[0] = 1;
  Object opresult993 = callmethod(*var_nextPath, "++", 1, partcv, params);
// compilenode returning opresult993
  *var_nextPath = opresult993;
  if (opresult993 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 354
  setline(354);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 350
  setline(350);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_address
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres997 = callmethod(*var_address, "[]", 1, partcv, params);
// compilenode returning idxres997
  int op_slot_left_996 = gc_frame_newslot(idxres997);
  if (strlit998 == NULL) {
    strlit998 = alloc_String("/");
    gc_root(strlit998);
  }
// compilenode returning strlit998
  int op_slot_right_996 = gc_frame_newslot(strlit998);
  params[0] = strlit998;
  partcv[0] = 1;
  Object opresult1000 = callmethod(idxres997, "==", 1, partcv, params);
// compilenode returning opresult1000
  Object if995 = done;
  if (istrue(opresult1000)) {
// Begin line 352
  setline(352);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 351
  setline(351);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toMake
  int op_slot_left_1001 = gc_frame_newslot(*var_toMake);
// compilenode returning *var_nextPath
  int op_slot_right_1001 = gc_frame_newslot(*var_nextPath);
  params[0] = *var_nextPath;
  partcv[0] = 1;
  Object opresult1003 = callmethod(*var_toMake, "++", 1, partcv, params);
// compilenode returning opresult1003
  *var_toMake = opresult1003;
  if (opresult1003 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 353
  setline(353);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 352
  setline(352);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1005 == NULL) {
    strlit1005 = alloc_String("");
    gc_root(strlit1005);
  }
// compilenode returning strlit1005
  *var_nextPath = strlit1005;
  if (strlit1005 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if995 = done;
  } else {
  }
// compilenode returning if995
// Begin line 355
  setline(355);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 354
  setline(354);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  int op_slot_left_1007 = gc_frame_newslot(*var_count);
  Object num1008 = alloc_Float64(1);
// compilenode returning num1008
  int op_slot_right_1007 = gc_frame_newslot(num1008);
  params[0] = num1008;
  partcv[0] = 1;
  Object sum1010 = callmethod(*var_count, "+", 1, partcv, params);
// compilenode returning sum1010
  *var_count = sum1010;
  if (sum1010 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_createDirectory965(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 18, (flags>>24)&0xff);
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
// Begin line 344
  setline(344);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 343
  setline(343);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array966 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array966
  *var_dir = array966;
  if (array966 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 345
  setline(345);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 344
  setline(344);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit967 == NULL) {
    strlit967 = alloc_String("");
    gc_root(strlit967);
  }
// compilenode returning strlit967
  *var_nextPath = strlit967;
  if (strlit967 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 346
  setline(346);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 345
  setline(345);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit968 == NULL) {
    strlit968 = alloc_String("");
    gc_root(strlit968);
  }
// compilenode returning strlit968
  *var_toMake = strlit968;
  if (strlit968 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 347
  setline(347);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 346
  setline(346);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num969 = alloc_Float64(1);
// compilenode returning num969
  *var_count = num969;
  if (num969 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 348
  setline(348);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 347
  setline(347);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit972 == NULL) {
    strlit972 = alloc_String("Directory being evaluated ");
    gc_root(strlit972);
  }
// compilenode returning strlit972
  int op_slot_left_971 = gc_frame_newslot(strlit972);
// compilenode returning *var_address
  int op_slot_right_971 = gc_frame_newslot(*var_address);
  params[0] = *var_address;
  partcv[0] = 1;
  Object opresult974 = callmethod(strlit972, "++", 1, partcv, params);
// compilenode returning opresult974
  int op_slot_left_970 = gc_frame_newslot(opresult974);
  if (strlit975 == NULL) {
    strlit975 = alloc_String("");
    gc_root(strlit975);
  }
// compilenode returning strlit975
  int op_slot_right_970 = gc_frame_newslot(strlit975);
  params[0] = strlit975;
  partcv[0] = 1;
  Object opresult977 = callmethod(opresult974, "++", 1, partcv, params);
// compilenode returning opresult977
  params[0] = opresult977;
  Object call978 = gracelib_print(NULL, 1,  params);
// compilenode returning call978
// Begin line 356
  setline(356);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe979 = gc_frame_new();
// Begin line 348
  setline(348);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block980 = alloc_Block(NULL, NULL, "PackageManager", 348);
  gc_frame_newslot(block980);
  block_savedest(block980);
  Object closure981 = createclosure(3, "_apply");
setclosureframe(closure981, stackframe);
  addtoclosure(closure981, var_count);
  addtoclosure(closure981, var_address);
  addtoclosure(closure981, selfslot);
  struct UserObject *uo981 = (struct UserObject*)block980;
  uo981->data[0] = (Object)closure981;
  Method *meth_meth_PackageManager__apply981 = addmethod2pos(block980, "_apply", &meth_PackageManager__apply981, 0);
int argcv_meth_PackageManager__apply981[] = {0};
meth_meth_PackageManager__apply981->type = alloc_MethodType(1, argcv_meth_PackageManager__apply981);
  meth_meth_PackageManager__apply981->definitionModule = modulename;
  meth_meth_PackageManager__apply981->definitionLine = 337;
// compilenode returning block980
  gc_frame_newslot(block980);
// Begin line 356
  setline(356);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block988 = alloc_Block(NULL, NULL, "PackageManager", 356);
  gc_frame_newslot(block988);
  block_savedest(block988);
  Object closure989 = createclosure(5, "_apply");
setclosureframe(closure989, stackframe);
  addtoclosure(closure989, var_nextPath);
  addtoclosure(closure989, var_address);
  addtoclosure(closure989, var_count);
  addtoclosure(closure989, var_toMake);
  addtoclosure(closure989, selfslot);
  struct UserObject *uo989 = (struct UserObject*)block988;
  uo989->data[0] = (Object)closure989;
  Method *meth_meth_PackageManager__apply989 = addmethod2pos(block988, "_apply", &meth_PackageManager__apply989, 0);
int argcv_meth_PackageManager__apply989[] = {0};
meth_meth_PackageManager__apply989->type = alloc_MethodType(1, argcv_meth_PackageManager__apply989);
  meth_meth_PackageManager__apply989->definitionModule = modulename;
  meth_meth_PackageManager__apply989->definitionLine = 348;
// compilenode returning block988
  gc_frame_newslot(block988);
  params[0] = block980;
  params[1] = block988;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1013 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe979);
// compilenode returning call1013
// Begin line 357
  setline(357);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 356
  setline(356);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1015 == NULL) {
    strlit1015 = alloc_String("Creating directory ");
    gc_root(strlit1015);
  }
// compilenode returning strlit1015
  int op_slot_left_1014 = gc_frame_newslot(strlit1015);
// compilenode returning *var_toMake
  int op_slot_right_1014 = gc_frame_newslot(*var_toMake);
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object opresult1017 = callmethod(strlit1015, "++", 1, partcv, params);
// compilenode returning opresult1017
  params[0] = opresult1017;
  Object call1018 = gracelib_print(NULL, 1,  params);
// compilenode returning call1018
// Begin line 361
  setline(361);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 357
  setline(357);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1020 = gc_frame_new();
  int callframe1021 = gc_frame_new();
// compilenode returning *var_toMake
  gc_frame_newslot(*var_toMake);
// compilenode returning *var_io
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object call1022 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe1021);
// compilenode returning call1022
  partcv[0] = 0;
  Object call1023 = callmethod(call1022, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1020);
// compilenode returning call1023
  Object if1019 = done;
  if (istrue(call1023)) {
// Begin line 359
  setline(359);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1024 = gc_frame_new();
// Begin line 358
  setline(358);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1026 == NULL) {
    strlit1026 = alloc_String("mkdir -p ");
    gc_root(strlit1026);
  }
// compilenode returning strlit1026
  int op_slot_left_1025 = gc_frame_newslot(strlit1026);
// compilenode returning *var_toMake
  int op_slot_right_1025 = gc_frame_newslot(*var_toMake);
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object opresult1028 = callmethod(strlit1026, "++", 1, partcv, params);
// compilenode returning opresult1028
  gc_frame_newslot(opresult1028);
// compilenode returning *var_io
  params[0] = opresult1028;
  partcv[0] = 1;
  Object call1029 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe1024);
// compilenode returning call1029
// Begin line 360
  setline(360);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 359
  setline(359);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1032 == NULL) {
    strlit1032 = alloc_String("made directory ");
    gc_root(strlit1032);
  }
// compilenode returning strlit1032
  int op_slot_left_1031 = gc_frame_newslot(strlit1032);
// compilenode returning *var_toMake
  int op_slot_right_1031 = gc_frame_newslot(*var_toMake);
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object opresult1034 = callmethod(strlit1032, "++", 1, partcv, params);
// compilenode returning opresult1034
  int op_slot_left_1030 = gc_frame_newslot(opresult1034);
  if (strlit1035 == NULL) {
    strlit1035 = alloc_String("");
    gc_root(strlit1035);
  }
// compilenode returning strlit1035
  int op_slot_right_1030 = gc_frame_newslot(strlit1035);
  params[0] = strlit1035;
  partcv[0] = 1;
  Object opresult1037 = callmethod(opresult1034, "++", 1, partcv, params);
// compilenode returning opresult1037
  params[0] = opresult1037;
  Object call1038 = gracelib_print(NULL, 1,  params);
// compilenode returning call1038
    gc_frame_newslot(call1038);
    if1019 = call1038;
  } else {
  }
// compilenode returning if1019
// Begin line 362
  setline(362);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1039 = gc_frame_new();
// Begin line 361
  setline(361);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toMake
  gc_frame_newslot(*var_toMake);
// compilenode returning *var_dir
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object call1040 = callmethod(*var_dir, "push",
    1, partcv, params);
  gc_frame_end(callframe1039);
// compilenode returning call1040
// Begin line 363
  setline(363);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1041 = gc_frame_new();
// Begin line 362
  setline(362);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_nextPath
  gc_frame_newslot(*var_nextPath);
// compilenode returning *var_dir
  params[0] = *var_nextPath;
  partcv[0] = 1;
  Object call1042 = callmethod(*var_dir, "push",
    1, partcv, params);
  gc_frame_end(callframe1041);
// compilenode returning call1042
// Begin line 364
  setline(364);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 363
  setline(363);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_dir
  return *var_dir;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply1049(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1050 = gc_frame_newslot(*var_curPos);
  int callframe1051 = gc_frame_new();
// compilenode returning *var_data
  partcv[0] = 0;
  Object call1052 = callmethod(*var_data, "size",
    1, partcv, params);
  gc_frame_end(callframe1051);
// compilenode returning call1052
// compilenode returning call1052
  int op_slot_right_1050 = gc_frame_newslot(call1052);
  params[0] = call1052;
  partcv[0] = 1;
  Object opresult1054 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1054
  gc_frame_end(frame);
  return opresult1054;
}
Object meth_PackageManager__apply1061(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1063 = gc_frame_newslot(*var_curPos);
  int callframe1064 = gc_frame_new();
// compilenode returning *var_data
  partcv[0] = 0;
  Object call1065 = callmethod(*var_data, "size",
    1, partcv, params);
  gc_frame_end(callframe1064);
// compilenode returning call1065
// compilenode returning call1065
  int op_slot_right_1063 = gc_frame_newslot(call1065);
  params[0] = call1065;
  partcv[0] = 1;
  Object opresult1067 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1067
  int op_slot_left_1062 = gc_frame_newslot(opresult1067);
// compilenode returning *var_data
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1069 = callmethod(*var_data, "[]", 1, partcv, params);
// compilenode returning idxres1069
  int op_slot_left_1068 = gc_frame_newslot(idxres1069);
  if (strlit1070 == NULL) {
    strlit1070 = alloc_String("""\x0a""");
    gc_root(strlit1070);
  }
// compilenode returning strlit1070
  int op_slot_right_1068 = gc_frame_newslot(strlit1070);
  params[0] = strlit1070;
  partcv[0] = 1;
  Object opresult1072 = callmethod(idxres1069, "!=", 1, partcv, params);
// compilenode returning opresult1072
  int op_slot_right_1062 = gc_frame_newslot(opresult1072);
  params[0] = opresult1072;
  partcv[0] = 1;
  Object opresult1074 = callmethod(opresult1067, "&&", 1, partcv, params);
// compilenode returning opresult1074
  gc_frame_end(frame);
  return opresult1074;
}
Object meth_PackageManager__apply1077(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 374
  setline(374);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 373
  setline(373);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1078 = gc_frame_newslot(*var_curPos);
  Object num1079 = alloc_Float64(1);
// compilenode returning num1079
  int op_slot_right_1078 = gc_frame_newslot(num1079);
  params[0] = num1079;
  partcv[0] = 1;
  Object sum1081 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1081
  *var_curPos = sum1081;
  if (sum1081 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1057(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 372
  setline(372);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 371
  setline(371);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 375
  setline(375);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1059 = gc_frame_new();
// Begin line 372
  setline(372);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1060 = alloc_Block(NULL, NULL, "PackageManager", 372);
  gc_frame_newslot(block1060);
  block_savedest(block1060);
  Object closure1061 = createclosure(3, "_apply");
setclosureframe(closure1061, stackframe);
  addtoclosure(closure1061, var_curPos);
  addtoclosure(closure1061, var_data);
  addtoclosure(closure1061, selfslot);
  struct UserObject *uo1061 = (struct UserObject*)block1060;
  uo1061->data[0] = (Object)closure1061;
  Method *meth_meth_PackageManager__apply1061 = addmethod2pos(block1060, "_apply", &meth_PackageManager__apply1061, 0);
int argcv_meth_PackageManager__apply1061[] = {0};
meth_meth_PackageManager__apply1061->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1061);
  meth_meth_PackageManager__apply1061->definitionModule = modulename;
  meth_meth_PackageManager__apply1061->definitionLine = 370;
// compilenode returning block1060
  gc_frame_newslot(block1060);
// Begin line 375
  setline(375);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1076 = alloc_Block(NULL, NULL, "PackageManager", 375);
  gc_frame_newslot(block1076);
  block_savedest(block1076);
  Object closure1077 = createclosure(2, "_apply");
setclosureframe(closure1077, stackframe);
  addtoclosure(closure1077, var_curPos);
  addtoclosure(closure1077, selfslot);
  struct UserObject *uo1077 = (struct UserObject*)block1076;
  uo1077->data[0] = (Object)closure1077;
  Method *meth_meth_PackageManager__apply1077 = addmethod2pos(block1076, "_apply", &meth_PackageManager__apply1077, 0);
int argcv_meth_PackageManager__apply1077[] = {0};
meth_meth_PackageManager__apply1077->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1077);
  meth_meth_PackageManager__apply1077->definitionModule = modulename;
  meth_meth_PackageManager__apply1077->definitionLine = 372;
// compilenode returning block1076
  gc_frame_newslot(block1076);
  params[0] = block1060;
  params[1] = block1076;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1084 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1059);
// compilenode returning call1084
// Begin line 376
  setline(376);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1085 = gc_frame_new();
// Begin line 375
  setline(375);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_startPos
  gc_frame_newslot(*var_startPos);
// compilenode returning *var_curPos
  int op_slot_left_1086 = gc_frame_newslot(*var_curPos);
  Object num1087 = alloc_Float64(1);
// compilenode returning num1087
  int op_slot_right_1086 = gc_frame_newslot(num1087);
  params[0] = num1087;
  partcv[0] = 1;
  Object diff1089 = callmethod(*var_curPos, "-", 1, partcv, params);
// compilenode returning diff1089
  gc_frame_newslot(diff1089);
// compilenode returning *var_data
  params[0] = *var_startPos;
  params[1] = diff1089;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1090 = callmethod(*var_data, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1085);
// compilenode returning call1090
  *var_line = call1090;
  if (call1090 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 379
  setline(379);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 376
  setline(376);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1092 = gc_frame_new();
  int callframe1093 = gc_frame_new();
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
  params[0] = *var_line;
  partcv[0] = 1;
  Object call1094 = callmethodflags(self, "processLine", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1093);
// compilenode returning call1094
  partcv[0] = 0;
  Object call1095 = callmethod(call1094, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1092);
// compilenode returning call1095
  Object if1091 = done;
  if (istrue(call1095)) {
// Begin line 378
  setline(378);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_done
  block_return(realself, *var_done);
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1091 = undefined;
  } else {
  }
// compilenode returning if1091
// Begin line 380
  setline(380);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 379
  setline(379);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1096 = gc_frame_newslot(*var_curPos);
  Object num1097 = alloc_Float64(1);
// compilenode returning num1097
  int op_slot_right_1096 = gc_frame_newslot(num1097);
  params[0] = num1097;
  partcv[0] = 1;
  Object sum1099 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1099
  *var_curPos = sum1099;
  if (sum1099 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_parseFile1043(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 19, (flags>>24)&0xff);
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
// Begin line 368
  setline(368);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 367
  setline(367);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1044 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1045 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe1044);
// compilenode returning call1045
// compilenode returning call1045
  *var_data = call1045;
  if (call1045 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 369
  setline(369);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 368
  setline(368);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1046 = alloc_Float64(1);
// compilenode returning num1046
  *var_curPos = num1046;
  if (num1046 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 370
  setline(370);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 369
  setline(369);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 381
  setline(381);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1047 = gc_frame_new();
// Begin line 370
  setline(370);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1048 = alloc_Block(NULL, NULL, "PackageManager", 370);
  gc_frame_newslot(block1048);
  block_savedest(block1048);
  Object closure1049 = createclosure(3, "_apply");
setclosureframe(closure1049, stackframe);
  addtoclosure(closure1049, var_curPos);
  addtoclosure(closure1049, var_data);
  addtoclosure(closure1049, selfslot);
  struct UserObject *uo1049 = (struct UserObject*)block1048;
  uo1049->data[0] = (Object)closure1049;
  Method *meth_meth_PackageManager__apply1049 = addmethod2pos(block1048, "_apply", &meth_PackageManager__apply1049, 0);
int argcv_meth_PackageManager__apply1049[] = {0};
meth_meth_PackageManager__apply1049->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1049);
  meth_meth_PackageManager__apply1049->definitionModule = modulename;
  meth_meth_PackageManager__apply1049->definitionLine = 367;
// compilenode returning block1048
  gc_frame_newslot(block1048);
// Begin line 381
  setline(381);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1056 = alloc_Block(NULL, NULL, "PackageManager", 381);
  gc_frame_newslot(block1056);
  block_savedest(block1056);
  Object closure1057 = createclosure(5, "_apply");
setclosureframe(closure1057, stackframe);
  addtoclosure(closure1057, var_curPos);
  addtoclosure(closure1057, var_startPos);
  addtoclosure(closure1057, var_data);
  addtoclosure(closure1057, var_done);
  addtoclosure(closure1057, selfslot);
  struct UserObject *uo1057 = (struct UserObject*)block1056;
  uo1057->data[0] = (Object)closure1057;
  Method *meth_meth_PackageManager__apply1057 = addmethod2pos(block1056, "_apply", &meth_PackageManager__apply1057, 0);
int argcv_meth_PackageManager__apply1057[] = {0};
meth_meth_PackageManager__apply1057->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1057);
  meth_meth_PackageManager__apply1057->definitionModule = modulename;
  meth_meth_PackageManager__apply1057->definitionLine = 370;
// compilenode returning block1056
  gc_frame_newslot(block1056);
  params[0] = block1048;
  params[1] = block1056;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1102 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1047);
// compilenode returning call1102
  gc_frame_end(frame);
  return call1102;
}
Object meth_PackageManager_processLine1103(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 399
  setline(399);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 384
  setline(384);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1106 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1107 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1106);
// compilenode returning call1107
// compilenode returning call1107
  int op_slot_left_1105 = gc_frame_newslot(call1107);
  Object num1108 = alloc_Float64(1);
// compilenode returning num1108
  int op_slot_right_1105 = gc_frame_newslot(num1108);
  params[0] = num1108;
  partcv[0] = 1;
  Object opresult1110 = callmethod(call1107, ">", 1, partcv, params);
// compilenode returning opresult1110
  Object if1104 = done;
  if (istrue(opresult1110)) {
// Begin line 398
  setline(398);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 385
  setline(385);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  Object num1113 = alloc_Float64(1);
// compilenode returning num1113
  params[0] = num1113;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1114 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1114
  int op_slot_left_1112 = gc_frame_newslot(idxres1114);
  if (strlit1115 == NULL) {
    strlit1115 = alloc_String("#");
    gc_root(strlit1115);
  }
// compilenode returning strlit1115
  int op_slot_right_1112 = gc_frame_newslot(strlit1115);
  params[0] = strlit1115;
  partcv[0] = 1;
  Object opresult1117 = callmethod(idxres1114, "==", 1, partcv, params);
// compilenode returning opresult1117
  Object if1111 = done;
  if (istrue(opresult1117)) {
// Begin line 387
  setline(387);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 386
  setline(386);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1118 = alloc_Boolean(1);
// compilenode returning bool1118
  return bool1118;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1111 = undefined;
  } else {
// Begin line 391
  setline(391);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 388
  setline(388);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1122 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1123 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1122);
// compilenode returning call1123
// compilenode returning call1123
  int op_slot_left_1121 = gc_frame_newslot(call1123);
  Object num1124 = alloc_Float64(2);
// compilenode returning num1124
  int op_slot_right_1121 = gc_frame_newslot(num1124);
  params[0] = num1124;
  partcv[0] = 1;
  Object opresult1126 = callmethod(call1123, ">", 1, partcv, params);
// compilenode returning opresult1126
  int op_slot_left_1120 = gc_frame_newslot(opresult1126);
  int callframe1128 = gc_frame_new();
  Object num1129 = alloc_Float64(1);
// compilenode returning num1129
  gc_frame_newslot(num1129);
  Object num1130 = alloc_Float64(2);
// compilenode returning num1130
  gc_frame_newslot(num1130);
// compilenode returning *var_line
  params[0] = num1129;
  params[1] = num1130;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1131 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1128);
// compilenode returning call1131
  int op_slot_left_1127 = gc_frame_newslot(call1131);
  if (strlit1132 == NULL) {
    strlit1132 = alloc_String("//");
    gc_root(strlit1132);
  }
// compilenode returning strlit1132
  int op_slot_right_1127 = gc_frame_newslot(strlit1132);
  params[0] = strlit1132;
  partcv[0] = 1;
  Object opresult1134 = callmethod(call1131, "==", 1, partcv, params);
// compilenode returning opresult1134
  int op_slot_right_1120 = gc_frame_newslot(opresult1134);
  params[0] = opresult1134;
  partcv[0] = 1;
  Object opresult1136 = callmethod(opresult1126, "&&", 1, partcv, params);
// compilenode returning opresult1136
  Object if1119 = done;
  if (istrue(opresult1136)) {
// Begin line 390
  setline(390);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 389
  setline(389);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1137 = alloc_Boolean(1);
// compilenode returning bool1137
  return bool1137;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1119 = undefined;
  } else {
// Begin line 395
  setline(395);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 391
  setline(391);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1141 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1142 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1141);
// compilenode returning call1142
// compilenode returning call1142
  int op_slot_left_1140 = gc_frame_newslot(call1142);
  Object num1143 = alloc_Float64(6);
// compilenode returning num1143
  int op_slot_right_1140 = gc_frame_newslot(num1143);
  params[0] = num1143;
  partcv[0] = 1;
  Object opresult1145 = callmethod(call1142, ">", 1, partcv, params);
// compilenode returning opresult1145
  int op_slot_left_1139 = gc_frame_newslot(opresult1145);
  int callframe1147 = gc_frame_new();
  Object num1148 = alloc_Float64(1);
// compilenode returning num1148
  gc_frame_newslot(num1148);
  Object num1149 = alloc_Float64(7);
// compilenode returning num1149
  gc_frame_newslot(num1149);
// compilenode returning *var_line
  params[0] = num1148;
  params[1] = num1149;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1150 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1147);
// compilenode returning call1150
  int op_slot_left_1146 = gc_frame_newslot(call1150);
  if (strlit1151 == NULL) {
    strlit1151 = alloc_String("import ");
    gc_root(strlit1151);
  }
// compilenode returning strlit1151
  int op_slot_right_1146 = gc_frame_newslot(strlit1151);
  params[0] = strlit1151;
  partcv[0] = 1;
  Object opresult1153 = callmethod(call1150, "==", 1, partcv, params);
// compilenode returning opresult1153
  int op_slot_right_1139 = gc_frame_newslot(opresult1153);
  params[0] = opresult1153;
  partcv[0] = 1;
  Object opresult1155 = callmethod(opresult1145, "&&", 1, partcv, params);
// compilenode returning opresult1155
  Object if1138 = done;
  if (istrue(opresult1155)) {
// Begin line 393
  setline(393);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1156 = gc_frame_new();
// Begin line 392
  setline(392);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1157 = gc_frame_new();
  Object num1158 = alloc_Float64(8);
// compilenode returning num1158
  gc_frame_newslot(num1158);
  int callframe1159 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1160 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1159);
// compilenode returning call1160
// compilenode returning call1160
  gc_frame_newslot(call1160);
// compilenode returning *var_line
  params[0] = num1158;
  params[1] = call1160;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1161 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1157);
// compilenode returning call1161
  gc_frame_newslot(call1161);
  params[0] = call1161;
  partcv[0] = 1;
  Object call1162 = callmethodflags(self, "parseImport", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1156);
// compilenode returning call1162
// Begin line 394
  setline(394);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 393
  setline(393);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1163 = alloc_Boolean(1);
// compilenode returning bool1163
  return bool1163;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1138 = undefined;
  } else {
// Begin line 398
  setline(398);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 395
  setline(395);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1167 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1168 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1167);
// compilenode returning call1168
// compilenode returning call1168
  int op_slot_left_1166 = gc_frame_newslot(call1168);
  Object num1169 = alloc_Float64(7);
// compilenode returning num1169
  int op_slot_right_1166 = gc_frame_newslot(num1169);
  params[0] = num1169;
  partcv[0] = 1;
  Object opresult1171 = callmethod(call1168, ">", 1, partcv, params);
// compilenode returning opresult1171
  int op_slot_left_1165 = gc_frame_newslot(opresult1171);
  int callframe1173 = gc_frame_new();
  Object num1174 = alloc_Float64(1);
// compilenode returning num1174
  gc_frame_newslot(num1174);
  Object num1175 = alloc_Float64(8);
// compilenode returning num1175
  gc_frame_newslot(num1175);
// compilenode returning *var_line
  params[0] = num1174;
  params[1] = num1175;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1176 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1173);
// compilenode returning call1176
  int op_slot_left_1172 = gc_frame_newslot(call1176);
  if (strlit1177 == NULL) {
    strlit1177 = alloc_String("dialect ");
    gc_root(strlit1177);
  }
// compilenode returning strlit1177
  int op_slot_right_1172 = gc_frame_newslot(strlit1177);
  params[0] = strlit1177;
  partcv[0] = 1;
  Object opresult1179 = callmethod(call1176, "==", 1, partcv, params);
// compilenode returning opresult1179
  int op_slot_right_1165 = gc_frame_newslot(opresult1179);
  params[0] = opresult1179;
  partcv[0] = 1;
  Object opresult1181 = callmethod(opresult1171, "&&", 1, partcv, params);
// compilenode returning opresult1181
  Object if1164 = done;
  if (istrue(opresult1181)) {
// Begin line 397
  setline(397);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 396
  setline(396);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1182 = alloc_Boolean(1);
// compilenode returning bool1182
  return bool1182;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1164 = undefined;
  } else {
  }
// compilenode returning if1164
    gc_frame_newslot(if1164);
    if1138 = if1164;
  }
// compilenode returning if1138
    gc_frame_newslot(if1138);
    if1119 = if1138;
  }
// compilenode returning if1119
    gc_frame_newslot(if1119);
    if1111 = if1119;
  }
// compilenode returning if1111
    gc_frame_newslot(if1111);
    if1104 = if1111;
  } else {
  }
// compilenode returning if1104
// Begin line 400
  setline(400);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 399
  setline(399);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1183 = alloc_Boolean(0);
// compilenode returning bool1183
  return bool1183;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply1198(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1200 = gc_frame_newslot(*var_curPos);
  int callframe1201 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1202 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1201);
// compilenode returning call1202
// compilenode returning call1202
  int op_slot_right_1200 = gc_frame_newslot(call1202);
  params[0] = call1202;
  partcv[0] = 1;
  Object opresult1204 = callmethod(*var_curPos, "<", 1, partcv, params);
// compilenode returning opresult1204
  int op_slot_left_1199 = gc_frame_newslot(opresult1204);
  int callframe1205 = gc_frame_new();
// compilenode returning *var_line
// compilenode returning *var_curPos
  int op_slot_left_1208 = gc_frame_newslot(*var_curPos);
  Object num1209 = alloc_Float64(1);
// compilenode returning num1209
  int op_slot_right_1208 = gc_frame_newslot(num1209);
  params[0] = num1209;
  partcv[0] = 1;
  Object sum1211 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1211
  params[0] = sum1211;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1212 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1212
  int op_slot_left_1207 = gc_frame_newslot(idxres1212);
  if (strlit1213 == NULL) {
    strlit1213 = alloc_String("""\x22""");
    gc_root(strlit1213);
  }
// compilenode returning strlit1213
  int op_slot_right_1207 = gc_frame_newslot(strlit1213);
  params[0] = strlit1213;
  partcv[0] = 1;
  Object opresult1215 = callmethod(idxres1212, "==", 1, partcv, params);
// compilenode returning opresult1215
  int op_slot_left_1206 = gc_frame_newslot(opresult1215);
// compilenode returning *var_line
// compilenode returning *var_curPos
  int op_slot_left_1217 = gc_frame_newslot(*var_curPos);
  Object num1218 = alloc_Float64(1);
// compilenode returning num1218
  int op_slot_right_1217 = gc_frame_newslot(num1218);
  params[0] = num1218;
  partcv[0] = 1;
  Object sum1220 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1220
  params[0] = sum1220;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1221 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1221
  int op_slot_left_1216 = gc_frame_newslot(idxres1221);
  if (strlit1222 == NULL) {
    strlit1222 = alloc_String(" ");
    gc_root(strlit1222);
  }
// compilenode returning strlit1222
  int op_slot_right_1216 = gc_frame_newslot(strlit1222);
  params[0] = strlit1222;
  partcv[0] = 1;
  Object opresult1224 = callmethod(idxres1221, "==", 1, partcv, params);
// compilenode returning opresult1224
  int op_slot_right_1206 = gc_frame_newslot(opresult1224);
  params[0] = opresult1224;
  partcv[0] = 1;
  Object opresult1226 = callmethod(opresult1215, "||", 1, partcv, params);
// compilenode returning opresult1226
  partcv[0] = 0;
  Object call1227 = callmethod(opresult1226, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1205);
// compilenode returning call1227
  int op_slot_right_1199 = gc_frame_newslot(call1227);
  params[0] = call1227;
  partcv[0] = 1;
  Object opresult1229 = callmethod(opresult1204, "&&", 1, partcv, params);
// compilenode returning opresult1229
  gc_frame_end(frame);
  return opresult1229;
}
Object meth_PackageManager__apply1232(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 411
  setline(411);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 410
  setline(410);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1233 = gc_frame_newslot(*var_curPos);
  Object num1234 = alloc_Float64(1);
// compilenode returning num1234
  int op_slot_right_1233 = gc_frame_newslot(num1234);
  params[0] = num1234;
  partcv[0] = 1;
  Object sum1236 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1236
  *var_curPos = sum1236;
  if (sum1236 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_parseImport1184(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 21, (flags>>24)&0xff);
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
// Begin line 404
  setline(404);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 403
  setline(403);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1185 = alloc_Float64(1);
// compilenode returning num1185
  *var_curPos = num1185;
  if (num1185 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 405
  setline(405);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 404
  setline(404);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 406
  setline(406);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 405
  setline(405);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1186 == NULL) {
    strlit1186 = alloc_String("");
    gc_root(strlit1186);
  }
// compilenode returning strlit1186
  *var_nextImport = strlit1186;
  if (strlit1186 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 407
  setline(407);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1187 = gc_frame_new();
// Begin line 406
  setline(406);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// compilenode returning *var_curPos
  gc_frame_newslot(*var_curPos);
  params[0] = *var_line;
  params[1] = *var_curPos;
  partcv[0] = 2;
  Object call1188 = callmethodflags(self, "skipWhiteSpace", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1187);
// compilenode returning call1188
  *var_curPos = call1188;
  if (call1188 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 408
  setline(408);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 407
  setline(407);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1190 = gc_frame_newslot(*var_curPos);
  Object num1191 = alloc_Float64(1);
// compilenode returning num1191
  int op_slot_right_1190 = gc_frame_newslot(num1191);
  params[0] = num1191;
  partcv[0] = 1;
  Object sum1193 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1193
  *var_curPos = sum1193;
  if (sum1193 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 409
  setline(409);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 408
  setline(408);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 412
  setline(412);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1196 = gc_frame_new();
// Begin line 409
  setline(409);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1197 = alloc_Block(NULL, NULL, "PackageManager", 409);
  gc_frame_newslot(block1197);
  block_savedest(block1197);
  Object closure1198 = createclosure(3, "_apply");
setclosureframe(closure1198, stackframe);
  addtoclosure(closure1198, var_curPos);
  addtoclosure(closure1198, var_line);
  addtoclosure(closure1198, selfslot);
  struct UserObject *uo1198 = (struct UserObject*)block1197;
  uo1198->data[0] = (Object)closure1198;
  Method *meth_meth_PackageManager__apply1198 = addmethod2pos(block1197, "_apply", &meth_PackageManager__apply1198, 0);
int argcv_meth_PackageManager__apply1198[] = {0};
meth_meth_PackageManager__apply1198->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1198);
  meth_meth_PackageManager__apply1198->definitionModule = modulename;
  meth_meth_PackageManager__apply1198->definitionLine = 395;
// compilenode returning block1197
  gc_frame_newslot(block1197);
// Begin line 412
  setline(412);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1231 = alloc_Block(NULL, NULL, "PackageManager", 412);
  gc_frame_newslot(block1231);
  block_savedest(block1231);
  Object closure1232 = createclosure(2, "_apply");
setclosureframe(closure1232, stackframe);
  addtoclosure(closure1232, var_curPos);
  addtoclosure(closure1232, selfslot);
  struct UserObject *uo1232 = (struct UserObject*)block1231;
  uo1232->data[0] = (Object)closure1232;
  Method *meth_meth_PackageManager__apply1232 = addmethod2pos(block1231, "_apply", &meth_PackageManager__apply1232, 0);
int argcv_meth_PackageManager__apply1232[] = {0};
meth_meth_PackageManager__apply1232->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1232);
  meth_meth_PackageManager__apply1232->definitionModule = modulename;
  meth_meth_PackageManager__apply1232->definitionLine = 409;
// compilenode returning block1231
  gc_frame_newslot(block1231);
  params[0] = block1197;
  params[1] = block1231;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1239 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1196);
// compilenode returning call1239
// Begin line 413
  setline(413);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1240 = gc_frame_new();
// Begin line 412
  setline(412);
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
  Object call1241 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1240);
// compilenode returning call1241
  *var_nextImport = call1241;
  if (call1241 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 417
  setline(417);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 413
  setline(413);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1244 = gc_frame_new();
  int callframe1245 = gc_frame_new();
// compilenode returning *var_nextImport
  gc_frame_newslot(*var_nextImport);
// compilenode returning *var_imported
  params[0] = *var_nextImport;
  partcv[0] = 1;
  Object call1246 = callmethod(*var_imported, "contains",
    1, partcv, params);
  gc_frame_end(callframe1245);
// compilenode returning call1246
  partcv[0] = 0;
  Object call1247 = callmethod(call1246, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1244);
// compilenode returning call1247
  Object if1243 = done;
  if (istrue(call1247)) {
// Begin line 415
  setline(415);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 414
  setline(414);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1249 == NULL) {
    strlit1249 = alloc_String("next import = ");
    gc_root(strlit1249);
  }
// compilenode returning strlit1249
  int op_slot_left_1248 = gc_frame_newslot(strlit1249);
// compilenode returning *var_nextImport
  int op_slot_right_1248 = gc_frame_newslot(*var_nextImport);
  params[0] = *var_nextImport;
  partcv[0] = 1;
  Object opresult1251 = callmethod(strlit1249, "++", 1, partcv, params);
// compilenode returning opresult1251
  params[0] = opresult1251;
  Object call1252 = gracelib_print(NULL, 1,  params);
// compilenode returning call1252
// Begin line 416
  setline(416);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1253 = gc_frame_new();
// Begin line 415
  setline(415);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_nextImport
  gc_frame_newslot(*var_nextImport);
// compilenode returning *var_toProcess
  params[0] = *var_nextImport;
  partcv[0] = 1;
  Object call1254 = callmethod(*var_toProcess, "push",
    1, partcv, params);
  gc_frame_end(callframe1253);
// compilenode returning call1254
    gc_frame_newslot(call1254);
    if1243 = call1254;
  } else {
  }
// compilenode returning if1243
  gc_frame_end(frame);
  return if1243;
}
Object meth_PackageManager__apply1258(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1260 = gc_frame_newslot(*var_count2);
  int callframe1261 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1262 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1261);
// compilenode returning call1262
// compilenode returning call1262
  int op_slot_right_1260 = gc_frame_newslot(call1262);
  params[0] = call1262;
  partcv[0] = 1;
  Object opresult1264 = callmethod(*var_count2, "<=", 1, partcv, params);
// compilenode returning opresult1264
  int op_slot_left_1259 = gc_frame_newslot(opresult1264);
// compilenode returning *var_line
// compilenode returning *var_count2
  params[0] = *var_count2;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1266 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1266
  int op_slot_left_1265 = gc_frame_newslot(idxres1266);
  if (strlit1267 == NULL) {
    strlit1267 = alloc_String("Could not retrieve ");
    gc_root(strlit1267);
  }
// compilenode returning strlit1267
  int op_slot_right_1265 = gc_frame_newslot(strlit1267);
  params[0] = strlit1267;
  partcv[0] = 1;
  Object opresult1269 = callmethod(idxres1266, "==", 1, partcv, params);
// compilenode returning opresult1269
  int op_slot_right_1259 = gc_frame_newslot(opresult1269);
  params[0] = opresult1269;
  partcv[0] = 1;
  Object opresult1271 = callmethod(opresult1264, "&&", 1, partcv, params);
// compilenode returning opresult1271
  gc_frame_end(frame);
  return opresult1271;
}
Object meth_PackageManager__apply1274(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 423
  setline(423);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 422
  setline(422);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count2
  int op_slot_left_1275 = gc_frame_newslot(*var_count2);
  Object num1276 = alloc_Float64(1);
// compilenode returning num1276
  int op_slot_right_1275 = gc_frame_newslot(num1276);
  params[0] = num1276;
  partcv[0] = 1;
  Object sum1278 = callmethod(*var_count2, "+", 1, partcv, params);
// compilenode returning sum1278
  *var_count2 = sum1278;
  if (sum1278 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_skipWhiteSpace1255(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 421
  setline(421);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 420
  setline(420);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  *var_count2 = *var_count;
  if (*var_count == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 424
  setline(424);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1256 = gc_frame_new();
// Begin line 421
  setline(421);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1257 = alloc_Block(NULL, NULL, "PackageManager", 421);
  gc_frame_newslot(block1257);
  block_savedest(block1257);
  Object closure1258 = createclosure(3, "_apply");
setclosureframe(closure1258, stackframe);
  addtoclosure(closure1258, var_count2);
  addtoclosure(closure1258, var_line);
  addtoclosure(closure1258, selfslot);
  struct UserObject *uo1258 = (struct UserObject*)block1257;
  uo1258->data[0] = (Object)closure1258;
  Method *meth_meth_PackageManager__apply1258 = addmethod2pos(block1257, "_apply", &meth_PackageManager__apply1258, 0);
int argcv_meth_PackageManager__apply1258[] = {0};
meth_meth_PackageManager__apply1258->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1258);
  meth_meth_PackageManager__apply1258->definitionModule = modulename;
  meth_meth_PackageManager__apply1258->definitionLine = 409;
// compilenode returning block1257
  gc_frame_newslot(block1257);
// Begin line 424
  setline(424);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1273 = alloc_Block(NULL, NULL, "PackageManager", 424);
  gc_frame_newslot(block1273);
  block_savedest(block1273);
  Object closure1274 = createclosure(2, "_apply");
setclosureframe(closure1274, stackframe);
  addtoclosure(closure1274, var_count2);
  addtoclosure(closure1274, selfslot);
  struct UserObject *uo1274 = (struct UserObject*)block1273;
  uo1274->data[0] = (Object)closure1274;
  Method *meth_meth_PackageManager__apply1274 = addmethod2pos(block1273, "_apply", &meth_PackageManager__apply1274, 0);
int argcv_meth_PackageManager__apply1274[] = {0};
meth_meth_PackageManager__apply1274->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1274);
  meth_meth_PackageManager__apply1274->definitionModule = modulename;
  meth_meth_PackageManager__apply1274->definitionLine = 421;
// compilenode returning block1273
  gc_frame_newslot(block1273);
  params[0] = block1257;
  params[1] = block1273;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1281 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1256);
// compilenode returning call1281
// Begin line 424
  setline(424);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count2
  gc_frame_end(frame);
  return *var_count2;
}
Object meth_PackageManager_displayHelp1282(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 429
  setline(429);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 428
  setline(428);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1283 == NULL) {
    strlit1283 = alloc_String("Available options are:");
    gc_root(strlit1283);
  }
// compilenode returning strlit1283
  params[0] = strlit1283;
  Object call1284 = gracelib_print(NULL, 1,  params);
// compilenode returning call1284
// Begin line 430
  setline(430);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 429
  setline(429);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1285 == NULL) {
    strlit1285 = alloc_String("-get [url] to fetch a package located at a given url");
    gc_root(strlit1285);
  }
// compilenode returning strlit1285
  params[0] = strlit1285;
  Object call1286 = gracelib_print(NULL, 1,  params);
// compilenode returning call1286
// Begin line 431
  setline(431);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 430
  setline(430);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1287 == NULL) {
    strlit1287 = alloc_String("-list to display currenly installed packages");
    gc_root(strlit1287);
  }
// compilenode returning strlit1287
  params[0] = strlit1287;
  Object call1288 = gracelib_print(NULL, 1,  params);
// compilenode returning call1288
  gc_frame_end(frame);
  return call1288;
}
Object meth_PackageManager_printMessage1289(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 24, (flags>>24)&0xff);
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
// Begin line 437
  setline(437);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 434
  setline(434);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_verbose
  Object if1290 = done;
  if (istrue(*var_verbose)) {
// Begin line 436
  setline(436);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 435
  setline(435);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_message
  params[0] = *var_message;
  Object call1291 = gracelib_print(NULL, 1,  params);
// compilenode returning call1291
    gc_frame_newslot(call1291);
    if1290 = call1291;
  } else {
  }
// compilenode returning if1290
  gc_frame_end(frame);
  return if1290;
}
Object meth_PackageManager__apply1417(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int callframe1419 = gc_frame_new();
// compilenode returning *var_imported
  partcv[0] = 0;
  Object call1420 = callmethod(*var_imported, "size",
    1, partcv, params);
  gc_frame_end(callframe1419);
// compilenode returning call1420
// compilenode returning call1420
  int op_slot_left_1418 = gc_frame_newslot(call1420);
  Object num1421 = alloc_Float64(0);
// compilenode returning num1421
  int op_slot_right_1418 = gc_frame_newslot(num1421);
  params[0] = num1421;
  partcv[0] = 1;
  Object opresult1423 = callmethod(call1420, ">", 1, partcv, params);
// compilenode returning opresult1423
  gc_frame_end(frame);
  return opresult1423;
}
Object meth_PackageManager__apply1426(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  Object *var_newDir = getfromclosure(closure, 1);
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
// Begin line 458
  setline(458);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 457
  setline(457);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1427 = gc_frame_new();
// compilenode returning *var_imported
  partcv[0] = 0;
  Object call1428 = callmethod(*var_imported, "pop",
    1, partcv, params);
  gc_frame_end(callframe1427);
// compilenode returning call1428
// compilenode returning call1428
  *var_curImport = call1428;
  if (call1428 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 459
  setline(459);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1429 = gc_frame_new();
// Begin line 458
  setline(458);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1434 == NULL) {
    strlit1434 = alloc_String("");
    gc_root(strlit1434);
  }
// compilenode returning strlit1434
  int op_slot_left_1433 = gc_frame_newslot(strlit1434);
// compilenode returning *var_newDir
  int op_slot_right_1433 = gc_frame_newslot(*var_newDir);
  params[0] = *var_newDir;
  partcv[0] = 1;
  Object opresult1436 = callmethod(strlit1434, "++", 1, partcv, params);
// compilenode returning opresult1436
  int op_slot_left_1432 = gc_frame_newslot(opresult1436);
  if (strlit1437 == NULL) {
    strlit1437 = alloc_String("/");
    gc_root(strlit1437);
  }
// compilenode returning strlit1437
  int op_slot_right_1432 = gc_frame_newslot(strlit1437);
  params[0] = strlit1437;
  partcv[0] = 1;
  Object opresult1439 = callmethod(opresult1436, "++", 1, partcv, params);
// compilenode returning opresult1439
  int op_slot_left_1431 = gc_frame_newslot(opresult1439);
  int callframe1440 = gc_frame_new();
  int callframe1441 = gc_frame_new();
// compilenode returning *var_curImport
  partcv[0] = 0;
  Object call1442 = callmethod(*var_curImport, "address",
    1, partcv, params);
  gc_frame_end(callframe1441);
// compilenode returning call1442
// compilenode returning call1442
  gc_frame_newslot(call1442);
  params[0] = call1442;
  partcv[0] = 1;
  Object call1443 = callmethodflags(self, "removeContainingDir", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1440);
// compilenode returning call1443
  int op_slot_right_1431 = gc_frame_newslot(call1443);
  params[0] = call1443;
  partcv[0] = 1;
  Object opresult1445 = callmethod(opresult1439, "++", 1, partcv, params);
// compilenode returning opresult1445
  int op_slot_left_1430 = gc_frame_newslot(opresult1445);
  if (strlit1446 == NULL) {
    strlit1446 = alloc_String("");
    gc_root(strlit1446);
  }
// compilenode returning strlit1446
  int op_slot_right_1430 = gc_frame_newslot(strlit1446);
  params[0] = strlit1446;
  partcv[0] = 1;
  Object opresult1448 = callmethod(opresult1445, "++", 1, partcv, params);
// compilenode returning opresult1448
  gc_frame_newslot(opresult1448);
  if (strlit1449 == NULL) {
    strlit1449 = alloc_String("w");
    gc_root(strlit1449);
  }
// compilenode returning strlit1449
  gc_frame_newslot(strlit1449);
// compilenode returning *var_io
  params[0] = opresult1448;
  params[1] = strlit1449;
  partcv[0] = 2;
  Object call1450 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1429);
// compilenode returning call1450
  *var_toWrite = call1450;
  if (call1450 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 460
  setline(460);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1452 = gc_frame_new();
// Begin line 459
  setline(459);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1453 = gc_frame_new();
// compilenode returning *var_curImport
  partcv[0] = 0;
  Object call1454 = callmethod(*var_curImport, "data",
    1, partcv, params);
  gc_frame_end(callframe1453);
// compilenode returning call1454
// compilenode returning call1454
  gc_frame_newslot(call1454);
// compilenode returning *var_toWrite
  params[0] = call1454;
  partcv[0] = 1;
  Object call1455 = callmethod(*var_toWrite, "write",
    1, partcv, params);
  gc_frame_end(callframe1452);
// compilenode returning call1455
// Begin line 460
  setline(460);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1456 = gc_frame_new();
// compilenode returning *var_toWrite
  partcv[0] = 0;
  Object call1457 = callmethod(*var_toWrite, "close",
    1, partcv, params);
  gc_frame_end(callframe1456);
// compilenode returning call1457
// compilenode returning call1457
  gc_frame_end(frame);
  return call1457;
}
Object meth_PackageManager__apply1329(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(5, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  Object *var_toBundle = getfromclosure(closure, 0);
  Object *var_io = getfromclosure(closure, 1);
  Object *var_newDir = getfromclosure(closure, 2);
  Object *var_imported = getfromclosure(closure, 3);
  Object self = *(getfromclosure(closure, 4));
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
  Object *var_openData = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "openData");
  Object *var_toWrite = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "toWrite");
// Begin line 464
  setline(464);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 446
  setline(446);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1332 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1333 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1332);
// compilenode returning call1333
// compilenode returning call1333
  int op_slot_left_1331 = gc_frame_newslot(call1333);
  Object num1334 = alloc_Float64(4);
// compilenode returning num1334
  int op_slot_right_1331 = gc_frame_newslot(num1334);
  params[0] = num1334;
  partcv[0] = 1;
  Object opresult1336 = callmethod(call1333, ">", 1, partcv, params);
// compilenode returning opresult1336
  Object if1330 = done;
  if (istrue(opresult1336)) {
// Begin line 463
  setline(463);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 447
  setline(447);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1339 = gc_frame_new();
  int callframe1341 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1342 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1341);
// compilenode returning call1342
// compilenode returning call1342
  int op_slot_left_1340 = gc_frame_newslot(call1342);
  Object num1343 = alloc_Float64(5);
// compilenode returning num1343
  int op_slot_right_1340 = gc_frame_newslot(num1343);
  params[0] = num1343;
  partcv[0] = 1;
  Object diff1345 = callmethod(call1342, "-", 1, partcv, params);
// compilenode returning diff1345
  gc_frame_newslot(diff1345);
  int callframe1346 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1347 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1346);
// compilenode returning call1347
// compilenode returning call1347
  gc_frame_newslot(call1347);
// compilenode returning *var_file
  params[0] = diff1345;
  params[1] = call1347;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1348 = callmethod(*var_file, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1339);
// compilenode returning call1348
  int op_slot_left_1338 = gc_frame_newslot(call1348);
  if (strlit1349 == NULL) {
    strlit1349 = alloc_String(".grace");
    gc_root(strlit1349);
  }
// compilenode returning strlit1349
  int op_slot_right_1338 = gc_frame_newslot(strlit1349);
  params[0] = strlit1349;
  partcv[0] = 1;
  Object opresult1351 = callmethod(call1348, "==", 1, partcv, params);
// compilenode returning opresult1351
  Object if1337 = done;
  if (istrue(opresult1351)) {
// Begin line 449
  setline(449);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 448
  setline(448);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1354 == NULL) {
    strlit1354 = alloc_String("FILE = ");
    gc_root(strlit1354);
  }
// compilenode returning strlit1354
  int op_slot_left_1353 = gc_frame_newslot(strlit1354);
// compilenode returning *var_file
  int op_slot_right_1353 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1356 = callmethod(strlit1354, "++", 1, partcv, params);
// compilenode returning opresult1356
  int op_slot_left_1352 = gc_frame_newslot(opresult1356);
  if (strlit1357 == NULL) {
    strlit1357 = alloc_String("");
    gc_root(strlit1357);
  }
// compilenode returning strlit1357
  int op_slot_right_1352 = gc_frame_newslot(strlit1357);
  params[0] = strlit1357;
  partcv[0] = 1;
  Object opresult1359 = callmethod(opresult1356, "++", 1, partcv, params);
// compilenode returning opresult1359
  params[0] = opresult1359;
  Object call1360 = gracelib_print(NULL, 1,  params);
// compilenode returning call1360
// Begin line 450
  setline(450);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1361 = gc_frame_new();
// Begin line 449
  setline(449);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1366 == NULL) {
    strlit1366 = alloc_String("");
    gc_root(strlit1366);
  }
// compilenode returning strlit1366
  int op_slot_left_1365 = gc_frame_newslot(strlit1366);
// compilenode returning *var_toBundle
  int op_slot_right_1365 = gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object opresult1368 = callmethod(strlit1366, "++", 1, partcv, params);
// compilenode returning opresult1368
  int op_slot_left_1364 = gc_frame_newslot(opresult1368);
  if (strlit1369 == NULL) {
    strlit1369 = alloc_String("");
    gc_root(strlit1369);
  }
// compilenode returning strlit1369
  int op_slot_right_1364 = gc_frame_newslot(strlit1369);
  params[0] = strlit1369;
  partcv[0] = 1;
  Object opresult1371 = callmethod(opresult1368, "++", 1, partcv, params);
// compilenode returning opresult1371
  int op_slot_left_1363 = gc_frame_newslot(opresult1371);
// compilenode returning *var_file
  int op_slot_right_1363 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1373 = callmethod(opresult1371, "++", 1, partcv, params);
// compilenode returning opresult1373
  int op_slot_left_1362 = gc_frame_newslot(opresult1373);
  if (strlit1374 == NULL) {
    strlit1374 = alloc_String("");
    gc_root(strlit1374);
  }
// compilenode returning strlit1374
  int op_slot_right_1362 = gc_frame_newslot(strlit1374);
  params[0] = strlit1374;
  partcv[0] = 1;
  Object opresult1376 = callmethod(opresult1373, "++", 1, partcv, params);
// compilenode returning opresult1376
  gc_frame_newslot(opresult1376);
  if (strlit1377 == NULL) {
    strlit1377 = alloc_String("r");
    gc_root(strlit1377);
  }
// compilenode returning strlit1377
  gc_frame_newslot(strlit1377);
// compilenode returning *var_io
  params[0] = opresult1376;
  params[1] = strlit1377;
  partcv[0] = 2;
  Object call1378 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1361);
// compilenode returning call1378
  *var_open = call1378;
  if (call1378 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 451
  setline(451);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 450
  setline(450);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1379 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call1380 = callmethod(*var_open, "read",
    1, partcv, params);
  gc_frame_end(callframe1379);
// compilenode returning call1380
// compilenode returning call1380
  *var_openData = call1380;
  if (call1380 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 452
  setline(452);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1381 = gc_frame_new();
// Begin line 451
  setline(451);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1386 == NULL) {
    strlit1386 = alloc_String("");
    gc_root(strlit1386);
  }
// compilenode returning strlit1386
  int op_slot_left_1385 = gc_frame_newslot(strlit1386);
// compilenode returning *var_newDir
  int op_slot_right_1385 = gc_frame_newslot(*var_newDir);
  params[0] = *var_newDir;
  partcv[0] = 1;
  Object opresult1388 = callmethod(strlit1386, "++", 1, partcv, params);
// compilenode returning opresult1388
  int op_slot_left_1384 = gc_frame_newslot(opresult1388);
  if (strlit1389 == NULL) {
    strlit1389 = alloc_String("");
    gc_root(strlit1389);
  }
// compilenode returning strlit1389
  int op_slot_right_1384 = gc_frame_newslot(strlit1389);
  params[0] = strlit1389;
  partcv[0] = 1;
  Object opresult1391 = callmethod(opresult1388, "++", 1, partcv, params);
// compilenode returning opresult1391
  int op_slot_left_1383 = gc_frame_newslot(opresult1391);
// compilenode returning *var_file
  int op_slot_right_1383 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1393 = callmethod(opresult1391, "++", 1, partcv, params);
// compilenode returning opresult1393
  int op_slot_left_1382 = gc_frame_newslot(opresult1393);
  if (strlit1394 == NULL) {
    strlit1394 = alloc_String("");
    gc_root(strlit1394);
  }
// compilenode returning strlit1394
  int op_slot_right_1382 = gc_frame_newslot(strlit1394);
  params[0] = strlit1394;
  partcv[0] = 1;
  Object opresult1396 = callmethod(opresult1393, "++", 1, partcv, params);
// compilenode returning opresult1396
  gc_frame_newslot(opresult1396);
  if (strlit1397 == NULL) {
    strlit1397 = alloc_String("w");
    gc_root(strlit1397);
  }
// compilenode returning strlit1397
  gc_frame_newslot(strlit1397);
// compilenode returning *var_io
  params[0] = opresult1396;
  params[1] = strlit1397;
  partcv[0] = 2;
  Object call1398 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1381);
// compilenode returning call1398
  *var_toWrite = call1398;
  if (call1398 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 453
  setline(453);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1399 = gc_frame_new();
// Begin line 452
  setline(452);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_openData
  gc_frame_newslot(*var_openData);
// compilenode returning *var_toWrite
  params[0] = *var_openData;
  partcv[0] = 1;
  Object call1400 = callmethod(*var_toWrite, "write",
    1, partcv, params);
  gc_frame_end(callframe1399);
// compilenode returning call1400
// Begin line 453
  setline(453);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1401 = gc_frame_new();
// compilenode returning *var_toWrite
  partcv[0] = 0;
  Object call1402 = callmethod(*var_toWrite, "close",
    1, partcv, params);
  gc_frame_end(callframe1401);
// compilenode returning call1402
// compilenode returning call1402
// Begin line 455
  setline(455);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1403 = gc_frame_new();
// Begin line 454
  setline(454);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_newDir
  int op_slot_left_1404 = gc_frame_newslot(*var_newDir);
// compilenode returning *var_file
  int op_slot_right_1404 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1406 = callmethod(*var_newDir, "++", 1, partcv, params);
// compilenode returning opresult1406
  gc_frame_newslot(opresult1406);
  params[0] = opresult1406;
  partcv[0] = 1;
  Object call1407 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1403);
// compilenode returning call1407
// Begin line 456
  setline(456);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 455
  setline(455);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1409 == NULL) {
    strlit1409 = alloc_String("Imported size = ");
    gc_root(strlit1409);
  }
// compilenode returning strlit1409
  int op_slot_left_1408 = gc_frame_newslot(strlit1409);
  int callframe1410 = gc_frame_new();
// compilenode returning *var_imported
  partcv[0] = 0;
  Object call1411 = callmethod(*var_imported, "size",
    1, partcv, params);
  gc_frame_end(callframe1410);
// compilenode returning call1411
// compilenode returning call1411
  int op_slot_right_1408 = gc_frame_newslot(call1411);
  params[0] = call1411;
  partcv[0] = 1;
  Object opresult1413 = callmethod(strlit1409, "++", 1, partcv, params);
// compilenode returning opresult1413
  params[0] = opresult1413;
  Object call1414 = gracelib_print(NULL, 1,  params);
// compilenode returning call1414
// Begin line 462
  setline(462);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1415 = gc_frame_new();
// Begin line 456
  setline(456);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1416 = alloc_Block(NULL, NULL, "PackageManager", 456);
  gc_frame_newslot(block1416);
  block_savedest(block1416);
  Object closure1417 = createclosure(2, "_apply");
setclosureframe(closure1417, stackframe);
  addtoclosure(closure1417, var_imported);
  addtoclosure(closure1417, selfslot);
  struct UserObject *uo1417 = (struct UserObject*)block1416;
  uo1417->data[0] = (Object)closure1417;
  Method *meth_meth_PackageManager__apply1417 = addmethod2pos(block1416, "_apply", &meth_PackageManager__apply1417, 0);
int argcv_meth_PackageManager__apply1417[] = {0};
meth_meth_PackageManager__apply1417->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1417);
  meth_meth_PackageManager__apply1417->definitionModule = modulename;
  meth_meth_PackageManager__apply1417->definitionLine = 455;
// compilenode returning block1416
  gc_frame_newslot(block1416);
// Begin line 462
  setline(462);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1425 = alloc_Block(NULL, NULL, "PackageManager", 462);
  gc_frame_newslot(block1425);
  block_savedest(block1425);
  Object closure1426 = createclosure(5, "_apply");
setclosureframe(closure1426, stackframe);
  addtoclosure(closure1426, var_imported);
  addtoclosure(closure1426, var_newDir);
  addtoclosure(closure1426, var_io);
  addtoclosure(closure1426, var_toWrite);
  addtoclosure(closure1426, selfslot);
  struct UserObject *uo1426 = (struct UserObject*)block1425;
  uo1426->data[0] = (Object)closure1426;
  Method *meth_meth_PackageManager__apply1426 = addmethod2pos(block1425, "_apply", &meth_PackageManager__apply1426, 0);
int argcv_meth_PackageManager__apply1426[] = {0};
meth_meth_PackageManager__apply1426->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1426);
  meth_meth_PackageManager__apply1426->definitionModule = modulename;
  meth_meth_PackageManager__apply1426->definitionLine = 456;
// compilenode returning block1425
  gc_frame_newslot(block1425);
  params[0] = block1416;
  params[1] = block1425;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1459 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1415);
// compilenode returning call1459
    gc_frame_newslot(call1459);
    if1337 = call1459;
  } else {
  }
// compilenode returning if1337
    gc_frame_newslot(if1337);
    if1330 = if1337;
  } else {
  }
// compilenode returning if1330
  gc_frame_end(frame);
  return if1330;
}
Object meth_PackageManager_bundle1292(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 25, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(5, getclosureframe(closure));
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
  Object *var_io = getfromclosure(closure, 1);
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
  Object *var_folder = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "folder");
// Begin line 442
  setline(442);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 440
  setline(440);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array1293 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array1293
  *var_imported = array1293;
  if (array1293 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 443
  setline(443);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 442
  setline(442);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1297 == NULL) {
    strlit1297 = alloc_String("In bundle. toBundle = ");
    gc_root(strlit1297);
  }
// compilenode returning strlit1297
  int op_slot_left_1296 = gc_frame_newslot(strlit1297);
// compilenode returning *var_toBundle
  int op_slot_right_1296 = gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object opresult1299 = callmethod(strlit1297, "++", 1, partcv, params);
// compilenode returning opresult1299
  int op_slot_left_1295 = gc_frame_newslot(opresult1299);
  if (strlit1300 == NULL) {
    strlit1300 = alloc_String("");
    gc_root(strlit1300);
  }
// compilenode returning strlit1300
  int op_slot_right_1295 = gc_frame_newslot(strlit1300);
  params[0] = strlit1300;
  partcv[0] = 1;
  Object opresult1302 = callmethod(opresult1299, "++", 1, partcv, params);
// compilenode returning opresult1302
  params[0] = opresult1302;
  Object call1303 = gracelib_print(NULL, 1,  params);
// compilenode returning call1303
// Begin line 444
  setline(444);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 443
  setline(443);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1304 = gc_frame_new();
  if (strlit1309 == NULL) {
    strlit1309 = alloc_String("");
    gc_root(strlit1309);
  }
// compilenode returning strlit1309
  int op_slot_left_1308 = gc_frame_newslot(strlit1309);
  int callframe1310 = gc_frame_new();
// compilenode returning *var_toBundle
  gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object call1311 = callmethodflags(self, "getContainingDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1310);
// compilenode returning call1311
  int op_slot_right_1308 = gc_frame_newslot(call1311);
  params[0] = call1311;
  partcv[0] = 1;
  Object opresult1313 = callmethod(strlit1309, "++", 1, partcv, params);
// compilenode returning opresult1313
  int op_slot_left_1307 = gc_frame_newslot(opresult1313);
  if (strlit1314 == NULL) {
    strlit1314 = alloc_String("../");
    gc_root(strlit1314);
  }
// compilenode returning strlit1314
  int op_slot_right_1307 = gc_frame_newslot(strlit1314);
  params[0] = strlit1314;
  partcv[0] = 1;
  Object opresult1316 = callmethod(opresult1313, "++", 1, partcv, params);
// compilenode returning opresult1316
  int op_slot_left_1306 = gc_frame_newslot(opresult1316);
// compilenode returning *var_name
  int op_slot_right_1306 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult1318 = callmethod(opresult1316, "++", 1, partcv, params);
// compilenode returning opresult1318
  int op_slot_left_1305 = gc_frame_newslot(opresult1318);
  if (strlit1319 == NULL) {
    strlit1319 = alloc_String("/");
    gc_root(strlit1319);
  }
// compilenode returning strlit1319
  int op_slot_right_1305 = gc_frame_newslot(strlit1319);
  params[0] = strlit1319;
  partcv[0] = 1;
  Object opresult1321 = callmethod(opresult1318, "++", 1, partcv, params);
// compilenode returning opresult1321
  gc_frame_newslot(opresult1321);
  params[0] = opresult1321;
  partcv[0] = 1;
  Object call1322 = callmethodflags(self, "createDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1304);
// compilenode returning call1322
  Object num1323 = alloc_Float64(1);
// compilenode returning num1323
  params[0] = num1323;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1324 = callmethod(call1322, "[]", 1, partcv, params);
// compilenode returning idxres1324
  *var_newDir = idxres1324;
  if (idxres1324 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 445
  setline(445);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1325 = gc_frame_new();
// Begin line 444
  setline(444);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toBundle
  gc_frame_newslot(*var_toBundle);
// compilenode returning *var_io
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object call1326 = callmethod(*var_io, "listdir",
    1, partcv, params);
  gc_frame_end(callframe1325);
// compilenode returning call1326
  *var_folder = call1326;
  if (call1326 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 465
  setline(465);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1327 = gc_frame_new();
// Begin line 445
  setline(445);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_folder
  gc_frame_newslot(*var_folder);
// Begin line 465
  setline(465);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1328 = alloc_Block(NULL, NULL, "PackageManager", 465);
  gc_frame_newslot(block1328);
  block_savedest(block1328);
  Object closure1329 = createclosure(5, "_apply");
setclosureframe(closure1329, stackframe);
  addtoclosure(closure1329, var_toBundle);
  addtoclosure(closure1329, var_io);
  addtoclosure(closure1329, var_newDir);
  addtoclosure(closure1329, var_imported);
  addtoclosure(closure1329, selfslot);
  struct UserObject *uo1329 = (struct UserObject*)block1328;
  uo1329->data[0] = (Object)closure1329;
  Method *meth_meth_PackageManager__apply1329 = addmethod2pos(block1328, "_apply", &meth_PackageManager__apply1329, 0);
int argcv_meth_PackageManager__apply1329[] = {1};
meth_meth_PackageManager__apply1329->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1329);
  meth_meth_PackageManager__apply1329->definitionModule = modulename;
  meth_meth_PackageManager__apply1329->definitionLine = 421;
// compilenode returning block1328
  gc_frame_newslot(block1328);
  params[0] = *var_folder;
  params[1] = block1328;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1461 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1327);
// compilenode returning call1461
  gc_frame_end(frame);
  return call1461;
}
Object meth_PackageManager__apply1533(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 481
  setline(481);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1534 = gc_frame_new();
// Begin line 480
  setline(480);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_d
  gc_frame_newslot(*var_d);
// compilenode returning *var_out
  params[0] = *var_d;
  partcv[0] = 1;
  Object call1535 = callmethod(*var_out, "write",
    1, partcv, params);
  gc_frame_end(callframe1534);
// compilenode returning call1535
  gc_frame_end(frame);
  return call1535;
}
Object meth_PackageManager__apply1469(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 484
  setline(484);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 471
  setline(471);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1472 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1473 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1472);
// compilenode returning call1473
// compilenode returning call1473
  int op_slot_left_1471 = gc_frame_newslot(call1473);
  Object num1474 = alloc_Float64(4);
// compilenode returning num1474
  int op_slot_right_1471 = gc_frame_newslot(num1474);
  params[0] = num1474;
  partcv[0] = 1;
  Object opresult1476 = callmethod(call1473, ">", 1, partcv, params);
// compilenode returning opresult1476
  Object if1470 = done;
  if (istrue(opresult1476)) {
// Begin line 483
  setline(483);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 472
  setline(472);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1479 = gc_frame_new();
  int callframe1481 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1482 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1481);
// compilenode returning call1482
// compilenode returning call1482
  int op_slot_left_1480 = gc_frame_newslot(call1482);
  Object num1483 = alloc_Float64(5);
// compilenode returning num1483
  int op_slot_right_1480 = gc_frame_newslot(num1483);
  params[0] = num1483;
  partcv[0] = 1;
  Object diff1485 = callmethod(call1482, "-", 1, partcv, params);
// compilenode returning diff1485
  gc_frame_newslot(diff1485);
  int callframe1486 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1487 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1486);
// compilenode returning call1487
// compilenode returning call1487
  gc_frame_newslot(call1487);
// compilenode returning *var_file
  params[0] = diff1485;
  params[1] = call1487;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1488 = callmethod(*var_file, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1479);
// compilenode returning call1488
  int op_slot_left_1478 = gc_frame_newslot(call1488);
  if (strlit1489 == NULL) {
    strlit1489 = alloc_String(".grace");
    gc_root(strlit1489);
  }
// compilenode returning strlit1489
  int op_slot_right_1478 = gc_frame_newslot(strlit1489);
  params[0] = strlit1489;
  partcv[0] = 1;
  Object opresult1491 = callmethod(call1488, "==", 1, partcv, params);
// compilenode returning opresult1491
  Object if1477 = done;
  if (istrue(opresult1491)) {
// Begin line 474
  setline(474);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1492 = gc_frame_new();
// Begin line 473
  setline(473);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_address
  int op_slot_left_1494 = gc_frame_newslot(*var_address);
  if (strlit1495 == NULL) {
    strlit1495 = alloc_String("/");
    gc_root(strlit1495);
  }
// compilenode returning strlit1495
  int op_slot_right_1494 = gc_frame_newslot(strlit1495);
  params[0] = strlit1495;
  partcv[0] = 1;
  Object opresult1497 = callmethod(*var_address, "++", 1, partcv, params);
// compilenode returning opresult1497
  int op_slot_left_1493 = gc_frame_newslot(opresult1497);
// compilenode returning *var_file
  int op_slot_right_1493 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1499 = callmethod(opresult1497, "++", 1, partcv, params);
// compilenode returning opresult1499
  gc_frame_newslot(opresult1499);
  if (strlit1500 == NULL) {
    strlit1500 = alloc_String("r");
    gc_root(strlit1500);
  }
// compilenode returning strlit1500
  gc_frame_newslot(strlit1500);
// compilenode returning *var_io
  params[0] = opresult1499;
  params[1] = strlit1500;
  partcv[0] = 2;
  Object call1501 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1492);
// compilenode returning call1501
  *var_open = call1501;
  if (call1501 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 475
  setline(475);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 474
  setline(474);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1502 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call1503 = callmethod(*var_open, "read",
    1, partcv, params);
  gc_frame_end(callframe1502);
// compilenode returning call1503
// compilenode returning call1503
  *var_readFile = call1503;
  if (call1503 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 476
  setline(476);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1504 = gc_frame_new();
// Begin line 475
  setline(475);
  setmodule(modulename);
  setsource(originalSourceLines);
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
  Object call1505 = callmethodflags(self, "parseAndPrefix", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1504);
// compilenode returning call1505
  *var_toWrite = call1505;
  if (call1505 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 476
  setline(476);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1506 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call1507 = callmethod(*var_open, "close",
    1, partcv, params);
  gc_frame_end(callframe1506);
// compilenode returning call1507
// compilenode returning call1507
// Begin line 478
  setline(478);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1508 = gc_frame_new();
// Begin line 477
  setline(477);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1512 == NULL) {
    strlit1512 = alloc_String("rm ");
    gc_root(strlit1512);
  }
// compilenode returning strlit1512
  int op_slot_left_1511 = gc_frame_newslot(strlit1512);
// compilenode returning *var_address
  int op_slot_right_1511 = gc_frame_newslot(*var_address);
  params[0] = *var_address;
  partcv[0] = 1;
  Object opresult1514 = callmethod(strlit1512, "++", 1, partcv, params);
// compilenode returning opresult1514
  int op_slot_left_1510 = gc_frame_newslot(opresult1514);
  if (strlit1515 == NULL) {
    strlit1515 = alloc_String("/");
    gc_root(strlit1515);
  }
// compilenode returning strlit1515
  int op_slot_right_1510 = gc_frame_newslot(strlit1515);
  params[0] = strlit1515;
  partcv[0] = 1;
  Object opresult1517 = callmethod(opresult1514, "++", 1, partcv, params);
// compilenode returning opresult1517
  int op_slot_left_1509 = gc_frame_newslot(opresult1517);
// compilenode returning *var_file
  int op_slot_right_1509 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1519 = callmethod(opresult1517, "++", 1, partcv, params);
// compilenode returning opresult1519
  gc_frame_newslot(opresult1519);
// compilenode returning *var_io
  params[0] = opresult1519;
  partcv[0] = 1;
  Object call1520 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe1508);
// compilenode returning call1520
// Begin line 479
  setline(479);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1521 = gc_frame_new();
// Begin line 478
  setline(478);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_address
  int op_slot_left_1523 = gc_frame_newslot(*var_address);
  if (strlit1524 == NULL) {
    strlit1524 = alloc_String("/");
    gc_root(strlit1524);
  }
// compilenode returning strlit1524
  int op_slot_right_1523 = gc_frame_newslot(strlit1524);
  params[0] = strlit1524;
  partcv[0] = 1;
  Object opresult1526 = callmethod(*var_address, "++", 1, partcv, params);
// compilenode returning opresult1526
  int op_slot_left_1522 = gc_frame_newslot(opresult1526);
// compilenode returning *var_file
  int op_slot_right_1522 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1528 = callmethod(opresult1526, "++", 1, partcv, params);
// compilenode returning opresult1528
  gc_frame_newslot(opresult1528);
  if (strlit1529 == NULL) {
    strlit1529 = alloc_String("w");
    gc_root(strlit1529);
  }
// compilenode returning strlit1529
  gc_frame_newslot(strlit1529);
// compilenode returning *var_io
  params[0] = opresult1528;
  params[1] = strlit1529;
  partcv[0] = 2;
  Object call1530 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1521);
// compilenode returning call1530
  *var_out = call1530;
  if (call1530 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 482
  setline(482);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1531 = gc_frame_new();
// Begin line 479
  setline(479);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toWrite
  gc_frame_newslot(*var_toWrite);
// Begin line 482
  setline(482);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1532 = alloc_Block(NULL, NULL, "PackageManager", 482);
  gc_frame_newslot(block1532);
  block_savedest(block1532);
  Object closure1533 = createclosure(2, "_apply");
setclosureframe(closure1533, stackframe);
  addtoclosure(closure1533, var_out);
  addtoclosure(closure1533, selfslot);
  struct UserObject *uo1533 = (struct UserObject*)block1532;
  uo1533->data[0] = (Object)closure1533;
  Method *meth_meth_PackageManager__apply1533 = addmethod2pos(block1532, "_apply", &meth_PackageManager__apply1533, 0);
int argcv_meth_PackageManager__apply1533[] = {1};
meth_meth_PackageManager__apply1533->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1533);
  meth_meth_PackageManager__apply1533->definitionModule = modulename;
  meth_meth_PackageManager__apply1533->definitionLine = 476;
// compilenode returning block1532
  gc_frame_newslot(block1532);
  params[0] = *var_toWrite;
  params[1] = block1532;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1537 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1531);
// compilenode returning call1537
    gc_frame_newslot(call1537);
    if1477 = call1537;
  } else {
  }
// compilenode returning if1477
    gc_frame_newslot(if1477);
    if1470 = if1477;
  } else {
  }
// compilenode returning if1470
  gc_frame_end(frame);
  return if1470;
}
Object meth_PackageManager_setImportDest1462(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 26, (flags>>24)&0xff);
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
// Begin line 469
  setline(469);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1463 = gc_frame_new();
// Begin line 468
  setline(468);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_address
  gc_frame_newslot(*var_address);
// compilenode returning *var_io
  params[0] = *var_address;
  partcv[0] = 1;
  Object call1464 = callmethod(*var_io, "getdir",
    1, partcv, params);
  gc_frame_end(callframe1463);
// compilenode returning call1464
  *var_folder = call1464;
  if (call1464 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 469
  setline(469);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1465 == NULL) {
    strlit1465 = alloc_String("Setting import dest");
    gc_root(strlit1465);
  }
// compilenode returning strlit1465
  params[0] = strlit1465;
  Object call1466 = gracelib_print(NULL, 1,  params);
// compilenode returning call1466
// Begin line 485
  setline(485);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1467 = gc_frame_new();
// Begin line 470
  setline(470);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_folder
  gc_frame_newslot(*var_folder);
// Begin line 485
  setline(485);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1468 = alloc_Block(NULL, NULL, "PackageManager", 485);
  gc_frame_newslot(block1468);
  block_savedest(block1468);
  Object closure1469 = createclosure(4, "_apply");
setclosureframe(closure1469, stackframe);
  addtoclosure(closure1469, var_address);
  addtoclosure(closure1469, var_io);
  addtoclosure(closure1469, var_prefix);
  addtoclosure(closure1469, selfslot);
  struct UserObject *uo1469 = (struct UserObject*)block1468;
  uo1469->data[0] = (Object)closure1469;
  Method *meth_meth_PackageManager__apply1469 = addmethod2pos(block1468, "_apply", &meth_PackageManager__apply1469, 0);
int argcv_meth_PackageManager__apply1469[] = {1};
meth_meth_PackageManager__apply1469->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1469);
  meth_meth_PackageManager__apply1469->definitionModule = modulename;
  meth_meth_PackageManager__apply1469->definitionLine = 460;
// compilenode returning block1468
  gc_frame_newslot(block1468);
  params[0] = *var_folder;
  params[1] = block1468;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1539 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1467);
// compilenode returning call1539
  gc_frame_end(frame);
  return call1539;
}
Object meth_PackageManager__apply1554(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1555 = gc_frame_newslot(*var_curPos);
  int callframe1556 = gc_frame_new();
// compilenode returning *var_readFile
  partcv[0] = 0;
  Object call1557 = callmethod(*var_readFile, "size",
    1, partcv, params);
  gc_frame_end(callframe1556);
// compilenode returning call1557
// compilenode returning call1557
  int op_slot_right_1555 = gc_frame_newslot(call1557);
  params[0] = call1557;
  partcv[0] = 1;
  Object opresult1559 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1559
  gc_frame_end(frame);
  return opresult1559;
}
Object meth_PackageManager__apply1566(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1568 = gc_frame_newslot(*var_curPos);
  int callframe1569 = gc_frame_new();
// compilenode returning *var_readFile
  partcv[0] = 0;
  Object call1570 = callmethod(*var_readFile, "size",
    1, partcv, params);
  gc_frame_end(callframe1569);
// compilenode returning call1570
// compilenode returning call1570
  int op_slot_right_1568 = gc_frame_newslot(call1570);
  params[0] = call1570;
  partcv[0] = 1;
  Object opresult1572 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1572
  int op_slot_left_1567 = gc_frame_newslot(opresult1572);
// compilenode returning *var_readFile
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1574 = callmethod(*var_readFile, "[]", 1, partcv, params);
// compilenode returning idxres1574
  int op_slot_left_1573 = gc_frame_newslot(idxres1574);
  if (strlit1575 == NULL) {
    strlit1575 = alloc_String("""\x0a""");
    gc_root(strlit1575);
  }
// compilenode returning strlit1575
  int op_slot_right_1573 = gc_frame_newslot(strlit1575);
  params[0] = strlit1575;
  partcv[0] = 1;
  Object opresult1577 = callmethod(idxres1574, "!=", 1, partcv, params);
// compilenode returning opresult1577
  int op_slot_right_1567 = gc_frame_newslot(opresult1577);
  params[0] = opresult1577;
  partcv[0] = 1;
  Object opresult1579 = callmethod(opresult1572, "&&", 1, partcv, params);
// compilenode returning opresult1579
  gc_frame_end(frame);
  return opresult1579;
}
Object meth_PackageManager__apply1582(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 497
  setline(497);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 496
  setline(496);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1583 = gc_frame_newslot(*var_curPos);
  Object num1584 = alloc_Float64(1);
// compilenode returning num1584
  int op_slot_right_1583 = gc_frame_newslot(num1584);
  params[0] = num1584;
  partcv[0] = 1;
  Object sum1586 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1586
  *var_curPos = sum1586;
  if (sum1586 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1607(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 503
  setline(503);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 502
  setline(502);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  int op_slot_left_1608 = gc_frame_newslot(*var_line);
// Begin line 503
  setline(503);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 502
  setline(502);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_readFile
// compilenode returning *var_b
  params[0] = *var_b;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1609 = callmethod(*var_readFile, "[]", 1, partcv, params);
// compilenode returning idxres1609
  int op_slot_right_1608 = gc_frame_newslot(idxres1609);
  params[0] = idxres1609;
  partcv[0] = 1;
  Object opresult1611 = callmethod(*var_line, "++", 1, partcv, params);
// compilenode returning opresult1611
  *var_line = opresult1611;
  if (opresult1611 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1651(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1653 = gc_frame_newslot(*var_pos);
  int callframe1654 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1655 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1654);
// compilenode returning call1655
// compilenode returning call1655
  int op_slot_right_1653 = gc_frame_newslot(call1655);
  params[0] = call1655;
  partcv[0] = 1;
  Object opresult1657 = callmethod(*var_pos, "<", 1, partcv, params);
// compilenode returning opresult1657
  int op_slot_left_1652 = gc_frame_newslot(opresult1657);
  int callframe1658 = gc_frame_new();
// compilenode returning *var_line
// compilenode returning *var_pos
  params[0] = *var_pos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1661 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1661
  int op_slot_left_1660 = gc_frame_newslot(idxres1661);
  if (strlit1662 == NULL) {
    strlit1662 = alloc_String("""\x22""");
    gc_root(strlit1662);
  }
// compilenode returning strlit1662
  int op_slot_right_1660 = gc_frame_newslot(strlit1662);
  params[0] = strlit1662;
  partcv[0] = 1;
  Object opresult1664 = callmethod(idxres1661, "==", 1, partcv, params);
// compilenode returning opresult1664
  int op_slot_left_1659 = gc_frame_newslot(opresult1664);
// compilenode returning *var_line
// compilenode returning *var_pos
  params[0] = *var_pos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1666 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1666
  int op_slot_left_1665 = gc_frame_newslot(idxres1666);
  if (strlit1667 == NULL) {
    strlit1667 = alloc_String(" ");
    gc_root(strlit1667);
  }
// compilenode returning strlit1667
  int op_slot_right_1665 = gc_frame_newslot(strlit1667);
  params[0] = strlit1667;
  partcv[0] = 1;
  Object opresult1669 = callmethod(idxres1666, "==", 1, partcv, params);
// compilenode returning opresult1669
  int op_slot_right_1659 = gc_frame_newslot(opresult1669);
  params[0] = opresult1669;
  partcv[0] = 1;
  Object opresult1671 = callmethod(opresult1664, "||", 1, partcv, params);
// compilenode returning opresult1671
  partcv[0] = 0;
  Object call1672 = callmethod(opresult1671, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1658);
// compilenode returning call1672
  int op_slot_right_1652 = gc_frame_newslot(call1672);
  params[0] = call1672;
  partcv[0] = 1;
  Object opresult1674 = callmethod(opresult1657, "&&", 1, partcv, params);
// compilenode returning opresult1674
  gc_frame_end(frame);
  return opresult1674;
}
Object meth_PackageManager__apply1677(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 514
  setline(514);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 513
  setline(513);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  int op_slot_left_1678 = gc_frame_newslot(*var_pos);
  Object num1679 = alloc_Float64(1);
// compilenode returning num1679
  int op_slot_right_1678 = gc_frame_newslot(num1679);
  params[0] = num1679;
  partcv[0] = 1;
  Object sum1681 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1681
  *var_pos = sum1681;
  if (sum1681 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1715(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 521
  setline(521);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1716 = gc_frame_new();
// Begin line 520
  setline(520);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_d
  gc_frame_newslot(*var_d);
// compilenode returning *var_outFile
  params[0] = *var_d;
  partcv[0] = 1;
  Object call1717 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1716);
// compilenode returning call1717
  gc_frame_end(frame);
  return call1717;
}
Object meth_PackageManager__apply1725(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 527
  setline(527);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1726 = gc_frame_new();
// Begin line 526
  setline(526);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_d
  gc_frame_newslot(*var_d);
// compilenode returning *var_outFile
  params[0] = *var_d;
  partcv[0] = 1;
  Object call1727 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1726);
// compilenode returning call1727
  gc_frame_end(frame);
  return call1727;
}
Object meth_PackageManager__apply1562(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 495
  setline(495);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 494
  setline(494);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 498
  setline(498);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1564 = gc_frame_new();
// Begin line 495
  setline(495);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1565 = alloc_Block(NULL, NULL, "PackageManager", 495);
  gc_frame_newslot(block1565);
  block_savedest(block1565);
  Object closure1566 = createclosure(3, "_apply");
setclosureframe(closure1566, stackframe);
  addtoclosure(closure1566, var_curPos);
  addtoclosure(closure1566, var_readFile);
  addtoclosure(closure1566, selfslot);
  struct UserObject *uo1566 = (struct UserObject*)block1565;
  uo1566->data[0] = (Object)closure1566;
  Method *meth_meth_PackageManager__apply1566 = addmethod2pos(block1565, "_apply", &meth_PackageManager__apply1566, 0);
int argcv_meth_PackageManager__apply1566[] = {0};
meth_meth_PackageManager__apply1566->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1566);
  meth_meth_PackageManager__apply1566->definitionModule = modulename;
  meth_meth_PackageManager__apply1566->definitionLine = 493;
// compilenode returning block1565
  gc_frame_newslot(block1565);
// Begin line 498
  setline(498);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1581 = alloc_Block(NULL, NULL, "PackageManager", 498);
  gc_frame_newslot(block1581);
  block_savedest(block1581);
  Object closure1582 = createclosure(2, "_apply");
setclosureframe(closure1582, stackframe);
  addtoclosure(closure1582, var_curPos);
  addtoclosure(closure1582, selfslot);
  struct UserObject *uo1582 = (struct UserObject*)block1581;
  uo1582->data[0] = (Object)closure1582;
  Method *meth_meth_PackageManager__apply1582 = addmethod2pos(block1581, "_apply", &meth_PackageManager__apply1582, 0);
int argcv_meth_PackageManager__apply1582[] = {0};
meth_meth_PackageManager__apply1582->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1582);
  meth_meth_PackageManager__apply1582->definitionModule = modulename;
  meth_meth_PackageManager__apply1582->definitionLine = 495;
// compilenode returning block1581
  gc_frame_newslot(block1581);
  params[0] = block1565;
  params[1] = block1581;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1589 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1564);
// compilenode returning call1589
// Begin line 499
  setline(499);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1590 = gc_frame_new();
// Begin line 498
  setline(498);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_startPos
  gc_frame_newslot(*var_startPos);
// compilenode returning *var_curPos
  int op_slot_left_1591 = gc_frame_newslot(*var_curPos);
  Object num1592 = alloc_Float64(1);
// compilenode returning num1592
  int op_slot_right_1591 = gc_frame_newslot(num1592);
  params[0] = num1592;
  partcv[0] = 1;
  Object diff1594 = callmethod(*var_curPos, "-", 1, partcv, params);
// compilenode returning diff1594
  gc_frame_newslot(diff1594);
// compilenode returning *var_readFile
  params[0] = *var_startPos;
  params[1] = diff1594;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1595 = callmethod(*var_readFile, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1590);
// compilenode returning call1595
  *var_data = call1595;
  if (call1595 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 500
  setline(500);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 499
  setline(499);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1596 == NULL) {
    strlit1596 = alloc_String("");
    gc_root(strlit1596);
  }
// compilenode returning strlit1596
  *var_line = strlit1596;
  if (strlit1596 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 501
  setline(501);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 500
  setline(500);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1597 = alloc_Float64(1);
// compilenode returning num1597
  *var_count = num1597;
  if (num1597 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 504
  setline(504);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1598 = gc_frame_new();
// Begin line 501
  setline(501);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_startPos
  int op_slot_left_1599 = gc_frame_newslot(*var_startPos);
// compilenode returning *var_curPos
  int op_slot_left_1600 = gc_frame_newslot(*var_curPos);
  Object num1601 = alloc_Float64(1);
// compilenode returning num1601
  int op_slot_right_1600 = gc_frame_newslot(num1601);
  params[0] = num1601;
  partcv[0] = 1;
  Object diff1603 = callmethod(*var_curPos, "-", 1, partcv, params);
// compilenode returning diff1603
  int op_slot_right_1599 = gc_frame_newslot(diff1603);
  params[0] = diff1603;
  partcv[0] = 1;
  Object opresult1605 = callmethod(*var_startPos, "..", 1, partcv, params);
// compilenode returning opresult1605
  gc_frame_newslot(opresult1605);
// Begin line 504
  setline(504);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1606 = alloc_Block(NULL, NULL, "PackageManager", 504);
  gc_frame_newslot(block1606);
  block_savedest(block1606);
  Object closure1607 = createclosure(3, "_apply");
setclosureframe(closure1607, stackframe);
  addtoclosure(closure1607, var_line);
  addtoclosure(closure1607, var_readFile);
  addtoclosure(closure1607, selfslot);
  struct UserObject *uo1607 = (struct UserObject*)block1606;
  uo1607->data[0] = (Object)closure1607;
  Method *meth_meth_PackageManager__apply1607 = addmethod2pos(block1606, "_apply", &meth_PackageManager__apply1607, 0);
int argcv_meth_PackageManager__apply1607[] = {1};
meth_meth_PackageManager__apply1607->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1607);
  meth_meth_PackageManager__apply1607->definitionModule = modulename;
  meth_meth_PackageManager__apply1607->definitionLine = 495;
// compilenode returning block1606
  gc_frame_newslot(block1606);
  params[0] = opresult1605;
  params[1] = block1606;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1614 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1598);
// compilenode returning call1614
// Begin line 505
  setline(505);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 504
  setline(504);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  params[0] = *var_line;
  Object call1615 = gracelib_print(NULL, 1,  params);
// compilenode returning call1615
// Begin line 530
  setline(530);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 505
  setline(505);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1619 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1620 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1619);
// compilenode returning call1620
// compilenode returning call1620
  int op_slot_left_1618 = gc_frame_newslot(call1620);
  Object num1621 = alloc_Float64(6);
// compilenode returning num1621
  int op_slot_right_1618 = gc_frame_newslot(num1621);
  params[0] = num1621;
  partcv[0] = 1;
  Object opresult1623 = callmethod(call1620, ">", 1, partcv, params);
// compilenode returning opresult1623
  int op_slot_left_1617 = gc_frame_newslot(opresult1623);
  int callframe1625 = gc_frame_new();
  Object num1626 = alloc_Float64(1);
// compilenode returning num1626
  gc_frame_newslot(num1626);
  Object num1627 = alloc_Float64(7);
// compilenode returning num1627
  gc_frame_newslot(num1627);
// compilenode returning *var_line
  params[0] = num1626;
  params[1] = num1627;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1628 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1625);
// compilenode returning call1628
  int op_slot_left_1624 = gc_frame_newslot(call1628);
  if (strlit1629 == NULL) {
    strlit1629 = alloc_String("import ");
    gc_root(strlit1629);
  }
// compilenode returning strlit1629
  int op_slot_right_1624 = gc_frame_newslot(strlit1629);
  params[0] = strlit1629;
  partcv[0] = 1;
  Object opresult1631 = callmethod(call1628, "==", 1, partcv, params);
// compilenode returning opresult1631
  int op_slot_right_1617 = gc_frame_newslot(opresult1631);
  params[0] = opresult1631;
  partcv[0] = 1;
  Object opresult1633 = callmethod(opresult1623, "&&", 1, partcv, params);
// compilenode returning opresult1633
  Object if1616 = done;
  if (istrue(opresult1633)) {
// Begin line 507
  setline(507);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 506
  setline(506);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1634 = alloc_Float64(1);
// compilenode returning num1634
  *var_pos = num1634;
  if (num1634 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 508
  setline(508);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 507
  setline(507);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  *var_start = *var_pos;
  if (*var_pos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 509
  setline(509);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 508
  setline(508);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1635 == NULL) {
    strlit1635 = alloc_String("");
    gc_root(strlit1635);
  }
// compilenode returning strlit1635
  *var_nextImport = strlit1635;
  if (strlit1635 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 510
  setline(510);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1636 = gc_frame_new();
// Begin line 509
  setline(509);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// compilenode returning *var_pos
  gc_frame_newslot(*var_pos);
  params[0] = *var_line;
  params[1] = *var_pos;
  partcv[0] = 2;
  Object call1637 = callmethodflags(self, "skipWhiteSpace", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1636);
// compilenode returning call1637
  *var_pos = call1637;
  if (call1637 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 511
  setline(511);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 510
  setline(510);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  int op_slot_left_1639 = gc_frame_newslot(*var_pos);
  Object num1640 = alloc_Float64(1);
// compilenode returning num1640
  int op_slot_right_1639 = gc_frame_newslot(num1640);
  params[0] = num1640;
  partcv[0] = 1;
  Object sum1642 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1642
  *var_pos = sum1642;
  if (sum1642 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 512
  setline(512);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 511
  setline(511);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1645 == NULL) {
    strlit1645 = alloc_String("LINE = ");
    gc_root(strlit1645);
  }
// compilenode returning strlit1645
  int op_slot_left_1644 = gc_frame_newslot(strlit1645);
// compilenode returning *var_line
  int op_slot_right_1644 = gc_frame_newslot(*var_line);
  params[0] = *var_line;
  partcv[0] = 1;
  Object opresult1647 = callmethod(strlit1645, "++", 1, partcv, params);
// compilenode returning opresult1647
  params[0] = opresult1647;
  Object call1648 = gracelib_print(NULL, 1,  params);
// compilenode returning call1648
// Begin line 515
  setline(515);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1649 = gc_frame_new();
// Begin line 512
  setline(512);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1650 = alloc_Block(NULL, NULL, "PackageManager", 512);
  gc_frame_newslot(block1650);
  block_savedest(block1650);
  Object closure1651 = createclosure(3, "_apply");
setclosureframe(closure1651, stackframe);
  addtoclosure(closure1651, var_pos);
  addtoclosure(closure1651, var_line);
  addtoclosure(closure1651, selfslot);
  struct UserObject *uo1651 = (struct UserObject*)block1650;
  uo1651->data[0] = (Object)closure1651;
  Method *meth_meth_PackageManager__apply1651 = addmethod2pos(block1650, "_apply", &meth_PackageManager__apply1651, 0);
int argcv_meth_PackageManager__apply1651[] = {0};
meth_meth_PackageManager__apply1651->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1651);
  meth_meth_PackageManager__apply1651->definitionModule = modulename;
  meth_meth_PackageManager__apply1651->definitionLine = 505;
// compilenode returning block1650
  gc_frame_newslot(block1650);
// Begin line 515
  setline(515);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1676 = alloc_Block(NULL, NULL, "PackageManager", 515);
  gc_frame_newslot(block1676);
  block_savedest(block1676);
  Object closure1677 = createclosure(2, "_apply");
setclosureframe(closure1677, stackframe);
  addtoclosure(closure1677, var_pos);
  addtoclosure(closure1677, selfslot);
  struct UserObject *uo1677 = (struct UserObject*)block1676;
  uo1677->data[0] = (Object)closure1677;
  Method *meth_meth_PackageManager__apply1677 = addmethod2pos(block1676, "_apply", &meth_PackageManager__apply1677, 0);
int argcv_meth_PackageManager__apply1677[] = {0};
meth_meth_PackageManager__apply1677->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1677);
  meth_meth_PackageManager__apply1677->definitionModule = modulename;
  meth_meth_PackageManager__apply1677->definitionLine = 512;
// compilenode returning block1676
  gc_frame_newslot(block1676);
  params[0] = block1650;
  params[1] = block1676;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1684 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1649);
// compilenode returning call1684
// Begin line 516
  setline(516);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 515
  setline(515);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  int op_slot_left_1685 = gc_frame_newslot(*var_pos);
  Object num1686 = alloc_Float64(1);
// compilenode returning num1686
  int op_slot_right_1685 = gc_frame_newslot(num1686);
  params[0] = num1686;
  partcv[0] = 1;
  Object sum1688 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1688
  *var_pos = sum1688;
  if (sum1688 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 517
  setline(517);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1690 = gc_frame_new();
// Begin line 516
  setline(516);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1691 = gc_frame_new();
// compilenode returning *var_pos
  int op_slot_left_1692 = gc_frame_newslot(*var_pos);
  Object num1693 = alloc_Float64(1);
// compilenode returning num1693
  int op_slot_right_1692 = gc_frame_newslot(num1693);
  params[0] = num1693;
  partcv[0] = 1;
  Object sum1695 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1695
  gc_frame_newslot(sum1695);
  int callframe1696 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1697 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1696);
// compilenode returning call1697
// compilenode returning call1697
  gc_frame_newslot(call1697);
// compilenode returning *var_line
  params[0] = sum1695;
  params[1] = call1697;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1698 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1691);
// compilenode returning call1698
  gc_frame_newslot(call1698);
  params[0] = call1698;
  partcv[0] = 1;
  Object call1699 = callmethodflags(self, "removeExistingUrls", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1690);
// compilenode returning call1699
  *var_remainder = call1699;
  if (call1699 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 518
  setline(518);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 517
  setline(517);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1703 == NULL) {
    strlit1703 = alloc_String("import ""\x22""");
    gc_root(strlit1703);
  }
// compilenode returning strlit1703
  int op_slot_left_1702 = gc_frame_newslot(strlit1703);
// compilenode returning *var_prefix
  int op_slot_right_1702 = gc_frame_newslot(*var_prefix);
  params[0] = *var_prefix;
  partcv[0] = 1;
  Object opresult1705 = callmethod(strlit1703, "++", 1, partcv, params);
// compilenode returning opresult1705
  int op_slot_left_1701 = gc_frame_newslot(opresult1705);
  if (strlit1706 == NULL) {
    strlit1706 = alloc_String("/");
    gc_root(strlit1706);
  }
// compilenode returning strlit1706
  int op_slot_right_1701 = gc_frame_newslot(strlit1706);
  params[0] = strlit1706;
  partcv[0] = 1;
  Object opresult1708 = callmethod(opresult1705, "++", 1, partcv, params);
// compilenode returning opresult1708
  int op_slot_left_1700 = gc_frame_newslot(opresult1708);
// compilenode returning *var_remainder
  int op_slot_right_1700 = gc_frame_newslot(*var_remainder);
  params[0] = *var_remainder;
  partcv[0] = 1;
  Object opresult1710 = callmethod(opresult1708, "++", 1, partcv, params);
// compilenode returning opresult1710
  *var_line = opresult1710;
  if (opresult1710 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 519
  setline(519);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 518
  setline(518);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  params[0] = *var_line;
  Object call1712 = gracelib_print(NULL, 1,  params);
// compilenode returning call1712
// Begin line 522
  setline(522);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1713 = gc_frame_new();
// Begin line 519
  setline(519);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// Begin line 522
  setline(522);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1714 = alloc_Block(NULL, NULL, "PackageManager", 522);
  gc_frame_newslot(block1714);
  block_savedest(block1714);
  Object closure1715 = createclosure(2, "_apply");
setclosureframe(closure1715, stackframe);
  addtoclosure(closure1715, var_outFile);
  addtoclosure(closure1715, selfslot);
  struct UserObject *uo1715 = (struct UserObject*)block1714;
  uo1715->data[0] = (Object)closure1715;
  Method *meth_meth_PackageManager__apply1715 = addmethod2pos(block1714, "_apply", &meth_PackageManager__apply1715, 0);
int argcv_meth_PackageManager__apply1715[] = {1};
meth_meth_PackageManager__apply1715->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1715);
  meth_meth_PackageManager__apply1715->definitionModule = modulename;
  meth_meth_PackageManager__apply1715->definitionLine = 516;
// compilenode returning block1714
  gc_frame_newslot(block1714);
  params[0] = *var_line;
  params[1] = block1714;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1719 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1713);
// compilenode returning call1719
// Begin line 523
  setline(523);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1720 = gc_frame_new();
// Begin line 522
  setline(522);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1721 == NULL) {
    strlit1721 = alloc_String("""\x0a""");
    gc_root(strlit1721);
  }
// compilenode returning strlit1721
  gc_frame_newslot(strlit1721);
// compilenode returning *var_outFile
  params[0] = strlit1721;
  partcv[0] = 1;
  Object call1722 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1720);
// compilenode returning call1722
    gc_frame_newslot(call1722);
    if1616 = call1722;
  } else {
// Begin line 528
  setline(528);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1723 = gc_frame_new();
// Begin line 525
  setline(525);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// Begin line 528
  setline(528);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1724 = alloc_Block(NULL, NULL, "PackageManager", 528);
  gc_frame_newslot(block1724);
  block_savedest(block1724);
  Object closure1725 = createclosure(2, "_apply");
setclosureframe(closure1725, stackframe);
  addtoclosure(closure1725, var_outFile);
  addtoclosure(closure1725, selfslot);
  struct UserObject *uo1725 = (struct UserObject*)block1724;
  uo1725->data[0] = (Object)closure1725;
  Method *meth_meth_PackageManager__apply1725 = addmethod2pos(block1724, "_apply", &meth_PackageManager__apply1725, 0);
int argcv_meth_PackageManager__apply1725[] = {1};
meth_meth_PackageManager__apply1725->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1725);
  meth_meth_PackageManager__apply1725->definitionModule = modulename;
  meth_meth_PackageManager__apply1725->definitionLine = 516;
// compilenode returning block1724
  gc_frame_newslot(block1724);
  params[0] = *var_line;
  params[1] = block1724;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1729 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1723);
// compilenode returning call1729
// Begin line 529
  setline(529);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1730 = gc_frame_new();
// Begin line 528
  setline(528);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1731 == NULL) {
    strlit1731 = alloc_String("""\x0a""");
    gc_root(strlit1731);
  }
// compilenode returning strlit1731
  gc_frame_newslot(strlit1731);
// compilenode returning *var_outFile
  params[0] = strlit1731;
  partcv[0] = 1;
  Object call1732 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1730);
// compilenode returning call1732
    gc_frame_newslot(call1732);
    if1616 = call1732;
  }
// compilenode returning if1616
// Begin line 531
  setline(531);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 530
  setline(530);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1733 = gc_frame_newslot(*var_curPos);
  Object num1734 = alloc_Float64(1);
// compilenode returning num1734
  int op_slot_right_1733 = gc_frame_newslot(num1734);
  params[0] = num1734;
  partcv[0] = 1;
  Object sum1736 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1736
  *var_curPos = sum1736;
  if (sum1736 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1551(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 532
  setline(532);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1552 = gc_frame_new();
// Begin line 493
  setline(493);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1553 = alloc_Block(NULL, NULL, "PackageManager", 493);
  gc_frame_newslot(block1553);
  block_savedest(block1553);
  Object closure1554 = createclosure(3, "_apply");
setclosureframe(closure1554, stackframe);
  addtoclosure(closure1554, var_curPos);
  addtoclosure(closure1554, var_readFile);
  addtoclosure(closure1554, selfslot);
  struct UserObject *uo1554 = (struct UserObject*)block1553;
  uo1554->data[0] = (Object)closure1554;
  Method *meth_meth_PackageManager__apply1554 = addmethod2pos(block1553, "_apply", &meth_PackageManager__apply1554, 0);
int argcv_meth_PackageManager__apply1554[] = {0};
meth_meth_PackageManager__apply1554->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1554);
  meth_meth_PackageManager__apply1554->definitionModule = modulename;
  meth_meth_PackageManager__apply1554->definitionLine = 492;
// compilenode returning block1553
  gc_frame_newslot(block1553);
// Begin line 532
  setline(532);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1561 = alloc_Block(NULL, NULL, "PackageManager", 532);
  gc_frame_newslot(block1561);
  block_savedest(block1561);
  Object closure1562 = createclosure(6, "_apply");
setclosureframe(closure1562, stackframe);
  addtoclosure(closure1562, var_curPos);
  addtoclosure(closure1562, var_startPos);
  addtoclosure(closure1562, var_readFile);
  addtoclosure(closure1562, var_prefix);
  addtoclosure(closure1562, var_outFile);
  addtoclosure(closure1562, selfslot);
  struct UserObject *uo1562 = (struct UserObject*)block1561;
  uo1562->data[0] = (Object)closure1562;
  Method *meth_meth_PackageManager__apply1562 = addmethod2pos(block1561, "_apply", &meth_PackageManager__apply1562, 0);
int argcv_meth_PackageManager__apply1562[] = {0};
meth_meth_PackageManager__apply1562->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1562);
  meth_meth_PackageManager__apply1562->definitionModule = modulename;
  meth_meth_PackageManager__apply1562->definitionLine = 493;
// compilenode returning block1561
  gc_frame_newslot(block1561);
  params[0] = block1553;
  params[1] = block1561;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1739 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1552);
// compilenode returning call1739
  gc_frame_end(frame);
  return call1739;
}
Object meth_PackageManager_parseAndPrefix1540(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 490
  setline(490);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 489
  setline(489);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1541 = alloc_Float64(1);
// compilenode returning num1541
  *var_curPos = num1541;
  if (num1541 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 491
  setline(491);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 490
  setline(490);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 492
  setline(492);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 491
  setline(491);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array1542 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array1542
  *var_outFile = array1542;
  if (array1542 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 533
  setline(533);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1543 = gc_frame_new();
// Begin line 492
  setline(492);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1545 = alloc_Float64(0);
// compilenode returning num1545
  int op_slot_left_1544 = gc_frame_newslot(num1545);
  int callframe1546 = gc_frame_new();
// compilenode returning *var_readFile
  partcv[0] = 0;
  Object call1547 = callmethod(*var_readFile, "size",
    1, partcv, params);
  gc_frame_end(callframe1546);
// compilenode returning call1547
// compilenode returning call1547
  int op_slot_right_1544 = gc_frame_newslot(call1547);
  params[0] = call1547;
  partcv[0] = 1;
  Object opresult1549 = callmethod(num1545, "..", 1, partcv, params);
// compilenode returning opresult1549
  gc_frame_newslot(opresult1549);
// Begin line 533
  setline(533);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1550 = alloc_Block(NULL, NULL, "PackageManager", 533);
  gc_frame_newslot(block1550);
  block_savedest(block1550);
  Object closure1551 = createclosure(6, "_apply");
setclosureframe(closure1551, stackframe);
  addtoclosure(closure1551, var_curPos);
  addtoclosure(closure1551, var_readFile);
  addtoclosure(closure1551, var_startPos);
  addtoclosure(closure1551, var_prefix);
  addtoclosure(closure1551, var_outFile);
  addtoclosure(closure1551, selfslot);
  struct UserObject *uo1551 = (struct UserObject*)block1550;
  uo1551->data[0] = (Object)closure1551;
  Method *meth_meth_PackageManager__apply1551 = addmethod2pos(block1550, "_apply", &meth_PackageManager__apply1551, 0);
int argcv_meth_PackageManager__apply1551[] = {1};
meth_meth_PackageManager__apply1551->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1551);
  meth_meth_PackageManager__apply1551->definitionModule = modulename;
  meth_meth_PackageManager__apply1551->definitionLine = 492;
// compilenode returning block1550
  gc_frame_newslot(block1550);
  params[0] = opresult1549;
  params[1] = block1550;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1741 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1543);
// compilenode returning call1741
// Begin line 536
  setline(536);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1742 = gc_frame_new();
// Begin line 533
  setline(533);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1743 == NULL) {
    strlit1743 = alloc_String("""\x0a""");
    gc_root(strlit1743);
  }
// compilenode returning strlit1743
  gc_frame_newslot(strlit1743);
// compilenode returning *var_outFile
  params[0] = strlit1743;
  partcv[0] = 1;
  Object call1744 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1742);
// compilenode returning call1744
// Begin line 537
  setline(537);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 536
  setline(536);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1746 == NULL) {
    strlit1746 = alloc_String("OUTFILE ");
    gc_root(strlit1746);
  }
// compilenode returning strlit1746
  int op_slot_left_1745 = gc_frame_newslot(strlit1746);
// compilenode returning *var_outFile
  int op_slot_right_1745 = gc_frame_newslot(*var_outFile);
  params[0] = *var_outFile;
  partcv[0] = 1;
  Object opresult1748 = callmethod(strlit1746, "++", 1, partcv, params);
// compilenode returning opresult1748
  params[0] = opresult1748;
  Object call1749 = gracelib_print(NULL, 1,  params);
// compilenode returning call1749
// Begin line 539
  setline(539);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 537
  setline(537);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_outFile
  return *var_outFile;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply1771(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1772 = gc_frame_newslot(*var_curPos);
  int callframe1773 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1774 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1773);
// compilenode returning call1774
// compilenode returning call1774
  int op_slot_right_1772 = gc_frame_newslot(call1774);
  params[0] = call1774;
  partcv[0] = 1;
  Object opresult1776 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1776
  gc_frame_end(frame);
  return opresult1776;
}
Object meth_PackageManager__apply1779(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 552
  setline(552);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 549
  setline(549);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1782 = callmethod(*var_importStatement, "[]", 1, partcv, params);
// compilenode returning idxres1782
  int op_slot_left_1781 = gc_frame_newslot(idxres1782);
  if (strlit1783 == NULL) {
    strlit1783 = alloc_String("/");
    gc_root(strlit1783);
  }
// compilenode returning strlit1783
  int op_slot_right_1781 = gc_frame_newslot(strlit1783);
  params[0] = strlit1783;
  partcv[0] = 1;
  Object opresult1785 = callmethod(idxres1782, "==", 1, partcv, params);
// compilenode returning opresult1785
  Object if1780 = done;
  if (istrue(opresult1785)) {
// Begin line 551
  setline(551);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 550
  setline(550);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_lastIndex = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if1780 = done;
  } else {
  }
// compilenode returning if1780
// Begin line 553
  setline(553);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 552
  setline(552);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1787 = gc_frame_newslot(*var_curPos);
  Object num1788 = alloc_Float64(1);
// compilenode returning num1788
  int op_slot_right_1787 = gc_frame_newslot(num1788);
  params[0] = num1788;
  partcv[0] = 1;
  Object sum1790 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1790
  *var_curPos = sum1790;
  if (sum1790 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_removeExistingUrls1750(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 545
  setline(545);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 542
  setline(542);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1753 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1754 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1753);
// compilenode returning call1754
// compilenode returning call1754
  int op_slot_left_1752 = gc_frame_newslot(call1754);
  Object num1755 = alloc_Float64(7);
// compilenode returning num1755
  int op_slot_right_1752 = gc_frame_newslot(num1755);
  params[0] = num1755;
  partcv[0] = 1;
  Object opresult1757 = callmethod(call1754, "<", 1, partcv, params);
// compilenode returning opresult1757
  Object if1751 = done;
  if (istrue(opresult1757)) {
// Begin line 544
  setline(544);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 543
  setline(543);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
  return *var_importStatement;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1751 = undefined;
  } else {
  }
// compilenode returning if1751
// Begin line 561
  setline(561);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 545
  setline(545);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1760 = gc_frame_new();
  Object num1761 = alloc_Float64(1);
// compilenode returning num1761
  gc_frame_newslot(num1761);
  Object num1762 = alloc_Float64(7);
// compilenode returning num1762
  gc_frame_newslot(num1762);
// compilenode returning *var_importStatement
  params[0] = num1761;
  params[1] = num1762;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1763 = callmethod(*var_importStatement, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1760);
// compilenode returning call1763
  int op_slot_left_1759 = gc_frame_newslot(call1763);
  if (strlit1764 == NULL) {
    strlit1764 = alloc_String("http://");
    gc_root(strlit1764);
  }
// compilenode returning strlit1764
  int op_slot_right_1759 = gc_frame_newslot(strlit1764);
  params[0] = strlit1764;
  partcv[0] = 1;
  Object opresult1766 = callmethod(call1763, "==", 1, partcv, params);
// compilenode returning opresult1766
  Object if1758 = done;
  if (istrue(opresult1766)) {
// Begin line 547
  setline(547);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 546
  setline(546);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1767 = alloc_Float64(7);
// compilenode returning num1767
  *var_lastIndex = num1767;
  if (num1767 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 548
  setline(548);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 547
  setline(547);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1768 = alloc_Float64(7);
// compilenode returning num1768
  *var_curPos = num1768;
  if (num1768 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 554
  setline(554);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1769 = gc_frame_new();
// Begin line 548
  setline(548);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1770 = alloc_Block(NULL, NULL, "PackageManager", 548);
  gc_frame_newslot(block1770);
  block_savedest(block1770);
  Object closure1771 = createclosure(3, "_apply");
setclosureframe(closure1771, stackframe);
  addtoclosure(closure1771, var_curPos);
  addtoclosure(closure1771, var_importStatement);
  addtoclosure(closure1771, selfslot);
  struct UserObject *uo1771 = (struct UserObject*)block1770;
  uo1771->data[0] = (Object)closure1771;
  Method *meth_meth_PackageManager__apply1771 = addmethod2pos(block1770, "_apply", &meth_PackageManager__apply1771, 0);
int argcv_meth_PackageManager__apply1771[] = {0};
meth_meth_PackageManager__apply1771->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1771);
  meth_meth_PackageManager__apply1771->definitionModule = modulename;
  meth_meth_PackageManager__apply1771->definitionLine = 542;
// compilenode returning block1770
  gc_frame_newslot(block1770);
// Begin line 554
  setline(554);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1778 = alloc_Block(NULL, NULL, "PackageManager", 554);
  gc_frame_newslot(block1778);
  block_savedest(block1778);
  Object closure1779 = createclosure(4, "_apply");
setclosureframe(closure1779, stackframe);
  addtoclosure(closure1779, var_importStatement);
  addtoclosure(closure1779, var_curPos);
  addtoclosure(closure1779, var_lastIndex);
  addtoclosure(closure1779, selfslot);
  struct UserObject *uo1779 = (struct UserObject*)block1778;
  uo1779->data[0] = (Object)closure1779;
  Method *meth_meth_PackageManager__apply1779 = addmethod2pos(block1778, "_apply", &meth_PackageManager__apply1779, 0);
int argcv_meth_PackageManager__apply1779[] = {0};
meth_meth_PackageManager__apply1779->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1779);
  meth_meth_PackageManager__apply1779->definitionModule = modulename;
  meth_meth_PackageManager__apply1779->definitionLine = 548;
// compilenode returning block1778
  gc_frame_newslot(block1778);
  params[0] = block1770;
  params[1] = block1778;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1793 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1769);
// compilenode returning call1793
// Begin line 555
  setline(555);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1794 = gc_frame_new();
// Begin line 554
  setline(554);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_lastIndex
  int op_slot_left_1795 = gc_frame_newslot(*var_lastIndex);
  Object num1796 = alloc_Float64(1);
// compilenode returning num1796
  int op_slot_right_1795 = gc_frame_newslot(num1796);
  params[0] = num1796;
  partcv[0] = 1;
  Object sum1798 = callmethod(*var_lastIndex, "+", 1, partcv, params);
// compilenode returning sum1798
  gc_frame_newslot(sum1798);
  int callframe1799 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1800 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1799);
// compilenode returning call1800
// compilenode returning call1800
  gc_frame_newslot(call1800);
// compilenode returning *var_importStatement
  params[0] = sum1798;
  params[1] = call1800;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1801 = callmethod(*var_importStatement, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1794);
// compilenode returning call1801
  *var_res = call1801;
  if (call1801 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 556
  setline(556);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 555
  setline(555);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_res
  return *var_res;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1758 = undefined;
  } else {
// Begin line 559
  setline(559);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 558
  setline(558);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
  return *var_importStatement;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1758 = undefined;
  }
// compilenode returning if1758
  gc_frame_end(frame);
  return if1758;
}
Object meth_PackageManager__apply1823(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1824 = gc_frame_newslot(*var_curPos);
  int callframe1825 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1826 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1825);
// compilenode returning call1826
// compilenode returning call1826
  int op_slot_right_1824 = gc_frame_newslot(call1826);
  params[0] = call1826;
  partcv[0] = 1;
  Object opresult1828 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1828
  gc_frame_end(frame);
  return opresult1828;
}
Object meth_PackageManager__apply1831(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 572
  setline(572);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1834 = callmethod(*var_importStatement, "[]", 1, partcv, params);
// compilenode returning idxres1834
  int op_slot_left_1833 = gc_frame_newslot(idxres1834);
  if (strlit1835 == NULL) {
    strlit1835 = alloc_String("/");
    gc_root(strlit1835);
  }
// compilenode returning strlit1835
  int op_slot_right_1833 = gc_frame_newslot(strlit1835);
  params[0] = strlit1835;
  partcv[0] = 1;
  Object opresult1837 = callmethod(idxres1834, "==", 1, partcv, params);
// compilenode returning opresult1837
  Object if1832 = done;
  if (istrue(opresult1837)) {
// Begin line 574
  setline(574);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 573
  setline(573);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_lastIndex = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if1832 = done;
  } else {
  }
// compilenode returning if1832
// Begin line 576
  setline(576);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1839 = gc_frame_newslot(*var_curPos);
  Object num1840 = alloc_Float64(1);
// compilenode returning num1840
  int op_slot_right_1839 = gc_frame_newslot(num1840);
  params[0] = num1840;
  partcv[0] = 1;
  Object sum1842 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1842
  *var_curPos = sum1842;
  if (sum1842 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_getBaseUrl1802(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct StackFrameObject *stackframe = alloc_StackFrame(5, NULL);
  pushclosure(NULL);
  pushstackframe(stackframe, "getBaseUrl");
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
    gracedie("missing argument list for getBaseUrl (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_importStatement = &(stackframe->slots[0]);
  *var_importStatement = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for getBaseUrl");
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
// Begin line 568
  setline(568);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 565
  setline(565);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1805 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1806 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1805);
// compilenode returning call1806
// compilenode returning call1806
  int op_slot_left_1804 = gc_frame_newslot(call1806);
  Object num1807 = alloc_Float64(7);
// compilenode returning num1807
  int op_slot_right_1804 = gc_frame_newslot(num1807);
  params[0] = num1807;
  partcv[0] = 1;
  Object opresult1809 = callmethod(call1806, "<", 1, partcv, params);
// compilenode returning opresult1809
  Object if1803 = done;
  if (istrue(opresult1809)) {
// Begin line 567
  setline(567);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 566
  setline(566);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
  return *var_importStatement;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1803 = undefined;
  } else {
  }
// compilenode returning if1803
// Begin line 584
  setline(584);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 568
  setline(568);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1812 = gc_frame_new();
  Object num1813 = alloc_Float64(1);
// compilenode returning num1813
  gc_frame_newslot(num1813);
  Object num1814 = alloc_Float64(7);
// compilenode returning num1814
  gc_frame_newslot(num1814);
// compilenode returning *var_importStatement
  params[0] = num1813;
  params[1] = num1814;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1815 = callmethod(*var_importStatement, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1812);
// compilenode returning call1815
  int op_slot_left_1811 = gc_frame_newslot(call1815);
  if (strlit1816 == NULL) {
    strlit1816 = alloc_String("http://");
    gc_root(strlit1816);
  }
// compilenode returning strlit1816
  int op_slot_right_1811 = gc_frame_newslot(strlit1816);
  params[0] = strlit1816;
  partcv[0] = 1;
  Object opresult1818 = callmethod(call1815, "==", 1, partcv, params);
// compilenode returning opresult1818
  Object if1810 = done;
  if (istrue(opresult1818)) {
// Begin line 570
  setline(570);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 569
  setline(569);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1819 = alloc_Float64(7);
// compilenode returning num1819
  *var_lastIndex = num1819;
  if (num1819 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 571
  setline(571);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 570
  setline(570);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1820 = alloc_Float64(7);
// compilenode returning num1820
  *var_curPos = num1820;
  if (num1820 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 577
  setline(577);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1821 = gc_frame_new();
// Begin line 571
  setline(571);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1822 = alloc_Block(NULL, NULL, "PackageManager", 571);
  gc_frame_newslot(block1822);
  block_savedest(block1822);
  Object closure1823 = createclosure(3, "_apply");
setclosureframe(closure1823, stackframe);
  addtoclosure(closure1823, var_curPos);
  addtoclosure(closure1823, var_importStatement);
  addtoclosure(closure1823, selfslot);
  struct UserObject *uo1823 = (struct UserObject*)block1822;
  uo1823->data[0] = (Object)closure1823;
  Method *meth_meth_PackageManager__apply1823 = addmethod2pos(block1822, "_apply", &meth_PackageManager__apply1823, 0);
int argcv_meth_PackageManager__apply1823[] = {0};
meth_meth_PackageManager__apply1823->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1823);
  meth_meth_PackageManager__apply1823->definitionModule = modulename;
  meth_meth_PackageManager__apply1823->definitionLine = 565;
// compilenode returning block1822
  gc_frame_newslot(block1822);
// Begin line 577
  setline(577);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1830 = alloc_Block(NULL, NULL, "PackageManager", 577);
  gc_frame_newslot(block1830);
  block_savedest(block1830);
  Object closure1831 = createclosure(4, "_apply");
setclosureframe(closure1831, stackframe);
  addtoclosure(closure1831, var_importStatement);
  addtoclosure(closure1831, var_curPos);
  addtoclosure(closure1831, var_lastIndex);
  addtoclosure(closure1831, selfslot);
  struct UserObject *uo1831 = (struct UserObject*)block1830;
  uo1831->data[0] = (Object)closure1831;
  Method *meth_meth_PackageManager__apply1831 = addmethod2pos(block1830, "_apply", &meth_PackageManager__apply1831, 0);
int argcv_meth_PackageManager__apply1831[] = {0};
meth_meth_PackageManager__apply1831->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1831);
  meth_meth_PackageManager__apply1831->definitionModule = modulename;
  meth_meth_PackageManager__apply1831->definitionLine = 571;
// compilenode returning block1830
  gc_frame_newslot(block1830);
  params[0] = block1822;
  params[1] = block1830;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1845 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1821);
// compilenode returning call1845
// Begin line 578
  setline(578);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1846 = gc_frame_new();
// Begin line 577
  setline(577);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1847 = alloc_Float64(1);
// compilenode returning num1847
  gc_frame_newslot(num1847);
// compilenode returning *var_lastIndex
  gc_frame_newslot(*var_lastIndex);
// compilenode returning *var_importStatement
  params[0] = num1847;
  params[1] = *var_lastIndex;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1848 = callmethod(*var_importStatement, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1846);
// compilenode returning call1848
  *var_res = call1848;
  if (call1848 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 579
  setline(579);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 578
  setline(578);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_res
  return *var_res;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1810 = undefined;
  } else {
// Begin line 582
  setline(582);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 581
  setline(581);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
  return *var_importStatement;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1810 = undefined;
  }
// compilenode returning if1810
  gc_frame_end(frame);
  return if1810;
}
Object meth_PackageManager__apply1856(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1857 = gc_frame_newslot(*var_count);
  int callframe1858 = gc_frame_new();
// compilenode returning *var_st
  partcv[0] = 0;
  Object call1859 = callmethod(*var_st, "size",
    1, partcv, params);
  gc_frame_end(callframe1858);
// compilenode returning call1859
// compilenode returning call1859
  int op_slot_right_1857 = gc_frame_newslot(call1859);
  params[0] = call1859;
  partcv[0] = 1;
  Object opresult1861 = callmethod(*var_count, "<=", 1, partcv, params);
// compilenode returning opresult1861
  gc_frame_end(frame);
  return opresult1861;
}
Object meth_PackageManager__apply1864(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 593
  setline(593);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 590
  setline(590);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_st
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1867 = callmethod(*var_st, "[]", 1, partcv, params);
// compilenode returning idxres1867
  int op_slot_left_1866 = gc_frame_newslot(idxres1867);
  if (strlit1868 == NULL) {
    strlit1868 = alloc_String("/");
    gc_root(strlit1868);
  }
// compilenode returning strlit1868
  int op_slot_right_1866 = gc_frame_newslot(strlit1868);
  params[0] = strlit1868;
  partcv[0] = 1;
  Object opresult1870 = callmethod(idxres1867, "==", 1, partcv, params);
// compilenode returning opresult1870
  Object if1865 = done;
  if (istrue(opresult1870)) {
// Begin line 592
  setline(592);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 591
  setline(591);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  *var_lastIndex = *var_count;
  if (*var_count == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if1865 = done;
  } else {
  }
// compilenode returning if1865
// Begin line 594
  setline(594);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 593
  setline(593);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  int op_slot_left_1872 = gc_frame_newslot(*var_count);
  Object num1873 = alloc_Float64(1);
// compilenode returning num1873
  int op_slot_right_1872 = gc_frame_newslot(num1873);
  params[0] = num1873;
  partcv[0] = 1;
  Object sum1875 = callmethod(*var_count, "+", 1, partcv, params);
// compilenode returning sum1875
  *var_count = sum1875;
  if (sum1875 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_removeContainingDir1849(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 588
  setline(588);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 587
  setline(587);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1850 = alloc_Float64(1);
// compilenode returning num1850
  *var_count = num1850;
  if (num1850 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 589
  setline(589);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1851 = gc_frame_new();
// Begin line 588
  setline(588);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1852 = alloc_Float64(1);
// compilenode returning num1852
  partcv[0] = 0;
  Object call1853 = callmethod(num1852, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1851);
// compilenode returning call1853
  *var_lastIndex = call1853;
  if (call1853 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 595
  setline(595);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1854 = gc_frame_new();
// Begin line 589
  setline(589);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1855 = alloc_Block(NULL, NULL, "PackageManager", 589);
  gc_frame_newslot(block1855);
  block_savedest(block1855);
  Object closure1856 = createclosure(3, "_apply");
setclosureframe(closure1856, stackframe);
  addtoclosure(closure1856, var_count);
  addtoclosure(closure1856, var_st);
  addtoclosure(closure1856, selfslot);
  struct UserObject *uo1856 = (struct UserObject*)block1855;
  uo1856->data[0] = (Object)closure1856;
  Method *meth_meth_PackageManager__apply1856 = addmethod2pos(block1855, "_apply", &meth_PackageManager__apply1856, 0);
int argcv_meth_PackageManager__apply1856[] = {0};
meth_meth_PackageManager__apply1856->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1856);
  meth_meth_PackageManager__apply1856->definitionModule = modulename;
  meth_meth_PackageManager__apply1856->definitionLine = 571;
// compilenode returning block1855
  gc_frame_newslot(block1855);
// Begin line 595
  setline(595);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1863 = alloc_Block(NULL, NULL, "PackageManager", 595);
  gc_frame_newslot(block1863);
  block_savedest(block1863);
  Object closure1864 = createclosure(4, "_apply");
setclosureframe(closure1864, stackframe);
  addtoclosure(closure1864, var_st);
  addtoclosure(closure1864, var_count);
  addtoclosure(closure1864, var_lastIndex);
  addtoclosure(closure1864, selfslot);
  struct UserObject *uo1864 = (struct UserObject*)block1863;
  uo1864->data[0] = (Object)closure1864;
  Method *meth_meth_PackageManager__apply1864 = addmethod2pos(block1863, "_apply", &meth_PackageManager__apply1864, 0);
int argcv_meth_PackageManager__apply1864[] = {0};
meth_meth_PackageManager__apply1864->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1864);
  meth_meth_PackageManager__apply1864->definitionModule = modulename;
  meth_meth_PackageManager__apply1864->definitionLine = 589;
// compilenode returning block1863
  gc_frame_newslot(block1863);
  params[0] = block1855;
  params[1] = block1863;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1878 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1854);
// compilenode returning call1878
// Begin line 598
  setline(598);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 595
  setline(595);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_lastIndex
  int op_slot_left_1880 = gc_frame_newslot(*var_lastIndex);
  int callframe1881 = gc_frame_new();
  Object num1882 = alloc_Float64(1);
// compilenode returning num1882
  partcv[0] = 0;
  Object call1883 = callmethod(num1882, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1881);
// compilenode returning call1883
  int op_slot_right_1880 = gc_frame_newslot(call1883);
  params[0] = call1883;
  partcv[0] = 1;
  Object opresult1885 = callmethod(*var_lastIndex, "==", 1, partcv, params);
// compilenode returning opresult1885
  Object if1879 = done;
  if (istrue(opresult1885)) {
// Begin line 597
  setline(597);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 596
  setline(596);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_st
  return *var_st;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1879 = undefined;
  } else {
  }
// compilenode returning if1879
// Begin line 598
  setline(598);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1886 = gc_frame_new();
// compilenode returning *var_lastIndex
  int op_slot_left_1887 = gc_frame_newslot(*var_lastIndex);
  Object num1888 = alloc_Float64(1);
// compilenode returning num1888
  int op_slot_right_1887 = gc_frame_newslot(num1888);
  params[0] = num1888;
  partcv[0] = 1;
  Object sum1890 = callmethod(*var_lastIndex, "+", 1, partcv, params);
// compilenode returning sum1890
  gc_frame_newslot(sum1890);
  int callframe1891 = gc_frame_new();
// compilenode returning *var_st
  partcv[0] = 0;
  Object call1892 = callmethod(*var_st, "size",
    1, partcv, params);
  gc_frame_end(callframe1891);
// compilenode returning call1892
// compilenode returning call1892
  gc_frame_newslot(call1892);
// compilenode returning *var_st
  params[0] = sum1890;
  params[1] = call1892;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1893 = callmethod(*var_st, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1886);
// compilenode returning call1893
  return call1893;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply1901(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1902 = gc_frame_newslot(*var_count);
  int callframe1903 = gc_frame_new();
// compilenode returning *var_st
  partcv[0] = 0;
  Object call1904 = callmethod(*var_st, "size",
    1, partcv, params);
  gc_frame_end(callframe1903);
// compilenode returning call1904
// compilenode returning call1904
  int op_slot_right_1902 = gc_frame_newslot(call1904);
  params[0] = call1904;
  partcv[0] = 1;
  Object opresult1906 = callmethod(*var_count, "<=", 1, partcv, params);
// compilenode returning opresult1906
  gc_frame_end(frame);
  return opresult1906;
}
Object meth_PackageManager__apply1909(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 608
  setline(608);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 605
  setline(605);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_st
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1912 = callmethod(*var_st, "[]", 1, partcv, params);
// compilenode returning idxres1912
  int op_slot_left_1911 = gc_frame_newslot(idxres1912);
  if (strlit1913 == NULL) {
    strlit1913 = alloc_String("/");
    gc_root(strlit1913);
  }
// compilenode returning strlit1913
  int op_slot_right_1911 = gc_frame_newslot(strlit1913);
  params[0] = strlit1913;
  partcv[0] = 1;
  Object opresult1915 = callmethod(idxres1912, "==", 1, partcv, params);
// compilenode returning opresult1915
  Object if1910 = done;
  if (istrue(opresult1915)) {
// Begin line 607
  setline(607);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 606
  setline(606);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  *var_last = *var_count;
  if (*var_count == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if1910 = done;
  } else {
  }
// compilenode returning if1910
// Begin line 609
  setline(609);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 608
  setline(608);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  int op_slot_left_1917 = gc_frame_newslot(*var_count);
  Object num1918 = alloc_Float64(1);
// compilenode returning num1918
  int op_slot_right_1917 = gc_frame_newslot(num1918);
  params[0] = num1918;
  partcv[0] = 1;
  Object sum1920 = callmethod(*var_count, "+", 1, partcv, params);
// compilenode returning sum1920
  *var_count = sum1920;
  if (sum1920 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_getContainingDirectory1894(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 603
  setline(603);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 602
  setline(602);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1895 = alloc_Float64(1);
// compilenode returning num1895
  *var_count = num1895;
  if (num1895 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 604
  setline(604);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1896 = gc_frame_new();
// Begin line 603
  setline(603);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1897 = alloc_Float64(1);
// compilenode returning num1897
  partcv[0] = 0;
  Object call1898 = callmethod(num1897, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1896);
// compilenode returning call1898
  *var_last = call1898;
  if (call1898 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 610
  setline(610);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1899 = gc_frame_new();
// Begin line 604
  setline(604);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1900 = alloc_Block(NULL, NULL, "PackageManager", 604);
  gc_frame_newslot(block1900);
  block_savedest(block1900);
  Object closure1901 = createclosure(3, "_apply");
setclosureframe(closure1901, stackframe);
  addtoclosure(closure1901, var_count);
  addtoclosure(closure1901, var_st);
  addtoclosure(closure1901, selfslot);
  struct UserObject *uo1901 = (struct UserObject*)block1900;
  uo1901->data[0] = (Object)closure1901;
  Method *meth_meth_PackageManager__apply1901 = addmethod2pos(block1900, "_apply", &meth_PackageManager__apply1901, 0);
int argcv_meth_PackageManager__apply1901[] = {0};
meth_meth_PackageManager__apply1901->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1901);
  meth_meth_PackageManager__apply1901->definitionModule = modulename;
  meth_meth_PackageManager__apply1901->definitionLine = 598;
// compilenode returning block1900
  gc_frame_newslot(block1900);
// Begin line 610
  setline(610);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1908 = alloc_Block(NULL, NULL, "PackageManager", 610);
  gc_frame_newslot(block1908);
  block_savedest(block1908);
  Object closure1909 = createclosure(4, "_apply");
setclosureframe(closure1909, stackframe);
  addtoclosure(closure1909, var_st);
  addtoclosure(closure1909, var_count);
  addtoclosure(closure1909, var_last);
  addtoclosure(closure1909, selfslot);
  struct UserObject *uo1909 = (struct UserObject*)block1908;
  uo1909->data[0] = (Object)closure1909;
  Method *meth_meth_PackageManager__apply1909 = addmethod2pos(block1908, "_apply", &meth_PackageManager__apply1909, 0);
int argcv_meth_PackageManager__apply1909[] = {0};
meth_meth_PackageManager__apply1909->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1909);
  meth_meth_PackageManager__apply1909->definitionModule = modulename;
  meth_meth_PackageManager__apply1909->definitionLine = 604;
// compilenode returning block1908
  gc_frame_newslot(block1908);
  params[0] = block1900;
  params[1] = block1908;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1923 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1899);
// compilenode returning call1923
// Begin line 613
  setline(613);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 610
  setline(610);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_last
  int op_slot_left_1925 = gc_frame_newslot(*var_last);
  int callframe1926 = gc_frame_new();
  Object num1927 = alloc_Float64(1);
// compilenode returning num1927
  partcv[0] = 0;
  Object call1928 = callmethod(num1927, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1926);
// compilenode returning call1928
  int op_slot_right_1925 = gc_frame_newslot(call1928);
  params[0] = call1928;
  partcv[0] = 1;
  Object opresult1930 = callmethod(*var_last, "==", 1, partcv, params);
// compilenode returning opresult1930
  Object if1924 = done;
  if (istrue(opresult1930)) {
// Begin line 612
  setline(612);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 611
  setline(611);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1932 = gc_frame_new();
  partcv[0] = 0;
  Object call1933 = callmethodflags(self, "getBuildPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1932);
// compilenode returning call1933
  int op_slot_left_1931 = gc_frame_newslot(call1933);
  if (strlit1934 == NULL) {
    strlit1934 = alloc_String("/");
    gc_root(strlit1934);
  }
// compilenode returning strlit1934
  int op_slot_right_1931 = gc_frame_newslot(strlit1934);
  params[0] = strlit1934;
  partcv[0] = 1;
  Object opresult1936 = callmethod(call1933, "++", 1, partcv, params);
// compilenode returning opresult1936
  return opresult1936;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1924 = undefined;
  } else {
  }
// compilenode returning if1924
// Begin line 615
  setline(615);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1937 = gc_frame_new();
// Begin line 613
  setline(613);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1938 = alloc_Float64(0);
// compilenode returning num1938
  gc_frame_newslot(num1938);
// compilenode returning *var_last
  int op_slot_left_1939 = gc_frame_newslot(*var_last);
  Object num1940 = alloc_Float64(1);
// compilenode returning num1940
  int op_slot_right_1939 = gc_frame_newslot(num1940);
  params[0] = num1940;
  partcv[0] = 1;
  Object diff1942 = callmethod(*var_last, "-", 1, partcv, params);
// compilenode returning diff1942
  gc_frame_newslot(diff1942);
// compilenode returning *var_st
  params[0] = num1938;
  params[1] = diff1942;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1943 = callmethod(*var_st, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1937);
// compilenode returning call1943
  return call1943;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply2064(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  Object *var_buildPath = getfromclosure(closure, 0);
  Object *var_io = getfromclosure(closure, 1);
  Object self = *(getfromclosure(closure, 2));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *var_s = &(stackframe->slots[0]);
  *var_s = args[curarg];
  curarg++;
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 639
  setline(639);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe2065 = gc_frame_new();
// Begin line 638
  setline(638);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit2070 == NULL) {
    strlit2070 = alloc_String("rm ");
    gc_root(strlit2070);
  }
// compilenode returning strlit2070
  int op_slot_left_2069 = gc_frame_newslot(strlit2070);
// compilenode returning *var_buildPath
  int op_slot_right_2069 = gc_frame_newslot(*var_buildPath);
  params[0] = *var_buildPath;
  partcv[0] = 1;
  Object opresult2072 = callmethod(strlit2070, "++", 1, partcv, params);
// compilenode returning opresult2072
  int op_slot_left_2068 = gc_frame_newslot(opresult2072);
  if (strlit2073 == NULL) {
    strlit2073 = alloc_String("/__pkg-temp");
    gc_root(strlit2073);
  }
// compilenode returning strlit2073
  int op_slot_right_2068 = gc_frame_newslot(strlit2073);
  params[0] = strlit2073;
  partcv[0] = 1;
  Object opresult2075 = callmethod(opresult2072, "++", 1, partcv, params);
// compilenode returning opresult2075
  int op_slot_left_2067 = gc_frame_newslot(opresult2075);
// compilenode returning *var_s
  int op_slot_right_2067 = gc_frame_newslot(*var_s);
  params[0] = *var_s;
  partcv[0] = 1;
  Object opresult2077 = callmethod(opresult2075, "++", 1, partcv, params);
// compilenode returning opresult2077
  int op_slot_left_2066 = gc_frame_newslot(opresult2077);
  if (strlit2078 == NULL) {
    strlit2078 = alloc_String("");
    gc_root(strlit2078);
  }
// compilenode returning strlit2078
  int op_slot_right_2066 = gc_frame_newslot(strlit2078);
  params[0] = strlit2078;
  partcv[0] = 1;
  Object opresult2080 = callmethod(opresult2077, "++", 1, partcv, params);
// compilenode returning opresult2080
  gc_frame_newslot(opresult2080);
// compilenode returning *var_io
  params[0] = opresult2080;
  partcv[0] = 1;
  Object call2081 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe2065);
// compilenode returning call2081
  gc_frame_end(frame);
  return call2081;
}
Object meth_PackageManager_package1944(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 32, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(9, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "package");
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
    gracedie("missing argument list for package (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_folder = &(stackframe->slots[0]);
  *var_folder = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for package");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_setupFile = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "setupFile");
  Object *var_buildPath = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "buildPath");
  Object *var_open = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "open");
  Object *var_pkgData = &(stackframe->slots[5]);
  setframeelementname(stackframe, 5, "pkgData");
  Object *var_loc = &(stackframe->slots[6]);
  setframeelementname(stackframe, 6, "loc");
  Object *var_create = &(stackframe->slots[7]);
  setframeelementname(stackframe, 7, "create");
  Object *var_suffix = &(stackframe->slots[8]);
  setframeelementname(stackframe, 8, "suffix");
// Begin line 619
  setline(619);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 618
  setline(618);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1945 == NULL) {
    strlit1945 = alloc_String("~/Packagemanager/setup.grace");
    gc_root(strlit1945);
  }
// compilenode returning strlit1945
  *var_setupFile = strlit1945;
  if (strlit1945 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 620
  setline(620);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1946 = gc_frame_new();
  partcv[0] = 0;
  Object call1947 = callmethodflags(self, "getBuildPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1946);
// compilenode returning call1947
  *var_buildPath = call1947;
  if (call1947 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 623
  setline(623);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 620
  setline(620);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1949 = gc_frame_new();
  int callframe1950 = gc_frame_new();
// compilenode returning *var_folder
  int op_slot_left_1951 = gc_frame_newslot(*var_folder);
  if (strlit1952 == NULL) {
    strlit1952 = alloc_String("/pkg.grace");
    gc_root(strlit1952);
  }
// compilenode returning strlit1952
  int op_slot_right_1951 = gc_frame_newslot(strlit1952);
  params[0] = strlit1952;
  partcv[0] = 1;
  Object opresult1954 = callmethod(*var_folder, "++", 1, partcv, params);
// compilenode returning opresult1954
  gc_frame_newslot(opresult1954);
// compilenode returning *var_io
  params[0] = opresult1954;
  partcv[0] = 1;
  Object call1955 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe1950);
// compilenode returning call1955
  partcv[0] = 0;
  Object call1956 = callmethod(call1955, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1949);
// compilenode returning call1956
  Object if1948 = done;
  if (istrue(call1956)) {
// Begin line 622
  setline(622);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 621
  setline(621);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1957 == NULL) {
    strlit1957 = alloc_String("Folder must contain pkg.grace file in order to create package");
    gc_root(strlit1957);
  }
// compilenode returning strlit1957
  params[0] = strlit1957;
  Object call1958 = gracelib_print(NULL, 1,  params);
// compilenode returning call1958
    gc_frame_newslot(call1958);
    if1948 = call1958;
  } else {
  }
// compilenode returning if1948
// Begin line 624
  setline(624);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1959 = gc_frame_new();
// Begin line 623
  setline(623);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1962 == NULL) {
    strlit1962 = alloc_String("");
    gc_root(strlit1962);
  }
// compilenode returning strlit1962
  int op_slot_left_1961 = gc_frame_newslot(strlit1962);
// compilenode returning *var_folder
  int op_slot_right_1961 = gc_frame_newslot(*var_folder);
  params[0] = *var_folder;
  partcv[0] = 1;
  Object opresult1964 = callmethod(strlit1962, "++", 1, partcv, params);
// compilenode returning opresult1964
  int op_slot_left_1960 = gc_frame_newslot(opresult1964);
  if (strlit1965 == NULL) {
    strlit1965 = alloc_String("/pkg.grace");
    gc_root(strlit1965);
  }
// compilenode returning strlit1965
  int op_slot_right_1960 = gc_frame_newslot(strlit1965);
  params[0] = strlit1965;
  partcv[0] = 1;
  Object opresult1967 = callmethod(opresult1964, "++", 1, partcv, params);
// compilenode returning opresult1967
  gc_frame_newslot(opresult1967);
  if (strlit1968 == NULL) {
    strlit1968 = alloc_String("r");
    gc_root(strlit1968);
  }
// compilenode returning strlit1968
  gc_frame_newslot(strlit1968);
// compilenode returning *var_io
  params[0] = opresult1967;
  params[1] = strlit1968;
  partcv[0] = 2;
  Object call1969 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1959);
// compilenode returning call1969
  *var_open = call1969;
  if (call1969 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 625
  setline(625);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 624
  setline(624);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1970 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call1971 = callmethod(*var_open, "read",
    1, partcv, params);
  gc_frame_end(callframe1970);
// compilenode returning call1971
// compilenode returning call1971
  *var_pkgData = call1971;
  if (call1971 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 625
  setline(625);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1972 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call1973 = callmethod(*var_open, "close",
    1, partcv, params);
  gc_frame_end(callframe1972);
// compilenode returning call1973
// compilenode returning call1973
// Begin line 627
  setline(627);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 626
  setline(626);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1976 == NULL) {
    strlit1976 = alloc_String("");
    gc_root(strlit1976);
  }
// compilenode returning strlit1976
  int op_slot_left_1975 = gc_frame_newslot(strlit1976);
// compilenode returning *var_buildPath
  int op_slot_right_1975 = gc_frame_newslot(*var_buildPath);
  params[0] = *var_buildPath;
  partcv[0] = 1;
  Object opresult1978 = callmethod(strlit1976, "++", 1, partcv, params);
// compilenode returning opresult1978
  int op_slot_left_1974 = gc_frame_newslot(opresult1978);
  if (strlit1979 == NULL) {
    strlit1979 = alloc_String("/__pkg-temp");
    gc_root(strlit1979);
  }
// compilenode returning strlit1979
  int op_slot_right_1974 = gc_frame_newslot(strlit1979);
  params[0] = strlit1979;
  partcv[0] = 1;
  Object opresult1981 = callmethod(opresult1978, "++", 1, partcv, params);
// compilenode returning opresult1981
  *var_loc = opresult1981;
  if (opresult1981 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 628
  setline(628);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1982 = gc_frame_new();
// Begin line 627
  setline(627);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1985 == NULL) {
    strlit1985 = alloc_String("");
    gc_root(strlit1985);
  }
// compilenode returning strlit1985
  int op_slot_left_1984 = gc_frame_newslot(strlit1985);
// compilenode returning *var_loc
  int op_slot_right_1984 = gc_frame_newslot(*var_loc);
  params[0] = *var_loc;
  partcv[0] = 1;
  Object opresult1987 = callmethod(strlit1985, "++", 1, partcv, params);
// compilenode returning opresult1987
  int op_slot_left_1983 = gc_frame_newslot(opresult1987);
  if (strlit1988 == NULL) {
    strlit1988 = alloc_String("");
    gc_root(strlit1988);
  }
// compilenode returning strlit1988
  int op_slot_right_1983 = gc_frame_newslot(strlit1988);
  params[0] = strlit1988;
  partcv[0] = 1;
  Object opresult1990 = callmethod(opresult1987, "++", 1, partcv, params);
// compilenode returning opresult1990
  gc_frame_newslot(opresult1990);
  if (strlit1991 == NULL) {
    strlit1991 = alloc_String("w");
    gc_root(strlit1991);
  }
// compilenode returning strlit1991
  gc_frame_newslot(strlit1991);
// compilenode returning *var_io
  params[0] = opresult1990;
  params[1] = strlit1991;
  partcv[0] = 2;
  Object call1992 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1982);
// compilenode returning call1992
  *var_create = call1992;
  if (call1992 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 629
  setline(629);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 628
  setline(628);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pkgData
  int op_slot_left_1993 = gc_frame_newslot(*var_pkgData);
  if (strlit1994 == NULL) {
    strlit1994 = alloc_String("""\x0a""def __bundle = true");
    gc_root(strlit1994);
  }
// compilenode returning strlit1994
  int op_slot_right_1993 = gc_frame_newslot(strlit1994);
  params[0] = strlit1994;
  partcv[0] = 1;
  Object opresult1996 = callmethod(*var_pkgData, "++", 1, partcv, params);
// compilenode returning opresult1996
  *var_pkgData = opresult1996;
  if (opresult1996 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 630
  setline(630);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 629
  setline(629);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pkgData
  int op_slot_left_1998 = gc_frame_newslot(*var_pkgData);
  if (strlit2001 == NULL) {
    strlit2001 = alloc_String("""\x0a""def __loc = ""\x22""");
    gc_root(strlit2001);
  }
// compilenode returning strlit2001
  int op_slot_left_2000 = gc_frame_newslot(strlit2001);
// compilenode returning *var_folder
  int op_slot_right_2000 = gc_frame_newslot(*var_folder);
  params[0] = *var_folder;
  partcv[0] = 1;
  Object opresult2003 = callmethod(strlit2001, "++", 1, partcv, params);
// compilenode returning opresult2003
  int op_slot_left_1999 = gc_frame_newslot(opresult2003);
  if (strlit2004 == NULL) {
    strlit2004 = alloc_String("""\x22""");
    gc_root(strlit2004);
  }
// compilenode returning strlit2004
  int op_slot_right_1999 = gc_frame_newslot(strlit2004);
  params[0] = strlit2004;
  partcv[0] = 1;
  Object opresult2006 = callmethod(opresult2003, "++", 1, partcv, params);
// compilenode returning opresult2006
  int op_slot_right_1998 = gc_frame_newslot(opresult2006);
  params[0] = opresult2006;
  partcv[0] = 1;
  Object opresult2008 = callmethod(*var_pkgData, "++", 1, partcv, params);
// compilenode returning opresult2008
  *var_pkgData = opresult2008;
  if (opresult2008 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe2010 = gc_frame_new();
// Begin line 630
  setline(630);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pkgData
  gc_frame_newslot(*var_pkgData);
// compilenode returning *var_create
  params[0] = *var_pkgData;
  partcv[0] = 1;
  Object call2011 = callmethod(*var_create, "write",
    1, partcv, params);
  gc_frame_end(callframe2010);
// compilenode returning call2011
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe2012 = gc_frame_new();
// compilenode returning *var_create
  partcv[0] = 0;
  Object call2013 = callmethod(*var_create, "close",
    1, partcv, params);
  gc_frame_end(callframe2012);
// compilenode returning call2013
// compilenode returning call2013
// Begin line 633
  setline(633);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe2014 = gc_frame_new();
// Begin line 632
  setline(632);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit2019 == NULL) {
    strlit2019 = alloc_String("mv ");
    gc_root(strlit2019);
  }
// compilenode returning strlit2019
  int op_slot_left_2018 = gc_frame_newslot(strlit2019);
// compilenode returning *var_loc
  int op_slot_right_2018 = gc_frame_newslot(*var_loc);
  params[0] = *var_loc;
  partcv[0] = 1;
  Object opresult2021 = callmethod(strlit2019, "++", 1, partcv, params);
// compilenode returning opresult2021
  int op_slot_left_2017 = gc_frame_newslot(opresult2021);
  if (strlit2022 == NULL) {
    strlit2022 = alloc_String(" ");
    gc_root(strlit2022);
  }
// compilenode returning strlit2022
  int op_slot_right_2017 = gc_frame_newslot(strlit2022);
  params[0] = strlit2022;
  partcv[0] = 1;
  Object opresult2024 = callmethod(opresult2021, "++", 1, partcv, params);
// compilenode returning opresult2024
  int op_slot_left_2016 = gc_frame_newslot(opresult2024);
// compilenode returning *var_loc
  int op_slot_right_2016 = gc_frame_newslot(*var_loc);
  params[0] = *var_loc;
  partcv[0] = 1;
  Object opresult2026 = callmethod(opresult2024, "++", 1, partcv, params);
// compilenode returning opresult2026
  int op_slot_left_2015 = gc_frame_newslot(opresult2026);
  if (strlit2027 == NULL) {
    strlit2027 = alloc_String(".grace");
    gc_root(strlit2027);
  }
// compilenode returning strlit2027
  int op_slot_right_2015 = gc_frame_newslot(strlit2027);
  params[0] = strlit2027;
  partcv[0] = 1;
  Object opresult2029 = callmethod(opresult2026, "++", 1, partcv, params);
// compilenode returning opresult2029
  gc_frame_newslot(opresult2029);
// compilenode returning *var_io
  params[0] = opresult2029;
  partcv[0] = 1;
  Object call2030 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe2014);
// compilenode returning call2030
// Begin line 634
  setline(634);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 633
  setline(633);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit2033 == NULL) {
    strlit2033 = alloc_String("build path = ");
    gc_root(strlit2033);
  }
// compilenode returning strlit2033
  int op_slot_left_2032 = gc_frame_newslot(strlit2033);
// compilenode returning *var_buildPath
  int op_slot_right_2032 = gc_frame_newslot(*var_buildPath);
  params[0] = *var_buildPath;
  partcv[0] = 1;
  Object opresult2035 = callmethod(strlit2033, "++", 1, partcv, params);
// compilenode returning opresult2035
  int op_slot_left_2031 = gc_frame_newslot(opresult2035);
  if (strlit2036 == NULL) {
    strlit2036 = alloc_String(" ++ minigrace");
    gc_root(strlit2036);
  }
// compilenode returning strlit2036
  int op_slot_right_2031 = gc_frame_newslot(strlit2036);
  params[0] = strlit2036;
  partcv[0] = 1;
  Object opresult2038 = callmethod(opresult2035, "++", 1, partcv, params);
// compilenode returning opresult2038
  params[0] = opresult2038;
  Object call2039 = gracelib_print(NULL, 1,  params);
// compilenode returning call2039
// Begin line 636
  setline(636);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe2040 = gc_frame_new();
// Begin line 634
  setline(634);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit2045 == NULL) {
    strlit2045 = alloc_String("cat ");
    gc_root(strlit2045);
  }
// compilenode returning strlit2045
  int op_slot_left_2044 = gc_frame_newslot(strlit2045);
// compilenode returning *var_setupFile
  int op_slot_right_2044 = gc_frame_newslot(*var_setupFile);
  params[0] = *var_setupFile;
  partcv[0] = 1;
  Object opresult2047 = callmethod(strlit2045, "++", 1, partcv, params);
// compilenode returning opresult2047
  int op_slot_left_2043 = gc_frame_newslot(opresult2047);
  if (strlit2048 == NULL) {
    strlit2048 = alloc_String(" | ");
    gc_root(strlit2048);
  }
// compilenode returning strlit2048
  int op_slot_right_2043 = gc_frame_newslot(strlit2048);
  params[0] = strlit2048;
  partcv[0] = 1;
  Object opresult2050 = callmethod(opresult2047, "++", 1, partcv, params);
// compilenode returning opresult2050
  int op_slot_left_2042 = gc_frame_newslot(opresult2050);
// compilenode returning *var_buildPath
  int op_slot_right_2042 = gc_frame_newslot(*var_buildPath);
  params[0] = *var_buildPath;
  partcv[0] = 1;
  Object opresult2052 = callmethod(opresult2050, "++", 1, partcv, params);
// compilenode returning opresult2052
  int op_slot_left_2041 = gc_frame_newslot(opresult2052);
  if (strlit2053 == NULL) {
    strlit2053 = alloc_String("/minigrace");
    gc_root(strlit2053);
  }
// compilenode returning strlit2053
  int op_slot_right_2041 = gc_frame_newslot(strlit2053);
  params[0] = strlit2053;
  partcv[0] = 1;
  Object opresult2055 = callmethod(opresult2052, "++", 1, partcv, params);
// compilenode returning opresult2055
  gc_frame_newslot(opresult2055);
// compilenode returning *var_io
  params[0] = opresult2055;
  partcv[0] = 1;
  Object call2056 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe2040);
// compilenode returning call2056
// Begin line 636
  setline(636);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array2057 = alloc_BuiltinList();
  gc_pause();
  if (strlit2058 == NULL) {
    strlit2058 = alloc_String(".grace");
    gc_root(strlit2058);
  }
// compilenode returning strlit2058
  params[0] = strlit2058;
  partcv[0] = 1;
  callmethod(array2057, "push", 1, partcv, params);
  if (strlit2059 == NULL) {
    strlit2059 = alloc_String(".gct");
    gc_root(strlit2059);
  }
// compilenode returning strlit2059
  params[0] = strlit2059;
  partcv[0] = 1;
  callmethod(array2057, "push", 1, partcv, params);
  if (strlit2060 == NULL) {
    strlit2060 = alloc_String(".gcn");
    gc_root(strlit2060);
  }
// compilenode returning strlit2060
  params[0] = strlit2060;
  partcv[0] = 1;
  callmethod(array2057, "push", 1, partcv, params);
  if (strlit2061 == NULL) {
    strlit2061 = alloc_String(".c");
    gc_root(strlit2061);
  }
// compilenode returning strlit2061
  params[0] = strlit2061;
  partcv[0] = 1;
  callmethod(array2057, "push", 1, partcv, params);
  gc_unpause();
// compilenode returning array2057
  *var_suffix = array2057;
  if (array2057 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 641
  setline(641);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe2062 = gc_frame_new();
// Begin line 637
  setline(637);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_suffix
  gc_frame_newslot(*var_suffix);
// Begin line 641
  setline(641);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block2063 = alloc_Block(NULL, NULL, "PackageManager", 641);
  gc_frame_newslot(block2063);
  block_savedest(block2063);
  Object closure2064 = createclosure(3, "_apply");
setclosureframe(closure2064, stackframe);
  addtoclosure(closure2064, var_buildPath);
  addtoclosure(closure2064, var_io);
  addtoclosure(closure2064, selfslot);
  struct UserObject *uo2064 = (struct UserObject*)block2063;
  uo2064->data[0] = (Object)closure2064;
  Method *meth_meth_PackageManager__apply2064 = addmethod2pos(block2063, "_apply", &meth_PackageManager__apply2064, 0);
int argcv_meth_PackageManager__apply2064[] = {1};
meth_meth_PackageManager__apply2064->type = alloc_MethodType(1, argcv_meth_PackageManager__apply2064);
  meth_meth_PackageManager__apply2064->definitionModule = modulename;
  meth_meth_PackageManager__apply2064->definitionLine = 631;
// compilenode returning block2063
  gc_frame_newslot(block2063);
  params[0] = *var_suffix;
  params[1] = block2063;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call2083 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe2062);
// compilenode returning call2083
  gc_frame_end(frame);
  return call2083;
}
Object meth_PackageManager_imported2084(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 33, (flags>>24)&0xff);
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
Object meth_PackageManager_imported_58__61_2085(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 34, (flags>>24)&0xff);
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
Object meth_PackageManager_toProcess2088(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 35, (flags>>24)&0xff);
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
Object meth_PackageManager_toProcess_58__61_2089(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 36, (flags>>24)&0xff);
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
Object meth_PackageManager_verbose2092(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 37, (flags>>24)&0xff);
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
Object meth_PackageManager_verbose_58__61_2093(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 38, (flags>>24)&0xff);
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
Object meth_PackageManager_global2096(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 39, (flags>>24)&0xff);
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
Object meth_PackageManager_global_58__61_2097(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 40, (flags>>24)&0xff);
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
Object meth_PackageManager_bundlePath2100(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 41, (flags>>24)&0xff);
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
Object meth_PackageManager_bundlePath_58__61_2101(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 42, (flags>>24)&0xff);
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
Object meth_PackageManager_baseUrl2104(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 43, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "baseUrl");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_baseUrl = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for baseUrl (probably reflection error): got %i lists, expected 1.", nparts);
if (argcv && argcv[0] > 0)
  gracedie("too many arguments for <Identifier[baseUrl]>");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 10
  setline(10);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_baseUrl
  gc_frame_end(frame);
  return *var_baseUrl;
}
Object meth_PackageManager_baseUrl_58__61_2105(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 44, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "baseUrl:=");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_baseUrl = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for baseUrl:= (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var__var_assign_tmp = &(stackframe->slots[0]);
  *var__var_assign_tmp = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for <Identifier[baseUrl:=]>");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var__var_assign_tmp
  *var_baseUrl = *var__var_assign_tmp;
  if (*var__var_assign_tmp == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_curFile2107(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 45, (flags>>24)&0xff);
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
// Begin line 11
  setline(11);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curFile
  gc_frame_end(frame);
  return *var_curFile;
}
Object meth_PackageManager_curFile_58__61_2108(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 46, (flags>>24)&0xff);
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
Object meth_PackageManager_build2111(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 47, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "build");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_build = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for build (probably reflection error): got %i lists, expected 1.", nparts);
if (argcv && argcv[0] > 0)
  gracedie("too many arguments for <Identifier[build]>");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 12
  setline(12);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_build
  gc_frame_end(frame);
  return *var_build;
}
Object meth_PackageManager_build_58__61_2112(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 48, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "build:=");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_build = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for build:= (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var__var_assign_tmp = &(stackframe->slots[0]);
  *var__var_assign_tmp = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for <Identifier[build:=]>");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var__var_assign_tmp
  *var_build = *var__var_assign_tmp;
  if (*var__var_assign_tmp == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_install2115(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 49, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "install");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_install = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for install (probably reflection error): got %i lists, expected 1.", nparts);
if (argcv && argcv[0] > 0)
  gracedie("too many arguments for <Identifier[install]>");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 13
  setline(13);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_install
  gc_frame_end(frame);
  return *var_install;
}
Object meth_PackageManager_install_58__61_2116(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 50, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "install:=");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_install = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for install:= (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var__var_assign_tmp = &(stackframe->slots[0]);
  *var__var_assign_tmp = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for <Identifier[install:=]>");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var__var_assign_tmp
  *var_install = *var__var_assign_tmp;
  if (*var__var_assign_tmp == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object module_PackageManager_init() {
  int flags = 0;
  int frame = gc_frame_new();
  Object self = alloc_obj2(53, 53);
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
  struct StackFrameObject *stackframe = alloc_StackFrame(53, NULL);
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
  Object *var_baseUrl = &(stackframe->slots[6]);
  setframeelementname(stackframe, 6, "baseUrl");
  Object *var_curFile = &(stackframe->slots[7]);
  setframeelementname(stackframe, 7, "curFile");
  Object *var_build = &(stackframe->slots[8]);
  setframeelementname(stackframe, 8, "build");
  Object *var_install = &(stackframe->slots[9]);
  setframeelementname(stackframe, 9, "install");
// Begin line 17
  setline(17);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure0 = createclosure(4, "parseArgs");
setclosureframe(closure0, stackframe);
  addtoclosure(closure0, var_verbose);
  addtoclosure(closure0, var_global);
  addtoclosure(closure0, var_build);
  addtoclosure(closure0, var_install);
  struct UserObject *uo0 = (struct UserObject*)self;
  uo0->data[1] = (Object)closure0;
  Method *meth_meth_PackageManager_parseArgs0 = addmethod2pos(self, "parseArgs", &meth_PackageManager_parseArgs0, 1);
int argcv_meth_PackageManager_parseArgs0[] = {1};
meth_meth_PackageManager_parseArgs0->type = alloc_MethodType(1, argcv_meth_PackageManager_parseArgs0);
  meth_meth_PackageManager_parseArgs0->definitionModule = modulename;
  meth_meth_PackageManager_parseArgs0->definitionLine = 17;
// compilenode returning 
// Begin line 56
  setline(56);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo84 = (struct UserObject*)self;
  uo84->data[2] = emptyclosure;
  Method *meth_meth_PackageManager_forArgs_40__41_do84 = addmethod2pos(self, "forArgs()do", &meth_PackageManager_forArgs_40__41_do84, 2);
int argcv_meth_PackageManager_forArgs_40__41_do84[] = {1, 1};
meth_meth_PackageManager_forArgs_40__41_do84->type = alloc_MethodType(2, argcv_meth_PackageManager_forArgs_40__41_do84);
  meth_meth_PackageManager_forArgs_40__41_do84->flags |= MFLAG_CONFIDENTIAL;
  meth_meth_PackageManager_forArgs_40__41_do84->definitionModule = modulename;
  meth_meth_PackageManager_forArgs_40__41_do84->definitionLine = 56;
  Method *meth_meth_PackageManager_forArgs_40__41_do84_object = addmethod2pos(self, "forArgs()do()object", &meth_PackageManager_forArgs_40__41_do84_object, 2);
int argcv_meth_PackageManager_forArgs_40__41_do84_object[] = {1, 1};
meth_meth_PackageManager_forArgs_40__41_do84_object->type = alloc_MethodType(2, argcv_meth_PackageManager_forArgs_40__41_do84_object);
  meth_meth_PackageManager_forArgs_40__41_do84_object->flags |= MFLAG_CONFIDENTIAL;
  meth_meth_PackageManager_forArgs_40__41_do84_object->definitionModule = modulename;
  meth_meth_PackageManager_forArgs_40__41_do84_object->definitionLine = 56;
// compilenode returning 
// Begin line 111
  setline(111);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure237 = createclosure(1, "listInstalled");
setclosureframe(closure237, stackframe);
  addtoclosure(closure237, var_sys);
  struct UserObject *uo237 = (struct UserObject*)self;
  uo237->data[3] = (Object)closure237;
  Method *meth_meth_PackageManager_listInstalled237 = addmethod2pos(self, "listInstalled", &meth_PackageManager_listInstalled237, 3);
int argcv_meth_PackageManager_listInstalled237[] = {0};
meth_meth_PackageManager_listInstalled237->type = alloc_MethodType(1, argcv_meth_PackageManager_listInstalled237);
  meth_meth_PackageManager_listInstalled237->definitionModule = modulename;
  meth_meth_PackageManager_listInstalled237->definitionLine = 111;
// compilenode returning 
// Begin line 119
  setline(119);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure266 = createclosure(1, "checkListPath");
setclosureframe(closure266, stackframe);
  addtoclosure(closure266, var_io);
  struct UserObject *uo266 = (struct UserObject*)self;
  uo266->data[4] = (Object)closure266;
  Method *meth_meth_PackageManager_checkListPath266 = addmethod2pos(self, "checkListPath", &meth_PackageManager_checkListPath266, 4);
int argcv_meth_PackageManager_checkListPath266[] = {1};
meth_meth_PackageManager_checkListPath266->type = alloc_MethodType(1, argcv_meth_PackageManager_checkListPath266);
meth_meth_PackageManager_checkListPath266->type->types[0] = type_String;
meth_meth_PackageManager_checkListPath266->type->names[0] = "path";
  meth_meth_PackageManager_checkListPath266->definitionModule = modulename;
  meth_meth_PackageManager_checkListPath266->definitionLine = 119;
// compilenode returning 
// Begin line 127
  setline(127);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure282 = createclosure(1, "recurseDirectory");
setclosureframe(closure282, stackframe);
  addtoclosure(closure282, var_io);
  struct UserObject *uo282 = (struct UserObject*)self;
  uo282->data[5] = (Object)closure282;
  Method *meth_meth_PackageManager_recurseDirectory282 = addmethod2pos(self, "recurseDirectory", &meth_PackageManager_recurseDirectory282, 5);
int argcv_meth_PackageManager_recurseDirectory282[] = {2};
meth_meth_PackageManager_recurseDirectory282->type = alloc_MethodType(1, argcv_meth_PackageManager_recurseDirectory282);
  meth_meth_PackageManager_recurseDirectory282->definitionModule = modulename;
  meth_meth_PackageManager_recurseDirectory282->definitionLine = 127;
// compilenode returning 
// Begin line 139
  setline(139);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure335 = createclosure(2, "doGet");
setclosureframe(closure335, stackframe);
  addtoclosure(closure335, var_imported);
  addtoclosure(closure335, var_done);
  struct UserObject *uo335 = (struct UserObject*)self;
  uo335->data[6] = (Object)closure335;
  Method *meth_meth_PackageManager_doGet335 = addmethod2pos(self, "doGet", &meth_PackageManager_doGet335, 6);
int argcv_meth_PackageManager_doGet335[] = {1};
meth_meth_PackageManager_doGet335->type = alloc_MethodType(1, argcv_meth_PackageManager_doGet335);
  meth_meth_PackageManager_doGet335->definitionModule = modulename;
  meth_meth_PackageManager_doGet335->definitionLine = 139;
// compilenode returning 
// Begin line 159
  setline(159);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo391 = (struct UserObject*)self;
  uo391->data[7] = emptyclosure;
  Method *meth_meth_PackageManager_getPackage391 = addmethod2pos(self, "getPackage", &meth_PackageManager_getPackage391, 7);
int argcv_meth_PackageManager_getPackage391[] = {1};
meth_meth_PackageManager_getPackage391->type = alloc_MethodType(1, argcv_meth_PackageManager_getPackage391);
  meth_meth_PackageManager_getPackage391->definitionModule = modulename;
  meth_meth_PackageManager_getPackage391->definitionLine = 159;
// compilenode returning 
// Begin line 166
  setline(166);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure392 = createclosure(1, "setBaseUrl");
setclosureframe(closure392, stackframe);
  addtoclosure(closure392, var_baseUrl);
  struct UserObject *uo392 = (struct UserObject*)self;
  uo392->data[8] = (Object)closure392;
  Method *meth_meth_PackageManager_setBaseUrl392 = addmethod2pos(self, "setBaseUrl", &meth_PackageManager_setBaseUrl392, 8);
int argcv_meth_PackageManager_setBaseUrl392[] = {1};
meth_meth_PackageManager_setBaseUrl392->type = alloc_MethodType(1, argcv_meth_PackageManager_setBaseUrl392);
meth_meth_PackageManager_setBaseUrl392->type->types[0] = type_String;
meth_meth_PackageManager_setBaseUrl392->type->names[0] = "baseAddress";
  meth_meth_PackageManager_setBaseUrl392->definitionModule = modulename;
  meth_meth_PackageManager_setBaseUrl392->definitionLine = 166;
// compilenode returning 
// Begin line 170
  setline(170);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure396 = createclosure(1, "setFile");
setclosureframe(closure396, stackframe);
  addtoclosure(closure396, var_curFile);
  struct UserObject *uo396 = (struct UserObject*)self;
  uo396->data[9] = (Object)closure396;
  Method *meth_meth_PackageManager_setFile396 = addmethod2pos(self, "setFile", &meth_PackageManager_setFile396, 9);
int argcv_meth_PackageManager_setFile396[] = {1};
meth_meth_PackageManager_setFile396->type = alloc_MethodType(1, argcv_meth_PackageManager_setFile396);
  meth_meth_PackageManager_setFile396->definitionModule = modulename;
  meth_meth_PackageManager_setFile396->definitionLine = 170;
// compilenode returning 
// Begin line 182
  setline(182);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure419 = createclosure(3, "fetchImports");
setclosureframe(closure419, stackframe);
  addtoclosure(closure419, var_curFile);
  addtoclosure(closure419, var_imported);
  addtoclosure(closure419, var_toProcess);
  struct UserObject *uo419 = (struct UserObject*)self;
  uo419->data[10] = (Object)closure419;
  Method *meth_meth_PackageManager_fetchImports419 = addmethod2pos(self, "fetchImports", &meth_PackageManager_fetchImports419, 10);
int argcv_meth_PackageManager_fetchImports419[] = {1};
meth_meth_PackageManager_fetchImports419->type = alloc_MethodType(1, argcv_meth_PackageManager_fetchImports419);
  meth_meth_PackageManager_fetchImports419->definitionModule = modulename;
  meth_meth_PackageManager_fetchImports419->definitionLine = 182;
// compilenode returning 
// Begin line 207
  setline(207);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure495 = createclosure(1, "performCurlFetch");
setclosureframe(closure495, stackframe);
  addtoclosure(closure495, var_curl);
  struct UserObject *uo495 = (struct UserObject*)self;
  uo495->data[11] = (Object)closure495;
  Method *meth_meth_PackageManager_performCurlFetch495 = addmethod2pos(self, "performCurlFetch", &meth_PackageManager_performCurlFetch495, 11);
int argcv_meth_PackageManager_performCurlFetch495[] = {1};
meth_meth_PackageManager_performCurlFetch495->type = alloc_MethodType(1, argcv_meth_PackageManager_performCurlFetch495);
  meth_meth_PackageManager_performCurlFetch495->definitionModule = modulename;
  meth_meth_PackageManager_performCurlFetch495->definitionLine = 207;
// compilenode returning 
// Begin line 224
  setline(224);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure554 = createclosure(1, "setFileData");
setclosureframe(closure554, stackframe);
  addtoclosure(closure554, var_baseUrl);
  struct UserObject *uo554 = (struct UserObject*)self;
  uo554->data[12] = (Object)closure554;
  Method *meth_meth_PackageManager_setFileData554 = addmethod2pos(self, "setFileData", &meth_PackageManager_setFileData554, 12);
int argcv_meth_PackageManager_setFileData554[] = {1};
meth_meth_PackageManager_setFileData554->type = alloc_MethodType(1, argcv_meth_PackageManager_setFileData554);
  meth_meth_PackageManager_setFileData554->definitionModule = modulename;
  meth_meth_PackageManager_setFileData554->definitionLine = 224;
// compilenode returning 
// Begin line 255
  setline(255);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure632 = createclosure(3, "findExisting");
setclosureframe(closure632, stackframe);
  addtoclosure(closure632, var_io);
  addtoclosure(closure632, var_sys);
  addtoclosure(closure632, var_bundlePath);
  struct UserObject *uo632 = (struct UserObject*)self;
  uo632->data[13] = (Object)closure632;
  Method *meth_meth_PackageManager_findExisting632 = addmethod2pos(self, "findExisting", &meth_PackageManager_findExisting632, 13);
int argcv_meth_PackageManager_findExisting632[] = {1};
meth_meth_PackageManager_findExisting632->type = alloc_MethodType(1, argcv_meth_PackageManager_findExisting632);
  meth_meth_PackageManager_findExisting632->definitionModule = modulename;
  meth_meth_PackageManager_findExisting632->definitionLine = 255;
// compilenode returning 
// Begin line 284
  setline(284);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo827 = (struct UserObject*)self;
  uo827->data[14] = emptyclosure;
  Method *meth_meth_PackageManager_validateFile827 = addmethod2pos(self, "validateFile", &meth_PackageManager_validateFile827, 14);
int argcv_meth_PackageManager_validateFile827[] = {1};
meth_meth_PackageManager_validateFile827->type = alloc_MethodType(1, argcv_meth_PackageManager_validateFile827);
  meth_meth_PackageManager_validateFile827->definitionModule = modulename;
  meth_meth_PackageManager_validateFile827->definitionLine = 284;
// compilenode returning 
// Begin line 294
  setline(294);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure850 = createclosure(3, "write");
setclosureframe(closure850, stackframe);
  addtoclosure(closure850, var_global);
  addtoclosure(closure850, var_sys);
  addtoclosure(closure850, var_io);
  struct UserObject *uo850 = (struct UserObject*)self;
  uo850->data[15] = (Object)closure850;
  Method *meth_meth_PackageManager_write850 = addmethod2pos(self, "write", &meth_PackageManager_write850, 15);
int argcv_meth_PackageManager_write850[] = {1};
meth_meth_PackageManager_write850->type = alloc_MethodType(1, argcv_meth_PackageManager_write850);
  meth_meth_PackageManager_write850->definitionModule = modulename;
  meth_meth_PackageManager_write850->definitionLine = 294;
// compilenode returning 
// Begin line 309
  setline(309);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure887 = createclosure(2, "getBuildPath");
setclosureframe(closure887, stackframe);
  addtoclosure(closure887, var_sys);
  addtoclosure(closure887, var_io);
  struct UserObject *uo887 = (struct UserObject*)self;
  uo887->data[16] = (Object)closure887;
  Method *meth_meth_PackageManager_getBuildPath887 = addmethod2pos(self, "getBuildPath", &meth_PackageManager_getBuildPath887, 16);
int argcv_meth_PackageManager_getBuildPath887[] = {0};
meth_meth_PackageManager_getBuildPath887->type = alloc_MethodType(1, argcv_meth_PackageManager_getBuildPath887);
  meth_meth_PackageManager_getBuildPath887->definitionModule = modulename;
  meth_meth_PackageManager_getBuildPath887->definitionLine = 309;
// compilenode returning 
// Begin line 326
  setline(326);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure925 = createclosure(3, "compile");
setclosureframe(closure925, stackframe);
  addtoclosure(closure925, var_global);
  addtoclosure(closure925, var_sys);
  addtoclosure(closure925, var_io);
  struct UserObject *uo925 = (struct UserObject*)self;
  uo925->data[17] = (Object)closure925;
  Method *meth_meth_PackageManager_compile925 = addmethod2pos(self, "compile", &meth_PackageManager_compile925, 17);
int argcv_meth_PackageManager_compile925[] = {1};
meth_meth_PackageManager_compile925->type = alloc_MethodType(1, argcv_meth_PackageManager_compile925);
  meth_meth_PackageManager_compile925->definitionModule = modulename;
  meth_meth_PackageManager_compile925->definitionLine = 326;
// compilenode returning 
// Begin line 342
  setline(342);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure965 = createclosure(1, "createDirectory");
setclosureframe(closure965, stackframe);
  addtoclosure(closure965, var_io);
  struct UserObject *uo965 = (struct UserObject*)self;
  uo965->data[18] = (Object)closure965;
  Method *meth_meth_PackageManager_createDirectory965 = addmethod2pos(self, "createDirectory", &meth_PackageManager_createDirectory965, 18);
int argcv_meth_PackageManager_createDirectory965[] = {1};
meth_meth_PackageManager_createDirectory965->type = alloc_MethodType(1, argcv_meth_PackageManager_createDirectory965);
  meth_meth_PackageManager_createDirectory965->definitionModule = modulename;
  meth_meth_PackageManager_createDirectory965->definitionLine = 342;
// compilenode returning 
// Begin line 366
  setline(366);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1043 = createclosure(1, "parseFile");
setclosureframe(closure1043, stackframe);
  addtoclosure(closure1043, var_done);
  struct UserObject *uo1043 = (struct UserObject*)self;
  uo1043->data[19] = (Object)closure1043;
  Method *meth_meth_PackageManager_parseFile1043 = addmethod2pos(self, "parseFile", &meth_PackageManager_parseFile1043, 19);
int argcv_meth_PackageManager_parseFile1043[] = {1};
meth_meth_PackageManager_parseFile1043->type = alloc_MethodType(1, argcv_meth_PackageManager_parseFile1043);
  meth_meth_PackageManager_parseFile1043->definitionModule = modulename;
  meth_meth_PackageManager_parseFile1043->definitionLine = 366;
// compilenode returning 
// Begin line 383
  setline(383);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1103 = (struct UserObject*)self;
  uo1103->data[20] = emptyclosure;
  Method *meth_meth_PackageManager_processLine1103 = addmethod2pos(self, "processLine", &meth_PackageManager_processLine1103, 20);
int argcv_meth_PackageManager_processLine1103[] = {1};
meth_meth_PackageManager_processLine1103->type = alloc_MethodType(1, argcv_meth_PackageManager_processLine1103);
  meth_meth_PackageManager_processLine1103->definitionModule = modulename;
  meth_meth_PackageManager_processLine1103->definitionLine = 383;
// compilenode returning 
// Begin line 402
  setline(402);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1184 = createclosure(2, "parseImport");
setclosureframe(closure1184, stackframe);
  addtoclosure(closure1184, var_imported);
  addtoclosure(closure1184, var_toProcess);
  struct UserObject *uo1184 = (struct UserObject*)self;
  uo1184->data[21] = (Object)closure1184;
  Method *meth_meth_PackageManager_parseImport1184 = addmethod2pos(self, "parseImport", &meth_PackageManager_parseImport1184, 21);
int argcv_meth_PackageManager_parseImport1184[] = {1};
meth_meth_PackageManager_parseImport1184->type = alloc_MethodType(1, argcv_meth_PackageManager_parseImport1184);
  meth_meth_PackageManager_parseImport1184->definitionModule = modulename;
  meth_meth_PackageManager_parseImport1184->definitionLine = 402;
// compilenode returning 
// Begin line 419
  setline(419);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1255 = (struct UserObject*)self;
  uo1255->data[22] = emptyclosure;
  Method *meth_meth_PackageManager_skipWhiteSpace1255 = addmethod2pos(self, "skipWhiteSpace", &meth_PackageManager_skipWhiteSpace1255, 22);
int argcv_meth_PackageManager_skipWhiteSpace1255[] = {2};
meth_meth_PackageManager_skipWhiteSpace1255->type = alloc_MethodType(1, argcv_meth_PackageManager_skipWhiteSpace1255);
  meth_meth_PackageManager_skipWhiteSpace1255->definitionModule = modulename;
  meth_meth_PackageManager_skipWhiteSpace1255->definitionLine = 419;
// compilenode returning 
// Begin line 427
  setline(427);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1282 = (struct UserObject*)self;
  uo1282->data[23] = emptyclosure;
  Method *meth_meth_PackageManager_displayHelp1282 = addmethod2pos(self, "displayHelp", &meth_PackageManager_displayHelp1282, 23);
int argcv_meth_PackageManager_displayHelp1282[] = {0};
meth_meth_PackageManager_displayHelp1282->type = alloc_MethodType(1, argcv_meth_PackageManager_displayHelp1282);
  meth_meth_PackageManager_displayHelp1282->definitionModule = modulename;
  meth_meth_PackageManager_displayHelp1282->definitionLine = 427;
// compilenode returning 
// Begin line 433
  setline(433);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1289 = createclosure(1, "printMessage");
setclosureframe(closure1289, stackframe);
  addtoclosure(closure1289, var_verbose);
  struct UserObject *uo1289 = (struct UserObject*)self;
  uo1289->data[24] = (Object)closure1289;
  Method *meth_meth_PackageManager_printMessage1289 = addmethod2pos(self, "printMessage", &meth_PackageManager_printMessage1289, 24);
int argcv_meth_PackageManager_printMessage1289[] = {1};
meth_meth_PackageManager_printMessage1289->type = alloc_MethodType(1, argcv_meth_PackageManager_printMessage1289);
  meth_meth_PackageManager_printMessage1289->definitionModule = modulename;
  meth_meth_PackageManager_printMessage1289->definitionLine = 433;
// compilenode returning 
// Begin line 439
  setline(439);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1292 = createclosure(2, "bundle");
setclosureframe(closure1292, stackframe);
  addtoclosure(closure1292, var_imported);
  addtoclosure(closure1292, var_io);
  struct UserObject *uo1292 = (struct UserObject*)self;
  uo1292->data[25] = (Object)closure1292;
  Method *meth_meth_PackageManager_bundle1292 = addmethod2pos(self, "bundle", &meth_PackageManager_bundle1292, 25);
int argcv_meth_PackageManager_bundle1292[] = {2};
meth_meth_PackageManager_bundle1292->type = alloc_MethodType(1, argcv_meth_PackageManager_bundle1292);
  meth_meth_PackageManager_bundle1292->definitionModule = modulename;
  meth_meth_PackageManager_bundle1292->definitionLine = 439;
// compilenode returning 
// Begin line 467
  setline(467);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1462 = createclosure(1, "setImportDest");
setclosureframe(closure1462, stackframe);
  addtoclosure(closure1462, var_io);
  struct UserObject *uo1462 = (struct UserObject*)self;
  uo1462->data[26] = (Object)closure1462;
  Method *meth_meth_PackageManager_setImportDest1462 = addmethod2pos(self, "setImportDest", &meth_PackageManager_setImportDest1462, 26);
int argcv_meth_PackageManager_setImportDest1462[] = {2};
meth_meth_PackageManager_setImportDest1462->type = alloc_MethodType(1, argcv_meth_PackageManager_setImportDest1462);
  meth_meth_PackageManager_setImportDest1462->definitionModule = modulename;
  meth_meth_PackageManager_setImportDest1462->definitionLine = 467;
// compilenode returning 
// Begin line 488
  setline(488);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1540 = (struct UserObject*)self;
  uo1540->data[27] = emptyclosure;
  Method *meth_meth_PackageManager_parseAndPrefix1540 = addmethod2pos(self, "parseAndPrefix", &meth_PackageManager_parseAndPrefix1540, 27);
int argcv_meth_PackageManager_parseAndPrefix1540[] = {3};
meth_meth_PackageManager_parseAndPrefix1540->type = alloc_MethodType(1, argcv_meth_PackageManager_parseAndPrefix1540);
meth_meth_PackageManager_parseAndPrefix1540->type->types[0] = type_String;
meth_meth_PackageManager_parseAndPrefix1540->type->names[0] = "readFile";
meth_meth_PackageManager_parseAndPrefix1540->type->types[1] = type_String;
meth_meth_PackageManager_parseAndPrefix1540->type->names[1] = "address";
meth_meth_PackageManager_parseAndPrefix1540->type->types[2] = type_String;
meth_meth_PackageManager_parseAndPrefix1540->type->names[2] = "prefix";
  meth_meth_PackageManager_parseAndPrefix1540->definitionModule = modulename;
  meth_meth_PackageManager_parseAndPrefix1540->definitionLine = 488;
// compilenode returning 
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1750 = (struct UserObject*)self;
  uo1750->data[28] = emptyclosure;
  Method *meth_meth_PackageManager_removeExistingUrls1750 = addmethod2pos(self, "removeExistingUrls", &meth_PackageManager_removeExistingUrls1750, 28);
int argcv_meth_PackageManager_removeExistingUrls1750[] = {1};
meth_meth_PackageManager_removeExistingUrls1750->type = alloc_MethodType(1, argcv_meth_PackageManager_removeExistingUrls1750);
meth_meth_PackageManager_removeExistingUrls1750->type->types[0] = type_String;
meth_meth_PackageManager_removeExistingUrls1750->type->names[0] = "importStatement";
  meth_meth_PackageManager_removeExistingUrls1750->definitionModule = modulename;
  meth_meth_PackageManager_removeExistingUrls1750->definitionLine = 541;
// compilenode returning 
// Begin line 563
  setline(563);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1802 = (struct UserObject*)self;
  uo1802->data[29] = emptyclosure;
  Method *meth_meth_PackageManager_getBaseUrl1802 = addmethod2pos(self, "getBaseUrl", &meth_PackageManager_getBaseUrl1802, 29);
int argcv_meth_PackageManager_getBaseUrl1802[] = {1};
meth_meth_PackageManager_getBaseUrl1802->type = alloc_MethodType(1, argcv_meth_PackageManager_getBaseUrl1802);
meth_meth_PackageManager_getBaseUrl1802->type->types[0] = type_String;
meth_meth_PackageManager_getBaseUrl1802->type->names[0] = "importStatement";
  meth_meth_PackageManager_getBaseUrl1802->definitionModule = modulename;
  meth_meth_PackageManager_getBaseUrl1802->definitionLine = 563;
// compilenode returning 
// Begin line 586
  setline(586);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1849 = (struct UserObject*)self;
  uo1849->data[30] = emptyclosure;
  Method *meth_meth_PackageManager_removeContainingDir1849 = addmethod2pos(self, "removeContainingDir", &meth_PackageManager_removeContainingDir1849, 30);
int argcv_meth_PackageManager_removeContainingDir1849[] = {1};
meth_meth_PackageManager_removeContainingDir1849->type = alloc_MethodType(1, argcv_meth_PackageManager_removeContainingDir1849);
meth_meth_PackageManager_removeContainingDir1849->type->types[0] = type_String;
meth_meth_PackageManager_removeContainingDir1849->type->names[0] = "st";
  meth_meth_PackageManager_removeContainingDir1849->definitionModule = modulename;
  meth_meth_PackageManager_removeContainingDir1849->definitionLine = 586;
// compilenode returning 
// Begin line 601
  setline(601);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1894 = (struct UserObject*)self;
  uo1894->data[31] = emptyclosure;
  Method *meth_meth_PackageManager_getContainingDirectory1894 = addmethod2pos(self, "getContainingDirectory", &meth_PackageManager_getContainingDirectory1894, 31);
int argcv_meth_PackageManager_getContainingDirectory1894[] = {1};
meth_meth_PackageManager_getContainingDirectory1894->type = alloc_MethodType(1, argcv_meth_PackageManager_getContainingDirectory1894);
meth_meth_PackageManager_getContainingDirectory1894->type->types[0] = type_String;
meth_meth_PackageManager_getContainingDirectory1894->type->names[0] = "st";
  meth_meth_PackageManager_getContainingDirectory1894->definitionModule = modulename;
  meth_meth_PackageManager_getContainingDirectory1894->definitionLine = 601;
// compilenode returning 
// Begin line 617
  setline(617);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1944 = createclosure(1, "package");
setclosureframe(closure1944, stackframe);
  addtoclosure(closure1944, var_io);
  struct UserObject *uo1944 = (struct UserObject*)self;
  uo1944->data[32] = (Object)closure1944;
  Method *meth_meth_PackageManager_package1944 = addmethod2pos(self, "package", &meth_PackageManager_package1944, 32);
int argcv_meth_PackageManager_package1944[] = {1};
meth_meth_PackageManager_package1944->type = alloc_MethodType(1, argcv_meth_PackageManager_package1944);
meth_meth_PackageManager_package1944->type->types[0] = type_String;
meth_meth_PackageManager_package1944->type->names[0] = "folder";
  meth_meth_PackageManager_package1944->definitionModule = modulename;
  meth_meth_PackageManager_package1944->definitionLine = 617;
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
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2084 = createclosure(1, "imported");
setclosureframe(closure2084, stackframe);
  addtoclosure(closure2084, var_imported);
  struct UserObject *uo2084 = (struct UserObject*)self;
  uo2084->data[33] = (Object)closure2084;
  Method *meth_meth_PackageManager_imported2084 = addmethod2pos(self, "imported", &meth_PackageManager_imported2084, 33);
int argcv_meth_PackageManager_imported2084[] = {0};
meth_meth_PackageManager_imported2084->type = alloc_MethodType(1, argcv_meth_PackageManager_imported2084);
  meth_meth_PackageManager_imported2084->definitionModule = modulename;
  meth_meth_PackageManager_imported2084->definitionLine = 631;
// compilenode returning 
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2085 = createclosure(1, "imported:=");
setclosureframe(closure2085, stackframe);
  addtoclosure(closure2085, var_imported);
  struct UserObject *uo2085 = (struct UserObject*)self;
  uo2085->data[34] = (Object)closure2085;
  Method *meth_meth_PackageManager_imported_58__61_2085 = addmethod2pos(self, "imported:=", &meth_PackageManager_imported_58__61_2085, 34);
int argcv_meth_PackageManager_imported_58__61_2085[] = {1};
meth_meth_PackageManager_imported_58__61_2085->type = alloc_MethodType(1, argcv_meth_PackageManager_imported_58__61_2085);
  meth_meth_PackageManager_imported_58__61_2085->definitionModule = modulename;
  meth_meth_PackageManager_imported_58__61_2085->definitionLine = 631;
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
  Object array2087 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array2087
  *var_toProcess = array2087;
  if (array2087 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2088 = createclosure(1, "toProcess");
setclosureframe(closure2088, stackframe);
  addtoclosure(closure2088, var_toProcess);
  struct UserObject *uo2088 = (struct UserObject*)self;
  uo2088->data[35] = (Object)closure2088;
  Method *meth_meth_PackageManager_toProcess2088 = addmethod2pos(self, "toProcess", &meth_PackageManager_toProcess2088, 35);
int argcv_meth_PackageManager_toProcess2088[] = {0};
meth_meth_PackageManager_toProcess2088->type = alloc_MethodType(1, argcv_meth_PackageManager_toProcess2088);
  meth_meth_PackageManager_toProcess2088->definitionModule = modulename;
  meth_meth_PackageManager_toProcess2088->definitionLine = 631;
// compilenode returning 
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2089 = createclosure(1, "toProcess:=");
setclosureframe(closure2089, stackframe);
  addtoclosure(closure2089, var_toProcess);
  struct UserObject *uo2089 = (struct UserObject*)self;
  uo2089->data[36] = (Object)closure2089;
  Method *meth_meth_PackageManager_toProcess_58__61_2089 = addmethod2pos(self, "toProcess:=", &meth_PackageManager_toProcess_58__61_2089, 36);
int argcv_meth_PackageManager_toProcess_58__61_2089[] = {1};
meth_meth_PackageManager_toProcess_58__61_2089->type = alloc_MethodType(1, argcv_meth_PackageManager_toProcess_58__61_2089);
  meth_meth_PackageManager_toProcess_58__61_2089->definitionModule = modulename;
  meth_meth_PackageManager_toProcess_58__61_2089->definitionLine = 631;
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
  Object bool2091 = alloc_Boolean(0);
// compilenode returning bool2091
  *var_verbose = bool2091;
  if (bool2091 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2092 = createclosure(1, "verbose");
setclosureframe(closure2092, stackframe);
  addtoclosure(closure2092, var_verbose);
  struct UserObject *uo2092 = (struct UserObject*)self;
  uo2092->data[37] = (Object)closure2092;
  Method *meth_meth_PackageManager_verbose2092 = addmethod2pos(self, "verbose", &meth_PackageManager_verbose2092, 37);
int argcv_meth_PackageManager_verbose2092[] = {0};
meth_meth_PackageManager_verbose2092->type = alloc_MethodType(1, argcv_meth_PackageManager_verbose2092);
  meth_meth_PackageManager_verbose2092->definitionModule = modulename;
  meth_meth_PackageManager_verbose2092->definitionLine = 631;
// compilenode returning 
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2093 = createclosure(1, "verbose:=");
setclosureframe(closure2093, stackframe);
  addtoclosure(closure2093, var_verbose);
  struct UserObject *uo2093 = (struct UserObject*)self;
  uo2093->data[38] = (Object)closure2093;
  Method *meth_meth_PackageManager_verbose_58__61_2093 = addmethod2pos(self, "verbose:=", &meth_PackageManager_verbose_58__61_2093, 38);
int argcv_meth_PackageManager_verbose_58__61_2093[] = {1};
meth_meth_PackageManager_verbose_58__61_2093->type = alloc_MethodType(1, argcv_meth_PackageManager_verbose_58__61_2093);
  meth_meth_PackageManager_verbose_58__61_2093->definitionModule = modulename;
  meth_meth_PackageManager_verbose_58__61_2093->definitionLine = 631;
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
  Object bool2095 = alloc_Boolean(0);
// compilenode returning bool2095
  *var_global = bool2095;
  if (bool2095 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2096 = createclosure(1, "global");
setclosureframe(closure2096, stackframe);
  addtoclosure(closure2096, var_global);
  struct UserObject *uo2096 = (struct UserObject*)self;
  uo2096->data[39] = (Object)closure2096;
  Method *meth_meth_PackageManager_global2096 = addmethod2pos(self, "global", &meth_PackageManager_global2096, 39);
int argcv_meth_PackageManager_global2096[] = {0};
meth_meth_PackageManager_global2096->type = alloc_MethodType(1, argcv_meth_PackageManager_global2096);
  meth_meth_PackageManager_global2096->definitionModule = modulename;
  meth_meth_PackageManager_global2096->definitionLine = 631;
// compilenode returning 
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2097 = createclosure(1, "global:=");
setclosureframe(closure2097, stackframe);
  addtoclosure(closure2097, var_global);
  struct UserObject *uo2097 = (struct UserObject*)self;
  uo2097->data[40] = (Object)closure2097;
  Method *meth_meth_PackageManager_global_58__61_2097 = addmethod2pos(self, "global:=", &meth_PackageManager_global_58__61_2097, 40);
int argcv_meth_PackageManager_global_58__61_2097[] = {1};
meth_meth_PackageManager_global_58__61_2097->type = alloc_MethodType(1, argcv_meth_PackageManager_global_58__61_2097);
  meth_meth_PackageManager_global_58__61_2097->definitionModule = modulename;
  meth_meth_PackageManager_global_58__61_2097->definitionLine = 631;
// compilenode returning 
// compilenode returning done
// Begin line 10
  setline(10);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 9
  setline(9);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit2099 == NULL) {
    strlit2099 = alloc_String("");
    gc_root(strlit2099);
  }
// compilenode returning strlit2099
  *var_bundlePath = strlit2099;
  if (strlit2099 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2100 = createclosure(1, "bundlePath");
setclosureframe(closure2100, stackframe);
  addtoclosure(closure2100, var_bundlePath);
  struct UserObject *uo2100 = (struct UserObject*)self;
  uo2100->data[41] = (Object)closure2100;
  Method *meth_meth_PackageManager_bundlePath2100 = addmethod2pos(self, "bundlePath", &meth_PackageManager_bundlePath2100, 41);
int argcv_meth_PackageManager_bundlePath2100[] = {0};
meth_meth_PackageManager_bundlePath2100->type = alloc_MethodType(1, argcv_meth_PackageManager_bundlePath2100);
  meth_meth_PackageManager_bundlePath2100->definitionModule = modulename;
  meth_meth_PackageManager_bundlePath2100->definitionLine = 631;
// compilenode returning 
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2101 = createclosure(1, "bundlePath:=");
setclosureframe(closure2101, stackframe);
  addtoclosure(closure2101, var_bundlePath);
  struct UserObject *uo2101 = (struct UserObject*)self;
  uo2101->data[42] = (Object)closure2101;
  Method *meth_meth_PackageManager_bundlePath_58__61_2101 = addmethod2pos(self, "bundlePath:=", &meth_PackageManager_bundlePath_58__61_2101, 42);
int argcv_meth_PackageManager_bundlePath_58__61_2101[] = {1};
meth_meth_PackageManager_bundlePath_58__61_2101->type = alloc_MethodType(1, argcv_meth_PackageManager_bundlePath_58__61_2101);
  meth_meth_PackageManager_bundlePath_58__61_2101->definitionModule = modulename;
  meth_meth_PackageManager_bundlePath_58__61_2101->definitionLine = 631;
// compilenode returning 
// compilenode returning done
// Begin line 11
  setline(11);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 10
  setline(10);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit2103 == NULL) {
    strlit2103 = alloc_String("");
    gc_root(strlit2103);
  }
// compilenode returning strlit2103
  *var_baseUrl = strlit2103;
  if (strlit2103 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2104 = createclosure(1, "baseUrl");
setclosureframe(closure2104, stackframe);
  addtoclosure(closure2104, var_baseUrl);
  struct UserObject *uo2104 = (struct UserObject*)self;
  uo2104->data[43] = (Object)closure2104;
  Method *meth_meth_PackageManager_baseUrl2104 = addmethod2pos(self, "baseUrl", &meth_PackageManager_baseUrl2104, 43);
int argcv_meth_PackageManager_baseUrl2104[] = {0};
meth_meth_PackageManager_baseUrl2104->type = alloc_MethodType(1, argcv_meth_PackageManager_baseUrl2104);
  meth_meth_PackageManager_baseUrl2104->definitionModule = modulename;
  meth_meth_PackageManager_baseUrl2104->definitionLine = 631;
// compilenode returning 
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2105 = createclosure(1, "baseUrl:=");
setclosureframe(closure2105, stackframe);
  addtoclosure(closure2105, var_baseUrl);
  struct UserObject *uo2105 = (struct UserObject*)self;
  uo2105->data[44] = (Object)closure2105;
  Method *meth_meth_PackageManager_baseUrl_58__61_2105 = addmethod2pos(self, "baseUrl:=", &meth_PackageManager_baseUrl_58__61_2105, 44);
int argcv_meth_PackageManager_baseUrl_58__61_2105[] = {1};
meth_meth_PackageManager_baseUrl_58__61_2105->type = alloc_MethodType(1, argcv_meth_PackageManager_baseUrl_58__61_2105);
  meth_meth_PackageManager_baseUrl_58__61_2105->definitionModule = modulename;
  meth_meth_PackageManager_baseUrl_58__61_2105->definitionLine = 631;
// compilenode returning 
// compilenode returning done
// Begin line 12
  setline(12);
  setmodule(modulename);
  setsource(originalSourceLines);
  *var_curFile = undefined;
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2107 = createclosure(1, "curFile");
setclosureframe(closure2107, stackframe);
  addtoclosure(closure2107, var_curFile);
  struct UserObject *uo2107 = (struct UserObject*)self;
  uo2107->data[45] = (Object)closure2107;
  Method *meth_meth_PackageManager_curFile2107 = addmethod2pos(self, "curFile", &meth_PackageManager_curFile2107, 45);
int argcv_meth_PackageManager_curFile2107[] = {0};
meth_meth_PackageManager_curFile2107->type = alloc_MethodType(1, argcv_meth_PackageManager_curFile2107);
  meth_meth_PackageManager_curFile2107->definitionModule = modulename;
  meth_meth_PackageManager_curFile2107->definitionLine = 631;
// compilenode returning 
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2108 = createclosure(1, "curFile:=");
setclosureframe(closure2108, stackframe);
  addtoclosure(closure2108, var_curFile);
  struct UserObject *uo2108 = (struct UserObject*)self;
  uo2108->data[46] = (Object)closure2108;
  Method *meth_meth_PackageManager_curFile_58__61_2108 = addmethod2pos(self, "curFile:=", &meth_PackageManager_curFile_58__61_2108, 46);
int argcv_meth_PackageManager_curFile_58__61_2108[] = {1};
meth_meth_PackageManager_curFile_58__61_2108->type = alloc_MethodType(1, argcv_meth_PackageManager_curFile_58__61_2108);
  meth_meth_PackageManager_curFile_58__61_2108->definitionModule = modulename;
  meth_meth_PackageManager_curFile_58__61_2108->definitionLine = 631;
// compilenode returning 
// compilenode returning done
// Begin line 13
  setline(13);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 12
  setline(12);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool2110 = alloc_Boolean(0);
// compilenode returning bool2110
  *var_build = bool2110;
  if (bool2110 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2111 = createclosure(1, "build");
setclosureframe(closure2111, stackframe);
  addtoclosure(closure2111, var_build);
  struct UserObject *uo2111 = (struct UserObject*)self;
  uo2111->data[47] = (Object)closure2111;
  Method *meth_meth_PackageManager_build2111 = addmethod2pos(self, "build", &meth_PackageManager_build2111, 47);
int argcv_meth_PackageManager_build2111[] = {0};
meth_meth_PackageManager_build2111->type = alloc_MethodType(1, argcv_meth_PackageManager_build2111);
  meth_meth_PackageManager_build2111->definitionModule = modulename;
  meth_meth_PackageManager_build2111->definitionLine = 631;
// compilenode returning 
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2112 = createclosure(1, "build:=");
setclosureframe(closure2112, stackframe);
  addtoclosure(closure2112, var_build);
  struct UserObject *uo2112 = (struct UserObject*)self;
  uo2112->data[48] = (Object)closure2112;
  Method *meth_meth_PackageManager_build_58__61_2112 = addmethod2pos(self, "build:=", &meth_PackageManager_build_58__61_2112, 48);
int argcv_meth_PackageManager_build_58__61_2112[] = {1};
meth_meth_PackageManager_build_58__61_2112->type = alloc_MethodType(1, argcv_meth_PackageManager_build_58__61_2112);
  meth_meth_PackageManager_build_58__61_2112->definitionModule = modulename;
  meth_meth_PackageManager_build_58__61_2112->definitionLine = 631;
// compilenode returning 
// compilenode returning done
// Begin line 15
  setline(15);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 13
  setline(13);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool2114 = alloc_Boolean(0);
// compilenode returning bool2114
  *var_install = bool2114;
  if (bool2114 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2115 = createclosure(1, "install");
setclosureframe(closure2115, stackframe);
  addtoclosure(closure2115, var_install);
  struct UserObject *uo2115 = (struct UserObject*)self;
  uo2115->data[49] = (Object)closure2115;
  Method *meth_meth_PackageManager_install2115 = addmethod2pos(self, "install", &meth_PackageManager_install2115, 49);
int argcv_meth_PackageManager_install2115[] = {0};
meth_meth_PackageManager_install2115->type = alloc_MethodType(1, argcv_meth_PackageManager_install2115);
  meth_meth_PackageManager_install2115->definitionModule = modulename;
  meth_meth_PackageManager_install2115->definitionLine = 631;
// compilenode returning 
// Begin line 631
  setline(631);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2116 = createclosure(1, "install:=");
setclosureframe(closure2116, stackframe);
  addtoclosure(closure2116, var_install);
  struct UserObject *uo2116 = (struct UserObject*)self;
  uo2116->data[50] = (Object)closure2116;
  Method *meth_meth_PackageManager_install_58__61_2116 = addmethod2pos(self, "install:=", &meth_PackageManager_install_58__61_2116, 50);
int argcv_meth_PackageManager_install_58__61_2116[] = {1};
meth_meth_PackageManager_install_58__61_2116->type = alloc_MethodType(1, argcv_meth_PackageManager_install_58__61_2116);
  meth_meth_PackageManager_install_58__61_2116->definitionModule = modulename;
  meth_meth_PackageManager_install_58__61_2116->definitionLine = 631;
// compilenode returning 
// compilenode returning done
// Begin line 15
  setline(15);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 17
  setline(17);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe2118 = gc_frame_new();
// Begin line 15
  setline(15);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe2119 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call2120 = callmethod(*var_sys, "argv",
    1, partcv, params);
  gc_frame_end(callframe2119);
// compilenode returning call2120
// compilenode returning call2120
  gc_frame_newslot(call2120);
  params[0] = call2120;
  partcv[0] = 1;
  Object call2121 = callmethodflags(self, "parseArgs", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe2118);
// compilenode returning call2121
// Begin line 17
  setline(17);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// compilenode returning 
// Begin line 56
  setline(56);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 111
  setline(111);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 119
  setline(119);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 127
  setline(127);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 139
  setline(139);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 159
  setline(159);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 166
  setline(166);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 170
  setline(170);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 182
  setline(182);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 207
  setline(207);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 224
  setline(224);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 255
  setline(255);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 284
  setline(284);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 294
  setline(294);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 309
  setline(309);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 326
  setline(326);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 342
  setline(342);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 366
  setline(366);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 383
  setline(383);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 402
  setline(402);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 419
  setline(419);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 427
  setline(427);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 433
  setline(433);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 439
  setline(439);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 467
  setline(467);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 488
  setline(488);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 563
  setline(563);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 586
  setline(586);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 601
  setline(601);
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
