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
static const char modulename[] = "test";
Object module_StandardPrelude_init();
static char *originalSourceLines[] = {
  "var t := \"hello\"",
  "var d := \"world\"",
  "var ob := object{ ",
  " var ad is public := t",
  " var data is public := d",
  "}",
  "print(ob.data)",
  "print(ob.ad)",
  NULL
};
Object reader_test_outer_9(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject*)self;
  return uo->data[0];
}
Object reader_test_ad_10(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  return uo->data[1];
}
Object writer_test_ad_10(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  uo->data[1] = args[0];
  return done;
}
Object reader_test_data_11(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  return uo->data[2];
}
Object writer_test_data_11(Object self, int nparams, int *argcv, Object* args, int flags) {
  struct UserObject *uo = (struct UserObject *)self;
  uo->data[2] = args[0];
  return done;
}
static Object strlit0;
static Object strlit4;
static ClassData objclass8;
Object meth_test_t1(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
Object meth_test_t_58__61_2(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
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
Object meth_test_d5(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 3, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "d");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_d = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for d (probably reflection error): got %i lists, expected 1.", nparts);
if (argcv && argcv[0] > 0)
  gracedie("too many arguments for <Identifier[d]>");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 2
  setline(2);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_d
  gc_frame_end(frame);
  return *var_d;
}
Object meth_test_d_58__61_6(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 4, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "d:=");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_d = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for d:= (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var__var_assign_tmp = &(stackframe->slots[0]);
  *var__var_assign_tmp = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for <Identifier[d:=]>");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var__var_assign_tmp
  *var_d = *var__var_assign_tmp;
  if (*var__var_assign_tmp == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_test_ob12(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 5, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "ob");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_ob = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for ob (probably reflection error): got %i lists, expected 1.", nparts);
if (argcv && argcv[0] > 0)
  gracedie("too many arguments for <Identifier[ob]>");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 3
  setline(3);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_ob
  gc_frame_end(frame);
  return *var_ob;
}
Object meth_test_ob_58__61_13(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 6, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(2, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "ob:=");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[1];
  int partcv[1];
  Object *var_ob = getfromclosure(closure, 0);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for ob:= (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var__var_assign_tmp = &(stackframe->slots[0]);
  *var__var_assign_tmp = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for <Identifier[ob:=]>");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var__var_assign_tmp
  *var_ob = *var__var_assign_tmp;
  if (*var__var_assign_tmp == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object module_test_init() {
  int flags = 0;
  int frame = gc_frame_new();
  Object self = alloc_obj2(8, 8);
  self->class->definitionModule = modulename;
  gc_root(self);
  prelude = module_StandardPrelude_init();
  adddatum2(self, prelude, 0);
  addmethod2(self, "outer", &grace_userobj_outer);
  setline(1);
  setmodule(modulename);
  setsource(originalSourceLines);
  setclassname(self, "Module<test>");
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
  struct StackFrameObject *stackframe = alloc_StackFrame(8, NULL);
  gc_root((Object)stackframe);
  pushstackframe(stackframe, "module scope");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  Object params[1];
  int partcv[1];
  Object *var_t = &(stackframe->slots[1]);
  setframeelementname(stackframe, 1, "t");
  Object *var_d = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "d");
  Object *var_ob = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "ob");
// Begin line 2
  setline(2);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 1
  setline(1);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit0 == NULL) {
    strlit0 = alloc_String("hello");
    gc_root(strlit0);
  }
// compilenode returning strlit0
  *var_t = strlit0;
  if (strlit0 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 8
  setline(8);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure1 = createclosure(1, "t");
setclosureframe(closure1, stackframe);
  addtoclosure(closure1, var_t);
  struct UserObject *uo1 = (struct UserObject*)self;
  uo1->data[1] = (Object)closure1;
  Method *meth_meth_test_t1 = addmethod2pos(self, "t", &meth_test_t1, 1);
int argcv_meth_test_t1[] = {0};
meth_meth_test_t1->type = alloc_MethodType(1, argcv_meth_test_t1);
  meth_meth_test_t1->definitionModule = modulename;
  meth_meth_test_t1->definitionLine = 8;
// compilenode returning 
// Begin line 8
  setline(8);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure2 = createclosure(1, "t:=");
setclosureframe(closure2, stackframe);
  addtoclosure(closure2, var_t);
  struct UserObject *uo2 = (struct UserObject*)self;
  uo2->data[2] = (Object)closure2;
  Method *meth_meth_test_t_58__61_2 = addmethod2pos(self, "t:=", &meth_test_t_58__61_2, 2);
int argcv_meth_test_t_58__61_2[] = {1};
meth_meth_test_t_58__61_2->type = alloc_MethodType(1, argcv_meth_test_t_58__61_2);
  meth_meth_test_t_58__61_2->definitionModule = modulename;
  meth_meth_test_t_58__61_2->definitionLine = 8;
// compilenode returning 
// compilenode returning done
// Begin line 3
  setline(3);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 2
  setline(2);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit4 == NULL) {
    strlit4 = alloc_String("world");
    gc_root(strlit4);
  }
// compilenode returning strlit4
  *var_d = strlit4;
  if (strlit4 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 8
  setline(8);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure5 = createclosure(1, "d");
setclosureframe(closure5, stackframe);
  addtoclosure(closure5, var_d);
  struct UserObject *uo5 = (struct UserObject*)self;
  uo5->data[3] = (Object)closure5;
  Method *meth_meth_test_d5 = addmethod2pos(self, "d", &meth_test_d5, 3);
int argcv_meth_test_d5[] = {0};
meth_meth_test_d5->type = alloc_MethodType(1, argcv_meth_test_d5);
  meth_meth_test_d5->definitionModule = modulename;
  meth_meth_test_d5->definitionLine = 8;
// compilenode returning 
// Begin line 8
  setline(8);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure6 = createclosure(1, "d:=");
setclosureframe(closure6, stackframe);
  addtoclosure(closure6, var_d);
  struct UserObject *uo6 = (struct UserObject*)self;
  uo6->data[4] = (Object)closure6;
  Method *meth_meth_test_d_58__61_6 = addmethod2pos(self, "d:=", &meth_test_d_58__61_6, 4);
int argcv_meth_test_d_58__61_6[] = {1};
meth_meth_test_d_58__61_6->type = alloc_MethodType(1, argcv_meth_test_d_58__61_6);
  meth_meth_test_d_58__61_6->definitionModule = modulename;
  meth_meth_test_d_58__61_6->definitionLine = 8;
// compilenode returning 
// compilenode returning done
// Begin line 3
  setline(3);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object inheritingObject8 = inheritingObject;
  if (isTailObject) {
    isTailObject = 0;
    inheritingObject = NULL;
  }
  Object obj8 = alloc_userobj2(4,4, objclass8);
  gc_frame_newslot(obj8);
// OBJECT OUTER DEC outer
  adddatum2(obj8, self, 0);
  addmethodreal(obj8,"outer", &reader_test_outer_9);
  Object oldself8 = self;
  struct StackFrameObject *oldstackframe8 = stackframe;
  stackframe = alloc_StackFrame(1, oldstackframe8);
  gc_frame_newslot((Object)stackframe);
  self = obj8;
  Object *oldselfslot8 = selfslot;
  selfslot = &stackframe->slots[0];
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  Object thisouter8 = (*(struct UserObject *)self).data[0], lowerouter8 = thisouter8;
  if (inheritingObject8) {
    struct UserObject *inho8 = (struct UserObject *)inheritingObject8;
    while (inho8->super != GraceDefaultObject) inho8 = (struct UserObject *)inho8->super;
    inho8->super = obj8;
    self = inheritingObject8;
    *selfslot = self;
    lowerouter8 = (*(struct UserObject *)self).data[0];
    (*(struct UserObject *)self).data[0] = thisouter8;
  }
if (objclass8 == NULL) {
  Method *reader10 = addmethodrealflags(obj8, "ad",&reader_test_ad_10, 0);
  Method *writer10 = addmethodrealflags(obj8, "ad:=",&writer_test_ad_10, 0);
  reader10->definitionModule = modulename;
  writer10->definitionModule = modulename;
  reader10->definitionLine = 5;
  writer10->definitionLine = 5;
}
obj8->flags |= OFLAG_MUTABLE;
adddatum2(obj8, alloc_Undefined(), 1);
if (objclass8 == NULL) {
  Method *reader11 = addmethodrealflags(obj8, "data",&reader_test_data_11, 0);
  Method *writer11 = addmethodrealflags(obj8, "data:=",&writer_test_data_11, 0);
  reader11->definitionModule = modulename;
  writer11->definitionModule = modulename;
  reader11->definitionLine = 6;
  writer11->definitionLine = 6;
}
obj8->flags |= OFLAG_MUTABLE;
adddatum2(obj8, alloc_Undefined(), 2);
  sourceObject = obj8;
// Begin line 4
  setline(4);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_t
  adddatum2(obj8, *var_t, 1);
  sourceObject = obj8;
// Begin line 5
  setline(5);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_d
  adddatum2(obj8, *var_d, 2);
objclass8 = obj8->class;
  objclass8->definitionModule = modulename;
  objclass8->definitionLine = 3;
  (*(struct UserObject *)self).data[0] = lowerouter8;
  self = oldself8;
  selfslot = oldselfslot8;
  stackframe = oldstackframe8;
// compilenode returning obj8
  *var_ob = obj8;
  if (obj8 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// Begin line 8
  setline(8);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure12 = createclosure(1, "ob");
setclosureframe(closure12, stackframe);
  addtoclosure(closure12, var_ob);
  struct UserObject *uo12 = (struct UserObject*)self;
  uo12->data[5] = (Object)closure12;
  Method *meth_meth_test_ob12 = addmethod2pos(self, "ob", &meth_test_ob12, 5);
int argcv_meth_test_ob12[] = {0};
meth_meth_test_ob12->type = alloc_MethodType(1, argcv_meth_test_ob12);
  meth_meth_test_ob12->definitionModule = modulename;
  meth_meth_test_ob12->definitionLine = 8;
// compilenode returning 
// Begin line 8
  setline(8);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure13 = createclosure(1, "ob:=");
setclosureframe(closure13, stackframe);
  addtoclosure(closure13, var_ob);
  struct UserObject *uo13 = (struct UserObject*)self;
  uo13->data[6] = (Object)closure13;
  Method *meth_meth_test_ob_58__61_13 = addmethod2pos(self, "ob:=", &meth_test_ob_58__61_13, 6);
int argcv_meth_test_ob_58__61_13[] = {1};
meth_meth_test_ob_58__61_13->type = alloc_MethodType(1, argcv_meth_test_ob_58__61_13);
  meth_meth_test_ob_58__61_13->definitionModule = modulename;
  meth_meth_test_ob_58__61_13->definitionLine = 8;
// compilenode returning 
// compilenode returning done
// Begin line 7
  setline(7);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe15 = gc_frame_new();
// compilenode returning *var_ob
  partcv[0] = 0;
  Object call16 = callmethod(*var_ob, "data",
    1, partcv, params);
  gc_frame_end(callframe15);
// compilenode returning call16
// compilenode returning call16
  params[0] = call16;
  Object call17 = gracelib_print(NULL, 1,  params);
// compilenode returning call17
// Begin line 8
  setline(8);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe18 = gc_frame_new();
// compilenode returning *var_ob
  partcv[0] = 0;
  Object call19 = callmethod(*var_ob, "ad",
    1, partcv, params);
  gc_frame_end(callframe18);
// compilenode returning call19
// compilenode returning call19
  params[0] = call19;
  Object call20 = gracelib_print(NULL, 1,  params);
// compilenode returning call20
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
  module_test_init();
  gracelib_stats();
  return 0;
}
