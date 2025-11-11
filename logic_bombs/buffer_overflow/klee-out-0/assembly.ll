; ModuleID = '/home/klee/logic_bombs/src/buffer_overflow/heap_bo_l1_klee_annotated.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@n = global i32 5, align 4
@.str = private unnamed_addr constant [28 x i8] c"0 && \22Logic bomb triggered\22\00", align 1
@.str1 = private unnamed_addr constant [28 x i8] c"heap_bo_l1_klee_annotated.c\00", align 1
@__PRETTY_FUNCTION__.logic_bomb = private unnamed_addr constant [23 x i8] c"int logic_bomb(char *)\00", align 1
@.str2 = private unnamed_addr constant [7 x i8] c"symvar\00", align 1
@switch.table = private unnamed_addr constant [5 x i32] [i32 0, i32 3073, i32 2048, i32 1024, i32 3072]
@.str3 = private unnamed_addr constant [56 x i8] c"/home/user/klee/runtime/Intrinsic/klee_div_zero_check.c\00", align 1
@.str14 = private unnamed_addr constant [15 x i8] c"divide by zero\00", align 1
@.str25 = private unnamed_addr constant [8 x i8] c"div.err\00", align 1
@.str36 = private unnamed_addr constant [8 x i8] c"IGNORED\00", align 1
@.str147 = private unnamed_addr constant [16 x i8] c"overshift error\00", align 1
@.str258 = private unnamed_addr constant [14 x i8] c"overshift.err\00", align 1
@.str6 = private unnamed_addr constant [47 x i8] c"/home/user/klee/runtime/Intrinsic/klee_range.c\00", align 1
@.str17 = private unnamed_addr constant [14 x i8] c"invalid range\00", align 1
@.str28 = private unnamed_addr constant [5 x i8] c"user\00", align 1
@.str9 = private unnamed_addr constant [59 x i8] c"/home/user/klee/runtime/Intrinsic/klee_set_rounding_mode.c\00", align 1
@.str110 = private unnamed_addr constant [22 x i8] c"Invalid rounding mode\00", align 1
@.str211 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1

; Function Attrs: nounwind uwtable
define i32 @logic_bomb(i8* %symvar) #0 {
entry:
  %retval = alloca i32, align 4
  %symvar.addr = alloca i8*, align 8
  %p = alloca i8*, align 8
  %q = alloca i8*, align 8
  store i8* %symvar, i8** %symvar.addr, align 8
  %call = call noalias i8* @malloc(i64 16) #10, !dbg !247
  store i8* %call, i8** %p, align 8, !dbg !247
  %call1 = call noalias i8* @malloc(i64 16) #10, !dbg !248
  store i8* %call1, i8** %q, align 8, !dbg !248
  %0 = load i8** %p, align 8, !dbg !249
  %1 = load i8** %symvar.addr, align 8, !dbg !249
  %call2 = call i8* @strcpy(i8* %0, i8* %1) #10, !dbg !249
  %2 = load i8** %q, align 8, !dbg !250
  call void @free(i8* %2) #10, !dbg !250
  %3 = load i32* @n, align 4, !dbg !251
  %cmp = icmp ne i32 %3, 5, !dbg !251
  %4 = load i8** %p, align 8, !dbg !253
  call void @free(i8* %4) #10, !dbg !253
  br i1 %cmp, label %if.then, label %if.end, !dbg !251

if.then:                                          ; preds = %entry
  %call3 = call i32 (i8*, i8*, i32, i8*, ...)* bitcast (i32 (...)* @__assert_fail to i32 (i8*, i8*, i32, i8*, ...)*)(i8* getelementptr inbounds ([28 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([28 x i8]* @.str1, i32 0, i32 0), i32 17, i8* ge
  br label %if.end, !dbg !256

if.end:                                           ; preds = %entry, %if.then
  %5 = load i32* %retval, !dbg !257
  ret i32 %5, !dbg !257
}

; Function Attrs: nounwind readnone
declare void @llvm.dbg.declare(metadata, metadata) #1

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #2

; Function Attrs: nounwind
declare i8* @strcpy(i8*, i8*) #2

; Function Attrs: nounwind
declare void @free(i8*) #2

; Function Attrs: noreturn
declare i32 @__assert_fail(...) #3

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %symvar = alloca [257 x i8], align 16
  store i32 0, i32* %retval
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  %0 = bitcast [257 x i8]* %symvar to i8*, !dbg !258
  call void @klee_make_symbolic(i8* %0, i64 257, i8* getelementptr inbounds ([7 x i8]* @.str2, i32 0, i32 0)), !dbg !258
  %arrayidx = getelementptr inbounds [257 x i8]* %symvar, i32 0, i64 256, !dbg !259
  %1 = load i8* %arrayidx, align 1, !dbg !259
  %conv = sext i8 %1 to i32, !dbg !259
  %cmp = icmp eq i32 %conv, 0, !dbg !259
  %conv1 = zext i1 %cmp to i32, !dbg !259
  %conv2 = sext i32 %conv1 to i64, !dbg !259
  call void @klee_assume(i64 %conv2), !dbg !259
  %arraydecay = getelementptr inbounds [257 x i8]* %symvar, i32 0, i32 0, !dbg !260
  %call = call i32 @logic_bomb(i8* %arraydecay), !dbg !260
  ret i32 %call, !dbg !260
}

declare void @klee_make_symbolic(i8*, i64, i8*) #4

declare void @klee_assume(i64) #4

declare zeroext i1 @klee_is_infinite_float(float) #5

declare zeroext i1 @klee_is_infinite_double(double) #5

declare zeroext i1 @klee_is_infinite_long_double(x86_fp80) #5

; Function Attrs: noinline nounwind optnone uwtable
define i32 @klee_internal_isinff(float %f) #6 {
entry:
  %isinf = tail call zeroext i1 @klee_is_infinite_float(float %f) #11
  %cmp = fcmp ogt float %f, 0.000000e+00
  %posOrNeg = select i1 %cmp, i32 1, i32 -1
  %result = select i1 %isinf, i32 %posOrNeg, i32 0
  ret i32 %result
}

; Function Attrs: noinline nounwind optnone uwtable
define i32 @klee_internal_isinf(double %d) #6 {
entry:
  %isinf = tail call zeroext i1 @klee_is_infinite_double(double %d) #11
  %cmp = fcmp ogt double %d, 0.000000e+00
  %posOrNeg = select i1 %cmp, i32 1, i32 -1
  %result = select i1 %isinf, i32 %posOrNeg, i32 0
  ret i32 %result
}

; Function Attrs: noinline optnone
define i32 @klee_internal_isinfl(x86_fp80 %d) #7 {
entry:
  %isinf = tail call zeroext i1 @klee_is_infinite_long_double(x86_fp80 %d) #11
  %cmp = fcmp ogt x86_fp80 %d, 0xK00000000000000000000
  %posOrNeg = select i1 %cmp, i32 1, i32 -1
  %result = select i1 %isinf, i32 %posOrNeg, i32 0
  ret i32 %result
}

; Function Attrs: nounwind uwtable
define double @klee_internal_fabs(double %d) #8 {
entry:
  %call = tail call double @klee_abs_double(double %d) #11, !dbg !261
  ret double %call, !dbg !261
}

declare double @klee_abs_double(double) #5

; Function Attrs: nounwind uwtable
define float @klee_internal_fabsf(float %f) #8 {
entry:
  %call = tail call float @klee_abs_float(float %f) #11, !dbg !262
  ret float %call, !dbg !262
}

declare float @klee_abs_float(float) #5

; Function Attrs: nounwind uwtable
define x86_fp80 @klee_internal_fabsl(x86_fp80 %f) #8 {
entry:
  %call = tail call x86_fp80 @klee_abs_long_double(x86_fp80 %f) #11, !dbg !263
  ret x86_fp80 %call, !dbg !263
}

declare x86_fp80 @klee_abs_long_double(x86_fp80) #5

; Function Attrs: nounwind readnone
declare void @llvm.dbg.value(metadata, i64, metadata) #1

; Function Attrs: nounwind uwtable
define i32 @klee_internal_fegetround() #8 {
entry:
  %call = tail call i32 (...)* @klee_get_rounding_mode() #11, !dbg !264
  %0 = icmp ult i32 %call, 5, !dbg !265
  br i1 %0, label %switch.lookup, label %return, !dbg !265

switch.lookup:                                    ; preds = %entry
  %1 = sext i32 %call to i64, !dbg !265
  %switch.gep = getelementptr inbounds [5 x i32]* @switch.table, i64 0, i64 %1, !dbg !265
  %switch.load = load i32* %switch.gep, align 4, !dbg !265
  ret i32 %switch.load, !dbg !265

return:                                           ; preds = %entry
  ret i32 -1, !dbg !266
}

declare i32 @klee_get_rounding_mode(...) #5

; Function Attrs: nounwind uwtable
define i32 @klee_internal_fesetround(i32 %rm) #8 {
entry:
  switch i32 %rm, label %return [
    i32 0, label %sw.bb
    i32 2048, label %sw.bb1
    i32 1024, label %sw.bb2
    i32 3072, label %sw.bb3
  ], !dbg !267

sw.bb:                                            ; preds = %entry
  tail call void @klee_set_rounding_mode(i32 0) #11, !dbg !268
  br label %return, !dbg !270

sw.bb1:                                           ; preds = %entry
  tail call void @klee_set_rounding_mode(i32 2) #11, !dbg !271
  br label %return, !dbg !272

sw.bb2:                                           ; preds = %entry
  tail call void @klee_set_rounding_mode(i32 3) #11, !dbg !273
  br label %return, !dbg !274

sw.bb3:                                           ; preds = %entry
  tail call void @klee_set_rounding_mode(i32 4) #11, !dbg !275
  br label %return, !dbg !276

return:                                           ; preds = %sw.bb3, %sw.bb2, %sw.bb1, %sw.bb, %entry
  %retval.0 = phi i32 [ -1, %entry ], [ 0, %sw.bb3 ], [ 0, %sw.bb2 ], [ 0, %sw.bb1 ], [ 0, %sw.bb ]
  ret i32 %retval.0, !dbg !277
}

; Function Attrs: nounwind uwtable
define i32 @klee_internal_isnanf(float %f) #8 {
entry:
  %call = tail call zeroext i1 @klee_is_nan_float(float %f) #11, !dbg !278
  %conv = zext i1 %call to i32, !dbg !278
  ret i32 %conv, !dbg !278
}

declare zeroext i1 @klee_is_nan_float(float) #5

; Function Attrs: nounwind uwtable
define i32 @klee_internal_isnan(double %d) #8 {
entry:
  %call = tail call zeroext i1 @klee_is_nan_double(double %d) #11, !dbg !279
  %conv = zext i1 %call to i32, !dbg !279
  ret i32 %conv, !dbg !279
}

declare zeroext i1 @klee_is_nan_double(double) #5

; Function Attrs: nounwind uwtable
define i32 @klee_internal_isnanl(x86_fp80 %d) #8 {
entry:
  %call = tail call zeroext i1 @klee_is_nan_long_double(x86_fp80 %d) #11, !dbg !280
  %conv = zext i1 %call to i32, !dbg !280
  ret i32 %conv, !dbg !280
}

declare zeroext i1 @klee_is_nan_long_double(x86_fp80) #5

; Function Attrs: nounwind uwtable
define i32 @klee_internal_fpclassifyf(float %f) #8 {
entry:
  %call = tail call zeroext i1 @klee_is_nan_float(float %f) #11, !dbg !281
  br i1 %call, label %return, label %if.else, !dbg !281

if.else:                                          ; preds = %entry
  %call1 = tail call zeroext i1 @klee_is_infinite_float(float %f) #11, !dbg !283
  br i1 %call1, label %return, label %if.else3, !dbg !283

if.else3:                                         ; preds = %if.else
  %cmp = fcmp oeq float %f, 0.000000e+00, !dbg !285
  br i1 %cmp, label %return, label %if.else5, !dbg !285

if.else5:                                         ; preds = %if.else3
  %call6 = tail call zeroext i1 @klee_is_normal_float(float %f) #11, !dbg !287
  %. = select i1 %call6, i32 4, i32 3, !dbg !289
  br label %return, !dbg !289

return:                                           ; preds = %if.else5, %if.else3, %if.else, %entry
  %retval.0 = phi i32 [ 0, %entry ], [ 1, %if.else ], [ 2, %if.else3 ], [ %., %if.else5 ]
  ret i32 %retval.0, !dbg !291
}

declare zeroext i1 @klee_is_normal_float(float) #5

; Function Attrs: nounwind uwtable
define i32 @klee_internal_fpclassify(double %f) #8 {
entry:
  %call = tail call zeroext i1 @klee_is_nan_double(double %f) #11, !dbg !292
  br i1 %call, label %return, label %if.else, !dbg !292

if.else:                                          ; preds = %entry
  %call1 = tail call zeroext i1 @klee_is_infinite_double(double %f) #11, !dbg !294
  br i1 %call1, label %return, label %if.else3, !dbg !294

if.else3:                                         ; preds = %if.else
  %cmp = fcmp oeq double %f, 0.000000e+00, !dbg !296
  br i1 %cmp, label %return, label %if.else5, !dbg !296

if.else5:                                         ; preds = %if.else3
  %call6 = tail call zeroext i1 @klee_is_normal_double(double %f) #11, !dbg !298
  %. = select i1 %call6, i32 4, i32 3, !dbg !300
  br label %return, !dbg !300

return:                                           ; preds = %if.else5, %if.else3, %if.else, %entry
  %retval.0 = phi i32 [ 0, %entry ], [ 1, %if.else ], [ 2, %if.else3 ], [ %., %if.else5 ]
  ret i32 %retval.0, !dbg !302
}

declare zeroext i1 @klee_is_normal_double(double) #5

; Function Attrs: nounwind uwtable
define i32 @klee_internal_fpclassifyl(x86_fp80 %ld) #8 {
entry:
  %call = tail call zeroext i1 @klee_is_nan_long_double(x86_fp80 %ld) #11, !dbg !303
  br i1 %call, label %return, label %if.else, !dbg !303

if.else:                                          ; preds = %entry
  %call1 = tail call zeroext i1 @klee_is_infinite_long_double(x86_fp80 %ld) #11, !dbg !305
  br i1 %call1, label %return, label %if.else3, !dbg !305

if.else3:                                         ; preds = %if.else
  %cmp = fcmp oeq x86_fp80 %ld, 0xK00000000000000000000, !dbg !307
  br i1 %cmp, label %return, label %if.else5, !dbg !307

if.else5:                                         ; preds = %if.else3
  %call6 = tail call zeroext i1 @klee_is_normal_long_double(x86_fp80 %ld) #11, !dbg !309
  %. = select i1 %call6, i32 4, i32 3, !dbg !311
  br label %return, !dbg !311

return:                                           ; preds = %if.else5, %if.else3, %if.else, %entry
  %retval.0 = phi i32 [ 0, %entry ], [ 1, %if.else ], [ 2, %if.else3 ], [ %., %if.else5 ]
  ret i32 %retval.0, !dbg !313
}

declare zeroext i1 @klee_is_normal_long_double(x86_fp80) #5

; Function Attrs: nounwind uwtable
define i32 @klee_internal_finitef(float %f) #8 {
entry:
  %call = tail call zeroext i1 @klee_is_nan_float(float %f) #11, !dbg !314
  %0 = zext i1 %call to i32, !dbg !314
  %lnot.ext = xor i32 %0, 1, !dbg !314
  %call1 = tail call zeroext i1 @klee_is_infinite_float(float %f) #11, !dbg !314
  %1 = zext i1 %call1 to i32, !dbg !314
  %lnot.ext3 = xor i32 %1, 1, !dbg !314
  %and = and i32 %lnot.ext3, %lnot.ext, !dbg !314
  ret i32 %and, !dbg !314
}

; Function Attrs: nounwind uwtable
define i32 @klee_internal_finite(double %f) #8 {
entry:
  %call = tail call zeroext i1 @klee_is_nan_double(double %f) #11, !dbg !315
  %0 = zext i1 %call to i32, !dbg !315
  %lnot.ext = xor i32 %0, 1, !dbg !315
  %call1 = tail call zeroext i1 @klee_is_infinite_double(double %f) #11, !dbg !315
  %1 = zext i1 %call1 to i32, !dbg !315
  %lnot.ext3 = xor i32 %1, 1, !dbg !315
  %and = and i32 %lnot.ext3, %lnot.ext, !dbg !315
  ret i32 %and, !dbg !315
}

; Function Attrs: nounwind uwtable
define i32 @klee_internal_finitel(x86_fp80 %f) #8 {
entry:
  %call = tail call zeroext i1 @klee_is_nan_long_double(x86_fp80 %f) #11, !dbg !316
  %0 = zext i1 %call to i32, !dbg !316
  %lnot.ext = xor i32 %0, 1, !dbg !316
  %call1 = tail call zeroext i1 @klee_is_infinite_long_double(x86_fp80 %f) #11, !dbg !316
  %1 = zext i1 %call1 to i32, !dbg !316
  %lnot.ext3 = xor i32 %1, 1, !dbg !316
  %and = and i32 %lnot.ext3, %lnot.ext, !dbg !316
  ret i32 %and, !dbg !316
}

; Function Attrs: nounwind uwtable
define void @klee_div_zero_check(i64 %z) #8 {
entry:
  %cmp = icmp eq i64 %z, 0, !dbg !317
  br i1 %cmp, label %if.then, label %if.end, !dbg !317

if.then:                                          ; preds = %entry
  tail call void @klee_report_error(i8* getelementptr inbounds ([56 x i8]* @.str3, i64 0, i64 0), i32 14, i8* getelementptr inbounds ([15 x i8]* @.str14, i64 0, i64 0), i8* getelementptr inbounds ([8 x i8]* @.str25, i64 0, i64 0)) #12, !dbg !319
  unreachable, !dbg !319

if.end:                                           ; preds = %entry
  ret void, !dbg !320
}

; Function Attrs: noreturn
declare void @klee_report_error(i8*, i32, i8*, i8*) #9

; Function Attrs: nounwind uwtable
define i32 @klee_int(i8* %name) #8 {
entry:
  %x = alloca i32, align 4
  %0 = bitcast i32* %x to i8*, !dbg !321
  call void @klee_make_symbolic(i8* %0, i64 4, i8* %name) #11, !dbg !321
  %1 = load i32* %x, align 4, !dbg !322, !tbaa !323
  ret i32 %1, !dbg !322
}

; Function Attrs: nounwind uwtable
define void @klee_overshift_check(i64 %bitWidth, i64 %shift) #8 {
entry:
  %cmp = icmp ult i64 %shift, %bitWidth, !dbg !327
  br i1 %cmp, label %if.end, label %if.then, !dbg !327

if.then:                                          ; preds = %entry
  tail call void @klee_report_error(i8* getelementptr inbounds ([8 x i8]* @.str36, i64 0, i64 0), i32 0, i8* getelementptr inbounds ([16 x i8]* @.str147, i64 0, i64 0), i8* getelementptr inbounds ([14 x i8]* @.str258, i64 0, i64 0)) #12, !dbg !329
  unreachable, !dbg !329

if.end:                                           ; preds = %entry
  ret void, !dbg !331
}

; Function Attrs: nounwind uwtable
define i32 @klee_range(i32 %start, i32 %end, i8* %name) #8 {
entry:
  %x = alloca i32, align 4
  %cmp = icmp slt i32 %start, %end, !dbg !332
  br i1 %cmp, label %if.end, label %if.then, !dbg !332

if.then:                                          ; preds = %entry
  call void @klee_report_error(i8* getelementptr inbounds ([47 x i8]* @.str6, i64 0, i64 0), i32 17, i8* getelementptr inbounds ([14 x i8]* @.str17, i64 0, i64 0), i8* getelementptr inbounds ([5 x i8]* @.str28, i64 0, i64 0)) #12, !dbg !334
  unreachable, !dbg !334

if.end:                                           ; preds = %entry
  %add = add nsw i32 %start, 1, !dbg !335
  %cmp1 = icmp eq i32 %add, %end, !dbg !335
  br i1 %cmp1, label %return, label %if.else, !dbg !335

if.else:                                          ; preds = %if.end
  %0 = bitcast i32* %x to i8*, !dbg !337
  call void @klee_make_symbolic(i8* %0, i64 4, i8* %name) #11, !dbg !337
  %cmp3 = icmp eq i32 %start, 0, !dbg !339
  %1 = load i32* %x, align 4, !dbg !341, !tbaa !323
  br i1 %cmp3, label %if.then4, label %if.else7, !dbg !339

if.then4:                                         ; preds = %if.else
  %cmp5 = icmp ult i32 %1, %end, !dbg !341
  %conv6 = zext i1 %cmp5 to i64, !dbg !341
  call void @klee_assume(i64 %conv6) #11, !dbg !341
  br label %if.end14, !dbg !343

if.else7:                                         ; preds = %if.else
  %cmp8 = icmp sge i32 %1, %start, !dbg !344
  %conv10 = zext i1 %cmp8 to i64, !dbg !344
  call void @klee_assume(i64 %conv10) #11, !dbg !344
  %2 = load i32* %x, align 4, !dbg !346, !tbaa !323
  %cmp11 = icmp slt i32 %2, %end, !dbg !346
  %conv13 = zext i1 %cmp11 to i64, !dbg !346
  call void @klee_assume(i64 %conv13) #11, !dbg !346
  br label %if.end14

if.end14:                                         ; preds = %if.else7, %if.then4
  %3 = load i32* %x, align 4, !dbg !347, !tbaa !323
  br label %return, !dbg !347

return:                                           ; preds = %if.end14, %if.end
  %retval.0 = phi i32 [ %3, %if.end14 ], [ %start, %if.end ]
  ret i32 %retval.0, !dbg !348
}

; Function Attrs: nounwind uwtable
define void @klee_set_rounding_mode(i32 %rm) #8 {
entry:
  switch i32 %rm, label %sw.default [
    i32 0, label %sw.bb
    i32 1, label %sw.bb1
    i32 2, label %sw.bb2
    i32 3, label %sw.bb3
    i32 4, label %sw.bb4
  ], !dbg !349

sw.bb:                                            ; preds = %entry
  tail call void @klee_set_rounding_mode_internal(i32 0) #11, !dbg !350
  br label %sw.epilog, !dbg !350

sw.bb1:                                           ; preds = %entry
  tail call void @klee_set_rounding_mode_internal(i32 1) #11, !dbg !352
  br label %sw.epilog, !dbg !352

sw.bb2:                                           ; preds = %entry
  tail call void @klee_set_rounding_mode_internal(i32 2) #11, !dbg !353
  br label %sw.epilog, !dbg !353

sw.bb3:                                           ; preds = %entry
  tail call void @klee_set_rounding_mode_internal(i32 3) #11, !dbg !354
  br label %sw.epilog, !dbg !354

sw.bb4:                                           ; preds = %entry
  tail call void @klee_set_rounding_mode_internal(i32 4) #11, !dbg !355
  br label %sw.epilog, !dbg !355

sw.default:                                       ; preds = %entry
  tail call void @klee_report_error(i8* getelementptr inbounds ([59 x i8]* @.str9, i64 0, i64 0), i32 31, i8* getelementptr inbounds ([22 x i8]* @.str110, i64 0, i64 0), i8* getelementptr inbounds ([1 x i8]* @.str211, i64 0, i64 0)) #12, !dbg !356
  unreachable, !dbg !356

sw.epilog:                                        ; preds = %sw.bb4, %sw.bb3, %sw.bb2, %sw.bb1, %sw.bb
  ret void, !dbg !357
}

declare void @klee_set_rounding_mode_internal(i32) #5

; Function Attrs: nounwind uwtable
define weak i8* @memcpy(i8* %destaddr, i8* %srcaddr, i64 %len) #8 {
entry:
  %cmp3 = icmp eq i64 %len, 0, !dbg !358
  br i1 %cmp3, label %while.end, label %while.body.preheader, !dbg !358

while.body.preheader:                             ; preds = %entry
  %n.vec = and i64 %len, -32
  %cmp.zero = icmp eq i64 %n.vec, 0
  %0 = add i64 %len, -1
  br i1 %cmp.zero, label %middle.block, label %vector.memcheck

vector.memcheck:                                  ; preds = %while.body.preheader
  %scevgep7 = getelementptr i8* %srcaddr, i64 %0
  %scevgep = getelementptr i8* %destaddr, i64 %0
  %bound1 = icmp uge i8* %scevgep, %srcaddr
  %bound0 = icmp uge i8* %scevgep7, %destaddr
  %memcheck.conflict = and i1 %bound0, %bound1
  %ptr.ind.end = getelementptr i8* %srcaddr, i64 %n.vec
  %ptr.ind.end9 = getelementptr i8* %destaddr, i64 %n.vec
  %rev.ind.end = sub i64 %len, %n.vec
  br i1 %memcheck.conflict, label %middle.block, label %vector.body

vector.body:                                      ; preds = %vector.body, %vector.memcheck
  %index = phi i64 [ %index.next, %vector.body ], [ 0, %vector.memcheck ]
  %next.gep = getelementptr i8* %srcaddr, i64 %index
  %next.gep106 = getelementptr i8* %destaddr, i64 %index
  %1 = bitcast i8* %next.gep to <16 x i8>*, !dbg !359
  %wide.load = load <16 x i8>* %1, align 1, !dbg !359
  %next.gep.sum282 = or i64 %index, 16, !dbg !359
  %2 = getelementptr i8* %srcaddr, i64 %next.gep.sum282, !dbg !359
  %3 = bitcast i8* %2 to <16 x i8>*, !dbg !359
  %wide.load203 = load <16 x i8>* %3, align 1, !dbg !359
  %4 = bitcast i8* %next.gep106 to <16 x i8>*, !dbg !359
  store <16 x i8> %wide.load, <16 x i8>* %4, align 1, !dbg !359
  %next.gep106.sum299 = or i64 %index, 16, !dbg !359
  %5 = getelementptr i8* %destaddr, i64 %next.gep106.sum299, !dbg !359
  %6 = bitcast i8* %5 to <16 x i8>*, !dbg !359
  store <16 x i8> %wide.load203, <16 x i8>* %6, align 1, !dbg !359
  %index.next = add i64 %index, 32
  %7 = icmp eq i64 %index.next, %n.vec
  br i1 %7, label %middle.block, label %vector.body, !llvm.loop !360

middle.block:                                     ; preds = %vector.body, %vector.memcheck, %while.body.preheader
  %resume.val = phi i8* [ %srcaddr, %while.body.preheader ], [ %srcaddr, %vector.memcheck ], [ %ptr.ind.end, %vector.body ]
  %resume.val8 = phi i8* [ %destaddr, %while.body.preheader ], [ %destaddr, %vector.memcheck ], [ %ptr.ind.end9, %vector.body ]
  %resume.val10 = phi i64 [ %len, %while.body.preheader ], [ %len, %vector.memcheck ], [ %rev.ind.end, %vector.body ]
  %new.indc.resume.val = phi i64 [ 0, %while.body.preheader ], [ 0, %vector.memcheck ], [ %n.vec, %vector.body ]
  %cmp.n = icmp eq i64 %new.indc.resume.val, %len
  br i1 %cmp.n, label %while.end, label %while.body

while.body:                                       ; preds = %while.body, %middle.block
  %src.06 = phi i8* [ %incdec.ptr, %while.body ], [ %resume.val, %middle.block ]
  %dest.05 = phi i8* [ %incdec.ptr1, %while.body ], [ %resume.val8, %middle.block ]
  %len.addr.04 = phi i64 [ %dec, %while.body ], [ %resume.val10, %middle.block ]
  %dec = add i64 %len.addr.04, -1, !dbg !358
  %incdec.ptr = getelementptr inbounds i8* %src.06, i64 1, !dbg !359
  %8 = load i8* %src.06, align 1, !dbg !359, !tbaa !363
  %incdec.ptr1 = getelementptr inbounds i8* %dest.05, i64 1, !dbg !359
  store i8 %8, i8* %dest.05, align 1, !dbg !359, !tbaa !363
  %cmp = icmp eq i64 %dec, 0, !dbg !358
  br i1 %cmp, label %while.end, label %while.body, !dbg !358, !llvm.loop !364

while.end:                                        ; preds = %while.body, %middle.block, %entry
  ret i8* %destaddr, !dbg !365
}

; Function Attrs: nounwind uwtable
define weak i8* @memmove(i8* %dst, i8* %src, i64 %count) #8 {
entry:
  %cmp = icmp eq i8* %src, %dst, !dbg !366
  br i1 %cmp, label %return, label %if.end, !dbg !366

if.end:                                           ; preds = %entry
  %cmp1 = icmp ugt i8* %src, %dst, !dbg !368
  br i1 %cmp1, label %while.cond.preheader, label %if.else, !dbg !368

while.cond.preheader:                             ; preds = %if.end
  %tobool27 = icmp eq i64 %count, 0, !dbg !370
  br i1 %tobool27, label %return, label %while.body.preheader, !dbg !370

while.body.preheader:                             ; preds = %while.cond.preheader
  %n.vec = and i64 %count, -32
  %cmp.zero = icmp eq i64 %n.vec, 0
  %0 = add i64 %count, -1
  br i1 %cmp.zero, label %middle.block, label %vector.memcheck

vector.memcheck:                                  ; preds = %while.body.preheader
  %scevgep37 = getelementptr i8* %src, i64 %0
  %scevgep = getelementptr i8* %dst, i64 %0
  %bound1 = icmp uge i8* %scevgep, %src
  %bound0 = icmp uge i8* %scevgep37, %dst
  %memcheck.conflict = and i1 %bound0, %bound1
  %ptr.ind.end = getelementptr i8* %src, i64 %n.vec
  %ptr.ind.end39 = getelementptr i8* %dst, i64 %n.vec
  %rev.ind.end = sub i64 %count, %n.vec
  br i1 %memcheck.conflict, label %middle.block, label %vector.body

vector.body:                                      ; preds = %vector.body, %vector.memcheck
  %index = phi i64 [ %index.next, %vector.body ], [ 0, %vector.memcheck ]
  %next.gep = getelementptr i8* %src, i64 %index
  %next.gep136 = getelementptr i8* %dst, i64 %index
  %1 = bitcast i8* %next.gep to <16 x i8>*, !dbg !370
  %wide.load = load <16 x i8>* %1, align 1, !dbg !370
  %next.gep.sum610 = or i64 %index, 16, !dbg !370
  %2 = getelementptr i8* %src, i64 %next.gep.sum610, !dbg !370
  %3 = bitcast i8* %2 to <16 x i8>*, !dbg !370
  %wide.load233 = load <16 x i8>* %3, align 1, !dbg !370
  %4 = bitcast i8* %next.gep136 to <16 x i8>*, !dbg !370
  store <16 x i8> %wide.load, <16 x i8>* %4, align 1, !dbg !370
  %next.gep136.sum627 = or i64 %index, 16, !dbg !370
  %5 = getelementptr i8* %dst, i64 %next.gep136.sum627, !dbg !370
  %6 = bitcast i8* %5 to <16 x i8>*, !dbg !370
  store <16 x i8> %wide.load233, <16 x i8>* %6, align 1, !dbg !370
  %index.next = add i64 %index, 32
  %7 = icmp eq i64 %index.next, %n.vec
  br i1 %7, label %middle.block, label %vector.body, !llvm.loop !372

middle.block:                                     ; preds = %vector.body, %vector.memcheck, %while.body.preheader
  %resume.val = phi i8* [ %src, %while.body.preheader ], [ %src, %vector.memcheck ], [ %ptr.ind.end, %vector.body ]
  %resume.val38 = phi i8* [ %dst, %while.body.preheader ], [ %dst, %vector.memcheck ], [ %ptr.ind.end39, %vector.body ]
  %resume.val40 = phi i64 [ %count, %while.body.preheader ], [ %count, %vector.memcheck ], [ %rev.ind.end, %vector.body ]
  %new.indc.resume.val = phi i64 [ 0, %while.body.preheader ], [ 0, %vector.memcheck ], [ %n.vec, %vector.body ]
  %cmp.n = icmp eq i64 %new.indc.resume.val, %count
  br i1 %cmp.n, label %return, label %while.body

while.body:                                       ; preds = %while.body, %middle.block
  %b.030 = phi i8* [ %incdec.ptr, %while.body ], [ %resume.val, %middle.block ]
  %a.029 = phi i8* [ %incdec.ptr3, %while.body ], [ %resume.val38, %middle.block ]
  %count.addr.028 = phi i64 [ %dec, %while.body ], [ %resume.val40, %middle.block ]
  %dec = add i64 %count.addr.028, -1, !dbg !370
  %incdec.ptr = getelementptr inbounds i8* %b.030, i64 1, !dbg !370
  %8 = load i8* %b.030, align 1, !dbg !370, !tbaa !363
  %incdec.ptr3 = getelementptr inbounds i8* %a.029, i64 1, !dbg !370
  store i8 %8, i8* %a.029, align 1, !dbg !370, !tbaa !363
  %tobool = icmp eq i64 %dec, 0, !dbg !370
  br i1 %tobool, label %return, label %while.body, !dbg !370, !llvm.loop !373

if.else:                                          ; preds = %if.end
  %sub = add i64 %count, -1, !dbg !374
  %tobool832 = icmp eq i64 %count, 0, !dbg !376
  br i1 %tobool832, label %return, label %while.body9.lr.ph, !dbg !376

while.body9.lr.ph:                                ; preds = %if.else
  %add.ptr5 = getelementptr inbounds i8* %src, i64 %sub, !dbg !377
  %add.ptr = getelementptr inbounds i8* %dst, i64 %sub, !dbg !374
  %n.vec241 = and i64 %count, -32
  %cmp.zero243 = icmp eq i64 %n.vec241, 0
  %9 = add i64 %count, -1
  br i1 %cmp.zero243, label %middle.block236, label %vector.memcheck250

vector.memcheck250:                               ; preds = %while.body9.lr.ph
  %scevgep245 = getelementptr i8* %src, i64 %9
  %scevgep244 = getelementptr i8* %dst, i64 %9
  %bound1247 = icmp ule i8* %scevgep245, %dst
  %bound0246 = icmp ule i8* %scevgep244, %src
  %memcheck.conflict249 = and i1 %bound0246, %bound1247
  %add.ptr5.sum = sub i64 %sub, %n.vec241
  %rev.ptr.ind.end = getelementptr i8* %src, i64 %add.ptr5.sum
  %add.ptr.sum = sub i64 %sub, %n.vec241
  %rev.ptr.ind.end255 = getelementptr i8* %dst, i64 %add.ptr.sum
  %rev.ind.end257 = sub i64 %count, %n.vec241
  br i1 %memcheck.conflict249, label %middle.block236, label %vector.body235

vector.body235:                                   ; preds = %vector.body235, %vector.memcheck250
  %index238 = phi i64 [ %index.next260, %vector.body235 ], [ 0, %vector.memcheck250 ]
  %add.ptr5.sum465 = sub i64 %sub, %index238
  %add.ptr.sum497 = sub i64 %sub, %index238
  %next.gep262.sum = add i64 %add.ptr5.sum465, -15, !dbg !376
  %10 = getelementptr i8* %src, i64 %next.gep262.sum, !dbg !376
  %11 = bitcast i8* %10 to <16 x i8>*, !dbg !376
  %wide.load460 = load <16 x i8>* %11, align 1, !dbg !376
  %reverse = shufflevector <16 x i8> %wide.load460, <16 x i8> undef, <16 x i32> <i32 15, i32 14, i32 13, i32 12, i32 11, i32 10, i32 9, i32 8, i32 7, i32 6, i32 5, i32 4, i32 3, i32 2, i32 1, i32 0>, !dbg !376
  %.sum = add i64 %add.ptr5.sum465, -31, !dbg !376
  %12 = getelementptr i8* %src, i64 %.sum, !dbg !376
  %13 = bitcast i8* %12 to <16 x i8>*, !dbg !376
  %wide.load461 = load <16 x i8>* %13, align 1, !dbg !376
  %reverse462 = shufflevector <16 x i8> %wide.load461, <16 x i8> undef, <16 x i32> <i32 15, i32 14, i32 13, i32 12, i32 11, i32 10, i32 9, i32 8, i32 7, i32 6, i32 5, i32 4, i32 3, i32 2, i32 1, i32 0>, !dbg !376
  %reverse463 = shufflevector <16 x i8> %reverse, <16 x i8> undef, <16 x i32> <i32 15, i32 14, i32 13, i32 12, i32 11, i32 10, i32 9, i32 8, i32 7, i32 6, i32 5, i32 4, i32 3, i32 2, i32 1, i32 0>, !dbg !376
  %next.gep359.sum = add i64 %add.ptr.sum497, -15, !dbg !376
  %14 = getelementptr i8* %dst, i64 %next.gep359.sum, !dbg !376
  %15 = bitcast i8* %14 to <16 x i8>*, !dbg !376
  store <16 x i8> %reverse463, <16 x i8>* %15, align 1, !dbg !376
  %reverse464 = shufflevector <16 x i8> %reverse462, <16 x i8> undef, <16 x i32> <i32 15, i32 14, i32 13, i32 12, i32 11, i32 10, i32 9, i32 8, i32 7, i32 6, i32 5, i32 4, i32 3, i32 2, i32 1, i32 0>, !dbg !376
  %.sum531 = add i64 %add.ptr.sum497, -31, !dbg !376
  %16 = getelementptr i8* %dst, i64 %.sum531, !dbg !376
  %17 = bitcast i8* %16 to <16 x i8>*, !dbg !376
  store <16 x i8> %reverse464, <16 x i8>* %17, align 1, !dbg !376
  %index.next260 = add i64 %index238, 32
  %18 = icmp eq i64 %index.next260, %n.vec241
  br i1 %18, label %middle.block236, label %vector.body235, !llvm.loop !378

middle.block236:                                  ; preds = %vector.body235, %vector.memcheck250, %while.body9.lr.ph
  %resume.val251 = phi i8* [ %add.ptr5, %while.body9.lr.ph ], [ %add.ptr5, %vector.memcheck250 ], [ %rev.ptr.ind.end, %vector.body235 ]
  %resume.val253 = phi i8* [ %add.ptr, %while.body9.lr.ph ], [ %add.ptr, %vector.memcheck250 ], [ %rev.ptr.ind.end255, %vector.body235 ]
  %resume.val256 = phi i64 [ %count, %while.body9.lr.ph ], [ %count, %vector.memcheck250 ], [ %rev.ind.end257, %vector.body235 ]
  %new.indc.resume.val258 = phi i64 [ 0, %while.body9.lr.ph ], [ 0, %vector.memcheck250 ], [ %n.vec241, %vector.body235 ]
  %cmp.n259 = icmp eq i64 %new.indc.resume.val258, %count
  br i1 %cmp.n259, label %return, label %while.body9

while.body9:                                      ; preds = %while.body9, %middle.block236
  %b.135 = phi i8* [ %incdec.ptr10, %while.body9 ], [ %resume.val251, %middle.block236 ]
  %a.134 = phi i8* [ %incdec.ptr11, %while.body9 ], [ %resume.val253, %middle.block236 ]
  %count.addr.133 = phi i64 [ %dec7, %while.body9 ], [ %resume.val256, %middle.block236 ]
  %dec7 = add i64 %count.addr.133, -1, !dbg !376
  %incdec.ptr10 = getelementptr inbounds i8* %b.135, i64 -1, !dbg !376
  %19 = load i8* %b.135, align 1, !dbg !376, !tbaa !363
  %incdec.ptr11 = getelementptr inbounds i8* %a.134, i64 -1, !dbg !376
  store i8 %19, i8* %a.134, align 1, !dbg !376, !tbaa !363
  %tobool8 = icmp eq i64 %dec7, 0, !dbg !376
  br i1 %tobool8, label %return, label %while.body9, !dbg !376, !llvm.loop !379

return:                                           ; preds = %while.body9, %middle.block236, %if.else, %while.body, %middle.block, %while.cond.preheader, %entry
  ret i8* %dst, !dbg !380
}

; Function Attrs: nounwind uwtable
define weak i8* @mempcpy(i8* %destaddr, i8* %srcaddr, i64 %len) #8 {
entry:
  %cmp3 = icmp eq i64 %len, 0, !dbg !381
  br i1 %cmp3, label %while.end, label %while.body.preheader, !dbg !381

while.body.preheader:                             ; preds = %entry
  %n.vec = and i64 %len, -32
  %cmp.zero = icmp eq i64 %n.vec, 0
  %0 = add i64 %len, -1
  br i1 %cmp.zero, label %middle.block, label %vector.memcheck

vector.memcheck:                                  ; preds = %while.body.preheader
  %scevgep8 = getelementptr i8* %srcaddr, i64 %0
  %scevgep7 = getelementptr i8* %destaddr, i64 %0
  %bound1 = icmp uge i8* %scevgep7, %srcaddr
  %bound0 = icmp uge i8* %scevgep8, %destaddr
  %memcheck.conflict = and i1 %bound0, %bound1
  %ptr.ind.end = getelementptr i8* %srcaddr, i64 %n.vec
  %ptr.ind.end10 = getelementptr i8* %destaddr, i64 %n.vec
  %rev.ind.end = sub i64 %len, %n.vec
  br i1 %memcheck.conflict, label %middle.block, label %vector.body

vector.body:                                      ; preds = %vector.body, %vector.memcheck
  %index = phi i64 [ %index.next, %vector.body ], [ 0, %vector.memcheck ]
  %next.gep = getelementptr i8* %srcaddr, i64 %index
  %next.gep107 = getelementptr i8* %destaddr, i64 %index
  %1 = bitcast i8* %next.gep to <16 x i8>*, !dbg !382
  %wide.load = load <16 x i8>* %1, align 1, !dbg !382
  %next.gep.sum283 = or i64 %index, 16, !dbg !382
  %2 = getelementptr i8* %srcaddr, i64 %next.gep.sum283, !dbg !382
  %3 = bitcast i8* %2 to <16 x i8>*, !dbg !382
  %wide.load204 = load <16 x i8>* %3, align 1, !dbg !382
  %4 = bitcast i8* %next.gep107 to <16 x i8>*, !dbg !382
  store <16 x i8> %wide.load, <16 x i8>* %4, align 1, !dbg !382
  %next.gep107.sum300 = or i64 %index, 16, !dbg !382
  %5 = getelementptr i8* %destaddr, i64 %next.gep107.sum300, !dbg !382
  %6 = bitcast i8* %5 to <16 x i8>*, !dbg !382
  store <16 x i8> %wide.load204, <16 x i8>* %6, align 1, !dbg !382
  %index.next = add i64 %index, 32
  %7 = icmp eq i64 %index.next, %n.vec
  br i1 %7, label %middle.block, label %vector.body, !llvm.loop !383

middle.block:                                     ; preds = %vector.body, %vector.memcheck, %while.body.preheader
  %resume.val = phi i8* [ %srcaddr, %while.body.preheader ], [ %srcaddr, %vector.memcheck ], [ %ptr.ind.end, %vector.body ]
  %resume.val9 = phi i8* [ %destaddr, %while.body.preheader ], [ %destaddr, %vector.memcheck ], [ %ptr.ind.end10, %vector.body ]
  %resume.val11 = phi i64 [ %len, %while.body.preheader ], [ %len, %vector.memcheck ], [ %rev.ind.end, %vector.body ]
  %new.indc.resume.val = phi i64 [ 0, %while.body.preheader ], [ 0, %vector.memcheck ], [ %n.vec, %vector.body ]
  %cmp.n = icmp eq i64 %new.indc.resume.val, %len
  br i1 %cmp.n, label %while.cond.while.end_crit_edge, label %while.body

while.body:                                       ; preds = %while.body, %middle.block
  %src.06 = phi i8* [ %incdec.ptr, %while.body ], [ %resume.val, %middle.block ]
  %dest.05 = phi i8* [ %incdec.ptr1, %while.body ], [ %resume.val9, %middle.block ]
  %len.addr.04 = phi i64 [ %dec, %while.body ], [ %resume.val11, %middle.block ]
  %dec = add i64 %len.addr.04, -1, !dbg !381
  %incdec.ptr = getelementptr inbounds i8* %src.06, i64 1, !dbg !382
  %8 = load i8* %src.06, align 1, !dbg !382, !tbaa !363
  %incdec.ptr1 = getelementptr inbounds i8* %dest.05, i64 1, !dbg !382
  store i8 %8, i8* %dest.05, align 1, !dbg !382, !tbaa !363
  %cmp = icmp eq i64 %dec, 0, !dbg !381
  br i1 %cmp, label %while.cond.while.end_crit_edge, label %while.body, !dbg !381, !llvm.loop !384

while.cond.while.end_crit_edge:                   ; preds = %while.body, %middle.block
  %scevgep = getelementptr i8* %destaddr, i64 %len
  br label %while.end, !dbg !381

while.end:                                        ; preds = %while.cond.while.end_crit_edge, %entry
  %dest.0.lcssa = phi i8* [ %scevgep, %while.cond.while.end_crit_edge ], [ %destaddr, %entry ]
  ret i8* %dest.0.lcssa, !dbg !385
}

; Function Attrs: nounwind uwtable
define weak i8* @memset(i8* %dst, i32 %s, i64 %count) #8 {
entry:
  %cmp2 = icmp eq i64 %count, 0, !dbg !386
  br i1 %cmp2, label %while.end, label %while.body.lr.ph, !dbg !386

while.body.lr.ph:                                 ; preds = %entry
  %conv = trunc i32 %s to i8, !dbg !387
  br label %while.body, !dbg !386

while.body:                                       ; preds = %while.body, %while.body.lr.ph
  %a.04 = phi i8* [ %dst, %while.body.lr.ph ], [ %incdec.ptr, %while.body ]
  %count.addr.03 = phi i64 [ %count, %while.body.lr.ph ], [ %dec, %while.body ]
  %dec = add i64 %count.addr.03, -1, !dbg !386
  %incdec.ptr = getelementptr inbounds i8* %a.04, i64 1, !dbg !387
  store volatile i8 %conv, i8* %a.04, align 1, !dbg !387, !tbaa !363
  %cmp = icmp eq i64 %dec, 0, !dbg !386
  br i1 %cmp, label %while.end, label %while.body, !dbg !386

while.end:                                        ; preds = %while.body, %entry
  ret i8* %dst, !dbg !388
}

; Function Attrs: nounwind uwtable
define double @klee_internal_sqrt(double %d) #8 {
entry:
  %call = tail call double @klee_sqrt_double(double %d) #11, !dbg !389
  ret double %call, !dbg !389
}

declare double @klee_sqrt_double(double) #5

; Function Attrs: nounwind uwtable
define float @klee_internal_sqrtf(float %f) #8 {
entry:
  %call = tail call float @klee_sqrt_float(float %f) #11, !dbg !390
  ret float %call, !dbg !390
}

declare float @klee_sqrt_float(float) #5

; Function Attrs: nounwind uwtable
define x86_fp80 @klee_internal_sqrtl(x86_fp80 %f) #8 {
entry:
  %call = tail call x86_fp80 @klee_sqrt_long_double(x86_fp80 %f) #11, !dbg !391
  ret x86_fp80 %call, !dbg !391
}

declare x86_fp80 @klee_sqrt_long_double(x86_fp80) #5

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float
attributes #1 = { nounwind readnone }
attributes #2 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false
attributes #3 = { noreturn "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false
attributes #4 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { noinline nounwind optnone uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #7 = { noinline optnone }
attributes #8 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #9 = { noreturn "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #10 = { nounwind }
attributes #11 = { nobuiltin nounwind }
attributes #12 = { nobuiltin noreturn nounwind }

!llvm.dbg.cu = !{!0, !17, !39, !70, !115, !125, !137, !148, !160, !170, !188, !202, !216, !231}
!llvm.module.flags = !{!244, !245}
!llvm.ident = !{!246, !246, !246, !246, !246, !246, !246, !246, !246, !246, !246, !246, !246, !246}

!0 = metadata !{i32 786449, metadata !1, i32 12, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 false, met
!1 = metadata !{metadata !"heap_bo_l1_klee_annotated.c", metadata !"/home/klee/logic_bombs/src/buffer_overflow"}
!2 = metadata !{i32 0}
!3 = metadata !{metadata !4, metadata !11}
!4 = metadata !{i32 786478, metadata !1, metadata !5, metadata !"logic_bomb", metadata !"logic_bomb", metadata !"", i32 9, metadata !6, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, i32 (i8*)* @logic_bomb, null, null, metadata !2, i32 9} ; [ 
!5 = metadata !{i32 786473, metadata !1}          ; [ DW_TAG_file_type ] [/home/klee/logic_bombs/src/buffer_overflow/heap_bo_l1_klee_annotated.c]
!6 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !7, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!7 = metadata !{metadata !8, metadata !9}
!8 = metadata !{i32 786468, null, null, metadata !"int", i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ] [int] [line 0, size 32, align 32, offset 0, enc DW_ATE_signed]
!9 = metadata !{i32 786447, null, null, metadata !"", i32 0, i64 64, i64 64, i64 0, i32 0, metadata !10} ; [ DW_TAG_pointer_type ] [line 0, size 64, align 64, offset 0] [from char]
!10 = metadata !{i32 786468, null, null, metadata !"char", i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ] [char] [line 0, size 8, align 8, offset 0, enc DW_ATE_signed_char]
!11 = metadata !{i32 786478, metadata !1, metadata !5, metadata !"main", metadata !"main", metadata !"", i32 24, metadata !12, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, i32 (i32, i8**)* @main, null, null, metadata !2, i32 24} ; [ DW_TAG_s
!12 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !13, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!13 = metadata !{metadata !8, metadata !8, metadata !14}
!14 = metadata !{i32 786447, null, null, metadata !"", i32 0, i64 64, i64 64, i64 0, i32 0, metadata !9} ; [ DW_TAG_pointer_type ] [line 0, size 64, align 64, offset 0] [from ]
!15 = metadata !{metadata !16}
!16 = metadata !{i32 786484, i32 0, null, metadata !"n", metadata !"n", metadata !"", metadata !5, i32 6, metadata !8, i32 0, i32 1, i32* @n, null} ; [ DW_TAG_variable ] [n] [line 6] [def]
!17 = metadata !{i32 786449, metadata !18, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, met
!18 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/fabs.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!19 = metadata !{metadata !20, metadata !27, metadata !33}
!20 = metadata !{i32 786478, metadata !18, metadata !21, metadata !"klee_internal_fabs", metadata !"klee_internal_fabs", metadata !"", i32 11, metadata !22, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, double (double)* @klee_internal_fabs, nu
!21 = metadata !{i32 786473, metadata !18}        ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fabs.c]
!22 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !23, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!23 = metadata !{metadata !24, metadata !24}
!24 = metadata !{i32 786468, null, null, metadata !"double", i32 0, i64 64, i64 64, i64 0, i32 0, i32 4} ; [ DW_TAG_base_type ] [double] [line 0, size 64, align 64, offset 0, enc DW_ATE_float]
!25 = metadata !{metadata !26}
!26 = metadata !{i32 786689, metadata !20, metadata !"d", metadata !21, i32 16777227, metadata !24, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [d] [line 11]
!27 = metadata !{i32 786478, metadata !18, metadata !21, metadata !"klee_internal_fabsf", metadata !"klee_internal_fabsf", metadata !"", i32 15, metadata !28, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, float (float)* @klee_internal_fabsf, n
!28 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !29, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!29 = metadata !{metadata !30, metadata !30}
!30 = metadata !{i32 786468, null, null, metadata !"float", i32 0, i64 32, i64 32, i64 0, i32 0, i32 4} ; [ DW_TAG_base_type ] [float] [line 0, size 32, align 32, offset 0, enc DW_ATE_float]
!31 = metadata !{metadata !32}
!32 = metadata !{i32 786689, metadata !27, metadata !"f", metadata !21, i32 16777231, metadata !30, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 15]
!33 = metadata !{i32 786478, metadata !18, metadata !21, metadata !"klee_internal_fabsl", metadata !"klee_internal_fabsl", metadata !"", i32 20, metadata !34, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, x86_fp80 (x86_fp80)* @klee_internal_fa
!34 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !35, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!35 = metadata !{metadata !36, metadata !36}
!36 = metadata !{i32 786468, null, null, metadata !"long double", i32 0, i64 128, i64 128, i64 0, i32 0, i32 4} ; [ DW_TAG_base_type ] [long double] [line 0, size 128, align 128, offset 0, enc DW_ATE_float]
!37 = metadata !{metadata !38}
!38 = metadata !{i32 786689, metadata !33, metadata !"f", metadata !21, i32 16777236, metadata !36, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 20]
!39 = metadata !{i32 786449, metadata !40, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, met
!40 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/fenv.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!41 = metadata !{metadata !42, metadata !51}
!42 = metadata !{i32 786436, metadata !43, null, metadata !"KleeRoundingMode", i32 185, i64 32, i64 32, i32 0, i32 0, null, metadata !44, i32 0, null, null, null} ; [ DW_TAG_enumeration_type ] [KleeRoundingMode] [line 185, size 32, align 32, offset 0] [d
!43 = metadata !{metadata !"/home/user/klee/include/klee/klee.h", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!44 = metadata !{metadata !45, metadata !46, metadata !47, metadata !48, metadata !49, metadata !50}
!45 = metadata !{i32 786472, metadata !"KLEE_FP_RNE", i64 0} ; [ DW_TAG_enumerator ] [KLEE_FP_RNE :: 0]
!46 = metadata !{i32 786472, metadata !"KLEE_FP_RNA", i64 1} ; [ DW_TAG_enumerator ] [KLEE_FP_RNA :: 1]
!47 = metadata !{i32 786472, metadata !"KLEE_FP_RU", i64 2} ; [ DW_TAG_enumerator ] [KLEE_FP_RU :: 2]
!48 = metadata !{i32 786472, metadata !"KLEE_FP_RD", i64 3} ; [ DW_TAG_enumerator ] [KLEE_FP_RD :: 3]
!49 = metadata !{i32 786472, metadata !"KLEE_FP_RZ", i64 4} ; [ DW_TAG_enumerator ] [KLEE_FP_RZ :: 4]
!50 = metadata !{i32 786472, metadata !"KLEE_FP_UNKNOWN", i64 5} ; [ DW_TAG_enumerator ] [KLEE_FP_UNKNOWN :: 5]
!51 = metadata !{i32 786436, metadata !40, null, metadata !"", i32 15, i64 32, i64 32, i32 0, i32 0, null, metadata !52, i32 0, null, null, null} ; [ DW_TAG_enumeration_type ] [line 15, size 32, align 32, offset 0] [def] [from ]
!52 = metadata !{metadata !53, metadata !54, metadata !55, metadata !56, metadata !57}
!53 = metadata !{i32 786472, metadata !"FE_TONEAREST", i64 0} ; [ DW_TAG_enumerator ] [FE_TONEAREST :: 0]
!54 = metadata !{i32 786472, metadata !"FE_DOWNWARD", i64 1024} ; [ DW_TAG_enumerator ] [FE_DOWNWARD :: 1024]
!55 = metadata !{i32 786472, metadata !"FE_UPWARD", i64 2048} ; [ DW_TAG_enumerator ] [FE_UPWARD :: 2048]
!56 = metadata !{i32 786472, metadata !"FE_TOWARDZERO", i64 3072} ; [ DW_TAG_enumerator ] [FE_TOWARDZERO :: 3072]
!57 = metadata !{i32 786472, metadata !"FE_TONEAREST_TIES_TO_AWAY", i64 3073} ; [ DW_TAG_enumerator ] [FE_TONEAREST_TIES_TO_AWAY :: 3073]
!58 = metadata !{metadata !59, metadata !65}
!59 = metadata !{i32 786478, metadata !40, metadata !60, metadata !"klee_internal_fegetround", metadata !"klee_internal_fegetround", metadata !"", i32 33, metadata !61, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 ()* @klee_internal_feget
!60 = metadata !{i32 786473, metadata !40}        ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fenv.c]
!61 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !62, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!62 = metadata !{metadata !8}
!63 = metadata !{metadata !64}
!64 = metadata !{i32 786688, metadata !59, metadata !"rm", metadata !60, i32 34, metadata !42, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [rm] [line 34]
!65 = metadata !{i32 786478, metadata !40, metadata !60, metadata !"klee_internal_fesetround", metadata !"klee_internal_fesetround", metadata !"", i32 52, metadata !66, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (i32)* @klee_internal_fe
!66 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !67, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!67 = metadata !{metadata !8, metadata !8}
!68 = metadata !{metadata !69}
!69 = metadata !{i32 786689, metadata !65, metadata !"rm", metadata !60, i32 16777268, metadata !8, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [rm] [line 52]
!70 = metadata !{i32 786449, metadata !71, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, met
!71 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/fpclassify.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!72 = metadata !{metadata !73}
!73 = metadata !{i32 786436, metadata !71, null, metadata !"", i32 58, i64 32, i64 32, i32 0, i32 0, null, metadata !74, i32 0, null, null, null} ; [ DW_TAG_enumeration_type ] [line 58, size 32, align 32, offset 0] [def] [from ]
!74 = metadata !{metadata !75, metadata !76, metadata !77, metadata !78, metadata !79}
!75 = metadata !{i32 786472, metadata !"FP_NAN", i64 0} ; [ DW_TAG_enumerator ] [FP_NAN :: 0]
!76 = metadata !{i32 786472, metadata !"FP_INFINITE", i64 1} ; [ DW_TAG_enumerator ] [FP_INFINITE :: 1]
!77 = metadata !{i32 786472, metadata !"FP_ZERO", i64 2} ; [ DW_TAG_enumerator ] [FP_ZERO :: 2]
!78 = metadata !{i32 786472, metadata !"FP_SUBNORMAL", i64 3} ; [ DW_TAG_enumerator ] [FP_SUBNORMAL :: 3]
!79 = metadata !{i32 786472, metadata !"FP_NORMAL", i64 4} ; [ DW_TAG_enumerator ] [FP_NORMAL :: 4]
!80 = metadata !{metadata !81, metadata !87, metadata !92, metadata !97, metadata !100, metadata !103, metadata !106, metadata !109, metadata !112}
!81 = metadata !{i32 786478, metadata !71, metadata !82, metadata !"klee_internal_isnanf", metadata !"klee_internal_isnanf", metadata !"", i32 16, metadata !83, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (float)* @klee_internal_isnanf, 
!82 = metadata !{i32 786473, metadata !71}        ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!83 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !84, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!84 = metadata !{metadata !8, metadata !30}
!85 = metadata !{metadata !86}
!86 = metadata !{i32 786689, metadata !81, metadata !"f", metadata !82, i32 16777232, metadata !30, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 16]
!87 = metadata !{i32 786478, metadata !71, metadata !82, metadata !"klee_internal_isnan", metadata !"klee_internal_isnan", metadata !"", i32 21, metadata !88, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (double)* @klee_internal_isnan, nu
!88 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !89, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!89 = metadata !{metadata !8, metadata !24}
!90 = metadata !{metadata !91}
!91 = metadata !{i32 786689, metadata !87, metadata !"d", metadata !82, i32 16777237, metadata !24, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [d] [line 21]
!92 = metadata !{i32 786478, metadata !71, metadata !82, metadata !"klee_internal_isnanl", metadata !"klee_internal_isnanl", metadata !"", i32 26, metadata !93, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (x86_fp80)* @klee_internal_isnan
!93 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !94, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!94 = metadata !{metadata !8, metadata !36}
!95 = metadata !{metadata !96}
!96 = metadata !{i32 786689, metadata !92, metadata !"d", metadata !82, i32 16777242, metadata !36, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [d] [line 26]
!97 = metadata !{i32 786478, metadata !71, metadata !82, metadata !"klee_internal_fpclassifyf", metadata !"klee_internal_fpclassifyf", metadata !"", i32 67, metadata !83, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (float)* @klee_interna
!98 = metadata !{metadata !99}
!99 = metadata !{i32 786689, metadata !97, metadata !"f", metadata !82, i32 16777283, metadata !30, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 67]
!100 = metadata !{i32 786478, metadata !71, metadata !82, metadata !"klee_internal_fpclassify", metadata !"klee_internal_fpclassify", metadata !"", i32 82, metadata !88, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (double)* @klee_interna
!101 = metadata !{metadata !102}
!102 = metadata !{i32 786689, metadata !100, metadata !"f", metadata !82, i32 16777298, metadata !24, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 82]
!103 = metadata !{i32 786478, metadata !71, metadata !82, metadata !"klee_internal_fpclassifyl", metadata !"klee_internal_fpclassifyl", metadata !"", i32 98, metadata !93, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (x86_fp80)* @klee_int
!104 = metadata !{metadata !105}
!105 = metadata !{i32 786689, metadata !103, metadata !"ld", metadata !82, i32 16777314, metadata !36, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [ld] [line 98]
!106 = metadata !{i32 786478, metadata !71, metadata !82, metadata !"klee_internal_finitef", metadata !"klee_internal_finitef", metadata !"", i32 114, metadata !83, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (float)* @klee_internal_fini
!107 = metadata !{metadata !108}
!108 = metadata !{i32 786689, metadata !106, metadata !"f", metadata !82, i32 16777330, metadata !30, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 114]
!109 = metadata !{i32 786478, metadata !71, metadata !82, metadata !"klee_internal_finite", metadata !"klee_internal_finite", metadata !"", i32 119, metadata !88, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (double)* @klee_internal_finit
!110 = metadata !{metadata !111}
!111 = metadata !{i32 786689, metadata !109, metadata !"f", metadata !82, i32 16777335, metadata !24, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 119]
!112 = metadata !{i32 786478, metadata !71, metadata !82, metadata !"klee_internal_finitel", metadata !"klee_internal_finitel", metadata !"", i32 124, metadata !93, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (x86_fp80)* @klee_internal_f
!113 = metadata !{metadata !114}
!114 = metadata !{i32 786689, metadata !112, metadata !"f", metadata !82, i32 16777340, metadata !36, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 124]
!115 = metadata !{i32 786449, metadata !116, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!116 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/klee_div_zero_check.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!117 = metadata !{metadata !118}
!118 = metadata !{i32 786478, metadata !116, metadata !119, metadata !"klee_div_zero_check", metadata !"klee_div_zero_check", metadata !"", i32 12, metadata !120, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, void (i64)* @klee_div_zero_check, 
!119 = metadata !{i32 786473, metadata !116}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_div_zero_check.c]
!120 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !121, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!121 = metadata !{null, metadata !122}
!122 = metadata !{i32 786468, null, null, metadata !"long long int", i32 0, i64 64, i64 64, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ] [long long int] [line 0, size 64, align 64, offset 0, enc DW_ATE_signed]
!123 = metadata !{metadata !124}
!124 = metadata !{i32 786689, metadata !118, metadata !"z", metadata !119, i32 16777228, metadata !122, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [z] [line 12]
!125 = metadata !{i32 786449, metadata !126, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!126 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/klee_int.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!127 = metadata !{metadata !128}
!128 = metadata !{i32 786478, metadata !126, metadata !129, metadata !"klee_int", metadata !"klee_int", metadata !"", i32 13, metadata !130, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (i8*)* @klee_int, null, null, metadata !134, i32 13}
!129 = metadata !{i32 786473, metadata !126}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_int.c]
!130 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !131, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!131 = metadata !{metadata !8, metadata !132}
!132 = metadata !{i32 786447, null, null, metadata !"", i32 0, i64 64, i64 64, i64 0, i32 0, metadata !133} ; [ DW_TAG_pointer_type ] [line 0, size 64, align 64, offset 0] [from ]
!133 = metadata !{i32 786470, null, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, metadata !10} ; [ DW_TAG_const_type ] [line 0, size 0, align 0, offset 0] [from char]
!134 = metadata !{metadata !135, metadata !136}
!135 = metadata !{i32 786689, metadata !128, metadata !"name", metadata !129, i32 16777229, metadata !132, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [name] [line 13]
!136 = metadata !{i32 786688, metadata !128, metadata !"x", metadata !129, i32 14, metadata !8, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [x] [line 14]
!137 = metadata !{i32 786449, metadata !138, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!138 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/klee_overshift_check.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!139 = metadata !{metadata !140}
!140 = metadata !{i32 786478, metadata !138, metadata !141, metadata !"klee_overshift_check", metadata !"klee_overshift_check", metadata !"", i32 20, metadata !142, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, void (i64, i64)* @klee_overshift
!141 = metadata !{i32 786473, metadata !138}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_overshift_check.c]
!142 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !143, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!143 = metadata !{null, metadata !144, metadata !144}
!144 = metadata !{i32 786468, null, null, metadata !"long long unsigned int", i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ] [long long unsigned int] [line 0, size 64, align 64, offset 0, enc DW_ATE_unsigned]
!145 = metadata !{metadata !146, metadata !147}
!146 = metadata !{i32 786689, metadata !140, metadata !"bitWidth", metadata !141, i32 16777236, metadata !144, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [bitWidth] [line 20]
!147 = metadata !{i32 786689, metadata !140, metadata !"shift", metadata !141, i32 33554452, metadata !144, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [shift] [line 20]
!148 = metadata !{i32 786449, metadata !149, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!149 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/klee_range.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!150 = metadata !{metadata !151}
!151 = metadata !{i32 786478, metadata !149, metadata !152, metadata !"klee_range", metadata !"klee_range", metadata !"", i32 13, metadata !153, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (i32, i32, i8*)* @klee_range, null, null, metada
!152 = metadata !{i32 786473, metadata !149}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_range.c]
!153 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !154, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!154 = metadata !{metadata !8, metadata !8, metadata !8, metadata !132}
!155 = metadata !{metadata !156, metadata !157, metadata !158, metadata !159}
!156 = metadata !{i32 786689, metadata !151, metadata !"start", metadata !152, i32 16777229, metadata !8, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [start] [line 13]
!157 = metadata !{i32 786689, metadata !151, metadata !"end", metadata !152, i32 33554445, metadata !8, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [end] [line 13]
!158 = metadata !{i32 786689, metadata !151, metadata !"name", metadata !152, i32 50331661, metadata !132, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [name] [line 13]
!159 = metadata !{i32 786688, metadata !151, metadata !"x", metadata !152, i32 14, metadata !8, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [x] [line 14]
!160 = metadata !{i32 786449, metadata !161, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!161 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/klee_set_rounding_mode.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!162 = metadata !{metadata !42}
!163 = metadata !{metadata !164}
!164 = metadata !{i32 786478, metadata !161, metadata !165, metadata !"klee_set_rounding_mode", metadata !"klee_set_rounding_mode", metadata !"", i32 16, metadata !166, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, void (i32)* @klee_set_roundi
!165 = metadata !{i32 786473, metadata !161}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_set_rounding_mode.c]
!166 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !167, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!167 = metadata !{null, metadata !42}
!168 = metadata !{metadata !169}
!169 = metadata !{i32 786689, metadata !164, metadata !"rm", metadata !165, i32 16777232, metadata !42, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [rm] [line 16]
!170 = metadata !{i32 786449, metadata !171, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!171 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/memcpy.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!172 = metadata !{metadata !173}
!173 = metadata !{i32 786478, metadata !171, metadata !174, metadata !"memcpy", metadata !"memcpy", metadata !"", i32 12, metadata !175, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i8* (i8*, i8*, i64)* @memcpy, null, null, metadata !182, i32
!174 = metadata !{i32 786473, metadata !171}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/memcpy.c]
!175 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !176, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!176 = metadata !{metadata !177, metadata !177, metadata !178, metadata !180}
!177 = metadata !{i32 786447, null, null, metadata !"", i32 0, i64 64, i64 64, i64 0, i32 0, null} ; [ DW_TAG_pointer_type ] [line 0, size 64, align 64, offset 0] [from ]
!178 = metadata !{i32 786447, null, null, metadata !"", i32 0, i64 64, i64 64, i64 0, i32 0, metadata !179} ; [ DW_TAG_pointer_type ] [line 0, size 64, align 64, offset 0] [from ]
!179 = metadata !{i32 786470, null, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null} ; [ DW_TAG_const_type ] [line 0, size 0, align 0, offset 0] [from ]
!180 = metadata !{i32 786454, metadata !171, null, metadata !"size_t", i32 42, i64 0, i64 0, i64 0, i32 0, metadata !181} ; [ DW_TAG_typedef ] [size_t] [line 42, size 0, align 0, offset 0] [from long unsigned int]
!181 = metadata !{i32 786468, null, null, metadata !"long unsigned int", i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ] [long unsigned int] [line 0, size 64, align 64, offset 0, enc DW_ATE_unsigned]
!182 = metadata !{metadata !183, metadata !184, metadata !185, metadata !186, metadata !187}
!183 = metadata !{i32 786689, metadata !173, metadata !"destaddr", metadata !174, i32 16777228, metadata !177, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [destaddr] [line 12]
!184 = metadata !{i32 786689, metadata !173, metadata !"srcaddr", metadata !174, i32 33554444, metadata !178, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [srcaddr] [line 12]
!185 = metadata !{i32 786689, metadata !173, metadata !"len", metadata !174, i32 50331660, metadata !180, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [len] [line 12]
!186 = metadata !{i32 786688, metadata !173, metadata !"dest", metadata !174, i32 13, metadata !9, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [dest] [line 13]
!187 = metadata !{i32 786688, metadata !173, metadata !"src", metadata !174, i32 14, metadata !132, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [src] [line 14]
!188 = metadata !{i32 786449, metadata !189, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!189 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/memmove.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!190 = metadata !{metadata !191}
!191 = metadata !{i32 786478, metadata !189, metadata !192, metadata !"memmove", metadata !"memmove", metadata !"", i32 12, metadata !193, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i8* (i8*, i8*, i64)* @memmove, null, null, metadata !196, 
!192 = metadata !{i32 786473, metadata !189}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/memmove.c]
!193 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !194, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!194 = metadata !{metadata !177, metadata !177, metadata !178, metadata !195}
!195 = metadata !{i32 786454, metadata !189, null, metadata !"size_t", i32 42, i64 0, i64 0, i64 0, i32 0, metadata !181} ; [ DW_TAG_typedef ] [size_t] [line 42, size 0, align 0, offset 0] [from long unsigned int]
!196 = metadata !{metadata !197, metadata !198, metadata !199, metadata !200, metadata !201}
!197 = metadata !{i32 786689, metadata !191, metadata !"dst", metadata !192, i32 16777228, metadata !177, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [dst] [line 12]
!198 = metadata !{i32 786689, metadata !191, metadata !"src", metadata !192, i32 33554444, metadata !178, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [src] [line 12]
!199 = metadata !{i32 786689, metadata !191, metadata !"count", metadata !192, i32 50331660, metadata !195, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [count] [line 12]
!200 = metadata !{i32 786688, metadata !191, metadata !"a", metadata !192, i32 13, metadata !9, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [a] [line 13]
!201 = metadata !{i32 786688, metadata !191, metadata !"b", metadata !192, i32 14, metadata !132, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [b] [line 14]
!202 = metadata !{i32 786449, metadata !203, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!203 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/mempcpy.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!204 = metadata !{metadata !205}
!205 = metadata !{i32 786478, metadata !203, metadata !206, metadata !"mempcpy", metadata !"mempcpy", metadata !"", i32 11, metadata !207, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i8* (i8*, i8*, i64)* @mempcpy, null, null, metadata !210, 
!206 = metadata !{i32 786473, metadata !203}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/mempcpy.c]
!207 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !208, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!208 = metadata !{metadata !177, metadata !177, metadata !178, metadata !209}
!209 = metadata !{i32 786454, metadata !203, null, metadata !"size_t", i32 42, i64 0, i64 0, i64 0, i32 0, metadata !181} ; [ DW_TAG_typedef ] [size_t] [line 42, size 0, align 0, offset 0] [from long unsigned int]
!210 = metadata !{metadata !211, metadata !212, metadata !213, metadata !214, metadata !215}
!211 = metadata !{i32 786689, metadata !205, metadata !"destaddr", metadata !206, i32 16777227, metadata !177, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [destaddr] [line 11]
!212 = metadata !{i32 786689, metadata !205, metadata !"srcaddr", metadata !206, i32 33554443, metadata !178, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [srcaddr] [line 11]
!213 = metadata !{i32 786689, metadata !205, metadata !"len", metadata !206, i32 50331659, metadata !209, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [len] [line 11]
!214 = metadata !{i32 786688, metadata !205, metadata !"dest", metadata !206, i32 12, metadata !9, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [dest] [line 12]
!215 = metadata !{i32 786688, metadata !205, metadata !"src", metadata !206, i32 13, metadata !132, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [src] [line 13]
!216 = metadata !{i32 786449, metadata !217, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!217 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/memset.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!218 = metadata !{metadata !219}
!219 = metadata !{i32 786478, metadata !217, metadata !220, metadata !"memset", metadata !"memset", metadata !"", i32 11, metadata !221, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i8* (i8*, i32, i64)* @memset, null, null, metadata !224, i32
!220 = metadata !{i32 786473, metadata !217}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/memset.c]
!221 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !222, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!222 = metadata !{metadata !177, metadata !177, metadata !8, metadata !223}
!223 = metadata !{i32 786454, metadata !217, null, metadata !"size_t", i32 42, i64 0, i64 0, i64 0, i32 0, metadata !181} ; [ DW_TAG_typedef ] [size_t] [line 42, size 0, align 0, offset 0] [from long unsigned int]
!224 = metadata !{metadata !225, metadata !226, metadata !227, metadata !228}
!225 = metadata !{i32 786689, metadata !219, metadata !"dst", metadata !220, i32 16777227, metadata !177, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [dst] [line 11]
!226 = metadata !{i32 786689, metadata !219, metadata !"s", metadata !220, i32 33554443, metadata !8, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [s] [line 11]
!227 = metadata !{i32 786689, metadata !219, metadata !"count", metadata !220, i32 50331659, metadata !223, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [count] [line 11]
!228 = metadata !{i32 786688, metadata !219, metadata !"a", metadata !220, i32 12, metadata !229, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [a] [line 12]
!229 = metadata !{i32 786447, null, null, metadata !"", i32 0, i64 64, i64 64, i64 0, i32 0, metadata !230} ; [ DW_TAG_pointer_type ] [line 0, size 64, align 64, offset 0] [from ]
!230 = metadata !{i32 786485, null, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, metadata !10} ; [ DW_TAG_volatile_type ] [line 0, size 0, align 0, offset 0] [from char]
!231 = metadata !{i32 786449, metadata !232, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!232 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/sqrt.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!233 = metadata !{metadata !234, metadata !238, metadata !241}
!234 = metadata !{i32 786478, metadata !232, metadata !235, metadata !"klee_internal_sqrt", metadata !"klee_internal_sqrt", metadata !"", i32 11, metadata !22, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, double (double)* @klee_internal_sqrt,
!235 = metadata !{i32 786473, metadata !232}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/sqrt.c]
!236 = metadata !{metadata !237}
!237 = metadata !{i32 786689, metadata !234, metadata !"d", metadata !235, i32 16777227, metadata !24, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [d] [line 11]
!238 = metadata !{i32 786478, metadata !232, metadata !235, metadata !"klee_internal_sqrtf", metadata !"klee_internal_sqrtf", metadata !"", i32 15, metadata !28, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, float (float)* @klee_internal_sqrtf
!239 = metadata !{metadata !240}
!240 = metadata !{i32 786689, metadata !238, metadata !"f", metadata !235, i32 16777231, metadata !30, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 15]
!241 = metadata !{i32 786478, metadata !232, metadata !235, metadata !"klee_internal_sqrtl", metadata !"klee_internal_sqrtl", metadata !"", i32 20, metadata !34, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, x86_fp80 (x86_fp80)* @klee_internal
!242 = metadata !{metadata !243}
!243 = metadata !{i32 786689, metadata !241, metadata !"f", metadata !235, i32 16777236, metadata !36, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 20]
!244 = metadata !{i32 2, metadata !"Dwarf Version", i32 4}
!245 = metadata !{i32 1, metadata !"Debug Info Version", i32 1}
!246 = metadata !{metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)"}
!247 = metadata !{i32 11, i32 0, metadata !4, null}
!248 = metadata !{i32 12, i32 0, metadata !4, null}
!249 = metadata !{i32 13, i32 0, metadata !4, null}
!250 = metadata !{i32 14, i32 0, metadata !4, null}
!251 = metadata !{i32 15, i32 0, metadata !252, null}
!252 = metadata !{i32 786443, metadata !1, metadata !4, i32 15, i32 0, i32 0} ; [ DW_TAG_lexical_block ] [/home/klee/logic_bombs/src/buffer_overflow/heap_bo_l1_klee_annotated.c]
!253 = metadata !{i32 16, i32 0, metadata !254, null}
!254 = metadata !{i32 786443, metadata !1, metadata !252, i32 15, i32 0, i32 1} ; [ DW_TAG_lexical_block ] [/home/klee/logic_bombs/src/buffer_overflow/heap_bo_l1_klee_annotated.c]
!255 = metadata !{i32 17, i32 0, metadata !254, null}
!256 = metadata !{i32 18, i32 0, metadata !254, null}
!257 = metadata !{i32 22, i32 0, metadata !4, null}
!258 = metadata !{i32 26, i32 0, metadata !11, null}
!259 = metadata !{i32 27, i32 0, metadata !11, null}
!260 = metadata !{i32 28, i32 0, metadata !11, null}
!261 = metadata !{i32 12, i32 0, metadata !20, null}
!262 = metadata !{i32 16, i32 0, metadata !27, null}
!263 = metadata !{i32 21, i32 0, metadata !33, null}
!264 = metadata !{i32 34, i32 0, metadata !59, null}
!265 = metadata !{i32 35, i32 0, metadata !59, null}
!266 = metadata !{i32 50, i32 0, metadata !59, null}
!267 = metadata !{i32 53, i32 0, metadata !65, null}
!268 = metadata !{i32 55, i32 0, metadata !269, null}
!269 = metadata !{i32 786443, metadata !40, metadata !65, i32 53, i32 0, i32 1} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fenv.c]
!270 = metadata !{i32 56, i32 0, metadata !269, null}
!271 = metadata !{i32 66, i32 0, metadata !269, null}
!272 = metadata !{i32 67, i32 0, metadata !269, null}
!273 = metadata !{i32 69, i32 0, metadata !269, null}
!274 = metadata !{i32 70, i32 0, metadata !269, null}
!275 = metadata !{i32 72, i32 0, metadata !269, null}
!276 = metadata !{i32 73, i32 0, metadata !269, null}
!277 = metadata !{i32 79, i32 0, metadata !65, null}
!278 = metadata !{i32 17, i32 0, metadata !81, null}
!279 = metadata !{i32 22, i32 0, metadata !87, null}
!280 = metadata !{i32 27, i32 0, metadata !92, null}
!281 = metadata !{i32 69, i32 0, metadata !282, null}
!282 = metadata !{i32 786443, metadata !71, metadata !97, i32 69, i32 0, i32 0} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!283 = metadata !{i32 71, i32 0, metadata !284, null}
!284 = metadata !{i32 786443, metadata !71, metadata !282, i32 71, i32 0, i32 2} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!285 = metadata !{i32 73, i32 0, metadata !286, null}
!286 = metadata !{i32 786443, metadata !71, metadata !284, i32 73, i32 0, i32 4} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!287 = metadata !{i32 75, i32 0, metadata !288, null}
!288 = metadata !{i32 786443, metadata !71, metadata !286, i32 75, i32 0, i32 6} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!289 = metadata !{i32 76, i32 0, metadata !290, null}
!290 = metadata !{i32 786443, metadata !71, metadata !288, i32 75, i32 0, i32 7} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!291 = metadata !{i32 79, i32 0, metadata !97, null}
!292 = metadata !{i32 84, i32 0, metadata !293, null}
!293 = metadata !{i32 786443, metadata !71, metadata !100, i32 84, i32 0, i32 8} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!294 = metadata !{i32 86, i32 0, metadata !295, null}
!295 = metadata !{i32 786443, metadata !71, metadata !293, i32 86, i32 0, i32 10} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!296 = metadata !{i32 88, i32 0, metadata !297, null}
!297 = metadata !{i32 786443, metadata !71, metadata !295, i32 88, i32 0, i32 12} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!298 = metadata !{i32 90, i32 0, metadata !299, null}
!299 = metadata !{i32 786443, metadata !71, metadata !297, i32 90, i32 0, i32 14} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!300 = metadata !{i32 91, i32 0, metadata !301, null}
!301 = metadata !{i32 786443, metadata !71, metadata !299, i32 90, i32 0, i32 15} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!302 = metadata !{i32 94, i32 0, metadata !100, null}
!303 = metadata !{i32 100, i32 0, metadata !304, null}
!304 = metadata !{i32 786443, metadata !71, metadata !103, i32 100, i32 0, i32 16} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!305 = metadata !{i32 102, i32 0, metadata !306, null}
!306 = metadata !{i32 786443, metadata !71, metadata !304, i32 102, i32 0, i32 18} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!307 = metadata !{i32 104, i32 0, metadata !308, null}
!308 = metadata !{i32 786443, metadata !71, metadata !306, i32 104, i32 0, i32 20} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!309 = metadata !{i32 106, i32 0, metadata !310, null}
!310 = metadata !{i32 786443, metadata !71, metadata !308, i32 106, i32 0, i32 22} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!311 = metadata !{i32 107, i32 0, metadata !312, null}
!312 = metadata !{i32 786443, metadata !71, metadata !310, i32 106, i32 0, i32 23} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!313 = metadata !{i32 110, i32 0, metadata !103, null}
!314 = metadata !{i32 115, i32 0, metadata !106, null}
!315 = metadata !{i32 120, i32 0, metadata !109, null}
!316 = metadata !{i32 125, i32 0, metadata !112, null}
!317 = metadata !{i32 13, i32 0, metadata !318, null}
!318 = metadata !{i32 786443, metadata !116, metadata !118, i32 13, i32 0, i32 0} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_div_zero_check.c]
!319 = metadata !{i32 14, i32 0, metadata !318, null}
!320 = metadata !{i32 15, i32 0, metadata !118, null}
!321 = metadata !{i32 15, i32 0, metadata !128, null}
!322 = metadata !{i32 16, i32 0, metadata !128, null}
!323 = metadata !{metadata !324, metadata !324, i64 0}
!324 = metadata !{metadata !"int", metadata !325, i64 0}
!325 = metadata !{metadata !"omnipotent char", metadata !326, i64 0}
!326 = metadata !{metadata !"Simple C/C++ TBAA"}
!327 = metadata !{i32 21, i32 0, metadata !328, null}
!328 = metadata !{i32 786443, metadata !138, metadata !140, i32 21, i32 0, i32 0} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_overshift_check.c]
!329 = metadata !{i32 27, i32 0, metadata !330, null}
!330 = metadata !{i32 786443, metadata !138, metadata !328, i32 21, i32 0, i32 1} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_overshift_check.c]
!331 = metadata !{i32 29, i32 0, metadata !140, null}
!332 = metadata !{i32 16, i32 0, metadata !333, null}
!333 = metadata !{i32 786443, metadata !149, metadata !151, i32 16, i32 0, i32 0} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_range.c]
!334 = metadata !{i32 17, i32 0, metadata !333, null}
!335 = metadata !{i32 19, i32 0, metadata !336, null}
!336 = metadata !{i32 786443, metadata !149, metadata !151, i32 19, i32 0, i32 1} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_range.c]
!337 = metadata !{i32 22, i32 0, metadata !338, null}
!338 = metadata !{i32 786443, metadata !149, metadata !336, i32 21, i32 0, i32 3} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_range.c]
!339 = metadata !{i32 25, i32 0, metadata !340, null}
!340 = metadata !{i32 786443, metadata !149, metadata !338, i32 25, i32 0, i32 4} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_range.c]
!341 = metadata !{i32 26, i32 0, metadata !342, null}
!342 = metadata !{i32 786443, metadata !149, metadata !340, i32 25, i32 0, i32 5} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_range.c]
!343 = metadata !{i32 27, i32 0, metadata !342, null}
!344 = metadata !{i32 28, i32 0, metadata !345, null}
!345 = metadata !{i32 786443, metadata !149, metadata !340, i32 27, i32 0, i32 6} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_range.c]
!346 = metadata !{i32 29, i32 0, metadata !345, null}
!347 = metadata !{i32 32, i32 0, metadata !338, null}
!348 = metadata !{i32 34, i32 0, metadata !151, null}
!349 = metadata !{i32 19, i32 0, metadata !164, null}
!350 = metadata !{i32 21, i32 0, metadata !351, null}
!351 = metadata !{i32 786443, metadata !161, metadata !164, i32 19, i32 0, i32 0} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_set_rounding_mode.c]
!352 = metadata !{i32 23, i32 0, metadata !351, null}
!353 = metadata !{i32 25, i32 0, metadata !351, null}
!354 = metadata !{i32 27, i32 0, metadata !351, null}
!355 = metadata !{i32 29, i32 0, metadata !351, null}
!356 = metadata !{i32 31, i32 0, metadata !351, null}
!357 = metadata !{i32 33, i32 0, metadata !164, null}
!358 = metadata !{i32 16, i32 0, metadata !173, null}
!359 = metadata !{i32 17, i32 0, metadata !173, null}
!360 = metadata !{metadata !360, metadata !361, metadata !362}
!361 = metadata !{metadata !"llvm.vectorizer.width", i32 1}
!362 = metadata !{metadata !"llvm.vectorizer.unroll", i32 1}
!363 = metadata !{metadata !325, metadata !325, i64 0}
!364 = metadata !{metadata !364, metadata !361, metadata !362}
!365 = metadata !{i32 18, i32 0, metadata !173, null}
!366 = metadata !{i32 16, i32 0, metadata !367, null}
!367 = metadata !{i32 786443, metadata !189, metadata !191, i32 16, i32 0, i32 0} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/memmove.c]
!368 = metadata !{i32 19, i32 0, metadata !369, null}
!369 = metadata !{i32 786443, metadata !189, metadata !191, i32 19, i32 0, i32 1} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/memmove.c]
!370 = metadata !{i32 20, i32 0, metadata !371, null}
!371 = metadata !{i32 786443, metadata !189, metadata !369, i32 19, i32 0, i32 2} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/memmove.c]
!372 = metadata !{metadata !372, metadata !361, metadata !362}
!373 = metadata !{metadata !373, metadata !361, metadata !362}
!374 = metadata !{i32 22, i32 0, metadata !375, null}
!375 = metadata !{i32 786443, metadata !189, metadata !369, i32 21, i32 0, i32 3} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/memmove.c]
!376 = metadata !{i32 24, i32 0, metadata !375, null}
!377 = metadata !{i32 23, i32 0, metadata !375, null}
!378 = metadata !{metadata !378, metadata !361, metadata !362}
!379 = metadata !{metadata !379, metadata !361, metadata !362}
!380 = metadata !{i32 28, i32 0, metadata !191, null}
!381 = metadata !{i32 15, i32 0, metadata !205, null}
!382 = metadata !{i32 16, i32 0, metadata !205, null}
!383 = metadata !{metadata !383, metadata !361, metadata !362}
!384 = metadata !{metadata !384, metadata !361, metadata !362}
!385 = metadata !{i32 17, i32 0, metadata !205, null}
!386 = metadata !{i32 13, i32 0, metadata !219, null}
!387 = metadata !{i32 14, i32 0, metadata !219, null}
!388 = metadata !{i32 15, i32 0, metadata !219, null}
!389 = metadata !{i32 12, i32 0, metadata !234, null}
!390 = metadata !{i32 16, i32 0, metadata !238, null}
!391 = metadata !{i32 21, i32 0, metadata !241, null}
