import "curl" as curl
import "sys" as sys

var req := curl.easy
req.url:= "http://homepages.ecs.vuw.ac.nz/~forretcrai/blah1.grace"
req.onReceive{d->
 print(d.decode("utf-8"))
}
req.perform
