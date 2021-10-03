//
// AUTO-GENERATED FILE - DO NOT EDIT!!
//

"options indenting = 4\n"
"options no_unused_block_arguments = false\n"
"options no_unused_function_arguments = false\n"
"options no_aot = true\n"
"\n"
"module templates shared private\n"
"\n"
"require ast\n"
"require rtti\n"
"require daslib/ast_boost\n"
"\n"
"[call_macro(name=\"decltype\")]\n"
"class DecltypeMacro : AstCallMacro\n"
"    def override visit ( prog:ProgramPtr; mod:Module?; var expr:smart_ptr<ExprCa"
"llMacro> ) : ExpressionPtr\n"
"        if expr.arguments.length !=1\n"
"            macro_error(prog,expr.at,\"decltype expecting 1 argument\")\n"
"            return [[ExpressionPtr]]\n"
"        if expr.arguments[0]._type==null || expr.arguments[0]._type.isAutoOrAlia"
"s\n"
"            macro_error(prog,expr.at,\"decltype requires fully inferred type\")\n"
"            return [[ExpressionPtr]]\n"
"        var mkt <- new [[ExprTypeDecl() at=expr.at,\n"
"            typeexpr <- clone_type(expr.arguments[0]._type)\n"
"        ]]\n"
"        return <- mkt\n"
"\n"
"class TemplateMacro : AstFunctionAnnotation\n"
"    def override transform ( var expr : smart_ptr<ExprCallFunc>; var errors : da"
"s_string ) : ExpressionPtr\n"
"        if expr.func==null\n"
"            compiling_program() |> macro_error(expr.at,\"expecting inferred funct"
"ion\")\n"
"            return expr\n"
"        let taidx = find_index_if(each(expr.func.annotations)) <| $ ( ann )\n"
"            var same = false\n"
"            peek(ann.annotation.name) <| $(st)\n"
"                same = (st == \"template\")\n"
"            return same\n"
"        if taidx == -1\n"
"            return expr\n"
"        var cfunc = expr.func\n"
"        var ffunc : FunctionPtr\n"
"        unsafe\n"
"            ffunc <- reinterpret<FunctionPtr> cfunc\n"
"        // new call\n"
"        var newcallexpr <- clone_expression(expr)\n"
"        var newcall : smart_ptr<ExprCallFunc>\n"
"        unsafe\n"
"            newcall <- reinterpret<smart_ptr<ExprCallFunc>> newcallexpr\n"
"        // new func\n"
"        var fclone <- clone_function(ffunc)\n"
"        fclone.fromGeneric = expr.func\n"
"        fclone.flags |= FunctionFlags privateFunction\n"
"        newcall.func = null\n"
"        var extra = \"\"\n"
"        for t in fclone.annotations[taidx].arguments\n"
"            var argidx = find_index_if(each(fclone.arguments)) <| $ ( farg )\n"
"                return farg.name == t.name\n"
"            if argidx==-1\n"
"                errors := \"can't find template argument {t.name} (or duplicate)\""
"\n"
"                fclone := [[FunctionPtr]]\n"
"                newcall := [[smart_ptr<ExprCallFunc>]]\n"
"                return [[ExpressionPtr]]\n"
"            var argExpr := newcall.arguments[argidx]\n"
"            if (argExpr is ExprRef2Value)\n"
"                let r2v = newcall.arguments[argidx] as ExprRef2Value\n"
"                argExpr := r2v.subexpr\n"
"            if !(argExpr is ExprTypeDecl)\n"
"                errors := \"template argument must be type<...> and not {describe"
"(newcall.arguments[argidx])}. {newcall.arguments[argidx].__rtti}\"\n"
"                fclone := [[FunctionPtr]]\n"
"                newcall := [[smart_ptr<ExprCallFunc>]]\n"
"                return [[ExpressionPtr]]\n"
"            var tname = describe_typedecl(argExpr._type,true,true,true)\n"
"            argExpr := null\n"
"            extra += \"`{hash(tname)}\"\n"
"            fclone.arguments |> erase(argidx)\n"
"            newcall.arguments |> erase(argidx)\n"
"        fclone.annotations |> erase(taidx)\n"
"        // name it\n"
"        fclone.name := \"`template{extra}`{fclone.name}\"\n"
"        newcall.name := fclone.name\n"
"        // result\n"
"        compiling_module() |> add_function(fclone)\n"
"        return newcall\n"
"\n"
"[macro]\n"
"def init\n"
"    if is_compiling_macros_in_module(\"templates\")\n"
"        add_new_function_annotation(\"template\", new TemplateMacro())\n"