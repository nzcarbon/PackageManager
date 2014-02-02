

print(removeContDir("./here/is/a/big/friggin/wowzer.grace"));
method removeContDir(st:String){
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
    else{
        var res := st.substringFrom(lastIndex+1)to(st.size);
        return res;
    }


}