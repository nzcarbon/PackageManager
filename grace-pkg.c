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
static const char modulename[] = "grace-pkg";
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
  "        on.doubleValue \"setAddress\" do { address, prefix ->",
  "            setImportDest(address,prefix)",
  "        }",
  "        on.flag \"verbose\" do { ",
  "            verbose := true",
  "        }",
  "        on.flag \"global\" do {",
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
  "    bundlePath := getContainingDirectory(toBundle)",
  "    var newDir := createDirectory(\"{name}/\")",
  "    var newFileName := removeContainingDir(toBundle)",
  "    var toOpen := io.open(\"{toBundle}\",\"r\")",
  "    var openData := toOpen.read",
  "    var toWrite := io.open(\"{name}/{newFileName}\", \"w\")",
  "    toWrite.write(openData)",
  "    fetchImports(toBundle)",
  "    print(\"Imported size = \"++imported.size)",
  "    while{imported.size > 0 }do{",
  "        var curImport := imported.pop",
  "        toWrite := io.open(\"{name}/{removeContainingDir(curImport.address)}\",\"w\")",
  "        toWrite.write(curImport.data)   ",
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
  NULL
};
Object reader_grace_45_pkg_outer_66(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject*)self;
  return uo->data[0];
}
Object reader_grace_45_pkg_outer_371(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject*)self;
  return uo->data[0];
}
Object reader_grace_45_pkg_address_372(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  return uo->data[1];
}
Object writer_grace_45_pkg_address_372(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  uo->data[1] = args[0];
  return done;
}
Object reader_grace_45_pkg_data_373(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  return uo->data[2];
}
Object writer_grace_45_pkg_data_373(Object self, int nparams, int *argcv, Object* args, int flags) {
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
static Object strlit83;
static Object strlit86;
static Object strlit123;
static Object strlit126;
static Object strlit186;
static Object strlit196;
static Object strlit199;
static Object strlit214;
static Object strlit217;
static Object strlit220;
static Object strlit226;
static Object strlit228;
static Object strlit236;
static Object strlit238;
static Object strlit248;
static Object strlit251;
static Object strlit256;
static Object strlit267;
static Object strlit271;
static Object strlit286;
static Object strlit299;
static Object strlit305;
static Object strlit327;
static Object strlit367;
static ClassData objclass370;
static Object strlit383;
static Object strlit388;
static Object strlit399;
static Object strlit407;
static Object strlit412;
static Object strlit443;
static Object strlit450;
static Object strlit461;
static Object strlit467;
static Object strlit472;
static Object strlit485;
static Object strlit490;
static Object strlit497;
static Object strlit504;
static Object strlit521;
static Object strlit561;
static Object strlit577;
static Object strlit595;
static Object strlit602;
static Object strlit609;
static Object strlit612;
static Object strlit617;
static Object strlit624;
static Object strlit627;
static Object strlit638;
static Object strlit641;
static Object strlit646;
static Object strlit654;
static Object strlit659;
static Object strlit670;
static Object strlit675;
static Object strlit680;
static Object strlit688;
static Object strlit693;
static Object strlit704;
static Object strlit709;
static Object strlit714;
static Object strlit723;
static Object strlit728;
static Object strlit733;
static Object strlit737;
static Object strlit745;
static Object strlit750;
static Object strlit755;
static Object strlit758;
static Object strlit763;
static Object strlit772;
static Object strlit775;
static Object strlit780;
static Object strlit783;
static Object strlit803;
static Object strlit806;
static Object strlit811;
static Object strlit813;
static Object strlit818;
static Object strlit820;
static Object strlit839;
static Object strlit850;
static Object strlit852;
static Object strlit866;
static Object strlit872;
static Object strlit876;
static Object strlit886;
static Object strlit888;
static Object strlit893;
static Object strlit895;
static Object strlit901;
static Object strlit904;
static Object strlit914;
static Object strlit927;
static Object strlit928;
static Object strlit932;
static Object strlit935;
static Object strlit958;
static Object strlit965;
static Object strlit975;
static Object strlit986;
static Object strlit992;
static Object strlit995;
static Object strlit1030;
static Object strlit1075;
static Object strlit1092;
static Object strlit1111;
static Object strlit1137;
static Object strlit1146;
static Object strlit1173;
static Object strlit1182;
static Object strlit1209;
static Object strlit1227;
static Object strlit1243;
static Object strlit1245;
static Object strlit1247;
static Object strlit1261;
static Object strlit1264;
static Object strlit1273;
static Object strlit1276;
static Object strlit1279;
static Object strlit1288;
static Object strlit1291;
static Object strlit1296;
static Object strlit1299;
static Object strlit1306;
static Object strlit1331;
static Object strlit1334;
static Object strlit1343;
static Object strlit1346;
static Object strlit1358;
static Object strlit1382;
static Object strlit1388;
static Object strlit1393;
static Object strlit1405;
static Object strlit1408;
static Object strlit1417;
static Object strlit1422;
static Object strlit1468;
static Object strlit1489;
static Object strlit1522;
static Object strlit1528;
static Object strlit1538;
static Object strlit1555;
static Object strlit1560;
static Object strlit1596;
static Object strlit1599;
static Object strlit1614;
static Object strlit1624;
static Object strlit1636;
static Object strlit1639;
static Object strlit1657;
static Object strlit1676;
static Object strlit1709;
static Object strlit1728;
static Object strlit1761;
static Object strlit1806;
static Object strlit1827;
Object module_sys;
Object module_sys_init();
Object module_io;
Object module_io_init();
Object module_curl;
Object module_curl_init();
static Object strlit1852;
static Object strlit1856;
Object meth_grace_45_pkg__apply13(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 22
  setline(22);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe14 = gc_frame_new();
// Begin line 21
  setline(21);
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
Object meth_grace_45_pkg__apply21(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 25
  setline(25);
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
Object meth_grace_45_pkg__apply29(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 28
  setline(28);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe30 = gc_frame_new();
// Begin line 27
  setline(27);
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
Object meth_grace_45_pkg__apply37(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 31
  setline(31);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe38 = gc_frame_new();
// Begin line 30
  setline(30);
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
Object meth_grace_45_pkg__apply45(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 34
  setline(34);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 33
  setline(33);
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
Object meth_grace_45_pkg__apply53(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 37
  setline(37);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 36
  setline(36);
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
Object meth_grace_45_pkg__apply7(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 20
  setline(20);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 19
  setline(19);
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
// Begin line 23
  setline(23);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe10 = gc_frame_new();
// Begin line 20
  setline(20);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit11 == NULL) {
    strlit11 = alloc_String("get");
    gc_root(strlit11);
  }
// compilenode returning strlit11
  gc_frame_newslot(strlit11);
// Begin line 23
  setline(23);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block12 = alloc_Block(NULL, NULL, "grace-pkg", 23);
  gc_frame_newslot(block12);
  block_savedest(block12);
  Object closure13 = createclosure(1, "_apply");
setclosureframe(closure13, stackframe);
  addtoclosure(closure13, selfslot);
  struct UserObject *uo13 = (struct UserObject*)block12;
  uo13->data[0] = (Object)closure13;
  Method *meth_meth_grace_45_pkg__apply13 = addmethod2pos(block12, "_apply", &meth_grace_45_pkg__apply13, 0);
int argcv_meth_grace_45_pkg__apply13[] = {1};
meth_meth_grace_45_pkg__apply13->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply13);
  meth_meth_grace_45_pkg__apply13->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply13->definitionLine = 17;
// compilenode returning block12
  gc_frame_newslot(block12);
// Begin line 20
  setline(20);
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
// Begin line 26
  setline(26);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe18 = gc_frame_new();
// Begin line 23
  setline(23);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit19 == NULL) {
    strlit19 = alloc_String("list");
    gc_root(strlit19);
  }
// compilenode returning strlit19
  gc_frame_newslot(strlit19);
// Begin line 26
  setline(26);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block20 = alloc_Block(NULL, NULL, "grace-pkg", 26);
  gc_frame_newslot(block20);
  block_savedest(block20);
  Object closure21 = createclosure(1, "_apply");
setclosureframe(closure21, stackframe);
  addtoclosure(closure21, selfslot);
  struct UserObject *uo21 = (struct UserObject*)block20;
  uo21->data[0] = (Object)closure21;
  Method *meth_meth_grace_45_pkg__apply21 = addmethod2pos(block20, "_apply", &meth_grace_45_pkg__apply21, 0);
int argcv_meth_grace_45_pkg__apply21[] = {0};
meth_meth_grace_45_pkg__apply21->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply21);
  meth_meth_grace_45_pkg__apply21->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply21->definitionLine = 17;
// compilenode returning block20
  gc_frame_newslot(block20);
// Begin line 23
  setline(23);
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
// Begin line 29
  setline(29);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe26 = gc_frame_new();
// Begin line 26
  setline(26);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit27 == NULL) {
    strlit27 = alloc_String("bundle");
    gc_root(strlit27);
  }
// compilenode returning strlit27
  gc_frame_newslot(strlit27);
// Begin line 29
  setline(29);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block28 = alloc_Block(NULL, NULL, "grace-pkg", 29);
  gc_frame_newslot(block28);
  block_savedest(block28);
  Object closure29 = createclosure(1, "_apply");
setclosureframe(closure29, stackframe);
  addtoclosure(closure29, selfslot);
  struct UserObject *uo29 = (struct UserObject*)block28;
  uo29->data[0] = (Object)closure29;
  Method *meth_meth_grace_45_pkg__apply29 = addmethod2pos(block28, "_apply", &meth_grace_45_pkg__apply29, 0);
int argcv_meth_grace_45_pkg__apply29[] = {2};
meth_meth_grace_45_pkg__apply29->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply29);
  meth_meth_grace_45_pkg__apply29->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply29->definitionLine = 17;
// compilenode returning block28
  gc_frame_newslot(block28);
// Begin line 26
  setline(26);
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
// Begin line 32
  setline(32);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe34 = gc_frame_new();
// Begin line 29
  setline(29);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit35 == NULL) {
    strlit35 = alloc_String("setAddress");
    gc_root(strlit35);
  }
// compilenode returning strlit35
  gc_frame_newslot(strlit35);
// Begin line 32
  setline(32);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block36 = alloc_Block(NULL, NULL, "grace-pkg", 32);
  gc_frame_newslot(block36);
  block_savedest(block36);
  Object closure37 = createclosure(1, "_apply");
setclosureframe(closure37, stackframe);
  addtoclosure(closure37, selfslot);
  struct UserObject *uo37 = (struct UserObject*)block36;
  uo37->data[0] = (Object)closure37;
  Method *meth_meth_grace_45_pkg__apply37 = addmethod2pos(block36, "_apply", &meth_grace_45_pkg__apply37, 0);
int argcv_meth_grace_45_pkg__apply37[] = {2};
meth_meth_grace_45_pkg__apply37->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply37);
  meth_meth_grace_45_pkg__apply37->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply37->definitionLine = 17;
// compilenode returning block36
  gc_frame_newslot(block36);
// Begin line 29
  setline(29);
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
// Begin line 35
  setline(35);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe42 = gc_frame_new();
// Begin line 32
  setline(32);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit43 == NULL) {
    strlit43 = alloc_String("verbose");
    gc_root(strlit43);
  }
// compilenode returning strlit43
  gc_frame_newslot(strlit43);
// Begin line 35
  setline(35);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block44 = alloc_Block(NULL, NULL, "grace-pkg", 35);
  gc_frame_newslot(block44);
  block_savedest(block44);
  Object closure45 = createclosure(2, "_apply");
setclosureframe(closure45, stackframe);
  addtoclosure(closure45, var_verbose);
  addtoclosure(closure45, selfslot);
  struct UserObject *uo45 = (struct UserObject*)block44;
  uo45->data[0] = (Object)closure45;
  Method *meth_meth_grace_45_pkg__apply45 = addmethod2pos(block44, "_apply", &meth_grace_45_pkg__apply45, 0);
int argcv_meth_grace_45_pkg__apply45[] = {0};
meth_meth_grace_45_pkg__apply45->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply45);
  meth_meth_grace_45_pkg__apply45->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply45->definitionLine = 17;
// compilenode returning block44
  gc_frame_newslot(block44);
// Begin line 32
  setline(32);
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
// Begin line 38
  setline(38);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe50 = gc_frame_new();
// Begin line 35
  setline(35);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit51 == NULL) {
    strlit51 = alloc_String("global");
    gc_root(strlit51);
  }
// compilenode returning strlit51
  gc_frame_newslot(strlit51);
// Begin line 38
  setline(38);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block52 = alloc_Block(NULL, NULL, "grace-pkg", 38);
  gc_frame_newslot(block52);
  block_savedest(block52);
  Object closure53 = createclosure(2, "_apply");
setclosureframe(closure53, stackframe);
  addtoclosure(closure53, var_global);
  addtoclosure(closure53, selfslot);
  struct UserObject *uo53 = (struct UserObject*)block52;
  uo53->data[0] = (Object)closure53;
  Method *meth_meth_grace_45_pkg__apply53 = addmethod2pos(block52, "_apply", &meth_grace_45_pkg__apply53, 0);
int argcv_meth_grace_45_pkg__apply53[] = {0};
meth_meth_grace_45_pkg__apply53->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply53);
  meth_meth_grace_45_pkg__apply53->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply53->definitionLine = 17;
// compilenode returning block52
  gc_frame_newslot(block52);
// Begin line 35
  setline(35);
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
Object meth_grace_45_pkg_parseArgs0(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 17
  setline(17);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 16
  setline(16);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1 = alloc_Float64(0);
// compilenode returning num1
  *var_count = num1;
  if (num1 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 18
  setline(18);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 17
  setline(17);
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
// Begin line 39
  setline(39);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe5 = gc_frame_new();
// Begin line 18
  setline(18);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_args
  gc_frame_newslot(*var_args);
// Begin line 39
  setline(39);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block6 = alloc_Block(NULL, NULL, "grace-pkg", 39);
  gc_frame_newslot(block6);
  block_savedest(block6);
  Object closure7 = createclosure(3, "_apply");
setclosureframe(closure7, stackframe);
  addtoclosure(closure7, var_verbose);
  addtoclosure(closure7, var_global);
  addtoclosure(closure7, selfslot);
  struct UserObject *uo7 = (struct UserObject*)block6;
  uo7->data[0] = (Object)closure7;
  Method *meth_meth_grace_45_pkg__apply7 = addmethod2pos(block6, "_apply", &meth_grace_45_pkg__apply7, 0);
int argcv_meth_grace_45_pkg__apply7[] = {2};
meth_meth_grace_45_pkg__apply7->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply7);
  meth_meth_grace_45_pkg__apply7->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply7->definitionLine = 17;
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
Object meth_grace_45_pkg_option_40__41_do67(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 49
  setline(49);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe68 = gc_frame_new();
// Begin line 48
  setline(48);
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
// Begin line 58
  setline(58);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 49
  setline(49);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arg
  int op_slot_left_71 = gc_frame_newslot(*var_arg);
// compilenode returning *var_name
  int op_slot_right_71 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult73 = callmethod(*var_arg, "==", 1, partcv, params);
// compilenode returning opresult73
  Object if70 = done;
  if (istrue(opresult73)) {
// Begin line 54
  setline(54);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 50
  setline(50);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe76 = gc_frame_new();
// compilenode returning *var_args
  partcv[0] = 0;
  Object call77 = callmethod(*var_args, "size",
    1, partcv, params);
  gc_frame_end(callframe76);
// compilenode returning call77
// compilenode returning call77
  int op_slot_left_75 = gc_frame_newslot(call77);
// compilenode returning *var_i
  int op_slot_right_75 = gc_frame_newslot(*var_i);
  params[0] = *var_i;
  partcv[0] = 1;
  Object opresult79 = callmethod(call77, "==", 1, partcv, params);
// compilenode returning opresult79
  Object if74 = done;
  if (istrue(opresult79)) {
// Begin line 52
  setline(52);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe80 = gc_frame_new();
// Begin line 51
  setline(51);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit83 == NULL) {
    strlit83 = alloc_String("Missing value for option ");
    gc_root(strlit83);
  }
// compilenode returning strlit83
  int op_slot_left_82 = gc_frame_newslot(strlit83);
// compilenode returning *var_name
  int op_slot_right_82 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult85 = callmethod(strlit83, "++", 1, partcv, params);
// compilenode returning opresult85
  int op_slot_left_81 = gc_frame_newslot(opresult85);
  if (strlit86 == NULL) {
    strlit86 = alloc_String("");
    gc_root(strlit86);
  }
// compilenode returning strlit86
  int op_slot_right_81 = gc_frame_newslot(strlit86);
  params[0] = strlit86;
  partcv[0] = 1;
  Object opresult88 = callmethod(opresult85, "++", 1, partcv, params);
// compilenode returning opresult88
  gc_frame_newslot(opresult88);
  int callframe89 = gc_frame_new();
  partcv[0] = 0;
  Object call90 = callmethodflags(prelude, "Exception", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe89);
// compilenode returning call90
  params[0] = opresult88;
  partcv[0] = 1;
  Object call91 = callmethod(call90, "raise",
    1, partcv, params);
  gc_frame_end(callframe80);
// compilenode returning call91
    gc_frame_newslot(call91);
    if74 = call91;
  } else {
  }
// compilenode returning if74
// Begin line 55
  setline(55);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 54
  setline(54);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_92 = gc_frame_newslot(*var_i);
  Object num93 = alloc_Float64(1);
// compilenode returning num93
  int op_slot_right_92 = gc_frame_newslot(num93);
  params[0] = num93;
  partcv[0] = 1;
  Object sum95 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum95
  *var_i = sum95;
  if (sum95 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 56
  setline(56);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe97 = gc_frame_new();
// Begin line 55
  setline(55);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe98 = gc_frame_new();
// compilenode returning *var_i
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call99 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe98);
// compilenode returning call99
  gc_frame_newslot(call99);
// compilenode returning *var_block_39_
  params[0] = call99;
  partcv[0] = 1;
  Object call100 = callmethod(*var_block_39_, "apply",
    1, partcv, params);
  gc_frame_end(callframe97);
// compilenode returning call100
// Begin line 57
  setline(57);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 56
  setline(56);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool101 = alloc_Boolean(1);
// compilenode returning bool101
  *var_ran = bool101;
  if (bool101 == undefined)
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
Object meth_grace_45_pkg_doubleValue_40__41_do103(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 62
  setline(62);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe104 = gc_frame_new();
// Begin line 61
  setline(61);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call105 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe104);
// compilenode returning call105
  *var_arg = call105;
  if (call105 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 73
  setline(73);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 62
  setline(62);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arg
  int op_slot_left_107 = gc_frame_newslot(*var_arg);
// compilenode returning *var_name
  int op_slot_right_107 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult109 = callmethod(*var_arg, "==", 1, partcv, params);
// compilenode returning opresult109
  Object if106 = done;
  if (istrue(opresult109)) {
// Begin line 66
  setline(66);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 63
  setline(63);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe112 = gc_frame_new();
// compilenode returning *var_args
  partcv[0] = 0;
  Object call113 = callmethod(*var_args, "size",
    1, partcv, params);
  gc_frame_end(callframe112);
// compilenode returning call113
// compilenode returning call113
  int op_slot_left_111 = gc_frame_newslot(call113);
// compilenode returning *var_i
  int op_slot_left_114 = gc_frame_newslot(*var_i);
  Object num115 = alloc_Float64(1);
// compilenode returning num115
  int op_slot_right_114 = gc_frame_newslot(num115);
  params[0] = num115;
  partcv[0] = 1;
  Object sum117 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum117
  int op_slot_right_111 = gc_frame_newslot(sum117);
  params[0] = sum117;
  partcv[0] = 1;
  Object opresult119 = callmethod(call113, "<", 1, partcv, params);
// compilenode returning opresult119
  Object if110 = done;
  if (istrue(opresult119)) {
// Begin line 65
  setline(65);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe120 = gc_frame_new();
// Begin line 64
  setline(64);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit123 == NULL) {
    strlit123 = alloc_String("Missing values for option ");
    gc_root(strlit123);
  }
// compilenode returning strlit123
  int op_slot_left_122 = gc_frame_newslot(strlit123);
// compilenode returning *var_name
  int op_slot_right_122 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult125 = callmethod(strlit123, "++", 1, partcv, params);
// compilenode returning opresult125
  int op_slot_left_121 = gc_frame_newslot(opresult125);
  if (strlit126 == NULL) {
    strlit126 = alloc_String("");
    gc_root(strlit126);
  }
// compilenode returning strlit126
  int op_slot_right_121 = gc_frame_newslot(strlit126);
  params[0] = strlit126;
  partcv[0] = 1;
  Object opresult128 = callmethod(opresult125, "++", 1, partcv, params);
// compilenode returning opresult128
  gc_frame_newslot(opresult128);
  int callframe129 = gc_frame_new();
  partcv[0] = 0;
  Object call130 = callmethodflags(prelude, "Exception", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe129);
// compilenode returning call130
  params[0] = opresult128;
  partcv[0] = 1;
  Object call131 = callmethod(call130, "raise",
    1, partcv, params);
  gc_frame_end(callframe120);
// compilenode returning call131
    gc_frame_newslot(call131);
    if110 = call131;
  } else {
  }
// compilenode returning if110
// Begin line 67
  setline(67);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe132 = gc_frame_new();
// Begin line 66
  setline(66);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_133 = gc_frame_newslot(*var_i);
  Object num134 = alloc_Float64(1);
// compilenode returning num134
  int op_slot_right_133 = gc_frame_newslot(num134);
  params[0] = num134;
  partcv[0] = 1;
  Object sum136 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum136
  gc_frame_newslot(sum136);
// compilenode returning *var_args
  params[0] = sum136;
  partcv[0] = 1;
  Object call137 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe132);
// compilenode returning call137
  *var_arga = call137;
  if (call137 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 68
  setline(68);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe138 = gc_frame_new();
// Begin line 67
  setline(67);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_139 = gc_frame_newslot(*var_i);
  Object num140 = alloc_Float64(2);
// compilenode returning num140
  int op_slot_right_139 = gc_frame_newslot(num140);
  params[0] = num140;
  partcv[0] = 1;
  Object sum142 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum142
  gc_frame_newslot(sum142);
// compilenode returning *var_args
  params[0] = sum142;
  partcv[0] = 1;
  Object call143 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe138);
// compilenode returning call143
  *var_argb = call143;
  if (call143 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 69
  setline(69);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe144 = gc_frame_new();
// Begin line 68
  setline(68);
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
  Object call145 = callmethod(*var_block_39_, "apply",
    1, partcv, params);
  gc_frame_end(callframe144);
// compilenode returning call145
// Begin line 70
  setline(70);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 69
  setline(69);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_146 = gc_frame_newslot(*var_i);
  Object num147 = alloc_Float64(2);
// compilenode returning num147
  int op_slot_right_146 = gc_frame_newslot(num147);
  params[0] = num147;
  partcv[0] = 1;
  Object sum149 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum149
  *var_i = sum149;
  if (sum149 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 71
  setline(71);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 70
  setline(70);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool151 = alloc_Boolean(1);
// compilenode returning bool151
  *var_ran = bool151;
  if (bool151 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if106 = done;
  } else {
  }
// compilenode returning if106
  gc_frame_end(frame);
  return if106;
}
Object meth_grace_45_pkg_flag_40__41_do153(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 77
  setline(77);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe154 = gc_frame_new();
// Begin line 76
  setline(76);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call155 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe154);
// compilenode returning call155
  *var_arg = call155;
  if (call155 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 81
  setline(81);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 77
  setline(77);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arg
  int op_slot_left_157 = gc_frame_newslot(*var_arg);
// compilenode returning *var_name
  int op_slot_right_157 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult159 = callmethod(*var_arg, "==", 1, partcv, params);
// compilenode returning opresult159
  Object if156 = done;
  if (istrue(opresult159)) {
// Begin line 78
  setline(78);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe160 = gc_frame_new();
// compilenode returning *var_block_39_
  partcv[0] = 0;
  Object call161 = callmethod(*var_block_39_, "apply",
    1, partcv, params);
  gc_frame_end(callframe160);
// compilenode returning call161
// compilenode returning call161
// Begin line 80
  setline(80);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 79
  setline(79);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool162 = alloc_Boolean(1);
// compilenode returning bool162
  *var_ran = bool162;
  if (bool162 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if156 = done;
  } else {
  }
// compilenode returning if156
  gc_frame_end(frame);
  return if156;
}
Object meth_grace_45_pkg__apply166(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_167 = gc_frame_newslot(*var_i);
// compilenode returning *var_size
  int op_slot_right_167 = gc_frame_newslot(*var_size);
  params[0] = *var_size;
  partcv[0] = 1;
  Object opresult169 = callmethod(*var_i, "<=", 1, partcv, params);
// compilenode returning opresult169
  gc_frame_end(frame);
  return opresult169;
}
Object meth_grace_45_pkg__apply172(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 85
  setline(85);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe173 = gc_frame_new();
// Begin line 84
  setline(84);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call174 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe173);
// compilenode returning call174
  *var_arg = call174;
  if (call174 == undefined)
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
// compilenode returning *var_arg
  params[0] = *var_arg;
  Object call175 = gracelib_print(NULL, 1,  params);
// compilenode returning call175
// Begin line 87
  setline(87);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 86
  setline(86);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool176 = alloc_Boolean(0);
// compilenode returning bool176
  *var_ran = bool176;
  if (bool176 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 88
  setline(88);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe178 = gc_frame_new();
// Begin line 87
  setline(87);
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
  Object call179 = callmethod(*var_block, "apply",
    1, partcv, params);
  gc_frame_end(callframe178);
// compilenode returning call179
// Begin line 92
  setline(92);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 88
  setline(88);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe183 = gc_frame_new();
  Object num184 = alloc_Float64(1);
// compilenode returning num184
  gc_frame_newslot(num184);
// compilenode returning *var_arg
  params[0] = num184;
  partcv[0] = 1;
  Object call185 = callmethod(*var_arg, "at",
    1, partcv, params);
  gc_frame_end(callframe183);
// compilenode returning call185
  int op_slot_left_182 = gc_frame_newslot(call185);
  if (strlit186 == NULL) {
    strlit186 = alloc_String("-");
    gc_root(strlit186);
  }
// compilenode returning strlit186
  int op_slot_right_182 = gc_frame_newslot(strlit186);
  params[0] = strlit186;
  partcv[0] = 1;
  Object opresult188 = callmethod(call185, "==", 1, partcv, params);
// compilenode returning opresult188
  int op_slot_left_181 = gc_frame_newslot(opresult188);
  int callframe189 = gc_frame_new();
// compilenode returning *var_ran
  partcv[0] = 0;
  Object call190 = callmethod(*var_ran, "not",
    1, partcv, params);
  gc_frame_end(callframe189);
// compilenode returning call190
// compilenode returning call190
  int op_slot_right_181 = gc_frame_newslot(call190);
  params[0] = call190;
  partcv[0] = 1;
  Object opresult192 = callmethod(opresult188, "&&", 1, partcv, params);
// compilenode returning opresult192
  Object if180 = done;
  if (istrue(opresult192)) {
// Begin line 90
  setline(90);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe193 = gc_frame_new();
// Begin line 89
  setline(89);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit196 == NULL) {
    strlit196 = alloc_String("Unrecognised argument ");
    gc_root(strlit196);
  }
// compilenode returning strlit196
  int op_slot_left_195 = gc_frame_newslot(strlit196);
// compilenode returning *var_arg
  int op_slot_right_195 = gc_frame_newslot(*var_arg);
  params[0] = *var_arg;
  partcv[0] = 1;
  Object opresult198 = callmethod(strlit196, "++", 1, partcv, params);
// compilenode returning opresult198
  int op_slot_left_194 = gc_frame_newslot(opresult198);
  if (strlit199 == NULL) {
    strlit199 = alloc_String("");
    gc_root(strlit199);
  }
// compilenode returning strlit199
  int op_slot_right_194 = gc_frame_newslot(strlit199);
  params[0] = strlit199;
  partcv[0] = 1;
  Object opresult201 = callmethod(opresult198, "++", 1, partcv, params);
// compilenode returning opresult201
  gc_frame_newslot(opresult201);
  int callframe202 = gc_frame_new();
  partcv[0] = 0;
  Object call203 = callmethodflags(prelude, "Exception", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe202);
// compilenode returning call203
  params[0] = opresult201;
  partcv[0] = 1;
  Object call204 = callmethod(call203, "raise",
    1, partcv, params);
  gc_frame_end(callframe193);
// compilenode returning call204
    gc_frame_newslot(call204);
    if180 = call204;
  } else {
  }
// compilenode returning if180
// Begin line 93
  setline(93);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 92
  setline(92);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_205 = gc_frame_newslot(*var_i);
  Object num206 = alloc_Float64(1);
// compilenode returning num206
  int op_slot_right_205 = gc_frame_newslot(num206);
  params[0] = num206;
  partcv[0] = 1;
  Object sum208 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum208
  *var_i = sum208;
  if (sum208 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_grace_45_pkg_forArgs_40__41_do60(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 43
  setline(43);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 42
  setline(42);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num61 = alloc_Float64(2);
// compilenode returning num61
  *var_i = num61;
  if (num61 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 44
  setline(44);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 43
  setline(43);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool62 = alloc_Boolean(0);
// compilenode returning bool62
  *var_ran = bool62;
  if (bool62 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 46
  setline(46);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 44
  setline(44);
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
// Begin line 46
  setline(46);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object inheritingObject65 = inheritingObject;
  if (isTailObject) {
    isTailObject = 0;
    inheritingObject = NULL;
  }
  Object obj65 = alloc_userobj2(3,4, objclass65);
  gc_frame_newslot(obj65);
// OBJECT OUTER DEC outer
  adddatum2(obj65, self, 0);
  addmethodreal(obj65,"outer", &reader_grace_45_pkg_outer_66);
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
  Object closure67 = createclosure(3, "option()do");
setclosureframe(closure67, stackframe);
  addtoclosure(closure67, var_i);
  addtoclosure(closure67, var_args);
  addtoclosure(closure67, var_ran);
  struct UserObject *uo67 = (struct UserObject*)obj65;
  uo67->data[1] = (Object)closure67;
  Method *meth_meth_grace_45_pkg_option_40__41_do67 = addmethod2pos(obj65, "option()do", &meth_grace_45_pkg_option_40__41_do67, 1);
int argcv_meth_grace_45_pkg_option_40__41_do67[] = {1, 1};
meth_meth_grace_45_pkg_option_40__41_do67->type = alloc_MethodType(2, argcv_meth_grace_45_pkg_option_40__41_do67);
meth_meth_grace_45_pkg_option_40__41_do67->type->types[0] = type_String;
meth_meth_grace_45_pkg_option_40__41_do67->type->names[0] = "name";
  meth_meth_grace_45_pkg_option_40__41_do67->definitionModule = modulename;
  meth_meth_grace_45_pkg_option_40__41_do67->definitionLine = 47;
  block_savedest(obj65);
  Object closure103 = createclosure(3, "doubleValue()do");
setclosureframe(closure103, stackframe);
  addtoclosure(closure103, var_i);
  addtoclosure(closure103, var_args);
  addtoclosure(closure103, var_ran);
  struct UserObject *uo103 = (struct UserObject*)obj65;
  uo103->data[2] = (Object)closure103;
  Method *meth_meth_grace_45_pkg_doubleValue_40__41_do103 = addmethod2pos(obj65, "doubleValue()do", &meth_grace_45_pkg_doubleValue_40__41_do103, 2);
int argcv_meth_grace_45_pkg_doubleValue_40__41_do103[] = {1, 1};
meth_meth_grace_45_pkg_doubleValue_40__41_do103->type = alloc_MethodType(2, argcv_meth_grace_45_pkg_doubleValue_40__41_do103);
meth_meth_grace_45_pkg_doubleValue_40__41_do103->type->types[0] = type_String;
meth_meth_grace_45_pkg_doubleValue_40__41_do103->type->names[0] = "name";
  meth_meth_grace_45_pkg_doubleValue_40__41_do103->definitionModule = modulename;
  meth_meth_grace_45_pkg_doubleValue_40__41_do103->definitionLine = 60;
  block_savedest(obj65);
  Object closure153 = createclosure(3, "flag()do");
setclosureframe(closure153, stackframe);
  addtoclosure(closure153, var_i);
  addtoclosure(closure153, var_args);
  addtoclosure(closure153, var_ran);
  struct UserObject *uo153 = (struct UserObject*)obj65;
  uo153->data[3] = (Object)closure153;
  Method *meth_meth_grace_45_pkg_flag_40__41_do153 = addmethod2pos(obj65, "flag()do", &meth_grace_45_pkg_flag_40__41_do153, 3);
int argcv_meth_grace_45_pkg_flag_40__41_do153[] = {1, 1};
meth_meth_grace_45_pkg_flag_40__41_do153->type = alloc_MethodType(2, argcv_meth_grace_45_pkg_flag_40__41_do153);
meth_meth_grace_45_pkg_flag_40__41_do153->type->types[0] = type_String;
meth_meth_grace_45_pkg_flag_40__41_do153->type->names[0] = "name";
  meth_meth_grace_45_pkg_flag_40__41_do153->definitionModule = modulename;
  meth_meth_grace_45_pkg_flag_40__41_do153->definitionLine = 75;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
objclass65 = obj65->class;
  objclass65->definitionModule = modulename;
  objclass65->definitionLine = 46;
  (*(struct UserObject *)self).data[0] = lowerouter65;
  self = oldself65;
  selfslot = oldselfslot65;
  stackframe = oldstackframe65;
// compilenode returning obj65
  *var_on = obj65;
  if (obj65 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 94
  setline(94);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe164 = gc_frame_new();
// Begin line 83
  setline(83);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block165 = alloc_Block(NULL, NULL, "grace-pkg", 83);
  gc_frame_newslot(block165);
  block_savedest(block165);
  Object closure166 = createclosure(3, "_apply");
setclosureframe(closure166, stackframe);
  addtoclosure(closure166, var_i);
  addtoclosure(closure166, var_size);
  addtoclosure(closure166, selfslot);
  struct UserObject *uo166 = (struct UserObject*)block165;
  uo166->data[0] = (Object)closure166;
  Method *meth_meth_grace_45_pkg__apply166 = addmethod2pos(block165, "_apply", &meth_grace_45_pkg__apply166, 0);
int argcv_meth_grace_45_pkg__apply166[] = {0};
meth_meth_grace_45_pkg__apply166->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply166);
  meth_meth_grace_45_pkg__apply166->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply166->definitionLine = 78;
// compilenode returning block165
  gc_frame_newslot(block165);
// Begin line 94
  setline(94);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block171 = alloc_Block(NULL, NULL, "grace-pkg", 94);
  gc_frame_newslot(block171);
  block_savedest(block171);
  Object closure172 = createclosure(6, "_apply");
setclosureframe(closure172, stackframe);
  addtoclosure(closure172, var_i);
  addtoclosure(closure172, var_args);
  addtoclosure(closure172, var_ran);
  addtoclosure(closure172, var_on);
  addtoclosure(closure172, var_block);
  addtoclosure(closure172, selfslot);
  struct UserObject *uo172 = (struct UserObject*)block171;
  uo172->data[0] = (Object)closure172;
  Method *meth_meth_grace_45_pkg__apply172 = addmethod2pos(block171, "_apply", &meth_grace_45_pkg__apply172, 0);
int argcv_meth_grace_45_pkg__apply172[] = {0};
meth_meth_grace_45_pkg__apply172->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply172);
  meth_meth_grace_45_pkg__apply172->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply172->definitionLine = 78;
// compilenode returning block171
  gc_frame_newslot(block171);
  params[0] = block165;
  params[1] = block171;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call211 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe164);
// compilenode returning call211
  gc_frame_end(frame);
  return call211;
}
Object meth_grace_45_pkg_forArgs_40__41_do60_object(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 43
  setline(43);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 42
  setline(42);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num61 = alloc_Float64(2);
// compilenode returning num61
  *var_i = num61;
  if (num61 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 44
  setline(44);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 43
  setline(43);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool62 = alloc_Boolean(0);
// compilenode returning bool62
  *var_ran = bool62;
  if (bool62 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 46
  setline(46);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 44
  setline(44);
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
// Begin line 46
  setline(46);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object inheritingObject65 = inheritingObject;
  if (isTailObject) {
    isTailObject = 0;
    inheritingObject = NULL;
  }
  Object obj65 = alloc_userobj2(3,4, objclass65);
  gc_frame_newslot(obj65);
// OBJECT OUTER DEC outer
  adddatum2(obj65, self, 0);
  addmethodreal(obj65,"outer", &reader_grace_45_pkg_outer_66);
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
  Object closure67 = createclosure(3, "option()do");
setclosureframe(closure67, stackframe);
  addtoclosure(closure67, var_i);
  addtoclosure(closure67, var_args);
  addtoclosure(closure67, var_ran);
  struct UserObject *uo67 = (struct UserObject*)obj65;
  uo67->data[1] = (Object)closure67;
  Method *meth_meth_grace_45_pkg_option_40__41_do67 = addmethod2pos(obj65, "option()do", &meth_grace_45_pkg_option_40__41_do67, 1);
int argcv_meth_grace_45_pkg_option_40__41_do67[] = {1, 1};
meth_meth_grace_45_pkg_option_40__41_do67->type = alloc_MethodType(2, argcv_meth_grace_45_pkg_option_40__41_do67);
meth_meth_grace_45_pkg_option_40__41_do67->type->types[0] = type_String;
meth_meth_grace_45_pkg_option_40__41_do67->type->names[0] = "name";
  meth_meth_grace_45_pkg_option_40__41_do67->definitionModule = modulename;
  meth_meth_grace_45_pkg_option_40__41_do67->definitionLine = 47;
  block_savedest(obj65);
  Object closure103 = createclosure(3, "doubleValue()do");
setclosureframe(closure103, stackframe);
  addtoclosure(closure103, var_i);
  addtoclosure(closure103, var_args);
  addtoclosure(closure103, var_ran);
  struct UserObject *uo103 = (struct UserObject*)obj65;
  uo103->data[2] = (Object)closure103;
  Method *meth_meth_grace_45_pkg_doubleValue_40__41_do103 = addmethod2pos(obj65, "doubleValue()do", &meth_grace_45_pkg_doubleValue_40__41_do103, 2);
int argcv_meth_grace_45_pkg_doubleValue_40__41_do103[] = {1, 1};
meth_meth_grace_45_pkg_doubleValue_40__41_do103->type = alloc_MethodType(2, argcv_meth_grace_45_pkg_doubleValue_40__41_do103);
meth_meth_grace_45_pkg_doubleValue_40__41_do103->type->types[0] = type_String;
meth_meth_grace_45_pkg_doubleValue_40__41_do103->type->names[0] = "name";
  meth_meth_grace_45_pkg_doubleValue_40__41_do103->definitionModule = modulename;
  meth_meth_grace_45_pkg_doubleValue_40__41_do103->definitionLine = 60;
  block_savedest(obj65);
  Object closure153 = createclosure(3, "flag()do");
setclosureframe(closure153, stackframe);
  addtoclosure(closure153, var_i);
  addtoclosure(closure153, var_args);
  addtoclosure(closure153, var_ran);
  struct UserObject *uo153 = (struct UserObject*)obj65;
  uo153->data[3] = (Object)closure153;
  Method *meth_meth_grace_45_pkg_flag_40__41_do153 = addmethod2pos(obj65, "flag()do", &meth_grace_45_pkg_flag_40__41_do153, 3);
int argcv_meth_grace_45_pkg_flag_40__41_do153[] = {1, 1};
meth_meth_grace_45_pkg_flag_40__41_do153->type = alloc_MethodType(2, argcv_meth_grace_45_pkg_flag_40__41_do153);
meth_meth_grace_45_pkg_flag_40__41_do153->type->types[0] = type_String;
meth_meth_grace_45_pkg_flag_40__41_do153->type->names[0] = "name";
  meth_meth_grace_45_pkg_flag_40__41_do153->definitionModule = modulename;
  meth_meth_grace_45_pkg_flag_40__41_do153->definitionLine = 75;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
objclass65 = obj65->class;
  objclass65->definitionModule = modulename;
  objclass65->definitionLine = 46;
  (*(struct UserObject *)self).data[0] = lowerouter65;
  self = oldself65;
  selfslot = oldselfslot65;
  stackframe = oldstackframe65;
// compilenode returning obj65
  *var_on = obj65;
  if (obj65 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 94
  setline(94);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe164 = gc_frame_new();
// Begin line 83
  setline(83);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block165 = alloc_Block(NULL, NULL, "grace-pkg", 83);
  gc_frame_newslot(block165);
  block_savedest(block165);
  Object closure166 = createclosure(3, "_apply");
setclosureframe(closure166, stackframe);
  addtoclosure(closure166, var_i);
  addtoclosure(closure166, var_size);
  addtoclosure(closure166, selfslot);
  struct UserObject *uo166 = (struct UserObject*)block165;
  uo166->data[0] = (Object)closure166;
  Method *meth_meth_grace_45_pkg__apply166 = addmethod2pos(block165, "_apply", &meth_grace_45_pkg__apply166, 0);
int argcv_meth_grace_45_pkg__apply166[] = {0};
meth_meth_grace_45_pkg__apply166->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply166);
  meth_meth_grace_45_pkg__apply166->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply166->definitionLine = 78;
// compilenode returning block165
  gc_frame_newslot(block165);
// Begin line 94
  setline(94);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block171 = alloc_Block(NULL, NULL, "grace-pkg", 94);
  gc_frame_newslot(block171);
  block_savedest(block171);
  Object closure172 = createclosure(6, "_apply");
setclosureframe(closure172, stackframe);
  addtoclosure(closure172, var_i);
  addtoclosure(closure172, var_args);
  addtoclosure(closure172, var_ran);
  addtoclosure(closure172, var_on);
  addtoclosure(closure172, var_block);
  addtoclosure(closure172, selfslot);
  struct UserObject *uo172 = (struct UserObject*)block171;
  uo172->data[0] = (Object)closure172;
  Method *meth_meth_grace_45_pkg__apply172 = addmethod2pos(block171, "_apply", &meth_grace_45_pkg__apply172, 0);
int argcv_meth_grace_45_pkg__apply172[] = {0};
meth_meth_grace_45_pkg__apply172->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply172);
  meth_meth_grace_45_pkg__apply172->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply172->definitionLine = 78;
// compilenode returning block171
  gc_frame_newslot(block171);
  params[0] = block165;
  params[1] = block171;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call211 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe164);
// compilenode returning call211
  gc_frame_end(frame);
  return call211;
}
Object meth_grace_45_pkg_listInstalled213(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 98
  setline(98);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 97
  setline(97);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit214 == NULL) {
    strlit214 = alloc_String("Installed packages:");
    gc_root(strlit214);
  }
// compilenode returning strlit214
  params[0] = strlit214;
  Object call215 = gracelib_print(NULL, 1,  params);
// compilenode returning call215
// Begin line 99
  setline(99);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe216 = gc_frame_new();
// Begin line 98
  setline(98);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit217 == NULL) {
    strlit217 = alloc_String("usr/lib/grace/packages/");
    gc_root(strlit217);
  }
// compilenode returning strlit217
  gc_frame_newslot(strlit217);
  params[0] = strlit217;
  partcv[0] = 1;
  Object call218 = callmethodflags(self, "checkListPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe216);
// compilenode returning call218
// Begin line 100
  setline(100);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe219 = gc_frame_new();
// Begin line 99
  setline(99);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit220 == NULL) {
    strlit220 = alloc_String("usr/lib/grace/packages/");
    gc_root(strlit220);
  }
// compilenode returning strlit220
  gc_frame_newslot(strlit220);
  params[0] = strlit220;
  partcv[0] = 1;
  Object call221 = callmethodflags(self, "checkListPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe219);
// compilenode returning call221
// Begin line 101
  setline(101);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe222 = gc_frame_new();
// Begin line 100
  setline(100);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe224 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call225 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe224);
// compilenode returning call225
// compilenode returning call225
  if (strlit226 == NULL) {
    strlit226 = alloc_String("HOME");
    gc_root(strlit226);
  }
// compilenode returning strlit226
  params[0] = strlit226;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres227 = callmethod(call225, "[]", 1, partcv, params);
// compilenode returning idxres227
  int op_slot_left_223 = gc_frame_newslot(idxres227);
  if (strlit228 == NULL) {
    strlit228 = alloc_String("/grace/packages/");
    gc_root(strlit228);
  }
// compilenode returning strlit228
  int op_slot_right_223 = gc_frame_newslot(strlit228);
  params[0] = strlit228;
  partcv[0] = 1;
  Object opresult230 = callmethod(idxres227, "++", 1, partcv, params);
// compilenode returning opresult230
  gc_frame_newslot(opresult230);
  params[0] = opresult230;
  partcv[0] = 1;
  Object call231 = callmethodflags(self, "checkListPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe222);
// compilenode returning call231
// Begin line 102
  setline(102);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe232 = gc_frame_new();
// Begin line 101
  setline(101);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe234 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call235 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe234);
// compilenode returning call235
// compilenode returning call235
  if (strlit236 == NULL) {
    strlit236 = alloc_String("HOME");
    gc_root(strlit236);
  }
// compilenode returning strlit236
  params[0] = strlit236;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres237 = callmethod(call235, "[]", 1, partcv, params);
// compilenode returning idxres237
  int op_slot_left_233 = gc_frame_newslot(idxres237);
  if (strlit238 == NULL) {
    strlit238 = alloc_String("/.local/lib/grace/modules/");
    gc_root(strlit238);
  }
// compilenode returning strlit238
  int op_slot_right_233 = gc_frame_newslot(strlit238);
  params[0] = strlit238;
  partcv[0] = 1;
  Object opresult240 = callmethod(idxres237, "++", 1, partcv, params);
// compilenode returning opresult240
  gc_frame_newslot(opresult240);
  params[0] = opresult240;
  partcv[0] = 1;
  Object call241 = callmethodflags(self, "checkListPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe232);
// compilenode returning call241
  gc_frame_end(frame);
  return call241;
}
Object meth_grace_45_pkg_checkListPath242(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 110
  setline(110);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 106
  setline(106);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe244 = gc_frame_new();
// compilenode returning *var_path
  gc_frame_newslot(*var_path);
// compilenode returning *var_io
  params[0] = *var_path;
  partcv[0] = 1;
  Object call245 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe244);
// compilenode returning call245
  Object if243 = done;
  if (istrue(call245)) {
// Begin line 108
  setline(108);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 107
  setline(107);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit248 == NULL) {
    strlit248 = alloc_String(" Installed in path: ");
    gc_root(strlit248);
  }
// compilenode returning strlit248
  int op_slot_left_247 = gc_frame_newslot(strlit248);
// compilenode returning *var_path
  int op_slot_right_247 = gc_frame_newslot(*var_path);
  params[0] = *var_path;
  partcv[0] = 1;
  Object opresult250 = callmethod(strlit248, "++", 1, partcv, params);
// compilenode returning opresult250
  int op_slot_left_246 = gc_frame_newslot(opresult250);
  if (strlit251 == NULL) {
    strlit251 = alloc_String("");
    gc_root(strlit251);
  }
// compilenode returning strlit251
  int op_slot_right_246 = gc_frame_newslot(strlit251);
  params[0] = strlit251;
  partcv[0] = 1;
  Object opresult253 = callmethod(opresult250, "++", 1, partcv, params);
// compilenode returning opresult253
  params[0] = opresult253;
  Object call254 = gracelib_print(NULL, 1,  params);
// compilenode returning call254
// Begin line 109
  setline(109);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe255 = gc_frame_new();
// Begin line 108
  setline(108);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_path
  gc_frame_newslot(*var_path);
  if (strlit256 == NULL) {
    strlit256 = alloc_String(" ");
    gc_root(strlit256);
  }
// compilenode returning strlit256
  gc_frame_newslot(strlit256);
  params[0] = *var_path;
  params[1] = strlit256;
  partcv[0] = 2;
  Object call257 = callmethodflags(self, "recurseDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe255);
// compilenode returning call257
    gc_frame_newslot(call257);
    if243 = call257;
  } else {
  }
// compilenode returning if243
  gc_frame_end(frame);
  return if243;
}
Object meth_grace_45_pkg__apply263(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 121
  setline(121);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 115
  setline(115);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_p
  int op_slot_left_266 = gc_frame_newslot(*var_p);
  if (strlit267 == NULL) {
    strlit267 = alloc_String("..");
    gc_root(strlit267);
  }
// compilenode returning strlit267
  int op_slot_right_266 = gc_frame_newslot(strlit267);
  params[0] = strlit267;
  partcv[0] = 1;
  Object opresult269 = callmethod(*var_p, "!=", 1, partcv, params);
// compilenode returning opresult269
  int op_slot_left_265 = gc_frame_newslot(opresult269);
// compilenode returning *var_p
  int op_slot_left_270 = gc_frame_newslot(*var_p);
  if (strlit271 == NULL) {
    strlit271 = alloc_String(".");
    gc_root(strlit271);
  }
// compilenode returning strlit271
  int op_slot_right_270 = gc_frame_newslot(strlit271);
  params[0] = strlit271;
  partcv[0] = 1;
  Object opresult273 = callmethod(*var_p, "!=", 1, partcv, params);
// compilenode returning opresult273
  int op_slot_right_265 = gc_frame_newslot(opresult273);
  params[0] = opresult273;
  partcv[0] = 1;
  Object opresult275 = callmethod(opresult269, "&&", 1, partcv, params);
// compilenode returning opresult275
  Object if264 = done;
  if (istrue(opresult275)) {
// Begin line 117
  setline(117);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 116
  setline(116);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_padding
  int op_slot_left_276 = gc_frame_newslot(*var_padding);
// compilenode returning *var_p
  int op_slot_right_276 = gc_frame_newslot(*var_p);
  params[0] = *var_p;
  partcv[0] = 1;
  Object opresult278 = callmethod(*var_padding, "++", 1, partcv, params);
// compilenode returning opresult278
  params[0] = opresult278;
  Object call279 = gracelib_print(NULL, 1,  params);
// compilenode returning call279
// Begin line 120
  setline(120);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 117
  setline(117);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe282 = gc_frame_new();
  int callframe283 = gc_frame_new();
// compilenode returning *var_path
  int op_slot_left_285 = gc_frame_newslot(*var_path);
  if (strlit286 == NULL) {
    strlit286 = alloc_String("/");
    gc_root(strlit286);
  }
// compilenode returning strlit286
  int op_slot_right_285 = gc_frame_newslot(strlit286);
  params[0] = strlit286;
  partcv[0] = 1;
  Object opresult288 = callmethod(*var_path, "++", 1, partcv, params);
// compilenode returning opresult288
  int op_slot_left_284 = gc_frame_newslot(opresult288);
// compilenode returning *var_p
  int op_slot_right_284 = gc_frame_newslot(*var_p);
  params[0] = *var_p;
  partcv[0] = 1;
  Object opresult290 = callmethod(opresult288, "++", 1, partcv, params);
// compilenode returning opresult290
  gc_frame_newslot(opresult290);
// compilenode returning *var_io
  params[0] = opresult290;
  partcv[0] = 1;
  Object call291 = callmethod(*var_io, "getdir",
    1, partcv, params);
  gc_frame_end(callframe283);
// compilenode returning call291
  partcv[0] = 0;
  Object call292 = callmethod(call291, "size",
    1, partcv, params);
  gc_frame_end(callframe282);
// compilenode returning call292
// compilenode returning call292
  int op_slot_left_281 = gc_frame_newslot(call292);
  Object num293 = alloc_Float64(0);
// compilenode returning num293
  int op_slot_right_281 = gc_frame_newslot(num293);
  params[0] = num293;
  partcv[0] = 1;
  Object opresult295 = callmethod(call292, ">", 1, partcv, params);
// compilenode returning opresult295
  Object if280 = done;
  if (istrue(opresult295)) {
// Begin line 119
  setline(119);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe296 = gc_frame_new();
// Begin line 118
  setline(118);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_path
  int op_slot_left_298 = gc_frame_newslot(*var_path);
  if (strlit299 == NULL) {
    strlit299 = alloc_String("/");
    gc_root(strlit299);
  }
// compilenode returning strlit299
  int op_slot_right_298 = gc_frame_newslot(strlit299);
  params[0] = strlit299;
  partcv[0] = 1;
  Object opresult301 = callmethod(*var_path, "++", 1, partcv, params);
// compilenode returning opresult301
  int op_slot_left_297 = gc_frame_newslot(opresult301);
// compilenode returning *var_p
  int op_slot_right_297 = gc_frame_newslot(*var_p);
  params[0] = *var_p;
  partcv[0] = 1;
  Object opresult303 = callmethod(opresult301, "++", 1, partcv, params);
// compilenode returning opresult303
  gc_frame_newslot(opresult303);
// compilenode returning *var_padding
  int op_slot_left_304 = gc_frame_newslot(*var_padding);
  if (strlit305 == NULL) {
    strlit305 = alloc_String("   ");
    gc_root(strlit305);
  }
// compilenode returning strlit305
  int op_slot_right_304 = gc_frame_newslot(strlit305);
  params[0] = strlit305;
  partcv[0] = 1;
  Object opresult307 = callmethod(*var_padding, "++", 1, partcv, params);
// compilenode returning opresult307
  gc_frame_newslot(opresult307);
  params[0] = opresult303;
  params[1] = opresult307;
  partcv[0] = 2;
  Object call308 = callmethodflags(self, "recurseDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe296);
// compilenode returning call308
    gc_frame_newslot(call308);
    if280 = call308;
  } else {
  }
// compilenode returning if280
    gc_frame_newslot(if280);
    if264 = if280;
  } else {
  }
// compilenode returning if264
  gc_frame_end(frame);
  return if264;
}
Object meth_grace_45_pkg_recurseDirectory258(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 114
  setline(114);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe259 = gc_frame_new();
// Begin line 113
  setline(113);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_path
  gc_frame_newslot(*var_path);
// compilenode returning *var_io
  params[0] = *var_path;
  partcv[0] = 1;
  Object call260 = callmethod(*var_io, "getdir",
    1, partcv, params);
  gc_frame_end(callframe259);
// compilenode returning call260
  *var_pathContents = call260;
  if (call260 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 122
  setline(122);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe261 = gc_frame_new();
// Begin line 114
  setline(114);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pathContents
  gc_frame_newslot(*var_pathContents);
// Begin line 122
  setline(122);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block262 = alloc_Block(NULL, NULL, "grace-pkg", 122);
  gc_frame_newslot(block262);
  block_savedest(block262);
  Object closure263 = createclosure(4, "_apply");
setclosureframe(closure263, stackframe);
  addtoclosure(closure263, var_padding);
  addtoclosure(closure263, var_path);
  addtoclosure(closure263, var_io);
  addtoclosure(closure263, selfslot);
  struct UserObject *uo263 = (struct UserObject*)block262;
  uo263->data[0] = (Object)closure263;
  Method *meth_meth_grace_45_pkg__apply263 = addmethod2pos(block262, "_apply", &meth_grace_45_pkg__apply263, 0);
int argcv_meth_grace_45_pkg__apply263[] = {1};
meth_meth_grace_45_pkg__apply263->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply263);
  meth_meth_grace_45_pkg__apply263->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply263->definitionLine = 101;
// compilenode returning block262
  gc_frame_newslot(block262);
  params[0] = *var_pathContents;
  params[1] = block262;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call310 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe261);
// compilenode returning call310
  gc_frame_end(frame);
  return call310;
}
Object meth_grace_45_pkg__apply338(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int callframe339 = gc_frame_new();
// Begin line 131
  setline(131);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_im
  gc_frame_newslot(*var_im);
  params[0] = *var_im;
  partcv[0] = 1;
  Object call340 = callmethodflags(self, "write", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe339);
// compilenode returning call340
  gc_frame_end(frame);
  return call340;
}
Object meth_grace_45_pkg__apply345(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int callframe346 = gc_frame_new();
// Begin line 134
  setline(134);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_im
  gc_frame_newslot(*var_im);
  params[0] = *var_im;
  partcv[0] = 1;
  Object call347 = callmethodflags(self, "compile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe346);
// compilenode returning call347
  gc_frame_end(frame);
  return call347;
}
Object meth_grace_45_pkg_doGet311(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 126
  setline(126);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 125
  setline(125);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array312 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array312
  *var_imported = array312;
  if (array312 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 129
  setline(129);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 126
  setline(126);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe317 = gc_frame_new();
// compilenode returning *var_impAddress
  partcv[0] = 0;
  Object call318 = callmethod(*var_impAddress, "size",
    1, partcv, params);
  gc_frame_end(callframe317);
// compilenode returning call318
// compilenode returning call318
  int op_slot_left_316 = gc_frame_newslot(call318);
  Object num319 = alloc_Float64(7);
// compilenode returning num319
  int op_slot_right_316 = gc_frame_newslot(num319);
  params[0] = num319;
  partcv[0] = 1;
  Object opresult321 = callmethod(call318, ">=", 1, partcv, params);
// compilenode returning opresult321
  int op_slot_left_315 = gc_frame_newslot(opresult321);
  int callframe323 = gc_frame_new();
  Object num324 = alloc_Float64(1);
// compilenode returning num324
  gc_frame_newslot(num324);
  Object num325 = alloc_Float64(7);
// compilenode returning num325
  gc_frame_newslot(num325);
// compilenode returning *var_impAddress
  params[0] = num324;
  params[1] = num325;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call326 = callmethod(*var_impAddress, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe323);
// compilenode returning call326
  int op_slot_left_322 = gc_frame_newslot(call326);
  if (strlit327 == NULL) {
    strlit327 = alloc_String("http://");
    gc_root(strlit327);
  }
// compilenode returning strlit327
  int op_slot_right_322 = gc_frame_newslot(strlit327);
  params[0] = strlit327;
  partcv[0] = 1;
  Object opresult329 = callmethod(call326, "==", 1, partcv, params);
// compilenode returning opresult329
  int op_slot_right_315 = gc_frame_newslot(opresult329);
  params[0] = opresult329;
  partcv[0] = 1;
  Object opresult331 = callmethod(opresult321, "&&", 1, partcv, params);
// compilenode returning opresult331
  Object if314 = done;
  if (istrue(opresult331)) {
// Begin line 128
  setline(128);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe332 = gc_frame_new();
// Begin line 127
  setline(127);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_impAddress
  gc_frame_newslot(*var_impAddress);
  params[0] = *var_impAddress;
  partcv[0] = 1;
  Object call333 = callmethodflags(self, "setBaseUrl", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe332);
// compilenode returning call333
    gc_frame_newslot(call333);
    if314 = call333;
  } else {
  }
// compilenode returning if314
// Begin line 130
  setline(130);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe334 = gc_frame_new();
// Begin line 129
  setline(129);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_impAddress
  gc_frame_newslot(*var_impAddress);
  params[0] = *var_impAddress;
  partcv[0] = 1;
  Object call335 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe334);
// compilenode returning call335
// Begin line 133
  setline(133);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe336 = gc_frame_new();
// Begin line 130
  setline(130);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_imported
  gc_frame_newslot(*var_imported);
// Begin line 133
  setline(133);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block337 = alloc_Block(NULL, NULL, "grace-pkg", 133);
  gc_frame_newslot(block337);
  block_savedest(block337);
  Object closure338 = createclosure(1, "_apply");
setclosureframe(closure338, stackframe);
  addtoclosure(closure338, selfslot);
  struct UserObject *uo338 = (struct UserObject*)block337;
  uo338->data[0] = (Object)closure338;
  Method *meth_meth_grace_45_pkg__apply338 = addmethod2pos(block337, "_apply", &meth_grace_45_pkg__apply338, 0);
int argcv_meth_grace_45_pkg__apply338[] = {1};
meth_meth_grace_45_pkg__apply338->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply338);
  meth_meth_grace_45_pkg__apply338->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply338->definitionLine = 126;
// compilenode returning block337
  gc_frame_newslot(block337);
  params[0] = *var_imported;
  params[1] = block337;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call342 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe336);
// compilenode returning call342
// Begin line 136
  setline(136);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe343 = gc_frame_new();
// Begin line 133
  setline(133);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_imported
  gc_frame_newslot(*var_imported);
// Begin line 136
  setline(136);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block344 = alloc_Block(NULL, NULL, "grace-pkg", 136);
  gc_frame_newslot(block344);
  block_savedest(block344);
  Object closure345 = createclosure(1, "_apply");
setclosureframe(closure345, stackframe);
  addtoclosure(closure345, selfslot);
  struct UserObject *uo345 = (struct UserObject*)block344;
  uo345->data[0] = (Object)closure345;
  Method *meth_meth_grace_45_pkg__apply345 = addmethod2pos(block344, "_apply", &meth_grace_45_pkg__apply345, 0);
int argcv_meth_grace_45_pkg__apply345[] = {1};
meth_meth_grace_45_pkg__apply345->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply345);
  meth_meth_grace_45_pkg__apply345->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply345->definitionLine = 126;
// compilenode returning block344
  gc_frame_newslot(block344);
  params[0] = *var_imported;
  params[1] = block344;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call349 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe343);
// compilenode returning call349
  gc_frame_end(frame);
  return call349;
}
Object meth_grace_45_pkg_setBaseUrl350(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 140
  setline(140);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe351 = gc_frame_new();
// Begin line 139
  setline(139);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_baseAddress
  gc_frame_newslot(*var_baseAddress);
  params[0] = *var_baseAddress;
  partcv[0] = 1;
  Object call352 = callmethodflags(self, "getBaseUrl", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe351);
// compilenode returning call352
  *var_baseUrl = call352;
  if (call352 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_grace_45_pkg_setFile354(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 151
  setline(151);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 143
  setline(143);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe357 = gc_frame_new();
  int callframe359 = gc_frame_new();
// compilenode returning *var_fileAddress
  partcv[0] = 0;
  Object call360 = callmethod(*var_fileAddress, "size",
    1, partcv, params);
  gc_frame_end(callframe359);
// compilenode returning call360
// compilenode returning call360
  int op_slot_left_358 = gc_frame_newslot(call360);
  Object num361 = alloc_Float64(5);
// compilenode returning num361
  int op_slot_right_358 = gc_frame_newslot(num361);
  params[0] = num361;
  partcv[0] = 1;
  Object diff363 = callmethod(call360, "-", 1, partcv, params);
// compilenode returning diff363
  gc_frame_newslot(diff363);
  int callframe364 = gc_frame_new();
// compilenode returning *var_fileAddress
  partcv[0] = 0;
  Object call365 = callmethod(*var_fileAddress, "size",
    1, partcv, params);
  gc_frame_end(callframe364);
// compilenode returning call365
// compilenode returning call365
  gc_frame_newslot(call365);
// compilenode returning *var_fileAddress
  params[0] = diff363;
  params[1] = call365;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call366 = callmethod(*var_fileAddress, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe357);
// compilenode returning call366
  int op_slot_left_356 = gc_frame_newslot(call366);
  if (strlit367 == NULL) {
    strlit367 = alloc_String(".grace");
    gc_root(strlit367);
  }
// compilenode returning strlit367
  int op_slot_right_356 = gc_frame_newslot(strlit367);
  params[0] = strlit367;
  partcv[0] = 1;
  Object opresult369 = callmethod(call366, "==", 1, partcv, params);
// compilenode returning opresult369
  Object if355 = done;
  if (istrue(opresult369)) {
// Begin line 144
  setline(144);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object inheritingObject370 = inheritingObject;
  if (isTailObject) {
    isTailObject = 0;
    inheritingObject = NULL;
  }
  Object obj370 = alloc_userobj2(4,4, objclass370);
  gc_frame_newslot(obj370);
// OBJECT OUTER DEC outer
  adddatum2(obj370, self, 0);
  addmethodreal(obj370,"outer", &reader_grace_45_pkg_outer_371);
  Object oldself370 = self;
  struct StackFrameObject *oldstackframe370 = stackframe;
  stackframe = alloc_StackFrame(1, oldstackframe370);
  gc_frame_newslot((Object)stackframe);
  self = obj370;
  Object *oldselfslot370 = selfslot;
  selfslot = &stackframe->slots[0];
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  Object thisouter370 = (*(struct UserObject *)self).data[0], lowerouter370 = thisouter370;
  if (inheritingObject370) {
    struct UserObject *inho370 = (struct UserObject *)inheritingObject370;
    while (inho370->super != GraceDefaultObject) inho370 = (struct UserObject *)inho370->super;
    inho370->super = obj370;
    self = inheritingObject370;
    *selfslot = self;
    lowerouter370 = (*(struct UserObject *)self).data[0];
    (*(struct UserObject *)self).data[0] = thisouter370;
  }
if (objclass370 == NULL) {
  Method *reader372 = addmethodrealflags(obj370, "address",&reader_grace_45_pkg_address_372, 0);
  Method *writer372 = addmethodrealflags(obj370, "address:=",&writer_grace_45_pkg_address_372, 0);
  reader372->definitionModule = modulename;
  writer372->definitionModule = modulename;
  reader372->definitionLine = 146;
  writer372->definitionLine = 146;
}
obj370->flags |= OFLAG_MUTABLE;
adddatum2(obj370, alloc_Undefined(), 1);
if (objclass370 == NULL) {
  Method *reader373 = addmethodrealflags(obj370, "data",&reader_grace_45_pkg_data_373, 0);
  Method *writer373 = addmethodrealflags(obj370, "data:=",&writer_grace_45_pkg_data_373, 0);
  reader373->definitionModule = modulename;
  writer373->definitionModule = modulename;
  reader373->definitionLine = 147;
  writer373->definitionLine = 147;
}
obj370->flags |= OFLAG_MUTABLE;
adddatum2(obj370, alloc_Undefined(), 2);
  sourceObject = obj370;
// Begin line 145
  setline(145);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_fileAddress
  adddatum2(obj370, *var_fileAddress, 1);
  sourceObject = obj370;
  adddatum2(obj370, undefined, 2);
objclass370 = obj370->class;
  objclass370->definitionModule = modulename;
  objclass370->definitionLine = 144;
  (*(struct UserObject *)self).data[0] = lowerouter370;
  self = oldself370;
  selfslot = oldselfslot370;
  stackframe = oldstackframe370;
// compilenode returning obj370
  *var_file = obj370;
  if (obj370 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 149
  setline(149);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 148
  setline(148);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_file
  *var_curFile = *var_file;
  if (*var_file == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 150
  setline(150);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 149
  setline(149);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool375 = alloc_Boolean(1);
// compilenode returning bool375
  return bool375;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if355 = undefined;
  } else {
  }
// compilenode returning if355
// Begin line 152
  setline(152);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 151
  setline(151);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool376 = alloc_Boolean(0);
// compilenode returning bool376
  return bool376;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_grace_45_pkg__apply422(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int callframe424 = gc_frame_new();
// compilenode returning *var_toProcess
  partcv[0] = 0;
  Object call425 = callmethod(*var_toProcess, "size",
    1, partcv, params);
  gc_frame_end(callframe424);
// compilenode returning call425
// compilenode returning call425
  int op_slot_left_423 = gc_frame_newslot(call425);
  Object num426 = alloc_Float64(0);
// compilenode returning num426
  int op_slot_right_423 = gc_frame_newslot(num426);
  params[0] = num426;
  partcv[0] = 1;
  Object opresult428 = callmethod(call425, ">", 1, partcv, params);
// compilenode returning opresult428
  gc_frame_end(frame);
  return opresult428;
}
Object meth_grace_45_pkg__apply431(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 167
  setline(167);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe432 = gc_frame_new();
// Begin line 166
  setline(166);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe433 = gc_frame_new();
// compilenode returning *var_toProcess
  partcv[0] = 0;
  Object call434 = callmethod(*var_toProcess, "pop",
    1, partcv, params);
  gc_frame_end(callframe433);
// compilenode returning call434
// compilenode returning call434
  gc_frame_newslot(call434);
  params[0] = call434;
  partcv[0] = 1;
  Object call435 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe432);
// compilenode returning call435
  gc_frame_end(frame);
  return call435;
}
Object meth_grace_45_pkg_fetchImports377(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 177
  setline(177);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 155
  setline(155);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe379 = gc_frame_new();
// compilenode returning *var_fileAddress
  gc_frame_newslot(*var_fileAddress);
  params[0] = *var_fileAddress;
  partcv[0] = 1;
  Object call380 = callmethodflags(self, "setFile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe379);
// compilenode returning call380
  Object if378 = done;
  if (istrue(call380)) {
// Begin line 157
  setline(157);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 156
  setline(156);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit383 == NULL) {
    strlit383 = alloc_String("File address ");
    gc_root(strlit383);
  }
// compilenode returning strlit383
  int op_slot_left_382 = gc_frame_newslot(strlit383);
  int callframe384 = gc_frame_new();
// compilenode returning *var_curFile
  partcv[0] = 0;
  Object call385 = callmethod(*var_curFile, "address",
    1, partcv, params);
  gc_frame_end(callframe384);
// compilenode returning call385
// compilenode returning call385
  int op_slot_right_382 = gc_frame_newslot(call385);
  params[0] = call385;
  partcv[0] = 1;
  Object opresult387 = callmethod(strlit383, "++", 1, partcv, params);
// compilenode returning opresult387
  int op_slot_left_381 = gc_frame_newslot(opresult387);
  if (strlit388 == NULL) {
    strlit388 = alloc_String("");
    gc_root(strlit388);
  }
// compilenode returning strlit388
  int op_slot_right_381 = gc_frame_newslot(strlit388);
  params[0] = strlit388;
  partcv[0] = 1;
  Object opresult390 = callmethod(opresult387, "++", 1, partcv, params);
// compilenode returning opresult390
  params[0] = opresult390;
  Object call391 = gracelib_print(NULL, 1,  params);
// compilenode returning call391
// Begin line 161
  setline(161);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 157
  setline(157);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe394 = gc_frame_new();
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call395 = callmethodflags(self, "setFileData", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe394);
// compilenode returning call395
  int op_slot_left_393 = gc_frame_newslot(call395);
  Object bool396 = alloc_Boolean(0);
// compilenode returning bool396
  int op_slot_right_393 = gc_frame_newslot(bool396);
  params[0] = bool396;
  partcv[0] = 1;
  Object opresult398 = callmethod(call395, "==", 1, partcv, params);
// compilenode returning opresult398
  Object if392 = done;
  if (istrue(opresult398)) {
// Begin line 159
  setline(159);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 158
  setline(158);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit399 == NULL) {
    strlit399 = alloc_String("Could not retrieve file data");
    gc_root(strlit399);
  }
// compilenode returning strlit399
  params[0] = strlit399;
  Object call400 = gracelib_print(NULL, 1,  params);
// compilenode returning call400
// Begin line 160
  setline(160);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 159
  setline(159);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool401 = alloc_Boolean(0);
// compilenode returning bool401
  return bool401;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if392 = undefined;
  } else {
  }
// compilenode returning if392
// Begin line 170
  setline(170);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 161
  setline(161);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe403 = gc_frame_new();
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call404 = callmethodflags(self, "validateFile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe403);
// compilenode returning call404
  Object if402 = done;
  if (istrue(call404)) {
// Begin line 163
  setline(163);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 162
  setline(162);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit407 == NULL) {
    strlit407 = alloc_String("Pushing ");
    gc_root(strlit407);
  }
// compilenode returning strlit407
  int op_slot_left_406 = gc_frame_newslot(strlit407);
  int callframe408 = gc_frame_new();
// compilenode returning *var_curFile
  partcv[0] = 0;
  Object call409 = callmethod(*var_curFile, "address",
    1, partcv, params);
  gc_frame_end(callframe408);
// compilenode returning call409
// compilenode returning call409
  int op_slot_right_406 = gc_frame_newslot(call409);
  params[0] = call409;
  partcv[0] = 1;
  Object opresult411 = callmethod(strlit407, "++", 1, partcv, params);
// compilenode returning opresult411
  int op_slot_left_405 = gc_frame_newslot(opresult411);
  if (strlit412 == NULL) {
    strlit412 = alloc_String(" to imported");
    gc_root(strlit412);
  }
// compilenode returning strlit412
  int op_slot_right_405 = gc_frame_newslot(strlit412);
  params[0] = strlit412;
  partcv[0] = 1;
  Object opresult414 = callmethod(opresult411, "++", 1, partcv, params);
// compilenode returning opresult414
  params[0] = opresult414;
  Object call415 = gracelib_print(NULL, 1,  params);
// compilenode returning call415
// Begin line 164
  setline(164);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe416 = gc_frame_new();
// Begin line 163
  setline(163);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
// compilenode returning *var_imported
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call417 = callmethod(*var_imported, "push",
    1, partcv, params);
  gc_frame_end(callframe416);
// compilenode returning call417
// Begin line 165
  setline(165);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe418 = gc_frame_new();
// Begin line 164
  setline(164);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call419 = callmethodflags(self, "parseFile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe418);
// compilenode returning call419
// Begin line 168
  setline(168);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe420 = gc_frame_new();
// Begin line 165
  setline(165);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block421 = alloc_Block(NULL, NULL, "grace-pkg", 165);
  gc_frame_newslot(block421);
  block_savedest(block421);
  Object closure422 = createclosure(2, "_apply");
setclosureframe(closure422, stackframe);
  addtoclosure(closure422, var_toProcess);
  addtoclosure(closure422, selfslot);
  struct UserObject *uo422 = (struct UserObject*)block421;
  uo422->data[0] = (Object)closure422;
  Method *meth_meth_grace_45_pkg__apply422 = addmethod2pos(block421, "_apply", &meth_grace_45_pkg__apply422, 0);
int argcv_meth_grace_45_pkg__apply422[] = {0};
meth_meth_grace_45_pkg__apply422->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply422);
  meth_meth_grace_45_pkg__apply422->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply422->definitionLine = 162;
// compilenode returning block421
  gc_frame_newslot(block421);
// Begin line 168
  setline(168);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block430 = alloc_Block(NULL, NULL, "grace-pkg", 168);
  gc_frame_newslot(block430);
  block_savedest(block430);
  Object closure431 = createclosure(2, "_apply");
setclosureframe(closure431, stackframe);
  addtoclosure(closure431, var_toProcess);
  addtoclosure(closure431, selfslot);
  struct UserObject *uo431 = (struct UserObject*)block430;
  uo431->data[0] = (Object)closure431;
  Method *meth_meth_grace_45_pkg__apply431 = addmethod2pos(block430, "_apply", &meth_grace_45_pkg__apply431, 0);
int argcv_meth_grace_45_pkg__apply431[] = {0};
meth_meth_grace_45_pkg__apply431->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply431);
  meth_meth_grace_45_pkg__apply431->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply431->definitionLine = 165;
// compilenode returning block430
  gc_frame_newslot(block430);
  params[0] = block421;
  params[1] = block430;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call437 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe420);
// compilenode returning call437
// Begin line 169
  setline(169);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 168
  setline(168);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool438 = alloc_Boolean(1);
// compilenode returning bool438
  return bool438;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if402 = undefined;
  } else {
  }
// compilenode returning if402
    gc_frame_newslot(if402);
    if378 = if402;
  } else {
// Begin line 176
  setline(176);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 172
  setline(172);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe441 = gc_frame_new();
// compilenode returning *var_fileAddress
  int op_slot_left_442 = gc_frame_newslot(*var_fileAddress);
  if (strlit443 == NULL) {
    strlit443 = alloc_String(".grace");
    gc_root(strlit443);
  }
// compilenode returning strlit443
  int op_slot_right_442 = gc_frame_newslot(strlit443);
  params[0] = strlit443;
  partcv[0] = 1;
  Object opresult445 = callmethod(*var_fileAddress, "++", 1, partcv, params);
// compilenode returning opresult445
  gc_frame_newslot(opresult445);
  params[0] = opresult445;
  partcv[0] = 1;
  Object call446 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe441);
// compilenode returning call446
  int op_slot_left_440 = gc_frame_newslot(call446);
  Object bool447 = alloc_Boolean(0);
// compilenode returning bool447
  int op_slot_right_440 = gc_frame_newslot(bool447);
  params[0] = bool447;
  partcv[0] = 1;
  Object opresult449 = callmethod(call446, "==", 1, partcv, params);
// compilenode returning opresult449
  Object if439 = done;
  if (istrue(opresult449)) {
// Begin line 174
  setline(174);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 173
  setline(173);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit450 == NULL) {
    strlit450 = alloc_String("Could not locate file. Check file address.");
    gc_root(strlit450);
  }
// compilenode returning strlit450
  params[0] = strlit450;
  Object call451 = gracelib_print(NULL, 1,  params);
// compilenode returning call451
// Begin line 175
  setline(175);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 174
  setline(174);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool452 = alloc_Boolean(0);
// compilenode returning bool452
  return bool452;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if439 = undefined;
  } else {
  }
// compilenode returning if439
    gc_frame_newslot(if439);
    if378 = if439;
  }
// compilenode returning if378
  gc_frame_end(frame);
  return if378;
}
Object meth_grace_45_pkg__apply471(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 186
  setline(186);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 185
  setline(185);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit472 == NULL) {
    strlit472 = alloc_String(" Response received");
    gc_root(strlit472);
  }
// compilenode returning strlit472
  params[0] = strlit472;
  Object call473 = gracelib_print(NULL, 1,  params);
// compilenode returning call473
// Begin line 190
  setline(190);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 186
  setline(186);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe476 = gc_frame_new();
// compilenode returning *var_req
  partcv[0] = 0;
  Object call477 = callmethod(*var_req, "responseCode",
    1, partcv, params);
  gc_frame_end(callframe476);
// compilenode returning call477
// compilenode returning call477
  int op_slot_left_475 = gc_frame_newslot(call477);
  Object num478 = alloc_Float64(200);
// compilenode returning num478
  int op_slot_right_475 = gc_frame_newslot(num478);
  params[0] = num478;
  partcv[0] = 1;
  Object opresult480 = callmethod(call477, "!=", 1, partcv, params);
// compilenode returning opresult480
  Object if474 = done;
  if (istrue(opresult480)) {
// Begin line 188
  setline(188);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 187
  setline(187);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit485 == NULL) {
    strlit485 = alloc_String(" Could not locate import: Error ");
    gc_root(strlit485);
  }
// compilenode returning strlit485
  int op_slot_left_484 = gc_frame_newslot(strlit485);
  int callframe486 = gc_frame_new();
// compilenode returning *var_req
  partcv[0] = 0;
  Object call487 = callmethod(*var_req, "responseCode",
    1, partcv, params);
  gc_frame_end(callframe486);
// compilenode returning call487
// compilenode returning call487
  int op_slot_right_484 = gc_frame_newslot(call487);
  params[0] = call487;
  partcv[0] = 1;
  Object opresult489 = callmethod(strlit485, "++", 1, partcv, params);
// compilenode returning opresult489
  int op_slot_left_483 = gc_frame_newslot(opresult489);
  if (strlit490 == NULL) {
    strlit490 = alloc_String(" for import ");
    gc_root(strlit490);
  }
// compilenode returning strlit490
  int op_slot_right_483 = gc_frame_newslot(strlit490);
  params[0] = strlit490;
  partcv[0] = 1;
  Object opresult492 = callmethod(opresult489, "++", 1, partcv, params);
// compilenode returning opresult492
  int op_slot_left_482 = gc_frame_newslot(opresult492);
  int callframe493 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call494 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe493);
// compilenode returning call494
// compilenode returning call494
  int op_slot_right_482 = gc_frame_newslot(call494);
  params[0] = call494;
  partcv[0] = 1;
  Object opresult496 = callmethod(opresult492, "++", 1, partcv, params);
// compilenode returning opresult496
  int op_slot_left_481 = gc_frame_newslot(opresult496);
  if (strlit497 == NULL) {
    strlit497 = alloc_String("");
    gc_root(strlit497);
  }
// compilenode returning strlit497
  int op_slot_right_481 = gc_frame_newslot(strlit497);
  params[0] = strlit497;
  partcv[0] = 1;
  Object opresult499 = callmethod(opresult496, "++", 1, partcv, params);
// compilenode returning opresult499
  params[0] = opresult499;
  Object call500 = gracelib_print(NULL, 1,  params);
// compilenode returning call500
// Begin line 189
  setline(189);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 188
  setline(188);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool501 = alloc_Boolean(0);
// compilenode returning bool501
  block_return(realself, bool501);
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if474 = undefined;
  } else {
  }
// compilenode returning if474
// Begin line 191
  setline(191);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 187
  setline(187);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe502 = gc_frame_new();
// Begin line 191
  setline(191);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe503 = gc_frame_new();
// Begin line 190
  setline(190);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit504 == NULL) {
    strlit504 = alloc_String("utf-8");
    gc_root(strlit504);
  }
// compilenode returning strlit504
  gc_frame_newslot(strlit504);
// compilenode returning *var_d
  params[0] = strlit504;
  partcv[0] = 1;
  Object call505 = callmethod(*var_d, "decode",
    1, partcv, params);
  gc_frame_end(callframe503);
// compilenode returning call505
  gc_frame_newslot(call505);
// compilenode returning *var_file
  params[0] = call505;
  partcv[0] = 1;
  Object call506 = callmethod(*var_file, "data:=",
    1, partcv, params);
  gc_frame_end(callframe502);
// compilenode returning call506
// compilenode returning done
// Begin line 192
  setline(192);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 191
  setline(191);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool507 = alloc_Boolean(1);
// compilenode returning bool507
  block_return(realself, bool507);
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_grace_45_pkg_performCurlFetch453(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 181
  setline(181);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 180
  setline(180);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe454 = gc_frame_new();
// compilenode returning *var_curl
  partcv[0] = 0;
  Object call455 = callmethod(*var_curl, "easy",
    1, partcv, params);
  gc_frame_end(callframe454);
// compilenode returning call455
// compilenode returning call455
  *var_req = call455;
  if (call455 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 182
  setline(182);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 180
  setline(180);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe456 = gc_frame_new();
// Begin line 181
  setline(181);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe457 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call458 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe457);
// compilenode returning call458
// compilenode returning call458
  gc_frame_newslot(call458);
// compilenode returning *var_req
  params[0] = call458;
  partcv[0] = 1;
  Object call459 = callmethod(*var_req, "url:=",
    1, partcv, params);
  gc_frame_end(callframe456);
// compilenode returning call459
// compilenode returning done
// Begin line 183
  setline(183);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 182
  setline(182);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit461 == NULL) {
    strlit461 = alloc_String("Searching for import: ");
    gc_root(strlit461);
  }
// compilenode returning strlit461
  int op_slot_left_460 = gc_frame_newslot(strlit461);
  int callframe462 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call463 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe462);
// compilenode returning call463
// compilenode returning call463
  int op_slot_right_460 = gc_frame_newslot(call463);
  params[0] = call463;
  partcv[0] = 1;
  Object opresult465 = callmethod(strlit461, "++", 1, partcv, params);
// compilenode returning opresult465
  params[0] = opresult465;
  Object call466 = gracelib_print(NULL, 1,  params);
// compilenode returning call466
// Begin line 184
  setline(184);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 183
  setline(183);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit467 == NULL) {
    strlit467 = alloc_String(" Connecting...");
    gc_root(strlit467);
  }
// compilenode returning strlit467
  params[0] = strlit467;
  Object call468 = gracelib_print(NULL, 1,  params);
// compilenode returning call468
// Begin line 193
  setline(193);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe469 = gc_frame_new();
  Object block470 = alloc_Block(NULL, NULL, "grace-pkg", 193);
  gc_frame_newslot(block470);
  block_savedest(block470);
  Object closure471 = createclosure(3, "_apply");
setclosureframe(closure471, stackframe);
  addtoclosure(closure471, var_req);
  addtoclosure(closure471, var_file);
  addtoclosure(closure471, selfslot);
  struct UserObject *uo471 = (struct UserObject*)block470;
  uo471->data[0] = (Object)closure471;
  Method *meth_meth_grace_45_pkg__apply471 = addmethod2pos(block470, "_apply", &meth_grace_45_pkg__apply471, 0);
int argcv_meth_grace_45_pkg__apply471[] = {1};
meth_meth_grace_45_pkg__apply471->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply471);
  meth_meth_grace_45_pkg__apply471->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply471->definitionLine = 182;
// compilenode returning block470
  gc_frame_newslot(block470);
// Begin line 184
  setline(184);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_req
  params[0] = block470;
  partcv[0] = 1;
  Object call509 = callmethod(*var_req, "onReceive",
    1, partcv, params);
  gc_frame_end(callframe469);
// compilenode returning call509
// Begin line 193
  setline(193);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe510 = gc_frame_new();
// compilenode returning *var_req
  partcv[0] = 0;
  Object call511 = callmethod(*var_req, "perform",
    1, partcv, params);
  gc_frame_end(callframe510);
// compilenode returning call511
// compilenode returning call511
  gc_frame_end(frame);
  return call511;
}
Object meth_grace_45_pkg_setFileData512(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 224
  setline(224);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 197
  setline(197);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe515 = gc_frame_new();
  Object num516 = alloc_Float64(1);
// compilenode returning num516
  gc_frame_newslot(num516);
  Object num517 = alloc_Float64(4);
// compilenode returning num517
  gc_frame_newslot(num517);
  int callframe518 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call519 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe518);
// compilenode returning call519
// compilenode returning call519
  params[0] = num516;
  params[1] = num517;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call520 = callmethod(call519, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe515);
// compilenode returning call520
  int op_slot_left_514 = gc_frame_newslot(call520);
  if (strlit521 == NULL) {
    strlit521 = alloc_String("http");
    gc_root(strlit521);
  }
// compilenode returning strlit521
  int op_slot_right_514 = gc_frame_newslot(strlit521);
  params[0] = strlit521;
  partcv[0] = 1;
  Object opresult523 = callmethod(call520, "==", 1, partcv, params);
// compilenode returning opresult523
  Object if513 = done;
  if (istrue(opresult523)) {
// Begin line 199
  setline(199);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe524 = gc_frame_new();
// Begin line 198
  setline(198);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num525 = alloc_Float64(1);
// compilenode returning num525
  gc_frame_newslot(num525);
  Object num526 = alloc_Float64(4);
// compilenode returning num526
  gc_frame_newslot(num526);
  int callframe527 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call528 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe527);
// compilenode returning call528
// compilenode returning call528
  params[0] = num525;
  params[1] = num526;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call529 = callmethod(call528, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe524);
// compilenode returning call529
  *var_strippedUrl = call529;
  if (call529 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 207
  setline(207);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 199
  setline(199);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe532 = gc_frame_new();
  int callframe533 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call534 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe533);
// compilenode returning call534
// compilenode returning call534
  gc_frame_newslot(call534);
  params[0] = call534;
  partcv[0] = 1;
  Object call535 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe532);
// compilenode returning call535
  int op_slot_left_531 = gc_frame_newslot(call535);
  Object bool536 = alloc_Boolean(0);
// compilenode returning bool536
  int op_slot_right_531 = gc_frame_newslot(bool536);
  params[0] = bool536;
  partcv[0] = 1;
  Object opresult538 = callmethod(call535, "!=", 1, partcv, params);
// compilenode returning opresult538
  Object if530 = done;
  if (istrue(opresult538)) {
// Begin line 201
  setline(201);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe539 = gc_frame_new();
// Begin line 200
  setline(200);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_strippedUrl
  gc_frame_newslot(*var_strippedUrl);
  params[0] = *var_strippedUrl;
  partcv[0] = 1;
  Object call540 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe539);
// compilenode returning call540
  *var_findData = call540;
  if (call540 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 205
  setline(205);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 201
  setline(201);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  int op_slot_left_542 = gc_frame_newslot(*var_findData);
  Object num543 = alloc_Float64(0);
// compilenode returning num543
  int op_slot_right_542 = gc_frame_newslot(num543);
  params[0] = num543;
  partcv[0] = 1;
  Object opresult545 = callmethod(*var_findData, ">", 1, partcv, params);
// compilenode returning opresult545
  Object if541 = done;
  if (istrue(opresult545)) {
// Begin line 203
  setline(203);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 199
  setline(199);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe546 = gc_frame_new();
// Begin line 202
  setline(202);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  gc_frame_newslot(*var_findData);
// compilenode returning *var_file
  params[0] = *var_findData;
  partcv[0] = 1;
  Object call547 = callmethod(*var_file, "data:=",
    1, partcv, params);
  gc_frame_end(callframe546);
// compilenode returning call547
// compilenode returning done
// Begin line 204
  setline(204);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 203
  setline(203);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool548 = alloc_Boolean(1);
// compilenode returning bool548
  return bool548;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if541 = undefined;
  } else {
  }
// compilenode returning if541
// Begin line 205
  setline(205);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool549 = alloc_Boolean(0);
// compilenode returning bool549
  return bool549;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if530 = undefined;
  } else {
  }
// compilenode returning if530
// Begin line 209
  setline(209);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe550 = gc_frame_new();
// Begin line 207
  setline(207);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_file
  gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object call551 = callmethodflags(self, "performCurlFetch", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe550);
// compilenode returning call551
  return call551;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if513 = undefined;
  } else {
// Begin line 218
  setline(218);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 210
  setline(210);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe554 = gc_frame_new();
  int callframe555 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call556 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe555);
// compilenode returning call556
// compilenode returning call556
  gc_frame_newslot(call556);
  params[0] = call556;
  partcv[0] = 1;
  Object call557 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe554);
// compilenode returning call557
  int op_slot_left_553 = gc_frame_newslot(call557);
  Object bool558 = alloc_Boolean(0);
// compilenode returning bool558
  int op_slot_right_553 = gc_frame_newslot(bool558);
  params[0] = bool558;
  partcv[0] = 1;
  Object opresult560 = callmethod(call557, "!=", 1, partcv, params);
// compilenode returning opresult560
  Object if552 = done;
  if (istrue(opresult560)) {
// Begin line 212
  setline(212);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 211
  setline(211);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit561 == NULL) {
    strlit561 = alloc_String("Now searching in find existing");
    gc_root(strlit561);
  }
// compilenode returning strlit561
  params[0] = strlit561;
  Object call562 = gracelib_print(NULL, 1,  params);
// compilenode returning call562
// Begin line 213
  setline(213);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe563 = gc_frame_new();
// Begin line 212
  setline(212);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe564 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call565 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe564);
// compilenode returning call565
// compilenode returning call565
  gc_frame_newslot(call565);
  params[0] = call565;
  partcv[0] = 1;
  Object call566 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe563);
// compilenode returning call566
  *var_findData = call566;
  if (call566 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 217
  setline(217);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 213
  setline(213);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  int op_slot_left_568 = gc_frame_newslot(*var_findData);
  Object bool569 = alloc_Boolean(0);
// compilenode returning bool569
  int op_slot_right_568 = gc_frame_newslot(bool569);
  params[0] = bool569;
  partcv[0] = 1;
  Object opresult571 = callmethod(*var_findData, "!=", 1, partcv, params);
// compilenode returning opresult571
  Object if567 = done;
  if (istrue(opresult571)) {
// Begin line 215
  setline(215);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 212
  setline(212);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe572 = gc_frame_new();
// Begin line 214
  setline(214);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  gc_frame_newslot(*var_findData);
// compilenode returning *var_file
  params[0] = *var_findData;
  partcv[0] = 1;
  Object call573 = callmethod(*var_file, "data:=",
    1, partcv, params);
  gc_frame_end(callframe572);
// compilenode returning call573
// compilenode returning done
// Begin line 216
  setline(216);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 215
  setline(215);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool574 = alloc_Boolean(1);
// compilenode returning bool574
  return bool574;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if567 = undefined;
  } else {
  }
// compilenode returning if567
    gc_frame_newslot(if567);
    if552 = if567;
  } else {
// Begin line 224
  setline(224);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 218
  setline(218);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_baseUrl
  int op_slot_left_576 = gc_frame_newslot(*var_baseUrl);
  if (strlit577 == NULL) {
    strlit577 = alloc_String("");
    gc_root(strlit577);
  }
// compilenode returning strlit577
  int op_slot_right_576 = gc_frame_newslot(strlit577);
  params[0] = strlit577;
  partcv[0] = 1;
  Object opresult579 = callmethod(*var_baseUrl, "!=", 1, partcv, params);
// compilenode returning opresult579
  Object if575 = done;
  if (istrue(opresult579)) {
// Begin line 221
  setline(221);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 212
  setline(212);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe580 = gc_frame_new();
// Begin line 221
  setline(221);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 220
  setline(220);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_baseUrl
  int op_slot_left_581 = gc_frame_newslot(*var_baseUrl);
  int callframe582 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call583 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe582);
// compilenode returning call583
// compilenode returning call583
  int op_slot_right_581 = gc_frame_newslot(call583);
  params[0] = call583;
  partcv[0] = 1;
  Object opresult585 = callmethod(*var_baseUrl, "++", 1, partcv, params);
// compilenode returning opresult585
  gc_frame_newslot(opresult585);
// compilenode returning *var_file
  params[0] = opresult585;
  partcv[0] = 1;
  Object call586 = callmethod(*var_file, "address:=",
    1, partcv, params);
  gc_frame_end(callframe580);
// compilenode returning call586
// compilenode returning done
// Begin line 223
  setline(223);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe587 = gc_frame_new();
// Begin line 221
  setline(221);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_file
  gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object call588 = callmethodflags(self, "performCurlFetch", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe587);
// compilenode returning call588
  return call588;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if575 = undefined;
  } else {
  }
// compilenode returning if575
    gc_frame_newslot(if575);
    if552 = if575;
  }
// compilenode returning if552
    gc_frame_newslot(if552);
    if513 = if552;
  }
// compilenode returning if513
// Begin line 225
  setline(225);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 224
  setline(224);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool589 = alloc_Boolean(0);
// compilenode returning bool589
  return bool589;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_grace_45_pkg_findExisting590(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 233
  setline(233);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 228
  setline(228);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe592 = gc_frame_new();
// compilenode returning *var_fileName
  gc_frame_newslot(*var_fileName);
// compilenode returning *var_io
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object call593 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe592);
// compilenode returning call593
  Object if591 = done;
  if (istrue(call593)) {
// Begin line 230
  setline(230);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe594 = gc_frame_new();
// Begin line 229
  setline(229);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_fileName
  gc_frame_newslot(*var_fileName);
  if (strlit595 == NULL) {
    strlit595 = alloc_String("r");
    gc_root(strlit595);
  }
// compilenode returning strlit595
  gc_frame_newslot(strlit595);
// compilenode returning *var_io
  params[0] = *var_fileName;
  params[1] = strlit595;
  partcv[0] = 2;
  Object call596 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe594);
// compilenode returning call596
  *var_open = call596;
  if (call596 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 231
  setline(231);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 230
  setline(230);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe597 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call598 = callmethod(*var_open, "read",
    1, partcv, params);
  gc_frame_end(callframe597);
// compilenode returning call598
// compilenode returning call598
  *var_ret = call598;
  if (call598 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 231
  setline(231);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_ret
  return *var_ret;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if591 = undefined;
  } else {
  }
// compilenode returning if591
// Begin line 236
  setline(236);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 233
  setline(233);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe600 = gc_frame_new();
  if (strlit602 == NULL) {
    strlit602 = alloc_String("/usr/lib/grace/modules/");
    gc_root(strlit602);
  }
// compilenode returning strlit602
  int op_slot_left_601 = gc_frame_newslot(strlit602);
// compilenode returning *var_fileName
  int op_slot_right_601 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult604 = callmethod(strlit602, "++", 1, partcv, params);
// compilenode returning opresult604
  gc_frame_newslot(opresult604);
// compilenode returning *var_io
  params[0] = opresult604;
  partcv[0] = 1;
  Object call605 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe600);
// compilenode returning call605
  Object if599 = done;
  if (istrue(call605)) {
// Begin line 235
  setline(235);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 234
  setline(234);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe606 = gc_frame_new();
  int callframe607 = gc_frame_new();
  if (strlit609 == NULL) {
    strlit609 = alloc_String("/usr/lib/grace/modules/");
    gc_root(strlit609);
  }
// compilenode returning strlit609
  int op_slot_left_608 = gc_frame_newslot(strlit609);
// compilenode returning *var_fileName
  int op_slot_right_608 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult611 = callmethod(strlit609, "++", 1, partcv, params);
// compilenode returning opresult611
  gc_frame_newslot(opresult611);
  if (strlit612 == NULL) {
    strlit612 = alloc_String("r");
    gc_root(strlit612);
  }
// compilenode returning strlit612
  gc_frame_newslot(strlit612);
// compilenode returning *var_io
  params[0] = opresult611;
  params[1] = strlit612;
  partcv[0] = 2;
  Object call613 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe607);
// compilenode returning call613
  partcv[0] = 0;
  Object call614 = callmethod(call613, "read",
    1, partcv, params);
  gc_frame_end(callframe606);
// compilenode returning call614
// compilenode returning call614
  return call614;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if599 = undefined;
  } else {
  }
// compilenode returning if599
// Begin line 237
  setline(237);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 236
  setline(236);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe615 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call616 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe615);
// compilenode returning call616
// compilenode returning call616
  if (strlit617 == NULL) {
    strlit617 = alloc_String("HOME");
    gc_root(strlit617);
  }
// compilenode returning strlit617
  params[0] = strlit617;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres618 = callmethod(call616, "[]", 1, partcv, params);
// compilenode returning idxres618
  *var_homePath = idxres618;
  if (idxres618 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 240
  setline(240);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 237
  setline(237);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe620 = gc_frame_new();
  if (strlit624 == NULL) {
    strlit624 = alloc_String("");
    gc_root(strlit624);
  }
// compilenode returning strlit624
  int op_slot_left_623 = gc_frame_newslot(strlit624);
// compilenode returning *var_homePath
  int op_slot_right_623 = gc_frame_newslot(*var_homePath);
  params[0] = *var_homePath;
  partcv[0] = 1;
  Object opresult626 = callmethod(strlit624, "++", 1, partcv, params);
// compilenode returning opresult626
  int op_slot_left_622 = gc_frame_newslot(opresult626);
  if (strlit627 == NULL) {
    strlit627 = alloc_String("/.local/share/grace/modules/");
    gc_root(strlit627);
  }
// compilenode returning strlit627
  int op_slot_right_622 = gc_frame_newslot(strlit627);
  params[0] = strlit627;
  partcv[0] = 1;
  Object opresult629 = callmethod(opresult626, "++", 1, partcv, params);
// compilenode returning opresult629
  int op_slot_left_621 = gc_frame_newslot(opresult629);
// compilenode returning *var_fileName
  int op_slot_right_621 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult631 = callmethod(opresult629, "++", 1, partcv, params);
// compilenode returning opresult631
  gc_frame_newslot(opresult631);
// compilenode returning *var_io
  params[0] = opresult631;
  partcv[0] = 1;
  Object call632 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe620);
// compilenode returning call632
  Object if619 = done;
  if (istrue(call632)) {
// Begin line 239
  setline(239);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 238
  setline(238);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe633 = gc_frame_new();
  int callframe634 = gc_frame_new();
  if (strlit638 == NULL) {
    strlit638 = alloc_String("");
    gc_root(strlit638);
  }
// compilenode returning strlit638
  int op_slot_left_637 = gc_frame_newslot(strlit638);
// compilenode returning *var_homePath
  int op_slot_right_637 = gc_frame_newslot(*var_homePath);
  params[0] = *var_homePath;
  partcv[0] = 1;
  Object opresult640 = callmethod(strlit638, "++", 1, partcv, params);
// compilenode returning opresult640
  int op_slot_left_636 = gc_frame_newslot(opresult640);
  if (strlit641 == NULL) {
    strlit641 = alloc_String("/.local/share/grace/modules/");
    gc_root(strlit641);
  }
// compilenode returning strlit641
  int op_slot_right_636 = gc_frame_newslot(strlit641);
  params[0] = strlit641;
  partcv[0] = 1;
  Object opresult643 = callmethod(opresult640, "++", 1, partcv, params);
// compilenode returning opresult643
  int op_slot_left_635 = gc_frame_newslot(opresult643);
// compilenode returning *var_fileName
  int op_slot_right_635 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult645 = callmethod(opresult643, "++", 1, partcv, params);
// compilenode returning opresult645
  gc_frame_newslot(opresult645);
  if (strlit646 == NULL) {
    strlit646 = alloc_String("r");
    gc_root(strlit646);
  }
// compilenode returning strlit646
  gc_frame_newslot(strlit646);
// compilenode returning *var_io
  params[0] = opresult645;
  params[1] = strlit646;
  partcv[0] = 2;
  Object call647 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe634);
// compilenode returning call647
  partcv[0] = 0;
  Object call648 = callmethod(call647, "read",
    1, partcv, params);
  gc_frame_end(callframe633);
// compilenode returning call648
// compilenode returning call648
  return call648;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if619 = undefined;
  } else {
  }
// compilenode returning if619
// Begin line 243
  setline(243);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 240
  setline(240);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe650 = gc_frame_new();
  if (strlit654 == NULL) {
    strlit654 = alloc_String("");
    gc_root(strlit654);
  }
// compilenode returning strlit654
  int op_slot_left_653 = gc_frame_newslot(strlit654);
  int callframe655 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call656 = callmethod(*var_sys, "execPath",
    1, partcv, params);
  gc_frame_end(callframe655);
// compilenode returning call656
// compilenode returning call656
  int op_slot_right_653 = gc_frame_newslot(call656);
  params[0] = call656;
  partcv[0] = 1;
  Object opresult658 = callmethod(strlit654, "++", 1, partcv, params);
// compilenode returning opresult658
  int op_slot_left_652 = gc_frame_newslot(opresult658);
  if (strlit659 == NULL) {
    strlit659 = alloc_String("/../lib/minigrace/modules/");
    gc_root(strlit659);
  }
// compilenode returning strlit659
  int op_slot_right_652 = gc_frame_newslot(strlit659);
  params[0] = strlit659;
  partcv[0] = 1;
  Object opresult661 = callmethod(opresult658, "++", 1, partcv, params);
// compilenode returning opresult661
  int op_slot_left_651 = gc_frame_newslot(opresult661);
// compilenode returning *var_fileName
  int op_slot_right_651 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult663 = callmethod(opresult661, "++", 1, partcv, params);
// compilenode returning opresult663
  gc_frame_newslot(opresult663);
// compilenode returning *var_io
  params[0] = opresult663;
  partcv[0] = 1;
  Object call664 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe650);
// compilenode returning call664
  Object if649 = done;
  if (istrue(call664)) {
// Begin line 242
  setline(242);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 241
  setline(241);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe665 = gc_frame_new();
  int callframe666 = gc_frame_new();
  if (strlit670 == NULL) {
    strlit670 = alloc_String("");
    gc_root(strlit670);
  }
// compilenode returning strlit670
  int op_slot_left_669 = gc_frame_newslot(strlit670);
  int callframe671 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call672 = callmethod(*var_sys, "execPath",
    1, partcv, params);
  gc_frame_end(callframe671);
// compilenode returning call672
// compilenode returning call672
  int op_slot_right_669 = gc_frame_newslot(call672);
  params[0] = call672;
  partcv[0] = 1;
  Object opresult674 = callmethod(strlit670, "++", 1, partcv, params);
// compilenode returning opresult674
  int op_slot_left_668 = gc_frame_newslot(opresult674);
  if (strlit675 == NULL) {
    strlit675 = alloc_String("/../lib/minigrace/modules/");
    gc_root(strlit675);
  }
// compilenode returning strlit675
  int op_slot_right_668 = gc_frame_newslot(strlit675);
  params[0] = strlit675;
  partcv[0] = 1;
  Object opresult677 = callmethod(opresult674, "++", 1, partcv, params);
// compilenode returning opresult677
  int op_slot_left_667 = gc_frame_newslot(opresult677);
// compilenode returning *var_fileName
  int op_slot_right_667 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult679 = callmethod(opresult677, "++", 1, partcv, params);
// compilenode returning opresult679
  gc_frame_newslot(opresult679);
  if (strlit680 == NULL) {
    strlit680 = alloc_String("r");
    gc_root(strlit680);
  }
// compilenode returning strlit680
  gc_frame_newslot(strlit680);
// compilenode returning *var_io
  params[0] = opresult679;
  params[1] = strlit680;
  partcv[0] = 2;
  Object call681 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe666);
// compilenode returning call681
  partcv[0] = 0;
  Object call682 = callmethod(call681, "read",
    1, partcv, params);
  gc_frame_end(callframe665);
// compilenode returning call682
// compilenode returning call682
  return call682;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if649 = undefined;
  } else {
  }
// compilenode returning if649
// Begin line 246
  setline(246);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 243
  setline(243);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe684 = gc_frame_new();
  if (strlit688 == NULL) {
    strlit688 = alloc_String("");
    gc_root(strlit688);
  }
// compilenode returning strlit688
  int op_slot_left_687 = gc_frame_newslot(strlit688);
  int callframe689 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call690 = callmethod(*var_sys, "execPath",
    1, partcv, params);
  gc_frame_end(callframe689);
// compilenode returning call690
// compilenode returning call690
  int op_slot_right_687 = gc_frame_newslot(call690);
  params[0] = call690;
  partcv[0] = 1;
  Object opresult692 = callmethod(strlit688, "++", 1, partcv, params);
// compilenode returning opresult692
  int op_slot_left_686 = gc_frame_newslot(opresult692);
  if (strlit693 == NULL) {
    strlit693 = alloc_String("/");
    gc_root(strlit693);
  }
// compilenode returning strlit693
  int op_slot_right_686 = gc_frame_newslot(strlit693);
  params[0] = strlit693;
  partcv[0] = 1;
  Object opresult695 = callmethod(opresult692, "++", 1, partcv, params);
// compilenode returning opresult695
  int op_slot_left_685 = gc_frame_newslot(opresult695);
// compilenode returning *var_fileName
  int op_slot_right_685 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult697 = callmethod(opresult695, "++", 1, partcv, params);
// compilenode returning opresult697
  gc_frame_newslot(opresult697);
// compilenode returning *var_io
  params[0] = opresult697;
  partcv[0] = 1;
  Object call698 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe684);
// compilenode returning call698
  Object if683 = done;
  if (istrue(call698)) {
// Begin line 245
  setline(245);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 244
  setline(244);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe699 = gc_frame_new();
  int callframe700 = gc_frame_new();
  if (strlit704 == NULL) {
    strlit704 = alloc_String("");
    gc_root(strlit704);
  }
// compilenode returning strlit704
  int op_slot_left_703 = gc_frame_newslot(strlit704);
  int callframe705 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call706 = callmethod(*var_sys, "execPath",
    1, partcv, params);
  gc_frame_end(callframe705);
// compilenode returning call706
// compilenode returning call706
  int op_slot_right_703 = gc_frame_newslot(call706);
  params[0] = call706;
  partcv[0] = 1;
  Object opresult708 = callmethod(strlit704, "++", 1, partcv, params);
// compilenode returning opresult708
  int op_slot_left_702 = gc_frame_newslot(opresult708);
  if (strlit709 == NULL) {
    strlit709 = alloc_String("/");
    gc_root(strlit709);
  }
// compilenode returning strlit709
  int op_slot_right_702 = gc_frame_newslot(strlit709);
  params[0] = strlit709;
  partcv[0] = 1;
  Object opresult711 = callmethod(opresult708, "++", 1, partcv, params);
// compilenode returning opresult711
  int op_slot_left_701 = gc_frame_newslot(opresult711);
// compilenode returning *var_fileName
  int op_slot_right_701 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult713 = callmethod(opresult711, "++", 1, partcv, params);
// compilenode returning opresult713
  gc_frame_newslot(opresult713);
  if (strlit714 == NULL) {
    strlit714 = alloc_String("r");
    gc_root(strlit714);
  }
// compilenode returning strlit714
  gc_frame_newslot(strlit714);
// compilenode returning *var_io
  params[0] = opresult713;
  params[1] = strlit714;
  partcv[0] = 2;
  Object call715 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe700);
// compilenode returning call715
  partcv[0] = 0;
  Object call716 = callmethod(call715, "read",
    1, partcv, params);
  gc_frame_end(callframe699);
// compilenode returning call716
// compilenode returning call716
  return call716;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if683 = undefined;
  } else {
  }
// compilenode returning if683
// Begin line 250
  setline(250);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 246
  setline(246);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe718 = gc_frame_new();
  if (strlit723 == NULL) {
    strlit723 = alloc_String("");
    gc_root(strlit723);
  }
// compilenode returning strlit723
  int op_slot_left_722 = gc_frame_newslot(strlit723);
  int callframe724 = gc_frame_new();
  partcv[0] = 0;
  Object call725 = callmethodflags(self, "getBuildPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe724);
// compilenode returning call725
  int op_slot_right_722 = gc_frame_newslot(call725);
  params[0] = call725;
  partcv[0] = 1;
  Object opresult727 = callmethod(strlit723, "++", 1, partcv, params);
// compilenode returning opresult727
  int op_slot_left_721 = gc_frame_newslot(opresult727);
  if (strlit728 == NULL) {
    strlit728 = alloc_String("/");
    gc_root(strlit728);
  }
// compilenode returning strlit728
  int op_slot_right_721 = gc_frame_newslot(strlit728);
  params[0] = strlit728;
  partcv[0] = 1;
  Object opresult730 = callmethod(opresult727, "++", 1, partcv, params);
// compilenode returning opresult730
  int op_slot_left_720 = gc_frame_newslot(opresult730);
// compilenode returning *var_fileName
  int op_slot_right_720 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult732 = callmethod(opresult730, "++", 1, partcv, params);
// compilenode returning opresult732
  int op_slot_left_719 = gc_frame_newslot(opresult732);
  if (strlit733 == NULL) {
    strlit733 = alloc_String("");
    gc_root(strlit733);
  }
// compilenode returning strlit733
  int op_slot_right_719 = gc_frame_newslot(strlit733);
  params[0] = strlit733;
  partcv[0] = 1;
  Object opresult735 = callmethod(opresult732, "++", 1, partcv, params);
// compilenode returning opresult735
  gc_frame_newslot(opresult735);
// compilenode returning *var_io
  params[0] = opresult735;
  partcv[0] = 1;
  Object call736 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe718);
// compilenode returning call736
  Object if717 = done;
  if (istrue(call736)) {
// Begin line 248
  setline(248);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 247
  setline(247);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit737 == NULL) {
    strlit737 = alloc_String("YES IT DOES");
    gc_root(strlit737);
  }
// compilenode returning strlit737
  params[0] = strlit737;
  Object call738 = gracelib_print(NULL, 1,  params);
// compilenode returning call738
// Begin line 249
  setline(249);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 248
  setline(248);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe739 = gc_frame_new();
  int callframe740 = gc_frame_new();
  if (strlit745 == NULL) {
    strlit745 = alloc_String("");
    gc_root(strlit745);
  }
// compilenode returning strlit745
  int op_slot_left_744 = gc_frame_newslot(strlit745);
  int callframe746 = gc_frame_new();
  partcv[0] = 0;
  Object call747 = callmethodflags(self, "getBuildPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe746);
// compilenode returning call747
  int op_slot_right_744 = gc_frame_newslot(call747);
  params[0] = call747;
  partcv[0] = 1;
  Object opresult749 = callmethod(strlit745, "++", 1, partcv, params);
// compilenode returning opresult749
  int op_slot_left_743 = gc_frame_newslot(opresult749);
  if (strlit750 == NULL) {
    strlit750 = alloc_String("/");
    gc_root(strlit750);
  }
// compilenode returning strlit750
  int op_slot_right_743 = gc_frame_newslot(strlit750);
  params[0] = strlit750;
  partcv[0] = 1;
  Object opresult752 = callmethod(opresult749, "++", 1, partcv, params);
// compilenode returning opresult752
  int op_slot_left_742 = gc_frame_newslot(opresult752);
// compilenode returning *var_fileName
  int op_slot_right_742 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult754 = callmethod(opresult752, "++", 1, partcv, params);
// compilenode returning opresult754
  int op_slot_left_741 = gc_frame_newslot(opresult754);
  if (strlit755 == NULL) {
    strlit755 = alloc_String("");
    gc_root(strlit755);
  }
// compilenode returning strlit755
  int op_slot_right_741 = gc_frame_newslot(strlit755);
  params[0] = strlit755;
  partcv[0] = 1;
  Object opresult757 = callmethod(opresult754, "++", 1, partcv, params);
// compilenode returning opresult757
  gc_frame_newslot(opresult757);
  if (strlit758 == NULL) {
    strlit758 = alloc_String("r");
    gc_root(strlit758);
  }
// compilenode returning strlit758
  gc_frame_newslot(strlit758);
// compilenode returning *var_io
  params[0] = opresult757;
  params[1] = strlit758;
  partcv[0] = 2;
  Object call759 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe740);
// compilenode returning call759
  partcv[0] = 0;
  Object call760 = callmethod(call759, "read",
    1, partcv, params);
  gc_frame_end(callframe739);
// compilenode returning call760
// compilenode returning call760
  return call760;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if717 = undefined;
  } else {
  }
// compilenode returning if717
// Begin line 254
  setline(254);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 250
  setline(250);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_bundlePath
  int op_slot_left_762 = gc_frame_newslot(*var_bundlePath);
  if (strlit763 == NULL) {
    strlit763 = alloc_String("");
    gc_root(strlit763);
  }
// compilenode returning strlit763
  int op_slot_right_762 = gc_frame_newslot(strlit763);
  params[0] = strlit763;
  partcv[0] = 1;
  Object opresult765 = callmethod(*var_bundlePath, "!=", 1, partcv, params);
// compilenode returning opresult765
  Object if761 = done;
  if (istrue(opresult765)) {
// Begin line 252
  setline(252);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 251
  setline(251);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe766 = gc_frame_new();
  int callframe767 = gc_frame_new();
  if (strlit772 == NULL) {
    strlit772 = alloc_String("");
    gc_root(strlit772);
  }
// compilenode returning strlit772
  int op_slot_left_771 = gc_frame_newslot(strlit772);
// compilenode returning *var_bundlePath
  int op_slot_right_771 = gc_frame_newslot(*var_bundlePath);
  params[0] = *var_bundlePath;
  partcv[0] = 1;
  Object opresult774 = callmethod(strlit772, "++", 1, partcv, params);
// compilenode returning opresult774
  int op_slot_left_770 = gc_frame_newslot(opresult774);
  if (strlit775 == NULL) {
    strlit775 = alloc_String("/");
    gc_root(strlit775);
  }
// compilenode returning strlit775
  int op_slot_right_770 = gc_frame_newslot(strlit775);
  params[0] = strlit775;
  partcv[0] = 1;
  Object opresult777 = callmethod(opresult774, "++", 1, partcv, params);
// compilenode returning opresult777
  int op_slot_left_769 = gc_frame_newslot(opresult777);
// compilenode returning *var_fileName
  int op_slot_right_769 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult779 = callmethod(opresult777, "++", 1, partcv, params);
// compilenode returning opresult779
  int op_slot_left_768 = gc_frame_newslot(opresult779);
  if (strlit780 == NULL) {
    strlit780 = alloc_String("");
    gc_root(strlit780);
  }
// compilenode returning strlit780
  int op_slot_right_768 = gc_frame_newslot(strlit780);
  params[0] = strlit780;
  partcv[0] = 1;
  Object opresult782 = callmethod(opresult779, "++", 1, partcv, params);
// compilenode returning opresult782
  gc_frame_newslot(opresult782);
  if (strlit783 == NULL) {
    strlit783 = alloc_String("r");
    gc_root(strlit783);
  }
// compilenode returning strlit783
  gc_frame_newslot(strlit783);
// compilenode returning *var_io
  params[0] = opresult782;
  params[1] = strlit783;
  partcv[0] = 2;
  Object call784 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe767);
// compilenode returning call784
  partcv[0] = 0;
  Object call785 = callmethod(call784, "read",
    1, partcv, params);
  gc_frame_end(callframe766);
// compilenode returning call785
// compilenode returning call785
  return call785;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if761 = undefined;
  } else {
  }
// compilenode returning if761
// Begin line 255
  setline(255);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 254
  setline(254);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool786 = alloc_Boolean(0);
// compilenode returning bool786
  return bool786;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_grace_45_pkg_validateFile787(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 264
  setline(264);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 258
  setline(258);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe790 = gc_frame_new();
  int callframe791 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call792 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe791);
// compilenode returning call792
// compilenode returning call792
  partcv[0] = 0;
  Object call793 = callmethod(call792, "size",
    1, partcv, params);
  gc_frame_end(callframe790);
// compilenode returning call793
// compilenode returning call793
  int op_slot_left_789 = gc_frame_newslot(call793);
  Object num794 = alloc_Float64(1);
// compilenode returning num794
  int op_slot_right_789 = gc_frame_newslot(num794);
  params[0] = num794;
  partcv[0] = 1;
  Object opresult796 = callmethod(call793, ">", 1, partcv, params);
// compilenode returning opresult796
  Object if788 = done;
  if (istrue(opresult796)) {
// Begin line 263
  setline(263);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 259
  setline(259);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe799 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call800 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe799);
// compilenode returning call800
// compilenode returning call800
  Object num801 = alloc_Float64(1);
// compilenode returning num801
  params[0] = num801;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres802 = callmethod(call800, "[]", 1, partcv, params);
// compilenode returning idxres802
  int op_slot_left_798 = gc_frame_newslot(idxres802);
  if (strlit803 == NULL) {
    strlit803 = alloc_String("<");
    gc_root(strlit803);
  }
// compilenode returning strlit803
  int op_slot_right_798 = gc_frame_newslot(strlit803);
  params[0] = strlit803;
  partcv[0] = 1;
  Object opresult805 = callmethod(idxres802, "==", 1, partcv, params);
// compilenode returning opresult805
  Object if797 = done;
  if (istrue(opresult805)) {
// Begin line 261
  setline(261);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 260
  setline(260);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit806 == NULL) {
    strlit806 = alloc_String("Not a valid grace file");
    gc_root(strlit806);
  }
// compilenode returning strlit806
  params[0] = strlit806;
  Object call807 = gracelib_print(NULL, 1,  params);
// compilenode returning call807
// Begin line 262
  setline(262);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 261
  setline(261);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool808 = alloc_Boolean(0);
// compilenode returning bool808
  return bool808;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if797 = undefined;
  } else {
  }
// compilenode returning if797
    gc_frame_newslot(if797);
    if788 = if797;
  } else {
  }
// compilenode returning if788
// Begin line 265
  setline(265);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 264
  setline(264);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool809 = alloc_Boolean(1);
// compilenode returning bool809
  return bool809;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_grace_45_pkg_write810(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 269
  setline(269);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 268
  setline(268);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit811 == NULL) {
    strlit811 = alloc_String("");
    gc_root(strlit811);
  }
// compilenode returning strlit811
  *var_usrDir = strlit811;
  if (strlit811 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 275
  setline(275);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 269
  setline(269);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_global
  Object if812 = done;
  if (istrue(*var_global)) {
// Begin line 271
  setline(271);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 270
  setline(270);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit813 == NULL) {
    strlit813 = alloc_String("usr/lib/grace/modules/");
    gc_root(strlit813);
  }
// compilenode returning strlit813
  *var_usrDir = strlit813;
  if (strlit813 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if812 = done;
  } else {
// Begin line 274
  setline(274);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 273
  setline(273);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe816 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call817 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe816);
// compilenode returning call817
// compilenode returning call817
  if (strlit818 == NULL) {
    strlit818 = alloc_String("HOME");
    gc_root(strlit818);
  }
// compilenode returning strlit818
  params[0] = strlit818;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres819 = callmethod(call817, "[]", 1, partcv, params);
// compilenode returning idxres819
  int op_slot_left_815 = gc_frame_newslot(idxres819);
  if (strlit820 == NULL) {
    strlit820 = alloc_String("/.local/lib/grace/modules/");
    gc_root(strlit820);
  }
// compilenode returning strlit820
  int op_slot_right_815 = gc_frame_newslot(strlit820);
  params[0] = strlit820;
  partcv[0] = 1;
  Object opresult822 = callmethod(idxres819, "++", 1, partcv, params);
// compilenode returning opresult822
  *var_usrDir = opresult822;
  if (opresult822 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if812 = done;
  }
// compilenode returning if812
// Begin line 276
  setline(276);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe824 = gc_frame_new();
// Begin line 275
  setline(275);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_usrDir
  int op_slot_left_825 = gc_frame_newslot(*var_usrDir);
  int callframe826 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call827 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe826);
// compilenode returning call827
// compilenode returning call827
  int op_slot_right_825 = gc_frame_newslot(call827);
  params[0] = call827;
  partcv[0] = 1;
  Object opresult829 = callmethod(*var_usrDir, "++", 1, partcv, params);
// compilenode returning opresult829
  gc_frame_newslot(opresult829);
  params[0] = opresult829;
  partcv[0] = 1;
  Object call830 = callmethodflags(self, "createDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe824);
// compilenode returning call830
  *var_fileDir = call830;
  if (call830 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 277
  setline(277);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe831 = gc_frame_new();
// Begin line 276
  setline(276);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_fileDir
  Object num833 = alloc_Float64(1);
// compilenode returning num833
  params[0] = num833;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres834 = callmethod(*var_fileDir, "[]", 1, partcv, params);
// compilenode returning idxres834
  int op_slot_left_832 = gc_frame_newslot(idxres834);
// compilenode returning *var_fileDir
  Object num835 = alloc_Float64(2);
// compilenode returning num835
  params[0] = num835;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres836 = callmethod(*var_fileDir, "[]", 1, partcv, params);
// compilenode returning idxres836
  int op_slot_right_832 = gc_frame_newslot(idxres836);
  params[0] = idxres836;
  partcv[0] = 1;
  Object opresult838 = callmethod(idxres834, "++", 1, partcv, params);
// compilenode returning opresult838
  gc_frame_newslot(opresult838);
  if (strlit839 == NULL) {
    strlit839 = alloc_String("w");
    gc_root(strlit839);
  }
// compilenode returning strlit839
  gc_frame_newslot(strlit839);
// compilenode returning *var_io
  params[0] = opresult838;
  params[1] = strlit839;
  partcv[0] = 2;
  Object call840 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe831);
// compilenode returning call840
  *var_toWrite = call840;
  if (call840 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 278
  setline(278);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe841 = gc_frame_new();
// Begin line 277
  setline(277);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe842 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call843 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe842);
// compilenode returning call843
// compilenode returning call843
  gc_frame_newslot(call843);
// compilenode returning *var_toWrite
  params[0] = call843;
  partcv[0] = 1;
  Object call844 = callmethod(*var_toWrite, "write",
    1, partcv, params);
  gc_frame_end(callframe841);
// compilenode returning call844
// Begin line 278
  setline(278);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe845 = gc_frame_new();
// compilenode returning *var_toWrite
  partcv[0] = 0;
  Object call846 = callmethod(*var_toWrite, "close",
    1, partcv, params);
  gc_frame_end(callframe845);
// compilenode returning call846
// compilenode returning call846
  gc_frame_end(frame);
  return call846;
}
Object meth_grace_45_pkg__apply861(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 295
  setline(295);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 286
  setline(286);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe864 = gc_frame_new();
// compilenode returning *var_t
  gc_frame_newslot(*var_t);
// compilenode returning *var_paths
  params[0] = *var_t;
  partcv[0] = 1;
  Object call865 = callmethod(*var_paths, "at",
    1, partcv, params);
  gc_frame_end(callframe864);
// compilenode returning call865
  int op_slot_left_863 = gc_frame_newslot(call865);
  if (strlit866 == NULL) {
    strlit866 = alloc_String(":");
    gc_root(strlit866);
  }
// compilenode returning strlit866
  int op_slot_right_863 = gc_frame_newslot(strlit866);
  params[0] = strlit866;
  partcv[0] = 1;
  Object opresult868 = callmethod(call865, "==", 1, partcv, params);
// compilenode returning opresult868
  Object if862 = done;
  if (istrue(opresult868)) {
// Begin line 290
  setline(290);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 287
  setline(287);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe870 = gc_frame_new();
// compilenode returning *var_buildPath
  int op_slot_left_871 = gc_frame_newslot(*var_buildPath);
  if (strlit872 == NULL) {
    strlit872 = alloc_String("/minigrace");
    gc_root(strlit872);
  }
// compilenode returning strlit872
  int op_slot_right_871 = gc_frame_newslot(strlit872);
  params[0] = strlit872;
  partcv[0] = 1;
  Object opresult874 = callmethod(*var_buildPath, "++", 1, partcv, params);
// compilenode returning opresult874
  gc_frame_newslot(opresult874);
// compilenode returning *var_io
  params[0] = opresult874;
  partcv[0] = 1;
  Object call875 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe870);
// compilenode returning call875
  Object if869 = done;
  if (istrue(call875)) {
// Begin line 288
  setline(288);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  block_return(realself, *var_buildPath);
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if869 = undefined;
  } else {
  }
// compilenode returning if869
// Begin line 291
  setline(291);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 290
  setline(290);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit876 == NULL) {
    strlit876 = alloc_String("");
    gc_root(strlit876);
  }
// compilenode returning strlit876
  *var_buildPath = strlit876;
  if (strlit876 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if862 = done;
  } else {
// Begin line 294
  setline(294);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 293
  setline(293);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  int op_slot_left_878 = gc_frame_newslot(*var_buildPath);
// Begin line 294
  setline(294);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 293
  setline(293);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_paths
// compilenode returning *var_t
  params[0] = *var_t;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres879 = callmethod(*var_paths, "[]", 1, partcv, params);
// compilenode returning idxres879
  int op_slot_right_878 = gc_frame_newslot(idxres879);
  params[0] = idxres879;
  partcv[0] = 1;
  Object opresult881 = callmethod(*var_buildPath, "++", 1, partcv, params);
// compilenode returning opresult881
  *var_buildPath = opresult881;
  if (opresult881 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if862 = done;
  }
// compilenode returning if862
  gc_frame_end(frame);
  return if862;
}
Object meth_grace_45_pkg_getBuildPath847(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 284
  setline(284);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 283
  setline(283);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe848 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call849 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe848);
// compilenode returning call849
// compilenode returning call849
  if (strlit850 == NULL) {
    strlit850 = alloc_String("PATH");
    gc_root(strlit850);
  }
// compilenode returning strlit850
  params[0] = strlit850;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres851 = callmethod(call849, "[]", 1, partcv, params);
// compilenode returning idxres851
  *var_paths = idxres851;
  if (idxres851 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 285
  setline(285);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 284
  setline(284);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit852 == NULL) {
    strlit852 = alloc_String("");
    gc_root(strlit852);
  }
// compilenode returning strlit852
  *var_buildPath = strlit852;
  if (strlit852 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 296
  setline(296);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe853 = gc_frame_new();
// Begin line 285
  setline(285);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num855 = alloc_Float64(1);
// compilenode returning num855
  int op_slot_left_854 = gc_frame_newslot(num855);
  int callframe856 = gc_frame_new();
// compilenode returning *var_paths
  partcv[0] = 0;
  Object call857 = callmethod(*var_paths, "size",
    1, partcv, params);
  gc_frame_end(callframe856);
// compilenode returning call857
// compilenode returning call857
  int op_slot_right_854 = gc_frame_newslot(call857);
  params[0] = call857;
  partcv[0] = 1;
  Object opresult859 = callmethod(num855, "..", 1, partcv, params);
// compilenode returning opresult859
  gc_frame_newslot(opresult859);
// Begin line 296
  setline(296);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block860 = alloc_Block(NULL, NULL, "grace-pkg", 296);
  gc_frame_newslot(block860);
  block_savedest(block860);
  Object closure861 = createclosure(4, "_apply");
setclosureframe(closure861, stackframe);
  addtoclosure(closure861, var_paths);
  addtoclosure(closure861, var_buildPath);
  addtoclosure(closure861, var_io);
  addtoclosure(closure861, selfslot);
  struct UserObject *uo861 = (struct UserObject*)block860;
  uo861->data[0] = (Object)closure861;
  Method *meth_meth_grace_45_pkg__apply861 = addmethod2pos(block860, "_apply", &meth_grace_45_pkg__apply861, 0);
int argcv_meth_grace_45_pkg__apply861[] = {1};
meth_meth_grace_45_pkg__apply861->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply861);
  meth_meth_grace_45_pkg__apply861->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply861->definitionLine = 285;
// compilenode returning block860
  gc_frame_newslot(block860);
  params[0] = opresult859;
  params[1] = block860;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call884 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe853);
// compilenode returning call884
// Begin line 297
  setline(297);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 296
  setline(296);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  return *var_buildPath;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_grace_45_pkg_compile885(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 301
  setline(301);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 300
  setline(300);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit886 == NULL) {
    strlit886 = alloc_String("");
    gc_root(strlit886);
  }
// compilenode returning strlit886
  *var_usrDir = strlit886;
  if (strlit886 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 307
  setline(307);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 301
  setline(301);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_global
  Object if887 = done;
  if (istrue(*var_global)) {
// Begin line 303
  setline(303);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 302
  setline(302);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit888 == NULL) {
    strlit888 = alloc_String("usr/lib/grace/modules/");
    gc_root(strlit888);
  }
// compilenode returning strlit888
  *var_usrDir = strlit888;
  if (strlit888 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if887 = done;
  } else {
// Begin line 306
  setline(306);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 305
  setline(305);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe891 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call892 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe891);
// compilenode returning call892
// compilenode returning call892
  if (strlit893 == NULL) {
    strlit893 = alloc_String("HOME");
    gc_root(strlit893);
  }
// compilenode returning strlit893
  params[0] = strlit893;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres894 = callmethod(call892, "[]", 1, partcv, params);
// compilenode returning idxres894
  int op_slot_left_890 = gc_frame_newslot(idxres894);
  if (strlit895 == NULL) {
    strlit895 = alloc_String("/.local/lib/grace/modules/");
    gc_root(strlit895);
  }
// compilenode returning strlit895
  int op_slot_right_890 = gc_frame_newslot(strlit895);
  params[0] = strlit895;
  partcv[0] = 1;
  Object opresult897 = callmethod(idxres894, "++", 1, partcv, params);
// compilenode returning opresult897
  *var_usrDir = opresult897;
  if (opresult897 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if887 = done;
  }
// compilenode returning if887
// Begin line 308
  setline(308);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 307
  setline(307);
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
    strlit901 = alloc_String("PATH");
    gc_root(strlit901);
  }
// compilenode returning strlit901
  params[0] = strlit901;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres902 = callmethod(call900, "[]", 1, partcv, params);
// compilenode returning idxres902
  *var_paths = idxres902;
  if (idxres902 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 309
  setline(309);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 308
  setline(308);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit904 == NULL) {
    strlit904 = alloc_String("PATHS = ");
    gc_root(strlit904);
  }
// compilenode returning strlit904
  int op_slot_left_903 = gc_frame_newslot(strlit904);
// compilenode returning *var_paths
  int op_slot_right_903 = gc_frame_newslot(*var_paths);
  params[0] = *var_paths;
  partcv[0] = 1;
  Object opresult906 = callmethod(strlit904, "++", 1, partcv, params);
// compilenode returning opresult906
  params[0] = opresult906;
  Object call907 = gracelib_print(NULL, 1,  params);
// compilenode returning call907
// Begin line 310
  setline(310);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe908 = gc_frame_new();
  partcv[0] = 0;
  Object call909 = callmethodflags(self, "getBuildPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe908);
// compilenode returning call909
  *var_buildPath = call909;
  if (call909 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 311
  setline(311);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe910 = gc_frame_new();
// Begin line 310
  setline(310);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  int op_slot_left_913 = gc_frame_newslot(*var_buildPath);
  if (strlit914 == NULL) {
    strlit914 = alloc_String("/minigrace ");
    gc_root(strlit914);
  }
// compilenode returning strlit914
  int op_slot_right_913 = gc_frame_newslot(strlit914);
  params[0] = strlit914;
  partcv[0] = 1;
  Object opresult916 = callmethod(*var_buildPath, "++", 1, partcv, params);
// compilenode returning opresult916
  int op_slot_left_912 = gc_frame_newslot(opresult916);
// compilenode returning *var_usrDir
  int op_slot_right_912 = gc_frame_newslot(*var_usrDir);
  params[0] = *var_usrDir;
  partcv[0] = 1;
  Object opresult918 = callmethod(opresult916, "++", 1, partcv, params);
// compilenode returning opresult918
  int op_slot_left_911 = gc_frame_newslot(opresult918);
  int callframe919 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call920 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe919);
// compilenode returning call920
// compilenode returning call920
  int op_slot_right_911 = gc_frame_newslot(call920);
  params[0] = call920;
  partcv[0] = 1;
  Object opresult922 = callmethod(opresult918, "++", 1, partcv, params);
// compilenode returning opresult922
  gc_frame_newslot(opresult922);
// compilenode returning *var_io
  params[0] = opresult922;
  partcv[0] = 1;
  Object call923 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe910);
// compilenode returning call923
// Begin line 313
  setline(313);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 311
  setline(311);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool924 = alloc_Boolean(1);
// compilenode returning bool924
  return bool924;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_grace_45_pkg__apply941(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_942 = gc_frame_newslot(*var_count);
  int callframe943 = gc_frame_new();
// compilenode returning *var_address
  partcv[0] = 0;
  Object call944 = callmethod(*var_address, "size",
    1, partcv, params);
  gc_frame_end(callframe943);
// compilenode returning call944
// compilenode returning call944
  int op_slot_right_942 = gc_frame_newslot(call944);
  params[0] = call944;
  partcv[0] = 1;
  Object opresult946 = callmethod(*var_count, "<=", 1, partcv, params);
// compilenode returning opresult946
  gc_frame_end(frame);
  return opresult946;
}
Object meth_grace_45_pkg__apply949(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 323
  setline(323);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 322
  setline(322);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_nextPath
  int op_slot_left_950 = gc_frame_newslot(*var_nextPath);
// Begin line 323
  setline(323);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 322
  setline(322);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_address
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres951 = callmethod(*var_address, "[]", 1, partcv, params);
// compilenode returning idxres951
  int op_slot_right_950 = gc_frame_newslot(idxres951);
  params[0] = idxres951;
  partcv[0] = 1;
  Object opresult953 = callmethod(*var_nextPath, "++", 1, partcv, params);
// compilenode returning opresult953
  *var_nextPath = opresult953;
  if (opresult953 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 327
  setline(327);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 323
  setline(323);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_address
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres957 = callmethod(*var_address, "[]", 1, partcv, params);
// compilenode returning idxres957
  int op_slot_left_956 = gc_frame_newslot(idxres957);
  if (strlit958 == NULL) {
    strlit958 = alloc_String("/");
    gc_root(strlit958);
  }
// compilenode returning strlit958
  int op_slot_right_956 = gc_frame_newslot(strlit958);
  params[0] = strlit958;
  partcv[0] = 1;
  Object opresult960 = callmethod(idxres957, "==", 1, partcv, params);
// compilenode returning opresult960
  Object if955 = done;
  if (istrue(opresult960)) {
// Begin line 325
  setline(325);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 324
  setline(324);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toMake
  int op_slot_left_961 = gc_frame_newslot(*var_toMake);
// compilenode returning *var_nextPath
  int op_slot_right_961 = gc_frame_newslot(*var_nextPath);
  params[0] = *var_nextPath;
  partcv[0] = 1;
  Object opresult963 = callmethod(*var_toMake, "++", 1, partcv, params);
// compilenode returning opresult963
  *var_toMake = opresult963;
  if (opresult963 == undefined)
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
  if (strlit965 == NULL) {
    strlit965 = alloc_String("");
    gc_root(strlit965);
  }
// compilenode returning strlit965
  *var_nextPath = strlit965;
  if (strlit965 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if955 = done;
  } else {
  }
// compilenode returning if955
// Begin line 328
  setline(328);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 327
  setline(327);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  int op_slot_left_967 = gc_frame_newslot(*var_count);
  Object num968 = alloc_Float64(1);
// compilenode returning num968
  int op_slot_right_967 = gc_frame_newslot(num968);
  params[0] = num968;
  partcv[0] = 1;
  Object sum970 = callmethod(*var_count, "+", 1, partcv, params);
// compilenode returning sum970
  *var_count = sum970;
  if (sum970 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_grace_45_pkg_createDirectory925(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 317
  setline(317);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 316
  setline(316);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array926 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array926
  *var_dir = array926;
  if (array926 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 318
  setline(318);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 317
  setline(317);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit927 == NULL) {
    strlit927 = alloc_String("");
    gc_root(strlit927);
  }
// compilenode returning strlit927
  *var_nextPath = strlit927;
  if (strlit927 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 319
  setline(319);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 318
  setline(318);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit928 == NULL) {
    strlit928 = alloc_String("");
    gc_root(strlit928);
  }
// compilenode returning strlit928
  *var_toMake = strlit928;
  if (strlit928 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 320
  setline(320);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 319
  setline(319);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num929 = alloc_Float64(1);
// compilenode returning num929
  *var_count = num929;
  if (num929 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 321
  setline(321);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 320
  setline(320);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit932 == NULL) {
    strlit932 = alloc_String("Directory being evaluated ");
    gc_root(strlit932);
  }
// compilenode returning strlit932
  int op_slot_left_931 = gc_frame_newslot(strlit932);
// compilenode returning *var_address
  int op_slot_right_931 = gc_frame_newslot(*var_address);
  params[0] = *var_address;
  partcv[0] = 1;
  Object opresult934 = callmethod(strlit932, "++", 1, partcv, params);
// compilenode returning opresult934
  int op_slot_left_930 = gc_frame_newslot(opresult934);
  if (strlit935 == NULL) {
    strlit935 = alloc_String("");
    gc_root(strlit935);
  }
// compilenode returning strlit935
  int op_slot_right_930 = gc_frame_newslot(strlit935);
  params[0] = strlit935;
  partcv[0] = 1;
  Object opresult937 = callmethod(opresult934, "++", 1, partcv, params);
// compilenode returning opresult937
  params[0] = opresult937;
  Object call938 = gracelib_print(NULL, 1,  params);
// compilenode returning call938
// Begin line 329
  setline(329);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe939 = gc_frame_new();
// Begin line 321
  setline(321);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block940 = alloc_Block(NULL, NULL, "grace-pkg", 321);
  gc_frame_newslot(block940);
  block_savedest(block940);
  Object closure941 = createclosure(3, "_apply");
setclosureframe(closure941, stackframe);
  addtoclosure(closure941, var_count);
  addtoclosure(closure941, var_address);
  addtoclosure(closure941, selfslot);
  struct UserObject *uo941 = (struct UserObject*)block940;
  uo941->data[0] = (Object)closure941;
  Method *meth_meth_grace_45_pkg__apply941 = addmethod2pos(block940, "_apply", &meth_grace_45_pkg__apply941, 0);
int argcv_meth_grace_45_pkg__apply941[] = {0};
meth_meth_grace_45_pkg__apply941->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply941);
  meth_meth_grace_45_pkg__apply941->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply941->definitionLine = 310;
// compilenode returning block940
  gc_frame_newslot(block940);
// Begin line 329
  setline(329);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block948 = alloc_Block(NULL, NULL, "grace-pkg", 329);
  gc_frame_newslot(block948);
  block_savedest(block948);
  Object closure949 = createclosure(5, "_apply");
setclosureframe(closure949, stackframe);
  addtoclosure(closure949, var_nextPath);
  addtoclosure(closure949, var_address);
  addtoclosure(closure949, var_count);
  addtoclosure(closure949, var_toMake);
  addtoclosure(closure949, selfslot);
  struct UserObject *uo949 = (struct UserObject*)block948;
  uo949->data[0] = (Object)closure949;
  Method *meth_meth_grace_45_pkg__apply949 = addmethod2pos(block948, "_apply", &meth_grace_45_pkg__apply949, 0);
int argcv_meth_grace_45_pkg__apply949[] = {0};
meth_meth_grace_45_pkg__apply949->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply949);
  meth_meth_grace_45_pkg__apply949->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply949->definitionLine = 321;
// compilenode returning block948
  gc_frame_newslot(block948);
  params[0] = block940;
  params[1] = block948;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call973 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe939);
// compilenode returning call973
// Begin line 330
  setline(330);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 329
  setline(329);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit975 == NULL) {
    strlit975 = alloc_String("Creating directory ");
    gc_root(strlit975);
  }
// compilenode returning strlit975
  int op_slot_left_974 = gc_frame_newslot(strlit975);
// compilenode returning *var_toMake
  int op_slot_right_974 = gc_frame_newslot(*var_toMake);
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object opresult977 = callmethod(strlit975, "++", 1, partcv, params);
// compilenode returning opresult977
  params[0] = opresult977;
  Object call978 = gracelib_print(NULL, 1,  params);
// compilenode returning call978
// Begin line 334
  setline(334);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 330
  setline(330);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe980 = gc_frame_new();
  int callframe981 = gc_frame_new();
// compilenode returning *var_toMake
  gc_frame_newslot(*var_toMake);
// compilenode returning *var_io
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object call982 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe981);
// compilenode returning call982
  partcv[0] = 0;
  Object call983 = callmethod(call982, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe980);
// compilenode returning call983
  Object if979 = done;
  if (istrue(call983)) {
// Begin line 332
  setline(332);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe984 = gc_frame_new();
// Begin line 331
  setline(331);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit986 == NULL) {
    strlit986 = alloc_String("mkdir -p ");
    gc_root(strlit986);
  }
// compilenode returning strlit986
  int op_slot_left_985 = gc_frame_newslot(strlit986);
// compilenode returning *var_toMake
  int op_slot_right_985 = gc_frame_newslot(*var_toMake);
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object opresult988 = callmethod(strlit986, "++", 1, partcv, params);
// compilenode returning opresult988
  gc_frame_newslot(opresult988);
// compilenode returning *var_io
  params[0] = opresult988;
  partcv[0] = 1;
  Object call989 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe984);
// compilenode returning call989
// Begin line 333
  setline(333);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 332
  setline(332);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit992 == NULL) {
    strlit992 = alloc_String("made directory ");
    gc_root(strlit992);
  }
// compilenode returning strlit992
  int op_slot_left_991 = gc_frame_newslot(strlit992);
// compilenode returning *var_toMake
  int op_slot_right_991 = gc_frame_newslot(*var_toMake);
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object opresult994 = callmethod(strlit992, "++", 1, partcv, params);
// compilenode returning opresult994
  int op_slot_left_990 = gc_frame_newslot(opresult994);
  if (strlit995 == NULL) {
    strlit995 = alloc_String("");
    gc_root(strlit995);
  }
// compilenode returning strlit995
  int op_slot_right_990 = gc_frame_newslot(strlit995);
  params[0] = strlit995;
  partcv[0] = 1;
  Object opresult997 = callmethod(opresult994, "++", 1, partcv, params);
// compilenode returning opresult997
  params[0] = opresult997;
  Object call998 = gracelib_print(NULL, 1,  params);
// compilenode returning call998
    gc_frame_newslot(call998);
    if979 = call998;
  } else {
  }
// compilenode returning if979
// Begin line 335
  setline(335);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe999 = gc_frame_new();
// Begin line 334
  setline(334);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toMake
  gc_frame_newslot(*var_toMake);
// compilenode returning *var_dir
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object call1000 = callmethod(*var_dir, "push",
    1, partcv, params);
  gc_frame_end(callframe999);
// compilenode returning call1000
// Begin line 336
  setline(336);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1001 = gc_frame_new();
// Begin line 335
  setline(335);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_nextPath
  gc_frame_newslot(*var_nextPath);
// compilenode returning *var_dir
  params[0] = *var_nextPath;
  partcv[0] = 1;
  Object call1002 = callmethod(*var_dir, "push",
    1, partcv, params);
  gc_frame_end(callframe1001);
// compilenode returning call1002
// Begin line 337
  setline(337);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 336
  setline(336);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_dir
  return *var_dir;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_grace_45_pkg__apply1009(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1010 = gc_frame_newslot(*var_curPos);
  int callframe1011 = gc_frame_new();
// compilenode returning *var_data
  partcv[0] = 0;
  Object call1012 = callmethod(*var_data, "size",
    1, partcv, params);
  gc_frame_end(callframe1011);
// compilenode returning call1012
// compilenode returning call1012
  int op_slot_right_1010 = gc_frame_newslot(call1012);
  params[0] = call1012;
  partcv[0] = 1;
  Object opresult1014 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1014
  gc_frame_end(frame);
  return opresult1014;
}
Object meth_grace_45_pkg__apply1021(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1023 = gc_frame_newslot(*var_curPos);
  int callframe1024 = gc_frame_new();
// compilenode returning *var_data
  partcv[0] = 0;
  Object call1025 = callmethod(*var_data, "size",
    1, partcv, params);
  gc_frame_end(callframe1024);
// compilenode returning call1025
// compilenode returning call1025
  int op_slot_right_1023 = gc_frame_newslot(call1025);
  params[0] = call1025;
  partcv[0] = 1;
  Object opresult1027 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1027
  int op_slot_left_1022 = gc_frame_newslot(opresult1027);
// compilenode returning *var_data
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1029 = callmethod(*var_data, "[]", 1, partcv, params);
// compilenode returning idxres1029
  int op_slot_left_1028 = gc_frame_newslot(idxres1029);
  if (strlit1030 == NULL) {
    strlit1030 = alloc_String("""\x0a""");
    gc_root(strlit1030);
  }
// compilenode returning strlit1030
  int op_slot_right_1028 = gc_frame_newslot(strlit1030);
  params[0] = strlit1030;
  partcv[0] = 1;
  Object opresult1032 = callmethod(idxres1029, "!=", 1, partcv, params);
// compilenode returning opresult1032
  int op_slot_right_1022 = gc_frame_newslot(opresult1032);
  params[0] = opresult1032;
  partcv[0] = 1;
  Object opresult1034 = callmethod(opresult1027, "&&", 1, partcv, params);
// compilenode returning opresult1034
  gc_frame_end(frame);
  return opresult1034;
}
Object meth_grace_45_pkg__apply1037(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 347
  setline(347);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 346
  setline(346);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1038 = gc_frame_newslot(*var_curPos);
  Object num1039 = alloc_Float64(1);
// compilenode returning num1039
  int op_slot_right_1038 = gc_frame_newslot(num1039);
  params[0] = num1039;
  partcv[0] = 1;
  Object sum1041 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1041
  *var_curPos = sum1041;
  if (sum1041 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_grace_45_pkg__apply1017(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 345
  setline(345);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 344
  setline(344);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 348
  setline(348);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1019 = gc_frame_new();
// Begin line 345
  setline(345);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1020 = alloc_Block(NULL, NULL, "grace-pkg", 345);
  gc_frame_newslot(block1020);
  block_savedest(block1020);
  Object closure1021 = createclosure(3, "_apply");
setclosureframe(closure1021, stackframe);
  addtoclosure(closure1021, var_curPos);
  addtoclosure(closure1021, var_data);
  addtoclosure(closure1021, selfslot);
  struct UserObject *uo1021 = (struct UserObject*)block1020;
  uo1021->data[0] = (Object)closure1021;
  Method *meth_meth_grace_45_pkg__apply1021 = addmethod2pos(block1020, "_apply", &meth_grace_45_pkg__apply1021, 0);
int argcv_meth_grace_45_pkg__apply1021[] = {0};
meth_meth_grace_45_pkg__apply1021->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1021);
  meth_meth_grace_45_pkg__apply1021->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1021->definitionLine = 343;
// compilenode returning block1020
  gc_frame_newslot(block1020);
// Begin line 348
  setline(348);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1036 = alloc_Block(NULL, NULL, "grace-pkg", 348);
  gc_frame_newslot(block1036);
  block_savedest(block1036);
  Object closure1037 = createclosure(2, "_apply");
setclosureframe(closure1037, stackframe);
  addtoclosure(closure1037, var_curPos);
  addtoclosure(closure1037, selfslot);
  struct UserObject *uo1037 = (struct UserObject*)block1036;
  uo1037->data[0] = (Object)closure1037;
  Method *meth_meth_grace_45_pkg__apply1037 = addmethod2pos(block1036, "_apply", &meth_grace_45_pkg__apply1037, 0);
int argcv_meth_grace_45_pkg__apply1037[] = {0};
meth_meth_grace_45_pkg__apply1037->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1037);
  meth_meth_grace_45_pkg__apply1037->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1037->definitionLine = 345;
// compilenode returning block1036
  gc_frame_newslot(block1036);
  params[0] = block1020;
  params[1] = block1036;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1044 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1019);
// compilenode returning call1044
// Begin line 349
  setline(349);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1045 = gc_frame_new();
// Begin line 348
  setline(348);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_startPos
  gc_frame_newslot(*var_startPos);
// compilenode returning *var_curPos
  int op_slot_left_1046 = gc_frame_newslot(*var_curPos);
  Object num1047 = alloc_Float64(1);
// compilenode returning num1047
  int op_slot_right_1046 = gc_frame_newslot(num1047);
  params[0] = num1047;
  partcv[0] = 1;
  Object diff1049 = callmethod(*var_curPos, "-", 1, partcv, params);
// compilenode returning diff1049
  gc_frame_newslot(diff1049);
// compilenode returning *var_data
  params[0] = *var_startPos;
  params[1] = diff1049;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1050 = callmethod(*var_data, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1045);
// compilenode returning call1050
  *var_line = call1050;
  if (call1050 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 352
  setline(352);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 349
  setline(349);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1052 = gc_frame_new();
  int callframe1053 = gc_frame_new();
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
  params[0] = *var_line;
  partcv[0] = 1;
  Object call1054 = callmethodflags(self, "processLine", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1053);
// compilenode returning call1054
  partcv[0] = 0;
  Object call1055 = callmethod(call1054, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1052);
// compilenode returning call1055
  Object if1051 = done;
  if (istrue(call1055)) {
// Begin line 351
  setline(351);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_done
  block_return(realself, *var_done);
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1051 = undefined;
  } else {
  }
// compilenode returning if1051
// Begin line 353
  setline(353);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 352
  setline(352);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1056 = gc_frame_newslot(*var_curPos);
  Object num1057 = alloc_Float64(1);
// compilenode returning num1057
  int op_slot_right_1056 = gc_frame_newslot(num1057);
  params[0] = num1057;
  partcv[0] = 1;
  Object sum1059 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1059
  *var_curPos = sum1059;
  if (sum1059 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_grace_45_pkg_parseFile1003(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 341
  setline(341);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 340
  setline(340);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1004 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1005 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe1004);
// compilenode returning call1005
// compilenode returning call1005
  *var_data = call1005;
  if (call1005 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 342
  setline(342);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 341
  setline(341);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1006 = alloc_Float64(1);
// compilenode returning num1006
  *var_curPos = num1006;
  if (num1006 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 343
  setline(343);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 342
  setline(342);
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
  int callframe1007 = gc_frame_new();
// Begin line 343
  setline(343);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1008 = alloc_Block(NULL, NULL, "grace-pkg", 343);
  gc_frame_newslot(block1008);
  block_savedest(block1008);
  Object closure1009 = createclosure(3, "_apply");
setclosureframe(closure1009, stackframe);
  addtoclosure(closure1009, var_curPos);
  addtoclosure(closure1009, var_data);
  addtoclosure(closure1009, selfslot);
  struct UserObject *uo1009 = (struct UserObject*)block1008;
  uo1009->data[0] = (Object)closure1009;
  Method *meth_meth_grace_45_pkg__apply1009 = addmethod2pos(block1008, "_apply", &meth_grace_45_pkg__apply1009, 0);
int argcv_meth_grace_45_pkg__apply1009[] = {0};
meth_meth_grace_45_pkg__apply1009->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1009);
  meth_meth_grace_45_pkg__apply1009->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1009->definitionLine = 340;
// compilenode returning block1008
  gc_frame_newslot(block1008);
// Begin line 354
  setline(354);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1016 = alloc_Block(NULL, NULL, "grace-pkg", 354);
  gc_frame_newslot(block1016);
  block_savedest(block1016);
  Object closure1017 = createclosure(5, "_apply");
setclosureframe(closure1017, stackframe);
  addtoclosure(closure1017, var_curPos);
  addtoclosure(closure1017, var_startPos);
  addtoclosure(closure1017, var_data);
  addtoclosure(closure1017, var_done);
  addtoclosure(closure1017, selfslot);
  struct UserObject *uo1017 = (struct UserObject*)block1016;
  uo1017->data[0] = (Object)closure1017;
  Method *meth_meth_grace_45_pkg__apply1017 = addmethod2pos(block1016, "_apply", &meth_grace_45_pkg__apply1017, 0);
int argcv_meth_grace_45_pkg__apply1017[] = {0};
meth_meth_grace_45_pkg__apply1017->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1017);
  meth_meth_grace_45_pkg__apply1017->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1017->definitionLine = 343;
// compilenode returning block1016
  gc_frame_newslot(block1016);
  params[0] = block1008;
  params[1] = block1016;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1062 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1007);
// compilenode returning call1062
  gc_frame_end(frame);
  return call1062;
}
Object meth_grace_45_pkg_processLine1063(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 372
  setline(372);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 357
  setline(357);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1066 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1067 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1066);
// compilenode returning call1067
// compilenode returning call1067
  int op_slot_left_1065 = gc_frame_newslot(call1067);
  Object num1068 = alloc_Float64(1);
// compilenode returning num1068
  int op_slot_right_1065 = gc_frame_newslot(num1068);
  params[0] = num1068;
  partcv[0] = 1;
  Object opresult1070 = callmethod(call1067, ">", 1, partcv, params);
// compilenode returning opresult1070
  Object if1064 = done;
  if (istrue(opresult1070)) {
// Begin line 371
  setline(371);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 358
  setline(358);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  Object num1073 = alloc_Float64(1);
// compilenode returning num1073
  params[0] = num1073;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1074 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1074
  int op_slot_left_1072 = gc_frame_newslot(idxres1074);
  if (strlit1075 == NULL) {
    strlit1075 = alloc_String("#");
    gc_root(strlit1075);
  }
// compilenode returning strlit1075
  int op_slot_right_1072 = gc_frame_newslot(strlit1075);
  params[0] = strlit1075;
  partcv[0] = 1;
  Object opresult1077 = callmethod(idxres1074, "==", 1, partcv, params);
// compilenode returning opresult1077
  Object if1071 = done;
  if (istrue(opresult1077)) {
// Begin line 360
  setline(360);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 359
  setline(359);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1078 = alloc_Boolean(1);
// compilenode returning bool1078
  return bool1078;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1071 = undefined;
  } else {
// Begin line 364
  setline(364);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 361
  setline(361);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1082 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1083 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1082);
// compilenode returning call1083
// compilenode returning call1083
  int op_slot_left_1081 = gc_frame_newslot(call1083);
  Object num1084 = alloc_Float64(2);
// compilenode returning num1084
  int op_slot_right_1081 = gc_frame_newslot(num1084);
  params[0] = num1084;
  partcv[0] = 1;
  Object opresult1086 = callmethod(call1083, ">", 1, partcv, params);
// compilenode returning opresult1086
  int op_slot_left_1080 = gc_frame_newslot(opresult1086);
  int callframe1088 = gc_frame_new();
  Object num1089 = alloc_Float64(1);
// compilenode returning num1089
  gc_frame_newslot(num1089);
  Object num1090 = alloc_Float64(2);
// compilenode returning num1090
  gc_frame_newslot(num1090);
// compilenode returning *var_line
  params[0] = num1089;
  params[1] = num1090;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1091 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1088);
// compilenode returning call1091
  int op_slot_left_1087 = gc_frame_newslot(call1091);
  if (strlit1092 == NULL) {
    strlit1092 = alloc_String("//");
    gc_root(strlit1092);
  }
// compilenode returning strlit1092
  int op_slot_right_1087 = gc_frame_newslot(strlit1092);
  params[0] = strlit1092;
  partcv[0] = 1;
  Object opresult1094 = callmethod(call1091, "==", 1, partcv, params);
// compilenode returning opresult1094
  int op_slot_right_1080 = gc_frame_newslot(opresult1094);
  params[0] = opresult1094;
  partcv[0] = 1;
  Object opresult1096 = callmethod(opresult1086, "&&", 1, partcv, params);
// compilenode returning opresult1096
  Object if1079 = done;
  if (istrue(opresult1096)) {
// Begin line 363
  setline(363);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 362
  setline(362);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1097 = alloc_Boolean(1);
// compilenode returning bool1097
  return bool1097;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1079 = undefined;
  } else {
// Begin line 368
  setline(368);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 364
  setline(364);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1101 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1102 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1101);
// compilenode returning call1102
// compilenode returning call1102
  int op_slot_left_1100 = gc_frame_newslot(call1102);
  Object num1103 = alloc_Float64(6);
// compilenode returning num1103
  int op_slot_right_1100 = gc_frame_newslot(num1103);
  params[0] = num1103;
  partcv[0] = 1;
  Object opresult1105 = callmethod(call1102, ">", 1, partcv, params);
// compilenode returning opresult1105
  int op_slot_left_1099 = gc_frame_newslot(opresult1105);
  int callframe1107 = gc_frame_new();
  Object num1108 = alloc_Float64(1);
// compilenode returning num1108
  gc_frame_newslot(num1108);
  Object num1109 = alloc_Float64(7);
// compilenode returning num1109
  gc_frame_newslot(num1109);
// compilenode returning *var_line
  params[0] = num1108;
  params[1] = num1109;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1110 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1107);
// compilenode returning call1110
  int op_slot_left_1106 = gc_frame_newslot(call1110);
  if (strlit1111 == NULL) {
    strlit1111 = alloc_String("import ");
    gc_root(strlit1111);
  }
// compilenode returning strlit1111
  int op_slot_right_1106 = gc_frame_newslot(strlit1111);
  params[0] = strlit1111;
  partcv[0] = 1;
  Object opresult1113 = callmethod(call1110, "==", 1, partcv, params);
// compilenode returning opresult1113
  int op_slot_right_1099 = gc_frame_newslot(opresult1113);
  params[0] = opresult1113;
  partcv[0] = 1;
  Object opresult1115 = callmethod(opresult1105, "&&", 1, partcv, params);
// compilenode returning opresult1115
  Object if1098 = done;
  if (istrue(opresult1115)) {
// Begin line 366
  setline(366);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1116 = gc_frame_new();
// Begin line 365
  setline(365);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1117 = gc_frame_new();
  Object num1118 = alloc_Float64(8);
// compilenode returning num1118
  gc_frame_newslot(num1118);
  int callframe1119 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1120 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1119);
// compilenode returning call1120
// compilenode returning call1120
  gc_frame_newslot(call1120);
// compilenode returning *var_line
  params[0] = num1118;
  params[1] = call1120;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1121 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1117);
// compilenode returning call1121
  gc_frame_newslot(call1121);
  params[0] = call1121;
  partcv[0] = 1;
  Object call1122 = callmethodflags(self, "parseImport", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1116);
// compilenode returning call1122
// Begin line 367
  setline(367);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 366
  setline(366);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1123 = alloc_Boolean(1);
// compilenode returning bool1123
  return bool1123;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1098 = undefined;
  } else {
// Begin line 371
  setline(371);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 368
  setline(368);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1127 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1128 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1127);
// compilenode returning call1128
// compilenode returning call1128
  int op_slot_left_1126 = gc_frame_newslot(call1128);
  Object num1129 = alloc_Float64(7);
// compilenode returning num1129
  int op_slot_right_1126 = gc_frame_newslot(num1129);
  params[0] = num1129;
  partcv[0] = 1;
  Object opresult1131 = callmethod(call1128, ">", 1, partcv, params);
// compilenode returning opresult1131
  int op_slot_left_1125 = gc_frame_newslot(opresult1131);
  int callframe1133 = gc_frame_new();
  Object num1134 = alloc_Float64(1);
// compilenode returning num1134
  gc_frame_newslot(num1134);
  Object num1135 = alloc_Float64(8);
// compilenode returning num1135
  gc_frame_newslot(num1135);
// compilenode returning *var_line
  params[0] = num1134;
  params[1] = num1135;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1136 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1133);
// compilenode returning call1136
  int op_slot_left_1132 = gc_frame_newslot(call1136);
  if (strlit1137 == NULL) {
    strlit1137 = alloc_String("dialect ");
    gc_root(strlit1137);
  }
// compilenode returning strlit1137
  int op_slot_right_1132 = gc_frame_newslot(strlit1137);
  params[0] = strlit1137;
  partcv[0] = 1;
  Object opresult1139 = callmethod(call1136, "==", 1, partcv, params);
// compilenode returning opresult1139
  int op_slot_right_1125 = gc_frame_newslot(opresult1139);
  params[0] = opresult1139;
  partcv[0] = 1;
  Object opresult1141 = callmethod(opresult1131, "&&", 1, partcv, params);
// compilenode returning opresult1141
  Object if1124 = done;
  if (istrue(opresult1141)) {
// Begin line 370
  setline(370);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 369
  setline(369);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1142 = alloc_Boolean(1);
// compilenode returning bool1142
  return bool1142;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1124 = undefined;
  } else {
  }
// compilenode returning if1124
    gc_frame_newslot(if1124);
    if1098 = if1124;
  }
// compilenode returning if1098
    gc_frame_newslot(if1098);
    if1079 = if1098;
  }
// compilenode returning if1079
    gc_frame_newslot(if1079);
    if1071 = if1079;
  }
// compilenode returning if1071
    gc_frame_newslot(if1071);
    if1064 = if1071;
  } else {
  }
// compilenode returning if1064
// Begin line 373
  setline(373);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 372
  setline(372);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1143 = alloc_Boolean(0);
// compilenode returning bool1143
  return bool1143;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_grace_45_pkg__apply1158(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1160 = gc_frame_newslot(*var_curPos);
  int callframe1161 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1162 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1161);
// compilenode returning call1162
// compilenode returning call1162
  int op_slot_right_1160 = gc_frame_newslot(call1162);
  params[0] = call1162;
  partcv[0] = 1;
  Object opresult1164 = callmethod(*var_curPos, "<", 1, partcv, params);
// compilenode returning opresult1164
  int op_slot_left_1159 = gc_frame_newslot(opresult1164);
  int callframe1165 = gc_frame_new();
// compilenode returning *var_line
// compilenode returning *var_curPos
  int op_slot_left_1168 = gc_frame_newslot(*var_curPos);
  Object num1169 = alloc_Float64(1);
// compilenode returning num1169
  int op_slot_right_1168 = gc_frame_newslot(num1169);
  params[0] = num1169;
  partcv[0] = 1;
  Object sum1171 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1171
  params[0] = sum1171;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1172 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1172
  int op_slot_left_1167 = gc_frame_newslot(idxres1172);
  if (strlit1173 == NULL) {
    strlit1173 = alloc_String("""\x22""");
    gc_root(strlit1173);
  }
// compilenode returning strlit1173
  int op_slot_right_1167 = gc_frame_newslot(strlit1173);
  params[0] = strlit1173;
  partcv[0] = 1;
  Object opresult1175 = callmethod(idxres1172, "==", 1, partcv, params);
// compilenode returning opresult1175
  int op_slot_left_1166 = gc_frame_newslot(opresult1175);
// compilenode returning *var_line
// compilenode returning *var_curPos
  int op_slot_left_1177 = gc_frame_newslot(*var_curPos);
  Object num1178 = alloc_Float64(1);
// compilenode returning num1178
  int op_slot_right_1177 = gc_frame_newslot(num1178);
  params[0] = num1178;
  partcv[0] = 1;
  Object sum1180 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1180
  params[0] = sum1180;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1181 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1181
  int op_slot_left_1176 = gc_frame_newslot(idxres1181);
  if (strlit1182 == NULL) {
    strlit1182 = alloc_String(" ");
    gc_root(strlit1182);
  }
// compilenode returning strlit1182
  int op_slot_right_1176 = gc_frame_newslot(strlit1182);
  params[0] = strlit1182;
  partcv[0] = 1;
  Object opresult1184 = callmethod(idxres1181, "==", 1, partcv, params);
// compilenode returning opresult1184
  int op_slot_right_1166 = gc_frame_newslot(opresult1184);
  params[0] = opresult1184;
  partcv[0] = 1;
  Object opresult1186 = callmethod(opresult1175, "||", 1, partcv, params);
// compilenode returning opresult1186
  partcv[0] = 0;
  Object call1187 = callmethod(opresult1186, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1165);
// compilenode returning call1187
  int op_slot_right_1159 = gc_frame_newslot(call1187);
  params[0] = call1187;
  partcv[0] = 1;
  Object opresult1189 = callmethod(opresult1164, "&&", 1, partcv, params);
// compilenode returning opresult1189
  gc_frame_end(frame);
  return opresult1189;
}
Object meth_grace_45_pkg__apply1192(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 384
  setline(384);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 383
  setline(383);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1193 = gc_frame_newslot(*var_curPos);
  Object num1194 = alloc_Float64(1);
// compilenode returning num1194
  int op_slot_right_1193 = gc_frame_newslot(num1194);
  params[0] = num1194;
  partcv[0] = 1;
  Object sum1196 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1196
  *var_curPos = sum1196;
  if (sum1196 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_grace_45_pkg_parseImport1144(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 377
  setline(377);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 376
  setline(376);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1145 = alloc_Float64(1);
// compilenode returning num1145
  *var_curPos = num1145;
  if (num1145 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 378
  setline(378);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 377
  setline(377);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 379
  setline(379);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 378
  setline(378);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1146 == NULL) {
    strlit1146 = alloc_String("");
    gc_root(strlit1146);
  }
// compilenode returning strlit1146
  *var_nextImport = strlit1146;
  if (strlit1146 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 380
  setline(380);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1147 = gc_frame_new();
// Begin line 379
  setline(379);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// compilenode returning *var_curPos
  gc_frame_newslot(*var_curPos);
  params[0] = *var_line;
  params[1] = *var_curPos;
  partcv[0] = 2;
  Object call1148 = callmethodflags(self, "skipWhiteSpace", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1147);
// compilenode returning call1148
  *var_curPos = call1148;
  if (call1148 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 381
  setline(381);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 380
  setline(380);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1150 = gc_frame_newslot(*var_curPos);
  Object num1151 = alloc_Float64(1);
// compilenode returning num1151
  int op_slot_right_1150 = gc_frame_newslot(num1151);
  params[0] = num1151;
  partcv[0] = 1;
  Object sum1153 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1153
  *var_curPos = sum1153;
  if (sum1153 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 382
  setline(382);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 381
  setline(381);
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
  int callframe1156 = gc_frame_new();
// Begin line 382
  setline(382);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1157 = alloc_Block(NULL, NULL, "grace-pkg", 382);
  gc_frame_newslot(block1157);
  block_savedest(block1157);
  Object closure1158 = createclosure(3, "_apply");
setclosureframe(closure1158, stackframe);
  addtoclosure(closure1158, var_curPos);
  addtoclosure(closure1158, var_line);
  addtoclosure(closure1158, selfslot);
  struct UserObject *uo1158 = (struct UserObject*)block1157;
  uo1158->data[0] = (Object)closure1158;
  Method *meth_meth_grace_45_pkg__apply1158 = addmethod2pos(block1157, "_apply", &meth_grace_45_pkg__apply1158, 0);
int argcv_meth_grace_45_pkg__apply1158[] = {0};
meth_meth_grace_45_pkg__apply1158->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1158);
  meth_meth_grace_45_pkg__apply1158->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1158->definitionLine = 368;
// compilenode returning block1157
  gc_frame_newslot(block1157);
// Begin line 385
  setline(385);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1191 = alloc_Block(NULL, NULL, "grace-pkg", 385);
  gc_frame_newslot(block1191);
  block_savedest(block1191);
  Object closure1192 = createclosure(2, "_apply");
setclosureframe(closure1192, stackframe);
  addtoclosure(closure1192, var_curPos);
  addtoclosure(closure1192, selfslot);
  struct UserObject *uo1192 = (struct UserObject*)block1191;
  uo1192->data[0] = (Object)closure1192;
  Method *meth_meth_grace_45_pkg__apply1192 = addmethod2pos(block1191, "_apply", &meth_grace_45_pkg__apply1192, 0);
int argcv_meth_grace_45_pkg__apply1192[] = {0};
meth_meth_grace_45_pkg__apply1192->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1192);
  meth_meth_grace_45_pkg__apply1192->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1192->definitionLine = 382;
// compilenode returning block1191
  gc_frame_newslot(block1191);
  params[0] = block1157;
  params[1] = block1191;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1199 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1156);
// compilenode returning call1199
// Begin line 386
  setline(386);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1200 = gc_frame_new();
// Begin line 385
  setline(385);
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
  Object call1201 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1200);
// compilenode returning call1201
  *var_nextImport = call1201;
  if (call1201 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 390
  setline(390);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 386
  setline(386);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1204 = gc_frame_new();
  int callframe1205 = gc_frame_new();
// compilenode returning *var_nextImport
  gc_frame_newslot(*var_nextImport);
// compilenode returning *var_imported
  params[0] = *var_nextImport;
  partcv[0] = 1;
  Object call1206 = callmethod(*var_imported, "contains",
    1, partcv, params);
  gc_frame_end(callframe1205);
// compilenode returning call1206
  partcv[0] = 0;
  Object call1207 = callmethod(call1206, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1204);
// compilenode returning call1207
  Object if1203 = done;
  if (istrue(call1207)) {
// Begin line 388
  setline(388);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 387
  setline(387);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1209 == NULL) {
    strlit1209 = alloc_String("next import = ");
    gc_root(strlit1209);
  }
// compilenode returning strlit1209
  int op_slot_left_1208 = gc_frame_newslot(strlit1209);
// compilenode returning *var_nextImport
  int op_slot_right_1208 = gc_frame_newslot(*var_nextImport);
  params[0] = *var_nextImport;
  partcv[0] = 1;
  Object opresult1211 = callmethod(strlit1209, "++", 1, partcv, params);
// compilenode returning opresult1211
  params[0] = opresult1211;
  Object call1212 = gracelib_print(NULL, 1,  params);
// compilenode returning call1212
// Begin line 389
  setline(389);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1213 = gc_frame_new();
// Begin line 388
  setline(388);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_nextImport
  gc_frame_newslot(*var_nextImport);
// compilenode returning *var_toProcess
  params[0] = *var_nextImport;
  partcv[0] = 1;
  Object call1214 = callmethod(*var_toProcess, "push",
    1, partcv, params);
  gc_frame_end(callframe1213);
// compilenode returning call1214
    gc_frame_newslot(call1214);
    if1203 = call1214;
  } else {
  }
// compilenode returning if1203
  gc_frame_end(frame);
  return if1203;
}
Object meth_grace_45_pkg__apply1218(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1220 = gc_frame_newslot(*var_count2);
  int callframe1221 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1222 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1221);
// compilenode returning call1222
// compilenode returning call1222
  int op_slot_right_1220 = gc_frame_newslot(call1222);
  params[0] = call1222;
  partcv[0] = 1;
  Object opresult1224 = callmethod(*var_count2, "<=", 1, partcv, params);
// compilenode returning opresult1224
  int op_slot_left_1219 = gc_frame_newslot(opresult1224);
// compilenode returning *var_line
// compilenode returning *var_count2
  params[0] = *var_count2;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1226 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1226
  int op_slot_left_1225 = gc_frame_newslot(idxres1226);
  if (strlit1227 == NULL) {
    strlit1227 = alloc_String("Could not retrieve ");
    gc_root(strlit1227);
  }
// compilenode returning strlit1227
  int op_slot_right_1225 = gc_frame_newslot(strlit1227);
  params[0] = strlit1227;
  partcv[0] = 1;
  Object opresult1229 = callmethod(idxres1226, "==", 1, partcv, params);
// compilenode returning opresult1229
  int op_slot_right_1219 = gc_frame_newslot(opresult1229);
  params[0] = opresult1229;
  partcv[0] = 1;
  Object opresult1231 = callmethod(opresult1224, "&&", 1, partcv, params);
// compilenode returning opresult1231
  gc_frame_end(frame);
  return opresult1231;
}
Object meth_grace_45_pkg__apply1234(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 396
  setline(396);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 395
  setline(395);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count2
  int op_slot_left_1235 = gc_frame_newslot(*var_count2);
  Object num1236 = alloc_Float64(1);
// compilenode returning num1236
  int op_slot_right_1235 = gc_frame_newslot(num1236);
  params[0] = num1236;
  partcv[0] = 1;
  Object sum1238 = callmethod(*var_count2, "+", 1, partcv, params);
// compilenode returning sum1238
  *var_count2 = sum1238;
  if (sum1238 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_grace_45_pkg_skipWhiteSpace1215(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 394
  setline(394);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 393
  setline(393);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  *var_count2 = *var_count;
  if (*var_count == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 397
  setline(397);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1216 = gc_frame_new();
// Begin line 394
  setline(394);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1217 = alloc_Block(NULL, NULL, "grace-pkg", 394);
  gc_frame_newslot(block1217);
  block_savedest(block1217);
  Object closure1218 = createclosure(3, "_apply");
setclosureframe(closure1218, stackframe);
  addtoclosure(closure1218, var_count2);
  addtoclosure(closure1218, var_line);
  addtoclosure(closure1218, selfslot);
  struct UserObject *uo1218 = (struct UserObject*)block1217;
  uo1218->data[0] = (Object)closure1218;
  Method *meth_meth_grace_45_pkg__apply1218 = addmethod2pos(block1217, "_apply", &meth_grace_45_pkg__apply1218, 0);
int argcv_meth_grace_45_pkg__apply1218[] = {0};
meth_meth_grace_45_pkg__apply1218->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1218);
  meth_meth_grace_45_pkg__apply1218->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1218->definitionLine = 382;
// compilenode returning block1217
  gc_frame_newslot(block1217);
// Begin line 397
  setline(397);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1233 = alloc_Block(NULL, NULL, "grace-pkg", 397);
  gc_frame_newslot(block1233);
  block_savedest(block1233);
  Object closure1234 = createclosure(2, "_apply");
setclosureframe(closure1234, stackframe);
  addtoclosure(closure1234, var_count2);
  addtoclosure(closure1234, selfslot);
  struct UserObject *uo1234 = (struct UserObject*)block1233;
  uo1234->data[0] = (Object)closure1234;
  Method *meth_meth_grace_45_pkg__apply1234 = addmethod2pos(block1233, "_apply", &meth_grace_45_pkg__apply1234, 0);
int argcv_meth_grace_45_pkg__apply1234[] = {0};
meth_meth_grace_45_pkg__apply1234->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1234);
  meth_meth_grace_45_pkg__apply1234->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1234->definitionLine = 394;
// compilenode returning block1233
  gc_frame_newslot(block1233);
  params[0] = block1217;
  params[1] = block1233;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1241 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1216);
// compilenode returning call1241
// Begin line 397
  setline(397);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count2
  gc_frame_end(frame);
  return *var_count2;
}
Object meth_grace_45_pkg_displayHelp1242(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 402
  setline(402);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 401
  setline(401);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1243 == NULL) {
    strlit1243 = alloc_String("Available options are:");
    gc_root(strlit1243);
  }
// compilenode returning strlit1243
  params[0] = strlit1243;
  Object call1244 = gracelib_print(NULL, 1,  params);
// compilenode returning call1244
// Begin line 403
  setline(403);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 402
  setline(402);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1245 == NULL) {
    strlit1245 = alloc_String("-get [url] to fetch a package located at a given url");
    gc_root(strlit1245);
  }
// compilenode returning strlit1245
  params[0] = strlit1245;
  Object call1246 = gracelib_print(NULL, 1,  params);
// compilenode returning call1246
// Begin line 404
  setline(404);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 403
  setline(403);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1247 == NULL) {
    strlit1247 = alloc_String("-list to display currenly installed packages");
    gc_root(strlit1247);
  }
// compilenode returning strlit1247
  params[0] = strlit1247;
  Object call1248 = gracelib_print(NULL, 1,  params);
// compilenode returning call1248
  gc_frame_end(frame);
  return call1248;
}
Object meth_grace_45_pkg_printMessage1249(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 410
  setline(410);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 407
  setline(407);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_verbose
  Object if1250 = done;
  if (istrue(*var_verbose)) {
// Begin line 409
  setline(409);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 408
  setline(408);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_message
  params[0] = *var_message;
  Object call1251 = gracelib_print(NULL, 1,  params);
// compilenode returning call1251
    gc_frame_newslot(call1251);
    if1250 = call1251;
  } else {
  }
// compilenode returning if1250
  gc_frame_end(frame);
  return if1250;
}
Object meth_grace_45_pkg__apply1314(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int callframe1316 = gc_frame_new();
// compilenode returning *var_imported
  partcv[0] = 0;
  Object call1317 = callmethod(*var_imported, "size",
    1, partcv, params);
  gc_frame_end(callframe1316);
// compilenode returning call1317
// compilenode returning call1317
  int op_slot_left_1315 = gc_frame_newslot(call1317);
  Object num1318 = alloc_Float64(0);
// compilenode returning num1318
  int op_slot_right_1315 = gc_frame_newslot(num1318);
  params[0] = num1318;
  partcv[0] = 1;
  Object opresult1320 = callmethod(call1317, ">", 1, partcv, params);
// compilenode returning opresult1320
  gc_frame_end(frame);
  return opresult1320;
}
Object meth_grace_45_pkg__apply1323(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 428
  setline(428);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 427
  setline(427);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1324 = gc_frame_new();
// compilenode returning *var_imported
  partcv[0] = 0;
  Object call1325 = callmethod(*var_imported, "pop",
    1, partcv, params);
  gc_frame_end(callframe1324);
// compilenode returning call1325
// compilenode returning call1325
  *var_curImport = call1325;
  if (call1325 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 429
  setline(429);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1326 = gc_frame_new();
// Begin line 428
  setline(428);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1331 == NULL) {
    strlit1331 = alloc_String("");
    gc_root(strlit1331);
  }
// compilenode returning strlit1331
  int op_slot_left_1330 = gc_frame_newslot(strlit1331);
// compilenode returning *var_name
  int op_slot_right_1330 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult1333 = callmethod(strlit1331, "++", 1, partcv, params);
// compilenode returning opresult1333
  int op_slot_left_1329 = gc_frame_newslot(opresult1333);
  if (strlit1334 == NULL) {
    strlit1334 = alloc_String("/");
    gc_root(strlit1334);
  }
// compilenode returning strlit1334
  int op_slot_right_1329 = gc_frame_newslot(strlit1334);
  params[0] = strlit1334;
  partcv[0] = 1;
  Object opresult1336 = callmethod(opresult1333, "++", 1, partcv, params);
// compilenode returning opresult1336
  int op_slot_left_1328 = gc_frame_newslot(opresult1336);
  int callframe1337 = gc_frame_new();
  int callframe1338 = gc_frame_new();
// compilenode returning *var_curImport
  partcv[0] = 0;
  Object call1339 = callmethod(*var_curImport, "address",
    1, partcv, params);
  gc_frame_end(callframe1338);
// compilenode returning call1339
// compilenode returning call1339
  gc_frame_newslot(call1339);
  params[0] = call1339;
  partcv[0] = 1;
  Object call1340 = callmethodflags(self, "removeContainingDir", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1337);
// compilenode returning call1340
  int op_slot_right_1328 = gc_frame_newslot(call1340);
  params[0] = call1340;
  partcv[0] = 1;
  Object opresult1342 = callmethod(opresult1336, "++", 1, partcv, params);
// compilenode returning opresult1342
  int op_slot_left_1327 = gc_frame_newslot(opresult1342);
  if (strlit1343 == NULL) {
    strlit1343 = alloc_String("");
    gc_root(strlit1343);
  }
// compilenode returning strlit1343
  int op_slot_right_1327 = gc_frame_newslot(strlit1343);
  params[0] = strlit1343;
  partcv[0] = 1;
  Object opresult1345 = callmethod(opresult1342, "++", 1, partcv, params);
// compilenode returning opresult1345
  gc_frame_newslot(opresult1345);
  if (strlit1346 == NULL) {
    strlit1346 = alloc_String("w");
    gc_root(strlit1346);
  }
// compilenode returning strlit1346
  gc_frame_newslot(strlit1346);
// compilenode returning *var_io
  params[0] = opresult1345;
  params[1] = strlit1346;
  partcv[0] = 2;
  Object call1347 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1326);
// compilenode returning call1347
  *var_toWrite = call1347;
  if (call1347 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 430
  setline(430);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1349 = gc_frame_new();
// Begin line 429
  setline(429);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1350 = gc_frame_new();
// compilenode returning *var_curImport
  partcv[0] = 0;
  Object call1351 = callmethod(*var_curImport, "data",
    1, partcv, params);
  gc_frame_end(callframe1350);
// compilenode returning call1351
// compilenode returning call1351
  gc_frame_newslot(call1351);
// compilenode returning *var_toWrite
  params[0] = call1351;
  partcv[0] = 1;
  Object call1352 = callmethod(*var_toWrite, "write",
    1, partcv, params);
  gc_frame_end(callframe1349);
// compilenode returning call1352
  gc_frame_end(frame);
  return call1352;
}
Object meth_grace_45_pkg_bundle1252(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 24, (flags>>24)&0xff);
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
// Begin line 417
  setline(417);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 416
  setline(416);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array1253 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array1253
  *var_imported = array1253;
  if (array1253 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 418
  setline(418);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1255 = gc_frame_new();
// Begin line 417
  setline(417);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toBundle
  gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object call1256 = callmethodflags(self, "getContainingDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1255);
// compilenode returning call1256
  *var_bundlePath = call1256;
  if (call1256 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 419
  setline(419);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1258 = gc_frame_new();
// Begin line 418
  setline(418);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1261 == NULL) {
    strlit1261 = alloc_String("");
    gc_root(strlit1261);
  }
// compilenode returning strlit1261
  int op_slot_left_1260 = gc_frame_newslot(strlit1261);
// compilenode returning *var_name
  int op_slot_right_1260 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult1263 = callmethod(strlit1261, "++", 1, partcv, params);
// compilenode returning opresult1263
  int op_slot_left_1259 = gc_frame_newslot(opresult1263);
  if (strlit1264 == NULL) {
    strlit1264 = alloc_String("/");
    gc_root(strlit1264);
  }
// compilenode returning strlit1264
  int op_slot_right_1259 = gc_frame_newslot(strlit1264);
  params[0] = strlit1264;
  partcv[0] = 1;
  Object opresult1266 = callmethod(opresult1263, "++", 1, partcv, params);
// compilenode returning opresult1266
  gc_frame_newslot(opresult1266);
  params[0] = opresult1266;
  partcv[0] = 1;
  Object call1267 = callmethodflags(self, "createDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1258);
// compilenode returning call1267
  *var_newDir = call1267;
  if (call1267 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 420
  setline(420);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1268 = gc_frame_new();
// Begin line 419
  setline(419);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toBundle
  gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object call1269 = callmethodflags(self, "removeContainingDir", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1268);
// compilenode returning call1269
  *var_newFileName = call1269;
  if (call1269 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 421
  setline(421);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1270 = gc_frame_new();
// Begin line 420
  setline(420);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1273 == NULL) {
    strlit1273 = alloc_String("");
    gc_root(strlit1273);
  }
// compilenode returning strlit1273
  int op_slot_left_1272 = gc_frame_newslot(strlit1273);
// compilenode returning *var_toBundle
  int op_slot_right_1272 = gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object opresult1275 = callmethod(strlit1273, "++", 1, partcv, params);
// compilenode returning opresult1275
  int op_slot_left_1271 = gc_frame_newslot(opresult1275);
  if (strlit1276 == NULL) {
    strlit1276 = alloc_String("");
    gc_root(strlit1276);
  }
// compilenode returning strlit1276
  int op_slot_right_1271 = gc_frame_newslot(strlit1276);
  params[0] = strlit1276;
  partcv[0] = 1;
  Object opresult1278 = callmethod(opresult1275, "++", 1, partcv, params);
// compilenode returning opresult1278
  gc_frame_newslot(opresult1278);
  if (strlit1279 == NULL) {
    strlit1279 = alloc_String("r");
    gc_root(strlit1279);
  }
// compilenode returning strlit1279
  gc_frame_newslot(strlit1279);
// compilenode returning *var_io
  params[0] = opresult1278;
  params[1] = strlit1279;
  partcv[0] = 2;
  Object call1280 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1270);
// compilenode returning call1280
  *var_toOpen = call1280;
  if (call1280 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 422
  setline(422);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 421
  setline(421);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1281 = gc_frame_new();
// compilenode returning *var_toOpen
  partcv[0] = 0;
  Object call1282 = callmethod(*var_toOpen, "read",
    1, partcv, params);
  gc_frame_end(callframe1281);
// compilenode returning call1282
// compilenode returning call1282
  *var_openData = call1282;
  if (call1282 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 423
  setline(423);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1283 = gc_frame_new();
// Begin line 422
  setline(422);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1288 == NULL) {
    strlit1288 = alloc_String("");
    gc_root(strlit1288);
  }
// compilenode returning strlit1288
  int op_slot_left_1287 = gc_frame_newslot(strlit1288);
// compilenode returning *var_name
  int op_slot_right_1287 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult1290 = callmethod(strlit1288, "++", 1, partcv, params);
// compilenode returning opresult1290
  int op_slot_left_1286 = gc_frame_newslot(opresult1290);
  if (strlit1291 == NULL) {
    strlit1291 = alloc_String("/");
    gc_root(strlit1291);
  }
// compilenode returning strlit1291
  int op_slot_right_1286 = gc_frame_newslot(strlit1291);
  params[0] = strlit1291;
  partcv[0] = 1;
  Object opresult1293 = callmethod(opresult1290, "++", 1, partcv, params);
// compilenode returning opresult1293
  int op_slot_left_1285 = gc_frame_newslot(opresult1293);
// compilenode returning *var_newFileName
  int op_slot_right_1285 = gc_frame_newslot(*var_newFileName);
  params[0] = *var_newFileName;
  partcv[0] = 1;
  Object opresult1295 = callmethod(opresult1293, "++", 1, partcv, params);
// compilenode returning opresult1295
  int op_slot_left_1284 = gc_frame_newslot(opresult1295);
  if (strlit1296 == NULL) {
    strlit1296 = alloc_String("");
    gc_root(strlit1296);
  }
// compilenode returning strlit1296
  int op_slot_right_1284 = gc_frame_newslot(strlit1296);
  params[0] = strlit1296;
  partcv[0] = 1;
  Object opresult1298 = callmethod(opresult1295, "++", 1, partcv, params);
// compilenode returning opresult1298
  gc_frame_newslot(opresult1298);
  if (strlit1299 == NULL) {
    strlit1299 = alloc_String("w");
    gc_root(strlit1299);
  }
// compilenode returning strlit1299
  gc_frame_newslot(strlit1299);
// compilenode returning *var_io
  params[0] = opresult1298;
  params[1] = strlit1299;
  partcv[0] = 2;
  Object call1300 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1283);
// compilenode returning call1300
  *var_toWrite = call1300;
  if (call1300 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 424
  setline(424);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1301 = gc_frame_new();
// Begin line 423
  setline(423);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_openData
  gc_frame_newslot(*var_openData);
// compilenode returning *var_toWrite
  params[0] = *var_openData;
  partcv[0] = 1;
  Object call1302 = callmethod(*var_toWrite, "write",
    1, partcv, params);
  gc_frame_end(callframe1301);
// compilenode returning call1302
// Begin line 425
  setline(425);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1303 = gc_frame_new();
// Begin line 424
  setline(424);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toBundle
  gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object call1304 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1303);
// compilenode returning call1304
// Begin line 426
  setline(426);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 425
  setline(425);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1306 == NULL) {
    strlit1306 = alloc_String("Imported size = ");
    gc_root(strlit1306);
  }
// compilenode returning strlit1306
  int op_slot_left_1305 = gc_frame_newslot(strlit1306);
  int callframe1307 = gc_frame_new();
// compilenode returning *var_imported
  partcv[0] = 0;
  Object call1308 = callmethod(*var_imported, "size",
    1, partcv, params);
  gc_frame_end(callframe1307);
// compilenode returning call1308
// compilenode returning call1308
  int op_slot_right_1305 = gc_frame_newslot(call1308);
  params[0] = call1308;
  partcv[0] = 1;
  Object opresult1310 = callmethod(strlit1306, "++", 1, partcv, params);
// compilenode returning opresult1310
  params[0] = opresult1310;
  Object call1311 = gracelib_print(NULL, 1,  params);
// compilenode returning call1311
// Begin line 431
  setline(431);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1312 = gc_frame_new();
// Begin line 426
  setline(426);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1313 = alloc_Block(NULL, NULL, "grace-pkg", 426);
  gc_frame_newslot(block1313);
  block_savedest(block1313);
  Object closure1314 = createclosure(2, "_apply");
setclosureframe(closure1314, stackframe);
  addtoclosure(closure1314, var_imported);
  addtoclosure(closure1314, selfslot);
  struct UserObject *uo1314 = (struct UserObject*)block1313;
  uo1314->data[0] = (Object)closure1314;
  Method *meth_meth_grace_45_pkg__apply1314 = addmethod2pos(block1313, "_apply", &meth_grace_45_pkg__apply1314, 0);
int argcv_meth_grace_45_pkg__apply1314[] = {0};
meth_meth_grace_45_pkg__apply1314->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1314);
  meth_meth_grace_45_pkg__apply1314->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1314->definitionLine = 425;
// compilenode returning block1313
  gc_frame_newslot(block1313);
// Begin line 431
  setline(431);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1322 = alloc_Block(NULL, NULL, "grace-pkg", 431);
  gc_frame_newslot(block1322);
  block_savedest(block1322);
  Object closure1323 = createclosure(5, "_apply");
setclosureframe(closure1323, stackframe);
  addtoclosure(closure1323, var_imported);
  addtoclosure(closure1323, var_name);
  addtoclosure(closure1323, var_io);
  addtoclosure(closure1323, var_toWrite);
  addtoclosure(closure1323, selfslot);
  struct UserObject *uo1323 = (struct UserObject*)block1322;
  uo1323->data[0] = (Object)closure1323;
  Method *meth_meth_grace_45_pkg__apply1323 = addmethod2pos(block1322, "_apply", &meth_grace_45_pkg__apply1323, 0);
int argcv_meth_grace_45_pkg__apply1323[] = {0};
meth_meth_grace_45_pkg__apply1323->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1323);
  meth_meth_grace_45_pkg__apply1323->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1323->definitionLine = 426;
// compilenode returning block1322
  gc_frame_newslot(block1322);
  params[0] = block1313;
  params[1] = block1322;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1354 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1312);
// compilenode returning call1354
  gc_frame_end(frame);
  return call1354;
}
Object meth_grace_45_pkg__apply1426(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 448
  setline(448);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1427 = gc_frame_new();
// Begin line 447
  setline(447);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_d
  gc_frame_newslot(*var_d);
// compilenode returning *var_out
  params[0] = *var_d;
  partcv[0] = 1;
  Object call1428 = callmethod(*var_out, "write",
    1, partcv, params);
  gc_frame_end(callframe1427);
// compilenode returning call1428
  gc_frame_end(frame);
  return call1428;
}
Object meth_grace_45_pkg__apply1362(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 451
  setline(451);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 437
  setline(437);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1365 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1366 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1365);
// compilenode returning call1366
// compilenode returning call1366
  int op_slot_left_1364 = gc_frame_newslot(call1366);
  Object num1367 = alloc_Float64(4);
// compilenode returning num1367
  int op_slot_right_1364 = gc_frame_newslot(num1367);
  params[0] = num1367;
  partcv[0] = 1;
  Object opresult1369 = callmethod(call1366, ">", 1, partcv, params);
// compilenode returning opresult1369
  Object if1363 = done;
  if (istrue(opresult1369)) {
// Begin line 450
  setline(450);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 438
  setline(438);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1372 = gc_frame_new();
  int callframe1374 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1375 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1374);
// compilenode returning call1375
// compilenode returning call1375
  int op_slot_left_1373 = gc_frame_newslot(call1375);
  Object num1376 = alloc_Float64(5);
// compilenode returning num1376
  int op_slot_right_1373 = gc_frame_newslot(num1376);
  params[0] = num1376;
  partcv[0] = 1;
  Object diff1378 = callmethod(call1375, "-", 1, partcv, params);
// compilenode returning diff1378
  gc_frame_newslot(diff1378);
  int callframe1379 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1380 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1379);
// compilenode returning call1380
// compilenode returning call1380
  gc_frame_newslot(call1380);
// compilenode returning *var_file
  params[0] = diff1378;
  params[1] = call1380;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1381 = callmethod(*var_file, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1372);
// compilenode returning call1381
  int op_slot_left_1371 = gc_frame_newslot(call1381);
  if (strlit1382 == NULL) {
    strlit1382 = alloc_String(".grace");
    gc_root(strlit1382);
  }
// compilenode returning strlit1382
  int op_slot_right_1371 = gc_frame_newslot(strlit1382);
  params[0] = strlit1382;
  partcv[0] = 1;
  Object opresult1384 = callmethod(call1381, "==", 1, partcv, params);
// compilenode returning opresult1384
  Object if1370 = done;
  if (istrue(opresult1384)) {
// Begin line 440
  setline(440);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1385 = gc_frame_new();
// Begin line 439
  setline(439);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_address
  int op_slot_left_1387 = gc_frame_newslot(*var_address);
  if (strlit1388 == NULL) {
    strlit1388 = alloc_String("/");
    gc_root(strlit1388);
  }
// compilenode returning strlit1388
  int op_slot_right_1387 = gc_frame_newslot(strlit1388);
  params[0] = strlit1388;
  partcv[0] = 1;
  Object opresult1390 = callmethod(*var_address, "++", 1, partcv, params);
// compilenode returning opresult1390
  int op_slot_left_1386 = gc_frame_newslot(opresult1390);
// compilenode returning *var_file
  int op_slot_right_1386 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1392 = callmethod(opresult1390, "++", 1, partcv, params);
// compilenode returning opresult1392
  gc_frame_newslot(opresult1392);
  if (strlit1393 == NULL) {
    strlit1393 = alloc_String("r");
    gc_root(strlit1393);
  }
// compilenode returning strlit1393
  gc_frame_newslot(strlit1393);
// compilenode returning *var_io
  params[0] = opresult1392;
  params[1] = strlit1393;
  partcv[0] = 2;
  Object call1394 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1385);
// compilenode returning call1394
  *var_open = call1394;
  if (call1394 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 441
  setline(441);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 440
  setline(440);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1395 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call1396 = callmethod(*var_open, "read",
    1, partcv, params);
  gc_frame_end(callframe1395);
// compilenode returning call1396
// compilenode returning call1396
  *var_readFile = call1396;
  if (call1396 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 442
  setline(442);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1397 = gc_frame_new();
// Begin line 441
  setline(441);
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
  Object call1398 = callmethodflags(self, "parseAndPrefix", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1397);
// compilenode returning call1398
  *var_toWrite = call1398;
  if (call1398 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 442
  setline(442);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1399 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call1400 = callmethod(*var_open, "close",
    1, partcv, params);
  gc_frame_end(callframe1399);
// compilenode returning call1400
// compilenode returning call1400
// Begin line 445
  setline(445);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1401 = gc_frame_new();
// Begin line 444
  setline(444);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1405 == NULL) {
    strlit1405 = alloc_String("rm ");
    gc_root(strlit1405);
  }
// compilenode returning strlit1405
  int op_slot_left_1404 = gc_frame_newslot(strlit1405);
// compilenode returning *var_address
  int op_slot_right_1404 = gc_frame_newslot(*var_address);
  params[0] = *var_address;
  partcv[0] = 1;
  Object opresult1407 = callmethod(strlit1405, "++", 1, partcv, params);
// compilenode returning opresult1407
  int op_slot_left_1403 = gc_frame_newslot(opresult1407);
  if (strlit1408 == NULL) {
    strlit1408 = alloc_String("/");
    gc_root(strlit1408);
  }
// compilenode returning strlit1408
  int op_slot_right_1403 = gc_frame_newslot(strlit1408);
  params[0] = strlit1408;
  partcv[0] = 1;
  Object opresult1410 = callmethod(opresult1407, "++", 1, partcv, params);
// compilenode returning opresult1410
  int op_slot_left_1402 = gc_frame_newslot(opresult1410);
// compilenode returning *var_file
  int op_slot_right_1402 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1412 = callmethod(opresult1410, "++", 1, partcv, params);
// compilenode returning opresult1412
  gc_frame_newslot(opresult1412);
// compilenode returning *var_io
  params[0] = opresult1412;
  partcv[0] = 1;
  Object call1413 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe1401);
// compilenode returning call1413
// Begin line 446
  setline(446);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1414 = gc_frame_new();
// Begin line 445
  setline(445);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_address
  int op_slot_left_1416 = gc_frame_newslot(*var_address);
  if (strlit1417 == NULL) {
    strlit1417 = alloc_String("/");
    gc_root(strlit1417);
  }
// compilenode returning strlit1417
  int op_slot_right_1416 = gc_frame_newslot(strlit1417);
  params[0] = strlit1417;
  partcv[0] = 1;
  Object opresult1419 = callmethod(*var_address, "++", 1, partcv, params);
// compilenode returning opresult1419
  int op_slot_left_1415 = gc_frame_newslot(opresult1419);
// compilenode returning *var_file
  int op_slot_right_1415 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1421 = callmethod(opresult1419, "++", 1, partcv, params);
// compilenode returning opresult1421
  gc_frame_newslot(opresult1421);
  if (strlit1422 == NULL) {
    strlit1422 = alloc_String("w");
    gc_root(strlit1422);
  }
// compilenode returning strlit1422
  gc_frame_newslot(strlit1422);
// compilenode returning *var_io
  params[0] = opresult1421;
  params[1] = strlit1422;
  partcv[0] = 2;
  Object call1423 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1414);
// compilenode returning call1423
  *var_out = call1423;
  if (call1423 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 449
  setline(449);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1424 = gc_frame_new();
// Begin line 446
  setline(446);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toWrite
  gc_frame_newslot(*var_toWrite);
// Begin line 449
  setline(449);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1425 = alloc_Block(NULL, NULL, "grace-pkg", 449);
  gc_frame_newslot(block1425);
  block_savedest(block1425);
  Object closure1426 = createclosure(2, "_apply");
setclosureframe(closure1426, stackframe);
  addtoclosure(closure1426, var_out);
  addtoclosure(closure1426, selfslot);
  struct UserObject *uo1426 = (struct UserObject*)block1425;
  uo1426->data[0] = (Object)closure1426;
  Method *meth_meth_grace_45_pkg__apply1426 = addmethod2pos(block1425, "_apply", &meth_grace_45_pkg__apply1426, 0);
int argcv_meth_grace_45_pkg__apply1426[] = {1};
meth_meth_grace_45_pkg__apply1426->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1426);
  meth_meth_grace_45_pkg__apply1426->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1426->definitionLine = 442;
// compilenode returning block1425
  gc_frame_newslot(block1425);
  params[0] = *var_toWrite;
  params[1] = block1425;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1430 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1424);
// compilenode returning call1430
    gc_frame_newslot(call1430);
    if1370 = call1430;
  } else {
  }
// compilenode returning if1370
    gc_frame_newslot(if1370);
    if1363 = if1370;
  } else {
  }
// compilenode returning if1363
  gc_frame_end(frame);
  return if1363;
}
Object meth_grace_45_pkg_setImportDest1355(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 435
  setline(435);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1356 = gc_frame_new();
// Begin line 434
  setline(434);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_address
  gc_frame_newslot(*var_address);
// compilenode returning *var_io
  params[0] = *var_address;
  partcv[0] = 1;
  Object call1357 = callmethod(*var_io, "getdir",
    1, partcv, params);
  gc_frame_end(callframe1356);
// compilenode returning call1357
  *var_folder = call1357;
  if (call1357 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 435
  setline(435);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1358 == NULL) {
    strlit1358 = alloc_String("Setting import dest");
    gc_root(strlit1358);
  }
// compilenode returning strlit1358
  params[0] = strlit1358;
  Object call1359 = gracelib_print(NULL, 1,  params);
// compilenode returning call1359
// Begin line 455
  setline(455);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1360 = gc_frame_new();
// Begin line 436
  setline(436);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_folder
  gc_frame_newslot(*var_folder);
// Begin line 455
  setline(455);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1361 = alloc_Block(NULL, NULL, "grace-pkg", 455);
  gc_frame_newslot(block1361);
  block_savedest(block1361);
  Object closure1362 = createclosure(4, "_apply");
setclosureframe(closure1362, stackframe);
  addtoclosure(closure1362, var_address);
  addtoclosure(closure1362, var_io);
  addtoclosure(closure1362, var_prefix);
  addtoclosure(closure1362, selfslot);
  struct UserObject *uo1362 = (struct UserObject*)block1361;
  uo1362->data[0] = (Object)closure1362;
  Method *meth_meth_grace_45_pkg__apply1362 = addmethod2pos(block1361, "_apply", &meth_grace_45_pkg__apply1362, 0);
int argcv_meth_grace_45_pkg__apply1362[] = {1};
meth_meth_grace_45_pkg__apply1362->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1362);
  meth_meth_grace_45_pkg__apply1362->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1362->definitionLine = 429;
// compilenode returning block1361
  gc_frame_newslot(block1361);
  params[0] = *var_folder;
  params[1] = block1361;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1432 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1360);
// compilenode returning call1432
  gc_frame_end(frame);
  return call1432;
}
Object meth_grace_45_pkg__apply1447(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1448 = gc_frame_newslot(*var_curPos);
  int callframe1449 = gc_frame_new();
// compilenode returning *var_readFile
  partcv[0] = 0;
  Object call1450 = callmethod(*var_readFile, "size",
    1, partcv, params);
  gc_frame_end(callframe1449);
// compilenode returning call1450
// compilenode returning call1450
  int op_slot_right_1448 = gc_frame_newslot(call1450);
  params[0] = call1450;
  partcv[0] = 1;
  Object opresult1452 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1452
  gc_frame_end(frame);
  return opresult1452;
}
Object meth_grace_45_pkg__apply1459(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1461 = gc_frame_newslot(*var_curPos);
  int callframe1462 = gc_frame_new();
// compilenode returning *var_readFile
  partcv[0] = 0;
  Object call1463 = callmethod(*var_readFile, "size",
    1, partcv, params);
  gc_frame_end(callframe1462);
// compilenode returning call1463
// compilenode returning call1463
  int op_slot_right_1461 = gc_frame_newslot(call1463);
  params[0] = call1463;
  partcv[0] = 1;
  Object opresult1465 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1465
  int op_slot_left_1460 = gc_frame_newslot(opresult1465);
// compilenode returning *var_readFile
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1467 = callmethod(*var_readFile, "[]", 1, partcv, params);
// compilenode returning idxres1467
  int op_slot_left_1466 = gc_frame_newslot(idxres1467);
  if (strlit1468 == NULL) {
    strlit1468 = alloc_String("""\x0a""");
    gc_root(strlit1468);
  }
// compilenode returning strlit1468
  int op_slot_right_1466 = gc_frame_newslot(strlit1468);
  params[0] = strlit1468;
  partcv[0] = 1;
  Object opresult1470 = callmethod(idxres1467, "!=", 1, partcv, params);
// compilenode returning opresult1470
  int op_slot_right_1460 = gc_frame_newslot(opresult1470);
  params[0] = opresult1470;
  partcv[0] = 1;
  Object opresult1472 = callmethod(opresult1465, "&&", 1, partcv, params);
// compilenode returning opresult1472
  gc_frame_end(frame);
  return opresult1472;
}
Object meth_grace_45_pkg__apply1475(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 467
  setline(467);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 466
  setline(466);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1476 = gc_frame_newslot(*var_curPos);
  Object num1477 = alloc_Float64(1);
// compilenode returning num1477
  int op_slot_right_1476 = gc_frame_newslot(num1477);
  params[0] = num1477;
  partcv[0] = 1;
  Object sum1479 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1479
  *var_curPos = sum1479;
  if (sum1479 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_grace_45_pkg__apply1500(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 473
  setline(473);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 472
  setline(472);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  int op_slot_left_1501 = gc_frame_newslot(*var_line);
// Begin line 473
  setline(473);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 472
  setline(472);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_readFile
// compilenode returning *var_b
  params[0] = *var_b;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1502 = callmethod(*var_readFile, "[]", 1, partcv, params);
// compilenode returning idxres1502
  int op_slot_right_1501 = gc_frame_newslot(idxres1502);
  params[0] = idxres1502;
  partcv[0] = 1;
  Object opresult1504 = callmethod(*var_line, "++", 1, partcv, params);
// compilenode returning opresult1504
  *var_line = opresult1504;
  if (opresult1504 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_grace_45_pkg__apply1544(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1546 = gc_frame_newslot(*var_pos);
  int callframe1547 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1548 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1547);
// compilenode returning call1548
// compilenode returning call1548
  int op_slot_right_1546 = gc_frame_newslot(call1548);
  params[0] = call1548;
  partcv[0] = 1;
  Object opresult1550 = callmethod(*var_pos, "<", 1, partcv, params);
// compilenode returning opresult1550
  int op_slot_left_1545 = gc_frame_newslot(opresult1550);
  int callframe1551 = gc_frame_new();
// compilenode returning *var_line
// compilenode returning *var_pos
  params[0] = *var_pos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1554 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1554
  int op_slot_left_1553 = gc_frame_newslot(idxres1554);
  if (strlit1555 == NULL) {
    strlit1555 = alloc_String("""\x22""");
    gc_root(strlit1555);
  }
// compilenode returning strlit1555
  int op_slot_right_1553 = gc_frame_newslot(strlit1555);
  params[0] = strlit1555;
  partcv[0] = 1;
  Object opresult1557 = callmethod(idxres1554, "==", 1, partcv, params);
// compilenode returning opresult1557
  int op_slot_left_1552 = gc_frame_newslot(opresult1557);
// compilenode returning *var_line
// compilenode returning *var_pos
  params[0] = *var_pos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1559 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1559
  int op_slot_left_1558 = gc_frame_newslot(idxres1559);
  if (strlit1560 == NULL) {
    strlit1560 = alloc_String(" ");
    gc_root(strlit1560);
  }
// compilenode returning strlit1560
  int op_slot_right_1558 = gc_frame_newslot(strlit1560);
  params[0] = strlit1560;
  partcv[0] = 1;
  Object opresult1562 = callmethod(idxres1559, "==", 1, partcv, params);
// compilenode returning opresult1562
  int op_slot_right_1552 = gc_frame_newslot(opresult1562);
  params[0] = opresult1562;
  partcv[0] = 1;
  Object opresult1564 = callmethod(opresult1557, "||", 1, partcv, params);
// compilenode returning opresult1564
  partcv[0] = 0;
  Object call1565 = callmethod(opresult1564, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1551);
// compilenode returning call1565
  int op_slot_right_1545 = gc_frame_newslot(call1565);
  params[0] = call1565;
  partcv[0] = 1;
  Object opresult1567 = callmethod(opresult1550, "&&", 1, partcv, params);
// compilenode returning opresult1567
  gc_frame_end(frame);
  return opresult1567;
}
Object meth_grace_45_pkg__apply1570(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 484
  setline(484);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 483
  setline(483);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  int op_slot_left_1571 = gc_frame_newslot(*var_pos);
  Object num1572 = alloc_Float64(1);
// compilenode returning num1572
  int op_slot_right_1571 = gc_frame_newslot(num1572);
  params[0] = num1572;
  partcv[0] = 1;
  Object sum1574 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1574
  *var_pos = sum1574;
  if (sum1574 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_grace_45_pkg__apply1608(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 491
  setline(491);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1609 = gc_frame_new();
// Begin line 490
  setline(490);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_d
  gc_frame_newslot(*var_d);
// compilenode returning *var_outFile
  params[0] = *var_d;
  partcv[0] = 1;
  Object call1610 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1609);
// compilenode returning call1610
  gc_frame_end(frame);
  return call1610;
}
Object meth_grace_45_pkg__apply1618(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 497
  setline(497);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1619 = gc_frame_new();
// Begin line 496
  setline(496);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_d
  gc_frame_newslot(*var_d);
// compilenode returning *var_outFile
  params[0] = *var_d;
  partcv[0] = 1;
  Object call1620 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1619);
// compilenode returning call1620
  gc_frame_end(frame);
  return call1620;
}
Object meth_grace_45_pkg__apply1455(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 465
  setline(465);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 464
  setline(464);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 468
  setline(468);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1457 = gc_frame_new();
// Begin line 465
  setline(465);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1458 = alloc_Block(NULL, NULL, "grace-pkg", 465);
  gc_frame_newslot(block1458);
  block_savedest(block1458);
  Object closure1459 = createclosure(3, "_apply");
setclosureframe(closure1459, stackframe);
  addtoclosure(closure1459, var_curPos);
  addtoclosure(closure1459, var_readFile);
  addtoclosure(closure1459, selfslot);
  struct UserObject *uo1459 = (struct UserObject*)block1458;
  uo1459->data[0] = (Object)closure1459;
  Method *meth_meth_grace_45_pkg__apply1459 = addmethod2pos(block1458, "_apply", &meth_grace_45_pkg__apply1459, 0);
int argcv_meth_grace_45_pkg__apply1459[] = {0};
meth_meth_grace_45_pkg__apply1459->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1459);
  meth_meth_grace_45_pkg__apply1459->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1459->definitionLine = 463;
// compilenode returning block1458
  gc_frame_newslot(block1458);
// Begin line 468
  setline(468);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1474 = alloc_Block(NULL, NULL, "grace-pkg", 468);
  gc_frame_newslot(block1474);
  block_savedest(block1474);
  Object closure1475 = createclosure(2, "_apply");
setclosureframe(closure1475, stackframe);
  addtoclosure(closure1475, var_curPos);
  addtoclosure(closure1475, selfslot);
  struct UserObject *uo1475 = (struct UserObject*)block1474;
  uo1475->data[0] = (Object)closure1475;
  Method *meth_meth_grace_45_pkg__apply1475 = addmethod2pos(block1474, "_apply", &meth_grace_45_pkg__apply1475, 0);
int argcv_meth_grace_45_pkg__apply1475[] = {0};
meth_meth_grace_45_pkg__apply1475->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1475);
  meth_meth_grace_45_pkg__apply1475->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1475->definitionLine = 465;
// compilenode returning block1474
  gc_frame_newslot(block1474);
  params[0] = block1458;
  params[1] = block1474;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1482 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1457);
// compilenode returning call1482
// Begin line 469
  setline(469);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1483 = gc_frame_new();
// Begin line 468
  setline(468);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_startPos
  gc_frame_newslot(*var_startPos);
// compilenode returning *var_curPos
  int op_slot_left_1484 = gc_frame_newslot(*var_curPos);
  Object num1485 = alloc_Float64(1);
// compilenode returning num1485
  int op_slot_right_1484 = gc_frame_newslot(num1485);
  params[0] = num1485;
  partcv[0] = 1;
  Object diff1487 = callmethod(*var_curPos, "-", 1, partcv, params);
// compilenode returning diff1487
  gc_frame_newslot(diff1487);
// compilenode returning *var_readFile
  params[0] = *var_startPos;
  params[1] = diff1487;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1488 = callmethod(*var_readFile, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1483);
// compilenode returning call1488
  *var_data = call1488;
  if (call1488 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 470
  setline(470);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 469
  setline(469);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1489 == NULL) {
    strlit1489 = alloc_String("");
    gc_root(strlit1489);
  }
// compilenode returning strlit1489
  *var_line = strlit1489;
  if (strlit1489 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 471
  setline(471);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 470
  setline(470);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1490 = alloc_Float64(1);
// compilenode returning num1490
  *var_count = num1490;
  if (num1490 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 474
  setline(474);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1491 = gc_frame_new();
// Begin line 471
  setline(471);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_startPos
  int op_slot_left_1492 = gc_frame_newslot(*var_startPos);
// compilenode returning *var_curPos
  int op_slot_left_1493 = gc_frame_newslot(*var_curPos);
  Object num1494 = alloc_Float64(1);
// compilenode returning num1494
  int op_slot_right_1493 = gc_frame_newslot(num1494);
  params[0] = num1494;
  partcv[0] = 1;
  Object diff1496 = callmethod(*var_curPos, "-", 1, partcv, params);
// compilenode returning diff1496
  int op_slot_right_1492 = gc_frame_newslot(diff1496);
  params[0] = diff1496;
  partcv[0] = 1;
  Object opresult1498 = callmethod(*var_startPos, "..", 1, partcv, params);
// compilenode returning opresult1498
  gc_frame_newslot(opresult1498);
// Begin line 474
  setline(474);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1499 = alloc_Block(NULL, NULL, "grace-pkg", 474);
  gc_frame_newslot(block1499);
  block_savedest(block1499);
  Object closure1500 = createclosure(3, "_apply");
setclosureframe(closure1500, stackframe);
  addtoclosure(closure1500, var_line);
  addtoclosure(closure1500, var_readFile);
  addtoclosure(closure1500, selfslot);
  struct UserObject *uo1500 = (struct UserObject*)block1499;
  uo1500->data[0] = (Object)closure1500;
  Method *meth_meth_grace_45_pkg__apply1500 = addmethod2pos(block1499, "_apply", &meth_grace_45_pkg__apply1500, 0);
int argcv_meth_grace_45_pkg__apply1500[] = {1};
meth_meth_grace_45_pkg__apply1500->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1500);
  meth_meth_grace_45_pkg__apply1500->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1500->definitionLine = 465;
// compilenode returning block1499
  gc_frame_newslot(block1499);
  params[0] = opresult1498;
  params[1] = block1499;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1507 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1491);
// compilenode returning call1507
// Begin line 475
  setline(475);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 474
  setline(474);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  params[0] = *var_line;
  Object call1508 = gracelib_print(NULL, 1,  params);
// compilenode returning call1508
// Begin line 500
  setline(500);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 475
  setline(475);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1512 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1513 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1512);
// compilenode returning call1513
// compilenode returning call1513
  int op_slot_left_1511 = gc_frame_newslot(call1513);
  Object num1514 = alloc_Float64(6);
// compilenode returning num1514
  int op_slot_right_1511 = gc_frame_newslot(num1514);
  params[0] = num1514;
  partcv[0] = 1;
  Object opresult1516 = callmethod(call1513, ">", 1, partcv, params);
// compilenode returning opresult1516
  int op_slot_left_1510 = gc_frame_newslot(opresult1516);
  int callframe1518 = gc_frame_new();
  Object num1519 = alloc_Float64(1);
// compilenode returning num1519
  gc_frame_newslot(num1519);
  Object num1520 = alloc_Float64(7);
// compilenode returning num1520
  gc_frame_newslot(num1520);
// compilenode returning *var_line
  params[0] = num1519;
  params[1] = num1520;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1521 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1518);
// compilenode returning call1521
  int op_slot_left_1517 = gc_frame_newslot(call1521);
  if (strlit1522 == NULL) {
    strlit1522 = alloc_String("import ");
    gc_root(strlit1522);
  }
// compilenode returning strlit1522
  int op_slot_right_1517 = gc_frame_newslot(strlit1522);
  params[0] = strlit1522;
  partcv[0] = 1;
  Object opresult1524 = callmethod(call1521, "==", 1, partcv, params);
// compilenode returning opresult1524
  int op_slot_right_1510 = gc_frame_newslot(opresult1524);
  params[0] = opresult1524;
  partcv[0] = 1;
  Object opresult1526 = callmethod(opresult1516, "&&", 1, partcv, params);
// compilenode returning opresult1526
  Object if1509 = done;
  if (istrue(opresult1526)) {
// Begin line 477
  setline(477);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 476
  setline(476);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1527 = alloc_Float64(1);
// compilenode returning num1527
  *var_pos = num1527;
  if (num1527 == undefined)
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
// compilenode returning *var_pos
  *var_start = *var_pos;
  if (*var_pos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 479
  setline(479);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 478
  setline(478);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1528 == NULL) {
    strlit1528 = alloc_String("");
    gc_root(strlit1528);
  }
// compilenode returning strlit1528
  *var_nextImport = strlit1528;
  if (strlit1528 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 480
  setline(480);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1529 = gc_frame_new();
// Begin line 479
  setline(479);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// compilenode returning *var_pos
  gc_frame_newslot(*var_pos);
  params[0] = *var_line;
  params[1] = *var_pos;
  partcv[0] = 2;
  Object call1530 = callmethodflags(self, "skipWhiteSpace", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1529);
// compilenode returning call1530
  *var_pos = call1530;
  if (call1530 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 481
  setline(481);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 480
  setline(480);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  int op_slot_left_1532 = gc_frame_newslot(*var_pos);
  Object num1533 = alloc_Float64(1);
// compilenode returning num1533
  int op_slot_right_1532 = gc_frame_newslot(num1533);
  params[0] = num1533;
  partcv[0] = 1;
  Object sum1535 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1535
  *var_pos = sum1535;
  if (sum1535 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 482
  setline(482);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 481
  setline(481);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1538 == NULL) {
    strlit1538 = alloc_String("LINE = ");
    gc_root(strlit1538);
  }
// compilenode returning strlit1538
  int op_slot_left_1537 = gc_frame_newslot(strlit1538);
// compilenode returning *var_line
  int op_slot_right_1537 = gc_frame_newslot(*var_line);
  params[0] = *var_line;
  partcv[0] = 1;
  Object opresult1540 = callmethod(strlit1538, "++", 1, partcv, params);
// compilenode returning opresult1540
  params[0] = opresult1540;
  Object call1541 = gracelib_print(NULL, 1,  params);
// compilenode returning call1541
// Begin line 485
  setline(485);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1542 = gc_frame_new();
// Begin line 482
  setline(482);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1543 = alloc_Block(NULL, NULL, "grace-pkg", 482);
  gc_frame_newslot(block1543);
  block_savedest(block1543);
  Object closure1544 = createclosure(3, "_apply");
setclosureframe(closure1544, stackframe);
  addtoclosure(closure1544, var_pos);
  addtoclosure(closure1544, var_line);
  addtoclosure(closure1544, selfslot);
  struct UserObject *uo1544 = (struct UserObject*)block1543;
  uo1544->data[0] = (Object)closure1544;
  Method *meth_meth_grace_45_pkg__apply1544 = addmethod2pos(block1543, "_apply", &meth_grace_45_pkg__apply1544, 0);
int argcv_meth_grace_45_pkg__apply1544[] = {0};
meth_meth_grace_45_pkg__apply1544->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1544);
  meth_meth_grace_45_pkg__apply1544->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1544->definitionLine = 475;
// compilenode returning block1543
  gc_frame_newslot(block1543);
// Begin line 485
  setline(485);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1569 = alloc_Block(NULL, NULL, "grace-pkg", 485);
  gc_frame_newslot(block1569);
  block_savedest(block1569);
  Object closure1570 = createclosure(2, "_apply");
setclosureframe(closure1570, stackframe);
  addtoclosure(closure1570, var_pos);
  addtoclosure(closure1570, selfslot);
  struct UserObject *uo1570 = (struct UserObject*)block1569;
  uo1570->data[0] = (Object)closure1570;
  Method *meth_meth_grace_45_pkg__apply1570 = addmethod2pos(block1569, "_apply", &meth_grace_45_pkg__apply1570, 0);
int argcv_meth_grace_45_pkg__apply1570[] = {0};
meth_meth_grace_45_pkg__apply1570->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1570);
  meth_meth_grace_45_pkg__apply1570->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1570->definitionLine = 482;
// compilenode returning block1569
  gc_frame_newslot(block1569);
  params[0] = block1543;
  params[1] = block1569;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1577 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1542);
// compilenode returning call1577
// Begin line 486
  setline(486);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 485
  setline(485);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  int op_slot_left_1578 = gc_frame_newslot(*var_pos);
  Object num1579 = alloc_Float64(1);
// compilenode returning num1579
  int op_slot_right_1578 = gc_frame_newslot(num1579);
  params[0] = num1579;
  partcv[0] = 1;
  Object sum1581 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1581
  *var_pos = sum1581;
  if (sum1581 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 487
  setline(487);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1583 = gc_frame_new();
// Begin line 486
  setline(486);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1584 = gc_frame_new();
// compilenode returning *var_pos
  int op_slot_left_1585 = gc_frame_newslot(*var_pos);
  Object num1586 = alloc_Float64(1);
// compilenode returning num1586
  int op_slot_right_1585 = gc_frame_newslot(num1586);
  params[0] = num1586;
  partcv[0] = 1;
  Object sum1588 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1588
  gc_frame_newslot(sum1588);
  int callframe1589 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1590 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1589);
// compilenode returning call1590
// compilenode returning call1590
  gc_frame_newslot(call1590);
// compilenode returning *var_line
  params[0] = sum1588;
  params[1] = call1590;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1591 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1584);
// compilenode returning call1591
  gc_frame_newslot(call1591);
  params[0] = call1591;
  partcv[0] = 1;
  Object call1592 = callmethodflags(self, "removeExistingUrls", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1583);
// compilenode returning call1592
  *var_remainder = call1592;
  if (call1592 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 488
  setline(488);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 487
  setline(487);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1596 == NULL) {
    strlit1596 = alloc_String("import ""\x22""");
    gc_root(strlit1596);
  }
// compilenode returning strlit1596
  int op_slot_left_1595 = gc_frame_newslot(strlit1596);
// compilenode returning *var_prefix
  int op_slot_right_1595 = gc_frame_newslot(*var_prefix);
  params[0] = *var_prefix;
  partcv[0] = 1;
  Object opresult1598 = callmethod(strlit1596, "++", 1, partcv, params);
// compilenode returning opresult1598
  int op_slot_left_1594 = gc_frame_newslot(opresult1598);
  if (strlit1599 == NULL) {
    strlit1599 = alloc_String("/");
    gc_root(strlit1599);
  }
// compilenode returning strlit1599
  int op_slot_right_1594 = gc_frame_newslot(strlit1599);
  params[0] = strlit1599;
  partcv[0] = 1;
  Object opresult1601 = callmethod(opresult1598, "++", 1, partcv, params);
// compilenode returning opresult1601
  int op_slot_left_1593 = gc_frame_newslot(opresult1601);
// compilenode returning *var_remainder
  int op_slot_right_1593 = gc_frame_newslot(*var_remainder);
  params[0] = *var_remainder;
  partcv[0] = 1;
  Object opresult1603 = callmethod(opresult1601, "++", 1, partcv, params);
// compilenode returning opresult1603
  *var_line = opresult1603;
  if (opresult1603 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 489
  setline(489);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 488
  setline(488);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  params[0] = *var_line;
  Object call1605 = gracelib_print(NULL, 1,  params);
// compilenode returning call1605
// Begin line 492
  setline(492);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1606 = gc_frame_new();
// Begin line 489
  setline(489);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// Begin line 492
  setline(492);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1607 = alloc_Block(NULL, NULL, "grace-pkg", 492);
  gc_frame_newslot(block1607);
  block_savedest(block1607);
  Object closure1608 = createclosure(2, "_apply");
setclosureframe(closure1608, stackframe);
  addtoclosure(closure1608, var_outFile);
  addtoclosure(closure1608, selfslot);
  struct UserObject *uo1608 = (struct UserObject*)block1607;
  uo1608->data[0] = (Object)closure1608;
  Method *meth_meth_grace_45_pkg__apply1608 = addmethod2pos(block1607, "_apply", &meth_grace_45_pkg__apply1608, 0);
int argcv_meth_grace_45_pkg__apply1608[] = {1};
meth_meth_grace_45_pkg__apply1608->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1608);
  meth_meth_grace_45_pkg__apply1608->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1608->definitionLine = 486;
// compilenode returning block1607
  gc_frame_newslot(block1607);
  params[0] = *var_line;
  params[1] = block1607;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1612 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1606);
// compilenode returning call1612
// Begin line 493
  setline(493);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1613 = gc_frame_new();
// Begin line 492
  setline(492);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1614 == NULL) {
    strlit1614 = alloc_String("""\x0a""");
    gc_root(strlit1614);
  }
// compilenode returning strlit1614
  gc_frame_newslot(strlit1614);
// compilenode returning *var_outFile
  params[0] = strlit1614;
  partcv[0] = 1;
  Object call1615 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1613);
// compilenode returning call1615
    gc_frame_newslot(call1615);
    if1509 = call1615;
  } else {
// Begin line 498
  setline(498);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1616 = gc_frame_new();
// Begin line 495
  setline(495);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// Begin line 498
  setline(498);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1617 = alloc_Block(NULL, NULL, "grace-pkg", 498);
  gc_frame_newslot(block1617);
  block_savedest(block1617);
  Object closure1618 = createclosure(2, "_apply");
setclosureframe(closure1618, stackframe);
  addtoclosure(closure1618, var_outFile);
  addtoclosure(closure1618, selfslot);
  struct UserObject *uo1618 = (struct UserObject*)block1617;
  uo1618->data[0] = (Object)closure1618;
  Method *meth_meth_grace_45_pkg__apply1618 = addmethod2pos(block1617, "_apply", &meth_grace_45_pkg__apply1618, 0);
int argcv_meth_grace_45_pkg__apply1618[] = {1};
meth_meth_grace_45_pkg__apply1618->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1618);
  meth_meth_grace_45_pkg__apply1618->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1618->definitionLine = 486;
// compilenode returning block1617
  gc_frame_newslot(block1617);
  params[0] = *var_line;
  params[1] = block1617;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1622 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1616);
// compilenode returning call1622
// Begin line 499
  setline(499);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1623 = gc_frame_new();
// Begin line 498
  setline(498);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1624 == NULL) {
    strlit1624 = alloc_String("""\x0a""");
    gc_root(strlit1624);
  }
// compilenode returning strlit1624
  gc_frame_newslot(strlit1624);
// compilenode returning *var_outFile
  params[0] = strlit1624;
  partcv[0] = 1;
  Object call1625 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1623);
// compilenode returning call1625
    gc_frame_newslot(call1625);
    if1509 = call1625;
  }
// compilenode returning if1509
// Begin line 501
  setline(501);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 500
  setline(500);
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
Object meth_grace_45_pkg__apply1444(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 502
  setline(502);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1445 = gc_frame_new();
// Begin line 463
  setline(463);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1446 = alloc_Block(NULL, NULL, "grace-pkg", 463);
  gc_frame_newslot(block1446);
  block_savedest(block1446);
  Object closure1447 = createclosure(3, "_apply");
setclosureframe(closure1447, stackframe);
  addtoclosure(closure1447, var_curPos);
  addtoclosure(closure1447, var_readFile);
  addtoclosure(closure1447, selfslot);
  struct UserObject *uo1447 = (struct UserObject*)block1446;
  uo1447->data[0] = (Object)closure1447;
  Method *meth_meth_grace_45_pkg__apply1447 = addmethod2pos(block1446, "_apply", &meth_grace_45_pkg__apply1447, 0);
int argcv_meth_grace_45_pkg__apply1447[] = {0};
meth_meth_grace_45_pkg__apply1447->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1447);
  meth_meth_grace_45_pkg__apply1447->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1447->definitionLine = 462;
// compilenode returning block1446
  gc_frame_newslot(block1446);
// Begin line 502
  setline(502);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1454 = alloc_Block(NULL, NULL, "grace-pkg", 502);
  gc_frame_newslot(block1454);
  block_savedest(block1454);
  Object closure1455 = createclosure(6, "_apply");
setclosureframe(closure1455, stackframe);
  addtoclosure(closure1455, var_curPos);
  addtoclosure(closure1455, var_startPos);
  addtoclosure(closure1455, var_readFile);
  addtoclosure(closure1455, var_prefix);
  addtoclosure(closure1455, var_outFile);
  addtoclosure(closure1455, selfslot);
  struct UserObject *uo1455 = (struct UserObject*)block1454;
  uo1455->data[0] = (Object)closure1455;
  Method *meth_meth_grace_45_pkg__apply1455 = addmethod2pos(block1454, "_apply", &meth_grace_45_pkg__apply1455, 0);
int argcv_meth_grace_45_pkg__apply1455[] = {0};
meth_meth_grace_45_pkg__apply1455->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1455);
  meth_meth_grace_45_pkg__apply1455->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1455->definitionLine = 463;
// compilenode returning block1454
  gc_frame_newslot(block1454);
  params[0] = block1446;
  params[1] = block1454;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1632 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1445);
// compilenode returning call1632
  gc_frame_end(frame);
  return call1632;
}
Object meth_grace_45_pkg_parseAndPrefix1433(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 460
  setline(460);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 459
  setline(459);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1434 = alloc_Float64(1);
// compilenode returning num1434
  *var_curPos = num1434;
  if (num1434 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 461
  setline(461);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 460
  setline(460);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
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
  Object array1435 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array1435
  *var_outFile = array1435;
  if (array1435 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 503
  setline(503);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1436 = gc_frame_new();
// Begin line 462
  setline(462);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1438 = alloc_Float64(0);
// compilenode returning num1438
  int op_slot_left_1437 = gc_frame_newslot(num1438);
  int callframe1439 = gc_frame_new();
// compilenode returning *var_readFile
  partcv[0] = 0;
  Object call1440 = callmethod(*var_readFile, "size",
    1, partcv, params);
  gc_frame_end(callframe1439);
// compilenode returning call1440
// compilenode returning call1440
  int op_slot_right_1437 = gc_frame_newslot(call1440);
  params[0] = call1440;
  partcv[0] = 1;
  Object opresult1442 = callmethod(num1438, "..", 1, partcv, params);
// compilenode returning opresult1442
  gc_frame_newslot(opresult1442);
// Begin line 503
  setline(503);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1443 = alloc_Block(NULL, NULL, "grace-pkg", 503);
  gc_frame_newslot(block1443);
  block_savedest(block1443);
  Object closure1444 = createclosure(6, "_apply");
setclosureframe(closure1444, stackframe);
  addtoclosure(closure1444, var_curPos);
  addtoclosure(closure1444, var_readFile);
  addtoclosure(closure1444, var_startPos);
  addtoclosure(closure1444, var_prefix);
  addtoclosure(closure1444, var_outFile);
  addtoclosure(closure1444, selfslot);
  struct UserObject *uo1444 = (struct UserObject*)block1443;
  uo1444->data[0] = (Object)closure1444;
  Method *meth_meth_grace_45_pkg__apply1444 = addmethod2pos(block1443, "_apply", &meth_grace_45_pkg__apply1444, 0);
int argcv_meth_grace_45_pkg__apply1444[] = {1};
meth_meth_grace_45_pkg__apply1444->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1444);
  meth_meth_grace_45_pkg__apply1444->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1444->definitionLine = 462;
// compilenode returning block1443
  gc_frame_newslot(block1443);
  params[0] = opresult1442;
  params[1] = block1443;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1634 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1436);
// compilenode returning call1634
// Begin line 506
  setline(506);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1635 = gc_frame_new();
// Begin line 503
  setline(503);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1636 == NULL) {
    strlit1636 = alloc_String("""\x0a""");
    gc_root(strlit1636);
  }
// compilenode returning strlit1636
  gc_frame_newslot(strlit1636);
// compilenode returning *var_outFile
  params[0] = strlit1636;
  partcv[0] = 1;
  Object call1637 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1635);
// compilenode returning call1637
// Begin line 507
  setline(507);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 506
  setline(506);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1639 == NULL) {
    strlit1639 = alloc_String("OUTFILE ");
    gc_root(strlit1639);
  }
// compilenode returning strlit1639
  int op_slot_left_1638 = gc_frame_newslot(strlit1639);
// compilenode returning *var_outFile
  int op_slot_right_1638 = gc_frame_newslot(*var_outFile);
  params[0] = *var_outFile;
  partcv[0] = 1;
  Object opresult1641 = callmethod(strlit1639, "++", 1, partcv, params);
// compilenode returning opresult1641
  params[0] = opresult1641;
  Object call1642 = gracelib_print(NULL, 1,  params);
// compilenode returning call1642
// Begin line 509
  setline(509);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 507
  setline(507);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_outFile
  return *var_outFile;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_grace_45_pkg__apply1664(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1665 = gc_frame_newslot(*var_curPos);
  int callframe1666 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1667 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1666);
// compilenode returning call1667
// compilenode returning call1667
  int op_slot_right_1665 = gc_frame_newslot(call1667);
  params[0] = call1667;
  partcv[0] = 1;
  Object opresult1669 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1669
  gc_frame_end(frame);
  return opresult1669;
}
Object meth_grace_45_pkg__apply1672(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 523
  setline(523);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 520
  setline(520);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1675 = callmethod(*var_importStatement, "[]", 1, partcv, params);
// compilenode returning idxres1675
  int op_slot_left_1674 = gc_frame_newslot(idxres1675);
  if (strlit1676 == NULL) {
    strlit1676 = alloc_String("/");
    gc_root(strlit1676);
  }
// compilenode returning strlit1676
  int op_slot_right_1674 = gc_frame_newslot(strlit1676);
  params[0] = strlit1676;
  partcv[0] = 1;
  Object opresult1678 = callmethod(idxres1675, "==", 1, partcv, params);
// compilenode returning opresult1678
  Object if1673 = done;
  if (istrue(opresult1678)) {
// Begin line 522
  setline(522);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 521
  setline(521);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_lastIndex = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if1673 = done;
  } else {
  }
// compilenode returning if1673
// Begin line 524
  setline(524);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 523
  setline(523);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1680 = gc_frame_newslot(*var_curPos);
  Object num1681 = alloc_Float64(1);
// compilenode returning num1681
  int op_slot_right_1680 = gc_frame_newslot(num1681);
  params[0] = num1681;
  partcv[0] = 1;
  Object sum1683 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1683
  *var_curPos = sum1683;
  if (sum1683 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_grace_45_pkg_removeExistingUrls1643(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 516
  setline(516);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 513
  setline(513);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1646 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1647 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1646);
// compilenode returning call1647
// compilenode returning call1647
  int op_slot_left_1645 = gc_frame_newslot(call1647);
  Object num1648 = alloc_Float64(7);
// compilenode returning num1648
  int op_slot_right_1645 = gc_frame_newslot(num1648);
  params[0] = num1648;
  partcv[0] = 1;
  Object opresult1650 = callmethod(call1647, "<", 1, partcv, params);
// compilenode returning opresult1650
  Object if1644 = done;
  if (istrue(opresult1650)) {
// Begin line 515
  setline(515);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 514
  setline(514);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
  return *var_importStatement;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1644 = undefined;
  } else {
  }
// compilenode returning if1644
// Begin line 532
  setline(532);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 516
  setline(516);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1653 = gc_frame_new();
  Object num1654 = alloc_Float64(1);
// compilenode returning num1654
  gc_frame_newslot(num1654);
  Object num1655 = alloc_Float64(7);
// compilenode returning num1655
  gc_frame_newslot(num1655);
// compilenode returning *var_importStatement
  params[0] = num1654;
  params[1] = num1655;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1656 = callmethod(*var_importStatement, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1653);
// compilenode returning call1656
  int op_slot_left_1652 = gc_frame_newslot(call1656);
  if (strlit1657 == NULL) {
    strlit1657 = alloc_String("http://");
    gc_root(strlit1657);
  }
// compilenode returning strlit1657
  int op_slot_right_1652 = gc_frame_newslot(strlit1657);
  params[0] = strlit1657;
  partcv[0] = 1;
  Object opresult1659 = callmethod(call1656, "==", 1, partcv, params);
// compilenode returning opresult1659
  Object if1651 = done;
  if (istrue(opresult1659)) {
// Begin line 518
  setline(518);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 517
  setline(517);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1660 = alloc_Float64(7);
// compilenode returning num1660
  *var_lastIndex = num1660;
  if (num1660 == undefined)
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
  Object num1661 = alloc_Float64(7);
// compilenode returning num1661
  *var_curPos = num1661;
  if (num1661 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 525
  setline(525);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1662 = gc_frame_new();
// Begin line 519
  setline(519);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1663 = alloc_Block(NULL, NULL, "grace-pkg", 519);
  gc_frame_newslot(block1663);
  block_savedest(block1663);
  Object closure1664 = createclosure(3, "_apply");
setclosureframe(closure1664, stackframe);
  addtoclosure(closure1664, var_curPos);
  addtoclosure(closure1664, var_importStatement);
  addtoclosure(closure1664, selfslot);
  struct UserObject *uo1664 = (struct UserObject*)block1663;
  uo1664->data[0] = (Object)closure1664;
  Method *meth_meth_grace_45_pkg__apply1664 = addmethod2pos(block1663, "_apply", &meth_grace_45_pkg__apply1664, 0);
int argcv_meth_grace_45_pkg__apply1664[] = {0};
meth_meth_grace_45_pkg__apply1664->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1664);
  meth_meth_grace_45_pkg__apply1664->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1664->definitionLine = 513;
// compilenode returning block1663
  gc_frame_newslot(block1663);
// Begin line 525
  setline(525);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1671 = alloc_Block(NULL, NULL, "grace-pkg", 525);
  gc_frame_newslot(block1671);
  block_savedest(block1671);
  Object closure1672 = createclosure(4, "_apply");
setclosureframe(closure1672, stackframe);
  addtoclosure(closure1672, var_importStatement);
  addtoclosure(closure1672, var_curPos);
  addtoclosure(closure1672, var_lastIndex);
  addtoclosure(closure1672, selfslot);
  struct UserObject *uo1672 = (struct UserObject*)block1671;
  uo1672->data[0] = (Object)closure1672;
  Method *meth_meth_grace_45_pkg__apply1672 = addmethod2pos(block1671, "_apply", &meth_grace_45_pkg__apply1672, 0);
int argcv_meth_grace_45_pkg__apply1672[] = {0};
meth_meth_grace_45_pkg__apply1672->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1672);
  meth_meth_grace_45_pkg__apply1672->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1672->definitionLine = 519;
// compilenode returning block1671
  gc_frame_newslot(block1671);
  params[0] = block1663;
  params[1] = block1671;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1686 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1662);
// compilenode returning call1686
// Begin line 526
  setline(526);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1687 = gc_frame_new();
// Begin line 525
  setline(525);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_lastIndex
  int op_slot_left_1688 = gc_frame_newslot(*var_lastIndex);
  Object num1689 = alloc_Float64(1);
// compilenode returning num1689
  int op_slot_right_1688 = gc_frame_newslot(num1689);
  params[0] = num1689;
  partcv[0] = 1;
  Object sum1691 = callmethod(*var_lastIndex, "+", 1, partcv, params);
// compilenode returning sum1691
  gc_frame_newslot(sum1691);
  int callframe1692 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1693 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1692);
// compilenode returning call1693
// compilenode returning call1693
  gc_frame_newslot(call1693);
// compilenode returning *var_importStatement
  params[0] = sum1691;
  params[1] = call1693;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1694 = callmethod(*var_importStatement, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1687);
// compilenode returning call1694
  *var_res = call1694;
  if (call1694 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 527
  setline(527);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 526
  setline(526);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_res
  return *var_res;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1651 = undefined;
  } else {
// Begin line 530
  setline(530);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 529
  setline(529);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
  return *var_importStatement;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1651 = undefined;
  }
// compilenode returning if1651
  gc_frame_end(frame);
  return if1651;
}
Object meth_grace_45_pkg__apply1716(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1717 = gc_frame_newslot(*var_curPos);
  int callframe1718 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1719 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1718);
// compilenode returning call1719
// compilenode returning call1719
  int op_slot_right_1717 = gc_frame_newslot(call1719);
  params[0] = call1719;
  partcv[0] = 1;
  Object opresult1721 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1721
  gc_frame_end(frame);
  return opresult1721;
}
Object meth_grace_45_pkg__apply1724(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 546
  setline(546);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 543
  setline(543);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1727 = callmethod(*var_importStatement, "[]", 1, partcv, params);
// compilenode returning idxres1727
  int op_slot_left_1726 = gc_frame_newslot(idxres1727);
  if (strlit1728 == NULL) {
    strlit1728 = alloc_String("/");
    gc_root(strlit1728);
  }
// compilenode returning strlit1728
  int op_slot_right_1726 = gc_frame_newslot(strlit1728);
  params[0] = strlit1728;
  partcv[0] = 1;
  Object opresult1730 = callmethod(idxres1727, "==", 1, partcv, params);
// compilenode returning opresult1730
  Object if1725 = done;
  if (istrue(opresult1730)) {
// Begin line 545
  setline(545);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 544
  setline(544);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_lastIndex = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if1725 = done;
  } else {
  }
// compilenode returning if1725
// Begin line 547
  setline(547);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 546
  setline(546);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1732 = gc_frame_newslot(*var_curPos);
  Object num1733 = alloc_Float64(1);
// compilenode returning num1733
  int op_slot_right_1732 = gc_frame_newslot(num1733);
  params[0] = num1733;
  partcv[0] = 1;
  Object sum1735 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1735
  *var_curPos = sum1735;
  if (sum1735 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_grace_45_pkg_getBaseUrl1695(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 539
  setline(539);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 536
  setline(536);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1698 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1699 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1698);
// compilenode returning call1699
// compilenode returning call1699
  int op_slot_left_1697 = gc_frame_newslot(call1699);
  Object num1700 = alloc_Float64(7);
// compilenode returning num1700
  int op_slot_right_1697 = gc_frame_newslot(num1700);
  params[0] = num1700;
  partcv[0] = 1;
  Object opresult1702 = callmethod(call1699, "<", 1, partcv, params);
// compilenode returning opresult1702
  Object if1696 = done;
  if (istrue(opresult1702)) {
// Begin line 538
  setline(538);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 537
  setline(537);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
  return *var_importStatement;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1696 = undefined;
  } else {
  }
// compilenode returning if1696
// Begin line 555
  setline(555);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 539
  setline(539);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1705 = gc_frame_new();
  Object num1706 = alloc_Float64(1);
// compilenode returning num1706
  gc_frame_newslot(num1706);
  Object num1707 = alloc_Float64(7);
// compilenode returning num1707
  gc_frame_newslot(num1707);
// compilenode returning *var_importStatement
  params[0] = num1706;
  params[1] = num1707;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1708 = callmethod(*var_importStatement, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1705);
// compilenode returning call1708
  int op_slot_left_1704 = gc_frame_newslot(call1708);
  if (strlit1709 == NULL) {
    strlit1709 = alloc_String("http://");
    gc_root(strlit1709);
  }
// compilenode returning strlit1709
  int op_slot_right_1704 = gc_frame_newslot(strlit1709);
  params[0] = strlit1709;
  partcv[0] = 1;
  Object opresult1711 = callmethod(call1708, "==", 1, partcv, params);
// compilenode returning opresult1711
  Object if1703 = done;
  if (istrue(opresult1711)) {
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 540
  setline(540);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1712 = alloc_Float64(7);
// compilenode returning num1712
  *var_lastIndex = num1712;
  if (num1712 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 542
  setline(542);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1713 = alloc_Float64(7);
// compilenode returning num1713
  *var_curPos = num1713;
  if (num1713 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 548
  setline(548);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1714 = gc_frame_new();
// Begin line 542
  setline(542);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1715 = alloc_Block(NULL, NULL, "grace-pkg", 542);
  gc_frame_newslot(block1715);
  block_savedest(block1715);
  Object closure1716 = createclosure(3, "_apply");
setclosureframe(closure1716, stackframe);
  addtoclosure(closure1716, var_curPos);
  addtoclosure(closure1716, var_importStatement);
  addtoclosure(closure1716, selfslot);
  struct UserObject *uo1716 = (struct UserObject*)block1715;
  uo1716->data[0] = (Object)closure1716;
  Method *meth_meth_grace_45_pkg__apply1716 = addmethod2pos(block1715, "_apply", &meth_grace_45_pkg__apply1716, 0);
int argcv_meth_grace_45_pkg__apply1716[] = {0};
meth_meth_grace_45_pkg__apply1716->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1716);
  meth_meth_grace_45_pkg__apply1716->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1716->definitionLine = 536;
// compilenode returning block1715
  gc_frame_newslot(block1715);
// Begin line 548
  setline(548);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1723 = alloc_Block(NULL, NULL, "grace-pkg", 548);
  gc_frame_newslot(block1723);
  block_savedest(block1723);
  Object closure1724 = createclosure(4, "_apply");
setclosureframe(closure1724, stackframe);
  addtoclosure(closure1724, var_importStatement);
  addtoclosure(closure1724, var_curPos);
  addtoclosure(closure1724, var_lastIndex);
  addtoclosure(closure1724, selfslot);
  struct UserObject *uo1724 = (struct UserObject*)block1723;
  uo1724->data[0] = (Object)closure1724;
  Method *meth_meth_grace_45_pkg__apply1724 = addmethod2pos(block1723, "_apply", &meth_grace_45_pkg__apply1724, 0);
int argcv_meth_grace_45_pkg__apply1724[] = {0};
meth_meth_grace_45_pkg__apply1724->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1724);
  meth_meth_grace_45_pkg__apply1724->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1724->definitionLine = 542;
// compilenode returning block1723
  gc_frame_newslot(block1723);
  params[0] = block1715;
  params[1] = block1723;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1738 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1714);
// compilenode returning call1738
// Begin line 549
  setline(549);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1739 = gc_frame_new();
// Begin line 548
  setline(548);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1740 = alloc_Float64(1);
// compilenode returning num1740
  gc_frame_newslot(num1740);
// compilenode returning *var_lastIndex
  gc_frame_newslot(*var_lastIndex);
// compilenode returning *var_importStatement
  params[0] = num1740;
  params[1] = *var_lastIndex;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1741 = callmethod(*var_importStatement, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1739);
// compilenode returning call1741
  *var_res = call1741;
  if (call1741 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 550
  setline(550);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 549
  setline(549);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_res
  return *var_res;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1703 = undefined;
  } else {
// Begin line 553
  setline(553);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 552
  setline(552);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
  return *var_importStatement;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1703 = undefined;
  }
// compilenode returning if1703
  gc_frame_end(frame);
  return if1703;
}
Object meth_grace_45_pkg__apply1749(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1750 = gc_frame_newslot(*var_count);
  int callframe1751 = gc_frame_new();
// compilenode returning *var_st
  partcv[0] = 0;
  Object call1752 = callmethod(*var_st, "size",
    1, partcv, params);
  gc_frame_end(callframe1751);
// compilenode returning call1752
// compilenode returning call1752
  int op_slot_right_1750 = gc_frame_newslot(call1752);
  params[0] = call1752;
  partcv[0] = 1;
  Object opresult1754 = callmethod(*var_count, "<=", 1, partcv, params);
// compilenode returning opresult1754
  gc_frame_end(frame);
  return opresult1754;
}
Object meth_grace_45_pkg__apply1757(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 564
  setline(564);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 561
  setline(561);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_st
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1760 = callmethod(*var_st, "[]", 1, partcv, params);
// compilenode returning idxres1760
  int op_slot_left_1759 = gc_frame_newslot(idxres1760);
  if (strlit1761 == NULL) {
    strlit1761 = alloc_String("/");
    gc_root(strlit1761);
  }
// compilenode returning strlit1761
  int op_slot_right_1759 = gc_frame_newslot(strlit1761);
  params[0] = strlit1761;
  partcv[0] = 1;
  Object opresult1763 = callmethod(idxres1760, "==", 1, partcv, params);
// compilenode returning opresult1763
  Object if1758 = done;
  if (istrue(opresult1763)) {
// Begin line 563
  setline(563);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 562
  setline(562);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  *var_lastIndex = *var_count;
  if (*var_count == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if1758 = done;
  } else {
  }
// compilenode returning if1758
// Begin line 565
  setline(565);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 564
  setline(564);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  int op_slot_left_1765 = gc_frame_newslot(*var_count);
  Object num1766 = alloc_Float64(1);
// compilenode returning num1766
  int op_slot_right_1765 = gc_frame_newslot(num1766);
  params[0] = num1766;
  partcv[0] = 1;
  Object sum1768 = callmethod(*var_count, "+", 1, partcv, params);
// compilenode returning sum1768
  *var_count = sum1768;
  if (sum1768 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_grace_45_pkg_removeContainingDir1742(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 559
  setline(559);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 558
  setline(558);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1743 = alloc_Float64(1);
// compilenode returning num1743
  *var_count = num1743;
  if (num1743 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 560
  setline(560);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1744 = gc_frame_new();
// Begin line 559
  setline(559);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1745 = alloc_Float64(1);
// compilenode returning num1745
  partcv[0] = 0;
  Object call1746 = callmethod(num1745, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1744);
// compilenode returning call1746
  *var_lastIndex = call1746;
  if (call1746 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 566
  setline(566);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1747 = gc_frame_new();
// Begin line 560
  setline(560);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1748 = alloc_Block(NULL, NULL, "grace-pkg", 560);
  gc_frame_newslot(block1748);
  block_savedest(block1748);
  Object closure1749 = createclosure(3, "_apply");
setclosureframe(closure1749, stackframe);
  addtoclosure(closure1749, var_count);
  addtoclosure(closure1749, var_st);
  addtoclosure(closure1749, selfslot);
  struct UserObject *uo1749 = (struct UserObject*)block1748;
  uo1749->data[0] = (Object)closure1749;
  Method *meth_meth_grace_45_pkg__apply1749 = addmethod2pos(block1748, "_apply", &meth_grace_45_pkg__apply1749, 0);
int argcv_meth_grace_45_pkg__apply1749[] = {0};
meth_meth_grace_45_pkg__apply1749->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1749);
  meth_meth_grace_45_pkg__apply1749->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1749->definitionLine = 542;
// compilenode returning block1748
  gc_frame_newslot(block1748);
// Begin line 566
  setline(566);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1756 = alloc_Block(NULL, NULL, "grace-pkg", 566);
  gc_frame_newslot(block1756);
  block_savedest(block1756);
  Object closure1757 = createclosure(4, "_apply");
setclosureframe(closure1757, stackframe);
  addtoclosure(closure1757, var_st);
  addtoclosure(closure1757, var_count);
  addtoclosure(closure1757, var_lastIndex);
  addtoclosure(closure1757, selfslot);
  struct UserObject *uo1757 = (struct UserObject*)block1756;
  uo1757->data[0] = (Object)closure1757;
  Method *meth_meth_grace_45_pkg__apply1757 = addmethod2pos(block1756, "_apply", &meth_grace_45_pkg__apply1757, 0);
int argcv_meth_grace_45_pkg__apply1757[] = {0};
meth_meth_grace_45_pkg__apply1757->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1757);
  meth_meth_grace_45_pkg__apply1757->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1757->definitionLine = 560;
// compilenode returning block1756
  gc_frame_newslot(block1756);
  params[0] = block1748;
  params[1] = block1756;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1771 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1747);
// compilenode returning call1771
// Begin line 569
  setline(569);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 566
  setline(566);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_lastIndex
  int op_slot_left_1773 = gc_frame_newslot(*var_lastIndex);
  int callframe1774 = gc_frame_new();
  Object num1775 = alloc_Float64(1);
// compilenode returning num1775
  partcv[0] = 0;
  Object call1776 = callmethod(num1775, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1774);
// compilenode returning call1776
  int op_slot_right_1773 = gc_frame_newslot(call1776);
  params[0] = call1776;
  partcv[0] = 1;
  Object opresult1778 = callmethod(*var_lastIndex, "==", 1, partcv, params);
// compilenode returning opresult1778
  Object if1772 = done;
  if (istrue(opresult1778)) {
// Begin line 568
  setline(568);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 567
  setline(567);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_st
  return *var_st;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1772 = undefined;
  } else {
  }
// compilenode returning if1772
// Begin line 569
  setline(569);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1779 = gc_frame_new();
// compilenode returning *var_lastIndex
  int op_slot_left_1780 = gc_frame_newslot(*var_lastIndex);
  Object num1781 = alloc_Float64(1);
// compilenode returning num1781
  int op_slot_right_1780 = gc_frame_newslot(num1781);
  params[0] = num1781;
  partcv[0] = 1;
  Object sum1783 = callmethod(*var_lastIndex, "+", 1, partcv, params);
// compilenode returning sum1783
  gc_frame_newslot(sum1783);
  int callframe1784 = gc_frame_new();
// compilenode returning *var_st
  partcv[0] = 0;
  Object call1785 = callmethod(*var_st, "size",
    1, partcv, params);
  gc_frame_end(callframe1784);
// compilenode returning call1785
// compilenode returning call1785
  gc_frame_newslot(call1785);
// compilenode returning *var_st
  params[0] = sum1783;
  params[1] = call1785;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1786 = callmethod(*var_st, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1779);
// compilenode returning call1786
  return call1786;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_grace_45_pkg__apply1794(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1795 = gc_frame_newslot(*var_count);
  int callframe1796 = gc_frame_new();
// compilenode returning *var_st
  partcv[0] = 0;
  Object call1797 = callmethod(*var_st, "size",
    1, partcv, params);
  gc_frame_end(callframe1796);
// compilenode returning call1797
// compilenode returning call1797
  int op_slot_right_1795 = gc_frame_newslot(call1797);
  params[0] = call1797;
  partcv[0] = 1;
  Object opresult1799 = callmethod(*var_count, "<=", 1, partcv, params);
// compilenode returning opresult1799
  gc_frame_end(frame);
  return opresult1799;
}
Object meth_grace_45_pkg__apply1802(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 579
  setline(579);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 576
  setline(576);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_st
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1805 = callmethod(*var_st, "[]", 1, partcv, params);
// compilenode returning idxres1805
  int op_slot_left_1804 = gc_frame_newslot(idxres1805);
  if (strlit1806 == NULL) {
    strlit1806 = alloc_String("/");
    gc_root(strlit1806);
  }
// compilenode returning strlit1806
  int op_slot_right_1804 = gc_frame_newslot(strlit1806);
  params[0] = strlit1806;
  partcv[0] = 1;
  Object opresult1808 = callmethod(idxres1805, "==", 1, partcv, params);
// compilenode returning opresult1808
  Object if1803 = done;
  if (istrue(opresult1808)) {
// Begin line 578
  setline(578);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 577
  setline(577);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  *var_last = *var_count;
  if (*var_count == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if1803 = done;
  } else {
  }
// compilenode returning if1803
// Begin line 580
  setline(580);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 579
  setline(579);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  int op_slot_left_1810 = gc_frame_newslot(*var_count);
  Object num1811 = alloc_Float64(1);
// compilenode returning num1811
  int op_slot_right_1810 = gc_frame_newslot(num1811);
  params[0] = num1811;
  partcv[0] = 1;
  Object sum1813 = callmethod(*var_count, "+", 1, partcv, params);
// compilenode returning sum1813
  *var_count = sum1813;
  if (sum1813 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_grace_45_pkg_getContainingDirectory1787(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 574
  setline(574);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 573
  setline(573);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1788 = alloc_Float64(1);
// compilenode returning num1788
  *var_count = num1788;
  if (num1788 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1789 = gc_frame_new();
// Begin line 574
  setline(574);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1790 = alloc_Float64(1);
// compilenode returning num1790
  partcv[0] = 0;
  Object call1791 = callmethod(num1790, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1789);
// compilenode returning call1791
  *var_last = call1791;
  if (call1791 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 581
  setline(581);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1792 = gc_frame_new();
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1793 = alloc_Block(NULL, NULL, "grace-pkg", 575);
  gc_frame_newslot(block1793);
  block_savedest(block1793);
  Object closure1794 = createclosure(3, "_apply");
setclosureframe(closure1794, stackframe);
  addtoclosure(closure1794, var_count);
  addtoclosure(closure1794, var_st);
  addtoclosure(closure1794, selfslot);
  struct UserObject *uo1794 = (struct UserObject*)block1793;
  uo1794->data[0] = (Object)closure1794;
  Method *meth_meth_grace_45_pkg__apply1794 = addmethod2pos(block1793, "_apply", &meth_grace_45_pkg__apply1794, 0);
int argcv_meth_grace_45_pkg__apply1794[] = {0};
meth_meth_grace_45_pkg__apply1794->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1794);
  meth_meth_grace_45_pkg__apply1794->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1794->definitionLine = 569;
// compilenode returning block1793
  gc_frame_newslot(block1793);
// Begin line 581
  setline(581);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1801 = alloc_Block(NULL, NULL, "grace-pkg", 581);
  gc_frame_newslot(block1801);
  block_savedest(block1801);
  Object closure1802 = createclosure(4, "_apply");
setclosureframe(closure1802, stackframe);
  addtoclosure(closure1802, var_st);
  addtoclosure(closure1802, var_count);
  addtoclosure(closure1802, var_last);
  addtoclosure(closure1802, selfslot);
  struct UserObject *uo1802 = (struct UserObject*)block1801;
  uo1802->data[0] = (Object)closure1802;
  Method *meth_meth_grace_45_pkg__apply1802 = addmethod2pos(block1801, "_apply", &meth_grace_45_pkg__apply1802, 0);
int argcv_meth_grace_45_pkg__apply1802[] = {0};
meth_meth_grace_45_pkg__apply1802->type = alloc_MethodType(1, argcv_meth_grace_45_pkg__apply1802);
  meth_meth_grace_45_pkg__apply1802->definitionModule = modulename;
  meth_meth_grace_45_pkg__apply1802->definitionLine = 575;
// compilenode returning block1801
  gc_frame_newslot(block1801);
  params[0] = block1793;
  params[1] = block1801;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1816 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1792);
// compilenode returning call1816
// Begin line 584
  setline(584);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 581
  setline(581);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_last
  int op_slot_left_1818 = gc_frame_newslot(*var_last);
  int callframe1819 = gc_frame_new();
  Object num1820 = alloc_Float64(1);
// compilenode returning num1820
  partcv[0] = 0;
  Object call1821 = callmethod(num1820, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1819);
// compilenode returning call1821
  int op_slot_right_1818 = gc_frame_newslot(call1821);
  params[0] = call1821;
  partcv[0] = 1;
  Object opresult1823 = callmethod(*var_last, "==", 1, partcv, params);
// compilenode returning opresult1823
  Object if1817 = done;
  if (istrue(opresult1823)) {
// Begin line 583
  setline(583);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 582
  setline(582);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1825 = gc_frame_new();
  partcv[0] = 0;
  Object call1826 = callmethodflags(self, "getBuildPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1825);
// compilenode returning call1826
  int op_slot_left_1824 = gc_frame_newslot(call1826);
  if (strlit1827 == NULL) {
    strlit1827 = alloc_String("/");
    gc_root(strlit1827);
  }
// compilenode returning strlit1827
  int op_slot_right_1824 = gc_frame_newslot(strlit1827);
  params[0] = strlit1827;
  partcv[0] = 1;
  Object opresult1829 = callmethod(call1826, "++", 1, partcv, params);
// compilenode returning opresult1829
  return opresult1829;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1817 = undefined;
  } else {
  }
// compilenode returning if1817
// Begin line 586
  setline(586);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1830 = gc_frame_new();
// Begin line 584
  setline(584);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1831 = alloc_Float64(0);
// compilenode returning num1831
  gc_frame_newslot(num1831);
// compilenode returning *var_last
  int op_slot_left_1832 = gc_frame_newslot(*var_last);
  Object num1833 = alloc_Float64(1);
// compilenode returning num1833
  int op_slot_right_1832 = gc_frame_newslot(num1833);
  params[0] = num1833;
  partcv[0] = 1;
  Object diff1835 = callmethod(*var_last, "-", 1, partcv, params);
// compilenode returning diff1835
  gc_frame_newslot(diff1835);
// compilenode returning *var_st
  params[0] = num1831;
  params[1] = diff1835;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1836 = callmethod(*var_st, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1830);
// compilenode returning call1836
  return call1836;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_grace_45_pkg_imported1837(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 31, (flags>>24)&0xff);
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
Object meth_grace_45_pkg_imported_58__61_1838(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 32, (flags>>24)&0xff);
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
Object meth_grace_45_pkg_toProcess1841(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 33, (flags>>24)&0xff);
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
Object meth_grace_45_pkg_toProcess_58__61_1842(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 34, (flags>>24)&0xff);
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
Object meth_grace_45_pkg_verbose1845(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 35, (flags>>24)&0xff);
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
Object meth_grace_45_pkg_verbose_58__61_1846(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 36, (flags>>24)&0xff);
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
Object meth_grace_45_pkg_global1849(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 37, (flags>>24)&0xff);
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
Object meth_grace_45_pkg_global_58__61_1850(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 38, (flags>>24)&0xff);
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
Object meth_grace_45_pkg_bundlePath1853(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 39, (flags>>24)&0xff);
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
Object meth_grace_45_pkg_bundlePath_58__61_1854(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 40, (flags>>24)&0xff);
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
Object meth_grace_45_pkg_baseUrl1857(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 41, (flags>>24)&0xff);
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
Object meth_grace_45_pkg_baseUrl_58__61_1858(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 42, (flags>>24)&0xff);
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
Object meth_grace_45_pkg_curFile1860(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 43, (flags>>24)&0xff);
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
Object meth_grace_45_pkg_curFile_58__61_1861(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 44, (flags>>24)&0xff);
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
Object module_grace_45_pkg_init() {
  int flags = 0;
  int frame = gc_frame_new();
  Object self = alloc_obj2(47, 47);
  self->class->definitionModule = modulename;
  gc_root(self);
  prelude = module_StandardPrelude_init();
  adddatum2(self, prelude, 0);
  addmethod2(self, "outer", &grace_userobj_outer);
  setline(1);
  setmodule(modulename);
  setsource(originalSourceLines);
  setclassname(self, "Module<grace-pkg>");
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
  struct StackFrameObject *stackframe = alloc_StackFrame(47, NULL);
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
// Begin line 15
  setline(15);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure0 = createclosure(2, "parseArgs");
setclosureframe(closure0, stackframe);
  addtoclosure(closure0, var_verbose);
  addtoclosure(closure0, var_global);
  struct UserObject *uo0 = (struct UserObject*)self;
  uo0->data[1] = (Object)closure0;
  Method *meth_meth_grace_45_pkg_parseArgs0 = addmethod2pos(self, "parseArgs", &meth_grace_45_pkg_parseArgs0, 1);
int argcv_meth_grace_45_pkg_parseArgs0[] = {1};
meth_meth_grace_45_pkg_parseArgs0->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_parseArgs0);
  meth_meth_grace_45_pkg_parseArgs0->definitionModule = modulename;
  meth_meth_grace_45_pkg_parseArgs0->definitionLine = 15;
// compilenode returning 
// Begin line 41
  setline(41);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo60 = (struct UserObject*)self;
  uo60->data[2] = emptyclosure;
  Method *meth_meth_grace_45_pkg_forArgs_40__41_do60 = addmethod2pos(self, "forArgs()do", &meth_grace_45_pkg_forArgs_40__41_do60, 2);
int argcv_meth_grace_45_pkg_forArgs_40__41_do60[] = {1, 1};
meth_meth_grace_45_pkg_forArgs_40__41_do60->type = alloc_MethodType(2, argcv_meth_grace_45_pkg_forArgs_40__41_do60);
  meth_meth_grace_45_pkg_forArgs_40__41_do60->flags |= MFLAG_CONFIDENTIAL;
  meth_meth_grace_45_pkg_forArgs_40__41_do60->definitionModule = modulename;
  meth_meth_grace_45_pkg_forArgs_40__41_do60->definitionLine = 41;
  Method *meth_meth_grace_45_pkg_forArgs_40__41_do60_object = addmethod2pos(self, "forArgs()do()object", &meth_grace_45_pkg_forArgs_40__41_do60_object, 2);
int argcv_meth_grace_45_pkg_forArgs_40__41_do60_object[] = {1, 1};
meth_meth_grace_45_pkg_forArgs_40__41_do60_object->type = alloc_MethodType(2, argcv_meth_grace_45_pkg_forArgs_40__41_do60_object);
  meth_meth_grace_45_pkg_forArgs_40__41_do60_object->flags |= MFLAG_CONFIDENTIAL;
  meth_meth_grace_45_pkg_forArgs_40__41_do60_object->definitionModule = modulename;
  meth_meth_grace_45_pkg_forArgs_40__41_do60_object->definitionLine = 41;
// compilenode returning 
// Begin line 96
  setline(96);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure213 = createclosure(1, "listInstalled");
setclosureframe(closure213, stackframe);
  addtoclosure(closure213, var_sys);
  struct UserObject *uo213 = (struct UserObject*)self;
  uo213->data[3] = (Object)closure213;
  Method *meth_meth_grace_45_pkg_listInstalled213 = addmethod2pos(self, "listInstalled", &meth_grace_45_pkg_listInstalled213, 3);
int argcv_meth_grace_45_pkg_listInstalled213[] = {0};
meth_meth_grace_45_pkg_listInstalled213->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_listInstalled213);
  meth_meth_grace_45_pkg_listInstalled213->definitionModule = modulename;
  meth_meth_grace_45_pkg_listInstalled213->definitionLine = 96;
// compilenode returning 
// Begin line 104
  setline(104);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure242 = createclosure(1, "checkListPath");
setclosureframe(closure242, stackframe);
  addtoclosure(closure242, var_io);
  struct UserObject *uo242 = (struct UserObject*)self;
  uo242->data[4] = (Object)closure242;
  Method *meth_meth_grace_45_pkg_checkListPath242 = addmethod2pos(self, "checkListPath", &meth_grace_45_pkg_checkListPath242, 4);
int argcv_meth_grace_45_pkg_checkListPath242[] = {1};
meth_meth_grace_45_pkg_checkListPath242->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_checkListPath242);
meth_meth_grace_45_pkg_checkListPath242->type->types[0] = type_String;
meth_meth_grace_45_pkg_checkListPath242->type->names[0] = "path";
  meth_meth_grace_45_pkg_checkListPath242->definitionModule = modulename;
  meth_meth_grace_45_pkg_checkListPath242->definitionLine = 104;
// compilenode returning 
// Begin line 112
  setline(112);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure258 = createclosure(1, "recurseDirectory");
setclosureframe(closure258, stackframe);
  addtoclosure(closure258, var_io);
  struct UserObject *uo258 = (struct UserObject*)self;
  uo258->data[5] = (Object)closure258;
  Method *meth_meth_grace_45_pkg_recurseDirectory258 = addmethod2pos(self, "recurseDirectory", &meth_grace_45_pkg_recurseDirectory258, 5);
int argcv_meth_grace_45_pkg_recurseDirectory258[] = {2};
meth_meth_grace_45_pkg_recurseDirectory258->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_recurseDirectory258);
  meth_meth_grace_45_pkg_recurseDirectory258->definitionModule = modulename;
  meth_meth_grace_45_pkg_recurseDirectory258->definitionLine = 112;
// compilenode returning 
// Begin line 124
  setline(124);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure311 = createclosure(1, "doGet");
setclosureframe(closure311, stackframe);
  addtoclosure(closure311, var_imported);
  struct UserObject *uo311 = (struct UserObject*)self;
  uo311->data[6] = (Object)closure311;
  Method *meth_meth_grace_45_pkg_doGet311 = addmethod2pos(self, "doGet", &meth_grace_45_pkg_doGet311, 6);
int argcv_meth_grace_45_pkg_doGet311[] = {1};
meth_meth_grace_45_pkg_doGet311->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_doGet311);
  meth_meth_grace_45_pkg_doGet311->definitionModule = modulename;
  meth_meth_grace_45_pkg_doGet311->definitionLine = 124;
// compilenode returning 
// Begin line 138
  setline(138);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure350 = createclosure(1, "setBaseUrl");
setclosureframe(closure350, stackframe);
  addtoclosure(closure350, var_baseUrl);
  struct UserObject *uo350 = (struct UserObject*)self;
  uo350->data[7] = (Object)closure350;
  Method *meth_meth_grace_45_pkg_setBaseUrl350 = addmethod2pos(self, "setBaseUrl", &meth_grace_45_pkg_setBaseUrl350, 7);
int argcv_meth_grace_45_pkg_setBaseUrl350[] = {1};
meth_meth_grace_45_pkg_setBaseUrl350->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_setBaseUrl350);
meth_meth_grace_45_pkg_setBaseUrl350->type->types[0] = type_String;
meth_meth_grace_45_pkg_setBaseUrl350->type->names[0] = "baseAddress";
  meth_meth_grace_45_pkg_setBaseUrl350->definitionModule = modulename;
  meth_meth_grace_45_pkg_setBaseUrl350->definitionLine = 138;
// compilenode returning 
// Begin line 142
  setline(142);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure354 = createclosure(1, "setFile");
setclosureframe(closure354, stackframe);
  addtoclosure(closure354, var_curFile);
  struct UserObject *uo354 = (struct UserObject*)self;
  uo354->data[8] = (Object)closure354;
  Method *meth_meth_grace_45_pkg_setFile354 = addmethod2pos(self, "setFile", &meth_grace_45_pkg_setFile354, 8);
int argcv_meth_grace_45_pkg_setFile354[] = {1};
meth_meth_grace_45_pkg_setFile354->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_setFile354);
  meth_meth_grace_45_pkg_setFile354->definitionModule = modulename;
  meth_meth_grace_45_pkg_setFile354->definitionLine = 142;
// compilenode returning 
// Begin line 154
  setline(154);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure377 = createclosure(3, "fetchImports");
setclosureframe(closure377, stackframe);
  addtoclosure(closure377, var_curFile);
  addtoclosure(closure377, var_imported);
  addtoclosure(closure377, var_toProcess);
  struct UserObject *uo377 = (struct UserObject*)self;
  uo377->data[9] = (Object)closure377;
  Method *meth_meth_grace_45_pkg_fetchImports377 = addmethod2pos(self, "fetchImports", &meth_grace_45_pkg_fetchImports377, 9);
int argcv_meth_grace_45_pkg_fetchImports377[] = {1};
meth_meth_grace_45_pkg_fetchImports377->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_fetchImports377);
  meth_meth_grace_45_pkg_fetchImports377->definitionModule = modulename;
  meth_meth_grace_45_pkg_fetchImports377->definitionLine = 154;
// compilenode returning 
// Begin line 179
  setline(179);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure453 = createclosure(1, "performCurlFetch");
setclosureframe(closure453, stackframe);
  addtoclosure(closure453, var_curl);
  struct UserObject *uo453 = (struct UserObject*)self;
  uo453->data[10] = (Object)closure453;
  Method *meth_meth_grace_45_pkg_performCurlFetch453 = addmethod2pos(self, "performCurlFetch", &meth_grace_45_pkg_performCurlFetch453, 10);
int argcv_meth_grace_45_pkg_performCurlFetch453[] = {1};
meth_meth_grace_45_pkg_performCurlFetch453->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_performCurlFetch453);
  meth_meth_grace_45_pkg_performCurlFetch453->definitionModule = modulename;
  meth_meth_grace_45_pkg_performCurlFetch453->definitionLine = 179;
// compilenode returning 
// Begin line 196
  setline(196);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure512 = createclosure(1, "setFileData");
setclosureframe(closure512, stackframe);
  addtoclosure(closure512, var_baseUrl);
  struct UserObject *uo512 = (struct UserObject*)self;
  uo512->data[11] = (Object)closure512;
  Method *meth_meth_grace_45_pkg_setFileData512 = addmethod2pos(self, "setFileData", &meth_grace_45_pkg_setFileData512, 11);
int argcv_meth_grace_45_pkg_setFileData512[] = {1};
meth_meth_grace_45_pkg_setFileData512->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_setFileData512);
  meth_meth_grace_45_pkg_setFileData512->definitionModule = modulename;
  meth_meth_grace_45_pkg_setFileData512->definitionLine = 196;
// compilenode returning 
// Begin line 227
  setline(227);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure590 = createclosure(3, "findExisting");
setclosureframe(closure590, stackframe);
  addtoclosure(closure590, var_io);
  addtoclosure(closure590, var_sys);
  addtoclosure(closure590, var_bundlePath);
  struct UserObject *uo590 = (struct UserObject*)self;
  uo590->data[12] = (Object)closure590;
  Method *meth_meth_grace_45_pkg_findExisting590 = addmethod2pos(self, "findExisting", &meth_grace_45_pkg_findExisting590, 12);
int argcv_meth_grace_45_pkg_findExisting590[] = {1};
meth_meth_grace_45_pkg_findExisting590->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_findExisting590);
  meth_meth_grace_45_pkg_findExisting590->definitionModule = modulename;
  meth_meth_grace_45_pkg_findExisting590->definitionLine = 227;
// compilenode returning 
// Begin line 257
  setline(257);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo787 = (struct UserObject*)self;
  uo787->data[13] = emptyclosure;
  Method *meth_meth_grace_45_pkg_validateFile787 = addmethod2pos(self, "validateFile", &meth_grace_45_pkg_validateFile787, 13);
int argcv_meth_grace_45_pkg_validateFile787[] = {1};
meth_meth_grace_45_pkg_validateFile787->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_validateFile787);
  meth_meth_grace_45_pkg_validateFile787->definitionModule = modulename;
  meth_meth_grace_45_pkg_validateFile787->definitionLine = 257;
// compilenode returning 
// Begin line 267
  setline(267);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure810 = createclosure(3, "write");
setclosureframe(closure810, stackframe);
  addtoclosure(closure810, var_global);
  addtoclosure(closure810, var_sys);
  addtoclosure(closure810, var_io);
  struct UserObject *uo810 = (struct UserObject*)self;
  uo810->data[14] = (Object)closure810;
  Method *meth_meth_grace_45_pkg_write810 = addmethod2pos(self, "write", &meth_grace_45_pkg_write810, 14);
int argcv_meth_grace_45_pkg_write810[] = {1};
meth_meth_grace_45_pkg_write810->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_write810);
  meth_meth_grace_45_pkg_write810->definitionModule = modulename;
  meth_meth_grace_45_pkg_write810->definitionLine = 267;
// compilenode returning 
// Begin line 282
  setline(282);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure847 = createclosure(2, "getBuildPath");
setclosureframe(closure847, stackframe);
  addtoclosure(closure847, var_sys);
  addtoclosure(closure847, var_io);
  struct UserObject *uo847 = (struct UserObject*)self;
  uo847->data[15] = (Object)closure847;
  Method *meth_meth_grace_45_pkg_getBuildPath847 = addmethod2pos(self, "getBuildPath", &meth_grace_45_pkg_getBuildPath847, 15);
int argcv_meth_grace_45_pkg_getBuildPath847[] = {0};
meth_meth_grace_45_pkg_getBuildPath847->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_getBuildPath847);
  meth_meth_grace_45_pkg_getBuildPath847->definitionModule = modulename;
  meth_meth_grace_45_pkg_getBuildPath847->definitionLine = 282;
// compilenode returning 
// Begin line 299
  setline(299);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure885 = createclosure(3, "compile");
setclosureframe(closure885, stackframe);
  addtoclosure(closure885, var_global);
  addtoclosure(closure885, var_sys);
  addtoclosure(closure885, var_io);
  struct UserObject *uo885 = (struct UserObject*)self;
  uo885->data[16] = (Object)closure885;
  Method *meth_meth_grace_45_pkg_compile885 = addmethod2pos(self, "compile", &meth_grace_45_pkg_compile885, 16);
int argcv_meth_grace_45_pkg_compile885[] = {1};
meth_meth_grace_45_pkg_compile885->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_compile885);
  meth_meth_grace_45_pkg_compile885->definitionModule = modulename;
  meth_meth_grace_45_pkg_compile885->definitionLine = 299;
// compilenode returning 
// Begin line 315
  setline(315);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure925 = createclosure(1, "createDirectory");
setclosureframe(closure925, stackframe);
  addtoclosure(closure925, var_io);
  struct UserObject *uo925 = (struct UserObject*)self;
  uo925->data[17] = (Object)closure925;
  Method *meth_meth_grace_45_pkg_createDirectory925 = addmethod2pos(self, "createDirectory", &meth_grace_45_pkg_createDirectory925, 17);
int argcv_meth_grace_45_pkg_createDirectory925[] = {1};
meth_meth_grace_45_pkg_createDirectory925->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_createDirectory925);
  meth_meth_grace_45_pkg_createDirectory925->definitionModule = modulename;
  meth_meth_grace_45_pkg_createDirectory925->definitionLine = 315;
// compilenode returning 
// Begin line 339
  setline(339);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1003 = createclosure(1, "parseFile");
setclosureframe(closure1003, stackframe);
  addtoclosure(closure1003, var_done);
  struct UserObject *uo1003 = (struct UserObject*)self;
  uo1003->data[18] = (Object)closure1003;
  Method *meth_meth_grace_45_pkg_parseFile1003 = addmethod2pos(self, "parseFile", &meth_grace_45_pkg_parseFile1003, 18);
int argcv_meth_grace_45_pkg_parseFile1003[] = {1};
meth_meth_grace_45_pkg_parseFile1003->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_parseFile1003);
  meth_meth_grace_45_pkg_parseFile1003->definitionModule = modulename;
  meth_meth_grace_45_pkg_parseFile1003->definitionLine = 339;
// compilenode returning 
// Begin line 356
  setline(356);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1063 = (struct UserObject*)self;
  uo1063->data[19] = emptyclosure;
  Method *meth_meth_grace_45_pkg_processLine1063 = addmethod2pos(self, "processLine", &meth_grace_45_pkg_processLine1063, 19);
int argcv_meth_grace_45_pkg_processLine1063[] = {1};
meth_meth_grace_45_pkg_processLine1063->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_processLine1063);
  meth_meth_grace_45_pkg_processLine1063->definitionModule = modulename;
  meth_meth_grace_45_pkg_processLine1063->definitionLine = 356;
// compilenode returning 
// Begin line 375
  setline(375);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1144 = createclosure(2, "parseImport");
setclosureframe(closure1144, stackframe);
  addtoclosure(closure1144, var_imported);
  addtoclosure(closure1144, var_toProcess);
  struct UserObject *uo1144 = (struct UserObject*)self;
  uo1144->data[20] = (Object)closure1144;
  Method *meth_meth_grace_45_pkg_parseImport1144 = addmethod2pos(self, "parseImport", &meth_grace_45_pkg_parseImport1144, 20);
int argcv_meth_grace_45_pkg_parseImport1144[] = {1};
meth_meth_grace_45_pkg_parseImport1144->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_parseImport1144);
  meth_meth_grace_45_pkg_parseImport1144->definitionModule = modulename;
  meth_meth_grace_45_pkg_parseImport1144->definitionLine = 375;
// compilenode returning 
// Begin line 392
  setline(392);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1215 = (struct UserObject*)self;
  uo1215->data[21] = emptyclosure;
  Method *meth_meth_grace_45_pkg_skipWhiteSpace1215 = addmethod2pos(self, "skipWhiteSpace", &meth_grace_45_pkg_skipWhiteSpace1215, 21);
int argcv_meth_grace_45_pkg_skipWhiteSpace1215[] = {2};
meth_meth_grace_45_pkg_skipWhiteSpace1215->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_skipWhiteSpace1215);
  meth_meth_grace_45_pkg_skipWhiteSpace1215->definitionModule = modulename;
  meth_meth_grace_45_pkg_skipWhiteSpace1215->definitionLine = 392;
// compilenode returning 
// Begin line 400
  setline(400);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1242 = (struct UserObject*)self;
  uo1242->data[22] = emptyclosure;
  Method *meth_meth_grace_45_pkg_displayHelp1242 = addmethod2pos(self, "displayHelp", &meth_grace_45_pkg_displayHelp1242, 22);
int argcv_meth_grace_45_pkg_displayHelp1242[] = {0};
meth_meth_grace_45_pkg_displayHelp1242->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_displayHelp1242);
  meth_meth_grace_45_pkg_displayHelp1242->definitionModule = modulename;
  meth_meth_grace_45_pkg_displayHelp1242->definitionLine = 400;
// compilenode returning 
// Begin line 406
  setline(406);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1249 = createclosure(1, "printMessage");
setclosureframe(closure1249, stackframe);
  addtoclosure(closure1249, var_verbose);
  struct UserObject *uo1249 = (struct UserObject*)self;
  uo1249->data[23] = (Object)closure1249;
  Method *meth_meth_grace_45_pkg_printMessage1249 = addmethod2pos(self, "printMessage", &meth_grace_45_pkg_printMessage1249, 23);
int argcv_meth_grace_45_pkg_printMessage1249[] = {1};
meth_meth_grace_45_pkg_printMessage1249->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_printMessage1249);
  meth_meth_grace_45_pkg_printMessage1249->definitionModule = modulename;
  meth_meth_grace_45_pkg_printMessage1249->definitionLine = 406;
// compilenode returning 
// Begin line 415
  setline(415);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1252 = createclosure(3, "bundle");
setclosureframe(closure1252, stackframe);
  addtoclosure(closure1252, var_imported);
  addtoclosure(closure1252, var_bundlePath);
  addtoclosure(closure1252, var_io);
  struct UserObject *uo1252 = (struct UserObject*)self;
  uo1252->data[24] = (Object)closure1252;
  Method *meth_meth_grace_45_pkg_bundle1252 = addmethod2pos(self, "bundle", &meth_grace_45_pkg_bundle1252, 24);
int argcv_meth_grace_45_pkg_bundle1252[] = {2};
meth_meth_grace_45_pkg_bundle1252->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_bundle1252);
  meth_meth_grace_45_pkg_bundle1252->definitionModule = modulename;
  meth_meth_grace_45_pkg_bundle1252->definitionLine = 415;
// compilenode returning 
// Begin line 433
  setline(433);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1355 = createclosure(1, "setImportDest");
setclosureframe(closure1355, stackframe);
  addtoclosure(closure1355, var_io);
  struct UserObject *uo1355 = (struct UserObject*)self;
  uo1355->data[25] = (Object)closure1355;
  Method *meth_meth_grace_45_pkg_setImportDest1355 = addmethod2pos(self, "setImportDest", &meth_grace_45_pkg_setImportDest1355, 25);
int argcv_meth_grace_45_pkg_setImportDest1355[] = {2};
meth_meth_grace_45_pkg_setImportDest1355->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_setImportDest1355);
  meth_meth_grace_45_pkg_setImportDest1355->definitionModule = modulename;
  meth_meth_grace_45_pkg_setImportDest1355->definitionLine = 433;
// compilenode returning 
// Begin line 458
  setline(458);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1433 = (struct UserObject*)self;
  uo1433->data[26] = emptyclosure;
  Method *meth_meth_grace_45_pkg_parseAndPrefix1433 = addmethod2pos(self, "parseAndPrefix", &meth_grace_45_pkg_parseAndPrefix1433, 26);
int argcv_meth_grace_45_pkg_parseAndPrefix1433[] = {3};
meth_meth_grace_45_pkg_parseAndPrefix1433->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_parseAndPrefix1433);
meth_meth_grace_45_pkg_parseAndPrefix1433->type->types[0] = type_String;
meth_meth_grace_45_pkg_parseAndPrefix1433->type->names[0] = "readFile";
meth_meth_grace_45_pkg_parseAndPrefix1433->type->types[1] = type_String;
meth_meth_grace_45_pkg_parseAndPrefix1433->type->names[1] = "address";
meth_meth_grace_45_pkg_parseAndPrefix1433->type->types[2] = type_String;
meth_meth_grace_45_pkg_parseAndPrefix1433->type->names[2] = "prefix";
  meth_meth_grace_45_pkg_parseAndPrefix1433->definitionModule = modulename;
  meth_meth_grace_45_pkg_parseAndPrefix1433->definitionLine = 458;
// compilenode returning 
// Begin line 511
  setline(511);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1643 = (struct UserObject*)self;
  uo1643->data[27] = emptyclosure;
  Method *meth_meth_grace_45_pkg_removeExistingUrls1643 = addmethod2pos(self, "removeExistingUrls", &meth_grace_45_pkg_removeExistingUrls1643, 27);
int argcv_meth_grace_45_pkg_removeExistingUrls1643[] = {1};
meth_meth_grace_45_pkg_removeExistingUrls1643->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_removeExistingUrls1643);
meth_meth_grace_45_pkg_removeExistingUrls1643->type->types[0] = type_String;
meth_meth_grace_45_pkg_removeExistingUrls1643->type->names[0] = "importStatement";
  meth_meth_grace_45_pkg_removeExistingUrls1643->definitionModule = modulename;
  meth_meth_grace_45_pkg_removeExistingUrls1643->definitionLine = 511;
// compilenode returning 
// Begin line 534
  setline(534);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1695 = (struct UserObject*)self;
  uo1695->data[28] = emptyclosure;
  Method *meth_meth_grace_45_pkg_getBaseUrl1695 = addmethod2pos(self, "getBaseUrl", &meth_grace_45_pkg_getBaseUrl1695, 28);
int argcv_meth_grace_45_pkg_getBaseUrl1695[] = {1};
meth_meth_grace_45_pkg_getBaseUrl1695->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_getBaseUrl1695);
meth_meth_grace_45_pkg_getBaseUrl1695->type->types[0] = type_String;
meth_meth_grace_45_pkg_getBaseUrl1695->type->names[0] = "importStatement";
  meth_meth_grace_45_pkg_getBaseUrl1695->definitionModule = modulename;
  meth_meth_grace_45_pkg_getBaseUrl1695->definitionLine = 534;
// compilenode returning 
// Begin line 557
  setline(557);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1742 = (struct UserObject*)self;
  uo1742->data[29] = emptyclosure;
  Method *meth_meth_grace_45_pkg_removeContainingDir1742 = addmethod2pos(self, "removeContainingDir", &meth_grace_45_pkg_removeContainingDir1742, 29);
int argcv_meth_grace_45_pkg_removeContainingDir1742[] = {1};
meth_meth_grace_45_pkg_removeContainingDir1742->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_removeContainingDir1742);
meth_meth_grace_45_pkg_removeContainingDir1742->type->types[0] = type_String;
meth_meth_grace_45_pkg_removeContainingDir1742->type->names[0] = "st";
  meth_meth_grace_45_pkg_removeContainingDir1742->definitionModule = modulename;
  meth_meth_grace_45_pkg_removeContainingDir1742->definitionLine = 557;
// compilenode returning 
// Begin line 572
  setline(572);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1787 = (struct UserObject*)self;
  uo1787->data[30] = emptyclosure;
  Method *meth_meth_grace_45_pkg_getContainingDirectory1787 = addmethod2pos(self, "getContainingDirectory", &meth_grace_45_pkg_getContainingDirectory1787, 30);
int argcv_meth_grace_45_pkg_getContainingDirectory1787[] = {1};
meth_meth_grace_45_pkg_getContainingDirectory1787->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_getContainingDirectory1787);
meth_meth_grace_45_pkg_getContainingDirectory1787->type->types[0] = type_String;
meth_meth_grace_45_pkg_getContainingDirectory1787->type->names[0] = "st";
  meth_meth_grace_45_pkg_getContainingDirectory1787->definitionModule = modulename;
  meth_meth_grace_45_pkg_getContainingDirectory1787->definitionLine = 572;
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
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1837 = createclosure(1, "imported");
setclosureframe(closure1837, stackframe);
  addtoclosure(closure1837, var_imported);
  struct UserObject *uo1837 = (struct UserObject*)self;
  uo1837->data[31] = (Object)closure1837;
  Method *meth_meth_grace_45_pkg_imported1837 = addmethod2pos(self, "imported", &meth_grace_45_pkg_imported1837, 31);
int argcv_meth_grace_45_pkg_imported1837[] = {0};
meth_meth_grace_45_pkg_imported1837->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_imported1837);
  meth_meth_grace_45_pkg_imported1837->definitionModule = modulename;
  meth_meth_grace_45_pkg_imported1837->definitionLine = 575;
// compilenode returning 
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1838 = createclosure(1, "imported:=");
setclosureframe(closure1838, stackframe);
  addtoclosure(closure1838, var_imported);
  struct UserObject *uo1838 = (struct UserObject*)self;
  uo1838->data[32] = (Object)closure1838;
  Method *meth_meth_grace_45_pkg_imported_58__61_1838 = addmethod2pos(self, "imported:=", &meth_grace_45_pkg_imported_58__61_1838, 32);
int argcv_meth_grace_45_pkg_imported_58__61_1838[] = {1};
meth_meth_grace_45_pkg_imported_58__61_1838->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_imported_58__61_1838);
  meth_meth_grace_45_pkg_imported_58__61_1838->definitionModule = modulename;
  meth_meth_grace_45_pkg_imported_58__61_1838->definitionLine = 575;
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
  Object array1840 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array1840
  *var_toProcess = array1840;
  if (array1840 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1841 = createclosure(1, "toProcess");
setclosureframe(closure1841, stackframe);
  addtoclosure(closure1841, var_toProcess);
  struct UserObject *uo1841 = (struct UserObject*)self;
  uo1841->data[33] = (Object)closure1841;
  Method *meth_meth_grace_45_pkg_toProcess1841 = addmethod2pos(self, "toProcess", &meth_grace_45_pkg_toProcess1841, 33);
int argcv_meth_grace_45_pkg_toProcess1841[] = {0};
meth_meth_grace_45_pkg_toProcess1841->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_toProcess1841);
  meth_meth_grace_45_pkg_toProcess1841->definitionModule = modulename;
  meth_meth_grace_45_pkg_toProcess1841->definitionLine = 575;
// compilenode returning 
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1842 = createclosure(1, "toProcess:=");
setclosureframe(closure1842, stackframe);
  addtoclosure(closure1842, var_toProcess);
  struct UserObject *uo1842 = (struct UserObject*)self;
  uo1842->data[34] = (Object)closure1842;
  Method *meth_meth_grace_45_pkg_toProcess_58__61_1842 = addmethod2pos(self, "toProcess:=", &meth_grace_45_pkg_toProcess_58__61_1842, 34);
int argcv_meth_grace_45_pkg_toProcess_58__61_1842[] = {1};
meth_meth_grace_45_pkg_toProcess_58__61_1842->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_toProcess_58__61_1842);
  meth_meth_grace_45_pkg_toProcess_58__61_1842->definitionModule = modulename;
  meth_meth_grace_45_pkg_toProcess_58__61_1842->definitionLine = 575;
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
  Object bool1844 = alloc_Boolean(0);
// compilenode returning bool1844
  *var_verbose = bool1844;
  if (bool1844 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1845 = createclosure(1, "verbose");
setclosureframe(closure1845, stackframe);
  addtoclosure(closure1845, var_verbose);
  struct UserObject *uo1845 = (struct UserObject*)self;
  uo1845->data[35] = (Object)closure1845;
  Method *meth_meth_grace_45_pkg_verbose1845 = addmethod2pos(self, "verbose", &meth_grace_45_pkg_verbose1845, 35);
int argcv_meth_grace_45_pkg_verbose1845[] = {0};
meth_meth_grace_45_pkg_verbose1845->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_verbose1845);
  meth_meth_grace_45_pkg_verbose1845->definitionModule = modulename;
  meth_meth_grace_45_pkg_verbose1845->definitionLine = 575;
// compilenode returning 
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1846 = createclosure(1, "verbose:=");
setclosureframe(closure1846, stackframe);
  addtoclosure(closure1846, var_verbose);
  struct UserObject *uo1846 = (struct UserObject*)self;
  uo1846->data[36] = (Object)closure1846;
  Method *meth_meth_grace_45_pkg_verbose_58__61_1846 = addmethod2pos(self, "verbose:=", &meth_grace_45_pkg_verbose_58__61_1846, 36);
int argcv_meth_grace_45_pkg_verbose_58__61_1846[] = {1};
meth_meth_grace_45_pkg_verbose_58__61_1846->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_verbose_58__61_1846);
  meth_meth_grace_45_pkg_verbose_58__61_1846->definitionModule = modulename;
  meth_meth_grace_45_pkg_verbose_58__61_1846->definitionLine = 575;
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
  Object bool1848 = alloc_Boolean(0);
// compilenode returning bool1848
  *var_global = bool1848;
  if (bool1848 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1849 = createclosure(1, "global");
setclosureframe(closure1849, stackframe);
  addtoclosure(closure1849, var_global);
  struct UserObject *uo1849 = (struct UserObject*)self;
  uo1849->data[37] = (Object)closure1849;
  Method *meth_meth_grace_45_pkg_global1849 = addmethod2pos(self, "global", &meth_grace_45_pkg_global1849, 37);
int argcv_meth_grace_45_pkg_global1849[] = {0};
meth_meth_grace_45_pkg_global1849->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_global1849);
  meth_meth_grace_45_pkg_global1849->definitionModule = modulename;
  meth_meth_grace_45_pkg_global1849->definitionLine = 575;
// compilenode returning 
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1850 = createclosure(1, "global:=");
setclosureframe(closure1850, stackframe);
  addtoclosure(closure1850, var_global);
  struct UserObject *uo1850 = (struct UserObject*)self;
  uo1850->data[38] = (Object)closure1850;
  Method *meth_meth_grace_45_pkg_global_58__61_1850 = addmethod2pos(self, "global:=", &meth_grace_45_pkg_global_58__61_1850, 38);
int argcv_meth_grace_45_pkg_global_58__61_1850[] = {1};
meth_meth_grace_45_pkg_global_58__61_1850->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_global_58__61_1850);
  meth_meth_grace_45_pkg_global_58__61_1850->definitionModule = modulename;
  meth_meth_grace_45_pkg_global_58__61_1850->definitionLine = 575;
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
  if (strlit1852 == NULL) {
    strlit1852 = alloc_String("");
    gc_root(strlit1852);
  }
// compilenode returning strlit1852
  *var_bundlePath = strlit1852;
  if (strlit1852 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1853 = createclosure(1, "bundlePath");
setclosureframe(closure1853, stackframe);
  addtoclosure(closure1853, var_bundlePath);
  struct UserObject *uo1853 = (struct UserObject*)self;
  uo1853->data[39] = (Object)closure1853;
  Method *meth_meth_grace_45_pkg_bundlePath1853 = addmethod2pos(self, "bundlePath", &meth_grace_45_pkg_bundlePath1853, 39);
int argcv_meth_grace_45_pkg_bundlePath1853[] = {0};
meth_meth_grace_45_pkg_bundlePath1853->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_bundlePath1853);
  meth_meth_grace_45_pkg_bundlePath1853->definitionModule = modulename;
  meth_meth_grace_45_pkg_bundlePath1853->definitionLine = 575;
// compilenode returning 
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1854 = createclosure(1, "bundlePath:=");
setclosureframe(closure1854, stackframe);
  addtoclosure(closure1854, var_bundlePath);
  struct UserObject *uo1854 = (struct UserObject*)self;
  uo1854->data[40] = (Object)closure1854;
  Method *meth_meth_grace_45_pkg_bundlePath_58__61_1854 = addmethod2pos(self, "bundlePath:=", &meth_grace_45_pkg_bundlePath_58__61_1854, 40);
int argcv_meth_grace_45_pkg_bundlePath_58__61_1854[] = {1};
meth_meth_grace_45_pkg_bundlePath_58__61_1854->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_bundlePath_58__61_1854);
  meth_meth_grace_45_pkg_bundlePath_58__61_1854->definitionModule = modulename;
  meth_meth_grace_45_pkg_bundlePath_58__61_1854->definitionLine = 575;
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
  if (strlit1856 == NULL) {
    strlit1856 = alloc_String("");
    gc_root(strlit1856);
  }
// compilenode returning strlit1856
  *var_baseUrl = strlit1856;
  if (strlit1856 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1857 = createclosure(1, "baseUrl");
setclosureframe(closure1857, stackframe);
  addtoclosure(closure1857, var_baseUrl);
  struct UserObject *uo1857 = (struct UserObject*)self;
  uo1857->data[41] = (Object)closure1857;
  Method *meth_meth_grace_45_pkg_baseUrl1857 = addmethod2pos(self, "baseUrl", &meth_grace_45_pkg_baseUrl1857, 41);
int argcv_meth_grace_45_pkg_baseUrl1857[] = {0};
meth_meth_grace_45_pkg_baseUrl1857->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_baseUrl1857);
  meth_meth_grace_45_pkg_baseUrl1857->definitionModule = modulename;
  meth_meth_grace_45_pkg_baseUrl1857->definitionLine = 575;
// compilenode returning 
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1858 = createclosure(1, "baseUrl:=");
setclosureframe(closure1858, stackframe);
  addtoclosure(closure1858, var_baseUrl);
  struct UserObject *uo1858 = (struct UserObject*)self;
  uo1858->data[42] = (Object)closure1858;
  Method *meth_meth_grace_45_pkg_baseUrl_58__61_1858 = addmethod2pos(self, "baseUrl:=", &meth_grace_45_pkg_baseUrl_58__61_1858, 42);
int argcv_meth_grace_45_pkg_baseUrl_58__61_1858[] = {1};
meth_meth_grace_45_pkg_baseUrl_58__61_1858->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_baseUrl_58__61_1858);
  meth_meth_grace_45_pkg_baseUrl_58__61_1858->definitionModule = modulename;
  meth_meth_grace_45_pkg_baseUrl_58__61_1858->definitionLine = 575;
// compilenode returning 
// compilenode returning done
// Begin line 13
  setline(13);
  setmodule(modulename);
  setsource(originalSourceLines);
  *var_curFile = undefined;
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1860 = createclosure(1, "curFile");
setclosureframe(closure1860, stackframe);
  addtoclosure(closure1860, var_curFile);
  struct UserObject *uo1860 = (struct UserObject*)self;
  uo1860->data[43] = (Object)closure1860;
  Method *meth_meth_grace_45_pkg_curFile1860 = addmethod2pos(self, "curFile", &meth_grace_45_pkg_curFile1860, 43);
int argcv_meth_grace_45_pkg_curFile1860[] = {0};
meth_meth_grace_45_pkg_curFile1860->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_curFile1860);
  meth_meth_grace_45_pkg_curFile1860->definitionModule = modulename;
  meth_meth_grace_45_pkg_curFile1860->definitionLine = 575;
// compilenode returning 
// Begin line 575
  setline(575);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1861 = createclosure(1, "curFile:=");
setclosureframe(closure1861, stackframe);
  addtoclosure(closure1861, var_curFile);
  struct UserObject *uo1861 = (struct UserObject*)self;
  uo1861->data[44] = (Object)closure1861;
  Method *meth_meth_grace_45_pkg_curFile_58__61_1861 = addmethod2pos(self, "curFile:=", &meth_grace_45_pkg_curFile_58__61_1861, 44);
int argcv_meth_grace_45_pkg_curFile_58__61_1861[] = {1};
meth_meth_grace_45_pkg_curFile_58__61_1861->type = alloc_MethodType(1, argcv_meth_grace_45_pkg_curFile_58__61_1861);
  meth_meth_grace_45_pkg_curFile_58__61_1861->definitionModule = modulename;
  meth_meth_grace_45_pkg_curFile_58__61_1861->definitionLine = 575;
// compilenode returning 
// compilenode returning done
// Begin line 13
  setline(13);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 15
  setline(15);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1863 = gc_frame_new();
// Begin line 13
  setline(13);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1864 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call1865 = callmethod(*var_sys, "argv",
    1, partcv, params);
  gc_frame_end(callframe1864);
// compilenode returning call1865
// compilenode returning call1865
  gc_frame_newslot(call1865);
  params[0] = call1865;
  partcv[0] = 1;
  Object call1866 = callmethodflags(self, "parseArgs", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1863);
// compilenode returning call1866
// Begin line 15
  setline(15);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// compilenode returning 
// Begin line 41
  setline(41);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 96
  setline(96);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 104
  setline(104);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 112
  setline(112);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 124
  setline(124);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 138
  setline(138);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 142
  setline(142);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 154
  setline(154);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 179
  setline(179);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 196
  setline(196);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 227
  setline(227);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 257
  setline(257);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 267
  setline(267);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 282
  setline(282);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 299
  setline(299);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 315
  setline(315);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 339
  setline(339);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 356
  setline(356);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 375
  setline(375);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 392
  setline(392);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 400
  setline(400);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 406
  setline(406);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 415
  setline(415);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 433
  setline(433);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 458
  setline(458);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 511
  setline(511);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 534
  setline(534);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 557
  setline(557);
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
  module_grace_45_pkg_init();
  gracelib_stats();
  return 0;
}
