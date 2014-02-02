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
static const char modulename[] = "fileWriteTest";
Object module_StandardPrelude_init();
static char *originalSourceLines[] = {
  "import \"io\" as io",
  "import \"sys\" as sys",
  "",
  "bundle",
  "",
  "method bundle{",
  " var toOpen := io.open(\"wow.grace\",\"r\")",
  " var toWrite := io.open(\"testWrite.grace\",\"w\")",
  " var data := toOpen.read;",
  " toWrite.write(data);",
  "}",
  NULL
};
static Object strlit2;
static Object strlit3;
static Object strlit6;
static Object strlit7;
Object module_io;
Object module_io_init();
Object module_sys;
Object module_sys_init();
Object meth_fileWriteTest_bundle0(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 1, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(4, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "bundle");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[2];
  int partcv[1];
  Object *var_io = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for bundle (probably reflection error): got %i lists, expected 1.", nparts);
if (argcv && argcv[0] > 0)
  gracedie("too many arguments for bundle");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_toOpen = &(stackframe->slots[1]);
  setframeelementname(stackframe, 1, "toOpen");
  Object *var_toWrite = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "toWrite");
  Object *var_data = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "data");
// Begin line 8
  setline(8);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe1 = gc_frame_new();
// Begin line 7
  setline(7);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit2 == NULL) {
    strlit2 = alloc_String("wow.grace");
    gc_root(strlit2);
  }
// compilenode returning strlit2
  gc_frame_newslot(strlit2);
  if (strlit3 == NULL) {
    strlit3 = alloc_String("r");
    gc_root(strlit3);
  }
// compilenode returning strlit3
  gc_frame_newslot(strlit3);
// compilenode returning *var_io
  params[0] = strlit2;
  params[1] = strlit3;
  partcv[0] = 2;
  Object call4 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe1);
// compilenode returning call4
  *var_toOpen = call4;
  if (call4 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 9
  setline(9);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe5 = gc_frame_new();
// Begin line 8
  setline(8);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit6 == NULL) {
    strlit6 = alloc_String("testWrite.grace");
    gc_root(strlit6);
  }
// compilenode returning strlit6
  gc_frame_newslot(strlit6);
  if (strlit7 == NULL) {
    strlit7 = alloc_String("w");
    gc_root(strlit7);
  }
// compilenode returning strlit7
  gc_frame_newslot(strlit7);
// compilenode returning *var_io
  params[0] = strlit6;
  params[1] = strlit7;
  partcv[0] = 2;
  Object call8 = callmethod(*var_io, "open",
    1, partcv, params);
  gc_frame_end(callframe5);
// compilenode returning call8
  *var_toWrite = call8;
  if (call8 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 9
  setline(9);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe9 = gc_frame_new();
// compilenode returning *var_toOpen
  partcv[0] = 0;
  Object call10 = callmethod(*var_toOpen, "read",
    1, partcv, params);
  gc_frame_end(callframe9);
// compilenode returning call10
// compilenode returning call10
  *var_data = call10;
  if (call10 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 10
  setline(10);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe11 = gc_frame_new();
// compilenode returning *var_data
  gc_frame_newslot(*var_data);
// compilenode returning *var_toWrite
  params[0] = *var_data;
  partcv[0] = 1;
  Object call12 = callmethod(*var_toWrite, "write",
    1, partcv, params);
  gc_frame_end(callframe11);
// compilenode returning call12
  gc_frame_end(frame);
  return call12;
}
Object module_fileWriteTest_init() {
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
  setclassname(self, "Module<fileWriteTest>");
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
Object *var_sys = alloc_var();
// Begin line 6
  setline(6);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure0 = createclosure(1, "bundle");
setclosureframe(closure0, stackframe);
  addtoclosure(closure0, var_io);
  struct UserObject *uo0 = (struct UserObject*)self;
  uo0->data[1] = (Object)closure0;
  Method *meth_meth_fileWriteTest_bundle0 = addmethod2pos(self, "bundle", &meth_fileWriteTest_bundle0, 1);
int argcv_meth_fileWriteTest_bundle0[] = {0};
meth_meth_fileWriteTest_bundle0->type = alloc_MethodType(1, argcv_meth_fileWriteTest_bundle0);
  meth_meth_fileWriteTest_bundle0->definitionModule = modulename;
  meth_meth_fileWriteTest_bundle0->definitionLine = 6;
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
// Begin line 4
  setline(4);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe13 = gc_frame_new();
  partcv[0] = 0;
  Object call14 = callmethodflags(self, "bundle", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe13);
// compilenode returning call14
// Begin line 6
  setline(6);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
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
  module_fileWriteTest_init();
  gracelib_stats();
  return 0;
}
