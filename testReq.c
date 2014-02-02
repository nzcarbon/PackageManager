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
static const char modulename[] = "testReq";
Object module_StandardPrelude_init();
static char *originalSourceLines[] = {
  "import \"curl\" as curl",
  "import \"sys\" as sys",
  "",
  "var req := curl.easy",
  "req.url:= \"http://homepages.ecs.vuw.ac.nz/~forretcrai/blah1.grace\"",
  "req.onReceive{d->",
  " print(d.decode(\"utf-8\"))",
  "}",
  "req.perform",
  NULL
};
Object module_curl;
Object module_curl_init();
Object module_sys;
Object module_sys_init();
static Object strlit6;
static Object strlit12;
Object meth_testReq_req2(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 1, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "req");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_req = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for req (probably reflection error): got %i lists, expected 1.", nparts);
if (argcv && argcv[0] > 0)
  gracedie("too many arguments for <Identifier[req]>");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var_req
  gc_frame_end(frame);
  return *var_req;
}
Object meth_testReq_req_58__61_3(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 2, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "req:=");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_req = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for req:= (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var__var_assign_tmp = &(stackframe->slots[0]);
  *var__var_assign_tmp = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for <Identifier[req:=]>");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var__var_assign_tmp
  *var_req = *var__var_assign_tmp;
  if (*var__var_assign_tmp == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_testReq__apply10(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  Object *var_d = &(stackframe->slots[0]);
  *var_d = args[curarg];
  curarg++;
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 8
  setline(8);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 7
  setline(7);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe11 = gc_frame_new();
  if (strlit12 == NULL) {
    strlit12 = alloc_String("utf-8");
    gc_root(strlit12);
  }
// compilenode returning strlit12
  gc_frame_newslot(strlit12);
// compilenode returning *var_d
  params[0] = strlit12;
  partcv[0] = 1;
  Object call13 = callmethod(*var_d, "decode",
    1, partcv, params);
  gc_frame_end(callframe11);
// compilenode returning call13
  params[0] = call13;
  Object call14 = gracelib_print(NULL, 1,  params);
// compilenode returning call14
  gc_frame_end(frame);
  return call14;
}
Object module_testReq_init() {
  int flags = 0;
  int frame = gc_frame_new();
  Object self = alloc_obj2(4, 4);
  self->class->definitionModule = modulename;
  gc_root(self);
  prelude = module_StandardPrelude_init();
  adddatum2(self, prelude, 0);
  addmethod2(self, "outer", &grace_userobj_outer);
  setline(1);
  setmodule(modulename);
  setsource(originalSourceLines);
  setclassname(self, "Module<testReq>");
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
  struct StackFrameObject *stackframe = alloc_StackFrame(4, NULL);
  gc_root((Object)stackframe);
  pushstackframe(stackframe, "module scope");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  Object params[1];
  int partcv[1];
Object *var_curl = alloc_var();
Object *var_sys = alloc_var();
  Object *var_req = &(stackframe->slots[1]);
  setframeelementname(stackframe, 1, "req");
// Import of curl as curl
  if (module_curl == NULL)
    module_curl = dlmodule("curl");
  *var_curl = module_curl;
// compilenode returning done
// Begin line 2
  setline(2);
  setmodule(modulename);
  setsource(originalSourceLines);
// Import of sys as sys
  if (module_sys == NULL)
    module_sys = module_sys_init();
  *var_sys = module_sys;
// compilenode returning done
// compilenode returning 
// Begin line 5
  setline(5);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 4
  setline(4);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe0 = gc_frame_new();
// compilenode returning *var_curl
  partcv[0] = 0;
  Object call1 = callmethod(*var_curl, "easy",
    1, partcv, params);
  gc_frame_end(callframe0);
// compilenode returning call1
// compilenode returning call1
  *var_req = call1;
  if (call1 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
  block_savedest(self);
  Object closure2 = createclosure(1, "req");
setclosureframe(closure2, stackframe);
  addtoclosure(closure2, var_req);
  struct UserObject *uo2 = (struct UserObject*)self;
  uo2->data[1] = (Object)closure2;
  Method *meth_meth_testReq_req2 = addmethod2pos(self, "req", &meth_testReq_req2, 1);
int argcv_meth_testReq_req2[] = {0};
meth_meth_testReq_req2->type = alloc_MethodType(1, argcv_meth_testReq_req2);
  meth_meth_testReq_req2->definitionModule = modulename;
  meth_meth_testReq_req2->definitionLine = 4;
// compilenode returning 
  block_savedest(self);
  Object closure3 = createclosure(1, "req:=");
setclosureframe(closure3, stackframe);
  addtoclosure(closure3, var_req);
  struct UserObject *uo3 = (struct UserObject*)self;
  uo3->data[2] = (Object)closure3;
  Method *meth_meth_testReq_req_58__61_3 = addmethod2pos(self, "req:=", &meth_testReq_req_58__61_3, 2);
int argcv_meth_testReq_req_58__61_3[] = {1};
meth_meth_testReq_req_58__61_3->type = alloc_MethodType(1, argcv_meth_testReq_req_58__61_3);
  meth_meth_testReq_req_58__61_3->definitionModule = modulename;
  meth_meth_testReq_req_58__61_3->definitionLine = 4;
// compilenode returning 
// compilenode returning done
// Begin line 6
  setline(6);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 4
  setline(4);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe5 = gc_frame_new();
// Begin line 5
  setline(5);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit6 == NULL) {
    strlit6 = alloc_String("http://homepages.ecs.vuw.ac.nz/~forretcrai/blah1.grace");
    gc_root(strlit6);
  }
// compilenode returning strlit6
  gc_frame_newslot(strlit6);
// compilenode returning *var_req
  params[0] = strlit6;
  partcv[0] = 1;
  Object call7 = callmethod(*var_req, "url:=",
    1, partcv, params);
  gc_frame_end(callframe5);
// compilenode returning call7
// compilenode returning done
// Begin line 9
  setline(9);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe8 = gc_frame_new();
  Object block9 = alloc_Block(NULL, NULL, "testReq", 9);
  gc_frame_newslot(block9);
  block_savedest(block9);
  Object closure10 = createclosure(1, "_apply");
setclosureframe(closure10, stackframe);
  addtoclosure(closure10, selfslot);
  struct UserObject *uo10 = (struct UserObject*)block9;
  uo10->data[0] = (Object)closure10;
  Method *meth_meth_testReq__apply10 = addmethod2pos(block9, "_apply", &meth_testReq__apply10, 0);
int argcv_meth_testReq__apply10[] = {1};
meth_meth_testReq__apply10->type = alloc_MethodType(1, argcv_meth_testReq__apply10);
  meth_meth_testReq__apply10->definitionModule = modulename;
  meth_meth_testReq__apply10->definitionLine = 4;
// compilenode returning block9
  gc_frame_newslot(block9);
// Begin line 6
  setline(6);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_req
  params[0] = block9;
  partcv[0] = 1;
  Object call16 = callmethod(*var_req, "onReceive",
    1, partcv, params);
  gc_frame_end(callframe8);
// compilenode returning call16
// Begin line 9
  setline(9);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe17 = gc_frame_new();
// compilenode returning *var_req
  partcv[0] = 0;
  Object call18 = callmethod(*var_req, "perform",
    1, partcv, params);
  gc_frame_end(callframe17);
// compilenode returning call18
// compilenode returning call18
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
  module_testReq_init();
  gracelib_stats();
  return 0;
}
