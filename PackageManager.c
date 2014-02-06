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
  "import \"SetupCall\" as setup",
  "",
  "var imported",
  "var toProcess := []",
  "var verbose := false",
  "var global := false",
  "var bundlePath := \"\"",
  "var baseUrl := \"\"",
  "var curFile",
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
  "    if ((impAddress.size >= 7) && (impAddress.substringFrom(1)to(7) == \"http://\"))then{",
  "        setBaseUrl(impAddress)",
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
  "        print(\"YES IT DOES\")",
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
  "//ToBundle is the name of the file that needs to be compiled. The compiler",
  "//must be in the directory of the file to bundle in order for this to run ",
  "//Name is the name of the package passed into the compiler",
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
  "",
  "                io.system(\"rm \"++address++\"/\"++file)",
  "                var out := io.open(address++\"/\"++file,\"w\")",
  "                for (toWrite) do { d-> ",
  "                    out.write(d)",
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
  "    print(\"BP \" ++buildPath)",
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
Object reader_PackageManager_outer_74(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject*)self;
  return uo->data[0];
}
Object reader_PackageManager_outer_379(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject*)self;
  return uo->data[0];
}
Object reader_PackageManager_address_380(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  return uo->data[1];
}
Object writer_PackageManager_address_380(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  uo->data[1] = args[0];
  return done;
}
Object reader_PackageManager_data_381(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  return uo->data[2];
}
Object writer_PackageManager_data_381(Object self, int nparams, int *argcv, Object* args, int flags) {
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
static ClassData objclass73;
static Object strlit91;
static Object strlit94;
static Object strlit131;
static Object strlit134;
static Object strlit194;
static Object strlit204;
static Object strlit207;
static Object strlit222;
static Object strlit225;
static Object strlit228;
static Object strlit234;
static Object strlit236;
static Object strlit244;
static Object strlit246;
static Object strlit256;
static Object strlit259;
static Object strlit264;
static Object strlit275;
static Object strlit279;
static Object strlit294;
static Object strlit307;
static Object strlit313;
static Object strlit335;
static Object strlit375;
static ClassData objclass378;
static Object strlit391;
static Object strlit396;
static Object strlit407;
static Object strlit415;
static Object strlit420;
static Object strlit451;
static Object strlit458;
static Object strlit469;
static Object strlit475;
static Object strlit480;
static Object strlit493;
static Object strlit498;
static Object strlit505;
static Object strlit512;
static Object strlit529;
static Object strlit569;
static Object strlit585;
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
static Object strlit1265;
static Object strlit1268;
static Object strlit1277;
static Object strlit1282;
static Object strlit1287;
static Object strlit1317;
static Object strlit1322;
static Object strlit1325;
static Object strlit1334;
static Object strlit1337;
static Object strlit1342;
static Object strlit1345;
static Object strlit1354;
static Object strlit1357;
static Object strlit1362;
static Object strlit1365;
static Object strlit1377;
static Object strlit1402;
static Object strlit1405;
static Object strlit1414;
static Object strlit1417;
static Object strlit1433;
static Object strlit1457;
static Object strlit1463;
static Object strlit1468;
static Object strlit1480;
static Object strlit1483;
static Object strlit1492;
static Object strlit1497;
static Object strlit1543;
static Object strlit1564;
static Object strlit1597;
static Object strlit1603;
static Object strlit1613;
static Object strlit1630;
static Object strlit1635;
static Object strlit1671;
static Object strlit1674;
static Object strlit1689;
static Object strlit1699;
static Object strlit1711;
static Object strlit1714;
static Object strlit1732;
static Object strlit1751;
static Object strlit1784;
static Object strlit1803;
static Object strlit1836;
static Object strlit1881;
static Object strlit1902;
static Object strlit1913;
static Object strlit1920;
static Object strlit1925;
static Object strlit1928;
static Object strlit1935;
static Object strlit1938;
static Object strlit1941;
static Object strlit1949;
static Object strlit1952;
static Object strlit1958;
static Object strlit1961;
static Object strlit1964;
static Object strlit1967;
static Object strlit1974;
static Object strlit1977;
static Object strlit1992;
static Object strlit1995;
static Object strlit2000;
static Object strlit2006;
static Object strlit2009;
static Object strlit2018;
static Object strlit2021;
static Object strlit2026;
static Object strlit2031;
static Object strlit2032;
static Object strlit2033;
static Object strlit2034;
static Object strlit2043;
static Object strlit2046;
static Object strlit2051;
Object module_sys;
Object module_sys_init();
Object module_io;
Object module_io_init();
Object module_curl;
Object module_curl_init();
Object module_SetupCall;
Object module_SetupCall_init();
static Object strlit2072;
static Object strlit2076;
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
// Begin line 23
  setline(23);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe14 = gc_frame_new();
// Begin line 22
  setline(22);
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
// Begin line 26
  setline(26);
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
// Begin line 29
  setline(29);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe30 = gc_frame_new();
// Begin line 28
  setline(28);
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
// Begin line 32
  setline(32);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe38 = gc_frame_new();
// Begin line 31
  setline(31);
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
// Begin line 36
  setline(36);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe46 = gc_frame_new();
// Begin line 35
  setline(35);
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
// Begin line 40
  setline(40);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 39
  setline(39);
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
// Begin line 43
  setline(43);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 42
  setline(42);
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
// Begin line 21
  setline(21);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 20
  setline(20);
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
// Begin line 24
  setline(24);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe10 = gc_frame_new();
// Begin line 21
  setline(21);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit11 == NULL) {
    strlit11 = alloc_String("get");
    gc_root(strlit11);
  }
// compilenode returning strlit11
  gc_frame_newslot(strlit11);
// Begin line 24
  setline(24);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block12 = alloc_Block(NULL, NULL, "PackageManager", 24);
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
  meth_meth_PackageManager__apply13->definitionLine = 18;
// compilenode returning block12
  gc_frame_newslot(block12);
// Begin line 21
  setline(21);
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
// Begin line 27
  setline(27);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe18 = gc_frame_new();
// Begin line 24
  setline(24);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit19 == NULL) {
    strlit19 = alloc_String("list");
    gc_root(strlit19);
  }
// compilenode returning strlit19
  gc_frame_newslot(strlit19);
// Begin line 27
  setline(27);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block20 = alloc_Block(NULL, NULL, "PackageManager", 27);
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
  meth_meth_PackageManager__apply21->definitionLine = 18;
// compilenode returning block20
  gc_frame_newslot(block20);
// Begin line 24
  setline(24);
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
// Begin line 30
  setline(30);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe26 = gc_frame_new();
// Begin line 27
  setline(27);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit27 == NULL) {
    strlit27 = alloc_String("bundle");
    gc_root(strlit27);
  }
// compilenode returning strlit27
  gc_frame_newslot(strlit27);
// Begin line 30
  setline(30);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block28 = alloc_Block(NULL, NULL, "PackageManager", 30);
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
  meth_meth_PackageManager__apply29->definitionLine = 18;
// compilenode returning block28
  gc_frame_newslot(block28);
// Begin line 27
  setline(27);
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
// Begin line 34
  setline(34);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe34 = gc_frame_new();
// Begin line 30
  setline(30);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit35 == NULL) {
    strlit35 = alloc_String("set-address");
    gc_root(strlit35);
  }
// compilenode returning strlit35
  gc_frame_newslot(strlit35);
// Begin line 34
  setline(34);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block36 = alloc_Block(NULL, NULL, "PackageManager", 34);
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
  meth_meth_PackageManager__apply37->definitionLine = 18;
// compilenode returning block36
  gc_frame_newslot(block36);
// Begin line 30
  setline(30);
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
// Begin line 38
  setline(38);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe42 = gc_frame_new();
// Begin line 34
  setline(34);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit43 == NULL) {
    strlit43 = alloc_String("package");
    gc_root(strlit43);
  }
// compilenode returning strlit43
  gc_frame_newslot(strlit43);
// Begin line 38
  setline(38);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block44 = alloc_Block(NULL, NULL, "PackageManager", 38);
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
  meth_meth_PackageManager__apply45->definitionLine = 18;
// compilenode returning block44
  gc_frame_newslot(block44);
// Begin line 34
  setline(34);
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
// Begin line 41
  setline(41);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe50 = gc_frame_new();
// Begin line 38
  setline(38);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit51 == NULL) {
    strlit51 = alloc_String("--verbose");
    gc_root(strlit51);
  }
// compilenode returning strlit51
  gc_frame_newslot(strlit51);
// Begin line 41
  setline(41);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block52 = alloc_Block(NULL, NULL, "PackageManager", 41);
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
  meth_meth_PackageManager__apply53->definitionLine = 18;
// compilenode returning block52
  gc_frame_newslot(block52);
// Begin line 38
  setline(38);
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
// Begin line 44
  setline(44);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe58 = gc_frame_new();
// Begin line 41
  setline(41);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit59 == NULL) {
    strlit59 = alloc_String("--global");
    gc_root(strlit59);
  }
// compilenode returning strlit59
  gc_frame_newslot(strlit59);
// Begin line 44
  setline(44);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block60 = alloc_Block(NULL, NULL, "PackageManager", 44);
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
  meth_meth_PackageManager__apply61->definitionLine = 18;
// compilenode returning block60
  gc_frame_newslot(block60);
// Begin line 41
  setline(41);
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
  gc_frame_end(frame);
  return call65;
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
// Begin line 18
  setline(18);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 17
  setline(17);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1 = alloc_Float64(0);
// compilenode returning num1
  *var_count = num1;
  if (num1 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 19
  setline(19);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 18
  setline(18);
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
// Begin line 45
  setline(45);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe5 = gc_frame_new();
// Begin line 19
  setline(19);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_args
  gc_frame_newslot(*var_args);
// Begin line 45
  setline(45);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block6 = alloc_Block(NULL, NULL, "PackageManager", 45);
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
  meth_meth_PackageManager__apply7->definitionLine = 18;
// compilenode returning block6
  gc_frame_newslot(block6);
  params[0] = *var_args;
  params[1] = block6;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call67 = callmethodflags(self, "forArgs()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe5);
// compilenode returning call67
  gc_frame_end(frame);
  return call67;
}
Object meth_PackageManager_option_40__41_do75(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 55
  setline(55);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe76 = gc_frame_new();
// Begin line 54
  setline(54);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call77 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe76);
// compilenode returning call77
  *var_arg = call77;
  if (call77 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 64
  setline(64);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 55
  setline(55);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arg
  int op_slot_left_79 = gc_frame_newslot(*var_arg);
// compilenode returning *var_name
  int op_slot_right_79 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult81 = callmethod(*var_arg, "==", 1, partcv, params);
// compilenode returning opresult81
  Object if78 = done;
  if (istrue(opresult81)) {
// Begin line 60
  setline(60);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 56
  setline(56);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe84 = gc_frame_new();
// compilenode returning *var_args
  partcv[0] = 0;
  Object call85 = callmethod(*var_args, "size",
    1, partcv, params);
  gc_frame_end(callframe84);
// compilenode returning call85
// compilenode returning call85
  int op_slot_left_83 = gc_frame_newslot(call85);
// compilenode returning *var_i
  int op_slot_right_83 = gc_frame_newslot(*var_i);
  params[0] = *var_i;
  partcv[0] = 1;
  Object opresult87 = callmethod(call85, "==", 1, partcv, params);
// compilenode returning opresult87
  Object if82 = done;
  if (istrue(opresult87)) {
// Begin line 58
  setline(58);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe88 = gc_frame_new();
// Begin line 57
  setline(57);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit91 == NULL) {
    strlit91 = alloc_String("Missing value for option ");
    gc_root(strlit91);
  }
// compilenode returning strlit91
  int op_slot_left_90 = gc_frame_newslot(strlit91);
// compilenode returning *var_name
  int op_slot_right_90 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult93 = callmethod(strlit91, "++", 1, partcv, params);
// compilenode returning opresult93
  int op_slot_left_89 = gc_frame_newslot(opresult93);
  if (strlit94 == NULL) {
    strlit94 = alloc_String("");
    gc_root(strlit94);
  }
// compilenode returning strlit94
  int op_slot_right_89 = gc_frame_newslot(strlit94);
  params[0] = strlit94;
  partcv[0] = 1;
  Object opresult96 = callmethod(opresult93, "++", 1, partcv, params);
// compilenode returning opresult96
  gc_frame_newslot(opresult96);
  int callframe97 = gc_frame_new();
  partcv[0] = 0;
  Object call98 = callmethodflags(prelude, "Exception", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe97);
// compilenode returning call98
  params[0] = opresult96;
  partcv[0] = 1;
  Object call99 = callmethod(call98, "raise",
    1, partcv, params);
  gc_frame_end(callframe88);
// compilenode returning call99
    gc_frame_newslot(call99);
    if82 = call99;
  } else {
  }
// compilenode returning if82
// Begin line 61
  setline(61);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 60
  setline(60);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_100 = gc_frame_newslot(*var_i);
  Object num101 = alloc_Float64(1);
// compilenode returning num101
  int op_slot_right_100 = gc_frame_newslot(num101);
  params[0] = num101;
  partcv[0] = 1;
  Object sum103 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum103
  *var_i = sum103;
  if (sum103 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 62
  setline(62);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe105 = gc_frame_new();
// Begin line 61
  setline(61);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe106 = gc_frame_new();
// compilenode returning *var_i
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call107 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe106);
// compilenode returning call107
  gc_frame_newslot(call107);
// compilenode returning *var_block_39_
  params[0] = call107;
  partcv[0] = 1;
  Object call108 = callmethod(*var_block_39_, "apply",
    1, partcv, params);
  gc_frame_end(callframe105);
// compilenode returning call108
// Begin line 63
  setline(63);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 62
  setline(62);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool109 = alloc_Boolean(1);
// compilenode returning bool109
  *var_ran = bool109;
  if (bool109 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if78 = done;
  } else {
  }
// compilenode returning if78
  gc_frame_end(frame);
  return if78;
}
Object meth_PackageManager_doubleValue_40__41_do111(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 68
  setline(68);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe112 = gc_frame_new();
// Begin line 67
  setline(67);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call113 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe112);
// compilenode returning call113
  *var_arg = call113;
  if (call113 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 79
  setline(79);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 68
  setline(68);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arg
  int op_slot_left_115 = gc_frame_newslot(*var_arg);
// compilenode returning *var_name
  int op_slot_right_115 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult117 = callmethod(*var_arg, "==", 1, partcv, params);
// compilenode returning opresult117
  Object if114 = done;
  if (istrue(opresult117)) {
// Begin line 72
  setline(72);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 69
  setline(69);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe120 = gc_frame_new();
// compilenode returning *var_args
  partcv[0] = 0;
  Object call121 = callmethod(*var_args, "size",
    1, partcv, params);
  gc_frame_end(callframe120);
// compilenode returning call121
// compilenode returning call121
  int op_slot_left_119 = gc_frame_newslot(call121);
// compilenode returning *var_i
  int op_slot_left_122 = gc_frame_newslot(*var_i);
  Object num123 = alloc_Float64(1);
// compilenode returning num123
  int op_slot_right_122 = gc_frame_newslot(num123);
  params[0] = num123;
  partcv[0] = 1;
  Object sum125 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum125
  int op_slot_right_119 = gc_frame_newslot(sum125);
  params[0] = sum125;
  partcv[0] = 1;
  Object opresult127 = callmethod(call121, "<", 1, partcv, params);
// compilenode returning opresult127
  Object if118 = done;
  if (istrue(opresult127)) {
// Begin line 71
  setline(71);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe128 = gc_frame_new();
// Begin line 70
  setline(70);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit131 == NULL) {
    strlit131 = alloc_String("Missing values for option ");
    gc_root(strlit131);
  }
// compilenode returning strlit131
  int op_slot_left_130 = gc_frame_newslot(strlit131);
// compilenode returning *var_name
  int op_slot_right_130 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult133 = callmethod(strlit131, "++", 1, partcv, params);
// compilenode returning opresult133
  int op_slot_left_129 = gc_frame_newslot(opresult133);
  if (strlit134 == NULL) {
    strlit134 = alloc_String("");
    gc_root(strlit134);
  }
// compilenode returning strlit134
  int op_slot_right_129 = gc_frame_newslot(strlit134);
  params[0] = strlit134;
  partcv[0] = 1;
  Object opresult136 = callmethod(opresult133, "++", 1, partcv, params);
// compilenode returning opresult136
  gc_frame_newslot(opresult136);
  int callframe137 = gc_frame_new();
  partcv[0] = 0;
  Object call138 = callmethodflags(prelude, "Exception", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe137);
// compilenode returning call138
  params[0] = opresult136;
  partcv[0] = 1;
  Object call139 = callmethod(call138, "raise",
    1, partcv, params);
  gc_frame_end(callframe128);
// compilenode returning call139
    gc_frame_newslot(call139);
    if118 = call139;
  } else {
  }
// compilenode returning if118
// Begin line 73
  setline(73);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe140 = gc_frame_new();
// Begin line 72
  setline(72);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_141 = gc_frame_newslot(*var_i);
  Object num142 = alloc_Float64(1);
// compilenode returning num142
  int op_slot_right_141 = gc_frame_newslot(num142);
  params[0] = num142;
  partcv[0] = 1;
  Object sum144 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum144
  gc_frame_newslot(sum144);
// compilenode returning *var_args
  params[0] = sum144;
  partcv[0] = 1;
  Object call145 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe140);
// compilenode returning call145
  *var_arga = call145;
  if (call145 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 74
  setline(74);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe146 = gc_frame_new();
// Begin line 73
  setline(73);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_147 = gc_frame_newslot(*var_i);
  Object num148 = alloc_Float64(2);
// compilenode returning num148
  int op_slot_right_147 = gc_frame_newslot(num148);
  params[0] = num148;
  partcv[0] = 1;
  Object sum150 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum150
  gc_frame_newslot(sum150);
// compilenode returning *var_args
  params[0] = sum150;
  partcv[0] = 1;
  Object call151 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe146);
// compilenode returning call151
  *var_argb = call151;
  if (call151 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 75
  setline(75);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe152 = gc_frame_new();
// Begin line 74
  setline(74);
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
  Object call153 = callmethod(*var_block_39_, "apply",
    1, partcv, params);
  gc_frame_end(callframe152);
// compilenode returning call153
// Begin line 76
  setline(76);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 75
  setline(75);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_154 = gc_frame_newslot(*var_i);
  Object num155 = alloc_Float64(2);
// compilenode returning num155
  int op_slot_right_154 = gc_frame_newslot(num155);
  params[0] = num155;
  partcv[0] = 1;
  Object sum157 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum157
  *var_i = sum157;
  if (sum157 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 77
  setline(77);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 76
  setline(76);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool159 = alloc_Boolean(1);
// compilenode returning bool159
  *var_ran = bool159;
  if (bool159 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if114 = done;
  } else {
  }
// compilenode returning if114
  gc_frame_end(frame);
  return if114;
}
Object meth_PackageManager_flag_40__41_do161(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call163 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe162);
// compilenode returning call163
  *var_arg = call163;
  if (call163 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 87
  setline(87);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 83
  setline(83);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arg
  int op_slot_left_165 = gc_frame_newslot(*var_arg);
// compilenode returning *var_name
  int op_slot_right_165 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult167 = callmethod(*var_arg, "==", 1, partcv, params);
// compilenode returning opresult167
  Object if164 = done;
  if (istrue(opresult167)) {
// Begin line 84
  setline(84);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe168 = gc_frame_new();
// compilenode returning *var_block_39_
  partcv[0] = 0;
  Object call169 = callmethod(*var_block_39_, "apply",
    1, partcv, params);
  gc_frame_end(callframe168);
// compilenode returning call169
// compilenode returning call169
// Begin line 86
  setline(86);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 85
  setline(85);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool170 = alloc_Boolean(1);
// compilenode returning bool170
  *var_ran = bool170;
  if (bool170 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if164 = done;
  } else {
  }
// compilenode returning if164
  gc_frame_end(frame);
  return if164;
}
Object meth_PackageManager__apply174(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_175 = gc_frame_newslot(*var_i);
// compilenode returning *var_size
  int op_slot_right_175 = gc_frame_newslot(*var_size);
  params[0] = *var_size;
  partcv[0] = 1;
  Object opresult177 = callmethod(*var_i, "<=", 1, partcv, params);
// compilenode returning opresult177
  gc_frame_end(frame);
  return opresult177;
}
Object meth_PackageManager__apply180(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 91
  setline(91);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe181 = gc_frame_new();
// Begin line 90
  setline(90);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call182 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe181);
// compilenode returning call182
  *var_arg = call182;
  if (call182 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 92
  setline(92);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 91
  setline(91);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arg
  params[0] = *var_arg;
  Object call183 = gracelib_print(NULL, 1,  params);
// compilenode returning call183
// Begin line 93
  setline(93);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 92
  setline(92);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool184 = alloc_Boolean(0);
// compilenode returning bool184
  *var_ran = bool184;
  if (bool184 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 94
  setline(94);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe186 = gc_frame_new();
// Begin line 93
  setline(93);
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
  Object call187 = callmethod(*var_block, "apply",
    1, partcv, params);
  gc_frame_end(callframe186);
// compilenode returning call187
// Begin line 98
  setline(98);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 94
  setline(94);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe191 = gc_frame_new();
  Object num192 = alloc_Float64(1);
// compilenode returning num192
  gc_frame_newslot(num192);
// compilenode returning *var_arg
  params[0] = num192;
  partcv[0] = 1;
  Object call193 = callmethod(*var_arg, "at",
    1, partcv, params);
  gc_frame_end(callframe191);
// compilenode returning call193
  int op_slot_left_190 = gc_frame_newslot(call193);
  if (strlit194 == NULL) {
    strlit194 = alloc_String("-");
    gc_root(strlit194);
  }
// compilenode returning strlit194
  int op_slot_right_190 = gc_frame_newslot(strlit194);
  params[0] = strlit194;
  partcv[0] = 1;
  Object opresult196 = callmethod(call193, "==", 1, partcv, params);
// compilenode returning opresult196
  int op_slot_left_189 = gc_frame_newslot(opresult196);
  int callframe197 = gc_frame_new();
// compilenode returning *var_ran
  partcv[0] = 0;
  Object call198 = callmethod(*var_ran, "not",
    1, partcv, params);
  gc_frame_end(callframe197);
// compilenode returning call198
// compilenode returning call198
  int op_slot_right_189 = gc_frame_newslot(call198);
  params[0] = call198;
  partcv[0] = 1;
  Object opresult200 = callmethod(opresult196, "&&", 1, partcv, params);
// compilenode returning opresult200
  Object if188 = done;
  if (istrue(opresult200)) {
// Begin line 96
  setline(96);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe201 = gc_frame_new();
// Begin line 95
  setline(95);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit204 == NULL) {
    strlit204 = alloc_String("Unrecognised argument ");
    gc_root(strlit204);
  }
// compilenode returning strlit204
  int op_slot_left_203 = gc_frame_newslot(strlit204);
// compilenode returning *var_arg
  int op_slot_right_203 = gc_frame_newslot(*var_arg);
  params[0] = *var_arg;
  partcv[0] = 1;
  Object opresult206 = callmethod(strlit204, "++", 1, partcv, params);
// compilenode returning opresult206
  int op_slot_left_202 = gc_frame_newslot(opresult206);
  if (strlit207 == NULL) {
    strlit207 = alloc_String("");
    gc_root(strlit207);
  }
// compilenode returning strlit207
  int op_slot_right_202 = gc_frame_newslot(strlit207);
  params[0] = strlit207;
  partcv[0] = 1;
  Object opresult209 = callmethod(opresult206, "++", 1, partcv, params);
// compilenode returning opresult209
  gc_frame_newslot(opresult209);
  int callframe210 = gc_frame_new();
  partcv[0] = 0;
  Object call211 = callmethodflags(prelude, "Exception", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe210);
// compilenode returning call211
  params[0] = opresult209;
  partcv[0] = 1;
  Object call212 = callmethod(call211, "raise",
    1, partcv, params);
  gc_frame_end(callframe201);
// compilenode returning call212
    gc_frame_newslot(call212);
    if188 = call212;
  } else {
  }
// compilenode returning if188
// Begin line 99
  setline(99);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 98
  setline(98);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_213 = gc_frame_newslot(*var_i);
  Object num214 = alloc_Float64(1);
// compilenode returning num214
  int op_slot_right_213 = gc_frame_newslot(num214);
  params[0] = num214;
  partcv[0] = 1;
  Object sum216 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum216
  *var_i = sum216;
  if (sum216 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_forArgs_40__41_do68(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 49
  setline(49);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 48
  setline(48);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num69 = alloc_Float64(2);
// compilenode returning num69
  *var_i = num69;
  if (num69 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 50
  setline(50);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 49
  setline(49);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool70 = alloc_Boolean(0);
// compilenode returning bool70
  *var_ran = bool70;
  if (bool70 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 52
  setline(52);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 50
  setline(50);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe71 = gc_frame_new();
// compilenode returning *var_args
  partcv[0] = 0;
  Object call72 = callmethod(*var_args, "size",
    1, partcv, params);
  gc_frame_end(callframe71);
// compilenode returning call72
// compilenode returning call72
  *var_size = call72;
  if (call72 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 52
  setline(52);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object inheritingObject73 = inheritingObject;
  if (isTailObject) {
    isTailObject = 0;
    inheritingObject = NULL;
  }
  Object obj73 = alloc_userobj2(3,4, objclass73);
  gc_frame_newslot(obj73);
// OBJECT OUTER DEC outer
  adddatum2(obj73, self, 0);
  addmethodreal(obj73,"outer", &reader_PackageManager_outer_74);
  Object oldself73 = self;
  struct StackFrameObject *oldstackframe73 = stackframe;
  stackframe = alloc_StackFrame(1, oldstackframe73);
  gc_frame_newslot((Object)stackframe);
  self = obj73;
  Object *oldselfslot73 = selfslot;
  selfslot = &stackframe->slots[0];
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  Object thisouter73 = (*(struct UserObject *)self).data[0], lowerouter73 = thisouter73;
  if (inheritingObject73) {
    struct UserObject *inho73 = (struct UserObject *)inheritingObject73;
    while (inho73->super != GraceDefaultObject) inho73 = (struct UserObject *)inho73->super;
    inho73->super = obj73;
    self = inheritingObject73;
    *selfslot = self;
    lowerouter73 = (*(struct UserObject *)self).data[0];
    (*(struct UserObject *)self).data[0] = thisouter73;
  }
  block_savedest(obj73);
  Object closure75 = createclosure(3, "option()do");
setclosureframe(closure75, stackframe);
  addtoclosure(closure75, var_i);
  addtoclosure(closure75, var_args);
  addtoclosure(closure75, var_ran);
  struct UserObject *uo75 = (struct UserObject*)obj73;
  uo75->data[1] = (Object)closure75;
  Method *meth_meth_PackageManager_option_40__41_do75 = addmethod2pos(obj73, "option()do", &meth_PackageManager_option_40__41_do75, 1);
int argcv_meth_PackageManager_option_40__41_do75[] = {1, 1};
meth_meth_PackageManager_option_40__41_do75->type = alloc_MethodType(2, argcv_meth_PackageManager_option_40__41_do75);
meth_meth_PackageManager_option_40__41_do75->type->types[0] = type_String;
meth_meth_PackageManager_option_40__41_do75->type->names[0] = "name";
  meth_meth_PackageManager_option_40__41_do75->definitionModule = modulename;
  meth_meth_PackageManager_option_40__41_do75->definitionLine = 53;
  block_savedest(obj73);
  Object closure111 = createclosure(3, "doubleValue()do");
setclosureframe(closure111, stackframe);
  addtoclosure(closure111, var_i);
  addtoclosure(closure111, var_args);
  addtoclosure(closure111, var_ran);
  struct UserObject *uo111 = (struct UserObject*)obj73;
  uo111->data[2] = (Object)closure111;
  Method *meth_meth_PackageManager_doubleValue_40__41_do111 = addmethod2pos(obj73, "doubleValue()do", &meth_PackageManager_doubleValue_40__41_do111, 2);
int argcv_meth_PackageManager_doubleValue_40__41_do111[] = {1, 1};
meth_meth_PackageManager_doubleValue_40__41_do111->type = alloc_MethodType(2, argcv_meth_PackageManager_doubleValue_40__41_do111);
meth_meth_PackageManager_doubleValue_40__41_do111->type->types[0] = type_String;
meth_meth_PackageManager_doubleValue_40__41_do111->type->names[0] = "name";
  meth_meth_PackageManager_doubleValue_40__41_do111->definitionModule = modulename;
  meth_meth_PackageManager_doubleValue_40__41_do111->definitionLine = 66;
  block_savedest(obj73);
  Object closure161 = createclosure(3, "flag()do");
setclosureframe(closure161, stackframe);
  addtoclosure(closure161, var_i);
  addtoclosure(closure161, var_args);
  addtoclosure(closure161, var_ran);
  struct UserObject *uo161 = (struct UserObject*)obj73;
  uo161->data[3] = (Object)closure161;
  Method *meth_meth_PackageManager_flag_40__41_do161 = addmethod2pos(obj73, "flag()do", &meth_PackageManager_flag_40__41_do161, 3);
int argcv_meth_PackageManager_flag_40__41_do161[] = {1, 1};
meth_meth_PackageManager_flag_40__41_do161->type = alloc_MethodType(2, argcv_meth_PackageManager_flag_40__41_do161);
meth_meth_PackageManager_flag_40__41_do161->type->types[0] = type_String;
meth_meth_PackageManager_flag_40__41_do161->type->names[0] = "name";
  meth_meth_PackageManager_flag_40__41_do161->definitionModule = modulename;
  meth_meth_PackageManager_flag_40__41_do161->definitionLine = 81;
  sourceObject = obj73;
  sourceObject = obj73;
  sourceObject = obj73;
objclass73 = obj73->class;
  objclass73->definitionModule = modulename;
  objclass73->definitionLine = 52;
  (*(struct UserObject *)self).data[0] = lowerouter73;
  self = oldself73;
  selfslot = oldselfslot73;
  stackframe = oldstackframe73;
// compilenode returning obj73
  *var_on = obj73;
  if (obj73 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 100
  setline(100);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe172 = gc_frame_new();
// Begin line 89
  setline(89);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block173 = alloc_Block(NULL, NULL, "PackageManager", 89);
  gc_frame_newslot(block173);
  block_savedest(block173);
  Object closure174 = createclosure(3, "_apply");
setclosureframe(closure174, stackframe);
  addtoclosure(closure174, var_i);
  addtoclosure(closure174, var_size);
  addtoclosure(closure174, selfslot);
  struct UserObject *uo174 = (struct UserObject*)block173;
  uo174->data[0] = (Object)closure174;
  Method *meth_meth_PackageManager__apply174 = addmethod2pos(block173, "_apply", &meth_PackageManager__apply174, 0);
int argcv_meth_PackageManager__apply174[] = {0};
meth_meth_PackageManager__apply174->type = alloc_MethodType(1, argcv_meth_PackageManager__apply174);
  meth_meth_PackageManager__apply174->definitionModule = modulename;
  meth_meth_PackageManager__apply174->definitionLine = 84;
// compilenode returning block173
  gc_frame_newslot(block173);
// Begin line 100
  setline(100);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block179 = alloc_Block(NULL, NULL, "PackageManager", 100);
  gc_frame_newslot(block179);
  block_savedest(block179);
  Object closure180 = createclosure(6, "_apply");
setclosureframe(closure180, stackframe);
  addtoclosure(closure180, var_i);
  addtoclosure(closure180, var_args);
  addtoclosure(closure180, var_ran);
  addtoclosure(closure180, var_on);
  addtoclosure(closure180, var_block);
  addtoclosure(closure180, selfslot);
  struct UserObject *uo180 = (struct UserObject*)block179;
  uo180->data[0] = (Object)closure180;
  Method *meth_meth_PackageManager__apply180 = addmethod2pos(block179, "_apply", &meth_PackageManager__apply180, 0);
int argcv_meth_PackageManager__apply180[] = {0};
meth_meth_PackageManager__apply180->type = alloc_MethodType(1, argcv_meth_PackageManager__apply180);
  meth_meth_PackageManager__apply180->definitionModule = modulename;
  meth_meth_PackageManager__apply180->definitionLine = 84;
// compilenode returning block179
  gc_frame_newslot(block179);
  params[0] = block173;
  params[1] = block179;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call219 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe172);
// compilenode returning call219
  gc_frame_end(frame);
  return call219;
}
Object meth_PackageManager_forArgs_40__41_do68_object(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 49
  setline(49);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 48
  setline(48);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num69 = alloc_Float64(2);
// compilenode returning num69
  *var_i = num69;
  if (num69 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 50
  setline(50);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 49
  setline(49);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool70 = alloc_Boolean(0);
// compilenode returning bool70
  *var_ran = bool70;
  if (bool70 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 52
  setline(52);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 50
  setline(50);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe71 = gc_frame_new();
// compilenode returning *var_args
  partcv[0] = 0;
  Object call72 = callmethod(*var_args, "size",
    1, partcv, params);
  gc_frame_end(callframe71);
// compilenode returning call72
// compilenode returning call72
  *var_size = call72;
  if (call72 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 52
  setline(52);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object inheritingObject73 = inheritingObject;
  if (isTailObject) {
    isTailObject = 0;
    inheritingObject = NULL;
  }
  Object obj73 = alloc_userobj2(3,4, objclass73);
  gc_frame_newslot(obj73);
// OBJECT OUTER DEC outer
  adddatum2(obj73, self, 0);
  addmethodreal(obj73,"outer", &reader_PackageManager_outer_74);
  Object oldself73 = self;
  struct StackFrameObject *oldstackframe73 = stackframe;
  stackframe = alloc_StackFrame(1, oldstackframe73);
  gc_frame_newslot((Object)stackframe);
  self = obj73;
  Object *oldselfslot73 = selfslot;
  selfslot = &stackframe->slots[0];
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  Object thisouter73 = (*(struct UserObject *)self).data[0], lowerouter73 = thisouter73;
  if (inheritingObject73) {
    struct UserObject *inho73 = (struct UserObject *)inheritingObject73;
    while (inho73->super != GraceDefaultObject) inho73 = (struct UserObject *)inho73->super;
    inho73->super = obj73;
    self = inheritingObject73;
    *selfslot = self;
    lowerouter73 = (*(struct UserObject *)self).data[0];
    (*(struct UserObject *)self).data[0] = thisouter73;
  }
  block_savedest(obj73);
  Object closure75 = createclosure(3, "option()do");
setclosureframe(closure75, stackframe);
  addtoclosure(closure75, var_i);
  addtoclosure(closure75, var_args);
  addtoclosure(closure75, var_ran);
  struct UserObject *uo75 = (struct UserObject*)obj73;
  uo75->data[1] = (Object)closure75;
  Method *meth_meth_PackageManager_option_40__41_do75 = addmethod2pos(obj73, "option()do", &meth_PackageManager_option_40__41_do75, 1);
int argcv_meth_PackageManager_option_40__41_do75[] = {1, 1};
meth_meth_PackageManager_option_40__41_do75->type = alloc_MethodType(2, argcv_meth_PackageManager_option_40__41_do75);
meth_meth_PackageManager_option_40__41_do75->type->types[0] = type_String;
meth_meth_PackageManager_option_40__41_do75->type->names[0] = "name";
  meth_meth_PackageManager_option_40__41_do75->definitionModule = modulename;
  meth_meth_PackageManager_option_40__41_do75->definitionLine = 53;
  block_savedest(obj73);
  Object closure111 = createclosure(3, "doubleValue()do");
setclosureframe(closure111, stackframe);
  addtoclosure(closure111, var_i);
  addtoclosure(closure111, var_args);
  addtoclosure(closure111, var_ran);
  struct UserObject *uo111 = (struct UserObject*)obj73;
  uo111->data[2] = (Object)closure111;
  Method *meth_meth_PackageManager_doubleValue_40__41_do111 = addmethod2pos(obj73, "doubleValue()do", &meth_PackageManager_doubleValue_40__41_do111, 2);
int argcv_meth_PackageManager_doubleValue_40__41_do111[] = {1, 1};
meth_meth_PackageManager_doubleValue_40__41_do111->type = alloc_MethodType(2, argcv_meth_PackageManager_doubleValue_40__41_do111);
meth_meth_PackageManager_doubleValue_40__41_do111->type->types[0] = type_String;
meth_meth_PackageManager_doubleValue_40__41_do111->type->names[0] = "name";
  meth_meth_PackageManager_doubleValue_40__41_do111->definitionModule = modulename;
  meth_meth_PackageManager_doubleValue_40__41_do111->definitionLine = 66;
  block_savedest(obj73);
  Object closure161 = createclosure(3, "flag()do");
setclosureframe(closure161, stackframe);
  addtoclosure(closure161, var_i);
  addtoclosure(closure161, var_args);
  addtoclosure(closure161, var_ran);
  struct UserObject *uo161 = (struct UserObject*)obj73;
  uo161->data[3] = (Object)closure161;
  Method *meth_meth_PackageManager_flag_40__41_do161 = addmethod2pos(obj73, "flag()do", &meth_PackageManager_flag_40__41_do161, 3);
int argcv_meth_PackageManager_flag_40__41_do161[] = {1, 1};
meth_meth_PackageManager_flag_40__41_do161->type = alloc_MethodType(2, argcv_meth_PackageManager_flag_40__41_do161);
meth_meth_PackageManager_flag_40__41_do161->type->types[0] = type_String;
meth_meth_PackageManager_flag_40__41_do161->type->names[0] = "name";
  meth_meth_PackageManager_flag_40__41_do161->definitionModule = modulename;
  meth_meth_PackageManager_flag_40__41_do161->definitionLine = 81;
  sourceObject = obj73;
  sourceObject = obj73;
  sourceObject = obj73;
objclass73 = obj73->class;
  objclass73->definitionModule = modulename;
  objclass73->definitionLine = 52;
  (*(struct UserObject *)self).data[0] = lowerouter73;
  self = oldself73;
  selfslot = oldselfslot73;
  stackframe = oldstackframe73;
// compilenode returning obj73
  *var_on = obj73;
  if (obj73 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 100
  setline(100);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe172 = gc_frame_new();
// Begin line 89
  setline(89);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block173 = alloc_Block(NULL, NULL, "PackageManager", 89);
  gc_frame_newslot(block173);
  block_savedest(block173);
  Object closure174 = createclosure(3, "_apply");
setclosureframe(closure174, stackframe);
  addtoclosure(closure174, var_i);
  addtoclosure(closure174, var_size);
  addtoclosure(closure174, selfslot);
  struct UserObject *uo174 = (struct UserObject*)block173;
  uo174->data[0] = (Object)closure174;
  Method *meth_meth_PackageManager__apply174 = addmethod2pos(block173, "_apply", &meth_PackageManager__apply174, 0);
int argcv_meth_PackageManager__apply174[] = {0};
meth_meth_PackageManager__apply174->type = alloc_MethodType(1, argcv_meth_PackageManager__apply174);
  meth_meth_PackageManager__apply174->definitionModule = modulename;
  meth_meth_PackageManager__apply174->definitionLine = 84;
// compilenode returning block173
  gc_frame_newslot(block173);
// Begin line 100
  setline(100);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block179 = alloc_Block(NULL, NULL, "PackageManager", 100);
  gc_frame_newslot(block179);
  block_savedest(block179);
  Object closure180 = createclosure(6, "_apply");
setclosureframe(closure180, stackframe);
  addtoclosure(closure180, var_i);
  addtoclosure(closure180, var_args);
  addtoclosure(closure180, var_ran);
  addtoclosure(closure180, var_on);
  addtoclosure(closure180, var_block);
  addtoclosure(closure180, selfslot);
  struct UserObject *uo180 = (struct UserObject*)block179;
  uo180->data[0] = (Object)closure180;
  Method *meth_meth_PackageManager__apply180 = addmethod2pos(block179, "_apply", &meth_PackageManager__apply180, 0);
int argcv_meth_PackageManager__apply180[] = {0};
meth_meth_PackageManager__apply180->type = alloc_MethodType(1, argcv_meth_PackageManager__apply180);
  meth_meth_PackageManager__apply180->definitionModule = modulename;
  meth_meth_PackageManager__apply180->definitionLine = 84;
// compilenode returning block179
  gc_frame_newslot(block179);
  params[0] = block173;
  params[1] = block179;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call219 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe172);
// compilenode returning call219
  gc_frame_end(frame);
  return call219;
}
Object meth_PackageManager_listInstalled221(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 104
  setline(104);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 103
  setline(103);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit222 == NULL) {
    strlit222 = alloc_String("Installed packages:");
    gc_root(strlit222);
  }
// compilenode returning strlit222
  params[0] = strlit222;
  Object call223 = gracelib_print(NULL, 1,  params);
// compilenode returning call223
// Begin line 105
  setline(105);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe224 = gc_frame_new();
// Begin line 104
  setline(104);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit225 == NULL) {
    strlit225 = alloc_String("usr/lib/grace/packages/");
    gc_root(strlit225);
  }
// compilenode returning strlit225
  gc_frame_newslot(strlit225);
  params[0] = strlit225;
  partcv[0] = 1;
  Object call226 = callmethodflags(self, "checkListPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe224);
// compilenode returning call226
// Begin line 106
  setline(106);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe227 = gc_frame_new();
// Begin line 105
  setline(105);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit228 == NULL) {
    strlit228 = alloc_String("usr/lib/grace/packages/");
    gc_root(strlit228);
  }
// compilenode returning strlit228
  gc_frame_newslot(strlit228);
  params[0] = strlit228;
  partcv[0] = 1;
  Object call229 = callmethodflags(self, "checkListPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe227);
// compilenode returning call229
// Begin line 107
  setline(107);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe230 = gc_frame_new();
// Begin line 106
  setline(106);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe232 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call233 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe232);
// compilenode returning call233
// compilenode returning call233
  if (strlit234 == NULL) {
    strlit234 = alloc_String("HOME");
    gc_root(strlit234);
  }
// compilenode returning strlit234
  params[0] = strlit234;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres235 = callmethod(call233, "[]", 1, partcv, params);
// compilenode returning idxres235
  int op_slot_left_231 = gc_frame_newslot(idxres235);
  if (strlit236 == NULL) {
    strlit236 = alloc_String("/grace/packages/");
    gc_root(strlit236);
  }
// compilenode returning strlit236
  int op_slot_right_231 = gc_frame_newslot(strlit236);
  params[0] = strlit236;
  partcv[0] = 1;
  Object opresult238 = callmethod(idxres235, "++", 1, partcv, params);
// compilenode returning opresult238
  gc_frame_newslot(opresult238);
  params[0] = opresult238;
  partcv[0] = 1;
  Object call239 = callmethodflags(self, "checkListPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe230);
// compilenode returning call239
// Begin line 108
  setline(108);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe240 = gc_frame_new();
// Begin line 107
  setline(107);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe242 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call243 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe242);
// compilenode returning call243
// compilenode returning call243
  if (strlit244 == NULL) {
    strlit244 = alloc_String("HOME");
    gc_root(strlit244);
  }
// compilenode returning strlit244
  params[0] = strlit244;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres245 = callmethod(call243, "[]", 1, partcv, params);
// compilenode returning idxres245
  int op_slot_left_241 = gc_frame_newslot(idxres245);
  if (strlit246 == NULL) {
    strlit246 = alloc_String("/.local/lib/grace/modules/");
    gc_root(strlit246);
  }
// compilenode returning strlit246
  int op_slot_right_241 = gc_frame_newslot(strlit246);
  params[0] = strlit246;
  partcv[0] = 1;
  Object opresult248 = callmethod(idxres245, "++", 1, partcv, params);
// compilenode returning opresult248
  gc_frame_newslot(opresult248);
  params[0] = opresult248;
  partcv[0] = 1;
  Object call249 = callmethodflags(self, "checkListPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe240);
// compilenode returning call249
  gc_frame_end(frame);
  return call249;
}
Object meth_PackageManager_checkListPath250(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 116
  setline(116);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 112
  setline(112);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe252 = gc_frame_new();
// compilenode returning *var_path
  gc_frame_newslot(*var_path);
// compilenode returning *var_io
  params[0] = *var_path;
  partcv[0] = 1;
  Object call253 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe252);
// compilenode returning call253
  Object if251 = done;
  if (istrue(call253)) {
// Begin line 114
  setline(114);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 113
  setline(113);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit256 == NULL) {
    strlit256 = alloc_String(" Installed in path: ");
    gc_root(strlit256);
  }
// compilenode returning strlit256
  int op_slot_left_255 = gc_frame_newslot(strlit256);
// compilenode returning *var_path
  int op_slot_right_255 = gc_frame_newslot(*var_path);
  params[0] = *var_path;
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
  params[0] = opresult261;
  Object call262 = gracelib_print(NULL, 1,  params);
// compilenode returning call262
// Begin line 115
  setline(115);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe263 = gc_frame_new();
// Begin line 114
  setline(114);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_path
  gc_frame_newslot(*var_path);
  if (strlit264 == NULL) {
    strlit264 = alloc_String(" ");
    gc_root(strlit264);
  }
// compilenode returning strlit264
  gc_frame_newslot(strlit264);
  params[0] = *var_path;
  params[1] = strlit264;
  partcv[0] = 2;
  Object call265 = callmethodflags(self, "recurseDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe263);
// compilenode returning call265
    gc_frame_newslot(call265);
    if251 = call265;
  } else {
  }
// compilenode returning if251
  gc_frame_end(frame);
  return if251;
}
Object meth_PackageManager__apply271(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 127
  setline(127);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 121
  setline(121);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_p
  int op_slot_left_274 = gc_frame_newslot(*var_p);
  if (strlit275 == NULL) {
    strlit275 = alloc_String("..");
    gc_root(strlit275);
  }
// compilenode returning strlit275
  int op_slot_right_274 = gc_frame_newslot(strlit275);
  params[0] = strlit275;
  partcv[0] = 1;
  Object opresult277 = callmethod(*var_p, "!=", 1, partcv, params);
// compilenode returning opresult277
  int op_slot_left_273 = gc_frame_newslot(opresult277);
// compilenode returning *var_p
  int op_slot_left_278 = gc_frame_newslot(*var_p);
  if (strlit279 == NULL) {
    strlit279 = alloc_String(".");
    gc_root(strlit279);
  }
// compilenode returning strlit279
  int op_slot_right_278 = gc_frame_newslot(strlit279);
  params[0] = strlit279;
  partcv[0] = 1;
  Object opresult281 = callmethod(*var_p, "!=", 1, partcv, params);
// compilenode returning opresult281
  int op_slot_right_273 = gc_frame_newslot(opresult281);
  params[0] = opresult281;
  partcv[0] = 1;
  Object opresult283 = callmethod(opresult277, "&&", 1, partcv, params);
// compilenode returning opresult283
  Object if272 = done;
  if (istrue(opresult283)) {
// Begin line 123
  setline(123);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 122
  setline(122);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_padding
  int op_slot_left_284 = gc_frame_newslot(*var_padding);
// compilenode returning *var_p
  int op_slot_right_284 = gc_frame_newslot(*var_p);
  params[0] = *var_p;
  partcv[0] = 1;
  Object opresult286 = callmethod(*var_padding, "++", 1, partcv, params);
// compilenode returning opresult286
  params[0] = opresult286;
  Object call287 = gracelib_print(NULL, 1,  params);
// compilenode returning call287
// Begin line 126
  setline(126);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 123
  setline(123);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe290 = gc_frame_new();
  int callframe291 = gc_frame_new();
// compilenode returning *var_path
  int op_slot_left_293 = gc_frame_newslot(*var_path);
  if (strlit294 == NULL) {
    strlit294 = alloc_String("/");
    gc_root(strlit294);
  }
// compilenode returning strlit294
  int op_slot_right_293 = gc_frame_newslot(strlit294);
  params[0] = strlit294;
  partcv[0] = 1;
  Object opresult296 = callmethod(*var_path, "++", 1, partcv, params);
// compilenode returning opresult296
  int op_slot_left_292 = gc_frame_newslot(opresult296);
// compilenode returning *var_p
  int op_slot_right_292 = gc_frame_newslot(*var_p);
  params[0] = *var_p;
  partcv[0] = 1;
  Object opresult298 = callmethod(opresult296, "++", 1, partcv, params);
// compilenode returning opresult298
  gc_frame_newslot(opresult298);
// compilenode returning *var_io
  params[0] = opresult298;
  partcv[0] = 1;
  Object call299 = callmethod(*var_io, "getdir",
    1, partcv, params);
  gc_frame_end(callframe291);
// compilenode returning call299
  partcv[0] = 0;
  Object call300 = callmethod(call299, "size",
    1, partcv, params);
  gc_frame_end(callframe290);
// compilenode returning call300
// compilenode returning call300
  int op_slot_left_289 = gc_frame_newslot(call300);
  Object num301 = alloc_Float64(0);
// compilenode returning num301
  int op_slot_right_289 = gc_frame_newslot(num301);
  params[0] = num301;
  partcv[0] = 1;
  Object opresult303 = callmethod(call300, ">", 1, partcv, params);
// compilenode returning opresult303
  Object if288 = done;
  if (istrue(opresult303)) {
// Begin line 125
  setline(125);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe304 = gc_frame_new();
// Begin line 124
  setline(124);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_path
  int op_slot_left_306 = gc_frame_newslot(*var_path);
  if (strlit307 == NULL) {
    strlit307 = alloc_String("/");
    gc_root(strlit307);
  }
// compilenode returning strlit307
  int op_slot_right_306 = gc_frame_newslot(strlit307);
  params[0] = strlit307;
  partcv[0] = 1;
  Object opresult309 = callmethod(*var_path, "++", 1, partcv, params);
// compilenode returning opresult309
  int op_slot_left_305 = gc_frame_newslot(opresult309);
// compilenode returning *var_p
  int op_slot_right_305 = gc_frame_newslot(*var_p);
  params[0] = *var_p;
  partcv[0] = 1;
  Object opresult311 = callmethod(opresult309, "++", 1, partcv, params);
// compilenode returning opresult311
  gc_frame_newslot(opresult311);
// compilenode returning *var_padding
  int op_slot_left_312 = gc_frame_newslot(*var_padding);
  if (strlit313 == NULL) {
    strlit313 = alloc_String("   ");
    gc_root(strlit313);
  }
// compilenode returning strlit313
  int op_slot_right_312 = gc_frame_newslot(strlit313);
  params[0] = strlit313;
  partcv[0] = 1;
  Object opresult315 = callmethod(*var_padding, "++", 1, partcv, params);
// compilenode returning opresult315
  gc_frame_newslot(opresult315);
  params[0] = opresult311;
  params[1] = opresult315;
  partcv[0] = 2;
  Object call316 = callmethodflags(self, "recurseDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe304);
// compilenode returning call316
    gc_frame_newslot(call316);
    if288 = call316;
  } else {
  }
// compilenode returning if288
    gc_frame_newslot(if288);
    if272 = if288;
  } else {
  }
// compilenode returning if272
  gc_frame_end(frame);
  return if272;
}
Object meth_PackageManager_recurseDirectory266(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 120
  setline(120);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe267 = gc_frame_new();
// Begin line 119
  setline(119);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_path
  gc_frame_newslot(*var_path);
// compilenode returning *var_io
  params[0] = *var_path;
  partcv[0] = 1;
  Object call268 = callmethod(*var_io, "getdir",
    1, partcv, params);
  gc_frame_end(callframe267);
// compilenode returning call268
  *var_pathContents = call268;
  if (call268 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 128
  setline(128);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe269 = gc_frame_new();
// Begin line 120
  setline(120);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pathContents
  gc_frame_newslot(*var_pathContents);
// Begin line 128
  setline(128);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block270 = alloc_Block(NULL, NULL, "PackageManager", 128);
  gc_frame_newslot(block270);
  block_savedest(block270);
  Object closure271 = createclosure(4, "_apply");
setclosureframe(closure271, stackframe);
  addtoclosure(closure271, var_padding);
  addtoclosure(closure271, var_path);
  addtoclosure(closure271, var_io);
  addtoclosure(closure271, selfslot);
  struct UserObject *uo271 = (struct UserObject*)block270;
  uo271->data[0] = (Object)closure271;
  Method *meth_meth_PackageManager__apply271 = addmethod2pos(block270, "_apply", &meth_PackageManager__apply271, 0);
int argcv_meth_PackageManager__apply271[] = {1};
meth_meth_PackageManager__apply271->type = alloc_MethodType(1, argcv_meth_PackageManager__apply271);
  meth_meth_PackageManager__apply271->definitionModule = modulename;
  meth_meth_PackageManager__apply271->definitionLine = 107;
// compilenode returning block270
  gc_frame_newslot(block270);
  params[0] = *var_pathContents;
  params[1] = block270;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call318 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe269);
// compilenode returning call318
  gc_frame_end(frame);
  return call318;
}
Object meth_PackageManager__apply346(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 138
  setline(138);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe347 = gc_frame_new();
// Begin line 137
  setline(137);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_im
  gc_frame_newslot(*var_im);
  params[0] = *var_im;
  partcv[0] = 1;
  Object call348 = callmethodflags(self, "write", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe347);
// compilenode returning call348
  gc_frame_end(frame);
  return call348;
}
Object meth_PackageManager__apply353(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 141
  setline(141);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe354 = gc_frame_new();
// Begin line 140
  setline(140);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_im
  gc_frame_newslot(*var_im);
  params[0] = *var_im;
  partcv[0] = 1;
  Object call355 = callmethodflags(self, "compile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe354);
// compilenode returning call355
  gc_frame_end(frame);
  return call355;
}
Object meth_PackageManager_doGet319(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 132
  setline(132);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 131
  setline(131);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array320 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array320
  *var_imported = array320;
  if (array320 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 135
  setline(135);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 132
  setline(132);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe325 = gc_frame_new();
// compilenode returning *var_impAddress
  partcv[0] = 0;
  Object call326 = callmethod(*var_impAddress, "size",
    1, partcv, params);
  gc_frame_end(callframe325);
// compilenode returning call326
// compilenode returning call326
  int op_slot_left_324 = gc_frame_newslot(call326);
  Object num327 = alloc_Float64(7);
// compilenode returning num327
  int op_slot_right_324 = gc_frame_newslot(num327);
  params[0] = num327;
  partcv[0] = 1;
  Object opresult329 = callmethod(call326, ">=", 1, partcv, params);
// compilenode returning opresult329
  int op_slot_left_323 = gc_frame_newslot(opresult329);
  int callframe331 = gc_frame_new();
  Object num332 = alloc_Float64(1);
// compilenode returning num332
  gc_frame_newslot(num332);
  Object num333 = alloc_Float64(7);
// compilenode returning num333
  gc_frame_newslot(num333);
// compilenode returning *var_impAddress
  params[0] = num332;
  params[1] = num333;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call334 = callmethod(*var_impAddress, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe331);
// compilenode returning call334
  int op_slot_left_330 = gc_frame_newslot(call334);
  if (strlit335 == NULL) {
    strlit335 = alloc_String("http://");
    gc_root(strlit335);
  }
// compilenode returning strlit335
  int op_slot_right_330 = gc_frame_newslot(strlit335);
  params[0] = strlit335;
  partcv[0] = 1;
  Object opresult337 = callmethod(call334, "==", 1, partcv, params);
// compilenode returning opresult337
  int op_slot_right_323 = gc_frame_newslot(opresult337);
  params[0] = opresult337;
  partcv[0] = 1;
  Object opresult339 = callmethod(opresult329, "&&", 1, partcv, params);
// compilenode returning opresult339
  Object if322 = done;
  if (istrue(opresult339)) {
// Begin line 134
  setline(134);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe340 = gc_frame_new();
// Begin line 133
  setline(133);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_impAddress
  gc_frame_newslot(*var_impAddress);
  params[0] = *var_impAddress;
  partcv[0] = 1;
  Object call341 = callmethodflags(self, "setBaseUrl", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe340);
// compilenode returning call341
    gc_frame_newslot(call341);
    if322 = call341;
  } else {
  }
// compilenode returning if322
// Begin line 136
  setline(136);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe342 = gc_frame_new();
// Begin line 135
  setline(135);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_impAddress
  gc_frame_newslot(*var_impAddress);
  params[0] = *var_impAddress;
  partcv[0] = 1;
  Object call343 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe342);
// compilenode returning call343
// Begin line 139
  setline(139);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe344 = gc_frame_new();
// Begin line 136
  setline(136);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_imported
  gc_frame_newslot(*var_imported);
// Begin line 139
  setline(139);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block345 = alloc_Block(NULL, NULL, "PackageManager", 139);
  gc_frame_newslot(block345);
  block_savedest(block345);
  Object closure346 = createclosure(1, "_apply");
setclosureframe(closure346, stackframe);
  addtoclosure(closure346, selfslot);
  struct UserObject *uo346 = (struct UserObject*)block345;
  uo346->data[0] = (Object)closure346;
  Method *meth_meth_PackageManager__apply346 = addmethod2pos(block345, "_apply", &meth_PackageManager__apply346, 0);
int argcv_meth_PackageManager__apply346[] = {1};
meth_meth_PackageManager__apply346->type = alloc_MethodType(1, argcv_meth_PackageManager__apply346);
  meth_meth_PackageManager__apply346->definitionModule = modulename;
  meth_meth_PackageManager__apply346->definitionLine = 132;
// compilenode returning block345
  gc_frame_newslot(block345);
  params[0] = *var_imported;
  params[1] = block345;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call350 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe344);
// compilenode returning call350
// Begin line 142
  setline(142);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe351 = gc_frame_new();
// Begin line 139
  setline(139);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_imported
  gc_frame_newslot(*var_imported);
// Begin line 142
  setline(142);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block352 = alloc_Block(NULL, NULL, "PackageManager", 142);
  gc_frame_newslot(block352);
  block_savedest(block352);
  Object closure353 = createclosure(1, "_apply");
setclosureframe(closure353, stackframe);
  addtoclosure(closure353, selfslot);
  struct UserObject *uo353 = (struct UserObject*)block352;
  uo353->data[0] = (Object)closure353;
  Method *meth_meth_PackageManager__apply353 = addmethod2pos(block352, "_apply", &meth_PackageManager__apply353, 0);
int argcv_meth_PackageManager__apply353[] = {1};
meth_meth_PackageManager__apply353->type = alloc_MethodType(1, argcv_meth_PackageManager__apply353);
  meth_meth_PackageManager__apply353->definitionModule = modulename;
  meth_meth_PackageManager__apply353->definitionLine = 132;
// compilenode returning block352
  gc_frame_newslot(block352);
  params[0] = *var_imported;
  params[1] = block352;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call357 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe351);
// compilenode returning call357
  gc_frame_end(frame);
  return call357;
}
Object meth_PackageManager_setBaseUrl358(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 7, (flags>>24)&0xff);
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
// Begin line 146
  setline(146);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe359 = gc_frame_new();
// Begin line 145
  setline(145);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_baseAddress
  gc_frame_newslot(*var_baseAddress);
  params[0] = *var_baseAddress;
  partcv[0] = 1;
  Object call360 = callmethodflags(self, "getBaseUrl", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe359);
// compilenode returning call360
  *var_baseUrl = call360;
  if (call360 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_setFile362(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 8, (flags>>24)&0xff);
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
// Begin line 157
  setline(157);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 149
  setline(149);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe365 = gc_frame_new();
  int callframe367 = gc_frame_new();
// compilenode returning *var_fileAddress
  partcv[0] = 0;
  Object call368 = callmethod(*var_fileAddress, "size",
    1, partcv, params);
  gc_frame_end(callframe367);
// compilenode returning call368
// compilenode returning call368
  int op_slot_left_366 = gc_frame_newslot(call368);
  Object num369 = alloc_Float64(5);
// compilenode returning num369
  int op_slot_right_366 = gc_frame_newslot(num369);
  params[0] = num369;
  partcv[0] = 1;
  Object diff371 = callmethod(call368, "-", 1, partcv, params);
// compilenode returning diff371
  gc_frame_newslot(diff371);
  int callframe372 = gc_frame_new();
// compilenode returning *var_fileAddress
  partcv[0] = 0;
  Object call373 = callmethod(*var_fileAddress, "size",
    1, partcv, params);
  gc_frame_end(callframe372);
// compilenode returning call373
// compilenode returning call373
  gc_frame_newslot(call373);
// compilenode returning *var_fileAddress
  params[0] = diff371;
  params[1] = call373;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call374 = callmethod(*var_fileAddress, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe365);
// compilenode returning call374
  int op_slot_left_364 = gc_frame_newslot(call374);
  if (strlit375 == NULL) {
    strlit375 = alloc_String(".grace");
    gc_root(strlit375);
  }
// compilenode returning strlit375
  int op_slot_right_364 = gc_frame_newslot(strlit375);
  params[0] = strlit375;
  partcv[0] = 1;
  Object opresult377 = callmethod(call374, "==", 1, partcv, params);
// compilenode returning opresult377
  Object if363 = done;
  if (istrue(opresult377)) {
// Begin line 150
  setline(150);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object inheritingObject378 = inheritingObject;
  if (isTailObject) {
    isTailObject = 0;
    inheritingObject = NULL;
  }
  Object obj378 = alloc_userobj2(4,4, objclass378);
  gc_frame_newslot(obj378);
// OBJECT OUTER DEC outer
  adddatum2(obj378, self, 0);
  addmethodreal(obj378,"outer", &reader_PackageManager_outer_379);
  Object oldself378 = self;
  struct StackFrameObject *oldstackframe378 = stackframe;
  stackframe = alloc_StackFrame(1, oldstackframe378);
  gc_frame_newslot((Object)stackframe);
  self = obj378;
  Object *oldselfslot378 = selfslot;
  selfslot = &stackframe->slots[0];
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  Object thisouter378 = (*(struct UserObject *)self).data[0], lowerouter378 = thisouter378;
  if (inheritingObject378) {
    struct UserObject *inho378 = (struct UserObject *)inheritingObject378;
    while (inho378->super != GraceDefaultObject) inho378 = (struct UserObject *)inho378->super;
    inho378->super = obj378;
    self = inheritingObject378;
    *selfslot = self;
    lowerouter378 = (*(struct UserObject *)self).data[0];
    (*(struct UserObject *)self).data[0] = thisouter378;
  }
if (objclass378 == NULL) {
  Method *reader380 = addmethodrealflags(obj378, "address",&reader_PackageManager_address_380, 0);
  Method *writer380 = addmethodrealflags(obj378, "address:=",&writer_PackageManager_address_380, 0);
  reader380->definitionModule = modulename;
  writer380->definitionModule = modulename;
  reader380->definitionLine = 152;
  writer380->definitionLine = 152;
}
obj378->flags |= OFLAG_MUTABLE;
adddatum2(obj378, alloc_Undefined(), 1);
if (objclass378 == NULL) {
  Method *reader381 = addmethodrealflags(obj378, "data",&reader_PackageManager_data_381, 0);
  Method *writer381 = addmethodrealflags(obj378, "data:=",&writer_PackageManager_data_381, 0);
  reader381->definitionModule = modulename;
  writer381->definitionModule = modulename;
  reader381->definitionLine = 153;
  writer381->definitionLine = 153;
}
obj378->flags |= OFLAG_MUTABLE;
adddatum2(obj378, alloc_Undefined(), 2);
  sourceObject = obj378;
// Begin line 151
  setline(151);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_fileAddress
  adddatum2(obj378, *var_fileAddress, 1);
  sourceObject = obj378;
  adddatum2(obj378, undefined, 2);
objclass378 = obj378->class;
  objclass378->definitionModule = modulename;
  objclass378->definitionLine = 150;
  (*(struct UserObject *)self).data[0] = lowerouter378;
  self = oldself378;
  selfslot = oldselfslot378;
  stackframe = oldstackframe378;
// compilenode returning obj378
  *var_file = obj378;
  if (obj378 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 155
  setline(155);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 154
  setline(154);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_file
  *var_curFile = *var_file;
  if (*var_file == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 156
  setline(156);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 155
  setline(155);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool383 = alloc_Boolean(1);
// compilenode returning bool383
  return bool383;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if363 = undefined;
  } else {
  }
// compilenode returning if363
// Begin line 158
  setline(158);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 157
  setline(157);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool384 = alloc_Boolean(0);
// compilenode returning bool384
  return bool384;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply430(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int callframe432 = gc_frame_new();
// compilenode returning *var_toProcess
  partcv[0] = 0;
  Object call433 = callmethod(*var_toProcess, "size",
    1, partcv, params);
  gc_frame_end(callframe432);
// compilenode returning call433
// compilenode returning call433
  int op_slot_left_431 = gc_frame_newslot(call433);
  Object num434 = alloc_Float64(0);
// compilenode returning num434
  int op_slot_right_431 = gc_frame_newslot(num434);
  params[0] = num434;
  partcv[0] = 1;
  Object opresult436 = callmethod(call433, ">", 1, partcv, params);
// compilenode returning opresult436
  gc_frame_end(frame);
  return opresult436;
}
Object meth_PackageManager__apply439(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 173
  setline(173);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe440 = gc_frame_new();
// Begin line 172
  setline(172);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe441 = gc_frame_new();
// compilenode returning *var_toProcess
  partcv[0] = 0;
  Object call442 = callmethod(*var_toProcess, "pop",
    1, partcv, params);
  gc_frame_end(callframe441);
// compilenode returning call442
// compilenode returning call442
  gc_frame_newslot(call442);
  params[0] = call442;
  partcv[0] = 1;
  Object call443 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe440);
// compilenode returning call443
  gc_frame_end(frame);
  return call443;
}
Object meth_PackageManager_fetchImports385(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 9, (flags>>24)&0xff);
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
// Begin line 183
  setline(183);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 161
  setline(161);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe387 = gc_frame_new();
// compilenode returning *var_fileAddress
  gc_frame_newslot(*var_fileAddress);
  params[0] = *var_fileAddress;
  partcv[0] = 1;
  Object call388 = callmethodflags(self, "setFile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe387);
// compilenode returning call388
  Object if386 = done;
  if (istrue(call388)) {
// Begin line 163
  setline(163);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 162
  setline(162);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit391 == NULL) {
    strlit391 = alloc_String("File address ");
    gc_root(strlit391);
  }
// compilenode returning strlit391
  int op_slot_left_390 = gc_frame_newslot(strlit391);
  int callframe392 = gc_frame_new();
// compilenode returning *var_curFile
  partcv[0] = 0;
  Object call393 = callmethod(*var_curFile, "address",
    1, partcv, params);
  gc_frame_end(callframe392);
// compilenode returning call393
// compilenode returning call393
  int op_slot_right_390 = gc_frame_newslot(call393);
  params[0] = call393;
  partcv[0] = 1;
  Object opresult395 = callmethod(strlit391, "++", 1, partcv, params);
// compilenode returning opresult395
  int op_slot_left_389 = gc_frame_newslot(opresult395);
  if (strlit396 == NULL) {
    strlit396 = alloc_String("");
    gc_root(strlit396);
  }
// compilenode returning strlit396
  int op_slot_right_389 = gc_frame_newslot(strlit396);
  params[0] = strlit396;
  partcv[0] = 1;
  Object opresult398 = callmethod(opresult395, "++", 1, partcv, params);
// compilenode returning opresult398
  params[0] = opresult398;
  Object call399 = gracelib_print(NULL, 1,  params);
// compilenode returning call399
// Begin line 167
  setline(167);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 163
  setline(163);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe402 = gc_frame_new();
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call403 = callmethodflags(self, "setFileData", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe402);
// compilenode returning call403
  int op_slot_left_401 = gc_frame_newslot(call403);
  Object bool404 = alloc_Boolean(0);
// compilenode returning bool404
  int op_slot_right_401 = gc_frame_newslot(bool404);
  params[0] = bool404;
  partcv[0] = 1;
  Object opresult406 = callmethod(call403, "==", 1, partcv, params);
// compilenode returning opresult406
  Object if400 = done;
  if (istrue(opresult406)) {
// Begin line 165
  setline(165);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 164
  setline(164);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit407 == NULL) {
    strlit407 = alloc_String("Could not retrieve file data");
    gc_root(strlit407);
  }
// compilenode returning strlit407
  params[0] = strlit407;
  Object call408 = gracelib_print(NULL, 1,  params);
// compilenode returning call408
// Begin line 166
  setline(166);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 165
  setline(165);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool409 = alloc_Boolean(0);
// compilenode returning bool409
  return bool409;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if400 = undefined;
  } else {
  }
// compilenode returning if400
// Begin line 176
  setline(176);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 167
  setline(167);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe411 = gc_frame_new();
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call412 = callmethodflags(self, "validateFile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe411);
// compilenode returning call412
  Object if410 = done;
  if (istrue(call412)) {
// Begin line 169
  setline(169);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 168
  setline(168);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit415 == NULL) {
    strlit415 = alloc_String("Pushing ");
    gc_root(strlit415);
  }
// compilenode returning strlit415
  int op_slot_left_414 = gc_frame_newslot(strlit415);
  int callframe416 = gc_frame_new();
// compilenode returning *var_curFile
  partcv[0] = 0;
  Object call417 = callmethod(*var_curFile, "address",
    1, partcv, params);
  gc_frame_end(callframe416);
// compilenode returning call417
// compilenode returning call417
  int op_slot_right_414 = gc_frame_newslot(call417);
  params[0] = call417;
  partcv[0] = 1;
  Object opresult419 = callmethod(strlit415, "++", 1, partcv, params);
// compilenode returning opresult419
  int op_slot_left_413 = gc_frame_newslot(opresult419);
  if (strlit420 == NULL) {
    strlit420 = alloc_String(" to imported");
    gc_root(strlit420);
  }
// compilenode returning strlit420
  int op_slot_right_413 = gc_frame_newslot(strlit420);
  params[0] = strlit420;
  partcv[0] = 1;
  Object opresult422 = callmethod(opresult419, "++", 1, partcv, params);
// compilenode returning opresult422
  params[0] = opresult422;
  Object call423 = gracelib_print(NULL, 1,  params);
// compilenode returning call423
// Begin line 170
  setline(170);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe424 = gc_frame_new();
// Begin line 169
  setline(169);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
// compilenode returning *var_imported
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call425 = callmethod(*var_imported, "push",
    1, partcv, params);
  gc_frame_end(callframe424);
// compilenode returning call425
// Begin line 171
  setline(171);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe426 = gc_frame_new();
// Begin line 170
  setline(170);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call427 = callmethodflags(self, "parseFile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe426);
// compilenode returning call427
// Begin line 174
  setline(174);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe428 = gc_frame_new();
// Begin line 171
  setline(171);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block429 = alloc_Block(NULL, NULL, "PackageManager", 171);
  gc_frame_newslot(block429);
  block_savedest(block429);
  Object closure430 = createclosure(2, "_apply");
setclosureframe(closure430, stackframe);
  addtoclosure(closure430, var_toProcess);
  addtoclosure(closure430, selfslot);
  struct UserObject *uo430 = (struct UserObject*)block429;
  uo430->data[0] = (Object)closure430;
  Method *meth_meth_PackageManager__apply430 = addmethod2pos(block429, "_apply", &meth_PackageManager__apply430, 0);
int argcv_meth_PackageManager__apply430[] = {0};
meth_meth_PackageManager__apply430->type = alloc_MethodType(1, argcv_meth_PackageManager__apply430);
  meth_meth_PackageManager__apply430->definitionModule = modulename;
  meth_meth_PackageManager__apply430->definitionLine = 168;
// compilenode returning block429
  gc_frame_newslot(block429);
// Begin line 174
  setline(174);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block438 = alloc_Block(NULL, NULL, "PackageManager", 174);
  gc_frame_newslot(block438);
  block_savedest(block438);
  Object closure439 = createclosure(2, "_apply");
setclosureframe(closure439, stackframe);
  addtoclosure(closure439, var_toProcess);
  addtoclosure(closure439, selfslot);
  struct UserObject *uo439 = (struct UserObject*)block438;
  uo439->data[0] = (Object)closure439;
  Method *meth_meth_PackageManager__apply439 = addmethod2pos(block438, "_apply", &meth_PackageManager__apply439, 0);
int argcv_meth_PackageManager__apply439[] = {0};
meth_meth_PackageManager__apply439->type = alloc_MethodType(1, argcv_meth_PackageManager__apply439);
  meth_meth_PackageManager__apply439->definitionModule = modulename;
  meth_meth_PackageManager__apply439->definitionLine = 171;
// compilenode returning block438
  gc_frame_newslot(block438);
  params[0] = block429;
  params[1] = block438;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call445 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe428);
// compilenode returning call445
// Begin line 175
  setline(175);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 174
  setline(174);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool446 = alloc_Boolean(1);
// compilenode returning bool446
  return bool446;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if410 = undefined;
  } else {
  }
// compilenode returning if410
    gc_frame_newslot(if410);
    if386 = if410;
  } else {
// Begin line 182
  setline(182);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 178
  setline(178);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe449 = gc_frame_new();
// compilenode returning *var_fileAddress
  int op_slot_left_450 = gc_frame_newslot(*var_fileAddress);
  if (strlit451 == NULL) {
    strlit451 = alloc_String(".grace");
    gc_root(strlit451);
  }
// compilenode returning strlit451
  int op_slot_right_450 = gc_frame_newslot(strlit451);
  params[0] = strlit451;
  partcv[0] = 1;
  Object opresult453 = callmethod(*var_fileAddress, "++", 1, partcv, params);
// compilenode returning opresult453
  gc_frame_newslot(opresult453);
  params[0] = opresult453;
  partcv[0] = 1;
  Object call454 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe449);
// compilenode returning call454
  int op_slot_left_448 = gc_frame_newslot(call454);
  Object bool455 = alloc_Boolean(0);
// compilenode returning bool455
  int op_slot_right_448 = gc_frame_newslot(bool455);
  params[0] = bool455;
  partcv[0] = 1;
  Object opresult457 = callmethod(call454, "==", 1, partcv, params);
// compilenode returning opresult457
  Object if447 = done;
  if (istrue(opresult457)) {
// Begin line 180
  setline(180);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 179
  setline(179);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit458 == NULL) {
    strlit458 = alloc_String("Could not locate file. Check file address.");
    gc_root(strlit458);
  }
// compilenode returning strlit458
  params[0] = strlit458;
  Object call459 = gracelib_print(NULL, 1,  params);
// compilenode returning call459
// Begin line 181
  setline(181);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 180
  setline(180);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool460 = alloc_Boolean(0);
// compilenode returning bool460
  return bool460;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if447 = undefined;
  } else {
  }
// compilenode returning if447
    gc_frame_newslot(if447);
    if386 = if447;
  }
// compilenode returning if386
  gc_frame_end(frame);
  return if386;
}
Object meth_PackageManager__apply479(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 192
  setline(192);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 191
  setline(191);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit480 == NULL) {
    strlit480 = alloc_String(" Response received");
    gc_root(strlit480);
  }
// compilenode returning strlit480
  params[0] = strlit480;
  Object call481 = gracelib_print(NULL, 1,  params);
// compilenode returning call481
// Begin line 196
  setline(196);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 192
  setline(192);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe484 = gc_frame_new();
// compilenode returning *var_req
  partcv[0] = 0;
  Object call485 = callmethod(*var_req, "responseCode",
    1, partcv, params);
  gc_frame_end(callframe484);
// compilenode returning call485
// compilenode returning call485
  int op_slot_left_483 = gc_frame_newslot(call485);
  Object num486 = alloc_Float64(200);
// compilenode returning num486
  int op_slot_right_483 = gc_frame_newslot(num486);
  params[0] = num486;
  partcv[0] = 1;
  Object opresult488 = callmethod(call485, "!=", 1, partcv, params);
// compilenode returning opresult488
  Object if482 = done;
  if (istrue(opresult488)) {
// Begin line 194
  setline(194);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 193
  setline(193);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit493 == NULL) {
    strlit493 = alloc_String(" Could not locate import: Error ");
    gc_root(strlit493);
  }
// compilenode returning strlit493
  int op_slot_left_492 = gc_frame_newslot(strlit493);
  int callframe494 = gc_frame_new();
// compilenode returning *var_req
  partcv[0] = 0;
  Object call495 = callmethod(*var_req, "responseCode",
    1, partcv, params);
  gc_frame_end(callframe494);
// compilenode returning call495
// compilenode returning call495
  int op_slot_right_492 = gc_frame_newslot(call495);
  params[0] = call495;
  partcv[0] = 1;
  Object opresult497 = callmethod(strlit493, "++", 1, partcv, params);
// compilenode returning opresult497
  int op_slot_left_491 = gc_frame_newslot(opresult497);
  if (strlit498 == NULL) {
    strlit498 = alloc_String(" for import ");
    gc_root(strlit498);
  }
// compilenode returning strlit498
  int op_slot_right_491 = gc_frame_newslot(strlit498);
  params[0] = strlit498;
  partcv[0] = 1;
  Object opresult500 = callmethod(opresult497, "++", 1, partcv, params);
// compilenode returning opresult500
  int op_slot_left_490 = gc_frame_newslot(opresult500);
  int callframe501 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call502 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe501);
// compilenode returning call502
// compilenode returning call502
  int op_slot_right_490 = gc_frame_newslot(call502);
  params[0] = call502;
  partcv[0] = 1;
  Object opresult504 = callmethod(opresult500, "++", 1, partcv, params);
// compilenode returning opresult504
  int op_slot_left_489 = gc_frame_newslot(opresult504);
  if (strlit505 == NULL) {
    strlit505 = alloc_String("");
    gc_root(strlit505);
  }
// compilenode returning strlit505
  int op_slot_right_489 = gc_frame_newslot(strlit505);
  params[0] = strlit505;
  partcv[0] = 1;
  Object opresult507 = callmethod(opresult504, "++", 1, partcv, params);
// compilenode returning opresult507
  params[0] = opresult507;
  Object call508 = gracelib_print(NULL, 1,  params);
// compilenode returning call508
// Begin line 195
  setline(195);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 194
  setline(194);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool509 = alloc_Boolean(0);
// compilenode returning bool509
  block_return(realself, bool509);
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if482 = undefined;
  } else {
  }
// compilenode returning if482
// Begin line 197
  setline(197);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 193
  setline(193);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe510 = gc_frame_new();
// Begin line 197
  setline(197);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe511 = gc_frame_new();
// Begin line 196
  setline(196);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit512 == NULL) {
    strlit512 = alloc_String("utf-8");
    gc_root(strlit512);
  }
// compilenode returning strlit512
  gc_frame_newslot(strlit512);
// compilenode returning *var_d
  params[0] = strlit512;
  partcv[0] = 1;
  Object call513 = callmethod(*var_d, "decode",
    1, partcv, params);
  gc_frame_end(callframe511);
// compilenode returning call513
  gc_frame_newslot(call513);
// compilenode returning *var_file
  params[0] = call513;
  partcv[0] = 1;
  Object call514 = callmethod(*var_file, "data:=",
    1, partcv, params);
  gc_frame_end(callframe510);
// compilenode returning call514
// compilenode returning done
// Begin line 198
  setline(198);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 197
  setline(197);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool515 = alloc_Boolean(1);
// compilenode returning bool515
  block_return(realself, bool515);
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_performCurlFetch461(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 10, (flags>>24)&0xff);
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
// Begin line 187
  setline(187);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 186
  setline(186);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe462 = gc_frame_new();
// compilenode returning *var_curl
  partcv[0] = 0;
  Object call463 = callmethod(*var_curl, "easy",
    1, partcv, params);
  gc_frame_end(callframe462);
// compilenode returning call463
// compilenode returning call463
  *var_req = call463;
  if (call463 == undefined)
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
  int callframe464 = gc_frame_new();
// Begin line 187
  setline(187);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe465 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call466 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe465);
// compilenode returning call466
// compilenode returning call466
  gc_frame_newslot(call466);
// compilenode returning *var_req
  params[0] = call466;
  partcv[0] = 1;
  Object call467 = callmethod(*var_req, "url:=",
    1, partcv, params);
  gc_frame_end(callframe464);
// compilenode returning call467
// compilenode returning done
// Begin line 189
  setline(189);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 188
  setline(188);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit469 == NULL) {
    strlit469 = alloc_String("Searching for import: ");
    gc_root(strlit469);
  }
// compilenode returning strlit469
  int op_slot_left_468 = gc_frame_newslot(strlit469);
  int callframe470 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call471 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe470);
// compilenode returning call471
// compilenode returning call471
  int op_slot_right_468 = gc_frame_newslot(call471);
  params[0] = call471;
  partcv[0] = 1;
  Object opresult473 = callmethod(strlit469, "++", 1, partcv, params);
// compilenode returning opresult473
  params[0] = opresult473;
  Object call474 = gracelib_print(NULL, 1,  params);
// compilenode returning call474
// Begin line 190
  setline(190);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 189
  setline(189);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit475 == NULL) {
    strlit475 = alloc_String(" Connecting...");
    gc_root(strlit475);
  }
// compilenode returning strlit475
  params[0] = strlit475;
  Object call476 = gracelib_print(NULL, 1,  params);
// compilenode returning call476
// Begin line 199
  setline(199);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe477 = gc_frame_new();
  Object block478 = alloc_Block(NULL, NULL, "PackageManager", 199);
  gc_frame_newslot(block478);
  block_savedest(block478);
  Object closure479 = createclosure(3, "_apply");
setclosureframe(closure479, stackframe);
  addtoclosure(closure479, var_req);
  addtoclosure(closure479, var_file);
  addtoclosure(closure479, selfslot);
  struct UserObject *uo479 = (struct UserObject*)block478;
  uo479->data[0] = (Object)closure479;
  Method *meth_meth_PackageManager__apply479 = addmethod2pos(block478, "_apply", &meth_PackageManager__apply479, 0);
int argcv_meth_PackageManager__apply479[] = {1};
meth_meth_PackageManager__apply479->type = alloc_MethodType(1, argcv_meth_PackageManager__apply479);
  meth_meth_PackageManager__apply479->definitionModule = modulename;
  meth_meth_PackageManager__apply479->definitionLine = 188;
// compilenode returning block478
  gc_frame_newslot(block478);
// Begin line 190
  setline(190);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_req
  params[0] = block478;
  partcv[0] = 1;
  Object call517 = callmethod(*var_req, "onReceive",
    1, partcv, params);
  gc_frame_end(callframe477);
// compilenode returning call517
// Begin line 199
  setline(199);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe518 = gc_frame_new();
// compilenode returning *var_req
  partcv[0] = 0;
  Object call519 = callmethod(*var_req, "perform",
    1, partcv, params);
  gc_frame_end(callframe518);
// compilenode returning call519
// compilenode returning call519
  gc_frame_end(frame);
  return call519;
}
Object meth_PackageManager_setFileData520(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 11, (flags>>24)&0xff);
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
// Begin line 230
  setline(230);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 203
  setline(203);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe523 = gc_frame_new();
  Object num524 = alloc_Float64(1);
// compilenode returning num524
  gc_frame_newslot(num524);
  Object num525 = alloc_Float64(4);
// compilenode returning num525
  gc_frame_newslot(num525);
  int callframe526 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call527 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe526);
// compilenode returning call527
// compilenode returning call527
  params[0] = num524;
  params[1] = num525;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call528 = callmethod(call527, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe523);
// compilenode returning call528
  int op_slot_left_522 = gc_frame_newslot(call528);
  if (strlit529 == NULL) {
    strlit529 = alloc_String("http");
    gc_root(strlit529);
  }
// compilenode returning strlit529
  int op_slot_right_522 = gc_frame_newslot(strlit529);
  params[0] = strlit529;
  partcv[0] = 1;
  Object opresult531 = callmethod(call528, "==", 1, partcv, params);
// compilenode returning opresult531
  Object if521 = done;
  if (istrue(opresult531)) {
// Begin line 205
  setline(205);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe532 = gc_frame_new();
// Begin line 204
  setline(204);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num533 = alloc_Float64(1);
// compilenode returning num533
  gc_frame_newslot(num533);
  Object num534 = alloc_Float64(4);
// compilenode returning num534
  gc_frame_newslot(num534);
  int callframe535 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call536 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe535);
// compilenode returning call536
// compilenode returning call536
  params[0] = num533;
  params[1] = num534;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call537 = callmethod(call536, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe532);
// compilenode returning call537
  *var_strippedUrl = call537;
  if (call537 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 213
  setline(213);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 205
  setline(205);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe540 = gc_frame_new();
  int callframe541 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call542 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe541);
// compilenode returning call542
// compilenode returning call542
  gc_frame_newslot(call542);
  params[0] = call542;
  partcv[0] = 1;
  Object call543 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe540);
// compilenode returning call543
  int op_slot_left_539 = gc_frame_newslot(call543);
  Object bool544 = alloc_Boolean(0);
// compilenode returning bool544
  int op_slot_right_539 = gc_frame_newslot(bool544);
  params[0] = bool544;
  partcv[0] = 1;
  Object opresult546 = callmethod(call543, "!=", 1, partcv, params);
// compilenode returning opresult546
  Object if538 = done;
  if (istrue(opresult546)) {
// Begin line 207
  setline(207);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe547 = gc_frame_new();
// Begin line 206
  setline(206);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_strippedUrl
  gc_frame_newslot(*var_strippedUrl);
  params[0] = *var_strippedUrl;
  partcv[0] = 1;
  Object call548 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe547);
// compilenode returning call548
  *var_findData = call548;
  if (call548 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 211
  setline(211);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 207
  setline(207);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  int op_slot_left_550 = gc_frame_newslot(*var_findData);
  Object num551 = alloc_Float64(0);
// compilenode returning num551
  int op_slot_right_550 = gc_frame_newslot(num551);
  params[0] = num551;
  partcv[0] = 1;
  Object opresult553 = callmethod(*var_findData, ">", 1, partcv, params);
// compilenode returning opresult553
  Object if549 = done;
  if (istrue(opresult553)) {
// Begin line 209
  setline(209);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 205
  setline(205);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe554 = gc_frame_new();
// Begin line 208
  setline(208);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  gc_frame_newslot(*var_findData);
// compilenode returning *var_file
  params[0] = *var_findData;
  partcv[0] = 1;
  Object call555 = callmethod(*var_file, "data:=",
    1, partcv, params);
  gc_frame_end(callframe554);
// compilenode returning call555
// compilenode returning done
// Begin line 210
  setline(210);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 209
  setline(209);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool556 = alloc_Boolean(1);
// compilenode returning bool556
  return bool556;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if549 = undefined;
  } else {
  }
// compilenode returning if549
// Begin line 211
  setline(211);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool557 = alloc_Boolean(0);
// compilenode returning bool557
  return bool557;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if538 = undefined;
  } else {
  }
// compilenode returning if538
// Begin line 215
  setline(215);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe558 = gc_frame_new();
// Begin line 213
  setline(213);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_file
  gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object call559 = callmethodflags(self, "performCurlFetch", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe558);
// compilenode returning call559
  return call559;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if521 = undefined;
  } else {
// Begin line 224
  setline(224);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 216
  setline(216);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe562 = gc_frame_new();
  int callframe563 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call564 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe563);
// compilenode returning call564
// compilenode returning call564
  gc_frame_newslot(call564);
  params[0] = call564;
  partcv[0] = 1;
  Object call565 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe562);
// compilenode returning call565
  int op_slot_left_561 = gc_frame_newslot(call565);
  Object bool566 = alloc_Boolean(0);
// compilenode returning bool566
  int op_slot_right_561 = gc_frame_newslot(bool566);
  params[0] = bool566;
  partcv[0] = 1;
  Object opresult568 = callmethod(call565, "!=", 1, partcv, params);
// compilenode returning opresult568
  Object if560 = done;
  if (istrue(opresult568)) {
// Begin line 218
  setline(218);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 217
  setline(217);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit569 == NULL) {
    strlit569 = alloc_String("Now searching in find existing");
    gc_root(strlit569);
  }
// compilenode returning strlit569
  params[0] = strlit569;
  Object call570 = gracelib_print(NULL, 1,  params);
// compilenode returning call570
// Begin line 219
  setline(219);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe571 = gc_frame_new();
// Begin line 218
  setline(218);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe572 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call573 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe572);
// compilenode returning call573
// compilenode returning call573
  gc_frame_newslot(call573);
  params[0] = call573;
  partcv[0] = 1;
  Object call574 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe571);
// compilenode returning call574
  *var_findData = call574;
  if (call574 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 223
  setline(223);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 219
  setline(219);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  int op_slot_left_576 = gc_frame_newslot(*var_findData);
  Object bool577 = alloc_Boolean(0);
// compilenode returning bool577
  int op_slot_right_576 = gc_frame_newslot(bool577);
  params[0] = bool577;
  partcv[0] = 1;
  Object opresult579 = callmethod(*var_findData, "!=", 1, partcv, params);
// compilenode returning opresult579
  Object if575 = done;
  if (istrue(opresult579)) {
// Begin line 221
  setline(221);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 218
  setline(218);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe580 = gc_frame_new();
// Begin line 220
  setline(220);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  gc_frame_newslot(*var_findData);
// compilenode returning *var_file
  params[0] = *var_findData;
  partcv[0] = 1;
  Object call581 = callmethod(*var_file, "data:=",
    1, partcv, params);
  gc_frame_end(callframe580);
// compilenode returning call581
// compilenode returning done
// Begin line 222
  setline(222);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 221
  setline(221);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool582 = alloc_Boolean(1);
// compilenode returning bool582
  return bool582;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if575 = undefined;
  } else {
  }
// compilenode returning if575
    gc_frame_newslot(if575);
    if560 = if575;
  } else {
// Begin line 230
  setline(230);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 224
  setline(224);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_baseUrl
  int op_slot_left_584 = gc_frame_newslot(*var_baseUrl);
  if (strlit585 == NULL) {
    strlit585 = alloc_String("");
    gc_root(strlit585);
  }
// compilenode returning strlit585
  int op_slot_right_584 = gc_frame_newslot(strlit585);
  params[0] = strlit585;
  partcv[0] = 1;
  Object opresult587 = callmethod(*var_baseUrl, "!=", 1, partcv, params);
// compilenode returning opresult587
  Object if583 = done;
  if (istrue(opresult587)) {
// Begin line 227
  setline(227);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 218
  setline(218);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe588 = gc_frame_new();
// Begin line 227
  setline(227);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 226
  setline(226);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_baseUrl
  int op_slot_left_589 = gc_frame_newslot(*var_baseUrl);
  int callframe590 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call591 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe590);
// compilenode returning call591
// compilenode returning call591
  int op_slot_right_589 = gc_frame_newslot(call591);
  params[0] = call591;
  partcv[0] = 1;
  Object opresult593 = callmethod(*var_baseUrl, "++", 1, partcv, params);
// compilenode returning opresult593
  gc_frame_newslot(opresult593);
// compilenode returning *var_file
  params[0] = opresult593;
  partcv[0] = 1;
  Object call594 = callmethod(*var_file, "address:=",
    1, partcv, params);
  gc_frame_end(callframe588);
// compilenode returning call594
// compilenode returning done
// Begin line 229
  setline(229);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe595 = gc_frame_new();
// Begin line 227
  setline(227);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_file
  gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object call596 = callmethodflags(self, "performCurlFetch", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe595);
// compilenode returning call596
  return call596;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if583 = undefined;
  } else {
  }
// compilenode returning if583
    gc_frame_newslot(if583);
    if560 = if583;
  }
// compilenode returning if560
    gc_frame_newslot(if560);
    if521 = if560;
  }
// compilenode returning if521
// Begin line 231
  setline(231);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 230
  setline(230);
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
  Object closure = getdatum((Object)uo, 12, (flags>>24)&0xff);
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
// Begin line 239
  setline(239);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 234
  setline(234);
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
// Begin line 236
  setline(236);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe602 = gc_frame_new();
// Begin line 235
  setline(235);
  setmodule(modulename);
  setsource(originalSourceLines);
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
// Begin line 237
  setline(237);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 236
  setline(236);
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
// Begin line 237
  setline(237);
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
// Begin line 242
  setline(242);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 239
  setline(239);
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
// Begin line 241
  setline(241);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 240
  setline(240);
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
// Begin line 243
  setline(243);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 242
  setline(242);
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
// Begin line 246
  setline(246);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 243
  setline(243);
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
// Begin line 245
  setline(245);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 244
  setline(244);
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
// Begin line 249
  setline(249);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 246
  setline(246);
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
// Begin line 248
  setline(248);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 247
  setline(247);
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
// Begin line 252
  setline(252);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 249
  setline(249);
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
// Begin line 251
  setline(251);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 250
  setline(250);
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
// Begin line 256
  setline(256);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 252
  setline(252);
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
// Begin line 254
  setline(254);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 253
  setline(253);
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
// Begin line 255
  setline(255);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 254
  setline(254);
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
// Begin line 260
  setline(260);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 256
  setline(256);
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
// Begin line 258
  setline(258);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 257
  setline(257);
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
// Begin line 261
  setline(261);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 260
  setline(260);
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
// Begin line 270
  setline(270);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 264
  setline(264);
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
// Begin line 269
  setline(269);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 265
  setline(265);
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
// Begin line 267
  setline(267);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 266
  setline(266);
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
// Begin line 268
  setline(268);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 267
  setline(267);
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
// Begin line 271
  setline(271);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 270
  setline(270);
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
  Object closure = getdatum((Object)uo, 14, (flags>>24)&0xff);
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
// Begin line 275
  setline(275);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 274
  setline(274);
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
// Begin line 281
  setline(281);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 275
  setline(275);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_global
  Object if820 = done;
  if (istrue(*var_global)) {
// Begin line 277
  setline(277);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 276
  setline(276);
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
// Begin line 280
  setline(280);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 279
  setline(279);
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
// Begin line 282
  setline(282);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe832 = gc_frame_new();
// Begin line 281
  setline(281);
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
// Begin line 283
  setline(283);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe839 = gc_frame_new();
// Begin line 282
  setline(282);
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
// Begin line 284
  setline(284);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe849 = gc_frame_new();
// Begin line 283
  setline(283);
  setmodule(modulename);
  setsource(originalSourceLines);
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
// Begin line 284
  setline(284);
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
// Begin line 301
  setline(301);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 292
  setline(292);
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
// Begin line 296
  setline(296);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 293
  setline(293);
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
// Begin line 294
  setline(294);
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
// Begin line 297
  setline(297);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 296
  setline(296);
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
// Begin line 300
  setline(300);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 299
  setline(299);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  int op_slot_left_886 = gc_frame_newslot(*var_buildPath);
// Begin line 300
  setline(300);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 299
  setline(299);
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
  Object closure = getdatum((Object)uo, 15, (flags>>24)&0xff);
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
// Begin line 290
  setline(290);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 289
  setline(289);
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
// Begin line 291
  setline(291);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 290
  setline(290);
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
// Begin line 302
  setline(302);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe861 = gc_frame_new();
// Begin line 291
  setline(291);
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
// Begin line 302
  setline(302);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block868 = alloc_Block(NULL, NULL, "PackageManager", 302);
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
  meth_meth_PackageManager__apply869->definitionLine = 291;
// compilenode returning block868
  gc_frame_newslot(block868);
  params[0] = opresult867;
  params[1] = block868;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call892 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe861);
// compilenode returning call892
// Begin line 303
  setline(303);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 302
  setline(302);
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
  Object closure = getdatum((Object)uo, 16, (flags>>24)&0xff);
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
// Begin line 307
  setline(307);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 306
  setline(306);
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
// Begin line 313
  setline(313);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 307
  setline(307);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_global
  Object if895 = done;
  if (istrue(*var_global)) {
// Begin line 309
  setline(309);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 308
  setline(308);
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
// Begin line 312
  setline(312);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 311
  setline(311);
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
// Begin line 314
  setline(314);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 313
  setline(313);
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
// Begin line 315
  setline(315);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 314
  setline(314);
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
// Begin line 316
  setline(316);
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
// Begin line 317
  setline(317);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe918 = gc_frame_new();
// Begin line 316
  setline(316);
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
// Begin line 319
  setline(319);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 317
  setline(317);
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
// Begin line 329
  setline(329);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 328
  setline(328);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_nextPath
  int op_slot_left_958 = gc_frame_newslot(*var_nextPath);
// Begin line 329
  setline(329);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 328
  setline(328);
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
// Begin line 333
  setline(333);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 329
  setline(329);
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
// Begin line 331
  setline(331);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 330
  setline(330);
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
// Begin line 332
  setline(332);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 331
  setline(331);
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
// Begin line 334
  setline(334);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 333
  setline(333);
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
  Object closure = getdatum((Object)uo, 17, (flags>>24)&0xff);
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
// Begin line 323
  setline(323);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 322
  setline(322);
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
// Begin line 324
  setline(324);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 323
  setline(323);
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
// Begin line 325
  setline(325);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 324
  setline(324);
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
// Begin line 326
  setline(326);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 325
  setline(325);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num937 = alloc_Float64(1);
// compilenode returning num937
  *var_count = num937;
  if (num937 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 327
  setline(327);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 326
  setline(326);
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
// Begin line 335
  setline(335);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe947 = gc_frame_new();
// Begin line 327
  setline(327);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block948 = alloc_Block(NULL, NULL, "PackageManager", 327);
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
  meth_meth_PackageManager__apply949->definitionLine = 316;
// compilenode returning block948
  gc_frame_newslot(block948);
// Begin line 335
  setline(335);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block956 = alloc_Block(NULL, NULL, "PackageManager", 335);
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
  meth_meth_PackageManager__apply957->definitionLine = 327;
// compilenode returning block956
  gc_frame_newslot(block956);
  params[0] = block948;
  params[1] = block956;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call981 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe947);
// compilenode returning call981
// Begin line 336
  setline(336);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 335
  setline(335);
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
// Begin line 340
  setline(340);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 336
  setline(336);
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
// Begin line 338
  setline(338);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe992 = gc_frame_new();
// Begin line 337
  setline(337);
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
// Begin line 339
  setline(339);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 338
  setline(338);
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
// Begin line 341
  setline(341);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1007 = gc_frame_new();
// Begin line 340
  setline(340);
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
// Begin line 342
  setline(342);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1009 = gc_frame_new();
// Begin line 341
  setline(341);
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
// Begin line 343
  setline(343);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 342
  setline(342);
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
// Begin line 353
  setline(353);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 352
  setline(352);
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
// Begin line 351
  setline(351);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 350
  setline(350);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 354
  setline(354);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1027 = gc_frame_new();
// Begin line 351
  setline(351);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1028 = alloc_Block(NULL, NULL, "PackageManager", 351);
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
  meth_meth_PackageManager__apply1029->definitionLine = 349;
// compilenode returning block1028
  gc_frame_newslot(block1028);
// Begin line 354
  setline(354);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1044 = alloc_Block(NULL, NULL, "PackageManager", 354);
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
  meth_meth_PackageManager__apply1045->definitionLine = 351;
// compilenode returning block1044
  gc_frame_newslot(block1044);
  params[0] = block1028;
  params[1] = block1044;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1052 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1027);
// compilenode returning call1052
// Begin line 355
  setline(355);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1053 = gc_frame_new();
// Begin line 354
  setline(354);
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
// Begin line 358
  setline(358);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 355
  setline(355);
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
// Begin line 357
  setline(357);
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
// Begin line 359
  setline(359);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 358
  setline(358);
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
  Object closure = getdatum((Object)uo, 18, (flags>>24)&0xff);
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
// Begin line 347
  setline(347);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 346
  setline(346);
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
// Begin line 348
  setline(348);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 347
  setline(347);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1014 = alloc_Float64(1);
// compilenode returning num1014
  *var_curPos = num1014;
  if (num1014 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 349
  setline(349);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 348
  setline(348);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 360
  setline(360);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1015 = gc_frame_new();
// Begin line 349
  setline(349);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1016 = alloc_Block(NULL, NULL, "PackageManager", 349);
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
  meth_meth_PackageManager__apply1017->definitionLine = 346;
// compilenode returning block1016
  gc_frame_newslot(block1016);
// Begin line 360
  setline(360);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1024 = alloc_Block(NULL, NULL, "PackageManager", 360);
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
  meth_meth_PackageManager__apply1025->definitionLine = 349;
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
// Begin line 378
  setline(378);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 363
  setline(363);
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
// Begin line 377
  setline(377);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 364
  setline(364);
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
// Begin line 366
  setline(366);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 365
  setline(365);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1086 = alloc_Boolean(1);
// compilenode returning bool1086
  return bool1086;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1079 = undefined;
  } else {
// Begin line 370
  setline(370);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 367
  setline(367);
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
// Begin line 369
  setline(369);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 368
  setline(368);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1105 = alloc_Boolean(1);
// compilenode returning bool1105
  return bool1105;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1087 = undefined;
  } else {
// Begin line 374
  setline(374);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 370
  setline(370);
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
// Begin line 372
  setline(372);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1124 = gc_frame_new();
// Begin line 371
  setline(371);
  setmodule(modulename);
  setsource(originalSourceLines);
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
// Begin line 373
  setline(373);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 372
  setline(372);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1131 = alloc_Boolean(1);
// compilenode returning bool1131
  return bool1131;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1106 = undefined;
  } else {
// Begin line 377
  setline(377);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 374
  setline(374);
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
// Begin line 376
  setline(376);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 375
  setline(375);
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
// Begin line 379
  setline(379);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 378
  setline(378);
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
// Begin line 390
  setline(390);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 389
  setline(389);
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
  Object closure = getdatum((Object)uo, 20, (flags>>24)&0xff);
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
// Begin line 383
  setline(383);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 382
  setline(382);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1153 = alloc_Float64(1);
// compilenode returning num1153
  *var_curPos = num1153;
  if (num1153 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 384
  setline(384);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 383
  setline(383);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 385
  setline(385);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 384
  setline(384);
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
// Begin line 386
  setline(386);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1155 = gc_frame_new();
// Begin line 385
  setline(385);
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
// Begin line 387
  setline(387);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 386
  setline(386);
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
// Begin line 388
  setline(388);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 387
  setline(387);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 391
  setline(391);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1164 = gc_frame_new();
// Begin line 388
  setline(388);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1165 = alloc_Block(NULL, NULL, "PackageManager", 388);
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
  meth_meth_PackageManager__apply1166->definitionLine = 374;
// compilenode returning block1165
  gc_frame_newslot(block1165);
// Begin line 391
  setline(391);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1199 = alloc_Block(NULL, NULL, "PackageManager", 391);
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
  meth_meth_PackageManager__apply1200->definitionLine = 388;
// compilenode returning block1199
  gc_frame_newslot(block1199);
  params[0] = block1165;
  params[1] = block1199;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1207 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1164);
// compilenode returning call1207
// Begin line 392
  setline(392);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1208 = gc_frame_new();
// Begin line 391
  setline(391);
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
// Begin line 396
  setline(396);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 392
  setline(392);
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
// Begin line 394
  setline(394);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 393
  setline(393);
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
// Begin line 395
  setline(395);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1221 = gc_frame_new();
// Begin line 394
  setline(394);
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
// Begin line 402
  setline(402);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 401
  setline(401);
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
// Begin line 400
  setline(400);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 399
  setline(399);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  *var_count2 = *var_count;
  if (*var_count == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 403
  setline(403);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1224 = gc_frame_new();
// Begin line 400
  setline(400);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1225 = alloc_Block(NULL, NULL, "PackageManager", 400);
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
  meth_meth_PackageManager__apply1226->definitionLine = 388;
// compilenode returning block1225
  gc_frame_newslot(block1225);
// Begin line 403
  setline(403);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1241 = alloc_Block(NULL, NULL, "PackageManager", 403);
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
  meth_meth_PackageManager__apply1242->definitionLine = 400;
// compilenode returning block1241
  gc_frame_newslot(block1241);
  params[0] = block1225;
  params[1] = block1241;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1249 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1224);
// compilenode returning call1249
// Begin line 403
  setline(403);
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
// Begin line 408
  setline(408);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 407
  setline(407);
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
// Begin line 409
  setline(409);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 408
  setline(408);
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
// Begin line 410
  setline(410);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 409
  setline(409);
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
  Object closure = getdatum((Object)uo, 23, (flags>>24)&0xff);
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
// Begin line 416
  setline(416);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 413
  setline(413);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_verbose
  Object if1258 = done;
  if (istrue(*var_verbose)) {
// Begin line 415
  setline(415);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 414
  setline(414);
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
Object meth_PackageManager__apply1385(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int callframe1387 = gc_frame_new();
// compilenode returning *var_imported
  partcv[0] = 0;
  Object call1388 = callmethod(*var_imported, "size",
    1, partcv, params);
  gc_frame_end(callframe1387);
// compilenode returning call1388
// compilenode returning call1388
  int op_slot_left_1386 = gc_frame_newslot(call1388);
  Object num1389 = alloc_Float64(0);
// compilenode returning num1389
  int op_slot_right_1386 = gc_frame_newslot(num1389);
  params[0] = num1389;
  partcv[0] = 1;
  Object opresult1391 = callmethod(call1388, ">", 1, partcv, params);
// compilenode returning opresult1391
  gc_frame_end(frame);
  return opresult1391;
}
Object meth_PackageManager__apply1394(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 440
  setline(440);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 439
  setline(439);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1395 = gc_frame_new();
// compilenode returning *var_imported
  partcv[0] = 0;
  Object call1396 = callmethod(*var_imported, "pop",
    1, partcv, params);
  gc_frame_end(callframe1395);
// compilenode returning call1396
// compilenode returning call1396
  *var_curImport = call1396;
  if (call1396 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 441
  setline(441);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1397 = gc_frame_new();
// Begin line 440
  setline(440);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1402 == NULL) {
    strlit1402 = alloc_String("");
    gc_root(strlit1402);
  }
// compilenode returning strlit1402
  int op_slot_left_1401 = gc_frame_newslot(strlit1402);
// compilenode returning *var_newDir
  int op_slot_right_1401 = gc_frame_newslot(*var_newDir);
  params[0] = *var_newDir;
  partcv[0] = 1;
  Object opresult1404 = callmethod(strlit1402, "++", 1, partcv, params);
// compilenode returning opresult1404
  int op_slot_left_1400 = gc_frame_newslot(opresult1404);
  if (strlit1405 == NULL) {
    strlit1405 = alloc_String("/");
    gc_root(strlit1405);
  }
// compilenode returning strlit1405
  int op_slot_right_1400 = gc_frame_newslot(strlit1405);
  params[0] = strlit1405;
  partcv[0] = 1;
  Object opresult1407 = callmethod(opresult1404, "++", 1, partcv, params);
// compilenode returning opresult1407
  int op_slot_left_1399 = gc_frame_newslot(opresult1407);
  int callframe1408 = gc_frame_new();
  int callframe1409 = gc_frame_new();
// compilenode returning *var_curImport
  partcv[0] = 0;
  Object call1410 = callmethod(*var_curImport, "address",
    1, partcv, params);
  gc_frame_end(callframe1409);
// compilenode returning call1410
// compilenode returning call1410
  gc_frame_newslot(call1410);
  params[0] = call1410;
  partcv[0] = 1;
  Object call1411 = callmethodflags(self, "removeContainingDir", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1408);
// compilenode returning call1411
  int op_slot_right_1399 = gc_frame_newslot(call1411);
  params[0] = call1411;
  partcv[0] = 1;
  Object opresult1413 = callmethod(opresult1407, "++", 1, partcv, params);
// compilenode returning opresult1413
  int op_slot_left_1398 = gc_frame_newslot(opresult1413);
  if (strlit1414 == NULL) {
    strlit1414 = alloc_String("");
    gc_root(strlit1414);
  }
// compilenode returning strlit1414
  int op_slot_right_1398 = gc_frame_newslot(strlit1414);
  params[0] = strlit1414;
  partcv[0] = 1;
  Object opresult1416 = callmethod(opresult1413, "++", 1, partcv, params);
// compilenode returning opresult1416
  gc_frame_newslot(opresult1416);
  if (strlit1417 == NULL) {
    strlit1417 = alloc_String("w");
    gc_root(strlit1417);
  }
// compilenode returning strlit1417
  gc_frame_newslot(strlit1417);
// compilenode returning *var_io
  params[0] = opresult1416;
  params[1] = strlit1417;
  partcv[0] = 2;
  Object call1418 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1397);
// compilenode returning call1418
  *var_toWrite = call1418;
  if (call1418 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 442
  setline(442);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1420 = gc_frame_new();
// Begin line 441
  setline(441);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1421 = gc_frame_new();
// compilenode returning *var_curImport
  partcv[0] = 0;
  Object call1422 = callmethod(*var_curImport, "data",
    1, partcv, params);
  gc_frame_end(callframe1421);
// compilenode returning call1422
// compilenode returning call1422
  gc_frame_newslot(call1422);
// compilenode returning *var_toWrite
  params[0] = call1422;
  partcv[0] = 1;
  Object call1423 = callmethod(*var_toWrite, "write",
    1, partcv, params);
  gc_frame_end(callframe1420);
// compilenode returning call1423
// Begin line 442
  setline(442);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1424 = gc_frame_new();
// compilenode returning *var_toWrite
  partcv[0] = 0;
  Object call1425 = callmethod(*var_toWrite, "close",
    1, partcv, params);
  gc_frame_end(callframe1424);
// compilenode returning call1425
// compilenode returning call1425
  gc_frame_end(frame);
  return call1425;
}
Object meth_PackageManager__apply1297(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 446
  setline(446);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 428
  setline(428);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1300 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1301 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1300);
// compilenode returning call1301
// compilenode returning call1301
  int op_slot_left_1299 = gc_frame_newslot(call1301);
  Object num1302 = alloc_Float64(4);
// compilenode returning num1302
  int op_slot_right_1299 = gc_frame_newslot(num1302);
  params[0] = num1302;
  partcv[0] = 1;
  Object opresult1304 = callmethod(call1301, ">", 1, partcv, params);
// compilenode returning opresult1304
  Object if1298 = done;
  if (istrue(opresult1304)) {
// Begin line 445
  setline(445);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 429
  setline(429);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1307 = gc_frame_new();
  int callframe1309 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1310 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1309);
// compilenode returning call1310
// compilenode returning call1310
  int op_slot_left_1308 = gc_frame_newslot(call1310);
  Object num1311 = alloc_Float64(5);
// compilenode returning num1311
  int op_slot_right_1308 = gc_frame_newslot(num1311);
  params[0] = num1311;
  partcv[0] = 1;
  Object diff1313 = callmethod(call1310, "-", 1, partcv, params);
// compilenode returning diff1313
  gc_frame_newslot(diff1313);
  int callframe1314 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1315 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1314);
// compilenode returning call1315
// compilenode returning call1315
  gc_frame_newslot(call1315);
// compilenode returning *var_file
  params[0] = diff1313;
  params[1] = call1315;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1316 = callmethod(*var_file, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1307);
// compilenode returning call1316
  int op_slot_left_1306 = gc_frame_newslot(call1316);
  if (strlit1317 == NULL) {
    strlit1317 = alloc_String(".grace");
    gc_root(strlit1317);
  }
// compilenode returning strlit1317
  int op_slot_right_1306 = gc_frame_newslot(strlit1317);
  params[0] = strlit1317;
  partcv[0] = 1;
  Object opresult1319 = callmethod(call1316, "==", 1, partcv, params);
// compilenode returning opresult1319
  Object if1305 = done;
  if (istrue(opresult1319)) {
// Begin line 431
  setline(431);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 430
  setline(430);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1322 == NULL) {
    strlit1322 = alloc_String("FILE = ");
    gc_root(strlit1322);
  }
// compilenode returning strlit1322
  int op_slot_left_1321 = gc_frame_newslot(strlit1322);
// compilenode returning *var_file
  int op_slot_right_1321 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1324 = callmethod(strlit1322, "++", 1, partcv, params);
// compilenode returning opresult1324
  int op_slot_left_1320 = gc_frame_newslot(opresult1324);
  if (strlit1325 == NULL) {
    strlit1325 = alloc_String("");
    gc_root(strlit1325);
  }
// compilenode returning strlit1325
  int op_slot_right_1320 = gc_frame_newslot(strlit1325);
  params[0] = strlit1325;
  partcv[0] = 1;
  Object opresult1327 = callmethod(opresult1324, "++", 1, partcv, params);
// compilenode returning opresult1327
  params[0] = opresult1327;
  Object call1328 = gracelib_print(NULL, 1,  params);
// compilenode returning call1328
// Begin line 432
  setline(432);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1329 = gc_frame_new();
// Begin line 431
  setline(431);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1334 == NULL) {
    strlit1334 = alloc_String("");
    gc_root(strlit1334);
  }
// compilenode returning strlit1334
  int op_slot_left_1333 = gc_frame_newslot(strlit1334);
// compilenode returning *var_toBundle
  int op_slot_right_1333 = gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object opresult1336 = callmethod(strlit1334, "++", 1, partcv, params);
// compilenode returning opresult1336
  int op_slot_left_1332 = gc_frame_newslot(opresult1336);
  if (strlit1337 == NULL) {
    strlit1337 = alloc_String("");
    gc_root(strlit1337);
  }
// compilenode returning strlit1337
  int op_slot_right_1332 = gc_frame_newslot(strlit1337);
  params[0] = strlit1337;
  partcv[0] = 1;
  Object opresult1339 = callmethod(opresult1336, "++", 1, partcv, params);
// compilenode returning opresult1339
  int op_slot_left_1331 = gc_frame_newslot(opresult1339);
// compilenode returning *var_file
  int op_slot_right_1331 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1341 = callmethod(opresult1339, "++", 1, partcv, params);
// compilenode returning opresult1341
  int op_slot_left_1330 = gc_frame_newslot(opresult1341);
  if (strlit1342 == NULL) {
    strlit1342 = alloc_String("");
    gc_root(strlit1342);
  }
// compilenode returning strlit1342
  int op_slot_right_1330 = gc_frame_newslot(strlit1342);
  params[0] = strlit1342;
  partcv[0] = 1;
  Object opresult1344 = callmethod(opresult1341, "++", 1, partcv, params);
// compilenode returning opresult1344
  gc_frame_newslot(opresult1344);
  if (strlit1345 == NULL) {
    strlit1345 = alloc_String("r");
    gc_root(strlit1345);
  }
// compilenode returning strlit1345
  gc_frame_newslot(strlit1345);
// compilenode returning *var_io
  params[0] = opresult1344;
  params[1] = strlit1345;
  partcv[0] = 2;
  Object call1346 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1329);
// compilenode returning call1346
  *var_open = call1346;
  if (call1346 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 433
  setline(433);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 432
  setline(432);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1347 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call1348 = callmethod(*var_open, "read",
    1, partcv, params);
  gc_frame_end(callframe1347);
// compilenode returning call1348
// compilenode returning call1348
  *var_openData = call1348;
  if (call1348 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 434
  setline(434);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1349 = gc_frame_new();
// Begin line 433
  setline(433);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1354 == NULL) {
    strlit1354 = alloc_String("");
    gc_root(strlit1354);
  }
// compilenode returning strlit1354
  int op_slot_left_1353 = gc_frame_newslot(strlit1354);
// compilenode returning *var_newDir
  int op_slot_right_1353 = gc_frame_newslot(*var_newDir);
  params[0] = *var_newDir;
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
  int op_slot_left_1351 = gc_frame_newslot(opresult1359);
// compilenode returning *var_file
  int op_slot_right_1351 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1361 = callmethod(opresult1359, "++", 1, partcv, params);
// compilenode returning opresult1361
  int op_slot_left_1350 = gc_frame_newslot(opresult1361);
  if (strlit1362 == NULL) {
    strlit1362 = alloc_String("");
    gc_root(strlit1362);
  }
// compilenode returning strlit1362
  int op_slot_right_1350 = gc_frame_newslot(strlit1362);
  params[0] = strlit1362;
  partcv[0] = 1;
  Object opresult1364 = callmethod(opresult1361, "++", 1, partcv, params);
// compilenode returning opresult1364
  gc_frame_newslot(opresult1364);
  if (strlit1365 == NULL) {
    strlit1365 = alloc_String("w");
    gc_root(strlit1365);
  }
// compilenode returning strlit1365
  gc_frame_newslot(strlit1365);
// compilenode returning *var_io
  params[0] = opresult1364;
  params[1] = strlit1365;
  partcv[0] = 2;
  Object call1366 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1349);
// compilenode returning call1366
  *var_toWrite = call1366;
  if (call1366 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 435
  setline(435);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1367 = gc_frame_new();
// Begin line 434
  setline(434);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_openData
  gc_frame_newslot(*var_openData);
// compilenode returning *var_toWrite
  params[0] = *var_openData;
  partcv[0] = 1;
  Object call1368 = callmethod(*var_toWrite, "write",
    1, partcv, params);
  gc_frame_end(callframe1367);
// compilenode returning call1368
// Begin line 435
  setline(435);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1369 = gc_frame_new();
// compilenode returning *var_toWrite
  partcv[0] = 0;
  Object call1370 = callmethod(*var_toWrite, "close",
    1, partcv, params);
  gc_frame_end(callframe1369);
// compilenode returning call1370
// compilenode returning call1370
// Begin line 437
  setline(437);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1371 = gc_frame_new();
// Begin line 436
  setline(436);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_newDir
  int op_slot_left_1372 = gc_frame_newslot(*var_newDir);
// compilenode returning *var_file
  int op_slot_right_1372 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1374 = callmethod(*var_newDir, "++", 1, partcv, params);
// compilenode returning opresult1374
  gc_frame_newslot(opresult1374);
  params[0] = opresult1374;
  partcv[0] = 1;
  Object call1375 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1371);
// compilenode returning call1375
// Begin line 438
  setline(438);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 437
  setline(437);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1377 == NULL) {
    strlit1377 = alloc_String("Imported size = ");
    gc_root(strlit1377);
  }
// compilenode returning strlit1377
  int op_slot_left_1376 = gc_frame_newslot(strlit1377);
  int callframe1378 = gc_frame_new();
// compilenode returning *var_imported
  partcv[0] = 0;
  Object call1379 = callmethod(*var_imported, "size",
    1, partcv, params);
  gc_frame_end(callframe1378);
// compilenode returning call1379
// compilenode returning call1379
  int op_slot_right_1376 = gc_frame_newslot(call1379);
  params[0] = call1379;
  partcv[0] = 1;
  Object opresult1381 = callmethod(strlit1377, "++", 1, partcv, params);
// compilenode returning opresult1381
  params[0] = opresult1381;
  Object call1382 = gracelib_print(NULL, 1,  params);
// compilenode returning call1382
// Begin line 444
  setline(444);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1383 = gc_frame_new();
// Begin line 438
  setline(438);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1384 = alloc_Block(NULL, NULL, "PackageManager", 438);
  gc_frame_newslot(block1384);
  block_savedest(block1384);
  Object closure1385 = createclosure(2, "_apply");
setclosureframe(closure1385, stackframe);
  addtoclosure(closure1385, var_imported);
  addtoclosure(closure1385, selfslot);
  struct UserObject *uo1385 = (struct UserObject*)block1384;
  uo1385->data[0] = (Object)closure1385;
  Method *meth_meth_PackageManager__apply1385 = addmethod2pos(block1384, "_apply", &meth_PackageManager__apply1385, 0);
int argcv_meth_PackageManager__apply1385[] = {0};
meth_meth_PackageManager__apply1385->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1385);
  meth_meth_PackageManager__apply1385->definitionModule = modulename;
  meth_meth_PackageManager__apply1385->definitionLine = 437;
// compilenode returning block1384
  gc_frame_newslot(block1384);
// Begin line 444
  setline(444);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1393 = alloc_Block(NULL, NULL, "PackageManager", 444);
  gc_frame_newslot(block1393);
  block_savedest(block1393);
  Object closure1394 = createclosure(5, "_apply");
setclosureframe(closure1394, stackframe);
  addtoclosure(closure1394, var_imported);
  addtoclosure(closure1394, var_newDir);
  addtoclosure(closure1394, var_io);
  addtoclosure(closure1394, var_toWrite);
  addtoclosure(closure1394, selfslot);
  struct UserObject *uo1394 = (struct UserObject*)block1393;
  uo1394->data[0] = (Object)closure1394;
  Method *meth_meth_PackageManager__apply1394 = addmethod2pos(block1393, "_apply", &meth_PackageManager__apply1394, 0);
int argcv_meth_PackageManager__apply1394[] = {0};
meth_meth_PackageManager__apply1394->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1394);
  meth_meth_PackageManager__apply1394->definitionModule = modulename;
  meth_meth_PackageManager__apply1394->definitionLine = 438;
// compilenode returning block1393
  gc_frame_newslot(block1393);
  params[0] = block1384;
  params[1] = block1393;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1427 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1383);
// compilenode returning call1427
    gc_frame_newslot(call1427);
    if1305 = call1427;
  } else {
  }
// compilenode returning if1305
    gc_frame_newslot(if1305);
    if1298 = if1305;
  } else {
  }
// compilenode returning if1298
  gc_frame_end(frame);
  return if1298;
}
Object meth_PackageManager_bundle1260(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 24, (flags>>24)&0xff);
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
// Begin line 424
  setline(424);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 422
  setline(422);
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
// Begin line 425
  setline(425);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 424
  setline(424);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1265 == NULL) {
    strlit1265 = alloc_String("In bundle. toBundle = ");
    gc_root(strlit1265);
  }
// compilenode returning strlit1265
  int op_slot_left_1264 = gc_frame_newslot(strlit1265);
// compilenode returning *var_toBundle
  int op_slot_right_1264 = gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object opresult1267 = callmethod(strlit1265, "++", 1, partcv, params);
// compilenode returning opresult1267
  int op_slot_left_1263 = gc_frame_newslot(opresult1267);
  if (strlit1268 == NULL) {
    strlit1268 = alloc_String("");
    gc_root(strlit1268);
  }
// compilenode returning strlit1268
  int op_slot_right_1263 = gc_frame_newslot(strlit1268);
  params[0] = strlit1268;
  partcv[0] = 1;
  Object opresult1270 = callmethod(opresult1267, "++", 1, partcv, params);
// compilenode returning opresult1270
  params[0] = opresult1270;
  Object call1271 = gracelib_print(NULL, 1,  params);
// compilenode returning call1271
// Begin line 426
  setline(426);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 425
  setline(425);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1272 = gc_frame_new();
  if (strlit1277 == NULL) {
    strlit1277 = alloc_String("");
    gc_root(strlit1277);
  }
// compilenode returning strlit1277
  int op_slot_left_1276 = gc_frame_newslot(strlit1277);
  int callframe1278 = gc_frame_new();
// compilenode returning *var_toBundle
  gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object call1279 = callmethodflags(self, "getContainingDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1278);
// compilenode returning call1279
  int op_slot_right_1276 = gc_frame_newslot(call1279);
  params[0] = call1279;
  partcv[0] = 1;
  Object opresult1281 = callmethod(strlit1277, "++", 1, partcv, params);
// compilenode returning opresult1281
  int op_slot_left_1275 = gc_frame_newslot(opresult1281);
  if (strlit1282 == NULL) {
    strlit1282 = alloc_String("../");
    gc_root(strlit1282);
  }
// compilenode returning strlit1282
  int op_slot_right_1275 = gc_frame_newslot(strlit1282);
  params[0] = strlit1282;
  partcv[0] = 1;
  Object opresult1284 = callmethod(opresult1281, "++", 1, partcv, params);
// compilenode returning opresult1284
  int op_slot_left_1274 = gc_frame_newslot(opresult1284);
// compilenode returning *var_name
  int op_slot_right_1274 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult1286 = callmethod(opresult1284, "++", 1, partcv, params);
// compilenode returning opresult1286
  int op_slot_left_1273 = gc_frame_newslot(opresult1286);
  if (strlit1287 == NULL) {
    strlit1287 = alloc_String("/");
    gc_root(strlit1287);
  }
// compilenode returning strlit1287
  int op_slot_right_1273 = gc_frame_newslot(strlit1287);
  params[0] = strlit1287;
  partcv[0] = 1;
  Object opresult1289 = callmethod(opresult1286, "++", 1, partcv, params);
// compilenode returning opresult1289
  gc_frame_newslot(opresult1289);
  params[0] = opresult1289;
  partcv[0] = 1;
  Object call1290 = callmethodflags(self, "createDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1272);
// compilenode returning call1290
  Object num1291 = alloc_Float64(1);
// compilenode returning num1291
  params[0] = num1291;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1292 = callmethod(call1290, "[]", 1, partcv, params);
// compilenode returning idxres1292
  *var_newDir = idxres1292;
  if (idxres1292 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 427
  setline(427);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1293 = gc_frame_new();
// Begin line 426
  setline(426);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toBundle
  gc_frame_newslot(*var_toBundle);
// compilenode returning *var_io
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object call1294 = callmethod(*var_io, "listdir",
    1, partcv, params);
  gc_frame_end(callframe1293);
// compilenode returning call1294
  *var_folder = call1294;
  if (call1294 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 447
  setline(447);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1295 = gc_frame_new();
// Begin line 427
  setline(427);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_folder
  gc_frame_newslot(*var_folder);
// Begin line 447
  setline(447);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1296 = alloc_Block(NULL, NULL, "PackageManager", 447);
  gc_frame_newslot(block1296);
  block_savedest(block1296);
  Object closure1297 = createclosure(5, "_apply");
setclosureframe(closure1297, stackframe);
  addtoclosure(closure1297, var_toBundle);
  addtoclosure(closure1297, var_io);
  addtoclosure(closure1297, var_newDir);
  addtoclosure(closure1297, var_imported);
  addtoclosure(closure1297, selfslot);
  struct UserObject *uo1297 = (struct UserObject*)block1296;
  uo1297->data[0] = (Object)closure1297;
  Method *meth_meth_PackageManager__apply1297 = addmethod2pos(block1296, "_apply", &meth_PackageManager__apply1297, 0);
int argcv_meth_PackageManager__apply1297[] = {1};
meth_meth_PackageManager__apply1297->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1297);
  meth_meth_PackageManager__apply1297->definitionModule = modulename;
  meth_meth_PackageManager__apply1297->definitionLine = 400;
// compilenode returning block1296
  gc_frame_newslot(block1296);
  params[0] = *var_folder;
  params[1] = block1296;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1429 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1295);
// compilenode returning call1429
  gc_frame_end(frame);
  return call1429;
}
Object meth_PackageManager__apply1501(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 464
  setline(464);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1502 = gc_frame_new();
// Begin line 463
  setline(463);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_d
  gc_frame_newslot(*var_d);
// compilenode returning *var_out
  params[0] = *var_d;
  partcv[0] = 1;
  Object call1503 = callmethod(*var_out, "write",
    1, partcv, params);
  gc_frame_end(callframe1502);
// compilenode returning call1503
  gc_frame_end(frame);
  return call1503;
}
Object meth_PackageManager__apply1437(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 467
  setline(467);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 453
  setline(453);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1440 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1441 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1440);
// compilenode returning call1441
// compilenode returning call1441
  int op_slot_left_1439 = gc_frame_newslot(call1441);
  Object num1442 = alloc_Float64(4);
// compilenode returning num1442
  int op_slot_right_1439 = gc_frame_newslot(num1442);
  params[0] = num1442;
  partcv[0] = 1;
  Object opresult1444 = callmethod(call1441, ">", 1, partcv, params);
// compilenode returning opresult1444
  Object if1438 = done;
  if (istrue(opresult1444)) {
// Begin line 466
  setline(466);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 454
  setline(454);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1447 = gc_frame_new();
  int callframe1449 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1450 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1449);
// compilenode returning call1450
// compilenode returning call1450
  int op_slot_left_1448 = gc_frame_newslot(call1450);
  Object num1451 = alloc_Float64(5);
// compilenode returning num1451
  int op_slot_right_1448 = gc_frame_newslot(num1451);
  params[0] = num1451;
  partcv[0] = 1;
  Object diff1453 = callmethod(call1450, "-", 1, partcv, params);
// compilenode returning diff1453
  gc_frame_newslot(diff1453);
  int callframe1454 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1455 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1454);
// compilenode returning call1455
// compilenode returning call1455
  gc_frame_newslot(call1455);
// compilenode returning *var_file
  params[0] = diff1453;
  params[1] = call1455;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1456 = callmethod(*var_file, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1447);
// compilenode returning call1456
  int op_slot_left_1446 = gc_frame_newslot(call1456);
  if (strlit1457 == NULL) {
    strlit1457 = alloc_String(".grace");
    gc_root(strlit1457);
  }
// compilenode returning strlit1457
  int op_slot_right_1446 = gc_frame_newslot(strlit1457);
  params[0] = strlit1457;
  partcv[0] = 1;
  Object opresult1459 = callmethod(call1456, "==", 1, partcv, params);
// compilenode returning opresult1459
  Object if1445 = done;
  if (istrue(opresult1459)) {
// Begin line 456
  setline(456);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1460 = gc_frame_new();
// Begin line 455
  setline(455);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_address
  int op_slot_left_1462 = gc_frame_newslot(*var_address);
  if (strlit1463 == NULL) {
    strlit1463 = alloc_String("/");
    gc_root(strlit1463);
  }
// compilenode returning strlit1463
  int op_slot_right_1462 = gc_frame_newslot(strlit1463);
  params[0] = strlit1463;
  partcv[0] = 1;
  Object opresult1465 = callmethod(*var_address, "++", 1, partcv, params);
// compilenode returning opresult1465
  int op_slot_left_1461 = gc_frame_newslot(opresult1465);
// compilenode returning *var_file
  int op_slot_right_1461 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1467 = callmethod(opresult1465, "++", 1, partcv, params);
// compilenode returning opresult1467
  gc_frame_newslot(opresult1467);
  if (strlit1468 == NULL) {
    strlit1468 = alloc_String("r");
    gc_root(strlit1468);
  }
// compilenode returning strlit1468
  gc_frame_newslot(strlit1468);
// compilenode returning *var_io
  params[0] = opresult1467;
  params[1] = strlit1468;
  partcv[0] = 2;
  Object call1469 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1460);
// compilenode returning call1469
  *var_open = call1469;
  if (call1469 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 457
  setline(457);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 456
  setline(456);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1470 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call1471 = callmethod(*var_open, "read",
    1, partcv, params);
  gc_frame_end(callframe1470);
// compilenode returning call1471
// compilenode returning call1471
  *var_readFile = call1471;
  if (call1471 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 458
  setline(458);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1472 = gc_frame_new();
// Begin line 457
  setline(457);
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
  Object call1473 = callmethodflags(self, "parseAndPrefix", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1472);
// compilenode returning call1473
  *var_toWrite = call1473;
  if (call1473 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 458
  setline(458);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1474 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call1475 = callmethod(*var_open, "close",
    1, partcv, params);
  gc_frame_end(callframe1474);
// compilenode returning call1475
// compilenode returning call1475
// Begin line 461
  setline(461);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1476 = gc_frame_new();
// Begin line 460
  setline(460);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1480 == NULL) {
    strlit1480 = alloc_String("rm ");
    gc_root(strlit1480);
  }
// compilenode returning strlit1480
  int op_slot_left_1479 = gc_frame_newslot(strlit1480);
// compilenode returning *var_address
  int op_slot_right_1479 = gc_frame_newslot(*var_address);
  params[0] = *var_address;
  partcv[0] = 1;
  Object opresult1482 = callmethod(strlit1480, "++", 1, partcv, params);
// compilenode returning opresult1482
  int op_slot_left_1478 = gc_frame_newslot(opresult1482);
  if (strlit1483 == NULL) {
    strlit1483 = alloc_String("/");
    gc_root(strlit1483);
  }
// compilenode returning strlit1483
  int op_slot_right_1478 = gc_frame_newslot(strlit1483);
  params[0] = strlit1483;
  partcv[0] = 1;
  Object opresult1485 = callmethod(opresult1482, "++", 1, partcv, params);
// compilenode returning opresult1485
  int op_slot_left_1477 = gc_frame_newslot(opresult1485);
// compilenode returning *var_file
  int op_slot_right_1477 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1487 = callmethod(opresult1485, "++", 1, partcv, params);
// compilenode returning opresult1487
  gc_frame_newslot(opresult1487);
// compilenode returning *var_io
  params[0] = opresult1487;
  partcv[0] = 1;
  Object call1488 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe1476);
// compilenode returning call1488
// Begin line 462
  setline(462);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1489 = gc_frame_new();
// Begin line 461
  setline(461);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_address
  int op_slot_left_1491 = gc_frame_newslot(*var_address);
  if (strlit1492 == NULL) {
    strlit1492 = alloc_String("/");
    gc_root(strlit1492);
  }
// compilenode returning strlit1492
  int op_slot_right_1491 = gc_frame_newslot(strlit1492);
  params[0] = strlit1492;
  partcv[0] = 1;
  Object opresult1494 = callmethod(*var_address, "++", 1, partcv, params);
// compilenode returning opresult1494
  int op_slot_left_1490 = gc_frame_newslot(opresult1494);
// compilenode returning *var_file
  int op_slot_right_1490 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1496 = callmethod(opresult1494, "++", 1, partcv, params);
// compilenode returning opresult1496
  gc_frame_newslot(opresult1496);
  if (strlit1497 == NULL) {
    strlit1497 = alloc_String("w");
    gc_root(strlit1497);
  }
// compilenode returning strlit1497
  gc_frame_newslot(strlit1497);
// compilenode returning *var_io
  params[0] = opresult1496;
  params[1] = strlit1497;
  partcv[0] = 2;
  Object call1498 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1489);
// compilenode returning call1498
  *var_out = call1498;
  if (call1498 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 465
  setline(465);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1499 = gc_frame_new();
// Begin line 462
  setline(462);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toWrite
  gc_frame_newslot(*var_toWrite);
// Begin line 465
  setline(465);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1500 = alloc_Block(NULL, NULL, "PackageManager", 465);
  gc_frame_newslot(block1500);
  block_savedest(block1500);
  Object closure1501 = createclosure(2, "_apply");
setclosureframe(closure1501, stackframe);
  addtoclosure(closure1501, var_out);
  addtoclosure(closure1501, selfslot);
  struct UserObject *uo1501 = (struct UserObject*)block1500;
  uo1501->data[0] = (Object)closure1501;
  Method *meth_meth_PackageManager__apply1501 = addmethod2pos(block1500, "_apply", &meth_PackageManager__apply1501, 0);
int argcv_meth_PackageManager__apply1501[] = {1};
meth_meth_PackageManager__apply1501->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1501);
  meth_meth_PackageManager__apply1501->definitionModule = modulename;
  meth_meth_PackageManager__apply1501->definitionLine = 458;
// compilenode returning block1500
  gc_frame_newslot(block1500);
  params[0] = *var_toWrite;
  params[1] = block1500;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1505 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1499);
// compilenode returning call1505
    gc_frame_newslot(call1505);
    if1445 = call1505;
  } else {
  }
// compilenode returning if1445
    gc_frame_newslot(if1445);
    if1438 = if1445;
  } else {
  }
// compilenode returning if1438
  gc_frame_end(frame);
  return if1438;
}
Object meth_PackageManager_setImportDest1430(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 25, (flags>>24)&0xff);
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
// Begin line 451
  setline(451);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1431 = gc_frame_new();
// Begin line 450
  setline(450);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_address
  gc_frame_newslot(*var_address);
// compilenode returning *var_io
  params[0] = *var_address;
  partcv[0] = 1;
  Object call1432 = callmethod(*var_io, "getdir",
    1, partcv, params);
  gc_frame_end(callframe1431);
// compilenode returning call1432
  *var_folder = call1432;
  if (call1432 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 451
  setline(451);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1433 == NULL) {
    strlit1433 = alloc_String("Setting import dest");
    gc_root(strlit1433);
  }
// compilenode returning strlit1433
  params[0] = strlit1433;
  Object call1434 = gracelib_print(NULL, 1,  params);
// compilenode returning call1434
// Begin line 471
  setline(471);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1435 = gc_frame_new();
// Begin line 452
  setline(452);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_folder
  gc_frame_newslot(*var_folder);
// Begin line 471
  setline(471);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1436 = alloc_Block(NULL, NULL, "PackageManager", 471);
  gc_frame_newslot(block1436);
  block_savedest(block1436);
  Object closure1437 = createclosure(4, "_apply");
setclosureframe(closure1437, stackframe);
  addtoclosure(closure1437, var_address);
  addtoclosure(closure1437, var_io);
  addtoclosure(closure1437, var_prefix);
  addtoclosure(closure1437, selfslot);
  struct UserObject *uo1437 = (struct UserObject*)block1436;
  uo1437->data[0] = (Object)closure1437;
  Method *meth_meth_PackageManager__apply1437 = addmethod2pos(block1436, "_apply", &meth_PackageManager__apply1437, 0);
int argcv_meth_PackageManager__apply1437[] = {1};
meth_meth_PackageManager__apply1437->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1437);
  meth_meth_PackageManager__apply1437->definitionModule = modulename;
  meth_meth_PackageManager__apply1437->definitionLine = 442;
// compilenode returning block1436
  gc_frame_newslot(block1436);
  params[0] = *var_folder;
  params[1] = block1436;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1507 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1435);
// compilenode returning call1507
  gc_frame_end(frame);
  return call1507;
}
Object meth_PackageManager__apply1522(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1523 = gc_frame_newslot(*var_curPos);
  int callframe1524 = gc_frame_new();
// compilenode returning *var_readFile
  partcv[0] = 0;
  Object call1525 = callmethod(*var_readFile, "size",
    1, partcv, params);
  gc_frame_end(callframe1524);
// compilenode returning call1525
// compilenode returning call1525
  int op_slot_right_1523 = gc_frame_newslot(call1525);
  params[0] = call1525;
  partcv[0] = 1;
  Object opresult1527 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1527
  gc_frame_end(frame);
  return opresult1527;
}
Object meth_PackageManager__apply1534(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1536 = gc_frame_newslot(*var_curPos);
  int callframe1537 = gc_frame_new();
// compilenode returning *var_readFile
  partcv[0] = 0;
  Object call1538 = callmethod(*var_readFile, "size",
    1, partcv, params);
  gc_frame_end(callframe1537);
// compilenode returning call1538
// compilenode returning call1538
  int op_slot_right_1536 = gc_frame_newslot(call1538);
  params[0] = call1538;
  partcv[0] = 1;
  Object opresult1540 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1540
  int op_slot_left_1535 = gc_frame_newslot(opresult1540);
// compilenode returning *var_readFile
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1542 = callmethod(*var_readFile, "[]", 1, partcv, params);
// compilenode returning idxres1542
  int op_slot_left_1541 = gc_frame_newslot(idxres1542);
  if (strlit1543 == NULL) {
    strlit1543 = alloc_String("""\x0a""");
    gc_root(strlit1543);
  }
// compilenode returning strlit1543
  int op_slot_right_1541 = gc_frame_newslot(strlit1543);
  params[0] = strlit1543;
  partcv[0] = 1;
  Object opresult1545 = callmethod(idxres1542, "!=", 1, partcv, params);
// compilenode returning opresult1545
  int op_slot_right_1535 = gc_frame_newslot(opresult1545);
  params[0] = opresult1545;
  partcv[0] = 1;
  Object opresult1547 = callmethod(opresult1540, "&&", 1, partcv, params);
// compilenode returning opresult1547
  gc_frame_end(frame);
  return opresult1547;
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
// Begin line 483
  setline(483);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 482
  setline(482);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1551 = gc_frame_newslot(*var_curPos);
  Object num1552 = alloc_Float64(1);
// compilenode returning num1552
  int op_slot_right_1551 = gc_frame_newslot(num1552);
  params[0] = num1552;
  partcv[0] = 1;
  Object sum1554 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1554
  *var_curPos = sum1554;
  if (sum1554 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1575(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 489
  setline(489);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 488
  setline(488);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  int op_slot_left_1576 = gc_frame_newslot(*var_line);
// Begin line 489
  setline(489);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 488
  setline(488);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_readFile
// compilenode returning *var_b
  params[0] = *var_b;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1577 = callmethod(*var_readFile, "[]", 1, partcv, params);
// compilenode returning idxres1577
  int op_slot_right_1576 = gc_frame_newslot(idxres1577);
  params[0] = idxres1577;
  partcv[0] = 1;
  Object opresult1579 = callmethod(*var_line, "++", 1, partcv, params);
// compilenode returning opresult1579
  *var_line = opresult1579;
  if (opresult1579 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1619(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1621 = gc_frame_newslot(*var_pos);
  int callframe1622 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1623 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1622);
// compilenode returning call1623
// compilenode returning call1623
  int op_slot_right_1621 = gc_frame_newslot(call1623);
  params[0] = call1623;
  partcv[0] = 1;
  Object opresult1625 = callmethod(*var_pos, "<", 1, partcv, params);
// compilenode returning opresult1625
  int op_slot_left_1620 = gc_frame_newslot(opresult1625);
  int callframe1626 = gc_frame_new();
// compilenode returning *var_line
// compilenode returning *var_pos
  params[0] = *var_pos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1629 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1629
  int op_slot_left_1628 = gc_frame_newslot(idxres1629);
  if (strlit1630 == NULL) {
    strlit1630 = alloc_String("""\x22""");
    gc_root(strlit1630);
  }
// compilenode returning strlit1630
  int op_slot_right_1628 = gc_frame_newslot(strlit1630);
  params[0] = strlit1630;
  partcv[0] = 1;
  Object opresult1632 = callmethod(idxres1629, "==", 1, partcv, params);
// compilenode returning opresult1632
  int op_slot_left_1627 = gc_frame_newslot(opresult1632);
// compilenode returning *var_line
// compilenode returning *var_pos
  params[0] = *var_pos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1634 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1634
  int op_slot_left_1633 = gc_frame_newslot(idxres1634);
  if (strlit1635 == NULL) {
    strlit1635 = alloc_String(" ");
    gc_root(strlit1635);
  }
// compilenode returning strlit1635
  int op_slot_right_1633 = gc_frame_newslot(strlit1635);
  params[0] = strlit1635;
  partcv[0] = 1;
  Object opresult1637 = callmethod(idxres1634, "==", 1, partcv, params);
// compilenode returning opresult1637
  int op_slot_right_1627 = gc_frame_newslot(opresult1637);
  params[0] = opresult1637;
  partcv[0] = 1;
  Object opresult1639 = callmethod(opresult1632, "||", 1, partcv, params);
// compilenode returning opresult1639
  partcv[0] = 0;
  Object call1640 = callmethod(opresult1639, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1626);
// compilenode returning call1640
  int op_slot_right_1620 = gc_frame_newslot(call1640);
  params[0] = call1640;
  partcv[0] = 1;
  Object opresult1642 = callmethod(opresult1625, "&&", 1, partcv, params);
// compilenode returning opresult1642
  gc_frame_end(frame);
  return opresult1642;
}
Object meth_PackageManager__apply1645(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 500
  setline(500);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 499
  setline(499);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  int op_slot_left_1646 = gc_frame_newslot(*var_pos);
  Object num1647 = alloc_Float64(1);
// compilenode returning num1647
  int op_slot_right_1646 = gc_frame_newslot(num1647);
  params[0] = num1647;
  partcv[0] = 1;
  Object sum1649 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1649
  *var_pos = sum1649;
  if (sum1649 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1683(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 507
  setline(507);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1684 = gc_frame_new();
// Begin line 506
  setline(506);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_d
  gc_frame_newslot(*var_d);
// compilenode returning *var_outFile
  params[0] = *var_d;
  partcv[0] = 1;
  Object call1685 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1684);
// compilenode returning call1685
  gc_frame_end(frame);
  return call1685;
}
Object meth_PackageManager__apply1693(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 513
  setline(513);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1694 = gc_frame_new();
// Begin line 512
  setline(512);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_d
  gc_frame_newslot(*var_d);
// compilenode returning *var_outFile
  params[0] = *var_d;
  partcv[0] = 1;
  Object call1695 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1694);
// compilenode returning call1695
  gc_frame_end(frame);
  return call1695;
}
Object meth_PackageManager__apply1530(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 481
  setline(481);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 480
  setline(480);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 484
  setline(484);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1532 = gc_frame_new();
// Begin line 481
  setline(481);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1533 = alloc_Block(NULL, NULL, "PackageManager", 481);
  gc_frame_newslot(block1533);
  block_savedest(block1533);
  Object closure1534 = createclosure(3, "_apply");
setclosureframe(closure1534, stackframe);
  addtoclosure(closure1534, var_curPos);
  addtoclosure(closure1534, var_readFile);
  addtoclosure(closure1534, selfslot);
  struct UserObject *uo1534 = (struct UserObject*)block1533;
  uo1534->data[0] = (Object)closure1534;
  Method *meth_meth_PackageManager__apply1534 = addmethod2pos(block1533, "_apply", &meth_PackageManager__apply1534, 0);
int argcv_meth_PackageManager__apply1534[] = {0};
meth_meth_PackageManager__apply1534->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1534);
  meth_meth_PackageManager__apply1534->definitionModule = modulename;
  meth_meth_PackageManager__apply1534->definitionLine = 479;
// compilenode returning block1533
  gc_frame_newslot(block1533);
// Begin line 484
  setline(484);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1549 = alloc_Block(NULL, NULL, "PackageManager", 484);
  gc_frame_newslot(block1549);
  block_savedest(block1549);
  Object closure1550 = createclosure(2, "_apply");
setclosureframe(closure1550, stackframe);
  addtoclosure(closure1550, var_curPos);
  addtoclosure(closure1550, selfslot);
  struct UserObject *uo1550 = (struct UserObject*)block1549;
  uo1550->data[0] = (Object)closure1550;
  Method *meth_meth_PackageManager__apply1550 = addmethod2pos(block1549, "_apply", &meth_PackageManager__apply1550, 0);
int argcv_meth_PackageManager__apply1550[] = {0};
meth_meth_PackageManager__apply1550->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1550);
  meth_meth_PackageManager__apply1550->definitionModule = modulename;
  meth_meth_PackageManager__apply1550->definitionLine = 481;
// compilenode returning block1549
  gc_frame_newslot(block1549);
  params[0] = block1533;
  params[1] = block1549;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1557 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1532);
// compilenode returning call1557
// Begin line 485
  setline(485);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1558 = gc_frame_new();
// Begin line 484
  setline(484);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_startPos
  gc_frame_newslot(*var_startPos);
// compilenode returning *var_curPos
  int op_slot_left_1559 = gc_frame_newslot(*var_curPos);
  Object num1560 = alloc_Float64(1);
// compilenode returning num1560
  int op_slot_right_1559 = gc_frame_newslot(num1560);
  params[0] = num1560;
  partcv[0] = 1;
  Object diff1562 = callmethod(*var_curPos, "-", 1, partcv, params);
// compilenode returning diff1562
  gc_frame_newslot(diff1562);
// compilenode returning *var_readFile
  params[0] = *var_startPos;
  params[1] = diff1562;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1563 = callmethod(*var_readFile, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1558);
// compilenode returning call1563
  *var_data = call1563;
  if (call1563 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 486
  setline(486);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 485
  setline(485);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1564 == NULL) {
    strlit1564 = alloc_String("");
    gc_root(strlit1564);
  }
// compilenode returning strlit1564
  *var_line = strlit1564;
  if (strlit1564 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 487
  setline(487);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 486
  setline(486);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1565 = alloc_Float64(1);
// compilenode returning num1565
  *var_count = num1565;
  if (num1565 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 490
  setline(490);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1566 = gc_frame_new();
// Begin line 487
  setline(487);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_startPos
  int op_slot_left_1567 = gc_frame_newslot(*var_startPos);
// compilenode returning *var_curPos
  int op_slot_left_1568 = gc_frame_newslot(*var_curPos);
  Object num1569 = alloc_Float64(1);
// compilenode returning num1569
  int op_slot_right_1568 = gc_frame_newslot(num1569);
  params[0] = num1569;
  partcv[0] = 1;
  Object diff1571 = callmethod(*var_curPos, "-", 1, partcv, params);
// compilenode returning diff1571
  int op_slot_right_1567 = gc_frame_newslot(diff1571);
  params[0] = diff1571;
  partcv[0] = 1;
  Object opresult1573 = callmethod(*var_startPos, "..", 1, partcv, params);
// compilenode returning opresult1573
  gc_frame_newslot(opresult1573);
// Begin line 490
  setline(490);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1574 = alloc_Block(NULL, NULL, "PackageManager", 490);
  gc_frame_newslot(block1574);
  block_savedest(block1574);
  Object closure1575 = createclosure(3, "_apply");
setclosureframe(closure1575, stackframe);
  addtoclosure(closure1575, var_line);
  addtoclosure(closure1575, var_readFile);
  addtoclosure(closure1575, selfslot);
  struct UserObject *uo1575 = (struct UserObject*)block1574;
  uo1575->data[0] = (Object)closure1575;
  Method *meth_meth_PackageManager__apply1575 = addmethod2pos(block1574, "_apply", &meth_PackageManager__apply1575, 0);
int argcv_meth_PackageManager__apply1575[] = {1};
meth_meth_PackageManager__apply1575->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1575);
  meth_meth_PackageManager__apply1575->definitionModule = modulename;
  meth_meth_PackageManager__apply1575->definitionLine = 481;
// compilenode returning block1574
  gc_frame_newslot(block1574);
  params[0] = opresult1573;
  params[1] = block1574;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1582 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1566);
// compilenode returning call1582
// Begin line 491
  setline(491);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 490
  setline(490);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  params[0] = *var_line;
  Object call1583 = gracelib_print(NULL, 1,  params);
// compilenode returning call1583
// Begin line 516
  setline(516);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 491
  setline(491);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1587 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1588 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1587);
// compilenode returning call1588
// compilenode returning call1588
  int op_slot_left_1586 = gc_frame_newslot(call1588);
  Object num1589 = alloc_Float64(6);
// compilenode returning num1589
  int op_slot_right_1586 = gc_frame_newslot(num1589);
  params[0] = num1589;
  partcv[0] = 1;
  Object opresult1591 = callmethod(call1588, ">", 1, partcv, params);
// compilenode returning opresult1591
  int op_slot_left_1585 = gc_frame_newslot(opresult1591);
  int callframe1593 = gc_frame_new();
  Object num1594 = alloc_Float64(1);
// compilenode returning num1594
  gc_frame_newslot(num1594);
  Object num1595 = alloc_Float64(7);
// compilenode returning num1595
  gc_frame_newslot(num1595);
// compilenode returning *var_line
  params[0] = num1594;
  params[1] = num1595;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1596 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1593);
// compilenode returning call1596
  int op_slot_left_1592 = gc_frame_newslot(call1596);
  if (strlit1597 == NULL) {
    strlit1597 = alloc_String("import ");
    gc_root(strlit1597);
  }
// compilenode returning strlit1597
  int op_slot_right_1592 = gc_frame_newslot(strlit1597);
  params[0] = strlit1597;
  partcv[0] = 1;
  Object opresult1599 = callmethod(call1596, "==", 1, partcv, params);
// compilenode returning opresult1599
  int op_slot_right_1585 = gc_frame_newslot(opresult1599);
  params[0] = opresult1599;
  partcv[0] = 1;
  Object opresult1601 = callmethod(opresult1591, "&&", 1, partcv, params);
// compilenode returning opresult1601
  Object if1584 = done;
  if (istrue(opresult1601)) {
// Begin line 493
  setline(493);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 492
  setline(492);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1602 = alloc_Float64(1);
// compilenode returning num1602
  *var_pos = num1602;
  if (num1602 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 494
  setline(494);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 493
  setline(493);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  *var_start = *var_pos;
  if (*var_pos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 495
  setline(495);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 494
  setline(494);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1603 == NULL) {
    strlit1603 = alloc_String("");
    gc_root(strlit1603);
  }
// compilenode returning strlit1603
  *var_nextImport = strlit1603;
  if (strlit1603 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 496
  setline(496);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1604 = gc_frame_new();
// Begin line 495
  setline(495);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// compilenode returning *var_pos
  gc_frame_newslot(*var_pos);
  params[0] = *var_line;
  params[1] = *var_pos;
  partcv[0] = 2;
  Object call1605 = callmethodflags(self, "skipWhiteSpace", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1604);
// compilenode returning call1605
  *var_pos = call1605;
  if (call1605 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 497
  setline(497);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 496
  setline(496);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  int op_slot_left_1607 = gc_frame_newslot(*var_pos);
  Object num1608 = alloc_Float64(1);
// compilenode returning num1608
  int op_slot_right_1607 = gc_frame_newslot(num1608);
  params[0] = num1608;
  partcv[0] = 1;
  Object sum1610 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1610
  *var_pos = sum1610;
  if (sum1610 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 498
  setline(498);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 497
  setline(497);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1613 == NULL) {
    strlit1613 = alloc_String("LINE = ");
    gc_root(strlit1613);
  }
// compilenode returning strlit1613
  int op_slot_left_1612 = gc_frame_newslot(strlit1613);
// compilenode returning *var_line
  int op_slot_right_1612 = gc_frame_newslot(*var_line);
  params[0] = *var_line;
  partcv[0] = 1;
  Object opresult1615 = callmethod(strlit1613, "++", 1, partcv, params);
// compilenode returning opresult1615
  params[0] = opresult1615;
  Object call1616 = gracelib_print(NULL, 1,  params);
// compilenode returning call1616
// Begin line 501
  setline(501);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1617 = gc_frame_new();
// Begin line 498
  setline(498);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1618 = alloc_Block(NULL, NULL, "PackageManager", 498);
  gc_frame_newslot(block1618);
  block_savedest(block1618);
  Object closure1619 = createclosure(3, "_apply");
setclosureframe(closure1619, stackframe);
  addtoclosure(closure1619, var_pos);
  addtoclosure(closure1619, var_line);
  addtoclosure(closure1619, selfslot);
  struct UserObject *uo1619 = (struct UserObject*)block1618;
  uo1619->data[0] = (Object)closure1619;
  Method *meth_meth_PackageManager__apply1619 = addmethod2pos(block1618, "_apply", &meth_PackageManager__apply1619, 0);
int argcv_meth_PackageManager__apply1619[] = {0};
meth_meth_PackageManager__apply1619->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1619);
  meth_meth_PackageManager__apply1619->definitionModule = modulename;
  meth_meth_PackageManager__apply1619->definitionLine = 491;
// compilenode returning block1618
  gc_frame_newslot(block1618);
// Begin line 501
  setline(501);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1644 = alloc_Block(NULL, NULL, "PackageManager", 501);
  gc_frame_newslot(block1644);
  block_savedest(block1644);
  Object closure1645 = createclosure(2, "_apply");
setclosureframe(closure1645, stackframe);
  addtoclosure(closure1645, var_pos);
  addtoclosure(closure1645, selfslot);
  struct UserObject *uo1645 = (struct UserObject*)block1644;
  uo1645->data[0] = (Object)closure1645;
  Method *meth_meth_PackageManager__apply1645 = addmethod2pos(block1644, "_apply", &meth_PackageManager__apply1645, 0);
int argcv_meth_PackageManager__apply1645[] = {0};
meth_meth_PackageManager__apply1645->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1645);
  meth_meth_PackageManager__apply1645->definitionModule = modulename;
  meth_meth_PackageManager__apply1645->definitionLine = 498;
// compilenode returning block1644
  gc_frame_newslot(block1644);
  params[0] = block1618;
  params[1] = block1644;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1652 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1617);
// compilenode returning call1652
// Begin line 502
  setline(502);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 501
  setline(501);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  int op_slot_left_1653 = gc_frame_newslot(*var_pos);
  Object num1654 = alloc_Float64(1);
// compilenode returning num1654
  int op_slot_right_1653 = gc_frame_newslot(num1654);
  params[0] = num1654;
  partcv[0] = 1;
  Object sum1656 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1656
  *var_pos = sum1656;
  if (sum1656 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 503
  setline(503);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1658 = gc_frame_new();
// Begin line 502
  setline(502);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1659 = gc_frame_new();
// compilenode returning *var_pos
  int op_slot_left_1660 = gc_frame_newslot(*var_pos);
  Object num1661 = alloc_Float64(1);
// compilenode returning num1661
  int op_slot_right_1660 = gc_frame_newslot(num1661);
  params[0] = num1661;
  partcv[0] = 1;
  Object sum1663 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1663
  gc_frame_newslot(sum1663);
  int callframe1664 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1665 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1664);
// compilenode returning call1665
// compilenode returning call1665
  gc_frame_newslot(call1665);
// compilenode returning *var_line
  params[0] = sum1663;
  params[1] = call1665;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1666 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1659);
// compilenode returning call1666
  gc_frame_newslot(call1666);
  params[0] = call1666;
  partcv[0] = 1;
  Object call1667 = callmethodflags(self, "removeExistingUrls", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1658);
// compilenode returning call1667
  *var_remainder = call1667;
  if (call1667 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 504
  setline(504);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 503
  setline(503);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1671 == NULL) {
    strlit1671 = alloc_String("import ""\x22""");
    gc_root(strlit1671);
  }
// compilenode returning strlit1671
  int op_slot_left_1670 = gc_frame_newslot(strlit1671);
// compilenode returning *var_prefix
  int op_slot_right_1670 = gc_frame_newslot(*var_prefix);
  params[0] = *var_prefix;
  partcv[0] = 1;
  Object opresult1673 = callmethod(strlit1671, "++", 1, partcv, params);
// compilenode returning opresult1673
  int op_slot_left_1669 = gc_frame_newslot(opresult1673);
  if (strlit1674 == NULL) {
    strlit1674 = alloc_String("/");
    gc_root(strlit1674);
  }
// compilenode returning strlit1674
  int op_slot_right_1669 = gc_frame_newslot(strlit1674);
  params[0] = strlit1674;
  partcv[0] = 1;
  Object opresult1676 = callmethod(opresult1673, "++", 1, partcv, params);
// compilenode returning opresult1676
  int op_slot_left_1668 = gc_frame_newslot(opresult1676);
// compilenode returning *var_remainder
  int op_slot_right_1668 = gc_frame_newslot(*var_remainder);
  params[0] = *var_remainder;
  partcv[0] = 1;
  Object opresult1678 = callmethod(opresult1676, "++", 1, partcv, params);
// compilenode returning opresult1678
  *var_line = opresult1678;
  if (opresult1678 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
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
  Object call1680 = gracelib_print(NULL, 1,  params);
// compilenode returning call1680
// Begin line 508
  setline(508);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1681 = gc_frame_new();
// Begin line 505
  setline(505);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// Begin line 508
  setline(508);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1682 = alloc_Block(NULL, NULL, "PackageManager", 508);
  gc_frame_newslot(block1682);
  block_savedest(block1682);
  Object closure1683 = createclosure(2, "_apply");
setclosureframe(closure1683, stackframe);
  addtoclosure(closure1683, var_outFile);
  addtoclosure(closure1683, selfslot);
  struct UserObject *uo1683 = (struct UserObject*)block1682;
  uo1683->data[0] = (Object)closure1683;
  Method *meth_meth_PackageManager__apply1683 = addmethod2pos(block1682, "_apply", &meth_PackageManager__apply1683, 0);
int argcv_meth_PackageManager__apply1683[] = {1};
meth_meth_PackageManager__apply1683->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1683);
  meth_meth_PackageManager__apply1683->definitionModule = modulename;
  meth_meth_PackageManager__apply1683->definitionLine = 502;
// compilenode returning block1682
  gc_frame_newslot(block1682);
  params[0] = *var_line;
  params[1] = block1682;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1687 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1681);
// compilenode returning call1687
// Begin line 509
  setline(509);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1688 = gc_frame_new();
// Begin line 508
  setline(508);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1689 == NULL) {
    strlit1689 = alloc_String("""\x0a""");
    gc_root(strlit1689);
  }
// compilenode returning strlit1689
  gc_frame_newslot(strlit1689);
// compilenode returning *var_outFile
  params[0] = strlit1689;
  partcv[0] = 1;
  Object call1690 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1688);
// compilenode returning call1690
    gc_frame_newslot(call1690);
    if1584 = call1690;
  } else {
// Begin line 514
  setline(514);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1691 = gc_frame_new();
// Begin line 511
  setline(511);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// Begin line 514
  setline(514);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1692 = alloc_Block(NULL, NULL, "PackageManager", 514);
  gc_frame_newslot(block1692);
  block_savedest(block1692);
  Object closure1693 = createclosure(2, "_apply");
setclosureframe(closure1693, stackframe);
  addtoclosure(closure1693, var_outFile);
  addtoclosure(closure1693, selfslot);
  struct UserObject *uo1693 = (struct UserObject*)block1692;
  uo1693->data[0] = (Object)closure1693;
  Method *meth_meth_PackageManager__apply1693 = addmethod2pos(block1692, "_apply", &meth_PackageManager__apply1693, 0);
int argcv_meth_PackageManager__apply1693[] = {1};
meth_meth_PackageManager__apply1693->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1693);
  meth_meth_PackageManager__apply1693->definitionModule = modulename;
  meth_meth_PackageManager__apply1693->definitionLine = 502;
// compilenode returning block1692
  gc_frame_newslot(block1692);
  params[0] = *var_line;
  params[1] = block1692;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1697 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1691);
// compilenode returning call1697
// Begin line 515
  setline(515);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1698 = gc_frame_new();
// Begin line 514
  setline(514);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1699 == NULL) {
    strlit1699 = alloc_String("""\x0a""");
    gc_root(strlit1699);
  }
// compilenode returning strlit1699
  gc_frame_newslot(strlit1699);
// compilenode returning *var_outFile
  params[0] = strlit1699;
  partcv[0] = 1;
  Object call1700 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1698);
// compilenode returning call1700
    gc_frame_newslot(call1700);
    if1584 = call1700;
  }
// compilenode returning if1584
// Begin line 517
  setline(517);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 516
  setline(516);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1701 = gc_frame_newslot(*var_curPos);
  Object num1702 = alloc_Float64(1);
// compilenode returning num1702
  int op_slot_right_1701 = gc_frame_newslot(num1702);
  params[0] = num1702;
  partcv[0] = 1;
  Object sum1704 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1704
  *var_curPos = sum1704;
  if (sum1704 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1519(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 518
  setline(518);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1520 = gc_frame_new();
// Begin line 479
  setline(479);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1521 = alloc_Block(NULL, NULL, "PackageManager", 479);
  gc_frame_newslot(block1521);
  block_savedest(block1521);
  Object closure1522 = createclosure(3, "_apply");
setclosureframe(closure1522, stackframe);
  addtoclosure(closure1522, var_curPos);
  addtoclosure(closure1522, var_readFile);
  addtoclosure(closure1522, selfslot);
  struct UserObject *uo1522 = (struct UserObject*)block1521;
  uo1522->data[0] = (Object)closure1522;
  Method *meth_meth_PackageManager__apply1522 = addmethod2pos(block1521, "_apply", &meth_PackageManager__apply1522, 0);
int argcv_meth_PackageManager__apply1522[] = {0};
meth_meth_PackageManager__apply1522->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1522);
  meth_meth_PackageManager__apply1522->definitionModule = modulename;
  meth_meth_PackageManager__apply1522->definitionLine = 478;
// compilenode returning block1521
  gc_frame_newslot(block1521);
// Begin line 518
  setline(518);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1529 = alloc_Block(NULL, NULL, "PackageManager", 518);
  gc_frame_newslot(block1529);
  block_savedest(block1529);
  Object closure1530 = createclosure(6, "_apply");
setclosureframe(closure1530, stackframe);
  addtoclosure(closure1530, var_curPos);
  addtoclosure(closure1530, var_startPos);
  addtoclosure(closure1530, var_readFile);
  addtoclosure(closure1530, var_prefix);
  addtoclosure(closure1530, var_outFile);
  addtoclosure(closure1530, selfslot);
  struct UserObject *uo1530 = (struct UserObject*)block1529;
  uo1530->data[0] = (Object)closure1530;
  Method *meth_meth_PackageManager__apply1530 = addmethod2pos(block1529, "_apply", &meth_PackageManager__apply1530, 0);
int argcv_meth_PackageManager__apply1530[] = {0};
meth_meth_PackageManager__apply1530->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1530);
  meth_meth_PackageManager__apply1530->definitionModule = modulename;
  meth_meth_PackageManager__apply1530->definitionLine = 479;
// compilenode returning block1529
  gc_frame_newslot(block1529);
  params[0] = block1521;
  params[1] = block1529;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1707 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1520);
// compilenode returning call1707
  gc_frame_end(frame);
  return call1707;
}
Object meth_PackageManager_parseAndPrefix1508(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 476
  setline(476);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 475
  setline(475);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1509 = alloc_Float64(1);
// compilenode returning num1509
  *var_curPos = num1509;
  if (num1509 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 477
  setline(477);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 476
  setline(476);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 478
  setline(478);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 477
  setline(477);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array1510 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array1510
  *var_outFile = array1510;
  if (array1510 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 519
  setline(519);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1511 = gc_frame_new();
// Begin line 478
  setline(478);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1513 = alloc_Float64(0);
// compilenode returning num1513
  int op_slot_left_1512 = gc_frame_newslot(num1513);
  int callframe1514 = gc_frame_new();
// compilenode returning *var_readFile
  partcv[0] = 0;
  Object call1515 = callmethod(*var_readFile, "size",
    1, partcv, params);
  gc_frame_end(callframe1514);
// compilenode returning call1515
// compilenode returning call1515
  int op_slot_right_1512 = gc_frame_newslot(call1515);
  params[0] = call1515;
  partcv[0] = 1;
  Object opresult1517 = callmethod(num1513, "..", 1, partcv, params);
// compilenode returning opresult1517
  gc_frame_newslot(opresult1517);
// Begin line 519
  setline(519);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1518 = alloc_Block(NULL, NULL, "PackageManager", 519);
  gc_frame_newslot(block1518);
  block_savedest(block1518);
  Object closure1519 = createclosure(6, "_apply");
setclosureframe(closure1519, stackframe);
  addtoclosure(closure1519, var_curPos);
  addtoclosure(closure1519, var_readFile);
  addtoclosure(closure1519, var_startPos);
  addtoclosure(closure1519, var_prefix);
  addtoclosure(closure1519, var_outFile);
  addtoclosure(closure1519, selfslot);
  struct UserObject *uo1519 = (struct UserObject*)block1518;
  uo1519->data[0] = (Object)closure1519;
  Method *meth_meth_PackageManager__apply1519 = addmethod2pos(block1518, "_apply", &meth_PackageManager__apply1519, 0);
int argcv_meth_PackageManager__apply1519[] = {1};
meth_meth_PackageManager__apply1519->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1519);
  meth_meth_PackageManager__apply1519->definitionModule = modulename;
  meth_meth_PackageManager__apply1519->definitionLine = 478;
// compilenode returning block1518
  gc_frame_newslot(block1518);
  params[0] = opresult1517;
  params[1] = block1518;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1709 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1511);
// compilenode returning call1709
// Begin line 522
  setline(522);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1710 = gc_frame_new();
// Begin line 519
  setline(519);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1711 == NULL) {
    strlit1711 = alloc_String("""\x0a""");
    gc_root(strlit1711);
  }
// compilenode returning strlit1711
  gc_frame_newslot(strlit1711);
// compilenode returning *var_outFile
  params[0] = strlit1711;
  partcv[0] = 1;
  Object call1712 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1710);
// compilenode returning call1712
// Begin line 523
  setline(523);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 522
  setline(522);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1714 == NULL) {
    strlit1714 = alloc_String("OUTFILE ");
    gc_root(strlit1714);
  }
// compilenode returning strlit1714
  int op_slot_left_1713 = gc_frame_newslot(strlit1714);
// compilenode returning *var_outFile
  int op_slot_right_1713 = gc_frame_newslot(*var_outFile);
  params[0] = *var_outFile;
  partcv[0] = 1;
  Object opresult1716 = callmethod(strlit1714, "++", 1, partcv, params);
// compilenode returning opresult1716
  params[0] = opresult1716;
  Object call1717 = gracelib_print(NULL, 1,  params);
// compilenode returning call1717
// Begin line 525
  setline(525);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 523
  setline(523);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_outFile
  return *var_outFile;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply1739(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1740 = gc_frame_newslot(*var_curPos);
  int callframe1741 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1742 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1741);
// compilenode returning call1742
// compilenode returning call1742
  int op_slot_right_1740 = gc_frame_newslot(call1742);
  params[0] = call1742;
  partcv[0] = 1;
  Object opresult1744 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1744
  gc_frame_end(frame);
  return opresult1744;
}
Object meth_PackageManager__apply1747(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 539
  setline(539);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 536
  setline(536);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1750 = callmethod(*var_importStatement, "[]", 1, partcv, params);
// compilenode returning idxres1750
  int op_slot_left_1749 = gc_frame_newslot(idxres1750);
  if (strlit1751 == NULL) {
    strlit1751 = alloc_String("/");
    gc_root(strlit1751);
  }
// compilenode returning strlit1751
  int op_slot_right_1749 = gc_frame_newslot(strlit1751);
  params[0] = strlit1751;
  partcv[0] = 1;
  Object opresult1753 = callmethod(idxres1750, "==", 1, partcv, params);
// compilenode returning opresult1753
  Object if1748 = done;
  if (istrue(opresult1753)) {
// Begin line 538
  setline(538);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 537
  setline(537);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_lastIndex = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if1748 = done;
  } else {
  }
// compilenode returning if1748
// Begin line 540
  setline(540);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 539
  setline(539);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1755 = gc_frame_newslot(*var_curPos);
  Object num1756 = alloc_Float64(1);
// compilenode returning num1756
  int op_slot_right_1755 = gc_frame_newslot(num1756);
  params[0] = num1756;
  partcv[0] = 1;
  Object sum1758 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1758
  *var_curPos = sum1758;
  if (sum1758 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_removeExistingUrls1718(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 532
  setline(532);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 529
  setline(529);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1721 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1722 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1721);
// compilenode returning call1722
// compilenode returning call1722
  int op_slot_left_1720 = gc_frame_newslot(call1722);
  Object num1723 = alloc_Float64(7);
// compilenode returning num1723
  int op_slot_right_1720 = gc_frame_newslot(num1723);
  params[0] = num1723;
  partcv[0] = 1;
  Object opresult1725 = callmethod(call1722, "<", 1, partcv, params);
// compilenode returning opresult1725
  Object if1719 = done;
  if (istrue(opresult1725)) {
// Begin line 531
  setline(531);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 530
  setline(530);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
  return *var_importStatement;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1719 = undefined;
  } else {
  }
// compilenode returning if1719
// Begin line 548
  setline(548);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 532
  setline(532);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1728 = gc_frame_new();
  Object num1729 = alloc_Float64(1);
// compilenode returning num1729
  gc_frame_newslot(num1729);
  Object num1730 = alloc_Float64(7);
// compilenode returning num1730
  gc_frame_newslot(num1730);
// compilenode returning *var_importStatement
  params[0] = num1729;
  params[1] = num1730;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1731 = callmethod(*var_importStatement, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1728);
// compilenode returning call1731
  int op_slot_left_1727 = gc_frame_newslot(call1731);
  if (strlit1732 == NULL) {
    strlit1732 = alloc_String("http://");
    gc_root(strlit1732);
  }
// compilenode returning strlit1732
  int op_slot_right_1727 = gc_frame_newslot(strlit1732);
  params[0] = strlit1732;
  partcv[0] = 1;
  Object opresult1734 = callmethod(call1731, "==", 1, partcv, params);
// compilenode returning opresult1734
  Object if1726 = done;
  if (istrue(opresult1734)) {
// Begin line 534
  setline(534);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 533
  setline(533);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1735 = alloc_Float64(7);
// compilenode returning num1735
  *var_lastIndex = num1735;
  if (num1735 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 535
  setline(535);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 534
  setline(534);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1736 = alloc_Float64(7);
// compilenode returning num1736
  *var_curPos = num1736;
  if (num1736 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1737 = gc_frame_new();
// Begin line 535
  setline(535);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1738 = alloc_Block(NULL, NULL, "PackageManager", 535);
  gc_frame_newslot(block1738);
  block_savedest(block1738);
  Object closure1739 = createclosure(3, "_apply");
setclosureframe(closure1739, stackframe);
  addtoclosure(closure1739, var_curPos);
  addtoclosure(closure1739, var_importStatement);
  addtoclosure(closure1739, selfslot);
  struct UserObject *uo1739 = (struct UserObject*)block1738;
  uo1739->data[0] = (Object)closure1739;
  Method *meth_meth_PackageManager__apply1739 = addmethod2pos(block1738, "_apply", &meth_PackageManager__apply1739, 0);
int argcv_meth_PackageManager__apply1739[] = {0};
meth_meth_PackageManager__apply1739->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1739);
  meth_meth_PackageManager__apply1739->definitionModule = modulename;
  meth_meth_PackageManager__apply1739->definitionLine = 529;
// compilenode returning block1738
  gc_frame_newslot(block1738);
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1746 = alloc_Block(NULL, NULL, "PackageManager", 541);
  gc_frame_newslot(block1746);
  block_savedest(block1746);
  Object closure1747 = createclosure(4, "_apply");
setclosureframe(closure1747, stackframe);
  addtoclosure(closure1747, var_importStatement);
  addtoclosure(closure1747, var_curPos);
  addtoclosure(closure1747, var_lastIndex);
  addtoclosure(closure1747, selfslot);
  struct UserObject *uo1747 = (struct UserObject*)block1746;
  uo1747->data[0] = (Object)closure1747;
  Method *meth_meth_PackageManager__apply1747 = addmethod2pos(block1746, "_apply", &meth_PackageManager__apply1747, 0);
int argcv_meth_PackageManager__apply1747[] = {0};
meth_meth_PackageManager__apply1747->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1747);
  meth_meth_PackageManager__apply1747->definitionModule = modulename;
  meth_meth_PackageManager__apply1747->definitionLine = 535;
// compilenode returning block1746
  gc_frame_newslot(block1746);
  params[0] = block1738;
  params[1] = block1746;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1761 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1737);
// compilenode returning call1761
// Begin line 542
  setline(542);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1762 = gc_frame_new();
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_lastIndex
  int op_slot_left_1763 = gc_frame_newslot(*var_lastIndex);
  Object num1764 = alloc_Float64(1);
// compilenode returning num1764
  int op_slot_right_1763 = gc_frame_newslot(num1764);
  params[0] = num1764;
  partcv[0] = 1;
  Object sum1766 = callmethod(*var_lastIndex, "+", 1, partcv, params);
// compilenode returning sum1766
  gc_frame_newslot(sum1766);
  int callframe1767 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1768 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1767);
// compilenode returning call1768
// compilenode returning call1768
  gc_frame_newslot(call1768);
// compilenode returning *var_importStatement
  params[0] = sum1766;
  params[1] = call1768;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1769 = callmethod(*var_importStatement, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1762);
// compilenode returning call1769
  *var_res = call1769;
  if (call1769 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 543
  setline(543);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 542
  setline(542);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_res
  return *var_res;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1726 = undefined;
  } else {
// Begin line 546
  setline(546);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 545
  setline(545);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
  return *var_importStatement;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1726 = undefined;
  }
// compilenode returning if1726
  gc_frame_end(frame);
  return if1726;
}
Object meth_PackageManager__apply1791(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1792 = gc_frame_newslot(*var_curPos);
  int callframe1793 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1794 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1793);
// compilenode returning call1794
// compilenode returning call1794
  int op_slot_right_1792 = gc_frame_newslot(call1794);
  params[0] = call1794;
  partcv[0] = 1;
  Object opresult1796 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1796
  gc_frame_end(frame);
  return opresult1796;
}
Object meth_PackageManager__apply1799(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 562
  setline(562);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 559
  setline(559);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1802 = callmethod(*var_importStatement, "[]", 1, partcv, params);
// compilenode returning idxres1802
  int op_slot_left_1801 = gc_frame_newslot(idxres1802);
  if (strlit1803 == NULL) {
    strlit1803 = alloc_String("/");
    gc_root(strlit1803);
  }
// compilenode returning strlit1803
  int op_slot_right_1801 = gc_frame_newslot(strlit1803);
  params[0] = strlit1803;
  partcv[0] = 1;
  Object opresult1805 = callmethod(idxres1802, "==", 1, partcv, params);
// compilenode returning opresult1805
  Object if1800 = done;
  if (istrue(opresult1805)) {
// Begin line 561
  setline(561);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 560
  setline(560);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_lastIndex = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if1800 = done;
  } else {
  }
// compilenode returning if1800
// Begin line 563
  setline(563);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 562
  setline(562);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1807 = gc_frame_newslot(*var_curPos);
  Object num1808 = alloc_Float64(1);
// compilenode returning num1808
  int op_slot_right_1807 = gc_frame_newslot(num1808);
  params[0] = num1808;
  partcv[0] = 1;
  Object sum1810 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1810
  *var_curPos = sum1810;
  if (sum1810 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_getBaseUrl1770(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 555
  setline(555);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 552
  setline(552);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1773 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1774 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1773);
// compilenode returning call1774
// compilenode returning call1774
  int op_slot_left_1772 = gc_frame_newslot(call1774);
  Object num1775 = alloc_Float64(7);
// compilenode returning num1775
  int op_slot_right_1772 = gc_frame_newslot(num1775);
  params[0] = num1775;
  partcv[0] = 1;
  Object opresult1777 = callmethod(call1774, "<", 1, partcv, params);
// compilenode returning opresult1777
  Object if1771 = done;
  if (istrue(opresult1777)) {
// Begin line 554
  setline(554);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 553
  setline(553);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
  return *var_importStatement;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1771 = undefined;
  } else {
  }
// compilenode returning if1771
// Begin line 571
  setline(571);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 555
  setline(555);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1780 = gc_frame_new();
  Object num1781 = alloc_Float64(1);
// compilenode returning num1781
  gc_frame_newslot(num1781);
  Object num1782 = alloc_Float64(7);
// compilenode returning num1782
  gc_frame_newslot(num1782);
// compilenode returning *var_importStatement
  params[0] = num1781;
  params[1] = num1782;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1783 = callmethod(*var_importStatement, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1780);
// compilenode returning call1783
  int op_slot_left_1779 = gc_frame_newslot(call1783);
  if (strlit1784 == NULL) {
    strlit1784 = alloc_String("http://");
    gc_root(strlit1784);
  }
// compilenode returning strlit1784
  int op_slot_right_1779 = gc_frame_newslot(strlit1784);
  params[0] = strlit1784;
  partcv[0] = 1;
  Object opresult1786 = callmethod(call1783, "==", 1, partcv, params);
// compilenode returning opresult1786
  Object if1778 = done;
  if (istrue(opresult1786)) {
// Begin line 557
  setline(557);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 556
  setline(556);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1787 = alloc_Float64(7);
// compilenode returning num1787
  *var_lastIndex = num1787;
  if (num1787 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 558
  setline(558);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 557
  setline(557);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1788 = alloc_Float64(7);
// compilenode returning num1788
  *var_curPos = num1788;
  if (num1788 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 564
  setline(564);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1789 = gc_frame_new();
// Begin line 558
  setline(558);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1790 = alloc_Block(NULL, NULL, "PackageManager", 558);
  gc_frame_newslot(block1790);
  block_savedest(block1790);
  Object closure1791 = createclosure(3, "_apply");
setclosureframe(closure1791, stackframe);
  addtoclosure(closure1791, var_curPos);
  addtoclosure(closure1791, var_importStatement);
  addtoclosure(closure1791, selfslot);
  struct UserObject *uo1791 = (struct UserObject*)block1790;
  uo1791->data[0] = (Object)closure1791;
  Method *meth_meth_PackageManager__apply1791 = addmethod2pos(block1790, "_apply", &meth_PackageManager__apply1791, 0);
int argcv_meth_PackageManager__apply1791[] = {0};
meth_meth_PackageManager__apply1791->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1791);
  meth_meth_PackageManager__apply1791->definitionModule = modulename;
  meth_meth_PackageManager__apply1791->definitionLine = 552;
// compilenode returning block1790
  gc_frame_newslot(block1790);
// Begin line 564
  setline(564);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1798 = alloc_Block(NULL, NULL, "PackageManager", 564);
  gc_frame_newslot(block1798);
  block_savedest(block1798);
  Object closure1799 = createclosure(4, "_apply");
setclosureframe(closure1799, stackframe);
  addtoclosure(closure1799, var_importStatement);
  addtoclosure(closure1799, var_curPos);
  addtoclosure(closure1799, var_lastIndex);
  addtoclosure(closure1799, selfslot);
  struct UserObject *uo1799 = (struct UserObject*)block1798;
  uo1799->data[0] = (Object)closure1799;
  Method *meth_meth_PackageManager__apply1799 = addmethod2pos(block1798, "_apply", &meth_PackageManager__apply1799, 0);
int argcv_meth_PackageManager__apply1799[] = {0};
meth_meth_PackageManager__apply1799->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1799);
  meth_meth_PackageManager__apply1799->definitionModule = modulename;
  meth_meth_PackageManager__apply1799->definitionLine = 558;
// compilenode returning block1798
  gc_frame_newslot(block1798);
  params[0] = block1790;
  params[1] = block1798;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1813 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1789);
// compilenode returning call1813
// Begin line 565
  setline(565);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1814 = gc_frame_new();
// Begin line 564
  setline(564);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1815 = alloc_Float64(1);
// compilenode returning num1815
  gc_frame_newslot(num1815);
// compilenode returning *var_lastIndex
  gc_frame_newslot(*var_lastIndex);
// compilenode returning *var_importStatement
  params[0] = num1815;
  params[1] = *var_lastIndex;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1816 = callmethod(*var_importStatement, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1814);
// compilenode returning call1816
  *var_res = call1816;
  if (call1816 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 566
  setline(566);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 565
  setline(565);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_res
  return *var_res;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1778 = undefined;
  } else {
// Begin line 569
  setline(569);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 568
  setline(568);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
  return *var_importStatement;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1778 = undefined;
  }
// compilenode returning if1778
  gc_frame_end(frame);
  return if1778;
}
Object meth_PackageManager__apply1824(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1825 = gc_frame_newslot(*var_count);
  int callframe1826 = gc_frame_new();
// compilenode returning *var_st
  partcv[0] = 0;
  Object call1827 = callmethod(*var_st, "size",
    1, partcv, params);
  gc_frame_end(callframe1826);
// compilenode returning call1827
// compilenode returning call1827
  int op_slot_right_1825 = gc_frame_newslot(call1827);
  params[0] = call1827;
  partcv[0] = 1;
  Object opresult1829 = callmethod(*var_count, "<=", 1, partcv, params);
// compilenode returning opresult1829
  gc_frame_end(frame);
  return opresult1829;
}
Object meth_PackageManager__apply1832(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 580
  setline(580);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 577
  setline(577);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_st
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1835 = callmethod(*var_st, "[]", 1, partcv, params);
// compilenode returning idxres1835
  int op_slot_left_1834 = gc_frame_newslot(idxres1835);
  if (strlit1836 == NULL) {
    strlit1836 = alloc_String("/");
    gc_root(strlit1836);
  }
// compilenode returning strlit1836
  int op_slot_right_1834 = gc_frame_newslot(strlit1836);
  params[0] = strlit1836;
  partcv[0] = 1;
  Object opresult1838 = callmethod(idxres1835, "==", 1, partcv, params);
// compilenode returning opresult1838
  Object if1833 = done;
  if (istrue(opresult1838)) {
// Begin line 579
  setline(579);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 578
  setline(578);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  *var_lastIndex = *var_count;
  if (*var_count == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if1833 = done;
  } else {
  }
// compilenode returning if1833
// Begin line 581
  setline(581);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 580
  setline(580);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  int op_slot_left_1840 = gc_frame_newslot(*var_count);
  Object num1841 = alloc_Float64(1);
// compilenode returning num1841
  int op_slot_right_1840 = gc_frame_newslot(num1841);
  params[0] = num1841;
  partcv[0] = 1;
  Object sum1843 = callmethod(*var_count, "+", 1, partcv, params);
// compilenode returning sum1843
  *var_count = sum1843;
  if (sum1843 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_removeContainingDir1817(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 574
  setline(574);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1818 = alloc_Float64(1);
// compilenode returning num1818
  *var_count = num1818;
  if (num1818 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 576
  setline(576);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1819 = gc_frame_new();
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1820 = alloc_Float64(1);
// compilenode returning num1820
  partcv[0] = 0;
  Object call1821 = callmethod(num1820, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1819);
// compilenode returning call1821
  *var_lastIndex = call1821;
  if (call1821 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 582
  setline(582);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1822 = gc_frame_new();
// Begin line 576
  setline(576);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1823 = alloc_Block(NULL, NULL, "PackageManager", 576);
  gc_frame_newslot(block1823);
  block_savedest(block1823);
  Object closure1824 = createclosure(3, "_apply");
setclosureframe(closure1824, stackframe);
  addtoclosure(closure1824, var_count);
  addtoclosure(closure1824, var_st);
  addtoclosure(closure1824, selfslot);
  struct UserObject *uo1824 = (struct UserObject*)block1823;
  uo1824->data[0] = (Object)closure1824;
  Method *meth_meth_PackageManager__apply1824 = addmethod2pos(block1823, "_apply", &meth_PackageManager__apply1824, 0);
int argcv_meth_PackageManager__apply1824[] = {0};
meth_meth_PackageManager__apply1824->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1824);
  meth_meth_PackageManager__apply1824->definitionModule = modulename;
  meth_meth_PackageManager__apply1824->definitionLine = 558;
// compilenode returning block1823
  gc_frame_newslot(block1823);
// Begin line 582
  setline(582);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1831 = alloc_Block(NULL, NULL, "PackageManager", 582);
  gc_frame_newslot(block1831);
  block_savedest(block1831);
  Object closure1832 = createclosure(4, "_apply");
setclosureframe(closure1832, stackframe);
  addtoclosure(closure1832, var_st);
  addtoclosure(closure1832, var_count);
  addtoclosure(closure1832, var_lastIndex);
  addtoclosure(closure1832, selfslot);
  struct UserObject *uo1832 = (struct UserObject*)block1831;
  uo1832->data[0] = (Object)closure1832;
  Method *meth_meth_PackageManager__apply1832 = addmethod2pos(block1831, "_apply", &meth_PackageManager__apply1832, 0);
int argcv_meth_PackageManager__apply1832[] = {0};
meth_meth_PackageManager__apply1832->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1832);
  meth_meth_PackageManager__apply1832->definitionModule = modulename;
  meth_meth_PackageManager__apply1832->definitionLine = 576;
// compilenode returning block1831
  gc_frame_newslot(block1831);
  params[0] = block1823;
  params[1] = block1831;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1846 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1822);
// compilenode returning call1846
// Begin line 585
  setline(585);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 582
  setline(582);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_lastIndex
  int op_slot_left_1848 = gc_frame_newslot(*var_lastIndex);
  int callframe1849 = gc_frame_new();
  Object num1850 = alloc_Float64(1);
// compilenode returning num1850
  partcv[0] = 0;
  Object call1851 = callmethod(num1850, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1849);
// compilenode returning call1851
  int op_slot_right_1848 = gc_frame_newslot(call1851);
  params[0] = call1851;
  partcv[0] = 1;
  Object opresult1853 = callmethod(*var_lastIndex, "==", 1, partcv, params);
// compilenode returning opresult1853
  Object if1847 = done;
  if (istrue(opresult1853)) {
// Begin line 584
  setline(584);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 583
  setline(583);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_st
  return *var_st;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1847 = undefined;
  } else {
  }
// compilenode returning if1847
// Begin line 585
  setline(585);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1854 = gc_frame_new();
// compilenode returning *var_lastIndex
  int op_slot_left_1855 = gc_frame_newslot(*var_lastIndex);
  Object num1856 = alloc_Float64(1);
// compilenode returning num1856
  int op_slot_right_1855 = gc_frame_newslot(num1856);
  params[0] = num1856;
  partcv[0] = 1;
  Object sum1858 = callmethod(*var_lastIndex, "+", 1, partcv, params);
// compilenode returning sum1858
  gc_frame_newslot(sum1858);
  int callframe1859 = gc_frame_new();
// compilenode returning *var_st
  partcv[0] = 0;
  Object call1860 = callmethod(*var_st, "size",
    1, partcv, params);
  gc_frame_end(callframe1859);
// compilenode returning call1860
// compilenode returning call1860
  gc_frame_newslot(call1860);
// compilenode returning *var_st
  params[0] = sum1858;
  params[1] = call1860;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1861 = callmethod(*var_st, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1854);
// compilenode returning call1861
  return call1861;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply1869(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1870 = gc_frame_newslot(*var_count);
  int callframe1871 = gc_frame_new();
// compilenode returning *var_st
  partcv[0] = 0;
  Object call1872 = callmethod(*var_st, "size",
    1, partcv, params);
  gc_frame_end(callframe1871);
// compilenode returning call1872
// compilenode returning call1872
  int op_slot_right_1870 = gc_frame_newslot(call1872);
  params[0] = call1872;
  partcv[0] = 1;
  Object opresult1874 = callmethod(*var_count, "<=", 1, partcv, params);
// compilenode returning opresult1874
  gc_frame_end(frame);
  return opresult1874;
}
Object meth_PackageManager__apply1877(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 595
  setline(595);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 592
  setline(592);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_st
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1880 = callmethod(*var_st, "[]", 1, partcv, params);
// compilenode returning idxres1880
  int op_slot_left_1879 = gc_frame_newslot(idxres1880);
  if (strlit1881 == NULL) {
    strlit1881 = alloc_String("/");
    gc_root(strlit1881);
  }
// compilenode returning strlit1881
  int op_slot_right_1879 = gc_frame_newslot(strlit1881);
  params[0] = strlit1881;
  partcv[0] = 1;
  Object opresult1883 = callmethod(idxres1880, "==", 1, partcv, params);
// compilenode returning opresult1883
  Object if1878 = done;
  if (istrue(opresult1883)) {
// Begin line 594
  setline(594);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 593
  setline(593);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  *var_last = *var_count;
  if (*var_count == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if1878 = done;
  } else {
  }
// compilenode returning if1878
// Begin line 596
  setline(596);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 595
  setline(595);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  int op_slot_left_1885 = gc_frame_newslot(*var_count);
  Object num1886 = alloc_Float64(1);
// compilenode returning num1886
  int op_slot_right_1885 = gc_frame_newslot(num1886);
  params[0] = num1886;
  partcv[0] = 1;
  Object sum1888 = callmethod(*var_count, "+", 1, partcv, params);
// compilenode returning sum1888
  *var_count = sum1888;
  if (sum1888 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_getContainingDirectory1862(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 590
  setline(590);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 589
  setline(589);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1863 = alloc_Float64(1);
// compilenode returning num1863
  *var_count = num1863;
  if (num1863 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 591
  setline(591);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1864 = gc_frame_new();
// Begin line 590
  setline(590);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1865 = alloc_Float64(1);
// compilenode returning num1865
  partcv[0] = 0;
  Object call1866 = callmethod(num1865, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1864);
// compilenode returning call1866
  *var_last = call1866;
  if (call1866 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 597
  setline(597);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1867 = gc_frame_new();
// Begin line 591
  setline(591);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1868 = alloc_Block(NULL, NULL, "PackageManager", 591);
  gc_frame_newslot(block1868);
  block_savedest(block1868);
  Object closure1869 = createclosure(3, "_apply");
setclosureframe(closure1869, stackframe);
  addtoclosure(closure1869, var_count);
  addtoclosure(closure1869, var_st);
  addtoclosure(closure1869, selfslot);
  struct UserObject *uo1869 = (struct UserObject*)block1868;
  uo1869->data[0] = (Object)closure1869;
  Method *meth_meth_PackageManager__apply1869 = addmethod2pos(block1868, "_apply", &meth_PackageManager__apply1869, 0);
int argcv_meth_PackageManager__apply1869[] = {0};
meth_meth_PackageManager__apply1869->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1869);
  meth_meth_PackageManager__apply1869->definitionModule = modulename;
  meth_meth_PackageManager__apply1869->definitionLine = 585;
// compilenode returning block1868
  gc_frame_newslot(block1868);
// Begin line 597
  setline(597);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1876 = alloc_Block(NULL, NULL, "PackageManager", 597);
  gc_frame_newslot(block1876);
  block_savedest(block1876);
  Object closure1877 = createclosure(4, "_apply");
setclosureframe(closure1877, stackframe);
  addtoclosure(closure1877, var_st);
  addtoclosure(closure1877, var_count);
  addtoclosure(closure1877, var_last);
  addtoclosure(closure1877, selfslot);
  struct UserObject *uo1877 = (struct UserObject*)block1876;
  uo1877->data[0] = (Object)closure1877;
  Method *meth_meth_PackageManager__apply1877 = addmethod2pos(block1876, "_apply", &meth_PackageManager__apply1877, 0);
int argcv_meth_PackageManager__apply1877[] = {0};
meth_meth_PackageManager__apply1877->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1877);
  meth_meth_PackageManager__apply1877->definitionModule = modulename;
  meth_meth_PackageManager__apply1877->definitionLine = 591;
// compilenode returning block1876
  gc_frame_newslot(block1876);
  params[0] = block1868;
  params[1] = block1876;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1891 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1867);
// compilenode returning call1891
// Begin line 600
  setline(600);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 597
  setline(597);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_last
  int op_slot_left_1893 = gc_frame_newslot(*var_last);
  int callframe1894 = gc_frame_new();
  Object num1895 = alloc_Float64(1);
// compilenode returning num1895
  partcv[0] = 0;
  Object call1896 = callmethod(num1895, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1894);
// compilenode returning call1896
  int op_slot_right_1893 = gc_frame_newslot(call1896);
  params[0] = call1896;
  partcv[0] = 1;
  Object opresult1898 = callmethod(*var_last, "==", 1, partcv, params);
// compilenode returning opresult1898
  Object if1892 = done;
  if (istrue(opresult1898)) {
// Begin line 599
  setline(599);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 598
  setline(598);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1900 = gc_frame_new();
  partcv[0] = 0;
  Object call1901 = callmethodflags(self, "getBuildPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1900);
// compilenode returning call1901
  int op_slot_left_1899 = gc_frame_newslot(call1901);
  if (strlit1902 == NULL) {
    strlit1902 = alloc_String("/");
    gc_root(strlit1902);
  }
// compilenode returning strlit1902
  int op_slot_right_1899 = gc_frame_newslot(strlit1902);
  params[0] = strlit1902;
  partcv[0] = 1;
  Object opresult1904 = callmethod(call1901, "++", 1, partcv, params);
// compilenode returning opresult1904
  return opresult1904;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1892 = undefined;
  } else {
  }
// compilenode returning if1892
// Begin line 602
  setline(602);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1905 = gc_frame_new();
// Begin line 600
  setline(600);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1906 = alloc_Float64(0);
// compilenode returning num1906
  gc_frame_newslot(num1906);
// compilenode returning *var_last
  int op_slot_left_1907 = gc_frame_newslot(*var_last);
  Object num1908 = alloc_Float64(1);
// compilenode returning num1908
  int op_slot_right_1907 = gc_frame_newslot(num1908);
  params[0] = num1908;
  partcv[0] = 1;
  Object diff1910 = callmethod(*var_last, "-", 1, partcv, params);
// compilenode returning diff1910
  gc_frame_newslot(diff1910);
// compilenode returning *var_st
  params[0] = num1906;
  params[1] = diff1910;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1911 = callmethod(*var_st, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1905);
// compilenode returning call1911
  return call1911;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply2037(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 627
  setline(627);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe2038 = gc_frame_new();
// Begin line 626
  setline(626);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit2043 == NULL) {
    strlit2043 = alloc_String("rm ");
    gc_root(strlit2043);
  }
// compilenode returning strlit2043
  int op_slot_left_2042 = gc_frame_newslot(strlit2043);
// compilenode returning *var_buildPath
  int op_slot_right_2042 = gc_frame_newslot(*var_buildPath);
  params[0] = *var_buildPath;
  partcv[0] = 1;
  Object opresult2045 = callmethod(strlit2043, "++", 1, partcv, params);
// compilenode returning opresult2045
  int op_slot_left_2041 = gc_frame_newslot(opresult2045);
  if (strlit2046 == NULL) {
    strlit2046 = alloc_String("/__pkg-temp");
    gc_root(strlit2046);
  }
// compilenode returning strlit2046
  int op_slot_right_2041 = gc_frame_newslot(strlit2046);
  params[0] = strlit2046;
  partcv[0] = 1;
  Object opresult2048 = callmethod(opresult2045, "++", 1, partcv, params);
// compilenode returning opresult2048
  int op_slot_left_2040 = gc_frame_newslot(opresult2048);
// compilenode returning *var_s
  int op_slot_right_2040 = gc_frame_newslot(*var_s);
  params[0] = *var_s;
  partcv[0] = 1;
  Object opresult2050 = callmethod(opresult2048, "++", 1, partcv, params);
// compilenode returning opresult2050
  int op_slot_left_2039 = gc_frame_newslot(opresult2050);
  if (strlit2051 == NULL) {
    strlit2051 = alloc_String("");
    gc_root(strlit2051);
  }
// compilenode returning strlit2051
  int op_slot_right_2039 = gc_frame_newslot(strlit2051);
  params[0] = strlit2051;
  partcv[0] = 1;
  Object opresult2053 = callmethod(opresult2050, "++", 1, partcv, params);
// compilenode returning opresult2053
  gc_frame_newslot(opresult2053);
// compilenode returning *var_io
  params[0] = opresult2053;
  partcv[0] = 1;
  Object call2054 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe2038);
// compilenode returning call2054
  gc_frame_end(frame);
  return call2054;
}
Object meth_PackageManager_package1912(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 31, (flags>>24)&0xff);
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
// Begin line 606
  setline(606);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 605
  setline(605);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1913 == NULL) {
    strlit1913 = alloc_String("~/Packagemanager/setup.grace");
    gc_root(strlit1913);
  }
// compilenode returning strlit1913
  *var_setupFile = strlit1913;
  if (strlit1913 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 607
  setline(607);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1914 = gc_frame_new();
  partcv[0] = 0;
  Object call1915 = callmethodflags(self, "getBuildPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1914);
// compilenode returning call1915
  *var_buildPath = call1915;
  if (call1915 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 610
  setline(610);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 607
  setline(607);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1917 = gc_frame_new();
  int callframe1918 = gc_frame_new();
// compilenode returning *var_folder
  int op_slot_left_1919 = gc_frame_newslot(*var_folder);
  if (strlit1920 == NULL) {
    strlit1920 = alloc_String("/pkg.grace");
    gc_root(strlit1920);
  }
// compilenode returning strlit1920
  int op_slot_right_1919 = gc_frame_newslot(strlit1920);
  params[0] = strlit1920;
  partcv[0] = 1;
  Object opresult1922 = callmethod(*var_folder, "++", 1, partcv, params);
// compilenode returning opresult1922
  gc_frame_newslot(opresult1922);
// compilenode returning *var_io
  params[0] = opresult1922;
  partcv[0] = 1;
  Object call1923 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe1918);
// compilenode returning call1923
  partcv[0] = 0;
  Object call1924 = callmethod(call1923, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1917);
// compilenode returning call1924
  Object if1916 = done;
  if (istrue(call1924)) {
// Begin line 609
  setline(609);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 608
  setline(608);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1925 == NULL) {
    strlit1925 = alloc_String("Folder must contain pkg.grace file in order to create package");
    gc_root(strlit1925);
  }
// compilenode returning strlit1925
  params[0] = strlit1925;
  Object call1926 = gracelib_print(NULL, 1,  params);
// compilenode returning call1926
    gc_frame_newslot(call1926);
    if1916 = call1926;
  } else {
  }
// compilenode returning if1916
// Begin line 611
  setline(611);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 610
  setline(610);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1928 == NULL) {
    strlit1928 = alloc_String("BP ");
    gc_root(strlit1928);
  }
// compilenode returning strlit1928
  int op_slot_left_1927 = gc_frame_newslot(strlit1928);
// compilenode returning *var_buildPath
  int op_slot_right_1927 = gc_frame_newslot(*var_buildPath);
  params[0] = *var_buildPath;
  partcv[0] = 1;
  Object opresult1930 = callmethod(strlit1928, "++", 1, partcv, params);
// compilenode returning opresult1930
  params[0] = opresult1930;
  Object call1931 = gracelib_print(NULL, 1,  params);
// compilenode returning call1931
// Begin line 612
  setline(612);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1932 = gc_frame_new();
// Begin line 611
  setline(611);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1935 == NULL) {
    strlit1935 = alloc_String("");
    gc_root(strlit1935);
  }
// compilenode returning strlit1935
  int op_slot_left_1934 = gc_frame_newslot(strlit1935);
// compilenode returning *var_folder
  int op_slot_right_1934 = gc_frame_newslot(*var_folder);
  params[0] = *var_folder;
  partcv[0] = 1;
  Object opresult1937 = callmethod(strlit1935, "++", 1, partcv, params);
// compilenode returning opresult1937
  int op_slot_left_1933 = gc_frame_newslot(opresult1937);
  if (strlit1938 == NULL) {
    strlit1938 = alloc_String("/pkg.grace");
    gc_root(strlit1938);
  }
// compilenode returning strlit1938
  int op_slot_right_1933 = gc_frame_newslot(strlit1938);
  params[0] = strlit1938;
  partcv[0] = 1;
  Object opresult1940 = callmethod(opresult1937, "++", 1, partcv, params);
// compilenode returning opresult1940
  gc_frame_newslot(opresult1940);
  if (strlit1941 == NULL) {
    strlit1941 = alloc_String("r");
    gc_root(strlit1941);
  }
// compilenode returning strlit1941
  gc_frame_newslot(strlit1941);
// compilenode returning *var_io
  params[0] = opresult1940;
  params[1] = strlit1941;
  partcv[0] = 2;
  Object call1942 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1932);
// compilenode returning call1942
  *var_open = call1942;
  if (call1942 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 613
  setline(613);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 612
  setline(612);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1943 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call1944 = callmethod(*var_open, "read",
    1, partcv, params);
  gc_frame_end(callframe1943);
// compilenode returning call1944
// compilenode returning call1944
  *var_pkgData = call1944;
  if (call1944 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 613
  setline(613);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1945 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call1946 = callmethod(*var_open, "close",
    1, partcv, params);
  gc_frame_end(callframe1945);
// compilenode returning call1946
// compilenode returning call1946
// Begin line 615
  setline(615);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 614
  setline(614);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1949 == NULL) {
    strlit1949 = alloc_String("");
    gc_root(strlit1949);
  }
// compilenode returning strlit1949
  int op_slot_left_1948 = gc_frame_newslot(strlit1949);
// compilenode returning *var_buildPath
  int op_slot_right_1948 = gc_frame_newslot(*var_buildPath);
  params[0] = *var_buildPath;
  partcv[0] = 1;
  Object opresult1951 = callmethod(strlit1949, "++", 1, partcv, params);
// compilenode returning opresult1951
  int op_slot_left_1947 = gc_frame_newslot(opresult1951);
  if (strlit1952 == NULL) {
    strlit1952 = alloc_String("/__pkg-temp");
    gc_root(strlit1952);
  }
// compilenode returning strlit1952
  int op_slot_right_1947 = gc_frame_newslot(strlit1952);
  params[0] = strlit1952;
  partcv[0] = 1;
  Object opresult1954 = callmethod(opresult1951, "++", 1, partcv, params);
// compilenode returning opresult1954
  *var_loc = opresult1954;
  if (opresult1954 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 616
  setline(616);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1955 = gc_frame_new();
// Begin line 615
  setline(615);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1958 == NULL) {
    strlit1958 = alloc_String("");
    gc_root(strlit1958);
  }
// compilenode returning strlit1958
  int op_slot_left_1957 = gc_frame_newslot(strlit1958);
// compilenode returning *var_loc
  int op_slot_right_1957 = gc_frame_newslot(*var_loc);
  params[0] = *var_loc;
  partcv[0] = 1;
  Object opresult1960 = callmethod(strlit1958, "++", 1, partcv, params);
// compilenode returning opresult1960
  int op_slot_left_1956 = gc_frame_newslot(opresult1960);
  if (strlit1961 == NULL) {
    strlit1961 = alloc_String("");
    gc_root(strlit1961);
  }
// compilenode returning strlit1961
  int op_slot_right_1956 = gc_frame_newslot(strlit1961);
  params[0] = strlit1961;
  partcv[0] = 1;
  Object opresult1963 = callmethod(opresult1960, "++", 1, partcv, params);
// compilenode returning opresult1963
  gc_frame_newslot(opresult1963);
  if (strlit1964 == NULL) {
    strlit1964 = alloc_String("w");
    gc_root(strlit1964);
  }
// compilenode returning strlit1964
  gc_frame_newslot(strlit1964);
// compilenode returning *var_io
  params[0] = opresult1963;
  params[1] = strlit1964;
  partcv[0] = 2;
  Object call1965 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1955);
// compilenode returning call1965
  *var_create = call1965;
  if (call1965 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 617
  setline(617);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 616
  setline(616);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pkgData
  int op_slot_left_1966 = gc_frame_newslot(*var_pkgData);
  if (strlit1967 == NULL) {
    strlit1967 = alloc_String("""\x0a""def __bundle = true");
    gc_root(strlit1967);
  }
// compilenode returning strlit1967
  int op_slot_right_1966 = gc_frame_newslot(strlit1967);
  params[0] = strlit1967;
  partcv[0] = 1;
  Object opresult1969 = callmethod(*var_pkgData, "++", 1, partcv, params);
// compilenode returning opresult1969
  *var_pkgData = opresult1969;
  if (opresult1969 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 618
  setline(618);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 617
  setline(617);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pkgData
  int op_slot_left_1971 = gc_frame_newslot(*var_pkgData);
  if (strlit1974 == NULL) {
    strlit1974 = alloc_String("""\x0a""def __loc = ""\x22""");
    gc_root(strlit1974);
  }
// compilenode returning strlit1974
  int op_slot_left_1973 = gc_frame_newslot(strlit1974);
// compilenode returning *var_folder
  int op_slot_right_1973 = gc_frame_newslot(*var_folder);
  params[0] = *var_folder;
  partcv[0] = 1;
  Object opresult1976 = callmethod(strlit1974, "++", 1, partcv, params);
// compilenode returning opresult1976
  int op_slot_left_1972 = gc_frame_newslot(opresult1976);
  if (strlit1977 == NULL) {
    strlit1977 = alloc_String("""\x22""");
    gc_root(strlit1977);
  }
// compilenode returning strlit1977
  int op_slot_right_1972 = gc_frame_newslot(strlit1977);
  params[0] = strlit1977;
  partcv[0] = 1;
  Object opresult1979 = callmethod(opresult1976, "++", 1, partcv, params);
// compilenode returning opresult1979
  int op_slot_right_1971 = gc_frame_newslot(opresult1979);
  params[0] = opresult1979;
  partcv[0] = 1;
  Object opresult1981 = callmethod(*var_pkgData, "++", 1, partcv, params);
// compilenode returning opresult1981
  *var_pkgData = opresult1981;
  if (opresult1981 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 619
  setline(619);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1983 = gc_frame_new();
// Begin line 618
  setline(618);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pkgData
  gc_frame_newslot(*var_pkgData);
// compilenode returning *var_create
  params[0] = *var_pkgData;
  partcv[0] = 1;
  Object call1984 = callmethod(*var_create, "write",
    1, partcv, params);
  gc_frame_end(callframe1983);
// compilenode returning call1984
// Begin line 619
  setline(619);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1985 = gc_frame_new();
// compilenode returning *var_create
  partcv[0] = 0;
  Object call1986 = callmethod(*var_create, "close",
    1, partcv, params);
  gc_frame_end(callframe1985);
// compilenode returning call1986
// compilenode returning call1986
// Begin line 621
  setline(621);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1987 = gc_frame_new();
// Begin line 620
  setline(620);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1992 == NULL) {
    strlit1992 = alloc_String("mv ");
    gc_root(strlit1992);
  }
// compilenode returning strlit1992
  int op_slot_left_1991 = gc_frame_newslot(strlit1992);
// compilenode returning *var_loc
  int op_slot_right_1991 = gc_frame_newslot(*var_loc);
  params[0] = *var_loc;
  partcv[0] = 1;
  Object opresult1994 = callmethod(strlit1992, "++", 1, partcv, params);
// compilenode returning opresult1994
  int op_slot_left_1990 = gc_frame_newslot(opresult1994);
  if (strlit1995 == NULL) {
    strlit1995 = alloc_String(" ");
    gc_root(strlit1995);
  }
// compilenode returning strlit1995
  int op_slot_right_1990 = gc_frame_newslot(strlit1995);
  params[0] = strlit1995;
  partcv[0] = 1;
  Object opresult1997 = callmethod(opresult1994, "++", 1, partcv, params);
// compilenode returning opresult1997
  int op_slot_left_1989 = gc_frame_newslot(opresult1997);
// compilenode returning *var_loc
  int op_slot_right_1989 = gc_frame_newslot(*var_loc);
  params[0] = *var_loc;
  partcv[0] = 1;
  Object opresult1999 = callmethod(opresult1997, "++", 1, partcv, params);
// compilenode returning opresult1999
  int op_slot_left_1988 = gc_frame_newslot(opresult1999);
  if (strlit2000 == NULL) {
    strlit2000 = alloc_String(".grace");
    gc_root(strlit2000);
  }
// compilenode returning strlit2000
  int op_slot_right_1988 = gc_frame_newslot(strlit2000);
  params[0] = strlit2000;
  partcv[0] = 1;
  Object opresult2002 = callmethod(opresult1999, "++", 1, partcv, params);
// compilenode returning opresult2002
  gc_frame_newslot(opresult2002);
// compilenode returning *var_io
  params[0] = opresult2002;
  partcv[0] = 1;
  Object call2003 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe1987);
// compilenode returning call2003
// Begin line 622
  setline(622);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 621
  setline(621);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit2006 == NULL) {
    strlit2006 = alloc_String("build path = ");
    gc_root(strlit2006);
  }
// compilenode returning strlit2006
  int op_slot_left_2005 = gc_frame_newslot(strlit2006);
// compilenode returning *var_buildPath
  int op_slot_right_2005 = gc_frame_newslot(*var_buildPath);
  params[0] = *var_buildPath;
  partcv[0] = 1;
  Object opresult2008 = callmethod(strlit2006, "++", 1, partcv, params);
// compilenode returning opresult2008
  int op_slot_left_2004 = gc_frame_newslot(opresult2008);
  if (strlit2009 == NULL) {
    strlit2009 = alloc_String(" ++ minigrace");
    gc_root(strlit2009);
  }
// compilenode returning strlit2009
  int op_slot_right_2004 = gc_frame_newslot(strlit2009);
  params[0] = strlit2009;
  partcv[0] = 1;
  Object opresult2011 = callmethod(opresult2008, "++", 1, partcv, params);
// compilenode returning opresult2011
  params[0] = opresult2011;
  Object call2012 = gracelib_print(NULL, 1,  params);
// compilenode returning call2012
// Begin line 624
  setline(624);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe2013 = gc_frame_new();
// Begin line 622
  setline(622);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit2018 == NULL) {
    strlit2018 = alloc_String("cat ");
    gc_root(strlit2018);
  }
// compilenode returning strlit2018
  int op_slot_left_2017 = gc_frame_newslot(strlit2018);
// compilenode returning *var_setupFile
  int op_slot_right_2017 = gc_frame_newslot(*var_setupFile);
  params[0] = *var_setupFile;
  partcv[0] = 1;
  Object opresult2020 = callmethod(strlit2018, "++", 1, partcv, params);
// compilenode returning opresult2020
  int op_slot_left_2016 = gc_frame_newslot(opresult2020);
  if (strlit2021 == NULL) {
    strlit2021 = alloc_String(" | ");
    gc_root(strlit2021);
  }
// compilenode returning strlit2021
  int op_slot_right_2016 = gc_frame_newslot(strlit2021);
  params[0] = strlit2021;
  partcv[0] = 1;
  Object opresult2023 = callmethod(opresult2020, "++", 1, partcv, params);
// compilenode returning opresult2023
  int op_slot_left_2015 = gc_frame_newslot(opresult2023);
// compilenode returning *var_buildPath
  int op_slot_right_2015 = gc_frame_newslot(*var_buildPath);
  params[0] = *var_buildPath;
  partcv[0] = 1;
  Object opresult2025 = callmethod(opresult2023, "++", 1, partcv, params);
// compilenode returning opresult2025
  int op_slot_left_2014 = gc_frame_newslot(opresult2025);
  if (strlit2026 == NULL) {
    strlit2026 = alloc_String("/minigrace");
    gc_root(strlit2026);
  }
// compilenode returning strlit2026
  int op_slot_right_2014 = gc_frame_newslot(strlit2026);
  params[0] = strlit2026;
  partcv[0] = 1;
  Object opresult2028 = callmethod(opresult2025, "++", 1, partcv, params);
// compilenode returning opresult2028
  gc_frame_newslot(opresult2028);
// compilenode returning *var_io
  params[0] = opresult2028;
  partcv[0] = 1;
  Object call2029 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe2013);
// compilenode returning call2029
// Begin line 624
  setline(624);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array2030 = alloc_BuiltinList();
  gc_pause();
  if (strlit2031 == NULL) {
    strlit2031 = alloc_String(".grace");
    gc_root(strlit2031);
  }
// compilenode returning strlit2031
  params[0] = strlit2031;
  partcv[0] = 1;
  callmethod(array2030, "push", 1, partcv, params);
  if (strlit2032 == NULL) {
    strlit2032 = alloc_String(".gct");
    gc_root(strlit2032);
  }
// compilenode returning strlit2032
  params[0] = strlit2032;
  partcv[0] = 1;
  callmethod(array2030, "push", 1, partcv, params);
  if (strlit2033 == NULL) {
    strlit2033 = alloc_String(".gcn");
    gc_root(strlit2033);
  }
// compilenode returning strlit2033
  params[0] = strlit2033;
  partcv[0] = 1;
  callmethod(array2030, "push", 1, partcv, params);
  if (strlit2034 == NULL) {
    strlit2034 = alloc_String(".c");
    gc_root(strlit2034);
  }
// compilenode returning strlit2034
  params[0] = strlit2034;
  partcv[0] = 1;
  callmethod(array2030, "push", 1, partcv, params);
  gc_unpause();
// compilenode returning array2030
  *var_suffix = array2030;
  if (array2030 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 629
  setline(629);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe2035 = gc_frame_new();
// Begin line 625
  setline(625);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_suffix
  gc_frame_newslot(*var_suffix);
// Begin line 629
  setline(629);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block2036 = alloc_Block(NULL, NULL, "PackageManager", 629);
  gc_frame_newslot(block2036);
  block_savedest(block2036);
  Object closure2037 = createclosure(3, "_apply");
setclosureframe(closure2037, stackframe);
  addtoclosure(closure2037, var_buildPath);
  addtoclosure(closure2037, var_io);
  addtoclosure(closure2037, selfslot);
  struct UserObject *uo2037 = (struct UserObject*)block2036;
  uo2037->data[0] = (Object)closure2037;
  Method *meth_meth_PackageManager__apply2037 = addmethod2pos(block2036, "_apply", &meth_PackageManager__apply2037, 0);
int argcv_meth_PackageManager__apply2037[] = {1};
meth_meth_PackageManager__apply2037->type = alloc_MethodType(1, argcv_meth_PackageManager__apply2037);
  meth_meth_PackageManager__apply2037->definitionModule = modulename;
  meth_meth_PackageManager__apply2037->definitionLine = 619;
// compilenode returning block2036
  gc_frame_newslot(block2036);
  params[0] = *var_suffix;
  params[1] = block2036;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call2056 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe2035);
// compilenode returning call2056
  gc_frame_end(frame);
  return call2056;
}
Object meth_PackageManager_imported2057(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 32, (flags>>24)&0xff);
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
// Begin line 6
  setline(6);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_imported
  gc_frame_end(frame);
  return *var_imported;
}
Object meth_PackageManager_imported_58__61_2058(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 33, (flags>>24)&0xff);
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
Object meth_PackageManager_toProcess2061(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 34, (flags>>24)&0xff);
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
// Begin line 7
  setline(7);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toProcess
  gc_frame_end(frame);
  return *var_toProcess;
}
Object meth_PackageManager_toProcess_58__61_2062(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 35, (flags>>24)&0xff);
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
Object meth_PackageManager_verbose2065(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 36, (flags>>24)&0xff);
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
// Begin line 8
  setline(8);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_verbose
  gc_frame_end(frame);
  return *var_verbose;
}
Object meth_PackageManager_verbose_58__61_2066(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 37, (flags>>24)&0xff);
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
Object meth_PackageManager_global2069(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 38, (flags>>24)&0xff);
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
// Begin line 9
  setline(9);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_global
  gc_frame_end(frame);
  return *var_global;
}
Object meth_PackageManager_global_58__61_2070(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 39, (flags>>24)&0xff);
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
Object meth_PackageManager_bundlePath2073(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 40, (flags>>24)&0xff);
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
// Begin line 10
  setline(10);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_bundlePath
  gc_frame_end(frame);
  return *var_bundlePath;
}
Object meth_PackageManager_bundlePath_58__61_2074(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 41, (flags>>24)&0xff);
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
Object meth_PackageManager_baseUrl2077(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 42, (flags>>24)&0xff);
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
// Begin line 11
  setline(11);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_baseUrl
  gc_frame_end(frame);
  return *var_baseUrl;
}
Object meth_PackageManager_baseUrl_58__61_2078(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 43, (flags>>24)&0xff);
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
Object meth_PackageManager_curFile2080(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 44, (flags>>24)&0xff);
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
// Begin line 12
  setline(12);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curFile
  gc_frame_end(frame);
  return *var_curFile;
}
Object meth_PackageManager_curFile_58__61_2081(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 45, (flags>>24)&0xff);
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
  Object self = alloc_obj2(48, 48);
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
  struct StackFrameObject *stackframe = alloc_StackFrame(48, NULL);
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
Object *var_setup = alloc_var();
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
// Begin line 16
  setline(16);
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
  meth_meth_PackageManager_parseArgs0->definitionLine = 16;
// compilenode returning 
// Begin line 47
  setline(47);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo68 = (struct UserObject*)self;
  uo68->data[2] = emptyclosure;
  Method *meth_meth_PackageManager_forArgs_40__41_do68 = addmethod2pos(self, "forArgs()do", &meth_PackageManager_forArgs_40__41_do68, 2);
int argcv_meth_PackageManager_forArgs_40__41_do68[] = {1, 1};
meth_meth_PackageManager_forArgs_40__41_do68->type = alloc_MethodType(2, argcv_meth_PackageManager_forArgs_40__41_do68);
  meth_meth_PackageManager_forArgs_40__41_do68->flags |= MFLAG_CONFIDENTIAL;
  meth_meth_PackageManager_forArgs_40__41_do68->definitionModule = modulename;
  meth_meth_PackageManager_forArgs_40__41_do68->definitionLine = 47;
  Method *meth_meth_PackageManager_forArgs_40__41_do68_object = addmethod2pos(self, "forArgs()do()object", &meth_PackageManager_forArgs_40__41_do68_object, 2);
int argcv_meth_PackageManager_forArgs_40__41_do68_object[] = {1, 1};
meth_meth_PackageManager_forArgs_40__41_do68_object->type = alloc_MethodType(2, argcv_meth_PackageManager_forArgs_40__41_do68_object);
  meth_meth_PackageManager_forArgs_40__41_do68_object->flags |= MFLAG_CONFIDENTIAL;
  meth_meth_PackageManager_forArgs_40__41_do68_object->definitionModule = modulename;
  meth_meth_PackageManager_forArgs_40__41_do68_object->definitionLine = 47;
// compilenode returning 
// Begin line 102
  setline(102);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure221 = createclosure(1, "listInstalled");
setclosureframe(closure221, stackframe);
  addtoclosure(closure221, var_sys);
  struct UserObject *uo221 = (struct UserObject*)self;
  uo221->data[3] = (Object)closure221;
  Method *meth_meth_PackageManager_listInstalled221 = addmethod2pos(self, "listInstalled", &meth_PackageManager_listInstalled221, 3);
int argcv_meth_PackageManager_listInstalled221[] = {0};
meth_meth_PackageManager_listInstalled221->type = alloc_MethodType(1, argcv_meth_PackageManager_listInstalled221);
  meth_meth_PackageManager_listInstalled221->definitionModule = modulename;
  meth_meth_PackageManager_listInstalled221->definitionLine = 102;
// compilenode returning 
// Begin line 110
  setline(110);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure250 = createclosure(1, "checkListPath");
setclosureframe(closure250, stackframe);
  addtoclosure(closure250, var_io);
  struct UserObject *uo250 = (struct UserObject*)self;
  uo250->data[4] = (Object)closure250;
  Method *meth_meth_PackageManager_checkListPath250 = addmethod2pos(self, "checkListPath", &meth_PackageManager_checkListPath250, 4);
int argcv_meth_PackageManager_checkListPath250[] = {1};
meth_meth_PackageManager_checkListPath250->type = alloc_MethodType(1, argcv_meth_PackageManager_checkListPath250);
meth_meth_PackageManager_checkListPath250->type->types[0] = type_String;
meth_meth_PackageManager_checkListPath250->type->names[0] = "path";
  meth_meth_PackageManager_checkListPath250->definitionModule = modulename;
  meth_meth_PackageManager_checkListPath250->definitionLine = 110;
// compilenode returning 
// Begin line 118
  setline(118);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure266 = createclosure(1, "recurseDirectory");
setclosureframe(closure266, stackframe);
  addtoclosure(closure266, var_io);
  struct UserObject *uo266 = (struct UserObject*)self;
  uo266->data[5] = (Object)closure266;
  Method *meth_meth_PackageManager_recurseDirectory266 = addmethod2pos(self, "recurseDirectory", &meth_PackageManager_recurseDirectory266, 5);
int argcv_meth_PackageManager_recurseDirectory266[] = {2};
meth_meth_PackageManager_recurseDirectory266->type = alloc_MethodType(1, argcv_meth_PackageManager_recurseDirectory266);
  meth_meth_PackageManager_recurseDirectory266->definitionModule = modulename;
  meth_meth_PackageManager_recurseDirectory266->definitionLine = 118;
// compilenode returning 
// Begin line 130
  setline(130);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure319 = createclosure(1, "doGet");
setclosureframe(closure319, stackframe);
  addtoclosure(closure319, var_imported);
  struct UserObject *uo319 = (struct UserObject*)self;
  uo319->data[6] = (Object)closure319;
  Method *meth_meth_PackageManager_doGet319 = addmethod2pos(self, "doGet", &meth_PackageManager_doGet319, 6);
int argcv_meth_PackageManager_doGet319[] = {1};
meth_meth_PackageManager_doGet319->type = alloc_MethodType(1, argcv_meth_PackageManager_doGet319);
  meth_meth_PackageManager_doGet319->definitionModule = modulename;
  meth_meth_PackageManager_doGet319->definitionLine = 130;
// compilenode returning 
// Begin line 144
  setline(144);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure358 = createclosure(1, "setBaseUrl");
setclosureframe(closure358, stackframe);
  addtoclosure(closure358, var_baseUrl);
  struct UserObject *uo358 = (struct UserObject*)self;
  uo358->data[7] = (Object)closure358;
  Method *meth_meth_PackageManager_setBaseUrl358 = addmethod2pos(self, "setBaseUrl", &meth_PackageManager_setBaseUrl358, 7);
int argcv_meth_PackageManager_setBaseUrl358[] = {1};
meth_meth_PackageManager_setBaseUrl358->type = alloc_MethodType(1, argcv_meth_PackageManager_setBaseUrl358);
meth_meth_PackageManager_setBaseUrl358->type->types[0] = type_String;
meth_meth_PackageManager_setBaseUrl358->type->names[0] = "baseAddress";
  meth_meth_PackageManager_setBaseUrl358->definitionModule = modulename;
  meth_meth_PackageManager_setBaseUrl358->definitionLine = 144;
// compilenode returning 
// Begin line 148
  setline(148);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure362 = createclosure(1, "setFile");
setclosureframe(closure362, stackframe);
  addtoclosure(closure362, var_curFile);
  struct UserObject *uo362 = (struct UserObject*)self;
  uo362->data[8] = (Object)closure362;
  Method *meth_meth_PackageManager_setFile362 = addmethod2pos(self, "setFile", &meth_PackageManager_setFile362, 8);
int argcv_meth_PackageManager_setFile362[] = {1};
meth_meth_PackageManager_setFile362->type = alloc_MethodType(1, argcv_meth_PackageManager_setFile362);
  meth_meth_PackageManager_setFile362->definitionModule = modulename;
  meth_meth_PackageManager_setFile362->definitionLine = 148;
// compilenode returning 
// Begin line 160
  setline(160);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure385 = createclosure(3, "fetchImports");
setclosureframe(closure385, stackframe);
  addtoclosure(closure385, var_curFile);
  addtoclosure(closure385, var_imported);
  addtoclosure(closure385, var_toProcess);
  struct UserObject *uo385 = (struct UserObject*)self;
  uo385->data[9] = (Object)closure385;
  Method *meth_meth_PackageManager_fetchImports385 = addmethod2pos(self, "fetchImports", &meth_PackageManager_fetchImports385, 9);
int argcv_meth_PackageManager_fetchImports385[] = {1};
meth_meth_PackageManager_fetchImports385->type = alloc_MethodType(1, argcv_meth_PackageManager_fetchImports385);
  meth_meth_PackageManager_fetchImports385->definitionModule = modulename;
  meth_meth_PackageManager_fetchImports385->definitionLine = 160;
// compilenode returning 
// Begin line 185
  setline(185);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure461 = createclosure(1, "performCurlFetch");
setclosureframe(closure461, stackframe);
  addtoclosure(closure461, var_curl);
  struct UserObject *uo461 = (struct UserObject*)self;
  uo461->data[10] = (Object)closure461;
  Method *meth_meth_PackageManager_performCurlFetch461 = addmethod2pos(self, "performCurlFetch", &meth_PackageManager_performCurlFetch461, 10);
int argcv_meth_PackageManager_performCurlFetch461[] = {1};
meth_meth_PackageManager_performCurlFetch461->type = alloc_MethodType(1, argcv_meth_PackageManager_performCurlFetch461);
  meth_meth_PackageManager_performCurlFetch461->definitionModule = modulename;
  meth_meth_PackageManager_performCurlFetch461->definitionLine = 185;
// compilenode returning 
// Begin line 202
  setline(202);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure520 = createclosure(1, "setFileData");
setclosureframe(closure520, stackframe);
  addtoclosure(closure520, var_baseUrl);
  struct UserObject *uo520 = (struct UserObject*)self;
  uo520->data[11] = (Object)closure520;
  Method *meth_meth_PackageManager_setFileData520 = addmethod2pos(self, "setFileData", &meth_PackageManager_setFileData520, 11);
int argcv_meth_PackageManager_setFileData520[] = {1};
meth_meth_PackageManager_setFileData520->type = alloc_MethodType(1, argcv_meth_PackageManager_setFileData520);
  meth_meth_PackageManager_setFileData520->definitionModule = modulename;
  meth_meth_PackageManager_setFileData520->definitionLine = 202;
// compilenode returning 
// Begin line 233
  setline(233);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure598 = createclosure(3, "findExisting");
setclosureframe(closure598, stackframe);
  addtoclosure(closure598, var_io);
  addtoclosure(closure598, var_sys);
  addtoclosure(closure598, var_bundlePath);
  struct UserObject *uo598 = (struct UserObject*)self;
  uo598->data[12] = (Object)closure598;
  Method *meth_meth_PackageManager_findExisting598 = addmethod2pos(self, "findExisting", &meth_PackageManager_findExisting598, 12);
int argcv_meth_PackageManager_findExisting598[] = {1};
meth_meth_PackageManager_findExisting598->type = alloc_MethodType(1, argcv_meth_PackageManager_findExisting598);
  meth_meth_PackageManager_findExisting598->definitionModule = modulename;
  meth_meth_PackageManager_findExisting598->definitionLine = 233;
// compilenode returning 
// Begin line 263
  setline(263);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo795 = (struct UserObject*)self;
  uo795->data[13] = emptyclosure;
  Method *meth_meth_PackageManager_validateFile795 = addmethod2pos(self, "validateFile", &meth_PackageManager_validateFile795, 13);
int argcv_meth_PackageManager_validateFile795[] = {1};
meth_meth_PackageManager_validateFile795->type = alloc_MethodType(1, argcv_meth_PackageManager_validateFile795);
  meth_meth_PackageManager_validateFile795->definitionModule = modulename;
  meth_meth_PackageManager_validateFile795->definitionLine = 263;
// compilenode returning 
// Begin line 273
  setline(273);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure818 = createclosure(3, "write");
setclosureframe(closure818, stackframe);
  addtoclosure(closure818, var_global);
  addtoclosure(closure818, var_sys);
  addtoclosure(closure818, var_io);
  struct UserObject *uo818 = (struct UserObject*)self;
  uo818->data[14] = (Object)closure818;
  Method *meth_meth_PackageManager_write818 = addmethod2pos(self, "write", &meth_PackageManager_write818, 14);
int argcv_meth_PackageManager_write818[] = {1};
meth_meth_PackageManager_write818->type = alloc_MethodType(1, argcv_meth_PackageManager_write818);
  meth_meth_PackageManager_write818->definitionModule = modulename;
  meth_meth_PackageManager_write818->definitionLine = 273;
// compilenode returning 
// Begin line 288
  setline(288);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure855 = createclosure(2, "getBuildPath");
setclosureframe(closure855, stackframe);
  addtoclosure(closure855, var_sys);
  addtoclosure(closure855, var_io);
  struct UserObject *uo855 = (struct UserObject*)self;
  uo855->data[15] = (Object)closure855;
  Method *meth_meth_PackageManager_getBuildPath855 = addmethod2pos(self, "getBuildPath", &meth_PackageManager_getBuildPath855, 15);
int argcv_meth_PackageManager_getBuildPath855[] = {0};
meth_meth_PackageManager_getBuildPath855->type = alloc_MethodType(1, argcv_meth_PackageManager_getBuildPath855);
  meth_meth_PackageManager_getBuildPath855->definitionModule = modulename;
  meth_meth_PackageManager_getBuildPath855->definitionLine = 288;
// compilenode returning 
// Begin line 305
  setline(305);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure893 = createclosure(3, "compile");
setclosureframe(closure893, stackframe);
  addtoclosure(closure893, var_global);
  addtoclosure(closure893, var_sys);
  addtoclosure(closure893, var_io);
  struct UserObject *uo893 = (struct UserObject*)self;
  uo893->data[16] = (Object)closure893;
  Method *meth_meth_PackageManager_compile893 = addmethod2pos(self, "compile", &meth_PackageManager_compile893, 16);
int argcv_meth_PackageManager_compile893[] = {1};
meth_meth_PackageManager_compile893->type = alloc_MethodType(1, argcv_meth_PackageManager_compile893);
  meth_meth_PackageManager_compile893->definitionModule = modulename;
  meth_meth_PackageManager_compile893->definitionLine = 305;
// compilenode returning 
// Begin line 321
  setline(321);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure933 = createclosure(1, "createDirectory");
setclosureframe(closure933, stackframe);
  addtoclosure(closure933, var_io);
  struct UserObject *uo933 = (struct UserObject*)self;
  uo933->data[17] = (Object)closure933;
  Method *meth_meth_PackageManager_createDirectory933 = addmethod2pos(self, "createDirectory", &meth_PackageManager_createDirectory933, 17);
int argcv_meth_PackageManager_createDirectory933[] = {1};
meth_meth_PackageManager_createDirectory933->type = alloc_MethodType(1, argcv_meth_PackageManager_createDirectory933);
  meth_meth_PackageManager_createDirectory933->definitionModule = modulename;
  meth_meth_PackageManager_createDirectory933->definitionLine = 321;
// compilenode returning 
// Begin line 345
  setline(345);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1011 = createclosure(1, "parseFile");
setclosureframe(closure1011, stackframe);
  addtoclosure(closure1011, var_done);
  struct UserObject *uo1011 = (struct UserObject*)self;
  uo1011->data[18] = (Object)closure1011;
  Method *meth_meth_PackageManager_parseFile1011 = addmethod2pos(self, "parseFile", &meth_PackageManager_parseFile1011, 18);
int argcv_meth_PackageManager_parseFile1011[] = {1};
meth_meth_PackageManager_parseFile1011->type = alloc_MethodType(1, argcv_meth_PackageManager_parseFile1011);
  meth_meth_PackageManager_parseFile1011->definitionModule = modulename;
  meth_meth_PackageManager_parseFile1011->definitionLine = 345;
// compilenode returning 
// Begin line 362
  setline(362);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1071 = (struct UserObject*)self;
  uo1071->data[19] = emptyclosure;
  Method *meth_meth_PackageManager_processLine1071 = addmethod2pos(self, "processLine", &meth_PackageManager_processLine1071, 19);
int argcv_meth_PackageManager_processLine1071[] = {1};
meth_meth_PackageManager_processLine1071->type = alloc_MethodType(1, argcv_meth_PackageManager_processLine1071);
  meth_meth_PackageManager_processLine1071->definitionModule = modulename;
  meth_meth_PackageManager_processLine1071->definitionLine = 362;
// compilenode returning 
// Begin line 381
  setline(381);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1152 = createclosure(2, "parseImport");
setclosureframe(closure1152, stackframe);
  addtoclosure(closure1152, var_imported);
  addtoclosure(closure1152, var_toProcess);
  struct UserObject *uo1152 = (struct UserObject*)self;
  uo1152->data[20] = (Object)closure1152;
  Method *meth_meth_PackageManager_parseImport1152 = addmethod2pos(self, "parseImport", &meth_PackageManager_parseImport1152, 20);
int argcv_meth_PackageManager_parseImport1152[] = {1};
meth_meth_PackageManager_parseImport1152->type = alloc_MethodType(1, argcv_meth_PackageManager_parseImport1152);
  meth_meth_PackageManager_parseImport1152->definitionModule = modulename;
  meth_meth_PackageManager_parseImport1152->definitionLine = 381;
// compilenode returning 
// Begin line 398
  setline(398);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1223 = (struct UserObject*)self;
  uo1223->data[21] = emptyclosure;
  Method *meth_meth_PackageManager_skipWhiteSpace1223 = addmethod2pos(self, "skipWhiteSpace", &meth_PackageManager_skipWhiteSpace1223, 21);
int argcv_meth_PackageManager_skipWhiteSpace1223[] = {2};
meth_meth_PackageManager_skipWhiteSpace1223->type = alloc_MethodType(1, argcv_meth_PackageManager_skipWhiteSpace1223);
  meth_meth_PackageManager_skipWhiteSpace1223->definitionModule = modulename;
  meth_meth_PackageManager_skipWhiteSpace1223->definitionLine = 398;
// compilenode returning 
// Begin line 406
  setline(406);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1250 = (struct UserObject*)self;
  uo1250->data[22] = emptyclosure;
  Method *meth_meth_PackageManager_displayHelp1250 = addmethod2pos(self, "displayHelp", &meth_PackageManager_displayHelp1250, 22);
int argcv_meth_PackageManager_displayHelp1250[] = {0};
meth_meth_PackageManager_displayHelp1250->type = alloc_MethodType(1, argcv_meth_PackageManager_displayHelp1250);
  meth_meth_PackageManager_displayHelp1250->definitionModule = modulename;
  meth_meth_PackageManager_displayHelp1250->definitionLine = 406;
// compilenode returning 
// Begin line 412
  setline(412);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1257 = createclosure(1, "printMessage");
setclosureframe(closure1257, stackframe);
  addtoclosure(closure1257, var_verbose);
  struct UserObject *uo1257 = (struct UserObject*)self;
  uo1257->data[23] = (Object)closure1257;
  Method *meth_meth_PackageManager_printMessage1257 = addmethod2pos(self, "printMessage", &meth_PackageManager_printMessage1257, 23);
int argcv_meth_PackageManager_printMessage1257[] = {1};
meth_meth_PackageManager_printMessage1257->type = alloc_MethodType(1, argcv_meth_PackageManager_printMessage1257);
  meth_meth_PackageManager_printMessage1257->definitionModule = modulename;
  meth_meth_PackageManager_printMessage1257->definitionLine = 412;
// compilenode returning 
// Begin line 421
  setline(421);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1260 = createclosure(2, "bundle");
setclosureframe(closure1260, stackframe);
  addtoclosure(closure1260, var_imported);
  addtoclosure(closure1260, var_io);
  struct UserObject *uo1260 = (struct UserObject*)self;
  uo1260->data[24] = (Object)closure1260;
  Method *meth_meth_PackageManager_bundle1260 = addmethod2pos(self, "bundle", &meth_PackageManager_bundle1260, 24);
int argcv_meth_PackageManager_bundle1260[] = {2};
meth_meth_PackageManager_bundle1260->type = alloc_MethodType(1, argcv_meth_PackageManager_bundle1260);
  meth_meth_PackageManager_bundle1260->definitionModule = modulename;
  meth_meth_PackageManager_bundle1260->definitionLine = 421;
// compilenode returning 
// Begin line 449
  setline(449);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1430 = createclosure(1, "setImportDest");
setclosureframe(closure1430, stackframe);
  addtoclosure(closure1430, var_io);
  struct UserObject *uo1430 = (struct UserObject*)self;
  uo1430->data[25] = (Object)closure1430;
  Method *meth_meth_PackageManager_setImportDest1430 = addmethod2pos(self, "setImportDest", &meth_PackageManager_setImportDest1430, 25);
int argcv_meth_PackageManager_setImportDest1430[] = {2};
meth_meth_PackageManager_setImportDest1430->type = alloc_MethodType(1, argcv_meth_PackageManager_setImportDest1430);
  meth_meth_PackageManager_setImportDest1430->definitionModule = modulename;
  meth_meth_PackageManager_setImportDest1430->definitionLine = 449;
// compilenode returning 
// Begin line 474
  setline(474);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1508 = (struct UserObject*)self;
  uo1508->data[26] = emptyclosure;
  Method *meth_meth_PackageManager_parseAndPrefix1508 = addmethod2pos(self, "parseAndPrefix", &meth_PackageManager_parseAndPrefix1508, 26);
int argcv_meth_PackageManager_parseAndPrefix1508[] = {3};
meth_meth_PackageManager_parseAndPrefix1508->type = alloc_MethodType(1, argcv_meth_PackageManager_parseAndPrefix1508);
meth_meth_PackageManager_parseAndPrefix1508->type->types[0] = type_String;
meth_meth_PackageManager_parseAndPrefix1508->type->names[0] = "readFile";
meth_meth_PackageManager_parseAndPrefix1508->type->types[1] = type_String;
meth_meth_PackageManager_parseAndPrefix1508->type->names[1] = "address";
meth_meth_PackageManager_parseAndPrefix1508->type->types[2] = type_String;
meth_meth_PackageManager_parseAndPrefix1508->type->names[2] = "prefix";
  meth_meth_PackageManager_parseAndPrefix1508->definitionModule = modulename;
  meth_meth_PackageManager_parseAndPrefix1508->definitionLine = 474;
// compilenode returning 
// Begin line 527
  setline(527);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1718 = (struct UserObject*)self;
  uo1718->data[27] = emptyclosure;
  Method *meth_meth_PackageManager_removeExistingUrls1718 = addmethod2pos(self, "removeExistingUrls", &meth_PackageManager_removeExistingUrls1718, 27);
int argcv_meth_PackageManager_removeExistingUrls1718[] = {1};
meth_meth_PackageManager_removeExistingUrls1718->type = alloc_MethodType(1, argcv_meth_PackageManager_removeExistingUrls1718);
meth_meth_PackageManager_removeExistingUrls1718->type->types[0] = type_String;
meth_meth_PackageManager_removeExistingUrls1718->type->names[0] = "importStatement";
  meth_meth_PackageManager_removeExistingUrls1718->definitionModule = modulename;
  meth_meth_PackageManager_removeExistingUrls1718->definitionLine = 527;
// compilenode returning 
// Begin line 550
  setline(550);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1770 = (struct UserObject*)self;
  uo1770->data[28] = emptyclosure;
  Method *meth_meth_PackageManager_getBaseUrl1770 = addmethod2pos(self, "getBaseUrl", &meth_PackageManager_getBaseUrl1770, 28);
int argcv_meth_PackageManager_getBaseUrl1770[] = {1};
meth_meth_PackageManager_getBaseUrl1770->type = alloc_MethodType(1, argcv_meth_PackageManager_getBaseUrl1770);
meth_meth_PackageManager_getBaseUrl1770->type->types[0] = type_String;
meth_meth_PackageManager_getBaseUrl1770->type->names[0] = "importStatement";
  meth_meth_PackageManager_getBaseUrl1770->definitionModule = modulename;
  meth_meth_PackageManager_getBaseUrl1770->definitionLine = 550;
// compilenode returning 
// Begin line 573
  setline(573);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1817 = (struct UserObject*)self;
  uo1817->data[29] = emptyclosure;
  Method *meth_meth_PackageManager_removeContainingDir1817 = addmethod2pos(self, "removeContainingDir", &meth_PackageManager_removeContainingDir1817, 29);
int argcv_meth_PackageManager_removeContainingDir1817[] = {1};
meth_meth_PackageManager_removeContainingDir1817->type = alloc_MethodType(1, argcv_meth_PackageManager_removeContainingDir1817);
meth_meth_PackageManager_removeContainingDir1817->type->types[0] = type_String;
meth_meth_PackageManager_removeContainingDir1817->type->names[0] = "st";
  meth_meth_PackageManager_removeContainingDir1817->definitionModule = modulename;
  meth_meth_PackageManager_removeContainingDir1817->definitionLine = 573;
// compilenode returning 
// Begin line 588
  setline(588);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1862 = (struct UserObject*)self;
  uo1862->data[30] = emptyclosure;
  Method *meth_meth_PackageManager_getContainingDirectory1862 = addmethod2pos(self, "getContainingDirectory", &meth_PackageManager_getContainingDirectory1862, 30);
int argcv_meth_PackageManager_getContainingDirectory1862[] = {1};
meth_meth_PackageManager_getContainingDirectory1862->type = alloc_MethodType(1, argcv_meth_PackageManager_getContainingDirectory1862);
meth_meth_PackageManager_getContainingDirectory1862->type->types[0] = type_String;
meth_meth_PackageManager_getContainingDirectory1862->type->names[0] = "st";
  meth_meth_PackageManager_getContainingDirectory1862->definitionModule = modulename;
  meth_meth_PackageManager_getContainingDirectory1862->definitionLine = 588;
// compilenode returning 
// Begin line 604
  setline(604);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1912 = createclosure(1, "package");
setclosureframe(closure1912, stackframe);
  addtoclosure(closure1912, var_io);
  struct UserObject *uo1912 = (struct UserObject*)self;
  uo1912->data[31] = (Object)closure1912;
  Method *meth_meth_PackageManager_package1912 = addmethod2pos(self, "package", &meth_PackageManager_package1912, 31);
int argcv_meth_PackageManager_package1912[] = {1};
meth_meth_PackageManager_package1912->type = alloc_MethodType(1, argcv_meth_PackageManager_package1912);
meth_meth_PackageManager_package1912->type->types[0] = type_String;
meth_meth_PackageManager_package1912->type->names[0] = "folder";
  meth_meth_PackageManager_package1912->definitionModule = modulename;
  meth_meth_PackageManager_package1912->definitionLine = 604;
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
// Begin line 4
  setline(4);
  setmodule(modulename);
  setsource(originalSourceLines);
// Import of SetupCall as setup
  if (module_SetupCall == NULL)
    module_SetupCall = module_SetupCall_init();
  *var_setup = module_SetupCall;
// compilenode returning done
// compilenode returning 
// Begin line 7
  setline(7);
  setmodule(modulename);
  setsource(originalSourceLines);
  *var_imported = undefined;
// Begin line 619
  setline(619);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2057 = createclosure(1, "imported");
setclosureframe(closure2057, stackframe);
  addtoclosure(closure2057, var_imported);
  struct UserObject *uo2057 = (struct UserObject*)self;
  uo2057->data[32] = (Object)closure2057;
  Method *meth_meth_PackageManager_imported2057 = addmethod2pos(self, "imported", &meth_PackageManager_imported2057, 32);
int argcv_meth_PackageManager_imported2057[] = {0};
meth_meth_PackageManager_imported2057->type = alloc_MethodType(1, argcv_meth_PackageManager_imported2057);
  meth_meth_PackageManager_imported2057->definitionModule = modulename;
  meth_meth_PackageManager_imported2057->definitionLine = 619;
// compilenode returning 
// Begin line 619
  setline(619);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2058 = createclosure(1, "imported:=");
setclosureframe(closure2058, stackframe);
  addtoclosure(closure2058, var_imported);
  struct UserObject *uo2058 = (struct UserObject*)self;
  uo2058->data[33] = (Object)closure2058;
  Method *meth_meth_PackageManager_imported_58__61_2058 = addmethod2pos(self, "imported:=", &meth_PackageManager_imported_58__61_2058, 33);
int argcv_meth_PackageManager_imported_58__61_2058[] = {1};
meth_meth_PackageManager_imported_58__61_2058->type = alloc_MethodType(1, argcv_meth_PackageManager_imported_58__61_2058);
  meth_meth_PackageManager_imported_58__61_2058->definitionModule = modulename;
  meth_meth_PackageManager_imported_58__61_2058->definitionLine = 619;
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
  Object array2060 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array2060
  *var_toProcess = array2060;
  if (array2060 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 619
  setline(619);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2061 = createclosure(1, "toProcess");
setclosureframe(closure2061, stackframe);
  addtoclosure(closure2061, var_toProcess);
  struct UserObject *uo2061 = (struct UserObject*)self;
  uo2061->data[34] = (Object)closure2061;
  Method *meth_meth_PackageManager_toProcess2061 = addmethod2pos(self, "toProcess", &meth_PackageManager_toProcess2061, 34);
int argcv_meth_PackageManager_toProcess2061[] = {0};
meth_meth_PackageManager_toProcess2061->type = alloc_MethodType(1, argcv_meth_PackageManager_toProcess2061);
  meth_meth_PackageManager_toProcess2061->definitionModule = modulename;
  meth_meth_PackageManager_toProcess2061->definitionLine = 619;
// compilenode returning 
// Begin line 619
  setline(619);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2062 = createclosure(1, "toProcess:=");
setclosureframe(closure2062, stackframe);
  addtoclosure(closure2062, var_toProcess);
  struct UserObject *uo2062 = (struct UserObject*)self;
  uo2062->data[35] = (Object)closure2062;
  Method *meth_meth_PackageManager_toProcess_58__61_2062 = addmethod2pos(self, "toProcess:=", &meth_PackageManager_toProcess_58__61_2062, 35);
int argcv_meth_PackageManager_toProcess_58__61_2062[] = {1};
meth_meth_PackageManager_toProcess_58__61_2062->type = alloc_MethodType(1, argcv_meth_PackageManager_toProcess_58__61_2062);
  meth_meth_PackageManager_toProcess_58__61_2062->definitionModule = modulename;
  meth_meth_PackageManager_toProcess_58__61_2062->definitionLine = 619;
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
  Object bool2064 = alloc_Boolean(0);
// compilenode returning bool2064
  *var_verbose = bool2064;
  if (bool2064 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 619
  setline(619);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2065 = createclosure(1, "verbose");
setclosureframe(closure2065, stackframe);
  addtoclosure(closure2065, var_verbose);
  struct UserObject *uo2065 = (struct UserObject*)self;
  uo2065->data[36] = (Object)closure2065;
  Method *meth_meth_PackageManager_verbose2065 = addmethod2pos(self, "verbose", &meth_PackageManager_verbose2065, 36);
int argcv_meth_PackageManager_verbose2065[] = {0};
meth_meth_PackageManager_verbose2065->type = alloc_MethodType(1, argcv_meth_PackageManager_verbose2065);
  meth_meth_PackageManager_verbose2065->definitionModule = modulename;
  meth_meth_PackageManager_verbose2065->definitionLine = 619;
// compilenode returning 
// Begin line 619
  setline(619);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2066 = createclosure(1, "verbose:=");
setclosureframe(closure2066, stackframe);
  addtoclosure(closure2066, var_verbose);
  struct UserObject *uo2066 = (struct UserObject*)self;
  uo2066->data[37] = (Object)closure2066;
  Method *meth_meth_PackageManager_verbose_58__61_2066 = addmethod2pos(self, "verbose:=", &meth_PackageManager_verbose_58__61_2066, 37);
int argcv_meth_PackageManager_verbose_58__61_2066[] = {1};
meth_meth_PackageManager_verbose_58__61_2066->type = alloc_MethodType(1, argcv_meth_PackageManager_verbose_58__61_2066);
  meth_meth_PackageManager_verbose_58__61_2066->definitionModule = modulename;
  meth_meth_PackageManager_verbose_58__61_2066->definitionLine = 619;
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
  Object bool2068 = alloc_Boolean(0);
// compilenode returning bool2068
  *var_global = bool2068;
  if (bool2068 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 619
  setline(619);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2069 = createclosure(1, "global");
setclosureframe(closure2069, stackframe);
  addtoclosure(closure2069, var_global);
  struct UserObject *uo2069 = (struct UserObject*)self;
  uo2069->data[38] = (Object)closure2069;
  Method *meth_meth_PackageManager_global2069 = addmethod2pos(self, "global", &meth_PackageManager_global2069, 38);
int argcv_meth_PackageManager_global2069[] = {0};
meth_meth_PackageManager_global2069->type = alloc_MethodType(1, argcv_meth_PackageManager_global2069);
  meth_meth_PackageManager_global2069->definitionModule = modulename;
  meth_meth_PackageManager_global2069->definitionLine = 619;
// compilenode returning 
// Begin line 619
  setline(619);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2070 = createclosure(1, "global:=");
setclosureframe(closure2070, stackframe);
  addtoclosure(closure2070, var_global);
  struct UserObject *uo2070 = (struct UserObject*)self;
  uo2070->data[39] = (Object)closure2070;
  Method *meth_meth_PackageManager_global_58__61_2070 = addmethod2pos(self, "global:=", &meth_PackageManager_global_58__61_2070, 39);
int argcv_meth_PackageManager_global_58__61_2070[] = {1};
meth_meth_PackageManager_global_58__61_2070->type = alloc_MethodType(1, argcv_meth_PackageManager_global_58__61_2070);
  meth_meth_PackageManager_global_58__61_2070->definitionModule = modulename;
  meth_meth_PackageManager_global_58__61_2070->definitionLine = 619;
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
  if (strlit2072 == NULL) {
    strlit2072 = alloc_String("");
    gc_root(strlit2072);
  }
// compilenode returning strlit2072
  *var_bundlePath = strlit2072;
  if (strlit2072 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 619
  setline(619);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2073 = createclosure(1, "bundlePath");
setclosureframe(closure2073, stackframe);
  addtoclosure(closure2073, var_bundlePath);
  struct UserObject *uo2073 = (struct UserObject*)self;
  uo2073->data[40] = (Object)closure2073;
  Method *meth_meth_PackageManager_bundlePath2073 = addmethod2pos(self, "bundlePath", &meth_PackageManager_bundlePath2073, 40);
int argcv_meth_PackageManager_bundlePath2073[] = {0};
meth_meth_PackageManager_bundlePath2073->type = alloc_MethodType(1, argcv_meth_PackageManager_bundlePath2073);
  meth_meth_PackageManager_bundlePath2073->definitionModule = modulename;
  meth_meth_PackageManager_bundlePath2073->definitionLine = 619;
// compilenode returning 
// Begin line 619
  setline(619);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2074 = createclosure(1, "bundlePath:=");
setclosureframe(closure2074, stackframe);
  addtoclosure(closure2074, var_bundlePath);
  struct UserObject *uo2074 = (struct UserObject*)self;
  uo2074->data[41] = (Object)closure2074;
  Method *meth_meth_PackageManager_bundlePath_58__61_2074 = addmethod2pos(self, "bundlePath:=", &meth_PackageManager_bundlePath_58__61_2074, 41);
int argcv_meth_PackageManager_bundlePath_58__61_2074[] = {1};
meth_meth_PackageManager_bundlePath_58__61_2074->type = alloc_MethodType(1, argcv_meth_PackageManager_bundlePath_58__61_2074);
  meth_meth_PackageManager_bundlePath_58__61_2074->definitionModule = modulename;
  meth_meth_PackageManager_bundlePath_58__61_2074->definitionLine = 619;
// compilenode returning 
// compilenode returning done
// Begin line 12
  setline(12);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 11
  setline(11);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit2076 == NULL) {
    strlit2076 = alloc_String("");
    gc_root(strlit2076);
  }
// compilenode returning strlit2076
  *var_baseUrl = strlit2076;
  if (strlit2076 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 619
  setline(619);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2077 = createclosure(1, "baseUrl");
setclosureframe(closure2077, stackframe);
  addtoclosure(closure2077, var_baseUrl);
  struct UserObject *uo2077 = (struct UserObject*)self;
  uo2077->data[42] = (Object)closure2077;
  Method *meth_meth_PackageManager_baseUrl2077 = addmethod2pos(self, "baseUrl", &meth_PackageManager_baseUrl2077, 42);
int argcv_meth_PackageManager_baseUrl2077[] = {0};
meth_meth_PackageManager_baseUrl2077->type = alloc_MethodType(1, argcv_meth_PackageManager_baseUrl2077);
  meth_meth_PackageManager_baseUrl2077->definitionModule = modulename;
  meth_meth_PackageManager_baseUrl2077->definitionLine = 619;
// compilenode returning 
// Begin line 619
  setline(619);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2078 = createclosure(1, "baseUrl:=");
setclosureframe(closure2078, stackframe);
  addtoclosure(closure2078, var_baseUrl);
  struct UserObject *uo2078 = (struct UserObject*)self;
  uo2078->data[43] = (Object)closure2078;
  Method *meth_meth_PackageManager_baseUrl_58__61_2078 = addmethod2pos(self, "baseUrl:=", &meth_PackageManager_baseUrl_58__61_2078, 43);
int argcv_meth_PackageManager_baseUrl_58__61_2078[] = {1};
meth_meth_PackageManager_baseUrl_58__61_2078->type = alloc_MethodType(1, argcv_meth_PackageManager_baseUrl_58__61_2078);
  meth_meth_PackageManager_baseUrl_58__61_2078->definitionModule = modulename;
  meth_meth_PackageManager_baseUrl_58__61_2078->definitionLine = 619;
// compilenode returning 
// compilenode returning done
// Begin line 14
  setline(14);
  setmodule(modulename);
  setsource(originalSourceLines);
  *var_curFile = undefined;
// Begin line 619
  setline(619);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2080 = createclosure(1, "curFile");
setclosureframe(closure2080, stackframe);
  addtoclosure(closure2080, var_curFile);
  struct UserObject *uo2080 = (struct UserObject*)self;
  uo2080->data[44] = (Object)closure2080;
  Method *meth_meth_PackageManager_curFile2080 = addmethod2pos(self, "curFile", &meth_PackageManager_curFile2080, 44);
int argcv_meth_PackageManager_curFile2080[] = {0};
meth_meth_PackageManager_curFile2080->type = alloc_MethodType(1, argcv_meth_PackageManager_curFile2080);
  meth_meth_PackageManager_curFile2080->definitionModule = modulename;
  meth_meth_PackageManager_curFile2080->definitionLine = 619;
// compilenode returning 
// Begin line 619
  setline(619);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2081 = createclosure(1, "curFile:=");
setclosureframe(closure2081, stackframe);
  addtoclosure(closure2081, var_curFile);
  struct UserObject *uo2081 = (struct UserObject*)self;
  uo2081->data[45] = (Object)closure2081;
  Method *meth_meth_PackageManager_curFile_58__61_2081 = addmethod2pos(self, "curFile:=", &meth_PackageManager_curFile_58__61_2081, 45);
int argcv_meth_PackageManager_curFile_58__61_2081[] = {1};
meth_meth_PackageManager_curFile_58__61_2081->type = alloc_MethodType(1, argcv_meth_PackageManager_curFile_58__61_2081);
  meth_meth_PackageManager_curFile_58__61_2081->definitionModule = modulename;
  meth_meth_PackageManager_curFile_58__61_2081->definitionLine = 619;
// compilenode returning 
// compilenode returning done
// Begin line 14
  setline(14);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 16
  setline(16);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe2083 = gc_frame_new();
// Begin line 14
  setline(14);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe2084 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call2085 = callmethod(*var_sys, "argv",
    1, partcv, params);
  gc_frame_end(callframe2084);
// compilenode returning call2085
// compilenode returning call2085
  gc_frame_newslot(call2085);
  params[0] = call2085;
  partcv[0] = 1;
  Object call2086 = callmethodflags(self, "parseArgs", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe2083);
// compilenode returning call2086
// Begin line 16
  setline(16);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// compilenode returning 
// Begin line 47
  setline(47);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 102
  setline(102);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 110
  setline(110);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 118
  setline(118);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 130
  setline(130);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 144
  setline(144);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 148
  setline(148);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 160
  setline(160);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 185
  setline(185);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 202
  setline(202);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 233
  setline(233);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 263
  setline(263);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 273
  setline(273);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 288
  setline(288);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 305
  setline(305);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 321
  setline(321);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 345
  setline(345);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 362
  setline(362);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 381
  setline(381);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 398
  setline(398);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 406
  setline(406);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 412
  setline(412);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 421
  setline(421);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 449
  setline(449);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 474
  setline(474);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 527
  setline(527);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 550
  setline(550);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 573
  setline(573);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 588
  setline(588);
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
