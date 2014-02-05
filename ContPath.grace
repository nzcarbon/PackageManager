import "sys" as sys
import "io" as io

print(getBaseUrl("http://wow/hello/good/morning.grace"));
print(getBaseUrl("http://"));

method getContPath(st : String) -> String{

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
    print({buildPath});
    return buildPath;
}


method getBaseUrl(importStatement : String) -> String{

    if (importStatement.size < 7)then{
        return importStatement
    }
    if (importStatement.substringFrom(1)to(7) == "http://" )then{
        var lastIndex := 7
        var curPos := 7
        while {curPos <= importStatement.size}do{
            if (importStatement[curPos] == "/")then{
                lastIndex := curPos
            }
            curPos := curPos +1
        }
        var res := importStatement.substringFrom(1)to(lastIndex)
        return res
    }
    else{
        return importStatement
    }

}