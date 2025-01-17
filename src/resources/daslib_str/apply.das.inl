//
// AUTO-GENERATED FILE - DO NOT EDIT!!
//

"options indenting = 4\n"
"options no_unused_block_arguments = false\n"
"options no_unused_function_arguments = false\n"
"options no_aot = true\n"
"\n"
"module apply shared private\n"
"\n"
"require ast\n"
"require daslib/ast_boost\n"
"\n"
"/*\n"
"    def apply`Foo(self:Foo;arg_field1:block<(name:string,value:field1-type):void"
">;arg_field2:...)\n"
"        invoke(arg_field1,\"field1\",self.field1)\n"
"        invoke(arg_field2,\"field2\",self.field2)\n"
"        ...\n"
"*/\n"
"[macro_function]\n"
"def generateApplyVisitStruct ( stype:TypeDeclPtr; fnname:string; at:LineInfo )\n"
"    assert(stype.baseType==Type tStructure)\n"
"    assert(stype.dim.length==0)\n"
"    var fn <- new [[Function() at=at, atDecl=at, name:=fnname]]                 "
"    // def apply`foo : void\n"
"    fn.flags |= FunctionFlags generated\n"
"    fn.flags |= FunctionFlags privateFunction\n"
"    fn.result <- new [[TypeDecl() baseType=Type autoinfer, at=at]]\n"
"    var selfT <- clone_type(stype)\n"
"    selfT.flags |= TypeDeclFlags isExplicit\n"
"    selfT.flags |= TypeDeclFlags explicitConst\n"
"    emplace_new(fn.arguments, new [[Variable() name:=\"__self\",at=at, _type <- se"
"lfT]])   // self:Foo\n"
"    var blk <- new [[ExprBlock() at=at]]\n"
"    for fld in stype.structType.fields\n"
"        var argT <- new [[TypeDecl() baseType=Type tBlock, at=at]]              "
"           // block<(name:string;x:field1type)>\n"
"        emplace_new(argT.argTypes, new [[TypeDecl() baseType=Type tString, at=at"
"]])\n"
"        var fldT <- clone_type(fld._type)\n"
"        if stype.flags.constant\n"
"            fldT.flags |= TypeDeclFlags constant\n"
"        emplace(argT.argTypes, fldT)\n"
"        argT.firstType <- new [[TypeDecl() baseType=Type tVoid,at=at]]\n"
"        argT.flags = TypeDeclFlags constant\n"
"        var argF <- new [[Variable() name:=\"__arg_{fld.name}\",at=at,_type <- arg"
"T]]   // arg_field1:block...\n"
"        emplace(fn.arguments,argF)\n"
"        var cfield <- new [[ExprInvoke() at=at]]                                "
"    // invoke()\n"
"        var argb <- new [[ExprVar() at=at, name:=\"__arg_{fld.name}\"]]           "
"      // arg_field1\n"
"        emplace(cfield.arguments,argb)\n"
"        var argn <- new [[ExprConstString() at=at, value:=fld.name]]            "
"    // \"name\"\n"
"        emplace(cfield.arguments,argn)\n"
"        var argvar <- new [[ExprVar() at=at, name:=\"__self\"]]                   "
"    // self\n"
"        var argfld <- new [[ExprField() at=at, name:=fld.name, value <- argvar]]"
"    // self.name\n"
"        emplace(cfield.arguments,argfld)\n"
"        emplace(blk.list, cfield)\n"
"    fn.body <- blk\n"
"    compiling_module() |> add_function(fn)\n"
"\n"
"[call_macro(name=\"apply\")]  // apply(value, block)\n"
"class ApplyMacro : AstCallMacro\n"
"    def override visit ( prog:ProgramPtr; mod:Module?; var expr:smart_ptr<ExprCa"
"llMacro> ) : ExpressionPtr\n"
"        if expr.arguments.length != 2\n"
"            macro_error(prog,expr.at,\"expecting apply(value, block)\")\n"
"            return [[ExpressionPtr]]\n"
"        if expr.arguments[0]._type!=null    // need value inferred\n"
"            if expr.arguments[0]._type.dim.length != 0\n"
"                macro_error(prog,expr.at,\"can't apply to dim\")\n"
"                return [[ExpressionPtr]]\n"
"            if expr.arguments[0]._type.baseType != Type tStructure\n"
"                macro_error(prog,expr.at,\"can only apply to structure\")\n"
"                return [[ExpressionPtr]]\n"
"            if !(expr.arguments[1] is ExprMakeBlock)\n"
"                macro_error(prog,expr.at,\"expecting make block, i.e. $(..)\")\n"
"            var callName = \"apply`{expr.arguments[0]._type.structType.name}\"\n"
"            generateApplyVisitStruct(expr.arguments[0]._type,callName,expr.at)\n"
"            // make a call\n"
"            var call <- new [[ExprCall() name:=\"_::\" + callName, at=expr.at]]\n"
"            emplace_new(call.arguments,clone_expression(expr.arguments[0]))\n"
"            let nfields = expr.arguments[0]._type.structType.fields.length\n"
"            for i in range(nfields)\n"
"                emplace_new(call.arguments,clone_expression(expr.arguments[1]))\n"
"            return <- call\n"
"        return [[ExpressionPtr]]\n"
"\n"
"\n"
"\n"
"\n"
