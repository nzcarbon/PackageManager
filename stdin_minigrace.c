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
static const char modulename[] = "stdin_minigrace";
Object module_StandardPrelude_init();
static char *originalSourceLines[] = {
  "var t := []",
  "t.asdh",
  NULL
};
Object meth_stdin_minigrace_t1(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 1, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "t");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_t = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for t (probably reflection error): got %i lists, expected 1.", nparts);
if (argcv && argcv[0] > 0)
  gracedie("too many arguments for <Identifier[t]>");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 1
  setline(1);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_t
  gc_frame_end(frame);
  return *var_t;
}
Object meth_stdin_minigrace_t_58__61_2(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 2, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "t:=");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_t = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for t:= (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var__var_assign_tmp = &(stackframe->slots[0]);
  *var__var_assign_tmp = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for <Identifier[t:=]>");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var__var_assign_tmp
  *var_t = *var__var_assign_tmp;
  if (*var__var_assign_tmp == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object module_stdin_minigrace_init() {
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
  setclassname(self, "Module<stdin_minigrace>");
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
  Object *var_t = &(stackframe->slots[1]);
  setframeelementname(stackframe, 1, "t");
// Begin line 2
  setline(2);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 1
  setline(1);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object array0 = alloc_BuiltinList();
  gc_pause();
  gc_unpause();
// compilenode returning array0
  *var_t = array0;
  if (array0 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 2
  setline(2);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1 = createclosure(1, "t");
setclosureframe(closure1, stackframe);
  addtoclosure(closure1, var_t);
  struct UserObject *uo1 = (struct UserObject*)self;
  uo1->data[1] = (Object)closure1;
  Method *meth_meth_stdin_minigrace_t1 = addmethod2pos(self, "t", &meth_stdin_minigrace_t1, 1);
int argcv_meth_stdin_minigrace_t1[] = {0};
meth_meth_stdin_minigrace_t1->type = alloc_MethodType(1, argcv_meth_stdin_minigrace_t1);
  meth_meth_stdin_minigrace_t1->definitionModule = modulename;
  meth_meth_stdin_minigrace_t1->definitionLine = 2;
// compilenode returning 
// Begin line 2
  setline(2);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2 = createclosure(1, "t:=");
setclosureframe(closure2, stackframe);
  addtoclosure(closure2, var_t);
  struct UserObject *uo2 = (struct UserObject*)self;
  uo2->data[2] = (Object)closure2;
  Method *meth_meth_stdin_minigrace_t_58__61_2 = addmethod2pos(self, "t:=", &meth_stdin_minigrace_t_58__61_2, 2);
int argcv_meth_stdin_minigrace_t_58__61_2[] = {1};
meth_meth_stdin_minigrace_t_58__61_2->type = alloc_MethodType(1, argcv_meth_stdin_minigrace_t_58__61_2);
  meth_meth_stdin_minigrace_t_58__61_2->definitionModule = modulename;
  meth_meth_stdin_minigrace_t_58__61_2->definitionLine = 2;
// compilenode returning 
// compilenode returning done
  int callframe4 = gc_frame_new();
// compilenode returning *var_t
  partcv[0] = 0;
  Object call5 = callmethod(*var_t, "asdh",
    1, partcv, params);
  gc_frame_end(callframe4);
// compilenode returning call5
// compilenode returning call5
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
  module_stdin_minigrace_init();
  gracelib_stats();
  return 0;
}
