import "sys" as sys
import "io" as io
import "curl" as curl

var imported
var toProcess := []
var verbose := false
var global := false
var bundlePath := "";
var curFile;

parseArgs(sys.argv);

method parseArgs(args : List<String>) {
    var count := 0;
    print(args.size);
    forArgs(args) do { arg, on ->
        print("in loop");
        on.option "get" do { toGet->
            doGet(toGet);
        }
        on.flag "list" do { 
            listInstalled();
        }
        on.doubleValue "bundle" do { toBundle, name ->
            bundle(toBundle,name);
        }
        on.doubleValue "setAddress" do { address, prefix ->
            setImportDest(address,prefix);
        }
        on.flag "verbose" do { 
            verbose := true;
        }
        on.flag "global" do {
            global := true;
        }
    }
} 

method forArgs(args : List<String>) do(block) is confidential {
    var i := 2
    var ran := false
    def size = args.size

    def on = object {
        method option(name : String) shortHand(sh : String) do(block') {
            def arg = args.at(i)
            if((arg == "--{name}") || (arg == "-{sh}")) then {
                if(args.size == i) then {
                    Exception.raise "Missing value for option {name}"
                }

                i := i + 1
                block'.apply(args.at(i))
                ran := true
            }
        }

        method doubleValue(name: String) do (block'){
            def arg = args.at(i)
            if((arg == "--{name}")) then {
                if(args.size < (i+1)) then {
                    Exception.raise "Missing values for option {name}"
                }
                var arga := args.at(i+1)
                var argb := args.at(i+2)
                block'.apply(arga,argb)
                i := i + 2
                ran := true
            }

        }

        method option(name : String) do(block') {
            option(name) shortHand("") do(block')
        }

        method flag(name : String) shortHand(sh : String) do(block') {
            def arg = args.at(i)
            if((arg == "--{name}") || (arg == "-{sh}")) then {
                block'.apply
                ran := true
            }
        }

        method flag(name : String) do(block') {
            flag(name) shortHand("") do(block')
        }
    }
    while { i <= size } do {
        def arg = args.at(i)
        print(arg);
        ran := false
        block.apply(arg, on)
        if((arg.at(1) == "-") && ran.not) then {
            Exception.raise("Unrecognised argument {arg}")
        }

        i := i + 1
    }
}

method listInstalled{ 
    print("Installed packages:")
    if (io.exists("usr/lib/grace/packages/"))then{
        print("Going into the first call");
        var globalDir := "usr/lib/grace/packages"
        recurseDirectory(globalDir,"")
    }
    if (io.exists(sys.environ["HOME"]++"/grace/packages/"))then{
        var usrDir := sys.environ["HOME"]++"/grace/packages"//++ sys.environ["HOME"]++"/.grace/packages")
        recurseDirectory(usrDir," ")
    }
}

method recurseDirectory(path,padding){
    var pathContents := io.getdir(path);
    for (pathContents) do { p-> 
        if ((p != "..") && (p != "."))then{
            print(padding++p);
            if(io.getdir(path++"/"++p).size > 0)then{
                recurseDirectory((path++"/"++p),padding++"   ")
            }
        }
    }
}

method doGet(impAddress){
    imported := []
    fetchImports(impAddress)
    for(imported)do { im->
        write(im);
    }
    for(imported)do { im->
        compile(im);
    }
}

method setFile(fileAddress){
    if (fileAddress.substringFrom(fileAddress.size-5)to(fileAddress.size) == ".grace")then{
        var file := object{
            var address is public := fileAddress
            var data is public
        }
        curFile := file;
        return true;
    }
    return false;
}

method fetchImports(fileAddress) -> Boolean{
    if (setFile(fileAddress))then{
        print("File address {curFile.address}")
        if (setFileData(curFile) == false)then{
            print("Could not retrieve file data")
            return false
        }
        if (validateFile(curFile))then{
            print("Pushing {curFile.address} to imported")
            imported.push(curFile)
            parseFile(curFile)
            while{toProcess.size > 0}do{ 
                fetchImports(toProcess.pop)
            }
            return true
        }
    }
    else{ 
        if (fetchImports(fileAddress++".grace")==false)then{
            print("Could not locate file. Check file address.");
            return false
        }
    }
}

method setFileData(file) -> Boolean {
    if (file.address.substringFrom(1)to(4) == "http")then{
        var strippedUrl := file.address.substringFrom(1)to(4);
        if (findExisting(file.address) != false)then{
            var findData := findExisting(strippedUrl)
            if (findData > 0)then{
                file.data := findData
                return true;
            }
        }
        var req := curl.easy
        req.url := file.address
        print("Searching for import: "++file.address)
        print(" Connecting...")
        req.onReceive {d->
            print(" Response received")   
            file.data := d.decode("utf-8")
            return true;
        }
        req.perform
        if(req.responseCode != 200)then{
            print(" Could not locate import: Error {req.responseCode} for import {file.address}");
            return false
        }
    }
    if (findExisting(file.address) != false)then{
        print("Now searching in find existing");
        var findData := findExisting(file.address)
        if (findData != false)then{
            file.data := findData
            return true;
        }
    }
    return false;
}

method findExisting(fileName){
    if(io.exists(fileName))then{
        var open := io.open(fileName,"r");
        var ret := open.read;
        return ret; 
    }   
    if(io.exists("/usr/lib/grace/modules/"++fileName))then{
        return io.open("/usr/lib/grace/modules/"++fileName,"r").read 
    }   
    var homePath := sys.environ["HOME"]
    if(io.exists("{homePath}/.local/share/grace/modules/"++fileName))then{
        return io.open("{homePath}/.local/share/grace/modules/"++fileName,"r").read 
    }   
    if(io.exists("{sys.execPath}/../lib/minigrace/modules/"++fileName))then{
        return io.open("{sys.execPath}/../lib/minigrace/modules/"++fileName,"r").read 
    }   
    if(io.exists("{sys.execPath}/"++fileName))then{
        return io.open("{sys.execPath}/"++fileName,"r").read 
    }   
    if(io.exists("{getBuildPath()}/{fileName}"))then{
        print("YES IT DOES");
        return io.open("{getBuildPath()}/{fileName}","r").read 
    } 
    if(bundlePath != "")then{
        return io.open("{bundlePath}/{fileName}","r").read;
    }

    return false
}

method validateFile(file) -> Boolean{
    if ((file.data.size)>1)then{
        if(file.data[1]=="<")then{
            print("Not a valid grace file");
            return false
        }
    }
    return true
}

method write(file) -> Boolean{
    var usrDir := ""
    if(global) then { 
       usrDir := "usr/lib/grace/modules/"
    }
    else{
        usrDir := sys.environ["HOME"]++"/.local/lib/grace/modules/"
    }
    var fileDir := createDirectory(usrDir++file.address)
    var toWrite := io.open(fileDir[1]++fileDir[2], "w")
    toWrite.write(file.data);
    toWrite.close;
    
 }

method getBuildPath() -> String{
    var paths := sys.environ["PATH"]
    var buildPath := ""
    for (1..paths.size) do { t->
        if(paths.at(t) == ":")then{
            if(io.exists(buildPath++"/minigrace"))then{
               return buildPath; 
            }
            buildPath := ""
        } 
        else{
            buildPath :=  buildPath++paths[t]
        }
    }
    return buildPath;
}

method compile(file){
    var usrDir := ""
    if(global) then { 
       usrDir := "usr/lib/grace/modules/"
    }
    else{
        usrDir := sys.environ["HOME"]++"/.local/lib/grace/modules/"
    }
    var paths := sys.environ["PATH"]
    print("PATHS = "++paths)
    var buildPath := getBuildPath();
    io.system(buildPath++"/minigrace "++usrDir++file.address)
    return true;

}

method createDirectory(address) -> List{
    var dir:= []
    var nextPath := "" 
    var toMake := ""
    var count := 1
    print("Directory being evaluated {address}")
    while{count <= address.size} do {
        nextPath := nextPath ++ address[count]
        if(address[count] == "/")then{
            toMake := toMake ++ nextPath 
            nextPath := ""   
        }
        count := count+1;
    }
    print("Creating directory "++toMake);
    if (!io.exists(toMake))then{
        io.system("mkdir -p "++toMake)
        print("made directory {toMake}")
    }
    dir.push(toMake)
    dir.push(nextPath)
    return dir
}

method parseFile(file){
    var data := file.data
    var curPos := 1;
    var startPos := curPos
    while{curPos <= data.size}do{
        startPos := curPos
        while {(curPos <= data.size) && (data[curPos] != "\n")}do{
            curPos := curPos+1;
        }
        var line := data.substringFrom(startPos)to(curPos-1)
        if (!processLine(line))then{
            return
        }
        curPos := curPos + 1
    }
}

method processLine(line) -> Boolean {
    if (line.size > 1)then{
        if (line[1] == "#")then{ 
            return true
        }
        elseif ((line.size > 2) && (line.substringFrom(1)to(2) == "//"))then{
            return true
        }
        elseif ((line.size > 6) && (line.substringFrom(1)to(7) == "import "))then{
            parseImport(line.substringFrom(8)to(line.size));
            return true
        }
        elseif ((line.size > 7) && (line.substringFrom(1)to(8) == "dialect "))then{
            return true
        }
    }
    return false
}

method parseImport(line) -> Boolean{
    var curPos := 1
    var startPos := curPos
    var nextImport := ""
    curPos := skipWhiteSpace(line,curPos)
    curPos := curPos +1 
    startPos := curPos
    while{((curPos < line.size) && !((line[curPos+1] == "\"")||(line[curPos+1] == " ")))} do {
        curPos := curPos+1
    }
    nextImport := line.substringFrom(startPos)to(curPos)
    if (!imported.contains(nextImport))then{   
        print("next import = "++nextImport)
        toProcess.push(nextImport)
    }
}

method skipWhiteSpace(line, count) -> Number{ 
    var count2 := count
    while{(count2 <= line.size) && (line[count2] == "Could not retrieve ")}do{
        count2 := count2 + 1
    }
    count2
}

method displayHelp{
    print("Available options are:")
    print("-get [url] to fetch a package located at a given url")
    print("-list to display currenly installed packages")
}

method printMessage(message){
    if (verbose)then{
        print(message)
    }
}

//ToBundle is the name of the file that needs to be compiled. The compiler
//must be in the directory of the file to bundle in order for this to run 
//Name is the name of the package passed into the compiler
method bundle(toBundle,name){
    imported := []
    bundlePath := getContainingDirectory(toBundle);
    var newDir := createDirectory("{name}/")
    var newFileName := removeContainingDir(toBundle);
    var toOpen := io.open("{toBundle}","r");
    var openData := toOpen.read
    var toWrite := io.open("{name}/{newFileName}", "w")
    toWrite.write(openData);
    fetchImports(toBundle)
    print("Imported size = "++imported.size);
    while{imported.size > 0 }do{
        var curImport := imported.pop
        toWrite := io.open("{name}/{removeContainingDir(curImport.address)}","w")
        toWrite.write(curImport.data)   
    }
}

method setImportDest(address,prefix){
    var folder := io.getdir(address);
    for(folder) do { file-> 
        if (file.size > 4)then{
            if (file.substringFrom(file.size-5)to(file.size) == ".grace")then{
                var open := io.open(address++"/"++file,"r");
                var readFile := open.read;
                var toWrite := parseAndPrefix(readFile,address,prefix);
                open.close;

                io.system("rm "++address++"/"++file)
                var out := io.open(address++"/"++file,"w");
                for (toWrite) do { d-> 
                    out.write(d);
                }
            }
        }
    }
    //go through directory looking at each file with a .grace suffix.
    //process each file and look at each import statement
    //prepend 
}


method parseAndPrefix (readFile: String, address : String,  prefix : String){ 
    var curPos := 1;
    var startPos := curPos
    var outFile := [];
    for (0..readFile.size) do { t-> 
        while{curPos <= readFile.size}do{
            startPos := curPos
            while {(curPos <= readFile.size) && (readFile[curPos] != "\n")}do{
                curPos := curPos+1;
            }
            var data := readFile.substringFrom(startPos)to(curPos-1)
            var line := "";
            var count := 1;
            for (startPos..(curPos-1)) do { b-> 
                line := line++readFile[b];
            }
            print(line);
            if ((line.size > 6) && (line.substringFrom(1)to(7) == "import "))then{
                var pos := 1
                var start := pos
                var nextImport := ""
                pos := skipWhiteSpace(line,pos)
                pos:= pos +1 
                print("LINE = "++line);
                while{((pos < line.size) && !((line[pos] == "\"")||(line[pos] == " ")))} do {
                    pos := pos+1
                }
                pos:= pos+1;
                var remainder := removeExistingUrls(line.substringFrom(pos+1)to(line.size));
                line := "import \""++prefix++"/"++remainder;
                print(line);
                for (line) do{ d->
                    outFile.push(d);
                }
            }
            else{
                for (line) do{ d->
                    outFile.push(d);
                }
            }
            curPos := curPos + 1
        }
    }


    print ("OUTFILE "++outFile)
    return outFile;

}

method removeExistingUrls(importStatement : String) -> String{

    if (importStatement.size < 7)then{
        return importStatement;
    }
    if (importStatement.substringFrom(1)to(7) == "http://" )then{
        var lastIndex := 7;
        var curPos := 7;
        while {curPos <= importStatement.size}do{
            if (importStatement[curPos] == "/")then{
                lastIndex := curPos;
            }
            curPos := curPos +1;
        }
        var res := importStatement.substringFrom(lastIndex+1)to(importStatement.size);
        return res;
    }
    else{
        return importStatement;
    }


}

method removeContainingDir(st:String) -> String{
    var count := 1;
    var lastIndex := -1;
    while{count <= st.size}do{
        if (st[count] == "/") then{
            lastIndex := count;
        }
        count := count+1;
    }
    if(lastIndex == -1)then{
        return st;
    }
    return st.substringFrom(lastIndex+1)to(st.size); 
}

method getContainingDirectory(st : String) -> String{
    var count := 1;
    var last := -1;
    while {count <= st.size}do{
        if (st[count] == "/")then{
            last := count;
        }
        count := count+1;
    }
    if (last == -1)then{
        return getBuildPath()++"/";
    }
    return st.substringFrom(0)to(last-1);

}

