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
  "    var buildPath := \"\"",
  "    for (1..paths.size) do { t->",
  "        if(paths.at(t) == \":\")then{",
  "            if(io.exists(buildPath++\"/minigrace\"))then{",
  "                io.system(buildPath++\"/minigrace \"++usrDir++file.address)",
  "            }",
  "            buildPath := \"\"",
  "        } ",
  "        else{",
  "            buildPath :=  buildPath++paths[t]",
  "        }",
  "    }",
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
  "    var newDir := createDirectory(\"{name}/\")",
  "    var toOpen := io.open(toBundle,\"r\");",
  "    var openData := toOpen.read",
  "    print(\"{name}/{toBundle} is what I'm printing\");",
  "    var toWrite := io.open(\"{name}/{toBundle}\", \"w\")",
  "    toWrite.write(openData);",
  "    var file := object{",
  "        var address : String is public",
  "        var data : String is public  ",
  "    }",
  "    fetchImports(toBundle)",
  "    print(\"Imported size = \"++imported.size);",
  "    while{imported.size > 0 }do{",
  "        var curImport := imported.pop",
  "        toWrite := io.open(\"{name}/{curImport.address}\",\"w\")",
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
  "                line := \"import \\\"\"++prefix++\"/\"++line.substringFrom(pos+1)to(line.size);",
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
Object reader_PackageManager_outer_1257(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject*)self;
  return uo->data[0];
}
Object reader_PackageManager_address_1258(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  return uo->data[1];
}
Object writer_PackageManager_address_1258(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  uo->data[1] = args[0];
  return done;
}
Object reader_PackageManager_data_1259(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  return uo->data[2];
}
Object writer_PackageManager_data_1259(Object self, int nparams, int *argcv, Object* args, int flags) {
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
static Object strlit746;
static Object strlit749;
static Object strlit754;
static Object strlit756;
static Object strlit761;
static Object strlit763;
static Object strlit782;
static Object strlit791;
static Object strlit793;
static Object strlit798;
static Object strlit800;
static Object strlit806;
static Object strlit809;
static Object strlit813;
static Object strlit827;
static Object strlit833;
static Object strlit841;
static Object strlit851;
static Object strlit864;
static Object strlit877;
static Object strlit878;
static Object strlit882;
static Object strlit885;
static Object strlit908;
static Object strlit915;
static Object strlit925;
static Object strlit936;
static Object strlit942;
static Object strlit945;
static Object strlit980;
static Object strlit1025;
static Object strlit1042;
static Object strlit1061;
static Object strlit1087;
static Object strlit1096;
static Object strlit1123;
static Object strlit1132;
static Object strlit1159;
static Object strlit1177;
static Object strlit1193;
static Object strlit1195;
static Object strlit1197;
static Object strlit1208;
static Object strlit1211;
static Object strlit1216;
static Object strlit1224;
static Object strlit1227;
static Object strlit1232;
static Object strlit1241;
static Object strlit1244;
static Object strlit1249;
static Object strlit1252;
static ClassData objclass1256;
static Object strlit1263;
static Object strlit1288;
static Object strlit1291;
static Object strlit1298;
static Object strlit1301;
static Object strlit1336;
static Object strlit1342;
static Object strlit1347;
static Object strlit1359;
static Object strlit1362;
static Object strlit1371;
static Object strlit1376;
static Object strlit1422;
static Object strlit1443;
static Object strlit1476;
static Object strlit1482;
static Object strlit1492;
static Object strlit1509;
static Object strlit1514;
static Object strlit1540;
static Object strlit1543;
static Object strlit1582;
Object module_sys;
Object module_sys_init();
Object module_io;
Object module_io_init();
Object module_curl;
Object module_curl_init();
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
// Begin line 19
  setline(19);
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
// Begin line 27
  setline(27);
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
// Begin line 31
  setline(31);
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
// Begin line 35
  setline(35);
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
// Begin line 39
  setline(39);
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
// Begin line 17
  setline(17);
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
// Begin line 18
  setline(18);
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
  meth_meth_PackageManager__apply13->definitionLine = 15;
// compilenode returning block12
  gc_frame_newslot(block12);
// Begin line 18
  setline(18);
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
// Begin line 26
  setline(26);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block20 = alloc_Block(NULL, NULL, "PackageManager", 26);
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
  meth_meth_PackageManager__apply21->definitionLine = 15;
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
// Begin line 30
  setline(30);
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
  meth_meth_PackageManager__apply29->definitionLine = 15;
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
    strlit35 = alloc_String("setAddress");
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
  meth_meth_PackageManager__apply37->definitionLine = 15;
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
    strlit43 = alloc_String("verbose");
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
  meth_meth_PackageManager__apply45->definitionLine = 15;
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
  Object call49 = callmethod(*var_on, "flag()do",
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
    strlit51 = alloc_String("global");
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
  addtoclosure(closure53, var_global);
  addtoclosure(closure53, selfslot);
  struct UserObject *uo53 = (struct UserObject*)block52;
  uo53->data[0] = (Object)closure53;
  Method *meth_meth_PackageManager__apply53 = addmethod2pos(block52, "_apply", &meth_PackageManager__apply53, 0);
int argcv_meth_PackageManager__apply53[] = {0};
meth_meth_PackageManager__apply53->type = alloc_MethodType(1, argcv_meth_PackageManager__apply53);
  meth_meth_PackageManager__apply53->definitionModule = modulename;
  meth_meth_PackageManager__apply53->definitionLine = 15;
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
// Begin line 14
  setline(14);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1 = alloc_Float64(0);
// compilenode returning num1
  *var_count = num1;
  if (num1 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 15
  setline(15);
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
// Begin line 43
  setline(43);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe5 = gc_frame_new();
// Begin line 16
  setline(16);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_args
  gc_frame_newslot(*var_args);
// Begin line 43
  setline(43);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block6 = alloc_Block(NULL, NULL, "PackageManager", 43);
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
  meth_meth_PackageManager__apply7->definitionLine = 15;
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
// Begin line 53
  setline(53);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe68 = gc_frame_new();
// Begin line 52
  setline(52);
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
// Begin line 62
  setline(62);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 53
  setline(53);
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
// Begin line 58
  setline(58);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 54
  setline(54);
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
// Begin line 56
  setline(56);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe102 = gc_frame_new();
// Begin line 55
  setline(55);
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
// Begin line 59
  setline(59);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 58
  setline(58);
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
// Begin line 60
  setline(60);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe119 = gc_frame_new();
// Begin line 59
  setline(59);
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
// Begin line 61
  setline(61);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 60
  setline(60);
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
// Begin line 66
  setline(66);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe126 = gc_frame_new();
// Begin line 65
  setline(65);
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
// Begin line 77
  setline(77);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 66
  setline(66);
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
// Begin line 70
  setline(70);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 67
  setline(67);
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
// Begin line 69
  setline(69);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe150 = gc_frame_new();
// Begin line 68
  setline(68);
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
// Begin line 71
  setline(71);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe162 = gc_frame_new();
// Begin line 70
  setline(70);
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
// Begin line 72
  setline(72);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe168 = gc_frame_new();
// Begin line 71
  setline(71);
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
// Begin line 73
  setline(73);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe174 = gc_frame_new();
// Begin line 72
  setline(72);
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
// Begin line 74
  setline(74);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 73
  setline(73);
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
// Begin line 75
  setline(75);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 74
  setline(74);
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
// Begin line 81
  setline(81);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe184 = gc_frame_new();
// Begin line 80
  setline(80);
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
// Begin line 85
  setline(85);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe188 = gc_frame_new();
// Begin line 84
  setline(84);
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
// Begin line 85
  setline(85);
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
// Begin line 90
  setline(90);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 86
  setline(86);
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
// Begin line 87
  setline(87);
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
// Begin line 89
  setline(89);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 88
  setline(88);
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
// Begin line 93
  setline(93);
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
// Begin line 95
  setline(95);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe232 = gc_frame_new();
// Begin line 94
  setline(94);
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
// Begin line 100
  setline(100);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe246 = gc_frame_new();
// Begin line 99
  setline(99);
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
// Begin line 100
  setline(100);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_arg
  params[0] = *var_arg;
  Object call248 = gracelib_print(NULL, 1,  params);
// compilenode returning call248
// Begin line 102
  setline(102);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 101
  setline(101);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool249 = alloc_Boolean(0);
// compilenode returning bool249
  *var_ran = bool249;
  if (bool249 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 104
  setline(104);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe251 = gc_frame_new();
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
  Object call252 = callmethod(*var_block, "apply",
    1, partcv, params);
  gc_frame_end(callframe251);
// compilenode returning call252
// Begin line 108
  setline(108);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 104
  setline(104);
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
// Begin line 106
  setline(106);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe266 = gc_frame_new();
// Begin line 105
  setline(105);
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
// Begin line 109
  setline(109);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 108
  setline(108);
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
// Begin line 47
  setline(47);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 46
  setline(46);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num61 = alloc_Float64(2);
// compilenode returning num61
  *var_i = num61;
  if (num61 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 48
  setline(48);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 47
  setline(47);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool62 = alloc_Boolean(0);
// compilenode returning bool62
  *var_ran = bool62;
  if (bool62 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 50
  setline(50);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 48
  setline(48);
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
// Begin line 50
  setline(50);
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
  meth_meth_PackageManager_option_40__41_shortHand_40__41_do67->definitionLine = 51;
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
  meth_meth_PackageManager_doubleValue_40__41_do125->definitionLine = 64;
  struct UserObject *uo183 = (struct UserObject*)obj65;
  uo183->data[3] = emptyclosure;
  Method *meth_meth_PackageManager_option_40__41_do183 = addmethod2pos(obj65, "option()do", &meth_PackageManager_option_40__41_do183, 3);
int argcv_meth_PackageManager_option_40__41_do183[] = {1, 1};
meth_meth_PackageManager_option_40__41_do183->type = alloc_MethodType(2, argcv_meth_PackageManager_option_40__41_do183);
meth_meth_PackageManager_option_40__41_do183->type->types[0] = type_String;
meth_meth_PackageManager_option_40__41_do183->type->names[0] = "name";
  meth_meth_PackageManager_option_40__41_do183->definitionModule = modulename;
  meth_meth_PackageManager_option_40__41_do183->definitionLine = 79;
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
  meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187->definitionLine = 83;
  struct UserObject *uo229 = (struct UserObject*)obj65;
  uo229->data[5] = emptyclosure;
  Method *meth_meth_PackageManager_flag_40__41_do229 = addmethod2pos(obj65, "flag()do", &meth_PackageManager_flag_40__41_do229, 5);
int argcv_meth_PackageManager_flag_40__41_do229[] = {1, 1};
meth_meth_PackageManager_flag_40__41_do229->type = alloc_MethodType(2, argcv_meth_PackageManager_flag_40__41_do229);
meth_meth_PackageManager_flag_40__41_do229->type->types[0] = type_String;
meth_meth_PackageManager_flag_40__41_do229->type->names[0] = "name";
  meth_meth_PackageManager_flag_40__41_do229->definitionModule = modulename;
  meth_meth_PackageManager_flag_40__41_do229->definitionLine = 92;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
objclass65 = obj65->class;
  objclass65->definitionModule = modulename;
  objclass65->definitionLine = 50;
  (*(struct UserObject *)self).data[0] = lowerouter65;
  self = oldself65;
  selfslot = oldselfslot65;
  stackframe = oldstackframe65;
// compilenode returning obj65
  *var_on = obj65;
  if (obj65 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 97
  setline(97);
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
// Begin line 110
  setline(110);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe237 = gc_frame_new();
// Begin line 98
  setline(98);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block238 = alloc_Block(NULL, NULL, "PackageManager", 98);
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
  meth_meth_PackageManager__apply239->definitionLine = 87;
// compilenode returning block238
  gc_frame_newslot(block238);
// Begin line 110
  setline(110);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block244 = alloc_Block(NULL, NULL, "PackageManager", 110);
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
  meth_meth_PackageManager__apply245->definitionLine = 87;
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
// Begin line 47
  setline(47);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 46
  setline(46);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num61 = alloc_Float64(2);
// compilenode returning num61
  *var_i = num61;
  if (num61 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 48
  setline(48);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 47
  setline(47);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool62 = alloc_Boolean(0);
// compilenode returning bool62
  *var_ran = bool62;
  if (bool62 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 50
  setline(50);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 48
  setline(48);
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
// Begin line 50
  setline(50);
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
  meth_meth_PackageManager_option_40__41_shortHand_40__41_do67->definitionLine = 51;
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
  meth_meth_PackageManager_doubleValue_40__41_do125->definitionLine = 64;
  struct UserObject *uo183 = (struct UserObject*)obj65;
  uo183->data[3] = emptyclosure;
  Method *meth_meth_PackageManager_option_40__41_do183 = addmethod2pos(obj65, "option()do", &meth_PackageManager_option_40__41_do183, 3);
int argcv_meth_PackageManager_option_40__41_do183[] = {1, 1};
meth_meth_PackageManager_option_40__41_do183->type = alloc_MethodType(2, argcv_meth_PackageManager_option_40__41_do183);
meth_meth_PackageManager_option_40__41_do183->type->types[0] = type_String;
meth_meth_PackageManager_option_40__41_do183->type->names[0] = "name";
  meth_meth_PackageManager_option_40__41_do183->definitionModule = modulename;
  meth_meth_PackageManager_option_40__41_do183->definitionLine = 79;
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
  meth_meth_PackageManager_flag_40__41_shortHand_40__41_do187->definitionLine = 83;
  struct UserObject *uo229 = (struct UserObject*)obj65;
  uo229->data[5] = emptyclosure;
  Method *meth_meth_PackageManager_flag_40__41_do229 = addmethod2pos(obj65, "flag()do", &meth_PackageManager_flag_40__41_do229, 5);
int argcv_meth_PackageManager_flag_40__41_do229[] = {1, 1};
meth_meth_PackageManager_flag_40__41_do229->type = alloc_MethodType(2, argcv_meth_PackageManager_flag_40__41_do229);
meth_meth_PackageManager_flag_40__41_do229->type->types[0] = type_String;
meth_meth_PackageManager_flag_40__41_do229->type->names[0] = "name";
  meth_meth_PackageManager_flag_40__41_do229->definitionModule = modulename;
  meth_meth_PackageManager_flag_40__41_do229->definitionLine = 92;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
  sourceObject = obj65;
objclass65 = obj65->class;
  objclass65->definitionModule = modulename;
  objclass65->definitionLine = 50;
  (*(struct UserObject *)self).data[0] = lowerouter65;
  self = oldself65;
  selfslot = oldselfslot65;
  stackframe = oldstackframe65;
// compilenode returning obj65
  *var_on = obj65;
  if (obj65 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 97
  setline(97);
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
// Begin line 110
  setline(110);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe237 = gc_frame_new();
// Begin line 98
  setline(98);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block238 = alloc_Block(NULL, NULL, "PackageManager", 98);
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
  meth_meth_PackageManager__apply239->definitionLine = 87;
// compilenode returning block238
  gc_frame_newslot(block238);
// Begin line 110
  setline(110);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block244 = alloc_Block(NULL, NULL, "PackageManager", 110);
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
  meth_meth_PackageManager__apply245->definitionLine = 87;
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
// Begin line 114
  setline(114);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 113
  setline(113);
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
// Begin line 119
  setline(119);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 114
  setline(114);
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
// Begin line 115
  setline(115);
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
// Begin line 117
  setline(117);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 116
  setline(116);
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
// Begin line 118
  setline(118);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe296 = gc_frame_new();
// Begin line 117
  setline(117);
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
// Begin line 123
  setline(123);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 119
  setline(119);
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
// Begin line 121
  setline(121);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 120
  setline(120);
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
// Begin line 122
  setline(122);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe318 = gc_frame_new();
// Begin line 121
  setline(121);
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
// Begin line 134
  setline(134);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 128
  setline(128);
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
// Begin line 129
  setline(129);
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
// Begin line 133
  setline(133);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 130
  setline(130);
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
// Begin line 132
  setline(132);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe359 = gc_frame_new();
// Begin line 131
  setline(131);
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
// Begin line 126
  setline(126);
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
// Begin line 135
  setline(135);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe324 = gc_frame_new();
// Begin line 127
  setline(127);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pathContents
  gc_frame_newslot(*var_pathContents);
// Begin line 135
  setline(135);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block325 = alloc_Block(NULL, NULL, "PackageManager", 135);
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
  meth_meth_PackageManager__apply326->definitionLine = 120;
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
// Begin line 141
  setline(141);
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
// Begin line 144
  setline(144);
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
// Begin line 139
  setline(139);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 138
  setline(138);
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
// Begin line 140
  setline(140);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe377 = gc_frame_new();
// Begin line 139
  setline(139);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_impAddress
  gc_frame_newslot(*var_impAddress);
  params[0] = *var_impAddress;
  partcv[0] = 1;
  Object call378 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe377);
// compilenode returning call378
// Begin line 143
  setline(143);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe379 = gc_frame_new();
// Begin line 140
  setline(140);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_imported
  gc_frame_newslot(*var_imported);
// Begin line 143
  setline(143);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block380 = alloc_Block(NULL, NULL, "PackageManager", 143);
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
  meth_meth_PackageManager__apply381->definitionLine = 130;
// compilenode returning block380
  gc_frame_newslot(block380);
  params[0] = *var_imported;
  params[1] = block380;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call385 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe379);
// compilenode returning call385
// Begin line 146
  setline(146);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe386 = gc_frame_new();
// Begin line 143
  setline(143);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_imported
  gc_frame_newslot(*var_imported);
// Begin line 146
  setline(146);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block387 = alloc_Block(NULL, NULL, "PackageManager", 146);
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
  meth_meth_PackageManager__apply388->definitionLine = 130;
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
// Begin line 157
  setline(157);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 149
  setline(149);
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
// Begin line 150
  setline(150);
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
  reader411->definitionLine = 152;
  writer411->definitionLine = 152;
}
obj409->flags |= OFLAG_MUTABLE;
adddatum2(obj409, alloc_Undefined(), 1);
if (objclass409 == NULL) {
  Method *reader412 = addmethodrealflags(obj409, "data",&reader_PackageManager_data_412, 0);
  Method *writer412 = addmethodrealflags(obj409, "data:=",&writer_PackageManager_data_412, 0);
  reader412->definitionModule = modulename;
  writer412->definitionModule = modulename;
  reader412->definitionLine = 153;
  writer412->definitionLine = 153;
}
obj409->flags |= OFLAG_MUTABLE;
adddatum2(obj409, alloc_Undefined(), 2);
  sourceObject = obj409;
// Begin line 151
  setline(151);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_fileAddress
  adddatum2(obj409, *var_fileAddress, 1);
  sourceObject = obj409;
  adddatum2(obj409, undefined, 2);
objclass409 = obj409->class;
  objclass409->definitionModule = modulename;
  objclass409->definitionLine = 150;
  (*(struct UserObject *)self).data[0] = lowerouter409;
  self = oldself409;
  selfslot = oldselfslot409;
  stackframe = oldstackframe409;
// compilenode returning obj409
  *var_file = obj409;
  if (obj409 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 154
  setline(154);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_file
  *var_curFile = *var_file;
  if (*var_file == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 155
  setline(155);
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
// Begin line 157
  setline(157);
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
// Begin line 173
  setline(173);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe462 = gc_frame_new();
// Begin line 172
  setline(172);
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
// Begin line 183
  setline(183);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 161
  setline(161);
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
// Begin line 163
  setline(163);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 162
  setline(162);
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
// Begin line 167
  setline(167);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 163
  setline(163);
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
// Begin line 165
  setline(165);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 164
  setline(164);
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
// Begin line 166
  setline(166);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 165
  setline(165);
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
// Begin line 176
  setline(176);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 167
  setline(167);
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
// Begin line 169
  setline(169);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe444 = gc_frame_new();
// Begin line 168
  setline(168);
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
// Begin line 170
  setline(170);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 169
  setline(169);
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
// Begin line 171
  setline(171);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe448 = gc_frame_new();
// Begin line 170
  setline(170);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curFile
  gc_frame_newslot(*var_curFile);
  params[0] = *var_curFile;
  partcv[0] = 1;
  Object call449 = callmethodflags(self, "parseFile", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe448);
// compilenode returning call449
// Begin line 174
  setline(174);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe450 = gc_frame_new();
// Begin line 171
  setline(171);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block451 = alloc_Block(NULL, NULL, "PackageManager", 171);
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
  meth_meth_PackageManager__apply452->definitionLine = 162;
// compilenode returning block451
  gc_frame_newslot(block451);
// Begin line 174
  setline(174);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block460 = alloc_Block(NULL, NULL, "PackageManager", 174);
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
  meth_meth_PackageManager__apply461->definitionLine = 171;
// compilenode returning block460
  gc_frame_newslot(block460);
  params[0] = block451;
  params[1] = block460;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call467 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe450);
// compilenode returning call467
// Begin line 175
  setline(175);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 174
  setline(174);
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
// Begin line 182
  setline(182);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 178
  setline(178);
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
// Begin line 179
  setline(179);
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
// Begin line 181
  setline(181);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 180
  setline(180);
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
// Begin line 201
  setline(201);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 200
  setline(200);
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
// Begin line 202
  setline(202);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 197
  setline(197);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe540 = gc_frame_new();
// Begin line 202
  setline(202);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe541 = gc_frame_new();
// Begin line 201
  setline(201);
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
// Begin line 202
  setline(202);
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
// Begin line 210
  setline(210);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 186
  setline(186);
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
// Begin line 187
  setline(187);
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
// Begin line 195
  setline(195);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 188
  setline(188);
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
// Begin line 190
  setline(190);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe510 = gc_frame_new();
// Begin line 189
  setline(189);
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
// Begin line 194
  setline(194);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 190
  setline(190);
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
// Begin line 192
  setline(192);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 188
  setline(188);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe517 = gc_frame_new();
// Begin line 191
  setline(191);
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
// Begin line 192
  setline(192);
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
// Begin line 196
  setline(196);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 195
  setline(195);
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
// Begin line 197
  setline(197);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 195
  setline(195);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe522 = gc_frame_new();
// Begin line 196
  setline(196);
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
// Begin line 198
  setline(198);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 197
  setline(197);
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
// Begin line 199
  setline(199);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 198
  setline(198);
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
// Begin line 204
  setline(204);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe535 = gc_frame_new();
  Object block536 = alloc_Block(NULL, NULL, "PackageManager", 204);
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
  meth_meth_PackageManager__apply537->definitionLine = 197;
// compilenode returning block536
  gc_frame_newslot(block536);
// Begin line 199
  setline(199);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_req
  params[0] = block536;
  partcv[0] = 1;
  Object call547 = callmethod(*var_req, "onReceive",
    1, partcv, params);
  gc_frame_end(callframe535);
// compilenode returning call547
// Begin line 204
  setline(204);
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
// Begin line 209
  setline(209);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 205
  setline(205);
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
// Begin line 206
  setline(206);
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
// Begin line 208
  setline(208);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 207
  setline(207);
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
// Begin line 218
  setline(218);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 210
  setline(210);
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
// Begin line 211
  setline(211);
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
// Begin line 213
  setline(213);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe589 = gc_frame_new();
// Begin line 212
  setline(212);
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
// Begin line 217
  setline(217);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 213
  setline(213);
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
// Begin line 215
  setline(215);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 212
  setline(212);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe598 = gc_frame_new();
// Begin line 214
  setline(214);
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
// Begin line 215
  setline(215);
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
// Begin line 218
  setline(218);
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
// Begin line 227
  setline(227);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 222
  setline(222);
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
// Begin line 223
  setline(223);
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
// Begin line 224
  setline(224);
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
// Begin line 225
  setline(225);
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
// Begin line 230
  setline(230);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 227
  setline(227);
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
// Begin line 229
  setline(229);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 228
  setline(228);
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
// Begin line 231
  setline(231);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 230
  setline(230);
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
// Begin line 234
  setline(234);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 231
  setline(231);
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
// Begin line 233
  setline(233);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 232
  setline(232);
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
// Begin line 237
  setline(237);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 234
  setline(234);
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
// Begin line 236
  setline(236);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 235
  setline(235);
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
// Begin line 240
  setline(240);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 237
  setline(237);
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
// Begin line 239
  setline(239);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 238
  setline(238);
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
// Begin line 241
  setline(241);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 240
  setline(240);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool729 = alloc_Boolean(0);
// compilenode returning bool729
  return bool729;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_validateFile730(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 250
  setline(250);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 244
  setline(244);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe733 = gc_frame_new();
  int callframe734 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call735 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe734);
// compilenode returning call735
// compilenode returning call735
  partcv[0] = 0;
  Object call736 = callmethod(call735, "size",
    1, partcv, params);
  gc_frame_end(callframe733);
// compilenode returning call736
// compilenode returning call736
  int op_slot_left_732 = gc_frame_newslot(call736);
  Object num737 = alloc_Float64(1);
// compilenode returning num737
  int op_slot_right_732 = gc_frame_newslot(num737);
  params[0] = num737;
  partcv[0] = 1;
  Object opresult739 = callmethod(call736, ">", 1, partcv, params);
// compilenode returning opresult739
  Object if731 = done;
  if (istrue(opresult739)) {
// Begin line 249
  setline(249);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 245
  setline(245);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe742 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call743 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe742);
// compilenode returning call743
// compilenode returning call743
  Object num744 = alloc_Float64(1);
// compilenode returning num744
  params[0] = num744;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres745 = callmethod(call743, "[]", 1, partcv, params);
// compilenode returning idxres745
  int op_slot_left_741 = gc_frame_newslot(idxres745);
  if (strlit746 == NULL) {
    strlit746 = alloc_String("<");
    gc_root(strlit746);
  }
// compilenode returning strlit746
  int op_slot_right_741 = gc_frame_newslot(strlit746);
  params[0] = strlit746;
  partcv[0] = 1;
  Object opresult748 = callmethod(idxres745, "==", 1, partcv, params);
// compilenode returning opresult748
  Object if740 = done;
  if (istrue(opresult748)) {
// Begin line 246
  setline(246);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit749 == NULL) {
    strlit749 = alloc_String("Not a valid grace file");
    gc_root(strlit749);
  }
// compilenode returning strlit749
  params[0] = strlit749;
  Object call750 = gracelib_print(NULL, 1,  params);
// compilenode returning call750
// Begin line 248
  setline(248);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 247
  setline(247);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool751 = alloc_Boolean(0);
// compilenode returning bool751
  return bool751;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if740 = undefined;
  } else {
  }
// compilenode returning if740
    gc_frame_newslot(if740);
    if731 = if740;
  } else {
  }
// compilenode returning if731
// Begin line 251
  setline(251);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 250
  setline(250);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool752 = alloc_Boolean(1);
// compilenode returning bool752
  return bool752;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_write753(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 255
  setline(255);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 254
  setline(254);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit754 == NULL) {
    strlit754 = alloc_String("");
    gc_root(strlit754);
  }
// compilenode returning strlit754
  *var_usrDir = strlit754;
  if (strlit754 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 261
  setline(261);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 255
  setline(255);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_global
  Object if755 = done;
  if (istrue(*var_global)) {
// Begin line 257
  setline(257);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 256
  setline(256);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit756 == NULL) {
    strlit756 = alloc_String("usr/lib/grace/modules/");
    gc_root(strlit756);
  }
// compilenode returning strlit756
  *var_usrDir = strlit756;
  if (strlit756 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if755 = done;
  } else {
// Begin line 260
  setline(260);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 259
  setline(259);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe759 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call760 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe759);
// compilenode returning call760
// compilenode returning call760
  if (strlit761 == NULL) {
    strlit761 = alloc_String("HOME");
    gc_root(strlit761);
  }
// compilenode returning strlit761
  params[0] = strlit761;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres762 = callmethod(call760, "[]", 1, partcv, params);
// compilenode returning idxres762
  int op_slot_left_758 = gc_frame_newslot(idxres762);
  if (strlit763 == NULL) {
    strlit763 = alloc_String("/.local/lib/grace/modules/");
    gc_root(strlit763);
  }
// compilenode returning strlit763
  int op_slot_right_758 = gc_frame_newslot(strlit763);
  params[0] = strlit763;
  partcv[0] = 1;
  Object opresult765 = callmethod(idxres762, "++", 1, partcv, params);
// compilenode returning opresult765
  *var_usrDir = opresult765;
  if (opresult765 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if755 = done;
  }
// compilenode returning if755
// Begin line 262
  setline(262);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe767 = gc_frame_new();
// Begin line 261
  setline(261);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_usrDir
  int op_slot_left_768 = gc_frame_newslot(*var_usrDir);
  int callframe769 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call770 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe769);
// compilenode returning call770
// compilenode returning call770
  int op_slot_right_768 = gc_frame_newslot(call770);
  params[0] = call770;
  partcv[0] = 1;
  Object opresult772 = callmethod(*var_usrDir, "++", 1, partcv, params);
// compilenode returning opresult772
  gc_frame_newslot(opresult772);
  params[0] = opresult772;
  partcv[0] = 1;
  Object call773 = callmethodflags(self, "createDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe767);
// compilenode returning call773
  *var_fileDir = call773;
  if (call773 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 263
  setline(263);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe774 = gc_frame_new();
// Begin line 262
  setline(262);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_fileDir
  Object num776 = alloc_Float64(1);
// compilenode returning num776
  params[0] = num776;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres777 = callmethod(*var_fileDir, "[]", 1, partcv, params);
// compilenode returning idxres777
  int op_slot_left_775 = gc_frame_newslot(idxres777);
// compilenode returning *var_fileDir
  Object num778 = alloc_Float64(2);
// compilenode returning num778
  params[0] = num778;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres779 = callmethod(*var_fileDir, "[]", 1, partcv, params);
// compilenode returning idxres779
  int op_slot_right_775 = gc_frame_newslot(idxres779);
  params[0] = idxres779;
  partcv[0] = 1;
  Object opresult781 = callmethod(idxres777, "++", 1, partcv, params);
// compilenode returning opresult781
  gc_frame_newslot(opresult781);
  if (strlit782 == NULL) {
    strlit782 = alloc_String("w");
    gc_root(strlit782);
  }
// compilenode returning strlit782
  gc_frame_newslot(strlit782);
// compilenode returning *var_io
  params[0] = opresult781;
  params[1] = strlit782;
  partcv[0] = 2;
  Object call783 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe774);
// compilenode returning call783
  *var_toWrite = call783;
  if (call783 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 263
  setline(263);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe784 = gc_frame_new();
  int callframe785 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call786 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe785);
// compilenode returning call786
// compilenode returning call786
  gc_frame_newslot(call786);
// compilenode returning *var_toWrite
  params[0] = call786;
  partcv[0] = 1;
  Object call787 = callmethod(*var_toWrite, "write",
    1, partcv, params);
  gc_frame_end(callframe784);
// compilenode returning call787
// Begin line 264
  setline(264);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe788 = gc_frame_new();
// compilenode returning *var_toWrite
  partcv[0] = 0;
  Object call789 = callmethod(*var_toWrite, "close",
    1, partcv, params);
  gc_frame_end(callframe788);
// compilenode returning call789
// compilenode returning call789
  gc_frame_end(frame);
  return call789;
}
Object meth_PackageManager__apply822(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  Object *var_usrDir = getfromclosure(closure, 3);
  Object *var_file = getfromclosure(closure, 4);
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
// Begin line 289
  setline(289);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 280
  setline(280);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe825 = gc_frame_new();
// compilenode returning *var_t
  gc_frame_newslot(*var_t);
// compilenode returning *var_paths
  params[0] = *var_t;
  partcv[0] = 1;
  Object call826 = callmethod(*var_paths, "at",
    1, partcv, params);
  gc_frame_end(callframe825);
// compilenode returning call826
  int op_slot_left_824 = gc_frame_newslot(call826);
  if (strlit827 == NULL) {
    strlit827 = alloc_String(":");
    gc_root(strlit827);
  }
// compilenode returning strlit827
  int op_slot_right_824 = gc_frame_newslot(strlit827);
  params[0] = strlit827;
  partcv[0] = 1;
  Object opresult829 = callmethod(call826, "==", 1, partcv, params);
// compilenode returning opresult829
  Object if823 = done;
  if (istrue(opresult829)) {
// Begin line 284
  setline(284);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 281
  setline(281);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe831 = gc_frame_new();
// compilenode returning *var_buildPath
  int op_slot_left_832 = gc_frame_newslot(*var_buildPath);
  if (strlit833 == NULL) {
    strlit833 = alloc_String("/minigrace");
    gc_root(strlit833);
  }
// compilenode returning strlit833
  int op_slot_right_832 = gc_frame_newslot(strlit833);
  params[0] = strlit833;
  partcv[0] = 1;
  Object opresult835 = callmethod(*var_buildPath, "++", 1, partcv, params);
// compilenode returning opresult835
  gc_frame_newslot(opresult835);
// compilenode returning *var_io
  params[0] = opresult835;
  partcv[0] = 1;
  Object call836 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe831);
// compilenode returning call836
  Object if830 = done;
  if (istrue(call836)) {
// Begin line 283
  setline(283);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe837 = gc_frame_new();
// Begin line 282
  setline(282);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  int op_slot_left_840 = gc_frame_newslot(*var_buildPath);
  if (strlit841 == NULL) {
    strlit841 = alloc_String("/minigrace ");
    gc_root(strlit841);
  }
// compilenode returning strlit841
  int op_slot_right_840 = gc_frame_newslot(strlit841);
  params[0] = strlit841;
  partcv[0] = 1;
  Object opresult843 = callmethod(*var_buildPath, "++", 1, partcv, params);
// compilenode returning opresult843
  int op_slot_left_839 = gc_frame_newslot(opresult843);
// compilenode returning *var_usrDir
  int op_slot_right_839 = gc_frame_newslot(*var_usrDir);
  params[0] = *var_usrDir;
  partcv[0] = 1;
  Object opresult845 = callmethod(opresult843, "++", 1, partcv, params);
// compilenode returning opresult845
  int op_slot_left_838 = gc_frame_newslot(opresult845);
  int callframe846 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call847 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe846);
// compilenode returning call847
// compilenode returning call847
  int op_slot_right_838 = gc_frame_newslot(call847);
  params[0] = call847;
  partcv[0] = 1;
  Object opresult849 = callmethod(opresult845, "++", 1, partcv, params);
// compilenode returning opresult849
  gc_frame_newslot(opresult849);
// compilenode returning *var_io
  params[0] = opresult849;
  partcv[0] = 1;
  Object call850 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe837);
// compilenode returning call850
    gc_frame_newslot(call850);
    if830 = call850;
  } else {
  }
// compilenode returning if830
// Begin line 285
  setline(285);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 284
  setline(284);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit851 == NULL) {
    strlit851 = alloc_String("");
    gc_root(strlit851);
  }
// compilenode returning strlit851
  *var_buildPath = strlit851;
  if (strlit851 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if823 = done;
  } else {
// Begin line 288
  setline(288);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 287
  setline(287);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  int op_slot_left_853 = gc_frame_newslot(*var_buildPath);
// Begin line 288
  setline(288);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 287
  setline(287);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_paths
// compilenode returning *var_t
  params[0] = *var_t;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres854 = callmethod(*var_paths, "[]", 1, partcv, params);
// compilenode returning idxres854
  int op_slot_right_853 = gc_frame_newslot(idxres854);
  params[0] = idxres854;
  partcv[0] = 1;
  Object opresult856 = callmethod(*var_buildPath, "++", 1, partcv, params);
// compilenode returning opresult856
  *var_buildPath = opresult856;
  if (opresult856 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if823 = done;
  }
// compilenode returning if823
  gc_frame_end(frame);
  return if823;
}
Object meth_PackageManager_compile790(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 12, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(5, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "compile");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
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
// Begin line 270
  setline(270);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 269
  setline(269);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit791 == NULL) {
    strlit791 = alloc_String("");
    gc_root(strlit791);
  }
// compilenode returning strlit791
  *var_usrDir = strlit791;
  if (strlit791 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 276
  setline(276);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 270
  setline(270);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_global
  Object if792 = done;
  if (istrue(*var_global)) {
// Begin line 272
  setline(272);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 271
  setline(271);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit793 == NULL) {
    strlit793 = alloc_String("usr/lib/grace/modules/");
    gc_root(strlit793);
  }
// compilenode returning strlit793
  *var_usrDir = strlit793;
  if (strlit793 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if792 = done;
  } else {
// Begin line 275
  setline(275);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 274
  setline(274);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe796 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call797 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe796);
// compilenode returning call797
// compilenode returning call797
  if (strlit798 == NULL) {
    strlit798 = alloc_String("HOME");
    gc_root(strlit798);
  }
// compilenode returning strlit798
  params[0] = strlit798;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres799 = callmethod(call797, "[]", 1, partcv, params);
// compilenode returning idxres799
  int op_slot_left_795 = gc_frame_newslot(idxres799);
  if (strlit800 == NULL) {
    strlit800 = alloc_String("/.local/lib/grace/modules/");
    gc_root(strlit800);
  }
// compilenode returning strlit800
  int op_slot_right_795 = gc_frame_newslot(strlit800);
  params[0] = strlit800;
  partcv[0] = 1;
  Object opresult802 = callmethod(idxres799, "++", 1, partcv, params);
// compilenode returning opresult802
  *var_usrDir = opresult802;
  if (opresult802 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if792 = done;
  }
// compilenode returning if792
// Begin line 277
  setline(277);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 276
  setline(276);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe804 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call805 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe804);
// compilenode returning call805
// compilenode returning call805
  if (strlit806 == NULL) {
    strlit806 = alloc_String("PATH");
    gc_root(strlit806);
  }
// compilenode returning strlit806
  params[0] = strlit806;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres807 = callmethod(call805, "[]", 1, partcv, params);
// compilenode returning idxres807
  *var_paths = idxres807;
  if (idxres807 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 278
  setline(278);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 277
  setline(277);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit809 == NULL) {
    strlit809 = alloc_String("PATHS = ");
    gc_root(strlit809);
  }
// compilenode returning strlit809
  int op_slot_left_808 = gc_frame_newslot(strlit809);
// compilenode returning *var_paths
  int op_slot_right_808 = gc_frame_newslot(*var_paths);
  params[0] = *var_paths;
  partcv[0] = 1;
  Object opresult811 = callmethod(strlit809, "++", 1, partcv, params);
// compilenode returning opresult811
  params[0] = opresult811;
  Object call812 = gracelib_print(NULL, 1,  params);
// compilenode returning call812
// Begin line 279
  setline(279);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 278
  setline(278);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit813 == NULL) {
    strlit813 = alloc_String("");
    gc_root(strlit813);
  }
// compilenode returning strlit813
  *var_buildPath = strlit813;
  if (strlit813 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 290
  setline(290);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe814 = gc_frame_new();
// Begin line 279
  setline(279);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num816 = alloc_Float64(1);
// compilenode returning num816
  int op_slot_left_815 = gc_frame_newslot(num816);
  int callframe817 = gc_frame_new();
// compilenode returning *var_paths
  partcv[0] = 0;
  Object call818 = callmethod(*var_paths, "size",
    1, partcv, params);
  gc_frame_end(callframe817);
// compilenode returning call818
// compilenode returning call818
  int op_slot_right_815 = gc_frame_newslot(call818);
  params[0] = call818;
  partcv[0] = 1;
  Object opresult820 = callmethod(num816, "..", 1, partcv, params);
// compilenode returning opresult820
  gc_frame_newslot(opresult820);
// Begin line 290
  setline(290);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block821 = alloc_Block(NULL, NULL, "PackageManager", 290);
  gc_frame_newslot(block821);
  block_savedest(block821);
  Object closure822 = createclosure(6, "_apply");
setclosureframe(closure822, stackframe);
  addtoclosure(closure822, var_paths);
  addtoclosure(closure822, var_buildPath);
  addtoclosure(closure822, var_io);
  addtoclosure(closure822, var_usrDir);
  addtoclosure(closure822, var_file);
  addtoclosure(closure822, selfslot);
  struct UserObject *uo822 = (struct UserObject*)block821;
  uo822->data[0] = (Object)closure822;
  Method *meth_meth_PackageManager__apply822 = addmethod2pos(block821, "_apply", &meth_PackageManager__apply822, 0);
int argcv_meth_PackageManager__apply822[] = {1};
meth_meth_PackageManager__apply822->type = alloc_MethodType(1, argcv_meth_PackageManager__apply822);
  meth_meth_PackageManager__apply822->definitionModule = modulename;
  meth_meth_PackageManager__apply822->definitionLine = 279;
// compilenode returning block821
  gc_frame_newslot(block821);
  params[0] = opresult820;
  params[1] = block821;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call859 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe814);
// compilenode returning call859
// Begin line 291
  setline(291);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe860 = gc_frame_new();
// Begin line 290
  setline(290);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  int op_slot_left_863 = gc_frame_newslot(*var_buildPath);
  if (strlit864 == NULL) {
    strlit864 = alloc_String("/minigrace ");
    gc_root(strlit864);
  }
// compilenode returning strlit864
  int op_slot_right_863 = gc_frame_newslot(strlit864);
  params[0] = strlit864;
  partcv[0] = 1;
  Object opresult866 = callmethod(*var_buildPath, "++", 1, partcv, params);
// compilenode returning opresult866
  int op_slot_left_862 = gc_frame_newslot(opresult866);
// compilenode returning *var_usrDir
  int op_slot_right_862 = gc_frame_newslot(*var_usrDir);
  params[0] = *var_usrDir;
  partcv[0] = 1;
  Object opresult868 = callmethod(opresult866, "++", 1, partcv, params);
// compilenode returning opresult868
  int op_slot_left_861 = gc_frame_newslot(opresult868);
  int callframe869 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call870 = callmethod(*var_file, "address",
    1, partcv, params);
  gc_frame_end(callframe869);
// compilenode returning call870
// compilenode returning call870
  int op_slot_right_861 = gc_frame_newslot(call870);
  params[0] = call870;
  partcv[0] = 1;
  Object opresult872 = callmethod(opresult868, "++", 1, partcv, params);
// compilenode returning opresult872
  gc_frame_newslot(opresult872);
// compilenode returning *var_io
  params[0] = opresult872;
  partcv[0] = 1;
  Object call873 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe860);
// compilenode returning call873
// Begin line 291
  setline(291);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool874 = alloc_Boolean(1);
// compilenode returning bool874
  return bool874;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply891(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_892 = gc_frame_newslot(*var_count);
  int callframe893 = gc_frame_new();
// compilenode returning *var_address
  partcv[0] = 0;
  Object call894 = callmethod(*var_address, "size",
    1, partcv, params);
  gc_frame_end(callframe893);
// compilenode returning call894
// compilenode returning call894
  int op_slot_right_892 = gc_frame_newslot(call894);
  params[0] = call894;
  partcv[0] = 1;
  Object opresult896 = callmethod(*var_count, "<=", 1, partcv, params);
// compilenode returning opresult896
  gc_frame_end(frame);
  return opresult896;
}
Object meth_PackageManager__apply899(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 303
  setline(303);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 302
  setline(302);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_nextPath
  int op_slot_left_900 = gc_frame_newslot(*var_nextPath);
// Begin line 303
  setline(303);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 302
  setline(302);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_address
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres901 = callmethod(*var_address, "[]", 1, partcv, params);
// compilenode returning idxres901
  int op_slot_right_900 = gc_frame_newslot(idxres901);
  params[0] = idxres901;
  partcv[0] = 1;
  Object opresult903 = callmethod(*var_nextPath, "++", 1, partcv, params);
// compilenode returning opresult903
  *var_nextPath = opresult903;
  if (opresult903 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 307
  setline(307);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 303
  setline(303);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_address
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres907 = callmethod(*var_address, "[]", 1, partcv, params);
// compilenode returning idxres907
  int op_slot_left_906 = gc_frame_newslot(idxres907);
  if (strlit908 == NULL) {
    strlit908 = alloc_String("/");
    gc_root(strlit908);
  }
// compilenode returning strlit908
  int op_slot_right_906 = gc_frame_newslot(strlit908);
  params[0] = strlit908;
  partcv[0] = 1;
  Object opresult910 = callmethod(idxres907, "==", 1, partcv, params);
// compilenode returning opresult910
  Object if905 = done;
  if (istrue(opresult910)) {
// Begin line 305
  setline(305);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 304
  setline(304);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toMake
  int op_slot_left_911 = gc_frame_newslot(*var_toMake);
// compilenode returning *var_nextPath
  int op_slot_right_911 = gc_frame_newslot(*var_nextPath);
  params[0] = *var_nextPath;
  partcv[0] = 1;
  Object opresult913 = callmethod(*var_toMake, "++", 1, partcv, params);
// compilenode returning opresult913
  *var_toMake = opresult913;
  if (opresult913 == undefined)
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
  if (strlit915 == NULL) {
    strlit915 = alloc_String("");
    gc_root(strlit915);
  }
// compilenode returning strlit915
  *var_nextPath = strlit915;
  if (strlit915 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if905 = done;
  } else {
  }
// compilenode returning if905
// Begin line 307
  setline(307);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  int op_slot_left_917 = gc_frame_newslot(*var_count);
  Object num918 = alloc_Float64(1);
// compilenode returning num918
  int op_slot_right_917 = gc_frame_newslot(num918);
  params[0] = num918;
  partcv[0] = 1;
  Object sum920 = callmethod(*var_count, "+", 1, partcv, params);
// compilenode returning sum920
  *var_count = sum920;
  if (sum920 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_createDirectory875(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 13, (flags>>24)&0xff);
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
// Begin line 297
  setline(297);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 296
  setline(296);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array876 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array876
  *var_dir = array876;
  if (array876 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 298
  setline(298);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 297
  setline(297);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit877 == NULL) {
    strlit877 = alloc_String("");
    gc_root(strlit877);
  }
// compilenode returning strlit877
  *var_nextPath = strlit877;
  if (strlit877 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 299
  setline(299);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 298
  setline(298);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit878 == NULL) {
    strlit878 = alloc_String("");
    gc_root(strlit878);
  }
// compilenode returning strlit878
  *var_toMake = strlit878;
  if (strlit878 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 300
  setline(300);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 299
  setline(299);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num879 = alloc_Float64(1);
// compilenode returning num879
  *var_count = num879;
  if (num879 == undefined)
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
  if (strlit882 == NULL) {
    strlit882 = alloc_String("Directory being evaluated ");
    gc_root(strlit882);
  }
// compilenode returning strlit882
  int op_slot_left_881 = gc_frame_newslot(strlit882);
// compilenode returning *var_address
  int op_slot_right_881 = gc_frame_newslot(*var_address);
  params[0] = *var_address;
  partcv[0] = 1;
  Object opresult884 = callmethod(strlit882, "++", 1, partcv, params);
// compilenode returning opresult884
  int op_slot_left_880 = gc_frame_newslot(opresult884);
  if (strlit885 == NULL) {
    strlit885 = alloc_String("");
    gc_root(strlit885);
  }
// compilenode returning strlit885
  int op_slot_right_880 = gc_frame_newslot(strlit885);
  params[0] = strlit885;
  partcv[0] = 1;
  Object opresult887 = callmethod(opresult884, "++", 1, partcv, params);
// compilenode returning opresult887
  params[0] = opresult887;
  Object call888 = gracelib_print(NULL, 1,  params);
// compilenode returning call888
// Begin line 309
  setline(309);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe889 = gc_frame_new();
// Begin line 301
  setline(301);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block890 = alloc_Block(NULL, NULL, "PackageManager", 301);
  gc_frame_newslot(block890);
  block_savedest(block890);
  Object closure891 = createclosure(3, "_apply");
setclosureframe(closure891, stackframe);
  addtoclosure(closure891, var_count);
  addtoclosure(closure891, var_address);
  addtoclosure(closure891, selfslot);
  struct UserObject *uo891 = (struct UserObject*)block890;
  uo891->data[0] = (Object)closure891;
  Method *meth_meth_PackageManager__apply891 = addmethod2pos(block890, "_apply", &meth_PackageManager__apply891, 0);
int argcv_meth_PackageManager__apply891[] = {0};
meth_meth_PackageManager__apply891->type = alloc_MethodType(1, argcv_meth_PackageManager__apply891);
  meth_meth_PackageManager__apply891->definitionModule = modulename;
  meth_meth_PackageManager__apply891->definitionLine = 290;
// compilenode returning block890
  gc_frame_newslot(block890);
// Begin line 309
  setline(309);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block898 = alloc_Block(NULL, NULL, "PackageManager", 309);
  gc_frame_newslot(block898);
  block_savedest(block898);
  Object closure899 = createclosure(5, "_apply");
setclosureframe(closure899, stackframe);
  addtoclosure(closure899, var_nextPath);
  addtoclosure(closure899, var_address);
  addtoclosure(closure899, var_count);
  addtoclosure(closure899, var_toMake);
  addtoclosure(closure899, selfslot);
  struct UserObject *uo899 = (struct UserObject*)block898;
  uo899->data[0] = (Object)closure899;
  Method *meth_meth_PackageManager__apply899 = addmethod2pos(block898, "_apply", &meth_PackageManager__apply899, 0);
int argcv_meth_PackageManager__apply899[] = {0};
meth_meth_PackageManager__apply899->type = alloc_MethodType(1, argcv_meth_PackageManager__apply899);
  meth_meth_PackageManager__apply899->definitionModule = modulename;
  meth_meth_PackageManager__apply899->definitionLine = 301;
// compilenode returning block898
  gc_frame_newslot(block898);
  params[0] = block890;
  params[1] = block898;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call923 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe889);
// compilenode returning call923
// Begin line 309
  setline(309);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit925 == NULL) {
    strlit925 = alloc_String("Creating directory ");
    gc_root(strlit925);
  }
// compilenode returning strlit925
  int op_slot_left_924 = gc_frame_newslot(strlit925);
// compilenode returning *var_toMake
  int op_slot_right_924 = gc_frame_newslot(*var_toMake);
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object opresult927 = callmethod(strlit925, "++", 1, partcv, params);
// compilenode returning opresult927
  params[0] = opresult927;
  Object call928 = gracelib_print(NULL, 1,  params);
// compilenode returning call928
// Begin line 314
  setline(314);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 310
  setline(310);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe930 = gc_frame_new();
  int callframe931 = gc_frame_new();
// compilenode returning *var_toMake
  gc_frame_newslot(*var_toMake);
// compilenode returning *var_io
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object call932 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe931);
// compilenode returning call932
  partcv[0] = 0;
  Object call933 = callmethod(call932, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe930);
// compilenode returning call933
  Object if929 = done;
  if (istrue(call933)) {
// Begin line 312
  setline(312);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe934 = gc_frame_new();
// Begin line 311
  setline(311);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit936 == NULL) {
    strlit936 = alloc_String("mkdir -p ");
    gc_root(strlit936);
  }
// compilenode returning strlit936
  int op_slot_left_935 = gc_frame_newslot(strlit936);
// compilenode returning *var_toMake
  int op_slot_right_935 = gc_frame_newslot(*var_toMake);
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object opresult938 = callmethod(strlit936, "++", 1, partcv, params);
// compilenode returning opresult938
  gc_frame_newslot(opresult938);
// compilenode returning *var_io
  params[0] = opresult938;
  partcv[0] = 1;
  Object call939 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe934);
// compilenode returning call939
// Begin line 313
  setline(313);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 312
  setline(312);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit942 == NULL) {
    strlit942 = alloc_String("made directory ");
    gc_root(strlit942);
  }
// compilenode returning strlit942
  int op_slot_left_941 = gc_frame_newslot(strlit942);
// compilenode returning *var_toMake
  int op_slot_right_941 = gc_frame_newslot(*var_toMake);
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object opresult944 = callmethod(strlit942, "++", 1, partcv, params);
// compilenode returning opresult944
  int op_slot_left_940 = gc_frame_newslot(opresult944);
  if (strlit945 == NULL) {
    strlit945 = alloc_String("");
    gc_root(strlit945);
  }
// compilenode returning strlit945
  int op_slot_right_940 = gc_frame_newslot(strlit945);
  params[0] = strlit945;
  partcv[0] = 1;
  Object opresult947 = callmethod(opresult944, "++", 1, partcv, params);
// compilenode returning opresult947
  params[0] = opresult947;
  Object call948 = gracelib_print(NULL, 1,  params);
// compilenode returning call948
    gc_frame_newslot(call948);
    if929 = call948;
  } else {
  }
// compilenode returning if929
// Begin line 315
  setline(315);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe949 = gc_frame_new();
// Begin line 314
  setline(314);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toMake
  gc_frame_newslot(*var_toMake);
// compilenode returning *var_dir
  params[0] = *var_toMake;
  partcv[0] = 1;
  Object call950 = callmethod(*var_dir, "push",
    1, partcv, params);
  gc_frame_end(callframe949);
// compilenode returning call950
// Begin line 316
  setline(316);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe951 = gc_frame_new();
// Begin line 315
  setline(315);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_nextPath
  gc_frame_newslot(*var_nextPath);
// compilenode returning *var_dir
  params[0] = *var_nextPath;
  partcv[0] = 1;
  Object call952 = callmethod(*var_dir, "push",
    1, partcv, params);
  gc_frame_end(callframe951);
// compilenode returning call952
// Begin line 317
  setline(317);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 316
  setline(316);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_dir
  return *var_dir;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply959(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_960 = gc_frame_newslot(*var_curPos);
  int callframe961 = gc_frame_new();
// compilenode returning *var_data
  partcv[0] = 0;
  Object call962 = callmethod(*var_data, "size",
    1, partcv, params);
  gc_frame_end(callframe961);
// compilenode returning call962
// compilenode returning call962
  int op_slot_right_960 = gc_frame_newslot(call962);
  params[0] = call962;
  partcv[0] = 1;
  Object opresult964 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult964
  gc_frame_end(frame);
  return opresult964;
}
Object meth_PackageManager__apply971(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_973 = gc_frame_newslot(*var_curPos);
  int callframe974 = gc_frame_new();
// compilenode returning *var_data
  partcv[0] = 0;
  Object call975 = callmethod(*var_data, "size",
    1, partcv, params);
  gc_frame_end(callframe974);
// compilenode returning call975
// compilenode returning call975
  int op_slot_right_973 = gc_frame_newslot(call975);
  params[0] = call975;
  partcv[0] = 1;
  Object opresult977 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult977
  int op_slot_left_972 = gc_frame_newslot(opresult977);
// compilenode returning *var_data
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres979 = callmethod(*var_data, "[]", 1, partcv, params);
// compilenode returning idxres979
  int op_slot_left_978 = gc_frame_newslot(idxres979);
  if (strlit980 == NULL) {
    strlit980 = alloc_String("""\x0a""");
    gc_root(strlit980);
  }
// compilenode returning strlit980
  int op_slot_right_978 = gc_frame_newslot(strlit980);
  params[0] = strlit980;
  partcv[0] = 1;
  Object opresult982 = callmethod(idxres979, "!=", 1, partcv, params);
// compilenode returning opresult982
  int op_slot_right_972 = gc_frame_newslot(opresult982);
  params[0] = opresult982;
  partcv[0] = 1;
  Object opresult984 = callmethod(opresult977, "&&", 1, partcv, params);
// compilenode returning opresult984
  gc_frame_end(frame);
  return opresult984;
}
Object meth_PackageManager__apply987(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 326
  setline(326);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_988 = gc_frame_newslot(*var_curPos);
  Object num989 = alloc_Float64(1);
// compilenode returning num989
  int op_slot_right_988 = gc_frame_newslot(num989);
  params[0] = num989;
  partcv[0] = 1;
  Object sum991 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum991
  *var_curPos = sum991;
  if (sum991 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply967(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 325
  setline(325);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 324
  setline(324);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 328
  setline(328);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe969 = gc_frame_new();
// Begin line 325
  setline(325);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block970 = alloc_Block(NULL, NULL, "PackageManager", 325);
  gc_frame_newslot(block970);
  block_savedest(block970);
  Object closure971 = createclosure(3, "_apply");
setclosureframe(closure971, stackframe);
  addtoclosure(closure971, var_curPos);
  addtoclosure(closure971, var_data);
  addtoclosure(closure971, selfslot);
  struct UserObject *uo971 = (struct UserObject*)block970;
  uo971->data[0] = (Object)closure971;
  Method *meth_meth_PackageManager__apply971 = addmethod2pos(block970, "_apply", &meth_PackageManager__apply971, 0);
int argcv_meth_PackageManager__apply971[] = {0};
meth_meth_PackageManager__apply971->type = alloc_MethodType(1, argcv_meth_PackageManager__apply971);
  meth_meth_PackageManager__apply971->definitionModule = modulename;
  meth_meth_PackageManager__apply971->definitionLine = 323;
// compilenode returning block970
  gc_frame_newslot(block970);
// Begin line 328
  setline(328);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block986 = alloc_Block(NULL, NULL, "PackageManager", 328);
  gc_frame_newslot(block986);
  block_savedest(block986);
  Object closure987 = createclosure(2, "_apply");
setclosureframe(closure987, stackframe);
  addtoclosure(closure987, var_curPos);
  addtoclosure(closure987, selfslot);
  struct UserObject *uo987 = (struct UserObject*)block986;
  uo987->data[0] = (Object)closure987;
  Method *meth_meth_PackageManager__apply987 = addmethod2pos(block986, "_apply", &meth_PackageManager__apply987, 0);
int argcv_meth_PackageManager__apply987[] = {0};
meth_meth_PackageManager__apply987->type = alloc_MethodType(1, argcv_meth_PackageManager__apply987);
  meth_meth_PackageManager__apply987->definitionModule = modulename;
  meth_meth_PackageManager__apply987->definitionLine = 325;
// compilenode returning block986
  gc_frame_newslot(block986);
  params[0] = block970;
  params[1] = block986;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call994 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe969);
// compilenode returning call994
// Begin line 329
  setline(329);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe995 = gc_frame_new();
// Begin line 328
  setline(328);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_startPos
  gc_frame_newslot(*var_startPos);
// compilenode returning *var_curPos
  int op_slot_left_996 = gc_frame_newslot(*var_curPos);
  Object num997 = alloc_Float64(1);
// compilenode returning num997
  int op_slot_right_996 = gc_frame_newslot(num997);
  params[0] = num997;
  partcv[0] = 1;
  Object diff999 = callmethod(*var_curPos, "-", 1, partcv, params);
// compilenode returning diff999
  gc_frame_newslot(diff999);
// compilenode returning *var_data
  params[0] = *var_startPos;
  params[1] = diff999;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1000 = callmethod(*var_data, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe995);
// compilenode returning call1000
  *var_line = call1000;
  if (call1000 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 332
  setline(332);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 329
  setline(329);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1002 = gc_frame_new();
  int callframe1003 = gc_frame_new();
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
  params[0] = *var_line;
  partcv[0] = 1;
  Object call1004 = callmethodflags(self, "processLine", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1003);
// compilenode returning call1004
  partcv[0] = 0;
  Object call1005 = callmethod(call1004, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1002);
// compilenode returning call1005
  Object if1001 = done;
  if (istrue(call1005)) {
// Begin line 331
  setline(331);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_done
  block_return(realself, *var_done);
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1001 = undefined;
  } else {
  }
// compilenode returning if1001
// Begin line 333
  setline(333);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 332
  setline(332);
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
Object meth_PackageManager_parseFile953(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 14, (flags>>24)&0xff);
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
// Begin line 321
  setline(321);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 320
  setline(320);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe954 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call955 = callmethod(*var_file, "data",
    1, partcv, params);
  gc_frame_end(callframe954);
// compilenode returning call955
// compilenode returning call955
  *var_data = call955;
  if (call955 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 321
  setline(321);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num956 = alloc_Float64(1);
// compilenode returning num956
  *var_curPos = num956;
  if (num956 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 323
  setline(323);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 322
  setline(322);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 334
  setline(334);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe957 = gc_frame_new();
// Begin line 323
  setline(323);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block958 = alloc_Block(NULL, NULL, "PackageManager", 323);
  gc_frame_newslot(block958);
  block_savedest(block958);
  Object closure959 = createclosure(3, "_apply");
setclosureframe(closure959, stackframe);
  addtoclosure(closure959, var_curPos);
  addtoclosure(closure959, var_data);
  addtoclosure(closure959, selfslot);
  struct UserObject *uo959 = (struct UserObject*)block958;
  uo959->data[0] = (Object)closure959;
  Method *meth_meth_PackageManager__apply959 = addmethod2pos(block958, "_apply", &meth_PackageManager__apply959, 0);
int argcv_meth_PackageManager__apply959[] = {0};
meth_meth_PackageManager__apply959->type = alloc_MethodType(1, argcv_meth_PackageManager__apply959);
  meth_meth_PackageManager__apply959->definitionModule = modulename;
  meth_meth_PackageManager__apply959->definitionLine = 320;
// compilenode returning block958
  gc_frame_newslot(block958);
// Begin line 334
  setline(334);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block966 = alloc_Block(NULL, NULL, "PackageManager", 334);
  gc_frame_newslot(block966);
  block_savedest(block966);
  Object closure967 = createclosure(5, "_apply");
setclosureframe(closure967, stackframe);
  addtoclosure(closure967, var_curPos);
  addtoclosure(closure967, var_startPos);
  addtoclosure(closure967, var_data);
  addtoclosure(closure967, var_done);
  addtoclosure(closure967, selfslot);
  struct UserObject *uo967 = (struct UserObject*)block966;
  uo967->data[0] = (Object)closure967;
  Method *meth_meth_PackageManager__apply967 = addmethod2pos(block966, "_apply", &meth_PackageManager__apply967, 0);
int argcv_meth_PackageManager__apply967[] = {0};
meth_meth_PackageManager__apply967->type = alloc_MethodType(1, argcv_meth_PackageManager__apply967);
  meth_meth_PackageManager__apply967->definitionModule = modulename;
  meth_meth_PackageManager__apply967->definitionLine = 323;
// compilenode returning block966
  gc_frame_newslot(block966);
  params[0] = block958;
  params[1] = block966;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1012 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe957);
// compilenode returning call1012
  gc_frame_end(frame);
  return call1012;
}
Object meth_PackageManager_processLine1013(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 352
  setline(352);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 337
  setline(337);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1016 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1017 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1016);
// compilenode returning call1017
// compilenode returning call1017
  int op_slot_left_1015 = gc_frame_newslot(call1017);
  Object num1018 = alloc_Float64(1);
// compilenode returning num1018
  int op_slot_right_1015 = gc_frame_newslot(num1018);
  params[0] = num1018;
  partcv[0] = 1;
  Object opresult1020 = callmethod(call1017, ">", 1, partcv, params);
// compilenode returning opresult1020
  Object if1014 = done;
  if (istrue(opresult1020)) {
// Begin line 351
  setline(351);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 338
  setline(338);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  Object num1023 = alloc_Float64(1);
// compilenode returning num1023
  params[0] = num1023;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1024 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1024
  int op_slot_left_1022 = gc_frame_newslot(idxres1024);
  if (strlit1025 == NULL) {
    strlit1025 = alloc_String("#");
    gc_root(strlit1025);
  }
// compilenode returning strlit1025
  int op_slot_right_1022 = gc_frame_newslot(strlit1025);
  params[0] = strlit1025;
  partcv[0] = 1;
  Object opresult1027 = callmethod(idxres1024, "==", 1, partcv, params);
// compilenode returning opresult1027
  Object if1021 = done;
  if (istrue(opresult1027)) {
// Begin line 340
  setline(340);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 339
  setline(339);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1028 = alloc_Boolean(1);
// compilenode returning bool1028
  return bool1028;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1021 = undefined;
  } else {
// Begin line 344
  setline(344);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 341
  setline(341);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1032 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1033 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1032);
// compilenode returning call1033
// compilenode returning call1033
  int op_slot_left_1031 = gc_frame_newslot(call1033);
  Object num1034 = alloc_Float64(2);
// compilenode returning num1034
  int op_slot_right_1031 = gc_frame_newslot(num1034);
  params[0] = num1034;
  partcv[0] = 1;
  Object opresult1036 = callmethod(call1033, ">", 1, partcv, params);
// compilenode returning opresult1036
  int op_slot_left_1030 = gc_frame_newslot(opresult1036);
  int callframe1038 = gc_frame_new();
  Object num1039 = alloc_Float64(1);
// compilenode returning num1039
  gc_frame_newslot(num1039);
  Object num1040 = alloc_Float64(2);
// compilenode returning num1040
  gc_frame_newslot(num1040);
// compilenode returning *var_line
  params[0] = num1039;
  params[1] = num1040;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1041 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1038);
// compilenode returning call1041
  int op_slot_left_1037 = gc_frame_newslot(call1041);
  if (strlit1042 == NULL) {
    strlit1042 = alloc_String("//");
    gc_root(strlit1042);
  }
// compilenode returning strlit1042
  int op_slot_right_1037 = gc_frame_newslot(strlit1042);
  params[0] = strlit1042;
  partcv[0] = 1;
  Object opresult1044 = callmethod(call1041, "==", 1, partcv, params);
// compilenode returning opresult1044
  int op_slot_right_1030 = gc_frame_newslot(opresult1044);
  params[0] = opresult1044;
  partcv[0] = 1;
  Object opresult1046 = callmethod(opresult1036, "&&", 1, partcv, params);
// compilenode returning opresult1046
  Object if1029 = done;
  if (istrue(opresult1046)) {
// Begin line 343
  setline(343);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 342
  setline(342);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1047 = alloc_Boolean(1);
// compilenode returning bool1047
  return bool1047;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1029 = undefined;
  } else {
// Begin line 348
  setline(348);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 344
  setline(344);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1051 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1052 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1051);
// compilenode returning call1052
// compilenode returning call1052
  int op_slot_left_1050 = gc_frame_newslot(call1052);
  Object num1053 = alloc_Float64(6);
// compilenode returning num1053
  int op_slot_right_1050 = gc_frame_newslot(num1053);
  params[0] = num1053;
  partcv[0] = 1;
  Object opresult1055 = callmethod(call1052, ">", 1, partcv, params);
// compilenode returning opresult1055
  int op_slot_left_1049 = gc_frame_newslot(opresult1055);
  int callframe1057 = gc_frame_new();
  Object num1058 = alloc_Float64(1);
// compilenode returning num1058
  gc_frame_newslot(num1058);
  Object num1059 = alloc_Float64(7);
// compilenode returning num1059
  gc_frame_newslot(num1059);
// compilenode returning *var_line
  params[0] = num1058;
  params[1] = num1059;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1060 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1057);
// compilenode returning call1060
  int op_slot_left_1056 = gc_frame_newslot(call1060);
  if (strlit1061 == NULL) {
    strlit1061 = alloc_String("import ");
    gc_root(strlit1061);
  }
// compilenode returning strlit1061
  int op_slot_right_1056 = gc_frame_newslot(strlit1061);
  params[0] = strlit1061;
  partcv[0] = 1;
  Object opresult1063 = callmethod(call1060, "==", 1, partcv, params);
// compilenode returning opresult1063
  int op_slot_right_1049 = gc_frame_newslot(opresult1063);
  params[0] = opresult1063;
  partcv[0] = 1;
  Object opresult1065 = callmethod(opresult1055, "&&", 1, partcv, params);
// compilenode returning opresult1065
  Object if1048 = done;
  if (istrue(opresult1065)) {
// Begin line 345
  setline(345);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1066 = gc_frame_new();
  int callframe1067 = gc_frame_new();
  Object num1068 = alloc_Float64(8);
// compilenode returning num1068
  gc_frame_newslot(num1068);
  int callframe1069 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1070 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1069);
// compilenode returning call1070
// compilenode returning call1070
  gc_frame_newslot(call1070);
// compilenode returning *var_line
  params[0] = num1068;
  params[1] = call1070;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1071 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1067);
// compilenode returning call1071
  gc_frame_newslot(call1071);
  params[0] = call1071;
  partcv[0] = 1;
  Object call1072 = callmethodflags(self, "parseImport", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1066);
// compilenode returning call1072
// Begin line 347
  setline(347);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 346
  setline(346);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1073 = alloc_Boolean(1);
// compilenode returning bool1073
  return bool1073;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1048 = undefined;
  } else {
// Begin line 351
  setline(351);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 348
  setline(348);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1077 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1078 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1077);
// compilenode returning call1078
// compilenode returning call1078
  int op_slot_left_1076 = gc_frame_newslot(call1078);
  Object num1079 = alloc_Float64(7);
// compilenode returning num1079
  int op_slot_right_1076 = gc_frame_newslot(num1079);
  params[0] = num1079;
  partcv[0] = 1;
  Object opresult1081 = callmethod(call1078, ">", 1, partcv, params);
// compilenode returning opresult1081
  int op_slot_left_1075 = gc_frame_newslot(opresult1081);
  int callframe1083 = gc_frame_new();
  Object num1084 = alloc_Float64(1);
// compilenode returning num1084
  gc_frame_newslot(num1084);
  Object num1085 = alloc_Float64(8);
// compilenode returning num1085
  gc_frame_newslot(num1085);
// compilenode returning *var_line
  params[0] = num1084;
  params[1] = num1085;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1086 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1083);
// compilenode returning call1086
  int op_slot_left_1082 = gc_frame_newslot(call1086);
  if (strlit1087 == NULL) {
    strlit1087 = alloc_String("dialect ");
    gc_root(strlit1087);
  }
// compilenode returning strlit1087
  int op_slot_right_1082 = gc_frame_newslot(strlit1087);
  params[0] = strlit1087;
  partcv[0] = 1;
  Object opresult1089 = callmethod(call1086, "==", 1, partcv, params);
// compilenode returning opresult1089
  int op_slot_right_1075 = gc_frame_newslot(opresult1089);
  params[0] = opresult1089;
  partcv[0] = 1;
  Object opresult1091 = callmethod(opresult1081, "&&", 1, partcv, params);
// compilenode returning opresult1091
  Object if1074 = done;
  if (istrue(opresult1091)) {
// Begin line 350
  setline(350);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 349
  setline(349);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1092 = alloc_Boolean(1);
// compilenode returning bool1092
  return bool1092;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1074 = undefined;
  } else {
  }
// compilenode returning if1074
    gc_frame_newslot(if1074);
    if1048 = if1074;
  }
// compilenode returning if1048
    gc_frame_newslot(if1048);
    if1029 = if1048;
  }
// compilenode returning if1029
    gc_frame_newslot(if1029);
    if1021 = if1029;
  }
// compilenode returning if1021
    gc_frame_newslot(if1021);
    if1014 = if1021;
  } else {
  }
// compilenode returning if1014
// Begin line 353
  setline(353);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 352
  setline(352);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object bool1093 = alloc_Boolean(0);
// compilenode returning bool1093
  return bool1093;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager__apply1108(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1110 = gc_frame_newslot(*var_curPos);
  int callframe1111 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1112 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1111);
// compilenode returning call1112
// compilenode returning call1112
  int op_slot_right_1110 = gc_frame_newslot(call1112);
  params[0] = call1112;
  partcv[0] = 1;
  Object opresult1114 = callmethod(*var_curPos, "<", 1, partcv, params);
// compilenode returning opresult1114
  int op_slot_left_1109 = gc_frame_newslot(opresult1114);
  int callframe1115 = gc_frame_new();
// compilenode returning *var_line
// compilenode returning *var_curPos
  int op_slot_left_1118 = gc_frame_newslot(*var_curPos);
  Object num1119 = alloc_Float64(1);
// compilenode returning num1119
  int op_slot_right_1118 = gc_frame_newslot(num1119);
  params[0] = num1119;
  partcv[0] = 1;
  Object sum1121 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1121
  params[0] = sum1121;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1122 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1122
  int op_slot_left_1117 = gc_frame_newslot(idxres1122);
  if (strlit1123 == NULL) {
    strlit1123 = alloc_String("""\x22""");
    gc_root(strlit1123);
  }
// compilenode returning strlit1123
  int op_slot_right_1117 = gc_frame_newslot(strlit1123);
  params[0] = strlit1123;
  partcv[0] = 1;
  Object opresult1125 = callmethod(idxres1122, "==", 1, partcv, params);
// compilenode returning opresult1125
  int op_slot_left_1116 = gc_frame_newslot(opresult1125);
// compilenode returning *var_line
// compilenode returning *var_curPos
  int op_slot_left_1127 = gc_frame_newslot(*var_curPos);
  Object num1128 = alloc_Float64(1);
// compilenode returning num1128
  int op_slot_right_1127 = gc_frame_newslot(num1128);
  params[0] = num1128;
  partcv[0] = 1;
  Object sum1130 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1130
  params[0] = sum1130;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1131 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1131
  int op_slot_left_1126 = gc_frame_newslot(idxres1131);
  if (strlit1132 == NULL) {
    strlit1132 = alloc_String(" ");
    gc_root(strlit1132);
  }
// compilenode returning strlit1132
  int op_slot_right_1126 = gc_frame_newslot(strlit1132);
  params[0] = strlit1132;
  partcv[0] = 1;
  Object opresult1134 = callmethod(idxres1131, "==", 1, partcv, params);
// compilenode returning opresult1134
  int op_slot_right_1116 = gc_frame_newslot(opresult1134);
  params[0] = opresult1134;
  partcv[0] = 1;
  Object opresult1136 = callmethod(opresult1125, "||", 1, partcv, params);
// compilenode returning opresult1136
  partcv[0] = 0;
  Object call1137 = callmethod(opresult1136, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1115);
// compilenode returning call1137
  int op_slot_right_1109 = gc_frame_newslot(call1137);
  params[0] = call1137;
  partcv[0] = 1;
  Object opresult1139 = callmethod(opresult1114, "&&", 1, partcv, params);
// compilenode returning opresult1139
  gc_frame_end(frame);
  return opresult1139;
}
Object meth_PackageManager__apply1142(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 364
  setline(364);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 363
  setline(363);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1143 = gc_frame_newslot(*var_curPos);
  Object num1144 = alloc_Float64(1);
// compilenode returning num1144
  int op_slot_right_1143 = gc_frame_newslot(num1144);
  params[0] = num1144;
  partcv[0] = 1;
  Object sum1146 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1146
  *var_curPos = sum1146;
  if (sum1146 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_parseImport1094(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 16, (flags>>24)&0xff);
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
// Begin line 357
  setline(357);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 356
  setline(356);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1095 = alloc_Float64(1);
// compilenode returning num1095
  *var_curPos = num1095;
  if (num1095 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 358
  setline(358);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 357
  setline(357);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 359
  setline(359);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 358
  setline(358);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1096 == NULL) {
    strlit1096 = alloc_String("");
    gc_root(strlit1096);
  }
// compilenode returning strlit1096
  *var_nextImport = strlit1096;
  if (strlit1096 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 360
  setline(360);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1097 = gc_frame_new();
// Begin line 359
  setline(359);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// compilenode returning *var_curPos
  gc_frame_newslot(*var_curPos);
  params[0] = *var_line;
  params[1] = *var_curPos;
  partcv[0] = 2;
  Object call1098 = callmethodflags(self, "skipWhiteSpace", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1097);
// compilenode returning call1098
  *var_curPos = call1098;
  if (call1098 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 361
  setline(361);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 360
  setline(360);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1100 = gc_frame_newslot(*var_curPos);
  Object num1101 = alloc_Float64(1);
// compilenode returning num1101
  int op_slot_right_1100 = gc_frame_newslot(num1101);
  params[0] = num1101;
  partcv[0] = 1;
  Object sum1103 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1103
  *var_curPos = sum1103;
  if (sum1103 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 362
  setline(362);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 361
  setline(361);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 365
  setline(365);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1106 = gc_frame_new();
// Begin line 362
  setline(362);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1107 = alloc_Block(NULL, NULL, "PackageManager", 362);
  gc_frame_newslot(block1107);
  block_savedest(block1107);
  Object closure1108 = createclosure(3, "_apply");
setclosureframe(closure1108, stackframe);
  addtoclosure(closure1108, var_curPos);
  addtoclosure(closure1108, var_line);
  addtoclosure(closure1108, selfslot);
  struct UserObject *uo1108 = (struct UserObject*)block1107;
  uo1108->data[0] = (Object)closure1108;
  Method *meth_meth_PackageManager__apply1108 = addmethod2pos(block1107, "_apply", &meth_PackageManager__apply1108, 0);
int argcv_meth_PackageManager__apply1108[] = {0};
meth_meth_PackageManager__apply1108->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1108);
  meth_meth_PackageManager__apply1108->definitionModule = modulename;
  meth_meth_PackageManager__apply1108->definitionLine = 348;
// compilenode returning block1107
  gc_frame_newslot(block1107);
// Begin line 365
  setline(365);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1141 = alloc_Block(NULL, NULL, "PackageManager", 365);
  gc_frame_newslot(block1141);
  block_savedest(block1141);
  Object closure1142 = createclosure(2, "_apply");
setclosureframe(closure1142, stackframe);
  addtoclosure(closure1142, var_curPos);
  addtoclosure(closure1142, selfslot);
  struct UserObject *uo1142 = (struct UserObject*)block1141;
  uo1142->data[0] = (Object)closure1142;
  Method *meth_meth_PackageManager__apply1142 = addmethod2pos(block1141, "_apply", &meth_PackageManager__apply1142, 0);
int argcv_meth_PackageManager__apply1142[] = {0};
meth_meth_PackageManager__apply1142->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1142);
  meth_meth_PackageManager__apply1142->definitionModule = modulename;
  meth_meth_PackageManager__apply1142->definitionLine = 362;
// compilenode returning block1141
  gc_frame_newslot(block1141);
  params[0] = block1107;
  params[1] = block1141;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1149 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1106);
// compilenode returning call1149
// Begin line 366
  setline(366);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1150 = gc_frame_new();
// Begin line 365
  setline(365);
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
  Object call1151 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1150);
// compilenode returning call1151
  *var_nextImport = call1151;
  if (call1151 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 370
  setline(370);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 366
  setline(366);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1154 = gc_frame_new();
  int callframe1155 = gc_frame_new();
// compilenode returning *var_nextImport
  gc_frame_newslot(*var_nextImport);
// compilenode returning *var_imported
  params[0] = *var_nextImport;
  partcv[0] = 1;
  Object call1156 = callmethod(*var_imported, "contains",
    1, partcv, params);
  gc_frame_end(callframe1155);
// compilenode returning call1156
  partcv[0] = 0;
  Object call1157 = callmethod(call1156, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1154);
// compilenode returning call1157
  Object if1153 = done;
  if (istrue(call1157)) {
// Begin line 368
  setline(368);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 367
  setline(367);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1159 == NULL) {
    strlit1159 = alloc_String("next import = ");
    gc_root(strlit1159);
  }
// compilenode returning strlit1159
  int op_slot_left_1158 = gc_frame_newslot(strlit1159);
// compilenode returning *var_nextImport
  int op_slot_right_1158 = gc_frame_newslot(*var_nextImport);
  params[0] = *var_nextImport;
  partcv[0] = 1;
  Object opresult1161 = callmethod(strlit1159, "++", 1, partcv, params);
// compilenode returning opresult1161
  params[0] = opresult1161;
  Object call1162 = gracelib_print(NULL, 1,  params);
// compilenode returning call1162
// Begin line 369
  setline(369);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1163 = gc_frame_new();
// Begin line 368
  setline(368);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_nextImport
  gc_frame_newslot(*var_nextImport);
// compilenode returning *var_toProcess
  params[0] = *var_nextImport;
  partcv[0] = 1;
  Object call1164 = callmethod(*var_toProcess, "push",
    1, partcv, params);
  gc_frame_end(callframe1163);
// compilenode returning call1164
    gc_frame_newslot(call1164);
    if1153 = call1164;
  } else {
  }
// compilenode returning if1153
  gc_frame_end(frame);
  return if1153;
}
Object meth_PackageManager__apply1168(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1170 = gc_frame_newslot(*var_count2);
  int callframe1171 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1172 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1171);
// compilenode returning call1172
// compilenode returning call1172
  int op_slot_right_1170 = gc_frame_newslot(call1172);
  params[0] = call1172;
  partcv[0] = 1;
  Object opresult1174 = callmethod(*var_count2, "<=", 1, partcv, params);
// compilenode returning opresult1174
  int op_slot_left_1169 = gc_frame_newslot(opresult1174);
// compilenode returning *var_line
// compilenode returning *var_count2
  params[0] = *var_count2;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1176 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1176
  int op_slot_left_1175 = gc_frame_newslot(idxres1176);
  if (strlit1177 == NULL) {
    strlit1177 = alloc_String("Could not retrieve ");
    gc_root(strlit1177);
  }
// compilenode returning strlit1177
  int op_slot_right_1175 = gc_frame_newslot(strlit1177);
  params[0] = strlit1177;
  partcv[0] = 1;
  Object opresult1179 = callmethod(idxres1176, "==", 1, partcv, params);
// compilenode returning opresult1179
  int op_slot_right_1169 = gc_frame_newslot(opresult1179);
  params[0] = opresult1179;
  partcv[0] = 1;
  Object opresult1181 = callmethod(opresult1174, "&&", 1, partcv, params);
// compilenode returning opresult1181
  gc_frame_end(frame);
  return opresult1181;
}
Object meth_PackageManager__apply1184(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 376
  setline(376);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 375
  setline(375);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count2
  int op_slot_left_1185 = gc_frame_newslot(*var_count2);
  Object num1186 = alloc_Float64(1);
// compilenode returning num1186
  int op_slot_right_1185 = gc_frame_newslot(num1186);
  params[0] = num1186;
  partcv[0] = 1;
  Object sum1188 = callmethod(*var_count2, "+", 1, partcv, params);
// compilenode returning sum1188
  *var_count2 = sum1188;
  if (sum1188 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager_skipWhiteSpace1165(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 374
  setline(374);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 373
  setline(373);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  *var_count2 = *var_count;
  if (*var_count == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 377
  setline(377);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1166 = gc_frame_new();
// Begin line 374
  setline(374);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1167 = alloc_Block(NULL, NULL, "PackageManager", 374);
  gc_frame_newslot(block1167);
  block_savedest(block1167);
  Object closure1168 = createclosure(3, "_apply");
setclosureframe(closure1168, stackframe);
  addtoclosure(closure1168, var_count2);
  addtoclosure(closure1168, var_line);
  addtoclosure(closure1168, selfslot);
  struct UserObject *uo1168 = (struct UserObject*)block1167;
  uo1168->data[0] = (Object)closure1168;
  Method *meth_meth_PackageManager__apply1168 = addmethod2pos(block1167, "_apply", &meth_PackageManager__apply1168, 0);
int argcv_meth_PackageManager__apply1168[] = {0};
meth_meth_PackageManager__apply1168->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1168);
  meth_meth_PackageManager__apply1168->definitionModule = modulename;
  meth_meth_PackageManager__apply1168->definitionLine = 362;
// compilenode returning block1167
  gc_frame_newslot(block1167);
// Begin line 377
  setline(377);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1183 = alloc_Block(NULL, NULL, "PackageManager", 377);
  gc_frame_newslot(block1183);
  block_savedest(block1183);
  Object closure1184 = createclosure(2, "_apply");
setclosureframe(closure1184, stackframe);
  addtoclosure(closure1184, var_count2);
  addtoclosure(closure1184, selfslot);
  struct UserObject *uo1184 = (struct UserObject*)block1183;
  uo1184->data[0] = (Object)closure1184;
  Method *meth_meth_PackageManager__apply1184 = addmethod2pos(block1183, "_apply", &meth_PackageManager__apply1184, 0);
int argcv_meth_PackageManager__apply1184[] = {0};
meth_meth_PackageManager__apply1184->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1184);
  meth_meth_PackageManager__apply1184->definitionModule = modulename;
  meth_meth_PackageManager__apply1184->definitionLine = 374;
// compilenode returning block1183
  gc_frame_newslot(block1183);
  params[0] = block1167;
  params[1] = block1183;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1191 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1166);
// compilenode returning call1191
// Begin line 377
  setline(377);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count2
  gc_frame_end(frame);
  return *var_count2;
}
Object meth_PackageManager_displayHelp1192(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 382
  setline(382);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 381
  setline(381);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1193 == NULL) {
    strlit1193 = alloc_String("Available options are:");
    gc_root(strlit1193);
  }
// compilenode returning strlit1193
  params[0] = strlit1193;
  Object call1194 = gracelib_print(NULL, 1,  params);
// compilenode returning call1194
// Begin line 383
  setline(383);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 382
  setline(382);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1195 == NULL) {
    strlit1195 = alloc_String("-get [url] to fetch a package located at a given url");
    gc_root(strlit1195);
  }
// compilenode returning strlit1195
  params[0] = strlit1195;
  Object call1196 = gracelib_print(NULL, 1,  params);
// compilenode returning call1196
// Begin line 384
  setline(384);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 383
  setline(383);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1197 == NULL) {
    strlit1197 = alloc_String("-list to display currenly installed packages");
    gc_root(strlit1197);
  }
// compilenode returning strlit1197
  params[0] = strlit1197;
  Object call1198 = gracelib_print(NULL, 1,  params);
// compilenode returning call1198
  gc_frame_end(frame);
  return call1198;
}
Object meth_PackageManager_printMessage1199(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 19, (flags>>24)&0xff);
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
// Begin line 390
  setline(390);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 387
  setline(387);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_verbose
  Object if1200 = done;
  if (istrue(*var_verbose)) {
// Begin line 389
  setline(389);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 388
  setline(388);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_message
  params[0] = *var_message;
  Object call1201 = gracelib_print(NULL, 1,  params);
// compilenode returning call1201
    gc_frame_newslot(call1201);
    if1200 = call1201;
  } else {
  }
// compilenode returning if1200
  gc_frame_end(frame);
  return if1200;
}
Object meth_PackageManager__apply1271(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int callframe1273 = gc_frame_new();
// compilenode returning *var_imported
  partcv[0] = 0;
  Object call1274 = callmethod(*var_imported, "size",
    1, partcv, params);
  gc_frame_end(callframe1273);
// compilenode returning call1274
// compilenode returning call1274
  int op_slot_left_1272 = gc_frame_newslot(call1274);
  Object num1275 = alloc_Float64(0);
// compilenode returning num1275
  int op_slot_right_1272 = gc_frame_newslot(num1275);
  params[0] = num1275;
  partcv[0] = 1;
  Object opresult1277 = callmethod(call1274, ">", 1, partcv, params);
// compilenode returning opresult1277
  gc_frame_end(frame);
  return opresult1277;
}
Object meth_PackageManager__apply1280(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 411
  setline(411);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 410
  setline(410);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1281 = gc_frame_new();
// compilenode returning *var_imported
  partcv[0] = 0;
  Object call1282 = callmethod(*var_imported, "pop",
    1, partcv, params);
  gc_frame_end(callframe1281);
// compilenode returning call1282
// compilenode returning call1282
  *var_curImport = call1282;
  if (call1282 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 412
  setline(412);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1283 = gc_frame_new();
// Begin line 411
  setline(411);
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
  int callframe1294 = gc_frame_new();
// compilenode returning *var_curImport
  partcv[0] = 0;
  Object call1295 = callmethod(*var_curImport, "address",
    1, partcv, params);
  gc_frame_end(callframe1294);
// compilenode returning call1295
// compilenode returning call1295
  int op_slot_right_1285 = gc_frame_newslot(call1295);
  params[0] = call1295;
  partcv[0] = 1;
  Object opresult1297 = callmethod(opresult1293, "++", 1, partcv, params);
// compilenode returning opresult1297
  int op_slot_left_1284 = gc_frame_newslot(opresult1297);
  if (strlit1298 == NULL) {
    strlit1298 = alloc_String("");
    gc_root(strlit1298);
  }
// compilenode returning strlit1298
  int op_slot_right_1284 = gc_frame_newslot(strlit1298);
  params[0] = strlit1298;
  partcv[0] = 1;
  Object opresult1300 = callmethod(opresult1297, "++", 1, partcv, params);
// compilenode returning opresult1300
  gc_frame_newslot(opresult1300);
  if (strlit1301 == NULL) {
    strlit1301 = alloc_String("w");
    gc_root(strlit1301);
  }
// compilenode returning strlit1301
  gc_frame_newslot(strlit1301);
// compilenode returning *var_io
  params[0] = opresult1300;
  params[1] = strlit1301;
  partcv[0] = 2;
  Object call1302 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1283);
// compilenode returning call1302
  *var_toWrite = call1302;
  if (call1302 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 413
  setline(413);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1304 = gc_frame_new();
// Begin line 412
  setline(412);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1305 = gc_frame_new();
// compilenode returning *var_curImport
  partcv[0] = 0;
  Object call1306 = callmethod(*var_curImport, "data",
    1, partcv, params);
  gc_frame_end(callframe1305);
// compilenode returning call1306
// compilenode returning call1306
  gc_frame_newslot(call1306);
// compilenode returning *var_toWrite
  params[0] = call1306;
  partcv[0] = 1;
  Object call1307 = callmethod(*var_toWrite, "write",
    1, partcv, params);
  gc_frame_end(callframe1304);
// compilenode returning call1307
  gc_frame_end(frame);
  return call1307;
}
Object meth_PackageManager_bundle1202(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 20, (flags>>24)&0xff);
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
  Object *var_toOpen = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "toOpen");
  Object *var_openData = &(stackframe->slots[5]);
  setframeelementname(stackframe, 5, "openData");
  Object *var_toWrite = &(stackframe->slots[6]);
  setframeelementname(stackframe, 6, "toWrite");
  Object *var_file = &(stackframe->slots[7]);
  setframeelementname(stackframe, 7, "file");
// Begin line 397
  setline(397);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 396
  setline(396);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array1203 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array1203
  *var_imported = array1203;
  if (array1203 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 398
  setline(398);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1205 = gc_frame_new();
// Begin line 397
  setline(397);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1208 == NULL) {
    strlit1208 = alloc_String("");
    gc_root(strlit1208);
  }
// compilenode returning strlit1208
  int op_slot_left_1207 = gc_frame_newslot(strlit1208);
// compilenode returning *var_name
  int op_slot_right_1207 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult1210 = callmethod(strlit1208, "++", 1, partcv, params);
// compilenode returning opresult1210
  int op_slot_left_1206 = gc_frame_newslot(opresult1210);
  if (strlit1211 == NULL) {
    strlit1211 = alloc_String("/");
    gc_root(strlit1211);
  }
// compilenode returning strlit1211
  int op_slot_right_1206 = gc_frame_newslot(strlit1211);
  params[0] = strlit1211;
  partcv[0] = 1;
  Object opresult1213 = callmethod(opresult1210, "++", 1, partcv, params);
// compilenode returning opresult1213
  gc_frame_newslot(opresult1213);
  params[0] = opresult1213;
  partcv[0] = 1;
  Object call1214 = callmethodflags(self, "createDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1205);
// compilenode returning call1214
  *var_newDir = call1214;
  if (call1214 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 398
  setline(398);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1215 = gc_frame_new();
// compilenode returning *var_toBundle
  gc_frame_newslot(*var_toBundle);
  if (strlit1216 == NULL) {
    strlit1216 = alloc_String("r");
    gc_root(strlit1216);
  }
// compilenode returning strlit1216
  gc_frame_newslot(strlit1216);
// compilenode returning *var_io
  params[0] = *var_toBundle;
  params[1] = strlit1216;
  partcv[0] = 2;
  Object call1217 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1215);
// compilenode returning call1217
  *var_toOpen = call1217;
  if (call1217 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 400
  setline(400);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 399
  setline(399);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1218 = gc_frame_new();
// compilenode returning *var_toOpen
  partcv[0] = 0;
  Object call1219 = callmethod(*var_toOpen, "read",
    1, partcv, params);
  gc_frame_end(callframe1218);
// compilenode returning call1219
// compilenode returning call1219
  *var_openData = call1219;
  if (call1219 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 400
  setline(400);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1224 == NULL) {
    strlit1224 = alloc_String("");
    gc_root(strlit1224);
  }
// compilenode returning strlit1224
  int op_slot_left_1223 = gc_frame_newslot(strlit1224);
// compilenode returning *var_name
  int op_slot_right_1223 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult1226 = callmethod(strlit1224, "++", 1, partcv, params);
// compilenode returning opresult1226
  int op_slot_left_1222 = gc_frame_newslot(opresult1226);
  if (strlit1227 == NULL) {
    strlit1227 = alloc_String("/");
    gc_root(strlit1227);
  }
// compilenode returning strlit1227
  int op_slot_right_1222 = gc_frame_newslot(strlit1227);
  params[0] = strlit1227;
  partcv[0] = 1;
  Object opresult1229 = callmethod(opresult1226, "++", 1, partcv, params);
// compilenode returning opresult1229
  int op_slot_left_1221 = gc_frame_newslot(opresult1229);
// compilenode returning *var_toBundle
  int op_slot_right_1221 = gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object opresult1231 = callmethod(opresult1229, "++", 1, partcv, params);
// compilenode returning opresult1231
  int op_slot_left_1220 = gc_frame_newslot(opresult1231);
  if (strlit1232 == NULL) {
    strlit1232 = alloc_String(" is what I'm printing");
    gc_root(strlit1232);
  }
// compilenode returning strlit1232
  int op_slot_right_1220 = gc_frame_newslot(strlit1232);
  params[0] = strlit1232;
  partcv[0] = 1;
  Object opresult1234 = callmethod(opresult1231, "++", 1, partcv, params);
// compilenode returning opresult1234
  params[0] = opresult1234;
  Object call1235 = gracelib_print(NULL, 1,  params);
// compilenode returning call1235
// Begin line 402
  setline(402);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1236 = gc_frame_new();
// Begin line 401
  setline(401);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1241 == NULL) {
    strlit1241 = alloc_String("");
    gc_root(strlit1241);
  }
// compilenode returning strlit1241
  int op_slot_left_1240 = gc_frame_newslot(strlit1241);
// compilenode returning *var_name
  int op_slot_right_1240 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult1243 = callmethod(strlit1241, "++", 1, partcv, params);
// compilenode returning opresult1243
  int op_slot_left_1239 = gc_frame_newslot(opresult1243);
  if (strlit1244 == NULL) {
    strlit1244 = alloc_String("/");
    gc_root(strlit1244);
  }
// compilenode returning strlit1244
  int op_slot_right_1239 = gc_frame_newslot(strlit1244);
  params[0] = strlit1244;
  partcv[0] = 1;
  Object opresult1246 = callmethod(opresult1243, "++", 1, partcv, params);
// compilenode returning opresult1246
  int op_slot_left_1238 = gc_frame_newslot(opresult1246);
// compilenode returning *var_toBundle
  int op_slot_right_1238 = gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object opresult1248 = callmethod(opresult1246, "++", 1, partcv, params);
// compilenode returning opresult1248
  int op_slot_left_1237 = gc_frame_newslot(opresult1248);
  if (strlit1249 == NULL) {
    strlit1249 = alloc_String("");
    gc_root(strlit1249);
  }
// compilenode returning strlit1249
  int op_slot_right_1237 = gc_frame_newslot(strlit1249);
  params[0] = strlit1249;
  partcv[0] = 1;
  Object opresult1251 = callmethod(opresult1248, "++", 1, partcv, params);
// compilenode returning opresult1251
  gc_frame_newslot(opresult1251);
  if (strlit1252 == NULL) {
    strlit1252 = alloc_String("w");
    gc_root(strlit1252);
  }
// compilenode returning strlit1252
  gc_frame_newslot(strlit1252);
// compilenode returning *var_io
  params[0] = opresult1251;
  params[1] = strlit1252;
  partcv[0] = 2;
  Object call1253 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1236);
// compilenode returning call1253
  *var_toWrite = call1253;
  if (call1253 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 402
  setline(402);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1254 = gc_frame_new();
// compilenode returning *var_openData
  gc_frame_newslot(*var_openData);
// compilenode returning *var_toWrite
  params[0] = *var_openData;
  partcv[0] = 1;
  Object call1255 = callmethod(*var_toWrite, "write",
    1, partcv, params);
  gc_frame_end(callframe1254);
// compilenode returning call1255
// Begin line 403
  setline(403);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object inheritingObject1256 = inheritingObject;
  if (isTailObject) {
    isTailObject = 0;
    inheritingObject = NULL;
  }
  Object obj1256 = alloc_userobj2(4,4, objclass1256);
  gc_frame_newslot(obj1256);
// OBJECT OUTER DEC outer
  adddatum2(obj1256, self, 0);
  addmethodreal(obj1256,"outer", &reader_PackageManager_outer_1257);
  Object oldself1256 = self;
  struct StackFrameObject *oldstackframe1256 = stackframe;
  stackframe = alloc_StackFrame(1, oldstackframe1256);
  gc_frame_newslot((Object)stackframe);
  self = obj1256;
  Object *oldselfslot1256 = selfslot;
  selfslot = &stackframe->slots[0];
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  Object thisouter1256 = (*(struct UserObject *)self).data[0], lowerouter1256 = thisouter1256;
  if (inheritingObject1256) {
    struct UserObject *inho1256 = (struct UserObject *)inheritingObject1256;
    while (inho1256->super != GraceDefaultObject) inho1256 = (struct UserObject *)inho1256->super;
    inho1256->super = obj1256;
    self = inheritingObject1256;
    *selfslot = self;
    lowerouter1256 = (*(struct UserObject *)self).data[0];
    (*(struct UserObject *)self).data[0] = thisouter1256;
  }
if (objclass1256 == NULL) {
  Method *reader1258 = addmethodrealflags(obj1256, "address",&reader_PackageManager_address_1258, 0);
  Method *writer1258 = addmethodrealflags(obj1256, "address:=",&writer_PackageManager_address_1258, 0);
  reader1258->definitionModule = modulename;
  writer1258->definitionModule = modulename;
  reader1258->definitionLine = 405;
  writer1258->definitionLine = 405;
}
obj1256->flags |= OFLAG_MUTABLE;
adddatum2(obj1256, alloc_Undefined(), 1);
if (objclass1256 == NULL) {
  Method *reader1259 = addmethodrealflags(obj1256, "data",&reader_PackageManager_data_1259, 0);
  Method *writer1259 = addmethodrealflags(obj1256, "data:=",&writer_PackageManager_data_1259, 0);
  reader1259->definitionModule = modulename;
  writer1259->definitionModule = modulename;
  reader1259->definitionLine = 406;
  writer1259->definitionLine = 406;
}
obj1256->flags |= OFLAG_MUTABLE;
adddatum2(obj1256, alloc_Undefined(), 2);
  sourceObject = obj1256;
  adddatum2(obj1256, undefined, 1);
  sourceObject = obj1256;
  adddatum2(obj1256, undefined, 2);
objclass1256 = obj1256->class;
  objclass1256->definitionModule = modulename;
  objclass1256->definitionLine = 403;
  (*(struct UserObject *)self).data[0] = lowerouter1256;
  self = oldself1256;
  selfslot = oldselfslot1256;
  stackframe = oldstackframe1256;
// compilenode returning obj1256
  *var_file = obj1256;
  if (obj1256 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 408
  setline(408);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1260 = gc_frame_new();
// Begin line 407
  setline(407);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toBundle
  gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object call1261 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1260);
// compilenode returning call1261
// Begin line 408
  setline(408);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1263 == NULL) {
    strlit1263 = alloc_String("Imported size = ");
    gc_root(strlit1263);
  }
// compilenode returning strlit1263
  int op_slot_left_1262 = gc_frame_newslot(strlit1263);
  int callframe1264 = gc_frame_new();
// compilenode returning *var_imported
  partcv[0] = 0;
  Object call1265 = callmethod(*var_imported, "size",
    1, partcv, params);
  gc_frame_end(callframe1264);
// compilenode returning call1265
// compilenode returning call1265
  int op_slot_right_1262 = gc_frame_newslot(call1265);
  params[0] = call1265;
  partcv[0] = 1;
  Object opresult1267 = callmethod(strlit1263, "++", 1, partcv, params);
// compilenode returning opresult1267
  params[0] = opresult1267;
  Object call1268 = gracelib_print(NULL, 1,  params);
// compilenode returning call1268
// Begin line 414
  setline(414);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1269 = gc_frame_new();
// Begin line 409
  setline(409);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1270 = alloc_Block(NULL, NULL, "PackageManager", 409);
  gc_frame_newslot(block1270);
  block_savedest(block1270);
  Object closure1271 = createclosure(2, "_apply");
setclosureframe(closure1271, stackframe);
  addtoclosure(closure1271, var_imported);
  addtoclosure(closure1271, selfslot);
  struct UserObject *uo1271 = (struct UserObject*)block1270;
  uo1271->data[0] = (Object)closure1271;
  Method *meth_meth_PackageManager__apply1271 = addmethod2pos(block1270, "_apply", &meth_PackageManager__apply1271, 0);
int argcv_meth_PackageManager__apply1271[] = {0};
meth_meth_PackageManager__apply1271->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1271);
  meth_meth_PackageManager__apply1271->definitionModule = modulename;
  meth_meth_PackageManager__apply1271->definitionLine = 408;
// compilenode returning block1270
  gc_frame_newslot(block1270);
// Begin line 414
  setline(414);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1279 = alloc_Block(NULL, NULL, "PackageManager", 414);
  gc_frame_newslot(block1279);
  block_savedest(block1279);
  Object closure1280 = createclosure(5, "_apply");
setclosureframe(closure1280, stackframe);
  addtoclosure(closure1280, var_imported);
  addtoclosure(closure1280, var_name);
  addtoclosure(closure1280, var_io);
  addtoclosure(closure1280, var_toWrite);
  addtoclosure(closure1280, selfslot);
  struct UserObject *uo1280 = (struct UserObject*)block1279;
  uo1280->data[0] = (Object)closure1280;
  Method *meth_meth_PackageManager__apply1280 = addmethod2pos(block1279, "_apply", &meth_PackageManager__apply1280, 0);
int argcv_meth_PackageManager__apply1280[] = {0};
meth_meth_PackageManager__apply1280->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1280);
  meth_meth_PackageManager__apply1280->definitionModule = modulename;
  meth_meth_PackageManager__apply1280->definitionLine = 409;
// compilenode returning block1279
  gc_frame_newslot(block1279);
  params[0] = block1270;
  params[1] = block1279;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1309 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1269);
// compilenode returning call1309
  gc_frame_end(frame);
  return call1309;
}
Object meth_PackageManager_bundle1202_object(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 20, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(8, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "bundle()object");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = args[0 + argcv[0]];
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
  Object *var_toOpen = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "toOpen");
  Object *var_openData = &(stackframe->slots[5]);
  setframeelementname(stackframe, 5, "openData");
  Object *var_toWrite = &(stackframe->slots[6]);
  setframeelementname(stackframe, 6, "toWrite");
  Object *var_file = &(stackframe->slots[7]);
  setframeelementname(stackframe, 7, "file");
// Begin line 397
  setline(397);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 396
  setline(396);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array1203 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array1203
  *var_imported = array1203;
  if (array1203 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 398
  setline(398);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1205 = gc_frame_new();
// Begin line 397
  setline(397);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1208 == NULL) {
    strlit1208 = alloc_String("");
    gc_root(strlit1208);
  }
// compilenode returning strlit1208
  int op_slot_left_1207 = gc_frame_newslot(strlit1208);
// compilenode returning *var_name
  int op_slot_right_1207 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult1210 = callmethod(strlit1208, "++", 1, partcv, params);
// compilenode returning opresult1210
  int op_slot_left_1206 = gc_frame_newslot(opresult1210);
  if (strlit1211 == NULL) {
    strlit1211 = alloc_String("/");
    gc_root(strlit1211);
  }
// compilenode returning strlit1211
  int op_slot_right_1206 = gc_frame_newslot(strlit1211);
  params[0] = strlit1211;
  partcv[0] = 1;
  Object opresult1213 = callmethod(opresult1210, "++", 1, partcv, params);
// compilenode returning opresult1213
  gc_frame_newslot(opresult1213);
  params[0] = opresult1213;
  partcv[0] = 1;
  Object call1214 = callmethodflags(self, "createDirectory", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1205);
// compilenode returning call1214
  *var_newDir = call1214;
  if (call1214 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 398
  setline(398);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1215 = gc_frame_new();
// compilenode returning *var_toBundle
  gc_frame_newslot(*var_toBundle);
  if (strlit1216 == NULL) {
    strlit1216 = alloc_String("r");
    gc_root(strlit1216);
  }
// compilenode returning strlit1216
  gc_frame_newslot(strlit1216);
// compilenode returning *var_io
  params[0] = *var_toBundle;
  params[1] = strlit1216;
  partcv[0] = 2;
  Object call1217 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1215);
// compilenode returning call1217
  *var_toOpen = call1217;
  if (call1217 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 400
  setline(400);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 399
  setline(399);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1218 = gc_frame_new();
// compilenode returning *var_toOpen
  partcv[0] = 0;
  Object call1219 = callmethod(*var_toOpen, "read",
    1, partcv, params);
  gc_frame_end(callframe1218);
// compilenode returning call1219
// compilenode returning call1219
  *var_openData = call1219;
  if (call1219 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 400
  setline(400);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1224 == NULL) {
    strlit1224 = alloc_String("");
    gc_root(strlit1224);
  }
// compilenode returning strlit1224
  int op_slot_left_1223 = gc_frame_newslot(strlit1224);
// compilenode returning *var_name
  int op_slot_right_1223 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult1226 = callmethod(strlit1224, "++", 1, partcv, params);
// compilenode returning opresult1226
  int op_slot_left_1222 = gc_frame_newslot(opresult1226);
  if (strlit1227 == NULL) {
    strlit1227 = alloc_String("/");
    gc_root(strlit1227);
  }
// compilenode returning strlit1227
  int op_slot_right_1222 = gc_frame_newslot(strlit1227);
  params[0] = strlit1227;
  partcv[0] = 1;
  Object opresult1229 = callmethod(opresult1226, "++", 1, partcv, params);
// compilenode returning opresult1229
  int op_slot_left_1221 = gc_frame_newslot(opresult1229);
// compilenode returning *var_toBundle
  int op_slot_right_1221 = gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object opresult1231 = callmethod(opresult1229, "++", 1, partcv, params);
// compilenode returning opresult1231
  int op_slot_left_1220 = gc_frame_newslot(opresult1231);
  if (strlit1232 == NULL) {
    strlit1232 = alloc_String(" is what I'm printing");
    gc_root(strlit1232);
  }
// compilenode returning strlit1232
  int op_slot_right_1220 = gc_frame_newslot(strlit1232);
  params[0] = strlit1232;
  partcv[0] = 1;
  Object opresult1234 = callmethod(opresult1231, "++", 1, partcv, params);
// compilenode returning opresult1234
  params[0] = opresult1234;
  Object call1235 = gracelib_print(NULL, 1,  params);
// compilenode returning call1235
// Begin line 402
  setline(402);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1236 = gc_frame_new();
// Begin line 401
  setline(401);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1241 == NULL) {
    strlit1241 = alloc_String("");
    gc_root(strlit1241);
  }
// compilenode returning strlit1241
  int op_slot_left_1240 = gc_frame_newslot(strlit1241);
// compilenode returning *var_name
  int op_slot_right_1240 = gc_frame_newslot(*var_name);
  params[0] = *var_name;
  partcv[0] = 1;
  Object opresult1243 = callmethod(strlit1241, "++", 1, partcv, params);
// compilenode returning opresult1243
  int op_slot_left_1239 = gc_frame_newslot(opresult1243);
  if (strlit1244 == NULL) {
    strlit1244 = alloc_String("/");
    gc_root(strlit1244);
  }
// compilenode returning strlit1244
  int op_slot_right_1239 = gc_frame_newslot(strlit1244);
  params[0] = strlit1244;
  partcv[0] = 1;
  Object opresult1246 = callmethod(opresult1243, "++", 1, partcv, params);
// compilenode returning opresult1246
  int op_slot_left_1238 = gc_frame_newslot(opresult1246);
// compilenode returning *var_toBundle
  int op_slot_right_1238 = gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object opresult1248 = callmethod(opresult1246, "++", 1, partcv, params);
// compilenode returning opresult1248
  int op_slot_left_1237 = gc_frame_newslot(opresult1248);
  if (strlit1249 == NULL) {
    strlit1249 = alloc_String("");
    gc_root(strlit1249);
  }
// compilenode returning strlit1249
  int op_slot_right_1237 = gc_frame_newslot(strlit1249);
  params[0] = strlit1249;
  partcv[0] = 1;
  Object opresult1251 = callmethod(opresult1248, "++", 1, partcv, params);
// compilenode returning opresult1251
  gc_frame_newslot(opresult1251);
  if (strlit1252 == NULL) {
    strlit1252 = alloc_String("w");
    gc_root(strlit1252);
  }
// compilenode returning strlit1252
  gc_frame_newslot(strlit1252);
// compilenode returning *var_io
  params[0] = opresult1251;
  params[1] = strlit1252;
  partcv[0] = 2;
  Object call1253 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1236);
// compilenode returning call1253
  *var_toWrite = call1253;
  if (call1253 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 402
  setline(402);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1254 = gc_frame_new();
// compilenode returning *var_openData
  gc_frame_newslot(*var_openData);
// compilenode returning *var_toWrite
  params[0] = *var_openData;
  partcv[0] = 1;
  Object call1255 = callmethod(*var_toWrite, "write",
    1, partcv, params);
  gc_frame_end(callframe1254);
// compilenode returning call1255
// Begin line 403
  setline(403);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object inheritingObject1256 = inheritingObject;
  if (isTailObject) {
    isTailObject = 0;
    inheritingObject = NULL;
  }
  Object obj1256 = alloc_userobj2(4,4, objclass1256);
  gc_frame_newslot(obj1256);
// OBJECT OUTER DEC outer
  adddatum2(obj1256, self, 0);
  addmethodreal(obj1256,"outer", &reader_PackageManager_outer_1257);
  Object oldself1256 = self;
  struct StackFrameObject *oldstackframe1256 = stackframe;
  stackframe = alloc_StackFrame(1, oldstackframe1256);
  gc_frame_newslot((Object)stackframe);
  self = obj1256;
  Object *oldselfslot1256 = selfslot;
  selfslot = &stackframe->slots[0];
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  Object thisouter1256 = (*(struct UserObject *)self).data[0], lowerouter1256 = thisouter1256;
  if (inheritingObject1256) {
    struct UserObject *inho1256 = (struct UserObject *)inheritingObject1256;
    while (inho1256->super != GraceDefaultObject) inho1256 = (struct UserObject *)inho1256->super;
    inho1256->super = obj1256;
    self = inheritingObject1256;
    *selfslot = self;
    lowerouter1256 = (*(struct UserObject *)self).data[0];
    (*(struct UserObject *)self).data[0] = thisouter1256;
  }
if (objclass1256 == NULL) {
  Method *reader1258 = addmethodrealflags(obj1256, "address",&reader_PackageManager_address_1258, 0);
  Method *writer1258 = addmethodrealflags(obj1256, "address:=",&writer_PackageManager_address_1258, 0);
  reader1258->definitionModule = modulename;
  writer1258->definitionModule = modulename;
  reader1258->definitionLine = 405;
  writer1258->definitionLine = 405;
}
obj1256->flags |= OFLAG_MUTABLE;
adddatum2(obj1256, alloc_Undefined(), 1);
if (objclass1256 == NULL) {
  Method *reader1259 = addmethodrealflags(obj1256, "data",&reader_PackageManager_data_1259, 0);
  Method *writer1259 = addmethodrealflags(obj1256, "data:=",&writer_PackageManager_data_1259, 0);
  reader1259->definitionModule = modulename;
  writer1259->definitionModule = modulename;
  reader1259->definitionLine = 406;
  writer1259->definitionLine = 406;
}
obj1256->flags |= OFLAG_MUTABLE;
adddatum2(obj1256, alloc_Undefined(), 2);
  sourceObject = obj1256;
  adddatum2(obj1256, undefined, 1);
  sourceObject = obj1256;
  adddatum2(obj1256, undefined, 2);
objclass1256 = obj1256->class;
  objclass1256->definitionModule = modulename;
  objclass1256->definitionLine = 403;
  (*(struct UserObject *)self).data[0] = lowerouter1256;
  self = oldself1256;
  selfslot = oldselfslot1256;
  stackframe = oldstackframe1256;
// compilenode returning obj1256
  *var_file = obj1256;
  if (obj1256 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 408
  setline(408);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1260 = gc_frame_new();
// Begin line 407
  setline(407);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toBundle
  gc_frame_newslot(*var_toBundle);
  params[0] = *var_toBundle;
  partcv[0] = 1;
  Object call1261 = callmethodflags(self, "fetchImports", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1260);
// compilenode returning call1261
// Begin line 408
  setline(408);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1263 == NULL) {
    strlit1263 = alloc_String("Imported size = ");
    gc_root(strlit1263);
  }
// compilenode returning strlit1263
  int op_slot_left_1262 = gc_frame_newslot(strlit1263);
  int callframe1264 = gc_frame_new();
// compilenode returning *var_imported
  partcv[0] = 0;
  Object call1265 = callmethod(*var_imported, "size",
    1, partcv, params);
  gc_frame_end(callframe1264);
// compilenode returning call1265
// compilenode returning call1265
  int op_slot_right_1262 = gc_frame_newslot(call1265);
  params[0] = call1265;
  partcv[0] = 1;
  Object opresult1267 = callmethod(strlit1263, "++", 1, partcv, params);
// compilenode returning opresult1267
  params[0] = opresult1267;
  Object call1268 = gracelib_print(NULL, 1,  params);
// compilenode returning call1268
// Begin line 414
  setline(414);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1269 = gc_frame_new();
// Begin line 409
  setline(409);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1270 = alloc_Block(NULL, NULL, "PackageManager", 409);
  gc_frame_newslot(block1270);
  block_savedest(block1270);
  Object closure1271 = createclosure(2, "_apply");
setclosureframe(closure1271, stackframe);
  addtoclosure(closure1271, var_imported);
  addtoclosure(closure1271, selfslot);
  struct UserObject *uo1271 = (struct UserObject*)block1270;
  uo1271->data[0] = (Object)closure1271;
  Method *meth_meth_PackageManager__apply1271 = addmethod2pos(block1270, "_apply", &meth_PackageManager__apply1271, 0);
int argcv_meth_PackageManager__apply1271[] = {0};
meth_meth_PackageManager__apply1271->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1271);
  meth_meth_PackageManager__apply1271->definitionModule = modulename;
  meth_meth_PackageManager__apply1271->definitionLine = 408;
// compilenode returning block1270
  gc_frame_newslot(block1270);
// Begin line 414
  setline(414);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1279 = alloc_Block(NULL, NULL, "PackageManager", 414);
  gc_frame_newslot(block1279);
  block_savedest(block1279);
  Object closure1280 = createclosure(5, "_apply");
setclosureframe(closure1280, stackframe);
  addtoclosure(closure1280, var_imported);
  addtoclosure(closure1280, var_name);
  addtoclosure(closure1280, var_io);
  addtoclosure(closure1280, var_toWrite);
  addtoclosure(closure1280, selfslot);
  struct UserObject *uo1280 = (struct UserObject*)block1279;
  uo1280->data[0] = (Object)closure1280;
  Method *meth_meth_PackageManager__apply1280 = addmethod2pos(block1279, "_apply", &meth_PackageManager__apply1280, 0);
int argcv_meth_PackageManager__apply1280[] = {0};
meth_meth_PackageManager__apply1280->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1280);
  meth_meth_PackageManager__apply1280->definitionModule = modulename;
  meth_meth_PackageManager__apply1280->definitionLine = 409;
// compilenode returning block1279
  gc_frame_newslot(block1279);
  params[0] = block1270;
  params[1] = block1279;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1309 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1269);
// compilenode returning call1309
  gc_frame_end(frame);
  return call1309;
}
Object meth_PackageManager__apply1380(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 429
  setline(429);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1381 = gc_frame_new();
// compilenode returning *var_d
  gc_frame_newslot(*var_d);
// compilenode returning *var_out
  params[0] = *var_d;
  partcv[0] = 1;
  Object call1382 = callmethod(*var_out, "write",
    1, partcv, params);
  gc_frame_end(callframe1381);
// compilenode returning call1382
  gc_frame_end(frame);
  return call1382;
}
Object meth_PackageManager__apply1316(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 433
  setline(433);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 419
  setline(419);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1319 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1320 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1319);
// compilenode returning call1320
// compilenode returning call1320
  int op_slot_left_1318 = gc_frame_newslot(call1320);
  Object num1321 = alloc_Float64(4);
// compilenode returning num1321
  int op_slot_right_1318 = gc_frame_newslot(num1321);
  params[0] = num1321;
  partcv[0] = 1;
  Object opresult1323 = callmethod(call1320, ">", 1, partcv, params);
// compilenode returning opresult1323
  Object if1317 = done;
  if (istrue(opresult1323)) {
// Begin line 432
  setline(432);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 420
  setline(420);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1326 = gc_frame_new();
  int callframe1328 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1329 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1328);
// compilenode returning call1329
// compilenode returning call1329
  int op_slot_left_1327 = gc_frame_newslot(call1329);
  Object num1330 = alloc_Float64(5);
// compilenode returning num1330
  int op_slot_right_1327 = gc_frame_newslot(num1330);
  params[0] = num1330;
  partcv[0] = 1;
  Object diff1332 = callmethod(call1329, "-", 1, partcv, params);
// compilenode returning diff1332
  gc_frame_newslot(diff1332);
  int callframe1333 = gc_frame_new();
// compilenode returning *var_file
  partcv[0] = 0;
  Object call1334 = callmethod(*var_file, "size",
    1, partcv, params);
  gc_frame_end(callframe1333);
// compilenode returning call1334
// compilenode returning call1334
  gc_frame_newslot(call1334);
// compilenode returning *var_file
  params[0] = diff1332;
  params[1] = call1334;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1335 = callmethod(*var_file, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1326);
// compilenode returning call1335
  int op_slot_left_1325 = gc_frame_newslot(call1335);
  if (strlit1336 == NULL) {
    strlit1336 = alloc_String(".grace");
    gc_root(strlit1336);
  }
// compilenode returning strlit1336
  int op_slot_right_1325 = gc_frame_newslot(strlit1336);
  params[0] = strlit1336;
  partcv[0] = 1;
  Object opresult1338 = callmethod(call1335, "==", 1, partcv, params);
// compilenode returning opresult1338
  Object if1324 = done;
  if (istrue(opresult1338)) {
// Begin line 421
  setline(421);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1339 = gc_frame_new();
// compilenode returning *var_address
  int op_slot_left_1341 = gc_frame_newslot(*var_address);
  if (strlit1342 == NULL) {
    strlit1342 = alloc_String("/");
    gc_root(strlit1342);
  }
// compilenode returning strlit1342
  int op_slot_right_1341 = gc_frame_newslot(strlit1342);
  params[0] = strlit1342;
  partcv[0] = 1;
  Object opresult1344 = callmethod(*var_address, "++", 1, partcv, params);
// compilenode returning opresult1344
  int op_slot_left_1340 = gc_frame_newslot(opresult1344);
// compilenode returning *var_file
  int op_slot_right_1340 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1346 = callmethod(opresult1344, "++", 1, partcv, params);
// compilenode returning opresult1346
  gc_frame_newslot(opresult1346);
  if (strlit1347 == NULL) {
    strlit1347 = alloc_String("r");
    gc_root(strlit1347);
  }
// compilenode returning strlit1347
  gc_frame_newslot(strlit1347);
// compilenode returning *var_io
  params[0] = opresult1346;
  params[1] = strlit1347;
  partcv[0] = 2;
  Object call1348 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1339);
// compilenode returning call1348
  *var_open = call1348;
  if (call1348 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 422
  setline(422);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1349 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call1350 = callmethod(*var_open, "read",
    1, partcv, params);
  gc_frame_end(callframe1349);
// compilenode returning call1350
// compilenode returning call1350
  *var_readFile = call1350;
  if (call1350 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 423
  setline(423);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1351 = gc_frame_new();
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
  Object call1352 = callmethodflags(self, "parseAndPrefix", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1351);
// compilenode returning call1352
  *var_toWrite = call1352;
  if (call1352 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 424
  setline(424);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1353 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call1354 = callmethod(*var_open, "close",
    1, partcv, params);
  gc_frame_end(callframe1353);
// compilenode returning call1354
// compilenode returning call1354
// Begin line 427
  setline(427);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1355 = gc_frame_new();
// Begin line 426
  setline(426);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1359 == NULL) {
    strlit1359 = alloc_String("rm ");
    gc_root(strlit1359);
  }
// compilenode returning strlit1359
  int op_slot_left_1358 = gc_frame_newslot(strlit1359);
// compilenode returning *var_address
  int op_slot_right_1358 = gc_frame_newslot(*var_address);
  params[0] = *var_address;
  partcv[0] = 1;
  Object opresult1361 = callmethod(strlit1359, "++", 1, partcv, params);
// compilenode returning opresult1361
  int op_slot_left_1357 = gc_frame_newslot(opresult1361);
  if (strlit1362 == NULL) {
    strlit1362 = alloc_String("/");
    gc_root(strlit1362);
  }
// compilenode returning strlit1362
  int op_slot_right_1357 = gc_frame_newslot(strlit1362);
  params[0] = strlit1362;
  partcv[0] = 1;
  Object opresult1364 = callmethod(opresult1361, "++", 1, partcv, params);
// compilenode returning opresult1364
  int op_slot_left_1356 = gc_frame_newslot(opresult1364);
// compilenode returning *var_file
  int op_slot_right_1356 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1366 = callmethod(opresult1364, "++", 1, partcv, params);
// compilenode returning opresult1366
  gc_frame_newslot(opresult1366);
// compilenode returning *var_io
  params[0] = opresult1366;
  partcv[0] = 1;
  Object call1367 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe1355);
// compilenode returning call1367
// Begin line 427
  setline(427);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1368 = gc_frame_new();
// compilenode returning *var_address
  int op_slot_left_1370 = gc_frame_newslot(*var_address);
  if (strlit1371 == NULL) {
    strlit1371 = alloc_String("/");
    gc_root(strlit1371);
  }
// compilenode returning strlit1371
  int op_slot_right_1370 = gc_frame_newslot(strlit1371);
  params[0] = strlit1371;
  partcv[0] = 1;
  Object opresult1373 = callmethod(*var_address, "++", 1, partcv, params);
// compilenode returning opresult1373
  int op_slot_left_1369 = gc_frame_newslot(opresult1373);
// compilenode returning *var_file
  int op_slot_right_1369 = gc_frame_newslot(*var_file);
  params[0] = *var_file;
  partcv[0] = 1;
  Object opresult1375 = callmethod(opresult1373, "++", 1, partcv, params);
// compilenode returning opresult1375
  gc_frame_newslot(opresult1375);
  if (strlit1376 == NULL) {
    strlit1376 = alloc_String("w");
    gc_root(strlit1376);
  }
// compilenode returning strlit1376
  gc_frame_newslot(strlit1376);
// compilenode returning *var_io
  params[0] = opresult1375;
  params[1] = strlit1376;
  partcv[0] = 2;
  Object call1377 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1368);
// compilenode returning call1377
  *var_out = call1377;
  if (call1377 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 431
  setline(431);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1378 = gc_frame_new();
// Begin line 428
  setline(428);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_toWrite
  gc_frame_newslot(*var_toWrite);
// Begin line 431
  setline(431);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1379 = alloc_Block(NULL, NULL, "PackageManager", 431);
  gc_frame_newslot(block1379);
  block_savedest(block1379);
  Object closure1380 = createclosure(2, "_apply");
setclosureframe(closure1380, stackframe);
  addtoclosure(closure1380, var_out);
  addtoclosure(closure1380, selfslot);
  struct UserObject *uo1380 = (struct UserObject*)block1379;
  uo1380->data[0] = (Object)closure1380;
  Method *meth_meth_PackageManager__apply1380 = addmethod2pos(block1379, "_apply", &meth_PackageManager__apply1380, 0);
int argcv_meth_PackageManager__apply1380[] = {1};
meth_meth_PackageManager__apply1380->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1380);
  meth_meth_PackageManager__apply1380->definitionModule = modulename;
  meth_meth_PackageManager__apply1380->definitionLine = 424;
// compilenode returning block1379
  gc_frame_newslot(block1379);
  params[0] = *var_toWrite;
  params[1] = block1379;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1384 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1378);
// compilenode returning call1384
    gc_frame_newslot(call1384);
    if1324 = call1384;
  } else {
  }
// compilenode returning if1324
    gc_frame_newslot(if1324);
    if1317 = if1324;
  } else {
  }
// compilenode returning if1317
  gc_frame_end(frame);
  return if1317;
}
Object meth_PackageManager_setImportDest1311(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 21, (flags>>24)&0xff);
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
// Begin line 417
  setline(417);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1312 = gc_frame_new();
// compilenode returning *var_address
  gc_frame_newslot(*var_address);
// compilenode returning *var_io
  params[0] = *var_address;
  partcv[0] = 1;
  Object call1313 = callmethod(*var_io, "getdir",
    1, partcv, params);
  gc_frame_end(callframe1312);
// compilenode returning call1313
  *var_folder = call1313;
  if (call1313 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 437
  setline(437);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1314 = gc_frame_new();
// Begin line 418
  setline(418);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_folder
  gc_frame_newslot(*var_folder);
// Begin line 437
  setline(437);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1315 = alloc_Block(NULL, NULL, "PackageManager", 437);
  gc_frame_newslot(block1315);
  block_savedest(block1315);
  Object closure1316 = createclosure(4, "_apply");
setclosureframe(closure1316, stackframe);
  addtoclosure(closure1316, var_address);
  addtoclosure(closure1316, var_io);
  addtoclosure(closure1316, var_prefix);
  addtoclosure(closure1316, selfslot);
  struct UserObject *uo1316 = (struct UserObject*)block1315;
  uo1316->data[0] = (Object)closure1316;
  Method *meth_meth_PackageManager__apply1316 = addmethod2pos(block1315, "_apply", &meth_PackageManager__apply1316, 0);
int argcv_meth_PackageManager__apply1316[] = {1};
meth_meth_PackageManager__apply1316->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1316);
  meth_meth_PackageManager__apply1316->definitionModule = modulename;
  meth_meth_PackageManager__apply1316->definitionLine = 412;
// compilenode returning block1315
  gc_frame_newslot(block1315);
  params[0] = *var_folder;
  params[1] = block1315;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1386 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1314);
// compilenode returning call1386
  gc_frame_end(frame);
  return call1386;
}
Object meth_PackageManager__apply1401(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1402 = gc_frame_newslot(*var_curPos);
  int callframe1403 = gc_frame_new();
// compilenode returning *var_readFile
  partcv[0] = 0;
  Object call1404 = callmethod(*var_readFile, "size",
    1, partcv, params);
  gc_frame_end(callframe1403);
// compilenode returning call1404
// compilenode returning call1404
  int op_slot_right_1402 = gc_frame_newslot(call1404);
  params[0] = call1404;
  partcv[0] = 1;
  Object opresult1406 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1406
  gc_frame_end(frame);
  return opresult1406;
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
  int op_slot_left_1415 = gc_frame_newslot(*var_curPos);
  int callframe1416 = gc_frame_new();
// compilenode returning *var_readFile
  partcv[0] = 0;
  Object call1417 = callmethod(*var_readFile, "size",
    1, partcv, params);
  gc_frame_end(callframe1416);
// compilenode returning call1417
// compilenode returning call1417
  int op_slot_right_1415 = gc_frame_newslot(call1417);
  params[0] = call1417;
  partcv[0] = 1;
  Object opresult1419 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult1419
  int op_slot_left_1414 = gc_frame_newslot(opresult1419);
// compilenode returning *var_readFile
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1421 = callmethod(*var_readFile, "[]", 1, partcv, params);
// compilenode returning idxres1421
  int op_slot_left_1420 = gc_frame_newslot(idxres1421);
  if (strlit1422 == NULL) {
    strlit1422 = alloc_String("""\x0a""");
    gc_root(strlit1422);
  }
// compilenode returning strlit1422
  int op_slot_right_1420 = gc_frame_newslot(strlit1422);
  params[0] = strlit1422;
  partcv[0] = 1;
  Object opresult1424 = callmethod(idxres1421, "!=", 1, partcv, params);
// compilenode returning opresult1424
  int op_slot_right_1414 = gc_frame_newslot(opresult1424);
  params[0] = opresult1424;
  partcv[0] = 1;
  Object opresult1426 = callmethod(opresult1419, "&&", 1, partcv, params);
// compilenode returning opresult1426
  gc_frame_end(frame);
  return opresult1426;
}
Object meth_PackageManager__apply1429(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 448
  setline(448);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1430 = gc_frame_newslot(*var_curPos);
  Object num1431 = alloc_Float64(1);
// compilenode returning num1431
  int op_slot_right_1430 = gc_frame_newslot(num1431);
  params[0] = num1431;
  partcv[0] = 1;
  Object sum1433 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1433
  *var_curPos = sum1433;
  if (sum1433 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1454(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 454
  setline(454);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  int op_slot_left_1455 = gc_frame_newslot(*var_line);
// compilenode returning *var_readFile
// compilenode returning *var_b
  params[0] = *var_b;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1456 = callmethod(*var_readFile, "[]", 1, partcv, params);
// compilenode returning idxres1456
  int op_slot_right_1455 = gc_frame_newslot(idxres1456);
  params[0] = idxres1456;
  partcv[0] = 1;
  Object opresult1458 = callmethod(*var_line, "++", 1, partcv, params);
// compilenode returning opresult1458
  *var_line = opresult1458;
  if (opresult1458 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1498(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  int op_slot_left_1500 = gc_frame_newslot(*var_pos);
  int callframe1501 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1502 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1501);
// compilenode returning call1502
// compilenode returning call1502
  int op_slot_right_1500 = gc_frame_newslot(call1502);
  params[0] = call1502;
  partcv[0] = 1;
  Object opresult1504 = callmethod(*var_pos, "<", 1, partcv, params);
// compilenode returning opresult1504
  int op_slot_left_1499 = gc_frame_newslot(opresult1504);
  int callframe1505 = gc_frame_new();
// compilenode returning *var_line
// compilenode returning *var_pos
  params[0] = *var_pos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1508 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1508
  int op_slot_left_1507 = gc_frame_newslot(idxres1508);
  if (strlit1509 == NULL) {
    strlit1509 = alloc_String("""\x22""");
    gc_root(strlit1509);
  }
// compilenode returning strlit1509
  int op_slot_right_1507 = gc_frame_newslot(strlit1509);
  params[0] = strlit1509;
  partcv[0] = 1;
  Object opresult1511 = callmethod(idxres1508, "==", 1, partcv, params);
// compilenode returning opresult1511
  int op_slot_left_1506 = gc_frame_newslot(opresult1511);
// compilenode returning *var_line
// compilenode returning *var_pos
  params[0] = *var_pos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres1513 = callmethod(*var_line, "[]", 1, partcv, params);
// compilenode returning idxres1513
  int op_slot_left_1512 = gc_frame_newslot(idxres1513);
  if (strlit1514 == NULL) {
    strlit1514 = alloc_String(" ");
    gc_root(strlit1514);
  }
// compilenode returning strlit1514
  int op_slot_right_1512 = gc_frame_newslot(strlit1514);
  params[0] = strlit1514;
  partcv[0] = 1;
  Object opresult1516 = callmethod(idxres1513, "==", 1, partcv, params);
// compilenode returning opresult1516
  int op_slot_right_1506 = gc_frame_newslot(opresult1516);
  params[0] = opresult1516;
  partcv[0] = 1;
  Object opresult1518 = callmethod(opresult1511, "||", 1, partcv, params);
// compilenode returning opresult1518
  partcv[0] = 0;
  Object call1519 = callmethod(opresult1518, "prefix!",
    1, partcv, params);
  gc_frame_end(callframe1505);
// compilenode returning call1519
  int op_slot_right_1499 = gc_frame_newslot(call1519);
  params[0] = call1519;
  partcv[0] = 1;
  Object opresult1521 = callmethod(opresult1504, "&&", 1, partcv, params);
// compilenode returning opresult1521
  gc_frame_end(frame);
  return opresult1521;
}
Object meth_PackageManager__apply1524(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 466
  setline(466);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 465
  setline(465);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  int op_slot_left_1525 = gc_frame_newslot(*var_pos);
  Object num1526 = alloc_Float64(1);
// compilenode returning num1526
  int op_slot_right_1525 = gc_frame_newslot(num1526);
  params[0] = num1526;
  partcv[0] = 1;
  Object sum1528 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1528
  *var_pos = sum1528;
  if (sum1528 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1560(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 471
  setline(471);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1561 = gc_frame_new();
// compilenode returning *var_d
  gc_frame_newslot(*var_d);
// compilenode returning *var_outFile
  params[0] = *var_d;
  partcv[0] = 1;
  Object call1562 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1561);
// compilenode returning call1562
  gc_frame_end(frame);
  return call1562;
}
Object meth_PackageManager__apply1567(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 476
  setline(476);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1568 = gc_frame_new();
// compilenode returning *var_d
  gc_frame_newslot(*var_d);
// compilenode returning *var_outFile
  params[0] = *var_d;
  partcv[0] = 1;
  Object call1569 = callmethod(*var_outFile, "push",
    1, partcv, params);
  gc_frame_end(callframe1568);
// compilenode returning call1569
  gc_frame_end(frame);
  return call1569;
}
Object meth_PackageManager__apply1409(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(7, getclosureframe(closure));
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
// Begin line 447
  setline(447);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 446
  setline(446);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 450
  setline(450);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1411 = gc_frame_new();
// Begin line 447
  setline(447);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1412 = alloc_Block(NULL, NULL, "PackageManager", 447);
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
  meth_meth_PackageManager__apply1413->definitionLine = 445;
// compilenode returning block1412
  gc_frame_newslot(block1412);
// Begin line 450
  setline(450);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1428 = alloc_Block(NULL, NULL, "PackageManager", 450);
  gc_frame_newslot(block1428);
  block_savedest(block1428);
  Object closure1429 = createclosure(2, "_apply");
setclosureframe(closure1429, stackframe);
  addtoclosure(closure1429, var_curPos);
  addtoclosure(closure1429, selfslot);
  struct UserObject *uo1429 = (struct UserObject*)block1428;
  uo1429->data[0] = (Object)closure1429;
  Method *meth_meth_PackageManager__apply1429 = addmethod2pos(block1428, "_apply", &meth_PackageManager__apply1429, 0);
int argcv_meth_PackageManager__apply1429[] = {0};
meth_meth_PackageManager__apply1429->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1429);
  meth_meth_PackageManager__apply1429->definitionModule = modulename;
  meth_meth_PackageManager__apply1429->definitionLine = 447;
// compilenode returning block1428
  gc_frame_newslot(block1428);
  params[0] = block1412;
  params[1] = block1428;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1436 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1411);
// compilenode returning call1436
// Begin line 451
  setline(451);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1437 = gc_frame_new();
// Begin line 450
  setline(450);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_startPos
  gc_frame_newslot(*var_startPos);
// compilenode returning *var_curPos
  int op_slot_left_1438 = gc_frame_newslot(*var_curPos);
  Object num1439 = alloc_Float64(1);
// compilenode returning num1439
  int op_slot_right_1438 = gc_frame_newslot(num1439);
  params[0] = num1439;
  partcv[0] = 1;
  Object diff1441 = callmethod(*var_curPos, "-", 1, partcv, params);
// compilenode returning diff1441
  gc_frame_newslot(diff1441);
// compilenode returning *var_readFile
  params[0] = *var_startPos;
  params[1] = diff1441;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1442 = callmethod(*var_readFile, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1437);
// compilenode returning call1442
  *var_data = call1442;
  if (call1442 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 451
  setline(451);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1443 == NULL) {
    strlit1443 = alloc_String("");
    gc_root(strlit1443);
  }
// compilenode returning strlit1443
  *var_line = strlit1443;
  if (strlit1443 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 452
  setline(452);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1444 = alloc_Float64(1);
// compilenode returning num1444
  *var_count = num1444;
  if (num1444 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 456
  setline(456);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1445 = gc_frame_new();
// Begin line 453
  setline(453);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_startPos
  int op_slot_left_1446 = gc_frame_newslot(*var_startPos);
// compilenode returning *var_curPos
  int op_slot_left_1447 = gc_frame_newslot(*var_curPos);
  Object num1448 = alloc_Float64(1);
// compilenode returning num1448
  int op_slot_right_1447 = gc_frame_newslot(num1448);
  params[0] = num1448;
  partcv[0] = 1;
  Object diff1450 = callmethod(*var_curPos, "-", 1, partcv, params);
// compilenode returning diff1450
  int op_slot_right_1446 = gc_frame_newslot(diff1450);
  params[0] = diff1450;
  partcv[0] = 1;
  Object opresult1452 = callmethod(*var_startPos, "..", 1, partcv, params);
// compilenode returning opresult1452
  gc_frame_newslot(opresult1452);
// Begin line 456
  setline(456);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1453 = alloc_Block(NULL, NULL, "PackageManager", 456);
  gc_frame_newslot(block1453);
  block_savedest(block1453);
  Object closure1454 = createclosure(3, "_apply");
setclosureframe(closure1454, stackframe);
  addtoclosure(closure1454, var_line);
  addtoclosure(closure1454, var_readFile);
  addtoclosure(closure1454, selfslot);
  struct UserObject *uo1454 = (struct UserObject*)block1453;
  uo1454->data[0] = (Object)closure1454;
  Method *meth_meth_PackageManager__apply1454 = addmethod2pos(block1453, "_apply", &meth_PackageManager__apply1454, 0);
int argcv_meth_PackageManager__apply1454[] = {1};
meth_meth_PackageManager__apply1454->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1454);
  meth_meth_PackageManager__apply1454->definitionModule = modulename;
  meth_meth_PackageManager__apply1454->definitionLine = 447;
// compilenode returning block1453
  gc_frame_newslot(block1453);
  params[0] = opresult1452;
  params[1] = block1453;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1461 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1445);
// compilenode returning call1461
// Begin line 456
  setline(456);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  params[0] = *var_line;
  Object call1462 = gracelib_print(NULL, 1,  params);
// compilenode returning call1462
// Begin line 479
  setline(479);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 457
  setline(457);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1466 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1467 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1466);
// compilenode returning call1467
// compilenode returning call1467
  int op_slot_left_1465 = gc_frame_newslot(call1467);
  Object num1468 = alloc_Float64(6);
// compilenode returning num1468
  int op_slot_right_1465 = gc_frame_newslot(num1468);
  params[0] = num1468;
  partcv[0] = 1;
  Object opresult1470 = callmethod(call1467, ">", 1, partcv, params);
// compilenode returning opresult1470
  int op_slot_left_1464 = gc_frame_newslot(opresult1470);
  int callframe1472 = gc_frame_new();
  Object num1473 = alloc_Float64(1);
// compilenode returning num1473
  gc_frame_newslot(num1473);
  Object num1474 = alloc_Float64(7);
// compilenode returning num1474
  gc_frame_newslot(num1474);
// compilenode returning *var_line
  params[0] = num1473;
  params[1] = num1474;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1475 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1472);
// compilenode returning call1475
  int op_slot_left_1471 = gc_frame_newslot(call1475);
  if (strlit1476 == NULL) {
    strlit1476 = alloc_String("import ");
    gc_root(strlit1476);
  }
// compilenode returning strlit1476
  int op_slot_right_1471 = gc_frame_newslot(strlit1476);
  params[0] = strlit1476;
  partcv[0] = 1;
  Object opresult1478 = callmethod(call1475, "==", 1, partcv, params);
// compilenode returning opresult1478
  int op_slot_right_1464 = gc_frame_newslot(opresult1478);
  params[0] = opresult1478;
  partcv[0] = 1;
  Object opresult1480 = callmethod(opresult1470, "&&", 1, partcv, params);
// compilenode returning opresult1480
  Object if1463 = done;
  if (istrue(opresult1480)) {
// Begin line 459
  setline(459);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 458
  setline(458);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1481 = alloc_Float64(1);
// compilenode returning num1481
  *var_pos = num1481;
  if (num1481 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 460
  setline(460);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 459
  setline(459);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_pos
  *var_start = *var_pos;
  if (*var_pos == undefined)
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
  if (strlit1482 == NULL) {
    strlit1482 = alloc_String("");
    gc_root(strlit1482);
  }
// compilenode returning strlit1482
  *var_nextImport = strlit1482;
  if (strlit1482 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 462
  setline(462);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1483 = gc_frame_new();
// Begin line 461
  setline(461);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// compilenode returning *var_pos
  gc_frame_newslot(*var_pos);
  params[0] = *var_line;
  params[1] = *var_pos;
  partcv[0] = 2;
  Object call1484 = callmethodflags(self, "skipWhiteSpace", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1483);
// compilenode returning call1484
  *var_pos = call1484;
  if (call1484 == undefined)
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
// compilenode returning *var_pos
  int op_slot_left_1486 = gc_frame_newslot(*var_pos);
  Object num1487 = alloc_Float64(1);
// compilenode returning num1487
  int op_slot_right_1486 = gc_frame_newslot(num1487);
  params[0] = num1487;
  partcv[0] = 1;
  Object sum1489 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1489
  *var_pos = sum1489;
  if (sum1489 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 463
  setline(463);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1492 == NULL) {
    strlit1492 = alloc_String("LINE = ");
    gc_root(strlit1492);
  }
// compilenode returning strlit1492
  int op_slot_left_1491 = gc_frame_newslot(strlit1492);
// compilenode returning *var_line
  int op_slot_right_1491 = gc_frame_newslot(*var_line);
  params[0] = *var_line;
  partcv[0] = 1;
  Object opresult1494 = callmethod(strlit1492, "++", 1, partcv, params);
// compilenode returning opresult1494
  params[0] = opresult1494;
  Object call1495 = gracelib_print(NULL, 1,  params);
// compilenode returning call1495
// Begin line 467
  setline(467);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1496 = gc_frame_new();
// Begin line 464
  setline(464);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1497 = alloc_Block(NULL, NULL, "PackageManager", 464);
  gc_frame_newslot(block1497);
  block_savedest(block1497);
  Object closure1498 = createclosure(3, "_apply");
setclosureframe(closure1498, stackframe);
  addtoclosure(closure1498, var_pos);
  addtoclosure(closure1498, var_line);
  addtoclosure(closure1498, selfslot);
  struct UserObject *uo1498 = (struct UserObject*)block1497;
  uo1498->data[0] = (Object)closure1498;
  Method *meth_meth_PackageManager__apply1498 = addmethod2pos(block1497, "_apply", &meth_PackageManager__apply1498, 0);
int argcv_meth_PackageManager__apply1498[] = {0};
meth_meth_PackageManager__apply1498->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1498);
  meth_meth_PackageManager__apply1498->definitionModule = modulename;
  meth_meth_PackageManager__apply1498->definitionLine = 457;
// compilenode returning block1497
  gc_frame_newslot(block1497);
// Begin line 467
  setline(467);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1523 = alloc_Block(NULL, NULL, "PackageManager", 467);
  gc_frame_newslot(block1523);
  block_savedest(block1523);
  Object closure1524 = createclosure(2, "_apply");
setclosureframe(closure1524, stackframe);
  addtoclosure(closure1524, var_pos);
  addtoclosure(closure1524, selfslot);
  struct UserObject *uo1524 = (struct UserObject*)block1523;
  uo1524->data[0] = (Object)closure1524;
  Method *meth_meth_PackageManager__apply1524 = addmethod2pos(block1523, "_apply", &meth_PackageManager__apply1524, 0);
int argcv_meth_PackageManager__apply1524[] = {0};
meth_meth_PackageManager__apply1524->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1524);
  meth_meth_PackageManager__apply1524->definitionModule = modulename;
  meth_meth_PackageManager__apply1524->definitionLine = 464;
// compilenode returning block1523
  gc_frame_newslot(block1523);
  params[0] = block1497;
  params[1] = block1523;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1531 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1496);
// compilenode returning call1531
// Begin line 467
  setline(467);
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
// Begin line 468
  setline(468);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1540 == NULL) {
    strlit1540 = alloc_String("import ""\x22""");
    gc_root(strlit1540);
  }
// compilenode returning strlit1540
  int op_slot_left_1539 = gc_frame_newslot(strlit1540);
// compilenode returning *var_prefix
  int op_slot_right_1539 = gc_frame_newslot(*var_prefix);
  params[0] = *var_prefix;
  partcv[0] = 1;
  Object opresult1542 = callmethod(strlit1540, "++", 1, partcv, params);
// compilenode returning opresult1542
  int op_slot_left_1538 = gc_frame_newslot(opresult1542);
  if (strlit1543 == NULL) {
    strlit1543 = alloc_String("/");
    gc_root(strlit1543);
  }
// compilenode returning strlit1543
  int op_slot_right_1538 = gc_frame_newslot(strlit1543);
  params[0] = strlit1543;
  partcv[0] = 1;
  Object opresult1545 = callmethod(opresult1542, "++", 1, partcv, params);
// compilenode returning opresult1545
  int op_slot_left_1537 = gc_frame_newslot(opresult1545);
  int callframe1546 = gc_frame_new();
// compilenode returning *var_pos
  int op_slot_left_1547 = gc_frame_newslot(*var_pos);
  Object num1548 = alloc_Float64(1);
// compilenode returning num1548
  int op_slot_right_1547 = gc_frame_newslot(num1548);
  params[0] = num1548;
  partcv[0] = 1;
  Object sum1550 = callmethod(*var_pos, "+", 1, partcv, params);
// compilenode returning sum1550
  gc_frame_newslot(sum1550);
  int callframe1551 = gc_frame_new();
// compilenode returning *var_line
  partcv[0] = 0;
  Object call1552 = callmethod(*var_line, "size",
    1, partcv, params);
  gc_frame_end(callframe1551);
// compilenode returning call1552
// compilenode returning call1552
  gc_frame_newslot(call1552);
// compilenode returning *var_line
  params[0] = sum1550;
  params[1] = call1552;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1553 = callmethod(*var_line, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe1546);
// compilenode returning call1553
  int op_slot_right_1537 = gc_frame_newslot(call1553);
  params[0] = call1553;
  partcv[0] = 1;
  Object opresult1555 = callmethod(opresult1545, "++", 1, partcv, params);
// compilenode returning opresult1555
  *var_line = opresult1555;
  if (opresult1555 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 469
  setline(469);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  params[0] = *var_line;
  Object call1557 = gracelib_print(NULL, 1,  params);
// compilenode returning call1557
// Begin line 473
  setline(473);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1558 = gc_frame_new();
// Begin line 470
  setline(470);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// Begin line 473
  setline(473);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1559 = alloc_Block(NULL, NULL, "PackageManager", 473);
  gc_frame_newslot(block1559);
  block_savedest(block1559);
  Object closure1560 = createclosure(2, "_apply");
setclosureframe(closure1560, stackframe);
  addtoclosure(closure1560, var_outFile);
  addtoclosure(closure1560, selfslot);
  struct UserObject *uo1560 = (struct UserObject*)block1559;
  uo1560->data[0] = (Object)closure1560;
  Method *meth_meth_PackageManager__apply1560 = addmethod2pos(block1559, "_apply", &meth_PackageManager__apply1560, 0);
int argcv_meth_PackageManager__apply1560[] = {1};
meth_meth_PackageManager__apply1560->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1560);
  meth_meth_PackageManager__apply1560->definitionModule = modulename;
  meth_meth_PackageManager__apply1560->definitionLine = 468;
// compilenode returning block1559
  gc_frame_newslot(block1559);
  params[0] = *var_line;
  params[1] = block1559;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1564 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1558);
// compilenode returning call1564
    gc_frame_newslot(call1564);
    if1463 = call1564;
  } else {
// Begin line 478
  setline(478);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1565 = gc_frame_new();
// Begin line 475
  setline(475);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_line
  gc_frame_newslot(*var_line);
// Begin line 478
  setline(478);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1566 = alloc_Block(NULL, NULL, "PackageManager", 478);
  gc_frame_newslot(block1566);
  block_savedest(block1566);
  Object closure1567 = createclosure(2, "_apply");
setclosureframe(closure1567, stackframe);
  addtoclosure(closure1567, var_outFile);
  addtoclosure(closure1567, selfslot);
  struct UserObject *uo1567 = (struct UserObject*)block1566;
  uo1567->data[0] = (Object)closure1567;
  Method *meth_meth_PackageManager__apply1567 = addmethod2pos(block1566, "_apply", &meth_PackageManager__apply1567, 0);
int argcv_meth_PackageManager__apply1567[] = {1};
meth_meth_PackageManager__apply1567->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1567);
  meth_meth_PackageManager__apply1567->definitionModule = modulename;
  meth_meth_PackageManager__apply1567->definitionLine = 468;
// compilenode returning block1566
  gc_frame_newslot(block1566);
  params[0] = *var_line;
  params[1] = block1566;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1571 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1565);
// compilenode returning call1571
    gc_frame_newslot(call1571);
    if1463 = call1571;
  }
// compilenode returning if1463
// Begin line 480
  setline(480);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 479
  setline(479);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_1572 = gc_frame_newslot(*var_curPos);
  Object num1573 = alloc_Float64(1);
// compilenode returning num1573
  int op_slot_right_1572 = gc_frame_newslot(num1573);
  params[0] = num1573;
  partcv[0] = 1;
  Object sum1575 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum1575
  *var_curPos = sum1575;
  if (sum1575 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_PackageManager__apply1398(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 481
  setline(481);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1399 = gc_frame_new();
// Begin line 445
  setline(445);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1400 = alloc_Block(NULL, NULL, "PackageManager", 445);
  gc_frame_newslot(block1400);
  block_savedest(block1400);
  Object closure1401 = createclosure(3, "_apply");
setclosureframe(closure1401, stackframe);
  addtoclosure(closure1401, var_curPos);
  addtoclosure(closure1401, var_readFile);
  addtoclosure(closure1401, selfslot);
  struct UserObject *uo1401 = (struct UserObject*)block1400;
  uo1401->data[0] = (Object)closure1401;
  Method *meth_meth_PackageManager__apply1401 = addmethod2pos(block1400, "_apply", &meth_PackageManager__apply1401, 0);
int argcv_meth_PackageManager__apply1401[] = {0};
meth_meth_PackageManager__apply1401->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1401);
  meth_meth_PackageManager__apply1401->definitionModule = modulename;
  meth_meth_PackageManager__apply1401->definitionLine = 444;
// compilenode returning block1400
  gc_frame_newslot(block1400);
// Begin line 481
  setline(481);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1408 = alloc_Block(NULL, NULL, "PackageManager", 481);
  gc_frame_newslot(block1408);
  block_savedest(block1408);
  Object closure1409 = createclosure(6, "_apply");
setclosureframe(closure1409, stackframe);
  addtoclosure(closure1409, var_curPos);
  addtoclosure(closure1409, var_startPos);
  addtoclosure(closure1409, var_readFile);
  addtoclosure(closure1409, var_prefix);
  addtoclosure(closure1409, var_outFile);
  addtoclosure(closure1409, selfslot);
  struct UserObject *uo1409 = (struct UserObject*)block1408;
  uo1409->data[0] = (Object)closure1409;
  Method *meth_meth_PackageManager__apply1409 = addmethod2pos(block1408, "_apply", &meth_PackageManager__apply1409, 0);
int argcv_meth_PackageManager__apply1409[] = {0};
meth_meth_PackageManager__apply1409->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1409);
  meth_meth_PackageManager__apply1409->definitionModule = modulename;
  meth_meth_PackageManager__apply1409->definitionLine = 445;
// compilenode returning block1408
  gc_frame_newslot(block1408);
  params[0] = block1400;
  params[1] = block1408;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1578 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1399);
// compilenode returning call1578
  gc_frame_end(frame);
  return call1578;
}
Object meth_PackageManager_parseAndPrefix1387(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
// Begin line 441
  setline(441);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1388 = alloc_Float64(1);
// compilenode returning num1388
  *var_curPos = num1388;
  if (num1388 == undefined)
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
// compilenode returning *var_curPos
  *var_startPos = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 443
  setline(443);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array1389 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array1389
  *var_outFile = array1389;
  if (array1389 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 484
  setline(484);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1390 = gc_frame_new();
// Begin line 444
  setline(444);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1392 = alloc_Float64(0);
// compilenode returning num1392
  int op_slot_left_1391 = gc_frame_newslot(num1392);
  int callframe1393 = gc_frame_new();
// compilenode returning *var_readFile
  partcv[0] = 0;
  Object call1394 = callmethod(*var_readFile, "size",
    1, partcv, params);
  gc_frame_end(callframe1393);
// compilenode returning call1394
// compilenode returning call1394
  int op_slot_right_1391 = gc_frame_newslot(call1394);
  params[0] = call1394;
  partcv[0] = 1;
  Object opresult1396 = callmethod(num1392, "..", 1, partcv, params);
// compilenode returning opresult1396
  gc_frame_newslot(opresult1396);
// Begin line 484
  setline(484);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block1397 = alloc_Block(NULL, NULL, "PackageManager", 484);
  gc_frame_newslot(block1397);
  block_savedest(block1397);
  Object closure1398 = createclosure(6, "_apply");
setclosureframe(closure1398, stackframe);
  addtoclosure(closure1398, var_curPos);
  addtoclosure(closure1398, var_readFile);
  addtoclosure(closure1398, var_startPos);
  addtoclosure(closure1398, var_prefix);
  addtoclosure(closure1398, var_outFile);
  addtoclosure(closure1398, selfslot);
  struct UserObject *uo1398 = (struct UserObject*)block1397;
  uo1398->data[0] = (Object)closure1398;
  Method *meth_meth_PackageManager__apply1398 = addmethod2pos(block1397, "_apply", &meth_PackageManager__apply1398, 0);
int argcv_meth_PackageManager__apply1398[] = {1};
meth_meth_PackageManager__apply1398->type = alloc_MethodType(1, argcv_meth_PackageManager__apply1398);
  meth_meth_PackageManager__apply1398->definitionModule = modulename;
  meth_meth_PackageManager__apply1398->definitionLine = 444;
// compilenode returning block1397
  gc_frame_newslot(block1397);
  params[0] = opresult1396;
  params[1] = block1397;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call1580 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1390);
// compilenode returning call1580
// Begin line 485
  setline(485);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 484
  setline(484);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit1582 == NULL) {
    strlit1582 = alloc_String("OUTFILE ");
    gc_root(strlit1582);
  }
// compilenode returning strlit1582
  int op_slot_left_1581 = gc_frame_newslot(strlit1582);
// compilenode returning *var_outFile
  int op_slot_right_1581 = gc_frame_newslot(*var_outFile);
  params[0] = *var_outFile;
  partcv[0] = 1;
  Object opresult1584 = callmethod(strlit1582, "++", 1, partcv, params);
// compilenode returning opresult1584
  params[0] = opresult1584;
  Object call1585 = gracelib_print(NULL, 1,  params);
// compilenode returning call1585
// Begin line 485
  setline(485);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_outFile
  return *var_outFile;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_PackageManager_imported1586(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 23, (flags>>24)&0xff);
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
Object meth_PackageManager_imported_58__61_1587(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 24, (flags>>24)&0xff);
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
Object meth_PackageManager_toProcess1589(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 25, (flags>>24)&0xff);
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
Object meth_PackageManager_toProcess_58__61_1590(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 26, (flags>>24)&0xff);
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
Object meth_PackageManager_verbose1593(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 27, (flags>>24)&0xff);
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
Object meth_PackageManager_verbose_58__61_1594(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 28, (flags>>24)&0xff);
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
Object meth_PackageManager_global1597(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 29, (flags>>24)&0xff);
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
Object meth_PackageManager_global_58__61_1598(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 30, (flags>>24)&0xff);
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
Object meth_PackageManager_curFile1600(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 31, (flags>>24)&0xff);
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
// Begin line 9
  setline(9);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curFile
  gc_frame_end(frame);
  return *var_curFile;
}
Object meth_PackageManager_curFile_58__61_1601(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 32, (flags>>24)&0xff);
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
  Object self = alloc_obj2(36, 36);
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
  struct StackFrameObject *stackframe = alloc_StackFrame(36, NULL);
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
  Object *var_curFile = &(stackframe->slots[5]);
  setframeelementname(stackframe, 5, "curFile");
// Begin line 13
  setline(13);
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
  meth_meth_PackageManager_parseArgs0->definitionLine = 13;
// compilenode returning 
// Begin line 45
  setline(45);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo60 = (struct UserObject*)self;
  uo60->data[2] = emptyclosure;
  Method *meth_meth_PackageManager_forArgs_40__41_do60 = addmethod2pos(self, "forArgs()do", &meth_PackageManager_forArgs_40__41_do60, 2);
int argcv_meth_PackageManager_forArgs_40__41_do60[] = {1, 1};
meth_meth_PackageManager_forArgs_40__41_do60->type = alloc_MethodType(2, argcv_meth_PackageManager_forArgs_40__41_do60);
  meth_meth_PackageManager_forArgs_40__41_do60->flags |= MFLAG_CONFIDENTIAL;
  meth_meth_PackageManager_forArgs_40__41_do60->definitionModule = modulename;
  meth_meth_PackageManager_forArgs_40__41_do60->definitionLine = 45;
  Method *meth_meth_PackageManager_forArgs_40__41_do60_object = addmethod2pos(self, "forArgs()do()object", &meth_PackageManager_forArgs_40__41_do60_object, 2);
int argcv_meth_PackageManager_forArgs_40__41_do60_object[] = {1, 1};
meth_meth_PackageManager_forArgs_40__41_do60_object->type = alloc_MethodType(2, argcv_meth_PackageManager_forArgs_40__41_do60_object);
  meth_meth_PackageManager_forArgs_40__41_do60_object->flags |= MFLAG_CONFIDENTIAL;
  meth_meth_PackageManager_forArgs_40__41_do60_object->definitionModule = modulename;
  meth_meth_PackageManager_forArgs_40__41_do60_object->definitionLine = 45;
// compilenode returning 
// Begin line 112
  setline(112);
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
  meth_meth_PackageManager_listInstalled286->definitionLine = 112;
// compilenode returning 
// Begin line 125
  setline(125);
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
  meth_meth_PackageManager_recurseDirectory321->definitionLine = 125;
// compilenode returning 
// Begin line 137
  setline(137);
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
  meth_meth_PackageManager_doGet374->definitionLine = 137;
// compilenode returning 
// Begin line 148
  setline(148);
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
  meth_meth_PackageManager_setFile393->definitionLine = 148;
// compilenode returning 
// Begin line 160
  setline(160);
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
  meth_meth_PackageManager_fetchImports416->definitionLine = 160;
// compilenode returning 
// Begin line 185
  setline(185);
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
  meth_meth_PackageManager_setFileData483->definitionLine = 185;
// compilenode returning 
// Begin line 221
  setline(221);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure602 = createclosure(2, "findExisting");
setclosureframe(closure602, stackframe);
  addtoclosure(closure602, var_io);
  addtoclosure(closure602, var_sys);
  struct UserObject *uo602 = (struct UserObject*)self;
  uo602->data[9] = (Object)closure602;
  Method *meth_meth_PackageManager_findExisting602 = addmethod2pos(self, "findExisting", &meth_PackageManager_findExisting602, 9);
int argcv_meth_PackageManager_findExisting602[] = {1};
meth_meth_PackageManager_findExisting602->type = alloc_MethodType(1, argcv_meth_PackageManager_findExisting602);
  meth_meth_PackageManager_findExisting602->definitionModule = modulename;
  meth_meth_PackageManager_findExisting602->definitionLine = 221;
// compilenode returning 
// Begin line 243
  setline(243);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo730 = (struct UserObject*)self;
  uo730->data[10] = emptyclosure;
  Method *meth_meth_PackageManager_validateFile730 = addmethod2pos(self, "validateFile", &meth_PackageManager_validateFile730, 10);
int argcv_meth_PackageManager_validateFile730[] = {1};
meth_meth_PackageManager_validateFile730->type = alloc_MethodType(1, argcv_meth_PackageManager_validateFile730);
  meth_meth_PackageManager_validateFile730->definitionModule = modulename;
  meth_meth_PackageManager_validateFile730->definitionLine = 243;
// compilenode returning 
// Begin line 253
  setline(253);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure753 = createclosure(3, "write");
setclosureframe(closure753, stackframe);
  addtoclosure(closure753, var_global);
  addtoclosure(closure753, var_sys);
  addtoclosure(closure753, var_io);
  struct UserObject *uo753 = (struct UserObject*)self;
  uo753->data[11] = (Object)closure753;
  Method *meth_meth_PackageManager_write753 = addmethod2pos(self, "write", &meth_PackageManager_write753, 11);
int argcv_meth_PackageManager_write753[] = {1};
meth_meth_PackageManager_write753->type = alloc_MethodType(1, argcv_meth_PackageManager_write753);
  meth_meth_PackageManager_write753->definitionModule = modulename;
  meth_meth_PackageManager_write753->definitionLine = 253;
// compilenode returning 
// Begin line 268
  setline(268);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure790 = createclosure(3, "compile");
setclosureframe(closure790, stackframe);
  addtoclosure(closure790, var_global);
  addtoclosure(closure790, var_sys);
  addtoclosure(closure790, var_io);
  struct UserObject *uo790 = (struct UserObject*)self;
  uo790->data[12] = (Object)closure790;
  Method *meth_meth_PackageManager_compile790 = addmethod2pos(self, "compile", &meth_PackageManager_compile790, 12);
int argcv_meth_PackageManager_compile790[] = {1};
meth_meth_PackageManager_compile790->type = alloc_MethodType(1, argcv_meth_PackageManager_compile790);
  meth_meth_PackageManager_compile790->definitionModule = modulename;
  meth_meth_PackageManager_compile790->definitionLine = 268;
// compilenode returning 
// Begin line 295
  setline(295);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure875 = createclosure(1, "createDirectory");
setclosureframe(closure875, stackframe);
  addtoclosure(closure875, var_io);
  struct UserObject *uo875 = (struct UserObject*)self;
  uo875->data[13] = (Object)closure875;
  Method *meth_meth_PackageManager_createDirectory875 = addmethod2pos(self, "createDirectory", &meth_PackageManager_createDirectory875, 13);
int argcv_meth_PackageManager_createDirectory875[] = {1};
meth_meth_PackageManager_createDirectory875->type = alloc_MethodType(1, argcv_meth_PackageManager_createDirectory875);
  meth_meth_PackageManager_createDirectory875->definitionModule = modulename;
  meth_meth_PackageManager_createDirectory875->definitionLine = 295;
// compilenode returning 
// Begin line 319
  setline(319);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure953 = createclosure(1, "parseFile");
setclosureframe(closure953, stackframe);
  addtoclosure(closure953, var_done);
  struct UserObject *uo953 = (struct UserObject*)self;
  uo953->data[14] = (Object)closure953;
  Method *meth_meth_PackageManager_parseFile953 = addmethod2pos(self, "parseFile", &meth_PackageManager_parseFile953, 14);
int argcv_meth_PackageManager_parseFile953[] = {1};
meth_meth_PackageManager_parseFile953->type = alloc_MethodType(1, argcv_meth_PackageManager_parseFile953);
  meth_meth_PackageManager_parseFile953->definitionModule = modulename;
  meth_meth_PackageManager_parseFile953->definitionLine = 319;
// compilenode returning 
// Begin line 336
  setline(336);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1013 = (struct UserObject*)self;
  uo1013->data[15] = emptyclosure;
  Method *meth_meth_PackageManager_processLine1013 = addmethod2pos(self, "processLine", &meth_PackageManager_processLine1013, 15);
int argcv_meth_PackageManager_processLine1013[] = {1};
meth_meth_PackageManager_processLine1013->type = alloc_MethodType(1, argcv_meth_PackageManager_processLine1013);
  meth_meth_PackageManager_processLine1013->definitionModule = modulename;
  meth_meth_PackageManager_processLine1013->definitionLine = 336;
// compilenode returning 
// Begin line 355
  setline(355);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1094 = createclosure(2, "parseImport");
setclosureframe(closure1094, stackframe);
  addtoclosure(closure1094, var_imported);
  addtoclosure(closure1094, var_toProcess);
  struct UserObject *uo1094 = (struct UserObject*)self;
  uo1094->data[16] = (Object)closure1094;
  Method *meth_meth_PackageManager_parseImport1094 = addmethod2pos(self, "parseImport", &meth_PackageManager_parseImport1094, 16);
int argcv_meth_PackageManager_parseImport1094[] = {1};
meth_meth_PackageManager_parseImport1094->type = alloc_MethodType(1, argcv_meth_PackageManager_parseImport1094);
  meth_meth_PackageManager_parseImport1094->definitionModule = modulename;
  meth_meth_PackageManager_parseImport1094->definitionLine = 355;
// compilenode returning 
// Begin line 372
  setline(372);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1165 = (struct UserObject*)self;
  uo1165->data[17] = emptyclosure;
  Method *meth_meth_PackageManager_skipWhiteSpace1165 = addmethod2pos(self, "skipWhiteSpace", &meth_PackageManager_skipWhiteSpace1165, 17);
int argcv_meth_PackageManager_skipWhiteSpace1165[] = {2};
meth_meth_PackageManager_skipWhiteSpace1165->type = alloc_MethodType(1, argcv_meth_PackageManager_skipWhiteSpace1165);
  meth_meth_PackageManager_skipWhiteSpace1165->definitionModule = modulename;
  meth_meth_PackageManager_skipWhiteSpace1165->definitionLine = 372;
// compilenode returning 
// Begin line 380
  setline(380);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1192 = (struct UserObject*)self;
  uo1192->data[18] = emptyclosure;
  Method *meth_meth_PackageManager_displayHelp1192 = addmethod2pos(self, "displayHelp", &meth_PackageManager_displayHelp1192, 18);
int argcv_meth_PackageManager_displayHelp1192[] = {0};
meth_meth_PackageManager_displayHelp1192->type = alloc_MethodType(1, argcv_meth_PackageManager_displayHelp1192);
  meth_meth_PackageManager_displayHelp1192->definitionModule = modulename;
  meth_meth_PackageManager_displayHelp1192->definitionLine = 380;
// compilenode returning 
// Begin line 386
  setline(386);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1199 = createclosure(1, "printMessage");
setclosureframe(closure1199, stackframe);
  addtoclosure(closure1199, var_verbose);
  struct UserObject *uo1199 = (struct UserObject*)self;
  uo1199->data[19] = (Object)closure1199;
  Method *meth_meth_PackageManager_printMessage1199 = addmethod2pos(self, "printMessage", &meth_PackageManager_printMessage1199, 19);
int argcv_meth_PackageManager_printMessage1199[] = {1};
meth_meth_PackageManager_printMessage1199->type = alloc_MethodType(1, argcv_meth_PackageManager_printMessage1199);
  meth_meth_PackageManager_printMessage1199->definitionModule = modulename;
  meth_meth_PackageManager_printMessage1199->definitionLine = 386;
// compilenode returning 
// Begin line 395
  setline(395);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1202 = createclosure(2, "bundle");
setclosureframe(closure1202, stackframe);
  addtoclosure(closure1202, var_imported);
  addtoclosure(closure1202, var_io);
  struct UserObject *uo1202 = (struct UserObject*)self;
  uo1202->data[20] = (Object)closure1202;
  Method *meth_meth_PackageManager_bundle1202 = addmethod2pos(self, "bundle", &meth_PackageManager_bundle1202, 20);
int argcv_meth_PackageManager_bundle1202[] = {2};
meth_meth_PackageManager_bundle1202->type = alloc_MethodType(1, argcv_meth_PackageManager_bundle1202);
  meth_meth_PackageManager_bundle1202->definitionModule = modulename;
  meth_meth_PackageManager_bundle1202->definitionLine = 395;
  block_savedest(self);
  Object closure1310 = createclosure(2, "bundle()object");
setclosureframe(closure1310, stackframe);
  addtoclosure(closure1310, var_imported);
  addtoclosure(closure1310, var_io);
  Method *meth_meth_PackageManager_bundle1202_object = addmethod2pos(self, "bundle()object", &meth_PackageManager_bundle1202_object, 20);
  meth_meth_PackageManager_bundle1202_object->definitionModule = modulename;
  meth_meth_PackageManager_bundle1202_object->definitionLine = 395;
// compilenode returning 
// Begin line 416
  setline(416);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1311 = createclosure(1, "setImportDest");
setclosureframe(closure1311, stackframe);
  addtoclosure(closure1311, var_io);
  struct UserObject *uo1311 = (struct UserObject*)self;
  uo1311->data[21] = (Object)closure1311;
  Method *meth_meth_PackageManager_setImportDest1311 = addmethod2pos(self, "setImportDest", &meth_PackageManager_setImportDest1311, 21);
int argcv_meth_PackageManager_setImportDest1311[] = {2};
meth_meth_PackageManager_setImportDest1311->type = alloc_MethodType(1, argcv_meth_PackageManager_setImportDest1311);
  meth_meth_PackageManager_setImportDest1311->definitionModule = modulename;
  meth_meth_PackageManager_setImportDest1311->definitionLine = 416;
// compilenode returning 
// Begin line 440
  setline(440);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo1387 = (struct UserObject*)self;
  uo1387->data[22] = emptyclosure;
  Method *meth_meth_PackageManager_parseAndPrefix1387 = addmethod2pos(self, "parseAndPrefix", &meth_PackageManager_parseAndPrefix1387, 22);
int argcv_meth_PackageManager_parseAndPrefix1387[] = {3};
meth_meth_PackageManager_parseAndPrefix1387->type = alloc_MethodType(1, argcv_meth_PackageManager_parseAndPrefix1387);
meth_meth_PackageManager_parseAndPrefix1387->type->types[0] = type_String;
meth_meth_PackageManager_parseAndPrefix1387->type->names[0] = "readFile";
meth_meth_PackageManager_parseAndPrefix1387->type->types[1] = type_String;
meth_meth_PackageManager_parseAndPrefix1387->type->names[1] = "address";
meth_meth_PackageManager_parseAndPrefix1387->type->types[2] = type_String;
meth_meth_PackageManager_parseAndPrefix1387->type->names[2] = "prefix";
  meth_meth_PackageManager_parseAndPrefix1387->definitionModule = modulename;
  meth_meth_PackageManager_parseAndPrefix1387->definitionLine = 440;
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
// Begin line 468
  setline(468);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1586 = createclosure(1, "imported");
setclosureframe(closure1586, stackframe);
  addtoclosure(closure1586, var_imported);
  struct UserObject *uo1586 = (struct UserObject*)self;
  uo1586->data[23] = (Object)closure1586;
  Method *meth_meth_PackageManager_imported1586 = addmethod2pos(self, "imported", &meth_PackageManager_imported1586, 23);
int argcv_meth_PackageManager_imported1586[] = {0};
meth_meth_PackageManager_imported1586->type = alloc_MethodType(1, argcv_meth_PackageManager_imported1586);
  meth_meth_PackageManager_imported1586->definitionModule = modulename;
  meth_meth_PackageManager_imported1586->definitionLine = 468;
// compilenode returning 
// Begin line 468
  setline(468);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1587 = createclosure(1, "imported:=");
setclosureframe(closure1587, stackframe);
  addtoclosure(closure1587, var_imported);
  struct UserObject *uo1587 = (struct UserObject*)self;
  uo1587->data[24] = (Object)closure1587;
  Method *meth_meth_PackageManager_imported_58__61_1587 = addmethod2pos(self, "imported:=", &meth_PackageManager_imported_58__61_1587, 24);
int argcv_meth_PackageManager_imported_58__61_1587[] = {1};
meth_meth_PackageManager_imported_58__61_1587->type = alloc_MethodType(1, argcv_meth_PackageManager_imported_58__61_1587);
  meth_meth_PackageManager_imported_58__61_1587->definitionModule = modulename;
  meth_meth_PackageManager_imported_58__61_1587->definitionLine = 468;
// compilenode returning 
// compilenode returning done
// Begin line 7
  setline(7);
  setmodule(modulename);
  setsource(originalSourceLines);
  *var_toProcess = undefined;
// Begin line 468
  setline(468);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1589 = createclosure(1, "toProcess");
setclosureframe(closure1589, stackframe);
  addtoclosure(closure1589, var_toProcess);
  struct UserObject *uo1589 = (struct UserObject*)self;
  uo1589->data[25] = (Object)closure1589;
  Method *meth_meth_PackageManager_toProcess1589 = addmethod2pos(self, "toProcess", &meth_PackageManager_toProcess1589, 25);
int argcv_meth_PackageManager_toProcess1589[] = {0};
meth_meth_PackageManager_toProcess1589->type = alloc_MethodType(1, argcv_meth_PackageManager_toProcess1589);
  meth_meth_PackageManager_toProcess1589->definitionModule = modulename;
  meth_meth_PackageManager_toProcess1589->definitionLine = 468;
// compilenode returning 
// Begin line 468
  setline(468);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1590 = createclosure(1, "toProcess:=");
setclosureframe(closure1590, stackframe);
  addtoclosure(closure1590, var_toProcess);
  struct UserObject *uo1590 = (struct UserObject*)self;
  uo1590->data[26] = (Object)closure1590;
  Method *meth_meth_PackageManager_toProcess_58__61_1590 = addmethod2pos(self, "toProcess:=", &meth_PackageManager_toProcess_58__61_1590, 26);
int argcv_meth_PackageManager_toProcess_58__61_1590[] = {1};
meth_meth_PackageManager_toProcess_58__61_1590->type = alloc_MethodType(1, argcv_meth_PackageManager_toProcess_58__61_1590);
  meth_meth_PackageManager_toProcess_58__61_1590->definitionModule = modulename;
  meth_meth_PackageManager_toProcess_58__61_1590->definitionLine = 468;
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
  Object bool1592 = alloc_Boolean(0);
// compilenode returning bool1592
  *var_verbose = bool1592;
  if (bool1592 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 468
  setline(468);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1593 = createclosure(1, "verbose");
setclosureframe(closure1593, stackframe);
  addtoclosure(closure1593, var_verbose);
  struct UserObject *uo1593 = (struct UserObject*)self;
  uo1593->data[27] = (Object)closure1593;
  Method *meth_meth_PackageManager_verbose1593 = addmethod2pos(self, "verbose", &meth_PackageManager_verbose1593, 27);
int argcv_meth_PackageManager_verbose1593[] = {0};
meth_meth_PackageManager_verbose1593->type = alloc_MethodType(1, argcv_meth_PackageManager_verbose1593);
  meth_meth_PackageManager_verbose1593->definitionModule = modulename;
  meth_meth_PackageManager_verbose1593->definitionLine = 468;
// compilenode returning 
// Begin line 468
  setline(468);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1594 = createclosure(1, "verbose:=");
setclosureframe(closure1594, stackframe);
  addtoclosure(closure1594, var_verbose);
  struct UserObject *uo1594 = (struct UserObject*)self;
  uo1594->data[28] = (Object)closure1594;
  Method *meth_meth_PackageManager_verbose_58__61_1594 = addmethod2pos(self, "verbose:=", &meth_PackageManager_verbose_58__61_1594, 28);
int argcv_meth_PackageManager_verbose_58__61_1594[] = {1};
meth_meth_PackageManager_verbose_58__61_1594->type = alloc_MethodType(1, argcv_meth_PackageManager_verbose_58__61_1594);
  meth_meth_PackageManager_verbose_58__61_1594->definitionModule = modulename;
  meth_meth_PackageManager_verbose_58__61_1594->definitionLine = 468;
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
  Object bool1596 = alloc_Boolean(0);
// compilenode returning bool1596
  *var_global = bool1596;
  if (bool1596 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 468
  setline(468);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1597 = createclosure(1, "global");
setclosureframe(closure1597, stackframe);
  addtoclosure(closure1597, var_global);
  struct UserObject *uo1597 = (struct UserObject*)self;
  uo1597->data[29] = (Object)closure1597;
  Method *meth_meth_PackageManager_global1597 = addmethod2pos(self, "global", &meth_PackageManager_global1597, 29);
int argcv_meth_PackageManager_global1597[] = {0};
meth_meth_PackageManager_global1597->type = alloc_MethodType(1, argcv_meth_PackageManager_global1597);
  meth_meth_PackageManager_global1597->definitionModule = modulename;
  meth_meth_PackageManager_global1597->definitionLine = 468;
// compilenode returning 
// Begin line 468
  setline(468);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1598 = createclosure(1, "global:=");
setclosureframe(closure1598, stackframe);
  addtoclosure(closure1598, var_global);
  struct UserObject *uo1598 = (struct UserObject*)self;
  uo1598->data[30] = (Object)closure1598;
  Method *meth_meth_PackageManager_global_58__61_1598 = addmethod2pos(self, "global:=", &meth_PackageManager_global_58__61_1598, 30);
int argcv_meth_PackageManager_global_58__61_1598[] = {1};
meth_meth_PackageManager_global_58__61_1598->type = alloc_MethodType(1, argcv_meth_PackageManager_global_58__61_1598);
  meth_meth_PackageManager_global_58__61_1598->definitionModule = modulename;
  meth_meth_PackageManager_global_58__61_1598->definitionLine = 468;
// compilenode returning 
// compilenode returning done
// Begin line 9
  setline(9);
  setmodule(modulename);
  setsource(originalSourceLines);
  *var_curFile = undefined;
// Begin line 468
  setline(468);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1600 = createclosure(1, "curFile");
setclosureframe(closure1600, stackframe);
  addtoclosure(closure1600, var_curFile);
  struct UserObject *uo1600 = (struct UserObject*)self;
  uo1600->data[31] = (Object)closure1600;
  Method *meth_meth_PackageManager_curFile1600 = addmethod2pos(self, "curFile", &meth_PackageManager_curFile1600, 31);
int argcv_meth_PackageManager_curFile1600[] = {0};
meth_meth_PackageManager_curFile1600->type = alloc_MethodType(1, argcv_meth_PackageManager_curFile1600);
  meth_meth_PackageManager_curFile1600->definitionModule = modulename;
  meth_meth_PackageManager_curFile1600->definitionLine = 468;
// compilenode returning 
// Begin line 468
  setline(468);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1601 = createclosure(1, "curFile:=");
setclosureframe(closure1601, stackframe);
  addtoclosure(closure1601, var_curFile);
  struct UserObject *uo1601 = (struct UserObject*)self;
  uo1601->data[32] = (Object)closure1601;
  Method *meth_meth_PackageManager_curFile_58__61_1601 = addmethod2pos(self, "curFile:=", &meth_PackageManager_curFile_58__61_1601, 32);
int argcv_meth_PackageManager_curFile_58__61_1601[] = {1};
meth_meth_PackageManager_curFile_58__61_1601->type = alloc_MethodType(1, argcv_meth_PackageManager_curFile_58__61_1601);
  meth_meth_PackageManager_curFile_58__61_1601->definitionModule = modulename;
  meth_meth_PackageManager_curFile_58__61_1601->definitionLine = 468;
// compilenode returning 
// compilenode returning done
// Begin line 11
  setline(11);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
  int callframe1603 = gc_frame_new();
  int callframe1604 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call1605 = callmethod(*var_sys, "argv",
    1, partcv, params);
  gc_frame_end(callframe1604);
// compilenode returning call1605
// compilenode returning call1605
  gc_frame_newslot(call1605);
  params[0] = call1605;
  partcv[0] = 1;
  Object call1606 = callmethodflags(self, "parseArgs", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe1603);
// compilenode returning call1606
// Begin line 13
  setline(13);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// compilenode returning 
// Begin line 45
  setline(45);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 112
  setline(112);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 125
  setline(125);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 137
  setline(137);
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
// Begin line 221
  setline(221);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 243
  setline(243);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 253
  setline(253);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 268
  setline(268);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 295
  setline(295);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 319
  setline(319);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 336
  setline(336);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 355
  setline(355);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 372
  setline(372);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 380
  setline(380);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 386
  setline(386);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 395
  setline(395);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
// Begin line 416
  setline(416);
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
