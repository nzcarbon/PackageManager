import "io" as io
import "sys" as sys
import "curl" as curl

var req := curl.easy
req.url := "http://homepages.ecs.vuw.ac.nz/~forretcrai/blahasd.grace"
var t := ""
req.onReceive{ d -> 
  print(d.decode("utf-8"))
}
req.perform
print(t)

