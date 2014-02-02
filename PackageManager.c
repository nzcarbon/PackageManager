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
  "var toProcess",
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
  "",
  "        on.flag \"list\" do { ",
  "            listInstalled();",
  "        }",
  "",
  "        on.doubleValue \"bundle\" do { toBundle, name ->",
  "            bundle(toBundle,name);",
  "        }",
  "",
  "        on.doubleValue \"setAddress\" do { address, prefix ->",
  "            setImportDest(address,prefix);",
  "        }",
  "",
  "        on.flag \"verbose\" do { ",
  "            verbose := true;",
  "        }",
  "",
  "        on.flag \"global\" do {",
  "            global := true;",
  "        }",
  "    }",
  "",
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
  "            print(\"In flag 2: Sh = {sh}\");",
  "            if((arg == \"--{name}\") || (arg == \"-{sh}\")) then {",
  "                block'.apply",
  "                ran := true",
  "            }",
  "        }",
  "",
  "        method flag(name : String) do(block') {",
  "            print(\"in flag 1\");",
  "            flag(name) shortHand(\"\") do(block')",
  "        }",
  "    }",
  "    print(\"In forargs\");",
  "    while { i <= size } do {",
  "        def arg = args.at(i)",
  "        print(arg);",
  "        ran := false",
  "        block.apply(arg, on)",
  "",
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
  "            imported.push(curFile)",
  "            toProcess := []",
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
  "    if (bundlePath != \"\")then{",
  "        return io.open(\"{bundlePath}/{fileName}\",\"r\").read;",
  "",
  "",
  "    }",
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
Object reader_PackageManager_outer_410(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject*)self;
  return uo->data[0];
}
Object reader_PackageManager_address_411(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  return uo->data[1];
}
Object writer_PackageManager_address_411(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  uo->data[1] = args[0];
  return done;
}
Object reader_PackageManager_data_412(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  return uo->data[2];
}
Object writer_PackageManager_data_412(Object self, int nparams, int *argcv, Object* args, int flags) {
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
static Object strlit192;
static Object strlit195;
static Object strlit204;
static Object strlit207;
static Object strlit215;
static Object strlit218;
static Object strlit230;
static Object strlit233;
static Object strlit235;
static Object strlit259;
static Object strlit269;
static Object strlit272;
static Object strlit287;
static Object strlit291;
static Object strlit293;
static Object strlit295;
static Object strlit297;
static Object strlit304;
static Object strlit306;
static Object strlit313;
static Object strlit315;
static Object strlit319;
static Object strlit330;
static Object strlit334;
static Object strlit349;
static Object strlit362;
static Object strlit368;
static Object strlit406;
static ClassData objclass409;
static Object strlit422;
static Object strlit427;
static Object strlit438;
static Object strlit473;
static Object strlit480;
static Object strlit492;
static Object strlit527;
static Object strlit533;
static Object strlit538;
static Object strlit542;
static Object strlit561;
static Object strlit566;
static Object strlit573;
static Object strlit587;
static Object strlit607;
static Object strlit614;
static Object strlit621;
static Object strlit624;
static Object strlit629;
static Object strlit636;
static Object strlit639;
static Object strlit650;
static Object strlit653;
static Object strlit658;
static Object strlit666;
static Object strlit671;
static Object strlit682;
static Object strlit687;
static Object strlit692;
static Object strlit700;
static Object strlit705;
static Object strlit716;
static Object strlit721;
static Object strlit726;
static Object strlit731;
static Object strlit740;
static Object strlit743;
static Object strlit748;
static Object strlit751;
static Object strlit771;
static Object strlit774;
static Object strlit779;
static Object strlit781;
static Object strlit786;
static Object strlit788;
static Object strlit807;
static Object strlit818;
static Object strlit820;
static Object strlit834;
static Object strlit840;
static Object strlit844;
static Object strlit854;
static Object strlit856;
static Object strlit861;
static Object strlit863;
static Object strlit869;
static Object strlit872;
static Object strlit882;
static Object strlit895;
static Object strlit896;
static Object strlit900;
static Object strlit903;
static Object strlit926;
static Object strlit933;
static Object strlit943;
static Object strlit954;
static Object strlit960;
static Object strlit963;
static Object strlit998;
static Object strlit1043;
static Object strlit1060;
static Object strlit1079;
static Object strlit1105;
static Object strlit1114;
static Object strlit1141;
static Object strlit1150;
static Object strlit1177;
static Object strlit1195;
static Object strlit1211;
static Object strlit1213;
static Object strlit1215;
static Object strlit1229;
static Object strlit1232;
static Object strlit1241;
static Object strlit1244;
static Object strlit1247;
static Object strlit1256;
static Object strlit1259;
static Object strlit1264;
static Object strlit1267;
static Object strlit1274;
static Object strlit1299;
static Object strlit1302;
static Object strlit1311;
static Object strlit1314;
static Object strlit1348;
static Object strlit1354;
static Object strlit1359;
static Object strlit1371;
static Object strlit1374;
static Object strlit1383;
static Object strlit1388;
static Object strlit1434;
static Object strlit1455;
static Object strlit1488;
static Object strlit1494;
static Object strlit1504;
static Object strlit1521;
static Object strlit1526;
static Object strlit1562;
static Object strlit1565;
static Object strlit1596;
static Object strlit1614;
static Object strlit1633;
static Object strlit1671;
static Object strlit1716;
static Object strlit1737;
Object module_sys;
Object module_sys_init();
Object module_io;
Object module_io_init();
Object module_curl;
Object module_curl_init();
static Object strlit1761;
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
// Begin line 24
  setline(24);
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
// Begin line 28
  setline(28);
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
// Begin line 32
  setline(32);
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
// Begin line 36
  setline(36);
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
// Begin line 40
  setline(40);
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
// Begin line 23
  setline(23);
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
// Begin line 23
  setline(23);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block12 = alloc_Block(NULL, NULL, "PackageManager", 23);
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
// Begin line 27
  setline(27);
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
  meth_meth_PackageManager__apply21->definitionLine = 16;
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
// Begin line 31
  setline(31);
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
  meth_meth_PackageManager__apply29->definitionLine = 16;
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
    strlit35 = alloc_String("setAddress");
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
  meth_meth_PackageManager__apply37->definitionLine = 16;
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
    strlit43 = alloc_String("verbose");
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
// Begin line 35
  setline(35);
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
    strlit51 = alloc_String("global");
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
// Begin line 44
  setline(44);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe5 = gc_frame_new();
// Begin line 17
  setline(17);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_args
  gc_frame_newslot(*var_args);
// Begin line 44
  setline(44);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block6 = alloc_Block(NULL, NULL, "PackageManager", 44);
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
// Begin line 54
  setline(54);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe68 = gc_frame_new();
// Begin line 53
  setline(53);
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
// Begin line 63
  setline(63);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 54
  setline(54);
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
// Begin line 59
  setline(59);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 55
  setline(55);
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
// Begin line 57
  setline(57);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe102 = gc_frame_new();
// Begin line 56
  setline(56);
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
// Begin line 60
  setline(60);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 59
  setline(59);
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
// Begin line 61
  setline(61);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe119 = gc_frame_new();
// Begin line 60
  setline(60);
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
// Begin line 62
  setline(62);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 61
  setline(61);
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
// Begin line 67
  setline(67);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe126 = gc_frame_new();
// Begin line 66
  setline(66);
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
// Begin line 78
  setline(78);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 67
  setline(67);
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
// Begin line 71
  setline(71);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 68
  setline(68);
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
// Begin line 70
  setline(70);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe150 = gc_frame_new();
// Begin line 69
  setline(69);
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
// Begin line 72
  setline(72);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe162 = gc_frame_new();
// Begin line 71
  setline(71);
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
// Begin line 73
  setline(73);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe168 = gc_frame_new();
// Begin line 72
  setline(72);
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
// Begin line 74
  setline(74);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe174 = gc_frame_new();
// Begin line 73
  setline(73);
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
// Begin line 75
  setline(75);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 74
  setline(74);
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
// Begin line 76
  setline(76);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 75
  setline(75);
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
// Begin line 82
  setline(82);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe184 = gc_frame_new();
// Begin line 81
  setline(81);
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
// Begin line 86
  setline(86);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe188 = gc_frame_new();
// Begin line 85
  setline(85);
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
// Begin line 86
  setline(86);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit192 == NULL) {
    strlit192 = alloc_String("In flag 2: Sh = ");
    gc_root(strlit192);
  }
// compilenode returning strlit192
  int op_slot_left_191 = gc_frame_newslot(strlit192);
// compilenode returning *var_sh
  int op_slot_right_191 = gc_frame_newslot(*var_sh);
  params[0] = *var_sh;
  partcv[0] = 1;
  Object opresult194 = callmethod(strlit192, "++", 1, partcv, params);
// compilenode returning opresult194
  int op_slot_left_190 = gc_frame_newslot(opresult194);
  if (strlit195 == NULL) {
    strlit195 = alloc_String("");
    gc_root(strlit195);
  }
// compilenode returning strlit195
  int op_slot_right_190 = gc_frame_newslot(strlit195);
  params[0] = strlit195;
  partcv[0] = 1;
  Object opresult197 = callmethod(opresult194, "++", 1, partcv, params);
// compilenode returning opresult197
  params[0] = opresult197;
  Object call198 = gracelib_print(NULL, 1,  params);
// compilenode returning call198
// Begin line 91
  setline(91);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 87
  setline(87);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arg
  int op_slot_left_201 = gc_frame_newslot(*var_arg);
  if (strlit204 == NULL) {
    strlit204 = alloc_String("--");
    gc_root(strlit204);
  }
// compilenode returning strlit204
  int op_slot_left_203 = gc_frame_newslot(strlit204);
// compilenode returning *var_name
  int op_slot_right_203 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
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
  int op_slot_right_201 = gc_frame_newslot(opresult209);
  params[0] = opresult209;
  partcv[0] = 1;
  Object opresult211 = callmethod(*var_arg, "==", 1, partcv, params);
// compilenode returning opresult211
  int op_slot_left_200 = gc_frame_newslot(opresult211);
// compilenode returning *var_arg
  int op_slot_left_212 = gc_frame_newslot(*var_arg);
  if (strlit215 == NULL) {
    strlit215 = alloc_String("-");
    gc_root(strlit215);
  }
// compilenode returning strlit215
  int op_slot_left_214 = gc_frame_newslot(strlit215);
// compilenode returning *var_sh
  int op_slot_right_214 = gc_frame_newslot(*var_sh);
  params[0] = *var_sh;
  partcv[0] = 1;
  Object opresult217 = callmethod(strlit215, "++", 1, partcv, params);
// compilenode returning opresult217
  int op_slot_left_213 = gc_frame_newslot(opresult217);
  if (strlit218 == NULL) {
    strlit218 = alloc_String("");
    gc_root(strlit218);
  }
// compilenode returning strlit218
  int op_slot_right_213 = gc_frame_newslot(strlit218);
  params[0] = strlit218;
  partcv[0] = 1;
  Object opresult220 = callmethod(opresult217, "++", 1, partcv, params);
// compilenode returning opresult220
  int op_slot_right_212 = gc_frame_newslot(opresult220);
  params[0] = opresult220;
  partcv[0] = 1;
  Object opresult222 = callmethod(*var_arg, "==", 1, partcv, params);
// compilenode returning opresult222
  int op_slot_right_200 = gc_frame_newslot(opresult222);
  params[0] = opresult222;
  partcv[0] = 1;
  Object opresult224 = callmethod(opresult211, "||", 1, partcv, params);
// compilenode returning opresult224
  Object if199 = done;
  if (istrue(opresult224)) {
// Begin line 88
  setline(88);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe225 = gc_frame_new();
// compilenode returning *var_block_39_
  partcv[0] = 0;
  Object call226 = callmethod(*var_block_39_, "apply",
    1, partcv, params);
  gc_frame_end(callframe225);
// compilenode returning call226
// compilenode returning call226
// Begin line 90
  setline(90);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 89
  setline(89);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool227 = alloc_Boolean(1);
// compilenode returning bool227
  *var_ran = bool227;
  if (bool227 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if199 = done;
  } else {
  }
// compilenode returning if199
  gc_frame_end(frame);
  return if199;
}
Object meth_PackageManager_flag_40__41_do229(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 94
  setline(94);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit230 == NULL) {
    strlit230 = alloc_String("in flag 1");
    gc_root(strlit230);
  }
// compilenode returning strlit230
  params[0] = strlit230;
  Object call231 = gracelib_print(NULL, 1,  params);
// compilenode returning call231
// Begin line 96
  setline(96);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe232 = gc_frame_new();
// Begin line 95
  setline(95);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_name
  gc_frame_newslot(*var_name);
  if (strlit233 == NULL) {
    strlit233 = alloc_String("");
    gc_root(strlit233);
  }
// compilenode returning strlit233
  gc_frame_newslot(strlit233);
// compilenode returning *var_block_39_
  gc_frame_newslot(*var_block_39_);
  params[0] = *var_name;
  params[1] = strlit233;
  params[2] = *var_block_39_;
  partcv[0] = 1;
  partcv[1] = 1;
  partcv[2] = 1;
  Object call234 = callmethodflags(self, "flag()shortHand()do", 3, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe232);
// compilenode returning call234
  gc_frame_end(frame);
  return call234;
}
Object meth_PackageManager__apply239(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_240 = gc_frame_newslot(*var_i);
// compilenode returning *var_size
  int op_slot_right_240 = gc_frame_newslot(*var_size);
  params[0] = *var_size;
  partcv[0] = 1;
  Object opresult242 = callmethod(*var_i, "<=", 1, partcv, params);
// compilenode returning opresult242
  gc_frame_end(frame);
  return opresult242;
}
Object meth_PackageManager__apply245(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 101
  setline(101);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe246 = gc_frame_new();
// Begin line 100
  setline(100);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  gc_frame_newslot(*var_i);
// compilenode returning *var_args
  params[0] = *var_i;
  partcv[0] = 1;
  Object call247 = callmethod(*var_args, "at",
    1, partcv, params);
  gc_frame_end(callframe246);
// compilenode returning call247
  *var_arg = call247;
  if (call247 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 101
  setline(101);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arg
  params[0] = *var_arg;
  Object call248 = gracelib_print(NULL, 1,  params);
// compilenode returning call248
// Begin line 103
  setline(103);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 102
  setline(102);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool249 = alloc_Boolean(0);
// compilenode returning bool249
  *var_ran = bool249;
  if (bool249 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 105
  setline(105);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe251 = gc_frame_new();
// Begin line 103
  setline(103);
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
  Object call252 = callmethod(*var_block, "apply",
    1, partcv, params);
  gc_frame_end(callframe251);
// compilenode returning call252
// Begin line 109
  setline(109);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 105
  setline(105);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe256 = gc_frame_new();
  Object num257 = alloc_Float64(1);
// compilenode returning num257
  gc_frame_newslot(num257);
// compilenode returning *var_arg
  params[0] = num257;
  partcv[0] = 1;
  Object call258 = callmethod(*var_arg, "at",
    1, partcv, params);
  gc_frame_end(callframe256);
// compilenode returning call258
  int op_slot_left_255 = gc_frame_newslot(call258);
  if (strlit259 == NULL) {
    strlit259 = alloc_String("-");
    gc_root(strlit259);
  }
// compilenode returning strlit259
  int op_slot_right_255 = gc_frame_newslot(strlit259);
  params[0] = strlit259;
  partcv[0] = 1;
  Object opresult261 = callmethod(call258, "==", 1, partcv, params);
// compilenode returning opresult261
  int op_slot_left_254 = gc_frame_newslot(opresult261);
  int callframe262 = gc_frame_new();
// compilenode returning *var_ran
  partcv[0] = 0;
  Object call263 = callmethod(*var_ran, "not",
    1, partcv, params);
  gc_frame_end(callframe262);
// compilenode returning call263
// compilenode returning call263
  int op_slot_right_254 = gc_frame_newslot(call263);
  params[0] = call263;
  partcv[0] = 1;
  Object opresult265 = callmethod(opresult261, "&&", 1, partcv, params);
// compilenode returning opresult265
  Object if253 = done;
  if (istrue(opresult265)) {
// Begin line 107
  setline(107);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe266 = gc_frame_new();
// Begin line 106
  setline(106);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit269 == NULL) {
    strlit269 = alloc_String("Unrecognised argument ");
    gc_root(strlit269);
  }
// compilenode returning strlit269
  int op_slot_left_268 = gc_frame_newslot(strlit269);
// compilenode returning *var_arg
  int op_slot_right_268 = gc_frame_newslot(*var_arg);
  params[0] = *var_arg;
  partcv[0] = 1;
  Object opresult271 = callmethod(strlit269, "++", 1, partcv, params);
// compilenode returning opresult271
  int op_slot_left_267 = gc_frame_newslot(opresult271);
  if (strlit272 == NULL) {
    strlit272 = alloc_String("");
    gc_root(strlit272);
  }
// compilenode returning strlit272
  int op_slot_right_267 = gc_frame_newslot(strlit272);
  params[0] = strlit272;
  partcv[0] = 1;
  Object opresult274 = callmethod(opresult271, "++", 1, partcv, params);
// compilenode returning opresult274
  gc_frame_newslot(opresult274);
  int callframe275 = gc_frame_new();
  partcv[0] = 0;
  Object call276 = callmethodflags(prelude, "Exception", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe275);
// compilenode returning call276
  params[0] = opresult274;
  partcv[0] = 1;
  Object call277 = callmethod(call276, "raise",
    1, partcv, params);
  gc_frame_end(callframe266);
// compilenode returning call277
    gc_frame_newslot(call277);
    if253 = call277;
  } else {
  }
// compilenode returning if253
// Begin line 110
  setline(110);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 109
  setline(109);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_i
  int op_slot_left_278 = gc_frame_newslot(*var_i);
  Object num279 = alloc_Float64(1);
// compilenode returning num279
  int op_slot_right_278 = gc_frame_newslot(num279);
  params[0] = num279;
  partcv[0] = 1;
  Object sum281 = callmethod(*var_i, "+", 1, partcv, params);
// compilenode returning sum281
  *var_i = sum281;
  if (sum281 == undefined)
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
// Begin line 48
  setline(48);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 47
  setline(47);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num61 = alloc_Float64(2);
// compilenode returning num61
  *var_i = num61;
  if (num61 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 49
  setline(49);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 48
  setline(48);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool62 = alloc_Boolean(0);
// compilenode returning bool62
  *var_ran = bool62;
  if (bool62 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 51
  setline(51);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 49
  setline(49);
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
// Begin line 51
  setline(51);
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
  meth_meth_PackageManager_option_40__41_shortHand_40__41_do67->definitionLine = 52;
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
  meth_meth_PackageManager_doubleValue_40__41_do125->definitionLine = 65;
  struct UserObject *uo183 = (struct UserObject*)obj65;
  uo183->data[3] = emptyclosure;
  Method *meth_meth_PackageManager_option_40__41_do183 = addmethod2pos(obj65, "option()do", &meth_PackageManager_option_40__41_do183, 3);
int argcv_meth_PackageManager_option_40__41_do183[] = {1, 1};
meth_meth_PackageManager_option_40__41_do183->type = alloc_MethodType(2, argcv_meth_PackageManager_option_40__41_do183);
meth_meth_PackageManager_option_40__41_do183->type->types[0] = type_String;
meth_meth_PackageManager_option_40__41_do183->type->names[0] = "name";
  meth_meth_PackageManager_option_40__41_do183->definitionModule = modulename;
  meth_meth_PackageManager_option_40__41_do183->definitionLine = 80;
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
  meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187->definitionLine = 84;
  struct UserObject *uo229 = (struct UserObject*)obj65;
  uo229->data[5] = emptyclosure;
  Method *meth_meth_PackageManager_flag_40__41_do229 = addmethod2pos(obj65, "flag()do", &meth_PackageManager_flag_40__41_do229, 5);
int argcv_meth_PackageManager_flag_40__41_do229[] = {1, 1};
meth_meth_PackageManager_flag_40__41_do229->type = alloc_MethodType(2, argcv_meth_PackageManager_flag_40__41_do229);
meth_meth_PackageManager_flag_40__41_do229->type->types[0] = type_String;
meth_meth_PackageManager_flag_40__41_do229->type->names[0] = "name";
  meth_meth_PackageManager_flag_40__41_do229->definitionModule = modulename;
  meth_meth_PackageManager_flag_40__41_do229->definitionLine = 93;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
objclass65 = obj65->class;
  objclass65->definitionModule = modulename;
  objclass65->definitionLine = 51;
  (*(struct UserObject *)self).data[0] = lowerouter65;
  self = oldself65;
  selfslot = oldselfslot65;
  stackframe = oldstackframe65;
// compilenode returning obj65
  *var_on = obj65;
  if (obj65 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 98
  setline(98);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit235 == NULL) {
    strlit235 = alloc_String("In forargs");
    gc_root(strlit235);
  }
// compilenode returning strlit235
  params[0] = strlit235;
  Object call236 = gracelib_print(NULL, 1,  params);
// compilenode returning call236
// Begin line 111
  setline(111);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe237 = gc_frame_new();
// Begin line 99
  setline(99);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block238 = alloc_Block(NULL, NULL, "PackageManager", 99);
  gc_frame_newslot(block238);
  block_savedest(block238);
  Object closure239 = createclosure(3, "_apply");
setclosureframe(closure239, stackframe);
  addtoclosure(closure239, var_i);
  addtoclosure(closure239, var_size);
  addtoclosure(closure239, selfslot);
  struct UserObject *uo239 = (struct UserObject*)block238;
  uo239->data[0] = (Object)closure239;
  Method *meth_meth_PackageManager__apply239 = addmethod2pos(block238, "_apply", &meth_PackageManager__apply239, 0);
int argcv_meth_PackageManager__apply239[] = {0};
meth_meth_PackageManager__apply239->type = alloc_MethodType(1, argcv_meth_PackageManager__apply239);
  meth_meth_PackageManager__apply239->definitionModule = modulename;
  meth_meth_PackageManager__apply239->definitionLine = 88;
// compilenode returning block238
  gc_frame_newslot(block238);
// Begin line 111
  setline(111);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block244 = alloc_Block(NULL, NULL, "PackageManager", 111);
  gc_frame_newslot(block244);
  block_savedest(block244);
  Object closure245 = createclosure(6, "_apply");
setclosureframe(closure245, stackframe);
  addtoclosure(closure245, var_i);
  addtoclosure(closure245, var_args);
  addtoclosure(closure245, var_ran);
  addtoclosure(closure245, var_on);
  addtoclosure(closure245, var_block);
  addtoclosure(closure245, selfslot);
  struct UserObject *uo245 = (struct UserObject*)block244;
  uo245->data[0] = (Object)closure245;
  Method *meth_meth_PackageManager__apply245 = addmethod2pos(block244, "_apply", &meth_PackageManager__apply245, 0);
int argcv_meth_PackageManager__apply245[] = {0};
meth_meth_PackageManager__apply245->type = alloc_MethodType(1, argcv_meth_PackageManager__apply245);
  meth_meth_PackageManager__apply245->definitionModule = modulename;
  meth_meth_PackageManager__apply245->definitionLine = 88;
// compilenode returning block244
  gc_frame_newslot(block244);
  params[0] = block238;
  params[1] = block244;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call284 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe237);
// compilenode returning call284
  gc_frame_end(frame);
  return call284;
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
// Begin line 48
  setline(48);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 47
  setline(47);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num61 = alloc_Float64(2);
// compilenode returning num61
  *var_i = num61;
  if (num61 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 49
  setline(49);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 48
  setline(48);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool62 = alloc_Boolean(0);
// compilenode returning bool62
  *var_ran = bool62;
  if (bool62 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 51
  setline(51);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 49
  setline(49);
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
// Begin line 51
  setline(51);
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
  meth_meth_PackageManager_option_40__41_shortHand_40__41_do67->definitionLine = 52;
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
  meth_meth_PackageManager_doubleValue_40__41_do125->definitionLine = 65;
  struct UserObject *uo183 = (struct UserObject*)obj65;
  uo183->data[3] = emptyclosure;
  Method *meth_meth_PackageManager_option_40__41_do183 = addmethod2pos(obj65, "option()do", &meth_PackageManager_option_40__41_do183, 3);
int argcv_meth_PackageManager_option_40__41_do183[] = {1, 1};
meth_meth_PackageManager_option_40__41_do183->type = alloc_MethodType(2, argcv_meth_PackageManager_option_40__41_do183);
meth_meth_PackageManager_option_40__41_do183->type->types[0] = type_String;
meth_meth_PackageManager_option_40__41_do183->type->names[0] = "name";
  meth_meth_PackageManager_option_40__41_do183->definitionModule = modulename;
  meth_meth_PackageManager_option_40__41_do183->definitionLine = 80;
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
  meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187->definitionLine = 84;
  struct UserObject *uo229 = (struct UserObject*)obj65;
  uo229->data[5] = emptyclosure;
  Method *meth_meth_PackageManager_flag_40__41_do229 = addmethod2pos(obj65, "flag()do", &meth_PackageManager_flag_40__41_do229, 5);
int argcv_meth_PackageManager_flag_40__41_do229[] = {1, 1};
meth_meth_PackageManager_flag_40__41_do229->type = alloc_MethodType(2, argcv_meth_PackageManager_flag_40__41_do229);
meth_meth_PackageManager_flag_40__41_do229->type->types[0] = type_String;
meth_meth_PackageManager_flag_40__41_do229->type->names[0] = "name";
  meth_meth_PackageManager_flag_40__41_do229->definitionModule = modulename;
  meth_meth_PackageManager_flag_40__41_do229->definitionLine = 93;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
objclass65 = obj65->class;
  objclass65->definitionModule = modulename;
  objclass65->definitionLine = 51;
  (*(struct UserObject *)self).data[0] = lowerouter65;
  self = oldself65;
  selfslot = oldselfslot65;
  stackframe = oldstackframe65;
// compilenode returning obj65
  *var_on = obj65;
  if (obj65 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 98
  setline(98);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit235 == NULL) {
    strlit235 = alloc_String("In forargs");
    gc_root(strlit235);
  }
// compilenode returning strlit235
  params[0] = strlit235;
  Object call236 = gracelib_print(NULL, 1,  params);
// compilenode returning call236
// Begin line 111
  setline(111);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe237 = gc_frame_new();
// Begin line 99
  setline(99);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block238 = alloc_Block(NULL, NULL, "PackageManager", 99);
  gc_frame_newslot(block238);
  block_savedest(block238);
  Object closure239 = createclosure(3, "_apply");
setclosureframe(closure239, stackframe);
  addtoclosure(closure239, var_i);
  addtoclosure(closure239, var_size);
  addtoclosure(closure239, selfslot);
  struct UserObject *uo239 = (struct UserObject*)block238;
  uo239->data[0] = (Object)closure239;
  Method *meth_meth_PackageManager__apply239 = addmethod2pos(block238, "_apply", &meth_PackageManager__apply239, 0);
int argcv_meth_PackageManager__apply239[] = {0};
meth_meth_PackageManager__apply239->type = alloc_MethodType(1, argcv_meth_PackageManager__apply239);
  meth_meth_PackageManager__apply239->definitionModule = modulename;
  meth_meth_PackageManager__apply239->definitionLine = 88;
// compilenode returning block238
  gc_frame_newslot(block238);
// Begin line 111
  setline(111);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block244 = alloc_Block(NULL, NULL, "PackageManager", 111);
  gc_frame_newslot(block244);
  block_savedest(block244);
  Object closure245 = createclosure(6, "_apply");
setclosureframe(closure245, stackframe);
  addtoclosure(closure245, var_i);
  addtoclosure(closure245, var_args);
  addtoclosure(closure245, var_ran);
  addtoclosure(closure245, var_on);
  addtoclosure(closure245, var_block);
  addtoclosure(closure245, selfslot);
  struct UserObject *uo245 = (struct UserObject*)block244;
  uo245->data[0] = (Object)closure245;
  Method *meth_meth_PackageManager__apply245 = addmethod2pos(block244, "_apply", &meth_PackageManager__apply245, 0);
int argcv_meth_PackageManager__apply245[] = {0};
meth_meth_PackageManager__apply245->type = alloc_MethodType(1, argcv_meth_PackageManager__apply245);
  meth_meth_PackageManager__apply245->definitionModule = modulename;
  meth_meth_PackageManager__apply245->definitionLine = 88;
// compilenode returning block244
  gc_frame_newslot(block244);
  params[0] = block238;
  params[1] = block244;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call284 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe237);
// compilenode returning call284
  gc_frame_end(frame);
  return call284;
}
Object meth_PackageManager_listInstalled286(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 115
  setline(115);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 114
  setline(114);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit287 == NULL) {
    strlit287 = alloc_String("Installed packages:");
    gc_root(strlit287);
  }
// compilenode returning strlit287
  params[0] = strlit287;
  Object call288 = gracelib_print(NULL, 1,  params);
// compilenode returning call288
// Begin line 120
  setline(120);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 115
  setline(115);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe290 = gc_frame_new();
  if (strlit291 == NULL) {
    strlit291 = alloc_String("usr/lib/grace/packages/");
    gc_root(strlit291);
  }
// compilenode returning strlit291
  gc_frame_newslot(strlit291);
// compilenode returning *var_io
  params[0] = strlit291;
  partcv[0] = 1;
  Object call292 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe290);
// compilenode returning call292
  Object if289 = done;
  if (istrue(call292)) {
// Begin line 116
  setline(116);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit293 == NULL) {
    strlit293 = alloc_String("Going into the first call");
    gc_root(strlit293);
  }
// compilenode returning strlit293
  params[0] = strlit293;
  Object call294 = gracelib_print(NULL, 1,  params);
// compilenode returning call294
// Begin line 118
  setline(118);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 117
  setline(117);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit295 == NULL) {
    strlit295 = alloc_String("usr/lib/grace/packages");
    gc_root(strlit295);
  }
// compilenode returning strlit295
  *var_globalDir = strlit295;
  if (strlit295 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 119
  setline(119);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe296 = gc_frame_new();
// Begin line 118
  setline(118);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_globalDir
  gc_frame_newslot(*var_globalDir);
  if (strlit297 == NULL) {
    strlit297 = alloc_String("");
    gc_root(strlit297);
  }
// compilenode returning strlit297
  gc_frame_newslot(strlit297);
  params[0] = *var_globalDir;
  params[1] = strlit297;
  partcv[0] = 2;
  Object call298 = callmethodflags(self, "recurseDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe296);
// compilenode returning call298
    gc_frame_newslot(call298);
    if289 = call298;
  } else {
  }
// compilenode returning if289
// Begin line 124
  setline(124);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 120
  setline(120);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe300 = gc_frame_new();
  int callframe302 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call303 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe302);
// compilenode returning call303
// compilenode returning call303
  if (strlit304 == NULL) {
    strlit304 = alloc_String("HOME");
    gc_root(strlit304);
  }
// compilenode returning strlit304
  params[0] = strlit304;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres305 = callmethod(call303, "[]", 1, partcv, params);
// compilenode returning idxres305
  int op_slot_left_301 = gc_frame_newslot(idxres305);
  if (strlit306 == NULL) {
    strlit306 = alloc_String("/grace/packages/");
    gc_root(strlit306);
  }
// compilenode returning strlit306
  int op_slot_right_301 = gc_frame_newslot(strlit306);
  params[0] = strlit306;
  partcv[0] = 1;
  Object opresult308 = callmethod(idxres305, "++", 1, partcv, params);
// compilenode returning opresult308
  gc_frame_newslot(opresult308);
// compilenode returning *var_io
  params[0] = opresult308;
  partcv[0] = 1;
  Object call309 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe300);
// compilenode returning call309
  Object if299 = done;
  if (istrue(call309)) {
// Begin line 122
  setline(122);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 121
  setline(121);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe311 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call312 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe311);
// compilenode returning call312
// compilenode returning call312
  if (strlit313 == NULL) {
    strlit313 = alloc_String("HOME");
    gc_root(strlit313);
  }
// compilenode returning strlit313
  params[0] = strlit313;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres314 = callmethod(call312, "[]", 1, partcv, params);
// compilenode returning idxres314
  int op_slot_left_310 = gc_frame_newslot(idxres314);
  if (strlit315 == NULL) {
    strlit315 = alloc_String("/grace/packages");
    gc_root(strlit315);
  }
// compilenode returning strlit315
  int op_slot_right_310 = gc_frame_newslot(strlit315);
  params[0] = strlit315;
  partcv[0] = 1;
  Object opresult317 = callmethod(idxres314, "++", 1, partcv, params);
// compilenode returning opresult317
  *var_usrDir = opresult317;
  if (opresult317 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 123
  setline(123);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe318 = gc_frame_new();
// Begin line 122
  setline(122);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_usrDir
  gc_frame_newslot(*var_usrDir);
  if (strlit319 == NULL) {
    strlit319 = alloc_String(" ");
    gc_root(strlit319);
  }
// compilenode returning strlit319
  gc_frame_newslot(strlit319);
  params[0] = *var_usrDir;
  params[1] = strlit319;
  partcv[0] = 2;
  Object call320 = callmethodflags(self, "recurseDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe318);
// compilenode returning call320
    gc_frame_newslot(call320);
    if299 = call320;
  } else {
  }
// compilenode returning if299
  gc_frame_end(frame);
  return if299;
}
Object meth_PackageManager__apply326(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 135
  setline(135);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 129
  setline(129);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_p
  int op_slot_left_329 = gc_frame_newslot(*var_p);
  if (strlit330 == NULL) {
    strlit330 = alloc_String("..");
    gc_root(strlit330);
  }
// compilenode returning strlit330
  int op_slot_right_329 = gc_frame_newslot(strlit330);
  params[0] = strlit330;
  partcv[0] = 1;
  Object opresult332 = callmethod(*var_p, "!=", 1, partcv, params);
// compilenode returning opresult332
  int op_slot_left_328 = gc_frame_newslot(opresult332);
// compilenode returning *var_p
  int op_slot_left_333 = gc_frame_newslot(*var_p);
  if (strlit334 == NULL) {
    strlit334 = alloc_String(".");
    gc_root(strlit334);
  }
// compilenode returning strlit334
  int op_slot_right_333 = gc_frame_newslot(strlit334);
  params[0] = strlit334;
  partcv[0] = 1;
  Object opresult336 = callmethod(*var_p, "!=", 1, partcv, params);
// compilenode returning opresult336
  int op_slot_right_328 = gc_frame_newslot(opresult336);
  params[0] = opresult336;
  partcv[0] = 1;
  Object opresult338 = callmethod(opresult332, "&&", 1, partcv, params);
// compilenode returning opresult338
  Object if327 = done;
  if (istrue(opresult338)) {
// Begin line 130
  setline(130);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_padding
  int op_slot_left_339 = gc_frame_newslot(*var_padding);
// compilenode returning *var_p
  int op_slot_right_339 = gc_frame_newslot(*var_p);
  params[0] = *var_p;
  partcv[0] = 1;
  Object opresult341 = callmethod(*var_padding, "++", 1, partcv, params);
// compilenode returning opresult341
  params[0] = opresult341;
  Object call342 = gracelib_print(NULL, 1,  params);
// compilenode returning call342
// Begin line 134
  setline(134);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 131
  setline(131);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe345 = gc_frame_new();
  int callframe346 = gc_frame_new();
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
// compilenode returning *var_io
  params[0] = opresult353;
  partcv[0] = 1;
  Object call354 = callmethod(*var_io, "getdir",
    1, partcv, params);
  gc_frame_end(callframe346);
// compilenode returning call354
  partcv[0] = 0;
  Object call355 = callmethod(call354, "size",
    1, partcv, params);
  gc_frame_end(callframe345);
// compilenode returning call355
// compilenode returning call355
  int op_slot_left_344 = gc_frame_newslot(call355);
  Object num356 = alloc_Float64(0);
// compilenode returning num356
  int op_slot_right_344 = gc_frame_newslot(num356);
  params[0] = num356;
  partcv[0] = 1;
  Object opresult358 = callmethod(call355, ">", 1, partcv, params);
// compilenode returning opresult358
  Object if343 = done;
  if (istrue(opresult358)) {
// Begin line 133
  setline(133);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe359 = gc_frame_new();
// Begin line 132
  setline(132);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_path
  int op_slot_left_361 = gc_frame_newslot(*var_path);
  if (strlit362 == NULL) {
    strlit362 = alloc_String("/");
    gc_root(strlit362);
  }
// compilenode returning strlit362
  int op_slot_right_361 = gc_frame_newslot(strlit362);
  params[0] = strlit362;
  partcv[0] = 1;
  Object opresult364 = callmethod(*var_path, "++", 1, partcv, params);
// compilenode returning opresult364
  int op_slot_left_360 = gc_frame_newslot(opresult364);
// compilenode returning *var_p
  int op_slot_right_360 = gc_frame_newslot(*var_p);
  params[0] = *var_p;
  partcv[0] = 1;
  Object opresult366 = callmethod(opresult364, "++", 1, partcv, params);
// compilenode returning opresult366
  gc_frame_newslot(opresult366);
// compilenode returning *var_padding
  int op_slot_left_367 = gc_frame_newslot(*var_padding);
  if (strlit368 == NULL) {
    strlit368 = alloc_String("   ");
    gc_root(strlit368);
  }
// compilenode returning strlit368
  int op_slot_right_367 = gc_frame_newslot(strlit368);
  params[0] = strlit368;
  partcv[0] = 1;
  Object opresult370 = callmethod(*var_padding, "++", 1, partcv, params);
// compilenode returning opresult370
  gc_frame_newslot(opresult370);
  params[0] = opresult366;
  params[1] = opresult370;
  partcv[0] = 2;
  Object call371 = callmethodflags(self, "recurseDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe359);
// compilenode returning call371
    gc_frame_newslot(call371);
    if343 = call371;
  } else {
  }
// compilenode returning if343
    gc_frame_newslot(if343);
    if327 = if343;
  } else {
  }
// compilenode returning if327
  gc_frame_end(frame);
  return if327;
}
Object meth_PackageManager_recurseDirectory321(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 127
  setline(127);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe322 = gc_frame_new();
// compilenode returning *var_path
  gc_frame_newslot(*var_path);
// compilenode returning *var_io
  params[0] = *var_path;
  partcv[0] = 1;
  Object call323 = callmethod(*var_io, "getdir",
    1, partcv, params);
  gc_frame_end(callframe322);
// compilenode returning call323
  *var_pathContents = call323;
  if (call323 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 136
  setline(136);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe324 = gc_frame_new();
// Begin line 128
  setline(128);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pathContents
  gc_frame_newslot(*var_pathContents);
// Begin line 136
  setline(136);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block325 = alloc_Block(NULL, NULL, "PackageManager", 136);
  gc_frame_newslot(block325);
  block_savedest(block325);
  Object closure326 = createclosure(4, "_apply");
setclosureframe(closure326, stackframe);
  addtoclosure(closure326, var_padding);
  addtoclosure(closure326, var_path);
  addtoclosure(closure326, var_io);
  addtoclosure(closure326, selfslot);
  struct UserObject *uo326 = (struct UserObject*)block325;
  uo326->data[0] = (Object)closure326;
  Method *meth_meth_PackageManager__apply326 = addmethod2pos(block325, "_apply", &meth_PackageManager__apply326, 0);
int argcv_meth_PackageManager__apply326[] = {1};
meth_meth_PackageManager__apply326->type = alloc_MethodType(1, argcv_meth_PackageManager__apply326);
  meth_meth_PackageManager__apply326->definitionModule = modulename;
  meth_meth_PackageManager__apply326->definitionLine = 121;
// compilenode returning block325
  gc_frame_newslot(block325);
  params[0] = *var_pathContents;
  params[1] = block325;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call373 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe324);
// compilenode returning call373
  gc_frame_end(frame);
  return call373;
}
Object meth_PackageManager__apply381(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 142
  setline(142);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe382 = gc_frame_new();
// compilenode returning *var_im
  gc_frame_newslot(*var_im);
  params[0] = *var_im;
  partcv[0] = 1;
  Object call383 = callmethodflags(self, "write", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe382);
// compilenode returning call383
  gc_frame_end(frame);
  return call383;
}
Object meth_PackageManager__apply388(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 145
  setline(145);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe389 = gc_frame_new();
// compilenode returning *var_im
  gc_frame_newslot(*var_im);
  params[0] = *var_im;
  partcv[0] = 1;
  Object call390 = callmethodflags(self, "compile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe389);
// compilenode returning call390
  gc_frame_end(frame);
  return call390;
}
Object meth_PackageManager_doGet374(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 140
  setline(140);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 139
  setline(139);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array375 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array375
  *var_imported = array375;
  if (array375 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 141
  setline(141);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe377 = gc_frame_new();
// Begin line 140
  setline(140);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_impAddress
  gc_frame_newslot(*var_impAddress);
  params[0] = *var_impAddress;
  partcv[0] = 1;
  Object call378 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe377);
// compilenode returning call378
// Begin line 144
  setline(144);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe379 = gc_frame_new();
// Begin line 141
  setline(141);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_imported
  gc_frame_newslot(*var_imported);
// Begin line 144
  setline(144);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block380 = alloc_Block(NULL, NULL, "PackageManager", 144);
  gc_frame_newslot(block380);
  block_savedest(block380);
  Object closure381 = createclosure(1, "_apply");
setclosureframe(closure381, stackframe);
  addtoclosure(closure381, selfslot);
  struct UserObject *uo381 = (struct UserObject*)block380;
  uo381->data[0] = (Object)closure381;
  Method *meth_meth_PackageManager__apply381 = addmethod2pos(block380, "_apply", &meth_PackageManager__apply381, 0);
int argcv_meth_PackageManager__apply381[] = {1};
meth_meth_PackageManager__apply381->type = alloc_MethodType(1, argcv_meth_PackageManager__apply381);
  meth_meth_PackageManager__apply381->definitionModule = modulename;
  meth_meth_PackageManager__apply381->definitionLine = 131;
// compilenode returning block380
  gc_frame_newslot(block380);
  params[0] = *var_imported;
  params[1] = block380;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call385 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe379);
// compilenode returning call385
// Begin line 147
  setline(147);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe386 = gc_frame_new();
// Begin line 144
  setline(144);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_imported
  gc_frame_newslot(*var_imported);
// Begin line 147
  setline(147);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block387 = alloc_Block(NULL, NULL, "PackageManager", 147);
  gc_frame_newslot(block387);
  block_savedest(block387);
  Object closure388 = createclosure(1, "_apply");
setclosureframe(closure388, stackframe);
  addtoclosure(closure388, selfslot);
  struct UserObject *uo388 = (struct UserObject*)block387;
  uo388->data[0] = (Object)closure388;
  Method *meth_meth_PackageManager__apply388 = addmethod2pos(block387, "_apply", &meth_PackageManager__apply388, 0);
int argcv_meth_PackageManager__apply388[] = {1};
meth_meth_PackageManager__apply388->type = alloc_MethodType(1, argcv_meth_PackageManager__apply388);
  meth_meth_PackageManager__apply388->definitionModule = modulename;
  meth_meth_PackageManager__apply388->definitionLine = 131;
// compilenode returning block387
  gc_frame_newslot(block387);
  params[0] = *var_imported;
  params[1] = block387;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call392 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe386);
// compilenode returning call392
  gc_frame_end(frame);
  return call392;
}
Object meth_PackageManager_setFile393(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 158
  setline(158);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 150
  setline(150);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe396 = gc_frame_new();
  int callframe398 = gc_frame_new();
// compilenode returning *var_fileAddress
  partcv[0] = 0;
  Object call399 = callmethod(*var_fileAddress, "size",
    1, partcv, params);
  gc_frame_end(callframe398);
// compilenode returning call399
// compilenode returning call399
  int op_slot_left_397 = gc_frame_newslot(call399);
  Object num400 = alloc_Float64(5);
// compilenode returning num400
  int op_slot_right_397 = gc_frame_newslot(num400);
  params[0] = num400;
  partcv[0] = 1;
  Object diff402 = callmethod(call399, "-", 1, partcv, params);
// compilenode returning diff402
  gc_frame_newslot(diff402);
  int callframe403 = gc_frame_new();
// compilenode returning *var_fileAddress
  partcv[0] = 0;
  Object call404 = callmethod(*var_fileAddress, "size",
    1, partcv, params);
  gc_frame_end(callframe403);
// compilenode returning call404
// compilenode returning call404
  gc_frame_newslot(call404);
// compilenode returning *var_fileAddress
  params[0] = diff402;
  params[1] = call404;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call405 = callmethod(*var_fileAddress, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe396);
// compilenode returning call405
  int op_slot_left_395 = gc_frame_newslot(call405);
  if (strlit406 == NULL) {
    strlit406 = alloc_String(".grace");
    gc_root(strlit406);
  }
// compilenode returning strlit406
  int op_slot_right_395 = gc_frame_newslot(strlit406);
  params[0] = strlit406;
  partcv[0] = 1;
  Object opresult408 = callmethod(call405, "==", 1, partcv, params);
// compilenode returning opresult408
  Object if394 = done;
  if (istrue(opresult408)) {
// Begin line 151
  setline(151);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object inheritingObject409 = inheritingObject;
  if (isTailObject) {
    isTailObject = 0;
    inheritingObject = NULL;
  }
  Object obj409 = alloc_userobj2(4,4, objclass409);
  gc_frame_newslot(obj409);
// OBJECT OUTER DEC outer
  adddatum2(obj409, self, 0);
  addmethodreal(obj409,"outer", &reader_PackageManager_outer_410);
  Object oldself409 = self;
  struct StackFrameObject *oldstackframe409 = stackframe;
  stackframe = alloc_StackFrame(1, oldstackframe409);
  gc_frame_newslot((Object)stackframe);
  self = obj409;
  Object *oldselfslot409 = selfslot;
  selfslot = &stackframe->slots[0];
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  Object thisouter409 = (*(struct UserObject *)self).data[0], lowerouter409 = thisouter409;
  if (inheritingObject409) {
    struct UserObject *inho409 = (struct UserObject *)inheritingObject409;
    while (inho409->super != GraceDefaultObject) inho409 = (struct UserObject *)inho409->super;
    inho409->super = obj409;
    self = inheritingObject409;
    *selfslot = self;
    lowerouter409 = (*(struct UserObject *)self).data[0];
    (*(struct UserObject *)self).data[0] = thisouter409;
  }
if (objclass409 == NULL) {
  Method *reader411 = addmethodrealflags(obj409, "address",&reader_PackageManager_address_411, 0);
  Method *writer411 = addmethodrealflags(obj409, "address:=",&writer_PackageManager_address_411, 0);
  reader411->definitionModule = modulename;
  writer411->definitionModule = modulename;
  reader411->definitionLine = 153;
  writer411->definitionLine = 153;
}
obj409->flags |= OFLAG_MUTABLE;
adddatum2(obj409, alloc_Undefined(), 1);
if (objclass409 == NULL) {
  Method *reader412 = addmethodrealflags(obj409, "data",&reader_PackageManager_data_412, 0);
  Method *writer412 = addmethodrealflags(obj409, "data:=",&writer_PackageManager_data_412, 0);
  reader412->definitionModule = modulename;
  writer412->definitionModule = modulename;
  reader412->definitionLine = 154;
  writer412->definitionLine = 154;
}
obj409->flags |= OFLAG_MUTABLE;
adddatum2(obj409, alloc_Undefined(), 2);
  sourceObject = obj409;
// Begin line 152
  setline(152);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_fileAddress
  adddatum2(obj409, *var_fileAddress, 1);
  sourceObject = obj409;
  adddatum2(obj409, undefined, 2);
objclass409 = obj409->class;
  objclass409->definitionModule = modulename;
  objclass409->definitionLine = 151;
  (*(struct UserObject *)self).data[0] = lowerouter409;
  self = oldself409;
  selfslot = oldselfslot409;
  stackframe = oldstackframe409;
// compilenode returning obj409
  *var_file = obj409;
  if (obj409 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 155
  setline(155);
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
  Object bool414 = alloc_Boolean(1);
// compilenode returning bool414
  return bool414;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if394 = undefined;
  } else {
  }
// compilenode returning if394
// Begin line 158
  setline(158);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool415 = alloc_Boolean(0);
// compilenode returning bool415
  return bool415;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply452(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int callframe454 = gc_frame_new();
// compilenode returning *var_toProcess
  partcv[0] = 0;
  Object call455 = callmethod(*var_toProcess, "size",
    1, partcv, params);
  gc_frame_end(callframe454);
// compilenode returning call455
// compilenode returning call455
  int op_slot_left_453 = gc_frame_newslot(call455);
  Object num456 = alloc_Float64(0);
// compilenode returning num456
  int op_slot_right_453 = gc_frame_newslot(num456);
  params[0] = num456;
  partcv[0] = 1;
  Object opresult458 = callmethod(call455, ">", 1, partcv, params);
// compilenode returning opresult458
  gc_frame_end(frame);
  return opresult458;
}
Object meth_PackageManager__apply461(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 174
  setline(174);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe462 = gc_frame_new();
// Begin line 173
  setline(173);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe463 = gc_frame_new();
// compilenode returning *var_toProcess
  partcv[0] = 0;
  Object call464 = callmethod(*var_toProcess, "pop",
    1, partcv, params);
  gc_frame_end(callframe463);
// compilenode returning call464
// compilenode returning call464
  gc_frame_newslot(call464);
  params[0] = call464;
  partcv[0] = 1;
  Object call465 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe462);
// compilenode returning call465
  gc_frame_end(frame);
  return call465;
}
Object meth_PackageManager_fetchImports416(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 184
  setline(184);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 162
  setline(162);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe418 = gc_frame_new();
// compilenode returning *var_fileAddress
  gc_frame_newslot(*var_fileAddress);
  params[0] = *var_fileAddress;
  partcv[0] = 1;
  Object call419 = callmethodflags(self, "setFile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe418);
// compilenode returning call419
  Object if417 = done;
  if (istrue(call419)) {
// Begin line 164
  setline(164);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 163
  setline(163);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit422 == NULL) {
    strlit422 = alloc_String("File address ");
    gc_root(strlit422);
  }
// compilenode returning strlit422
  int op_slot_left_421 = gc_frame_newslot(strlit422);
  int callframe423 = gc_frame_new();
// compilenode returning *var_curFile
  partcv[0] = 0;
  Object call424 = callmethod(*var_curFile, "address",
    1, partcv, params);
  gc_frame_end(callframe423);
// compilenode returning call424
// compilenode returning call424
  int op_slot_right_421 = gc_frame_newslot(call424);
  params[0] = call424;
  partcv[0] = 1;
  Object opresult426 = callmethod(strlit422, "++", 1, partcv, params);
// compilenode returning opresult426
  int op_slot_left_420 = gc_frame_newslot(opresult426);
  if (strlit427 == NULL) {
    strlit427 = alloc_String("");
    gc_root(strlit427);
  }
// compilenode returning strlit427
  int op_slot_right_420 = gc_frame_newslot(strlit427);
  params[0] = strlit427;
  partcv[0] = 1;
  Object opresult429 = callmethod(opresult426, "++", 1, partcv, params);
// compilenode returning opresult429
  params[0] = opresult429;
  Object call430 = gracelib_print(NULL, 1,  params);
// compilenode returning call430
// Begin line 168
  setline(168);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 164
  setline(164);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe433 = gc_frame_new();
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call434 = callmethodflags(self, "setFileData", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe433);
// compilenode returning call434
  int op_slot_left_432 = gc_frame_newslot(call434);
  Object bool435 = alloc_Boolean(0);
// compilenode returning bool435
  int op_slot_right_432 = gc_frame_newslot(bool435);
  params[0] = bool435;
  partcv[0] = 1;
  Object opresult437 = callmethod(call434, "==", 1, partcv, params);
// compilenode returning opresult437
  Object if431 = done;
  if (istrue(opresult437)) {
// Begin line 166
  setline(166);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 165
  setline(165);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit438 == NULL) {
    strlit438 = alloc_String("Could not retrieve file data");
    gc_root(strlit438);
  }
// compilenode returning strlit438
  params[0] = strlit438;
  Object call439 = gracelib_print(NULL, 1,  params);
// compilenode returning call439
// Begin line 167
  setline(167);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 166
  setline(166);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool440 = alloc_Boolean(0);
// compilenode returning bool440
  return bool440;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if431 = undefined;
  } else {
  }
// compilenode returning if431
// Begin line 177
  setline(177);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 168
  setline(168);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe442 = gc_frame_new();
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call443 = callmethodflags(self, "validateFile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe442);
// compilenode returning call443
  Object if441 = done;
  if (istrue(call443)) {
// Begin line 170
  setline(170);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe444 = gc_frame_new();
// Begin line 169
  setline(169);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
// compilenode returning *var_imported
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call445 = callmethod(*var_imported, "push",
    1, partcv, params);
  gc_frame_end(callframe444);
// compilenode returning call445
// Begin line 171
  setline(171);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 170
  setline(170);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array446 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array446
  *var_toProcess = array446;
  if (array446 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 172
  setline(172);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe448 = gc_frame_new();
// Begin line 171
  setline(171);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call449 = callmethodflags(self, "parseFile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe448);
// compilenode returning call449
// Begin line 175
  setline(175);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe450 = gc_frame_new();
// Begin line 172
  setline(172);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block451 = alloc_Block(NULL, NULL, "PackageManager", 172);
  gc_frame_newslot(block451);
  block_savedest(block451);
  Object closure452 = createclosure(2, "_apply");
setclosureframe(closure452, stackframe);
  addtoclosure(closure452, var_toProcess);
  addtoclosure(closure452, selfslot);
  struct UserObject *uo452 = (struct UserObject*)block451;
  uo452->data[0] = (Object)closure452;
  Method *meth_meth_PackageManager__apply452 = addmethod2pos(block451, "_apply", &meth_PackageManager__apply452, 0);
int argcv_meth_PackageManager__apply452[] = {0};
meth_meth_PackageManager__apply452->type = alloc_MethodType(1, argcv_meth_PackageManager__apply452);
  meth_meth_PackageManager__apply452->definitionModule = modulename;
  meth_meth_PackageManager__apply452->definitionLine = 163;
// compilenode returning block451
  gc_frame_newslot(block451);
// Begin line 175
  setline(175);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block460 = alloc_Block(NULL, NULL, "PackageManager", 175);
  gc_frame_newslot(block460);
  block_savedest(block460);
  Object closure461 = createclosure(2, "_apply");
setclosureframe(closure461, stackframe);
  addtoclosure(closure461, var_toProcess);
  addtoclosure(closure461, selfslot);
  struct UserObject *uo461 = (struct UserObject*)block460;
  uo461->data[0] = (Object)closure461;
  Method *meth_meth_PackageManager__apply461 = addmethod2pos(block460, "_apply", &meth_PackageManager__apply461, 0);
int argcv_meth_PackageManager__apply461[] = {0};
meth_meth_PackageManager__apply461->type = alloc_MethodType(1, argcv_meth_PackageManager__apply461);
  meth_meth_PackageManager__apply461->definitionModule = modulename;
  meth_meth_PackageManager__apply461->definitionLine = 172;
// compilenode returning block460
  gc_frame_newslot(block460);
  params[0] = block451;
  params[1] = block460;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call467 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe450);
// compilenode returning call467
// Begin line 176
  setline(176);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 175
  setline(175);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool468 = alloc_Boolean(1);
// compilenode returning bool468
  return bool468;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if441 = undefined;
  } else {
  }
// compilenode returning if441
    gc_frame_newslot(if441);
    if417 = if441;
  } else {
// Begin line 183
  setline(183);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 179
  setline(179);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe471 = gc_frame_new();
// compilenode returning *var_fileAddress
  int op_slot_left_472 = gc_frame_newslot(*var_fileAddress);
  if (strlit473 == NULL) {
    strlit473 = alloc_String(".grace");
    gc_root(strlit473);
  }
// compilenode returning strlit473
  int op_slot_right_472 = gc_frame_newslot(strlit473);
  params[0] = strlit473;
  partcv[0] = 1;
  Object opresult475 = callmethod(*var_fileAddress, "++", 1, partcv, params);
// compilenode returning opresult475
  gc_frame_newslot(opresult475);
  params[0] = opresult475;
  partcv[0] = 1;
  Object call476 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe471);
// compilenode returning call476
  int op_slot_left_470 = gc_frame_newslot(call476);
  Object bool477 = alloc_Boolean(0);
// compilenode returning bool477
  int op_slot_right_470 = gc_frame_newslot(bool477);
  params[0] = bool477;
  partcv[0] = 1;
  Object opresult479 = callmethod(call476, "==", 1, partcv, params);
// compilenode returning opresult479
  Object if469 = done;
  if (istrue(opresult479)) {
// Begin line 180
  setline(180);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit480 == NULL) {
    strlit480 = alloc_String("Could not locate file. Check file address.");
    gc_root(strlit480);
  }
// compilenode returning strlit480
  params[0] = strlit480;
  Object call481 = gracelib_print(NULL, 1,  params);
// compilenode returning call481
// Begin line 182
  setline(182);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 181
  setline(181);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool482 = alloc_Boolean(0);
// compilenode returning bool482
  return bool482;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if469 = undefined;
  } else {
  }
// compilenode returning if469
    gc_frame_newslot(if469);
    if417 = if469;
  }
// compilenode returning if417
  gc_frame_end(frame);
  return if417;
}
Object meth_PackageManager__apply537(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 202
  setline(202);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 201
  setline(201);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit538 == NULL) {
    strlit538 = alloc_String(" Response received");
    gc_root(strlit538);
  }
// compilenode returning strlit538
  params[0] = strlit538;
  Object call539 = gracelib_print(NULL, 1,  params);
// compilenode returning call539
// Begin line 203
  setline(203);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 198
  setline(198);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe540 = gc_frame_new();
// Begin line 203
  setline(203);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe541 = gc_frame_new();
// Begin line 202
  setline(202);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit542 == NULL) {
    strlit542 = alloc_String("utf-8");
    gc_root(strlit542);
  }
// compilenode returning strlit542
  gc_frame_newslot(strlit542);
// compilenode returning *var_d
  params[0] = strlit542;
  partcv[0] = 1;
  Object call543 = callmethod(*var_d, "decode",
    1, partcv, params);
  gc_frame_end(callframe541);
// compilenode returning call543
  gc_frame_newslot(call543);
// compilenode returning *var_file
  params[0] = call543;
  partcv[0] = 1;
  Object call544 = callmethod(*var_file, "data:=",
    1, partcv, params);
  gc_frame_end(callframe540);
// compilenode returning call544
// compilenode returning done
// Begin line 203
  setline(203);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool545 = alloc_Boolean(1);
// compilenode returning bool545
  block_return(realself, bool545);
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_setFileData483(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 211
  setline(211);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 187
  setline(187);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe486 = gc_frame_new();
  Object num487 = alloc_Float64(1);
// compilenode returning num487
  gc_frame_newslot(num487);
  Object num488 = alloc_Float64(4);
// compilenode returning num488
  gc_frame_newslot(num488);
  int callframe489 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call490 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe489);
// compilenode returning call490
// compilenode returning call490
  params[0] = num487;
  params[1] = num488;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call491 = callmethod(call490, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe486);
// compilenode returning call491
  int op_slot_left_485 = gc_frame_newslot(call491);
  if (strlit492 == NULL) {
    strlit492 = alloc_String("http");
    gc_root(strlit492);
  }
// compilenode returning strlit492
  int op_slot_right_485 = gc_frame_newslot(strlit492);
  params[0] = strlit492;
  partcv[0] = 1;
  Object opresult494 = callmethod(call491, "==", 1, partcv, params);
// compilenode returning opresult494
  Object if484 = done;
  if (istrue(opresult494)) {
// Begin line 188
  setline(188);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe495 = gc_frame_new();
  Object num496 = alloc_Float64(1);
// compilenode returning num496
  gc_frame_newslot(num496);
  Object num497 = alloc_Float64(4);
// compilenode returning num497
  gc_frame_newslot(num497);
  int callframe498 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call499 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe498);
// compilenode returning call499
// compilenode returning call499
  params[0] = num496;
  params[1] = num497;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call500 = callmethod(call499, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe495);
// compilenode returning call500
  *var_strippedUrl = call500;
  if (call500 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 196
  setline(196);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 189
  setline(189);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe503 = gc_frame_new();
  int callframe504 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call505 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe504);
// compilenode returning call505
// compilenode returning call505
  gc_frame_newslot(call505);
  params[0] = call505;
  partcv[0] = 1;
  Object call506 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe503);
// compilenode returning call506
  int op_slot_left_502 = gc_frame_newslot(call506);
  Object bool507 = alloc_Boolean(0);
// compilenode returning bool507
  int op_slot_right_502 = gc_frame_newslot(bool507);
  params[0] = bool507;
  partcv[0] = 1;
  Object opresult509 = callmethod(call506, "!=", 1, partcv, params);
// compilenode returning opresult509
  Object if501 = done;
  if (istrue(opresult509)) {
// Begin line 191
  setline(191);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe510 = gc_frame_new();
// Begin line 190
  setline(190);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_strippedUrl
  gc_frame_newslot(*var_strippedUrl);
  params[0] = *var_strippedUrl;
  partcv[0] = 1;
  Object call511 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe510);
// compilenode returning call511
  *var_findData = call511;
  if (call511 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 195
  setline(195);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 191
  setline(191);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  int op_slot_left_513 = gc_frame_newslot(*var_findData);
  Object num514 = alloc_Float64(0);
// compilenode returning num514
  int op_slot_right_513 = gc_frame_newslot(num514);
  params[0] = num514;
  partcv[0] = 1;
  Object opresult516 = callmethod(*var_findData, ">", 1, partcv, params);
// compilenode returning opresult516
  Object if512 = done;
  if (istrue(opresult516)) {
// Begin line 193
  setline(193);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 189
  setline(189);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe517 = gc_frame_new();
// Begin line 192
  setline(192);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  gc_frame_newslot(*var_findData);
// compilenode returning *var_file
  params[0] = *var_findData;
  partcv[0] = 1;
  Object call518 = callmethod(*var_file, "data:=",
    1, partcv, params);
  gc_frame_end(callframe517);
// compilenode returning call518
// compilenode returning done
// Begin line 193
  setline(193);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool519 = alloc_Boolean(1);
// compilenode returning bool519
  return bool519;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if512 = undefined;
  } else {
  }
// compilenode returning if512
    gc_frame_newslot(if512);
    if501 = if512;
  } else {
  }
// compilenode returning if501
// Begin line 197
  setline(197);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 196
  setline(196);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe520 = gc_frame_new();
// compilenode returning *var_curl
  partcv[0] = 0;
  Object call521 = callmethod(*var_curl, "easy",
    1, partcv, params);
  gc_frame_end(callframe520);
// compilenode returning call521
// compilenode returning call521
  *var_req = call521;
  if (call521 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 198
  setline(198);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 196
  setline(196);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe522 = gc_frame_new();
// Begin line 197
  setline(197);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe523 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call524 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe523);
// compilenode returning call524
// compilenode returning call524
  gc_frame_newslot(call524);
// compilenode returning *var_req
  params[0] = call524;
  partcv[0] = 1;
  Object call525 = callmethod(*var_req, "url:=",
    1, partcv, params);
  gc_frame_end(callframe522);
// compilenode returning call525
// compilenode returning done
// Begin line 199
  setline(199);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 198
  setline(198);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit527 == NULL) {
    strlit527 = alloc_String("Searching for import: ");
    gc_root(strlit527);
  }
// compilenode returning strlit527
  int op_slot_left_526 = gc_frame_newslot(strlit527);
  int callframe528 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call529 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe528);
// compilenode returning call529
// compilenode returning call529
  int op_slot_right_526 = gc_frame_newslot(call529);
  params[0] = call529;
  partcv[0] = 1;
  Object opresult531 = callmethod(strlit527, "++", 1, partcv, params);
// compilenode returning opresult531
  params[0] = opresult531;
  Object call532 = gracelib_print(NULL, 1,  params);
// compilenode returning call532
// Begin line 200
  setline(200);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 199
  setline(199);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit533 == NULL) {
    strlit533 = alloc_String(" Connecting...");
    gc_root(strlit533);
  }
// compilenode returning strlit533
  params[0] = strlit533;
  Object call534 = gracelib_print(NULL, 1,  params);
// compilenode returning call534
// Begin line 205
  setline(205);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe535 = gc_frame_new();
  Object block536 = alloc_Block(NULL, NULL, "PackageManager", 205);
  gc_frame_newslot(block536);
  block_savedest(block536);
  Object closure537 = createclosure(2, "_apply");
setclosureframe(closure537, stackframe);
  addtoclosure(closure537, var_file);
  addtoclosure(closure537, selfslot);
  struct UserObject *uo537 = (struct UserObject*)block536;
  uo537->data[0] = (Object)closure537;
  Method *meth_meth_PackageManager__apply537 = addmethod2pos(block536, "_apply", &meth_PackageManager__apply537, 0);
int argcv_meth_PackageManager__apply537[] = {1};
meth_meth_PackageManager__apply537->type = alloc_MethodType(1, argcv_meth_PackageManager__apply537);
  meth_meth_PackageManager__apply537->definitionModule = modulename;
  meth_meth_PackageManager__apply537->definitionLine = 198;
// compilenode returning block536
  gc_frame_newslot(block536);
// Begin line 200
  setline(200);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_req
  params[0] = block536;
  partcv[0] = 1;
  Object call547 = callmethod(*var_req, "onReceive",
    1, partcv, params);
  gc_frame_end(callframe535);
// compilenode returning call547
// Begin line 205
  setline(205);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe548 = gc_frame_new();
// compilenode returning *var_req
  partcv[0] = 0;
  Object call549 = callmethod(*var_req, "perform",
    1, partcv, params);
  gc_frame_end(callframe548);
// compilenode returning call549
// compilenode returning call549
// Begin line 210
  setline(210);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 206
  setline(206);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe552 = gc_frame_new();
// compilenode returning *var_req
  partcv[0] = 0;
  Object call553 = callmethod(*var_req, "responseCode",
    1, partcv, params);
  gc_frame_end(callframe552);
// compilenode returning call553
// compilenode returning call553
  int op_slot_left_551 = gc_frame_newslot(call553);
  Object num554 = alloc_Float64(200);
// compilenode returning num554
  int op_slot_right_551 = gc_frame_newslot(num554);
  params[0] = num554;
  partcv[0] = 1;
  Object opresult556 = callmethod(call553, "!=", 1, partcv, params);
// compilenode returning opresult556
  Object if550 = done;
  if (istrue(opresult556)) {
// Begin line 207
  setline(207);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit561 == NULL) {
    strlit561 = alloc_String(" Could not locate import: Error ");
    gc_root(strlit561);
  }
// compilenode returning strlit561
  int op_slot_left_560 = gc_frame_newslot(strlit561);
  int callframe562 = gc_frame_new();
// compilenode returning *var_req
  partcv[0] = 0;
  Object call563 = callmethod(*var_req, "responseCode",
    1, partcv, params);
  gc_frame_end(callframe562);
// compilenode returning call563
// compilenode returning call563
  int op_slot_right_560 = gc_frame_newslot(call563);
  params[0] = call563;
  partcv[0] = 1;
  Object opresult565 = callmethod(strlit561, "++", 1, partcv, params);
// compilenode returning opresult565
  int op_slot_left_559 = gc_frame_newslot(opresult565);
  if (strlit566 == NULL) {
    strlit566 = alloc_String(" for import ");
    gc_root(strlit566);
  }
// compilenode returning strlit566
  int op_slot_right_559 = gc_frame_newslot(strlit566);
  params[0] = strlit566;
  partcv[0] = 1;
  Object opresult568 = callmethod(opresult565, "++", 1, partcv, params);
// compilenode returning opresult568
  int op_slot_left_558 = gc_frame_newslot(opresult568);
  int callframe569 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call570 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe569);
// compilenode returning call570
// compilenode returning call570
  int op_slot_right_558 = gc_frame_newslot(call570);
  params[0] = call570;
  partcv[0] = 1;
  Object opresult572 = callmethod(opresult568, "++", 1, partcv, params);
// compilenode returning opresult572
  int op_slot_left_557 = gc_frame_newslot(opresult572);
  if (strlit573 == NULL) {
    strlit573 = alloc_String("");
    gc_root(strlit573);
  }
// compilenode returning strlit573
  int op_slot_right_557 = gc_frame_newslot(strlit573);
  params[0] = strlit573;
  partcv[0] = 1;
  Object opresult575 = callmethod(opresult572, "++", 1, partcv, params);
// compilenode returning opresult575
  params[0] = opresult575;
  Object call576 = gracelib_print(NULL, 1,  params);
// compilenode returning call576
// Begin line 209
  setline(209);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 208
  setline(208);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool577 = alloc_Boolean(0);
// compilenode returning bool577
  return bool577;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if550 = undefined;
  } else {
  }
// compilenode returning if550
    gc_frame_newslot(if550);
    if484 = if550;
  } else {
  }
// compilenode returning if484
// Begin line 219
  setline(219);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 211
  setline(211);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe580 = gc_frame_new();
  int callframe581 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call582 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe581);
// compilenode returning call582
// compilenode returning call582
  gc_frame_newslot(call582);
  params[0] = call582;
  partcv[0] = 1;
  Object call583 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe580);
// compilenode returning call583
  int op_slot_left_579 = gc_frame_newslot(call583);
  Object bool584 = alloc_Boolean(0);
// compilenode returning bool584
  int op_slot_right_579 = gc_frame_newslot(bool584);
  params[0] = bool584;
  partcv[0] = 1;
  Object opresult586 = callmethod(call583, "!=", 1, partcv, params);
// compilenode returning opresult586
  Object if578 = done;
  if (istrue(opresult586)) {
// Begin line 212
  setline(212);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit587 == NULL) {
    strlit587 = alloc_String("Now searching in find existing");
    gc_root(strlit587);
  }
// compilenode returning strlit587
  params[0] = strlit587;
  Object call588 = gracelib_print(NULL, 1,  params);
// compilenode returning call588
// Begin line 214
  setline(214);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe589 = gc_frame_new();
// Begin line 213
  setline(213);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe590 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call591 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe590);
// compilenode returning call591
// compilenode returning call591
  gc_frame_newslot(call591);
  params[0] = call591;
  partcv[0] = 1;
  Object call592 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe589);
// compilenode returning call592
  *var_findData = call592;
  if (call592 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 218
  setline(218);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 214
  setline(214);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  int op_slot_left_594 = gc_frame_newslot(*var_findData);
  Object bool595 = alloc_Boolean(0);
// compilenode returning bool595
  int op_slot_right_594 = gc_frame_newslot(bool595);
  params[0] = bool595;
  partcv[0] = 1;
  Object opresult597 = callmethod(*var_findData, "!=", 1, partcv, params);
// compilenode returning opresult597
  Object if593 = done;
  if (istrue(opresult597)) {
// Begin line 216
  setline(216);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 213
  setline(213);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe598 = gc_frame_new();
// Begin line 215
  setline(215);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_findData
  gc_frame_newslot(*var_findData);
// compilenode returning *var_file
  params[0] = *var_findData;
  partcv[0] = 1;
  Object call599 = callmethod(*var_file, "data:=",
    1, partcv, params);
  gc_frame_end(callframe598);
// compilenode returning call599
// compilenode returning done
// Begin line 216
  setline(216);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool600 = alloc_Boolean(1);
// compilenode returning bool600
  return bool600;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if593 = undefined;
  } else {
  }
// compilenode returning if593
    gc_frame_newslot(if593);
    if578 = if593;
  } else {
  }
// compilenode returning if578
// Begin line 219
  setline(219);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool601 = alloc_Boolean(0);
// compilenode returning bool601
  return bool601;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_findExisting602(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 228
  setline(228);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 223
  setline(223);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe604 = gc_frame_new();
// compilenode returning *var_fileName
  gc_frame_newslot(*var_fileName);
// compilenode returning *var_io
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object call605 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe604);
// compilenode returning call605
  Object if603 = done;
  if (istrue(call605)) {
// Begin line 224
  setline(224);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe606 = gc_frame_new();
// compilenode returning *var_fileName
  gc_frame_newslot(*var_fileName);
  if (strlit607 == NULL) {
    strlit607 = alloc_String("r");
    gc_root(strlit607);
  }
// compilenode returning strlit607
  gc_frame_newslot(strlit607);
// compilenode returning *var_io
  params[0] = *var_fileName;
  params[1] = strlit607;
  partcv[0] = 2;
  Object call608 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe606);
// compilenode returning call608
  *var_open = call608;
  if (call608 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 225
  setline(225);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe609 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call610 = callmethod(*var_open, "read",
    1, partcv, params);
  gc_frame_end(callframe609);
// compilenode returning call610
// compilenode returning call610
  *var_ret = call610;
  if (call610 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 226
  setline(226);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_ret
  return *var_ret;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if603 = undefined;
  } else {
  }
// compilenode returning if603
// Begin line 231
  setline(231);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 228
  setline(228);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe612 = gc_frame_new();
  if (strlit614 == NULL) {
    strlit614 = alloc_String("/usr/lib/grace/modules/");
    gc_root(strlit614);
  }
// compilenode returning strlit614
  int op_slot_left_613 = gc_frame_newslot(strlit614);
// compilenode returning *var_fileName
  int op_slot_right_613 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult616 = callmethod(strlit614, "++", 1, partcv, params);
// compilenode returning opresult616
  gc_frame_newslot(opresult616);
// compilenode returning *var_io
  params[0] = opresult616;
  partcv[0] = 1;
  Object call617 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe612);
// compilenode returning call617
  Object if611 = done;
  if (istrue(call617)) {
// Begin line 230
  setline(230);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 229
  setline(229);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe618 = gc_frame_new();
  int callframe619 = gc_frame_new();
  if (strlit621 == NULL) {
    strlit621 = alloc_String("/usr/lib/grace/modules/");
    gc_root(strlit621);
  }
// compilenode returning strlit621
  int op_slot_left_620 = gc_frame_newslot(strlit621);
// compilenode returning *var_fileName
  int op_slot_right_620 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult623 = callmethod(strlit621, "++", 1, partcv, params);
// compilenode returning opresult623
  gc_frame_newslot(opresult623);
  if (strlit624 == NULL) {
    strlit624 = alloc_String("r");
    gc_root(strlit624);
  }
// compilenode returning strlit624
  gc_frame_newslot(strlit624);
// compilenode returning *var_io
  params[0] = opresult623;
  params[1] = strlit624;
  partcv[0] = 2;
  Object call625 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe619);
// compilenode returning call625
  partcv[0] = 0;
  Object call626 = callmethod(call625, "read",
    1, partcv, params);
  gc_frame_end(callframe618);
// compilenode returning call626
// compilenode returning call626
  return call626;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if611 = undefined;
  } else {
  }
// compilenode returning if611
// Begin line 232
  setline(232);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 231
  setline(231);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe627 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call628 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe627);
// compilenode returning call628
// compilenode returning call628
  if (strlit629 == NULL) {
    strlit629 = alloc_String("HOME");
    gc_root(strlit629);
  }
// compilenode returning strlit629
  params[0] = strlit629;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres630 = callmethod(call628, "[]", 1, partcv, params);
// compilenode returning idxres630
  *var_homePath = idxres630;
  if (idxres630 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 235
  setline(235);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 232
  setline(232);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe632 = gc_frame_new();
  if (strlit636 == NULL) {
    strlit636 = alloc_String("");
    gc_root(strlit636);
  }
// compilenode returning strlit636
  int op_slot_left_635 = gc_frame_newslot(strlit636);
// compilenode returning *var_homePath
  int op_slot_right_635 = gc_frame_newslot(*var_homePath);
  params[0] = *var_homePath;
  partcv[0] = 1;
  Object opresult638 = callmethod(strlit636, "++", 1, partcv, params);
// compilenode returning opresult638
  int op_slot_left_634 = gc_frame_newslot(opresult638);
  if (strlit639 == NULL) {
    strlit639 = alloc_String("/.local/share/grace/modules/");
    gc_root(strlit639);
  }
// compilenode returning strlit639
  int op_slot_right_634 = gc_frame_newslot(strlit639);
  params[0] = strlit639;
  partcv[0] = 1;
  Object opresult641 = callmethod(opresult638, "++", 1, partcv, params);
// compilenode returning opresult641
  int op_slot_left_633 = gc_frame_newslot(opresult641);
// compilenode returning *var_fileName
  int op_slot_right_633 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult643 = callmethod(opresult641, "++", 1, partcv, params);
// compilenode returning opresult643
  gc_frame_newslot(opresult643);
// compilenode returning *var_io
  params[0] = opresult643;
  partcv[0] = 1;
  Object call644 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe632);
// compilenode returning call644
  Object if631 = done;
  if (istrue(call644)) {
// Begin line 234
  setline(234);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 233
  setline(233);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe645 = gc_frame_new();
  int callframe646 = gc_frame_new();
  if (strlit650 == NULL) {
    strlit650 = alloc_String("");
    gc_root(strlit650);
  }
// compilenode returning strlit650
  int op_slot_left_649 = gc_frame_newslot(strlit650);
// compilenode returning *var_homePath
  int op_slot_right_649 = gc_frame_newslot(*var_homePath);
  params[0] = *var_homePath;
  partcv[0] = 1;
  Object opresult652 = callmethod(strlit650, "++", 1, partcv, params);
// compilenode returning opresult652
  int op_slot_left_648 = gc_frame_newslot(opresult652);
  if (strlit653 == NULL) {
    strlit653 = alloc_String("/.local/share/grace/modules/");
    gc_root(strlit653);
  }
// compilenode returning strlit653
  int op_slot_right_648 = gc_frame_newslot(strlit653);
  params[0] = strlit653;
  partcv[0] = 1;
  Object opresult655 = callmethod(opresult652, "++", 1, partcv, params);
// compilenode returning opresult655
  int op_slot_left_647 = gc_frame_newslot(opresult655);
// compilenode returning *var_fileName
  int op_slot_right_647 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult657 = callmethod(opresult655, "++", 1, partcv, params);
// compilenode returning opresult657
  gc_frame_newslot(opresult657);
  if (strlit658 == NULL) {
    strlit658 = alloc_String("r");
    gc_root(strlit658);
  }
// compilenode returning strlit658
  gc_frame_newslot(strlit658);
// compilenode returning *var_io
  params[0] = opresult657;
  params[1] = strlit658;
  partcv[0] = 2;
  Object call659 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe646);
// compilenode returning call659
  partcv[0] = 0;
  Object call660 = callmethod(call659, "read",
    1, partcv, params);
  gc_frame_end(callframe645);
// compilenode returning call660
// compilenode returning call660
  return call660;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if631 = undefined;
  } else {
  }
// compilenode returning if631
// Begin line 238
  setline(238);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 235
  setline(235);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe662 = gc_frame_new();
  if (strlit666 == NULL) {
    strlit666 = alloc_String("");
    gc_root(strlit666);
  }
// compilenode returning strlit666
  int op_slot_left_665 = gc_frame_newslot(strlit666);
  int callframe667 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call668 = callmethod(*var_sys, "execPath",
    1, partcv, params);
  gc_frame_end(callframe667);
// compilenode returning call668
// compilenode returning call668
  int op_slot_right_665 = gc_frame_newslot(call668);
  params[0] = call668;
  partcv[0] = 1;
  Object opresult670 = callmethod(strlit666, "++", 1, partcv, params);
// compilenode returning opresult670
  int op_slot_left_664 = gc_frame_newslot(opresult670);
  if (strlit671 == NULL) {
    strlit671 = alloc_String("/../lib/minigrace/modules/");
    gc_root(strlit671);
  }
// compilenode returning strlit671
  int op_slot_right_664 = gc_frame_newslot(strlit671);
  params[0] = strlit671;
  partcv[0] = 1;
  Object opresult673 = callmethod(opresult670, "++", 1, partcv, params);
// compilenode returning opresult673
  int op_slot_left_663 = gc_frame_newslot(opresult673);
// compilenode returning *var_fileName
  int op_slot_right_663 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult675 = callmethod(opresult673, "++", 1, partcv, params);
// compilenode returning opresult675
  gc_frame_newslot(opresult675);
// compilenode returning *var_io
  params[0] = opresult675;
  partcv[0] = 1;
  Object call676 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe662);
// compilenode returning call676
  Object if661 = done;
  if (istrue(call676)) {
// Begin line 237
  setline(237);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 236
  setline(236);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe677 = gc_frame_new();
  int callframe678 = gc_frame_new();
  if (strlit682 == NULL) {
    strlit682 = alloc_String("");
    gc_root(strlit682);
  }
// compilenode returning strlit682
  int op_slot_left_681 = gc_frame_newslot(strlit682);
  int callframe683 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call684 = callmethod(*var_sys, "execPath",
    1, partcv, params);
  gc_frame_end(callframe683);
// compilenode returning call684
// compilenode returning call684
  int op_slot_right_681 = gc_frame_newslot(call684);
  params[0] = call684;
  partcv[0] = 1;
  Object opresult686 = callmethod(strlit682, "++", 1, partcv, params);
// compilenode returning opresult686
  int op_slot_left_680 = gc_frame_newslot(opresult686);
  if (strlit687 == NULL) {
    strlit687 = alloc_String("/../lib/minigrace/modules/");
    gc_root(strlit687);
  }
// compilenode returning strlit687
  int op_slot_right_680 = gc_frame_newslot(strlit687);
  params[0] = strlit687;
  partcv[0] = 1;
  Object opresult689 = callmethod(opresult686, "++", 1, partcv, params);
// compilenode returning opresult689
  int op_slot_left_679 = gc_frame_newslot(opresult689);
// compilenode returning *var_fileName
  int op_slot_right_679 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult691 = callmethod(opresult689, "++", 1, partcv, params);
// compilenode returning opresult691
  gc_frame_newslot(opresult691);
  if (strlit692 == NULL) {
    strlit692 = alloc_String("r");
    gc_root(strlit692);
  }
// compilenode returning strlit692
  gc_frame_newslot(strlit692);
// compilenode returning *var_io
  params[0] = opresult691;
  params[1] = strlit692;
  partcv[0] = 2;
  Object call693 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe678);
// compilenode returning call693
  partcv[0] = 0;
  Object call694 = callmethod(call693, "read",
    1, partcv, params);
  gc_frame_end(callframe677);
// compilenode returning call694
// compilenode returning call694
  return call694;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if661 = undefined;
  } else {
  }
// compilenode returning if661
// Begin line 241
  setline(241);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 238
  setline(238);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe696 = gc_frame_new();
  if (strlit700 == NULL) {
    strlit700 = alloc_String("");
    gc_root(strlit700);
  }
// compilenode returning strlit700
  int op_slot_left_699 = gc_frame_newslot(strlit700);
  int callframe701 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call702 = callmethod(*var_sys, "execPath",
    1, partcv, params);
  gc_frame_end(callframe701);
// compilenode returning call702
// compilenode returning call702
  int op_slot_right_699 = gc_frame_newslot(call702);
  params[0] = call702;
  partcv[0] = 1;
  Object opresult704 = callmethod(strlit700, "++", 1, partcv, params);
// compilenode returning opresult704
  int op_slot_left_698 = gc_frame_newslot(opresult704);
  if (strlit705 == NULL) {
    strlit705 = alloc_String("/");
    gc_root(strlit705);
  }
// compilenode returning strlit705
  int op_slot_right_698 = gc_frame_newslot(strlit705);
  params[0] = strlit705;
  partcv[0] = 1;
  Object opresult707 = callmethod(opresult704, "++", 1, partcv, params);
// compilenode returning opresult707
  int op_slot_left_697 = gc_frame_newslot(opresult707);
// compilenode returning *var_fileName
  int op_slot_right_697 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult709 = callmethod(opresult707, "++", 1, partcv, params);
// compilenode returning opresult709
  gc_frame_newslot(opresult709);
// compilenode returning *var_io
  params[0] = opresult709;
  partcv[0] = 1;
  Object call710 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe696);
// compilenode returning call710
  Object if695 = done;
  if (istrue(call710)) {
// Begin line 240
  setline(240);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 239
  setline(239);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe711 = gc_frame_new();
  int callframe712 = gc_frame_new();
  if (strlit716 == NULL) {
    strlit716 = alloc_String("");
    gc_root(strlit716);
  }
// compilenode returning strlit716
  int op_slot_left_715 = gc_frame_newslot(strlit716);
  int callframe717 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call718 = callmethod(*var_sys, "execPath",
    1, partcv, params);
  gc_frame_end(callframe717);
// compilenode returning call718
// compilenode returning call718
  int op_slot_right_715 = gc_frame_newslot(call718);
  params[0] = call718;
  partcv[0] = 1;
  Object opresult720 = callmethod(strlit716, "++", 1, partcv, params);
// compilenode returning opresult720
  int op_slot_left_714 = gc_frame_newslot(opresult720);
  if (strlit721 == NULL) {
    strlit721 = alloc_String("/");
    gc_root(strlit721);
  }
// compilenode returning strlit721
  int op_slot_right_714 = gc_frame_newslot(strlit721);
  params[0] = strlit721;
  partcv[0] = 1;
  Object opresult723 = callmethod(opresult720, "++", 1, partcv, params);
// compilenode returning opresult723
  int op_slot_left_713 = gc_frame_newslot(opresult723);
// compilenode returning *var_fileName
  int op_slot_right_713 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult725 = callmethod(opresult723, "++", 1, partcv, params);
// compilenode returning opresult725
  gc_frame_newslot(opresult725);
  if (strlit726 == NULL) {
    strlit726 = alloc_String("r");
    gc_root(strlit726);
  }
// compilenode returning strlit726
  gc_frame_newslot(strlit726);
// compilenode returning *var_io
  params[0] = opresult725;
  params[1] = strlit726;
  partcv[0] = 2;
  Object call727 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe712);
// compilenode returning call727
  partcv[0] = 0;
  Object call728 = callmethod(call727, "read",
    1, partcv, params);
  gc_frame_end(callframe711);
// compilenode returning call728
// compilenode returning call728
  return call728;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if695 = undefined;
  } else {
  }
// compilenode returning if695
// Begin line 246
  setline(246);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 241
  setline(241);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_bundlePath
  int op_slot_left_730 = gc_frame_newslot(*var_bundlePath);
  if (strlit731 == NULL) {
    strlit731 = alloc_String("");
    gc_root(strlit731);
  }
// compilenode returning strlit731
  int op_slot_right_730 = gc_frame_newslot(strlit731);
  params[0] = strlit731;
  partcv[0] = 1;
  Object opresult733 = callmethod(*var_bundlePath, "!=", 1, partcv, params);
// compilenode returning opresult733
  Object if729 = done;
  if (istrue(opresult733)) {
// Begin line 242
  setline(242);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe734 = gc_frame_new();
  int callframe735 = gc_frame_new();
  if (strlit740 == NULL) {
    strlit740 = alloc_String("");
    gc_root(strlit740);
  }
// compilenode returning strlit740
  int op_slot_left_739 = gc_frame_newslot(strlit740);
// compilenode returning *var_bundlePath
  int op_slot_right_739 = gc_frame_newslot(*var_bundlePath);
  params[0] = *var_bundlePath;
  partcv[0] = 1;
  Object opresult742 = callmethod(strlit740, "++", 1, partcv, params);
// compilenode returning opresult742
  int op_slot_left_738 = gc_frame_newslot(opresult742);
  if (strlit743 == NULL) {
    strlit743 = alloc_String("/");
    gc_root(strlit743);
  }
// compilenode returning strlit743
  int op_slot_right_738 = gc_frame_newslot(strlit743);
  params[0] = strlit743;
  partcv[0] = 1;
  Object opresult745 = callmethod(opresult742, "++", 1, partcv, params);
// compilenode returning opresult745
  int op_slot_left_737 = gc_frame_newslot(opresult745);
// compilenode returning *var_fileName
  int op_slot_right_737 = gc_frame_newslot(*var_fileName);
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object opresult747 = callmethod(opresult745, "++", 1, partcv, params);
// compilenode returning opresult747
  int op_slot_left_736 = gc_frame_newslot(opresult747);
  if (strlit748 == NULL) {
    strlit748 = alloc_String("");
    gc_root(strlit748);
  }
// compilenode returning strlit748
  int op_slot_right_736 = gc_frame_newslot(strlit748);
  params[0] = strlit748;
  partcv[0] = 1;
  Object opresult750 = callmethod(opresult747, "++", 1, partcv, params);
// compilenode returning opresult750
  gc_frame_newslot(opresult750);
  if (strlit751 == NULL) {
    strlit751 = alloc_String("r");
    gc_root(strlit751);
  }
// compilenode returning strlit751
  gc_frame_newslot(strlit751);
// compilenode returning *var_io
  params[0] = opresult750;
  params[1] = strlit751;
  partcv[0] = 2;
  Object call752 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe735);
// compilenode returning call752
  partcv[0] = 0;
  Object call753 = callmethod(call752, "read",
    1, partcv, params);
  gc_frame_end(callframe734);
// compilenode returning call753
// compilenode returning call753
  return call753;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if729 = undefined;
  } else {
  }
// compilenode returning if729
// Begin line 247
  setline(247);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 246
  setline(246);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool754 = alloc_Boolean(0);
// compilenode returning bool754
  return bool754;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_validateFile755(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 256
  setline(256);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 250
  setline(250);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe758 = gc_frame_new();
  int callframe759 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call760 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe759);
// compilenode returning call760
// compilenode returning call760
  partcv[0] = 0;
  Object call761 = callmethod(call760, "size",
    1, partcv, params);
  gc_frame_end(callframe758);
// compilenode returning call761
// compilenode returning call761
  int op_slot_left_757 = gc_frame_newslot(call761);
  Object num762 = alloc_Float64(1);
// compilenode returning num762
  int op_slot_right_757 = gc_frame_newslot(num762);
  params[0] = num762;
  partcv[0] = 1;
  Object opresult764 = callmethod(call761, ">", 1, partcv, params);
// compilenode returning opresult764
  Object if756 = done;
  if (istrue(opresult764)) {
// Begin line 255
  setline(255);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 251
  setline(251);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe767 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call768 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe767);
// compilenode returning call768
// compilenode returning call768
  Object num769 = alloc_Float64(1);
// compilenode returning num769
  params[0] = num769;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres770 = callmethod(call768, "[]", 1, partcv, params);
// compilenode returning idxres770
  int op_slot_left_766 = gc_frame_newslot(idxres770);
  if (strlit771 == NULL) {
    strlit771 = alloc_String("<");
    gc_root(strlit771);
  }
// compilenode returning strlit771
  int op_slot_right_766 = gc_frame_newslot(strlit771);
  params[0] = strlit771;
  partcv[0] = 1;
  Object opresult773 = callmethod(idxres770, "==", 1, partcv, params);
// compilenode returning opresult773
  Object if765 = done;
  if (istrue(opresult773)) {
// Begin line 252
  setline(252);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit774 == NULL) {
    strlit774 = alloc_String("Not a valid grace file");
    gc_root(strlit774);
  }
// compilenode returning strlit774
  params[0] = strlit774;
  Object call775 = gracelib_print(NULL, 1,  params);
// compilenode returning call775
// Begin line 254
  setline(254);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 253
  setline(253);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool776 = alloc_Boolean(0);
// compilenode returning bool776
  return bool776;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if765 = undefined;
  } else {
  }
// compilenode returning if765
    gc_frame_newslot(if765);
    if756 = if765;
  } else {
  }
// compilenode returning if756
// Begin line 257
  setline(257);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 256
  setline(256);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool777 = alloc_Boolean(1);
// compilenode returning bool777
  return bool777;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_write778(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 261
  setline(261);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 260
  setline(260);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit779 == NULL) {
    strlit779 = alloc_String("");
    gc_root(strlit779);
  }
// compilenode returning strlit779
  *var_usrDir = strlit779;
  if (strlit779 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 267
  setline(267);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 261
  setline(261);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_global
  Object if780 = done;
  if (istrue(*var_global)) {
// Begin line 263
  setline(263);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 262
  setline(262);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit781 == NULL) {
    strlit781 = alloc_String("usr/lib/grace/modules/");
    gc_root(strlit781);
  }
// compilenode returning strlit781
  *var_usrDir = strlit781;
  if (strlit781 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if780 = done;
  } else {
// Begin line 266
  setline(266);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 265
  setline(265);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe784 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call785 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe784);
// compilenode returning call785
// compilenode returning call785
  if (strlit786 == NULL) {
    strlit786 = alloc_String("HOME");
    gc_root(strlit786);
  }
// compilenode returning strlit786
  params[0] = strlit786;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres787 = callmethod(call785, "[]", 1, partcv, params);
// compilenode returning idxres787
  int op_slot_left_783 = gc_frame_newslot(idxres787);
  if (strlit788 == NULL) {
    strlit788 = alloc_String("/.local/lib/grace/modules/");
    gc_root(strlit788);
  }
// compilenode returning strlit788
  int op_slot_right_783 = gc_frame_newslot(strlit788);
  params[0] = strlit788;
  partcv[0] = 1;
  Object opresult790 = callmethod(idxres787, "++", 1, partcv, params);
// compilenode returning opresult790
  *var_usrDir = opresult790;
  if (opresult790 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if780 = done;
  }
// compilenode returning if780
// Begin line 268
  setline(268);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe792 = gc_frame_new();
// Begin line 267
  setline(267);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_usrDir
  int op_slot_left_793 = gc_frame_newslot(*var_usrDir);
  int callframe794 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call795 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe794);
// compilenode returning call795
// compilenode returning call795
  int op_slot_right_793 = gc_frame_newslot(call795);
  params[0] = call795;
  partcv[0] = 1;
  Object opresult797 = callmethod(*var_usrDir, "++", 1, partcv, params);
// compilenode returning opresult797
  gc_frame_newslot(opresult797);
  params[0] = opresult797;
  partcv[0] = 1;
  Object call798 = callmethodflags(self, "createDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe792);
// compilenode returning call798
  *var_fileDir = call798;
  if (call798 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 269
  setline(269);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe799 = gc_frame_new();
// Begin line 268
  setline(268);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_fileDir
  Object num801 = alloc_Float64(1);
// compilenode returning num801
  params[0] = num801;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres802 = callmethod(*var_fileDir, "[]", 1, partcv, params);
// compilenode returning idxres802
  int op_slot_left_800 = gc_frame_newslot(idxres802);
// compilenode returning *var_fileDir
  Object num803 = alloc_Float64(2);
// compilenode returning num803
  params[0] = num803;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres804 = callmethod(*var_fileDir, "[]", 1, partcv, params);
// compilenode returning idxres804
  int op_slot_right_800 = gc_frame_newslot(idxres804);
  params[0] = idxres804;
  partcv[0] = 1;
  Object opresult806 = callmethod(idxres802, "++", 1, partcv, params);
// compilenode returning opresult806
  gc_frame_newslot(opresult806);
  if (strlit807 == NULL) {
    strlit807 = alloc_String("w");
    gc_root(strlit807);
  }
// compilenode returning strlit807
  gc_frame_newslot(strlit807);
// compilenode returning *var_io
  params[0] = opresult806;
  params[1] = strlit807;
  partcv[0] = 2;
  Object call808 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe799);
// compilenode returning call808
  *var_toWrite = call808;
  if (call808 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 269
  setline(269);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe809 = gc_frame_new();
  int callframe810 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call811 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe810);
// compilenode returning call811
// compilenode returning call811
  gc_frame_newslot(call811);
// compilenode returning *var_toWrite
  params[0] = call811;
  partcv[0] = 1;
  Object call812 = callmethod(*var_toWrite, "write",
    1, partcv, params);
  gc_frame_end(callframe809);
// compilenode returning call812
// Begin line 270
  setline(270);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe813 = gc_frame_new();
// compilenode returning *var_toWrite
  partcv[0] = 0;
  Object call814 = callmethod(*var_toWrite, "close",
    1, partcv, params);
  gc_frame_end(callframe813);
// compilenode returning call814
// compilenode returning call814
  gc_frame_end(frame);
  return call814;
}
Object meth_PackageManager__apply829(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 287
  setline(287);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 278
  setline(278);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe832 = gc_frame_new();
// compilenode returning *var_t
  gc_frame_newslot(*var_t);
// compilenode returning *var_paths
  params[0] = *var_t;
  partcv[0] = 1;
  Object call833 = callmethod(*var_paths, "at",
    1, partcv, params);
  gc_frame_end(callframe832);
// compilenode returning call833
  int op_slot_left_831 = gc_frame_newslot(call833);
  if (strlit834 == NULL) {
    strlit834 = alloc_String(":");
    gc_root(strlit834);
  }
// compilenode returning strlit834
  int op_slot_right_831 = gc_frame_newslot(strlit834);
  params[0] = strlit834;
  partcv[0] = 1;
  Object opresult836 = callmethod(call833, "==", 1, partcv, params);
// compilenode returning opresult836
  Object if830 = done;
  if (istrue(opresult836)) {
// Begin line 282
  setline(282);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 279
  setline(279);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe838 = gc_frame_new();
// compilenode returning *var_buildPath
  int op_slot_left_839 = gc_frame_newslot(*var_buildPath);
  if (strlit840 == NULL) {
    strlit840 = alloc_String("/minigrace");
    gc_root(strlit840);
  }
// compilenode returning strlit840
  int op_slot_right_839 = gc_frame_newslot(strlit840);
  params[0] = strlit840;
  partcv[0] = 1;
  Object opresult842 = callmethod(*var_buildPath, "++", 1, partcv, params);
// compilenode returning opresult842
  gc_frame_newslot(opresult842);
// compilenode returning *var_io
  params[0] = opresult842;
  partcv[0] = 1;
  Object call843 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe838);
// compilenode returning call843
  Object if837 = done;
  if (istrue(call843)) {
// Begin line 280
  setline(280);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  block_return(realself, *var_buildPath);
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if837 = undefined;
  } else {
  }
// compilenode returning if837
// Begin line 283
  setline(283);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 282
  setline(282);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit844 == NULL) {
    strlit844 = alloc_String("");
    gc_root(strlit844);
  }
// compilenode returning strlit844
  *var_buildPath = strlit844;
  if (strlit844 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if830 = done;
  } else {
// Begin line 286
  setline(286);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 285
  setline(285);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  int op_slot_left_846 = gc_frame_newslot(*var_buildPath);
// Begin line 286
  setline(286);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 285
  setline(285);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_paths
// compilenode returning *var_t
  params[0] = *var_t;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres847 = callmethod(*var_paths, "[]", 1, partcv, params);
// compilenode returning idxres847
  int op_slot_right_846 = gc_frame_newslot(idxres847);
  params[0] = idxres847;
  partcv[0] = 1;
  Object opresult849 = callmethod(*var_buildPath, "++", 1, partcv, params);
// compilenode returning opresult849
  *var_buildPath = opresult849;
  if (opresult849 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if830 = done;
  }
// compilenode returning if830
  gc_frame_end(frame);
  return if830;
}
Object meth_PackageManager_getBuildPath815(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 276
  setline(276);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 275
  setline(275);
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
    strlit818 = alloc_String("PATH");
    gc_root(strlit818);
  }
// compilenode returning strlit818
  params[0] = strlit818;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres819 = callmethod(call817, "[]", 1, partcv, params);
// compilenode returning idxres819
  *var_paths = idxres819;
  if (idxres819 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 277
  setline(277);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 276
  setline(276);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit820 == NULL) {
    strlit820 = alloc_String("");
    gc_root(strlit820);
  }
// compilenode returning strlit820
  *var_buildPath = strlit820;
  if (strlit820 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 288
  setline(288);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe821 = gc_frame_new();
// Begin line 277
  setline(277);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num823 = alloc_Float64(1);
// compilenode returning num823
  int op_slot_left_822 = gc_frame_newslot(num823);
  int callframe824 = gc_frame_new();
// compilenode returning *var_paths
  partcv[0] = 0;
  Object call825 = callmethod(*var_paths, "size",
    1, partcv, params);
  gc_frame_end(callframe824);
// compilenode returning call825
// compilenode returning call825
  int op_slot_right_822 = gc_frame_newslot(call825);
  params[0] = call825;
  partcv[0] = 1;
  Object opresult827 = callmethod(num823, "..", 1, partcv, params);
// compilenode returning opresult827
  gc_frame_newslot(opresult827);
// Begin line 288
  setline(288);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block828 = alloc_Block(NULL, NULL, "PackageManager", 288);
  gc_frame_newslot(block828);
  block_savedest(block828);
  Object closure829 = createclosure(4, "_apply");
setclosureframe(closure829, stackframe);
  addtoclosure(closure829, var_paths);
  addtoclosure(closure829, var_buildPath);
  addtoclosure(closure829, var_io);
  addtoclosure(closure829, selfslot);
  struct UserObject *uo829 = (struct UserObject*)block828;
  uo829->data[0] = (Object)closure829;
  Method *meth_meth_PackageManager__apply829 = addmethod2pos(block828, "_apply", &meth_PackageManager__apply829, 0);
int argcv_meth_PackageManager__apply829[] = {1};
meth_meth_PackageManager__apply829->type = alloc_MethodType(1, argcv_meth_PackageManager__apply829);
  meth_meth_PackageManager__apply829->definitionModule = modulename;
  meth_meth_PackageManager__apply829->definitionLine = 277;
// compilenode returning block828
  gc_frame_newslot(block828);
  params[0] = opresult827;
  params[1] = block828;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call852 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe821);
// compilenode returning call852
// Begin line 288
  setline(288);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  return *var_buildPath;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_compile853(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 293
  setline(293);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 292
  setline(292);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit854 == NULL) {
    strlit854 = alloc_String("");
    gc_root(strlit854);
  }
// compilenode returning strlit854
  *var_usrDir = strlit854;
  if (strlit854 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 299
  setline(299);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 293
  setline(293);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_global
  Object if855 = done;
  if (istrue(*var_global)) {
// Begin line 295
  setline(295);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 294
  setline(294);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit856 == NULL) {
    strlit856 = alloc_String("usr/lib/grace/modules/");
    gc_root(strlit856);
  }
// compilenode returning strlit856
  *var_usrDir = strlit856;
  if (strlit856 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if855 = done;
  } else {
// Begin line 298
  setline(298);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 297
  setline(297);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe859 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call860 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe859);
// compilenode returning call860
// compilenode returning call860
  if (strlit861 == NULL) {
    strlit861 = alloc_String("HOME");
    gc_root(strlit861);
  }
// compilenode returning strlit861
  params[0] = strlit861;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres862 = callmethod(call860, "[]", 1, partcv, params);
// compilenode returning idxres862
  int op_slot_left_858 = gc_frame_newslot(idxres862);
  if (strlit863 == NULL) {
    strlit863 = alloc_String("/.local/lib/grace/modules/");
    gc_root(strlit863);
  }
// compilenode returning strlit863
  int op_slot_right_858 = gc_frame_newslot(strlit863);
  params[0] = strlit863;
  partcv[0] = 1;
  Object opresult865 = callmethod(idxres862, "++", 1, partcv, params);
// compilenode returning opresult865
  *var_usrDir = opresult865;
  if (opresult865 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if855 = done;
  }
// compilenode returning if855
// Begin line 300
  setline(300);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 299
  setline(299);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe867 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call868 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe867);
// compilenode returning call868
// compilenode returning call868
  if (strlit869 == NULL) {
    strlit869 = alloc_String("PATH");
    gc_root(strlit869);
  }
// compilenode returning strlit869
  params[0] = strlit869;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres870 = callmethod(call868, "[]", 1, partcv, params);
// compilenode returning idxres870
  *var_paths = idxres870;
  if (idxres870 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 301
  setline(301);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 300
  setline(300);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit872 == NULL) {
    strlit872 = alloc_String("PATHS = ");
    gc_root(strlit872);
  }
// compilenode returning strlit872
  int op_slot_left_871 = gc_frame_newslot(strlit872);
// compilenode returning *var_paths
  int op_slot_right_871 = gc_frame_newslot(*var_paths);
  params[0] = *var_paths;
  partcv[0] = 1;
  Object opresult874 = callmethod(strlit872, "++", 1, partcv, params);
// compilenode returning opresult874
  params[0] = opresult874;
  Object call875 = gracelib_print(NULL, 1,  params);
// compilenode returning call875
// Begin line 301
  setline(301);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe876 = gc_frame_new();
  partcv[0] = 0;
  Object call877 = callmethodflags(self, "getBuildPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe876);
// compilenode returning call877
  *var_buildPath = call877;
  if (call877 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 303
  setline(303);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe878 = gc_frame_new();
// Begin line 302
  setline(302);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  int op_slot_left_881 = gc_frame_newslot(*var_buildPath);
  if (strlit882 == NULL) {
    strlit882 = alloc_String("/minigrace ");
    gc_root(strlit882);
  }
// compilenode returning strlit882
  int op_slot_right_881 = gc_frame_newslot(strlit882);
  params[0] = strlit882;
  partcv[0] = 1;
  Object opresult884 = callmethod(*var_buildPath, "++", 1, partcv, params);
// compilenode returning opresult884
  int op_slot_left_880 = gc_frame_newslot(opresult884);
// compilenode returning *var_usrDir
  int op_slot_right_880 = gc_frame_newslot(*var_usrDir);
  params[0] = *var_usrDir;
  partcv[0] = 1;
  Object opresult886 = callmethod(opresult884, "++", 1, partcv, params);
// compilenode returning opresult886
  int op_slot_left_879 = gc_frame_newslot(opresult886);
  int callframe887 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call888 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe887);
// compilenode returning call888
// compilenode returning call888
  int op_slot_right_879 = gc_frame_newslot(call888);
  params[0] = call888;
  partcv[0] = 1;
  Object opresult890 = callmethod(opresult886, "++", 1, partcv, params);
// compilenode returning opresult890
  gc_frame_newslot(opresult890);
// compilenode returning *var_io
  params[0] = opresult890;
  partcv[0] = 1;
  Object call891 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe878);
// compilenode returning call891
// Begin line 303
  setline(303);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool892 = alloc_Boolean(1);
// compilenode returning bool892
  return bool892;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply909(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_910 = gc_frame_newslot(*var_count);
  int callframe911 = gc_frame_new();
// compilenode returning *var_address
  partcv[0] = 0;
  Object call912 = callmethod(*var_address, "size",
    1, partcv, params);
  gc_frame_end(callframe911);
// compilenode returning call912
// compilenode returning call912
  int op_slot_right_910 = gc_frame_newslot(call912);
  params[0] = call912;
  partcv[0] = 1;
  Object opresult914 = callmethod(*var_count, "<=", 1, partcv, params);
// compilenode returning opresult914
  gc_frame_end(frame);
  return opresult914;
}
Object meth_PackageManager__apply917(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 315
  setline(315);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 314
  setline(314);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_nextPath
  int op_slot_left_918 = gc_frame_newslot(*var_nextPath);
// Begin line 315
  setline(315);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 314
  setline(314);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_address
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres919 = callmethod(*var_address, "[]", 1, partcv, params);
// compilenode returning idxres919
  int op_slot_right_918 = gc_frame_newslot(idxres919);
  params[0] = idxres919;
  partcv[0] = 1;
  Object opresult921 = callmethod(*var_nextPath, "++", 1, partcv, params);
// compilenode returning opresult921
  *var_nextPath = opresult921;
  if (opresult921 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 319
  setline(319);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 315
  setline(315);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_address
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres925 = callmethod(*var_address, "[]", 1, partcv, params);
// compilenode returning idxres925
  int op_slot_left_924 = gc_frame_newslot(idxres925);
  if (strlit926 == NULL) {
    strlit926 = alloc_String("/");
    gc_root(strlit926);
  }
// compilenode returning strlit926
  int op_slot_right_924 = gc_frame_newslot(strlit926);
  params[0] = strlit926;
  partcv[0] = 1;
  Object opresult928 = callmethod(idxres925, "==", 1, partcv, params);
// compilenode returning opresult928
  Object if923 = done;
  if (istrue(opresult928)) {
// Begin line 317
  setline(317);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 316
  setline(316);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toMake
  int op_slot_left_929 = gc_frame_newslot(*var_toMake);
// compilenode returning *var_nextPath
  int op_slot_right_929 = gc_frame_newslot(*var_nextPath);
  params[0] = *var_nextPath;
  partcv[0] = 1;
  Object opresult931 = callmethod(*var_toMake, "++", 1, partcv, params);
// compilenode returning opresult931
  *var_toMake = opresult931;
  if (opresult931 == undefined)
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
  if (strlit933 == NULL) {
    strlit933 = alloc_String("");
    gc_root(strlit933);
  }
// compilenode returning strlit933
  *var_nextPath = strlit933;
  if (strlit933 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if923 = done;
  } else {
  }
// compilenode returning if923
// Begin line 319
  setline(319);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  int op_slot_left_935 = gc_frame_newslot(*var_count);
  Object num936 = alloc_Float64(1);
// compilenode returning num936
  int op_slot_right_935 = gc_frame_newslot(num936);
  params[0] = num936;
  partcv[0] = 1;
  Object sum938 = callmethod(*var_count, "+", 1, partcv, params);
// compilenode returning sum938
  *var_count = sum938;
  if (sum938 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_createDirectory893(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 309
  setline(309);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 308
  setline(308);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array894 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array894
  *var_dir = array894;
  if (array894 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 310
  setline(310);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 309
  setline(309);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit895 == NULL) {
    strlit895 = alloc_String("");
    gc_root(strlit895);
  }
// compilenode returning strlit895
  *var_nextPath = strlit895;
  if (strlit895 == undefined)
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
  if (strlit896 == NULL) {
    strlit896 = alloc_String("");
    gc_root(strlit896);
  }
// compilenode returning strlit896
  *var_toMake = strlit896;
  if (strlit896 == undefined)
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
  Object num897 = alloc_Float64(1);
// compilenode returning num897
  *var_count = num897;
  if (num897 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 313
  setline(313);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 312
  setline(312);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit900 == NULL) {
    strlit900 = alloc_String("Directory being evaluated ");
    gc_root(strlit900);
  }
// compilenode returning strlit900
  int op_slot_left_899 = gc_frame_newslot(strlit900);
// compilenode returning *var_address
  int op_slot_right_899 = gc_frame_newslot(*var_address);
  params[0] = *var_address;
  partcv[0] = 1;
  Object opresult902 = callmethod(strlit900, "++", 1, partcv, params);
// compilenode returning opresult902
  int op_slot_left_898 = gc_frame_newslot(opresult902);
  if (strlit903 == NULL) {
    strlit903 = alloc_String("");
    gc_root(strlit903);
  }
// compilenode returning strlit903
  int op_slot_right_898 = gc_frame_newslot(strlit903);
  params[0] = strlit903;
  partcv[0] = 1;
  Object opresult905 = callmethod(opresult902, "++", 1, partcv, params);
// compilenode returning opresult905
  params[0] = opresult905;
  Object call906 = gracelib_print(NULL, 1,  params);
// compilenode returning call906
// Begin line 321
  setline(321);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe907 = gc_frame_new();
// Begin line 313
  setline(313);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block908 = alloc_Block(NULL, NULL, "PackageManager", 313);
  gc_frame_newslot(block908);
  block_savedest(block908);
  Object closure909 = createclosure(3, "_apply");
setclosureframe(closure909, stackframe);
  addtoclosure(closure909, var_count);
  addtoclosure(closure909, var_address);
  addtoclosure(closure909, selfslot);
  struct UserObject *uo909 = (struct UserObject*)block908;
  uo909->data[0] = (Object)closure909;
  Method *meth_meth_PackageManager__apply909 = addmethod2pos(block908, "_apply", &meth_PackageManager__apply909, 0);
int argcv_meth_PackageManager__apply909[] = {0};
meth_meth_PackageManager__apply909->type = alloc_MethodType(1, argcv_meth_PackageManager__apply909);
  meth_meth_PackageManager__apply909->definitionModule = modulename;
  meth_meth_PackageManager__apply909->definitionLine = 302;
// compilenode returning block908
  gc_frame_newslot(block908);
// Begin line 321
  setline(321);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block916 = alloc_Block(NULL, NULL, "PackageManager", 321);
  gc_frame_newslot(block916);
  block_savedest(block916);
  Object closure917 = createclosure(5, "_apply");
setclosureframe(closure917, stackframe);
  addtoclosure(closure917, var_nextPath);
  addtoclosure(closure917, var_address);
  addtoclosure(closure917, var_count);
  addtoclosure(closure917, var_toMake);
  addtoclosure(closure917, selfslot);
  struct UserObject *uo917 = (struct UserObject*)block916;
  uo917->data[0] = (Object)closure917;
  Method *meth_meth_PackageManager__apply917 = addmethod2pos(block916, "_apply", &meth_PackageManager__apply917, 0);
int argcv_meth_PackageManager__apply917[] = {0};
meth_meth_PackageManager__apply917->type = alloc_MethodType(1, argcv_meth_PackageManager__apply917);
  meth_meth_PackageManager__apply917->definitionModule = modulename;
  meth_meth_PackageManager__apply917->definitionLine = 313;
// compilenode returning block916
  gc_frame_newslot(block916);
  params[0] = block908;
  params[1] = block916;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call941 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe907);
// compilenode returning call941
// Begin line 321
  setline(321);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit943 == NULL) {
    strlit943 = alloc_String("Creating directory ");
    gc_root(strlit943);
  }
// compilenode returning strlit943
  int op_slot_left_942 = gc_frame_newslot(strlit943);
// compilenode returning *var_toMake
  int op_slot_right_942 = gc_frame_newslot(*var_toMake);
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object opresult945 = callmethod(strlit943, "++", 1, partcv, params);
// compilenode returning opresult945
  params[0] = opresult945;
  Object call946 = gracelib_print(NULL, 1,  params);
// compilenode returning call946
// Begin line 326
  setline(326);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 322
  setline(322);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe948 = gc_frame_new();
  int callframe949 = gc_frame_new();
// compilenode returning *var_toMake
  gc_frame_newslot(*var_toMake);
// compilenode returning *var_io
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object call950 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe949);
// compilenode returning call950
  partcv[0] = 0;
  Object call951 = callmethod(call950, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe948);
// compilenode returning call951
  Object if947 = done;
  if (istrue(call951)) {
// Begin line 324
  setline(324);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe952 = gc_frame_new();
// Begin line 323
  setline(323);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit954 == NULL) {
    strlit954 = alloc_String("mkdir -p ");
    gc_root(strlit954);
  }
// compilenode returning strlit954
  int op_slot_left_953 = gc_frame_newslot(strlit954);
// compilenode returning *var_toMake
  int op_slot_right_953 = gc_frame_newslot(*var_toMake);
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object opresult956 = callmethod(strlit954, "++", 1, partcv, params);
// compilenode returning opresult956
  gc_frame_newslot(opresult956);
// compilenode returning *var_io
  params[0] = opresult956;
  partcv[0] = 1;
  Object call957 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe952);
// compilenode returning call957
// Begin line 325
  setline(325);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 324
  setline(324);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit960 == NULL) {
    strlit960 = alloc_String("made directory ");
    gc_root(strlit960);
  }
// compilenode returning strlit960
  int op_slot_left_959 = gc_frame_newslot(strlit960);
// compilenode returning *var_toMake
  int op_slot_right_959 = gc_frame_newslot(*var_toMake);
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object opresult962 = callmethod(strlit960, "++", 1, partcv, params);
// compilenode returning opresult962
  int op_slot_left_958 = gc_frame_newslot(opresult962);
  if (strlit963 == NULL) {
    strlit963 = alloc_String("");
    gc_root(strlit963);
  }
// compilenode returning strlit963
  int op_slot_right_958 = gc_frame_newslot(strlit963);
  params[0] = strlit963;
  partcv[0] = 1;
  Object opresult965 = callmethod(opresult962, "++", 1, partcv, params);
// compilenode returning opresult965
  params[0] = opresult965;
  Object call966 = gracelib_print(NULL, 1,  params);
// compilenode returning call966
    gc_frame_newslot(call966);
    if947 = call966;
  } else {
  }
// compilenode returning if947
// Begin line 327
  setline(327);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe967 = gc_frame_new();
// Begin line 326
  setline(326);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toMake
  gc_frame_newslot(*var_toMake);
// compilenode returning *var_dir
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object call968 = callmethod(*var_dir, "push",
    1, partcv, params);
  gc_frame_end(callframe967);
// compilenode returning call968
// Begin line 328
  setline(328);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe969 = gc_frame_new();
// Begin line 327
  setline(327);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_nextPath
  gc_frame_newslot(*var_nextPath);
// compilenode returning *var_dir
  params[0] = *var_nextPath;
  partcv[0] = 1;
  Object call970 = callmethod(*var_dir, "push",
    1, partcv, params);
  gc_frame_end(callframe969);
// compilenode returning call970
// Begin line 329
  setline(329);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 328
  setline(328);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_dir
  return *var_dir;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply977(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_978 = gc_frame_newslot(*var_curPos);
  int callframe979 = gc_frame_new();
// compilenode returning *var_data
  partcv[0] = 0;
  Object call980 = callmethod(*var_data, "size",
    1, partcv, params);
  gc_frame_end(callframe979);
// compilenode returning call980
// compilenode returning call980
  int op_slot_right_978 = gc_frame_newslot(call980);
  params[0] = call980;
  partcv[0] = 1;
  Object opresult982 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult982
  gc_frame_end(frame);
  return opresult982;
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
  int op_slot_left_991 = gc_frame_newslot(*var_curPos);
  int callframe992 = gc_frame_new();
// compilenode returning *var_data
  partcv[0] = 0;
  Object call993 = callmethod(*var_data, "size",
    1, partcv, params);
  gc_frame_end(callframe992);
// compilenode returning call993
// compilenode returning call993
  int op_slot_right_991 = gc_frame_newslot(call993);
  params[0] = call993;
  partcv[0] = 1;
  Object opresult995 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult995
  int op_slot_left_990 = gc_frame_newslot(opresult995);
// compilenode returning *var_data
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres997 = callmethod(*var_data, "[]", 1, partcv, params);
// compilenode returning idxres997
  int op_slot_left_996 = gc_frame_newslot(idxres997);
  if (strlit998 == NULL) {
    strlit998 = alloc_String("""\x0a""");
    gc_root(strlit998);
  }
// compilenode returning strlit998
  int op_slot_right_996 = gc_frame_newslot(strlit998);
  params[0] = strlit998;
  partcv[0] = 1;
  Object opresult1000 = callmethod(idxres997, "!=", 1, partcv, params);
// compilenode returning opresult1000
  int op_slot_right_990 = gc_frame_newslot(opresult1000);
  params[0] = opresult1000;
  partcv[0] = 1;
  Object opresult1002 = callmethod(opresult995, "&&", 1, partcv, params);
// compilenode returning opresult1002
  gc_frame_end(frame);
  return opresult1002;
}
Object meth_PackageManager__apply1005(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 338
  setline(338);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1006 = gc_frame_newslot(*var_curPos);
  Object num1007 = alloc_Float64(1);
// compilenode returning num1007
  int op_slot_right_1006 = gc_frame_newslot(num1007);
  params[0] = num1007;
  partcv[0] = 1;
  Object sum1009 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1009
  *var_curPos = sum1009;
  if (sum1009 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply985(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 337
  setline(337);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 336
  setline(336);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 340
  setline(340);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe987 = gc_frame_new();
// Begin line 337
  setline(337);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block988 = alloc_Block(NULL, NULL, "PackageManager", 337);
  gc_frame_newslot(block988);
  block_savedest(block988);
  Object closure989 = createclosure(3, "_apply");
setclosureframe(closure989, stackframe);
  addtoclosure(closure989, var_curPos);
  addtoclosure(closure989, var_data);
  addtoclosure(closure989, selfslot);
  struct UserObject *uo989 = (struct UserObject*)block988;
  uo989->data[0] = (Object)closure989;
  Method *meth_meth_PackageManager__apply989 = addmethod2pos(block988, "_apply", &meth_PackageManager__apply989, 0);
int argcv_meth_PackageManager__apply989[] = {0};
meth_meth_PackageManager__apply989->type = alloc_MethodType(1, argcv_meth_PackageManager__apply989);
  meth_meth_PackageManager__apply989->definitionModule = modulename;
  meth_meth_PackageManager__apply989->definitionLine = 335;
// compilenode returning block988
  gc_frame_newslot(block988);
// Begin line 340
  setline(340);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1004 = alloc_Block(NULL, NULL, "PackageManager", 340);
  gc_frame_newslot(block1004);
  block_savedest(block1004);
  Object closure1005 = createclosure(2, "_apply");
setclosureframe(closure1005, stackframe);
  addtoclosure(closure1005, var_curPos);
  addtoclosure(closure1005, selfslot);
  struct UserObject *uo1005 = (struct UserObject*)block1004;
  uo1005->data[0] = (Object)closure1005;
  Method *meth_meth_PackageManager__apply1005 = addmethod2pos(block1004, "_apply", &meth_PackageManager__apply1005, 0);
int argcv_meth_PackageManager__apply1005[] = {0};
meth_meth_PackageManager__apply1005->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1005);
  meth_meth_PackageManager__apply1005->definitionModule = modulename;
  meth_meth_PackageManager__apply1005->definitionLine = 337;
// compilenode returning block1004
  gc_frame_newslot(block1004);
  params[0] = block988;
  params[1] = block1004;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1012 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe987);
// compilenode returning call1012
// Begin line 341
  setline(341);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1013 = gc_frame_new();
// Begin line 340
  setline(340);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_startPos
  gc_frame_newslot(*var_startPos);
// compilenode returning *var_curPos
  int op_slot_left_1014 = gc_frame_newslot(*var_curPos);
  Object num1015 = alloc_Float64(1);
// compilenode returning num1015
  int op_slot_right_1014 = gc_frame_newslot(num1015);
  params[0] = num1015;
  partcv[0] = 1;
  Object diff1017 = callmethod(*var_curPos, "-", 1, partcv, params);
// compilenode returning diff1017
  gc_frame_newslot(diff1017);
// compilenode returning *var_data
  params[0] = *var_startPos;
  params[1] = diff1017;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1018 = callmethod(*var_data, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1013);
// compilenode returning call1018
  *var_line = call1018;
  if (call1018 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 344
  setline(344);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 341
  setline(341);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1020 = gc_frame_new();
  int callframe1021 = gc_frame_new();
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
  params[0] = *var_line;
  partcv[0] = 1;
  Object call1022 = callmethodflags(self, "processLine", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1021);
// compilenode returning call1022
  partcv[0] = 0;
  Object call1023 = callmethod(call1022, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1020);
// compilenode returning call1023
  Object if1019 = done;
  if (istrue(call1023)) {
// Begin line 343
  setline(343);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_done
  block_return(realself, *var_done);
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1019 = undefined;
  } else {
  }
// compilenode returning if1019
// Begin line 345
  setline(345);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 344
  setline(344);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1024 = gc_frame_newslot(*var_curPos);
  Object num1025 = alloc_Float64(1);
// compilenode returning num1025
  int op_slot_right_1024 = gc_frame_newslot(num1025);
  params[0] = num1025;
  partcv[0] = 1;
  Object sum1027 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1027
  *var_curPos = sum1027;
  if (sum1027 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_parseFile971(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 333
  setline(333);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 332
  setline(332);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe972 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call973 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe972);
// compilenode returning call973
// compilenode returning call973
  *var_data = call973;
  if (call973 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 333
  setline(333);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num974 = alloc_Float64(1);
// compilenode returning num974
  *var_curPos = num974;
  if (num974 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 335
  setline(335);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 334
  setline(334);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 346
  setline(346);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe975 = gc_frame_new();
// Begin line 335
  setline(335);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block976 = alloc_Block(NULL, NULL, "PackageManager", 335);
  gc_frame_newslot(block976);
  block_savedest(block976);
  Object closure977 = createclosure(3, "_apply");
setclosureframe(closure977, stackframe);
  addtoclosure(closure977, var_curPos);
  addtoclosure(closure977, var_data);
  addtoclosure(closure977, selfslot);
  struct UserObject *uo977 = (struct UserObject*)block976;
  uo977->data[0] = (Object)closure977;
  Method *meth_meth_PackageManager__apply977 = addmethod2pos(block976, "_apply", &meth_PackageManager__apply977, 0);
int argcv_meth_PackageManager__apply977[] = {0};
meth_meth_PackageManager__apply977->type = alloc_MethodType(1, argcv_meth_PackageManager__apply977);
  meth_meth_PackageManager__apply977->definitionModule = modulename;
  meth_meth_PackageManager__apply977->definitionLine = 332;
// compilenode returning block976
  gc_frame_newslot(block976);
// Begin line 346
  setline(346);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block984 = alloc_Block(NULL, NULL, "PackageManager", 346);
  gc_frame_newslot(block984);
  block_savedest(block984);
  Object closure985 = createclosure(5, "_apply");
setclosureframe(closure985, stackframe);
  addtoclosure(closure985, var_curPos);
  addtoclosure(closure985, var_startPos);
  addtoclosure(closure985, var_data);
  addtoclosure(closure985, var_done);
  addtoclosure(closure985, selfslot);
  struct UserObject *uo985 = (struct UserObject*)block984;
  uo985->data[0] = (Object)closure985;
  Method *meth_meth_PackageManager__apply985 = addmethod2pos(block984, "_apply", &meth_PackageManager__apply985, 0);
int argcv_meth_PackageManager__apply985[] = {0};
meth_meth_PackageManager__apply985->type = alloc_MethodType(1, argcv_meth_PackageManager__apply985);
  meth_meth_PackageManager__apply985->definitionModule = modulename;
  meth_meth_PackageManager__apply985->definitionLine = 335;
// compilenode returning block984
  gc_frame_newslot(block984);
  params[0] = block976;
  params[1] = block984;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1030 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe975);
// compilenode returning call1030
  gc_frame_end(frame);
  return call1030;
}
Object meth_PackageManager_processLine1031(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 364
  setline(364);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 349
  setline(349);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1034 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1035 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1034);
// compilenode returning call1035
// compilenode returning call1035
  int op_slot_left_1033 = gc_frame_newslot(call1035);
  Object num1036 = alloc_Float64(1);
// compilenode returning num1036
  int op_slot_right_1033 = gc_frame_newslot(num1036);
  params[0] = num1036;
  partcv[0] = 1;
  Object opresult1038 = callmethod(call1035, ">", 1, partcv, params);
// compilenode returning opresult1038
  Object if1032 = done;
  if (istrue(opresult1038)) {
// Begin line 363
  setline(363);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 350
  setline(350);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  Object num1041 = alloc_Float64(1);
// compilenode returning num1041
  params[0] = num1041;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1042 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1042
  int op_slot_left_1040 = gc_frame_newslot(idxres1042);
  if (strlit1043 == NULL) {
    strlit1043 = alloc_String("#");
    gc_root(strlit1043);
  }
// compilenode returning strlit1043
  int op_slot_right_1040 = gc_frame_newslot(strlit1043);
  params[0] = strlit1043;
  partcv[0] = 1;
  Object opresult1045 = callmethod(idxres1042, "==", 1, partcv, params);
// compilenode returning opresult1045
  Object if1039 = done;
  if (istrue(opresult1045)) {
// Begin line 352
  setline(352);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 351
  setline(351);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1046 = alloc_Boolean(1);
// compilenode returning bool1046
  return bool1046;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1039 = undefined;
  } else {
// Begin line 356
  setline(356);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 353
  setline(353);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1050 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1051 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1050);
// compilenode returning call1051
// compilenode returning call1051
  int op_slot_left_1049 = gc_frame_newslot(call1051);
  Object num1052 = alloc_Float64(2);
// compilenode returning num1052
  int op_slot_right_1049 = gc_frame_newslot(num1052);
  params[0] = num1052;
  partcv[0] = 1;
  Object opresult1054 = callmethod(call1051, ">", 1, partcv, params);
// compilenode returning opresult1054
  int op_slot_left_1048 = gc_frame_newslot(opresult1054);
  int callframe1056 = gc_frame_new();
  Object num1057 = alloc_Float64(1);
// compilenode returning num1057
  gc_frame_newslot(num1057);
  Object num1058 = alloc_Float64(2);
// compilenode returning num1058
  gc_frame_newslot(num1058);
// compilenode returning *var_line
  params[0] = num1057;
  params[1] = num1058;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1059 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1056);
// compilenode returning call1059
  int op_slot_left_1055 = gc_frame_newslot(call1059);
  if (strlit1060 == NULL) {
    strlit1060 = alloc_String("//");
    gc_root(strlit1060);
  }
// compilenode returning strlit1060
  int op_slot_right_1055 = gc_frame_newslot(strlit1060);
  params[0] = strlit1060;
  partcv[0] = 1;
  Object opresult1062 = callmethod(call1059, "==", 1, partcv, params);
// compilenode returning opresult1062
  int op_slot_right_1048 = gc_frame_newslot(opresult1062);
  params[0] = opresult1062;
  partcv[0] = 1;
  Object opresult1064 = callmethod(opresult1054, "&&", 1, partcv, params);
// compilenode returning opresult1064
  Object if1047 = done;
  if (istrue(opresult1064)) {
// Begin line 355
  setline(355);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 354
  setline(354);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1065 = alloc_Boolean(1);
// compilenode returning bool1065
  return bool1065;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1047 = undefined;
  } else {
// Begin line 360
  setline(360);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 356
  setline(356);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1069 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1070 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1069);
// compilenode returning call1070
// compilenode returning call1070
  int op_slot_left_1068 = gc_frame_newslot(call1070);
  Object num1071 = alloc_Float64(6);
// compilenode returning num1071
  int op_slot_right_1068 = gc_frame_newslot(num1071);
  params[0] = num1071;
  partcv[0] = 1;
  Object opresult1073 = callmethod(call1070, ">", 1, partcv, params);
// compilenode returning opresult1073
  int op_slot_left_1067 = gc_frame_newslot(opresult1073);
  int callframe1075 = gc_frame_new();
  Object num1076 = alloc_Float64(1);
// compilenode returning num1076
  gc_frame_newslot(num1076);
  Object num1077 = alloc_Float64(7);
// compilenode returning num1077
  gc_frame_newslot(num1077);
// compilenode returning *var_line
  params[0] = num1076;
  params[1] = num1077;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1078 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1075);
// compilenode returning call1078
  int op_slot_left_1074 = gc_frame_newslot(call1078);
  if (strlit1079 == NULL) {
    strlit1079 = alloc_String("import ");
    gc_root(strlit1079);
  }
// compilenode returning strlit1079
  int op_slot_right_1074 = gc_frame_newslot(strlit1079);
  params[0] = strlit1079;
  partcv[0] = 1;
  Object opresult1081 = callmethod(call1078, "==", 1, partcv, params);
// compilenode returning opresult1081
  int op_slot_right_1067 = gc_frame_newslot(opresult1081);
  params[0] = opresult1081;
  partcv[0] = 1;
  Object opresult1083 = callmethod(opresult1073, "&&", 1, partcv, params);
// compilenode returning opresult1083
  Object if1066 = done;
  if (istrue(opresult1083)) {
// Begin line 357
  setline(357);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1084 = gc_frame_new();
  int callframe1085 = gc_frame_new();
  Object num1086 = alloc_Float64(8);
// compilenode returning num1086
  gc_frame_newslot(num1086);
  int callframe1087 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1088 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1087);
// compilenode returning call1088
// compilenode returning call1088
  gc_frame_newslot(call1088);
// compilenode returning *var_line
  params[0] = num1086;
  params[1] = call1088;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1089 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1085);
// compilenode returning call1089
  gc_frame_newslot(call1089);
  params[0] = call1089;
  partcv[0] = 1;
  Object call1090 = callmethodflags(self, "parseImport", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1084);
// compilenode returning call1090
// Begin line 359
  setline(359);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 358
  setline(358);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1091 = alloc_Boolean(1);
// compilenode returning bool1091
  return bool1091;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1066 = undefined;
  } else {
// Begin line 363
  setline(363);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 360
  setline(360);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1095 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1096 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1095);
// compilenode returning call1096
// compilenode returning call1096
  int op_slot_left_1094 = gc_frame_newslot(call1096);
  Object num1097 = alloc_Float64(7);
// compilenode returning num1097
  int op_slot_right_1094 = gc_frame_newslot(num1097);
  params[0] = num1097;
  partcv[0] = 1;
  Object opresult1099 = callmethod(call1096, ">", 1, partcv, params);
// compilenode returning opresult1099
  int op_slot_left_1093 = gc_frame_newslot(opresult1099);
  int callframe1101 = gc_frame_new();
  Object num1102 = alloc_Float64(1);
// compilenode returning num1102
  gc_frame_newslot(num1102);
  Object num1103 = alloc_Float64(8);
// compilenode returning num1103
  gc_frame_newslot(num1103);
// compilenode returning *var_line
  params[0] = num1102;
  params[1] = num1103;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1104 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1101);
// compilenode returning call1104
  int op_slot_left_1100 = gc_frame_newslot(call1104);
  if (strlit1105 == NULL) {
    strlit1105 = alloc_String("dialect ");
    gc_root(strlit1105);
  }
// compilenode returning strlit1105
  int op_slot_right_1100 = gc_frame_newslot(strlit1105);
  params[0] = strlit1105;
  partcv[0] = 1;
  Object opresult1107 = callmethod(call1104, "==", 1, partcv, params);
// compilenode returning opresult1107
  int op_slot_right_1093 = gc_frame_newslot(opresult1107);
  params[0] = opresult1107;
  partcv[0] = 1;
  Object opresult1109 = callmethod(opresult1099, "&&", 1, partcv, params);
// compilenode returning opresult1109
  Object if1092 = done;
  if (istrue(opresult1109)) {
// Begin line 362
  setline(362);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 361
  setline(361);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1110 = alloc_Boolean(1);
// compilenode returning bool1110
  return bool1110;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1092 = undefined;
  } else {
  }
// compilenode returning if1092
    gc_frame_newslot(if1092);
    if1066 = if1092;
  }
// compilenode returning if1066
    gc_frame_newslot(if1066);
    if1047 = if1066;
  }
// compilenode returning if1047
    gc_frame_newslot(if1047);
    if1039 = if1047;
  }
// compilenode returning if1039
    gc_frame_newslot(if1039);
    if1032 = if1039;
  } else {
  }
// compilenode returning if1032
// Begin line 365
  setline(365);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 364
  setline(364);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1111 = alloc_Boolean(0);
// compilenode returning bool1111
  return bool1111;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply1126(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1128 = gc_frame_newslot(*var_curPos);
  int callframe1129 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1130 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1129);
// compilenode returning call1130
// compilenode returning call1130
  int op_slot_right_1128 = gc_frame_newslot(call1130);
  params[0] = call1130;
  partcv[0] = 1;
  Object opresult1132 = callmethod(*var_curPos, "<", 1, partcv, params);
// compilenode returning opresult1132
  int op_slot_left_1127 = gc_frame_newslot(opresult1132);
  int callframe1133 = gc_frame_new();
// compilenode returning *var_line
// compilenode returning *var_curPos
  int op_slot_left_1136 = gc_frame_newslot(*var_curPos);
  Object num1137 = alloc_Float64(1);
// compilenode returning num1137
  int op_slot_right_1136 = gc_frame_newslot(num1137);
  params[0] = num1137;
  partcv[0] = 1;
  Object sum1139 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1139
  params[0] = sum1139;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1140 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1140
  int op_slot_left_1135 = gc_frame_newslot(idxres1140);
  if (strlit1141 == NULL) {
    strlit1141 = alloc_String("""\x22""");
    gc_root(strlit1141);
  }
// compilenode returning strlit1141
  int op_slot_right_1135 = gc_frame_newslot(strlit1141);
  params[0] = strlit1141;
  partcv[0] = 1;
  Object opresult1143 = callmethod(idxres1140, "==", 1, partcv, params);
// compilenode returning opresult1143
  int op_slot_left_1134 = gc_frame_newslot(opresult1143);
// compilenode returning *var_line
// compilenode returning *var_curPos
  int op_slot_left_1145 = gc_frame_newslot(*var_curPos);
  Object num1146 = alloc_Float64(1);
// compilenode returning num1146
  int op_slot_right_1145 = gc_frame_newslot(num1146);
  params[0] = num1146;
  partcv[0] = 1;
  Object sum1148 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1148
  params[0] = sum1148;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1149 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1149
  int op_slot_left_1144 = gc_frame_newslot(idxres1149);
  if (strlit1150 == NULL) {
    strlit1150 = alloc_String(" ");
    gc_root(strlit1150);
  }
// compilenode returning strlit1150
  int op_slot_right_1144 = gc_frame_newslot(strlit1150);
  params[0] = strlit1150;
  partcv[0] = 1;
  Object opresult1152 = callmethod(idxres1149, "==", 1, partcv, params);
// compilenode returning opresult1152
  int op_slot_right_1134 = gc_frame_newslot(opresult1152);
  params[0] = opresult1152;
  partcv[0] = 1;
  Object opresult1154 = callmethod(opresult1143, "||", 1, partcv, params);
// compilenode returning opresult1154
  partcv[0] = 0;
  Object call1155 = callmethod(opresult1154, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1133);
// compilenode returning call1155
  int op_slot_right_1127 = gc_frame_newslot(call1155);
  params[0] = call1155;
  partcv[0] = 1;
  Object opresult1157 = callmethod(opresult1132, "&&", 1, partcv, params);
// compilenode returning opresult1157
  gc_frame_end(frame);
  return opresult1157;
}
Object meth_PackageManager__apply1160(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 376
  setline(376);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 375
  setline(375);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1161 = gc_frame_newslot(*var_curPos);
  Object num1162 = alloc_Float64(1);
// compilenode returning num1162
  int op_slot_right_1161 = gc_frame_newslot(num1162);
  params[0] = num1162;
  partcv[0] = 1;
  Object sum1164 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1164
  *var_curPos = sum1164;
  if (sum1164 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_parseImport1112(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 369
  setline(369);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 368
  setline(368);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1113 = alloc_Float64(1);
// compilenode returning num1113
  *var_curPos = num1113;
  if (num1113 == undefined)
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
// Begin line 371
  setline(371);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 370
  setline(370);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1114 == NULL) {
    strlit1114 = alloc_String("");
    gc_root(strlit1114);
  }
// compilenode returning strlit1114
  *var_nextImport = strlit1114;
  if (strlit1114 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 372
  setline(372);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1115 = gc_frame_new();
// Begin line 371
  setline(371);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// compilenode returning *var_curPos
  gc_frame_newslot(*var_curPos);
  params[0] = *var_line;
  params[1] = *var_curPos;
  partcv[0] = 2;
  Object call1116 = callmethodflags(self, "skipWhiteSpace", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1115);
// compilenode returning call1116
  *var_curPos = call1116;
  if (call1116 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 373
  setline(373);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 372
  setline(372);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1118 = gc_frame_newslot(*var_curPos);
  Object num1119 = alloc_Float64(1);
// compilenode returning num1119
  int op_slot_right_1118 = gc_frame_newslot(num1119);
  params[0] = num1119;
  partcv[0] = 1;
  Object sum1121 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1121
  *var_curPos = sum1121;
  if (sum1121 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 374
  setline(374);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 373
  setline(373);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 377
  setline(377);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1124 = gc_frame_new();
// Begin line 374
  setline(374);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1125 = alloc_Block(NULL, NULL, "PackageManager", 374);
  gc_frame_newslot(block1125);
  block_savedest(block1125);
  Object closure1126 = createclosure(3, "_apply");
setclosureframe(closure1126, stackframe);
  addtoclosure(closure1126, var_curPos);
  addtoclosure(closure1126, var_line);
  addtoclosure(closure1126, selfslot);
  struct UserObject *uo1126 = (struct UserObject*)block1125;
  uo1126->data[0] = (Object)closure1126;
  Method *meth_meth_PackageManager__apply1126 = addmethod2pos(block1125, "_apply", &meth_PackageManager__apply1126, 0);
int argcv_meth_PackageManager__apply1126[] = {0};
meth_meth_PackageManager__apply1126->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1126);
  meth_meth_PackageManager__apply1126->definitionModule = modulename;
  meth_meth_PackageManager__apply1126->definitionLine = 360;
// compilenode returning block1125
  gc_frame_newslot(block1125);
// Begin line 377
  setline(377);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1159 = alloc_Block(NULL, NULL, "PackageManager", 377);
  gc_frame_newslot(block1159);
  block_savedest(block1159);
  Object closure1160 = createclosure(2, "_apply");
setclosureframe(closure1160, stackframe);
  addtoclosure(closure1160, var_curPos);
  addtoclosure(closure1160, selfslot);
  struct UserObject *uo1160 = (struct UserObject*)block1159;
  uo1160->data[0] = (Object)closure1160;
  Method *meth_meth_PackageManager__apply1160 = addmethod2pos(block1159, "_apply", &meth_PackageManager__apply1160, 0);
int argcv_meth_PackageManager__apply1160[] = {0};
meth_meth_PackageManager__apply1160->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1160);
  meth_meth_PackageManager__apply1160->definitionModule = modulename;
  meth_meth_PackageManager__apply1160->definitionLine = 374;
// compilenode returning block1159
  gc_frame_newslot(block1159);
  params[0] = block1125;
  params[1] = block1159;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1167 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1124);
// compilenode returning call1167
// Begin line 378
  setline(378);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1168 = gc_frame_new();
// Begin line 377
  setline(377);
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
  Object call1169 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1168);
// compilenode returning call1169
  *var_nextImport = call1169;
  if (call1169 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 382
  setline(382);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 378
  setline(378);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1172 = gc_frame_new();
  int callframe1173 = gc_frame_new();
// compilenode returning *var_nextImport
  gc_frame_newslot(*var_nextImport);
// compilenode returning *var_imported
  params[0] = *var_nextImport;
  partcv[0] = 1;
  Object call1174 = callmethod(*var_imported, "contains",
    1, partcv, params);
  gc_frame_end(callframe1173);
// compilenode returning call1174
  partcv[0] = 0;
  Object call1175 = callmethod(call1174, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1172);
// compilenode returning call1175
  Object if1171 = done;
  if (istrue(call1175)) {
// Begin line 380
  setline(380);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 379
  setline(379);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1177 == NULL) {
    strlit1177 = alloc_String("next import = ");
    gc_root(strlit1177);
  }
// compilenode returning strlit1177
  int op_slot_left_1176 = gc_frame_newslot(strlit1177);
// compilenode returning *var_nextImport
  int op_slot_right_1176 = gc_frame_newslot(*var_nextImport);
  params[0] = *var_nextImport;
  partcv[0] = 1;
  Object opresult1179 = callmethod(strlit1177, "++", 1, partcv, params);
// compilenode returning opresult1179
  params[0] = opresult1179;
  Object call1180 = gracelib_print(NULL, 1,  params);
// compilenode returning call1180
// Begin line 381
  setline(381);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1181 = gc_frame_new();
// Begin line 380
  setline(380);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_nextImport
  gc_frame_newslot(*var_nextImport);
// compilenode returning *var_toProcess
  params[0] = *var_nextImport;
  partcv[0] = 1;
  Object call1182 = callmethod(*var_toProcess, "push",
    1, partcv, params);
  gc_frame_end(callframe1181);
// compilenode returning call1182
    gc_frame_newslot(call1182);
    if1171 = call1182;
  } else {
  }
// compilenode returning if1171
  gc_frame_end(frame);
  return if1171;
}
Object meth_PackageManager__apply1186(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1188 = gc_frame_newslot(*var_count2);
  int callframe1189 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1190 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1189);
// compilenode returning call1190
// compilenode returning call1190
  int op_slot_right_1188 = gc_frame_newslot(call1190);
  params[0] = call1190;
  partcv[0] = 1;
  Object opresult1192 = callmethod(*var_count2, "<=", 1, partcv, params);
// compilenode returning opresult1192
  int op_slot_left_1187 = gc_frame_newslot(opresult1192);
// compilenode returning *var_line
// compilenode returning *var_count2
  params[0] = *var_count2;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1194 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1194
  int op_slot_left_1193 = gc_frame_newslot(idxres1194);
  if (strlit1195 == NULL) {
    strlit1195 = alloc_String("Could not retrieve ");
    gc_root(strlit1195);
  }
// compilenode returning strlit1195
  int op_slot_right_1193 = gc_frame_newslot(strlit1195);
  params[0] = strlit1195;
  partcv[0] = 1;
  Object opresult1197 = callmethod(idxres1194, "==", 1, partcv, params);
// compilenode returning opresult1197
  int op_slot_right_1187 = gc_frame_newslot(opresult1197);
  params[0] = opresult1197;
  partcv[0] = 1;
  Object opresult1199 = callmethod(opresult1192, "&&", 1, partcv, params);
// compilenode returning opresult1199
  gc_frame_end(frame);
  return opresult1199;
}
Object meth_PackageManager__apply1202(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 388
  setline(388);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 387
  setline(387);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count2
  int op_slot_left_1203 = gc_frame_newslot(*var_count2);
  Object num1204 = alloc_Float64(1);
// compilenode returning num1204
  int op_slot_right_1203 = gc_frame_newslot(num1204);
  params[0] = num1204;
  partcv[0] = 1;
  Object sum1206 = callmethod(*var_count2, "+", 1, partcv, params);
// compilenode returning sum1206
  *var_count2 = sum1206;
  if (sum1206 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_skipWhiteSpace1183(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 386
  setline(386);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 385
  setline(385);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  *var_count2 = *var_count;
  if (*var_count == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 389
  setline(389);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1184 = gc_frame_new();
// Begin line 386
  setline(386);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1185 = alloc_Block(NULL, NULL, "PackageManager", 386);
  gc_frame_newslot(block1185);
  block_savedest(block1185);
  Object closure1186 = createclosure(3, "_apply");
setclosureframe(closure1186, stackframe);
  addtoclosure(closure1186, var_count2);
  addtoclosure(closure1186, var_line);
  addtoclosure(closure1186, selfslot);
  struct UserObject *uo1186 = (struct UserObject*)block1185;
  uo1186->data[0] = (Object)closure1186;
  Method *meth_meth_PackageManager__apply1186 = addmethod2pos(block1185, "_apply", &meth_PackageManager__apply1186, 0);
int argcv_meth_PackageManager__apply1186[] = {0};
meth_meth_PackageManager__apply1186->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1186);
  meth_meth_PackageManager__apply1186->definitionModule = modulename;
  meth_meth_PackageManager__apply1186->definitionLine = 374;
// compilenode returning block1185
  gc_frame_newslot(block1185);
// Begin line 389
  setline(389);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1201 = alloc_Block(NULL, NULL, "PackageManager", 389);
  gc_frame_newslot(block1201);
  block_savedest(block1201);
  Object closure1202 = createclosure(2, "_apply");
setclosureframe(closure1202, stackframe);
  addtoclosure(closure1202, var_count2);
  addtoclosure(closure1202, selfslot);
  struct UserObject *uo1202 = (struct UserObject*)block1201;
  uo1202->data[0] = (Object)closure1202;
  Method *meth_meth_PackageManager__apply1202 = addmethod2pos(block1201, "_apply", &meth_PackageManager__apply1202, 0);
int argcv_meth_PackageManager__apply1202[] = {0};
meth_meth_PackageManager__apply1202->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1202);
  meth_meth_PackageManager__apply1202->definitionModule = modulename;
  meth_meth_PackageManager__apply1202->definitionLine = 386;
// compilenode returning block1201
  gc_frame_newslot(block1201);
  params[0] = block1185;
  params[1] = block1201;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1209 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1184);
// compilenode returning call1209
// Begin line 389
  setline(389);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count2
  gc_frame_end(frame);
  return *var_count2;
}
Object meth_PackageManager_displayHelp1210(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 394
  setline(394);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 393
  setline(393);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1211 == NULL) {
    strlit1211 = alloc_String("Available options are:");
    gc_root(strlit1211);
  }
// compilenode returning strlit1211
  params[0] = strlit1211;
  Object call1212 = gracelib_print(NULL, 1,  params);
// compilenode returning call1212
// Begin line 395
  setline(395);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 394
  setline(394);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1213 == NULL) {
    strlit1213 = alloc_String("-get [url] to fetch a package located at a given url");
    gc_root(strlit1213);
  }
// compilenode returning strlit1213
  params[0] = strlit1213;
  Object call1214 = gracelib_print(NULL, 1,  params);
// compilenode returning call1214
// Begin line 396
  setline(396);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 395
  setline(395);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1215 == NULL) {
    strlit1215 = alloc_String("-list to display currenly installed packages");
    gc_root(strlit1215);
  }
// compilenode returning strlit1215
  params[0] = strlit1215;
  Object call1216 = gracelib_print(NULL, 1,  params);
// compilenode returning call1216
  gc_frame_end(frame);
  return call1216;
}
Object meth_PackageManager_printMessage1217(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 402
  setline(402);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 399
  setline(399);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_verbose
  Object if1218 = done;
  if (istrue(*var_verbose)) {
// Begin line 401
  setline(401);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 400
  setline(400);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_message
  params[0] = *var_message;
  Object call1219 = gracelib_print(NULL, 1,  params);
// compilenode returning call1219
    gc_frame_newslot(call1219);
    if1218 = call1219;
  } else {
  }
// compilenode returning if1218
  gc_frame_end(frame);
  return if1218;
}
Object meth_PackageManager__apply1282(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int callframe1284 = gc_frame_new();
// compilenode returning *var_imported
  partcv[0] = 0;
  Object call1285 = callmethod(*var_imported, "size",
    1, partcv, params);
  gc_frame_end(callframe1284);
// compilenode returning call1285
// compilenode returning call1285
  int op_slot_left_1283 = gc_frame_newslot(call1285);
  Object num1286 = alloc_Float64(0);
// compilenode returning num1286
  int op_slot_right_1283 = gc_frame_newslot(num1286);
  params[0] = num1286;
  partcv[0] = 1;
  Object opresult1288 = callmethod(call1285, ">", 1, partcv, params);
// compilenode returning opresult1288
  gc_frame_end(frame);
  return opresult1288;
}
Object meth_PackageManager__apply1291(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 420
  setline(420);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 419
  setline(419);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1292 = gc_frame_new();
// compilenode returning *var_imported
  partcv[0] = 0;
  Object call1293 = callmethod(*var_imported, "pop",
    1, partcv, params);
  gc_frame_end(callframe1292);
// compilenode returning call1293
// compilenode returning call1293
  *var_curImport = call1293;
  if (call1293 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 421
  setline(421);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1294 = gc_frame_new();
// Begin line 420
  setline(420);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1299 == NULL) {
    strlit1299 = alloc_String("");
    gc_root(strlit1299);
  }
// compilenode returning strlit1299
  int op_slot_left_1298 = gc_frame_newslot(strlit1299);
// compilenode returning *var_name
  int op_slot_right_1298 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult1301 = callmethod(strlit1299, "++", 1, partcv, params);
// compilenode returning opresult1301
  int op_slot_left_1297 = gc_frame_newslot(opresult1301);
  if (strlit1302 == NULL) {
    strlit1302 = alloc_String("/");
    gc_root(strlit1302);
  }
// compilenode returning strlit1302
  int op_slot_right_1297 = gc_frame_newslot(strlit1302);
  params[0] = strlit1302;
  partcv[0] = 1;
  Object opresult1304 = callmethod(opresult1301, "++", 1, partcv, params);
// compilenode returning opresult1304
  int op_slot_left_1296 = gc_frame_newslot(opresult1304);
  int callframe1305 = gc_frame_new();
  int callframe1306 = gc_frame_new();
// compilenode returning *var_curImport
  partcv[0] = 0;
  Object call1307 = callmethod(*var_curImport, "address",
    1, partcv, params);
  gc_frame_end(callframe1306);
// compilenode returning call1307
// compilenode returning call1307
  gc_frame_newslot(call1307);
  params[0] = call1307;
  partcv[0] = 1;
  Object call1308 = callmethodflags(self, "removeContainingDir", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1305);
// compilenode returning call1308
  int op_slot_right_1296 = gc_frame_newslot(call1308);
  params[0] = call1308;
  partcv[0] = 1;
  Object opresult1310 = callmethod(opresult1304, "++", 1, partcv, params);
// compilenode returning opresult1310
  int op_slot_left_1295 = gc_frame_newslot(opresult1310);
  if (strlit1311 == NULL) {
    strlit1311 = alloc_String("");
    gc_root(strlit1311);
  }
// compilenode returning strlit1311
  int op_slot_right_1295 = gc_frame_newslot(strlit1311);
  params[0] = strlit1311;
  partcv[0] = 1;
  Object opresult1313 = callmethod(opresult1310, "++", 1, partcv, params);
// compilenode returning opresult1313
  gc_frame_newslot(opresult1313);
  if (strlit1314 == NULL) {
    strlit1314 = alloc_String("w");
    gc_root(strlit1314);
  }
// compilenode returning strlit1314
  gc_frame_newslot(strlit1314);
// compilenode returning *var_io
  params[0] = opresult1313;
  params[1] = strlit1314;
  partcv[0] = 2;
  Object call1315 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1294);
// compilenode returning call1315
  *var_toWrite = call1315;
  if (call1315 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 422
  setline(422);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1317 = gc_frame_new();
// Begin line 421
  setline(421);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1318 = gc_frame_new();
// compilenode returning *var_curImport
  partcv[0] = 0;
  Object call1319 = callmethod(*var_curImport, "data",
    1, partcv, params);
  gc_frame_end(callframe1318);
// compilenode returning call1319
// compilenode returning call1319
  gc_frame_newslot(call1319);
// compilenode returning *var_toWrite
  params[0] = call1319;
  partcv[0] = 1;
  Object call1320 = callmethod(*var_toWrite, "write",
    1, partcv, params);
  gc_frame_end(callframe1317);
// compilenode returning call1320
  gc_frame_end(frame);
  return call1320;
}
Object meth_PackageManager_bundle1220(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 409
  setline(409);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 408
  setline(408);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array1221 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array1221
  *var_imported = array1221;
  if (array1221 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 409
  setline(409);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1223 = gc_frame_new();
// compilenode returning *var_toBundle
  gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object call1224 = callmethodflags(self, "getContainingDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1223);
// compilenode returning call1224
  *var_bundlePath = call1224;
  if (call1224 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 411
  setline(411);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1226 = gc_frame_new();
// Begin line 410
  setline(410);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1229 == NULL) {
    strlit1229 = alloc_String("");
    gc_root(strlit1229);
  }
// compilenode returning strlit1229
  int op_slot_left_1228 = gc_frame_newslot(strlit1229);
// compilenode returning *var_name
  int op_slot_right_1228 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult1231 = callmethod(strlit1229, "++", 1, partcv, params);
// compilenode returning opresult1231
  int op_slot_left_1227 = gc_frame_newslot(opresult1231);
  if (strlit1232 == NULL) {
    strlit1232 = alloc_String("/");
    gc_root(strlit1232);
  }
// compilenode returning strlit1232
  int op_slot_right_1227 = gc_frame_newslot(strlit1232);
  params[0] = strlit1232;
  partcv[0] = 1;
  Object opresult1234 = callmethod(opresult1231, "++", 1, partcv, params);
// compilenode returning opresult1234
  gc_frame_newslot(opresult1234);
  params[0] = opresult1234;
  partcv[0] = 1;
  Object call1235 = callmethodflags(self, "createDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1226);
// compilenode returning call1235
  *var_newDir = call1235;
  if (call1235 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 411
  setline(411);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1236 = gc_frame_new();
// compilenode returning *var_toBundle
  gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object call1237 = callmethodflags(self, "removeContainingDir", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1236);
// compilenode returning call1237
  *var_newFileName = call1237;
  if (call1237 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 412
  setline(412);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1238 = gc_frame_new();
  if (strlit1241 == NULL) {
    strlit1241 = alloc_String("");
    gc_root(strlit1241);
  }
// compilenode returning strlit1241
  int op_slot_left_1240 = gc_frame_newslot(strlit1241);
// compilenode returning *var_toBundle
  int op_slot_right_1240 = gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object opresult1243 = callmethod(strlit1241, "++", 1, partcv, params);
// compilenode returning opresult1243
  int op_slot_left_1239 = gc_frame_newslot(opresult1243);
  if (strlit1244 == NULL) {
    strlit1244 = alloc_String("");
    gc_root(strlit1244);
  }
// compilenode returning strlit1244
  int op_slot_right_1239 = gc_frame_newslot(strlit1244);
  params[0] = strlit1244;
  partcv[0] = 1;
  Object opresult1246 = callmethod(opresult1243, "++", 1, partcv, params);
// compilenode returning opresult1246
  gc_frame_newslot(opresult1246);
  if (strlit1247 == NULL) {
    strlit1247 = alloc_String("r");
    gc_root(strlit1247);
  }
// compilenode returning strlit1247
  gc_frame_newslot(strlit1247);
// compilenode returning *var_io
  params[0] = opresult1246;
  params[1] = strlit1247;
  partcv[0] = 2;
  Object call1248 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1238);
// compilenode returning call1248
  *var_toOpen = call1248;
  if (call1248 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 414
  setline(414);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 413
  setline(413);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1249 = gc_frame_new();
// compilenode returning *var_toOpen
  partcv[0] = 0;
  Object call1250 = callmethod(*var_toOpen, "read",
    1, partcv, params);
  gc_frame_end(callframe1249);
// compilenode returning call1250
// compilenode returning call1250
  *var_openData = call1250;
  if (call1250 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 415
  setline(415);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1251 = gc_frame_new();
// Begin line 414
  setline(414);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1256 == NULL) {
    strlit1256 = alloc_String("");
    gc_root(strlit1256);
  }
// compilenode returning strlit1256
  int op_slot_left_1255 = gc_frame_newslot(strlit1256);
// compilenode returning *var_name
  int op_slot_right_1255 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult1258 = callmethod(strlit1256, "++", 1, partcv, params);
// compilenode returning opresult1258
  int op_slot_left_1254 = gc_frame_newslot(opresult1258);
  if (strlit1259 == NULL) {
    strlit1259 = alloc_String("/");
    gc_root(strlit1259);
  }
// compilenode returning strlit1259
  int op_slot_right_1254 = gc_frame_newslot(strlit1259);
  params[0] = strlit1259;
  partcv[0] = 1;
  Object opresult1261 = callmethod(opresult1258, "++", 1, partcv, params);
// compilenode returning opresult1261
  int op_slot_left_1253 = gc_frame_newslot(opresult1261);
// compilenode returning *var_newFileName
  int op_slot_right_1253 = gc_frame_newslot(*var_newFileName);
  params[0] = *var_newFileName;
  partcv[0] = 1;
  Object opresult1263 = callmethod(opresult1261, "++", 1, partcv, params);
// compilenode returning opresult1263
  int op_slot_left_1252 = gc_frame_newslot(opresult1263);
  if (strlit1264 == NULL) {
    strlit1264 = alloc_String("");
    gc_root(strlit1264);
  }
// compilenode returning strlit1264
  int op_slot_right_1252 = gc_frame_newslot(strlit1264);
  params[0] = strlit1264;
  partcv[0] = 1;
  Object opresult1266 = callmethod(opresult1263, "++", 1, partcv, params);
// compilenode returning opresult1266
  gc_frame_newslot(opresult1266);
  if (strlit1267 == NULL) {
    strlit1267 = alloc_String("w");
    gc_root(strlit1267);
  }
// compilenode returning strlit1267
  gc_frame_newslot(strlit1267);
// compilenode returning *var_io
  params[0] = opresult1266;
  params[1] = strlit1267;
  partcv[0] = 2;
  Object call1268 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1251);
// compilenode returning call1268
  *var_toWrite = call1268;
  if (call1268 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 415
  setline(415);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1269 = gc_frame_new();
// compilenode returning *var_openData
  gc_frame_newslot(*var_openData);
// compilenode returning *var_toWrite
  params[0] = *var_openData;
  partcv[0] = 1;
  Object call1270 = callmethod(*var_toWrite, "write",
    1, partcv, params);
  gc_frame_end(callframe1269);
// compilenode returning call1270
// Begin line 417
  setline(417);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1271 = gc_frame_new();
// Begin line 416
  setline(416);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toBundle
  gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object call1272 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1271);
// compilenode returning call1272
// Begin line 417
  setline(417);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1274 == NULL) {
    strlit1274 = alloc_String("Imported size = ");
    gc_root(strlit1274);
  }
// compilenode returning strlit1274
  int op_slot_left_1273 = gc_frame_newslot(strlit1274);
  int callframe1275 = gc_frame_new();
// compilenode returning *var_imported
  partcv[0] = 0;
  Object call1276 = callmethod(*var_imported, "size",
    1, partcv, params);
  gc_frame_end(callframe1275);
// compilenode returning call1276
// compilenode returning call1276
  int op_slot_right_1273 = gc_frame_newslot(call1276);
  params[0] = call1276;
  partcv[0] = 1;
  Object opresult1278 = callmethod(strlit1274, "++", 1, partcv, params);
// compilenode returning opresult1278
  params[0] = opresult1278;
  Object call1279 = gracelib_print(NULL, 1,  params);
// compilenode returning call1279
// Begin line 423
  setline(423);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1280 = gc_frame_new();
// Begin line 418
  setline(418);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1281 = alloc_Block(NULL, NULL, "PackageManager", 418);
  gc_frame_newslot(block1281);
  block_savedest(block1281);
  Object closure1282 = createclosure(2, "_apply");
setclosureframe(closure1282, stackframe);
  addtoclosure(closure1282, var_imported);
  addtoclosure(closure1282, selfslot);
  struct UserObject *uo1282 = (struct UserObject*)block1281;
  uo1282->data[0] = (Object)closure1282;
  Method *meth_meth_PackageManager__apply1282 = addmethod2pos(block1281, "_apply", &meth_PackageManager__apply1282, 0);
int argcv_meth_PackageManager__apply1282[] = {0};
meth_meth_PackageManager__apply1282->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1282);
  meth_meth_PackageManager__apply1282->definitionModule = modulename;
  meth_meth_PackageManager__apply1282->definitionLine = 417;
// compilenode returning block1281
  gc_frame_newslot(block1281);
// Begin line 423
  setline(423);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1290 = alloc_Block(NULL, NULL, "PackageManager", 423);
  gc_frame_newslot(block1290);
  block_savedest(block1290);
  Object closure1291 = createclosure(5, "_apply");
setclosureframe(closure1291, stackframe);
  addtoclosure(closure1291, var_imported);
  addtoclosure(closure1291, var_name);
  addtoclosure(closure1291, var_io);
  addtoclosure(closure1291, var_toWrite);
  addtoclosure(closure1291, selfslot);
  struct UserObject *uo1291 = (struct UserObject*)block1290;
  uo1291->data[0] = (Object)closure1291;
  Method *meth_meth_PackageManager__apply1291 = addmethod2pos(block1290, "_apply", &meth_PackageManager__apply1291, 0);
int argcv_meth_PackageManager__apply1291[] = {0};
meth_meth_PackageManager__apply1291->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1291);
  meth_meth_PackageManager__apply1291->definitionModule = modulename;
  meth_meth_PackageManager__apply1291->definitionLine = 418;
// compilenode returning block1290
  gc_frame_newslot(block1290);
  params[0] = block1281;
  params[1] = block1290;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1322 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1280);
// compilenode returning call1322
  gc_frame_end(frame);
  return call1322;
}
Object meth_PackageManager__apply1392(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 438
  setline(438);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1393 = gc_frame_new();
// compilenode returning *var_d
  gc_frame_newslot(*var_d);
// compilenode returning *var_out
  params[0] = *var_d;
  partcv[0] = 1;
  Object call1394 = callmethod(*var_out, "write",
    1, partcv, params);
  gc_frame_end(callframe1393);
// compilenode returning call1394
  gc_frame_end(frame);
  return call1394;
}
Object meth_PackageManager__apply1328(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 442
  setline(442);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 428
  setline(428);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1331 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1332 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1331);
// compilenode returning call1332
// compilenode returning call1332
  int op_slot_left_1330 = gc_frame_newslot(call1332);
  Object num1333 = alloc_Float64(4);
// compilenode returning num1333
  int op_slot_right_1330 = gc_frame_newslot(num1333);
  params[0] = num1333;
  partcv[0] = 1;
  Object opresult1335 = callmethod(call1332, ">", 1, partcv, params);
// compilenode returning opresult1335
  Object if1329 = done;
  if (istrue(opresult1335)) {
// Begin line 441
  setline(441);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 429
  setline(429);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1338 = gc_frame_new();
  int callframe1340 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1341 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1340);
// compilenode returning call1341
// compilenode returning call1341
  int op_slot_left_1339 = gc_frame_newslot(call1341);
  Object num1342 = alloc_Float64(5);
// compilenode returning num1342
  int op_slot_right_1339 = gc_frame_newslot(num1342);
  params[0] = num1342;
  partcv[0] = 1;
  Object diff1344 = callmethod(call1341, "-", 1, partcv, params);
// compilenode returning diff1344
  gc_frame_newslot(diff1344);
  int callframe1345 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1346 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1345);
// compilenode returning call1346
// compilenode returning call1346
  gc_frame_newslot(call1346);
// compilenode returning *var_file
  params[0] = diff1344;
  params[1] = call1346;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1347 = callmethod(*var_file, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1338);
// compilenode returning call1347
  int op_slot_left_1337 = gc_frame_newslot(call1347);
  if (strlit1348 == NULL) {
    strlit1348 = alloc_String(".grace");
    gc_root(strlit1348);
  }
// compilenode returning strlit1348
  int op_slot_right_1337 = gc_frame_newslot(strlit1348);
  params[0] = strlit1348;
  partcv[0] = 1;
  Object opresult1350 = callmethod(call1347, "==", 1, partcv, params);
// compilenode returning opresult1350
  Object if1336 = done;
  if (istrue(opresult1350)) {
// Begin line 430
  setline(430);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1351 = gc_frame_new();
// compilenode returning *var_address
  int op_slot_left_1353 = gc_frame_newslot(*var_address);
  if (strlit1354 == NULL) {
    strlit1354 = alloc_String("/");
    gc_root(strlit1354);
  }
// compilenode returning strlit1354
  int op_slot_right_1353 = gc_frame_newslot(strlit1354);
  params[0] = strlit1354;
  partcv[0] = 1;
  Object opresult1356 = callmethod(*var_address, "++", 1, partcv, params);
// compilenode returning opresult1356
  int op_slot_left_1352 = gc_frame_newslot(opresult1356);
// compilenode returning *var_file
  int op_slot_right_1352 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1358 = callmethod(opresult1356, "++", 1, partcv, params);
// compilenode returning opresult1358
  gc_frame_newslot(opresult1358);
  if (strlit1359 == NULL) {
    strlit1359 = alloc_String("r");
    gc_root(strlit1359);
  }
// compilenode returning strlit1359
  gc_frame_newslot(strlit1359);
// compilenode returning *var_io
  params[0] = opresult1358;
  params[1] = strlit1359;
  partcv[0] = 2;
  Object call1360 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1351);
// compilenode returning call1360
  *var_open = call1360;
  if (call1360 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 431
  setline(431);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1361 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call1362 = callmethod(*var_open, "read",
    1, partcv, params);
  gc_frame_end(callframe1361);
// compilenode returning call1362
// compilenode returning call1362
  *var_readFile = call1362;
  if (call1362 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 432
  setline(432);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1363 = gc_frame_new();
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
  Object call1364 = callmethodflags(self, "parseAndPrefix", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1363);
// compilenode returning call1364
  *var_toWrite = call1364;
  if (call1364 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 433
  setline(433);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1365 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call1366 = callmethod(*var_open, "close",
    1, partcv, params);
  gc_frame_end(callframe1365);
// compilenode returning call1366
// compilenode returning call1366
// Begin line 436
  setline(436);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1367 = gc_frame_new();
// Begin line 435
  setline(435);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1371 == NULL) {
    strlit1371 = alloc_String("rm ");
    gc_root(strlit1371);
  }
// compilenode returning strlit1371
  int op_slot_left_1370 = gc_frame_newslot(strlit1371);
// compilenode returning *var_address
  int op_slot_right_1370 = gc_frame_newslot(*var_address);
  params[0] = *var_address;
  partcv[0] = 1;
  Object opresult1373 = callmethod(strlit1371, "++", 1, partcv, params);
// compilenode returning opresult1373
  int op_slot_left_1369 = gc_frame_newslot(opresult1373);
  if (strlit1374 == NULL) {
    strlit1374 = alloc_String("/");
    gc_root(strlit1374);
  }
// compilenode returning strlit1374
  int op_slot_right_1369 = gc_frame_newslot(strlit1374);
  params[0] = strlit1374;
  partcv[0] = 1;
  Object opresult1376 = callmethod(opresult1373, "++", 1, partcv, params);
// compilenode returning opresult1376
  int op_slot_left_1368 = gc_frame_newslot(opresult1376);
// compilenode returning *var_file
  int op_slot_right_1368 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1378 = callmethod(opresult1376, "++", 1, partcv, params);
// compilenode returning opresult1378
  gc_frame_newslot(opresult1378);
// compilenode returning *var_io
  params[0] = opresult1378;
  partcv[0] = 1;
  Object call1379 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe1367);
// compilenode returning call1379
// Begin line 436
  setline(436);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1380 = gc_frame_new();
// compilenode returning *var_address
  int op_slot_left_1382 = gc_frame_newslot(*var_address);
  if (strlit1383 == NULL) {
    strlit1383 = alloc_String("/");
    gc_root(strlit1383);
  }
// compilenode returning strlit1383
  int op_slot_right_1382 = gc_frame_newslot(strlit1383);
  params[0] = strlit1383;
  partcv[0] = 1;
  Object opresult1385 = callmethod(*var_address, "++", 1, partcv, params);
// compilenode returning opresult1385
  int op_slot_left_1381 = gc_frame_newslot(opresult1385);
// compilenode returning *var_file
  int op_slot_right_1381 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1387 = callmethod(opresult1385, "++", 1, partcv, params);
// compilenode returning opresult1387
  gc_frame_newslot(opresult1387);
  if (strlit1388 == NULL) {
    strlit1388 = alloc_String("w");
    gc_root(strlit1388);
  }
// compilenode returning strlit1388
  gc_frame_newslot(strlit1388);
// compilenode returning *var_io
  params[0] = opresult1387;
  params[1] = strlit1388;
  partcv[0] = 2;
  Object call1389 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1380);
// compilenode returning call1389
  *var_out = call1389;
  if (call1389 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 440
  setline(440);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1390 = gc_frame_new();
// Begin line 437
  setline(437);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toWrite
  gc_frame_newslot(*var_toWrite);
// Begin line 440
  setline(440);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1391 = alloc_Block(NULL, NULL, "PackageManager", 440);
  gc_frame_newslot(block1391);
  block_savedest(block1391);
  Object closure1392 = createclosure(2, "_apply");
setclosureframe(closure1392, stackframe);
  addtoclosure(closure1392, var_out);
  addtoclosure(closure1392, selfslot);
  struct UserObject *uo1392 = (struct UserObject*)block1391;
  uo1392->data[0] = (Object)closure1392;
  Method *meth_meth_PackageManager__apply1392 = addmethod2pos(block1391, "_apply", &meth_PackageManager__apply1392, 0);
int argcv_meth_PackageManager__apply1392[] = {1};
meth_meth_PackageManager__apply1392->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1392);
  meth_meth_PackageManager__apply1392->definitionModule = modulename;
  meth_meth_PackageManager__apply1392->definitionLine = 433;
// compilenode returning block1391
  gc_frame_newslot(block1391);
  params[0] = *var_toWrite;
  params[1] = block1391;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1396 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1390);
// compilenode returning call1396
    gc_frame_newslot(call1396);
    if1336 = call1396;
  } else {
  }
// compilenode returning if1336
    gc_frame_newslot(if1336);
    if1329 = if1336;
  } else {
  }
// compilenode returning if1329
  gc_frame_end(frame);
  return if1329;
}
Object meth_PackageManager_setImportDest1323(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 426
  setline(426);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1324 = gc_frame_new();
// compilenode returning *var_address
  gc_frame_newslot(*var_address);
// compilenode returning *var_io
  params[0] = *var_address;
  partcv[0] = 1;
  Object call1325 = callmethod(*var_io, "getdir",
    1, partcv, params);
  gc_frame_end(callframe1324);
// compilenode returning call1325
  *var_folder = call1325;
  if (call1325 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 446
  setline(446);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1326 = gc_frame_new();
// Begin line 427
  setline(427);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_folder
  gc_frame_newslot(*var_folder);
// Begin line 446
  setline(446);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1327 = alloc_Block(NULL, NULL, "PackageManager", 446);
  gc_frame_newslot(block1327);
  block_savedest(block1327);
  Object closure1328 = createclosure(4, "_apply");
setclosureframe(closure1328, stackframe);
  addtoclosure(closure1328, var_address);
  addtoclosure(closure1328, var_io);
  addtoclosure(closure1328, var_prefix);
  addtoclosure(closure1328, selfslot);
  struct UserObject *uo1328 = (struct UserObject*)block1327;
  uo1328->data[0] = (Object)closure1328;
  Method *meth_meth_PackageManager__apply1328 = addmethod2pos(block1327, "_apply", &meth_PackageManager__apply1328, 0);
int argcv_meth_PackageManager__apply1328[] = {1};
meth_meth_PackageManager__apply1328->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1328);
  meth_meth_PackageManager__apply1328->definitionModule = modulename;
  meth_meth_PackageManager__apply1328->definitionLine = 421;
// compilenode returning block1327
  gc_frame_newslot(block1327);
  params[0] = *var_folder;
  params[1] = block1327;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1398 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1326);
// compilenode returning call1398
  gc_frame_end(frame);
  return call1398;
}
Object meth_PackageManager__apply1413(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1414 = gc_frame_newslot(*var_curPos);
  int callframe1415 = gc_frame_new();
// compilenode returning *var_readFile
  partcv[0] = 0;
  Object call1416 = callmethod(*var_readFile, "size",
    1, partcv, params);
  gc_frame_end(callframe1415);
// compilenode returning call1416
// compilenode returning call1416
  int op_slot_right_1414 = gc_frame_newslot(call1416);
  params[0] = call1416;
  partcv[0] = 1;
  Object opresult1418 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1418
  gc_frame_end(frame);
  return opresult1418;
}
Object meth_PackageManager__apply1425(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1427 = gc_frame_newslot(*var_curPos);
  int callframe1428 = gc_frame_new();
// compilenode returning *var_readFile
  partcv[0] = 0;
  Object call1429 = callmethod(*var_readFile, "size",
    1, partcv, params);
  gc_frame_end(callframe1428);
// compilenode returning call1429
// compilenode returning call1429
  int op_slot_right_1427 = gc_frame_newslot(call1429);
  params[0] = call1429;
  partcv[0] = 1;
  Object opresult1431 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1431
  int op_slot_left_1426 = gc_frame_newslot(opresult1431);
// compilenode returning *var_readFile
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1433 = callmethod(*var_readFile, "[]", 1, partcv, params);
// compilenode returning idxres1433
  int op_slot_left_1432 = gc_frame_newslot(idxres1433);
  if (strlit1434 == NULL) {
    strlit1434 = alloc_String("""\x0a""");
    gc_root(strlit1434);
  }
// compilenode returning strlit1434
  int op_slot_right_1432 = gc_frame_newslot(strlit1434);
  params[0] = strlit1434;
  partcv[0] = 1;
  Object opresult1436 = callmethod(idxres1433, "!=", 1, partcv, params);
// compilenode returning opresult1436
  int op_slot_right_1426 = gc_frame_newslot(opresult1436);
  params[0] = opresult1436;
  partcv[0] = 1;
  Object opresult1438 = callmethod(opresult1431, "&&", 1, partcv, params);
// compilenode returning opresult1438
  gc_frame_end(frame);
  return opresult1438;
}
Object meth_PackageManager__apply1441(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 457
  setline(457);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1442 = gc_frame_newslot(*var_curPos);
  Object num1443 = alloc_Float64(1);
// compilenode returning num1443
  int op_slot_right_1442 = gc_frame_newslot(num1443);
  params[0] = num1443;
  partcv[0] = 1;
  Object sum1445 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1445
  *var_curPos = sum1445;
  if (sum1445 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1466(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 463
  setline(463);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  int op_slot_left_1467 = gc_frame_newslot(*var_line);
// compilenode returning *var_readFile
// compilenode returning *var_b
  params[0] = *var_b;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1468 = callmethod(*var_readFile, "[]", 1, partcv, params);
// compilenode returning idxres1468
  int op_slot_right_1467 = gc_frame_newslot(idxres1468);
  params[0] = idxres1468;
  partcv[0] = 1;
  Object opresult1470 = callmethod(*var_line, "++", 1, partcv, params);
// compilenode returning opresult1470
  *var_line = opresult1470;
  if (opresult1470 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1510(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1512 = gc_frame_newslot(*var_pos);
  int callframe1513 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1514 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1513);
// compilenode returning call1514
// compilenode returning call1514
  int op_slot_right_1512 = gc_frame_newslot(call1514);
  params[0] = call1514;
  partcv[0] = 1;
  Object opresult1516 = callmethod(*var_pos, "<", 1, partcv, params);
// compilenode returning opresult1516
  int op_slot_left_1511 = gc_frame_newslot(opresult1516);
  int callframe1517 = gc_frame_new();
// compilenode returning *var_line
// compilenode returning *var_pos
  params[0] = *var_pos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1520 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1520
  int op_slot_left_1519 = gc_frame_newslot(idxres1520);
  if (strlit1521 == NULL) {
    strlit1521 = alloc_String("""\x22""");
    gc_root(strlit1521);
  }
// compilenode returning strlit1521
  int op_slot_right_1519 = gc_frame_newslot(strlit1521);
  params[0] = strlit1521;
  partcv[0] = 1;
  Object opresult1523 = callmethod(idxres1520, "==", 1, partcv, params);
// compilenode returning opresult1523
  int op_slot_left_1518 = gc_frame_newslot(opresult1523);
// compilenode returning *var_line
// compilenode returning *var_pos
  params[0] = *var_pos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1525 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1525
  int op_slot_left_1524 = gc_frame_newslot(idxres1525);
  if (strlit1526 == NULL) {
    strlit1526 = alloc_String(" ");
    gc_root(strlit1526);
  }
// compilenode returning strlit1526
  int op_slot_right_1524 = gc_frame_newslot(strlit1526);
  params[0] = strlit1526;
  partcv[0] = 1;
  Object opresult1528 = callmethod(idxres1525, "==", 1, partcv, params);
// compilenode returning opresult1528
  int op_slot_right_1518 = gc_frame_newslot(opresult1528);
  params[0] = opresult1528;
  partcv[0] = 1;
  Object opresult1530 = callmethod(opresult1523, "||", 1, partcv, params);
// compilenode returning opresult1530
  partcv[0] = 0;
  Object call1531 = callmethod(opresult1530, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1517);
// compilenode returning call1531
  int op_slot_right_1511 = gc_frame_newslot(call1531);
  params[0] = call1531;
  partcv[0] = 1;
  Object opresult1533 = callmethod(opresult1516, "&&", 1, partcv, params);
// compilenode returning opresult1533
  gc_frame_end(frame);
  return opresult1533;
}
Object meth_PackageManager__apply1536(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 475
  setline(475);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 474
  setline(474);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  int op_slot_left_1537 = gc_frame_newslot(*var_pos);
  Object num1538 = alloc_Float64(1);
// compilenode returning num1538
  int op_slot_right_1537 = gc_frame_newslot(num1538);
  params[0] = num1538;
  partcv[0] = 1;
  Object sum1540 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1540
  *var_pos = sum1540;
  if (sum1540 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1574(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 481
  setline(481);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1575 = gc_frame_new();
// compilenode returning *var_d
  gc_frame_newslot(*var_d);
// compilenode returning *var_outFile
  params[0] = *var_d;
  partcv[0] = 1;
  Object call1576 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1575);
// compilenode returning call1576
  gc_frame_end(frame);
  return call1576;
}
Object meth_PackageManager__apply1581(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 486
  setline(486);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1582 = gc_frame_new();
// compilenode returning *var_d
  gc_frame_newslot(*var_d);
// compilenode returning *var_outFile
  params[0] = *var_d;
  partcv[0] = 1;
  Object call1583 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1582);
// compilenode returning call1583
  gc_frame_end(frame);
  return call1583;
}
Object meth_PackageManager__apply1421(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 456
  setline(456);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 455
  setline(455);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 459
  setline(459);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1423 = gc_frame_new();
// Begin line 456
  setline(456);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1424 = alloc_Block(NULL, NULL, "PackageManager", 456);
  gc_frame_newslot(block1424);
  block_savedest(block1424);
  Object closure1425 = createclosure(3, "_apply");
setclosureframe(closure1425, stackframe);
  addtoclosure(closure1425, var_curPos);
  addtoclosure(closure1425, var_readFile);
  addtoclosure(closure1425, selfslot);
  struct UserObject *uo1425 = (struct UserObject*)block1424;
  uo1425->data[0] = (Object)closure1425;
  Method *meth_meth_PackageManager__apply1425 = addmethod2pos(block1424, "_apply", &meth_PackageManager__apply1425, 0);
int argcv_meth_PackageManager__apply1425[] = {0};
meth_meth_PackageManager__apply1425->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1425);
  meth_meth_PackageManager__apply1425->definitionModule = modulename;
  meth_meth_PackageManager__apply1425->definitionLine = 454;
// compilenode returning block1424
  gc_frame_newslot(block1424);
// Begin line 459
  setline(459);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1440 = alloc_Block(NULL, NULL, "PackageManager", 459);
  gc_frame_newslot(block1440);
  block_savedest(block1440);
  Object closure1441 = createclosure(2, "_apply");
setclosureframe(closure1441, stackframe);
  addtoclosure(closure1441, var_curPos);
  addtoclosure(closure1441, selfslot);
  struct UserObject *uo1441 = (struct UserObject*)block1440;
  uo1441->data[0] = (Object)closure1441;
  Method *meth_meth_PackageManager__apply1441 = addmethod2pos(block1440, "_apply", &meth_PackageManager__apply1441, 0);
int argcv_meth_PackageManager__apply1441[] = {0};
meth_meth_PackageManager__apply1441->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1441);
  meth_meth_PackageManager__apply1441->definitionModule = modulename;
  meth_meth_PackageManager__apply1441->definitionLine = 456;
// compilenode returning block1440
  gc_frame_newslot(block1440);
  params[0] = block1424;
  params[1] = block1440;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1448 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1423);
// compilenode returning call1448
// Begin line 460
  setline(460);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1449 = gc_frame_new();
// Begin line 459
  setline(459);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_startPos
  gc_frame_newslot(*var_startPos);
// compilenode returning *var_curPos
  int op_slot_left_1450 = gc_frame_newslot(*var_curPos);
  Object num1451 = alloc_Float64(1);
// compilenode returning num1451
  int op_slot_right_1450 = gc_frame_newslot(num1451);
  params[0] = num1451;
  partcv[0] = 1;
  Object diff1453 = callmethod(*var_curPos, "-", 1, partcv, params);
// compilenode returning diff1453
  gc_frame_newslot(diff1453);
// compilenode returning *var_readFile
  params[0] = *var_startPos;
  params[1] = diff1453;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1454 = callmethod(*var_readFile, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1449);
// compilenode returning call1454
  *var_data = call1454;
  if (call1454 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 460
  setline(460);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1455 == NULL) {
    strlit1455 = alloc_String("");
    gc_root(strlit1455);
  }
// compilenode returning strlit1455
  *var_line = strlit1455;
  if (strlit1455 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 461
  setline(461);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1456 = alloc_Float64(1);
// compilenode returning num1456
  *var_count = num1456;
  if (num1456 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 465
  setline(465);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1457 = gc_frame_new();
// Begin line 462
  setline(462);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_startPos
  int op_slot_left_1458 = gc_frame_newslot(*var_startPos);
// compilenode returning *var_curPos
  int op_slot_left_1459 = gc_frame_newslot(*var_curPos);
  Object num1460 = alloc_Float64(1);
// compilenode returning num1460
  int op_slot_right_1459 = gc_frame_newslot(num1460);
  params[0] = num1460;
  partcv[0] = 1;
  Object diff1462 = callmethod(*var_curPos, "-", 1, partcv, params);
// compilenode returning diff1462
  int op_slot_right_1458 = gc_frame_newslot(diff1462);
  params[0] = diff1462;
  partcv[0] = 1;
  Object opresult1464 = callmethod(*var_startPos, "..", 1, partcv, params);
// compilenode returning opresult1464
  gc_frame_newslot(opresult1464);
// Begin line 465
  setline(465);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1465 = alloc_Block(NULL, NULL, "PackageManager", 465);
  gc_frame_newslot(block1465);
  block_savedest(block1465);
  Object closure1466 = createclosure(3, "_apply");
setclosureframe(closure1466, stackframe);
  addtoclosure(closure1466, var_line);
  addtoclosure(closure1466, var_readFile);
  addtoclosure(closure1466, selfslot);
  struct UserObject *uo1466 = (struct UserObject*)block1465;
  uo1466->data[0] = (Object)closure1466;
  Method *meth_meth_PackageManager__apply1466 = addmethod2pos(block1465, "_apply", &meth_PackageManager__apply1466, 0);
int argcv_meth_PackageManager__apply1466[] = {1};
meth_meth_PackageManager__apply1466->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1466);
  meth_meth_PackageManager__apply1466->definitionModule = modulename;
  meth_meth_PackageManager__apply1466->definitionLine = 456;
// compilenode returning block1465
  gc_frame_newslot(block1465);
  params[0] = opresult1464;
  params[1] = block1465;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1473 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1457);
// compilenode returning call1473
// Begin line 465
  setline(465);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  params[0] = *var_line;
  Object call1474 = gracelib_print(NULL, 1,  params);
// compilenode returning call1474
// Begin line 489
  setline(489);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 466
  setline(466);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1478 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1479 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1478);
// compilenode returning call1479
// compilenode returning call1479
  int op_slot_left_1477 = gc_frame_newslot(call1479);
  Object num1480 = alloc_Float64(6);
// compilenode returning num1480
  int op_slot_right_1477 = gc_frame_newslot(num1480);
  params[0] = num1480;
  partcv[0] = 1;
  Object opresult1482 = callmethod(call1479, ">", 1, partcv, params);
// compilenode returning opresult1482
  int op_slot_left_1476 = gc_frame_newslot(opresult1482);
  int callframe1484 = gc_frame_new();
  Object num1485 = alloc_Float64(1);
// compilenode returning num1485
  gc_frame_newslot(num1485);
  Object num1486 = alloc_Float64(7);
// compilenode returning num1486
  gc_frame_newslot(num1486);
// compilenode returning *var_line
  params[0] = num1485;
  params[1] = num1486;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1487 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1484);
// compilenode returning call1487
  int op_slot_left_1483 = gc_frame_newslot(call1487);
  if (strlit1488 == NULL) {
    strlit1488 = alloc_String("import ");
    gc_root(strlit1488);
  }
// compilenode returning strlit1488
  int op_slot_right_1483 = gc_frame_newslot(strlit1488);
  params[0] = strlit1488;
  partcv[0] = 1;
  Object opresult1490 = callmethod(call1487, "==", 1, partcv, params);
// compilenode returning opresult1490
  int op_slot_right_1476 = gc_frame_newslot(opresult1490);
  params[0] = opresult1490;
  partcv[0] = 1;
  Object opresult1492 = callmethod(opresult1482, "&&", 1, partcv, params);
// compilenode returning opresult1492
  Object if1475 = done;
  if (istrue(opresult1492)) {
// Begin line 468
  setline(468);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 467
  setline(467);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1493 = alloc_Float64(1);
// compilenode returning num1493
  *var_pos = num1493;
  if (num1493 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 469
  setline(469);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 468
  setline(468);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  *var_start = *var_pos;
  if (*var_pos == undefined)
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
  if (strlit1494 == NULL) {
    strlit1494 = alloc_String("");
    gc_root(strlit1494);
  }
// compilenode returning strlit1494
  *var_nextImport = strlit1494;
  if (strlit1494 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 471
  setline(471);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1495 = gc_frame_new();
// Begin line 470
  setline(470);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// compilenode returning *var_pos
  gc_frame_newslot(*var_pos);
  params[0] = *var_line;
  params[1] = *var_pos;
  partcv[0] = 2;
  Object call1496 = callmethodflags(self, "skipWhiteSpace", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1495);
// compilenode returning call1496
  *var_pos = call1496;
  if (call1496 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 472
  setline(472);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 471
  setline(471);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  int op_slot_left_1498 = gc_frame_newslot(*var_pos);
  Object num1499 = alloc_Float64(1);
// compilenode returning num1499
  int op_slot_right_1498 = gc_frame_newslot(num1499);
  params[0] = num1499;
  partcv[0] = 1;
  Object sum1501 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1501
  *var_pos = sum1501;
  if (sum1501 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 472
  setline(472);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1504 == NULL) {
    strlit1504 = alloc_String("LINE = ");
    gc_root(strlit1504);
  }
// compilenode returning strlit1504
  int op_slot_left_1503 = gc_frame_newslot(strlit1504);
// compilenode returning *var_line
  int op_slot_right_1503 = gc_frame_newslot(*var_line);
  params[0] = *var_line;
  partcv[0] = 1;
  Object opresult1506 = callmethod(strlit1504, "++", 1, partcv, params);
// compilenode returning opresult1506
  params[0] = opresult1506;
  Object call1507 = gracelib_print(NULL, 1,  params);
// compilenode returning call1507
// Begin line 476
  setline(476);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1508 = gc_frame_new();
// Begin line 473
  setline(473);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1509 = alloc_Block(NULL, NULL, "PackageManager", 473);
  gc_frame_newslot(block1509);
  block_savedest(block1509);
  Object closure1510 = createclosure(3, "_apply");
setclosureframe(closure1510, stackframe);
  addtoclosure(closure1510, var_pos);
  addtoclosure(closure1510, var_line);
  addtoclosure(closure1510, selfslot);
  struct UserObject *uo1510 = (struct UserObject*)block1509;
  uo1510->data[0] = (Object)closure1510;
  Method *meth_meth_PackageManager__apply1510 = addmethod2pos(block1509, "_apply", &meth_PackageManager__apply1510, 0);
int argcv_meth_PackageManager__apply1510[] = {0};
meth_meth_PackageManager__apply1510->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1510);
  meth_meth_PackageManager__apply1510->definitionModule = modulename;
  meth_meth_PackageManager__apply1510->definitionLine = 466;
// compilenode returning block1509
  gc_frame_newslot(block1509);
// Begin line 476
  setline(476);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1535 = alloc_Block(NULL, NULL, "PackageManager", 476);
  gc_frame_newslot(block1535);
  block_savedest(block1535);
  Object closure1536 = createclosure(2, "_apply");
setclosureframe(closure1536, stackframe);
  addtoclosure(closure1536, var_pos);
  addtoclosure(closure1536, selfslot);
  struct UserObject *uo1536 = (struct UserObject*)block1535;
  uo1536->data[0] = (Object)closure1536;
  Method *meth_meth_PackageManager__apply1536 = addmethod2pos(block1535, "_apply", &meth_PackageManager__apply1536, 0);
int argcv_meth_PackageManager__apply1536[] = {0};
meth_meth_PackageManager__apply1536->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1536);
  meth_meth_PackageManager__apply1536->definitionModule = modulename;
  meth_meth_PackageManager__apply1536->definitionLine = 473;
// compilenode returning block1535
  gc_frame_newslot(block1535);
  params[0] = block1509;
  params[1] = block1535;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1543 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1508);
// compilenode returning call1543
// Begin line 476
  setline(476);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  int op_slot_left_1544 = gc_frame_newslot(*var_pos);
  Object num1545 = alloc_Float64(1);
// compilenode returning num1545
  int op_slot_right_1544 = gc_frame_newslot(num1545);
  params[0] = num1545;
  partcv[0] = 1;
  Object sum1547 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1547
  *var_pos = sum1547;
  if (sum1547 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 477
  setline(477);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1549 = gc_frame_new();
  int callframe1550 = gc_frame_new();
// compilenode returning *var_pos
  int op_slot_left_1551 = gc_frame_newslot(*var_pos);
  Object num1552 = alloc_Float64(1);
// compilenode returning num1552
  int op_slot_right_1551 = gc_frame_newslot(num1552);
  params[0] = num1552;
  partcv[0] = 1;
  Object sum1554 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1554
  gc_frame_newslot(sum1554);
  int callframe1555 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1556 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1555);
// compilenode returning call1556
// compilenode returning call1556
  gc_frame_newslot(call1556);
// compilenode returning *var_line
  params[0] = sum1554;
  params[1] = call1556;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1557 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1550);
// compilenode returning call1557
  gc_frame_newslot(call1557);
  params[0] = call1557;
  partcv[0] = 1;
  Object call1558 = callmethodflags(self, "removeExistingUrls", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1549);
// compilenode returning call1558
  *var_remainder = call1558;
  if (call1558 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 478
  setline(478);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1562 == NULL) {
    strlit1562 = alloc_String("import ""\x22""");
    gc_root(strlit1562);
  }
// compilenode returning strlit1562
  int op_slot_left_1561 = gc_frame_newslot(strlit1562);
// compilenode returning *var_prefix
  int op_slot_right_1561 = gc_frame_newslot(*var_prefix);
  params[0] = *var_prefix;
  partcv[0] = 1;
  Object opresult1564 = callmethod(strlit1562, "++", 1, partcv, params);
// compilenode returning opresult1564
  int op_slot_left_1560 = gc_frame_newslot(opresult1564);
  if (strlit1565 == NULL) {
    strlit1565 = alloc_String("/");
    gc_root(strlit1565);
  }
// compilenode returning strlit1565
  int op_slot_right_1560 = gc_frame_newslot(strlit1565);
  params[0] = strlit1565;
  partcv[0] = 1;
  Object opresult1567 = callmethod(opresult1564, "++", 1, partcv, params);
// compilenode returning opresult1567
  int op_slot_left_1559 = gc_frame_newslot(opresult1567);
// compilenode returning *var_remainder
  int op_slot_right_1559 = gc_frame_newslot(*var_remainder);
  params[0] = *var_remainder;
  partcv[0] = 1;
  Object opresult1569 = callmethod(opresult1567, "++", 1, partcv, params);
// compilenode returning opresult1569
  *var_line = opresult1569;
  if (opresult1569 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 479
  setline(479);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  params[0] = *var_line;
  Object call1571 = gracelib_print(NULL, 1,  params);
// compilenode returning call1571
// Begin line 483
  setline(483);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1572 = gc_frame_new();
// Begin line 480
  setline(480);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// Begin line 483
  setline(483);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1573 = alloc_Block(NULL, NULL, "PackageManager", 483);
  gc_frame_newslot(block1573);
  block_savedest(block1573);
  Object closure1574 = createclosure(2, "_apply");
setclosureframe(closure1574, stackframe);
  addtoclosure(closure1574, var_outFile);
  addtoclosure(closure1574, selfslot);
  struct UserObject *uo1574 = (struct UserObject*)block1573;
  uo1574->data[0] = (Object)closure1574;
  Method *meth_meth_PackageManager__apply1574 = addmethod2pos(block1573, "_apply", &meth_PackageManager__apply1574, 0);
int argcv_meth_PackageManager__apply1574[] = {1};
meth_meth_PackageManager__apply1574->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1574);
  meth_meth_PackageManager__apply1574->definitionModule = modulename;
  meth_meth_PackageManager__apply1574->definitionLine = 477;
// compilenode returning block1573
  gc_frame_newslot(block1573);
  params[0] = *var_line;
  params[1] = block1573;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1578 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1572);
// compilenode returning call1578
    gc_frame_newslot(call1578);
    if1475 = call1578;
  } else {
// Begin line 488
  setline(488);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1579 = gc_frame_new();
// Begin line 485
  setline(485);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// Begin line 488
  setline(488);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1580 = alloc_Block(NULL, NULL, "PackageManager", 488);
  gc_frame_newslot(block1580);
  block_savedest(block1580);
  Object closure1581 = createclosure(2, "_apply");
setclosureframe(closure1581, stackframe);
  addtoclosure(closure1581, var_outFile);
  addtoclosure(closure1581, selfslot);
  struct UserObject *uo1581 = (struct UserObject*)block1580;
  uo1581->data[0] = (Object)closure1581;
  Method *meth_meth_PackageManager__apply1581 = addmethod2pos(block1580, "_apply", &meth_PackageManager__apply1581, 0);
int argcv_meth_PackageManager__apply1581[] = {1};
meth_meth_PackageManager__apply1581->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1581);
  meth_meth_PackageManager__apply1581->definitionModule = modulename;
  meth_meth_PackageManager__apply1581->definitionLine = 477;
// compilenode returning block1580
  gc_frame_newslot(block1580);
  params[0] = *var_line;
  params[1] = block1580;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1585 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1579);
// compilenode returning call1585
    gc_frame_newslot(call1585);
    if1475 = call1585;
  }
// compilenode returning if1475
// Begin line 490
  setline(490);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 489
  setline(489);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1586 = gc_frame_newslot(*var_curPos);
  Object num1587 = alloc_Float64(1);
// compilenode returning num1587
  int op_slot_right_1586 = gc_frame_newslot(num1587);
  params[0] = num1587;
  partcv[0] = 1;
  Object sum1589 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1589
  *var_curPos = sum1589;
  if (sum1589 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1410(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 491
  setline(491);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1411 = gc_frame_new();
// Begin line 454
  setline(454);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1412 = alloc_Block(NULL, NULL, "PackageManager", 454);
  gc_frame_newslot(block1412);
  block_savedest(block1412);
  Object closure1413 = createclosure(3, "_apply");
setclosureframe(closure1413, stackframe);
  addtoclosure(closure1413, var_curPos);
  addtoclosure(closure1413, var_readFile);
  addtoclosure(closure1413, selfslot);
  struct UserObject *uo1413 = (struct UserObject*)block1412;
  uo1413->data[0] = (Object)closure1413;
  Method *meth_meth_PackageManager__apply1413 = addmethod2pos(block1412, "_apply", &meth_PackageManager__apply1413, 0);
int argcv_meth_PackageManager__apply1413[] = {0};
meth_meth_PackageManager__apply1413->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1413);
  meth_meth_PackageManager__apply1413->definitionModule = modulename;
  meth_meth_PackageManager__apply1413->definitionLine = 453;
// compilenode returning block1412
  gc_frame_newslot(block1412);
// Begin line 491
  setline(491);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1420 = alloc_Block(NULL, NULL, "PackageManager", 491);
  gc_frame_newslot(block1420);
  block_savedest(block1420);
  Object closure1421 = createclosure(6, "_apply");
setclosureframe(closure1421, stackframe);
  addtoclosure(closure1421, var_curPos);
  addtoclosure(closure1421, var_startPos);
  addtoclosure(closure1421, var_readFile);
  addtoclosure(closure1421, var_prefix);
  addtoclosure(closure1421, var_outFile);
  addtoclosure(closure1421, selfslot);
  struct UserObject *uo1421 = (struct UserObject*)block1420;
  uo1421->data[0] = (Object)closure1421;
  Method *meth_meth_PackageManager__apply1421 = addmethod2pos(block1420, "_apply", &meth_PackageManager__apply1421, 0);
int argcv_meth_PackageManager__apply1421[] = {0};
meth_meth_PackageManager__apply1421->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1421);
  meth_meth_PackageManager__apply1421->definitionModule = modulename;
  meth_meth_PackageManager__apply1421->definitionLine = 454;
// compilenode returning block1420
  gc_frame_newslot(block1420);
  params[0] = block1412;
  params[1] = block1420;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1592 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1411);
// compilenode returning call1592
  gc_frame_end(frame);
  return call1592;
}
Object meth_PackageManager_parseAndPrefix1399(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 450
  setline(450);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1400 = alloc_Float64(1);
// compilenode returning num1400
  *var_curPos = num1400;
  if (num1400 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 452
  setline(452);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 451
  setline(451);
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
  Object array1401 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array1401
  *var_outFile = array1401;
  if (array1401 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 494
  setline(494);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1402 = gc_frame_new();
// Begin line 453
  setline(453);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1404 = alloc_Float64(0);
// compilenode returning num1404
  int op_slot_left_1403 = gc_frame_newslot(num1404);
  int callframe1405 = gc_frame_new();
// compilenode returning *var_readFile
  partcv[0] = 0;
  Object call1406 = callmethod(*var_readFile, "size",
    1, partcv, params);
  gc_frame_end(callframe1405);
// compilenode returning call1406
// compilenode returning call1406
  int op_slot_right_1403 = gc_frame_newslot(call1406);
  params[0] = call1406;
  partcv[0] = 1;
  Object opresult1408 = callmethod(num1404, "..", 1, partcv, params);
// compilenode returning opresult1408
  gc_frame_newslot(opresult1408);
// Begin line 494
  setline(494);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1409 = alloc_Block(NULL, NULL, "PackageManager", 494);
  gc_frame_newslot(block1409);
  block_savedest(block1409);
  Object closure1410 = createclosure(6, "_apply");
setclosureframe(closure1410, stackframe);
  addtoclosure(closure1410, var_curPos);
  addtoclosure(closure1410, var_readFile);
  addtoclosure(closure1410, var_startPos);
  addtoclosure(closure1410, var_prefix);
  addtoclosure(closure1410, var_outFile);
  addtoclosure(closure1410, selfslot);
  struct UserObject *uo1410 = (struct UserObject*)block1409;
  uo1410->data[0] = (Object)closure1410;
  Method *meth_meth_PackageManager__apply1410 = addmethod2pos(block1409, "_apply", &meth_PackageManager__apply1410, 0);
int argcv_meth_PackageManager__apply1410[] = {1};
meth_meth_PackageManager__apply1410->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1410);
  meth_meth_PackageManager__apply1410->definitionModule = modulename;
  meth_meth_PackageManager__apply1410->definitionLine = 453;
// compilenode returning block1409
  gc_frame_newslot(block1409);
  params[0] = opresult1408;
  params[1] = block1409;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1594 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1402);
// compilenode returning call1594
// Begin line 495
  setline(495);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 494
  setline(494);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1596 == NULL) {
    strlit1596 = alloc_String("OUTFILE ");
    gc_root(strlit1596);
  }
// compilenode returning strlit1596
  int op_slot_left_1595 = gc_frame_newslot(strlit1596);
// compilenode returning *var_outFile
  int op_slot_right_1595 = gc_frame_newslot(*var_outFile);
  params[0] = *var_outFile;
  partcv[0] = 1;
  Object opresult1598 = callmethod(strlit1596, "++", 1, partcv, params);
// compilenode returning opresult1598
  params[0] = opresult1598;
  Object call1599 = gracelib_print(NULL, 1,  params);
// compilenode returning call1599
// Begin line 495
  setline(495);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_outFile
  return *var_outFile;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply1621(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1622 = gc_frame_newslot(*var_curPos);
  int callframe1623 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1624 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1623);
// compilenode returning call1624
// compilenode returning call1624
  int op_slot_right_1622 = gc_frame_newslot(call1624);
  params[0] = call1624;
  partcv[0] = 1;
  Object opresult1626 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1626
  gc_frame_end(frame);
  return opresult1626;
}
Object meth_PackageManager__apply1629(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 511
  setline(511);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 508
  setline(508);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1632 = callmethod(*var_importStatement, "[]", 1, partcv, params);
// compilenode returning idxres1632
  int op_slot_left_1631 = gc_frame_newslot(idxres1632);
  if (strlit1633 == NULL) {
    strlit1633 = alloc_String("/");
    gc_root(strlit1633);
  }
// compilenode returning strlit1633
  int op_slot_right_1631 = gc_frame_newslot(strlit1633);
  params[0] = strlit1633;
  partcv[0] = 1;
  Object opresult1635 = callmethod(idxres1632, "==", 1, partcv, params);
// compilenode returning opresult1635
  Object if1630 = done;
  if (istrue(opresult1635)) {
// Begin line 509
  setline(509);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_lastIndex = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if1630 = done;
  } else {
  }
// compilenode returning if1630
// Begin line 511
  setline(511);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1637 = gc_frame_newslot(*var_curPos);
  Object num1638 = alloc_Float64(1);
// compilenode returning num1638
  int op_slot_right_1637 = gc_frame_newslot(num1638);
  params[0] = num1638;
  partcv[0] = 1;
  Object sum1640 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1640
  *var_curPos = sum1640;
  if (sum1640 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_removeExistingUrls1600(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 504
  setline(504);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 501
  setline(501);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1603 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1604 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1603);
// compilenode returning call1604
// compilenode returning call1604
  int op_slot_left_1602 = gc_frame_newslot(call1604);
  Object num1605 = alloc_Float64(7);
// compilenode returning num1605
  int op_slot_right_1602 = gc_frame_newslot(num1605);
  params[0] = num1605;
  partcv[0] = 1;
  Object opresult1607 = callmethod(call1604, "<", 1, partcv, params);
// compilenode returning opresult1607
  Object if1601 = done;
  if (istrue(opresult1607)) {
// Begin line 502
  setline(502);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
  return *var_importStatement;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1601 = undefined;
  } else {
  }
// compilenode returning if1601
// Begin line 521
  setline(521);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 504
  setline(504);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1610 = gc_frame_new();
  Object num1611 = alloc_Float64(1);
// compilenode returning num1611
  gc_frame_newslot(num1611);
  Object num1612 = alloc_Float64(7);
// compilenode returning num1612
  gc_frame_newslot(num1612);
// compilenode returning *var_importStatement
  params[0] = num1611;
  params[1] = num1612;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1613 = callmethod(*var_importStatement, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1610);
// compilenode returning call1613
  int op_slot_left_1609 = gc_frame_newslot(call1613);
  if (strlit1614 == NULL) {
    strlit1614 = alloc_String("http://");
    gc_root(strlit1614);
  }
// compilenode returning strlit1614
  int op_slot_right_1609 = gc_frame_newslot(strlit1614);
  params[0] = strlit1614;
  partcv[0] = 1;
  Object opresult1616 = callmethod(call1613, "==", 1, partcv, params);
// compilenode returning opresult1616
  Object if1608 = done;
  if (istrue(opresult1616)) {
// Begin line 505
  setline(505);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1617 = alloc_Float64(7);
// compilenode returning num1617
  *var_lastIndex = num1617;
  if (num1617 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 506
  setline(506);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1618 = alloc_Float64(7);
// compilenode returning num1618
  *var_curPos = num1618;
  if (num1618 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 513
  setline(513);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1619 = gc_frame_new();
// Begin line 507
  setline(507);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1620 = alloc_Block(NULL, NULL, "PackageManager", 507);
  gc_frame_newslot(block1620);
  block_savedest(block1620);
  Object closure1621 = createclosure(3, "_apply");
setclosureframe(closure1621, stackframe);
  addtoclosure(closure1621, var_curPos);
  addtoclosure(closure1621, var_importStatement);
  addtoclosure(closure1621, selfslot);
  struct UserObject *uo1621 = (struct UserObject*)block1620;
  uo1621->data[0] = (Object)closure1621;
  Method *meth_meth_PackageManager__apply1621 = addmethod2pos(block1620, "_apply", &meth_PackageManager__apply1621, 0);
int argcv_meth_PackageManager__apply1621[] = {0};
meth_meth_PackageManager__apply1621->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1621);
  meth_meth_PackageManager__apply1621->definitionModule = modulename;
  meth_meth_PackageManager__apply1621->definitionLine = 501;
// compilenode returning block1620
  gc_frame_newslot(block1620);
// Begin line 513
  setline(513);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1628 = alloc_Block(NULL, NULL, "PackageManager", 513);
  gc_frame_newslot(block1628);
  block_savedest(block1628);
  Object closure1629 = createclosure(4, "_apply");
setclosureframe(closure1629, stackframe);
  addtoclosure(closure1629, var_importStatement);
  addtoclosure(closure1629, var_curPos);
  addtoclosure(closure1629, var_lastIndex);
  addtoclosure(closure1629, selfslot);
  struct UserObject *uo1629 = (struct UserObject*)block1628;
  uo1629->data[0] = (Object)closure1629;
  Method *meth_meth_PackageManager__apply1629 = addmethod2pos(block1628, "_apply", &meth_PackageManager__apply1629, 0);
int argcv_meth_PackageManager__apply1629[] = {0};
meth_meth_PackageManager__apply1629->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1629);
  meth_meth_PackageManager__apply1629->definitionModule = modulename;
  meth_meth_PackageManager__apply1629->definitionLine = 507;
// compilenode returning block1628
  gc_frame_newslot(block1628);
  params[0] = block1620;
  params[1] = block1628;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1643 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1619);
// compilenode returning call1643
// Begin line 513
  setline(513);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1644 = gc_frame_new();
// compilenode returning *var_lastIndex
  int op_slot_left_1645 = gc_frame_newslot(*var_lastIndex);
  Object num1646 = alloc_Float64(1);
// compilenode returning num1646
  int op_slot_right_1645 = gc_frame_newslot(num1646);
  params[0] = num1646;
  partcv[0] = 1;
  Object sum1648 = callmethod(*var_lastIndex, "+", 1, partcv, params);
// compilenode returning sum1648
  gc_frame_newslot(sum1648);
  int callframe1649 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call1650 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe1649);
// compilenode returning call1650
// compilenode returning call1650
  gc_frame_newslot(call1650);
// compilenode returning *var_importStatement
  params[0] = sum1648;
  params[1] = call1650;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1651 = callmethod(*var_importStatement, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1644);
// compilenode returning call1651
  *var_res = call1651;
  if (call1651 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 514
  setline(514);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_res
  return *var_res;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1608 = undefined;
  } else {
// Begin line 517
  setline(517);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
  return *var_importStatement;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1608 = undefined;
  }
// compilenode returning if1608
  gc_frame_end(frame);
  return if1608;
}
Object meth_PackageManager__apply1659(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1660 = gc_frame_newslot(*var_count);
  int callframe1661 = gc_frame_new();
// compilenode returning *var_st
  partcv[0] = 0;
  Object call1662 = callmethod(*var_st, "size",
    1, partcv, params);
  gc_frame_end(callframe1661);
// compilenode returning call1662
// compilenode returning call1662
  int op_slot_right_1660 = gc_frame_newslot(call1662);
  params[0] = call1662;
  partcv[0] = 1;
  Object opresult1664 = callmethod(*var_count, "<=", 1, partcv, params);
// compilenode returning opresult1664
  gc_frame_end(frame);
  return opresult1664;
}
Object meth_PackageManager__apply1667(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 530
  setline(530);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 527
  setline(527);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_st
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1670 = callmethod(*var_st, "[]", 1, partcv, params);
// compilenode returning idxres1670
  int op_slot_left_1669 = gc_frame_newslot(idxres1670);
  if (strlit1671 == NULL) {
    strlit1671 = alloc_String("/");
    gc_root(strlit1671);
  }
// compilenode returning strlit1671
  int op_slot_right_1669 = gc_frame_newslot(strlit1671);
  params[0] = strlit1671;
  partcv[0] = 1;
  Object opresult1673 = callmethod(idxres1670, "==", 1, partcv, params);
// compilenode returning opresult1673
  Object if1668 = done;
  if (istrue(opresult1673)) {
// Begin line 528
  setline(528);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  *var_lastIndex = *var_count;
  if (*var_count == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if1668 = done;
  } else {
  }
// compilenode returning if1668
// Begin line 530
  setline(530);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  int op_slot_left_1675 = gc_frame_newslot(*var_count);
  Object num1676 = alloc_Float64(1);
// compilenode returning num1676
  int op_slot_right_1675 = gc_frame_newslot(num1676);
  params[0] = num1676;
  partcv[0] = 1;
  Object sum1678 = callmethod(*var_count, "+", 1, partcv, params);
// compilenode returning sum1678
  *var_count = sum1678;
  if (sum1678 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_removeContainingDir1652(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 524
  setline(524);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1653 = alloc_Float64(1);
// compilenode returning num1653
  *var_count = num1653;
  if (num1653 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 525
  setline(525);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1654 = gc_frame_new();
  Object num1655 = alloc_Float64(1);
// compilenode returning num1655
  partcv[0] = 0;
  Object call1656 = callmethod(num1655, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1654);
// compilenode returning call1656
  *var_lastIndex = call1656;
  if (call1656 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 532
  setline(532);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1657 = gc_frame_new();
// Begin line 526
  setline(526);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1658 = alloc_Block(NULL, NULL, "PackageManager", 526);
  gc_frame_newslot(block1658);
  block_savedest(block1658);
  Object closure1659 = createclosure(3, "_apply");
setclosureframe(closure1659, stackframe);
  addtoclosure(closure1659, var_count);
  addtoclosure(closure1659, var_st);
  addtoclosure(closure1659, selfslot);
  struct UserObject *uo1659 = (struct UserObject*)block1658;
  uo1659->data[0] = (Object)closure1659;
  Method *meth_meth_PackageManager__apply1659 = addmethod2pos(block1658, "_apply", &meth_PackageManager__apply1659, 0);
int argcv_meth_PackageManager__apply1659[] = {0};
meth_meth_PackageManager__apply1659->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1659);
  meth_meth_PackageManager__apply1659->definitionModule = modulename;
  meth_meth_PackageManager__apply1659->definitionLine = 513;
// compilenode returning block1658
  gc_frame_newslot(block1658);
// Begin line 532
  setline(532);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1666 = alloc_Block(NULL, NULL, "PackageManager", 532);
  gc_frame_newslot(block1666);
  block_savedest(block1666);
  Object closure1667 = createclosure(4, "_apply");
setclosureframe(closure1667, stackframe);
  addtoclosure(closure1667, var_st);
  addtoclosure(closure1667, var_count);
  addtoclosure(closure1667, var_lastIndex);
  addtoclosure(closure1667, selfslot);
  struct UserObject *uo1667 = (struct UserObject*)block1666;
  uo1667->data[0] = (Object)closure1667;
  Method *meth_meth_PackageManager__apply1667 = addmethod2pos(block1666, "_apply", &meth_PackageManager__apply1667, 0);
int argcv_meth_PackageManager__apply1667[] = {0};
meth_meth_PackageManager__apply1667->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1667);
  meth_meth_PackageManager__apply1667->definitionModule = modulename;
  meth_meth_PackageManager__apply1667->definitionLine = 526;
// compilenode returning block1666
  gc_frame_newslot(block1666);
  params[0] = block1658;
  params[1] = block1666;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1681 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1657);
// compilenode returning call1681
// Begin line 535
  setline(535);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 532
  setline(532);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_lastIndex
  int op_slot_left_1683 = gc_frame_newslot(*var_lastIndex);
  int callframe1684 = gc_frame_new();
  Object num1685 = alloc_Float64(1);
// compilenode returning num1685
  partcv[0] = 0;
  Object call1686 = callmethod(num1685, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1684);
// compilenode returning call1686
  int op_slot_right_1683 = gc_frame_newslot(call1686);
  params[0] = call1686;
  partcv[0] = 1;
  Object opresult1688 = callmethod(*var_lastIndex, "==", 1, partcv, params);
// compilenode returning opresult1688
  Object if1682 = done;
  if (istrue(opresult1688)) {
// Begin line 533
  setline(533);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_st
  return *var_st;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1682 = undefined;
  } else {
  }
// compilenode returning if1682
// Begin line 535
  setline(535);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1689 = gc_frame_new();
// compilenode returning *var_lastIndex
  int op_slot_left_1690 = gc_frame_newslot(*var_lastIndex);
  Object num1691 = alloc_Float64(1);
// compilenode returning num1691
  int op_slot_right_1690 = gc_frame_newslot(num1691);
  params[0] = num1691;
  partcv[0] = 1;
  Object sum1693 = callmethod(*var_lastIndex, "+", 1, partcv, params);
// compilenode returning sum1693
  gc_frame_newslot(sum1693);
  int callframe1694 = gc_frame_new();
// compilenode returning *var_st
  partcv[0] = 0;
  Object call1695 = callmethod(*var_st, "size",
    1, partcv, params);
  gc_frame_end(callframe1694);
// compilenode returning call1695
// compilenode returning call1695
  gc_frame_newslot(call1695);
// compilenode returning *var_st
  params[0] = sum1693;
  params[1] = call1695;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1696 = callmethod(*var_st, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1689);
// compilenode returning call1696
  return call1696;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply1704(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1705 = gc_frame_newslot(*var_count);
  int callframe1706 = gc_frame_new();
// compilenode returning *var_st
  partcv[0] = 0;
  Object call1707 = callmethod(*var_st, "size",
    1, partcv, params);
  gc_frame_end(callframe1706);
// compilenode returning call1707
// compilenode returning call1707
  int op_slot_right_1705 = gc_frame_newslot(call1707);
  params[0] = call1707;
  partcv[0] = 1;
  Object opresult1709 = callmethod(*var_count, "<=", 1, partcv, params);
// compilenode returning opresult1709
  gc_frame_end(frame);
  return opresult1709;
}
Object meth_PackageManager__apply1712(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 545
  setline(545);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 542
  setline(542);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_st
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1715 = callmethod(*var_st, "[]", 1, partcv, params);
// compilenode returning idxres1715
  int op_slot_left_1714 = gc_frame_newslot(idxres1715);
  if (strlit1716 == NULL) {
    strlit1716 = alloc_String("/");
    gc_root(strlit1716);
  }
// compilenode returning strlit1716
  int op_slot_right_1714 = gc_frame_newslot(strlit1716);
  params[0] = strlit1716;
  partcv[0] = 1;
  Object opresult1718 = callmethod(idxres1715, "==", 1, partcv, params);
// compilenode returning opresult1718
  Object if1713 = done;
  if (istrue(opresult1718)) {
// Begin line 543
  setline(543);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  *var_last = *var_count;
  if (*var_count == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if1713 = done;
  } else {
  }
// compilenode returning if1713
// Begin line 545
  setline(545);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  int op_slot_left_1720 = gc_frame_newslot(*var_count);
  Object num1721 = alloc_Float64(1);
// compilenode returning num1721
  int op_slot_right_1720 = gc_frame_newslot(num1721);
  params[0] = num1721;
  partcv[0] = 1;
  Object sum1723 = callmethod(*var_count, "+", 1, partcv, params);
// compilenode returning sum1723
  *var_count = sum1723;
  if (sum1723 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_getContainingDirectory1697(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 539
  setline(539);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1698 = alloc_Float64(1);
// compilenode returning num1698
  *var_count = num1698;
  if (num1698 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 540
  setline(540);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1699 = gc_frame_new();
  Object num1700 = alloc_Float64(1);
// compilenode returning num1700
  partcv[0] = 0;
  Object call1701 = callmethod(num1700, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1699);
// compilenode returning call1701
  *var_last = call1701;
  if (call1701 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 547
  setline(547);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1702 = gc_frame_new();
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1703 = alloc_Block(NULL, NULL, "PackageManager", 541);
  gc_frame_newslot(block1703);
  block_savedest(block1703);
  Object closure1704 = createclosure(3, "_apply");
setclosureframe(closure1704, stackframe);
  addtoclosure(closure1704, var_count);
  addtoclosure(closure1704, var_st);
  addtoclosure(closure1704, selfslot);
  struct UserObject *uo1704 = (struct UserObject*)block1703;
  uo1704->data[0] = (Object)closure1704;
  Method *meth_meth_PackageManager__apply1704 = addmethod2pos(block1703, "_apply", &meth_PackageManager__apply1704, 0);
int argcv_meth_PackageManager__apply1704[] = {0};
meth_meth_PackageManager__apply1704->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1704);
  meth_meth_PackageManager__apply1704->definitionModule = modulename;
  meth_meth_PackageManager__apply1704->definitionLine = 535;
// compilenode returning block1703
  gc_frame_newslot(block1703);
// Begin line 547
  setline(547);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1711 = alloc_Block(NULL, NULL, "PackageManager", 547);
  gc_frame_newslot(block1711);
  block_savedest(block1711);
  Object closure1712 = createclosure(4, "_apply");
setclosureframe(closure1712, stackframe);
  addtoclosure(closure1712, var_st);
  addtoclosure(closure1712, var_count);
  addtoclosure(closure1712, var_last);
  addtoclosure(closure1712, selfslot);
  struct UserObject *uo1712 = (struct UserObject*)block1711;
  uo1712->data[0] = (Object)closure1712;
  Method *meth_meth_PackageManager__apply1712 = addmethod2pos(block1711, "_apply", &meth_PackageManager__apply1712, 0);
int argcv_meth_PackageManager__apply1712[] = {0};
meth_meth_PackageManager__apply1712->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1712);
  meth_meth_PackageManager__apply1712->definitionModule = modulename;
  meth_meth_PackageManager__apply1712->definitionLine = 541;
// compilenode returning block1711
  gc_frame_newslot(block1711);
  params[0] = block1703;
  params[1] = block1711;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1726 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1702);
// compilenode returning call1726
// Begin line 550
  setline(550);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 547
  setline(547);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_last
  int op_slot_left_1728 = gc_frame_newslot(*var_last);
  int callframe1729 = gc_frame_new();
  Object num1730 = alloc_Float64(1);
// compilenode returning num1730
  partcv[0] = 0;
  Object call1731 = callmethod(num1730, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe1729);
// compilenode returning call1731
  int op_slot_right_1728 = gc_frame_newslot(call1731);
  params[0] = call1731;
  partcv[0] = 1;
  Object opresult1733 = callmethod(*var_last, "==", 1, partcv, params);
// compilenode returning opresult1733
  Object if1727 = done;
  if (istrue(opresult1733)) {
// Begin line 548
  setline(548);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1735 = gc_frame_new();
  partcv[0] = 0;
  Object call1736 = callmethodflags(self, "getBuildPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1735);
// compilenode returning call1736
  int op_slot_left_1734 = gc_frame_newslot(call1736);
  if (strlit1737 == NULL) {
    strlit1737 = alloc_String("/");
    gc_root(strlit1737);
  }
// compilenode returning strlit1737
  int op_slot_right_1734 = gc_frame_newslot(strlit1737);
  params[0] = strlit1737;
  partcv[0] = 1;
  Object opresult1739 = callmethod(call1736, "++", 1, partcv, params);
// compilenode returning opresult1739
  return opresult1739;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1727 = undefined;
  } else {
  }
// compilenode returning if1727
// Begin line 550
  setline(550);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1740 = gc_frame_new();
  Object num1741 = alloc_Float64(0);
// compilenode returning num1741
  gc_frame_newslot(num1741);
// compilenode returning *var_last
  int op_slot_left_1742 = gc_frame_newslot(*var_last);
  Object num1743 = alloc_Float64(1);
// compilenode returning num1743
  int op_slot_right_1742 = gc_frame_newslot(num1743);
  params[0] = num1743;
  partcv[0] = 1;
  Object diff1745 = callmethod(*var_last, "-", 1, partcv, params);
// compilenode returning diff1745
  gc_frame_newslot(diff1745);
// compilenode returning *var_st
  params[0] = num1741;
  params[1] = diff1745;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1746 = callmethod(*var_st, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1740);
// compilenode returning call1746
  return call1746;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_imported1747(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
Object meth_PackageManager_imported_58__61_1748(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
Object meth_PackageManager_toProcess1750(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
Object meth_PackageManager_toProcess_58__61_1751(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
Object meth_PackageManager_verbose1754(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
Object meth_PackageManager_verbose_58__61_1755(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
Object meth_PackageManager_global1758(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
Object meth_PackageManager_global_58__61_1759(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
Object meth_PackageManager_bundlePath1762(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
Object meth_PackageManager_bundlePath_58__61_1763(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
Object meth_PackageManager_curFile1765(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
Object meth_PackageManager_curFile_58__61_1766(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 46
  setline(46);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo60 = (struct UserObject*)self;
  uo60->data[2] = emptyclosure;
  Method *meth_meth_PackageManager_forArgs_40__41_do60 = addmethod2pos(self, "forArgs()do", &meth_PackageManager_forArgs_40__41_do60, 2);
int argcv_meth_PackageManager_forArgs_40__41_do60[] = {1, 1};
meth_meth_PackageManager_forArgs_40__41_do60->type = alloc_MethodType(2, argcv_meth_PackageManager_forArgs_40__41_do60);
  meth_meth_PackageManager_forArgs_40__41_do60->flags |= MFLAG_CONFIDENTIAL;
  meth_meth_PackageManager_forArgs_40__41_do60->definitionModule = modulename;
  meth_meth_PackageManager_forArgs_40__41_do60->definitionLine = 46;
  Method *meth_meth_PackageManager_forArgs_40__41_do60_object = addmethod2pos(self, "forArgs()do()object", &meth_PackageManager_forArgs_40__41_do60_object, 2);
int argcv_meth_PackageManager_forArgs_40__41_do60_object[] = {1, 1};
meth_meth_PackageManager_forArgs_40__41_do60_object->type = alloc_MethodType(2, argcv_meth_PackageManager_forArgs_40__41_do60_object);
  meth_meth_PackageManager_forArgs_40__41_do60_object->flags |= MFLAG_CONFIDENTIAL;
  meth_meth_PackageManager_forArgs_40__41_do60_object->definitionModule = modulename;
  meth_meth_PackageManager_forArgs_40__41_do60_object->definitionLine = 46;
// compilenode returning 
// Begin line 113
  setline(113);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure286 = createclosure(2, "listInstalled");
setclosureframe(closure286, stackframe);
  addtoclosure(closure286, var_io);
  addtoclosure(closure286, var_sys);
  struct UserObject *uo286 = (struct UserObject*)self;
  uo286->data[3] = (Object)closure286;
  Method *meth_meth_PackageManager_listInstalled286 = addmethod2pos(self, "listInstalled", &meth_PackageManager_listInstalled286, 3);
int argcv_meth_PackageManager_listInstalled286[] = {0};
meth_meth_PackageManager_listInstalled286->type = alloc_MethodType(1, argcv_meth_PackageManager_listInstalled286);
  meth_meth_PackageManager_listInstalled286->definitionModule = modulename;
  meth_meth_PackageManager_listInstalled286->definitionLine = 113;
// compilenode returning 
// Begin line 126
  setline(126);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure321 = createclosure(1, "recurseDirectory");
setclosureframe(closure321, stackframe);
  addtoclosure(closure321, var_io);
  struct UserObject *uo321 = (struct UserObject*)self;
  uo321->data[4] = (Object)closure321;
  Method *meth_meth_PackageManager_recurseDirectory321 = addmethod2pos(self, "recurseDirectory", &meth_PackageManager_recurseDirectory321, 4);
int argcv_meth_PackageManager_recurseDirectory321[] = {2};
meth_meth_PackageManager_recurseDirectory321->type = alloc_MethodType(1, argcv_meth_PackageManager_recurseDirectory321);
  meth_meth_PackageManager_recurseDirectory321->definitionModule = modulename;
  meth_meth_PackageManager_recurseDirectory321->definitionLine = 126;
// compilenode returning 
// Begin line 138
  setline(138);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure374 = createclosure(1, "doGet");
setclosureframe(closure374, stackframe);
  addtoclosure(closure374, var_imported);
  struct UserObject *uo374 = (struct UserObject*)self;
  uo374->data[5] = (Object)closure374;
  Method *meth_meth_PackageManager_doGet374 = addmethod2pos(self, "doGet", &meth_PackageManager_doGet374, 5);
int argcv_meth_PackageManager_doGet374[] = {1};
meth_meth_PackageManager_doGet374->type = alloc_MethodType(1, argcv_meth_PackageManager_doGet374);
  meth_meth_PackageManager_doGet374->definitionModule = modulename;
  meth_meth_PackageManager_doGet374->definitionLine = 138;
// compilenode returning 
// Begin line 149
  setline(149);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure393 = createclosure(1, "setFile");
setclosureframe(closure393, stackframe);
  addtoclosure(closure393, var_curFile);
  struct UserObject *uo393 = (struct UserObject*)self;
  uo393->data[6] = (Object)closure393;
  Method *meth_meth_PackageManager_setFile393 = addmethod2pos(self, "setFile", &meth_PackageManager_setFile393, 6);
int argcv_meth_PackageManager_setFile393[] = {1};
meth_meth_PackageManager_setFile393->type = alloc_MethodType(1, argcv_meth_PackageManager_setFile393);
  meth_meth_PackageManager_setFile393->definitionModule = modulename;
  meth_meth_PackageManager_setFile393->definitionLine = 149;
// compilenode returning 
// Begin line 161
  setline(161);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure416 = createclosure(3, "fetchImports");
setclosureframe(closure416, stackframe);
  addtoclosure(closure416, var_curFile);
  addtoclosure(closure416, var_imported);
  addtoclosure(closure416, var_toProcess);
  struct UserObject *uo416 = (struct UserObject*)self;
  uo416->data[7] = (Object)closure416;
  Method *meth_meth_PackageManager_fetchImports416 = addmethod2pos(self, "fetchImports", &meth_PackageManager_fetchImports416, 7);
int argcv_meth_PackageManager_fetchImports416[] = {1};
meth_meth_PackageManager_fetchImports416->type = alloc_MethodType(1, argcv_meth_PackageManager_fetchImports416);
  meth_meth_PackageManager_fetchImports416->definitionModule = modulename;
  meth_meth_PackageManager_fetchImports416->definitionLine = 161;
// compilenode returning 
// Begin line 186
  setline(186);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure483 = createclosure(1, "setFileData");
setclosureframe(closure483, stackframe);
  addtoclosure(closure483, var_curl);
  struct UserObject *uo483 = (struct UserObject*)self;
  uo483->data[8] = (Object)closure483;
  Method *meth_meth_PackageManager_setFileData483 = addmethod2pos(self, "setFileData", &meth_PackageManager_setFileData483, 8);
int argcv_meth_PackageManager_setFileData483[] = {1};
meth_meth_PackageManager_setFileData483->type = alloc_MethodType(1, argcv_meth_PackageManager_setFileData483);
  meth_meth_PackageManager_setFileData483->definitionModule = modulename;
  meth_meth_PackageManager_setFileData483->definitionLine = 186;
// compilenode returning 
// Begin line 222
  setline(222);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure602 = createclosure(3, "findExisting");
setclosureframe(closure602, stackframe);
  addtoclosure(closure602, var_io);
  addtoclosure(closure602, var_sys);
  addtoclosure(closure602, var_bundlePath);
  struct UserObject *uo602 = (struct UserObject*)self;
  uo602->data[9] = (Object)closure602;
  Method *meth_meth_PackageManager_findExisting602 = addmethod2pos(self, "findExisting", &meth_PackageManager_findExisting602, 9);
int argcv_meth_PackageManager_findExisting602[] = {1};
meth_meth_PackageManager_findExisting602->type = alloc_MethodType(1, argcv_meth_PackageManager_findExisting602);
  meth_meth_PackageManager_findExisting602->definitionModule = modulename;
  meth_meth_PackageManager_findExisting602->definitionLine = 222;
// compilenode returning 
// Begin line 249
  setline(249);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo755 = (struct UserObject*)self;
  uo755->data[10] = emptyclosure;
  Method *meth_meth_PackageManager_validateFile755 = addmethod2pos(self, "validateFile", &meth_PackageManager_validateFile755, 10);
int argcv_meth_PackageManager_validateFile755[] = {1};
meth_meth_PackageManager_validateFile755->type = alloc_MethodType(1, argcv_meth_PackageManager_validateFile755);
  meth_meth_PackageManager_validateFile755->definitionModule = modulename;
  meth_meth_PackageManager_validateFile755->definitionLine = 249;
// compilenode returning 
// Begin line 259
  setline(259);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure778 = createclosure(3, "write");
setclosureframe(closure778, stackframe);
  addtoclosure(closure778, var_global);
  addtoclosure(closure778, var_sys);
  addtoclosure(closure778, var_io);
  struct UserObject *uo778 = (struct UserObject*)self;
  uo778->data[11] = (Object)closure778;
  Method *meth_meth_PackageManager_write778 = addmethod2pos(self, "write", &meth_PackageManager_write778, 11);
int argcv_meth_PackageManager_write778[] = {1};
meth_meth_PackageManager_write778->type = alloc_MethodType(1, argcv_meth_PackageManager_write778);
  meth_meth_PackageManager_write778->definitionModule = modulename;
  meth_meth_PackageManager_write778->definitionLine = 259;
// compilenode returning 
// Begin line 274
  setline(274);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure815 = createclosure(2, "getBuildPath");
setclosureframe(closure815, stackframe);
  addtoclosure(closure815, var_sys);
  addtoclosure(closure815, var_io);
  struct UserObject *uo815 = (struct UserObject*)self;
  uo815->data[12] = (Object)closure815;
  Method *meth_meth_PackageManager_getBuildPath815 = addmethod2pos(self, "getBuildPath", &meth_PackageManager_getBuildPath815, 12);
int argcv_meth_PackageManager_getBuildPath815[] = {0};
meth_meth_PackageManager_getBuildPath815->type = alloc_MethodType(1, argcv_meth_PackageManager_getBuildPath815);
  meth_meth_PackageManager_getBuildPath815->definitionModule = modulename;
  meth_meth_PackageManager_getBuildPath815->definitionLine = 274;
// compilenode returning 
// Begin line 291
  setline(291);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure853 = createclosure(3, "compile");
setclosureframe(closure853, stackframe);
  addtoclosure(closure853, var_global);
  addtoclosure(closure853, var_sys);
  addtoclosure(closure853, var_io);
  struct UserObject *uo853 = (struct UserObject*)self;
  uo853->data[13] = (Object)closure853;
  Method *meth_meth_PackageManager_compile853 = addmethod2pos(self, "compile", &meth_PackageManager_compile853, 13);
int argcv_meth_PackageManager_compile853[] = {1};
meth_meth_PackageManager_compile853->type = alloc_MethodType(1, argcv_meth_PackageManager_compile853);
  meth_meth_PackageManager_compile853->definitionModule = modulename;
  meth_meth_PackageManager_compile853->definitionLine = 291;
// compilenode returning 
// Begin line 307
  setline(307);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure893 = createclosure(1, "createDirectory");
setclosureframe(closure893, stackframe);
  addtoclosure(closure893, var_io);
  struct UserObject *uo893 = (struct UserObject*)self;
  uo893->data[14] = (Object)closure893;
  Method *meth_meth_PackageManager_createDirectory893 = addmethod2pos(self, "createDirectory", &meth_PackageManager_createDirectory893, 14);
int argcv_meth_PackageManager_createDirectory893[] = {1};
meth_meth_PackageManager_createDirectory893->type = alloc_MethodType(1, argcv_meth_PackageManager_createDirectory893);
  meth_meth_PackageManager_createDirectory893->definitionModule = modulename;
  meth_meth_PackageManager_createDirectory893->definitionLine = 307;
// compilenode returning 
// Begin line 331
  setline(331);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure971 = createclosure(1, "parseFile");
setclosureframe(closure971, stackframe);
  addtoclosure(closure971, var_done);
  struct UserObject *uo971 = (struct UserObject*)self;
  uo971->data[15] = (Object)closure971;
  Method *meth_meth_PackageManager_parseFile971 = addmethod2pos(self, "parseFile", &meth_PackageManager_parseFile971, 15);
int argcv_meth_PackageManager_parseFile971[] = {1};
meth_meth_PackageManager_parseFile971->type = alloc_MethodType(1, argcv_meth_PackageManager_parseFile971);
  meth_meth_PackageManager_parseFile971->definitionModule = modulename;
  meth_meth_PackageManager_parseFile971->definitionLine = 331;
// compilenode returning 
// Begin line 348
  setline(348);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1031 = (struct UserObject*)self;
  uo1031->data[16] = emptyclosure;
  Method *meth_meth_PackageManager_processLine1031 = addmethod2pos(self, "processLine", &meth_PackageManager_processLine1031, 16);
int argcv_meth_PackageManager_processLine1031[] = {1};
meth_meth_PackageManager_processLine1031->type = alloc_MethodType(1, argcv_meth_PackageManager_processLine1031);
  meth_meth_PackageManager_processLine1031->definitionModule = modulename;
  meth_meth_PackageManager_processLine1031->definitionLine = 348;
// compilenode returning 
// Begin line 367
  setline(367);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1112 = createclosure(2, "parseImport");
setclosureframe(closure1112, stackframe);
  addtoclosure(closure1112, var_imported);
  addtoclosure(closure1112, var_toProcess);
  struct UserObject *uo1112 = (struct UserObject*)self;
  uo1112->data[17] = (Object)closure1112;
  Method *meth_meth_PackageManager_parseImport1112 = addmethod2pos(self, "parseImport", &meth_PackageManager_parseImport1112, 17);
int argcv_meth_PackageManager_parseImport1112[] = {1};
meth_meth_PackageManager_parseImport1112->type = alloc_MethodType(1, argcv_meth_PackageManager_parseImport1112);
  meth_meth_PackageManager_parseImport1112->definitionModule = modulename;
  meth_meth_PackageManager_parseImport1112->definitionLine = 367;
// compilenode returning 
// Begin line 384
  setline(384);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1183 = (struct UserObject*)self;
  uo1183->data[18] = emptyclosure;
  Method *meth_meth_PackageManager_skipWhiteSpace1183 = addmethod2pos(self, "skipWhiteSpace", &meth_PackageManager_skipWhiteSpace1183, 18);
int argcv_meth_PackageManager_skipWhiteSpace1183[] = {2};
meth_meth_PackageManager_skipWhiteSpace1183->type = alloc_MethodType(1, argcv_meth_PackageManager_skipWhiteSpace1183);
  meth_meth_PackageManager_skipWhiteSpace1183->definitionModule = modulename;
  meth_meth_PackageManager_skipWhiteSpace1183->definitionLine = 384;
// compilenode returning 
// Begin line 392
  setline(392);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1210 = (struct UserObject*)self;
  uo1210->data[19] = emptyclosure;
  Method *meth_meth_PackageManager_displayHelp1210 = addmethod2pos(self, "displayHelp", &meth_PackageManager_displayHelp1210, 19);
int argcv_meth_PackageManager_displayHelp1210[] = {0};
meth_meth_PackageManager_displayHelp1210->type = alloc_MethodType(1, argcv_meth_PackageManager_displayHelp1210);
  meth_meth_PackageManager_displayHelp1210->definitionModule = modulename;
  meth_meth_PackageManager_displayHelp1210->definitionLine = 392;
// compilenode returning 
// Begin line 398
  setline(398);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1217 = createclosure(1, "printMessage");
setclosureframe(closure1217, stackframe);
  addtoclosure(closure1217, var_verbose);
  struct UserObject *uo1217 = (struct UserObject*)self;
  uo1217->data[20] = (Object)closure1217;
  Method *meth_meth_PackageManager_printMessage1217 = addmethod2pos(self, "printMessage", &meth_PackageManager_printMessage1217, 20);
int argcv_meth_PackageManager_printMessage1217[] = {1};
meth_meth_PackageManager_printMessage1217->type = alloc_MethodType(1, argcv_meth_PackageManager_printMessage1217);
  meth_meth_PackageManager_printMessage1217->definitionModule = modulename;
  meth_meth_PackageManager_printMessage1217->definitionLine = 398;
// compilenode returning 
// Begin line 407
  setline(407);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1220 = createclosure(3, "bundle");
setclosureframe(closure1220, stackframe);
  addtoclosure(closure1220, var_imported);
  addtoclosure(closure1220, var_bundlePath);
  addtoclosure(closure1220, var_io);
  struct UserObject *uo1220 = (struct UserObject*)self;
  uo1220->data[21] = (Object)closure1220;
  Method *meth_meth_PackageManager_bundle1220 = addmethod2pos(self, "bundle", &meth_PackageManager_bundle1220, 21);
int argcv_meth_PackageManager_bundle1220[] = {2};
meth_meth_PackageManager_bundle1220->type = alloc_MethodType(1, argcv_meth_PackageManager_bundle1220);
  meth_meth_PackageManager_bundle1220->definitionModule = modulename;
  meth_meth_PackageManager_bundle1220->definitionLine = 407;
// compilenode returning 
// Begin line 425
  setline(425);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1323 = createclosure(1, "setImportDest");
setclosureframe(closure1323, stackframe);
  addtoclosure(closure1323, var_io);
  struct UserObject *uo1323 = (struct UserObject*)self;
  uo1323->data[22] = (Object)closure1323;
  Method *meth_meth_PackageManager_setImportDest1323 = addmethod2pos(self, "setImportDest", &meth_PackageManager_setImportDest1323, 22);
int argcv_meth_PackageManager_setImportDest1323[] = {2};
meth_meth_PackageManager_setImportDest1323->type = alloc_MethodType(1, argcv_meth_PackageManager_setImportDest1323);
  meth_meth_PackageManager_setImportDest1323->definitionModule = modulename;
  meth_meth_PackageManager_setImportDest1323->definitionLine = 425;
// compilenode returning 
// Begin line 449
  setline(449);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1399 = (struct UserObject*)self;
  uo1399->data[23] = emptyclosure;
  Method *meth_meth_PackageManager_parseAndPrefix1399 = addmethod2pos(self, "parseAndPrefix", &meth_PackageManager_parseAndPrefix1399, 23);
int argcv_meth_PackageManager_parseAndPrefix1399[] = {3};
meth_meth_PackageManager_parseAndPrefix1399->type = alloc_MethodType(1, argcv_meth_PackageManager_parseAndPrefix1399);
meth_meth_PackageManager_parseAndPrefix1399->type->types[0] = type_String;
meth_meth_PackageManager_parseAndPrefix1399->type->names[0] = "readFile";
meth_meth_PackageManager_parseAndPrefix1399->type->types[1] = type_String;
meth_meth_PackageManager_parseAndPrefix1399->type->names[1] = "address";
meth_meth_PackageManager_parseAndPrefix1399->type->types[2] = type_String;
meth_meth_PackageManager_parseAndPrefix1399->type->names[2] = "prefix";
  meth_meth_PackageManager_parseAndPrefix1399->definitionModule = modulename;
  meth_meth_PackageManager_parseAndPrefix1399->definitionLine = 449;
// compilenode returning 
// Begin line 499
  setline(499);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1600 = (struct UserObject*)self;
  uo1600->data[24] = emptyclosure;
  Method *meth_meth_PackageManager_removeExistingUrls1600 = addmethod2pos(self, "removeExistingUrls", &meth_PackageManager_removeExistingUrls1600, 24);
int argcv_meth_PackageManager_removeExistingUrls1600[] = {1};
meth_meth_PackageManager_removeExistingUrls1600->type = alloc_MethodType(1, argcv_meth_PackageManager_removeExistingUrls1600);
meth_meth_PackageManager_removeExistingUrls1600->type->types[0] = type_String;
meth_meth_PackageManager_removeExistingUrls1600->type->names[0] = "importStatement";
  meth_meth_PackageManager_removeExistingUrls1600->definitionModule = modulename;
  meth_meth_PackageManager_removeExistingUrls1600->definitionLine = 499;
// compilenode returning 
// Begin line 523
  setline(523);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1652 = (struct UserObject*)self;
  uo1652->data[25] = emptyclosure;
  Method *meth_meth_PackageManager_removeContainingDir1652 = addmethod2pos(self, "removeContainingDir", &meth_PackageManager_removeContainingDir1652, 25);
int argcv_meth_PackageManager_removeContainingDir1652[] = {1};
meth_meth_PackageManager_removeContainingDir1652->type = alloc_MethodType(1, argcv_meth_PackageManager_removeContainingDir1652);
meth_meth_PackageManager_removeContainingDir1652->type->types[0] = type_String;
meth_meth_PackageManager_removeContainingDir1652->type->names[0] = "st";
  meth_meth_PackageManager_removeContainingDir1652->definitionModule = modulename;
  meth_meth_PackageManager_removeContainingDir1652->definitionLine = 523;
// compilenode returning 
// Begin line 538
  setline(538);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1697 = (struct UserObject*)self;
  uo1697->data[26] = emptyclosure;
  Method *meth_meth_PackageManager_getContainingDirectory1697 = addmethod2pos(self, "getContainingDirectory", &meth_PackageManager_getContainingDirectory1697, 26);
int argcv_meth_PackageManager_getContainingDirectory1697[] = {1};
meth_meth_PackageManager_getContainingDirectory1697->type = alloc_MethodType(1, argcv_meth_PackageManager_getContainingDirectory1697);
meth_meth_PackageManager_getContainingDirectory1697->type->types[0] = type_String;
meth_meth_PackageManager_getContainingDirectory1697->type->names[0] = "st";
  meth_meth_PackageManager_getContainingDirectory1697->definitionModule = modulename;
  meth_meth_PackageManager_getContainingDirectory1697->definitionLine = 538;
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
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1747 = createclosure(1, "imported");
setclosureframe(closure1747, stackframe);
  addtoclosure(closure1747, var_imported);
  struct UserObject *uo1747 = (struct UserObject*)self;
  uo1747->data[27] = (Object)closure1747;
  Method *meth_meth_PackageManager_imported1747 = addmethod2pos(self, "imported", &meth_PackageManager_imported1747, 27);
int argcv_meth_PackageManager_imported1747[] = {0};
meth_meth_PackageManager_imported1747->type = alloc_MethodType(1, argcv_meth_PackageManager_imported1747);
  meth_meth_PackageManager_imported1747->definitionModule = modulename;
  meth_meth_PackageManager_imported1747->definitionLine = 541;
// compilenode returning 
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1748 = createclosure(1, "imported:=");
setclosureframe(closure1748, stackframe);
  addtoclosure(closure1748, var_imported);
  struct UserObject *uo1748 = (struct UserObject*)self;
  uo1748->data[28] = (Object)closure1748;
  Method *meth_meth_PackageManager_imported_58__61_1748 = addmethod2pos(self, "imported:=", &meth_PackageManager_imported_58__61_1748, 28);
int argcv_meth_PackageManager_imported_58__61_1748[] = {1};
meth_meth_PackageManager_imported_58__61_1748->type = alloc_MethodType(1, argcv_meth_PackageManager_imported_58__61_1748);
  meth_meth_PackageManager_imported_58__61_1748->definitionModule = modulename;
  meth_meth_PackageManager_imported_58__61_1748->definitionLine = 541;
// compilenode returning 
// compilenode returning done
// Begin line 7
  setline(7);
  setmodule(modulename);
  setsource(originalSourceLines);
  *var_toProcess = undefined;
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1750 = createclosure(1, "toProcess");
setclosureframe(closure1750, stackframe);
  addtoclosure(closure1750, var_toProcess);
  struct UserObject *uo1750 = (struct UserObject*)self;
  uo1750->data[29] = (Object)closure1750;
  Method *meth_meth_PackageManager_toProcess1750 = addmethod2pos(self, "toProcess", &meth_PackageManager_toProcess1750, 29);
int argcv_meth_PackageManager_toProcess1750[] = {0};
meth_meth_PackageManager_toProcess1750->type = alloc_MethodType(1, argcv_meth_PackageManager_toProcess1750);
  meth_meth_PackageManager_toProcess1750->definitionModule = modulename;
  meth_meth_PackageManager_toProcess1750->definitionLine = 541;
// compilenode returning 
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1751 = createclosure(1, "toProcess:=");
setclosureframe(closure1751, stackframe);
  addtoclosure(closure1751, var_toProcess);
  struct UserObject *uo1751 = (struct UserObject*)self;
  uo1751->data[30] = (Object)closure1751;
  Method *meth_meth_PackageManager_toProcess_58__61_1751 = addmethod2pos(self, "toProcess:=", &meth_PackageManager_toProcess_58__61_1751, 30);
int argcv_meth_PackageManager_toProcess_58__61_1751[] = {1};
meth_meth_PackageManager_toProcess_58__61_1751->type = alloc_MethodType(1, argcv_meth_PackageManager_toProcess_58__61_1751);
  meth_meth_PackageManager_toProcess_58__61_1751->definitionModule = modulename;
  meth_meth_PackageManager_toProcess_58__61_1751->definitionLine = 541;
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
  Object bool1753 = alloc_Boolean(0);
// compilenode returning bool1753
  *var_verbose = bool1753;
  if (bool1753 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1754 = createclosure(1, "verbose");
setclosureframe(closure1754, stackframe);
  addtoclosure(closure1754, var_verbose);
  struct UserObject *uo1754 = (struct UserObject*)self;
  uo1754->data[31] = (Object)closure1754;
  Method *meth_meth_PackageManager_verbose1754 = addmethod2pos(self, "verbose", &meth_PackageManager_verbose1754, 31);
int argcv_meth_PackageManager_verbose1754[] = {0};
meth_meth_PackageManager_verbose1754->type = alloc_MethodType(1, argcv_meth_PackageManager_verbose1754);
  meth_meth_PackageManager_verbose1754->definitionModule = modulename;
  meth_meth_PackageManager_verbose1754->definitionLine = 541;
// compilenode returning 
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1755 = createclosure(1, "verbose:=");
setclosureframe(closure1755, stackframe);
  addtoclosure(closure1755, var_verbose);
  struct UserObject *uo1755 = (struct UserObject*)self;
  uo1755->data[32] = (Object)closure1755;
  Method *meth_meth_PackageManager_verbose_58__61_1755 = addmethod2pos(self, "verbose:=", &meth_PackageManager_verbose_58__61_1755, 32);
int argcv_meth_PackageManager_verbose_58__61_1755[] = {1};
meth_meth_PackageManager_verbose_58__61_1755->type = alloc_MethodType(1, argcv_meth_PackageManager_verbose_58__61_1755);
  meth_meth_PackageManager_verbose_58__61_1755->definitionModule = modulename;
  meth_meth_PackageManager_verbose_58__61_1755->definitionLine = 541;
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
  Object bool1757 = alloc_Boolean(0);
// compilenode returning bool1757
  *var_global = bool1757;
  if (bool1757 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1758 = createclosure(1, "global");
setclosureframe(closure1758, stackframe);
  addtoclosure(closure1758, var_global);
  struct UserObject *uo1758 = (struct UserObject*)self;
  uo1758->data[33] = (Object)closure1758;
  Method *meth_meth_PackageManager_global1758 = addmethod2pos(self, "global", &meth_PackageManager_global1758, 33);
int argcv_meth_PackageManager_global1758[] = {0};
meth_meth_PackageManager_global1758->type = alloc_MethodType(1, argcv_meth_PackageManager_global1758);
  meth_meth_PackageManager_global1758->definitionModule = modulename;
  meth_meth_PackageManager_global1758->definitionLine = 541;
// compilenode returning 
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1759 = createclosure(1, "global:=");
setclosureframe(closure1759, stackframe);
  addtoclosure(closure1759, var_global);
  struct UserObject *uo1759 = (struct UserObject*)self;
  uo1759->data[34] = (Object)closure1759;
  Method *meth_meth_PackageManager_global_58__61_1759 = addmethod2pos(self, "global:=", &meth_PackageManager_global_58__61_1759, 34);
int argcv_meth_PackageManager_global_58__61_1759[] = {1};
meth_meth_PackageManager_global_58__61_1759->type = alloc_MethodType(1, argcv_meth_PackageManager_global_58__61_1759);
  meth_meth_PackageManager_global_58__61_1759->definitionModule = modulename;
  meth_meth_PackageManager_global_58__61_1759->definitionLine = 541;
// compilenode returning 
// compilenode returning done
// Begin line 9
  setline(9);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1761 == NULL) {
    strlit1761 = alloc_String("");
    gc_root(strlit1761);
  }
// compilenode returning strlit1761
  *var_bundlePath = strlit1761;
  if (strlit1761 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1762 = createclosure(1, "bundlePath");
setclosureframe(closure1762, stackframe);
  addtoclosure(closure1762, var_bundlePath);
  struct UserObject *uo1762 = (struct UserObject*)self;
  uo1762->data[35] = (Object)closure1762;
  Method *meth_meth_PackageManager_bundlePath1762 = addmethod2pos(self, "bundlePath", &meth_PackageManager_bundlePath1762, 35);
int argcv_meth_PackageManager_bundlePath1762[] = {0};
meth_meth_PackageManager_bundlePath1762->type = alloc_MethodType(1, argcv_meth_PackageManager_bundlePath1762);
  meth_meth_PackageManager_bundlePath1762->definitionModule = modulename;
  meth_meth_PackageManager_bundlePath1762->definitionLine = 541;
// compilenode returning 
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1763 = createclosure(1, "bundlePath:=");
setclosureframe(closure1763, stackframe);
  addtoclosure(closure1763, var_bundlePath);
  struct UserObject *uo1763 = (struct UserObject*)self;
  uo1763->data[36] = (Object)closure1763;
  Method *meth_meth_PackageManager_bundlePath_58__61_1763 = addmethod2pos(self, "bundlePath:=", &meth_PackageManager_bundlePath_58__61_1763, 36);
int argcv_meth_PackageManager_bundlePath_58__61_1763[] = {1};
meth_meth_PackageManager_bundlePath_58__61_1763->type = alloc_MethodType(1, argcv_meth_PackageManager_bundlePath_58__61_1763);
  meth_meth_PackageManager_bundlePath_58__61_1763->definitionModule = modulename;
  meth_meth_PackageManager_bundlePath_58__61_1763->definitionLine = 541;
// compilenode returning 
// compilenode returning done
// Begin line 10
  setline(10);
  setmodule(modulename);
  setsource(originalSourceLines);
  *var_curFile = undefined;
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1765 = createclosure(1, "curFile");
setclosureframe(closure1765, stackframe);
  addtoclosure(closure1765, var_curFile);
  struct UserObject *uo1765 = (struct UserObject*)self;
  uo1765->data[37] = (Object)closure1765;
  Method *meth_meth_PackageManager_curFile1765 = addmethod2pos(self, "curFile", &meth_PackageManager_curFile1765, 37);
int argcv_meth_PackageManager_curFile1765[] = {0};
meth_meth_PackageManager_curFile1765->type = alloc_MethodType(1, argcv_meth_PackageManager_curFile1765);
  meth_meth_PackageManager_curFile1765->definitionModule = modulename;
  meth_meth_PackageManager_curFile1765->definitionLine = 541;
// compilenode returning 
// Begin line 541
  setline(541);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1766 = createclosure(1, "curFile:=");
setclosureframe(closure1766, stackframe);
  addtoclosure(closure1766, var_curFile);
  struct UserObject *uo1766 = (struct UserObject*)self;
  uo1766->data[38] = (Object)closure1766;
  Method *meth_meth_PackageManager_curFile_58__61_1766 = addmethod2pos(self, "curFile:=", &meth_PackageManager_curFile_58__61_1766, 38);
int argcv_meth_PackageManager_curFile_58__61_1766[] = {1};
meth_meth_PackageManager_curFile_58__61_1766->type = alloc_MethodType(1, argcv_meth_PackageManager_curFile_58__61_1766);
  meth_meth_PackageManager_curFile_58__61_1766->definitionModule = modulename;
  meth_meth_PackageManager_curFile_58__61_1766->definitionLine = 541;
// compilenode returning 
// compilenode returning done
// Begin line 12
  setline(12);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
  int callframe1768 = gc_frame_new();
  int callframe1769 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call1770 = callmethod(*var_sys, "argv",
    1, partcv, params);
  gc_frame_end(callframe1769);
// compilenode returning call1770
// compilenode returning call1770
  gc_frame_newslot(call1770);
  params[0] = call1770;
  partcv[0] = 1;
  Object call1771 = callmethodflags(self, "parseArgs", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1768);
// compilenode returning call1771
// Begin line 14
  setline(14);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// compilenode returning 
// Begin line 46
  setline(46);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 113
  setline(113);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 126
  setline(126);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 138
  setline(138);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 149
  setline(149);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 161
  setline(161);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 186
  setline(186);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 222
  setline(222);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 249
  setline(249);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 259
  setline(259);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 274
  setline(274);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 291
  setline(291);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 307
  setline(307);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 331
  setline(331);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 348
  setline(348);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 367
  setline(367);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 384
  setline(384);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 392
  setline(392);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 398
  setline(398);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 407
  setline(407);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 425
  setline(425);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 449
  setline(449);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 499
  setline(499);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 523
  setline(523);
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
