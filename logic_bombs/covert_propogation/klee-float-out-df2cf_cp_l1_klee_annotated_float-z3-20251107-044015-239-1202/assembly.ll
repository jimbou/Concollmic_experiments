; ModuleID = 'df2cf_cp_l1_klee_annotated_float.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [28 x i8] c"0 && \22Logic bomb triggered\22\00", align 1
@.str1 = private unnamed_addr constant [29 x i8] c"df2cf_cp_l1_klee_annotated.c\00", align 1
@__PRETTY_FUNCTION__.logic_bomb = private unnamed_addr constant [23 x i8] c"int logic_bomb(char *)\00", align 1
@.str2 = private unnamed_addr constant [2 x i8] c"s\00", align 1
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
define i32 @df2cf(i8 signext %a) #0 {
entry:
  %a.addr = alloca i8, align 1
  %b = alloca i32, align 4
  store i8 %a, i8* %a.addr, align 1
  %0 = load i8* %a.addr, align 1, !dbg !248
  %conv = sext i8 %0 to i32, !dbg !248
  switch i32 %conv, label %sw.default [
    i32 0, label %sw.bb
    i32 1, label %sw.bb1
    i32 2, label %sw.bb2
    i32 3, label %sw.bb3
    i32 4, label %sw.bb4
    i32 5, label %sw.bb5
    i32 6, label %sw.bb6
    i32 7, label %sw.bb7
    i32 8, label %sw.bb8
    i32 9, label %sw.bb9
  ], !dbg !248

sw.bb:                                            ; preds = %entry
  store i32 0, i32* %b, align 4, !dbg !249
  br label %sw.epilog, !dbg !251

sw.bb1:                                           ; preds = %entry
  store i32 1, i32* %b, align 4, !dbg !252
  br label %sw.epilog, !dbg !253

sw.bb2:                                           ; preds = %entry
  store i32 2, i32* %b, align 4, !dbg !254
  br label %sw.epilog, !dbg !255

sw.bb3:                                           ; preds = %entry
  store i32 3, i32* %b, align 4, !dbg !256
  br label %sw.epilog, !dbg !257

sw.bb4:                                           ; preds = %entry
  store i32 4, i32* %b, align 4, !dbg !258
  br label %sw.epilog, !dbg !259

sw.bb5:                                           ; preds = %entry
  store i32 5, i32* %b, align 4, !dbg !260
  br label %sw.epilog, !dbg !261

sw.bb6:                                           ; preds = %entry
  store i32 6, i32* %b, align 4, !dbg !262
  br label %sw.epilog, !dbg !263

sw.bb7:                                           ; preds = %entry
  store i32 7, i32* %b, align 4, !dbg !264
  br label %sw.epilog, !dbg !265

sw.bb8:                                           ; preds = %entry
  store i32 8, i32* %b, align 4, !dbg !266
  br label %sw.epilog, !dbg !267

sw.bb9:                                           ; preds = %entry
  store i32 9, i32* %b, align 4, !dbg !268
  br label %sw.epilog, !dbg !269

sw.default:                                       ; preds = %entry
  store i32 0, i32* %b, align 4, !dbg !270
  br label %sw.epilog, !dbg !271

sw.epilog:                                        ; preds = %sw.default, %sw.bb9, %sw.bb8, %sw.bb7, %sw.bb6, %sw.bb5, %sw.bb4, %sw.bb3, %sw.bb2, %sw.bb1, %sw.bb
  %1 = load i32* %b, align 4, !dbg !272
  ret i32 %1, !dbg !272
}

; Function Attrs: nounwind readnone
declare void @llvm.dbg.declare(metadata, metadata) #1

; Function Attrs: nounwind uwtable
define i32 @logic_bomb(i8* %s) #0 {
entry:
  %retval = alloca i32, align 4
  %s.addr = alloca i8*, align 8
  %symvar = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i8* %s, i8** %s.addr, align 8
  %0 = load i8** %s.addr, align 8, !dbg !273
  %arrayidx = getelementptr inbounds i8* %0, i64 0, !dbg !273
  %1 = load i8* %arrayidx, align 1, !dbg !273
  %conv = sext i8 %1 to i32, !dbg !273
  %sub = sub nsw i32 %conv, 48, !dbg !273
  store i32 %sub, i32* %symvar, align 4, !dbg !273
  %2 = load i32* %symvar, align 4, !dbg !274
  %int_cast_to_i64 = zext i32 10 to i64
  call void @klee_div_zero_check(i64 %int_cast_to_i64), !dbg !274
  %rem = srem i32 %2, 10, !dbg !274
  %conv1 = trunc i32 %rem to i8, !dbg !274
  %call = call i32 @df2cf(i8 signext %conv1), !dbg !274
  store i32 %call, i32* %a, align 4, !dbg !274
  %3 = load i32* %a, align 4, !dbg !275
  %inc = add nsw i32 %3, 1, !dbg !275
  store i32 %inc, i32* %a, align 4, !dbg !275
  %4 = load i32* %symvar, align 4, !dbg !276
  %5 = load i32* %a, align 4, !dbg !276
  %add = add nsw i32 %4, %5, !dbg !276
  store i32 %add, i32* %b, align 4, !dbg !276
  %6 = load i32* %b, align 4, !dbg !277
  %cmp = icmp eq i32 %6, 15, !dbg !277
  br i1 %cmp, label %if.then, label %if.end, !dbg !277

if.then:                                          ; preds = %entry
  %call3 = call i32 (i8*, i8*, i32, i8*, ...)* bitcast (i32 (...)* @__assert_fail to i32 (i8*, i8*, i32, i8*, ...)*)(i8* getelementptr inbounds ([28 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([29 x i8]* @.str1, i32 0, i32 0), i32 58, i8* ge
  br label %if.end, !dbg !279

if.end:                                           ; preds = %entry, %if.then
  %7 = load i32* %retval, !dbg !280
  ret i32 %7, !dbg !280
}

; Function Attrs: noreturn
declare i32 @__assert_fail(...) #2

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %s = alloca [5 x i8], align 1
  store i32 0, i32* %retval
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  %0 = bitcast [5 x i8]* %s to i8*, !dbg !281
  call void @klee_make_symbolic(i8* %0, i64 5, i8* getelementptr inbounds ([2 x i8]* @.str2, i32 0, i32 0)), !dbg !281
  %arrayidx = getelementptr inbounds [5 x i8]* %s, i32 0, i64 4, !dbg !282
  %1 = load i8* %arrayidx, align 1, !dbg !282
  %conv = sext i8 %1 to i32, !dbg !282
  %cmp = icmp eq i32 %conv, 0, !dbg !282
  %conv1 = zext i1 %cmp to i32, !dbg !282
  %conv2 = sext i32 %conv1 to i64, !dbg !282
  call void @klee_assume(i64 %conv2), !dbg !282
  %arraydecay = getelementptr inbounds [5 x i8]* %s, i32 0, i32 0, !dbg !283
  %call = call i32 @logic_bomb(i8* %arraydecay), !dbg !283
  ret i32 %call, !dbg !283
}

declare void @klee_make_symbolic(i8*, i64, i8*) #3

declare void @klee_assume(i64) #3

declare zeroext i1 @klee_is_infinite_float(float) #4

declare zeroext i1 @klee_is_infinite_double(double) #4

declare zeroext i1 @klee_is_infinite_long_double(x86_fp80) #4

; Function Attrs: noinline nounwind optnone uwtable
define i32 @klee_internal_isinff(float %f) #5 {
entry:
  %isinf = tail call zeroext i1 @klee_is_infinite_float(float %f) #9
  %cmp = fcmp ogt float %f, 0.000000e+00
  %posOrNeg = select i1 %cmp, i32 1, i32 -1
  %result = select i1 %isinf, i32 %posOrNeg, i32 0
  ret i32 %result
}

; Function Attrs: noinline nounwind optnone uwtable
define i32 @klee_internal_isinf(double %d) #5 {
entry:
  %isinf = tail call zeroext i1 @klee_is_infinite_double(double %d) #9
  %cmp = fcmp ogt double %d, 0.000000e+00
  %posOrNeg = select i1 %cmp, i32 1, i32 -1
  %result = select i1 %isinf, i32 %posOrNeg, i32 0
  ret i32 %result
}

; Function Attrs: noinline optnone
define i32 @klee_internal_isinfl(x86_fp80 %d) #6 {
entry:
  %isinf = tail call zeroext i1 @klee_is_infinite_long_double(x86_fp80 %d) #9
  %cmp = fcmp ogt x86_fp80 %d, 0xK00000000000000000000
  %posOrNeg = select i1 %cmp, i32 1, i32 -1
  %result = select i1 %isinf, i32 %posOrNeg, i32 0
  ret i32 %result
}

; Function Attrs: nounwind uwtable
define double @klee_internal_fabs(double %d) #7 {
entry:
  %call = tail call double @klee_abs_double(double %d) #9, !dbg !284
  ret double %call, !dbg !284
}

declare double @klee_abs_double(double) #4

; Function Attrs: nounwind uwtable
define float @klee_internal_fabsf(float %f) #7 {
entry:
  %call = tail call float @klee_abs_float(float %f) #9, !dbg !285
  ret float %call, !dbg !285
}

declare float @klee_abs_float(float) #4

; Function Attrs: nounwind uwtable
define x86_fp80 @klee_internal_fabsl(x86_fp80 %f) #7 {
entry:
  %call = tail call x86_fp80 @klee_abs_long_double(x86_fp80 %f) #9, !dbg !286
  ret x86_fp80 %call, !dbg !286
}

declare x86_fp80 @klee_abs_long_double(x86_fp80) #4

; Function Attrs: nounwind readnone
declare void @llvm.dbg.value(metadata, i64, metadata) #1

; Function Attrs: nounwind uwtable
define i32 @klee_internal_fegetround() #7 {
entry:
  %call = tail call i32 (...)* @klee_get_rounding_mode() #9, !dbg !287
  %0 = icmp ult i32 %call, 5, !dbg !288
  br i1 %0, label %switch.lookup, label %return, !dbg !288

switch.lookup:                                    ; preds = %entry
  %1 = sext i32 %call to i64, !dbg !288
  %switch.gep = getelementptr inbounds [5 x i32]* @switch.table, i64 0, i64 %1, !dbg !288
  %switch.load = load i32* %switch.gep, align 4, !dbg !288
  ret i32 %switch.load, !dbg !288

return:                                           ; preds = %entry
  ret i32 -1, !dbg !289
}

declare i32 @klee_get_rounding_mode(...) #4

; Function Attrs: nounwind uwtable
define i32 @klee_internal_fesetround(i32 %rm) #7 {
entry:
  switch i32 %rm, label %return [
    i32 0, label %sw.bb
    i32 2048, label %sw.bb1
    i32 1024, label %sw.bb2
    i32 3072, label %sw.bb3
  ], !dbg !290

sw.bb:                                            ; preds = %entry
  tail call void @klee_set_rounding_mode(i32 0) #9, !dbg !291
  br label %return, !dbg !293

sw.bb1:                                           ; preds = %entry
  tail call void @klee_set_rounding_mode(i32 2) #9, !dbg !294
  br label %return, !dbg !295

sw.bb2:                                           ; preds = %entry
  tail call void @klee_set_rounding_mode(i32 3) #9, !dbg !296
  br label %return, !dbg !297

sw.bb3:                                           ; preds = %entry
  tail call void @klee_set_rounding_mode(i32 4) #9, !dbg !298
  br label %return, !dbg !299

return:                                           ; preds = %sw.bb3, %sw.bb2, %sw.bb1, %sw.bb, %entry
  %retval.0 = phi i32 [ -1, %entry ], [ 0, %sw.bb3 ], [ 0, %sw.bb2 ], [ 0, %sw.bb1 ], [ 0, %sw.bb ]
  ret i32 %retval.0, !dbg !300
}

; Function Attrs: nounwind uwtable
define i32 @klee_internal_isnanf(float %f) #7 {
entry:
  %call = tail call zeroext i1 @klee_is_nan_float(float %f) #9, !dbg !301
  %conv = zext i1 %call to i32, !dbg !301
  ret i32 %conv, !dbg !301
}

declare zeroext i1 @klee_is_nan_float(float) #4

; Function Attrs: nounwind uwtable
define i32 @klee_internal_isnan(double %d) #7 {
entry:
  %call = tail call zeroext i1 @klee_is_nan_double(double %d) #9, !dbg !302
  %conv = zext i1 %call to i32, !dbg !302
  ret i32 %conv, !dbg !302
}

declare zeroext i1 @klee_is_nan_double(double) #4

; Function Attrs: nounwind uwtable
define i32 @klee_internal_isnanl(x86_fp80 %d) #7 {
entry:
  %call = tail call zeroext i1 @klee_is_nan_long_double(x86_fp80 %d) #9, !dbg !303
  %conv = zext i1 %call to i32, !dbg !303
  ret i32 %conv, !dbg !303
}

declare zeroext i1 @klee_is_nan_long_double(x86_fp80) #4

; Function Attrs: nounwind uwtable
define i32 @klee_internal_fpclassifyf(float %f) #7 {
entry:
  %call = tail call zeroext i1 @klee_is_nan_float(float %f) #9, !dbg !304
  br i1 %call, label %return, label %if.else, !dbg !304

if.else:                                          ; preds = %entry
  %call1 = tail call zeroext i1 @klee_is_infinite_float(float %f) #9, !dbg !306
  br i1 %call1, label %return, label %if.else3, !dbg !306

if.else3:                                         ; preds = %if.else
  %cmp = fcmp oeq float %f, 0.000000e+00, !dbg !308
  br i1 %cmp, label %return, label %if.else5, !dbg !308

if.else5:                                         ; preds = %if.else3
  %call6 = tail call zeroext i1 @klee_is_normal_float(float %f) #9, !dbg !310
  %. = select i1 %call6, i32 4, i32 3, !dbg !312
  br label %return, !dbg !312

return:                                           ; preds = %if.else5, %if.else3, %if.else, %entry
  %retval.0 = phi i32 [ 0, %entry ], [ 1, %if.else ], [ 2, %if.else3 ], [ %., %if.else5 ]
  ret i32 %retval.0, !dbg !314
}

declare zeroext i1 @klee_is_normal_float(float) #4

; Function Attrs: nounwind uwtable
define i32 @klee_internal_fpclassify(double %f) #7 {
entry:
  %call = tail call zeroext i1 @klee_is_nan_double(double %f) #9, !dbg !315
  br i1 %call, label %return, label %if.else, !dbg !315

if.else:                                          ; preds = %entry
  %call1 = tail call zeroext i1 @klee_is_infinite_double(double %f) #9, !dbg !317
  br i1 %call1, label %return, label %if.else3, !dbg !317

if.else3:                                         ; preds = %if.else
  %cmp = fcmp oeq double %f, 0.000000e+00, !dbg !319
  br i1 %cmp, label %return, label %if.else5, !dbg !319

if.else5:                                         ; preds = %if.else3
  %call6 = tail call zeroext i1 @klee_is_normal_double(double %f) #9, !dbg !321
  %. = select i1 %call6, i32 4, i32 3, !dbg !323
  br label %return, !dbg !323

return:                                           ; preds = %if.else5, %if.else3, %if.else, %entry
  %retval.0 = phi i32 [ 0, %entry ], [ 1, %if.else ], [ 2, %if.else3 ], [ %., %if.else5 ]
  ret i32 %retval.0, !dbg !325
}

declare zeroext i1 @klee_is_normal_double(double) #4

; Function Attrs: nounwind uwtable
define i32 @klee_internal_fpclassifyl(x86_fp80 %ld) #7 {
entry:
  %call = tail call zeroext i1 @klee_is_nan_long_double(x86_fp80 %ld) #9, !dbg !326
  br i1 %call, label %return, label %if.else, !dbg !326

if.else:                                          ; preds = %entry
  %call1 = tail call zeroext i1 @klee_is_infinite_long_double(x86_fp80 %ld) #9, !dbg !328
  br i1 %call1, label %return, label %if.else3, !dbg !328

if.else3:                                         ; preds = %if.else
  %cmp = fcmp oeq x86_fp80 %ld, 0xK00000000000000000000, !dbg !330
  br i1 %cmp, label %return, label %if.else5, !dbg !330

if.else5:                                         ; preds = %if.else3
  %call6 = tail call zeroext i1 @klee_is_normal_long_double(x86_fp80 %ld) #9, !dbg !332
  %. = select i1 %call6, i32 4, i32 3, !dbg !334
  br label %return, !dbg !334

return:                                           ; preds = %if.else5, %if.else3, %if.else, %entry
  %retval.0 = phi i32 [ 0, %entry ], [ 1, %if.else ], [ 2, %if.else3 ], [ %., %if.else5 ]
  ret i32 %retval.0, !dbg !336
}

declare zeroext i1 @klee_is_normal_long_double(x86_fp80) #4

; Function Attrs: nounwind uwtable
define i32 @klee_internal_finitef(float %f) #7 {
entry:
  %call = tail call zeroext i1 @klee_is_nan_float(float %f) #9, !dbg !337
  %0 = zext i1 %call to i32, !dbg !337
  %lnot.ext = xor i32 %0, 1, !dbg !337
  %call1 = tail call zeroext i1 @klee_is_infinite_float(float %f) #9, !dbg !337
  %1 = zext i1 %call1 to i32, !dbg !337
  %lnot.ext3 = xor i32 %1, 1, !dbg !337
  %and = and i32 %lnot.ext3, %lnot.ext, !dbg !337
  ret i32 %and, !dbg !337
}

; Function Attrs: nounwind uwtable
define i32 @klee_internal_finite(double %f) #7 {
entry:
  %call = tail call zeroext i1 @klee_is_nan_double(double %f) #9, !dbg !338
  %0 = zext i1 %call to i32, !dbg !338
  %lnot.ext = xor i32 %0, 1, !dbg !338
  %call1 = tail call zeroext i1 @klee_is_infinite_double(double %f) #9, !dbg !338
  %1 = zext i1 %call1 to i32, !dbg !338
  %lnot.ext3 = xor i32 %1, 1, !dbg !338
  %and = and i32 %lnot.ext3, %lnot.ext, !dbg !338
  ret i32 %and, !dbg !338
}

; Function Attrs: nounwind uwtable
define i32 @klee_internal_finitel(x86_fp80 %f) #7 {
entry:
  %call = tail call zeroext i1 @klee_is_nan_long_double(x86_fp80 %f) #9, !dbg !339
  %0 = zext i1 %call to i32, !dbg !339
  %lnot.ext = xor i32 %0, 1, !dbg !339
  %call1 = tail call zeroext i1 @klee_is_infinite_long_double(x86_fp80 %f) #9, !dbg !339
  %1 = zext i1 %call1 to i32, !dbg !339
  %lnot.ext3 = xor i32 %1, 1, !dbg !339
  %and = and i32 %lnot.ext3, %lnot.ext, !dbg !339
  ret i32 %and, !dbg !339
}

; Function Attrs: nounwind uwtable
define void @klee_div_zero_check(i64 %z) #7 {
entry:
  %cmp = icmp eq i64 %z, 0, !dbg !340
  br i1 %cmp, label %if.then, label %if.end, !dbg !340

if.then:                                          ; preds = %entry
  tail call void @klee_report_error(i8* getelementptr inbounds ([56 x i8]* @.str3, i64 0, i64 0), i32 14, i8* getelementptr inbounds ([15 x i8]* @.str14, i64 0, i64 0), i8* getelementptr inbounds ([8 x i8]* @.str25, i64 0, i64 0)) #10, !dbg !342
  unreachable, !dbg !342

if.end:                                           ; preds = %entry
  ret void, !dbg !343
}

; Function Attrs: noreturn
declare void @klee_report_error(i8*, i32, i8*, i8*) #8

; Function Attrs: nounwind uwtable
define i32 @klee_int(i8* %name) #7 {
entry:
  %x = alloca i32, align 4
  %0 = bitcast i32* %x to i8*, !dbg !344
  call void @klee_make_symbolic(i8* %0, i64 4, i8* %name) #9, !dbg !344
  %1 = load i32* %x, align 4, !dbg !345, !tbaa !346
  ret i32 %1, !dbg !345
}

; Function Attrs: nounwind uwtable
define void @klee_overshift_check(i64 %bitWidth, i64 %shift) #7 {
entry:
  %cmp = icmp ult i64 %shift, %bitWidth, !dbg !350
  br i1 %cmp, label %if.end, label %if.then, !dbg !350

if.then:                                          ; preds = %entry
  tail call void @klee_report_error(i8* getelementptr inbounds ([8 x i8]* @.str36, i64 0, i64 0), i32 0, i8* getelementptr inbounds ([16 x i8]* @.str147, i64 0, i64 0), i8* getelementptr inbounds ([14 x i8]* @.str258, i64 0, i64 0)) #10, !dbg !352
  unreachable, !dbg !352

if.end:                                           ; preds = %entry
  ret void, !dbg !354
}

; Function Attrs: nounwind uwtable
define i32 @klee_range(i32 %start, i32 %end, i8* %name) #7 {
entry:
  %x = alloca i32, align 4
  %cmp = icmp slt i32 %start, %end, !dbg !355
  br i1 %cmp, label %if.end, label %if.then, !dbg !355

if.then:                                          ; preds = %entry
  call void @klee_report_error(i8* getelementptr inbounds ([47 x i8]* @.str6, i64 0, i64 0), i32 17, i8* getelementptr inbounds ([14 x i8]* @.str17, i64 0, i64 0), i8* getelementptr inbounds ([5 x i8]* @.str28, i64 0, i64 0)) #10, !dbg !357
  unreachable, !dbg !357

if.end:                                           ; preds = %entry
  %add = add nsw i32 %start, 1, !dbg !358
  %cmp1 = icmp eq i32 %add, %end, !dbg !358
  br i1 %cmp1, label %return, label %if.else, !dbg !358

if.else:                                          ; preds = %if.end
  %0 = bitcast i32* %x to i8*, !dbg !360
  call void @klee_make_symbolic(i8* %0, i64 4, i8* %name) #9, !dbg !360
  %cmp3 = icmp eq i32 %start, 0, !dbg !362
  %1 = load i32* %x, align 4, !dbg !364, !tbaa !346
  br i1 %cmp3, label %if.then4, label %if.else7, !dbg !362

if.then4:                                         ; preds = %if.else
  %cmp5 = icmp ult i32 %1, %end, !dbg !364
  %conv6 = zext i1 %cmp5 to i64, !dbg !364
  call void @klee_assume(i64 %conv6) #9, !dbg !364
  br label %if.end14, !dbg !366

if.else7:                                         ; preds = %if.else
  %cmp8 = icmp sge i32 %1, %start, !dbg !367
  %conv10 = zext i1 %cmp8 to i64, !dbg !367
  call void @klee_assume(i64 %conv10) #9, !dbg !367
  %2 = load i32* %x, align 4, !dbg !369, !tbaa !346
  %cmp11 = icmp slt i32 %2, %end, !dbg !369
  %conv13 = zext i1 %cmp11 to i64, !dbg !369
  call void @klee_assume(i64 %conv13) #9, !dbg !369
  br label %if.end14

if.end14:                                         ; preds = %if.else7, %if.then4
  %3 = load i32* %x, align 4, !dbg !370, !tbaa !346
  br label %return, !dbg !370

return:                                           ; preds = %if.end14, %if.end
  %retval.0 = phi i32 [ %3, %if.end14 ], [ %start, %if.end ]
  ret i32 %retval.0, !dbg !371
}

; Function Attrs: nounwind uwtable
define void @klee_set_rounding_mode(i32 %rm) #7 {
entry:
  switch i32 %rm, label %sw.default [
    i32 0, label %sw.bb
    i32 1, label %sw.bb1
    i32 2, label %sw.bb2
    i32 3, label %sw.bb3
    i32 4, label %sw.bb4
  ], !dbg !372

sw.bb:                                            ; preds = %entry
  tail call void @klee_set_rounding_mode_internal(i32 0) #9, !dbg !373
  br label %sw.epilog, !dbg !373

sw.bb1:                                           ; preds = %entry
  tail call void @klee_set_rounding_mode_internal(i32 1) #9, !dbg !375
  br label %sw.epilog, !dbg !375

sw.bb2:                                           ; preds = %entry
  tail call void @klee_set_rounding_mode_internal(i32 2) #9, !dbg !376
  br label %sw.epilog, !dbg !376

sw.bb3:                                           ; preds = %entry
  tail call void @klee_set_rounding_mode_internal(i32 3) #9, !dbg !377
  br label %sw.epilog, !dbg !377

sw.bb4:                                           ; preds = %entry
  tail call void @klee_set_rounding_mode_internal(i32 4) #9, !dbg !378
  br label %sw.epilog, !dbg !378

sw.default:                                       ; preds = %entry
  tail call void @klee_report_error(i8* getelementptr inbounds ([59 x i8]* @.str9, i64 0, i64 0), i32 31, i8* getelementptr inbounds ([22 x i8]* @.str110, i64 0, i64 0), i8* getelementptr inbounds ([1 x i8]* @.str211, i64 0, i64 0)) #10, !dbg !379
  unreachable, !dbg !379

sw.epilog:                                        ; preds = %sw.bb4, %sw.bb3, %sw.bb2, %sw.bb1, %sw.bb
  ret void, !dbg !380
}

declare void @klee_set_rounding_mode_internal(i32) #4

; Function Attrs: nounwind uwtable
define weak i8* @memcpy(i8* %destaddr, i8* %srcaddr, i64 %len) #7 {
entry:
  %cmp3 = icmp eq i64 %len, 0, !dbg !381
  br i1 %cmp3, label %while.end, label %while.body.preheader, !dbg !381

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
  %1 = bitcast i8* %next.gep to <16 x i8>*, !dbg !382
  %wide.load = load <16 x i8>* %1, align 1, !dbg !382
  %next.gep.sum282 = or i64 %index, 16, !dbg !382
  %2 = getelementptr i8* %srcaddr, i64 %next.gep.sum282, !dbg !382
  %3 = bitcast i8* %2 to <16 x i8>*, !dbg !382
  %wide.load203 = load <16 x i8>* %3, align 1, !dbg !382
  %4 = bitcast i8* %next.gep106 to <16 x i8>*, !dbg !382
  store <16 x i8> %wide.load, <16 x i8>* %4, align 1, !dbg !382
  %next.gep106.sum299 = or i64 %index, 16, !dbg !382
  %5 = getelementptr i8* %destaddr, i64 %next.gep106.sum299, !dbg !382
  %6 = bitcast i8* %5 to <16 x i8>*, !dbg !382
  store <16 x i8> %wide.load203, <16 x i8>* %6, align 1, !dbg !382
  %index.next = add i64 %index, 32
  %7 = icmp eq i64 %index.next, %n.vec
  br i1 %7, label %middle.block, label %vector.body, !llvm.loop !383

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
  %dec = add i64 %len.addr.04, -1, !dbg !381
  %incdec.ptr = getelementptr inbounds i8* %src.06, i64 1, !dbg !382
  %8 = load i8* %src.06, align 1, !dbg !382, !tbaa !386
  %incdec.ptr1 = getelementptr inbounds i8* %dest.05, i64 1, !dbg !382
  store i8 %8, i8* %dest.05, align 1, !dbg !382, !tbaa !386
  %cmp = icmp eq i64 %dec, 0, !dbg !381
  br i1 %cmp, label %while.end, label %while.body, !dbg !381, !llvm.loop !387

while.end:                                        ; preds = %while.body, %middle.block, %entry
  ret i8* %destaddr, !dbg !388
}

; Function Attrs: nounwind uwtable
define weak i8* @memmove(i8* %dst, i8* %src, i64 %count) #7 {
entry:
  %cmp = icmp eq i8* %src, %dst, !dbg !389
  br i1 %cmp, label %return, label %if.end, !dbg !389

if.end:                                           ; preds = %entry
  %cmp1 = icmp ugt i8* %src, %dst, !dbg !391
  br i1 %cmp1, label %while.cond.preheader, label %if.else, !dbg !391

while.cond.preheader:                             ; preds = %if.end
  %tobool27 = icmp eq i64 %count, 0, !dbg !393
  br i1 %tobool27, label %return, label %while.body.preheader, !dbg !393

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
  %1 = bitcast i8* %next.gep to <16 x i8>*, !dbg !393
  %wide.load = load <16 x i8>* %1, align 1, !dbg !393
  %next.gep.sum610 = or i64 %index, 16, !dbg !393
  %2 = getelementptr i8* %src, i64 %next.gep.sum610, !dbg !393
  %3 = bitcast i8* %2 to <16 x i8>*, !dbg !393
  %wide.load233 = load <16 x i8>* %3, align 1, !dbg !393
  %4 = bitcast i8* %next.gep136 to <16 x i8>*, !dbg !393
  store <16 x i8> %wide.load, <16 x i8>* %4, align 1, !dbg !393
  %next.gep136.sum627 = or i64 %index, 16, !dbg !393
  %5 = getelementptr i8* %dst, i64 %next.gep136.sum627, !dbg !393
  %6 = bitcast i8* %5 to <16 x i8>*, !dbg !393
  store <16 x i8> %wide.load233, <16 x i8>* %6, align 1, !dbg !393
  %index.next = add i64 %index, 32
  %7 = icmp eq i64 %index.next, %n.vec
  br i1 %7, label %middle.block, label %vector.body, !llvm.loop !395

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
  %dec = add i64 %count.addr.028, -1, !dbg !393
  %incdec.ptr = getelementptr inbounds i8* %b.030, i64 1, !dbg !393
  %8 = load i8* %b.030, align 1, !dbg !393, !tbaa !386
  %incdec.ptr3 = getelementptr inbounds i8* %a.029, i64 1, !dbg !393
  store i8 %8, i8* %a.029, align 1, !dbg !393, !tbaa !386
  %tobool = icmp eq i64 %dec, 0, !dbg !393
  br i1 %tobool, label %return, label %while.body, !dbg !393, !llvm.loop !396

if.else:                                          ; preds = %if.end
  %sub = add i64 %count, -1, !dbg !397
  %tobool832 = icmp eq i64 %count, 0, !dbg !399
  br i1 %tobool832, label %return, label %while.body9.lr.ph, !dbg !399

while.body9.lr.ph:                                ; preds = %if.else
  %add.ptr5 = getelementptr inbounds i8* %src, i64 %sub, !dbg !400
  %add.ptr = getelementptr inbounds i8* %dst, i64 %sub, !dbg !397
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
  %next.gep262.sum = add i64 %add.ptr5.sum465, -15, !dbg !399
  %10 = getelementptr i8* %src, i64 %next.gep262.sum, !dbg !399
  %11 = bitcast i8* %10 to <16 x i8>*, !dbg !399
  %wide.load460 = load <16 x i8>* %11, align 1, !dbg !399
  %reverse = shufflevector <16 x i8> %wide.load460, <16 x i8> undef, <16 x i32> <i32 15, i32 14, i32 13, i32 12, i32 11, i32 10, i32 9, i32 8, i32 7, i32 6, i32 5, i32 4, i32 3, i32 2, i32 1, i32 0>, !dbg !399
  %.sum = add i64 %add.ptr5.sum465, -31, !dbg !399
  %12 = getelementptr i8* %src, i64 %.sum, !dbg !399
  %13 = bitcast i8* %12 to <16 x i8>*, !dbg !399
  %wide.load461 = load <16 x i8>* %13, align 1, !dbg !399
  %reverse462 = shufflevector <16 x i8> %wide.load461, <16 x i8> undef, <16 x i32> <i32 15, i32 14, i32 13, i32 12, i32 11, i32 10, i32 9, i32 8, i32 7, i32 6, i32 5, i32 4, i32 3, i32 2, i32 1, i32 0>, !dbg !399
  %reverse463 = shufflevector <16 x i8> %reverse, <16 x i8> undef, <16 x i32> <i32 15, i32 14, i32 13, i32 12, i32 11, i32 10, i32 9, i32 8, i32 7, i32 6, i32 5, i32 4, i32 3, i32 2, i32 1, i32 0>, !dbg !399
  %next.gep359.sum = add i64 %add.ptr.sum497, -15, !dbg !399
  %14 = getelementptr i8* %dst, i64 %next.gep359.sum, !dbg !399
  %15 = bitcast i8* %14 to <16 x i8>*, !dbg !399
  store <16 x i8> %reverse463, <16 x i8>* %15, align 1, !dbg !399
  %reverse464 = shufflevector <16 x i8> %reverse462, <16 x i8> undef, <16 x i32> <i32 15, i32 14, i32 13, i32 12, i32 11, i32 10, i32 9, i32 8, i32 7, i32 6, i32 5, i32 4, i32 3, i32 2, i32 1, i32 0>, !dbg !399
  %.sum531 = add i64 %add.ptr.sum497, -31, !dbg !399
  %16 = getelementptr i8* %dst, i64 %.sum531, !dbg !399
  %17 = bitcast i8* %16 to <16 x i8>*, !dbg !399
  store <16 x i8> %reverse464, <16 x i8>* %17, align 1, !dbg !399
  %index.next260 = add i64 %index238, 32
  %18 = icmp eq i64 %index.next260, %n.vec241
  br i1 %18, label %middle.block236, label %vector.body235, !llvm.loop !401

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
  %dec7 = add i64 %count.addr.133, -1, !dbg !399
  %incdec.ptr10 = getelementptr inbounds i8* %b.135, i64 -1, !dbg !399
  %19 = load i8* %b.135, align 1, !dbg !399, !tbaa !386
  %incdec.ptr11 = getelementptr inbounds i8* %a.134, i64 -1, !dbg !399
  store i8 %19, i8* %a.134, align 1, !dbg !399, !tbaa !386
  %tobool8 = icmp eq i64 %dec7, 0, !dbg !399
  br i1 %tobool8, label %return, label %while.body9, !dbg !399, !llvm.loop !402

return:                                           ; preds = %while.body9, %middle.block236, %if.else, %while.body, %middle.block, %while.cond.preheader, %entry
  ret i8* %dst, !dbg !403
}

; Function Attrs: nounwind uwtable
define weak i8* @mempcpy(i8* %destaddr, i8* %srcaddr, i64 %len) #7 {
entry:
  %cmp3 = icmp eq i64 %len, 0, !dbg !404
  br i1 %cmp3, label %while.end, label %while.body.preheader, !dbg !404

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
  %1 = bitcast i8* %next.gep to <16 x i8>*, !dbg !405
  %wide.load = load <16 x i8>* %1, align 1, !dbg !405
  %next.gep.sum283 = or i64 %index, 16, !dbg !405
  %2 = getelementptr i8* %srcaddr, i64 %next.gep.sum283, !dbg !405
  %3 = bitcast i8* %2 to <16 x i8>*, !dbg !405
  %wide.load204 = load <16 x i8>* %3, align 1, !dbg !405
  %4 = bitcast i8* %next.gep107 to <16 x i8>*, !dbg !405
  store <16 x i8> %wide.load, <16 x i8>* %4, align 1, !dbg !405
  %next.gep107.sum300 = or i64 %index, 16, !dbg !405
  %5 = getelementptr i8* %destaddr, i64 %next.gep107.sum300, !dbg !405
  %6 = bitcast i8* %5 to <16 x i8>*, !dbg !405
  store <16 x i8> %wide.load204, <16 x i8>* %6, align 1, !dbg !405
  %index.next = add i64 %index, 32
  %7 = icmp eq i64 %index.next, %n.vec
  br i1 %7, label %middle.block, label %vector.body, !llvm.loop !406

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
  %dec = add i64 %len.addr.04, -1, !dbg !404
  %incdec.ptr = getelementptr inbounds i8* %src.06, i64 1, !dbg !405
  %8 = load i8* %src.06, align 1, !dbg !405, !tbaa !386
  %incdec.ptr1 = getelementptr inbounds i8* %dest.05, i64 1, !dbg !405
  store i8 %8, i8* %dest.05, align 1, !dbg !405, !tbaa !386
  %cmp = icmp eq i64 %dec, 0, !dbg !404
  br i1 %cmp, label %while.cond.while.end_crit_edge, label %while.body, !dbg !404, !llvm.loop !407

while.cond.while.end_crit_edge:                   ; preds = %while.body, %middle.block
  %scevgep = getelementptr i8* %destaddr, i64 %len
  br label %while.end, !dbg !404

while.end:                                        ; preds = %while.cond.while.end_crit_edge, %entry
  %dest.0.lcssa = phi i8* [ %scevgep, %while.cond.while.end_crit_edge ], [ %destaddr, %entry ]
  ret i8* %dest.0.lcssa, !dbg !408
}

; Function Attrs: nounwind uwtable
define weak i8* @memset(i8* %dst, i32 %s, i64 %count) #7 {
entry:
  %cmp2 = icmp eq i64 %count, 0, !dbg !409
  br i1 %cmp2, label %while.end, label %while.body.lr.ph, !dbg !409

while.body.lr.ph:                                 ; preds = %entry
  %conv = trunc i32 %s to i8, !dbg !410
  br label %while.body, !dbg !409

while.body:                                       ; preds = %while.body, %while.body.lr.ph
  %a.04 = phi i8* [ %dst, %while.body.lr.ph ], [ %incdec.ptr, %while.body ]
  %count.addr.03 = phi i64 [ %count, %while.body.lr.ph ], [ %dec, %while.body ]
  %dec = add i64 %count.addr.03, -1, !dbg !409
  %incdec.ptr = getelementptr inbounds i8* %a.04, i64 1, !dbg !410
  store volatile i8 %conv, i8* %a.04, align 1, !dbg !410, !tbaa !386
  %cmp = icmp eq i64 %dec, 0, !dbg !409
  br i1 %cmp, label %while.end, label %while.body, !dbg !409

while.end:                                        ; preds = %while.body, %entry
  ret i8* %dst, !dbg !411
}

; Function Attrs: nounwind uwtable
define double @klee_internal_sqrt(double %d) #7 {
entry:
  %call = tail call double @klee_sqrt_double(double %d) #9, !dbg !412
  ret double %call, !dbg !412
}

declare double @klee_sqrt_double(double) #4

; Function Attrs: nounwind uwtable
define float @klee_internal_sqrtf(float %f) #7 {
entry:
  %call = tail call float @klee_sqrt_float(float %f) #9, !dbg !413
  ret float %call, !dbg !413
}

declare float @klee_sqrt_float(float) #4

; Function Attrs: nounwind uwtable
define x86_fp80 @klee_internal_sqrtl(x86_fp80 %f) #7 {
entry:
  %call = tail call x86_fp80 @klee_sqrt_long_double(x86_fp80 %f) #9, !dbg !414
  ret x86_fp80 %call, !dbg !414
}

declare x86_fp80 @klee_sqrt_long_double(x86_fp80) #4

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float
attributes #1 = { nounwind readnone }
attributes #2 = { noreturn "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false
attributes #3 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { noinline nounwind optnone uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { noinline optnone }
attributes #7 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #8 = { noreturn "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #9 = { nobuiltin nounwind }
attributes #10 = { nobuiltin noreturn nounwind }

!llvm.dbg.cu = !{!0, !18, !40, !71, !116, !126, !138, !149, !161, !171, !189, !203, !217, !232}
!llvm.module.flags = !{!245, !246}
!llvm.ident = !{!247, !247, !247, !247, !247, !247, !247, !247, !247, !247, !247, !247, !247, !247}

!0 = metadata !{i32 786449, metadata !1, i32 12, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 false, met
!1 = metadata !{metadata !"df2cf_cp_l1_klee_annotated.c", metadata !"/home/klee/logic_bombs/src/covert_propogation"}
!2 = metadata !{i32 0}
!3 = metadata !{metadata !4, metadata !10, metadata !14}
!4 = metadata !{i32 786478, metadata !1, metadata !5, metadata !"df2cf", metadata !"df2cf", metadata !"", i32 10, metadata !6, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, i32 (i8)* @df2cf, null, null, metadata !2, i32 11} ; [ DW_TAG_subprog
!5 = metadata !{i32 786473, metadata !1}          ; [ DW_TAG_file_type ] [/home/klee/logic_bombs/src/covert_propogation/df2cf_cp_l1_klee_annotated.c]
!6 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !7, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!7 = metadata !{metadata !8, metadata !9}
!8 = metadata !{i32 786468, null, null, metadata !"int", i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ] [int] [line 0, size 32, align 32, offset 0, enc DW_ATE_signed]
!9 = metadata !{i32 786468, null, null, metadata !"char", i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ] [char] [line 0, size 8, align 8, offset 0, enc DW_ATE_signed_char]
!10 = metadata !{i32 786478, metadata !1, metadata !5, metadata !"logic_bomb", metadata !"logic_bomb", metadata !"", i32 52, metadata !11, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, i32 (i8*)* @logic_bomb, null, null, metadata !2, i32 52} 
!11 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !12, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!12 = metadata !{metadata !8, metadata !13}
!13 = metadata !{i32 786447, null, null, metadata !"", i32 0, i64 64, i64 64, i64 0, i32 0, metadata !9} ; [ DW_TAG_pointer_type ] [line 0, size 64, align 64, offset 0] [from char]
!14 = metadata !{i32 786478, metadata !1, metadata !5, metadata !"main", metadata !"main", metadata !"", i32 63, metadata !15, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, i32 (i32, i8**)* @main, null, null, metadata !2, i32 63} ; [ DW_TAG_s
!15 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !16, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!16 = metadata !{metadata !8, metadata !8, metadata !17}
!17 = metadata !{i32 786447, null, null, metadata !"", i32 0, i64 64, i64 64, i64 0, i32 0, metadata !13} ; [ DW_TAG_pointer_type ] [line 0, size 64, align 64, offset 0] [from ]
!18 = metadata !{i32 786449, metadata !19, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, met
!19 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/fabs.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!20 = metadata !{metadata !21, metadata !28, metadata !34}
!21 = metadata !{i32 786478, metadata !19, metadata !22, metadata !"klee_internal_fabs", metadata !"klee_internal_fabs", metadata !"", i32 11, metadata !23, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, double (double)* @klee_internal_fabs, nu
!22 = metadata !{i32 786473, metadata !19}        ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fabs.c]
!23 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !24, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!24 = metadata !{metadata !25, metadata !25}
!25 = metadata !{i32 786468, null, null, metadata !"double", i32 0, i64 64, i64 64, i64 0, i32 0, i32 4} ; [ DW_TAG_base_type ] [double] [line 0, size 64, align 64, offset 0, enc DW_ATE_float]
!26 = metadata !{metadata !27}
!27 = metadata !{i32 786689, metadata !21, metadata !"d", metadata !22, i32 16777227, metadata !25, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [d] [line 11]
!28 = metadata !{i32 786478, metadata !19, metadata !22, metadata !"klee_internal_fabsf", metadata !"klee_internal_fabsf", metadata !"", i32 15, metadata !29, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, float (float)* @klee_internal_fabsf, n
!29 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !30, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!30 = metadata !{metadata !31, metadata !31}
!31 = metadata !{i32 786468, null, null, metadata !"float", i32 0, i64 32, i64 32, i64 0, i32 0, i32 4} ; [ DW_TAG_base_type ] [float] [line 0, size 32, align 32, offset 0, enc DW_ATE_float]
!32 = metadata !{metadata !33}
!33 = metadata !{i32 786689, metadata !28, metadata !"f", metadata !22, i32 16777231, metadata !31, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 15]
!34 = metadata !{i32 786478, metadata !19, metadata !22, metadata !"klee_internal_fabsl", metadata !"klee_internal_fabsl", metadata !"", i32 20, metadata !35, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, x86_fp80 (x86_fp80)* @klee_internal_fa
!35 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !36, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!36 = metadata !{metadata !37, metadata !37}
!37 = metadata !{i32 786468, null, null, metadata !"long double", i32 0, i64 128, i64 128, i64 0, i32 0, i32 4} ; [ DW_TAG_base_type ] [long double] [line 0, size 128, align 128, offset 0, enc DW_ATE_float]
!38 = metadata !{metadata !39}
!39 = metadata !{i32 786689, metadata !34, metadata !"f", metadata !22, i32 16777236, metadata !37, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 20]
!40 = metadata !{i32 786449, metadata !41, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, met
!41 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/fenv.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!42 = metadata !{metadata !43, metadata !52}
!43 = metadata !{i32 786436, metadata !44, null, metadata !"KleeRoundingMode", i32 185, i64 32, i64 32, i32 0, i32 0, null, metadata !45, i32 0, null, null, null} ; [ DW_TAG_enumeration_type ] [KleeRoundingMode] [line 185, size 32, align 32, offset 0] [d
!44 = metadata !{metadata !"/home/user/klee/include/klee/klee.h", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!45 = metadata !{metadata !46, metadata !47, metadata !48, metadata !49, metadata !50, metadata !51}
!46 = metadata !{i32 786472, metadata !"KLEE_FP_RNE", i64 0} ; [ DW_TAG_enumerator ] [KLEE_FP_RNE :: 0]
!47 = metadata !{i32 786472, metadata !"KLEE_FP_RNA", i64 1} ; [ DW_TAG_enumerator ] [KLEE_FP_RNA :: 1]
!48 = metadata !{i32 786472, metadata !"KLEE_FP_RU", i64 2} ; [ DW_TAG_enumerator ] [KLEE_FP_RU :: 2]
!49 = metadata !{i32 786472, metadata !"KLEE_FP_RD", i64 3} ; [ DW_TAG_enumerator ] [KLEE_FP_RD :: 3]
!50 = metadata !{i32 786472, metadata !"KLEE_FP_RZ", i64 4} ; [ DW_TAG_enumerator ] [KLEE_FP_RZ :: 4]
!51 = metadata !{i32 786472, metadata !"KLEE_FP_UNKNOWN", i64 5} ; [ DW_TAG_enumerator ] [KLEE_FP_UNKNOWN :: 5]
!52 = metadata !{i32 786436, metadata !41, null, metadata !"", i32 15, i64 32, i64 32, i32 0, i32 0, null, metadata !53, i32 0, null, null, null} ; [ DW_TAG_enumeration_type ] [line 15, size 32, align 32, offset 0] [def] [from ]
!53 = metadata !{metadata !54, metadata !55, metadata !56, metadata !57, metadata !58}
!54 = metadata !{i32 786472, metadata !"FE_TONEAREST", i64 0} ; [ DW_TAG_enumerator ] [FE_TONEAREST :: 0]
!55 = metadata !{i32 786472, metadata !"FE_DOWNWARD", i64 1024} ; [ DW_TAG_enumerator ] [FE_DOWNWARD :: 1024]
!56 = metadata !{i32 786472, metadata !"FE_UPWARD", i64 2048} ; [ DW_TAG_enumerator ] [FE_UPWARD :: 2048]
!57 = metadata !{i32 786472, metadata !"FE_TOWARDZERO", i64 3072} ; [ DW_TAG_enumerator ] [FE_TOWARDZERO :: 3072]
!58 = metadata !{i32 786472, metadata !"FE_TONEAREST_TIES_TO_AWAY", i64 3073} ; [ DW_TAG_enumerator ] [FE_TONEAREST_TIES_TO_AWAY :: 3073]
!59 = metadata !{metadata !60, metadata !66}
!60 = metadata !{i32 786478, metadata !41, metadata !61, metadata !"klee_internal_fegetround", metadata !"klee_internal_fegetround", metadata !"", i32 33, metadata !62, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 ()* @klee_internal_feget
!61 = metadata !{i32 786473, metadata !41}        ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fenv.c]
!62 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !63, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!63 = metadata !{metadata !8}
!64 = metadata !{metadata !65}
!65 = metadata !{i32 786688, metadata !60, metadata !"rm", metadata !61, i32 34, metadata !43, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [rm] [line 34]
!66 = metadata !{i32 786478, metadata !41, metadata !61, metadata !"klee_internal_fesetround", metadata !"klee_internal_fesetround", metadata !"", i32 52, metadata !67, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (i32)* @klee_internal_fe
!67 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !68, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!68 = metadata !{metadata !8, metadata !8}
!69 = metadata !{metadata !70}
!70 = metadata !{i32 786689, metadata !66, metadata !"rm", metadata !61, i32 16777268, metadata !8, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [rm] [line 52]
!71 = metadata !{i32 786449, metadata !72, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, met
!72 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/fpclassify.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!73 = metadata !{metadata !74}
!74 = metadata !{i32 786436, metadata !72, null, metadata !"", i32 58, i64 32, i64 32, i32 0, i32 0, null, metadata !75, i32 0, null, null, null} ; [ DW_TAG_enumeration_type ] [line 58, size 32, align 32, offset 0] [def] [from ]
!75 = metadata !{metadata !76, metadata !77, metadata !78, metadata !79, metadata !80}
!76 = metadata !{i32 786472, metadata !"FP_NAN", i64 0} ; [ DW_TAG_enumerator ] [FP_NAN :: 0]
!77 = metadata !{i32 786472, metadata !"FP_INFINITE", i64 1} ; [ DW_TAG_enumerator ] [FP_INFINITE :: 1]
!78 = metadata !{i32 786472, metadata !"FP_ZERO", i64 2} ; [ DW_TAG_enumerator ] [FP_ZERO :: 2]
!79 = metadata !{i32 786472, metadata !"FP_SUBNORMAL", i64 3} ; [ DW_TAG_enumerator ] [FP_SUBNORMAL :: 3]
!80 = metadata !{i32 786472, metadata !"FP_NORMAL", i64 4} ; [ DW_TAG_enumerator ] [FP_NORMAL :: 4]
!81 = metadata !{metadata !82, metadata !88, metadata !93, metadata !98, metadata !101, metadata !104, metadata !107, metadata !110, metadata !113}
!82 = metadata !{i32 786478, metadata !72, metadata !83, metadata !"klee_internal_isnanf", metadata !"klee_internal_isnanf", metadata !"", i32 16, metadata !84, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (float)* @klee_internal_isnanf, 
!83 = metadata !{i32 786473, metadata !72}        ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!84 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !85, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!85 = metadata !{metadata !8, metadata !31}
!86 = metadata !{metadata !87}
!87 = metadata !{i32 786689, metadata !82, metadata !"f", metadata !83, i32 16777232, metadata !31, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 16]
!88 = metadata !{i32 786478, metadata !72, metadata !83, metadata !"klee_internal_isnan", metadata !"klee_internal_isnan", metadata !"", i32 21, metadata !89, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (double)* @klee_internal_isnan, nu
!89 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !90, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!90 = metadata !{metadata !8, metadata !25}
!91 = metadata !{metadata !92}
!92 = metadata !{i32 786689, metadata !88, metadata !"d", metadata !83, i32 16777237, metadata !25, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [d] [line 21]
!93 = metadata !{i32 786478, metadata !72, metadata !83, metadata !"klee_internal_isnanl", metadata !"klee_internal_isnanl", metadata !"", i32 26, metadata !94, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (x86_fp80)* @klee_internal_isnan
!94 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !95, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!95 = metadata !{metadata !8, metadata !37}
!96 = metadata !{metadata !97}
!97 = metadata !{i32 786689, metadata !93, metadata !"d", metadata !83, i32 16777242, metadata !37, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [d] [line 26]
!98 = metadata !{i32 786478, metadata !72, metadata !83, metadata !"klee_internal_fpclassifyf", metadata !"klee_internal_fpclassifyf", metadata !"", i32 67, metadata !84, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (float)* @klee_interna
!99 = metadata !{metadata !100}
!100 = metadata !{i32 786689, metadata !98, metadata !"f", metadata !83, i32 16777283, metadata !31, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 67]
!101 = metadata !{i32 786478, metadata !72, metadata !83, metadata !"klee_internal_fpclassify", metadata !"klee_internal_fpclassify", metadata !"", i32 82, metadata !89, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (double)* @klee_interna
!102 = metadata !{metadata !103}
!103 = metadata !{i32 786689, metadata !101, metadata !"f", metadata !83, i32 16777298, metadata !25, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 82]
!104 = metadata !{i32 786478, metadata !72, metadata !83, metadata !"klee_internal_fpclassifyl", metadata !"klee_internal_fpclassifyl", metadata !"", i32 98, metadata !94, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (x86_fp80)* @klee_int
!105 = metadata !{metadata !106}
!106 = metadata !{i32 786689, metadata !104, metadata !"ld", metadata !83, i32 16777314, metadata !37, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [ld] [line 98]
!107 = metadata !{i32 786478, metadata !72, metadata !83, metadata !"klee_internal_finitef", metadata !"klee_internal_finitef", metadata !"", i32 114, metadata !84, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (float)* @klee_internal_fini
!108 = metadata !{metadata !109}
!109 = metadata !{i32 786689, metadata !107, metadata !"f", metadata !83, i32 16777330, metadata !31, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 114]
!110 = metadata !{i32 786478, metadata !72, metadata !83, metadata !"klee_internal_finite", metadata !"klee_internal_finite", metadata !"", i32 119, metadata !89, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (double)* @klee_internal_finit
!111 = metadata !{metadata !112}
!112 = metadata !{i32 786689, metadata !110, metadata !"f", metadata !83, i32 16777335, metadata !25, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 119]
!113 = metadata !{i32 786478, metadata !72, metadata !83, metadata !"klee_internal_finitel", metadata !"klee_internal_finitel", metadata !"", i32 124, metadata !94, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (x86_fp80)* @klee_internal_f
!114 = metadata !{metadata !115}
!115 = metadata !{i32 786689, metadata !113, metadata !"f", metadata !83, i32 16777340, metadata !37, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 124]
!116 = metadata !{i32 786449, metadata !117, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!117 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/klee_div_zero_check.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!118 = metadata !{metadata !119}
!119 = metadata !{i32 786478, metadata !117, metadata !120, metadata !"klee_div_zero_check", metadata !"klee_div_zero_check", metadata !"", i32 12, metadata !121, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, void (i64)* @klee_div_zero_check, 
!120 = metadata !{i32 786473, metadata !117}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_div_zero_check.c]
!121 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !122, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!122 = metadata !{null, metadata !123}
!123 = metadata !{i32 786468, null, null, metadata !"long long int", i32 0, i64 64, i64 64, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ] [long long int] [line 0, size 64, align 64, offset 0, enc DW_ATE_signed]
!124 = metadata !{metadata !125}
!125 = metadata !{i32 786689, metadata !119, metadata !"z", metadata !120, i32 16777228, metadata !123, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [z] [line 12]
!126 = metadata !{i32 786449, metadata !127, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!127 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/klee_int.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!128 = metadata !{metadata !129}
!129 = metadata !{i32 786478, metadata !127, metadata !130, metadata !"klee_int", metadata !"klee_int", metadata !"", i32 13, metadata !131, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (i8*)* @klee_int, null, null, metadata !135, i32 13}
!130 = metadata !{i32 786473, metadata !127}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_int.c]
!131 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !132, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!132 = metadata !{metadata !8, metadata !133}
!133 = metadata !{i32 786447, null, null, metadata !"", i32 0, i64 64, i64 64, i64 0, i32 0, metadata !134} ; [ DW_TAG_pointer_type ] [line 0, size 64, align 64, offset 0] [from ]
!134 = metadata !{i32 786470, null, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, metadata !9} ; [ DW_TAG_const_type ] [line 0, size 0, align 0, offset 0] [from char]
!135 = metadata !{metadata !136, metadata !137}
!136 = metadata !{i32 786689, metadata !129, metadata !"name", metadata !130, i32 16777229, metadata !133, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [name] [line 13]
!137 = metadata !{i32 786688, metadata !129, metadata !"x", metadata !130, i32 14, metadata !8, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [x] [line 14]
!138 = metadata !{i32 786449, metadata !139, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!139 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/klee_overshift_check.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!140 = metadata !{metadata !141}
!141 = metadata !{i32 786478, metadata !139, metadata !142, metadata !"klee_overshift_check", metadata !"klee_overshift_check", metadata !"", i32 20, metadata !143, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, void (i64, i64)* @klee_overshift
!142 = metadata !{i32 786473, metadata !139}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_overshift_check.c]
!143 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !144, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!144 = metadata !{null, metadata !145, metadata !145}
!145 = metadata !{i32 786468, null, null, metadata !"long long unsigned int", i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ] [long long unsigned int] [line 0, size 64, align 64, offset 0, enc DW_ATE_unsigned]
!146 = metadata !{metadata !147, metadata !148}
!147 = metadata !{i32 786689, metadata !141, metadata !"bitWidth", metadata !142, i32 16777236, metadata !145, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [bitWidth] [line 20]
!148 = metadata !{i32 786689, metadata !141, metadata !"shift", metadata !142, i32 33554452, metadata !145, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [shift] [line 20]
!149 = metadata !{i32 786449, metadata !150, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!150 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/klee_range.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!151 = metadata !{metadata !152}
!152 = metadata !{i32 786478, metadata !150, metadata !153, metadata !"klee_range", metadata !"klee_range", metadata !"", i32 13, metadata !154, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i32 (i32, i32, i8*)* @klee_range, null, null, metada
!153 = metadata !{i32 786473, metadata !150}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_range.c]
!154 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !155, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!155 = metadata !{metadata !8, metadata !8, metadata !8, metadata !133}
!156 = metadata !{metadata !157, metadata !158, metadata !159, metadata !160}
!157 = metadata !{i32 786689, metadata !152, metadata !"start", metadata !153, i32 16777229, metadata !8, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [start] [line 13]
!158 = metadata !{i32 786689, metadata !152, metadata !"end", metadata !153, i32 33554445, metadata !8, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [end] [line 13]
!159 = metadata !{i32 786689, metadata !152, metadata !"name", metadata !153, i32 50331661, metadata !133, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [name] [line 13]
!160 = metadata !{i32 786688, metadata !152, metadata !"x", metadata !153, i32 14, metadata !8, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [x] [line 14]
!161 = metadata !{i32 786449, metadata !162, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!162 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/klee_set_rounding_mode.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!163 = metadata !{metadata !43}
!164 = metadata !{metadata !165}
!165 = metadata !{i32 786478, metadata !162, metadata !166, metadata !"klee_set_rounding_mode", metadata !"klee_set_rounding_mode", metadata !"", i32 16, metadata !167, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, void (i32)* @klee_set_roundi
!166 = metadata !{i32 786473, metadata !162}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_set_rounding_mode.c]
!167 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !168, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!168 = metadata !{null, metadata !43}
!169 = metadata !{metadata !170}
!170 = metadata !{i32 786689, metadata !165, metadata !"rm", metadata !166, i32 16777232, metadata !43, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [rm] [line 16]
!171 = metadata !{i32 786449, metadata !172, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!172 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/memcpy.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!173 = metadata !{metadata !174}
!174 = metadata !{i32 786478, metadata !172, metadata !175, metadata !"memcpy", metadata !"memcpy", metadata !"", i32 12, metadata !176, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i8* (i8*, i8*, i64)* @memcpy, null, null, metadata !183, i32
!175 = metadata !{i32 786473, metadata !172}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/memcpy.c]
!176 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !177, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!177 = metadata !{metadata !178, metadata !178, metadata !179, metadata !181}
!178 = metadata !{i32 786447, null, null, metadata !"", i32 0, i64 64, i64 64, i64 0, i32 0, null} ; [ DW_TAG_pointer_type ] [line 0, size 64, align 64, offset 0] [from ]
!179 = metadata !{i32 786447, null, null, metadata !"", i32 0, i64 64, i64 64, i64 0, i32 0, metadata !180} ; [ DW_TAG_pointer_type ] [line 0, size 64, align 64, offset 0] [from ]
!180 = metadata !{i32 786470, null, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null} ; [ DW_TAG_const_type ] [line 0, size 0, align 0, offset 0] [from ]
!181 = metadata !{i32 786454, metadata !172, null, metadata !"size_t", i32 42, i64 0, i64 0, i64 0, i32 0, metadata !182} ; [ DW_TAG_typedef ] [size_t] [line 42, size 0, align 0, offset 0] [from long unsigned int]
!182 = metadata !{i32 786468, null, null, metadata !"long unsigned int", i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ] [long unsigned int] [line 0, size 64, align 64, offset 0, enc DW_ATE_unsigned]
!183 = metadata !{metadata !184, metadata !185, metadata !186, metadata !187, metadata !188}
!184 = metadata !{i32 786689, metadata !174, metadata !"destaddr", metadata !175, i32 16777228, metadata !178, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [destaddr] [line 12]
!185 = metadata !{i32 786689, metadata !174, metadata !"srcaddr", metadata !175, i32 33554444, metadata !179, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [srcaddr] [line 12]
!186 = metadata !{i32 786689, metadata !174, metadata !"len", metadata !175, i32 50331660, metadata !181, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [len] [line 12]
!187 = metadata !{i32 786688, metadata !174, metadata !"dest", metadata !175, i32 13, metadata !13, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [dest] [line 13]
!188 = metadata !{i32 786688, metadata !174, metadata !"src", metadata !175, i32 14, metadata !133, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [src] [line 14]
!189 = metadata !{i32 786449, metadata !190, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!190 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/memmove.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!191 = metadata !{metadata !192}
!192 = metadata !{i32 786478, metadata !190, metadata !193, metadata !"memmove", metadata !"memmove", metadata !"", i32 12, metadata !194, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i8* (i8*, i8*, i64)* @memmove, null, null, metadata !197, 
!193 = metadata !{i32 786473, metadata !190}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/memmove.c]
!194 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !195, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!195 = metadata !{metadata !178, metadata !178, metadata !179, metadata !196}
!196 = metadata !{i32 786454, metadata !190, null, metadata !"size_t", i32 42, i64 0, i64 0, i64 0, i32 0, metadata !182} ; [ DW_TAG_typedef ] [size_t] [line 42, size 0, align 0, offset 0] [from long unsigned int]
!197 = metadata !{metadata !198, metadata !199, metadata !200, metadata !201, metadata !202}
!198 = metadata !{i32 786689, metadata !192, metadata !"dst", metadata !193, i32 16777228, metadata !178, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [dst] [line 12]
!199 = metadata !{i32 786689, metadata !192, metadata !"src", metadata !193, i32 33554444, metadata !179, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [src] [line 12]
!200 = metadata !{i32 786689, metadata !192, metadata !"count", metadata !193, i32 50331660, metadata !196, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [count] [line 12]
!201 = metadata !{i32 786688, metadata !192, metadata !"a", metadata !193, i32 13, metadata !13, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [a] [line 13]
!202 = metadata !{i32 786688, metadata !192, metadata !"b", metadata !193, i32 14, metadata !133, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [b] [line 14]
!203 = metadata !{i32 786449, metadata !204, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!204 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/mempcpy.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!205 = metadata !{metadata !206}
!206 = metadata !{i32 786478, metadata !204, metadata !207, metadata !"mempcpy", metadata !"mempcpy", metadata !"", i32 11, metadata !208, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i8* (i8*, i8*, i64)* @mempcpy, null, null, metadata !211, 
!207 = metadata !{i32 786473, metadata !204}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/mempcpy.c]
!208 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !209, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!209 = metadata !{metadata !178, metadata !178, metadata !179, metadata !210}
!210 = metadata !{i32 786454, metadata !204, null, metadata !"size_t", i32 42, i64 0, i64 0, i64 0, i32 0, metadata !182} ; [ DW_TAG_typedef ] [size_t] [line 42, size 0, align 0, offset 0] [from long unsigned int]
!211 = metadata !{metadata !212, metadata !213, metadata !214, metadata !215, metadata !216}
!212 = metadata !{i32 786689, metadata !206, metadata !"destaddr", metadata !207, i32 16777227, metadata !178, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [destaddr] [line 11]
!213 = metadata !{i32 786689, metadata !206, metadata !"srcaddr", metadata !207, i32 33554443, metadata !179, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [srcaddr] [line 11]
!214 = metadata !{i32 786689, metadata !206, metadata !"len", metadata !207, i32 50331659, metadata !210, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [len] [line 11]
!215 = metadata !{i32 786688, metadata !206, metadata !"dest", metadata !207, i32 12, metadata !13, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [dest] [line 12]
!216 = metadata !{i32 786688, metadata !206, metadata !"src", metadata !207, i32 13, metadata !133, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [src] [line 13]
!217 = metadata !{i32 786449, metadata !218, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!218 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/memset.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!219 = metadata !{metadata !220}
!220 = metadata !{i32 786478, metadata !218, metadata !221, metadata !"memset", metadata !"memset", metadata !"", i32 11, metadata !222, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, i8* (i8*, i32, i64)* @memset, null, null, metadata !225, i32
!221 = metadata !{i32 786473, metadata !218}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/memset.c]
!222 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !223, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!223 = metadata !{metadata !178, metadata !178, metadata !8, metadata !224}
!224 = metadata !{i32 786454, metadata !218, null, metadata !"size_t", i32 42, i64 0, i64 0, i64 0, i32 0, metadata !182} ; [ DW_TAG_typedef ] [size_t] [line 42, size 0, align 0, offset 0] [from long unsigned int]
!225 = metadata !{metadata !226, metadata !227, metadata !228, metadata !229}
!226 = metadata !{i32 786689, metadata !220, metadata !"dst", metadata !221, i32 16777227, metadata !178, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [dst] [line 11]
!227 = metadata !{i32 786689, metadata !220, metadata !"s", metadata !221, i32 33554443, metadata !8, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [s] [line 11]
!228 = metadata !{i32 786689, metadata !220, metadata !"count", metadata !221, i32 50331659, metadata !224, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [count] [line 11]
!229 = metadata !{i32 786688, metadata !220, metadata !"a", metadata !221, i32 12, metadata !230, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [a] [line 12]
!230 = metadata !{i32 786447, null, null, metadata !"", i32 0, i64 64, i64 64, i64 0, i32 0, metadata !231} ; [ DW_TAG_pointer_type ] [line 0, size 64, align 64, offset 0] [from ]
!231 = metadata !{i32 786485, null, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, metadata !9} ; [ DW_TAG_volatile_type ] [line 0, size 0, align 0, offset 0] [from char]
!232 = metadata !{i32 786449, metadata !233, i32 1, metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)", i1 true, m
!233 = metadata !{metadata !"/home/user/klee/runtime/Intrinsic/sqrt.c", metadata !"/home/user/klee/build/runtime/Intrinsic"}
!234 = metadata !{metadata !235, metadata !239, metadata !242}
!235 = metadata !{i32 786478, metadata !233, metadata !236, metadata !"klee_internal_sqrt", metadata !"klee_internal_sqrt", metadata !"", i32 11, metadata !23, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, double (double)* @klee_internal_sqrt,
!236 = metadata !{i32 786473, metadata !233}      ; [ DW_TAG_file_type ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/sqrt.c]
!237 = metadata !{metadata !238}
!238 = metadata !{i32 786689, metadata !235, metadata !"d", metadata !236, i32 16777227, metadata !25, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [d] [line 11]
!239 = metadata !{i32 786478, metadata !233, metadata !236, metadata !"klee_internal_sqrtf", metadata !"klee_internal_sqrtf", metadata !"", i32 15, metadata !29, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, float (float)* @klee_internal_sqrtf
!240 = metadata !{metadata !241}
!241 = metadata !{i32 786689, metadata !239, metadata !"f", metadata !236, i32 16777231, metadata !31, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 15]
!242 = metadata !{i32 786478, metadata !233, metadata !236, metadata !"klee_internal_sqrtl", metadata !"klee_internal_sqrtl", metadata !"", i32 20, metadata !35, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 true, x86_fp80 (x86_fp80)* @klee_internal
!243 = metadata !{metadata !244}
!244 = metadata !{i32 786689, metadata !242, metadata !"f", metadata !236, i32 16777236, metadata !37, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [f] [line 20]
!245 = metadata !{i32 2, metadata !"Dwarf Version", i32 4}
!246 = metadata !{i32 1, metadata !"Debug Info Version", i32 1}
!247 = metadata !{metadata !"clang version 3.4.2 (https://github.com/llvm-mirror/clang adb6bc4cf0e09a5aca773ab7680bc4c7bf581909) (https://github.com/llvm-mirror/llvm 5c6aa738fb3325ae499454877f1e2926d2368135)"}
!248 = metadata !{i32 13, i32 0, metadata !4, null}
!249 = metadata !{i32 15, i32 0, metadata !250, null}
!250 = metadata !{i32 786443, metadata !1, metadata !4, i32 13, i32 0, i32 0} ; [ DW_TAG_lexical_block ] [/home/klee/logic_bombs/src/covert_propogation/df2cf_cp_l1_klee_annotated.c]
!251 = metadata !{i32 16, i32 0, metadata !250, null}
!252 = metadata !{i32 18, i32 0, metadata !250, null}
!253 = metadata !{i32 19, i32 0, metadata !250, null}
!254 = metadata !{i32 21, i32 0, metadata !250, null}
!255 = metadata !{i32 22, i32 0, metadata !250, null}
!256 = metadata !{i32 24, i32 0, metadata !250, null}
!257 = metadata !{i32 25, i32 0, metadata !250, null}
!258 = metadata !{i32 27, i32 0, metadata !250, null}
!259 = metadata !{i32 28, i32 0, metadata !250, null}
!260 = metadata !{i32 30, i32 0, metadata !250, null}
!261 = metadata !{i32 31, i32 0, metadata !250, null}
!262 = metadata !{i32 33, i32 0, metadata !250, null}
!263 = metadata !{i32 34, i32 0, metadata !250, null}
!264 = metadata !{i32 36, i32 0, metadata !250, null}
!265 = metadata !{i32 37, i32 0, metadata !250, null}
!266 = metadata !{i32 39, i32 0, metadata !250, null}
!267 = metadata !{i32 40, i32 0, metadata !250, null}
!268 = metadata !{i32 42, i32 0, metadata !250, null}
!269 = metadata !{i32 43, i32 0, metadata !250, null}
!270 = metadata !{i32 45, i32 0, metadata !250, null}
!271 = metadata !{i32 46, i32 0, metadata !250, null}
!272 = metadata !{i32 48, i32 0, metadata !4, null}
!273 = metadata !{i32 53, i32 0, metadata !10, null}
!274 = metadata !{i32 54, i32 0, metadata !10, null}
!275 = metadata !{i32 55, i32 0, metadata !10, null}
!276 = metadata !{i32 56, i32 0, metadata !10, null}
!277 = metadata !{i32 57, i32 0, metadata !278, null}
!278 = metadata !{i32 786443, metadata !1, metadata !10, i32 57, i32 0, i32 1} ; [ DW_TAG_lexical_block ] [/home/klee/logic_bombs/src/covert_propogation/df2cf_cp_l1_klee_annotated.c]
!279 = metadata !{i32 58, i32 0, metadata !278, null} ; [ DW_TAG_imported_module ]
!280 = metadata !{i32 61, i32 0, metadata !10, null}
!281 = metadata !{i32 65, i32 0, metadata !14, null}
!282 = metadata !{i32 66, i32 0, metadata !14, null}
!283 = metadata !{i32 67, i32 0, metadata !14, null}
!284 = metadata !{i32 12, i32 0, metadata !21, null}
!285 = metadata !{i32 16, i32 0, metadata !28, null}
!286 = metadata !{i32 21, i32 0, metadata !34, null}
!287 = metadata !{i32 34, i32 0, metadata !60, null}
!288 = metadata !{i32 35, i32 0, metadata !60, null}
!289 = metadata !{i32 50, i32 0, metadata !60, null}
!290 = metadata !{i32 53, i32 0, metadata !66, null}
!291 = metadata !{i32 55, i32 0, metadata !292, null}
!292 = metadata !{i32 786443, metadata !41, metadata !66, i32 53, i32 0, i32 1} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fenv.c]
!293 = metadata !{i32 56, i32 0, metadata !292, null}
!294 = metadata !{i32 66, i32 0, metadata !292, null}
!295 = metadata !{i32 67, i32 0, metadata !292, null}
!296 = metadata !{i32 69, i32 0, metadata !292, null}
!297 = metadata !{i32 70, i32 0, metadata !292, null}
!298 = metadata !{i32 72, i32 0, metadata !292, null}
!299 = metadata !{i32 73, i32 0, metadata !292, null}
!300 = metadata !{i32 79, i32 0, metadata !66, null}
!301 = metadata !{i32 17, i32 0, metadata !82, null}
!302 = metadata !{i32 22, i32 0, metadata !88, null}
!303 = metadata !{i32 27, i32 0, metadata !93, null}
!304 = metadata !{i32 69, i32 0, metadata !305, null}
!305 = metadata !{i32 786443, metadata !72, metadata !98, i32 69, i32 0, i32 0} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!306 = metadata !{i32 71, i32 0, metadata !307, null}
!307 = metadata !{i32 786443, metadata !72, metadata !305, i32 71, i32 0, i32 2} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!308 = metadata !{i32 73, i32 0, metadata !309, null}
!309 = metadata !{i32 786443, metadata !72, metadata !307, i32 73, i32 0, i32 4} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!310 = metadata !{i32 75, i32 0, metadata !311, null}
!311 = metadata !{i32 786443, metadata !72, metadata !309, i32 75, i32 0, i32 6} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!312 = metadata !{i32 76, i32 0, metadata !313, null}
!313 = metadata !{i32 786443, metadata !72, metadata !311, i32 75, i32 0, i32 7} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!314 = metadata !{i32 79, i32 0, metadata !98, null}
!315 = metadata !{i32 84, i32 0, metadata !316, null}
!316 = metadata !{i32 786443, metadata !72, metadata !101, i32 84, i32 0, i32 8} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!317 = metadata !{i32 86, i32 0, metadata !318, null}
!318 = metadata !{i32 786443, metadata !72, metadata !316, i32 86, i32 0, i32 10} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!319 = metadata !{i32 88, i32 0, metadata !320, null}
!320 = metadata !{i32 786443, metadata !72, metadata !318, i32 88, i32 0, i32 12} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!321 = metadata !{i32 90, i32 0, metadata !322, null}
!322 = metadata !{i32 786443, metadata !72, metadata !320, i32 90, i32 0, i32 14} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!323 = metadata !{i32 91, i32 0, metadata !324, null}
!324 = metadata !{i32 786443, metadata !72, metadata !322, i32 90, i32 0, i32 15} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!325 = metadata !{i32 94, i32 0, metadata !101, null}
!326 = metadata !{i32 100, i32 0, metadata !327, null}
!327 = metadata !{i32 786443, metadata !72, metadata !104, i32 100, i32 0, i32 16} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!328 = metadata !{i32 102, i32 0, metadata !329, null}
!329 = metadata !{i32 786443, metadata !72, metadata !327, i32 102, i32 0, i32 18} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!330 = metadata !{i32 104, i32 0, metadata !331, null}
!331 = metadata !{i32 786443, metadata !72, metadata !329, i32 104, i32 0, i32 20} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!332 = metadata !{i32 106, i32 0, metadata !333, null}
!333 = metadata !{i32 786443, metadata !72, metadata !331, i32 106, i32 0, i32 22} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!334 = metadata !{i32 107, i32 0, metadata !335, null}
!335 = metadata !{i32 786443, metadata !72, metadata !333, i32 106, i32 0, i32 23} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/fpclassify.c]
!336 = metadata !{i32 110, i32 0, metadata !104, null}
!337 = metadata !{i32 115, i32 0, metadata !107, null}
!338 = metadata !{i32 120, i32 0, metadata !110, null}
!339 = metadata !{i32 125, i32 0, metadata !113, null}
!340 = metadata !{i32 13, i32 0, metadata !341, null}
!341 = metadata !{i32 786443, metadata !117, metadata !119, i32 13, i32 0, i32 0} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_div_zero_check.c]
!342 = metadata !{i32 14, i32 0, metadata !341, null}
!343 = metadata !{i32 15, i32 0, metadata !119, null}
!344 = metadata !{i32 15, i32 0, metadata !129, null}
!345 = metadata !{i32 16, i32 0, metadata !129, null}
!346 = metadata !{metadata !347, metadata !347, i64 0}
!347 = metadata !{metadata !"int", metadata !348, i64 0}
!348 = metadata !{metadata !"omnipotent char", metadata !349, i64 0}
!349 = metadata !{metadata !"Simple C/C++ TBAA"}
!350 = metadata !{i32 21, i32 0, metadata !351, null}
!351 = metadata !{i32 786443, metadata !139, metadata !141, i32 21, i32 0, i32 0} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_overshift_check.c]
!352 = metadata !{i32 27, i32 0, metadata !353, null}
!353 = metadata !{i32 786443, metadata !139, metadata !351, i32 21, i32 0, i32 1} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_overshift_check.c]
!354 = metadata !{i32 29, i32 0, metadata !141, null}
!355 = metadata !{i32 16, i32 0, metadata !356, null}
!356 = metadata !{i32 786443, metadata !150, metadata !152, i32 16, i32 0, i32 0} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_range.c]
!357 = metadata !{i32 17, i32 0, metadata !356, null}
!358 = metadata !{i32 19, i32 0, metadata !359, null}
!359 = metadata !{i32 786443, metadata !150, metadata !152, i32 19, i32 0, i32 1} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_range.c]
!360 = metadata !{i32 22, i32 0, metadata !361, null}
!361 = metadata !{i32 786443, metadata !150, metadata !359, i32 21, i32 0, i32 3} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_range.c]
!362 = metadata !{i32 25, i32 0, metadata !363, null}
!363 = metadata !{i32 786443, metadata !150, metadata !361, i32 25, i32 0, i32 4} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_range.c]
!364 = metadata !{i32 26, i32 0, metadata !365, null}
!365 = metadata !{i32 786443, metadata !150, metadata !363, i32 25, i32 0, i32 5} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_range.c]
!366 = metadata !{i32 27, i32 0, metadata !365, null}
!367 = metadata !{i32 28, i32 0, metadata !368, null}
!368 = metadata !{i32 786443, metadata !150, metadata !363, i32 27, i32 0, i32 6} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_range.c]
!369 = metadata !{i32 29, i32 0, metadata !368, null}
!370 = metadata !{i32 32, i32 0, metadata !361, null}
!371 = metadata !{i32 34, i32 0, metadata !152, null}
!372 = metadata !{i32 19, i32 0, metadata !165, null}
!373 = metadata !{i32 21, i32 0, metadata !374, null}
!374 = metadata !{i32 786443, metadata !162, metadata !165, i32 19, i32 0, i32 0} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/klee_set_rounding_mode.c]
!375 = metadata !{i32 23, i32 0, metadata !374, null}
!376 = metadata !{i32 25, i32 0, metadata !374, null}
!377 = metadata !{i32 27, i32 0, metadata !374, null}
!378 = metadata !{i32 29, i32 0, metadata !374, null}
!379 = metadata !{i32 31, i32 0, metadata !374, null}
!380 = metadata !{i32 33, i32 0, metadata !165, null}
!381 = metadata !{i32 16, i32 0, metadata !174, null}
!382 = metadata !{i32 17, i32 0, metadata !174, null}
!383 = metadata !{metadata !383, metadata !384, metadata !385}
!384 = metadata !{metadata !"llvm.vectorizer.width", i32 1}
!385 = metadata !{metadata !"llvm.vectorizer.unroll", i32 1}
!386 = metadata !{metadata !348, metadata !348, i64 0}
!387 = metadata !{metadata !387, metadata !384, metadata !385}
!388 = metadata !{i32 18, i32 0, metadata !174, null}
!389 = metadata !{i32 16, i32 0, metadata !390, null}
!390 = metadata !{i32 786443, metadata !190, metadata !192, i32 16, i32 0, i32 0} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/memmove.c]
!391 = metadata !{i32 19, i32 0, metadata !392, null}
!392 = metadata !{i32 786443, metadata !190, metadata !192, i32 19, i32 0, i32 1} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/memmove.c]
!393 = metadata !{i32 20, i32 0, metadata !394, null}
!394 = metadata !{i32 786443, metadata !190, metadata !392, i32 19, i32 0, i32 2} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/memmove.c]
!395 = metadata !{metadata !395, metadata !384, metadata !385}
!396 = metadata !{metadata !396, metadata !384, metadata !385}
!397 = metadata !{i32 22, i32 0, metadata !398, null}
!398 = metadata !{i32 786443, metadata !190, metadata !392, i32 21, i32 0, i32 3} ; [ DW_TAG_lexical_block ] [/home/user/klee/build/runtime/Intrinsic//home/user/klee/runtime/Intrinsic/memmove.c]
!399 = metadata !{i32 24, i32 0, metadata !398, null}
!400 = metadata !{i32 23, i32 0, metadata !398, null}
!401 = metadata !{metadata !401, metadata !384, metadata !385}
!402 = metadata !{metadata !402, metadata !384, metadata !385}
!403 = metadata !{i32 28, i32 0, metadata !192, null}
!404 = metadata !{i32 15, i32 0, metadata !206, null}
!405 = metadata !{i32 16, i32 0, metadata !206, null}
!406 = metadata !{metadata !406, metadata !384, metadata !385}
!407 = metadata !{metadata !407, metadata !384, metadata !385}
!408 = metadata !{i32 17, i32 0, metadata !206, null}
!409 = metadata !{i32 13, i32 0, metadata !220, null}
!410 = metadata !{i32 14, i32 0, metadata !220, null}
!411 = metadata !{i32 15, i32 0, metadata !220, null}
!412 = metadata !{i32 12, i32 0, metadata !235, null}
!413 = metadata !{i32 16, i32 0, metadata !239, null}
!414 = metadata !{i32 21, i32 0, metadata !242, null}
