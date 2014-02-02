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
static const char modulename[] = "ContPath";
Object module_StandardPrelude_init();
static char *originalSourceLines[] = {
  "import \"sys\" as sys",
  "import \"io\" as io",
  "",
  "print(getContPath(\"wow/hello/good/morning.grace\"));",
  "print(getContPath(\"wow\"));",
  "",
  "method getContPath(st : String) -> String{",
  "",
  "    var count := 1;",
  "",
  "    var last := -1;",
  "    while {count <= st.size}do{",
  "",
  "        if (st[count] == \"/\")then{",
  "            last := count;",
  "        }",
  "        count := count+1;",
  "",
  "    }",
  "    if (last == -1)then{",
  "        return getBuildPath()++\"/\";",
  "    }",
  "    return st.substringFrom(0)to(last-1);",
  "",
  "}",
  "",
  "method getBuildPath() -> String{",
  "    var paths := sys.environ[\"PATH\"]",
  "    var buildPath := \"\"",
  "    for (1..paths.size) do { t->",
  "        if(paths.at(t) == \":\")then{",
  "            if(io.exists(buildPath++\"/minigrace\"))then{",
  "               return buildPath; ",
  "            }",
  "            buildPath := \"\"",
  "        } ",
  "        else{",
  "            buildPath :=  buildPath++paths[t]",
  "        }",
  "    }",
  "    print({buildPath});",
  "    return buildPath;",
  "}",
  NULL
};
static Object strlit19;
static Object strlit40;
static Object strlit53;
static Object strlit55;
static Object strlit69;
static Object strlit75;
static Object strlit79;
Object module_sys;
Object module_sys_init();
Object module_io;
Object module_io_init();
static Object strlit93;
static Object strlit97;
Object meth_ContPath__apply7(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
Object meth_ContPath__apply15(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  Object *var_last = getfromclosure(closure, 2);
  Object self = *(getfromclosure(closure, 3));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 17
  setline(17);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 14
  setline(14);
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
// Begin line 15
  setline(15);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_count
  *var_last = *var_count;
  if (*var_count == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if16 = done;
  } else {
  }
// compilenode returning if16
// Begin line 17
  setline(17);
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
Object meth_ContPath_getContPath0(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct StackFrameObject *stackframe = alloc_StackFrame(4, NULL);
  pushclosure(NULL);
  pushstackframe(stackframe, "getContPath");
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
    gracedie("missing argument list for getContPath (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_st = &(stackframe->slots[0]);
  *var_st = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for getContPath");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_count = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "count");
  Object *var_last = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "last");
// Begin line 9
  setline(9);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num1 = alloc_Float64(1);
// compilenode returning num1
  *var_count = num1;
  if (num1 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 11
  setline(11);
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
  *var_last = call4;
  if (call4 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 20
  setline(20);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe5 = gc_frame_new();
// Begin line 12
  setline(12);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block6 = alloc_Block(NULL, NULL, "ContPath", 12);
  gc_frame_newslot(block6);
  block_savedest(block6);
  Object closure7 = createclosure(3, "_apply");
setclosureframe(closure7, stackframe);
  addtoclosure(closure7, var_count);
  addtoclosure(closure7, var_st);
  addtoclosure(closure7, selfslot);
  struct UserObject *uo7 = (struct UserObject*)block6;
  uo7->data[0] = (Object)closure7;
  Method *meth_meth_ContPath__apply7 = addmethod2pos(block6, "_apply", &meth_ContPath__apply7, 0);
int argcv_meth_ContPath__apply7[] = {0};
meth_meth_ContPath__apply7->type = alloc_MethodType(1, argcv_meth_ContPath__apply7);
  meth_meth_ContPath__apply7->definitionModule = modulename;
  meth_meth_ContPath__apply7->definitionLine = 42;
// compilenode returning block6
  gc_frame_newslot(block6);
// Begin line 20
  setline(20);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block14 = alloc_Block(NULL, NULL, "ContPath", 20);
  gc_frame_newslot(block14);
  block_savedest(block14);
  Object closure15 = createclosure(4, "_apply");
setclosureframe(closure15, stackframe);
  addtoclosure(closure15, var_st);
  addtoclosure(closure15, var_count);
  addtoclosure(closure15, var_last);
  addtoclosure(closure15, selfslot);
  struct UserObject *uo15 = (struct UserObject*)block14;
  uo15->data[0] = (Object)closure15;
  Method *meth_meth_ContPath__apply15 = addmethod2pos(block14, "_apply", &meth_ContPath__apply15, 0);
int argcv_meth_ContPath__apply15[] = {0};
meth_meth_ContPath__apply15->type = alloc_MethodType(1, argcv_meth_ContPath__apply15);
  meth_meth_ContPath__apply15->definitionModule = modulename;
  meth_meth_ContPath__apply15->definitionLine = 12;
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
// Begin line 20
  setline(20);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_last
  int op_slot_left_31 = gc_frame_newslot(*var_last);
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
  Object opresult36 = callmethod(*var_last, "==", 1, partcv, params);
// compilenode returning opresult36
  Object if30 = done;
  if (istrue(opresult36)) {
// Begin line 21
  setline(21);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe38 = gc_frame_new();
  partcv[0] = 0;
  Object call39 = callmethodflags(self, "getBuildPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe38);
// compilenode returning call39
  int op_slot_left_37 = gc_frame_newslot(call39);
  if (strlit40 == NULL) {
    strlit40 = alloc_String("/");
    gc_root(strlit40);
  }
// compilenode returning strlit40
  int op_slot_right_37 = gc_frame_newslot(strlit40);
  params[0] = strlit40;
  partcv[0] = 1;
  Object opresult42 = callmethod(call39, "++", 1, partcv, params);
// compilenode returning opresult42
  return opresult42;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if30 = undefined;
  } else {
  }
// compilenode returning if30
// Begin line 23
  setline(23);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe43 = gc_frame_new();
  Object num44 = alloc_Float64(0);
// compilenode returning num44
  gc_frame_newslot(num44);
// compilenode returning *var_last
  int op_slot_left_45 = gc_frame_newslot(*var_last);
  Object num46 = alloc_Float64(1);
// compilenode returning num46
  int op_slot_right_45 = gc_frame_newslot(num46);
  params[0] = num46;
  partcv[0] = 1;
  Object diff48 = callmethod(*var_last, "-", 1, partcv, params);
// compilenode returning diff48
  gc_frame_newslot(diff48);
// compilenode returning *var_st
  params[0] = num44;
  params[1] = diff48;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call49 = callmethod(*var_st, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe43);
// compilenode returning call49
  return call49;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object meth_ContPath__apply64(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  Object *var_paths = getfromclosure(closure, 0);
  Object *var_buildPath = getfromclosure(closure, 1);
  Object *var_io = getfromclosure(closure, 2);
  Object self = *(getfromclosure(closure, 3));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *var_t = &(stackframe->slots[0]);
  *var_t = args[curarg];
  curarg++;
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// Begin line 40
  setline(40);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 31
  setline(31);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe67 = gc_frame_new();
// compilenode returning *var_t
  gc_frame_newslot(*var_t);
// compilenode returning *var_paths
  params[0] = *var_t;
  partcv[0] = 1;
  Object call68 = callmethod(*var_paths, "at",
    1, partcv, params);
  gc_frame_end(callframe67);
// compilenode returning call68
  int op_slot_left_66 = gc_frame_newslot(call68);
  if (strlit69 == NULL) {
    strlit69 = alloc_String(":");
    gc_root(strlit69);
  }
// compilenode returning strlit69
  int op_slot_right_66 = gc_frame_newslot(strlit69);
  params[0] = strlit69;
  partcv[0] = 1;
  Object opresult71 = callmethod(call68, "==", 1, partcv, params);
// compilenode returning opresult71
  Object if65 = done;
  if (istrue(opresult71)) {
// Begin line 35
  setline(35);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 32
  setline(32);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe73 = gc_frame_new();
// compilenode returning *var_buildPath
  int op_slot_left_74 = gc_frame_newslot(*var_buildPath);
  if (strlit75 == NULL) {
    strlit75 = alloc_String("/minigrace");
    gc_root(strlit75);
  }
// compilenode returning strlit75
  int op_slot_right_74 = gc_frame_newslot(strlit75);
  params[0] = strlit75;
  partcv[0] = 1;
  Object opresult77 = callmethod(*var_buildPath, "++", 1, partcv, params);
// compilenode returning opresult77
  gc_frame_newslot(opresult77);
// compilenode returning *var_io
  params[0] = opresult77;
  partcv[0] = 1;
  Object call78 = callmethod(*var_io, "exists",
    1, partcv, params);
  gc_frame_end(callframe73);
// compilenode returning call78
  Object if72 = done;
  if (istrue(call78)) {
// Begin line 33
  setline(33);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  block_return(realself, *var_buildPath);
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if72 = undefined;
  } else {
  }
// compilenode returning if72
// Begin line 36
  setline(36);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 35
  setline(35);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit79 == NULL) {
    strlit79 = alloc_String("");
    gc_root(strlit79);
  }
// compilenode returning strlit79
  *var_buildPath = strlit79;
  if (strlit79 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if65 = done;
  } else {
// Begin line 39
  setline(39);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 38
  setline(38);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  int op_slot_left_81 = gc_frame_newslot(*var_buildPath);
// Begin line 39
  setline(39);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 38
  setline(38);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_paths
// compilenode returning *var_t
  params[0] = *var_t;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres82 = callmethod(*var_paths, "[]", 1, partcv, params);
// compilenode returning idxres82
  int op_slot_right_81 = gc_frame_newslot(idxres82);
  params[0] = idxres82;
  partcv[0] = 1;
  Object opresult84 = callmethod(*var_buildPath, "++", 1, partcv, params);
// compilenode returning opresult84
  *var_buildPath = opresult84;
  if (opresult84 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if65 = done;
  }
// compilenode returning if65
  gc_frame_end(frame);
  return if65;
}
Object meth_ContPath__apply89(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  Object *var_buildPath = getfromclosure(closure, 0);
  Object self = *(getfromclosure(closure, 1));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var_buildPath
  gc_frame_end(frame);
  return *var_buildPath;
}
Object meth_ContPath_getBuildPath50(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct UserObject *uo = (struct UserObject*)self;
  Object closure = getdatum((Object)uo, 2, (flags>>24)&0xff);
  struct StackFrameObject *stackframe = alloc_StackFrame(3, getclosureframe(closure));
  pushclosure(closure);
  pushstackframe(stackframe, "getBuildPath");
  int frame = gc_frame_new();
  gc_frame_newslot((Object)stackframe);
  Object methodInheritingObject = NULL;
  Object params[2];
  int partcv[2];
  Object *var_sys = getfromclosure(closure, 0);
  Object *var_io = getfromclosure(closure, 1);
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  if (nparts < 1 && args)
    gracedie("missing argument list for getBuildPath (probably reflection error): got %i lists, expected 1.", nparts);
if (argcv && argcv[0] > 0)
  gracedie("too many arguments for getBuildPath");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_paths = &(stackframe->slots[1]);
  setframeelementname(stackframe, 1, "paths");
  Object *var_buildPath = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "buildPath");
// Begin line 29
  setline(29);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 28
  setline(28);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe51 = gc_frame_new();
// compilenode returning *var_sys
  partcv[0] = 0;
  Object call52 = callmethod(*var_sys, "environ",
    1, partcv, params);
  gc_frame_end(callframe51);
// compilenode returning call52
// compilenode returning call52
  if (strlit53 == NULL) {
    strlit53 = alloc_String("PATH");
    gc_root(strlit53);
  }
// compilenode returning strlit53
  params[0] = strlit53;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres54 = callmethod(call52, "[]", 1, partcv, params);
// compilenode returning idxres54
  *var_paths = idxres54;
  if (idxres54 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 30
  setline(30);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 29
  setline(29);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit55 == NULL) {
    strlit55 = alloc_String("");
    gc_root(strlit55);
  }
// compilenode returning strlit55
  *var_buildPath = strlit55;
  if (strlit55 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 41
  setline(41);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe56 = gc_frame_new();
// Begin line 30
  setline(30);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num58 = alloc_Float64(1);
// compilenode returning num58
  int op_slot_left_57 = gc_frame_newslot(num58);
  int callframe59 = gc_frame_new();
// compilenode returning *var_paths
  partcv[0] = 0;
  Object call60 = callmethod(*var_paths, "size",
    1, partcv, params);
  gc_frame_end(callframe59);
// compilenode returning call60
// compilenode returning call60
  int op_slot_right_57 = gc_frame_newslot(call60);
  params[0] = call60;
  partcv[0] = 1;
  Object opresult62 = callmethod(num58, "..", 1, partcv, params);
// compilenode returning opresult62
  gc_frame_newslot(opresult62);
// Begin line 41
  setline(41);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block63 = alloc_Block(NULL, NULL, "ContPath", 41);
  gc_frame_newslot(block63);
  block_savedest(block63);
  Object closure64 = createclosure(4, "_apply");
setclosureframe(closure64, stackframe);
  addtoclosure(closure64, var_paths);
  addtoclosure(closure64, var_buildPath);
  addtoclosure(closure64, var_io);
  addtoclosure(closure64, selfslot);
  struct UserObject *uo64 = (struct UserObject*)block63;
  uo64->data[0] = (Object)closure64;
  Method *meth_meth_ContPath__apply64 = addmethod2pos(block63, "_apply", &meth_ContPath__apply64, 0);
int argcv_meth_ContPath__apply64[] = {1};
meth_meth_ContPath__apply64->type = alloc_MethodType(1, argcv_meth_ContPath__apply64);
  meth_meth_ContPath__apply64->definitionModule = modulename;
  meth_meth_ContPath__apply64->definitionLine = 30;
// compilenode returning block63
  gc_frame_newslot(block63);
  params[0] = opresult62;
  params[1] = block63;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call87 = callmethodflags(prelude, "for()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe56);
// compilenode returning call87
// Begin line 41
  setline(41);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block88 = alloc_Block(NULL, NULL, "ContPath", 41);
  gc_frame_newslot(block88);
  block_savedest(block88);
  Object closure89 = createclosure(2, "_apply");
setclosureframe(closure89, stackframe);
  addtoclosure(closure89, var_buildPath);
  addtoclosure(closure89, selfslot);
  struct UserObject *uo89 = (struct UserObject*)block88;
  uo89->data[0] = (Object)closure89;
  Method *meth_meth_ContPath__apply89 = addmethod2pos(block88, "_apply", &meth_ContPath__apply89, 0);
int argcv_meth_ContPath__apply89[] = {0};
meth_meth_ContPath__apply89->type = alloc_MethodType(1, argcv_meth_ContPath__apply89);
  meth_meth_ContPath__apply89->definitionModule = modulename;
  meth_meth_ContPath__apply89->definitionLine = 30;
// compilenode returning block88
  params[0] = block88;
  Object call91 = gracelib_print(NULL, 1,  params);
// compilenode returning call91
// Begin line 42
  setline(42);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_buildPath
  return *var_buildPath;
// compilenode returning undefined
  gc_frame_end(frame);
  return undefined;
}
Object module_ContPath_init() {
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
  setclassname(self, "Module<ContPath>");
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
Object *var_sys = alloc_var();
Object *var_io = alloc_var();
// Begin line 7
  setline(7);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo0 = (struct UserObject*)self;
  uo0->data[1] = emptyclosure;
  Method *meth_meth_ContPath_getContPath0 = addmethod2pos(self, "getContPath", &meth_ContPath_getContPath0, 1);
int argcv_meth_ContPath_getContPath0[] = {1};
meth_meth_ContPath_getContPath0->type = alloc_MethodType(1, argcv_meth_ContPath_getContPath0);
meth_meth_ContPath_getContPath0->type->types[0] = type_String;
meth_meth_ContPath_getContPath0->type->names[0] = "st";
  meth_meth_ContPath_getContPath0->definitionModule = modulename;
  meth_meth_ContPath_getContPath0->definitionLine = 7;
// compilenode returning 
// Begin line 27
  setline(27);
  setmodule(modulename);
  setsource(originalSourceLines);
  block_savedest(self);
  Object closure50 = createclosure(2, "getBuildPath");
setclosureframe(closure50, stackframe);
  addtoclosure(closure50, var_sys);
  addtoclosure(closure50, var_io);
  struct UserObject *uo50 = (struct UserObject*)self;
  uo50->data[2] = (Object)closure50;
  Method *meth_meth_ContPath_getBuildPath50 = addmethod2pos(self, "getBuildPath", &meth_ContPath_getBuildPath50, 2);
int argcv_meth_ContPath_getBuildPath50[] = {0};
meth_meth_ContPath_getBuildPath50->type = alloc_MethodType(1, argcv_meth_ContPath_getBuildPath50);
  meth_meth_ContPath_getBuildPath50->definitionModule = modulename;
  meth_meth_ContPath_getBuildPath50->definitionLine = 27;
// compilenode returning 
// Begin line 1
  setline(1);
  setmodule(modulename);
  setsource(originalSourceLines);
// Import of sys as sys
  if (module_sys == NULL)
    module_sys = module_sys_init();
  *var_sys = module_sys;
// compilenode returning done
// Begin line 2
  setline(2);
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
  int callframe92 = gc_frame_new();
  if (strlit93 == NULL) {
    strlit93 = alloc_String("wow/hello/good/morning.grace");
    gc_root(strlit93);
  }
// compilenode returning strlit93
  gc_frame_newslot(strlit93);
  params[0] = strlit93;
  partcv[0] = 1;
  Object call94 = callmethodflags(self, "getContPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe92);
// compilenode returning call94
  params[0] = call94;
  Object call95 = gracelib_print(NULL, 1,  params);
// compilenode returning call95
// Begin line 5
  setline(5);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe96 = gc_frame_new();
  if (strlit97 == NULL) {
    strlit97 = alloc_String("wow");
    gc_root(strlit97);
  }
// compilenode returning strlit97
  gc_frame_newslot(strlit97);
  params[0] = strlit97;
  partcv[0] = 1;
  Object call98 = callmethodflags(self, "getContPath", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe96);
// compilenode returning call98
  params[0] = call98;
  Object call99 = gracelib_print(NULL, 1,  params);
// compilenode returning call99
// Begin line 7
  setline(7);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning 
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
  module_ContPath_init();
  gracelib_stats();
  return 0;
}
