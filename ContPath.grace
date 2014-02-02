import "sys" as sys
import "io" as io

print(getContPath("wow/hello/good/morning.grace"));
print(getContPath("wow"));

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