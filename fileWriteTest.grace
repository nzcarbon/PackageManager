import "io" as io
import "sys" as sys

bundle

method bundle{
 var toOpen := io.open("wow.grace","r")
 var toWrite := io.open("testWrite.grace","w")
 var data := toOpen.read;
 toWrite.write(data);
}
