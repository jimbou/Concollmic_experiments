; ModuleID = 'test.bc'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@n = dso_local global i32 5, align 4, !dbg !0
@.str = private unnamed_addr constant [28 x i8] c"0 && \22Logic bomb triggered\22\00", align 1
@.str.1 = private unnamed_addr constant [7 x i8] c"test.c\00", align 1
@__PRETTY_FUNCTION__.logic_bomb = private unnamed_addr constant [23 x i8] c"int logic_bomb(char *)\00", align 1
@.str.2 = private unnamed_addr constant [7 x i8] c"symvar\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @logic_bomb(i8* %symvar) #0 !dbg !16 {
entry:
  %retval = alloca i32, align 4
  %symvar.addr = alloca i8*, align 8
  %p = alloca i8*, align 8
  %q = alloca i8*, align 8
  store i8* %symvar, i8** %symvar.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %symvar.addr, metadata !19, metadata !DIExpression()), !dbg !20
  call void @llvm.dbg.declare(metadata i8** %p, metadata !21, metadata !DIExpression()), !dbg !22
  call void @llvm.dbg.declare(metadata i8** %q, metadata !23, metadata !DIExpression()), !dbg !24
  %call = call noalias align 16 i8* @malloc(i64 16) #5, !dbg !25
  store i8* %call, i8** %p, align 8, !dbg !26
  %call1 = call noalias align 16 i8* @malloc(i64 16) #5, !dbg !27
  store i8* %call1, i8** %q, align 8, !dbg !28
  %0 = load i8*, i8** %p, align 8, !dbg !29
  %1 = load i8*, i8** %symvar.addr, align 8, !dbg !30
  %call2 = call i8* @strcpy(i8* %0, i8* %1) #5, !dbg !31
  %2 = load i8*, i8** %q, align 8, !dbg !32
  call void @free(i8* %2) #5, !dbg !33
  %3 = load i32, i32* @n, align 4, !dbg !34
  %cmp = icmp ne i32 %3, 5, !dbg !36
  br i1 %cmp, label %if.then, label %if.else, !dbg !37

if.then:                                          ; preds = %entry
  %4 = load i8*, i8** %p, align 8, !dbg !38
  call void @free(i8* %4) #5, !dbg !40
  %call3 = call i32 (i8*, i8*, i32, i8*, ...) bitcast (i32 (...)* @__assert_fail to i32 (i8*, i8*, i32, i8*, ...)*)(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str, i64 0, i64 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i64 0, i64 0), i32 17, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.logic_bomb, i64 0, i64 0)), !dbg !41
  br label %if.end, !dbg !42

if.else:                                          ; preds = %entry
  %5 = load i8*, i8** %p, align 8, !dbg !43
  call void @free(i8* %5) #5, !dbg !45
  %call4 = call i32 (i8*, i8*, i32, i8*, ...) bitcast (i32 (...)* @__assert_fail to i32 (i8*, i8*, i32, i8*, ...)*)(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str, i64 0, i64 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i64 0, i64 0), i32 20, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.logic_bomb, i64 0, i64 0)), !dbg !46
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %6 = load i32, i32* %retval, align 4, !dbg !47
  ret i32 %6, !dbg !47
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nounwind
declare dso_local noalias align 16 i8* @malloc(i64) #2

; Function Attrs: nounwind
declare dso_local i8* @strcpy(i8*, i8*) #2

; Function Attrs: nounwind
declare dso_local void @free(i8*) #2

; Function Attrs: noreturn
declare dso_local i32 @__assert_fail(...) #3

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 %argc, i8** %argv) #0 !dbg !48 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %symvar = alloca [257 x i8], align 16
  store i32 0, i32* %retval, align 4
  store i32 %argc, i32* %argc.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %argc.addr, metadata !52, metadata !DIExpression()), !dbg !53
  store i8** %argv, i8*** %argv.addr, align 8
  call void @llvm.dbg.declare(metadata i8*** %argv.addr, metadata !54, metadata !DIExpression()), !dbg !55
  call void @llvm.dbg.declare(metadata [257 x i8]* %symvar, metadata !56, metadata !DIExpression()), !dbg !60
  %0 = bitcast [257 x i8]* %symvar to i8*, !dbg !61
  call void @klee_make_symbolic(i8* %0, i64 257, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.2, i64 0, i64 0)), !dbg !62
  %arrayidx = getelementptr inbounds [257 x i8], [257 x i8]* %symvar, i64 0, i64 256, !dbg !63
  %1 = load i8, i8* %arrayidx, align 16, !dbg !63
  %conv = sext i8 %1 to i32, !dbg !63
  %cmp = icmp eq i32 %conv, 0, !dbg !64
  %conv1 = zext i1 %cmp to i32, !dbg !64
  %conv2 = sext i32 %conv1 to i64, !dbg !63
  call void @klee_assume(i64 %conv2), !dbg !65
  %arraydecay = getelementptr inbounds [257 x i8], [257 x i8]* %symvar, i64 0, i64 0, !dbg !66
  %call = call i32 @logic_bomb(i8* %arraydecay), !dbg !67
  ret i32 %call, !dbg !68
}

declare dso_local void @klee_make_symbolic(i8*, i64, i8*) #4

declare dso_local void @klee_assume(i64) #4

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #2 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { noreturn "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!10, !11, !12, !13, !14}
!llvm.ident = !{!15}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "n", scope: !2, file: !3, line: 6, type: !9, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "clang version 13.0.1 (https://github.com/llvm/llvm-project.git 75e33f71c2dae584b13a7d1186ae0a038ba98838)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, retainedTypes: !5, globals: !8, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "test.c", directory: "/home/klee/logic_bombs/src/buffer_overflow")
!4 = !{}
!5 = !{!6}
!6 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !7, size: 64)
!7 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!8 = !{!0}
!9 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!10 = !{i32 7, !"Dwarf Version", i32 4}
!11 = !{i32 2, !"Debug Info Version", i32 3}
!12 = !{i32 1, !"wchar_size", i32 4}
!13 = !{i32 7, !"uwtable", i32 1}
!14 = !{i32 7, !"frame-pointer", i32 2}
!15 = !{!"clang version 13.0.1 (https://github.com/llvm/llvm-project.git 75e33f71c2dae584b13a7d1186ae0a038ba98838)"}
!16 = distinct !DISubprogram(name: "logic_bomb", scope: !3, file: !3, line: 9, type: !17, scopeLine: 9, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!17 = !DISubroutineType(types: !18)
!18 = !{!9, !6}
!19 = !DILocalVariable(name: "symvar", arg: 1, scope: !16, file: !3, line: 9, type: !6)
!20 = !DILocation(line: 9, column: 22, scope: !16)
!21 = !DILocalVariable(name: "p", scope: !16, file: !3, line: 10, type: !6)
!22 = !DILocation(line: 10, column: 11, scope: !16)
!23 = !DILocalVariable(name: "q", scope: !16, file: !3, line: 10, type: !6)
!24 = !DILocation(line: 10, column: 15, scope: !16)
!25 = !DILocation(line: 11, column: 16, scope: !16)
!26 = !DILocation(line: 11, column: 7, scope: !16)
!27 = !DILocation(line: 12, column: 16, scope: !16)
!28 = !DILocation(line: 12, column: 7, scope: !16)
!29 = !DILocation(line: 13, column: 12, scope: !16)
!30 = !DILocation(line: 13, column: 15, scope: !16)
!31 = !DILocation(line: 13, column: 5, scope: !16)
!32 = !DILocation(line: 14, column: 10, scope: !16)
!33 = !DILocation(line: 14, column: 5, scope: !16)
!34 = !DILocation(line: 15, column: 9, scope: !35)
!35 = distinct !DILexicalBlock(scope: !16, file: !3, line: 15, column: 9)
!36 = !DILocation(line: 15, column: 11, scope: !35)
!37 = !DILocation(line: 15, column: 9, scope: !16)
!38 = !DILocation(line: 16, column: 14, scope: !39)
!39 = distinct !DILexicalBlock(scope: !35, file: !3, line: 15, column: 16)
!40 = !DILocation(line: 16, column: 9, scope: !39)
!41 = !DILocation(line: 17, column: 9, scope: !39)
!42 = !DILocation(line: 18, column: 5, scope: !39)
!43 = !DILocation(line: 19, column: 14, scope: !44)
!44 = distinct !DILexicalBlock(scope: !35, file: !3, line: 18, column: 11)
!45 = !DILocation(line: 19, column: 9, scope: !44)
!46 = !DILocation(line: 20, column: 9, scope: !44)
!47 = !DILocation(line: 22, column: 1, scope: !16)
!48 = distinct !DISubprogram(name: "main", scope: !3, file: !3, line: 24, type: !49, scopeLine: 24, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!49 = !DISubroutineType(types: !50)
!50 = !{!9, !9, !51}
!51 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !6, size: 64)
!52 = !DILocalVariable(name: "argc", arg: 1, scope: !48, file: !3, line: 24, type: !9)
!53 = !DILocation(line: 24, column: 14, scope: !48)
!54 = !DILocalVariable(name: "argv", arg: 2, scope: !48, file: !3, line: 24, type: !51)
!55 = !DILocation(line: 24, column: 27, scope: !48)
!56 = !DILocalVariable(name: "symvar", scope: !48, file: !3, line: 25, type: !57)
!57 = !DICompositeType(tag: DW_TAG_array_type, baseType: !7, size: 2056, elements: !58)
!58 = !{!59}
!59 = !DISubrange(count: 257)
!60 = !DILocation(line: 25, column: 6, scope: !48)
!61 = !DILocation(line: 26, column: 20, scope: !48)
!62 = !DILocation(line: 26, column: 1, scope: !48)
!63 = !DILocation(line: 27, column: 13, scope: !48)
!64 = !DILocation(line: 27, column: 24, scope: !48)
!65 = !DILocation(line: 27, column: 1, scope: !48)
!66 = !DILocation(line: 28, column: 19, scope: !48)
!67 = !DILocation(line: 28, column: 8, scope: !48)
!68 = !DILocation(line: 28, column: 1, scope: !48)
