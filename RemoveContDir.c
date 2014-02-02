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
static const char modulename[] = "RemoveContDir";
Object module_StandardPrelude_init();
static char *originalSourceLines[] = {
  "",
  "",
  "print(removeContDir(\"./here/is/a/big/friggin/wowzer.grace\"));",
  "method removeContDir(st:String){",
  "    var count := 1;",
  "    var lastIndex := -1;",
  "    while{count <= st.size}do{",
  "        if (st[count] == \"/\") then{",
  "            lastIndex := count;",
  "        }",
  "        count := count+1;",
  "    }",
  "",
  "    if(lastIndex == -1)then{",
  "        return st;",
  "    }",
  "    else{",
  "        var res := st.substringFrom(lastIndex+1)to(st.size);",
  "        return res;",
  "    }",
  "",
  "",
  "}",
  NULL
};
static Object strlit19;
static Object strlit46;
Object meth_RemoveContDir__apply7(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_count = getfromclosure(closure, 0);
  Object *var_st = getfromclosure(closure, 1);
  Object self = *(getfromclosure(closure, 2));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var_count
  int op_slot_left_8 = gc_frame_newslot(*var_count);
  int callframe9 = gc_frame_new();
// compilenode returning *var_st
  partcv[0] = 0;
  Object call10 = callmethod(*var_st, "size",
    1, partcv, params);
  gc_frame_end(callframe9);
// compilenode returning call10
// compilenode returning call10
  int op_slot_right_8 = gc_frame_newslot(call10);
  params[0] = call10;
  partcv[0] = 1;
  Object opresult12 = callmethod(*var_count, "<=", 1, partcv, params);
// compilenode returning opresult12
  gc_frame_end(frame);
  return opresult12;
}
Object meth_RemoveContDir__apply15(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)realself;
  Object closure = getdatum((Object)uo, 0, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(1, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "_apply");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[1];
  int partcv[1];
  Object *var_st = getfromclosure(closure, 0);
  Object *var_count = getfromclosure(closure, 1);
  Object *var_lastIndex = getfromclosure(closure, 2);
  Object self = *(getfromclosure(closure, 3));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 11
  setline(11);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 8
  setline(8);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_st
// compilenode returning *var_count
  params[0] = *var_count;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres18 = callmethod(*var_st, "[]", 1, partcv, params);
// compilenode returning idxres18
  int op_slot_left_17 = gc_frame_newslot(idxres18);
  if (strlit19 == NULL) {
    strlit19 = alloc_String("/");
    gc_root(strlit19);
  }
// compilenode returning strlit19
  int op_slot_right_17 = gc_frame_newslot(strlit19);
  params[0] = strlit19;
  partcv[0] = 1;
  Object opresult21 = callmethod(idxres18, "==", 1, partcv, params);
// compilenode returning opresult21
  Object if16 = done;
  if (istrue(opresult21)) {
// Begin line 9
  setline(9);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  *var_lastIndex = *var_count;
  if (*var_count == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if16 = done;
  } else {
  }
// compilenode returning if16
// Begin line 11
  setline(11);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  int op_slot_left_23 = gc_frame_newslot(*var_count);
  Object num24 = alloc_Float64(1);
// compilenode returning num24
  int op_slot_right_23 = gc_frame_newslot(num24);
  params[0] = num24;
  partcv[0] = 1;
  Object sum26 = callmethod(*var_count, "+", 1, partcv, params);
// compilenode returning sum26
  *var_count = sum26;
  if (sum26 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_RemoveContDir_removeContDir0(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct StackFrameObject *stackframe = alloc_StackFrame(5, NULL);
  pushclosure(NULL);
  pushstackframe(stackframe, "removeContDir");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  if (nparts > 0 && argcv[0] < 1)
    gracedie("insufficient arguments to method");
  Object params[2];
  int partcv[2];
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for removeContDir (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_st = &(stackframe->slots[0]);
  *var_st = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for removeContDir");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_count = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "count");
  Object *var_lastIndex = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "lastIndex");
  Object *var_res = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "res");
// Begin line 5
  setline(5);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1 = alloc_Float64(1);
// compilenode returning num1
  *var_count = num1;
  if (num1 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 6
  setline(6);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe2 = gc_frame_new();
  Object num3 = alloc_Float64(1);
// compilenode returning num3
  partcv[0] = 0;
  Object call4 = callmethod(num3, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe2);
// compilenode returning call4
  *var_lastIndex = call4;
  if (call4 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 14
  setline(14);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe5 = gc_frame_new();
// Begin line 7
  setline(7);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block6 = alloc_Block(NULL, NULL, "RemoveContDir", 7);
  gc_frame_newslot(block6);
  block_savedest(block6);
  Object closure7 = createclosure(3, "_apply");
setclosureframe(closure7, stackframe);
  addtoclosure(closure7, var_count);
  addtoclosure(closure7, var_st);
  addtoclosure(closure7, selfslot);
  struct UserObject *uo7 = (struct UserObject*)block6;
  uo7->data[0] = (Object)closure7;
  Method *meth_meth_RemoveContDir__apply7 = addmethod2pos(block6, "_apply", &meth_RemoveContDir__apply7, 0);
int argcv_meth_RemoveContDir__apply7[] = {0};
meth_meth_RemoveContDir__apply7->type = alloc_MethodType(1, argcv_meth_RemoveContDir__apply7);
  meth_meth_RemoveContDir__apply7->definitionModule = modulename;
  meth_meth_RemoveContDir__apply7->definitionLine = 19;
// compilenode returning block6
  gc_frame_newslot(block6);
// Begin line 14
  setline(14);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block14 = alloc_Block(NULL, NULL, "RemoveContDir", 14);
  gc_frame_newslot(block14);
  block_savedest(block14);
  Object closure15 = createclosure(4, "_apply");
setclosureframe(closure15, stackframe);
  addtoclosure(closure15, var_st);
  addtoclosure(closure15, var_count);
  addtoclosure(closure15, var_lastIndex);
  addtoclosure(closure15, selfslot);
  struct UserObject *uo15 = (struct UserObject*)block14;
  uo15->data[0] = (Object)closure15;
  Method *meth_meth_RemoveContDir__apply15 = addmethod2pos(block14, "_apply", &meth_RemoveContDir__apply15, 0);
int argcv_meth_RemoveContDir__apply15[] = {0};
meth_meth_RemoveContDir__apply15->type = alloc_MethodType(1, argcv_meth_RemoveContDir__apply15);
  meth_meth_RemoveContDir__apply15->definitionModule = modulename;
  meth_meth_RemoveContDir__apply15->definitionLine = 7;
// compilenode returning block14
  gc_frame_newslot(block14);
  params[0] = block6;
  params[1] = block14;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call29 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe5);
// compilenode returning call29
// Begin line 23
  setline(23);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 14
  setline(14);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_lastIndex
  int op_slot_left_31 = gc_frame_newslot(*var_lastIndex);
  int callframe32 = gc_frame_new();
  Object num33 = alloc_Float64(1);
// compilenode returning num33
  partcv[0] = 0;
  Object call34 = callmethod(num33, "prefix-",
    1, partcv, params);
  gc_frame_end(callframe32);
// compilenode returning call34
  int op_slot_right_31 = gc_frame_newslot(call34);
  params[0] = call34;
  partcv[0] = 1;
  Object opresult36 = callmethod(*var_lastIndex, "==", 1, partcv, params);
// compilenode returning opresult36
  Object if30 = done;
  if (istrue(opresult36)) {
// Begin line 15
  setline(15);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_st
  return *var_st;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if30 = undefined;
  } else {
// Begin line 18
  setline(18);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe37 = gc_frame_new();
// compilenode returning *var_lastIndex
  int op_slot_left_38 = gc_frame_newslot(*var_lastIndex);
  Object num39 = alloc_Float64(1);
// compilenode returning num39
  int op_slot_right_38 = gc_frame_newslot(num39);
  params[0] = num39;
  partcv[0] = 1;
  Object sum41 = callmethod(*var_lastIndex, "+", 1, partcv, params);
// compilenode returning sum41
  gc_frame_newslot(sum41);
  int callframe42 = gc_frame_new();
// compilenode returning *var_st
  partcv[0] = 0;
  Object call43 = callmethod(*var_st, "size",
    1, partcv, params);
  gc_frame_end(callframe42);
// compilenode returning call43
// compilenode returning call43
  gc_frame_newslot(call43);
// compilenode returning *var_st
  params[0] = sum41;
  params[1] = call43;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call44 = callmethod(*var_st, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe37);
// compilenode returning call44
  *var_res = call44;
  if (call44 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 19
  setline(19);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_res
  return *var_res;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if30 = undefined;
  }
// compilenode returning if30
  gc_frame_end(frame);
  return if30;
}
Object module_RemoveContDir_init() {
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
  setclassname(self, "Module<RemoveContDir>");
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
// Begin line 4
  setline(4);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo0 = (struct UserObject*)self;
  uo0->data[1] = emptyclosure;
  Method *meth_meth_RemoveContDir_removeContDir0 = addmethod2pos(self, "removeContDir", &meth_RemoveContDir_removeContDir0, 1);
int argcv_meth_RemoveContDir_removeContDir0[] = {1};
meth_meth_RemoveContDir_removeContDir0->type = alloc_MethodType(1, argcv_meth_RemoveContDir_removeContDir0);
meth_meth_RemoveContDir_removeContDir0->type->types[0] = type_String;
meth_meth_RemoveContDir_removeContDir0->type->names[0] = "st";
  meth_meth_RemoveContDir_removeContDir0->definitionModule = modulename;
  meth_meth_RemoveContDir_removeContDir0->definitionLine = 4;
// compilenode returning 
// Begin line 3
  setline(3);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe45 = gc_frame_new();
  if (strlit46 == NULL) {
    strlit46 = alloc_String("./here/is/a/big/friggin/wowzer.grace");
    gc_root(strlit46);
  }
// compilenode returning strlit46
  gc_frame_newslot(strlit46);
  params[0] = strlit46;
  partcv[0] = 1;
  Object call47 = callmethodflags(self, "removeContDir", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe45);
// compilenode returning call47
  params[0] = call47;
  Object call48 = gracelib_print(NULL, 1,  params);
// compilenode returning call48
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
  module_RemoveContDir_init();
  gracelib_stats();
  return 0;
}
