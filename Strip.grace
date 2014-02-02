

print(stripLine("http://a"));


method stripLine(importStatement : String) -> String{

    print("Size = {importStatement.size}");
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