#pragma once

export namespace stdx::compiler::clang {
#ifdef STDLIBX_EXTENSIONS_COMPILE_COMPILER_CLANG_LIBRARY
    using ::clang::APINotesOptions;

    namespace api_notes {
        using ::clang::api_notes::APINotesManager;
        using ::clang::api_notes::APINotesReader;
        using ::clang::api_notes::APINotesWriter;
        using ::clang::api_notes::RetainCountConventionKind;
        using ::clang::api_notes::EnumExtensibilityKind;
        using ::clang::api_notes::SwiftNewTypeKind;
        using ::clang::api_notes::CommonEntityInfo;
        using ::clang::api_notes::CommonTypeInfo;
        using ::clang::api_notes::ContextInfo;
        using ::clang::api_notes::VariableInfo;
        using ::clang::api_notes::ObjCPropertyInfo;
        using ::clang::api_notes::ParamInfo;
        using ::clang::api_notes::FunctionInfo;
        using ::clang::api_notes::ObjCMethodInfo;
        using ::clang::api_notes::GlobalVariableInfo;
        using ::clang::api_notes::GlobalFunctionInfo;
        using ::clang::api_notes::CXXMethodInfo;
        using ::clang::api_notes::EnumConstantInfo;
        using ::clang::api_notes::TagInfo;
        using ::clang::api_notes::TypedefInfo;
        
        inline constexpr char SOURCE_APINOTES_EXTENSION[] = "apinotes";

        using ::clang::api_notes::ContextID;
        using ::clang::api_notes::ContextKind;
        using ::clang::api_notes::Context;
        using ::clang::api_notes::ObjCSelectorRef;

        using ::clang::api_notes::operator==;
        using ::clang::api_notes::operator!=;

        using ::clang::api_notes::parseAndDumpAPINotes;
        using ::clang::api_notes::compileAPINotes;
    }
#endif
}
