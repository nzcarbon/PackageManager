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
static const char modulename[] = "Strip";
Object module_StandardPrelude_init();
static char *originalSourceLines[] = {
  "",
  "",
  "print(stripLine(\"http://a\"));",
  "",
  "",
  "method stripLine(importStatement : String) -> String{",
  "",
  "    print(\"Size = {importStatement.size}\");",
  "    if (importStatement.size < 7)then{",
  "        return importStatement;",
  "    }",
  "    if (importStatement.substringFrom(1)to(7) == \"http://\" )then{",
  "        var lastIndex := 7;",
  "        var curPos := 7;",
  "        while {curPos <= importStatement.size}do{",
  "            if (importStatement[curPos] == \"/\")then{",
  "                lastIndex := curPos;",
  "            }",
  "            curPos := curPos +1;",
  "        }",
  "        var res := importStatement.substringFrom(lastIndex+1)to(importStatement.size);",
  "        return res;",
  "    }",
  "    else{",
  "        return importStatement;",
  "    }",
  "",
  "",
  "}",
  NULL
};
static Object strlit3;
static Object strlit8;
static Object strlit25;
static Object strlit44;
static Object strlit64;
Object meth_Strip__apply32(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  Object *var_curPos = getfromclosure(closure, 0);
  Object *var_importStatement = getfromclosure(closure, 1);
  Object self = *(getfromclosure(closure, 2));
  sourceObject = self;
  int i;
  int curarg = 0;
  int pushcv[] = {1};
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
// compilenode returning *var_curPos
  int op_slot_left_33 = gc_frame_newslot(*var_curPos);
  int callframe34 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call35 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe34);
// compilenode returning call35
// compilenode returning call35
  int op_slot_right_33 = gc_frame_newslot(call35);
  params[0] = call35;
  partcv[0] = 1;
  Object opresult37 = callmethod(*var_curPos, "<=", 1, partcv, params);
// compilenode returning opresult37
  gc_frame_end(frame);
  return opresult37;
}
Object meth_Strip__apply40(Object realself, int nparts, int *argcv, Object *args, int32_t flags) {
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
  Object *var_importStatement = getfromclosure(closure, 0);
  Object *var_curPos = getfromclosure(closure, 1);
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
// Begin line 19
  setline(19);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 16
  setline(16);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
// compilenode returning *var_curPos
  params[0] = *var_curPos;
  gc_frame_newslot(params[0]);
  partcv[0] = 1;
  Object idxres43 = callmethod(*var_importStatement, "[]", 1, partcv, params);
// compilenode returning idxres43
  int op_slot_left_42 = gc_frame_newslot(idxres43);
  if (strlit44 == NULL) {
    strlit44 = alloc_String("/");
    gc_root(strlit44);
  }
// compilenode returning strlit44
  int op_slot_right_42 = gc_frame_newslot(strlit44);
  params[0] = strlit44;
  partcv[0] = 1;
  Object opresult46 = callmethod(idxres43, "==", 1, partcv, params);
// compilenode returning opresult46
  Object if41 = done;
  if (istrue(opresult46)) {
// Begin line 17
  setline(17);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  *var_lastIndex = *var_curPos;
  if (*var_curPos == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
    gc_frame_newslot(done);
    if41 = done;
  } else {
  }
// compilenode returning if41
// Begin line 19
  setline(19);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_curPos
  int op_slot_left_48 = gc_frame_newslot(*var_curPos);
  Object num49 = alloc_Float64(1);
// compilenode returning num49
  int op_slot_right_48 = gc_frame_newslot(num49);
  params[0] = num49;
  partcv[0] = 1;
  Object sum51 = callmethod(*var_curPos, "+", 1, partcv, params);
// compilenode returning sum51
  *var_curPos = sum51;
  if (sum51 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
  gc_frame_end(frame);
  return done;
}
Object meth_Strip_stripLine0(Object self, int nparts, int *argcv, Object *args, int32_t flags) {
  struct StackFrameObject *stackframe = alloc_StackFrame(5, NULL);
  pushclosure(NULL);
  pushstackframe(stackframe, "stripLine");
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
    gracedie("missing argument list for stripLine (probably reflection error): got %i lists, expected 1.", nparts);
  Object *var_importStatement = &(stackframe->slots[0]);
  *var_importStatement = args[curarg];
  curarg++;
if (argcv && argcv[0] > 1)
  gracedie("too many arguments for stripLine");
  Object *selfslot = &(stackframe->slots[1]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  if (methodInheritingObject) curarg++;
  Object *var_lastIndex = &(stackframe->slots[2]);
  setframeelementname(stackframe, 2, "lastIndex");
  Object *var_curPos = &(stackframe->slots[3]);
  setframeelementname(stackframe, 3, "curPos");
  Object *var_res = &(stackframe->slots[4]);
  setframeelementname(stackframe, 4, "res");
// Begin line 8
  setline(8);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit3 == NULL) {
    strlit3 = alloc_String("Size = ");
    gc_root(strlit3);
  }
// compilenode returning strlit3
  int op_slot_left_2 = gc_frame_newslot(strlit3);
  int callframe4 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call5 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe4);
// compilenode returning call5
// compilenode returning call5
  int op_slot_right_2 = gc_frame_newslot(call5);
  params[0] = call5;
  partcv[0] = 1;
  Object opresult7 = callmethod(strlit3, "++", 1, partcv, params);
// compilenode returning opresult7
  int op_slot_left_1 = gc_frame_newslot(opresult7);
  if (strlit8 == NULL) {
    strlit8 = alloc_String("");
    gc_root(strlit8);
  }
// compilenode returning strlit8
  int op_slot_right_1 = gc_frame_newslot(strlit8);
  params[0] = strlit8;
  partcv[0] = 1;
  Object opresult10 = callmethod(opresult7, "++", 1, partcv, params);
// compilenode returning opresult10
  params[0] = opresult10;
  Object call11 = gracelib_print(NULL, 1,  params);
// compilenode returning call11
// Begin line 12
  setline(12);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 9
  setline(9);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe14 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call15 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe14);
// compilenode returning call15
// compilenode returning call15
  int op_slot_left_13 = gc_frame_newslot(call15);
  Object num16 = alloc_Float64(7);
// compilenode returning num16
  int op_slot_right_13 = gc_frame_newslot(num16);
  params[0] = num16;
  partcv[0] = 1;
  Object opresult18 = callmethod(call15, "<", 1, partcv, params);
// compilenode returning opresult18
  Object if12 = done;
  if (istrue(opresult18)) {
// Begin line 10
  setline(10);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
  return *var_importStatement;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if12 = undefined;
  } else {
  }
// compilenode returning if12
// Begin line 29
  setline(29);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 12
  setline(12);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe21 = gc_frame_new();
  Object num22 = alloc_Float64(1);
// compilenode returning num22
  gc_frame_newslot(num22);
  Object num23 = alloc_Float64(7);
// compilenode returning num23
  gc_frame_newslot(num23);
// compilenode returning *var_importStatement
  params[0] = num22;
  params[1] = num23;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call24 = callmethod(*var_importStatement, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe21);
// compilenode returning call24
  int op_slot_left_20 = gc_frame_newslot(call24);
  if (strlit25 == NULL) {
    strlit25 = alloc_String("http://");
    gc_root(strlit25);
  }
// compilenode returning strlit25
  int op_slot_right_20 = gc_frame_newslot(strlit25);
  params[0] = strlit25;
  partcv[0] = 1;
  Object opresult27 = callmethod(call24, "==", 1, partcv, params);
// compilenode returning opresult27
  Object if19 = done;
  if (istrue(opresult27)) {
// Begin line 13
  setline(13);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num28 = alloc_Float64(7);
// compilenode returning num28
  *var_lastIndex = num28;
  if (num28 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 14
  setline(14);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object num29 = alloc_Float64(7);
// compilenode returning num29
  *var_curPos = num29;
  if (num29 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 21
  setline(21);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe30 = gc_frame_new();
// Begin line 15
  setline(15);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block31 = alloc_Block(NULL, NULL, "Strip", 15);
  gc_frame_newslot(block31);
  block_savedest(block31);
  Object closure32 = createclosure(3, "_apply");
setclosureframe(closure32, stackframe);
  addtoclosure(closure32, var_curPos);
  addtoclosure(closure32, var_importStatement);
  addtoclosure(closure32, selfslot);
  struct UserObject *uo32 = (struct UserObject*)block31;
  uo32->data[0] = (Object)closure32;
  Method *meth_meth_Strip__apply32 = addmethod2pos(block31, "_apply", &meth_Strip__apply32, 0);
int argcv_meth_Strip__apply32[] = {0};
meth_meth_Strip__apply32->type = alloc_MethodType(1, argcv_meth_Strip__apply32);
  meth_meth_Strip__apply32->definitionModule = modulename;
  meth_meth_Strip__apply32->definitionLine = 9;
// compilenode returning block31
  gc_frame_newslot(block31);
// Begin line 21
  setline(21);
  setmodule(modulename);
  setsource(originalSourceLines);
  Object block39 = alloc_Block(NULL, NULL, "Strip", 21);
  gc_frame_newslot(block39);
  block_savedest(block39);
  Object closure40 = createclosure(4, "_apply");
setclosureframe(closure40, stackframe);
  addtoclosure(closure40, var_importStatement);
  addtoclosure(closure40, var_curPos);
  addtoclosure(closure40, var_lastIndex);
  addtoclosure(closure40, selfslot);
  struct UserObject *uo40 = (struct UserObject*)block39;
  uo40->data[0] = (Object)closure40;
  Method *meth_meth_Strip__apply40 = addmethod2pos(block39, "_apply", &meth_Strip__apply40, 0);
int argcv_meth_Strip__apply40[] = {0};
meth_meth_Strip__apply40->type = alloc_MethodType(1, argcv_meth_Strip__apply40);
  meth_meth_Strip__apply40->definitionModule = modulename;
  meth_meth_Strip__apply40->definitionLine = 15;
// compilenode returning block39
  gc_frame_newslot(block39);
  params[0] = block31;
  params[1] = block39;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call54 = callmethodflags(prelude, "while()do", 2, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe30);
// compilenode returning call54
// Begin line 21
  setline(21);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe55 = gc_frame_new();
// compilenode returning *var_lastIndex
  int op_slot_left_56 = gc_frame_newslot(*var_lastIndex);
  Object num57 = alloc_Float64(1);
// compilenode returning num57
  int op_slot_right_56 = gc_frame_newslot(num57);
  params[0] = num57;
  partcv[0] = 1;
  Object sum59 = callmethod(*var_lastIndex, "+", 1, partcv, params);
// compilenode returning sum59
  gc_frame_newslot(sum59);
  int callframe60 = gc_frame_new();
// compilenode returning *var_importStatement
  partcv[0] = 0;
  Object call61 = callmethod(*var_importStatement, "size",
    1, partcv, params);
  gc_frame_end(callframe60);
// compilenode returning call61
// compilenode returning call61
  gc_frame_newslot(call61);
// compilenode returning *var_importStatement
  params[0] = sum59;
  params[1] = call61;
  partcv[0] = 1;
  partcv[1] = 1;
  Object call62 = callmethod(*var_importStatement, "substringFrom()to",
    2, partcv, params);
  gc_frame_end(callframe55);
// compilenode returning call62
  *var_res = call62;
  if (call62 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 22
  setline(22);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_res
  return *var_res;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if19 = undefined;
  } else {
// Begin line 25
  setline(25);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_importStatement
  return *var_importStatement;
// compilenode returning undefined
    gc_frame_newslot(undefined);
    if19 = undefined;
  }
// compilenode returning if19
  gc_frame_end(frame);
  return if19;
}
Object module_Strip_init() {
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
  setclassname(self, "Module<Strip>");
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
// Begin line 6
  setline(6);
  setmodule(modulename);
  setsource(originalSourceLines);
  struct UserObject *uo0 = (struct UserObject*)self;
  uo0->data[1] = emptyclosure;
  Method *meth_meth_Strip_stripLine0 = addmethod2pos(self, "stripLine", &meth_Strip_stripLine0, 1);
int argcv_meth_Strip_stripLine0[] = {1};
meth_meth_Strip_stripLine0->type = alloc_MethodType(1, argcv_meth_Strip_stripLine0);
meth_meth_Strip_stripLine0->type->types[0] = type_String;
meth_meth_Strip_stripLine0->type->names[0] = "importStatement";
  meth_meth_Strip_stripLine0->definitionModule = modulename;
  meth_meth_Strip_stripLine0->definitionLine = 6;
// compilenode returning 
// Begin line 3
  setline(3);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe63 = gc_frame_new();
  if (strlit64 == NULL) {
    strlit64 = alloc_String("http://a");
    gc_root(strlit64);
  }
// compilenode returning strlit64
  gc_frame_newslot(strlit64);
  params[0] = strlit64;
  partcv[0] = 1;
  Object call65 = callmethodflags(self, "stripLine", 1, partcv, params, CFLAG_SELF);
  gc_frame_end(callframe63);
// compilenode returning call65
  params[0] = call65;
  Object call66 = gracelib_print(NULL, 1,  params);
// compilenode returning call66
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
  module_Strip_init();
  gracelib_stats();
  return 0;
}
