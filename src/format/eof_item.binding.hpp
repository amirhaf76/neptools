// Auto generated code, do not edit. See gen_binding in project root.
#ifndef NEPTOOLS_WITHOUT_LUA
#include "lua/user_type.hpp"


const char ::Neptools::EofItem::TYPE_NAME[] = "neptools.eof_item";

namespace Neptools::Lua
{

// class neptools.eof_item
template<>
void TypeRegisterTraits<::Neptools::EofItem>::Register(TypeBuilder& bld)
{
    bld.Inherit<::Neptools::EofItem, ::Neptools::Item>();

    bld.AddFunction<
        &::Neptools::Lua::TypeTraits<::Neptools::EofItem>::Make<LuaGetRef<::Neptools::Item::Key>, LuaGetRef<::Neptools::Context &>>
    >("new");

}
static TypeRegister::StateRegister<::Neptools::EofItem> reg_neptools_eof_item;

}
#endif
