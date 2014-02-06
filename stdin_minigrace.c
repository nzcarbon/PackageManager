#include <gracelib.h>
#include <stdlib.h>
#ifndef __CYGWIN__
#pragma weak main
#endif
static char compilerRevision[] = "bfad4d7ac24371f29852d9ad68fdaf701178d943";
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
  "import \"io\" as io",
  "import \"__pkg-temp\" as pkg",
  "import \"PackageManager\" as pm",
  "",
  "print (io.listdir(\"./\"));",
  "print (\"setup piped\")",
  "print (\"New package name = {pkg.name}\")",
  "",
  "",
  "if (pkg.__bundle)then{",
  "",
  "    pm.bundle(pkg.__loc,pkg.name)",
  "    var cmd := \"tar cvzf {pkg.__loc}../{pkg.name}.tar.gz {pkg.__loc}../{pkg.name}\"",
  "    print(cmd)",
  "    io.system(cmd)",
  "",
  "}",
  "",
  "else{",
  "",
  "",
  "",
  "",
  "",
  "",
  "}",
  NULL
};
Object module_io;
Object module_io_init();
Object module___pkg_45_temp;
Object module___pkg_45_temp_init();
Object module_PackageManager;
Object module_PackageManager_init();
static Object strlit1;
static Object strlit4;
static Object strlit7;
static Object strlit13;
static Object strlit27;
static Object strlit34;
static Object strlit41;
static Object strlit48;
static Object strlit54;
Object module_stdin_minigrace_init() {
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
  struct StackFrameObject *stackframe = alloc_StackFrame(3, NULL);
  gc_root((Object)stackframe);
  pushstackframe(stackframe, "module scope");
  Object *selfslot = &(stackframe->slots[0]);
  *selfslot = self;
  setframeelementname(stackframe, 0, "self");
  Object params[2];
  int partcv[1];
Object *var_io = alloc_var();
Object *var_pkg = alloc_var();
Object *var_pm = alloc_var();
  Object *var_cmd = &(stackframe->slots[1]);
  setframeelementname(stackframe, 1, "cmd");
// Import of io as io
  if (module_io == NULL)
    module_io = module_io_init();
  *var_io = module_io;
// compilenode returning done
// Begin line 2
  setline(2);
  setmodule(modulename);
  setsource(originalSourceLines);
// Import of __pkg-temp as pkg
  if (module___pkg_45_temp == NULL)
    module___pkg_45_temp = module___pkg_45_temp_init();
  *var_pkg = module___pkg_45_temp;
// compilenode returning done
// Begin line 3
  setline(3);
  setmodule(modulename);
  setsource(originalSourceLines);
// Import of PackageManager as pm
  if (module_PackageManager == NULL)
    module_PackageManager = module_PackageManager_init();
  *var_pm = module_PackageManager;
// compilenode returning done
// compilenode returning 
// Begin line 5
  setline(5);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe0 = gc_frame_new();
  if (strlit1 == NULL) {
    strlit1 = alloc_String("./");
    gc_root(strlit1);
  }
// compilenode returning strlit1
  gc_frame_newslot(strlit1);
// compilenode returning *var_io
  params[0] = strlit1;
  partcv[0] = 1;
  Object call2 = callmethod(*var_io, "listdir",
    1, partcv, params);
  gc_frame_end(callframe0);
// compilenode returning call2
  params[0] = call2;
  Object call3 = gracelib_print(NULL, 1,  params);
// compilenode returning call3
// Begin line 6
  setline(6);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit4 == NULL) {
    strlit4 = alloc_String("setup piped");
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
  if (strlit7 == NULL) {
    strlit7 = alloc_String("");
    gc_root(strlit7);
  }
// compilenode returning strlit7
  int op_slot_right_6 = gc_frame_newslot(strlit7);
  int callframe10 = gc_frame_new();
// compilenode returning *var_pkg
  partcv[0] = 0;
  Object call11 = callmethod(*var_pkg, "name",
    1, partcv, params);
  gc_frame_end(callframe10);
// compilenode returning call11
// compilenode returning call11
  int op_slot_right_9 = gc_frame_newslot(call11);
  if (strlit13 == NULL) {
    strlit13 = alloc_String("New package name = ");
    gc_root(strlit13);
  }
// compilenode returning strlit13
  int op_slot_left_9 = gc_frame_newslot(strlit13);
  params[0] = call11;
  partcv[0] = 1;
  Object opresult14 = callmethod(strlit13, "++", 1, partcv, params);
// compilenode returning opresult14
  int op_slot_left_6 = gc_frame_newslot(opresult14);
  params[0] = strlit7;
  partcv[0] = 1;
  Object opresult15 = callmethod(opresult14, "++", 1, partcv, params);
// compilenode returning opresult15
  params[0] = opresult15;
  Object call16 = gracelib_print(NULL, 1,  params);
// compilenode returning call16
// compilenode returning 
// Begin line 26
  setline(26);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 10
  setline(10);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe18 = gc_frame_new();
// compilenode returning *var_pkg
  partcv[0] = 0;
  Object call19 = callmethod(*var_pkg, "__bundle",
    1, partcv, params);
  gc_frame_end(callframe18);
// compilenode returning call19
// compilenode returning call19
  Object if17 = done;
  if (istrue(call19)) {
// Begin line 12
  setline(12);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe20 = gc_frame_new();
  int callframe21 = gc_frame_new();
// compilenode returning *var_pkg
  partcv[0] = 0;
  Object call22 = callmethod(*var_pkg, "__loc",
    1, partcv, params);
  gc_frame_end(callframe21);
// compilenode returning call22
// compilenode returning call22
  gc_frame_newslot(call22);
  int callframe23 = gc_frame_new();
// compilenode returning *var_pkg
  partcv[0] = 0;
  Object call24 = callmethod(*var_pkg, "name",
    1, partcv, params);
  gc_frame_end(callframe23);
// compilenode returning call24
// compilenode returning call24
  gc_frame_newslot(call24);
// compilenode returning *var_pm
  params[0] = call22;
  params[1] = call24;
  partcv[0] = 2;
  Object call25 = callmethod(*var_pm, "bundle",
    1, partcv, params);
  gc_frame_end(callframe20);
// compilenode returning call25
// Begin line 14
  setline(14);
  setmodule(modulename);
  setsource(originalSourceLines);
// Begin line 13
  setline(13);
  setmodule(modulename);
  setsource(originalSourceLines);
  if (strlit27 == NULL) {
    strlit27 = alloc_String("");
    gc_root(strlit27);
  }
// compilenode returning strlit27
  int op_slot_right_26 = gc_frame_newslot(strlit27);
  int callframe30 = gc_frame_new();
// compilenode returning *var_pkg
  partcv[0] = 0;
  Object call31 = callmethod(*var_pkg, "name",
    1, partcv, params);
  gc_frame_end(callframe30);
// compilenode returning call31
// compilenode returning call31
  int op_slot_right_29 = gc_frame_newslot(call31);
  if (strlit34 == NULL) {
    strlit34 = alloc_String("../");
    gc_root(strlit34);
  }
// compilenode returning strlit34
  int op_slot_right_33 = gc_frame_newslot(strlit34);
  int callframe37 = gc_frame_new();
// compilenode returning *var_pkg
  partcv[0] = 0;
  Object call38 = callmethod(*var_pkg, "__loc",
    1, partcv, params);
  gc_frame_end(callframe37);
// compilenode returning call38
// compilenode returning call38
  int op_slot_right_36 = gc_frame_newslot(call38);
  if (strlit41 == NULL) {
    strlit41 = alloc_String(".tar.gz ");
    gc_root(strlit41);
  }
// compilenode returning strlit41
  int op_slot_right_40 = gc_frame_newslot(strlit41);
  int callframe44 = gc_frame_new();
// compilenode returning *var_pkg
  partcv[0] = 0;
  Object call45 = callmethod(*var_pkg, "name",
    1, partcv, params);
  gc_frame_end(callframe44);
// compilenode returning call45
// compilenode returning call45
  int op_slot_right_43 = gc_frame_newslot(call45);
  if (strlit48 == NULL) {
    strlit48 = alloc_String("../");
    gc_root(strlit48);
  }
// compilenode returning strlit48
  int op_slot_right_47 = gc_frame_newslot(strlit48);
  int callframe51 = gc_frame_new();
// compilenode returning *var_pkg
  partcv[0] = 0;
  Object call52 = callmethod(*var_pkg, "__loc",
    1, partcv, params);
  gc_frame_end(callframe51);
// compilenode returning call52
// compilenode returning call52
  int op_slot_right_50 = gc_frame_newslot(call52);
  if (strlit54 == NULL) {
    strlit54 = alloc_String("tar cvzf ");
    gc_root(strlit54);
  }
// compilenode returning strlit54
  int op_slot_left_50 = gc_frame_newslot(strlit54);
  params[0] = call52;
  partcv[0] = 1;
  Object opresult55 = callmethod(strlit54, "++", 1, partcv, params);
// compilenode returning opresult55
  int op_slot_left_47 = gc_frame_newslot(opresult55);
  params[0] = strlit48;
  partcv[0] = 1;
  Object opresult56 = callmethod(opresult55, "++", 1, partcv, params);
// compilenode returning opresult56
  int op_slot_left_43 = gc_frame_newslot(opresult56);
  params[0] = call45;
  partcv[0] = 1;
  Object opresult57 = callmethod(opresult56, "++", 1, partcv, params);
// compilenode returning opresult57
  int op_slot_left_40 = gc_frame_newslot(opresult57);
  params[0] = strlit41;
  partcv[0] = 1;
  Object opresult58 = callmethod(opresult57, "++", 1, partcv, params);
// compilenode returning opresult58
  int op_slot_left_36 = gc_frame_newslot(opresult58);
  params[0] = call38;
  partcv[0] = 1;
  Object opresult59 = callmethod(opresult58, "++", 1, partcv, params);
// compilenode returning opresult59
  int op_slot_left_33 = gc_frame_newslot(opresult59);
  params[0] = strlit34;
  partcv[0] = 1;
  Object opresult60 = callmethod(opresult59, "++", 1, partcv, params);
// compilenode returning opresult60
  int op_slot_left_29 = gc_frame_newslot(opresult60);
  params[0] = call31;
  partcv[0] = 1;
  Object opresult61 = callmethod(opresult60, "++", 1, partcv, params);
// compilenode returning opresult61
  int op_slot_left_26 = gc_frame_newslot(opresult61);
  params[0] = strlit27;
  partcv[0] = 1;
  Object opresult62 = callmethod(opresult61, "++", 1, partcv, params);
// compilenode returning opresult62
  *var_cmd = opresult62;
  if (opresult62 == undefined)
    callmethod(done, "assignment", 0, NULL, NULL);
// compilenode returning done
// Begin line 14
  setline(14);
  setmodule(modulename);
  setsource(originalSourceLines);
// compilenode returning *var_cmd
  params[0] = *var_cmd;
  Object call63 = gracelib_print(NULL, 1,  params);
// compilenode returning call63
// Begin line 15
  setline(15);
  setmodule(modulename);
  setsource(originalSourceLines);
  int callframe64 = gc_frame_new();
// compilenode returning *var_cmd
  gc_frame_newslot(*var_cmd);
// compilenode returning *var_io
  params[0] = *var_cmd;
  partcv[0] = 1;
  Object call65 = callmethod(*var_io, "system",
    1, partcv, params);
  gc_frame_end(callframe64);
// compilenode returning call65
    gc_frame_newslot(call65);
    if17 = call65;
  } else {
  }
// compilenode returning if17
  gc_frame_end(frame);
  return self;
}
int main(int argc, char **argv) {
  initprofiling();
  setCompilerModulePath("/am/rialto/home1/forretcrai/MinigraceRevs/minigrace");
  setModulePath("/usr/lib/grace/modules");
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
