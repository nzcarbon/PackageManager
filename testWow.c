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
static const char modulename[] = "testWow";
Object module_StandardPrelude_init();
static char *originalSourceLines[] = {
  "import \"io\" as io",
  "",
  "print(findExisting(\"wow.grace\"))",
  "method findExisting(fileName){",
  " if(io.exists(fileName))then{",
  "  print(\"hello world\")",
  "  var open := io.open(fileName,\"r\");",
  "  var ret := open.read;",
  "  return ret;",
  " }",
  "}",
  "",
  NULL
};
static Object strlit4;
static Object strlit7;
Object module_io;
Object module_io_init();
static Object strlit12;
Object meth_testWow_findExisting0(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 1, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(4, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "findExisting");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[1];
  Object *var_io = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for findExisting (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_fileName = &(stackframe->slots[0]);
  *var_fileName = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for findExisting");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_open = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "open");
  Object *var_ret = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "ret");
// Begin line 11
  setline(11);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 5
  setline(5);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe2 = gc_frame_new();
// compilenode returning *var_fileName
  gc_frame_newslot(*var_fileName);
// compilenode returning *var_io
  params[0] = *var_fileName;
  partcv[0] = 1;
  Object call3 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe2);
// compilenode returning call3
  Object if1 = done;
  if (istrue(call3)) {
// Begin line 7
  setline(7);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 6
  setline(6);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit4 == NULL) {
    strlit4 = alloc_String("hello world");
    gc_root(strlit4);
  }
// compilenode returning strlit4
  params[0] = strlit4;
  Object call5 = gracelib_print(NULL, 1,  params);
// compilenode returning call5
// Begin line 7
  setline(7);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe6 = gc_frame_new();
// compilenode returning *var_fileName
  gc_frame_newslot(*var_fileName);
  if (strlit7 == NULL) {
    strlit7 = alloc_String("r");
    gc_root(strlit7);
  }
// compilenode returning strlit7
  gc_frame_newslot(strlit7);
// compilenode returning *var_io
  params[0] = *var_fileName;
  params[1] = strlit7;
  partcv[0] = 2;
  Object call8 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe6);
// compilenode returning call8
  *var_open = call8;
  if (call8 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 8
  setline(8);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe9 = gc_frame_new();
// compilenode returning *var_open
  partcv[0] = 0;
  Object call10 = callmethod(*var_open, "read",
    1, partcv, params);
  gc_frame_end(callframe9);
// compilenode returning call10
// compilenode returning call10
  *var_ret = call10;
  if (call10 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 9
  setline(9);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_ret
  return *var_ret;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if1 = undefined;
  } else {
  }
// compilenode returning if1
  gc_frame_end(frame);
  return if1;
}
Object module_testWow_init() {
  int flags = 0;
  int frame = gc_frame_new();
  Object self = alloc_obj2(3, 3);
  self->class->definitionModule = modulename;
  gc_root(self);
  prelude = module_StandardPrelude_init();
  adddatum2(self, prelude, 0);
  addmethod2(self, "outer", &grace_userobj_outer);
  setline(1);
  setmodule(modulename);
  setsource(originalSourceLines);
  setclassname(self, "Module<testWow>");
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
  struct StackFrameObject *stackframe = alloc_StackFrame(3, NULL);
  gc_root((Object)stackframe);
  pushstackframe(stackframe, "module scope");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  Object params[1];
  int partcv[1];
Object *var_io = alloc_var();
// Begin line 4
  setline(4);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure0 = createclosure(1, "findExisting");
setclosureframe(closure0, stackframe);
  addtoclosure(closure0, var_io);
  struct UserObject *uo0 = (struct UserObject*)self;
  uo0->data[1] = (Object)closure0;
  Method *meth_meth_testWow_findExisting0 = addmethod2pos(self, "findExisting", &meth_testWow_findExisting0, 1);
int argcv_meth_testWow_findExisting0[] = {1};
meth_meth_testWow_findExisting0->type = alloc_MethodType(1, argcv_meth_testWow_findExisting0);
  meth_meth_testWow_findExisting0->definitionModule = modulename;
  meth_meth_testWow_findExisting0->definitionLine = 4;
// compilenode returning 
// Begin line 1
  setline(1);
  setmodule(modulename);
  setsource(originalSourceLines);
// Import of io as io
  if (module_io == NULL)
    module_io = module_io_init();
  *var_io = module_io;
// compilenode returning done
// compilenode returning 
// Begin line 4
  setline(4);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 3
  setline(3);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe11 = gc_frame_new();
  if (strlit12 == NULL) {
    strlit12 = alloc_String("wow.grace");
    gc_root(strlit12);
  }
// compilenode returning strlit12
  gc_frame_newslot(strlit12);
  params[0] = strlit12;
  partcv[0] = 1;
  Object call13 = callmethodflags(self, "findExisting", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe11);
// compilenode returning call13
  params[0] = call13;
  Object call14 = gracelib_print(NULL, 1,  params);
// compilenode returning call14
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
  module_testWow_init();
  gracelib_stats();
  return 0;
}
