import "io" as io

print(findExisting("wow.grace"))
method findExisting(fileName){
 if(io.exists(fileName))then{
  print("hello world")
  var open := io.open(fileName,"r");
  var ret := open.read;
  return ret;
 }
}

